#pragma once

#include "global.h"
#include "types.h"

class CellAnimation;
class UnkStruct_020651bc;

// One player's panel on the upper screen (battle_u): sprites, the name text buffers and a few state bytes. 0x38 bytes.
struct BattlePanel {
    /* 00 */ CellAnimation *mAnims[7];
    /* 1c */ u16 mUnk_1c[4];
    /* 24 */ s8 mUnk_24; // character shown in the icon (-1: none)
    /* 25 */ u8 mUnk_25;
    /* 26 */ u8 mUnk_26;
    /* 27 */ u8 mUnk_27;
    /* 28 */ u8 mUnk_28;
    /* 29 */ u8 mUnk_29;
    /* 2a */ PAD(0x2a, 0x2b);
    /* 2b */ u8 mUnk_2b;
    /* 2c */ void *mBuffer;  // glyph buffer of the name text
    /* 30 */ void *mBuffer2; // text manager buffer
    /* 34 */ u16 mUnk_34;
    /* 36 */ u8 mUnk_36;
    /* 37 */ PAD(0x37, 0x38);
    /* 38 */
};

// The four player panels of the battle upper screen (battle_u files). No vtable, 0x100 bytes.
class BattleStatusPanels {
public:
    /* 00 */ UnkStruct_020651bc *mIcons;
    /* 04 */ BattlePanel mPanels[4];
    /* e4 */ CellAnimation *mAnimC;
    /* e8 */ CellAnimation *mAnimD;
    /* ec */ u32 mCharBase;
    /* f0 */ u8 mPalette;
    /* f1 */ u8 mUnk_f1;
    /* f2 */ u8 mUnk_f2;
    /* f3 */ PAD(0xf3, 0xf4);
    /* f4 */ u32 mBufferSize;
    /* f8 */ u32 mGlyphCount;
    /* fc */ u8 mMode;
    /* fd */ u8 mUnk_fd;
    /* fe */ PAD(0xfe, 0x100);
    /* 100 */

    BattleStatusPanels(u32 charBase, u8 palette, u8 mode, u8 flag);
    ~BattleStatusPanels();
    void func_ov007_02234efc();
    void func_ov007_02235280(u32 index, s32 character, u32 team, u8 side);
    void func_ov007_02235fe4();
    void func_ov007_0223619c(u32 index);
    void func_ov007_022361c4(u32 index);
    u8 func_ov007_02236458(u32 index);
};
