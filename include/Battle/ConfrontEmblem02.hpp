#pragma once

#include "global.h"
#include "types.h"

class CellAnimation;

// Emblem sprite that drops onto the confrontation screen (vs_emblem_NN_nce.bin), used by ConfrontBg02. 0x24 bytes.
class ConfrontEmblem02 {
public:
    /* 04 */ u8 mState;
    /* 05 */ u8 mSide;
    /* 06 */ u8 mEmblem;
    /* 07 */ u8 mTouched : 1;
    /* 07 */ u8 mDrawLate : 1;
    /* 07 */ u8 mCenterReported : 1;
    /* 07 */ u8 mShrinkStarted : 1;
    /* 08 */ u8 mStepFrames;
    /* 09 */ u8 mStep;
    /* 0a */ PAD(0x0a, 0x0c);
    /* 0c */ s32 mDistance;
    /* 10 */ float mX;
    /* 14 */ float mScale;
    /* 18 */ s32 mSpeed;
    /* 1c */ s32 mAccel;
    /* 20 */ CellAnimation *mCellAnim;
    /* 24 */

    ConfrontEmblem02(u8 emblem);
    virtual ~ConfrontEmblem02();
    void Start(u8 side);
    void Update();
    void StartShrink();
    void Draw();
    u8 IsIdle();
    int func_ov007_0221e268(u8 flag);
};
