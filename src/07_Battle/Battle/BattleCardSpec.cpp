#include "System/InputManager.hpp"
#include "Battle/BattleCardSpec.hpp"
#include "Game/UnkStruct_020d433c.hpp"
#include "Graphics/Camera.hpp"
#include "Graphics/Model.hpp"
#include "Graphics/SpriteManager.hpp"
#include "Scene/SceneBase.hpp"
#include "System/FileSystem.hpp"
#include "System/InputManager.hpp"
#include "System/WirelessManager.hpp"
#include <nitro/fx.h>
#include <nitro/gx.h>
#include <stdio.h>

BOOL func_ov007_022632d0(BattlePlayerList *list, u32 flag);
BOOL func_ov007_022631bc(BattlePlayerList *list, u32 id);
extern const VecFx32 data_ov007_0227aa1c;


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

BattleCardSpec::BattleCardSpec(u32 a, u32 b, u32 c, u8 d)
    : SceneBase(0x3e8, 0) {
    mUnk_0c = 0;
    mUnk_10 = 0;
    mUnk_14 = 0;
    mState  = 0;
    mUnk_19 = 0;
    mUnk_1e = 0;
    mUnk_1d = d;
    mUnk_1a = a;
    mUnk_1b = b;
    mUnk_1c = c;
    SpriteManager::Get().func_0202740c();
    switch (a) {
        case 0:
        case 1:
        case 2:
        case 4:
            mUnk_19 = 1;
            mUnk_1e = 1;
            break;
    }
}

BattleCardSpec::~BattleCardSpec() {
    ModelInstance *a = mUnk_10;
    if (a) {
        a->mOwnFlags = 0xf;
        delete a;
    }
    ModelInstance *b = mUnk_14;
    if (b) {
        b->mOwnFlags = 0xf;
        delete b;
    }
    SpriteManager::Get().func_0202740c();
}

void BattleCardSpec::Update() {
    switch (mState) {
        case 0: {
            CameraManager::Get().mActive = 0;
            CameraManager::Get().SetCamerasAndRequestMode(1, data_020d433c.mBattlePlayers->mCamera, 0);
            GX_SetVisiblePlane(0x11);
            G2_SetBG0Priority(0);
            func_ov007_0225fbdc();
            mUnk_0c = new BattleCardModel(data_020d433c.mEffects, data_020d433c.mBattlePlayers->mCamera, mUnk_1a, mUnk_1b, mUnk_1c, mUnk_1d);
            mContext->mUnk_12 = 0;
            mState++;
            break;
        }
        case 1:
            mState++;
            break;
        case 2:
            if (mContext->mUnk_12 != 0) {
                mState = 5;
            } else if (mUnk_19) {
                if (func_ov007_022632d0(data_020d433c.mBattlePlayers, 0)) {
                    mState = 4;
                } else if (InputManager::Get().IsDown(8)) {
                    mState++;
                }
            }
            break;
        case 3:
            WirelessManager::Get();
            if (IsWirelessMode(&data_020c4030.mUnk_228)) {
                if (func_ov007_022632d0(data_020d433c.mBattlePlayers, 1)) {
                    mState++;
                }
            } else {
                mState++;
            }
            break;
        case 4:
            func_ov007_02273e3c(mUnk_0c);
            mState++;
            break;
        case 5:
            SpriteManager::Get();
            if (SpriteManager::sFade[0] <= 1) {
                mState++;
            }
            break;
        case 6:
            if (mUnk_1e) {
                if (!func_ov007_022631bc(data_020d433c.mBattlePlayers, 0x11)) {
                    return;
                }
                RequestScene(1);
                mFinished = 1;
            } else {
                RequestScene(1);
                mFinished = 1;
            }
            return;
    }
    if (mState > 1) {
        SnapshotCamera();
        MtxFx43 mtx   = gIdentityMtx43;
        VecFx32 pos   = data_ov007_0227aa1c;
        mUnk_10->Draw(&mtx, &pos);
        mUnk_14->AdvanceAnim();
        mUnk_14->Draw(&mtx, &pos);
    }
}

void BattleCardSpec::func_ov007_0225fbdc() {
    ChangeDir("/Game/Battle/3d/Map/");
    TextureFile *texture = new NitroTexture("realbase_00.nsbtx");
    NitroModel *model           = new NitroModel("realbase_00.nsbmd", texture);
    mUnk_10                     = model->CreateInstance(NULL);
    mUnk_10->mUseBoxTest            = 0;
    texture->FreeFile();
    TextureFile *texture2 = new NitroTexture("realnormal_99.nsbtx");
    NitroModel *model2           = new NitroModel("realnormal_99.nsbmd", texture2);
    texture2->FreeFile();
    ModelAnimData *data = new ModelAnimData("realnormal_99_data.bin");
    ModelAnim *anim     = new ModelAnim(data, model2);
    mUnk_14             = model2->CreateInstance(anim);
    mUnk_14->StartAnim(0, 1);
    mUnk_14->mUseBoxTest = 0;
    ChangeDir("/");
}
