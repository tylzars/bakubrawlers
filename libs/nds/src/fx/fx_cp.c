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


void func_01ff8154(fx32 num, fx32 den);
fx32 func_01ff80c0(void);
void func_01ff80f8(fx32 x);
fx32 func_01ff8128(void);

fx32 FX_Div(fx32 num, fx32 den) {
    func_01ff8154(num, den);
    return func_01ff80c0();
}

s64 func_01ff8010(fx32 num, fx32 den) {
    func_01ff8154(num, den);
    while (REG_DIVCNT & 0x8000) {
    }
    return REG_DIV_RESULT;
}

s64 func_01ff803c(fx32 x) {
    func_01ff80f8(x);
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
    return func_01ff8128();
}

s64 func_01ff809c(void) {
    while (REG_DIVCNT & 0x8000) {
    }
    return REG_DIV_RESULT;
}

fx32 func_01ff80c0(void) {
    s64 r;
    while (REG_DIVCNT & 0x8000) {
    }
    r = REG_DIV_RESULT;
    return (fx32) ((r + 0x80000) >> 20);
}

void func_01ff80f8(fx32 x) {
    REG_DIVCNT = 1;
    REG_DIV_NUMER = 0x1000LL << 32;
    REG_DIV_DENOM = (u32) x;
}

fx32 func_01ff8128(void) {
    while (REG_SQRTCNT & 0x8000) {
    }
    return (REG_SQRT_RESULT + 0x200) >> 10;
}

void func_01ff8154(fx32 num, fx32 den) {
    REG_DIVCNT = 1;
    REG_DIV_NUMER = (s64) num << 32;
    REG_DIV_DENOM = (u32) den;
}
