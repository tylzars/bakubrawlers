#include "cri_obj_b.h"

extern long data_02215c78;           /* decode bias (dither) */
extern int data_02218f3c;            /* decoder variant: 0 = stereo split, else mono downmix */
extern const int data_0209e2e4[16];  /* signed nibble table */

#define SW16(x) ((((x) >> 8) & 0xff) | (((x) << 8) & 0xff00))
#define S16(p) (*(short *)(p))
#define CLIP(v)                                   \
    if ((v) > 0x7fff || (v) < -0x8000) {          \
        if ((v) < -0x8000) (v) = -0x8000;         \
        else if ((v) > 0x7fff) (v) = 0x7fff;      \
    }

static inline int clip16(int v)
{
    if (v > 0x7fff || v < -0x8000) {
        if (v < -0x8000) v = -0x8000;
        else if (v > 0x7fff) v = 0x7fff;
    }
    return v;
}

int func_0203f714(unsigned char *src, int nblk, short *out, short *hist,
                  short c0, short c1, short *rnd, short mul, short add)
{
    // Placeholder: the zero initialisers on scale and y2 are never read; they only set the register order the
    // original has. Declaration order alone doesn't reproduce it.
    int scale = 0;
    int y2 = 0;
    int i;
    int y1;
    int b;
    int j;
    y1 = hist[0];
    y2 = hist[1];
    i = 0;
    if (nblk > 0) {
        do {
            int k;
            scale = (short)SW16(S16(src));
            if (scale & 0x8000) return i;
            k = scale ^ *rnd;
            *rnd = *rnd * mul + add;
            *rnd &= 0x7fff;
            src += 2;
            for (j = 0; j < 0x20; j += 2) {
                scale = (short)((k & 0x1fff) + 1);
                b = *(signed char *)src++;
                y2 = (b >> 4) * scale + ((c0 * y1 + c1 * y2) >> 12);
                y2 += data_02215c78;
                y2 = clip16(y2);
                *out++ = y2;
                b = data_0209e2e4[b & 0xf];
                y1 = b * scale + ((c0 * y2 + c1 * y1) >> 12);
                y1 += data_02215c78;
                y1 = clip16(y1);
                *out++ = y1;
            }
            i++;
        } while (i < nblk);
    }
    hist[0] = y1;
    hist[1] = y2;
    return nblk;
}

int func_0203f8c4(unsigned char *src, int n, short *out1, short *hist1, short *out2, short *hist2,
                  short c0, short c1, short *rnd, short mul, short add)
{
    int j;
    int i;
    int b1;
    int b2;
    int nb;
    int y2a;
    int y1b;
    int y1a;
    int mix;
    int y2b;
    int scale1;
    int scale2;
    int k;
    nb = n / 2;
    y1a = hist1[0];
    y2a = hist1[1];
    y1b = hist2[0];
    y2b = hist2[1];
    i = 0;
    if (nb > 0) {
        do {
            scale1 = (short)SW16(S16(src));
            if (scale1 & 0x8000) return i << 1;
            k = scale1 ^ *rnd;
            scale1 = (short)((k & 0x1fff) + 1);
            *rnd = *rnd * mul + add;
            *rnd &= 0x7fff;
            scale2 = (short)SW16(S16(src + 0x12));
            if (scale2 & 0x8000) return i << 1;
            k = scale2 ^ *rnd;
            src += 2;
            *rnd = *rnd * mul + add;
            scale2 = (short)((k & 0x1fff) + 1);
            *rnd &= 0x7fff;
            for (j = 0; j < 0x20; j += 2) {
                b1 = *(signed char *)src;
                b2 = *(signed char *)(src + 0x12);
                src++;
                y2a = (b1 >> 4) * scale1 + ((c0 * y1a + c1 * y2a) >> 12);
                CLIP(y2a);
                y2b = (b2 >> 4) * scale2 + ((c0 * y1b + c1 * y2b) >> 12);
                CLIP(y2b);
                mix = (y2a + y2b) * 7 / 10;
                CLIP(mix);
                b1 = data_0209e2e4[b1 & 0xf];
                b2 = data_0209e2e4[b2 & 0xf];
                *out1++ = *out2++ = mix;
                y1a = b1 * scale1 + ((c0 * y2a + c1 * y1a) >> 12);
                CLIP(y1a);
                y1b = b2 * scale2 + ((c0 * y2b + c1 * y1b) >> 12);
                CLIP(y1b);
                mix = (y1a + y1b) * 7 / 10;
                mix += data_02215c78;
                CLIP(mix);
                *out1++ = *out2++ = mix;
            }
            src += 0x12;
            i++;
        } while (i < nb);
    }
    hist1[0] = y1a;
    hist1[1] = y2a;
    hist2[0] = y1b;
    hist2[1] = y2b;
    return n;
}

int func_0203fcd8(unsigned char *src, int n, short *out1, short *hist1, short *out2, short *hist2,
                  short c0, short c1, short *rnd, short mul, short add)
{
    // Placeholder: the zero initialisers on scale1, scale2, d, y2a, y1b and y1a are never read; they only set the
    // register and stack-slot order the original has.
    int scale1 = 0;
    int scale2 = 0;
    int d = 0;
    int j;
    int i = 0;
    int b1;
    int b2;
    int nb;
    int y2a = 0;
    int y1b = 0;
    int y1a = 0;
    int y2b;
    int k;
    y1a = hist1[0];
    y2a = hist1[1];
    y1b = hist2[0];
    y2b = hist2[1];
    nb = n / 2;
    if (nb > 0) {
        do {
            scale1 = (short)SW16(S16(src));
            if (scale1 & 0x8000) return i << 1;
            d = *rnd;
            k = scale1 ^ d;
            scale1 = (short)((k & 0x1fff) + 1);
            *rnd = d * mul + add;
            *rnd &= 0x7fff;
            scale2 = (short)SW16(S16(src + 0x12));
            if (scale2 & 0x8000) return i << 1;
            d = *rnd;
            k = scale2 ^ d;
            src += 2;
            *rnd = d * mul + add;
            scale2 = (short)((k & 0x1fff) + 1);
            *rnd &= 0x7fff;
            for (j = 0; j < 0x20; j += 2) {
                b1 = *(signed char *)src;
                b2 = *(signed char *)(src + 0x12);
                src++;
                y2a = (b1 >> 4) * scale1 + ((c0 * y1a + c1 * y2a) >> 12);
                d = data_02215c78;
                y2a += d;
                y2a = clip16(y2a);
                y2b = (b2 >> 4) * scale2 + ((c0 * y1b + c1 * y2b) >> 12);
                y2b += d;
                y2b = clip16(y2b);
                out1[0] = y2a;
                b1 = data_0209e2e4[b1 & 0xf];
                out2[0] = y2b;
                b2 = data_0209e2e4[b2 & 0xf];
                y1a = b1 * scale1 + ((c0 * y2a + c1 * y1a) >> 12);
                d = data_02215c78;
                y1a += d;
                y1a = clip16(y1a);
                y1b = b2 * scale2 + ((c0 * y2b + c1 * y1b) >> 12);
                y1b += d;
                y1b = clip16(y1b);
                out1[1] = y1a;
                out2[1] = y1b;
                out1 += 2;
                out2 += 2;
            }
            src += 0x12;
            i++;
        } while (i < nb);
    }
    hist1[0] = y1a;
    hist1[1] = y2a;
    hist2[0] = y1b;
    hist2[1] = y2b;
    return n;
}

int func_0204006c(unsigned char *src, int n, short *out1, short *hist1, short *out2, short *hist2,
                  short c0, short c1, short *rnd, short mul, short add)
{
    if (data_02218f3c == 0)
        return func_0203fcd8(src, n, out1, hist1, out2, hist2, c0, c1, rnd, mul, add);
    return func_0203f8c4(src, n, out1, hist1, out2, hist2, c0, c1, rnd, mul, add);
}
