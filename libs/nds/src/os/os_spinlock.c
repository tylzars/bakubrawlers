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

static BOOL lockInitialized;   // data_020bdcc0

void func_02004c50(void) {
    OSLockWord *lockp = OS_LOCKWORD_BUF;
    if (lockInitialized) {
        return;
    }
    lockp->lockFlag = 0;
    lockInitialized = TRUE;
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

void OS_UnLockCartridge(u16 lockID) {
    OS_UnlockCartridge(lockID);
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
