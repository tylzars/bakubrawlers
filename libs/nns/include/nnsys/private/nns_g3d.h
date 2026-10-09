#ifndef NNS_G3D_H
#define NNS_G3D_H
#include <nnsys/private/nns_types.h>

// Resource dictionary header and lookup (inlined at every use in the binary).
typedef struct { u8 rev; volatile u8 num; u16 byteSize; u16 pad04; u16 tableOfs; } Dict;

static inline void *DictEntry(Dict *dict, u32 idx) {
    if (dict != 0 && idx < dict->num) {
        u8 *e = (u8 *)dict + dict->tableOfs;
        return e + 4 + *(u16 *)e * idx;
    }
    return 0;
}

static inline u8 *DictName(Dict *d, u32 idx) {
    if (d != 0 && idx < d->num) {
        u8 *e = (u8 *)d + d->tableOfs;
        return e + *(u16 *)(e + 2) + idx * 16;
    }
    return 0;
}

// Material block (NNSG3dResMat): two offsets to the texture / palette to material lists, then the material dict.
typedef struct { u16 texListOfs; u16 plttListOfs; Dict dict; } ResMat;
// Material index list for one texture or palette name.
typedef struct { u16 idxOfs; u8 count; u8 flag; } MatList;
// Material data (parts used here).
typedef struct {
    u8 pad0[0x14]; u32 texParam; u16 pad18; u16 pad1a; u16 plttBase; u16 pad1e;
    u16 texW; u16 texH; s32 scaleS; s32 scaleT;
} MatData;
// Model (NNSG3dResMdl): block header, then offsets
typedef struct { u32 kind; u32 size; u32 ofsMat; } ResMdl;
typedef struct { u32 kind; u32 size; Dict dict; } ResMdlSet;

typedef struct {
    u32 magic; u32 size;
    u32 texKey; u16 texSize; u16 texDictOfs; u16 texLoaded; u16 pad12; u32 texOfs;
    u32 tex4x4Key; u16 tex4x4Size; u16 tex4x4DictOfs; u16 tex4x4Loaded; u16 pad22; u32 tex4x4Ofs; u32 tex4x4IdxOfs;
    u32 plttKey; u16 plttSize; u16 plttLoaded; u16 plttDictOfs; u16 pad36; u32 plttOfs;
    Dict texDict;   // 0x3c
} ResTex;

typedef struct AnmObj {
    s32 frame; s32 weight; void *res; u32 f0c; struct AnmObj *next; void *tex;
    u8 prio; u8 mapCount; u16 map[1];
} AnmObj;

extern void GX_BeginLoadTex(void);      // GX_BeginLoadTex
extern void GX_LoadTex(const void *src, u32 offset, u32 size);   // GX_LoadTex
extern void GX_EndLoadTex(void);      // GX_EndLoadTex
extern void GX_BeginLoadTexPltt(void);      // GX_BeginLoadTexPltt
extern void GX_LoadTexPltt(const void *src, u32 offset, u32 size);  // GX_LoadTexPltt
extern void GX_EndLoadTexPltt(void);      // GX_EndLoadTexPltt
extern s32 FX_Div(s32 a, s32 b);   // FX_Div
extern void *func_0203324c(Dict *dict, const char *name);
#endif
