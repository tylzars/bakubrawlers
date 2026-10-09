#include <nitro/private/os_types.h>

extern void OS_Terminate(void);
extern void *OS_GetIrqFunction(u32 mask);                              // 0x02004a48
extern void OS_SetIrqFunction(u32 mask, void (*func)(void));            // 0x020049c0
extern u32 OS_EnableIrqMask(u32 mask);        // 0x02004b90
extern u32 OS_DisableIrqMask(u32 mask);       // 0x02004bc0
extern u32 OS_ResetRequestIrqMask(u32 mask);  // 0x02004bf0
extern void OSi_EnterDmaCallback(int dmaNo, void (*callback)(void *), void *arg);   // 0x02004ad4
extern void func_01ffcdbc(int dmaNo, const void *src, void *dest, u32 ctrl);   // ITCM
extern void func_02007804(int dmaNo);
extern void MIi_CheckAnotherAutoDMA(int dmaNo, u32 mode);   // 0x02007910
extern void MIi_CheckDma0SourceAddress(int dmaNo, u32 src, u32 size, u32 dir);   // 0x02007994
extern void GxFifoDmaNext(void);
extern void GxFifoDmaEnd(void *arg);
extern void GxFastDmaEnd(void *arg);

#define REG_GXSTAT (*(vu32 *) 0x4000600)

// 0x020be068
static struct {
    volatile u32 busy;                // 0x00
    int dmaNo;               // 0x04
    u32 src;                 // 0x08
    u32 remaining;           // 0x0c
    void (*callback)(void *); // 0x10
    void *arg;               // 0x14
    u32 savedIrqMode;        // 0x18
    void *savedIrqFunc;      // 0x1c
} gxState;

void MI_SendGXCommandAsync(int dmaNo, const void *src, u32 size, void (*callback)(void *), void *arg) {
    OSIntrMode e;
    if (size == 0) {
        if (callback != NULL) {
            callback(arg);
        }
        return;
    }
    while (gxState.busy != 0) {
    }
    while (!(((REG_GXSTAT & 0x7000000) >> 24) & 2)) {
    }
    gxState.busy = 1;
    gxState.dmaNo = dmaNo;
    gxState.src = (u32) src;
    gxState.remaining = size;
    gxState.callback = callback;
    gxState.arg = arg;
    MIi_CheckDma0SourceAddress(dmaNo, (u32) src, size, 0);
    func_02007804(dmaNo);
    e = OS_DisableInterrupts();
    gxState.savedIrqMode = (REG_GXSTAT & 0xc0000000) >> 30;
    gxState.savedIrqFunc = OS_GetIrqFunction(0x200000);
    REG_GXSTAT = (REG_GXSTAT & ~0xc0000000) | 0x40000000;
    OS_SetIrqFunction(0x200000, GxFifoDmaNext);
    OS_EnableIrqMask(0x200000);
    GxFifoDmaNext();
    OS_RestoreInterrupts(e);
}

void GxFifoDmaNext(void) {
    u32 src;
    u32 count;
    if (gxState.remaining == 0) {
        return;
    }
    count = gxState.remaining;
    if (count >= 0x1d8) {
        count = 0x1d8;
    }
    src = gxState.src;
    gxState.remaining -= count;
    gxState.src += count;
    if (gxState.remaining == 0) {
        OSi_EnterDmaCallback(gxState.dmaNo, GxFifoDmaEnd, NULL);
        func_01ffcdbc(gxState.dmaNo, (const void *) src, (void *) 0x4000400, 0xc4400000 | (count >> 2));
        OS_ResetRequestIrqMask(0x200000);
    } else {
        func_01ffcdbc(gxState.dmaNo, (const void *) src, (void *) 0x4000400, 0x84400000 | (count >> 2));
        OS_ResetRequestIrqMask(0x200000);
    }
}

void GxFifoDmaEnd(void *arg) {
    void (*cb)(void *);
    void *cbArg;
    OS_DisableIrqMask(0x200000);
    REG_GXSTAT = (gxState.savedIrqMode << 30) | (REG_GXSTAT & ~0xc0000000);
    OS_SetIrqFunction(0x200000, (void (*)(void)) gxState.savedIrqFunc);
    gxState.busy = 0;
    cb = gxState.callback;
    cbArg = gxState.arg;
    if (cb != NULL) {
        cb(cbArg);
    }
}

void MI_SendGXCommandAsyncFast(int dmaNo, const void *src, u32 size, void (*callback)(void *), void *arg) {
    if (size == 0) {
        if (callback != NULL) {
            callback(arg);
        }
        return;
    }
    while (gxState.busy != 0) {
    }
    gxState.busy = 1;
    gxState.dmaNo = dmaNo;
    gxState.callback = callback;
    gxState.arg = arg;
    MIi_CheckAnotherAutoDMA(dmaNo, 0x38000000);
    MIi_CheckDma0SourceAddress(dmaNo, (u32) src, size, 0);
    func_02007804(dmaNo);
    OSi_EnterDmaCallback(dmaNo, GxFastDmaEnd, NULL);
    func_01ffcdbc(dmaNo, src, (void *) 0x4000400, 0xfc400000 | (size >> 2));
}

void GxFastDmaEnd(void *arg) {
    void (*cb)(void *);
    void *cbArg;
    gxState.busy = 0;
    cb = gxState.callback;
    cbArg = gxState.arg;
    if (cb != NULL) {
        cb(cbArg);
    }
}
