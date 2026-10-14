#include <nitro/private/rtcdev.h>
int func_02011b28(void);
void WaitByLoop(u32 n);

typedef struct CTRDGTiming { s32 rom1st; s32 rom2nd; } CTRDGTiming;
typedef struct CTRDGLock { s32 locked; u32 irq; } CTRDGLock;

typedef struct CTRDGTask {
    void *func;       // 0x00
    void (*cb)(struct CTRDGTask *); // 0x04
    u32 result;       // 0x08
    u32 pad[6];
} CTRDGTask;          // 0x24

typedef struct CTRDGWork {
    u32 f00, f04;
    u32 init;         // 0x08
    u32 f0c;          // 0x0c
    u32 f10, f14;
    u32 (*cb)(void);  // 0x18
} CTRDGWork;

extern u32 data_020c1180;
extern CTRDGWork data_020c11a0;

typedef struct { u16 flag; u16 id; } CTRDGState;
extern CTRDGState data_020c1184;
extern u8 data_020c11c0[];
extern u8 data_020c11c4[];
extern u32 data_020c1280;
extern u8 data_020c1790[];
extern u32 data_020c136c[];
typedef struct { void *work; u8 pad[0x22]; u8 f26; } CTRDGGlobal;
extern CTRDGGlobal data_020c1368;
void MIi_CpuCopy32(const void *src, void *dst, u32 size); // MI word copy (ITCM)
void func_02012070(u32 tag, u32 data);
void func_0201209c(u32 tag, u32 data);
void func_0201210c(u32 tag, u32 data);
void func_02011e7c(void);
void func_020120f8(void);
void func_02012120(u32 *p);
void func_020121ac(void *p);
void func_020121c0(u32 *w);
void func_02011ab0(void);

#define EXMEMCNT (*(vu16 *)0x04000204)

void func_02011c34(CTRDGTiming *t)
{
    t->rom1st = (EXMEMCNT & 0xc) >> 2;
    t->rom2nd = (EXMEMCNT & 0x10) >> 4;
    EXMEMCNT = (EXMEMCNT & ~0xc) | 0xc;
    EXMEMCNT = EXMEMCNT & ~0x10;
}

void func_02011c7c(CTRDGTiming *t)
{
    EXMEMCNT = (t->rom1st << 2) | (EXMEMCNT & ~0xc);
    EXMEMCNT = (t->rom2nd << 4) | (EXMEMCNT & ~0x10);
}

void func_02011cb0(u16 id, CTRDGLock *l)
{
    for (;;) {
        l->irq = OS_DisableInterrupts();
        l->locked = OS_ReadOwnerOfLockWord((void *)0x027fffe8) & 0x40;
        if (l->locked) return;
        if (OS_TryLockCartridge(id) == 0) return;
        OS_RestoreInterrupts(l->irq);
        WaitByLoop(1);
    }
}

void func_02011d0c(u16 id, CTRDGLock *l)
{
    if (l->locked == 0) OS_UnLockCartridge(id);
    OS_RestoreInterrupts(l->irq);
}

void func_02011d30(u32 data)
{
    while (PXI_SendWordByFifo(0xd, data, 0) != 0) {
        WaitByLoop(1);
    }
}

void func_02011d80(u32 v)
{
    u32 e = OS_DisableInterrupts();
    data_020c1180 = v;
    if (func_02011ae8() == 0) {
        func_02006860(0xf000, v ? 0x1000 : 0x5000);
    }
    OS_RestoreInterrupts(e);
}

void func_02011dc8(void)
{
    if (data_020c11a0.init != 0) return;
    data_020c11a0.init = 1;
    func_02011ab0();
    data_020c11a0.f0c = 0;
    PXI_Init();
    while (!PXI_IsCallbackReady(0xd, 1)) {}
    PXI_SetFifoRecvCallback(0xd, (void *)func_02012070);
    func_02011e7c();
    PXI_SetFifoRecvCallback(0xd, 0);
    PXI_SetFifoRecvCallback(0xd, (void *)func_0201209c);
    data_020c11a0.cb = 0;
    func_02012120(&data_020c1280);
    PXI_SetFifoRecvCallback(0x11, (void *)func_0201210c);
    func_02011d80(0);
}

void func_02011e7c(void)
{
    CTRDGLock lock;
    CTRDGTiming timing;
    u32 irq;
    u16 ime;
    s32 cart;
    u16 post;
    u16 imeold;
    if (data_020c11a0.f00 != 0) return;
    post = *(vu16 *)0x04000300;
    data_020c11a0.f00 = 1;
    if (!(post & 1)) return;
    irq = OS_SetIrqMask(0x40000);
    ime = *(vu16 *)0x04000208;
    *(vu16 *)0x04000208 = 1;
    func_02011cb0(data_020c1184.id, &lock);
    cart = (EXMEMCNT & 0x8000) >> 15;
    func_02011c34(&timing);
    EXMEMCNT &= ~0x8000;
    DC_InvalidateRange(data_020c11c0 + 0x80, 0x40);
    MI_DmaCopy16(1, 0x8000080, data_020c11c0 + 0x80, 0x40);
    EXMEMCNT = (EXMEMCNT & ~0x8000) | (cart << 15);
    func_02011c7c(&timing);
    func_02011d0c(data_020c1184.id, &lock);
    if (*(u8 *)0x027fff9b == 0 && *(u8 *)0x027fff9a != 0) {
    } else {
        u8 *hdr = data_020c11c0;
        u16 *dst = (u16 *)0x027ffc30;
        int i;
        dst[0] = *(u16 *)(hdr + 0xbe);
        i = 0;
        while (i < 3) {
            ((u8 *)dst)[2 + i] = hdr[0xb5 + i];
            i++;
        }
        dst[3] = *(u16 *)(hdr + 0xb0);
        *(u32 *)(dst + 4) = *(u32 *)(hdr + 0xac);
        *(u8 *)0x027fff9b = func_02011b28() != 0;
        *(u8 *)0x027fff9a = 1;
    }
    MIi_CpuCopy32((void *)0xffff0020, data_020c11c4, 0x9c);
    func_02005e30();
    func_02011d30((((u32)data_020c11c0 - 0x02000000) >> 5 << 6) | 1);
    while (data_020c1184.flag != 1) {
        WaitByLoop(1);
    }
    imeold = *(vu16 *)0x04000208;
    *(vu16 *)0x04000208 = ime;
    OS_SetIrqMask(irq);
}

void func_02012070(u32 tag, u32 data)
{
    if ((data & 0x3f) == 1) {
        data_020c1184.flag = 1;
        return;
    }
    OS_Terminate();
}

void func_0201209c(u32 tag, u32 data)
{
    if ((data & 0x3f) == 0x11) {
        u32 r;
        if (data_020c11a0.f0c != 0) return;
        r = 0;
        if (data_020c11a0.cb) r = data_020c11a0.cb();
        if (r) func_020120f8();
        data_020c11a0.f0c = 1;
        return;
    }
    OS_Terminate();
}

void func_020120f8(void)
{
    func_02011d30(2);
    OS_Terminate();
}

void func_0201210c(u32 tag, u32 data)
{
    data_020c11a0.f04 = 0;
}

void func_02012120(u32 *p)
{
    u32 e = OS_DisableInterrupts();
    if (data_020c1368.work == 0) {
        data_020c1368.work = p;
        func_020121ac(p + 0x31);
        func_020121ac(data_020c136c);
        p[0x30] = 0;
        OS_CreateThread(p, (void *)func_020121c0, p, data_020c1790, 0x400, 0x14);
        OS_WakeupThreadDirect(p);
    }
    OS_RestoreInterrupts(e);
}

void func_020121ac(void *p)
{
    MI_CpuFill8(p, 0, 0x24);
}

void func_020121c0(u32 *w)
{
    CTRDGTask t;
    u32 e;
    void (*cb)(CTRDGTask *);
    for (;;) {
        MI_CpuFill8(&t, 0, 0x24);
        e = OS_DisableInterrupts();
        while (((vu32 *)w)[0x30] == 0) {
            OS_SleepThread(0);
        }
        t = *(CTRDGTask *)w[0x30];
        OS_RestoreInterrupts(e);
        if (t.func) t.result = ((u32 (*)(CTRDGTask *))t.func)(&t);
        e = OS_DisableInterrupts();
        cb = t.cb;
        data_020c1368.f26 = 0;
        if (cb) cb(&t);
        if (data_020c1368.work == 0) break;
        w[0x30] = 0;
        OS_RestoreInterrupts(e);
    }
    OS_ExitThread();
}
