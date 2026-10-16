#include "UI/MessageWindow.hpp"

#include "Graphics/CellAnimation.hpp"
#include "Graphics/SpriteManager.hpp"
#include "System/FileSystem.hpp"
#include "Text/TextManager.hpp"

#include <nitro/mi.h>
#include <nitro/os.h>
#include <stdio.h>

// One 16x16 OAM cell, drawn once per 16 pixels of the window's width.
static const CellAnimOam data_020a10a0 = {0, 0x4000, 0};

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

enum ScreenIndex { SCREEN_INDEX_0 };

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
        // The screen index is an enum-typed local (a real load, before the stack arguments); a plain read is folded
        // into the call and loaded after them.
        SpriteManager &sprites = SpriteManager::Get();
        ScreenIndex screen     = (ScreenIndex) mScreen;
        func_02025c1c(sprites.Screen(screen).mBgIndex, 4, mVram, mBuffer, 0x800, 0);
        BOOL negative = mUnk_22 < 0;
        BOOL last     = mUnk_20 == -mUnk_22;
        if (negative & last) {
            mUnk_22 = mWidth;
        }
    }
    SpriteScreen &screen = SpriteManager::Get().Screen(mScreen);
    u8 i                 = 0;
    do {
        screen.func_02027778((void *) &data_020a10a0, 1, (s16)(mX + i * 16), (s16)(mY + 1), mUnk_27, (u16)(i + (mVram >> 7)), 0xf, 0);
        i++;
    } while (i < mWidth / 16);
}

void MessageWindow::func_02065bcc(u32 vram) {
    mVram = vram;
}

void MessageWindow::func_02065bd4(u32 unk1, u32 unk2, u32 unk3) {
    mX      = unk1;
    mY      = unk2;
    mUnk_27 = unk3;
}

void MessageWindow::func_02065be4(u32 unk1) {
    mWidth = (s32) (unk1 >> 4) << 4;
}

extern const u8 data_020a15ac[][6];
extern const u8 data_020a1258[][4];
extern const u16 data_020a10ac[];

u8 func_02065bf4(u32 id, u32 index) {
    return data_020a15ac[id][index];
}

u8 GetCardInfo(u32 id, u32 kind) {
    return data_020a1258[id][kind];
}

u16 func_02065c20(u32 id) {
    if (id < 0xd5) {
        return data_020a10ac[id];
    }
    return 0;
}
