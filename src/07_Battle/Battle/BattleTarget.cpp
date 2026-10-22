#include "Graphics/Camera.hpp"
#include "System/SoundManager.hpp"
#include "System/TouchManager.hpp"
#include "System/WirelessManager.hpp"
#include "Graphics/SpriteManager.hpp"
#include "Battle/BattleCpu.hpp"
#include <nitro/fx.h>
#include "Battle/BattleCardEffect.hpp"
#include "Battle/BattleUnitModels.hpp"
#include "Battle/BattleStatusPanels.hpp"
#include "Battle/BattleTarget.hpp"
#include "Battle/BattleCommand.hpp"
#include "Battle/BattleAbility.hpp"
#include "Game/Random.hpp"
#include "System/TouchManager.hpp"
#include "System/WirelessManager.hpp"
#include "Battle/GimmickTypes.hpp"
#include "Game/UnkStruct_020d433c.hpp"
#include "Graphics/Camera.hpp"
#include "Graphics/CellAnimation.hpp"
#include "Graphics/Display.hpp"
#include "Graphics/Effect.hpp"
#include "Graphics/SpriteManager.hpp"
#include "Graphics/BakuganModel.hpp"
#include "System/FileSystem.hpp"
#include "System/SoundManager.hpp"
#include <nitro/gx.h>
#include <nitro/mi.h>
#include <nitro/os.h>

struct GimmickBuffer {
    u8 mData[0x804];
};

#define ABS(x) ((x) < 0 ? -(x) : (x))
#define INT_TO_FX(n) ((fx32) ((n) > 0 ? 0.5f + (float) ((n) << 12) : (float) ((n) << 12) - 0.5f))

// The constant tables at 0x0227a5e0-0x0227a74c are one object: as separate tables mwcc sorts them by size, and the
// original has them in this (unsorted) order, followed by the three byte tables in their own sections. Header order
// above (Camera, Sound, Touch, Wireless, Sprite) gives the original .bss node order.
struct TargetTables {
    // read at mUnitHitCount 0..2: [2] runs into mHitFrameCounts[0], harmless (that hit sets mUnk_140 = 0 at once)
    /* 000 */ u16 mSpKind1HitFrameCounts[2];
    /* 004 */ u16 mHitFrameCounts[5]; // only [0..2] are read (mUnitHitCount stops at 3)
    /* 00e */ u16 mBaseRotY[4];
    /* 016 */ u16 mRestAngleLimits[5];
    /* 020 */ u16 mHeadingAngleLimits[5];
    /* 02c */ VecFx32 mIkazuchiEffectOffset;
    /* 038 */ VecFx32 mUnk_38;
    /* 044 */ VecFx32 mJumpObjCameraOffset;
    /* 050 */ u8 mBarOam[12];
    /* 05c */ VecFx32 mUnk_5c;
    /* 068 */ VecFx32 mHitEffectOffset;
    /* 074 */ VecFx32 mUnk_74;
    /* 080 */ u16 mSpKind3Speeds[8]; // both readers add 0x50
    /* 090 */ u16 mSpKind3RotZ[8];
    /* 0a0 */ u32 mResultFrameCounts[5];
    /* 0b4 */ s32 mHeadingMinSpeeds[5];
    /* 0c8 */ s32 mPushScaleFactors[5];
    /* 0dc */ s32 mPushScaleFrameCounts[5];
    /* 0f0 */ s32 mRestSpeedLimits[5];
    /* 104 */ s32 mUnk_104[8];
    /* 124 */ VecFx32 mSpKind1HitCameraOffsets[3]; // both tables: x / z turned and added to a position, y absolute
    /* 148 */ VecFx32 mHitCameraOffsets[3];
};

static const TargetTables sTables = {
    {0x2d, 0x2d},
    {0x28, 0x23, 0x3c, 0x4f, 0x59},
    {0, 0xc000, 0x8000, 0x4000},
    {0x18e3, 0x2000, 0x271c, 0x31c7, 0x3c71},
    {0x4fa, 0x38e, 0x222, 0x222, 0xb6},
    {0, 0xf000, 0xa000},
    {0x37000, 0xf000, 0x37000},
    {0, 0x19000, 0},
    {0x00, 0x40, 0x00, 0x40, 0, 0, 0, 0, 0, 0, 0, 0},
    {0x800, 0x1000, 0x800},
    {0, 0, 0xa000},
    {0x14000, 0x5a000, 0x14000},
    {0x14, 0x14, 0x14, 0x14, 0x1e, 0x14, 0x14, 0x1e},
    {0x38e3, 0x3777, 0x3555, 0x3555, 0x31c7, 0x3a4f, 0x36c1, 0x3555},
    {0xf, 0x32, 0xaa, 0xaa, 0xaa},
    {0x5000, 0x5333, 0x5800, 0x5ccd, 0x6000},
    {0xf33, 0xf9a, 0xfae, 0xfc3, 0xfd7},
    {0x384, 0x3fc, 0x4b0, 0x5a0, 0x708},
    {0x2800, 0x3000, 0x3800, 0x4000, 0x4800},
    {0x12c000, 0x12c000, 0x12c000, 0x12c000, 0x12c000, 0x12c000, 0x12c000, 0x12c000},
    {{0x1e000, 0x32000, -0xa000}, {-0x14000, 0xf000, 0x19000}, {0x28000, 0x1e000, 0x28000}},
    {{-0xf000, 0x19000, -0x1e000}, {-0xf000, 0x14000, 0x1e000}, {0x46000, 0x32000, 0x46000}},
};
static const u8 data_ov007_0227a74c[4] = {0xc, 0xa, 0xb, 0};
static const u8 data_ov007_0227a750[4] = {0, 1, 1, 0};
static const s8 sShotSpeedAdjusts[5] = {-10, -5, 0, 5, 10};

u8 AddUnit(BattlePlayerList *list, const VecFx32 *pos, u16 rot, u8 player, u8 record, u8 cell, u8 flag);
void SetUnitAB(BattlePlayerList *list, u8 a, u8 b);
void GetNearestPointOnBox(BattleSphere *sphere, BattleHitBox *box, VecFx32 *out);
extern "C" void MTX_ApplyRotX33(MtxFx33 *m, u16 angle);
extern "C" void DC_FlushRange(void *buf, u32 size);
extern "C" void MTX_ApplyRotZ33(MtxFx33 *m, u16 angle);
// Two callers leave r2 as it is (the original passes a value already in r2); call through a two-argument type.
static inline void ReleaseSlot(BattlePlayerList *list, u8 index) {
    ((void (*)(BattlePlayerList *, u8)) RemoveUnit)(list, index);
}
void func_ov007_022337ec(VecFx32 *A, VecFx32 *B, VecFx32 *C, VecFx32 *D, VecFx32 *E, fx32 F, fx32 G, int flag);
extern "C" fx32 FX_Atan2(fx32 x, fx32 z);
void NormalizeDiff(const VecFx32 *a, const VecFx32 *b, int unused, VecFx32 *out);
void func_ov007_0226a380(BattlePlayer *player, u8 record, int unk);
void GetVelocityTowardXZ(VecFx32 *a, VecFx32 *b, VecFx32 *speed, VecFx32 *out);
extern "C" u32 FX_AcosIdx(s32 value);

extern "C" void SetDirAndPredict(BattleCommandMap *map, u32 a, u32 b, u32 c);
extern "C" void DrawShadowPieceXZ(BattleCommandMap *map, VecFx32 *v);
extern "C" void FreeBakuganModelInstance(BakuganModel *pose);
BOOL SetMessageTypeAndCheckAll(BattlePlayerList *list, u32 id);

static inline void SetSphere(BattleSphere *s, const VecFx32 *pos, fx32 radius) {
    s->mPos   = *pos;
    s->mRadius = radius;
}

static inline void SetVec(VecFx32 *v, fx32 x, fx32 y, fx32 z) {
    v->x = x;
    v->y = y;
    v->z = z;
}

// Scales v by an unsigned fx32 factor. Called with a constant in both arms of an if/else (AddPushXZ): mwcc
// merges the two copies only after scheduling, which keeps the following call's argument setup after the products.
static inline void ScaleVec(VecFx32 *v, u32 scale) {
    v->x = (fx32) (((fx64) v->x * scale + 0x800LL) >> 12);
    v->y = (fx32) (((fx64) v->y * scale + 0x800LL) >> 12);
    v->z = (fx32) (((fx64) v->z * scale + 0x800LL) >> 12);
}

// A rotation (three u16 angles) copied as one 6-byte object; see TryRestOnCell / StartShot.
struct Rot3 {
    u16 r[3];
};

// True when the effect with this handle (slot number + 1) is still running.
enum EffectState { EFFECT_DEAD, EFFECT_ALIVE };
static inline BOOL EffectAlive(EffectManager *manager, u16 handle) {
    EffectState alive = EFFECT_DEAD;
    if (handle != 0) {
        if (manager->mEffects[handle - 1] != 0) {
            alive = EFFECT_ALIVE;
        }
    }
    return alive;
}

// Drops the status panels and starts the effect of a result (kind).
static inline void StartResult(BattleTarget *t, u32 kind) {
    BattleStatusPanels *panels = t->mStatusPanels;
    if (panels) {
        delete panels;
    }
    t->mStatusPanels                      = 0;
    data_020d433c.mGimmicks->mState = 0;
    new BattleCardEffect(0x1000, 0, kind, 0);
}

// Copies the active camera into the 3D engine state and the camera manager's snapshot of it.
static inline void SnapshotCamera() {
    CameraManager &manager        = CameraManager::Get();
    Camera *camera                = manager.mCameras[manager.mCameraIndex];
    data_02211374.mCameraPosition = camera->mPosition;
    data_02211374.mCameraUp       = camera->mUp;
    data_02211374.mCameraTarget   = camera->mTarget;
    MTX_LookAt(&camera->mPosition, &camera->mUp, &camera->mTarget, &data_02211374.mView);
    data_02211374.mFlags &= ~0xe8;
    manager.mProjection[manager.mCameraIndex] = data_02211374.mProjection;
    manager.mView[manager.mCameraIndex]       = data_02211374.mView;
    manager.mMtxA[manager.mCameraIndex]       = *func_02032710();
    manager.mMtxB[manager.mCameraIndex]       = *func_02032604();
    manager.mMtxC[manager.mCameraIndex]       = *func_020323b8();
}

// Bits 12-14 of the halfword at +2 of a record.
struct RecordBits {
    u16 mUnk_0 : 12;
    u16 mIndex : 3;
    u16 mUnk_15 : 1;
};

union ObstacleBuf {
    BattleObstacle o;
    u32 w[8];
};
#define OBST(set, idx) ((BattleObstacle *) ((u8 *) (set) + 0x10 + (idx) * 0x20))

static inline void AddRot(BattleObstacle *o, GimmickSetData *s) {
    u16 *r = &s->mRotX;
    o->mRotY += r[1];
    o->mRotZ += r[2];
    o->mRotX += r[0];
}

// Like AddRot, but all three sums are computed before any store (UpdateCollisions, HandleGimmickHit).
static inline void AddRotAtOnce(BattleObstacle *o, GimmickSetData *s) {
    u16 *r = &s->mRotX;
    u16 y  = o->mRotY + r[1];
    u16 z  = o->mRotZ + r[2];
    u16 x  = o->mRotX + r[0];
    o->mRotX = x;
    o->mRotY = y;
    o->mRotZ = z;
}

// Same, reading the set's angles as fields rather than through a pointer.
static inline void AddRotAtOnceFields(BattleObstacle *o, GimmickSetData *s) {
    u16 y = o->mRotY + s->mRotY;
    u16 z = o->mRotZ + s->mRotZ;
    u16 x = o->mRotX + s->mRotX;
    o->mRotX = x;
    o->mRotY = y;
    o->mRotZ = z;
}

BattleTarget::BattleTarget(Camera *camera, u8 unk2, u8 unk3)
    : SceneBase(0x3e8, 0) {
    mStatusPanels = 0;
    mUnk_88 = 0;
    mResultFrames = 0;
    mTimer = 0;
    mInField = 1;
    mKind2ObstaclesEnabled = 0;
    mRestOnCellDisabled = 0;
    mLanded = 0;
    mPrevLanded = 0;
    mResult = 0;
    mCameraTargetWeight = 0xe;
    mState = 0;
    mVisible = 1;
    mUnk_1e7_1 = 0;
    mPopupEnabled = 0;
    mUnk_1ec = 0;
    mCamera = camera;
    CameraManager::Get().mCameraIndex = 0;
    CameraManager::Get().SetCamerasAndRequestMode(1, mCamera, 0);
    mPushZ = 0;
    mPushX = 0;
    mUnk_d4.x = 0;
    mUnk_d4.y = 0;
    mUnk_d4.z = 0;
    mUnk_1b2 = 0;
    mUnk_1b4 = 0;
    mUnk_1b6 = 0;
    mPrevPosition.x = 0;
    mPrevPosition.y = 0;
    mPrevPosition.z = 0;
    mVelocity.x = 0;
    mVelocity.y = 0;
    mVelocity.z = 0;
    mPrevVelocity.x = 0;
    mPrevVelocity.y = 0;
    mPrevVelocity.z = 0;
    mCollisionNormal.x = 0;
    mCollisionNormal.y = 0;
    mCollisionNormal.z = 0;
    mPushVector.x = 0;
    mPushVector.y = 0;
    mPushVector.z = 0;
    mBaseVelocity.x = 0;
    mBaseVelocity.y = 0;
    mBaseVelocity.z = 0;
    mUnk_b8.x = 0;
    mUnk_b8.y = 0;
    mUnk_b8.z = 0;
    mPushVelocity.x = 0;
    mPushVelocity.y = 0;
    mPushVelocity.z = 0;
    mPushPerFrame.x = 0;
    mPushPerFrame.y = 0;
    mPushPerFrame.z = 0;
    mGravityPerFrame = 0x1c3;
    mPushScale = 0x32000;
    mAirFramesBeforeIntegrate = mPrevAirFrames = mAirFrames = 1;
    mForceAirborne = 0;
    mRestFrames = 0;
    mUnk_34 = 0;
    mMoveFrames = 0;
    mPushScaleTimer = 1;
    mPushScaleFrameCount = 1;
    mBarLength = 0;
    mPushScaleZeroFrames = 0;
    mBounceFactor = 0;
    mPlayer = unk2;
    mRecord = unk3;
    mPushInputStopped = 0;
    mUnk_1d7 = 0;
    mPartner = data_020d433c.mBattlePlayers->mPlayers[mPlayer]->mPartner;
    mRotYAdjust = 0;
    mRotZAdjust = 0;
    mCameraEaseFactor = 0;
    mUnk_11c.x = 0x5000;
    mUnk_11c.y = 0x5000;
    mUnk_11c.z = 0x5000;
    mCameraTarget.x = 0;
    mCameraTarget.y = 0;
    mCameraTarget.z = 0;
    mUnitHitVelocity.x = 0;
    mUnitHitVelocity.y = 0;
    mUnitHitVelocity.z = 0;
    mHitUnitPosition.x = 0;
    mHitUnitPosition.y = 0;
    mHitUnitPosition.z = 0;
    mUnitHitBakuganPosition.x = 0;
    mUnitHitBakuganPosition.y = 0;
    mUnitHitBakuganPosition.z = 0;
    mRoulettePalette = 0;
    mRoulettePaletteSize = 0;
    mUnk_1db = 0;
    mUnitHitCount = 0;
    mUnk_140 = 0;
    mIkazuchiTimer = 0;
    mHitGimmickType = 0;
    mUnk_1dd = 0;
    mTornadoPosition = 0;
    mVerticalContactY = 0x899a;
    mShotEffectHandle = 0;
    mUnitHitEffectHandle = 0;
    mUnk_1c4 = 0;
    mSkipHeadingTest = 0;
    mHitUnitHandle = 0;
    mPushMode = 0;
    mCameraMode = 0;
    mJumpObjCameraStep = 0;
    mUnk_17c = 0;
    mCameraRotY = 0;
    mCameraShakeY = 0;
    mCameraOffset.x = 0;
    mCameraOffset.y = 0;
    mCameraOffset.z = 0;
    mTornadoCameraPullback = 0;
    mSpKind = 0;
    mShotSpKind = 0;
    mUnk_1be = 0;
    mCollision = 0;
    mCollided = 0;
    mUnitCollisionBuffer = 0;
    u8 i = 0;
    do {
        mUnitCollisions[i] = 0;
        i++;
    } while (i < 12);
    u8 j = 0;
    do {
        mCellAnims[j] = 0;
        j++;
    } while (j < 7);
    mUnk_1a0 = 0;
    mUnitHandle = 0;
    mBakugan = 0;
    if (data_020d433c.mBattlePlayers->mUnitModels) {
        data_020d433c.mBattlePlayers->mUnitModels->mDrawMode = 1;
        data_020d433c.mBattlePlayers->mUnitModels->mUnk_9e = 1;
    }
    if (data_020d433c.mFieldId == 7) {
        mFloorOutsideField = 1;
    } else {
        mFloorOutsideField = 0;
    }
    data_020d433c.mGimmickCollision->mEnabled = 1;
}

BattleTarget::~BattleTarget() {
    data_020d433c.mGimmickCollision->mEnabled = 0;
    data_020d433c.mEffects->FreeAllEffects();
    FreeEffectGroup(data_020d433c.mEffects, 1);
    delete mBakugan;
    if (mUnitCollisionBuffer) {
        delete mUnitCollisionBuffer;
    }
    u8 i = 0;
    do {
        delete mCellAnims[i];
        i++;
    } while (i < 7);
    u8 j = 0;
    do {
        if (mUnitCollisions[j]) {
            RemoveCollisionSet(data_020d433c.mGimmickCollision, mUnitCollisions[j]);
            mUnitCollisions[j] = 0;
        }
        j++;
    } while (j < 12);
    if (mCollision) {
        RemoveCollisionSet(data_020d433c.mGimmickCollision, mCollision);
    }
    if (mMapCollision) {
        RemoveCollisionSet(data_020d433c.mGimmickCollision, mMapCollision);
    }
    delete mStatusPanels;
    if (mRoulettePalette) {
        OS_Free(mRoulettePalette);
    }
    SpriteManager::Get().ResetSlots();
}

void BattleTarget::Update() {
    data_020d433c.mFlags &= 8;
    if (data_020d433c.mFlags == 0) {
        switch (mState) {
            case 0:
                Init();
                SpriteManager::Get();
                SpriteManager::sFade[0] = FADE_MODE_TO_ZERO;
                mState++;
                break;
            case 1:
                mPopupEnabled = 1;
                switch (mSpKind) {
                    case 0:
                    case 2:
                    case 4:
                    case 5:
                    case 6:
                        SetDirAndPredict(mCommandMap, (u16) (0x6000 - mRotYAdjust), (u16) (mRotZAdjust + 0x31c + 0x400), 0x50);
                        break;
                    case 1:
                        SetDirAndPredict(mCommandMap, (u16) (0x6000 - mRotYAdjust), (u16) (mRotZAdjust + 0x8e + 0x300), 0x78);
                        break;
                    case 3: {
                        u32 idx = data_020d433c.mFieldId;
                        SetDirAndPredict(mCommandMap, (u16) (0x6000 - mRotYAdjust), sTables.mSpKind3RotZ[idx], (u16) (sTables.mSpKind3Speeds[idx] + 0x50));
                        break;
                    }
                }
                break;
            case 2:
                mPopupEnabled = 1;
                if (mResultFrames == 0) {
                    UpdateMove();
                } else {
                    mPushScaleTimer                                         = 0;
                    data_020d433c.mBattlePlayers->mUnitModels->mUnk_9e = 0;
                    mState++;
                    mPopupEnabled = 0;
                    if (mResult >= 2) {
                        BattlePlayerList *list = data_020d433c.mBattlePlayers;
                        u8 unit                = list->Units()[(u8) (mUnitHandle - 1)].mMarker;
                        BattleMarker *marker   = &list->Markers()[unit];
                        CreateCursorModelAtCell(marker->mCellX, marker->mCellY);
                    }
                }
                {
                    GimmickSetData *set = mCollision;
                    if (set) {
                        BakuganModel *pose = mBakugan;
                        set->mPosition.x          = pose->mPosition.x;
                        set->mPosition.y          = pose->mPosition.y;
                        set->mPosition.z          = pose->mPosition.z;
                        BakuganModel *src = mBakugan;
                        GimmickSetData *dst = mCollision;
                        u16 *rot = &dst->mRotX;
                        rot[0] = src->mRotX;
                        rot[1] = src->mRotY;
                        rot[2] = src->mRotZ;
                    }
                }
                break;
            case 3:
                if (mResultFrames != 0) {
                    if (mInField == 0) {
                        UpdateMove();
                        GimmickSetData *set = mCollision;
                        if (set) {
                            BakuganModel *pose = mBakugan;
                            set->mPosition.x          = pose->mPosition.x;
                            set->mPosition.y          = pose->mPosition.y;
                            set->mPosition.z          = pose->mPosition.z;
                            BakuganModel *src = mBakugan;
                            GimmickSetData *dst = mCollision;
                            u16 *rot = &dst->mRotX;
                            rot[0] = src->mRotX;
                            rot[1] = src->mRotY;
                            rot[2] = src->mRotZ;
                        }
                    }
                    mResultFrames = mResultFrames + 1;
                }
                mPushInputStopped = 1;
                mUnk_1d7 = 1;
                if (mResult == 2 && mUnitHandle != 0 && mResultFrames == 0x32) {
                    switch (mSpKind) {
                        case 6:
                            data_020d433c.mBattlePlayers->mUnitModels->SetStIconUnit(mUnitHandle - 1, 0);
                            break;
                        case 5:
                            data_020d433c.mBattlePlayers->mUnitModels->SetStIconUnit(mUnitHandle - 1, 1);
                            break;
                    }
                }
                if (mResultFrames > sTables.mResultFrameCounts[mResult]) {
                    SoundManager::Get().StopSeqArcPlayerSounds(0x3c);
                    BattlePlayerList *list = data_020d433c.mBattlePlayers;
                    u8 kind                = list->mPlayers[mPlayer]->mRecords[mRecord].mBakuganId;
                    u8 a                   = list->mUnitA;
                    u8 b                   = list->mUnitB;
                    if (kind == 4 && (mResult == 2 || (mResult == 3 && list->Units()[a].mPlayer != list->Units()[b].mPlayer))) {
                        mState++;
                    } else {
                        mState = 0xb;
                    }
                    mPopupEnabled = 0;
                }
                break;
            case 4:
                QueueUpload(SpriteManager::Get().Screen(1).mBgIndex, UPLOAD_KIND_OBJ_PLTT, 0, mRoulettePalette, mRoulettePaletteSize, 0);
                DrawRoulette();
                mUnk_34 = 0;
                mState++;
                break;
            case 5: {
                u32 n   = mUnk_34 + 1;
                mUnk_34 = n;
                if (n % 3 == 0) {
                    SoundManager::Get().StartSeqArc(0x59);
                }
                WirelessManager::Get();
                if (IsWirelessMode(&data_020c4030.mUnk_228)) {
                    if (mPlayer == data_020d433c.mBattlePlayers->mLocalPlayer) {
                        if (TouchManager::Get().mPressed) {
                            mState++;
                        }
                    } else {
                        mState++;
                    }
                } else {
                    if ((mPlayer == data_020d433c.mBattlePlayers->mLocalPlayer && TouchManager::Get().mPressed) || data_020d433c.mBattlePlayers->mPlayers[mPlayer]->IsCpu()) {
                        mState++;
                    }
                }
                DrawRoulette();
                break;
            }
            case 6: {
                u8 pick;
                u32 n   = mUnk_34 + 1;
                mUnk_34 = n;
                if (n % 3 == 0) {
                    SoundManager::Get().StartSeqArc(0x59);
                }
                if (SetMessageTypeAndCheckAll(data_020d433c.mBattlePlayers, 0x15)) {
                    BattleRecord *record = &data_020d433c.mBattlePlayers->mPlayers[mPlayer]->mRecords[mRecord];
                    u8 values[3]         = {1, 2, 4};
                    pick                 = MATH_Rand32(&gRandom, 3);
                    mCellAnims[4]->SetAnimation(pick + 3);
                    mCellAnims[5]->SetAnimation(1);
                    record->mAttribute = values[pick];
                    mUnk_34         = 0;
                    mState++;
                }
                DrawRoulette();
                break;
            }
            case 7: {
                if (mCellAnims[4]->mFinished != 0) {
                    if (data_020d433c.mBattlePlayers->mPlayers[mPlayer]->mRecords[mRecord].mAttribute == 1) {
                        mState = 0xb;
                    } else {
                        mTimer = 0x78;
                        mState++;
                    }
                }
                DrawRoulette();
                u8 frame = mCellAnims[4]->mFrameIndex;
                if (mUnk_34 != frame) {
                    u8 t = data_ov007_0227a74c[mCellAnims[4]->mAnimIndex - 3];
                    if (t <= frame) {
                        if ((t - frame) % 2 == 0) {
                            SoundManager::Get().StartSeqArc(0x59);
                        }
                    } else {
                        SoundManager::Get().StartSeqArc(0x59);
                    }
                }
                mUnk_34 = mCellAnims[4]->mFrameIndex;
                break;
            }
            case 8:
                mTimer = mTimer - 1;
                if (mTimer == 0) {
                    SpriteManager::Get();
                    SpriteManager::sFade[0] = FADE_MODE_UP;
                    SoundManager::Get().StartSeqArc(6);
                    mState++;
                }
                DrawRoulette();
                break;
            case 9: {
                if (SpriteManager::Get().IsFading()) {
                    break;
                }
                BattlePlayerList *list = data_020d433c.mBattlePlayers;
                BattleRecord *record   = &list->mPlayers[mPlayer]->mRecords[mRecord];
                mBakugan->Setup(record->mBakuganId, record->mAttribute, BAKUGAN_MODEL_MODE_H_HOLD_FRAME);
                SpriteManager::Get();
                SpriteManager::sFade[0] = FADE_MODE_TO_ZERO;
                mTimer                   = 0x78;
                mState++;
                break;
            }
            case 10:
                if (SpriteManager::Get().IsFading()) {
                    break;
                }
                mTimer = mTimer - 1;
                if (mTimer == 0) {
                    mState++;
                }
                break;
            case 11: {
                data_020d433c.mEffects->FreeAllEffects();
                FreeEffectGroup(data_020d433c.mEffects, 0);
                FreeEffectGroup(data_020d433c.mEffects, 1);
                BattlePlayerList *list = data_020d433c.mBattlePlayers;
                u8 cur                 = list->mTurnPlayer;
                u8 a                   = list->mUnitA;
                u8 b                   = list->mUnitB;
                if (list->mPlayers[cur]->mAbilityId != 0 && (mResult == 2 || (mResult == 3 && list->Units()[a].mPlayer != list->Units()[b].mPlayer))) {
                    SpriteManager::Get();
                    SpriteManager::sFade[0] = FADE_MODE_DOWN;
                    mState++;
                    break;
                }
                SpriteManager::Get().StartFade(FADE_MODE_DOWN);
                mState = 0xf;
                break;
            }
            case 12:
                if (SpriteManager::Get().IsFading()) {
                    break;
                }
                mContext->mRequest = 0;
                CloseCommandMapNow(mCommandMap);
                FreeBakuganModelInstance(mBakugan);
                mVisible = 0;
                mState++;
                break;
            case 13: {
                SceneContext *context = mContext;
                if (context->mRequest == 0) {
                    break;
                }
                context->mRequest = 0;
                new BattleAbility();
                mState++;
                break;
            }
            case 14: {
                SceneContext *context = mContext;
                switch (context->mRequest) {
                    case 1:
                        context->mRequest = 0;
                        CloseCommandMapNow(mCommandMap);
                        mVisible = 0;
                        mState   = 0x10;
                        break;
                    case 2:
                        context->mRequest = 1;
                        mState         = 0x10;
                        break;
                    default:
                        if (mUnk_1e7_1) {
                            mUnk_1e7_1 = 0;
                            func_ov007_0225ce08();
                        }
                        break;
                }
                break;
            }
            case 15:
                if (SpriteManager::Get().IsFading()) {
                    break;
                }
                mContext->mRequest = 0;
                CloseCommandMapNow(mCommandMap);
                mVisible = 0;
                mState++;
                break;
            case 16: {
                mPopupEnabled = 0;
                if (mContext->mRequest == 0) {
                    break;
                }
                mResultFrames                                                  = 0;
                data_020d433c.mBattlePlayers->mPlayers[mPlayer]->mAbilityId = 0;
                if (mSpKind != 0) {
                    data_020d433c.mBattlePlayers->mPlayers[mPlayer]->mSpGauge = 0;
                }
                switch (mResult) {
                    case 0:
                    case 1:
                        RequestScene(1);
                        mFinished = 1;
                        break;
                    case 2:
                        RequestScene(2);
                        mFinished = 1;
                        break;
                    case 3:
                        RequestScene(3);
                        mFinished = 1;
                        break;
                    case 4:
                        RequestScene(4);
                        mFinished = 1;
                        break;
                }
                break;
            }
            case 17: {
                SceneContext *context = mContext;
                if (context->mRequest == 0) {
                    break;
                }
                if (mUnk_1ec != 0) {
                    RequestScene(6);
                    mFinished = 1;
                } else {
                    RequestScene(5);
                    mFinished = 1;
                }
                break;
            }
        }
    }
    if (!mVisible) {
        return;
    }
    if (mBakugan) {
        mBakugan->Update();
    }
    BakuganModel *pose = mBakugan;
    if (pose && pose->mVisible && (mInField || mFloorOutsideField) && mCommandMap) {
        if (mPushInputStopped == 0) {
            DrawShadowPieceXZ(mCommandMap, &mPrevPosition);
        } else {
            DrawShadowPieceXZ(mCommandMap, &pose->mPosition);
        }
    }
    UpdateEffects();
    UpdateCameraAndDraw();
}

void BattleTarget::func_ov007_02253fbc() {
    if (mState == 0x11) {
        return;
    }
    mState = 0x11;
    mContext->mRequest = 0;
    CloseCommandMapNow(mCommandMap);
    mBakugan->mVisible = 0;
    data_020d433c.mFlags &= 8;
    if (data_020d433c.mFlags) {
        mUnk_1ec = 1;
    }
}

void BattleTarget::Init() {
    BattlePlayerList *list = data_020d433c.mBattlePlayers;
    BattleRecord *record   = &list->mPlayers[mPlayer]->mRecords[mRecord];
    LoadEffectGroup(data_020d433c.mEffects, 1, "shoot");
    mCommandMap = new BattleCommandMap(data_020d433c.mFieldId);
    mCommandMap->mUseStatusPanels = 0;
    mCommandMap->Init();
    SetPredictVisible(mCommandMap, 1);
    LoadUnitCollisions();
    LoadMapCollision();
    mPushVector.x = 0;
    mPushVector.y = 0;
    mPushVector.z = -0x1000;
    mBakugan = new BakuganModel();
    if ((u8) (record->mBakuganId + 0xfb) <= 1) {
        record->mBakuganId = list->mRandomValue % 2 + 5;
    }
    mBakugan->Setup(record->mBakuganId, record->mAttribute, BAKUGAN_MODEL_MODE_H_NO_LOOP);
    mBakugan->mVisible = 0;
    PlaceBakugan();
    mStatusPanels = new BattleStatusPanels(0x1000, 0, 1, 1);
    u8 i = 0;
    do {
        u8 slot = data_020d433c.mBattlePlayers->mTurnOrder[i];
        if (slot == mPlayer) {
            mStatusPanels->SetupPanel(slot, (s8) data_020d433c.mBattlePlayers->mPlayers[slot]->mCharacterId, data_ov007_0227a750[i], 2);
        }
        i++;
    } while (i < data_020d433c.mPlayerCount);
    mCamera->SetUp(0, 0x1000, 0);
    mCamera->SetClip(0x1000, 0x1388000);
    mCamera->SetFov(0x2aaa);
    CameraManager::Get().mCameraIndex = 0;
    CameraManager::Get().SetCamerasAndRequestMode(1, mCamera, 0);
    SnapshotCamera();
    Camera *camera = mCamera;
    mCameraBasePosition.x     = camera->mPosition.x;
    mCameraBasePosition.y     = camera->mPosition.y;
    mCameraBasePosition.z     = camera->mPosition.z;
    ChangeDir("/Game/Battle/Obj/");
    SpriteScreen &screen = SpriteManager::Get().Screen(0);
    screen.LoadCharacters("battle_shoot_b_gp.nce_ncg1D.bin", 0, 0);
    screen.LoadPalette("battle_shoot_b_gp_ncl.bin", 0x140, 0);
    u8 j = 0;
    do {
        mCellAnims[j] = new CellAnimation("battle_shoot_b_gp_nce.bin", 0, 1, 1);
        mCellAnims[j]->mCharBase = screen.GetCharBase(0);
        mCellAnims[j]->mPaletteBase   = 10;
        j++;
    } while (j <= 3);
    mCellAnims[1]->SetAnimation(0);
    mCellAnims[0]->SetAnimation(1);
    mCellAnims[2]->SetAnimation(2);
    mCellAnims[3]->SetAnimation(3);
    SpriteScreen &screen1 = SpriteManager::Get().Screen(1);
    screen1.LoadCharacters("battle_b_obj_roulette.nce_ncg1D.bin", 0xa000, 0);
    mRoulettePalette = LoadFile("battle_b_obj_roulette_ncl.bin", 0, 0, 0, &mRoulettePaletteSize, 0);
    mCellAnims[4] = new CellAnimation("battle_b_obj_roulette_nce.bin", 2, 1, 1);
    mCellAnims[4]->mCharBase = screen1.GetCharBase(0xa000);
    mCellAnims[4]->mPaletteBase   = 0;
    mCellAnims[5] = new CellAnimation("battle_b_obj_roulette_nce.bin", 0, 1, 1);
    mCellAnims[5]->mCharBase = screen1.GetCharBase(0xa000);
    mCellAnims[5]->mPaletteBase   = 0;
    mCellAnims[6] = new CellAnimation("battle_b_obj_roulette_nce.bin", 6, 1, 1);
    mCellAnims[6]->mCharBase = screen1.GetCharBase(0xa000);
    mCellAnims[6]->mPaletteBase   = 0;
    ChangeDir("/");
}

void BattleTarget::PlaceBakugan() {
    VecFx32 pos;
    GetCornerPosition(mCommandMap, &pos);
    VecFx32 *dst = &mBakugan->mPosition;
    dst->x = pos.x;
    dst->y = pos.y;
    dst->z = pos.z;
    mPrevPosition.x = pos.x;
    mPrevPosition.y = pos.y;
    mPrevPosition.z = pos.z;
}

void BattleTarget::LoadUnitCollisions() {
    u16 rot[3];
    BattleUnit *units = data_020d433c.mBattlePlayers->mUnits;
    u8 i              = 0;
    do {
        BattleUnit *unit = &units[i];
        if (unit->mStatus != 0) {
            if (!mUnitCollisionBuffer) {
                mUnitCollisionBuffer = (u8 *) new GimmickBuffer;
                LoadCollisionFile(mUnitCollisionBuffer, 0xb);
            }
            rot[1] = unit->mRotY;
            rot[0] = 0;
            rot[2] = 0;
            mUnitCollisions[i] = AddCollisionSet(data_020d433c.mGimmickCollision, COLLISION_GROUP_UNIT, mUnitCollisionBuffer, &unit->mPos, rot, i + 1);
        }
        i++;
    } while (i < 12);
}

void BattleTarget::LoadMapCollision() {
    char buf[0x804];
    VecFx32 pos;
    u16 rot[3];
    pos.x = 0;
    pos.y = 0;
    pos.z = 0;
    rot[0] = 0;
    rot[1] = 0;
    rot[2] = 0;
    UnkStruct_020d433c *data = &data_020d433c;
    u8 id                    = data->mFieldId;
    if (data->mUnk_9a_4) {
        LoadCollisionFile(buf, id);
    } else {
        LoadCollisionFile(buf, id + 0x64);
    }
    mMapCollision = AddCollisionSet(data_020d433c.mGimmickCollision, COLLISION_GROUP_FIELD, buf, &pos, rot, 0);
}

void BattleTarget::SetMapPredictVisible(u32 unk) {
    SetPredictVisible(mCommandMap, unk);
}

BOOL BattleTarget::IsHeadingToward(VecFx32 *a, VecFx32 *b, VecFx32 *vel, u8 index) {
    VecFx32 diff;
    VecFx32 dir = *vel;
    if (vel->x == 0 && vel->y == 0 && vel->z == 0) {
        return 0;
    }
    VEC_Subtract(b, a, &diff);
    dir.y  = 0;
    diff.y = 0;
    fx32 mag = VEC_Mag(vel);
    if (mag > sTables.mHeadingMinSpeeds[index]) {
        VEC_Normalize(&diff, &diff);
        VEC_Normalize(&dir, &dir);
        fx32 dot = VEC_DotProduct(&diff, &dir);
        if (dot > 0x1000) {
            dot = 0x1000;
        }
        if (FX_AcosIdx(dot) <= sTables.mHeadingAngleLimits[index]) {
            return 1;
        }
    }
    return 0;
}

void BattleTarget::LaunchHitUnit(GimmickSetData *set) {
    BattleMoveState state;
    state.mVelocity.x = 0;
    state.mVelocity.y = 0;
    state.mVelocity.z = 0;
    state.mRot[0]     = 0;
    state.mRot[1]     = 0;
    state.mRot[2]     = 0;
    state.mMode       = MOVE_MODE_LAUNCH_SPIN_Y;
    if (!set || !set->mTag) {
        return;
    }
    BattlePlayerList *list = data_020d433c.mBattlePlayers;
    ReleaseSlot(list, set->mTag - 1);
    BakuganModel *other = list->mUnitModels->GetBakugan(set->mTag - 1);
    GetVelocityTowardXZ(&mBakugan->mPosition, &other->mPosition, &mVelocity, &state.mVelocity);
    list->mUnitModels->StartMove(set->mTag - 1, &state);
    RemoveCollisionSet(data_020d433c.mGimmickCollision, set);
    mUnitCollisions[set->mTag - 1] = 0;
}




void BattleTarget::UpdateCollisions(VecFx32 *a, VecFx32 *b, u32 *c, u32 *d) {
    VecFx32 move   = {0, 0, 0};
    VecFx32 hitOut = {0, 0, 0};
    VecFx32 vel    = {0, 0, 0};
    ObstacleBuf self;
    ObstacleBuf obs;
    u8 hit;
    u32 none = 0xffff;
    s32 wallFree;
    u16 marker;
    s32 moved;
    s32 ceilingHit;
    u16 handle;
    BattleRecord *record;
    u8 i;
    marker     = 0;
    hit        = 0;
    mHitGimmickType   = 0;
    moved      = 0;
    ceilingHit = 0;
    wallFree   = 1;
    if (mMoveFrames != 0) {
        mInField = 0;
    }
    record = &data_020d433c.mBattlePlayers->mPlayers[mPlayer]->mRecords[mRecord];
    if (mCollision->mHitCount != 0) {
        i = 0;
        do {
            GimmickSetData *set = &data_020d433c.mGimmickCollision->mSets[mCollision->mHitSlots[i]];
            if (!set) {
                continue;
            }
            self = *(ObstacleBuf *) ((u8 *) mCollision + 0x10);
            obs = *(ObstacleBuf *) OBST(set, mCollision->mHitObstacleIndices[i]);
            switch (set->mMode) {
                case COLLISION_GROUP_UNIT: {
                    if (!set->mTag) {
                        continue;
                    }
                    BattleUnit *unit = &data_020d433c.mBattlePlayers->mUnits[(u8) (set->mTag - 1)];
                    if (unit->mStatus == 2 || mSpKind == 5) {
                        continue;
                    }
                    if (mSpKind == 6 && unit->mPlayer != mPlayer && unit->mPlayer != mPartner && unit->mStatus != 3) {
                        SoundManager::Get().StartSeqArc(0x10);
                        LaunchHitUnit(set);
                        continue;
                    }
                    if (mSpKind == 4 && unit->mPlayer != mPlayer && unit->mPlayer != mPartner && unit->mStatus != 3) {
                        mUnk_1dd = 3;
                        marker   = none;
                        continue;
                    }
                    if (mShotSpKind == 3 && unit->mStatus != 3) {
                        VecFx32 v           = unit->mPos;
                        v.y                 = 0x8999;
                        mBakugan->mPosition = v;
                        mVelocity.x           = 0;
                        mVelocity.y           = 0;
                        mVelocity.z           = 0;
                        mBaseVelocity.x           = 0;
                        mBaseVelocity.y           = 0;
                        mBaseVelocity.z           = 0;
                        mPushPerFrame.x           = 0;
                        mPushPerFrame.y           = 0;
                        mPushPerFrame.z           = 0;
                        mPushVelocity.x           = 0;
                        mPushVelocity.y           = 0;
                        mPushVelocity.z           = 0;
                        continue;
                    }
                    *c = 0x666;
                    *d = 0x4cd;
                    VEC_Add(&self.o.mPos, &mCollision->mPosition, &self.o.mPos);
                    AddRotAtOnce(&self.o, mCollision);
                    VEC_Add(&obs.o.mPos, &set->mPosition, &obs.o.mPos);
                    AddRotAtOnceFields(&obs.o, set);
                    marker = 0;
                    if (data_020d433c.mTutorialId == 0 && unit->mPlayer != mPlayer && unit->mPlayer != mPartner && unit->mStatus != 3 &&
                        (mSkipHeadingTest != 0 || IsHeadingToward(&self.o.mPos, &obs.o.mPos, a, ((RecordBits *) ((u8 *) record + 2))->mIndex))) {
                        mUnk_1dd = 6;
                        mUnk_1db = 0;
                        mUnitHitBakuganPosition = mBakugan->mPosition;
                        mHitUnitPosition = obs.o.mPos;
                        mHitUnitHandle = set->mTag;
                        mUnitHitVelocity = *a;
                        mPushInputStopped = 1;
                    } else {
                        GetObstacleNormal((BattleSphere *) &self.o, a, &obs.o, &hitOut, 0, 0);
                        SoundManager::Get().StartSeqArc(4);
                    }
                    VEC_Add(&move, &hitOut, &move);
                    if (!(move.x == 0 && move.y == 0 && move.z == 0)) {
                        VEC_Normalize(&move, &move);
                    }
                    moved = 1;
                    break;
                }
                case COLLISION_GROUP_FIELD: {
                    u8 kind = OBST(set, mCollision->mHitObstacleIndices[i])->mKind;
                    switch (kind) {
                        case 0:
                        case 2: {
                            if (kind == 2) {
                                wallFree = 0;
                            }
                            if (kind == 2 && mKind2ObstaclesEnabled == 0) {
                                continue;
                            }
                            if (kind == 2) {
                                if (VEC_Mag(&mVelocity) >= 0x2000) {
                                    *c = 0xccd;
                                } else {
                                    *c = 0x1333;
                                }
                            } else {
                                *c = 0x800;
                            }
                            *d = 0x4cd;
                            VEC_Add(&self.o.mPos, &mCollision->mPosition, &self.o.mPos);
                            AddRotAtOnce(&self.o, mCollision);
                            VEC_Add(&obs.o.mPos, &set->mPosition, &obs.o.mPos);
                            AddRotAtOnceFields(&obs.o, set);
                            vel.x = a->x;
                            vel.y = a->y;
                            vel.z = a->z;
                            vel.y = vel.y * (none - 0x10000);
                            if (OBST(set, mCollision->mHitObstacleIndices[i])->mKind == 2) {
                                GetObstacleNormal((BattleSphere *) &self.o, &vel, &obs.o, &move, 1, &hit);
                            } else {
                                GetObstacleNormal((BattleSphere *) &self.o, &vel, &obs.o, &move, 0, &hit);
                            }
                            if (move.x != 0 || move.z != 0) {
                                SoundManager::Get().StartSeqArc(4);
                            }
                            marker = 0;
                            if (move.y != 0 && OBST(set, mCollision->mHitObstacleIndices[i])->mKind == 0) {
                                mPrevPosition.y = set->mPosition.y + obs.o.mPos.y + obs.o.mHalfY + self.o.mHalfY;
                            }
                            if (OBST(set, mCollision->mHitObstacleIndices[i])->mKind == 2) {
                                ceilingHit = 1;
                                if (move.x != 0 || move.z != 0) {
                                    handle = StartEffect(data_020d433c.mEffects, 1, 0xd, 0, 1);
                                    if (handle != 0) {
                                        CameraManager::Get();
                                        MtxFx43 mtx = data_020c1cf4;
                                        VecFx32 pos = mBakugan->mPosition;
                                        MTX_SetTranslation43(&mtx, &pos);
                                        SetEffectMatrix(data_020d433c.mEffects, handle, &mtx);
                                    }
                                }
                            }
                            break;
                        }
                        case 1:
                            mInField = 1;
                            break;
                    }
                    break;
                }
                case COLLISION_GROUP_GIMMICK:
                case COLLISION_GROUP_GIMMICK_HIT_BY_SP5:
                    *c = 0x4cd;
                    *d = 0x29;
                    vel.x = a->x;
                    vel.y = a->y;
                    vel.z = a->z;
                    HandleGimmickHit(set, mCollision->mHitObstacleIndices[i], &vel, &move);
                    marker = 0;
                    break;
            }
        } while (++i < mCollision->mHitCount);
    }
    if (wallFree != 0) {
        if (mKind2ObstaclesEnabled == 0) {
            mKind2ObstaclesEnabled = 1;
        }
    }
    if (ceilingHit != 0) {
        if (mInField == 0) {
            mInField = 1;
        }
    }
    if (a->y > 0 && move.y > 0) {
        move.y = 0;
    }
    if (!(move.x == 0 && move.y == 0 && move.z == 0)) {
        VecFx32 pos = mBakugan->mPosition;
        mCollided    = 1;
        if (move.x != 0) {
            pos.x = mPrevPosition.x;
        }
        s32 movedNow = moved;
        if (move.y != 0) {
            pos.y = mPrevPosition.y;
        }
        if (move.z != 0) {
            pos.z = mPrevPosition.z;
        }
        func_ov007_022337ec(&mVelocity, &mBaseVelocity, &mPushVelocity, &mUnk_b8, &move, *c, *d, movedNow);
        fx32 mag = VEC_Mag(&mBaseVelocity);
        if (moved != 0 && mag <= 0x1000 && move.y != 0) {
            if (ABS(move.x) <= 0x800) {
                if (move.x < 0) {
                    mBaseVelocity.x = -0x800;
                } else {
                    mBaseVelocity.x = 0x800;
                }
            } else {
                mBaseVelocity.x = move.x;
            }
            if (ABS(move.z) <= 0x800) {
                if (move.z < 0) {
                    mBaseVelocity.z = -0x800;
                } else {
                    mBaseVelocity.z = 0x800;
                }
            } else {
                mBaseVelocity.z = move.z;
            }
            mBaseVelocity.y += 0x1000;
        }
        if (move.y != 0 && marker != none) {
            mAirFrames = none - 0x10000;
        }
        mBakugan->mPosition = pos;
    }
    if (move.y != 0) {
        mVerticalContactY = mPrevPosition.y;
    }
    if (mInField == 0) {
        mResult = 0;
        mPushInputStopped = 1;
        if (mResultFrames == 0) {
            mResultFrames = 1;
        }
    }
    *b = move;
}

void BattleTarget::UpdateMove() {
    u32 outA = 0;
    u32 outB = 0;
    mBounceFactor = 0x533;
    if (func_ov007_022583b8()) {
        if (!func_ov007_02259af0()) {
            if (TryRestOnCell()) {
                return;
            }
            UpdateCollisions(&mVelocity, &mCollisionNormal, &outA, &outB);
        }
        StopSoundAfterVentKaze();
        mForceAirborne = 0;
        if (data_020d433c.mTutorialId == 0 && mUnk_1dd != 4) {
            if ((mAirFrames == 0 && mAirFramesBeforeIntegrate != -1) || mAirFrames == -1) {
                if (mPushScaleTimer != 0) {
                    BattlePlayer *player = data_020d433c.mBattlePlayers->mPlayers[mPlayer];
                    if (player->mMicCharge != 0) {
                        u8 n = player->mMicBounceCount;
                        if (n < 3) {
                            mBaseVelocity.y += (3 - n) * 0x2666 / 3;
                            SoundManager::Get().StartSeqArc(0x43);
                            n        = n + 1;
                            mForceAirborne = 1;
                        }
                        data_020d433c.mBattlePlayers->mPlayers[mPlayer]->mMicBounceCount = n;
                        VEC_Add(&mBaseVelocity, &mPushVelocity, &mBaseVelocity);
                        mPushVelocity.x = 0;
                        mPushVelocity.y = 0;
                        mPushVelocity.z = 0;
                    }
                    player = data_020d433c.mBattlePlayers->mPlayers[mPlayer];
                    if (player->mOtherMicCharge != 0) {
                        u8 n = player->mOtherMicBounceCount;
                        if (n < 3) {
                            mBaseVelocity.y += (3 - n) * 0x2666 / 3;
                            SoundManager::Get().StartSeqArc(0x43);
                            n        = n + 1;
                            mForceAirborne = 1;
                        }
                        data_020d433c.mBattlePlayers->mPlayers[mPlayer]->mOtherMicBounceCount = n;
                        VEC_Add(&mBaseVelocity, &mPushVelocity, &mBaseVelocity);
                        mPushVelocity.x = 0;
                        mPushVelocity.y = 0;
                        mPushVelocity.z = 0;
                    }
                }
            }
        }
        BakuganModel *pose = mBakugan;
        mPrevPosition.x                = pose->mPosition.x;
        mPrevPosition.y                = pose->mPosition.y;
        mPrevPosition.z                = pose->mPosition.z;
        mPrevVelocity.x                = mVelocity.x;
        mPrevVelocity.y                = mVelocity.y;
        mPrevVelocity.z                = mVelocity.z;
        mPrevLanded                 = mLanded;
        mPrevHitGimmickType                 = mHitGimmickType;
        mAirFramesBeforeIntegrate                  = mAirFrames;
        if (mUnk_1dd != 6) {
            pose = mBakugan;
            IntegrateMotion(&pose->mPosition, &pose->mRotX, &mVelocity, &mBaseVelocity, &mPushVelocity, &mAirFrames, mForceAirborne, 0);
        }
    } else {
        BakuganModel *pose = mBakugan;
        mPrevPosition.x                = pose->mPosition.x;
        mPrevPosition.y                = pose->mPosition.y;
        mPrevPosition.z                = pose->mPosition.z;
    }
    func_ov007_02258448();
    UpdateOnMarkerCell();
    StartLandingSound();
    mPrevAirFrames = mAirFrames;
    mMoveFrames++;
    if (mPushScaleTimer != 0) {
        if (mLanded != 0 && mMoveFrames % 30 == 0 && mSpKind != 2) {
            BattleRecord *record = &data_020d433c.mBattlePlayers->mPlayers[mPlayer]->mRecords[mRecord];
            mPushScale              = FX_Mul(mPushScale, sTables.mPushScaleFactors[record->mPushTableIndex]);
        }
        if (mSpKind != 2) {
            mPushScaleTimer--;
        }
    } else {
        mPushScaleZeroFrames++;
        mPushScale = 0;
    }
}

void BattleTarget::StopSoundAfterVentKaze() {
    if (mPrevHitGimmickType != GIMMICK_TYPE_VENT_KAZE) {
        return;
    }
    if (mHitGimmickType == GIMMICK_TYPE_VENT_KAZE) {
        return;
    }
    StopSeqArc(&SoundManager::Get(), 0xf, 0x3c);
}

void BattleTarget::StartLandingSound() {
    if (mAirFrames != 0) {
        return;
    }
    if (mPrevAirFrames < 10) {
        return;
    }
    SoundManager::Get().StartSeqArc(4);
}

void BattleTarget::func_ov007_02255b78(VecFx32 *b, VecFx32 *c, VecFx32 *in, VecFx32 *out) {
    VecFx32 diff;
    MtxFx33 mtx;
    diff.x = 0;
    diff.z = 0;
    diff.y = 0;
    VEC_Subtract(c, b, &diff);
    int angle = -(FX_Atan2Idx(diff.z, diff.x) + 0x4000);
    mtx       = gIdentityMtx33;
    MTX_ApplyRotY33(&mtx, angle);
    MTX_MultVec33(in, &mtx, out);
}




void BattleTarget::func_ov007_02255c04(VecFx32 *vec, s32 *out) {
    VecFx32 target;
    VecFx32 diff;
    VecFx32 rotated;
    MtxFx33 mtx = gIdentityMtx33;
    Camera *camera = mCamera;
    target.x = camera->mTarget.x;
    target.y = camera->mTarget.y;
    target.z = camera->mTarget.z;
    VEC_Subtract(&mCameraBasePosition, &target, &diff);
    int angle = -(FX_Atan2Idx(diff.x, diff.z) + 0x4000);
    MTX_ApplyRotY33(&mtx, angle);
    MTX_MultVec33(vec, &mtx, &rotated);
    out[1] = rotated.x;
    out[0] = rotated.z;
}

void BattleTarget::UpdateCameraPosition(VecFx32 *target) {
    VecFx32 pos;
    VecFx32 u;
    VecFx32 cam;
    VecFx32 curr;
    VecFx32 diff;
    MtxFx33 rot;
    MtxFx33 ident;
    VecFx32 v;
    MtxFx33 m0;
    m0    = gIdentityMtx33;
    rot   = m0;
    ident = m0;
    BattlePlayer *player = data_020d433c.mBattlePlayers->mPlayers[mPlayer];
    u8 step = mState;
    if (step >= 0xb) {
        return;
    }
    switch (mCameraMode) {
        case 0:
            if (step > 1) {
                mUnk_11c.x = mUnk_11c.x * 15 / 16;
                mUnk_11c.y = mUnk_11c.y * 15 / 16;
                mUnk_11c.z = mUnk_11c.z * 15 / 16;
                mRotZAdjust   = mRotZAdjust * 15 / 16;
            }
            switch (mResult) {
                case 0:
                case 1: {
                    s32 k = mTornadoCameraPullback;
                    u.x   = k << 13;
                    u.y   = k * 0x333;
                    u.z   = k << 13;
                    VecFx32 tp;
                    tp.z = mUnk_11c.z + 0x37000 + (k << 13);
                    tp.y = u.y + (mUnk_11c.y + 0x19000);
                    tp.x = mUnk_11c.x + 0x37000 + (k << 13);
                    pos = tp;
                    MTX_ApplyRotY33(&rot, (u16) (sTables.mBaseRotY[player->mCorner] - mRotYAdjust));
                    MTX_MultVec33(&pos, &rot, &pos);
                    MTX_MultVec33(&mCameraOffset, &rot, &mCameraOffset);
                    pos.x = FX_Mul(pos.x, ident._[4]);
                    pos.z = FX_Mul(pos.z, ident._[8]);
                    if (target == 0) {
                        SetVec(&cam, pos.x + mBakugan->mPosition.x, pos.y + mBakugan->mPosition.y + 0x4000, pos.z + mBakugan->mPosition.z);
                    } else {
                        SetVec(&cam, target->x + pos.x, pos.y + mBakugan->mPosition.y + 0x4000, target->z + pos.z);
                    }
                    curr = mCameraBasePosition;
                    VEC_Subtract(&curr, &cam, &diff);
                    s32 scale = mCameraEaseFactor;
                    diff.x    = FX_Mul(diff.x, scale);
                    diff.y    = FX_Mul(diff.y, scale);
                    diff.z    = FX_Mul(diff.z, scale);
                    VEC_Add(&cam, &diff, &mCameraBasePosition);
                    VecFx32 t;
                    t.z = mCameraOffset.z + (cam.z + diff.z);
                    t.y = mCameraOffset.y + (cam.y + diff.y);
                    t.x = mCameraOffset.x + (cam.x + diff.x);
                    mCamera->mPosition = t;
                    return;
                }
                case 2:
                case 3: {
                    pos.x = 0x46000;
                    pos.y = 0x32000;
                    pos.z = 0x46000;
                    MTX_ApplyRotY33(&rot, mBakugan->mRotY);
                    MTX_MultVec33(&pos, &rot, &pos);
                    if (target == 0) {
                        SetVec(&mCamera->mPosition, pos.x + mBakugan->mPosition.x, pos.y, pos.z + mBakugan->mPosition.z);
                    } else {
                        SetVec(&mCamera->mPosition, target->x + pos.x, pos.y, target->z + pos.z);
                    }
                    Camera *c = mCamera;
                    mCameraBasePosition.x = c->mPosition.x;
                    mCameraBasePosition.y = c->mPosition.y;
                    mCameraBasePosition.z = c->mPosition.z;
                    return;
                }
                case 4: {
                    u8 slot = mUnitHitCount;
                    if (slot == 0) {
                        return;
                    }
                    if (mSpKind == 1) {
                        pos.x = sTables.mSpKind1HitCameraOffsets[slot - 1].x;
                        pos.y = sTables.mSpKind1HitCameraOffsets[slot - 1].y;
                        pos.z = sTables.mSpKind1HitCameraOffsets[slot - 1].z;
                    } else {
                        pos.x = sTables.mHitCameraOffsets[slot - 1].x;
                        pos.y = sTables.mHitCameraOffsets[slot - 1].y;
                        pos.z = sTables.mHitCameraOffsets[slot - 1].z;
                    }
                    MTX_ApplyRotY33(&rot, mBakugan->mRotY);
                    MTX_MultVec33(&pos, &rot, &pos);
                    if (target == 0) {
                        SetVec(&mCamera->mPosition, pos.x + mBakugan->mPosition.x, pos.y, pos.z + mBakugan->mPosition.z);
                    } else {
                        SetVec(&mCamera->mPosition, target->x + pos.x, pos.y, target->z + pos.z);
                    }
                    Camera *c = mCamera;
                    mCameraBasePosition.x = c->mPosition.x;
                    mCameraBasePosition.y = c->mPosition.y;
                    mCameraBasePosition.z = c->mPosition.z;
                    return;
                }
                default:
                    break;
            }
            break;
        case 1: {
            VecFx32 tp;
            tp.z = mUnk_11c.z + 0x37000;
            tp.y = mUnk_11c.y + 0x19000;
            tp.x = mUnk_11c.x + 0x37000;
            pos = tp;
            MTX_ApplyRotY33(&rot, (u16) (sTables.mBaseRotY[player->mCorner] - mRotYAdjust));
            MTX_MultVec33(&pos, &rot, &pos);
            pos.x = FX_Mul(pos.x, ident._[4]);
            pos.z = FX_Mul(pos.z, ident._[8]);
            VecFx32 t;
            t.z = pos.z + mBakugan->mPosition.z;
            t.y = pos.y + mBakugan->mPosition.y + 0x4000;
            t.x = pos.x + mBakugan->mPosition.x;
            cam = t;
            SetVec(&v, mBakugan->mPosition.x, sTables.mUnk_104[data_020d433c.mFieldId], mBakugan->mPosition.z);
            if (v.x < 0) {
                v.x = v.x - 0x14000;
            } else {
                v.x = v.x + 0x14000;
            }
            if (v.z < 0) {
                v.z = v.z - 0x14000;
            } else {
                v.z = v.z + 0x14000;
            }
            RotateVecY(&v, mCameraRotY, &v);
            VEC_Subtract(&v, &cam, &diff);
            fx32 dz = FX_Div(diff.z, 0x14000);
            fx32 dy = FX_Div(diff.y, 0x14000);
            fx32 dx = FX_Div(diff.x, 0x14000);
            u.x     = dx;
            u.y     = dy;
            u.z     = dz;
            int scale = INT_TO_FX(mUnk_17c);
            u.x       = FX_Mul(u.x, scale);
            u.y       = FX_Mul(u.y, scale);
            u.z       = FX_Mul(u.z, scale);
            VEC_Add(&cam, &u, &cam);
            Camera *c      = mCamera;
            c->mPosition.x = cam.x;
            c->mPosition.y = cam.y;
            c->mPosition.z = cam.z;
            return;
        }
        case 2: {
            pos.x = -0x32000;
            pos.y = 0x5000;
            pos.z = 0x32000;
            MTX_ApplyRotY33(&rot, mBakugan->mRotY);
            MTX_MultVec33(&pos, &rot, &pos);
            VecFx32 t;
            t.z = pos.z + mBakugan->mPosition.z;
            t.y = pos.y;
            t.x = pos.x + mBakugan->mPosition.x;
            mCamera->mPosition = t;
            return;
        }
        case 3: {
            if (mVelocity.x == 0 && mVelocity.z == 0) {
                return;
            }
            pos.x = 0x37000;
            pos.y = 0xf000;
            pos.z = 0x37000;
            MTX_ApplyRotY33(&rot, (u16) (FX_Atan2Idx(mVelocity.x, mVelocity.z) - 0xa000));
            MTX_MultVec33(&pos, &rot, &pos);
            Camera *c = mCamera;
            cam.x     = c->mPosition.x;
            cam.y     = c->mPosition.y;
            cam.z     = c->mPosition.z;
            VEC_Add(&pos, &mBakugan->mPosition, &pos);
            VEC_Subtract(&cam, &pos, &u);
            s32 scale = mCameraEaseFactor;
            u.x       = FX_Mul(u.x, scale);
            u.y       = FX_Mul(u.y, scale);
            u.z       = FX_Mul(u.z, scale);
            SetVec(&mCamera->mPosition, cam.x - u.x, cam.y - u.y, cam.z - u.z);
            return;
        }
        case 4: {
            if (mPrevVelocity.x == 0 && mPrevVelocity.y == 0 && mPrevVelocity.z == 0) {
                return;
            }
            pos.x = 0x14000;
            pos.y = 0x14000;
            pos.z = 0x14000;
            MTX_ApplyRotY33(&rot, (u16) (FX_Atan2Idx(mPrevVelocity.x, mPrevVelocity.z) - 0xa000));
            s32 a = FX_Atan2Idx(mPrevVelocity.y, 0x1000);
            MTX_MultVec33(&pos, &rot, &pos);
            MTX_MultVec33(&mCameraOffset, &rot, &mCameraOffset);
            pos.y = FX_Mul(pos.y, -FX_SinCosTable_[(a >> 4) * 2]) + 0x5a000;
            cam.x = mCameraBasePosition.x;
            cam.y = mCameraBasePosition.y;
            cam.z = mCameraBasePosition.z;
            VEC_Add(&pos, &mBakugan->mPosition, &pos);
            VEC_Subtract(&cam, &pos, &u);
            s32 scale = mCameraEaseFactor;
            u.x       = FX_Mul(u.x, scale);
            u.y       = FX_Mul(u.y, scale);
            u.z       = FX_Mul(u.z, scale);
            SetVec(&mCameraBasePosition, cam.x - u.x, mBakugan->mPosition.y + 0x5a000, cam.z - u.z);
            VEC_Add(&cam, &mCameraOffset, &cam);
            SetVec(&mCamera->mPosition, cam.x - u.x, mBakugan->mPosition.y + 0x5a000, cam.z - u.z);
            return;
        }
        case 5:
            UpdateJumpObjCameraPosition();
            return;
        case 6:
            return;
    }
}

void BattleTarget::UpdateJumpObjCameraPosition() {
    VecFx32 c;
    VecFx32 b;
    VecFx32 a;
    VecFx32 dir;
    MtxFx33 mtx = gIdentityMtx33;
    switch (mJumpObjCameraStep) {
        case 0: {
            mCameraEaseFactor = 0x333;
            a.x     = mCameraBasePosition.x;
            a.y     = mCameraBasePosition.y;
            a.z     = mCameraBasePosition.z;
            VecFx32 t;
            t.z = -mVelocity.z;
            t.y = -mVelocity.y;
            t.x = -mVelocity.x;
            dir = t;
            if (VEC_Mag(&dir) == 0) {
                dir.x = -1;
                dir.y = -1;
                dir.z = -1;
            }
            VEC_Normalize(&dir, &dir);
            b = mBakugan->mPosition;
            b.x += FX_Mul(dir.x, 0x28000);
            b.y += FX_Mul(dir.y, 0x28000);
            b.z += FX_Mul(dir.z, 0x28000);
            VEC_Subtract(&a, &b, &c);
            VecFx32 t0;
            t0.z = FX_Mul(c.z, mCameraEaseFactor);
            t0.y = FX_Mul(c.y, mCameraEaseFactor);
            t0.x = FX_Mul(c.x, mCameraEaseFactor);
            b = t0;
            VecFx32 t1;
            t1.z = a.z - b.z;
            t1.y = a.y - b.y;
            t1.x = a.x - b.x;
            mCamera->mPosition = t1;
            VecFx32 t2;
            t2.z = a.z - b.z;
            t2.y = a.y - b.y;
            t2.x = a.x - b.x;
            mCameraBasePosition = t2;
            mUnk_17c--;
            if (mUnk_17c == 0) {
                mJumpObjCameraStep++;
            }
            break;
        }
        case 1: {
            mCameraEaseFactor = 0x148;
            a.x = mCameraBasePosition.x;
            a.y = mCameraBasePosition.y;
            a.z = mCameraBasePosition.z;
            VEC_Subtract(&mBakugan->mPosition, &a, &b);
            if (VEC_Mag(&b) >= 0x5f000) {
                mJumpObjCameraStep++;
                mResetCameraOffset = 1;
            }
            break;
        }
        case 2: {
            mCameraEaseFactor = 0x148;
            a.x = mCameraBasePosition.x;
            a.y = mCameraBasePosition.y;
            a.z = mCameraBasePosition.z;
            VEC_Subtract(&mBakugan->mPosition, &a, &b);
            c.x = 0x2f800;
            c.y = 0x2f800;
            c.z = 0x2f800;
            MTX_ApplyRotY33(&mtx, FX_Atan2Idx(mVelocity.x, mVelocity.z) - 0xa000);
            MTX_MultVec33(&c, &mtx, &c);
            MTX_MultVec33(&mCameraOffset, &mtx, &mCameraOffset);
            u16 pitch = FX_Atan2Idx(mVelocity.y, 0x1000);
            c.y       = FX_Mul(c.y, -FX_SinCosTable_[(pitch >> 4) * 2]);
            VEC_Add(&c, &mBakugan->mPosition, &c);
            VEC_Add(&c, &sTables.mJumpObjCameraOffset, &c);
            VEC_Subtract(&a, &c, &b);
            fx32 k = mCameraEaseFactor;
            b.x = FX_Mul(b.x, k);
            b.y = FX_Mul(b.y, k);
            b.z = FX_Mul(b.z, k);
            SetVec(&mCameraBasePosition, a.x - b.x, a.y - b.y, a.z - b.z);
            VEC_Subtract(&b, &mCameraOffset, &b);
            VecFx32 t;
            t.z = a.z - b.z;
            t.y = a.y - b.y;
            t.x = a.x - b.x;
            mCamera->mPosition = t;
            break;
        }
        case 3:
        case 4:
        case 5:
        case 6:
            break;
    }
}

void BattleTarget::UpdateCameraTarget(VecFx32 *target) {
    VecFx32 cam;
    VecFx32 diff;
    VecFx32 move;
    MtxFx33 mtx;
    if (mState >= 0xb) {
        return;
    }
    switch (mCameraMode) {
        case 0:
            if (target == 0) {
                target = &mBakugan->mPosition;
            }
            if (mState > 1) {
                mtx    = gIdentityMtx33;
                move.x = 0;
                move.y = 0;
                move.z = 0;
                diff.x = 0;
                diff.y = 0;
                diff.z = 0;
                Camera *camera = mCamera;
                cam.x          = camera->mPosition.x;
                cam.y          = camera->mPosition.y;
                cam.z          = camera->mPosition.z;
                VEC_Subtract(&cam, &mBakugan->mPosition, &diff);
                int angle = -(FX_Atan2Idx(diff.z, diff.x) + 0x4000);
                MTX_ApplyRotY33(&mtx, angle);
                MTX_MultVec33(&move, &mtx, &move);
                if (mCameraTargetWeight != 0) {
                    mCameraTargetWeight--;
                }
                mCameraTarget.x = (target->x + mCameraTarget.x * mCameraTargetWeight) / (mCameraTargetWeight + 1);
                mCameraTarget.y = (target->y + mCameraTarget.y * mCameraTargetWeight) / (mCameraTargetWeight + 1);
                mCameraTarget.z = (target->z + mCameraTarget.z * mCameraTargetWeight) / (mCameraTargetWeight + 1);
                mCameraTarget.x += move.x;
                mCameraTarget.z += move.z;
                mCameraTarget.y += move.y;
                Camera *cameraOut = mCamera;
                cameraOut->mTarget.x = mCameraTarget.x;
                cameraOut->mTarget.y = mCameraTarget.y;
                cameraOut->mTarget.z = mCameraTarget.z;
                return;
            }
            if (mState != 1) {
                return;
            }
            VecFx32 off0 = {0, 0, 0};
            off0.x = -mBakugan->mPosition.x;
            off0.z = -mBakugan->mPosition.z;
            RotateVecY(&off0, -mRotYAdjust, &mCameraTarget);
            VEC_Add(&mBakugan->mPosition, &mCameraTarget, &mCameraTarget);
            mCameraTarget.y = FX_Mul(FX_Div(INT_TO_FX(mRotZAdjust), 0xb6000), 0x2000);
            Camera *c = mCamera;
            c->mTarget.x = mCameraTarget.x;
            c->mTarget.y = mCameraTarget.y;
            c->mTarget.z = mCameraTarget.z;
            return;
        case 1: {
            VecFx32 off1 = {0, 0, 0};
            VecFx32 v;
            off1.x = -mBakugan->mPosition.x;
            off1.z = -mBakugan->mPosition.z;
            RotateVecY(&off1, -mRotYAdjust, &mCameraTarget);
            VEC_Add(&mBakugan->mPosition, &mCameraTarget, &mCameraTarget);
            mCameraTarget.y = FX_Mul(FX_Div(INT_TO_FX(mRotZAdjust), 0xb6000), 0x2000);
            v.x = mCameraTarget.x;
            v.y = mCameraTarget.y;
            v.z = mCameraTarget.z;
            fx32 vz = FX_Div(v.z, 0x14000);
            fx32 vy = FX_Div(v.y, 0x14000);
            fx32 vx = FX_Div(v.x, 0x14000);
            v.x     = vx;
            v.y     = vy;
            v.z     = vz;
            int scale = INT_TO_FX(mUnk_17c);
            v.x       = FX_Mul(v.x, scale);
            v.y       = FX_Mul(v.y, scale);
            v.z       = FX_Mul(v.z, scale);
            VEC_Subtract(&mCameraTarget, &v, &mCameraTarget);
            Camera *c = mCamera;
            c->mTarget.x = mCameraTarget.x;
            c->mTarget.y = mCameraTarget.y;
            c->mTarget.z = mCameraTarget.z;
            return;
        }
        case 2: {
            BakuganModel *pose = mBakugan;
            SetVec(&mCamera->mTarget, pose->mPosition.x, mCameraShakeY + pose->mPosition.y, pose->mPosition.z);
            mCameraShakeY                 = FX_Mul(mCameraShakeY, -0xf33);
            return;
        }
        case 3: {
            BakuganModel *pose = mBakugan;
            SetVec(&mCamera->mTarget, pose->mPosition.x, pose->mPosition.y, pose->mPosition.z);
            return;
        }
        case 4: {
            BakuganModel *pose = mBakugan;
            SetVec(&mCamera->mTarget, pose->mPosition.x, pose->mPosition.y + 0x64cd, pose->mPosition.z);
            return;
        }
        case 5: {
            BakuganModel *pose = mBakugan;
            SetVec(&mCamera->mTarget, pose->mPosition.x, pose->mPosition.y + 0x64cd, pose->mPosition.z);
            mCameraTarget = mCamera->mTarget;
            return;
        }
        case 6:
            return;
    }
}

void BattleTarget::UpdateCamera() {
    UpdateCameraPosition(0);
    UpdateCameraTarget(0);
    SnapshotCamera();
}

void BattleTarget::UpdateCameraAndDraw() {
    data_020d433c.mFlags &= 8;
    if (data_020d433c.mFlags == 0) {
        UpdateCamera();
    }
    if (mStatusPanels) {
        mStatusPanels->DrawPanels();
        DrawBar();
    }
    if (mState != 0xd && mState != 0xe) {
        DrawRecordTotal(0xff, 0xff);
    }
    if (mBakugan) {
        mBakugan->DrawModel();
    }
}

void BattleTarget::IntegrateMotion(VecFx32 *pos, u16 *rot, VecFx32 *vel, VecFx32 *a, VecFx32 *b, u32 *counter, u8 flag,
                                       u32 mode) {
    float angle = 0;
    VecFx32 tmp = {0, 0, 0};
    (*counter)++;
    int speed = 0;
    if (mode == 0) {
        fx32 len;
        if (VEC_Mag(b) > 0x800 || VEC_Mag(a) <= 0) {
            len = FX_Atan2(b->x, b->z);
            speed = ABS(b->z) + ABS(b->x);
        } else {
            len = FX_Atan2(vel->x, vel->z);
            speed = ABS(vel->z) + ABS(vel->x);
        }
        fx32 q = FX_Div(len, 0x47);
        angle  = (float) q / 4096.0f;
        if (angle < 0.0f) {
            angle += 360.0f;
        }
    }
    LimitSumLength(&mPushVelocity, &mPushPerFrame);
    VEC_Add(&mPushVelocity, &mPushPerFrame, &mPushVelocity);
    mPushPerFrame.x = 0;
    mPushPerFrame.y = 0;
    mPushPerFrame.z = 0;
    if (flag == 0) {
        if (pos->y >= 0x899a && *counter != 0) {
            goto airborne;
        }
        if (mInField != 0 || mFloorOutsideField != 0) {
            b->y = 0;
            if (a->y < 0) {
                a->y = FX_Mul(a->y, -mBounceFactor);
            }
            if (ABS(a->y) <= 0x19a) {
                a->y = 0;
            }
            if (ABS(a->x) <= 0xcd) {
                a->x = 0;
            } else {
                a->x = FX_Mul(a->x, 0xf85);
            }
            if (ABS(a->z) <= 0xcd) {
                a->z = 0;
            } else {
                a->z = FX_Mul(a->z, 0xf85);
            }
            if (mMoveFrames % 5 == 0) {
                if (ABS(b->x) <= 0xcd) {
                    b->x = 0;
                } else {
                    b->x = FX_Mul(b->x, 0xecd);
                }
                if (ABS(b->z) <= 0xcd) {
                    b->z = 0;
                } else {
                    b->z = FX_Mul(b->z, 0xecd);
                }
            }
            if (ABS(mUnk_b8.x) <= 0xcd) {
                mUnk_b8.x = 0;
            } else {
                mUnk_b8.x = FX_Mul(mUnk_b8.x, 0xfae);
            }
            if (ABS(mUnk_b8.y) <= 0xcd) {
                mUnk_b8.y = 0;
            } else {
                mUnk_b8.y = FX_Mul(mUnk_b8.y, 0xfae);
            }
            if (ABS(mUnk_b8.z) <= 0xcd) {
                mUnk_b8.z = 0;
            } else {
                mUnk_b8.z = FX_Mul(mUnk_b8.z, 0xfae);
            }
            if (pos->y < 0x899a) {
                mVerticalContactY = 0x899a;
                pos->y   = 0x8999;
            }
            *counter = 0;
        } else {
            a->y = a->y - mGravityPerFrame;
        }
        if (a->y > 0x29) {
            VecFx32 vt;
            vt.z = a->z + FX_Mul(b->z, 0x1000);
            vt.y = a->y + FX_Mul(b->y, 0x1000);
            vt.x = a->x + FX_Mul(b->x, 0x1000);
            *vel = vt;
        } else {
            VecFx32 vt;
            vt.z = a->z + b->z;
            vt.y = a->y + b->y;
            vt.x = a->x + b->x;
            *vel = vt;
        }
        mLanded = 1;
    } else {
    airborne:
        a->y = a->y - mGravityPerFrame;
        switch (mPushMode) {
            case 0:
                { VecFx32 vt; vt.z = a->z; vt.y = a->y; vt.x = a->x; *vel = vt; }
                break;
            case 1:
                VecFx32 vt;
                vt.z = a->z + b->z;
                vt.y = a->y + b->y;
                vt.x = a->x + b->x;
                *vel = vt;
                break;
        }
    }
    VEC_Add(vel, &mUnk_b8, vel);
    if (b->x != 0 || b->z != 0 || a->x != 0 || a->y != 0 || a->z != 0 || mUnk_b8.x != 0 || mUnk_b8.z != 0) {
        if (mode == 0) {
            fx32 q = FX_Div(INT_TO_FX(speed), 0x5dc000);
            q      = FX_Mul(q, 0x280000);
            rot[0] = rot[0] + (fx32) ((float) q / 4096.0f);
            rot[1] = (u32) (angle * 65535.0f / 360.0f);
        }
        if (a->y != -mGravityPerFrame) {
            mRestFrames = 0;
        } else {
            mRestFrames++;
        }
    } else {
        mRestFrames++;
        if (mRestFrames >= 0x28) {
            vel->x = 0;
            vel->y = 0;
            vel->z = 0;
            a->x   = 0;
            a->y   = 0;
            a->z   = 0;
            b->x   = 0;
            b->y   = 0;
            b->z   = 0;
            if (mResult == 0) {
                mResult = 1;
            }
            mResultFrames = 1;
            mRestFrames  = 0;
        }
    }
    BattlePlayer *player = data_020d433c.mBattlePlayers->mPlayers[mPlayer];
    BOOL charged         = FALSE;
    if (player->mMicCharge != 0) {
        if ((u8) player->mMicBounceCount < 3) {
            charged = TRUE;
        }
    }
    if (player->mOtherMicCharge != 0) {
        if ((u8) player->mOtherMicBounceCount < 3) {
            charged = TRUE;
        }
    }
    if (mUnk_1dd != 4 && data_020d433c.mTutorialId == 0 && charged) {
        s32 sum = data_020d433c.mBattlePlayers->mPlayers[mPlayer]->mMicCharge + data_020d433c.mBattlePlayers->mPlayers[mPlayer]->mOtherMicCharge;
        if (sum != 0) {
            tmp.x = vel->x;
            tmp.z = vel->z;
            fx32 m = VEC_Mag(&tmp);
            if (m > 0) {
                if (sum > 0) {
                    if (sum > m) {
                        sum = m;
                    }
                } else if (sum < 0) {
                    if (-sum > m) {
                        sum = -m;
                    }
                }
                VEC_Normalize(&tmp, &tmp);
                tmp.x = FX_Mul(tmp.x, sum);
                tmp.y = FX_Mul(tmp.y, sum);
                tmp.z = FX_Mul(tmp.z, sum);
            }
        }
    }
    pos->y += vel->y + tmp.y;
    pos->x += vel->x + tmp.x;
    pos->z += vel->z + tmp.z;
}

BOOL BattleTarget::GetObstacleNormal(BattleSphere *sphere, VecFx32 *dir, BattleObstacle *obj, VecFx32 *out, u8 flag, u8 *outFlag) {
    BattleHitBox box;
    VecFx32 center = sphere->mPos;
    BattleSphere ball;
    u8 tries = 0;
    switch (obj->mType) {
        case OBSTACLE_TYPE_BOX:
            do {
                box.mPos.x = obj->mPos.x;
                box.mPos.y = obj->mPos.y;
                box.mPos.z = obj->mPos.z;
                MI_CpuClear32(box.pad_0x0c, 0x24);
                box.mHalfX = obj->mHalfX;
                box.mHalfY = obj->mHalfY;
                box.mHalfZ = obj->mHalfZ;
                box.mRotX  = obj->mRotX;
                box.mRotY  = obj->mRotY;
                box.mRotZ  = obj->mRotZ;
                SetSphere(&ball, &center, sphere->mRadius);
                if (GetHitBoxNormal(&box, &ball, dir, out, flag, outFlag)) {
                    return 1;
                }
                if (outFlag == 0) {
                    return 0;
                }
                center.x = sphere->mPos.x;
                center.y = sphere->mPos.y;
                center.z = sphere->mPos.z;
                center.x -= (tries + 1) * ((center.x - mPrevPosition.x) * 2 / 10);
                center.z -= (tries + 1) * ((center.z - mPrevPosition.z) * 2 / 10);
                tries++;
                sphere->mPos.x = center.x;
                sphere->mPos.y = center.y;
                sphere->mPos.z = center.z;
            } while (tries < 10);
            return 0;
        case OBSTACLE_TYPE_SPHERE: {
            BattleSphere a;
            BattleSphere b;
            SetSphere(&a, &obj->mPos, obj->mHalfX);
            SetSphere(&b, &sphere->mPos, sphere->mRadius);
            NormalizeDiff(&b.mPos, &a.mPos, (int) dir, out);
            return 1;
        }
        case OBSTACLE_TYPE_CYLINDER: {
            BattleSphere a;
            BattleSphere b;
            SetSphere(&a, &obj->mPos, obj->mHalfX);
            SetSphere(&b, &center, sphere->mRadius);
            fx32 top = obj->mPos.y + obj->mHalfY;
            if (top >= center.y && obj->mPos.y - obj->mHalfY <= center.y) {
                NormalizeDiff(&b.mPos, &a.mPos, (int) dir, out);
                out->y = 0;
            } else if (top <= center.y) {
                out->y = -0x1000;
            }
            return 1;
        }
    }
    return tries;
}


void BattleTarget::HandleGimmickHit(GimmickSetData *set, u32 index, VecFx32 *vel, VecFx32 *out) {
    fx32 mag = VEC_Mag(vel);
    BattleObstacle self = *(BattleObstacle *) ((u8 *) mCollision + 0x10);
    BattleObstacle *selfp = &self;
    BattleObstacle other = *(BattleObstacle *) ((u8 *) set + 0x10 + index * 0x20);
    BattleRecord *record = &data_020d433c.mBattlePlayers->mPlayers[mPlayer]->mRecords[mRecord];
    mHitGimmickType = set->mTag; // a gimmick's set carries its GIMMICK_TYPE in mTag
    switch (set->mTag) {
        case 0: // the Gimmick constructor's default; every subclass constructor overwrites it
        case GIMMICK_TYPE_EXE_POINT:
        case GIMMICK_TYPE_FIELD_POWER_G:
        case GIMMICK_TYPE_FIELD_POWER_S:
        case GIMMICK_TYPE_DASH_BASE:
        case GIMMICK_TYPE_FIELD_DESTROY:
            break;
        case GIMMICK_TYPE_BLOCK:
            VEC_Add(&selfp->mPos, &mCollision->mPosition, &selfp->mPos);
            AddRotAtOnce(&self, mCollision);
            VEC_Add(&other.mPos, &set->mPosition, &other.mPos);
            AddRotAtOnceFields(&other, set);
            GetObstacleNormal((BattleSphere *) &self, vel, &other, out, 0, 0);
            if (out->x != 0 || out->z != 0) {
                SoundManager::Get().StartSeqArc(4);
            }
            if (out->y > 0) {
                if (mAirFrames > 10) {
                    SoundManager::Get().StartSeqArc(4);
                }
                mPrevPosition.y = other.mPos.y + other.mHalfY + self.mHalfY;
            }
            break;
        case GIMMICK_TYPE_JUMP_OBJ:
            if (mShotSpKind == 3) {
                mShotSpKind = 0;
            }
            break;
        case GIMMICK_TYPE_HAOS_BALL:
            if (record->mAttribute == 3) {
                return;
            }
            if (mUnk_1dd != 4) {
                return;
            }
            if (mUnk_1db >= 2) {
                return;
            }
            mUnk_140 = 0x19;
            mUnk_1db = 2;
            mGravityPerFrame  = 0x1dc - 0x19;
            break;
        case GIMMICK_TYPE_TORNADO:
            if (record->mAttribute != 5) {
                mTornadoPosition = &set->mPosition;
                mUnk_1dd = 1;
            }
            if (mShotSpKind == 3) {
                mShotSpKind = 0;
            }
            break;
        case GIMMICK_TYPE_VENT_KAZE:
            if (mPrevHitGimmickType == GIMMICK_TYPE_VENT_KAZE) {
                return;
            }
            if (mHitGimmickType != GIMMICK_TYPE_VENT_KAZE) {
                return;
            }
            SoundManager::Get().StartSeqArc(0xf);
            break;
        case GIMMICK_TYPE_SUTE_WALL:
            VEC_Add(&selfp->mPos, &mCollision->mPosition, &selfp->mPos);
            AddRotAtOnce(&self, mCollision);
            VEC_Add(&other.mPos, &set->mPosition, &other.mPos);
            AddRotAtOnceFields(&other, set);
            if (mag < 0x6800) {
                GetObstacleNormal((BattleSphere *) &self, vel, &other, out, 0, 0);
            }
            break;
        case GIMMICK_TYPE_HAOS_WALL:
            mInField = 1;
            break;
    }
}

BOOL BattleTarget::func_ov007_022583b8() {
    switch (mUnk_1dd) {
        case 0:
            return 1;
        case 1:
            return mUnk_1db == 0;
        case 2:
            return 1;
        case 3:
            return 0;
        case 4:
            return 1;
        case 5:
            return 0;
        case 6:
            return 0;
        case 7:
            return 0;
        case 8:
            return 0;
        default:
            return 1;
    }
}

void BattleTarget::func_ov007_02258448() {
    if (mIkazuchiTimer != 0) {
        mIkazuchiTimer--;
    }
    switch (mUnk_1dd) {
        case 0:
            if (mTornadoCameraPullback != 0) {
                mTornadoCameraPullback--;
            }
            mCameraMode = 0;
            mPushMode = 0;
            mGravityPerFrame  = 0x1c3;
            mCameraEaseFactor = 0;
            mUnk_1db = 0;
            break;
        case 1:
            UpdateTornado(mTornadoPosition);
            break;
        case 2:
            UpdateSpKind3();
            break;
        case 3:
            UpdateUnitHoming();
            mCameraEaseFactor = 0x19a;
            break;
        case 4:
            UpdateJumpObj();
            break;
        case 5: {
            VecFx32 pos = mBakugan->mPosition;
            mInField    = 0;
            mGravityPerFrame     = 0x29;
            mCameraMode    = 6;
            mVelocity.y -= mGravityPerFrame;
            pos.y += mVelocity.y;
            BakuganModel *pose = mBakugan;
            pose->mPosition.x        = pos.x;
            pose->mPosition.y        = pos.y;
            pose->mPosition.z        = pos.z;
            break;
        }
        case 6:
            mCameraMode = 0;
            mResult = 4;
            data_020d433c.mGimmicks->mState = 0;
            UpdateUnitHit();
            break;
        case 7:
            UpdateHaosWall();
            break;
        case 8:
            UpdateIkazuchi();
            break;
    }
}

void BattleTarget::UpdateSpKind3() {
    switch (mUnk_1db) {
        case 0:
            mCameraMode = 4;
            mPushMode = 1;
            mCameraEaseFactor = 0x19a;
            break;
        case 1:
            mCameraMode = 2;
            mCameraEaseFactor = 0;
            mPushMode = 0;
            mUnk_1dd = 0;
            mUnk_1db = 0;
            break;
        case 2:
            mCameraEaseFactor = 0;
            mCameraMode = 0;
            mCameraTargetWeight = 0;
            mPushMode = 0;
            mUnk_1dd = 0;
            mUnk_1db = 0;
            break;
    }
}

void BattleTarget::UpdateUnitHit() {
    switch (mUnk_1db) {
        case 0: {
            mUnk_1db++;
            StartUnitHit(&mUnitHitBakuganPosition, &mHitUnitPosition, &mUnitHitVelocity, mHitUnitHandle);
            if (mSpKind == 1) {
                mUnk_140 = sTables.mSpKind1HitFrameCounts[mUnitHitCount];
            } else {
                mUnk_140 = sTables.mHitFrameCounts[mUnitHitCount];
            }
            mVelocity.x = 0;
            mVelocity.y = 0;
            mVelocity.z = 0;
            mBaseVelocity.x = 0;
            mBaseVelocity.y = 0;
            mBaseVelocity.z = 0;
            mPushVelocity.x = 0;
            mPushVelocity.y = 0;
            mPushVelocity.z = 0;
            mUnk_b8.x = 0;
            mUnk_b8.y = 0;
            mUnk_b8.z = 0;
            SoundManager::Get().StartSeqArc(0x3c);
            mUnitHitEffectHandle = StartEffect(data_020d433c.mEffects, 1, 1, 0, 0);
            if (mUnitHitEffectHandle != 0) {
                MtxFx43 mtx = CameraManager::Get().mMtxC[0];
                mtx._[11]   = 0;
                mtx._[10]   = 0;
                mtx._[9]    = 0;
                MTX_SetTranslation43(&mtx, &mBakugan->mPosition);
                SetEffectMatrix(data_020d433c.mEffects, mUnitHitEffectHandle, &mtx);
            }
            mUnitHitCount++;
            if (mUnitHitCount <= 2) {
                return;
            }
            BattlePlayerList *list = data_020d433c.mBattlePlayers;
            u8 a                   = 0;
            u8 b                   = 0;
            mUnk_1db               = 2;
            mUnk_140               = 0;
            mRestOnCellDisabled               = 1;
            mResultFrames               = 1;
            FindMarkerCellAt(&mHitUnitPosition, &a, &b);
            mUnitHandle = AddUnit(list, &mBakugan->mPosition, mBakugan->mRotY, mPlayer, mRecord,
                                           list->mMarkerHandles[a][b] - 1, 1);
            SetUnitAB(list, mHitUnitHandle - 1, *(u8 *) &list->mUnitHandles[a][b] - 1);
            new BattleCardEffect(0x1000, 0, 1, 0);
            break;
        }
        case 1:
            mUnk_140--;
            if (mUnk_140 == 0) {
                mUnk_1db = 0;
                mBakugan->mModel->StartAnimWithIncrement(0, 0, 0x1000);
            }
            break;
        case 2:
            break;
    }
}

void BattleTarget::UpdateHaosWall() {
    switch (mUnk_1db) {
        case 0:
            mBakugan->mVisible = 0;
            mUnk_140          = 0x1e;
            mUnk_1db++;
            break;
        case 1:
            mUnk_140--;
            if (mUnk_140 == 0) {
                mUnk_1db++;
            }
            break;
        case 2:
            mBakugan->mVisible = 1;
            mUnk_1db          = 0;
            mUnk_1dd          = 0;
            if (mShotSpKind == 3) {
                mUnk_1dd = 2;
            }
            BattleCpu *fighter = data_020d433c.mBattlePlayers->mPlayers[mPlayer]->mCpu;
            if (fighter) {
                fighter->mTimer = 0x3c;
            }
            break;
    }
}

void BattleTarget::UpdateIkazuchi() {
    switch (mUnk_1db) {
        case 0: {
            mCameraMode = 6;
            if (mShotSpKind == 3) {
                mShotSpKind = 0;
            }
            mUnk_1db++;
            u16 handle = StartEffect(data_020d433c.mEffects, 1, 5, 0, 0);
            if (handle == 0) {
                return;
            }
            MtxFx43 mtx = CameraManager::Get().mMtxC[0];
            VecFx32 a   = data_020d433c.mBattlePlayers->mBattleTarget->mBakugan->mPosition;
            VecFx32 b   = sTables.mIkazuchiEffectOffset;
            mtx._[11]   = 0;
            mtx._[10]   = 0;
            mtx._[9]    = 0;
            MTX_MultVec43(&b, &mtx, &b);
            VEC_Add(&a, &b, &a);
            MTX_SetTranslation43(&mtx, &a);
            SetEffectMatrix(data_020d433c.mEffects, handle, &mtx);
            break;
        }
        case 1: {
            mUnk_140--;
            if (mUnk_140 == 0) {
                mUnk_1db++;
            }
            BakuganModel *pose = mBakugan;
            VecFx32 pos              = pose->mPosition;
            if ((mUnk_140 & 1) == 0) {
                pos.y += 0x800;
            } else {
                pos.y -= 0x800;
            }
            pose->SetPosition(&pos);
            break;
        }
        case 2: {
            mUnk_1db = 0;
            mUnk_1dd = 0;
            mIkazuchiTimer = 0x3c;
            SoundManager::Get().StartSeqArc(0x60);
            BattlePlayer *player = data_020d433c.mBattlePlayers->mPlayers[mPlayer];
            u8 record            = player->mRecord;
            int v = player->mRecords[record].mUnk_04;
            if (v > 0x32) {
                player->mRecords[record].mUnk_04 -= 0x32;
                return;
            }
            int amount                       = (s16) -((u8) (0x32 - player->mRecords[record].mUnk_04) + 10);
            player->mRecords[record].mUnk_04 = 10;
            func_ov007_0226a380(player, record, amount);
            break;
        }
    }
}

BOOL BattleTarget::StartUnitHit(VecFx32 *a, VecFx32 *b, VecFx32 *c, u8 d) {
    BattleMoveState state;
    u8 y = 0;
    u8 x = 0;
    state.mVelocity.x = 0;
    state.mVelocity.y = 0;
    state.mVelocity.z = 0;
    state.mRot[0]     = 0;
    state.mRot[1]     = 0;
    state.mRot[2]     = 0;
    state.mMode       = MOVE_MODE_DAMPED;
    BattlePlayerList *list   = data_020d433c.mBattlePlayers;
    BakuganModel *pose = mBakugan;
    u16 rotY                 = pose->mRotY;
    u16 rotZ                 = pose->mRotZ;
    FindMarkerCellAt(b, &y, &x);
    BakuganModel *other = list->mUnitModels->GetBakugan(d - 1);
    if (other) {
        VecFx32 tmp = {0, 0, 0};
        GetVelocityTowardXZ(a, b, c, &tmp);
        pose              = mBakugan;
        pose->mPosition.x = b->x;
        pose->mPosition.y = b->y;
        pose->mPosition.z = b->z;
        mPrevPosition.x         = b->x;
        mPrevPosition.y         = b->y;
        mPrevPosition.z         = b->z;
        pose              = mBakugan;
        pose->mRotX       = 0;
        pose->mRotY       = rotY;
        pose->mRotZ       = rotZ;
        mBakugan->mAnimUpdateEnabled = 1;
        mResult          = 4;
        other->mPosition.x = b->x;
        other->mPosition.y = b->y;
        other->mPosition.z = b->z;
        pose              = mBakugan;
        u16 r0            = pose->mRotX;
        u16 r1            = pose->mRotY;
        u16 r2            = pose->mRotZ;
        state.mVelocity = tmp;
        other->mRotX      = r0 + 0x4000;
        other->mRotY      = r1;
        other->mRotZ      = r2;
        list->mUnitModels->StartMove(d - 1, &state);
        UpdateCameraTarget(&mBakugan->mPosition);
        UpdateCameraPosition(&mBakugan->mPosition);
        if (mShotEffectHandle != 0) {
            FreeEffect(data_020d433c.mEffects, mShotEffectHandle);
            mShotEffectHandle = 0;
        }
        BattleStatusPanels *panels = mStatusPanels;
        if (panels) {
            delete panels;
        }
        mStatusPanels  = 0;
        mRestOnCellDisabled = 1;
        return 1;
    }
    return 0;
}

void BattleTarget::UpdateUnitHoming() {
    VecFx32 v3c;
    VecFx32 v30;
    VecFx32 pos = mBakugan->mPosition;
    BattleMoveState state;
    u16 rot[3];
    u16 rot2[3];
    u8 i                     = 0;
    fx32 best                = -1;
    *(Rot3 *) rot = *(Rot3 *) &mBakugan->mRotX;
    state.mVelocity.x        = 0;
    state.mVelocity.y        = 0;
    state.mVelocity.z        = 0;
    state.mRot[0]            = 0;
    state.mRot[1]            = 0;
    state.mRot[2]            = 0;
    state.mMode              = MOVE_MODE_LAUNCH_SPIN_Y;
    mCameraMode                 = 3;
    switch (mUnk_1db) {
        case 0: {
            mUnk_140 = i;
            do {
                GimmickSetData *set = mUnitCollisions[i];
                if (set && set->mTag) {
                    BattleUnit *unit = &data_020d433c.mBattlePlayers->mUnits[(u8) (set->mTag - 1)];
                    if (unit->mPlayer != mPlayer && unit->mPlayer != mPartner && unit->mStatus != 2 &&
                        unit->mStatus != 3) {
                        VEC_Subtract(&mBakugan->mPosition, &set->mPosition, &v30);
                        fx32 dist = VEC_Mag(&v30);
                        if (best == -1 || best > dist) {
                            best  = dist;
                            set   = mUnitCollisions[i];
                            v3c.x = set->mPosition.x;
                            v3c.y = set->mPosition.y;
                            v3c.z = set->mPosition.z;
                        }
                        set = mUnitCollisions[i];
                        if (set->mHitCount != 0) {
                            RemoveCollisionSet(data_020d433c.mGimmickCollision, set);
                            RemoveUnit(data_020d433c.mBattlePlayers, mUnitCollisions[i]->mTag - 1, 0);
                            mUnitCollisions[i] = 0;
                            mUnk_1db   = 1;
                            mUnk_140   = i;
                            break;
                        }
                    }
                }
                i++;
            } while (i < 12);
            if (best == -1) {
                mUnk_1db = 2;
                break;
            }
            VEC_Subtract(&v3c, &mBakugan->mPosition, &v30);
            v30.x = FX_Mul(v30.x, 0x19a);
            v30.y = FX_Mul(v30.y, 0x19a);
            v30.z = FX_Mul(v30.z, 0x19a);
            mBaseVelocity.x = v30.x;
            mBaseVelocity.y = v30.y;
            mBaseVelocity.z = v30.z;
            mVelocity.x = mBaseVelocity.x;
            mVelocity.y = mBaseVelocity.y;
            mVelocity.z = mBaseVelocity.z;
            VEC_Add(&pos, &v30, &pos);
            break;
        }
        case 1: {
            VEC_Subtract(&mBakugan->mPosition, &v3c, &v30);
            VEC_Normalize(&v30, &v30);
            state.mVelocity.x = v30.x;
            state.mVelocity.y = v30.y;
            state.mVelocity.z = v30.z;
            data_020d433c.mBattlePlayers->mUnitModels->StartMove(mUnk_140, &state);
            mUnk_1db = i;
            u8 handle = StartEffect(data_020d433c.mEffects, 1, 10, 0, i);
            u16 yaw   = FX_Atan2Idx(mVelocity.x, mVelocity.z);
            SoundManager::Get().StartSeqArc(0x10);
            rot2[0] = 0;
            rot2[1] = yaw + 0x4000;
            rot2[2] = 0;
            SetEffectRotation(data_020d433c.mEffects, handle, rot2);
            SetEffectPosition(data_020d433c.mEffects, handle, &mBakugan->mPosition);
            break;
        }
        case 2: {
            mVelocity.x = i;
            mVelocity.y = i;
            mVelocity.z = i;
            mPushVector.x = i;
            mPushVector.y = i;
            mPushVector.z = i;
            mUnk_b8.x = i;
            mUnk_b8.y = i;
            mUnk_b8.z = i;
            mPushVelocity.x = i;
            mPushVelocity.y = i;
            mPushVelocity.z = i;
            VEC_Normalize(&mBaseVelocity, &mBaseVelocity);
            mBaseVelocity.x = FX_Mul(mBaseVelocity.x, 0x3000);
            mBaseVelocity.y = FX_Mul(mBaseVelocity.y, 0x3000);
            mBaseVelocity.z = FX_Mul(mBaseVelocity.z, 0x3000);
            mVelocity.x = mBaseVelocity.x;
            mVelocity.y = mBaseVelocity.y;
            mVelocity.z = mBaseVelocity.z;
            if (mShotEffectHandle != 0) {
                FreeEffect(data_020d433c.mEffects, mShotEffectHandle);
            }
            mUnk_1dd = 0;
            mUnk_1db = 0;
            mUnk_140 = 0;
            mCameraMode = 0;
            break;
        }
    }
    mBakugan->SetPosition(&pos);
    mBakugan->SetRotation(rot);
}

void BattleTarget::UpdateJumpObj() {
    switch (mUnk_1db) {
        case 0:
            mPushMode = 1;
            mCameraMode = 5;
            mJumpObjCameraStep = 0;
            mUnk_17c = 10;
            mGravityPerFrame  = 0xa4;
            mUnk_1db++;
            break;
        case 1:
            mUnk_140--;
            if (mUnk_140 != 0) {
                if (mPushMode == 0) {
                    return;
                }
                if (mAirFrames != 0) {
                    return;
                }
            }
            mUnk_1db++;
            mUnk_140 = 0x19;
            break;
        case 2:
            mPushMode = 0;
            mCameraMode = 0;
            mCameraEaseFactor = 0xb9a;
            mUnk_140--;
            if (mUnk_140 == 0) {
                mUnk_1db++;
            }
            break;
        case 3:
            mUnk_1dd = 0;
            mUnk_1db = 0;
            break;
    }
}

void BattleTarget::UpdateTornado(VecFx32 *target) {
    VecFx32 pos     = mBakugan->mPosition;
    VecFx32 tgt     = *target;
    VecFx32 d;
    VecFx32 v20;
    VecFx32 v14;
    VecFx32 oldPos;
    u16 rot[3];
    *(Rot3 *) rot = *(Rot3 *) &mBakugan->mRotX;
    fx32 mag = 0;
    mCameraMode = mag;
    switch (mUnk_1db) {
        case 0:
            target->y = 0x899a;
            VEC_Subtract(target, &mBakugan->mPosition, &d);
            VEC_Normalize(&d, &d);
            d.y = 0;
            d.x = FX_Mul(d.x, 0x666);
            d.y = FX_Mul(d.y, 0x666);
            d.z = FX_Mul(d.z, 0x666);
            VEC_Add(&mBaseVelocity, &d, &mBaseVelocity);
            if (!(mVelocity.x == 0 && mVelocity.y == 0 && mVelocity.z == 0)) {
                SetVec(&v20, mVelocity.x, 0, mVelocity.z);
                mag   = VEC_Mag(&v20);
            }
            if (mag <= 0x4800) {
                mUnk_1db++;
                SoundManager::Get().StartSeqArc(0xf);
            }
            if (mHitGimmickType != GIMMICK_TYPE_TORNADO) {
                mHitGimmickType = 0;
                mUnk_1dd = 0;
                mUnk_140 = 0;
                mUnk_1db = 0;
            }
            break;
        case 1: {
            fx32 y = 0x899a;
            tgt.y  = y;
            VEC_Subtract(&pos, &tgt, &d);
            fx32 m = VEC_Mag(&d);
            fx32 k = 0x9800 - y;
            d.x    = FX_Mul(d.x, k);
            d.y    = FX_Mul(d.y, k);
            d.z    = FX_Mul(d.z, k);
            pos.x  = tgt.x + d.x;
            pos.y  = tgt.y + d.y;
            pos.z  = tgt.z + d.z;
            mUnk_140++;
            if (m <= 0xb000) {
                mUnk_140 = 0;
                mUnk_1db++;
            }
            break;
        }
        case 2:
            tgt.y = 0x899a;
            VEC_Subtract(&pos, &tgt, &d);
            oldPos = pos;
            d.x = FX_Mul(d.x, 0x105a);
            d.y = FX_Mul(d.y, 0x105a);
            d.z = FX_Mul(d.z, 0x105a);
            RotateVecY(&d, 0x18e3, &d);
            pos.x  = tgt.x + d.x;
            pos.y  = tgt.y + d.y + 0x4cd;
            pos.z  = tgt.z + d.z;
            rot[1] = rot[1] + 0xaaa;
            mag    = VEC_Mag(&d);
            if (mTornadoCameraPullback <= 0x14) {
                mTornadoCameraPullback++;
            }
            if (mag >= 0x32000) {
                mUnk_140 = 0;
                mUnk_1db = 0;
                mHitGimmickType = 0;
                VEC_Subtract(&pos, &oldPos, &v14);
                mVelocity.x = 0;
                mVelocity.y = 1;
                mVelocity.z = 0;
                mPushVector.x = 0;
                mPushVector.y = 0;
                mPushVector.z = 0;
                mUnk_b8.x = 0;
                mUnk_b8.y = 0;
                mUnk_b8.z = 0;
                mPushVelocity.x = 0;
                mPushVelocity.y = 0;
                mPushVelocity.z = 0;
                VEC_Normalize(&v14, &v14);
                v14.x     = FX_Mul(v14.x, 0x3b33);
                v14.y     = FX_Mul(v14.y, 0x3b33);
                v14.z     = FX_Mul(v14.z, 0x3b33);
                mVerticalContactY  = pos.y;
                mBaseVelocity.x = v14.x;
                mBaseVelocity.y = v14.y;
                mBaseVelocity.z = v14.z;
                mHitGimmickType  = 0;
                mUnk_1dd  = 0;
                StopSeqArc(&SoundManager::Get(), 0xf, 0x14);
            }
            break;
    }
    mBakugan->SetPosition(&pos);
    mBakugan->SetRotation(rot);
}

void BattleTarget::UpdateOnMarkerCell() {
    u8 a = 0;
    u8 b = 0;
    if (FindMarkerCellAt(&mBakugan->mPosition, &a, &b)) {
        mOnMarkerCell = 1;
    } else {
        mOnMarkerCell = 0;
    }
}

void BattleTarget::MoveClearOfUnit(u8 a, u8 b, VecFx32 *out, VecFx32 *ref) {
    VecFx32 pos;
    VecFx32 diff;
    VEC_Subtract(out, ref, &diff);
    if (VEC_Mag(&diff) >= 0xc99a) {
        return;
    }
    GetCellPosition(&pos, a, b);
    out->z = pos.z < ref->z ? ref->z - 0xc99a : ref->z + 0xc99a;
    if (pos.x < ref->x) {
        out->x = ref->x - 0x64cd;
    } else {
        out->x = ref->x + 0x64cd;
    }
}

BOOL BattleTarget::func_ov007_02259af0() {
    u8 a = 0;
    u8 b = 0;
    BattleMoveState state;
    VecFx32 d;
    u8 i;
    u8 j;
    state.mVelocity.x = 0;
    state.mVelocity.y = 0;
    state.mVelocity.z = 0;
    state.mRot[0]     = 0;
    state.mRot[1]     = 0;
    state.mRot[2]     = 0;
    state.mMode       = MOVE_MODE_SLOW_LAUNCH_SPIN_X;
    BattlePlayerList *list = data_020d433c.mBattlePlayers;
    if (mShotSpKind != 3) {
        return 0;
    }
    if (mCollisionNormal.x != 0 || mCollisionNormal.z != 0) {
        mUnk_1db = 2;
        mShotSpKind = 0;
        return 0;
    }
    if (mAirFrames == 0 && mMoveFrames != 0) {
        mShotSpKind = 0;
        mCameraTargetWeight = 0;
        if (mVerticalContactY != 0x899a || !FindMarkerCellAt(&mBakugan->mPosition, &a, &b)) {
            if (mUnk_1dd == 2) {
                mUnk_1db = 2;
            }
            return 0;
        }
        SoundManager::Get().StartSeqArc(0x10);
        mUnk_1db = 1;
        u8 owner = list->mUnitHandles[a][b];
        u8 cell  = list->mMarkerHandles[a][b];
        mCameraMode = 2;
        mUnk_1dd = 2;
        SetVec(&mBakugan->mPosition, mBakugan->mPosition.x, 0x8999, mBakugan->mPosition.z);
        BakuganModel *pose = mBakugan;
        {
            u16 r1 = pose->mRotY;
            u16 r2 = pose->mRotZ;
            pose->mRotX = 0;
            pose->mRotY = r1;
            pose->mRotZ = r2;
        }
        mBakugan->mAnimUpdateEnabled = 1;
        if (owner != 0) {
            int idx          = owner - 1;
            BattleUnit *unit = &list->mUnits[(u8) idx];
            if (data_020d433c.mTutorialId != 0 || (u8) (unit->mStatus + 0xfe) <= 1) {
                if (unit->mPlayer == mPlayer) {
                    mResult = 3;
                    StartResult(this, 2);
                } else if (unit->mPlayer == mPartner) {
                    mResult = 1;
                    CreateCursorModelAtCell(a, b);
                    StartResult(this, 0x10);
                } else {
                    mResult = 3;
                    StartResult(this, 0xa);
                }
            } else if (unit->mPlayer == mPlayer || unit->mPlayer == mPartner) {
                mResult = 2;
                RemoveUnit(data_020d433c.mBattlePlayers, idx, 2);
                StartResult(this, 0xa);
                VEC_Subtract(&list->mUnitModels->GetBakugan(idx)->mPosition, &mBakugan->mPosition, &d);
                if (d.x == 0 && d.z == 0) {
                    state.mVelocity.z = 0x1000;
                    state.mVelocity.x = 0x1000;
                } else {
                    state.mVelocity = d;
                }
                list->mUnitModels->StartMove(idx, &state);
            } else {
                mResult = 4;
                StartResult(this, 1);
                VEC_Subtract(&list->mUnitModels->GetBakugan(idx)->mPosition, &mBakugan->mPosition, &d);
                if (d.x == 0 && d.z == 0) {
                    state.mVelocity.z = 0x1000;
                    state.mVelocity.x = 0x1000;
                } else {
                    state.mVelocity = d;
                }
                list->mUnitModels->StartMove(idx, &state);
            }
            if (mResult != 1) {
                pose     = mBakugan;
                mUnitHandle = AddUnit(list, &pose->mPosition, pose->mRotY, mPlayer, mRecord, cell - 1, 1);
                if (mResult != 2) {
                    SetUnitAB(list, idx, (u8) list->mUnitHandles[a][b] - 1);
                }
            }
        } else {
            mResult = 2;
            pose     = mBakugan;
            mUnitHandle = AddUnit(list, &pose->mPosition, pose->mRotY, mPlayer, mRecord, cell - 1, 1);
            StartResult(this, 0xa);
            SetUnitAB(list, mUnitHandle - 1, mUnitHandle - 1);
        }
        i = 0;
        do {
            j = 0;
            do {
                s8 ni = a + i - 1;
                s8 nj = b + j - 1;
                if (ni >= 0 && nj >= 0 && ni < 14 && nj < 10 && (ni == a || nj == b) && !(ni == a && nj == b)) {
                    u8 other = list->mUnitHandles[(u8) ni][(u8) nj];
                    if (other != 0) {
                        u8 kind = list->mUnits[(u8) (other - 1)].mStatus;
                        if (kind != 2 && kind != 3) {
                            RemoveUnit(data_020d433c.mBattlePlayers, other - 1, 2);
                            VEC_Subtract(&list->mUnitModels->GetBakugan(other - 1)->mPosition, &mBakugan->mPosition, &d);
                            state.mVelocity.x = d.x;
                            state.mVelocity.y = d.y;
                            state.mVelocity.z = d.z;
                            list->mUnitModels->StartMove(other - 1, &state);
                        }
                    }
                }
                j++;
            } while (j < 3);
            i++;
        } while (i < 3);
        mCameraShakeY  = 0x3000;
        mResultFrames  = 1;
        mVelocity.x = 0;
        mVelocity.y = 0;
        mVelocity.z = 0;
        mBaseVelocity.x = 0;
        mBaseVelocity.y = 0;
        mBaseVelocity.z = 0;
        mPushVelocity.x = 0;
        mPushVelocity.y = 0;
        mPushVelocity.z = 0;
        mUnk_b8.x = 0;
        mUnk_b8.y = 0;
        mUnk_b8.z = 0;
        return 1;
    }
    return 0;
}

BOOL BattleTarget::TryRestOnCell() {
    u8 a = 0;
    u8 b = 0;
    BattlePlayerList *list = data_020d433c.mBattlePlayers;
    if (mRestOnCellDisabled == 0) {
        BakuganModel *pose = mBakugan;
        if (pose->mPosition.y <= 0x899a) {
            if (CanRestOnCell(&pose->mPosition, &pose->mRotX, &mVelocity, &a, &b)) {
                u8 cell = list->mMarkerHandles[a][b];
                if (cell != 0) {
                    pose        = mBakugan;
                    Rot3 rot    = *(Rot3 *) &pose->mRotX;
                    VecFx32 pos = pose->mPosition;
                    SoundManager::Get().StartSeqArc(3);
                    mVelocity.z = 0;
                    mVelocity.y = 0;
                    mVelocity.x = 0;
                    rot.r[0] = 0;
                    mBakugan->SetRotation(rot.r);
                    pos.y             = 0x899a;
                    pose              = mBakugan;
                    pose->mPosition.x = pos.x;
                    pose->mPosition.y = pos.y;
                    pose->mPosition.z = pos.z;
                    mRestOnCellDisabled          = 1;
                    mResultFrames          = 1;
                    mCameraMode          = 0;
                    if (mShotEffectHandle != 0) {
                        FreeEffect(data_020d433c.mEffects, mShotEffectHandle);
                        mShotEffectHandle = 0;
                    }
                    mBakugan->mAnimUpdateEnabled = 1;
                    u8 owner = list->mUnitHandles[a][b];
                    if (owner != 0) {
                        BattleUnit *unit = &list->mUnits[(u8) (owner - 1)];
                        VecFx32 pos2     = mBakugan->mPosition;
                        VecFx32 ref      = unit->mPos;
                        MoveClearOfUnit(a, b, &pos2, &ref);
                        pose              = mBakugan;
                        pose->mPosition.x = pos2.x;
                        pose->mPosition.y = pos2.y;
                        pose->mPosition.z = pos2.z;
                        mResult          = 3;
                        UpdateCameraPosition(0);
                        if (unit->mPlayer == mPlayer) {
                            StartResult(this, 2);
                        } else if (unit->mPlayer == mPartner) {
                            StartResult(this, 0x10);
                            CreateCursorModelAtCell(a, b);
                            mResult = 1;
                            return 1;
                        } else {
                            StartResult(this, 0xa);
                        }
                        pose     = mBakugan;
                        mUnitHandle = AddUnit(list, &pose->mPosition, pose->mRotY, mPlayer, mRecord, cell - 1, 1);
                        SetUnitAB(list, owner - 1, (u8) list->mUnitHandles[a][b] - 1);
                    } else {
                        VecFx32 p3 = mBakugan->mPosition;
                        p3.y -= 0x64cd;
                        mResult = 2;
                        UpdateCameraPosition(0);
                        switch (mSpKind) {
                            case 5: {
                                pose     = mBakugan;
                                mUnitHandle = AddUnit(list, &pose->mPosition, pose->mRotY, mPlayer, mRecord, cell - 1, 2);
                                s16 h    = StartEffect(data_020d433c.mEffects, 1, 0x13, 0, 1);
                                if (h > 0) {
                                    SetEffectPosition(data_020d433c.mEffects, h, &p3);
                                }
                                SoundManager::Get().StartSeqArc(0x5e);
                                break;
                            }
                            case 6: {
                                pose     = mBakugan;
                                mUnitHandle = AddUnit(list, &pose->mPosition, pose->mRotY, mPlayer, mRecord, cell - 1, 3);
                                s16 h    = StartEffect(data_020d433c.mEffects, 1, 0x14, 0, 1);
                                if (h > 0) {
                                    SetEffectPosition(data_020d433c.mEffects, h, &p3);
                                }
                                SoundManager::Get().StartSeqArc(0x5e);
                                break;
                            }
                            default:
                                pose     = mBakugan;
                                mUnitHandle = AddUnit(list, &pose->mPosition, pose->mRotY, mPlayer, mRecord, cell - 1, 1);
                                break;
                        }
                        StartResult(this, 0xa);
                        SetUnitAB(list, mUnitHandle - 1, mUnitHandle - 1);
                    }
                    return 1;
                }
            }
        }
    }
    return 0;
}

BOOL BattleTarget::IntersectSegmentPlane(VecFx32 *a, VecFx32 *b, BattlePlane *plane, fx32 *t, VecFx32 *hit) {
    VecFx32 d;
    VEC_Subtract(b, a, &d);
    fx32 dot  = VEC_DotProduct(&plane->mNormal, a);
    fx32 den  = VEC_DotProduct(&plane->mNormal, &d);
    if (den == 0) {
        *t = plane->mDist - dot;
    } else {
        *t = FX_Div(plane->mDist - dot, den);
    }
    fx32 tt = *t;
    if ((float) tt >= 0.0f && tt <= 0x1000) {
        d.x = FX_Mul(d.x, tt);
        d.y = FX_Mul(d.y, tt);
        d.z = FX_Mul(d.z, tt);
        VEC_Add(a, &d, hit);
        return 1;
    }
    return 0;
}

BOOL BattleTarget::IntersectSegmentTrianglePlane(VecFx32 *a, VecFx32 *b, VecFx32 *c, VecFx32 *d, VecFx32 *e, fx32 *t, VecFx32 *hit) {
    BattlePlane p;
    VecFx32 eb;
    VecFx32 ea;
    VecFx32 ab;
    VEC_Subtract(d, c, &ea);
    VEC_Subtract(e, c, &eb);
    VEC_Subtract(a, b, &ab);
    VEC_CrossProduct(&ea, &eb, &p.mNormal);
    p.mDist = VEC_DotProduct(&p.mNormal, c);
    if ((float) VEC_DotProduct(&p.mNormal, &ab) <= 0.0f) {
        return 0;
    }
    return IntersectSegmentPlane(a, b, &p, t, hit);
}

BOOL BattleTarget::TestPointInHitBoxXZ(BattleHitBox *box, VecFx32 *point) {
    VecFx32 d;
    MtxFx33 mtx;
    VEC_Subtract(point, &box->mPos, &d);
    mtx = gIdentityMtx33;
    if (box->mRotY) {
        MTX_ApplyRotY33(&mtx, -box->mRotY);
    }
    if (box->mRotX) {
        MTX_ApplyRotX33(&mtx, -box->mRotX);
    }
    if (box->mRotZ) {
        MTX_ApplyRotZ33(&mtx, -box->mRotZ);
    }
    MTX_MultVec33(&d, &mtx, &d);
    VEC_Add(&d, &box->mPos, &d);
    fx32 pz    = box->mPos.z;
    fx32 hz    = box->mHalfZ;
    fx32 lowX  = box->mPos.x - box->mHalfX;
    fx32 lowZ  = pz - hz;
    fx32 highZ = hz + pz;
    fx32 highX = box->mHalfX + box->mPos.x;
    if (lowX >= d.x || highX <= d.x || lowZ >= d.z || highZ <= d.z) {
        return 0;
    }
    return 1;
}

BOOL BattleTarget::GetTriangleNormalY(VecFx32 *a, VecFx32 *b, VecFx32 *tri, fx32 *t, VecFx32 *out) {
    VecFx32 e1;
    VecFx32 e0;
    VecFx32 hit;
    VecFx32 n;
    if (!IntersectSegmentTrianglePlane(a, b, &tri[0], &tri[1], &tri[2], t, &hit)) {
        return 0;
    }
    VEC_Subtract(&tri[1], &tri[0], &e0);
    VEC_Subtract(&tri[2], &tri[1], &e1);
    VEC_CrossProduct(&e0, &e1, &n);
    out->y = n.y;
    return 1;
}

BOOL BattleTarget::GetHitBoxNormal(BattleHitBox *box, BattleSphere *sphere, VecFx32 *dir, VecFx32 *out, u8 flag, u8 *outFlag) {
    VecFx32 c[8];
    VecFx32 e1;
    VecFx32 e0;
    VecFx32 n = {0, 0, 0};
    VecFx32 tri[3];
    VecFx32 qa;
    if (VEC_Mag(dir) <= 0) {
        return 0;
    }
    SetVec(&c[0], box->mHalfX, box->mHalfY, box->mHalfZ);
    SetVec(&c[1], -box->mHalfX, box->mHalfY, box->mHalfZ);
    SetVec(&c[2], -box->mHalfX, box->mHalfY, -box->mHalfZ);
    SetVec(&c[3], box->mHalfX, box->mHalfY, -box->mHalfZ);
    SetVec(&c[4], box->mHalfX, -box->mHalfY, box->mHalfZ);
    SetVec(&c[5], -box->mHalfX, -box->mHalfY, box->mHalfZ);
    SetVec(&c[6], -box->mHalfX, -box->mHalfY, -box->mHalfZ);
    SetVec(&c[7], box->mHalfX, -box->mHalfY, -box->mHalfZ);
    RotateVecY(&c[0], box->mRotY, &c[0]);
    RotateVecY(&c[1], box->mRotY, &c[1]);
    RotateVecY(&c[2], box->mRotY, &c[2]);
    RotateVecY(&c[3], box->mRotY, &c[3]);
    RotateVecY(&c[4], box->mRotY, &c[4]);
    RotateVecY(&c[5], box->mRotY, &c[5]);
    RotateVecY(&c[6], box->mRotY, &c[6]);
    RotateVecY(&c[7], box->mRotY, &c[7]);
    u8 i = 0;
    do {
        VEC_Add(&c[i], &box->mPos, &c[i]);
        i++;
    } while (i < 8);
    if (outFlag) {
        *outFlag = 0;
    }
    if (TestPointInHitBoxXZ(box, &sphere->mPos)) {
        fx32 t2;
        if (outFlag) {
            *outFlag = 1;
        }
        tri[0] = c[1];
        tri[1] = c[0];
        tri[2] = c[3];
        VecFx32 ta;
        ta.z = sphere->mPos.z;
        ta.y = sphere->mPos.y - sphere->mRadius;
        ta.x = sphere->mPos.x;
        qa = ta;
        if (GetTriangleNormalY(&sphere->mPos, &qa, tri, &t2, out)) {
            return 1;
        }
        tri[0] = c[7];
        tri[1] = c[4];
        tri[2] = c[5];
        VecFx32 tb;
        tb.z = sphere->mPos.z;
        tb.y = sphere->mPos.y + sphere->mRadius;
        tb.x = sphere->mPos.x;
        qa = tb;
        if (GetTriangleNormalY(&sphere->mPos, &qa, tri, &t2, out)) {
            return 1;
        }
    }
    fx32 f[4] = {0, 0, 0, 0};
    VecFx32 va0;
    VecFx32 nearest;
    VecFx32 p2;
    VecFx32 p1;
    VecFx32 p0;
    VecFx32 q;
    VecFx32 hit;
    fx32 t;
    u8 idx;
    fx32 best = 0xffff;
    fx32 value;
    i = 0;
    idx = i;
    GetNearestPointOnBox(sphere, box, &nearest);
    VEC_Subtract(&nearest, &sphere->mPos, &va0);
    value = VEC_Mag(&va0);
    if (value == 0) {
        if (outFlag) {
            *outFlag = 1;
        }
        return 0;
    }
    do {
        p0 = c[i + 4];
        p1 = c[i];
        p2 = c[i + 1];
        VecFx32 d1 = *dir;
        VecFx32 pos = sphere->mPos;
        if (flag == 0) {
            VEC_Normalize(&d1, &d1);
            SetVec(&q, pos.x + FX_Mul(d1.x, sphere->mRadius), pos.y, pos.z + FX_Mul(d1.z, sphere->mRadius));
        } else {
            SetVec(&q, pos.x + va0.x, pos.y, pos.z + va0.z);
        }
        if (IntersectSegmentTrianglePlane(&pos, &q, &p0, &p1, &p2, &t, &hit)) {
            VEC_Subtract(&p1, &p0, &e0);
            VEC_Subtract(&p2, &p0, &e1);
            VEC_CrossProduct(&e1, &e0, &n);
            f[i] = t;
        }
        value = f[i];
        if (0.0f != (float) value && best >= value) {
            best = value;
            idx  = (u8) (i + 1);
        }
        i++;
    } while (i < 4);
    if (idx == 0) {
        return 0;
    }
    VecFx32 pc;
    VecFx32 pb;
    VecFx32 pa;
    pa = c[idx + 3];
    pb = c[idx - 1];
    pc = c[idx];
    VEC_Subtract(&pb, &pa, &e0);
    VEC_Subtract(&pc, &pa, &e1);
    VEC_CrossProduct(&e1, &e0, &n);
    out->x += n.x;
    out->z += n.z;
    return 1;
}

void BattleTarget::GetPushVector(s32 *pair, u8 mode, VecFx32 *out) {
    out->x = 0;
    out->y = 0;
    out->z = 0;
    if ((pair[1] | pair[0]) == 0) {
        return;
    }
    switch (mode) {
        case 0: {
        if (pair[1] > 0) {
            pair[1] = FX_Mul(pair[1], 0x8f);
        } else if (pair[1] < 0) {
            pair[1] = FX_Mul(pair[1], 0xcd);
        }
        s32 x = pair[0];
        if (0.0f != (float) x) {
            pair[0] = FX_Mul(x, 0xcd);
        }
        break;
        }
        case 1: {
        if (pair[1] > 0) {
            pair[1] = FX_Mul(pair[1], 0x66);
        } else if (pair[1] < 0) {
            pair[1] = FX_Mul(pair[1], 0x66);
        }
        s32 x = pair[0];
        if (0.0f != (float) x) {
            pair[0] = FX_Mul(x, 0x1ec);
        }
        break;
        }
    }
    s32 px = pair[0];
    s32 pz = pair[1];
    out->x = px;
    out->y = 0;
    out->z = pz;
    out->x = FX_Mul(out->x, 0x1000);
    out->y = FX_Mul(out->y, 0x1000);
    out->z = FX_Mul(out->z, 0x1000);
}

BOOL BattleTarget::CanRestOnCell(VecFx32 *pos, u16 *angle, VecFx32 *vel, u8 *outA, u8 *outB) {
    u8 a = 0;
    u8 b = 0;
    s32 speed;
    BattleRecord *record;
    s32 x = vel->x;
    s32 z = vel->z;
    if (x < 0) {
        x = -x;
    }
    if (z < 0) {
        z = -z;
    }
    speed = z + x;
    record = &data_020d433c.mBattlePlayers->mPlayers[mPlayer]->mRecords[mRecord];
    if (mPrevLanded != 0 && pos->y <= 0x899a) {
        u32 k = record->mRestLimitIndex;
        if (speed < sTables.mRestSpeedLimits[k]) {
            u16 lim = sTables.mRestAngleLimits[k];
            if (!(*angle > lim && *angle < (u16) -lim)) {
                if (FindMarkerCellAt(pos, &a, &b)) {
                    BattlePlayerList *list = data_020d433c.mBattlePlayers;
                    BattlePlayer *p0 = list->mPlayers[mPlayer];
                    BattlePlayer *p1 = list->mPlayers[mPartner];
                    u8 cell = *(u8 *) &list->mMarkerHandles[a][b];
                    if (cell != 0) {
                        *outA = a;
                        *outB = b;
                        u8 m = list->mMarkers[(u8) (cell - 1)].mUnitHandle;
                        if (m != 0) {
                            BattleUnit *unit = &data_020d433c.mBattlePlayers->mUnits[(u8) (m - 1)];
                            if (p0->IsCpu() && !p1->IsCpu() && unit->mPlayer == mPartner) {
                                if (speed < 0x1000) {
                                    if (!(*angle > 0xe38 && *angle < 0x10000 - 0xe38)) {
                                        return 1;
                                    }
                                }
                            } else {
                                return 1;
                            }
                        } else {
                            return 1;
                        }
                    }
                }
            }
        }
    }
    return 0;
}

void BattleTarget::CreateCursorModelAtCell(u8 a, u8 b) {
    SetUseMapModels(mCommandMap, 1);
    CreateCursorModel(mCommandMap);
    BattleCommandMap *map = mCommandMap;
    map->mCursorX         = a;
    map->mCursorY         = b;
}

void BattleTarget::StartShot() {
    char buf[0x804];
    VecFx32 vel;
    VecFx32 dir;
    u16 rot[3];
    u16 rot2[3];
    u16 r3[3];
    if (mState != 1) {
        return;
    }
    vel.x                = 0;
    vel.y                = 0;
    vel.z                = 0;
    int power;
    BattlePlayer *player = data_020d433c.mBattlePlayers->mPlayers[mPlayer];
    BattleRecord *record = &player->mRecords[mRecord];
    dir.x                = 0x1000;
    dir.y                = 0;
    dir.z                = 0;
    *(Rot3 *) rot = *(Rot3 *) player->mShotRot;
    rot[1]               = rot[1] + sTables.mBaseRotY[player->mCorner];
    power             = (u8) (sShotSpeedAdjusts[record->mShotSpeedAdjustIndex] + player->mShotSpeed);
    mPushScaleTimer = mPushScaleFrameCount = sTables.mPushScaleFrameCounts[record->mPushTableIndex];
    if (player->mShotSpeed == 0) {
        power = 10;
    }
    rot[1]               = rot[1] - mRotYAdjust;
    func_ov007_0225c360(&rot[2], mRotZAdjust, &rot[2]);
    rot[2] = rot[2] + mRotZAdjust;
    switch (mSpKind) {
        case 0:
            RotateDirScaled(rot, power, &dir, &vel);
            break;
        case 1:
            rot[2] = mRotZAdjust + 0x38e;
            RotateDirScaled(rot, 0x78, &dir, &vel);
            mShotEffectHandle = StartEffect(data_020d433c.mEffects, 1, 0, 0, 1);
            SoundManager::Get().StartSeqArc(0x3d);
            mSkipHeadingTest = 1;
            break;
        case 2:
            RotateDirScaled(rot, power, &dir, &vel);
            mShotEffectHandle = StartEffect(data_020d433c.mEffects, 1, 2, 0, 1);
            SoundManager::Get().StartSeqArc(0x3e);
            break;
        case 5:
            RotateDirScaled(rot, power, &dir, &vel);
            mShotEffectHandle = StartEffect(data_020d433c.mEffects, 1, 7, 0, 1);
            SoundManager::Get().StartSeqArc(0x41);
            break;
        case 6:
            RotateDirScaled(rot, power, &dir, &vel);
            mShotEffectHandle = StartEffect(data_020d433c.mEffects, 1, 8, 0, 1);
            SoundManager::Get().StartSeqArc(0x3f);
            break;
        case 4: {
            RotateDirScaled(rot, power, &dir, &vel);
            mShotEffectHandle  = StartEffect(data_020d433c.mEffects, 1, 9, 0, 1);
            u8 handle = StartEffect(data_020d433c.mEffects, 1, 10, 0, 0);
            u16 t = FX_Atan2Idx(vel.x, vel.z);
            rot2[0]   = 0;
            rot2[1]   = t + 0x4000;
            rot2[2]   = 0;
            SetEffectRotation(data_020d433c.mEffects, handle, rot2);
            SetEffectPosition(data_020d433c.mEffects, handle, &mBakugan->mPosition);
            SoundManager::Get().StartSeqArc(0x42);
            break;
        }
        case 3: {
            mUnk_1dd = 2;
            u8 k     = data_020d433c.mFieldId;
            u16 a    = sTables.mSpKind3Speeds[k];
            u16 b    = sTables.mSpKind3RotZ[k];
            rot[2]   = b;
            RotateDirScaled(rot, (u16) (a + 0x50), &dir, &vel);
            mShotEffectHandle  = StartEffect(data_020d433c.mEffects, 1, 0xb, 0, 0);
            mUnk_11c.x = 0;
            mUnk_11c.y = 0;
            mUnk_11c.z = 0;
            SoundManager::Get().StartSeqArc(0x40);
            break;
        }
    }
    mShotSpKind  = mSpKind;
    mUnk_1be  = rot[1];
    SetVec(&mVelocity, vel.x, vel.y, vel.z);
    mBaseVelocity.x = mVelocity.x;
    mBaseVelocity.y = mVelocity.y;
    mBaseVelocity.z = mVelocity.z;
    mBaseVelocity.x = FX_Mul(mBaseVelocity.x, 0xb800);
    mBaseVelocity.y = FX_Mul(mBaseVelocity.y, 0xb800);
    mBaseVelocity.z = FX_Mul(mBaseVelocity.z, 0xb800);
    mPushVelocity.x = 0;
    mPushVelocity.y = 0;
    mPushVelocity.z = 0;
    mPushZ  = player->mPushSumZ;
    mPushX  = -player->mPushSumX;
    GetPushVector((s32 *) &mPushX, mPushMode, &mPushVector);
    mPushVelocity.x = mPushVector.x;
    mPushVelocity.y = mPushVector.y;
    mPushVelocity.z = mPushVector.z;
    LoadCollisionFile(buf, 0xa);
    if (mSpKind != 5) {
        mCollision = AddCollisionSet(data_020d433c.mGimmickCollision, COLLISION_GROUP_BAKUGAN, buf, &mBakugan->mPosition, &mBakugan->mRotX, 0);
    } else {
        mCollision = AddCollisionSet(data_020d433c.mGimmickCollision, COLLISION_GROUP_BAKUGAN_SP5, buf, &mBakugan->mPosition, &mBakugan->mRotX, 0);
    }
    SetPredictVisible(mCommandMap, 0);
    mBakugan->mVisible = 1;
    if (mSpKind == 0) {
        if (power > 0x59) {
            mShotEffectHandle = StartEffect(data_020d433c.mEffects, 1, 0x11, 0, 0);
            SoundManager::Get().StartSeqArc(0x39);
        } else if (power > 0x4f) {
            mShotEffectHandle = StartEffect(data_020d433c.mEffects, 1, 0xf, 0, 0);
            SoundManager::Get().StartSeqArc(0x3a);
        } else if (power > 0x3c) {
            mShotEffectHandle = StartEffect(data_020d433c.mEffects, 1, 0x10, 0, 0);
            SoundManager::Get().StartSeqArc(0x3b);
        }
        if (mShotEffectHandle != 0) {
            r3[1] = FX_Atan2Idx(mVelocity.x, mVelocity.z) + 0x4000;
            r3[0] = 0;
            r3[2] = 0;
            SetEffectRotation(data_020d433c.mEffects, mShotEffectHandle, r3);
            SetEffectPosition(data_020d433c.mEffects, mShotEffectHandle, &mBakugan->mPosition);
        }
    }
    player->mMicCharge = 0;
    player->mMicBounceCount = 0;
    player->mOtherMicCharge = 0;
    player->mOtherMicBounceCount = 0;
    mState++;
}

void BattleTarget::AddPlayerPush() {
    BattlePlayer *player = data_020d433c.mBattlePlayers->mPlayers[mPlayer];
    mPushZ             = player->mPushSumZ;
    mPushX             = player->mPushSumX;
    AddPushXZ(&mPushX);
}

void BattleTarget::LimitSumLength(VecFx32 *a, VecFx32 *b) {
    VecFx32 sum  = {0, 0, 0};
    VecFx32 unused = {0, 0, 0};
    VecFx32 rest = {0, 0, 0};
    VecFx32 full = {0, 0, 0};
    VEC_Add(a, b, &sum);
    full.x = sum.x;
    full.y = sum.y;
    full.z = sum.z;
    if (VEC_Mag(&sum) >= 0x3000) {
        VEC_Normalize(&sum, &sum);
        sum.x = FX_Mul(sum.x, 0x3000);
        sum.y = FX_Mul(sum.y, 0x3000);
        sum.z = FX_Mul(sum.z, 0x3000);
    }
    VEC_Subtract(&full, &sum, &rest);
    VEC_Subtract(b, &rest, b);
}

void BattleTarget::AddPushXZ(s32 *pair) {
    VecFx32 target;
    VecFx32 cam;
    mPushZ = pair[1];
    mPushX = -pair[0];
    if (mPlayer != data_020d433c.mBattlePlayers->mLocalPlayer && data_020d433c.mTutorialId != 0) {
        return;
    }
    if (mAirFrames > 3) {
        GetPushVector((s32 *) &mPushX, mPushMode, &mPushVector);
        Camera *camera = mCamera;
        target.x       = camera->mTarget.x;
        target.y       = camera->mTarget.y;
        target.z       = camera->mTarget.z;
        cam.x          = mCameraBasePosition.x;
        cam.y          = mCameraBasePosition.y;
        cam.z          = mCameraBasePosition.z;
        func_ov007_02255b78(&target, &cam, &mPushVector, &mPushVector);
        if (mUnk_1dd == 2) {
            ScaleVec(&mPushVector, 0x14000);
        } else {
            ScaleVec(&mPushVector, 0x5000);
        }
        VEC_Add(&mPushPerFrame, &mPushVector, &mPushPerFrame);
    } else if (mPushScale >= 0x800) {
        if (mPushZ == 0 && mPushX == 0) {
            return;
        }
        GetPushVector((s32 *) &mPushX, mPushMode, &mPushVector);
        Camera *camera = mCamera;
        target.x       = camera->mTarget.x;
        target.y       = camera->mTarget.y;
        target.z       = camera->mTarget.z;
        cam.x          = mCameraBasePosition.x;
        cam.y          = mCameraBasePosition.y;
        cam.z          = mCameraBasePosition.z;
        func_ov007_02255b78(&target, &cam, &mPushVector, &mPushVector);
        fx32 s    = mPushScale;
        mPushVector.x = FX_Mul(mPushVector.x, s);
        mPushVector.y = FX_Mul(mPushVector.y, s);
        mPushVector.z = FX_Mul(mPushVector.z, s);
        VEC_Add(&mPushPerFrame, &mPushVector, &mPushPerFrame);
    } else {
        mPushScale = 0;
    }
}

void BattleTarget::AddScaledPush(VecFx32 *v) {
    VecFx32 d;
    fx32 scale = mPushScale;
    d.x = FX_Mul(v->x, scale);
    d.y = FX_Mul(v->y, scale);
    d.z = FX_Mul(v->z, scale);
    VEC_Add(&mPushPerFrame, &d, &mPushPerFrame);
}

void BattleTarget::func_ov007_0225c360(u16 *in, s32 a, u16 *out) {
    u16 n    = *in - 910;
    int base = 0x1c71;
    fx32 q   = FX_Div(INT_TO_FX(n), INT_TO_FX(base));
    fx32 d   = FX_Div(0x4000000, 0x2aaa000);
    fx32 m   = FX_Mul(a / 182, d);
    int rem  = 100 - m;
    fx32 r   = INT_TO_FX(rem) / 100;
    fx32 t1  = FX_Mul(INT_TO_FX(base), r);
    fx32 res = FX_Mul(t1, q);
    *out     = (u16) (u32) (910.0f + (float) res / 4096.0f);
}

void BattleTarget::UpdateEffects() {
    switch (mSpKind) {
        case 0: {
            if (mShotEffectHandle == 0) {
                break;
            }
            if (EffectAlive(data_020d433c.mEffects, mShotEffectHandle)) {
                u16 rot[3];
                rot[1] = FX_Atan2Idx(mVelocity.x, mVelocity.z) + 0x4000;
                rot[0] = 0;
                rot[2] = 0;
                SetEffectRotation(data_020d433c.mEffects, mShotEffectHandle, rot);
                SetEffectPosition(data_020d433c.mEffects, mShotEffectHandle, &mBakugan->mPosition);
            } else {
                mShotEffectHandle = 0;
            }
            break;
        }
        case 1: {
            if (mShotEffectHandle == 0) {
                break;
            }
            if (VEC_Mag(&mVelocity) <= 0x1000) {
                FreeEffect(data_020d433c.mEffects, mShotEffectHandle);
                mSkipHeadingTest = 0;
                mShotEffectHandle = 0;
            } else {
                u16 rot[3];
                rot[1] = FX_Atan2Idx(mVelocity.x, mVelocity.z) + 0x4000;
                rot[0] = 0;
                rot[2] = 0;
                SetEffectRotation(data_020d433c.mEffects, mShotEffectHandle, rot);
                SetEffectPosition(data_020d433c.mEffects, mShotEffectHandle, &mBakugan->mPosition);
            }
            break;
        }
        case 2: {
            if (mShotEffectHandle == 0) {
                break;
            }
            u16 rot[3];
            rot[1] = FX_Atan2Idx(mVelocity.x, mVelocity.z) + 0x4000;
            rot[0] = 0;
            rot[2] = 0;
            SetEffectRotation(data_020d433c.mEffects, mShotEffectHandle, rot);
            SetEffectPosition(data_020d433c.mEffects, mShotEffectHandle, &mBakugan->mPosition);
            break;
        }
        case 5:
        case 6:
            if (mShotEffectHandle == 0) {
                break;
            }
            VEC_Mag(&mVelocity);
            SetEffectPosition(data_020d433c.mEffects, mShotEffectHandle, &mBakugan->mPosition);
            break;
        case 3:
            if (mShotEffectHandle == 0) {
                break;
            }
            SetEffectPosition(data_020d433c.mEffects, mShotEffectHandle, &mBakugan->mPosition);
            if (mShotSpKind != 3) {
                FreeEffect(data_020d433c.mEffects, mShotEffectHandle);
                mShotEffectHandle = 0;
            }
            break;
        case 4: {
            if (mShotEffectHandle == 0) {
                break;
            }
            fx32 mag = VEC_Mag(&mVelocity);
            SetEffectPosition(data_020d433c.mEffects, mShotEffectHandle, &mBakugan->mPosition);
            if ((float) mag <= 0.0f) {
                FreeEffect(data_020d433c.mEffects, mShotEffectHandle);
                mShotEffectHandle = 0;
            } else {
                MtxFx43 mtx = CameraManager::Get().mMtxC[0];
                VecFx32 pos = mBakugan->mPosition;
                MTX_SetTranslation43(&mtx, &pos);
                SetEffectMatrix(data_020d433c.mEffects, mShotEffectHandle, &mtx);
            }
            break;
        }
    }
    if (mUnitHitEffectHandle == 0) {
        return;
    }
    MtxFx43 mtx = CameraManager::Get().mMtxC[0];
    VecFx32 pos = mBakugan->mPosition;
    VecFx32 off = sTables.mHitEffectOffset;
    mtx._[11]   = 0;
    mtx._[10]   = 0;
    mtx._[9]    = 0;
    MTX_MultVec43(&off, &mtx, &off);
    VEC_Add(&pos, &off, &pos);
    MTX_SetTranslation43(&mtx, &pos);
    SetEffectMatrix(data_020d433c.mEffects, mUnitHitEffectHandle, &mtx);
}

void SetTargetVelocity(BattleTarget *target, VecFx32 *dir, int unk) {
    target->mBaseVelocity.x = dir->x;
    target->mBaseVelocity.y = dir->y;
    target->mBaseVelocity.z = dir->z;
    target->mVelocity.x = target->mBaseVelocity.x;
    target->mVelocity.y = target->mBaseVelocity.y;
    target->mVelocity.z = target->mBaseVelocity.z;
    target->mUnk_b8.x = 0;
    target->mUnk_b8.y = 0;
    target->mUnk_b8.z = 0;
    target->mPushVelocity.x = 0;
    target->mPushVelocity.y = 0;
    target->mPushVelocity.z = 0;
    if (unk != 0) {
        target->mPushPerFrame.x = 0;
        target->mPushPerFrame.y = 0;
        target->mPushPerFrame.z = 0;
    }
}

void SetTargetVelocityXyz(BattleTarget *target, int a, int b, int c, int d) {
    target->mVelocity.x = a;
    target->mVelocity.y = b;
    target->mVelocity.z = c;
    target->mBaseVelocity.x = target->mVelocity.x;
    target->mBaseVelocity.y = target->mVelocity.y;
    target->mBaseVelocity.z = target->mVelocity.z;
    target->mUnk_b8.x = 0;
    target->mUnk_b8.y = 0;
    target->mUnk_b8.z = 0;
    target->mPushVelocity.x = 0;
    target->mPushVelocity.y = 0;
    target->mPushVelocity.z = 0;
    if (d != 0) {
        target->mPushPerFrame.x = 0;
        target->mPushPerFrame.y = 0;
        target->mPushPerFrame.z = 0;
    }
}

void func_ov007_0225ca18(BattleTarget *target, int a, int b) {
    if (!target->func_ov007_022583b8()) {
        return;
    }
    target->mUnk_1dd = a;
    target->mUnk_140 = b;
    target->mUnk_1db = 0;
}

// Both arms compute the record address, so the size 12 is set before the branch and the identical tails are merged.
// Arm 1 reads its index into an enum-typed local (a real load, coloured after `pl`); a plain read is folded into the
// address and the arm takes other registers, so the tails no longer merge.
enum RecordSlot { RECORD_SLOT_0 };

void BattleTarget::DrawRecordTotal(u32 player, u32 record) {
    BattlePlayer *pl;
    BattleRecord *rec;
    if (player == 0xff) {
        player = data_020d433c.mBattlePlayers->mTurnPlayer;
        pl     = data_020d433c.mBattlePlayers->mPlayers[player];
        RecordSlot e  = (RecordSlot) pl->mRecord;
        rec           = &pl->mRecords[e];
    } else {
        pl  = data_020d433c.mBattlePlayers->mPlayers[player];
        rec = &pl->mRecords[record];
    }
    u16 total         = rec->mUnk_04 + rec->mUnk_06;
    u8 i              = 0;
    do {
        if (total == 0) {
            break;
        }
        mCellAnims[1]->DrawFrame(0, total % 10, 0xc0 + (3 - i) * 12, 0xaa, 1);
        i++;
        total = total / 10;
    } while (i < 4);
    if (data_020d433c.mBattlePlayers->mPlayers[player]->mAbilityId == 0) {
        return;
    }
    mCellAnims[0]->Draw(0, 0xf2, 0xab, 1);
}

void BattleTarget::DrawBar() {
    u8 old = mBarLength;
    mBarLength = (mPushScaleTimer * 32 / mPushScaleFrameCount + old * 3) / 4;
    if (old != mBarLength) {
        u8 hi  = mPushScaleTimer * 6 / mPushScaleFrameCount + 2;
        u8 *buf = (u8 *) OS_Alloc(0x80);
        MI_CpuFill8(buf, 0, 0x80);
        u16 i = 0;
        do {
            if (!(i & 0x10)) {
                u8 v   = (i % 32) % 4 + (i / 32) * 8;
                u8 lo  = 1;
                u8 up  = 1;
                if (v < mBarLength) {
                    lo = hi;
                    if (v + 1 < mBarLength) {
                        up = hi;
                    }
                }
                buf[i] = lo + (up << 4);
            }
            i++;
        } while (i < 0x80);
        DC_FlushRange(buf, 0x80);
        QueueUpload(0, UPLOAD_KIND_OBJ, 0xe00, buf, 0x80, 1);
    }
    mCellAnims[2]->Draw(0, 0xbe, 0xb7, 1);
    mCellAnims[3]->Draw(0, 0xce, 0xba, 1);
    SpriteManager::Get().Screen(0).AddOam((void *) sTables.mBarOam, 1, 0xce, 0xba, 1, 0x70, 0xb, 0);
}

void BattleTarget::DrawRoulette() {
    mCellAnims[4]->Update();
    mCellAnims[4]->Draw(1, 0x80, 0x62, 0);
    if (mPlayer == data_020d433c.mBattlePlayers->mLocalPlayer) {
        mCellAnims[5]->Update();
        mCellAnims[5]->Draw(1, 0x80, 0x62, 0);
    }
    mCellAnims[6]->Update();
    mCellAnims[6]->Draw(1, 0x80, 0x62, 0);
}

void BattleTarget::func_ov007_0225ce08() {
    GX_SetDispSelect(1);
    SpriteManager::Get().SetupMode(0, 7);
    GX_SetGraphicsMode(1, 0, 1);
    GXS_SetGraphicsMode(3);
    G2_SetBG0Offset(0, 0);
    G2_SetBG0Priority(2);
    GX_SetVisiblePlane(0x11);
    G2_SetBlendBrightness(0x1f, 0);
    GX_SetVisibleWnd(0);
    G2_SetWndOutsidePlane(0x1f, FALSE);
    G2_BlendNone();
    ChangeDir("/Game/Battle/Obj/");
    SpriteScreen &screen = SpriteManager::Get().Screen(0);
    screen.LoadCharacters("battle_shoot_b_gp.nce_ncg1D.bin", 0, 0);
    screen.LoadPalette("battle_shoot_b_gp_ncl.bin", 0x140, 0);
    ChangeDir("/");
    mCommandMap = new BattleCommandMap(data_020d433c.mFieldId);
    mCommandMap->mUseStatusPanels = 0;
    mCommandMap->Init();
    data_020d433c.mGimmicks->mState = 0;
    mBakugan->SetupMode(BAKUGAN_MODEL_MODE_H_HOLD_FRAME);
    if (mUnitHandle != 0) {
        BakuganModel *pose = data_020d433c.mBattlePlayers->mUnitModels->GetBakugan(mUnitHandle - 1);
        pose->mVisible            = 0;
    }
    UpdateCameraPosition(0);
    UpdateCameraTarget(0);
    mVisible = 1;
}

BOOL BattleTarget::IsFadingToAbility() {
    return mState == 0xc;
}
