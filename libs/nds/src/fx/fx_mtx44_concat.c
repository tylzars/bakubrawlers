#include <nitro/internal.h>

void MTX_Concat44(const MtxFx44 *a, const MtxFx44 *b, MtxFx44 *ab) {
    MtxFx44 tmp;
    MtxFx44 *p;
    fx32 x, y, z, w;
    fx32 xx, yy, zz, ww;

    if (ab == b) {
        p = &tmp;
    } else {
        p = ab;
    }
    x = a->_[0];
    y = a->_[1];
    z = a->_[2];
    w = a->_[3];
    p->_[0] = (fx32) (((fx64) x * b->_[0] + (fx64) y * b->_[4] + (fx64) z * b->_[8] + (fx64) w * b->_[12]) >> FX32_SHIFT);
    p->_[1] = (fx32) (((fx64) x * b->_[1] + (fx64) y * b->_[5] + (fx64) z * b->_[9] + (fx64) w * b->_[13]) >> FX32_SHIFT);
    p->_[3] = (fx32) (((fx64) x * b->_[3] + (fx64) y * b->_[7] + (fx64) z * b->_[11] + (fx64) w * b->_[15]) >> FX32_SHIFT);
    xx = b->_[2];
    yy = b->_[6];
    zz = b->_[10];
    ww = b->_[14];
    p->_[2] = (fx32) (((fx64) x * xx + (fx64) y * yy + (fx64) z * zz + (fx64) w * ww) >> FX32_SHIFT);
    x = a->_[4];
    y = a->_[5];
    z = a->_[6];
    w = a->_[7];
    p->_[6] = (fx32) (((fx64) x * xx + (fx64) y * yy + (fx64) z * zz + (fx64) w * ww) >> FX32_SHIFT);
    p->_[5] = (fx32) (((fx64) x * b->_[1] + (fx64) y * b->_[5] + (fx64) z * b->_[9] + (fx64) w * b->_[13]) >> FX32_SHIFT);
    p->_[7] = (fx32) (((fx64) x * b->_[3] + (fx64) y * b->_[7] + (fx64) z * b->_[11] + (fx64) w * b->_[15]) >> FX32_SHIFT);
    xx = b->_[0];
    yy = b->_[4];
    zz = b->_[8];
    ww = b->_[12];
    p->_[4] = (fx32) (((fx64) x * xx + (fx64) y * yy + (fx64) z * zz + (fx64) w * ww) >> FX32_SHIFT);
    x = a->_[8];
    y = a->_[9];
    z = a->_[10];
    w = a->_[11];
    p->_[8] = (fx32) (((fx64) x * xx + (fx64) y * yy + (fx64) z * zz + (fx64) w * ww) >> FX32_SHIFT);
    p->_[9] = (fx32) (((fx64) x * b->_[1] + (fx64) y * b->_[5] + (fx64) z * b->_[9] + (fx64) w * b->_[13]) >> FX32_SHIFT);
    p->_[11] = (fx32) (((fx64) x * b->_[3] + (fx64) y * b->_[7] + (fx64) z * b->_[11] + (fx64) w * b->_[15]) >> FX32_SHIFT);
    xx = b->_[2];
    yy = b->_[6];
    zz = b->_[10];
    ww = b->_[14];
    p->_[10] = (fx32) (((fx64) x * xx + (fx64) y * yy + (fx64) z * zz + (fx64) w * ww) >> FX32_SHIFT);
    x = a->_[12];
    y = a->_[13];
    z = a->_[14];
    w = a->_[15];
    p->_[14] = (fx32) (((fx64) x * xx + (fx64) y * yy + (fx64) z * zz + (fx64) w * ww) >> FX32_SHIFT);
    p->_[13] = (fx32) (((fx64) x * b->_[1] + (fx64) y * b->_[5] + (fx64) z * b->_[9] + (fx64) w * b->_[13]) >> FX32_SHIFT);
    p->_[12] = (fx32) (((fx64) x * b->_[0] + (fx64) y * b->_[4] + (fx64) z * b->_[8] + (fx64) w * b->_[12]) >> FX32_SHIFT);
    p->_[15] = (fx32) (((fx64) x * b->_[3] + (fx64) y * b->_[7] + (fx64) z * b->_[11] + (fx64) w * b->_[15]) >> FX32_SHIFT);

    if (p == &tmp) {
        *ab = tmp;
    }
}
