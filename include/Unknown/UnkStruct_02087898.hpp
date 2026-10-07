#pragma once

#include "global.h"
#include "types.h"

// A timed screen effect (0x4c bytes, ctor 02087898): loaded with func_02087cf8, started with func_02088e64 and
// polled with func_02087cac. The battle scene keeps one at BattlePlayerList::mUnk_2a4.
class UnkStruct_02087898 {
public:
    /* 00 */ PAD(0x00, 0x34);
    /* 34 */ s16 mUnk_34;
    /* 36 */ PAD(0x36, 0x42);
    /* 42 */ s16 mUnk_42;
    /* 44 */ PAD(0x44, 0x4a);
    /* 4a */ u8 mUnk_4a;
    /* 4b */ PAD(0x4b, 0x4c);
    /* 4c */

    UnkStruct_02087898();
    BOOL func_02087cac();
    void func_02087cc0();
    void func_02087cf8(const char *path);
    void func_02087eec();
    void func_02088a18(u32 unk1, u32 unk2);
    BOOL func_02088dac(u32 unk1);
    void func_02088e64(s32 unk1, s32 unk2, u32 unk3, u32 unk4);
    void func_02088ebc(u32 unk1);
    BOOL func_02088ef4();
    u8 func_02088efc();
    BOOL func_02088f14();
};
