// NitroSystem G3d / Gfd: last texture SRT builder, default VRAM key hooks, frame texture VRAM manager. 0x02035c68-0x020360c4
#include <nnsys/private/nns_g3d.h>
#define P32(p, o) (*(s32 *)((u8 *)(p) + (o)))
extern void func_01ff93d8(u32 cmd, const void *params, u32 count);
static inline s32 FxMul(s32 a, s32 b) { return (s32)(((long long)a * b) >> 12); }

extern void (*data_020a5860[8])(s32 *out, u32 *m);
typedef struct { u32 cmd; u32 n; s32 out[16]; u32 last; } TexCmd;
void func_02035c68(u32 *m) {   // load the texture matrix, forcing identity parts for unset components
    TexCmd c;
    if (m[0] & 8) c.cmd = 0x101610;
    else c.cmd = 0x101810;
    c.n = 3;
    c.last = 2;
    c.out[15] = 0x1000;
    c.out[14] = 0;
    c.out[11] = 0;
    c.out[10] = 0;
    c.out[9] = 0;
    c.out[8] = 0;
    c.out[7] = 0;
    c.out[6] = 0;
    c.out[3] = 0;
    c.out[2] = 0;
    if (m[0] & 1) {
        m[7] = 0x1000;
        m[6] = 0x1000;
    }
    if (m[0] & 2) {
        *(s16 *)((u8 *)m + 0x22) = 0x1000;
        *(s16 *)((u8 *)m + 0x20) = 0;
    }
    if (m[0] & 4) {
        m[10] = 0;
        m[9] = 0;
    }
    data_020a5860[m[0] & 7](c.out, m);
    if (m[12] != 0x1000) {
        c.out[0] = FxMul(m[12], c.out[0]);
        c.out[1] = FxMul(m[12], c.out[1]);
        c.out[12] = FxMul(m[12], c.out[12]);
    }
    if (m[13] != 0x1000) {
        c.out[4] = FxMul(m[13], c.out[4]);
        c.out[5] = FxMul(m[13], c.out[5]);
        c.out[13] = FxMul(m[13], c.out[13]);
    }
    func_01ff93d8(c.cmd, (u32 *)&c + 1, 0x12);
}
u32 func_02035df0(void) { return 0; }    // default texture VRAM allocator (fails)
s32 func_02035df8(void) { return -1; }   // default texture VRAM free
u32 func_02035e00(void) { return 0; }    // default palette VRAM allocator (fails)
s32 func_02035e08(void) { return -1; }   // default palette VRAM free

typedef struct { u32 start; u32 end; u32 enabled; u32 mapped; u32 pad10; u32 pad14; } TexSlot;   // 0x18 bytes
extern TexSlot data_020a58ac[5];
extern TexSlot *data_020a5890[];   // slot pointers used from +8
extern u16 data_022115d8;
void func_02035e10(u32 a, u32 b, u32 c, u32 d, u32 e) {   // order the five texture slots
    data_020a5890[2] = &data_020a58ac[a];
    data_020a5890[3] = &data_020a58ac[b];
    data_020a5890[4] = &data_020a58ac[c];
    data_020a5890[5] = &data_020a58ac[d];
    data_020a5890[6] = &data_020a58ac[e];
}
extern u32 func_02035f54(u32 size, int is4x4, u32 opt);
extern int func_020360c4(u32 key);
extern u32 (*data_020a5880)(u32 size, int is4x4, u32 opt);
extern int (*data_020a5884)(u32 key);
extern void func_02035ee0(void);
void func_02035e58(u16 mode, BOOL install) {   // NNS_GfdInitFrmTexVramManager
    if (mode <= 2) func_02035e10(4, 3, 2, 0, 1);
    else func_02035e10(4, 3, 0, 2, 1);
    data_022115d8 = mode;
    func_02035ee0();
    if (install) {
        data_020a5880 = func_02035f54;
        data_020a5884 = func_020360c4;
    }
}
void func_02035ee0(void) {
    int i;
    TexSlot *s = data_020a58ac;
    u32 n = data_022115d8;
    if (n > 1) n++;
    for (i = 0; i < 5; i++, s++) {
        if (i < (int)n) s->enabled = 1;
        else s->enabled = 0;
        if (s->mapped != 0) {
            s->start = 0;
            s->end = 0x10000;
        } else {
            s->start = 0;
            s->end = 0x20000;
        }
    }
}
static inline int Alloc4x4(u32 *key, u32 aligned) {
    int i;
    for (i = 0; i < 2; i++) {
        TexSlot *s = data_020a5890[i];
        TexSlot *idx;
        if (s->enabled == 0) continue;
        if (s->end - s->start < aligned) continue;
        switch (*(u16 *)((u8 *)s + 0x10)) { case 0: idx = &data_020a58ac[1]; break; case 3: idx = &data_020a58ac[2]; break; default: idx = 0; break; }
        if (idx->enabled == 0) continue;
        if (idx->end - idx->start < (aligned >> 1)) continue;
        s->start += aligned; *key = s->start - aligned + s->pad14; idx->start += aligned >> 1;
        return 1;
    }
    return 0;
}
static inline int AllocNormal(u32 *key, u32 aligned) {
    TexSlot **slots = &data_020a5890[2];
    int i;
    for (i = 0; i < 5; i++) {
        TexSlot *s = slots[i];
        if (s->enabled == 0) continue;
        if (s->end - s->start < aligned) continue;
        s->end = s->end - aligned;
        *key = s->end + s->pad14;
        return 1;
    }
    return 0;
}
u32 func_02035f54(u32 size, int is4x4, u32 opt) {
    u32 key;
    u32 aligned;
    int ok;
    if (size == 0) aligned = 0x10;
    else aligned = (size + 0xf) & ~0xf;
    if (aligned >= 0x7fff0) return 0;
    if (is4x4) ok = Alloc4x4(&key, aligned);
    else ok = AllocNormal(&key, aligned);
    if (ok == 0) return 0;
    return ((key << 13) >> 16) | ((aligned >> 4) << 16) | (is4x4 << 31);
}
