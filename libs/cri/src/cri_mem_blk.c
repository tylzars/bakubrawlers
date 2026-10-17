/* CRI audio: CriMemBlk, a 0xc-byte object with a vtable around one Adx object (its other functions are in
   cri_adx_vol.c), 0x02050e60-0x02050f68 */
typedef struct CriMemSrc {
    int a;                            /* 0x00 */
    int b;                            /* 0x04 */
    int c;                            /* 0x08 */
} CriMemSrc;

/* vtable data_0209e4ac: func_02050ebc, func_02050f24, func_02050f48, func_02050f58 here, then func_02050f68..
   func_020510a4 (cri_adx_vol.c) */
typedef struct CriMemBlk {
    void *vt;                         /* 0x00, data_0209e4ac, set by func_020510ac */
    void *adx;                        /* 0x04, the Adx object from AdxAlloc / AdxAllocNch1 */
    int src;                          /* 0x08 */
} CriMemBlk;

extern char data_020b0078[], data_020b00ac[], data_020b00e4[];
extern void SendErrWrapper(char *);
extern void func_020510ac(CriMemBlk *);
extern void *AdxAllocNch1(int, int);
extern void *AdxAlloc(int, int, int);
extern void AdxDealloc(void *);
extern void func_0204375c(void *, int);
extern void func_020436b4(void *, int, int, int);

CriMemBlk *func_02050e60(CriMemBlk *m, unsigned size)
{
    if (m == 0) {
        SendErrWrapper(data_020b0078);
        return 0;
    }
    if (size < 0xc) {
        SendErrWrapper(data_020b00ac);
        return 0;
    }
    func_020510ac(m);
    return m;
}

void func_02050ea8(CriMemBlk *m)
{
    if (m == 0) {
        return;
    }
    func_020510ac(m);
}

int func_02050ebc(CriMemBlk *m, CriMemSrc *s, int a, int b)
{
    if (s->c != 0) {
        m->adx = AdxAllocNch1(a, b - 0xc);
    } else {
        m->adx = AdxAlloc(s->a, a, b - 0xc);
    }
    if (m->adx == 0) {
        SendErrWrapper(data_020b00e4);
        return 0;
    }
    m->src = s->a;
    return 1;
}

void func_02050f24(CriMemBlk *m)
{
    if (m->adx == 0) {
        return;
    }
    AdxDealloc(m->adx);
    m->adx = 0;
}

void func_02050f48(CriMemBlk *m, int a)
{
    func_0204375c(m->adx, a);
}

void func_02050f58(CriMemBlk *m, int a, int b, int c)
{
    func_020436b4(m->adx, a, b, c);
}
