#define __HI(x) *(1+(int*)&x)
#define __LO(x) *(int*)&x
extern double frexp(double, int *);
extern double scalbn(double, int);
int GetSignBit(double x) {
    return __HI(x) & 0x80000000;
}
int ClassifyDouble(double x) {
    int hx = __HI(x);
    int e = hx & 0x7ff00000;
    switch (e) {
    case 0x7ff00000:
        return ((hx & 0xfffff) != 0 || __LO(x) != 0) ? 1 : 2;
    case 0:
        return ((hx & 0xfffff) != 0 || __LO(x) != 0) ? 5 : 3;
    }
    return 4;
}
double func_0201d700(double x, int n) {
    int e;
    x = frexp(x, &e);
    e += n;
    return scalbn(x, e);
}
