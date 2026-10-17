#pragma once

#include "global.h"
#include "types.h"

class CellAnimation;

// One icon of the set.
struct CharacterIconSlot {
    /* 00 */ CellAnimation *mAnim;
    /* 04 */ s16 mX;
    /* 06 */ s16 mY;
    /* 08 */ u8 mVisible;
    /* 09 */ s8 mCharacterId; // -1: none
    /* 0a */ u8 mUnk_0a;
    /* 0b */ PAD(0x0b, 0x0c);
    /* 0c */
};

// Four character icons (icon_ch_NN files). Owned by pointer and destroyed with a plain (non-virtual) destructor.
class CharacterIcons {
public:
    /* 00 */ CharacterIconSlot mSlots[4];
    /* 30 */ u8 mUnk_30;
    /* 31 */ PAD(0x31, 0x34);
    /* 34 */ u32 mVram;
    /* 38 */ u8 mPalette;
    /* 39 */ u8 mScreen;
    /* 3a */ PAD(0x3a, 0x3c);
    /* 3c */

    CharacterIcons(u32 unk1, u32 unk2, s32 unk3); // 0x02065158
    ~CharacterIcons();
    void SetCharacter(u32 slot, s32 character, u32 unk3, u32 unk4);
    void SetDrawArgs(u8 slot, s16 x, s16 y, u32 unk4);
    void DrawIcons();
    void SetVisible(u32 slot, u32 visible);
};
