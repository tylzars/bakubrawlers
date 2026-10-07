#pragma once

#include "global.h"
#include "types.h"

struct UnkStruct_020d433c {
    /* 00 */ PAD(0x00, 0x76);
    /* 76 */ u8 mUnk_76;
    /* 77 */ PAD(0x77, 0x8a);
    /* 8a */ u8 mUnk_8a_0 : 4;
    /* 8a */ u8 mUnk_8a_4 : 4;
    /* 8b */ u8 mUnk_8b_0 : 4;
    /* 8b */ u8 mUnk_8b_4 : 4;
    /* 8c */ u8 mUnk_8c_0 : 4;
    /* 8c */ u8 mUnk_8c_4 : 4;
    /* 8d */ PAD(0x8d, 0x97);
    /* 97 */ u8 mPlayerCount;
};

extern UnkStruct_020d433c data_020d433c;
