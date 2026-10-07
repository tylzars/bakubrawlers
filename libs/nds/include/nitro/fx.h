#pragma once

#include "types.h"


#define FX32_SHIFT 12

#define FX32_CONST(x) ((fx32) (((x) > 0) ? ((x) * 4096.0f + 0.5f) : ((x) * 4096.0f - 0.5f)))

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

// 3x3 matrix (rotation only), row-major.
typedef struct MtxFx33 {
    fx32 _[9];
} MtxFx33;

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
void VEC_Add(const VecFx32 *a, const VecFx32 *b, VecFx32 *ab);      // 0x01ff8e3c (itcm)
void VEC_Subtract(const VecFx32 *a, const VecFx32 *b, VecFx32 *ab); // 0x01ff8e70 (itcm)
fx32 VEC_Mag(const VecFx32 *v);                                      // 0x01ff8f64 (itcm)
fx32 VEC_DotProduct(const VecFx32 *a, const VecFx32 *b);             // 0x01ff8ea4 (itcm)
void VEC_CrossProduct(const VecFx32 *a, const VecFx32 *b, VecFx32 *axb); // 0x01ff8ee0 (itcm)
void VEC_Normalize(const VecFx32 *src, VecFx32 *dst);                // 0x01ff8fc4 (itcm)
fx32 FX_Sqrt(fx32 x); // 0x01ff8068 (itcm, hardware square root)
void func_01ffcef4(MtxFx22 *mtx, fx32 scaleX, fx32 scaleY, u16 angle); // itcm: rotation and scale
#ifdef __cplusplus
}
#endif
