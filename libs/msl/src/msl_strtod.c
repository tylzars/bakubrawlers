/* MSL float scanner (strtod core used by scanf %a %e %f %g): state machine over the reader
   callback rd(ctx, ch, op). Handles sign, INF/INFINITY, NAN/NAN(...), decimal digits with
   fraction/exponent, and 0x hex floats with p-exponent. maxw = field width, *nread = chars
   consumed (0 on failure), *ovf = range error flag. Returns the value as a double.
   Main 0x02015c74, 0x124c bytes. */
typedef struct S9 { char s[9]; } S9;
typedef struct S5 { char s[5]; } S5;
typedef struct Dec {
    char sign;
    char unk;
    short exp;
    unsigned char len;
    unsigned char text[32];
} Dec;

extern const unsigned short data_020903a8[];
extern const unsigned char data_02090328[];
extern float data_020a5074;
extern double data_020a507c;

#define ISSPACE(c) (((c) < 0 || (c) >= 128) ? 0 : (data_020903a8[c] & 0x100))
#define ISDIGIT(c) (((c) < 0 || (c) >= 128) ? 0 : (data_020903a8[c] & 8))
#define ISXDIGIT(c) (((c) < 0 || (c) >= 128) ? 0 : (data_020903a8[c] & 0x400))
#define ISALPHA(c) (((c) < 0 || (c) >= 128) ? 0 : (data_020903a8[c] & 1))
static inline int TOUPPERF(int x)
{
    return (x < 0 || x >= 128) ? x : data_02090328[x];
}
#define TOUPPER(c) (((c) < 0 || (c) >= 128) ? (c) : data_02090328[c])

extern double func_02012350(const char *);
extern double func_0201cfc4(Dec *);

double ScanDouble(int maxw, int (*rd)(void *, int, int), void *ctx, int *nread, int *ovf)
{
    int hexst, idx;
    int ws, neg, expneg, expv, adj, signcnt;
    unsigned char *mp;
    int hexexp, hexneg;
    unsigned hexcnt;
    enum { HEXMAX = 14 } hexmax = HEXMAX; /* the limit is kept in a register (enum locals are not propagated) */
    int count, state, c;
    char *p;
    hexst = 0;
    count = 0;
    ws = 0;
    state = 1;
    {
    Dec d = {0};
    neg = 0;
    *ovf = 0;
    expneg = 0; expv = 0; adj = 0; signcnt = 0;
    hexexp = 0; hexneg = 0; hexcnt = 0;
    count++;
    c = rd(ctx, 0, 0);
    {
    unsigned char mant[8];
    while (count <= maxw && c != -1 && !(state & 0x1800)) {
        switch (state) {
        case 1:
            if (ISSPACE(c)) {
                c = rd(ctx, 0, 0);
                ws++;
            } else {
                switch (TOUPPER(c)) {
                case '-':
                    neg = 1;
                case '+':
                    count++;
                    c = rd(ctx, 0, 0);
                    signcnt = 1;
                    break;
                case 'I':
                    count++;
                    c = rd(ctx, 0, 0);
                    state = 0x4000;
                    break;
                case 'N':
                    count++;
                    c = rd(ctx, 0, 0);
                    state = 0x2000;
                    break;
                default:
                    state = 2;
                    break;
                }
            }
            break;
        case 0x4000:
            state = 1;
            {
            S9 inf_s = {"INFINITY"};
            S9 infbuf = inf_s;
            int k;
            p = infbuf.s + 1;
            while (state < 8 && TOUPPER(c) == *p) {
                p++;
                state++;
                count++;
                c = rd(ctx, 0, 0);
            }
            k = state;
            if (k == 3 || k == 8) {
                double r;
                if (neg) {
                    r = (double)(0.0f - data_020a5074);
                } else {
                    r = (double)data_020a5074;
                }
                *nread = ws + k + signcnt;
                return r;
            }
            state = 0x1000;
            break;
        }
        case 0x2000:
            {
            int i = 1;
            S5 nan_s = {"NAN("};
            int j = 0;
            S5 nbuf = nan_s;
            char nb[32] = {0};
            p = nbuf.s + 1;
            while (i < 4 && TOUPPER(c) == *p) {
                p++;
                i++;
                count++;
                c = rd(ctx, 0, 0);
            }
            if ((unsigned)(i - 3) <= 1) {
                if (i == 4) {
                    while (j < 32 && (ISDIGIT(c) || ISALPHA(c) || c == '.')) {
                        nb[j++] = c;
                        count++;
                        c = rd(ctx, 0, 0);
                    }
                    if (c != ')') {
                        state = 0x1000;
                        break;
                    }
                    j++;
                }
                nb[j] = 0;
                {
                    double r;
                    if (neg) r = 0.0 - func_02012350(nb);
                    else r = func_02012350(nb);
                    *nread = ws + i + j + signcnt;
                    return r;
                }
            }
            state = 0x1000;
            break;
        }
        case 2:
            if (c == '.') {
                state = 0x10;
                count++;
                c = rd(ctx, 0, 0);
            } else if (!ISDIGIT(c)) {
                state = 0x1000;
            } else if (c == '0') {
                count++;
                if (TOUPPERF(c = rd(ctx, 0, 0)) == 'X') {
                    state = 0x8000;
                    hexst = 1;
                } else {
                    state = 4;
                }
            } else {
                state = 8;
            }
            break;
        case 4:
            if (c == '0') { count++; c = rd(ctx, 0, 0); }
            else state = 8;
            break;
        case 8:
            if (!ISDIGIT(c)) {
                if (c == '.') { state = 0x20; count++; c = rd(ctx, 0, 0); }
                else state = 0x40;
            } else {
                if (d.len < 20) d.text[d.len++] = c;
                else adj++;
                count++;
                c = rd(ctx, 0, 0);
            }
            break;
        case 0x10:
            if (!ISDIGIT(c)) state = 0x1000; else state = 0x20;
            break;
        case 0x20:
            if (!ISDIGIT(c)) {
                state = 0x40;
            } else {
                if (d.len < 20) {
                    if (!(c == '0' && d.len == 0)) d.text[d.len++] = c;
                    adj--;
                }
                count++;
                c = rd(ctx, 0, 0);
            }
            break;
        case 0x40:
            if (TOUPPER(c) == 'E') { state = 0x80; count++; c = rd(ctx, 0, 0); }
            else state = 0x800;
            break;
        case 0x80:
            if (c == '+') { count++; c = rd(ctx, 0, 0); }
            else if (c == '-') { count++; c = rd(ctx, 0, 0); expneg = 1; }
            state = 0x100;
            break;
        case 0x100:
            if (!ISDIGIT(c)) state = 0x1000;
            else if (c == '0') { state = 0x200; count++; c = rd(ctx, 0, 0); }
            else state = 0x400;
            break;
        case 0x200:
            if (c == '0') { count++; c = rd(ctx, 0, 0); }
            else state = 0x400;
            break;
        case 0x400:
            if (!ISDIGIT(c)) state = 0x800;
            else {
                expv = expv * 10 + (c - '0');
                if (expv > 0x7fff) *ovf = 1;
                count++;
                c = rd(ctx, 0, 0);
            }
            break;
        case 0x8000:
            switch (hexst) {
            case 1:
                mp = (unsigned char *)&mant; /* placeholder: through &mant so the clears below go through mp; the real form is unknown */
                mp[0] = 0; mp[1] = 0; mp[2] = 0; mp[3] = 0;
                mp[4] = 0; mp[5] = 0; mp[6] = 0; mp[7] = 0;
                hexcnt = 0;
                idx = 0;
                hexst = 2;
                count++;
                c = rd(ctx, 0, 0);
                break;
            case 2:
                if (c == '0') { count++; c = rd(ctx, 0, 0); }
                else hexst = 4;
                break;
            case 4:
                if (!ISXDIGIT(c)) {
                    if (c == '.') { hexst = 8; count++; c = rd(ctx, 0, 0); }
                    else hexst = 0x10;
                } else {
                    if (hexcnt < hexmax) {
                        unsigned char v;
                        hexcnt++;
                        v = mp[idx / 2];
                        c = TOUPPER(c);
                        if (c >= 'A') c -= '7'; else c -= '0';
                        {
                            unsigned char n = c;
                            if (idx % 2) v |= n;
                            else v |= (unsigned char)(n << 4);
                        }
                        mp[idx++ / 2] = v;
                        count++;
                        c = rd(ctx, 0, 0);
                    } else {
                        count++;
                        c = rd(ctx, 0, 0);
                    }
                }
                break;
            case 8:
                if (!ISXDIGIT(c)) hexst = 0x10;
                else {
                    if (hexcnt < hexmax) {
                        unsigned char v;
                        v = mp[idx / 2];
                        c = TOUPPER(c);
                        if (c >= 'A') c -= '7'; else c -= '0';
                        {
                            unsigned char n = c;
                            if (idx % 2) v |= n;
                            else v |= (unsigned char)(n << 4);
                        }
                        mp[idx++ / 2] = v;
                        count++;
                        c = rd(ctx, 0, 0);
                    } else {
                        count++;
                        c = rd(ctx, 0, 0);
                    }
                }
                break;
            case 0x10:
                if (TOUPPER(c) == 'P') { hexst = 0x20; count++; c = rd(ctx, 0, 0); }
                else state = 0x800;
                break;
            case 0x20:
                if (c == '-') hexneg = 1;
                else if (c != '+') { rd(ctx, c, 1); count--; }
                hexst = 0x40;
                count++;
                c = rd(ctx, 0, 0);
                break;
            case 0x40:
                if (!ISDIGIT(c)) state = 0x1000;
                else if (c == '0') { hexst = 0x80; count++; c = rd(ctx, 0, 0); }
                else hexst = 0x100;
                break;
            case 0x80:
                if (c == '0') { count++; c = rd(ctx, 0, 0); }
                else hexst = 0x100;
                break;
            case 0x100:
                if (!ISDIGIT(c)) state = 0x800;
                else {
                    hexexp = hexexp * 10 + (c - '0');
                    count++;
                    if (expv > 0x7fff) *ovf = 1;
                    c = rd(ctx, 0, 0);
                }
                break;
            }
            break;
        }
    }
    {
        int bad;
        if (state != 0x8000) bad = (state & 0xe2c) ? 0 : 1;
        else bad = (count - 1 > 2 && (hexst & 0x18e)) ? 0 : 1;
        if (bad) *nread = 0;
        else *nread = (count - 1) + ws;
    }
    rd(ctx, c, 1);
    if (hexst == 0) {
        int n;
        unsigned char *q;
        double x;
        if (expneg) expv = -expv;
        n = d.len;
        q = d.text + n;
        while (n-- != 0 && *--q == '0') adj++;
        d.len = n + 1;
        if (d.len == 0) d.text[d.len++] = '0';
        expv = expv + adj;
        if (expv < -0x8000 || expv > 0x7fff) *ovf = 1;
        if (*ovf) {
            if (expneg) return 0.0;
            if (!neg) return data_020a507c;
            return 0.0 - data_020a507c;
        }
        d.exp = expv;
        x = func_0201cfc4(&d);
        if (x != 0.0 && x < 2.2250738585072014e-308) {
            *ovf = 1;
        } else if (x > 1.7976931348623157e308) {
            *ovf = 1;
            x = data_020a507c;
        }
        if (neg && (state & 0xe2c)) x = 0.0 - x;
        return x;
    } else {
        union { unsigned char b[8]; double d; } tu;
        unsigned char first;
        unsigned dst; /* leading-zero count, then the output byte index */
        unsigned i;
        int k;
        int j;
        unsigned char *tmp = tu.b;
        int n;
        if (hexneg) hexexp = -hexexp;
        hexexp += hexcnt * 4;
        first = mant[0];
        dst = 0;
        while (dst < 4 && !(first & (0x80 >> dst))) { hexexp--; dst++; }
        n = dst + 1;
        if (n != 0) {
            unsigned char carry = 0;
            mp = mant + 7;
            while (mp >= mant) {
                unsigned char b = *mp;
                *mp = carry | (b << n);
                carry = (b >> (8 - n));
                mp--;
            }
        }
        tmp[0] = 0; tmp[1] = 0; tmp[2] = 0; tmp[3] = 0;
        tmp[4] = 0; tmp[5] = 0; tmp[6] = 0; tmp[7] = 0;
        k = 12; dst = 1; i = 0;
        do {
            unsigned char b = mant[i / 8];
            if (i + 8 > 52) b &= (0xff << (52 - i));
            tmp[dst] |= (unsigned char)(b >> (k & 7));
            dst++;
            tmp[dst] |= (unsigned char)(b << (8 - (k & 7)));
            k += 8;
            i += 8;
        } while (i < 52);
        {
            unsigned e = hexexp + 0xfe + 0x300;
            if (e & -0x800) { *ovf = 1; return 0.0; }
            e <<= 21;
            tmp[0] |= e >> 25;
            tmp[1] |= e >> 17;
            if (neg) tmp[0] |= 0x80;
        }
        for (j = 0; j < 4; j++) {
            unsigned char t = tmp[j];
            tmp[j] = tmp[7 - j];
            tmp[7 - j] = t;
        }
        return *(double *)tmp;
    }
    }
    }
}
