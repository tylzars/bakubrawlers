/* CRI ADX player decode/stream-state code, 0x02043c14-0x02044614 */
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


extern Pl data_02215dd4[4];
extern int data_02215dc0, data_02216840, data_02216844, data_02216848;
extern void (*data_0221683c)(Pl *, int, int, int);
extern char data_020acfc0[];

extern void func_02040170(const char *);
extern void func_020401cc(const char *, const char *);
extern void func_020402f0(int, int, char *, int);
extern void *func_02012484(void *, int, int);
extern void func_0203eda4(void);
extern void func_0203edb0(void);
extern int func_0203ef10(int, int, short *);
extern int func_0203f69c(int, int, short *);
extern int func_02051478(int, int, short *);
extern void func_0204604c(Out *, int, Out *, Out *);
extern int func_020409a0(int), func_02040988(int), func_0204138c(int), func_02040948(int);
extern int func_02040964(int, int), func_02040978(int);
extern int func_020416b0(int), func_020416b8(int), func_020416f4(int);
extern int func_02041704(int), func_02041714(int), func_02041724(int), func_02041734(int), func_02041744(int);
extern int func_02041754(int), func_02041848(int), func_020417a0(int);
extern int func_020416c0(int, int), func_020416d0(int, void *, void *), func_020416dc(int, int);
extern int func_020416e4(int, int), func_020416ec(int, int);
extern int func_02041d50(int), func_02041f50(int, int), func_02041f18(int, void *, void *);
extern unsigned func_0201f970(unsigned, int);
extern void func_02042dd8(Pl *, int, int, int, int);
extern void func_02042f14(Pl *);
extern int func_02042f94(Pl *), func_02042fdc(Pl *);
extern void func_02043188(Pl *, int);
extern void func_0204355c(Pl *, int);
extern void func_020434fc(Pl *, int, int);
extern void func_02043528(Pl *, int *, int *);
extern void func_02043bd0(Pl *);
extern void func_02043b44(Pl *);
extern void func_0204395c(Pl *);
extern void func_02043c14(Pl *);
extern int func_02047914(int, int), func_0204792c(int), func_02047938(int), func_02047944(int);
extern int func_02047920(int, int);
extern int func_02047950(void);
extern int func_0204795c(int, int), func_02047968(int, int), func_0204798c(int, int), func_02047998(int);
extern int func_020479bc(int, int), func_020479c0(int, int);
extern int func_02051500(int, int), func_02051530(int);

void func_02043c14(Pl *o)
{
    int h = o->h;
    Io *io = o->io;
    short s2, s0;
    Out A, C, B, D;
    int r8, r0, r9, r11, r5, r4;
    if (o->b98 == 0) return;
    s0 = 0;
    s2 = 0;
    func_0203eda4();
    io->vt->f18(io, 1, 0x7fffffff, &A);
    io->vt->f18(io, 1, 0x7fffffff, &B);
    r11 = 0;
    r9 = 0;
    if (func_020416f4(h) == 0 && func_0203f69c(A.p, A.n, &s2) != 0) {
        func_0204355c(o, r11);
        io->vt->f1c(io, 1, &B);
        io->vt->f1c(io, 1, &A);
        func_0203edb0();
        return;
    }
    r9 += s2;
    if (func_020416f4(h) == 1) {
        r8 = func_02051478(A.p + r9, A.n - r9, &s2);
        if (r8 == 0) r0 = -1;
        else r0 = func_02051478(B.p, B.n, &s0);
    } else {
        r8 = func_0203ef10(A.p + r9, A.n - r9, &s2);
        if (r8 == 0) r0 = -1;
        else r0 = func_0203ef10(B.p, B.n, &s0);
    }
    r5 = s2;
    r4 = s0;
    if (r8 != 0 && r0 != 0) {
        io->vt->f1c(io, 1, &B);
        io->vt->f1c(io, 1, &A);
        func_0204355c(o, 0);
        func_0203edb0();
        return;
    }
    if (r8 == 0) {
        io->vt->f1c(io, 1, &B);
        func_0204604c(&A, r9 + r5, &A, &C);
        io->vt->f20(io, 0, &A);
        io->vt->f1c(io, 1, &C);
    } else {
        io->vt->f20(io, 0, &A);
        func_0204604c(&B, r11 + r4, &B, &D);
        io->vt->f20(io, 0, &B);
        io->vt->f1c(io, 1, &D);
    }
    func_0203edb0();
    o->ia4 += func_020416b8(h);
    func_020409a0(h);
    func_02040988(h);
    func_0204138c(h);
    if (func_02040948(h) != 2) {
        func_0204355c(o, 0);
        return;
    }
    func_02040964(h, o->i48);
    func_020416dc(h, func_02041744(h));
    func_020416ec(h, 0);
    func_020416e4(h, 0);
}

void func_02043f28(Pl *o)
{
    int h = o->h;
    int a, b, n, r5, r6, p, q, t;
    char buf[32];
    int st;
    a = 0;
    b = 0;
    if ((unsigned char)o->s2 <= 1 && o->ba8 == 1) {
        if (func_02041d50(o->i8) == 2) return;
        if (o->io10 != 0) o->io10->vt->f14(o->io10);
        func_02042dd8(o, o->ib0, o->ib4, o->ib8, o->ibc);
        o->ba8 = 0;
    }
    st = func_02040948(h);
    if (st == 2) {
        n = func_02041714(h);
        if (n > o->s3) {
            func_020402f0(n, o->s3, buf, 16);
            func_020401cc(data_020acfc0, buf);
            func_02042f14(o);
            return;
        }
        r6 = func_02041704(h);
        r5 = func_02041754(h);
        if (func_020416f4(h) == 10) {
            o->i48 = func_0201f970(r6, o->i38);
            r6 = func_02041734(h);
        } else {
            if (r5 > 0) o->i48 = func_0201f970(r6, o->i38) * 3;
            else o->i48 = (int)func_0201f970(r6, o->i38) * 3 / 2;
            r6 = func_02041734(h) * 2;
        }
        o->i48 = r6 * func_0201f970(o->i48 + r6, r6);
        func_02040964(h, o->i48);
        if (r5 > 0) {
            if (o->s2 == 2) {
                o->i50 = 0;
            } else {
                int x = func_020417a0(h);
                r5 = 2048 - x % 2048;
                o->i50 = r5 % 2048;
                o->i8c = (x + 0x7ff) / 2048;
                func_02041f50(o->i8, o->i8c);
                func_02041f18(o->i8, func_02043b44, o);
            }
            func_02041790(h);
            o->i90 = func_02041764(h);
            func_020416dc(h, o->i90);
            func_020416ec(h, 0);
            func_020416e4(h, 0);
            func_020416d0(h, func_0204395c, o);
        } else {
            if (o->i8 != 0) func_02041f50(o->i8, 0x7fffffff);
            func_020416dc(h, func_02041744(h));
            func_020416ec(h, 0);
            func_020416e4(h, 0);
            func_020416d0(h, func_02043c14, o);
        }
        p = func_02041704(h);
        q = func_02041714(h);
        t = func_02041744(h);
        func_0204798c(o->ic, func_02041724(h));
        func_02047968(o->ic, p);
        func_0204795c(o->ic, q);
        func_020479bc(o->ic, t);
        func_02043188(o, o->s40);
        func_02043528(o, &a, &b);
        if (a != 0 || b != 0) func_020434fc(o, a, b);
        func_02043bd0(o);
        if (o->i74 != 0) func_02051500(o->i74, p);
        if (func_020416f4(h) == 2) func_020479c0(o->ic, func_02041848(h));
        func_02047914(o->ic, 1);
        if (data_0221683c != 0) data_0221683c(o, p, q, t);
        o->state = 2;
    } else if (st == 4) {
        o->state = 6;
    }
}

void func_020442ec(Pl *o)
{
    int ic, r5, r6, h, lim, n, i, sz;
    Out out;
    ic = o->ic;
    h = o->h;
    r5 = func_0204792c(ic);
    r6 = func_02047938(ic);
    lim = o->i48;
    if (lim >= 0x800) lim = 0x800;
    if (r5 >= lim || r6 <= func_02041734(h) || func_02040948(o->h) == 3) {
        if (o->b70 == 0) {
            if (o->b72 == 0) {
                func_02047920(ic, 1);
                o->i9c = 0;
                o->ia0 = data_02215dc0;
            }
            o->state = 3;
        }
        o->b71 = 1;
    }
    if (func_02040948(o->h) != 3) return;
    n = func_02042fdc(o);
    sz = o->i48 * n * 2;
    for (i = 0; i < n; i++) {
        Io *ch = o->ch[i];
        ch->vt->f18(ch, 0, sz, &out);
        func_02012484((void *)out.p, 0, out.n);
        ch->vt->f20(ch, 1, &out);
    }
}

void func_02044440(Pl *o)
{
    int i, n, v;
    if (o->b6c == 0 && o->ic0 != 0) {
        if (func_020416b0(o->h) >= o->ic0) func_020416dc(o->h, -1);
    }
    if (func_02040948(o->h) != 3) return;
    n = func_02041714(o->h);
    data_02216848 = n;
    func_02047998(o->ic);
    i = 0;
    if (n > 0) {
        do {
            v = o->ch[i]->vt->f24(o->ch[i], 1);
            if (v < 0x40) i++;
            data_02216844 = v;
        } while (v < 0x40 && i < n);
    }
    if (i != n) return;
    func_02047914(o->ic, 0);
    o->state = 4;
}

void func_02044514(Pl *o)
{
    data_02216840 = func_0204792c(o->ic);
    if (func_02047944(o->ic) != 2) o->state = 5;
}

void func_02044548(void) { }

void func_0204454c(Pl *o)
{
    if (o->i8 != 0 && func_02041d50(o->i8) == 4) {
        *(short *)((char *)o + 0x60) = -1;
        o->state = 6;
    }
    if (o->i94 == 0) return;
    if (func_02051530(o->i94) != 3) return;
    *(short *)((char *)o + 0x60) = -1;
    o->state = 6;
}

void func_020445a8(Pl *o)
{
    if (o->i8 == 0) return;
    if (func_02042f94(o) == 0) return;
    switch (o->s2) {
    case 0:
    case 1:
        if (func_02041d50(o->i8) == 3) func_02040978(o->h);
        break;
    case 2:
        func_02040978(o->h);
        break;
    case 3:
    case 4:
        break;
    }
}
