#pragma once

#include "global.h"
#include "types.h"

// Created with a table of records and polled once per frame (overlay 1).
class UnkStruct_0202f420 {
public:
    /* 00 */ PAD(0x00, 0x05);
    /* 05 */ s8 mUnk_05;
    /* 06 */ u8 mUnk_06;
    /* 07 */ s8 mUnk_07;
    /* 08 */ u8 mUnk_08;
    /* 09 */ PAD(0x09, 0x14);
    /* 14 */

    UnkStruct_0202f420(u32 count, const void *table, u32 unk3);
    u32 func_0202f468(u32 unk1);
};
