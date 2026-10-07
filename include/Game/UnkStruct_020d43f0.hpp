#pragma once

#include "global.h"
#include "types.h"

struct UnkStruct_020d43f0 {
    /* 000 */ PAD(0x000, 0x01c);
    /* 01c */ u32 mMoney;
    /* 020 */ PAD(0x020, 0x024);
    /* 024 */ u8 mUnk_24;
    /* 025 */ PAD(0x025, 0x578);
    /* 578 */ u8 mUnk_578;
    /* 579 */ PAD(0x579, 0x5b8);
    /* 5b8 */ u8 mUnk_5b8;
    /* 5b9 */ u8 mUnk_5b9_0 : 1;
    /* 5b9 */ u8 mUnk_5b9_1 : 7;
    /* 5ba */ PAD(0x5ba, 0x5c1);
    /* 5c1 */ u8 mUnk_5c1;
};

// Declared as an array: code that stores through it only matches when indexed (`data_020d43f0[0]`).
extern UnkStruct_020d43f0 data_020d43f0[];

// Bit-flag arrays inside data_020d43f0 (at +0x578, +0x57d, +0x580) that code addresses directly.
extern u8 data_020d4968[];
extern u8 data_020d496d[];
extern u8 data_020d4970[];

static inline BOOL TestFlagBit(const u8 *flags, int index) {
    return flags[(u32) index >> 3] & (1 << (index % 8));
}
