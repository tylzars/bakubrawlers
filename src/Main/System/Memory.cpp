#include <nitro/os.h>

extern "C" void *OS_GetArenaLo(int id);
extern "C" void *OS_GetArenaHi(int id);
extern "C" void *OS_AllocFromArenaHi(int id, u32 size, u32 align);
extern "C" void *OS_InitAlloc(int id, void *lo, void *hi, int maxHeaps);
extern "C" void OS_SetArenaLo(int id, void *lo);
extern "C" void *OS_AllocFromArenaLo(int id, u32 size, u32 align);
extern "C" s32 OS_CreateHeap(int id, void *lo, void *hi);
extern "C" s32 OS_SetCurrentHeap(int id, s32 heap);
extern "C" void OS_DestroyHeap(int id, s32 heap);
extern "C" void OS_ClearAlloc(int id);
extern "C" void OS_CheckHeap(int id, s32 heap);
extern "C" void OS_AddToHeap(int id, s32 heap, void *lo, void *hi);

struct GameHeap {
    s32 mHandle;
    u8 *mStart;
    u32 mSize;
};

extern GameHeap gGameHeap;

static inline void CreateGameHeap() {
    void *lo = OS_GetArenaLo(0);
    void *hi = OS_GetArenaHi(0);
    OS_SetArenaLo(0, OS_InitAlloc(0, lo, hi, 1));
    u8 *top        = (u8 *) OS_GetArenaHi(0);
    u8 *bottom     = (u8 *) OS_GetArenaLo(0);
    gGameHeap.mSize = (u32) (top - bottom) & ~0xf;
    gGameHeap.mStart = (u8 *) OS_AllocFromArenaLo(0, gGameHeap.mSize, 0x20);
    s32 handle = OS_CreateHeap(0, gGameHeap.mStart, gGameHeap.mStart + gGameHeap.mSize);
    gGameHeap.mHandle = handle;
    OS_SetCurrentHeap(0, handle);
    OS_DestroyHeap(0, gGameHeap.mHandle);
}

// Returns the block reserved from the high arena first (`reserve` rounded up to 0x20; NULL for 0), then creates
// the game heap.
void *InitGameHeap(u32 reserve) {
    void *block;
    if (reserve == 0) {
        block = 0;
    } else {
        block = OS_AllocFromArenaHi(0, (reserve + 0x1f) & ~0x1f, 0x20);
    }
    CreateGameHeap();
    return block;
}

void RecreateGameHeap(void *lo) {
    OS_ClearAlloc(0);
    OS_SetArenaLo(0, lo);
    CreateGameHeap();
}

s32 GetGameHeapHandle() {
    return gGameHeap.mHandle;
}

void AddGameHeapBlock() {
    OS_CheckHeap(0, gGameHeap.mHandle);
    OS_AddToHeap(0, gGameHeap.mHandle, gGameHeap.mStart, gGameHeap.mStart + gGameHeap.mSize);
    OS_CheckHeap(0, gGameHeap.mHandle);
}

void *operator new(size_t size) {
    return OS_Alloc(size);
}

void *operator new[](size_t size) {
    return OS_Alloc(size);
}

void operator delete(void *ptr) {
    if (ptr != 0) {
        OS_Free(ptr);
    }
}

void operator delete[](void *ptr) {
    if (ptr != 0) {
        OS_Free(ptr);
    }
}
