#include "Battle/GimmickTypes.hpp"
#include "Game/Random.hpp"
#include "Graphics/Camera.hpp"
#include "System/FileSystem.hpp"
#include "System/SoundManager.hpp"

static u8 sCount;
static ModelResource *sResources[2];

GimmickIkazuchiStart::GimmickIkazuchiStart(u8 id)
    : Gimmick(id, 0) {
    mModelB  = 0;
    mTimer = 0;
    mDelayIndex  = 0;
    mMarkerNear  = 0;
    for (u8 i = 0; i < 8; i++) {
        mDelays[i] = MATH_Rand32(&gRandom, 0x3c) + 0x3c;
    }
    mState  = 0;
    mType = GIMMICK_TYPE_IKAZUCHI_START;
}

GimmickIkazuchiStart::~GimmickIkazuchiStart() {
    Unload();
}

void GimmickIkazuchiStart::Load() {
    mMarkerNear = IsNearMarker(0xa000);
    if (mMarkerNear) {
        return;
    }
    if (!sResources[1]) {
        sResources[1] = new ModelResource("ikazuchistart.nsbmd", "ikazuchistart_data.bin", "ikazuchistart.nsbtx", 1);
        sResources[0] = new ModelResource("ikaz00.nsbmd", "ikaz00_data.bin", "ikaz00.nsbtx", 1);
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
}

void GimmickIkazuchiStart::Unload() {
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

void GimmickIkazuchiStart::Finish() {
    mState = 8;
}

void GimmickIkazuchiStart::Update() {
    switch (mState) {
    case 0:
        LoadCollision(0x52, COLLISION_GROUP_GIMMICK);
        mState++;
        break;
    case 1:
        if (mMarkerNear) {
            return;
        }
        mDelayIndex  = (mDelayIndex + 1) % 8;
        mTimer = mDelays[mDelayIndex];
        mState++;
        break;
    case 2:
        if (mMarkerNear) {
            return;
        }
        gGameData.mFlags &= 8;
        if (gGameData.mFlags) {
            return;
        }
        mTimer--;
        if (mTimer) {
            return;
        }
        mTimer = 0x1e;
        if (sCount >= 2) {
            mState = 1;
            return;
        }
        sCount++;
        mState++;
        break;
    case 3:
        if (mMarkerNear) {
            mState = 6;
            return;
        }
        func_ov007_022789e0(0);
        gGameData.mFlags &= 8;
        if (gGameData.mFlags) {
            return;
        }
        mTimer--;
        if (mTimer) {
            return;
        }
        if (mModel && (int)gGameData.mGimmicks->mState >= 1) {
            SoundManager::Get().StartSeqArc(0x61);
        }
        mTimer = 0x46;
        mState++;
        break;
    case 4: {
        if (mMarkerNear) {
            mState = 6;
            return;
        }
        if (func_ov007_022789e0(1)) {
            return;
        }
        if (!(mCollision && mCollision->mHitCount)) {
            return;
        }
        BattlePlayerList *list = gGameData.mBattlePlayers;
        BattleTarget *t        = list->mBattleTarget;
        if (!t) {
            return;
        }
        if (t->mIkazuchiTimer) {
            return;
        }
        BattlePlayer *p = list->Current();
        if (p->mRecords[p->mRecord].mAttribute == 4) {
            return;
        }
        VecFx32 v = {0, 0, 0};
        t         = list->mBattleTarget;
        v.x       = t->mVelocity.x;
        v.y       = t->mVelocity.y;
        v.z       = t->mVelocity.z;
        v.y       = 0x2000;
        SetTargetVelocity(list->mBattleTarget, &v, 0);
        func_ov007_0225ca18(gGameData.mBattlePlayers->mBattleTarget, 8, 0x1e);
        mState++;
        break;
    }
    case 5:
        if (mMarkerNear) {
            mState = 6;
            return;
        }
        func_ov007_022789e0(1);
        break;
    case 6:
        sCount--;
        mState++;
        break;
    case 7:
        if (mMarkerNear) {
            return;
        }
        mState = 1;
        if (!mModelB) {
            return;
        }
        mModelB->StartAnimWithIncrement(0, 0, 0x1000);
        break;
    case 8:
        mFinished = 1;
        break;
    }
}

int GimmickIkazuchiStart::func_ov007_022789e0(int count) {
    int result = 0;
    if (mModel && (int)gGameData.mGimmicks->mState >= 1) {
        DrawInstance();
        if (count && mModelB) {
            MtxFx43 mtx = gIdentityMtx43;
            MTX_SetTranslation43(&mtx, &mPosition);
            gGameData.mFlags &= 8;
            if (!gGameData.mFlags) {
                mModelB->AdvanceAnim();
            }
            mModelB->Draw(&mtx, &mScale);
        }
    }
    if (count) {
        gGameData.mFlags &= 8;
        if (!gGameData.mFlags) {
            mTimer--;
            if (mTimer == 0) {
                mState = 1;
                sCount--;
                if (mModelB) {
                    mModelB->StartAnimWithIncrement(0, 0, 0x1000);
                }
                result = 1;
            }
        }
    }
    return result;
}
