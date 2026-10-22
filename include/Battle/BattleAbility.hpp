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

// Ability ids (BattleAbility::mAbilityId, copied from BattlePlayer::mAbilityId, which GimmickAbilityField sets from
// gAbilityRows). The handler for an id is sAbilityHandlers[id - 0xc9] (BattleAbility.cpp).
// The caster is mPlayer (the turn player); its unit is mUnit (BattleTarget::mUnitHandle - 1).
// 0xc9: plays "shuto_return" and sets BattlePlayerList::mAbilityRequest = 1; the list then sets mRepeatTurn, and its state 26
// skips mTurnCount++ for it, so mTurnPlayer stays the same.
#define ABILITY_ID_REPEAT_TURN     0xc9
// 0xca: plays "shuto_return" and sets mAbilityRequest = 2: the list removes mUnitA (or mUnitB and puts mUnitA back on its
// cell) and goes straight back to BattleShoot for the same turn player (BattleCommand, mAddMarkerEnabled cleared, when that
// player is not BattlePlayerList::mLocalPlayer).
#define ABILITY_ID_SHOOT_AGAIN     0xca
// 0xcb: picks one of the caster's mSlots54 entries (in a BattleCommandPage prompt when the caster is
// BattlePlayerList::mLocalPlayer; at random for a CPU caster, func_ov007_02264a5c; from its mMsg for any other caster),
// then SetMarkerPlayerSlot gives the marker of mUnit to the caster with that slot (the old owner's slot flag is
// cleared).
#define ABILITY_ID_REASSIGN_MARKER 0xcb
// 0xcc: picks one of the caster's records (in a BattleCommandList prompt when the caster is mLocalPlayer; at random
// among the records with mStatus 0 for a CPU caster; from its mMsg for any other caster), plays "bkn_exchange";
// ReplaceUnitRecord puts the record in the unit mUnit and the unit's model is set up with its mBakuganId.
#define ABILITY_ID_REPLACE_RECORD  0xcc
// 0xce: ("bkn_move") moves mUnit onto the marker of a unit of another player (not the partner),
// picked at random by func_ov007_02264a5c.
#define ABILITY_ID_MOVE_TO_UNIT    0xce
// 0xcf: ("bkn_move") moves mUnit to a marker with no unit or with another player's unit, picked at
// random by func_ov007_02264a5c.
#define ABILITY_ID_MOVE_TO_MARKER  0xcf
// 0xd0: plays "defense_up" and sets mUnit's mStatus to 3; BattleTarget's unit checks skip units with mStatus 3.
#define ABILITY_ID_DEFENSE_UP      0xd0
// Not named (no word for what they change): 0xcd (no handler; sets mAbilityRequest = 3 when mUnk_0f is 3); 0xd1 (adds
// the record's mUnk_06 to itself); 0xd2 (sets BattlePlayer::mSpGauge to 0xbe); 0xd3 / 0xd4 ("gp_down" /
// "gp_pickup2": clear / add to itself the record mUnk_06 of the caster's unit, then of every other placed unit).

// Battle class with vtable 0228a08c (ctor 022637ac, dtor 02263944/022639a8, Update 02263a14, 0x74 bytes). Update
// switches on mState; state 13 runs sAbilityHandlers[mAbilityIndex] (table at 0228a098, 8-byte entries: function,
// zero).
class BattleAbility : public SceneBase {
public:
    /* 0c */ u8 mState;
    /* 0d */ u8 mNotLocal;
    /* 0e */ u8 mIsCpu;
    /* 0f */ u8 mUnk_0f;
    /* 10 */ u8 mRecord;
    /* 11 */ u8 mAbilityIndex;
    /* 12 */ PAD(0x12, 0x14);
    /* 14 */ u8 mUnk_14;
    /* 15 */ u8 mPlayer;
    /* 16 */ u8 mUnit;
    /* 17 */ u8 mNextScene;
    /* 18 */ u16 mAbilityId; // ABILITY_ID_*
    /* 1a */ u16 mUnk_1a;
    /* 1c */ BattleTarget *mTarget;
    /* 20 */ BattleUnionAbility *mUnionAbility;
    /* 24 */ BattleCommandPage *mCommandPage;
    /* 28 */ u8 mStep;
    /* 29 */ u8 mDestUnit;
    /* 2a */ u8 mDestMarker;
    /* 2b */ u8 mPickIndex;
    /* 2c */ EffectManager *mEffects;
    /* 30 */ Camera *mCamera;
    /* 34 */ BattleCommandList *mCommandList;
    /* 38 */ u8 mCameraStep;
    /* 39 */ PAD(0x39, 0x3a);
    /* 3a */ u16 mOrbitAngle;
    /* 3c */ VecFx32 mCameraPositionPerFrame;
    /* 48 */ VecFx32 mCameraTargetPerFrame;
    /* 54 */ VecFx32 mDestPosition;
    /* 60 */ u8 mAmountStep;
    /* 61 */ u8 mAmountDigitCount;
    /* 62 */ u8 mSubtract : 1;
    /* 62 */ u8 mUnk_62_1 : 1;
    /* 62 */ u8 mStIconSet : 1;
    /* 63 */ u8 mAmountFrames;
    /* 64 */ s32 mAmountY;
    /* 68 */ s32 mAmountYPerFrame;
    /* 6c */ u16 mAmountRemaining;
    /* 6e */ u16 mShownAmount;
    /* 70 */ u8 mOtherUnitCount;
    /* 71 */ u8 mOtherUnits[12];
    /* 7d */ PAD(0x7d, 0x80);
    /* 80 */ CellAnimation *mNumberCellAnims[2];
    /* 88 */

    BattleAbility();
    virtual ~BattleAbility();
    /* 08 */ virtual void Update();
    void LoadGraphics();
    void OpenUnionAbility();
    BOOL func_ov007_02264a5c();
    u8 UpdateRepeatTurn();
    u8 UpdateShootAgain();
    u8 UpdateReassignMarker();
    u8 UpdateReplaceRecord();
    u8 UpdateMoveToUnit();
    u8 UpdateMoveToMarker();
    u8 UpdateDefenseUp();
    u8 func_ov007_022665ec();
    u8 func_ov007_02266868();
    u8 func_ov007_02266a28();
};
