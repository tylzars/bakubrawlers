#include "Battle/GimmickTypes.hpp"
#include <nitro/fx.h>
#include "Graphics/Camera.hpp"
#include "System/SoundManager.hpp"

static VecFx32 data_ov007_02285a98 = {0, 0xf000, 0xa000};

GimmickUnk_02274830::GimmickUnk_02274830(u8 id)
    : Gimmick(id, 0) {
    mUnk_46 = 0;
    mState  = 0;
    mUnk_3a = 0x1f;
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
        func_ov007_0222ea28(0x1f, 8);
        mState++;
        break;
    case 1: {
        data_020d433c.mUnk_14 &= 8;
        if (data_020d433c.mUnk_14) {
            return;
        }
        BattlePlayerList *list;
        BattleTarget *target;
        if (!(mUnk_3c && mUnk_3c->mUnk_823 && (target = (list = data_020d433c.mBattlePlayers)->mUnk_298))) {
            return;
        }
        if (target->mUnk_1a4->mPosition.y > 0x899a) {
            return;
        }
        if (target->mUnk_1da) {
            return;
        }
        if (target->mUnk_38 > 3) {
            return;
        }
        u8 idx          = list->mUnk_28a;
        BattlePlayer *p = list->mPlayers[idx];
        u8 rec          = p->mRecord;
        if (p->mRecords[rec].mUnk_09 == 0) {
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
        SoundManager::Get().func_020246f8(0x60);
        v = data_020d433c.mBattlePlayers->mUnk_298->mUnk_64;
        v.y = 0;
        VEC_Normalize(&v, &v);
        v.x = FX_Mul(v.x, 0x2000);
        v.y = FX_Mul(v.y, 0x2000);
        v.z = FX_Mul(v.z, 0x2000);
        v.y = 0x2000;
        func_ov007_0225c960(data_020d433c.mBattlePlayers->mUnk_298, &v, 0);
        CameraManager::Get();
        MtxFx43 mtx    = data_020c1cf4;
        VecFx32 camPos = data_020d433c.mBattlePlayers->mUnk_298->mUnk_1a4->mPosition;
        VecFx32 w      = data_ov007_02285a98;
        u16 handle     = func_0202bd40(data_020d433c.mEffects, 1, 4, 0, 0);
        if (handle) {
            mtx._[11] = 0;
            mtx._[10] = 0;
            mtx._[9]  = 0;
            MTX_MultVec43(&w, &mtx, &w);
            VEC_Add(&camPos, &w, &camPos);
            func_01ffcf64(&mtx, &camPos);
            func_0202bf60(data_020d433c.mEffects, handle, &mtx);
        }
        handle = func_0202bd40(data_020d433c.mEffects, 1, 0xd, 0, 1);
        if (handle) {
            CameraManager::Get();
            MtxFx43 mtx2    = data_020c1cf4;
            VecFx32 camPos2 = data_020d433c.mBattlePlayers->mUnk_298->mUnk_1a4->mPosition;
            mtx2._[11]      = 0;
            mtx2._[10]      = 0;
            mtx2._[9]       = 0;
            func_01ffcf64(&mtx2, &camPos2);
            func_0202bf60(data_020d433c.mEffects, handle, &mtx2);
        }
        mUnk_46 = 0x78;
        mState++;
        break;
    }
    case 2:
        data_020d433c.mUnk_14 &= 8;
        if (data_020d433c.mUnk_14) {
            return;
        }
        mUnk_46--;
        if (mUnk_46 == 0) {
            mState = 1;
        }
        break;
    case 3:
        mFinished = 1;
        break;
    }
}
