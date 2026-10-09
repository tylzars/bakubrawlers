/* CRI ADX voice object (create/destroy/control), 0x0204f3c8-0x0204fc00 */
#pragma dont_inline on

typedef struct VT {
    int (*f0)();
    void (*destroy)();
    int pad8[6];
    void (*f20)();
    int (*f24)();
    int (*f28)();
    int pad2c[5];
    void *(*f40)();
} VT;
typedef struct VObj { const VT *vt; } VObj;

typedef struct Cfg {
    int w0, w4, w8, wc, w10;
    signed char b14, b15, b16, b17;
    int i18, i1c, i20, i24;
} Cfg;

typedef struct Ent {
    VObj *o;
    signed char s4, s5, s6, s7;
    void *buf;
} Ent;

typedef struct Obj {
    VObj *slot[2];       /* 0x00 */
    VObj *cur;           /* 0x08 */
    int pad0c;
    int idx;             /* 0x10 */
    int pad14;
    Ent ent[2];          /* 0x18 */
    int i30;             /* 0x30 */
    char *p34;           /* 0x34 */
    int pad38[8];
    int i58, i5c;        /* 0x58 */
    int pad60;
    int i64, i68;        /* 0x64 */
    int pad6c[2];
    char *p74;           /* 0x74 */
    char *p78;           /* 0x78 */
    int pad7c;
    Cfg cfg;             /* 0x80 */
    int cnt;             /* 0xa8 */
    int padac;
    int ib0, ib4, ib8, ibc, ic0, ic4;
    char *pc8;
    int padcc[5];
} Obj;                   /* 0xe0 */

typedef struct Pool {
    Obj *slot[4];        /* 0x00 */
    int h;               /* 0x10 */
    char buf[0x60];      /* 0x14 */
    int cnt;             /* 0x74 */
    int i78;
    int i7c;
    void (*cb80)(int);
    int a84;
    void (*cb88)(int);
    int a8c;
} Pool;

extern Pool *data_020afa5c;
extern char data_020afa94[], data_020afaa4[], data_020afad8[], data_020afb10[], data_020afb40[], data_020afb7c[];
extern char data_020afbb4[], data_020afbe8[], data_020afc1c[], data_020afc4c[], data_020afc84[], data_020afcbc[], data_020afcf8[];
extern char data_020afd3c[], data_020afd70[], data_020afd9c[], data_020afdc8[], data_020afdfc[], data_020afe2c[];
extern char data_020afe5c[], data_020afe8c[], data_020afebc[], data_020afeec[], data_020aff1c[], data_020aff4c[];

extern void func_02040170(const char *);
extern void func_0204048c(void);
extern void func_020405d4(void);
extern void func_020463d8(int, int, void *, void *, void *);
extern void func_02046328(int, int);
extern void func_02046174(void), func_02046184(void);
extern void *func_020510c8(void *, int);
extern int func_02051124(void);
extern void func_02051110(int);
extern void func_020511a4(int);
extern int func_020511d4(int), func_020511dc(int), func_020511e4(int);
extern void func_020511f4(int, void *);
extern void func_020511fc(int);
extern void func_020511c8(void);
extern void *func_02012484(void *, int, int);
extern VObj *func_02050e60(void *, int);
extern void func_02050ea8(void *);
extern void *func_02050698(void *, int);
extern void func_020506e0(void *);
extern int func_020506f4(void *);
extern void func_0204fc7c(Obj *), func_0204fc8c(Obj *), func_0204fc64(Obj *, int);
extern void func_0204fd2c();
extern void func_02051568(void *, int, int, int);
extern void func_0204fe20(Obj *, void *);
extern void func_0205012c(Obj *, Ent *, int, int);
extern void func_02050268(Obj *, Ent *, int, int, int);
extern void func_0204bdfc(void *, void *, int);
extern void func_0204bf1c(void *);
extern void *func_0204fc00(Obj *, int);

extern int func_0204f324(Pool *);
extern void func_0204f2ec(Pool *);
extern void func_0204f360(const char *);
extern void func_0204ee0c(Pool *);
extern int func_0204f338(Pool *);
extern int func_0204f3c8(Obj *, Cfg *, char *, int);
extern void func_0204f734(Obj *);
extern void func_0204ef90(Pool *, Obj *);
extern Obj *func_0204f36c(char *, unsigned);
extern void func_0204f3b4(Obj *);
extern void func_0204f264(Pool *, Obj *);
extern int func_0204f7b8(Obj *);
extern int func_0204f98c(Obj *);
extern void func_0204f1e8(Pool *);
extern void func_0204ed44(void), func_0204ed48(void), func_0204ed10(void);
extern void func_0204ed2c(void), func_0204ed38(void);
extern int func_0204f2e4(Pool *);
extern void *func_0204fb64(Obj *, int);
extern int func_0204fbc8(Obj *);

typedef struct Par { int a, b, c; } Par;

/* non-matching: same code, r4/r5/r6 allocation differs (original: i=r4, off=r5, entry pointer=r6) */
int func_0204f3c8(Obj *o, Cfg *cfg, char *buf, int size)
{
    int off;
    int i;
    Par par;
    int n;
    off = 0;
    if (o->i58 == 1 || o->i5c == 1) {
        func_0204f360(data_020afc1c);
        return 0;
    }
    func_0204fd2c(o);
    o->i5c = 1;
    o->cfg = *cfg;
    __clear(&par, sizeof(Par));
    par.a = cfg->w4;
    if (cfg->i1c == 1) {
        par.b = cfg->wc;
    } else {
        par.b = 1;
    }
    par.c = cfg->i24;
    if (cfg->i18 == 1) {
        o->i64 = 1;
        o->cnt = 2;
    } else {
        o->i64 = 0;
        o->cnt = 1;
    }
    for (i = 0; i < o->cnt; i++) {
        if (cfg->i1c == 0) {
            VObj *v = func_02050e60(buf + off, 0xc);
            o->slot[i] = v;
            if (v == 0) {
                func_0204f360(data_020afc4c);
                func_0204f734(o);
                return 0;
            }
            off += 0xc;
            if (cfg->i24 != 0) {
                n = cfg->w10 * (((((cfg->w8 / 1000) * 2500) / 16 * (cfg->w0 + 1) + 0x800) / 0x800) * 0x800 + 0x24) + 0x2900;
            } else {
                n = cfg->w10 * (((((cfg->w8 / 1000) * (cfg->w4 * 2500)) / 16 * (cfg->w0 + 1) + 0x800) / 0x800) * 0x800 + 0x24) + ((cfg->w4 * 0x1460) << 1) + 0x40;
            }
            if (v->vt->f0(v, &par, buf + off, n) == 0) {
                func_0204f360(data_020afc84);
                func_0204f734(o);
                return 0;
            }
            off += n;
            o->ent[i].o = o->slot[i];
        }
        o->ent[i].buf = func_02050698(buf + off, 0x44);
        if (o->ent[i].buf == 0) {
            func_0204f360(data_020afcbc);
            func_0204f734(o);
            return 0;
        }
        off += 0x44;
    }
    o->idx = 0;
    o->cur = o->ent[o->idx].o;
    if (cfg->i20 == 1) {
        o->p74 = buf + off;
        off += o->cnt * 0x1800;
    }
    o->p78 = o->p34 = buf + off;
    off += 0x100;
    if (cfg->i1c == 1) {
        o->pc8 = buf + off;
        off += cfg->wc * 4;
        func_02012484(o->pc8, 0, cfg->wc * 4);
    }
    if (off > size) {
        func_0204f360(data_020afcf8);
        func_0204f734(o);
        return 0;
    }
    o->i58 = 1;
    o->i5c = 0;
    return 1;
}

void func_0204f734(Obj *o)
{
    int i;
    if (o->i58 != 1 && o->i5c != 1) return;
    o->i58 = 0;
    o->i5c = 0;
    for (i = 0; i < o->cnt; i++) {
        func_020506e0(o->ent[i].buf);
        if (o->slot[i] != 0) {
            o->slot[i]->vt->destroy(o->slot[i]);
            if (o->cfg.i1c == 0) {
                func_02050ea8(o->slot[i]);
            }
        }
    }
}

int func_0204f7b8(Obj *o) { return o->i58; }

void func_0204f7c0(Obj *o, int a)
{
    if (o->i58 == 0) {
        func_0204f360(data_020afd3c);
        return;
    }
    o->i30 = 0;
    func_02051568(o->p34, 0x100, a, 0x100);
    func_0204fe20(o, &o->i30);
}

void func_0204f810(Obj *o)
{
    if (o->i58 == 0) {
        func_0204f360(data_020afd70);
        return;
    }
    if (o->cur->vt->f28(o->cur) == 0) return;
    func_0204ed2c();
    func_0205012c(o, &o->ent[o->idx], 0, 2);
    func_0204ed38();
}

void func_0204f878(Obj *o, int v)
{
    int i;
    if (o->i58 == 0) {
        func_0204f360(data_020afd9c);
        return;
    }
    func_0204ed2c();
    for (i = 0; i < o->cnt; i++) {
        o->ent[i].o->vt->f20(o->ent[i].o, v);
    }
    o->i68 = v;
    func_0204ed38();
}

int func_0204f8ec(Obj *o)
{
    if (o->i58 == 0) {
        func_0204f360(data_020afdc8);
        return 4;
    }
    return o->cur->vt->f24(o->cur);
}

int func_0204f924(Obj *o)
{
    int r;
    if (o->i58 == 0) {
        func_0204f360(data_020afdfc);
        return 4;
    }
    r = o->cur->vt->f28(o->cur);
    if (r == 3) {
        if (func_020506f4(o->ent[o->idx].buf) != 0) r = 2;
    }
    return r;
}

int func_0204f98c(Obj *o) { return o->cfg.b14; }

void func_0204f994(Obj *o, int v)
{
    Ent *base;
    Ent *e;
    if (o->i58 == 0) {
        func_0204f360(data_020afe2c);
        return;
    }
    func_0204ed2c();
    base = o->ent;
    e = &base[o->idx];
    o->ic4 = v;
    if (e->s4 != 1) {
        func_02050268(o, &base[o->idx], 0, 0, v);
    }
    func_0204ed38();
}

void func_0204fa04(Obj *o, int v) { o->ib0 = v; }
void func_0204fa0c(Obj *o, int v) { o->ib4 = v; }
void func_0204fa14(Obj *o, int v) { o->ib8 = v; }
void func_0204fa1c(Obj *o, int v) { o->ibc = v; }
void func_0204fa24(Obj *o, int v) { o->ic0 = v; }

/* the original passes the (unassigned) result of the fc00 branch on; x is uninitialised there */
void func_0204fa2c(Obj *o)
{
    int i;
    void *x;
    int off;
    if (o->i58 == 0) {
        func_0204f360(data_020afe5c);
        return;
    }
    if (o->cfg.i20 == 0) {
        func_0204f360(data_020afe8c);
        return;
    }
    for (i = 0; i < o->cnt; i++) {
        off = i * 0x1800;
        if (o->cfg.i1c == 1) {
            func_0204fc00(o, i);
        } else {
            x = func_0204fb64(o, i);
        }
        func_0204bdfc(x, o->p74 + off, 0x1800);
    }
}

void func_0204fad4(Obj *o)
{
    int i;
    void *x;
    if (o->i58 == 0) {
        func_0204f360(data_020afebc);
        return;
    }
    if (o->cfg.i20 == 0) {
        func_0204f360(data_020afeec);
        return;
    }
    i = 0;
    if (o->cnt <= 0) return;
    for (; i < o->cnt; i++) {
        if (o->cfg.i1c == 1) {
            func_0204fc00(o, i);
        } else {
            x = func_0204fb64(o, i);
        }
        func_0204bf1c(x);
    }
}

void *func_0204fb64(Obj *o, int i)
{
    if (o->i58 == 0) {
        func_0204f360(data_020aff1c);
        return 0;
    }
    if (o->cfg.i1c == 1) return 0;
    if (i < 0 || i >= o->cnt) return 0;
    return o->slot[i]->vt->f40(o->slot[i]);
}

int func_0204fbc8(Obj *o)
{
    if (o->i58 == 0) {
        func_0204f360(data_020aff4c);
        return 0;
    }
    if (o->cfg.i1c == 1) return 0;
    return o->cnt;
}
