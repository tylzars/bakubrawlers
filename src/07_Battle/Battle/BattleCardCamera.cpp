#include "Battle/BattleConfrontCard.hpp"
#include "Battle/BattleCardEffect.hpp"
#include "Battle/BattleMapTypes.hpp"
#include "Game/UnkStruct_020d433c.hpp"
#include "Graphics/SpriteManager.hpp"
#include "Graphics/Camera.hpp"
#include "Graphics/Model.hpp"
#include "Graphics/ModelTransform.hpp"
#include "System/FileSystem.hpp"
#include <nitro/os.h>

BattleCardCamera::BattleCardCamera(u16 a, u16 b)
    : SceneBase(0x3e8, 0) {
    mUnk_1c = 0;
    mUnk_24 = 0;
    mState  = 0;
    mUnk_27 = 1;
    mUnk_29 = 0;
    mUnk_2a = 0;
    mUnk_2b = a;
    mUnk_2c = b;
    mUnk_2d = a >> 8;
    mUnk_2e = b >> 8;
    u8 i    = 0;
    do {
        mUnk_10[i] = 0;
        i++;
    } while (i < 2);
    mUnk_18 = 0;
    mUnk_0c = 0;
}

BattleCardCamera::~BattleCardCamera() {
    if (mUnk_0c) {
        delete mUnk_0c;
    }
    u8 i = 0;
    do {
        BakuganModel *p = mUnk_10[i];
        if (p) {
            delete p;
        }
        i++;
    } while (i < 2);
    if (mUnk_18) {
        delete mUnk_18;
    }
    if (mUnk_1c) {
        delete mUnk_1c;
    }
    if (mUnk_20) {
        OS_Free(mUnk_20);
    }
}

void BattleCardCamera::Update() {
    switch (mState) {
        case 0:
            func_ov007_02244bb4();
            mUnk_24 = 0;
            mState++;
            SpriteManager::Get();
            SpriteManager::sFade[0] = 2;
            break;
        case 1:
            SpriteManager::Get();
            if (SpriteManager::sFade[0] <= 1) {
                new BattleCardEffect(0, 0, 0, 0);
                mState++;
            }
            break;
        case 3:
            SpriteManager::Get();
            if (SpriteManager::sFade[0] <= 1) {
                mState++;
            }
            break;
        case 4:
            RequestScene(1);
            mFinished = 1;
            break;
    }
    func_ov007_02244e98();
}


extern const u8 data_ov007_02279e88[];
// The battle camera through an inline: the original keeps it as a compiler temporary (r5, above the path strings).
static inline Camera *BattleCamera() { return data_020d433c.mBattlePlayers->mCamera; }

void BattleCardCamera::func_ov007_02244bb4() {
    mUnk_10[0] = new BakuganModel();
    mUnk_10[0]->Setup(mUnk_2b, mUnk_2d, 3);
    mUnk_10[0]->mAnimUpdateEnabled = 1;
    mUnk_10[1] = new BakuganModel();
    mUnk_10[1]->Setup(mUnk_2c, mUnk_2e, 3);
    mUnk_10[1]->mAnimUpdateEnabled = 1;
    BakuganModel *p0 = mUnk_10[0];
    fx32 height            = 0x6000;
    p0->mPosition.x = -0xe000;
    p0->mPosition.y = height;
    p0->mPosition.z = 0;
    p0 = mUnk_10[0];
    p0->mRotX = 0;
    p0->mRotY = 0x4000;
    p0->mRotZ = 0;
    BakuganModel *p1 = mUnk_10[1];
    p1->mPosition.x = 0xe000;
    p1->mPosition.y = height;
    p1->mPosition.z = 0;
    p1 = mUnk_10[1];
    p1->mRotX = 0;
    // Placeholder: the original stores -0x4000 as a signed 16-bit value (built as 0xe000 - 0x12000).
    *(s16 *) &p1->mRotY = -0x4000;
    p1->mRotZ = 0;
    mUnk_18 = new BattleMapModel(2);
    mUnk_18->StartAnim(0, 0);
    BattleMapModel *model = mUnk_18;
    model->mRotX = 0;
    model->mRotY = 0x4000;
    model->mRotZ = 0;
    mUnk_0c = new BattleMapObj(0xc);
    mUnk_0c->Load();
    Camera *camera = BattleCamera();
    camera->SetUp(0, 0x1000, 0);
    camera->SetClip(0x1000, 0x1388000);
    camera->SetFov(0x18e3);
    camera->SetPosition(0x14cd, 0x1ddec, 0x6b852);
    camera->SetTarget(0, 0x7000, 0);
    ChangeDir("/Game/Battle/3d/MapCam");
    mUnk_20 = LoadFile("card_cam_00.cam", 0, 0, 0, 0, 0);
    ChangeDir("/");
    if (mUnk_20) {
        camera->SetPath(mUnk_20, 0);
    }
    CameraManager::Get().mActive = 0;
    CameraManager::Get().SetCamerasAndRequestMode(1, camera, 0);
    mUnk_1c = new ModelLightGroup(data_ov007_02279e88);
}

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

void BattleCardCamera::func_ov007_02244e98() {
    Camera *camera = data_020d433c.mBattlePlayers->mCamera;
    camera->AdvanceFrame(0);
    camera->ApplyPath();
    SnapshotCamera();
    mUnk_10[0]->Update();
    mUnk_10[0]->DrawModel();
    mUnk_10[1]->Update();
    mUnk_10[1]->DrawModel();
    if (mUnk_27) {
        mUnk_18->func_020526bc();
    }
    mUnk_0c->Draw();
}
