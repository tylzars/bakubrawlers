// MI_CpuFill8 and MI_CpuCopy8 (ITCM) are asm bodies as well: their byte loops don't come out of C.
// ITCM NitroSDK memory routines 0x01ff94c8-0x01ff9802 (compiler 2.0/sp1p5, -O4,p -ipa file).
// Hand-written asm (not reproducible from C: conditional-execution loops, single-register ldm/stm, 8-register stm runs):
//   MIi_CpuClear16 0x01ff94c8, MIi_CpuCopy16 0x01ff94e0, MIi_CpuClear32 0x01ff94fc, MIi_CpuCopy32, func_01ff9528,
//   MIi_CpuClearFast 0x01ff9540, MIi_CpuCopyFast(fast, ldm/stm x8) 0x01ff958c, MI_Copy36B, func_01ff95e0, func_01ff9604,
//   func_01ff97f4 (thumb, stm x3).
// Not matching (scheduling/register allocation only): MI_CpuFill8, MI_CpuCopy8 (byte copy).
#include <nitro/internal.h>

asm void MI_CpuFill8(void *dstp, u32 data, u32 size) {
    cmp r2, #0x0
    bxeq lr
    tst r0, #0x1
    beq L965c
    ldrh ip, [r0, #-0x1]
    and ip, ip, #0xff
    orr r3, ip, r1, lsl #0x8
    strh r3, [r0, #-0x1]
    add r0, r0, #0x1
    subs r2, r2, #0x1
    bxeq lr
L965c:
    cmp r2, #0x2
    blo L96a4
    orr r1, r1, r1, lsl #0x8
    tst r0, #0x2
    beq L967c
    strh r1, [r0], #0x2
    subs r2, r2, #0x2
    bxeq lr
L967c:
    orr r1, r1, r1, lsl #0x10
    bics r3, r2, #0x3
    beq L969c
    sub r2, r2, r3
    add ip, r3, r0
L9690:
    str r1, [r0], #0x4
    cmp r0, ip
    blo L9690
L969c:
    tst r2, #0x2
    strneh r1, [r0], #0x2
L96a4:
    tst r2, #0x1
    bxeq lr
    ldrh r3, [r0]
    and r3, r3, #0xff00
    and r1, r1, #0xff
    orr r1, r1, r3
    strh r1, [r0]
    bx lr
}

asm void MI_CpuCopy8(const void *srcp, void *dstp, u32 size) {
    cmp r2, #0x0
    bxeq lr
    tst r1, #0x1
    beq L9704
    ldrh ip, [r1, #-0x1]
    and ip, ip, #0xff
    tst r0, #0x1
    ldrneh r3, [r0, #-0x1]
    movne r3, r3, lsr #0x8
    ldreqh r3, [r0]
    orr r3, ip, r3, lsl #0x8
    strh r3, [r1, #-0x1]
    add r0, r0, #0x1
    add r1, r1, #0x1
    subs r2, r2, #0x1
    bxeq lr
L9704:
    eor ip, r1, r0
    tst ip, #0x1
    beq L9758
    bic r0, r0, #0x1
    ldrh ip, [r0], #0x2
    mov r3, ip, lsr #0x8
    subs r2, r2, #0x2
    blo L973c
L9724:
    ldrh ip, [r0], #0x2
    orr ip, r3, ip, lsl #0x8
    strh ip, [r1], #0x2
    mov r3, ip, lsr #0x10
    subs r2, r2, #0x2
    bhs L9724
L973c:
    tst r2, #0x1
    bxeq lr
    ldrh ip, [r1]
    and ip, ip, #0xff00
    orr ip, ip, r3
    strh ip, [r1]
    bx lr
L9758:
    tst ip, #0x2
    beq L9784
    bics r3, r2, #0x1
    beq L97d0
    sub r2, r2, r3
    add ip, r3, r1
L9770:
    ldrh r3, [r0], #0x2
    strh r3, [r1], #0x2
    cmp r1, ip
    blo L9770
    b L97d0
L9784:
    cmp r2, #0x2
    blo L97d0
    tst r1, #0x2
    beq L97a4
    ldrh r3, [r0], #0x2
    strh r3, [r1], #0x2
    subs r2, r2, #0x2
    bxeq lr
L97a4:
    bics r3, r2, #0x3
    beq L97c4
    sub r2, r2, r3
    add ip, r3, r1
L97b4:
    ldr r3, [r0], #0x4
    str r3, [r1], #0x4
    cmp r1, ip
    blo L97b4
L97c4:
    tst r2, #0x2
    ldrneh r3, [r0], #0x2
    strneh r3, [r1], #0x2
L97d0:
    tst r2, #0x1
    bxeq lr
    ldrh r2, [r1]
    ldrh r0, [r0]
    and r2, r2, #0xff00
    and r0, r0, #0xff
    orr r0, r2, r0
    strh r0, [r1]
    bx lr
}

