#include "cri_obj_b.h"

extern void func_02044848(int, int, int, short *, int);
extern void CriSlotStart(int);
extern void func_0204487c(int, int, int, short *, short *);
extern int CriSlotGetResult(int);
extern int CriSlotGetState(int);
extern void func_0204492c(int);
extern void func_020448fc(int);
extern void func_0203deac(CriObj *);
extern void CopyShorts(short *, short *, int);
extern void func_0203e9dc(CriObj *);

void func_0203e954(CriObj *o, int n)
{
    int h = o->h;
    short *p = o->i5c;
    p += o->i68;
    func_02044848(h, o->i48, n, p, 0);
    CriSlotStart(h);
}

void func_0203e994(CriObj *o, int n)
{
    short *p = o->i5c + o->i68;
    int h = o->h;
    func_0204487c(h, o->i48, n * 2, p, p + o->i64);
    CriSlotStart(h);
}

void func_0203e9dc(CriObj *o)
{
    int pos, lim, f, x, u, r5, y, base, t, a, b, yb;
    f = o->i50;
    a = o->i4c / f;
    pos = o->i70;
    b = o->i58;
    t = pos + b - 1;
    base = o->i68;
    r5 = o->i6c;
    x = t / b;
    u = b - 1 - (t % b);
    lim = o->i60;
    y = (lim - base + b - 1) / b;
    yb = y * b;
    if (x < y) {
        if (base + yb - u < lim) y++;
    }
    if (pos < r5) r5 += u;
    r5 = r5 / b;
    if (a > r5) a = r5;
    if (a > x) a = x;
    if (a > y) a = y;
    if (f == 2) func_0203e994(o, a);
    else func_0203e954(o, a);
}

void CopyShorts(short *dst, short *src, int n)
{
    while (n > 0) {
        *dst++ = *src++;
        n--;
    }
}

void func_0203eb00(short *base, int a, int b, int n)
{
    CopyShorts(base, base + a, n);
    CopyShorts(base + b, base + (b + a), n);
}

void func_0203eb38(short *base, int a, int b, int n)
{
    CopyShorts(base, base + a, n);
}

extern void func_0203eb38(short *, int, int, int);
extern void func_0203eb00(short *, int, int, int);

void func_0203eb4c(CriObj *o)
{
int c = o->i54;
int b = o->i58;
int base = o->i68;
short *buf = o->i5c;
int lim = o->i40;
int gap = o->i44;
int v, f, q, r, n, d;
int t = o->i70 + b - 1;
int rem = b - 1 - (t % b);
v = CriSlotGetResult(o->h);
f = o->i50;
q = (v * b) / f;
r = t / b;
if (r * f <= v) q -= rem;
n = base + q;
o->i90 = q;
o->i94 = v * c;
if (n < lim) return;
d = n - lim;
if (o->i50 == 2) func_0203eb00(buf, lim, gap, d);
else func_0203eb38(buf, lim, gap, d);
}

void func_0203ec2c(CriObj *o)
{
    if (o->state == 1 && CriSlotGetState(o->h) == 0) {
        o->cb78(o->i7c, &o->i68, &o->i6c, &o->i70);
        func_0203e9dc(o);
        o->state = 2;
    }
    if (o->state != 2) return;
    func_0204492c(o->h);
    if (CriSlotGetState(o->h) != 3) return;
    func_0203eb4c(o);
    func_020448fc(o->h);
    o->cb80(o->i84, o->i94, o->i90);
    o->state = 3;
}

extern void func_0203ed0c(CriObj *);

void func_0203eccc(CriObj *o)
{
    int m = o->s98;
    if (m == 0) func_0203ec2c(o);
    else if (m == 10) func_0203deac(o);
    if (o->cbe4 != 0) func_0203ed0c(o);
}

void func_0203ed0c(CriObj *o)
{
    int c = o->i90;
    int d = o->i94 - o->idc;
    if (d < 0) d = 0x7fffffff - o->idc + o->i94;
    o->cbe4(o->ie8, d, o->e * (c << 1));
    o->idc = o->i94;
}

typedef struct CriLock { int f0, f4, cnt; } CriLock;
extern volatile CriLock data_02215c6c;

void func_0203ed54(void)
{
    data_02215c6c.cnt++;
    if (data_02215c6c.cnt == 1) data_02215c6c.f4 = 0;
}

void func_0203ed7c(void)
{
    data_02215c6c.cnt--;
    if (data_02215c6c.cnt == 0) data_02215c6c.f4 = 0;
}

extern void CriLockEnter1(void);
extern void CriLockLeave1(void);

void func_0203eda4(void) { CriLockEnter1(); }

void func_0203edb0(void) { CriLockLeave1(); }

void func_0203edbc(void) {}

void func_0203edc0(void) {}
