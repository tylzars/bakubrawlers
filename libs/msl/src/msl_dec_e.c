typedef struct Dec {
    char sign;
    unsigned char unk1;
    short exp;
    unsigned char len;
    unsigned char text[33];
} Dec;

void func_0201caf8(Dec *res, const Dec *a, const Dec *b)
{
    int e1, m, diff;
    unsigned char *base;
    const unsigned char *bbase;
    unsigned char *p;
    int lower, n1, la;
    const unsigned char *q;
    unsigned char *end;
    int t, lb;
    const unsigned char *q0;
    unsigned char *r;

    *res = *a;
    if (b->text[0] == 0) return;
    la = res->len;
    lb = b->len;
    e1 = res->exp;
    m = la;
    if (la < lb) m = lb;
    diff = e1 - b->exp;
    m += diff;
    if (m > 32) m = 32;
    while (la < m) {
        res->len++;
        res->text[la] = 0;
        la = res->len;
    }
    base = res->text;
    p = base + m;
    t = b->len + diff;
    if (t < m) p = base + t;
    bbase = b->text;
    q = bbase + (p - base - diff);
    q0 = q;
    while (p > base && q > bbase) {
        --p;
        --q;
        if (*p < *q) {
            r = p - 1;
            if (p[-1] == 0) {
                do {
                    --r;
                } while (*r == 0);
            }
            if (r != p) {
                do {
                    --*r;
                    ++r;
                    *r += 10;
                } while (r != p);
            }
        }
        *p = *p - *q;
    }
    lb = b->len;
    n1 = q0 - bbase;
    if (n1 < lb) {
        lower = 0;
        if (*q0 < 5) {
            lower = 1;
        } else if (*q0 == 5) {
            const unsigned char *send = b->text + lb;
            const unsigned char *s = q0 + 1;
            for (; s < send; s++) {
                if (*s != 0) goto fin;
            }
            p = base + n1 + diff - 1;
            if (*p & 1) lower = 1;
        }
        if (lower) {
            if (*p < 1) {
                r = p - 1;
                if (p[-1] == 0) {
                    do {
                        --r;
                    } while (*r == 0);
                }
                if (r != p) {
                    do {
                        --*r;
                        ++r;
                        *r += 10;
                    } while (r != p);
                }
            }
            --*p;
        }
    }
fin:
    r = base;
    while (*r == 0) r++;
    if (r > base) {
        int cnt = (unsigned char)(r - base);
        res->exp -= cnt;
        end = base + res->len;
        if (r < end) {
            do {
                *base = *r; base++; r++;
            } while (r < end);
        }
        res->len -= cnt;
    }
    base = res->text;
    end = base + res->len;
    if (end > base) {
        do {
            --end;
            if (*end != 0) break;
        } while (end > base);
    }
    res->len = end - base + 1;
}
