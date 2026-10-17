#include "System/SoundManager.hpp"
#include "Battle/GimmickTypes.hpp"
#include <nitro/fx.h>
#include "Game/UnkStruct_020d433c.hpp"
#include "Graphics/Camera.hpp"
#include "System/FileSystem.hpp"
#include "System/SoundManager.hpp"

void func_ov007_02271d78(GimmickCollision *data, GimmickSetData *set);
void func_ov007_022719c8(void *buf, u8 id);
GimmickSetData *func_ov007_02271ca8(GimmickCollision *data, int count, void *buf, VecFx32 *pos, u16 *rot, u8 unk);
extern MtxFx43 data_020c1cf4;

static ModelResource *sResources[2];

GimmickSuteWall::GimmickSuteWall(u8 a)
    : Gimmick(a, 0) {
    mModelB = 0;
    mUnk_4c = 0;
    mState  = 0;
    mUnk_3a = 0x3c;
}

GimmickSuteWall::~GimmickSuteWall() {
    Unload();
}

void GimmickSuteWall::Load() {
    char buf[0x804];
    mUnk_4c = func_ov007_0222ea84(0xa000);
    if (mUnk_4c) {
        GimmickSetData *set = mUnk_3c;
        if (!set) {
            return;
        }
        func_ov007_02271d78(data_020d433c.mGimmickCollision, set);
        mUnk_3c = 0;
        return;
    }
    if (!mUnk_3c) {
        func_ov007_022719c8(buf, 0x3c);
        mUnk_3c = func_ov007_02271ca8(data_020d433c.mGimmickCollision, 8, buf, &mPosition, &mRotX, mUnk_3a);
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
        func_ov007_0222ea28(0x3c, 8);
        mState++;
        break;
    case 1: {
        if (mUnk_4c) {
            return;
        }
        if (mModel && (int)data_020d433c.mGimmicks->mState >= 2) {
            func_020526bc();
        }
        BattleTarget *target = data_020d433c.mBattlePlayers->mBattleTarget;
        if (!(target && mUnk_3c && mUnk_3c->mUnk_823)) {
            return;
        }
        if (VEC_Mag(&target->mUnk_64) < 0x5000) {
            return;
        }
        func_ov007_02271d78(data_020d433c.mGimmickCollision, mUnk_3c);
        SoundManager::Get().StartSeqArc(4);
        mState++;
        break;
    }
    case 2:
        if (mUnk_4c) {
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
            data_020d433c.mGimmicks->func_ov007_0222e860(mUnk_39);
        }
        mState++;
    }
    CameraManager::Get();
    MtxFx43 mtx = data_020c1cf4;
    func_01ffcf64(&mtx, &mPosition);
    mModelB->Draw(&mtx, &mScale);
}
