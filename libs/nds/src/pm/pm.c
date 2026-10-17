#include <nitro/private/sdk_tp.h>

extern void PmFifoReceive(u32 tag, u32 data, int err);
extern int PmTryLock(void);
extern void func_0200ceac(u32 *cmd, int n);
extern int func_0200cff8(u32 cmd, u32 data, u16 *res, void (*cb)(u32, u32 *), u32 *arg);
extern int func_0200d07c(u32 cmd, u32 data, u16 *res);
extern int func_0200d0c0(u32 a, void (*cb)(u32, u32 *), u32 *arg);
extern int func_0200d124(u32 a);
extern int func_0200d158(u32 a, u32 b, void (*cb)(u32, u32 *), u32 *arg);
extern int func_0200d1e0(u32 a, u32 b);
extern int func_0200d214(void (*cb)(u32, u32 *), u32 *res);
extern int func_0200d268(void);
extern int func_0200d308(u32 v);
extern int func_0200d354(u32 *a, u32 *b);
extern void PmFifoSend(u32 word);
extern int func_0200d6ec(int a, int b, int c, int d);
extern int func_0200d924(void);
extern void PmRunCallbacks(PMCallbackInfo *list);
extern void func_0200cc58(void);
extern void func_0200cc98(void);
extern void func_0200cce8(void);
extern u32 func_0200d2b4(void);
extern void PmPrependCallback(PMCallbackInfo **head, PMCallbackInfo *node);
extern void PmAppendCallback(PMCallbackInfo **head, PMCallbackInfo *node);
extern void PmRemoveCallback(PMCallbackInfo **head, PMCallbackInfo *node);
extern u32 OS_DisableIrqMask(u32 mask);       // 0x02004bc0
extern void OS_SetIrqMask(u32 mask);          // 0x02004b64
extern int OS_IsTickAvailable(void);
extern void SleepUntilVBlank(void);
extern void OS_Halt(void);                    // 0x02007530
extern void MI_StopAllDma(void);
extern int func_02011b28(void);
extern void func_0200da80(void);

#define REG_IME (*(vu16 *)0x04000208)
#define REG_POWCNT (*(vu16 *)0x04000304)

int PmTryLock(void)
{
    if (pmWork.lock != 0) return 0;
    pmWork.lock = 1;
    return 1;
}

void func_0200cc58(void)
{
    vu32 *l = (vu32 *)&pmWork.lock;
    if (pmWork.lock == 0) return;
    do {
        if (OS_GetProcMode() == 0x12) PXIi_HandlerRecvFifoNotEmpty();
    } while (*l != 0);
}

void func_0200cc98(void)
{
    vu32 *l = (vu32 *)&pmWork.lock;
    if (pmWork.lock == 0) return;
    do {
        if (OS_GetCpsrIrqBits() == 0x80 || REG_IME == 0) PXIi_HandlerRecvFifoNotEmpty();
    } while (*l != 0);
}

void func_0200cce8(void)
{
    vu32 *l = (vu32 *)&pmWork.lock;
    if (pmWork.lock == 0) return;
    do {
        if (OS_GetCpsrIrqBits() == 0x80 || REG_IME == 0 || OS_GetProcMode() == 0x12) PXIi_HandlerRecvFifoNotEmpty();
    } while (*l != 0);
}

void PmStoreResult(u32 v, u32 *p)
{
    if (p != NULL) *p = v;
}

void PmUnlockAndCallCallback(u32 result)
{
    void (*cb)(u32, void *) = pmWork.cb;
    void *arg = pmWork.cbArg;
    pmWork.lock = 0;
    if (cb == NULL) return;
    pmWork.cb = NULL;
    cb(result, arg);
}

void PmInit(void)
{
    if (pmWork.init) return;
    pmWork.init = 1;
    pmWork.lock = 0;
    pmWork.cb = NULL;
    PXI_Init();
    if (!PXI_IsCallbackReady(8, 1)) {
        while (1) {
            WaitByLoop(100);
            if (PXI_IsCallbackReady(8, 1)) break;
        }
    }
    PXI_SetFifoRecvCallback(8, PmFifoReceive);
    pmWork.vcount4 = pmWork.vcount8 = *(vu32 *)0x027ffc3c;
}

void PmFifoReceive(u32 tag, u32 data, int err)
{
    u16 cmd = (data & 0x7f00) >> 8;
    u16 arg = data & 0xff;
    if (err) {
        switch (cmd) {
        case 0x61:
        case 0x62:
            arg = 1;
            break;
        default:
            arg = 2;
            break;
        }
        PmUnlockAndCallCallback(arg);
        return;
    }
    switch (cmd) {
    case 0x61:
        if (pmWork.resultPtr) *pmWork.resultPtr = arg;
        arg = 0;
        break;
    case 0x60:
        arg = 0;
        break;
    case 0x62:
        break;
    case 0x63:
        pmWork.flag = 1;
        break;
    }
    PmUnlockAndCallCallback(arg);
}

void func_0200ceac(u32 *cmd, int n)
{
    vu32 res;
    int i;
    u32 e;
    while (1) {
        e = OS_DisableInterrupts();
        if (!PmTryLock()) {
            OS_RestoreInterrupts(e);
            continue;
        }
        res = 0xffff0000;
        pmWork.cb = (void (*)(u32, void *))PmStoreResult;
        pmWork.cbArg = (void *)&res;
        for (i = 0; i < n; i++) PmFifoSend(cmd[i]);
        OS_RestoreInterrupts(e);
        while (res == 0xffff0000) OS_SpinWait(0xa3a47);
        if (res == 0) return;
        OS_SpinWait(0xa3a47);
    }
}

int func_0200cf6c(u32 a, u32 b)
{
    u32 cmd[2];
    cmd[0] = 0x3006000;
    func_0200ceac(cmd, 1);
    while (func_0200d6ec(0, 2, 0, 1) != 1) {}
    cmd[0] = (a & 0xff) | 0x6200 | 0x2000000;
    cmd[1] = 0x1010000 | (u16)b;
    func_0200ceac(cmd, 2);
    return 0;
}

int func_0200cff8(u32 cmd, u32 data, u16 *res, void (*cb)(u32, u32 *), u32 *arg)
{
    u32 e = OS_DisableInterrupts();
    if (!PmTryLock()) {
        OS_RestoreInterrupts(e);
        return 1;
    }
    pmWork.cb = (void (*)(u32, void *))cb;
    pmWork.cbArg = arg;
    pmWork.resultPtr = res;
    PmFifoSend((cmd & 0xff) | 0x6100 | 0x2000000);
    PmFifoSend(0x1010000 | (u16)data);
    OS_RestoreInterrupts(e);
    return 0;
}

int func_0200d07c(u32 cmd, u32 data, u16 *res)
{
    u32 r;
    int ret = func_0200cff8(cmd, data, res, PmStoreResult, &r);
    if (ret != 0) return ret;
    pmWaitFunc();
    return r;
}

int func_0200d0c0(u32 a, void (*cb)(u32, u32 *), u32 *arg)
{
    u32 cmd;
    switch (a) {
    case 1: cmd = 1; break;
    case 3: cmd = 2; break;
    case 2: cmd = 3; break;
    default: cmd = 0; break;
    }
    if (cmd == 0) return 0xffff;
    return func_0200cff8(cmd, 0, 0, cb, arg);
}

int func_0200d124(u32 a)
{
    u32 r;
    int ret = func_0200d0c0(a, PmStoreResult, &r);
    if (ret != 0) return ret;
    pmWaitFunc();
    return r;
}

int func_0200d158(u32 a, u32 b, void (*cb)(u32, u32 *), u32 *arg)
{
    u32 cmd = 0;
    if (a == 0) {
        if (b == 1) cmd = 6;
        if (b == 0) cmd = 7;
    } else if (a == 1) {
        if (b == 1) cmd = 4;
        if (b == 0) cmd = 5;
    } else if (a == 2) {
        if (b == 1) cmd = 8;
        if (b == 0) cmd = 9;
    }
    if (cmd == 0) return 0xffff;
    return func_0200cff8(cmd, 0, 0, cb, arg);
}

int func_0200d1e0(u32 a, u32 b)
{
    u32 r;
    int ret = func_0200d158(a, b, PmStoreResult, &r);
    if (ret != 0) return ret;
    pmWaitFunc();
    return r;
}

int func_0200d214(void (*cb)(u32, u32 *), u32 *arg)
{
    void (*old)(void) = pmWaitFunc;
    pmWaitFunc = func_0200cce8;
    func_0200da80();
    pmWaitFunc = old;
    return func_0200cff8(0xe, 0, 0, cb, arg);
}

int func_0200d268(void)
{
    u32 r;
    int ret = func_0200d214(PmStoreResult, &r);
    if (ret != 0) return ret;
    pmWaitFunc = func_0200cc98;
    pmWaitFunc();
    pmWaitFunc = func_0200cc58;
    return r;
}

// Never returns: once func_0200d268 reports 0 it disables interrupts, calls MI_StopAllDma and loops on OS_Halt.
// Declared u32 as in card_dev.h because TerminateAfterCardPullout (card_pullout.c) tests a result; those tests
// never run.
u32 func_0200d2b4(void)
{
    if (func_0200d268() != 0) {
        do {
            OS_SpinWait(0xa3a47);
        } while (func_0200d268() != 0);
    }
    OS_DisableInterrupts();
    MI_StopAllDma();
    while (1) OS_Halt();
}

int func_0200d308(u32 v);
int func_0200d2f0(u32 v)
{
    pmWork.field10 = v;
    return func_0200d308(v);
}

int func_0200d308(u32 v)
{
    if (!func_0200d924()) return 0;
    return func_0200d07c(0x10, (u16)v, 0);
}

int func_0200d338(u32 v)
{
    return func_0200d07c(0x11, (u16)v, 0);
}

int func_0200d354(u32 *a, u32 *b)
{
    u16 st;
    int ret = func_0200d07c(0xf, 1, &st);
    if (ret != 0) return ret;
    if (a) *a = (st & 8) != 0;
    if (b) *b = (st & 4) != 0;
    return ret;
}

void PmFifoSend(u32 word)
{
    while (PXI_SendWordByFifo(8, word, 0) != 0) {}
}

void func_0200d3e0(u32 flags, u32 a, u32 b)
{
    int ok = 0;
    u32 dispA;
    u32 led0;
    u32 e;
    u32 mask;
    u32 led1;
    u32 dispB;
    u32 lcd;
    vu32 tick;
    u32 cur;
    u16 ime;
    u32 t;
    PmRunCallbacks(pmWork.preList);
    ime = REG_IME;
    REG_IME = 0;
    e = OS_DisableInterrupts();
    mask = OS_DisableIrqMask(0x3fffff);
    OS_SetIrqMask((OS_IsTickAvailable() ? 8 : 0) | 0x40000);
    OS_RestoreInterrupts(e);
    t = REG_IME;
    REG_IME = 1;
    if (flags & 8) {
        if (*(vu16 *)0x027ffc40 == 2) flags &= ~8;
    }
    if (flags & 0x10) {
        if (func_02011b28() == 0) flags &= ~0x10;
    }
    dispA = *(vu32 *)0x04000000;
    dispB = *(vu32 *)0x04001000;
    lcd = func_0200d924();
    while (func_0200d354(&led0, &led1)) OS_SpinWait(0xa3a47);
    while (func_0200d1e0(2, 0)) OS_SpinWait(0xa3a47);
    tick = *(vu32 *)0x027ffc3c;
    do { cur = *(vu32 *)0x027ffc3c; } while (tick == cur);
    tick = *(vu32 *)0x027ffc3c;
    *(vu32 *)0x04000000 &= ~0x30000;
    *(vu32 *)0x04001000 &= ~0x10000;
    do { cur = *(vu32 *)0x027ffc3c; } while (tick == cur);
    tick = *(vu32 *)0x027ffc3c;
    do { cur = *(vu32 *)0x027ffc3c; } while (tick == cur);
    pmWork.flag = 0;
    func_0200cf6c((u16)(flags | (led0 ? 0x40 : 0) | (led1 ? 0x80 : 0)), (u16)(a | b));
    while (pmWork.flag == 0) OS_Halt();
    if (flags & 8) {
        if (*(vu32 *)0x04000214 & 0x100000) ok = 1;
    }
    if (!ok) {
        if (lcd == 1) {
            while (func_0200d6ec(1, 1, 1, 1) != 1) {}
        } else {
            if (func_0200d124(1)) {
                do { OS_SpinWait(0xa3a47); } while (func_0200d124(1));
            }
        }
        *(vu32 *)0x04000000 = dispA;
        *(vu32 *)0x04001000 = dispB;
    }
    OS_SpinWait(0x360000);
    OS_DisableInterrupts();
    OS_SetIrqMask(mask);
    OS_RestoreInterrupts(e);
    t = REG_IME;
    REG_IME = ime;
    if (ok) func_0200d2b4();
    PmRunCallbacks(pmWork.postList);
}

int func_0200d6ec(int a, int b, int c, int d)
{
    switch (a) {
    case 1:
        if (c == 0) {
            if (*(vu32 *)0x027ffc3c - pmWork.vcount4 <= 7) return 0;
        }
        if (b) {
            if (d) {
                if (func_0200d124(b)) {
                    do { OS_SpinWait(0xa3a47); } while (func_0200d124(b));
                }
            } else {
                if (func_0200d0c0(b, 0, 0)) {
                    do { OS_SpinWait(0xa3a47); } while (func_0200d0c0(b, 0, 0));
                }
            }
        }
        REG_POWCNT |= 1;
        if (func_0200d308(pmWork.field10)) {
            do { OS_SpinWait(0xa3a47); } while (func_0200d308(pmWork.field10));
        }
        break;
    case 0:
        if (func_0200d308(0)) {
            do { OS_SpinWait(0xa3a47); } while (func_0200d308(0));
        }
        if (*(vu32 *)0x027ffc3c - pmWork.vcount8 <= 2) {
            SleepUntilVBlank();
            SleepUntilVBlank();
        }
        REG_POWCNT &= ~1;
        pmWork.vcount4 = *(vu32 *)0x027ffc3c;
        if (b) {
            if (d) {
                if (func_0200d124(b)) {
                    do { OS_SpinWait(0xa3a47); } while (func_0200d124(b));
                }
            } else {
                if (func_0200d0c0(b, 0, 0)) {
                    do { OS_SpinWait(0xa3a47); } while (func_0200d0c0(b, 0, 0));
                }
            }
        }
        break;
    }
    return 1;
}

int func_0200d904(int a)
{
    return func_0200d6ec(a == 1 ? a : 0, 0, 0, 1);
}

int func_0200d924(void)
{
    return (REG_POWCNT & 1) != 0;
}

void PmPrependCallback(PMCallbackInfo **head, PMCallbackInfo *node)
{
    if (head != NULL) {
        node->next = *head;
        *head = node;
    }
}

void PmAppendCallback(PMCallbackInfo **head, PMCallbackInfo *node)
{
    PMCallbackInfo *cur, *nx;
    if (head == NULL) return;
    cur = *head;
    if (cur == NULL) {
        node->next = NULL;
        *head = node;
        return;
    }
    nx = cur->next;
    while (nx != NULL) {
        cur = nx;
        nx = nx->next;
    }
    node->next = nx;
    cur->next = node;
}

void PmRemoveCallback(PMCallbackInfo **head, PMCallbackInfo *node)
{
    PMCallbackInfo *cur, *prev;
    u32 e;
    if (head == NULL) return;
    e = OS_DisableInterrupts();
    cur = *head;
    prev = cur;
    while (cur != NULL) {
        if (cur == node) {
            if (cur == prev) *head = cur->next;
            else prev->next = cur->next;
            break;
        }
        prev = cur;
        cur = cur->next;
    }
    OS_RestoreInterrupts(e);
}

void PmRunCallbacks(PMCallbackInfo *list)
{
    if (list == NULL) return;
    do {
        list->func(list->arg);
        list = list->next;
    } while (list != NULL);
}

void PmPrependPreCallback(PMCallbackInfo *node)
{
    PmPrependCallback(&pmWork.preList, node);
}

void PmAppendPostCallback(PMCallbackInfo *node)
{
    PmAppendCallback(&pmWork.postList, node);
}

void PmRemovePreCallback(PMCallbackInfo *node)
{
    PmRemoveCallback(&pmWork.preList, node);
}

void PmRemovePostCallback(PMCallbackInfo *node)
{
    PmRemoveCallback(&pmWork.postList, node);
}
