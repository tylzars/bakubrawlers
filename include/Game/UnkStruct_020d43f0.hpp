#pragma once

#include "global.h"
#include "types.h"

struct UnkStruct_020d43f0 {
    /* 000 */ PAD(0x000, 0x01c);
    /* 01c */ u32 mMoney;
    /* 020 */ PAD(0x020, 0x024);
    /* 024 */ u8 mUnk_24;
    /* 025 */ PAD(0x025, 0x5b9);
    /* 5b9 */ u8 mUnk_5b9_0 : 1;
    /* 5b9 */ u8 mUnk_5b9_1 : 7;
};

// Declared as an array: code that stores through it only matches when indexed (`data_020d43f0[0]`).
extern UnkStruct_020d43f0 data_020d43f0[];
