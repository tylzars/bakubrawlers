// NitroSystem G3d node animation evaluation in ITCM (NNS_G3dGetAnm* family), 0x01ff9b64-0x01ffae0f.
// Compiler: 2.0/sp2p2 reproduces the single-mla "x * 3 + y" 64-bit idiom of func_01ffa1cc; sp1p5 emits the
// double-zero mla form, so this code was built with sp2p2 or later.
#include <nnsys/private/g3d_rs.h>

typedef struct { s16 x, y; } P16;
typedef struct { s32 x, y; } P32;

extern const u8 data_0208b4bc[];   // 4-byte stride tables: output slot for each of a, b, +-b, +-a
extern const u8 data_0208b4bd[];
extern const u8 data_0208b4be[];
extern const u8 data_0208b4bf[];
extern void func_01ff8fc4(const s32 *src, s32 *dst);   // VEC_Normalize

extern void func_01ff9f18(s32 *out, s32 frame, u32 *info, u8 *res);   // one value, no interpolation (defined in another unit)
extern void func_01ff98bc(s32 *out);
extern void func_01ff9954(s32 *out);
extern void func_01ff99e4(s32 *out);
extern void func_01ffa070(s32 *out, s32 frame, u32 *info, u8 *res);
extern void func_01ffa1cc(s32 *out, s32 frame, u32 *info, u8 *res);
extern void func_01ffa3b4(s32 *out, s32 frame, u32 *info, u8 *res);
extern void func_01ffa548(s32 *out, s32 frame, u32 *info, u8 *res);
extern void func_01ffa960(s32 *out, s32 frame, u32 *info, u8 *res);
int func_01ffaca4(s32 *m, u8 *t1, u8 *t2, u32 idx);
typedef void (*AnmCb)(s32 *out, s32 *scale, u8 *cmd, u32 flag);

void func_01ff9b64(u8 *res, int idx, int frame, s32 *out) {
    s32 scale[6];
    s32 sA[2];
    s32 sB[2];
    s32 sC[2];
    u32 hdr;
    u32 *p;
    int interp;
    u32 ofs = *(u16 *) (res + 0x14 + idx * 2);
    hdr = *(u32 *) (res + ofs);
    p = (u32 *) (res + ofs + 4);
    if (hdr & 1) {
        out[0] = 7;
        goto emit;
    }
    if ((frame & 0xfff) && (*(u32 *) (res + 8) & 1)) {
        interp = 1;
    } else {
        interp = 0;
    }
    out[0] = 0;
    if (!(hdr & 6)) {
        if (!(hdr & 8)) {
            if (interp) {
                func_01ffa070(out + 0x4c / 4, frame, p, res);
            } else {
                func_01ff9f18(out + 0x4c / 4, frame, p, res);
            }
            p += 2;
        } else {
            out[0x4c / 4] = *p++;
        }
        if (!(hdr & 0x10)) {
            if (interp) {
                func_01ffa070(out + 0x50 / 4, frame, p, res);
            } else {
                func_01ff9f18(out + 0x50 / 4, frame, p, res);
            }
            p += 2;
        } else {
            out[0x50 / 4] = *p++;
        }
        if (!(hdr & 0x20)) {
            if (interp) {
                func_01ffa070(out + 0x54 / 4, frame, p, res);
            } else {
                func_01ff9f18(out + 0x54 / 4, frame, p, res);
            }
            p += 2;
        } else {
            out[0x54 / 4] = *p++;
        }
    } else if (hdr & 2) {
        out[0] |= 4;
    } else {
        func_01ff98bc(out);
    }
    if (!(hdr & 0xc0)) {
        if (!(hdr & 0x100)) {
            if (interp) {
                func_01ffa960(out + 0x28 / 4, frame, p, res);
            } else {
                func_01ffa548(out + 0x28 / 4, frame, p, res);
            }
            p += 2;
        } else {
            if (func_01ffaca4(out + 0x28 / 4, res + *(u32 *) (res + 0xc), res + *(u32 *) (res + 0x10), p[0]) != 0) {
                s32 m5 = out[0x3c / 4];
                s32 m1 = out[0x2c / 4];
                s32 m0 = out[0x28 / 4];
                s32 m4 = out[0x38 / 4];
                s32 m2 = out[0x30 / 4];
                s32 m3 = out[0x34 / 4];
                out[0x40 / 4] = (m1 * m5 - m2 * m4) >> 12;
                out[0x44 / 4] = (m2 * m3 - m0 * m5) >> 12;
                out[0x48 / 4] = (m0 * m4 - m1 * m3) >> 12;
            }
            p += 1;
        }
    } else if (hdr & 0x40) {
        out[0] |= 2;
    } else {
        func_01ff99e4(out);
    }
    if (!(hdr & 0x600)) {
        if (!(hdr & 0x800)) {
            if (interp) {
                func_01ffa3b4(sA, frame, p, res);
            } else {
                func_01ffa1cc(sA, frame, p, res);
            }
            scale[0] = sA[0];
            scale[3] = sA[1];
        } else {
            scale[0] = p[0];
            scale[3] = p[1];
        }
        if (!(hdr & 0x1000)) {
            if (interp) {
                func_01ffa3b4(sB, frame, p + 2, res);
            } else {
                func_01ffa1cc(sB, frame, p + 2, res);
            }
            scale[1] = sB[0];
            scale[4] = sB[1];
        } else {
            scale[1] = p[2];
            scale[4] = p[3];
        }
        if (!(hdr & 0x2000)) {
            if (interp) {
                func_01ffa3b4(sC, frame, p + 4, res);
            } else {
                func_01ffa1cc(sC, frame, p + 4, res);
            }
            scale[2] = sC[0];
            scale[5] = sC[1];
        } else {
            scale[2] = p[4];
            scale[5] = p[5];
        }
    } else if (hdr & 0x200) {
        out[0] |= 1;
    } else {
        func_01ff9954(out);
        return;
    }
emit:
    {
        Rs *g = data_020baf00;
        u32 fl = (out[0] & 1) ? 4 : 0;
        ((AnmCb) g->cbE8)(out, scale, g->cmd, fl);
    }
}

void func_01ffa070(s32 *out, s32 ratio, u32 *info, u8 *res) {
    u8 *data;
    u32 f = ratio >> 12;
    u32 hdr;
    s32 sc, sh;
    u32 last;
    s32 w, a, b;
    data = res + info[1];
    hdr = info[0];
    if (*(u16 *) (res + 4) - 1 == (ratio >> 12)) {
        if (hdr & 0xc0000000) {
            if (hdr & 0x40000000) {
                f = (f & 1) + (f >> 1);
            } else {
                f = (f & 3) + (f >> 2);
            }
        }
        if (*(u32 *) (res + 8) & 2) {
            w = ratio & 0xfff;
            if (hdr & 0x20000000) {
                a = ((s16 *) data)[f];
                b = ((s16 *) data)[0];
            } else {
                a = ((s32 *) data)[f];
                b = ((s32 *) data)[0];
            }
            *out = a + ((w * (b - a)) >> 12);
        } else {
            *out = (hdr & 0x20000000) ? ((s16 *) data)[f] : ((s32 *) data)[f];
        }
        return;
    }
    if (hdr & 0xc0000000) {
        last = (hdr & 0x1fff0000) >> 16;
        if (hdr & 0x40000000) {
            if (f >= last) {
                f = last >> 1;
                goto one;
            }
            f = f >> 1;
            ratio &= 0x1fff;
            sc = 2;
            sh = 1;
            goto blend;
        } else {
            if (f >= last) {
                f = (f & 3) + (f >> 2);
                goto one;
            }
            f = f >> 2;
            ratio &= 0x3fff;
            sc = 4;
            sh = 2;
            goto blend;
        }
    }
one:
    ratio &= 0xfff;
    sc = 1;
    sh = 0;
blend:
    if (hdr & 0x20000000) {
        s16 *p = (s16 *) data + f;
        a = ((s16 *) data)[f];
        b = p[1];
    } else {
        s32 *p = (s32 *) data + f;
        a = p[0];
        b = p[1];
    }
    *out = (a * sc + ((ratio * (b - a)) >> 12)) >> sh;
}

void func_01ffa1cc(s32 *out, s32 frame, u32 *info, u8 *res) {
    u32 f = frame >> 12;
    u32 hdr = info[0];
    u8 *data = res + info[1];
    u32 last;
    u32 a, b, g;
    if (hdr & 0xc0000000) {
        last = (hdr & 0x1fff0000) >> 16;
        if (hdr & 0x40000000) {
            if (f & 1) {
                if (f > last) { f = (last >> 1) + 1; goto fetch; }
                g = f >> 1;
                goto avg;
            }
            f = f >> 1;
            goto fetch;
        } else {
            u32 r = f & 3;
            if (r == 0) { f = f >> 2; goto fetch; }
            if (f > last) { f = r + (last >> 2); goto fetch; }
            if (f & 1) {
                if (f & 2) { f >>= 2; a = f + 1; b = f; }
                else { a = f >> 2; b = a + 1; }
                if (hdr & 0x20000000) {
                    P16 *pa = (P16 *) data + a;
                    P16 *pb = (P16 *) data + b;
                    out[0] = (pa->x * 3 + pb->x) >> 2;
                    out[1] = (pa->y * 3 + pb->y) >> 2;
                } else {
                    P32 *pa = (P32 *) data + a;
                    P32 *pb = (P32 *) data + b;
                    out[0] = (s32) (((s64) pa->x * 3 + pb->x) >> 2);
                    out[1] = (s32) (((s64) pa->y * 3 + pb->y) >> 2);
                }
                return;
            }
            g = f >> 2;
            goto avg;
        }
    }
fetch:
    if (hdr & 0x20000000) {
        P16 *p = (P16 *) data + f;
        out[0] = p->x;
        out[1] = p->y;
    } else {
        P32 *p = (P32 *) data + f;
        out[0] = p->x;
        out[1] = p->y;
    }
    return;
avg:
    if (hdr & 0x20000000) {
        P16 *p = (P16 *) data + g;
        out[0] = (p[0].x + p[1].x) >> 1;
        out[1] = (p[0].y + p[1].y) >> 1;
    } else {
        P32 *p = (P32 *) data + g;
        out[0] = (p[0].x + p[1].x) >> 1;
        out[1] = (p[0].y + p[1].y) >> 1;
    }
}

void func_01ffa3b4(s32 *out, s32 ratio, u32 *info, u8 *res) {
    u32 hdr;
    u32 g;
    u32 last;
    u8 *data;
    s32 a0, a1, b0, b1;
    u32 f = ratio >> 12;
    s32 sc;
    s32 sh;
    data = res + info[1];
    hdr = info[0];
    if (*(u16 *) (res + 4) - 1 == (ratio >> 12)) {
        if (hdr & 0xc0000000) {
            if (hdr & 0x40000000) {
                f = (f & 1) + (f >> 1);
            } else {
                f = (f & 3) + (f >> 2);
            }
        }
        if (*(u32 *) (res + 8) & 2) {
            g = 0;
            goto one;
        }
        if (hdr & 0x20000000) {
            out[0] = ((P16 *) data)[f].x;
            out[1] = ((P16 *) data)[f].y;
        } else {
            out[0] = ((P32 *) data)[f].x;
            out[1] = ((P32 *) data)[f].y;
        }
        return;
    }
    if (hdr & 0xc0000000) {
        last = (hdr & 0x1fff0000) >> 16;
        if (hdr & 0x40000000) {
            if (f >= last) {
                f = last >> 1;
                g = f + 1;
                goto one;
            }
            f = f >> 1;
            g = f + 1;
            ratio &= 0x1fff;
            sc = 2;
            sh = 1;
            goto blend;
        } else {
            if (f >= last) {
                f = (f & 3) + (f >> 2);
                g = f + 1;
                goto one;
            }
            f = f >> 2;
            g = f + 1;
            ratio &= 0x3fff;
            sc = 4;
            sh = 2;
            goto blend;
        }
    }
    g = f + 1;
one:
    ratio &= 0xfff;
    sc = 1;
    sh = 0;
blend:
    if (hdr & 0x20000000) {
        a0 = ((P16 *) data)[f].x;
        a1 = ((P16 *) data)[f].y;
        b0 = ((P16 *) data)[g].x;
        b1 = ((P16 *) data)[g].y;
    } else {
        a0 = ((P32 *) data)[f].x;
        a1 = ((P32 *) data)[f].y;
        b0 = ((P32 *) data)[g].x;
        b1 = ((P32 *) data)[g].y;
    }
    out[0] = (a0 * sc + ((ratio * (b0 - a0)) >> 12)) >> sh;
    out[1] = (a1 * sc + ((ratio * (b1 - a1)) >> 12)) >> sh;
}

#define CROSS(m)                                                         \
    {                                                                    \
        s32 m5 = (m)[5];                                                 \
        s32 m0 = (m)[0];                                                 \
        s32 m2 = (m)[2];                                                 \
        s32 m3 = (m)[3];                                                 \
        s32 m1 = (m)[1];                                                 \
        s32 m4 = (m)[4];                                                 \
        (m)[6] = (m1 * m5 - m2 * m4) >> 12;                              \
        (m)[7] = (m2 * m3 - m0 * m5) >> 12;                              \
        (m)[8] = (m0 * m4 - m1 * m3) >> 12;                              \
    }

void func_01ffa548(s32 *out, s32 ratio, u32 *info, u8 *res) {
    u32 hdr = info[0];
    s32 tA[9];
    s32 tB[9];
    u16 *data = (u16 *) (res + info[1]);
    u32 o1 = *(u32 *) (res + 0xc);
    u32 o2 = *(u32 *) (res + 0x10);
    u32 f = ratio >> 12;
    u32 g;
    u32 a, b;
    int fl;
    if (hdr & 0xc0000000) {
        u32 last = (hdr & 0x1fff0000) >> 16;
        if (hdr & 0x40000000) {
            if (f & 1) {
                if (f > last) {
                    f = (last >> 1) + 1;
                    goto fetch;
                }
                g = f >> 1;
                goto avg;
            }
            f = f >> 1;
            goto fetch;
        } else {
            u32 r = f & 3;
            if (r == 0) {
                f = f >> 2;
                goto fetch;
            }
            if (f > last) {
                f = r + (last >> 2);
                goto fetch;
            }
            if (f & 1) {
                if (f & 2) {
                    a = (f >> 2) + 1;
                    b = f >> 2;
                } else {
                    b = f >> 2;
                    a = b + 1;
                }
                fl = 0;
                fl |= func_01ffaca4(out, res + o1, res + o2, data[b]);
                fl |= func_01ffaca4(tA, res + o1, res + o2, data[a]);
                out[0] = tA[0] + out[0] * 3;
                out[1] = tA[1] + out[1] * 3;
                out[2] = tA[2] + out[2] * 3;
                out[3] = tA[3] + out[3] * 3;
                out[4] = tA[4] + out[4] * 3;
                out[5] = tA[5] + out[5] * 3;
                func_01ff8fc4(out, out);
                func_01ff8fc4(out + 3, out + 3);
                if (fl == 0) {
                    out[6] = tA[6] + out[6] * 3;
                    out[7] = tA[7] + out[7] * 3;
                    out[8] = tA[8] + out[8] * 3;
                    func_01ff8fc4(out + 6, out + 6);
                    return;
                }
                CROSS(out)
                return;
            }
            g = f >> 2;
            goto avg;
        }
    }
    goto fetch;
avg:
    fl = 0;
    fl |= func_01ffaca4(out, res + o1, res + o2, data[g]);
    fl |= func_01ffaca4(tB, res + o1, res + o2, data[g + 1]);
    out[0] += tB[0];
    out[1] += tB[1];
    out[2] += tB[2];
    out[3] += tB[3];
    out[4] += tB[4];
    out[5] += tB[5];
    func_01ff8fc4(out, out);
    func_01ff8fc4(out + 3, out + 3);
    if (fl == 0) {
        out[6] += tB[6];
        out[7] += tB[7];
        out[8] += tB[8];
        func_01ff8fc4(out + 6, out + 6);
        return;
    }
    CROSS(out)
    return;
fetch:
    if (func_01ffaca4(out, res + o1, res + o2, data[f]) != 0) {
        CROSS(out)
        return;
    }
    func_01ff8fc4(out + 6, out + 6);
}

void func_01ffa960(s32 *out, s32 ratio, u32 *info, u8 *res) {
    u16 *data = (u16 *) (res + info[1]);
    s32 f = ratio >> 12;
    u32 o1 = *(u32 *) (res + 0xc);
    u32 o2 = *(u32 *) (res + 0x10);
    u32 hdr = info[0];
    s32 tA[9];
    s32 tB[9];
    u32 b;
    s32 w0, w1;
    int fl;
    if (*(u16 *) (res + 4) - 1 == f) {
        if (hdr & 0xc0000000) {
            if (hdr & 0x40000000) {
                f = (f & 1) + (f >> 1);
            } else {
                f = (f & 3) + (f >> 2);
            }
        }
        if (*(u32 *) (res + 8) & 2) {
            b = 0;
            goto one;
        }
        if (func_01ffaca4(out, res + o1, res + o2, data[f]) != 0) {
            CROSS(out)
            return;
        }
        func_01ff8fc4(out + 6, out + 6);
        return;
    }
    if (hdr & 0xc0000000) {
        u32 last = (hdr & 0x1fff0000) >> 16;
        if (hdr & 0x40000000) {
            if (f >= last) {
                f = last >> 1;
                b = f + 1;
                goto one;
            }
            f = f >> 1;
            b = f + 1;
            w0 = ratio & 0x1fff;
            w1 = 2;
            goto blend;
        } else {
            if (f >= last) {
                f = (f & 3) + (f >> 2);
                b = f + 1;
                goto one;
            }
            f = f >> 2;
            b = f + 1;
            w0 = ratio & 0x3fff;
            w1 = 4;
            goto blend;
        }
    }
    b = f + 1;
one:
    w1 = 1;
    w0 = ratio & 0xfff;
blend:
    fl = func_01ffaca4(tA, res + o1, res + o2, data[f]);
    fl |= func_01ffaca4(tB, res + o1, res + o2, data[b]);
    out[0] = tA[0] * w1 + ((w0 * (tB[0] - tA[0])) >> 12);
    out[1] = tA[1] * w1 + ((w0 * (tB[1] - tA[1])) >> 12);
    out[2] = tA[2] * w1 + ((w0 * (tB[2] - tA[2])) >> 12);
    out[3] = tA[3] * w1 + ((w0 * (tB[3] - tA[3])) >> 12);
    out[4] = tA[4] * w1 + ((w0 * (tB[4] - tA[4])) >> 12);
    out[5] = tA[5] * w1 + ((w0 * (tB[5] - tA[5])) >> 12);
    func_01ff8fc4(out, out);
    func_01ff8fc4(out + 3, out + 3);
    if (fl == 0) {
        out[6] = tA[6] * w1 + ((w0 * (tB[6] - tA[6])) >> 12);
        out[7] = tA[7] * w1 + ((w0 * (tB[7] - tA[7])) >> 12);
        out[8] = tA[8] * w1 + ((w0 * (tB[8] - tA[8])) >> 12);
        func_01ff8fc4(out + 6, out + 6);
        return;
    }
    CROSS(out)
}

int func_01ffaca4(s32 *m, u8 *res1, u8 *res2, u32 idx) {
    s16 *t1 = (s16 *) res1;
    s16 *t2 = (s16 *) res2;
    if (idx & 0x8000) {
        s16 *p;
        s32 v0, v1, v2, k;
        s32 w;
        m[0] = m[1] = m[2] = m[3] = m[4] = m[5] = m[6] = m[7] = m[8] = 0;
        idx &= 0x7fff;
        p = t1 + idx * 3;
        v0 = p[0];
        v1 = p[1];
        v2 = p[2];
        k = v0 & 0xf;
        w = (v0 & 0x10) ? -0x1000 : 0x1000;
        m[k] = w;
        m[data_0208b4bc[k * 4]] = v1;
        m[data_0208b4bd[k * 4]] = v2;
        if (p[0] & 0x20) {
            v2 = -v2;
        }
        m[data_0208b4be[k * 4]] = v2;
        if (p[0] & 0x40) {
            v1 = -v1;
        }
        m[data_0208b4bf[k * 4]] = v1;
        return 0;
    } else {
        s16 *q = t2 + (idx & 0x7fff) * 5;
        s16 w;
        s32 v;
        v = q[4];
        m[4] = v >> 3;
        w = v & 7;
        v = q[0];
        m[0] = v >> 3;
        w = (v & 7) | (w << 3);
        v = q[1];
        m[1] = v >> 3;
        w = (v & 7) | (w << 3);
        v = q[2];
        m[2] = v >> 3;
        w = (v & 7) | (w << 3);
        v = q[3];
        m[3] = v >> 3;
        w = (v & 7) | (w << 3);
        m[5] = ((s32) w << 19) >> 19;
        return 1;
    }
}

