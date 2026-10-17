#include <nitro/private/card_dev.h>
extern u8 data_020c06c0[];
void CardPulloutFifoReceive(u32 tag, u32 data, int err);
void func_0200f7ac(void);
void func_0200f898(u32 a, u32 b);

void InitCardPullout(void)
{
    PXI_Init();
    PXI_SetFifoRecvCallback(0xe, CardPulloutFifoReceive);
    ((CardPullout *)data_020c06c0)->callback = NULL;
}

#define PO ((CardPullout *)data_020c06c0)
void CardPulloutFifoReceive(u32 tag, u32 data, int err)
{
    if ((data & 0x3f) == 0x11) {
        BOOL r;
        if (PO->pulledOut != 0) {
            return;
        }
        PO->pulledOut = 1;
        r = 1;
        if (PO->callback != NULL) {
            r = PO->callback();
        }
        if (r == 0) {
            return;
        }
        func_0200f7ac();
    } else {
        OS_Terminate();
    }
}

BOOL IsCardPulledOut(void)
{
    return ((CardPullout *)data_020c06c0)->pulledOut;
}

void func_0200f7ac(void)
{
    int doit = 1;
    MI_StopDma(0);
    MI_StopDma(1);
    MI_StopDma(2);
    MI_StopDma(3);
    if (((s32)(*(vu16 *)0x27fffa8 & 0x8000)) >> 15) {
        u32 r = func_0200d2b4();
        if (r == 4) {
            do {
                OS_SpinWait(0xa3a47);
                r = func_0200d2b4();
            } while (r == 4);
        }
        if (r == 0) {
            doit = 0;
        }
    }
    if (doit) {
        func_0200f898(1, 1);
    }
    OS_Terminate();
}

void func_0200f838(u32 id)
{
    vu32 cur;
    u32 e;
    u32 *p;
    if (*(vu16 *)0x27ffc10 == 0) {
        p = (u32 *)0x27ff800;
    } else {
        p = (u32 *)0x27ffc00;
    }
    cur = *p;
    if (id == cur) {
        return;
    }
    e = OS_DisableInterrupts();
    CardPulloutFifoReceive(0xe, 0x11, 0);
    OS_RestoreInterrupts(e);
}

void func_0200f898(u32 a, u32 b)
{
    if (PXI_SendWordByFifo(0xe, a, 0) == 0) {
        return;
    }
    do {
        WaitByLoop(b);
    } while (PXI_SendWordByFifo(0xe, a, 0) != 0);
}
