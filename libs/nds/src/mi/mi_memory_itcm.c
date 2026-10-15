// MI_CpuFill8 and MI_CpuCopy8 (ITCM) are asm bodies as well: their byte loops don't come out of C.
// ITCM memory routines 0x01ff94c8-0x01ff9802 (compiler 2.0/sp1p5, -O4,p -ipa file).
// Hand-written asm (not reproducible from C: conditional-execution loops, single-register ldm/stm, 8-register stm runs):
//   MIi_CpuClear16 0x01ff94c8, MIi_CpuCopy16 0x01ff94e0, MIi_CpuClear32 0x01ff94fc, MIi_CpuCopy32, func_01ff9528,
//   MIi_CpuClearFast 0x01ff9540, MIi_CpuCopyFast(fast, ldm/stm x8) 0x01ff958c, MI_Copy36B, func_01ff95e0, func_01ff9604,
//   func_01ff97f4 (thumb, stm x3).
// MI_CpuFill8 and MI_CpuCopy8 (byte fill/copy) follow near the end.
#include <nitro/internal.h>

// MIi_CpuClear16(value, dst, size): stores the halfword 'value' over 'size'
// bytes at dst, two bytes at a time, while the running offset is below size.
asm void MIi_CpuClear16(unsigned int value, void *dst, unsigned int size) {
    mov r3, #0
L1:
    cmp r3, r2
    strlth r0, [r1, r3]
    addlt r3, r3, #2
    blt L1
    bx lr
}

// MIi_CpuCopy16(src, dst, size): copies size bytes as halfwords.
asm void MIi_CpuCopy16(const void *src, void *dst, unsigned int size) {
    mov ip, #0
L1:
    cmp ip, r2
    ldrlth r3, [r0, ip]
    strlth r3, [r1, ip]
    addlt ip, ip, #2
    blt L1
    bx lr
}

// MIi_CpuClear32(value, dst, size): stores the word 'value' over size bytes.
asm void MIi_CpuClear32(unsigned int value, void *dst, unsigned int size) {
    add ip, r1, r2              // end address
L1:
    cmp r1, ip
    stmltia r1!, {r0}
    blt L1
    bx lr
}

// MIi_CpuCopy32(src, dst, size): copies size bytes as words.
asm void MIi_CpuCopy32(const void *src, void *dst, unsigned int size) {
    add ip, r1, r2
L1:
    cmp r1, ip
    ldmltia r0!, {r2}
    stmltia r1!, {r2}
    blt L1
    bx lr
}

// func_01ff9528(src, dst, size): reads size bytes of words from src (advancing)
// and stores each to the same single word at dst (dst never advances). Ends
// when the source pointer reaches src+size.
asm void func_01ff9528(const void *src, volatile void *dst, unsigned int size) {
    add ip, r0, r2
L1:
    cmp r0, ip
    ldmltia r0!, {r2}
    strlt r2, [r1]
    blt L1
    bx lr
}

// MIi_CpuClearFast(value, dst, size): fills size bytes at dst with the word
// 'value'. Whole 32-byte blocks are written eight words at a time using copies
// of value in r0,r2-r8; the leftover bytes are written a word at a time.
asm void MIi_CpuClearFast(unsigned int value, void *dst, unsigned int size) {
    stmfd sp!, {r4, r5, r6, r7, r8, r9}
    add r9, r1, r2                  // end address
    mov ip, r2, lsr #5
    add ip, r1, ip, lsl #5          // end of the whole-block part
    mov r2, r0
    mov r3, r2
    mov r4, r2
    mov r5, r2
    mov r6, r2
    mov r7, r2
    mov r8, r2
L1:
    cmp r1, ip
    stmltia r1!, {r0, r2, r3, r4, r5, r6, r7, r8}
    blt L1
L2:
    cmp r1, r9
    stmltia r1!, {r0}
    blt L2
    ldmfd sp!, {r4, r5, r6, r7, r8, r9}
    bx lr
}

// MIi_CpuCopyFast(src, dst, size): copies size bytes; whole 32-byte blocks
// eight words at a time, then the remaining words one at a time.
asm void MIi_CpuCopyFast(const void *src, void *dst, unsigned int size) {
    stmfd sp!, {r4, r5, r6, r7, r8, r9, r10}
    add r10, r1, r2                  // end address
    mov ip, r2, lsr #5
    add ip, r1, ip, lsl #5          // end of the whole-block part
L1:
    cmp r1, ip
    ldmltia r0!, {r2, r3, r4, r5, r6, r7, r8, r9}
    stmltia r1!, {r2, r3, r4, r5, r6, r7, r8, r9}
    blt L1
L2:
    cmp r1, r10
    ldmltia r0!, {r2}
    stmltia r1!, {r2}
    blt L2
    ldmfd sp!, {r4, r5, r6, r7, r8, r9, r10}
    bx lr
}

// MI_Copy36B(src, dst): copies 9 words (36 bytes) from src to dst, three at a
// time; both pointers advance.
asm void MI_Copy36B(const void *src, volatile void *dst) {
    ldmia r0!, {r2, r3, ip}
    stmia r1!, {r2, r3, ip}
    ldmia r0!, {r2, r3, ip}
    stmia r1!, {r2, r3, ip}
    ldmia r0!, {r2, r3, ip}
    stmia r1!, {r2, r3, ip}
    bx lr
}

// func_01ff95e0(src, dst): copies 12 words (48 bytes), three at a time.
asm void func_01ff95e0(const void *src, void *dst) {
    ldmia r0!, {r2, r3, ip}
    stmia r1!, {r2, r3, ip}
    ldmia r0!, {r2, r3, ip}
    stmia r1!, {r2, r3, ip}
    ldmia r0!, {r2, r3, ip}
    stmia r1!, {r2, r3, ip}
    ldmia r0!, {r2, r3, ip}
    stmia r1!, {r2, r3, ip}
    bx lr
}

// func_01ff9604(src, dst): copies 16 words (64 bytes): four groups of three,
// then a final group of four (r0 is reloaded as one of the data registers
// without write-back).
asm void func_01ff9604(const void *src, void *dst) {
    ldmia r0!, {r2, r3, ip}
    stmia r1!, {r2, r3, ip}
    ldmia r0!, {r2, r3, ip}
    stmia r1!, {r2, r3, ip}
    ldmia r0!, {r2, r3, ip}
    stmia r1!, {r2, r3, ip}
    ldmia r0!, {r2, r3, ip}
    stmia r1!, {r2, r3, ip}
    ldmia r0, {r0, r2, r3, ip}
    stmia r1!, {r0, r2, r3, ip}
    bx lr
}

// MI_CpuFill8(dst, data, size): fills size bytes at dst with the low byte of
// data using only halfword and word stores. An odd leading byte and an odd
// trailing byte are merged into their halfword by read-modify-write; between
// them a halfword store reaches word alignment, then words, then a last
// halfword if two bytes remain. Returns at once when size is 0.
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

// MI_CpuCopy8(src, dst, size): copies size bytes using only halfword and word
// stores. An odd leading destination byte is merged into its halfword first.
// If src and dst then differ in bit 0, aligned source halfwords are shifted
// together and stored as halfwords; if they differ only in bit 1, halfwords are
// copied; otherwise a halfword reaches word alignment, then words, then a last
// halfword. An odd trailing byte is merged by read-modify-write. Returns at
// once when size is 0.
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

// func_01ff97f4 (Thumb): zeroes nine words at r0 (three stores of three zeros).


#pragma thumb on
asm void func_01ff97f4(void *dst) {
    mov r1, #0
    mov r2, #0
    mov r3, #0
    stmia r0!, {r1, r2, r3}
    stmia r0!, {r1, r2, r3}
    stmia r0!, {r1, r2, r3}
    bx lr
}
#pragma thumb off
