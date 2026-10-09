#include "cri_obj_b.h"

extern int data_02215c78;            /* decode bias (dither); not volatile: fcd8 loads it once for both channels */
extern int data_02218f3c;            /* decoder variant: 0 = stereo split, else mono downmix */
extern int data_0209e2e4[16];        /* signed nibble table */

#define SW16(x) ((((x) >> 8) & 0xff) | (((x) << 8) & 0xff00))
#define S16(p) (*(short *)(p))
#define CLIP(v)                                   \
    if ((v) > 0x7fff || (v) < -0x8000) {          \
        if ((v) < -0x8000) (v) = -0x8000;         \
        else if ((v) > 0x7fff) (v) = 0x7fff;      \
    }

int func_0203f714(unsigned char *src, int nblk, short *out, short *hist,
                  short c0, short c1, short *rnd, short mul, short add)
{
    int y1 = hist[0];
    int y2 = hist[1];
    int i = 0;
    if (nblk > 0) {
        do {
            int scale, k, j;
            signed char b;
            scale = (short)SW16(S16(src));
            if (scale & 0x8000) return i;
            k = scale ^ *rnd;
            *rnd = *rnd * mul + add;
            *rnd = *rnd & 0x7fff;
            scale = (short)((k & 0x1fff) + 1);
            src += 2;
            for (j = 0; j < 0x20; j += 2) {
                b = *(signed char *)src++;
                y2 = (b >> 4) * scale + ((c0 * y1 + c1 * y2) >> 12);
                y2 += data_02215c78;
                CLIP(y2);
                out[0] = y2;
                y1 = data_0209e2e4[b & 0xf] * scale + ((c0 * y2 + c1 * y1) >> 12) + data_02215c78;
                CLIP(y1);
                out[1] = y1;
                out += 2;
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
    int y1a;
    int y1b;
    int y2a;
    int y2b;
    int nb;
    int i;
    y1a = hist1[0];
    y2a = hist1[1];
    y1b = hist2[0];
    y2b = hist2[1];
    nb = n / 2;
    i = 0;
    if (nb > 0) {
        do {
            int scale1, scale2, k, j, mix;
            signed char b1, b2;
            scale1 = (short)SW16(S16(src));
            if (scale1 & 0x8000) return i << 1;
            k = scale1 ^ *rnd;
            scale1 = (short)((k & 0x1fff) + 1);
            *rnd = *rnd * mul + add;
            *rnd = *rnd & 0x7fff;
            scale2 = (short)SW16(S16(src + 0x12));
            if (scale2 & 0x8000) return i << 1;
            src += 2;
            k = scale2 ^ *rnd;
            *rnd = *rnd * mul + add;
            scale2 = (short)((k & 0x1fff) + 1);
            *rnd = *rnd & 0x7fff;
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
                out2[0] = mix;
                out1[0] = out2[0];
                y1a = data_0209e2e4[b1 & 0xf] * scale1 + ((c0 * y2a + c1 * y1a) >> 12);
                CLIP(y1a);
                y1b = data_0209e2e4[b2 & 0xf] * scale2 + ((c0 * y2b + c1 * y1b) >> 12);
                CLIP(y1b);
                mix = (y1a + y1b) * 7 / 10 + data_02215c78;
                CLIP(mix);
                out2[1] = mix;
                out1[1] = out2[1];
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

int func_0203fcd8(unsigned char *src, int n, short *out1, short *hist1, short *out2, short *hist2,
                  short c0, short c1, short *rnd, short mul, short add)
{
    int y1a = hist1[0];
    int y2a = hist1[1];
    int y1b = hist2[0];
    int y2b = hist2[1];
    int nb = n / 2;
    int i = 0;
    if (nb > 0) {
        do {
            int scale1, scale2, k, j, t1, t2;
            signed char b1, b2;
            scale1 = (short)SW16(S16(src));
            if (scale1 & 0x8000) return i << 1;
            k = scale1 ^ *rnd;
            scale1 = (short)((k & 0x1fff) + 1);
            *rnd = *rnd * mul + add;
            *rnd = *rnd & 0x7fff;
            scale2 = (short)SW16(S16(src + 0x12));
            if (scale2 & 0x8000) return i << 1;
            src += 2;
            k = scale2 ^ *rnd;
            *rnd = *rnd * mul + add;
            scale2 = (short)((k & 0x1fff) + 1);
            *rnd = *rnd & 0x7fff;
            for (j = 0; j < 0x20; j += 2) {
                b1 = *(signed char *)src;
                b2 = *(signed char *)(src + 0x12);
                src++;
                y2a = (b1 >> 4) * scale1 + ((c0 * y1a + c1 * y2a) >> 12);
                y2a += data_02215c78;
                CLIP(y2a);
                y2b = (b2 >> 4) * scale2 + ((c0 * y1b + c1 * y2b) >> 12);
                y2b += data_02215c78;
                CLIP(y2b);
                out1[0] = y2a;
                t1 = data_0209e2e4[b1 & 0xf];
                out2[0] = y2b;
                t2 = data_0209e2e4[b2 & 0xf];
                y1a = t1 * scale1 + ((c0 * y2a + c1 * y1a) >> 12);
                y1a += data_02215c78;
                CLIP(y1a);
                y1b = t2 * scale2 + ((c0 * y2b + c1 * y1b) >> 12);
                y1b += data_02215c78;
                CLIP(y1b);
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
