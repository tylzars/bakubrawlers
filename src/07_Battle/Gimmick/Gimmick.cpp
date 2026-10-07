#include "Battle/Gimmick.hpp"
#include <nitro/fx.h>
#include "Game/UnkStruct_020d433c.hpp"

void func_ov007_022719c8(void *buf, u8 id);
GimmickSetData *func_ov007_02271ca8(UnkStruct_020d433c_0c *data, int count, void *buf, VecFx32 *pos, u16 *rot, u8 unk);
void func_ov007_02248944(VecFx32 *out, u8 a, u8 b);

Gimmick::Gimmick(u8 a, u8 b)
    : SceneBase(0x2710, 0) {
    mState = 0;
    mUnk_3a = 0;
    mUnk_39 = a;
    mUnk_3c = 0;
    mUnk_3b = b;
    mUnk_44 = 0;
    mUnk_40 = 0;
}

Gimmick::~Gimmick() {
    ModelInstance *model = mModel;
    if (model) {
        model->mUnk_68 = 0xf;
        delete model;
        mModel = 0;
    }
}

void Gimmick::func_ov007_0222ea28(int id, int count) {
    char buf[0x804];
    func_ov007_022719c8(buf, id);
    mUnk_3c = func_ov007_02271ca8(data_020d433c.mUnk_0c, count, buf, &mPosition, &mRotX, mUnk_3a);
}

BOOL Gimmick::func_ov007_0222ea84(int radius) {
    VecFx32 tmp;
    VecFx32 diff;
    for (u8 i = 0; i < 12; i++) {
        BattleMarker *base = data_020d433c.mBattlePlayers->Entries();
        BattleMarker *e    = &base[i];
        if (e->mUnk_2) {
            func_ov007_02248944(&tmp, e->mUnk_3, e->mUnk_4);
            tmp.y = mPosition.y;
            VEC_Subtract(&tmp, &mPosition, &diff);
            if (radius + 0x17800 >= VEC_Mag(&diff)) {
                return TRUE;
            }
        }
    }
    return FALSE;
}
