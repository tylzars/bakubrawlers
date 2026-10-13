/* MSL strtoul/strtoull, main 0x02016ec0-0x020177ac: __strtoul (16ec0), __strtoull (172a8), strtoul (17714). */
extern const unsigned short data_020903a8[];
extern const unsigned char data_02090328[];
extern int data_020c1790;
#define ISSPACE(c) (((c) < 0 || (c) >= 128) ? 0 : (data_020903a8[c] & 0x100))
#define ISDIGIT(c) (((c) < 0 || (c) >= 128) ? 0 : (data_020903a8[c] & 8))
#define ISALPHA(c) (((c) < 0 || (c) >= 128) ? 0 : (data_020903a8[c] & 1))
#define TOUPPER(c) (((c) < 0 || (c) >= 128) ? (c) : data_02090328[c])

extern int func_02015950(void *, int, int);

typedef struct StrRead { const char *p; int eof; } StrRead;

unsigned long func_02016ec0(int base, int maxw, int (*rd)(void *, int, int), void *ctx,
                            int *nread, int *neg, int *ovf)
{
    int state = 1;
    int count = 0;
    int nspace = 0;
    unsigned long value = 0;
    unsigned long max_over = 0;
    int ch;
    int stop = -1;

    *neg = *ovf = 0;
    if (base < 0 || base == 1 || base > 36 || maxw < 1) {
        state = 0x40;
    } else {
        count++;
        ch = rd(ctx, 0, 0);
    }
    if (base != 0) {
        max_over = 0xffffffffu / base;
    }
    while (count <= maxw && ch != stop && !(state & 0x60)) {
        switch (state) {
        case 1:
            if (ISSPACE(ch)) {
                ch = rd(ctx, 0, 0);
                nspace++;
                break;
            }
            if (ch == '+') {
                count++;
                ch = rd(ctx, 0, 0);
            } else if (ch == '-') {
                count++;
                ch = rd(ctx, 0, 0);
                *neg = 1;
            }
            state = 2;
            break;
        case 2:
            if ((base == 0 || base == 16) && ch == '0') {
                state = 4;
                count++;
                ch = rd(ctx, 0, 0);
            } else {
                state = 8;
            }
            break;
        case 4:
            if (ch == 'X' || ch == 'x') {
                base = 16;
                state = 8;
                count++;
                ch = rd(ctx, 0, 0);
            } else {
                if (base == 0) base = 8;
                state = 0x10;
            }
            break;
        case 8:
        case 0x10:
            if (base == 0) base = 10;
            if (max_over == 0) max_over = (unsigned)stop / base;
            if (ISDIGIT(ch)) {
                ch -= '0';
                if (ch >= base) {
                    if (state == 0x10) state = 0x20; else state = 0x40;
                    ch += '0';
                    break;
                }
            } else if (!ISALPHA(ch) || TOUPPER(ch) - 'A' + 10 >= base) {
                if (state == 0x10) state = 0x20; else state = 0x40;
                break;
            } else {
                ch = TOUPPER(ch) - 'A' + 10;
            }
            if (value > max_over) *ovf = 1;
            value *= base;
            if (ch > (unsigned long)(stop - value)) *ovf = 1;
            value += ch;
            state = 0x10;
            count++;
            ch = rd(ctx, 0, 0);
            break;
        }
    }
    if (!(state & 0x34)) {
        value = 0;
        *nread = 0;
    } else {
        *nread = count - 1 + nspace;
    }
    rd(ctx, ch, 1);
    return value;
}


unsigned long long func_020172a8(int base, int maxw, int (*rd)(void *, int, int), void *ctx,
                            int *nread, int *neg, int *ovf)
{
    int state = 1;
    int count = 0;
    int nspace = 0;
    unsigned long long value = 0;
    unsigned long long max_over = 0;
    int ch;
    int stop = -1;

    *neg = *ovf = 0;
    if (base < 0 || base == 1 || base > 36 || maxw < 1) {
        state = 0x40;
    } else {
        count++;
        ch = rd(ctx, 0, 0);
    }
    if (base != 0) {
        max_over = 0xffffffffffffffffull / base;
    }
    while (count <= maxw && ch != stop && !(state & 0x60)) {
        switch (state) {
        case 1:
            if (ISSPACE(ch)) {
                ch = rd(ctx, 0, 0);
                nspace++;
                break;
            }
            if (ch == '+') {
                count++;
                ch = rd(ctx, 0, 0);
            } else if (ch == '-') {
                count++;
                ch = rd(ctx, 0, 0);
                *neg = 1;
            }
            state = 2;
            break;
        case 2:
            if ((base == 0 || base == 16) && ch == '0') {
                state = 4;
                count++;
                ch = rd(ctx, 0, 0);
            } else {
                state = 8;
            }
            break;
        case 4:
            if (ch == 'X' || ch == 'x') {
                base = 16;
                state = 8;
                count++;
                ch = rd(ctx, 0, 0);
            } else {
                if (base == 0) base = 8;
                state = 0x10;
            }
            break;
        case 8:
        case 0x10:
            if (base == 0) base = 10;
            if (max_over == 0) max_over = (unsigned long long)stop / base;
            if (ISDIGIT(ch)) {
                ch -= '0';
                if (ch >= base) {
                    if (state == 0x10) state = 0x20; else state = 0x40;
                    ch += '0';
                    break;
                }
            } else if (!ISALPHA(ch) || TOUPPER(ch) - 'A' + 10 >= base) {
                if (state == 0x10) state = 0x20; else state = 0x40;
                break;
            } else {
                ch = TOUPPER(ch) - 'A' + 10;
            }
            if (value > max_over) *ovf = 1;
            value *= base;
            if (ch > (unsigned long long)(stop - value)) *ovf = 1;
            value += ch;
            state = 0x10;
            count++;
            ch = rd(ctx, 0, 0);
            break;
        }
    }
    if (!(state & 0x34)) {
        value = 0;
        *nread = 0;
    } else {
        *nread = count - 1 + nspace;
    }
    rd(ctx, ch, 1);
    return value;
}


unsigned long func_02017714(const char *str, char **end, int base)
{
    int scanned, neg, ovf;
    StrRead s;
    unsigned long v;
    s.p = str;
    s.eof = 0;
    v = func_02016ec0(base, 0x7fffffff, func_02015950, &s, &scanned, &neg, &ovf);
    if (end) *end = (char *)str + scanned;
    if (ovf) {
        v = -1;
        data_020c1790 = 34;
    } else if (neg) {
        v = -v;
    }
    return v;
}
