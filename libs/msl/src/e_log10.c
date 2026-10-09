/*
 * ====================================================
 * Copyright (C) 1993 by Sun Microsystems, Inc. All rights reserved.
 *
 * Developed at SunPro, a Sun Microsystems, Inc. business.
 * Permission to use, copy, modify, and distribute this
 * software is freely granted, provided that this notice
 * is preserved.
 * ====================================================
 */

#define __HI(x) *(1+(int*)&x)
#define __LO(x) *(int*)&x
extern double func_020186f0(double);
extern int data_020c1790;
extern double data_020c179c;
static const double two54 = 1.80143985094819840000e+16;
static const double ivln10 = 4.34294481903251816668e-01;
static const double log10_2hi = 3.01029995663611771306e-01;
static const double log10_2lo = 3.69423907715893078616e-13;

double func_02018e28(double x)
{
    double y, z;
    int i, k, hx;
    unsigned lx;

    hx = __HI(x);
    lx = __LO(x);
    k = 0;
    if (hx < 0x00100000) {
        if (((hx & 0x7fffffff) | lx) == 0) {
            data_020c1790 = 33;
            return -two54 / data_020c179c;
        }
        if (hx < 0) {
            data_020c1790 = 33;
            return (x - x) / data_020c179c;
        }
        k -= 54;
        x *= two54;
        hx = __HI(x);
    }
    if (hx >= 0x7ff00000) return x + x;
    k += (hx >> 20) - 1023;
    i = ((unsigned)k & 0x80000000) >> 31;
    hx = (hx & 0x000fffff) | ((0x3ff - i) << 20);
    y = (double)(k + i);
    __HI(x) = hx;
    z = y * log10_2lo + ivln10 * func_020186f0(x);
    return z + y * log10_2hi;
}
