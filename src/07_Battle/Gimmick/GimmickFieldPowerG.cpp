#include "Battle/GimmickTypes.hpp"
#include <nitro/fx.h>
#include "Graphics/Camera.hpp"
#include "System/FileSystem.hpp"
#include "System/SoundManager.hpp"

static const VecFx32 data_ov007_02279420     = {0, 0xf000, -0xa000};
static const u8 sPowerGTurnLimits[12] = {3, 4, 5, 0, 0, 0, 0x55, 0x15, 0, 0, 0, 0};

static ModelResource *sResource;

GimmickFieldPowerG::GimmickFieldPowerG(u8 a)
    : Gimmick(a, 0) {
    mState  = 0;
    mType = GIMMICK_TYPE_FIELD_POWER_G;
}

GimmickFieldPowerG::~GimmickFieldPowerG() {
    Unload();
}

void GimmickFieldPowerG::Load() {
    if (!sResource) {
        sResource = new ModelResource("fieldpower_g_0.nsbmd", 0, "fieldpower_g_0.nsbtx", 1);
    }
    ChangeDir("/Game/Gimmick");
    sResource->AddRef(1);
    mModel = sResource->mModel->CreateInstance(0);
    ChangeDir("/");
}

void GimmickFieldPowerG::Unload() {
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

void GimmickFieldPowerG::Finish() {
    mState = 3;
}

void GimmickFieldPowerG::Update() {
    switch (mState) {
    case 0:
        LoadCollision(0x5c, 0x20);
        mState++;
        break;
    case 1: {
        if (mModel && (int)data_020d433c.mGimmicks->mState >= 2) {
            func_020526bc();
        }
        if (!(mCollision && mCollision->mHitCount)) {
            return;
        }
        if (!data_020d433c.mBattlePlayers->mBattleTarget) {
            return;
        }
        mState++;
        if (data_020d433c.mTutorialId == 0xc) {
            data_020d433c.mBattlePlayers->mUnk_2b0 = 1;
        }
        RemoveCollisionSet(data_020d433c.mGimmickCollision, mCollision);
        BattlePlayer *p = data_020d433c.mBattlePlayers->Current();
        func_ov007_0226a380(p, p->mRecord, 0x1e);
        p->mUnk_fd++;
        if (p->mUnk_fd > 0x63) {
            p->mUnk_fd = 0x63;
        }
        SoundManager::Get().StartSeqArc(0x35);
        CameraManager::Get();
        MtxFx43 mtx = data_020c1cf4;
        VecFx32 camPos = data_020d433c.mBattlePlayers->mBattleTarget->mBakugan->mPosition;
        VecFx32 v      = data_ov007_02279420;
        u16 handle     = StartEffect(data_020d433c.mEffects, 1, 3, 0, 0);
        if (handle) {
            mtx._[11] = 0;
            mtx._[10] = 0;
            mtx._[9]  = 0;
            MTX_MultVec43(&v, &mtx, &v);
            VEC_Add(&camPos, &v, &camPos);
            func_01ffcf64(&mtx, &camPos);
            SetEffectMatrix(data_020d433c.mEffects, handle, &mtx);
        }
        handle = StartEffect(data_020d433c.mEffects, 1, 0xd, 0, 1);
        if (handle) {
            VecFx32 d;
            VecFx32 camPos2 = data_020d433c.mBattlePlayers->mBattleTarget->mBakugan->mPosition;
            CameraManager::Get();
            MtxFx43 mtx2 = data_020c1cf4;
            func_01ffcf64(&mtx2, &mPosition);
            Camera *h = data_020d433c.mBattlePlayers->mBattleTarget->mCamera;
            d.x                 = h->mPosition.x;
            d.y                 = h->mPosition.y;
            d.z                 = h->mPosition.z;
            VEC_Subtract(&d, &camPos2, &d);
            VEC_Normalize(&d, &d);
            d.x = FX_Mul(d.x, 0x1e000);
            d.y = FX_Mul(d.y, 0x1e000);
            d.z = FX_Mul(d.z, 0x1e000);
            VEC_Add(&d, &data_020d433c.mBattlePlayers->mBattleTarget->mBakugan->mPosition, &d);
            func_01ffcf64(&mtx2, &d);
            SetEffectMatrix(data_020d433c.mEffects, handle, &mtx2);
        }
        mCountingTurns = 1;
        mCollision = 0;
        break;
    }
    case 2: {
        u8 players = data_020d433c.mPlayerCount;
        if (players < 2 || players > 4) {
            break;
        }
        u32 limit;
        if (data_020d433c.mHasPartners) {
            limit = 3;
        } else {
            limit = sPowerGTurnLimits[players - 2];
        }
        if (mTurnCount < limit) {
            break;
        }
        LoadCollision(0x5c, 0x20);
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
