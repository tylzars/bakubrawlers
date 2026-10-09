/* CRI middleware, 0x02044a20-0x020450a4 */
extern void *func_02012484(void *, int, int);

/* ---- 0x02044a20-0x02045098: 24-entry ring buffer pool (0x48 bytes) ---- */
typedef struct CriRing {
    void *vt;           /* 0x00 */
    int used;           /* 0x04 */
    void *p8;           /* 0x08 */
    int c, d10;         /* 0x0c, 0x10 */
    int rd, wr;         /* 0x14, 0x18 */
    char *buf;          /* 0x1c */
    unsigned size;      /* 0x20 */
    int extra;          /* 0x24 */
    int tot28, tot2c, tot30, tot34;
    void (*cb)(void *, int);   /* 0x38 */
    void *cbarg;        /* 0x3c */
    int i40, i44;
} CriRing;

typedef struct CriSpan { char *ptr; int len; } CriSpan;

extern int g_ringCount;         /* 0x02216940 */
extern CriRing g_rings[24];     /* 0x02216944 */
extern char vt_020ad02c[], dat_0209e368[];
extern char str_020ad05c[];
extern char s_020ad068[], s_020ad074[], s_020ad090[], s_020ad09c[];
extern char s_020ad0bc[], s_020ad0c8[], s_020ad0e4[], s_020ad0f0[];
extern char s_020ad110[], s_020ad11c[], s_020ad138[], s_020ad144[];
extern char s_020ad164[], s_020ad170[], s_020ad18c[], s_020ad198[];
extern char s_020ad1b8[], s_020ad1c4[], s_020ad1e0[], s_020ad1ec[];
extern char s_020ad20c[], s_020ad218[], s_020ad234[], s_020ad240[], s_020ad260[], s_020ad26c[];

extern void func_0204568c(const char *);
extern void func_02051558(char *, int, const char *);
extern void func_0205157c(char *, int, const char *);
extern void func_02045f10(void);
extern void func_02045f60(void);
extern void func_02045f6c(void);
extern void func_02045f38(void);
extern void func_020450a4(void);
extern void func_02044a88(void);
extern void func_02044af8(void);
extern CriRing *func_02044b78(int, int, int);
extern void func_02044c34(CriRing *);
extern int func_02044cb8(CriRing *);
extern void func_02044d38(CriRing *, void (*)(void *, int), void *);
extern void func_02044da4(CriRing *);
extern int func_02044e44(CriRing *, int);
extern void func_02044f00(CriRing *, int, int, CriSpan *);

void func_02044a20(void)
{
    func_0204568c(str_020ad05c);
}

void func_02044a34(const char *a, const char *b)
{
    char buf[64];
    func_02051558(buf, 64, a);
    func_0205157c(buf, 64, b);
    func_0204568c(buf);
}

void func_02044a70(void)
{
    func_02045f10();
    func_02045f60();
    func_02044a88();
    func_02045f6c();
}

void func_02044a88(void)
{
    if (g_ringCount == 0) {
        __clear(g_rings, sizeof(g_rings));
    }
    g_ringCount++;
}

void func_02044ae0(void)
{
    func_02045f60();
    func_02044af8();
    func_02045f6c();
    func_02045f38();
}

void func_02044af8(void)
{
    if (--g_ringCount == 0) {
        __clear(g_rings, sizeof(g_rings));
    }
}

CriRing *func_02044b44(int a, int b, int c)
{
    CriRing *r;
    func_02045f60();
    r = func_02044b78(a, b, c);
    func_02045f6c();
    return r;
}

CriRing *func_02044b78(int a, int b, int c)
{
    int i;
    CriRing *r;
    for (i = 0; i < 24; i++) {
        if (g_rings[i].used == 0) break;
    }
    if (i == 24) {
        r = 0;
    } else {
        r = &g_rings[i];
        r->used = 1;
        r->vt = vt_020ad02c;
        r->buf = (char *)a;
        r->size = b;
        r->extra = c;
        r->p8 = dat_0209e368;
        r->cb = (void (*)(void *, int))func_02044a20;
        r->cbarg = r;
        r->i40 = 0;
        r->i44 = 0;
        func_02044da4(r);
    }
    return r;
}

void func_02044c18(CriRing *r)
{
    func_02045f60();
    func_02044c34(r);
    func_02045f6c();
}

void func_02044c34(CriRing *r)
{
    if (r == 0) {
        func_02044a34(s_020ad068, s_020ad074);
        return;
    }
    if (r->used == 0) {
        func_02044a34(s_020ad090, s_020ad09c);
        return;
    }
    func_02012484(r, 0, 0x48);
    r->used = 0;
}

int func_02044c94(CriRing *r)
{
    int v;
    func_02045f60();
    v = func_02044cb8(r);
    func_02045f6c();
    return v;
}

int func_02044cb8(CriRing *r)
{
    if (r == 0) {
        func_02044a34(s_020ad0bc, s_020ad0c8);
        return 0;
    }
    if (r->used != 0) return (int)r->p8;
    func_02044a34(s_020ad0e4, s_020ad0f0);
    return 0;
}

void func_02044d0c(CriRing *r, void (*cb)(void *, int), void *arg)
{
    func_02045f60();
    func_02044d38(r, cb, arg);
    func_02045f6c();
}

void func_02044d38(CriRing *r, void (*cb)(void *, int), void *arg)
{
    if (r == 0) {
        func_02044a34(s_020ad110, s_020ad11c);
        return;
    }
    if (r->used != 0) {
        r->cb = cb;
        r->cbarg = arg;
        return;
    }
    func_02044a34(s_020ad138, s_020ad144);
}

void func_02044d88(CriRing *r)
{
    func_02045f60();
    func_02044da4(r);
    func_02045f6c();
}

void func_02044da4(CriRing *r)
{
    if (r == 0) {
        func_02044a34(s_020ad164, s_020ad170);
        return;
    }
    if (r->used == 0) {
        func_02044a34(s_020ad18c, s_020ad198);
        return;
    }
    r->c = 0;
    r->d10 = r->size;
    r->rd = 0;
    r->wr = 0;
    r->tot28 = 0;
    r->tot2c = 0;
    r->tot30 = 0;
    r->tot34 = 0;
}

int func_02044e18(CriRing *r, int w)
{
    int v;
    func_02045f60();
    v = func_02044e44(r, w);
    func_02045f6c();
    return v;
}

int func_02044e44(CriRing *r, int w)
{
    if (r == 0) {
        func_02044a34(s_020ad1b8, s_020ad1c4);
        return 0;
    }
    if (r->used == 0) {
        func_02044a34(s_020ad1e0, s_020ad1ec);
        return 0;
    }
    if (w == 1) return r->c;
    if (w == 0) return r->d10;
    if (r->cb != 0) r->cb(r->cbarg, -3);
    return 0;
}

void func_02044ecc(CriRing *r, int a, int b, CriSpan *s)
{
    func_02045f60();
    func_02044f00(r, a, b, s);
    func_02045f6c();
}

void func_02044f00(CriRing *r, int mode, int max, CriSpan *s)
{
    if (r == 0) {
        func_02044a34(s_020ad20c, s_020ad218);
        return;
    }
    if (r->used == 0) {
        func_02044a34(s_020ad234, s_020ad240);
        return;
    }
    if (r->size == 0) {
        func_02044a34(s_020ad260, s_020ad26c);
        return;
    }
    if (mode == 0) {
        int lim = r->extra + (r->size - r->rd);
        int n = r->d10;
        if (n >= lim) n = lim;
        s->len = n;
        if (n >= max) n = max;
        s->len = n;
        s->ptr = r->buf + r->rd;
        r->rd = (r->rd + s->len) % r->size;
        r->d10 = r->d10 - s->len;
        r->tot28 = r->tot28 + s->len;
    } else if (mode == 1) {
        int lim = r->extra + (r->size - r->wr);
        int n = r->c;
        if (n >= lim) n = lim;
        s->len = n;
        if (n >= max) n = max;
        s->len = n;
        s->ptr = r->buf + r->wr;
        r->wr = (r->wr + s->len) % r->size;
        r->c = r->c - s->len;
        r->tot30 = r->tot30 + s->len;
    } else {
        s->len = 0;
        s->ptr = 0;
        if (r->cb != 0) r->cb(r->cbarg, -3);
    }
}

void func_02045098(void)
{
    func_020450a4();
}
