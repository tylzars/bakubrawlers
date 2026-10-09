/* CRI MPEG audio: synthesis filter driver and table setup, 0x0204d78c-0x0204d968 */
#pragma dont_inline on
typedef struct MpSyn {
    signed char used;            /* 0x00 */
    char pad1[3];
    int pos[2];                  /* 0x04 */
    short *win;                  /* 0x0c */
    short *coef;                 /* 0x10 */
    short buf[2][1024];          /* 0x14 */
} MpSyn;                         /* 0x814 */

typedef struct MpTab {
    int count;                   /* 0x00 */
    char *coefCopy;              /* 0x04 */
    char *winCopy;               /* 0x08 */
    int ready;                   /* 0x0c */
} MpTab;

extern MpTab data_02218f20;
extern char *data_02218f30;
extern char *data_02218f34;
extern void (*data_020af5a0[16])(short *, short *, short *);

extern void func_0204c088(short *, short *, short *);
extern void func_02012484(void *, int, int);
extern void func_0204ad00(void);
void func_0204d864(void);

void func_0204d78c(MpSyn *s, short *in, int ch, short *out) {
    short *dst;
    int pos;
    s->pos[ch] = (s->pos[ch] - 0x40) & 0x3ff;
    pos = s->pos[ch];
    dst = &s->buf[ch][pos];
    if (((unsigned)in & 0x1f) != 0 || ((unsigned)s->win & 0x1f) != 0) for (;;) {}
    func_0204c088(in, s->win, dst);
    pos = s->pos[ch];
    data_020af5a0[pos >> 6](&s->buf[ch][pos], s->coef, out);
}

void func_0204d814(char *p) { data_02218f34 = p; }
void func_0204d824(char *p) { data_02218f30 = p; }

void func_0204d834(void) {
    if (data_02218f20.count == 0) func_0204d864();
    data_02218f20.count++;
}

void func_0204d864(void) {
    int i;
    char *src;
    char *dst;
    if (data_02218f20.ready != 0) return;
    src = data_02218f30;
    dst = (char *)(((unsigned)src + 0x1f) & ~0x1f);
    data_02218f20.coefCopy = dst;
    for (i = 0x400; i >= 0; i--) dst[i] = src[i];
    src = data_02218f34;
    dst = (char *)(((unsigned)src + 0x1f) & ~0x1f);
    data_02218f20.winCopy = dst;
    for (i = 0x1000; i >= 0; i--) dst[i] = src[i];
    data_02218f20.ready = 1;
}

void func_0204d8e4(void) {
    data_02218f20.count--;
}

MpSyn *func_0204d8fc(MpSyn *o, int size) {
    func_02012484(o, 0, size);
    o->pos[0] = o->pos[1] = 0x40;
    o->coef = (short *)data_02218f20.coefCopy;
    o->win = (short *)data_02218f20.winCopy;
    o->used = 1;
    return o;
}

void func_0204d944(MpSyn *o) {
    func_02012484(o, 0, 0x814);
}

void func_0204d95c(void) {
    func_0204ad00();
}
