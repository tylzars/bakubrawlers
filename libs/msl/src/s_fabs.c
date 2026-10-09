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
extern double func_0201e20c(double, double);

double func_0201bb40(double x) {
    __HI(x) &= 0x7fffffff;
    return x;
}
