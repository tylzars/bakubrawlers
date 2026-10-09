// NitroSystem G3d: texture SRT matrix builders (the "rotate about centre" family) and node matrix cache. 0x02034d5c-0x02035c68
#include <nnsys/private/nns_g3d.h>
#define U32(p, o) (*(u32 *)((u8 *)(p) + (o)))
#define P32(p, o) (*(s32 *)((u8 *)(p) + (o)))
#define PS16(p, o) (*(s16 *)((u8 *)(p) + (o)))
#define PU16(p, o) (*(u16 *)((u8 *)(p) + (o)))
extern void func_01ff93d8(u32 cmd, const void *params, u32 count);   // geometry command
extern s32 func_01ff8154(s32 a, s32 b);
extern s32 func_01ff80c0();
extern void func_01ff9510(const void *src, void *dst, u32 size);
static inline s32 FxMul(s32 a, s32 b) { return (s32)(((long long)a * b) >> 12); }

// Node matrix cache owned by the G3d state (pointer at data_020baf00, bit set at +0xc4, 6-word records).
typedef struct { u32 pad[0x31]; u32 bits[1]; } NodeCache;
extern NodeCache *data_020baf00;
extern s32 data_020bbd04[][6];

void func_02034d5c(u32 *m, s32 *v, u8 *idx, u32 flags) {   // combine a node translation with the cached parent
    u32 a = idx[1];
    u32 b = idx[2];
    if (flags & 4) {
        m[0] |= 1;
        if (data_020baf00->bits[b >> 5] & (1 << (b & 0x1f))) {
            data_020baf00->bits[a >> 5] |= 1 << (a & 0x1f);
            m[0] |= 0x18;
            return;
        }
        func_01ff9510(data_020bbd04[b], data_020bbd04[a], 0x18);
        func_01ff9510(data_020bbd04[b], &m[4], 0x18);
        return;
    }
    m[1] = v[0];
    m[2] = v[1];
    m[3] = v[2];
    if (data_020baf00->bits[b >> 5] & (1 << (b & 0x1f))) {
        func_01ff9510(v, data_020bbd04[a], 0x18);
        data_020baf00->bits[a >> 5] &= ~(1 << (a & 0x1f));
        m[0] |= 0x18;
        return;
    }
    data_020baf00->bits[a >> 5] &= ~(1 << (a & 0x1f));
    data_020bbd04[a][0] = FxMul(v[0], data_020bbd04[b][0]);
    data_020bbd04[a][1] = FxMul(v[1], data_020bbd04[b][1]);
    data_020bbd04[a][2] = FxMul(v[2], data_020bbd04[b][2]);
    data_020bbd04[a][3] = FxMul(v[3], data_020bbd04[b][3]);
    data_020bbd04[a][4] = FxMul(v[4], data_020bbd04[b][4]);
    data_020bbd04[a][5] = FxMul(v[5], data_020bbd04[b][5]);
    func_01ff9510(data_020bbd04[a], &m[4], 0x18);
}
void func_02034f78(u32 *m) {   // load the texture matrix (translation form)
    u32 buf[15];
    s32 *out = (s32 *)&buf[2];
    if (m[0] & 8) buf[0] = 0x101710;
    else buf[0] = 0x101910;
    buf[1] = 3;
    buf[14] = 2;
    out[13] = 0;
    out[10] = 0;
    out[9] = 0;
    out[8] = 0;
    out[7] = 0;
    out[5] = 0;
    out[4] = 0;
    out[3] = 0;
    if (m[0] & 4) {
        out[11] = 0;
        out[12] = 0;
        if (m[0] & 1) {
            out[0] = 0x1000;
            out[4 + 6 - 4] = 0x1000;
        } else {
            out[0] = m[6];
            out[4 + 6 - 4] = m[7];
        }
    } else if (m[0] & 1) {
        out[11] = -(P32(m, 0x24) << 4) * PU16(m, 0x2c);
        out[12] = -(P32(m, 0x28) << 4) * PU16(m, 0x2e);
        out[0] = 0x1000;
        out[4 + 6 - 4] = 0x1000;
    } else {
        out[11] = -(s32)(((long long)P32(m, 0x18) * P32(m, 0x24)) >> 8) * PU16(m, 0x2c);
        out[12] = -(s32)(((long long)P32(m, 0x1c) * P32(m, 0x28)) >> 8) * PU16(m, 0x2e);
        out[0] = m[6];
        out[4 + 6 - 4] = m[7];
    }
    if (m[12] != 0x1000) {
        out[0] = FxMul(m[12], out[0]);
        out[11] = FxMul(m[12], out[11]);
    }
    if (m[13] != 0x1000) {
        out[6] = FxMul(m[13], out[6]);
        out[12] = FxMul(m[13], out[12]);
    }
    func_01ff93d8(buf[0], &buf[1], 14);
}

#define W16 PU16(p, 0x2c)
#define H16 PU16(p, 0x2e)
void func_02035144(s32 *out, u8 *p) {   // scale, rotate and translate about the texture centre
    s32 w = W16 << 12;
    s32 h = H16 << 12;
    s32 q = func_01ff8154(h, w);
    s32 c = PS16(p, 0x22);
    s32 sS = P32(p, 0x18);
    s32 s = PS16(p, 0x20);
    s32 sT = P32(p, 0x1c);
    s32 r7 = FxMul(sS, c);
    s32 r6 = FxMul(sS, s);
    s32 r5 = FxMul(sT, c);
    s32 r4 = FxMul(sT, s);
    s32 a, b;
    out[0] = r7;
    out[5] = r5;
    out[1] = (r4 * func_01ff80c0(q)) >> 12;
    q = func_01ff8154(w, h);
    a = P32(p, 0x28) * H16 + (-(s32)H16 << 11);
    b = (-(s32)W16 << 11) - P32(p, 0x24) * W16;
    out[12] = (s32)((((long long)r7 * b) - ((long long)r6 * a)) >> 8) + (W16 << 15);
    out[13] = (s32)((((long long)r5 * a) + ((long long)r4 * b)) >> 8) + (H16 << 15);
    out[4] = (-r6 * func_01ff80c0(q)) >> 12;
}
void func_0203524c(s32 *out, u8 *p) {   // rotate and translate about the texture centre
    s32 w = W16 << 12;
    s32 h = H16 << 12;
    s32 q = func_01ff8154(h, w);
    s32 a, b;
    out[0] = PS16(p, 0x22);
    out[5] = PS16(p, 0x22);
    out[1] = (PS16(p, 0x20) * func_01ff80c0(q)) >> 12;
    q = func_01ff8154(w, h);
    a = P32(p, 0x28) * H16 + (-(s32)H16 << 11);
    b = (-(s32)W16 << 11) - P32(p, 0x24) * W16;
    out[12] = (s32)((((long long)PS16(p, 0x22) * b) - ((long long)PS16(p, 0x20) * a)) >> 8) + (W16 << 15);
    out[13] = (s32)((((long long)PS16(p, 0x22) * a) + ((long long)PS16(p, 0x20) * b)) >> 8) + (H16 << 15);
    out[4] = (-PS16(p, 0x20) * func_01ff80c0(q)) >> 12;
}
void func_02035334(s32 *out, u8 *p) {   // scale and translate about the texture centre
    s32 a, b;
    out[0] = P32(p, 0x18);
    out[5] = P32(p, 0x1c);
    out[1] = 0;
    b = (-(s32)W16 << 11) - P32(p, 0x24) * W16;
    out[12] = (s32)(((long long)P32(p, 0x18) * b) >> 8) + (W16 << 15);
    a = P32(p, 0x28) * H16 + (-(s32)H16 << 11);
    out[13] = (s32)(((long long)P32(p, 0x1c) * a) >> 8) + (H16 << 15);
    out[4] = 0;
}
void func_020353b4(s32 *out, u8 *p) {   // translate only
    out[0] = 0x1000;
    out[5] = 0x1000;
    out[1] = 0;
    out[12] = (-P32(p, 0x24) * W16) << 4;
    out[13] = (P32(p, 0x28) * H16) << 4;
    out[4] = 0;
}
void func_020353fc(s32 *out, u8 *p) {   // scale and rotate about the texture centre
    s32 w = W16 << 12;
    s32 h = H16 << 12;
    s32 q = func_01ff8154(h, w);
    s32 c = PS16(p, 0x22);
    s32 sS = P32(p, 0x18);
    s32 s = PS16(p, 0x20);
    s32 sT = P32(p, 0x1c);
    s32 r7 = FxMul(sS, c);
    s32 r6 = FxMul(sS, s);
    s32 r5 = FxMul(sT, c);
    s32 r4 = FxMul(sT, s);
    s32 a = -(s32)H16 << 11;
    s32 b = -(s32)W16 << 11;
    out[0] = r7;
    out[5] = r5;
    out[1] = (r4 * func_01ff80c0(q)) >> 12;
    q = func_01ff8154(w, h);
    out[12] = (s32)((((long long)r7 * b) - ((long long)r6 * a)) >> 8) + (W16 << 15);
    out[13] = (s32)((((long long)r5 * a) + ((long long)r4 * b)) >> 8) + (H16 << 15);
    out[4] = (-r6 * func_01ff80c0(q)) >> 12;
}
void func_020354f4(s32 *out, u8 *p) {   // rotate about the texture centre
    s32 w = W16 << 12;
    s32 h = H16 << 12;
    s32 q = func_01ff8154(h, w);
    s32 a, b;
    out[0] = PS16(p, 0x22);
    out[5] = PS16(p, 0x22);
    out[1] = (PS16(p, 0x20) * func_01ff80c0(q)) >> 12;
    q = func_01ff8154(w, h);
    b = -(s32)W16 << 11;
    a = -(s32)H16 << 11;
    out[12] = (s32)((((long long)PS16(p, 0x22) * b) - ((long long)PS16(p, 0x20) * a)) >> 8) + (W16 << 15);
    out[13] = (s32)((((long long)PS16(p, 0x22) * a) + ((long long)PS16(p, 0x20) * b)) >> 8) + (H16 << 15);
    out[4] = (-PS16(p, 0x20) * func_01ff80c0(q)) >> 12;
}
void func_020355cc(s32 *out, u8 *p) {   // scale about the texture centre
    out[0] = P32(p, 0x18);
    out[5] = P32(p, 0x1c);
    out[1] = 0;
    out[12] = ((0x1000 - P32(p, 0x18)) * W16) << 3;
    out[13] = ((0x1000 - P32(p, 0x1c)) * H16) << 3;
    out[4] = 0;
}
void func_0203561c(s32 *out) {
    out[0] = 0x1000;
    out[1] = 0;
    out[4] = 0;
    out[5] = 0x1000;
    out[12] = 0;
    out[13] = 0;
}
extern void (*data_020a5840[8])(s32 *out, u32 *m);
void func_02035640(u32 *m) {   // load the texture matrix built from a texture SRT (centre-rotate family)
    u32 buf[19];
    s32 *out = (s32 *)&buf[2];
    if (m[0] & 8) buf[0] = 0x101610;
    else buf[0] = 0x101810;
    buf[17] = 0x1000;
    buf[1] = 3;
    buf[18] = 2;
    out[14] = 0;
    out[11] = 0;
    out[10] = 0;
    out[9] = 0;
    out[8] = 0;
    out[7] = 0;
    out[6] = 0;
    out[3] = 0;
    out[2] = 0;
    data_020a5840[m[0] & 7](out, m);
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
void func_02035788(s32 *out, u8 *p) {   // scale, rotate and translate (translation in the rotated frame)
    s32 w = W16 << 12;
    s32 h = H16 << 12;
    s32 q = func_01ff8154(h, w);
    s32 c = PS16(p, 0x22);
    s32 sS = P32(p, 0x18);
    s32 s = PS16(p, 0x20);
    s32 sT = P32(p, 0x1c);
    s32 r4, r5;
    s32 x, y;
    out[0] = FxMul(sS, c);
    r4 = FxMul(sT, c);
    r5 = FxMul(sS, s);
    out[5] = r4;
    out[1] = (FxMul(sT, s) * func_01ff80c0(q)) >> 12;
    q = func_01ff8154(w, h);
    x = (s32)((((long long)P32(p, 0x24) * s) - ((long long)P32(p, 0x28) * c)) >> 12);
    y = (s32)((((long long)P32(p, 0x28) * s) + ((long long)P32(p, 0x24) * c)) >> 12);
    out[12] = ((s32)W16 * (r5 - FxMul(x, sS))) << 4;
    out[13] = (-(s32)H16 * (r4 + FxMul(y, sT) - 0x1000)) << 4;
    out[4] = (-r5 * func_01ff80c0(q)) >> 12;
}
void func_020358dc(s32 *out, u8 *p) {   // rotate and translate (translation in the rotated frame)
    s32 w = W16 << 12;
    s32 h = H16 << 12;
    s32 q = func_01ff8154(h, w);
    s32 s = PS16(p, 0x20);
    s32 c = PS16(p, 0x22);
    s32 x, y;
    out[0] = c;
    out[5] = c;
    out[1] = (s * func_01ff80c0(q)) >> 12;
    q = func_01ff8154(w, h);
    x = (s32)((((long long)P32(p, 0x28) * s) + ((long long)P32(p, 0x24) * c)) >> 12);
    y = (s32)((((long long)P32(p, 0x24) * s) - ((long long)P32(p, 0x28) * c)) >> 12);
    out[12] = (W16 * (s - x)) << 4;
    out[13] = (-(s32)H16 * (c + y - 0x1000)) << 4;
    out[4] = (-s * func_01ff80c0(q)) >> 12;
}
void func_020359bc(s32 *out, u8 *p) {   // scale and translate
    out[0] = P32(p, 0x18);
    out[5] = P32(p, 0x1c);
    out[1] = 0;
    out[12] = (W16 * -FxMul(P32(p, 0x24), P32(p, 0x18))) << 4;
    out[13] = (-(s32)H16 * (P32(p, 0x1c) + FxMul(-P32(p, 0x28), P32(p, 0x1c)) - 0x1000)) << 4;
    out[4] = 0;
}
void func_02035a40(s32 *out, u8 *p) {   // translate only
    out[0] = 0x1000;
    out[5] = 0x1000;
    out[1] = 0;
    out[12] = (W16 * -P32(p, 0x24)) << 4;
    out[13] = (-(s32)H16 * -P32(p, 0x28)) << 4;
    out[4] = 0;
}
void func_02035a94(s32 *out, u8 *p) {   // scale and rotate
    s32 w = W16 << 12;
    s32 h = H16 << 12;
    s32 q = func_01ff8154(h, w);
    s32 c = PS16(p, 0x22);
    s32 sS = P32(p, 0x18);
    s32 s = PS16(p, 0x20);
    s32 sT = P32(p, 0x1c);
    s32 r4, r6;
    out[0] = FxMul(sS, c);
    r4 = FxMul(sT, c);
    r6 = FxMul(sS, s);
    out[5] = r4;
    out[1] = (FxMul(sT, s) * func_01ff80c0(q)) >> 12;
    q = func_01ff8154(w, h);
    out[12] = (W16 * r6) << 4;
    out[13] = (-(s32)H16 * (r4 - 0x1000)) << 4;
    out[4] = (-r6 * func_01ff80c0(q)) >> 12;
}
void func_02035b60(s32 *out, u8 *p) {   // rotate only
    s32 w = W16 << 12;
    s32 h = H16 << 12;
    func_01ff8154(h, w);
    out[0] = PS16(p, 0x22);
    out[5] = PS16(p, 0x22);
    out[1] = (PS16(p, 0x20) * func_01ff80c0()) >> 12;
    func_01ff8154(w, h);
    out[12] = (W16 * PS16(p, 0x20)) << 4;
    out[13] = (-(s32)H16 * (PS16(p, 0x22) - 0x1000)) << 4;
    out[4] = (-PS16(p, 0x20) * func_01ff80c0()) >> 12;
}
void func_02035c04(s32 *out, u8 *p) {   // scale only
    out[0] = P32(p, 0x18);
    out[5] = P32(p, 0x1c);
    out[1] = 0;
    out[12] = 0;
    out[13] = (-(s32)H16 * (P32(p, 0x1c) - 0x1000)) << 4;
    out[4] = 0;
}
void func_02035c44(s32 *out) {
    out[0] = 0x1000;
    out[1] = 0;
    out[4] = 0;
    out[5] = 0x1000;
    out[12] = 0;
    out[13] = 0;
}
