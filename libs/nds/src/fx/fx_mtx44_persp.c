#include <nitro/internal.h>

fx32 FX_Div(fx32 num, fx32 den);   // 0x01ff8000
fx64 FX_GetDivResultFx64c(void);    // 0x01ff809c
fx32 FX_GetDivResult(void);        // 0x01ff80c0

#define reg_DIV_NUMER (*(u64 *) 0x04000290)
#define reg_DIV_DENOM (*(u64 *) 0x04000298)

static inline fx32 FX_Mul32x64c(fx32 a, fx64 b) {
    return (fx32) ((b * (fx64) a + 0x80000000LL) >> 32);
}

void MTX_Perspective(fx32 sinFov, fx32 cosFov, fx32 aspect, fx32 n, fx32 f, fx32 scale, MtxFx44 *mtx) {
    fx32 cot;
    fx64 val;
    fx32 tmp;
    cot = FX_Div(cosFov, sinFov);
    reg_DIV_NUMER = (u64) 0x1000 << 32;
    reg_DIV_DENOM = (u32) (n - f);
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
    val = FX_GetDivResultFx64c();
    reg_DIV_NUMER = (u64) (u32) cot << 32;
    reg_DIV_DENOM = (u32) aspect;
    if (scale != 0x1000) {
        val = (val * scale) / 0x1000;
    }
    mtx->_[10] = FX_Mul32x64c(f + n, val);
    tmp = FX_Mul(n * 2, f);
    mtx->_[14] = FX_Mul32x64c(tmp, val);
    mtx->_[0] = FX_GetDivResult();
}
