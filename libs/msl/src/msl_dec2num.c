// MSL decimal-to-double (inverse of the Dec helpers), main 0x0201cfc4, matches 1/1 (2.0/sp1p5 -O4,p -ipa file -enum int)
typedef struct Dec {
    char sign;
    unsigned char unk1;
    short exp;
    unsigned char len;
    unsigned char text[33];
} Dec;


extern float data_020a5074;
extern char data_020a53f4[];
extern double data_020a52e0[];
extern double copysign(double, double);
extern double pow(double, double);
extern double scalbn(double, int);
extern int ClassifyDouble(double);
extern void func_0201c518(Dec *, const char *, short);
extern void func_0201cd9c(Dec *, double);
extern int func_0201c938(const Dec *, const Dec *);
extern int func_0201ca18(const Dec *, const Dec *);
extern void func_0201caf8(Dec *, const Dec *, const Dec *);

double func_0201cfc4(Dec *d)
{
    Dec dd, lim, cur, cur2, d1, d2;
    double r;
    double acc;
    double bits;
    unsigned long long *pb;
    unsigned char *p, *end, *q;
    int e, closer, down;
    double t, s;

    if (d->len == 0) {
        return copysign(0.0, d->sign == 0 ? 1.0 : -1.0);
    }
    switch (d->text[0]) {
    case '0':
        return copysign(0.0, d->sign == 0 ? 1.0 : -1.0);
    case 'I':
        return copysign((double)data_020a5074, d->sign == 0 ? 1.0 : -1.0);
    case 'N': {
        unsigned long long *q = (unsigned long long *)&r;
        *q = 0x7ff0000000000000ULL;
        if (d->sign) *q |= 0x8000000000000000ULL;
        *q |= 0x0008000000000000ULL;
        return r;
    }
    }
    dd = *d;
    q = dd.text;
    end = q + dd.len;
    while (q < end) {
        *q = *q - '0';
        q++;
    }
    dd.exp += dd.len - 1;
    e = dd.exp;
    func_0201c518(&lim, data_020a53f4, 0x134);
    if (func_0201ca18(&lim, &dd)) {
        return copysign((double)data_020a5074, d->sign == 0 ? 1.0 : -1.0);
    }
    q = dd.text;
    p = q + 1;
    acc = (double)*q;
    while (p < end) {
        int n, i;
        unsigned val = 0;
        n = (end - p) % 8;
        if (n == 0) n = 8;
        for (i = 0; i < n; i++) {
            val = val * 10 + *p++;
        }
        t = acc * data_020a52e0[n - 1];
        s = t + (double)val;
        if (val != 0 && t == s) break;
        acc = s;
        e -= n;
    }
    if (e < 0) {
        s = pow(5.0, (double)(-e));
        acc = acc / s;
    } else {
        s = pow(5.0, (double)e);
        acc = acc * s;
    }
    acc = scalbn(acc, e);
    if (ClassifyDouble(acc) == 2) {
        *(unsigned long long *)&acc = 0x7fefffffffffffffULL;
    }
    pb = (unsigned long long *)&bits;
    closer = 0;
    func_0201cd9c(&cur, acc);
    if (func_0201c938(&cur, &dd) == 0) {
        if (func_0201ca18(&cur, &dd)) closer = 1;
        bits = acc;
        down = closer == 0;
        for (;;) {
            if (!down) {
                (*pb)++;
                if (ClassifyDouble(bits) == 2) goto done;
            } else {
                (*pb)--;
            }
            func_0201cd9c(&cur2, bits);
            if (closer) {
                if (func_0201ca18(&cur2, &dd) == 0) goto bracket;
            }
            if (!closer && func_0201ca18(&dd, &cur2) == 0) {
                Dec *tmp = &d1;
                double tb;
                *tmp = cur;
                cur = cur2;
                cur2 = *tmp;
                tb = acc;
                acc = bits;
                bits = tb;
                goto bracket;
            }
            cur = cur2;
            acc = bits;
        }
    bracket:
        func_0201caf8(&d1, &dd, &cur);
        func_0201caf8(&d2, &cur2, &dd);
        if (func_0201c938(&d1, &d2)) {
            if ((*(unsigned long long *)&acc & 1) != 0) {
                acc = bits;
            }
        } else if (func_0201ca18(&d1, &d2) == 0) {
            acc = bits;
        }
    }
done:
    if (dd.sign) acc = -acc;
    return acc;
}
