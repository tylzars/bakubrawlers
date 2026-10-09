// Boot-time card/ROM loader helpers in ITCM: 0x01ffcb20-0x01ffcdb8 (func_01ffcb54 and func_01ffcc20 are hand assembly).
#include <nitro/internal.h>

extern struct { vu16 state; u16 initialized; } resetState;   // os_reset.c
extern u32 OS_DisableInterrupts(void);
extern u32 OS_RestoreInterrupts(u32 state);
void func_02005e04(void);
void func_02005df8(void);
void func_02005ecc(void);
void DC_WaitWriteBufferEmpty(void);
void func_01ffcb54(void);
void func_01ffcc3c(void);
void func_01ffcce4(u32 rom, void *dst, u32 size);

void func_01ffcb20(void) {
    while (resetState.state == 0) {
    }
    *(vu16 *) 0x04000208 = 0;
    func_01ffcc3c();
    func_01ffcb54();
}

typedef struct {
    u32 arm9Rom, arm9Entry, arm9Ram, arm9Size;
    u32 arm7Rom, arm7Entry, arm7Ram, arm7Size;
} BootHdr;

void func_01ffcc3c(void) {
    u32 off = *(u32 *) 0x027ffc2c;
    BootHdr *h = (BootHdr *) 0x027ffe20;
    u32 a9Rom, a9Ram, a9Size, a7Rom, a7Ram, a7Size;
    u32 irq;
    if (off >= 0x8000) {
        func_01ffcce4(off, (void *) 0x027ffe00, 0x160);
    }
    a9Rom = h->arm9Rom;
    a9Ram = h->arm9Ram;
    a9Size = h->arm9Size;
    a7Rom = h->arm7Rom;
    a7Ram = h->arm7Ram;
    a7Size = h->arm7Size;
    irq = OS_DisableInterrupts();
    func_02005e04();
    func_02005df8();
    OS_RestoreInterrupts(irq);
    func_02005ecc();
    DC_WaitWriteBufferEmpty();
    a9Rom += off;
    if (a9Rom < 0x8000) {
        u32 d = 0x8000 - a9Rom;
        a9Ram += d;
        a9Size -= d;
        a9Rom = 0x8000;
    }
    func_01ffcce4(a9Rom, (void *) a9Ram, a9Size);
    func_01ffcce4(a7Rom + off, (void *) a7Ram, a7Size);
}

void func_01ffcce4(u32 rom, void *dst, u32 size) {
    u32 ctl = (*(u32 *) 0x027ffe60 & ~0x07000000) | 0xa1000000;
    s32 pos = -(s32) (rom & 0x1ff);
    u32 st;
    u32 hi;
    while (*(vu32 *) 0x040001a4 & 0x80000000) {
    }
    *(vu8 *) 0x040001a1 = 0x80;
    rom += pos;
    if (pos >= (s32) size) {
        return;
    }
    hi = rom >> 8;
    do {
        *(vu8 *) 0x040001a8 = 0xb7;
        *(vu8 *) 0x040001a9 = rom >> 24;
        *(vu8 *) 0x040001aa = rom >> 16;
        *(vu8 *) 0x040001ab = hi;
        *(vu8 *) 0x040001ac = rom;
        *(vu8 *) 0x040001ad = 0;
        *(vu8 *) 0x040001ae = 0;
        *(vu8 *) 0x040001af = 0;
        *(vu32 *) 0x040001a4 = ctl;
        do {
            st = *(vu32 *) 0x040001a4;
            if (st & 0x800000) {
                u32 w = *(vu32 *) 0x04100010;
                if (pos >= 0 && pos < (s32) size) {
                    *(u32 *) ((u32) dst + pos) = w;
                }
                pos += 4;
            }
        } while (st & 0x80000000);
        hi += 2;
        rom += 0x200;
    } while (pos < (s32) size);
}
