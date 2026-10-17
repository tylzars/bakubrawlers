#pragma once

#include "global.h"
#include "types.h"

#include "Battle/BattleBase.hpp"

class CellAnimation;

struct BattleObj01Motion {
    /* 00 */ float mStartSpeed;
    /* 04 */ float mLimit;
    /* 08 */ float mDecel;
    /* 0c */ float mSideSpeed;
    /* 10 */ float mSideDecel;
};

// A sprite pair (vs_b_obj_01) that flies from a start point towards a target.
class BattleObj01 : public BattleObject {
public:
    /* 04 */ u8 mActive : 1;
    /* 05 */ u8 mUnk_05;
    /* 06 */ PAD(0x06, 0x08);
    /* 08 */ float mSpeed;
    /* 0c */ float mSideSpeed;
    /* 10 */ float mX;
    /* 14 */ float mY;
    /* 18 */ s16 mScreenX;
    /* 1a */ s16 mScreenY;
    /* 1c */ float mTargetX;
    /* 20 */ float mTargetY;
    /* 24 */ float mDirX;
    /* 28 */ float mDirY;
    /* 2c */ float mSideX;
    /* 30 */ float mSideY;
    /* 34 */ CellAnimation *mAnims[2];
    /* 3c */ BattleObj01Motion *mMotion;
    /* 40 */

    BattleObj01(u8 layer, u32 charBase);
    ~BattleObj01();
    BOOL Start(u8 fast, int x, int y, s16 targetX, s16 targetY, u16 angle, int motion);
    BOOL Update();
};
