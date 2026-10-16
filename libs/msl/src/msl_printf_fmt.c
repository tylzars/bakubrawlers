// func_02013df0's switch depends on the build host's x87 precision: mwcc merges cases into a jump table while
// count/range >= 0.4, computed with x87 division, and here the 'X'..'u' step is exactly 12/30. With Windows' 53-bit
// precision (the pinned wibo, tools/build_wibo.sh) the table runs 'd'..'u' as in the original; with 64-bit
// precision the quotient falls just below 0.4 and the table stops at 'o'.
typedef struct FmtSpec {
    unsigned char justify;   /* 0 left, 1 right, 2 right with zero fill */
    unsigned char sign;      /* 0 none, 1 '+', 2 ' ' */
    unsigned char prec_set;
    unsigned char alt;
    unsigned char len;       /* length modifier */
    unsigned char conv;      /* conversion char, 0xff if invalid */
    int width;
    int prec;
} FmtSpec;

typedef struct DecDigits {
    char sign;
    unsigned char unk1;
    short exp;
    unsigned char len;
    unsigned char text[35];
} DecDigits;

typedef struct DecForm {
    unsigned char mode;
    short ndigits;
} DecForm;

extern const unsigned short data_020903a8[];
#define ISDIGIT(c) (((c) < 0 || (c) >= 128) ? 0 : (data_020903a8[c] & 8))
#define ISUPPER(c) (((c) < 0 || (c) >= 128) ? 0 : (data_020903a8[c] & 0x200))
typedef struct B4 { char c[4]; } B4;
typedef struct B5 { char c[5]; } B5;
extern const B5 data_020a52a8, data_020a52b0, data_020a52c0, data_020a52c8;
extern const B4 data_020a52b8, data_020a52bc, data_020a52d0, data_020a52d4;
extern void func_0201cf1c(DecForm *, double, DecDigits *);
extern void func_02013568(DecDigits *, int);
extern const char *func_020125b4(const char *, char **, FmtSpec *);
extern void *memcpy(void *, const void *, unsigned);
extern const char dA[], dB[], dC[], dD[];


char *func_0201368c(double x, char *end, FmtSpec spec)
{
    int sign;
    int alt;
    int prec;
    int conv;
    char *p;
    char *q;
    DecForm f;
    DecDigits d;
    int ex, n, i, k;
    char esign;
    int whole;   /* digits before the point (%f) */
    int frac;    /* digits after the point (%f) */
    alt = spec.alt;
    sign = spec.sign;
    prec = spec.prec;
    conv = spec.conv;
    if (prec > 509) return 0;
    f.mode = 0;
    f.ndigits = 32;
    func_0201cf1c(&f, x, &d);
    q = d.text + d.len;
    while (d.len > 1 && *--q == '0') {
        d.len--;
        d.exp++;
    }
    switch (d.text[0]) {
    case '0':
        d.exp = 0;
        break;
    case 'I':
        if (x < 0.0) {
            p = end - 5;
            if (ISUPPER(conv)) __memcpy(p, &data_020a52a8, 5);
            else __memcpy(p, &data_020a52b0, 5);
        } else {
            p = end - 4;
            if (ISUPPER(conv)) __memcpy(p, &data_020a52b8, 4);
            else __memcpy(p, &data_020a52bc, 4);
        }
        return p;
    case 'N':
        if (d.sign) {
            p = end - 5;
            if (ISUPPER(conv)) __memcpy(p, &data_020a52c0, 5);
            else __memcpy(p, &data_020a52c8, 5);
        } else {
            p = end - 4;
            if (ISUPPER(conv)) __memcpy(p, &data_020a52d0, 4);
            else __memcpy(p, &data_020a52d4, 4);
        }
        return p;
    }
    p = end;
    d.exp += d.len - 1;
    *--p = 0;
    switch (conv) {
    case 'g':
    case 'G':
        if (d.len > prec) func_02013568(&d, prec);
        if (d.exp < -4 || d.exp >= prec) {
            if (alt) prec--;
            else prec = d.len - 1;
            if (spec.conv == 'g') spec.conv = 'e'; else spec.conv = 'E';
            goto do_e;
        } else {
            if (alt) {
                prec -= d.exp + 1;
            } else {
                prec = d.len - (d.exp + 1);
                if (prec < 0) prec = 0;
            }
            goto do_f;
        }
    case 'e':
    case 'E':
    do_e:
        if (d.len > prec + 1) func_02013568(&d, prec + 1);
        ex = d.exp;
        esign = '+';
        n = 0;
        if (ex < 0) {
            ex = -ex;
            esign = '-';
        }
        while (ex != 0 || n < 2) {
            *--p = ex % 10 + '0';
            ex /= 10;
            n++;
        }
        *--p = esign;
        *--p = spec.conv;
        if ((end - p) + prec > 509) return 0;
        if (d.len < prec + 1) {
            for (i = prec + 2 - d.len; --i != 0; ) *--p = '0';
        }
        q = d.text + d.len;
        for (i = d.len - 1; i != 0; i--) *--p = *--q;
        if (prec != 0 || alt != 0) *--p = '.';
        *--p = d.text[0];
        if (d.sign) *--p = '-';
        else if (sign == 1) *--p = '+';
        else if (sign == 2) *--p = ' ';
        break;
    case 'f':
    case 'F':
    do_f:
        frac = d.len - d.exp - 1;
        if (frac < 0) frac = 0;
        if (frac > prec) {
            func_02013568(&d, d.len - (frac - prec));
            frac = d.len - d.exp - 1;
            if (frac < 0) frac = 0;
        }
        whole = d.exp + 1;
        if (whole < 0) whole = 0;
        if (whole + frac > 509) return 0;
        q = d.text + d.len;
        for (i = 0; i < prec - frac; i++) *--p = '0';
        for (i = 0; i < frac && i < d.len; i++) *--p = *--q;
        for (; i < frac; i++) *--p = '0';
        if (prec != 0 || alt != 0) *--p = '.';
        if (whole != 0) {
            for (k = 0; k < whole - d.len; k++) *--p = '0';
            for (; k < whole; k++) *--p = *--q;
        } else {
            *--p = '0';
        }
        if (d.sign) *--p = '-';
        else if (sign == 1) *--p = '+';
        else if (sign == 2) *--p = ' ';
        break;
    }
    return p;
}


typedef unsigned int size_t_;
extern char *func_02015c38(const char *, int);
extern size_t_ func_02015a50(const char *);
extern void *func_02012498(const void *, int, size_t_);
extern int func_020123ec(char *, const void *, size_t_);
extern char *func_02012b44(int, char *, FmtSpec);
extern char *func_02012d90(long long, char *, FmtSpec);
extern char *func_0201368c(double, char *, FmtSpec);
extern char *func_0201308c(double, char *, FmtSpec);
extern const char data_020a52d8[], data_020a52dc[];

size_t_ func_02013df0(int (*cb)(void *, const char *, size_t_), void *arg, const char *fmt, char *ap)
{
    int count, v, len, n;
    FmtSpec spec;
    long long ll;
    double d;
    char *s;
    const char *pct;
    char fill = ' ';
    char space;
    char buf[512];
    int total = 0;
    while (*fmt) {
        pct = func_02015c38(fmt, '%');
        if (pct == 0) {
            n = func_02015a50(fmt);
            total += n;
            if (n == 0) break;
            if (cb(arg, fmt, n)) break;
            return -1;
        }
        n = pct - fmt;
        total += n;
        if (n) {
            if (!cb(arg, fmt, n)) return -1;
        }
        fmt = func_020125b4(pct, &ap, &spec);
        switch (spec.conv) {
        case 'd':
        case 'i':
            if (spec.len == 3) {
                v = *(int *)((ap += 4) - 4);
            } else if (spec.len == 4) {
                ll = *(long long *)((ap += 8) - 8);
            } else if (spec.len == 6) {
                ll = *(long long *)((ap += 8) - 8);
            } else if (spec.len == 7) {
                v = *(unsigned *)((ap += 4) - 4);
            } else if (spec.len == 8) {
                v = *(long *)((ap += 4) - 4);
            } else {
                v = *(int *)((ap += 4) - 4);
            }
            if (spec.len == 2) v = (short)v;
            if (spec.len == 1) v = (signed char)v;
            if (spec.len == 4 || spec.len == 6) {
                s = func_02012d90(ll, buf + 512, spec);
                if (s == 0) goto bad;
            } else {
                s = func_02012b44(v, buf + 512, spec);
                if (s == 0) goto bad;
            }
            len = (buf + 511) - s;
            break;
        case 'o':
        case 'u':
        case 'x':
        case 'X':
            if (spec.len == 3) {
                v = *(int *)((ap += 4) - 4);
            } else if (spec.len == 4) {
                ll = *(long long *)((ap += 8) - 8);
            } else if (spec.len == 6) {
                ll = *(long long *)((ap += 8) - 8);
            } else if (spec.len == 7) {
                v = *(unsigned *)((ap += 4) - 4);
            } else if (spec.len == 8) {
                v = *(long *)((ap += 4) - 4);
            } else {
                v = *(int *)((ap += 4) - 4);
            }
            if (spec.len == 2) v = (unsigned short)v;
            if (spec.len == 1) v = (unsigned char)v;
            if (spec.len == 4 || spec.len == 6) {
                s = func_02012d90(ll, buf + 512, spec);
                if (s == 0) goto bad;
            } else {
                s = func_02012b44(v, buf + 512, spec);
                if (s == 0) goto bad;
            }
            len = (buf + 511) - s;
            break;
        case 'e':
        case 'E':
        case 'f':
        case 'F':
        case 'g':
        case 'G':
            if (spec.len == 9) {
                ap += 8;
                d = *(long double *)(ap - 8);
            } else {
                ap += 8;
                d = *(double *)(ap - 8);
            }
            s = func_0201368c(d, buf + 512, spec);
            if (s == 0) goto bad;
            len = (buf + 511) - s;
            break;
        case 'a':
        case 'A':
            if (spec.len == 9) {
                ap += 8;
                d = *(long double *)(ap - 8);
            } else {
                ap += 8;
                d = *(double *)(ap - 8);
            }
            s = func_0201308c(d, buf + 512, spec);
            if (s == 0) goto bad;
            len = (buf + 511) - s;
            break;
        case 's':
            if (spec.len == 5) {
                const void *w = *(const void **)((ap += 4) - 4);
                if (w == 0) w = data_020a52d8;
                if (func_020123ec(buf, w, 512) < 0) goto bad;
                s = buf;
            } else {
                s = *(char **)((ap += 4) - 4);
            }
            if (s == 0) s = (char *)data_020a52dc;
            if (spec.alt) {
                len = (unsigned char)*s++;
                if (spec.prec_set) {
                    if (len > spec.prec) len = spec.prec;
                }
            } else if (spec.prec_set) {
                char *e;
                len = spec.prec;
                e = func_02012498(s, 0, len);
                if (e) len = e - s;
            } else {
                len = func_02015a50(s);
            }
            break;
        case 'n': {
            void *p = *(void **)((ap += 4) - 4);
            switch (spec.len) {
            case 0: *(int *)p = total; break;
            case 2: *(short *)p = total; break;
            case 3: *(int *)p = total; break;
            case 6: *(long long *)p = total; break;
            case 7: *(int *)p = total; break;
            case 8: *(int *)p = total; break;
            case 4: *(long long *)p = total; break;
            }
            continue;
        }
        case 'c':
            s = buf;
            buf[0] = *(int *)((ap += 4) - 4);
            len = 1;
            break;
        case '%':
            buf[0] = '%';
            s = buf;
            len = 1;
            break;
        case 0xff:
        default:
        bad:
            n = func_02015a50(pct);
            if (n) {
                if (!cb(arg, pct, n)) return -1;
            }
            return total + n;
        }
        count = len;
        if (spec.justify != 0) {
            fill = (spec.justify == 2) ? '0' : ' ';
            if ((*s == '+' || *s == '-' || *s == ' ') && fill == '0') {
                if (!cb(arg, s, 1)) return -1;
                s++;
                len--;
            }
            if (count < spec.width) {
                do {
                    if (!cb(arg, &fill, 1)) return -1;
                } while (++count < spec.width);
            }
        }
        if (len) {
            if (!cb(arg, s, len)) return -1;
        }
        if (spec.justify == 0 && count < spec.width) {
            do {
                space = ' ';
                if (!cb(arg, &space, 1)) return -1;
            } while (++count < spec.width);
        }
        total += count;
    }
    return total;
}
