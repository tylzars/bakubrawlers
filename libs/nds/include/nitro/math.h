#pragma once

#include "types.h"

typedef struct MATHRandContext32 {
    u64 x;
    u64 mul;
    u64 add;
} MATHRandContext32;

static inline void MATH_InitRand32(MATHRandContext32 *context, u64 seed) {
    context->x   = seed;
    context->mul = 0x5D588B656C078965ULL;
    context->add = 0x269EC3ULL;
}

static inline u32 MATH_Rand32(MATHRandContext32 *context, u32 max) {
    context->x = context->mul * context->x + context->add;
    if (max == 0) {
        return (u32) (context->x >> 32);
    }
    return (u32) (((context->x >> 32) * max) >> 32);
}
