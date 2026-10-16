/* CRI MPEG audio: frame header, layer II allocation/scalefactors/requantisation, 0x0204b314-0x0204bdd4 */
#pragma dont_inline on
typedef struct Bs Bs;

void func_0204b314(char *o, char *s) {
    unsigned i;
    unsigned char *q = (unsigned char *)s;
    unsigned char *d = (unsigned char *)o + 0xa24;
    for (i = 4; i != 0; i--) { unsigned char x = q[0], y = q[1]; q += 2; d[0] = x; d[1] = y; d += 2; }
}

typedef struct FrameHdr {
    int id, layer, nocrc, bitrate, freq, pad, priv, mode, modeext, copy, orig, emph;
} FrameHdr;

typedef struct QEnt { short grp, bits, cls, pad; int mul, add; } QEnt;

typedef struct SbInfo {
    int i0;
    int nch;          /* 0x04 */
    int i8;
    int ic;
    int sblimit;      /* 0x10 */
    int bound;        /* 0x14 */
    QEnt (*alloc)[16]; /* 0x18 */
    int pad1c[6];
    short sel[4];     /* 0x34 */
} SbInfo;

extern int func_0204e6f0(Bs *, int);
extern int func_0204ab20(int, QEnt (**)[16]);
extern int data_020ae458[][4], data_020ae2d0[][4];

int func_0204b344(Bs *bs, FrameHdr *h) {
    h->id = func_0204e6f0(bs, 1);
    h->layer = 4 - func_0204e6f0(bs, 2);
    h->nocrc = func_0204e6f0(bs, 1) == 0;
    h->bitrate = func_0204e6f0(bs, 4);
    h->freq = func_0204e6f0(bs, 2);
    h->pad = func_0204e6f0(bs, 1);
    h->priv = func_0204e6f0(bs, 1);
    h->mode = func_0204e6f0(bs, 2);
    h->modeext = func_0204e6f0(bs, 2);
    h->copy = func_0204e6f0(bs, 1);
    h->orig = func_0204e6f0(bs, 1);
    h->emph = func_0204e6f0(bs, 2);
    if (h->mode != 3 || h->modeext != 0) return -1;
    return 0;
}

int func_0204b438(FrameHdr *h, int b, QEnt (**c)[16]) {
    return func_0204ab20(0, c);
}

void func_0204b44c(FrameHdr *h, SbInfo *si) {
    int r;
    if (si->i0 == 0) {
        if (h->mode == 3) { si->nch = 1; } else { si->nch = 2; }
    }
    if (h->layer != 2) r = 0x20;
    else r = func_0204b438(h, si->nch, &si->alloc);
    si->sblimit = r;
    if (h->mode == 1) r = data_020ae458[h->layer][h->modeext]; else r = si->sblimit;
    si->bound = r;
    if (si->i0 == 0) si->i8 = data_020ae2d0[h->id][h->freq];
}

void func_0204b4f8(Bs *bs, SbInfo *si, int (*out)[32]) {
    int i, ch;
    QEnt (*tab)[16] = si->alloc;
    int bound = si->bound, sblimit = si->sblimit, nch = si->nch;
    for (i = 0; i < bound; i++)
        for (ch = 0; ch < nch; ch++)
            out[ch][i] = func_0204e6f0(bs, tab[i][0].bits);
    for (i = bound; i < sblimit; i++) {
        int v = func_0204e6f0(bs, tab[i][0].bits);
        out[1][i] = v;
        out[0][i] = v;
    }
    for (i = sblimit; i < 32; i++)
        for (ch = 0; ch < nch; ch++)
            out[ch][i] = 0;
}

void func_0204b5f4(Bs *bs, SbInfo *si, int (*alloc)[32], int (*sfsel)[32], int (*scf)[3][32]) {
    int sblimit = si->sblimit;
    int nch = si->nch;
    int sb, ch;
    short m;
    if (alloc[0][0] != 0) {
        sfsel[0][0] = func_0204e6f0(bs, 2);
    } else {
        sfsel[0][0] = 0;
    }
    m = si->sel[sfsel[0][0]];
    for (sb = 1; sb < sblimit; sb++) {
        if (alloc[0][sb] != 0) {
            sfsel[0][sb] = func_0204e6f0(bs, 2);
            sfsel[0][sb] ^= m & 3;
        }
        m >>= 2;
    }
    for (sb = sblimit; sb < 32; sb++)
        for (ch = 0; ch < nch; ch++)
            sfsel[ch][sb] = 0;
    for (sb = 0; sb < sblimit; sb++) {
        for (ch = 0; ch < nch; ch++) {
            if (alloc[ch][sb] != 0) {
                switch (sfsel[ch][sb]) {
                case 0:
                    scf[ch][0][sb] = func_0204e6f0(bs, 6);
                    scf[ch][1][sb] = func_0204e6f0(bs, 6);
                    scf[ch][2][sb] = func_0204e6f0(bs, 6);
                    break;
                case 1:
                    scf[ch][0][sb] = scf[ch][1][sb] = func_0204e6f0(bs, 6);
                    scf[ch][2][sb] = func_0204e6f0(bs, 6);
                    break;
                case 3:
                    scf[ch][0][sb] = func_0204e6f0(bs, 6);
                    scf[ch][1][sb] = scf[ch][2][sb] = func_0204e6f0(bs, 6);
                    break;
                case 2:
                    scf[ch][0][sb] = scf[ch][1][sb] = scf[ch][2][sb] = func_0204e6f0(bs, 6);
                    break;
                }
            } else {
                scf[ch][0][sb] = scf[ch][1][sb] = scf[ch][2][sb] = 0x3f;
            }
        }
    }
    for (; sblimit < 32; sblimit++) {
        for (ch = 0; ch < nch; ch++) {
            scf[ch][0][sblimit] = scf[ch][1][sblimit] = scf[ch][2][sblimit] = 0x3f;
        }
    }
}

extern int data_020ae488[];

// `long` part/sb/ch/nch/bound fix the stack frame. The generic grouping loop counts with `a` (the allocation
// value, dead by then); a separate counter variable takes a different callee-saved register.
void func_0204b880(Bs *bs, SbInfo *si, int (*alloc)[32], int (*scf)[3][32], long part, short (*out)[3][32], int *mtab) {
    long sb, ch, nch, bound;
    QEnt (*tab)[16];
    int sblimit;
    sblimit = si->sblimit;
    nch = si->nch;
    bound = si->bound;
    tab = si->alloc;
    for (sb = 0; sb < sblimit; sb++) {
        for (ch = 0; ch < (sb < bound ? nch : 1); ch++) {
            int a = alloc[ch][sb];
            if (a != 0) {
                QEnt *e = &tab[sb][a];
                int mul, add;
                int g;
                int t;
                add = e->add;
                mul = e->mul;
                g = mtab[scf[ch][part][sb]];
                if (e->cls == 3) {
                    int nb = e->bits;
                    if (nb <= 8) {
                        int v = func_0204e6f0(bs, nb * 3);
                        int m = data_020ae488[nb];
                        int x0 = v & m;
                        int x1 = (v >> nb) & m;
                        int x2 = (v >> nb) >> nb;
                        t = (mul * x0 + add) >> 2; t = (t * g) >> 16; out[ch][2][sb] = t;
                        t = (mul * x1 + add) >> 2; t = (t * g) >> 16; out[ch][1][sb] = t;
                        t = (x2 * mul + add) >> 2; t = (t * g) >> 16; out[ch][0][sb] = t;
                    } else {
                        t = func_0204e6f0(bs, nb); t = (t * mul + add) >> 2; t = (t * g) >> 16; out[ch][0][sb] = t;
                        t = func_0204e6f0(bs, nb); t = (t * mul + add) >> 2; t = (t * g) >> 16; out[ch][1][sb] = t;
                        t = func_0204e6f0(bs, nb); t = (t * mul + add) >> 2; t = (t * g) >> 16; out[ch][2][sb] = t;
                    }
                } else {
                    int grp = e->grp;
                    int v = func_0204e6f0(bs, e->bits);
                    if (grp == 3) {
                        t = (v % 3 * mul + add) >> 2; t = (t * g) >> 16; out[ch][0][sb] = t;
                        v /= 3;
                        t = (v % 3 * mul + add) >> 2; t = (t * g) >> 16; out[ch][1][sb] = t;
                        v /= 3;
                        t = (v % 3 * mul + add) >> 2; t = (t * g) >> 16; out[ch][2][sb] = t;
                    } else if (grp == 5) {
                        t = v % 5; v /= 5; t = (t * mul + add) >> 2; t = (t * g) >> 16; out[ch][0][sb] = t;
                        t = (v % 5 * mul + add) >> 2; t = (t * g) >> 16; out[ch][1][sb] = t;
                        v /= 5;
                        t = (v % 5 * mul + add) >> 2; t = (t * g) >> 16; out[ch][2][sb] = t;
                    } else if (grp == 9) {
                        t = v % 9; v /= 9; t = (t * mul + add) >> 2; t = (t * g) >> 16; out[ch][0][sb] = t;
                        t = (v % 9 * mul + add) >> 2; t = (t * g) >> 16; out[ch][1][sb] = t;
                        v /= 9;
                        t = (v % 9 * mul + add) >> 2; t = (t * g) >> 16; out[ch][2][sb] = t;
                    } else {
                        for (a = 0; a < 3; a++) {
                            t = v % grp; t = (t * mul + add) >> 2; t = (t * g) >> 16; out[ch][a][sb] = t;
                            v /= grp;
                        }
                    }
                }
            } else {
                out[ch][0][sb] = 0;
                out[ch][1][sb] = 0;
                out[ch][2][sb] = 0;
            }
            if (nch == 2 && sb >= bound) {
                int i;
                for (i = 0; i < 3; i++) out[1][i][sb] = out[0][i][sb];
            }
        }
    }
    for (; sblimit < 32; sblimit++) {
        for (sb = 0; sb < nch; sb++) {
            for (ch = 0; ch < 3; ch++) out[sb][ch][sblimit] = 0;
        }
    }
}
