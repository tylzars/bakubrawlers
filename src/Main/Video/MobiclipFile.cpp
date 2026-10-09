#include "Video/MobiclipDecoder.hpp"
typedef struct { u8 pad[0x24]; u32 o24, o28, o2c; } FSFile;
extern "C" {
void MIi_CpuCopyFast(const void *src, void *dst, u32 n);
void func_0200ab18(FSFile *);
int func_0200ac40(FSFile *, u32, u32);
int func_0200ac30(FSFile *, void *, u32);
int func_0200ac20(FSFile *, void *, u32);
void *OS_AllocFromHeap(int, int, u32);
void OS_FreeToHeap(int, int, void *);
struct F { u32 o0; u32 o4; u32 o8; FSFile *file; u8 flag; };
int func_0203b658(F *f, FSFile *file) {
    f->file = file;
    f->o4 = file->o28 - file->o24;
    f->o8 = f->file->o2c - f->file->o24;
    f->flag = 0;
    return 1;
}
int func_0203b690(F *f, u32 pos) {
    if (f->flag == 1) {
        func_0200ab18(f->file);
        f->flag = 0;
    }
    if (func_0200ac40(f->file, pos, 0) == 0) return 0;
    f->o8 = pos;
    return 1;
}
int func_0203b6dc(F *f, void *buf, u32 n) {
    if (f->flag == 1) {
        func_0200ab18(f->file);
        func_0200ac40(f->file, f->o8, 0);
        f->flag = 0;
    }
    if (func_0200ac30(f->file, buf, n) == -1) return 0;
    f->o8 += n;
    return 1;
}
int func_0203b74c(F *f, void *buf, u32 n) {
    u32 aligned = f->o8 & -0x200;
    u32 off = f->o8 - aligned;
    u32 len = n + off;
    if (len & 0x1ff) len = (len & -0x200) + 0x200;
    func_0200ac40(f->file, aligned, 0);
    if (func_0200ac20(f->file, buf, len) == -1) return 0;
    f->o8 += n;
    f->flag = 1;
    return off;
}
void func_0203b7c8(F *f) {
    if (f->flag == 1) func_0200ab18(f->file);
    f->flag = 0;
}
void func_0203b7f0(Src *s) { s->adv(); }
void func_0203b804() {}

struct G { u32 a, b, c, s0c; u8 *p10; u32 s14; u8 *p18; u32 pad; u32 d; };
extern G data_02215860;
u32 func_0203bdc8() { return 0x659c; }
u32 func_0203bdd4() { return 0x2580; }
void func_0203bddc(u8 *p, u32 n) {
    data_02215860.p18 = p;
    data_02215860.s14 = n & ~3;
    data_02215860.c = 0;
}
void func_0203bdfc(u8 *p, u32 n) {
    data_02215860.p10 = p;
    data_02215860.s0c = n & ~3;
    data_02215860.b = 0;
    data_02215860.a = 0;
    data_02215860.d = 0;
}
#define ALLOC(name, field, size, src) \
void *name() { \
    if (data_02215860.field == 0) { \
        if (data_02215860.s0c < size) return (void *) src; \
        data_02215860.field = (u32) data_02215860.p10; \
        MIi_CpuCopyFast((void *) src, (void *) data_02215860.field, size); \
        data_02215860.p10 += size; \
        data_02215860.s0c -= size; \
    } \
    return (void *) data_02215860.field; \
}
void *func_0203be24() {
    if (data_02215860.c == 0) {
        if (data_02215860.s14 < 0x659c) return (void *) 0x2096a90;
        data_02215860.c = (u32) data_02215860.p18;
        MIi_CpuCopyFast((void *) 0x2096a90, data_02215860.p18, 0x659c);
        data_02215860.p18 += 0x659c;
        data_02215860.s14 -= 0x659c;
    }
    return (void *) data_02215860.c;
}
void *func_0203bea0() {
    if (data_02215860.b == 0) {
        if (data_02215860.s0c < 0x2100) return (void *) 0x2094990;
        data_02215860.b = (u32) data_02215860.p10;
        MIi_CpuCopyFast((void *) 0x2094990, data_02215860.p10, 0x2100);
        data_02215860.p10 += 0x2100;
        data_02215860.s0c -= 0x2100;
    }
    return (void *) data_02215860.b;
}
extern u8 data_0209d6f8[], data_0209d578[];
void *func_0203bf10() {
    if (data_02215860.a == 0) {
        if (data_02215860.s0c < 0x300) return data_0209d6f8;
        data_02215860.a = (u32) data_02215860.p10;
        MIi_CpuCopyFast(data_0209d6f8, data_02215860.p10, 0x300);
        data_02215860.p10 += 0x300;
        data_02215860.s0c -= 0x300;
    }
    return (void *) data_02215860.a;
}
void *func_0203bf80() {
    if (data_02215860.d == 0) {
        if (data_02215860.s0c < 0x180) return data_0209d578;
        data_02215860.d = (u32) data_02215860.p10;
        MIi_CpuCopyFast(data_0209d578, data_02215860.p10, 0x180);
        data_02215860.p10 += 0x180;
        data_02215860.s0c -= 0x180;
    }
    return (void *) data_02215860.d;
}
void func_0203bff0(u32 *s, u8 *p) {
    s[1] = p[0];
    s[0] = *(s16 *) (p + 2);
}
// non-matching: 0x150 vs 0x158; the clamp constants and table pointers are allocated differently and a zero is hoisted (mov r5, r7) where the original uses movmi.
struct AdSt { int pred; int idx; };
extern const s8 data_0209d9f8[];
extern const s16 data_0209da08[];
#define NIB(n) \
    { \
        int idx = st->idx; \
        st->idx = idx + data_0209d9f8[(n) & 7]; \
        if (st->idx < 0) st->idx = 0; \
        int step = data_0209da08[idx]; \
        if (st->idx > 0x58) st->idx = 0x58; \
        int diff = step >> 3; \
        if ((n) & 4) diff += step; \
        if ((n) & 2) diff += step >> 1; \
        if ((n) & 1) diff += step >> 2; \
        if ((n) & 8) st->pred -= diff; else st->pred += diff; \
        if (st->pred < -0x8000) st->pred = -0x8000; \
        if (st->pred > 0x7fff) st->pred = 0x7fff; \
    }
void func_0203c004(AdSt *st, u8 *in, int n, u32 *out) {
    for (int i = 0; i < n;) {
        int b = in[i];
        i++;
        NIB(b)
        int s1 = st->pred;
        b >>= 4;
        NIB(b)
        *out++ = (st->pred << 16) | ((u32) (s1 << 16) >> 16);
    }
}

void *func_0203c15c(u32 n) { return OS_AllocFromHeap(0, -1, n); }
void func_0203c174(void *q) { OS_FreeToHeap(0, -1, q); }
}
