#include "System/InputManager.hpp"
#include "Scene/MyroomTipsScene.hpp"
#include "Game/ScriptPlayer.hpp"
#include "Game/SaveData.hpp"
extern u8 gUnlockedTipBits[]; // inside gSaveData at +0x5ac (SaveData::mUnk_5ac), set by UnlockTips
extern u8 data_020d49a1[];
#include "Graphics/CellAnimation.hpp"
#include "Graphics/Display.hpp"
#include "Graphics/SpriteManager.hpp"
#include "System/FileSystem.hpp"
#include "System/InputManager.hpp"
#include "System/SoundManager.hpp"
#include "System/TouchManager.hpp"
#include "Text/TextManager.hpp"
#include "UI/MessageWindow.hpp"
#include "UI/SelectWindow.hpp"

#include <nitro/gx.h>
#include <nitro/mi.h>
#include <nitro/os.h>
#include <stdio.h>

// The scene context's SceneInfo bytes as this scene uses them (SceneInfo's own names follow StoreScene).
struct MyroomTipsSceneInfo {
    union {
        /* 00 */ u16 mUnk_00;
        /* 00 */ u8 mState; // the state Update switches on
    };
    // Cleared with bytes 0-4 by the ctor (MI_CpuFill8(&mContext->mInfo, 0, 5)) and never written by this scene, so the
    // mUnk_02 == 0 test in func_ov008_022274f8 is always true in this build.
    /* 02 */ u8 mUnk_02;
    /* 03 */ u8 mCategory;
    /* 04 */ s8 mMenuResult;
    /* 05 */ s8 mSelection;
    /* 06 */ u8 mUnk_06;
    /* 07 */ u8 mScrollRemaining;
    /* 08 */ u8 mScrollDir;
    /* 09 */ u8 mPage;
    /* 0a */ u8 mUnk_0a;
    /* 0b */ u8 mUnk_0b;
    /* 0c */
};

#define TIPS_INFO(ctx) (*(MyroomTipsSceneInfo *) &(ctx)->mInfo)

#define TEST_BIT(flags, index) ((flags)[(index) / 8] & (1 << ((index) % 8)))
#define SET_BIT(flags, index) ((flags)[(index) / 8] |= 1 << ((index) % 8))

// One row per tips category: each entry holds the text id shown in the list and the event message it opens.
struct TipsEntry {
    s8 mText;
    s8 mMessage;
};

extern "C" void InvalidateStorytalkMessageIndex(ScriptPlayer *event);
extern "C" void func_0206f88c(ScriptPlayer *event, u32 category, s32 value, u32 unk4);
extern "C" void StartMessageScript(ScriptPlayer *event, const char *name, u32 id, u32 unk3, u32 unk4);

const s8 data_ov008_02229658[6] = {0, 11, 17, 24, 31, 44};

const u8 sMyroomTipsCategoryPos[6][2] = {{0x14, 0x22}, {0x64, 0x22}, {0xb4, 0x22}, {0x14, 0x68}, {0x64, 0x68}, {0xb4, 0x68}};

const WindowCellRecord sMyroomTipsWindowBCells[5] = {
    {"/Game/2D/union_b/union_b_obj_cockpit_nce.bin", 2, 0, 4, 3, 1, {2, 3, 3, 0}},
    {"/Game/2D/union_b/union_b_obj_scroll_nce.bin", 4, 28672, 4, 3, 1, {4, 4, 4, 0}},
    {"/Game/2D/union_b/union_b_obj_scroll_nce.bin", 0, 28672, 4, 3, 1, {0, 1, 1, 0}},
    {"/Game/2D/union_b/union_b_obj_scroll_nce.bin", 2, 28672, 4, 3, 1, {2, 3, 3, 0}},
    {"/Game/2D/union_b/union_b_obj_scroll_nce.bin", 5, 28672, 4, 3, 1, {5, 5, 5, 0}},
};

const s8 sMyroomTipsCategoryCounts[6] = {10, 5, 6, 6, 6, 6};

const TipsEntry sMyroomTipsEntries[6][10] = {
    {{0, 1}, {1, 2}, {2, 3}, {3, 4}, {9, 10}, {4, 5}, {5, 6}, {6, 7}, {7, 8}, {8, 9}},
    {{10, 12}, {11, 13}, {12, 14}, {13, 15}, {14, 16}, {-1, -1}, {0, 0}, {0, 0}, {0, 0}, {0, 0}},
    {{15, 18}, {16, 19}, {17, 20}, {18, 21}, {19, 22}, {20, 23}, {-1, -1}, {0, 0}, {0, 0}, {0, 0}},
    {{21, 25}, {22, 26}, {23, 27}, {24, 28}, {25, 29}, {26, 30}, {-1, -1}, {0, 0}, {0, 0}, {0, 0}},
    {{27, 32}, {28, 33}, {29, 34}, {32, 37}, {30, 35}, {31, 36}, {-1, -1}, {0, 0}, {0, 0}, {0, 0}},
    {{33, 45}, {34, 46}, {35, 47}, {36, 48}, {37, 49}, {38, 50}, {-1, -1}, {0, 0}, {0, 0}, {0, 0}},
};

const WindowItemRecord sMyroomTipsWindowBItems[5] = {
    {0, 0x2, 225, 170, 30, 20, 0x0, {-1, -1, -1, -1}, 0, WINDOW_TOUCH_RELEASE},
    {1, WINDOW_ITEM_SCROLL_DRAG, 228, 48, 32, 12, 0x0, {-1, -1, -1, -1}, 0, 0},
    {2, WINDOW_ITEM_SCROLL_PREV, 228, 28, 32, 20, 0x0, {-1, -1, -1, -1}, 0, 3},
    {3, WINDOW_ITEM_SCROLL_NEXT, 228, 144, 32, 20, 0x0, {-1, -1, -1, -1}, 0, 3},
    {4, WINDOW_ITEM_CURSOR_SKIP, 238, 36, 0, 0, 0x0, {-1, -1, -1, -1}, 0, 0},
};

const WindowItemRecord sMyroomTipsWindowAItems[1] = {
    {0, 0x2, 225, 170, 30, 20, 0x0, {-1, -1, -1, -1}, 0, WINDOW_TOUCH_RELEASE},
};

const u8 sMyroomTipsCategoryFirstBits[6] = {0x00, 0x10, 0x15, 0x1b, 0x0a, 0x21};

const WindowCellRecord sMyroomTipsWindowACells[1] = {
    {"/Game/2D/union_b/union_b_obj_cockpit_nce.bin", 2, 0, 4, 3, 1, {2, 3, 3, 0}},
};

MyroomTipsScene::MyroomTipsScene() :
    SceneBase(2, 0) {
    mRowGlyphCount = 0;
    mFlags  = 0;
    mScroll = 0;
    mRow    = 0;
    mCount  = 5;
    mUnk_85 = 0;
    MI_CpuFill8(&mContext->mInfo, 0, 5);
    MI_CpuClear16(mTextWidth, 8);
    MI_CpuClear16(mTextX, 8);
    MI_CpuFill8(mTimers, 0x3c, 4);
    mWindowA = NULL;
    mWindowB = NULL;
    mEvent   = NULL;
    mCellAnimB   = NULL;
    mMessageWindow = NULL;
    mCellAnimA   = NULL;
    u8 i;
    for (i = 0; i < 6; i++) {
        mCellAnims[i] = NULL;
    }
    for (i = 0; i < 2; i++) {
        mRowCells[i] = NULL;
    }
    for (i = 0; i < 2; i++) {
        mNewCells[i] = NULL;
    }
    mMessages    = NULL;
    mGlyphBuffer = NULL;
    for (i = 0; i < 4; i++) {
        mTextBuffers[i] = NULL;
    }
    for (i = 0; i < 4; i++) {
        mIndices[i] = 0;
    }
    CountTips();
}

MyroomTipsScene::~MyroomTipsScene() {
    delete mWindowA;
    delete mWindowB;
    CellAnimation *cellB = mCellAnimB;
    if (cellB != NULL) {
        delete cellB;
    }
    delete mMessageWindow;
    CellAnimation *cellA = mCellAnimA;
    if (cellA != NULL) {
        delete cellA;
    }
    for (u8 i = 0; i < 6; i++) {
        CellAnimation *cell = mCellAnims[i];
        if (cell != NULL) {
            delete cell;
        }
    }
    {
        CellAnimation *cell;
        for (u8 j = 0; j < 2; j++) {
            cell = mRowCells[j];
            if (cell != NULL) {
                delete cell;
            }
        }
    }
    {
        CellAnimation *cell;
        for (u8 j = 0; j < 2; j++) {
            cell = mNewCells[j];
            if (cell != NULL) {
                delete cell;
            }
        }
    }
    if (mMessages != NULL) {
        OS_Free(mMessages);
    }
    if (mGlyphBuffer != NULL) {
        OS_Free(mGlyphBuffer);
    }
    for (u8 i = 0; i < 4; i++) {
        if (mTextBuffers[i] != NULL) {
            OS_Free(mTextBuffers[i]);
        }
    }
}

void MyroomTipsScene::Update() {
    SceneContext *context = mContext;
    u8 state                        = TIPS_INFO(context).mState;
    switch (state) {
        case 0:
            InitDisplay();
            LoadGraphics();
            CreateMessage();
            mEvent = new ScriptPlayer(SCRIPT_KIND_MYROOM_TIPS, 0, 1);
            GXS_SetVisiblePlane(0x13);
            SpriteManager::Get().StartFade(FADE_MODE_TO_ZERO);
            TIPS_INFO(context).mState++;
            return;
        case 1:
            if (!SpriteManager::Get().IsFading()) {
                TIPS_INFO(context).mState = 4;
            }
            Draw();
            return;
        case 2:
            if (SpriteManager::Get().sFade[0] <= FADE_MODE_HOLD) {
                mEvent->RequestFinish();
                TIPS_INFO(context).mState++;
            }
            Draw();
            return;
        case 3:
            mEvent = new ScriptPlayer(SCRIPT_KIND_MYROOM_TIPS, 0, 1);
            SetMessage(0x2d);
            SpriteManager::Get().sFade[0] = FADE_MODE_TO_ZERO;
            TIPS_INFO(context).mState++;
            Draw();
            return;
        case 4:
            if (SpriteManager::Get().sFade[0] <= FADE_MODE_HOLD) {
                if (!mEvent->IsRunning()) {
                    u32 id = 0x33;
                    if (mUnlocked[0] == 0) {
                        id = 0x34;
                    }
                    StartMessageScript(mEvent, "mes_tips_Dan", id, 10, 0x15);
                    SpriteManager::Get().sFade[0] = FADE_MODE_TO_ZERO;
                    TIPS_INFO(context).mState             = 5;
                }
            }
            Draw();
            return;
        case 5:
            if (SpriteManager::Get().sFade[0] <= FADE_MODE_HOLD) {
                mWindowA->mState = 4;
                mWindowA->Reset(-1);
                mFlags = MYROOM_TIPS_WINDOW_A;
                TIPS_INFO(context).mState++;
            }
            Draw();
            return;
        case 6:
            GXS_SetVisiblePlane(0x13);
            Draw();
            TIPS_INFO(context).mState++;
            return;
        case 7:
            func_ov008_022274f8();
            Draw();
            return;
        case 8:
            if (!mEvent->IsRunning()) {
                mWindowB->mState = 4;
                mWindowB->Reset(-1);
                mWindowB->mCellAnims[0]->SetAnimation(2);
                MI_CpuClear16(mTextWidth, 8);
                MI_CpuClear16(mTextX, 8);
                MI_CpuFill8(mTimers, 0x3c, 4);
                SetMessage(0x2f);
                mFlags = MYROOM_TIPS_WINDOW_B;
                TIPS_INFO(context).mState++;
            }
            Draw();
            return;
        case 9: {
            u8 i = 0;
            mRow    = 0;
            mScroll = 0;
            do {
                mIndices[i] = GetUnlockedTip((u8) (mScroll + i));
                i++;
            } while (i < 4);
            PrintRows(-1);
            GXS_SetVisiblePlane(0x11);
            mWindowB->mCursor->SetScroll(mScroll);
            SelectWindow *window  = mWindowB;
            u8 y                  = window->mCursor->mScrollItemY;
            window->mCellPos[1].x = 0xe4;
            window->mCellPos[1].y = y;
            mUnk_85               = 0;
            TIPS_INFO(context).mState++;
            Draw();
            return;
        }
        case 10:
            UpdateTextX();
            func_ov008_022274f8();
            Draw();
            return;
        case 11:
            if (mEvent->mUnk_14f1 == 0) {
                TIPS_INFO(context).mState = state + 1;
                Draw();
            }
        case 12:
            if (!mEvent->IsRunning()) {
                TIPS_INFO(context).mState = 10;
                MI_CpuClear16(mTextWidth, 8);
                MI_CpuClear16(mTextX, 8);
                MI_CpuFill8(mTimers, 0x3c, 4);
                PrintRows(-1);
            }
            func_ov008_022274f8();
            Draw();
            return;
        case 13:
            TIPS_INFO(context).mState = state + 1;
            SpriteManager::Get().StartFade(FADE_MODE_DOWN);
            GXS_SetVisiblePlane(0x11);
            return;
        case 14:
            if (!SpriteManager::Get().IsFading()) {
                mEvent->RequestFinish();
                mContext->mRequest = 0;
                TIPS_INFO(context).mState++;
            }
            return;
        case 15:
            if (context->mRequest != 0) {
                TIPS_INFO(context).mState = state + 1;
            }
            return;
        case 16:
            RequestScene(1);
            mFinished = TRUE;
            return;
    }
}

void MyroomTipsScene::InitDisplay() {
    GX_SetDispSelect(1);
    SpriteManager::Get().SetupMode(1, 7);
    GX_SetGraphicsMode(1, 0, 1);
    GXS_SetGraphicsMode(0);
    G2S_SetBG0Control(0, 1, 0, 1, 0);
    G2S_SetBG1Control(0, 1, 2, 4, 0);
    G2S_SetBG0Offset(0, 0);
    G2S_SetBG1Offset(0, 0);
    G2S_SetBG0Priority(3);
    G2S_SetBG1Priority(2);
    GXS_SetVisiblePlane(0x11);
    G2S_SetBlendBrightness(0x1f, 0);
    GXS_SetVisibleWnd(0);
    G2S_SetWndOutsidePlane(0x1f, FALSE);
    G2S_BlendNone();
}

void MyroomTipsScene::LoadGraphics() {
    ChangeDir("/Game/Myroom/");
    LoadAndQueueUpload(1, UPLOAD_KIND_BG_EXT_PLTT, 0, "myroom_b_bg_00_ncl.bin", 0, 0, 0);
    LoadAndQueueUpload(1, UPLOAD_KIND_BG0_SCR, 0, "myroom_b_bg_00_nsc.bin", 0, 0, 0);
    LoadAndQueueUpload(1, UPLOAD_KIND_BG0_CHAR, 0, "myroom_b_bg_00_ncg.bin", 0, 0, 0);
    LoadAndQueueUpload(1, UPLOAD_KIND_BG_EXT_PLTT, 0x2000, "myroom_tips_b_bg_00_ncl.bin", 0, 0, 0);
    LoadAndQueueUpload(1, UPLOAD_KIND_BG1_SCR, 0, "myroom_tips_b_bg_00_nsc.bin", 0, 0, 0);
    LoadAndQueueUpload(1, UPLOAD_KIND_BG1_CHAR, 0, "myroom_tips_b_bg_00_ncg.bin", 0, 0, 0);
    SpriteScreen &sub = SpriteManager::Get().Screen(1);
    sub.SetObjVramMode(3);
    ChangeDir("/Game/Myroom/");
    sub.LoadCharacters("myroom_tips_obj_00.nce_ncg1D.bin", 0x2000, 0);
    sub.LoadPalette("myroom_tips_obj_00_ncl.bin", 0, 0);
    mCellAnimA            = new CellAnimation("myroom_tips_obj_00_nce.bin", 0, 1, 1);
    mCellAnimA->mCharBase = sub.GetCharBase(0x2000);
    mCellAnimA->mPaletteBase   = 0;
    for (u8 i = 0; i < 6; i++) {
        mCellAnims[i]            = new CellAnimation("myroom_tips_obj_00_nce.bin", 0, 1, 1);
        mCellAnims[i]->mCharBase = sub.GetCharBase(0x2000);
        mCellAnims[i]->mPaletteBase   = 0;
        mCellAnims[i]->SetAnimation(i + 2);
    }
    mCellAnimB            = new CellAnimation("myroom_tips_obj_00_nce.bin", 0, 1, 1);
    mCellAnimB->mCharBase = sub.GetCharBase(0x2000);
    mCellAnimB->mPaletteBase   = 0;
    mCellAnimB->SetAnimation(1);
    mNewCells[0]            = new CellAnimation("myroom_tips_obj_00_nce.bin", 8, 1, 1);
    mNewCells[0]->mCharBase = sub.GetCharBase(0x2000);
    mNewCells[0]->mPaletteBase   = 0;
    ChangeDir("/Game/Tutorial/obj/");
    sub.LoadCharacters("tutorial_b_obj_00.nce_ncg1D.bin", 0x5000, 0);
    sub.LoadPalette("tutorial_b_obj_00_ncl.bin", 0xa0, 0);
    for (u8 i = 0; i < 2; i++) {
        mRowCells[i]            = new CellAnimation("tutorial_b_obj_00_nce.bin", 0, 1, 1);
        mRowCells[i]->mCharBase = sub.GetCharBase(0x5000);
        mRowCells[i]->mPaletteBase   = 5;
    }
    mRowCells[0]->SetAnimation(1);
    mRowCells[1]->SetAnimation(2);
    ChangeDir("/Game/2D/union_b/");
    sub.LoadCharacters("union_b_obj_scroll.nce_ncg1D.bin", 0x7000, 0);
    sub.LoadPalette("union_b_obj_scroll_ncl.bin", 0x80, 0);
    sub.LoadCharacters("union_b_obj_cockpit.nce_ncg1D.bin", 0, 0);
    sub.LoadCharacters("union_b_obj_new.nce_ncg1D.bin", 0xc000, 0);
    sub.LoadPalette("union_b_obj_new_ncl.bin", 0x100, 0);
    mNewCells[1]            = new CellAnimation("union_b_obj_new_nce.bin", 0, 1, 1);
    mNewCells[1]->mCharBase = sub.GetCharBase(0xc000);
    mNewCells[1]->mPaletteBase   = 8;
    ChangeDir("/");
    mWindowA = new SelectWindow(1, sMyroomTipsWindowAItems, 0);
    mWindowA->SetCells(1, sMyroomTipsWindowACells);
    mWindowA->mCursor->mSelection = -1;
    mWindowA->mEnabled            = 0;
    mWindowB                      = new SelectWindow(5, sMyroomTipsWindowBItems, -1);
    mWindowB->SetCells(5, sMyroomTipsWindowBCells);
    mWindowB->mCursor->mSelection = -1;
    mWindowB->mCursor->mSelection = mScroll;
    mWindowB->mCursor->SetupScroll(0x30, 0x90, 4, mCount);
    mWindowB->mEnabled = 0;
    mRow               = 0;
    LoadText();
    mFlags = MYROOM_TIPS_WINDOW_A;
}

void MyroomTipsScene::LoadText() {
    ChangeDir("/font/");
    mMessages = (MessageFile *) LoadFile("mes_TIPS_sys.mes", 0, 0, 0, 0, 0);
    mTextBufferSize = TextManager::Get().BufferSize(0x30, 2);
    mGlyphCount     = TextManager::Get().GlyphCount(0x30, 2, 0x11);
    mRowGlyphCount         = mGlyphCount / 2;
    for (u8 i = 0; i < 4; i++) {
        mTextBuffers[i] = OS_Alloc(mTextBufferSize);
        MI_CpuClear16(mTextBuffers[i], mTextBufferSize);
    }
    mGlyphBuffer = OS_Alloc(mGlyphCount * 0xc);
    TextManager::Get().Init(mGlyphBuffer, 1, 0, 0x30, 2, 0x11);
    TextManager::Get().SetColor(1, 0, 0xf);
    PrintRows(-1);
}

void MyroomTipsScene::Draw() {
    SceneContext *context = mContext;
    func_ov008_022271f8();
    DrawCategoryCursorAndRowCells();
    DrawMessage();
    if (mFlags & MYROOM_TIPS_WINDOW_A) {
        mNewCells[0]->Update();
        BOOL show = TRUE;
        if (!gSaveData[0].mUnk_5bf_0 && gSaveData[0].mUnk_42 == 6) {
            show = FALSE;
        }
        BOOL first = (show == FALSE);
        u8 i       = first;
        do {
            if (mNew[i] == 0) {
                i++;
                continue;
            }
            mNewCells[0]->Draw(1, sMyroomTipsCategoryPos[i][0], sMyroomTipsCategoryPos[i][1], 1);
            i++;
        } while (i < 6);
    }
    if (!(mFlags & MYROOM_TIPS_WINDOW_B)) {
        return;
    }
    mNewCells[1]->Update();
    u8 j = 0;
    do {
        SpriteManager::Get().Screen(1).AddOam(mGlyphBuffer, mRowGlyphCount, 0x26, (s16) (j * 32 + 0x29), 1,
                                                     (u16) ((j * 3 * 0x400 + 0x8000) >> 7), 0xf, 0);
        if (!TEST_BIT(data_020d49a1, sMyroomTipsCategoryFirstBits[(s8) TIPS_INFO(context).mCategory] + mIndices[j])) {
            mNewCells[1]->Draw(1, 0x20, (s16) (j * 32 + 0x20), 1);
        }
        j++;
    } while (j < 4);
}

void MyroomTipsScene::func_ov008_022271f8() {
    SceneContext *context = mContext;
    if (mFlags & MYROOM_TIPS_WINDOW_A) {
        BOOL drawn = FALSE;
        mCellAnimA->Update();
        if (!gSaveData[0].mUnk_5bf_0 && gSaveData[0].mUnk_42 == 6 && mUnlocked[0] != 0) {
            mCellAnimA->Draw(1, 0x14, 0x22, 2);
            drawn = TRUE;
        }
        u8 i = 0;
        if (drawn) {
            i = 1;
        }
        do {
            if (mUnlocked[i] == 0) {
                mCellAnimA->Draw(1, sMyroomTipsCategoryPos[i][0], sMyroomTipsCategoryPos[i][1], 2);
            } else {
                mCellAnims[i]->Update();
                mCellAnims[i]->Draw(1, sMyroomTipsCategoryPos[i][0], sMyroomTipsCategoryPos[i][1], 2);
            }
            i++;
        } while (i < 6);
        if (mWindowA != NULL) {
            if (!(InputManager::Get().IsDown(1))) {
                TIPS_INFO(context).mMenuResult = mWindowA->Update(TRUE);
            }
            mWindowA->Draw();
        }
    }
    if (!(mFlags & MYROOM_TIPS_WINDOW_B)) {
        return;
    }
    if (mUnk_85 == 0) {
        if (!(InputManager::Get().IsDown(1))) {
            TIPS_INFO(context).mMenuResult = mWindowB->Update(TRUE);
        }
    }
    mWindowB->Draw();
}

void MyroomTipsScene::DrawCategoryCursorAndRowCells() {
    SceneContext *context = mContext;
    if (mFlags & MYROOM_TIPS_WINDOW_A) {
        DrawCategoryCursor(sMyroomTipsCategoryPos[0], TIPS_INFO(context).mCategory);
    }
    if (!(mFlags & MYROOM_TIPS_WINDOW_B)) {
        return;
    }
    mRowCells[0]->Update();
    mRowCells[1]->Update();
    for (u8 i = 0; i < 4; i++) {
        u8 cell = 0;
        if (i == mRow) {
            cell = 1;
        }
        mRowCells[cell]->Draw(1, 0x20, (s16) (i * 32 + 0x20), 2);
    }
}

void MyroomTipsScene::DrawCategoryCursor(const u8 *pos, u32 index) {
    if (mCellAnimB == NULL) {
        return;
    }
    mCellAnimB->Update();
    mCellAnimB->Draw(1, pos[index * 2], pos[index * 2 + 1], 2);
}

void MyroomTipsScene::func_ov008_022274f8() {
    if (mEvent->IsRunning()) {
        mWindowA->mEnabled = 0;
        mWindowB->mEnabled = 0;
        return;
    }
    SceneContext *context = mContext;
    if (mFlags & MYROOM_TIPS_WINDOW_A) {
        mWindowA->mEnabled = 1;
        mWindowB->mEnabled = 0;
        if (mWindowA != NULL) {
            switch (*(u8 *) &TIPS_INFO(context).mMenuResult) {
                case 2:
                    TIPS_INFO(context).mState = 0xd;
                    break;
                case 3:
                    break;
            }
        }
        if (TIPS_INFO(context).mUnk_02 == 0) { // always true in this build (see MyroomTipsSceneInfo::mUnk_02)
            s32 selection = UpdateCategorySelection(sMyroomTipsCategoryPos[0], 6);
            if (selection >= 0) {
                s8 category = TIPS_INFO(context).mCategory;
                if (category != selection) {
                    TIPS_INFO(context).mCategory = selection;
                    SoundManager::Get().StartSeqArc(5);
                } else if (mUnlocked[category] != 0) {
                    BOOL available = TRUE;
                    if (category == 0) {
                        if (!gSaveData[0].mUnk_5bf_0 && gSaveData[0].mUnk_42 == 6) {
                            available = FALSE;
                        }
                    }
                    if (available) {
                        SoundManager::Get().StartSeqArc(0);
                        s8 current = TIPS_INFO(context).mCategory;
                        func_0206f88c(mEvent, (u8) current, data_ov008_02229658[current], 10);
                        mCount = mUnlocked[(s8) TIPS_INFO(context).mCategory];
                        mWindowB->mCursor->SetupScroll(0x30, 0x90, 4, mCount);
                        if (mCount <= 4) {
                            mWindowB->mButtons[1].mEnabled = 0;
                        } else {
                            mWindowB->mButtons[1].mEnabled = 1;
                        }
                        TIPS_INFO(context).mState = 8;
                    }
                }
            }
        }
    }
    u32 second = mFlags & MYROOM_TIPS_WINDOW_B;
    if (second == 0) {
        return;
    }
    if (mUnk_85 == 0) {
        mWindowA->mEnabled = 0;
        mWindowB->mEnabled = 1;
        BOOL changed       = FALSE;
        switch (*(u8 *) &TIPS_INFO(context).mMenuResult) {
            case 1:
                mWindowB->mState = 4;
                changed            = TRUE;
                mWindowB->Reset(0);
                break;
            case 2:
                SpriteManager::Get().sFade[0] = FADE_MODE_DOWN;
                TIPS_INFO(context).mState             = 2;
                break;
            case 0:
                if (UpdateRowSelection() == 1) {
                    changed = TRUE;
                }
                break;
        }
        if (!changed) {
            return;
        }
        SoundManager::Get().StartSeqArc(0);
        mRowCells[1]->SetAnimation(3);
        mWindowB->mCursor->SetScroll(mScroll);
        SelectWindow *window = mWindowB;
        u8 y                 = window->mCursor->mScrollItemY;
        window->mCellPos[1].x = 0xe4;
        window->mCellPos[1].y = y;
        mUnk_85                 = 1;
    } else {
        if (second == 0) {
            return;
        }
        CellAnimation *cell = mRowCells[1];
        if (cell->mAnimIndex != 3) {
            return;
        }
        if (cell->mFinished == 0) {
            return;
        }
        cell->SetAnimation(2);
        mUnk_85 = 0;
        s32 category = (s8) TIPS_INFO(context).mCategory;
        u8 entry     = mIndices[mRow];
        u16 message  = sMyroomTipsEntries[category][entry].mMessage;
        if (category == 4 && sMyroomTipsEntries[category][entry].mText == 0x20) {
            if (gSaveData[0].mUnk_5bf_0) {
                message = 0x2b;
            } else {
                u8 level = gSaveData[0].mUnk_42;
                if (level <= 1) {
                    message = 0x25;
                } else if (level <= 3) {
                    message = 0x26;
                } else if (level <= 6) {
                    message = 0x27;
                } else if (level <= 7) {
                    message = 0x28;
                } else if (level <= 8) {
                    message = 0x29;
                } else if (level <= 9) {
                    message = 0x2a;
                }
            }
        }
        InvalidateStorytalkMessageIndex(mEvent);
        mEvent->func_0206f564(message, 11, -1);
        TIPS_INFO(context).mState = 0xb;
        int flag             = sMyroomTipsCategoryFirstBits[(s8) TIPS_INFO(context).mCategory] + mIndices[mRow];
        if (!TEST_BIT(data_020d49a1, flag)) {
            SET_BIT(data_020d49a1, flag);
            mNew[(s8) TIPS_INFO(context).mCategory]--;
        }
    }
}

s8 MyroomTipsScene::UpdateCategorySelection(const u8 *pos, u8 count) {
    SceneContext *context = mContext;
    u8 saved                        = TIPS_INFO(context).mCategory;
    if (TouchManager::Get().mPressed != 0) {
        u8 i = 0;
        do {
            if (TouchManager::Get().IsPressedIn((s16) (pos[i * 2] - 9), (s16) (pos[i * 2 + 1] - 10), 0x4b, 0x42)) {
                return i;
            }
            i++;
        } while (i < count);
    }
    if (InputManager::Get().IsTrigger(KEY_LEFT)) {
        s8 current = TIPS_INFO(context).mCategory;
        if (current < 3) {
            TIPS_INFO(context).mCategory = current + (count - 1);
            TIPS_INFO(context).mCategory = (s8) TIPS_INFO(context).mCategory % 3;
        } else {
            TIPS_INFO(context).mCategory = current + (count - 1);
            TIPS_INFO(context).mCategory = (s8) TIPS_INFO(context).mCategory % 3 + 3;
        }
    } else if (InputManager::Get().IsTrigger(KEY_RIGHT)) {
        s8 current = TIPS_INFO(context).mCategory;
        if (current < 3) {
            TIPS_INFO(context).mCategory = current + 1;
            TIPS_INFO(context).mCategory = (s8) TIPS_INFO(context).mCategory % 3;
        } else {
            TIPS_INFO(context).mCategory = current + 1;
            TIPS_INFO(context).mCategory = (s8) TIPS_INFO(context).mCategory % 3 + 3;
        }
    }
    if (InputManager::Get().IsTrigger(KEY_DOWN)) {
        TIPS_INFO(context).mCategory = (s8) TIPS_INFO(context).mCategory + 3;
        TIPS_INFO(context).mCategory = (s8) TIPS_INFO(context).mCategory % count;
    } else if (InputManager::Get().IsTrigger(KEY_UP)) {
        TIPS_INFO(context).mCategory = (s8) TIPS_INFO(context).mCategory + (count - 3);
        TIPS_INFO(context).mCategory = (s8) TIPS_INFO(context).mCategory % count;
    }
    if ((s8) TIPS_INFO(context).mCategory != saved) {
        SoundManager::Get().StartSeqArc(5);
    }
    if (InputManager::Get().IsPressed(1)) {
        return TIPS_INFO(context).mCategory;
    }
    return -1;
}

BOOL MyroomTipsScene::UpdateRowSelection() {
    u8 confirm = FALSE;
    u8 row       = mRow;
    u8 scroll    = mScroll;
    mScroll      = mWindowB->mCursor->mScroll;
    if (InputManager::Get().IsTrigger(KEY_UP)) {
        if (mRow != 0) {
            mRow--;
        } else if (mScroll != 0) {
            mScroll--;
        }
    } else if (InputManager::Get().IsTrigger(KEY_DOWN)) {
        if (mRow < 3) {
            mRow++;
        } else if (mScroll + 4 < mCount) {
            mScroll++;
        }
    }
    if (TouchManager::Get().mReleased != 0) {
        u8 i = 0;
        do {
            if (TouchManager::Get().IsReleasedIn(0x20, (s16) (i * 32 + 0x20), 0xc0, 0x1c)) {
                if (mRow == i) {
                    confirm = TRUE;
                } else {
                    mRow = i;
                }
                break;
            }
            i++;
        } while (i < 4);
    }
    if (InputManager::Get().IsPressed(1)) {
        confirm = TRUE;
    }
    if ((row != mRow) | (scroll != mScroll)) {
        for (u8 j = 0; j < 4; j++) {
            mIndices[j] = GetUnlockedTip((u8) (mScroll + j));
        }
        if (scroll != mScroll) {
            if (mScroll > scroll) {
                u8 delta = mScroll - scroll;
                for (s8 k = 0; k < 4; k++) {
                    if (k < 4 - delta) {
                        mTextX[k] = mTextX[k + delta];
                        mTextWidth[k] = mTextWidth[k + delta];
                        mTimers[k] = mTimers[k + delta];
                    } else {
                        mTextX[k] = 0;
                        mTextWidth[k] = 0;
                        mTimers[k] = 0x3c;
                    }
                }
            } else {
                u8 delta = scroll - mScroll;
                for (s8 k = 3; k >= 0; k--) {
                    if (k >= delta) {
                        mTextX[k] = mTextX[k - delta];
                        mTextWidth[k] = mTextWidth[k - delta];
                        mTimers[k] = mTimers[k - delta];
                    } else {
                        mTextX[k] = 0;
                        mTextWidth[k] = 0;
                        mTimers[k] = 0x3c;
                    }
                }
            }
        }
        PrintRows(-1);
        mWindowB->mCursor->SetScroll(mScroll);
        SelectWindow *window  = mWindowB;
        u8 y                  = window->mCursor->mScrollItemY;
        window->mCellPos[1].x = 0xe4;
        window->mCellPos[1].y = y;
        SoundManager::Get().StartSeqArc(5);
        mRowCells[1]->Rewind();
    }
    return confirm;
}

void MyroomTipsScene::PrintRows(s32 row) {
    s16 *widths;
    SceneContext *context = mContext;
    u8 i   = 0;
    u8 end = 4;
    if (row >= 0) {
        i   = row;
        end = i;
    }
    widths = mTextWidth;
    do {
        MI_CpuClear16(mTextBuffers[i], mTextBufferSize);
        TextManager &text = TextManager::Get();
        u16 *pixelWidth = (mTextX[i] == 0) ? (u16 *) &widths[i] : NULL;
        s16 width;
        s16 height;
        height = 0;
        width = mTextX[i];
        s8 menu = TIPS_INFO(context).mCategory;
        u16 index = sMyroomTipsEntries[menu][mIndices[i]].mText;
        MessageFile *file = mMessages;
        text.Print(file->GetText(index), file->GetLength(index), mTextBuffers[i], 0x30, 2, &width, &height, pixelWidth, 0, 2,
                   0x11, 0x10, 0, 0, 1, 1, 1);
        QueueUpload(1, UPLOAD_KIND_OBJ, 0x8000 + i * 0xc00, mTextBuffers[i], mTextBufferSize, 0);
        i++;
    } while (i < end);
}

void MyroomTipsScene::CreateMessage() {
    mMessageWindow = new MessageWindow("mes_myroom", 1, 0x3800);
    mMessageWindow->func_02065bd4(0x14, 4, 1);
    mMessageWindow->SetWidth(0xd8);
    mMessageWindow->StartMessage(0x2d, 0, 1);
}

void MyroomTipsScene::SetMessage(u32 message) {
    ClearMessageBuffer();
    mMessageWindow->StartMessage(message, 0, 1);
}

void MyroomTipsScene::DrawMessage() {
    if (mMessageWindow != NULL) {
        mMessageWindow->Update();
    }
}

void MyroomTipsScene::ClearMessageBuffer() {
    if (mMessageWindow != NULL) {
        mMessageWindow->ClearBuffer();
    }
}

void MyroomTipsScene::CountTips() {
    for (u8 i = 0; i < 6; i++) {
        mUnlocked[i] = 0;
        mNew[i]      = 0;
        s8 count     = sMyroomTipsCategoryCounts[i];
        u8 start     = sMyroomTipsCategoryFirstBits[i];
        u8 j         = 0;
        do {
            if (TEST_BIT(gUnlockedTipBits, start + j)) {
                mUnlocked[i]++;
                if (!TEST_BIT(data_020d49a1, sMyroomTipsCategoryFirstBits[i] + j)) {
                    mNew[i]++;
                }
            }
            j++;
        } while (j < count);
    }
}

u32 MyroomTipsScene::GetUnlockedTip(s16 index) {
    SceneContext *context = mContext;
    u8 i        = 0;
    do {
        if (TEST_BIT(gUnlockedTipBits, sMyroomTipsCategoryFirstBits[(s8) TIPS_INFO(context).mCategory] + i)) {
            index--;
            if (index < 0) {
                return i;
            }
        }
        i++;
    } while (i < sMyroomTipsCategoryCounts[(s8) TIPS_INFO(context).mCategory]);
    return 0;
}

void MyroomTipsScene::UpdateTextX() {
    for (u8 i = 0; i < 4; i++) {
        if (mTimers[i] != 0) {
            mTimers[i]--;
        } else if (mTextWidth[i] > 0xc0) {
            if (mTextWidth[i] <= -mTextX[i]) {
                mTextX[i] = 0xc0;
            } else {
                mTextX[i]--;
                if (mTextX[i] == 0) {
                    mTimers[i] = 0x3c;
                }
            }
            PrintRows((s8) i);
        }
    }
}
