/* CRI audio: fade group controller, 0x0204fc00-0x02050698 */
typedef struct CriFader CriFader;
typedef struct CriFx CriFx;

typedef struct CriFxVt {
    int p0, p4;
    void (*m08)(CriFx *, int);
    void (*m0c)(CriFx *, int, int, int);
    void (*m10)(CriFx *, int, int);
    void (*m14)(CriFx *, int, int);
    void (*m18)(CriFx *, int, int);
    void (*m1c)(CriFx *);
    void (*m20)(CriFx *, int);
    int p24;
    int (*m28)(CriFx *);
    void (*m2c)(CriFx *, int);
    int p30;
    void (*m34)(CriFx *, int, int);
    int p38, p3c;
    int (*m40)(CriFx *);
} CriFxVt;

struct CriFx {
    CriFxVt *vt;
};

typedef struct CriSlot {
    CriFx *obj;                       /* 0x00 */
    signed char st;                   /* 0x04 */
    signed char mode;                 /* 0x05 */
    short pad6;
    CriFader *f;                      /* 0x08 */
} CriSlot;

typedef struct CriFadeGrp CriFadeGrp;
typedef void (*CriGrpCb)(CriFadeGrp *, int);

struct CriFadeGrp {
    CriFx *objs[2];                   /* 0x00 */
    CriFx *cur;                       /* 0x08 */
    CriFx *prev;                      /* 0x0c */
    int i10, i14;
    CriSlot slot[2];                  /* 0x18 */
    int w30[10];                      /* 0x30 */
    int w58, w5c, w60, w64, w68, w6c, w70, w74, w78, w7c;
    int w80[3];                       /* 0x80 */
    int ch_n;                         /* 0x8c */
    int w90[3];
    int w9c;                          /* 0x9c */
    int wa0[2];
    int n;                            /* 0xa8 */
    signed char state;                /* 0xac */
    int b0, b4, b8, bc, c0, c4;
    int *pc8;                         /* 0xc8 */
    int cc, d0;
    int d4, d8;
    CriGrpCb dc;                      /* 0xdc */
};

struct CriFader {
    signed char st;                   /* 0x00 */
    int i4, i8, ic, i10, i14;
    signed char curve;                /* 0x18 */
    int i1c, i20, i24, i28;
    void (*cb1)(CriFadeGrp *, int);   /* 0x2c */
    CriFadeGrp *a1;
    int b1;
    void (*cb2)(CriFadeGrp *, int);   /* 0x38 */
    CriFadeGrp *a2;
    int b2;
};

extern char data_020aff80[], data_020affb0[], data_020affe4[];
extern char data_020b0010[], data_020b0044[];
extern void SendErrWrapper(char *);
extern void func_0204ed2c(void), func_0204ed38(void);
extern void *memset(void *, int, int);
extern double cos(double);   /* cos */
extern double sin(double);   /* sin */
extern double acos(double);   /* acos */
extern double asin(double);   /* asin */
extern double log10(double);   /* log10 */
extern double pow(double, double); /* pow */

int func_0204fc00(CriFadeGrp *g, int i);
void func_0204fc64(CriFadeGrp *g, int v);
void func_0204fc7c(CriFadeGrp *g);
void CriFadeGrpAdvance(CriFadeGrp *g);
void CriFadeGrpReset(CriFadeGrp *g);
void func_0204fe20(CriFadeGrp *g, int *arg);
void func_0204ffc8(CriFadeGrp *g, int *c);
void func_02050080(CriFadeGrp *g, CriSlot *s, int a, int b);
void func_0205012c(CriFadeGrp *g, CriSlot *s, int a, int b);
void func_02050268(CriFadeGrp *g, CriSlot *s, int a, int b, int t);
void CriFadeGrpSetState(CriFadeGrp *g, int v);
void func_020502f0(CriFadeGrp *g, CriSlot *s);
void func_02050390(CriFadeGrp *g);
void func_02050414(CriFadeGrp *g);
void func_020504c4(CriFadeGrp *g);
void CriFadeGrpSendValue(CriFadeGrp *g, CriSlot *s);
int CriFadeGrpNoFaderRunning(CriFadeGrp *g);
void CriFadeGrpCallCallback(CriFadeGrp *g, int ev);
CriFader *func_02050698(CriFader *f, unsigned size);
void func_020506e0(CriFader *f);
int CriFaderGetState(CriFader *f);
int CriFaderGetValue(CriFader *f);
void CriFaderSetValue(CriFader *f, int v);
void CriFaderStart(CriFader *f, int a, int b, int c, int d);
void CriFaderSkipToEnd(CriFader *f);
void CriFaderAdvance(CriFader *f);
void CriFaderSetRampStartCallback(CriFader *f, void (*cb)(CriFadeGrp *, int), CriFadeGrp *a, int b);
void CriFaderSetEndCallback(CriFader *f, void (*cb)(CriFadeGrp *, int), CriFadeGrp *a, int b);
void CriFaderReset(CriFader *f);
int CriFaderComputeIncrement(CriFader *f, int a, int b, int c);
int CriFaderCallRampStartCallback(CriFader *f);
int CriFaderCallEndCallback(CriFader *f);
int CriFaderLinearToValue(CriFader *f, int x);
int CriFaderValueToLinear(CriFader *f, int x);

void func_0204ffc8(CriFadeGrp *g, int *c);
void func_02050080(CriFadeGrp *g, CriSlot *s, int a, int b);
void func_0205012c(CriFadeGrp *g, CriSlot *s, int a, int b);
void func_02050268(CriFadeGrp *g, CriSlot *s, int a, int b, int t);
void CriFadeGrpSetState(CriFadeGrp *g, int v);
void func_020502f0(CriFadeGrp *g, CriSlot *s);
void func_02050390(CriFadeGrp *g);
void func_02050414(CriFadeGrp *g);
void func_020504c4(CriFadeGrp *g);
void CriFadeGrpSendValue(CriFadeGrp *g, CriSlot *s);
int CriFadeGrpNoFaderRunning(CriFadeGrp *g);
void CriFadeGrpCallCallback(CriFadeGrp *g, int ev);
CriFader *func_02050698(CriFader *f, unsigned size);
void func_020506e0(CriFader *f);
int CriFaderGetState(CriFader *f);
int CriFaderGetValue(CriFader *f);
void CriFaderSetValue(CriFader *f, int v);
void CriFaderStart(CriFader *f, int a, int b, int c, int d);
void CriFaderSkipToEnd(CriFader *f);
void CriFaderAdvance(CriFader *f);
void CriFaderSetRampStartCallback(CriFader *f, void (*cb)(CriFadeGrp *, int), CriFadeGrp *a, int b);
void CriFaderSetEndCallback(CriFader *f, void (*cb)(CriFadeGrp *, int), CriFadeGrp *a, int b);
void CriFaderReset(CriFader *f);
int CriFaderComputeIncrement(CriFader *f, int a, int b, int c);

int func_0204fc00(CriFadeGrp *g, int i)
{
    if (g->w58 == 0) {
        SendErrWrapper(data_020aff80);
        return 0;
    }
    if (g->w9c == 0) {
        return 0;
    }
    if (i < 0 || i >= g->n) {
        return 0;
    }
    return g->objs[i]->vt->m40(g->objs[i]);
}

void func_0204fc64(CriFadeGrp *g, int v)
{
    g->cc += v;
    g->d0 = 1;
}

void func_0204fc7c(CriFadeGrp *g)
{
    g->cc = 0;
    g->d0 = 0;
}

void CriFadeGrpAdvance(CriFadeGrp *g)
{
    int i;
    if (g->w58 == 0) {
        SendErrWrapper(data_020affb0);
        return;
    }
    func_0204ed2c();
    if (g->state == 1) {
        func_02050414(g);
    }
    for (i = 0; i < g->n; i++) {
        if (g->d0 == 1 || g->slot[i].st != 0) {
            CriFadeGrpSendValue(g, &g->slot[i]);
        }
    }
    func_020504c4(g);
    func_0204ed38();
}

void CriFadeGrpReset(CriFadeGrp *g)
{
    int i;
    unsigned char *p;
    g->cur = 0;
    g->prev = 0;
    g->i10 = 0;
    g->i14 = 0;
    for (i = 0; i < 2; i++) {
        g->objs[i] = 0;
        p = (unsigned char *) &g->slot[i];
        p[0] = 0;
        p[1] = 0;
        p[2] = 0;
        p[3] = 0;
        p[4] = 0;
        p[5] = 0;
        p[6] = 0;
        p[7] = 0;
        p[8] = 0;
        p[9] = 0;
        p[10] = 0;
        p[11] = 0;
    }
    memset(g->w30, 0, 0x28);
    g->w58 = 0;
    g->w5c = 0;
    g->w60 = 0;
    g->w64 = 0;
    g->w68 = 0;
    g->w6c = 0;
    g->w70 = 0;
    g->w74 = 0;
    g->w78 = 0;
    g->w7c = 0;
    memset(g->w80, 0, 0x28);
    g->n = 0;
    g->state = 0;
    g->b0 = 30;
    g->b4 = 0;
    g->b8 = 0;
    g->bc = 0;
    g->c0 = 60;
    g->c4 = 0;
    g->pc8 = 0;
    g->cc = 0;
    g->d0 = 0;
    g->dc = 0;
    g->d4 = 0;
    g->d8 = 0;
}

void func_0204fe20(CriFadeGrp *g, int *arg)
{
    int done = 0;
    int in;
    int out;
    if (g->w58 == 0) {
        SendErrWrapper(data_020affe4);
        return;
    }
    func_0204ed2c();
    in = g->bc;
    if (in < 0) {
        out = -in;
        in = 0;
    } else {
        out = 0;
    }
    if (g->d8 == 1) {
        g->c4 = g->d4;
        g->d4 = 0;
        g->d8 = 0;
    }
    if (g->w64 != 0) {
        func_02050390(g);
        if (g->slot[g->i14].obj->vt->m28(g->slot[g->i14].obj) != 0) {
            func_0205012c(g, &g->slot[g->i14], out, 2);
        }
        if (g->slot[g->i14].obj->vt->m28(g->slot[g->i14].obj) == 2) {
            func_02050080(g, &g->slot[g->i10], in, 0);
        } else {
            func_02050080(g, &g->slot[g->i10], 0, 0);
        }
        done = 1;
    } else {
        if (g->cur->vt->m28(g->cur) != 0) {
            func_0205012c(g, &g->slot[g->i10], 0, 1);
        } else {
            func_02050080(g, &g->slot[g->i10], 0, 0);
            done = 1;
        }
    }
    func_0204ed38();
    if (done != 1) {
        return;
    }
    CriFadeGrpSendValue(g, &g->slot[g->i10]);
    func_0204ffc8(g, arg);
}

void func_0204ffc8(CriFadeGrp *g, int *c)
{
    switch (c[0]) {
    case 0:
        g->cur->vt->m08(g->cur, c[1]);
        break;
    case 1:
        g->cur->vt->m0c(g->cur, c[1], c[8], c[9]);
        break;
    case 2:
        g->cur->vt->m10(g->cur, c[2], c[3]);
        break;
    case 3:
        g->cur->vt->m14(g->cur, c[4], c[5]);
        break;
    case 4:
        g->cur->vt->m18(g->cur, c[6], c[7]);
        break;
    }
}

void func_02050080(CriFadeGrp *g, CriSlot *s, int a, int b)
{
    CriFader *f = s->f;
    CriFadeGrpSetState(g, 1);
    g->cur->vt->m20(g->cur, 1);
    g->w6c = 1;
    CriFaderSetValue(f, -960);
    CriFaderStart(f, g->c4, g->b4, a, g->c0);
    CriFaderSetRampStartCallback(f, CriFadeGrpCallCallback, g, 1);
    CriFaderSetEndCallback(f, CriFadeGrpCallCallback, g, 4);
    s->st = 2;
    s->mode = b;
}

void func_0205012c(CriFadeGrp *g, CriSlot *s, int a, int b)
{
    CriFader *f = s->f;
    CriFadeGrpSetState(g, 1);
    if (s->obj == g->cur) {
        if (g->w68 == 0 && g->w6c == 1) {
            g->cur->vt->m20(g->cur, 0);
        }
        g->w6c = 0;
    }
    if (s->obj->vt->m28(s->obj) == 2 && g->w68 == 0) {
        CriFaderStart(f, -960, g->b0, a, g->c0);
        CriFaderSetRampStartCallback(f, CriFadeGrpCallCallback, g, 0);
        CriFaderSetEndCallback(f, CriFadeGrpCallCallback, g, 3);
    } else {
        CriFaderStart(f, -960, 0, 0, 0);
        CriFaderSetRampStartCallback(f, CriFadeGrpCallCallback, g, 0);
        CriFaderSetEndCallback(f, CriFadeGrpCallCallback, g, 3);
        CriFaderSkipToEnd(f);
        s->obj->vt->m1c(s->obj);
    }
    s->st = 1;
    s->mode = b;
}

void func_02050268(CriFadeGrp *g, CriSlot *s, int a, int b, int t)
{
    CriFader *f = s->f;
    CriFadeGrpSetState(g, 1);
    CriFaderStart(f, t, g->b8, a, g->c0);
    CriFaderSetRampStartCallback(f, CriFadeGrpCallCallback, g, 2);
    CriFaderSetEndCallback(f, CriFadeGrpCallCallback, g, 5);
    s->st = 3;
    s->mode = b;
}

void CriFadeGrpSetState(CriFadeGrp *g, int v)
{
    g->state = v;
}

void func_020502f0(CriFadeGrp *g, CriSlot *s)
{
    switch (s->mode) {
    case 0:
        break;
    case 1:
        func_0204ed2c();
        func_02050080(g, s, g->bc > g->b0 ? g->bc - g->b0 : 0, 0);
        func_0204ed38();
        CriFadeGrpSendValue(g, s);
        func_0204ffc8(g, g->w30);
        break;
    case 2:
        s->obj->vt->m1c(s->obj);
        break;
    case 3:
        s->obj->vt->m20(s->obj, 1);
        break;
    }
}

void func_02050390(CriFadeGrp *g)
{
    func_0204ed2c();
    if (g->w68 == 0 && g->w6c == 1) {
        g->cur->vt->m20(g->cur, 0);
    }
    g->i14 = g->i10;
    g->i10 = g->i10 + 1;
    if (g->i10 >= g->n) {
        g->i10 = 0;
    }
    g->prev = g->slot[g->i14].obj;
    g->cur = g->slot[g->i10].obj;
    func_0204ed38();
}

void func_02050414(CriFadeGrp *g)
{
    int i;
    if (g->w68 == 1) {
        return;
    }
    if (g->w6c == 1) {
        if (CriFaderGetState(g->slot[g->i10].f) != 1) {
            if (g->cur->vt->m28(g->cur) != 2) {
                return;
            }
            g->cur->vt->m20(g->cur, 0);
            g->w6c = 0;
        }
    }
    for (i = 0; i < g->n; i++) {
        if (g->slot[i].st != 0) {
            CriFaderAdvance(g->slot[i].f);
        }
    }
}

void func_020504c4(CriFadeGrp *g)
{
    int i;
    CriSlot *s;
    CriFader *f;
    if (g->state != 1) {
        return;
    }
    for (i = 0; i < g->n; i++) {
        s = &g->slot[i];
        f = s->f;
        if (g->objs[i]->vt->m28(g->objs[i]) == 3) {
            CriFaderSkipToEnd(f);
        }
        if (CriFaderGetState(f) == 0 && s->st != 0) {
            s->st = 0;
            func_020502f0(g, s);
        }
    }
    if (CriFadeGrpNoFaderRunning(g) == 1) {
        if (g->dc != 0) {
            g->dc(g, 6);
        }
        CriFadeGrpSetState(g, 0);
    }
}

void CriFadeGrpSendValue(CriFadeGrp *g, CriSlot *s)
{
    int i;
    CriFader *f = s->f;
    if (g->w9c == 1) {
        for (i = 0; i < g->ch_n; i++) {
int v = g->cc + (CriFaderGetValue(f) + g->pc8[i]); s->obj->vt->m34(s->obj, i, v);
        }
    } else {
        int v = CriFaderGetValue(f);
        s->obj->vt->m2c(s->obj, g->cc + v);
    }
}

int CriFadeGrpNoFaderRunning(CriFadeGrp *g)
{
    int i;
    int r = 0;
    for (i = 0; i < g->n; i++) {
        if (CriFaderGetState(g->slot[i].f) != 0) {
            break;
        }
    }
    if (i == g->n) {
        r = 1;
    }
    return r;
}

void CriFadeGrpCallCallback(CriFadeGrp *g, int ev)
{
    if (g != 0 && g->dc != 0) {
        g->dc(g, ev);
    }
}

CriFader *func_02050698(CriFader *f, unsigned size)
{
    if (f == 0) {
        SendErrWrapper(data_020b0010);
        return 0;
    }
    if (size < 0x44) {
        SendErrWrapper(data_020b0044);
        return 0;
    }
    CriFaderReset(f);
    return f;
}

void func_020506e0(CriFader *f)
{
    if (f == 0) {
        return;
    }
    CriFaderReset(f);
}

int CriFaderGetState(CriFader *f)
{
    return f->st;
}

int CriFaderGetValue(CriFader *f)
{
    return f->i1c;
}

void CriFaderSetValue(CriFader *f, int v)
{
    f->i1c = v;
    f->i20 = CriFaderValueToLinear(f, v);
}

void CriFaderStart(CriFader *f, int a, int b, int c, int d)
{
    f->i28 = f->i24;
    f->i24 = a;
    f->i4 = CriFaderComputeIncrement(f, f->i1c, a, b);
    f->i10 = c;
    f->i14 = d;
    f->i8 = 0;
    f->ic = 0;
    if (b > 0 || c > 0) {
        f->st = 1;
        return;
    }
    f->st = 2;
    CriFaderSetValue(f, a);
}

void CriFaderSkipToEnd(CriFader *f)
{
    CriFaderSetValue(f, f->i24);
    f->st = 0;
    CriFaderCallEndCallback(f);
}

void CriFaderAdvance(CriFader *f)
{
    int st = CriFaderGetState(f);
    if (st == 0) {
        return;
    }
    f->i8++;
    if (f->i8 < f->i10) {
        return;
    }
    if (st == 1) {
        st = 2;
        f->st = 2;
        CriFaderCallRampStartCallback(f);
    }
    if (st == 2) {
        int v = f->i20 + f->i4;
        f->i20 = v;
        if (v <= 0) {
            v = 0;
        }
        f->i20 = v;
        if (v >= 0x316938) {
            v = 0x316938;
        }
        f->i20 = v;
        f->i1c = CriFaderLinearToValue(f, v);
        if ((f->i4 > 0 && f->i1c >= f->i24) || (f->i4 < 0 && f->i1c <= f->i24) || f->i4 == 0) {
            CriFaderSetValue(f, f->i24);
            f->ic = f->i8;
            st = 3;
            f->st = 3;
        }
    }
    if (st == 3) {
        if (f->i8 - f->ic >= f->i14) {
            f->st = 0;
            CriFaderCallEndCallback(f);
        }
    }
}

void CriFaderSetRampStartCallback(CriFader *f, void (*cb)(CriFadeGrp *, int), CriFadeGrp *a, int b)
{
    f->cb1 = cb;
    f->a1 = a;
    f->b1 = b;
}

void CriFaderSetEndCallback(CriFader *f, void (*cb)(CriFadeGrp *, int), CriFadeGrp *a, int b)
{
    f->cb2 = cb;
    f->a2 = a;
    f->b2 = b;
}

void CriFaderReset(CriFader *f)
{
    unsigned char *p;
    f->st = 0;
    f->i4 = 0;
    f->i8 = 0;
    f->ic = 0;
    f->i10 = 0;
    f->i14 = 0;
    f->curve = 0;
    f->i1c = -960;
    f->i20 = 0;
    f->i24 = -960;
    f->i28 = -960;
    p = (unsigned char *) &f->cb1;
    p[0] = 0;
    p[1] = 0;
    p[2] = 0;
    p[3] = 0;
    p[4] = 0;
    p[5] = 0;
    p[6] = 0;
    p[7] = 0;
    p[8] = 0;
    p[9] = 0;
    p[10] = 0;
    p[11] = 0;
    p[12] = 0;
    p[13] = 0;
    p[14] = 0;
    p[15] = 0;
    p[16] = 0;
    p[17] = 0;
    p[18] = 0;
    p[19] = 0;
    p[20] = 0;
    p[21] = 0;
    p[22] = 0;
    p[23] = 0;
}

int CriFaderComputeIncrement(CriFader *f, int a, int b, int c)
{
    if (c <= 0) {
        return 0x316938;
    }
    return (CriFaderValueToLinear(f, b) - CriFaderValueToLinear(f, a)) / c;
}

int CriFaderLinearToValue(CriFader *f, int x)
{
    float t;
    float s;
    if (x == 0) {
        return -960;
    }
    if (x == 0x316938) {
        return 0;
    }
    switch (f->curve) {
    case 0:
    default:
        t = (float) (200.0 * log10((double) ((float) x / 3238200.0f)));
        break;
    case 1:
        t = -960.0f + 960.0f * ((float) x / 3238200.0f);
        break;
    case 2:
        s = (float) sin((double) (1.5707964f * (float) x / 3238200.0f));
        t = (float) (200.0 * log10((double) (3238200.0f * s / 3238200.0f)));
        break;
    case 3:
        s = (float) cos((double) (1.5707964f * (float) x / 3238200.0f));
        t = (float) (200.0 * log10((double) (3238200.0f * (1.0f - s) / 3238200.0f)));
        break;
    case 4:
        s = (float) log10(960.0);
        t = 0.0f - (float) pow(10.0, (double) (s * (float) (0x316938 - x) / 3238200.0f));
        break;

    }
    if (!(t > -960.0f)) {
        t = -960.0f;
    }
    if (!(t < 0.0f)) {
        t = 0.0f;
    }
    return (int) t;
}

int CriFaderValueToLinear(CriFader *f, int x)
{
    float t;
    float q;
    float L;
    if (x == 0) {
        return 0x316938;
    }
    if (x == -960) {
        return 0;
    }
    switch (f->curve) {
    case 0:
    default:
        t = 3238200.0f * (float) pow(10.0, (double) ((float) x / 200.0f));
        break;
    case 1:
        t = 3238200.0f * ((float) (x + 960) / 960.0f);
        break;
    case 2:
        t = 3238200.0f * (float) pow(10.0, (double) ((float) x / 200.0f));
        q = t / 3238200.0f;
        t = 3238200.0f * ((float) asin((double) q) / 1.5707964f);
        break;
    case 3:
        t = 3238200.0f * (float) pow(10.0, (double) ((float) x / 200.0f));
        q = t / 3238200.0f;
        t = 3238200.0f * ((float) acos((double) (1.0f - q)) / 1.5707964f);
        break;
    case 4:
        L = (float) log10(960.0);
        t = 3238200.0f * (float) log10((double) -x) / L;
        break;
    }
    if (!(t > 0.0f)) {
        t = 0.0f;
    }
    if (!(t < 3238200.0f)) {
        t = 3238200.0f;
    }
    return (int) t;
}

int CriFaderCallRampStartCallback(CriFader *f)
{
    if (f->cb1 == 0) {
        return 0;
    }
    f->cb1((CriFadeGrp *) f->a1, f->b1);
    return 1;
}

int CriFaderCallEndCallback(CriFader *f)
{
    if (f->cb2 == 0) {
        return 0;
    }
    f->cb2((CriFadeGrp *) f->a2, f->b2);
    return 1;
}
