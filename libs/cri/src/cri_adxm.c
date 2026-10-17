/* CRI "ADX mixer" high level API and error output, 0x02051804-0x02052320 */

typedef struct CriMemCfg {
    void *(*alloc)(unsigned);
    void (*free)(void *);
} CriMemCfg;

typedef struct CriOpen {           /* argument block of func_02051ae4 */
    CriMemCfg *mem;
    void *buf;
    int mode;
    int f0c;
    int f10;
} CriOpen;

typedef struct CriGlob {           /* 0x02218fd0 */
    unsigned char init;
    char pad1[3];
    signed char sflag;             /* 0x04 */
    unsigned char count;           /* 0x05 */
    signed char ch;                /* 0x06 */
    char pad7[0x1a1];
    void *f1a8;
    CriMemCfg mem;                 /* 0x1ac */
    void *f1b4;
    void *f1b8;
    void *f1bc;
    char pad1c0[0x40];
    void (*err)(const char *);     /* 0x200 */
} CriGlob;

struct CriElem;
typedef struct CriHdl { int pad; struct CriElem *e; } CriHdl;
typedef struct CriElem {           /* 0x68 bytes, array at 0x02218fd8 */
    void *h;                       /* 0x00 */
    void *work;                    /* 0x04 */
    CriHdl hdl;                    /* 0x08, handed out to callers */          /* 0x0c */
    CriMemCfg mem;                 /* 0x10 */
    int f18;
    int os[3];                     /* 0x1c */
    unsigned flags;                /* 0x28 */
    char rest[0x3c];
} CriElem;
typedef struct CriElemW { int pad; void *used; char rest[0x60]; } CriElemW;  /* same array seen from 0x02218fd4 */

typedef struct CriVolSet { int a, b, c; void *p1, *p2, *p3; int s1, s2, s3; } CriVolSet;

extern CriGlob data_02218fd0;
extern CriElemW data_02218fd4[4];
extern CriElem data_02218fd8[4];
typedef struct CriFreeCfg { int pad; void (*free)(void *); } CriFreeCfg;
extern CriFreeCfg data_0221917c;
extern char data_02219190[];
extern const char *data_020b230c[];
extern char data_020b2440[], data_020b2454[], data_020b2474[], data_020b2490[], data_020b24a0[];

extern void MI_CpuFill8(void *, int, unsigned);
extern int sprintf(char *, const char *, ...);
extern void func_0204713c(void);
extern void func_02047414(int, void *);
extern void CriInit(void);
extern void GlobalPoolAddRef(void);
extern void func_0204ecfc(void);
extern void func_02047408(void (*)(int, const char *), int);
extern void func_020483f0(const void *, int);
extern void func_020522e4(void);
extern void CriGlobTextCallback(int, const char *);
extern void CriGlobSendErr(const char *, const char *);
extern void func_02052208(void);
extern void GlobalPoolReleaseRef(void);
extern void CriExit(void);
extern void CriStopThreads(void);
extern void func_02047224(void);
extern void func_02047810(int);
extern void func_020471d0(void);
extern int GlobalPoolComputeWorkSize(void *);
extern void *GlobalPoolAllocObj(void *, void *, int);
extern void func_0204eb54(void *, int);
extern void func_0204eb84(void *, int);
extern void func_0204ebb4(void *, int);
extern void func_0204ebe4(void *, int);
extern void func_0204ec14(void *, int);
extern void func_0204ec44(void *);
extern int func_0204eccc(void *);
extern void *func_0204ec94(void *, int);
extern void func_020491c4(void *, int);
extern void FS_InitFile(void *);
extern void func_0204ec6c(void *);
extern void GlobalPoolDeallocObj(void *);
extern int func_0204eaf4(void *);
extern void func_0204ea6c(void *, const char *);
extern void func_0204ea9c(void *);
extern void func_0204eac4(void *, int);
extern void func_0204eb24(void *, int);
extern void FS_CloseFile(void *);
extern double log10(double);
CriElem *func_02052248(CriHdl *hdl);
void func_020522a0(CriElem *e);
void *func_02051ae4(CriOpen *p);
void CriGlobInit(CriMemCfg *cfg, int a, const void *b, void (*c)(const char *), int flag);

void CriGlobInitWithThreads(CriMemCfg *cfg, int a, const void *b, void (*c)(const char *))
{
    CriGlobInit(cfg, a, b, c, 1);
}

void CriGlobInit(CriMemCfg *cfg, int a, const void *b, void (*c)(const char *), int flag)
{
    if (data_02218fd0.init != 0) {
        data_02218fd0.init++;
        return;
    }
    MI_CpuFill8(data_02218fd4, 0, 0x200);
    if (c == 0) { data_02218fd0.err = (void (*)(const char *))func_020522e4; } else { data_02218fd0.err = c; }
    if (cfg == 0) {
        CriGlobSendErr(data_020b2440, data_020b230c[3]);
        return;
    }
    func_0204713c();
    data_02218fd0.mem = *cfg;
    if (flag == 0) {
        func_02047414(1, 0);
        data_02218fd0.sflag = 0;
    } else {
        CriVolSet s;
        s.a = 12;
        s.b = 14;
        s.c = 24;
        data_02218fd0.f1b4 = cfg->alloc(0x1000);
        data_02218fd0.f1b8 = cfg->alloc(0x1000);
        data_02218fd0.f1bc = cfg->alloc(0x1000);
        s.p1 = data_02218fd0.f1b4;
        s.p2 = data_02218fd0.f1b8;
        s.p3 = data_02218fd0.f1bc;
        s.s1 = 0x1000;
        s.s2 = 0x1000;
        s.s3 = 0x1000;
        func_02047414(2, &s);
        data_02218fd0.sflag = 1;
    }
    CriInit();
    GlobalPoolAddRef();
    func_0204ecfc();
    func_02047408(CriGlobTextCallback, 0);
    func_020483f0(b, a);
    data_02218fd0.ch = 2;
    data_02218fd0.init = 1;
}

void CriGlobExit(void)
{
    if (data_02218fd0.init == 0) {
        return;
    }
    if (data_02218fd0.init == 1) {
        func_02052208();
        GlobalPoolReleaseRef();
        CriExit();
        CriStopThreads();
        if (data_02218fd0.sflag != 0) {
            data_02218fd0.mem.free(data_02218fd0.f1bc);
            data_02218fd0.mem.free(data_02218fd0.f1b8);
            data_02218fd0.mem.free(data_02218fd0.f1b4);
        }
    }
    data_02218fd0.init--;
}

void CriGlobUpdate(void)
{
    if (data_02218fd0.init != 0) {
        func_02047224();
    }
}

void func_02051a5c(void)
{
    if (data_02218fd0.init != 0 && data_02218fd0.sflag != 0) {
        func_02047810(0);
    }
}

void func_02051a88(void)
{
    if (data_02218fd0.init != 0 && data_02218fd0.sflag != 0) {
        func_020471d0();
    }
}

void *CriHdlAlloc(CriMemCfg *cfg, int a, int b)
{
    CriOpen o;
    o.mem = cfg;
    o.buf = 0;
    o.mode = a;
    o.f0c = b;
    o.f10 = 1;
    return func_02051ae4(&o);
}

void *func_02051ae4(CriOpen *p)
{
    CriElem *e;
    CriVolSet unused;
    struct {
        int a, b, c, d, e;
        char f;
        int g, h, i, j;
    } s;
    int size;
    int i;

    if (data_02218fd0.init == 0) {
        CriGlobSendErr(data_020b230c[8], data_020b230c[1]);
        return 0;
    }
    if (p->mem == 0 && p->buf == 0) {
        CriGlobSendErr(data_020b230c[8], data_020b230c[3]);
        return 0;
    }
    if (p->mem != 0 && (p->mem->alloc == 0 || p->mem->free == 0)) {
        CriGlobSendErr(data_020b230c[8], data_020b230c[3]);
        return 0;
    }
    if (p->mode == 0 || (unsigned)p->mode > 2) {
        CriGlobSendErr(data_020b230c[8], data_020b2454);
        return 0;
    }
    e = func_02052248(0);
    if (e == 0) {
        return 0;
    }
    size = (data_02218fd0.ch + 1) * 0x1ff6;
    MI_CpuFill8(&s, 0, 0x28);
    s.a = 1;
    s.b = p->mode;
    s.c = size;
    s.d = 0;
    s.e = 1;
    s.f = 1;
    s.g = p->f0c;
    s.i = p->f10;
    s.h = 0;
    size = GlobalPoolComputeWorkSize(&s);
    if (p->mem == 0) {
        e->work = p->buf;
    } else {
        e->work = p->mem->alloc(size);
    }
    e->h = GlobalPoolAllocObj(&s, e->work, size);
    if (e->h == 0) {
        CriGlobSendErr(data_020b230c[8], data_020b2474);
        return 0;
    }
    func_0204eb54(e->h, 0x10);
    func_0204eb84(e->h, 0);
    func_0204ebb4(e->h, 0);
    func_0204ebe4(e->h, 0);
    func_0204ec14(e->h, 0);
    if (p->f10 != 0) {
        func_0204ec44(e->h);
    }
    e->f18 = p->f10;
    if (p->mem != 0) {
        e->mem = *p->mem;
    } else {
        e->mem.alloc = 0;
        e->mem.free = 0;
    }
    e->hdl.e = e;
    for (i = 0; i < func_0204eccc(e->h); i++) {
        func_020491c4(func_0204ec94(e->h, i), 1);
    }
    FS_InitFile(e->os);
    data_02218fd0.count++;
    return &e->hdl;
}

void CriHdlDealloc(CriHdl *hdl)
{
    CriElem *e;
    if (data_02218fd0.init == 0) {
        CriGlobSendErr(data_020b230c[9], data_020b230c[1]);
        return;
    }
    if (hdl == 0) {
        CriGlobSendErr(data_020b230c[9], data_020b230c[5]);
        return;
    }
    e = func_02052248(hdl);
    if (e == 0) {
        CriGlobSendErr(data_020b230c[9], data_020b2490);
        return;
    }
    if (e->f18 != 0) {
        func_0204ec6c(e->h);
    }
    GlobalPoolDeallocObj(e->h);
    e->h = 0;
    if (e->mem.free != 0) {
        e->mem.free(e->work);
    }
    func_020522a0(e);
    data_02218fd0.count--;
}

int CriHdlGetState(CriHdl *hdl)
{
    if (data_02218fd0.init == 0) {
        CriGlobSendErr(data_020b230c[11], data_020b230c[1]);
        return 0;
    }
    if (hdl == 0) {
        CriGlobSendErr(data_020b230c[11], data_020b230c[5]);
        return 0;
    }
    {
        int r = func_0204eaf4(func_02052248(hdl)->h);
        if (r == 0) return 0;
        if (r == 1) return 1;
        if (r == 2) return 2;
        if (r == 3) return 3;
        return 0;
    }
}

void CriHdlStart(CriHdl *hdl, const char *path)
{
    CriElem *e;
    if (data_02218fd0.init == 0) {
        CriGlobSendErr(data_020b230c[10], data_020b230c[1]);
        return;
    }
    if (hdl == 0) {
        CriGlobSendErr(data_020b230c[10], data_020b230c[5]);
        return;
    }
    if (path == 0) {
        CriGlobSendErr(data_020b230c[10], data_020b230c[4]);
        return;
    }
    e = func_02052248(hdl);
    func_020522a0(e);
    func_0204ea6c(e->h, path);
}

void CriHdlStop(CriHdl *hdl)
{
    if (data_02218fd0.init == 0) {
        CriGlobSendErr(data_020b230c[2], data_020b230c[1]);
        return;
    }
    if (hdl == 0) {
        CriGlobSendErr(data_020b230c[2], data_020b230c[5]);
        return;
    }
    func_0204ea9c(func_02052248(hdl)->h);
}

void func_02051fb8(CriHdl *hdl, int a)
{
    if (data_02218fd0.init == 0) {
        CriGlobSendErr(data_020b230c[6], data_020b230c[1]);
        return;
    }
    if (hdl == 0) {
        CriGlobSendErr(data_020b230c[6], data_020b230c[5]);
        return;
    }
    func_0204eac4(func_02052248(hdl)->h, a);
}

void CriHdlSetVolumeFloat(CriHdl *hdl, float v)
{
    CriElem *e;
    int r;
    if (data_02218fd0.init == 0) {
        CriGlobSendErr(data_020b230c[7], data_020b230c[1]);
        return;
    }
    if (hdl == 0) {
        CriGlobSendErr(data_020b230c[7], data_020b230c[5]);
        return;
    }
    e = func_02052248(hdl);
    if (v < 0.01f) {
        r = -960;
    } else if (v >= 1.0f) {
        r = 0;
    } else {
        r = (int)(10.0f * (20.0f * (float)log10(v)));
    }
    func_0204eb24(e->h, r);
}

void CriHdlSetVolume(CriHdl *hdl, int a)
{
    if (data_02218fd0.init == 0) {
        CriGlobSendErr(data_020b230c[7], data_020b230c[1]);
        return;
    }
    if (hdl == 0) {
        CriGlobSendErr(data_020b230c[7], data_020b230c[5]);
        return;
    }
    func_0204eb24(func_02052248(hdl)->h, a);
}

void func_0205214c(CriHdl *hdl, unsigned a, unsigned b)
{
    CriElem *e;
    if (data_02218fd0.init == 0) {
        CriGlobSendErr(data_020b230c[0], data_020b230c[1]);
        return;
    }
    if (hdl == 0) {
        CriGlobSendErr(data_020b230c[0], data_020b230c[5]);
        return;
    }
    e = func_02052248(hdl);
    func_0204eb54(e->h, (int)((16.6f + (float)b) / 16.7f));
    func_0204eb84(e->h, (int)((16.6f + (float)a) / 16.7f));
}

void func_02052208(void)
{
    CriFreeCfg *s;
    if (data_02218fd0.init != 0 && (s = &data_0221917c, data_02218fd0.f1a8 != 0)) {
        s->free(data_02218fd0.f1a8);
        data_02218fd0.f1a8 = 0;
    }
}

CriElem *func_02052248(CriHdl *hdl)
{
    CriElem *r = 0;
    int i;
    if (hdl == 0) {
        for (i = 0; i < 4; i++) {
            if (data_02218fd4[i].used == 0) {
                r = &data_02218fd8[i];
                break;
            }
        }
    } else {
        r = hdl->e;
    }
    return r;
}

void func_020522a0(CriElem *e)
{
    int r;
    if (e->flags & 0x10) {
        r = 1;
    } else {
        r = 0;
    }
    if (r != 0) {
        FS_CloseFile(e->os);
    }
}

void CriGlobTextCallback(int unused, const char *msg)
{
    data_02218fd0.err(msg);
}

void func_020522e4(void) {}

void CriGlobSendErr(const char *a, const char *b)
{
    sprintf(data_02219190, data_020b24a0, a, b);
    data_02218fd0.err(data_02219190);
}
