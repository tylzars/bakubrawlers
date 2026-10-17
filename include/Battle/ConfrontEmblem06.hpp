#pragma once

#include "global.h"
#include "types.h"

#include "Graphics/CellAnimation.hpp"

// Floating emblem sprite of the confrontation screen 06 (vs_emblem_NN plus vs_b_obj_06_burst). 0x28 bytes, vtable 0228642c.
class ConfrontEmblem06 {
public:
    /* 04 */ u8 mState;
    /* 05 */ u8 mTouchHeld : 1;
    /* 05 */ u8 mShakeRight : 1;
    /* 05 */ u8 mBurstStarted : 1;
    /* 05 */ u8 mStopped : 1;
    /* 05 */ u8 mMuted : 1;
    /* 06 */ u8 mShakeFrames;
    /* 07 */ PAD(0x07, 0x08);
    /* 08 */ float mScale;
    /* 0c */ float mSpeed;
    /* 10 */ float mX;
    /* 14 */ float mY;
    /* 18 */ float mDirX;
    /* 1c */ float mDirY;
    /* 20 */ CellAnimation *mAnims[2];
    /* 28 */

    ConfrontEmblem06(u8 index);
    virtual ~ConfrontEmblem06();
    void Update();
    void Draw();
    void PickDir();
    void Bounce();
    int func_ov007_02221f2c(int a);
    int func_ov007_0222210c();
    void EaseSpeed();
    u8 IsBursting();
    u8 TakeBurstStarted();
};
