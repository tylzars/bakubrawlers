// MI_CpuFill8 and MI_CpuCopy8 (ITCM) are asm bodies as well: their byte loops don't come out of C.
// ITCM memory routines 0x01ff94c8-0x01ff9802 (compiler 2.0/sp1p5, -O4,p -ipa file).
// Hand-written asm (not reproducible from C: conditional-execution loops, single-register ldm/stm, 8-register stm runs):
//   MIi_CpuClear16 0x01ff94c8, MIi_CpuCopy16 0x01ff94e0, MIi_CpuClear32 0x01ff94fc, MIi_CpuCopy32, MI_CpuSend32,
//   MIi_CpuClearFast 0x01ff9540, MIi_CpuCopyFast(fast, ldm/stm x8) 0x01ff958c, MI_Copy36B, MI_Copy48B, MI_Copy64B,
//   MI_Zero36B (thumb, stm x3).
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

// MI_CpuSend32(src, dst, size): reads size bytes of words from src (advancing)
// and stores each to the same single word at dst (dst never advances). Ends
// when the source pointer reaches src+size.
asm void MI_CpuSend32(const void *src, volatile void *dst, unsigned int size) {
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

// MI_Copy48B(src, dst): copies 12 words (48 bytes), three at a time.
asm void MI_Copy48B(const void *src, void *dst) {
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

// MI_Copy64B(src, dst): copies 16 words (64 bytes): four groups of three,
// then a final group of four (r0 is reloaded as one of the data registers
// without write-back).
asm void MI_Copy64B(const void *src, void *dst) {
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
    cmp r2, #0
    bxeq lr                     // nothing to fill
    tst r0, #1
    beq L_even                  // dst already halfword aligned
    ldrh ip, [r0, #-1]          // odd start: keep the byte below dst
    and ip, ip, #0xff
    orr r3, ip, r1, lsl #8      // and put the fill byte above it
    strh r3, [r0, #-1]
    add r0, r0, #1
    subs r2, r2, #1
    bxeq lr
L_even:
    cmp r2, #2
    blo L_last_byte             // a single byte left
    orr r1, r1, r1, lsl #8      // fill byte in both halves of a halfword
    tst r0, #2
    beq L_word_aligned
    strh r1, [r0], #2           // one halfword up to word alignment
    subs r2, r2, #2
    bxeq lr
L_word_aligned:
    orr r1, r1, r1, lsl #16     // fill byte in all four bytes
    bics r3, r2, #3             // r3 = bytes to store as words
    beq L_tail_half
    sub r2, r2, r3
    add ip, r3, r0              // end of the word run
L_words:
    str r1, [r0], #4
    cmp r0, ip
    blo L_words
L_tail_half:
    tst r2, #2
    strneh r1, [r0], #2         // two bytes left: one halfword
L_last_byte:
    tst r2, #1
    bxeq lr
    ldrh r3, [r0]               // odd end: keep the byte above
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
    cmp r2, #0
    bxeq lr                     // nothing to copy
    tst r1, #1
    beq L_dst_even              // dst already halfword aligned
    ldrh ip, [r1, #-1]          // odd dst: keep the byte below it
    and ip, ip, #0xff
    tst r0, #1
    ldrneh r3, [r0, #-1]        // first source byte, from whichever half holds it
    movne r3, r3, lsr #8
    ldreqh r3, [r0]
    orr r3, ip, r3, lsl #8
    strh r3, [r1, #-1]
    add r0, r0, #1
    add r1, r1, #1
    subs r2, r2, #1
    bxeq lr
L_dst_even:
    eor ip, r1, r0
    tst ip, #1
    beq L_same_parity
    // src odd, dst even: read aligned source halfwords and shift each byte pair together.
    bic r0, r0, #1
    ldrh ip, [r0], #2
    mov r3, ip, lsr #8          // r3 = byte carried into the next store
    subs r2, r2, #2
    blo L_shift_tail
L_shift_loop:
    ldrh ip, [r0], #2
    orr ip, r3, ip, lsl #8
    strh ip, [r1], #2
    mov r3, ip, lsr #16
    subs r2, r2, #2
    bhs L_shift_loop
L_shift_tail:
    tst r2, #1
    bxeq lr
    ldrh ip, [r1]               // last byte: merge the carried byte under the one above
    and ip, ip, #0xff00
    orr ip, ip, r3
    strh ip, [r1]
    bx lr
L_same_parity:
    tst ip, #2
    beq L_same_word
    // Same halfword alignment but different word alignment: copy halfwords.
    bics r3, r2, #1
    beq L_last_byte
    sub r2, r2, r3
    add ip, r3, r1              // end of the halfword run
L_halves:
    ldrh r3, [r0], #2
    strh r3, [r1], #2
    cmp r1, ip
    blo L_halves
    b L_last_byte
L_same_word:
    cmp r2, #2
    blo L_last_byte
    tst r1, #2
    beq L_word_aligned
    ldrh r3, [r0], #2           // one halfword up to word alignment
    strh r3, [r1], #2
    subs r2, r2, #2
    bxeq lr
L_word_aligned:
    bics r3, r2, #3             // r3 = bytes to copy as words
    beq L_tail_half
    sub r2, r2, r3
    add ip, r3, r1              // end of the word run
L_words:
    ldr r3, [r0], #4
    str r3, [r1], #4
    cmp r1, ip
    blo L_words
L_tail_half:
    tst r2, #2
    ldrneh r3, [r0], #2         // two bytes left: one halfword
    strneh r3, [r1], #2
L_last_byte:
    tst r2, #1
    bxeq lr
    ldrh r2, [r1]               // odd end: keep the dst byte above, take the low source byte
    ldrh r0, [r0]
    and r2, r2, #0xff00
    and r0, r0, #0xff
    orr r0, r2, r0
    strh r0, [r1]
    bx lr
}

// MI_Zero36B (Thumb): zeroes nine words at r0 (three stores of three zeros).


#pragma thumb on
asm void MI_Zero36B(void *dst) {
    mov r1, #0
    mov r2, #0
    mov r3, #0
    stmia r0!, {r1, r2, r3}
    stmia r0!, {r1, r2, r3}
    stmia r0!, {r1, r2, r3}
    bx lr
}
#pragma thumb off
