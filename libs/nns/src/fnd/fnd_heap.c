// NitroSystem foundation heaps: expanded heap, frame heap, unit heap, allocators. 0x02030b7c-0x02031418
#include <nnsys/private/nns_types.h>

typedef struct MBlock {
    u16 magic;
    u16 attr;
    u32 size;
    struct MBlock *prev;
    struct MBlock *next;
} MBlock;
typedef struct { MBlock *head; MBlock *tail; } MBList;
typedef struct { u32 start; u32 end; } MBRegion;
typedef struct { MBList freeList; MBList usedList; u16 group; u16 flags; } ExpBody;
typedef struct { NNSFndHeapHead head; ExpBody body; } ExpHeap;
#define HEAD_OF(b) ((NNSFndHeapHead *)((u8 *)(b) - 0x24))

static inline u16 GetAlign(MBlock *b) { return (b->attr >> 8) & 0x7f; }
static inline void SetDir(MBlock *b, u16 d) { b->attr &= ~0x8000; b->attr |= (u16)(d << 15); }
static inline void SetAlign(MBlock *b, u16 a) { b->attr &= ~0x7f00; b->attr |= (a & 0x7f) << 8; }
static inline void SetGroup(MBlock *b, u8 g) { b->attr &= ~0xff; b->attr |= g & 0xff; }

static inline u16 GetAllocMode(ExpHeap *h) { return h->body.flags & 1; }
static inline u16 GetOpt(NNSFndHeapHead *h) { return h->opts & 0xff; }
void GetMBlockRegion(MBRegion *r, MBlock *b) {   // block's region incl. alignment padding
    r->start = (u32)b - GetAlign(b);
    r->end = b->size + ((u32)b + 0x10);
}
MBlock *UnlinkMBlock(MBList *l, MBlock *b) {   // unlink, returns previous
    MBlock *prev = b->prev;
    MBlock *next = b->next;
    if (prev) prev->next = next; else l->head = next;
    if (next) next->prev = prev; else l->tail = prev;
    return prev;
}
MBlock *LinkMBlockAfter(MBList *l, MBlock *b, MBlock *prev) {   // link after prev
    MBlock *next;
    b->prev = prev;
    if (prev) { next = prev->next; prev->next = b; }
    else { next = l->head; l->head = b; }
    b->next = next;
    if (next) next->prev = b; else l->tail = b;
    return b;
}
MBlock *InitMBlockAtRegionStart(MBRegion *r, u16 sig) {   // make a block header at the start of a region
    MBlock *b = (MBlock *)r->start;
    b->magic = sig;
    b->attr = 0;
    b->size = r->end - ((u32)b + 0x10);
    b->prev = 0;
    b->next = 0;
    return b;
}
u32 CarveUsedMBlock(ExpBody *h, MBlock *freeBlk, u32 mem, u32 size, u16 dir) {
    MBRegion front, rear, used;
    MBlock *prev, *b;
    u32 head;
    u32 tail;
    u32 rearEnd;
    GetMBlockRegion(&front, freeBlk);
    head = mem - 0x10;
    tail = size + mem;
    rearEnd = front.end;
    front.end = head;
    rear.end = rearEnd;
    rear.start = tail;
    prev = UnlinkMBlock(&h->freeList, freeBlk);
    if (front.end - front.start < 0x14) front.end = front.start;
    else prev = LinkMBlockAfter(&h->freeList, InitMBlockAtRegionStart(&front, 0x4652), prev);
    if (rear.end - rear.start < 0x14) rear.start = rear.end;
    else LinkMBlockAfter(&h->freeList, InitMBlockAtRegionStart(&rear, 0x4652), prev);
    {
        void *clr = (void *)front.end;
        u32 n = rear.start - front.end;
        if (GetOpt(HEAD_OF(h)) & 1) MIi_CpuClear32(0, clr, n);
    }
    used.start = head;
    used.end = rear.start;
    b = InitMBlockAtRegionStart(&used, 0x5544);
    SetDir(b, dir);
    SetAlign(b, (u32)b - front.end);
    SetGroup(b, h->group);
    LinkMBlockAfter(&h->usedList, b, h->usedList.tail);
    return mem;
}

void *AllocFromExpFreeListHead(ExpHeap *h, u32 size, u32 align) {   // allocate from the head end
    MBlock *b;
    BOOL firstFit = GetAllocMode(h) == 0;
    MBlock *best = 0;
    u32 bestSize = 0xffffffff;
    u32 bestAddr = 0;
    for (b = h->body.freeList.head; b; b = b->next) {
        u32 start = (u32)b + 0x10;
        u32 addr = (align - 1 + start) & ~(align - 1);
        if (b->size < size + (addr - start)) continue;
        if (bestSize > b->size) {
            best = b; bestSize = b->size; bestAddr = addr;
            if (firstFit) break;
            if (b->size == size) break;
        }
    }
    if (!best) return 0;
    return (void *)CarveUsedMBlock(&h->body, best, bestAddr, size, 0);
}
void *AllocFromExpFreeListTail(ExpHeap *h, u32 size, u32 align) {   // allocate from the tail end
    MBlock *b;
    BOOL firstFit = GetAllocMode(h) == 0;
    MBlock *best = 0;
    u32 bestSize = 0xffffffff;
    u32 mask;
    u32 bestAddr = 0;
    for (b = h->body.freeList.tail; b; b = b->prev) {
        u32 start = (u32)b + 0x10;
        u32 addr = (b->size + start - size) & ~(align - 1);
        if ((int)(addr - start) < 0) continue;
        if (bestSize > b->size) {
            best = b; bestSize = b->size; bestAddr = addr;
            if (firstFit) break;
            if (b->size == size) break;
        }
    }
    if (!best) return 0;
    return (void *)CarveUsedMBlock(&h->body, best, bestAddr, size, 1);
}

BOOL MergeRegionIntoFreeList(ExpBody *h, MBRegion *r) {   // give a region back to the free list, merging neighbours
    MBRegion reg = *r;
    MBlock *prev = 0;
    MBlock *b;
    for (b = h->freeList.head; b; b = b->next) {
        if ((u32)b < r->start) { prev = b; continue; }
        if ((u32)b == r->end) {
            reg.end = b->size + ((u32)b + 0x10);
            UnlinkMBlock(&h->freeList, b);
        }
        break;
    }
    if (prev) {
        u32 pe = prev->size + ((u32)prev + 0x10);
        if (pe == r->start) {
            reg.start = (u32)prev;
            prev = UnlinkMBlock(&h->freeList, prev);
        }
    }
    if (reg.end - reg.start < 0x10) return 0;
    LinkMBlockAfter(&h->freeList, InitMBlockAtRegionStart(&reg, 0x4652), prev);
    return 1;
}
void *ExpHeapAlloc(ExpHeap *h, u32 size, s32 align) {
    if (size == 0) size = 1;
    size = (size + 3) & ~3;
    if (align >= 0) return AllocFromExpFreeListHead(h, size, align);
    return AllocFromExpFreeListTail(h, size, -align);
}
void ExpHeapFree(ExpHeap *h, void *p) {
    MBlock *b = (MBlock *)((u8 *)p - 0x10);
    MBRegion reg;
    GetMBlockRegion(&reg, b);
    UnlinkMBlock(&h->body.usedList, b);
    MergeRegionIntoFreeList(&h->body, &reg);
}

typedef struct FrmState { u32 tag; void *head; void *tail; struct FrmState *next; } FrmState;
typedef struct { void *head; void *tail; FrmState *state; } FrmBody;
typedef struct { NNSFndHeapHead head; FrmBody body; } FrmHeap;

FrmHeap *InitFrmHeapAt(FrmHeap *h, void *end, u16 opt) {   // create a frame heap over [h, end)
    InitAndLinkHeapHead(&h->head, 0x46524d48, (u8 *)h + 0x30, end, opt);
    h->body.head = h->head.start;
    h->body.tail = h->head.end;
    h->body.state = 0;
    return h;
}
void *AllocAtFrmHead(FrmBody *f, u32 size, u32 align) {   // allocate from the head end
    u32 oldHead = (u32)f->head;
    u32 start = (align - 1 + oldHead) & ~(align - 1);
    u32 newHead = size + start;
    u32 n;
    if (newHead > (u32)f->tail) return 0;
    n = newHead - oldHead;
    if (GetOpt((NNSFndHeapHead *)((u8 *)f - 0x24)) & 1) MIi_CpuClear32(0, (void *)oldHead, n);
    f->head = (void *)newHead;
    return (void *)start;
}
void *AllocAtFrmTail(FrmBody *f, u32 size, u32 align) {   // allocate from the tail end
    u32 tail = (u32)f->tail;
    u32 start = (tail - size) & ~(align - 1);
    u32 n;
    if (start < (u32)f->head) return 0;
    n = tail - start;
    if (GetOpt((NNSFndHeapHead *)((u8 *)f - 0x24)) & 1) MIi_CpuClear32(0, (void *)start, n);
    f->tail = (void *)start;
    return (void *)start;
}
void ResetFrmHeadAndStates(FrmHeap *h) {   // free the head end
    h->body.head = h->head.start;
    h->body.state = 0;
}
void ResetFrmTailAndStateTails(FrmHeap *h) {   // free the tail end
    FrmState *s;
    for (s = h->body.state; s; s = s->next) s->tail = h->head.end;
    h->body.tail = h->head.end;
}
FrmHeap *CreateFrmHeapInRange(u32 start, u32 size, u16 opt) {
    u32 end = (size + start) & ~3;
    start = (start + 3) & ~3;
    if (start > end || end - start < 0x30) return 0;
    return InitFrmHeapAt((FrmHeap *)start, (void *)end, opt);
}
void UnlinkFrmHeap(NNSFndHeapHead *h) { UnlinkHeapHead(h); }   // destroy
void *FrmHeapAlloc(FrmHeap *h, u32 size, s32 align) {
    if (size == 0) size = 1;
    size = (size + 3) & ~3;
    if (align >= 0) return AllocAtFrmHead(&h->body, size, align);
    return AllocAtFrmTail(&h->body, size, -align);
}
void ResetFrmHeapByMask(FrmHeap *h, u32 mode) {
    if (mode & 1) ResetFrmHeadAndStates(h);
    if (mode & 2) ResetFrmTailAndStateTails(h);
}
BOOL PushFrmState(FrmHeap *h, u32 tag) {
    void *oldHead = h->body.head;
    FrmState *s = AllocAtFrmHead(&h->body, 0x10, 4);
    if (!s) return 0;
    s->tag = tag;
    s->head = oldHead;
    s->tail = h->body.tail;
    s->next = h->body.state;
    h->body.state = s;
    return 1;
}
BOOL RestoreFrmState(FrmHeap *h, u32 tag) {
    FrmState *s = h->body.state;
    if (tag != 0 && s != 0) {
        do {
            if (s->tag == tag) break;
            s = s->next;
        } while (s != 0);
    }
    if (!s) return 0;
    h->body.head = s->head;
    h->body.tail = s->tail;
    h->body.state = s->next;
    return 1;
}

typedef struct { NNSFndHeapHead head; void *freeList; u32 unitSize; } UnitHeap;
void *PopFreeNode(void **list) {   // pop a unit from the free list
    void *n = *list;
    if (n) *list = *(void **)n;
    return n;
}
void *UnitHeapAlloc(UnitHeap *h) {
    void *n;
    n = PopFreeNode(&h->freeList);
    if (n) { u32 sz = h->unitSize; if (GetOpt(&h->head) & 1) MIi_CpuClear32(0, n, sz); }
    return n;
}
void UnitHeapFree(UnitHeap *h, void *blk) {
    *(void **)blk = h->freeList;
    h->freeList = blk;
}

typedef struct { const void *func; void *heap; s32 param1; s32 param2; } NNSFndAllocator;
void *ExpAllocatorAlloc(NNSFndAllocator *a, u32 size) { return ExpHeapAlloc(a->heap, size, a->param1); }
void ExpAllocatorFree(NNSFndAllocator *a, void *p) { ExpHeapFree(a->heap, p); }
void *FrmAllocatorAlloc(NNSFndAllocator *a, u32 size) { return FrmHeapAlloc(a->heap, size, a->param1); }
void func_0203138c(void) {}
void *UnitAllocatorAlloc(NNSFndAllocator *a, u32 size) {
    UnitHeap *h = a->heap;
    if (size > h->unitSize) return 0;
    return UnitHeapAlloc(h);
}
void UnitAllocatorFree(NNSFndAllocator *a, void *p) { UnitHeapFree(a->heap, p); }
void *OSHeapAllocatorAlloc(NNSFndAllocator *a, u32 size) { return OS_AllocFromHeap(a->param1, (int)a->heap, size); }
void OSHeapAllocatorFree(NNSFndAllocator *a, void *p) { OS_FreeToHeap(a->param1, (int)a->heap, p); }
extern const u8 data_02090a58[];
void InitOSHeapAllocator(NNSFndAllocator *a, s32 arena, void *heap) {   // allocator over an OS heap
    a->func = data_02090a58;
    a->heap = heap;
    a->param1 = arena;
    a->param2 = 0;
}
