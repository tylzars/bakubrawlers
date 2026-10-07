#pragma once

#include "global.h"
#include "types.h"

#include "Graphics/CellAnimation.hpp"

// Emblem sprite shown by Confront03 (vs_emblem_%02d_nce.bin). Vtable 02286068.
class ConfrontEmblem03 {
public:
    /* 04 */ u8 mUnk_04;
    /* 05 */ u8 mUnk_05;
    /* 06 */ PAD(0x06, 0x07);
    /* 07 */ u8 mUnk_07_0 : 1;
    /* 07 */ u8 mUnk_07_1 : 1;
    /* 07 */ u8 mUnk_07_2 : 1;
    /* 08 */ u8 mUnk_08;
    /* 09 */ u8 mUnk_09;
    /* 0a */ u8 mUnk_0a;
    /* 0b */ s8 mUnk_0b;
    /* 0c */ PAD(0x0c, 0x14);
    /* 14 */ float mUnk_14;
    /* 18 */ float mUnk_18;
    /* 1c */ float mUnk_1c;
    /* 20 */ CellAnimation *mAnim;
    /* 24 */

    ConfrontEmblem03(u8 index);
    virtual ~ConfrontEmblem03();
    void func_ov007_0221f1b8(u8 slot, int flag);
    void func_ov007_0221f268();
    void func_ov007_0221f368();
    void func_ov007_0221f3a4();
    u8 func_ov007_0221f57c();
    int func_ov007_0221f594();
    int func_ov007_0221f70c();
};
