#include <nitro/private/sdk.h>

typedef s32 OSArenaId;

extern void PXI_Init(void);   // PXI_Init
extern void OS_InitLock(void);     // 0x02004c50
extern void func_020049a8(void);
extern void func_02004c1c(void);
extern void OS_InitException(void);  // 0x02006884
extern void func_02007f18(void);
extern void OS_InitVAlarm(void);   // 0x020070b0
extern void func_02007464(void);
extern void OS_InitThread(void);   // 0x020052bc
extern void OS_InitReset(void);   // OS_InitReset
extern void func_02011dc8(void);
extern void func_0200f4c8(void);
extern void func_0200cd80(void);
extern void OSi_WaitVCount0(void);
extern void OS_InitArena(void);
extern void OS_InitArenaEx(void);
extern void *OS_GetArenaHi(OSArenaId id);
extern void *OS_GetArenaLo(OSArenaId id);
extern void *OS_GetInitArenaHi(OSArenaId id);
extern void *OS_GetInitArenaLo(OSArenaId id);
extern void OS_SetArenaHi(OSArenaId id, void *newHi);
extern void OS_SetArenaLo(OSArenaId id, void *newLo);
extern u32 func_02005a68(void);
extern void OS_SetProtectionRegion1(u32 param);   // 0x02006874 (asm)
extern void OS_SetProtectionRegion2(u32 param);   // 0x0200687c (asm)

#define OS_ARENA_INFO_BASE 0x27ffda0

// 0x020bdf40 (global: os code outside this file reads it too)
struct {
    BOOL initialized;
    BOOL exArena;
} arenaState;

asm void OSi_WaitVCount0(void) {
    mov ip, #0x4000000
    ldr r1, [ip, #0x208]
    str ip, [ip, #0x208]
L1:
    ldrh r0, [ip, #6]
    cmp r0, #0
    bne L1
    str r1, [ip, #0x208]
    bx lr
}

void OS_Init(void) {
    OS_InitArena();
    PXI_Init();
    OS_InitLock();
    OS_InitArenaEx();
    func_020049a8();
    func_02004c1c();
    OS_InitException();
    func_02007f18();
    OS_InitVAlarm();
    func_02007464();
    OS_InitThread();
    OS_InitReset();
    func_02011dc8();
    func_0200f4c8();
    func_0200cd80();
    OSi_WaitVCount0();
}

void OS_InitArena(void) {
    if (arenaState.initialized) {
        return;
    }
    arenaState.initialized = TRUE;
    OS_SetArenaHi(0, OS_GetInitArenaHi(0));
    OS_SetArenaLo(0, OS_GetInitArenaLo(0));
    OS_SetArenaLo(2, NULL);
    OS_SetArenaHi(2, NULL);
    OS_SetArenaHi(3, OS_GetInitArenaHi(3));
    OS_SetArenaLo(3, OS_GetInitArenaLo(3));
    OS_SetArenaHi(4, OS_GetInitArenaHi(4));
    OS_SetArenaLo(4, OS_GetInitArenaLo(4));
    OS_SetArenaHi(5, OS_GetInitArenaHi(5));
    OS_SetArenaLo(5, OS_GetInitArenaLo(5));
    OS_SetArenaHi(6, OS_GetInitArenaHi(6));
    OS_SetArenaLo(6, OS_GetInitArenaLo(6));
}

void OS_InitArenaEx(void) {
    OS_SetArenaHi(2, OS_GetInitArenaHi(2));
    OS_SetArenaLo(2, OS_GetInitArenaLo(2));
    if (arenaState.exArena) {
        if ((func_02005a68() & 3) != 1) {
            return;
        }
    }
    OS_SetProtectionRegion1(0x200002b);
    OS_SetProtectionRegion2(0x23e0021);
}

void *OS_GetArenaHi(OSArenaId id) {
    return ((void **) (OS_ARENA_INFO_BASE + 0x24))[id];
}

void *OS_GetArenaLo(OSArenaId id) {
    return ((void **) OS_ARENA_INFO_BASE)[id];
}

void *OS_GetInitArenaHi(OSArenaId id) {
    switch (id) {
    case 0:
        return (void *) 0x23e0000;
    case 2:
        if (arenaState.exArena == FALSE || (func_02005a68() & 3) == 1) {
            return NULL;
        }
        return (void *) 0x2700000;
    case 3:
        return (void *) 0x2000000;
    case 4: {
        u32 dtcm = HW_DTCM;
        u32 irqBottom = dtcm + 0x3f80 - (u32) SDK_IRQ_STACKSIZE;
        s32 sysSize = (s32) SDK_SYS_STACKSIZE;
        if (sysSize == 0) {
            return (void *) (dtcm < 0x27e0080 ? 0x27e0080 : dtcm);
        }
        if (sysSize < 0) {
            return (void *) (0x27e0080 - sysSize);
        }
        return (void *) (irqBottom - sysSize);
    }
    case 5:
        return (void *) 0x27ff680;
    case 6:
        return (void *) 0x37f8000;
    default:
        return NULL;
    }
}

void *OS_GetInitArenaLo(OSArenaId id) {
    switch (id) {
    case 0:
        return (void *) 0x228bc20;
    case 2:
        if (arenaState.exArena == FALSE || (func_02005a68() & 3) == 1) {
            return NULL;
        }
        return (void *) 0x23e0000;
    case 3:
        return (void *) 0x1ffd1c0;
    case 4:
        return (void *) 0x27e0080;
    case 5:
        return (void *) 0x27ff000;
    case 6:
        return (void *) 0x37f8000;
    default:
        return NULL;
    }
}

void OS_SetArenaHi(OSArenaId id, void *newHi) {
    ((void **) (OS_ARENA_INFO_BASE + 0x24))[id] = newHi;
}

void OS_SetArenaLo(OSArenaId id, void *newLo) {
    ((void **) OS_ARENA_INFO_BASE)[id] = newLo;
}

void *OS_AllocFromArenaLo(OSArenaId id, u32 size, u32 align) {
    u32 start;
    u32 end;
    u32 lo = (u32) OS_GetArenaLo(id);
    if (lo == 0) {
        return NULL;
    }
    start = (lo + align - 1) & ~(align - 1);
    end = start + size;
    end = (end + align - 1) & ~(align - 1);
    if (end > (u32) OS_GetArenaHi(id)) {
        return NULL;
    }
    OS_SetArenaLo(id, (void *) end);
    return (void *) start;
}

void *OS_AllocFromArenaHi(OSArenaId id, u32 size, u32 align) {
    u32 hi = (u32) OS_GetArenaHi(id);
    u32 newHi;
    if (hi == 0) {
        return NULL;
    }
    hi &= ~(align - 1);
    newHi = hi - size;
    newHi &= ~(align - 1);
    if (newHi < (u32) OS_GetArenaLo(id)) {
        return NULL;
    }
    OS_SetArenaHi(id, (void *) newHi);
    return (void *) newHi;
}
