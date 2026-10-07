#include "Game/UnkStruct_020d433c.hpp"
#include "Game/UnkStruct_020d43f0.hpp"
#include "Graphics/CellAnimation.hpp"
#include "Graphics/SpriteManager.hpp"
#include "Scene/SceneBase.hpp"
#include "System/FileSystem.hpp"

extern const u8 data_ov007_0227a33c[];
extern const u8 data_ov007_0227a33d[];

class BattleObj00 : public SceneBase {
public:
    /* 0c */ CellAnimation *mAnims[3];
    /* 18 */ PAD(0x18, 0x1c);
    /* 1c */ void *mCamera;
    /* 20 */ PAD(0x20, 0x24);
    /* 24 */ u32 mUnk_24;
    /* 28 */ u16 mUnk_28;
    /* 2a */ u16 mUnk_2a;
    /* 2c */ u16 mUnk_2c;
    /* 2e */ u16 mUnk_2e;
    /* 30 */ u16 mUnk_30;
    /* 32 */ u8 mUnk_32;
    /* 33 */ u8 mUnk_33;
    /* 34 */ u8 mUnk_34;
    /* 35 */ u8 mUnk_35;
    /* 36 */ u8 mUnk_36;
    /* 37 */ PAD(0x37, 0x3a);
    /* 3a */ u8 mUnk_3a;
    /* 3b */ PAD(0x3b, 0x3c);
    /* 3c */ u8 mUnk_3c;
    /* 3d */ PAD(0x3d, 0x3e);
    /* 3e */ u8 mUnk_3e;
    /* 3f */ u8 mUnk_3f;
    /* 40 */

    BattleObj00(u8 flag);
    virtual ~BattleObj00();
    virtual void Update();
    void func_ov007_0224ad2c();
};

BattleObj00::BattleObj00(u8 flag)
    : SceneBase(0x3e8, 0) {
    mUnk_28 = 0;
    mUnk_2a = 0;
    mUnk_2c = 0;
    mUnk_33 = 0;
    mUnk_34 = 0;
    mUnk_35 = 0;
    mUnk_36 = 0;
    mUnk_3e = 0;
    if (data_020d43f0[0].mUnk_28_5 != 0) {
        mUnk_3f = 0;
    } else {
        mUnk_3f = 4;
    }
    mCamera = data_020d433c.mBattlePlayers->mCamera;
    mAnims[2] = 0;
    mAnims[1] = 0;
    mAnims[0] = 0;
    mUnk_24 = 0;
    mUnk_2e = 0;
    mUnk_30 = 0;
    mUnk_3a = data_020d433c.mBattlePlayers->mUnk_28a;
    if (flag == 0) {
        mUnk_32 = 0;
    } else {
        mUnk_32 = 11;
    }
    u32 v = data_020d433c.mBattlePlayers->mPlayers[data_020d433c.mBattlePlayers->mUnk_2a9]->mUnk_f1;
    if (v <= 1) {
        mUnk_3c = 0;
    } else {
        mUnk_3c = 1;
    }
    mUnk_34 = data_ov007_0227a33c[v * 2];
    u8 b    = data_ov007_0227a33d[v * 2];
    mUnk_35 = b;
    BattlePlayerList *l = data_020d433c.mBattlePlayers;
    l->mUnk_28b = mUnk_34;
    l->mUnk_28c = b;
}

BattleObj00::~BattleObj00() {
    CellAnimation *anim = mAnims[2];
    if (anim) {
        delete anim;
    }
    mAnims[2] = 0;
    anim = mAnims[1];
    if (anim) {
        delete anim;
    }
    mAnims[1] = 0;
    anim = mAnims[0];
    if (anim) {
        delete anim;
    }
    mAnims[0] = 0;
    SpriteManager::Get().func_0202740c();
}

void BattleObj00::func_ov007_0224ad2c() {
    ChangeDir("/Game/Battle/Obj");
    func_02025b64(0, 4, 0, "battle_u.nce_ncg1D.bin", 0, 0, 0);
    func_02025b64(0, 5, 0, "battle_u_ncl.bin", 0, 0, 0);
    ChangeDir("/");
    SpriteScreen &screen = SpriteManager::Get().Screen(1);
    screen.func_02027624(3);
    ChangeDir("/Game/Battle/Obj");
    mAnims[0] = new CellAnimation("battle_b_obj_00_nce.bin", 0x15, 1, 1);
    mAnims[0]->mCharBase = screen.func_020276f4(0x10000);
    mAnims[0]->mUnk_11 = 0;
    mAnims[1] = new CellAnimation("battle_b_obj_00_nce.bin", 0x16, 1, 1);
    mAnims[1]->mCharBase = screen.func_020276f4(0x10000);
    mAnims[1]->mUnk_11 = 0;
    mAnims[2] = new CellAnimation("battle_b_obj_00_nce.bin", 0x1b, 1, 1);
    mAnims[2]->mCharBase = screen.func_020276f4(0x10000);
    mAnims[2]->mUnk_11 = 0;
    ChangeDir("/Game/2D/union_b");
}
