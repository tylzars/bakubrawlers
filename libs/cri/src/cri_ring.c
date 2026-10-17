/* CRI middleware, 0x02044a20-0x020450a4 */
extern void *memset(void *, int, int);

/* ---- 0x02044a20-0x02045098: 24-entry ring buffer pool (0x48 bytes) ---- */
typedef struct CriRing {
    void *vt;           /* 0x00 */
    int used;           /* 0x04 */
    void *p8;           /* 0x08 */
    int c, d10;         /* 0x0c, 0x10 */
    int rd, wr;         /* 0x14, 0x18 */
    char *buf;          /* 0x1c */
    int size;           /* 0x20 */
    int extra;          /* 0x24 */
    int tot28, tot2c, tot30, tot34;
    void (*cb)(void *, int);   /* 0x38 */
    void *cbarg;        /* 0x3c */
    int i40, i44;
} CriRing;

typedef struct CriSpan { char *ptr; int len; } CriSpan;

extern int gRingCount;         /* 0x02216940 */
extern CriRing gRings[24];     /* 0x02216944 */
extern char data_020ad02c[], data_0209e368[];
extern char data_020ad05c[];
extern char data_020ad068[], data_020ad074[], data_020ad090[], data_020ad09c[];
extern char data_020ad0bc[], data_020ad0c8[], data_020ad0e4[], data_020ad0f0[];
extern char data_020ad110[], data_020ad11c[], data_020ad138[], data_020ad144[];
extern char data_020ad164[], data_020ad170[], data_020ad18c[], data_020ad198[];
extern char data_020ad1b8[], data_020ad1c4[], data_020ad1e0[], data_020ad1ec[];
extern char data_020ad20c[], data_020ad218[], data_020ad234[], data_020ad240[], data_020ad260[], data_020ad26c[];

extern void func_0204568c(const char *);
extern void CriCopyString(char *, int, const char *);
extern void CriAppendString(char *, int, const char *);
extern void func_02045f10(void);
extern void func_02045f60(void);
extern void func_02045f6c(void);
extern void func_02045f38(void);
extern void CriRingCommitSpan(void);
extern void CriRingPoolAddRefNoLock(void);
extern void CriRingPoolReleaseRefNoLock(void);
extern CriRing *CriRingAllocNoLock(int, int, int);
extern void CriRingDeallocNoLock(CriRing *);
extern int func_02044cb8(CriRing *);
extern void CriRingSetCallbackNoLock(CriRing *, void (*)(void *, int), void *);
extern void CriRingResetNoLock(CriRing *);
extern int CriRingGetSideBytesNoLock(CriRing *, int);
extern void CriRingGetSpanNoLock(CriRing *, int, int, CriSpan *);

void CriRingDefaultCallback(void)
{
    func_0204568c(data_020ad05c);
}

void CriRingSendJoinedText(const char *a, const char *b)
{
    char buf[64];
    CriCopyString(buf, 64, a);
    CriAppendString(buf, 64, b);
    func_0204568c(buf);
}

void CriRingPoolAddRef(void)
{
    func_02045f10();
    func_02045f60();
    CriRingPoolAddRefNoLock();
    func_02045f6c();
}

void CriRingPoolAddRefNoLock(void)
{
    if (gRingCount == 0) {
        __clear(gRings, sizeof(gRings));
    }
    gRingCount++;
}

void CriRingPoolReleaseRef(void)
{
    func_02045f60();
    CriRingPoolReleaseRefNoLock();
    func_02045f6c();
    func_02045f38();
}

void CriRingPoolReleaseRefNoLock(void)
{
    if (--gRingCount == 0) {
        __clear(gRings, sizeof(gRings));
    }
}

CriRing *CriRingAlloc(int a, int b, int c)
{
    CriRing *r;
    func_02045f60();
    r = CriRingAllocNoLock(a, b, c);
    func_02045f6c();
    return r;
}

CriRing *CriRingAllocNoLock(int a, int b, int c)
{
    int i;
    CriRing *r;
    for (i = 0; i < 24; i++) {
        if (gRings[i].used == 0) break;
    }
    if (i == 24) {
        r = 0;
    } else {
        r = &gRings[i];
        r->used = 1;
        r->vt = data_020ad02c;
        r->buf = (char *)a;
        r->size = b;
        r->extra = c;
        r->p8 = data_0209e368;
        r->cb = (void (*)(void *, int))CriRingDefaultCallback;
        r->cbarg = r;
        r->i40 = 0;
        r->i44 = 0;
        CriRingResetNoLock(r);
    }
    return r;
}

void CriRingDealloc(CriRing *r)
{
    func_02045f60();
    CriRingDeallocNoLock(r);
    func_02045f6c();
}

void CriRingDeallocNoLock(CriRing *r)
{
    if (r == 0) {
        CriRingSendJoinedText(data_020ad068, data_020ad074);
        return;
    }
    if (r->used == 0) {
        CriRingSendJoinedText(data_020ad090, data_020ad09c);
        return;
    }
    memset(r, 0, 0x48);
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
        CriRingSendJoinedText(data_020ad0bc, data_020ad0c8);
        return 0;
    }
    if (r->used != 0) return (int)r->p8;
    CriRingSendJoinedText(data_020ad0e4, data_020ad0f0);
    return 0;
}

void CriRingSetCallback(CriRing *r, void (*cb)(void *, int), void *arg)
{
    func_02045f60();
    CriRingSetCallbackNoLock(r, cb, arg);
    func_02045f6c();
}

void CriRingSetCallbackNoLock(CriRing *r, void (*cb)(void *, int), void *arg)
{
    if (r == 0) {
        CriRingSendJoinedText(data_020ad110, data_020ad11c);
        return;
    }
    if (r->used != 0) {
        r->cb = cb;
        r->cbarg = arg;
        return;
    }
    CriRingSendJoinedText(data_020ad138, data_020ad144);
}

void CriRingReset(CriRing *r)
{
    func_02045f60();
    CriRingResetNoLock(r);
    func_02045f6c();
}

void CriRingResetNoLock(CriRing *r)
{
    if (r == 0) {
        CriRingSendJoinedText(data_020ad164, data_020ad170);
        return;
    }
    if (r->used == 0) {
        CriRingSendJoinedText(data_020ad18c, data_020ad198);
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

int CriRingGetSideBytes(CriRing *r, int w)
{
    int v;
    func_02045f60();
    v = CriRingGetSideBytesNoLock(r, w);
    func_02045f6c();
    return v;
}

int CriRingGetSideBytesNoLock(CriRing *r, int w)
{
    if (r == 0) {
        CriRingSendJoinedText(data_020ad1b8, data_020ad1c4);
        return 0;
    }
    if (r->used == 0) {
        CriRingSendJoinedText(data_020ad1e0, data_020ad1ec);
        return 0;
    }
    if (w == 1) return r->c;
    if (w == 0) return r->d10;
    if (r->cb != 0) r->cb(r->cbarg, -3);
    return 0;
}

void CriRingGetSpan(CriRing *r, int a, int b, CriSpan *s)
{
    func_02045f60();
    CriRingGetSpanNoLock(r, a, b, s);
    func_02045f6c();
}

void CriRingGetSpanNoLock(CriRing *r, int mode, int max, CriSpan *s)
{
    if (r == 0) {
        CriRingSendJoinedText(data_020ad20c, data_020ad218);
        return;
    }
    if (r->used == 0) {
        CriRingSendJoinedText(data_020ad234, data_020ad240);
        return;
    }
    if (r->size == 0) {
        CriRingSendJoinedText(data_020ad260, data_020ad26c);
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
    CriRingCommitSpan();
}
