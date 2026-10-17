#pragma once

#include "global.h"
#include "types.h"

#include "Scene/SceneBase.hpp"

class CellAnimation;
class SelectWindow;

// "Union ability" prompt of the battle (battle_b_obj_union_ability): a card that slides in with one or two
// buttons (mType 5: one button), result in mUnk_26. Vtable 02287614, 0x30 bytes.
class BattleUnionAbility : public SceneBase {
public:
    /* 00 SceneBase */
    /* 0c */ CellAnimation *mAnimA;
    /* 10 */ CellAnimation *mAnimB;
    /* 14 */ CellAnimation *mAnims[2];
    /* 1c */ SelectWindow *mWindow;
    /* 20 */ s16 mSlide;
    /* 22 */ u16 mStateFrames;
    /* 24 */ u8 mState;
    /* 25 */ u8 mType;
    /* 26 */ u8 mChoice;
    /* 27 */ u8 mCancel;
    /* 28 */ u32 mCharOffset;
    /* 2c */ u16 mPalette;
    /* 2e */ PAD(0x2e, 0x30);
    /* 30 */

    BattleUnionAbility(u8 type, u32 charOffset, u16 palette);
    virtual ~BattleUnionAbility();
    /* 08 */ virtual void Update();
    void Close(u8 choice);
    void SetupSubWndAndAntenna();
    void func_ov007_022344a4();
    void LoadGraphics();
    void Draw();
};
