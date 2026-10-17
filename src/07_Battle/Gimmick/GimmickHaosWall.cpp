#include "Battle/GimmickTypes.hpp"
#include <nitro/fx.h>
#include "System/FileSystem.hpp"
#include "System/SoundManager.hpp"

extern "C" void MTX_ApplyRotY33(MtxFx33 *m, u16 angle);
extern MtxFx33 data_0220e848;

static ModelResource *sResource;

GimmickHaosWall::GimmickHaosWall(u8 id, u8 param)
    : Gimmick(id, 0) {
    mTimer = 0;
    mUnk_46 = param;
    mState  = 0;
    mType = GIMMICK_TYPE_HAOS_WALL;
}

GimmickHaosWall::~GimmickHaosWall() {
    Unload();
}

void GimmickHaosWall::Load() {
    if (!sResource) {
        sResource = new ModelResource("haos_wall.nsbmd", "haos_wall_data.bin", "haos_wall.nsbtx", 1);
    }
    ChangeDir("/Game/Gimmick");
    sResource->AddRef(1);
    mModel = sResource->mModel->CreateInstance(new ModelAnim(sResource->mAnimData, sResource->mModel));
    mModel->StartAnimWithIncrement(0, 1, 0x1000);
    mAnimEnabled = 1;
    ChangeDir("/");
}

void GimmickHaosWall::Unload() {
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

void GimmickHaosWall::Finish() {
    mState = 3;
}

void GimmickHaosWall::StartTimer(u8 cooldown) {
    mState  = 2;
    mTimer = cooldown;
}

void GimmickHaosWall::Update() {
    switch (mState) {
    case 0:
        LoadCollision(0x47, COLLISION_GROUP_GIMMICK_HIT_BY_SP5);
        mState++;
        break;
    case 1: {
        if (mModel && (int)data_020d433c.mGimmicks->mState >= 2) {
            DrawInstance();
        }
        if (!(mCollision && mCollision->mHitCount)) {
            return;
        }
        BattlePlayerList *list = data_020d433c.mBattlePlayers;
        if (!list->mBattleTarget) {
            return;
        }
        VecFx32 a;
        VecFx32 b;
        VecFx32 d;
        MtxFx33 m = data_0220e848;
        BakuganModel *h = list->mBattleTarget->mBakugan;
        VEC_Subtract(&mPosition, &h->mPosition, &a);
        if (mRotY) {
            MTX_ApplyRotY33(&m, -mRotY);
        }
        MTX_MultVec33(&a, &m, &b);
        GimmickNode *node = &data_020d433c.mGimmicks->mNodes[(u8)(mUnk_46 + 1)];
        ((GimmickHaosWall *)node->mGimmick)->StartTimer(0x1e);
        VecFx32 c = node->mGimmick->mPosition;
        m = data_0220e848;
        MTX_ApplyRotY33(&m, node->mGimmick->mRotY + 0x8000);
        MTX_MultVec33(&b, &m, &b);
        VEC_Add(&c, &b, &c);
        c.y = h->mPosition.y;
        d.x = c.x;
        d.y = 0;
        d.z = c.z;
        VEC_Normalize(&d, &d);
        d.x = FX_Mul(d.x, 0xf000);
        d.y = FX_Mul(d.y, 0xf000);
        d.z = FX_Mul(d.z, 0xf000);
        VEC_Subtract(&c, &d, &c);
        h->mPosition.x = c.x;
        h->mPosition.y = c.y;
        h->mPosition.z = c.z;
        mTimer = 0x1e;
        func_ov007_0225ca18(data_020d433c.mBattlePlayers->mBattleTarget, 7, 0);
        SoundManager::Get().StartSeqArc(0x5b);
        mState++;
        break;
    }
    case 2:
        if (mModel && (int)data_020d433c.mGimmicks->mState >= 2) {
            DrawInstance();
        }
        if (!mCollision) {
            return;
        }
        if (mCollision->mHitCount) {
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
