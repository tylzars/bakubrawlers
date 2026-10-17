/* CRI stream control, 0x02041f50-0x02042760 */
typedef struct StmIO { struct StmIOVt *vt; } StmIO;
struct StmIOVt { int v[6]; void (*get18)(StmIO *, int, int, void *); void (*put1c)(StmIO *, int, void *);
                 void (*put20)(StmIO *, int, void *); int (*get24)(StmIO *, int); };
typedef struct Span { int p; int n; } Span;
typedef struct Stm {
    signed char used, state, s2, s3;
    StmIO *io;
    void *h8;
    int c;
    long long size;
    int sectors;
    int i1c, i20, i24;
    Span span;
    int i30, i34;
    int i38;
    void (*cb)(int);
    int cbarg;
    int i44;
    signed char b48, b49, b4a, b4b, b4c, b4d;
    short pad4e;
    int i50, i54, i58, i5c;
    unsigned i60;
} Stm;

typedef struct StmGlob { int pad0; int i4; int idxA; int refs; int i10; int i14; } StmGlob;

extern StmGlob data_022163d8;
extern int data_022163dc;
extern Stm data_022163f0[10];
extern char data_020acb98[], data_020acbc4[];

extern void func_0203eda4(void), func_0203edb0(void), func_0203edbc(void), func_0203edc0(void);
extern int CriTrySetFlag(void *);
extern void func_02046e7c(void);
extern int func_02046eb4(void *);
extern int func_020470e0(void *);
extern void CriSplitSpan(void *, int, void *, void *);
extern void CriDevDeallocHandle(void *);
extern void *CriDevAllocHandle(int, int, int);
extern int func_02046fc8(void *);
extern int func_02046f0c(int);
extern int func_02046dcc(void *, int, int);
extern int func_02046d74(void *);
extern int func_02046e24(void *, int, int);
extern void SendErrConcat(const char *, int);
extern void func_02041d7c(Stm *, int);
extern void func_02041f84(void);
extern void func_02042394(Stm *);
extern void func_02041f9c(Stm *);
extern void StmLockEnter(void), StmLockLeave(void), func_02042748(void), func_02042754(void);
extern int func_0204273c(void *);
extern int func_02042714(Stm *, int, int);
extern void func_02042660(void);
extern void func_020426d0(void);

static void func_02041f74(Stm *s, int v);
void func_02041f50(Stm *s, int v)
{
    func_02042748();
    func_02041f74(s, v);
    func_02042754();
}

static void func_02041f74(Stm *s, int v)
{
    if (v < 0) v = s->sectors;
    s->i34 = v;
}

void func_02041f84(void)
{
    data_022163d8.i14++;
}

static inline int tail(Stm *s)
{
    int cfg = data_022163d8.i10;
    if (cfg >= 0 && s->i50 >= cfg) {
        s->state = 4;
        return 1;
    }
    if (s->i50 < 0x7fffffff) s->i50++;
    return 0;
}

void func_02041f9c(Stm *s)
{
    void *h = s->h8;
    StmIO *io = s->io;
    int st = func_02046eb4(h);
    StmLockEnter();
    if (s->s2 == 1) {
        if (st == 1) {
            int n;
            int sec;
            Span a;
            Span c;
            Span b;
            s->s2 = 0;
            StmLockLeave();
            n = s->i24;
            CriSplitSpan(&s->span, n << 11, &c, &b);
            io->vt->put20(io, 1, &c);
            io->vt->put1c(io, 0, &b);
            s->i5c += s->i24;
            s->i38 += n << 11;
            s->span.p = 0;
            s->span.n = 0;
            sec = s->sectors;
            if (s->i5c == s->i34 && s->cb) s->cb(s->cbarg);
            if (s->i5c >= sec) {
                s->state = 3;
            } else if (!(s->i60 > (unsigned)s->i38 >> 11)) {
                if (s->i60 < 0xfffff) s->state = 3;
            }
            s->i50 = 0;
        } else if (st == 3) {
            s->s2 = 0;
            StmLockLeave();
            io->vt->put1c(io, 0, &s->span);
            s->span.p = 0;
            s->span.n = 0;
            if (func_020470e0(s->h8) == 0) s->state = 4;
            else tail(s);
        } else {
            StmLockLeave();
        }
    } else {
        StmLockLeave();
    }
    if (s->state == 4) return;
    StmLockEnter();
    if (s->s2 == 0) {
    s->s2 = 1;
    s->span.p = 0;
    s->span.n = 0;
    StmLockLeave();
    if (s->b48 == 1 || s->b4c == 1) {
        s->s2 = 0;
        return;
    }
    if (s->sectors == 0) {
        s->s2 = 0;
        s->i24 = 0;
        s->state = 3;
        return;
    }
    if (io == 0 || io->vt == 0) {
        s->s2 = 0;
        func_02041f84();
        return;
    }
    {
        int r = io->vt->get24(io, 0);
        int n;
        Span a;
        if (s->i44 - r >= s->i20) {
            s->s2 = 0;
            return;
        }
        io->vt->get18(io, 0, s->i1c, &a);
        n = a.n / 2048;
        if (s->i34 - s->i5c <= n) n = s->i34 - s->i5c;
        if (n >= s->sectors - s->i5c) n = s->sectors - s->i5c;
        if (n >= s->i30) n = s->i30;
        func_02046dcc(s->h8, s->c + s->i5c, 0);
        if (s->i60 != 0xfffff) {
            int x = s->i60 - s->i38 / 2048;
            if (n >= x) n = x;
        }
        s->i24 = func_02046e24(s->h8, n, a.p);
        s->span.p = a.p;
        s->span.n = a.n;
        if (s->i24 > 0) return;
        io->vt->put1c(io, 0, &s->span);
        s->span.p = 0;
        s->span.n = 0;
        s->s2 = 0;
        if (func_02046eb4(s->h8) != 3) return;
        if (func_020470e0(s->h8) == 0) {
            s->state = 4;
            return;
        }
        tail(s);
    }
    } else {
        StmLockLeave();
    }
}

void func_02042394(Stm *s)
{
    if (s->s2 == 0) {
        if (s->b4c == 1) {
            s->b4c = 0;
            if (s->b4b == 0) s->state = 1;
        }
        if (s->b4a == 1) {
            if (s->h8) {
                void *h = s->h8;
                s->h8 = 0;
                CriDevDeallocHandle(h);
            }
            StmLockEnter();
            s->b4a = 0;
            s->b4d = 0;
            StmLockLeave();
        }
        {
        int fl;
        StmLockEnter();
        fl = 0;
        if (s->b49 == 1) {
            if (s->b49 == 1 && s->b4a == 1) return;
            if (s->b4d == 0) {
                s->b4d = 1;
                StmLockLeave();
                fl = 1;
                if (s->h8 == 0) {
                    s->h8 = CriDevAllocHandle(s->i54, s->i58, 0);
                    if (s->h8 == 0) {
                        SendErrConcat(data_020acb98, s->i54);
                        s->state = 4;
                        s->b4d = 0;
                        s->b49 = 0;
                        return;
                    }
                }
            }
            if (s->b4d == 1) {
                long long x;
                int nsec;
                if (fl == 0) StmLockLeave();
                if (s->b49 == 1 && s->b4a == 1) return;
                if (s->i58 == 0) {
                    x = func_02046fc8(s->h8);
                    if (x < 0) x = func_02046f0c(s->i54);
                    nsec = (x + 0x7ff) / 0x800;
                } else {
                    func_02046dcc(s->h8, 0, 2);
                    nsec = func_02046d74(s->h8);
                    x = nsec << 11;
                    func_02046dcc(s->h8, 0, 0);
                }
                if (s->size == 0x7ffff800LL) {
                    s->size = x;
                    s->sectors = nsec;
                }
                if (s->c > nsec) s->c = nsec;
                if (s->sectors + s->c > nsec) {
                    int d = nsec - s->c;
                    s->sectors = d;
                    s->size = (long long)d << 11;
                }
                func_02041d7c(s, 0);
                s->b49 = 0;
                if (func_02046eb4(s->h8) == 3) {
                    SendErrConcat(data_020acbc4, s->i54);
                    if (s->h8) {
                        void *h = s->h8;
                        s->h8 = 0;
                        CriDevDeallocHandle(h);
                    }
                    s->state = 4;
                    s->b4d = 0;
                    s->b49 = 0;
                    return;
                }
            }
        } else {
            StmLockLeave();
        }
        }
        if (s->b4b == 1) s->b4b = 0;
    }
    if (s->state == 2 && s->b4d == 1 && s->b49 == 0) func_02041f9c(s);
}

void func_0204264c(void)
{
    func_02042748();
    func_02042660();
    func_02042754();
}

void func_02042660(void)
{
    int i;
    Stm *p;
    if (func_0204273c(&data_022163dc) == 0) return;
    p = data_022163f0;
    for (i = 0; i < 10; i++, p++) {
        if (p->used == 1) func_02042394(p);
    }
    data_022163d8.i4 = 0;
}

void func_020426bc(void)
{
    func_02042748();
    func_020426d0();
    func_02042754();
}

void func_020426d0(void) { func_02046e7c(); }
void func_020426dc(void) { }

int func_020426e0(Stm *s, int a, int b)
{
    int r;
    func_02042748();
    r = func_02042714(s, a, b);
    func_02042754();
    return r;
}

int func_02042714(Stm *s, int a, int b)
{
    s->i20 = a;
    s->i1c = b;
    return 1;
}

/* The shared CRI lock: func_0203eda4 / func_0203edb0 are CriLockEnter1 / CriLockLeave1 (cri_adx_buf.c). */
void StmLockEnter(void) { func_0203eda4(); }
void StmLockLeave(void) { func_0203edb0(); }
int func_0204273c(void *p) { return CriTrySetFlag(p); }
void func_02042748(void) { func_0203edbc(); }
void func_02042754(void) { func_0203edc0(); }
