#ifndef NNS_TYPES_H
#define NNS_TYPES_H
typedef unsigned char u8; typedef signed char s8;
typedef unsigned short u16; typedef short s16;
typedef unsigned int u32; typedef int s32;
typedef int BOOL;
typedef struct { void *prev; void *next; } NNSFndLink;
typedef struct { void *head; void *tail; u16 count; u16 offset; } NNSFndList;
typedef struct {
    u32 magic; NNSFndLink sibling; NNSFndList children;
    void *start; void *end; u32 opts;
} NNSFndHeapHead;
extern void MIi_CpuClear32(u32 data, void *dest, u32 size);
extern void *OS_AllocFromHeap(int arena, int heap, u32 size);
extern void OS_FreeToHeap(int arena, int heap, void *ptr);
#endif
