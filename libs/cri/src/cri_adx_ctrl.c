/* CRI ADX player control, 0x02043220-0x020445a8 */
#pragma dont_inline on

typedef struct Io { struct IoVt *vt; } Io;
struct IoVt { int v[5]; int (*f14)(); int (*f18)(); int (*f1c)(); int (*f20)(); int (*f24)(); };

typedef struct Pl {
    signed char used;      /* 0x00 */
    signed char state;     /* 0x01 */
    signed char s2;        /* 0x02 */
    signed char s3;        /* 0x03 */
    int h;                 /* 0x04 */
    int i8;                /* 0x08 */
    int ic;                /* 0x0c */
    Io *io10;              /* 0x10 */
    Io *io;                /* 0x14 */
    Io *ch[8];             /* 0x18 */
    int i38;
    int pad3c;
    short s40, s42, s44, s46;
    int i48, i4c, i50;
    int pad54[6];
    signed char b6c, b6d, b6e, b6f;
    signed char b70, b71, b72, b73;
    int i74;
    int pad78[5];
    int i8c, i90, i94;
    signed char b98, b99, b9a, b9b;
    int i9c, ia0, ia4;
    signed char ba8, ba9, baa, bab;
    int iac, ib0, ib4, ib8, ibc, ic0;
} Pl;                      /* 0xc4 */

typedef struct Hdr { int w0; int w4; struct { int a, b; } e[1]; } Hdr;
typedef struct Out { int p; int n; } Out;

typedef struct Glob {
    void (*f0)(int); int a4;
    void (*f8)(int); int ac;
    void (*f10)(int); int a14;
    void (*f18)(int); int a1c;
    int pad20;
    int st;
    int i28, i2c;
} Glob;

extern Glob data_022167d8;
extern Pl data_02215dd4[4];
extern int data_0221683c_unused;
extern char data_020acdd8[], data_020ace04[], data_020ace30[], data_020ace60[], data_020ace8c[];
extern char data_020aceb0[], data_020acee0[], data_020acf0c[], data_020acf38[], data_020acf64[];
extern char data_020acf94[];

extern void func_02040170(const char *);
extern void func_020401cc(const char *, const char *);
extern void func_020402f0(int, int, char *, int);
extern void func_0203eda4(void);
extern void func_0203edb0(void);
extern void func_0203edbc(void);
extern void func_0203edc0(void);
extern void func_02041644(void);
extern void func_02047950(void);
extern void func_02044614(Pl *);
extern int func_020416b0(int);
extern int func_020416c0(int, int);
extern int func_020416c8(int);
extern int func_020416d0(int, void *, void *);
extern int func_020416dc(int, int);
extern int func_020416e4(int, int);
extern int func_020416ec(int, int);
extern int func_02041714(int);
extern int func_02041764(int);
extern int func_02041774(int);
extern int func_02041790(int);
extern int func_020417a0(int);
extern int func_02041850(int);
extern int func_02041860(int);
extern int func_02041d50(int);
extern int func_02041d7c(int, int);
extern int func_02041f50(int, int);
extern int func_020479a4(int);
extern void func_020479b0(int, int);
extern int func_02051514(int, int, int, int *, int *, int *);
extern void func_02051558(int, int, int);
extern int func_020457cc(int, int);
extern void func_02042f14(Pl *);
extern void func_02042d18(Pl *, int);
extern void func_0204303c(Pl *, int, int);
extern void func_02043554(Pl *, int, int);
extern void func_02043558(Pl *, int, int);
extern void func_02043580(Pl *, int);
extern void func_020435c8(Pl *, int, int);
extern void func_020436e8(Pl *, int, int, int);
extern void func_020437c4(Pl *, int, int);
extern void func_02043874(Pl *, Hdr *, int);
extern void func_02043780(Pl *, int);
extern void func_0204349c(Pl *, int);
extern int func_020434d0(Pl *);
extern void func_02043290(void);
extern void func_0204339c(Pl *, int);
extern void func_0204355c(Pl *, int);
extern void func_02043268(int);
extern void func_020439e0(Pl *);
extern void func_02043b44(Pl *);
extern void func_0204395c(Pl *);

int func_02043220(Pl *p)
{
    if (p != 0) return p->s40;
    func_02040170(data_020acdd8);
    return 0;
}

void func_02043244(Pl *p, int v) { p->ba9 = v; }

void func_0204324c(int v)
{
    func_0203edbc();
    func_02043268(v);
    func_0203edc0();
}

void func_02043268(int v) { data_022167d8.i28 = v; data_022167d8.i2c = v; }

void func_0204327c(void)
{
    func_0203edbc();
    func_02043290();
    func_0203edc0();
}

void func_02043290(void)
{
    int i;
    Pl *p;
    func_0203eda4();
    if (data_022167d8.st != 0) {
        func_0203edb0();
        return;
    }
    data_022167d8.st = 1;
    func_0203edb0();
    if (data_022167d8.f18 != 0) data_022167d8.f18(data_022167d8.a14);
    func_02041644();
    data_022167d8.st = 2;
    p = data_02215dd4;
    for (i = 0; i < 4; i++) {
        if (p->used == 1) func_02044614(p);
        p++;
    }
    data_022167d8.st = 3;
    if (data_022167d8.f8 != 0) data_022167d8.f8(data_022167d8.a4);
    func_02047950();
    if (data_022167d8.f0 != 0) data_022167d8.f0(data_022167d8.a1c);
    data_022167d8.st = 0;
    if (data_022167d8.f10 != 0) data_022167d8.f10(data_022167d8.ac);
}

void func_02043378(Pl *p, int v)
{
    func_0203edbc();
    func_0204339c(p, v);
    func_0203edc0();
}

void func_0204339c(Pl *o, int flag)
{
    int a, r5, r4, r8;
    if (o == 0) {
        func_02040170(data_020ace04);
        return;
    }
    if (o->io == 0) a = 0;
    else a = o->io->vt->f24(o->io, 1);
    if (o->s2 != 4 && o->b6c == 1 && flag == 0) {
        r5 = a + func_020416b0(o->h);
        r4 = (func_02041774(o->h) + 0x7ff) / 2048;
        r8 = ((func_020417a0(o->h) + 0x7ff) / 2048) * 2048 - r4 * 2048;
        if (r8 <= 0) {
            o->ic0 = 0;
        } else {
            o->ic0 = r8 * ((r5 - r4 * 2048) / r8) + r4 * 2048;
        }
    }
    o->b6c = flag;
}

void func_02043478(Pl *p, int v)
{
    func_0203edbc();
    func_0204349c(p, v);
    func_0203edc0();
}

void func_0204349c(Pl *p, int v) { func_020479b0(p->ic, v); }

int func_020434ac(Pl *p)
{
    int r;
    func_0203edbc();
    r = func_020434d0(p);
    func_0203edc0();
    return r;
}

int func_020434d0(Pl *p)
{
    if (p == 0) {
        func_02040170(data_020ace30);
        return 0;
    }
    return func_020479a4(p->ic);
}

void func_020434fc(Pl *p, int a, int b)
{
    func_0203edbc();
    func_02043554(p, a, b);
    func_0203edc0();
}

void func_02043528(Pl *p, int a, int b)
{
    func_0203edbc();
    func_02043558(p, a, b);
    func_0203edc0();
}

void func_02043554(Pl *p, int a, int b) { }
void func_02043558(Pl *p, int a, int b) { }

void func_0204355c(Pl *p, int v)
{
    func_0203edbc();
    func_02043580(p, v);
    func_0203edc0();
}

void func_02043580(Pl *p, int v)
{
    p->b98 = v;
    if (p->h == 0) return;
    func_020416c8(p->h);
}

void func_0204359c(Pl *p, int a, int b)
{
    func_0203edbc();
    func_020435c8(p, a, b);
    func_0203edc0();
}

void func_020435c8(Pl *o, int a, int b)
{
    char buf[16];
    int sc, s10, s8;
    if (o == 0) {
        func_02040170(data_020ace60);
        return;
    }
    func_02042f14(o);
    if (func_02051514(a, b, o->iac, &s8, &sc, &s10) != 0) return;
    if (o->i8 == 0) {
        func_020402f0(a, b, buf, 16);
        func_020401cc(data_020ace8c, buf);
        *(short *)((char *)o + 0x60) = -1;
        o->state = 6;
        return;
    }
    o->ib0 = o->iac;
    o->ib4 = s8;
    o->ib8 = sc;
    o->ibc = s10;
    o->state = 1;
    o->ba8 = 1;
    o->s2 = 1;
    func_0204355c(o, 0);
}

void func_020436b4(Pl *p, int a, int b, int c)
{
    func_0203edbc();
    func_020436e8(p, a, b, c);
    func_0203edc0();
}

void func_020436e8(Pl *o, int a, int b, int c)
{
    if (o == 0 || a == 0) {
        func_02040170(data_020aceb0);
        return;
    }
    func_02042f14(o);
    func_02051558(o->iac, 0x100, a);
    o->ib0 = o->iac;
    o->ib4 = 0;
    o->ib8 = b;
    o->ibc = c;
    o->state = 1;
    o->ba8 = 1;
    o->s2 = 0;
    func_0204355c(o, 0);
}

void func_0204375c(Pl *p, int a)
{
    func_0203edbc();
    func_02043780(p, a);
    func_0203edc0();
}

void func_02043780(Pl *p, int a) { func_020436e8(p, a, 0, 0xfffff); }

void func_02043798(Pl *p, int a, int b)
{
    func_0203edbc();
    func_020437c4(p, a, b);
    func_0203edc0();
}

void func_020437c4(Pl *o, int a, int b)
{
    int r;
    if (o == 0 || a == 0 || b < 0) {
        func_02040170(data_020acee0);
        return;
    }
    func_02042f14(o);
    func_0203eda4();
    r = func_020457cc(a, b);
    if (r == 0) {
        func_0203edb0();
        func_02040170(data_020acf0c);
        return;
    }
    o->s2 = 2;
    func_02042d18(o, r);
    func_0204355c(o, 0);
    func_0203edb0();
}

void func_02043848(Pl *p, int a, int b)
{
    func_0203edbc();
    func_02043874(p, a, b);
    func_0203edc0();
}

#define BSWAP(x) ((((x) >> 24) & 0xff) | (((x) >> 8) & 0xff00) | (((x) << 8) & 0xff0000) | ((x) << 24))

void func_02043874(Pl *o, Hdr *h, int idx)
{
    int r;
    int off;
    if (o == 0 || h == 0 || idx < 0) {
        func_02040170(data_020acf38);
        return;
    }
    func_02042f14(o);
    if (idx >= BSWAP(h->w4)) return;
    if (idx < 0) return;
    func_0203eda4();
    off = h->e[idx].a;
    r = func_020457cc((int)h + BSWAP(off), 0x40000000);
    if (r == 0) {
        func_0203edb0();
        func_02040170(data_020acf64);
        return;
    }
    o->s2 = 2;
    func_02042d18(o, r);
    func_0204355c(o, 0);
    func_0203edb0();
}

void func_0204395c(Pl *o)
{
    int h = o->h;
    int a = func_02041764(h);
    int b = func_02041774(h);
    int c = func_02041790(h);
    int d;
    func_02041850(h);
    func_020416e4(h, 0);
    d = c - a;
    o->i90 = d;
    func_020416dc(h, d);
    func_020416ec(h, b);
    func_020416c0(h, a);
    func_020416d0(h, func_020439e0, o);
}

void func_020439e0(Pl *o)
{
    Out out;
    int h = o->h;
    Io *io = o->io;
    int a = func_02041764(h);
    int b = func_02041774(h);
    int c = func_02041790(h);
    if ((unsigned char)(signed char)(o->s2 - 2) <= 1 && o->b6c == 0) {
        func_020416dc(o->h, -1);
        return;
    }
    io->vt->f18(io, 1, o->i50, &out);
    if (out.n < o->i50) func_02040170(data_020acf94);
    io->vt->f20(io, 0, &out);
    func_020416e4(h, 0);
    func_020416dc(h, o->i90 = c - a);
    func_020416ec(h, b);
    func_020416c0(h, a);
    if (o->s2 == 2) {
        io->vt->f14(io);
        io->vt->f18(io, 1, b, &out);
        io->vt->f20(io, 0, &out);
    }
    func_02041860(h);
    o->i4c++;
}

void func_02043b44(Pl *o)
{
    int h = o->h;
    int io8 = o->i8;
    int r;
    if (io8 == 0 || h == 0) return;
    r = func_02041774(h);
    if (o->s2 == 4) {
        func_02041f50(o->i8, 0x7fffffff);
        return;
    }
    if (o->b6c == 0) {
        if (func_020416b0(o->h) >= o->ic0) func_020416dc(o->h, -1);
        func_02041f50(o->i8, 0x7fffffff);
        return;
    }
    func_02041d7c(io8, r / 2048);
}

void func_02043bd0(Pl *o)
{
    if (func_02041714(o->h) == 1) {
        func_0204303c(o, 0, o->s42);
    } else {
        func_0204303c(o, 0, o->s42);
        func_0204303c(o, 1, o->s44);
    }
}
