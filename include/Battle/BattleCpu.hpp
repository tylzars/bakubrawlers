#pragma once

#include "global.h"
#include "types.h"

#include <nitro/fx.h>

// Computer player helper of the battle (vtable 02287604: empty destructor + deleting destructor).
// mUnk_79 is the player index it plays for.
class BattleCpu {
public:
    /* 00 */ // vtable
    /* 04 */ VecFx32 mPos;
    /* 10 */ VecFx32 mSavedPos;
    /* 1c */ VecFx32 mTargets[4];
    /* 4c */ s32 mUnk_4c;
    /* 50 */ s32 mUnk_50;
    /* 54 */ s32 mUnk_54;
    /* 58 */ s32 mUnk_58;
    /* 5c */ s32 mUnk_5c;
    /* 60 */ u16 mUnk_60[8];
    /* 70 */ u16 mUnk_70;
    /* 72 */ u16 mUnk_72;
    /* 74 */ u16 mUnk_74;
    /* 76 */ u8 mUnk_76;
    /* 77 */ u8 mUnk_77;
    /* 78 */ u8 mUnk_78;
    /* 79 */ u8 mUnk_79;
    /* 7a */ u8 mUnk_7a;
    /* 7b */ u8 mUnk_7b;
    /* 7c */ PAD(0x7c, 0x7d);
    /* 7d */ u8 mUnk_7d;
    /* 7e */ PAD(0x7e, 0x7f);
    /* 7f */ u8 mUnk_7f[4];
    /* 83 */ u8 mUnk_83;
    /* 84 */ u8 mUnk_84;
    /* 85 */ u8 mUnk_85[8];
    /* 8d */ u8 mUnk_8d;
    /* 8e */

    BattleCpu(u8 player);
    virtual ~BattleCpu();
    void func_ov007_02231038();
    u8 func_ov007_022310f0();
    u8 func_ov007_02231300();
    u8 func_ov007_02231dd0();
    void func_ov007_022328dc();
    void func_ov007_022329cc(int a);
    void func_ov007_02232bfc();
    void func_ov007_02232c5c();
    void func_ov007_02232c88();
    void func_ov007_02232d8c();
    bool func_ov007_02232e04(u32 a, u32 b, u32 c);
    void func_ov007_02232eb8(s32 *x, s32 *y);
};
