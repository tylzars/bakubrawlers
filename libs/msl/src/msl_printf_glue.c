/* MSL printf glue, main 0x02014614-0x02014704 (the formatter itself, func_02013df0, is in msl_printf_fmt.c). */

typedef unsigned int size_t_;

typedef struct StrWriteCtx {
    char *buf;
    size_t_ size;
    size_t_ pos;
} StrWriteCtx;

extern void *CopyBytes(void *, const void *, size_t_);
extern size_t_ func_02013df0(int (*)(void *, const char *, size_t_), void *, const char *, char *);

/* 0x02014614: formatter output callback, appends to a bounded buffer */
int WriteToStrBuffer(void *arg, const char *src, size_t_ n)
{
    StrWriteCtx *c = arg;
    size_t_ pos = c->pos;
    size_t_ size = c->size;
    if (pos + n > size) n = size - pos;
    CopyBytes(c->buf + pos, src, n);
    c->pos += n;
    return 1;
}

/* 0x02014658: vsnprintf */
size_t_ vsnprintf(char *buf, size_t_ size, const char *fmt, char *ap)
{
    StrWriteCtx c;
    size_t_ n;
    c.buf = buf;
    c.size = size;
    c.pos = 0;
    n = func_02013df0(WriteToStrBuffer, &c, fmt, ap);
    if (buf == 0) return n;
    if (n < size) {
        buf[n] = 0;
        return n;
    }
    if (size) (buf + size)[-1] = 0;
    return n;
}

/* 0x020146c0: vsprintf */
size_t_ vsprintf(char *buf, const char *fmt, char *ap)
{
    return vsnprintf(buf, -1, fmt, ap);
}

/* 0x020146d8 */
int sprintf(char *buf, const char *fmt, ...)
{
    char *ap = (char *)((unsigned)&fmt & ~3) + 4;
    return vsnprintf(buf, -1, fmt, ap);
}
