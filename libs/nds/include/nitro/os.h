#pragma once

#include "types.h"

#ifdef __cplusplus
extern "C" {
#endif

// Heap allocation (os_alloc). Verified against the ROM: OS_AllocFromHeap 0x0200645c, OS_FreeToHeap 0x02006564.
typedef enum {
    OS_ARENA_MAIN = 0,
} OSArenaId;

typedef s32 OSHeapHandle;
#define OS_CURRENT_HEAP_HANDLE ((OSHeapHandle) -1)

void *OS_AllocFromHeap(OSArenaId id, OSHeapHandle heap, u32 size);
void OS_FreeToHeap(OSArenaId id, OSHeapHandle heap, void *ptr);

#define OS_Alloc(size) OS_AllocFromHeap(OS_ARENA_MAIN, OS_CURRENT_HEAP_HANDLE, (size))
#define OS_Free(ptr) OS_FreeToHeap(OS_ARENA_MAIN, OS_CURRENT_HEAP_HANDLE, (ptr))

#ifdef __cplusplus
}
#endif
