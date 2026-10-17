/* CRI middleware, 0x02044614-0x02044a20: 4-slot object pool (0x3c bytes), plus AdxAdvance / AdxAdvanceInner: those take a gCriDrv Adx entry (declared CriSlot *; byte +1, CriSlotHdr.type, is the Adx state) */
typedef struct CriSlot {
    int used;           /* 0x00 */
    int idx;            /* 0x04 */
    int i8;             /* 0x08 */
    int state;          /* 0x0c */
    int result;         /* 0x10 */
    int mode;           /* 0x14 */
    int a18, a1c, a20, a24;
    union { short s[4]; signed char b[8]; } u28;   /* 0x28 */
    short s30, s32, s34, s36, s38;
    short pad3a;
} CriSlot;

typedef struct CriSlotHdr { char c0; signed char type; } CriSlotHdr;

extern CriSlot gSlots[4];      /* 0x02216850 */
extern int gFlag;              /* 0x0221684c */
extern char data_020ad000[];

extern void func_0203edbc(void);
extern void func_0203edc0(void);
extern int func_0203edc4(int, int, short *, short *);
extern void SendErr(const char *);
extern void AdxAdvanceState3(CriSlot *);
extern void AdxAdvanceState1(CriSlot *);
extern void AdxAdvanceState2(CriSlot *);
extern void AdxAdvanceState4(CriSlot *);
extern void func_02044548(CriSlot *);
extern void func_020445a8(CriSlot *);
extern void func_0204454c(CriSlot *);
extern void *memset(void *, int, int);
extern int func_0203f714(int, int, int, short *, int, int, int, int, int);
extern int func_0204006c(int, int, int, short *, int, short *, int, int, short *, int, int);

void AdxAdvanceInner(CriSlot *p);

void AdxAdvance(CriSlot *p)
{
    func_0203edbc();
    AdxAdvanceInner(p);
    func_0203edc0();
}

void AdxAdvanceInner(CriSlot *p)
{
    if (p == 0) {
        SendErr(data_020ad000);
        return;
    }
    {
        int t = ((CriSlotHdr *)p)->type;
        if (t == 3) AdxAdvanceState3(p);
        else if (t == 1) AdxAdvanceState1(p);
        else if (t == 2) AdxAdvanceState2(p);
        else if (t == 4) AdxAdvanceState4(p);
        else if (t == 5) func_02044548(p);
    }
    func_020445a8(p);
    func_0204454c(p);
}

void CriSlotClearAll(void)
{
    __clear(gSlots, sizeof(gSlots));
}

CriSlot *CriSlotAlloc(void)
{
    int i;
    CriSlot *p;
    for (i = 0; i < 4; i++) {
        if (gSlots[i].used == 0) break;
    }
    if (i == 4) return 0;
    p = &gSlots[i];
    memset(p, 0, 0x3c);
    p->used = 1;
    p->idx = i;
    p->i8 = 0;
    p->state = 0;
    func_0203edc4(0x1f4, 0xac44, &p->s30, &p->s32);
    p->u28.b[0] = 0; p->u28.b[1] = 0; p->u28.b[2] = 0; p->u28.b[3] = 0;
    p->u28.b[4] = 0; p->u28.b[5] = 0; p->u28.b[6] = 0; p->u28.b[7] = 0;
    return p;
}

void func_02044790(CriSlot *p, int a, int b)
{
    func_0203edc4(b, a, &p->s30, &p->s32);
}

void func_020447ac(CriSlot *p, short *a, short *b)
{
    p->u28.s[0] = a[0];
    p->u28.s[1] = b[0];
    p->u28.s[2] = a[1];
    p->u28.s[3] = b[1];
}

void func_020447d0(CriSlot *p, short *a, short *b)
{
    a[0] = p->u28.s[0];
    b[0] = p->u28.s[1];
    a[1] = p->u28.s[2];
    b[1] = p->u28.s[3];
}

void func_020447f4(CriSlot *p, int a, int b, int c)
{
    p->s34 = a;
    p->s36 = b;
    p->s38 = c;
}

void func_02044804(CriSlot *p, short *a, short *b, short *c)
{
    *a = p->s34;
    *b = p->s36;
    *c = p->s38;
}

void CriSlotDealloc(CriSlot *p)
{
    if (p == 0) return;
    p->used = 0;
    memset(p, 0, 0x3c);
}

int CriSlotGetState(CriSlot *p)
{
    return p->state;
}

int CriSlotSetupMode1(CriSlot *p, int a, int b, int c, int d)
{
    if (p->state != 0) return 0;
    p->mode = 1;
    p->a18 = a;
    p->a1c = b;
    p->a20 = c;
    p->a24 = d;
    return 1;
}

int CriSlotSetupMode2(CriSlot *p, int a, int b, int c, int d)
{
    if (p->state != 0) return 0;
    p->mode = 2;
    p->a18 = a;
    p->a1c = b;
    p->a20 = c;
    p->a24 = d;
    return 1;
}

void CriSlotStart(CriSlot *p)
{
    if (p->state != 0) return;
    p->result = 0;
    p->state = 1;
}

void CriSlotStop(CriSlot *p)
{
    p->state = 0;
    p->u28.b[0] = 0; p->u28.b[1] = 0; p->u28.b[2] = 0; p->u28.b[3] = 0;
    p->u28.b[4] = 0; p->u28.b[5] = 0; p->u28.b[6] = 0; p->u28.b[7] = 0;
}

void func_020448fc(CriSlot *p)
{
    if (p->state == 3) p->state = 0;
}

int CriSlotGetResult(CriSlot *p)
{
    return p->result;
}

void func_02044918(void)
{
    gFlag = 1;
}

void CriSlotRun(CriSlot *p)
{
    if (p->state == 1) p->state = 2;
    if (p->state != 2) return;
    if (p->mode == 1) {
        p->result = func_0203f714(p->a18, p->a1c, p->a20, &p->u28.s[0], p->s30, p->s32, (int)&p->s34, p->s36, p->s38);
    } else {
        int r = func_0204006c(p->a18, p->a1c, p->a20, &p->u28.s[0], p->a24, &p->u28.s[2], p->s30, p->s32, &p->s34, p->s36, p->s38);
        p->result = r;
        if (r % 2 == 1) func_02044918();
    }
    p->state = 3;
}
