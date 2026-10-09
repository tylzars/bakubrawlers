#include <nitro/internal.h>

fx32 FX_Div(fx32 num, fx32 den);   // 0x01ff8000
fx64 FX_GetDivResultFx64(void);    // 0x01ff809c
fx32 FX_GetDivResult(void);        // 0x01ff80c0

#define reg_DIV_NUMER_L (*(u32 *) 0x04000290)
#define reg_DIV_NUMER_H (*(u32 *) 0x04000294)
#define reg_DIV_DENOM_L (*(u32 *) 0x04000298)
#define reg_DIV_DENOM_H (*(u32 *) 0x0400029c)

static inline fx32 FX_Mul32x64c(fx32 a, fx64 b) {
    return (fx32) ((b * (fx64) a + 0x80000000LL) >> 32);
}

void func_02001688(fx32 sinFov, fx32 cosFov, fx32 aspect, fx32 n, fx32 f, fx32 scale, MtxFx44 *mtx) {
    fx32 cot;
    fx64 val;
    fx32 tmp;
    cot = FX_Div(cosFov, sinFov);
    reg_DIV_NUMER_L = 0;
    reg_DIV_NUMER_H = 0x1000;
    reg_DIV_DENOM_L = n - f;
    reg_DIV_DENOM_H = 0;
    if (scale != 0x1000) {
        cot = (cot * scale) / 0x1000;
    }
    mtx->_[1] = 0;
    mtx->_[2] = 0;
    mtx->_[3] = 0;
    mtx->_[4] = 0;
    mtx->_[5] = cot;
    mtx->_[6] = 0;
    mtx->_[7] = 0;
    mtx->_[8] = 0;
    mtx->_[9] = 0;
    mtx->_[11] = -scale;
    mtx->_[12] = 0;
    mtx->_[13] = 0;
    mtx->_[15] = 0;
    val = FX_GetDivResultFx64();
    reg_DIV_NUMER_L = 0;
    reg_DIV_NUMER_H = cot;
    reg_DIV_DENOM_L = aspect;
    reg_DIV_DENOM_H = 0;
    if (scale != 0x1000) {
        val = (val * scale) / 0x1000;
    }
    mtx->_[10] = FX_Mul32x64c(f + n, val);
    tmp = FX_Mul(n * 2, f);
    mtx->_[14] = FX_Mul32x64c(tmp, val);
    mtx->_[0] = FX_GetDivResult();
}
