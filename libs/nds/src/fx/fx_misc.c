#include <nitro/internal.h>

#define FX32_INT_MASK 0x7ffff000
#define FX32_DEC_MASK 0x00000fff

void func_02001b34(void) {
}

fx32 FX_Modf(fx32 x, fx32 *iPtr) {
    if (x >= 0) {
        *iPtr = x & FX32_INT_MASK;
        return x & FX32_DEC_MASK;
    } else {
        x = -x;
        *iPtr = -(x & FX32_INT_MASK);
        return -(x & FX32_DEC_MASK);
    }
}

extern const s16 FX_SinCosTable_[];

u16 FX_AsinIdx(fx32 x) {
    int low, high, mid;
    s16 v;
    if (x >= 0) {
        low = 0;
        high = 0x400;
    } else {
        low = 0xc00;
        high = 0x1000;
    }
    v = (s16) x;
    while (low <= high) {
        mid = (low + high) / 2;
        if (v == FX_SinCosTable_[mid * 2]) {
            break;
        }
        if (FX_SinCosTable_[mid * 2] < v) {
            low = mid + 1;
        } else {
            high = mid - 1;
        }
    }
    return (u16) ((mid << 17) / 8192);
}

u16 FX_AcosIdx(fx32 x) {
    int low, high, mid;
    s16 v;
    if (x >= 0) {
        low = 0;
        high = 0x400;
    } else {
        low = 0x400;
        high = 0x800;
    }
    v = (s16) x;
    while (low <= high) {
        mid = (low + high) / 2;
        if (v == FX_SinCosTable_[mid * 2 + 1]) {
            break;
        }
        if (FX_SinCosTable_[mid * 2 + 1] < v) {
            high = mid - 1;
        } else {
            low = mid + 1;
        }
    }
    return (u16) (((mid * 2 + 1) << 16) / 8192);
}
