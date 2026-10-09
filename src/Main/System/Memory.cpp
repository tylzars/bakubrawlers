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

extern GameHeap data_020d4300;

static inline void CreateGameHeap() {
    void *lo = OS_GetArenaLo(0);
    void *hi = OS_GetArenaHi(0);
    OS_SetArenaLo(0, OS_InitAlloc(0, lo, hi, 1));
    u8 *top        = (u8 *) OS_GetArenaHi(0);
    u8 *bottom     = (u8 *) OS_GetArenaLo(0);
    data_020d4300.mSize = (u32) (top - bottom) & ~0xf;
    data_020d4300.mStart = (u8 *) OS_AllocFromArenaLo(0, data_020d4300.mSize, 0x20);
    s32 handle = OS_CreateHeap(0, data_020d4300.mStart, data_020d4300.mStart + data_020d4300.mSize);
    data_020d4300.mHandle = handle;
    OS_SetCurrentHeap(0, handle);
    OS_DestroyHeap(0, data_020d4300.mHandle);
}

void *func_02021c60(u32 reserve) {
    void *block;
    if (reserve == 0) {
        block = 0;
    } else {
        block = OS_AllocFromArenaHi(0, (reserve + 0x1f) & ~0x1f, 0x20);
    }
    CreateGameHeap();
    return block;
}

void func_02021d38(void *lo) {
    OS_ClearAlloc(0);
    OS_SetArenaLo(0, lo);
    CreateGameHeap();
}

s32 func_02021e00() {
    return data_020d4300.mHandle;
}

void func_02021e10() {
    OS_CheckHeap(0, data_020d4300.mHandle);
    OS_AddToHeap(0, data_020d4300.mHandle, data_020d4300.mStart, data_020d4300.mStart + data_020d4300.mSize);
    OS_CheckHeap(0, data_020d4300.mHandle);
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
