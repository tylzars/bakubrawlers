/* CRI stream player slots, 0x020407bc-0x02041744 */
typedef struct CriPair { void *p; int n; } CriPair;
struct CriStm;
struct CriVObj;
typedef struct CriVt {
    int pad[6];
    void (*get)(struct CriVObj *, int, int, CriPair *);      /* 0x18 */
    void (*put)(struct CriVObj *, int, CriPair *);           /* 0x1c */
    void (*commit)(struct CriVObj *, int, CriPair *);        /* 0x20 */
    int (*size)(struct CriVObj *, int);                      /* 0x24 */
} CriVt;
typedef struct CriVObj { const CriVt *vt; } CriVObj;

typedef struct CriPlayer { char pad[0x98]; short s98; short s9a; } CriPlayer;

typedef struct CriStm {
    signed char used;          /* 0x00 */
    signed char state;         /* 0x01 */
    signed char nch;           /* 0x02 */
    signed char flag;          /* 0x03 */
    CriPlayer *obj;            /* 0x04 */
    CriVObj *src;              /* 0x08 */
    CriVObj *ch[2];            /* 0x0c */
    CriPair in;                /* 0x14 */
    CriPair chb[2];            /* 0x1c */
    int i2c, i30, i34, i38, i3c, i40, i44;
    void (*cb48)(int);         /* 0x48 */
    int arg4c;
    int i50, i54;
    void (*cb58)(int, int, void *, int);   /* 0x58 */
    int arg5c;
    char buf60[0x40];
    int ia0, ia4, ia8, iac;
    int pad_b0;
} CriStm;                      /* 0xb4 */

typedef struct CriStmSys {
    void (*post)(int);
    int preArg;
    void (*pre)(int);
    int refcnt;
    int postArg;
} CriStmSys;
extern CriStmSys g_criStmSys;  /* 0x022160f4 */
extern CriStm g_criStm[4];     /* 0x02216108 */

extern char str_cri_a[], str_cri_b[];

extern void *func_02012484(void *, int, int);
extern void *func_02012464(void *, void *, int);
extern unsigned func_0201f970(unsigned, int);
extern void func_020401cc(const char *, const char *);
extern int func_02045524(void *);
extern int func_0204559c(void *);
extern int func_02045614(void *);
extern void func_0204604c(CriPair *, int, CriPair *, CriPair *);
extern void func_0203eda4(void);
extern void func_0203edb0(void);
extern int func_0203e06c(void);
extern void *func_0203e150(int, int, int, int);
extern void func_0203e21c(void *);
extern void func_0203e67c(void *, void *, void *);
extern int func_0203e634(void *, void *);
extern void func_0203e590(void *);
extern int func_0203e688(void *);
extern int func_0203e690(void *);
extern int func_0203e698(void *);
extern int func_0203e6a0(void *);
extern int func_0203e6c4(void *);
extern int func_0203e6cc(void *);
extern int func_0203e6d4(void *);
extern int func_0203e6dc(void *);
extern int func_0203e874(void *);
extern void func_0203e87c(void *, void *, int);
extern void func_0203e8e8(void *);
extern void func_0203e8fc(void *);
extern void func_0203e918(void *);
extern int func_0203e944(void *);
extern int func_0203e94c(void *);
extern void func_0203eccc(void *);
extern int func_0203f69c(void *, int, short *);
extern void func_0203de40(void *, int);
extern void func_0203de64(void *, int);
extern void func_0203dec4(void *);
extern int func_02041744(CriStm *);
extern int func_02041734(CriStm *);
extern int func_02041870(CriStm *);
extern int func_02040c40(struct CriStm *, int *, int *, int *);
extern void func_020409bc(CriStm *);
extern void func_02040d10(CriStm *);
extern void func_02041110(CriStm *);
extern void func_020412bc(CriStm *);
extern void func_02041320(CriStm *);
extern void func_0204138c(CriStm *);
extern void func_020413f8(CriStm *);
extern void func_02041528(CriStm *);
extern void func_020407bc(CriStm *);
extern char data_020acaf4[], data_020acb14[], data_020acb38[], data_020acb5c[];

void func_020407bc(CriStm *s)
{
    s->ia0 = 0;
    s->i2c = 0;
    s->i30 = 0;
    s->i34 = 0;
    s->i38 = 0x7fffffff;
    s->i3c = -1;
    s->i40 = 0;
    s->i44 = 0;
    s->flag = 0;
    s->ia8 = 0;
    s->iac = 0;
}

CriStm *func_020407f8(int owner, int nch, CriVObj **srcs)
{
    CriStm *s;
    int i, a, b, c;
    CriStm *p = g_criStm;
    CriVObj *first = srcs[0];
    for (i = 0; i < 4; i++) {
        if (p->used == 0) break;
        p++;
    }
    if (i == 4) return 0;
    s = &g_criStm[i];
    a = func_02045524(first);
    b = func_0204559c(first) / 2;
    c = b + func_02045614(first) / 2;
    s->obj = func_0203e150(nch, a, b, c);
    if (s->obj == 0) return 0;
    func_0203e67c(s->obj, (void *)func_02040c40, s);
    s->src = (CriVObj *)owner;
    s->nch = nch;
    for (i = 0; i < nch; i++) s->ch[i] = srcs[i];
    s->state = 0;
    func_020407bc(s);
    s->cb48 = 0;
    s->arg4c = 0;
    s->cb58 = 0;
    s->arg5c = 0;
    s->used = 1;
    return s;
}

void func_02040908(CriStm *s)
{
    if (s == 0) return;
    if (s->obj != 0) {
        void *o = s->obj;
        s->obj = 0;
        func_0203e21c(o);
    }
    func_0203eda4();
    func_02012484(s, 0, 0xb4);
    func_0203edb0();
}

int func_02040948(CriStm *s) { return s->state; }

void func_02040950(CriStm *s, int v) { s->src = (CriVObj *)v; func_0203de40(s->obj, v); }

void func_02040964(CriStm *s, int v) { s->i38 = v; func_0203de64(s->obj, v); }

void func_02040978(CriStm *s) { func_0203dec4(s->obj); }

void func_02040988(CriStm *s)
{
    func_020407bc(s);
    s->state = 1;
}

void func_020409a0(CriStm *s)
{
    func_0203e8fc(s->obj);
    s->state = 0;
}

void func_020409bc(CriStm *s)
{
    CriPlayer *obj;
    CriVObj *src;
    CriPair a, b;
    int i, n, st;
    src = s->src;
    obj = s->obj;
    src->vt->get(src, 1, 0xc800, &a);
    i = 0;
    if (a.n > 0) {
        signed char *q = (signed char *)a.p;
        do {
            if (*q != 0) break;
            i++;
            q++;
        } while (i < a.n);
    }
    func_0204604c(&a, i, &b, &a);
    src->vt->commit(src, 0, &b);
    if (0x10 > a.n) {
        src->vt->put(src, 1, &a);
        return;
    }
    n = func_0203e634(obj, a.p);
    if (n == 0 || n > a.n) {
        src->vt->put(src, 1, &a);
        return;
    }
    if (n < 0) {
        if (obj->s9a != 0) {
            func_0203e590(obj);
            n = 0;
        } else {
            src->vt->put(src, 1, &a);
            if (func_0203e06c() == 0) func_020401cc(data_020acaf4, data_020acb14);
            s->state = 4;
            return;
        }
    }
    s->ia0 = n;
    if (s->i50 != 0) {
        int r10 = func_0203e690(obj);
        int r9 = func_0203e6a0(obj);
        int r8 = func_0203e698(obj);
        int r0 = func_0203e6dc(obj);
        ((void (*)(int, int, int, int, int))s->i50)(s->i54, r10, r9, r8, r0);
    }
    if (func_0203e690(obj) == 4) s->flag = 1;
    if (func_0203e690(obj) == 2) {
        int l = a.n;
        if (l >= 0x40) l = 0x40;
        func_02012464(s->buf60, a.p, l);
    }
    st = func_0203e690(obj);
    if (st == 10 || st == 11 || st == 12 || st == 20 || st == 15) {
        src->vt->put(src, 1, &a);
    } else {
        func_0204604c(&a, n, &a, &b);
        src->vt->commit(src, 0, &a);
        src->vt->put(src, 1, &b);
    }
    s->state = 2;
}

int func_02040c40(CriStm *s, int *o0, int *o1, int *o2)
{
    CriVObj *first = s->ch[0];
    int i = 0;
    int h;
    if (func_0203e6a0(s->obj) > 0) {
        CriPair *q = s->chb;
        do {
            s->ch[i]->vt->get(s->ch[i], 0, 0x4000, q);
            i++;
            q++;
        } while (i < func_0203e6a0(s->obj));
    }
    *o0 = ((int)s->chb[0].p - func_02045524(first)) / 2;
    h = s->chb[0].n / 2;
    if (s->i38 <= h) h = s->i38;
    *o1 = h;
    if (s->i3c >= 0) *o2 = s->i3c - s->i40;
    else *o2 = 0x1fffffff;
    return func_0203e688(s->obj);
}

void func_02040d10(CriStm *s)
{
    CriPlayer *obj;
    CriVObj *src;
    int flag, i, n, pos;
    short hdr;
    CriPair t;
    CriPair u;
    short sw;
    obj = s->obj;
    src = s->src;
    flag = 0;
    if (s->i3c >= 0 && s->i40 >= s->i3c) {
        if (s->cb48 != 0) s->cb48(s->arg4c);
    }
    if (s->flag == 1) {
        if (src->vt->size(src, 1) == 0) {
            s->state = 3;
            return;
        }
    }
    src->vt->get(src, 1, 0x7fffffff, &s->in);
    if (func_0203e690(obj) == 0 && s->in.n >= 4) {
        short v = *(short *)s->in.p;
        sw = ((v >> 8) & 0xff) | ((v << 8) & 0xff00);
        if ((unsigned short)sw == 0x8001) {
            s->state = 3;
            if (func_0203f69c(s->in.p, s->in.n, &hdr) == 0) {
                if (hdr > s->in.n) {
                    src->vt->put(src, 1, &s->in);
                    return;
                }
                func_0204604c(&s->in, hdr, &s->in, &t);
                src->vt->commit(src, 0, &s->in);
                src->vt->put(src, 1, &t);
            }
            if (s->ia4 == 0) return;
            do {
                src->vt->get(src, 1, 0x7fffffff, &s->in);
                n = s->in.n;
                if (n == 0) return;
                i = 0;
                if (n > 0) {
                    signed char *q = (signed char *)s->in.p;
                    do {
                        if (*q != 0) break;
                        i++;
                        q++;
                    } while (i < n);
                }
                func_0204604c(&s->in, i, &s->in, &t);
                src->vt->commit(src, 0, &s->in);
                src->vt->put(src, 1, &t);
            } while (i >= n);
            return;
        }
    }
    pos = func_02041744(s);
    if (s->i34 >= pos) {
        if (func_0203e690(obj) == 1 && func_02041870(s) == 1) {
        } else flag = 1;
    } else {
        if (func_0203e690(obj) == 10) {
            if (s->i34 + 0x240 >= pos) flag = 1;
        }
    }
    if (flag != 0) {
        s->state = 3;
        src->vt->put(src, 1, &s->in);
        return;
    }
    n = func_02041734(s);
    i = s->ch[0]->vt->size(s->ch[0], 0);
    if (n > i / 2) {
        src->vt->put(src, 1, &s->in);
        return;
    }
    if (func_02041870(s) != 1 && func_0203e690(obj) == 1) {
        if (func_0203e6c4(obj) == 16) {
            int nch = func_0203e6a0(s->obj);
            int base = s->i34;
            int half = (int)func_0201f970(s->in.n, nch) / 2;
            if (base + half > pos) {
                func_0204604c(&s->in, nch * (pos - base) * 2, &s->in, &u);
                src->vt->put(src, 1, &u);
            }
        } else {
            func_020401cc(data_020acb38, data_020acb5c);
        }
    }
    if (func_0203e690(obj) == 10) src->vt->put(src, 1, &s->in);
    func_0203e87c(obj, s->in.p, s->in.n);
    func_0203e8e8(obj);
}

void func_02041110(CriStm *s)
{
    CriPlayer *obj = s->obj;
    CriVObj *src = s->src;
    int a, b, c, i;
    CriPair x, y;
    a = func_0203e6dc(obj);
    b = func_0203e944(obj);
    c = func_0203e94c(obj);
    if (func_0203e690(obj) == 1 && func_02041870(s) == 1) {
    } else {
        int t = a - s->i34;
        c = c < t ? c : t;
    }
    func_0204604c(&s->in, b, &x, &y);
    src->vt->commit(src, 0, &x);
    src->vt->put(src, 1, &y);
    i = 0;
    if (func_0203e6a0(s->obj) > 0) {
        CriPair *q = s->chb;
        do {
            func_0204604c(q, c * 2, &x, &y);
            if (s->cb58 != 0) s->cb58(s->arg5c, i, x.p, x.n);
            s->ch[i]->vt->commit(s->ch[i], 1, &x);
            s->ch[i]->vt->put(s->ch[i], 0, &y);
            i++;
            q++;
        } while (i < func_0203e6a0(s->obj));
    }
    s->i2c += c;
    s->i30 += b;
    s->i34 += c;
    s->i40 += c;
    s->i44 += b;
    func_0203e918(obj);
}

void func_020412bc(CriStm *s)
{
    CriPlayer *obj = s->obj;
    int a = func_0203e6dc(obj);
    int b = func_0203e944(obj);
    int c = func_0203e94c(obj);
    int t = a - s->i34;
    if (c >= t) c = t;
    s->i2c += c;
    s->i30 += b;
    s->i34 += c;
}

void func_02041320(CriStm *s)
{
    CriPlayer *obj = s->obj;
    int st;
    if (func_0203e874(obj) == 0) func_02040d10(s);
    func_0203eccc(obj);
    if (func_0203e874(obj) == 3) func_02041110(s);
    st = obj->s98;
    if (st == 10 || st == 20 || st == 11 || st == 12 || st == 15) func_020412bc(s);
}

void func_0204138c(CriStm *s)
{
    if (s->ia8 > 0) {
        func_0203eda4();
        func_020413f8(s);
        func_0203edb0();
    }
    if (s->state == 2) func_02041320(s);
    else if (s->state == 1) func_020409bc(s);
    if (s->iac > 0) {
        func_0203eda4();
        func_02041528(s);
        func_0203edb0();
    }
}

void func_020413f8(CriStm *s)
{
    CriPair p;
    int bytes = s->ia8 * 2;
    int i = 0;
    int half, len;
    if (s->nch > 0) {
        do {
            s->ch[i]->vt->get(s->ch[i], 0, 0x7fffffff, &p);
            bytes = bytes < p.n ? bytes : p.n;
            s->ch[i]->vt->put(s->ch[i], 0, &p);
            i++;
        } while (i < s->nch);
    }
    half = bytes / 2;
    len = half * 2;
    if (len <= 0) return;
    i = 0;
    if (s->nch > 0) {
        do {
            s->ch[i]->vt->get(s->ch[i], 0, len, &p);
            func_02012484(p.p, 0, len);
            s->ch[i]->vt->commit(s->ch[i], 1, &p);
            i++;
        } while (i < s->nch);
    }
    s->ia8 -= half;
}

void func_02041528(CriStm *s)
{
    CriPair p;
    int bytes = s->iac * 2;
    int i = 0;
    int half, len;
    if (s->nch > 0) {
        do {
            s->ch[i]->vt->get(s->ch[i], 1, 0x7fffffff, &p);
            bytes = bytes < p.n ? bytes : p.n;
            s->ch[i]->vt->put(s->ch[i], 1, &p);
            i++;
        } while (i < s->nch);
    }
    half = bytes / 2;
    len = half * 2;
    if (len <= 0) return;
    i = 0;
    if (s->nch > 0) {
        do {
            s->ch[i]->vt->get(s->ch[i], 1, len, &p);
            s->ch[i]->vt->commit(s->ch[i], 0, &p);
            i++;
        } while (i < s->nch);
    }
    s->iac -= half;
}

void func_02041644(void)
{
    int i;
    CriStm *p;
    if (g_criStmSys.pre != 0) g_criStmSys.pre(g_criStmSys.preArg);
    p = g_criStm;
    for (i = 0; i < 4; i++) {
        if (p->used == 1) func_0204138c(p);
        p++;
    }
    if (g_criStmSys.post != 0) g_criStmSys.post(g_criStmSys.postArg);
}

int func_020416b0(CriStm *s) { return s->i30; }
int func_020416b8(CriStm *s) { return s->i2c; }
void func_020416c0(CriStm *s, int v) { s->i34 = v; }
void func_020416c8(CriStm *s, int v) { s->ia4 = v; }
void func_020416d0(CriStm *s, void (*cb)(int), int arg) { s->cb48 = cb; s->arg4c = arg; }
void func_020416dc(CriStm *s, int v) { s->i3c = v; }
void func_020416e4(CriStm *s, int v) { s->i40 = v; }
void func_020416ec(CriStm *s, int v) { s->i44 = v; }
int func_020416f4(CriStm *s) { return func_0203e690(s->obj); }
int func_02041704(CriStm *s) { return func_0203e698(s->obj); }
int func_02041714(CriStm *s) { return func_0203e6a0(s->obj); }
int func_02041724(CriStm *s) { return func_0203e6cc(s->obj); }
int func_02041734(CriStm *s) { return func_0203e6d4(s->obj); }
