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
    mUnk_4e  = 0;
    mUnk_4f  = 0;
    mState   = 0;
    mUnk_3a  = 0x1e;
}

GimmickPyrusEruption::~GimmickPyrusEruption() {
    Unload();
}

void GimmickPyrusEruption::Load() {
    mUnk_4f = func_ov007_0222ea84(0xa000);
    if (mUnk_4f) {
        return;
    }
    if (!sResources[0]) {
        sResources[0] = new ModelResource("Pyruseruption_n.nsbmd", "Pyruseruption_n_data.bin", "Pyruseruption_n.nsbtx", 1);
        sResources[1] = new ModelResource("Pyruseruption_b.nsbmd", "Pyruseruption_b_data.bin", "Pyruseruption_b.nsbtx", 1);
    }
    ChangeDir("/Game/Gimmick");
    sResources[0]->func_020306e4(1);
    mModel = sResources[0]->mModel->func_02028b78(new ModelAnim(sResources[0]->mAnimData, sResources[0]->mModel));
    mModel->func_02028e54(0, 1, 0x1000);
    mUnk_00 = 1;
    sResources[1]->func_020306e4(1);
    mModelB = sResources[1]->mModel->func_02028b78(new ModelAnim(sResources[1]->mAnimData, sResources[1]->mModel));
    mModelB->func_02028e54(0, 0, 0x1000);
    ChangeDir("/");
}

void GimmickPyrusEruption::Unload() {
    if (mModel) {
        ModelInstance *m = mModel;
        m->mUnk_68       = 1;
        delete m;
        mModel = 0;
        ModelInstance *b = mModelB;
        b->mUnk_68       = 1;
        delete b;
        mModelB = 0;
    }
    if (sResources[0]) {
        sResources[1]->func_020307e8();
        if (sResources[0]->func_020307e8()) {
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
        func_ov007_0222ea28(0x1e, 8);
        mState++;
        break;
    case 1:
        if (mUnk_4f) {
            return;
        }
        func_ov007_02230040(0);
        if (!(mUnk_3c && mUnk_3c->mUnk_823)) {
            return;
        }
        mCounter = 0xf;
        mState++;
        break;
    case 2:
        if (mUnk_4f) {
            return;
        }
        func_ov007_02230040(0);
        if (!func_ov007_0223008c(0x1e, (u8)(mState + 1))) {
            return;
        }
        SoundManager::Get().func_020246f8(0x62);
        break;
    case 3: {
        if (mUnk_4f) {
            return;
        }
        func_ov007_02230040(1);
        if (func_ov007_0223008c(0x14, (u8)(mState + 1))) {
            return;
        }
        if (!mUnk_3c) {
            return;
        }
        if (mUnk_3c->mUnk_823 != 2) {
            return;
        }
        BattleTarget *target = data_020d433c.mBattlePlayers->mUnk_298;
        if (!target) {
            return;
        }
        if (mUnk_4e) {
            return;
        }
        if (target->mUnk_44 > 0x5dc) {
            return;
        }
        BattlePlayer *p = data_020d433c.mBattlePlayers->Current();
        if (p->mRecords[p->mRecord].mUnk_09 == 0) {
            return;
        }
        VEC_Subtract(&mPosition, &target->mUnk_1a4->mPosition, &d);
        fx32 len = VEC_Mag(&d);
        VEC_Normalize(&d, &d);
        d.x      = FX_Mul(d.x, 0x1e000);
        d.y      = FX_Mul(d.y, 0x1e000);
        d.z      = FX_Mul(d.z, 0x1e000);
        fx32 z   = -func_01ff8000(d.z, len);
        fx32 y   = -func_01ff8000(d.y, len);
        d.x      = -func_01ff8000(d.x, len);
        d.y      = y;
        d.z      = z;
        VEC_Add(&data_020d433c.mBattlePlayers->mUnk_298->mUnk_a0, &d, &data_020d433c.mBattlePlayers->mUnk_298->mUnk_a0);
        mState++;
        mUnk_4e = 1;
        break;
    }
    case 4:
        if (mUnk_4f) {
            return;
        }
        func_ov007_02230040(1);
        if (!func_ov007_0223008c(0, 1)) {
            return;
        }
        mUnk_4e = 0;
        if (!mModelB) {
            return;
        }
        mModelB->func_02028e54(0, 0, 0x1000);
        break;
    case 5:
        mFinished = 1;
        break;
    }
}

void GimmickPyrusEruption::func_ov007_0222ffb0() {
    if (!mModelB) {
        return;
    }
    MtxFx43 mtx = data_0220e86c;
    func_01ffcf64(&mtx, &mPosition);
    data_020d433c.mUnk_14 &= 8;
    if (!data_020d433c.mUnk_14) {
        mModelB->func_02028d80();
    }
    mModelB->func_02029174(&mtx, &mScale);
}

void GimmickPyrusEruption::func_ov007_02230040(int drawB) {
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
    func_ov007_0222ffb0();
}

int GimmickPyrusEruption::func_ov007_0223008c(int frames, int state) {
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
