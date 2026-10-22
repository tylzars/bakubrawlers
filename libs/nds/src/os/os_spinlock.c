#include <nitro/private/sdk.h>
#include <nitro/mi.h>

extern s32 OS_LockByWord(u16 lockID, OSLockWord *lockp, void (*ctrlFuncp)(void));
extern s32 OS_UnlockByWord(u16 lockID, OSLockWord *lockp, void (*ctrlFuncp)(void));
extern s32 OSi_DoUnlockByWord(u16 lockID, OSLockWord *lockp, void (*ctrlFuncp)(void), BOOL disableFiq);
extern s32 OSi_DoTryLockByWord(u16 lockID, OSLockWord *lockp, void (*ctrlFuncp)(void), BOOL disableFiq);
extern void func_02004eb4(void);
extern void func_02004ecc(void);
extern void func_02004f1c(void);
extern void func_02004f34(void);

#define OS_LOCKWORD_BUF ((OSLockWord *) 0x27ffff0)
#define OS_CARTRIDGE_LOCK ((OSLockWord *) 0x27fffe8)
#define OS_CARD_LOCK ((OSLockWord *) 0x27fffe0)

void OS_InitLock(void) {
    static BOOL lockInitialized;   // data_020bdcc0
    OSLockWord *lockp = OS_LOCKWORD_BUF;
    if (lockInitialized) {
        return;
    }
    lockInitialized = TRUE;
    lockp->lockFlag = 0;
    OS_LockByWord(0x7e, lockp, NULL);
    while (lockp->extension != 0) {
        WaitByLoop(0x400);
    }
    {
        vu32 *ids = (vu32 *) 0x27fffb0;
        ids[0] = (u32) -1;
        ids[1] = (u32) -1 << 16;
        MI_CpuClear32((void *) 0x27fffc0, 0x28);
    }
    REG_EXMEMCNT |= 0x800;
    REG_EXMEMCNT |= 0x80;
    OS_UnlockByWord(0x7e, lockp, NULL);
    OS_LockByWord(0x7f, lockp, NULL);
}

s32 OSi_DoLockByWord(u16 lockID, OSLockWord *lockp, void (*ctrlFuncp)(void), BOOL disableFiq) {
    s32 lastLockFlag = OSi_DoTryLockByWord(lockID, lockp, ctrlFuncp, disableFiq);
    if (lastLockFlag > 0) {
        do {
            WaitByLoop(0x400);
            lastLockFlag = OSi_DoTryLockByWord(lockID, lockp, ctrlFuncp, disableFiq);
        } while (lastLockFlag > 0);
    }
    return lastLockFlag;
}

s32 OS_LockByWord(u16 lockID, OSLockWord *lockp, void (*ctrlFuncp)(void)) {
    return OSi_DoLockByWord(lockID, lockp, ctrlFuncp, FALSE);
}

s32 OSi_DoUnlockByWord(u16 lockID, OSLockWord *lockp, void (*ctrlFuncp)(void), BOOL disableFiq) {
    OSIntrMode irq;
    if (lockID != lockp->ownerID) {
        return -2;
    }
    if (disableFiq) {
        irq = OS_DisableInterrupts_IrqAndFiq();
    } else {
        irq = OS_DisableInterrupts();
    }
    lockp->ownerID = 0;
    if (ctrlFuncp) {
        ctrlFuncp();
    }
    lockp->lockFlag = 0;
    if (disableFiq) {
        OS_RestoreInterrupts_IrqAndFiq(irq);
    } else {
        OS_RestoreInterrupts(irq);
    }
    return 0;
}

s32 OS_UnlockByWord(u16 lockID, OSLockWord *lockp, void (*ctrlFuncp)(void)) {
    return OSi_DoUnlockByWord(lockID, lockp, ctrlFuncp, FALSE);
}

s32 OSi_DoTryLockByWord(u16 lockID, OSLockWord *lockp, void (*ctrlFuncp)(void), BOOL disableFiq) {
    OSIntrMode irq;
    s32 lastLockFlag;
    if (disableFiq) {
        irq = OS_DisableInterrupts_IrqAndFiq();
    } else {
        irq = OS_DisableInterrupts();
    }
    lastLockFlag = MI_SwapWord(lockID, &lockp->lockFlag);
    if (lastLockFlag == 0) {
        if (ctrlFuncp) {
            ctrlFuncp();
        }
        lockp->ownerID = lockID;
    }
    if (disableFiq) {
        OS_RestoreInterrupts_IrqAndFiq(irq);
    } else {
        OS_RestoreInterrupts(irq);
    }
    return lastLockFlag;
}

s32 OS_UnlockCartridge(u16 lockID) {
    return OSi_DoUnlockByWord(lockID, OS_CARTRIDGE_LOCK, func_02004ecc, TRUE);
}

// OS_UnLockCartridge(lockID): jumps straight to OS_UnlockCartridge with the
// argument unchanged.
// Not compiled: jumps on through r1 (`ldr r1, =OS_UnlockCartridge; bx r1`). In the forms tried the compiler turns a
// final call into a jump only when it calls a named function, and then as `ldr ip, =f; bx ip` (for a function in the
// same file as well); a call through a pointer saves lr and uses `blx`. No compiled function in this build jumps
// through r0-r3.
asm void OS_UnLockCartridge(u16 lockID) {
    ldr r1, =OS_UnlockCartridge
    bx r1
}

s32 OS_TryLockCartridge(u16 lockID) {
    return OSi_DoTryLockByWord(lockID, OS_CARTRIDGE_LOCK, func_02004eb4, TRUE);
}

void func_02004eb4(void) {
    REG_EXMEMCNT &= ~0x80;
}

void func_02004ecc(void) {
    REG_EXMEMCNT |= 0x80;
}

s32 OS_LockCard(u16 lockID) {
    return OS_LockByWord(lockID, OS_CARD_LOCK, func_02004f1c);
}

s32 OS_UnlockCard(u16 lockID) {
    return OS_UnlockByWord(lockID, OS_CARD_LOCK, func_02004f34);
}

void func_02004f1c(void) {
    REG_EXMEMCNT &= ~0x800;
}

void func_02004f34(void) {
    REG_EXMEMCNT |= 0x800;
}

u16 OS_ReadOwnerOfLockWord(OSLockWord *lockp) {
    return lockp->ownerID;
}

// OS_GetLockID: allocates an id from a pair of 32-bit free-bit words at
// 0x027fffb0 (+0 covers ids 0x40-0x5f, +4 covers ids 0x60-0x7f). Takes the
// highest set bit of the first word, or failing that of the second; the id is
// base + leading-zero count, and that bit is cleared. Returns -3 when both
// words are empty. Some compares feed two consecutive conditional branches
// (the flags are reused across them).
// Not compiled: finds the free bit with clz, which this compiler never emits: it has no clz intrinsic, and
// `__builtin_clz` and `__clz` compile as calls to an external function.
asm int OS_GetLockID(void) {
    ldr r3, =0x27fffb0
    ldr r1, [r3, #0]
    clz r2, r1
    cmp r2, #0x20
    bne Lm1
    b Lm2
Lm1:
    mov r0, #0x40
Lm2:
    bne Ltake
    add r3, r3, #4
    ldr r1, [r3, #0]
    clz r2, r1
    cmp r2, #0x20
    ldr r0, =0xfffffffd
    beq Lnone
    b Lsecond
Lnone:
    bx lr                       // nothing free: return -3
Lsecond:
    mov r0, #0x60
Ltake:
    add r0, r0, r2              // id = base + leading zeros
    mov r1, #0x80000000
    mov r1, r1, lsr r2
    ldr r2, [r3, #0]
    bic r2, r2, r1              // mark the id as used
    str r2, [r3, #0]
    bx lr
}

// OS_FreeLockID(id): marks id (0x40-0x7f) free again by setting bit
// 0x80000000 >> (id - 0x40) in the word at 0x027fffb0 (ids below 0x60) or
// 0x80000000 >> (id - 0x60) in the word at 0x027fffb4 (ids 0x60 and up; the
// compare is signed). Returns nothing. The flags of the single compare are
// reused by all the conditional branches.
// Not compiled: one `cmp r0, #0x60` feeds `bpl`, `bpl` and `bmi` with instructions between them, each a conditional
// branch over a lone `b`. No compiled function in this build tests a compare with mi or pl or has such a branch. In
// the forms tried, `(int)id >= 0x60` is tested with ge/lt and the compare is repeated when the test is written again;
// pl/mi came only from a subtract made flag-setting (`(int)(id - 0x60) >= 0` gave `subs r1, r0, #0x60`). -O2 and up
// turn the steps into conditional instructions (`addge`, `subge`, `sublt`), -O1 into single inverted branches, and at
// -O0 the parameter leaves r0.
asm void OS_FreeLockID(unsigned int id) {
    ldr r3, =0x27fffb0
    cmp r0, #0x60
    bpl Lhi
    b Llo
Lhi:
    add r3, r3, #4
Llo:
    bpl Lsub60
    b Lcont
Lsub60:
    sub r0, r0, #0x60
Lcont:
    bmi Lsub40
    b Lbit
Lsub40:
    sub r0, r0, #0x40
Lbit:
    mov r1, #0x80000000
    mov r1, r1, lsr r0
    ldr r2, [r3, #0]
    orr r2, r2, r1
    str r2, [r3, #0]
    bx lr
}
