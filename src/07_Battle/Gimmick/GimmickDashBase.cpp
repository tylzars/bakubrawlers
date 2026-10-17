#include "Battle/GimmickTypes.hpp"
#include "System/FileSystem.hpp"
#include "System/SoundManager.hpp"

void func_ov007_0225c960(BattleTarget *target, VecFx32 *dir, int unk);
extern "C" void func_01ffcfd8(MtxFx33 *m, u16 angle);
extern "C" void func_01ffcf80(MtxFx33 *m, u16 angle);
extern "C" void func_01ffd030(MtxFx33 *m, u16 angle);

static ModelResource *sResource;

GimmickDashBase::GimmickDashBase(u8 a)
    : Gimmick(a, 0) {
    mUnk_54 = 0;
    mUnk_55 = 0;
    mState  = 0;
    mUnk_3a = 5;
    mDir.x  = 0;
    mDir.y  = 0;
    mDir.z  = -0xa000;
}

GimmickDashBase::~GimmickDashBase() {
    Unload();
}

void GimmickDashBase::func_ov007_0222c48c() {
    char buf[0x804];
    func_ov007_022719c8(buf, 0x5e);
    mUnk_3c = func_ov007_02271ca8(data_020d433c.mUnk_0c, 0x20, buf, &mPosition, &mRotX, mUnk_3a);
    func_ov007_0222c824(0);
}

void GimmickDashBase::Load() {
    mUnk_55 = func_ov007_0222ea84(0xa000);
    if (mUnk_55) {
        return;
    }
    if (!sResource) {
        sResource = new ModelResource("dashbase_00.nsbmd", "dashbase_00_data.bin", "dashbase_00.nsbtx", 1);
    }
    ChangeDir("/Game/Gimmick");
    sResource->AddRef(1);
    mModel = sResource->mModel->CreateInstance(new ModelAnim(sResource->mAnimData, sResource->mModel));
    mModel->StartAnimWithIncrement(0, 1, 0x1000);
    mAnimEnabled = 1;
    ChangeDir("/");
}

void GimmickDashBase::Unload() {
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

void GimmickDashBase::Finish() {
    mState = 3;
}

void GimmickDashBase::Update() {
    switch (mState) {
    case 0:
        func_ov007_0222c48c();
        mState++;
        break;
    case 1: {
        if (mUnk_55) {
            return;
        }
        func_ov007_0222c7f0();
        BattleTarget *target = data_020d433c.mBattlePlayers->mUnk_298;
        if (!target) {
            return;
        }
        data_020d433c.mUnk_14 &= 8;
        if (data_020d433c.mUnk_14) {
            return;
        }
        target = data_020d433c.mBattlePlayers->mUnk_298;
        if (target->mUnk_44 > 0x5dc) {
            return;
        }
        if (!(mUnk_3c && mUnk_3c->mUnk_823)) {
            return;
        }
        if (target->mUnk_38 > 3) {
            return;
        }
        func_ov007_0225c960(target, &mDir, 0);
        SoundManager::Get().StartSeqArc(0x38);
        mUnk_54 = 3;
        mState++;
        break;
    }
    case 2:
        if (mUnk_55) {
            return;
        }
        func_ov007_0222c7f0();
        if (!mUnk_3c) {
            return;
        }
        if (mUnk_3c->mUnk_823 == 0) {
            mState = 1;
        }
        break;
    case 3:
        mFinished = 1;
        break;
    }
}

void GimmickDashBase::func_ov007_0222c7f0() {
    if (!mModel) {
        return;
    }
    if ((int)data_020d433c.mGimmicks->mState < 2) {
        return;
    }
    func_020526bc();
}

void GimmickDashBase::func_ov007_0222c824(int reset) {
    MtxFx33 m = data_0220e848;
    if (mRotY) {
        func_01ffcfd8(&m, mRotY);
    }
    if (mRotX) {
        func_01ffcf80(&m, mRotX);
    }
    if (mRotZ) {
        func_01ffd030(&m, mRotZ);
    }
    if (reset) {
        mDir.x = 0;
        mDir.y = 0;
        mDir.z = -0xa000;
    }
    func_01ff84d0(&mDir, &m, &mDir);
}
