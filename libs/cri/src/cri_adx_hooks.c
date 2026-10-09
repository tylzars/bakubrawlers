/* CRI middleware callback thunks, 0x0203de40-0x0203dee8 (precede cri_adx_player.c). */
typedef struct CriHooks {
    void (*f0)(int);
    void (*f4)(int, int);
    void (*f8)(int, int);
    void (*fc)(int);
} CriHooks;
extern CriHooks data_02215890;

typedef struct CriObj {
    int pad[0x2d];
    int h;      /* 0xb4 */
    int v;      /* 0xb8 */
    int v96;    /* 0xbc */
} CriObj;

void func_0203de40(CriObj *o, int a)
{
    if (o->h != 0) {
        data_02215890.f8(o->h, a);
    }
}

void func_0203de64(CriObj *o, int v)
{
    if (o->h != 0) {
        data_02215890.f4(o->h, v);
    }
    o->v = v;
    o->v96 = v / 96;
}

void func_0203deac(int a)
{
    data_02215890.fc(a);
}

void func_0203dec4(CriObj *o)
{
    if (o->h != 0) {
        data_02215890.f0(o->h);
    }
}
