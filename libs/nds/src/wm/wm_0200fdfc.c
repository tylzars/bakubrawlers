// WM wireless / ctrdg-detect library, 0x0200fdfc-0x02011c34 (2.0/sp1p5 -O4,p -ipa file -enum int)
#include <nitro/private/wm_dev.h>
#define H(b, i) (((u16 *)((b) + 0x800))[i])
typedef struct { u16 aidBitmap; u8 pad[0x1fe]; } WMDataSet;  /* 0x200 bytes, one of 4 at the start of WMDataSharingInfo */
void func_0200fdfc(u32 tag, u16 *m, int err)
{
    WMWork *w = data_020c06e0.work;
    u16 id;
    WMCb cb;
    if (err) return;
    DC_InvalidateRange(w->recv, 0x100);
    if (w->h16 == 0) DC_InvalidateRange(w->st, 0x800);
    if (m != w->recv) DC_InvalidateRange(m, 0x100);
    id = m[0];
    if (id >= 0x2c) {
        if (id == 0x80) {
            if (m[1] == 0x13) OS_Terminate();
            if (w->cbInd) w->cbInd(m);
        } else if (id == 0x82) {
            u16 i = m[3];
            if (w->indCb[i]) {
                *(u32*)(m + 14) = w->indArg[i];
                m[0x11] = w->bitmap;
                DC_InvalidateRange(*(void**)(m + 4), w->st->h72);
                w->indCb[m[3]](m);
            }
        } else if (id == 0x81) {
            m[0] = 0xf;
            if (*(WMCb*)(m + 14)) (*(WMCb*)(m + 14))(m);
        }
    } else {
        if (id == 0xe && (u16)(m[2] + 0xfff5) <= 1 && m[1] == 0)
            DC_InvalidateRange(*(void**)(m + 4), w->st->h72);
        if (m[0] == 2 && m[1] == 0) {
            cb = w->cbTable[m[0]];
            func_0200faf8();
            if (cb) cb(m);
            return;
        }
        cb = w->cbTable[m[0]];
        if (cb) {
            cb(m);
            if (data_020c06e0.h00 == 0) return;
        }
        if (m[0] == 8 || m[0] == 12) {
            u16 extra;
            u16 kind;
            u16 aid;
            u16 * data;
            u16 * mac;
            u16 i;
            u16 mac2;
            u16 a;
            u16 b;
            if (m[0] == 8) {
                mac = m + 5; kind = m[4]; aid = m[8]; data = m + 10; mac2 = 0;
                extra = m[9]; a = m[0x16]; b = m[0x17];
            } else if (m[0] == 12) {
                kind = m[4]; mac2 = m[5]; extra = m[6]; aid = 0; data = 0;
                mac = m + 8; a = m[0xb]; b = m[0xc];
            }
            if (kind == 7 || kind == 9 || kind == 0x1a) {
                if (kind == 7) w->bitmap = w->bitmap | (1 << aid);
                else w->bitmap = w->bitmap & ~(1 << aid);
                w->h150 = mac2;
                MI_CpuFill8(&data_020c06e0.ind, 0, 0x44);
                data_020c06e0.ind.id = 0x82;
                data_020c06e0.ind.h2 = 0;
                data_020c06e0.ind.h4 = kind;
                data_020c06e0.ind.w8 = 0;
                data_020c06e0.ind.wc = 0;
                data_020c06e0.ind.h10 = 0;
                data_020c06e0.ind.h12 = aid;
                data_020c06e0.ind.h20 = mac2;
                data_020c06e0.ind.h22 = w->bitmap;
                data_020c06e0.ind.h1a = 0xffff;
                data_020c06e0.ind.h3c = extra;
                MI_CpuCopy8(mac, data_020c06e0.ind.mac, 6);
                if (data) MIi_CpuCopy16(data, data_020c06e0.ind.data, 0x18);
                else MIi_CpuClear16(0, data_020c06e0.ind.data, 0x18);
                data_020c06e0.ind.h40 = mac2 == 0 ? a : b;
                if (mac2 != 0) b = a;
                data_020c06e0.ind.h42 = b;
                for (i = 0; i < 16; i++) {
                    data_020c06e0.ind.h6 = i;
                    if (w->indCb[i]) {
                        data_020c06e0.ind.w1c = w->indArg[i];
                        w->indCb[i](&data_020c06e0.ind.id);
                    }
                }
            }
        }
    }
    DC_InvalidateRange(w->recv, 0x100);
    func_020101a8();
    if (m == w->recv) return;
    m[0] |= 0x8000;
    DC_StoreRange(m, 0x100);
}

void func_020101a8(void)
{
    u16 *p = (u16 *)0x27fff96;
    u16 v = *p;
    if (v & 1) {
        *p = v & ~1;
    }
}

u16 func_020101c4(void)
{
    u32 e = OS_DisableInterrupts();
    WMWork *w = data_020c06e0.work;
    u16 r;
    if (w != 0) {
        r = w->h150;
    } else {
        r = 0;
    }
    OS_RestoreInterrupts(e);
    return r;
}

u16 func_020101f4(void)
{
    u32 e = OS_DisableInterrupts();
    WMWork *w = data_020c06e0.work;
    u32 r;
    if (w != 0) {
        r = w->bitmap;
    } else {
        r = 0;
    }
    OS_RestoreInterrupts(e);
    return (u16)r;
}

int func_02010224(u32 idx, WMCb cb, u32 arg)
{
    WMInd ind;
    u32 e;
    int r;
    WMWork *w;
    if (cb != 0) {
        MI_CpuFill8(&ind, 0, 0x44);
        ind.id = 0x82;
        ind.h2 = 0;
        ind.h4 = 0x19;
        ind.h6 = idx;
        ind.w8 = 0;
        ind.wc = 0;
        ind.h10 = 0;
        ind.h1a = 0xffff;
        ind.w1c = arg;
        ind.h12 = 0;
        OS_GetMacAddress(ind.mac);
    }
    e = OS_DisableInterrupts();
    r = func_0200fd04();
    if (r != 0) {
        OS_RestoreInterrupts(e);
        return r;
    }
    w = func_0200fcf4();
    w->indCb[idx] = cb;
    w->indArg[idx] = arg;
    if (cb != 0) {
        ind.h22 = func_020101f4();
        ind.h20 = func_020101c4();
        cb(&ind);
    }
    OS_RestoreInterrupts(e);
    return 0;
}

int func_020102fc(void)
{
    WMWork *w = func_0200fcf4();
    if (func_0200fd68(2, 7, 8) != 0) {
        return 0;
    }
    DC_InvalidateRange(&w->st->w0c, 4);
    if (w->st->w0c == 1) {
        return 0;
    }
    DC_InvalidateRange(&w->st->h3c, 4);
    return (w->st->h3c + 0x1f) & ~0x1f;
}

int func_02010368(void)
{
    WMWork *w = func_0200fcf4();
    u32 r, t;
    if (func_0200fd68(2, 7, 8) != 0) {
        return 0;
    }
    DC_InvalidateRange(&w->st->w0c, 4);
    if (w->st->w0c == 1) {
        return 0;
    }
    DC_InvalidateRange(&w->st->h188, 2);
    r = w->st->h188 == 0 ? 1 : 0;
    DC_InvalidateRange(&w->st->h3e, 2);
    t = w->st->h3e;
    if (r == 1) {
        DC_InvalidateRange(&w->st->hf8, 2);
        return (((t + 12) * w->st->hf8 + 0x29) & ~0x1f) << 1;
    }
    return ((t + 0x51) & ~0x1f) << 1;
}

u32 func_0201042c(void)
{
    if (func_0200fd04() != 0) {
        return 0x8000;
    }
    return *(u16 *)0x27ffcfa;
}

u32 func_0201044c(void)
{
    WMWork *w = func_0200fcf4();
    if (func_0200fd04() != 0) {
        return 0;
    }
    DC_InvalidateRange(w->st, 2);
    switch (w->st->h00) {
    case 9:
        DC_InvalidateRange(&w->st->h182, 2);
        if (w->st->h182 == 0) {
            return 0;
        }
    case 10:
    case 11:
        DC_InvalidateRange(&w->st->hbc, 2);
        return w->st->hbc;
    default:
        return 0;
    }
}

u16 func_020104dc(void)
{
    u8 mac[8];
    u16 sum;
    u16 t;
    int i;
    OS_GetMacAddress(mac);
    i = 0;
    sum = 0;
    for (; i < 6; i++) {
        sum = sum + mac[i];
    }
    t = sum + *(u32 *)0x27ffc3c;
    t = t * 7;
    return 200 + t % 20;
}

u16 func_0201056c(void)
{
    u8 mac[8];
    int i;
    u16 sum;
    u16 t;
    OS_GetMacAddress(mac);
    i = 0;
    sum = 0;
    for (; i < 6; i++) {
        sum = sum + mac[i];
    }
    t = sum + *(u32 *)0x27ffc3c;
    t = t * 13;
    return 30 + t % 10;
}

extern u32 data_020a5070;
u16 func_02010600(void)
{
    u32 buf[3];
    if (data_020a5070 == 0x10000) {
        RTC_Init();
        if (RTC_GetDate(buf) == 0) {
            data_020a5070 = (u16)(buf[2] + (buf[1] << 8));
        }
    }
    data_020a5070 = (u16)(data_020a5070 + 1);
    return data_020a5070;
}

int func_02010684(WMCb cb, int b, int c, int d);

int func_02010674(WMCb cb, int b, int c)
{
    return func_02010684(cb, b, c, 0);
}

int func_02010684(WMCb cb, int b, int c, int d)
{
    int r;
    WMWork *w;
    r = func_0200f8e8((int)cb, c);
    if (r != 0) {
        return r;
    }
    func_0200fb68(0, (WMCb)b);
    w = func_0200fcf4();
    r = func_0200fbd8(0, 4, w->f00, w->st, w->recv, d);
    if (r == 0) {
        r = 2;
    }
    return r;
}

int func_020106e4(WMCb cb)
{
    int r = func_0200fd20();
    if (r != 0) {
        return r;
    }
    func_0200fb68(1, cb);
    r = func_0200fbd8(1, 0);
    if (r == 0) {
        r = 2;
    }
    return r;
}

int func_0201071c(WMCb cb)
{
    int r = func_0200fd68(1, 2);
    if (r != 0) {
        return r;
    }
    func_0200fb68(2, cb);
    r = func_0200fbd8(2, 0);
    if (r == 0) {
        r = 2;
    }
    return r;
}

typedef struct WMParent {
    void *w0;
    u16 h4;
    u8 pad6[0xe];
    u16 h14;
    u8 pad16[2];
    u16 h18;
    u8 pad1a[0x18];
    u16 h32;
    u16 h34;
    u16 h36;
} WMParent;

// Defined in the original function order, after some of their callers.
int func_02010838(WMParent *p);
int func_0201075c(WMCb cb, WMParent *p);
void func_02011450(u16 *m);
int func_02011108(u8 *buf, void *pa, void *pb);
u8 *func_020119bc(u8 *buf, u32 mask, u8 *base, u32 n);
u8 *func_02011968(u8 *buf, u16 *p, u32 n);
int func_02011b10(void);
int func_02011b28(void);
int func_02011ae8(void);

int func_0201075c(WMCb cb, WMParent *p)
{
    int r = func_0200fd68(1, 2);
    if (r != 0) {
        return r;
    }
    if (p == 0) {
        return 6;
    }
    if (p->h4 != 0) {
        if (p->w0 == 0) {
            return 6;
        }
    }
    if (p->h34 + (p->h14 != 0 ? 0x2a : 0) > 0x200 || p->h36 + (p->h14 != 0 ? 6 : 0) > 0x200) {
        return 6;
    }
    func_02010838(p);
    func_0200fb68(7, cb);
    DC_StoreRange(p, 0x40);
    if (p->h4 != 0) {
        DC_StoreRange(p->w0, p->h4);
    }
    r = func_0200fbd8(7, 1, p);
    if (r == 0) {
        r = 2;
    }
    return r;
}

int func_02010838(WMParent *p)
{
    if (p->h4 > 0x70) {
        return 0;
    }
    if (p->h18 < 10 || p->h18 > 0x3e8) {
        return 0;
    }
    if (p->h32 < 1 || p->h32 > 14) {
        return 0;
    }
    return 1;
}

int func_02010888(WMCb cb, int x)
{
    int r = func_0200fd68(1, 2);
    WMWork *w;
    if (r != 0) {
        return r;
    }
    w = func_0200fcf4();
    w->h150 = 0;
    w->bitmap = 0;
    func_0200fb68(8, cb);
    r = func_0200fbd8(8, 1, x);
    if (r == 0) {
        r = 2;
    }
    return r;
}

int func_020108e4(WMCb cb)
{
    return func_02010888(cb, 1);
}

typedef struct WMScanParam {
    void *w0;
    u16 h4;
    u16 h6;
    u8 b8[6];
} WMScanParam;

typedef struct WMScanMsg {
    u16 id;
    u16 h2;
    void *w4;
    u16 h8;
    u8 b[6];
} WMScanMsg;

int func_020108f4(WMCb cb, WMScanParam *p)
{
    WMScanMsg m;
    int r = func_0200fd68(3, 2, 3, 5);
    if (r != 0) {
        return r;
    }
    if (p == 0) {
        return 6;
    }
    if (p->w0 == 0) {
        return 6;
    }
    if (p->h4 < 1 || p->h4 > 14) {
        return 6;
    }
    func_0200fb68(10, cb);
    m.id = 10;
    m.h2 = p->h4;
    m.w4 = p->w0;
    m.h8 = p->h6;
    m.b[0] = p->b8[0];
    m.b[1] = p->b8[1];
    m.b[2] = p->b8[2];
    m.b[3] = p->b8[3];
    m.b[4] = p->b8[4];
    m.b[5] = p->b8[5];
    r = func_0200fc84(&m, 0x10);
    if (r == 0) {
        r = 2;
    }
    return r;
}

typedef struct WMConnMsg {
    u16 id;
    u16 pad2;
    void *w4;
    u8 b8[0x18];
    u32 w20;
    u16 pad24;
    u16 h26;
} WMConnMsg;

int func_020109e0(WMCb cb, u16 *p, void *q, u32 arg, u16 extra)
{
    WMConnMsg m;
    int r = func_0200fd68(1, 2);
    WMWork *w;
    if (r != 0) {
        return r;
    }
    if (p == 0) {
        return 6;
    }
    DC_StoreRange(p, p[0] * 2);
    w = func_0200fcf4();
    w->h150 = 0;
    w->bitmap = 0;
    func_0200fb68(12, cb);
    m.id = 12;
    m.w4 = p;
    if (q != 0) {
        MI_CpuCopy8(q, m.b8, 0x18);
    } else {
        MI_CpuFill8(m.b8, 0, 0x18);
    }
    m.w20 = arg;
    m.h26 = extra;
    r = func_0200fc84(&m, 0x28);
    if (r == 0) {
        r = 2;
    }
    return r;
}

int func_02010aac(WMCb cb, u32 a, int b, u32 c, u16 e, void *f)
{
    WMWork *w = func_0200fcf4();
    WMStatus *st = w->st;
    u32 buf[16];
    int r = func_0200fd68(2, 7, 8);
    if (r != 0) {
        return r;
    }
    DC_InvalidateRange(&st->h188, 2);
    DC_InvalidateRange(&st->hc6, 2);
    if (st->h188 != 0 && st->hc6 != 1) {
        return 3;
    }
    DC_InvalidateRange(&st->w0c, 4);
    if (st->w0c == 1) {
        return 3;
    }
    if (b & 0x3f) {
        return 6;
    }
    if (e & 0x1f) {
        return 6;
    }
    DC_InvalidateRange(&st->h9c, 2);
    if (st->h9c == 0) {
        if (b < func_02010368()) {
            return 6;
        }
        if (e < func_020102fc()) {
            return 6;
        }
    }
    func_0200fb68(0xe, cb);
    MIi_CpuClear32(0, buf, 0x40);
    ((u16 *)buf)[0] = 0xe;
    buf[1] = a;
    buf[2] = (u32)b >> 1;
    buf[3] = c;
    buf[4] = e;
    MIi_CpuClear32(0, &buf[5], 0x1c);
    MIi_CpuCopy32(f, &buf[12], 0x10);
    r = func_0200fc84(buf, 0x40);
    if (r == 0) {
        r = 2;
    }
    return r;
}

int func_02010c1c(WMCb cb, u32 a, u32 b, u32 c, u16 e, u16 f)
{
    u32 t[4];
    MIi_CpuClear32(0, t, 0x10);
    t[0] = 3;
    ((u16 *)t)[2] = f;
    ((u16 *)t)[3] = f;
    return func_02010aac(cb, a, b, c, e, t);
}

int func_02010c84(WMCb cb, void *p)
{
    u32 buf[8];
    int r;
    func_0200fcf4();
    r = func_0200fd04();
    if (r != 0) {
        return r;
    }
    func_0200fb68(0x23, cb);
    MIi_CpuClear32(0, buf, 0x20);
    *(u16 *)buf = 0x23;
    MIi_CpuCopy32(p, &buf[1], 0x1c);
    r = func_0200fc84(buf, 0x20);
    if (r == 0) {
        r = 2;
    }
    return r;
}

int func_02010cf8(WMCb cb, u16 h)
{
    u32 t[7];
    MIi_CpuClear32(0, t, 0x1c);
    t[0] = 3;
    ((u16 *)t)[2] = h;
    ((u16 *)t)[3] = h;
    return func_02010c84(cb, t);
}
#define H(b, i) (((u16 *)((b) + 0x800))[i])

int func_02010d3c(WMCb cb, void *a, void *buf, u32 size, u16 x, u16 y, u16 z)
{
    u16 ok = 1;
    WMStatus *st = func_0200fcf4()->st;
    int r = func_0200fd68(2, 9, 10);
    if (r != 0) {
        return r;
    }
    DC_InvalidateRange(&st->h188, 2);
    if (st->h188 == 0) {
        DC_InvalidateRange(&st->h182, 2);
        ok = st->h182;
        DC_InvalidateRange(&st->h86, 2);
    }
    if (buf == 0) {
        return 6;
    }
    if (ok == 0) {
        return 7;
    }
    DC_InvalidateRange(&st->w7c, 2);
    if ((u32)buf == st->w7c) {
        return 6;
    }
    if (size > 0x200) {
        return 6;
    }
    if (size == 0) {
        return 6;
    }
    DC_StoreRange(buf, size);
    r = func_0200fbd8(0xf, 7, buf, size, x, y, z, cb, a);
    if (r == 0) {
        r = 2;
    }
    return r;
}

int func_02010e6c(u8 *buf, u16 n, u16 mask, u16 m, int flag)
{
    int i;
    u32 cnt;
    u8 *cur;
    u32 bm = 1;
    u32 aid;
    int k;
    int r = func_0200fd68(2, 9, 10);
    if (r != 0) {
        return r;
    }
    if (buf == 0) {
        return 6;
    }
    if (n >= 0x10) {
        return 6;
    }
    if (mask == 0) {
        return 6;
    }
    aid = func_020101c4();
    if (aid == 0) {
        bm = func_020101f4();
    }
    MIi_CpuClearFast(0, buf, 0x820);
    H(buf, 4) = 0;
    H(buf, 5) = 0;
    H(buf, 6) = 0;
    H(buf, 8) = m;
    H(buf, 0xb) = n;
    H(buf, 7) = 0;
    H(buf, 0xc) = flag != 0 ? 1 : 0;
    {u16 t = mask | (1 << aid); H(buf, 7) = t; cnt = func_020122b0(t);}
    H(buf, 9) = cnt;
    H(buf, 10) = m * cnt;
    if (H(buf, 10) > 0x1fc) {
        H(buf, 7) = 0;
        return 6;
    }
    H(buf, 10) = H(buf, 10) + 4;
    H(buf, 0xe) = 1;
    if (aid == 0) {
        aid = bm | 1;
        for (i = 0; i < 4; i++) {
            ((WMDataSet *)buf)[i].aidBitmap = H(buf, 7) & aid;
        }
        func_02010224(n, func_02011528, (u32)buf);
        cur = buf;
        for (k = 0; k < (H(buf, 0xc) == 1 ? 2 : 1); k++) {
            H(buf, 4) = (H(buf, 4) + 1) & 3;
            r = func_02010d3c(func_02011450, buf, cur, H(buf, 10), H(buf, 7) & bm, H(buf, 0xb), 1);
            if (r == 7) {
                H(buf, k) = 0xffff;
                H(buf, 5) = (H(buf, 5) + 1) & 3;
            } else if (r != 0 && r != 2) {
                H(buf, 0xe) = 5;
                return 1;
            }
            cur += 0x200;
        }
    } else {
        H(buf, 5) = 3;
        func_02010224(n, func_02011654, (u32)buf);
    }
    return 0;
}

int func_020110c0(u8 *buf)
{
    if (buf == 0) {
        return 6;
    }
    if (H(buf, 7) == 0) {
        return 3;
    }
    func_02010224(H(buf, 0xb), 0, 0);
    H(buf, 7) = 0;
    H(buf, 0xe) = 0;
    return 0;
}

int func_02011108(u8 *buf, void *pa, void *pb)
{
    u32 bm;
    u32 aid;
    u32 got;
    u32 idx;
    u32 arm;
    u32 res;
    u32 off;
    int r = func_0200fd68(2, 9, 10);
    if (r != 0) {
        return r;
    }
    if (buf == 0) {
        return 6;
    }
    if (pa == 0) {
        return 6;
    }
    if (pb == 0) {
        return 6;
    }
    aid = func_020101c4();
    if (aid == 0) {
        bm = func_020101f4();
    }
    if (H(buf, 0xe) == 5) {
        return 1;
    }
    if (H(buf, 0xe) != 1 && H(buf, 0xe) != 4) {
        return 3;
    }
    res = 5;
    if (aid == 0) {
        got = 0;
        arm = 0;
        if (H(buf, 0xe) == 4) {
            H(buf, 0xe) = 1;
            idx = (H(buf, 4) + 3) & 3;
            r = func_02010d3c(func_02011450, buf, buf + (idx << 9), H(buf, 10), H(buf, 7) & bm, H(buf, 0xb), 1);
            if (r == 7) {
                H(buf, idx) = 0xffff;
                H(buf, 5) = (H(buf, 5) + 1) & 3;
            } else if (r != 0 && r != 2) {
                H(buf, 0xe) = 5;
                return 1;
            }
        }
        if (H(buf, 6) != H(buf, 5)) {
            off = H(buf, 6) << 9;
            *(u16 *)(buf + off) |= 1;
            MIi_CpuCopy16(buf + (H(buf, 6) << 9), pb, 0x200);
            got = 1;
            res = 0;
            H(buf, 0xd) = H(buf, H(buf, 6));
            H(buf, 6) = (H(buf, 6) + 1) & 3;
            if (H(buf, 0xc) == 0 && bm != 0 && *(u16 *)(buf + (H(buf, 4) << 9)) == 1) {
                arm = got;
            } else {
                arm = 0;
            }
        }
        func_02011814(buf, 0);
        if (got != 0) {
            func_02011748(buf, 0, pa);
            if (H(buf, 0xc) == 0) {
                func_02011814(buf, arm);
            }
        }
    } else {
        got = 0;
        if (H(buf, 0xe) == 4) {
            got = 1;
            H(buf, 0xe) = 1;
        } else if (H(buf, 6) != H(buf, 4)) {
            off = H(buf, 6) << 9;
            if ((*(u16 *)(buf + off) & 1) == 0) {
                *(u16 *)(buf + off) |= 1;
            } else {
                MIi_CpuCopy16(buf + off, pb, 0x200);
                H(buf, 0xd) = H(buf, H(buf, 6));
                got = 1;
                res = 0;
                H(buf, 6) = (H(buf, 6) + 1) & 3;
            }
        }
        if (got != 0) {
            u8 *p = buf + (H(buf, 5) << 9);
            MIi_CpuCopy16(pa, p + 0x20, H(buf, 8));
            r = func_02010d3c(func_02011450, buf, p + 0x20, H(buf, 8), H(buf, 7), H(buf, 0xb), 1);
            H(buf, 5) = (H(buf, 5) + 1) & 3;
            if (r != 2 && r != 0) {
                H(buf, 0xe) = 5;
                res = 1;
            }
        }
    }
    return res;
}

void func_02011450(u16 *m)
{
    WMWork *w = func_0200fcf4();
    WMCb cb = w->indCb[m[5]];
    u8 *buf = (u8 *)w->indArg[m[5]];
    u16 aid;
    if (cb != func_02011528 && cb != func_02011654) {
        return;
    }
    if (buf == 0) {
        return;
    }
    if ((u32)buf != *(u32 *)(m + 0x10)) {
        return;
    }
    aid = func_020101c4();
    if (m[1] == 0) {
        if (aid != 0) {
            return;
        }
        H(buf, H(buf, 5)) = m[0xd] >> 1;
        H(buf, 5) = (H(buf, 5) + 1) & 3;
        return;
    }
    if (m[1] == 10) {
        if (aid != 0) {
            H(buf, 5) = (H(buf, 5) + 3) & 3;
        }
        H(buf, 0xe) = 4;
        return;
    }
    H(buf, 0xe) = 5;
}

void func_02011528(WMInd *m)
{
    u8 *buf = (u8 *)m->w1c;
    u32 bit;
    u32 e;
    u16 j;
    u16 idx;
    u32 mask;
    if (buf == 0) {
        return;
    }
    if (m->h2 == 0) {
    switch (m->h4) {
    case 0x15:
        func_02011748(buf, m->h12, (void *)m->wc);
        func_02011814(buf, 0);
        return;
    case 7:
        func_02011814(buf, 0);
        return;
    case 9:
    case 0x1a:
        bit = 1 << m->h12;
        e = OS_DisableInterrupts();
        idx = H(buf, 4);
        mask = ~bit;
        ((WMDataSet *)buf)[idx].aidBitmap &= mask;
        if (H(buf, 0xc) == 1) {
            j = (idx + 1) & 3;
            ((WMDataSet *)buf)[j].aidBitmap &= mask;
        }
        OS_RestoreInterrupts(e);
        func_02011814(buf, 0);
        if (H(buf, 0xc) == 1) {
            func_02011814(buf, 0);
        }
        return;
    case 0x19:
        return;
    default:
        return;
    }
    } else {
        H(buf, 0xe) = 5;
    }
}

void func_02011654(WMInd *m)
{
    u8 *buf = (u8 *)m->w1c;
    u32 len;
    u16 bits;
    u16 *data;
    u16 aid;
    if (buf == 0) {
        return;
    }
    if (m->h2 == 0) {
    switch (m->h4) {
    case 0x15:
        data = (u16 *)m->wc;
        len = m->h10;
        bits = data[0];
        aid = func_020101c4();
        if (len != H(buf, 10)) {
            if (len > 0x200) {
                len = 0x200;
            }
        }
        if (len < 4) {
            return;
        }
        if (!(bits & (1 << aid))) {
            return;
        }
        MIi_CpuCopy16(data, buf + (H(buf, 4) << 9), len);
        H(buf, H(buf, 4)) = m->h1a >> 1;
        H(buf, 4) = (H(buf, 4) + 1) & 3;
        return;
    case 7:
    case 9:
    case 0x19:
    case 0x1a:
        return;
    default:
        return;
    }
    } else {
        H(buf, 0xe) = 5;
    }
}

void func_02011748(u8 *buf, u32 idx, void *src)
{
    u16 bit = 1 << idx;
    u32 cur;
    u8 *p;
    u32 e;
    u32 off;
    if (!(H(buf, 7) & bit)) {
        return;
    }
    cur = H(buf, 4);
    if (!(((u16 *)buf)[cur << 8] & bit)) {
        if (H(buf, 0xc) != 1) {
            return;
        }
        cur = (cur + 1) & 3;
        if (!(((u16 *)buf)[cur << 8] & bit)) {
            return;
        }
    }
    p = func_020119bc(buf, H(buf, 7), buf + cur * 0x200 + 4, idx);
    if (src != 0) {
        MIi_CpuCopy16(src, p, H(buf, 8));
    } else {
        MIi_CpuClear16(0, p, H(buf, 8));
    }
    e = OS_DisableInterrupts();
    off = cur << 9;
    *(u16 *)(buf + off) &= ~bit;
    *(u16 *)((buf + 2) + off) |= bit;
    OS_RestoreInterrupts(e);
}

void func_02011814(u8 *buf, int flag)
{
    u32 e = OS_DisableInterrupts();
    u32 nxt;
    u32 cur;
    u32 bm;
    u32 clr;
    int r;
    if (*(u16 *)(buf + (H(buf, 4) << 9)) == 0) {
        bm = func_020101f4();
        cur = H(buf, 4);
        nxt = (cur + 1) & 3;
        if (H(buf, 0xc) == 1) {
            clr = (nxt + 1) & 3;
        } else {
            clr = nxt;
        }
        MIi_CpuClear16(0, buf + (clr << 9), 0x200);
        *(u16 *)(buf + (clr * 0x200)) = H(buf, 7) & (bm | 1);
        H(buf, 4) = nxt;
        *(u16 *)(buf + (cur << 9)) = H(buf, 7);
        if (flag == 1) {
            *(u16 *)(buf + (cur << 9)) &= ~1;
        }
        OS_RestoreInterrupts(e);
        r = func_02010d3c(func_02011450, buf, buf + (cur * 0x200), H(buf, 10), H(buf, 7) & bm, H(buf, 0xb), 1);
        if (r == 7) {
            H(buf, cur) = 0xffff;
            H(buf, 5) = (H(buf, 5) + 1) & 3;
            return;
        }
        if (r != 0 && r != 2) {
            H(buf, 0xe) = 5;
        }
    } else {
        OS_RestoreInterrupts(e);
    }
}

u8 *func_02011968(u8 *buf, u16 *p, u32 n)
{
    u16 a = p[0];
    u16 b = p[1];
    u32 bit = 1 << n;
    if (buf == 0) {
        return 0;
    }
    if (p == 0) {
        return 0;
    }
    if (!(a & bit)) {
        return 0;
    }
    if (!(b & bit)) {
        return 0;
    }
    return func_020119bc(buf, a, (u8 *)(p + 2), n);
}

u8 *func_020119bc(u8 *buf, u32 mask, u8 *base, u32 n)
{
    u32 cnt = func_020122b0(mask & ((1 << n) - 1));
    return base + H(buf, 8) * cnt;
}

int func_020119ec(WMCb cb, u16 a, u16 b, u16 c, u16 d)
{
    u16 m[5];
    int r;
    func_0200fcf4();
    r = func_0200fd68(1, 2);
    if (r != 0) {
        return r;
    }
    func_0200fb68(0x1e, cb);
    m[0] = 0x1e;
    m[1] = a;
    m[2] = b;
    m[3] = c;
    m[4] = d;
    r = func_0200fc84(m, 10);
    if (r == 0) {
        r = 2;
    }
    return r;
}

int func_02011a64(WMCb cb, int x)
{
    int r = func_0200fd68(2, 7, 9);
    if (r != 0) {
        return r;
    }
    func_0200fb68(0x21, cb);
    r = func_0200fbd8(0x21, 1, x);
    if (r == 0) {
        r = 2;
    }
    return r;
}

extern struct { u16 h0, h2, h4, h6; } data_020c1180;

void func_02011ab0(void)
{
    u32 z = 0;
    CpuSet(&z, data_020c1184, 0x5000001);
    data_020c1180.h6 = OS_GetLockID();
}

extern CtrdgInfo data_027ffc30; // the cartridge info block the boot code leaves in main RAM (lcf_edits.txt)
int func_02011ae8(void)
{
    if (func_02011b28() != 0) {
        if (func_02011b10() == 0) {
            return 1;
        }
    }
    return 0;
}

int func_02011b10(void)
{
    return data_027ffc30.b.f0;
}

int func_02011b28(void)
{
    CtrdgInfo *info = (CtrdgInfo *)0x27ffc30;
    u8 b[8];
    u8 a[8];
    u8 *rom = (u8 *)0x8000000;
    int ok = 1;
    u8 tag;
    if (info->h0 == 0xffff) {
        return 0;
    }
    if (info->b.f1 == 1) {
        return 0;
    }
    func_02011cb0(data_020c1180.h6, b);
    func_02011c34(a);
    tag = rom[0xb2];
    if (tag == 0x96 && info->h0 != *(u16 *)(rom + 0xbe)) {
        goto bad;
    }
    if (tag != 0x96 && info->h0 != *(u16 *)0x801fffe) {
        goto bad;
    }
    if (info->w8 != *(u32 *)(rom + 0xac) && info->b.f0) {
        goto bad;
    }
    goto done;
bad:
    ok = 0;
    info->b.f1 = 1;
done:
    func_02011c7c(a);
    func_02011d0c(data_020c1180.h6, b);
    return ok;
}
