/* ADX header parse, 0x0204dff4-0x0204e330 */
typedef struct CriHdrA {           /* 0x14 bytes */
    short w0, w2;
    int i4, i8, ic, i10;
} CriHdrA;
typedef struct CriHdrB {           /* 0xc bytes */
    short w0, w2;
    signed char b4, b5, b6, b7, b8, b9, ba, bb;
} CriHdrB;
typedef struct CriHdr {
    signed char b0, b1, b2, nb;    /* 0x00 */
    int i4;
    int i8;
    unsigned short wc;
    short na;                      /* 0x0e */
    CriHdrA a[1];                  /* 0x10 */
    CriHdrB b[2];                  /* 0x24 */
    signed char b3c, b3d;
    short pad3e;
} CriHdr;

extern void func_0204dff4(unsigned char *, CriHdr *);
extern void func_0204e080(unsigned char *, CriHdr *);
extern void CriHdrARead(unsigned char *, CriHdrA *);
extern void CriHdrBRead(unsigned char *, CriHdrB *);

void func_0204dff4(unsigned char *s, CriHdr *d)
{
    ((signed char *)d)[0] = ((signed char *)s)[0];
    ((signed char *)d)[1] = ((signed char *)s)[1];
    ((signed char *)d)[2] = ((signed char *)s)[2];
    ((signed char *)d)[3] = ((signed char *)s)[3];
    d->i4 = (s[4] << 24) | (s[5] << 16) | (s[6] << 8) | s[7];
    d->i8 = (s[8] << 24) | (s[9] << 16) | (s[10] << 8) | s[11];
    d->wc = (s[12] << 8) | s[13];
    d->b3c = ((signed char *)s)[14];
    d->b3d = ((signed char *)s)[15];
}

void func_0204e080(unsigned char *s, CriHdr *d)
{
    d->na = (s[2] << 8) | s[3];
}

void CriHdrARead(unsigned char *s, CriHdrA *d)
{
    d->w0 = (s[0] << 8) | s[1];
    d->w2 = (s[2] << 8) | s[3];
    d->i4 = (s[4] << 24) | (s[5] << 16) | (s[6] << 8) | s[7];
    d->i8 = (s[8] << 24) | (s[9] << 16) | (s[10] << 8) | s[11];
    d->ic = (s[12] << 24) | (s[13] << 16) | (s[14] << 8) | s[15];
    d->i10 = (s[16] << 24) | (s[17] << 16) | (s[18] << 8) | s[19];
}

void CriHdrBRead(unsigned char *ss, CriHdrB *d)
{
    signed char *s = (signed char *)ss;
    d->w0 = (ss[0] << 8) | ss[1];
    if (d->w0 > 0) d->w2 = (ss[2] << 8) | ss[3];
    d->b4 = s[4];
    if (d->b4 > 0) d->b5 = s[5];
    d->b6 = s[6];
    if (d->b6 > 0) d->b7 = s[7];
    d->b8 = s[8];
    if (d->b8 > 0) d->b9 = s[9];
    d->ba = s[10];
    if (d->ba > 0) d->bb = s[11];
}

/* dst == 0: stores only the length (*outsize). The A loop reads every entry from the same src (src is not moved),
   so the B entries start right after the 4-byte count. */
int CriHdrParse(unsigned char *src, int size, int *outsize, CriHdr *dst)
{
    long n;
    long i;
    if (outsize) *outsize = 0;
    if (size < 4) return -1;
    if ((unsigned short)((src[0] << 8) | src[1]) != 0x8000) return -4;
    n = (src[2] << 8) | src[3];
    if (outsize) *outsize = n;
    if (dst == 0) return 0;
    if (size < n + 4) return -2;
    if (n - 6 < 0x10) return -2;
    func_0204dff4(src + 4, dst);
    src += 0x14;
    n -= 0x16;
    if (dst->b3c == 4) {
        src += 0xc;
        n -= 0xc;
    }
    if (n < 4) return 0;
    func_0204e080(src, dst);
    src += 4;
    if (n < dst->na * 0x14) return 0;
    for (i = 0; i < dst->na; i++) CriHdrARead(src, &dst->a[i]);
    n -= dst->na * 0x14;
    for (i = 0; i < dst->nb; i++) {
        if (n < 0xc) return 0;
        CriHdrBRead(src, &dst->b[i]);
        src += 0xc;
        n -= 0xc;
    }
    return 0;
}

