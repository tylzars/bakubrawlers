#include "Battle/Gimmick.hpp"
#include <nitro/fx.h>
#include "Game/UnkStruct_020d433c.hpp"

void LoadCollisionFile(void *buf, u8 id);
GimmickSetData *AddCollisionSet(GimmickCollision *data, int mode, void *buf, VecFx32 *pos, u16 *rot, u8 unk);
void GetCellPosition(VecFx32 *out, u8 a, u8 b);

Gimmick::Gimmick(u8 a, u8 b)
    : SceneBase(0x2710, 0) {
    mState = 0;
    mType = 0;
    mId = a;
    mCollision = 0;
    mUnk_3b = b;
    mTurnCount = 0;
    mCountingTurns = 0;
}

Gimmick::~Gimmick() {
    ModelInstance *model = mModel;
    if (model) {
        model->mOwnFlags = 0xf;
        delete model;
        mModel = 0;
    }
}

void Gimmick::LoadCollision(int id, int mode) {
    char buf[0x804];
    LoadCollisionFile(buf, id);
    mCollision = AddCollisionSet(data_020d433c.mGimmickCollision, mode, buf, &mPosition, &mRotX, mType);
}

BOOL Gimmick::IsNearMarker(int radius) {
    VecFx32 tmp;
    VecFx32 diff;
    for (u8 i = 0; i < 12; i++) {
        BattleMarker *base = data_020d433c.mBattlePlayers->Markers();
        BattleMarker *e    = &base[i];
        if (e->mStatus) {
            GetCellPosition(&tmp, e->mCellX, e->mCellY);
            tmp.y = mPosition.y;
            VEC_Subtract(&tmp, &mPosition, &diff);
            if (radius + 0x17800 >= VEC_Mag(&diff)) {
                return TRUE;
            }
        }
    }
    return FALSE;
}
