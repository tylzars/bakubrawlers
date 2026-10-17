// Rotation/scale helpers in ITCM: 0x01ffcef4-0x01ffd190. Compiler: 2.0/sp1p5 (7 of 8 match).
#include <nitro/internal.h>

extern const s16 FX_SinCosTable_[];   // sin/cos pairs indexed by angle >> 4
void MTX_RotX33_(MtxFx33 *m, fx32 sin, fx32 cos);
void MTX_RotY33_(MtxFx33 *m, fx32 sin, fx32 cos);
void MTX_RotZ33_(MtxFx33 *m, fx32 sin, fx32 cos);
void MTX_RotX43_(MtxFx43 *m, fx32 sin, fx32 cos);
void MTX_RotY43_(MtxFx43 *m, fx32 sin, fx32 cos);
void MTX_RotZ43_(MtxFx43 *m, fx32 sin, fx32 cos);
void MTX_Concat33(const MtxFx33 *a, const MtxFx33 *b, MtxFx33 *ab);
void MTX_Concat43(const MtxFx43 *a, const MtxFx43 *b, MtxFx43 *ab);
void MTX_Concat22(const fx32 *a, const fx32 *b, fx32 *ab);

void MTX_ScaleRot22(MtxFx22 *out, fx32 sx, fx32 sy, u16 angle) {
    fx32 scale[4];
    fx32 rot[4];
    int idx = angle >> 4;
    fx32 sn, cs;
    scale[0] = sx; scale[1] = 0; scale[2] = 0; scale[3] = sy;
    sn = FX_SinCosTable_[idx * 2];
    cs = FX_SinCosTable_[idx * 2 + 1];
    rot[0] = cs; rot[1] = sn; rot[2] = -sn; rot[3] = cs;
    MTX_Concat22(scale, rot, (fx32 *) out);
}

void MTX_SetTranslation43(MtxFx43 *m, VecFx32 *v) {
    m->_[9] = v->x;
    m->_[10] = v->y;
    m->_[11] = v->z;
}

#define ROT33(NAME, FN) \
void NAME(MtxFx33 *m, int angle) { \
    MtxFx33 tmp; \
    int idx = (u16) angle >> 4; \
    FN(&tmp, FX_SinCosTable_[idx * 2], FX_SinCosTable_[idx * 2 + 1]); \
    MTX_Concat33(&tmp, m, m); \
}
#define ROT43(NAME, FN) \
void NAME(MtxFx43 *m, int angle) { \
    MtxFx43 tmp; \
    int idx = (u16) angle >> 4; \
    FN(&tmp, FX_SinCosTable_[idx * 2], FX_SinCosTable_[idx * 2 + 1]); \
    MTX_Concat43(&tmp, m, m); \
}
ROT33(MTX_ApplyRotX33, MTX_RotX33_)
ROT33(MTX_ApplyRotY33, MTX_RotY33_)
ROT33(MTX_ApplyRotZ33, MTX_RotZ33_)
ROT43(MTX_ApplyRotX43, MTX_RotX43_)
ROT43(MTX_ApplyRotY43, MTX_RotY43_)
ROT43(MTX_ApplyRotZ43, MTX_RotZ43_)
