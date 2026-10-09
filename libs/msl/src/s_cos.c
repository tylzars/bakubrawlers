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
extern double func_0201a850(double, double);
extern double func_0201b72c(double, double, int);
extern int func_0201a188(double, double *);

double cos(double x) {
    double y[2], z = 0.0;
    int n, ix;
    ix = __HI(x);
    ix &= 0x7fffffff;
    if (ix <= 0x3fe921fb) return func_0201a850(x, z);
    else if (ix >= 0x7ff00000) return x - x;
    else {
        n = func_0201a188(x, y);
        switch (n & 3) {
        case 0: return func_0201a850(y[0], y[1]);
        case 1: return -func_0201b72c(y[0], y[1], 1);
        case 2: return -func_0201a850(y[0], y[1]);
        default: return func_0201b72c(y[0], y[1], 1);
        }
    }
}
