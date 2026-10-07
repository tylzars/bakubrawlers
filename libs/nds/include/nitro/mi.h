#pragma once

#include "types.h"

#ifdef __cplusplus
extern "C" {
#endif

// Memory fill (mi_memory, placed in ITCM). MIi_CpuClear16 at 0x01ff94c8, MIi_CpuClear32 at 0x01ff94fc.
void MIi_CpuClear16(u16 data, void *dest, u32 size);
void MI_CpuFill8(void *dest, u8 data, u32 size); // 0x01ff9630
void MI_CpuCopy8(const void *src, void *dest, u32 size); // 0x01ff958c
void MIi_CpuClear32(u32 data, void *dest, u32 size);
void MIi_CpuClearFast(u32 data, void *dest, u32 size); // 0x01ff9540
void MI_Copy36B(const void *src, void *dest);            // 0x01ff95c4

static inline void MI_CpuClear8(void *dest, u32 size) {
    MI_CpuFill8(dest, 0, size);
}

static inline void MI_CpuClear16(void *dest, u32 size) {
    MIi_CpuClear16(0, dest, size);
}

static inline void MI_CpuClear32(void *dest, u32 size) {
    MIi_CpuClear32(0, dest, size);
}

static inline void MI_CpuClearFast(void *dest, u32 size) {
    MIi_CpuClearFast(0, dest, size);
}

#ifdef __cplusplus
}
#endif
