#include <nitro/internal.h>

void G2x_SetBGyAffine_(vu16 *reg, const MtxFx22 *mtx, int centerX, int centerY, int x1, int y1) {
    vu32 *r = (vu32 *) reg;
    int dx, dy, x, y;
    r[0] = (u16) (s16) (mtx->_[0] >> 4) | ((u16) (s16) (mtx->_[1] >> 4) << 16);
    r[1] = (u16) (s16) (mtx->_[2] >> 4) | ((u16) (s16) (mtx->_[3] >> 4) << 16);
    dx = x1 - centerX;
    dy = y1 - centerY;
    x = mtx->_[0] * dx + mtx->_[1] * dy + (centerX << 12);
    y = mtx->_[2] * dx + mtx->_[3] * dy + (centerY << 12);
    r[2] = x >> 4;
    r[3] = y >> 4;
}

void G2x_SetBlendAlpha_(vu16 *reg, int plane1, int plane2, int ev1, int ev2) {
    *(vu32 *) reg = (plane1 | 0x40 | (plane2 << 8)) | ((ev1 | (ev2 << 8)) << 16);
}

void G2x_SetBlendBrightness_(vu16 *reg, int plane, int brightness) {
    if (brightness < 0) {
        *reg = (u16) (plane | 0xc0);
        reg[2] = (u16) -brightness;
    } else {
        *reg = (u16) (plane | 0x80);
        reg[2] = (u16) brightness;
    }
}
