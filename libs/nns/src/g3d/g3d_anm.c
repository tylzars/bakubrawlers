// NitroSystem G3d: animation data evaluation (material colour, texture SRT, ...). 0x0203382c-
#include <nnsys/private/nns_g3d.h>
#define U16(p, o) (*(u16 *)((u8 *)(p) + (o)))
#define U32(p, o) (*(u32 *)((u8 *)(p) + (o)))
#define U8(p, o) (*((u8 *)(p) + (o)))

extern void *data_020a5798;
extern void *data_020a5790;
extern s32 func_0203340c(Dict *dict, const u8 *name);
extern void MIi_CpuClear16(u32 data, void *dest, u32 size);
extern void *data_020a5794;
extern void *func_02033714(u8 *res, u32 idx, u32 frame);
extern u8 *func_020336bc(u8 *res, u32 idx);
extern u8 *func_020336e8(u8 *res, u32 idx);
extern void *data_020a5788;
extern void func_01ff93d8(u32 cmd, const void *params, u32 count);   // geometry command
extern void FX_DivAsync(s32 a, s32 b);
extern s32 FX_GetDivResult(void);
static inline s32 FxMul(s32 a, s32 b) { return (s32)(((long long)a * b) >> 12); }
#define P32(p, o) (*(s32 *)((u8 *)(p) + (o)))
#define PS16(p, o) (*(s16 *)((u8 *)(p) + (o)))
#define PU16(p, o) (*(u16 *)((u8 *)(p) + (o)))
extern void (*data_020a5820[8])(s32 *out, u32 *m);
static inline s32 SampleAt(u8 *p, u32 info, u32 i) {
    if (info & 0x10000000) return *(s16 *)(p + i * 2);
    return *(s32 *)(p + i * 4);
}
static inline s32 SampleBlend(u8 *p, u32 info, u32 i) {
    s32 a, b;
    if (!(info & 0x10000000)) { a = *(s32 *)(p + i * 4); b = *(s32 *)(p + i * 4 + 4); }
    else { a = *(s16 *)(p + i * 2); b = *(s16 *)(p + i * 2 + 2); }
    return (a + b) >> 1;
}

static inline u16 Blend555(u8 *p, u32 a) {
    u32 v1 = *(u16 *)(p + a * 2);
    u32 v2;
    p += a * 2;
    v2 = *(u16 *)(p + 2);
    return (u16)((((v1 & 0x7c1f) + (v2 & 0x7c1f)) >> 1 & 0x7c1f) | ((((v1 & 0x3e0) + (v2 & 0x3e0)) >> 1) & 0x3e0));
}
u16 func_0203382c(u8 *base, u32 info, u32 idx) {   // read one 5-5-5 colour from an animation track
    u8 *p;
    u32 last;
    u32 rem;
    u32 c;
    u32 d;
    u32 v1;
    u32 v2;
    if (info & 0x20000000) return (u16)info;
    p = base + (u16)info;
    if ((info & 0xc0000000) == 0) return *(u16 *)(p + idx * 2);
    last = (info & 0x1fff0000) >> 16;
    if (info & 0x40000000) {
        if (idx & 1) {
            if (idx > last) return *(u16 *)((p + (last & ~1)) + 2);
            rem = idx >> 1;
            goto blend;
        }
        return *(u16 *)(p + (idx & ~1));
    }
    rem = idx & 3;
    if (rem != 0) {
        if (idx > last) return *(u16 *)(p + (last >> 2) * 2 + rem * 2);
        if (idx & 1) {
            if (idx & 2) { d = idx >> 2; c = d + 1; }
            else { c = idx >> 2; d = c + 1; }
            v1 = *(u16 *)(p + c * 2);
            v2 = *(u16 *)(p + d * 2);
            return (u16)((((v1 & 0x7c1f) + (v1 & 0x7c1f) + (v1 & 0x7c1f) + (v2 & 0x7c1f)) >> 2 & 0x7c1f) | ((((v1 & 0x3e0) + (v1 & 0x3e0) + (v1 & 0x3e0) + (v2 & 0x3e0)) >> 2) & 0x3e0));
        }
        rem = idx >> 2;
        goto blend;
    }
    idx >>= 2;
    return *(u16 *)(p + idx * 2);
blend:
    return Blend555(p, rem);
}
u32 func_02033994(u8 *base, u32 info, u32 idx) {   // read one byte from an animation track
    u8 *p;
    u8 *q;
    u32 rem;
    u32 last;
    u32 a, b;
    u32 v1, v2;
    if (info & 0x20000000) return (u16)info;
    p = base + (u16)info;
    if ((info & 0xc0000000) == 0) return p[idx];
    last = (info & 0x1fff0000) >> 16;
    if (info & 0x40000000) {
        if (idx & 1) {
            if (idx > last) {
                q = p + (last >> 1);
                return q[1];
            }
            q = p + (idx >> 1);
            return (u32)(q[0] + q[1]) << 15 >> 16;
        }
        return p[idx >> 1];
    }
    rem = idx & 3;
    if (rem != 0) {
        if (idx > last) {
            q = p + (last >> 2);
            return q[rem];
        }
        if (idx & 1) {
            if (idx & 2) { idx >>= 2; a = idx + 1; }
            else { a = idx >> 2; idx = a + 1; }
            v1 = p[a] * 3;
            v2 = p[idx];
            return (u32)(v1 + v2) << 14 >> 16;
        }
        q = p + (idx >> 2);
        return (u32)(q[0] + q[1]) << 15 >> 16;
    }
    return p[idx >> 2];
}
void func_02033a80(u8 *obj, u8 *anm, u8 *mdl) {   // NNS_G3dAnmObjInitNsBma: map material animation entries to materials
    u32 i;
    u8 *mat;
    u32 n;
    if (mdl != 0 && U32(mdl, 8) != 0) mat = mdl + U32(mdl, 8);
    else mat = 0;
    U32(obj, 0xc) = (u32)data_020a5798;
    n = U8(mdl, 0x18);
    U8(obj, 0x19) = n;
    MIi_CpuClear16(0, obj + 0x1a, n * 2);
    for (i = 0; i < U8(anm, 9); i++) {
        u8 *name = DictName((Dict *)(anm + 8), i);
        s32 r = mat == 0 ? -1 : func_0203340c((Dict *)(mat + 4), name);
        if (r >= 0) ((u16 *)(obj + 0x1a))[r] = i | 0x100;
    }
}
// entry idx of the animation's data dictionary, or 0 when idx is out of range
static inline u32 *AnmEntryByIdx(u8 *res, u32 idx) {
    u8 *d = res + 8;
    if (d != 0 && (u16)idx < (u32)U8(res, 9)) {
        u8 *t = d + U16(res, 0xe);
        return (u32 *)(t + 4 + *(u16 *)t * (u16)idx);
    }
    return 0;
}
void func_02033b54(u32 *out, u8 *obj, u32 idx) {   // material colour animation at the current frame
    u8 *res = (u8 *)U32(obj, 8);
    u32 frame = (u32)((s32)U32(obj, 0) >> 12);
    u32 *e = AnmEntryByIdx(res, idx);
    u32 a, b;
    a = func_0203382c(res, e[0], frame);
    b = func_0203382c(res, e[1], frame);
    out[1] = a | (b << 16) | (((out[1] & 0x8000) != 0) << 15);
    a = func_0203382c(res, e[3], frame);
    b = func_0203382c(res, e[2], frame);
    out[2] = b | (a << 16) | (((out[2] & 0x8000) != 0) << 15);
    out[3] = (out[3] & ~0x1f0000) | (func_02033994(res, e[4], frame) << 16);
}
s32 func_02033c44(u8 *base, u32 info, u32 value, u32 idx) {
    u32 last, rem;
    if (info & 0x20000000) return value;
    base += value;
    if (info & 0xc0000000) {
        last = (u16) info;
        if (info & 0x40000000) {
            if (idx & 1) {
                if (idx > last) {
                    idx = (last >> 1) + 1;
                    goto single;
                }
                idx = idx >> 1;
                goto blend;
            }
            idx = idx >> 1;
            goto single;
        }
        rem = idx & 3;
        if (rem) {
            if (idx > last) {
                idx = rem + (last >> 2);
                goto single;
            }
            if (idx & 1) {
                u32 a, b;
                s32 x, y;
                if (idx & 2) {
                    a = (idx >> 2) + 1;
                    b = idx >> 2;
                } else {
                    a = idx >> 2;
                    b = a + 1;
                }
                if (info & 0x10000000) {
                    x = *(s16 *) (base + a * 2);
                    y = *(s16 *) (base + b * 2);
                } else {
                    x = *(s32 *) (base + a * 4);
                    y = *(s32 *) (base + b * 4);
                }
                x *= 3;
                return (x + y) >> 2;
            }
            idx = idx >> 2;
            goto blend;
        }
        idx = idx >> 2;
        goto single;
    }
    single:
    if (info & 0x10000000) return *(s16 *) (base + idx * 2);
    return *(s32 *) (base + idx * 4);
blend:
    {
        s32 x, y;
        if (info & 0x10000000) {
            s16 *q = (s16 *) base + idx;
            x = ((s16 *) base)[idx];
            y = q[1];
        } else {
            x = *(s32 *) (base + idx * 4);
            y = *(s32 *) (base + idx * 4 + 4);
        }
        return (x + y) >> 1;
    }
}
u32 func_02033d44(u8 *base, u32 info, u32 value, u32 idx) {   // read an interpolated pair of 16-bit values
    u8 *p;
    u32 last, rem, c;
    if (info & 0x20000000) return value;
    p = base + value;
    if ((info & 0xc0000000) != 0) {
        last = (u16)info;
        if (info & 0x40000000) {
            if (idx & 1) {
                if (idx > last) {
                    idx = (last >> 1) + 1;
                } else {
                    c = idx >> 1;
                    goto blend;
                }
            } else {
                idx >>= 1;
            }
        } else {
            rem = idx & 3;
            if (rem != 0) {
                if (idx > last) {
                    idx = rem + (last >> 2);
                } else if (idx & 1) {
                    u32 a, b;
                    s32 f0, f1, s0, s1;
                    if (idx & 2) { b = idx >> 2; a = b + 1; }
                    else { a = idx >> 2; b = a + 1; }
                    f1 = *(s16 *)(p + a * 4 + 2);
                    f0 = *(s16 *)(p + a * 4);
                    s1 = *(s16 *)(p + b * 4 + 2);
                    s0 = *(s16 *)(p + b * 4);
                    return (u16)((f0 + f0 + f0 + s0) >> 2) | (((f1 + f1 + f1 + s1) >> 2) << 16);
                } else {
                    c = idx >> 2;
                    goto blend;
                }
            } else {
                idx >>= 2;
            }
        }
    }
    return *(u32 *)(p + idx * 4);
blend:
    {
        s32 f0 = *(s16 *)(p + c * 4);
        s32 f1, s0, s1;
        p += c * 4;
        f1 = *(s16 *)(p + 2);
        s0 = *(s16 *)(p + 4);
        s1 = *(s16 *)(p + 6);
        return (u16)((f0 + s0) >> 1) | (((f1 + s1) >> 1) << 16);
    }
}
void func_02033e50(u8 *res, u32 idx, u32 frame, u8 *out) {   // texture SRT animation entry at a frame
    u32 flags;
    s32 a, b;
    u32 pr;
    u32 *e;
    if (res + 8 != 0 && idx < U8(res, 9)) {
        u8 *t = res + 8 + U16(res, 0xe);
        e = (u32 *)(t + 4 + *(u16 *)t * idx);
    } else {
        e = 0;
    }
    flags = U32(out, 0);
    a = func_02033c44(res, e[6], e[7], frame);
    b = func_02033c44(res, e[8], e[9], frame);
    if (a == 0 && b == 0) {
        flags |= 4;
    } else {
        U32(out, 0x24) = a;
        U32(out, 0x28) = b;
        flags &= ~4;
    }
    pr = func_02033d44(res, e[4], e[5], frame);
    if (pr == 0x10000000) {
        flags |= 2;
    } else {
        U16(out, 0x20) = pr;
        U16(out, 0x22) = pr >> 16;
        flags &= ~2;
    }
    a = func_02033c44(res, e[0], e[1], frame);
    b = func_02033c44(res, e[2], e[3], frame);
    if (a == 0x1000 && b == 0x1000) {
        flags |= 1;
    } else {
        U32(out, 0x18) = a;
        U32(out, 0x1c) = b;
        flags &= ~1;
    }
    U32(out, 0) = flags;
}
void func_02033f50(u8 *obj, u8 *anm, u8 *mdl) {   // map joint animation entries to joints
    u32 i;
    u8 *mat;
    u32 n;
    if (mdl != 0 && U32(mdl, 8) != 0) mat = mdl + U32(mdl, 8);
    else mat = 0;
    U32(obj, 0xc) = (u32)data_020a5790;
    n = U8(mdl, 0x18);
    U8(obj, 0x19) = n;
    MIi_CpuClear16(0, obj + 0x1a, n * 2);
    for (i = 0; i < U8(anm, 9); i++) {
        u8 *name = DictName((Dict *)(anm + 8), i);
        s32 r = mat == 0 ? -1 : func_0203340c((Dict *)(mat + 4), name);
        if (r >= 0) ((u16 *)(obj + 0x1a))[r] = i | 0x100;
    }
}
void func_02034024(u8 *out, u8 *obj, u32 idx) {   // texture SRT animation for one material
    func_02033e50((u8 *)U32(obj, 8), (u16)idx, (s32)U32(obj, 0) >> 12, out);
    U32(out, 0x10) = (U32(out, 0x10) & ~0xc0000000) | 0x40000000;
    U32(out, 0) |= 8;
}
void func_02034068(u8 *obj, u8 *anm, u8 *mdl) {   // map texture SRT animation entries to materials
    u32 i;
    u8 *mat;
    u32 n;
    if (mdl != 0 && U32(mdl, 8) != 0) mat = mdl + U32(mdl, 8);
    else mat = 0;
    U32(obj, 0xc) = (u32)data_020a5794;
    n = U8(mdl, 0x18);
    U8(obj, 0x19) = n;
    U32(obj, 8) = (u32)anm;
    MIi_CpuClear16(0, obj + 0x1a, U8(obj, 0x19) * 2);
    for (i = 0; i < U8(anm, 0xd); i++) {
        u8 *name = DictName((Dict *)(anm + 0xc), i);
        s32 r = mat == 0 ? -1 : func_0203340c((Dict *)(mat + 4), name);
        if (r >= 0) ((u16 *)(obj + 0x1a))[r] = i | 0x100;
    }
}
void func_02034144(ResTex *tex, u8 *name, u8 *out) {   // texture pattern: texture image parameters and scale
    u32 *info = tex == 0 ? 0 : func_0203324c(&tex->texDict, (char *)name);
    u32 key;
    u32 w, h;
    u32 t;
    if ((info[0] & 0x1c000000) != 0x14000000) key = (u16)tex->texKey & ~0xe0000000;
    else key = (u16)tex->tex4x4Key & ~0xe0000000;
    t = U32(out, 0x10) & 0xc00f0000;
    U32(out, 0x10) = t;
    U32(out, 0x10) = t | (info[0] + key);
    U16(out, 0x2c) = info[1] & 0x7ff;
    U16(out, 0x2e) = (info[1] & (0x7ff << 11)) >> 11;
    w = info[1] & 0x7ff;
    h = (info[1] & (0x7ff << 11)) >> 11;
    if (w == U16(out, 0x2c)) U32(out, 0x30) = 0x1000;
    else U32(out, 0x30) = FX_Div(w << 12, U16(out, 0x2c) << 12);
    {
        s32 s;
        if (h == U16(out, 0x2e)) s = 0x1000;
        else s = FX_Div(h << 12, U16(out, 0x2e) << 12);
        U32(out, 0x34) = s;
    }
}
void func_0203421c(ResTex *tex, u8 *name, u8 *out) {   // palette pattern
    u16 *info = (tex == 0 || tex->plttDictOfs == 0) ? 0 : func_0203324c((Dict *)((u8 *)tex + tex->plttDictOfs), (char *)name);
    u16 key = (u16)tex->plttKey & ~0xe0000000;
    u16 off = info[0];
    if (!(info[1] & 1)) {
        off = off >> 1;
        key = key >> 1;
    }
    U32(out, 0x14) = off + key;
}
void func_02034284(u8 *out, u8 *obj, u32 idx) {   // evaluate a texture pattern animation for one material
    u8 *res = (u8 *)U32(obj, 8);
    u8 *e = func_02033714(res, (u16)idx, (U32(obj, 0) << 4) >> 16);
    func_02034144((ResTex *)U32(obj, 0x14), func_020336bc(res, U8(e, 2)), out);
    if (U8(e, 3) == 0xff) return;
    func_0203421c((ResTex *)U32(obj, 0x14), func_020336e8(res, U8(e, 3)), out);
}
void func_020342f8(u8 *obj, u8 *anm, u8 *mdl) {   // visibility animation object: every node is driven
    u32 i;
    U32(obj, 0xc) = (u32)data_020a5788;
    U8(obj, 0x19) = U8(mdl, 0x17);
    U32(obj, 8) = (u32)anm;
    for (i = 0; i < U8(obj, 0x19); i++) ((u16 *)(obj + 0x1a))[i] = i | 0x100;
}
void func_02034344(u32 *out, u8 *obj, u32 idx) {   // node visibility bit at the current frame
    u8 *res = (u8 *)U32(obj, 8);
    s32 frame = (s32)U32(obj, 0) >> 12;
    u32 bit = frame * U16(res, 6) + idx;
    *out = ((u32 *)(res + 0xc))[bit >> 5] & (1 << (bit & 0x1f));
}
void func_0203437c(u32 *m) {   // send the pending matrix commands (mode without scale)
    if (!(m[0] & 4)) {
        if (!(m[0] & 2)) func_01ff93d8(0x19, &m[10], 12);
        else func_01ff93d8(0x1c, &m[19], 3);
    } else {
        if (!(m[0] & 2)) func_01ff93d8(0x1a, &m[10], 9);
    }
    if (!(m[0] & 1)) func_01ff93d8(0x1b, &m[1], 3);
}
void func_020343f8(u32 *m, u32 *v, u32 unused, u32 flags) {   // set the translation of a node transform
    if (flags & 4) {
        m[0] |= 1;
    } else {
        m[1] = v[0];
        m[2] = v[1];
        m[3] = v[2];
    }
    m[0] |= 0x18;
}
void func_02034438(u32 *m) {   // send the pending matrix commands (with deferred scale)
    BOOL scale = 0;
    if (!(m[0] & 4)) scale = 1;
    if ((m[0] & 0x20) && !(m[0] & 8)) {
        if (scale) {
            func_01ff93d8(0x1c, &m[19], 3);
            scale = 0;
        }
        func_01ff93d8(0x1b, &m[4], 3);
    }
    if (!(m[0] & 2)) {
        if (scale) func_01ff93d8(0x19, &m[10], 12);
        else func_01ff93d8(0x1a, &m[10], 9);
    } else {
        if (scale) func_01ff93d8(0x1c, &m[19], 3);
    }
    if (!(m[0] & 1)) func_01ff93d8(0x1b, &m[1], 3);
}
typedef struct { u32 pad[0x31]; u32 bits[1]; } NodeCache;
extern NodeCache *data_020baf00;
typedef struct { s32 x, y, z; } TV3;
typedef struct { TV3 head; TV3 trans; } NodeTrans;
extern NodeTrans data_020bbd04[];
void func_020344fc(u32 *m, s32 *v, u8 *idx, u32 flags) {   // record a node translation in the cache
    u32 f = idx[3];
    if (flags & 4) {
        m[0] |= 1;
        if (f & 2) {
            u8 a = idx[1];
            data_020baf00->bits[(u32)a >> 5] |= 1 << (a & 0x1f);
        }
    } else {
        m[1] = v[0];
        m[2] = v[1];
        m[3] = v[2];
        if (f & 2) {
            u8 a = idx[1];
            data_020baf00->bits[(u32)a >> 5] &= ~(1 << (a & 0x1f));
            data_020bbd04[a].trans.x = v[3];
            data_020bbd04[a].trans.y = v[4];
            data_020bbd04[a].trans.z = v[5];
        }
    }
    if (f & 1) {
        u8 b = idx[2];
        m[0] |= 0x20;
        if (data_020baf00->bits[(u32)b >> 5] & (1 << (b & 0x1f))) {
            m[0] |= 8;
        } else {
            *(TV3 *)&m[4] = data_020bbd04[b].trans;
        }
    }
    m[0] |= 0x10;
}
void func_0203464c(s32 *out, s16 *p) {   // texture matrix from scale / rotation / translation
    u32 *pu = (u32 *)p;
    s32 w = *(u16 *)((u8 *)p + 0x2c) << 12;
    s32 h = *(u16 *)((u8 *)p + 0x2e) << 12;
    s32 sS, sT, s, c, r4, r5, r6, r7;
    FX_DivAsync(h, w);
    c = p[0x22 / 2];
    sS = *(s32 *)((u8 *)p + 0x18);
    s = p[0x20 / 2];
    sT = *(s32 *)((u8 *)p + 0x1c);
    r7 = FxMul(sS, s);
    r6 = FxMul(sS, c);
    r5 = FxMul(sT, s);
    r4 = FxMul(sT, c);
    out[0] = r6;
    out[0x14 / 4] = r4;
    out[4 / 4] = (-r5 * FX_GetDivResult()) >> 12;
    FX_DivAsync(w, h);
    {
        s32 a = r5 - r4;
        s32 b = r7 + r6;
        s32 sc = *(s32 *)((u8 *)p + 0x18);
        s32 tw = *(u16 *)((u8 *)p + 0x2c);
        s32 tS = (s32)(((long long)sc * *(s32 *)((u8 *)p + 0x24)) >> 8);
        out[0x30 / 4] = ((tw * (sc - b)) << 3) - tw * tS;
        {
            s32 sc2 = *(s32 *)((u8 *)p + 0x1c);
            s32 th = *(u16 *)((u8 *)p + 0x2e);
            s32 tT = (s32)(((long long)sc2 * *(s32 *)((u8 *)p + 0x28)) >> 8);
            out[0x34 / 4] = ((th * (a - sc2 + 0x2000u)) << 3) + th * tT;
        }
    }
    out[0x10 / 4] = (r7 * FX_GetDivResult()) >> 12;
}
void func_02034754(s32 *out, u8 *p) {   // texture matrix: rotation with scale 1
    s32 w = PU16(p, 0x2c) << 12;
    s32 h = PU16(p, 0x2e) << 12;
    FX_DivAsync(h, w);
    out[0] = PS16(p, 0x22);
    out[5] = PS16(p, 0x22);
    out[1] = (-PS16(p, 0x20) * FX_GetDivResult()) >> 12;
    FX_DivAsync(w, h);
    out[12] = (PU16(p, 0x2c) * (-(PS16(p, 0x20) + PS16(p, 0x22)) + 0x1000u) << 3) - ((P32(p, 0x24) * PU16(p, 0x2c)) << 4);
    out[13] = (PU16(p, 0x2e) * (PS16(p, 0x20) - PS16(p, 0x22) + 0x1000u) << 3) + ((P32(p, 0x28) * PU16(p, 0x2e)) << 4);
    out[4] = (PS16(p, 0x20) * FX_GetDivResult()) >> 12;
}
void func_02034824(s32 *out, u8 *p) {   // texture matrix: scale and translate
    out[0] = P32(p, 0x18);
    out[5] = P32(p, 0x1c);
    out[1] = 0;
    out[12] = -(s32)(((long long)P32(p, 0x18) * P32(p, 0x24)) >> 8) * PU16(p, 0x2c);
    out[13] = (s32)(((long long)P32(p, 0x1c) * P32(p, 0x28)) >> 8) * PU16(p, 0x2e) + ((PU16(p, 0x2e) * (P32(p, 0x1c) * -2 + 0x2000)) << 3);
    out[4] = 0;
}
void func_020348a0(s32 *out, u8 *p) {   // texture matrix: translate only
    out[0] = 0x1000;
    out[5] = 0x1000;
    out[1] = 0;
    out[12] = (-(P32(p, 0x24) * PU16(p, 0x2c))) << 4;
    out[13] = (P32(p, 0x28) * PU16(p, 0x2e)) << 4;
    out[4] = 0;
}
void func_020348e8(s32 *out, u8 *p) {
    s32 w = PU16(p, 0x2c) << 12;
    s32 h = PU16(p, 0x2e) << 12;
    s32 r4, r5, r6, r7, c, sS, s, sT;
    FX_DivAsync(h, w);
    c = PS16(p, 0x22);
    sS = P32(p, 0x18);
    s = PS16(p, 0x20);
    sT = P32(p, 0x1c);
    r7 = FxMul(sS, s);
    r6 = FxMul(sS, c);
    r5 = FxMul(sT, s);
    r4 = FxMul(sT, c);
    out[0] = r6;
    out[5] = r4;
    out[1] = (-r5 * FX_GetDivResult()) >> 12;
    FX_DivAsync(w, h);
    out[12] = (PU16(p, 0x2c) * (P32(p, 0x18) - (r7 + r6))) << 3;
    out[13] = (PU16(p, 0x2e) * (r5 - r4 - P32(p, 0x1c) + 0x2000u)) << 3;
    out[4] = (r7 * FX_GetDivResult()) >> 12;
}
void func_020349c8(s32 *out, u8 *p) {   // texture matrix: rotation only
    s32 w = PU16(p, 0x2c) << 12;
    s32 h = PU16(p, 0x2e) << 12;
    FX_DivAsync(h, w);
    out[0] = PS16(p, 0x22);
    out[5] = PS16(p, 0x22);
    out[1] = (-PS16(p, 0x20) * FX_GetDivResult()) >> 12;
    FX_DivAsync(w, h);
    out[12] = (PU16(p, 0x2c) * (-(PS16(p, 0x20) + PS16(p, 0x22)) + 0x1000u)) << 3;
    out[13] = (PU16(p, 0x2e) * (PS16(p, 0x20) - PS16(p, 0x22) + 0x1000u)) << 3;
    out[4] = (PS16(p, 0x20) * FX_GetDivResult()) >> 12;
}
void func_02034a80(s32 *out, u8 *p) {   // texture matrix: scale only
    s32 t;
    out[0] = P32(p, 0x18);
    out[5] = P32(p, 0x1c);
    out[1] = 0;
    out[12] = 0;
    out[13] = (PU16(p, 0x2e) * (-(P32(p, 0x1c) * 2) + 0x2000u)) << 3;
    out[4] = 0;
}
void func_02034ac4(s32 *out) {   // texture matrix: identity
    out[0] = 0x1000;
    out[1] = 0;
    out[4] = 0;
    out[5] = 0x1000;
    out[12] = 0;
    out[13] = 0;
}
void func_02034ae8(u32 *m) {   // load the texture matrix built from a texture SRT
    struct { u32 cmd; u32 mode; s32 mtx[16]; u32 end; } t;
    if (m[0] & 8) t.cmd = 0x101610;
    else t.cmd = 0x101810;
    t.mode = 3;
    t.end = 2;
    t.mtx[14] = 0;
    t.mtx[11] = 0;
    t.mtx[10] = 0;
    t.mtx[9] = 0;
    t.mtx[8] = 0;
    t.mtx[7] = 0;
    t.mtx[6] = 0;
    t.mtx[3] = 0;
    t.mtx[2] = 0;
    t.mtx[15] = 0x1000;
    data_020a5820[m[0] & 7](t.mtx, m);
    if (m[12] != 0x1000) {
        t.mtx[0] = FxMul(m[12], t.mtx[0]);
        t.mtx[1] = FxMul(m[12], t.mtx[1]);
        t.mtx[12] = FxMul(m[12], t.mtx[12]);
    }
    if (m[13] != 0x1000) {
        t.mtx[4] = FxMul(m[13], t.mtx[4]);
        t.mtx[5] = FxMul(m[13], t.mtx[5]);
        t.mtx[13] = FxMul(m[13], t.mtx[13]);
    }
    func_01ff93d8(t.cmd, (u32 *)&t + 1, 0x12);
}
void func_02034c30(u32 *m) {   // send the pending matrix commands (rotation / scale in one go)
    BOOL scale = 0;
    u32 deferred = m[0] & 0x18;
    s32 v[3];
    if (deferred == 0) func_01ff93d8(0x1b, &m[7], 3);
    if (!(m[0] & 4)) {
        if (deferred != 0) {
            scale = 1;
        } else {
            v[0] = FxMul(m[19], m[4]);
            v[1] = FxMul(m[20], m[5]);
            v[2] = FxMul(m[21], m[6]);
            func_01ff93d8(0x1c, v, 3);
        }
    }
    if (!(m[0] & 2)) {
        if (scale) func_01ff93d8(0x19, &m[10], 12);
        else func_01ff93d8(0x1a, &m[10], 9);
    } else {
        if (scale) func_01ff93d8(0x1c, &m[19], 3);
    }
    if (deferred == 0) func_01ff93d8(0x1b, &m[4], 3);
    if (!(m[0] & 1)) func_01ff93d8(0x1b, &m[1], 3);
}
