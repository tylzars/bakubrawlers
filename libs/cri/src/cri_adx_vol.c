/* Two objects live in this unit.
   CriMemBlk (0xc bytes, as in cri_mem_blk.c): its vtable data_0209e4ac lists func_02050ebc..func_02050f58
   (cri_mem_blk.c) and func_02050f68..func_020510a4 below; func_020510ac sets it up. Those below work on the Adx
   object at +4 (the AdxAlloc / AdxAllocNch1 result stored by func_02050ebc): most hand it and their other
   arguments, unchanged, to a cri_adx_ctrl.c / cri_adx_obj.c function; func_02050fc8 maps AdxGetState's result,
   func_0205102c raises the value to at least -960 first, func_0205105c / func_02051078 act only when their first
   argument is 0, and func_020510a4 returns the object.
   CriX (0x60 bytes, CriXCreate checks the size): the pool fader object, created by PoolAddRef in a Pool's buf
   (cri_adx_pool.c); func_020511f4 stores the owning Pool at +0. */
typedef struct CriMemBlk {
    void *vt;                         /* 0x00, data_0209e4ac */
    void *adx;                        /* 0x04 */
    int src;                          /* 0x08 */
} CriMemBlk;

typedef struct CriX {
    void *pool;                       /* 0x00, the owning Pool: its slot[4] are read in func_020511fc */
    int h;
    char sub[0x44];
    int f4c;
    signed char mode;
    int t54, t58, t5c;
} CriX;

extern int data_0209e4ac, data_020b0120, data_020b0154, data_020b0188, data_020b01b8, data_020b01f0;
extern void func_0204359c(void *, int, int), func_02043798(void *, int, int), func_02043848(void *, int, int);
extern void AdxStop(void *), func_02043478(void *, int);
extern int func_020434ac(void *);
extern int AdxGetState(void *);
extern void func_02043188(void *, int);
extern int func_020431fc(void *);
extern void func_02043378(void *, int);
extern void SendErrWrapper(void *);
extern int PoolObjIsInit(int), func_0204f98c(int), PoolObjGetState(int), func_0204f8ec(int);
extern int CriFaderCreate(void *, int);
extern void func_020506e0(int);
extern int CriFaderGetState(int), CriFaderGetValue(int);
extern void CriFaderStart(int, int, int, int, int);
extern void CriFaderAdvance(int);
extern void memset(void *, int, int);
extern int memcmp(void *, void *, int);
void func_02051414(CriX *o, int m);
int func_0205141c(CriX *o);
int func_02051448(CriX *o);
void CriXReset(CriX *o);

void func_02050f68(CriMemBlk *o, int a, int b) { func_0204359c(o->adx, a, b); }
void func_02050f78(CriMemBlk *o, int a, int b) { func_02043798(o->adx, a, b); }
void func_02050f88(CriMemBlk *o, int a, int b) { func_02043848(o->adx, a, b); }
void func_02050f98(CriMemBlk *o) { AdxStop(o->adx); }
void func_02050fa8(CriMemBlk *o, int v) { func_02043478(o->adx, v); }
int func_02050fb8(CriMemBlk *o) { return func_020434ac(o->adx); }

int func_02050fc8(CriMemBlk *o)
{
    switch (AdxGetState(o->adx)) {
    case 0: return 0;
    case 1:
    case 2: return 1;
    case 3:
    case 4: return 2;
    case 5: return 3;
    case 6: return 4;
    default: return 4;
    }
}

void func_0205102c(CriMemBlk *o, int v)
{
    if (v < -960) v = -960;
    func_02043188(o->adx, v);
}

int func_0205104c(CriMemBlk *o) { return func_020431fc(o->adx); }

void func_0205105c(CriMemBlk *o, int a, int v)
{
    if (a != 0) return;
    func_02043188(o->adx, v);
}

int func_02051078(CriMemBlk *o, int a)
{
    if (a != 0) return 0;
    return func_020431fc(o->adx);
}

void func_02051094(CriMemBlk *o, int v) { func_02043378(o->adx, v); }
void *func_020510a4(CriMemBlk *o) { return o->adx; }

void func_020510ac(CriMemBlk *o)
{
    o->vt = &data_0209e4ac;
    o->adx = 0;
    o->src = 0;
}

CriX *CriXCreate(CriX *o, unsigned size)
{
    if (o == 0) {
        SendErrWrapper(&data_020b0120);
        return 0;
    }
    if (size < 0x60) {
        SendErrWrapper(&data_020b0154);
        return 0;
    }
    CriXReset(o);
    return o;
}

void func_02051110(CriX *o)
{
    if (o == 0) return;
    CriXReset(o);
}

int func_02051124(CriX *o)
{
    if (o->h != 0) {
        SendErrWrapper(&data_020b0188);
        return 0;
    }
    o->h = CriFaderCreate(o->sub, 0x44);
    if (o->h == 0) {
        SendErrWrapper(&data_020b01b8);
        return 0;
    }
    CriFaderStart(o->h, 0, 0, 0, 0);
    return 1;
}

void func_020511a4(CriX *o)
{
    if (o->h == 0) return;
    func_020506e0(o->h);
    o->h = 0;
}

void func_020511c8(CriX *o) { o->f4c = 0; }
int func_020511d4(CriX *o) { return o->f4c; }
int func_020511dc(CriX *o) { return o->mode; }
int func_020511e4(CriX *o) { return CriFaderGetValue(o->h); }
void func_020511f4(CriX *o, void *pool) { o->pool = pool; }

void func_020511fc(CriX *o)
{
    int i;
    int busy = 0;
    for (i = 0; i < 4; i++) {
        int s = ((int *)o->pool)[i];
        if (s != 0 && PoolObjIsInit(s) != 0 && func_0204f98c(s) == 2) {
            switch (PoolObjGetState(s)) {
            case 0: case 3: case 4: break;
            case 1:
            case 2:
                if (func_0204f8ec(s) == 0) busy = 1;
                break;
            }
        }
    }
    switch (o->mode) {
    case 0:
        if (busy == 1) {
            CriFaderStart(o->h, -o->t5c, o->t54, 0, 0);
            func_02051414(o, 1);
        }
        break;
    case 1:
        if (busy == 0) {
            CriFaderStart(o->h, 0, func_0205141c(o), 0, 0);
            func_02051414(o, 3);
        } else if (CriFaderGetState(o->h) == 0) {
            func_02051414(o, 2);
        }
        break;
    case 2:
        if (busy == 0) {
            CriFaderStart(o->h, 0, o->t58, 0, 0);
            func_02051414(o, 3);
        }
        break;
    case 3:
        if (busy == 1) {
            CriFaderStart(o->h, -o->t5c, func_02051448(o), 0, 0);
            func_02051414(o, 1);
        } else if (CriFaderGetState(o->h) == 0) {
            func_02051414(o, 0);
        }
        break;
    }
    CriFaderAdvance(o->h);
}

void CriXReset(CriX *o)
{
    o->pool = 0;
    o->h = 0;
    memset(o->sub, 0, 0x44);
    o->f4c = 0;
    o->mode = 0;
    o->t54 = 30;
    o->t58 = 60;
    o->t5c = 120;
}

void func_02051414(CriX *o, int m) { o->mode = m; }

int func_0205141c(CriX *o)
{
    int a = o->t54;
    unsigned r = CriFaderGetValue(o->h);
    return a - a * (int)r / -o->t5c;
}

int func_02051448(CriX *o)
{
    unsigned r = CriFaderGetValue(o->h);
    return o->t54 - o->t58 * (int)r / -o->t5c;
}

int FindFourBytes(char *s, int n, short *out)
{
    int lim = n - 3;
    int found = 0x7fffffff;
    int i = 0;
    if (lim > 0) {
        do {
            if (memcmp(s + i, &data_020b01f0, 4) == 0) {
                if (i >= 0x7fffffff) i = 0x7fffffff;
                found = i;
                break;
            }
            i++;
        } while (i < lim);
    }
    if (found != 0x7fffffff) {
        *out = found;
        return 0;
    }
    *out = 0;
    return -1;
}

void func_020514f8(void) {}
void func_020514fc(void) {}
void func_02051500(void) {}
void func_02051504(void) {}
void func_02051508(void) {}
void func_0205150c(void) {}
void func_02051510(void) {}
int func_02051514(void) { return 0; }
void func_0205151c(void) {}
void func_02051520(void) {}
void func_02051524(void) {}
