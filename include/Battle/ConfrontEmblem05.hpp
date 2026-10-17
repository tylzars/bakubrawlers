#pragma once

#include "global.h"
#include "types.h"

#include "Graphics/CellAnimation.hpp"

// Orbiting emblem sprite (vs_emblem_NN_nce.bin) of the confrontation screen 05 (0x18 bytes).
class ConfrontEmblem05 {
public:
    /* 00 (vtable 0228627c) */
    /* 04 */ u8 mUnk_04_0 : 1; // direction of the sweep
    /* 05 */ PAD(0x05, 0x08);
    /* 08 */ u16 mAngle;
    /* 0a */ u16 mSpeed;
    /* 0c */ float mX;
    /* 10 */ float mY;
    /* 14 */ CellAnimation *mAnim;
    /* 18 */

    ConfrontEmblem05(u8 emblem);
    virtual ~ConfrontEmblem05();
    void Update();
    void Draw();
    int func_ov007_02220b9c(int fast);
    void SlowDown();
};
