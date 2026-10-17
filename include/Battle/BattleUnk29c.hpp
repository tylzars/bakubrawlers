#pragma once

#include "global.h"
#include "types.h"

#include <nitro/fx.h>

#include "Battle/BattleMapTypes.hpp"
#include "Graphics/BakuganModel.hpp"

// Motions of a BattleMoveState (mMode), set by CreateBakugan (IDLE) and BattleTarget through StartMove. StartMove
// scales the given velocity; each frame BattleUnk29c::Update adds it to the model's position (all but IDLE). The
// LAUNCH modes normalise it, scale it to the speed below, set an upward y velocity and a spin, then subtract 0x800
// from the y velocity each frame and hide the model below y 0x899a. No code stores LAUNCH_SPIN_X in this build.
#define MOVE_MODE_IDLE               1 // no motion step; the only mode func_ov007_0226ff08 measures distances for
#define MOVE_MODE_DAMPED             2 // velocity * 0xccd at the start, * 0xf0a each frame, no gravity
#define MOVE_MODE_LAUNCH_SPIN_Y      3 // speed 0xa000, up 0x8000, spins about y
#define MOVE_MODE_SLOW_LAUNCH_SPIN_X 4 // speed 0x5000, up 0x5800, spins about x
#define MOVE_MODE_LAUNCH_SPIN_X      5 // speed 0xa000, up 0x8000, spins about x

// Motion record of one mBakugans model (0x14 bytes, allocated by CreateBakugan).
struct BattleMoveState {
    /* 00 */ VecFx32 mVelocity;
    /* 0c */ u16 mRot[3];
    /* 12 */ u8 mMode; // MOVE_MODE_*
    /* 13 */ u8 mUnk_13;
};

// Bakugan models, name plates and cursors of the battle command map (0xa4 bytes, ctor 0226fb74, dtor 0226fc18).
struct BattleUnk29c {
    /* 00 */ BakuganModel *mBakugans[12];
    /* 30 */ BattleMoveState *mMoveStates[12];
    /* 60 */ BattleMapPiece *mShadow;
    /* 64 */ BattleFontObj *mFontObjs[9];
    /* 88 */ VecFx32 mStIconPosition;
    /* 94 */ u8 mStIconUnitHandle; // index + 1 into mUnits, 0 = none; only tested for non-zero (DrawStIcons)
    /* 95 */ u8 mUnk_95;
    /* 96 */ u8 mStIconFontObjIndex;
    /* 97 */ PAD(0x97, 0x98);
    /* 98 */ u8 mUnk_98[3];
    /* 9b */ u8 mNearUnitHandles[3];
    /* 9e */ u8 mUnk_9e;
    /* 9f */ u8 mUnk_9f;
    /* a0 */ u8 mUnk_a0;
    /* a1 */ PAD(0xa1, 0xa4);
    /* a4 */

    BattleUnk29c();  // 0226fb74
    ~BattleUnk29c(); // 0226fc18
    BakuganModel *GetBakugan(u8 index);
    void ReleaseBakugan(u32 index);
    void CreateFontObjs();
    void Release();
    void Update();
    // Unless flag and mUnk_9e are both 0: mNearUnitHandles = up to three idle units within 0x64000 of the target's
    // bakugan, nearest first; mUnk_98 = up to three idle units with mStatus 2 / 3 or mUnk_12, placed by comparing with
    // the first list's distances (far[]), not their own.
    void func_ov007_0226ff08(u32 flag);
    void CreateBakugan(u32 index, VecFx32 *pos, u16 *rot, u8 a, u8 b);
    void Draw();
    void DrawNearPnames();
    void DrawNearPyPt();
    void DrawStIcons();
    void DrawUnitStIcons(u8 index);
    // type 0: mFontObjs[5], 1: [6]; drawn above the unit's position at the call until Release
    void SetStIconUnit(u8 index, u32 type);
    void StartMove(u8 index, BattleMoveState *state);
    void UpdateMoveMode2(u32 index);
    void UpdateMoveMode3(u32 index);
    void UpdateMoveMode4(u32 index);
    void UpdateMoveMode5(u32 index);
    void DrawFontObj(u8 index, VecFx32 *pos);
    void DrawPyFontObj(VecFx32 *pos);
    void DrawPtFontObj(VecFx32 *pos);
};
