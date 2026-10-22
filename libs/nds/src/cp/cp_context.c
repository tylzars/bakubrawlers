// Divider and square-root unit register save/restore (0x0200be08-0x0200be84), hand-written assembly.

// Saves to the buffer at r0 (0x1c bytes): the four words at 0x04000290..0x0400029c, then the two
// words at 0x040002b8/0x040002bc, then two halfwords: the 16-bit value at 0x04000280 masked to
// 2 bits and the one at 0x040002b0 masked to 1 bit.
// Not compiled: stores with `stmia r0!, {r2, r3, r4, ip}` and `stmia r0!, {r2, r3}` (base written back, data outside
// r0-r3), and saves r4 alone (`stmfd sp!, {r4}` ... `bx lr`). In this build's compiled code an ldm/stm writes back a
// base other than sp only in struct copies and fills, which carry the data in r0-r3 (r0-r1 for a fill) and never use
// r0 or r1 as the base. Every compiled ARM function in this build that saves any of r4-r11 pushes lr with them and,
// unless it is variadic, returns by popping pc (`push {r4, lr}` ... `pop {r4, pc}`).
asm void SaveDivSqrtRegisters(unsigned *dst) {
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
// Not compiled: saves r4 alone (`stmfd sp!, {r4}` ... `ldmfd sp!, {r4}; bx lr`). Every compiled ARM function in this
// build that saves any of r4-r11 pushes lr with them and, unless it is variadic, returns by popping pc. Written in C,
// this body copies the four words through r0-r3 from a base moved to lr and saves {r3, lr} instead (`push {r3, lr}`
// ... `pop {r3, pc}`).
asm void RestoreDivSqrtRegisters(unsigned *src) {
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
