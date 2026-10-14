#include "cri_obj_b.h"

extern double sqrt(double);
extern double cos(double);

void func_0203edc4(int a, int b, short *o0, short *o1)
{
    double s = sqrt(2.0);
    float w = 6.2831855f * a / b;
    float p = (float)s - (float)cos(w);
    float q = (float)sqrt(2.0) - 1.0f;
    float c = (p - (float)sqrt((double)((p + q) * (p - q)))) / q;
    *o0 = (short)(4096.0f * (2.0f * c));
    *o1 = (short)(4096.0f * ((0.0f - c) * c));
}

int func_0203ef10(unsigned char *p, int n, short *out)
{
    int lim = n - 1;
    int found = 0x7fffffff;
    int i = 0;
    if (lim > 0) {
        do {
            if (*(unsigned short *)(p + i) == 0x80) {
                if (i >= 0x7fffffff) i = 0x7fffffff;
                found = i;
                break;
            }
            i += 2;
        } while (i < lim);
    }
    if (found != 0x7fffffff) {
        *out = found;
        return 0;
    }
    *out = 0;
    return -1;
}

#define SW16(x) ((((x) >> 8) & 0xff) | (((x) << 8) & 0xff00))
#define SW32(x) ((((x) >> 24) & 0xff) | (((x) >> 8) & 0xff00) | (((x) << 8) & 0xff0000) | ((x) << 24))
#define U16(p) (*(unsigned short *)(p))
#define S16(p) (*(short *)(p))
#define S32(p) (*(int *)(p))

int func_0203ef70(unsigned char *p, int len, short *hdr, signed char *c, signed char *d,
                  signed char *f, signed char *e, int *rate, int *total, int *spb)
{
    if (len < 16) return -1;
    if ((unsigned short)((p[0] << 8) | p[1]) != 0x8000) return -2;
    *hdr = ((p[2] << 8) | p[3]) + 4;
    *c = ((signed char *)p)[4];
    *f = ((signed char *)p)[5];
    *d = ((signed char *)p)[6];
    *e = ((signed char *)p)[7];
    *rate = (p[8] << 24) | (p[9] << 16) | (p[10] << 8) | p[11];
    *total = (p[12] << 24) | (p[13] << 16) | (p[14] << 8) | p[15];
    if (*d == 0) *spb = 0; else *spb = (*f - 2) * 8 / *d;
    return 0;
}

int func_0203f060(unsigned char *p, int len, short *out)
{
    if (len < 0x12) return -1;
    if ((unsigned short)(short)SW16(U16(p)) != 0x8000) return -2;
    if ((short)SW16(S16(p + 2)) < 0xe) return -1;
    *out = SW16(U16(p + 0x10));
    return 0;
}

int func_0203f0f0(unsigned char *p, int len, unsigned char *o0, unsigned char *o1)
{
    if (len < 0x14) return -1;
    if ((unsigned short)(short)SW16(U16(p)) != 0x8000) return -2;
    if ((short)SW16(S16(p + 2)) < 0x10) return -1;
    *o0 = p[0x12];
    *o1 = p[0x13];
    return 0;
}

int func_0203f174(unsigned char *p, int len, short *a, short *b)
{
    unsigned char c1, c0;
    if (func_0203f0f0(p, len, &c1, &c0) != 0) return -1;
    if (c1 >= 4) {
        if (len < 0x20) return -1;
        if ((unsigned short)(short)SW16(U16(p)) != 0x8000) return -2;
        if ((short)SW16(S16(p + 2)) < 0x1c) return -1;
        a[0] = SW16(U16(p + 0x18));
        b[0] = SW16(U16(p + 0x1a));
        a[1] = SW16(U16(p + 0x1c));
        b[1] = SW16(U16(p + 0x1e));
    } else {
        b[1] = 0;
        a[1] = 0;
        b[0] = 0;
        a[0] = 0;
    }
    return 0;
}

int func_0203f2a8(unsigned char *p, int len, int *r0, short *r1, short *r2, int *r3, int *r4, int *r5, int *r6)
{
    unsigned char c1, c0;
    int need, off, rc;
    unsigned char *q;
    *r1 = 0;
    rc = func_0203f0f0(p, len, &c1, &c0);
    if (rc != 0) return rc;
    need = 0x18;
    need += 0x18;
    if (c1 == 4) need += 0xc;
    if (len < need) return -1;
    if ((unsigned short)(short)SW16(U16(p)) != 0x8000) return -2;
    if ((short)SW16(S16(p + 2)) < need - 4) return -1;
    off = 0x14;
    if (c1 == 4) off += 0xc;
    q = p + off;
    *r0 = (short)SW16(S16(q));
    *r1 = SW16(S16(q + 2));
    if (*r1 != 1) return -2;
    *r2 = SW16(S16(q + 6));
    *r3 = SW32(S32(q + 8));
    *r4 = SW32(S32(q + 0xc));
    *r5 = SW32(S32(q + 0x10));
    *r6 = SW32(S32(q + 0x14));
    return 0;
}

int func_0203f4a0(unsigned char *p, int len, int *total, signed char *tbl, short *s0, short *s1)
{
    unsigned char c1, c0;
    int need, off, rc, v;
    unsigned i;
    unsigned char *q, *src;
    *total = 0;
    rc = func_0203f0f0(p, len, &c1, &c0);
    if (rc != 0) return rc;
    need = 0x18;
    need += 0x24;
    if (c1 == 4) need += 0xc;
    if (len < need) return -1;
    if ((unsigned short)(short)SW16(U16(p)) != 0x8000) return -2;
    if ((short)SW16(S16(p + 2)) < need - 4) return -1;
    off = 0x14;
    if (c1 == 4) off += 0xc;
    q = p + off;
    v = (short)SW16(S16(q + 2));
    off += 4;
    if (v != 0) off += 0x14;
    q = p + off;
    if (((q[0] << 24) | (q[1] << 16) | (q[2] << 8) | q[3]) != 0x41494e46) return -2;
    *total = SW32(S32(q + 4));
    src = p + (off + 8);
    i = 8;
    do {
        unsigned char a = src[0];
        unsigned char b = src[1];
        src += 2;
        tbl[0] = a;
        tbl[1] = b;
        tbl += 2;
    } while (--i);
    *s0 = SW16(S16(p + 0x18 + off));
    s1[0] = SW16(S16(p + 0x1c + off));
    s1[1] = SW16(S16(p + 0x1e + off));
    return 0;
}

int func_0203f69c(unsigned char *p, int len, short *out)
{
    if (len < 0x10) return -1;
    if ((unsigned short)(short)SW16(U16(p)) != 0x8001) return -2;
    *out = SW16(S16(p + 2));
    *out = *out + 4;
    return 0;
}
