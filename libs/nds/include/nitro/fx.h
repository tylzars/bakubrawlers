#pragma once

#include "types.h"


#define FX32_SHIFT 12

static inline fx32 FX_Mul(fx32 v1, fx32 v2) {
    return (fx32) (((fx64) v1 * v2 + 0x800LL) >> FX32_SHIFT);
}
