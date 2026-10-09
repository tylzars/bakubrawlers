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
static const double half = 0.5;
static const double S1 = -1.66666666666666324348e-01;
static const double S2 = 8.33333333332248946124e-03;
static const double S3 = -1.98412698298579493134e-04;
static const double S4 = 2.75573137070700676789e-06;
static const double S5 = -2.50507602534068634195e-08;
static const double S6 = 1.58969099521155010221e-10;

double func_0201b72c(double x, double y, int iy)
{
    double z, r, v;
    int ix;
    ix = __HI(x) & 0x7fffffff;
    if (ix < 0x3e400000) {
        if ((int)x == 0) return x;
    }
    z = x * x;
    v = z * x;
    r = S2 + z * (S3 + z * (S4 + z * (S5 + z * S6)));
    if (iy == 0) return x + v * (S1 + z * r);
    else return x - ((z * (half * y - v * r) - y) - v * S1);
}
