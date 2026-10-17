extern int data_020afa58;
extern int data_020afa5c;
extern int data_0209e4a8;
extern int func_02047680(void);
extern void func_0204657c(void), func_0204658c(void), func_0204659c(void), func_020465ac(void), func_020465bc(void), func_020465cc(void), func_020465dc(void), func_020465ec(void);
extern void func_0204ed44(void), func_0204ed48(void);
extern int func_0204ecf0(int);
extern int AddPoolRef(int, int);
extern int ReleasePoolRef(int);
extern int CreatePoolObj(int, int, int, int);
extern int ReleasePoolObj(int, int);
extern int func_0204efd8(int, int);
extern void func_0204f7c0(int, int);
extern void func_0204f810(int);
extern void func_0204f878(int, int);
extern int func_0204f924(int);
extern void func_0204f994(int, int);
extern void func_0204fa04(int, int);
extern void func_0204fa0c(int, int);
extern void func_0204fa14(int, int);
extern void func_0204fa1c(int, int);
extern void func_0204fa24(int, int);
extern void func_0204fa2c(int);
extern void func_0204fad4(int);
extern int func_0204fb64(int, int);

void func_0204e844(int v);
int func_0204e854(void);
int func_0204e8ac(void);
int func_0204e908(void);
int func_0204e934(int x);
int AddGlobalPoolRef(void);
int ReleaseGlobalPoolRef(void);
int CreateGlobalPoolObj(int a, int b, int c);
int CreateGlobalPoolObjInner(int a, int b, int c);
void ReleaseGlobalPoolObj(int a);
int ReleaseGlobalPoolObjInner(int a);
int func_0204ea2c(int a);
int func_0204ea50(int a);
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
    case 1: func_0204e908(); break;
    case 2: func_020465cc(); break;
    case 3: func_020465dc(); func_020465ec(); break;
    }
    return 0;
}

int func_0204e8ac(void)
{
    switch (func_0204e934(data_020afa58)) {
    case -1: case 0: case 1: break;
    case 2: func_0204657c(); break;
    case 3: func_0204657c(); func_0204658c(); func_0204659c(); func_020465ac(); func_020465cc(); break;
    }
    return 0;
}

int func_0204e908(void)
{
    func_0204657c(); func_0204658c(); func_0204659c(); func_020465ac();
    func_020465bc(); func_020465cc(); func_020465dc(); func_020465ec();
    return 0;
}

int func_0204e934(int x)
{
    if (x != -1) return x;
    if (func_02047680() == 1) return 2;
    return 1;
}

int AddGlobalPoolRef(void) { return AddPoolRef(data_020afa5c, data_0209e4a8); }
int ReleaseGlobalPoolRef(void) { return ReleasePoolRef(data_020afa5c); }
int CreateGlobalPoolObj(int a, int b, int c)
{
    int r;
    func_0204ed44();
    r = CreateGlobalPoolObjInner(a, b, c);
    func_0204ed48();
    return r;
}
int CreateGlobalPoolObjInner(int a, int b, int c) { return CreatePoolObj(data_020afa5c, a, b, c); }
void ReleaseGlobalPoolObj(int a) { func_0204ed44(); ReleaseGlobalPoolObjInner(a); func_0204ed48(); }
int ReleaseGlobalPoolObjInner(int a) { return ReleasePoolObj(data_020afa5c, a); }
int func_0204ea2c(int a)
{
    int r;
    func_0204ed44();
    r = func_0204ea50(a);
    func_0204ed48();
    return r;
}
int func_0204ea50(int a) { return func_0204efd8(data_020afa5c, a); }
void func_0204ea6c(int a, int b)
{
    func_0204ed44();
    func_0204ea90(a, b);
    func_0204ed48();
}
void func_0204ea90(int a, int b) { func_0204f7c0(a, b); }
void func_0204ea9c(int a)
{
    func_0204ed44();
    func_0204eab8(a);
    func_0204ed48();
}
void func_0204eab8(int a) { func_0204f810(a); }
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
int func_0204eb18(int a) { return func_0204f924(a); }
void func_0204eb24(int a, int b)
{
    func_0204ed44();
    func_0204eb48(a, b);
    func_0204ed48();
}
void func_0204eb48(int a, int b) { func_0204f994(a, b); }
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
int func_0204ecc0(int a, int b) { return func_0204fb64(a, b); }
int func_0204eccc(int a)
{
    int r;
    func_0204ed44();
    r = func_0204ecf0(a);
    func_0204ed48();
    return r;
}
