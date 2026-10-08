#pragma once

#include "Scene/SceneBase.hpp"
#include "UI/MessageWindow.hpp"
#include "global.h"
#include "types.h"

class CellAnimation;
class MessageWindow;
class SelectWindow;
struct SelectWindowCursor;
class BattleCommandList;
class BattleCommandPage;
class UnkStruct_02076058;
class UnkStruct_02071d24;

// Object with a virtual destructor in the second slot (vtable +4).
class UnkObject_02078e14 {
public:
    /* 00 */ virtual ~UnkObject_02078e14();
    /* 04 */ u8 mUnk_04;
    /* 05 */ PAD(0x05, 0x64);
    /* 64 */ u8 mUnk_64;
};

// Owned by scenes that list or edit the player's data (constructed in StoreScene).
class UnkStruct_02078e14 : public SceneBase {
public:
    /* 00c */ void *mUnk_0c;
    /* 010 */ void *mUnk_10;
    /* 014 */ void *mUnk_14;
    /* 018 */ void *mUnk_18;
    /* 01c */ void *mUnk_1c;
    /* 020 */ void *mUnk_20;
    /* 024 */ u16 mUnk_24;
    /* 026 */ PAD(0x026, 0x028);
    /* 028 */ void *mUnk_28[2];
    /* 030 */ void *mUnk_30[2];
    /* 038 */ u32 mUnk_38[2]; // buffer sizes
    /* 040 */ u32 mUnk_40[2]; // glyph counts
    /* 048 */ SelectWindow *mUnk_48;
    /* 04c */ SelectWindow *mUnk_4c;
    /* 050 */ SelectWindow *mUnk_50;
    /* 054 */ SelectWindowCursor *mUnk_54;
    /* 058 */ SelectWindowCursor *mUnk_58;
    /* 05c */ CellAnimation *mUnk_5c[6];
    /* 074 */ CellAnimation *mUnk_74[19];
    /* 0c0 */ BattleCommandList *mUnk_c0;
    /* 0c4 */ BattleCommandPage *mUnk_c4;
    /* 0c8 */ u16 mUnk_c8[12];
    /* 0e0 */ // (the u8 fields below start here)
    /* 0e0 */ u8 mUnk_e0;
    /* 0e1 */ u8 mUnk_e1;
    /* 0e2 */ u8 mUnk_e2;
    /* 0e3 */ PAD(0x0e3, 0x0e4);
    /* 0e4 */ s16 mUnk_e4;
    /* 0e6 */ u8 mUnk_e6;
    /* 0e7 */ s8 mUnk_e7;
    /* 0e8 */ u8 mUnk_e8;
    /* 0e9 */ u8 mUnk_e9;
    /* 0ea */ u8 mUnk_ea;
    /* 0eb */ PAD(0x0eb, 0x0ec);
    /* 0ec */ u32 mUnk_ec;
    /* 0f0 */ u32 mUnk_f0;
    /* 0f4 */ u8 mUnk_f4;
    /* 0f5 */ u8 mUnk_f5;
    /* 0f6 */ PAD(0x0f6, 0x0f8);
    /* 0f8 */ s16 mUnk_f8;
    /* 0fa */ u8 mUnk_fa;
    /* 0fb */ s8 mUnk_fb;
    /* 0fc */ u8 mUnk_fc;
    /* 0fd */ u8 mUnk_fd;
    /* 0fe */ PAD(0x0fe, 0x100);
    /* 100 */ u32 mUnk_100;
    /* 104 */ u8 mUnk_104;
    /* 105 */ PAD(0x105, 0x106);
    /* 106 */ u16 mUnk_106[0x40];
    /* 186 */ u8 mUnk_186[13];
    /* 193 */ PAD(0x193, 0x194);
    /* 194 */ u32 mUnk_194[3];
    /* 1a0 */ u8 mUnk_1a0;
    /* 1a1 */ u8 mUnk_1a1;
    /* 1a2 */ struct {
        u16 mUnk_a;
        u16 mUnk_b;
    } mUnk_1a2[14];
    /* 1da */ PAD(0x1da, 0x1dc);
    /* 1dc */ u32 mUnk_1dc;
    /* 1e0 */ UnkStruct_02076058 *mUnk_1e0;
    /* 1e4 */ UnkStruct_02071d24 *mUnk_1e4;
    /* 1e8 */ UnkObject_02078e14 *mUnk_1e8;
    /* 1ec */ MessageWindow *mUnk_1ec;
    /* 1f0 */ u8 mUnk_1f0;
    /* 1f1 */ u8 mUnk_1f1;
    /* 1f2 */ u16 mUnk_1f2[0x100];
    /* 3f2 */ u8 mUnk_3f2;
    /* 3f3 */ PAD(0x3f3, 0x3f4);
    /* 3f4 */ u32 mUnk_3f4;
    /* 3f8 */ u8 mUnk_3f8;
    /* 3f9 */ u8 mUnk_3f9;
    /* 3fa */ u8 mUnk_3fa;
    /* 3fb */ PAD(0x3fb, 0x3fc);
    /* 3fc */ s16 mUnk_3fc;
    /* 3fe */ s8 mUnk_3fe;
    /* 3ff */ u8 mUnk_3ff;
    /* 400 */ u8 mUnk_400;
    /* 401 */ u8 mUnk_401;
    /* 402 */ PAD(0x402, 0x404);
    /* 404 */ s32 mUnk_404;
    /* 408 */

    UnkStruct_02078e14(u32 unk1, u32 unk2);
    virtual ~UnkStruct_02078e14();
    virtual void Update();
    void func_0207966c(u32 unk1);
    void func_0207ed3c();
    void func_0207b3a8();
    void func_0207c568();
    void func_0207ca60();
    void func_0207cf84();
    void func_0207ceb0();
    void func_0207dea4();
    void func_0207e074();
    void func_0207e424();
    void func_0207eec4();
    void func_0207f2f0();
    void func_0207f90c();
    void func_0207ff54();
    void func_02080104();
    u32 func_020864ec(u32 unk1, u32 unk2);
    u32 func_02086224(u32 unk1, u32 unk2);
    void func_020816ec(u32 unk1, u32 unk2);
    BOOL func_02082d00(u32 unk1, u32 unk2, u32 unk3);
    void func_0207f32c();
    void func_02081a74(u8 unk1, u32 unk2, u32 unk3, s16 unk4, s16 unk5);
    void func_02082264(u8 unk1, u8 unk2, u32 unk3, s16 unk4, s16 unk5);
    s32 func_0208644c();
    u8 func_0208349c(u32 unk1);
    void func_020831f4();
    void func_0207ea3c();
    void func_02081c2c(u32 unk1, u32 unk2, u32 unk3);
    void func_020823d8(u32 unk1, u32 unk2, u32 unk3);
    void func_02080898();
    void func_02081128();
    void func_02081594();
    void func_02085460(s32 unk1);
    void func_0207e7e4();
    BOOL func_020862d0();
    BOOL func_020842a0();
    void func_02081994(u32 unk1, u32 unk2, u32 unk3);
    void func_020807cc();
    BOOL func_02080b48(u32 unk1, u32 unk2);
    void func_02082660();
    void func_0208038c();
    void func_0208329c();
    void func_020842e8();
    void func_020851d0();
    void func_0208403c();
    void func_02082d70();
    void func_020826ec();
    u32 func_02086548(u32 unk1, u32 unk2);
    u32 func_02086570(u32 unk1, u32 unk2);
    void func_02081d5c(u32 unk1, u32 unk2, u32 unk3, u32 unk4);
    void func_0208250c(u32 unk1, u32 unk2);
    void func_0207b6e8();
    void func_02080210();
    void func_02080bdc(u32 unk1);
    void func_020832bc();
    void func_0207b760();
    void func_0207cfa4();
};

// Helpers of the second part of the class (UnkStruct_02078e14_b.cpp), free functions taking the object.
void func_020834ec(UnkStruct_02078e14 *self, u32 mode);
void func_0208415c(UnkStruct_02078e14 *self);
void func_020843ac(UnkStruct_02078e14 *self, u32 unk1);
void func_020848c8(UnkStruct_02078e14 *self);
BOOL func_02084a10(UnkStruct_02078e14 *self);
BOOL func_02084c38(UnkStruct_02078e14 *self);
BOOL func_02084e64(UnkStruct_02078e14 *self); // self is passed but unused
BOOL func_02084f40(UnkStruct_02078e14 *self); // self is passed but unused
BOOL func_0208501c(UnkStruct_02078e14 *self);
void func_02085220(UnkStruct_02078e14 *self, u32 mode);
void func_0208550c(UnkStruct_02078e14 *self);
