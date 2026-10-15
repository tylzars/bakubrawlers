// IRQ dispatch and thread-switch return path in ITCM (0x01ffc93c-0x01ffcb20), hand-written assembly.

extern char SDK_AUTOLOAD_DTCM_START[];
extern char data_027e0060[];
extern char OSi_ThreadInfo[];
extern void func_0200be08(void);
extern void func_0200be48(void);
extern void func_01ffc9a4(void);

// func_01ffc93c: top-level interrupt dispatcher. Reads the interrupt master
// enable (register at +0x208 from 0x04000000); returns at once if it is zero.
// Otherwise ANDs the enable and request words (+0x210, +0x214); returns if no
// enabled request is pending. It then picks the highest-numbered pending bit
// (leading-zero count), acknowledges it by writing that single bit back to the
// request register, and jumps to the handler stored at index (31 - bit) of the
// table at the start of the fast-memory area. The link register is preset to
// func_01ffc9a4, so the handler returns into the thread-switch code.
// The routine ends with two literal words (table base, return address).
asm void func_01ffc93c(void) {
    stmfd sp!, {lr}
    mov ip, #0x4000000
    add ip, ip, #0x210
    ldr r1, [ip, #-8]           // master enable
    cmp r1, #0
    beq Lret1
    b Lgo1
Lret1:
    ldmfd sp!, {pc}
Lgo1:
    ldmia ip, {r1, r2}          // r1 = enable, r2 = request
    ands r1, r1, r2             // pending and enabled
    beq Lret2
    b Lgo2
Lret2:
    ldmfd sp!, {pc}
Lgo2:
    mov r3, #0x80000000
Lfind:
    clz r0, r1                  // highest set bit
    bics r1, r1, r3, lsr r0     // clear it in the working copy
    bne Lfind                   // loop to the lowest set bit
    mov r1, r3, lsr r0
    str r1, [ip, #4]            // acknowledge that request bit
    rsbs r0, r0, #31
    ldr r1, =SDK_AUTOLOAD_DTCM_START
    ldr r0, [r1, r0, lsl #2]    // handler address from the table
    ldr lr, =func_01ffc9a4
    bx r0
}

// func_01ffc9a4: runs after an interrupt handler returns. First, if the
// thread-state pointer in the first literal (data_027e0060) is non-NULL, walks
// a chain of records: for each it stores 1 at +0x64, zeroes +0x78/+0x7c/+0x80
// (reading the old +0x80 as the next link), then clears the pointer and its
// neighbour and sets a halfword flag (at OSi_ThreadInfo) to 1. If that flag is
// zero it returns. Otherwise it clears the flag, enters IRQ mode with
// interrupts masked and scans the thread list (starting at flag area + 8)
// for the first thread whose state (+0x64 halfword) is 1 (linked through +0x68).
// With none found, or when the thread equals the current one (+4), it switches
// back to supervisor mode and returns. Otherwise it calls the optional switch
// hook (+0xc) if set, records the new current thread, saves the interrupted
// context (SPSR plus registers, user-mode registers via the ^ forms) into the
// old thread's context record, restores the new thread's context from its
// record (supervisor sp, SPSR, user registers, resume address), and resumes it
// via the final stack-popped return. Four literal words follow the routine.
asm void func_01ffc9a4(void) {
    ldr ip, =data_027e0060
    mov r3, #0
    ldr ip, [ip]
    mov r2, #1
    cmp ip, #0
    beq Lnochain
Lchain:
    str r2, [ip, #0x64]
    str r3, [ip, #0x78]
    str r3, [ip, #0x7c]
    ldr r0, [ip, #0x80]
    str r3, [ip, #0x80]
    mov ip, r0
    cmp ip, #0
    bne Lchain
    ldr ip, =data_027e0060
    str r3, [ip]
    str r3, [ip, #4]
    ldr ip, =OSi_ThreadInfo
    mov r1, #1
    strh r1, [ip]
Lnochain:
    ldr ip, =OSi_ThreadInfo
    ldrh r1, [ip]
    cmp r1, #0
    beq Lskip1
    b Lgo
Lskip1:
    ldr pc, [sp], #4
Lgo:
    mov r1, #0
    strh r1, [ip]
    mov r3, #0xd2               // IRQ mode, interrupts masked
    msr cpsr_c, r3
    add r2, ip, #8
    ldr r1, [r2]
Lscan:
    cmp r1, #0
    bne Lload
    b Lnoload
Lload:
    ldrh r0, [r1, #0x64]
Lnoload:
    bne Lcmp
    b Lnocmp
Lcmp:
    cmp r0, #1
Lnocmp:
    bne Lnext
    b Lnonext
Lnext:
    ldr r1, [r1, #0x68]
Lnonext:
    bne Lscan
    cmp r1, #0
    bne Lfound
Lreturn:
    mov r3, #0x92
    msr cpsr_c, r3
    ldr pc, [sp], #4
Lfound:
    ldr r0, [ip, #4]
    cmp r1, r0
    beq Lreturn
    ldr r3, [ip, #0xc]
    cmp r3, #0
    beq Lnohook
    stmfd sp!, {r0, r1, ip}
    mov lr, pc
    bx r3
    ldmfd sp!, {r0, r1, ip}
Lnohook:
    str r1, [ip, #4]
    mrs r2, spsr
    str r2, [r0, #0]!
    stmfd sp!, {r0, r1}
    add r0, r0, #0
    add r0, r0, #0x48
    ldr r1, =func_0200be08
    blx r1
    ldmfd sp!, {r0, r1}
    ldmib sp!, {r2, r3}
    stmib r0!, {r2, r3}
    ldmib sp!, {r2, r3, ip, lr}
    stmib r0!, {r2, r3, r4, r5, r6, r7, r8, r9, r10, r11, ip, sp, lr}^
    stmib r0!, {lr}
    mov r3, #0xd3               // supervisor mode, interrupts masked
    msr cpsr_c, r3
    stmib r0!, {sp}
    stmfd sp!, {r1}
    add r0, r1, #0
    add r0, r0, #0x48
    ldr r1, =func_0200be48
    blx r1
    ldmfd sp!, {r1}
    ldr sp, [r1, #0x44]
    mov r3, #0xd2
    msr cpsr_c, r3
    ldr r2, [r1, #0]!
    msr spsr_fc, r2
    ldr lr, [r1, #0x40]
    ldmib r1, {r0, r1, r2, r3, r4, r5, r6, r7, r8, r9, r10, r11, ip, sp, lr}^
    mov r0, r0
    stmda sp!, {r0, r1, r2, r3, ip, lr}
    ldmfd sp!, {pc}
}
