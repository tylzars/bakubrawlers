/* CRI middleware, device/path layer, 0x02046924-0x02047138 */

/* device driver vtable: function pointer slots indexed by offset/4 */
typedef struct CriDev {
    int (*f0)();        /* 0x00 */
    int pad4;
    int (*f8)();        /* 0x08 */
    int padc;
    int (*f10)();       /* 0x10 */
    int (*f14)();       /* 0x14 */
    int (*f18)(void *, int, int);       /* 0x18 */
    int (*f1c)();       /* 0x1c */
    int (*f20)(void *, int, int);       /* 0x20 */
    int pad24, pad28;
    int (*f2c)();       /* 0x2c */
    int pad30[12];
    int (*f60)();       /* 0x60 */
} CriDev;

/* open handle: 8-byte slot */
typedef struct CriDevHandle { CriDev *dev; void *obj; } CriDevHandle;

typedef struct CriDevEntry { int f0; char name[12]; } CriDevEntry;

extern CriDevEntry data_022176b4[32];
extern CriDevHandle data_022178b4[10];
extern char data_02217a30[];
extern char data_02217904[];
extern int data_022176a8[3];
extern int data_02217a3c;
extern int data_0209e388;

extern char data_020ad9b8[], data_020ad9e0[], data_020ada08[], data_020ada28[], data_020ada48[];
extern char data_020ada6c[], data_020ada8c[], data_020adaa4[], data_020adac4[], data_020adae0[];
extern char data_020adaf8[], data_020adb14[], data_020adb2c[], data_020adb48[], data_020adb60[];
extern char data_020adb7c[], data_020adb94[], data_020adbb0[], data_020adbcc[], data_020adbf4[];
extern char data_020adc1c[], data_020adc40[], data_020adc60[], data_020adc90[], data_020adc98[];

extern void CriDevSendText(const char *);
extern int func_02015a50(const char *);
extern void *func_02012464(void *, const void *, int);
extern int func_02015c04(const char *, const char *, int);
extern void CriStrToUpper(char *);
extern CriDev *CriDevTableFind(char *);
extern void CriCopyString(char *, int, const char *);
extern void CriFormatString(char *, int, const char *, char *, char *);
extern void SendErr(const char *, ...);

extern int CriDevTableHasName(char *, int);
extern char *CriDevGetDefaultName(void);
extern CriDev *func_020469f8(char *, char *, char *);
extern CriDevHandle *CriDevFindFreeHandle(void);
extern void CriDevClearHandle(CriDevHandle *);
extern void CriDevSplitPath(char *, char *, char *);
extern void CriDevCopyDefaultName(char *);
extern int func_02047044(char *);
extern void func_0204707c(char *, char *);

void CriDevSetDefaultName(char *path)
{
    int len;
    if (path == 0) {
        CriDevSendText(data_020ad9b8);
        return;
    }
    len = func_02015a50(path);
    if (len == 0) {
        data_02217a30[0] = 0;
        return;
    }
    CriStrToUpper(path);
    if (CriDevTableHasName(path, len) == 1) {
        func_02012464(data_02217a30, path, len + 1);
        return;
    }
    CriDevSendText(data_020ad9e0);
}

int CriDevTableHasName(char *path, int len)
{
    int i;
    CriDevEntry *e = data_022176b4;
    for (i = 0; i < 32; i++, e++) {
        if (func_02015c04(path, e->name, len) == 0) return 1;
    }
    return 0;
}

char *CriDevGetDefaultName(void)
{
    return data_02217a30;
}

CriDev *func_020469f8(char *a, char *b, char *c)
{
    CriDev *d;
    if (a[0] == 0) {
        CriDevCopyDefaultName(a);
        if (a[0] == 0) return 0;
    }
    func_0204707c(a, b);
    d = CriDevTableFind(a);
    if (d == 0) {
        CriDevCopyDefaultName(a);
        d = CriDevTableFind(a);
        if (d == 0) return 0;
        CriCopyString(b, 0x129, c);
    }
    return d;
}

CriDevHandle *CriDevAllocHandle(char *path, int arg1, int arg2)
{
    char tmp[0x12a];
    char name[0x12a];
    CriDevHandle *h;
    if (path == 0) {
        CriDevSendText(data_020ada08);
        return 0;
    }
    CriDevSplitPath(tmp, name, path);
    if (name[0] == 0) {
        CriDevSendText(data_020ada28);
        return 0;
    }
    h = CriDevFindFreeHandle();
    if (h == 0) {
        CriDevSendText(data_020ada48);
        return 0;
    }
    h->dev = func_020469f8(tmp, name, path);
    if (h->dev == 0) {
        CriDevClearHandle(h);
        CriDevSendText(data_020ada6c);
        return 0;
    }
    if (h->dev->f10 != 0) {
        h->obj = (void *)h->dev->f10(name, arg1, arg2);
    } else {
        CriDevClearHandle(h);
        CriDevSendText(data_020ada8c);
        return 0;
    }
    if (h->obj == 0) {
        CriDevClearHandle(h);
        CriDevSendText(data_020adaa4);
        return 0;
    }
    return h;
}

CriDevHandle *CriDevFindFreeHandle(void)
{
    int i;
    for (i = 0; i < 10; i++) {
        if (data_022178b4[i].obj == 0) break;
    }
    if (i == 10) return 0;
    return &data_022178b4[i];
}

void CriDevClearHandle(CriDevHandle *h)
{
    h->obj = 0;
    h->dev = 0;
}

void CriDevSplitPath(char *out, char *rest, char *src)
{
    int i, j, k;
    if (src == 0) return;
    for (i = 0; i < 0x129; i++) {
        char c = src[i];
        if (c == ':' || c == 0) break;
        out[i] = c;
    }
    if (src[i] == 0) {
        out[i] = 0;
        func_02012464(rest, out, func_02015a50(out) + 1);
        out[0] = 0;
        return;
    }
    out[i] = 0;
    i++;
    if (i == 2) {
        i = 0;
        out[0] = 0;
    }
    k = i;
    for (; k < 0x129; k++) {
        char c = src[k];
        if (c == 0) break;
        rest[k - i] = c;
    }
    rest[k - i] = 0;
    CriStrToUpper(out);
}

void CriDevCopyDefaultName(char *dst)
{
    int len = func_02015a50(data_02217a30);
    if (data_02217a30[0] == 0) {
        dst[0] = 0;
        return;
    }
    func_02012464(dst, data_02217a30, len + 1);
}

void CriDevDeallocHandle(CriDevHandle *h)
{
    if (h == 0) {
        CriDevSendText(data_020adac4);
        return;
    }
    if (h->dev->f14 != 0) {
        h->dev->f14(h->obj);
        CriDevClearHandle(h);
    } else {
        CriDevSendText(data_020adae0);
    }
}

int func_02046d74(CriDevHandle *h)
{
    int r;
    if (h == 0) {
        CriDevSendText(data_020adaf8);
        return 0;
    }
    if (h->dev->f1c != 0) {
        r = h->dev->f1c(h->obj);
    } else {
        r = 0;
        CriDevSendText(data_020adb14);
    }
    return r;
}

int func_02046dcc(CriDevHandle *h, int a, int b)
{
    int r;
    if (h == 0) {
        CriDevSendText(data_020adb2c);
        return 0;
    }
    if (h->dev->f18 != 0) {
        r = h->dev->f18(h->obj, a, b);
    } else {
        r = 0;
        CriDevSendText(data_020adb48);
    }
    return r;
}

int func_02046e24(CriDevHandle *h, int a, int b)
{
    int r;
    if (h == 0) {
        CriDevSendText(data_020adb60);
        return 0;
    }
    if (h->dev->f20 != 0) {
        r = h->dev->f20(h->obj, a, b);
    } else {
        r = 0;
        CriDevSendText(data_020adb7c);
    }
    return r;
}

void func_02046e7c(void)
{
    int i;
    for (i = 0; i < 32; i++) {
        CriDev *d = (CriDev *)data_022176b4[i].f0;
        if (d != 0 && d->f0 != 0) d->f0();
    }
}

int func_02046eb4(CriDevHandle *h)
{
    int r = 3;
    if (h == 0) {
        CriDevSendText(data_020adb94);
        return r;
    }
    if (h->dev->f2c != 0) {
        r = h->dev->f2c(h->obj);
    } else {
        CriDevSendText(data_020adbb0);
    }
    return r;
}

int func_02046f0c(char *path)
{
    char tmp[0x129];
    char name[0x129];
    CriDev *d;
    if (path == 0) {
        CriDevSendText(data_020adbcc);
        return 0;
    }
    CriDevSplitPath(tmp, name, path);
    if (name[0] == 0) {
        CriDevSendText(data_020adbf4);
        return 0;
    }
    d = func_020469f8(tmp, name, path);
    if (d == 0) CriDevSendText(data_020adc1c);
    if (d->f8 != 0) return d->f8(name);
    CriDevSendText(data_020adc40);
    return 0;
}

int func_02046fc8(CriDevHandle *h)
{
    int r;
    if (h == 0) {
        CriDevSendText(data_020adc60);
        return -1;
    }
    r = 0x7fffffff;
    if (h->dev->f60 != 0) {
        r = h->dev->f60(h->obj, 0x12c, 0, 0);
    }
    return r;
}

void CriDevSetCallback(int a, int b)
{
    if (a != 0) {
        data_022176a8[1] = a;
        data_022176a8[2] = b;
    } else {
        data_022176a8[1] = 0;
        data_022176a8[2] = 0;
    }
}

int func_02047044(char *path)
{
    CriDev *d = CriDevTableFind(path);
    if (d == 0) return 0;
    if (d->f60 == 0) return 0;
    return d->f60(0, 0x64, 0, 0);
}

void func_0204707c(char *path, char *b)
{
    if (path == 0) path = CriDevGetDefaultName();
    if (func_02047044(path) == 1) {
        CriCopyString(data_02217904, 0x129, b);
        CriFormatString(b, 0x129, data_020adc90, path, data_02217904);
    }
}

int func_020470e0(CriDevHandle *h)
{
    if (h == 0) {
        CriDevSendText(data_020adc98);
        return 0;
    }
    if (h->dev->f60 == 0) return 1;
    return h->dev->f60(h->obj, 0x190, 0, 0);
}

void func_0204712c(int a, int b)
{
    SendErr((const char *)b);
}
