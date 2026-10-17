#pragma once

#include "global.h"
#include "types.h"

#include <nitro/fx.h>

// Computer player helper of the battle (vtable 02287604: empty destructor + deleting destructor).
// mPlayer is the player index it plays for. mUnk_4c / mUnk_50 hold the current push offset x / z (SetPushOffset).
class BattleCpu {
public:
    /* 00 */ // vtable
    /* 04 */ VecFx32 mPos;
    /* 10 */ VecFx32 mSavedPos;
    /* 1c */ VecFx32 mTargets[4];
    /* 4c */ s32 mUnk_4c;
    /* 50 */ s32 mUnk_50;
    /* 54 */ s32 mUnk_54;
    /* 58 */ s32 mUnk_58;
    /* 5c */ s32 mUnk_5c;
    /* 60 */ u16 mPushOffsetFrameCounts[8];
    /* 70 */ u16 mPushOffsetFrames;
    /* 72 */ u16 mPushOffsetInterval;
    /* 74 */ u16 mTimer;
    /* 76 */ u8 mUsingTargets;
    /* 77 */ u8 mPushIndex;
    /* 78 */ u8 mUnk_78;
    /* 79 */ u8 mPlayer;
    /* 7a */ u8 mRecord;
    /* 7b */ u8 mUnk_7b;
    /* 7c */ PAD(0x7c, 0x7d);
    /* 7d */ u8 mUnk_7d;
    /* 7e */ u8 mSpKind;
    /* 7f */ u8 mTargetNodeIndices[4];
    /* 83 */ u8 mTargetIndex; // (+ 1) % mJumpObjCount per target; set to 0 only by the ctor
    /* 84 */ u8 mJumpObjCount;
    /* 85 */ u8 mPushOffsetEntries[8];
    /* 8d */ u8 mPushOffsetEntryIndex;
    /* 8e */

    BattleCpu(u32 player);
    virtual ~BattleCpu();
    void func_ov007_02231038();
    u8 TryPickUnflaggedSlot54();
    u8 PickMarkerCell();
    u8 PickRecord();
    void PickShot(struct BattleCpuRot *rot, u16 *outA, u16 *outB, u8 *outC);
    void PickPushIndex();
    // also sets mUnk_54 (0x47e + rand 301) and mUnk_58 (0xe10), the BattleTarget::mMoveFrames values
    // BattleShoot::UpdateTurnPlayerCpu waits for
    void StartJumpObjTargets(int a);
    void NextTarget();
    void StopJumpObjTargets();
    void RandomizePushOffsets();
    void SetPushOffset();
    bool func_ov007_02232e04(u32 a, u32 b, u32 c);
    // (0, 0) unless the player's mUnk_f7 != 0 and mPushOffsetFrames % mPushOffsetInterval == 0
    void GetPushOffset(s32 *x, s32 *z);
};

// Ballistics helpers (free functions in BattleCpu.cpp).
int PredictLanding(VecFx32 *pos, VecFx32 *vel, VecFx32 *out); // steps until landed
void RotateDirScaled(u16 *rot, u32 s, VecFx32 *dir, VecFx32 *out);
int PredictStop(VecFx32 *pos, VecFx32 *vel, VecFx32 *out, u32 unk4, u32 unk5); // unk4/unk5 unused
