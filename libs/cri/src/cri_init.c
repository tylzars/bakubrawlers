/* CRI middleware init/exit, 0x0204046c-0x020407bc */
typedef struct CriSys {          /* 0x02215dbc */
    int f0, f4, h8, padc;
    int refcnt;                  /* 0x10 */
    int h14;
} CriSys;
typedef struct CriHook {         /* 0x022160e4 */
    void (*post)(int);           /* 0x00 */
    int preArg;                  /* 0x04 */
    void (*pre)(int);            /* 0x08 */
    int postArg;                 /* 0x0c */
} CriHook;
typedef struct CriDrvObj { signed char used; char pad[3]; int rest[0x30]; } CriDrvObj;  /* 0xc4 */

extern CriSys gCriSys;          /* 0x02215dbc */
extern CriDrvObj gCriDrv[4];    /* 0x02215dd4 */
extern CriHook gCriHook;        /* 0x022160e4 */
typedef struct CriStmSys {       /* 0x022160f4 */
    void (*post)(int);           /* 0x00 */
    int preArg;                  /* 0x04 */
    void (*pre)(int);            /* 0x08 */
    int refcnt;                  /* 0x0c */
    int postArg;                 /* 0x10 */
} CriStmSys;
typedef struct CriStmSlot { signed char used; char pad[3]; int rest[0x2c]; } CriStmSlot;  /* 0xb4 */
extern CriStmSys gCriStmSys;    /* 0x022160f4 */
extern CriStmSlot gCriStm[4];   /* 0x02216108 */
extern char *data_020aca44;
extern char data_0209e324[];
extern char data_020aca48[], data_020aca58[], data_020aca68[], data_020aca7c[], data_020acac8[];

extern int func_0204327c(void);
extern int func_0204035c(void);
extern int *func_02051540(void);
extern void func_0203ed54(void);
extern void func_0203eda4(void);
extern void func_0203edb0(void);
extern void func_0203edbc(void);
extern void func_0203edc0(void);
extern void func_0203ed7c(void);
extern void func_02045f78(void);
extern void func_02044a70(void);
extern void func_020456e8(void);
extern void func_02040110(void);
extern void StmPoolAddRef(void);
extern void func_02040704(void);
extern void func_02051508(void);
extern void func_020478cc(void);
extern void func_0205151c(void);
extern void func_0204668c(void);
extern void func_020478e4(void *, int);
extern void func_0205152c(void *, int);
extern void func_0204043c(void);
extern void func_0204044c(void);
extern void func_0204045c(void);
extern int func_0204046c(void);
extern int func_0204047c(void);
extern int CriFuncTableSet(int, int, void *, int, void *);
extern int CriFuncTableAdd(int, void *, int, void *);
extern void func_0204324c(int);
extern void CriSendText(void *);
extern void AdxDeallocAll(void);
extern void func_020478d8(void);
extern void func_0205150c(void);
extern void StmPoolReleaseRef(void);
extern void func_02051520(void);
extern void CriFuncTableRemove(int, int);
extern void func_020466bc(void);
extern void func_02040764(void);
extern void func_02040140(void);
extern void func_02045760(void);
extern void func_02044ae0(void);
extern void func_02045fe8(void);
extern void AdxDealloc(void *);
extern void func_0205153c(void);
extern void func_0203e07c(void);
extern void func_020406c4(void);

int func_0204046c(void) { func_0204327c(); return 0; }

int func_0204047c(void) { func_0204035c(); return 0; }

/* Counted: the set-up runs on the first call only; the two lines before the refcnt test run on every call. */
void CriInit(void)
{
    data_020aca44 = data_0209e324;
    func_02051540();
    if (gCriSys.refcnt == 0) {
        func_0203ed54();
        func_0203eda4();
        func_02045f78();
        func_02044a70();
        func_020456e8();
        func_02040110();
        StmPoolAddRef();
        func_02040704();
        func_02051508();
        func_020478cc();
        func_0205151c();
        func_0204668c();
        func_020478e4((void *)func_0204043c, 0);
        func_0205152c((void *)func_0204044c, 0);
        __clear(gCriDrv, sizeof(gCriDrv));
        CriFuncTableSet(2, 1, (void *)func_0204046c, 0, data_020aca48);
        gCriSys.h14 = CriFuncTableAdd(4, (void *)func_0204047c, 0, data_020aca58);
        gCriSys.h8 = CriFuncTableAdd(5, (void *)func_0204045c, 0, data_020aca68);
        gCriSys.f4 = 0;
        gCriSys.f0 = 0;
        func_0204324c(0x3c);
        func_0203edb0();
    }
    gCriSys.refcnt++;
}

/* Counted: the clean-up runs when the last reference goes. */
void CriExit(void)
{
    int i;
    CriDrvObj *p;
    if (gCriSys.refcnt <= 0) {
        CriSendText(data_020aca7c);
        return;
    }
    if (--gCriSys.refcnt != 0) return;
    AdxDeallocAll();
    func_020478d8();
    func_0205150c();
    StmPoolReleaseRef();
    func_02051520();
    func_0203eda4();
    CriFuncTableRemove(2, 1);
    CriFuncTableRemove(4, gCriSys.h14);
    CriFuncTableRemove(5, gCriSys.h8);
    func_020466bc();
    func_02040764();
    func_02040140();
    func_02045760();
    func_02044ae0();
    func_02045fe8();
    func_0203edb0();
    func_0203ed7c();
    p = gCriDrv;
    for (i = 0; i < 4; i++) {
        if (p->used != 0) {
            CriSendText(data_020acac8);
            AdxDealloc(p);
        }
        p++;
    }
}

void func_020406b0(void)
{
    func_0203edbc();
    func_020406c4();
    func_0203edc0();
}

void func_020406c4(void)
{
    if (gCriHook.pre) gCriHook.pre(gCriHook.preArg);
    func_0205153c();
    if (gCriHook.post) gCriHook.post(gCriHook.postArg);
}

void func_02040704(void)
{
    if (gCriStmSys.refcnt == 0) {
        func_0203e07c();
        __clear(gCriStm, sizeof(gCriStm));
    }
    gCriStmSys.refcnt++;
}

void func_02040764(void)
{
    volatile CriStmSys *s = &gCriStmSys;
    s->refcnt--;
    if (s->refcnt != 0) return;
    __clear(gCriStm, sizeof(gCriStm));
}
