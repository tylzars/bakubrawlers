/* CRI file reader objects, 0x02047d80-0x02048304 */
typedef struct FObj {
    signed char used, state, b2, b3;
    int h4[18];            /* embedded file handle, 0x4..0x4b */
    int *hp;               /* 0x4c */
    int mode;              /* 0x50 */
    int i54;
    int secsize;           /* 0x58 */
    int size;              /* 0x5c */
    int nsect;             /* 0x60 */
    int pos;               /* 0x64 */
    int i68;
    int i6c;
    int i70;
    int i74;
    int i78;
    int idx;               /* 0x7c */
    int base;              /* 0x80 */
} FObj;

extern FObj data_02217d14[10];
extern char data_020ade54[], data_020ade7c[], data_020adea4[], data_020aded8[], data_020adefc[];
extern char data_020adf14[], data_020adf2c[], data_020adf44[], data_020adf68[], data_020adf8c[];
extern char data_020adfb8[], data_020adfd0[], data_020adfe8[], data_020ae000[], data_020ae018[], data_020ae034[];

extern void func_020479c8(int, const char *);
extern void func_02047a4c(char *, int);
extern int func_02047d00(int, int *, int *, int *);
extern void func_0200a7b4(void *);
extern int func_0200aa94(void *, char *);
extern void func_0200aadc(void *);
extern void func_0200ac40(int *, int, int);
extern void func_0200abd8(int *);
extern void func_02012484(void *, int, int);
extern void func_02046154(void);
extern void func_02046164(void);
extern void func_02049230(void);

FObj *func_02047d80(int name, int unused, int flag)
{
    char buf[0x200];
    int a, b, c;
    FObj *o;
    int i, size, ss;
    if (name == 0) {
        func_020479c8(0, data_020ade54);
        return 0;
    }
    if (flag != 0) {
        func_020479c8(0, data_020ade7c);
        return 0;
    }
    for (i = 0; i < 10; i++) {
        o = &data_02217d14[i];
        if (o->used == 0) break;
    }
    if (i == 10) {
        func_020479c8(0, data_020adea4);
        return 0;
    }
    o->idx = i;
    func_02047a4c(buf, name);
    if (func_02047d00(name, &a, &b, &c) == 1) {
        o->hp = (int *)a;
        o->base = b;
        size = c;
    } else {
        func_0200a7b4(o->h4);
        o->hp = o->h4;
        if (func_0200aa94(o->h4, buf) == 0) {
            func_020479c8(0, data_020aded8);
            return 0;
        }
        o->base = 0;
        size = o->hp[10] - o->hp[9];
    }
    o->size = size;
    o->secsize = 0x800;
    o->nsect = o->size / o->secsize + (o->size % o->secsize > 0);
    o->pos = 0;
    o->i54 = 0;
    o->i6c = 0;
    o->i68 = 0;
    o->state = 0;
    o->used = 1;
    o->mode = 0;
    return o;
}

void func_02047f20(FObj *o)
{
    if (o == 0) return;
    if (o->hp == o->h4) func_0200aadc(o->hp);
    o->used = 0;
    func_02012484(o, 0, 0x84);
}

int func_02047f58(FObj *o, int off, int whence)
{
    int r = 0;
    if (o == 0) {
        func_020479c8(r, data_020adefc);
        return r;
    }
    func_02046154();
    if (whence == 0) {
        r = off;
    } else if (whence == 2) {
        r = o->nsect + off;
    } else if (whence == 1) {
        r = o->pos + off;
    }
    if (r >= o->nsect) r = o->nsect;
    if (r <= 0) r = 0;
    o->pos = r;
    func_02046164();
    func_0200ac40(o->hp, o->pos * o->secsize + o->base, 0);
    return o->pos;
}

int func_02047ff4(FObj *o)
{
    if (o != 0) return o->pos;
    func_020479c8(0, data_020adf14);
    return 0;
}

int func_0204801c(FObj *o, int n, int arg)
{
    if (o == 0) {
        func_020479c8(0, data_020adf2c);
        return 0;
    }
    if (n < 0) {
        func_020479c8((int)o, data_020adf44);
        return 0;
    }
    if (arg == 0) {
        func_020479c8((int)o, data_020adf68);
        return 0;
    }
    if (n >= 0x100000) {
        func_020479c8((int)o, data_020adf8c);
        return 0;
    }
    if (o->mode == 2) return 0;
    if (n == 0) {
        o->state = 1;
        return 0;
    }
    func_02046154();
    o->i6c = n;
    o->i74 = o->i6c * o->secsize;
    o->mode = 1;
    o->i54 = arg;
    o->state = 2;
    func_02046164();
    return o->i6c;
}

void func_020480f8(FObj *o)
{
    int st;
    if (o == 0) {
        func_020479c8(0, data_020adfb8);
        return;
    }
    st = o->state;
    func_02046154();
    o->state = 0;
    o->mode = 0;
    func_02046164();
    if (st == 2) func_0200abd8(o->hp);
}

int func_02048144(FObj *o)
{
    if (o != 0) return o->state;
    func_020479c8(0, data_020adfd0);
    return 0;
}

int func_0204816c(FObj *o)
{
    if (o != 0) return o->secsize;
    func_020479c8(0, data_020adfe8);
    return 0;
}

void func_02048194(FObj *o, int sz)
{
    int pos;
    if (o == 0) { func_020479c8(0, data_020ae000); return; }
    if (sz % 32 != 0) { func_020479c8(0, data_020ae018); return; }
    pos = o->pos * o->secsize;
    o->secsize = sz;
    o->nsect = o->size / o->secsize + (o->size % o->secsize > 0);
    o->pos = pos / o->secsize;
    o->i68 = o->i6c * o->secsize;
}

int func_0204823c(FObj *o)
{
    if (o != 0) return o->i68;
    func_020479c8(0, data_020ae034);
    return 0;
}

int func_02048264(void) { return 0; }
int func_0204826c(FObj *o) { return o->i68; }

int func_02048274(int name)
{
    int a, b, c;
    int tmp[18];
    int len;
    if (func_02047d00(name, &a, &b, &c) == 1) {
        len = c;
    } else {
        func_0200a7b4(tmp);
        if (func_0200aa94(tmp, (char *)name) == 0) return 0;
        len = tmp[10] - tmp[9];
        func_0200aadc(tmp);
    }
    return len;
}

void func_020482e0(void) { func_02046154(); }
void func_020482ec(void) { func_02046164(); }
void func_020482f8(void) { func_02049230(); }
