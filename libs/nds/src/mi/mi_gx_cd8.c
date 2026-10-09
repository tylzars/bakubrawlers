#include "types.h"
extern void func_01ffcdbc(int dmaNo, const void *src, void *dest, u32 ctrl);   // ITCM
extern void MIi_CheckAnotherAutoDMA(int dmaNo, u32 mode);   // 0x02007910
extern void MIi_CheckDma0SourceAddress(int dmaNo, u32 src, u32 size, u32 dir);   // 0x02007994
void func_02007cd8(int dmaNo, const void *src, void *dest, u32 size) {
    vu32 *dmaCntp;
    MIi_CheckAnotherAutoDMA(dmaNo, 0xffffffff);
    MIi_CheckDma0SourceAddress(dmaNo, (u32) src, size, 0x1000000);
    if (size == 0) {
        return;
    }
    dmaCntp = (vu32 *) (0x4000000 + (dmaNo * 3 + 2) * 4 + 0xb0);
    while (*dmaCntp & 0x80000000) {
    }
    func_01ffcdbc(dmaNo, src, dest, 0xaf000001);
}
