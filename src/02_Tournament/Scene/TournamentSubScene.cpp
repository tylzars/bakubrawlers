#include "Scene/TournamentSubScene.hpp"
#include "UI/SelectWindow.hpp"

// The tables are defined before the singleton headers are parsed: the size sort then gives the original order of
// the cell paths in .data and of the singleton nodes in .bss (TextManager, TouchManager, InputManager, SoundManager).
struct CellPosByte {
    u8 x;
    u8 y;
};

static const WindowCellRecord sCells[9] = {
    {"Game/2D/union_b/union_b_obj_cockpit_nce.bin", 2, 0x2000, 3, 1, 1, {2, 3, 3, 0}},
    {"Game/Tournament/Obj/tournament_b_obj_01_nce.bin", 2, 0x0, 0, 1, 1, {2, 2, 2, 0}},
    {"Game/2D/union_b/union_b_obj_scroll_nce.bin", 4, 0x5000, 7, 1, 1, {4, 4, 4, 0}},
    {"Game/2D/union_b/union_b_obj_scroll_nce.bin", 0, 0x5000, 7, 1, 1, {0, 1, 1, 0}},
    {"Game/2D/union_b/union_b_obj_scroll_nce.bin", 2, 0x5000, 7, 1, 1, {2, 3, 3, 0}},
    {"Game/2D/union_b/union_b_obj_scroll_nce.bin", 5, 0x5000, 7, 1, 1, {5, 5, 5, 0}},
    {"Game/Tournament/Obj/tournament_b_obj_01_nce.bin", 0, 0x0, 0, 2, 1, {0, 1, 1, 0}},
    {"Game/Tournament/Obj/tournament_b_obj_01_nce.bin", 0, 0x0, 0, 2, 1, {0, 1, 1, 0}},
    {"Game/Tournament/Obj/tournament_b_obj_01_nce.bin", 0, 0x0, 0, 2, 1, {0, 1, 1, 0}},
};
static const CellPosByte sCellPos[3][3] = {
    {{0x07, 0x26}, {0x00, 0x00}, {0x00, 0x00}},
    {{0x07, 0x26}, {0x07, 0x46}, {0x00, 0x00}},
    {{0x07, 0x26}, {0x07, 0x46}, {0x07, 0x66}},
};
static const CellPosByte sTouchPos[3][3] = {
    {{0x07, 0x26}, {0x00, 0x00}, {0x00, 0x00}},
    {{0x07, 0x26}, {0x07, 0x46}, {0x00, 0x00}},
    {{0x07, 0x26}, {0x07, 0x46}, {0x07, 0x66}},
};
// Touch items, patched by Init. In the original they follow the cell paths, after the vtable, so they
// are a section of their own: a function-local static of an inline accessor gets one and keeps its place in the size
// sort.
inline WindowItemRecord *TouchItems() {
    static WindowItemRecord sItems[9] = {
        {0, 0x2, 225, 170, 30, 20, 0, {-1, -1, -1, -1}, 0, 1},
        {1, 0x8000, 32, 48, 0, 0, 0, {-1, -1, -1, -1}, 0, 0},
        {2, 0x10, 228, 47, 32, 12, 0, {-1, -1, -1, -1}, 0, 0},
        {3, 0x20, 228, 28, 32, 20, 0, {-1, -1, -1, -1}, 0, 3},
        {4, 0x40, 228, 144, 32, 20, 0, {-1, -1, -1, -1}, 0, 3},
        {5, 0x8000, 238, 36, 0, 0, 0, {-1, -1, -1, -1}, 0, 0},
        {6, 0x8000, 0, 0, 32, 32, 0, {-1, -1, -1, -1}, 0, 0},
        {7, 0x8000, 0, 0, 32, 32, 0, {-1, -1, -1, -1}, 0, 0},
        {8, 0x8000, 0, 0, 32, 32, 0, {-1, -1, -1, -1}, 0, 0},
    };
    return sItems;
}
// Placeholder shape: the original packs this table between the position tables and sCells at byte offsets, so it is
// in the shared .rodata section, which a plain u8 array (always a section of its own) is not. 24 bytes, of which the
// first 17 are used.
static const u8 sMessageIds[1][24] = {{0, 1, 2, 3, 4, 5, 6, 7, 0x0a, 8, 9, 0x0b, 0x0c, 0x0d, 0x10, 0x0e, 0x0f}};

#include "Game/UnkStruct_020d433c.hpp"
#include "Game/SaveData.hpp"
#include "Graphics/Display.hpp"
#include "Graphics/SpriteManager.hpp"
#include "System/FileSystem.hpp"
#include "System/InputManager.hpp"
#include "System/TouchManager.hpp"
#include "System/SoundManager.hpp"
#include "Text/TextManager.hpp"
#include "UI/MessageWindow.hpp"
#include "Graphics/CharacterIcons.hpp"
#include "Scene/RankingCharacterView.hpp"
#include "Game/Ranking.hpp"

#include <nitro/gx.h>
#include <nitro/mi.h>
#include <nitro/os.h>
#include <stdio.h>

TournamentSubScene::TournamentSubScene() :
    SceneBase(1000, 0) {
    mRanking         = NULL;
    mCharacterView         = NULL;
    mMessages       = NULL;
    mMessageWindow         = NULL;
    mCharacterIcons         = NULL;
    mResult         = 0;
    mTextLineCount         = 0;
    mScroll         = -1;
    mSelected       = 0;
    mState          = 0;
    mCounter        = 0;
    mUnk_58         = 0;
    mUnk_54 = gSaveData[0].mUnk_42 > 6;
    mCount = data_020d433c.mPlayerCount - 1;
    u32 skip = 0;
    if (data_020d433c.mHasPartners != 0) {
        mCount--;
        skip = 1;
    }
    u8 i = 0;
    do {
        mEntries[i].mId     = data_020d433c.mCharacterIds[1 + skip + i];
        mEntries[i].mRank = 0;
        mEntries[i].mScore = 0;
        i++;
    } while (i < mCount);
    mTextBuffer     = NULL;
    mGlyphBuffer    = NULL;
    mTextBufferSize = 0;
    mGlyphCount     = 0;
}



TournamentSubScene::~TournamentSubScene() {
    delete mRanking;
    delete mWindow;
    if (mCharacterIcons != NULL) {
        delete mCharacterIcons;
    }
    if (mMessages != NULL) {
        OS_Free(mMessages);
    }
    ClearTextBuffer();
    OS_Free(mTextBuffer);
    if (mGlyphBuffer != NULL) {
        OS_Free(mGlyphBuffer);
    }
    delete mMessageWindow;
    MI_CpuClearFast((void *) 0x6000000, 0x80000);
    MI_CpuClearFast((void *) 0x6200000, 0x20000);
    SpriteManager::Get().ResetSlots();
}


void TournamentSubScene::Update() {
    BOOL input = TRUE;
    switch (mState) {
        case 0:
            input = FALSE;
            InitDisplay();
            Init();
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
                mCharacterView->mFinishRequested = 1;
                mState           = 4;
            } else {
                if (UpdateEntrySelection() != -1) {
                    u8 id = mEntries[mSelected].mId;
                    if (id != mCharacterView->mCharacterId) {
                        mCharacterView->ChangeCharacter(id, mEntries[mSelected].mRank,
                                               mEntries[mSelected].mScore);
                        mState = 3;
                    }
                }
            }
            func_ov002_02220618();
            break;
        case 3:
            if (mCharacterView->mChanging == 0) {
                SetupProfile();
                mState = 2;
            }
            break;
        case 4:
            if (mWindow->mState == 4) {
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


void TournamentSubScene::InitDisplay() {
    MI_CpuClearFast((void *) 0x6000000, 0x80000);
    MI_CpuClearFast((void *) 0x6200000, 0x20000);
    reg_GX_POWCNT |= 0x8000;
    SpriteManager::Get().SetupMode(0, 7);
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
void TournamentSubScene::Init() {
    mRanking = new Ranking();
    u8 i = 0;
    do {
        UnkStruct_020758f4_Record *info = mRanking->FindEntry(mEntries[i].mId);
        if (info != NULL) {
            mEntries[i].mRank = info->mTextOffset;
            mEntries[i].mScore = info->mTextLength;
        }
        i++;
    } while (i < mCount);
    mCharacterView = new RankingCharacterView(mEntries[mSelected].mId, mEntries[mSelected].mRank,
                                     mEntries[mSelected].mScore, 1);
    ChangeDir("/Game/Myroom/");
    LoadAndQueueUpload(1, 7, 0, "myroom_b_bg_00_nsc.bin", 0, 0, 0);
    LoadAndQueueUpload(1, 0xb, 0, "myroom_b_bg_00_ncg.bin", 0, 0, 0);
    LoadAndQueueUpload(1, 1, 0, "myroom_b_bg_00_ncl.bin", 0, 0, 0);
    ChangeDir("/Game/Tournament/Bg/");
    LoadAndQueueUpload(1, 8, 0, "tournament_b_bg_01_nsc.bin", 0, 0, 0);
    LoadAndQueueUpload(1, 0xc, 0, "tournament_b_bg_01_ncg.bin", 0, 0, 0);
    LoadAndQueueUpload(1, 1, 0x2000, "tournament_b_bg_01_ncl.bin", 0, 0, 0);
    u8 count = mCount;
    u8 n = 9 - (3 - count);
    u8 j;
    j = 0;
    do {
        (&TouchItems()[j])[6].mX = sTouchPos[mCount - 1][j].x;
        (&TouchItems()[j])[6].mY = sTouchPos[mCount - 1][j].y;
        j++;
    } while (j < mCount);
    mWindow = NULL;
    mWindow = new SelectWindow(n, TouchItems(), -1);
    SpriteScreen &screen = SpriteManager::Get().Screen(1);
    screen.SetObjVramMode(3);
    ChangeDir("/");
    screen.LoadCharacters("Game/Tournament/Obj/tournament_b_obj_01.nce_ncg1D.bin", 0, 0);
    screen.LoadPalette("Game/Tournament/Obj/tournament_b_obj_01_ncl.bin", 0, 0);
    screen.LoadCharacters("Game/2D/union_b/union_b_obj_cockpit.nce_ncg1D.bin", 0x2000, 0);
    screen.LoadPalette("Game/2D/union_b/union_b_obj_cockpit_ncl.bin", 0x60, 0);
    screen.LoadCharacters("Game/2D/union_b/union_b_obj_scroll.nce_ncg1D.bin", 0x5000, 0);
    screen.LoadPalette("Game/2D/union_b/union_b_obj_scroll_ncl.bin", 0xe0, 0);
    mWindow->SetCells(n, sCells);
    mCharacterIcons = new CharacterIcons(0x4000, 4, 1);
    u8 k = 0;
    do {
        mCharacterIcons->SetCharacter(k, (s8) mEntries[k].mId, 0, 0);
        mCharacterIcons->SetVisible(k, 1);
        mCharacterIcons->SetDrawArgs(k, sTouchPos[mCount - 1][k].x + 2, sTouchPos[mCount - 1][k].y + 2, 0);
        k++;
    } while (k < mCount);
    ChangeDir("/font/");
    mMessages = (MessageFile *) LoadFile("mes_Profile.mes", 0, 0, 0, 0, 0);
    mTextBufferSize = TextManager::Get().BufferSize(0x14, 0xe);
    mGlyphCount     = TextManager::Get().GlyphCount(0x23);
    mTextBuffer     = OS_Alloc(mTextBufferSize);
    MI_CpuClear16(mTextBuffer, mTextBufferSize);
    mGlyphBuffer = OS_Alloc(mGlyphCount * 0xc);
    TextManager::Get().Init(mGlyphBuffer, 1, 0, 0x14, 0xe, 0x21);
    TextManager::Get().SetColor(1, 0, 0xf);
    SetupProfile();
    mMessageWindow = new MessageWindow("mes_tornament", 1, 0xc000);
    mMessageWindow->func_02065bd4(0x14, 4, 0);
    mMessageWindow->SetWidth(0xd8);
    mMessageWindow->StartMessage(6, 0, 1);
    ChangeDir("/");
}


void TournamentSubScene::Draw() {
    DrawSubScreen();
}


void TournamentSubScene::DrawSubScreen() {
    u8 i = 0;
    do {
        u8 j = 0;
        do {
            SpriteManager::Get().Screen(1).AddOam(mGlyphBuffer, 1, j * 0x20 + 0x3c, i * 0x10 + 0x2c, 2,
                                                          (u16) ((j + i * 5) * 2 + 0x100), 0xf, 0);
            j++;
        } while (j < 5);
        i++;
    } while (i < 7);
    SelectWindow *window = mWindow;
    s16 x = TouchItems()[2].mX;
    u8 y  = window->mCursor->mScrollItemY;
    window->mCellPos[2].x = x;
    window->mCellPos[2].y = y;
    mCharacterIcons->DrawIcons();
    mWindow->Draw();
    mMessageWindow->Update();
}


s32 TournamentSubScene::UpdateEntrySelection() {
    s32 result = -1;
    u8 i       = 0;
    do {
        if (TouchManager::Get().IsPressedIn((&TouchItems()[i])[6].mX, (&TouchItems()[i])[6].mY,
                                              (&TouchItems()[i])[6].mWidth, (&TouchItems()[i])[6].mHeight)) {
            result = (s8) i;
        }
        i++;
    } while (i < mCount);
    if (result == -1) {
        result = UpdateEntryKeySelection();
    } else if (mSelected != result) {
        SoundManager::Get().StartSeqArc(5);
        mSelected = result;
    }
    return result;
}



s32 TournamentSubScene::UpdateEntryKeySelection() {
    s8 next = -1;
    if (InputManager::Get().IsTrigger(KEY_UP)) {
        if (mSelected <= 0) {
            next = mCount - 1;
        } else {
            next = mSelected - 1;
        }
    } else if (InputManager::Get().IsTrigger(KEY_DOWN)) {
        if (mSelected >= mCount - 1) {
            next = 0;
        } else {
            next = mSelected + 1;
        }
    }
    if (next != -1) {
        if (mCount != 1) {
            SoundManager::Get().StartSeqArc(5);
        } else {
            if (InputManager::Get().IsHeld(KEY_DOWN) | InputManager::Get().IsHeld(KEY_UP)) {
                SoundManager::Get().StartSeqArc(2);
            }
        }
        mSelected = next;
    }
    return next;
}


void TournamentSubScene::func_ov002_02220618() {
    u8 x = sCellPos[mCount - 1][mSelected].x;
    u8 y = sCellPos[mCount - 1][mSelected].y;
    SelectWindow *window = mWindow;
    window->mCellPos[1].x = x;
    window->mCellPos[1].y = y;
    u8 cursor = mWindow->mCursor->mScroll;
    if (mScroll != cursor) {
        mScroll = cursor;
        PrintProfile();
    }
}



void TournamentSubScene::PrintProfile() {
    MI_CpuClear16(mTextBuffer, mTextBufferSize);
    TextManager &text = TextManager::Get();
    s16 height;
    s16 width;
    width  = mScroll * -14;
    height = 0;
    u8 index          = sMessageIds[0][mEntries[mSelected].mId];
    MessageFile *file = mMessages;
    u16 length        = file->GetLength(index);
    const u8 *message = file->GetText(index);
    void *buffer      = mTextBuffer;
    u16 half          = 0xffff;
    if (length < half * 2) {
        half = length / 2;
    }
    text.mLineStartCount = 0x10;
    text.Print(message, half * 2, buffer, 0x14, 0xe, &height, &width, 0, 0, 2, 0x21, 0xe, 0, 0, 0, 1, 1);
    QueueUpload(SpriteManager::Get().Screen(1).mBgIndex, 4, 0x8000, mTextBuffer, mTextBufferSize, 0);
}


void TournamentSubScene::SetupProfile() {
    u16 pixelWidth;
    s16 height;
    s16 width;
    TextManager &text = TextManager::Get();
    width  = 0;
    height = 0;
    u8 index          = sMessageIds[0][mEntries[mSelected].mId];
    MessageFile *file;
    void *buffer = mTextBuffer;
    file         = mMessages;
    MI_CpuClear16(text.mLineStarts, 0x20);
    text.mLineStartCount = 1;
    text.Print(file->GetText(index), file->GetLength(index), buffer, 0x14, 0xe, &height, &width, &pixelWidth, 0, 2,
               0x21, 0xe, 0, 1, 0, 1, 1);
    u16 w = width;
    for (u8 i = 1; i < 0x10; i++) {
        if (text.mUnk_36[i + 1] == 0) {
            text.mUnk_36[i + 1] = text.mUnk_36[i];
        }
    }
    mTextLineCount = w / 14;
    u32 count = mTextLineCount;
    if (count <= 8) {
        count = 8;
    }
    mWindow->mCursor->SetupScroll(0x30, 0x90, 8, count);
    mWindow->mCursor->SetScroll(0);
    if (mTextLineCount > 8) {
        mWindow->mButtons[2].mEnabled = 1;
    } else {
        mWindow->mButtons[2].mEnabled = 0;
    }
    mScroll = 0;
    PrintProfile();
}


void TournamentSubScene::ClearTextBuffer() {
    MI_CpuClear16(mTextBuffer, mTextBufferSize);
}

