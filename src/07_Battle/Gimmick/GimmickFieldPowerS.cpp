#include "Battle/GimmickTypes.hpp"
#include <nitro/fx.h>
#include "Graphics/Camera.hpp"
#include "System/FileSystem.hpp"
#include "System/SoundManager.hpp"

static ModelResource *sResources[2];

static const u8 sPowerSPartnerTurnLimits[4]  = {5, 3, 0, 0}; // [mVariant], used when gGameData.mHasPartners
static const u8 sPowerSTurnLimits[2][3] = {{5, 7, 9}, {5, 7, 9}};
static const VecFx32 sPowerSEffectOffset      = {0, 0, 0x1e000};
static const VecFx32 sPowerSEffectScale      = {0x800, 0x800, 0x800}; // applied only when mVariant == 1


GimmickFieldPowerS::GimmickFieldPowerS(u8 id, u8 variant)
    : Gimmick(id, 0) {
    mVariant = variant;
    mState   = 0;
    mType  = GIMMICK_TYPE_FIELD_POWER_S;
}

GimmickFieldPowerS::~GimmickFieldPowerS() {
    Unload();
}

void GimmickFieldPowerS::Load() {
    if (!sResources[mVariant]) {
        switch (mVariant) {
        case 0:
            sResources[mVariant] = new ModelResource("fieldpower_s_0.nsbmd", 0, "fieldpower_s_0.nsbtx", 1);
            break;
        case 1:
            sResources[mVariant] = new ModelResource("fieldpower_s_1.nsbmd", 0, "fieldpower_s_1.nsbtx", 1);
            break;
        default:
            return;
        }
    }
    ChangeDir("/Game/Gimmick");
    if (sResources[mVariant]) {
        sResources[mVariant]->AddRef(1);
    }
    mModel = sResources[mVariant]->mModel->CreateInstance(0);
    ChangeDir("/");
}

void GimmickFieldPowerS::Unload() {
    if (mModel) {
        ModelInstance *m = mModel;
        m->mOwnFlags       = MODEL_OWN_ANIM;
        delete m;
        mModel = 0;
    }
    if (sResources[0] && sResources[0]->ReleaseRef()) {
        ModelResource *r = sResources[0];
        delete r;
        sResources[0] = 0;
    }
    if (sResources[1] && sResources[1]->ReleaseRef()) {
        ModelResource *r = sResources[1];
        delete r;
        sResources[1] = 0;
    }
}

void GimmickFieldPowerS::Finish() {
    mState = 3;
}

void GimmickFieldPowerS::Update() {
    switch (mState) {
    case 0:
        LoadVariantCollision();
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
        switch (mVariant) {
        case 0: {
            BattlePlayer *p = gGameData.mBattlePlayers->Current();
            int v           = p->mSpGauge + 0xbe;
            if (v >= 0xbe) {
                p->mSpGauge = 0xbe;
            } else {
                p->mSpGauge = v;
            }
            BattlePlayer *p2 = gGameData.mBattlePlayers->Current();
            p2->mFieldPowerSHitCount++;
            if (p2->mFieldPowerSHitCount > 0x63) {
                p2->mFieldPowerSHitCount = 0x63;
            }
            break;
        }
        case 1: {
            BattlePlayer *p = gGameData.mBattlePlayers->Current();
            int v           = p->mSpGauge + 0xa;
            if (v < 0xbe) {
                p->mSpGauge = v;
            } else {
                p->mSpGauge = 0xbe;
            }
            break;
        }
        }
        SoundManager::Get().StartSeqArc(0x34);
        u8 handle = StartEffect(gGameData.mEffects, 1, 0xc, 0, 1);
        if (handle) {
            CameraManager::Get();
            MtxFx43 mtx    = data_020c1cf4;
            MtxFx43 rot    = gIdentityMtx43;
            VecFx32 camPos = gGameData.mBattlePlayers->mBattleTarget->mBakugan->mPosition;
            VecFx32 v      = sPowerSEffectOffset;
            VecFx32 w      = sPowerSEffectScale;
            mtx._[11]      = 0;
            mtx._[10]      = 0;
            mtx._[9]       = 0;
            MTX_MultVec43(&v, &mtx, &v);
            VEC_Add(&camPos, &v, &camPos);
            MTX_ApplyRotX43(&rot, 0x1555);
            MTX_Concat43(&rot, &mtx, &mtx);
            MTX_SetTranslation43(&mtx, &camPos);
            SetEffectMatrix(gGameData.mEffects, handle, &mtx);
            if (mVariant == 1) {
                SetEffectScale(gGameData.mEffects, handle, &w);
            }
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
            limit = sPowerSPartnerTurnLimits[mVariant];
        } else {
            limit = sPowerSTurnLimits[mVariant][players - 2];
        }
        if (mTurnCount < limit) {
            break;
        }
        LoadVariantCollision();
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

void GimmickFieldPowerS::LoadVariantCollision() {
    switch (mVariant) {
    case 0:
        LoadCollision(0x5b, COLLISION_GROUP_GIMMICK_HIT_BY_SP5);
        break;
    case 1:
        LoadCollision(0x61, COLLISION_GROUP_GIMMICK_HIT_BY_SP5);
        break;
    }
}
