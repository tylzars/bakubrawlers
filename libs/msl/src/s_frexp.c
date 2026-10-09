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
extern double func_0201d72c(double, double);
extern double func_0201dcf4(double, double);
extern int func_0201e87c(double, double);
extern int func_0201ea54(double, double);
extern int func_0201d690(double);
static const double two54 = 1.80143985094819840000e+16;

double func_0201bd20(double x, int *eptr) {
    int hx, ix, lx;
    hx = __HI(x);
    ix = 0x7fffffff & hx;
    lx = __LO(x);
    *eptr = 0;
    if (ix >= 0x7ff00000 || ((ix | lx) == 0)) return x;
    if (ix < 0x00100000) {
        x *= two54;
        hx = __HI(x);
        ix = hx & 0x7fffffff;
        *eptr = -54;
    }
    *eptr += (ix >> 20) - 1022;
    hx = (hx & 0x800fffff) | 0x3fe00000;
    __HI(x) = hx;
    return x;
}

