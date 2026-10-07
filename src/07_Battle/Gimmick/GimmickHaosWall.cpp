#include "Battle/GimmickTypes.hpp"
#include <nitro/fx.h>
#include "System/FileSystem.hpp"
#include "System/SoundManager.hpp"

void func_01ffcfd8(MtxFx33 *m, u16 angle);
extern MtxFx33 data_0220e848;

static ModelResource *sResource;

GimmickHaosWall::GimmickHaosWall(u8 id, u8 param)
    : Gimmick(id, 0) {
    mUnk_45 = 0;
    mUnk_46 = param;
    mState  = 0;
    mUnk_3a = 0x47;
}

GimmickHaosWall::~GimmickHaosWall() {
    Unload();
}

void GimmickHaosWall::Load() {
    if (!sResource) {
        sResource = new ModelResource("haos_wall.nsbmd", "haos_wall_data.bin", "haos_wall.nsbtx", 1);
    }
    ChangeDir("/Game/Gimmick");
    sResource->func_020306e4(1);
    mModel = sResource->mModel->func_02028b78(new ModelAnim(sResource->mAnimData, sResource->mModel));
    mModel->func_02028e54(0, 1, 0x1000);
    mUnk_00 = 1;
    ChangeDir("/");
}

void GimmickHaosWall::Unload() {
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

void GimmickHaosWall::Finish() {
    mState = 3;
}

void GimmickHaosWall::func_ov007_02274fac(u8 cooldown) {
    mState  = 2;
    mUnk_45 = cooldown;
}

void GimmickHaosWall::Update() {
    switch (mState) {
    case 0:
        func_ov007_0222ea28(0x47, 0x20);
        mState++;
        break;
    case 1: {
        if (mModel && (int)data_020d433c.mGimmicks->mState >= 2) {
            func_020526bc();
        }
        if (!(mUnk_3c && mUnk_3c->mUnk_823)) {
            return;
        }
        BattlePlayerList *list = data_020d433c.mBattlePlayers;
        if (!list->mUnk_298) {
            return;
        }
        VecFx32 a;
        VecFx32 b;
        VecFx32 d;
        MtxFx33 m = data_0220e848;
        BattleVecHolder4 *h = list->mUnk_298->mUnk_1a4;
        VEC_Subtract(&mPosition, &h->mPos, &a);
        if (mRotY) {
            func_01ffcfd8(&m, -mRotY);
        }
        func_01ff84d0(&a, &m, &b);
        GimmickNode *node = &data_020d433c.mGimmicks->mNodes[(u8)(mUnk_46 + 1)];
        ((GimmickHaosWall *)node->mGimmick)->func_ov007_02274fac(0x1e);
        VecFx32 c = node->mGimmick->mPosition;
        m = data_0220e848;
        func_01ffcfd8(&m, node->mGimmick->mRotY + 0x8000);
        func_01ff84d0(&b, &m, &b);
        VEC_Add(&c, &b, &c);
        c.y = h->mPos.y;
        d.x = c.x;
        d.y = 0;
        d.z = c.z;
        VEC_Normalize(&d, &d);
        d.x = FX_Mul(d.x, 0xf000);
        d.y = FX_Mul(d.y, 0xf000);
        d.z = FX_Mul(d.z, 0xf000);
        VEC_Subtract(&c, &d, &c);
        h->mPos.x = c.x;
        h->mPos.y = c.y;
        h->mPos.z = c.z;
        mUnk_45 = 0x1e;
        func_ov007_0225ca18(data_020d433c.mBattlePlayers->mUnk_298, 7, 0);
        SoundManager::Get().func_020246f8(0x5b);
        mState++;
        break;
    }
    case 2:
        if (mModel && (int)data_020d433c.mGimmicks->mState >= 2) {
            func_020526bc();
        }
        if (!mUnk_3c) {
            return;
        }
        if (mUnk_3c->mUnk_823) {
            return;
        }
        mUnk_45--;
        if (mUnk_45 == 0) {
            mState = 1;
        }
        break;
    case 3:
        mFinished = 1;
        break;
    }
}
