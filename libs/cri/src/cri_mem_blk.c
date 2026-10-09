/* CRI audio: memory block wrapper, 0x02050e60-0x02050f68 */
typedef struct CriMemSrc {
    int a;                            /* 0x00 */
    int b;                            /* 0x04 */
    int c;                            /* 0x08 */
} CriMemSrc;

typedef struct CriMemBlk {
    int pad0;
    void *buf;                        /* 0x04 */
    int src;                          /* 0x08 */
} CriMemBlk;

extern char data_020b0078[], data_020b00ac[], data_020b00e4[];
extern void func_0204f360(char *);
extern void func_020510ac(CriMemBlk *);
extern void *func_02042ab8(int, int);
extern void *func_02042760(int, int, int);
extern void func_02042b24(void *);
extern int func_0204375c(void *);
extern int func_020436b4(void *);

CriMemBlk *func_02050e60(CriMemBlk *m, unsigned size)
{
    if (m == 0) {
        func_0204f360(data_020b0078);
        return 0;
    }
    if (size < 0xc) {
        func_0204f360(data_020b00ac);
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
        m->buf = func_02042ab8(a, b - 0xc);
    } else {
        m->buf = func_02042760(s->a, a, b - 0xc);
    }
    if (m->buf == 0) {
        func_0204f360(data_020b00e4);
        return 0;
    }
    m->src = s->a;
    return 1;
}

void func_02050f24(CriMemBlk *m)
{
    if (m->buf == 0) {
        return;
    }
    func_02042b24(m->buf);
    m->buf = 0;
}

int func_02050f48(CriMemBlk *m)
{
    return func_0204375c(m->buf);
}

int func_02050f58(CriMemBlk *m)
{
    return func_020436b4(m->buf);
}
