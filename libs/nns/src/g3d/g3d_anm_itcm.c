#include <nitro/internal.h>

#include <nnsys/private/nns_g3d.h>

extern u32 data_020a578c;
void MIi_CpuClear16(u16 data, void *dest, u32 size);
void func_01ff9b64(void *res, int idx, int frame, void *out);

void func_01ff9804(AnmObj *obj, u8 *res, u8 *mdl) {
    u32 i;
    u8 n;
    u16 *ofs;
    obj->res = res;
    obj->f0c = data_020a578c;
    n = mdl[0x17];
    obj->mapCount = n;
    MIi_CpuClear16(0, obj->map, n * 2);
    ofs = (u16 *) (res + 0x14);
    for (i = 0; i < *(u16 *) (res + 6); i++) {
        obj->map[i] = (u16) ((*(u32 *) (res + ofs[i]) >> 24) | 0x100);
    }
}

void func_01ff9880(void *out, AnmObj *obj, int idx) {
    u8 *res = (u8 *) obj->res;
    int f = obj->frame;
    int max = *(u16 *) (res + 4) << 12;
    if (f >= max) {
        f = max - 1;
    } else if (f < 0) {
        f = 0;
    }
    func_01ff9b64(res, idx, f, out);
}

typedef struct G3dState {
    u8 *mCur;                                    // 0x00: current record, byte 1 is the index
    u8 pad4[0xd0];
    Dict *mDict;                                 // 0xd4
    u8 padd8[0x10];
    void (*mCallback)(void *out, void *data, u8 *cmd, u32 flags);    // 0xe8
} G3dState;
extern G3dState *data_020baf00;

void func_01ff98bc(u32 *out) {
    G3dState *s = data_020baf00;
    u32 idx = s->mCur[1];
    Dict *dict = s->mDict;
    u16 *p;
    u32 *e;
    if (dict != 0 && (e = (u32 *) DictEntry(dict, idx)) != 0) {
        p = (u16 *) ((u8 *) dict + *e);
    } else {
        p = 0;
    }
    if (p[0] & 1) {
        out[0] |= 4;
        return;
    }
    out[0x4c / 4] = ((u32 *) p)[1];
    out[0x50 / 4] = ((u32 *) p)[2];
    out[0x54 / 4] = ((u32 *) p)[3];
}

void func_01ff9954(void *out) {
    G3dState *s = data_020baf00;
    u8 *cmd = s->mCur;
    u32 idx = cmd[1];
    Dict *dict = s->mDict;
    u16 *p;
    u32 *e;
    u16 flags;
    if (dict != 0 && (e = (u32 *) DictEntry(dict, idx)) != 0) {
        p = (u16 *) ((u8 *) dict + *e);
    } else {
        p = 0;
    }
    flags = *p;
    p += 2;
    if (!(flags & 1)) {
        p += 6;
    }
    if (!(flags & 2)) {
        if (flags & 8) {
            p += 2;
        } else {
            p += 8;
        }
    }
    s->mCallback(out, p, cmd, flags);
}

extern const u8 data_0208b4bc[][4]; // 4-byte stride tables: output slot for each of a, b, +-b, +-a
extern const u8 data_0208b4bd[][4];
extern const u8 data_0208b4be[];
extern const u8 data_0208b4bf[];
void func_01ff97f4(void *mtx);

typedef struct PivotOut {
    u32 mFlags;      // 0x0: bit 1 = rotation unavailable
    u8 pad4[0x24];
    s32 mMtx[9];     // 0x28: 3x3 rotation
} PivotOut;

void func_01ff99e4(PivotOut *out) {
    G3dState *s = data_020baf00;
    int k;
    u32 idx = s->mCur[1];
    Dict *dict = s->mDict;
    u16 *p;
    u32 *e;
    s16 *v;
    if (dict != 0 && (e = (u32 *) DictEntry(dict, idx)) != 0) {
        p = (u16 *) ((u8 *) dict + *e);
    } else {
        p = 0;
    }
    v = (s16 *) (p + 2);
    if (!(p[0] & 1)) {
        v += 6;
    }
    if (!(p[0] & 2)) {
        if (p[0] & 8) {
            s32 b;
            s32 a;
            k = (p[0] & 0xf0) >> 4;
            a = v[0];
            b = v[1];
            func_01ff97f4(out->mMtx);
            out->mMtx[k] = (p[0] & 0x100) ? -0x1000 : 0x1000;
            out->mMtx[data_0208b4bc[k][0]] = a;
            out->mMtx[data_0208b4bd[k][0]] = b;
            if (p[0] & 0x200) {
                b = -b;
            }
            out->mMtx[data_0208b4be[k * 4]] = b;
            if (p[0] & 0x400) {
                a = -a;
            }
            out->mMtx[data_0208b4bf[k * 4]] = a;
        } else {
            out->mMtx[0] = ((s16 *) p)[1];
            out->mMtx[1] = v[0];
            out->mMtx[2] = v[1];
            out->mMtx[3] = v[2];
            out->mMtx[4] = v[3];
            out->mMtx[5] = v[4];
            out->mMtx[6] = v[5];
            out->mMtx[7] = v[6];
            out->mMtx[8] = v[7];
        }
    } else {
        out->mFlags |= 2;
    }
}
