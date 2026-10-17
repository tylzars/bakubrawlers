/* CRI tail: empty hooks, wrappers, sample cross-fade, 0x02051528-0x02051800 */
typedef char *va_list;
#define va_start(ap, last) ((ap) = (va_list)((((int)&(last)) & ~3) + 4))

extern int data_0209e4f0;
extern int *data_020b01f8;
extern int data_020b01fc[4];

extern int func_02015a6c(void *, int);
extern int func_02015b34(void *, int, int);
extern int func_02015b84(void *, int);
extern int func_02015bb4(void *, int, int);
extern int func_020146c0(char *, const char *, va_list);
extern void func_02012464(void *, const void *, int);

void func_02051528(void) {}
void func_0205152c(void) {}
void func_02051530(void) {}
void func_02051534(void) {}
void func_02051538(void) {}
void func_0205153c(void) {}

int *func_02051540(void)
{
    return data_020b01f8 = &data_0209e4f0;
}

int CriCopyString(void *a, int b, int c) { return func_02015a6c(a, c); }
int CriCopyStringN(void *a, int b, int c, int d) { return func_02015b34(a, c, d); }
int CriAppendString(void *a, int b, int c) { return func_02015b84(a, c); }
int CriAppendStringN(void *a, int b, int c, int d) { return func_02015bb4(a, c, d); }

int CriFormatString(char *buf, int unused, const char *fmt, ...)
{
    va_list ap;
    va_start(ap, fmt);
    return func_020146c0(buf, fmt, ap);
}

int CriFormatStringV(char *buf, int unused, const char *fmt, va_list ap)
{
    return func_020146c0(buf, fmt, ap);
}

void CriCrossFadeResizeSamples(short *src, int n1, short *dst, int n2)
{
    int i;
    if (n1 < n2) {
        int d = n2 - n1;
        int m = n1 - d;
        for (i = 0; i < d; i++) {
            dst[i] = src[i];
        }
        for (i = 0; i < m; i++) {
            short a = (src[d + i] * (m - i)) / m;
            short b = (src[i] * i) / m;
            int v = a + b;
            if (v > 0x7fff) {
                v = 0x7fff;
            } else if (v < -0x8000) {
                v = -0x8000;
            }
            dst[d + i] = v;
        }
        for (i = 0; i < d; i++) {
            dst[n1 + i] = src[m + i];
        }
    } else if (n1 == n2) {
        func_02012464(dst, src, n2 * 2);
    } else if (n1 > n2) {
        int off = n1 - n2;
        for (i = 0; i < n2; i++) {
            short a = (src[i] * (n2 - i)) / n2;
            short b = (src[off + i] * i) / n2;
            int v = a + b;
            if (v > 0x7fff) {
                v = 0x7fff;
            } else if (v < -0x8000) {
                v = -0x8000;
            }
            dst[i] = v;
        }
    }
}

void func_020517b4(int *a, int *b)
{
    if (a) *a = data_020b01fc[2];
    if (b) *b = data_020b01fc[0];
}

void func_020517dc(int *a, int *b)
{
    if (a) *a = data_020b01fc[3];
    if (b) *b = data_020b01fc[1];
}
