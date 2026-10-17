typedef struct CriX {
    void *vt;
    int h;
    char sub[0x44];
    int f4c;
    signed char mode;
    int t54, t58, t5c;
} CriX;

extern int data_0209e4ac, data_020b0120, data_020b0154, data_020b0188, data_020b01b8, data_020b01f0;
extern int func_0204359c(int), func_02043798(int), func_02043848(int), StopAdx(int), func_02043478(int), func_020434ac(int);
extern int GetAdxState(int);
extern int func_02043188(int, int);
extern int func_020431fc(int);
extern int func_02043378(int);
extern void SendErrWrapper(void *);
extern int IsPoolObjInit(int), func_0204f98c(int), func_0204f924(int), func_0204f8ec(int);
extern int func_02050698(void *, int);
extern void func_020506e0(int);
extern int CriFaderGetState(int), CriFaderGetValue(int);
extern void CriFaderStart(int, int, int, int, int);
extern void CriFaderAdvance(int);
extern void memset(void *, int, int);
extern int memcmp(void *, void *, int);
void func_02051414(CriX *o, int m);
int func_0205141c(CriX *o);
int func_02051448(CriX *o);
void ResetCriX(CriX *o);

int func_02050f68(CriX *o) { return func_0204359c(o->h); }
int func_02050f78(CriX *o) { return func_02043798(o->h); }
int func_02050f88(CriX *o) { return func_02043848(o->h); }
int func_02050f98(CriX *o) { return StopAdx(o->h); }
int func_02050fa8(CriX *o) { return func_02043478(o->h); }
int func_02050fb8(CriX *o) { return func_020434ac(o->h); }

int func_02050fc8(CriX *o)
{
    switch (GetAdxState(o->h)) {
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

int func_0205102c(CriX *o, int v)
{
    if (v < -960) v = -960;
    return func_02043188(o->h, v);
}

int func_0205104c(CriX *o) { return func_020431fc(o->h); }

void func_0205105c(CriX *o, int a, int v)
{
    if (a != 0) return;
    func_02043188(o->h, v);
}

int func_02051078(CriX *o, int a)
{
    if (a != 0) return 0;
    return func_020431fc(o->h);
}

int func_02051094(CriX *o) { return func_02043378(o->h); }
int func_020510a4(CriX *o) { return o->h; }

void func_020510ac(CriX *o)
{
    o->vt = &data_0209e4ac;
    o->h = 0;
    *(int *)((char *)o + 8) = 0;
}

CriX *CreateCriX(CriX *o, unsigned size)
{
    if (o == 0) {
        SendErrWrapper(&data_020b0120);
        return 0;
    }
    if (size < 0x60) {
        SendErrWrapper(&data_020b0154);
        return 0;
    }
    ResetCriX(o);
    return o;
}

void func_02051110(CriX *o)
{
    if (o == 0) return;
    ResetCriX(o);
}

int func_02051124(CriX *o)
{
    if (o->h != 0) {
        SendErrWrapper(&data_020b0188);
        return 0;
    }
    o->h = func_02050698(o->sub, 0x44);
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
void func_020511f4(int *o, int v) { *o = v; }

void func_020511fc(CriX *o)
{
    int i;
    int busy = 0;
    for (i = 0; i < 4; i++) {
        int s = ((int *)o->vt)[i];
        if (s != 0 && IsPoolObjInit(s) != 0 && func_0204f98c(s) == 2) {
            switch (func_0204f924(s)) {
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

void ResetCriX(CriX *o)
{
    o->vt = 0;
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
