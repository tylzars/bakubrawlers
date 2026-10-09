// NitroSystem G3d / Gfd: last texture SRT builder, default VRAM key hooks, frame texture VRAM manager. 0x02035c68-0x020360c4
#include <nnsys/private/nns_g3d.h>
#define P32(p, o) (*(s32 *)((u8 *)(p) + (o)))
extern void func_01ff93d8(u32 cmd, const void *params, u32 count);
static inline s32 FxMul(s32 a, s32 b) { return (s32)(((long long)a * b) >> 12); }

extern void (*data_020a5860[8])(s32 *out, u32 *m);
void func_02035c68(u32 *m) {   // load the texture matrix, forcing identity parts for unset components
    u32 buf[19];
    s32 *out = (s32 *)&buf[2];
    if (m[0] & 8) buf[0] = 0x101610;
    else buf[0] = 0x101810;
    buf[1] = 3;
    buf[18] = 2;
    buf[17] = 0x1000;
    out[14] = 0;
    out[11] = 0;
    out[10] = 0;
    out[9] = 0;
    out[8] = 0;
    out[7] = 0;
    out[6] = 0;
    out[3] = 0;
    out[2] = 0;
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
    data_020a5860[m[0] & 7](out, m);
    if (m[12] != 0x1000) {
        out[0] = FxMul(m[12], out[0]);
        out[1] = FxMul(m[12], out[1]);
        out[12] = FxMul(m[12], out[12]);
    }
    if (m[13] != 0x1000) {
        out[4] = FxMul(m[13], out[4]);
        out[5] = FxMul(m[13], out[5]);
        out[13] = FxMul(m[13], out[13]);
    }
    func_01ff93d8(buf[0], &buf[1], 0x12);
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
void func_02035ee0(void) {   // reset the five slots
    int n = data_022115d8;
    int i;
    TexSlot *s = data_020a58ac;
    if (n > 1) n++;
    for (i = 0; i < 5; i++, s++) {
        if (i < n) s->enabled = 1;
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
extern TexSlot *data_020a5890_slots[];
u32 func_02035f54(u32 size, int is4x4, u32 opt) {   // NNS_GfdAllocFrmTexVram
    u32 key;
    u32 aligned;
    int i;
    if (size == 0) aligned = 0x10;
    else aligned = (size + 0xf) & ~0xf;
    if (aligned >= 0x7fff0) return 0;
    if (is4x4) {
        for (i = 0; i < 2; i++) {
            TexSlot *s = data_020a5890[i];
            TexSlot *idx;
            u32 old;
            if (s->enabled == 0) continue;
            if (s->end - s->start < aligned) continue;
            if (*(u16 *)((u8 *)s + 0x10) == 0) idx = &data_020a58ac[1];
            else if (*(u16 *)((u8 *)s + 0x10) == 3) idx = &data_020a58ac[2];
            else idx = 0;
            if (idx->enabled == 0) continue;
            if (idx->end - idx->start < (aligned >> 1)) continue;
            old = s->start;
            s->start = old + aligned;
            idx->start += aligned >> 1;
            key = old + s->pad14;
            goto done;
        }
        return 0;
    } else {
        TexSlot **slots = &data_020a5890[2];
        for (i = 0; i < 5; i++) {
            TexSlot *s = slots[i];
            if (s->enabled == 0) continue;
            if (s->end - s->start < aligned) continue;
            s->end = s->end - aligned;
            key = s->end + s->pad14;
            goto done;
        }
        return 0;
    }
done:
    return ((key << 13) >> 16) | ((aligned >> 4) << 16) | (is4x4 << 31);
}
