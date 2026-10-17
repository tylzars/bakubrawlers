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
    /* 07 */ u8 mUnk_07_1 : 1;
    /* 07 */ u8 mUnk_07_2 : 1;
    /* 07 */ u8 mShrinkStarted : 1;
    /* 08 */ u8 mTimer;
    /* 09 */ u8 mPhase;
    /* 0a */ PAD(0x0a, 0x0c);
    /* 0c */ s32 mPos;
    /* 10 */ float mAngle;
    /* 14 */ float mScale;
    /* 18 */ s32 mSpeed;
    /* 1c */ s32 mAccel;
    /* 20 */ CellAnimation *mAnim;
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
