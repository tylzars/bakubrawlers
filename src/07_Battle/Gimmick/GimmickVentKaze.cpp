#include "Battle/GimmickTypes.hpp"
#include <nitro/fx.h>
#include "System/FileSystem.hpp"

extern "C" void MTX_ApplyRotY33(MtxFx33 *m, u16 angle);
extern MtxFx33 gIdentityMtx33;

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
        m->mOwnFlags       = MODEL_OWN_ANIM;
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
    MtxFx33 m = gIdentityMtx33;
    switch (mState) {
    case 0:
        LoadCollision(0x34, COLLISION_GROUP_GIMMICK);
        mState++;
        break;
    case 1: {
        if (mModel && (int)gGameData.mGimmicks->mState >= 1) {
            DrawInstance();
        }
        if (!(mCollision && mCollision->mHitCount)) {
            return;
        }
        if (!gGameData.mBattlePlayers->mBattleTarget) {
            return;
        }
        BattlePlayer *p = gGameData.mBattlePlayers->Current();
        BattleRecord *r = &p->mRecords[p->mRecord];
        gGameData.mFlags &= 8;
        if (gGameData.mFlags) {
            return;
        }
        if (r->mAttribute == 5) {
            return;
        }
        BakuganModel *h = gGameData.mBattlePlayers->mBattleTarget->mBakugan;
        VecFx32 a           = data_ov007_02279450;
        m                   = gIdentityMtx33;
        if (mRotY) {
            MTX_ApplyRotY33(&m, mRotY);
        }
        MTX_MultVec33(&a, &m, &a);
        VEC_Add(&a, &mPosition, &a);
        VEC_Subtract(&a, &h->mPosition, &b);
        fx32 z = FX_Div(0x2800, -b.y);
        fx32 y = FX_Div(0x8000, -b.y);
        fx32 x = FX_Div(0, -b.y);
        c.x    = x;
        c.y    = y;
        c.z    = z;
        m   = gIdentityMtx33;
        if (mRotY) {
            MTX_ApplyRotY33(&m, mRotY);
        }
        MTX_MultVec33(&c, &m, &c);
        VEC_Add(&gGameData.mBattlePlayers->mBattleTarget->mBaseVelocity, &c, &gGameData.mBattlePlayers->mBattleTarget->mBaseVelocity);
        break;
    }
    case 2:
        mFinished = 1;
        break;
    }
}
