#pragma once

#include "global.h"
#include "types.h"

struct UnkStruct_020d48f0 {
    /* 00 */ PAD(0x00, 0xba);
    /* ba */ u16 mRank;
    /* bc */ u16 mPrevRank;
    /* be */ PAD(0xbe, 0xd4);
    /* d4 */ u16 mUnk_d4;
    /* d6 */ PAD(0xd6, 0x180); // the real extent is unknown; MapScene stores a u16 at +0xbc of a later element, so it is more than 0x178
    /* 180 */
};

extern UnkStruct_020d48f0 data_020d48f0;
