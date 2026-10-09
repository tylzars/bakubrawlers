#include "cri_obj_b.h"

extern void func_02044848(int, int, int, short *, int);
extern void func_020448b0(int);
extern void func_0204487c(int, int, int, short *, short *);
extern int func_02044910(int);
extern int func_02044840(int);
extern void func_0204492c(int);
extern void func_020448fc(int);
extern void func_0203deac(CriObj *);
extern void func_0203eae0(short *, short *, int);
extern void func_0203e9dc(CriObj *);

void func_0203e954(CriObj *o, int n)
{
    int h = o->h;
    short *p = o->i5c + o->i68;
    func_02044848(h, o->i48, n, p, 0);
    func_020448b0(h);
}

void func_0203e994(CriObj *o, int n)
{
    short *p = o->i5c + o->i68;
    int h = o->h;
    func_0204487c(h, o->i48, n * 2, p, p + o->i64);
    func_020448b0(h);
}

void func_0203e9dc(CriObj *o)
{
    int f, t, r5, y, base, a, b, x, pos, lim, yb;
    f = o->i50;
    a = func_0201f970(o->i4c, f);
    pos = o->i70;
    b = o->i58;
    t = pos + b - 1;
    base = o->i68;
    r5 = o->i6c;
    x = func_0201f970(t, b);
    t = b - 1 - (t % b);
    lim = o->i60;
    y = func_0201f970(lim - base + b - 1, b);
    yb = y * b;
    if (x < y) {
        if (base + yb - t < lim) y++;
    }
    if (pos < r5) r5 += t;
    r5 = func_0201f970(r5, b);
    if (a > r5) a = r5;
    if (a > x) a = x;
    if (a > y) a = y;
    if (f == 2) func_0203e994(o, a);
    else func_0203e954(o, a);
}

void func_0203eae0(short *dst, short *src, int n)
{
    while (n > 0) {
        *dst++ = *src++;
        n--;
    }
}

void func_0203eb00(short *base, int a, int b, int n)
{
    func_0203eae0(base, base + a, n);
    func_0203eae0(base + b, base + (b + a), n);
}

void func_0203eb38(short *base, int a, int b, int n)
{
    func_0203eae0(base, base + a, n);
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
v = func_02044910(o->h);
f = o->i50;
q = func_0201f970(v * b, f);
r = func_0201f970(t, b);
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
    if (o->state == 1 && func_02044840(o->h) == 0) {
        o->cb78(o->i7c, &o->i68, &o->i6c, &o->i70);
        func_0203e9dc(o);
        o->state = 2;
    }
    if (o->state != 2) return;
    func_0204492c(o->h);
    if (func_02044840(o->h) != 3) return;
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

extern void func_02046154(void);
extern void func_02046164(void);

void func_0203eda4(void) { func_02046154(); }

void func_0203edb0(void) { func_02046164(); }

void func_0203edbc(void) {}

void func_0203edc0(void) {}
