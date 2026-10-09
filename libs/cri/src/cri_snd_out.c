/* CRI audio: PCM output stream (ring buffer fed to a sound timer, with rate conversion) */
typedef unsigned char u8;
typedef unsigned short u16;
typedef signed short s16;
typedef unsigned int u32;

typedef struct SndOut SndOut;
struct SndOut {
    u16 flags;      /* 0x00: 2 = updating, 4 = measure level, 8 = running */
    s16 id;         /* 0x02 */
    u8 *buf;        /* 0x04 */
    int i08;        /* 0x08 */
    u32 played;     /* 0x0c */
    u32 mark;       /* 0x10 */
    u32 i14;
    u16 size;       /* 0x18 */
    u16 wpos;       /* 0x1a */
    u16 filled;     /* 0x1c */
    u16 space;      /* 0x1e */
    u16 u20;
    s16 pan;        /* 0x22 */
    u16 level;      /* 0x24 */
    s16 resample;   /* 0x26 */
    u16 mode;       /* 0x28: 1..6 = upsampling ratio, 8 = copy */
    s16 prev;       /* 0x2a */
    s16 s2c;
    void (*cb)(int, u8 *, int);  /* 0x30 */
    u8 timer[1];    /* 0x34 */
};

typedef volatile u16 vu16;
#define REG_IME (*(vu16 *)0x4000208)

#define AVG(a, b) ((s16)(((a) + (b)) / 2))
#define PACK(lo, hi) (((u32)(u16)(lo)) | ((u32)(u16)(hi) << 16))

extern void MIi_CpuClearFast(u32, void *, u32);
extern int func_02037364(void *, int, void *, int, int, int, void *, void *);
extern void func_02037634(void *, int, int);
extern void func_02037574(void *, int);
extern void func_020375e0(void *, int, int);
extern void func_02037504(void *);
extern void func_02037558(void *);
extern void func_01ff96c4(void *, void *, int);
extern void MI_CpuFill8(void *, int, int);
extern void (*data_02218884)(SndOut *, void *);
extern char data_020ae2bc[];
extern int func_0201f274(int);
extern int func_0201edc4(int, int);
extern int func_0201e5c0(int, int);
extern int func_0201f200(int);

extern void func_02049f48(SndOut *);
extern void func_0204a018(SndOut *);
extern void func_02049cec(int, int, int, u32, int, SndOut *);
extern void func_02049fac(SndOut *, u16 *, u16 *);
extern void func_02049fe0(SndOut *, int);
extern u16 func_0204a2c8(SndOut *, u16 *, u16 *);
extern s16 *func_0204a320(SndOut *, s16 *, u32);
extern int func_02049c34(SndOut *);

int func_02049c34(SndOut *o)
{
    int r;
    if (o->flags & 8) return 0;
    MIi_CpuClearFast(0, o->buf, o->size);
    func_0204a018(o);
    r = func_02037364(o->timer, 1, o->buf, o->size, o->i08, o->s2c, func_02049cec, o);
    func_02037634(o->timer, 0, o->u20);
    func_02037574(o->timer, o->pan);
    func_020375e0(o->timer, 0, 0);
    o->prev = 0;
    o->flags |= 8;
    return r;
}

void func_02049cec(int type, int b, int c, u32 n, int d, SndOut *o)
{
    int hit = 0;
    if (type == 1) {
        o->played += n;
        if (o->filled != 0) {
            if (n > o->filled) n = o->filled;
            o->filled -= n;
            o->i14 += n >> 1;
        }
        if (!(o->flags & 2)) {
            u32 t = o->mark;
            u32 q = o->played;
            if (q > t) {
                if (q - t < 0x80000000) hit = 1;
            } else {
                if (t - q >= 0x80000000) hit = 1;
            }
            if (hit) {
                if (data_02218884 != 0) data_02218884(o, data_020ae2bc);
                o->wpos = o->played % 0x1800;
                o->mark = o->played;
            }
        }
    } else {
        o->played += n;
        o->wpos = o->played % 0x1800;
        o->mark = o->played;
    }
    o->space = 0x1800 - o->filled;
}

int func_02049e08(SndOut *o, int rate)
{
    if (o == 0) return 0;
    if (o->resample != 0) {
        if (rate == 0xffb) {
            o->mode = 1;
            rate = 0xffb << 3;
        } else if (rate == 0xffb * 2) {
            o->mode = 2;
            rate = 0xffb << 3;
        } else if (rate == 0x2ff1) {
            o->mode = 3;
            rate = 0xffb << 3;
        } else if (rate == 0xffb * 4) {
            o->mode = 4;
            rate = 0xffb << 3;
        } else if (rate == 0x4fe7) {
            o->mode = 5;
            rate = 0xffb << 3;
        } else if (rate == 0x2ff1 * 2) {
            o->mode = 6;
            rate = 0xffb << 3;
        } else {
            o->mode = 8;
        }
    } else {
        o->mode = 8;
    }
    o->i08 = (int)(0.5f + 523655.0f / (float)rate);
    if (o->flags & 8) {
        func_02049f48(o);
        func_02049c34(o);
    }
    return rate;
}

void func_02049f30(SndOut *o)
{
    func_02049c34(o);
    func_02037504(o->timer);
}

void func_02049f48(SndOut *o)
{
    u16 a, b;
    if (!(o->flags & 8)) return;
    func_02037558(o->timer);
    o->flags &= ~8;
    func_02049fac(o, &a, &b);
    if (b != 0) o->i14 += (u32)b >> 1;
    o->level = 0;
}

void func_02049fac(SndOut *o, u16 *sp, u16 *fp)
{
    u16 old = REG_IME;
    REG_IME = 0;
    *sp = o->space;
    *fp = o->filled;
    REG_IME;
    REG_IME = old;
}

void func_02049fe0(SndOut *o, int n)
{
    u16 old = REG_IME;
    REG_IME = 0;
    o->space -= n;
    o->filled += n;
    REG_IME;
    REG_IME = old;
}

void func_0204a018(SndOut *o)
{
    u16 old = REG_IME;
    REG_IME = 0;
    o->wpos = 0;
    o->space = 0x1800;
    o->filled = 0;
    o->played = 0;
    o->mark = 0;
    o->flags &= ~2;
    REG_IME;
    REG_IME = old;
}

// non-matching: same code, but 'first' and 'mode' swap registers (r7/r6 vs r6/r7)
u16 func_0204a05c(SndOut *o, u32 n, u16 *out)
{
    u16 a, b;
    u16 first;
    u16 m;
    u16 cnt;
    u16 unit;
    u16 mode;
    if (n == 0) {
        *out = 0;
        return 0;
    }
    func_0204a2c8(o, &a, &b);
    first = a;
    if (first == 0) {
        *out = 0;
        return 0;
    }
    m = n;
    if (m > 0xc00) m = 0xc00;
    if (m > first) m = first;
    mode = o->mode;
    if (mode != 8) {
        if (mode == 5) {
            if (m < 10) {
                *out = 0;
                return m;
            }
            m = (m / 10) * 10;
            unit = 0;
            cnt = (m / mode) << 3;
        } else if (mode == 3 || mode == 6) {
            if (m < 6) {
                *out = 0;
                return m;
            }
            m = (m / 6) * 6;
            unit = 0;
            cnt = (m / mode) << 3;
        } else {
            unit = 8 / mode;
            cnt = m * unit;
        }
    } else {
        cnt = m;
        unit = 1;
    }
    if (cnt > first) {
        if (mode == 3 || mode == 5 || mode == 6) {
            m = (mode * first) / 8;
            *out = (m / mode) << 3;
        } else {
            m = m - (cnt - first) / unit;
            *out = m * unit;
        }
    } else {
        *out = cnt;
    }
    return m;
}

u32 func_0204a244(SndOut *o, s16 *src, u32 n)
{
    func_02049fe0(o, n);
    src = func_0204a320(o, src, n);
    if ((o->flags & 4) && n >= 2) {
        u32 sum = 0;
        u32 i;
        for (i = 0; i < n; i++) {
            int v = src[i];
            if (v < 0) v = -v;
            sum += v;
        }
        o->level = sum / (n >> 1);
    }
    return n;
}

u16 func_0204a2c8(SndOut *o, u16 *first, u16 *second)
{
    u16 a, b;
    int end;
    func_02049fac(o, &a, &b);
    end = o->wpos + a;
    if (end > 0x1800) {
        *first = 0x1800 - o->wpos;
        *second = end - 0x1800;
    } else {
        *first = a;
        *second = 0;
    }
    return a;
}

// non-matching: 0x67c vs 0x694 bytes; modes 8, 5, 6, 3 and the prologue are the right size, modes 4/2/1 need
// the 16-bit halves of the source word materialised as separate s16 locals (extra asr/lsl pairs), and the
// registers of n/o/src and the loop counters differ
s16 *func_0204a320(SndOut *o, s16 *src, u32 n)
{
    u16 wp = o->wpos;
    u8 *dst = o->buf + wp;
    u16 e = wp + n;
    u16 mode;
    if (e >= 0x1800) e -= 0x1800;
    o->flags |= 2;
    o->mark += n;
    o->wpos = e;
    o->flags &= ~2;
    mode = o->mode;
    if (mode != 8) {
    if (mode == 5) {
        int cnt = (int)(n >> 1) / 8;
        int i;
        for (i = 0; i < cnt; i++) {
            s16 s0 = src[0], s1 = src[1], s2 = src[2], s3 = src[3], s4 = src[4];
            s16 a0 = AVG(o->prev, s0);
            s16 a1 = AVG(s1, s2);
            s16 a2 = AVG(s3, s4);
            ((u32 *)dst)[0] = PACK(a0, s0);
            ((u32 *)dst)[1] = PACK(s1, a1);
            ((u32 *)dst)[2] = PACK(s2, s3);
            ((u32 *)dst)[3] = PACK(a2, s4);
            o->prev = s4;
            src += 5;
            dst += 16;
        }
    } else if (mode == 6) {
        int cnt = (int)(n >> 1) / 4;
        int i;
        for (i = 0; i < cnt; i++) {
            s16 s0 = src[0], s1 = src[1], s2 = src[2];
            ((s16 *)dst)[0] = s0;
            ((s16 *)dst)[1] = (s0 + s1 * 2) / 3;
            ((s16 *)dst)[2] = (s2 + s1 * 2) / 3;
            ((s16 *)dst)[3] = s2;
            o->prev = s2;
            src += 3;
            dst += 8;
        }
    } else if (mode == 3) {
        int cnt = (int)(n >> 1) / 8;
        int i;
        for (i = 0; i < cnt; i++) {
            s16 s0 = src[0], s1 = src[1], s2 = src[2];
            s16 q1 = (s0 + s1 * 2) / 3;
            s16 q2 = (s2 + s1 * 2) / 3;
            ((s16 *)dst)[0] = (o->prev + s0) / 2;
            ((s16 *)dst)[1] = s0;
            ((s16 *)dst)[2] = (s0 + q1) / 2;
            ((s16 *)dst)[3] = q1;
            ((s16 *)dst)[4] = (q1 + q2) / 2;
            ((s16 *)dst)[5] = q2;
            ((s16 *)dst)[6] = (s2 + q2) / 2;
            ((s16 *)dst)[7] = s2;
            o->prev = s2;
            src += 3;
            dst += 16;
        }
    } else if (mode == 4) {
        int cnt = n >> 2;
        s16 prev = o->prev;
        while (cnt > 0) {
            int w = *(int *)src;
            s16 s0 = w;
            s16 a0 = AVG(prev, s0);
            s16 a1;
            prev = w >> 16;
            a1 = AVG(s0, prev);
            src += 2;
            cnt -= 2;
            ((u32 *)dst)[0] = PACK(a0, s0);
            ((u32 *)dst)[1] = PACK(a1, prev);
            dst += 8;
        }
        o->prev = prev;
    } else if (mode == 2) {
        int cnt = n >> 3;
        s16 prev = o->prev;
        while (cnt > 0) {
            int w = *(int *)src;
            s16 s0 = w, s1 = w >> 16;
            s16 a = AVG(prev, s0);
            s16 b = AVG(prev, a);
            s16 c = AVG(s0, s1);
            src += 2;
            cnt -= 2;
            ((u32 *)dst)[0] = PACK(b, a);
            ((u32 *)dst)[1] = PACK(AVG(a, s0), s0);
            ((u32 *)dst)[2] = PACK(AVG(s0, c), c);
            ((u32 *)dst)[3] = PACK(AVG(c, s1), s1);
            prev = s1;
            dst += 16;
        }
        o->prev = prev;
    } else if (mode == 1) {
        int cnt = (int)(n >> 1) / 8;
        s16 prev = o->prev;
        int i;
        for (i = 0; i < cnt; i++) {
            int w = *(int *)src;
            s16 a = AVG(prev, (s16)w);
            s16 b = AVG(prev, a);
            s16 d, c, e, f;
            ((u32 *)dst)[0] = PACK(AVG(prev, b), b);
            ((u32 *)dst)[1] = PACK(AVG(b, a), a);
            d = AVG(a, (s16)w);
            ((u32 *)dst)[2] = PACK(AVG(a, d), d);
            ((u32 *)dst)[3] = PACK(AVG(d, (s16)w), (s16)w);
            c = AVG((s16)w, w >> 16);
            e = AVG((s16)w, c);
            ((u32 *)dst)[4] = PACK(AVG((s16)w, e), e);
            ((u32 *)dst)[5] = PACK(AVG(e, c), c);
            f = AVG(c, w >> 16);
            ((u32 *)dst)[6] = PACK(AVG(c, f), f);
            ((u32 *)dst)[7] = PACK(AVG(f, w >> 16), w >> 16);
            src += 2;
            prev = w >> 16;
            dst += 32;
        }
        o->prev = prev;
    }
    } else {
        func_01ff96c4(src, dst, n);
        src += n / 2;
    }
    if (o->cb != 0) o->cb(o->id, o->buf + wp, n);
    return src;
}

u32 func_0204a9b4(SndOut *o)
{
    u16 a, b;
    u16 n, tail, head;
    int wp;
    if (o == 0) return 0;
    func_02049fac(o, &a, &b);
    n = a;
    if (n == 0) return 0;
    wp = o->wpos;
    if ((int)(wp + n) <= 0x1800) {
        head = n;
        tail = 0;
    } else {
        head = 0x1800 - wp;
        tail = wp + n - 0x1800;
    }
    MI_CpuFill8(o->buf + wp, 0, head);
    if (tail != 0) MI_CpuFill8(o->buf, 0, tail);
    o->prev = 0;
    return n;
}

u32 func_0204aa58(SndOut *o)
{
    if (o == 0) return 0;
    return (u32)o->filled >> 1;
}

u32 func_0204aa6c(SndOut *o)
{
    if (o == 0) return 0;
    return (u32)o->space >> 1;
}

int func_0204aa80(SndOut *o, int pan)
{
    o->pan = pan;
    func_02037574(o->timer, pan);
    func_020375e0(o->timer, 0, 0);
    return pan;
}

void func_0204aab0(SndOut *o, int v)
{
    int r;
    if (v == 0) r = 0x40;
    else if (v == -15) r = 0;
    else if (v == 15) r = 0x7f;
    else r = ((v + 15) << 7) / 30;
    o->u20 = r;
    func_02037634(o->timer, 0, r);
}
