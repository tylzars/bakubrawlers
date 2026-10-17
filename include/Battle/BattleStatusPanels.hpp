#pragma once

#include "global.h"
#include "types.h"

class CellAnimation;
class CharacterIcons;

// One player's panel on the upper screen (battle_u): sprites, the name text buffers and a few state bytes. 0x38 bytes.
struct BattlePanel {
    /* 00 */ CellAnimation *mCellAnims[7];
    /* 1c */ s16 mOffset[4]; // [0] / [1] the x / y offset added when drawn, stepping by 1 towards [2] / [3]
    /* 24 */ s8 mCharacterId; // character shown in the icon (-1: none)
    /* 25 */ u8 mMirrored; // both callers pass side 2, so it always equals mCol (team & 1) in this build
    /* 26 */ u8 mCol;
    /* 27 */ u8 mRow;
    /* 28 */ u8 mUnk_28;
    /* 29 */ u8 mUnk_29;
    /* 2a */ PAD(0x2a, 0x2b);
    /* 2b */ u8 mPrevLevel;
    /* 2c */ void *mBuffer;  // text buffer the name is printed into and uploaded from (mBufferSize bytes)
    /* 30 */ void *mBuffer2; // glyph buffer (mGlyphCount * 0xc) given to TextManager::Init and drawn by AddOam
    /* 34 */ u16 mNameWidth;
    /* 36 */ u8 mUnk_36;
    /* 37 */ PAD(0x37, 0x38);
    /* 38 */
};

// The four player panels of the battle upper screen (battle_u files). No vtable, 0x100 bytes.
class BattleStatusPanels;

class BattleStatusPanels {
public:
    /* 00 */ CharacterIcons *mIcons;
    /* 04 */ BattlePanel mPanels[4];
    /* e4 */ CellAnimation *mCellAnimC;
    /* e8 */ CellAnimation *mCellAnimD;
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
    void LoadGraphics();
    void SetupPanel(u32 index, s32 character, u32 team, u8 side);
    void DrawPanels();
    void InitNameText();
    void ClearNameText(u32 index);
    void PrintName(u32 index);
    u8 GetLevel(u32 index);
};
