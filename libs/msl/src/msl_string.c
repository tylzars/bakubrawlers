/* MSL string / mem / wchar helpers, main 0x020122b0-0x02012504. */

typedef unsigned short wchar_t_;
typedef unsigned int size_t_;

typedef struct CharConv {
    int (*mbtowc)(wchar_t_ *, const char *, size_t_);
    int (*wctomb)(char *, wchar_t_);
} CharConv;

typedef struct Locale {
    void *unk0;
    void *unk4;
    CharConv *conv;
} Locale;

extern float data_020a5078;
extern Locale data_020a50d8;

extern void func_02015b34(char *, const char *, size_t_);
extern void func_02012504(void *, int, size_t_);

/* 0x020122b0: population count */
unsigned func_020122b0(unsigned x)
{
    x = x - ((x >> 1) & 0x55555555);
    x = (x & 0x33333333) + ((x >> 2) & 0x33333333);
    x = (x + (x >> 4)) & 0x0f0f0f0f;
    x = x + (x >> 8);
    x = x + (x >> 16);
    return x & 0xff;
}

/* 0x020122f4: strcpy */
char *func_020122f4(char *dst, const char *src)
{
    char *ret = dst;
    while (*src) { *dst++ = *src++; }
    *dst = 0;
    return ret;
}

/* 0x02012328: strlen */
int func_02012328(const char *s)
{
    int n = 0;
    while (s[n]) n++;
    return n;
}

/* 0x02012350: the quiet NaN constant as a double; the NAN(...) text argument is ignored */
double func_02012350(const char *tag)
{
    return data_020a5078;
}

/* 0x02012368 */
int func_02012368(wchar_t_ *pwc, const char *s, size_t_ n)
{
    return data_020a50d8.conv->mbtowc(pwc, s, n);
}

/* 0x02012384: mbtowc */
int func_02012384(wchar_t_ *pwc, const char *s, size_t_ n)
{
    if (s == 0) return 0;
    if (n == 0) return -1;
    if (pwc) *pwc = *(unsigned char *)s;
    if (*s == 0) return 0;
    return 1;
}

/* 0x020123bc: wctomb */
int func_020123bc(char *s, wchar_t_ wc)
{
    if (s == 0) return 0;
    *s = wc;
    return 1;
}

/* 0x020123d0 */
int func_020123d0(char *s, wchar_t_ wc)
{
    return data_020a50d8.conv->wctomb(s, wc);
}

/* 0x020123ec: wcstombs */
size_t_ func_020123ec(char *dst, const wchar_t_ *src, size_t_ n)
{
    char tmp[4];
    size_t_ len = 0;
    size_t_ cl;
    if (dst == 0 || src == 0) return 0;
    while (1) {
        if (*src == 0) { dst[len] = 0; break; }
        cl = func_020123d0(tmp, *src++);
        if (len + cl > n) break;
        func_02015b34(dst + len, tmp, cl);
        len += cl;
        if (len > n) break;
    }
    return len;
}

/* 0x02012464: forward byte copy */
void *func_02012464(void *dst, const char *src, size_t_ n)
{
    char *d = dst;
    if (n) {
        do { *d++ = *src++; } while (--n);
    }
    return dst;
}

/* 0x02012484: memset */
void *func_02012484(void *dst, int c, size_t_ n)
{
    func_02012504(dst, c, n);
    return dst;
}

/* 0x02012498: memchr */
void *func_02012498(const void *p, int c, size_t_ n)
{
    const unsigned char *s = p;
    unsigned char ch = c;
    if (n) {
        do {
            if (*s++ == ch) return (void *)(s - 1);
        } while (--n);
    }
    return 0;
}

/* 0x020124c4: memcmp */
int func_020124c4(const void *a, const void *b, size_t_ n)
{
    const unsigned char *p = a, *q = b;
    if (n) {
        do {
            if (*p++ != *q++) return (p[-1] < q[-1]) ? -1 : 1;
        } while (--n);
    }
    return 0;
}

/* 0x02012504: fill memory with a byte */
void func_02012504(void *dst, int val, size_t_ n)
{
    unsigned char *p = dst;
    unsigned v = (unsigned char)val;
    if (n >= 32) {
        unsigned a = -(unsigned)p & 3;
        if (a) {
            n -= a;
            do { *p++ = (unsigned char)v; } while (--a);
        }
        if (v) { v = (v << 24) | (v << 16) | (v << 8) | v; }
        {
            unsigned blocks = n >> 5;
            unsigned *wp = (unsigned *)p;
            if (blocks) {
                do {
                    wp[0] = v; wp[1] = v; wp[2] = v; wp[3] = v;
                    wp[4] = v; wp[5] = v; wp[6] = v; wp[7] = v;
                    wp += 8;
                } while (--blocks);
            }
            blocks = (n & 0x1f) >> 2;
            if (blocks) {
                do { *wp++ = v; } while (--blocks);
            }
            p = (unsigned char *)wp;
        }
        n &= 3;
    }
    if (n == 0) return;
    do { *p++ = (unsigned char)v; } while (--n);
}
