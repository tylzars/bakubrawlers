#pragma once

#include "global.h"
#include "types.h"

#include "Scene/SceneBase.hpp"

class CellAnimation;

// One entry of the card effect table at 0227c02c (stride 8): effect file name and the screen it is drawn on.
struct BattleCardEffectInfo {
    /* 00 */ const char *mName;
    /* 04 */ u8 mScreen;
    /* 05 */ PAD(0x05, 0x08);
    /* 08 */
};

// 0x1c byte object that plays a card effect cell animation (Game/Battle/Obj/<name>). Vtable 0228ab70.
class BattleCardEffect : public SceneBase {
public:
    /* 0c */ CellAnimation *mAnim;
    /* 10 */ u8 mState;
    /* 11 */ u8 mAnimPairIndex; // palette variant for kinds >= 0x11
    /* 12 */ PAD(0x12, 0x14);
    /* 14 */ u32 mCharOffset; // character offset passed to SpriteScreen::LoadCharacters
    /* 18 */ u8 mPalette;
    /* 19 */ u8 mKind;
    /* 1a */ u8 mTimer;
    /* 1b */ u8 mSetNextScene;
    /* 1c */

    BattleCardEffect(u32 charOffset, u32 palette, u32 kind, u32 unk4);
    virtual ~BattleCardEffect();
    /* 08 */ virtual void Update();

    void LoadAnimation(); // load the animation files
    int UpdateAnimation();  // update and draw, TRUE when the animation finished
    void SetFirstAnimation();
    void SetMiddleAnimation();
    void SetLastAnimation();
};
