// IRQ dispatch and thread-switch return path in ITCM (0x01ffc93c-0x01ffcb20), hand-written assembly.

extern char SDK_AUTOLOAD_DTCM_START[];
extern char data_027e0060[];
extern char OSi_ThreadInfo[];
extern void SaveDivSqrtRegisters(void);
extern void RestoreDivSqrtRegisters(void);
extern void func_01ffc9a4(void);

// DispatchIrq: top-level interrupt dispatcher. Reads the interrupt master
// enable (register at +0x208 from 0x04000000); returns at once if it is zero.
// Otherwise ANDs the enable and request words (+0x210, +0x214); returns if no
// enabled request is pending. It then picks the lowest-numbered pending bit
// (clearing the highest set bit until one remains, by leading-zero count),
// acknowledges it by writing that single bit back to the request register, and
// jumps to the handler stored at index = bit number (31 - leading zeros) of the
// word table at SDK_AUTOLOAD_DTCM_START. The link register is preset to
// func_01ffc9a4, so the handler returns into the thread-switch code; the lr
// pushed on entry is popped there. The routine ends with two literal words
// (table base, return address).
// Not compiled: finds the pending bit with clz, which this compiler never emits: it has no clz intrinsic, and
// `__builtin_clz` and `__clz` compile as calls to an external function. It also jumps to the handler with lr preset
// to func_01ffc9a4, which pops the lr this routine pushed.
asm void DispatchIrq(void) {
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

// func_01ffc9a4: runs after an interrupt handler returns. First, if the word
// at data_027e0060 is non-zero, walks a chain of records from it: for each it
// stores 1 at +0x64, zeroes +0x78/+0x7c/+0x80 (reading the old +0x80 as the
// next link), then zeroes both words at data_027e0060 and sets the halfword
// flag at OSi_ThreadInfo to 1. If that flag is zero it returns (popping the
// lr pushed by DispatchIrq). Otherwise it clears the flag, enters IRQ mode
// with IRQ/FIQ masked (0xd2) and scans the thread list (head word at
// OSi_ThreadInfo + 8, linked through +0x68) for the first thread whose
// halfword at +0x64 is 1. With none found, or when that thread equals the
// current one (OSi_ThreadInfo + 4), it sets the CPSR control byte to 0x92 (IRQ mode, only IRQ
// masked) and returns. Otherwise it calls the function at OSi_ThreadInfo + 0xc
// if non-zero (r0 = current thread, r1 = new one), records the new current
// thread, and saves into the old thread's context record: SPSR (+0), the
// interrupted r0-r3/ip from the IRQ stack frame, r4-r11 and user-bank sp/lr
// (+4..+0x3c), the IRQ lr (+0x40), the supervisor sp (+0x44) and, through
// SaveDivSqrtRegisters, the divider/square-root registers (+0x48). It then restores
// the new thread's divider/square-root registers (RestoreDivSqrtRegisters), supervisor
// sp, SPSR and user-bank registers from its record, writes r0-r3, ip and the
// resume address (+0x40) over the IRQ stack frame, and returns through the
// word popped from the stack. Four literal words follow the routine.
// Not compiled: changes mode with msr and reads SPSR with mrs, which C has no expression for. It also saves and
// restores the user-bank registers with `stmib r0!, {...}^` / `ldmib r1, {...}^`; in the forms tried the compiler
// emits `^` only in the `ldm sp!, {..., pc}^` return of an `__irq` function, and no compiled function in this build
// has any.
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
    mov r3, #0xd2               // IRQ mode, IRQ/FIQ masked
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
    ldr r1, =SaveDivSqrtRegisters
    blx r1
    ldmfd sp!, {r0, r1}
    ldmib sp!, {r2, r3}
    stmib r0!, {r2, r3}
    ldmib sp!, {r2, r3, ip, lr}
    stmib r0!, {r2, r3, r4, r5, r6, r7, r8, r9, r10, r11, ip, sp, lr}^
    stmib r0!, {lr}
    mov r3, #0xd3               // supervisor mode, IRQ/FIQ masked
    msr cpsr_c, r3
    stmib r0!, {sp}
    stmfd sp!, {r1}
    add r0, r1, #0
    add r0, r0, #0x48
    ldr r1, =RestoreDivSqrtRegisters
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
