#include "global.h"
#include "types.h"
#include "Scene/SceneBase.hpp"
#include "Game/UnkStruct_020d433c.hpp"
#include "Graphics/CellAnimation.hpp"
#include "Battle/BattleTarget.hpp"
#include "Graphics/Camera.hpp"
#include "Graphics/Effect.hpp"
#include "Graphics/SpriteManager.hpp"
#include "UI/SelectWindow.hpp"
#include "Battle/BattleCpu.hpp"
#include "System/InputManager.hpp"
#include <nitro/gx.h>
#include <nitro/mi.h>
#include <nitro/os.h>

struct BattleShootCell { // 0xc bytes
    /* 00 */ CellAnimation *mAnim;
    /* 04 */ u32 mUnk_04;
    /* 08 */ u32 mUnk_08;
};

struct BattleShootIcon { // 0x10 bytes
    /* 00 */ CellAnimation *mAnim;
    /* 04 */ u16 mUnk_04;
    /* 06 */ u16 mUnk_06;
    /* 08 */ u16 mUnk_08;
    /* 0a */ u8 mUnk_0a;
    /* 0b */ u8 mUnk_0b;
    /* 0c */ u8 mUnk_0c;
    /* 0d */ PAD(0x0d, 0x10);
};

// The shoot minigame (vtable 0228a5a8, 0x61b0 bytes).
class BattleShoot : public SceneBase {
public:
    /* 0000 */ PAD(0x0c, 0x600c);
    /* 600c */ BattleShootCell mCells[7];
    /* 6060 */ BattleShootIcon mIcons[5];
    /* 60b0 */ u8 mUnk_60b0[0x80];
    /* 6130 */ BattleTarget *mUnk_6130;
    /* 6134 */ SelectWindow *mUnk_6134;
    /* 6138 */ SelectWindow *mUnk_6138;
    /* 613c */ void *mUnk_613c;
    /* 6140 */ u32 mUnk_6140;
    /* 6144 */ Camera *mUnk_6144;
    /* 6148 */ PAD(0x6148, 0x6154);
    /* 6154 */ u16 mUnk_6154;
    /* 6156 */ u16 mUnk_6156;
    /* 6158 */ u16 mUnk_6158;
    /* 615a */ PAD(0x615a, 0x615c);
    /* 615c */ u32 mUnk_615c;
    /* 6160 */ u32 mUnk_6160;
    /* 6164 */ u32 mUnk_6164;
    /* 6168 */ u16 mUnk_6168;
    /* 616a */ u16 mUnk_616a;
    /* 616c */ u32 mUnk_616c;
    /* 6170 */ u32 mUnk_6170;
    /* 6174 */ u16 mUnk_6174;
    /* 6176 */ u16 mUnk_6176;
    /* 6178 */ u16 mUnk_6178;
    /* 617a */ u16 mUnk_617a;
    /* 617c */ u16 mUnk_617c;
    /* 617e */ u16 mUnk_617e;
    /* 6180 */ u16 mUnk_6180;
    /* 6182 */ u16 mUnk_6182;
    /* 6184 */ u16 mUnk_6184;
    /* 6186 */ u8 mUnk_6186;
    /* 6187 */ PAD(0x6187, 0x6188);
    /* 6188 */ u8 mUnk_6188;
    /* 6189 */ u8 mUnk_6189;
    /* 618a */ u8 mUnk_618a;
    /* 618b */ u8 mUnk_618b;
    /* 618c */ u8 mUnk_618c;
    /* 618d */ u8 mUnk_618d;
    /* 618e */ u8 mUnk_618e;
    /* 618f */ u8 mUnk_618f;
    /* 6190 */ u8 mUnk_6190;
    /* 6191 */ u8 mUnk_6191;
    /* 6192 */ u8 mUnk_6192;
    /* 6193 */ u8 mUnk_6193;
    /* 6194 */ u8 mUnk_6194;
    /* 6195 */ u8 mUnk_6195;
    /* 6196 */ u8 mUnk_6196;
    /* 6197 */ u8 mUnk_6197;
    /* 6198 */ u8 mUnk_6198;
    /* 6199 */ u8 mUnk_6199;
    /* 619a */ u8 mUnk_619a;
    /* 619b */ u8 mUnk_619b;
    /* 619c */ u8 mUnk_619c;
    /* 619d */ u8 mUnk_619d;
    /* 619e */ u8 mUnk_619e;
    /* 619f */ u8 mUnk_619f;
    /* 61a0 */ u8 mUnk_61a0;
    /* 61a1 */ u8 mUnk_61a1;
    /* 61a2 */ u8 mUnk_61a2;
    /* 61a3 */ PAD(0x61a3, 0x61a4);
    /* 61a4 */ u8 mUnk_61a4;
    /* 61a5 */ u8 mUnk_61a5;
    /* 61a6 */ u8 mUnk_61a6;
    /* 61a7 */ u8 mUnk_61a7;
    /* 61a8 */ PAD(0x61a8, 0x61a9);
    /* 61a9 */ u8 mUnk_61a9;
    /* 61aa */ PAD(0x61aa, 0x61b0);
    /* 61b0 */

    BattleShoot(Camera *unk1, u8 unk2);
    BattleShoot(Camera *unk1, const u16 *pos, u16 unk3, u16 unk4, u8 unk5, u8 unk6, u8 unk7);
    virtual ~BattleShoot();
    /* 08 */ virtual void Update();
    void func_ov007_0226aa14(Camera *unk1);
    void func_ov007_0226bb2c();
    void func_ov007_0226bd3c();
    void func_ov007_0226bdfc();
    void func_ov007_0226c798();
    void func_ov007_0226cad8();
    void func_ov007_0226d690();
    void func_ov007_0226e234();
    void func_ov007_0226f87c();
    BOOL func_ov007_0226d214();
    u16 func_ov007_0226df24(int a, int b);
    u16 func_ov007_0226e390();
};

extern u16 data_ov007_0227bcd0[];

BattleShoot::BattleShoot(Camera *unk1, u8 unk2)
    : SceneBase(0x3e8, 0) {
    func_ov007_0226aa14(unk1);
    mUnk_6154 = 0;
    mUnk_6156 = 0x6000;
    mUnk_6158 = 0x38e;
    mUnk_619e = 0;
    mUnk_61a2 = unk2;
    mUnk_6186 = 0;
}

BattleShoot::BattleShoot(Camera *unk1, const u16 *pos, u16 unk3, u16 unk4, u8 unk5, u8 unk6, u8 unk7)
    : SceneBase(0x3e8, 0) {
    func_ov007_0226aa14(unk1);
    mUnk_6154 = pos[0];
    mUnk_6156 = pos[1];
    mUnk_6158 = pos[2];
    mUnk_6193 = unk5;
    mUnk_6168 = unk3;
    mUnk_616a = unk4;
    mUnk_6198 = unk6;
    mUnk_61a2 = unk7;
    mUnk_6186 = 7;
}

void BattleShoot::func_ov007_0226aa14(Camera *unk1) {
    mUnk_61a7 = 0;
    mUnk_6188 = 0;
    mUnk_6193 = 0;
    mUnk_6189 = 0;
    mUnk_618a = 0;
    mUnk_618b = 0;
    mUnk_618c = 0;
    mUnk_6164 = 0;
    mUnk_618f = 0;
    mUnk_6190 = 0;
    mUnk_618d = 0;
    mUnk_618e = 0;
    mUnk_6191 = 0;
    mUnk_6192 = 0;
    mUnk_615c = 0;
    mUnk_6160 = 0;
    mUnk_6194 = 0;
    mUnk_6168 = 0;
    mUnk_616a = data_ov007_0227bcd0[data_020d433c.mUnk_95];
    mUnk_6195 = 1;
    mUnk_6196 = 0;
    mUnk_6197 = 0;
    mUnk_6198 = 0;
    mUnk_6199 = 0;
    mUnk_6182 = 0x122;
    mUnk_6144 = unk1;
    mUnk_6134 = 0;
    mUnk_6130 = 0;
    mUnk_6138 = 0;
    mUnk_616c = 0x5b;
    mUnk_6170 = 0xb6;
    mUnk_6174 = 0;
    mUnk_6176 = 0;
    mUnk_6178 = 0;
    mUnk_617a = 0;
    mUnk_617c = 0;
    mUnk_617e = 0;
    mUnk_6180 = 0;
    mUnk_61a0 = 1;
    mUnk_61a1 = 1;
    mUnk_61a5 = 0;
    mUnk_61a4 = 0;
    mUnk_61a6 = 0x30;
    mUnk_6184 = 0;
    mUnk_619a = 0x80;
    mUnk_619b = 0x80;
    mUnk_613c = 0;
    mUnk_6140 = 0;
    mUnk_619c = 0;
    mUnk_619d = 0;
    mUnk_619f = 0;
    mUnk_61a2 = 0;
    if (data_020d433c.mBattlePlayers->mUnk_28a == data_020d433c.mBattlePlayers->mUnk_2a9) {
        mUnk_6197 = 1;
    }
    MI_CpuClear32(mCells, 0x54);
    MI_CpuClear32(mIcons, 0x50);
    MI_CpuClear32(mUnk_60b0, 0x80);
    mUnk_61a9 = data_020d433c.mPlayerCount - 1;
}

BattleShoot::~BattleShoot() {
    u8 i = 0;
    do {
        CellAnimation *a = mCells[i].mAnim;
        if (a) {
            delete a;
        }
        i++;
    } while (i < 7);
    u8 j = 0;
    do {
        CellAnimation *a = mIcons[j].mAnim;
        if (a) {
            delete a;
        }
        j++;
    } while (j < 5);
    if (mUnk_613c) {
        OS_Free(mUnk_613c);
    }
    data_020d433c.mEffects->func_0202be68();
    func_0202bc58(data_020d433c.mEffects, 0);
    data_020d433c.mBattlePlayers->mUnk_298 = 0;
    if (mUnk_6134) {
        delete mUnk_6134;
    }
    if (mUnk_6138) {
        delete mUnk_6138;
    }
    G2_BlendNone();
    G2S_BlendNone();
}

void BattleShoot::func_ov007_0226bd3c() {
    func_ov007_0226bb2c();
    func_ov007_0226bdfc();
    func_ov007_0226c798();
    u8 player = data_020d433c.mBattlePlayers->mUnk_28a;
    u8 record = data_020d433c.mBattlePlayers->mPlayers[player]->mRecord;
    delete mUnk_6130;
    mUnk_6130                              = new BattleTarget(mUnk_6144, player, record);
    data_020d433c.mBattlePlayers->mUnk_298 = mUnk_6130;
    mContext->mUnk_12                      = 0;
    mUnk_6130->mUnk_1b8                    = mUnk_6168;
    mUnk_6130->mUnk_1ba                    = mUnk_616a;
}

void BattleShoot::func_ov007_0226cad8() {
    SpriteManager::Get().Screen(1).func_02027778(mUnk_613c, mUnk_6140, (s16) (0x80 - ((u32) mUnk_619f >> 1)), 0x50, *(u8 *) ((u8 *) this + 0x6038), 0x280, 0xf, 0);
}

BOOL BattleShoot::func_ov007_0226d214() {
    return (u8) mUnk_6134->Update(1) == 2;
}

#define INT_TO_FX(n) ((fx32) ((n) > 0 ? 0.5f + (float) ((n) << 12) : (float) ((n) << 12) - 0.5f))

u16 BattleShoot::func_ov007_0226df24(int a, int b) {
    VecFx32 v;
    v.x = INT_TO_FX(a);
    v.y = INT_TO_FX(b);
    v.z = 0;
    return VEC_Mag(&v) >> 12;
}

u16 func_0200198c(fx32 x, fx32 y);

// non-matching: 2 words, the copy of the player's position uses ip instead of r5 as the destination pointer.
u16 BattleShoot::func_ov007_0226e390() {
    BattlePlayerList *list = data_020d433c.mBattlePlayers;
    u8 index               = list->mUnk_28a;
    VecFx32 cpuPos         = list->mPlayers[index]->mUnk_c8->mPos;
    UnkStruct_0203d374 *target = mUnk_6130->mUnk_1a4;
    VecFx32 delta;
    VecFx32 offset = {0, 0, 0};
    VEC_Subtract(&offset, &target->mPosition, &offset);
    VEC_Subtract(&cpuPos, &target->mPosition, &delta);
    u16 first = func_0200198c(offset.z, offset.x);
    u16 second = func_0200198c(delta.z, delta.x);
    return second - first;
}

u16 func_0200198c(fx32 x, fx32 y);

void BattleShoot::func_ov007_0226d690() {
    if (mUnk_6130->mUnk_1e5 != 0) {
        mUnk_6180               = 0;
        mUnk_6130->mUnk_1e5 = 0;
    }
    if (InputManager::Get().IsDown(0x200) | InputManager::Get().IsDown(0x100)) {
        if (mUnk_6180 < 0x1e) {
            mUnk_6180++;
        }
    } else {
        if (mUnk_6180 != 0) {
            mUnk_6180--;
        }
    }
    u16 value            = mUnk_6180;
    s32 spread           = value * 0x1800;
    BattleTarget *target = mUnk_6130;
    target->mUnk_188.x   = spread;
    target->mUnk_188.y   = value << 12;
    target->mUnk_188.z   = spread;
}

// non-matching: 13 words, register assignment only (original: loop index i in r4, distance in r5).
void BattleShoot::func_ov007_0226e234() {
    if (!mIcons[0].mAnim) {
        return;
    }
    if (mUnk_6188 < 0x10) {
        mUnk_6188++;
        func_ov007_0226f87c();
    }
    if (mUnk_6194 != 0) {
        if (mUnk_6189 == 0) {
            mUnk_6194 = 0;
        }
        return;
    }
    if (mUnk_6189 == 0) {
        return;
    }
    s32 dx = mUnk_618c - mUnk_6190;
    if (dx < 0) {
        dx = -dx;
    }
    s32 dy = mUnk_618b - mUnk_618f;
    if (dy < 0) {
        dy = -dy;
    }
    u8 i;
    int distance = (u16) (dy + dx);
    if (distance >= 0x20) {
        i = 0;
        do {
            if (mIcons[i].mUnk_0c == 0) {
                mIcons[i].mAnim->SetAnimation(0);
                mIcons[i].mUnk_04 = mUnk_618f;
                mIcons[i].mUnk_06 = mUnk_6190;
                mIcons[i].mUnk_0a = 0;
                mIcons[i].mUnk_0b = 0;
                mIcons[i].mUnk_08 = func_0200198c(-(mUnk_618b - mUnk_618f), mUnk_618c - mUnk_6190);
                if (distance > 0x20) {
                    distance = 0x20;
                }
                mIcons[i].mUnk_0c = distance;
                mUnk_6194         = 1;
                break;
            }
            i++;
        } while (i < 5);
        if (mUnk_619c != 0) {
            mUnk_619c--;
        }
    }
}
