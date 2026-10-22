#include "Battle/GimmickTypes.hpp"
#include <nitro/fx.h>
#include "Graphics/Camera.hpp"
#include "System/FileSystem.hpp"
#include "System/SoundManager.hpp"

static const u8 sExePointTurnLimits[4] = {3, 4, 5, 0};

static ModelResource *sResource;

GimmickExePoint::GimmickExePoint(u8 a)
    : Gimmick(a, 0) {
    mState  = 0;
    mType = GIMMICK_TYPE_EXE_POINT;
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
        m->mOwnFlags       = MODEL_OWN_ANIM;
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
        LoadCollision(0x5f, COLLISION_GROUP_GIMMICK_HIT_BY_SP5);
        mState++;
        break;
    case 1: {
        if (mModel && (int)gGameData.mGimmicks->mState >= 2) {
            DrawInstance();
        }
        if (!(mCollision && mCollision->mHitCount)) {
            return;
        }
        if (!gGameData.mBattlePlayers->mBattleTarget) {
            return;
        }
        mState++;
        RemoveCollisionSet(gGameData.mGimmickCollision, mCollision);
        BattleRecord *rec = &gGameData.mBattlePlayers->Current()->mRecords[gGameData.mBattlePlayers->Current()->mRecord];
        if ((int)rec->mGauge < 0xfd7) {
            rec->mGauge += 0x28;
        }
        SoundManager::Get().StartSeqArc(0x36);
        u8 handle = StartEffect(gGameData.mEffects, 1, 0xe, 0, 1);
        if (handle) {
            VecFx32 d;
            VecFx32 camPos = gGameData.mBattlePlayers->mBattleTarget->mBakugan->mPosition;
            CameraManager::Get();
            MtxFx43 mtx = data_020c1cf4;
            MTX_SetTranslation43(&mtx, &mPosition);
            Camera *h = gGameData.mBattlePlayers->mBattleTarget->mCamera;
            d.x                 = h->mPosition.x;
            d.y                 = h->mPosition.y;
            d.z                 = h->mPosition.z;
            VEC_Subtract(&d, &camPos, &d);
            VEC_Normalize(&d, &d);
            d.x = FX_Mul(d.x, 0x1e000);
            d.y = FX_Mul(d.y, 0x1e000);
            d.z = FX_Mul(d.z, 0x1e000);
            VEC_Add(&d, &gGameData.mBattlePlayers->mBattleTarget->mBakugan->mPosition, &d);
            MTX_SetTranslation43(&mtx, &d);
            SetEffectMatrix(gGameData.mEffects, handle, &mtx);
        }
        mCountingTurns = 1;
        mCollision = 0;
        break;
    }
    case 2: {
        u8 players = gGameData.mPlayerCount;
        if (players < 2 || players > 4) {
            break;
        }
        u32 limit;
        if (gGameData.mHasPartners) {
            limit = 2;
        } else {
            limit = sExePointTurnLimits[players - 2];
        }
        if (mTurnCount < limit) {
            break;
        }
        LoadCollision(0x5f, COLLISION_GROUP_GIMMICK_HIT_BY_SP5);
        mCountingTurns = 0;
        mState  = 1;
        mTurnCount = 0;
        break;
    }
    case 3:
        mFinished = 1;
        break;
    }
}
