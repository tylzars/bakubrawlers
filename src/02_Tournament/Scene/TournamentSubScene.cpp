#include "Scene/TournamentSubScene.hpp"
#include "Game/UnkStruct_020d433c.hpp"
#include "Game/UnkStruct_020d43f0.hpp"
#include "Graphics/Display.hpp"
#include "Graphics/SpriteManager.hpp"
#include "System/FileSystem.hpp"
#include "System/InputManager.hpp"
#include "System/SoundManager.hpp"
#include "System/TouchManager.hpp"
#include "Text/TextManager.hpp"
#include "UI/MessageWindow.hpp"
#include "UI/SelectWindow.hpp"
#include "Unknown/UnkStruct_020651bc.hpp"
#include "Unknown/UnkStruct_02074938.hpp"
#include "Unknown/UnkStruct_020758f4.hpp"

#include <nitro/gx.h>
#include <nitro/mi.h>
#include <nitro/os.h>
#include <stdio.h>

extern const WindowCellRecord data_ov002_02220ed8[];
extern const u8 data_ov002_02220eae[];
extern WindowItemRecord data_ov002_02221c84[9]; // touch items, patched by func_ov002_0221fb24


TournamentSubScene::TournamentSubScene() :
    SceneBase(1000, 0) {
    mUnk_0c         = NULL;
    mUnk_10         = NULL;
    mMessages       = NULL;
    mMessage         = NULL;
    mUnk_48         = NULL;
    mResult         = 0;
    mUnk_50         = 0;
    mUnk_51         = -1;
    mSelected       = 0;
    mState          = 0;
    mCounter        = 0;
    mUnk_58         = 0;
    mUnk_54 = data_020d43f0[0].mUnk_42 > 6;
    mCount = data_020d433c.mPlayerCount - 1;
    u32 skip = 0;
    if (data_020d433c.mUnk_98 != 0) {
        mCount--;
        skip = 1;
    }
    u8 i = 0;
    do {
        mEntries[i].mId     = data_020d433c.mUnk_9f[1 + skip + i];
        mEntries[i].mUnk_02 = 0;
        mEntries[i].mUnk_04 = 0;
        i++;
    } while (i < mCount);
    mTextBuffer     = NULL;
    mGlyphBuffer    = NULL;
    mTextBufferSize = 0;
    mGlyphCount     = 0;
}



TournamentSubScene::~TournamentSubScene() {
    delete mUnk_0c;
    delete mWindow;
    if (mUnk_48 != NULL) {
        delete mUnk_48;
    }
    if (mMessages != NULL) {
        OS_Free(mMessages);
    }
    ClearTextBuffer();
    OS_Free(mTextBuffer);
    if (mGlyphBuffer != NULL) {
        OS_Free(mGlyphBuffer);
    }
    delete mMessage;
    MI_CpuClearFast((void *) 0x6000000, 0x80000);
    MI_CpuClearFast((void *) 0x6200000, 0x20000);
    SpriteManager::Get().func_0202740c();
}


void TournamentSubScene::Update() {
    BOOL input = TRUE;
    switch (mState) {
        case 0:
            input = FALSE;
            func_ov002_0221f930();
            func_ov002_0221fb24();
            mCounter = 10;
            mState++;
            break;
        case 1:
            input = FALSE;
            mCounter--;
            if (mCounter <= 0) {
                SpriteManager::Get().StartFade(2);
                mState++;
            }
            break;
        case 2:
            if (mUnk_58 != 0) {
                SpriteManager::Get().StartFade(3);
                mUnk_10->mUnk_84 = 1;
                mState           = 4;
            } else {
                if (func_ov002_022202cc() != -1) {
                    u8 id = mEntries[mSelected].mId;
                    if (id != mUnk_10->mUnk_58) {
                        mUnk_10->func_02074938(id, mEntries[mSelected].mUnk_02,
                                               mEntries[mSelected].mUnk_04);
                        mState = 3;
                    }
                }
            }
            func_ov002_02220618();
            break;
        case 3:
            if (mUnk_10->mUnk_80 == 0) {
                func_ov002_02220830();
                mState = 2;
            }
            break;
        case 4:
            if (mWindow->mUnk_061 == 4) {
                if (!SpriteManager::Get().IsFading()) {
                    mState = 5;
                }
            }
            break;
        case 5:
            input = FALSE;
            RequestScene(1);
            mFinished = TRUE;
            break;
    }
    if (input) {
        mResult = mWindow->Update(TRUE);
        if (mResult == 2) {
            mUnk_58 = 1;
        }
        Draw();
    }
}


void TournamentSubScene::func_ov002_0221f930() {
    MI_CpuClearFast((void *) 0x6000000, 0x80000);
    MI_CpuClearFast((void *) 0x6200000, 0x20000);
    reg_GX_POWCNT |= 0x8000;
    SpriteManager::Get().func_02026dec(0, 7);
    GX_SetGraphicsMode(1, 0, 1);
    GXS_SetGraphicsMode(0);
    G2S_SetBG0Control(0, 1, 0, 1, 0);
    G2S_SetBG1Control(0, 1, 2, 4, 0);
    G2S_SetBG0Offset(0, 0);
    G2S_SetBG1Offset(0, 0);
    G2S_SetBG2Offset(0, 0);
    G2S_SetBG3Offset(0, 0);
    G2S_SetBG0Priority(3);
    G2S_SetBG1Priority(2);
    G2S_SetBG2Priority(3);
    G2S_SetBG3Priority(3);
    GX_SetVisiblePlane(0x1f);
    GXS_SetVisiblePlane(0x13);
    G2_SetBlendBrightness(0x1f, 0);
    G2S_SetBlendBrightness(0x1f, 0);
    GXS_SetVisibleWnd(0);
    G2S_SetWndOutsidePlane(0x1f, FALSE);
    G2S_SetWnd0InsidePlane(0x1f, FALSE);
    G2S_SetWnd1InsidePlane(0x1f, FALSE);
    G2S_SetWndOBJInsidePlane(0x1f, FALSE);
    G2S_SetWnd0Position(0, 0, 0, 0);
    G2S_SetWnd1Position(0, 0, 0, 0);
    G2_BlendNone();
    G2S_BlendNone();
}

// UnkStruct_020758f4_Record::mTextLength must be u32 (loaded with ldr).
void TournamentSubScene::func_ov002_0221fb24() {
    mUnk_0c = new UnkStruct_020758f4();
    u8 i = 0;
    do {
        UnkStruct_020758f4_Record *info = mUnk_0c->func_02075998(mEntries[i].mId);
        if (info != NULL) {
            mEntries[i].mUnk_02 = info->mTextOffset;
            mEntries[i].mUnk_04 = info->mTextLength;
        }
        i++;
    } while (i < mCount);
    mUnk_10 = new UnkStruct_02074938(mEntries[mSelected].mId, mEntries[mSelected].mUnk_02,
                                     mEntries[mSelected].mUnk_04, 1);
    ChangeDir("/Game/Myroom/");
    func_02025b64(1, 7, 0, "myroom_b_bg_00_nsc.bin", 0, 0, 0);
    func_02025b64(1, 0xb, 0, "myroom_b_bg_00_ncg.bin", 0, 0, 0);
    func_02025b64(1, 1, 0, "myroom_b_bg_00_ncl.bin", 0, 0, 0);
    ChangeDir("/Game/Tournament/Bg/");
    func_02025b64(1, 8, 0, "tournament_b_bg_01_nsc.bin", 0, 0, 0);
    func_02025b64(1, 0xc, 0, "tournament_b_bg_01_ncg.bin", 0, 0, 0);
    func_02025b64(1, 1, 0x2000, "tournament_b_bg_01_ncl.bin", 0, 0, 0);
    u8 count = mCount;
    u8 n = 9 - (3 - count);
    u8 j;
    j = 0;
    do {
        (&data_ov002_02221c84[j])[6].mX = *(data_ov002_02220eae + mCount * 6 + j * 2 - 6);
        (&data_ov002_02221c84[j])[6].mY = *(data_ov002_02220eae + mCount * 6 + j * 2 - 5);
        j++;
    } while (j < mCount);
    mWindow = NULL;
    mWindow = new SelectWindow(n, data_ov002_02221c84, -1);
    SpriteScreen &screen = SpriteManager::Get().Screen(1);
    screen.func_02027624(3);
    ChangeDir("/");
    screen.LoadCharacters("Game/Tournament/Obj/tournament_b_obj_01.nce_ncg1D.bin", 0, 0);
    screen.LoadPalette("Game/Tournament/Obj/tournament_b_obj_01_ncl.bin", 0, 0);
    screen.LoadCharacters("Game/2D/union_b/union_b_obj_cockpit.nce_ncg1D.bin", 0x2000, 0);
    screen.LoadPalette("Game/2D/union_b/union_b_obj_cockpit_ncl.bin", 0x60, 0);
    screen.LoadCharacters("Game/2D/union_b/union_b_obj_scroll.nce_ncg1D.bin", 0x5000, 0);
    screen.LoadPalette("Game/2D/union_b/union_b_obj_scroll_ncl.bin", 0xe0, 0);
    mWindow->SetCells(n, data_ov002_02220ed8);
    mUnk_48 = new UnkStruct_020651bc(0x4000, 4, 1);
    u8 k = 0;
    do {
        mUnk_48->func_02065204(k, (s8) mEntries[k].mId, 0, 0);
        mUnk_48->func_020654f8(k, 1);
        const u8 *pos = data_ov002_02220eae + mCount * 6 + k * 2;
        mUnk_48->func_0206547c(k, pos[-6] + 2, pos[-5] + 2, 0);
        k++;
    } while (k < mCount);
    ChangeDir("/font/");
    mMessages = (MessageFile *) func_02023504("mes_Profile.mes", 0, 0, 0, 0, 0);
    mTextBufferSize = TextManager::Get().BufferSize(0x14, 0xe);
    mGlyphCount     = TextManager::Get().GlyphCount(0x23);
    mTextBuffer     = OS_Alloc(mTextBufferSize);
    MI_CpuClear16(mTextBuffer, mTextBufferSize);
    mGlyphBuffer = OS_Alloc(mGlyphCount * 0xc);
    TextManager::Get().Init(mGlyphBuffer, 1, 0, 0x14, 0xe, 0x21);
    TextManager::Get().SetColor(1, 0, 0xf);
    func_ov002_02220830();
    mMessage = new MessageWindow("mes_tornament", 1, 0xc000);
    mMessage->func_02065bd4(0x14, 4, 0);
    mMessage->func_02065be4(0xd8);
    mMessage->func_02065890(6, 0, 1);
    ChangeDir("/");
}


void TournamentSubScene::Draw() {
    func_ov002_02220184();
}


void TournamentSubScene::func_ov002_02220184() {
    u8 i = 0;
    do {
        u8 j = 0;
        do {
            SpriteManager::Get().Screen(1).func_02027778(mGlyphBuffer, 1, j * 0x20 + 0x3c, i * 0x10 + 0x2c, 2,
                                                          (u16) ((j + i * 5) * 2 + 0x100), 0xf, 0);
            j++;
        } while (j < 5);
        i++;
    } while (i < 7);
    SelectWindow *window = mWindow;
    s16 x = data_ov002_02221c84[2].mX;
    u8 y  = window->mCursor->mUnk_0b;
    window->mCellPos[2].x = x;
    window->mCellPos[2].y = y;
    mUnk_48->func_020654a0();
    mWindow->Draw();
    mMessage->func_020658d0();
}


s32 TournamentSubScene::func_ov002_022202cc() {
    s32 result = -1;
    u8 i       = 0;
    do {
        if (TouchManager::Get().func_02022860((&data_ov002_02221c84[i])[6].mX, (&data_ov002_02221c84[i])[6].mY,
                                              (&data_ov002_02221c84[i])[6].mWidth, (&data_ov002_02221c84[i])[6].mHeight)) {
            result = (s8) i;
        }
        i++;
    } while (i < mCount);
    if (result == -1) {
        result = func_ov002_022203ec();
    } else if (mSelected != result) {
        SoundManager::Get().func_020246f8(5);
        mSelected = result;
    }
    return result;
}



s32 TournamentSubScene::func_ov002_022203ec() {
    s8 next = -1;
    if (InputManager::Get().IsTrigger(0x40)) {
        if (mSelected <= 0) {
            next = mCount - 1;
        } else {
            next = mSelected - 1;
        }
    } else if (InputManager::Get().IsTrigger(0x80)) {
        if (mSelected >= mCount - 1) {
            next = 0;
        } else {
            next = mSelected + 1;
        }
    }
    if (next != -1) {
        if (mCount != 1) {
            SoundManager::Get().func_020246f8(5);
        } else {
            if (InputManager::Get().IsHeld(0x80) | InputManager::Get().IsHeld(0x40)) {
                SoundManager::Get().func_020246f8(2);
            }
        }
        mSelected = next;
    }
    return next;
}


struct CellPosByte {
    u8 x;
    u8 y;
};

static const CellPosByte sCellPos[3][3] = {
    {{0x07, 0x26}, {0x00, 0x00}, {0x00, 0x00}},
    {{0x07, 0x26}, {0x07, 0x46}, {0x00, 0x00}},
    {{0x07, 0x26}, {0x07, 0x46}, {0x07, 0x66}},
};

void TournamentSubScene::func_ov002_02220618() {
    u8 x = sCellPos[mCount - 1][mSelected].x;
    u8 y = sCellPos[mCount - 1][mSelected].y;
    SelectWindow *window = mWindow;
    window->mCellPos[1].x = x;
    window->mCellPos[1].y = y;
    u8 cursor = mWindow->mCursor->mScroll;
    if (mUnk_51 != cursor) {
        mUnk_51 = cursor;
        func_ov002_02220680();
    }
}


static const u8 sMessageIds[17] = {0, 1, 2, 3, 4, 5, 6, 7, 0x0a, 8, 9, 0x0b, 0x0c, 0x0d, 0x10, 0x0e, 0x0f};

void TournamentSubScene::func_ov002_02220680() {
    MI_CpuClear16(mTextBuffer, mTextBufferSize);
    TextManager &text = TextManager::Get();
    s16 height;
    s16 width;
    width  = mUnk_51 * -14;
    height = 0;
    u8 index          = sMessageIds[mEntries[mSelected].mId];
    MessageFile *file = mMessages;
    u16 length        = file->GetLength(index);
    const u8 *message = file->GetText(index);
    void *buffer      = mTextBuffer;
    u16 half          = 0xffff;
    if (length < half * 2) {
        half = length / 2;
    }
    text.mUnk_58 = 0x10;
    text.Print(message, half * 2, buffer, 0x14, 0xe, &height, &width, 0, 0, 2, 0x21, 0xe, 0, 0, 0, 1, 1);
    func_02025c1c(SpriteManager::Get().Screen(1).mBgIndex, 4, 0x8000, mTextBuffer, mTextBufferSize, 0);
}


void TournamentSubScene::func_ov002_02220830() {
    u16 pixelWidth;
    s16 height;
    s16 width;
    TextManager &text = TextManager::Get();
    width  = 0;
    height = 0;
    u8 index          = sMessageIds[mEntries[mSelected].mId];
    MessageFile *file;
    void *buffer = mTextBuffer;
    file         = mMessages;
    MI_CpuClear16(text.mPalette, 0x20);
    text.mUnk_58 = 1;
    text.Print(file->GetText(index), file->GetLength(index), buffer, 0x14, 0xe, &height, &width, &pixelWidth, 0, 2,
               0x21, 0xe, 0, 1, 0, 1, 1);
    u16 w = width;
    for (u8 i = 1; i < 0x10; i++) {
        if (text.mUnk_36[i + 1] == 0) {
            text.mUnk_36[i + 1] = text.mUnk_36[i];
        }
    }
    mUnk_50 = w / 14;
    u32 count = mUnk_50;
    if (count <= 8) {
        count = 8;
    }
    mWindow->mCursor->func_02030474(0x30, 0x90, 8, count);
    mWindow->mCursor->SetScroll(0);
    if (mUnk_50 > 8) {
        mWindow->mButtons[2].mEnabled = 1;
    } else {
        mWindow->mButtons[2].mEnabled = 0;
    }
    mUnk_51 = 0;
    func_ov002_02220680();
}


void TournamentSubScene::ClearTextBuffer() {
    MI_CpuClear16(mTextBuffer, mTextBufferSize);
}

