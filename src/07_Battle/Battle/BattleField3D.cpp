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
extern u8 data_ov007_022791c4[];
extern VecFx32 data_ov007_022791ac;
extern VecFx32 data_ov007_022791b8;
extern VecFx32 data_ov007_022791a0;
void func_01ffd0e0(MtxFx43 *mtx, u16 angle);
void func_ov007_02229998(BattleField3D *obj);

// Effect (particle) system functions: effects are loaded by name into numbered slots, spawned as emitters addressed by
// a 16-bit handle, and moved by handle.
void func_0202ba64(EffectManager *manager, u32 slot, const char *name);
void func_0202bc58(EffectManager *manager, u32 slot);
u16 func_0202bd40(EffectManager *manager, u32 a, u32 b, u32 c, u32 d);
void func_0202beb8(EffectManager *manager, u16 handle);
void func_0202bef4(EffectManager *manager, u16 handle, VecFx32 *position);

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
    mUnk_0c = 0;
    mUnk_10 = 0;
    mUnk_14 = 0;
    mUnk_18 = 0;
    mUnk_34 = 10240.0f;
    mUnk_38 = 0;
    mUnk_3c = 0;
    mCamFile = 0;
    mEffects = data_020d433c.mEffects;
    mData = data;
    Camera *camera = data_020d433c.mBattlePlayers->mCamera;
    mUnk_38 = new BattleMapObj(0xd);
    mUnk_38->func_ov007_022493fc();
    camera->func_02026004(0x1555);
    CameraManager::Get().func_02026504(1, camera, 0);
    ChangeDir("/Game/Battle/3d/MapCam");
    mCamFile = func_02023504("commandfiled0.cam", 0, 0, 0, 0, 0);
    ChangeDir("/");
    camera->func_0202610c(mCamFile, 0);
    camera->func_02026180();
    SnapshotCamera();
    mUnk_3c = new UnkStruct_02029ca8(data_ov007_022791c4);
    BattleEntry *entries = data->mEntries;
    mUnk_48 = new BattleRealBaku(entries[0].mUnk_0c, 0xb, entries[0].mUnk_0d_0, 1, 0, 0, entries[0].mUnk_0e == 0x15);
    mUnk_4c = new BattleRealBaku(entries[1].mUnk_0c, 0xb, entries[1].mUnk_0d_0, 1, 0, 0, entries[1].mUnk_0e == 0x15);
    BattleCamRow *row = &data_ov007_022791ec[entries[0].mUnk_0c];
    s32 z0 = row->mUnk_08;
    s32 y0 = row->mUnk_04;
    mPos[0].x = -0x1a000;
    mPos[0].y = y0;
    mPos[0].z = z0;
    mUnk_0c = row->mUnk_00;
    row = &data_ov007_022791ec[entries[1].mUnk_0c];
    s32 z1 = row->mUnk_08;
    s32 y1 = row->mUnk_04;
    mPos[1].x = 0x1a000;
    mPos[1].y = y1;
    mPos[1].z = z1;
    mUnk_10 = -row->mUnk_00;
    char name[0x30];
    sprintf(name, "aura%02d", entries[0].mUnk_0d_0);
    func_0202ba64(mEffects, 0, name);
    sprintf(name, "aura%02d", entries[1].mUnk_0d_0);
    func_0202ba64(mEffects, 1, name);
    mUnk_14 = -1556;
    switch (unk2) {
        case 1:
            mPos[0].x = mUnk_0c;
            mPos[1].x = mUnk_10;
            mUnk_04_1 = 0;
            mUnk_04_2 = 0;
            func_ov007_02229900();
            mUnk_04_0 = 1;
        case 2:
            camera->func_02026164(camera->mUnk_50);
            camera->func_02026180();
            SnapshotCamera();
            break;
    }
}

BattleField3D::~BattleField3D() {
    func_ov007_02229998(this);
    func_0202bc58(mEffects, 1);
    func_0202bc58(mEffects, 0);
    if (mUnk_4c) {
        delete mUnk_4c;
    }
    if (mUnk_48) {
        delete mUnk_48;
    }
    UnkStruct_02029ca8 *palette = mUnk_3c;
    if (palette) {
        delete palette;
    }
    OS_Free(mCamFile);
    BattleMapObj *map = mUnk_38;
    if (map) {
        delete map;
    }
    SpriteManager::Get().func_0202740c();
}

// The caller passes its BattleField3D, which the function doesn't use.
BOOL func_ov007_02229524(BattleField3D *field) {
    Camera *camera = data_020d433c.mBattlePlayers->mCamera;
    camera->func_020264a4(0);
    camera->func_02026180();
    SnapshotCamera();
    return camera->mUnk_52 + 1 >= camera->mUnk_50;
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
    if (obj->mPos[0].x >= obj->mUnk_0c) {
        obj->mPos[0].x = obj->mUnk_0c;
        reached = 1;
    }
    if (obj->mPos[1].x <= obj->mUnk_10) {
        if (reached) {
            done = 1;
        }
        obj->mPos[1].x = obj->mUnk_10;
    }
    if (!done) {
        return 0;
    }
    obj->func_ov007_02229900();
    obj->mUnk_04_0 = 1;
    return 1;
}

BOOL func_ov007_022297f0(BattleField3D *obj) {
    BattleData *data = obj->mData;
    if (data->mEntries[0].mUnk_10) {
        float a = 0.0f - obj->mUnk_34 / 3.0f;
        obj->func_ov007_022298bc(a, 0.0f - obj->mUnk_34);
    } else {
        obj->func_ov007_022298bc(obj->mUnk_34, obj->mUnk_34 / 3.0f);
    }
    obj->mUnk_34 -= 1229.0f;
    if (obj->mUnk_34 < -4096.0f) {
        obj->mUnk_34 = 0;
    }
    if (obj->mUnk_34 == 0.0f) {
        if (data->mEntries[0].mUnk_10) {
            obj->mUnk_04_2 = 1;
        } else {
            obj->mUnk_04_1 = 1;
        }
        return 1;
    }
    return 0;
}

void BattleField3D::func_ov007_022298bc(float a, float b) {
    mPos[0].x = (s32) ((float) mPos[0].x - a);
    mPos[1].x = (s32) ((float) mPos[1].x - b);
}

void BattleField3D::func_ov007_02229900() {
    mAura[0] = func_0202bd40(mEffects, 0, 0, 0, 0);
    mAura[1] = func_0202bd40(mEffects, 1, 1, 0, 0);
    VecFx32 pos = data_ov007_022791ac;
    pos.x = mUnk_14;
    func_0202bef4(mEffects, mAura[0], &pos);
    pos.x = mUnk_14 + 0xccd;
    func_0202bef4(mEffects, mAura[1], &pos);
}

void func_ov007_02229998(BattleField3D *obj) {
    if (obj->mAura[0] != -1) {
        func_0202beb8(obj->mEffects, obj->mAura[0]);
        obj->mAura[0] = -1;
    }
    if (obj->mAura[1] != -1) {
        func_0202beb8(obj->mEffects, obj->mAura[1]);
        obj->mAura[1] = -1;
    }
}

// non-matching: delta in r3 instead of r2, and one load register.
void BattleField3D::func_ov007_022299f4(u32 frame) {
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
    s32 delta = target - mUnk_18;
    mPos[0].x += delta;
    mPos[1].x += delta;
    mUnk_14 += delta / 4;
    VecFx32 pos = data_ov007_022791b8;
    pos.x = mUnk_14;
    if (mAura[0] != -1) {
        func_0202bef4(mEffects, mAura[0], &pos);
    }
    if (mAura[1] != -1) {
        pos.x = mUnk_14 + 0xccd;
        func_0202bef4(mEffects, mAura[1], &pos);
    }
    mUnk_18 = target;
}

void BattleField3D::func_ov007_02229b2c(u32 frame) {
    VecFx32 scale = data_ov007_022791a0;
    MtxFx43 mtx = data_0220e86c;
    if (mUnk_04_0) {
        func_ov007_022299f4(frame);
    }
    if (!mUnk_04_1) {
        func_01ffcf64(&mtx, &mPos[0]);
        func_01ffd0e0(&mtx, 0x3fff);
        mUnk_48->mModel->func_02028d80();
        mUnk_48->mModel->func_02029174(&mtx, &scale);
    }
    if (!mUnk_04_2) {
        mtx = data_0220e86c;
        func_01ffcf64(&mtx, &mPos[1]);
        func_01ffd0e0(&mtx, 0xc001);
        mUnk_4c->mModel->func_02028d80();
        mUnk_4c->mModel->func_02029174(&mtx, &scale);
    }
    mUnk_38->func_ov007_0224954c();
}
