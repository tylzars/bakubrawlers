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

extern int GetSignBit(double);
extern int ClassifyDouble(double);
extern double frexp(double, int *);
extern double scalbn(double, int);
extern unsigned long long func_0201dbec(double);
extern int CountSetBits64(unsigned long long);
extern void func_0201c5b4(Dec *, int);
extern void U64ToDec(Dec *, unsigned long long);
extern void MultiplyDec(Dec *, const Dec *, const Dec *);
extern void RoundDec(Dec *, int);

void DoubleToDec(Dec *d, double x)
{
    char sign = GetSignBit(x) != 0;
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
    if (ClassifyDouble(x) <= 2) {
        d->sign = sign;
        d->exp = 0;
        d->len = 1;
        d->text[0] = (ClassifyDouble(x) == 1) ? 'N' : 'I';
        return;
    }
    if (sign) x = -x;
    f = frexp(x, &e);
    t = f;
    bits = *(unsigned long long *)&t | 0x0010000000000000ULL;
    n = 53 - CountSetBits64((bits & -bits) - 1);
    func_0201c5b4(&p2, e - n);
    U64ToDec(&iv, func_0201dbec(scalbn(f, n)));
    MultiplyDec(d, &iv, &p2);
    d->sign = sign;
}

void func_0201cf1c(const DecForm *form, double x, Dec *d)
{
    int digits = form->ndigits;
    int i;
    DoubleToDec(d, x);
    if (d->text[0] > 9) return;
    if (digits > 32) digits = 32;
    RoundDec(d, digits);
    while (d->len < digits) {
        d->text[d->len++] = 0;
    }
    d->exp -= d->len - 1;
    for (i = 0; i < d->len; i++) {
        d->text[i] += '0';
    }
}
