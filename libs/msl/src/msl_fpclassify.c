#define __HI(x) *(1+(int*)&x)
#define __LO(x) *(int*)&x
extern double func_0201bd20(double, int *);
extern double func_0201bddc(double, int);
int func_0201d67c(double x) {
    return __HI(x) & 0x80000000;
}
int func_0201d690(double x) {
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
    x = func_0201bd20(x, &e);
    e += n;
    return func_0201bddc(x, e);
}
