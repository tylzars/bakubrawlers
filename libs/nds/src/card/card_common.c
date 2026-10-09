#include <nitro/private/card_dev.h>
extern u8 data_020bfe60[];
extern CardRomReq data_020c04a0;

int func_0200e5a4(void);
int func_0200e5f0(void);
u32 func_0200e56c(void);
void func_0200e57c(void);
void func_0200e34c(u32 id, u32 type);
void func_0200e3d0(u32 id, u32 type);
void func_0200e310(void *task);
void func_0200e690(s32 id);
void func_0200ea14(CardCommon *c);
u32 func_0200ecec(void);
int func_0200ee44(void);
int func_0200ee38(void);
int func_0200ee50(CardRomReq *r);
void func_0200eedc(u32 a, u32 b);
void func_0200ef3c(void);
void func_0200ef90(void);
int func_0200f060(CardRomReq *r);
void func_0200f1ec(CardRomReq *r);
u32 func_0200f2dc(void);
void func_0200f338(void);
int func_0200f52c(void);
void *func_0200f538(void);
void func_0200f544(u32 tag, u32 data, int err);
void func_0200f578(void);
int func_0200f5c8(CardCommon *c, u32 cmd, int tries);
void func_0200f71c(void);
void func_0200f838(u32 id);
void func_0200e45c(void);
void func_0200e594(u32);

void func_0200e310(void *task)
{
    CardCommon *c = (CardCommon *)data_020bfe60;
    OS_SetThreadPriority(&c->thread, c->priority);
    c->curThread = &c->thread;
    c->task = (void (*)(CardCommon *))task;
    c->f114 |= CF_TASK;
    OS_WakeupThreadDirect(&c->thread);
}

void func_0200e34c(u32 id, u32 type)
{
    CardCommon *c = (CardCommon *)data_020bfe60;
    u32 e = OS_DisableInterrupts();
    if (c->owner == id) {
        if (c->lockTarget != type) {
            OS_Terminate();
        }
    } else {
        for (;;) {
            if (c->owner == -3) {
                break;
            }
            OS_SleepThread(&c->lockQueue);
        }
        c->owner = id;
        c->lockTarget = type;
    }
    c->lockCount++;
    c->arg->result = 0;
    OS_RestoreInterrupts(e);
}

void func_0200e3d0(u32 id, u32 type)
{
    CardCommon *c = (CardCommon *)data_020bfe60;
    u32 e = OS_DisableInterrupts();
    if (c->owner != id || c->lockCount == 0) {
        OS_Terminate();
    } else {
        if (c->lockTarget != type) {
            OS_Terminate();
        }
        if (--c->lockCount == 0) {
            c->owner = -3;
            c->lockTarget = 0;
            OS_WakeupThread(&c->lockQueue);
        }
    }
    c->arg->result = 0;
    OS_RestoreInterrupts(e);
}

static inline BOOL IsNitro(void)
{
    return *(vu16 *)0x27ffc40 == 2;
}

void func_0200e45c(void)
{
    CardCommon *c = (CardCommon *)data_020bfe60;
    c->owner = -3;
    c->lockCount = 0;
    c->lockTarget = 0;
    c->arg = &data_020bfe00;
    MI_CpuClearFast(&data_020bfe00, 0x60);
    DC_StoreRange(&data_020bfe00, 0x60);
    c->f118 = -1;
    c->f11c = -1;
    if (!IsNitro()) {
        MI_CpuCopy8((void *)0x27ffe00, (void *)0x27ffa80, 0x160);
    }
    c->priority = 4;
    c->lockQueue.b = 0;
    c->lockQueue.a = 0;
    c->queue.b = 0;
    c->queue.a = 0;
    OS_CreateThread(&c->thread, func_0200f578, 0, &data_020c0480, 0x400, c->priority);
    OS_WakeupThreadDirect(&c->thread);
    PXI_SetCallback(0xb, func_0200f544);
    if (IsNitro()) {
        return;
    }
    func_0200e594(1);
}

u32 func_0200e56c(void)
{
    return data_020bfde0;
}

void func_0200e57c(void)
{
    if (func_0200e56c() == 0) {
        OS_Terminate();
    }
}

void func_0200e594(u32 v)
{
    data_020bfde0 = v;
}

#pragma optimize_for_size on
#pragma dont_inline on
int func_0200e5a4(void)
{
    CardCommon *c = (CardCommon *)data_020bfe60;
    u32 e = OS_DisableInterrupts();
    while (c->f114 & CF_BUSY) {
        OS_SleepThread(&c->queue);
    }
    OS_RestoreInterrupts(e);
    return c->arg->result == 0;
}
#pragma dont_inline reset
#pragma optimize_for_size reset

int func_0200e5f0(void)
{
    return (((CardCommon *)data_020bfe60)->f114 & CF_BUSY) == 0;
}

u32 func_0200e60c(void)
{
    return ((CardCommon *)data_020bfe60)->arg->result;
}

void func_0200e620(u32 id)
{
    func_0200e34c(id, 1);
    OS_LockCard(id);
}

void func_0200e63c(u32 id)
{
    OS_UnlockCard(id);
    func_0200e3d0(id, 1);
}

void func_0200e658(u32 id)
{
    func_0200e34c(id, 2);
}

void func_0200e668(u32 id)
{
    if (!func_0200ee44()) {
        func_0200ee38();
    }
    func_0200e3d0(id, 2);
}

void func_0200e690(s32 id)
{
    CardArg *a = ((CardCommon *)data_020bfe60)->arg;
    u32 size;
    u32 type;
    u32 variant;
    MI_CpuFill8(&a->f18, 0, 0x48);
    a->id = id;
    a->f58 = 0x203f;
    if (id == 0) {
        return;
    }
    size = 1 << ((id >> 8) & 0xff);
    type = id & 0xff;
    variant = (id >> 16) & 0xff;
    a->f18 = size;
    a->f54 = 0xff;
    if (type == 1) {
        switch (size) {
        case 0x200:
            a->f24 = 0x10;
            a->f28 = 1;
            a->f2c = 5;
            a->f54 = 0xf0;
            break;
        case 0x2000:
            a->f24 = 0x20;
            a->f28 = 2;
            a->f2c = 5;
            a->f54 = 0;
            break;
        case 0x10000:
            a->f24 = 0x80;
            a->f28 = 2;
            a->f2c = 10;
            a->f54 = 0;
            break;
        case 0x20000:
            a->f24 = 0x100;
            a->f28 = 3;
            a->f2c = 5;
            a->f54 = 0;
            break;
        default:
            goto fail;
        }
        a->f1c = a->f24;
        a->f58 |= 0x40;
        a->f58 |= 0x4300;
        return;
    } else if (type == 2) {
        switch (size) {
        case 0x40000:
        case 0x80000:
        case 0x100000:
            a->f30 = 0x19;
            a->f34 = 0x12c;
            a->f50 = 0x12c;
            a->f40 = 0x1388;
            a->f58 |= 0x480;
            break;
        case 0x200000:
            a->f30 = 0x17;
            a->f34 = 0x12c;
            a->f40 = 0x1f4;
            a->f44 = 0x1388;
            a->f38 = a->f44 << 1;
            a->f3c = 0xea60;
            a->f54 = 0;
            a->f58 |= 0x80;
            a->f58 |= 0x5400;
            break;
        case 0x400000:
            a->f40 = 0x258;
            a->f44 = 0xbb8;
            a->f48 = 0x46;
            a->f4c = 0x96;
            a->f38 = 0x59d8;
            a->f3c = 0xc3500;
            a->f54 = 0;
            a->f20 = 0x1000;
            a->f58 |= 0xd000;
            break;
        case 0x800000:
            if (variant == 0) {
                a->f40 = 0x3e8;
                a->f44 = 0xbb8;
                a->f38 = 0x109a0;
                a->f3c = 0x27100;
                a->f54 = 0;
                a->f58 |= 0x5000;
            } else if (variant == 1) {
                a->f40 = 0x3e8;
                a->f44 = 0xbb8;
                a->f38 = 0x109a0;
                a->f3c = 0x27100;
                a->f54 = 0x84;
                a->f58 |= 0x5000;
            }
            break;
        default:
            goto fail;
        }
        a->f1c = 0x10000;
        a->f24 = 0x100;
        a->f28 = 3;
        a->f2c = 5;
        a->f58 |= 0xb40;
        return;
    } else if (type == 3) {
        if (size == 0x2000 || size == 0x8000) {
            a->f24 = size;
            a->f1c = size;
            a->f28 = 2;
            a->f54 = 0;
            a->f58 |= 0x40;
            a->f58 |= 0x4300;
            return;
        }
    }
fail:
    a->id = 0;
    a->f18 = 0;
    ((CardCommon *)data_020bfe60)->arg->result = 3;
}

void func_0200ea14(CardCommon *c)
{
    u32 mode;
    u32 chunk;
    u8 *buf;
    u32 cmd;
    u32 len;
    u32 retry;
    void *cbarg;
    void (*fn)(void *);
    u32 e;
    cmd = c->cmd;
    mode = c->mode;
    retry = c->retry;
    chunk = 0x100;
    func_02000b9c((void *)0x2000bc4);
    if (cmd == 0xb) {
        chunk = func_0200ecec();
    } else if (cmd == 0xf) {
        chunk = ((CardCommon *)data_020bfe60)->arg->f20;
    }
    buf = c->buf;
    do {
        len = c->len;
        if (chunk < len) {
            len = chunk;
        }
        c->arg->f14 = len;
        if (c->f114 & CF_CANCEL) {
            c->f114 &= ~CF_CANCEL;
            c->arg->result = 7;
            break;
        }
        switch (mode) {
        case 0:
            DC_InvalidateRange(buf, len);
            c->arg->f0c = c->src;
            c->arg->f10 = (u32)buf;
            break;
        case 1:
        case 2:
            MI_CpuCopy8((void *)c->src, buf, len);
            DC_StoreRange(buf, len);
            DC_WaitWriteBufferEmpty();
            c->arg->f0c = (u32)buf;
            c->arg->f10 = c->dst;
            break;
        case 3:
            c->arg->f0c = c->src;
            c->arg->f10 = c->dst;
            break;
        }
        if (!func_0200f5c8(c, cmd, retry)) {
            break;
        }
        if (mode == 2) {
            if (!func_0200f5c8(c, 9, 1)) {
                break;
            }
        } else if (mode == 0) {
            MI_CpuCopy8(buf, (void *)c->dst, len);
        }
        c->src += len;
        c->dst += len;
        c->len -= len;
    } while (c->len != 0);
    fn = c->callback;
    cbarg = c->cbArg;
    e = OS_DisableInterrupts();
    c->f114 &= ~0x4c;
    OS_WakeupThread(&c->queue);
    if (c->f114 & CF_TASKWAIT) {
        OS_WakeupThreadDirect(&c->thread);
    }
    OS_RestoreInterrupts(e);
    if (fn) {
        fn(cbarg);
    }
}

int func_0200ec04(u32 src, void *dst, u32 len, void (*cb)(void *), void *cbarg, int async, u32 cmd, u32 retry, u32 mode)
{
    CardCommon *c = (CardCommon *)data_020bfe60;
    u32 e;
    func_02000b9c((void *)0x2000bc4);
    e = OS_DisableInterrupts();
    while (c->f114 & CF_BUSY) {
        OS_SleepThread(&c->queue);
    }
    c->f114 |= CF_BUSY;
    c->callback = cb;
    c->cbArg = cbarg;
    OS_RestoreInterrupts(e);
    c->src = src;
    c->dst = (u32)dst;
    c->len = len;
    c->cmd = cmd;
    c->retry = retry;
    c->mode = mode;
    if (async) {
        func_0200e310(func_0200ea14);
        return 1;
    }
    ((CardCommon *)data_020bfe60)->curThread = OSi_ThreadInfo.curThread;
    func_0200ea14(c);
    return c->arg->result == 0;
}

u32 func_0200ecec(void)
{
    return ((CardCommon *)data_020bfe60)->arg->f1c;
}

int func_0200ed00(u32 id)
{
    CardCommon *c = (CardCommon *)data_020bfe60;
    u32 e;
    u32 e2;
    void *arg;
    void (*fn)(void *);
    func_02000b9c((void *)0x2000bc4);
    if (id == 0) {
        OS_Terminate();
    }
    func_0200e57c();
    e = OS_DisableInterrupts();
    while (c->f114 & CF_BUSY) {
        OS_SleepThread(&c->queue);
    }
    c->f114 |= CF_BUSY;
    c->callback = NULL;
    c->cbArg = NULL;
    OS_RestoreInterrupts(e);
    func_0200e690(id);
    ((CardCommon *)data_020bfe60)->curThread = OSi_ThreadInfo.curThread;
    func_0200f5c8(c, 2, 1);
    c->arg->f0c = 0;
    c->arg->f10 = (u32)c->buf;
    c->arg->f14 = 1;
    func_0200f5c8(c, 6, 1);
    fn = c->callback;
    arg = c->cbArg;
    e2 = OS_DisableInterrupts();
    c->f114 &= ~0x4c;
    OS_WakeupThread(&c->queue);
    if (c->f114 & CF_TASKWAIT) {
        OS_WakeupThreadDirect(&c->thread);
    }
    OS_RestoreInterrupts(e2);
    if (fn) {
        fn(arg);
    }
    return c->arg->result == 0;
}

int func_0200ee38(void)
{
    return func_0200e5a4();
}

int func_0200ee44(void)
{
    return func_0200e5f0();
}

int func_0200ee50(CardRomReq *r)
{
    CardCommon *c = (CardCommon *)data_020bfe60;
    u32 page = c->src & ~0x1ff;
    if (page == r->page) {
        u32 off = c->src - page;
        u32 n = 0x200 - off;
        if (n > c->len) {
            n = c->len;
        }
        MI_CpuCopy8(r->cache + off, (void *)c->dst, n);
        c->src += n;
        c->dst += n;
        c->len -= n;
    }
    return c->len != 0;
}

void func_0200eedc(u32 a, u32 b)
{
    volatile u8 *p;
    while (*(vu32 *)0x40001a4 & 0x80000000) {
    }
    p = (volatile u8 *)0x40001a1;
    p[0] = 0xc0;
    p[7] = a >> 24;
    p[8] = a >> 16;
    p[9] = a >> 8;
    p[10] = a;
    p[11] = b >> 24;
    p[12] = b >> 16;
    p[13] = b >> 8;
    p[14] = b;
}

void func_0200ef3c(void)
{
    CardCommon *c = (CardCommon *)data_020bfe60;
    CardRomReq *r = &data_020c04a0;
    func_02007cd8(c->dma, 0x4100010, c->dst, 0x200);
    func_0200eedc((c->src >> 8) | 0xb7000000, c->src << 24);
    *(vu32 *)0x40001a4 = r->ctrl;
}

void func_0200ef90(void)
{
    CardCommon *c = (CardCommon *)data_020bfe60;
    MI_StopDma(c->dma);
    c->src += 0x200;
    c->dst += 0x200;
    if ((c->len -= 0x200) != 0) {
        func_0200ef3c();
    } else {
        void (*fn)(void *);
        void *arg;
        u32 e;
        OS_DisableIrqMask(0x80000);
        OS_ResetRequestIrqMask(0x80000);
        func_0200f838(func_0200f2dc());
        c->arg->result = 0;
        fn = c->callback;
        arg = c->cbArg;
        e = OS_DisableInterrupts();
        c->f114 &= ~0x4c;
        OS_WakeupThread(&c->queue);
        if (c->f114 & CF_TASKWAIT) {
            OS_WakeupThreadDirect(&c->thread);
        }
        OS_RestoreInterrupts(e);
        if (fn) {
            fn(arg);
        }
    }
}

int func_0200f060(CardRomReq *r)
{
    CardCommon *c = (CardCommon *)data_020bfe60;
    u32 dst = c->dst;
    u32 len = c->len;
    u32 mis = dst & 0x1f;
    int ok = 0;
    int pageAligned = 0;
    int noTcm = 0;
    int dmaOk = 0;
    if (mis == 0 && c->dma <= 3) {
        dmaOk = 1;
    }
    if (dmaOk) {
        u32 dtcm = (u32)func_0200682c();
        u32 end = dst + len;
        int inItcm = 0;
        int tcm = 1;
        if (end > 0x1ff8000 && dst < 0x2000000) {
            inItcm = 1;
        }
        if (!inItcm) {
            if (dtcm >= end || dtcm + 0x4000 <= dst) {
                tcm = 0;
            }
        }
        if (!tcm) {
            noTcm = 1;
        }
    }
    if (noTcm) {
        if (((c->src | len) & 0x1ff) == 0) {
            pageAligned = 1;
        }
    }
    r->ctrl = (data_020a506c->romctrl & ~0x7000000) | 0xa1000000;
    if (pageAligned && len != 0) {
        u32 e;
        ok = 1;
        e = OS_DisableInterrupts();
        if (len < c->f118) {
            DC_FlushRange((void *)dst, len);
        } else {
            DC_FlushAll();
        }
        if (len < c->f11c) {
            if (mis != 0) {
                dst -= mis;
                DC_StoreRange2((void *)dst, 0x20);
                DC_StoreRange2((void *)(dst + len), 0x20);
                len += 0x20;
            }
            DC_InvalidateRange((void *)dst, len);
            DC_WaitWriteBufferEmpty();
        } else {
            DC_InvalidateAll();
        }
        OS_SetIrqFunction(0x80000, func_0200ef90);
        OS_ResetRequestIrqMask(0x80000);
        OS_EnableIrqMask(0x80000);
        OS_RestoreInterrupts(e);
        func_0200ef3c();
    }
    return ok;
}

void func_0200f1ec(CardRomReq *r)
{
    CardCommon *c = (CardCommon *)data_020bfe60;
    for (;;) {
        u32 page = c->src & ~0x1ff;
        u32 *dst;
        u32 i = 0;
        u32 n = 0;
        u32 st;
        if (page != c->src || ((u32)(dst = (u32 *)c->dst) & 3) || c->len < 0x200) {
            r->page = page;
            dst = (u32 *)r->cache;
        }
        func_0200eedc((page >> 8) | 0xb7000000, page << 24);
        *(vu32 *)0x40001a4 = r->ctrl;
        do {
            st = *(vu32 *)0x40001a4;
            if (st & 0x800000) {
                u32 w = *(vu32 *)0x4100010;
                if (n < 0x200) {
                    dst[i] = w;
                    n += 4;
                    i++;
                }
            }
        } while (st & 0x80000000);
        if ((u32)dst == c->dst) {
            c->src += 0x200;
            c->dst += 0x200;
            if ((c->len -= 0x200) == 0) {
                return;
            }
        } else {
            if (!func_0200ee50(r)) {
                return;
            }
        }
    }
}

u32 func_0200f2dc(void)
{
    u32 ctrl;
    func_0200eedc(0xb8000000, 0);
    ctrl = (data_020a506c->romctrl & ~0x7000000) | 0xa7000000;
    *(vu32 *)0x40001a4 = ctrl & ~0x1fff;
    while (!(*(vu32 *)0x40001a4 & 0x800000)) {
    }
    return *(vu32 *)0x4100010;
}

void func_0200f338(void)
{
    CardCommon *c = (CardCommon *)data_020bfe60;
    CardRomReq *r = &data_020c04a0;
    void (*fn)(void *);
    void *arg;
    u32 e;
    if (func_0200ee50(r)) {
        r->func(r);
    }
    func_0200f838(func_0200f2dc());
    c->arg->result = 0;
    fn = c->callback;
    arg = c->cbArg;
    e = OS_DisableInterrupts();
    c->f114 &= ~0x4c;
    OS_WakeupThread(&c->queue);
    if (c->f114 & CF_TASKWAIT) {
        OS_WakeupThreadDirect(&c->thread);
    }
    OS_RestoreInterrupts(e);
    if (fn) {
        fn(arg);
    }
}

void func_0200f3d0(u32 dma, u32 src, void *dst, u32 len, void (*cb)(void *), void *cbarg, int async)
{
    CardRomReq *r = &data_020c04a0;
    CardCommon *c = (CardCommon *)data_020bfe60;
    u32 e;
    func_0200e57c();
    e = OS_DisableInterrupts();
    while (c->f114 & CF_BUSY) {
        OS_SleepThread(&c->queue);
    }
    c->f114 |= CF_BUSY;
    c->callback = cb;
    c->cbArg = cbarg;
    OS_RestoreInterrupts(e);
    c->dma = dma;
    c->dst = (u32)dst;
    c->src = src + data_020c0480.base;
    c->len = len;
    if (dma <= 3) {
        MI_StopDma(dma);
    }
    if (func_0200f060(r)) {
        if (async) {
            return;
        }
        func_0200f52c();
        return;
    }
    if (async) {
        func_0200e310(func_0200f338);
        return;
    }
    c->curThread = OSi_ThreadInfo.curThread;
    func_0200f338();
}

void func_0200f4c8(void)
{
    CardCommon *c = (CardCommon *)data_020bfe60;
    if (c->f114 != 0) {
        return;
    }
    c->f114 = 1;
    c->len = 0;
    c->dst = 0;
    c->src = 0;
    c->dma = -1;
    c->callback = NULL;
    c->cbArg = NULL;
    data_020c0480.base = 0;
    func_0200e45c();
    data_020c0480.romfn = func_0200f538();
    func_0200f71c();
}

int func_0200f52c(void)
{
    return func_0200e5a4();
}

void *func_0200f538(void)
{
    return func_0200f1ec;
}

void func_0200f544(u32 tag, u32 data, int err)
{
    CardCommon *c = (CardCommon *)data_020bfe60;
    if (tag != 0xb) {
        return;
    }
    if (err == 0) {
        return;
    }
    c->f114 &= ~CF_PXIWAIT;
    OS_WakeupThreadDirect(c->curThread);
}

void func_0200f578(void)
{
    CardCommon *c = (CardCommon *)data_020bfe60;
    for (;;) {
        u32 e = OS_DisableInterrupts();
        while (!(c->f114 & CF_TASK)) {
            OS_SleepThread(NULL);
        }
        OS_RestoreInterrupts(e);
        c->task(c);
    }
}

int func_0200f5c8(CardCommon *c, u32 cmd, int tries)
{
    u32 e;
    if (!(c->f114 & CF_PXI)) {
        c->f114 |= CF_PXI;
        if (!PXI_IsCallbackReady(0xb, 1)) {
            do {
                OS_SpinWait(100);
            } while (!PXI_IsCallbackReady(0xb, 1));
        }
        func_0200f5c8(c, 0, 1);
    }
    DC_StoreRange(c->arg, 0x60);
    DC_WaitWriteBufferEmpty();
    for (;;) {
        c->f04 = cmd;
        c->f114 |= CF_PXIWAIT;
        while (PXI_SendWord(0xb, cmd, 1) < 0) {
        }
        if (cmd == 0) {
            CardArg *a = c->arg;
            while (PXI_SendWord(0xb, (u32)a, 1) < 0) {
            }
        }
        e = OS_DisableInterrupts();
        while (c->f114 & CF_PXIWAIT) {
            OS_SleepThread(NULL);
        }
        OS_RestoreInterrupts(e);
        DC_InvalidateRange(c->arg, 0x60);
        if (c->arg->result != 4) {
            break;
        }
        if (--tries <= 0) {
            break;
        }
    }
    return c->arg->result == 0;
}
