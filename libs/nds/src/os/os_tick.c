#include <nitro/private/os_types.h>

typedef u64 OSTick;

extern void OS_SetIrqFunction(u32 mask, void (*func)(void));            // 0x020049c0
extern void OSi_EnterTimerCallback(int timerNum, void (*callback)(void *), void *arg);   // 0x02004b1c
extern u32 OS_EnableIrqMask(u32 mask);                                  // 0x02004b90
extern void OSi_SetTimerReserved(int timerNum);                         // 0x02006a98
extern void OSi_CountUpTick(void);

// 0x020be01c
static struct {
    u16 initialized;
    int needReset;
    OSTick counter;
} tickState;

void OS_InitTick(void) {
    if (tickState.initialized) {
        return;
    }
    tickState.initialized = TRUE;
    OSi_SetTimerReserved(0);
    tickState.counter = 0;
    *(vu16 *) 0x4000102 = 0;
    *(vu16 *) 0x4000100 = 0;
    *(vu16 *) 0x4000102 = 0xc1;
    OS_SetIrqFunction(8, OSi_CountUpTick);
    OS_EnableIrqMask(8);
    tickState.needReset = FALSE;
}

BOOL OS_IsTickAvailable(void) {
    return tickState.initialized;
}

void OSi_CountUpTick(void) {
    tickState.counter++;
    if (tickState.needReset) {
        *(vu16 *) 0x4000102 = 0;
        *(vu16 *) 0x4000100 = 0;
        *(vu16 *) 0x4000102 = 0xc1;
        tickState.needReset = FALSE;
    }
    OSi_EnterTimerCallback(0, (void (*)(void *)) OSi_CountUpTick, NULL);
}

OSTick OS_GetTick(void) {
    OSIntrMode e = OS_DisableInterrupts();
    volatile u16 countL = *(vu16 *) 0x4000100;
    volatile u64 countH = tickState.counter & 0x0000ffffffff0000ULL;
    if ((*(vu32 *) 0x4000214 & 8) && !(countL & 0x8000)) {
        countH++;
    }
    OS_RestoreInterrupts(e);
    return (countH << 16) | countL;
}
