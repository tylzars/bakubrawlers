#include "Battle/GimmickTypes.hpp"
#include "System/FileSystem.hpp"
#include "System/SoundManager.hpp"

void SetTargetVelocity(BattleTarget *target, VecFx32 *dir, int unk);
extern "C" void func_01ffcfd8(MtxFx33 *m, u16 angle);
extern "C" void func_01ffcf80(MtxFx33 *m, u16 angle);
extern "C" void func_01ffd030(MtxFx33 *m, u16 angle);

static ModelResource *sResource;

GimmickDashBase::GimmickDashBase(u8 a)
    : Gimmick(a, 0) {
    mUnk_54 = 0;
    mMarkerNear = 0;
    mState  = 0;
    mType = GIMMICK_TYPE_DASH_BASE;
    mDir.x  = 0;
    mDir.y  = 0;
    mDir.z  = -0xa000;
}

GimmickDashBase::~GimmickDashBase() {
    Unload();
}

void GimmickDashBase::Init() {
    char buf[0x804];
    LoadCollisionFile(buf, 0x5e);
    mCollision = AddCollisionSet(data_020d433c.mGimmickCollision, 0x20, buf, &mPosition, &mRotX, mType);
    RotateDir(0);
}

void GimmickDashBase::Load() {
    mMarkerNear = IsNearMarker(0xa000);
    if (mMarkerNear) {
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
        Init();
        mState++;
        break;
    case 1: {
        if (mMarkerNear) {
            return;
        }
        DrawModel();
        BattleTarget *target = data_020d433c.mBattlePlayers->mBattleTarget;
        if (!target) {
            return;
        }
        data_020d433c.mUnk_14 &= 8;
        if (data_020d433c.mUnk_14) {
            return;
        }
        target = data_020d433c.mBattlePlayers->mBattleTarget;
        if (target->mUnk_44 > 0x5dc) {
            return;
        }
        if (!(mCollision && mCollision->mHitCount)) {
            return;
        }
        if (target->mUnk_38 > 3) {
            return;
        }
        SetTargetVelocity(target, &mDir, 0);
        SoundManager::Get().StartSeqArc(0x38);
        mUnk_54 = 3;
        mState++;
        break;
    }
    case 2:
        if (mMarkerNear) {
            return;
        }
        DrawModel();
        if (!mCollision) {
            return;
        }
        if (mCollision->mHitCount == 0) {
            mState = 1;
        }
        break;
    case 3:
        mFinished = 1;
        break;
    }
}

void GimmickDashBase::DrawModel() {
    if (!mModel) {
        return;
    }
    if ((int)data_020d433c.mGimmicks->mState < 2) {
        return;
    }
    func_020526bc();
}

void GimmickDashBase::RotateDir(int reset) {
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
