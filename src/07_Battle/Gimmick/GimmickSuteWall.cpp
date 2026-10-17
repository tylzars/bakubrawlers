#include "System/SoundManager.hpp"
#include "Battle/GimmickTypes.hpp"
#include <nitro/fx.h>
#include "Game/UnkStruct_020d433c.hpp"
#include "Graphics/Camera.hpp"
#include "System/FileSystem.hpp"
#include "System/SoundManager.hpp"

void RemoveCollisionSet(GimmickCollision *data, GimmickSetData *set);
void LoadCollisionFile(void *buf, u8 id);
GimmickSetData *AddCollisionSet(GimmickCollision *data, int count, void *buf, VecFx32 *pos, u16 *rot, u8 unk);
extern MtxFx43 data_020c1cf4;

static ModelResource *sResources[2];

GimmickSuteWall::GimmickSuteWall(u8 a)
    : Gimmick(a, 0) {
    mModelB = 0;
    mMarkerNear = 0;
    mState  = 0;
    mType = GIMMICK_TYPE_SUTE_WALL;
}

GimmickSuteWall::~GimmickSuteWall() {
    Unload();
}

void GimmickSuteWall::Load() {
    char buf[0x804];
    mMarkerNear = IsNearMarker(0xa000);
    if (mMarkerNear) {
        GimmickSetData *set = mCollision;
        if (!set) {
            return;
        }
        RemoveCollisionSet(data_020d433c.mGimmickCollision, set);
        mCollision = 0;
        return;
    }
    if (!mCollision) {
        LoadCollisionFile(buf, 0x3c);
        mCollision = AddCollisionSet(data_020d433c.mGimmickCollision, 8, buf, &mPosition, &mRotX, mType);
    }
    if (!sResources[1]) {
        sResources[1] = new ModelResource("sutewall_n.nsbmd", "sutewall_n_data.bin", "sutewall_n.nsbtx", 1);
        sResources[0] = new ModelResource("sutewall_b.nsbmd", "sutewall_b_data.bin", "sutewall_b.nsbtx", 1);
    }
    ChangeDir("/Game/Gimmick");
    sResources[1]->AddRef(1);
    mModel = sResources[1]->mModel->CreateInstance(new ModelAnim(sResources[1]->mAnimData, sResources[1]->mModel));
    mModel->StartAnimWithIncrement(0, 1, 0x1000);
    mAnimEnabled = 1;
    sResources[0]->AddRef(1);
    mModelB = sResources[0]->mModel->CreateInstance(new ModelAnim(sResources[0]->mAnimData, sResources[0]->mModel));
    mModelB->StartAnimWithIncrement(0, 0, 0x1000);
    mModelB->mUseBoxTest = 0;
    ChangeDir("/");
}

void GimmickSuteWall::Unload() {
    if (mModel) {
        ModelInstance *m = mModelB;
        m->mOwnFlags       = 1;
        delete m;
        mModelB = 0;
        m       = mModel;
        m->mOwnFlags = 1;
        delete m;
        mModel = 0;
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

void GimmickSuteWall::Finish() {
    mState = 3;
}

void GimmickSuteWall::Update() {
    switch (mState) {
    case 0:
        LoadCollision(0x3c, 8);
        mState++;
        break;
    case 1: {
        if (mMarkerNear) {
            return;
        }
        if (mModel && (int)data_020d433c.mGimmicks->mState >= 2) {
            func_020526bc();
        }
        BattleTarget *target = data_020d433c.mBattlePlayers->mBattleTarget;
        if (!(target && mCollision && mCollision->mHitCount)) {
            return;
        }
        if (VEC_Mag(&target->mVelocity) < 0x5000) {
            return;
        }
        RemoveCollisionSet(data_020d433c.mGimmickCollision, mCollision);
        SoundManager::Get().StartSeqArc(4);
        mState++;
        break;
    }
    case 2:
        if (mMarkerNear) {
            return;
        }
        if (!mModel) {
            return;
        }
        if ((int)data_020d433c.mGimmicks->mState < 2) {
            return;
        }
        func_ov007_0222c2f0();
        break;
    case 3:
        mFinished = 1;
        break;
    }
}

int GimmickSuteWall::func_ov007_0222c2f0() {
    if (!mModelB) {
        return 0;
    }
    if (!mModelB->AdvanceAnim()) {
        if (data_020d433c.mGimmicks) {
            data_020d433c.mGimmicks->Remove(mId);
        }
        mState++;
    }
    CameraManager::Get();
    MtxFx43 mtx = data_020c1cf4;
    MTX_SetTranslation43(&mtx, &mPosition);
    mModelB->Draw(&mtx, &mScale);
}
