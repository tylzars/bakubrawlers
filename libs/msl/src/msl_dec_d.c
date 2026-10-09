typedef struct Dec {
    char sign;
    unsigned char unk1;
    short exp;
    unsigned char len;
    unsigned char text[33];
} Dec;
typedef struct DecForm {
    unsigned char mode;
    short ndigits;
} DecForm;

extern int func_0201d67c(double);
extern int func_0201d690(double);
extern int func_0201ea54(double, double);
extern double func_0201e20c(double, double);
extern double func_0201bd20(double, int *);
extern double func_0201bddc(double, int);
extern unsigned long long func_0201dbec(double);
extern int func_0201d5e8(unsigned long long);
extern void func_0201c5b4(Dec *, int);
extern void func_0201c2c8(Dec *, unsigned long long);
extern void func_0201c394(Dec *, const Dec *, const Dec *);
extern void func_0201c288(Dec *, int);

void func_0201cd9c(Dec *d, double x)
{
    char sign = func_0201d67c(x) != 0;
    Dec iv, p2;
    int e, n;
    double f, t;
    unsigned long long bits;

    if (x == 0.0) {
        d->sign = sign;
        d->exp = 0;
        d->len = 1;
        d->text[0] = 0;
        return;
    }
    if (func_0201d690(x) <= 2) {
        d->sign = sign;
        d->exp = 0;
        d->len = 1;
        d->text[0] = (func_0201d690(x) == 1) ? 'N' : 'I';
        return;
    }
    if (sign) x = -x;
    f = func_0201bd20(x, &e);
    t = f;
    bits = *(unsigned long long *)&t | 0x0010000000000000ULL;
    n = 53 - func_0201d5e8((bits & -bits) - 1);
    func_0201c5b4(&p2, e - n);
    func_0201c2c8(&iv, func_0201dbec(func_0201bddc(f, n)));
    func_0201c394(d, &iv, &p2);
    d->sign = sign;
}

void func_0201cf1c(const DecForm *form, double x, Dec *d)
{
    int digits = form->ndigits;
    int i;
    func_0201cd9c(d, x);
    if (d->text[0] > 9) return;
    if (digits > 32) digits = 32;
    func_0201c288(d, digits);
    while (d->len < digits) {
        d->text[d->len++] = 0;
    }
    d->exp -= d->len - 1;
    for (i = 0; i < d->len; i++) {
        d->text[i] += '0';
    }
}
