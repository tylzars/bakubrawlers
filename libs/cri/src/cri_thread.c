/* CRI middleware, driver thread control, 0x0204713c-0x020477d8 */

/* init configuration passed to func_02047414/func_02047484 (0x24 bytes) */
typedef struct CriCfg {
    int prio0, prio1, prio2;    /* 0x00, 0x04, 0x08 */
    void *stk0;                 /* 0x0c */
    void *stk1;                 /* 0x10 */
    void *stk2;                 /* 0x14 */
    int size0, size1, size2;    /* 0x18, 0x1c, 0x20 */
} CriCfg;

/* thread control block, data_02217a9c */
typedef struct CriThreadCtl {
    int run2, done2;            /* 0x00, 0x04 */
    int run0, f0c;              /* 0x08, 0x0c */
    int run1, done1;            /* 0x10, 0x14 */
    int f18, f1c;
    int mode;                   /* 0x20 */
    char pad24[0x264 - 0x24];
    int arg;                    /* 0x264 */
    int cnt0, cnt1, cnt2;       /* 0x268, 0x26c, 0x270 */
} CriThreadCtl;

typedef struct CriLockCtl { int pad0, pad4; volatile int cnt; int active; } CriLockCtl;

extern CriThreadCtl data_02217a9c;
extern CriCfg data_02217a78;
extern CriLockCtl data_02217a40;
extern int data_02217a5c[5];
extern int data_02217a70;
extern int data_02217a3c;
extern int data_0209e388, data_0209e38c;
extern int OSi_ThreadInfo[2];
extern char data_02217a3c_dummy;
extern char data_020adcd8[], data_020add50[];
extern int data_020add08, data_020add0c, data_020add14, data_020add90, data_020add1c;
extern char data_02217ac0[], data_02217b80[], data_02217c40[];

extern void func_02040170(const char *, ...);
extern void func_020479fc(void);
extern void func_0204701c(int, int);
extern void func_0204712c(int, int);
extern void func_0204678c(int *, void *, int);
extern void func_02049344(void);
extern void func_020479f0(void);
extern void func_02046924(int *);
extern void func_02007230(void);
extern void func_02047884(void);
extern void OS_WakeupThreadDirect(void *);
extern void OS_SleepThread(int);
extern void func_0204e854(void);
extern void func_02046154(void);
extern void func_02046164(void);
extern void OS_DisableIrqMask(int);
extern void OS_EnableIrqMask(int);
extern void func_0204658c(void);
extern void func_0204659c(void);
extern void func_020477e8(void);
extern void func_020465bc(void);
extern int func_020465dc(void);
extern void func_020478a8(void);
extern void func_020464b8(void);
extern void func_0204e844(int);
extern void func_02046194(char *);
extern void func_020477a8(void);
extern int func_02047798(void);
extern void func_0204668c(void);
extern void func_020464e0(void *, int);
extern void func_020464f0(void *, int);
extern int func_020492f0(int *, int *);
extern void func_020477fc(void);
extern void OS_CreateThread(void *, void *, int, int, int, int);
extern void func_02047248(void);
extern void func_02047294(void);
extern void func_020472bc(void);
extern void func_02047330(void);
extern void func_02047388(void);
extern void func_02047484(CriCfg *);
extern int func_02047680(void);
extern void func_020477c0(void);
extern void OS_SetThreadPriority(void *, unsigned);
extern int func_020477d8(void);
extern void func_020466bc(void);

void func_0204713c(void)
{
    int v = data_0209e388;
    int f = data_02217a3c;
    if (f == 1) {
        func_02040170((const char *)data_020adcd8, v);
        return;
    }
    func_020479fc();
    func_0204701c((int)func_0204712c, 0);
    func_0204678c(&data_020add08, (void *)func_02049344, 0);
    func_0204678c(&data_020add0c, (void *)func_020479f0, 0);
    func_02046924(&data_020add14);
    data_02217a3c = 1;
}

void func_020471d0(void)
{
    unsigned m = data_02217a9c.mode;
    if (m <= 1) {
        func_02007230();
        return;
    }
    if (m != 2) return;
    func_02047884();
    data_02217a5c[4] = 0;
    OS_WakeupThreadDirect(data_02217c40);
    OS_SleepThread(0);
}

void func_02047224(void) { func_0204e854(); }
void func_02047230(void) { func_02046154(); }
void func_0204723c(void) { func_02046164(); }

void func_02047248(void)
{
    if (data_02217a40.cnt == 0) {
        int ie = *(volatile int *)0x4000210;
        OS_DisableIrqMask(1);
        data_02217a70 = ie;
    }
    data_02217a40.cnt++;
}

void func_02047294(void)
{
    int c = data_02217a40.cnt;
    c--;
    data_02217a40.cnt = c;
    if (c != 0) return;
    OS_EnableIrqMask(1);
}

void func_020472bc(void)
{
    if (data_02217a9c.run0 == 1) {
        do {
            if (data_02217a9c.done1 == 1) {
                OS_WakeupThreadDirect((void *)data_02217a9c.arg);
            } else {
                OS_WakeupThreadDirect(data_02217b80);
            }
            func_0204658c();
            func_0204659c();
            data_02217a9c.cnt0++;
            OS_SleepThread(0);
        } while (data_02217a9c.run0 == 1);
    }
    func_020477e8();
}

void func_02047330(void)
{
    if (data_02217a9c.run1 == 1) {
        do {
            OS_WakeupThreadDirect((void *)data_02217a9c.arg);
            func_020465bc();
            data_02217a9c.cnt1++;
            OS_SleepThread(0);
        } while (data_02217a9c.run1 == 1);
    }
    data_02217a9c.done1 = 1;
}

void func_02047388(void)
{
    if (data_02217a9c.run2 == 1) {
        do {
            if (func_020465dc() == 0) {
                func_02046154();
                if (data_02217a5c[4] == 0) {
                    data_02217a5c[4] = 1;
                    func_020478a8();
                }
                func_02046164();
                OS_SleepThread(0);
            }
            data_02217a9c.cnt2++;
        } while (data_02217a9c.run2 == 1);
    }
    data_02217a9c.done2 = 1;
}

void func_02047408(void) { func_020464b8(); }

int func_02047414(unsigned mode, CriCfg *cfg)
{
    data_02217a9c.mode = mode;
    __clear(data_02217a5c, sizeof(data_02217a5c));
    if ((unsigned)data_02217a9c.mode <= 1) {
        func_0204e844(1);
        return 1;
    }
    if (data_02217a9c.mode == 2) func_0204e844(2);
    func_02047484(cfg);
    return 1;
}

void func_02047484(CriCfg *cfg)
{
    int tmp;
    (void)*(volatile int *)&data_0209e38c;
    if (cfg == 0) {
        func_02046194((char *)data_020add50);
        return;
    }
    func_020477a8();
    if (func_02047798() != 1) return;
    data_02217a9c.arg = OSi_ThreadInfo[1];
    if (data_02217a9c.mode != 2) data_02217a9c.mode = 2;
    func_0204668c();
    func_020464e0((void *)func_02047248, 0);
    func_020464f0((void *)func_02047294, 0);
    if (func_020492f0(&data_020add90, &tmp) == 0) {
        data_020add1c = tmp;
    } else {
        data_020add1c = 15;
    }
    data_02217a78 = *cfg;
    data_02217a9c.f18 = 0;
    data_02217a9c.f1c = 0;
    data_02217a9c.run0 = 1;
    func_020477fc();
    {
        int stk = (int)cfg->stk0;
        int sz = cfg->size0;
        stk = (stk + 3) & ~3;
        OS_CreateThread(data_02217ac0, (void *)func_020472bc, 0, stk + sz, sz, data_02217a78.prio0);
    }
    data_02217a9c.run1 = 1;
    data_02217a9c.done1 = 0;
    OS_CreateThread(data_02217b80, (void *)func_02047330, 0,
                  (((int)cfg->stk1 + 3) & ~3) + cfg->size1, cfg->size1, data_02217a78.prio1);
    if (data_02217a9c.mode == 2) {
        data_02217a9c.run2 = 1;
        data_02217a9c.done2 = 0;
        OS_CreateThread(data_02217c40, (void *)func_02047388, 0,
                      (((int)cfg->stk2 + 3) & ~3) + cfg->size2, cfg->size2, data_02217a78.prio2);
    } else {
        data_02217a9c.run2 = 0;
        data_02217a9c.done2 = 1;
    }
}

int func_02047680(void)
{
    int r;
    if (func_02047798() != 0) {
        r = 1;
    } else {
        r = 0;
    }
    return r;
}

int func_02047698(void)
{
    if (func_02047680() == 0) return 0;
    func_020477c0();
    if (func_02047798() == 0) {
        data_02217a9c.run2 = 0;
        OS_SetThreadPriority(data_02217c40, 0);
        while (data_02217a9c.done2 == 0) OS_WakeupThreadDirect(data_02217c40);
        data_02217a9c.run1 = 0;
        OS_SetThreadPriority(data_02217b80, 0);
        while (data_02217a9c.done1 == 0) OS_WakeupThreadDirect(data_02217b80);
        data_02217a9c.run0 = 0;
        OS_SetThreadPriority(data_02217ac0, 0);
        if (func_020477d8() == 0) {
            do {
                OS_WakeupThreadDirect(data_02217ac0);
            } while (func_020477d8() == 0);
        }
        func_020466bc();
        data_02217a9c.mode = 1;
        func_0204e844(1);
    }
    return 1;
}

int func_02047798(void)
{
    return data_02217a40.active;
}

void func_020477a8(void)
{
    data_02217a40.active++;
}

void func_020477c0(void)
{
    data_02217a40.active--;
}
