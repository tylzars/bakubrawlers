/* CRI middleware, 0x020477d8-0x02047d80 */
typedef struct CriUsrA { int p0; int p4; int p8; int pc; int p10; } CriUsrA;      /* 0x02217a5c */
typedef struct CriUsrB { int p0[3]; int flag; int p10[4]; int mode; } CriUsrB;    /* 0x02217a9c */
struct CriH { int a, b, c, flags; };
typedef struct CriFile {
    char used;           /* 0x00 */
    char b1;
    char pad2[0x4a];
    struct CriH *p4c;
    int st;              /* 0x50 */
    int p54;
    int sz;              /* 0x58 */
    int p5c;
    int pad60;
    int p64, p68, p6c;
    int pad70;
    int p74;
    int pad78[3];
} CriFile;
typedef struct CriFiles {
    CriFile ent[10];     /* 0x000 */
    char drive[2];       /* 0x528 -- accessed as ent+0x500+0x28 */
    char pad[0x1fe];
    void (*cb)(void *, const char *, int);  /* 0x728 */
    void *cbarg;         /* 0x72c */
    int f730, f734;
} CriFiles;

extern CriUsrA data_02217a5c;
extern CriUsrB data_02217a9c;
extern CriFiles data_02217d14;
extern char data_02217ac0[], data_020ade08[], data_0221823c[], data_020ade34[], data_020ade3c[];
extern char data_020add9c[];
extern char data_0209e390[];
extern void *data_020ade04;

extern int func_02047798(void);
extern void func_0204e8ac(void);
extern void OS_WakeupThreadDirect(void *);
extern void func_02048304(void), func_02048398(void), func_020482f8(void), ChAlloc(void);
extern void func_02048718(int), ChDealloc(int);
extern void func_0204871c(void), ChSetSndOutStarted(void), ChGetFilledSamples(void), ChGetSpaceSamples(void);
extern void func_02048930(void), ChAdvanceAll(void), func_02048f2c(void), func_02048f38(void);
extern void ChSetVolume(void), func_02049088(void), func_02049118(void), func_02049124(void);
extern void func_02049138(void), func_02049150(void);
extern char *func_02015a6c(char *, const char *);
extern char *strcat(char *, const char *);
extern int FS_ReadFile(int, int, int); /* declared with int parameters; fs_file.c takes (FSFile *, void *, int) */
extern int FS_ReadFileAsync(int, int, int); /* the same for FS_ReadFileAsync */
extern void CriLockEnter1(void), CriLockLeave1(void);
extern int func_02048274(char *);
extern int func_02048264(int), func_0204826c(int);
extern int func_02047cf8(CriFile *);
extern int memcmp(const void *, const char *, int);
extern int sscanf(const char *, const char *, ...);

extern void func_020478a8(void);

int CriGetThread0Done(void) { return data_02217a9c.flag; }
void CriSetThread0Done(void) { data_02217a9c.flag = 1; }
void CriClearThread0Done(void) { data_02217a9c.flag = 0; }

int func_02047810(void)
{
    if (func_02047798() == 0) return 0;
    if ((unsigned)data_02217a9c.mode <= 1) return 0;
    if (CriGetThread0Done() != 0) return 0;
    if (data_02217a5c.p10 == 0) {
        data_02217a5c.p10 = 1;
        func_020478a8();
    }
    func_0204e8ac();
    OS_WakeupThreadDirect(data_02217ac0);
    return 0;
}

void func_02047884(void)
{
    if (data_02217a5c.p0 == 0) return;
    ((void (*)(int))data_02217a5c.p0)(data_02217a5c.p4);
}

void func_020478a8(void)
{
    if (data_02217a5c.p8 == 0) return;
    ((void (*)(int))data_02217a5c.p8)(data_02217a5c.pc);
}

void func_020478cc(void) { func_02048304(); }
void func_020478d8(void) { func_02048398(); }
void func_020478e4(void) { func_020482f8(); }
void func_020478f0(void) { ChAlloc(); }

void func_020478fc(int a)
{
    func_02048718(a);
    ChDealloc(a);
}

void func_02047914(void) { func_0204871c(); }
void func_02047920(void) { ChSetSndOutStarted(); }
void func_0204792c(void) { ChGetFilledSamples(); }
void func_02047938(void) { ChGetSpaceSamples(); }
void func_02047944(void) { func_02048930(); }
void func_02047950(void) { ChAdvanceAll(); }
void func_0204795c(void) { func_02048f2c(); }
void func_02047968(void) { func_02048f38(); }
void func_02047974(void) { ChSetVolume(); }
void func_02047980(void) { func_02049088(); }
void func_0204798c(void) { func_02049118(); }
void func_02047998(void) { func_02049124(); }
void func_020479a4(void) { func_02049138(); }
void func_020479b0(void) { func_02049150(); }

void func_020479bc(void) { }
int func_020479c0(void) { return 0; }

void FObjCallCallback(int code, const char *msg)
{
    if (data_02217d14.cb == 0) return;
    data_02217d14.cb(data_02217d14.cbarg, msg, code);
}

const char *func_020479f0(void) { return data_020add9c; }

void func_020479fc(void)
{
    data_020ade04 = data_0209e390;
    __clear(&data_02217d14, 0x738);
}

void FObjBuildPath(char *dst, char *src)
{
    if (src == 0) {
        func_02015a6c(dst, data_0221823c);
    } else if (src[0] == 0) {
        func_02015a6c(dst, data_0221823c);
    } else if (src[0] == 0x5c) {
        dst[0] = data_02217d14.drive[0];
        dst[1] = data_02217d14.drive[1];
        dst[2] = 0;
    } else if (src[1] == 0x3a) {
        dst[0] = 0;
    } else {
        func_02015a6c(dst, data_0221823c);
    }
    if (src == 0) return;
    strcat(dst, src);
}

int func_02047ae8(CriFile *o)
{
    int r = 0;
    if (data_02217d14.f734 == 0) {
        if (data_02217d14.f730 != 0) {
            r = FS_ReadFile((int)o->p4c, o->p54, o->p74);
        } else {
            r = FS_ReadFileAsync((int)o->p4c, o->p54, o->p74);
        }
        if (o->p74 < 0 || r < 0) {
            r = 0;
        } else {
            o->p74 = r;
            o->st = 2;
        }
    }
    return (r + o->sz - 1) / o->sz;
}

static inline int criIsFlag(struct CriH *h) { int r; if (h->flags & 1) r = 1; else r = 0; return r; }

void func_02047b60(CriFile *o)
{
    int t = criIsFlag(o->p4c);
    if (t != 0) return;
    CriLockEnter1();
    o->st = 0;
    o->b1 = 1;
    o->p64 = o->p64 + o->p74 / o->sz;
    o->p68 = o->p74;
    o->p6c = 0;
    CriLockLeave1();
}

void func_02047bc8(CriFile *o)
{
    if (o->st == 1) func_02047ae8(o);
    if (o->st != 2) return;
    func_02047b60(o);
}

void func_02047bf8(void)
{
    int i;
    CriFile *f = data_02217d14.ent;
    for (i = 0; i < 10; i++, f++) {
        if (f->used == 1) func_02047bc8(f);
    }
}

void FObjSetCallback(void (*cb)(void *, const char *, int), void *arg)
{
    data_02217d14.cb = cb;
    data_02217d14.cbarg = arg;
}

int func_02047c44(char *path)
{
    char buf[0x200];
    int r;
    if (path == 0) {
        FObjCallCallback(0, data_020ade08);
        return 0;
    }
    FObjBuildPath(buf, path);
    r = func_02048274(buf);
    if (r < 0) return 0x7ffff800;
    return r;
}

int func_02047c94(int a, int code)
{
    switch (code) {
    case 0x12c: return func_02047cf8(a);
    case 0xc8: return func_02048264(a);
    case 0xc9: return func_0204826c(a);
    case 0x190: return 1;
    }
    return -1;
}

int func_02047cf8(CriFile *o) { return o->p5c; }

int func_02047d00(char *path, int *a, int *b, int *c)
{
    if (memcmp(data_020ade34, path, 7) == 0) {
        if (sscanf(path, data_020ade3c, a, b, c) == 3) {
            if (*c == 0) *c = 0x7fffffff;
            return 1;
        }
    }
    *a = 0;
    *b = 0;
    *c = 0;
    return 0;
}
