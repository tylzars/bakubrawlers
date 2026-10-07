#pragma once

#include "types.h"

#ifdef __cplusplus
extern "C" {
#endif

// Memory fill (mi_memory, placed in ITCM). MIi_CpuClear16 at 0x01ff94c8, MIi_CpuClear32 at 0x01ff94fc.
void MIi_CpuClear16(u16 data, void *dest, u32 size);
void MIi_CpuClear32(u32 data, void *dest, u32 size);

static inline void MI_CpuClear16(void *dest, u32 size) {
    MIi_CpuClear16(0, dest, size);
}

static inline void MI_CpuClear32(void *dest, u32 size) {
    MIi_CpuClear32(0, dest, size);
}

#ifdef __cplusplus
}
#endif
