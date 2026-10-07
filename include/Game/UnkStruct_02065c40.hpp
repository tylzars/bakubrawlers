#pragma once

#include "global.h"
#include "types.h"

// Script/event player.
class UnkStruct_02065c40 {
public:
    /* 0000 */ PAD(0x0000, 0x14e7);
    /* 14e7 */ u8 mUnk_14e7;
    /* 14e8 */ PAD(0x14e8, 0x14f1);
    /* 14f1 */ u8 mUnk_14f1;
    /* 14f2 */ PAD(0x14f2, 0x14f4);
    /* 14f4 */

    UnkStruct_02065c40(u32 unk1, u32 unk2, u32 unk3);
    u32 func_020667ec();
    BOOL func_02066c7c();
    u32 func_020670b4(u32 id);
    BOOL func_020678e8();
    u32 func_02067900();
    BOOL func_02067938();
    void func_02067944();
    u32 func_02067954();
    void func_0206796c();
    u32 func_0206797c();
    void func_02067988();
    void func_0206eb10(const char *path);
    void func_020676f8();
    void func_0206ef4c(const char *name, u32 unk2, u32 unk3, u32 unk4, u32 unk5);
    void func_02067998(u8 unk1);
    void func_0206f564(u8 unk1, u32 unk2, s32 unk3);
    u32 func_0206f9b4(const char *name, u32 unk2, u32 unk3, u32 unk4, u32 unk5, u32 unk6, u32 unk7);
    u8 func_0206fa20(const char *name, const u16 *unk2, const u8 *unk3, u32 unk4, u32 unk5, u32 unk6, u32 unk7,
                     u32 unk8, u32 unk9);
    void func_020702e0(s32 unk1, s32 unk2, s32 unk3, s32 unk4, s32 unk5, s32 unk6);
};
