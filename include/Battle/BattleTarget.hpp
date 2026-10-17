#pragma once

#include "global.h"
#include "types.h"

#include <nitro/fx.h>

class Camera;
#include "Graphics/BakuganModel.hpp"
#include "Scene/SceneBase.hpp"

struct GimmickSetData;
class CellAnimation;
class BattleStatusPanels;
class BattleCommandMap;
struct BattleUnk29c;
struct BattlePlayer;

// A box on the field floor (centre, half sizes and the rotation of its frame).
struct BattleHitBox {
    /* 00 */ VecFx32 mPos;
    union {
        /* 0c */ VecFx32 mAxes[3]; // the frame's axes, refreshed from the rotation by GetNearestPointOnBox
        /* 0c */ PAD(0x0c, 0x30);
    };
    /* 30 */ fx32 mHalfX;
    /* 34 */ fx32 mHalfY;
    /* 38 */ fx32 mHalfZ;
    /* 3c */ u16 mRotX;
    /* 3e */ u16 mRotY;
    /* 40 */ u16 mRotZ;
    /* 42 */ PAD(0x42, 0x44);
    /* 44 */
};

// Shapes of a BattleObstacle (mType), switched on by GimmickCollision::TestObstacles and
// BattleTarget::GetObstacleNormal. Types 2 and 3 have no literal site: TestObstacles reaches them only through
// sTests[type - 1] (2: the first object's position inside the second's rotated x/z box; 3: inside its x/z radius).
// When both objects are type 0, TestObstacles still tests the second one as a sphere (TestBoxSphere(a, b)).
#define OBSTACLE_TYPE_BOX      0 // centre, half sizes and rotation, tested as a BattleHitBox
#define OBSTACLE_TYPE_SPHERE   1 // centre and radius mHalfX, tested as a BattleSphere
#define OBSTACLE_TYPE_CYLINDER 4 // x/z radius mHalfX around the centre, y within +-mHalfY

// One 0x20-byte obstacle record of a gimmick set file (at set + 0x10 + 0x20 * i): box centre, half sizes, rotation, shape.
struct BattleObstacle {
    /* 00 */ VecFx32 mPos;
    /* 0c */ fx32 mHalfX;
    /* 10 */ fx32 mHalfY;
    /* 14 */ fx32 mHalfZ;
    /* 18 */ u16 mRotX;
    /* 1a */ u16 mRotY;
    /* 1c */ u16 mRotZ;
    /* 1e */ u8 mType; // OBSTACLE_TYPE_*
    /* 1f */ u8 mUnk_1f;
};

// A ball: centre and radius.
struct BattleSphere {
    VecFx32 mPos;
    fx32 mRadius;
};

// A plane: normal and distance.
struct BattlePlane {
    VecFx32 mNormal;
    fx32 mDist;
};

// The shoot minigame object (shoot, battle_shoot_b_*): the camera, the fired bakugan (a posed 3D object), its status
// panels and the sprites. Vtable 02289810 (0x1f0 bytes, created by Setup).
class BattleTarget : public SceneBase {
public:
    /* 00 SceneBase */
    /* 0c */ CellAnimation *mCellAnims[7];
    /* 28 */ BattleStatusPanels *mStatusPanels;
    /* 2c */ void *mRoulettePalette;
    /* 30 */ u32 mRoulettePaletteSize;
    /* 34 */ s32 mUnk_34;
    /* 38 */ u32 mAirFrames;
    /* 3c */ u32 mPrevAirFrames;
    /* 40 */ u32 mUnk_40;
    /* 44 */ u32 mUnk_44;
    // + 1 on a step with no motion, or with vertical velocity exactly -mGravityPerFrame (otherwise 0); reaching 0x28 on
    // a motionless step stops the bakugan
    /* 48 */ u32 mRestFrames;
    /* 4c */ u32 mMoveFrames; // calls of UpdateMove since construction (any branch); never reset
    /* 50 */ u32 mUnk_50;
    /* 54 */ u32 mUnk_54;
    /* 58 */ VecFx32 mPrevPosition; // its y is also set to an obstacle's top on landing
    /* 64 */ VecFx32 mVelocity;
    /* 70 */ VecFx32 mPrevVelocity;
    /* 7c */ VecFx32 mCollisionNormal;
    /* 88 */ u32 mUnk_88;
    /* 8c */ fx32 mPushScale;
    /* 90 */ u32 mBounceFactor; // always 0x533 when read (UpdateMove sets it first)
    /* 94 */ VecFx32 mUnk_94;
    /* a0 */ VecFx32 mBaseVelocity;
    /* ac */ VecFx32 mPushVelocity;
    /* b8 */ VecFx32 mUnk_b8;
    /* c4 */ u32 mGravityPerFrame; // 0x1c3 (ctor), 0x29, 0xa4 or 0x1dc - 0x19 by the handler
    /* c8 */ VecFx32 mPushPerFrame;
    /* d4 */ VecFx32 mUnk_d4;
    /* e0 */ u8 *mUnitCollisionBuffer;
    /* e4 */ GimmickSetData *mUnitCollisions[12];
    /* 114 */ s32 mUnk_114;
    /* 118 */ s32 mUnk_118;
    /* 11c */ VecFx32 mUnk_11c;
    /* 128 */ VecFx32 mCameraTarget;
    /* 134 */ VecFx32 mUnitHitVelocity;
    /* 140 */ u32 mUnk_140;
    /* 144 */ VecFx32 *mTornadoPosition;
    /* 148 */ u32 mVerticalContactY;
    /* 14c */ u32 mUnk_14c;
    /* 150 */ PAD(0x150, 0x15c);
    /* 15c */ VecFx32 mUnitHitBakuganPosition;
    /* 168 */ VecFx32 mHitUnitPosition;
    /* 174 */ Camera *mCamera;
    /* 178 */ BattleCommandMap *mCommandMap;
    /* 17c */ u32 mUnk_17c;
    /* 180 */ s32 mCameraShakeY;
    /* 184 */ s32 mUnk_184;
    /* 188 */ VecFx32 mCameraOffset;
    /* 194 */ VecFx32 mCameraBasePosition;
    /* 1a0 */ u32 mUnk_1a0;
    /* 1a4 */ BakuganModel *mBakugan;
    /* 1a8 */ GimmickSetData *mCollision;
    /* 1ac */ GimmickSetData *mMapCollision;
    /* 1b0 */ u16 mCameraRotY;
    /* 1b2 */ u16 mUnk_1b2;
    /* 1b4 */ u16 mUnk_1b4;
    /* 1b6 */ u16 mUnk_1b6;
    /* 1b8 */ u16 mRotYAdjust;
    /* 1ba */ u16 mRotZAdjust;
    /* 1bc */ u16 mIkazuchiTimer;
    /* 1be */ u16 mUnk_1be;
    /* 1c0 */ u16 mShotEffectHandle;
    /* 1c2 */ u16 mUnitHitEffectHandle;
    /* 1c4 */ u16 mUnk_1c4;
    /* 1c6 */ u16 mUnk_1c6;
    /* 1c8 */ u16 mTimer; // the 0x78-frame waits of states 8 and 10
    /* 1ca */ u8 mUnk_1ca;
    /* 1cb */ u8 mUnk_1cb;
    /* 1cc */ u8 mUnk_1cc;
    /* 1cd */ u8 mInField;
    /* 1ce */ u8 mUnk_1ce;
    /* 1cf */ u8 mUnk_1cf;
    /* 1d0 */ u8 mLanded;
    /* 1d1 */ u8 mPrevLanded;
    /* 1d2 */ u8 mResult;
    /* 1d3 */ u8 mRecord;
    /* 1d4 */ u8 mPlayer;
    /* 1d5 */ u8 mPartner;
    /* 1d6 */ u8 mUnk_1d6;
    /* 1d7 */ u8 mUnk_1d7;
    /* 1d8 */ u8 mHitGimmickType; // keeps the last test's value on steps that skip UpdateCollisions
    /* 1d9 */ u8 mPrevHitGimmickType;
    /* 1da */ u8 mOnMarkerCell;
    /* 1db */ u8 mUnk_1db;
    /* 1dc */ u8 mUnitHitCount;
    /* 1dd */ u8 mUnk_1dd;
    /* 1de */ u8 mHitUnitHandle;
    /* 1df */ u8 mSpKind;
    /* 1e0 */ u8 mShotSpKind;
    /* 1e1 */ u8 mUnk_1e1;
    /* 1e2 */ u8 mState;
    /* 1e3 */ u8 mCollided;
    /* 1e4 */ u8 mCameraMode;
    /* 1e5 */ u8 mResetCameraOffset;
    /* 1e6 */ u8 mJumpObjCameraStep;
    /* 1e7 */ u8 mUnk_1e7_0 : 1;
    /* 1e7 */ u8 mUnk_1e7_1 : 1;
    /* 1e7 */ u8 mUnk_1e7_2 : 6;
    /* 1e8 */ u8 mUnitHandle;
    /* 1e9 */ u8 mUnk_1e9;
    /* 1ea */ u8 mBarLength;
    /* 1eb */ u8 mPopupEnabled;
    /* 1ec */ u8 mUnk_1ec;
    /* 1ed */ PAD(0x1ed, 0x1f0);
    /* 1f0 */

    BattleTarget(Camera *camera, u8 unk2, u8 unk3);
    virtual ~BattleTarget();
    /* 08 */ virtual void Update();
    void func_ov007_02253fbc();
    void Init();
    void PlaceBakugan();
    void LoadUnitCollisions();
    void LoadMapCollision();
    void SetMapPredictVisible(u32 unk);
    BOOL IsHeadingToward(VecFx32 *a, VecFx32 *b, VecFx32 *vel, u8 index);
    void LaunchHitUnit(GimmickSetData *set);
    void UpdateMove();
    void UpdateHaosWall();
    void UpdateJumpObj();
    void UpdateJumpObjCameraPosition();
    void UpdateCameraTarget(VecFx32 *target);
    void UpdateCameraPosition(VecFx32 *target);
    void UpdateCamera();
    void UpdateCameraAndDraw();
    void DrawRecordTotal(u32 player, u32 record);
    void DrawRoulette();
    BOOL func_ov007_0225d078();
    void StartShot();
    void HandleGimmickHit(GimmickSetData *set, u32 index, VecFx32 *vel, VecFx32 *out);
    BOOL GetObstacleNormal(BattleSphere *sphere, VecFx32 *dir, BattleObstacle *obj, VecFx32 *out, u8 flag, u8 *outFlag);
    void UpdateEffects();
    void GetPushVector(s32 *pair, u8 mode, VecFx32 *out);
    BOOL CanRestOnCell(VecFx32 *pos, u16 *angle, VecFx32 *vel, u8 *outA, u8 *outB);
    void func_ov007_0225c360(u16 *in, s32 a, u16 *out);
    void func_ov007_0225ce08();
    void DrawBar();
    BOOL func_ov007_022583b8();
    void func_ov007_02258448();
    void UpdateTornado(VecFx32 *target);
    void UpdateUnitHoming();
    void UpdateUnitHit();
    BOOL StartUnitHit(VecFx32 *a, VecFx32 *b, VecFx32 *c, u8 d);
    void UpdateIkazuchi();
    void UpdateOnMarkerCell();
    BOOL func_ov007_02259af0();
    BOOL TryRestOnCell();
    void UpdateCollisions(VecFx32 *a, VecFx32 *b, u32 *c, u32 *d);
    void IntegrateMotion(VecFx32 *pos, u16 *rot, VecFx32 *vel, VecFx32 *a, VecFx32 *b, u32 *counter, u8 flag, u32 mode);
    void LimitSumLength(VecFx32 *a, VecFx32 *b);
    void StopSoundAfterVentKaze(); // stops seq 0xf the step after the last VENT_KAZE hit (the tornado also uses 0xf)
    void func_ov007_022585e0();
    BOOL GetHitBoxNormal(BattleHitBox *box, BattleSphere *sphere, VecFx32 *dir, VecFx32 *out, u8 flag, u8 *outFlag);
    BOOL TestPointInHitBoxXZ(BattleHitBox *box, VecFx32 *point);
    BOOL func_ov007_0225aba0(VecFx32 *a, VecFx32 *b, VecFx32 *tri, fx32 *t, VecFx32 *out);
    void func_ov007_0225b69c(u8 a, u8 b);
    void AddPlayerPush();
    void AddPushXZ(s32 *pair);
    void AddScaledPush(VecFx32 *v);
    void MoveClearOfUnit(u8 a, u8 b, VecFx32 *out, VecFx32 *ref);
    BOOL IntersectSegmentPlane(VecFx32 *a, VecFx32 *b, BattlePlane *plane, fx32 *t, VecFx32 *hit);
    BOOL IntersectSegmentTrianglePlane(VecFx32 *a, VecFx32 *b, VecFx32 *c, VecFx32 *d, VecFx32 *e, fx32 *t, VecFx32 *hit);
    void StartLandingSound();
    void func_ov007_02255b78(VecFx32 *b, VecFx32 *c, VecFx32 *in, VecFx32 *out);
    void func_ov007_02255c04(VecFx32 *vec, s32 *out);
};
