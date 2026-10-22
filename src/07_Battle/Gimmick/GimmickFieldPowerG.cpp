#include "Battle/GimmickTypes.hpp"
#include <nitro/fx.h>
#include "Graphics/Camera.hpp"
#include "System/FileSystem.hpp"
#include "System/SoundManager.hpp"

static const VecFx32 sPowerGEffectOffset     = {0, 0xf000, -0xa000};
static const u8 sPowerGTurnLimits[12] = {3, 4, 5, 0, 0, 0, 0x55, 0x15, 0, 0, 0, 0}; // [players - 2]; bytes 3-11 unread

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

void GimmickFieldPowerG::Finish() {
    mState = 3;
}

void GimmickFieldPowerG::Update() {
    switch (mState) {
    case 0:
        LoadCollision(0x5c, COLLISION_GROUP_GIMMICK_HIT_BY_SP5);
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
        if (gGameData.mTutorialId == 0xc) {
            gGameData.mBattlePlayers->mUnk_2b0 = 1;
        }
        RemoveCollisionSet(gGameData.mGimmickCollision, mCollision);
        BattlePlayer *p = gGameData.mBattlePlayers->Current();
        func_ov007_0226a380(p, p->mRecord, 0x1e);
        p->mFieldPowerGHitCount++;
        if (p->mFieldPowerGHitCount > 0x63) {
            p->mFieldPowerGHitCount = 0x63;
        }
        SoundManager::Get().StartSeqArc(0x35);
        CameraManager::Get();
        MtxFx43 mtx = data_020c1cf4;
        VecFx32 camPos = gGameData.mBattlePlayers->mBattleTarget->mBakugan->mPosition;
        VecFx32 v      = sPowerGEffectOffset;
        u16 handle     = StartEffect(gGameData.mEffects, 1, 3, 0, 0);
        if (handle) {
            mtx._[11] = 0;
            mtx._[10] = 0;
            mtx._[9]  = 0;
            MTX_MultVec43(&v, &mtx, &v);
            VEC_Add(&camPos, &v, &camPos);
            MTX_SetTranslation43(&mtx, &camPos);
            SetEffectMatrix(gGameData.mEffects, handle, &mtx);
        }
        handle = StartEffect(gGameData.mEffects, 1, 0xd, 0, 1);
        if (handle) {
            VecFx32 d;
            VecFx32 camPos2 = gGameData.mBattlePlayers->mBattleTarget->mBakugan->mPosition;
            CameraManager::Get();
            MtxFx43 mtx2 = data_020c1cf4;
            MTX_SetTranslation43(&mtx2, &mPosition);
            Camera *h = gGameData.mBattlePlayers->mBattleTarget->mCamera;
            d.x                 = h->mPosition.x;
            d.y                 = h->mPosition.y;
            d.z                 = h->mPosition.z;
            VEC_Subtract(&d, &camPos2, &d);
            VEC_Normalize(&d, &d);
            d.x = FX_Mul(d.x, 0x1e000);
            d.y = FX_Mul(d.y, 0x1e000);
            d.z = FX_Mul(d.z, 0x1e000);
            VEC_Add(&d, &gGameData.mBattlePlayers->mBattleTarget->mBakugan->mPosition, &d);
            MTX_SetTranslation43(&mtx2, &d);
            SetEffectMatrix(gGameData.mEffects, handle, &mtx2);
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
            limit = 3;
        } else {
            limit = sPowerGTurnLimits[players - 2];
        }
        if (mTurnCount < limit) {
            break;
        }
        LoadCollision(0x5c, COLLISION_GROUP_GIMMICK_HIT_BY_SP5);
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
