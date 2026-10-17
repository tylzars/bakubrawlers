#include "Graphics/CharacterIcons.hpp"

#include "Graphics/CellAnimation.hpp"
#include "Graphics/SpriteManager.hpp"
#include "System/FileSystem.hpp"

#include <stdio.h>

extern u8 gCharacterLook[];
void LoadEditedIcon(u8 screen, u32 offset, u32 palette, const void *colors, u32 unk5);

CharacterIcons::CharacterIcons(u32 unk1, u32 unk2, s32 unk3) {
    mUnk_30 = 0;
    mVram   = unk1;
    mPalette = unk2;
    mScreen = unk3 % 2;
    for (u8 i = 0; i < 4; i++) {
        mSlots[i].mCharacter = -1;
        mSlots[i].mAnim      = NULL;
        mSlots[i].mX         = 0;
        mSlots[i].mY         = 0;
        mSlots[i].mUnk_0a    = 0;
        mSlots[i].mVisible   = 0;
    }
}

CharacterIcons::~CharacterIcons() {
    for (u8 i = 0; i < 4; i++) {
        CellAnimation *anim = mSlots[i].mAnim;
        if (anim != NULL) {
            delete anim;
        }
    }
}

void CharacterIcons::SetCharacter(u32 slot, s32 character, u32 unk3, u32 unk4) {
    char path[0x80];
    if (slot >= 4) {
        return;
    }
    if (character == mSlots[slot].mCharacter) {
        return;
    }
    mSlots[slot].mCharacter = character;
    if (mSlots[slot].mCharacter < 0) {
        mSlots[slot].mVisible = 0;
        return;
    }
    u8 number = mSlots[slot].mCharacter + 1;
    ChangeDir("/Game/Character/Icon/");
    SpriteScreen &screen = SpriteManager::Get().Screen(mScreen);
    u32 offset           = mVram + slot * 0x200;
    u8 palette           = mPalette + slot;
    if (mSlots[slot].mCharacter != 0x15) {
        sprintf(path, "icon_ch_%02d.nce_ncg1D.bin", number);
        screen.LoadCharacters(path, offset, 0);
        sprintf(path, "icon_ch_%02d_ncl.bin", number);
        screen.LoadPalette(path, palette << 5, 0);
    } else {
        if (unk3 != 0) {
            LoadEditedIcon(mScreen, offset, palette, (const void *) unk3, unk4);
        } else {
            LoadEditedIcon(mScreen, offset, palette, gCharacterLook, unk4);
        }
        ChangeDir("/Game/Character/Icon/");
    }
    if (mSlots[slot].mAnim != NULL) {
        delete mSlots[slot].mAnim;
    }
    mSlots[slot].mAnim = NULL;
    sprintf(path, "icon_ch_%02d_nce.bin", number);
    mSlots[slot].mAnim = new CellAnimation(path, 0, 1, 1);
    mSlots[slot].mAnim->mCharBase = screen.GetCharBase(offset);
    mSlots[slot].mAnim->mPaletteBase   = palette;
    mSlots[slot].mVisible = 1;
    ChangeDir("/");
}

void CharacterIcons::SetDrawArgs(u8 slot, s16 x, s16 y, u32 unk4) {
    if (slot >= 4) {
        return;
    }
    mSlots[slot].mX      = x;
    mSlots[slot].mY      = y;
    mSlots[slot].mUnk_0a = *(u8 *) &unk4;
}

void CharacterIcons::DrawIcons() {
    for (u8 i = 0; i < 4; i++) {
        CharacterIconSlot *slot = &mSlots[i];
        if (slot->mVisible != 0) {
            slot->mAnim->Draw(mScreen, slot->mX, slot->mY, slot->mUnk_0a);
        }
    }
}

void CharacterIcons::SetVisible(u32 slot, u32 visible) {
    CharacterIconSlot *s = &mSlots[slot];
    if (s != NULL) {
        s->mVisible = visible;
    }
}
