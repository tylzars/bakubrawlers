#include "Battle/BattleBase.hpp"
#include "Battle/BattleCommand.hpp"
#include "Battle/BattleMapTypes.hpp"
#include "Battle/BattleStatusPanels.hpp"
#include "Battle/GimmickTypes.hpp"
#include "Game/UnkStruct_020d433c.hpp"
#include "Graphics/Camera.hpp"
#include "Graphics/Effect.hpp"
#include "Graphics/Model.hpp"
#include "Graphics/SpriteManager.hpp"
#include <nitro/mi.h>
#include <nitro/os.h>

extern "C" void func_ov007_0226fca8(BattleUnk29c *unk);
extern "C" void func_ov007_0226fd78(BattleUnk29c *unk);
extern "C" void func_ov007_0226fc5c(BattleUnk29c *unk, u32 index);
extern "C" void func_ov007_0226fe4c(BattleUnk29c *unk);
extern "C" void func_ov007_02248748(BattleCommandMap *map, void *unk);
extern "C" void func_ov007_022476e0(BattleCommandMap *map);
extern "C" void func_ov007_02247898(BattleCommandMap *map);
extern "C" void func_ov007_0224796c(BattleCommandMap *map);
extern "C" void func_ov007_022479d4(BattleCommandMap *map, u32 a);
extern "C" void func_ov007_02247a1c(BattleCommandMap *map, u32 a);
extern const u8 data_ov007_02279efc[];
extern "C" void func_ov007_02246a6c(BattleCommandMap *map);
extern "C" void func_ov007_02246804(BattleCommandMap *map);
extern "C" void func_ov007_02246758(BattleCommandMap *map);
extern "C" void func_ov007_0224677c(BattleCommandMap *map);
extern "C" void func_ov007_02246730(BattleCommandMap *map);
extern "C" void func_ov007_02246578(BattleCommandMap *map);
extern "C" void func_ov007_02246558(BattleCommandMap *map);
void func_ov007_02233078(VecFx32 *a, VecFx32 *b, int c, VecFx32 *d);

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

BattleCommandMap::BattleCommandMap(u8 unk)
    : SceneBase(0x7d0, 0) {
    mUnk_155 = 0;
    mState   = 0;
    mUnk_14b = unk;
    mUnk_148 = 0;
    mUnk_149 = 0;
    mUnk_14c = 0;
    mUnk_14d = 0;
    mUnk_14f = 0;
    mUnk_150 = 0;
    mUnk_151 = 1;
    mUnk_152 = 0;
    mUnk_153 = 0;
    mUnk_154 = 0;
    mUnk_13c = 0;
    mUnk_13e = 0;
    mUnk_140 = 0;
    mUnk_142 = 0;
    mUnk_12c = 0;
    mUnk_144 = 0;
    mUnk_130 = 0;
    mUnk_134 = 0;
    MI_CpuClear32(mUnk_114, 0xc);
    MI_CpuClear32(mUnk_d8, 0xc);
    MI_CpuClear32(mUnk_108, 0xc);
    MI_CpuClear32(mUnk_e4, 0xc);
    MI_CpuClear32(mUnk_f0, 0xc);
    MI_CpuClear32(mUnk_fc, 0xc);
    MI_CpuClear32(mUnk_78, 0x60);
    u8 i = 0;
    do {
        mUnk_18[i] = 0;
        i++;
    } while (i < 12);
    mUnk_14 = 0;
    mUnk_48 = 0;
    mUnk_4c = 0;
    u8 j = 0;
    do {
        mUnk_50[j] = 0;
        j++;
    } while (j < 4);
    mUnk_10 = 0;
    mUnk_64 = 0;
    mUnk_68 = 0;
    mUnk_6c = 0;
    mUnk_70 = 0;
    mUnk_0c = 0;
    if (data_020d433c.mGimmicks) {
        data_020d433c.mGimmicks->LoadAll();
    }
    mUnk_74 = data_020d433c.mBattlePlayers->mUnk_29c;
    if (mUnk_74) {
        func_ov007_0226fca8(mUnk_74);
    }
    mUnk_60 = new UnkStruct_02029ca8(data_ov007_02279efc);
    func_ov007_02248748(this, mUnk_120);
}

BattleCommandMap::~BattleCommandMap() {
    u8 i = 0;
    do {
        func_ov007_0226fc5c(mUnk_74, i);
        i++;
    } while (i < 12);
    if (mUnk_14) {
        delete mUnk_14;
    }
    if (mUnk_48) {
        delete mUnk_48;
    }
    if (mUnk_4c) {
        delete mUnk_4c;
    }
    func_ov007_022455c0();
    if (mUnk_10) {
        delete mUnk_10;
    }
    if (mUnk_64) {
        delete mUnk_64;
    }
    if (mUnk_68) {
        delete mUnk_68;
    }
    if (mUnk_6c) {
        delete mUnk_6c;
    }
    if (mUnk_70) {
        delete mUnk_70;
    }
    func_ov007_02245594();
    if (data_020d433c.mGimmicks) {
        data_020d433c.mGimmicks->UnloadAll();
    }
    if (mUnk_74) {
        func_ov007_0226fd78(mUnk_74);
    }
    if (mUnk_60) {
        delete mUnk_60;
    }
}

void BattleCommandMap::func_ov007_02245594() {
    if (mUnk_0c) {
        OS_Free(mUnk_0c);
        mUnk_0c = 0;
    }
}

void BattleCommandMap::func_ov007_022455c0() {
    u8 i = 0;
    do {
        BattleMapModel *model = mUnk_50[i];
        if (model) {
            delete model;
        }
        mUnk_50[i] = 0;
        i++;
    } while (i < 4);
}

void BattleCommandMap::func_ov007_0224560c(u32 index) {
    BattleMapModel *model = mUnk_50[index];
    if (model == 0) {
        return;
    }
    if (model->mUnk_2c != 3) {
        return;
    }
    if (model) {
        delete model;
    }
    mUnk_50[index] = 0;
}

void func_ov007_0224587c(BattleCommandMap *map) {
    map->mState = 3;
}

void func_ov007_02245888(BattleCommandMap *map) {
    map->mState = 1;
}

void BattleCommandMap::func_ov007_02245894() {
    func_ov007_022479d4(this, 1);
    func_ov007_02247a1c(this, 1);
}

void BattleCommandMap::vfunc_0c() {
    func_ov007_02245894();
    func_ov007_022458cc();
}

void BattleCommandMap::func_ov007_022458cc() {
    func_ov007_022476e0(this);
    func_ov007_02247898(this);
    func_ov007_0224796c(this);
    func_ov007_0226fe4c(mUnk_74);
}

void BattleCommandMap::func_ov007_022458f4() {
    func_ov007_02245f08();
}

void BattleCommandMap::Update() {
    switch (mState) {
        case 0:
            func_ov007_02245b30();
            func_ov007_02245900();
            if (mUnk_14f != 0) {
                func_ov007_02245e58();
            }
            func_ov007_022458f4();
            func_ov007_022458cc();
            func_ov007_02246450();
            break;
        case 1:
            func_ov007_02246450();
            if (CameraManager::Get().mMode == 2) {
                SpriteManager::Get().StartFade(3);
            } else {
                SpriteManager::Get();
                SpriteManager::sFade.mMain = 3;
            }
            mState++;
            break;
        case 2:
            func_ov007_02246450();
            SpriteManager::Get();
            if (SpriteManager::sFade.mMain <= 1) {
                mState++;
            }
            break;
        case 3:
            RequestScene(1);
            mFinished = 1;
            break;
    }
}

void BattleCommandMap::func_ov007_02245900() {
    if (mUnk_0c == 0) {
        return;
    }
    Camera *camera = data_020d433c.mBattlePlayers->mCamera;
    if (camera == 0) {
        return;
    }
    camera->func_020264a4(0);
    camera->func_02026180();
    SnapshotCamera();
}

void BattleCommandMap::func_ov007_02245e58() {
    float step = (float) mUnk_12c / 8.0f;
    u8 i       = 0;
    do {
        func_ov007_02233078((VecFx32 *) mUnk_120, (VecFx32 *) mUnk_114, (u32) (mUnk_134 + (float) i * step), (VecFx32 *) &mUnk_78[i * 0xc]);
        i++;
    } while (i < 8);
    float limit = (float) mUnk_12c / 8.0f;
    mUnk_134    = mUnk_134 + limit / 30.0f;
    if (mUnk_134 >= limit) {
        mUnk_134 = 0;
    }
}

void BattleCommandMap::func_ov007_02246450() {
    func_ov007_02246a6c(this);
    func_ov007_02246804(this);
    func_ov007_02246758(this);
    if (mUnk_14f != 0) {
        func_ov007_0224677c(this);
    }
    func_ov007_02246730(this);
    data_020d433c.mEffects->func_0202bf94(1);
    BOOL show = TRUE;
    if (CameraManager::Get().mMode == 2) {
        if (CameraManager::Get().mActive != 0) {
            show = FALSE;
        }
    }
    if (show) {
        func_ov007_02246578(this);
        func_ov007_02246558(this);
    }
}
