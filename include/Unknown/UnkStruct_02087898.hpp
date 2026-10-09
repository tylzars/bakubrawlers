#pragma once

#include "Scene/SceneBase.hpp"
#include "global.h"
#include "types.h"

class CellAnimation;

// A timed screen effect (0x4c bytes, ctor 02087898, vtable 020baa50): loaded with func_02087cf8, started with
// func_02088e64 and polled with func_02087cac. The battle scene keeps one at BattlePlayerList::mUnk_2a4.
class UnkStruct_02087898 : public SceneBase {
public:
    /* 0c */ CellAnimation *mUnk_0c[3];
    /* 18 */ void *mUnk_18;
    /* 1c */ void *mUnk_1c;
    /* 20 */ void *mUnk_20[2];
    /* 28 */ u32 mUnk_28[2];
    /* 30 */ u8 mUnk_30; // state
    /* 31 */ u8 mUnk_31;
    /* 32 */ u8 mUnk_32;
    /* 33 */ PAD(0x33, 0x34);
    /* 34 */ s16 mUnk_34;
    /* 36 */ u8 mUnk_36;
    /* 37 */ u8 mUnk_37;
    /* 38 */ u8 mUnk_38;
    /* 39 */ u8 mUnk_39;
    /* 3a */ u8 mUnk_3a;
    /* 3b */ u8 mUnk_3b;
    /* 3c */ u16 mUnk_3c;
    /* 3e */ u8 mUnk_3e;
    /* 3f */ u8 mUnk_3f;
    /* 40 */ u16 mUnk_40;
    /* 42 */ s16 mUnk_42;
    /* 44 */ u8 mUnk_44;
    /* 45 */ u8 mUnk_45;
    /* 46 */ u8 mUnk_46;
    /* 47 */ u8 mUnk_47;
    /* 48 */ u8 mUnk_48;
    /* 49 */ u8 mUnk_49;
    /* 4a */ u8 mUnk_4a;
    /* 4b */ u8 mUnk_4b;
    /* 4c */

    UnkStruct_02087898();
    virtual ~UnkStruct_02087898();
    virtual void Update();
    BOOL func_02087cac();
    void func_02087cc0();
    void func_02087cf8(const char *path);
    void func_02087eec();
    void func_02088a18(u32 unk1, u32 unk2);
    BOOL func_02088dac(u32 unk1);
    void func_02088e64(s32 unk1, s32 unk2, u32 unk3, u32 unk4);
    void func_02088ebc(u32 unk1);
    u32 func_02088ef4();
    BOOL func_02088efc();
    BOOL func_02088f14();
};
