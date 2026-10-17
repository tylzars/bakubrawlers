#pragma once

#include "Battle/BattleBase.hpp"

class CellAnimation;
class ConfrontEmblem02;

// One row of the emblem drop schedule (table at 02278bf8, 5 rows).
struct ConfrontEmblemStep {
    /* 00 */ u16 mUntil;   // upper bound of the cumulative phase length (compared with BattleBase::mUnk_16)
    /* 02 */ u8 mMinDelay;
    /* 03 */ u8 mMaxDelay;
    /* 04 */
};
extern ConfrontEmblemStep gConfrontEmblemSteps[];

// Confrontation screen for field 02 (vs_b_bg_02_*, vs_emblem_NN_nce.bin, vs_b_obj_02_line / vs_b_obj_02_marker).
class ConfrontBg02 : public BattleBase {
public:
    /* 68 */ u16 mDelay;
    /* 6a */ u8 mSide;
    /* 6b */ PAD(0x6b, 0x6c);
    /* 6c */ ConfrontEmblemStep *mStep;
    /* 70 */ BattleData *mData;
    /* 74 */ CellAnimation *mCellAnims[3]; // [0] emblem of the local player (0x1c000), [1] marker (0x1e000), [2] line (0x1d000)
    /* 80 */ ConfrontEmblem02 *mEmblems[4];
    /* 90 */

    ConfrontBg02();
    virtual ~ConfrontBg02();
    /* 24 */ virtual void Init(BattleData *data, u32 mode);
    /* 28 */ virtual void LoadGraphics();
    /* 2c */ virtual void InitSubScreen();
    /* 30 */ virtual BOOL func_ov007_0221bd84();
    /* 34 */ virtual void vfunc_34();
    /* 38 */ virtual BOOL func_ov007_0221bfa4();
    /* 3c */ virtual void vfunc_3c();
    /* 40 */ virtual u8 UpdateTouch(int a);
    /* 44 */ virtual void vfunc_44(int index, u16 count);

    void UpdateEmblemStep();
    BOOL CountDownDelay();
    void StartIdleEmblem();
};
