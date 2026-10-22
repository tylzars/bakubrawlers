#include "Battle/BattleConfrontCard.hpp"
#include "Battle/BattleCardEffect.hpp"
#include "Battle/BattleMapTypes.hpp"
#include "Game/GameData.hpp"
#include "Graphics/SpriteManager.hpp"
#include "Graphics/Camera.hpp"
#include "Graphics/Model.hpp"
#include "Graphics/ModelTransform.hpp"
#include "System/FileSystem.hpp"
#include <nitro/os.h>

BattleCardCamera::BattleCardCamera(u16 a, u16 b)
    : SceneBase(0x3e8, 0) {
    mLights = 0;
    mUnk_24 = 0;
    mState  = 0;
    mMapModelVisible = 1;
    mUnk_29 = 0;
    mUnk_2a = 0;
    mSide0BakuganId = a;
    mSide1BakuganId = b;
    mSide0Variant = a >> 8;
    mSide1Variant = b >> 8;
    u8 i    = 0;
    do {
        mBakugans[i] = 0;
        i++;
    } while (i < 2);
    mMapModel = 0;
    mMapObj = 0;
}

BattleCardCamera::~BattleCardCamera() {
    if (mMapObj) {
        delete mMapObj;
    }
    u8 i = 0;
    do {
        BakuganModel *p = mBakugans[i];
        if (p) {
            delete p;
        }
        i++;
    } while (i < 2);
    if (mMapModel) {
        delete mMapModel;
    }
    if (mLights) {
        delete mLights;
    }
    if (mCamFile) {
        OS_Free(mCamFile);
    }
}

void BattleCardCamera::Update() {
    switch (mState) {
        case 0:
            Init();
            mUnk_24 = 0;
            mState++;
            SpriteManager::Get();
            SpriteManager::sFade[0] = FADE_MODE_TO_ZERO;
            break;
        case 1:
            SpriteManager::Get();
            if (SpriteManager::sFade[0] <= FADE_MODE_HOLD) {
                new BattleCardEffect(0, 0, 0, 0);
                mState++;
            }
            break;
        case 3:
            SpriteManager::Get();
            if (SpriteManager::sFade[0] <= FADE_MODE_HOLD) {
                mState++;
            }
            break;
        case 4:
            RequestScene(1);
            mFinished = 1;
            break;
    }
    UpdateCameraAndDraw();
}


extern const u8 gCardCameraLights[];
// The battle camera through an inline: the original keeps it as a compiler temporary (r5, above the path strings).
static inline Camera *BattleCamera() { return gGameData.mBattlePlayers->mCamera; }

void BattleCardCamera::Init() {
    mBakugans[0] = new BakuganModel();
    mBakugans[0]->Setup(mSide0BakuganId, mSide0Variant, BAKUGAN_MODEL_MODE_H_HOLD_FRAME);
    mBakugans[0]->mAnimUpdateEnabled = 1;
    mBakugans[1] = new BakuganModel();
    mBakugans[1]->Setup(mSide1BakuganId, mSide1Variant, BAKUGAN_MODEL_MODE_H_HOLD_FRAME);
    mBakugans[1]->mAnimUpdateEnabled = 1;
    BakuganModel *p0 = mBakugans[0];
    fx32 height            = 0x6000;
    p0->mPosition.x = -0xe000;
    p0->mPosition.y = height;
    p0->mPosition.z = 0;
    p0 = mBakugans[0];
    p0->mRotX = 0;
    p0->mRotY = 0x4000;
    p0->mRotZ = 0;
    BakuganModel *p1 = mBakugans[1];
    p1->mPosition.x = 0xe000;
    p1->mPosition.y = height;
    p1->mPosition.z = 0;
    p1 = mBakugans[1];
    p1->mRotX = 0;
    // Placeholder: the original stores -0x4000 as a signed 16-bit value (built as 0xe000 - 0x12000).
    *(s16 *) &p1->mRotY = -0x4000;
    p1->mRotZ = 0;
    mMapModel = new BattleMapModel(MAP_MODEL_KIND_O00BTL_000);
    mMapModel->StartAnim(0, 0);
    BattleMapModel *model = mMapModel;
    model->mRotX = 0;
    model->mRotY = 0x4000;
    model->mRotZ = 0;
    mMapObj = new BattleMapObj(MAP_OBJ_KIND_REALPOP);
    mMapObj->Load();
    Camera *camera = BattleCamera();
    camera->SetUp(0, 0x1000, 0);
    camera->SetClip(0x1000, 0x1388000);
    camera->SetFov(0x18e3);
    camera->SetPosition(0x14cd, 0x1ddec, 0x6b852);
    camera->SetTarget(0, 0x7000, 0);
    ChangeDir("/Game/Battle/3d/MapCam");
    mCamFile = LoadFile("card_cam_00.cam", 0, 0, 0, 0, 0);
    ChangeDir("/");
    if (mCamFile) {
        camera->SetPath(mCamFile, 0);
    }
    CameraManager::Get().mCameraIndex = 0;
    CameraManager::Get().SetCamerasAndRequestMode(1, camera, 0);
    mLights = new ModelLightGroup(gCardCameraLights);
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

void BattleCardCamera::UpdateCameraAndDraw() {
    Camera *camera = gGameData.mBattlePlayers->mCamera;
    camera->AdvanceFrame(0);
    camera->ApplyPath();
    SnapshotCamera();
    mBakugans[0]->Update();
    mBakugans[0]->DrawModel();
    mBakugans[1]->Update();
    mBakugans[1]->DrawModel();
    if (mMapModelVisible) {
        mMapModel->DrawInstance();
    }
    mMapObj->Draw();
}
