#pragma once

#include "global.h"
#include "types.h"

#include <nitro/fx.h>

#include "Battle/BattleMapTypes.hpp"
#include "Graphics/BakuganModel.hpp"

// Motion record of a pose (0x14 bytes, allocated by func_ov007_02270120).
struct BattleMoveState {
    /* 00 */ VecFx32 mVelocity;
    /* 0c */ u16 mRot[3];
    /* 12 */ u8 mMode; // 1 = idle, 2..5 = launched
    /* 13 */ u8 mUnk_13;
};

// Poses, name plates and cursors of the battle command map (0xa4 bytes, ctor 0226fb74, dtor 0226fc18).
struct BattleUnk29c {
    /* 00 */ BakuganModel *mUnk_00[12];
    /* 30 */ BattleMoveState *mUnk_30[12];
    /* 60 */ BattleMapPiece *mUnk_60;
    /* 64 */ BattleFontObj *mUnk_64[9];
    /* 88 */ VecFx32 mUnk_88;
    /* 94 */ u8 mUnk_94;
    /* 95 */ u8 mUnk_95;
    /* 96 */ u8 mUnk_96;
    /* 97 */ PAD(0x97, 0x98);
    /* 98 */ u8 mUnk_98[3];
    /* 9b */ u8 mUnk_9b[3];
    /* 9e */ u8 mUnk_9e;
    /* 9f */ u8 mUnk_9f;
    /* a0 */ u8 mUnk_a0;
    /* a1 */ PAD(0xa1, 0xa4);
    /* a4 */

    BattleUnk29c();  // 0226fb74
    ~BattleUnk29c(); // 0226fc18
    BakuganModel *func_ov007_0226fc4c(u8 index);
    void func_ov007_0226fc5c(u32 index);
    void func_ov007_0226fca8();
    void func_ov007_0226fd78();
    void func_ov007_0226fe4c();
    void func_ov007_0226ff08(u32 flag);
    void func_ov007_02270120(u32 index, VecFx32 *pos, u16 *rot, u8 a, u8 b);
    void func_ov007_02270248();
    void func_ov007_02270350();
    void func_ov007_0227044c();
    void func_ov007_0227068c();
    void func_ov007_02270784(u8 index);
    void func_ov007_02270a7c(u8 index, u32 type);
    void func_ov007_02270ae0(u8 index, BattleMoveState *state);
    void func_ov007_02270dbc(u32 index);
    void func_ov007_02270f1c(u32 index);
    void func_ov007_02271054(u32 index);
    void func_ov007_0227118c(u32 index);
    void func_ov007_022712c4(u8 index, VecFx32 *pos);
    void func_ov007_02271338(VecFx32 *pos);
    void func_ov007_022713a4(VecFx32 *pos);
};
