#include <nitro/private/card_dev.h>
extern WMState data_020c06e0;
extern u8 data_020c06e8[];
extern u8 data_020c0708[];
extern u8 data_020c0780[];
void func_0200fdfc(void);
int func_0200f914(void *buf, u32 dma, u32 size);
int func_0200fd04(void);
int func_0200fd68(int n, ...);
void *func_0200fb80(void);

int func_0200f8e8(void *buf, u32 dma)
{
    int r = func_0200f914(buf, dma, 0xf00);
    if (r == 0) {
        data_020c06e0.work->f16 = 0;
    }
    return r;
}

#pragma optimize_for_size on
#pragma dont_inline on
int func_0200f914(void *buf, u32 dma, u32 size)
{
    u32 e = OS_DisableInterrupts();
    WMWork *w;
    int i;
    if (data_020c06e0.init != 0) {
        OS_RestoreInterrupts(e);
        return 3;
    }
    if (buf == NULL) {
        OS_RestoreInterrupts(e);
        return 6;
    }
    if (dma > 3) {
        OS_RestoreInterrupts(e);
        return 6;
    }
    if ((u32)buf & 0x1f) {
        OS_RestoreInterrupts(e);
        return 6;
    }
    PXI_Init();
    if (!PXI_IsCallbackReady(0xa, 1)) {
        OS_RestoreInterrupts(e);
        return 4;
    }
    DC_InvalidateRange(buf, size);
    MI_DmaFill32(dma, buf, 0, size);
    data_020c06e0.work = (WMWork *)buf;
    *(u32 *)buf = (u32)buf + 0x200;
    data_020c06e0.work->p04 = data_020c06e0.work->p00 + 0x300;
    data_020c06e0.work->p0c = data_020c06e0.work->p04 + 0x800;
    data_020c06e0.work->p10 = data_020c06e0.work->p0c + 0x100;
    func_020101a8();
    data_020c06e0.work->dma = dma;
    data_020c06e0.work->f14c = 0;
    data_020c06e0.work->f150 = 0;
    for (i = 0; i < 16; i++) {
        data_020c06e0.work->a[i] = 0;
        data_020c06e0.work->b[i] = 0;
    }
    OS_InitMessageQueue(data_020c06e8, data_020c0708, 10);
    for (i = 0; i < 10; i++) {
        *(u16 *)(data_020c0780 + i * 0x100) = 0x8000;
        DC_StoreRange2(data_020c0780 + i * 0x100, 2);
        OS_SendMessage(data_020c06e8, data_020c0780 + i * 0x100, 1);
    }
    PXI_SetCallback(0xa, func_0200fdfc);
    data_020c06e0.init = 1;
    OS_RestoreInterrupts(e);
    return 0;
}
#pragma dont_inline reset
#pragma optimize_for_size reset

int func_0200faf8(void)
{
    u32 e = OS_DisableInterrupts();
    int r;
    if (func_0200fd04() != 0) {
        OS_RestoreInterrupts(e);
        return 3;
    }
    r = func_0200fd68(1, 0);
    if (r != 0) {
        return r;
    }
    func_020101a8();
    PXI_SetCallback(0xa, NULL);
    data_020c06e0.work = NULL;
    data_020c06e0.init = 0;
    OS_RestoreInterrupts(e);
    return 0;
}

void func_0200fb68(u32 idx, u32 cb)
{
    data_020c06e0.work->cb[idx] = cb;
}

void *func_0200fb80(void)
{
    u16 *msg;
    if (!OS_ReceiveMessage(data_020c06e8, &msg, 0)) {
        return NULL;
    }
    DC_InvalidateRange(msg, 2);
    if (*msg & 0x8000) {
        return msg;
    }
    OS_JamMessage(data_020c06e8, msg, 1);
    return NULL;
}

int func_0200fbd8(u32 cmd, u16 argc, ...)
{
    u32 *ap;
    int i;
    int r;
    u32 *msg = (u32 *)func_0200fb80();
    if (msg == NULL) {
        return 8;
    }
    *(u16 *)msg = cmd;
    ap = (u32 *)(((u32)&argc + 3 & ~3) + 4);
    for (i = 0; i < argc; i++) {
        msg[1 + i] = *ap++;
    }
    DC_StoreRange2(msg, 0x100);
    r = PXI_SendWord(0xa, (u32)msg, 0);
    OS_SendMessage(data_020c06e8, msg, 1);
    return r < 0 ? 8 : 2;
}

int func_0200fc84(void *src, u32 len)
{
    int r;
    void *msg = func_0200fb80();
    if (msg == NULL) {
        return 8;
    }
    MI_CpuCopy8(src, msg, len);
    DC_StoreRange2(msg, len);
    r = PXI_SendWord(0xa, (u32)msg, 0);
    OS_SendMessage(data_020c06e8, msg, 1);
    return r < 0 ? 8 : 2;
}

void *func_0200fcf4(void)
{
    return data_020c06e0.work;
}

int func_0200fd04(void)
{
    if (data_020c06e0.init != 0) {
        return 0;
    }
    return 3;
}

int func_0200fd20(void)
{
    int r = func_0200fd04();
    if (r != 0) {
        return r;
    }
    DC_InvalidateRange((void *)data_020c06e0.work->p04, 2);
    if (*(u16 *)data_020c06e0.work->p04 <= 1) {
        return 3;
    }
    return 0;
}

int func_0200fd68(int n, ...)
{
    int r = func_0200fd04();
    u32 *ap;
    u32 st;
    if (r != 0) {
        return r;
    }
    DC_InvalidateRange((void *)data_020c06e0.work->p04, 2);
    st = *(u16 *)data_020c06e0.work->p04;
    ap = (u32 *)(((u32)&n + 3 & ~3) + 4);
    r = 3;
    if (n == 0) {
        return 3;
    }
    do {
        if (*ap++ == st) {
            r = 0;
        }
    } while (--n != 0);
    return r;
}
