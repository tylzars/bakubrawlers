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

// 2x2 matrix (BG affine parameters).
typedef struct MtxFx22 {
    fx32 _[4];
} MtxFx22;

#ifdef __cplusplus
extern "C" {
#endif
// Sine/cosine pairs: [2 * i] is sin, [2 * i + 1] is cos, for i = angle >> 4.
extern const s16 FX_SinCosTable_[];
void MTX_Identity22_(MtxFx22 *mtx); // 0x02000cb4
void func_01ffcef4(MtxFx22 *mtx, fx32 scaleX, fx32 scaleY, u16 angle); // itcm: rotation and scale
#ifdef __cplusplus
}
#endif
