#include "Battle/GimmickTypes.hpp"
#include <nitro/fx.h>
#include "Game/Random.hpp"
#include "System/FileSystem.hpp"
#include "System/SoundManager.hpp"

static ModelResource *sResource;

GimmickHaosBall::GimmickHaosBall(u8 a)
    : Gimmick(a, 0) {
    mTimer  = 0;
    mState  = 0;
    mType = GIMMICK_TYPE_HAOS_BALL;
}

GimmickHaosBall::~GimmickHaosBall() {
    Unload();
}

void GimmickHaosBall::Load() {
    if (!sResource) {
        sResource = new ModelResource("Haosball.nsbmd", "Haosball_data.bin", "Haosball.nsbtx", 1);
    }
    ChangeDir("/Game/Gimmick");
    sResource->AddRef(1);
    mModel = sResource->mModel->CreateInstance(new ModelAnim(sResource->mAnimData, sResource->mModel));
    mModel->StartAnimWithIncrement(0, 1, 0x1000);
    mAnimEnabled = 1;
    ChangeDir("/");
}

void GimmickHaosBall::Unload() {
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

void GimmickHaosBall::Finish() {
    mState = 5;
}

void GimmickHaosBall::Update() {
    VecFx32 rnd;
    VecFx32 d;
    switch (mState) {
    case 0:
        LoadCollision(0x46, COLLISION_GROUP_GIMMICK);
        mState++;
        break;
    case 1: {
        DrawModel();
        if (!(mCollision && mCollision->mHitCount)) {
            return;
        }
        BattlePlayer *p = data_020d433c.mBattlePlayers->Current();
        if (p->mRecords[p->mRecord].mAttribute == 3) {
            return;
        }
        mTimer = 0x1e;
        if (mModel) {
            mModel->mAnim->mFramePerFrame = 0x4000;
        }
        mState++;
        break;
    }
    case 2: {
        DrawModel();
        BattleTarget *target = data_020d433c.mBattlePlayers->mBattleTarget;
        if (!target) {
            return;
        }
        BakuganModel *h = target->mBakugan;
        VEC_Subtract(&mPosition, &h->mPosition, &d);
        d.x = FX_Mul(d.x, 0x400);
        d.y = FX_Mul(d.y, 0x400);
        d.z = FX_Mul(d.z, 0x400);
        VecFx32 sum;
        sum.z = d.z + h->mPosition.z;
        sum.y = d.y + h->mPosition.y;
        sum.x = d.x + h->mPosition.x;
        h->mPosition = sum;
        SetTargetVelocityXyz(data_020d433c.mBattlePlayers->mBattleTarget, 0, 0x333, 0, 0);
        data_020d433c.mUnk_14 &= 8;
        if (data_020d433c.mUnk_14) {
            return;
        }
        mTimer--;
        if (mTimer != 0) {
            return;
        }
        mState = 3;
        if (mModel) {
            mModel->mAnim->mFramePerFrame = -0x1000;
        }
        break;
    }
    case 3: {
        DrawModel();
        u32 x = MATH_Rand32(&gRandom, 0x2000);
        u32 y = MATH_Rand32(&gRandom, 0x1000);
        u32 z = MATH_Rand32(&gRandom, 0x2000);
        rnd.x = x - 0x1000; rnd.y = y; rnd.z = z - 0x1000;
        VEC_Normalize(&rnd, &rnd);
        rnd.x = FX_Mul(rnd.x, 0x6000);
        rnd.y = FX_Mul(rnd.y, 0x6000);
        rnd.z = FX_Mul(rnd.z, 0x6000);
        if (data_020d433c.mBattlePlayers->mBattleTarget) {
            SetTargetVelocity(data_020d433c.mBattlePlayers->mBattleTarget, &rnd, 0);
        }
        SoundManager::Get().StartSeqArc(0x3a);
        mState++;
        break;
    }
    case 4:
        DrawModel();
        if (!mCollision) {
            return;
        }
        if (mCollision->mHitCount) {
            return;
        }
        mState = 1;
        if (mModel) {
            mModel->mAnim->mFramePerFrame = 0x1000;
        }
        break;
    case 5:
        mFinished = 1;
        break;
    }
}

void GimmickHaosBall::DrawModel() {
    if (!mModel) {
        return;
    }
    if ((int)data_020d433c.mGimmicks->mState < 2) {
        return;
    }
    DrawInstance();
}
