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
    mCounter = 0;
    mUnk_5e  = 0;
    mUnk_5f  = 0;
    for (u8 i = 0; i < 8; i++) {
        mDelays[i] = MATH_Rand32(&data_020d42e8, 0x3c) + 0x3c;
    }
    mState  = 0;
    mUnk_3a = 0x52;
}

GimmickIkazuchiStart::~GimmickIkazuchiStart() {
    Unload();
}

void GimmickIkazuchiStart::Load() {
    mUnk_5f = func_ov007_0222ea84(0xa000);
    if (mUnk_5f) {
        return;
    }
    if (!sResources[1]) {
        sResources[1] = new ModelResource("ikazuchistart.nsbmd", "ikazuchistart_data.bin", "ikazuchistart.nsbtx", 1);
        sResources[0] = new ModelResource("ikaz00.nsbmd", "ikaz00_data.bin", "ikaz00.nsbtx", 1);
    }
    ChangeDir("/Game/Gimmick");
    sResources[1]->func_020306e4(1);
    mModel = sResources[1]->mModel->func_02028b78(new ModelAnim(sResources[1]->mAnimData, sResources[1]->mModel));
    mModel->func_02028e54(0, 1, 0x1000);
    mUnk_00 = 1;
    sResources[0]->func_020306e4(1);
    mModelB = sResources[0]->mModel->func_02028b78(new ModelAnim(sResources[0]->mAnimData, sResources[0]->mModel));
    mModelB->func_02028e54(0, 0, 0x1000);
    ChangeDir("/");
}

void GimmickIkazuchiStart::Unload() {
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
    if (sResources[1]) {
        sResources[0]->func_020307e8();
        if (sResources[1]->func_020307e8()) {
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
        func_ov007_0222ea28(0x52, 8);
        mState++;
        break;
    case 1:
        if (mUnk_5f) {
            return;
        }
        mUnk_5e  = (mUnk_5e + 1) % 8;
        mCounter = mDelays[mUnk_5e];
        mState++;
        break;
    case 2:
        if (mUnk_5f) {
            return;
        }
        data_020d433c.mUnk_14 &= 8;
        if (data_020d433c.mUnk_14) {
            return;
        }
        mCounter--;
        if (mCounter) {
            return;
        }
        mCounter = 0x1e;
        if (sCount >= 2) {
            mState = 1;
            return;
        }
        sCount++;
        mState++;
        break;
    case 3:
        if (mUnk_5f) {
            mState = 6;
            return;
        }
        func_ov007_022789e0(0);
        data_020d433c.mUnk_14 &= 8;
        if (data_020d433c.mUnk_14) {
            return;
        }
        mCounter--;
        if (mCounter) {
            return;
        }
        if (mModel && (int)data_020d433c.mGimmicks->mState >= 1) {
            SoundManager::Get().func_020246f8(0x61);
        }
        mCounter = 0x46;
        mState++;
        break;
    case 4: {
        if (mUnk_5f) {
            mState = 6;
            return;
        }
        if (func_ov007_022789e0(1)) {
            return;
        }
        if (!(mUnk_3c && mUnk_3c->mUnk_823)) {
            return;
        }
        BattlePlayerList *list = data_020d433c.mBattlePlayers;
        BattleTarget *t        = list->mUnk_298;
        if (!t) {
            return;
        }
        if (t->mUnk_1bc) {
            return;
        }
        BattlePlayer *p = list->Current();
        if (p->mRecords[p->mRecord].mUnk_09 == 4) {
            return;
        }
        VecFx32 v = {0, 0, 0};
        t         = list->mUnk_298;
        v.x       = t->mUnk_64.x;
        v.y       = t->mUnk_64.y;
        v.z       = t->mUnk_64.z;
        v.y       = 0x2000;
        func_ov007_0225c960(list->mUnk_298, &v, 0);
        func_ov007_0225ca18(data_020d433c.mBattlePlayers->mUnk_298, 8, 0x1e);
        mState++;
        break;
    }
    case 5:
        if (mUnk_5f) {
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
        if (mUnk_5f) {
            return;
        }
        mState = 1;
        if (!mModelB) {
            return;
        }
        mModelB->func_02028e54(0, 0, 0x1000);
        break;
    case 8:
        mFinished = 1;
        break;
    }
}

int GimmickIkazuchiStart::func_ov007_022789e0(int count) {
    int result = 0;
    if (mModel && (int)data_020d433c.mGimmicks->mState >= 1) {
        func_020526bc();
        if (count && mModelB) {
            MtxFx43 mtx = data_0220e86c;
            func_01ffcf64(&mtx, &mPosition);
            data_020d433c.mUnk_14 &= 8;
            if (!data_020d433c.mUnk_14) {
                mModelB->func_02028d80();
            }
            mModelB->func_02029174(&mtx, &mScale);
        }
    }
    if (count) {
        data_020d433c.mUnk_14 &= 8;
        if (!data_020d433c.mUnk_14) {
            mCounter--;
            if (mCounter == 0) {
                mState = 1;
                sCount--;
                if (mModelB) {
                    mModelB->func_02028e54(0, 0, 0x1000);
                }
                result = 1;
            }
        }
    }
    return result;
}
