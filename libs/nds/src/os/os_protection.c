// Protection-unit and CP15 control helpers (0x0200682c-0x02006884), hand-written assembly.

// func_0200682c: reads a coprocessor-15 register (c9,c1,0) and returns it with
// the low 12 bits cleared (mask 0xfffff000, kept in a literal word).
asm unsigned int func_0200682c(void) {
    mrc p15, 0, r0, c9, c1, 0
    ldr r1, =0xfffff000
    and r0, r0, r1
    bx lr
}

// func_02006840: sets bit 0 of the control register (c1,c0,0).
asm void func_02006840(void) {
    mrc p15, 0, r0, c1, c0, 0
    orr r0, r0, #1
    mcr p15, 0, r0, c1, c0, 0
    bx lr
}

// func_02006850: clears bit 0 of the control register (c1,c0,0).
asm void func_02006850(void) {
    mrc p15, 0, r0, c1, c0, 0
    bic r0, r0, #1
    mcr p15, 0, r0, c1, c0, 0
    bx lr
}

// func_02006860(clear, set): read-modify-write of register c5,c0,2:
// value = (value & ~clear) | set.
asm void func_02006860(unsigned int clear, unsigned int set) {
    mrc p15, 0, r2, c5, c0, 2
    bic r2, r2, r0
    orr r2, r2, r1
    mcr p15, 0, r2, c5, c0, 2
    bx lr
}

// OS_SetProtectionRegion1(value): writes r0 to region register c6,c1,0.
asm void OS_SetProtectionRegion1(unsigned int value) {
    mcr p15, 0, r0, c6, c1, 0
    bx lr
}

// OS_SetProtectionRegion2(value): writes r0 to region register c6,c2,0.
asm void OS_SetProtectionRegion2(unsigned int value) {
    mcr p15, 0, r0, c6, c2, 0
    bx lr
}
