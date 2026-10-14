#include <nitro/internal.h>

#define REG_DIVCNT (*(vu16 *) 0x04000280)
#define REG_DIV_NUMER (*(vs64 *) 0x04000290)
#define REG_DIV_DENOM (*(vs64 *) 0x04000298)
#define REG_DIV_NUMER_L (*(vu32 *) 0x04000290)
#define REG_DIV_NUMER_H (*(vu32 *) 0x04000294)
#define REG_DIV_DENOM_L (*(vu32 *) 0x04000298)
#define REG_DIV_DENOM_H (*(vu32 *) 0x0400029c)
#define REG_DIV_RESULT (*(s64 *) 0x040002a0)
#define REG_DIV_RESULT_L (*(vu32 *) 0x040002a0)
#define REG_DIV_RESULT_H (*(vu32 *) 0x040002a4)
#define REG_SQRTCNT (*(vu16 *) 0x040002b0)
#define REG_SQRT_RESULT (*(vu32 *) 0x040002b4)
#define REG_SQRT_PARAM_L (*(vu32 *) 0x040002b8)
#define REG_SQRT_PARAM_H (*(vu32 *) 0x040002bc)


void FX_DivAsync(fx32 num, fx32 den);
fx32 FX_GetDivResult(void);
void FX_InvAsync(fx32 x);
fx32 FX_GetSqrtResult(void);

fx32 FX_Div(fx32 num, fx32 den) {
    FX_DivAsync(num, den);
    return FX_GetDivResult();
}

s64 FX_DivFx64c(fx32 num, fx32 den) {
    FX_DivAsync(num, den);
    while (REG_DIVCNT & 0x8000) {
    }
    return REG_DIV_RESULT;
}

s64 FX_InvFx64c(fx32 x) {
    FX_InvAsync(x);
    while (REG_DIVCNT & 0x8000) {
    }
    return REG_DIV_RESULT;
}

fx32 FX_Sqrt(fx32 x) {
    if (x <= 0) {
        return 0;
    }
    REG_SQRTCNT = 1;
    REG_SQRT_PARAM_L = 0;
    REG_SQRT_PARAM_H = x;
    return FX_GetSqrtResult();
}

s64 FX_GetDivResultFx64c(void) {
    while (REG_DIVCNT & 0x8000) {
    }
    return REG_DIV_RESULT;
}

fx32 FX_GetDivResult(void) {
    s64 r;
    while (REG_DIVCNT & 0x8000) {
    }
    r = REG_DIV_RESULT;
    return (fx32) ((r + 0x80000) >> 20);
}

void FX_InvAsync(fx32 x) {
    REG_DIVCNT = 1;
    REG_DIV_NUMER = 0x1000LL << 32;
    REG_DIV_DENOM = (u32) x;
}

fx32 FX_GetSqrtResult(void) {
    while (REG_SQRTCNT & 0x8000) {
    }
    return (REG_SQRT_RESULT + 0x200) >> 10;
}

void FX_DivAsync(fx32 num, fx32 den) {
    REG_DIVCNT = 1;
    REG_DIV_NUMER = (s64) num << 32;
    REG_DIV_DENOM = (u32) den;
}
