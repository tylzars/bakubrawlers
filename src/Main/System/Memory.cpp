#include <nitro/os.h>

extern void *func_020060e0(int id);
extern void *func_020060cc(int id);
extern void *func_02006314(int id, u32 size, u32 align);
extern void *func_020065fc(int id, void *lo, void *hi, int maxHeaps);
extern void func_02006294(int id, void *lo);
extern void *func_020062a8(int id, u32 size, u32 align);
extern s32 func_020066b8(int id, void *lo, void *hi);
extern s32 func_020065cc(int id, s32 heap);
extern void func_02006750(int id, s32 heap);
extern void func_020066a4(int id);
extern void func_02006754(int id, s32 heap);
extern void func_020067b8(int id, s32 heap, void *lo, void *hi);

struct GameHeap {
    s32 mHandle;
    u8 *mStart;
    u32 mSize;
};

extern GameHeap data_020d4300;

static inline void CreateGameHeap() {
    void *lo = func_020060e0(0);
    void *hi = func_020060cc(0);
    func_02006294(0, func_020065fc(0, lo, hi, 1));
    u8 *top        = (u8 *) func_020060cc(0);
    u8 *bottom     = (u8 *) func_020060e0(0);
    data_020d4300.mSize = (u32) (top - bottom) & ~0xf;
    data_020d4300.mStart = (u8 *) func_020062a8(0, data_020d4300.mSize, 0x20);
    s32 handle = func_020066b8(0, data_020d4300.mStart, data_020d4300.mStart + data_020d4300.mSize);
    data_020d4300.mHandle = handle;
    func_020065cc(0, handle);
    func_02006750(0, data_020d4300.mHandle);
}

void *func_02021c60(u32 reserve) {
    void *block;
    if (reserve == 0) {
        block = 0;
    } else {
        block = func_02006314(0, (reserve + 0x1f) & ~0x1f, 0x20);
    }
    CreateGameHeap();
    return block;
}

void func_02021d38(void *lo) {
    func_020066a4(0);
    func_02006294(0, lo);
    CreateGameHeap();
}

s32 func_02021e00() {
    return data_020d4300.mHandle;
}

void func_02021e10() {
    func_02006754(0, data_020d4300.mHandle);
    func_020067b8(0, data_020d4300.mHandle, data_020d4300.mStart, data_020d4300.mStart + data_020d4300.mSize);
    func_02006754(0, data_020d4300.mHandle);
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
