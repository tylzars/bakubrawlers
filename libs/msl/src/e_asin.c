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
extern float data_020a5078;
extern double func_0201e058(double);
static const double huge = 1.000e+300;
static const double pio4_hi = 7.85398163397448278999e-01;
extern double func_0201bb40(double);
static const double one = 1.00000000000000000000e+00;
static const double pi = 3.14159265358979311600e+00;
static const double pio2_hi = 1.57079632679489655800e+00;
static const double pio2_lo = 6.12323399573676603587e-17;
static const double pS0 = 1.66666666666666657415e-01;
static const double pS1 = -3.25565818622400915405e-01;
static const double pS2 = 2.01212532134862925881e-01;
static const double pS3 = -4.00555345006794114027e-02;
static const double pS4 = 7.91534994289814532176e-04;
static const double pS5 = 3.47933107596021167570e-05;
static const double qS1 = -2.40339491173441421878e+00;
static const double qS2 = 2.02094576023350569471e+00;
static const double qS3 = -6.88283971605453293030e-01;
static const double qS4 = 7.70381505559019352791e-02;

double func_02017fa4(double x)
{
    double t, w, p, q, c, r, s;
    int hx, ix;
    hx = __HI(x);
    ix = hx & 0x7fffffff;
    if (ix >= 0x3ff00000) {
        if (((ix - 0x3ff00000) | __LO(x)) == 0)
            return x * pio2_hi + x * pio2_lo;
        return data_020a5078;
    } else if (ix < 0x3fe00000) {
        if (ix < 0x3e400000) {
            if (huge + x > one) return x;
        } else
            t = x * x;
        p = t * (pS0 + t * (pS1 + t * (pS2 + t * (pS3 + t * (pS4 + t * pS5)))));
        q = one + t * (qS1 + t * (qS2 + t * (qS3 + t * qS4)));
        w = p / q;
        return x + x * w;
    }
    w = one - func_0201bb40(x);
    t = w * 0.5;
    p = t * (pS0 + t * (pS1 + t * (pS2 + t * (pS3 + t * (pS4 + t * pS5)))));
    q = one + t * (qS1 + t * (qS2 + t * (qS3 + t * qS4)));
    s = func_0201e058(t);
    if (ix >= 0x3FEF3333) {
        w = p / q;
        t = pio2_hi - (2.0 * (s + s * w) - pio2_lo);
    } else {
        w = s;
        __LO(w) = 0;
        c = (t - w * w) / (s + w);
        r = p / q;
        p = 2.0 * s * r - (pio2_lo - 2.0 * c);
        q = pio4_hi - 2.0 * w;
        t = pio4_hi - (p - q);
    }
    if (hx > 0) return t;
    else return -t;
}
