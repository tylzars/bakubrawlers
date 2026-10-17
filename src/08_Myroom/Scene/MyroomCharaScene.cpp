#include "Scene/MyroomCharaScene.hpp"
#include "Game/SaveData.hpp"
#include "Game/UnkStruct_020d48f0.hpp"
#include "Graphics/CellAnimation.hpp"
#include "Graphics/Display.hpp"
#include "Graphics/SpriteManager.hpp"
#include "Scene/MyroomStatusScene.hpp"
#include "System/FileSystem.hpp"
#include "System/InputManager.hpp"
#include "System/SoundManager.hpp"
#include "UI/MessageWindow.hpp"
#include "UI/SelectWindow.hpp"
#include "Unknown/UnkFunctions.hpp"

#include <nitro/gx.h>
#include <nitro/mi.h>
#include <nitro/os.h>
#include <stdio.h>

extern u16 data_020d444e[];

// Where each of the seven list lines is drawn, per tab (x, y and the right edge of the value column; the last row is
// unused).
struct CharaTextPos {
    u8 mX;
    u8 mY;
    u8 mRight;
};

const u32 sMyroomCharaLineVramOffsets[7] = {0x10000, 0x11000, 0x12000, 0x13000, 0x14000, 0x15000, 0x16000};

const WindowCellRecord sMyroomCharaStatusCells[4] = {
    {"myroom_status_b_obj_00_nce.bin", 1, 0, 0, 1, 1, {1, 2, 2, 0}},
    {"myroom_status_b_obj_00_nce.bin", 4, 0, 0, 1, 1, {4, 5, 5, 0}},
    {"myroom_status_b_obj_00_nce.bin", 10, 0, 0, 1, 1, {10, 11, 11, 0}},
    {"myroom_status_b_obj_00_nce.bin", 7, 0, 0, 1, 1, {7, 8, 8, 0}},
};

const WindowCellRecord sMyroomCharaCockpitCells[1] = {
    {"union_b_obj_cockpit_nce.bin", 2, 0x10000, 4, 1, 1, {2, 3, 3, 0}},
};

const CharaTextPos sMyroomCharaTextPos[5][7] = {
    {{0x6a, 0x37, 0x00}, {0xdc, 0x5b, 0x00}, {0xc4, 0x6d, 0x00}, {0xc4, 0x7f, 0x00}, {0xb8, 0x91, 0x00}},
    {{0x3e, 0x37, 0x00}, {0xdc, 0x49, 0x00}, {0xdc, 0x5b, 0x00}, {0xca, 0x6d, 0x00},
     {0xd0, 0x7f, 0x00}, {0xd0, 0x91, 0x00}, {0xd0, 0xa3, 0x00}},
    {{0xd0, 0x7f, 0x00}, {0xd0, 0x91, 0x00}, {0xd0, 0xa3, 0x00}},
    {{0x12, 0x37, 0xd0}, {0x12, 0x49, 0xd0}, {0x12, 0x5b, 0xd0}, {0x12, 0x6d, 0xd0},
     {0x12, 0x7f, 0xd0}, {0x12, 0x91, 0xd0}, {0x12, 0xa3, 0xd0}},
};

const WindowCellRecord sMyroomCharaPageCells[2] = {
    {"union_b_obj_page_nce.bin", 0, 0x12000, 5, 1, 1, {0, 1, 1, 0}},
    {"union_b_obj_page_nce.bin", 3, 0x12000, 5, 1, 1, {3, 4, 4, 0}},
};

// Room for 22 items; the window uses the first 7.
const WindowItemRecord sMyroomCharaWindowItems[22] = {
    {0, 0x8, 40, 44, 176, 20, 0x0, {3, 1, 0, 0}, 0, 0},
    {1, 0x8, 40, 72, 176, 20, 0x0, {0, 2, 1, 1}, 0, 0},
    {2, 0x8, 40, 100, 176, 20, 0x0, {1, 3, 2, 2}, 0, 0},
    {3, 0x8, 40, 128, 176, 20, 0x0, {2, 0, 3, 3}, 0, 0},
    {4, 0x208, 192, 26, 32, 16, 0x0, {-1, -1, -1, -1}, 0, 1},
    {5, 0x408, 224, 26, 32, 16, 0x0, {-1, -1, -1, -1}, 0, 1},
    {6, 0x2, 225, 170, 30, 20, 0x0, {-1, -1, -1, -1}, 0, 1},
};

// Included after the tables: where TextManager::Get() is first parsed decides the order of the file's data.
#include "Text/TextManager.hpp"

MyroomCharaScene::MyroomCharaScene() :
    SceneBase(2, 0) {
    mState      = 0;
    mTab        = 0;
    mShownTab     = 0;
    mResult     = 0;
    mPage       = 0;
    mListCount  = 0;
    mMessageWindow    = NULL;
    for (u8 i = 0; i < 2; i++) {
        mCellAnims[i]   = NULL;
        mCellsVisible[i]  = 0;
    }
    mCell = NULL;
    mCellVisible = 0;
    for (u8 i = 0; i < 7; i++) {
        mTextBuffers[i] = NULL;
        mLinesVisible[i]      = 0;
    }
    mStoryCharaNameMessages       = NULL;
    mMessages    = NULL;
    mGlyphBuffer = NULL;
    mTextBufferSize = 0;
    mGlyphCount  = 0;
}

MyroomCharaScene::~MyroomCharaScene() {
    Cleanup();
    MI_CpuClearFast((void *) 0x6000000, 0x80000);
    MI_CpuClearFast((void *) 0x6200000, 0x20000);
    SpriteManager::Get().ResetSlots();
}

void MyroomCharaScene::Cleanup() {
    delete mWindow;
    mWindow = NULL;
    CellAnimation *cell = mCell;
    if (cell != NULL) {
        delete cell;
    }
    mCell = NULL;
    FreeCells();
    FreeText();
    delete mMessageWindow;
    mMessageWindow = NULL;
}

void MyroomCharaScene::FreeCells() {
    for (u8 i = 0; i < 2; i++) {
        CellAnimation *cell = mCellAnims[i];
        if (cell != NULL) {
            delete cell;
        }
        mCellAnims[i] = NULL;
    }
}

void MyroomCharaScene::Update() {
    switch (mState) {
        case 0:
            BuildList();
            MI_CpuClearFast((void *) 0x6200000, 0x20000);
            InitDisplay();
            LoadSubGraphics();
            mStatus                  = new MyroomStatusScene(0x15);
            mContext->mRequest        = 0;
            mWindow->mCursor->mSelection = 0;
            mWindow->mCellAnims[0]->SetAnimation(2);
            mTab    = 0;
            mShownTab = 0;
            RefreshPageButtons();
            mWindow->mEnabled = 0;
            mState            = 1;
            return;
        case 1:
            SpriteManager::Get().StartFade(FADE_MODE_TO_ZERO);
            mState = 2;
            break;
        case 2:
            if (!SpriteManager::Get().IsFading()) {
                mWindow->mEnabled = 1;
                if (mWindow->mState == 1) {
                    mState = 3;
                }
            }
            break;
        case 3: {
            s8 id  = mWindow->GetSelectedId();
            u8 tab = id;
            if (tab < 4 && tab != mTab) {
                mTab = tab;
                SpriteManager::Get().sFade[0] = FADE_MODE_DOWN;
                mState = 4;
                break;
            }
            if (mResult == 3) {
                if (tab == 4) {
                    mPage = (mPage + mPageCount - 1) % mPageCount;
                    RenderLines();
                    RefreshPageButtons();
                } else if (tab == 5) {
                    mPage = (mPage + 1) % mPageCount;
                    RenderLines();
                    RefreshPageButtons();
                }
            } else if (mResult == 2) {
                SpriteManager::Get().StartFade(FADE_MODE_DOWN);
                mState = 0x11;
            }
            break;
        }
        case 4:
            mWindow->mEnabled = 0;
            if (SpriteManager::Get().sFade[0] <= FADE_MODE_HOLD) {
                mState = 5;
                if (mShownTab == 0) {
                    SetReady();
                }
            }
            break;
        case 5:
            if (mShownTab == 0) {
                if (mContext->mRequest != 0) {
                    mStatus = NULL;
                    InitDisplay();
                    LoadMainGraphics();
                    SetupTabCells();
                    mPage = 0;
                    RefreshPageButtons();
                    LoadText();
                    RenderLines();
                    SpriteManager::Get().sFade[0] = FADE_MODE_TO_ZERO;
                    mState                     = 7;
                    mShownTab                    = mTab;
                }
            } else {
                if (mTab == 0) {
                    FreeCells();
                    FreeText();
                    mStatus           = new MyroomStatusScene(0x15);
                    mContext->mRequest = 0;
                    RefreshPageButtons();
                } else {
                    SetupTabCells();
                    mPage = 0;
                    RefreshPageButtons();
                    RenderLines();
                }
                mState++;
                mShownTab = mTab;
            }
            break;
        case 6:
            SpriteManager::Get().sFade[0] = FADE_MODE_TO_ZERO;
            mState++;
            break;
        case 7:
            if (SpriteManager::Get().sFade[0] <= FADE_MODE_HOLD) {
                mState = 8;
            }
            break;
        case 8:
            switch (mTab) {
                case 0:
                    mState = 0xc;
                    break;
                case 1:
                    mState = 0xd;
                    break;
                case 3:
                    mState = 0xe;
                    break;
                case 2:
                    mState = 0xf;
                    break;
            }
            break;
        case 12:
        case 13:
        case 14:
        case 15:
        case 16:
            mWindow->mEnabled = 1;
            mState            = 3;
            break;
        case 17:
            if (mWindow->mState == 4) {
                if (!SpriteManager::Get().IsFading()) {
                    SetReady();
                    mState = 0x12;
                }
            }
            break;
        case 18:
            if (mContext->mRequest != 0) {
                RequestScene(1);
                mFinished = TRUE;
            }
            break;
    }
    mResult = mWindow->Update(TRUE);
    Draw();
}

void MyroomCharaScene::SetReady() {
    if (mStatus != NULL) {
        mStatus->SetReady();
    }
}

void MyroomCharaScene::InitDisplay() {
    MI_CpuClearFast((void *) 0x6000000, 0x80000);
    GX_SetDispSelect(1);
    SpriteManager::Get().SetupMode(0, 1);
    GX_SetGraphicsMode(1, 0, 0);
    G2_SetBG0Control(0, 1, 0, 1, 0);
    G2_SetBG1Control(0, 1, 2, 4, 0);
    G2_SetBG2Control(0, 1, 4, 8, 0);
    G2_SetBG3Control(0, 1, 6, 0xc, 0);
    G2S_SetBG0Control(0, 1, 0, 1, 0);
    G2S_SetBG1Control(0, 1, 2, 2, 0);
    G2S_SetBG2Control(0, 1, 4, 4, 0);
    G2S_SetBG3Control(0, 1, 6, 6, 0);
    G2_SetBG0Offset(0, 0);
    G2_SetBG1Offset(0, 0);
    G2_SetBG2Offset(0, 0);
    G2_SetBG3Offset(0, 0);
    G2S_SetBG0Offset(0, 0);
    G2S_SetBG1Offset(0, 0);
    G2S_SetBG2Offset(0, 0);
    G2S_SetBG3Offset(0, 0);
    G2_SetBG0Priority(3);
    G2_SetBG1Priority(3);
    G2_SetBG2Priority(3);
    G2_SetBG3Priority(3);
    G2S_SetBG0Priority(3);
    G2S_SetBG1Priority(3);
    G2S_SetBG2Priority(3);
    G2S_SetBG3Priority(3);
    GX_SetVisiblePlane(0x11);
    GXS_SetVisiblePlane(0x11);
    G2_SetBlendBrightness(0x1f, 0);
    G2S_SetBlendBrightness(0x1f, 0);
    GX_SetVisibleWnd(0);
    G2_SetWndOutsidePlane(0x1f, FALSE);
    G2_SetWnd0InsidePlane(0x1f, FALSE);
    G2_SetWnd1InsidePlane(0x1f, FALSE);
    G2_SetWndOBJInsidePlane(0x1f, FALSE);
    G2_SetWnd0Position(0, 0, 0, 0);
    G2_SetWnd1Position(0, 0, 0, 0);
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

void MyroomCharaScene::LoadSubGraphics() {
    ChangeDir("Game/Myroom/");
    LoadAndQueueUpload(1, UPLOAD_KIND_BG0_SCR, 0, "myroom_b_bg_00_nsc.bin", 0, 0, 0);
    LoadAndQueueUpload(1, UPLOAD_KIND_BG0_CHAR, 0, "myroom_b_bg_00_ncg.bin", 0, 0, 0);
    LoadAndQueueUpload(1, UPLOAD_KIND_BG_EXT_PLTT, 0, "myroom_b_bg_00_ncl.bin", 0, 0, 0);
    mWindow = NULL;
    mWindow = new SelectWindow(7, sMyroomCharaWindowItems, 0);
    SpriteScreen &sub = SpriteManager::Get().Screen(1);
    sub.SetObjVramMode(3);
    sub.LoadCharacters("myroom_status_b_obj_00.nce_ncg1D.bin", 0, 0);
    sub.LoadPalette("myroom_status_b_obj_00_ncl.bin", 0, 0);
    mWindow->SetCells(4, sMyroomCharaStatusCells);
    ChangeDir("/Game/2d/union_b/");
    sub.LoadCharacters("union_b_obj_page.nce_ncg1D.bin", 0x12000, 0);
    sub.LoadPalette("union_b_obj_page_ncl.bin", 0xa0, 0);
    mWindow->SetCells(2, sMyroomCharaPageCells);
    sub.LoadCharacters("union_b_obj_cockpit.nce_ncg1D.bin", 0x10000, 0);
    sub.LoadPalette("union_b_obj_cockpit_ncl.bin", 0x80, 0);
    mWindow->SetCells(1, sMyroomCharaCockpitCells);
    ChangeDir("/Game/Myroom/");
    mCell            = new CellAnimation("myroom_status_b_obj_00_nce.bin", 0, 1, 1);
    mCell->mCharBase = sub.GetCharBase(0);
    mCell->mPaletteBase   = 0;
    mCell->SetAnimation(0);
    mCellVisible = 1;
    ChangeDir("/");
    ChangeDir("/font/");
    mMessageWindow = new MessageWindow("mes_myroom", 1, 0x13000);
    mMessageWindow->func_02065bd4(0x14, 4, 0);
    mMessageWindow->SetWidth(0xd8);
    mMessageWindow->StartMessage(0x27, 0, 1);
    ChangeDir("/");
}

void MyroomCharaScene::LoadMainGraphics() {
    ChangeDir("/Game/Myroom/");
    LoadAndQueueUpload(0, UPLOAD_KIND_BG0_SCR, 0, "story_bg_01_nsc.bin", 0, 0, 0);
    LoadAndQueueUpload(0, UPLOAD_KIND_BG0_CHAR, 0, "story_bg_01_ncg.bin", 0, 0, 0);
    LoadAndQueueUpload(0, UPLOAD_KIND_BG_EXT_PLTT, 0, "story_bg_01_ncl.bin", 0, 0, 0);
    SpriteScreen &main = SpriteManager::Get().Screen(0);
    main.SetObjVramMode(3);
    main.ResetOam();
    main.LoadCharacters("myroom_status_u_obj_00.nce_ncg1D.bin", 0, 0);
    main.LoadPalette("myroom_status_u_obj_00_ncl.bin", 0, 0);
    main.LoadCharacters("myroom_status_u_obj_01.nce_ncg1D.bin", 0x9000, 0);
    main.LoadPalette("myroom_status_u_obj_01_ncl.bin", 0x80, 0);
    u8 i;
    for (i = 0; i == 0; i++) {
        mCellAnims[i]            = new CellAnimation("myroom_status_u_obj_00_nce.bin", 0, 1, 1);
        mCellAnims[i]->mCharBase = main.GetCharBase(0);
        mCellAnims[i]->mPaletteBase   = 0;
    }
    do {
        mCellAnims[i]            = new CellAnimation("myroom_status_u_obj_01_nce.bin", 0, 1, 1);
        mCellAnims[i]->mCharBase = main.GetCharBase(0x9000);
        mCellAnims[i]->mPaletteBase   = 4;
        i++;
    } while (i < 2);
    ChangeDir("/");
}

void MyroomCharaScene::SetupTabCells() {
    mCellsVisible[0] = 1;
    mCellsVisible[1] = 0;
    switch (mTab) {
        case 0:
            mCellsVisible[0] = 0;
            break;
        case 1:
            mCellAnims[0]->SetAnimation(0);
            break;
        case 2:
            mCellAnims[0]->SetAnimation(1);
            mCellAnims[1]->SetAnimation(func_ov008_02224a4c());
            mCellsVisible[1] = 1;
            break;
        case 3:
            mCellAnims[0]->SetAnimation(2);
            break;
    }
}

void MyroomCharaScene::Draw() {
    DrawWindow();
    DrawCells();
    DrawCell();
    mMessageWindow->Update();
    DrawText();
}

void MyroomCharaScene::DrawWindow() {
    mWindow->Draw();
}

void MyroomCharaScene::DrawCells() {
    for (u8 i = 0; i < 2; i++) {
        if (mCellAnims[i] != NULL && mCellsVisible[i] != 0) {
            mCellAnims[i]->Update();
            if (i == 1) {
                mCellAnims[i]->Draw(0, 0x80, 0x60, 0);
            } else {
                mCellAnims[i]->Draw(0, 0x80, 0x60, 1);
            }
        }
    }
}

void MyroomCharaScene::DrawCell() {
    if (mCellVisible != 0 && mCell != NULL) {
        mCell->Update();
        mCell->Draw(1, 0x28, (s16) (mTab * 0x1c + 0x2c), 0);
    }
}

u8 MyroomCharaScene::func_ov008_02224a4c() {
    return gSaveData[0].mUnk_5b8;
}

void MyroomCharaScene::LoadText() {
    ChangeDir("/font/");
    mStoryCharaNameMessages    = (MessageFile *) LoadFile("mes_StoryCharaName.mes", 0, 0, 0, 0, 0);
    mMessages = (MessageFile *) LoadFile("mes_myroom_status.mes", 0, 0, 0, 0, 0);
    mTextBufferSize = TextManager::Get().BufferSize(0x20, 2);
    for (u8 i = 0; i < 7; i++) {
        mTextBuffers[i] = OS_Alloc(mTextBufferSize);
        MI_CpuClear16(mTextBuffers[i], mTextBufferSize);
    }
    mGlyphCount  = TextManager::Get().GlyphCount(0x20, 2, 0x21);
    mGlyphBuffer = OS_Alloc(mGlyphCount * 0xc);
    TextManager::Get().Init(mGlyphBuffer, 0, 0, 0x20, 2, 0x21);
    TextManager::Get().SetColor(0, 0, 0xf);
    ClearTextBuffers();
}

void MyroomCharaScene::ClearTextBuffers() {
    for (u8 i = 0; i < 7; i++) {
        mLinesVisible[i] = 0;
        if (mTextBuffers[i] != NULL) {
            MI_CpuClear16(mTextBuffers[i], mTextBufferSize);
        }
    }
}

void MyroomCharaScene::PrintValue(u32 line, u32 value, u8 digits, s16 x, s16 suffix) {
    s16 pos = x;
    BOOL special = FALSE;
    s16 unused;
    s16 type = suffix;
    unused       = 0;
    switch (mTab) {
        case 1:
            if (gSaveData[0].mUnk_48 + gSaveData[0].mUnk_4a == 0) {
                special = TRUE;
            }
            break;
        case 2:
            if (value == 0) {
                special = TRUE;
            }
            break;
    }
    if (special) {
        pos += (digits - 1) * 6;
        TextManager &text = TextManager::Get();
        s16 width;
        s16 height;
        height            = 0;
        width             = pos;
        MessageFile *file = mMessages;
        text.Print(file->GetText(7), file->GetLength(7), mTextBuffers[line], 0x20, 2, &width, &height, 0, 0, 2, 0x21, 0, 0, 0, 0,
                   1, 1);
        pos += 6;
        if (type != 4 && type != 5) {
            pos += 6;
        }
    } else {
        TextManager::Get().PrintNumber(value, digits, 0, mTextBuffers[line], 0x20, 2, &pos, &unused, 0, 2, 0x21, 0, 0, 1, 1);
        if (type != 4 && type != 5) {
            pos += 6;
        }
    }
    if (type == -1) {
        return;
    }
    TextManager &text2 = TextManager::Get();
    s16 width2;
    s16 height2;
    height2            = 0;
    width2             = pos;
    MessageFile *file2 = mMessages;
    u16 index          = type;
    text2.Print(file2->GetText(index), file2->GetLength(index), mTextBuffers[line], 0x20, 2, &width2, &height2, 0, 0, 2, 0x21, 0,
                0, 0, 0, 1, 1);
}

void MyroomCharaScene::RenderLine(u32 line, u32 kind) {
    s32 value;
    s16 x     = 0;
    s32 ratio = 0;
    switch (kind) {
        case 0: {
            PrintValue(line, gSaveData[0].mUnk_48, 4, x, 2);
            x += 0x30;
            PrintValue(line, gSaveData[0].mUnk_4a, 4, x, 3);
            x += 0x2a;
            value = 0;
            if (gSaveData[0].mUnk_48 + gSaveData[0].mUnk_4a > 0) {
                value = gSaveData[0].mUnk_48 * 1000 / (gSaveData[0].mUnk_48 + gSaveData[0].mUnk_4a);
            }
            s32 rest = value % 10;
            if (rest == 0) {
                x += 0xc;
                PrintValue(line, value / 10, 3, x, 4);
            } else {
                TextManager::Get().mUnk_10 = rest;
                PrintValue(line, value / 10, 3, x, 5);
            }
            break;
        }
        case 1:
            PrintValue(line, gSaveData[0].mFewestRounds, 2, 0, -1);
            break;
        case 2:
            PrintValue(line, gSaveData[0].mMostRounds, 2, 0, -1);
            break;
        case 3: {
            if (gSaveData[0].mUnk_4e != 0) {
                ratio = gSaveData[0].mUnk_50 * 1000 / gSaveData[0].mUnk_4e;
            }
            s32 rest = ratio % 10;
            if (rest == 0) {
                PrintValue(line, ratio / 10, 3, 0xc, 4);
            } else {
                TextManager::Get().mUnk_10 = rest;
                PrintValue(line, ratio / 10, 3, 0, 5);
            }
            break;
        }
        case 4:
            PrintValue(line, data_020d48f0.mUnk_d4, 4, 0, -1);
            break;
        case 5:
            PrintValue(line, gSaveData[0].mUnk_5c, 4, 0, -1);
            break;
        case 6:
            PrintValue(line, gSaveData[0].mUnk_52, 4, 0, -1);
            break;
        case 7:
            PrintValue(line, gSaveData[0].mUnk_80, 3, 0, 6);
            break;
        case 8:
            PrintValue(line, gSaveData[0].mUnk_84, 3, 0, 6);
            break;
        case 9:
            PrintValue(line, gSaveData[0].mUnk_82, 3, 0, 6);
            break;
        case 10: {
            TextManager::Get().mUse4bppFont = 1;
            TextManager &text          = TextManager::Get();
            s16 width;
            s16 height;
            height    = 0;
            width     = 0;
            u8 index  = mList[mPage * 7 + line];
            MessageFile *file = mStoryCharaNameMessages;
            text.Print(file->GetText(index), file->GetLength(index), mTextBuffers[line], 0x20, 2, &width, &height, 0, 0, 2, 0x21, 0, 0,
                       0, 0, 1, 1);
            TextManager::Get().mUse4bppFont = 0;
            PrintValue(line, data_020d444e[mList[mPage * 7 + line]], 3,
                                (s16) (sMyroomCharaTextPos[3][line].mRight - sMyroomCharaTextPos[3][line].mX), 6);
            break;
        }
    }
    QueueUpload(SpriteManager::Get().Screen(0).mBgIndex, UPLOAD_KIND_OBJ, sMyroomCharaLineVramOffsets[line], mTextBuffers[line], mTextBufferSize, 0);
    mLinesVisible[line] = 1;
}

void MyroomCharaScene::DrawText() {
    for (u8 i = 0; i < 7; i++) {
        if (mLinesVisible[i] != 0) {
            SpriteManager::Get().Screen(0).AddOam(mGlyphBuffer, mGlyphCount, sMyroomCharaTextPos[mShownTab][i].mX,
                                                         sMyroomCharaTextPos[mShownTab][i].mY, 0,
                                                         (u16) (sMyroomCharaLineVramOffsets[i] >> 7), 0xf, 0);
        }
    }
}

void MyroomCharaScene::FreeText() {
    u8 i = 0;
    ClearTextBuffers();
    for (; i < 7; i++) {
        if (mTextBuffers[i] != NULL) {
            OS_Free(mTextBuffers[i]);
            mTextBuffers[i] = NULL;
        }
    }
    if (mStoryCharaNameMessages != NULL) {
        OS_Free(mStoryCharaNameMessages);
        mStoryCharaNameMessages = NULL;
    }
    if (mMessages != NULL) {
        OS_Free(mMessages);
        mMessages = NULL;
    }
    if (mGlyphBuffer != NULL) {
        OS_Free(mGlyphBuffer);
        mGlyphBuffer = NULL;
    }
}

void MyroomCharaScene::RenderLines() {
    ClearTextBuffers();
    switch (mTab) {
        case 0:
            break;
        case 1:
            RenderLine(0, 0);
            RenderLine(1, 1);
            RenderLine(2, 2);
            RenderLine(3, 3);
            RenderLine(4, 4);
            RenderLine(5, 5);
            RenderLine(6, 6);
            break;
        case 2:
            RenderLine(0, 7);
            RenderLine(1, 8);
            RenderLine(2, 9);
            break;
        case 3: {
            if (mListCount == 0) {
                return;
            }
            u8 i = 0;
            do {
                RenderLine(i, 10);
                i++;
            } while (i < 7 && i + mPage * 7 < mListCount);
            break;
        }
    }
}

void MyroomCharaScene::BuildList() {
    s16 values[17];
    u8 ids[17];
    u8 best;
    u8 k;
    u8 i;
    u8 j;
    s16 max;
    mListCount = 0;
    for (i = 0; i < 0x11; i++) {
        if (i != 5 && gSaveData[0].mUnk_5e[i] != 0) {
            ids[mListCount]    = i;
            values[mListCount] = gSaveData[0].mUnk_5e[i];
            mListCount++;
        }
    }
    j    = 0;
    best = 0;
    do {
        k   = 0;
        max = -1;
        do {
            if (values[k] > max) {
                max  = values[k];
                best = k;
            }
            k++;
        } while (k < mListCount);
        mList[j]     = ids[best];
        values[best] = -1;
        j++;
    } while (j < mListCount);
    mPageCount = (mListCount - 1) / 7 + 1;
}

void MyroomCharaScene::RefreshPageButtons() {
    WindowItem *items = mWindow->mCursor->mItems;
    if (mTab == 3) {
        if (mPageCount == 1) {
            mWindow->mCellAnims[4]->SetAnimation(2);
            mWindow->mCellAnims[5]->SetAnimation(5);
            mWindow->mButtons[4].mEnabled = 0;
            mWindow->mButtons[5].mEnabled = 0;
            items[4].mFlags               = 0x8000;
            items[5].mFlags               = 0x8000;
            return;
        }
        if (mPage != 0) {
            mWindow->mCellAnims[4]->SetAnimation(0);
            mWindow->mButtons[4].mEnabled = 1;
            items[4].mFlags               = 0x208;
        } else {
            mWindow->mCellAnims[4]->SetAnimation(2);
            mWindow->mButtons[4].mEnabled = 0;
            items[4].mFlags               = 0x8000;
        }
        if (mPage < mPageCount - 1) {
            mWindow->mCellAnims[5]->SetAnimation(3);
            mWindow->mButtons[5].mEnabled = 1;
            items[5].mFlags               = 0x408;
        } else {
            mWindow->mCellAnims[5]->SetAnimation(5);
            mWindow->mButtons[5].mEnabled = 0;
            items[5].mFlags               = 0x8000;
        }
    } else {
        mWindow->mCellAnims[4]->SetAnimation(2);
        mWindow->mCellAnims[5]->SetAnimation(5);
        mWindow->mButtons[4].mEnabled = 0;
        mWindow->mButtons[5].mEnabled = 0;
        items[4].mFlags               = 0x8000;
        items[5].mFlags               = 0x8000;
    }
}
