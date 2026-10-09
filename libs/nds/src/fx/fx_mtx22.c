#include <nitro/internal.h>

void MTX_Concat22(const MtxFx22 *a, const MtxFx22 *b, MtxFx22 *ab) {
    MtxFx22 tmp;
    MtxFx22 *p = (ab == b) ? &tmp : ab;
    fx32 x, y;
    x = a->_[0];
    y = a->_[1];
    p->_[0] = (fx32) (((fx64) x * b->_[0] + (fx64) y * b->_[2]) >> 12);
    p->_[1] = (fx32) (((fx64) x * b->_[1] + (fx64) y * b->_[3]) >> 12);
    x = a->_[2];
    y = a->_[3];
    p->_[2] = (fx32) (((fx64) x * b->_[0] + (fx64) y * b->_[2]) >> 12);
    p->_[3] = (fx32) (((fx64) x * b->_[1] + (fx64) y * b->_[3]) >> 12);
    if (p == &tmp) {
        *ab = tmp;
    }
}
