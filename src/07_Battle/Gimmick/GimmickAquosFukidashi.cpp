#include "Battle/GimmickTypes.hpp"
#include <nitro/fx.h>
#include "Graphics/Camera.hpp"
#include "System/FileSystem.hpp"
#include "System/SoundManager.hpp"

extern "C" void StopSeqArc(SoundManager *manager, u32 a, u32 b);

static const VecFx32 sPushVelocity = {0, 0x3000, 0};

static u8 sCount;
static ModelResource *sResources[2];

GimmickAquosFukidashi::GimmickAquosFukidashi(u8 id)
    : Gimmick(id, 0) {
    mModelB  = 0;
    mTimer = 0;
    mPushTimer  = 0;
    mTargetNear  = 0;
    mMarkerNear  = 0;
    mPushed  = 0;
    mState   = 0;
    mType  = GIMMICK_TYPE_AQUOS_FUKIDASHI;
}

GimmickAquosFukidashi::~GimmickAquosFukidashi() {
    Unload();
}

void GimmickAquosFukidashi::Load() {
    mMarkerNear = IsNearMarker(0xa000);
    if (mMarkerNear) {
        return;
    }
    if (!sResources[1]) {
        sResources[1] = new ModelResource("aqousfukidasi_n.nsbmd", "aqousfukidasi_n_data.bin", "aqousfukidasi_n.nsbtx", 1);
        sResources[0] = new ModelResource("aqousfukidasi_b.nsbmd", "aqousfukidasi_b_data.bin", "aqousfukidasi_b.nsbtx", 1);
    }
    ChangeDir("/Game/Gimmick");
    sResources[1]->AddRef(1);
    mModel = sResources[1]->mModel->CreateInstance(new ModelAnim(sResources[1]->mAnimData, sResources[1]->mModel));
    mModel->StartAnimWithIncrement(0, 1, 0x1000);
    mAnimEnabled = 1;
    sResources[0]->AddRef(1);
    mModelB = sResources[0]->mModel->CreateInstance(new ModelAnim(sResources[0]->mAnimData, sResources[0]->mModel));
    mModelB->StartAnimWithIncrement(0, 0, 0x1000);
    ChangeDir("/");
    switch (mState) {
    case 2: {
        u32 n   = 0x104 - mTimer;
        u32 v   = n << 12;
        float f = n > 0 ? 0.5f + v : v - 0.5f;
        mModelB->SetAnimFrame((int)f);
        break;
    }
    case 3: {
        u32 n   = 0x140 - mTimer;
        u32 v   = n << 12;
        float f = n > 0 ? 0.5f + v : v - 0.5f;
        mModelB->SetAnimFrame((int)f);
        break;
    }
    }
}

void GimmickAquosFukidashi::Unload() {
    if (mModel) {
        ModelInstance *m = mModel;
        m->mOwnFlags       = MODEL_OWN_ANIM;
        delete m;
        mModel = 0;
        ModelInstance *b = mModelB;
        b->mOwnFlags       = MODEL_OWN_ANIM;
        delete b;
        mModelB = 0;
    }
    if (sResources[1]) {
        sResources[0]->ReleaseRef();
        if (sResources[1]->ReleaseRef()) {
            ModelResource *r = sResources[1];
            delete r;
            sResources[1] = 0;
            r             = sResources[0];
            delete r;
            sResources[0] = 0;
        }
    }
}

void GimmickAquosFukidashi::Finish() {
    mState = 4;
}

void GimmickAquosFukidashi::Update() {
    switch (mState) {
    case 0:
        LoadCollision(0x3c, COLLISION_GROUP_GIMMICK);
        mState++;
        mTimer = 0x78;
        break;
    case 1:
        if (mMarkerNear) {
            return;
        }
        if (mPushTimer) {
            mPushTimer--;
        }
        DrawModels(0);
        CountDownToState(0x104, (u8)(mState + 1));
        mPushed = 0;
        break;
    case 2: {
        if (mMarkerNear) {
            return;
        }
        int near = IsTargetNear();
        if (mPushTimer) {
            mPushTimer--;
        }
        DrawModels(1);
        if (!mTargetNear && near) {
            SoundManager::Get().StartSeqArc(0x63);
            sCount++;
        } else if (mTargetNear && !near) {
            sCount--;
            if (!sCount) {
                StopSeqArc(&SoundManager::Get(), 0x63, 0x1e);
            }
        }
        mTargetNear = near;
        if (CountDownToState(0x3c, (u8)(mState + 1))) {
            StopSeqArc(&SoundManager::Get(), 0x63, 0x1e);
            if (mTargetNear) {
                sCount--;
            }
            mTargetNear = 0;
        }
        if ((u32)(0x104 - mTimer) >= 10 && mCollision && mCollision->mHitCount && !mPushTimer) {
            if (data_020d433c.mBattlePlayers->mBattleTarget) {
                BattlePlayer *p = data_020d433c.mBattlePlayers->Current();
                if (p->mRecords[p->mRecord].mAttribute != 1) {
                    VecFx32 a = sPushVelocity;
                    VecFx32 b = data_020d433c.mBattlePlayers->mBattleTarget->mVelocity;
                    int dy = data_020d433c.mBattlePlayers->mBattleTarget->mBakugan->mPosition.y - mPosition.y + -0x54cd;
                    if (dy <= 0x1000) {
                        dy = 0x1000;
                    }
                    a.y = FX_Div(a.y, FX_Mul(dy, 0x800));
                    VEC_Add(&b, &a, &b);
                    b.x = FX_Mul(b.x, 0xc00);
                    b.z = FX_Mul(b.z, 0xc00);
                    SetTargetVelocity(data_020d433c.mBattlePlayers->mBattleTarget, &b, 0);
                    mPushed = 1;
                }
            }
        }
        if (mCollision && mPushed) {
            if (!mPushTimer && !mCollision->mHitCount) {
                mPushTimer = 0x8c;
            }
        }
        break;
    }
    case 3:
        if (mMarkerNear) {
            return;
        }
        if (mPushTimer) {
            mPushTimer--;
        }
        DrawModels(1);
        if (!CountDownToState(0x78, 1)) {
            return;
        }
        mPushed = 0;
        if (!mModelB) {
            return;
        }
        mModelB->StartAnimWithIncrement(0, 0, 0x1000);
        break;
    case 4:
        mFinished = 1;
        break;
    }
}

void GimmickAquosFukidashi::DrawModelB() {
    if (!mModelB) {
        return;
    }
    MtxFx43 mtx = gIdentityMtx43;
    MTX_SetTranslation43(&mtx, &mPosition);
    data_020d433c.mFlags &= 8;
    if (!data_020d433c.mFlags) {
        mModelB->AdvanceAnim();
    }
    mModelB->Draw(&mtx, &mScale);
}

void GimmickAquosFukidashi::DrawModels(int drawB) {
    if (!mModel) {
        return;
    }
    if ((int)data_020d433c.mGimmicks->mState < 2) {
        return;
    }
    DrawInstance();
    if (!drawB) {
        return;
    }
    DrawModelB();
}

int GimmickAquosFukidashi::CountDownToState(int frames, int state) {
    data_020d433c.mFlags &= 8;
    if (!data_020d433c.mFlags) {
        mTimer--;
        if (mTimer == 0) {
            mTimer = frames;
            mState   = state;
            return 1;
        }
    }
    return 0;
}

int GimmickAquosFukidashi::IsTargetNear() {
    VecFx32 d;
    BattleTarget *target = data_020d433c.mBattlePlayers->mBattleTarget;
    if (target && target->mBakugan && target->mBakugan->mVisible) {
        VEC_Subtract(&target->mBakugan->mPosition, &mPosition, &d);
        if (VEC_Mag(&d) <= 0x96000) {
            return 1;
        }
    }
    return 0;
}
