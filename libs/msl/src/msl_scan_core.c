// non-matching: 9 words in the format-side whitespace skip differ in register choice (zero/one constants).
/* MSL scanf core: walks the format string, reads input through the reader
   callback rd(ctx, ch, op) (op 0 = get, 1 = unget, 2 = test eof), stores converted values
   through ap. flag != 0 selects the "secure"/length-checked mode (%c/%s/%[ take a buffer size).
   Returns the number of stored conversions, or -1 if input ended before any conversion.
   Main 0x02014bfc, 0xd54 bytes. Types: ScanSpec from msl_scan_spec.c (spec parser is
   func_02014704). func_020172a8 / func_02016ec0 are the 64-bit / 32-bit integer readers
   (base, width, rd, ctx, &consumed, &neg, &overflow); func_02015c74 is the float reader. */
typedef struct ScanSpec {
    unsigned char suppress;
    unsigned char width_set;
    unsigned char len;
    unsigned char conv;
    int width;
    unsigned char set[32];
} ScanSpec;

typedef int (*ReadFn)(void *, int, int);

extern const unsigned short data_020903a8[];
extern float data_020a5078;
#define ISSPACEM(c) (((c) < 0 || (c) >= 128) ? 0 : (data_020903a8[c] & 0x100))
static inline int ISSPACE(int c)
{
    return (c < 0 || c >= 128) ? 0 : (data_020903a8[c] & 0x100);
}

extern const char *func_02014704(const char *, ScanSpec *);
extern long long func_020172a8(int, int, ReadFn, void *, int *, int *, int *);
extern int func_02016ec0(int, int, ReadFn, void *, int *, int *, int *);
extern double func_02015c74(int, ReadFn, void *, int *, int *);
extern double func_0201f17c(float);
extern float func_0201da44(double);
extern int func_02012368(char *, const char *, int);

int func_02014bfc(ReadFn rd, void *ctx, const char *fmt, char *ap, int flag)
{
    ScanSpec spec;
    int consumed, neg, ovf;
    char ch;
    int stored;
    int fc;
    int attempted;
    int tmp32;
    long long tmp64;
    int stop;
    char *dst0c;
    char *dst0b;
    long long sv64;
    int sv32;
    int count;
    void *dst;
    int base;
    unsigned buflen;
    int room;
    int c;

    sv64 = 0;
    stop = 0;
    count = 0;
    stored = 0;
    attempted = 0;
    if (*fmt == 0) goto done;
    do {
        if (ISSPACEM((int)*fmt)) {
            do {
                fmt++;
            } while (ISSPACEM(*fmt));
            if (!stop) {
                while (ISSPACE(ch = rd(ctx, 0, 0))) {
                    count++;
                }
                rd(ctx, ch, 1);
            }
            goto next;
        }
        fc = *fmt;
        if (fc != '%' && !stop) {
            ch = rd(ctx, 0, 0);
            if ((unsigned char)fc != ch) {
                rd(ctx, ch, 1);
                if (flag == 0) goto done;
                stop = 1;
                fmt++;
                goto next;
            }
            count++;
            fmt++;
            goto next;
        }
        fmt = func_02014704(fmt, &spec);
        if (spec.suppress == 0 && spec.conv != '%') {
            dst = *(void **)((ap += 4) - 4);
        } else {
            dst = 0;
        }
        if (spec.conv != 'n' && !stop) {
            if (rd(ctx, 0, 2)) {
                if (flag == 0) goto done;
                stop = 1;
            }
        }
        switch (spec.conv) {
        case 'd':
            base = 10;
            goto do_signed;
        case 'i':
            base = 0;
        do_signed:
            if (stop) {
                sv32 = 0;
                sv64 = 0;
            } else {
                if (spec.len == 7 || spec.len == 4) {
                    tmp64 = func_020172a8(base, spec.width, rd, ctx, &consumed, &neg, &ovf);
                } else {
                    tmp32 = func_02016ec0(base, spec.width, rd, ctx, &consumed, &neg, &ovf);
                }
                if (consumed == 0) {
                    if (flag == 0) goto done;
                    stop = 1;
                    sv32 = 0;
                    sv64 = 0;
                } else {
                    count += consumed;
                    if (spec.len == 7 || spec.len == 4) {
                        if (neg) {
                            sv64 = -tmp64;
                        } else {
                            sv64 = tmp64;
                        }
                    } else {
                        if (neg) {
                            sv32 = -tmp32;
                        } else {
                            sv32 = tmp32;
                        }
                    }
                }
            }
            if (dst) {
                switch (spec.len) {
                case 0: *(int *)dst = sv32; break;
                case 1: *(char *)dst = sv32; break;
                case 2: *(short *)dst = sv32; break;
                case 3: *(int *)dst = sv32; break;
                case 4: *(long long *)dst = sv64; break;
                case 5: *(int *)dst = sv32; break;
                case 6: *(int *)dst = sv32; break;
                case 7: *(long long *)dst = sv64; break;
                }
                if (!stop) stored++;
            }
            attempted++;
            break;
        case 'o':
            base = 8;
            goto do_unsigned;
        case 'u':
            base = 10;
            goto do_unsigned;
        case 'X':
        case 'x':
            base = 16;
        do_unsigned:
            if (stop) {
                tmp32 = 0;
                tmp64 = 0;
            } else {
                if (spec.len == 7 || spec.len == 4) {
                    tmp64 = func_020172a8(base, spec.width, rd, ctx, &consumed, &neg, &ovf);
                } else {
                    tmp32 = func_02016ec0(base, spec.width, rd, ctx, &consumed, &neg, &ovf);
                }
                if (consumed == 0) {
                    if (flag == 0) goto done;
                    stop = 1;
                    tmp32 = 0;
                    tmp64 = 0;
                } else {
                    count += consumed;
                    if (neg) {
                        if (spec.len == 7) {
                            tmp64 = -tmp64;
                        } else {
                            tmp32 = -tmp32;
                        }
                    }
                }
            }
            if (dst) {
                switch (spec.len) {
                case 0: *(int *)dst = tmp32; break;
                case 1: *(char *)dst = tmp32; break;
                case 2: *(short *)dst = tmp32; break;
                case 3: *(int *)dst = tmp32; break;
                case 4: *(long long *)dst = tmp64; break;
                case 5: *(int *)dst = tmp32; break;
                case 6: *(int *)dst = tmp32; break;
                case 7: *(long long *)dst = tmp64; break;
                }
                if (!stop) stored++;
            }
            attempted++;
            break;
        case 'a': case 'A': case 'e': case 'E': case 'f': case 'F': case 'g': case 'G': {
            double x;
            if (stop) {
                x = func_0201f17c(data_020a5078);
            } else {
                x = func_02015c74(spec.width, rd, ctx, &consumed, &ovf);
                if (consumed == 0) {
                    if (flag == 0) goto done;
                    stop = 1;
                    x = func_0201f17c(data_020a5078);
                } else {
                    count += consumed;
                }
            }
            if (dst) {
                switch (spec.len) {
                case 0: *(float *)dst = func_0201da44(x); break;
                case 8: *(double *)dst = x; break;
                case 9: *(double *)dst = x; break;
                }
                if (!stop) stored++;
            }
            attempted++;
            break;
        }
        case 'c': {
            char *p;
            if (spec.width_set == 0) spec.width = 1;
            if (dst) {
                if (flag) {
                    room = 1;
                    buflen = *(int *)((ap += 4) - 4);
                }
                consumed = 0;
                if (stop) {
                    if (buflen != 0) *(char *)dst = 0;
                    goto next;
                }
                p = dst;
                dst0c = p;
                while (spec.width-- != 0 &&
                       (flag == 0 || (room = (buflen > consumed)) != 0) &&
                       (c = rd(ctx, 0, 0)) != -1) {
                    ch = c;
                    if (spec.len != 10) {
                        *p++ = ch;
                    } else {
                        func_02012368(p, &ch, 1);
                        p += 1;
                    }
                    consumed++;
                }
                ch = c;
                if (consumed == 0 || (flag && !room)) {
                    if (flag == 0) goto done;
                    stop = 1;
                    if (buflen) *dst0c = 0;
                    goto next;
                }
                count += consumed;
                stored++;
            } else {
                consumed = 0;
                while (spec.width-- != 0 && (c = rd(ctx, 0, 0)) != -1) {
                    ch = c;
                    consumed++;
                }
                ch = c;
                if (consumed == 0) goto done;
            }
            attempted++;
            break;
        }
        case '%':
            if (!stop) {
                while (ISSPACE(ch = rd(ctx, 0, 0))) {
                    count++;
                }
                if (ch != '%') {
                    rd(ctx, ch, 1);
                    if (flag == 0) goto done;
                    stop = 1;
                } else {
                    count++;
                }
            }
            break;
        case 's':
            if (!stop) {
                ch = rd(ctx, 0, 0);
                while (ISSPACE(ch)) {
                    count++;
                    ch = rd(ctx, 0, 0);
                }
                rd(ctx, ch, 1);
            }
        case '[': {
            char *p;
            if (dst) {
                if (flag) {
                    room = 1;
                    buflen = *(int *)((ap += 4) - 4) - 1;
                }
                consumed = 0;
                if (stop) {
                    if (buflen != 0) *(char *)dst = 0;
                    goto next;
                }
                p = dst;
                dst0b = p;
                while (spec.width-- != 0 &&
                       (flag == 0 || (room = (buflen >= consumed)) != 0) &&
                       (c = rd(ctx, 0, 0)) != -1) {
                    ch = c;
                    if (!(spec.set[(unsigned char)ch >> 3] & (1 << (ch & 7)))) break;
                    if (spec.len != 10) {
                        *p++ = ch;
                    } else {
                        func_02012368(p, &ch, 1);
                        p += 2;
                    }
                    consumed++;
                }
                ch = c;
                if (consumed == 0 || (flag && !room)) {
                    rd(ctx, ch, 1);
                    if (flag == 0) goto done;
                    stop = 1;
                    if (buflen) *dst0b = 0;
                    goto next;
                }
                count += consumed;
                if (spec.len == 10) {
                    *(short *)p = 0;
                } else {
                    *p = 0;
                }
                stored++;
            } else {
                consumed = 0;
                while (spec.width-- != 0 && (c = rd(ctx, 0, 0)) != -1) {
                    ch = c;
                    if (!(spec.set[(unsigned char)ch >> 3] & (1 << (ch & 7)))) break;
                    consumed++;
                }
                ch = c;
                if (consumed == 0) {
                    rd(ctx, ch, 1);
                    goto next;
                }
                count += consumed;
            }
            if (spec.width >= 0) {
                rd(ctx, ch, 1);
            }
            attempted++;
            break;
        }
        case 'n':
            if (dst) {
                switch (spec.len) {
                case 0: *(int *)dst = count; break;
                case 2: *(short *)dst = count; break;
                case 3: *(int *)dst = count; break;
                case 1: *(char *)dst = count; break;
                case 7: *(long long *)dst = count; break;
                }
            }
            break;
        case 0xff:
            goto done;
        default:
            goto done;
        }
    next:
        ;
    } while (*fmt != 0);
done:
    if (rd(ctx, 0, 2) != 0) {
        if (attempted == 0) return -1;
    }
    return stored;
}
