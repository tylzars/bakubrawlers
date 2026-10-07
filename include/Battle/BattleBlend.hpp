#pragma once

#include "global.h"
#include "types.h"

// Sub-screen alpha-blend fade helper (0x10 bytes). Vtable 022865f4, ctor 02223cf0 (outside this range).
class BattleBlend {
public:
    /* 04 */ u8 mUnk_04;
    /* 05 */ u8 mLevel;
    /* 06 */ u16 mUnk_06;
    /* 08 */ s16 mWobble;
    /* 0a */ u8 mUnk_0a;
    /* 0b */ u8 mFalling;
    /* 0c */ u16 mPeriod;
    /* 0e */ u8 mMin;
    /* 0f */ u8 mMax;
    /* 10 */

    BattleBlend(u16 period, u8 min, u8 max); // 02223cf0
    virtual ~BattleBlend();           // 02223d4c deleting
    void func_ov007_02223d7c();
    void func_ov007_02223dc4();
    BOOL func_ov007_02223ea0();
    void func_ov007_02223ee4();
};
