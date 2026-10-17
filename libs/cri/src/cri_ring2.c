/* CRI middleware, 0x020450a4-0x02045f90 */
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
    void (*peek)(void *, void *);  /* 0x40 */
    void *peekarg;      /* 0x44 */
} CriRing;

typedef struct CriSpan { char *ptr; int len; } CriSpan;

/* 0x24-byte object (6-entry pool) */
typedef struct CriBuf {
    void *vt;           /* 0x00 */
    int used;           /* 0x04 */
    void *p8;           /* 0x08 */
    int c, d10;         /* 0x0c, 0x10 */
    char *buf;          /* 0x14 */
    int size;           /* 0x18 */
    void (*cb)(void *, int);   /* 0x1c */
    void *cbarg;        /* 0x20 */
} CriBuf;

typedef struct CriLock { int pad0; int f4; volatile int cnt; } CriLock;

extern CriBuf gBufs[6];        /* 0x02217008 */
extern int gBufCount;          /* 0x02217004 */
extern char gPool3[0x240];     /* 0x022170f0 */
extern int gPool3Count;        /* 0x022170ec */
extern CriLock gLock;          /* 0x022170e0 */
extern char data_020ad4a0[], data_0209e378[];
extern char data_020ad284[], data_020ad290[], data_020ad2ac[], data_020ad2b8[];
extern char data_020ad2d8[], data_020ad2e4[], data_020ad300[], data_020ad30c[], data_020ad32c[], data_020ad338[];
extern char data_020ad350[], data_020ad35c[], data_020ad378[], data_020ad384[];
extern char data_020ad3a4[], data_020ad3b0[], data_020ad3cc[], data_020ad3d8[];
extern char data_020ad3f8[], data_020ad404[], data_020ad420[], data_020ad42c[];
extern char data_020ad44c[], data_020ad458[], data_020ad474[], data_020ad480[];
extern char data_020ad4d0[];
extern char data_020ad4dc[], data_020ad4e8[], data_020ad504[], data_020ad510[];
extern char data_020ad530[], data_020ad53c[], data_020ad558[], data_020ad564[];
extern char data_020ad584[], data_020ad590[], data_020ad5ac[], data_020ad5b8[];
extern char data_020ad5d8[], data_020ad5e4[], data_020ad600[], data_020ad60c[];
extern char data_020ad62c[], data_020ad638[], data_020ad654[], data_020ad660[];
extern char data_020ad680[], data_020ad68c[], data_020ad6a8[], data_020ad6b4[];
extern char data_020ad6d4[], data_020ad6e0[], data_020ad6fc[], data_020ad708[];
extern char data_020ad728[], data_020ad734[], data_020ad750[], data_020ad75c[];
extern char data_020ad77c[], data_020ad788[], data_020ad7a4[], data_020ad7b0[];

extern void CriRingSendJoinedText(const char *, const char *);
extern void *func_02012464(void *, void *, int);
extern void *memset(void *, int, int);
extern void CriSendText(const char *);
extern void CriCopyString(char *, int, const char *);
extern void CriAppendString(char *, int, const char *);
extern void func_02045f10(void);
extern void func_02045f38(void);
extern void func_02045f60(void);
extern void func_02045f6c(void);
extern void CriLockEnter1(void);
extern void CriLockLeave1(void);

extern void CriRingCommitSpan(CriRing *, int, CriSpan *);
extern void CriRingPutBackSpanNoLock(CriRing *, int, CriSpan *);
extern int CriRingCanGetNoLock(CriRing *, int, int, int *);
extern void *CriRingGetBufNoLock(CriRing *);
extern int CriRingGetSizeNoLock(CriRing *);
extern int CriRingGetExtraNoLock(CriRing *);
extern void func_0204568c(const char *);
extern void CriBufDefaultCallback(void);
extern void CriBufSendJoinedText(const char *, const char *);
extern void CriBufPoolAddRef(void);
extern void CriBufPoolReleaseRef(void);
extern CriBuf *CriBufAllocNoLock(char *, int);
extern void CriBufDeallocNoLock(CriBuf *);
extern void *func_02045928(CriBuf *);
extern void CriBufSetCallbackNoLock(CriBuf *, void (*)(void *, int), void *);
extern void CriBufResetNoLock(CriBuf *);
extern int CriBufGetSideBytesNoLock(CriBuf *, int);
extern void CriBufGetSpanNoLock(CriBuf *, int, int, CriSpan *);
extern void func_02045c58(CriBuf *, int, CriSpan *);
extern void CriBufPutBackSpanNoLock(CriBuf *, int, CriSpan *);
extern int CriBufCanGetNoLock(CriBuf *, int, int, int *);
extern void func_02045f90(void);

void CriRingCommitSpan(CriRing *r, int mode, CriSpan *s)
{
    if (r == 0) {
        CriRingSendJoinedText(data_020ad284, data_020ad290);
        return;
    }
    if (r->used == 0) {
        CriRingSendJoinedText(data_020ad2ac, data_020ad2b8);
        return;
    }
    if (s->len <= 0) return;
    if (s->ptr == 0) return;
    if (mode == 1) {
        int off, lim;
        if (r->peek != 0) r->peek(r->peekarg, s);
        off = (int)s->ptr - (int)r->buf;
        lim = r->extra;
        if (off < lim) {
            int n = s->len;
            if (n >= lim - off) n = lim - off;
            func_02012464(r->buf + (r->size + off), s->ptr, n);
        }
        off = (int)s->ptr - (int)r->buf + s->len;
        if (off > (int)r->size) {
            int n = s->len;
            int ex = off - (int)r->size;
            if (n >= ex) n = ex;
            func_02012464(r->buf, r->buf + (off - n), n);
        }
        func_02045f60();
        r->c = r->c + s->len;
        r->tot34 = r->tot34 + s->len;
        func_02045f6c();
    } else if (mode == 0) {
        func_02045f60();
        r->d10 = r->d10 + s->len;
        r->tot2c = r->tot2c + s->len;
        func_02045f6c();
    } else {
        func_02045f60();
        s->len = 0;
        s->ptr = 0;
        func_02045f6c();
        if (r->cb != 0) r->cb(r->cbarg, -3);
    }
}

void CriRingPutBackSpan(CriRing *r, int mode, CriSpan *s)
{
    func_02045f60();
    CriRingPutBackSpanNoLock(r, mode, s);
    func_02045f6c();
}

void CriRingPutBackSpanNoLock(CriRing *r, int mode, CriSpan *s)
{
    char *ptr;
    int size;
    if (r == 0) {
        CriRingSendJoinedText(data_020ad2d8, data_020ad2e4);
        return;
    }
    if (r->used == 0) {
        CriRingSendJoinedText(data_020ad300, data_020ad30c);
        return;
    }
    size = r->size;
    if (size == 0) {
        CriRingSendJoinedText(data_020ad32c, data_020ad338);
        return;
    }
    if (s->len <= 0) return;
    ptr = s->ptr;
    if (ptr == 0) return;
    if (mode == 0) {
        unsigned a = (r->rd + size - s->len) % size;
        unsigned b = (ptr - r->buf) % size;
        if (a == b) {
            r->rd = a;
            r->d10 = r->d10 + s->len;
        } else if (r->cb != 0) {
            r->cb(r->cbarg, -3);
        }
        r->tot28 = r->tot28 - s->len;
    } else if (mode == 1) {
        unsigned a = (r->wr + size - s->len) % size;
        unsigned b = (ptr - r->buf) % size;
        if (a == b) {
            r->wr = a;
            r->c = r->c + s->len;
        } else if (r->cb != 0) {
            r->cb(r->cbarg, -3);
        }
        r->tot30 = r->tot30 - s->len;
    } else {
        s->len = 0;
        s->ptr = 0;
        if (r->cb != 0) r->cb(r->cbarg, -3);
    }
}

int CriRingCanGet(CriRing *r, int mode, int max, int *out)
{
    int v;
    func_02045f60();
    v = CriRingCanGetNoLock(r, mode, max, out);
    func_02045f6c();
    return v;
}

int CriRingCanGetNoLock(CriRing *r, int mode, int max, int *out)
{
    int n;
    if (r == 0) {
        CriRingSendJoinedText(data_020ad350, data_020ad35c);
        return 0;
    }
    if (r->used == 0) {
        CriRingSendJoinedText(data_020ad378, data_020ad384);
        return 0;
    }
    if (mode == 0) {
        n = r->d10;
        if (n >= r->extra + (r->size - r->rd)) n = r->extra + (r->size - r->rd);
        if (n >= max) n = max;
    } else if (mode == 1) {
        n = r->c;
        if (n >= r->extra + (r->size - r->wr)) n = r->extra + (r->size - r->wr);
        if (n >= max) n = max;
    } else {
        n = 0;
        if (r->cb != 0) r->cb(r->cbarg, -3);
    }
    *out = n;
    return n == max;
}

void *CriRingGetBuf(CriRing *r)
{
    void *v;
    func_02045f60();
    v = CriRingGetBufNoLock(r);
    func_02045f6c();
    return v;
}

void *CriRingGetBufNoLock(CriRing *r)
{
    if (r == 0) {
        CriRingSendJoinedText(data_020ad3a4, data_020ad3b0);
        return 0;
    }
    if (r->used != 0) return r->buf;
    CriRingSendJoinedText(data_020ad3cc, data_020ad3d8);
    return 0;
}

int CriRingGetSize(CriRing *r)
{
    int v;
    func_02045f60();
    v = CriRingGetSizeNoLock(r);
    func_02045f6c();
    return v;
}

int CriRingGetSizeNoLock(CriRing *r)
{
    if (r == 0) {
        CriRingSendJoinedText(data_020ad3f8, data_020ad404);
        return 0;
    }
    if (r->used != 0) return r->size;
    CriRingSendJoinedText(data_020ad420, data_020ad42c);
    return 0;
}

int CriRingGetExtra(CriRing *r)
{
    int v;
    func_02045f60();
    v = CriRingGetExtraNoLock(r);
    func_02045f6c();
    return v;
}

int CriRingGetExtraNoLock(CriRing *r)
{
    if (r == 0) {
        CriRingSendJoinedText(data_020ad44c, data_020ad458);
        return 0;
    }
    if (r->used != 0) return r->extra;
    CriRingSendJoinedText(data_020ad474, data_020ad480);
    return 0;
}

void func_0204568c(const char *s)
{
    CriSendText(s);
}

void CriBufDefaultCallback(void)
{
    func_0204568c(data_020ad4d0);
}

void CriBufSendJoinedText(const char *a, const char *b)
{
    char buf[64];
    CriCopyString(buf, 64, a);
    CriAppendString(buf, 64, b);
    func_0204568c(buf);
}

void func_020456e8(void)
{
    func_02045f10();
    func_02045f60();
    CriBufPoolAddRef();
    func_02045f6c();
}

void CriBufPoolAddRef(void)
{
    if (gBufCount == 0) {
        __clear(gBufs, sizeof(gBufs));
    }
    gBufCount++;
}

void func_02045760(void)
{
    func_02045f60();
    CriBufPoolReleaseRef();
    func_02045f6c();
    func_02045f38();
}

void CriBufPoolReleaseRef(void)
{
    if (--gBufCount == 0) {
        __clear(gBufs, sizeof(gBufs));
    }
}

CriBuf *CriBufAlloc(char *buf, int size)
{
    CriBuf *b;
    func_02045f60();
    b = CriBufAllocNoLock(buf, size);
    func_02045f6c();
    return b;
}

CriBuf *CriBufAllocNoLock(char *buf, int size)
{
    int i;
    CriBuf *b;
    for (i = 0; i < 6; i++) {
        if (gBufs[i].used == 0) break;
    }
    if (i == 6) return 0;
    b = &gBufs[i];
    b->used = 1;
    b->vt = data_020ad4a0;
    b->buf = buf;
    b->size = size;
    b->p8 = data_0209e378;
    b->cb = (void (*)(void *, int))CriBufDefaultCallback;
    b->cbarg = b;
    CriBufResetNoLock(b);
    return b;
}

void CriBufDealloc(CriBuf *b)
{
    func_02045f60();
    CriBufDeallocNoLock(b);
    func_02045f6c();
}

void CriBufDeallocNoLock(CriBuf *b)
{
    if (b == 0) {
        CriBufSendJoinedText(data_020ad4dc, data_020ad4e8);
        return;
    }
    if (b->used == 0) {
        CriBufSendJoinedText(data_020ad504, data_020ad510);
        return;
    }
    memset(b, 0, 0x24);
    b->used = 0;
}

void *func_02045904(CriBuf *b)
{
    void *v;
    func_02045f60();
    v = func_02045928(b);
    func_02045f6c();
    return v;
}

void *func_02045928(CriBuf *b)
{
    if (b == 0) {
        CriBufSendJoinedText(data_020ad530, data_020ad53c);
        return 0;
    }
    if (b->used != 0) return b->p8;
    CriBufSendJoinedText(data_020ad558, data_020ad564);
    return 0;
}

void CriBufSetCallback(CriBuf *b, void (*cb)(void *, int), void *arg)
{
    func_02045f60();
    CriBufSetCallbackNoLock(b, cb, arg);
    func_02045f6c();
}

void CriBufSetCallbackNoLock(CriBuf *b, void (*cb)(void *, int), void *arg)
{
    if (b == 0) {
        CriBufSendJoinedText(data_020ad584, data_020ad590);
        return;
    }
    if (b->used != 0) {
        b->cb = cb;
        b->cbarg = arg;
        return;
    }
    CriBufSendJoinedText(data_020ad5ac, data_020ad5b8);
}

void CriBufReset(CriBuf *b)
{
    func_02045f60();
    CriBufResetNoLock(b);
    func_02045f6c();
}

void CriBufResetNoLock(CriBuf *b)
{
    if (b == 0) {
        CriBufSendJoinedText(data_020ad5d8, data_020ad5e4);
        return;
    }
    if (b->used == 0) {
        CriBufSendJoinedText(data_020ad600, data_020ad60c);
        return;
    }
    b->c = b->size;
    b->d10 = 0;
}

int CriBufGetSideBytes(CriBuf *b, int w)
{
    int v;
    func_02045f60();
    v = CriBufGetSideBytesNoLock(b, w);
    func_02045f6c();
    return v;
}

int CriBufGetSideBytesNoLock(CriBuf *b, int w)
{
    if (b == 0) {
        CriBufSendJoinedText(data_020ad62c, data_020ad638);
        return 0;
    }
    if (b->used == 0) {
        CriBufSendJoinedText(data_020ad654, data_020ad660);
        return 0;
    }
    if (w == 1) return b->c;
    if (w == 0) return 0;
    if (b->cb != 0) b->cb(b->cbarg, -3);
    return 0;
}

void CriBufGetSpan(CriBuf *b, int mode, int max, CriSpan *s)
{
    func_02045f60();
    CriBufGetSpanNoLock(b, mode, max, s);
    func_02045f6c();
}

void CriBufGetSpanNoLock(CriBuf *b, int mode, int max, CriSpan *s)
{
    if (b == 0) {
        CriBufSendJoinedText(data_020ad680, data_020ad68c);
        return;
    }
    if (b->used == 0) {
        CriBufSendJoinedText(data_020ad6a8, data_020ad6b4);
        return;
    }
    if (mode == 0) {
        s->len = 0;
        s->ptr = 0;
        return;
    }
    if (mode == 1) {
        int n = b->c;
        if (n >= max) n = max;
        s->len = n;
        s->ptr = b->buf + b->d10;
        b->d10 = b->d10 + s->len;
        b->c = b->c - s->len;
        return;
    }
    s->len = 0;
    s->ptr = 0;
    if (b->cb != 0) b->cb(b->cbarg, -3);
}

void func_02045c2c(CriBuf *b, int mode, CriSpan *s)
{
    func_02045f60();
    func_02045c58(b, mode, s);
    func_02045f6c();
}

void func_02045c58(CriBuf *b, int mode, CriSpan *s)
{
    if (b == 0) {
        CriBufSendJoinedText(data_020ad6d4, data_020ad6e0);
        return;
    }
    if (b->used == 0) {
        CriBufSendJoinedText(data_020ad6fc, data_020ad708);
        return;
    }
    if (s->len <= 0) return;
    if (s->ptr == 0) return;
    if (!(mode != 0 && mode != 1)) return;
    s->len = 0;
    s->ptr = 0;
    if (b->cb != 0) b->cb(b->cbarg, -3);
}

void CriBufPutBackSpan(CriBuf *b, int mode, CriSpan *s)
{
    func_02045f60();
    CriBufPutBackSpanNoLock(b, mode, s);
    func_02045f6c();
}

void CriBufPutBackSpanNoLock(CriBuf *b, int mode, CriSpan *s)
{
    int d;
    if (b == 0) {
        CriBufSendJoinedText(data_020ad728, data_020ad734);
        return;
    }
    if (b->used == 0) {
        CriBufSendJoinedText(data_020ad750, data_020ad75c);
        return;
    }
    if (s->len <= 0) return;
    if (s->ptr == 0) return;
    if (mode == 0) {
        if (b->cb != 0) b->cb(b->cbarg, -3);
        return;
    }
    if (mode == 1) {
        d = b->d10 - s->len;
        if (d <= 0) d = 0;
        b->d10 = d;
        b->c = b->size < b->c + s->len ? b->size : b->c + s->len;
        if (d != (int)s->ptr - (int)b->buf && b->cb != 0) b->cb(b->cbarg, -3);
        return;
    }
    s->len = 0;
    s->ptr = 0;
    if (b->cb != 0) b->cb(b->cbarg, -3);
}

int CriBufCanGet(CriBuf *b, int mode, int max, int *out)
{
    int v;
    func_02045f60();
    v = CriBufCanGetNoLock(b, mode, max, out);
    func_02045f6c();
    return v;
}

int CriBufCanGetNoLock(CriBuf *b, int mode, int max, int *out)
{
    int n;
    if (b == 0) {
        CriBufSendJoinedText(data_020ad77c, data_020ad788);
        return 0;
    }
    if (b->used == 0) {
        CriBufSendJoinedText(data_020ad7a4, data_020ad7b0);
        return 0;
    }
    if (mode == 0) {
        n = 0;
    } else if (mode == 1) {
        n = b->c;
        if (n >= max) n = max;
    } else {
        n = 0;
        if (b->cb != 0) b->cb(b->cbarg, -3);
    }
    *out = n;
    return n == max;
}

void func_02045f10(void)
{
    gLock.cnt++;
    if (gLock.cnt == 1) gLock.f4 = 0;
}

void func_02045f38(void)
{
    gLock.cnt--;
    if (gLock.cnt == 0) gLock.f4 = 0;
}

void func_02045f60(void) { CriLockEnter1(); }

void func_02045f6c(void) { CriLockLeave1(); }

void func_02045f78(void)
{
    func_02045f10();
    func_02045f60();
    func_02045f90();
    func_02045f6c();
}
