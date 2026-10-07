#pragma once

#include "global.h"
#include "types.h"

// Sub-screen alpha-blend fade helper (0x10 bytes). Vtable 022865f4, ctor 02223cf0 (outside this range).
class BattleBlend {
public:
    /* 04 */ u8 mUnk_04;
    /* 05 */ u8 mUnk_05;
    /* 06 */ u16 mUnk_06;
    /* 08 */ s16 mUnk_08;
    /* 0a */ u8 mUnk_0a;
    /* 0b */ u8 mUnk_0b;
    /* 0c */ u16 mUnk_0c;
    /* 0e */ u8 mUnk_0e;
    /* 0f */ u8 mUnk_0f;
    /* 10 */

    BattleBlend(u16 a, u8 b, u8 c);   // 02223cf0
    virtual ~BattleBlend();           // 02223d4c deleting
    void func_ov007_02223d7c();
    void func_ov007_02223dc4();
    BOOL func_ov007_02223ea0();
    void func_ov007_02223ee4();
};
