#include <nitro/internal.h>

void MTX_Concat22(const MtxFx22 *a, const MtxFx22 *b, MtxFx22 *ab) {
    MtxFx22 tmp;
    MtxFx22 *p = (ab == b) ? &tmp : ab;
    fx32 a0, a1, a2, a3;
    a0 = a->_[0];
    a1 = a->_[1];
    p->_[0] = (fx32) (((fx64) a0 * b->_[0] + (fx64) a1 * b->_[2]) >> 12);
    p->_[1] = (fx32) (((fx64) a0 * b->_[1] + (fx64) a1 * b->_[3]) >> 12);
    a2 = a->_[2];
    a3 = a->_[3];
    p->_[2] = (fx32) (((fx64) a2 * b->_[0] + (fx64) a3 * b->_[2]) >> 12);
    p->_[3] = (fx32) (((fx64) a2 * b->_[1] + (fx64) a3 * b->_[3]) >> 12);
    if (p == &tmp) {
        *ab = tmp;
    }
}
