/* CRI audio: sound-library init counter, noise table generator */
typedef unsigned char u8;
typedef unsigned short u16;
typedef signed short s16;
typedef unsigned int u32;

typedef struct SndGlob {
    int i00;
    int cnt;        /* 0x04: table init done */
    int i08;
    int i0c;
    int users;      /* 0x10: init refcount */
} SndGlob;
extern int data_02218d08;
extern SndGlob data_02218d0c;
extern s16 data_0209dae4[];
extern s16 data_0209dce4, data_0209dee4, data_0209e0e4;
extern signed char data_020ae2c8[];

extern void func_0204d814(void);
extern void func_0204d824(void);
extern void func_0204d834(void);
extern void func_0204d8e4(void);

void func_0204ab10(int v)
{
    data_02218d08 = v;
}

int func_0204ab20(int unused, int *out)
{
    *out = data_02218d08;
    return 30;
}

int func_0204ab38(void)
{
    data_02218d0c.cnt++;
    return 0;
}

int func_0204ab54(signed char *p, int n, s16 *o1, s16 *o2, s16 *o3)
{
    s16 v2;
    s16 v1;
    s16 v3;
    int i;
    if (data_02218d0c.cnt == 0) func_0204ab38();
    *o1 = 0;
    *o2 = 0;
    *o3 = 0;
    if (p == 0 && n <= 0) return 0;
    v1 = data_0209dce4;
    for (i = 0; i < n; i++) v1 = data_0209dae4[(v1 * data_0209dae4[128 + p[i]]) % 1024];
    v2 = data_0209dee4;
    for (i = 0; i < n; i++) v2 = data_0209dae4[(v2 * data_0209dae4[128 + p[i]]) % 1024];
    v3 = data_0209e0e4;
    for (i = 0; i < n; i++) v3 = data_0209dae4[(v3 * data_0209dae4[128 + p[i]]) % 1024];
    *o1 = v1;
    *o2 = v2;
    *o3 = v3;
    return 0;
}

int func_0204acac(s16 *a, s16 *b, s16 *c)
{
    if (data_02218d0c.cnt == 0) func_0204ab38();
    func_0204ab54(data_020ae2c8, 6, a, b, c);
    return 0;
}

void func_0204ad00(int v)
{
    func_0204ab10(v);
}

void func_0204ad0c(int v)
{
    data_02218d0c.i0c = v;
}

void func_0204ad1c(void)
{
    func_0204d814();
}

void func_0204ad28(void)
{
    func_0204d824();
}

void func_0204ad34(void)
{
    if (data_02218d0c.users == 0) func_0204d834();
    data_02218d0c.users++;
}

void func_0204ad64(void)
{
    if (--data_02218d0c.users == 0) func_0204d8e4();
}
