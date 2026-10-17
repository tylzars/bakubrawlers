typedef struct Dec {
    char sign;
    unsigned char unk1;
    short exp;
    unsigned char len;
    unsigned char text[33];
} Dec;

extern void StringToDec(Dec *, const char *, short);
extern void MultiplyDec(Dec *, const Dec *, const Dec *);
extern char data_020a5320[], data_020a5350[], data_020a5378[], data_020a5390[], data_020a53a0[], data_020a53a8[],
    data_020a53b0[], data_020a53b8[], data_020a53c0[], data_020a53c4[], data_020a53c8[], data_020a53cc[],
    data_020a53d0[], data_020a53d4[], data_020a53d8[], data_020a53dc[], data_020a53e0[], data_020a53e4[],
    data_020a53e8[], data_020a53ec[], data_020a53f0[];

void func_0201c5b4(Dec *d, int n)
{
    Dec b, a;
    switch (n) {
    case -64: StringToDec(d, data_020a5320, -20); return;
    case -53: StringToDec(d, data_020a5350, -16); return;
    case -32: StringToDec(d, data_020a5378, -10); return;
    case -16: StringToDec(d, data_020a5390, -5); return;
    case -8: StringToDec(d, data_020a53a0, -3); return;
    case -7: StringToDec(d, data_020a53a8, -3); return;
    case -6: StringToDec(d, data_020a53b0, -2); return;
    case -5: StringToDec(d, data_020a53b8, -2); return;
    case -4: StringToDec(d, data_020a53c0, -2); return;
    case -3: StringToDec(d, data_020a53c4, -1); return;
    case -2: StringToDec(d, data_020a53c8, -1); return;
    case -1: StringToDec(d, data_020a53cc, -1); return;
    case 0: StringToDec(d, data_020a53d0, 0); return;
    case 1: StringToDec(d, data_020a53d4, 0); return;
    case 2: StringToDec(d, data_020a53d8, 0); return;
    case 3: StringToDec(d, data_020a53dc, 0); return;
    case 4: StringToDec(d, data_020a53e0, 1); return;
    case 5: StringToDec(d, data_020a53e4, 1); return;
    case 6: StringToDec(d, data_020a53e8, 1); return;
    case 7: StringToDec(d, data_020a53ec, 2); return;
    case 8: StringToDec(d, data_020a53f0, 2); return;
    }
    func_0201c5b4(&b, (n + (int)((n & 0x80000000) >> 31)) >> 1);
    MultiplyDec(d, &b, &b);
    if (n & 1) {
        a = *d;
        if (n > 0) {
            StringToDec(&b, data_020a53d4, 0);
        } else {
            StringToDec(&b, data_020a53cc, -1);
        }
        MultiplyDec(d, &a, &b);
    }
}

int IsDecEqual(const Dec *a, const Dec *b)
{
    int i, m;
    int la;
    if (a->text[0] == 0) return b->text[0] == 0;
    if (b->text[0] == 0) return a->text[0] == 0;
    if (a->exp == b->exp) {
        la = a->len;
        m = la;
        if (la > b->len) m = b->len;
        for (i = 0; i < m; i++) {
            if (a->text[i] != b->text[i]) return 0;
        }
        if (m == la) a = b;
        for (; i < a->len; i++) {
            if (a->text[i] != 0) return 0;
        }
        return 1;
    }
    return 0;
}
