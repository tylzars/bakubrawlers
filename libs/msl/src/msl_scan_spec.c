/* MSL scanf format spec parser, main 0x02014704 */

typedef struct ScanSpec {
    unsigned char suppress;   /* '*' */
    unsigned char width_set;
    unsigned char len;
    unsigned char conv;       /* 0xff if invalid */
    int width;
    unsigned char set[32];    /* character set for %[ and %s */
} ScanSpec;

extern const unsigned short data_020903a8[];   /* character class table */
#define ISDIGIT(c) (((c) < 0 || (c) >= 128) ? 0 : (data_020903a8[c] & 8))
extern const ScanSpec data_020904a8;           /* default spec */

/* 0x02014704: parse one conversion spec after '%'; returns the char after it */
const char *func_02014704(const char *fmt, ScanSpec *out)
{
    ScanSpec s = data_020904a8;
    const char *p = fmt + 1;
    int c = fmt[1];
    int more;
    if (c == '%') {
        s.conv = c;
        *out = s;
        return p + 1;
    }
    if (c == '*') {
        s.suppress = 1;
        c = *++p;
    }
    if (ISDIGIT(c)) {
        s.width = 0;
        do {
            s.width = s.width * 10 + (c - '0');
            c = *++p;
        } while (ISDIGIT(c));
        if (s.width == 0) {
            s.conv = 0xff;
            *out = s;
            return p + 1;
        }
        s.width_set = 1;
    }
    more = 1;
    switch (c) {
    case 'h':
        s.len = 2;
        if (p[1] == 'h') { s.len = 1; c = *++p; }
        break;
    case 'l':
        s.len = 3;
        if (p[1] == 'l') { s.len = 7; c = *++p; }
        break;
    case 'L': s.len = 9; break;
    case 'j': s.len = 4; break;
    case 'z': s.len = 5; break;
    case 't': s.len = 6; break;
    default: more = 0; break;
    }
    if (more) c = *++p;
    s.conv = c;
    switch (c) {
    case 'd': case 'i': case 'o': case 'u': case 'x': case 'X':
        if (s.len == 9) s.conv = 0xff;
        break;
    case 'a': case 'A': case 'e': case 'E': case 'f': case 'F': case 'g': case 'G':
        if (s.len == 1 || s.len == 2 || (unsigned char)(s.len + 252) <= 3) s.conv = 0xff;
        else if (s.len == 3) s.len = 8;
        break;
    case 'p':
        s.len = 3;
        s.conv = 'x';
        break;
    case 'c':
        if (s.len == 3) s.len = 10;
        else if (s.len != 0) s.conv = 0xff;
        break;
    case 's': {
        int i;
        if (s.len == 3) s.len = 10;
        else if (s.len != 0) s.conv = 0xff;
        i = 32;
        {
            unsigned char *q = s.set;
            do { i--; *q++ = 0xff; } while (i > 0);
        }
        s.set[1] = 0xc1;
        s.set[4] = 0xfe;
        break;
    }
    case '[': {
        int invert;
        if (s.len == 3) s.len = 10;
        else if (s.len != 0) s.conv = 0xff;
        c = *++p;
        invert = 0;
        if (c == '^') {
            c = *++p;
            invert = 1;
        }
        if (c == ']') {
            s.set[0x5d >> 3] |= 1 << (0x5d & 7);
            c = *++p;
        }
        while (c != 0 && c != ']') {
            s.set[(unsigned char)c >> 3] |= 1 << (c & 7);
            if (p[1] == '-' && p[2] != 0 && p[2] != ']') {
                int hi = p[2];
                for (c++; c <= hi; c++) {
                    s.set[(unsigned char)c >> 3] |= 1 << (c & 7);
                }
                c = *(p += 3);
            } else {
                c = *++p;
            }
        }
        if (c == 0) {
            s.conv = 0xff;
        } else if (invert) {
            int i = 32;
            unsigned char *q = s.set;
            do { i--; *q = ~*q; q++; } while (i > 0);
        }
        break;
    }
    case 'n':
        break;
    default:
        s.conv = 0xff;
        break;
    }
    *out = s;
    return p + 1;
}
