/* CRI middleware, 0x02049150-0x02049c34: ADX flag setter, error callback, name table, file stream objects, 8-slot pool */
#pragma dont_inline on

typedef struct CriFlagObj {
    unsigned char b0;
    unsigned char b1;
    unsigned char pad2[4];
    unsigned char b6;
    unsigned char pad7;
    int pad8;
    struct { int pad[1]; unsigned char b4; } *ic;
} CriFlagObj;

/* error callback pair registered through CriMsgSetCallback */
typedef struct CriMsgCb { void (*fn)(int, char *); int arg; } CriMsgCb;
/* error handler registered through CriFilSetErrorCallback */
typedef struct CriErrCb { int arg; void (*fn)(int, int, int); } CriErrCb;

typedef struct CriNameEnt { char name[12]; int val; } CriNameEnt;

/* file stream object, 0x38 bytes */
typedef struct CriFil {
    signed char used;     /* 0x00 */
    signed char state;    /* 0x01 */
    short pad2;
    int ssize;            /* 0x04 */
    int i8;               /* 0x08 */
    int nsec;             /* 0x0c */
    int pos;              /* 0x10 */
    int i14;              /* 0x14 */
    int cnt;              /* 0x18 */
    char name[0x14];      /* 0x1c */
    int off;              /* 0x30 */
    int len;              /* 0x34 */
} CriFil;

/* 8-slot pool entry, 0x90 bytes */
typedef struct CriPool {
    unsigned short used;  /* 0x00 */
    unsigned short s2;    /* 0x02 */
    int i4;               /* 0x04 */
    int i8;               /* 0x08 */
    char pad0c[0x0c];
    unsigned short s18;   /* 0x18 */
    char pad1a[0x0c];
    short s26;   /* 0x26 */
    short s28;   /* 0x28 */
    unsigned short pad2a;
    short s2c;   /* 0x2c */
    unsigned short pad2e;
    int i30;              /* 0x30 */
    int h34[23];        /* 0x34 */
} CriPool;

extern void *func_02012484(void *, int, int);
extern void func_02015b34(char *, const char *, int);
extern int func_02015c04(const char *, const char *, int);
extern unsigned func_02015a50(const char *);
extern unsigned func_02017714(char *, char **, int);
extern void CriLockEnter1(void);
extern void CriLockLeave1(void);
extern void func_0203eda4(void);
extern void func_0203edb0(void);
extern void func_02048808(void *, int);
extern void CriCopyString(char *, int, char *);
extern void InitSndStrm(void *);
extern int ReserveSndStrmChannels(void *, int, void *);
extern void UnreserveSndStrmChannels(void *);
extern void SndOutStop(void *);
extern void func_02012464(void *, int, int);

extern char data_02218544[0x100];
extern CriMsgCb data_0221853c;
extern int data_02218644;
extern CriNameEnt *data_02218648;
extern CriErrCb data_0221864c;
extern CriFil data_02218654[10];
extern int data_02218884;
extern CriPool data_02218888[8];
extern volatile int data_0209e41c;
extern char data_020ae0cc[];
extern char data_020ae134[], data_020ae158[], data_020ae17c[], data_020ae1ac[], data_020ae1c8[], data_020ae1e4[];
extern char data_020ae200[], data_020ae220[], data_020ae244[], data_020ae25c[], data_020ae274[], data_020ae28c[], data_020ae2a4[];

extern void CriFilCallErrorCallback(int, void *);
extern unsigned CriParseHexPair(char *, unsigned *);
extern int CriParseHexPairSecond(char *);
extern CriFil *CriFilFindFree(void);
extern void CriFilClear(CriFil *);
extern void CriFilInit(CriFil *);
extern void CriFilResetState(CriFil *);
extern int CriFilGetLastReadSize(CriFil *);
extern void func_02049ad4(CriPool *, short);
extern void func_02049ab0(short);
extern unsigned func_02049384(char *, char **, int);
extern unsigned CriParseUnsigned(char *, char **, int);
extern void func_02049460(CriFil *);
extern CriNameEnt *CriNameTableFind(char *);
extern void CriFilLockEnter(void);
extern void CriFilLockLeave(void);

void func_02049150(CriFlagObj *p, int v)
{
    int t;
    if (p == 0) return;
    t = p->b6;
    if (v == ((p->b1 & 8) == 8 ? 1 : 0)) return;
    func_0203eda4();
    p->b1 = (p->b1 & ~8) | (v << 3);
    func_0203edb0();
    if (t != 2) return;
    if (v == 1) {
        func_02048808(p, 0);
    } else {
        func_02048808(p, 1);
    }
}

void func_020491c4(CriFlagObj *p, int v)
{
    p->ic->b4 = v;
}

void func_020491d0(void)
{
    func_02012484(data_02218544, 0, 0x100);
    data_0221853c.fn = 0;
    data_0221853c.arg = 0;
}

void func_02049200(void)
{
    func_02012484(data_02218544, 0, 0x100);
    data_0221853c.fn = 0;
    data_0221853c.arg = 0;
}

void CriMsgSetCallback(void (*fn)(int, char *), int arg)
{
    data_0221853c.fn = fn;
    data_0221853c.arg = arg;
}

void CriMsgSendText(char *msg)
{
    func_02015b34(data_02218544, msg, 0xff);
    if (data_0221853c.fn) {
        data_0221853c.fn(data_0221853c.arg, data_02218544);
    }
}

CriNameEnt *CriNameTableFind(char *name)
{
    int i;
    CriNameEnt *e;
    int n;
    if (name[0] == 0) return 0;
    e = data_02218648;
    n = data_02218644;
    for (i = 0; i < n; i++) {
        if (func_02015c04((char *)e, name, 12) == 0) return e;
        e++;
    }
    return 0;
}

int CriNameTableGetValue(char *name, int *out)
{
    CriNameEnt *e;
    if (data_02218648 == 0) return -1;
    e = CriNameTableFind(name);
    if (e == 0) return -3;
    *out = e->val;
    return 0;
}

void CriFilLockEnter(void)
{
    CriLockEnter1();
}

void CriFilLockLeave(void)
{
    CriLockLeave1();
}

char *func_02049344(void)
{
    int a = data_0209e41c;
    return data_020ae0cc;
}

void CriFilCallErrorCallback(int a, void *msg)
{
    if (data_0221864c.fn) {
        data_0221864c.fn(data_0221864c.arg, (int)msg, a);
    }
}

unsigned func_02049384(char *s, char **e, int b)
{
    return func_02017714(s, e, b);
}

unsigned CriParseUnsigned(char *s, char **end, int base)
{
    unsigned v = 0;
    int c;
    for (;;) {
        c = *s;
        if (c >= '0' && c <= '9') {
            c -= '0';
        } else if (c >= 'a' && c <= 'f') {
            c -= 'a' - 10;
        } else if (c >= 'A' && c <= 'F') {
            c -= 'A' - 10;
        } else {
            break;
        }
        v = base * v + c;
        s++;
    }
    *end = s;
    return v;
}

unsigned CriParseHexPair(char *s, unsigned *out)
{
    char *p;
    unsigned r;
    if (func_02015a50(s) >= 0x12) {
        CriFilCallErrorCallback(0, 0);
    }
    p = s;
    r = CriParseUnsigned(s, &p, 16);
    if (*p != 0) p++;
    if (out != 0) {
        *out = func_02049384(p, &p, 16);
    }
    return r;
}

void func_02049460(CriFil *f)
{
}

void func_02049464(void)
{
    int i;
    for (i = 0; i < 10; i++) {
        if (data_02218654[i].used != 0) {
            func_02049460(&data_02218654[i]);
        }
    }
}

void CriFilSetErrorCallback(void (*fn)(int, int, int), int arg)
{
    data_0221864c.fn = fn;
    data_0221864c.arg = arg;
}

int CriParseHexPairSecond(char *s)
{
    unsigned v;
    CriParseHexPair(s, &v);
    return v;
}

CriFil *CriFilFindFree(void)
{
    CriFil *r = 0;
    int i;
    for (i = 0; i < 10; i++) {
        if (data_02218654[i].used == 0) {
            r = &data_02218654[i];
            break;
        }
    }
    return r;
}

void CriFilClear(CriFil *f)
{
    func_02012484(f, 0, 0x38);
}

void CriFilInit(CriFil *f)
{
    f->ssize = 0x800;
    f->i8 = CriParseHexPairSecond(f->name);
    f->nsec = (f->i8 + (f->ssize - 1)) / f->ssize;
    f->pos = 0;
    f->cnt = 0;
    f->i14 = 0;
    f->state = 0;
    f->used = 1;
}

CriFil *CriFilAlloc(char *name, int a1, int a2)
{
    CriFil *f;
    if (name == 0) {
        CriFilCallErrorCallback(0, data_020ae134);
        return 0;
    }
    if (a2 != 0) {
        CriFilCallErrorCallback(0, data_020ae158);
        return 0;
    }
    f = CriFilFindFree();
    if (f == 0) {
        CriFilCallErrorCallback(0, data_020ae17c);
        return 0;
    }
    CriCopyString(f->name, 0x12, name);
    CriFilInit(f);
    return f;
}

void CriFilDealloc(CriFil *f)
{
    if (f == 0) return;
    CriFilResetState(f);
    if (f->used == 1) {
        f->used = 0;
        CriFilClear(f);
    }
}

int CriFilSetPos(CriFil *f, int off, int whence)
{
    int p;
    int n;
    if (f == 0) {
        CriFilCallErrorCallback(0, data_020ae1ac);
        return 0;
    }
    CriFilLockEnter();
    if (whence == 0) {
        f->pos = off;
    } else if (whence == 2) {
        f->pos = f->nsec + off;
    } else if (whence == 1) {
        f->pos = f->pos + off;
    }
    n = f->nsec;
    p = f->pos;
    if (p >= n) p = n;
    f->pos = p;
    if (p <= 0) p = 0;
    f->pos = p;
    CriFilLockLeave();
    return f->pos;
}

int CriFilGetPos(CriFil *f)
{
    if (f != 0) return f->pos;
    CriFilCallErrorCallback(0, data_020ae1c8);
    return 0;
}

int CriFilRead(CriFil *f, int n, char *buf)
{
    int a;
    int b;
    int rem;
    unsigned sz;
    unsigned base;
    if (f == 0) {
        CriFilCallErrorCallback(0, data_020ae1e4);
        return 0;
    }
    if (n < 0) {
        CriFilCallErrorCallback((int)f, data_020ae200);
        return 0;
    }
    if (buf == 0) {
        CriFilCallErrorCallback((int)f, data_020ae220);
        return 0;
    }
    if (n == 0) {
        f->state = 1;
        return 0;
    }
    if (f->state == 2) return 0;
    CriFilLockEnter();
    f->i14 = 0;
    rem = f->nsec - f->pos;
    if (n >= rem) n = rem;
    f->cnt = n;
    a = f->pos * f->ssize;
    b = f->cnt * f->ssize;
    if (b == 0) {
        f->state = 1;
        CriFilLockLeave();
        return 0;
    }
    f->off = a;
    f->len = b;
    f->state = 2;
    base = CriParseHexPair(f->name, &sz);
    rem = sz - f->off;
    if (f->len <= rem) rem = f->len;
    CriFilLockLeave();
    func_02012464(buf, base + f->off, f->len);
    func_02012484(buf + rem, 0, f->len - rem);
    CriFilLockEnter();
    f->i14 = f->cnt * f->ssize;
    f->pos = f->pos + f->cnt;
    f->state = 1;
    CriFilLockLeave();
    return f->cnt;
}

void CriFilResetState(CriFil *f)
{
    if (f == 0) {
        CriFilCallErrorCallback(0, data_020ae244);
        return;
    }
    CriFilLockEnter();
    f->state = 0;
    CriFilLockLeave();
}

int CriFilGetState(CriFil *f)
{
    if (f != 0) return f->state;
    CriFilCallErrorCallback(0, data_020ae25c);
    return 0;
}

int CriFilGetBlockSize(CriFil *f)
{
    if (f != 0) return f->ssize;
    CriFilCallErrorCallback(0, data_020ae274);
    return 0;
}

void CriFilSetBlockSize(CriFil *f, int ss)
{
    int t;
    if (f == 0) {
        CriFilCallErrorCallback(0, data_020ae28c);
        return;
    }
    t = f->pos * f->ssize;
    f->ssize = ss;
    f->nsec = (f->i8 + (f->ssize - 1)) / f->ssize;
    f->pos = t / f->ssize;
    f->i14 = f->cnt * ss;
}

int CriFilGetLastReadSize(CriFil *f)
{
    if (f != 0) return f->i14;
    CriFilCallErrorCallback(0, data_020ae2a4);
    return 0;
}

int func_02049960(CriFil *f, int id)
{
    if (f == 0) return 0;
    switch (id) {
    case 300: return f->i8;
    case 301: return 0;
    case 302: return f->i8;
    case 200: return 0;
    case 201: return CriFilGetLastReadSize(f);
    case 202: return 0;
    case 203: return CriParseHexPairSecond((char *)f);
    case 204: return 0;
    case 205: return CriParseHexPairSecond((char *)f);
    case 299: return 0;
    case 600: return 1;
    case 400: return 0;
    }
    return -1;
}

void func_02049a58(void)
{
    __clear(data_02218888, sizeof(data_02218888));
    data_02218884 = 0;
    func_02049ad4(0, 1);
    func_02049ab0(2);
}

void func_02049aac(void)
{
}

void func_02049ab0(short v)
{
    int i;
    for (i = 0; i < 8; i++) {
        data_02218888[i].s2c = v;
    }
}

void func_02049ad4(CriPool *p, short v)
{
    int i;
    if (p == 0) {
        for (i = 0; i < 8; i++) {
            data_02218888[i].s26 = v;
            data_02218888[i].s28 = 8;
        }
    } else {
        p->s26 = v;
        p->s28 = 8;
    }
}

int func_02049b18(CriPool *p)
{
    return p->s26;
}

CriPool *func_02049b20(int a, int b, unsigned char c, ...)
{
    int i;
    int o;
    int r5;
    for (i = 0; i < 8; i++) {
        if (data_02218888[i].used == 0) break;
    }
    if (i == 8) return 0;
    o = i * 0x90;
    InitSndStrm(data_02218888[i].h34);
    if (ReserveSndStrmChannels(data_02218888[i].h34, 1, &c) == 0) return 0;
    r5 = a * 0x1800 + b;
    data_02218888[i].i4 = r5;
    data_02218888[i].i8 = 16;
    data_02218888[i].s18 = 0x1800;
    data_02218888[i].used = 1;
    data_02218888[i].s2 = c;
    return &data_02218888[i];
}

void func_02049c04(CriPool *p)
{
    if (p == 0 || p->used == 0) return;
    SndOutStop(p);
    UnreserveSndStrmChannels(p->h34);
    p->i30 = 0;
    p->used = 0;
}
