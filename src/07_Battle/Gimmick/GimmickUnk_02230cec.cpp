#include "Battle/GimmickTypes.hpp"

GimmickUnk_02230cec::GimmickUnk_02230cec(u8 id)
    : Gimmick(id, 0) {
    mState  = 0;
    mType = 0x3d;
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
        LoadCollision(0x3d, COLLISION_GROUP_GIMMICK);
        mState++;
        break;
    case 1: {
        if (!(mCollision && mCollision->mHitCount)) {
            return;
        }
        BattleTarget *target = gGameData.mBattlePlayers->mBattleTarget;
        if (target->mUnk_1dd == 4 || !target) {
            return;
        }
        if (target->mBakugan->mPosition.y > 0x899a) {
            return;
        }
        if (target->mOnMarkerCell) {
            return;
        }
        if (target->mAirFrames > 3) {
            return;
        }
        if (target->mMoveFrames < 5) {
            return;
        }
        BattlePlayer *p = gGameData.mBattlePlayers->Current();
        BattleRecord *r = &p->mRecords[p->mRecord];
        VecFx32 v;
        v.x = target->mVelocity.x;
        v.y = target->mVelocity.y;
        v.z = target->mVelocity.z;
        gGameData.mFlags &= 8;
        if (!gGameData.mFlags && r->mAttribute != 2) {
            v.x = FX_Mul(v.x, 0x1000);
            v.y = FX_Mul(v.y, 0x1000);
            v.z = FX_Mul(v.z, 0x1000);
        }
        v.y = 0;
        SetTargetVelocity(gGameData.mBattlePlayers->mBattleTarget, &v, 0);
        break;
    }
    case 2:
        mFinished = 1;
        break;
    }
}
