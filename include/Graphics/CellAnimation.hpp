#pragma once

#include "global.h"
#include "types.h"

// A cell animation loaded from an NCE file.
struct CellAnimFrame {
    /* 00 */ PAD(0x00, 0x10);
    /* 10 */ u32 mUnk_10;
    /* 14 */
};

class CellAnimation {
public:
    /* 00 */ CellAnimFrame *mFrames;
    /* 04 */ PAD(0x04, 0x0c);
    /* 0c */ u16 mCharBase;
    /* 0e */ u8 mUnk_0e;
    /* 0f */ u8 mUnk_0f;
    /* 10 */ u8 mUnk_10;
    /* 11 */ s8 mUnk_11;
    /* 12 */ u8 mUnk_12;
    /* 13 */ PAD(0x13, 0x18);
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
    void func_0202a494(u32 screen, s32 x, s32 y, u32 mode, s32 scaleX, s32 scaleY, u32 unk7); // scaled draw
};
