#include "Battle/BattleBase.hpp"
#include <nitro/fx.h>
#include "Battle/BattleCardEffect.hpp"
#include "Battle/BattleUnitModels.hpp"
#include "Battle/BattleCpu.hpp"
#include "Battle/BattleCommand.hpp"
#include "Battle/BattleMapTypes.hpp"
#include "Battle/BattleStatusPanels.hpp"
#include "Battle/GimmickTypes.hpp"
#include "Game/GameData.hpp"
#include "Graphics/Camera.hpp"
#include "Graphics/Effect.hpp"
#include "Graphics/Model.hpp"
#include "Graphics/SpriteManager.hpp"
#include <nitro/mi.h>
#include <nitro/os.h>
#include "System/FileSystem.hpp"

// The order of these definitions sets the data layout: mwcc sorts a file's objects by size, and the order of
// equal sizes depends on where each was defined (docs/decompiling.md, "Data layout within a section").

// Only the middle angle is read, folded to a constant; the 6 bytes at 0x02279eb4 show the table was there.
static const u16 sCommandCamAngles[3] = {0, 0x2000, 0};
static const VecFx32 sCornerPositions0[4] = {
    {0x87000, 0x899a, 0x87000},
    {-0x87000, 0x899a, 0x87000},
    {-0x87000, 0x899a, -0x87000},
    {0x87000, 0x899a, -0x87000},
};
static const VecFx32 sShadowPieceScale = {0x1000, 0x1000, 0x1000};
static const VecFx32 sCornerPositions1[4] = {
    {0xaa000, 0x899a, 0xaa000},
    {-0xaa000, 0x899a, 0xaa000},
    {-0xaa000, 0x899a, -0xaa000},
    {0xaa000, 0x899a, -0xaa000},
};
static const VecFx32 sCornerPositions2[4] = {
    {0xc8000, 0x899a, 0xc8000},
    {-0xc8000, 0x899a, 0xc8000},
    {-0xc8000, 0x899a, -0xc8000},
    {0xc8000, 0x899a, -0xc8000},
};
static const VecFx32 sCornerPositions3[4] = {
    {0xbe000, 0x899a, 0xbe000},
    {-0xbe000, 0x899a, 0xbe000},
    {-0xbe000, 0x899a, -0xbe000},
    {0xbe000, 0x899a, -0xbe000},
};
static const VecFx32 sCornerPositions4[4] = {
    {0x14a000, 0x899a, 0x14a000},
    {-0x14a000, 0x899a, 0x14a000},
    {-0x14a000, 0x899a, -0x14a000},
    {0x14a000, 0x899a, -0x14a000},
};
static const VecFx32 sCornerPositions5[4] = {
    {0x78000, 0x899a, 0x78000},
    {-0x78000, 0x899a, 0x78000},
    {-0x78000, 0x899a, -0x78000},
    {0x78000, 0x899a, -0x78000},
};
static const u8 data_ov007_02279eb0[4] = {0, 1, 3, 2};
static const VecFx32 sCornerPositions6[4] = {
    {0xaf000, 0x899a, 0xaf000},
    {-0xaf000, 0x899a, 0xaf000},
    {-0xaf000, 0x899a, -0xaf000},
    {0xaf000, 0x899a, -0xaf000},
};
static const u16 sCommandMapLights[20] = {1, 0x7fff, 0xf666, 0xf5ec, 0xf829};
static const VecFx32 sCornerPositions7[4] = {
    {0x11db0a, 0x899a, 0x11db0a},
    {-0x11db0a, 0x899a, 0x11db0a},
    {-0x11db0a, 0x899a, -0x11db0a},
    {0x11db0a, 0x899a, -0x11db0a},
};
static const u32 sCameraHeights[8] = {0x4b000, 0x3a000, 0x46000, 0x46000, 0x46000, 0x46000, 0x46000, 0x46000};
static const VecFx32 sPyPtFontObjOffset = {-0xf000, 0, 0};
// Start camera file per stage.
static const char *sStartCamFiles[] = {
    "StaStartCam0.cam", "pyruStartCam0.cam", "aquStartCam0.cam", "VenStartCam0.cam",
    "SubStartCam0.cam", "HaoStartCam0.cam",  "DarkStartCam0.cam", "aniStartCam0.cam",
};
// Map cursor start position per stage, indexed by the player's corner.
static const VecFx32 *sCornerPositions[] = {
    sCornerPositions0, sCornerPositions1, sCornerPositions2, sCornerPositions3,
    sCornerPositions4, sCornerPositions5, sCornerPositions6, sCornerPositions7,
};
static const VecFx32 sCornerCameraPositions[4] = {
    {0x78000, 0x46000, 0xc8000},
    {-0x78000, 0x46000, 0xc8000},
    {-0x78000, 0x46000, -0xc8000},
    {0x78000, 0x46000, -0xc8000},
};
// Camera near and far clip per stage.
static const fx32 sClipPlanes[8][2] = {
    {0x2000, 0xe74000}, {0x2000, 0xe74000}, {0x2000, 0xe74000}, {0x2000, 0xe74000},
    {0x2000, 0xe74000}, {0x2000, 0xe74000}, {0x2000, 0xe74000}, {0x2000, 0xe74000},
};

extern "C" void GetCornerPosition(BattleCommandMap *map, VecFx32 *out);
extern "C" void CreateStatusPanels(BattleCommandMap *map);
extern "C" void UpdateMarkerAndF00gatModels(BattleCommandMap *map);
extern "C" void CreateMapObj(BattleCommandMap *map);
extern "C" void func_ov007_022479d4(BattleCommandMap *map, u32 a);
extern "C" void func_ov007_02247a1c(BattleCommandMap *map, u32 a);
extern "C" void DrawMapObj(BattleCommandMap *map);
extern "C" void UpdateAndDrawMapModels(BattleCommandMap *map);
extern "C" void DrawUnitBakugans(BattleCommandMap *map);
extern "C" void DrawPredictPieces(BattleCommandMap *map);
extern "C" void DrawBakuganModel(BattleCommandMap *map);
extern "C" void DrawCursorUnitFontObjs(BattleCommandMap *map);
extern "C" void DrawStatusPanels(BattleCommandMap *map);
void PredictPosition(VecFx32 *a, VecFx32 *b, int c, VecFx32 *d);


struct Rot16 {
    u16 x, y, z;
};

static inline void ApplyRot(BakuganModel *obj, Rot16 *rot, u16 base);

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

BattleCommandMap::BattleCommandMap(u8 unk)
    : SceneBase(0x7d0, 0) {
    mMarkerSlideStep = 0;
    mState   = 0;
    mKind = unk;
    mCursorX = 0;
    mCursorY = 0;
    mUseUnitBakugans = 0;
    mUseMapModels = 0;
    mPredictVisible = 0;
    mUnk_150 = 0;
    mUseStatusPanels = 1;
    mUnitBakugansVisible = 0;
    mCursorAndMarkerModelsVisible = 0;
    mUnk_154 = 0;
    mDirRotY = 0;
    mDirRotZ = 0;
    mDirScale = 0;
    mF00gatTimer = 0;
    mStopStepCount = 0;
    mCameraMoveTimer = 0;
    mCameraDistance = 0;
    mPredictOffset = 0;
    MI_CpuClear32(mVelocity, 0xc);
    MI_CpuClear32(mLandingPosition, 0xc);
    MI_CpuClear32(mUnk_108, 0xc);
    MI_CpuClear32(mCameraEndTarget, 0xc);
    MI_CpuClear32(mCameraEndPosition, 0xc);
    MI_CpuClear32(mCameraMoveOffset, 0xc);
    MI_CpuClear32(mPredictPositions, 0x60);
    u8 i = 0;
    do {
        mUnk_18[i] = 0;
        i++;
    } while (i < 12);
    mUnk_14 = 0;
    mMarkerModel = 0;
    mCursorModel = 0;
    u8 j = 0;
    do {
        mF00gatModels[j] = 0;
        j++;
    } while (j < 4);
    mMapObj = 0;
    mPredictPiece = 0;
    mLandingPiece = 0;
    mShadowPiece = 0;
    mStatusPanels = 0;
    mStartCamFile = 0;
    if (gGameData.mGimmicks) {
        gGameData.mGimmicks->LoadAll();
    }
    mUnitModels = gGameData.mBattlePlayers->mUnitModels;
    if (mUnitModels) {
        mUnitModels->CreateFontObjs();
    }
    mLights = new ModelLightGroup(sCommandMapLights);
    GetCornerPosition(this, (VecFx32 *) mCornerPosition);
}

BattleCommandMap::~BattleCommandMap() {
    u8 i = 0;
    do {
        mUnitModels->ReleaseBakugan(i);
        i++;
    } while (i < 12);
    if (mUnk_14) {
        delete mUnk_14;
    }
    if (mMarkerModel) {
        delete mMarkerModel;
    }
    if (mCursorModel) {
        delete mCursorModel;
    }
    FreeF00gatModels();
    if (mMapObj) {
        delete mMapObj;
    }
    if (mPredictPiece) {
        delete mPredictPiece;
    }
    if (mLandingPiece) {
        delete mLandingPiece;
    }
    if (mShadowPiece) {
        delete mShadowPiece;
    }
    if (mStatusPanels) {
        delete mStatusPanels;
    }
    FreeStartCamFile();
    if (gGameData.mGimmicks) {
        gGameData.mGimmicks->UnloadAll();
    }
    if (mUnitModels) {
        mUnitModels->Release();
    }
    if (mLights) {
        delete mLights;
    }
}

void BattleCommandMap::FreeStartCamFile() {
    if (mStartCamFile) {
        OS_Free(mStartCamFile);
        mStartCamFile = 0;
    }
}

void BattleCommandMap::FreeF00gatModels() {
    u8 i = 0;
    do {
        BattleMapModel *model = mF00gatModels[i];
        if (model) {
            delete model;
        }
        mF00gatModels[i] = 0;
        i++;
    } while (i < 4);
}

void BattleCommandMap::FreeTimedF00gatModel(u32 index) {
    BattleMapModel *model = mF00gatModels[index];
    if (model == 0) {
        return;
    }
    if (model->mKind != MAP_MODEL_KIND_F00GAT_000_TIMED) {
        return;
    }
    if (model) {
        delete model;
    }
    mF00gatModels[index] = 0;
}

void BattleCommandMap::Update() {
    switch (mState) {
        case 0:
            UpdateCameraMove();
            UpdateStartCamPath();
            if (mPredictVisible != 0) {
                UpdatePredictPositions();
            }
            func_ov007_022458f4();
            func_ov007_022458cc();
            func_ov007_02246450();
            break;
        case 1:
            func_ov007_02246450();
            if (CameraManager::Get().mMode == 2) {
                SpriteManager::Get().StartFade(FADE_MODE_DOWN);
            } else {
                SpriteManager::Get();
                SpriteManager::sFade[0] = FADE_MODE_DOWN;
            }
            mState++;
            break;
        case 2:
            func_ov007_02246450();
            SpriteManager::Get();
            if (SpriteManager::sFade[0] <= FADE_MODE_HOLD) {
                mState++;
            }
            break;
        case 3:
            RequestScene(1);
            mFinished = 1;
            break;
    }
}

void CloseCommandMapNow(BattleCommandMap *map) {
    map->mState = 3;
}

// State 1: the map fades the screen down and ends after the fade (CloseCommandMapNow skips the fade).
void CloseCommandMap(BattleCommandMap *map) {
    map->mState = 1;
}

void BattleCommandMap::func_ov007_02245894() {
    func_ov007_022479d4(this, 1);
    func_ov007_02247a1c(this, 1);
}

void BattleCommandMap::Init() {
    func_ov007_02245894();
    func_ov007_022458cc();
}

void BattleCommandMap::func_ov007_022458cc() {
    CreateStatusPanels(this);
    UpdateMarkerAndF00gatModels(this);
    CreateMapObj(this);
    mUnitModels->Update();
}

void BattleCommandMap::func_ov007_022458f4() {
    func_ov007_02245f08();
}

void BattleCommandMap::UpdateStartCamPath() {
    if (mStartCamFile == 0) {
        return;
    }
    Camera *camera = gGameData.mBattlePlayers->mCamera;
    if (camera == 0) {
        return;
    }
    camera->AdvanceFrame(0);
    camera->ApplyPath();
    SnapshotCamera();
}

void BattleCommandMap::UpdateCameraMove() {
    if (mCameraMoveTimer == 0) {
        return;
    }
    Camera *camera = gGameData.mBattlePlayers->mCamera;
    if (camera) {
        VecFx32 target = *(VecFx32 *) mCameraEndTarget;
        VecFx32 pos    = *(VecFx32 *) mCameraEndPosition;
        VecFx32 delta;
        mCameraMoveTimer--;
        if (mCameraMoveTimer != 0) {
            float t = (float) (u32) mCameraMoveTimer / 30.0f;
            float s = t * t;
            delta.x = (fx32) (s * (float) (s32) mCameraMoveOffset[0]);
            delta.y = (fx32) (s * (float) (s32) mCameraMoveOffset[1]);
            delta.z = (fx32) (s * (float) (s32) mCameraMoveOffset[2]);
            VEC_Subtract(&target, &delta, &target);
            VEC_Subtract(&pos, &delta, &pos);
        }
        camera->mTarget   = target;
        camera->mPosition = pos;
    }
    SnapshotCamera();
}

void BattleCommandMap::UpdatePredictPositions() {
    float step = (float) mStopStepCount / 8.0f;
    u8 i       = 0;
    do {
        PredictPosition((VecFx32 *) mCornerPosition, (VecFx32 *) mVelocity, (u32) (mPredictOffset + (float) i * step), (VecFx32 *) &mPredictPositions[i * 0xc]);
        i++;
    } while (i < 8);
    float limit = (float) mStopStepCount / 8.0f;
    mPredictOffset    = mPredictOffset + limit / 30.0f;
    if (mPredictOffset >= limit) {
        mPredictOffset = 0;
    }
}


void BattleCommandMap::func_ov007_02245f08() {
    if (mUnk_150 == 0) {
        return;
    }
    BakuganModel *obj; 
    obj = mUnk_14;
    if (obj == 0) {
        return;
    }
    u16 angle = 0; 
    BattlePlayer *player; 
    s32 dy;
    s32 sx;
    s32 sy;
    float fy;
    s32 dx;
    float fx;
    player = gGameData.mBattlePlayers->Current();
    dy     = -player->mPushSumZ;
    dx     = player->mPushSumX;
    s32 ay = dy;
    if (ay < 0) {
        ay = -ay;
    }
    if (ay < 0x31) {
        sy = 0;
    } else if (dy < 0) {
        sy = -1;
    } else if (dy > 0) {
        sy = 1;
    }
    s32 ax = dx;
    if (ax < 0) {
        ax = -ax;
    }
    if (ax < 0x31) {
        sx = 0;
    } else if (dx < 0) {
        sx = -1;
    } else if (dx > 0) {
        sx = 1;
    }

    fy     = (float) dy / 4096.0f;
    fx     = (float) dx / 4096.0f;
    if (sx != 0) {
        if (fx < 0.0f) {
            fx = 0.0f - fx;
        }
        fx *= 14.84058f;
    }
    if (sy != 0) {
        if (fy < 0.0f) {
            fy = 0.0f - fy;
        }
        fy *= 14.84058f;
    }
    if (sx == 0) {
        fx = 0;
        fy *= 2.0f;
    }
    if (sy == 0) {
        fy = 0;
        fx *= 2.0f;
    }
    float m = fy + fx;
    m *= 12.0f;
    Rot16 rot = {0, 0, 0};
    if (sx == 0 && sy == 0) {
    } else if (sy != 0 && sx == 0) {
        if (sy < 0) {
            rot.z = 65536.0f * (360.0f - m) / 360.0f;
        } else {
            rot.z = 65536.0f * m / 360.0f;
        }
        ApplyRot(obj, &rot, 0);
        angle += 90;
    } else if (sx != 0 && sy == 0) {
        if (sx < 0) {
            rot.z = 65536.0f * m / 360.0f;
        } else {
            rot.z = 65536.0f * (360.0f - m) / 360.0f;
        }
        ApplyRot(obj, &rot, 0x4000);
    } else if (sx < 0 && sy < 0) {
        rot.z = 65536.0f * m / 360.0f;
        angle += 130;
        ApplyRot(obj, &rot, 0x78e3);
    } else if (sx > 0 && sy < 0) {
        rot.z = 65536.0f * m / 360.0f;
        angle += 60;
        ApplyRot(obj, &rot, 0x98e3);
    } else if (sx > 0 && sy > 0) {
        rot.z = 65536.0f * (360.0f - m) / 360.0f;
        angle += 130;
        ApplyRot(obj, &rot, 0x78e3);
    } else if (sx < 0 && sy > 0) {
        rot.z = 65536.0f * (360.0f - m) / 360.0f;
        angle += 60;
        ApplyRot(obj, &rot, 0x98e3);
    }
    obj->mRotY = (u16) (65536.0f * (float) (u16) (angle % 360) / 360.0f);
    Camera *camera = gGameData.mBattlePlayers->mCamera;
    if (camera) {
        obj->mRotY = obj->mRotY + camera->mYaw;
    }
    BakuganModel *dst = mUnk_14;
    dst->mRotX = obj->mRotX;
    dst->mRotY = obj->mRotY;
    dst->mRotZ = obj->mRotZ;
}




static inline void ApplyRot(BakuganModel *obj, Rot16 *rot, u16 base) {
    u16 x = obj->mRotX + rot->x;
    u16 y = obj->mRotY + rot->y;
    u16 z = obj->mRotZ + rot->z;
    obj->mRotX = x;
    obj->mRotY = y;
    obj->mRotZ = z;
    obj->mRotX = base;
}

void BattleCommandMap::func_ov007_02246450() {
    DrawMapObj(this);
    UpdateAndDrawMapModels(this);
    DrawUnitBakugans(this);
    if (mPredictVisible != 0) {
        DrawPredictPieces(this);
    }
    DrawBakuganModel(this);
    gGameData.mEffects->UpdateTagged(1);
    BOOL show = TRUE;
    if (CameraManager::Get().mMode == 2) {
        if (CameraManager::Get().mCameraIndex != 0) {
            show = FALSE;
        }
    }
    if (show) {
        DrawCursorUnitFontObjs(this);
        DrawStatusPanels(this);
    }
}

void GetCellPosition(VecFx32 *out, u8 a, u8 b);

extern "C" void DrawStatusPanels(BattleCommandMap *map) {
    if (map->mUseStatusPanels != 0 && map->mStatusPanels) {
        map->mStatusPanels->DrawPanels();
    }
}

extern "C" void DrawCursorUnitFontObjs(BattleCommandMap *map) {
    s8 index               = gGameData.mBattlePlayers->mUnitHandles[map->mCursorX][map->mCursorY];
    u8 current             = gGameData.mBattlePlayers->mLocalPlayer;
    if (index <= 0) {
        return;
    }
    BattleUnit *unit = &gGameData.mBattlePlayers->Units()[(u8) (s8) (index - 1)];
    if (unit->mStatus == 0) {
        return;
    }
    VecFx32 pos;
    pos.x = unit->mPos.x;
    pos.y = unit->mPos.y + 0x2299a;
    pos.z = unit->mPos.z;
    map->mUnitModels->DrawFontObj(unit->mPlayer, &pos);
    pos.y += 0xa000;
    CameraManager::Get();
    MtxFx43 mtx = data_020c1cf4;
    VecFx32 v   = sPyPtFontObjOffset;
    mtx._[11]   = 0;
    mtx._[10]   = 0;
    mtx._[9]    = 0;
    MTX_MultVec43(&v, &mtx, &v);
    VEC_Add(&pos, &v, &pos);
    if (unit->mPlayer == current) {
        map->mUnitModels->DrawPyFontObj(&pos);
    } else if (unit->mPlayer == gGameData.mBattlePlayers->mPlayers[current]->mPartner) {
        map->mUnitModels->DrawPtFontObj(&pos);
    }
}

extern "C" void DrawBakuganModel(BattleCommandMap *map) {
    if (map->mUnk_150 != 0 && map->mUnk_154 != 0 && map->mUnk_14) {
        map->mUnk_14->DrawModel();
    }
}

extern "C" void DrawUnitBakugans(BattleCommandMap *map) {
    if (map->mUseUnitBakugans != 0 && map->mUnitBakugansVisible != 0) {
        map->mUnitModels->Draw();
    }
}

extern "C" void DrawPredictPieces(BattleCommandMap *map) {
    u8 i = 0;
    do {
        map->mPredictPiece->mPosition = *(VecFx32 *) ((u8 *) map + i * 0xc + 0x78);
        map->mPredictPiece->DrawInstance();
        i++;
    } while (i < 8);
    VecFx32 v = *(VecFx32 *) map->mLandingPosition;
    map->mLandingPiece->mPosition = v;
    map->mLandingPiece->DrawInstance();
}


extern "C" void UpdateAndDrawMapModels(BattleCommandMap *map) {
    if (map->mUseMapModels == 0) {
        return;
    }
    u8 i = 0;
    do {
        if (map->mF00gatModels[i]) {
            map->mF00gatModels[i]->DrawInstance();
        }
        i++;
    } while (i < 4);
    if (map->mCursorAndMarkerModelsVisible == 0) {
        return;
    }
    VecFx32 a;
    GetCellPosition(&a, map->mCursorX, map->mCursorY);
    if (map->mCursorModel) {
        a.y += 0x75c;
        map->mCursorModel->mPosition = a;
        map->mCursorModel->DrawInstance();
    }
    s32 neg = -0x6000;
    u8 j    = 0;
    do {
        BattleMarker *marker = &gGameData.mBattlePlayers->Markers()[j];
        if (marker->mStatus == 1) {
            if (map->mMarkerModel) {
                VecFx32 b;
                GetCellPosition(&b, marker->mCellX, marker->mCellY);
                map->mMarkerModel->mPosition = b;
                s32 frame = map->mMarkerModel->mModel->GetAnimFrame();
                if (marker->mPlayer == gGameData.mBattlePlayers->mLocalPlayer) {
                    map->mMarkerModel->DrawPulsing();
                } else {
                    map->mMarkerModel->mModel->GetAnimFrame();
                    map->mMarkerModel->mModel->SetAnimFrame(0);
                    map->mMarkerModel->DrawInstance();
                }
                map->mMarkerModel->mModel->SetAnimFrame(frame);
            }
        } else if (marker->mStatus == 3) {
            if (map->mMarkerModel) {
                VecFx32 c;
                GetCellPosition(&c, marker->mCellX, marker->mCellY);
                if (map->mMarkerSlideStep == 1) {
                    map->mMarkerSlideX = map->mMarkerSlideX - 0x3000;
                    if (!map->mMarkerModel->mModel->AdvanceAnim()) {
                        map->mMarkerModel->mModel->mAnim->mFramePerFrame = neg;
                        map->mMarkerSlideX = map->mMarkerSlideX * (neg >> 15);
                        map->mMarkerSlideStep++;
                    }
                } else {
                    map->mMarkerSlideX = map->mMarkerSlideX - 0x3000;
                    map->mMarkerModel->mModel->AdvanceAnim();
                    if (map->mMarkerModel->mModel->GetAnimFrame() <= 0x6000) {
                        gGameData.mBattlePlayers->Markers()[j].mStatus = 1;
                    }
                }
                c.x += map->mMarkerSlideX;
                map->mMarkerModel->mPosition = c;
                map->mMarkerModel->DrawInstance();
            }
        }
        j++;
    } while (j < 12);
}

extern "C" void func_ov007_02246d88(BattleCommandMap *map);
extern "C" void func_ov007_02246ddc(BattleCommandMap *map);
extern "C" void SetCornerCameraOverCursorCell(BattleCommandMap *map);
extern "C" void FlipToCornerQuadrant(BattleCommandMap *map, VecFx32 *v);
extern "C" void SetCursorCellAndMoveCamera(BattleCommandMap *map, u8 a, u8 b, u32 frames);
extern "C" void StartCameraMove(BattleCommandMap *map, VecFx32 *target, VecFx32 *pos, u32 frames);
extern "C" void ComputeCameraDistance(BattleCommandMap *map);
extern "C" void CopyCameraYaw(BattleCommandMap *map);

extern "C" void DrawMapObj(BattleCommandMap *map) {
    if (map->mMapObj) {
        map->mMapObj->Draw();
    }
}

void SetupMapCamera(BattleCommandMap *map, u32 mode) {
    Camera *camera = gGameData.mBattlePlayers->mCamera;
    camera->mUp.x  = 0;
    camera->mUp.y  = 0x1000;
    camera->mUp.z  = 0;
    camera->mNear  = 0x1000;
    camera->mFar   = 0x800000;
    switch (mode) {
        case 3:
            func_ov007_02246d88(map);
            break;
        case 2:
        case 4:
        case 5:
        case 7:
        case 8:
            SetCornerCameraOverCursorCell(map);
            break;
    }
    CameraManager::Get().mCameraIndex = 0;
    CameraManager::Get().SetCamerasAndRequestMode(1, camera, 0);
    SnapshotCamera();
    ComputeCameraDistance(map);
    CopyCameraYaw(map);
}

extern "C" void func_ov007_02246d88(BattleCommandMap *map) {
    Camera *camera = gGameData.mBattlePlayers->mCamera;
    if (camera == 0) {
        return;
    }
    camera->SetFov(0x18e3);
    camera->mTarget = *(VecFx32 *) map->mLandingPosition;
    func_ov007_02246ddc(map);
}

extern "C" void func_ov007_02246ddc(BattleCommandMap *map) {
    Camera *camera = gGameData.mBattlePlayers->mCamera;
    if (camera == 0 || map->mUnk_14 == 0) {
        return;
    }
    camera->mPosition = map->mUnk_14->mPosition;
    camera->ComputeDistanceAndAngles();
    VecFx32 v;
    v.x       = camera->mPosition.x;
    v.y       = camera->mPosition.y;
    v.z       = camera->mPosition.z;
    s32 index = (camera->mYaw >> 4) * 2;
    v.x       = (fx32) ((float) v.x + 156.0f * (float) FX_SinCosTable_[index]);
    v.z       = (fx32) ((float) v.z + 156.0f * (float) FX_SinCosTable_[index + 1]);
    FlipToCornerQuadrant(map, &v);
    fx32 z = v.z;
    camera->mPosition.x = v.x;
    camera->mPosition.y = 0x37000;
    camera->mPosition.z = z;
}


extern "C" void SetCornerCameraOverCursorCell(BattleCommandMap *map) {
    Camera *camera = gGameData.mBattlePlayers->mCamera;
    if (camera) {
        camera->SetFov(sCommandCamAngles[1]);
        VecFx32 v = sCornerCameraPositions[gGameData.mBattlePlayers->mPlayers[gGameData.mBattlePlayers->mLocalPlayer]->mCorner];
        v.y       = sCameraHeights[map->mKind];
        camera->mPosition = v;
        camera->mTarget.x = 0;
        camera->mTarget.y = 0;
        camera->mTarget.z = 0;
    }
    SetCursorCellAndMoveCamera(map, gGameData.mBattlePlayers->mCursorX, gGameData.mBattlePlayers->mCursorY, 0);
}

extern "C" u32 GetCornerRotYDegrees() {
    s32 result = 0;
    switch (gGameData.mBattlePlayers->Current()->mCorner) {
        case 0:
            return result;
        case 1:
            result = 0x10e;
            break;
        case 2:
            result = 0xb4;
            break;
        case 3:
            result = 0x5a;
            break;
    }
    return result;
}

extern "C" void FlipToCornerQuadrant(BattleCommandMap *map, VecFx32 *v) {
    switch (gGameData.mBattlePlayers->Current()->mCorner) {
        case 0:
            if (v->x < 0) {
                v->x = -v->x;
            }
            if (v->z < 0) {
                v->z = -v->z;
            }
            break;
        case 1:
            if (v->x > 0) {
                v->x = -v->x;
            }
            if (v->z < 0) {
                v->z = -v->z;
            }
            break;
        case 2:
            if (v->x > 0) {
                v->x = -v->x;
            }
            if (v->z > 0) {
                v->z = -v->z;
            }
            break;
        case 3:
            if (v->x < 0) {
                v->x = -v->x;
            }
            if (v->z > 0) {
                v->z = -v->z;
            }
            break;
    }
}

void func_ov007_022470e4(BattleCommandMap *map, u8 a, u8 b) {
    SetCursorCellAndMoveCamera(map, a, b, 0x1e);
}

extern "C" void ApplyCameraYaw(BattleCommandMap *map);

// Moves the camera, offset kept, over cell (a, b) in `frames` frames (0: at once); the same cell with 0x1e: no-op.
extern "C" void SetCursorCellAndMoveCamera(BattleCommandMap *map, u8 a, u8 b, u32 frames) {
    if (a == map->mCursorX && b == map->mCursorY && frames == 0x1e) {
        return;
    }
    map->mCursorX = a;
    map->mCursorY = b;
    Camera *camera = gGameData.mBattlePlayers->mCamera;
    if (camera == 0) {
        return;
    }
    VecFx32 pos;
    pos.x = camera->mPosition.x;
    pos.y = camera->mPosition.y;
    pos.z = camera->mPosition.z;
    VecFx32 target;
    target.x = camera->mTarget.x;
    target.y = camera->mTarget.y;
    target.z = camera->mTarget.z;
    VecFx32 w;
    VecFx32 d;
    VecFx32 e;
    GetCellPosition(&w, a, b);
    VEC_Subtract(&pos, &target, &d);
    VEC_Add(&w, &d, &e);
    w.y = target.y;
    e.y = pos.y;
    StartCameraMove(map, &w, &e, frames);
}

extern "C" void RotateCameraYaw(BattleCommandMap *map, u32 direction) {
    if (map->mCameraMoveTimer != 0) {
        return;
    }
    s32 delta = 0;
    if (direction != 0) {
        if (direction == 1) {
            delta = 1;
        }
    } else {
        delta = delta - 1;
    }
    u16 old       = map->mCameraYawDegrees;
    map->mCameraYawDegrees = old + (delta + 360);
    map->mCameraYawDegrees = map->mCameraYawDegrees % 360;
    if (old != map->mCameraYawDegrees) {
        ApplyCameraYaw(map);
    }
}

// Puts the camera at mCameraYawDegrees on a circle of radius mCameraDistance around its target (y kept).
extern "C" void ApplyCameraYaw(BattleCommandMap *map) {
    Camera *camera = gGameData.mBattlePlayers->mCamera;
    if (camera == 0) {
        return;
    }
    u16 angle = (u16) (65536.0f * (float) (s32) map->mCameraYawDegrees / 360.0f);
    s32 index = (angle >> 4) * 2;
    s32 sinv  = FX_SinCosTable_[index];
    s32 cosv  = FX_SinCosTable_[index + 1];
    if (map->mCameraDistance == 0) {
        return;
    }
    s32 radius = (s32) ((float) map->mCameraDistance / 4096.0f);
    s32 x      = (s32) ((float) radius * (float) sinv);
    s32 z      = (s32) ((float) radius * (float) cosv);
    x += camera->mTarget.x;
    z += camera->mTarget.z;
    VecFx32 p;
    p.x = x;
    p.y = camera->mPosition.y;
    p.z = z;
    camera->mPosition = p;
    SnapshotCamera();
}

extern "C" s32 VEC_Distance(VecFx32 *a, VecFx32 *b);
extern "C" void CreateUnitBakugansAndShadow(BattleCommandMap *map);

extern "C" void StartCameraMove(BattleCommandMap *map, VecFx32 *target, VecFx32 *pos, u32 frames) {
    Camera *camera = gGameData.mBattlePlayers->mCamera;
    if (camera == 0) {
        return;
    }
    VecFx32 cur;
    cur.x         = camera->mPosition.x;
    cur.y         = camera->mPosition.y;
    cur.z         = camera->mPosition.z;
    map->mCameraMoveTimer = frames;
    *(VecFx32 *) map->mCameraEndTarget = *target;
    *(VecFx32 *) map->mCameraEndPosition = *pos;
    VEC_Subtract(pos, &cur, (VecFx32 *) map->mCameraMoveOffset);
    if (map->mCameraMoveTimer != 0) {
        return;
    }
    camera->mTarget   = *(VecFx32 *) map->mCameraEndTarget;
    camera->mPosition = *(VecFx32 *) map->mCameraEndPosition;
}

extern "C" void ComputeCameraDistance(BattleCommandMap *map) {
    Camera *camera = gGameData.mBattlePlayers->mCamera;
    if (camera == 0) {
        return;
    }
    VecFx32 pos;
    VecFx32 target;
    pos.x    = camera->mPosition.x;
    pos.y    = camera->mPosition.y;
    pos.z    = camera->mPosition.z;
    target.x = camera->mTarget.x;
    target.y = camera->mTarget.y;
    target.z = camera->mTarget.z;
    target.y = pos.y;
    map->mCameraDistance = VEC_Distance(&target, &pos);
}

// mCameraYawDegrees = the camera's yaw in degrees (0..359; RotateCameraYaw wraps it with % 360).
extern "C" void CopyCameraYaw(BattleCommandMap *map) {
    Camera *camera = gGameData.mBattlePlayers->mCamera;
    if (camera == 0) {
        return;
    }
    camera->ComputeDistanceAndAngles();
    map->mCameraYawDegrees = (u16) ((float) (camera->mYaw * 360) / 65536.0f);
}

extern "C" void CreateStatusPanels(BattleCommandMap *map) {
    if (map->mUseStatusPanels == 0) {
        return;
    }
    if (map->mStatusPanels != 0) {
        return;
    }
    map->mStatusPanels = new BattleStatusPanels(0, 0, 0, 1);
    u8 i         = 0;
    do {
        u8 index             = gGameData.mBattlePlayers->mTurnOrder[i];
        BattlePlayer *player = gGameData.mBattlePlayers->mPlayers[index];
        map->mStatusPanels->SetupPanel(index, (s8) player->mCharacterId, data_ov007_02279eb0[i], 2);
        i++;
    } while (i < gGameData.mPlayerCount);
}

extern "C" void CreateUnitBakugansAndShadow(BattleCommandMap *map) {
    if (map->mUseUnitBakugans == 0) {
        return;
    }
    VecFx32 pos; 
    u8 i = 0;    
    do {
        BattleUnit *unit = &gGameData.mBattlePlayers->Units()[i];
        if (unit->mStatus != 0) {
            pos.x = unit->mPos.x;
            pos.y = unit->mPos.y;
            pos.z = unit->mPos.z;
            Rot16 rot = {0, 0, 0};
            rot.y = unit->mRotY;
            BattleRecord *record = &gGameData.mBattlePlayers->mPlayers[unit->mPlayer]->mRecords[unit->mRecord];
            map->mUnitModels->CreateBakugan(i, &pos, &rot.x, record->mBakuganId, record->mAttribute);
        }
        i++;
    } while (i < 12);
    if (map->mShadowPiece == 0) {
        map->mShadowPiece = new BattleMapPiece(MAP_PIECE_KIND_SPHERE_SHADOW);
    }
}

extern "C" void UpdateMarkerAndF00gatModels(BattleCommandMap *map) {
    if (map->mUseMapModels == 0) {
        return;
    }
    u8 i = 0;
    do {
        if (gGameData.mBattlePlayers->Markers()[i].mStatus == 1 && map->mMarkerModel == 0) {
            map->mMarkerModel = new BattleMapModel(MAP_MODEL_KIND_CARD_00_00);
        }
        i++;
    } while (i < 12);
    u8 j = 0;
    do {
        BattleMapModel *model = map->mF00gatModels[j];
        if (model) {
            if (map->mF00gatTimer == 0) {
                map->FreeTimedF00gatModel(j);
            } else if (map->mF00gatTimer != 0) {
                model->mAnimEnabled = 1;
            }
        }
        j++;
    } while (j < 4);
    if (map->mF00gatTimer != 0) {
        map->mF00gatTimer--;
    }
}

extern "C" void CreateMapObj(BattleCommandMap *map) {
    if (map->mMapObj != 0) {
        return;
    }
    map->mMapObj = new BattleMapObj(map->mKind);
    map->mMapObj->Load();
}

extern "C" void SetUseUnitBakugans(BattleCommandMap *map, u32 a) {
    map->mUseUnitBakugans = a != 0;
    CreateUnitBakugansAndShadow(map);
}

extern "C" void SetUnitBakugansVisible(BattleCommandMap *map, u32 a) {
    map->mUnitBakugansVisible = a != 0;
}

extern "C" void func_ov007_022479d4(BattleCommandMap *map, u32 a) {
    SetUseUnitBakugans(map, a);
    SetUnitBakugansVisible(map, a);
}

extern "C" void SetUseMapModels(BattleCommandMap *map, u32 a) {
    map->mUseMapModels = a != 0;
}

extern "C" void SetCursorAndMarkerModelsVisible(BattleCommandMap *map, u32 a) {
    map->mCursorAndMarkerModelsVisible = a != 0;
}

extern "C" void func_ov007_02247a1c(BattleCommandMap *map, u32 a) {
    SetUseMapModels(map, a);
    SetCursorAndMarkerModelsVisible(map, a);
}

void CreateCursorModel(BattleCommandMap *map) {
    if (map->mUseMapModels == 0) {
        return;
    }
    if (map->mCursorModel != 0) {
        return;
    }
    map->mCursorModel = new BattleMapModel(MAP_MODEL_KIND_CARD_FUTI);
}

extern "C" void SetupTimedF00gatModelAndCamera(BattleCommandMap *map) {
    if (map->mUseMapModels == 0) {
        return;
    }
    map->FreeF00gatModels();
    if (map->mF00gatModels[0] != 0) {
        return;
    }
    BattleMapModel *model = new BattleMapModel(MAP_MODEL_KIND_F00GAT_000_TIMED);
    map->mF00gatModels[0]       = model;
    model->mAnimEnabled        = 0;
    map->mF00gatModels[0]->StartAnim(0, 0);
    VecFx32 a;
    GetCellPosition(&a, gGameData.mBattlePlayers->mCursorX, gGameData.mBattlePlayers->mCursorY);
    map->mF00gatModels[0]->mPosition = a;
    map->mF00gatTimer              = 0x97;
    Camera *camera             = gGameData.mBattlePlayers->mCamera;
    if (camera) {
        camera->mTarget = a;
        a.y += 0x50000;
        a.z += 0x46000;
        camera->mPosition = a;
    }
    CameraManager::Get().mCameraIndex = 0;
    CameraManager::Get().SetCamerasAndRequestMode(1, camera, 0);
    SnapshotCamera();
}

extern "C" void SetupF00gatModelsAndCamera(BattleCommandMap *map) {
    if (map->mUseMapModels == 0) {
        return;
    }
    map->FreeF00gatModels();
    u8 count    = gGameData.mPlayerCount;
    u8 coords[4][2] = {{6, 4}, {7, 5}, {7, 4}, {6, 5}};
    if (gGameData.mLastBattle != 0) {
        if (map->mF00gatModels[0] == 0) {
            BattleMapModel *model = new BattleMapModel(MAP_MODEL_KIND_F00GAT_000);
            map->mF00gatModels[0]       = model;
            model->mAnimEnabled        = 1;
            map->mF00gatModels[0]->StartAnim(0, 0);
            VecFx32 a;
            GetCellPosition(&a, coords[0][0], coords[0][1]);
            map->mF00gatModels[0]->mPosition = a;
        }
    } else {
        u8 i = 0;
        do {
            if (gGameData.mBattlePlayers->mPlayers[i]->mSlots54Remaining != 0 && map->mF00gatModels[i] == 0) {
                BattleMapModel *model = new BattleMapModel((u8) (i + MAP_MODEL_KIND_F00GAT_000));
                map->mF00gatModels[i]       = model;
                model->mAnimEnabled        = 1;
                map->mF00gatModels[i]->StartAnim(0, 0);
                VecFx32 b;
                GetCellPosition(&b, coords[i][0], coords[i][1]);
                map->mF00gatModels[i]->mPosition = b;
            }
            i++;
        } while (i < count);
    }
    Camera *camera = gGameData.mBattlePlayers->mCamera;
    if (camera) {
        camera->SetFov(0x2aaa);
        if (gGameData.mLastBattle != 0) {
            camera->mTarget.x   = -0x11800;
            camera->mTarget.y   = 0;
            camera->mTarget.z   = -0x17800;
            camera->mPosition.x = -0x11800;
            camera->mPosition.y = 0x58000;
            camera->mPosition.z = 0x4c800;
        } else {
            camera->mTarget.x   = 0;
            camera->mTarget.y   = 0;
            camera->mTarget.z   = 0;
            camera->mPosition.x = 0;
            camera->mPosition.y = 0x58000;
            camera->mPosition.z = 0x64000;
        }
    }
    CameraManager::Get().mCameraIndex = 0;
    CameraManager::Get().SetCamerasAndRequestMode(1, camera, 0);
    SnapshotCamera();
}

extern "C" void DrawShadowPiece(BattleCommandMap *map, VecFx32 *v, u32 mode);


extern "C" void StartStartCamPath(BattleCommandMap *map) {
    ChangeDir("/Game/Battle/3d/MapCam");
    map->mUseStatusPanels  = 0;
    Camera *camera = gGameData.mBattlePlayers->mCamera;
    if (camera) {
        map->FreeStartCamFile();
        map->mStartCamFile = LoadFile(sStartCamFiles[map->mKind], 0, 0, 0, 0, 0);
        camera->SetPath(map->mStartCamFile, 0);
        u8 index      = map->mKind;
        camera->mNear = sClipPlanes[index][0];
        camera->mFar  = sClipPlanes[index][1];
        new BattleCardEffect(0, 0, 3, 0);
    }
    CameraManager::Get().mCameraIndex = 0;
    CameraManager::Get().SetCamerasAndRequestMode(2, camera, 0);
    SnapshotCamera();
    ChangeDir("/");
}

extern "C" void SetPredictVisible(BattleCommandMap *map, u32 flag) {
    map->mPredictVisible = flag;
    if ((flag & 0xff) == 0) {
        return;
    }
    if (map->mPredictPiece == 0) {
        map->mPredictPiece = new BattleMapPiece(MAP_PIECE_KIND_LPOINT_01_00);
    }
    if (map->mLandingPiece == 0) {
        map->mLandingPiece = new BattleMapPiece(MAP_PIECE_KIND_LPOINT_00_00);
    }
}

extern "C" void DrawShadowPieceXZ(BattleCommandMap *map, VecFx32 *v) {
    VecFx32 w = {0, 0, 0};
    w.x       = v->x;
    w.z       = v->z;
    DrawShadowPiece(map, &w, 0);
}

extern "C" void DrawShadowPiece(BattleCommandMap *map, VecFx32 *v, u32 mode) {
    if (map->mShadowPiece == 0) {
        return;
    }
    fx32 x = v->x;
    fx32 y = v->y + 0x3800;
    fx32 z = v->z;
    VecFx32 scale = sShadowPieceScale;
    if (mode != 0 && mode != 1) {
        return;
    }
    map->mShadowPiece->mScale = scale;
    VecFx32 *dst = &map->mShadowPiece->mPosition;
    dst->x       = x;
    dst->y       = y;
    dst->z       = z;
    map->mShadowPiece->DrawInstance();
}

extern "C" void GetCornerPosition(BattleCommandMap *map, VecFx32 *out) {
    if (map->mKind >= 8) {
        return;
    }
    *out = sCornerPositions[map->mKind][gGameData.mBattlePlayers->Current()->mCorner];
}

extern "C" void PredictLandingAndSetCameraTarget(BattleCommandMap *map);

extern "C" void ComputeVelocity(BattleCommandMap *map) {
    VecFx32 v;
    v.x = 0x1000;
    v.y = 0;
    v.z = 0;
    u16 rot[3];
    u32 degrees = GetCornerRotYDegrees();
    rot[0] = 0;
    rot[1] = map->mDirRotY + (u16) (65536.0f * (float) degrees / 360.0f);
    rot[2] = map->mDirRotZ;
    RotateDirScaled(rot, map->mDirScale, &v, (VecFx32 *) map->mVelocity);
}

// Also stores PredictStop's step count in mStopStepCount.
extern "C" void PredictLandingAndSetCameraTarget(BattleCommandMap *map) {
    VecFx32 tmp;
    PredictLanding((VecFx32 *) map->mCornerPosition, (VecFx32 *) map->mVelocity, (VecFx32 *) map->mLandingPosition);
    map->mLandingPosition[1] = 0x24cd;
    map->mStopStepCount   = PredictStop((VecFx32 *) map->mCornerPosition, (VecFx32 *) map->mVelocity, &tmp, 0, 0);
    Camera *camera  = gGameData.mBattlePlayers->mCamera;
    if (camera == 0) {
        return;
    }
    camera->mTarget = *(VecFx32 *) map->mLandingPosition;
}

extern "C" void SetDirAndPredict(BattleCommandMap *map, u32 a, u32 b, u32 c) {
    u8 changed = 0;
    if (a != map->mDirRotY) {
        map->mDirRotY = a;
        changed       = 1;
    }
    if (b != map->mDirRotZ) {
        map->mDirRotZ = b;
        changed       = 1;
    }
    if (c != map->mDirScale) {
        map->mDirScale = c;
        changed       = 1;
    }
    if (changed == 0) {
        return;
    }
    ComputeVelocity(map);
    PredictLandingAndSetCameraTarget(map);
}

extern "C" void StartMarkerSlide(BattleCommandMap *map) {
    map->mMarkerModel->mModel->StartAnimWithIncrement(0, 0, 0x6000);
    map->mMarkerSlideStep = 1;
    map->mMarkerSlideX = 0;
}

void GetCellPosition(VecFx32 *out, u8 a, u8 b) {
    if (a >= 14 || b >= 10) {
        return;
    }
    s8 x    = a - 6;
    s8 z    = b - 4;
    out->x = x * 0x23000 - 0x11800;
    out->y = 0x24cd;
    out->z = z * 0x2f000 - 0x17800;
}

extern "C" u32 FindMarkerCellAt(VecFx32 *v, u8 *outA, u8 *outB) {
    u8 i = 0;
    do {
        BattleMarker *marker = &gGameData.mBattlePlayers->Markers()[i];
        if (marker->mStatus != 0) {
            VecFx32 p;
            GetCellPosition(&p, marker->mCellX, marker->mCellY);
            s32 x0 = p.x - 0x11800;
            s32 x1 = p.x + 0x11800;
            s32 z0 = p.z - 0x17800;
            s32 z1 = p.z + 0x17800;
            if (v->x >= x0 && v->x <= x1 && v->z >= z0 && v->z <= z1) {
                *outA = marker->mCellX;
                *outB = marker->mCellY;
                return 1;
            }
        }
        i++;
    } while (i < 12);
    return 0;
}
