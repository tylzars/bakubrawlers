// Fixed-point matrix and vector math in ITCM: 0x01ff81a0-0x01ff9208.
// 0x01ff817c, 0x01ff854c, 0x01ff8574, 0x01ff85dc, 0x01ff85fc, 0x01ff8618 (stm-heavy identity/rotation fills) are hand assembly.
#include <nitro/internal.h>

// func_01ff817c: writes a 3x3 fixed-point identity matrix to r0 (nine words,
// 0x1000 on the diagonal). Element 8 is stored first, then the rest in order.
asm void func_01ff817c(void *m) {
    mov r2, #0x1000
    str r2, [r0, #32]           // m[8] = 1.0
    mov r3, #0
    stmia r0!, {r2, r3}         // m[0]=1.0, m[1]=0
    mov r1, #0
    stmia r0!, {r1, r3}         // m[2]=0, m[3]=0
    stmia r0!, {r2, r3}         // m[4]=1.0, m[5]=0
    stmia r0!, {r1, r3}         // m[6]=0, m[7]=0
    bx lr
}
extern void FX_InvAsync(fx32 x);
extern fx32 FX_GetDivResult(void);
extern void func_01ff95e0(const void *src, void *dst);   // 48-byte copy

#define REG_DIVCNT (*(vu16 *) 0x04000280)
#define REG_DIV_NUMER (*(vs64 *) 0x04000290)
#define REG_DIV_DENOM (*(vs64 *) 0x04000298)
#define REG_DIV_RESULT (*(s64 *) 0x040002a0)
#define REG_SQRTCNT (*(vu16 *) 0x040002b0)
#define REG_SQRT_RESULT (*(s32 *) 0x040002b4)
#define REG_SQRT_PARAM (*(s64 *) 0x040002b8)

#define MUL33(a, b) ((fx32) (((fx64) (a) * (b)) >> 12))

void MTX_ApplyScale33(const MtxFx33 *src, MtxFx33 *dst, fx32 x, fx32 y, fx32 z) {
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

asm void func_01ff827c(MtxFx33 *m, fx32 sin, fx32 cos) {
    str r2, [r0, #0]
    str r2, [r0, #0x20]
    mov r3, #0
    str r3, [r0, #4]
    str r3, [r0, #0xc]
    str r3, [r0, #0x14]
    str r3, [r0, #0x1c]
    neg r2, r1
    mov r3, #1
    lsl r3, r3, #12
    str r1, [r0, #0x18]
    str r2, [r0, #8]
    str r3, [r0, #0x10]
    bx lr
}

asm void MTX_RotZ33_(MtxFx33 *m, fx32 sin, fx32 cos) {
    stmia r0!, {r2}
    mov r3, #0
    stmia r0!, {r1, r3}
    neg r1, r1
    stmia r0!, {r1, r2}
    mov r1, #1
    lsl r1, r1, #12
    str r3, [r0, #0]
    str r3, [r0, #4]
    str r3, [r0, #8]
    str r1, [r0, #0xc]
    bx lr
}
#pragma thumb off

void func_01ff82b0(const MtxFx33 *a, const MtxFx33 *b, MtxFx33 *ab) {
    MtxFx33 tmp;
    MtxFx33 *p;
    fx32 x, y, z;
    fx32 xx, yy, zz;

    if (ab == b) {
        p = &tmp;
    } else {
        p = ab;
    }
    x = a->_[0];
    y = a->_[1];
    z = a->_[2];
    p->_[0] = (fx32) (((fx64) x * b->_[0] + (fx64) y * b->_[3] + (fx64) z * b->_[6]) >> FX32_SHIFT);
    p->_[1] = (fx32) (((fx64) x * b->_[1] + (fx64) y * b->_[4] + (fx64) z * b->_[7]) >> FX32_SHIFT);
    xx = b->_[2];
    yy = b->_[5];
    zz = b->_[8];
    p->_[2] = (fx32) (((fx64) x * xx + (fx64) y * yy + (fx64) z * zz) >> FX32_SHIFT);
    x = a->_[3];
    y = a->_[4];
    z = a->_[5];
    p->_[5] = (fx32) (((fx64) x * xx + (fx64) y * yy + (fx64) z * zz) >> FX32_SHIFT);
    p->_[4] = (fx32) (((fx64) x * b->_[1] + (fx64) y * b->_[4] + (fx64) z * b->_[7]) >> FX32_SHIFT);
    xx = b->_[0];
    yy = b->_[3];
    zz = b->_[6];
    p->_[3] = (fx32) (((fx64) x * xx + (fx64) y * yy + (fx64) z * zz) >> FX32_SHIFT);
    x = a->_[6];
    y = a->_[7];
    z = a->_[8];
    p->_[6] = (fx32) (((fx64) x * xx + (fx64) y * yy + (fx64) z * zz) >> FX32_SHIFT);
    p->_[7] = (fx32) (((fx64) x * b->_[1] + (fx64) y * b->_[4] + (fx64) z * b->_[7]) >> FX32_SHIFT);
    p->_[8] = (fx32) (((fx64) x * b->_[2] + (fx64) y * b->_[5] + (fx64) z * b->_[8]) >> FX32_SHIFT);

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

asm void func_01ff854c(MtxFx43 *m) {
    mov r2, #0x1000
    mov r3, #0
    stmia r0!, {r2, r3}
    mov r1, #0
    stmia r0!, {r1, r3}
    stmia r0!, {r2, r3}
    stmia r0!, {r1, r3}
    stmia r0!, {r2, r3}
    stmia r0!, {r1, r3}
    bx lr
}

asm void MTX_Copy43To44(const MtxFx43 *src, void *dst) {
    stmfd sp!, {r4}
    mov ip, #0
    ldmia r0!, {r2, r3, r4}
    stmia r1!, {r2, r3, r4, ip}
    ldmia r0!, {r2, r3, r4}
    stmia r1!, {r2, r3, r4, ip}
    ldmia r0!, {r2, r3, r4}
    stmia r1!, {r2, r3, r4, ip}
    mov ip, #0x1000
    ldmia r0!, {r2, r3, r4}
    stmia r1!, {r2, r3, r4, ip}
    ldmfd sp!, {r4}
    bx lr
}

void MTX_ApplyScale43(const MtxFx43 *src, MtxFx43 *dst, fx32 x, fx32 y, fx32 z) {
    MTX_ApplyScale33((const MtxFx33 *) src, (MtxFx33 *) dst, x, y, z);
    dst->_[9] = src->_[9];
    dst->_[10] = src->_[10];
    dst->_[11] = src->_[11];
}

#pragma thumb on
asm void MTX_RotX43_(MtxFx43 *m, fx32 sin, fx32 cos) {
    str r1, [r0, #0x14]
    neg r1, r1
    str r1, [r0, #0x1c]
    mov r1, #1
    lsl r1, r1, #12
    stmia r0!, {r1}
    mov r3, #0
    mov r1, #0
    stmia r0!, {r1, r3}
    stmia r0!, {r1, r2}
    str r1, [r0, #4]
    add r0, #0xc
    stmia r0!, {r2, r3}
    stmia r0!, {r1, r3}
    bx lr
}
asm void MTX_RotY43_(MtxFx43 *m, fx32 sin, fx32 cos) {
    str r1, [r0, #0x18]
    mov r3, #0
    stmia r0!, {r2, r3}
    neg r1, r1
    stmia r0!, {r1, r3}
    mov r1, #1
    lsl r1, r1, #12
    stmia r0!, {r1, r3}
    add r0, #4
    mov r1, #0
    stmia r0!, {r1, r2, r3}
    stmia r0!, {r1, r3}
    bx lr
}
asm void MTX_RotZ43_(MtxFx43 *m, fx32 sin, fx32 cos) {
    stmia r0!, {r2}
    mov r3, #0
    stmia r0!, {r1, r3}
    neg r1, r1
    stmia r0!, {r1, r2, r3}
    mov r1, #0
    mov r2, #0
    mov r3, #1
    lsl r3, r3, #12
    stmia r0!, {r1, r2, r3}
    mov r3, #0
    stmia r0!, {r1, r2, r3}
    bx lr
}
#pragma thumb off

// a*d - b*c on fx32 inputs, kept as a 64-bit product sum (no shift yet).
#define CROSS64(a, d, b, c) ((fx64) (a) * (d) - (fx64) (b) * (c))
// Back from a 64-bit product sum to fx32: truncated, or rounded to the nearest 1/4096.
#define TRUNC12(x) ((fx32) ((x) >> 12))
#define ROUND12(x) ((fx32) (((x) + (fx64) 0x800) >> 12))

// MTX_Inverse43: inverts the 3x3 part through its 2x2 minors and the determinant, then sets the translation row to
// minus the old translation times the new 3x3 part. Elements are numbered row by row (0-8 the 3x3 part, 9-11 the
// translation). Returns -1 and leaves dst alone when the determinant is zero, 0 otherwise. src may equal dst; the
// result then goes through a stack copy.
int func_01ff8634(const MtxFx43 *src, MtxFx43 *dst) {
    MtxFx43 tmp;
    MtxFx43 *out;
    fx32 det;                  // the determinant, later its reciprocal
    fx32 minor0, minor1, minor2;
    fx32 minor3, minor6, minor4, minor7;   // named after the element each one ends up in (transposed)

    if (src == dst) {
        out = &tmp;
    } else {
        out = dst;
    }

    // Minors of the first row (rounded), then the determinant by expansion along that row.
    minor0 = ROUND12(CROSS64(src->_[4], src->_[8], src->_[5], src->_[7]));
    minor1 = ROUND12(CROSS64(src->_[3], src->_[8], src->_[5], src->_[6]));
    minor2 = ROUND12(CROSS64(src->_[3], src->_[7], src->_[4], src->_[6]));

    det = ROUND12(CROSS64(src->_[0], minor0, src->_[1], minor1) + (fx64) src->_[2] * minor2);

    if (det == 0) {
        return -1;
    }

    // Start 1/det on the divider and fill the wait with four more minors (truncated).
    FX_InvAsync(det);

    minor3 = TRUNC12(CROSS64(src->_[1], src->_[8], src->_[7], src->_[2]));
    minor6 = TRUNC12(CROSS64(src->_[1], src->_[5], src->_[4], src->_[2]));
    minor4 = TRUNC12(CROSS64(src->_[0], src->_[8], src->_[6], src->_[2]));
    minor7 = TRUNC12(CROSS64(src->_[0], src->_[5], src->_[3], src->_[2]));

    // Adjugate times 1/det, with the checkerboard signs; the last two minors are formed in place.
    det = FX_GetDivResult();
    out->_[0] = MUL33(det, minor0);
    out->_[1] = -MUL33(det, minor3);
    out->_[2] = MUL33(det, minor6);

    out->_[3] = -MUL33(det, minor1);
    out->_[4] = MUL33(det, minor4);
    out->_[5] = -MUL33(det, minor7);

    out->_[6] = MUL33(det, minor2);
    out->_[7] = -MUL33(det, TRUNC12(CROSS64(src->_[0], src->_[7], src->_[6], src->_[1])));
    out->_[8] = MUL33(det, TRUNC12(CROSS64(src->_[0], src->_[4], src->_[3], src->_[1])));

    // New translation: -(old translation) * (new 3x3 part), one column at a time.
    out->_[9] = -TRUNC12((fx64) out->_[0] * src->_[9] + (fx64) out->_[3] * src->_[10] + (fx64) out->_[6] * src->_[11]);
    out->_[10] = -TRUNC12((fx64) out->_[1] * src->_[9] + (fx64) out->_[4] * src->_[10] + (fx64) out->_[7] * src->_[11]);
    out->_[11] = -TRUNC12((fx64) out->_[2] * src->_[9] + (fx64) out->_[5] * src->_[10] + (fx64) out->_[8] * src->_[11]);

    if (out == &tmp) {
        func_01ff95e0(&tmp, dst);
    }
    return 0;
}

#undef CROSS64
#undef TRUNC12
#undef ROUND12

void func_01ff89c0(const MtxFx43 *a, const MtxFx43 *b, MtxFx43 *ab) {
    MtxFx43 tmp;
    MtxFx43 *p;
    fx32 x, y, z;
    fx32 xx, yy, zz;

    if (ab == b) {
        p = &tmp;
    } else {
        p = ab;
    }
    x = a->_[0];
    y = a->_[1];
    z = a->_[2];
    p->_[0] = (fx32) (((fx64) x * b->_[0] + (fx64) y * b->_[3] + (fx64) z * b->_[6]) >> FX32_SHIFT);
    p->_[1] = (fx32) (((fx64) x * b->_[1] + (fx64) y * b->_[4] + (fx64) z * b->_[7]) >> FX32_SHIFT);
    xx = b->_[2];
    yy = b->_[5];
    zz = b->_[8];
    p->_[2] = (fx32) (((fx64) x * xx + (fx64) y * yy + (fx64) z * zz) >> FX32_SHIFT);
    x = a->_[3];
    y = a->_[4];
    z = a->_[5];
    p->_[5] = (fx32) (((fx64) x * xx + (fx64) y * yy + (fx64) z * zz) >> FX32_SHIFT);
    p->_[4] = (fx32) (((fx64) x * b->_[1] + (fx64) y * b->_[4] + (fx64) z * b->_[7]) >> FX32_SHIFT);
    xx = b->_[0];
    yy = b->_[3];
    zz = b->_[6];
    p->_[3] = (fx32) (((fx64) x * xx + (fx64) y * yy + (fx64) z * zz) >> FX32_SHIFT);
    x = a->_[6];
    y = a->_[7];
    z = a->_[8];
    p->_[6] = (fx32) (((fx64) x * xx + (fx64) y * yy + (fx64) z * zz) >> FX32_SHIFT);
    p->_[7] = (fx32) (((fx64) x * b->_[1] + (fx64) y * b->_[4] + (fx64) z * b->_[7]) >> FX32_SHIFT);
    xx = b->_[2];
    yy = b->_[5];
    zz = b->_[8];
    p->_[8] = (fx32) (((fx64) x * xx + (fx64) y * yy + (fx64) z * zz) >> FX32_SHIFT);
    x = a->_[9];
    y = a->_[10];
    z = a->_[11];
    p->_[11] = (fx32) ((((fx64) x * xx + (fx64) y * yy + (fx64) z * zz) >> FX32_SHIFT) + b->_[11]);
    p->_[10] = (fx32) ((((fx64) x * b->_[1] + (fx64) y * b->_[4] + (fx64) z * b->_[7]) >> FX32_SHIFT) + b->_[10]);
    xx = b->_[0];
    yy = b->_[3];
    zz = b->_[6];
    p->_[9] = (fx32) ((((fx64) x * xx + (fx64) y * yy + (fx64) z * zz) >> FX32_SHIFT) + b->_[9]);

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

void MTX_LookAt(const VecFx32 *pos, const VecFx32 *up, const VecFx32 *target, MtxFx43 *mtx) {
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

void VEC_CrossProduct(const VecFx32 *a, const VecFx32 *b, VecFx32 *axb) {
    fx32 x = (fx32) (((fx64) a->y * b->z - (fx64) a->z * b->y + 0x800LL) >> 12);
    fx32 y = (fx32) (((fx64) a->z * b->x - (fx64) a->x * b->z + 0x800LL) >> 12);
    fx32 z = (fx32) (((fx64) a->x * b->y - (fx64) a->y * b->x + 0x800LL) >> 12);
    axb->x = x;
    axb->y = y;
    axb->z = z;
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

void VEC_NormalizeS16(const s16 *src, s16 *dst) {
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

fx32 VEC_Distance(const VecFx32 *a, const VecFx32 *b) {
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

