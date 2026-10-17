/* CRI audio: sound player object (0xa2c bytes) */
typedef unsigned char u8;
typedef unsigned short u16;
typedef signed short s16;
typedef unsigned int u32;

typedef struct SndGlob {
    int i00;
    int cnt;
    int i08;
    int i0c;        /* 0x0c */
    int users;
} SndGlob;
extern SndGlob data_02218d0c;
extern int func_0204acac(s16 *, s16 *, s16 *);

/* ---- player object (0xa2c bytes) ---- */
typedef struct Bytes8 { u8 b[8]; } Bytes8;
typedef struct Pair16 { u16 a, b; } Pair16;
typedef struct SndInfo {
    union {
        struct { int flag, i04; };
        u8 b[8];
    };
    int i08, i0c;
    u8 pad10[0x0c];
    Pair16 p1c, p20, p24, p28, p2c, p30;
    union {
        Bytes8 tail;    /* 0x34 */
        struct { u16 w34, w36, w38, w3a; };
    };
} SndInfo;

typedef struct SndPlayer {
    u8 *buf0;               /* 0x000 */
    u8 pad04[0x1a0];
    int i1a4;
    signed char b1a8, b1a9, b1aa, b1ab;
    int i1ac;
    int src;                /* 0x1b0 */
    void *h;                /* 0x1b4 */
    u8 a1b8[0x30];
    SndInfo info;           /* 0x1e8 */
    u8 pad224[0x224 - 0x1e8 - sizeof(SndInfo)];
    u8 a224[0x100];
    u8 a324[0x100];
    u8 a424[0x300];
    u8 a724[0x300];
    Bytes8 tail;            /* 0xa24 */
} SndPlayer;

extern void *memset(void *, int, int);
extern void func_0204d814(void);
extern void func_0204d824(void);
extern void func_0204d834(void);
extern void func_0204d8e4(void);
extern void *func_0204d8fc(void *, int);
extern void func_0204d944(void *);
extern int func_0204d78c(void *, void *, int, void *);
extern int func_0204e7a0(int);
extern int CriBitsCanRead(int, int);
extern int func_0204b344(int, void *);
extern void func_0204b44c(void *, void *);
extern void func_0204b4f8(int, void *, void *);
extern void func_0204b5f4(int, void *, void *, void *, void *);
extern void func_0204b880(int, void *, void *, void *, int, void *, int);


typedef struct SndHdr {
    u8 b0, b1, b2;
    signed char b3;
    int i4;
    int i8;
    u8 pad0c[0x24 - 0x0c];
    Pair16 p24, p28, p2c, p30, p34, p38;
    u8 pad3c[0x3d - 0x3c];
    signed char b3d;
    u8 pad3e[2];
} SndHdr;

extern u8 data_02218d20[];
extern u8 data_02218d24[];
extern u8 CriBitsRead(int, int);
extern int CriHdrParse(u8 *, int, int *, SndHdr *);


SndPlayer *SndPlayerCreate(SndPlayer *p, u32 size)
{
    u8 *a = (u8 *)(((u32)p + 7) & ~7);
    u8 *e;
    memset(a, 0, 0xa2c);
    e = a + 0x22c;
    e = e + 0x800;
    e = (u8 *)(((u32)e + 7) & ~7);
    if ((u32)(e + 0x81c) < size) return 0;
    p->h = func_0204d8fc(e, 0x81c);
    if (p->h == 0) return 0;
    p->buf0 = (u8 *)(((u32)p + 0x23) & ~0x1f);
    p->info.b[0] = 0;
    p->info.b[1] = 0;
    p->info.b[2] = 0;
    p->info.b[3] = 0;
    p->info.b[4] = 0;
    p->info.b[5] = 0;
    p->info.b[6] = 0;
    p->info.b[7] = 0;
    p->i1a4 = data_02218d0c.i0c;
    p->b1ab = 0x10;
    p->b1a8 = 1;
    return p;
}

void SndPlayerRelease(SndPlayer *p)
{
    void *h = p->h;
    if (h != 0) {
        p->h = 0;
        func_0204d944(h);
    }
    memset(p, 0, 0xa2c);
}

void SndPlayerReset(SndPlayer *p)
{
    void *h = p->h;
    memset(p->a1b8, 0, 0x30);
    memset(&p->info, 0, 0x3c);
    memset(p->a224, 0, 0x100);
    memset(p->a324, 0, 0x100);
    memset(p->a424, 0, 0x300);
    memset(p->buf0, 0, 0x180);
    memset(p->a724, 0, 0x300);
    p->b1a9 = 0;
    p->i1ac = 0;
    if (p->h == 0) return;
    {
        void *t = p->h;
        p->h = 0;
        func_0204d944(t);
    }
    p->h = func_0204d8fc(h, 0x81c);
}

void SndPlayerResetAndSetSrc(SndPlayer *p, int src)
{
    SndPlayerReset(p);
    p->src = src;
}

/* the tail copy is the __memcpy intrinsic: a struct copy swaps the source and destination registers */
void SndPlayerReadHdr(SndPlayer *p)
{
    u8 *end;
    SndInfo *info = &p->info;
    u8 *q;
    int h = p->src;
    if (h == 0) return;
    {
        SndHdr hdr = {0};
        int len = 0;
        q = data_02218d20;
        end = data_02218d24;
        while (q != end) { *q = CriBitsRead(h, 8); q++; }
        CriHdrParse(data_02218d20, 4, &len, 0);
        end += len;
        while (q != end) { *q = CriBitsRead(h, 8); q++; }
        if (CriHdrParse(data_02218d20, 0x200, &len, &hdr) < 0) return;
        info->i04 = hdr.b3;
        info->i08 = hdr.i4;
        info->i0c = hdr.i8;
        info->p1c = hdr.p24;
        info->p20 = hdr.p28;
        info->p24 = hdr.p2c;
        info->p28 = hdr.p30;
        info->p2c = hdr.p34;
        info->p30 = hdr.p38;
        info->flag = 1;
        if (hdr.b3d >= 0x10) {
            info->w34 = 0;
            func_0204acac((s16 *)&info->w36, (s16 *)&info->w38, (s16 *)&info->w3a);
            return;
        }
        if (hdr.b3d >= 8) {
            __memcpy(&info->tail, &p->tail, 8);
            return;
        }
        info->tail.b[0] = 0;
        info->tail.b[1] = 0;
        info->tail.b[2] = 0;
        info->tail.b[3] = 0;
        info->tail.b[4] = 0;
        info->tail.b[5] = 0;
        info->tail.b[6] = 0;
        info->tail.b[7] = 0;
    }
}

int func_0204b150(SndPlayer *p)
{
    if (p->src == 0) return -1;
    {
        int r = func_0204e7a0(p->src);
        if (r == -1) return -1;
        if (r != 1) return 1;
    }
    if (CriBitsCanRead(p->src, 0x14) == 0) return -1;
    if (func_0204b344(p->src, p->a1b8) < 0) return -1;
    func_0204b44c(p->a1b8, &p->info);
    func_0204b4f8(p->src, &p->info, p->a224);
    func_0204b5f4(p->src, &p->info, p->a224, p->a324, p->a424);
    p->b1a9 = 1;
    p->i1ac = 0;
    return 0;
}

int func_0204b224(SndPlayer *p)
{
    return p->i1ac == 12;
}

int func_0204b238(SndPlayer *p, u8 *dst)
{
    int i, off;
    if (p->i1ac == 12) return 0;
    func_0204b880(p->src, &p->info, p->a224, p->a424, p->i1ac >> 2, p->buf0, p->i1a4);
    for (i = 0, off = 0; i < 3; i++) {
        func_0204d78c(p->h, p->buf0 + off, 0, dst);
        dst += 0x40;
        off += 0x40;
    }
    p->i1ac++;
    return 0x60;
}

int func_0204b2dc(SndPlayer *p)
{
    return p->b1ab;
}

int func_0204b30c(SndPlayer *p);
int func_0204b2e8(SndPlayer *p)
{
    return func_0204b30c(p);
}

int func_0204b2f4(SndPlayer *p)
{
    if (p->b1a9 == 0) return 0;
    return p->info.i0c;
}

int func_0204b30c(SndPlayer *p)
{
    return 0x60;
}
