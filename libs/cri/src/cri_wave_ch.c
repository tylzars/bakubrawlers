/* CRI sound channel objects, 0x02048304-0x02048e30 and 0x02048e30-0x02049150 */
typedef struct PlObj { short s0, s2; int pad[4]; int i14; } PlObj;
typedef struct Ch {
    signed char used;
    unsigned char flags;
    signed char nch, ncnt, b4;
    unsigned char b5, b6;
    signed char b7;
    int pad8;
    short s0c, s0e;
    int *src[2];          /* 0x10 */
    short s18;
    short s1a[2];
    short pad1e;
    PlObj *pl[2];         /* 0x20 */
    short *buf;           /* 0x28 */
    unsigned short s2c, s2e, s30, s32;
} Ch;

typedef struct ChGlob {
    Ch ch[4];
    short *tmp;           /* 0xd0 */
    int n;                /* 0xd4 */
    unsigned char ids[8]; /* 0xd8 */
    unsigned char use[8]; /* 0xe0 */
    void (*cb)(unsigned char *); /* 0xe8 */
} ChGlob;

extern ChGlob data_02218450;
extern int data_0221844c;
extern void *data_020ae094;
extern char data_0209e3d4[], data_020ae04c[], data_020ae070[];

extern void func_020491d0(void);
extern void func_02049a58(void);
extern void func_02049aac(void);
extern void func_02049200(void);
extern void CriMsgSendText(const char *);
extern void CopyBytes(void *, void *, int);
extern PlObj *func_02049b20(int, int, int);
extern void func_02049c04(PlObj *);
extern void SndOutSetup(PlObj *);
extern void SndOutStop(PlObj *);
extern void func_02049f30(PlObj *);
extern int func_02049b18(PlObj *);
extern short func_02049e08(PlObj *, int);
extern void SndOutClearSpace(PlObj *);
extern int SndOutGetFilledSamples(PlObj *);
extern int SndOutGetSpaceSamples(PlObj *);
extern void func_0204aa80(PlObj *, int);
extern void func_0204aab0(PlObj *, int);

typedef struct SpanV { short *p; int n; } SpanV;
typedef struct SrcVt { int v[6]; void (*get18)(void *, int, int, SpanV *); void (*put1c)(void *, int, SpanV *);
                       void (*put20)(void *, int, SpanV *); int (*get24)(void *, int); } SrcVt;
typedef struct SrcObj { SrcVt *vt; } SrcObj;
extern unsigned short func_0204a05c(PlObj *, unsigned, unsigned short *);
extern unsigned func_0204a244(PlObj *, short *, unsigned);
extern void CriCrossFadeResizeSamples(short *, int, short *, int);
extern void CriSplitSpan(SpanV *, int, SpanV *, SpanV *);
extern void func_020482e0(void);
extern void func_020482ec(void);
extern void func_02048f2c(Ch *, int);
extern void func_02048f38(Ch *, int);
extern void func_02049118(Ch *, int);
extern void func_02049024(Ch *, int);
extern void func_02049088(Ch *, int, int);
extern void func_02048808(Ch *, int);
extern void func_0204871c(Ch *, int);
extern void func_0204866c(Ch *);
extern void func_02048940(Ch *);

void func_02048304(void);
void func_02048398(void);
void func_020483f0(void *p, int n);
Ch *func_02048430(int **srcs, int n, int a2);
void func_0204866c(Ch *c);
void func_02048718(void);
int func_020487e8(Ch *c);
int func_020488d8(Ch *c);
int func_020488f8(Ch *c);
int func_02048914(Ch *c);
int func_02048930(Ch *c);
void func_02048f2c(Ch *c, int v);
void func_02049118(Ch *c, int v);
void func_02049124(Ch *c);
int func_02049138(Ch *c);
void func_0204871c(Ch *c, int m);
void func_02048808(Ch *c, int m);
void func_02048940(Ch *c);
void func_02048e30(Ch *c);
void func_02048e88(Ch *c);
void func_02048ef4(void);
void func_02048f38(Ch *c, int rate);
void func_02049024(Ch *c, int v);
void func_02049088(Ch *c, int idx, int v);

void func_02048304(void)
{
    data_020ae094 = data_0209e3d4;
    if (data_0221844c == 0) {
        func_020491d0();
        __clear(&data_02218450, sizeof(ChGlob));
        func_02049a58();
        data_02218450.n = 2;
        data_02218450.ids[0] = 4;
        data_02218450.ids[1] = 5;
    }
    data_0221844c++;
}

void func_02048398(void)
{
    int i;
    if (--data_0221844c != 0) return;
    for (i = 0; i < 4; i++) {
        if (data_02218450.ch[i].used == 1) func_0204866c(&data_02218450.ch[i]);
    }
    func_02049aac();
    func_02049200();
}

void func_020483f0(void *p, int n)
{
    if (n > 8) return;
    data_02218450.n = n;
    if (n <= 0) return;
    if (p == 0) return;
    CopyBytes(data_02218450.ids, p, n);
}

Ch *func_02048430(int **srcs, int n, int a2)
{
    Ch *c;
    int i, j;
    unsigned char id;
    PlObj *p;
    if (n <= 0) return 0;
    if (srcs == 0) return 0;
    for (i = 0; i < n; i++) {
        if (srcs[i] == 0) return 0;
    }
    for (i = 0; i < 4; i++) {
        if (data_02218450.ch[i].used == 0) break;
    }
    if (i == 4) {
        CriMsgSendText(data_020ae04c);
        return 0;
    }
    c = &data_02218450.ch[i];
    c->ncnt = n;
    c->nch = c->ncnt;
    for (i = 0; i < c->nch; i++) c->src[i] = srcs[i];
    c->s18 = 0;
    for (j = 0; j < n; j++) {
        id = 0xff;
        if (data_02218450.cb != 0) {
            data_02218450.cb(&id);
        } else {
            for (i = 0; i < data_02218450.n; i++) {
                if (data_02218450.use[i] == 0) {
                    data_02218450.use[i] = 1;
                    id = data_02218450.ids[i];
                    break;
                }
            }
        }
        if (id == 0xff) {
            func_0204866c(c);
            return 0;
        }
        p = func_02049b20(j, a2, id);
        if (p == 0) {
            func_0204866c(c);
            return 0;
        }
        c->pl[j] = p;
    }
    func_02048f2c(c, c->ncnt);
    func_02048f38(c, 0x7fd8);
    func_02049118(c, 0x10);
    func_02049024(c, 0);
    if (c->nch == 2) {
        func_02049088(c, 0, -15);
        func_02049088(c, 1, 15);
    } else {
        func_02049088(c, 0, 0);
    }
    c->flags = 0;
    c->used = 1;
    c->b4 = 0;
    c->b5 = 0;
    c->b6 = 0;
    c->b7 = 0;
    return c;
}

void func_0204866c(Ch *c)
{
    int i, j;
    if (c == 0) return;
    func_02048808(c, 0);
    func_0204871c(c, 0);
    for (i = 0; i < c->nch; i++) {
        for (j = 0; j < data_02218450.n; j++) {
            if (data_02218450.ids[j] == c->pl[i]->s2) {
                data_02218450.use[j] = 0;
                break;
            }
        }
        func_02049c04(c->pl[i]);
    }
    c->used = 0;
}

void func_02048718(void) {}

void func_0204871c(Ch *c, int m)
{
    int n, i;
    if (c == 0) return;
    if (m == func_020487e8(c)) return;
    if (m == 1) {
        func_020482e0();
        n = nplay(c);
        for (i = 0; i < n; i++) {
            SndOutStop(c->pl[i]);
            c->pl[i]->i14 = 0;
            SndOutSetup(c->pl[i]);
        }
        c->b6 = 2;
        c->flags |= 1;
        func_020482ec();
        c->s2e = 0;
        c->s30 = 0;
        c->s2c = 0;
    } else if (m == 0) {
        c->flags ^= 1;
    }
}

int func_020487e8(Ch *c)
{
    int r;
    if (c == 0) return -1;
    if (c->flags & 1) r = 1; else r = 0;
    return r;
}

void func_02048808(Ch *c, int m)
{
    int n, i;
    if (c == 0) return;
    if (m == func_020488d8(c)) return;
    n = nplay(c);
    if (m == 1) {
        if ((c->flags & 8) == 8) return;
        for (i = 0; i < n; i++) func_02049f30(c->pl[i]);
        c->flags = (c->flags | 2) & ~4;
    } else if (m == 0) {
        for (i = 0; i < n; i++) SndOutStop(c->pl[i]);
        c->flags ^= 2;
    }
}

int func_020488d8(Ch *c)
{
    int r;
    if (c == 0) return -1;
    if (c->flags & 2) r = 1; else r = 0;
    return r;
}

int func_020488f8(Ch *c)
{
    if (c == 0) return -1;
    return SndOutGetFilledSamples(c->pl[0]);
}

int func_02048914(Ch *c)
{
    if (c == 0) return -1;
    return SndOutGetSpaceSamples(c->pl[0]);
}

int func_02048930(Ch *c)
{
    if (c == 0) return -1;
    return c->b6;
}

// Placeholder: the repeated `r6 > 0` test changes no code but gives the original's registers. Both halves of the
// `||` branch into the get18 block, so at the backend's first local CSE that block has two predecessors and starts
// a new extended block: the uses of r6 below it are not replaced by the get24 call temporary, the copy into r6 stays
// live and the coalescer folds the temporary into r6. With r4 declared before r5, the count then takes r4, the
// pointer r5 and the get24 value r6. After colouring, the head/tail merge and the conditional optimisation drop the
// repeated compare and its branch. The same repeat on the c->b5 test works too; a different-looking second test
// (`r6 >= 1`) keeps its compare. The original's own form of this is not known.
void func_02048940(Ch *c)
{
    unsigned short so;
    unsigned short k;
    int r4;
    short *r5;
    SpanV sp[2];
    unsigned t7;
    unsigned short s2;
    unsigned short s0;
    int r7;
    int r6;
    long r8;
    int zero;
    SpanV tm[2];
    if (c->ncnt == 1) {
        if (c->s2c >= 6) {
            sp[0].p = 0;
            r4 = c->s2c;
            r5 = c->buf + c->s2e;
        } else {
            r6 = ((SrcObj *)c->src[0])->vt->get24(c->src[0], 1);
            if (r6 > 0 || r6 > 0) {
                ((SrcObj *)c->src[0])->vt->get18(c->src[0], 1, r6, &sp[0]);
                r5 = sp[0].p;
                r4 = sp[0].n;
                if (c->b5 != 0) {
                    if (r6 >= c->s32 * 2) {
                        r8 = c->s32;
                        if (r4 >= c->s32 * 2) {
                            r7 = (unsigned short)(c->s32 * 2);
                        } else {
                            CopyBytes(data_02218450.tmp, r5, r4);
                            ((SrcObj *)c->src[0])->vt->put20(c->src[0], 0, &sp[0]);
                            r7 = (unsigned short)(r8 * 2 - r4);
                            ((SrcObj *)c->src[0])->vt->get18(c->src[0], 1, r7, &sp[0]);
                            CopyBytes(data_02218450.tmp + r4 / 2, sp[0].p, sp[0].n);
                            r4 = sp[0].n;
                            r5 = data_02218450.tmp;
                        }
                    } else if (c->flags & 4) {
                        r8 = r4 / 2;
                        r7 = (unsigned short)r4;
                    } else {
                        r5 = 0;
                        r4 = 0;
                        r7 = 0;
                    }
                    if (r4 != 0) {
                        r6 = (r8 * c->b5 / 100) & ~3;
                        r4 = r6 * 2;
                        CriCrossFadeResizeSamples(r5, r8, c->buf + c->s30, r6);
                        c->s30 += r6;
                        r5 = c->buf + c->s2e;
                        c->s2c += (unsigned short)r4;
                    }
                }
            } else {
                r4 = c->s2c;
                if (r4 == 0) return;
                r5 = c->buf + c->s2e;
                sp[0].p = 0;
            }
        }
        if (c->nch == 2 && c->b4 >= 1) {
            r6 = func_0204a05c(c->pl[0], r4, &s2);
            t7 = func_0204a05c(c->pl[1], r4, &s0);
            if ((unsigned)r6 > t7) {
                r6 = t7;
                so = s0;
            } else {
                so = s2;
            }
            if (so != 0 && c->b4 != 2) {
                func_0204a244(c->pl[0], r5, so);
                func_0204a244(c->pl[1], r5, so);
            }
        } else {
            r6 = func_0204a05c(c->pl[0], r4, &so);
            if (so != 0) func_0204a244(c->pl[0], r5, so);
        }
        if (c->b5 != 0) {
            if (c->s2c != 0) {
                c->s2e += (unsigned)r6 >> 1;
                c->s2c -= r6;
                if (c->s2c < 6) {
                    if (c->s2c != 0) CopyBytes(c->buf, c->buf + c->s2e, c->s2c);
                    c->s30 = (unsigned)c->s2c >> 1;
                    c->s2e = 0;
                }
            }
        } else {
            r7 = r6;
        }
    } else {
        int a = ((SrcObj *)c->src[0])->vt->get24(c->src[0], 1);
        int b = ((SrcObj *)c->src[1])->vt->get24(c->src[1], 1);
        ((SrcObj *)c->src[0])->vt->get18(c->src[0], 1, a, &sp[0]);
        ((SrcObj *)c->src[1])->vt->get18(c->src[1], 1, b, &sp[1]);
        r4 = func_0204a05c(c->pl[0], sp[0].n, &s2);
        r7 = func_0204a05c(c->pl[1], sp[1].n, &s0);
        if ((unsigned)r4 > (unsigned)r7) {
            so = s0;
        } else {
            r7 = r4;
            so = s2;
        }
        if (so != 0) {
            func_0204a244(c->pl[0], sp[0].p, so);
            func_0204a244(c->pl[1], sp[1].p, so);
        }
    }
    if (sp[0].p == 0) return;
    zero = 0;
    for (k = zero; c->ncnt > k; k++) {
        if (r7 == sp[k].n) {
            ((SrcObj *)c->src[k])->vt->put20(c->src[k], zero, &sp[k]);
        } else if (r7 == 0) {
            ((SrcObj *)c->src[k])->vt->put1c(c->src[k], 1, &sp[k]);
        } else {
            CriSplitSpan(&sp[k], r7, &sp[k], &tm[k]);
            ((SrcObj *)c->src[k])->vt->put20(c->src[k], zero, &sp[k]);
            ((SrcObj *)c->src[k])->vt->put1c(c->src[k], 1, &tm[k]);
        }
    }
}

void func_02048e30(Ch *c)
{
    int n, i;
    n = nplay(c);
    for (i = 0; i < n; i++) SndOutClearSpace(c->pl[i]);
}

static inline int nplay(Ch *c)
{
    int n;
    if (c->b4 >= 1 && c->ncnt == 1 && c->nch == 2) n = 2; else n = c->ncnt;
    return n;
}

void func_02048e88(Ch *c)
{
    if (func_020487e8(c) == 1) {
        func_02048940(c);
        return;
    }
    if (func_020488d8(c) != 1) return;
    if (c->s2c != 0) func_02048940(c); else func_02048e30(c);
    if (SndOutGetFilledSamples(c->pl[0]) != 0) return;
    c->b6 = 0;
    func_02048808(c, 0);
}

void func_02048ef4(void)
{
    int i;
    for (i = 0; i < 4; i++) {
        if (data_02218450.ch[i].used == 1) func_02048e88(&data_02218450.ch[i]);
    }
}

void func_02048f2c(Ch *c, int v)
{
    if (c != 0) c->ncnt = v;
}

void func_02048f38(Ch *c, int rate)
{
    int i;
    if (c == 0) return;
    if (func_02049b18(c->pl[0]) == 0) {
        c->b7 = 0;
    } else if (rate == 0x7fd8) {
        c->b7 = 0;
    } else if (rate == 0x7fd8 / 8 || rate == 0x7fd8 / 4 || rate == 0x7fd8 / 2 ||
               rate == 0x2ff1 || rate == 0x2ff1 * 2 || rate == 0x4fe7) {
        c->b7 = rate / 4091;
    } else if (rate == 0x553a) {
        c->b7 = 9;
    } else {
        c->b7 = 0;
        CriMsgSendText(data_020ae070);
    }
    for (i = 0; i < c->nch; i++) c->s0c = func_02049e08(c->pl[i], rate);
}

void func_02049024(Ch *c, int v)
{
    short x;
    int i;
    if (c == 0) return;
    if (v >= 0) v = 0;
    x = v;
    if (x <= -999) x = -999;
    c->s18 = x;
    for (i = 0; i < c->nch; i++) func_0204aa80(c->pl[i], c->s18);
}

void func_02049088(Ch *c, int idx, int v)
{
    short t;
    if (c == 0) return;
    if (idx >= c->nch) return;
    if (v >= 15) v = 15;
    t = v;
    if (t <= -15) t = -15;
    c->s1a[idx] = t;
    if (c->b4 >= 1 && c->ncnt == 1 && c->nch == 2) {
        func_0204aab0(c->pl[0], -15);
        func_0204aab0(c->pl[1], 15);
    } else {
        func_0204aab0(c->pl[idx], c->s1a[idx]);
    }
}

void func_02049118(Ch *c, int v)
{
    if (c != 0) c->s0e = v;
}

void func_02049124(Ch *c)
{
    if (c != 0) c->flags |= 4;
}

int func_02049138(Ch *c)
{
    return (c->flags & 8) == 8 ? 1 : 0;
}
