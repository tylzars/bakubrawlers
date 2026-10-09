/* CRI middleware, 0x0203dee8-0x0203e954 */
typedef struct CriGlobals {
    int f0, f4;
    int init;                         /* 0x08 */
    void (*cb)(int, short *);         /* 0x0c */
    short dflt[3];                    /* 0x10 */
    short pad16;
    int mode;                         /* 0x18 */
} CriGlobals;

typedef struct CriObj {
    short used;                       /* 0x00 */
    short s2;
    int state;                        /* 0x04 */
    int h;                            /* 0x08 */
    signed char c;                    /* 0x0c */
    signed char d, e, f;              /* 0x0d.. */
    int i10, i14, i18;
    short s1c;
    short pad1e;
    int i20;
    short s24, s26;
    int i28, i2c, i30, i34;
    int i38, i3c, i40, i44, i48, i4c;
    int i50, i54, i58, i5c, i60, i64;
    int pad68[3];
    int i74;
    int i78, i7c, i80, i84, i88, i8c, i90, i94;
    short s98, s9a;
    short pad9c[2];
    short sa0, sa2, sa4, sa6, sa8, saa;
    int iac;
    int ib0;
    int ib4;
    int pad_b8[2];
    int ic0;
    signed char bc4[16];
    short sd4, sd6, sd8;
    short padda;
    int idc, ie0, ie4;
    int pade8;
} CriObj;                             /* 0xe8 + */

extern CriGlobals g_cri;              /* 0x022158a0 */
extern CriObj g_criObjs[4];           /* 0x022158bc */
extern short tbl[];                   /* 0x0209dae4 */
extern short seed0, seed1, seed2;     /* 0x0209dce4 / dee4 / e0e4 */
extern char str_a[], str_b[], str_c[], str_d[], str_e[], str_f[];

extern void *memset(void *, int, unsigned);
extern void func_020446b0(void);
extern void *func_02012484(void *, int, int);
extern int func_020446e8(void);
extern void func_02044820(int);
extern void func_020401cc(const char *, const char *);
extern void func_02040170(const char *);
extern int func_0203ef70(int, int, short *, signed char *, signed char *, signed char *, signed char *, int *, int *, int *);
extern int func_0203f0f0(int, int, unsigned char *, unsigned char *);
extern int func_0203f060(int, int, short *);
extern int func_0203f174(int, int, short *, short *);
extern int func_0203f2a8(int, int, int *, short *, short *, int *, int *, int *, int *);
extern int func_0203f4a0(int, int, int *, signed char *, short *, short *);
extern void func_02044790(int, int, int);
extern void func_020447ac(int, int *, int *);
extern void func_020447d0(int, int *, int *);
extern void func_020447f4(int, int, int, int);
extern void func_02044804(int, short *, short *, short *);
extern void func_020448d0(int);
extern void func_020448fc(int);
extern unsigned func_0201f970(unsigned, int);
extern void func_020515a0(void *, int, const char *);

int func_0203dee8(void) { g_cri.init++; return 0; }

int func_0203df04(signed char *s, int n, short *o0, short *o1, short *o2)
{
    int i;
    short a, b, c;
    if (g_cri.init == 0) func_0203dee8();
    *o0 = 0; *o1 = 0; *o2 = 0;
    if (s == 0 && n <= 0) return 0;
    a = seed0;
    for (i = 0; i < n; i++) a = tbl[(a * tbl[128 + s[i]]) % 1024];
    b = seed1;
    for (i = 0; i < n; i++) b = tbl[(b * tbl[128 + s[i]]) % 1024];
    c = seed2;
    for (i = 0; i < n; i++) c = tbl[(c * tbl[128 + s[i]]) % 1024];
    *o0 = a; *o1 = b; *o2 = c;
    return 0;
}

void func_0203e05c(int v) { g_cri.mode = v; }

int func_0203e06c(void) { return g_cri.mode; }

void func_0203e07c(void)
{
    func_020446b0();
    func_0203dee8();
    memset(g_criObjs, 0, sizeof(g_criObjs));
    func_0203e05c(0);
}

int func_0203e0c4(CriObj *o, int *a, int *b, int *c)
{
    *a = o->i8c;
    *b = o->i40 - o->i8c;
    *c = o->i18 - o->i88;
    return o->i3c;
}

void func_0203e0f4(CriObj *o, int unused, int n)
{
    o->i8c += n;
    o->i88 += n;
}

void func_0203e110(CriObj *o)
{
    unsigned i;
    signed char *p;
    o->ic0 = 0;
    o->sd4 = 0;
    o->sd6 = -128;
    o->sd8 = -128;
    p = o->bc4;
    i = 4;
    do { p[0] = 0; p[1] = 0; p[2] = 0; p[3] = 0; p += 4; } while (--i);
}

extern void func_0203e21c(CriObj *);

CriObj *func_0203e150(int a, int b, int c, int d)
{
    CriObj *p = g_criObjs;
    int i;
    CriObj *o;
    for (i = 0; i < 4; i++) {
        if (p->used == 0) break;
        p++;
    }
    if (i == 4) return 0;
    o = &g_criObjs[i];
    func_02012484(o, 0, 0xec);
    g_criObjs[i].used = 1;
    o->h = func_020446e8();
    if (o->h == 0) { func_0203e21c(o); return 0; }
    o->i38 = a; o->i3c = b; o->i40 = c; o->i44 = d;
    o->i78 = (int)func_0203e0c4;
    o->i7c = (int)o;
    o->i80 = (int)func_0203e0f4;
    o->i84 = (int)o;
    func_0203e110(o);
    return o;
}

void func_0203e21c(CriObj *o)
{
    int h;
    if (o == 0) return;
    h = o->h;
    o->h = 0;
    func_02044820(h);
    func_02012484(o, 0, 0xec);
    o->used = 0;
}

int func_0203e254(CriObj *o, int flag, int len)
{
    int s30;
    short s2e, s2c, s2a;
    short s28, s26, s24;
    short s22;
    short r20;
    short s1e, s1c, s1a;
    unsigned char c19, c18;
    if ((flag & 1) && func_0203e06c() == 0) {
        func_020401cc(str_a, str_b);
        return -1;
    }
    o->s2 = 1;
    if (func_0203ef70(flag, len, &r20, &o->c, &o->d, &o->f, &o->e, &o->i14, &o->i18, &o->i10) < 0) return 0;
    if (o->c > 4) {
        if (o->ib4 == 0) {
            func_020401cc(str_c, str_d);
            return -1;
        }
        o->d = 8;
        o->f = o->e * 0xc0;
        o->i10 = 0x60;
        o->s98 = 10;
        o->s1c = 0; o->s24 = 0; o->s26 = 0;
        o->i20 = 0; o->i28 = 0; o->i2c = 0; o->i30 = 0; o->i34 = 0;
        o->i88 = 0;
        if (func_0203f0f0(flag, len, &c19, &c18) < 0) return 0;
        s22 = 0;
        if (func_0203e790(o, c19, c18, o->i18, &s24, &s26, &s28) < 0) return -1;
        if (g_cri.cb != 0) g_cri.cb(o->ib4, &s22);
    } else {
        if (func_0203f0f0(flag, len, &c19, &c18) < 0) return 0;
        if (func_0203e790(o, c19, c18, o->i18, &s1e, &s1c, &s1a) < 0) return -1;
        func_020447f4(o->h, s1e, s1c, s1a);
        if (func_0203f060(flag, len, &o->s1c) < 0) return 0;
        if (func_0203f174(flag, len, &s2e, &s2a) < 0) return 0;
        func_02044790(o->h, o->i14, o->s1c);
        func_020447ac(o->h, (int *)&s2e, (int *)&s2a);
        func_0203f2a8(flag, len, &o->i20, &o->s24, &o->s26, &o->i28, &o->i2c, &o->i30, &o->i34);
        func_0203f4a0(flag, len, &o->ic0, o->bc4, &o->sd4, &o->sd6);
        o->s98 = 0;
    }
    o->i50 = o->e;
    o->i54 = o->f;
    o->i58 = o->i10;
    o->i5c = o->i3c;
    o->i60 = o->i40;
    o->i64 = o->i44;
    o->i8c = 0;
    return r20;
}

void func_0203e590(CriObj *o)
{
    o->s2 = 1;
    o->i14 = 0xbb80;
    o->e = 2;
    o->d = 16;
    o->i18 = 0x7fffffff;
    o->f = 0x7f;
    o->i10 = 0x400;
    o->s98 = o->s9a;
    o->i50 = o->e;
    o->i54 = o->f;
    o->i58 = o->i10;
    o->i5c = o->i3c;
    o->i60 = o->i40;
    o->i64 = o->i44;
    o->i8c = 0;
    o->s1c = 0; o->s24 = 0; o->s26 = 0;
    o->i20 = 0; o->i28 = 0; o->i2c = 0; o->i30 = 0; o->i34 = 0;
    o->i88 = 0;
}

int func_0203e634(CriObj *o, unsigned char *p, int n)
{
    func_0203e110(o);
    if ((unsigned short)((p[0] << 8) | p[1]) != 0x8000) return -1;
    return func_0203e254(o, (int)p, n);
}

void func_0203e67c(CriObj *o, int a, int b) { o->i78 = a; o->i7c = b; }
int func_0203e688(CriObj *o) { return o->i3c; }
int func_0203e690(CriObj *o) { return o->s98; }
int func_0203e698(CriObj *o) { return o->i14; }
int func_0203e6a0(CriObj *o)
{
    if (o != 0) return o->e;
    func_02040170(str_a);
    return -1;
}
int func_0203e6c4(CriObj *o) { return o->d; }
int func_0203e6cc(CriObj *o) { return 16; }
int func_0203e6d4(CriObj *o) { return o->i10; }
int func_0203e6dc(CriObj *o) { return o->i18; }
int func_0203e6e4(CriObj *o) { return o->s24; }
int func_0203e6ec(CriObj *o) { return o->i28; }
int func_0203e6f4(CriObj *o) { if (o == 0) return 0; return o->i2c; }
int func_0203e704(CriObj *o) { return o->i30; }
int func_0203e70c(CriObj *o) { return o->i34; }
int func_0203e714(CriObj *o) { return o->ic0; }
int func_0203e71c(CriObj *o) { return o->sd4; }
int func_0203e724(CriObj *o, int i) { return (&o->sd6)[i]; }

void func_0203e730(CriObj *o)
{
    func_020447d0(o->h, &o->iac, &o->ib0);
    func_02044804(o->h, &o->sa6, &o->sa8, &o->saa);
}

void func_0203e760(CriObj *o)
{
    func_020447ac(o->h, &o->iac, &o->ib0);
    func_020447f4(o->h, o->sa6, o->sa8, o->saa);
}

int func_0203e790(CriObj *o, unsigned a, unsigned b, int c, short *p0, short *p1, short *p2)
{
    short buf[8];
    if (a < 4) {
        *p0 = 0; *p1 = 0; *p2 = 0;
    } else if (b >= 16) {
        func_020515a0(buf, 16, str_f);
        func_0203df04((signed char *)buf, 8, p0, p1, p2);
    } else if (b >= 8) {
        if (o->sa0 == 0 && o->sa2 == 0 && o->sa4 == 0) {
            o->sa0 = g_cri.dflt[0];
            o->sa2 = g_cri.dflt[1];
            o->sa4 = g_cri.dflt[2];
        }
        *p0 = o->sa0; *p1 = o->sa2; *p2 = o->sa4;
    } else {
        *p0 = 0; *p1 = 0; *p2 = 0;
    }
    return 0;
}

int func_0203e874(CriObj *o) { return o->state; }

void func_0203e87c(CriObj *o, int a, unsigned b)
{
    int t = o->s98;
    o->i48 = a;
    if (t == 0) {
        o->i4c = func_0201f970(b, o->f);
    } else {
        o->i4c = func_0201f970(b, (o->d / 8) * o->e);
    }
    o->i74 = 0;
    o->i90 = 0;
    o->i94 = 0;
    o->ie0 = 0;
    o->idc = 0;
}

void func_0203e8e8(CriObj *o) { if (o->state == 0) o->state = 1; }

void func_0203e8fc(CriObj *o) { func_020448d0(o->h); o->state = 0; }

void func_0203e918(CriObj *o)
{
    if (o->state != 3) return;
    func_020448fc(o->h);
    o->i8c = 0;
    o->state = 0;
}
int func_0203e944(CriObj *o) { return o->i94; }
int func_0203e94c(CriObj *o) { return o->i90; }
