#pragma once

#include "global.h"
#include "types.h"

#include "Scene/SceneBase.hpp"
#include "Game/UnkStruct_020d433c.hpp"

class CellAnimation;
class EffectManager;
class Camera;
class BattleUnionAbility;
class BattleCommandPage;
class BattleCommandList;
#include "Graphics/Camera.hpp"
#include "Battle/BattleTarget.hpp"

// View of a 0x14 byte BattleUnit entry with its position.
struct BattleUnk022637acUnit {
    /* 00 */ VecFx32 mPos;
    /* 0c */ u16 mUnk_0c;
    /* 0e */ u8 mUnk_0e;
    /* 0f */ u8 mUnk_0f;
    /* 10 */ u8 mUnk_10;
    /* 11 */ u8 mUnk_11;
    /* 12 */ PAD(0x12, 0x14);
    /* 14 */
};

// Battle class with vtable 0228a08c (ctor 022637ac, dtor 02263944/022639a8, Update 02263a14, 0x74 bytes). Runs a
// state machine through the handler table at 0228a098 (state = mUnk_0c, entries of 8 bytes: function, zero).
class BattleAbility : public SceneBase {
public:
    /* 0c */ u8 mState;
    /* 0d */ u8 mUnk_0d;
    /* 0e */ u8 mUnk_0e;
    /* 0f */ u8 mUnk_0f;
    /* 10 */ u8 mRecord;
    /* 11 */ u8 mAbilityIndex;
    /* 12 */ PAD(0x12, 0x14);
    /* 14 */ u8 mUnk_14;
    /* 15 */ u8 mPlayer;
    /* 16 */ u8 mUnit;
    /* 17 */ u8 mNextScene;
    /* 18 */ u16 mAbilityId;
    /* 1a */ u16 mUnk_1a;
    /* 1c */ BattleTarget *mTarget;
    /* 20 */ BattleUnionAbility *mUnionAbility;
    /* 24 */ BattleCommandPage *mCommandPage;
    /* 28 */ u8 mStep;
    /* 29 */ u8 mUnk_29;
    /* 2a */ u8 mUnk_2a;
    /* 2b */ u8 mUnk_2b;
    /* 2c */ EffectManager *mEffects;
    /* 30 */ Camera *mCamera;
    /* 34 */ BattleCommandList *mCommandList;
    /* 38 */ u8 mUnk_38;
    /* 39 */ PAD(0x39, 0x3a);
    /* 3a */ u16 mUnk_3a;
    /* 3c */ VecFx32 mUnk_3c;
    /* 48 */ VecFx32 mUnk_48;
    /* 54 */ VecFx32 mUnk_54;
    /* 60 */ u8 mUnk_60;
    /* 61 */ u8 mUnk_61;
    /* 62 */ u8 mUnk_62_0 : 1;
    /* 62 */ u8 mUnk_62_1 : 1;
    /* 62 */ u8 mUnk_62_2 : 1;
    /* 63 */ u8 mUnk_63;
    /* 64 */ s32 mUnk_64;
    /* 68 */ s32 mUnk_68;
    /* 6c */ u16 mUnk_6c;
    /* 6e */ u16 mUnk_6e;
    /* 70 */ u8 mUnk_70;
    /* 71 */ u8 mUnk_71[12];
    /* 7d */ PAD(0x7d, 0x80);
    /* 80 */ CellAnimation *mNumberCellAnims[2];
    /* 88 */

    BattleAbility();
    virtual ~BattleAbility();
    /* 08 */ virtual void Update();
    void LoadGraphics();
    void OpenUnionAbility();
    BOOL func_ov007_02264a5c();
    u8 func_ov007_02265118();
    u8 func_ov007_022652c8();
    u8 func_ov007_0226547c();
    u8 func_ov007_02265718();
    u8 func_ov007_02265af8();
    u8 func_ov007_02265fac();
    u8 func_ov007_022663f0();
    u8 func_ov007_022665ec();
    u8 func_ov007_02266868();
    u8 func_ov007_02266a28();
};
