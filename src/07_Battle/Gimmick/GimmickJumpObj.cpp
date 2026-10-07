#include "Battle/GimmickTypes.hpp"
#include "Game/Random.hpp"
#include "System/FileSystem.hpp"
#include "System/SoundManager.hpp"

void func_01ffcfd8(MtxFx33 *m, u16 angle);
void func_01ffcf80(MtxFx33 *m, u16 angle);
void func_01ffd030(MtxFx33 *m, u16 angle);

static ModelResource *sResource;

GimmickJumpObj::GimmickJumpObj(u8 a)
    : Gimmick(a, 0) {
    mTimer  = 0;
    mState  = 0;
    mUnk_3a = 4;
    mDir.x  = 0;
    mDir.y  = 0x3000;
    mDir.z  = 0;
}

GimmickJumpObj::~GimmickJumpObj() {
    Unload();
}

void GimmickJumpObj::func_ov007_0222dc3c() {
    char buf[0x804];
    func_ov007_022719c8(buf, 0x5d);
    mUnk_3c = func_ov007_02271ca8(data_020d433c.mUnk_0c, 0x20, buf, &mPosition, &mRotX, mUnk_3a);
    func_ov007_0222e08c(0);
}

void GimmickJumpObj::Load() {
    if (!sResource) {
        sResource = new ModelResource("jumpobj_00.nsbmd", "jumpobj_00_data.bin", "jumpobj_00.nsbtx", 1);
    }
    ChangeDir("/Game/Gimmick");
    sResource->func_020306e4(1);
    mModel = sResource->mModel->func_02028b78(new ModelAnim(sResource->mAnimData, sResource->mModel));
    mModel->func_02028e54(0, 1, 0x1000);
    mUnk_00 = 1;
    ChangeDir("/");
}

void GimmickJumpObj::Unload() {
    if (mModel) {
        ModelInstance *m = mModel;
        m->mUnk_68       = 1;
        delete m;
        mModel = 0;
    }
    if (sResource && sResource->func_020307e8()) {
        ModelResource *r = sResource;
        delete r;
        sResource = 0;
    }
}

void GimmickJumpObj::Finish() {
    mState = 3;
}

void GimmickJumpObj::Update() {
    MtxFx33 m = data_0220e848;
    switch (mState) {
    case 0:
        func_ov007_0222dc3c();
        mState++;
        break;
    case 1: {
        if (mModel && (int)data_020d433c.mGimmicks->mState >= 2) {
            if (mTimer == 0) {
                func_020526bc();
            } else {
                data_020d433c.mUnk_14 &= 8;
                if (data_020d433c.mUnk_14 == 0) {
                    mTimer--;
                }
            }
        }
        if (!(mUnk_3c && mUnk_3c->mUnk_823)) {
            return;
        }
        BattleTarget *target = data_020d433c.mBattlePlayers->mUnk_298;
        if (!target) {
            return;
        }
        if (target->mUnk_1c6) {
            return;
        }
        func_ov007_0222e08c(1);
        u32 r = MATH_Rand32(&data_020d42e8, 0x444);
        func_01ffcfd8(&m, r - 0x222);
        func_01ff84d0(&mDir, &m, &mDir);
        func_ov007_0225c960(data_020d433c.mBattlePlayers->mUnk_298, &mDir, 1);
        func_ov007_0225ca18(data_020d433c.mBattlePlayers->mUnk_298, 4, 0xb4);
        mTimer = 0x3c;
        SoundManager::Get().func_020246f8(0x37);
        mState++;
        break;
    }
    case 2:
        mTimer = 0x3c;
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

void GimmickJumpObj::func_ov007_0222e08c(int reset) {
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
        mDir.y = 0x3000;
        mDir.z = 0;
    }
    func_01ff84d0(&mDir, &m, &mDir);
}
