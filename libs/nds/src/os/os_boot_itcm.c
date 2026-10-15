// Boot-time card/ROM loader helpers in ITCM: 0x01ffcb20-0x01ffcdb8 (func_01ffcb54 and func_01ffcc20 are hand assembly).
#include <nitro/internal.h>

extern char SDK_AUTOLOAD_DTCM_START[];
extern struct { vu16 state; u16 initialized; } resetState;   // os_reset.c
extern u32 OS_DisableInterrupts(void);
extern u32 OS_RestoreInterrupts(u32 state);
void func_02005e04(void);
void func_02005df8(void);
void IC_InvalidateAll(void);
void DC_WaitWriteBufferEmpty(void);
void func_01ffcb54(void);
void func_01ffcc20(u32 v, u32 *dst, u32 size);
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

asm void func_01ffcb54(void) {
    mov ip, #0x4000000
    str ip, [ip, #0x208]
    ldr r1, =SDK_AUTOLOAD_DTCM_START
    add r1, r1, #0x3fc0
    add r1, r1, #0x3c
    mov r0, #0
    str r0, [r1]
    ldr r1, =0x4000180
L1:
    ldrh r0, [r1]
    and r0, r0, #0xf
    cmp r0, #1
    bne L1
    mov r0, #0x100
    strh r0, [r1]
    mov r0, #0
    ldr r3, =0x27ffd9c
    ldr r4, [r3]
    ldr r1, =0x27ffd80
    mov r2, #0x80
    bl func_01ffcc20
    str r4, [r3]
    ldr r1, =0x27fff80
    mov r2, #0x18
    bl func_01ffcc20
    ldr r1, =0x27fff98
    strh r0, [r1]
    ldr r1, =0x27fff9c
    mov r2, #0x64
    bl func_01ffcc20
    ldr r1, =0x4000180
L2:
    ldrh r0, [r1]
    and r0, r0, #0xf
    cmp r0, #1
    beq L2
    mov r0, #0
    strh r0, [r1]
    ldr r3, =0x27ffe00
    ldr ip, [r3, #0x24]
    mov lr, ip
    ldr r11, =0x27fff80
    ldmia r11, {r0, r1, r2, r3, r4, r5, r6, r7, r8, r9, r10}
    mov r11, #0
    bx ip
}

asm void func_01ffcc20(u32 v, u32 *dst, u32 size) {
    add ip, r1, r2
L1:
    cmp r1, ip
    blt L2
    b L3
L2:
    stmia r1!, {r0}
L3:
    blt L1
    bx lr
}

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
    IC_InvalidateAll();
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
