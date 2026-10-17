/* CRI play object, 0x0204d968-0x0204dfcc */
typedef struct CriPair { void *p; int n; } CriPair;
struct CriVObj;
typedef struct CriVt {
    int pad[6];
    void (*get)(struct CriVObj *, int, int, CriPair *);      /* 0x18 */
    void (*put)(struct CriVObj *, int, CriPair *);           /* 0x1c */
    void (*commit)(struct CriVObj *, int, CriPair *);        /* 0x20 */
    int (*size)(struct CriVObj *, int);                      /* 0x24 */
} CriVt;
typedef struct CriVObj { const CriVt *vt; } CriVObj;

typedef struct CriPlay {
    void *dec;                 /* 0x00 */
    void *bits;                /* 0x04 */
    signed char used;          /* 0x08 */
    signed char state;         /* 0x09 */
    signed char nch;           /* 0x0a */
    signed char f0b;           /* 0x0b */
    int i0c;
    CriVObj *src;              /* 0x10 */
    CriVObj *ch[2];            /* 0x14 */
    int i1c, i20, i24, i28, i2c;
    int i30, i34, i38, i3c, i40;
    void (*cb44)(int);
    int arg48;
    void (*cb4c)(int, int, int, int);
    int arg50;
} CriPlay;

extern void *memset(void *, int, int);
extern void func_0204ad0c(int, int);
extern void func_0204ad1c(int, int);
extern void func_0204ad28(int, int);
extern void func_0204ad34(void);
extern void func_0204ad64(void);
extern void *SndPlayerCreate(void *, int);
extern void SndPlayerRelease(void *);
extern void SndPlayerReset(void *);
extern void SndPlayerResetAndSetSrc(void *, void *);
extern void SndPlayerReadHdr(void *);
extern int func_0204b150(void *);
extern int func_0204b224(void *);
extern int func_0204b238(void *, int, int, int);
extern int func_0204b2dc(void *);
extern int func_0204b2e8(void *);
extern int func_0204b2f4(void *);
extern int func_0204b314(void *);
extern void func_0204bdd4(int *, int *);
extern void func_020517b4(int *, int *);
extern void func_020517dc(int *, int *);
extern void func_0204d95c(int, int);
extern void *CriBitsCreate(CriVObj *, void *, int);
extern void CriBitsClear(void *);
extern void CriBitsReset(void *, CriVObj *);
extern int CriBitsGetPos(void *);
extern int CriBitsIsEmpty(void *);
extern int data_02218f38;
extern volatile int data_0209e420;
extern int data_020af610[2];
extern void func_0204dfcc(int *, int *);
extern void CriPlayReset(CriPlay *);
extern void func_0204df00(CriPair *, int, CriPair *, CriPair *);
extern void CriPlayAdvanceState2(CriPlay *);
extern void CriPlayAdvanceState1(CriPlay *);
extern int func_0204df88(CriPlay *);
extern int func_0204df98(CriPlay *);

void func_0204d968(int a, int b) { func_0204ad0c(a, b); }
void func_0204d974(int a, int b) { func_0204ad1c(a, b); }
void func_0204d980(int a, int b) { func_0204ad28(a, b); }

void func_0204d98c(void)
{
    int a, b;
    data_0209e420;
    data_0209e420;
    if (data_02218f38 == 0) {
        func_020517b4(&a, &b);
        func_0204d95c(a, b);
        func_020517dc(&a, &b);
        func_0204d968(a, b);
        func_0204bdd4(&a, &b);
        func_0204d974(a, b);
        func_0204dfcc(&a, &b);
        func_0204d980(a, b);
        func_0204ad34();
    }
    data_02218f38++;
}

void func_0204da34(void)
{
    if (data_02218f38 == 1) {
        func_0204ad64();
    }
    data_02218f38--;
}

void CriPlayReset(CriPlay *o)
{
    o->i0c = 0;
    o->i1c = 0;
    o->i20 = 0;
    o->i24 = 0;
    o->i28 = 0;
    o->i2c = 0;
    o->i30 = 0x7fffffff;
    o->i34 = -1;
    o->i38 = 0;
    o->i3c = 0;
    o->f0b = 0;
}

CriPlay *CriPlayCreate(CriVObj *h, int n, CriVObj **arr, char *buf, int size)
{
    char *end;
    CriPlay *o = (CriPlay *)(((unsigned)buf + 7) & ~7);
    char *p;
    char *q;
    int i;
    end = buf + size;
    memset(o, 0, 0x54);
    p = (char *)((((unsigned)o + 0x5b)) & ~7);
    q = p + 0x34;
    if (q > end) return 0;
    o->bits = CriBitsCreate(h, p, 0x34);
    if (o->bits == 0) return 0;
    q = (char *)(((unsigned)q + 7) & ~7);
    if (q + 0x250 + 0x1000 > end) {
        CriBitsClear(o->bits);
        return 0;
    }
    o->dec = SndPlayerCreate(q, 0x1250);
    if (o->dec == 0) {
        CriBitsClear(o->bits);
        return 0;
    }
    o->nch = n;
    o->src = h;
    for (i = 0; i < n; i++) o->ch[i] = arr[i];
    o->state = 0;
    CriPlayReset(o);
    o->cb44 = 0;
    o->arg48 = 0;
    o->cb4c = 0;
    o->arg50 = 0;
    o->used = 1;
    return o;
}

void CriPlayRelease(CriPlay *o)
{
    if (o == 0) return;
    if (o->bits) {
        void *b = o->bits;
        o->bits = 0;
        CriBitsClear(b);
    }
    if (o->dec) {
        void *d = o->dec;
        o->dec = 0;
        SndPlayerRelease(d);
    }
    memset(o, 0, 0x54);
}

int CriPlayGetState(CriPlay *o) { return o->state; }
void CriPlaySetSrc(CriPlay *o, CriVObj *s) { o->src = s; }

void CriPlayStart(CriPlay *o)
{
    CriPlayReset(o);
    CriBitsReset(o->bits, o->src);
    SndPlayerResetAndSetSrc(o->dec, o->bits);
    o->state = 1;
}

/* Also called by cri_mpa_dec.c's func_0204bfb0 on every poll while CriPlayGetState is 0, and once it is 3. */
void CriPlayStop(CriPlay *o)
{
    SndPlayerReset(o->dec);
    o->state = 0;
}

void CriPlayAdvanceState1(CriPlay *o)
{
    if (o->src->vt->size(o->src, 1) < 0x24) return;
    SndPlayerReadHdr(o->dec);
    func_0204b150(o->dec);
    o->state = 2;
}

void CriPlayAdvanceState2(CriPlay *o)
{
    CriPair in;
    CriPair out;
    int c = o->f0b;
    void *dec = o->dec;
    CriVObj *src = o->src;
    int unit, blk, n, take, lim;
    void (*cb)(int, int, void *, int);

    if (c == 1 && src->vt->size(src, 1) == 0 && CriBitsIsEmpty(o->bits) == 1) {
        o->state = 3;
        return;
    }
    blk = func_0204df98(o);
    unit = func_0204df88(o) / 8;
    if ((int)(o->ch[0]->vt->size(o->ch[0], 0) / unit) < blk) return;
    if (func_0204b224(dec) != 0) {
        int r = func_0204b150(dec);
        if (r == 1) {
            o->state = 3;
            return;
        }
        if (r == -1) return;
    }
    o->ch[0]->vt->get(o->ch[0], 0, blk * unit, &in);
    if (blk != (int)(in.n / unit)) {
        o->ch[0]->vt->put(o->ch[0], 0, &in);
        return;
    }
    n = func_0204b238(dec, (int)in.p, 0, blk);
    if (o->i20 < 0x1e0) take = 0; else take = n;
    lim = func_0204b2f4(o->dec);
    if (take + o->i28 > lim) take = lim - o->i28;
    func_0204df00(&in, take * unit, &in, &out);
    cb = (void (*)(int, int, void *, int))o->cb4c;
    if (cb && take > 0) cb(o->arg50, 0, in.p, in.n);
    o->ch[0]->vt->commit(o->ch[0], 1, &in);
    o->ch[0]->vt->put(o->ch[0], 0, &out);
    o->i20 += n;
    o->i24 = (CriBitsGetPos(o->bits) + 7) / 8;
    o->i28 += take;
    o->i2c += n;
    o->i38 += n;
    if (o->i34 >= 0 && o->i38 >= o->i34) {
        if (o->cb44) o->cb44(o->arg48);
    }
    if (o->i28 >= lim) o->state = 3;
}

/* Same body as CriSplitSpan (cri_err.c) and func_0204e67c (cri_ahx_bitrd.c), on CriPair. */
void func_0204df00(CriPair *src, int limit, CriPair *a, CriPair *b)
{
    int len = src->n;
    char *p = src->p;
    a->p = p;
    a->n = len;
    b->n = len;
    if (a->n > limit) a->n = limit;
    b->n = b->n - a->n;
    if (b->n == 0) {
        b->p = 0;
    } else {
        b->p = (char *)a->p + a->n;
    }
}

void CriPlayAdvance(CriPlay *o)
{
    int s = o->state;
    if (s == 2) {
        CriPlayAdvanceState2(o);
    } else if (s == 1) {
        CriPlayAdvanceState1(o);
    }
}
int func_0204df6c(CriPlay *o) { return o->i24; }
int func_0204df74(CriPlay *o) { return o->i28; }
void func_0204df7c(CriPlay *o, void (*cb)(int, int, int, int), int arg) { o->cb4c = cb; o->arg50 = arg; }
int func_0204df88(CriPlay *o) { return func_0204b2dc(o->dec); }
int func_0204df98(CriPlay *o) { return func_0204b2e8(o->dec); }
int func_0204dfa8(CriPlay *o) { return func_0204b314(o->dec); }
void func_0204dfb8(CriPlay *o, int v) { o->i30 = v; }
void func_0204dfc0(CriPlay *o) { o->i0c = 1; }
void func_0204dfcc(int *a, int *b)
{
    if (a) *a = data_020af610[0];
    if (b) *b = data_020af610[1];
}
