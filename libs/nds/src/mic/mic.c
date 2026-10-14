#include <nitro/private/sdk_tp.h>

extern int func_0200cae4(u32 buf, u32 size, u32 rate, u32 type);
extern int func_0200cbe8(void);
extern void func_0200cc10(u32 res, void *arg);
extern void func_0200cc20(void);
extern void func_0200c9bc(u32 tag, u32 data, int err);

void func_0200c744(void)
{
    if (micWork.init) return;
    micWork.init = 1;
    micWork.busy = 0;
    micWork.cb = NULL;
    PXI_Init();
    while (!PXI_IsCallbackReady(9, 1)) {}
    *(vu32 *)0x027fff90 = 0;
    PXI_SetFifoRecvCallback(9, func_0200c9bc);
}

u32 func_0200c7b4(MICAutoParam *p, void (*cb)(u32, void *), void *arg)
{
    u32 type, e;
    if ((u32)p->buffer & 0x1f) return 2;
    if (p->size & 0x1f) return 2;
    if (p->size == 0) return 2;
    if (p->rate < 0x400) return 2;
    switch (p->type) {
    case 0: type = 0; break;
    case 1: type = 1; break;
    case 2: type = 2; break;
    case 3: type = 3; break;
    case 4: type = 5; break;
    case 5: type = 7; break;
    default: return 2;
    }
    if (p->loop) type = (u8)(type | 0x10);
    else type = (u8)type;
    type = (u8)type;
    e = OS_DisableInterrupts();
    if (micWork.busy) {
        OS_RestoreInterrupts(e);
        return 1;
    }
    micWork.busy = 1;
    OS_RestoreInterrupts(e);
    micWork.cb = cb;
    micWork.cbArg = arg;
    micWork.fullCb = p->full;
    micWork.fullArg = p->fullArg;
    return func_0200cae4((u32)p->buffer, p->size, p->rate, type) ? 0 : 3;
}

u32 func_0200c8e0(MICAutoParam *p)
{
    micWork.result = func_0200c7b4(p, (void (*)(u32, void *))func_0200cc10, NULL);
    if (micWork.result == 0) func_0200cc20();
    return micWork.result;
}

u32 func_0200c918(void (*cb)(u32, void *), void *arg)
{
    u32 e = OS_DisableInterrupts();
    if (micWork.busy) {
        OS_RestoreInterrupts(e);
        return 1;
    }
    micWork.busy = 1;
    OS_RestoreInterrupts(e);
    micWork.cb = cb;
    micWork.cbArg = arg;
    return func_0200cbe8() ? 0 : 3;
}

u32 func_0200c974(void)
{
    micWork.result = func_0200c918((void (*)(u32, void *))func_0200cc10, NULL);
    if (micWork.result == 0) func_0200cc20();
    return micWork.result;
}

u32 func_0200c9ac(void)
{
    return *(vu32 *)0x027fff90;
}

void func_0200c9bc(u32 tag, u32 data, int err)
{
    u32 res;
    u16 cmd, r;
    if (err) {
        if (micWork.busy) micWork.busy = 0;
        if (micWork.cb) {
            void (*f)(u32, void *) = (void (*)(u32, void *))micWork.cb;
            micWork.cb = NULL;
            f(6, micWork.cbArg);
        }
    }
    cmd = (data & 0x7f00) >> 8;
    r = data & 0xff;
    switch (r) {
    case 0: res = 0; break;
    case 1: res = 4; break;
    case 2: res = 2; break;
    case 3: res = 5; break;
    case 4: res = 1; break;
    default: res = 6; break;
    }
    if (cmd == 0x51) {
        if (micWork.fullCb) micWork.fullCb(res, micWork.fullArg);
        return;
    }
    if (cmd == 0x40) {
        if (micWork.lastAddr) *micWork.lastAddr = *(vu16 *)0x027fff94;
    }
    if (micWork.busy) micWork.busy = 0;
    if (micWork.cb) {
        void (*f)(u32, void *) = (void (*)(u32, void *))micWork.cb;
        micWork.cb = NULL;
        f(res, micWork.cbArg);
    }
}

int func_0200cae4(u32 buf, u32 size, u32 rate, u32 type)
{
    if (PXI_SendWordByFifo(9, (type | 0x4100) | 0x2000000, 0) < 0) return 0;
    if (PXI_SendWordByFifo(9, (buf >> 16) | 0x10000, 0) < 0) return 0;
    if (PXI_SendWordByFifo(9, (u16)buf | 0x20000, 0) < 0) return 0;
    if (PXI_SendWordByFifo(9, (size >> 16) | 0x30000, 0) < 0) return 0;
    if (PXI_SendWordByFifo(9, (u16)size | 0x40000, 0) < 0) return 0;
    if (PXI_SendWordByFifo(9, (rate >> 16) | 0x50000, 0) < 0) return 0;
    return PXI_SendWordByFifo(9, 0x1060000 | (u16)rate, 0) >= 0;
}

int func_0200cbe8(void)
{
    return PXI_SendWordByFifo(9, 0x3004200, 0) >= 0;
}

void func_0200cc10(u32 res, void *arg)
{
    micWork.result = res;
}

extern vu32 data_020bfd50;   // micWork.busy as a pool symbol

asm void func_0200cc20(void)
{
    ldr ip, =data_020bfd50
loop:
    ldr r0, [ip, #0]
    cmp r0, #1
    beq loop
    bx lr
}
