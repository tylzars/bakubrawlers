typedef struct Dec {
    char sign;
    unsigned char unk1;
    short exp;
    unsigned char len;
    unsigned char text[36];
} Dec;


int func_0201c1cc(Dec *d, int n)
{
    unsigned char *p = &d->text[n];
    unsigned char *end;
    if (d->text[n] > 5) return 1;
    if (d->text[n] < 5) return -1;
    p++;
    end = d->text + d->len;
    while (p < end) {
        if (*p != 0) return 1;
        p++;
    }
    return (d->text[n - 1] & 1) ? 1 : -1;
}

void func_0201c238(Dec *d, int n)
{
    unsigned char *p = d->text + n;
    int carry = 0;
    p--;
    while (1) {
        if (*p < 9) {
            (*p)++;
            return;
        }
        if (p == d->text) {
            *p = 1;
            d->exp++;
            return;
        }
        *p-- = 0;
    }
}


/* Does nothing unless 0 < n < d->len. */
void RoundDec(Dec *d, int n)
{
    if (n <= 0) return;
    if (n >= d->len) return;
    {
        int r = func_0201c1cc(d, n);
        d->len = n;
        if (r < 0) return;
        func_0201c238(d, n);
    }
}

/* sign is always 0; v == 0 gives len 0 and exp -1. */
void U64ToDec(Dec *d, unsigned long long v)
{
    int i, j;
    d->sign = 0;
    d->len = 0;
    if (v != 0) {
        do {
            int k = d->len++;
            d->text[k] = (unsigned char)(v % 10);
            v = v / 10;
        } while (v != 0);
    }
    {
        unsigned char *a = d->text;
        unsigned char *b = a + d->len - 1;
        while (a < b) {
            unsigned char t = *b;
            unsigned char u = *a;
            *a++ = t;
            *b-- = u;
        }
    }
    d->exp = d->len - 1;
}

/* res->sign is set to 0: the signs are not multiplied. */
void MultiplyDec(Dec *res, const Dec *a, const Dec *b)
{
    unsigned char buf[64];
    unsigned int sum = 0;
    int n = a->len + b->len - 1;
    unsigned char *p = buf + n + 1;
    unsigned char *end = p;
    int k;

    res->sign = 0;
    while (n > 0) {
        int i = b->len - 1;
        int j = n - i - 1;
        int cnt;
        const unsigned char *pa, *pb;
        if (j < 0) {
            j = 0;
            i = n - 1;
        }
        pa = &a->text[j];
        pb = &b->text[i];
        cnt = i + 1;
        if (cnt > a->len - j) cnt = a->len - j;
        while (cnt > 0) {
            sum += *pa * *pb;
            pa++;
            pb--;
            cnt--;
        }
        n--;
        *--p = sum % 10;
        sum /= 10;
    }
    res->exp = a->exp + b->exp;
    if (sum != 0) {
        *--p = sum;
        res->exp++;
    }
    for (k = 0; k < 32 && p < end; k++) {
        res->text[k] = *p; p++;
    }
    res->len = k;
    if (p >= end) return;
    if (*p < 5) return;
    if (*p == 5) {
        const unsigned char *q = p + 1;
        while (q < end) {
            if (*q != 0) goto up;
            q++;
        }
        if (!(p[-1] & 1)) return;
    }
up:
    func_0201c238(res, res->len);
}

/* sign is set to 0. The tail tests compare a character with 5, so any string longer than 32 digits rounds up. */
void StringToDec(Dec *d, const char *s, short exp)
{
    int i;
    d->exp = exp;
    d->sign = 0;
    for (i = 0; i < 32 && *s != 0; i++) {
        d->text[i] = *s++ - '0';
    }
    d->len = i;
    if (*s == 0) return;
    if (*s < 5) return;
    if (*s <= 5) {
        const char *p = s + 1;
        for (; *p != 0; p++) {
            if (*p != '0') goto up;
        }
        if (!(d->text[i - 1] & 1)) return;
    }
up:
    func_0201c238(d, d->len);
}
