// Divider and square-root unit register save/restore (0x0200be08-0x0200be84), hand-written assembly.

// Copies the eight words at 0x04000290.. into the buffer at r0: four words, then (from 0x040002b8)
// two more, then the 16-bit value at 0x04000280 masked to 2 bits and the one at 0x040002b0 masked
// to 1 bit as two halfwords.
asm void func_0200be08(unsigned *dst) {
    ldr r1, =0x4000290
    stmfd sp!, {r4}
    ldmia r1, {r2, r3, r4, ip}
    stmia r0!, {r2, r3, r4, ip}
    ldrh ip, [r1, #-0x10]
    add r1, r1, #0x28
    ldmia r1, {r2, r3}
    stmia r0!, {r2, r3}
    and ip, ip, #3
    ldrh r2, [r1, #-8]
    strh ip, [r0, #0]
    and r2, r2, #1
    strh r2, [r0, #2]
    ldmfd sp!, {r4}
    bx lr
}

// The inverse of the previous routine: writes the saved words and halfwords from the buffer at r0
// back to the same registers (four words at 0x04000290, halfwords at 0x04000280 and 0x040002b0,
// two words at 0x040002b8).
asm void func_0200be48(unsigned *src) {
    stmfd sp!, {r4}
    ldr r1, =0x4000290
    ldmia r0, {r2, r3, r4, ip}
    stmia r1, {r2, r3, r4, ip}
    ldrh r2, [r0, #0x18]
    ldrh r3, [r0, #0x1a]
    strh r2, [r1, #-0x10]
    strh r3, [r1, #0x20]
    add r0, r0, #0x10
    add r1, r1, #0x28
    ldmia r0, {r2, r3}
    stmia r1, {r2, r3}
    ldmfd sp!, {r4}
    bx lr
}
