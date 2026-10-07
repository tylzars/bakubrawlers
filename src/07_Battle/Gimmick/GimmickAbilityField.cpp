#include "Battle/GimmickTypes.hpp"
#include <nitro/fx.h>
#include "Graphics/Camera.hpp"
#include "System/FileSystem.hpp"
#include "System/SoundManager.hpp"

static const u8 data_ov007_02279514[4] = {5, 7, 9, 0};
static const u8 data_ov007_02279554[4] = {0x24, 0x25, 0x26, 0x27};

static ModelResource *sResources[2];

GimmickAbilityField::GimmickAbilityField(u8 id, u8 param)
    : Gimmick(id, 0) {
    mModelB = 0;
    mState  = 0;
    mUnk_4c = param;
    mUnk_3a = 9;
}

GimmickAbilityField::~GimmickAbilityField() {
    Unload();
}

void GimmickAbilityField::Load() {
    if (!sResources[0]) {
        sResources[0] = new ModelResource("ability_filed0.nsbmd", "ability_filed0_data.bin", "ability_filed0.nsbtx", 1);
        sResources[1] = new ModelResource("ability_filed1.nsbmd", 0, "ability_filed1.nsbtx", 1);
    }
    ChangeDir("/Game/Gimmick");
    mUnk_4d = sResources[0]->func_020306e4(1);
    if (mUnk_4d >= 4) {
        mUnk_4d = 3;
    }
    mModel = sResources[0]->mModel->func_02028b78(new ModelAnim(sResources[0]->mAnimData, sResources[0]->mModel));
    mModel->func_02028e54(0, 1, 0x1000);
    mUnk_00 = 1;
    sResources[1]->func_020306e4(1);
    mModelB = sResources[1]->mModel->func_02028b78(0);
    func_020331a4(mModelB->mUnk_58->mUnk_04, 0, 0x3f000000);
    ChangeDir("/");
}

void GimmickAbilityField::Unload() {
    if (mModel) {
        ModelInstance *m = mModel;
        m->mUnk_68       = 1;
        delete m;
        mModel = 0;
        ModelInstance *b = mModelB;
        b->mUnk_68       = 1;
        delete b;
        mModelB = 0;
    }
    if (sResources[0]) {
        sResources[1]->func_020307e8();
        if (sResources[0]->func_020307e8()) {
            ModelResource *r = sResources[1];
            delete r;
            sResources[1] = 0;
            r             = sResources[0];
            delete r;
            sResources[0] = 0;
        }
    }
}

void GimmickAbilityField::Finish() {
    mState = 3;
}

void GimmickAbilityField::func_ov007_022303f4() {
    VecFx32 v;
    Camera *cam = CameraManager::Get().mCameras[0];
    VecFx32 pos = mPosition;
    CameraManager::Get();
    MtxFx43 mtx = data_020c1cf4;
    func_01ffcf64(&mtx, &mPosition);
    v = cam->mPosition;
    VEC_Subtract(&v, &pos, &v);
    VEC_Normalize(&v, &v);
    v.x = FX_Mul(v.x, 0xa000);
    v.y = FX_Mul(v.y, 0xa000);
    v.z = FX_Mul(v.z, 0xa000);
    VEC_Add(&v, &mPosition, &v);
    func_01ffcf64(&mtx, &v);
    data_02211374.mUnk_9c = 0x1f00c0 | (data_ov007_02279554[mUnk_4d] << 24);
    mModelB->func_02029174(&mtx, &mScale);
}

void GimmickAbilityField::Update() {
    switch (mState) {
    case 0:
        func_ov007_0222ea28(0x60, 0x20);
        mState++;
        break;
    case 1: {
        if (mModel && (int)data_020d433c.mGimmicks->mState >= 2) {
            func_020526bc();
            func_ov007_022303f4();
        }
        if (!(mUnk_3c && mUnk_3c->mUnk_823)) {
            return;
        }
        mState++;
        func_ov007_02271d78(data_020d433c.mUnk_0c, mUnk_3c);
        SoundManager::Get().func_020246f8(0x35);
        s8 pick = func_02021a30(data_ov007_02279568[mUnk_4c].mCount, data_ov007_02279568[mUnk_4c].mWeights);
        data_020d433c.mBattlePlayers->Current()->mUnk_fa = data_ov007_02279568[mUnk_4c].mIds[pick];
        u8 handle = func_0202bd40(data_020d433c.mEffects, 1, 0x12, 0, 1);
        if (handle) {
            CameraManager::Get();
            MtxFx43 mtx    = data_020c1cf4;
            VecFx32 camPos = data_020d433c.mBattlePlayers->mUnk_298->mUnk_1a4->mPos;
            mtx._[11]      = 0;
            mtx._[10]      = 0;
            mtx._[9]       = 0;
            func_01ffcf64(&mtx, &camPos);
            func_0202bf60(data_020d433c.mEffects, handle, &mtx);
        }
        mUnk_40 = 1;
        mUnk_3c = 0;
        break;
    }
    case 2: {
        u8 players = data_020d433c.mPlayerCount;
        if (players < 2 || players > 4) {
            break;
        }
        u32 limit;
        if (data_020d433c.mUnk_98) {
            limit = 3;
        } else {
            limit = data_ov007_02279514[players - 2];
        }
        if (mUnk_44 < limit) {
            break;
        }
        func_ov007_0222ea28(0x60, 0x20);
        mUnk_40 = 0;
        mState  = 1;
        mUnk_44 = 0;
        break;
    }
    case 3:
        mFinished = 1;
        break;
    }
}
