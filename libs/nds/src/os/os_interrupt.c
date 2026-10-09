#include <nitro/private/sdk.h>

u32 func_02004b64(u32 mask) {
    u16 prev = REG_IME;
    u32 old;
    REG_IME = 0;
    old = REG_IE;
    REG_IE = mask;
    (void) REG_IME;
    REG_IME = prev;
    return old;
}

u32 func_02004b90(u32 mask) {
    u16 prev = REG_IME;
    u32 old;
    REG_IME = 0;
    old = REG_IE;
    REG_IE = old | mask;
    (void) REG_IME;
    REG_IME = prev;
    return old;
}

u32 func_02004bc0(u32 mask) {
    u16 prev = REG_IME;
    u32 old;
    REG_IME = 0;
    old = REG_IE;
    REG_IE = old & ~mask;
    (void) REG_IME;
    REG_IME = prev;
    return old;
}

u32 func_02004bf0(u32 mask) {
    u16 prev = REG_IME;
    u32 old;
    REG_IME = 0;
    old = REG_IF;
    REG_IF = mask;
    (void) REG_IME;
    REG_IME = prev;
    return old;
}

extern char SDK_IRQ_STACKSIZE[];

void func_02004c1c(void) {
    u32 top = HW_DTCM + 0x3f80;
    *(u32 *) (HW_DTCM + 0x3f7c) = 0xfddb597d;
    *(u32 *) (top - (u32) SDK_IRQ_STACKSIZE) = 0x7bf9dd5b;
}
