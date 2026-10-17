#include "Battle/GimmickTypes.hpp"
#include <nitro/fx.h>
#include "System/SoundManager.hpp"

extern "C" void MTX_ApplyRotY33(MtxFx33 *m, u16 angle);
extern MtxFx33 data_0220e848;
extern "C" void StopSeqArc(SoundManager *manager, u32 a, u32 b);

GimmickUnk_022308a8::GimmickUnk_022308a8(u8 id)
    : Gimmick(id, 0) {
    mSoundStarted = 0;
    mState  = 0;
    mType = 0x28;
    mPullPerFrame.x = 0;
    mPullPerFrame.y = 0;
    mPullPerFrame.z = 0;
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
        LoadCollision(0x28, COLLISION_GROUP_GIMMICK);
        mState++;
        break;
    case 1: {
        if (mCollision && mCollision->mHitCount == 1 && !data_020d433c.mBattlePlayers->mBattleTarget->mOnMarkerCell &&
            data_020d433c.mBattlePlayers->mBattleTarget->mBakugan->mPosition.y <= 0x899a &&
            data_020d433c.mBattlePlayers->mBattleTarget->mUnk_44 <= 0x5dc && data_020d433c.mBattlePlayers->mBattleTarget->mAirFrames <= 3) {
            BattlePlayer *p = data_020d433c.mBattlePlayers->Current();
            BattleRecord *r = &p->mRecords[p->mRecord];
            if (!mSoundStarted) {
                SoundManager::Get().StartSeqArc(0x64);
                mSoundStarted = 1;
            }
            data_020d433c.mUnk_14 &= 8;
            if (data_020d433c.mUnk_14) {
                return;
            }
            if (r->mAttribute == 1) {
                return;
            }
            m = data_0220e848;
            VEC_Subtract(&data_020d433c.mBattlePlayers->mBattleTarget->mBakugan->mPosition, &mPosition, &d);
            d.y = 0;
            e.x = d.x;
            e.y = 0;
            e.z = d.z;
            VEC_Normalize(&e, &e);
            e.x = FX_Mul(e.x, 0x8000);
            e.y = FX_Mul(e.y, 0x8000);
            e.z = FX_Mul(e.z, 0x8000);
            VEC_Subtract(&d, &e, &e);
            MTX_ApplyRotY33(&m, 0x71c);
            MTX_MultVec33(&e, &m, &e);
            VEC_Subtract(&e, &d, &mPullPerFrame);
            VEC_Normalize(&mPullPerFrame, &mPullPerFrame);
            mPullPerFrame.x = FX_Mul(mPullPerFrame.x, 0x19a);
            mPullPerFrame.y = FX_Mul(mPullPerFrame.y, 0x19a);
            mPullPerFrame.z = FX_Mul(mPullPerFrame.z, 0x19a);
            VEC_Add(&data_020d433c.mBattlePlayers->mBattleTarget->mBaseVelocity, &mPullPerFrame,
                          &data_020d433c.mBattlePlayers->mBattleTarget->mBaseVelocity);
        } else if (mSoundStarted) {
            StopSeqArc(&SoundManager::Get(), 0x64, 10);
            mSoundStarted = 0;
        }
        break;
    }
    case 2:
        mFinished = 1;
        break;
    }
}
