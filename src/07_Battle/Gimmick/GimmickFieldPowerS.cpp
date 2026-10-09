#include "Battle/GimmickTypes.hpp"
#include <nitro/fx.h>
#include "Graphics/Camera.hpp"
#include "System/FileSystem.hpp"
#include "System/SoundManager.hpp"

static ModelResource *sResources[2];

static const u8 data_ov007_0227942c[4]  = {5, 3, 0, 0};
static const u8 data_ov007_02279430[2][3] = {{5, 7, 9}, {5, 7, 9}};
static const VecFx32 data_ov007_02279438      = {0, 0, 0x1e000};
static const VecFx32 data_ov007_02279444      = {0x800, 0x800, 0x800};


GimmickFieldPowerS::GimmickFieldPowerS(u8 id, u8 variant)
    : Gimmick(id, 0) {
    mVariant = variant;
    mState   = 0;
    mUnk_3a  = 3;
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
        sResources[mVariant]->func_020306e4(1);
    }
    mModel = sResources[mVariant]->mModel->func_02028b78(0);
    ChangeDir("/");
}

void GimmickFieldPowerS::Unload() {
    if (mModel) {
        ModelInstance *m = mModel;
        m->mUnk_68       = 1;
        delete m;
        mModel = 0;
    }
    if (sResources[0] && sResources[0]->func_020307e8()) {
        ModelResource *r = sResources[0];
        delete r;
        sResources[0] = 0;
    }
    if (sResources[1] && sResources[1]->func_020307e8()) {
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
        func_ov007_0222f1ac();
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
        switch (mVariant) {
        case 0: {
            BattlePlayer *p = data_020d433c.mBattlePlayers->Current();
            int v           = p->mUnk_f3 + 0xbe;
            if (v >= 0xbe) {
                p->mUnk_f3 = 0xbe;
            } else {
                p->mUnk_f3 = v;
            }
            BattlePlayer *p2 = data_020d433c.mBattlePlayers->Current();
            p2->mUnk_fc++;
            if (p2->mUnk_fc > 0x63) {
                p2->mUnk_fc = 0x63;
            }
            break;
        }
        case 1: {
            BattlePlayer *p = data_020d433c.mBattlePlayers->Current();
            int v           = p->mUnk_f3 + 0xa;
            if (v < 0xbe) {
                p->mUnk_f3 = v;
            } else {
                p->mUnk_f3 = 0xbe;
            }
            break;
        }
        }
        SoundManager::Get().func_020246f8(0x34);
        u8 handle = func_0202bd40(data_020d433c.mEffects, 1, 0xc, 0, 1);
        if (handle) {
            CameraManager::Get();
            MtxFx43 mtx    = data_020c1cf4;
            MtxFx43 rot    = data_0220e86c;
            VecFx32 camPos = data_020d433c.mBattlePlayers->mUnk_298->mUnk_1a4->mPosition;
            VecFx32 v      = data_ov007_02279438;
            VecFx32 w      = data_ov007_02279444;
            mtx._[11]      = 0;
            mtx._[10]      = 0;
            mtx._[9]       = 0;
            MTX_MultVec43(&v, &mtx, &v);
            VEC_Add(&camPos, &v, &camPos);
            func_01ffd088(&rot, 0x1555);
            func_01ff89c0(&rot, &mtx, &mtx);
            func_01ffcf64(&mtx, &camPos);
            func_0202bf60(data_020d433c.mEffects, handle, &mtx);
            if (mVariant == 1) {
                func_0202bf18(data_020d433c.mEffects, handle, &w);
            }
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
            limit = data_ov007_0227942c[mVariant];
        } else {
            limit = data_ov007_02279430[mVariant][players - 2];
        }
        if (mUnk_44 < limit) {
            break;
        }
        func_ov007_0222f1ac();
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

void GimmickFieldPowerS::func_ov007_0222f1ac() {
    switch (mVariant) {
    case 0:
        func_ov007_0222ea28(0x5b, 0x20);
        break;
    case 1:
        func_ov007_0222ea28(0x61, 0x20);
        break;
    }
}
