/* CRI MPEG bit reader, 0x0204e330-0x0204e844 */
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

typedef struct CriBits {
    int used;                      /* 0x00 */
    CriVObj *src;                  /* 0x04 */
    int acc;                       /* 0x08 */
    int bits;                      /* 0x0c */
    int pos;                       /* 0x10 */
    int total;                     /* 0x14 */
    int chunk;                     /* 0x18 */
    unsigned char *base;           /* 0x1c */
    int len;                       /* 0x20 */
    int avail;                     /* 0x24 */
    unsigned char *cur;            /* 0x28 */
} CriBits;

extern void *memset(void *, int, int);
extern const int data_0209e424[];
extern void RefillCriBits(CriBits *);
extern void func_0204e67c(CriPair *, int, CriPair *, CriPair *);
extern int ReadCriBits(CriBits *, int);
extern int GetCriBitsPos(CriBits *);
extern int IsCriBitsEmpty(CriBits *);

CriBits *CreateCriBits(CriVObj *src, char *buf, int size)
{
    CriBits *r;
    int a, b;
    if ((unsigned)size < 0x34) return 0;
    r = (CriBits *)(((unsigned)buf + 7) & ~7);
    memset(r, 0, 0x2c);
    r->src = src;
    r->pos = 0;
    a = src->vt->size(src, 1);
    b = src->vt->size(src, 0);
    r->total = a + b;
    r->chunk = r->total / 4;
    r->avail = 0;
    r->base = 0;
    r->len = 0;
    r->cur = 0;
    r->used = 1;
    return r;
}

void ClearCriBits(CriBits *r)
{
    if (r == 0) return;
    memset(r, 0, 0x2c);
}

void ResetCriBits(CriBits *r, CriVObj *src)
{
    int a, b;
    if (r == 0) return;
    memset(r, 0, 0x2c);
    r->src = src;
    r->pos = 0;
    a = src->vt->size(src, 1);
    b = src->vt->size(src, 0);
    r->total = a + b;
    r->chunk = r->total / 4;
    r->avail = 0;
    r->base = 0;
    r->len = 0;
    r->cur = 0;
    r->used = 1;
}

void RefillCriBits(CriBits *r)
{
    CriPair pa;
    CriPair pb;
    int n = (0x20 - r->bits) / 8;
    unsigned char *p;
    int a;
    if (r->avail < 4) {
        int len = r->len;
        CriPair *pp = &pa;
        pa.p = r->base;
        pa.n = len;
        if (len != 0) {
            func_0204e67c(pp, len - r->avail, pp, &pb);
            r->src->vt->commit(r->src, 0, &pa);
            r->src->vt->put(r->src, 1, &pb);
        }
        r->src->vt->get(r->src, 1, r->chunk, (CriPair *)&r->base);
        r->cur = r->base;
        r->avail = r->len;
    }
    if (n >= r->avail) n = r->avail;
    if (n == 3) {
        p = r->cur;
        a = r->acc;
        a = (a << 8) | *p++;
        a = (a << 8) | *p++;
        a = (a << 8) | *p++;
        r->cur = p;
        r->acc = a;
        r->bits += 0x18;
        r->avail -= 3;
    } else if (n == 2) {
        p = r->cur;
        a = r->acc;
        a = (a << 8) | *p++;
        a = (a << 8) | *p++;
        r->cur = p;
        r->acc = a;
        r->bits += 0x10;
        r->avail -= 2;
    } else if (n == 1) {
        p = r->cur;
        a = r->acc;
        a = (a << 8) | *p++;
        r->cur = p;
        r->acc = a;
        r->bits += 8;
        r->avail -= 1;
    } else if (n == 4) {
        p = r->cur;
        a = r->acc;
        a = (a << 8) | *p++;
        a = (a << 8) | *p++;
        a = (a << 8) | *p++;
        a = (a << 8) | *p++;
        r->cur = p;
        r->acc = a;
        r->bits += 0x20;
        r->avail -= 4;
    }
}

void func_0204e67c(CriPair *src, int limit, CriPair *a, CriPair *b)
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

int CanReadCriBits(CriBits *r, int n)
{
    if (r->bits < n) RefillCriBits(r);
    if (n > r->bits) return 0;
    return 1;
}

int ReadCriBits(CriBits *r, int n)
{
    int sh, v, a;
    if (r->bits < n) RefillCriBits(r);
    if (n > r->bits) {
        r->pos += r->bits;
        r->bits = 0;
        return 0;
    }
    a = r->acc;
    sh = r->bits - n;
    v = data_0209e424[n];
    r->bits = sh;
    v &= a >> sh;
    r->pos += n;
    return v;
}

int GetCriBitsPos(CriBits *r) { return r->pos; }

int IsCriBitsEmpty(CriBits *r)
{
    if (r->src->vt->size(r->src, 1) == 0 && r->bits == 0 && r->avail == 0) return 1;
    return 0;
}

int func_0204e7a0(CriBits *r)
{
    int rem;
    int v;
    int mask;
    mask = data_0209e424[12];
    rem = GetCriBitsPos(r) & 7;
    if (rem) ReadCriBits(r, 8 - rem);
    v = ReadCriBits(r, 12);
    if (IsCriBitsEmpty(r) == 0) {
        do {
            if ((v & mask) == 0xfff) return 1;
            if (v == 0x8001000c) return 2;
            v = ReadCriBits(r, 4) | (v << 4);
        } while (IsCriBitsEmpty(r) == 0);
    }
    return -1;
}
