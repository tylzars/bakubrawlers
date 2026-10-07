#pragma once

#include "global.h"
#include "types.h"

#include "Graphics/CellAnimation.hpp"

// Floating emblem sprite of the confrontation screen 06 (vs_emblem_NN plus vs_b_obj_06_burst). 0x28 bytes, vtable 0228642c.
class ConfrontEmblem06 {
public:
    /* 04 */ u8 mUnk_04;
    /* 05 */ u8 mUnk_05_0 : 1;
    /* 05 */ u8 mUnk_05_1 : 1;
    /* 05 */ u8 mUnk_05_2 : 1;
    /* 05 */ u8 mUnk_05_3 : 1;
    /* 05 */ u8 mUnk_05_4 : 1;
    /* 06 */ u8 mUnk_06;
    /* 07 */ PAD(0x07, 0x08);
    /* 08 */ float mUnk_08;
    /* 0c */ float mUnk_0c;
    /* 10 */ float mUnk_10;
    /* 14 */ float mUnk_14;
    /* 18 */ float mUnk_18;
    /* 1c */ float mUnk_1c;
    /* 20 */ CellAnimation *mAnims[2];
    /* 28 */

    ConfrontEmblem06(u8 index);
    virtual ~ConfrontEmblem06();
    void func_ov007_02221884();
    void func_ov007_02221ad0();
    void func_ov007_02221c68();
    void func_ov007_02221d44();
    int func_ov007_02221f2c(int a);
    int func_ov007_0222210c();
};
