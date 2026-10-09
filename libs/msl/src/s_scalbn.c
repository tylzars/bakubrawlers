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
extern double func_0201b9a8(double, double);
extern int func_0201d690(double);
static const double huge = 1.0e300;
static const double tiny = 1.0e-300;
static const double two54 = 1.80143985094819840000e+16;
static const double twom54 = 5.55111512312578270212e-17;

double func_0201bddc(double x, int n) {
    int k, hx, lx;
    if (func_0201d690(x) <= 2 || x == 0.0) return x;
    hx = __HI(x);
    lx = __LO(x);
    k = (hx & 0x7ff00000) >> 20;
    if (k == 0) {
        if ((lx | (hx & 0x7fffffff)) == 0) return x;
        x *= two54;
        hx = __HI(x);
        k = ((hx & 0x7ff00000) >> 20) - 54;
        if (n < -50000) return tiny * x;
    }
    if (k == 0x7ff) return x + x;
    k = k + n;
    if (k > 0x7fe) return huge * func_0201b9a8(huge, x);
    if (k > 0) {
        __HI(x) = (hx & 0x800fffff) | (k << 20);
        return x;
    }
    if (k <= -54) {
        if (n > 50000) return huge * func_0201b9a8(huge, x);
        else return tiny * func_0201b9a8(tiny, x);
    }
    k += 54;
    __HI(x) = (hx & 0x800fffff) | (k << 20);
    return x * twom54;
}
