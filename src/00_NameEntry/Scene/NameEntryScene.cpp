#include "Scene/NameEntryScene.hpp"

#include "Game/UnkStruct_020d43f0.hpp"
#include "Graphics/CellAnimation.hpp"
#include "Graphics/Display.hpp"
#include "Graphics/SpriteManager.hpp"
#include "System/FileSystem.hpp"
#include "System/SoundManager.hpp"
#include "System/TouchManager.hpp"
#include "Text/TextManager.hpp"
#include "UI/MessageWindow.hpp"
#include "Unknown/UnkFunctions.hpp"
#include <nitro/gx.h>
#include <nitro/mi.h>
#include <nitro/os.h>

extern u16 data_ov000_02225fc8[]; // {0, 10, 50, 100, 1000, 5000}
extern u8 data_ov000_02226044[6][12];

NameEntryScene::NameEntryScene(u8 mode) :
    SceneBase(2, 0) {
    mCharTable = NULL;
    mMessages = NULL;
    mMessage = NULL;
    mTextBuffer = NULL;
    mGlyphBuffer = NULL;
    mTextBufferSize = 0;
    mGlyphCount = 0;
    mCellAnims[0] = NULL;
    mCellAnims[1] = NULL;
    mCellAnims[2] = NULL;
    mObj34 = NULL;
    mObj38 = NULL;
    mBuf3c = NULL;
    mBuf40 = NULL;
    mBuf44 = NULL;
    mBuf48 = NULL;
    mMaxLength = 12;
    mLength = 0;
    mUnk_66 = 0;
    mState = 0;
    mCol = 0;
    mRow = 0;
    mUnk_6a = 0;
    mTab = 0;
    mUnk_6c = 0;
    mMode = mode;
    mUnk_70 = -1;
    mUnk_74 = 0;
    MI_CpuClear16(mName, sizeof(mName));
}

NameEntryScene::~NameEntryScene() {
    if (mBuf3c != NULL) {
        OS_Free(mBuf3c);
    }
    if (mBuf40 != NULL) {
        OS_Free(mBuf40);
    }
    if (mBuf44 != NULL) {
        OS_Free(mBuf44);
    }
    if (mBuf48 != NULL) {
        OS_Free(mBuf48);
    }
    delete mObj34;
    delete mObj38;
    if (mCellAnims[0] != NULL) {
        delete mCellAnims[0];
    }
    if (mCellAnims[1] != NULL) {
        delete mCellAnims[1];
    }
    if (mCellAnims[2] != NULL) {
        delete mCellAnims[2];
    }
    delete mMessage;
    if (mCharTable != NULL) {
        OS_Free(mCharTable);
        mCharTable = NULL;
    }
    if (mMessages != NULL) {
        OS_Free(mMessages);
        mMessages = NULL;
    }
    if (mTextBuffer != NULL) {
        ClearTextBuffer();
        OS_Free(mTextBuffer);
        mTextBuffer = NULL;
    }
    if (mGlyphBuffer != NULL) {
        OS_Free(mGlyphBuffer);
        mGlyphBuffer = NULL;
    }
}

void NameEntryScene::InitDisplay() {
    GX_SetDispSelect(1);
    SpriteManager::Get().func_02026dec(1, 1);
    func_02001e68(1, 0, 0);
    func_02001ed0(0);
    G2S_SetBG0Control(0, 1, 0, 2, 0);
    G2S_SetBG1Control(0, 1, 2, 4, 0);
    G2S_SetBG2Control(0, 1, 4, 1, 0);
    G2S_SetBG3Control(0, 1, 6, 0xc, 0);
    G2S_SetBG0Offset(0, 0);
    G2S_SetBG1Offset(0, 0);
    G2S_SetBG2Offset(0, 0);
    G2S_SetBG3Offset(0, 0);
    G2S_SetBG0Priority(1);
    G2S_SetBG1Priority(3);
    G2S_SetBG2Priority(0);
    G2S_SetBG3Priority(3);
    GXS_SetVisiblePlane(0x13);
    G2S_SetBlendBrightness(0x1f, 0);
    GXS_SetVisibleWnd(0);
    G2S_SetWndOutsidePlane(0x1f, FALSE);
    G2S_SetWnd0InsidePlane(0x1f, FALSE);
    G2S_SetWnd1InsidePlane(0x1f, FALSE);
    G2S_SetWndOBJInsidePlane(0x1f, FALSE);
    G2S_SetWnd0Position(0, 0, 0, 0);
    G2S_SetWnd1Position(0, 0, 0, 0);
    G2S_BlendNone();
}

void NameEntryScene::func_ov000_0221b190() {
    switch (mTab) {
        case 0:
            func_02025c1c(1, 7, 0x1c0, mBuf3c + 0x1c0 + mUnk_66 * 0xc0, 0x300, 0);
            break;
        case 1:
            func_02025c1c(1, 7, 0x1c0, mBuf40 + 0x1c0 + mUnk_66 * 0xc0, 0x300, 0);
            break;
    }
}

void NameEntryScene::func_ov000_0221b22c() {
    switch (mTab) {
        case 0:
            func_02025c1c(1, 0xb, 0, mBuf44, 0x5000, 0);
            break;
        case 1:
            func_02025c1c(1, 0xb, 0, mBuf48, 0x5000, 0);
            break;
    }
}

// non-matching: 9 words, only register naming (original x-0x20 in r1/y in r0, ours r2/r1) in the two s16 subtractions.
void NameEntryScene::func_ov000_0221b2a0() {
    if (TouchManager::Get().mTouching == 0) {
        return;
    }
    s16 x = TouchManager::Get().mX;
    s16 y = TouchManager::Get().mY;
    s16 px = x - 0x20;
    s16 py = y - 0x38;
    if (px < 0 || py < 0) {
        return;
    }
    u8 rx = px % 24;
    u8 ry = py % 24;
    if (rx > 0x18 || ry > 0x16) {
        return;
    }
    u8 col = px / 24;
    u8 row = py / 24;
    if (col >= 8 || row >= 4) {
        return;
    }
    mCol = col;
    mRow = row;
    AddChar((u8) (mCol + (mRow + mUnk_66) * 8 + mTab * 0x60));
}

void NameEntryScene::Backspace() {
    if (mLength == 0) {
        return;
    }
    mLength--;
    mName[mLength] = 0;
    RenderName();
    if (mLength == 0) {
        mObj34->mUnk_130 = 0;
        mObj34->mMid->mInner->mUnk_ac = 0x8000;
        if (mState == 3) {
            mState = 2;
        }
    }
}

void NameEntryScene::AddChar(s32 index) {
    if (mLength == 0) {
        switch ((u8) (index % 0x60)) {
            case 0x1a:
            case 0x1b:
            case 0x1c:
            case 0x1d:
            case 0x1e:
            case 0x1f:
            case 0x3b:
            case 0x3c:
            case 0x3d:
            case 0x3e:
            case 0x3f:
            case 0x5f:
                return;
        }
    }
    if (mLength >= mMaxLength) {
        mLength = mMaxLength - 1;
    }
    u8 c = index % 0x60;
    TextManager::Get();
    const u16 *chars = (const u16 *) mCharTable->GetText(mTab);
    chars += c;
    mName[mLength] = *chars;
    mLength++;
    RenderName();
    SoundManager::Get().func_020246f8(0);
    mObj34->mUnk_130 = 1;
    mObj34->mMid->mInner->mUnk_ac = 8;
    mCellAnims[0]->func_02029fc0();
    if (mLength >= mMaxLength) {
        mCellAnims[2]->SetAnimation(9);
        mState = 3;
    } else {
        mCellAnims[2]->SetAnimation(8);
        mState = 0;
    }
}

void NameEntryScene::Load() {
    SpriteManager::Get().Screen(1).func_02027624(3);
    ChangeDir("/font/");
    mMessage = new MessageWindow("mes_name", 1, 0x3000);
    mMessage->func_02065bd4(0x14, 4, 0);
    mMessage->func_02065be4(0xd8);
    mMessage->func_02065890(0, 0, 1);
    mCharTable = (MessageFile *) func_02023504("mes_Alphabet.mes", 0, 0, 0, 0, 0);
    mMessages = (MessageFile *) func_02023504("mes_name.mes", 0, 0, 0, 0, 0);
    mTextBufferSize = TextManager::Get().BufferSize(0x18, 8);
    mTextBuffer = OS_Alloc(mTextBufferSize);
    MI_CpuClear16(mTextBuffer, mTextBufferSize);
    if (mMode == 0) {
        if (data_020d43f0[0].mName[0] != 0) {
            u8 i = 0;
            do {
                u16 c = data_020d43f0[0].mName[i];
                if (c == 0) {
                    break;
                }
                mName[i] = c;
                i++;
            } while (i < 12);
            mLength = i;
        } else {
            mLength = 0;
            mObj34->mUnk_130 = 0;
            mObj34->mMid->mInner->mUnk_ac = 0x8000;
            if (mState == 3) {
                mState = 2;
            }
        }
    }
    mGlyphCount = TextManager::Get().GlyphCount(3);
    mGlyphBuffer = OS_Alloc(mGlyphCount * 0xc);
    TextManager::Get().Init(mGlyphBuffer, 1, 0, 0x18, 8, 0x44);
    TextManager::Get().SetColor(1, 0, 0xe);
    if (mMode == 0) {
        RenderName();
    }
}

// non-matching: 5 words at the end. The original loads the SpriteManager guard before the TextManager::mUnk_5b = 0 store;
// ours stores first (same scheduling problem as StoreScene::PrintStatus).
void NameEntryScene::RenderName() {
    ClearTextBuffer();
    TextManager::Get().mUnk_5b = 1;
    u8 i = 0;
    do {
        s16 x;
        s16 y;
        u16 width;
        y = 0;
        width = 0;
        int base = i * 13;
        x = base;
        TextManager::Get().Print(&mName[i], 2, mTextBuffer, 0x18, 8, &x, &y, &width, 0, 2, 0x44, 0, 0, 0, 0, 1, 0);
        y = 0;
        x = base + (11 - width) / 2;
        TextManager::Get().Print(&mName[i], 2, mTextBuffer, 0x18, 8, &x, &y, 0, 0, 2, 0x44, 0, 0, 0, 0, 1, 1);
        i++;
    } while (i < mMaxLength);
    func_02005e9c(mTextBuffer, mTextBufferSize);
    TextManager &endText = TextManager::Get();
    endText.mUnk_5b = 0;
    func_02025c1c(SpriteManager::Get().Screen(1).mBgIndex, 4, 0x4000, mTextBuffer, mTextBufferSize, 0);
}

void NameEntryScene::func_ov000_0221bc98() {
    if (mMessage != NULL) {
        mMessage->func_020658d0();
    }
    SpriteManager::Get().Screen(1).func_02027778(mGlyphBuffer, mGlyphCount, 0x33, 0x1e, 0, 0x80, 0xe, 0);
}

void NameEntryScene::ClearTextBuffer() {
    if (mMessage != NULL) {
        mMessage->func_02065878();
    }
    if (mTextBuffer != NULL) {
        MI_CpuClear16(mTextBuffer, mTextBufferSize);
    }
}

// non-matching: 17 words in the inner loop. Original: row ptr r6, name char loaded before the table char, guard value in r2;
// ours has row in r5 and the guard in r1 and loads the table char first.
void NameEntryScene::CheckSpecialName() {
    BOOL match;
    mUnk_74 = 0;
    u32 value = data_020d43f0[0].mMoney - 3000;
    u32 digit1k = (value / 1000) % 10;
    u32 digit10k = (value / 10000) % 10;
    u32 digit100k = (value / 100000) % 10;
    for (u8 i = 0; i < 6; i++) {
        switch (i) {
            case 0:
                if (data_020d43f0[0].mUnk_5d2 != 0) {
                    continue;
                }
                break;
            case 1:
                if ((u8) digit1k == 1 || (u8) digit1k == 6) {
                    continue;
                }
                break;
            case 2:
                if ((u8) digit1k == 5 || (u8) digit1k == 6) {
                    continue;
                }
                break;
            case 3:
                if ((u8) digit10k == 1) {
                    continue;
                }
                break;
            case 4:
                if ((u8) digit100k == 1 || (u8) digit100k == 6) {
                    continue;
                }
                break;
            case 5:
                if ((u8) digit100k == 5 || (u8) digit100k == 6) {
                    continue;
                }
                break;
        }
        u8 c;
        u8 *row = data_ov000_02226044[i];
        u8 k = 0;
        match = TRUE;
        do {
            if (mName[k] == 0) {
                match = FALSE;
                break;
            }
            c = row[k] % 0x60;
            TextManager::Get();
            MessageFile *table = mCharTable;
            u16 tab = row[k] >= 0x60 ? 1 : 0;
            const u16 *chars = (const u16 *) table->GetText(tab);
            chars += c;
            if (mName[k] != *chars) {
                match = FALSE;
                break;
            }
            k++;
        } while (k < 12);
        if (match) {
            mUnk_74 = i + 1;
            return;
        }
    }
}

void NameEntryScene::PrintMessage() {
    ClearTextBuffer();
    int index;
    if (mUnk_74 == 1) {
        index = 1;
    } else {
        TextManager::Get().mUnk_10 = data_ov000_02225fc8[mUnk_74 - 1];
        index = 2;
    }
    TextManager &text = TextManager::Get();
    s16 height;
    s16 width;
    width = 0;
    height = 0;
    MessageFile *file = mMessages;
    text.Print(file->GetText(index), file->GetLength(index), mTextBuffer, 0x18, 8, &height, &width, 0, 0, 2, 0x44, 0x10, 0, 0, 1, 1, 1);
    func_02025c1c(SpriteManager::Get().Screen(1).mBgIndex, 4, 0x4000, mTextBuffer, mTextBufferSize, 0);
}

void NameEntryScene::func_ov000_0221c1e4() {
    SpriteManager::Get().Screen(1).func_02027778(mGlyphBuffer, mGlyphCount, 0x20, 0x38, 0, 0x80, 0xe, 0);
}
