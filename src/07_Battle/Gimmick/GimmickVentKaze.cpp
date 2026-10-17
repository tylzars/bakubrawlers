#include "Battle/GimmickTypes.hpp"
#include <nitro/fx.h>
#include "System/FileSystem.hpp"

extern "C" void func_01ffcfd8(MtxFx33 *m, u16 angle);
extern MtxFx33 data_0220e848;

static const VecFx32 data_ov007_02279450 = {0, 0, -0x14000};

static ModelResource *sResource;

GimmickVentKaze::GimmickVentKaze(u8 id)
    : Gimmick(id, 0) {
    mState  = 0;
    mType = GIMMICK_TYPE_VENT_KAZE;
}

GimmickVentKaze::~GimmickVentKaze() {
    Unload();
}

void GimmickVentKaze::Load() {
    if (!sResource) {
        sResource = new ModelResource("ventkaze0.nsbmd", "ventkaze0_data.bin", "ventkaze0.nsbtx", 1);
    }
    ChangeDir("/Game/Gimmick");
    sResource->AddRef(1);
    mModel = sResource->mModel->CreateInstance(new ModelAnim(sResource->mAnimData, sResource->mModel));
    mModel->StartAnimWithIncrement(0, 1, 0x1000);
    mAnimEnabled = 1;
    ChangeDir("/");
}

void GimmickVentKaze::Unload() {
    if (mModel) {
        ModelInstance *m = mModel;
        m->mOwnFlags       = 1;
        delete m;
        mModel = 0;
    }
    if (sResource && sResource->ReleaseRef()) {
        ModelResource *r = sResource;
        delete r;
        sResource = 0;
    }
}

void GimmickVentKaze::Finish() {
    mState = 2;
}

void GimmickVentKaze::Update() {
    VecFx32 c;
    VecFx32 b;
    MtxFx33 m = data_0220e848;
    switch (mState) {
    case 0:
        LoadCollision(0x34, 8);
        mState++;
        break;
    case 1: {
        if (mModel && (int)data_020d433c.mGimmicks->mState >= 1) {
            func_020526bc();
        }
        if (!(mCollision && mCollision->mHitCount)) {
            return;
        }
        if (!data_020d433c.mBattlePlayers->mBattleTarget) {
            return;
        }
        BattlePlayer *p = data_020d433c.mBattlePlayers->Current();
        BattleRecord *r = &p->mRecords[p->mRecord];
        data_020d433c.mUnk_14 &= 8;
        if (data_020d433c.mUnk_14) {
            return;
        }
        if (r->mAttribute == 5) {
            return;
        }
        BakuganModel *h = data_020d433c.mBattlePlayers->mBattleTarget->mBakugan;
        VecFx32 a           = data_ov007_02279450;
        m                   = data_0220e848;
        if (mRotY) {
            func_01ffcfd8(&m, mRotY);
        }
        func_01ff84d0(&a, &m, &a);
        VEC_Add(&a, &mPosition, &a);
        VEC_Subtract(&a, &h->mPosition, &b);
        fx32 z = FX_Div(0x2800, -b.y);
        fx32 y = FX_Div(0x8000, -b.y);
        fx32 x = FX_Div(0, -b.y);
        c.x    = x;
        c.y    = y;
        c.z    = z;
        m   = data_0220e848;
        if (mRotY) {
            func_01ffcfd8(&m, mRotY);
        }
        func_01ff84d0(&c, &m, &c);
        VEC_Add(&data_020d433c.mBattlePlayers->mBattleTarget->mUnk_a0, &c, &data_020d433c.mBattlePlayers->mBattleTarget->mUnk_a0);
        break;
    }
    case 2:
        mFinished = 1;
        break;
    }
}
