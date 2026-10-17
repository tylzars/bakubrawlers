#include "UI/MessageWindow.hpp"

#include "Graphics/CellAnimation.hpp"
#include "Graphics/SpriteManager.hpp"
#include "System/FileSystem.hpp"
#include "Text/TextManager.hpp"

#include <nitro/mi.h>
#include <nitro/os.h>
#include <stdio.h>

// One 16x16 OAM cell, drawn once per 16 pixels of the window's width.
static const CellAnimOam sMessageWindowOam = {0, 0x4000, 0};

MessageWindow::MessageWindow(const char *name, u32 unk2, u32 unk3) {
    mFile       = NULL;
    mBuffer     = NULL;
    mGlyphs     = NULL;
    mBufferSize = 0;
    mGlyphCount = 0;
    mVram       = 0;
    mX          = 0;
    mY          = 0;
    mTextWidth     = 0;
    mTextX     = 0;
    mMessage    = -1;
    mUnk_27     = 0;
    mScreen     = unk2;
    mWidth      = 0xc0;
    mTimer      = 0;
    mLoopShortText     = 0;
    SetVram(unk3);
    LoadMessageFile(name);
    InitText();
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

void MessageWindow::LoadMessageFile(const char *name) {
    char path[0x80];
    if (mFile != NULL) {
        OS_Free(mFile);
    }
    sprintf(path, "/font/%s.mes", name);
    mFile = (MessageFile *) LoadFile(path, 0, 0, 0, 0, 0);
}

void MessageWindow::InitText() {
    mBufferSize = TextManager::Get().BufferSize(0x28, 2);
    mBuffer     = OS_Alloc(mBufferSize);
    mGlyphCount = TextManager::Get().GlyphCount(0x14);
    mGlyphs     = OS_Alloc(mGlyphCount * 0xc);
    TextManager::Get().Init(mGlyphs, mScreen, 0, 0x28, 2, 0x11);
    UploadTextPalette();
    if (mScreen == 0) {
        MI_CpuClearFast((void *) (mVram + 0x6400000), 0x800);
    } else {
        MI_CpuClearFast((void *) (mVram + 0x6600000), 0x800);
    }
    ClearBuffer();
}

void MessageWindow::UploadTextPalette() {
    TextManager::Get().SetColor(mScreen, 0, 0xf);
}

void MessageWindow::ClearBuffer() {
    MI_CpuClear16(mBuffer, mBufferSize);
}

void MessageWindow::StartMessage(u32 unk1, u32 unk2, u32 unk3) {
    mMessage = unk1;
    mLoopShortText  = unk2;
    ClearBuffer();
    mUnk_26 = 0;
    mTimer  = 0;
    mTextX = unk3 != 0 ? mWidth : 2;
    mTextWidth = mWidth;
}

enum ScreenIndex { SCREEN_INDEX_0 };

void MessageWindow::Update() {
    if (mMessage < 0) {
        return;
    }
    u32 flag = mTextWidth < mWidth ? mLoopShortText : 1;
    if (mTimer != 0) {
        mTimer--;
    } else if (!((mTextX == 0) & (flag == 0))) {
        mTextX--;
        if (mTextX == 0) {
            mTimer = 0x3c;
        }
        ClearBuffer();
        TextManager &text = TextManager::Get();
        s16 width;
        s16 height;
        height            = 0;
        width             = mTextX;
        MessageFile *file = mFile;
        text.Print(file->GetText((u16) mMessage), file->GetLength((u16) mMessage), mBuffer, 0x28, 2, &width, &height,
                   &mTextWidth, 0, 2, 0x11, 0x10, 0, 0, 0, 1, 1);
        // The screen index is an enum-typed local (a real load, before the stack arguments); a plain read is folded
        // into the call and loaded after them.
        SpriteManager &sprites = SpriteManager::Get();
        ScreenIndex screen     = (ScreenIndex) mScreen;
        QueueUpload(sprites.Screen(screen).mBgIndex, UPLOAD_KIND_OBJ, mVram, mBuffer, 0x800, 0);
        BOOL negative = mTextX < 0;
        BOOL last     = mTextWidth == -mTextX;
        if (negative & last) {
            mTextX = mWidth;
        }
    }
    SpriteScreen &screen = SpriteManager::Get().Screen(mScreen);
    u8 i                 = 0;
    do {
        screen.AddOam((void *) &sMessageWindowOam, 1, (s16)(mX + i * 16), (s16)(mY + 1), mUnk_27, (u16)(i + (mVram >> 7)), 0xf, 0);
        i++;
    } while (i < mWidth / 16);
}

void MessageWindow::SetVram(u32 vram) {
    mVram = vram;
}

void MessageWindow::func_02065bd4(u32 unk1, u32 unk2, u32 unk3) {
    mX      = unk1;
    mY      = unk2;
    mUnk_27 = unk3;
}

void MessageWindow::SetWidth(u32 unk1) {
    mWidth = (s32) (unk1 >> 4) << 4;
}

extern const u8 gCardAttributeValues[][6];
extern const u8 gCardInfo[][4];
extern const u16 gCardNumbers[];

u8 GetCardAttributeValue(u32 id, u32 index) {
    return gCardAttributeValues[id][index];
}

u8 GetCardInfo(u32 id, u32 kind) {
    return gCardInfo[id][kind];
}

u16 GetCardNumber(u32 id) {
    if (id < 0xd5) {
        return gCardNumbers[id];
    }
    return 0;
}
