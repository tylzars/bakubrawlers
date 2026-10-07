#pragma once

#include "types.h"

typedef struct MATHRandContext32 {
    u64 x;
    u64 mul;
    u64 add;
} MATHRandContext32;

static inline u32 MATH_Rand32(MATHRandContext32 *context, u32 max) {
    context->x = context->mul * context->x + context->add;
    return (u32) (((context->x >> 32) * max) >> 32);
}
