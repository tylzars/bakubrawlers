#include "Battle/GimmickTypes.hpp"
#include <nitro/fx.h>
#include "Graphics/Camera.hpp"
#include "System/FileSystem.hpp"
#include "System/SoundManager.hpp"

extern "C" void func_02024764(SoundManager *manager, u32 a, u32 b);

static VecFx32 data_ov007_02285bd0 = {0, 0x3000, 0};

static u8 sCount;
static ModelResource *sResources[2];

GimmickAquosFukidashi::GimmickAquosFukidashi(u8 id)
    : Gimmick(id, 0) {
    mModelB  = 0;
    mCounter = 0;
    mUnk_4e  = 0;
    mUnk_50  = 0;
    mUnk_51  = 0;
    mUnk_52  = 0;
    mState   = 0;
    mUnk_3a  = 0x29;
}

GimmickAquosFukidashi::~GimmickAquosFukidashi() {
    Unload();
}

void GimmickAquosFukidashi::Load() {
    mUnk_51 = func_ov007_0222ea84(0xa000);
    if (mUnk_51) {
        return;
    }
    if (!sResources[1]) {
        sResources[1] = new ModelResource("aqousfukidasi_n.nsbmd", "aqousfukidasi_n_data.bin", "aqousfukidasi_n.nsbtx", 1);
        sResources[0] = new ModelResource("aqousfukidasi_b.nsbmd", "aqousfukidasi_b_data.bin", "aqousfukidasi_b.nsbtx", 1);
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
    switch (mState) {
    case 2: {
        u32 n   = 0x104 - mCounter;
        u32 v   = n << 12;
        float f = n > 0 ? 0.5f + v : v - 0.5f;
        mModelB->func_02028e70((int)f);
        break;
    }
    case 3: {
        u32 n   = 0x140 - mCounter;
        u32 v   = n << 12;
        float f = n > 0 ? 0.5f + v : v - 0.5f;
        mModelB->func_02028e70((int)f);
        break;
    }
    }
}

void GimmickAquosFukidashi::Unload() {
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

void GimmickAquosFukidashi::Finish() {
    mState = 4;
}

void GimmickAquosFukidashi::Update() {
    switch (mState) {
    case 0:
        func_ov007_0222ea28(0x3c, 8);
        mState++;
        mCounter = 0x78;
        break;
    case 1:
        if (mUnk_51) {
            return;
        }
        if (mUnk_4e) {
            mUnk_4e--;
        }
        func_ov007_022766c4(0);
        func_ov007_02276710(0x104, (u8)(mState + 1));
        mUnk_52 = 0;
        break;
    case 2: {
        if (mUnk_51) {
            return;
        }
        int near = func_ov007_02276760();
        if (mUnk_4e) {
            mUnk_4e--;
        }
        func_ov007_022766c4(1);
        if (!mUnk_50 && near) {
            SoundManager::Get().func_020246f8(0x63);
            sCount++;
        } else if (mUnk_50 && !near) {
            sCount--;
            if (!sCount) {
                func_02024764(&SoundManager::Get(), 0x63, 0x1e);
            }
        }
        mUnk_50 = near;
        if (func_ov007_02276710(0x3c, (u8)(mState + 1))) {
            func_02024764(&SoundManager::Get(), 0x63, 0x1e);
            if (mUnk_50) {
                sCount--;
            }
            mUnk_50 = 0;
        }
        if ((u32)(0x104 - mCounter) >= 10 && mUnk_3c && mUnk_3c->mUnk_823 && !mUnk_4e) {
            if (data_020d433c.mBattlePlayers->mUnk_298) {
                BattlePlayer *p = data_020d433c.mBattlePlayers->Current();
                if (p->mRecords[p->mRecord].mUnk_09 != 1) {
                    VecFx32 a = data_ov007_02285bd0;
                    VecFx32 b = data_020d433c.mBattlePlayers->mUnk_298->mUnk_64;
                    int dy = data_020d433c.mBattlePlayers->mUnk_298->mUnk_1a4->mPosition.y - mPosition.y + -0x54cd;
                    if (dy <= 0x1000) {
                        dy = 0x1000;
                    }
                    a.y = func_01ff8000(a.y, FX_Mul(dy, 0x800));
                    VEC_Add(&b, &a, &b);
                    b.x = FX_Mul(b.x, 0xc00);
                    b.z = FX_Mul(b.z, 0xc00);
                    func_ov007_0225c960(data_020d433c.mBattlePlayers->mUnk_298, &b, 0);
                    mUnk_52 = 1;
                }
            }
        }
        if (mUnk_3c && mUnk_52) {
            if (!mUnk_4e && !mUnk_3c->mUnk_823) {
                mUnk_4e = 0x8c;
            }
        }
        break;
    }
    case 3:
        if (mUnk_51) {
            return;
        }
        if (mUnk_4e) {
            mUnk_4e--;
        }
        func_ov007_022766c4(1);
        if (!func_ov007_02276710(0x78, 1)) {
            return;
        }
        mUnk_52 = 0;
        if (!mModelB) {
            return;
        }
        mModelB->func_02028e54(0, 0, 0x1000);
        break;
    case 4:
        mFinished = 1;
        break;
    }
}

void GimmickAquosFukidashi::func_ov007_02276634() {
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

void GimmickAquosFukidashi::func_ov007_022766c4(int drawB) {
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
    func_ov007_02276634();
}

int GimmickAquosFukidashi::func_ov007_02276710(int frames, int state) {
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

int GimmickAquosFukidashi::func_ov007_02276760() {
    VecFx32 d;
    BattleTarget *target = data_020d433c.mBattlePlayers->mUnk_298;
    if (target && target->mUnk_1a4 && target->mUnk_1a4->mUnk_4a) {
        VEC_Subtract(&target->mUnk_1a4->mPosition, &mPosition, &d);
        if (VEC_Mag(&d) <= 0x96000) {
            return 1;
        }
    }
    return 0;
}
