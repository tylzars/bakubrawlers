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

// A collision set (0x838 bytes) that AddCollisionSet builds from a buffer LoadCollisionFile fills from
// "/Game/Collision/atari_%02d.bin"; GimmickCollision tests the sets against each other.
struct GimmickSetData {
    /* 000 */ GimmickSetData *mPrev; // list node: previous
    /* 004 */ GimmickSetData *mNext;
    /* 008 */ u16 mMode; // collision groups this set belongs to
    /* 00a */ u16 mModeMask; // collision groups it reacts to
    /* 00c */ u16 mUnk_0c; // first 4 bytes of the file (count at +e)
    /* 00e */ u16 mUnk_0e;
    /* 010 */ GimmickSetBlock mObstacles; // object records
    /* 810 */ VecFx32 mPosition;
    /* 81c */ u16 mRotX;
    /* 81e */ u16 mRotY;
    /* 820 */ u16 mRotZ;
    /* 822 */ u8 mSlot; // own slot index
    /* 823 */ u8 mHitCount;
    /* 824 */ u8 mHitSlots[8];
    /* 82c */ u8 mHitObstacleIndices[8];
    /* 834 */ u8 mTag; // AddCollisionSet's last argument: the unit index + 1 for BattleTarget's unit sets (0 for its
                       // field and mBakugan sets), the GIMMICK_TYPE for a gimmick's set
    /* 835 */ PAD(0x835, 0x838);
};

// Object types of the SetData files (GimmickSetEntry::mType). GimmickManager::CreateFromFile creates one
// subclass per type and each subclass constructor stores its type in Gimmick::mType, which LoadCollision and the
// subclasses' own collision loaders pass on as the collision set's tag (BattleTarget switches on it). Type 1 creates a
// GimmickExePoint when data_020d433c.mUnk_7a is set, otherwise a GimmickFieldPowerS with variant 1, which stores
// GIMMICK_TYPE_FIELD_POWER_S. Types 0x1f, 0x28 and 0x3d (GimmickUnk_02274830, GimmickUnk_022308a8,
// GimmickUnk_02230cec) are not named.
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
    /* 3a */ u8 mType;
    /* 3b */ u8 mUnk_3b;
    /* 3c */ GimmickSetData *mCollision;
    /* 40 */ s32 mCountingTurns;
    /* 44 */ u8 mTurnCount;
    /* 45 */

    Gimmick(u8 a, u8 b);
    void LoadCollision(int id, int mode); // loads "/Game/Collision/atari_%02d.bin" `id` as a collision set into mCollision
    BOOL IsNearMarker(int radius);       // TRUE when mPosition is within radius of a marked player entry
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
