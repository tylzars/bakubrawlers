/* CRI stream objects, 0x02041744-0x02041f50 */
typedef struct StmIO { struct StmIOVt *vt; } StmIO;
struct StmIOVt { int v[9]; int (*get)(StmIO *, int); };
typedef struct Stm {
    signed char used;      /* 0x00 */
    signed char state;     /* 0x01 */
    signed char s2;
    signed char s3;
    StmIO *io;             /* 0x04 */
    void *h8;              /* 0x08 */
    
    int c;                 /* 0x0c */
    long long size;        /* 0x10 */
    int sectors;           /* 0x18 */
    int i1c, i20, i24;
    int i28, i2c, i30, i34, i38, i3c, i40, i44;
    signed char b48, b49, b4a, b4b, b4c, b4d;
    short pad4e;
    int i50, i54, i58, i5c, i60;
} Stm;

typedef struct StmGlob { int pad0; int idxB; int idxA; int refs; } StmGlob;
typedef struct StmCfg { int countB; int startB; int countA; } StmCfg;

extern StmGlob data_022163d8;
extern Stm data_022163f0[10];
extern StmCfg data_020acb8c;
extern int data_02215db4;

extern void StmLockEnter(void);
extern void StmLockLeave(void);
extern void func_02042748(void);
extern void func_02042754(void);
extern void *func_02012484(void *, int, int);
extern void func_0204035c(void);
extern void OS_Sleep(int);
extern int func_0203e6dc(void *), func_0203e6e4(void *), func_0203e6ec(void *), func_0203e6f4(void *);
extern int func_0203e704(void *), func_0203e70c(void *), func_0203e714(void *), func_0203e71c(void *);
extern int func_0203e724(void *, int), SaveCriSlotValues(void *), RestoreCriSlotValues(void *);

int func_02041744(Stm *s) { return func_0203e6dc(s->io); }
int func_02041754(Stm *s) { return func_0203e6e4(s->io); }
int func_02041764(Stm *s) { return func_0203e6ec(s->io); }
int func_02041774(Stm *s) { if (s == 0) return 0; return func_0203e6f4(s->io); }
int func_02041790(Stm *s) { return func_0203e704(s->io); }
int func_020417a0(Stm *s) { return func_0203e70c(s->io); }

int func_020417b0(Stm *s)
{
    if (func_0203e714(s->io) > 0 && (unsigned char)(signed char)(s->state - 2) <= 1)
        return func_0203e71c(s->io);
    return 0;
}

int func_020417f8(Stm *s, int a)
{
    if (func_0203e714(s->io) > 0 && (unsigned char)(signed char)(s->state - 2) <= 1)
        return func_0203e724(s->io, a);
    return -128;
}

void *func_02041848(Stm *s) { return &s->i60; }
int func_02041850(Stm *s) { return SaveCriSlotValues(s->io); }
int func_02041860(Stm *s) { return RestoreCriSlotValues(s->io); }
int func_02041870(Stm *s) { return *(int *)((char *)s + 0xb0); }


int StmPoolAddRef(void)
{
    if (++data_022163d8.refs == 1)
        __clear(data_022163f0, sizeof(data_022163f0));
    return 1;
}

void StmPoolReleaseRef(void)
{
    if (--data_022163d8.refs == 0)
        __clear(data_022163f0, sizeof(data_022163f0));
}

void StmInit(Stm *s, void *h, int c, int size, StmIO *io)
{
    StmLockEnter();
    s->state = 1;
    s->s2 = 0;
    s->io = io;
    s->h8 = h;
    s->c = c;
    s->size = size;
    s->sectors = size / 2048 + (size % 2048 > 0);
    s->i30 = 64;
    s->i5c = 0;
    s->i60 = 0xfffff;
    s->i34 = s->sectors;
    if (s->io != 0) {
        int a = io->vt->get(io, 0);
        int b = io->vt->get(io, 1);
        int t = a + b;
        s->i44 = t;
        s->i1c = t;
        s->i20 = t;
    }
    s->b48 = 0;
    s->used = 1;
    StmLockLeave();
}

Stm *StmAllocFromA(void *a, int b, int c, StmIO *d)
{
    Stm *p = 0;
    int n;
    int cnt = data_020acb8c.countA;
    for (n = 0; n < cnt; n++) {
        p = &data_022163f0[data_022163d8.idxA + n];
        if (p->used == 0) break;
    }
    if (n == cnt) return 0;
    StmInit(p, a, b, c, d);
    p->s3 = 1;
    return p;
}

Stm *StmAllocFromB(void *a, int b, int c, StmIO *d)
{
    Stm *p = 0;
    int n;
    int cnt = data_020acb8c.countB;
    for (n = 0; n < cnt; n++) {
        p = &data_022163f0[data_020acb8c.startB + n];
        if (p->used == 0) break;
    }
    if (n == cnt) return 0;
    StmInit(p, a, b, c, d);
    p->s3 = 0;
    return p;
}

extern int StmStartInner(Stm *);
extern void StmStopAndWait(Stm *);
extern void func_02041ce0(Stm *);
extern void StmStop(Stm *);
extern void func_02041c94(Stm *);
extern void func_02041cb0(Stm *);
extern void func_02041cfc(Stm *);
extern void func_02041dc0(Stm *);
extern void StmStopInner(Stm *);
extern void StmStopAndWaitInner(Stm *);
extern Stm *StmAllocInner(int, int);
extern void StmDeallocInner(Stm *);
extern void func_02041c28(Stm *, int, int, int, long long);
extern int StmGetStateInner(Stm *);
extern int func_02041da8(Stm *, int);
extern void func_02041f44(Stm *, int, int);

Stm *StmAlloc(int a, int n)
{
    Stm *r;
    func_02042748();
    r = StmAllocInner(a, n);
    func_02042754();
    return r;
}

Stm *StmAllocInner(int a, int n)
{
    if (n < 256) return StmAllocFromA(0, 0, 0, (StmIO *)a);
    return StmAllocFromB(0, 0, 0, (StmIO *)a);
}

void StmDealloc(Stm *s)
{
    func_02042748();
    StmDeallocInner(s);
    func_02042754();
}

void StmDeallocInner(Stm *s)
{
    if (s == 0) return;
    StmStopAndWait(s);
    func_02041ce0(s);
    s->used = 0;
    func_02012484(s, 0, 100);
}

void func_02041bdc(Stm *s, int a, int b, int c, long long x)
{
    func_02042748();
    func_02041c28(s, a, b, c, x);
    func_02042754();
}

void func_02041c28(Stm *s, int a, int b, int c, long long x)
{
    StmLockEnter();
    s->c = c;
    s->size = x;
    s->sectors = (x + 0x7ff) / 0x800;
    s->i54 = a;
    s->i58 = b;
    s->b49 = 1;
    StmLockLeave();
}

void func_02041c94(Stm *s)
{
    func_02042748();
    func_02041cb0(s);
    func_02042754();
}

void func_02041cb0(Stm *s)
{
    StmStop(s);
    StmLockEnter();
    if (s->b4d == 1) s->b4a = 1;
    s->b49 = 0;
    StmLockLeave();
}

void func_02041ce0(Stm *s)
{
    func_02042748();
    func_02041cfc(s);
    func_02042754();
}

void func_02041cfc(Stm *s)
{
    StmStopAndWait(s);
    func_02041c94(s);
    for (;;) {
        if (s->b4d == 0 && s->b4a == 0) return;
        func_0204035c();
        if (data_02215db4 != 0) OS_Sleep(1);
    }
}

int StmGetState(Stm *s)
{
    int r;
    func_02042748();
    r = StmGetStateInner(s);
    func_02042754();
    return r;
}

int StmGetStateInner(Stm *s) { return s->state; }

int func_02041d7c(Stm *s, int v)
{
    int r;
    func_02042748();
    r = func_02041da8(s, v);
    func_02042754();
    return r;
}

int func_02041da8(Stm *s, int v)
{
    s->i5c = v;
    if (v > s->sectors) s->i5c = s->sectors;
    return s->i5c;
}

void func_02041dc0(Stm *s)
{
    s->i38 = 0;
    s->i50 = 0;
    s->state = (s->sectors == 0) ? 3 : 2;
    s->s2 = 0;
    s->i28 = 0;
    s->i2c = 0;
    s->b4b = 1;
}

int StmStart(Stm *s)
{
    int r;
    func_02042748();
    r = StmStartInner(s);
    func_02042754();
    return r;
}

int StmStartInner(Stm *s)
{
    StmLockEnter();
    func_02041dc0(s);
    s->i60 = 0xfffff;
    StmLockLeave();
    return 1;
}

void StmStop(Stm *s)
{
    func_02042748();
    StmStopInner(s);
    func_02042754();
}

void StmStopInner(Stm *s)
{
    StmLockEnter();
    if (s->state == 2 && s->s2 == 1) {
        s->b4c = 1;
        if (s->b4b == 1) s->b4b = 0;
    } else {
        s->state = 1;
    }
    StmLockLeave();
}

void StmStopAndWait(Stm *s)
{
    func_02042748();
    StmStopAndWaitInner(s);
    func_02042754();
}

void StmStopAndWaitInner(Stm *s)
{
    StmStop(s);
    for (;;) {
        if (s->state == 1 && s->i28 == 0) return;
        func_0204035c();
        if (data_02215db4 != 0) OS_Sleep(1);
    }
}

void func_02041f18(Stm *s, int a, int b)
{
    func_02042748();
    func_02041f44(s, a, b);
    func_02042754();
}

void func_02041f44(Stm *s, int a, int b)
{
    s->i3c = a;
    s->i40 = b;
}
