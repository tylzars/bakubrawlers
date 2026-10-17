#include "Battle/GimmickTypes.hpp"
#include <nitro/fx.h>
#include "Graphics/Camera.hpp"
#include "System/FileSystem.hpp"
#include "System/SoundManager.hpp"

static const u8 data_ov007_02279410[4] = {3, 4, 5, 0};

static ModelResource *sResource;

GimmickExePoint::GimmickExePoint(u8 a)
    : Gimmick(a, 0) {
    mState  = 0;
    mUnk_3a = 1;
}

GimmickExePoint::~GimmickExePoint() {
    Unload();
}

void GimmickExePoint::Load() {
    if (!sResource) {
        sResource = new ModelResource("exepoint_00.nsbmd", 0, "exepoint_00.nsbtx", 1);
    }
    ChangeDir("/Game/Gimmick");
    sResource->AddRef(1);
    mModel = sResource->mModel->CreateInstance(0);
    ChangeDir("/");
}

void GimmickExePoint::Unload() {
    if (mModel) {
        ModelInstance *m = mModel;
        m->mOwnFlags       = 1;
        delete m;
        mModel = 0;
    }
    if (sResource && sResource->ReleaseRef()) {
        ModelResource *r = sResource;
        delete r;
        sResource = 0;
    }
}

void GimmickExePoint::Finish() {
    mState = 3;
}

void GimmickExePoint::Update() {
    switch (mState) {
    case 0:
        func_ov007_0222ea28(0x5f, 0x20);
        mState++;
        break;
    case 1: {
        if (mModel && (int)data_020d433c.mGimmicks->mState >= 2) {
            func_020526bc();
        }
        if (!(mUnk_3c && mUnk_3c->mUnk_823)) {
            return;
        }
        if (!data_020d433c.mBattlePlayers->mUnk_298) {
            return;
        }
        mState++;
        func_ov007_02271d78(data_020d433c.mUnk_0c, mUnk_3c);
        BattleRecord *rec = &data_020d433c.mBattlePlayers->Current()->mRecords[data_020d433c.mBattlePlayers->Current()->mRecord];
        if ((int)rec->mGauge < 0xfd7) {
            rec->mGauge += 0x28;
        }
        SoundManager::Get().StartSeqArc(0x36);
        u8 handle = StartEffect(data_020d433c.mEffects, 1, 0xe, 0, 1);
        if (handle) {
            VecFx32 d;
            VecFx32 camPos = data_020d433c.mBattlePlayers->mUnk_298->mUnk_1a4->mPosition;
            CameraManager::Get();
            MtxFx43 mtx = data_020c1cf4;
            func_01ffcf64(&mtx, &mPosition);
            Camera *h = data_020d433c.mBattlePlayers->mUnk_298->mUnk_174;
            d.x                 = h->mPosition.x;
            d.y                 = h->mPosition.y;
            d.z                 = h->mPosition.z;
            VEC_Subtract(&d, &camPos, &d);
            VEC_Normalize(&d, &d);
            d.x = FX_Mul(d.x, 0x1e000);
            d.y = FX_Mul(d.y, 0x1e000);
            d.z = FX_Mul(d.z, 0x1e000);
            VEC_Add(&d, &data_020d433c.mBattlePlayers->mUnk_298->mUnk_1a4->mPosition, &d);
            func_01ffcf64(&mtx, &d);
            SetEffectMatrix(data_020d433c.mEffects, handle, &mtx);
        }
        mUnk_40 = 1;
        mUnk_3c = 0;
        break;
    }
    case 2: {
        u8 players = data_020d433c.mPlayerCount;
        if (players < 2 || players > 4) {
            break;
        }
        u32 limit;
        if (data_020d433c.mUnk_98) {
            limit = 2;
        } else {
            limit = data_ov007_02279410[players - 2];
        }
        if (mUnk_44 < limit) {
            break;
        }
        func_ov007_0222ea28(0x5f, 0x20);
        mUnk_40 = 0;
        mState  = 1;
        mUnk_44 = 0;
        break;
    }
    case 3:
        mFinished = 1;
        break;
    }
}
