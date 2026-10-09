#include <nitro/private/os_types.h>

extern u32 OS_CountLeadingZeros(u32 x);   // 0x0200745c (asm)

// 0x020be050: bitmask, 0x020be054: u16 table
static u32 irqOwnerMask;
static u16 irqOwnerTable[9];

void func_02007464(void) {
    int i;
    irqOwnerMask = 0;
    for (i = 0; i < 9; i++) {
        irqOwnerTable[i] = 0;
    }
}

void func_02007498(u32 mask, u16 tag) {
    u32 bits;
    int i;
    OSIntrMode e = OS_DisableInterrupts();
    bits = mask & irqOwnerMask & 0x1ff;
    while (1) {
        i = 31 - OS_CountLeadingZeros(bits);
        if (i < 0) {
            break;
        }
        {
            u32 clr = ~(1 << i);
            bits &= clr;
            if (tag == irqOwnerTable[i]) {
                irqOwnerTable[i] = 0;
                irqOwnerMask &= clr;
            }
        }
    }
    OS_RestoreInterrupts(e);
}
