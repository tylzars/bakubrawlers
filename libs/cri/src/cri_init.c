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

extern CriSys g_criSys;          /* 0x02215dbc */
extern CriDrvObj g_criDrv[4];    /* 0x02215dd4 */
extern CriHook g_criHook;        /* 0x022160e4 */
typedef struct CriStmSys {       /* 0x022160f4 */
    void (*post)(int);           /* 0x00 */
    int preArg;                  /* 0x04 */
    void (*pre)(int);            /* 0x08 */
    int refcnt;                  /* 0x0c */
    int postArg;                 /* 0x10 */
} CriStmSys;
typedef struct CriStmSlot { signed char used; char pad[3]; int rest[0x2c]; } CriStmSlot;  /* 0xb4 */
extern CriStmSys g_criStmSys;    /* 0x022160f4 */
extern CriStmSlot g_criStm[4];   /* 0x02216108 */
extern char *data_020aca44;
extern char data_0209e324[];
extern char data_020aca48[], data_020aca58[], data_020aca68[], data_020aca7c[], data_020acac8[];

extern int func_0204327c(void);
extern int func_0204035c(void);
extern void func_02051540(void);
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
extern void func_02041878(void);
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
extern int func_020463d8(int, int, void *, int, void *);
extern int func_0204625c(int, void *, int, void *);
extern void func_0204324c(int);
extern void func_02046200(void *);
extern void func_02042ccc(void);
extern void func_020478d8(void);
extern void func_0205150c(void);
extern void func_020418d0(void);
extern void func_02051520(void);
extern void func_02046328(int, int);
extern void func_020466bc(void);
extern void func_02040764(void);
extern void func_02040140(void);
extern void func_02045760(void);
extern void func_02044ae0(void);
extern void func_02045fe8(void);
extern void func_02042b24(void *);
extern void func_0205153c(void);
extern void func_0203e07c(void);
extern void func_020406c4(void);
extern void *memset(void *, int, unsigned);

int func_0204046c(void) { func_0204327c(); return 0; }

int func_0204047c(void) { func_0204035c(); return 0; }

void func_0204048c(void)
{
    data_020aca44 = data_0209e324;
    func_02051540();
    if (g_criSys.refcnt == 0) {
        func_0203ed54();
        func_0203eda4();
        func_02045f78();
        func_02044a70();
        func_020456e8();
        func_02040110();
        func_02041878();
        func_02040704();
        func_02051508();
        func_020478cc();
        func_0205151c();
        func_0204668c();
        func_020478e4((void *)func_0204043c, 0);
        func_0205152c((void *)func_0204044c, 0);
        memset(g_criDrv, 0, sizeof(g_criDrv));
        func_020463d8(2, 1, (void *)func_0204046c, 0, data_020aca48);
        g_criSys.h14 = func_0204625c(4, (void *)func_0204047c, 0, data_020aca58);
        g_criSys.h8 = func_0204625c(5, (void *)func_0204045c, 0, data_020aca68);
        g_criSys.f4 = 0;
        g_criSys.f0 = 0;
        func_0204324c(0x3c);
        func_0203edb0();
    }
    g_criSys.refcnt++;
}

void func_020405d4(void)
{
    int i;
    CriDrvObj *p;
    if (g_criSys.refcnt <= 0) {
        func_02046200(data_020aca7c);
        return;
    }
    if (--g_criSys.refcnt != 0) return;
    func_02042ccc();
    func_020478d8();
    func_0205150c();
    func_020418d0();
    func_02051520();
    func_0203eda4();
    func_02046328(2, 1);
    func_02046328(4, g_criSys.h14);
    func_02046328(5, g_criSys.h8);
    func_020466bc();
    func_02040764();
    func_02040140();
    func_02045760();
    func_02044ae0();
    func_02045fe8();
    func_0203edb0();
    func_0203ed7c();
    p = g_criDrv;
    for (i = 0; i < 4; i++) {
        if (p->used != 0) {
            func_02046200(data_020acac8);
            func_02042b24(p);
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
    if (g_criHook.pre) g_criHook.pre(g_criHook.preArg);
    func_0205153c();
    if (g_criHook.post) g_criHook.post(g_criHook.postArg);
}

void func_02040704(void)
{
    if (g_criStmSys.refcnt == 0) {
        func_0203e07c();
        memset(g_criStm, 0, sizeof(g_criStm));
    }
    g_criStmSys.refcnt++;
}

void func_02040764(void)
{
    g_criStmSys.refcnt--;
    if (g_criStmSys.refcnt != 0) return;
    memset(g_criStm, 0, sizeof(g_criStm));
}
