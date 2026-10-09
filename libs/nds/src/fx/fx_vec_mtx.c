// Fixed-point matrix and vector math in ITCM: 0x01ff81a0-0x01ff9208.
// 0x01ff817c, 0x01ff854c, 0x01ff8574, 0x01ff85dc, 0x01ff85fc, 0x01ff8618 (stm-heavy identity/rotation fills) are hand assembly.
#include <nitro/internal.h>
extern void func_01ff80f8(fx32 x);                // FX_InvAsync
extern fx32 func_01ff80c0(void);                  // FX_GetDivResult
extern void func_01ff95e0(const void *src, void *dst);   // 48-byte copy
#define NEG(x) (-(x))

#define REG_DIVCNT (*(vu16 *) 0x04000280)
#define REG_DIV_NUMER (*(vs64 *) 0x04000290)
#define REG_DIV_DENOM (*(vs64 *) 0x04000298)
#define REG_DIV_RESULT (*(s64 *) 0x040002a0)
#define REG_SQRTCNT (*(vu16 *) 0x040002b0)
#define REG_SQRT_RESULT (*(s32 *) 0x040002b4)
#define REG_SQRT_PARAM (*(s64 *) 0x040002b8)

#define MUL33(a, b) ((fx32) (((fx64) (a) * (b)) >> 12))

void func_01ff81a0(const MtxFx33 *src, MtxFx33 *dst, fx32 x, fx32 y, fx32 z) {
    dst->_[0] = MUL33(x, src->_[0]);
    dst->_[1] = MUL33(x, src->_[1]);
    dst->_[2] = MUL33(x, src->_[2]);
    dst->_[3] = MUL33(y, src->_[3]);
    dst->_[4] = MUL33(y, src->_[4]);
    dst->_[5] = MUL33(y, src->_[5]);
    dst->_[6] = MUL33(z, src->_[6]);
    dst->_[7] = MUL33(z, src->_[7]);
    dst->_[8] = MUL33(z, src->_[8]);
}

#pragma thumb on
void func_01ff8260(MtxFx33 *mtx, fx32 sin, fx32 cos) {
    mtx->_[0] = 0x1000;
    mtx->_[1] = 0;
    mtx->_[2] = 0;
    mtx->_[3] = 0;
    mtx->_[4] = cos;
    mtx->_[5] = sin;
    mtx->_[6] = 0;
    mtx->_[7] = -sin;
    mtx->_[8] = cos;
}

// near match (zero kept in r2 instead of r3)
void func_01ff827c(MtxFx33 *mtx, fx32 sin, fx32 cos) {
    mtx->_[0] = cos;
    mtx->_[8] = cos;
    mtx->_[1] = 0;
    mtx->_[3] = 0;
    mtx->_[5] = 0;
    mtx->_[7] = 0;
    mtx->_[2] = -sin;
    mtx->_[6] = sin;
    mtx->_[4] = 0x1000;
}

// not matching (original uses stm; probably assembly)
void func_01ff8298(MtxFx33 *mtx, fx32 sin, fx32 cos) {
    fx32 *p = mtx->_;
    *p++ = cos;
    *p++ = sin;
    *p++ = 0;
    *p++ = -sin;
    *p++ = cos;
    p[0] = 0;
    p[1] = 0;
    p[2] = 0;
    p[3] = 0x1000;
}
#pragma thumb off

// not matching: original keeps b elements in registers and mixes umull/mla forms for two of the nine outputs
void func_01ff82b0(const MtxFx33 *a, const MtxFx33 *b, MtxFx33 *ab) {
    MtxFx33 tmp;
    MtxFx33 *p = (ab == b) ? &tmp : ab;
    fx32 a00 = a->_[0], a01 = a->_[1], a02 = a->_[2];
    p->_[0] = (fx32) (((fx64) a00 * b->_[0] + (fx64) a01 * b->_[3] + (fx64) a02 * b->_[6]) >> 12);
    p->_[1] = (fx32) (((fx64) a00 * b->_[1] + (fx64) a01 * b->_[4] + (fx64) a02 * b->_[7]) >> 12);
    p->_[2] = (fx32) (((fx64) a00 * b->_[2] + (fx64) a01 * b->_[5] + (fx64) a02 * b->_[8]) >> 12);
    {
        fx32 a10 = a->_[3], a11 = a->_[4], a12 = a->_[5];
        p->_[5] = (fx32) (((fx64) a10 * b->_[2] + (fx64) a11 * b->_[5] + (fx64) a12 * b->_[8]) >> 12);
        p->_[4] = (fx32) (((fx64) a10 * b->_[1] + (fx64) a11 * b->_[4] + (fx64) a12 * b->_[7]) >> 12);
        p->_[3] = (fx32) (((fx64) a10 * b->_[0] + (fx64) a11 * b->_[3] + (fx64) a12 * b->_[6]) >> 12);
    }
    {
        fx32 a20 = a->_[6], a21 = a->_[7], a22 = a->_[8];
        p->_[6] = (fx32) (((fx64) a20 * b->_[0] + (fx64) a21 * b->_[3] + (fx64) a22 * b->_[6]) >> 12);
        p->_[7] = (fx32) (((fx64) a20 * b->_[1] + (fx64) a21 * b->_[4] + (fx64) a22 * b->_[7]) >> 12);
        p->_[8] = (fx32) (((fx64) a20 * b->_[2] + (fx64) a21 * b->_[5] + (fx64) a22 * b->_[8]) >> 12);
    }
    if (p == &tmp) {
        *ab = tmp;
    }
}

void func_01ff84d0(const VecFx32 *v, const MtxFx33 *m, VecFx32 *out) {
    fx32 x = v->x, y = v->y, z = v->z;
    out->x = (fx32) (((fx64) x * m->_[0] + (fx64) y * m->_[3] + (fx64) z * m->_[6]) >> 12);
    out->y = (fx32) (((fx64) x * m->_[1] + (fx64) y * m->_[4] + (fx64) z * m->_[7]) >> 12);
    out->z = (fx32) (((fx64) x * m->_[2] + (fx64) y * m->_[5] + (fx64) z * m->_[8]) >> 12);
}

void func_01ff85a8(const MtxFx43 *src, MtxFx43 *dst, fx32 x, fx32 y, fx32 z) {
    func_01ff81a0((const MtxFx33 *) src, (MtxFx33 *) dst, x, y, z);
    dst->_[9] = src->_[9];
    dst->_[10] = src->_[10];
    dst->_[11] = src->_[11];
}

// not matching: same mixed umull/mla pattern as func_01ff82b0
void func_01ff89c0(const MtxFx43 *a, const MtxFx43 *b, MtxFx43 *ab) {
    MtxFx43 tmp;
    MtxFx43 *p = (ab == b) ? &tmp : ab;
    fx32 a00 = a->_[0], a01 = a->_[1], a02 = a->_[2];
    p->_[0] = (fx32) (((fx64) a00 * b->_[0] + (fx64) a01 * b->_[3] + (fx64) a02 * b->_[6]) >> 12);
    p->_[1] = (fx32) (((fx64) a00 * b->_[1] + (fx64) a01 * b->_[4] + (fx64) a02 * b->_[7]) >> 12);
    p->_[2] = (fx32) (((fx64) a00 * b->_[2] + (fx64) a01 * b->_[5] + (fx64) a02 * b->_[8]) >> 12);
    {
        fx32 a10 = a->_[3], a11 = a->_[4], a12 = a->_[5];
        p->_[5] = (fx32) (((fx64) a10 * b->_[2] + (fx64) a11 * b->_[5] + (fx64) a12 * b->_[8]) >> 12);
        p->_[4] = (fx32) (((fx64) a10 * b->_[1] + (fx64) a11 * b->_[4] + (fx64) a12 * b->_[7]) >> 12);
        p->_[3] = (fx32) (((fx64) a10 * b->_[0] + (fx64) a11 * b->_[3] + (fx64) a12 * b->_[6]) >> 12);
    }
    {
        fx32 a20 = a->_[6], a21 = a->_[7], a22 = a->_[8];
        p->_[6] = (fx32) (((fx64) a20 * b->_[0] + (fx64) a21 * b->_[3] + (fx64) a22 * b->_[6]) >> 12);
        p->_[7] = (fx32) (((fx64) a20 * b->_[1] + (fx64) a21 * b->_[4] + (fx64) a22 * b->_[7]) >> 12);
        p->_[8] = (fx32) (((fx64) a20 * b->_[2] + (fx64) a21 * b->_[5] + (fx64) a22 * b->_[8]) >> 12);
    }
    {
        fx32 t0 = a->_[9], t1 = a->_[10], t2 = a->_[11];
        p->_[11] = (fx32) (((fx64) t0 * b->_[2] + (fx64) t1 * b->_[5] + (fx64) t2 * b->_[8]) >> 12) + b->_[11];
        p->_[10] = (fx32) (((fx64) t0 * b->_[1] + (fx64) t1 * b->_[4] + (fx64) t2 * b->_[7]) >> 12) + b->_[10];
        p->_[9] = (fx32) (((fx64) t0 * b->_[0] + (fx64) t1 * b->_[3] + (fx64) t2 * b->_[6]) >> 12) + b->_[9];
    }
    if (p == &tmp) {
        *ab = tmp;
    }
}

void MTX_MultVec43(const VecFx32 *vec, const MtxFx43 *m, VecFx32 *dst) {
    fx32 x = vec->x, y = vec->y, z = vec->z;
    fx32 r;
    r = (fx32) (((fx64) x * m->_[0] + (fx64) y * m->_[3] + (fx64) z * m->_[6]) >> 12);
    dst->x = r;
    dst->x = r + m->_[9];
    r = (fx32) (((fx64) x * m->_[1] + (fx64) y * m->_[4] + (fx64) z * m->_[7]) >> 12);
    dst->y = r;
    dst->y = r + m->_[10];
    r = (fx32) (((fx64) x * m->_[2] + (fx64) y * m->_[5] + (fx64) z * m->_[8]) >> 12);
    dst->z = r;
    dst->z = r + m->_[11];
}

void func_01ff8d34(const VecFx32 *pos, const VecFx32 *up, const VecFx32 *target, MtxFx43 *mtx) {
    VecFx32 vz, vx, vy;
    vz.x = pos->x - target->x;
    vz.y = pos->y - target->y;
    vz.z = pos->z - target->z;
    VEC_Normalize(&vz, &vz);
    VEC_CrossProduct(up, &vz, &vx);
    VEC_Normalize(&vx, &vx);
    VEC_CrossProduct(&vz, &vx, &vy);
    mtx->_[0] = vx.x;
    mtx->_[1] = vy.x;
    mtx->_[2] = vz.x;
    mtx->_[3] = vx.y;
    mtx->_[4] = vy.y;
    mtx->_[5] = vz.y;
    mtx->_[6] = vx.z;
    mtx->_[7] = vy.z;
    mtx->_[8] = vz.z;
    mtx->_[9] = -VEC_DotProduct(pos, &vx);
    mtx->_[10] = -VEC_DotProduct(pos, &vy);
    mtx->_[11] = -VEC_DotProduct(pos, &vz);
}

void VEC_Add(const VecFx32 *a, const VecFx32 *b, VecFx32 *ab) {
    ab->x = a->x + b->x;
    ab->y = a->y + b->y;
    ab->z = a->z + b->z;
}

void VEC_Subtract(const VecFx32 *a, const VecFx32 *b, VecFx32 *ab) {
    ab->x = a->x - b->x;
    ab->y = a->y - b->y;
    ab->z = a->z - b->z;
}

fx32 VEC_DotProduct(const VecFx32 *a, const VecFx32 *b) {
    return (fx32) (((fx64) a->x * b->x + (fx64) a->y * b->y + (fx64) a->z * b->z + 0x800LL) >> 12);
}

// not matching: right size and schedule, registers differ
void VEC_CrossProduct(const VecFx32 *a, const VecFx32 *b, VecFx32 *axb) {
    fx32 ax = a->x, ay = a->y, az = a->z;
    fx32 bx = b->x, by = b->y, bz = b->z;
    axb->x = (fx32) (((fx64) ay * bz - (fx64) az * by + 0x800LL) >> 12);
    axb->y = (fx32) (((fx64) az * bx - (fx64) ax * bz + 0x800LL) >> 12);
    axb->z = (fx32) (((fx64) ax * by - (fx64) ay * bx + 0x800LL) >> 12);
}

fx32 VEC_Mag(const VecFx32 *v) {
    fx64 sum = (fx64) v->x * v->x;
    sum += (fx64) v->y * v->y;
    sum += (fx64) v->z * v->z;
    REG_SQRTCNT = 1;
    REG_SQRT_PARAM = sum << 2;
    while (REG_SQRTCNT & 0x8000) {
    }
    return (REG_SQRT_RESULT + 1) >> 1;
}

void VEC_Normalize(const VecFx32 *src, VecFx32 *dst) {
    fx64 sum = (fx64) src->x * src->x;
    fx64 t;
    s32 s;
    sum += (fx64) src->y * src->y;
    sum += (fx64) src->z * src->z;
    REG_DIVCNT = 2;
    REG_DIV_NUMER = 0x1000000LL << 32;
    REG_DIV_DENOM = sum;
    REG_SQRTCNT = 1;
    REG_SQRT_PARAM = sum << 2;
    while (REG_SQRTCNT & 0x8000) {
    }
    s = REG_SQRT_RESULT;
    while (REG_DIVCNT & 0x8000) {
    }
    t = REG_DIV_RESULT * s;
    dst->x = (fx32) ((t * src->x + (0x1000LL << 32)) >> 45);
    dst->y = (fx32) ((t * src->y + (0x1000LL << 32)) >> 45);
    dst->z = (fx32) ((t * src->z + (0x1000LL << 32)) >> 45);
}

void func_01ff90dc(const s16 *src, s16 *dst) {
    fx64 sum = (fx64) (src[0] * src[0]);
    fx64 t;
    s32 s;
    sum += (fx64) (src[1] * src[1]);
    sum += (fx64) (src[2] * src[2]);
    REG_DIVCNT = 2;
    REG_DIV_NUMER = 0x1000000LL << 32;
    REG_DIV_DENOM = sum;
    REG_SQRTCNT = 1;
    REG_SQRT_PARAM = sum << 2;
    while (REG_SQRTCNT & 0x8000) {
    }
    s = REG_SQRT_RESULT;
    while (REG_DIVCNT & 0x8000) {
    }
    t = REG_DIV_RESULT * s;
    dst[0] = (s16) ((t * src[0] + (0x1000LL << 32)) >> 45);
    dst[1] = (s16) ((t * src[1] + (0x1000LL << 32)) >> 45);
    dst[2] = (s16) ((t * src[2] + (0x1000LL << 32)) >> 45);
}

fx32 func_01ff9208(const VecFx32 *a, const VecFx32 *b) {
    fx32 dx = a->x - b->x;
    fx32 dy = a->y - b->y;
    fx32 dz = a->z - b->z;
    fx64 sum = (fx64) dx * dx;
    sum += (fx64) dy * dy;
    sum += (fx64) dz * dz;
    REG_SQRTCNT = 1;
    REG_SQRT_PARAM = sum << 2;
    while (REG_SQRTCNT & 0x8000) {
    }
    return (REG_SQRT_RESULT + 1) >> 1;
}

int func_01ff8634(const MtxFx43 *src, MtxFx43 *dst) {
    MtxFx43 tmp;
    MtxFx43 *p = (src == dst) ? &tmp : dst;
    fx64 A, B, C;
    fx32 D, E, F, G, det, inv;
    const fx32 *m = src->_;
    A = (fx32) (((fx64) m[3] * m[8] - (fx64) m[5] * m[6] + 0x800) >> 12);
    B = (fx32) (((fx64) m[4] * m[8] - (fx64) m[5] * m[7] + 0x800) >> 12);
    C = (fx32) (((fx64) m[3] * m[7] - (fx64) m[4] * m[6] + 0x800) >> 12);
    det = (fx32) (((fx64) m[0] * B - (fx64) m[1] * A + (fx64) m[2] * C + 0x800) >> 12);
    if (det == 0) {
        return -1;
    }
    func_01ff80f8(det);
    D = (fx32) (((fx64) m[1] * m[8] - (fx64) m[7] * m[2]) >> 12);
    E = (fx32) (((fx64) m[1] * m[5] - (fx64) m[4] * m[2]) >> 12);
    F = (fx32) (((fx64) m[0] * m[8] - (fx64) m[6] * m[2]) >> 12);
    G = (fx32) (((fx64) m[0] * m[5] - (fx64) m[3] * m[2]) >> 12);
    inv = func_01ff80c0();
    p->_[0] = (fx32) (((fx64) inv * B) >> 12);
    p->_[1] = NEG((fx32) (((fx64) inv * D) >> 12));
    p->_[2] = (fx32) (((fx64) inv * E) >> 12);
    p->_[3] = NEG((fx32) (((fx64) inv * A) >> 12));
    p->_[4] = (fx32) (((fx64) inv * F) >> 12);
    p->_[5] = NEG((fx32) (((fx64) inv * G) >> 12));
    p->_[6] = (fx32) (((fx64) inv * C) >> 12);
    p->_[7] = NEG((fx32) (((fx64) inv * (fx32) (((fx64) m[0] * m[7] - (fx64) m[6] * m[1]) >> 12)) >> 12));
    p->_[8] = (fx32) (((fx64) inv * (fx32) (((fx64) m[0] * m[4] - (fx64) m[3] * m[1]) >> 12)) >> 12);
    p->_[9] = NEG((fx32) (((fx64) p->_[0] * m[9] + (fx64) p->_[3] * m[10] + (fx64) p->_[6] * m[11]) >> 12));
    p->_[10] = NEG((fx32) (((fx64) p->_[1] * m[9] + (fx64) p->_[4] * m[10] + (fx64) p->_[7] * m[11]) >> 12));
    p->_[11] = NEG((fx32) (((fx64) p->_[2] * m[9] + (fx64) p->_[5] * m[10] + (fx64) p->_[8] * m[11]) >> 12));
    if (p == &tmp) {
        func_01ff95e0(&tmp, dst);
    }
    return 0;
}
