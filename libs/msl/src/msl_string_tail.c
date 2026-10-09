/* MSL string.h and sscanf glue, main 0x02015950-0x02015c74 (func_02015c74, the scanf core, is not done). */

typedef unsigned int size_t_;

typedef struct StrReadCtx {
    const char *p;
    int eof;
} StrReadCtx;

extern int func_02014bfc(int (*)(void *, int, int), void *, const char *, char *, int);
extern int func_02015950(void *, int, int);
extern int func_020159d4(const char *, const char *, char *);

/* 0x02015950: scanf string reader callback (get / unget / test eof) */
int func_02015950(void *arg, int ch, int op)
{
    StrReadCtx *c = arg;
    switch (op) {
    case 0: {
        char v = *c->p;
        if (v == 0) {
            c->eof = 1;
            return -1;
        }
        c->p++;
        return (unsigned char)v;
    }
    case 1:
        if (c->eof == 0) {
            c->p--;
        } else {
            c->eof = 0;
        }
        return ch;
    case 2:
        return c->eof;
    }
    return 0;
}

/* 0x020159d4: vsscanf */
int func_020159d4(const char *s, const char *fmt, char *ap)
{
    StrReadCtx c;
    c.p = s;
    if (s == 0 || *s == 0) return -1;
    c.eof = 0;
    return func_02014bfc(func_02015950, &c, fmt, ap, 0);
}

/* 0x02015a28: sscanf */
int func_02015a28(const char *s, const char *fmt, ...)
{
    char *ap = (char *)((unsigned)&fmt & ~3) + 4;
    return func_020159d4(s, fmt, ap);
}

/* 0x02015a50: strlen */
int func_02015a50(const char *s)
{
    int n = -1;
    do { n++; } while (*s++);
    return n;
}

/* 0x02015a6c */
char *func_02015a6c(char *dst, const char *src)
{
    unsigned char c;
    unsigned sa = (unsigned)src & 3;
    unsigned da = (unsigned)dst & 3;
    unsigned k;
    const unsigned char *s = (const unsigned char *)src;
    unsigned char *d = (unsigned char *)dst;
    if (da == sa) {
        if (sa != 0) {
            c = *s;
            *d = c;
            if (c == 0) return dst;
            k = 3 - sa;
            if (k != 0) {
                do {
                    c = *++s;
                    *++d = c;
                    if (c == 0) return dst;
                } while (--k);
            }
            d++;
            s++;
        }
        {
            unsigned w = *(const unsigned *)s;
            if (!(((w + 0xfefefeff) & ~w) & 0x80808080)) {
                d -= 4;
                do {
                    unsigned t;
                    *(unsigned *)(d += 4) = w;
                    w = *(const unsigned *)(s += 4);
                    t = w + 0xfefefeff;
                    t = t & ~w;
                    if (t & 0x80808080) break;
                } while (1);
                d += 4;
            }
        }
    }
    c = *s;
    *d = c;
    if (c == 0) return dst;
    do {
        c = *++s;
        *++d = c;
    } while (c != 0);
    return dst;
}

/* 0x02015b34: strncpy */
char *func_02015b34(char *dst, const char *src, size_t_ n)
{
    char *d = dst;
    if (n == 0) return dst;
    do {
        if ((*d++ = *src++) == 0) {
            if (--n == 0) return dst;
            do { *d++ = 0; } while (--n);
            return dst;
        }
    } while (--n);
    return dst;
}

/* 0x02015b84: strcat */
char *func_02015b84(char *dst, const char *src)
{
    char *d = dst;
    while (*d++) {}
    d--;
    while ((*d++ = *src++) != 0) {}
    return dst;
}

/* 0x02015bb4: strncat */
char *func_02015bb4(char *dst, const char *src, size_t_ n)
{
    char *d = dst;
    while (*d++) {}
    d--;
    if (n) {
        do {
            if ((*d++ = *src++) == 0) { d--; break; }
        } while (--n);
    }
    *d = 0;
    return dst;
}

/* 0x02015c04: strncmp */
int func_02015c04(const char *a, const char *b, size_t_ n)
{
    if (n) {
        do {
            unsigned char cb = *b++;
            unsigned char ca = *a++;
            if (ca != cb) return ca - cb;
            if (ca == 0) break;
        } while (--n);
    }
    return 0;
}

/* 0x02015c38: strchr */
char *func_02015c38(const char *s, int c)
{
    char ch = c;
    char v = *s++;
    if (v) {
        do {
            if (v == ch) return (char *)(s - 1);
            v = *s++;
        } while (v);
    }
    if (ch != 0) return 0;
    return (char *)(s - 1);
}
