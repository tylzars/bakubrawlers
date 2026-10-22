// NitroSystem G3d: global state (G3dGlb), matrix helpers, resource setup. 0x02032130-0x02033574
#include <nnsys/private/nns_g3d.h>
#define U16(p, o) (*(u16 *)((u8 *)(p) + (o)))
#define U32(p, o) (*(u32 *)((u8 *)(p) + (o)))
#define U8(p, o) (*((u8 *)(p) + (o)))
#define P32(p, o) (*(s32 *)((u8 *)(p) + (o)))
extern void SendFifoCommand(u32 cmd, const void *params, u32 count);
extern void MIi_CpuCopy32(const void *src, void *dst, u32 size);
extern void MTX_Identity43_(void *m43);      // MTX_Identity43
extern void MTX_Identity44_(void *m44);      // MTX_Identity44
extern void MTX_Identity33_(void *m33);      // MTX_Identity33
extern void MTX_Inverse43(void *src, void *dst);   // MTX_Inverse43
extern void MI_Copy64B(const void *src, void *dst);   // copy 4x4
extern void MTX_Copy43To44(const void *src, void *dst);   // 4x3 to 4x4
extern void MTX_Concat44(const void *a, const void *b, void *dst);   // MTX_Concat44
extern void MTX_Concat43(void *a, void *b, void *dst);   // MTX_Concat43
extern void MTX_ApplyScale43(void *src, void *dst, s32 x, s32 y, s32 z);   // scale
extern long long FX_InvFx64c(s32 x);   // FX_Inv (64 bit)
extern s32 FX_Div(s32 a, s32 b);
extern void MIi_CpuClearFast(u32 data, void *dst, u32 size);
extern void VEC_CrossProduct(void *a, void *b, void *dst);   // VEC_CrossProduct
extern void VEC_Normalize(void *src, void *dst);   // VEC_Normalize
static inline s32 FxMul(s32 a, s32 b) { return (s32)(((long long)a * b) >> 12); }

// G3d global state (G3dGlb): one struct, which every access fits; the base translation/scale and the dirty flags sit at 0xe0/0xec/0xfc.
typedef struct { s32 x, y, z; } VecFx32;
typedef union { u32 w[0x99]; struct { u32 pad0[0x38]; VecFx32 trans; VecFx32 scale; u32 pad; u32 flag; } s; } G3dGlb;
extern G3dGlb data_02211374;
extern G3dGlb data_02211374_b;   // same address as data_02211374: the second literal-pool word of func_02032284
#define GU (data_02211374.w)
extern u8 data_0221137c[], data_022113c0[], data_02211430[], data_02211474[], data_022114a4[], data_022114d4[], data_02211534[], data_02211574[];
extern u32 data_022113f4[], data_0221141c[];

void InitG3dGlb(void) {
    u32 *g = GU;
    g[0] = 0x17101610;
    g[1] = 0;
    g[0x48 / 4] = 2;
    g[0x7c / 4] = 0x32323232;
    g[0x90 / 4] = 0x60293130;
    g[0xa4 / 4] = 0x33333333;
    g[0xb8 / 4] = 0x2a1b19;
    MTX_Identity43_(data_022113c0);
    MTX_Identity44_(data_0221137c);
    g[0x80 / 4] = 0x2d8b62d8;
    g[0x84 / 4] = 0x40000200;
    g[0x88 / 4] = 0x40000200 - 0xc0000001;
    g[0x8c / 4] = 0xc0080000;
    g[0x94 / 4] = 0x4210c210;
    g[0x98 / 4] = 0x4210c210;
    g[0x9c / 4] = 0x1f008f;
    g[0xa0 / 4] = 0xc0080000 - 0x90000;
    g[0xa8 / 4] = 0x7fff;
    g[0xac / 4] = 0x4000001f;
    g[0xb0 / 4] = 0x800003e0;
    g[0xb4 / 4] = 0x7e00 - 0x40000200;
    g[0xe0 / 4] = 0;
    g[0xe4 / 4] = 0;
    g[0xe8 / 4] = 0;
    MTX_Identity33_(data_02211430);
    g[0xec / 4] = 0x1000;
    g[0xf0 / 4] = 0x1000;
    g[0xf4 / 4] = 0x1000;
    g[0xf8 / 4] = 0;
    g[0xfc / 4] = 0;
    g[0x248 / 4] = 0;
    g[0x244 / 4] = 0;
    g[0x240 / 4] = 0;
    g[0x254 / 4] = 0;
    g[0x24c / 4] = 0;
    g[0x250 / 4] = 0x1000;
    g[0x25c / 4] = 0;
    g[0x258 / 4] = 0;
    g[0x260 / 4] = -0x1000;
}
void func_02032284(void) {
    SendFifoCommand(GU[0], (u32 *)&data_02211374_b + 1, 0x3e);
    GU[0xfc / 4] &= ~1;
    GU[0xfc / 4] &= ~2;
}
void SetG3dGlbTrans(const VecFx32 *trans) {
    if (trans == 0) return;
    data_02211374.s.trans = *trans;
    data_02211374.s.flag &= ~0xa4;
}
void SetG3dGlbScale(const VecFx32 *scale) {
    if (scale == 0) return;
    data_02211374.s.scale = *scale;
    data_02211374.s.flag &= ~0xa4;
}
void func_02032320(u32 idx, s32 x, s32 y, s32 z) {
    s32 vx = x >> 3;
    s32 vy = y >> 3;
    s32 vz = z >> 3;
    data_022113f4[idx] = (vx & 0x3ff) | ((vy & 0x3ff) << 10) | ((vz & 0x3ff) << 20) | (idx << 30);
}
void func_0203235c(u32 idx, u32 color) {
    data_0221141c[idx] = color | (idx << 30);
}
void func_02032370(u32 diffuse, u32 ambient, BOOL setVtx) {
    GU[0x94 / 4] = diffuse | (ambient << 16) | ((setVtx != 0) << 15);
}
void func_02032394(u32 specular, u32 emission, BOOL shininess) {
    GU[0x98 / 4] = specular | (emission << 16) | ((shininess != 0) << 15);
}
void *func_020323b8(void) {   // inverse of the current position matrix (cached)
    if (!(GU[0xfc / 4] & 8)) {
        MTX_Inverse43(data_022113c0, data_02211474);
        GU[0xfc / 4] |= 8;
    }
    return data_02211474;
}
s32 InvertMtx44(s32 *src, s32 *dstp) {   // MTX_Inverse44 (Gauss-Jordan, fixed point)
    s32 tmp[4][4];
    s32 (*dst)[4] = (s32 (*)[4]) dstp;
    s32 piv, i, j, k, best;
    MI_Copy64B(src, (s32 *) tmp);
    MTX_Identity44_(dstp);
    for (i = 0; i < 4; i++) {
        long long inv;
        piv = i;
        best = 0;
        for (j = i; j < 4; j++) {
            s32 v = tmp[j][i];
            if (v < 0) v = -v;
            if (v > best) { piv = j; best = v; }
        }
        if (best == 0) return -1;
        if (piv != i) {
            for (k = 0; k < 4; k++) {
                s32 t = tmp[i][k]; tmp[i][k] = tmp[piv][k]; tmp[piv][k] = t;
                t = dst[i][k]; dst[i][k] = dst[piv][k]; dst[piv][k] = t;
            }
        }
        inv = FX_InvFx64c(tmp[i][i]);
        for (k = 0; k < 4; k++) {
            tmp[i][k] = (s32)(((long long)tmp[i][k] * inv + 0x80000000LL) >> 32);
            dst[i][k] = (s32)(((long long)dst[i][k] * inv + 0x80000000LL) >> 32);
        }
        for (j = 0; j < 4; j++) {
            if (j != i) {
                s32 f = tmp[j][i];
                for (k = 0; k < 4; k++) {
                    tmp[j][k] -= FxMul(f, tmp[i][k]);
                    dst[j][k] -= FxMul(f, dst[i][k]);
                }
            }
        }
    }
    return 0;
}
void *func_02032604(void) {   // inverse of the projection matrix (cached)
    if (!(GU[0xfc / 4] & 0x10)) {
        InvertMtx44((s32 *)data_0221137c, (s32 *)data_02211534);
        GU[0xfc / 4] |= 0x10;
    }
    return data_02211534;
}
void func_02032648(void) {   // base matrix = position * base scale, and its inverse
    MTX_Concat43(data_02211430, data_022113c0, data_022114a4);
    MTX_ApplyScale43(data_022114a4, data_022114a4, GU[0xec / 4], GU[0xf0 / 4], GU[0xf4 / 4]);
    MTX_Inverse43(data_022114a4, data_022114d4);
}
void *func_020326a0(void) {
    if (!(GU[0xfc / 4] & 0x80)) {
        func_02032648();
        GU[0xfc / 4] |= 0x80;
    }
    return data_022114a4;
}
void *func_020326d8(void) {
    if (!(GU[0xfc / 4] & 0x80)) {
        func_02032648();
        GU[0xfc / 4] |= 0x80;
    }
    return data_022114d4;
}
void *func_02032710(void) {   // combined position and projection matrix (cached)
    u8 tmp[0x40];
    if (!(GU[0xfc / 4] & 0x40)) {
        void *a = func_020323b8();
        void *b = func_02032604();
        MTX_Copy43To44(a, tmp);
        MTX_Concat44(b, tmp, data_02211574);
        GU[0xfc / 4] |= 0x40;
    }
    return data_02211574;
}
BOOL func_02032778(void *out, AnmObj *o, u32 idx) {   // runs an animation list's callbacks; installed at RObj+0xc,
                                                       // which the material handler calls per material
    BOOL r = 0;
    if (o == 0) return 0;
    do {
        if (idx < o->mapCount && (o->map[idx] & 0x300) == 0x100) {
            void (*cb)(void *, AnmObj *, u32) = (void (*)(void *, AnmObj *, u32))o->f0c;
            if (cb != 0) {
                cb(out, o, o->map[idx] & 0xff);
                r = 1;
            }
        }
        o = o->next;
    } while (o != 0);
    return r;
}
void AddWeightedVec(s32 *v, s32 *w, s32 ratio, u32 flag) {   // accumulate a weighted vector
    if (flag != 0) {
        v[0] += ratio;
        v[1] += ratio;
        v[2] += ratio;
    } else {
        v[0] += (ratio * w[0]) >> 12;
        v[1] += (ratio * w[1]) >> 12;
        v[2] += (ratio * w[2]) >> 12;
    }
}

typedef struct { u32 flags; s32 trans[3]; s32 pivot[3]; s32 pivot2[3]; s32 rot[9]; s32 rot2[3]; s32 scale[3]; } JntResult;
BOOL func_02032854(u8 *out, AnmObj *list, u32 idx) {   // blend the joint animations of a node
    u8 tmp[0x58];
    u8 keep[0x18];
    AnmObj *o;
    s32 total;
    s32 count;
    AnmObj *last;
    s32 n;
    s32 w;
    if (list == 0) return 0;
    if (list->next == 0) {
        if (idx < list->mapCount) {
            if ((list->map[idx] & 0x300) == 0x100) {
                void (*cb)(void *, AnmObj *, u32) = (void (*)(void *, AnmObj *, u32))list->f0c;
                if (cb == 0) return 0;
                cb(out, list, list->map[idx] & 0xff);
                return 1;
            }
            return 0;
        }
        return 0;
    }
    total = 0;
    count = 0;
    n = 0;
    o = list;
    do {
        if (idx < o->mapCount && (o->map[idx] & 0x300) == 0x100) {
            if (o->weight > 0x1000) total += 0x1000;
            else if (o->weight > 0) total += o->weight;
            last = o;
            count++;
        }
        o = o->next;
    } while (o != 0);
    if (total == 0) return 0;
    if (count == 1) {
        void (*cb)(void *, AnmObj *, u32) = (void (*)(void *, AnmObj *, u32))last->f0c;
        u32 mi = last->map[idx];
        if (cb == 0) return 0;
        cb(out, last, mi & 0xff);
        return 1;
    }
    MIi_CpuClearFast(0, out, 0x58);
    U32(out, 0) = -1;
    do {
        if (idx < list->mapCount && (list->map[idx] & 0x300) == 0x100 && list->weight > 0 && list->f0c != 0) {
            ((void (*)(void *, AnmObj *, u32))list->f0c)(tmp, list, list->map[idx] & 0xff);
            if (n == 0) {
                MIi_CpuCopy32(tmp + 0x28, keep + 0xc, 12);
                MIi_CpuCopy32(tmp + 0x40, keep, 12);
            }
            if (total == 0x1000) w = list->weight;
            else w = FX_Div(list->weight, total);
            AddWeightedVec((s32 *)(out + 4), (s32 *)(tmp + 4), w, U32(tmp, 0) & 1);
            AddWeightedVec((s32 *)(out + 0x10), (s32 *)(tmp + 0x10), w, U32(tmp, 0) & 8);
            AddWeightedVec((s32 *)(out + 0x1c), (s32 *)(tmp + 0x1c), w, U32(tmp, 0) & 0x10);
            if (!(U32(tmp, 0) & 4)) {
                P32(out, 0x4c) += FxMul(w, P32(tmp, 0x4c));
                P32(out, 0x50) += FxMul(w, P32(tmp, 0x50));
                P32(out, 0x54) += FxMul(w, P32(tmp, 0x54));
            }
            if (!(U32(tmp, 0) & 2)) {
                P32(out, 0x28) += (w * P32(tmp, 0x28)) >> 12;
                P32(out, 0x2c) += (w * P32(tmp, 0x2c)) >> 12;
                P32(out, 0x30) += (w * P32(tmp, 0x30)) >> 12;
                P32(out, 0x34) += (w * P32(tmp, 0x34)) >> 12;
                P32(out, 0x38) += (w * P32(tmp, 0x38)) >> 12;
                P32(out, 0x3c) += (w * P32(tmp, 0x3c)) >> 12;
            } else {
                P32(out, 0x28) += w;
                P32(out, 0x38) += w;
            }
            U32(out, 0) &= U32(tmp, 0);
        }
        list = list->next;
        n++;
    } while (list != 0);
    VEC_CrossProduct(out + 0x28, out + 0x34, out + 0x40);
    if (P32(out, 0x28) == 0 && P32(out, 0x2c) == 0 && P32(out, 0x30) == 0) MIi_CpuCopy32(keep + 0xc, out + 0x28, 12);
    else VEC_Normalize(out + 0x28, out + 0x28);
    if (P32(out, 0x40) == 0 && P32(out, 0x44) == 0 && P32(out, 0x48) == 0) MIi_CpuCopy32(keep, out + 0x40, 12);
    else VEC_Normalize(out + 0x40, out + 0x40);
    VEC_CrossProduct(out + 0x40, out + 0x28, out + 0x34);
    return 1;
}
BOOL func_02032c40(u32 *out, AnmObj *o, u32 idx) {   // OR together the flags of all animations driving a node
    BOOL r = 0;
    u32 t;
    *out = 0;
    do {
        if (idx < o->mapCount && (o->map[idx] & 0x300) == 0x100 && o->f0c != 0) {
            ((void (*)(void *, AnmObj *, u32))o->f0c)(&t, o, o->map[idx] & 0xff);
            *out |= t;
            r = 1;
        }
        o = o->next;
    } while (o != 0);
    return r;
}
extern s32 G3X_GetClipMtx(void *p);
extern void MTX_Copy44To43(void *a, void *b);
extern s32 G3X_GetVectorMtx(void *p);
extern void FlushFifoCommandBuffer(void);
// Sends the buffered commands (FlushFifoCommandBuffer), then reads the clip matrix into a (as 4x3) and the vector
// matrix into b, each only when non-NULL.
void func_02032cc8(void *a, void *b) {
    u8 buf[0x40];
    FlushFifoCommandBuffer();
    *(volatile u32 *)0x4000440 = 0;
    *(volatile u32 *)0x4000444 = 0;
    *(volatile u32 *)0x4000454 = 0;
    if (a != 0) {
        while (G3X_GetClipMtx(buf) != 0) {}
        MTX_Copy44To43(buf, a);
    }
    if (b != 0) {
        while (G3X_GetVectorMtx(b) != 0) {}
    }
    *(volatile u32 *)0x4000448 = 1;
    *(volatile u32 *)0x4000440 = 2;
}
extern void G3X_Init(void);
void InitG3d(void) {
    u32 v;
    G3X_Init();
    InitG3dGlb();
    v = *(volatile u32 *)0x4000600;
    *(volatile u32 *)0x4000600 = (v & ~0xc0000000) | 0x80000000;
}

extern u32 GetTexSize(ResTex *t);
extern u32 GetTex4x4Size(ResTex *t);
extern u32 GetPlttSize(ResTex *t);
extern void SetTexKeys(ResTex *t, u32 key, u32 key4x4);
extern void SetPlttKey(ResTex *t, u32 key);
extern void LoadResTex(ResTex *t, BOOL begin);
extern void LoadResPltt(ResTex *t, BOOL begin);
extern void TakeTexKeys(ResTex *t, u32 *key, u32 *key4x4);
extern u32 TakePlttKey(ResTex *t);
extern BOOL ApplyResTexToMdlSet(ResMdlSet *set, ResTex *t);
extern void RemoveResTexFromMdlSet(ResMdlSet *set);
extern void *GetResMdlSet(void *file);
extern void *GetResTex(void *file);
extern u32 (*data_020a5880)(u32 size, int is4x4, u32 opt);
extern int (*data_020a5884)(u32 key);
extern u32 (*data_020a5888)(u32 size, int is4pltt, u32 opt);
extern int (*data_020a588c)(u32 key);

static inline u8 *MatByIdx2(ResMat *mat, u32 idx) {
    if (mat != 0) {
        u32 *e = DictEntry(&mat->dict, idx);
        if (e != 0) return (u8 *)mat + *e;
    }
    return 0;
}
BOOL SetupResFile(void *file) {   // allocate VRAM, upload textures, bind
    u32 magic = U32(file, 0);
    ResTex *tex;
    BOOL ok1;
    BOOL ok2;
    u32 sz4x4;
    u32 szP;
    u32 key;
    BOOL ok3;
    u32 key4x4;
    u32 keyP;
    u32 sz;
    switch (magic) {
    case 0x30414342: case 0x30414d42: case 0x30415642: case 0x30415442: case 0x30505442:
        goto yes;
    case 0x30444d42: case 0x30585442:
        break;
    default:
        goto no;
    }
    ok1 = 1;
    ok2 = 1;
    ok3 = 1;
    tex = GetResTex(file);
    if (tex != 0) {
        sz = GetTexSize(tex);
        sz4x4 = GetTex4x4Size(tex);
        szP = GetPlttSize(tex);
        if (sz != 0) {
            key = data_020a5880(sz, 0, 0);
            if (key == 0) ok1 = 0;
        } else {
            key = 0;
        }
        if (sz4x4 != 0) {
            key4x4 = data_020a5880(sz4x4, 1, 0);
            if (key4x4 == 0) ok2 = 0;
        } else {
            key4x4 = 0;
        }
        if (szP != 0) {
            keyP = data_020a5888(szP, tex->tex4x4Loaded & 0x8000, 0);
            if (keyP == 0) ok3 = 0;
        } else {
            keyP = 0;
        }
        if (!(ok1 && ok2 && ok3)) {
            if (ok3) data_020a588c(keyP);
            if (ok2) data_020a5884(key4x4);
            if (ok1) data_020a5884(key);
            return 0;
        }
        SetTexKeys(tex, key, key4x4);
        SetPlttKey(tex, keyP);
        LoadResTex(tex, 1);
        LoadResPltt(tex, 1);
    }
    if (U32(file, 0) == 0x30444d42) {
        ResMdlSet *set = GetResMdlSet(file);
        if (tex != 0) ApplyResTexToMdlSet(set, tex);
    }
    return 1;
yes:
    return 1;
no:
    return 0;
}
void ReleaseResFile(void *file) {   // release what SetupResFile set up
    u32 magic = U32(file, 0);
    ResTex *tex;
    u32 keyP, key, key4x4;
    switch (magic) {
    case 0x30414342: case 0x30414d42: case 0x30415642: case 0x30415442: case 0x30505442:
        return;
    case 0x30444d42:
        {
            ResMdlSet *set = GetResMdlSet(file);
            if (GetResTex(file) != 0) RemoveResTexFromMdlSet(set);
        }
        break;
    case 0x30585442:
        break;
    default:
        return;
    }
    tex = GetResTex(file);
    if (tex == 0) return;
    keyP = TakePlttKey(tex);
    TakeTexKeys(tex, &key, &key4x4);
    if (keyP != 0) data_020a588c(keyP);
    if (key4x4 != 0) data_020a5884(key4x4);
    if (key != 0) data_020a5884(key);
}
void func_020330fc(u8 *mdl, BOOL set, u32 mask) {   // set or clear bits in every material's 16-bit flags
    u32 n = U8(mdl, 0x18);
    u32 i;
    ResMat *mat;
    if (mdl != 0 && U32(mdl, 8) != 0) mat = (ResMat *)(mdl + U32(mdl, 8)); else mat = 0;
    for (i = 0; i < n; i++) {
        u16 *m = (u16 *)MatByIdx2(mat, i);
        if (set) m[0x1e / 2] |= mask;
        else m[0x1e / 2] &= ~mask;
    }
}
void func_020331a4(u8 *mdl, BOOL set, u32 mask) {   // set or clear bits in every material's 32-bit flags
    u32 n = U8(mdl, 0x18);
    u32 i;
    ResMat *mat;
    if (mdl != 0 && U32(mdl, 8) != 0) mat = (ResMat *)(mdl + U32(mdl, 8)); else mat = 0;
    for (i = 0; i < n; i++) {
        u32 *m = (u32 *)MatByIdx2(mat, i);
        u32 v = m[0x10 / 4];
        if (set) v |= mask;
        else v &= ~mask;
        m[0x10 / 4] = v;
    }
}

// Lookup of a 16-byte name in a resource dictionary (linear for small dictionaries, Patricia tree otherwise).
static inline BOOL NameEq(const u32 *a, const u32 *b) {
    return a[0] == b[0] && a[1] == b[1] && a[2] == b[2] && a[3] == b[3];
}
void *FindDictEntryByName(Dict *dictp, const char *namep) {   // data pointer by name, or NULL
    u8 *dict = (u8 *)dictp;
    const u32 *name = (const u32 *)namep;
    u8 n;
    u32 i;
    if (name == 0) return 0;
    n = U8(dict, 1);
    if (n < 16) {
        u32 n0 = name[0], n1 = name[1], n2 = name[2], n3 = name[3];
        for (i = 0; i < U8(dict, 1); i++) {
            u32 *e4 = (u32 *)DictName((Dict *)dict, i);
            if (e4[0] == n0 && e4[1] == n1 && e4[2] == n2 && e4[3] == n3) {
                if (dict != 0 && i < n) {
                    u8 *e = dict + *(u16 *)(dict + 6);
                    return e + 4 + *(u16 *)e * i;
                }
                return 0;
            }
        }
    } else {
        u8 *t = dict + 8;
        u8 *node;
        u8 *prev;
        u8 ref;
        u32 idx;
        u8 *ent;
        if (U8(t, 1) != 0) {
            node = t + U8(t, 1) * 4;
            ref = U8(node, 0);
            prev = t;
            while (U8(prev, 0) > ref) {
                u32 bit = (name[ref >> 5] >> (ref & 0x1f)) & 1;
                u32 next = U8(node, bit + 1);
                prev = node;
                node = t + next * 4;
                ref = U8(node, 0);
            }
            idx = U8(node, 3);
            if (dict != 0 && idx < n) {
                u8 *e = dict + *(u16 *)(dict + 6);
                ent = e + *(u16 *)(e + 2) + idx * 16;
            } else {
                ent = 0;
            }
            if (NameEq((u32 *)ent, name)) {
                if (dict != 0 && idx < n) {
                    u8 *e = dict + *(u16 *)(dict + 6);
                    return e + 4 + *(u16 *)e * idx;
                }
                return 0;
            }
        }
    }
    return 0;
}
s32 FindDictIndexByName(Dict *dictp, const u8 *namep) {   // index by name, or -1
    u8 *dict = (u8 *)dictp;
    const u32 *name = (const u32 *)namep;
    u8 n;
    u32 i;
    if (name == 0) return -1;
    n = U8(dict, 1);
    if (n < 16) {
        u32 n0 = name[0], n1 = name[1], n2 = name[2], n3 = name[3];
        for (i = 0; i < U8(dict, 1); i++) {
            u32 *e4 = (u32 *)DictName((Dict *)dict, i);
            if (e4[0] == n0 && e4[1] == n1 && e4[2] == n2 && e4[3] == n3) return i;
        }
    } else {
        u8 *t = dict + 8;
        u8 *node;
        u8 *prev;
        u8 ref;
        u8 *ent;
        if (U8(t, 1) != 0) {
            node = t + U8(t, 1) * 4;
            ref = U8(node, 0);
            prev = t;
            while (U8(prev, 0) > ref) {
                u32 bit = (name[ref >> 5] >> (ref & 0x1f)) & 1;
                u32 next = U8(node, bit + 1);
                prev = node;
                node = t + next * 4;
                ref = U8(node, 0);
            }
            i = U8(node, 3);
            if (dict != 0 && i < n) {
                u8 *e = dict + *(u16 *)(dict + 6);
                ent = e + *(u16 *)(e + 2) + i * 16;
            } else {
                ent = 0;
            }
            if (NameEq((u32 *)ent, name)) return i;
        }
    }
    return -1;
}
