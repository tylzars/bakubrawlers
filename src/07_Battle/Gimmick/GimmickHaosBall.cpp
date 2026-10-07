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
    mUnk_3a = 0x46;
}

GimmickHaosBall::~GimmickHaosBall() {
    Unload();
}

void GimmickHaosBall::Load() {
    if (!sResource) {
        sResource = new ModelResource("Haosball.nsbmd", "Haosball_data.bin", "Haosball.nsbtx", 1);
    }
    ChangeDir("/Game/Gimmick");
    sResource->func_020306e4(1);
    mModel = sResource->mModel->func_02028b78(new ModelAnim(sResource->mAnimData, sResource->mModel));
    mModel->func_02028e54(0, 1, 0x1000);
    mUnk_00 = 1;
    ChangeDir("/");
}

void GimmickHaosBall::Unload() {
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

void GimmickHaosBall::Finish() {
    mState = 5;
}

void GimmickHaosBall::func_ov007_0222db58() {
    if (!mModel) {
        return;
    }
    if ((int)data_020d433c.mGimmicks->mState < 2) {
        return;
    }
    func_020526bc();
}

void GimmickHaosBall::Update() {
    VecFx32 rnd;
    VecFx32 d;
    switch (mState) {
    case 0:
        func_ov007_0222ea28(0x46, 8);
        mState++;
        break;
    case 1: {
        func_ov007_0222db58();
        if (!(mUnk_3c && mUnk_3c->mUnk_823)) {
            return;
        }
        BattlePlayer *p = data_020d433c.mBattlePlayers->Current();
        if (p->mRecords[p->mRecord].mUnk_09 == 3) {
            return;
        }
        mTimer = 0x1e;
        if (mModel) {
            mModel->mAnim->mUnk_0c = 0x4000;
        }
        mState++;
        break;
    }
    case 2: {
        func_ov007_0222db58();
        BattleTarget *target = data_020d433c.mBattlePlayers->mUnk_298;
        if (!target) {
            return;
        }
        BattleVecHolder4 *h = target->mUnk_1a4;
        VEC_Subtract(&mPosition, &h->mPos, &d);
        d.x = FX_Mul(d.x, 0x400);
        d.y = FX_Mul(d.y, 0x400);
        d.z = FX_Mul(d.z, 0x400);
        VecFx32 sum;
        sum.z = d.z + h->mPos.z;
        sum.y = d.y + h->mPos.y;
        sum.x = d.x + h->mPos.x;
        h->mPos = sum;
        func_ov007_0225c9c0(data_020d433c.mBattlePlayers->mUnk_298, 0, 0x333, 0, 0);
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
            mModel->mAnim->mUnk_0c = -0x1000;
        }
        break;
    }
    case 3: {
        func_ov007_0222db58();
        u32 x = MATH_Rand32(&data_020d42e8, 0x2000);
        u32 y = MATH_Rand32(&data_020d42e8, 0x1000);
        u32 z = MATH_Rand32(&data_020d42e8, 0x2000);
        rnd.x = x - 0x1000; rnd.y = y; rnd.z = z - 0x1000;
        VEC_Normalize(&rnd, &rnd);
        rnd.x = FX_Mul(rnd.x, 0x6000);
        rnd.y = FX_Mul(rnd.y, 0x6000);
        rnd.z = FX_Mul(rnd.z, 0x6000);
        if (data_020d433c.mBattlePlayers->mUnk_298) {
            func_ov007_0225c960(data_020d433c.mBattlePlayers->mUnk_298, &rnd, 0);
        }
        SoundManager::Get().func_020246f8(0x3a);
        mState++;
        break;
    }
    case 4:
        func_ov007_0222db58();
        if (!mUnk_3c) {
            return;
        }
        if (mUnk_3c->mUnk_823) {
            return;
        }
        mState = 1;
        if (mModel) {
            mModel->mAnim->mUnk_0c = 0x1000;
        }
        break;
    case 5:
        mFinished = 1;
        break;
    }
}
