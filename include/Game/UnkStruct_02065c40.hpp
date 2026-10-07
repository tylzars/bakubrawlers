#pragma once

#include "global.h"
#include "types.h"

// Script/event player.
class UnkStruct_02065c40 {
public:
    /* 0000 */ PAD(0x0000, 0x14f4);
    /* 14f4 */

    UnkStruct_02065c40(u32 unk1, u32 unk2, u32 unk3);
    BOOL func_020678e8();
    void func_0206eb10(const char *path);
    void func_020676f8();
    void func_0206ef4c(const char *name, u32 unk2, u32 unk3, u32 unk4, u32 unk5);
    void func_02067998(u8 unk1);
    void func_0206f564(u8 unk1, u32 unk2, s32 unk3);
    void func_020702e0(s32 unk1, s32 unk2, s32 unk3, s32 unk4, s32 unk5, s32 unk6);
};
