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

extern CriGlobals gCri;              /* 0x022158a0 */
extern CriObj gCriObjs[4];           /* 0x022158bc */
extern short data_0209dae4[];                   /* 0x0209dae4 */
extern short data_0209dce4, data_0209dee4, data_0209e0e4;     /* 0x0209dce4 / dee4 / e0e4 */
extern char str_a[], str_b[], str_c[], str_d[], str_e[], str_f[];

extern void CriSlotClearAll(void);
extern void *memset(void *, int, int);
extern int CriSlotAlloc(void);
extern void CriSlotDealloc(int);
extern void SendErrConcat(const char *, const char *);
extern void SendErr(const char *);
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
extern void CriFormatString(void *, int, const char *);

int func_0203dee8(void) { gCri.init++; return 0; }

int func_0203df04(signed char *s, int n, short *o0, short *o1, short *o2)
{
    int i;
    short a, b, c;
    if (gCri.init == 0) func_0203dee8();
    *o0 = 0; *o1 = 0; *o2 = 0;
    if (s == 0 && n <= 0) return 0;
    a = data_0209dce4;
    for (i = 0; i < n; i++) a = data_0209dae4[(a * data_0209dae4[128 + s[i]]) % 1024];
    b = data_0209dee4;
    for (i = 0; i < n; i++) b = data_0209dae4[(b * data_0209dae4[128 + s[i]]) % 1024];
    c = data_0209e0e4;
    for (i = 0; i < n; i++) c = data_0209dae4[(c * data_0209dae4[128 + s[i]]) % 1024];
    *o0 = a; *o1 = b; *o2 = c;
    return 0;
}

void func_0203e05c(int v) { gCri.mode = v; }

int func_0203e06c(void) { return gCri.mode; }

void func_0203e07c(void)
{
    CriSlotClearAll();
    func_0203dee8();
    __clear(gCriObjs, sizeof(gCriObjs));
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

// The zero is an enum-typed local so it stays in a register through the loop (mwcc does not propagate enum
// values); storing it as (unsigned char) avoids a sign-extension before the signed char stores.
void func_0203e110(CriObj *o)
{
    enum { ZERO } z = ZERO;
    unsigned i;
    signed char *p;
    o->ic0 = z;
    o->sd4 = z;
    o->sd6 = -128;
    o->sd8 = -128;
    p = o->bc4;
    i = 4;
    do {
        p[0] = (unsigned char)z;
        p[1] = (unsigned char)z;
        p[2] = (unsigned char)z;
        p[3] = (unsigned char)z;
        p += 4;
    } while (--i);
}

extern void CriObjDealloc(CriObj *);

CriObj *CriObjAlloc(int a, int b, int c, int d)
{
    CriObj *p = gCriObjs;
    int i;
    CriObj *o;
    for (i = 0; i < 4; i++) {
        if (p->used == 0) break;
        p++;
    }
    if (i == 4) return 0;
    o = &gCriObjs[i];
    memset(o, 0, 0xec);
    gCriObjs[i].used = 1;
    o->h = CriSlotAlloc();
    if (o->h == 0) { CriObjDealloc(o); return 0; }
    o->i38 = a; o->i3c = b; o->i40 = c; o->i44 = d;
    o->i78 = (int)func_0203e0c4;
    o->i7c = (int)o;
    o->i80 = (int)func_0203e0f4;
    o->i84 = (int)o;
    func_0203e110(o);
    return o;
}

void CriObjDealloc(CriObj *o)
{
    int h;
    if (o == 0) return;
    h = o->h;
    o->h = 0;
    CriSlotDealloc(h);
    memset(o, 0, 0xec);
    o->used = 0;
}

int func_0203e254(CriObj *o, int flag, int len)
{
    int s30;
    short s2[4];
    short s28, s26, s24;
    short s22;
    short r20;
    short s1e, s1c, s1a;
    unsigned char c19, c18;
    if ((flag & 1) && func_0203e06c() == 0) {
        SendErrConcat(str_e, str_b);
        return -1;
    }
    o->s2 = 1;
    if (func_0203ef70(flag, len, &r20, &o->c, &o->d, &o->f, &o->e, &o->i14, &o->i18, &o->i10) < 0) return 0;
    if (o->c > 4) {
        if (o->ib4 == 0) {
            SendErrConcat(str_c, str_d);
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
        if (gCri.cb != 0) gCri.cb(o->ib4, &s22);
    } else {
        if (func_0203f0f0(flag, len, &c19, &c18) < 0) return 0;
        if (func_0203e790(o, c19, c18, o->i18, &s1e, &s1c, &s1a) < 0) return -1;
        func_020447f4(o->h, s1e, s1c, s1a);
        if (func_0203f060(flag, len, &o->s1c) < 0) return 0;
        if (func_0203f174(flag, len, &s2[2], &s2[0]) < 0) return 0;
        func_02044790(o->h, o->i14, o->s1c);
        func_020447ac(o->h, (int *)&s2[2], (int *)&s2[0]);
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
    SendErr(str_a);
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

/* u28.s[0..3] and s34 / s36 / s38 only (not s30 / s32). */
void CriObjSaveSlotValues(CriObj *o)
{
    func_020447d0(o->h, &o->iac, &o->ib0);
    func_02044804(o->h, &o->sa6, &o->sa8, &o->saa);
}

void CriObjRestoreSlotValues(CriObj *o)
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
        CriFormatString(buf, 16, str_f);
        func_0203df04((signed char *)buf, 8, p0, p1, p2);
    } else if (b >= 8) {
        if (o->sa0 == 0 && o->sa2 == 0 && o->sa4 == 0) {
            o->sa0 = gCri.dflt[0];
            o->sa2 = gCri.dflt[1];
            o->sa4 = gCri.dflt[2];
        }
        *p0 = o->sa0; *p1 = o->sa2; *p2 = o->sa4;
    } else {
        *p0 = 0; *p1 = 0; *p2 = 0;
    }
    return 0;
}

int CriObjGetState(CriObj *o) { return o->state; }

void func_0203e87c(CriObj *o, int a, int b)
{
    if (o->s98 == 0) {
        o->i48 = a;
        o->i4c = b / o->f;
        o->i74 = 0;
    } else {
        o->i48 = a;
        o->i4c = b / ((o->d / 8) * o->e);
        o->i74 = 0;
    }
    o->i90 = 0;
    o->i94 = 0;
    o->ie0 = 0;
    o->idc = 0;
}

void StartCriObj(CriObj *o) { if (o->state == 0) o->state = 1; }   /* nothing unless state is 0 */

/* func_020448d0 also zeroes the slot's state and u28 values (the ones CriObjSaveSlotValues copies). */
void StopCriObj(CriObj *o) { func_020448d0(o->h); o->state = 0; }

void func_0203e918(CriObj *o)
{
    if (o->state != 3) return;
    func_020448fc(o->h);
    o->i8c = 0;
    o->state = 0;
}
int func_0203e944(CriObj *o) { return o->i94; }
int func_0203e94c(CriObj *o) { return o->i90; }
