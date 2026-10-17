#include <nitro/private/os_types.h>

extern void WaitByLoop(u32 count);
extern void SleepUntilIrq(int a, int b);
extern void PXI_Init(void);                                           // 0x02009050
extern void PXI_SetFifoRecvCallback(int fifoTag, void (*callback)(int tag, u32 data, BOOL err));   // 0x0200915c
extern BOOL PXI_IsCallbackReady(int fifoTag, int proc);               // 0x020091a8
extern int PXI_SendWordByFifo(int fifoTag, u32 data, BOOL err);       // 0x020091cc
extern void OS_Terminate(void);
extern s32 OS_GetLockID(void);
extern void func_0200e620(u32 lockId);
extern u32 OS_SetIrqMask(u32 mask);       // 0x02004b64
extern u32 OS_ResetRequestIrqMask(u32 mask);   // 0x02004bf0
extern void MI_StopDma(int dmaNo);        // 0x0200786c
extern void func_01ffcb20(void);          // ITCM
extern void ResetFifoReceive(int tag, u32 data, BOOL err);
extern void ResetFifoSend(u32 command);

// 0x020be04c
struct {
    u16 state;
    u16 initialized;
} resetState;

/* The V_BLANK check bit is set by the game's UpdateFrame (OS_SetIrqFunction(1, ...)) once every sFrameInterval
   vblanks. */
void SleepUntilVBlank(void) {
    WaitByLoop(1);
    SleepUntilIrq(1, 1);
}

void OS_InitReset(void) {
    if (resetState.initialized != 0) {
        return;
    }
    resetState.initialized = 1;
    PXI_Init();
    while (!PXI_IsCallbackReady(12, 1)) {
    }
    PXI_SetFifoRecvCallback(12, ResetFifoReceive);
}

void ResetFifoReceive(int tag, u32 data, BOOL err) {
    if ((((data & 0x7f00) << 8) >> 16) == 0x10) {
        resetState.state = 1;
    } else {
        OS_Terminate();
    }
}

void ResetFifoSend(u32 command) {
    while (PXI_SendWordByFifo(12, command << 8, FALSE) != 0) {
    }
}

static inline BOOL BootTypeIsTwo(void) {
    return *(vu16 *) 0x27ffc40 == 2;
}

void OS_ResetSystem(u32 parameter) {
    if (BootTypeIsTwo()) {
        OS_Terminate();
    }
    func_0200e620((u16) OS_GetLockID());
    OS_SetIrqMask(0x40000);
    OS_ResetRequestIrqMask(0xfffbffff);
    MI_StopDma(0);
    MI_StopDma(1);
    MI_StopDma(2);
    MI_StopDma(3);
    *(vu32 *) 0x27ffc20 = parameter;
    ResetFifoSend(0x10);
    asm {
        ldr r0, =0x27e3f80
        ldr r1, =0x1000
        sub r0, r0, r1
        mov sp, r0
    }
    func_01ffcb20();
}
