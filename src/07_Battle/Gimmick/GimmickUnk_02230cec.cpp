#include "Battle/GimmickTypes.hpp"

GimmickUnk_02230cec::GimmickUnk_02230cec(u8 id)
    : Gimmick(id, 0) {
    mState  = 0;
    mUnk_3a = 0x3d;
}

GimmickUnk_02230cec::~GimmickUnk_02230cec() {
    Unload();
}

void GimmickUnk_02230cec::Load() {
}

void GimmickUnk_02230cec::Unload() {
}

void GimmickUnk_02230cec::Finish() {
    mState = 2;
}

void GimmickUnk_02230cec::Update() {
    switch (mState) {
    case 0:
        func_ov007_0222ea28(0x3d, 8);
        mState++;
        break;
    case 1: {
        if (!(mUnk_3c && mUnk_3c->mUnk_823)) {
            return;
        }
        BattleTarget *target = data_020d433c.mBattlePlayers->mUnk_298;
        if (target->mUnk_1dd == 4 || !target) {
            return;
        }
        if (target->mUnk_1a4->mPos.y > 0x899a) {
            return;
        }
        if (target->mUnk_1da) {
            return;
        }
        if (target->mUnk_38 > 3) {
            return;
        }
        if (target->mUnk_4c < 5) {
            return;
        }
        BattlePlayer *p = data_020d433c.mBattlePlayers->Current();
        BattleRecord *r = &p->mRecords[p->mRecord];
        VecFx32 v;
        v.x = target->mUnk_64.x;
        v.y = target->mUnk_64.y;
        v.z = target->mUnk_64.z;
        data_020d433c.mUnk_14 &= 8;
        if (!data_020d433c.mUnk_14 && r->mUnk_09 != 2) {
            v.x = FX_Mul(v.x, 0x1000);
            v.y = FX_Mul(v.y, 0x1000);
            v.z = FX_Mul(v.z, 0x1000);
        }
        v.y = 0;
        func_ov007_0225c960(data_020d433c.mBattlePlayers->mUnk_298, &v, 0);
        break;
    }
    case 2:
        mFinished = 1;
        break;
    }
}
