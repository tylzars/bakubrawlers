// NitroSystem G3d: animation objects, render objects, texture / material binding. 0x02031418-0x02032130
#include <nnsys/private/nns_g3d.h>

u32 func_02031418(u8 *anm, u8 *mdl) {   // NNS_G3dAnmObjCalcSizeRequired
    if (anm == 0 || mdl == 0) return 0;
    switch (*anm) {
    case 'M': return (mdl[0x18] * 2 + 0x1c) & ~3;
    case 'J': case 'V': return (mdl[0x17] * 2 + 0x1c) & ~3;
    default: return 0;
    }
}

typedef struct { u8 category; u8 pad; u16 revision; u32 pad4; } AnmInitKey;
typedef struct { void (*init)(AnmObj *, void *, void *); u32 pad; } AnmInitFn;
extern u32 data_020a5784;
extern AnmInitKey data_020a57a8[];
extern AnmInitFn data_020a57ac[];

void func_02031478(AnmObj *o, u8 *anm, void *mdl, void *tex) {   // NNS_G3dAnmObjInit
    u32 i;
    o->frame = 0;
    o->res = anm;
    o->next = 0;
    o->prio = 0x7f;
    o->weight = 0x1000;
    o->tex = tex;
    o->mapCount = 0;
    o->f0c = 0;
    for (i = 0; i < data_020a5784; i++) {
        if (anm[0] == data_020a57a8[i].category && *(u16 *)(anm + 2) == data_020a57a8[i].revision) {
            if (data_020a57ac[i].init != 0) data_020a57ac[i].init(o, anm, mdl);
            return;
        }
    }
}
void func_02031518(AnmObj *o, s32 id) {   // NNS_G3dAnmObjDisableID
    if (id < 0 || id >= o->mapCount) return;
    if (o->map[id] & 0x100) o->map[id] &= ~0x200;
}

typedef struct {
    u32 flags; void *model; AnmObj *matAnims; u32 pad0c; AnmObj *jointAnims; u32 pad14; AnmObj *visAnims; u32 pad1c;
    void *cb; u8 cbNode; u8 cbStage;
} RenderObj;
extern void *data_020a57a4, *data_020a57a0, *data_020a579c;
void func_02031554(u32 *r, void *mdl) {   // NNS_G3dRenderObjInit
    MIi_CpuClear32(0, r, 0x54);
    r[3] = (u32)data_020a57a4;
    r[5] = (u32)data_020a57a0;
    r[7] = (u32)data_020a579c;
    r[1] = (u32)mdl;
}
void func_020315a8(AnmObj **head, AnmObj *n) {   // insert into the priority-ordered animation list
    AnmObj *cur = *head;
    AnmObj *nx;
    if (cur == 0) { *head = n; return; }
    nx = cur->next;
    if (nx == 0) {
        if (cur->prio > n->prio) {
            AnmObj *t = n;
            while (t->next != 0) t = t->next;
            t->next = cur;
            *head = n;
        } else {
            cur->next = n;
        }
        return;
    }
    while (nx != 0) {
        if (nx->prio >= n->prio) {
            AnmObj *t = n;
            while (t->next != 0) t = t->next;
            cur->next = n;
            t->next = nx;
            return;
        }
        cur = nx;
        nx = nx->next;
    }
    cur->next = n;
}
void func_02031660(u32 *bits, AnmObj *list) {   // collect the ids driven by a list of animations
    if (list == 0) return;
    do {
        s32 i;
        for (i = 0; i < list->mapCount; i++) {
            if (list->map[i] & 0x100) bits[i >> 5] |= 1 << (i & 0x1f);
        }
        list = list->next;
    } while (list != 0);
}
void func_020316c8(u8 *r, AnmObj *a) {   // NNS_G3dRenderObjAddAnmObj
    if (a == 0 || a->res == 0) return;
    switch (*(u8 *)a->res) {
    case 'M':
        func_02031660((u32 *)(r + 0x3c), a);
        func_020315a8((AnmObj **)(r + 0x8), a);
        break;
    case 'J':
        func_02031660((u32 *)(r + 0x44), a);
        func_020315a8((AnmObj **)(r + 0x10), a);
        break;
    case 'V':
        func_02031660((u32 *)(r + 0x4c), a);
        func_020315a8((AnmObj **)(r + 0x18), a);
        break;
    }
}
BOOL func_02031748(AnmObj **head, AnmObj *n) {   // unlink from an animation list
    AnmObj *nx;
    AnmObj *cur;
    cur = *head;
    if (cur == 0) return 0;
    if (cur == n) {
        *head = cur->next;
        n->next = 0;
        return 1;
    }
    nx = cur->next;
    while (nx != 0) {
        if (nx == n) {
            cur->next = nx->next;
            nx->next = 0;
            return 1;
        }
        cur = nx;
        nx = nx->next;
    }
    return 0;
}
void func_020317bc(u8 *r, AnmObj *a) {   // NNS_G3dRenderObjRemoveAnmObj
    if (!func_02031748((AnmObj **)(r + 0x8), a) && !func_02031748((AnmObj **)(r + 0x10), a)
        && !func_02031748((AnmObj **)(r + 0x18), a)) return;
    *(u32 *)r |= 0x10;
}
void func_02031810(u8 *r, void *cb, u32 unused, u32 node, u32 timing) {   // NNS_G3dRenderObjSetCallBack
    *(void **)(r + 0x20) = cb;
    r[0x24] = node;
    r[0x25] = timing;
}
void func_02031824(u8 *r) {   // NNS_G3dRenderObjResetCallBack
    *(void **)(r + 0x20) = 0;
    r[0x24] = 0;
    r[0x25] = 0;
}
void func_02031838(u8 *r, void *p) { *(void **)(r + 0x28) = p; }
u32 func_02031840(ResTex *t) { return t ? t->texSize << 3 : 0; }
u32 func_02031854(ResTex *t) { return t ? t->tex4x4Size << 3 : 0; }
void func_02031868(ResTex *t, u32 key, u32 key4x4) {   // NNS_G3dTexSetTexKey
    if (key != 0) t->texKey = key;
    if (key4x4 != 0) t->tex4x4Key = key4x4;
}
void func_0203187c(ResTex *t, BOOL begin) {   // NNS_G3dTexLoad
    if (begin) GX_BeginLoadTex();
    if (t->texSize << 3) {
        GX_LoadTex((u8 *)t + t->texOfs, (u16)t->texKey << 3, t->texSize << 3);
        t->texLoaded |= 1;
    }
    if (t->tex4x4Size << 3) {
        u32 key;
        u32 off;
        u32 idx;
        u32 size;
        size = t->tex4x4Size << 3;
        key = t->tex4x4Key << 16;
        off = key >> 13;
        idx = t->tex4x4IdxOfs;
        GX_LoadTex((u8 *)t + t->tex4x4Ofs, off, size);
        GX_LoadTex((u8 *)t + idx, (((0x1ffff & (key >> 13)) >> 1) + 0x20000) + ((off & 0x40000) >> 2), size >> 1);
        t->tex4x4Loaded |= 1;
    }
    if (begin) GX_EndLoadTex();
}
void func_02031934(ResTex *t, u32 *key, u32 *key4x4) {   // NNS_G3dTexReleaseTexKey
    if (key != 0) {
        t->texLoaded &= ~1;
        *key = t->texKey;
        t->texKey = 0;
    }
    if (key4x4 != 0) {
        t->tex4x4Loaded &= ~1;
        *key4x4 = t->tex4x4Key;
        t->tex4x4Key = 0;
    }
}
u32 func_02031980(ResTex *t) { return t ? t->plttSize << 3 : 0; }
void func_02031994(ResTex *t, u32 key) { t->plttKey = key; }
void func_0203199c(ResTex *t, BOOL begin) {   // NNS_G3dPlttLoad
    if (begin) GX_BeginLoadTexPltt();
    GX_LoadTexPltt((u8 *)t + t->plttOfs, (t->plttKey << 16) >> 13, t->plttSize << 3);
    t->plttLoaded |= 1;
    if (begin) GX_EndLoadTexPltt();
}
u32 func_020319ec(ResTex *t) {   // NNS_G3dPlttReleasePlttKey
    u32 key;
    t->plttLoaded &= ~1;
    key = t->plttKey;
    t->plttKey = 0;
    return key;
}

static inline u8 *MatByIdx(ResMat *mat, u32 idx) {   // material data by index (inlined)
    if (mat != 0) {
        u32 *e = DictEntry(&mat->dict, idx);
        if (e != 0) return (u8 *)mat + *e;
    }
    return 0;
}
typedef struct { u32 imageParam; u32 size; } TexInfo;

void func_02031a0c(ResMat *mat, MatList *list, ResTex *tex, TexInfo *info) {   // bind one texture to its materials
    u8 *idx = (u8 *)mat + list->idxOfs;
    u32 key;
    u32 i;
    if ((info->imageParam & 0x1c000000) != 0x14000000) key = tex->texKey;
    else key = tex->tex4x4Key;
    key = (u16)key & ~0xe0000000;
    for (i = 0; i < list->count; i++) {
        MatData *m = (MatData *)MatByIdx(mat, idx[i]);
        u32 w, h;
        m->texParam |= info->imageParam + key;
        w = info->size & 0x7ff;
        h = (info->size & (0x7ff << 11)) >> 11;
        if (w == m->texW) m->scaleS = 0x1000;
        else m->scaleS = FX_Div(w << 12, m->texW << 12);
        if (h == m->texH) m->scaleT = 0x1000;
        else m->scaleT = FX_Div(h << 12, m->texH << 12);
    }
    list->flag |= 1;
}
void func_02031b3c(ResMat *mat, MatList *list) {   // undo a texture binding
    u8 *idx = (u8 *)mat + list->idxOfs;
    u32 i;
    for (i = 0; i < list->count; i++) {
        MatData *m = (MatData *)MatByIdx(mat, idx[i]);
        m->texParam &= 0xc00f0000;
        m->scaleS = 0x1000;
        m->scaleT = 0x1000;
    }
    list->flag &= ~1;
}

static inline ResMat *MdlMat(ResMdl *mdl) {
    if (mdl != 0 && mdl->ofsMat != 0) return (ResMat *)((u8 *)mdl + mdl->ofsMat);
    return 0;
}
BOOL func_02031bf4(ResMdl *mdl, ResTex *tex) {   // NNS_G3dBindMdlTex
    ResMat *mat = MdlMat(mdl);
    Dict *d = (Dict *)((u8 *)mat + mat->texListOfs);
    u32 i;
    BOOL ok;
    ok = 1;
    for (i = 0; i < d->num; i++) {
        u8 *name = DictName(d, i);
        TexInfo *info = tex == 0 ? 0 : func_0203324c(&tex->texDict, (char *)name);
        if (info != 0) {
            MatList *l = DictEntry(d, i);
            if (!(l->flag & 1)) func_02031a0c(mat, l, tex, info);
        } else {
            ok = 0;
        }
    }
    return ok;
}
void func_02031cf4(ResMdl *mdl) {   // NNS_G3dReleaseMdlTex
    ResMat *mat = MdlMat(mdl);
    Dict *d = (Dict *)((u8 *)mat + mat->texListOfs);
    u32 i;
    for (i = 0; i < d->num; i++) {
        MatList *l = DictEntry(d, i);
        if (l->flag & 1) func_02031b3c(mat, l);
    }
}
void func_02031d80(ResMat *mat, MatList *list, ResTex *tex, u16 *info) {   // bind one palette to its materials
    u8 *idx = (u8 *)mat + list->idxOfs;
    u16 off = info[0];
    u16 key = (u16)tex->plttKey & ~0xe0000000;
    u32 i;
    if (!(info[1] & 1)) {
        off = off >> 1;
        key = key >> 1;
    }
    for (i = 0; i < list->count; i++) {
        MatData *m = (MatData *)MatByIdx(mat, idx[i]);
        m->plttBase = (u16)(off + key);
    }
    list->flag |= 1;
}
BOOL func_02031e60(ResMdl *mdl, ResTex *tex) {   // NNS_G3dBindMdlPltt
    ResMat *mat = MdlMat(mdl);
    Dict *d = (Dict *)((u8 *)mat + mat->plttListOfs);
    u32 i;
    BOOL ok;
    ok = 1;
    for (i = 0; i < d->num; i++) {
        u8 *name = DictName(d, i);
        u16 *info = (tex == 0 || tex->plttDictOfs == 0) ? 0 : func_0203324c((Dict *)((u8 *)tex + tex->plttDictOfs), (char *)name);
        if (info != 0) {
            MatList *l = DictEntry(d, i);
            if (!(l->flag & 1)) func_02031d80(mat, l, tex, info);
        } else {
            ok = 0;
        }
    }
    return ok;
}
void func_02031f68(ResMdl *mdl) {   // NNS_G3dReleaseMdlPltt
    ResMat *mat = MdlMat(mdl);
    Dict *d = (Dict *)((u8 *)mat + mat->plttListOfs);
    u32 i;
    for (i = 0; i < d->num; i++) {
        MatList *l = DictEntry(d, i);
        if (l->flag & 1) l->flag &= ~1;
    }
}
static inline ResMdl *MdlByIdx(ResMdlSet *set, u32 idx) {
    if (set != 0) {
        u32 *e = DictEntry(&set->dict, idx);
        if (e != 0) return (ResMdl *)((u8 *)set + *e);
    }
    return 0;
}
BOOL func_02031fec(ResMdlSet *set, ResTex *tex) {   // NNS_G3dBindMdlSet
    u32 i;
    BOOL ok;
    ok = 1;
    for (i = 0; i < set->dict.num; i++) {
        ResMdl *mdl = MdlByIdx(set, i);
        ok &= func_02031bf4(mdl, tex);
        ok &= func_02031e60(mdl, tex);
    }
    return ok;
}
void func_0203209c(ResMdlSet *set) {   // NNS_G3dReleaseMdlSet
    u32 i;
    for (i = 0; i < set->dict.num; i++) {
        ResMdl *mdl = MdlByIdx(set, i);
        func_02031cf4(mdl);
        func_02031f68(mdl);
    }
}
