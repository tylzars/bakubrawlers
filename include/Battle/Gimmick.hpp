#pragma once

#include "global.h"
#include "types.h"

#include "Graphics/ModelTransform.hpp"
#include "Scene/SceneBase.hpp"

struct GimmickSetData;
// 0x800 byte object table of a set file.
struct GimmickSetBlock {
    u32 mData[0x200];
};

// Collision groups of a GimmickSetData (mMode, one bit per set; the `mode` argument of AddCollisionSet and
// Gimmick::LoadCollision). SetCollisionMode gives each group the groups it reacts to (mModeMask) and
// GimmickCollision::TestSets tests two sets only when one's mask has the other's group. BattleTarget makes the
// BAKUGAN / BAKUGAN_SP5 set at mBakugan (BAKUGAN_SP5 only while mSpKind == 5; every other mSpKind uses BAKUGAN),
// one UNIT set per BattleUnit with mStatus != 0 (tag = unit index + 1) and the FIELD set from the field's collision
// file; gimmicks make GIMMICK or GIMMICK_HIT_BY_SP5 sets. mMode 0 (the list head) is skipped by Update. SP5 in a
// name is BattleTarget::mSpKind == 5 (mSpKind values are not named).
#define COLLISION_GROUP_BAKUGAN            1    // reacts to every other group (mask 0xfffe)
#define COLLISION_GROUP_UNIT               2    // reacts to BAKUGAN
#define COLLISION_GROUP_FIELD              4    // reacts to BAKUGAN
#define COLLISION_GROUP_GIMMICK            8    // reacts to BAKUGAN
#define COLLISION_GROUP_BAKUGAN_SP5        0x10 // reacts to FIELD and GIMMICK_HIT_BY_SP5 only
#define COLLISION_GROUP_GIMMICK_HIT_BY_SP5 0x20 // reacts to BAKUGAN and BAKUGAN_SP5

// A collision set (0x838 bytes) that AddCollisionSet builds from a buffer LoadCollisionFile fills from
// "/Game/Collision/atari_%02d.bin"; GimmickCollision tests the sets against each other.
struct GimmickSetData {
    /* 000 */ GimmickSetData *mPrev; // list node: previous
    /* 004 */ GimmickSetData *mNext;
    /* 008 */ u16 mMode; // COLLISION_GROUP_* of this set
    /* 00a */ u16 mModeMask; // COLLISION_GROUP_* bits it reacts to
    /* 00c */ u16 mUnk_0c; // first 4 bytes of the file (count at +e)
    /* 00e */ u16 mUnk_0e;
    /* 010 */ GimmickSetBlock mObstacles; // object records
    /* 810 */ VecFx32 mPosition;
    /* 81c */ u16 mRotX;
    /* 81e */ u16 mRotY;
    /* 820 */ u16 mRotZ;
    /* 822 */ u8 mSlot; // own slot index
    /* 823 */ u8 mHitCount; // overlaps recorded by GimmickCollision::TestSets, at most 8
    /* 824 */ u8 mHitSlots[8];
    /* 82c */ u8 mHitObstacleIndices[8]; // both sets store j, the index into b's records (for set b its own)
    /* 834 */ u8 mTag; // AddCollisionSet's last argument: the unit index + 1 for BattleTarget's unit sets (0 for its
                       // field and mBakugan sets), the GIMMICK_TYPE for a gimmick's set
    /* 835 */ PAD(0x835, 0x838);
};

// Object types of the SetData files (GimmickSetEntry::mType). GimmickManager::CreateFromFile creates one
// subclass per type and each subclass constructor stores its type in Gimmick::mType, which LoadCollision and the
// subclasses' own collision loaders pass on as the collision set's tag (BattleTarget switches on it). Type 1 creates a
// GimmickExePoint when data_020d433c.mScriptMode is set, otherwise a GimmickFieldPowerS with variant 1, which stores
// GIMMICK_TYPE_FIELD_POWER_S. Types 0x1f, 0x28 and 0x3d (GimmickUnk_02274830, GimmickPull,
// GimmickUnk_02230cec) stay literals.
enum {
    GIMMICK_TYPE_EXE_POINT       = 1,
    GIMMICK_TYPE_FIELD_POWER_G   = 2,
    GIMMICK_TYPE_FIELD_POWER_S   = 3,
    GIMMICK_TYPE_JUMP_OBJ        = 4,
    GIMMICK_TYPE_DASH_BASE       = 5,
    GIMMICK_TYPE_BLOCK           = 6,
    GIMMICK_TYPE_FIELD_DESTROY   = 7,
    GIMMICK_TYPE_BAKUGAN         = 8,
    GIMMICK_TYPE_ABILITY_FIELD   = 9,
    GIMMICK_TYPE_PYRUS_ERUPTION  = 0x1e,
    GIMMICK_TYPE_AQUOS_FUKIDASHI = 0x29,
    GIMMICK_TYPE_TORNADO         = 0x32,
    GIMMICK_TYPE_VENT_KAZE       = 0x33,
    GIMMICK_TYPE_SUTE_WALL       = 0x3c,
    GIMMICK_TYPE_HAOS_BALL       = 0x46,
    GIMMICK_TYPE_HAOS_WALL       = 0x47,
    GIMMICK_TYPE_IKAZUCHI_START  = 0x52,
};

// Base of the field gimmick objects of the battle overlay (vtable 0228729c, 0x46 bytes in the derived classes).
// The ModelTransform sits at +0c (constructed first), the SceneBase at +00.
class Gimmick : public ModelTransform, public SceneBase {
public:
    /* 00 SceneBase */
    /* 0c ModelTransform */
    /* 38 */ u8 mState;
    /* 39 */ u8 mId;
    /* 3a */ u8 mType; // GIMMICK_TYPE_* of the class (a type 1 entry made into GimmickFieldPowerS stores FIELD_POWER_S)
    /* 3b */ u8 mMoveMode;
    /* 3c */ GimmickSetData *mCollision;
    /* 40 */ s32 mCountingTurns;
    /* 44 */ u8 mTurnCount; // + 1 per BattlePlayerList::mTurnCount++ while mCountingTurns (per player turn, not round)
    /* 45 */

    Gimmick(u8 a, u8 b);
    void LoadCollision(int id, int mode); // loads "/Game/Collision/atari_%02d.bin" `id` as a collision set into mCollision
    BOOL IsNearMarker(int radius);       // TRUE when a marker with mStatus set is within radius + 0x17800 in x / z
    virtual ~Gimmick();
    /* 08 */ virtual void Update() = 0;
    /* 0c */ virtual void Load() = 0;
    /* 10 */ virtual void Unload() = 0;
    /* 14 */ virtual void Finish() = 0;
    /* 18 */ virtual void CountTurn() {
        if (mCountingTurns) {
            mTurnCount++;
        }
    }
};
