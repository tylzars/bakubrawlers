#pragma once

#include "global.h"
#include "types.h"

// Owned by pointer and destroyed with a plain (non-virtual) destructor.
class UnkStruct_020651bc {
public:
    /* 00 */ PAD(0x00, 0x3c);
    /* 3c */

    UnkStruct_020651bc(u32 unk1, u32 unk2, u32 unk3); // 0x02065158
    ~UnkStruct_020651bc();
    void func_02065204(u32 unk1, s32 unk2, u32 unk3, u32 unk4);
    void func_0206547c(u32 unk1, s16 unk2, s16 unk3, u32 unk4);
    void func_020654a0();
    void func_020654f8(u32 unk1, u32 unk2);
};
