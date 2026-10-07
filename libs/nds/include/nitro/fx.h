#pragma once

#include "types.h"


#define FX32_SHIFT 12

static inline fx32 FX_Mul(fx32 v1, fx32 v2) {
    return (fx32) (((fx64) v1 * v2 + 0x800LL) >> FX32_SHIFT);
}

// 3D vector of fixed-point values.
typedef struct {
    fx32 x;
    fx32 y;
    fx32 z;
} VecFx32;

// 4x3 matrix (three basis rows plus translation), row-major.
typedef struct {
    fx32 _[12];
} MtxFx43;

// 4x4 matrix, row-major.
typedef struct {
    fx32 _[16];
} MtxFx44;
