/* CRI ADX handle pool and work-size calc, 0x0204ecf0-0x0204f3c8 */
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

extern void SendErr(const char *);
extern void CriInit(void);
extern void CriExit(void);
extern void CriFuncTableSet(int, int, void *, void *, void *);
extern void CriFuncTableRemove(int, int);
extern void CriLockEnter3(void), CriLockLeave3(void);
extern void *CriXCreate(void *, int);
extern int func_02051124(void);
extern void func_02051110(int);
extern void func_020511a4(int);
extern int func_020511d4(int), func_020511dc(int), func_020511e4(int);
extern void func_020511f4(int, void *);
extern void func_020511fc(int);
extern void func_020511c8(void);
extern void *memset(void *, int, int);
extern VObj *func_02050e60(void *, int);
extern void func_02050ea8(void *);
extern void *CriFaderCreate(void *, int);
extern void func_020506e0(void *);
extern int CriFaderGetState(void *);
extern void func_0204fc7c(Obj *), CriFadeGrpAdvance(Obj *), func_0204fc64(Obj *, int);
extern void CriFadeGrpReset();
extern void CriCopyStringN(void *, int, int, int);
extern void func_0204fe20(Obj *, void *);
extern void func_0205012c(Obj *, Ent *, int, int);
extern void func_02050268(Obj *, Ent *, int, int, int);
extern void func_0204bdfc(void *, void *, int);
extern void func_0204bf1c(void *);
extern void *func_0204fc00(Obj *, int);

extern int PoolHasRef(Pool *);
extern void PoolClear(Pool *);
extern void SendErrWrapper(const char *);
extern void PoolReleaseRef(Pool *);
extern int func_0204f338(Pool *);
extern int PoolObjInit(Obj *, Cfg *, char *, int);
extern void PoolObjReleaseParts(Obj *);
extern void PoolDeallocObj(Pool *, Obj *);
extern Obj *PoolObjCreate(char *, unsigned);
extern void func_0204f3b4(Obj *);
extern void PoolAdvanceObj(Pool *, Obj *);
extern int PoolObjIsInit(Obj *);
extern int func_0204f98c(Obj *);
extern void PoolAdvance(Pool *);
extern void func_0204ed44(void), func_0204ed48(void), func_0204ed10(void);
extern void func_0204ed2c(void), func_0204ed38(void);
extern int func_0204f2e4(Pool *);
extern void *PoolObjGetAdx(Obj *, int);
extern int PoolObjGetAdxCount(Obj *);

int func_0204ecf0(Obj *a) { return PoolObjGetAdxCount(a); }

void func_0204ecfc(void)
{
    func_0204ed44();
    func_0204ed10();
    func_0204ed48();
}

void func_0204ed10(void)
{
    func_0204f2e4(data_020afa5c);
    func_020511c8();
}

void func_0204ed2c(void) { CriLockEnter3(); }
void func_0204ed38(void) { CriLockLeave3(); }
void func_0204ed44(void) { }
void func_0204ed48(void) { }

int PoolAddRef(Pool *p)
{
    if (PoolHasRef(p) == 0 && p->i78 == 0) {
        PoolClear(p);
        p->i78 = 1;
        CriInit();
        CriFuncTableSet(2, 4, func_0204f338, p, data_020afa94);
        p->h = (int) CriXCreate(p->buf, 0x60);
        if (func_02051124() == 0) {
            SendErrWrapper(data_020afaa4);
            PoolReleaseRef(p);
            return 0;
        }
        func_020511f4(p->h, p);
        p->i78 = 0;
    }
    p->cnt++;
    return 1;
}

void PoolReleaseRef(Pool *p)
{
    int i;
    if (p->i78 == 0) {
        p->cnt--;
    }
    if (p->cnt < 0) {
        SendErrWrapper(data_020afad8);
        return;
    }
    if (p->cnt != 0 && p->i78 != 1) {
        return;
    }
    for (i = 0; i < 4; i++) {
        PoolDeallocObj(p, p->slot[i]);
    }
    if (p->h != 0) {
        func_020511a4(p->h);
        func_02051110(p->h);
        p->h = 0;
    }
    CriFuncTableRemove(2, 4);
    CriFuncTableRemove(5, p->i7c);
    CriExit();
    p->i78 = 0;
}

Obj *PoolAllocObj(Pool *p, Cfg *cfg, char *buf, int size)
{
    int i;
    Obj *o = 0;
    if (PoolHasRef(p) == 0) {
        SendErrWrapper(data_020afb10);
        return 0;
    }
    for (i = 0; i < 4; i++) {
        if (p->slot[i] == 0) {
            o = p->slot[i] = PoolObjCreate(buf, size);
            buf += 0xe0;
            size -= 0xe0;
            break;
        }
    }
    if (i == 4) {
        SendErrWrapper(data_020afb40);
        PoolDeallocObj(p, o);
        return 0;
    }
    if (PoolObjInit(o, cfg, buf, size) != 0) {
        return o;
    }
    SendErrWrapper(data_020afb7c);
    PoolDeallocObj(p, o);
    return 0;
}

void PoolDeallocObj(Pool *p, Obj *o)
{
    int i;
    if (o == 0) return;
    PoolObjReleaseParts(o);
    for (i = 0; i < 4; i++) {
        if (p->slot[i] == o) {
            func_0204f3b4(p->slot[i]);
            p->slot[i] = 0;
            return;
        }
    }
}

int PoolComputeWorkSize(int unused, Cfg *c)
{
    int base = 0;
    int r0 = 0;
    int has = c->i1c;
    r0 += 0xe0;
    if (has != 0) {
        base += 0x10;
        if (c->i24 != 0) {
            base += c->wc * (((((c->w8 / 1000) * 2500) / 16 * (c->w0 + 1) + 0x800) / 0x800) * 0x800 + 0x24 + 0x28c0u) + 0x40;
        } else {
            base += c->wc * (((((c->w8 / 1000) * (c->w4 * 2500)) / 16 * (c->w0 + 1) + 0x800) / 0x800) * 0x800 + 0x24 + ((c->w4 * 0x1460) << 1)) + 0x40;
        }
    } else {
        base += 0xc;
        if (c->i24 != 0) {
            base += c->w10 * (((((c->w8 / 1000) * 2500) / 16 * (c->w0 + 1) + 0x800) / 0x800) * 0x800 + 0x24) + 0x2900;
        } else {
            base += c->w10 * (((((c->w8 / 1000) * (c->w4 * 2500)) / 16 * (c->w0 + 1) + 0x800) / 0x800) * 0x800 + 0x24) + ((c->w4 * 0x1460) << 1) + 0x40;
        }
    }
    if (c->i20 != 0) base += 0x1800;
    base += 0x44;
    if (c->i18 != 0) base <<= 1;
    r0 += base;
    r0 += 0x100;
    if (has != 0) r0 += c->wc * 4;
    return r0;
}

void PoolAdvance(Pool *p)
{
    int i;
    if (PoolHasRef(p) == 0) return;
    if (p->cb80 != 0) {
        p->cb80(p->a84);
    }
    if (func_020511d4(p->h) != 0) {
        func_020511fc(p->h);
    }
    for (i = 0; i < 4; i++) {
        if (p->slot[i] != 0) {
            PoolAdvanceObj(p, p->slot[i]);
        }
    }
    if (p->cb88 != 0) {
        p->cb88(p->a8c);
    }
}

void PoolAdvanceObj(Pool *p, Obj *o)
{
    if (PoolHasRef(p) == 0) return;
    if (PoolObjIsInit(o) == 0) return;
    func_0204fc7c(o);
    if (func_020511d4(p->h) != 0 && func_020511dc(p->h) != 0 && func_0204f98c(o) == 1) {
        func_0204fc64(o, func_020511e4(p->h));
    }
    CriFadeGrpAdvance(o);
}

int func_0204f2e4(Pool *p) { return p->h; }

/* Zeroes every member except h and buf. */
void PoolClear(Pool *p)
{
    int i;
    for (i = 0; i < 4; i++) {
        p->slot[i] = 0;
    }
    p->cnt = 0;
    p->i78 = 0;
    p->i7c = 0;
    p->cb80 = 0;
    p->a84 = 0;
    p->cb88 = 0;
    p->a8c = 0;
}

int PoolHasRef(Pool *p)
{
    if (p->cnt > 0) return 1;
    return 0;
}

int func_0204f338(Pool *p)
{
    if (p == 0) return 0;
    func_0204ed44();
    PoolAdvance(p);
    func_0204ed48();
    return 0;
}

void SendErrWrapper(const char *s) { SendErr(s); }

Obj *PoolObjCreate(char *buf, unsigned size)
{
    Obj *o = (Obj *) buf;
    if (o == 0) {
        SendErrWrapper(data_020afbb4);
        return 0;
    }
    if (size < 0xe0) {
        SendErrWrapper(data_020afbe8);
        return 0;
    }
    CriFadeGrpReset(o, size);
    return o;
}

void func_0204f3b4(Obj *o)
{
    if (o == 0) return;
    CriFadeGrpReset(o);
}
