#include <nitro/private/os_types.h>

extern void OSi_ExceptionHandler(void);   // 0x020068f4 (asm)

// 0x020bdf6c
struct {
    u32 buf;
    u32 unk04;
    u32 unk08;
} exceptionState;

void OS_InitException(void) {
    u32 buf = *(vu32 *) 0x27ffd9c;
    if (buf >= 0x2600000 && buf < 0x2800000) {
        exceptionState.buf = buf;
    } else {
        exceptionState.buf = 0;
    }
    if (exceptionState.buf == 0) {
        vu8 *dtcm = (vu8 *) 0x27e3000;
        *(vu32 *) 0x27ffd9c = (u32) OSi_ExceptionHandler;
        *(vu32 *) (dtcm + 0xfdc) = (u32) OSi_ExceptionHandler;
    }
    exceptionState.unk08 = 0;
}

extern char data_020bdf98[];
extern void func_02006978(void);
extern void func_0200698c(void);
extern void func_02006a1c(void);
extern void func_02006840(void);
extern void func_02006850(void);

// OSi_ExceptionHandler: entry for processor exceptions (data/prefetch abort).
// If the first word of exceptionState holds a user hook, it is called
// first (lr set to the following code so it returns here). Then r0-r3, the
// entry sp and lr are pushed onto a small stack at 0x02000000 going down, r0
// is set to the entry sp's low bit, and sp is moved to that stack. The mode
// bits of CPSR are tested: for modes 0x17 and 0x1b (abort/undefined) the
// context capture is run (func_02006978). Afterwards, if the hook word is
// still non-zero the code spins forever; otherwise it restores r0-r3, sp
// (through ip) and lr and returns.
asm void OSi_ExceptionHandler(void) {
    ldr ip, =exceptionState
    ldr ip, [ip]
    cmp ip, #0
    bne Lcall
    b Lskip
Lcall:
    mov lr, pc
Lskip:
    bne Ljump
    b Lnojump
Ljump:
    bx ip
Lnojump:
    ldr ip, =0x2000000
    stmdb ip!, {r0, r1, r2, r3, sp, lr}
    and r0, sp, #1
    mov sp, ip
    mrs r1, cpsr
    and r1, r1, #0x1f
    teq r1, #0x17               // abort mode
    bne Ltry2
    bl func_02006978
    b Ldone
Ltry2:
    teq r1, #0x1b               // undefined-instruction mode
    bne Ldone
    bl func_02006978
Ldone:
    ldr ip, =exceptionState
    ldr ip, [ip]
    cmp ip, #0
Lspin:
    beq Lspin                   // no hook: halt here
Lnop:
    mov r0, r0
    b Lnop                      // hook present: loop forever as well
    ldmfd sp!, {r0, r1, r2, r3, ip, lr}
    mov sp, ip
    bx lr
}

// func_02006978: saves r0 and lr, runs the register capture (func_0200698c)
// and the hook launcher (func_02006a1c), then returns.
asm void func_02006978(void) {
    stmfd sp!, {r0, lr}
    bl func_0200698c
    bl func_02006a1c
    ldmfd sp!, {r0, lr}
    bx lr
}

// func_0200698c: captures the faulting context into the block at
// data_020bdf98. r0 is the value saved at entry, ip points at the pushed
// exception frame { r0, r1, r2, r3, sp, lr }. Stores CPSR (+0x74), r0 (+0x6c),
// the pushed r0-r3 (+4..+0x10), r4-r11 (+0x14), the frame address (+0x70), then
// reads four words from the address in the frame's fifth word with bit 0
// cleared (+0x64, +0, +0x34, +0x40). It then briefly switches to the mode held
// in the second of those words (with interrupts masked, Thumb bit cleared) to
// grab that mode's sp (+0x38), lr (+0x3c) and SPSR (+0x7c), and restores the
// original CPSR.
asm void func_0200698c(void) {
    ldr r1, =data_020bdf98
    mrs r2, cpsr
    str r2, [r1, #0x74]
    str r0, [r1, #0x6c]
    ldr r0, [ip, #0]
    str r0, [r1, #4]
    ldr r0, [ip, #4]
    str r0, [r1, #8]
    ldr r0, [ip, #8]
    str r0, [r1, #0xc]
    ldr r0, [ip, #0xc]
    str r0, [r1, #0x10]
    ldr r2, [ip, #0x10]
    bic r2, r2, #1
    add r0, r1, #0x14
    stmia r0, {r4, r5, r6, r7, r8, r9, r10, r11}
    str ip, [r1, #0x70]
    ldr r0, [r2, #0]
    str r0, [r1, #0x64]
    ldr r3, [r2, #4]
    str r3, [r1, #0]
    ldr r0, [r2, #8]
    str r0, [r1, #0x34]
    ldr r0, [r2, #0xc]
    str r0, [r1, #0x40]
    mrs r0, cpsr
    orr r3, r3, #0x80
    bic r3, r3, #0x20
    msr cpsr_fsxc, r3           // enter the captured mode
    str sp, [r1, #0x38]
    str lr, [r1, #0x3c]
    mrs r2, spsr
    str r2, [r1, #0x7c]
    msr cpsr_fsxc, r0           // back to the handler's mode
    bx lr
}

// func_02006a1c: if the third word of exceptionState is non-zero, runs it
// as a hook in system mode (CPSR 0x9f): saves the caller's CPSR and sp, calls
// func_02006840, then jumps to the function in the third word with r0 =
// data_020bdf98 and r1 = the second word, lr preset to the following
// 'bl func_02006850' instruction (address 0x02006a6c, held as a constant).
// After the hook returns, calls func_02006850, restores sp and CPSR and returns.
asm void func_02006a1c(void) {
    stmfd sp!, {r3, lr}
    ldr r0, =exceptionState
    ldr r0, [r0, #8]
    cmp r0, #0
    ldmeqfd sp!, {r3, pc}
    mrs r2, cpsr
    mov r0, sp
    ldr r1, =0x9f
    msr cpsr_fsxc, r1
    mov r1, sp
    mov sp, r0
    stmfd sp!, {r1, r2}
    bl func_02006840
    ldr r0, =data_020bdf98
    ldr r1, =exceptionState+4
    ldr r1, [r1, #0]
    ldr ip, =exceptionState+8
    ldr ip, [ip, #0]
    ldr lr, =0x2006a6c
    bx ip
    bl func_02006850
    ldmfd sp!, {r1, r2}
    mov sp, r1
    msr cpsr_fsxc, r2
    ldmfd sp!, {r3, pc}
}
