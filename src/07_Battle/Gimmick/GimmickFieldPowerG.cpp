#include "Battle/GimmickTypes.hpp"
#include <nitro/fx.h>
#include "Graphics/Camera.hpp"
#include "System/FileSystem.hpp"
#include "System/SoundManager.hpp"

static const u8 data_ov007_02279414[12] = {3, 4, 5, 0, 0, 0, 0x55, 0x15, 0, 0, 0, 0};
static VecFx32 data_ov007_02279420     = {0, 0xf000, -0xa000};

static ModelResource *sResource;

GimmickFieldPowerG::GimmickFieldPowerG(u8 a)
    : Gimmick(a, 0) {
    mState  = 0;
    mUnk_3a = 2;
}

GimmickFieldPowerG::~GimmickFieldPowerG() {
    Unload();
}

void GimmickFieldPowerG::Load() {
    if (!sResource) {
        sResource = new ModelResource("fieldpower_g_0.nsbmd", 0, "fieldpower_g_0.nsbtx", 1);
    }
    ChangeDir("/Game/Gimmick");
    sResource->func_020306e4(1);
    mModel = sResource->mModel->func_02028b78(0);
    ChangeDir("/");
}

void GimmickFieldPowerG::Unload() {
    if (mModel) {
        ModelInstance *m = mModel;
        m->mUnk_68       = 1;
        delete m;
        mModel = 0;
    }
    if (sResource && sResource->func_020307e8()) {
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
        func_ov007_0222ea28(0x5c, 0x20);
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
        if (data_020d433c.mUnk_85 == 0xc) {
            data_020d433c.mBattlePlayers->mUnk_2b0 = 1;
        }
        func_ov007_02271d78(data_020d433c.mUnk_0c, mUnk_3c);
        BattlePlayer *p = data_020d433c.mBattlePlayers->Current();
        func_ov007_0226a380(p, p->mRecord, 0x1e);
        p->mUnk_fd++;
        if (p->mUnk_fd > 0x63) {
            p->mUnk_fd = 0x63;
        }
        SoundManager::Get().func_020246f8(0x35);
        CameraManager::Get();
        MtxFx43 mtx = data_020c1cf4;
        VecFx32 camPos = data_020d433c.mBattlePlayers->mUnk_298->mUnk_1a4->mPos;
        VecFx32 v      = data_ov007_02279420;
        u16 handle     = func_0202bd40(data_020d433c.mEffects, 1, 3, 0, 0);
        if (handle) {
            mtx._[11] = 0;
            mtx._[10] = 0;
            mtx._[9]  = 0;
            func_01ff8c94(&v, &mtx, &v);
            VEC_Add(&camPos, &v, &camPos);
            func_01ffcf64(&mtx, &camPos);
            func_0202bf60(data_020d433c.mEffects, handle, &mtx);
        }
        handle = func_0202bd40(data_020d433c.mEffects, 1, 0xd, 0, 1);
        if (handle) {
            VecFx32 d;
            VecFx32 camPos2 = data_020d433c.mBattlePlayers->mUnk_298->mUnk_1a4->mPos;
            CameraManager::Get();
            MtxFx43 mtx2 = data_020c1cf4;
            func_01ffcf64(&mtx2, &mPosition);
            BattleVecHolder8 *h = data_020d433c.mBattlePlayers->mUnk_298->mUnk_174;
            d.x                 = h->mPos.x;
            d.y                 = h->mPos.y;
            d.z                 = h->mPos.z;
            VEC_Subtract(&d, &camPos2, &d);
            VEC_Normalize(&d, &d);
            d.x = FX_Mul(d.x, 0x1e000);
            d.y = FX_Mul(d.y, 0x1e000);
            d.z = FX_Mul(d.z, 0x1e000);
            VEC_Add(&d, &data_020d433c.mBattlePlayers->mUnk_298->mUnk_1a4->mPos, &d);
            func_01ffcf64(&mtx2, &d);
            func_0202bf60(data_020d433c.mEffects, handle, &mtx2);
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
            limit = 3;
        } else {
            limit = data_ov007_02279414[players - 2];
        }
        if (mUnk_44 < limit) {
            break;
        }
        func_ov007_0222ea28(0x5c, 0x20);
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
