#pragma once

#include "global.h"
#include "types.h"

#include "Scene/SceneBase.hpp"

class CellAnimation;
class SelectWindow;
struct MessageFile;

// Reward screen shown after a battle (0xe8 bytes, vtable 0228b23c, created by BattleResult): counts the amount
// earned into gSaveData.mMoney with coin animations and a printed number.
class BattleMoneyReward : public SceneBase {
public:
    /* 00 SceneBase */
    /* 0c */ CellAnimation *mCellAnims[9];
    /* 30 */ CellAnimation *mCellAnimsB[15];
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
    /* 98 */ u16 mSparkleFrames[6];
    /* a4 */ u8 mAmountVisible;
    /* a5 */ u8 mState;
    /* a6 */ u8 mKind;
    /* a7 */ s8 mTransferTimer;
    /* a8 */ u8 mFade;
    /* a9 */ u8 mUnk_a9;
    /* aa */ s8 mSparkleSoundTimer;
    /* ab */ s8 mTransferSoundTimer;
    /* ac */ u8 mPos[15][2];
    /* ca */ s8 mTimers[15][2];
    /* e8 */

    BattleMoneyReward();
    virtual ~BattleMoneyReward();
    /* 08 */ virtual void Update();
    void InitDisplay();
    void Init();
    void Draw();
    void PrintTotal();
    void ClearText();
};
