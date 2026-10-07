#pragma once

#include "global.h"
#include "types.h"

class UnkStruct_02074938 {
public:
    /* 00 */ PAD(0x00, 0x58);
    /* 58 */ u8 mUnk_58;
    /* 59 */ PAD(0x59, 0x80);
    /* 80 */ u32 mUnk_80;
    /* 84 */ u32 mUnk_84;
    /* 88 */

    UnkStruct_02074938(u8 unk1, u16 unk2, u32 unk3, u32 unk4); // 0x020741b4
    void func_02074938(u8 unk1, u16 unk2, u32 unk3);
};
