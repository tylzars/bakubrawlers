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

void func_0204b5f4(Bs *bs, SbInfo *si, int (*alloc)[32], int (*sfsel)[32], int *scf) {
    int sblimit = si->sblimit;
    int nch = si->nch;
    int sb, ch;
    short m;
    if (alloc[0][0] == 0) {
        sfsel[0][0] = 0;
    } else {
        sfsel[0][0] = func_0204e6f0(bs, 2);
    }
    m = si->sel[sfsel[0][0]];
    for (sb = 1; sb < sblimit; sb++) {
        if (alloc[0][sb] != 0) {
            sfsel[0][sb] = func_0204e6f0(bs, 2) ^ (m & 3);
        }
        m >>= 2;
    }
    for (sb = sblimit; sb < 32; sb++)
        for (ch = 0; ch < nch; ch++)
            sfsel[ch][sb] = 0;
    for (sb = 0; sb < sblimit; sb++) {
        int *p = scf + sb;
        for (ch = 0; ch < nch; ch++) {
            if (alloc[ch][sb] != 0) {
                int a;
                switch (sfsel[ch][sb]) {
                case 0:
                    p[0] = func_0204e6f0(bs, 6);
                    p[32] = func_0204e6f0(bs, 6);
                    p[64] = func_0204e6f0(bs, 6);
                    break;
                case 1:
                    a = func_0204e6f0(bs, 6);
                    p[32] = a;
                    p[0] = a;
                    p[64] = func_0204e6f0(bs, 6);
                    break;
                case 3:
                    a = func_0204e6f0(bs, 6);
                    p[64] = a;
                    p[32] = a;
                    p[0] = a;
                    break;
                case 2:
                    p[0] = func_0204e6f0(bs, 6);
                    a = func_0204e6f0(bs, 6);
                    p[64] = a;
                    p[32] = a;
                    break;
                }
            } else {
                p[64] = 0x3f;
                p[32] = 0x3f;
                p[0] = 0x3f;
            }
            p += 96;
        }
    }
    for (; sblimit < 32; sblimit++) {
        int *p = scf + sblimit;
        for (ch = 0; ch < nch; ch++) {
            p[64] = 0x3f;
            p[32] = 0x3f;
            p[0] = 0x3f;
            p += 96;
        }
    }
}

extern int data_020ae488[];

void func_0204b880(Bs *bs, SbInfo *si, int (*alloc)[32], int (*scf)[32], int part, short (*out)[3][32], int *mtab) {
    int sblimit = si->sblimit;
    int nch = si->nch;
    int bound = si->bound;
    QEnt (*tab)[16] = si->alloc;
    int sb, ch, i;
    for (sb = 0; sb < sblimit; sb++) {
        int *sp = &scf[part][sb];
        short *o = &out[0][0][sb];
        short *o0 = o;
        for (ch = 0; ch < (sb >= bound ? 1 : nch); ch++) {
            int a = alloc[ch][sb];
            if (a != 0) {
                QEnt *e = &tab[sb][a];
                int mul = e->mul, add = e->add;
                int g = mtab[*sp];
                if (e->cls == 3) {
                    int nb = e->bits;
                    if (nb <= 8) {
                        int v = func_0204e6f0(bs, nb * 3);
                        int m = data_020ae488[nb];
                        int x0 = v & m;
                        int x1 = (v >> nb) & m;
                        int x2 = (v >> nb) >> nb;
                        o[64] = (((x0 * mul + add) >> 2) * g) >> 16;
                        o[32] = (((x1 * mul + add) >> 2) * g) >> 16;
                        o[0] = (((x2 * mul + add) >> 2) * g) >> 16;
                    } else {
                        o[0] = (((func_0204e6f0(bs, nb) * mul + add) >> 2) * g) >> 16;
                        o[32] = (((func_0204e6f0(bs, nb) * mul + add) >> 2) * g) >> 16;
                        o[64] = (((func_0204e6f0(bs, nb) * mul + add) >> 2) * g) >> 16;
                    }
                } else {
                    int grp = e->grp;
                    int v = func_0204e6f0(bs, e->bits);
                    if (grp == 3) {
                        o[0] = (((v % 3 * mul + add) >> 2) * g) >> 16;
                        v /= 3;
                        o[32] = (((v % 3 * mul + add) >> 2) * g) >> 16;
                        v /= 3;
                        o[64] = (((v % 3 * mul + add) >> 2) * g) >> 16;
                    } else if (grp == 5) {
                        o[0] = (((v % 5 * mul + add) >> 2) * g) >> 16;
                        v /= 5;
                        o[32] = (((v % 5 * mul + add) >> 2) * g) >> 16;
                        v /= 5;
                        o[64] = (((v % 5 * mul + add) >> 2) * g) >> 16;
                    } else if (grp == 9) {
                        o[0] = (((v % 9 * mul + add) >> 2) * g) >> 16;
                        v /= 9;
                        o[32] = (((v % 9 * mul + add) >> 2) * g) >> 16;
                        v /= 9;
                        o[64] = (((v % 9 * mul + add) >> 2) * g) >> 16;
                    } else {
                        for (i = 0; i < 3; i++) {
                            o[i * 32] = (((v % grp * mul + add) >> 2) * g) >> 16;
                            v /= grp;
                        }
                    }
                }
            } else {
                o[0] = 0;
                o[32] = 0;
                o[64] = 0;
            }
            if (nch == 2 && sb >= bound) {
                for (i = 0; i < 3; i++) o0[i * 32 + 96] = o0[i * 32];
            }
            o += 96;
            sp += 96;
        }
    }
    for (; sblimit < 32; sblimit++) {
        short *o = &out[0][0][sblimit];
        for (ch = 0; ch < nch; ch++) {
            for (i = 0; i < 3; i++) o[i * 32] = 0;
            o += 96;
        }
    }
}
