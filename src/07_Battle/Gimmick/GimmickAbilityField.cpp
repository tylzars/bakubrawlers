#include "Battle/GimmickTypes.hpp"
#include <nitro/fx.h>
#include "System/FileSystem.hpp"
#include "Graphics/Camera.hpp"
#include "System/SoundManager.hpp"
static const u8 data_ov007_02279514[3] = {5, 7, 9};
static const u8 data_ov007_02279554[4] = {0x24, 0x25, 0x26, 0x27};
static ModelResource *sResources[2];
static const u8 sAbilityWeights01[1] = {255};
static const u8 sAbilityWeights03[2] = {32, 20};
static const u8 sAbilityWeights20[3] = {60, 20, 20};
static const u8 sAbilityWeights02[2] = {80, 20};
static const u8 sAbilityWeights23[2] = {80, 20};
static const u8 sAbilityWeights21[3] = {40, 30, 30};
static const u8 sAbilityIds01[1] = {0xcf};
static const u8 sAbilityIds28[4] = {0xce, 0xca, 0xcf, 0xcd};
static const u8 sAbilityWeights28[4] = {33, 33, 24, 10};
static const u8 sAbilityIds04[2] = {0xce, 0xd0};
static const u8 sAbilityIds22[4] = {0xce, 0xcb, 0xd4, 0xc9};
static const u8 sAbilityIds29[3] = {0xcb, 0xd3, 0xd1};
static const u8 sAbilityIds15[1] = {0xd1};
static const u8 sAbilityWeights22[4] = {25, 25, 25, 25};
static const u8 sAbilityWeights15[1] = {255};
static const u8 sAbilityIds25[2] = {0xca, 0xcb};
static const u8 sAbilityIds07[3] = {0xcb, 0xd1, 0xc9};
static const u8 sAbilityIds09[3] = {0xca, 0xc9, 0xcd};
static const u8 sAbilityWeights05[2] = {66, 34};
static const u8 sAbilityIds26[6] = {0xce, 0xcc, 0xd4, 0xd3, 0xd0, 0xc9};
static const u8 sAbilityWeights12[3] = {80, 16, 4};
static const u8 sAbilityWeights00[1] = {255};
static const u8 sAbilityIds13[3] = {0xd4, 0xce, 0xcd};
static const u8 sAbilityIds18[3] = {0xcc, 0xcb, 0xd1};
static const u8 sAbilityIds12[3] = {0xd3, 0xcf, 0xcd};
static const u8 sAbilityWeights18[3] = {60, 20, 20};
static const u8 sAbilityIds11[3] = {0xd0, 0xd2, 0xca};
static const u8 sAbilityWeights06[1] = {255};
static const u8 sAbilityIds17[2] = {0xcf, 0xcd};
static const u8 sAbilityIds19[3] = {0xd0, 0xd4, 0xd2};
static const u8 sAbilityWeights19[3] = {60, 20, 20};
static const u8 sAbilityWeights04[2] = {80, 20};
static const u8 sAbilityIds08[3] = {0xcc, 0xcf, 0xd2};
static const u8 sAbilityIds21[3] = {0xcc, 0xd0, 0xd4};
static const u8 sAbilityWeights07[3] = {60, 32, 8};
static const u8 sAbilityIds00[1] = {0xcb};
static const u8 sAbilityWeights09[3] = {80, 16, 4};
static const u8 sAbilityWeights10[3] = {80, 16, 4};
static const u8 sAbilityWeights11[3] = {60, 16, 24};
static const u8 sAbilityWeights25[2] = {50, 50};
static const u8 sAbilityIds14[4] = {0xc9, 0xcd, 0xd1, 0xcf};
static const u8 sAbilityWeights17[2] = {50, 50};
static const u8 sAbilityWeights14[4] = {60, 20, 15, 5};
static const u8 sAbilityIds16[1] = {0xd3};
static const u8 sAbilityIds27[4] = {0xd3, 0xd4, 0xd1, 0xcf};
static const u8 sAbilityWeights16[1] = {255};
static const u8 sAbilityWeights27[4] = {33, 33, 24, 10};
static const u8 sAbilityIds20[3] = {0xca, 0xd3, 0xc9};
static const u8 sAbilityWeights08[3] = {60, 16, 24};
static const u8 sAbilityWeights24[2] = {80, 20};
static const u8 sAbilityWeights29[3] = {34, 33, 33};
static const u8 sAbilityIds05[2] = {0xd4, 0xc9};
static const u8 sAbilityIds02[2] = {0xca, 0xd1};
static const u8 sAbilityWeights26[6] = {20, 20, 20, 16, 12, 12};
static const u8 sAbilityIds03[2] = {0xd3, 0xd2};
static const u8 sAbilityIds23[2] = {0xd1, 0xc9};
static const u8 sAbilityIds06[1] = {0xc9};
static const u8 sAbilityWeights13[3] = {60, 36, 4};
static const u8 sAbilityIds10[3] = {0xce, 0xd2, 0xcd};
static const u8 sAbilityIds24[2] = {0xd4, 0xcf};
const GimmickAbilityRow data_ov007_02279568[] = {
    {1, {}, sAbilityIds00, sAbilityWeights00},
    {1, {}, sAbilityIds01, sAbilityWeights01},
    {2, {}, sAbilityIds02, sAbilityWeights02},
    {2, {}, sAbilityIds03, sAbilityWeights03},
    {2, {}, sAbilityIds04, sAbilityWeights04},
    {2, {}, sAbilityIds05, sAbilityWeights05},
    {1, {}, sAbilityIds06, sAbilityWeights06},
    {3, {}, sAbilityIds07, sAbilityWeights07},
    {3, {}, sAbilityIds08, sAbilityWeights08},
    {3, {}, sAbilityIds09, sAbilityWeights09},
    {3, {}, sAbilityIds10, sAbilityWeights10},
    {3, {}, sAbilityIds11, sAbilityWeights11},
    {3, {}, sAbilityIds12, sAbilityWeights12},
    {3, {}, sAbilityIds13, sAbilityWeights13},
    {4, {}, sAbilityIds14, sAbilityWeights14},
    {1, {}, sAbilityIds15, sAbilityWeights15},
    {1, {}, sAbilityIds16, sAbilityWeights16},
    {2, {}, sAbilityIds17, sAbilityWeights17},
    {3, {}, sAbilityIds18, sAbilityWeights18},
    {3, {}, sAbilityIds19, sAbilityWeights19},
    {3, {}, sAbilityIds20, sAbilityWeights20},
    {3, {}, sAbilityIds21, sAbilityWeights21},
    {4, {}, sAbilityIds22, sAbilityWeights22},
    {2, {}, sAbilityIds23, sAbilityWeights23},
    {2, {}, sAbilityIds24, sAbilityWeights24},
    {2, {}, sAbilityIds25, sAbilityWeights25},
    {6, {}, sAbilityIds26, sAbilityWeights26},
    {4, {}, sAbilityIds27, sAbilityWeights27},
    {4, {}, sAbilityIds28, sAbilityWeights28},
    {3, {}, sAbilityIds29, sAbilityWeights29},
};

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
            VecFx32 camPos = data_020d433c.mBattlePlayers->mUnk_298->mUnk_1a4->mPosition;
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
