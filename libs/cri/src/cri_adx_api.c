extern int data_020afa58;
extern int data_020afa5c;
extern int data_0209e4a8;
extern int func_02047680(void);
extern void CriFuncTableRun0(void), CriFuncTableRun1(void), CriFuncTableRun2(void), CriFuncTableRun3(void), CriFuncTableRun4(void), CriFuncTableRun5(void), CriFuncTableRun6(void), CriFuncTableRun7(void);
extern void func_0204ed44(void), func_0204ed48(void);
extern int func_0204ecf0(int);
extern int PoolAddRef(int, int);
extern int PoolReleaseRef(int);
extern int PoolAllocObj(int, int, int, int);
extern int PoolDeallocObj(int, int);
extern int PoolComputeWorkSize(int, int);
extern void PoolObjStart(int, int);
extern void PoolObjStop(int);
extern void func_0204f878(int, int);
extern int PoolObjGetState(int);
extern void PoolObjSetVolume(int, int);
extern void func_0204fa04(int, int);
extern void func_0204fa0c(int, int);
extern void func_0204fa14(int, int);
extern void func_0204fa1c(int, int);
extern void func_0204fa24(int, int);
extern void func_0204fa2c(int);
extern void func_0204fad4(int);
extern int PoolObjGetAdx(int, int);

void func_0204e844(int v);
int func_0204e854(void);
int func_0204e8ac(void);
int CriFuncTableRunAll(void);
int func_0204e934(int x);
int GlobalPoolAddRef(void);
int GlobalPoolReleaseRef(void);
int GlobalPoolAllocObj(int a, int b, int c);
int GlobalPoolAllocObjInner(int a, int b, int c);
void GlobalPoolDeallocObj(int a);
int GlobalPoolDeallocObjInner(int a);
int GlobalPoolComputeWorkSize(int a);
int GlobalPoolComputeWorkSizeInner(int a);
void func_0204ea6c(int a, int b);
void func_0204ea90(int a, int b);
void func_0204ea9c(int a);
void func_0204eab8(int a);
void func_0204eac4(int a, int b);
void func_0204eae8(int a, int b);
int func_0204eaf4(int a);
int func_0204eb18(int a);
void func_0204eb24(int a, int b);
void func_0204eb48(int a, int b);
void func_0204eb54(int a, int b);
void func_0204eb78(int a, int b);
void func_0204eb84(int a, int b);
void func_0204eba8(int a, int b);
void func_0204ebb4(int a, int b);
void func_0204ebd8(int a, int b);
void func_0204ebe4(int a, int b);
void func_0204ec08(int a, int b);
void func_0204ec14(int a, int b);
void func_0204ec38(int a, int b);
void func_0204ec44(int a);
void func_0204ec60(int a);
void func_0204ec6c(int a);
void func_0204ec88(int a);
int func_0204ec94(int a, int b);
int func_0204ecc0(int a, int b);
int func_0204eccc(int a);

void func_0204e844(int v) { data_020afa58 = v; }

int func_0204e854(void)
{
    switch (func_0204e934(data_020afa58)) {
    case -1: break;
    case 1: CriFuncTableRunAll(); break;
    case 2: CriFuncTableRun5(); break;
    case 3: CriFuncTableRun6(); CriFuncTableRun7(); break;
    }
    return 0;
}

int func_0204e8ac(void)
{
    switch (func_0204e934(data_020afa58)) {
    case -1: case 0: case 1: break;
    case 2: CriFuncTableRun0(); break;
    case 3: CriFuncTableRun0(); CriFuncTableRun1(); CriFuncTableRun2(); CriFuncTableRun3(); CriFuncTableRun5(); break;
    }
    return 0;
}

int CriFuncTableRunAll(void)
{
    CriFuncTableRun0(); CriFuncTableRun1(); CriFuncTableRun2(); CriFuncTableRun3();
    CriFuncTableRun4(); CriFuncTableRun5(); CriFuncTableRun6(); CriFuncTableRun7();
    return 0;
}

int func_0204e934(int x)
{
    if (x != -1) return x;
    if (func_02047680() == 1) return 2;
    return 1;
}

int GlobalPoolAddRef(void) { return PoolAddRef(data_020afa5c, data_0209e4a8); }   /* PoolAddRef reads no 2nd argument */
int GlobalPoolReleaseRef(void) { return PoolReleaseRef(data_020afa5c); }   /* PoolReleaseRef is void: no result */
int GlobalPoolAllocObj(int a, int b, int c)
{
    int r;
    func_0204ed44();
    r = GlobalPoolAllocObjInner(a, b, c);
    func_0204ed48();
    return r;
}
int GlobalPoolAllocObjInner(int a, int b, int c) { return PoolAllocObj(data_020afa5c, a, b, c); }
void GlobalPoolDeallocObj(int a) { func_0204ed44(); GlobalPoolDeallocObjInner(a); func_0204ed48(); }
int GlobalPoolDeallocObjInner(int a) { return PoolDeallocObj(data_020afa5c, a); }
int GlobalPoolComputeWorkSize(int a)
{
    int r;
    func_0204ed44();
    r = GlobalPoolComputeWorkSizeInner(a);
    func_0204ed48();
    return r;
}
int GlobalPoolComputeWorkSizeInner(int a) { return PoolComputeWorkSize(data_020afa5c, a); }
void func_0204ea6c(int a, int b)
{
    func_0204ed44();
    func_0204ea90(a, b);
    func_0204ed48();
}
void func_0204ea90(int a, int b) { PoolObjStart(a, b); }
void func_0204ea9c(int a)
{
    func_0204ed44();
    func_0204eab8(a);
    func_0204ed48();
}
void func_0204eab8(int a) { PoolObjStop(a); }
void func_0204eac4(int a, int b)
{
    func_0204ed44();
    func_0204eae8(a, b);
    func_0204ed48();
}
void func_0204eae8(int a, int b) { func_0204f878(a, b); }
int func_0204eaf4(int a)
{
    int r;
    func_0204ed44();
    r = func_0204eb18(a);
    func_0204ed48();
    return r;
}
int func_0204eb18(int a) { return PoolObjGetState(a); }
void func_0204eb24(int a, int b)
{
    func_0204ed44();
    func_0204eb48(a, b);
    func_0204ed48();
}
void func_0204eb48(int a, int b) { PoolObjSetVolume(a, b); }
void func_0204eb54(int a, int b)
{
    func_0204ed44();
    func_0204eb78(a, b);
    func_0204ed48();
}
void func_0204eb78(int a, int b) { func_0204fa04(a, b); }
void func_0204eb84(int a, int b)
{
    func_0204ed44();
    func_0204eba8(a, b);
    func_0204ed48();
}
void func_0204eba8(int a, int b) { func_0204fa0c(a, b); }
void func_0204ebb4(int a, int b)
{
    func_0204ed44();
    func_0204ebd8(a, b);
    func_0204ed48();
}
void func_0204ebd8(int a, int b) { func_0204fa14(a, b); }
void func_0204ebe4(int a, int b)
{
    func_0204ed44();
    func_0204ec08(a, b);
    func_0204ed48();
}
void func_0204ec08(int a, int b) { func_0204fa1c(a, b); }
void func_0204ec14(int a, int b)
{
    func_0204ed44();
    func_0204ec38(a, b);
    func_0204ed48();
}
void func_0204ec38(int a, int b) { func_0204fa24(a, b); }
void func_0204ec44(int a)
{
    func_0204ed44();
    func_0204ec60(a);
    func_0204ed48();
}
void func_0204ec60(int a) { func_0204fa2c(a); }
void func_0204ec6c(int a)
{
    func_0204ed44();
    func_0204ec88(a);
    func_0204ed48();
}
void func_0204ec88(int a) { func_0204fad4(a); }
int func_0204ec94(int a, int b)
{
    int r;
    func_0204ed44();
    r = func_0204ecc0(a, b);
    func_0204ed48();
    return r;
}
int func_0204ecc0(int a, int b) { return PoolObjGetAdx(a, b); }
int func_0204eccc(int a)
{
    int r;
    func_0204ed44();
    r = func_0204ecf0(a);
    func_0204ed48();
    return r;
}
