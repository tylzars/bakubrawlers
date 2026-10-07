#pragma once

#include <stddef.h>

// Fixed-width integer types, matching NitroSDK naming.
typedef unsigned char u8;
typedef unsigned short u16;
typedef unsigned int u32;
typedef unsigned long long u64;
typedef signed char s8;
typedef signed short s16;
typedef signed int s32;
typedef signed long long s64;

typedef volatile u8 vu8;
typedef volatile u16 vu16;
typedef volatile u32 vu32;
typedef volatile u64 vu64;
typedef volatile s8 vs8;
typedef volatile s16 vs16;
typedef volatile s32 vs32;
typedef volatile s64 vs64;

// Placeholders for values whose real type isn't known yet.
typedef s8 unk8;
typedef s16 unk16;
typedef s32 unk32;

typedef s32 BOOL;
#define TRUE 1
#define FALSE 0

#ifndef NULL
    #define NULL 0
#endif

// A 2D point of signed 16-bit coordinates.
struct Vec2s {
    s16 x;
    s16 y;
};

// fx32 and friends: NitroSDK fixed point, 12 fractional bits.
typedef s16 fx16;
typedef s32 fx32;
typedef s64 fx64;
