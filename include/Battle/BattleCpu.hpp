#pragma once

#include "global.h"
#include "types.h"

#include <nitro/fx.h>

// Computer player helper of the battle (vtable 02287604: empty destructor + deleting destructor).
// mPlayer is the player index it plays for.
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
    /* 60 */ u16 mOffsetFrameCounts[8];
    /* 70 */ u16 mOffsetFrames;
    /* 72 */ u16 mOffsetInterval;
    /* 74 */ u16 mTimer;
    /* 76 */ u8 mUsingTargets;
    /* 77 */ u8 mUnk_77;
    /* 78 */ u8 mUnk_78;
    /* 79 */ u8 mPlayer;
    /* 7a */ u8 mRecord;
    /* 7b */ u8 mUnk_7b;
    /* 7c */ PAD(0x7c, 0x7d);
    /* 7d */ u8 mUnk_7d;
    /* 7e */ u8 mUnk_7e;
    /* 7f */ u8 mTargetNodeIndices[4];
    /* 83 */ u8 mTargetIndex; // (+ 1) % mJumpObjCount per target; set to 0 only by the ctor
    /* 84 */ u8 mJumpObjCount;
    /* 85 */ u8 mOffsetEntries[8];
    /* 8d */ u8 mOffsetEntryIndex;
    /* 8e */

    BattleCpu(u32 player);
    virtual ~BattleCpu();
    void func_ov007_02231038();
    u8 func_ov007_022310f0();
    u8 PickMarkerCell();
    u8 PickRecord();
    void func_ov007_02231e2c(struct BattleCpuRot *rot, u16 *outA, u16 *outB, u8 *outC);
    void func_ov007_022328dc();
    // also sets mUnk_54 (0x47e + rand 301) and mUnk_58 (0xe10), the BattleTarget::mMoveFrames values
    // BattleShoot::UpdateTurnPlayerCpu waits for
    void StartJumpObjTargets(int a);
    void NextTarget();
    void StopJumpObjTargets();
    void RandomizeOffsets();
    void SetOffset();
    bool func_ov007_02232e04(u32 a, u32 b, u32 c);
    void GetOffset(s32 *x, s32 *y); // (0, 0) unless the player's mUnk_f7 != 0 and mOffsetFrames % mOffsetInterval == 0
};

// Ballistics helpers (free functions in BattleCpu.cpp).
int PredictLanding(VecFx32 *pos, VecFx32 *vel, VecFx32 *out); // steps until landed
void RotateDirScaled(u16 *rot, u32 s, VecFx32 *dir, VecFx32 *out);
int PredictStop(VecFx32 *pos, VecFx32 *vel, VecFx32 *out, u32 unk4, u32 unk5); // unk4/unk5 unused
