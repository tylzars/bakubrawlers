// Rotation/scale helpers in ITCM: 0x01ffcef4-0x01ffd190. Compiler: 2.0/sp1p5 (7 of 8 match).
#include <nitro/internal.h>

extern const s16 FX_SinCosTable_[];   // sin/cos pairs indexed by angle >> 4
void func_01ff8260(MtxFx33 *m, fx32 sin, fx32 cos);
void func_01ff827c(MtxFx33 *m, fx32 sin, fx32 cos);
void func_01ff8298(MtxFx33 *m, fx32 sin, fx32 cos);
void func_01ff85dc(MtxFx43 *m, fx32 sin, fx32 cos);
void func_01ff85fc(MtxFx43 *m, fx32 sin, fx32 cos);
void func_01ff8618(MtxFx43 *m, fx32 sin, fx32 cos);
void func_01ff82b0(const MtxFx33 *a, const MtxFx33 *b, MtxFx33 *ab);
void func_01ff89c0(const MtxFx43 *a, const MtxFx43 *b, MtxFx43 *ab);
void MTX_Concat22(const fx32 *a, const fx32 *b, fx32 *ab);

void func_01ffcef4(MtxFx22 *out, fx32 sx, fx32 sy, u16 angle) {
    int idx = angle >> 4;
    fx32 sn = FX_SinCosTable_[idx * 2];
    fx32 cs = FX_SinCosTable_[idx * 2 + 1];
    fx32 scale[4];
    fx32 rot[4];
    scale[0] = sx;
    scale[1] = 0;
    scale[2] = 0;
    scale[3] = sy;
    rot[0] = cs;
    rot[1] = sn;
    rot[2] = -sn;
    rot[3] = cs;
    MTX_Concat22(scale, rot, (fx32 *) out);
}

void func_01ffcf64(MtxFx43 *m, VecFx32 *v) {
    m->_[9] = v->x;
    m->_[10] = v->y;
    m->_[11] = v->z;
}

#define ROT33(NAME, FN) \
void NAME(MtxFx33 *m, int angle) { \
    MtxFx33 tmp; \
    int idx = (u16) angle >> 4; \
    FN(&tmp, FX_SinCosTable_[idx * 2], FX_SinCosTable_[idx * 2 + 1]); \
    func_01ff82b0(&tmp, m, m); \
}
#define ROT43(NAME, FN) \
void NAME(MtxFx43 *m, int angle) { \
    MtxFx43 tmp; \
    int idx = (u16) angle >> 4; \
    FN(&tmp, FX_SinCosTable_[idx * 2], FX_SinCosTable_[idx * 2 + 1]); \
    func_01ff89c0(&tmp, m, m); \
}
ROT33(func_01ffcf80, func_01ff8260)
ROT33(func_01ffcfd8, func_01ff827c)
ROT33(func_01ffd030, func_01ff8298)
ROT43(func_01ffd088, func_01ff85dc)
ROT43(func_01ffd0e0, func_01ff85fc)
ROT43(func_01ffd138, func_01ff8618)
