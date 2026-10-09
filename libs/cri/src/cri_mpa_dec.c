/* CRI MPEG audio: decoder object create/destroy/poll, 0x0204bdd4-0x0204c088 */
#pragma dont_inline on
typedef struct MpX {             /* decoder state, X */
    int i0;
    int state;                   /* 0x04 */
    int pad8[0x1d];
    int i7c[2];
    int pad84;
    int i88;                     /* 0x88 */
    int pad8c;
    int i90;                     /* 0x90 */
    int i94;                     /* 0x94 */
    int pad98[0x7];
    int h;                       /* 0xb4 */
    int padb8;
    int nstep;                   /* 0xbc */
} MpX;

typedef struct MpCtx {
    int i0;
    MpX *x;                      /* 0x04 */
    int pad8[0x14];
    int i5c58[2];                /* 0x58, 0x5c */
} MpCtx;

typedef struct MpObj {
    int i0;
    MpCtx *ctx;                  /* 0x04 */
    int pad8[2];
    int h10;                     /* 0x10 */
    int pad14;
    char area[1];                /* 0x18 */
} MpObj;

extern int data_020ae4ac[2];
extern char data_020af4f4[], data_020af51c[], data_020af548[], data_020af578[];
extern void (*data_02216808)(MpObj *);
extern void (*data_0221589c)(MpX *);
extern void (*data_02215890)(int);
extern void (*data_02215894)(int);
extern void (*data_02215898)(int);
extern void (*data_022158ac)(int);

extern void func_02040170(const char *);
extern void func_02042f14(void);
extern void func_02047230(void);
extern void func_0204723c(void);
extern void func_0204d98c(void);
extern int func_0204daa0(int, int, void *, void *, int);
extern void func_0204df7c(int, int, int);
extern void func_0204dc40(int);
extern void func_0204dbb4(int);
extern void func_0204da34(void);
extern int func_0204dc04(int);
extern void func_0204dc14(int);
extern void func_0204df44(int);
extern int func_0204df74(int);
extern int func_0204df6c(int);
extern void func_0204dc0c(int);
extern void func_0204dfa8(int);
extern void func_0204dfb8(int);
extern void func_0204dfc0(int);

void func_0204bf1c(MpObj *);
void func_0204bf3c(MpObj *);
void func_0204bf98(int);
void func_0204bfa4(int);
void func_0204bfb0(MpX *);

void func_0204bdd4(int *a, int *b) {
    if (a != 0) *a = data_020ae4ac[0];
    if (b != 0) *b = data_020ae4ac[1];
}

void func_0204bdfc(MpObj *o, void *buf, int size) {
    MpCtx *ctx;
    MpX *x;
    int h;
    int r;
    if (o == 0 || buf == 0) {
        func_02040170(data_020af4f4);
        return;
    }
    if (size < 0x1800) {
        func_02040170(data_020af51c);
        return;
    }
    func_02042f14();
    func_0204bf1c(o);
    func_0204d98c();
    ctx = o->ctx;
    h = o->h10;
    x = ctx->x;
    func_02047230();
    r = func_0204daa0(h, 1, o->area, buf, size);
    if (r == 0) {
        func_02040170(data_020af548);
        return;
    }
    x->h = r;
    data_02216808 = (void (*)(MpObj *))func_0204bf3c;
    data_0221589c = func_0204bfb0;
    data_02215890 = func_0204bfa4;
    data_02215894 = func_0204bf98;
    data_02215898 = func_0204dc0c;
    data_022158ac = func_0204dfa8;
    func_0204df7c(r, ctx->i5c58[0], ctx->i5c58[1]);
    func_0204723c();
}

void func_0204bf1c(MpObj *o) {
    if (data_02216808 != 0) data_02216808(o);
}

void func_0204bf3c(MpObj *o) {
    MpX *x;
    int h;
    if (o == 0) {
        func_02040170(data_020af578);
        return;
    }
    func_02042f14();
    x = o->ctx->x;
    h = x->h;
    if (h == 0) return;
    func_02047230();
    func_0204dc40(h);
    func_0204dbb4(h);
    x->h = 0;
    func_0204723c();
    func_0204da34();
}

void func_0204bf98(int a) { func_0204dfb8(a); }
void func_0204bfa4(int a) { func_0204dfc0(a); }

void func_0204bfb0(MpX *x) {
    int h = x->h;
    int i;
    if (func_0204dc04(h) == 0) {
        x->i90 = 0;
        x->i88 = 0;
        func_0204dc40(h);
    }
    if (x->state == 1) {
        x->i88 = 0;
        func_0204dc14(h);
        x->state = 2;
        return;
    }
    if (x->state != 2) return;
    for (i = 0; i < x->nstep; i++) func_0204df44(h);
    x->i90 = func_0204df74(h) - x->i88;
    x->i88 = x->i88 + x->i90;
    x->i94 = func_0204df6c(h);
    if (func_0204dc04(h) == 3) {
        func_0204dc40(h);
        x->state = 0;
    }
}
