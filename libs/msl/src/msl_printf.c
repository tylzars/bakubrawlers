/* MSL printf core, main 0x020125b4-0x02014704 (format spec parser, integer and float conversion, formatter). */

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

extern const unsigned short data_020903a8[];   /* character class table */
#define ISDIGIT(c) (((c) < 0 || (c) >= 128) ? 0 : (data_020903a8[c] & 8))

/* 0x020125b4: parse one conversion spec after '%'; returns the char after it */
const char *func_020125b4(const char *fmt, char **ap, FmtSpec *out)
{
    FmtSpec s;
    const char *p = fmt + 1;
    int c = fmt[1];
    int more;
    int w;
    s.justify = 1;
    s.sign = 0;
    s.prec_set = 0;
    s.alt = 0;
    s.len = 0;
    s.width = 0;
    s.prec = 0;
    if (c == '%') {
        s.conv = c;
        *out = s;
        return p + 1;
    }
    do {
        more = 1;
        switch (c) {
        case '-': s.justify = 0; break;
        case '+': s.sign = 1; break;
        case ' ': if (s.sign != 1) s.sign = 2; break;
        case '#': s.alt = 1; break;
        case '0': if (s.justify != 0) s.justify = 2; break;
        default: more = 0; break;
        }
        if (!more) break;
        c = *++p;
    } while (1);
    if (c == '*') {
        w = *(int *)((*ap += 4) - 4);
        s.width = w;
        if (w < 0) {
            s.justify = 0;
            s.width = -w;
        }
        c = *++p;
    } else {
        while (ISDIGIT(c)) {
            s.width = s.width * 10 + (c - '0');
            c = *++p;
        }
    }
    if (s.width > 509) {
        s.conv = 0xff;
        *out = s;
        return p + 1;
    }
    if (c == '.') {
        c = *++p;
        s.prec_set = 1;
        if (c == '*') {
            s.prec = *(int *)((*ap += 4) - 4);
            c = *++p;
            if (s.prec < 0) s.prec_set = 0;
        } else {
            while (ISDIGIT(c)) {
                s.prec = s.prec * 10 + (c - '0');
                c = *++p;
            }
        }
    }
    more = 1;
    switch (c) {
    case 'h':
        s.len = 2;
        if (p[1] == 'h') { s.len = 1; c = *++p; }
        break;
    case 'l':
        s.len = 3;
        if (p[1] == 'l') { s.len = 4; c = *++p; }
        break;
    case 'L': s.len = 9; break;
    case 'j': s.len = 6; break;
    case 't': s.len = 8; break;
    case 'z': s.len = 7; break;
    default: more = 0; break;
    }
    if (more) c = *++p;
    s.conv = c;
    switch (c) {
    case 'd': case 'i': case 'o': case 'u': case 'x': case 'X':
        if (s.len == 9) s.conv = 0xff;
        else if (!s.prec_set) s.prec = 1;
        else if (s.justify == 2) s.justify = 1;
        break;
    case 'f': case 'F':
        if (s.len == 2 || s.len == 6 || s.len == 7 || s.len == 8 || s.len == 4) s.conv = 0xff;
        else if (!s.prec_set) s.prec = 6;
        break;
    case 'a': case 'A':
        if (!s.prec_set) s.prec = 13;
        if (s.len == 2 || s.len == 6 || s.len == 7 || s.len == 8 || s.len == 4 || s.len == 1) s.conv = 0xff;
        break;
    case 'g': case 'G':
        if (s.prec == 0) s.prec = 1;
        /* fall through */
    case 'e': case 'E':
        if (s.len == 2 || s.len == 6 || s.len == 7 || s.len == 8 || s.len == 4 || s.len == 1) s.conv = 0xff;
        else if (!s.prec_set) s.prec = 6;
        break;
    case 'p':
        s.conv = 'x';
        s.alt = 1;
        s.len = 3;
        s.prec = 8;
        break;
    case 'c':
        if (s.len == 3) s.len = 5;
        else if (s.prec_set != 0 || s.len != 0) s.conv = 0xff;
        break;
    case 's':
        if (s.len == 3) s.len = 5;
        else if (s.len != 0) s.conv = 0xff;
        break;
    case 'n':
        if (s.len == 9) s.conv = 0xff;
        break;
    default:
        s.conv = 0xff;
        break;
    }
    *out = s;
    return p + 1;
}


/* 0x02012b44: integer to digits, written backwards from end; returns start (0 if too long) */
char *func_02012b44(int val, char *end, FmtSpec spec)
{
    unsigned u, base;
    char *p;
    int digit, count;
    int neg = 0;
    u = val;
    neg = 0;
    p = end;
    *--p = 0;
    count = 0;
    if (val == 0 && spec.prec == 0 && !(spec.alt && spec.conv == 'o')) return p;
    switch (spec.conv) {
    case 'd': case 'i':
        base = 10;
        if (val < 0) {
            if (val != (int)0x80000000) u = -val;
            neg = 1;
        }
        break;
    case 'o': base = 8; spec.sign = 0; break;
    case 'u': base = 10; spec.sign = 0; break;
    case 'x': case 'X': base = 16; spec.sign = 0; break;
    }
    do {
        digit = u % base;
        u /= base;
        if (digit < 10) {
            digit += '0';
        } else {
            digit -= 10;
            if (spec.conv == 'x') digit += 'a';
            else digit += 'A';
        }
        *--p = digit;
        ++count;
    } while (u != 0);
    if (base == 8 && spec.alt && *p != '0') {
        *--p = '0';
        ++count;
    }
    if (spec.justify == 2) {
        spec.prec = spec.width;
        if (neg || spec.sign != 0) --spec.prec;
        if (base == 16 && spec.alt) spec.prec -= 2;
    }
    if (end - p + spec.prec > 509) return 0;
    while (count < spec.prec) {
        *--p = '0';
        ++count;
    }
    if (base == 16 && spec.alt) {
        *--p = spec.conv;
        *--p = '0';
    }
    if (neg) *--p = '-';
    else if (spec.sign == 1) *--p = '+';
    else if (spec.sign == 2) *--p = ' ';
    return p;
}

/* 0x02012d90: the same for 64-bit values */
char *func_02012d90(long long val, char *end, FmtSpec spec)
{
    unsigned long long u, base;
    char *p;
    int digit, count;
    int neg = 0;
    u = val;
    neg = 0;
    p = end;
    *--p = 0;
    count = 0;
    if (val == 0 && spec.prec == 0 && !(spec.alt && spec.conv == 'o')) return p;
    switch (spec.conv) {
    case 'd': case 'i':
        base = 10;
        if (val < 0) {
            if (val != (long long)0x8000000000000000ULL) u = -val;
            neg = 1;
        }
        break;
    case 'o': base = 8; spec.sign = 0; break;
    case 'u': base = 10; spec.sign = 0; break;
    case 'x': case 'X': base = 16; spec.sign = 0; break;
    }
    do {
        digit = u % base;
        u /= base;
        if (digit < 10) {
            digit += '0';
        } else {
            digit -= 10;
            if (spec.conv == 'x') digit += 'a';
            else digit += 'A';
        }
        *--p = digit;
        ++count;
    } while (u != 0);
    if (base == 8 && spec.alt && *p != '0') {
        *--p = '0';
        ++count;
    }
    if (spec.justify == 2) {
        spec.prec = spec.width;
        if (neg || spec.sign != 0) --spec.prec;
        if (base == 16 && spec.alt) spec.prec -= 2;
    }
    if (end - p + spec.prec > 509) return 0;
    while (count < spec.prec) {
        *--p = '0';
        ++count;
    }
    if (base == 16 && spec.alt) {
        *--p = spec.conv;
        *--p = '0';
    }
    if (neg) *--p = '-';
    else if (spec.sign == 1) *--p = '+';
    else if (spec.sign == 2) *--p = ' ';
    return p;
}


typedef struct B4 { char c[4]; } B4;
typedef struct B5 { char c[5]; } B5;
typedef struct B6 { char c[6]; } B6;

extern const B6 data_020a52a0;
extern const B5 data_020a52a8, data_020a52b0, data_020a52c0, data_020a52c8;
extern const B4 data_020a52b8, data_020a52bc, data_020a52d0, data_020a52d4;
extern void func_0201cf1c(DecForm *, double, DecDigits *);
extern double func_0201bb40(double);

/* 0x0201308c: double to "0x1.xxxp+e" (hex float), inf and nan; digits written backwards from end */
#define XB ((unsigned char *)&x)
char *func_0201308c(double x, char *end, FmtSpec spec)
{
    char *p = end;
    DecForm f;
    FmtSpec e;
    DecDigits d;
    unsigned char *xb;
    int i, n, bit, ex;
    double ld;
    unsigned snum;
    char *q;
    ld = x;
    if (spec.prec > 509) return 0;
    f.mode = 0;
    f.ndigits = 32;
    func_0201cf1c(&f, ld, &d);
    if (0.0 == func_0201bb40(x)) {
        p = end - 6;
        __memcpy(p, &data_020a52a0, 6);
        return p;
    }
    if (d.text[0] == 'I') {
        if (d.sign) {
            p = end - 5;
            if (spec.conv == 'A') __memcpy(p, &data_020a52a8, 5);
            else __memcpy(p, &data_020a52b0, 5);
        } else {
            p = end - 4;
            if (spec.conv == 'A') __memcpy(p, &data_020a52b8, 4);
            else __memcpy(p, &data_020a52bc, 4);
        }
        return p;
    }
    if (d.text[0] == 'N') {
        if (d.sign) {
            p = end - 5;
            if (spec.conv == 'A') __memcpy(p, &data_020a52c0, 5);
            else __memcpy(p, &data_020a52c8, 5);
        } else {
            p = end - 4;
            if (spec.conv == 'A') __memcpy(p, &data_020a52d0, 4);
            else __memcpy(p, &data_020a52d4, 4);
        }
        return p;
    }
    e.justify = 1;
    e.sign = 1;
    e.prec_set = 0;
    e.alt = 0;
    e.len = 0;
    e.width = 0;
    e.prec = 1;
    e.conv = 'd';
    q = (char *)&x;
    for (i = 0; i < 4; i++) {
        char t = q[i];
        q[i] = q[7 - i];
        q[7 - i] = t;
    }
    snum = XB[0] << 25;
    snum |= XB[1] << 17;
    snum = (snum >> 21) & 0x7ff;
    ex = snum - 1023;
    p = func_02012b44(ex, p, e);
    if (spec.conv == 'a') *--p = 'p';
    else *--p = 'P';
    xb = (unsigned char *)&x;
    bit = spec.prec * 4 + 11;
    for (n = spec.prec; n >= 1; n--) {
        unsigned char ch;
        if (bit < 64) {
            unsigned char v = xb[bit >> 3] >> (7 - (bit & 7));
            if ((bit & ~7) != ((bit - 4) & ~7)) {
                v = (v | (xb[(bit >> 3) - 1] << 8) >> (7 - (bit & 7))) & 0xff;
            }
            v &= 0xf;
            if (v < 10) ch = v + '0';
            else if (spec.conv == 'a') ch = v + 'a' - 10;
            else ch = v + 'A' - 10;
        } else {
            ch = '0';
        }
        *--p = ch;
        bit -= 4;
    }
    if (spec.prec != 0 || spec.alt != 0) *--p = '.';
    *--p = '1';
    if (spec.conv == 'a') *--p = 'x';
    else *--p = 'X';
    *--p = '0';
    if (d.sign) *--p = '-';
    else if (spec.sign == 1) *--p = '+';
    else if (spec.sign == 2) *--p = ' ';
    return p;
}

/* 0x02013568: round a decimal digit string to n digits (half to even) */
void func_02013568(DecDigits *d, int n)
{
    char *t;
    int len;
    char *p;
    int carry;
    int c;
    if (n < 0) {
zero:
        d->exp = 0;
        d->len = 1;
        d->text[0] = '0';
        return;
    }
    len = d->len;
    if (n >= len) return;
    t = d->text;
    p = t + n + 1;
    c = (char)(*--p - '0');
    if (c == 5) {
        char *q = t + len;
        do {
            q--;
        } while (q > p && *q == '0');
        if (q == p) carry = p[-1] & 1;
        else carry = 1;
    } else {
        carry = c > 5;
    }
    while (n != 0) {
        c = (char)(*--p - '0' + carry);
        carry = c > 9;
        if (carry != 0 || c == 0) {
            n--;
        } else {
            *p = c + '0';
            break;
        }
    }
    if (carry) {
        d->exp++;
        d->len = 1;
        d->text[0] = '1';
        return;
    }
    if (n == 0) goto zero;
    d->len = n;
}
