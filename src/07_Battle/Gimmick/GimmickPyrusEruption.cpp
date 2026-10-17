#include "Battle/GimmickTypes.hpp"
#include <nitro/fx.h>
#include "Graphics/Camera.hpp"
#include "System/FileSystem.hpp"
#include "System/SoundManager.hpp"

static ModelResource *sResources[2];

GimmickPyrusEruption::GimmickPyrusEruption(u8 id)
    : Gimmick(id, 0) {
    mModelB  = 0;
    mCounter = 0;
    mPushed  = 0;
    mMarkerNear  = 0;
    mState   = 0;
    mType  = GIMMICK_TYPE_PYRUS_ERUPTION;
}

GimmickPyrusEruption::~GimmickPyrusEruption() {
    Unload();
}

void GimmickPyrusEruption::Load() {
    mMarkerNear = IsNearMarker(0xa000);
    if (mMarkerNear) {
        return;
    }
    if (!sResources[0]) {
        sResources[0] = new ModelResource("Pyruseruption_n.nsbmd", "Pyruseruption_n_data.bin", "Pyruseruption_n.nsbtx", 1);
        sResources[1] = new ModelResource("Pyruseruption_b.nsbmd", "Pyruseruption_b_data.bin", "Pyruseruption_b.nsbtx", 1);
    }
    ChangeDir("/Game/Gimmick");
    sResources[0]->AddRef(1);
    mModel = sResources[0]->mModel->CreateInstance(new ModelAnim(sResources[0]->mAnimData, sResources[0]->mModel));
    mModel->StartAnimWithIncrement(0, 1, 0x1000);
    mAnimEnabled = 1;
    sResources[1]->AddRef(1);
    mModelB = sResources[1]->mModel->CreateInstance(new ModelAnim(sResources[1]->mAnimData, sResources[1]->mModel));
    mModelB->StartAnimWithIncrement(0, 0, 0x1000);
    ChangeDir("/");
}

void GimmickPyrusEruption::Unload() {
    if (mModel) {
        ModelInstance *m = mModel;
        m->mOwnFlags       = 1;
        delete m;
        mModel = 0;
        ModelInstance *b = mModelB;
        b->mOwnFlags       = 1;
        delete b;
        mModelB = 0;
    }
    if (sResources[0]) {
        sResources[1]->ReleaseRef();
        if (sResources[0]->ReleaseRef()) {
            ModelResource *r = sResources[0];
            delete r;
            sResources[0]    = 0;
            r                = sResources[1];
            delete r;
            sResources[1] = 0;
        }
    }
}

void GimmickPyrusEruption::Finish() {
    mState = 5;
}

void GimmickPyrusEruption::Update() {
    VecFx32 d;
    switch (mState) {
    case 0:
        LoadCollision(0x1e, 8);
        mState++;
        break;
    case 1:
        if (mMarkerNear) {
            return;
        }
        DrawModels(0);
        if (!(mCollision && mCollision->mHitCount)) {
            return;
        }
        mCounter = 0xf;
        mState++;
        break;
    case 2:
        if (mMarkerNear) {
            return;
        }
        DrawModels(0);
        if (!CountDownToState(0x1e, (u8)(mState + 1))) {
            return;
        }
        SoundManager::Get().StartSeqArc(0x62);
        break;
    case 3: {
        if (mMarkerNear) {
            return;
        }
        DrawModels(1);
        if (CountDownToState(0x14, (u8)(mState + 1))) {
            return;
        }
        if (!mCollision) {
            return;
        }
        if (mCollision->mHitCount != 2) {
            return;
        }
        BattleTarget *target = data_020d433c.mBattlePlayers->mBattleTarget;
        if (!target) {
            return;
        }
        if (mPushed) {
            return;
        }
        if (target->mUnk_44 > 0x5dc) {
            return;
        }
        BattlePlayer *p = data_020d433c.mBattlePlayers->Current();
        if (p->mRecords[p->mRecord].mAttribute == 0) {
            return;
        }
        VEC_Subtract(&mPosition, &target->mBakugan->mPosition, &d);
        fx32 len = VEC_Mag(&d);
        VEC_Normalize(&d, &d);
        d.x      = FX_Mul(d.x, 0x1e000);
        d.y      = FX_Mul(d.y, 0x1e000);
        d.z      = FX_Mul(d.z, 0x1e000);
        fx32 z   = -FX_Div(d.z, len);
        fx32 y   = -FX_Div(d.y, len);
        d.x      = -FX_Div(d.x, len);
        d.y      = y;
        d.z      = z;
        VEC_Add(&data_020d433c.mBattlePlayers->mBattleTarget->mUnk_a0, &d, &data_020d433c.mBattlePlayers->mBattleTarget->mUnk_a0);
        mState++;
        mPushed = 1;
        break;
    }
    case 4:
        if (mMarkerNear) {
            return;
        }
        DrawModels(1);
        if (!CountDownToState(0, 1)) {
            return;
        }
        mPushed = 0;
        if (!mModelB) {
            return;
        }
        mModelB->StartAnimWithIncrement(0, 0, 0x1000);
        break;
    case 5:
        mFinished = 1;
        break;
    }
}

void GimmickPyrusEruption::DrawModelB() {
    if (!mModelB) {
        return;
    }
    MtxFx43 mtx = gIdentityMtx43;
    func_01ffcf64(&mtx, &mPosition);
    data_020d433c.mUnk_14 &= 8;
    if (!data_020d433c.mUnk_14) {
        mModelB->AdvanceAnim();
    }
    mModelB->Draw(&mtx, &mScale);
}

void GimmickPyrusEruption::DrawModels(int drawB) {
    if (!mModel) {
        return;
    }
    if ((int)data_020d433c.mGimmicks->mState < 2) {
        return;
    }
    func_020526bc();
    if (!drawB) {
        return;
    }
    DrawModelB();
}

int GimmickPyrusEruption::CountDownToState(int frames, int state) {
    data_020d433c.mUnk_14 &= 8;
    if (!data_020d433c.mUnk_14) {
        mCounter--;
        if (mCounter == 0) {
            mCounter = frames;
            mState   = state;
            return 1;
        }
    }
    return 0;
}
