/* CRI MPEG audio: decoder object create/destroy/poll and the synthesis dot products (hand-written asm), 0x0204bdd4-0x0204c40c */
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

extern void SendErr(const char *);
extern void StopAdx(void);
extern void func_02047230(void);
extern void func_0204723c(void);
extern void func_0204d98c(void);
extern int CreateCriPlay(int, int, void *, void *, int);
extern void func_0204df7c(int, int, int);
extern void StopCriPlay(int);
extern void ReleaseCriPlay(int);
extern void func_0204da34(void);
extern int GetCriPlayState(int);
extern void StartCriPlay(int);
extern void func_0204df44(int);
extern int func_0204df74(int);
extern int func_0204df6c(int);
extern void SetCriPlaySrc(int);
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
        SendErr(data_020af4f4);
        return;
    }
    if (size < 0x1800) {
        SendErr(data_020af51c);
        return;
    }
    StopAdx();
    func_0204bf1c(o);
    func_0204d98c();
    ctx = o->ctx;
    h = o->h10;
    x = ctx->x;
    func_02047230();
    r = CreateCriPlay(h, 1, o->area, buf, size);
    if (r == 0) {
        SendErr(data_020af548);
        return;
    }
    x->h = r;
    data_02216808 = (void (*)(MpObj *))func_0204bf3c;
    data_0221589c = func_0204bfb0;
    data_02215890 = func_0204bfa4;
    data_02215894 = func_0204bf98;
    data_02215898 = SetCriPlaySrc;
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
        SendErr(data_020af578);
        return;
    }
    StopAdx();
    x = o->ctx->x;
    h = x->h;
    if (h == 0) return;
    func_02047230();
    StopCriPlay(h);
    ReleaseCriPlay(h);
    x->h = 0;
    func_0204723c();
    func_0204da34();
}

void func_0204bf98(int a) { func_0204dfb8(a); }
void func_0204bfa4(int a) { func_0204dfc0(a); }

void func_0204bfb0(MpX *x) {
    int h = x->h;
    int i;
    if (GetCriPlayState(h) == 0) {
        x->i90 = 0;
        x->i88 = 0;
        StopCriPlay(h);
    }
    if (x->state == 1) {
        x->i88 = 0;
        StartCriPlay(h);
        x->state = 2;
        return;
    }
    if (x->state != 2) return;
    for (i = 0; i < x->nstep; i++) func_0204df44(h);
    x->i90 = func_0204df74(h) - x->i88;
    x->i88 = x->i88 + x->i90;
    x->i94 = func_0204df6c(h);
    if (GetCriPlayState(h) == 3) {
        StopCriPlay(h);
        x->state = 0;
    }
}

// r0 -> one fixed vector of 32 signed halfwords (read as words, two per word, never advanced),
// r1 -> rows of 32 halfwords (64 bytes apart), r2 -> 64-entry output halfword array. Keeps r1,
// r2+0x20 and r2+0x60 on the stack. Each output is the dot product of the r0 vector with one row
// (signed 16x16 products, bottom*bottom then top*top per word). Pass 1 (rows 0-15) fills
// out[0..15] and, negated, out[32] down to out[17]; out[16] is set to 0; pass 2 (rows 33-47) fills
// out[33..47] and out[63] down to out[49] with the same value; out[48] gets the negated plain sum
// of the 32 halfwords at r0.
asm void func_0204c088(const short *coef, const short *samples, short *out) {
    stmfd sp!, {r4, r5, r6, r7, r8, r9, r10, r11, lr}
    sub sp, sp, #12
    add r3, r2, #32
    add r4, r2, #96
    str r3, [sp, #4]
    str r1, [sp, #8]
    add r11, r2, #64
    add r3, r2, #66
    str r4, [sp, #0]
    add ip, r2, #126
    mov lr, #16
    // pass 1, 16 iterations: dot product of the 32 halfwords at r0 with the 32 halfwords at r1
    // (accumulated pairwise, low*low + high*high), shifted right 12 bits; stored forward at r2
    // and, negated, backward from r2+0x40; r1 advances one 64-byte row per iteration
L_0204c0b4:
    ldr r4, [r0, #0]
    ldr r5, [r1, #0]
    ldmib r0, {r6, r8}
    smulbb r10, r4, r5
    smlatt r10, r4, r5, r10
    ldmib r1, {r7, r9}
    smlabb r10, r6, r7, r10
    smlatt r10, r6, r7, r10
    smlabb r10, r8, r9, r10
    smlatt r10, r8, r9, r10
    ldr r4, [r0, #12]
    ldr r5, [r1, #12]
    ldr r6, [r0, #16]
    smlabb r10, r4, r5, r10
    smlatt r10, r4, r5, r10
    ldr r7, [r1, #16]
    ldr r8, [r0, #20]
    smlabb r10, r6, r7, r10
    smlatt r10, r6, r7, r10
    ldr r9, [r1, #20]
    ldr r4, [r0, #24]
    smlabb r10, r8, r9, r10
    smlatt r10, r8, r9, r10
    ldr r5, [r1, #24]
    ldr r6, [r0, #28]
    smlabb r10, r4, r5, r10
    smlatt r10, r4, r5, r10
    ldr r7, [r1, #28]
    ldr r8, [r0, #32]
    smlabb r10, r6, r7, r10
    smlatt r10, r6, r7, r10
    ldr r9, [r1, #32]
    ldr r4, [r0, #36]
    smlabb r10, r8, r9, r10
    smlatt r10, r8, r9, r10
    ldr r5, [r1, #36]
    ldr r6, [r0, #40]
    smlabb r10, r4, r5, r10
    smlatt r10, r4, r5, r10
    ldr r7, [r1, #40]
    ldr r8, [r0, #44]
    smlabb r10, r6, r7, r10
    smlatt r10, r6, r7, r10
    ldr r9, [r1, #44]
    ldr r4, [r0, #48]
    smlabb r10, r8, r9, r10
    smlatt r10, r8, r9, r10
    ldr r5, [r1, #48]
    ldr r6, [r0, #52]
    smlabb r10, r4, r5, r10
    smlatt r10, r4, r5, r10
    ldr r7, [r1, #52]
    ldr r8, [r0, #56]
    smlabb r10, r6, r7, r10
    ldr r9, [r1, #56]
    smlatt r10, r6, r7, r10
    smlabb r10, r8, r9, r10
    ldr r5, [r1, #60]
    ldr r4, [r0, #60]
    smlatt r10, r8, r9, r10
    smlabb r10, r4, r5, r10
    smlatt r4, r4, r5, r10
    mov r4, r4, asr #12
    strh r4, [r2], #2
    rsb r4, r4, #0
    strh r4, [r11], #-2
    add r1, r1, #64
    subs lr, lr, #1
    bne L_0204c0b4
    // store 0 at the saved r2+0x20 (out[16])
    ldr r1, [sp, #4]
    mov r2, #0
    strh r2, [r1, #0]
    ldr r1, [sp, #8]
    mov lr, #15
    add r1, r1, #2112
    // pass 2, 15 iterations: the same dot product against the rows from the original r1+0x840
    // (row 33) on, result reduced by a 4-bit left and 16-bit arithmetic right shift; the same
    // value is stored forward from r2+0x42 and backward from r2+0x7e
L_0204c1e0:
    ldr r4, [r0, #0]
    ldr r5, [r1, #0]
    ldr r7, [r1, #4]
    smulbb r10, r4, r5
    smlatt r10, r4, r5, r10
    ldmib r0, {r6, r8}
    smlabb r10, r6, r7, r10
    smlatt r10, r6, r7, r10
    ldr r9, [r1, #8]
    ldr r4, [r0, #12]
    smlabb r10, r8, r9, r10
    smlatt r10, r8, r9, r10
    ldr r5, [r1, #12]
    ldr r6, [r0, #16]
    smlabb r10, r4, r5, r10
    smlatt r10, r4, r5, r10
    ldr r7, [r1, #16]
    ldr r8, [r0, #20]
    smlabb r10, r6, r7, r10
    smlatt r10, r6, r7, r10
    ldr r9, [r1, #20]
    ldr r4, [r0, #24]
    smlabb r10, r8, r9, r10
    smlatt r10, r8, r9, r10
    ldr r5, [r1, #24]
    ldr r6, [r0, #28]
    smlabb r10, r4, r5, r10
    smlatt r10, r4, r5, r10
    ldr r7, [r1, #28]
    ldr r8, [r0, #32]
    smlabb r10, r6, r7, r10
    smlatt r10, r6, r7, r10
    ldr r9, [r1, #32]
    ldr r4, [r0, #36]
    smlabb r10, r8, r9, r10
    smlatt r10, r8, r9, r10
    ldr r5, [r1, #36]
    ldr r6, [r0, #40]
    smlabb r10, r4, r5, r10
    smlatt r10, r4, r5, r10
    ldr r7, [r1, #40]
    ldr r8, [r0, #44]
    smlabb r10, r6, r7, r10
    smlatt r10, r6, r7, r10
    ldr r9, [r1, #44]
    ldr r4, [r0, #48]
    smlabb r10, r8, r9, r10
    smlatt r10, r8, r9, r10
    ldr r5, [r1, #48]
    ldr r6, [r0, #52]
    smlabb r10, r4, r5, r10
    smlatt r10, r4, r5, r10
    ldr r7, [r1, #52]
    ldr r8, [r0, #56]
    smlabb r10, r6, r7, r10
    ldr r9, [r1, #56]
    smlatt r10, r6, r7, r10
    smlabb r10, r8, r9, r10
    ldr r5, [r1, #60]
    ldr r4, [r0, #60]
    smlatt r10, r8, r9, r10
    smlabb r10, r4, r5, r10
    smlatt r4, r4, r5, r10
    mov r2, r4, lsl #4
    mov r2, r2, asr #16
    strh r2, [r3], #2
    strh r2, [ip], #-2
    add r1, r1, #64
    subs lr, lr, #1
    bne L_0204c1e0
    // final: sum the 32 halfwords at r0 (offsets 0-0x20, then r0 += 0x20 and offsets 2-0x1e) and
    // store the negated total at the saved r2+0x60 (out[48])
    ldrsh r4, [r0, #0]
    ldrsh r3, [r0, #2]
    ldrsh r1, [r0, #4]
    ldrsh r2, [r0, #6]
    add r3, r4, r3
    add r3, r3, r1
    ldrsh r1, [r0, #8]
    add r3, r3, r2
    ldrsh r2, [r0, #10]
    add r3, r3, r1
    ldrsh r1, [r0, #12]
    add r3, r3, r2
    ldrsh r2, [r0, #14]
    add r3, r3, r1
    ldrsh r1, [r0, #16]
    add r3, r3, r2
    ldrsh r2, [r0, #18]
    add r3, r3, r1
    ldrsh r1, [r0, #20]
    add r3, r3, r2
    ldrsh r2, [r0, #22]
    add r3, r3, r1
    ldrsh r1, [r0, #24]
    add r3, r3, r2
    ldrsh r2, [r0, #26]
    add r3, r3, r1
    ldrsh r1, [r0, #28]
    add r3, r3, r2
    ldrsh r2, [r0, #30]
    add r3, r3, r1
    ldrsh r1, [r0, #32]
    add r2, r3, r2
    add r0, r0, #32
    add r4, r2, r1
    ldrsh r3, [r0, #2]
    ldrsh r1, [r0, #4]
    ldrsh r2, [r0, #6]
    add r3, r4, r3
    add r3, r3, r1
    ldrsh r1, [r0, #8]
    add r3, r3, r2
    ldrsh r2, [r0, #10]
    add r3, r3, r1
    ldrsh r1, [r0, #12]
    add r3, r3, r2
    ldrsh r2, [r0, #14]
    add r3, r3, r1
    ldrsh r1, [r0, #16]
    add r3, r3, r2
    ldrsh r2, [r0, #18]
    add r3, r3, r1
    ldrsh r1, [r0, #20]
    add r3, r3, r2
    ldrsh r2, [r0, #22]
    add r3, r3, r1
    ldrsh r1, [r0, #24]
    add r3, r3, r2
    ldrsh r2, [r0, #26]
    add r3, r3, r1
    ldrsh r1, [r0, #28]
    add r2, r3, r2
    ldrsh r0, [r0, #30]
    add r1, r2, r1
    add r0, r1, r0
    rsb r1, r0, #0
    ldr r0, [sp, #0]
    strh r1, [r0, #0]
    add sp, sp, #12
    ldmfd sp!, {r4, r5, r6, r7, r8, r9, r10, r11, pc}
}
