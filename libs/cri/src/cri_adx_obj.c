/* CRI ADX player object create/destroy/accessors, 0x02042760-0x02043220 */
struct ObjVt { void *v0, *v4, *v8; void (*del)(void *); void *v10; void (*f14)(void *); };
typedef struct Obj { struct ObjVt *vt; } Obj;
typedef struct Adx {
    signed char used, state, mode, nch;
    Obj *p4;
    void *stm;
    void *p0c;
    Obj *p10;
    Obj *p14;
    Obj *p18[2];
    int i20, i24, i28, i2c, i30, i34, i38;
    short s3c, s3e, s40, s42[2], s46;
    int pad48, pad4c_, i50;
    int i54, i58, i5c;
    short s60, pad62;
    int i64;
    short s68, s6a;
    signed char b6c, b6d, b6e, b6f, b70, b71, b72, b73;
    void *p74;
    Obj *p78[2];
    Obj *p80[2];
    int i88, i8c, i90;
    void *p94;
    int i98, i9c, ia0, ia4;
    signed char ba8, ba9, baa, bab;
    int iac, ib0, ib4, ib8, ibc, ic0;
} Adx;
typedef struct AdxGlob { int pad[10]; int i28; int i2c; void (*fn30)(void *); } AdxGlob;

extern AdxGlob data_022167d8;
extern Adx gCriDrv[4];
extern int gCriSys, data_02215dc0;
extern int data_020acbf0;
extern char data_020acbf8[], data_020acc20[], data_020acc4c[], data_020acc7c[], data_020acca4[],
            data_020acccc[], data_020accf4[], data_020acd20[], data_020acd54[], data_020acd80[], data_020acdac[];

extern void *memset(void *, int, int);
extern void SendErr(const char *);
extern void func_0203eda4(void), func_0203edb0(void), func_0203edbc(void), func_0203edc0(void);
extern void *CriRingAlloc(int, int, int);
extern void *StmAlloc(void *, int);
extern void *CriStmAlloc(void *, int, void *);
extern void *func_020478f0(void *, int, int);
extern void *func_02051524(void *);
extern void func_02051534(void *, void *);
extern void func_02043580(Adx *);
extern void func_02043244(Adx *, int);
extern void func_020478fc(void *);
extern void CriStmDealloc(void *);
extern void StmSetReadEndCallback(void *, int, int);
extern void StmDealloc(void *);
extern void func_02051528(void *);
extern void func_02051504(void *);
extern void CriStmSetSrc(void *, int);
extern void CriStmStart(void *);
extern void func_02043378(Adx *);
extern void func_020514f8(void *);
extern void func_020426e0(void *, int, int);
extern void StmSetReadEnd(void *, int);
extern void StmSetPos(void *, int);
extern void StmStop(void *);
extern void StmCloseFile(void *);
extern void StmRequestOpenFile(void *, int, int, int, long long);
extern void StmStart(void *);
extern void func_02047914(void *, int), func_02047920(void *, int);
extern void CriStmStop(void *);
extern void func_020514fc(void *);
extern void func_02051538(void *);
extern int func_02041714(void *);
extern int func_020417f8(void *, int);
extern int func_020417b0(void *);
extern void func_02047980(void *, int, int);
extern void func_02047974(void *, int);
extern Adx *AdxAllocInner(int, int, int);
extern void AdxDeallocInner(Adx *);
extern void func_02042b04(Adx *);
extern void AdxStopInner(Adx *);
extern void AdxDeallocAllInner(void);
extern void AdxStart(Adx *, int);
extern void func_02042e94(Adx *);
extern Adx *AdxAllocNch1Inner(int, int);
extern int AdxGetStateInner(Adx *);
extern int func_02043000(Adx *);
extern void func_02043068(Adx *, int, int);
extern void func_020431ac(Adx *, int);
extern int AdxGetVolumeInner(Adx *);

Adx *AdxAlloc(int a, int b, int c)
{
    Adx *r;
    func_0203edbc();
    r = AdxAllocInner(a, b, c);
    func_0203edc0();
    return r;
}

Adx *AdxAllocInner(int nch, int buf, int size)
{
    Adx *o;
    int rem, base;
    int i, k;
    base = (buf + 0x3f) & ~0x3f;
    rem = size - (base - buf);
    if (nch < 0 || buf == 0 || size < 0) {
        SendErr(data_020acbf8);
        return 0;
    }
    for (i = 0; i < 4; i++) {
        if (gCriDrv[i].used == 0) break;
    }
    if (i == 4) {
        SendErr(data_020acc20);
        return 0;
    }
    o = &gCriDrv[i];
    memset(o, 0, 0xc4);
    o->nch = nch;
    k = nch * 0x1460;
    o->i20 = base + k * 2;
    o->i24 = ((rem - k * 2 - 0x124) / 2048) << 11;
    if (o->i24 < 0) {
        SendErr(data_020acc4c);
        return 0;
    }
    o->i28 = 0x24;
    o->iac = o->i20 + (o->i24 + o->i28);
    o->i2c = base;
    o->i30 = 0x800;
    o->i34 = 0x860;
    o->p14 = 0;
    o->p10 = CriRingAlloc(o->i20, o->i24, o->i28);
    if (o->p10 == 0) {
        AdxDeallocInner(o);
        return 0;
    }
    o->stm = StmAlloc(o->p10, 0);
    if (o->stm == 0) {
        AdxDeallocInner(o);
        return 0;
    }
    i = 0;
    if (nch > 0) {
        do {
            o->p18[i] = CriRingAlloc(o->i2c + o->i34 * i * 2, o->i30 * 2, (o->i34 - o->i30) * 2);
            if (o->p18[i] == 0) {
                AdxDeallocInner(o);
                return 0;
            }
            i++;
        } while (i < nch);
    }
    o->p4 = CriStmAlloc(o->p10, nch, o->p18);
    if (o->p4 == 0) {
        AdxDeallocInner(o);
        return 0;
    }
    o->p0c = func_020478f0(o->p18, nch, nch * 0x10c0 + base);
    if (o->p0c == 0) {
        AdxDeallocInner(o);
        return 0;
    }
    o->p94 = func_02051524(o->p10);
    if (o->p94 == 0) {
        AdxDeallocInner(o);
        return 0;
    }
    func_02051534(o->p94, o->stm);
    func_0203eda4();
    o->i38 = data_022167d8.i28;
    o->s3c = o->i24 / 2048;
    o->s3e = 0.85f * (float)o->s3c;
    o->s40 = 0;
    for (i = 0; i < nch; i++) o->s42[i] = -128;
    o->s46 = 0;
    o->b6c = 1;
    o->i54 = 0;
    o->i58 = 0;
    o->i5c = 0;
    o->s60 = 0;
    o->i64 = 0;
    o->s68 = 0;
    o->s6a = 0;
    o->b6d = 1;
    o->b72 = 0;
    o->i88 = 0;
    func_02043580(o);
    func_02043244(o, 1);
    o->used = 1;
    func_0203edb0();
    return o;
}

Adx *AdxAllocNch1(int a, int b)
{
    Adx *r;
    func_0203edbc();
    r = AdxAllocNch1Inner(a, b);
    func_0203edc0();
    return r;
}

Adx *AdxAllocNch1Inner(int a, int b)
{
    Adx *r = AdxAllocInner(1, a, b);
    if (r == 0) return 0;
    return r;
}

void func_02042b04(Adx *o)
{
    if (data_022167d8.fn30) data_022167d8.fn30(o);
}

void AdxDealloc(Adx *o)
{
    func_0203edbc();
    AdxDeallocInner(o);
    func_0203edc0();
}

void AdxDeallocInner(Adx *o)
{
    Obj *p;
    void *q;
    int i;
    if (o == 0) {
        SendErr(data_020acc7c);
        return;
    }
    func_02042b04(o);
    if (o->used == 1) AdxStopInner(o);
    if (o->p0c) {
        q = o->p0c;
        o->p0c = 0;
        func_020478fc(q);
    }
    if (o->p4) {
        q = o->p4;
        o->p4 = 0;
        CriStmDealloc(q);
    }
    q = o->stm;
    if (q) {
        o->stm = 0;
        StmSetReadEndCallback(q, 0, 0);
        StmDealloc(q);
    }
    if (o->p94) {
        q = o->p94;
        o->p94 = 0;
        func_02051528(q);
    }
    func_0203eda4();
    p = o->p10;
    if (p) {
        o->p10 = 0;
        p->vt->del(p);
    }
    for (i = 0; i < o->nch; i++) {
        p = o->p18[i];
        if (p) {
            o->p18[i] = 0;
            p->vt->del(p);
        }
        p = o->p78[i];
        if (p) {
            o->p78[i] = 0;
            p->vt->del(p);
        }
        p = o->p80[i];
        if (p) {
            o->p80[i] = 0;
            p->vt->del(p);
        }
    }
    if (o->p74) {
        q = o->p74;
        o->p74 = 0;
        func_02051504(q);
    }
    memset(o, 0, 0xc4);
    o->used = 0;
    func_0203edb0();
}

void AdxDeallocAll(void)
{
    func_0203edbc();
    AdxDeallocAllInner();
    func_0203edc0();
}

void AdxDeallocAllInner(void)
{
    int i;
    Adx *p = gCriDrv;
    for (i = 0; i < 4; i++, p++) {
        if (p->used == 1) AdxDeallocInner(p);
    }
}

/* Also reached from AdxAdvanceState1 (cri_adx_ctrl2.c, the type-1 step of AdxAdvanceInner) through AdxStartFile while
   ba8 == 1: the cri_adx_ctrl.c set-ups that only store state 1 and ba8 = 1 start there. */
void AdxStart(Adx *o, int v)
{
    int i;
    for (i = 0; i < o->nch; i++) o->p18[i]->vt->f14(o->p18[i]);
    CriStmSetSrc(o->p4, v);
    o->p14 = (Obj *)v;
    CriStmStart(o->p4);
    o->state = 1;
    o->pad4c_ = 0;
    o->b71 = 0;
    o->i8c = 0x7fffffff;
    o->i90 = -1;
    o->i9c = 0;
    o->ia4 = 0;
    o->ia0 = data_02215dc0;
    o->ic0 = 0;
    if (o->mode == 4) func_02043378(o);
    if (o->p74) func_020514f8(o->p74);
}

void AdxStartFile(Adx *o, int a, int b, int c, int d)
{
    func_020426e0(o->stm, o->s3e << 11, o->s3c << 11);
    StmSetReadEnd(o->stm, data_020acbf0);
    StmSetReadEndCallback(o->stm, 0, 0);
    StmSetPos(o->stm, 0);
    StmStop(o->stm);
    StmCloseFile(o->stm);
    StmRequestOpenFile(o->stm, a, b, c, (long long)d << 11);
    StmStart(o->stm);
    AdxStart(o, (int)o->p10);
}

void func_02042e94(Adx *o)
{
    func_0203eda4();
    func_02047914(o->p0c, 0);
    func_02047920(o->p0c, 0);
    CriStmStop(o->p4);
    if (o->mode == 2 && o->p14) {
        Obj *p = o->p14;
        o->p14 = 0;
        p->vt->del(p);
    }
    if (o->p74) func_020514fc(o->p74);
    o->p14 = 0;
    o->state = 0;
    o->ba8 = 0;
    func_0203edb0();
}

void AdxStop(Adx *o)
{
    func_0203edbc();
    AdxStopInner(o);
    func_0203edc0();
}

void AdxStopInner(Adx *o)
{
    if (o == 0) {
        SendErr(data_020acca4);
        return;
    }
    if (o->stm) StmCloseFile(o->stm);
    if (o->mode == 4) {
        func_02051538(o->p94);
        if (o->p14) o->p14->vt->f14(o->p14);
    }
    func_02042e94(o);
}

int AdxGetState(Adx *o)
{
    int r;
    func_0203edbc();
    r = AdxGetStateInner(o);
    func_0203edc0();
    return r;
}

/* -1 and a report when o is 0. */
int AdxGetStateInner(Adx *o)
{
    if (o) return o->state;
    SendErr(data_020acccc);
    return -1;
}

int func_02042fdc(Adx *o)
{
    int r;
    func_0203edbc();
    r = func_02043000(o);
    func_0203edc0();
    return r;
}

int func_02043000(Adx *o)
{
    if (o == 0) {
        SendErr(data_020accf4);
        return -1;
    }
    if (o->state < 2) return 0;
    return func_02041714(o->p4);
}

void func_0204303c(Adx *o, int ch, int vol)
{
    func_0203edbc();
    func_02043068(o, ch, vol);
    func_0203edc0();
}

void func_02043068(Adx *o, int ch, int vol)
{
    int a, r;
    if (o == 0) {
        SendErr(data_020acd20);
        return;
    }
    if (ch != 0 && ch != 1) {
        SendErr(data_020acd54);
        return;
    }
    if (vol != -128) {
        if (vol < -15) vol = -15;
        else if (vol > 15) vol = 15;
    }
    if (o->ba9 == 1) {
        a = func_020417f8(o->p4, ch);
        if (a == -128) a = 0;
    } else {
        a = 0;
    }
    if (gCriSys == 0) {
        if (vol == -128) {
            if (func_02041714(o->p4) == 2) r = (ch == 0) ? -15 : 15;
            else r = 0;
            r += a;
        } else {
            r = vol + a;
        }
    } else {
        r = 0;
    }
    o->s42[ch] = vol;
    if (ch < o->nch) {
        func_02047980(o->p0c, ch, r);
        return;
    }
    SendErr(data_020acd80);
}

void func_02043188(Adx *o, int v)
{
    func_0203edbc();
    func_020431ac(o, v);
    func_0203edc0();
}

void func_020431ac(Adx *o, int v)
{
    int a;
    if (o == 0) {
        SendErr(data_020acdac);
        return;
    }
    o->s40 = v;
    if (o->ba9 == 1) a = func_020417b0(o->p4);
    else a = 0;
    func_02047974(o->p0c, o->s40 + a);
}

int func_020431fc(Adx *o)
{
    int r;
    func_0203edbc();
    r = AdxGetVolumeInner(o);
    func_0203edc0();
    return r;
}
