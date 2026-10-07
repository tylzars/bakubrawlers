#pragma once

#include "global.h"
#include "types.h"

#include "Scene/SceneBase.hpp"

class CellAnimation;
class SelectWindow;
struct MessageFile;

// Reward screen shown after a battle (0xe8 bytes, vtable 0228b23c, created by BattleResult): counts the amount
// earned into data_020d43f0.mMoney with coin animations and a printed number.
class BattleUnk022767c8 : public SceneBase {
public:
    /* 00 SceneBase */
    /* 0c */ CellAnimation *mAnims[9];
    /* 30 */ CellAnimation *mAnimsB[15];
    /* 6c */ SelectWindow *mWindow;
    /* 70 */ MessageFile *mMessages;
    /* 74 */ void *mTextBuffer;
    /* 78 */ void *mGlyphBuffer;
    /* 7c */ u32 mTextSize;
    /* 80 */ u32 mGlyphCount;
    /* 84 */ u32 mAmount; // still to be counted into mTotal
    /* 88 */ u32 mTotal;
    /* 8c */ s32 mTimer;
    /* 90 */ s16 mSlideA;
    /* 92 */ s16 mSlideB;
    /* 94 */ u16 mScroll;
    /* 96 */ s16 mHold;
    /* 98 */ u16 mFrames[6];
    /* a4 */ u8 mUnk_a4;
    /* a5 */ u8 mState;
    /* a6 */ u8 mKind;
    /* a7 */ s8 mUnk_a7;
    /* a8 */ u8 mFade;
    /* a9 */ u8 mUnk_a9;
    /* aa */ s8 mUnk_aa;
    /* ab */ s8 mUnk_ab;
    /* ac */ u8 mPos[15][2];
    /* ca */ s8 mTimers[15][2];
    /* e8 */

    BattleUnk022767c8();
    virtual ~BattleUnk022767c8();
    /* 08 */ virtual void Update();
    void func_ov007_02277368();
    void func_ov007_02277514();
    void func_ov007_02277c44();
    void func_ov007_02277f3c();
    void func_ov007_02278250();
};
