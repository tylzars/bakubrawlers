#include <nitro/internal.h>

fx32 FX_Div(fx32 num, fx32 den); // 0x01ff8000 (itcm)
extern const s16 FX_AtanTable_[]; // 0x0208f524, radians
extern const s16 FX_AtanIdxTable_[]; // 0x0208f628, angle index

s16 FX_Atan2(fx32 y, fx32 x) {
    fx32 a;
    fx32 b;
    int base;
    int add;
    if (y > 0) {
        if (x > 0) {
            if (x > y) {
                a = y;
                b = x;
                base = 0;
                add = 1;
            } else if (x < y) {
                a = x;
                b = y;
                base = 0x1922;
                add = 0;
            } else {
                return 0xc91;
            }
        } else if (x < 0) {
            x = -x;
            if (x < y) {
                a = x;
                b = y;
                base = 0x1922;
                add = 1;
            } else if (x > y) {
                a = y;
                b = x;
                base = 0x3244;
                add = 0;
            } else {
                return 0x25b3;
            }
        } else {
            return 0x1922;
        }
    } else if (y < 0) {
        y = -y;
        if (x < 0) {
            x = -x;
            if (x > y) {
                a = y;
                b = x;
                base = -0x3244;
                add = 1;
            } else if (x < y) {
                a = x;
                b = y;
                base = -0x1922;
                add = 0;
            } else {
                return -0x25b3;
            }
        } else if (x > 0) {
            if (x < y) {
                a = x;
                b = y;
                base = -0x1922;
                add = 1;
            } else if (x > y) {
                a = y;
                b = x;
                base = 0;
                add = 0;
            } else {
                return -0xc91;
            }
        } else {
            return -0x1922;
        }
    } else {
        if (x >= 0) {
            return 0;
        }
        return 0x3244;
    }
    if (b == 0) {
        return 0;
    }
    if (add) {
        return (s16) (base + FX_AtanTable_[FX_Div(a, b) >> 5]);
    }
    return (s16) (base - FX_AtanTable_[FX_Div(a, b) >> 5]);
}

u16 FX_Atan2Idx(fx32 y, fx32 x) {
    fx32 a;
    fx32 b;
    int base;
    int add;
    if (y > 0) {
        if (x > 0) {
            if (x > y) {
                a = y;
                b = x;
                base = 0;
                add = 1;
            } else if (x < y) {
                a = x;
                b = y;
                base = 0x4000;
                add = 0;
            } else {
                return 0x2000;
            }
        } else if (x < 0) {
            x = -x;
            if (x < y) {
                a = x;
                b = y;
                base = 0x4000;
                add = 1;
            } else if (x > y) {
                a = y;
                b = x;
                base = 0x8000;
                add = 0;
            } else {
                return 0x6000;
            }
        } else {
            return 0x4000;
        }
    } else if (y < 0) {
        y = -y;
        if (x < 0) {
            x = -x;
            if (x > y) {
                a = y;
                b = x;
                base = -0x8000;
                add = 1;
            } else if (x < y) {
                a = x;
                b = y;
                base = -0x4000;
                add = 0;
            } else {
                return 0xa000;
            }
        } else if (x > 0) {
            if (x < y) {
                a = x;
                b = y;
                base = -0x4000;
                add = 1;
            } else if (x > y) {
                a = y;
                b = x;
                base = 0;
                add = 0;
            } else {
                return 0xe000;
            }
        } else {
            return 0xc000;
        }
    } else {
        if (x >= 0) {
            return 0;
        }
        return 0x8000;
    }
    if (b == 0) {
        return 0;
    }
    if (add) {
        return (u16) (base + FX_AtanIdxTable_[FX_Div(a, b) >> 5]);
    }
    return (u16) (base - FX_AtanIdxTable_[FX_Div(a, b) >> 5]);
}
