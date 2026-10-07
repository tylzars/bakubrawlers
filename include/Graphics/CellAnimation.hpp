#pragma once

#include "global.h"
#include "types.h"

// A cell animation loaded from an NCE file.
class CellAnimation {
public:
    /* 00 */ PAD(0x00, 0x0c);
    /* 0c */ u16 mCharBase;
    /* 0e */ u8 mUnk_0e;
    /* 0f */ PAD(0x0f, 0x11);
    /* 11 */ u8 mUnk_11;
    /* 12 */ PAD(0x12, 0x18);
    /* 18 */

    CellAnimation(const char *path, u32 unk2, u32 unk3, u32 unk4);
    ~CellAnimation();
    void Update();
    void Draw(u32 unk1, s32 x, s32 y, u32 unk4);
    void func_02029f6c();
    void SetAnimation(u8 index) {
        mUnk_0e = index;
        func_02029f6c();
    }
    void func_02029fb8(u32 unk1);
    void func_02029fc0();
    void func_0202a3ec(u32 unk1, u8 frame, s16 x, s16 y, u32 unk5);
};
