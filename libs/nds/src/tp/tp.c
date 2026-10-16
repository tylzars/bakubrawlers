#include <nitro/private/sdk_tp.h>

#define TP_SHARED ((vu16 *)0x027fffaa)
#define NVRAM_TP ((vu16 *)0x027ffc80)

extern int TP_CalcCalibrateParam(TPCalibrateParam *p, u16 x1, u16 y1, u16 px1, u16 py1, u16 x2, u16 y2, u16 px2, u16 py2);
extern void func_0200c72c(u32 mask);

typedef union { TPRawBits b; u16 h[2]; } TPRawUnion;

// PXI receive callback for the touch panel (tag 6)
void func_0200be84(u32 tag, u32 data, int err)
{
    u16 cmd = ((u16)data & 0x7f00) >> 8;
    if (err) {
        tpWork.err |= 1 << cmd;
        if (tpWork.callback) tpWork.callback(cmd, 4, 0);
        return;
    }
    if (cmd == 0x10) {
        TPRawUnion u;
        TPData *e;
        tpWork.index++;
        if (tpWork.index >= tpWork.bufSize) tpWork.index = 0;
        u.h[0] = TP_SHARED[0];
        u.h[1] = TP_SHARED[1];
        e = &tpWork.buf[tpWork.index];
        e->x = u.b.x;
        e->y = u.b.y;
        e->touch = (u8)u.b.touch;
        e->validity = (u8)u.b.validity;
        if (tpWork.callback) tpWork.callback(cmd, 0, tpWork.index & 0xff);
        return;
    }
    if (!(data & 0x1000000)) return;
    {
        u32 res = (u16)data & 0xff;
        u32 r;
        switch (res) {
        case 0:
            switch (cmd) {
            case 0: {
                TPRawUnion u;
                u.h[0] = TP_SHARED[0];
                u.h[1] = TP_SHARED[1];
                tpWork.raw.x = u.b.x;
                tpWork.raw.y = u.b.y;
                tpWork.raw.touch = (u8)u.b.touch;
                tpWork.raw.validity = (u8)u.b.validity;
                tpWork.state = 0;
                break; }
            case 1:
                tpWork.state = 2;
                break;
            case 2:
                tpWork.state = 0;
                break;
            }
            tpWork.busy &= ~(1 << cmd);
            if (tpWork.callback) tpWork.callback(cmd, 0, 0);
            return;
        case 4: r = 3; break;
        case 2: r = 1; break;
        case 3: r = 2; break;
        default:
            goto term;
        }
        tpWork.err |= 1 << cmd;
        tpWork.busy &= ~(1 << cmd);
        if (tpWork.callback) tpWork.callback(cmd, r & 0xff, 0);
        return;
    }
term:
    OS_Terminate();
}

void TP_Init(void)
{
    if (tpWork.initialized) return;
    tpWork.initialized = 1;
    PXI_Init();
    tpWork.index = 0;
    tpWork.callback = NULL;
    tpWork.buf = NULL;
    tpWork.state = 0;
    tpWork.calibrate = 0;
    tpWork.busy = 0;
    tpWork.err = 0;
    while (!PXI_IsCallbackReady(6, 1)) {}
    PXI_SetFifoRecvCallback(6, func_0200be84);
}

typedef struct { u8 pad[0x58]; u16 x1, y1; u8 px1, py1; u16 x2, y2; u8 px2, py2; } TPNvCal;

int TP_GetUserInfo(TPCalibrateParam *p)
{
    TPNvCal *c = (TPNvCal *)0x027ffc80;
    u16 ax1 = c->x1, ay1 = c->y1;
    u16 ax2 = c->x2, ay2 = c->y2;
    u8 px1 = c->px1, py1 = c->py1, px2 = c->px2, py2 = c->py2;
    if ((ax1 == 0 && ax2 == 0 && ay1 == 0 && ay2 == 0) || TP_CalcCalibrateParam(p, ax1, ay1, px1, py1, ax2, ay2, px2, py2) != 0) {
        p->x0 = 0;
        p->y0 = 0;
        p->xDotSize = 0;
        p->yDotSize = 0;
        return 1;
    }
    return 1;
}

void TP_SetCalibrateParam(const TPCalibrateParam *p)
{
    u32 e;
    s32 xd, yd;
    if (p == NULL) {
        tpWork.calibrate = 0;
        return;
    }
    e = OS_DisableInterrupts();
    xd = p->xDotSize;
    if (xd != 0) {
        REG_DIVCNT = 0;
        REG_DIV_NUMER = 0x10000000;
        *(vu64 *)0x04000298 = (u32)xd;
        tpWork.cal.x0 = p->x0;
        tpWork.cal.dx = p->xDotSize;
        while (REG_DIVCNT & 0x8000) {}
        tpWork.cal.xs = REG_DIV_RESULT;
    } else {
        tpWork.cal.x0 = 0;
        tpWork.cal.dx = 0;
        tpWork.cal.xs = 0;
    }
    yd = p->yDotSize;
    if (yd != 0) {
        REG_DIVCNT = 0;
        REG_DIV_NUMER = 0x10000000;
        *(vu64 *)0x04000298 = (u32)yd;
        tpWork.cal.y0 = p->y0;
        tpWork.cal.dy = p->yDotSize;
        while (REG_DIVCNT & 0x8000) {}
        tpWork.cal.ys = REG_DIV_RESULT;
    } else {
        tpWork.cal.y0 = 0;
        tpWork.cal.dy = 0;
        tpWork.cal.ys = 0;
    }
    OS_RestoreInterrupts(e);
    tpWork.calibrate = 1;
}

static inline BOOL TpSendFifo(u32 data)
{
    return PXI_SendWordByFifo(6, data, 0) >= 0;
}

void func_0200c338(void)
{
    u32 e = OS_DisableInterrupts();
    if (!TpSendFifo(0x3000000)) {
        OS_RestoreInterrupts(e);
        tpWork.err |= 1;
        if (tpWork.callback) tpWork.callback(0, 4, 0);
        return;
    }
    tpWork.busy |= 1;
    tpWork.err &= ~1;
    OS_RestoreInterrupts(e);
}

int func_0200c3cc(TPData *out)
{
    func_0200c72c(1);
    if (tpWork.err & 1) return 1;
    *out = tpWork.raw;
    return 0;
}

int TP_CalcCalibrateParam(TPCalibrateParam *p, u16 x1, u16 y1, u16 px1, u16 py1, u16 x2, u16 y2, u16 px2, u16 py2)
{
    u32 e;
    s32 r, dy, ty;
    u16 lx2, lpx2, lpy2, lpy1, ly2;
    if (!(x1 < 0x1000 && y1 < 0x1000 && x2 < 0x1000 && y2 < 0x1000)) return 1;
    if (!(px1 < 0x100 && px2 < 0x100 && py1 < 0xc0 && py2 < 0xc0)) return 1;
    if (px1 == px2 || py1 == py2 || x1 == x2 || y1 == y2) return 1;
    e = OS_DisableInterrupts();
    lx2 = x2;
    lpx2 = px2;
    lpy2 = py2;
    lpy1 = py1;
    ly2 = y2;
    REG_DIVCNT = 0;
    REG_DIV_NUMER = (x1 - lx2) << 8;
    *(vu64 *)0x04000298 = (u32)(px1 - lpx2);
    ty = y1 - ly2;
    dy = lpy1 - lpy2;
    while (REG_DIVCNT & 0x8000) {}
    r = REG_DIV_RESULT;
    REG_DIVCNT = 0;
    REG_DIV_NUMER = ty << 8;
    *(u64 *)0x04000298 = (u32)dy;
    if (r >= 0x8000 || r < -0x8000) {
        OS_RestoreInterrupts(e);
        return 1;
    }
    p->xDotSize = r;
    // The halfword conversion becomes a shift pair (lsl #9, asr #16) in the backend, and the register for its first
    // shift is numbered after the shared -0x8000 constant, so it is coloured first: that puts the y0 value in r0 and
    // the constant in r1. With the conversion the range checks below can no longer fail in C terms, but they are still
    // emitted. The cast may stand in for a different construct with the same codegen.
    {
        s32 v = (s16)((((x1 + lx2) << 8) - p->xDotSize * (px1 + lpx2)) >> 7);
        if (v >= 0x8000 || v < -0x8000) {
            OS_RestoreInterrupts(e);
            return 1;
        }
        p->x0 = v;
    }
    while (REG_DIVCNT & 0x8000) {}
    r = REG_DIV_RESULT;
    OS_RestoreInterrupts(e);
    if (r >= 0x8000 || r < -0x8000) return 1;
    p->yDotSize = r;
    {
        s32 v = (s16)((((y1 + ly2) << 8) - p->yDotSize * (lpy1 + lpy2)) >> 7);
        if (v >= 0x8000 || v < -0x8000) return 1;
        p->y0 = v;
    }
    return 0;
}

void TP_GetCalibratedPoint(TPData *out, const TPData *raw)
{
    s32 *c;
    s32 v;
    if (!tpWork.calibrate) {
        *out = *raw;
        return;
    }
    c = &tpWork.cal.x0;
    out->touch = raw->touch;
    out->validity = raw->validity;
    if (raw->touch == 0) {
        out->x = 0;
        out->y = 0;
        return;
    }
    out->x = ((s64)c[2] * ((s64)(raw->x << 2) - c[0])) >> 22;
    v = (s16)out->x;
    if (v < 0) out->x = 0;
    else if (v > 0xff) out->x = 0xff;
    out->y = ((s64)c[5] * ((s64)(raw->y << 2) - c[3])) >> 22;
    v = (s16)out->y;
    if (v < 0) out->y = 0;
    else if (v > 0xbf) out->y = 0xbf;
}

void func_0200c72c(u32 mask)
{
    while ((*(vu16 *)&tpWork.busy & mask) != 0) {}
}
