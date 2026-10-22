#include "Battle/GimmickTypes.hpp"
#include <nitro/fx.h>
#include "Graphics/Camera.hpp"
#include "System/SoundManager.hpp"

static const VecFx32 sEffectOffset = {0, 0xf000, 0xa000};

GimmickUnk_02274830::GimmickUnk_02274830(u8 id)
    : Gimmick(id, 0) {
    mTimer = 0;
    mState  = 0;
    mType = 0x1f;
}

GimmickUnk_02274830::~GimmickUnk_02274830() {
    Unload();
}

void GimmickUnk_02274830::Load() {
}

void GimmickUnk_02274830::Unload() {
}

void GimmickUnk_02274830::Finish() {
    mState = 3;
}

void GimmickUnk_02274830::Update() {
    switch (mState) {
    case 0:
        LoadCollision(0x1f, COLLISION_GROUP_GIMMICK);
        mState++;
        break;
    case 1: {
        gGameData.mFlags &= 8;
        if (gGameData.mFlags) {
            return;
        }
        BattlePlayerList *list;
        BattleTarget *target;
        if (!(mCollision && mCollision->mHitCount && (target = (list = gGameData.mBattlePlayers)->mBattleTarget))) {
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
        u8 idx          = list->mTurnPlayer;
        BattlePlayer *p = list->mPlayers[idx];
        u8 rec          = p->mRecord;
        if (p->mRecords[rec].mAttribute == 0) {
            return;
        }
        VecFx32 v = {0, 0, 0};
        p = list->mPlayers[idx];
        int gauge = p->mRecords[rec].mUnk_04;
        if (gauge > 10) {
            p->mRecords[rec].mUnk_04 = gauge - 10;
        } else {
            int d                    = (u8)(10 - gauge) + 10;
            BattleRecord *r2 = &p->mRecords[rec];
            r2->mUnk_04 = 10;
            func_ov007_0226a380(p, rec, (s16)-d);
        }
        SoundManager::Get().StartSeqArc(0x60);
        v = gGameData.mBattlePlayers->mBattleTarget->mVelocity;
        v.y = 0;
        VEC_Normalize(&v, &v);
        v.x = FX_Mul(v.x, 0x2000);
        v.y = FX_Mul(v.y, 0x2000);
        v.z = FX_Mul(v.z, 0x2000);
        v.y = 0x2000;
        SetTargetVelocity(gGameData.mBattlePlayers->mBattleTarget, &v, 0);
        CameraManager::Get();
        MtxFx43 mtx    = data_020c1cf4;
        VecFx32 camPos = gGameData.mBattlePlayers->mBattleTarget->mBakugan->mPosition;
        VecFx32 w      = sEffectOffset;
        u16 handle     = StartEffect(gGameData.mEffects, 1, 4, 0, 0);
        if (handle) {
            mtx._[11] = 0;
            mtx._[10] = 0;
            mtx._[9]  = 0;
            MTX_MultVec43(&w, &mtx, &w);
            VEC_Add(&camPos, &w, &camPos);
            MTX_SetTranslation43(&mtx, &camPos);
            SetEffectMatrix(gGameData.mEffects, handle, &mtx);
        }
        handle = StartEffect(gGameData.mEffects, 1, 0xd, 0, 1);
        if (handle) {
            CameraManager::Get();
            MtxFx43 mtx2    = data_020c1cf4;
            VecFx32 camPos2 = gGameData.mBattlePlayers->mBattleTarget->mBakugan->mPosition;
            mtx2._[11]      = 0;
            mtx2._[10]      = 0;
            mtx2._[9]       = 0;
            MTX_SetTranslation43(&mtx2, &camPos2);
            SetEffectMatrix(gGameData.mEffects, handle, &mtx2);
        }
        mTimer = 0x78;
        mState++;
        break;
    }
    case 2:
        gGameData.mFlags &= 8;
        if (gGameData.mFlags) {
            return;
        }
        mTimer--;
        if (mTimer == 0) {
            mState = 1;
        }
        break;
    case 3:
        mFinished = 1;
        break;
    }
}
