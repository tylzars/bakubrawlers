#include "UI/MessageWindow.hpp"

#include "Graphics/CellAnimation.hpp"
#include "Graphics/SpriteManager.hpp"
#include "System/FileSystem.hpp"
#include "Text/TextManager.hpp"
#include "Unknown/UnkStruct_020651bc.hpp"

#include <nitro/mi.h>
#include <nitro/os.h>
#include <stdio.h>

extern u8 data_020d4404[];
extern u8 data_020a10a0[];
void func_0207889c(u32 screen, u32 offset, u32 palette, const void *colors, u32 unk5);

UnkStruct_020651bc::UnkStruct_020651bc(u32 unk1, u32 unk2, s32 unk3) {
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

UnkStruct_020651bc::~UnkStruct_020651bc() {
    for (u8 i = 0; i < 4; i++) {
        CellAnimation *anim = mSlots[i].mAnim;
        if (anim != NULL) {
            delete anim;
        }
    }
}

void UnkStruct_020651bc::func_02065204(u32 slot, s32 character, u32 unk3, u32 unk4) {
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
            func_0207889c(mScreen, offset, palette, (const void *) unk3, unk4);
        } else {
            func_0207889c(mScreen, offset, palette, data_020d4404, unk4);
        }
        ChangeDir("/Game/Character/Icon/");
    }
    if (mSlots[slot].mAnim != NULL) {
        delete mSlots[slot].mAnim;
    }
    mSlots[slot].mAnim = NULL;
    sprintf(path, "icon_ch_%02d_nce.bin", number);
    mSlots[slot].mAnim = new CellAnimation(path, 0, 1, 1);
    mSlots[slot].mAnim->mCharBase = screen.func_020276f4(offset);
    mSlots[slot].mAnim->mUnk_11   = palette;
    mSlots[slot].mVisible = 1;
    ChangeDir("/");
}

void UnkStruct_020651bc::func_0206547c(u8 slot, s16 x, s16 y, u32 unk4) {
    if (slot >= 4) {
        return;
    }
    mSlots[slot].mX      = x;
    mSlots[slot].mY      = y;
    mSlots[slot].mUnk_0a = *(u8 *) &unk4;
}

void UnkStruct_020651bc::func_020654a0() {
    for (u8 i = 0; i < 4; i++) {
        CharacterIconSlot *slot = &mSlots[i];
        if (slot->mVisible != 0) {
            slot->mAnim->Draw(mScreen, slot->mX, slot->mY, slot->mUnk_0a);
        }
    }
}

void UnkStruct_020651bc::func_020654f8(u32 slot, u32 visible) {
    CharacterIconSlot *s = &mSlots[slot];
    if (s != NULL) {
        s->mVisible = visible;
    }
}

MessageWindow::MessageWindow(const char *name, u32 unk2, u32 unk3) {
    mFile       = NULL;
    mBuffer     = NULL;
    mGlyphs     = NULL;
    mBufferSize = 0;
    mGlyphCount = 0;
    mVram       = 0;
    mX          = 0;
    mY          = 0;
    mUnk_20     = 0;
    mUnk_22     = 0;
    mMessage    = -1;
    mUnk_27     = 0;
    mScreen     = unk2;
    mWidth      = 0xc0;
    mTimer      = 0;
    mUnk_2b     = 0;
    func_02065bcc(unk3);
    func_02065660(name);
    func_020656c4();
}

MessageWindow::~MessageWindow() {
    if (mFile != NULL) {
        OS_Free(mFile);
    }
    if (mBuffer != NULL) {
        OS_Free(mBuffer);
    }
    if (mGlyphs != NULL) {
        OS_Free(mGlyphs);
    }
}

void MessageWindow::func_02065660(const char *name) {
    char path[0x80];
    if (mFile != NULL) {
        OS_Free(mFile);
    }
    sprintf(path, "/font/%s.mes", name);
    mFile = (MessageFile *) func_02023504(path, 0, 0, 0, 0, 0);
}

void MessageWindow::func_020656c4() {
    mBufferSize = TextManager::Get().BufferSize(0x28, 2);
    mBuffer     = OS_Alloc(mBufferSize);
    mGlyphCount = TextManager::Get().GlyphCount(0x14);
    mGlyphs     = OS_Alloc(mGlyphCount * 0xc);
    TextManager::Get().Init(mGlyphs, mScreen, 0, 0x28, 2, 0x11);
    func_02065818();
    if (mScreen == 0) {
        MI_CpuClearFast((void *) (mVram + 0x6400000), 0x800);
    } else {
        MI_CpuClearFast((void *) (mVram + 0x6600000), 0x800);
    }
    func_02065878();
}

void MessageWindow::func_02065818() {
    TextManager::Get().SetColor(mScreen, 0, 0xf);
}

void MessageWindow::func_02065878() {
    MI_CpuClear16(mBuffer, mBufferSize);
}

void MessageWindow::func_02065890(u32 unk1, u32 unk2, u32 unk3) {
    mMessage = unk1;
    mUnk_2b  = unk2;
    func_02065878();
    mUnk_26 = 0;
    mTimer  = 0;
    mUnk_22 = unk3 != 0 ? mWidth : 2;
    mUnk_20 = mWidth;
}

// non-matching: argument setup order for the func_02025c1c call.
void MessageWindow::func_020658d0() {
    if (mMessage < 0) {
        return;
    }
    u32 flag = mUnk_20 < mWidth ? mUnk_2b : 1;
    if (mTimer != 0) {
        mTimer--;
    } else if (!((mUnk_22 == 0) & (flag == 0))) {
        mUnk_22--;
        if (mUnk_22 == 0) {
            mTimer = 0x3c;
        }
        func_02065878();
        TextManager &text = TextManager::Get();
        s16 width;
        s16 height;
        height            = 0;
        width             = mUnk_22;
        MessageFile *file = mFile;
        text.Print(file->GetText((u16) mMessage), file->GetLength((u16) mMessage), mBuffer, 0x28, 2, &width, &height,
                   &mUnk_20, 0, 2, 0x11, 0x10, 0, 0, 0, 1, 1);
        func_02025c1c(SpriteManager::Get().Screen(mScreen).mBgIndex, 4, mVram, mBuffer, 0x800, 0);
        BOOL negative = mUnk_22 < 0;
        BOOL last     = mUnk_20 == -mUnk_22;
        if (negative & last) {
            mUnk_22 = mWidth;
        }
    }
    SpriteScreen &screen = SpriteManager::Get().Screen(mScreen);
    u8 i                 = 0;
    do {
        screen.func_02027778(data_020a10a0, 1, (s16)(mX + i * 16), (s16)(mY + 1), mUnk_27, (u16)(i + (mVram >> 7)), 0xf, 0);
        i++;
    } while (i < ((u32) mWidth >> 4));
}

void MessageWindow::func_02065bcc(u32 vram) {
    mVram = vram;
}

void MessageWindow::func_02065bd4(u32 unk1, u32 unk2, u32 unk3) {
    mX      = unk1;
    mY      = unk2;
    mUnk_27 = unk3;
}

// non-matching: the original clears the low nibble with lsr/lsl; every form here folds to bic.
void MessageWindow::func_02065be4(u32 unk1) {
    mWidth = (unk1 >> 4) << 4;
}

extern u8 data_020a15ac[][6];
extern u8 data_020a1258[][4];
extern u16 data_020a10ac[];

u8 func_02065bf4(u32 id, u32 index) {
    return data_020a15ac[id][index];
}

int func_02065c0c(u32 id, u32 kind) {
    return data_020a1258[id][kind];
}

u16 func_02065c20(u32 id) {
    if (id < 0xd5) {
        return data_020a10ac[id];
    }
    return 0;
}
