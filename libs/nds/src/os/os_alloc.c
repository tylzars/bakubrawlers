#include <nitro/private/os_types.h>

typedef s32 OSArenaId;
typedef s32 OSHeapHandle;

typedef struct Cell {
    struct Cell *prev;   // 0x00
    struct Cell *next;   // 0x04
    s32 size;            // 0x08
} Cell;

typedef struct {
    s32 size;            // 0x00 (-1 when the slot is unused)
    Cell *free;          // 0x04
    Cell *allocated;     // 0x08
} HeapDesc;

typedef struct {
    OSHeapHandle current; // 0x00
    s32 numHeaps;         // 0x04
    void *arenaStart;     // 0x08
    void *arenaEnd;       // 0x0c
    HeapDesc *heapArray;  // 0x10
} HeapInfo;               // 0x14, the descriptors follow

extern HeapInfo *heapTable[];   // 0x020bdf48

Cell *DLAddFront(Cell *list, Cell *cell) {
    cell->next = list;
    cell->prev = NULL;
    if (list != NULL) {
        list->prev = cell;
    }
    return cell;
}

Cell *DLExtract(Cell *list, Cell *cell) {
    if (cell->next != NULL) {
        cell->next->prev = cell->prev;
    }
    if (cell->prev == NULL) {
        list = cell->next;
    } else {
        cell->prev->next = cell->next;
    }
    return list;
}

Cell *DLInsert(Cell *list, Cell *cell) {
    Cell *prev = NULL;
    Cell *next = list;
    while (next != NULL) {
        if (cell <= next) {
            break;
        }
        prev = next;
        next = next->next;
    }
    cell->prev = prev;
    cell->next = next;
    if (next != NULL) {
        next->prev = cell;
        if ((u8 *) cell + cell->size == (u8 *) next) {
            cell->size += next->size;
            next = next->next;
            cell->next = next;
            if (next != NULL) {
                next->prev = cell;
            }
        }
    }
    if (prev != NULL) {
        prev->next = cell;
        if ((u8 *) prev + prev->size == (u8 *) cell) {
            prev->size += cell->size;
            prev->next = next;
            if (next != NULL) {
                next->prev = prev;
            }
        }
        return list;
    }
    return cell;
}

void *OS_AllocFromHeap(OSArenaId id, OSHeapHandle heap, u32 size) {
    HeapInfo *info;
    HeapDesc *hd;
    Cell *cell;
    u32 left;
    OSIntrMode e = OS_DisableInterrupts();
    info = heapTable[id];
    if (info == NULL) {
        OS_RestoreInterrupts(e);
        return NULL;
    }
    if (heap < 0) {
        heap = info->current;
    }
    hd = &info->heapArray[heap];
    size = (size + 0x3f) & ~0x1f;
    for (cell = hd->free; cell != NULL; cell = cell->next) {
        if ((s32) size <= cell->size) {
            break;
        }
    }
    if (cell == NULL) {
        OS_RestoreInterrupts(e);
        return NULL;
    }
    left = cell->size - size;
    if (left < 0x40) {
        hd->free = DLExtract(hd->free, cell);
    } else {
        Cell *rest;
        cell->size = size;
        rest = (Cell *) ((u8 *) cell + size);
        rest->size = left;
        rest->prev = cell->prev;
        rest->next = cell->next;
        if (rest->next != NULL) {
            rest->next->prev = rest;
        }
        if (rest->prev != NULL) {
            rest->prev->next = rest;
        } else {
            hd->free = rest;
        }
    }
    hd->allocated = DLAddFront(hd->allocated, cell);
    OS_RestoreInterrupts(e);
    return (u8 *) cell + 0x20;
}

void OS_FreeToHeap(OSArenaId id, OSHeapHandle heap, void *ptr) {
    HeapInfo *info;
    HeapDesc *hd;
    Cell *cell;
    OSIntrMode e = OS_DisableInterrupts();
    info = heapTable[id];
    if (heap < 0) {
        heap = info->current;
    }
    hd = &info->heapArray[heap];
    cell = (Cell *) ((u8 *) ptr - 0x20);
    hd->allocated = DLExtract(hd->allocated, cell);
    hd->free = DLInsert(hd->free, cell);
    OS_RestoreInterrupts(e);
}

OSHeapHandle OS_SetCurrentHeap(OSArenaId id, OSHeapHandle heap) {
    OSIntrMode e = OS_DisableInterrupts();
    HeapInfo *info = heapTable[id];
    OSHeapHandle old = info->current;
    info->current = heap;
    OS_RestoreInterrupts(e);
    return old;
}

void *OS_InitAlloc(OSArenaId id, void *lo, void *hi, int maxHeaps) {
    OSIntrMode e = OS_DisableInterrupts();
    HeapInfo *info = (HeapInfo *) lo;
    int i;
    u32 arraySize;
    heapTable[id] = info;
    info->heapArray = (HeapDesc *) ((u8 *) info + sizeof(HeapInfo));
    arraySize = maxHeaps * sizeof(HeapDesc);
    info->numHeaps = maxHeaps;
    for (i = 0; i < info->numHeaps; i++) {
        HeapDesc *hd = &info->heapArray[i];
        hd->size = -1;
        hd->allocated = NULL;
        hd->free = NULL;
    }
    info->current = -1;
    info->arenaStart = (void *) (((u32) info->heapArray + arraySize + 0x1f) & ~0x1f);
    info->arenaEnd = (void *) ((u32) hi & ~0x1f);
    OS_RestoreInterrupts(e);
    return info->arenaStart;
}

void OS_ClearAlloc(OSArenaId id) {
    heapTable[id] = NULL;
}

OSHeapHandle OS_CreateHeap(OSArenaId id, void *lo, void *hi) {
    OSIntrMode e = OS_DisableInterrupts();
    HeapInfo *info = heapTable[id];
    HeapDesc *hd;
    int i;
    lo = (void *) (((u32) lo + 0x1f) & ~0x1f);
    hi = (void *) ((u32) hi & ~0x1f);
    for (i = 0; i < info->numHeaps; i++) {
        hd = &info->heapArray[i];
        if (hd->size < 0) {
            Cell *c = (Cell *) lo;
            hd->size = (u32) hi - (u32) lo;
            c->prev = NULL;
            c->next = NULL;
            c->size = hd->size;
            hd->free = c;
            hd->allocated = NULL;
            OS_RestoreInterrupts(e);
            return i;
        }
    }
    OS_RestoreInterrupts(e);
    return -1;
}

void OS_DestroyHeap(OSArenaId id, OSHeapHandle heap) {
}

s32 OS_CheckHeap(OSArenaId id, OSHeapHandle heap) {
    s32 total = 0;
    OSIntrMode e = OS_DisableInterrupts();
    HeapInfo *info = heapTable[id];
    Cell *cell;
    if (heap < 0) {
        heap = info->current;
    }
    for (cell = info->heapArray[heap].free; cell != NULL; cell = cell->next) {
        total += cell->size - 0x20;
    }
    OS_RestoreInterrupts(e);
    return total;
}

void OS_AddToHeap(OSArenaId id, OSHeapHandle heap, void *lo, void *hi) {
    OSIntrMode e = OS_DisableInterrupts();
    HeapInfo *info = heapTable[id];
    HeapDesc *hd;
    Cell *c;
    lo = (void *) (((u32) lo + 0x1f) & ~0x1f);
    hi = (void *) ((u32) hi & ~0x1f);
    if (heap < 0) {
        heap = info->current;
    }
    hd = &info->heapArray[heap];
    hd->size = (u32) hi - (u32) lo;
    c = (Cell *) lo;
    c->prev = NULL;
    c->next = NULL;
    c->size = hd->size;
    hd->free = c;
    hd->allocated = NULL;
    OS_RestoreInterrupts(e);
}
