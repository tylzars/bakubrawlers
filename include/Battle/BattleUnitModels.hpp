#pragma once

#include "global.h"
#include "types.h"

#include <nitro/fx.h>

#include "Battle/BattleMapTypes.hpp"
#include "Graphics/BakuganModel.hpp"

// Motions of a BattleMoveState (mMode), set by CreateBakugan (IDLE) and BattleTarget through StartMove. StartMove
// scales the given velocity; each frame BattleUnitModels::Update adds it to the model's position (all but IDLE). The
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

// Models drawn at the units: bakugans, shadow, name plates, st icons and py / pt marks (BattleCommand, BattleTarget, BattleAbility) (0xa4 bytes, ctor 0226fb74, dtor 0226fc18).
struct BattleUnitModels {
    /* 00 */ BakuganModel *mBakugans[12];
    /* 30 */ BattleMoveState *mMoveStates[12];
    /* 60 */ BattleMapPiece *mShadow;
    /* 64 */ BattleFontObj *mFontObjs[9];
    /* 88 */ VecFx32 mStIconPosition;
    /* 94 */ u8 mStIconUnitHandle; // index + 1 into mUnits, 0 = none; only tested for non-zero (DrawStIcons)
    /* 95 */ u8 mStIconKind;
    /* 96 */ u8 mStIconFontObjIndex;
    /* 97 */ PAD(0x97, 0x98);
    /* 98 */ u8 mUnk_98[3];
    /* 9b */ u8 mNearUnitHandles[3];
    /* 9e */ u8 mUnk_9e;
    /* 9f */ u8 mUnk_9f;
    /* a0 */ u8 mDrawMode;
    /* a1 */ PAD(0xa1, 0xa4);
    /* a4 */

    BattleUnitModels();  // 0226fb74
    ~BattleUnitModels(); // 0226fc18
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
    void UpdateDampedMove(u32 index);
    // The three LAUNCH bodies are identical (gravity 0x800, hide below y 0x899a, AddRot); speed and spin axis come
    // from StartMove. LAUNCH_SPIN_X is never stored, so UpdateLaunchSpinXMove does not run in this build.
    void UpdateLaunchSpinYMove(u32 index);
    void UpdateSlowLaunchSpinXMove(u32 index);
    void UpdateLaunchSpinXMove(u32 index);
    void DrawFontObj(u8 index, VecFx32 *pos);
    void DrawPyFontObj(VecFx32 *pos);
    void DrawPtFontObj(VecFx32 *pos);
};
