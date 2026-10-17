#pragma once

#include "global.h"
#include "types.h"

#include "Graphics/CellAnimation.hpp"

// Emblem sprite shown by Confront03 (vs_emblem_%02d_nce.bin). Vtable 02286068.
class ConfrontEmblem03 {
public:
    /* 04 */ u8 mState;
    /* 05 */ u8 mEmblem;
    /* 06 */ PAD(0x06, 0x07);
    /* 07 */ u8 mTouched : 1;
    /* 07 */ u8 mUnk_07_1 : 1;
    /* 07 */ u8 mShrinkStarted : 1;
    /* 08 */ u8 mHoldFrames;
    /* 09 */ u8 mHoldFrameCount;
    /* 0a */ u8 mStep;
    /* 0b */ s8 mSlot;
    /* 0c */ PAD(0x0c, 0x14);
    /* 14 */ float mX;
    /* 18 */ float mY;
    /* 1c */ float mScale;
    /* 20 */ CellAnimation *mCellAnim;
    /* 24 */

    ConfrontEmblem03(u8 index);
    virtual ~ConfrontEmblem03();
    void Start(u8 slot, int flag);
    void Update();
    void StartShrink();
    void Draw();
    u8 IsIdle();
    int func_ov007_0221f594();
    int GetSlotIfIdle();
};
