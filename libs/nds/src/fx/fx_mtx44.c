#include <nitro/internal.h>

void FX_InvAsync(fx32 den);   // 0x01ff80f8 (itcm)
fx64 FX_GetDivResultFx64(void); // 0x01ff809c (itcm)

#define reg_DIV_NUMER_L (*(vu32 *) 0x04000290)
#define reg_DIV_NUMER_H (*(vu32 *) 0x04000294)
#define reg_DIV_DENOM_L (*(vu32 *) 0x04000298)
#define reg_DIV_DENOM_H (*(vu32 *) 0x0400029c)

static inline void StartFxReciprocal(fx32 den) {
    *(volatile u64 *) 0x04000290 = 0x1000ULL << 32;
    *(volatile u64 *) 0x04000298 = (u32) den;
}

static inline fx32 FX_Mul32x64c(fx32 a, fx64 b) {
    return (fx32) ((b * (fx64) a + 0x80000000LL) >> 32);
}

void MTX_Frustum(fx32 t, fx32 b, fx32 l, fx32 r, fx32 n, fx32 f, fx32 scale, MtxFx44 *mtx) {
    fx64 val1;
    fx64 val2;
    fx64 val3;
    fx32 tmp;
    fx32 n2;
    FX_InvAsync(r - l);
    mtx->_[1] = 0;
    mtx->_[2] = 0;
    mtx->_[3] = 0;
    mtx->_[4] = 0;
    mtx->_[6] = 0;
    mtx->_[7] = 0;
    mtx->_[11] = -scale;
    mtx->_[12] = 0;
    mtx->_[13] = 0;
    mtx->_[15] = 0;
    n2 = n * 2;
    val1 = FX_GetDivResultFx64();
    StartFxReciprocal(t - b);
    if (scale != 0x1000) {
        val1 = (val1 * scale) / 0x1000;
    }
    mtx->_[0] = FX_Mul32x64c(n2, val1);
    val2 = FX_GetDivResultFx64();
    StartFxReciprocal(n - f);
    if (scale != 0x1000) {
        val2 = (val2 * scale) / 0x1000;
    }
    mtx->_[5] = FX_Mul32x64c(n2, val2);
    mtx->_[8] = FX_Mul32x64c(r + l, val1);
    mtx->_[9] = FX_Mul32x64c(t + b, val2);
    val3 = FX_GetDivResultFx64();
    if (scale != 0x1000) {
        val3 = (val3 * scale) / 0x1000;
    }
    mtx->_[10] = FX_Mul32x64c(f + n, val3);
    tmp = FX_Mul(n2, f);
    mtx->_[14] = FX_Mul32x64c(tmp, val3);
}
