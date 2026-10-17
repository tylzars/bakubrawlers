// NitroSystem G3d SBC renderer and its opcode handlers, ITCM 0x01ffae10-0x01ffc93c.
// Compiler: 2.0/sp1p5 (same bytes on sp2p2).
#include <nnsys/private/g3d_rs.h>

extern const u8 data_0208b500[];
extern const u8 data_0208b501[];
extern const u8 data_0208b502[];
extern const u8 data_0208b503[];
extern const u32 data_0208b4e0[];
extern u32 data_02211374[];
extern MatSt data_020baf04[];
extern u8 data_022113c0[];
extern SbcFn data_020a4ea0[];                       // SBC opcode handlers, low 5 bits of the command byte
extern void (*data_020a5804[])(void);
extern void (*data_020a57f8[])(void);
extern void (*data_020a5810[])(void);
extern void (*data_020a4e80[])(Rs *, u32, u16 *, u32);   // material handlers
extern void (*data_020a4e90[])(Rs *, u32, u16 *, u32);   // node-description handlers

extern s32 VEC_Mag(const s32 *v);        // VEC_Mag
extern void VEC_Normalize(const s32 *src, s32 *dst);    // VEC_Normalize
extern void func_01ff8574(const void *src, void *dst);  // 4x3 to 4x4
extern void func_02000de8(const s32 *a, const s32 *b, s32 *ab);   // MTX_Concat44
extern int G3X_GetClipMtx(s32 *m);   // G3X_GetClipMtx
extern void *func_020326a0(void);
extern void *func_020326d8(void);
extern void *func_020323b8(void);
extern void func_01ff92a4(void);
extern void func_01ff9528(const void *src, void *dst, u32 size);
#define GXFIFO (*(vu32 *) 0x04000400)

// Two geometry-command blocks kept in ITCM in front of the renderer: MTX_POP(1), MTX_MODE(2), MTX_LOAD_4x3, MTX_SCALE.
// func_01ffbbac and func_01ffbe2c fill in the translation and scale (and the second block's rotation) before sending.
typedef struct {
    u32 op;          // packed command bytes 0x12, 0x10, 0x17, 0x1b
    u32 pop;
    u32 mode;
    s32 mtx[9];      // 3x3 part of the 4x3 load
    s32 trans[3];
    s32 scale[3];
} EnvCmd;

#pragma define_section ITCMDATA ".text" ".text"
#pragma section ITCMDATA begin
EnvCmd data_01ffae10[2] = {
    {0x1b171012, 1, 2, {0x1000, 0, 0, 0, 0x1000, 0, 0, 0, 0x1000}, {0, 0, 0}, {0, 0, 0}},
    {0x1b171012, 1, 2, {0x1000, 0, 0, 0, 0x1000, 0, 0, 0, 0x1000}, {0, 0, 0}, {0, 0, 0}},
};
#pragma section ITCMDATA end
extern void G3_MultMtx43(void *p);
extern int G3X_GetVectorMtx(void *p);
extern void G3_MultMtx33(void *p);
extern void G3_LoadMtx43(void *p);
extern u8 data_020bc304[];   // per-matrix cache entries, 100 bytes each: 4x4 position matrix then 3x3 normal matrix
#define MTX_MODE (*(vu32 *) 0x04000440)
#define MTX_STORE (*(vu32 *) 0x0400044c)
#define MTX_RESTORE (*(vu32 *) 0x04000450)
#define MTX_IDENTITY (*(vu32 *) 0x04000454)
#define NRM(k) acc.nrm[k] += (w64 * prevNrm[k]) >> 12
#define NRMALL NRM(0); NRM(1); NRM(2); NRM(3); NRM(4); NRM(5); NRM(6); NRM(7); NRM(8);
#define POS(k, e) acc.pos[k] += ((s64) (s32) w64 * pos[e]) >> 12

void func_01ffaea0(Rs *rs) {
    do {
        u32 c;
        rs->flag &= ~0x40;
        c = *rs->cmd;
        data_020a4ea0[c & 0x1f](rs, c & 0xe0);
    } while (!(rs->flag & 0x20));
}

static inline u8 *ResAt(u8 *m, int ofs) {
    u32 o;
    if (m != 0 && (o = *(u32 *) (m + ofs)) != 0) {
        return m + o;
    }
    return 0;
}

void func_01ffaee8(Rs *rs, RObj *obj) {
    MIi_CpuClearFast(0, rs, 0x188);
    rs->one = 1;
    rs->flag = 1;
    if (obj->sbcOverride) {
        rs->cmd = obj->sbcOverride;
    } else {
        rs->cmd = obj->mdl + *(u32 *) (obj->mdl + 4);
    }
    rs->obj = obj;
    rs->dNode = obj->mdl != 0 ? (Dict *) (obj->mdl + 0x40) : 0;
    rs->dMat = (ResMat *) ResAt(obj->mdl, 8);
    rs->dShp = (Dict *) ResAt(obj->mdl, 0xc);
    rs->cbE8 = data_020a5804[obj->mdl[0x15]];
    rs->cbEc = data_020a57f8[obj->mdl[0x15]];
    rs->cbF0 = data_020a5810[obj->mdl[0x16]];
    rs->mdlScale = *(s32 *) (obj->mdl + 0x1c);
    rs->mdlScaleInv = *(s32 *) (obj->mdl + 0x20);
    if (*(u32 *) ((u8 *) obj + 0x20) != 0 && ((u8 *) obj)[0x24] < 0x20) {
        rs->slot[((u8 *) obj)[0x24]] = *(void **) ((u8 *) obj + 0x20);
        rs->slotId[((u8 *) obj)[0x24]] = ((u8 *) obj)[0x25];
    }
    if (obj->flag & 1) {
        rs->flag |= 0x80;
    }
    if (obj->flag & 2) {
        rs->flag |= 0x100;
    }
    if (obj->flag & 4) {
        rs->flag |= 0x200;
    }
    if (obj->flag & 8) {
        rs->flag |= 0x400;
    }
    if (*(void (**)(Rs *)) ((u8 *) obj + 0x28) != 0) {
        (*(void (**)(Rs *)) ((u8 *) obj + 0x28))(rs);
    }
    func_01ffaea0(rs);
    obj->flag &= ~1;
}

typedef struct BitNode {
    u8 pad0[0x10];
    struct BitNode *next;      // 0x10
    u8 pad14[5];
    u8 count;                  // 0x19
    u16 info[1];               // 0x1a
} BitNode;

void func_01ffb084(u32 *bits, BitNode *n) {
    if (n == 0) {
        return;
    }
    do {
        int i;
        for (i = 0; i < n->count; i++) {
            if (n->info[i] & 0x100) {
                bits[i >> 5] |= 1 << (i & 0x1f);
            }
        }
        n = n->next;
    } while (n != 0);
}

void func_01ffb0ec(void *p) {
    u8 *o = (u8 *) p;
    if ((*(u32 *) o & 0x10) == 0x10) {
        MIi_CpuClear32(0, o + 0x3c, 8);
        MIi_CpuClear32(0, o + 0x44, 8);
        MIi_CpuClear32(0, o + 0x4c, 8);
        if (*(BitNode **) (o + 8) != 0) {
            func_01ffb084((u32 *) (o + 0x3c), *(BitNode **) (o + 8));
        }
        if (*(BitNode **) (o + 0x10) != 0) {
            func_01ffb084((u32 *) (o + 0x44), *(BitNode **) (o + 0x10));
        }
        if (*(BitNode **) (o + 0x18) != 0) {
            func_01ffb084((u32 *) (o + 0x4c), *(BitNode **) (o + 0x18));
        }
        *(u32 *) o &= ~0x10;
    }
    func_01ffaee8(data_020baf00, (RObj *) o);
}

void func_01ffb194(Rs *rs, u32 op) {
    rs->cmd++;
}

void func_01ffb1a4(Rs *rs, u32 op) {
    rs->flag |= 0x20;
}

void func_01ffb1b4(Rs *rs, u32 op) {
    if (!(rs->flag & 0x200)) {
        u32 idx = rs->cmd[1];
        RObj *o;
        rs->curNode = idx;
        rs->flag |= 4;
        rs->pVis = &rs->visStore;
        o = rs->obj;
        if (*(u32 *) ((u8 *) o + 0x18) == 0 || !(*(u32 *) ((u8 *) o + 0x4c + (idx >> 5) * 4) & (1 << (idx & 0x1f))) ||
            (*(int (**)(s32 *, u32, u32)) ((u8 *) o + 0x1c))(rs->pVis, *(u32 *) ((u8 *) o + 0x18), idx) == 0) {
            *rs->pVis = rs->cmd[2] & 1;
        }
        if (*rs->pVis != 0) {
            rs->flag |= 1;
        } else {
            rs->flag &= ~1;
        }
    }
    rs->cmd += 3;
}

void func_01ffb26c(Rs *rs, u32 op) {
    u32 arg;
    if (!(rs->flag & 0x200) && (rs->flag & 1)) {
        arg = rs->cmd[1];
        if (!(rs->flag & 0x100)) {
            func_01ff93d8(0x14, &arg, 1);
        }
    }
    rs->cmd += 2;
}

static inline MatRes *MatLookup(ResMat *m, u32 idx) {
    if (m != 0) {
        u32 *e = DictEntry(&m->dict, idx);
        if (e != 0) {
            return (MatRes *) ((u8 *) m + *e);
        }
    }
    return 0;
}

void func_01ffb2c8(Rs *rs, u32 op, MatRes *md, u32 idx) {
    MatSt *cur;
    MatSt *buf;
    u32 *g;
    rs->curMat = idx;
    rs->flag |= 8;
    rs->pMat = rs->matStore;
    buf = *(MatSt **) ((u8 *) rs->obj + 0x38);
    if (buf != 0 && !(rs->flag & 0x80)) {
        cur = buf + idx;
        goto done;
    }
    if ((op == 0x20 || op == 0x40) && (rs->matDone[idx >> 5] & (1 << (idx & 0x1f)))) {
        if (buf != 0) {
            cur = buf + idx;
        } else {
            cur = data_020baf04 + idx;
        }
        goto done;
    }
    if (buf != 0) {
        rs->matDone[idx >> 5] |= 1 << (idx & 0x1f);
        cur = *(MatSt **) ((u8 *) rs->obj + 0x38) + idx;
    } else if (op == 0x40) {
        rs->matDone[idx >> 5] |= 1 << (idx & 0x1f);
        cur = data_020baf04 + idx;
    } else {
        cur = (MatSt *) rs->matStore;
    }
    cur->flag = 0;
    if (MatLookup(rs->dMat, idx)->flag & 0x20) {
        cur->flag |= 0x20;
    }
    g = data_02211374;
    {
        u32 m = data_0208b4e0[(md->flag >> 6) & 7];
        cur->diffAmb = (g[0x94 / 4] & ~m) | (md->diffAmb & m);
        m = data_0208b4e0[(md->flag >> 9) & 7];
        cur->specEmi = (g[0x98 / 4] & ~m) | (md->specEmi & m);
        m = md->polyMask;
        cur->polyAttr = (g[0x9c / 4] & ~m) | (md->polyAttr & m);
    }
    cur->texParam = md->texParam;
    cur->plttBase = md->plttBase;
    if (md->flag & 1) {
        s32 *t = md->texMtx;
        if (!(md->flag & 2)) {
            cur->scaleS = t[0];
            cur->scaleT = t[1];
            t += 2;
        } else {
            cur->flag |= 1;
        }
        if (!(md->flag & 4)) {
            cur->rotSin = ((s16 *) t)[0];
            cur->rotCos = ((s16 *) t)[1];
            t += 1;
        } else {
            cur->flag |= 2;
        }
        if (!(md->flag & 8)) {
            cur->transS = t[0];
            cur->transT = t[1];
        } else {
            cur->flag |= 4;
        }
        cur->flag |= 8;
    }
    {
        RObj *o = rs->obj;
        u32 cb = *(u32 *) ((u8 *) o + 8);
        if (cb != 0 && (*(u32 *) ((u8 *) o + 0x3c + (idx >> 5) * 4) & (1 << (idx & 0x1f)))) {
            (*(void (**)(MatSt *, u32, u32)) ((u8 *) o + 0xc))(cur, cb, idx);
        }
    }
    if (cur->flag & 0x18) {
        cur->texW = md->texW;
        cur->texH = md->texH;
        cur->scaleW = md->scaleW;
        cur->scaleH = md->scaleH;
    }
done:
    rs->pMat = cur;
    if (cur->polyAttr & 0x1f0000) {
        u32 cmd[7];
        if (cur->flag & 0x20) {
            cur->polyAttr &= ~0x1f0000;
        }
        rs->flag &= ~2;
        if (rs->flag & 0x100) {
            return;
        }
        cmd[0] = 0x293130;
        cmd[1] = cur->diffAmb;
        cmd[2] = cur->specEmi;
        cmd[3] = cur->polyAttr;
        cmd[4] = 0x2b2a;
        cmd[5] = cur->texParam;
        cmd[6] = cur->plttBase;
        func_01ff93d8(cmd[0], &cmd[1], 6);
        if (cur->flag & 0x18) {
            ((void (*)(MatSt *)) rs->cbF0)(cur);
        }
    } else {
        rs->flag |= 2;
    }
}

static inline u16 *DictPtr(ResMat *m, u32 idx) {
    u16 *p;
    if (m != 0) {
        u32 *e = DictEntry(&m->dict, idx);
        if (e != 0) {
            return (u16 *) ((u8 *) m + *e);
        }
    }
    return 0;
}

void func_01ffb684(Rs *rs, u32 op) {
    if (!(rs->flag & 0x200)) {
        u32 idx = rs->cmd[1];
        if ((rs->flag & 1) || !(rs->flag & 8) || idx != rs->curMat) {
            u16 *p = DictPtr(rs->dMat, idx);
            data_020a4e80[*p](rs, op, p, idx);
        }
    }
    rs->cmd += 2;
}

void func_01ffb734(Rs *rs, u32 op, u32 *data) {
    if (!(rs->flag & 0x100)) {
        func_01ff9328((u8 *) data + data[2], data[3]);
    }
}

static inline u16 *DictPtr2(Dict *d, u32 idx) {
    if (d != 0) {
        u32 *e = DictEntry(d, idx);
        if (e != 0) {
            return (u16 *) ((u8 *) d + *e);
        }
    }
    return 0;
}

void func_01ffb758(Rs *rs, u32 op) {
    if (!(rs->flag & 0x200) && (rs->flag & 1) && !(rs->flag & 2)) {
        u32 idx = rs->cmd[1];
        u16 *p = DictPtr2(rs->dShp, idx);
        data_020a4e90[*p](rs, op, p, idx);
    }
    rs->cmd += 2;
}

static inline NodeRes *NodePtr(Dict *d, u32 idx) {
    if (d != 0) {
        u32 *e = DictEntry(d, idx);
        if (e != 0) {
            return (NodeRes *) ((u8 *) d + *e);
        }
    }
    return 0;
}

void func_01ffb7f8(Rs *rs, u32 op) {
    u32 arg;
    u32 arg2;
    u32 arg3;
    u32 idx = rs->cmd[1];
    u32 len = 4;
    ShpSt *s;
    u8 *buf;
    int cached;
    int skip;
    rs->curShp = idx;
    rs->flag |= 0x10;
    if (rs->flag & 0x400) {
        if (op == 0x40 || op == 0x60) {
            len++;
        }
        if (op == 0x20 || op == 0x60) {
            len++;
            if (!(rs->flag & 0x100)) {
                arg = rs->cmd[4];
                func_01ff93d8(0x14, &arg, 1);
            }
        }
        rs->cmd += len;
        return;
    }
    if (op == 0x40 || op == 0x60) {
        if (op == 0x40) {
            arg2 = rs->cmd[4];
        } else {
            arg2 = rs->cmd[5];
        }
        len++;
        if (!(rs->flag & 0x100)) {
            func_01ff93d8(0x14, &arg2, 1);
        }
    }
    s = (ShpSt *) rs->shpStore;
    rs->pShp = s;
    buf = *(u8 **) ((u8 *) rs->obj + 0x34);
    if (buf != 0) {
        s = (ShpSt *) (buf + idx * 0x58);
        cached = !(rs->flag & 0x80);
    } else {
        cached = 0;
    }
    if (!cached) {
        NodeRes *n;
        s32 *p;
        s->flag = 0;
        if (*(u32 *) ((u8 *) rs->obj + 0x10) != 0 &&
            (*(int (**)(ShpSt *, u32, u32)) ((u8 *) rs->obj + 0x14))(s, *(u32 *) ((u8 *) rs->obj + 0x10), idx) != 0) {
            goto emit;
        }
        n = NodePtr(rs->dNode, idx);
        p = n->data;
        if (n->flag & 1) {
            s->flag |= 4;
        } else {
            s->trans[0] = p[0];
            s->trans[1] = p[1];
            s->trans[2] = p[2];
            p += 3;
        }
        if (n->flag & 2) {
            s->flag |= 2;
        } else if (n->flag & 8) {
            u32 k = (n->flag & 0xf0) >> 4;
            s32 a = ((s16 *) p)[1];
            s32 b = ((s16 *) p)[0];
            func_01ff97f4(s->mtx);
            s->mtx[k] = (n->flag & 0x100) ? -0x1000 : 0x1000;
            s->mtx[data_0208b500[k * 4]] = b;
            s->mtx[data_0208b501[k * 4]] = a;
            s->mtx[data_0208b502[k << 2]] = (n->flag & 0x200) ? -a : a;
            s->mtx[data_0208b503[k << 2]] = (n->flag & 0x400) ? -b : b;
            p += 1;
        } else {
            s->mtx[0] = n->hw02;
            s->mtx[1] = ((s16 *) p)[0];
            s->mtx[2] = ((s16 *) p)[1];
            s->mtx[3] = ((s16 *) p)[2];
            s->mtx[4] = ((s16 *) p)[3];
            s->mtx[5] = ((s16 *) p)[4];
            s->mtx[6] = ((s16 *) p)[5];
            s->mtx[7] = ((s16 *) p)[6];
            s->mtx[8] = ((s16 *) p)[7];
            p += 4;
        }
        (*(void (**)(ShpSt *, s32 *, u8 *, u32)) &rs->cbE8)(s, p, rs->cmd, n->flag);
    }
emit:
    rs->pShp = s;
    if (!(rs->flag & 0x100)) {
        (*(void (**)(ShpSt *)) &rs->cbEc)(rs->pShp);
    }
    rs->pShp = 0;
    skip = 0;
    if (rs->slot[6] != 0) {
        skip = rs->slotId[6];
    }
    if (skip == 3) {
        rs->flag &= ~0x40;
        ((void (*)(Rs *)) rs->slot[6])(rs);
        skip = rs->flag & 0x40;
    } else {
        skip = 0;
    }
    if (op == 0x20 || op == 0x60) {
        len++;
        if (skip == 0 && !(rs->flag & 0x100)) {
            arg3 = rs->cmd[4];
            func_01ff93d8(0x13, &arg3, 1);
        }
    }
    rs->cmd += len;
}

void func_01ffbbac(Rs *rs, u32 op) {
    s32 clip[16];
    s32 t1[16];
    s32 t2[16];
    u32 arg2;
    u32 arg3;
    u32 len = 2;
    s32 *v0 = data_01ffae10[0].trans;
    s32 *v1 = data_01ffae10[0].scale;
    u32 g;
    if (rs->flag & 0x200) {
        if (op == 0x40 || op == 0x60) {
            len++;
        }
        if (op == 0x20 || op == 0x60) {
            len++;
        }
        rs->cmd += len;
        return;
    }
    if (op == 0x40 || op == 0x60) {
        len++;
        if (!(rs->flag & 0x100)) {
            if (op == 0x40) {
                arg2 = rs->cmd[2];
            } else {
                arg2 = rs->cmd[3];
            }
            func_01ff93d8(0x14, &arg2, 1);
        }
    }
    if (!(rs->flag & 0x100)) {
        func_01ff92a4();
        GXFIFO = 0x151110;
        GXFIFO = 0;
        GXFIFO = 0;
        while (G3X_GetClipMtx(clip) != 0) {
        }
        g = data_02211374[0xfc / 4];
        if (g & 1) {
            func_01ff8574(func_020326a0(), t1);
            func_02000de8(clip, t1, clip);
        } else if (g & 2) {
            func_01ff8574(data_022113c0, t2);
            func_02000de8(clip, t2, clip);
        }
        v0[0] = clip[12];
        v0[1] = clip[13];
        v0[2] = clip[14];
        v1[0] = VEC_Mag(&clip[0]);
        v1[1] = VEC_Mag(&clip[4]);
        v1[2] = VEC_Mag(&clip[8]);
        g = data_02211374[0xfc / 4];
        if (g & 1) {
            GXFIFO = 0x171012;
            func_01ff9528(&data_01ffae10[0].pop, (void *) 0x04000400, 8);
            func_01ff9528(func_020326d8(), (void *) 0x04000400, 0x30);
            GXFIFO = 0x1b19;
            func_01ff9528(data_01ffae10[0].mtx, (void *) 0x04000400, 0x3c);
        } else if (g & 2) {
            GXFIFO = 0x171012;
            func_01ff9528(&data_01ffae10[0].pop, (void *) 0x04000400, 8);
            func_01ff9528(func_020323b8(), (void *) 0x04000400, 0x30);
            GXFIFO = 0x1b19;
            func_01ff9528(data_01ffae10[0].mtx, (void *) 0x04000400, 0x3c);
        } else {
            func_01ff9528(&data_01ffae10[0], (void *) 0x04000400, 0x48);
        }
    }
    if (op == 0x20 || op == 0x60) {
        len++;
        if (!(rs->flag & 0x100)) {
            arg3 = rs->cmd[2];
            func_01ff93d8(0x13, &arg3, 1);
        }
    }
    rs->cmd += len;
}

void func_01ffbe2c(Rs *rs, u32 op) {
    s32 clip[16];
    s32 t1[16];
    s32 t2[16];
    u32 arg2;
    u32 arg3;
    u32 len = 2;
    s32 *v0 = data_01ffae10[1].trans;
    s32 *v1 = data_01ffae10[1].scale;
    s32 *bb = data_01ffae10[1].mtx;
    u32 g;
    if (rs->flag & 0x200) {
        if (op == 0x40 || op == 0x60) {
            len++;
        }
        if (op == 0x20 || op == 0x60) {
            len++;
        }
        rs->cmd += len;
        return;
    }
    if (op == 0x40 || op == 0x60) {
        len++;
        if (!(rs->flag & 0x100)) {
            if (op == 0x40) {
                arg2 = rs->cmd[2];
            } else {
                arg2 = rs->cmd[3];
            }
            func_01ff93d8(0x14, &arg2, 1);
        }
    }
    if (!(rs->flag & 0x100)) {
        func_01ff92a4();
        GXFIFO = 0x151110;
        GXFIFO = 0;
        GXFIFO = 0;
        while (G3X_GetClipMtx(clip) != 0) {
        }
        g = data_02211374[0xfc / 4];
        if (g & 1) {
            func_01ff8574(func_020326a0(), t1);
            func_02000de8(clip, t1, clip);
        } else if (g & 2) {
            func_01ff8574(data_022113c0, t2);
            func_02000de8(clip, t2, clip);
        }
        v0[0] = clip[12];
        v0[1] = clip[13];
        v0[2] = clip[14];
        v1[0] = VEC_Mag(&clip[0]);
        v1[1] = VEC_Mag(&clip[4]);
        v1[2] = VEC_Mag(&clip[8]);
        if (clip[5] != 0 || clip[6] != 0) {
            VEC_Normalize(&clip[4], &bb[3]);
            bb[7] = -bb[5];
            bb[8] = bb[4];
        } else {
            VEC_Normalize(&clip[8], &bb[6]);
            bb[5] = -bb[7];
            bb[4] = bb[8];
        }
        g = data_02211374[0xfc / 4];
        if (g & 1) {
            GXFIFO = 0x171012;
            func_01ff9528(&data_01ffae10[1].pop, (void *) 0x04000400, 8);
            func_01ff9528(func_020326d8(), (void *) 0x04000400, 0x30);
            GXFIFO = 0x1b19;
            func_01ff9528(data_01ffae10[1].mtx, (void *) 0x04000400, 0x3c);
        } else if (g & 2) {
            GXFIFO = 0x171012;
            func_01ff9528(&data_01ffae10[1].pop, (void *) 0x04000400, 8);
            func_01ff9528(func_020323b8(), (void *) 0x04000400, 0x30);
            GXFIFO = 0x1b19;
            func_01ff9528(data_01ffae10[1].mtx, (void *) 0x04000400, 0x3c);
        } else {
            func_01ff9528(&data_01ffae10[1], (void *) 0x04000400, 0x48);
        }
    }
    if (op == 0x20 || op == 0x60) {
        len++;
        if (!(rs->flag & 0x100)) {
            arg3 = rs->cmd[2];
            func_01ff93d8(0x13, &arg3, 1);
        }
    }
    rs->cmd += len;
}

static inline u32 TestBit(const u32 *p, u32 i) {
    return p[i >> 5] & (1 << (i & 31));
}

void func_01ffc108(Rs *rs) {
    struct {
        s32 pos[12];
        s32 nrm[9];
    } acc;
    s64 w64 = 0;
    u8 *mdl = rs->obj->mdl;
    u8 *cmd = rs->cmd;
    u8 *tbl = mdl + *(u32 *) (mdl + 0x10);
    u32 n = cmd[2];
    s32 *pos;
    s32 *prevNrm;
    u32 j;
    int i;
    cmd += 3;
    MIi_CpuClearFast(0, &acc, sizeof(acc));
    func_01ff92a4();
    MTX_MODE = 0;
    MTX_STORE = 1;
    MTX_IDENTITY = 0;
    MTX_MODE = 2;
    for (j = 0; j < n; j++) {
        u32 idx = cmd[1];
        u32 bit = TestBit(rs->shpDone, idx);
        pos = (s32 *) (data_020bc304 + idx * 100);
        if (!bit) {
            rs->shpDone[idx >> 5] |= 1 << (idx & 31);
            MTX_RESTORE = cmd[0];
            MTX_MODE = 1;
            G3_MultMtx43(tbl + idx * 0x54);
        }
        if (j != 0) {
            NRMALL
        }
        if (!bit) {
            while (G3X_GetClipMtx(pos)) {
            }
            MTX_MODE = 2;
            G3_MultMtx33(tbl + idx * 0x54 + 0x30);
        }
        w64 = cmd[2] << 4;
        POS(0, 0);
        POS(1, 1);
        POS(2, 2);
        POS(3, 4);
        POS(4, 5);
        POS(5, 6);
        POS(6, 8);
        POS(7, 9);
        POS(8, 10);
        POS(9, 12);
        POS(10, 13);
        POS(11, 14);
        cmd += 3;
        prevNrm = (s32 *) (data_020bc304 + 0x40 + idx * 100);
        if (!bit) {
            while (G3X_GetVectorMtx(prevNrm)) {
            }
        }
    }
    NRMALL
    G3_LoadMtx43(acc.nrm);
    MTX_MODE = 1;
    G3_LoadMtx43(acc.pos);
    MTX_MODE = 0;
    MTX_RESTORE = 1;
    MTX_MODE = 2;
    MTX_STORE = rs->cmd[1];
    rs->cmd += (rs->cmd[2] + 1) * 3;
}

void func_01ffc864(Rs *rs, u32 op) {
    if (!(rs->flag & 0x100)) {
        u8 *c = rs->cmd;
        u32 ofs = c[1] | (c[2] << 8) | (c[3] << 16) | (c[4] << 24);
        u32 size = c[5] | (c[6] << 8) | (c[7] << 16) | (c[8] << 24);
        func_01ff9328(c + ofs, size);
    }
    rs->cmd += 9;
}

void func_01ffc8cc(Rs *rs, u32 op) {
    if (!(rs->flag & 0x100) && !(rs->flag & 0x200)) {
        s32 v[3];
        if (op == 0) {
            v[0] = v[1] = v[2] = rs->mdlScale;
        } else {
            v[0] = v[1] = v[2] = rs->mdlScaleInv;
        }
        func_01ff93d8(0x1b, v, 3);
    }
    rs->cmd += 1;
}
