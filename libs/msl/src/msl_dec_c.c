typedef struct Dec {
    char sign;
    unsigned char unk1;
    short exp;
    unsigned char len;
    unsigned char text[33];
} Dec;

int func_0201ca18(const Dec *a, const Dec *b)
{
    int i, m, lb, la;
    if (a->text[0] == 0) return b->text[0] != 0;
    if (b->text[0] == 0) return 0;
    if (a->exp == b->exp) {
        la = a->len;
        lb = b->len;
        m = lb;
        if (lb > la) m = la;
        for (i = 0; i < m; i++) {
            if (a->text[i] < b->text[i]) return 1;
            if (b->text[i] < a->text[i]) return 0;
        }
        if (m == la && i < lb) {
            for (; i < b->len; i++) {
                if (b->text[i] != 0) return 1;
            }
        }
        return 0;
    }
    return a->exp < b->exp;
}
