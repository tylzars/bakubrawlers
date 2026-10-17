#include "Battle/GimmickTypes.hpp"
#include <nitro/fx.h>
#include "System/SoundManager.hpp"

extern "C" void func_01ffcfd8(MtxFx33 *m, u16 angle);
extern MtxFx33 data_0220e848;
extern "C" void func_02024764(SoundManager *manager, u32 a, u32 b);

GimmickUnk_022308a8::GimmickUnk_022308a8(u8 id)
    : Gimmick(id, 0) {
    mUnk_54 = 0;
    mState  = 0;
    mUnk_3a = 0x28;
    mUnk_48.x = 0;
    mUnk_48.y = 0;
    mUnk_48.z = 0;
}

GimmickUnk_022308a8::~GimmickUnk_022308a8() {
    Unload();
}

void GimmickUnk_022308a8::Load() {
}

void GimmickUnk_022308a8::Unload() {
}

void GimmickUnk_022308a8::Finish() {
    mState = 2;
}

void GimmickUnk_022308a8::Update() {
    VecFx32 d;
    VecFx32 e;
    MtxFx33 m;
    switch (mState) {
    case 0:
        func_ov007_0222ea28(0x28, 8);
        mState++;
        break;
    case 1: {
        if (mUnk_3c && mUnk_3c->mUnk_823 == 1 && !data_020d433c.mBattlePlayers->mBattleTarget->mUnk_1da &&
            data_020d433c.mBattlePlayers->mBattleTarget->mUnk_1a4->mPosition.y <= 0x899a &&
            data_020d433c.mBattlePlayers->mBattleTarget->mUnk_44 <= 0x5dc && data_020d433c.mBattlePlayers->mBattleTarget->mUnk_38 <= 3) {
            BattlePlayer *p = data_020d433c.mBattlePlayers->Current();
            BattleRecord *r = &p->mRecords[p->mRecord];
            if (!mUnk_54) {
                SoundManager::Get().StartSeqArc(0x64);
                mUnk_54 = 1;
            }
            data_020d433c.mUnk_14 &= 8;
            if (data_020d433c.mUnk_14) {
                return;
            }
            if (r->mAttribute == 1) {
                return;
            }
            m = data_0220e848;
            VEC_Subtract(&data_020d433c.mBattlePlayers->mBattleTarget->mUnk_1a4->mPosition, &mPosition, &d);
            d.y = 0;
            e.x = d.x;
            e.y = 0;
            e.z = d.z;
            VEC_Normalize(&e, &e);
            e.x = FX_Mul(e.x, 0x8000);
            e.y = FX_Mul(e.y, 0x8000);
            e.z = FX_Mul(e.z, 0x8000);
            VEC_Subtract(&d, &e, &e);
            func_01ffcfd8(&m, 0x71c);
            func_01ff84d0(&e, &m, &e);
            VEC_Subtract(&e, &d, &mUnk_48);
            VEC_Normalize(&mUnk_48, &mUnk_48);
            mUnk_48.x = FX_Mul(mUnk_48.x, 0x19a);
            mUnk_48.y = FX_Mul(mUnk_48.y, 0x19a);
            mUnk_48.z = FX_Mul(mUnk_48.z, 0x19a);
            VEC_Add(&data_020d433c.mBattlePlayers->mBattleTarget->mUnk_a0, &mUnk_48,
                          &data_020d433c.mBattlePlayers->mBattleTarget->mUnk_a0);
        } else if (mUnk_54) {
            func_02024764(&SoundManager::Get(), 0x64, 10);
            mUnk_54 = 0;
        }
        break;
    }
    case 2:
        mFinished = 1;
        break;
    }
}
