#pragma once

#include "global.h"
#include "types.h"

struct UnkStruct_020d433c {
    /* 00 */ PAD(0x00, 0x76);
    /* 76 */ u8 mUnk_76;
    /* 77 */ PAD(0x77, 0x7b);
    /* 7b */ u8 mUnk_7b;
    /* 7c */ PAD(0x7c, 0x7e);
    /* 7e */ u8 mUnk_7e_0 : 4;
    /* 7e */ u8 mUnk_7e_4 : 4;
    /* 7f */ u8 mUnk_7f;
    /* 80 */ PAD(0x80, 0x82);
    /* 82 */ u8 mUnk_82;
    /* 83 */ u8 mUnk_83;
    /* 84 */ u8 mUnk_84;
    /* 85 */ PAD(0x85, 0x86);
    /* 86 */ u8 mUnk_86;
    /* 87 */ PAD(0x87, 0x8a);
    /* 8a */ u8 mUnk_8a_0 : 4;
    /* 8a */ u8 mUnk_8a_4 : 4;
    /* 8b */ u8 mUnk_8b_0 : 4;
    /* 8b */ u8 mUnk_8b_4 : 4;
    /* 8c */ u8 mUnk_8c_0 : 4;
    /* 8c */ u8 mUnk_8c_4 : 4;
    /* 8d */ PAD(0x8d, 0x97);
    /* 97 */ u8 mPlayerCount;
    /* 98 */ u8 mUnk_98;
    /* 99 */ u8 mUnk_99;
    union {
        /* 9a */ u8 mUnk_9a;
        struct {
            /* 9a */ u8 mUnk_9a_0 : 1;
            /* 9a */ u8 mUnk_9a_1 : 7;
        };
    };
    /* 9b */ u8 mUnk_9b[4];
    /* 9f */ u8 mUnk_9f[4];
    /* a3 */ u8 mUnk_a3[4];
    /* a7 */
};

extern UnkStruct_020d433c data_020d433c;
