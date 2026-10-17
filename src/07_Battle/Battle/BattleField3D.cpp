#include "Battle/BattleBase.hpp"

#include "Game/UnkStruct_020d433c.hpp"
#include "Graphics/Camera.hpp"
#include "Graphics/Model.hpp"
#include "Graphics/SpriteManager.hpp"
#include "System/FileSystem.hpp"
#include <nitro/fx.h>
#include <nitro/os.h>
#include <stdio.h>

extern BattleCamRow data_ov007_022791ec[];
extern const u8 data_ov007_022791c4[];
extern const VecFx32 data_ov007_022791ac;
extern const VecFx32 data_ov007_022791b8;
extern const VecFx32 data_ov007_022791a0;
extern "C" void func_01ffd0e0(MtxFx43 *mtx, u16 angle);
void func_ov007_02229998(BattleField3D *obj);

// Effect (particle) system functions: effects are loaded by name into numbered slots, spawned as emitters addressed by
// a 16-bit handle, and moved by handle.
void LoadEffectGroup(EffectManager *manager, u32 slot, const char *name);
void FreeEffectGroup(EffectManager *manager, u32 slot);
u16 StartEffect(EffectManager *manager, u32 a, u32 b, u32 c, u32 d);
void FreeEffect(EffectManager *manager, u16 handle);
void SetEffectPosition(EffectManager *manager, u16 handle, VecFx32 *position);

// Copies the active camera into the 3D engine state and the camera manager's snapshot of it.
static inline void SnapshotCamera() {
    CameraManager &manager        = CameraManager::Get();
    Camera *camera                = manager.mCameras[manager.mActive];
    data_02211374.mCameraPosition = camera->mPosition;
    data_02211374.mCameraUp       = camera->mUp;
    data_02211374.mCameraTarget   = camera->mTarget;
    func_01ff8d34(&camera->mPosition, &camera->mUp, &camera->mTarget, &data_02211374.mView);
    data_02211374.mFlags &= ~0xe8;
    manager.mProjection[manager.mActive] = data_02211374.mProjection;
    manager.mView[manager.mActive]       = data_02211374.mView;
    manager.mMtxA[manager.mActive]       = *func_02032710();
    manager.mMtxB[manager.mActive]       = *func_02032604();
    manager.mMtxC[manager.mActive]       = *func_020323b8();
}

BattleField3D::BattleField3D(BattleData *data, int unk2) {
    mUnk_04_0 = 0;
    mUnk_04_1 = 1;
    mUnk_04_2 = 1;
    mAura[0] = -1;
    mAura[1] = -1;
    mSide0EndX = 0;
    mSide1EndX = 0;
    mAuraX = 0;
    mFollowOffset = 0;
    mMovePerFrame = 10240.0f;
    mMapObj = 0;
    mLights = 0;
    mCamFile = 0;
    mEffects = data_020d433c.mEffects;
    mData = data;
    Camera *camera = data_020d433c.mBattlePlayers->mCamera;
    mMapObj = new BattleMapObj(0xd);
    mMapObj->Load();
    camera->SetFov(0x1555);
    CameraManager::Get().SetCamerasAndRequestMode(1, camera, 0);
    ChangeDir("/Game/Battle/3d/MapCam");
    mCamFile = LoadFile("commandfiled0.cam", 0, 0, 0, 0, 0);
    ChangeDir("/");
    camera->SetPath(mCamFile, 0);
    camera->ApplyPath();
    SnapshotCamera();
    mLights = new ModelLightGroup(data_ov007_022791c4);
    BattleEntry *entries = data->mEntries;
    mSide0RealBaku = new BattleRealBaku(entries[0].mUnk_0c, 0xb, entries[0].mUnk_0d_0, 1, 0, 0, entries[0].mUnk_0e == 0x15);
    mSide1RealBaku = new BattleRealBaku(entries[1].mUnk_0c, 0xb, entries[1].mUnk_0d_0, 1, 0, 0, entries[1].mUnk_0e == 0x15);
    BattleCamRow *row = &data_ov007_022791ec[entries[0].mUnk_0c];
    s32 z0 = row->mZ;
    s32 y0 = row->mY;
    mPos[0].x = -0x1a000;
    mPos[0].y = y0;
    mPos[0].z = z0;
    mSide0EndX = row->mX;
    row = &data_ov007_022791ec[entries[1].mUnk_0c];
    s32 z1 = row->mZ;
    s32 y1 = row->mY;
    mPos[1].x = 0x1a000;
    mPos[1].y = y1;
    mPos[1].z = z1;
    mSide1EndX = -row->mX;
    char name[0x30];
    sprintf(name, "aura%02d", entries[0].mUnk_0d_0);
    LoadEffectGroup(mEffects, 0, name);
    sprintf(name, "aura%02d", entries[1].mUnk_0d_0);
    LoadEffectGroup(mEffects, 1, name);
    mAuraX = -1556;
    switch (unk2) {
        case 1:
            mPos[0].x = mSide0EndX;
            mPos[1].x = mSide1EndX;
            mUnk_04_1 = 0;
            mUnk_04_2 = 0;
            CreateAuras();
            mUnk_04_0 = 1;
        case 2:
            camera->SetFrameIndex(camera->mFrameCount);
            camera->ApplyPath();
            SnapshotCamera();
            break;
    }
}

BattleField3D::~BattleField3D() {
    func_ov007_02229998(this);
    FreeEffectGroup(mEffects, 1);
    FreeEffectGroup(mEffects, 0);
    if (mSide1RealBaku) {
        delete mSide1RealBaku;
    }
    if (mSide0RealBaku) {
        delete mSide0RealBaku;
    }
    ModelLightGroup *palette = mLights;
    if (palette) {
        delete palette;
    }
    OS_Free(mCamFile);
    BattleMapObj *map = mMapObj;
    if (map) {
        delete map;
    }
    SpriteManager::Get().func_0202740c();
}

// The caller passes its BattleField3D, which the function doesn't use.
BOOL func_ov007_02229524(BattleField3D *field) {
    Camera *camera = data_020d433c.mBattlePlayers->mCamera;
    camera->AdvanceFrame(0);
    camera->ApplyPath();
    SnapshotCamera();
    return camera->mFrameIndex + 1 >= camera->mFrameCount;
}

void func_ov007_02229758(BattleField3D *obj) {
    obj->mUnk_04_1 = 0;
    obj->mUnk_04_2 = 0;
}

BOOL func_ov007_02229770(BattleField3D *obj) {
    obj->mPos[1].x -= 0x1000;
    obj->mPos[0].x += 0x1000;
    BOOL reached = 0;
    BOOL done = 0;
    if (obj->mPos[0].x >= obj->mSide0EndX) {
        obj->mPos[0].x = obj->mSide0EndX;
        reached = 1;
    }
    if (obj->mPos[1].x <= obj->mSide1EndX) {
        if (reached) {
            done = 1;
        }
        obj->mPos[1].x = obj->mSide1EndX;
    }
    if (!done) {
        return 0;
    }
    obj->CreateAuras();
    obj->mUnk_04_0 = 1;
    return 1;
}

BOOL func_ov007_022297f0(BattleField3D *obj) {
    BattleData *data = obj->mData;
    if (data->mEntries[0].mUnk_10) {
        float a = 0.0f - obj->mMovePerFrame / 3.0f;
        obj->MoveModels(a, 0.0f - obj->mMovePerFrame);
    } else {
        obj->MoveModels(obj->mMovePerFrame, obj->mMovePerFrame / 3.0f);
    }
    obj->mMovePerFrame -= 1229.0f;
    if (obj->mMovePerFrame < -4096.0f) {
        obj->mMovePerFrame = 0;
    }
    if (obj->mMovePerFrame == 0.0f) {
        if (data->mEntries[0].mUnk_10) {
            obj->mUnk_04_2 = 1;
        } else {
            obj->mUnk_04_1 = 1;
        }
        return 1;
    }
    return 0;
}

void BattleField3D::MoveModels(float a, float b) {
    mPos[0].x = (s32) ((float) mPos[0].x - a);
    mPos[1].x = (s32) ((float) mPos[1].x - b);
}

void BattleField3D::CreateAuras() {
    mAura[0] = StartEffect(mEffects, 0, 0, 0, 0);
    mAura[1] = StartEffect(mEffects, 1, 1, 0, 0);
    VecFx32 pos = data_ov007_022791ac;
    pos.x = mAuraX;
    SetEffectPosition(mEffects, mAura[0], &pos);
    pos.x = mAuraX + 0xccd;
    SetEffectPosition(mEffects, mAura[1], &pos);
}

void func_ov007_02229998(BattleField3D *obj) {
    if (obj->mAura[0] != -1) {
        FreeEffect(obj->mEffects, obj->mAura[0]);
        obj->mAura[0] = -1;
    }
    if (obj->mAura[1] != -1) {
        FreeEffect(obj->mEffects, obj->mAura[1]);
        obj->mAura[1] = -1;
    }
}

void BattleField3D::FollowPushPercent(u32 frame) {
    if (frame < 0x14) {
        frame = 0x14;
    } else if (frame > 0x50) {
        frame = 0x50;
    }
    float k = 4.0f;
    float t = (float) (u8) (frame - 0x14) * k / 60.0f;
    float f;
    if (t > 0.0f) {
        f = 0.5f + 4096.0f * t;
    } else {
        f = 4096.0f * t - 0.5f;
    }
    s32 target = (s32) f - 0x2000;
    s32 delta = target - mFollowOffset;
    mPos[0].x += delta;
    mPos[1].x += delta;
    mAuraX += delta / 4;
    VecFx32 pos = data_ov007_022791b8;
    pos.x = mAuraX;
    if (mAura[0] != -1) {
        SetEffectPosition(mEffects, mAura[0], &pos);
    }
    if (mAura[1] != -1) {
        pos.x = mAuraX + 0xccd;
        SetEffectPosition(mEffects, mAura[1], &pos);
    }
    mFollowOffset = target;
}

void BattleField3D::Draw(u32 frame) {
    VecFx32 scale = data_ov007_022791a0;
    MtxFx43 mtx = gIdentityMtx43;
    if (mUnk_04_0) {
        FollowPushPercent(frame);
    }
    if (!mUnk_04_1) {
        func_01ffcf64(&mtx, &mPos[0]);
        func_01ffd0e0(&mtx, 0x3fff);
        mSide0RealBaku->mModel->AdvanceAnim();
        mSide0RealBaku->mModel->Draw(&mtx, &scale);
    }
    if (!mUnk_04_2) {
        mtx = gIdentityMtx43;
        func_01ffcf64(&mtx, &mPos[1]);
        func_01ffd0e0(&mtx, 0xc001);
        mSide1RealBaku->mModel->AdvanceAnim();
        mSide1RealBaku->mModel->Draw(&mtx, &scale);
    }
    mMapObj->Draw();
}
