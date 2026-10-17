/* CRI middleware, 0x02045f90-0x02046924 */
typedef struct CriSpan { char *ptr; int len; } CriSpan;
typedef void (*CriCb)(void *);
typedef struct CriCbPair { CriCb fn; void *arg; } CriCbPair;
typedef void (*CriCb2)(void *, void *);
typedef struct CriCbPair2 { CriCb2 fn; void *arg; } CriCbPair2;
typedef struct CriLockSt { void *volatile owner; volatile int cnt; volatile int nest; } CriLockSt;
typedef struct CriErrEnt { void *fn; void *arg; void *name; } CriErrEnt;   /* 0xc */
typedef struct CriErrTab { CriErrEnt e[6]; } CriErrTab;                   /* 0x48 */
typedef struct CriNameEnt { void *val; char name[12]; } CriNameEnt;       /* 0x10 */
typedef struct CriVA { void *fn; void (*cb)(void *, void *, int); void *arg; } CriVA;

extern int g_pool3Count;           /* 0x022170ec */
extern int g_pool3[0x90];        /* 0x022170f0 */
extern CriLockSt g_lockSt;         /* 0x02217330 */
extern int data_0221733c[2], data_02217344[2];
extern int (*data_0221734c)(int *);
extern CriCbPair2 data_02217350;
extern CriErrTab data_02217398[8];
extern int data_022175d8[8], data_022175f8[8];
extern CriCbPair data_02217618, data_02217620;
extern char data_02217628[0x80];
extern CriVA data_022176a8;
extern char data_022176b0[];
extern CriNameEnt data_022176b4[32];
extern char data_020ad7d0[], data_020ad818[], data_020ad840[], data_020ad848[];
extern char data_020ad878[], data_020ad898[], data_020ad8c0[], data_020ad8e4[];
extern char data_020ad90c[], data_020ad944[], data_020ad94c[], data_020ad970[], data_020ad994[];

extern void func_02012464(void *, const void *, int);
extern void func_02012484(void *, int, int);
extern int func_02015a50(const char *);
extern int func_02015c04(const char *, const char *, int);
extern void CriFormatStringV(char *, int, const char *, char *);
extern void CriCopyStringN(char *, int, const char *, int);
extern void func_02045f38(void);
extern void func_02045f60(void);
extern void func_02045f6c(void);
extern void CriLockEnter1(void);
extern void func_02046000(void);
extern void CriLockEnter(void *);
extern void CriLockLeave(void *);
extern void func_020465fc(void);
extern void CriLockLeave1(void);

void CriSendFormattedText(const char *fmt, ...);
void CriSendText(const char *msg);
int CriFuncTableAddNoLock(int a, void *b, void *c, void *d);
void CriFuncTableRemoveNoLock(int a, int b);
void CriFuncTableSetNoLock(int a, int b, void *c, void *d, void *e);
int CriDevTableAdd(const char *s, void *(*f)(void));
void *CriDevTableFind(const char *s);
void CriStrToUpper(char *s);
void CriDevSendText(char *a);
void CriLockEnter(void *);
void CriLockLeave(void *);
void func_020465fc(void);

void func_02045f90(void)
{
    if (g_pool3Count == 0) {
        __clear(g_pool3, sizeof(g_pool3));
    }
    g_pool3Count++;
}

void func_02045fe8(void)
{
    func_02045f60();
    func_02046000();
    func_02045f6c();
    func_02045f38();
}

void func_02046000(void)
{
    if (--g_pool3Count == 0) {
        __clear(g_pool3, sizeof(g_pool3));
    }
}

void CriSplitSpan(CriSpan *in, int limit, CriSpan *a, CriSpan *b)
{
    int len = in->len;
    char *p = in->ptr;
    a->ptr = p;
    a->len = len;
    b->len = len;
    if (a->len > limit) {
        a->len = limit;
    }
    b->len = b->len - a->len;
    if (b->len == 0) {
        b->ptr = 0;
    } else {
        b->ptr = a->ptr + a->len;
    }
}

void CriLockEnter(void *x)
{
    if (data_02217620.fn == 0) return;
    data_02217620.fn(data_02217620.arg);
    if (g_lockSt.nest == 0) {
        g_lockSt.owner = x;
    }
    g_lockSt.nest++;
}

void CriLockLeave(void *x)
{
    if (data_02217618.fn == 0) return;
    g_lockSt.nest--;
    if (g_lockSt.nest == 0) {
        if (g_lockSt.owner != x) {
            CriSendFormattedText(data_020ad7d0, g_lockSt.owner, x);
        }
        g_lockSt.owner = 0;
    }
    data_02217618.fn(data_02217618.arg);
}

void CriLockEnter1(void) { CriLockEnter((void *)1); }
void CriLockLeave1(void) { CriLockLeave((void *)1); }
void CriLockEnter3(void) { CriLockEnter((void *)3); }
void CriLockLeave3(void) { CriLockLeave((void *)3); }

void CriSendFormattedText(const char *fmt, ...)
{
    char *ap = (char *)(((unsigned)&fmt & ~3u) + 4);
    func_02012484(data_02217628, 0, 0x80);
    CriFormatStringV(data_02217628, 0x80, fmt, ap);
    if (data_02217350.fn) {
        data_02217350.fn(data_02217350.arg, data_02217628);
    }
}

void CriSendText(const char *msg)
{
    if (msg == 0) {
        data_02217350.fn(data_02217350.arg, 0);
        return;
    }
    CriCopyStringN(data_02217628, 0x80, msg, 0x7f);
    if (data_02217350.fn) {
        data_02217350.fn(data_02217350.arg, data_02217628);
    }
}

int CriFuncTableAdd(int a, void *b, void *c, void *d)
{
    int r;
    CriLockEnter1();
    r = CriFuncTableAddNoLock(a, b, c, d);
    CriLockLeave1();
    return r;
}

int CriFuncTableAddNoLock(int a, void *b, void *c, void *d)
{
    CriErrEnt *e;
    int i;
    if (a < 0 || a >= 8) {
        CriSendText(data_020ad818);
        return -1;
    }
    e = data_02217398[a].e;
    i = 0;
    do {
        if (e->fn == 0) {
            e->fn = b;
            e->arg = c;
            if (d != 0) {
                e->name = d;
            } else {
                e->name = data_020ad840;
            }
            break;
        }
        i++;
        e++;
    } while (i < 6);
    if (i == 6) {
        CriSendText(data_020ad848);
        return -1;
    }
    return i;
}

void CriFuncTableRemove(int a, int b)
{
    CriLockEnter1();
    CriFuncTableRemoveNoLock(a, b);
    CriLockLeave1();
}

void CriFuncTableRemoveNoLock(int a, int b)
{
    if (b < 0 || b >= 6) {
        CriSendText(data_020ad878);
        return;
    }
    if (a < 0 || a >= 8) {
        CriSendText(data_020ad898);
        return;
    }
    data_02217398[a].e[b].fn = 0;
    data_02217398[a].e[b].arg = 0;
    data_02217398[a].e[b].name = 0;
}

void CriFuncTableSet(int a, int b, void *c, void *d, void *e)
{
    CriLockEnter1();
    CriFuncTableSetNoLock(a, b, c, d, e);
    CriLockLeave1();
}

void CriFuncTableSetNoLock(int a, int b, void *c, void *d, void *e)
{
    CriErrEnt *p;
    if (b < 0 || b >= 6) {
        CriSendText(data_020ad8c0);
        return;
    }
    if (a < 0 || a >= 8) {
        CriSendText(data_020ad8e4);
        return;
    }
    p = &data_02217398[a].e[b];
    if (p->fn != 0) {
        CriSendText(data_020ad90c);
    }
    p->fn = c;
    p->arg = d;
    if (e != 0) {
        p->name = e;
    } else {
        p->name = data_020ad944;
    }
}

void CriSetTextCallback(CriCb2 f, void *arg)
{
    CriLockEnter1();
    data_02217350.fn = f;
    data_02217350.arg = arg;
    CriLockLeave1();
}

void CriSetLockEnterHook(CriCb f, void *arg)
{
    data_02217620.fn = f;
    data_02217620.arg = arg;
}

void CriSetLockLeaveHook(CriCb f, void *arg)
{
    data_02217618.fn = f;
    data_02217618.arg = arg;
}

int CriFuncTableRun(int n)
{
    int i;
    int r = 0;
    CriErrEnt *e = data_02217398[n].e;
    for (i = 0; i < 6; i++, e++) {
        int (*fn)(void *) = e->fn;
        void *arg = e->arg;
        if (fn != 0) {
            data_022175d8[n] = 1;
            r |= fn(arg);
            data_022175d8[n] = 0;
        }
    }
    data_022175f8[n]++;
    return r;
}

int CriFuncTableRun0(void) { return CriFuncTableRun(0); }
int CriFuncTableRun1(void) { return CriFuncTableRun(1); }
int CriFuncTableRun2(void) { return CriFuncTableRun(2); }
int CriFuncTableRun3(void) { return CriFuncTableRun(3); }
int CriFuncTableRun4(void) { return CriFuncTableRun(4); }
int CriFuncTableRun5(void) { return CriFuncTableRun(5); }
int CriFuncTableRun6(void) { return CriFuncTableRun(6); }
int CriFuncTableRun7(void) { return CriFuncTableRun(7); }

void func_020465fc(void)
{
    int i;
    __clear(data_022175d8, sizeof(data_022175d8));
    __clear(&data_02217620, 8);
    __clear(&data_02217618, 8);
    __clear(data_02217344, 8);
    __clear(data_0221733c, 8);
    for (i = 0; i < 6; i++) {
        data_022175f8[i] = 0;
    }
    data_0221734c = 0;
}

void func_0204668c(void)
{
    if (g_lockSt.cnt == 0) {
        func_020465fc();
    }
    g_lockSt.cnt++;
}

void func_020466bc(void)
{
    g_lockSt.cnt--;
    if (g_lockSt.cnt != 0) return;
    func_020465fc();
    __clear(&data_02217350, sizeof(data_02217350));
}

int CriTrySetFlag(int *p)
{
    int r;
    if (data_0221734c != 0) {
        r = data_0221734c(p);
    } else {
        CriLockEnter1();
        if (*p != 1) {
            r = 1;
        } else {
            r = 0;
        }
        *p = 1;
        CriLockLeave1();
    }
    return r;
}

void CriDevCallCallback(void *a, void *b, int c)
{
    if (data_022176a8.cb == 0) return;
    data_022176a8.cb(data_022176a8.arg, b, c);
}

void CriDevSendText(char *a)
{
    CriDevCallCallback(data_022176b0, a, 0);
}

void CriDevTableAddAndSetCallback(const char *a, void *(*f)(void))
{
    void **r;
    if (a == 0) {
        CriDevSendText(data_020ad94c);
        return;
    }
    if (f == 0) {
        CriDevSendText(data_020ad970);
        return;
    }
    r = (void **)CriDevTableAdd(a, f);
    if (r == 0) {
        CriDevSendText(data_020ad994);
        return;
    }
    if (r[1] == 0) return;
    ((void (*)(void *, void *))r[1])(CriDevCallCallback, 0);
}

int CriDevTableAdd(const char *s, void *(*f)(void))
{
    int i;
    void *v;
    CriStrToUpper((char *)s);
    v = f();
    if (CriDevTableFind(s) != 0) {
        return (int)v;
    }
    for (i = 0; i < 32; i++) {
        if (data_022176b4[i].name[0] == 0) break;
    }
    if (i == 32) return 0;
    data_022176b4[i].val = v;
    func_02012464(data_022176b4[i].name, s, func_02015a50(s) + 1);
    return (int)v;
}

void *CriDevTableFind(const char *s)
{
    unsigned i;
    CriNameEnt *p;
    int n = func_02015a50(s);
    p = data_022176b4;
    for (i = 0; i < 32; i++, p++) {
        if (func_02015c04(s, p->name, n) == 0) {
            return data_022176b4[i].val;
        }
    }
    return 0;
}

void CriStrToUpper(char *s)
{
    unsigned n = func_02015a50(s) + 1;
    unsigned i = 0;
    if (n == 0) return;
    do {
        if (s[i] >= 'a' && s[i] <= 'z') {
            s[i] -= 0x20;
        }
        i++;
    } while (i < n);
}
