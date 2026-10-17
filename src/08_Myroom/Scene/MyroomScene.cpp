#include "Scene/MyroomScene.hpp"
#include "Scene/ListScene.hpp"
#include "Scene/CharacterEditScene.hpp"
#include "Scene/MyroomCharaScene.hpp"
#include "Scene/MyroomGiftScene.hpp"
#include "Scene/MyroomRankingScene.hpp"
#include "Scene/MyroomShootScene.hpp"
#include "Scene/MyroomTipsScene.hpp"
#include "UI/SaveWindow.hpp"
#include "Game/ScriptPlayer.hpp"
#include "Game/UnkStruct_020d433c.hpp"
#include "Graphics/CellAnimation.hpp"
#include "Graphics/Display.hpp"
#include "System/FileSystem.hpp"
#include "UI/MessageWindow.hpp"
#include "UI/SelectWindow.hpp"

#include <nitro/gx.h>
#include <nitro/mi.h>
#include <nitro/os.h>

// The scene context's SceneInfo bytes as this scene uses them (SceneInfo's own names follow StoreScene). Only the
// bytes this scene reads differently go through MYROOM_INFO; mUnk_00, mState, mSelection, mUnk_06 and mUnk_0a keep
// SceneInfo's names and are read as ctx->mInfo.<field> outside PrintStatus (reading mState through the cast changes
// Update's code).
struct MyroomSceneInfo {
    /* 00 */ u16 mUnk_00;
    /* 02 */ u8 mState;
    /* 03 */ u8 mPage;
    /* 04 */ s8 mMenuResult;
    /* 05 */ s8 mSelection;
    /* 06 */ u8 mUnk_06;
    /* 07 */ u8 mMenuEntry;
    /* 08 */ u8 mScrollRemaining;
    /* 09 */ u8 mScrollDir;
    /* 0a */ u8 mUnk_0a;
    /* 0b */ u8 mUnk_0b;
    /* 0c */
};

#define MYROOM_INFO(ctx) (*(MyroomSceneInfo *) &(ctx)->mInfo)

// Scroll directions: the values UpdateMenu stores in mScrollDir when an arrow of the first page starts a scroll
// (mScrollRemaining counts the distance down here). PREV goes with mUnk_0a - 1 (arrow item 6 or KEY_LEFT), NEXT with
// mUnk_0a + 1 (arrow item 7 or KEY_RIGHT); UpdateScroll negates its x step for NEXT and, when the scroll ends, sets
// arrow cell 6 for PREV and arrow cell 7 otherwise.
#define MYROOM_SCROLL_PREV 1
#define MYROOM_SCROLL_NEXT 2

// Menu entries: the values HandleWindowResult stores in mMenuEntry for the chosen icon (not the cursor position),
// which Update switches on to move the event camera (state 6) and to create the screen (state 10), each named after
// the class created for it (LIST_EDIT / LIST_GALLERY: ListScene with LIST_MODE_EDIT / LIST_MODE_GALLERY).
#define MYROOM_MENU_GIFT 0
#define MYROOM_MENU_SHOOT 1
#define MYROOM_MENU_RANKING 2
#define MYROOM_MENU_CHARACTER_EDIT 3
#define MYROOM_MENU_CHARA 4
#define MYROOM_MENU_TIPS 5
#define MYROOM_MENU_LIST_EDIT 6
#define MYROOM_MENU_LIST_GALLERY 7

// The three entries of pages 1 and 2 only (sMyroomMenu1Items / sMyroomMenu2Items, the touch test, mCellAnim).
static const Vec2s sMyroomMenuEntryPos[3] = {
    {16, 72}, {96, 72}, {176, 72},
};
static const u8 sMyroomMenu2StatusTextIndices[3] = {0x3b, 0x3a, 0x39};
extern const s16 data_ov008_02228740[2][3] = {{58, 16, -667}, {-28, 4, -717}};
extern const s16 data_ov008_0222871c[2][3] = {{224, 8, -595}, {146, -39, -657}};
extern const s16 data_ov008_02228734[2][3] = {{182, 8, -665}, {96, -4, -715}};
extern const s16 data_ov008_0222874c[2][3] = {{60, 8, -606}, {-26, -4, -656}};
static const u8 sMyroomMenu0StatusTextIndices[5] = {0x32, 0x35, 0x31, 0x38, 0x33};
static const WindowItemRecord sMyroomMenu0Items[8] = {
    {0, WINDOW_ITEM_CURSOR_SKIP, 88, 56, 80, 80, 0x0, {-1, -1, -1, -1}, 0, WINDOW_TOUCH_RELEASE},
    {1, WINDOW_ITEM_CURSOR_SKIP, 186, 56, 80, 80, 0x0, {-1, -1, -1, -1}, 0, WINDOW_TOUCH_RELEASE},
    {2, WINDOW_ITEM_CURSOR_SKIP, 284, 56, 80, 80, 0x0, {-1, -1, -1, -1}, 0, WINDOW_TOUCH_RELEASE},
    {3, WINDOW_ITEM_CURSOR_SKIP, 382, 56, 80, 80, 0x0, {-1, -1, -1, -1}, 0, WINDOW_TOUCH_RELEASE},
    {4, WINDOW_ITEM_CURSOR_SKIP, 480, 56, 80, 80, 0x0, {-1, -1, -1, -1}, 0, WINDOW_TOUCH_RELEASE},
    {5, 0x2, 225, 170, 30, 20, 0x0, {-1, -1, -1, -1}, 0, WINDOW_TOUCH_RELEASE},
    {6, WINDOW_ITEM_NO_SLIDE_OUT, 0, 72, 80, 48, 0x0, {-1, -1, -1, -1}, 0, WINDOW_TOUCH_RELEASE},
    {7, WINDOW_ITEM_NO_SLIDE_OUT, 176, 72, 80, 48, 0x0, {-1, -1, -1, -1}, 0, WINDOW_TOUCH_RELEASE},
};
static const u8 sMyroomMenu1StatusTextIndices[3] = {0x34, 0x36, 0x37};
extern const s16 data_ov008_02228728[2][3] = {{118, -52, -619}, {31, -64, -669}};
static const u8 sMyroomMenu0SelectedAnimIndices[5] = {0x2, 0x5, 0x8, 0xb, 0xe};
static const u8 sMyroomMenu0MapExtraAnimIndices[5] = {0x4, 0x5, 0x2, 0x1, 0x3};
static const u8 sMyroomMenu0AnimIndices[5] = {0x1, 0x4, 0x7, 0xa, 0xd};
static const u8 data_ov008_02228764[3][8] = {
    {3, 6, 8, 10, 4}, {5, 14, 11}, {13, 12, 15},
};
static const WindowCellRecord sMyroomMenu0Cells[8] = {
    {"/Game/Myroom/myroom_b_obj_00_nce.bin", 1, 0x5000, 2, 2, 1, {2, 3, 2, 0}},
    {"/Game/Myroom/myroom_b_obj_00_nce.bin", 4, 0x5000, 2, 2, 1, {5, 6, 5, 0}},
    {"/Game/Myroom/myroom_b_obj_00_nce.bin", 7, 0x5000, 2, 2, 1, {8, 9, 8, 0}},
    {"/Game/Myroom/myroom_b_obj_00_nce.bin", 10, 0x5000, 2, 2, 1, {11, 12, 11, 0}},
    {"/Game/Myroom/myroom_b_obj_00_nce.bin", 13, 0x5000, 2, 2, 1, {14, 15, 14, 0}},
    {"/Game/2D/union_b/union_b_obj_cockpit_nce.bin", 2, 0x0, 0, 2, 1, {2, 3, 3, 0}},
    {"/Game/2D/union_b/union_b_obj_arrow_nce.bin", 0, 0x4000, 1, 2, 0, {0, 2, 0, 0}},
    {"/Game/2D/union_b/union_b_obj_arrow_nce.bin", 1, 0x4000, 1, 2, 1, {1, 3, 1, 0}},
};
#pragma section npdata begin
WindowItemRecord sMyroomMenu1Items[5] = {
    {0, WINDOW_ITEM_CURSOR_SKIP, sMyroomMenuEntryPos[0].x, sMyroomMenuEntryPos[0].y, 56, 48, 0x0, {-1, -1, -1, -1}, 0, WINDOW_TOUCH_RELEASE},
    {1, WINDOW_ITEM_CURSOR_SKIP, sMyroomMenuEntryPos[1].x, sMyroomMenuEntryPos[1].y, 56, 48, 0x0, {-1, -1, -1, -1}, 0, WINDOW_TOUCH_RELEASE},
    {2, WINDOW_ITEM_CURSOR_SKIP, sMyroomMenuEntryPos[2].x, sMyroomMenuEntryPos[2].y, 56, 48, 0x0, {-1, -1, -1, -1}, 0, WINDOW_TOUCH_RELEASE},
    {3, 0x2, 225, 170, 30, 20, 0x0, {-1, -1, -1, -1}, 0, WINDOW_TOUCH_RELEASE},
    {4, WINDOW_ITEM_CURSOR_SKIP, 128, 96, 80, 80, 0x0, {-1, -1, -1, -1}, 0, WINDOW_TOUCH_RELEASE_IF_SELECTED},
};
#pragma section npdata end
static const WindowCellRecord sMyroomMenu1Cells[5] = {
    {"/Game/Myroom/myroom_b_obj_00_nce.bin", 21, 0x5000, 2, 2, 1, {21, 22, 21, 0}},
    {"/Game/Myroom/myroom_b_obj_00_nce.bin", 23, 0x5000, 2, 2, 1, {23, 24, 23, 0}},
    {"/Game/Myroom/myroom_b_obj_00_nce.bin", 25, 0x5000, 2, 2, 1, {25, 26, 25, 0}},
    {"/Game/2D/union_b/union_b_obj_cockpit_nce.bin", 2, 0x0, 0, 2, 1, {2, 3, 3, 0}},
    {"/Game/Myroom/myroom_b_obj_00_nce.bin", 34, 0x5000, 2, 2, 1, {34, 34, 34, 0}},
};
#pragma section npdata begin
WindowItemRecord sMyroomMenu2Items[5] = {
    {0, WINDOW_ITEM_CURSOR_SKIP, sMyroomMenuEntryPos[0].x, sMyroomMenuEntryPos[0].y, 56, 48, 0x0, {-1, -1, -1, -1}, 0, WINDOW_TOUCH_RELEASE},
    {1, WINDOW_ITEM_CURSOR_SKIP, sMyroomMenuEntryPos[1].x, sMyroomMenuEntryPos[1].y, 56, 48, 0x0, {-1, -1, -1, -1}, 0, WINDOW_TOUCH_RELEASE},
    {2, WINDOW_ITEM_CURSOR_SKIP, sMyroomMenuEntryPos[2].x, sMyroomMenuEntryPos[2].y, 56, 48, 0x0, {-1, -1, -1, -1}, 0, WINDOW_TOUCH_RELEASE},
    {3, 0x2, 225, 170, 30, 20, 0x0, {-1, -1, -1, -1}, 0, WINDOW_TOUCH_RELEASE},
    {4, WINDOW_ITEM_CURSOR_SKIP, 128, 96, 80, 80, 0x0, {-1, -1, -1, -1}, 0, WINDOW_TOUCH_RELEASE_IF_SELECTED},
};
#pragma section npdata end
static const WindowCellRecord sMyroomMenu2Cells[5] = {
    {"/Game/Myroom/myroom_b_obj_00_nce.bin", 27, 0x5000, 2, 2, 1, {27, 28, 27, 0}},
    {"/Game/Myroom/myroom_b_obj_00_nce.bin", 29, 0x5000, 2, 2, 1, {29, 30, 29, 0}},
    {"/Game/Myroom/myroom_b_obj_00_nce.bin", 31, 0x5000, 2, 2, 1, {31, 32, 31, 0}},
    {"/Game/2D/union_b/union_b_obj_cockpit_nce.bin", 2, 0x0, 0, 2, 1, {2, 3, 3, 0}},
    {"/Game/Myroom/myroom_b_obj_00_nce.bin", 33, 0x5000, 2, 2, 1, {33, 33, 33, 0}},
};

#include "System/FrameTask.hpp"
#include "System/TouchManager.hpp"
#include "System/InputManager.hpp"
#include "System/SoundManager.hpp"
#include "Text/TextManager.hpp"
#include "Graphics/SpriteManager.hpp"

MyroomScene::MyroomScene() :
    SceneBase(2, 0) {
    mMessages       = NULL;
    mTextBuffer     = NULL;
    mGlyphBuffer    = NULL;
    mTextBufferSize = 0;
    mGlyphCount     = 0;
    mCellAnim       = NULL;
    MI_CpuClear8(&mContext->mInfo, sizeof(SceneInfo));
    mWindows[0] = NULL;
    mWindows[1] = NULL;
    mWindows[2] = NULL;
    mMessageWindow  = NULL;
    data_020d433c.mSaveDirty = 0;
    FrameTask::Get().StartChannel(0, 0x13);
}

MyroomScene::~MyroomScene() {
    Cleanup();
    FrameTask::Get().StopAllChannels();
}

void MyroomScene::Cleanup() {
    ClearTextBuffer();
    if (mMessages != NULL) {
        OS_Free(mMessages);
        mMessages = NULL;
    }
    if (mTextBuffer != NULL) {
        OS_Free(mTextBuffer);
        mTextBuffer = NULL;
    }
    if (mGlyphBuffer != NULL) {
        OS_Free(mGlyphBuffer);
        mGlyphBuffer = NULL;
    }
    delete mWindows[0];
    mWindows[0] = NULL;
    delete mWindows[1];
    mWindows[1] = NULL;
    delete mWindows[2];
    mWindows[2] = NULL;
    delete mMessageWindow;
    mMessageWindow = NULL;
    CellAnimation *cell = mCellAnim;
    if (cell != NULL) {
        delete cell;
    }
    mCellAnim = NULL;
}

void MyroomScene::Update() {
    SceneContext *ctx = mContext;
    switch (ctx->mInfo.mState) {
        case 0:
            InitDisplay();
            LoadGraphics();
            SetupText();
            mEvent = new ScriptPlayer(SCRIPT_KIND_MYROOM, 0, 0);
            ctx->mInfo.mState++;
            break;
        case 1:
            if (mEvent->IsRunning()) {
                return;
            }
            mEvent->func_0206ef4c("mes_rm_button", 0, 0xa, 0, 0x15);
            if (mWindows[0] != NULL) {
                mWindows[0]->Reset(-1);
                mWindows[0]->mCursor->mSelection = -1;
                mWindows[0]->mCursor->mSelection = *(s8 *) &ctx->mInfo.mUnk_06;
                mWindows[0]->mCellAnims[(u8) (s8) ctx->mInfo.mUnk_06]->SetAnimation(sMyroomMenu0SelectedAnimIndices[(s8) ctx->mInfo.mUnk_06]);
            }
            PrintStatus();
            ctx->mInfo.mState++;
            break;
        case 2:
            if (mEvent->IsRunning()) {
                return;
            }
            switch (MYROOM_INFO(ctx).mPage) {
                case 0:
                    mEvent->SetMapExtraAnim(sMyroomMenu0MapExtraAnimIndices[(s8) ctx->mInfo.mUnk_06]);
                    break;
                case 1:
                    mEvent->SetMapExtraAnim(4);
                    break;
                case 2:
                    mEvent->SetMapExtraAnim(3);
                    break;
            }
            SpriteManager::Get().StartFade(FADE_MODE_TO_ZERO);
            ctx->mInfo.mState++;
            break;
        case 3:
            switch (MYROOM_INFO(ctx).mPage) {
                case 0:
                    mWindows[0]->Draw();
                    break;
                case 1:
                    mWindows[1]->Draw();
                    break;
                case 2:
                    mWindows[2]->Draw();
                    break;
            }
            DrawText();
            if (SpriteManager::Get().IsFading()) {
                return;
            }
            ctx->mInfo.mState++;
            break;
        case 4:
            HandleWindowResult();
            UpdateMenu();
            DrawText();
            break;
        case 5:
            if (UpdateScroll()) {
                ctx->mInfo.mState = 4;
            }
            DrawText();
            break;
        case 6:
            UpdateMenu();
            DrawText();
            SpriteManager::Get().StartSubFade(FADE_MODE_DOWN);
            switch (MYROOM_INFO(ctx).mMenuEntry) {
                case MYROOM_MENU_RANKING:
                case MYROOM_MENU_CHARA:
                case MYROOM_MENU_TIPS:
                    mEvent->func_020702e0(0x3c, 8, -606, -26, -4, -656);
                    break;
                case MYROOM_MENU_LIST_EDIT:
                    mEvent->func_020702e0(0x76, -52, -619, 0x1f, -64, -669);
                    break;
                case MYROOM_MENU_CHARACTER_EDIT:
                    mEvent->func_020702e0(0xe0, 8, -595, 0x92, -39, -657);
                    break;
                case MYROOM_MENU_GIFT:
                case MYROOM_MENU_SHOOT:
                case MYROOM_MENU_LIST_GALLERY:
                    mEvent->func_020702e0(0xb6, 8, -665, 0x60, -4, -715);
                    break;
            }
            ctx->mInfo.mState++;
            break;
        case 7:
            UpdateMenu();
            DrawText();
            if (mEvent->IsRunning()) {
                return;
            }
            SpriteManager::Get().StartFade(FADE_MODE_DOWN);
            ctx->mInfo.mState++;
            break;
        case 8:
            UpdateMenu();
            DrawText();
            if (SpriteManager::Get().IsFading()) {
                return;
            }
            mEvent->RequestFinish();
            mContext->mRequest = 0;
            ctx->mInfo.mState++;
            break;
        case 9:
            if (ctx->mRequest == 0) {
                return;
            }
            SpriteManager::Get().StartFade(FADE_MODE_DOWN);
            ctx->mInfo.mState++;
            break;
        case 10:
            if (SpriteManager::Get().IsFading()) {
                return;
            }
            switch (MYROOM_INFO(ctx).mMenuEntry) {
                case MYROOM_MENU_GIFT:
                    new MyroomGiftScene();
                    break;
                case MYROOM_MENU_SHOOT:
                    new MyroomShootScene();
                    break;
                case MYROOM_MENU_RANKING:
                    new MyroomRankingScene();
                    break;
                case MYROOM_MENU_CHARA:
                    new MyroomCharaScene();
                    break;
                case MYROOM_MENU_CHARACTER_EDIT:
                    new CharacterEditScene(1);
                    break;
                case MYROOM_MENU_TIPS:
                    new MyroomTipsScene();
                    break;
                case MYROOM_MENU_LIST_EDIT:
                    new ListScene(LIST_MODE_EDIT, 0);
                    break;
                case MYROOM_MENU_LIST_GALLERY:
                    new ListScene(LIST_MODE_GALLERY, 0);
                    break;
            }
            mContext->mRequest = 0;
            Cleanup();
            ctx->mInfo.mState++;
            break;
        case 11:
            if (ctx->mRequest == 0) {
                return;
            }
            MYROOM_INFO(ctx).mMenuResult = 0;
            ctx->mInfo.mState         = 0;
            break;
        case 12:
            UpdateMenu();
            DrawText();
            mEvent->func_020702e0(0x3a, 0x10, -667, -28, 4, -717);
            ctx->mInfo.mState++;
            break;
        case 13:
            UpdateMenu();
            DrawText();
            if (mEvent->IsRunning()) {
                return;
            }
            new SaveWindow(1, 1, 0, 0x2000, 2, 0, 0, 0x1e000, 0x1a0);
            mContext->mRequest = 0;
            data_020d433c.mSaveDirty = 0;
            ctx->mInfo.mState++;
            break;
        case 14:
            if (ctx->mRequest == 0) {
                return;
            }
            GXS_SetVisiblePlane(0x11);
            ctx->mInfo.mState         = 1;
            MYROOM_INFO(ctx).mMenuResult = 0;
            FrameTask::Get().StartChannel(0, 0x13);
            break;
        case 15:
            if ((data_020d433c.mFlags & 2) && data_020d433c.mUnk_7f == SCENE_TITLE) {
                new SaveWindow(1, 1, 0, 0x2000, 2, 0, 0, 0x1e000, 0x1a0);
                mContext->mRequest = 0;
                ctx->mInfo.mState++;
            } else {
                ctx->mInfo.mState = 0x11;
                mWindows[0]->Draw();
                DrawText();
            }
            break;
        case 16:
            if (ctx->mRequest != 0) {
                ctx->mInfo.mState++;
            }
            break;
        case 17:
            SpriteManager::Get().StartFade(FADE_MODE_DOWN);
            ctx->mInfo.mState++;
            mWindows[0]->Draw();
            DrawText();
            break;
        case 18:
            mWindows[0]->Draw();
            DrawText();
            if (SpriteManager::Get().IsFading()) {
                return;
            }
            mEvent->RequestFinish();
            mContext->mRequest = 0;
            ctx->mInfo.mState++;
            break;
        case 19:
            if (ctx->mRequest != 0) {
                ctx->mInfo.mState++;
            }
            break;
        case 20:
            RequestScene(data_020d433c.mUnk_7f);
            mFinished = 1;
            break;
    }
}

void MyroomScene::InitDisplay() {
    GX_SetDispSelect(1);
    SpriteManager::Get().SetupMode(1, 7);
    GX_SetGraphicsMode(1, 0, 1);
    GXS_SetGraphicsMode(0);
    G2_SetBG0Control(1, 1, 0, 1, 1);
    G2S_SetBG0Control(0, 1, 0, 1, 0);
    G2S_SetBG1Control(0, 1, 2, 4, 0);
    G2S_SetBG2ControlText(0, 0, 4, 6);
    G2_SetBG0Offset(0, 0);
    G2S_SetBG0Offset(0, 0);
    G2S_SetBG1Offset(0, 0);
    G2S_SetBG2Offset(0, 0);
    G2S_SetBG3Offset(0, 0);
    G2S_SetBG0Priority(3);
    G2S_SetBG1Priority(3);
    G2S_SetBG2Priority(3);
    G2S_SetBG3Priority(3);
    GX_SetVisiblePlane(0x11);
    GXS_SetVisiblePlane(0x11);
    G2_SetBlendBrightness(0x1f, 0);
    G2S_SetBlendBrightness(0x1f, 0);
    GX_SetVisibleWnd(0);
    GXS_SetVisibleWnd(0);
    G2_SetWndOutsidePlane(0x1f, FALSE);
    G2S_SetWndOutsidePlane(0x1f, FALSE);
    G2_BlendNone();
    G2S_BlendNone();
    MI_CpuClearFast((void *) 0x06000000, 0x80000);
    MI_CpuClearFast((void *) 0x06400000, 0x40000);
    MI_CpuClearFast((void *) 0x06200000, 0x20000);
}

void MyroomScene::LoadGraphics() {
    ChangeDir("/Game/Myroom/");
    LoadAndQueueUpload(1, UPLOAD_KIND_BG0_SCR, 0, "myroom_b_bg_01_nsc.bin", 0, 0, 0);
    LoadAndQueueUpload(1, UPLOAD_KIND_BG0_CHAR, 0, "myroom_b_bg_01_ncg.bin", 0, 0, 0);
    LoadAndQueueUpload(1, UPLOAD_KIND_BG_EXT_PLTT, 0, "myroom_b_bg_01_ncl.bin", 0, 0, 0);
    SpriteScreen &sub = SpriteManager::Get().Screen(1);
    sub.SetObjVramMode(3);
    sub.LoadCharacters("myroom_b_obj_00.nce_ncg1D.bin", 0x5000, 0);
    sub.LoadPalette("myroom_b_obj_00_ncl.bin", 0x40, 0);
    mCellAnim            = new CellAnimation("myroom_b_obj_00_nce.bin", 0, 1, 1);
    mCellAnim->mCharBase = sub.GetCharBase(0x5000);
    mCellAnim->mPaletteBase   = 2;
    ChangeDir("/Game/2D/union_b/");
    SpriteScreen &sub2 = SpriteManager::Get().Screen(1);
    sub2.SetObjVramMode(3);
    sub2.LoadCharacters("union_b_obj_cockpit.nce_ncg1D.bin", 0, 0);
    sub2.LoadPalette("union_b_obj_cockpit_ncl.bin", 0, 0);
    sub2.LoadCharacters("union_b_obj_arrow.nce_ncg1D.bin", 0x4000, 0);
    sub2.LoadPalette("union_b_obj_arrow_ncl.bin", 0x20, 0);
    ChangeDir("/");
    SceneContext *ctx = mContext;
    s32 selection1              = 0;
    if (MYROOM_INFO(ctx).mPage == 1) {
        selection1 = (s8) ctx->mInfo.mUnk_06;
    }
    mWindows[1] = new SelectWindow(5, sMyroomMenu1Items, selection1);
    mWindows[1]->SetCells(5, sMyroomMenu1Cells);
    mWindows[1]->mCursor->mSelection = -1;
    mWindows[1]->mCursor->mSelection = selection1;
    s32 selection2                 = 0;
    if (MYROOM_INFO(ctx).mPage == 2) {
        selection2 = (s8) ctx->mInfo.mUnk_06;
    }
    mWindows[2] = new SelectWindow(5, sMyroomMenu2Items, selection2);
    mWindows[2]->SetCells(5, sMyroomMenu2Cells);
    mWindows[2]->mCursor->mSelection = -1;
    mWindows[2]->mCursor->mSelection = selection2;
    SwitchPage(MYROOM_INFO(mContext).mPage);
}

void MyroomScene::SwitchPage(u8 page) {
    SceneContext *ctx = mContext;
    if (MYROOM_INFO(ctx).mPage != page) {
        switch (MYROOM_INFO(ctx).mPage) {
            case 1:
                ctx->mInfo.mUnk_06 = 0;
                ctx->mInfo.mUnk_0a = 0;
                break;
            case 2:
                ctx->mInfo.mUnk_06 = 4;
                ctx->mInfo.mUnk_0a = 4;
                break;
        }
        MYROOM_INFO(ctx).mPage = page;
        func_ov008_0221d2c0();
    }
    switch (MYROOM_INFO(ctx).mPage) {
        case 0:
            delete mWindows[0];
            mWindows[0] = new SelectWindow(8, sMyroomMenu0Items, (s8) ctx->mInfo.mUnk_06);
            mWindows[0]->SetCells(8, sMyroomMenu0Cells);
            mWindows[0]->mCursor->mSelection = -1;
            MYROOM_INFO(ctx).mMenuResult      = 0;
            break;
        case 1:
            delete mWindows[1];
            mWindows[1] = new SelectWindow(5, sMyroomMenu1Items, -1);
            mWindows[1]->SetCells(5, sMyroomMenu1Cells);
            mWindows[1]->mCursor->mSelection = -1;
            mWindows[1]->mCursor->mSelection = *(s8 *) &ctx->mInfo.mUnk_06;
            break;
        case 2:
            delete mWindows[2];
            mWindows[2] = new SelectWindow(5, sMyroomMenu2Items, -1);
            mWindows[2]->SetCells(5, sMyroomMenu2Cells);
            mWindows[2]->mCursor->mSelection = -1;
            mWindows[2]->mCursor->mSelection = *(s8 *) &ctx->mInfo.mUnk_06;
            break;
    }
    RefreshCellLayout();
}

void MyroomScene::UpdateMenu() {
    SceneContext *ctx = mContext;
    u8 first                    = 0;
    u8 last                     = 0;
    u8 count                    = 0;
    SelectWindow *window        = NULL;
    ctx->mInfo.mSelection       = *(s8 *) &ctx->mInfo.mUnk_06;
    switch (MYROOM_INFO(ctx).mPage) {
        case 0:
            first  = 6;
            window = mWindows[0];
            last   = 7;
            count  = 5;
            break;
        case 1:
            count  = 3;
            window = mWindows[1];
            last   = 2;
            break;
        case 2:
            count  = 3;
            window = mWindows[2];
            last   = 2;
            break;
    }
    if (window == NULL) {
        return;
    }
    if (ctx->mInfo.mState == 4) {
        u8 hit    = window->Update(TRUE);
        if ((u8) MYROOM_INFO(ctx).mMenuResult != 2) {
            MYROOM_INFO(ctx).mMenuResult = hit;
        }
        s32 delta = 0;
        switch (MYROOM_INFO(ctx).mPage) {
            case 0:
                switch ((u8) MYROOM_INFO(ctx).mMenuResult) {
                    case 1:
                    case 2:
                        break;
                    case 3: {
                        u8 id = window->GetSelectedId();
                        if (id == first) {
                            delta = -1;
                        } else if (id == last) {
                            delta = 1;
                        }
                    }
                    // fall through
                    default:
                        if (delta == 0 && window->mState == 1) {
                            if (InputManager::Get().IsTrigger(KEY_LEFT)) {
                                if ((s8) ctx->mInfo.mUnk_06 != 0) {
                                    window->SetResultItem(first, 3);
                                    SoundManager::Get().StartSeqArc(0);
                                }
                            } else if (InputManager::Get().IsTrigger(KEY_RIGHT)) {
                                if ((s8) ctx->mInfo.mUnk_06 < count - 1) {
                                    window->SetResultItem(last, 3);
                                    SoundManager::Get().StartSeqArc(0);
                                }
                            }
                        }
                        break;
                }
                if (delta != 0) {
                    MYROOM_INFO(ctx).mScrollDir = delta > 0 ? MYROOM_SCROLL_NEXT : MYROOM_SCROLL_PREV;
                    ctx->mInfo.mUnk_0a += delta;
                    StartScroll();
                    MYROOM_INFO(ctx).mMenuResult = 0;
                }
                break;
            case 1:
            case 2:
                switch ((u8) MYROOM_INFO(ctx).mMenuResult) {
                    case 1:
                    case 2:
                        break;
                    case 3: {
                        u8 id = window->GetSelectedId();
                        if (id >= first && id <= last) {
                            delta = (s8) (id - (s8) ctx->mInfo.mUnk_06 - first);
                        }
                    }
                    // fall through
                    default:
                        if (delta == 0) {
                            u8 n = last - first + 1;
                            u8 k = 0;
                            do {
                                if (TouchManager::Get().IsPressedIn(sMyroomMenuEntryPos[k].x, sMyroomMenuEntryPos[k].y, 0x38,
                                                                      0x30)) {
                                    s8 selection = (s8) ctx->mInfo.mUnk_06;
                                    if (k != selection) {
                                        delta = (s8) (k - selection - first);
                                    } else if (window->mState == 1) {
                                        window->SetResultItem((u8) selection, 3);
                                        SoundManager::Get().StartSeqArc(0);
                                    }
                                }
                                k++;
                            } while (k < n);
                            if (window->mState == 1) {
                                if (InputManager::Get().IsTrigger(KEY_LEFT)) {
                                    s8 selection = (s8) ctx->mInfo.mUnk_06;
                                    if (selection > first) {
                                        delta = -1;
                                    } else {
                                        delta = (s8) (last - selection - first);
                                    }
                                } else if (InputManager::Get().IsTrigger(KEY_RIGHT)) {
                                    s8 selection = (s8) ctx->mInfo.mUnk_06;
                                    if (selection < last) {
                                        delta = 1;
                                    } else {
                                        delta = (s8) -selection;
                                    }
                                }
                            }
                        }
                        break;
                }
                if (delta != 0) {
                    ctx->mInfo.mUnk_06 = (s8) ctx->mInfo.mUnk_06 + delta;
                    window->mCursor->mSelection = (s8) ctx->mInfo.mUnk_06 + first;
                    PrintStatus();
                    MYROOM_INFO(ctx).mMenuResult = 0;
                    SoundManager::Get().StartSeqArc(0);
                }
                break;
        }
    }
    window->Draw();
    switch (MYROOM_INFO(ctx).mPage) {
        case 1:
        case 2:
            mCellAnim->Update();
            s8 cur = ctx->mInfo.mUnk_06;
            mCellAnim->Draw(1, sMyroomMenuEntryPos[cur].x, sMyroomMenuEntryPos[cur].y, 0);
            break;
    }
}

void MyroomScene::StartScroll() {
    SceneContext *ctx = mContext;
    ctx->mInfo.mState           = 5;
    MYROOM_INFO(ctx).mScrollRemaining       = 0x62;
    if (MYROOM_INFO(ctx).mPage != 0) {
        return;
    }
    SelectWindow *window = mWindows[0];
    u8 i;
    WindowItem *items = window->mCursor->mItems;
    for (i = 0; i < 5; i++) {
        if (ctx->mInfo.mUnk_0a == i) {
            window->mCellAnims[i]->ResetAnimation();
            window->mCellAnims[i]->SetAnimation(sMyroomMenu0SelectedAnimIndices[i]);
        } else if ((s8) ctx->mInfo.mUnk_06 == i) {
            window->mCellAnims[i]->SetMode(CELL_ANIM_MODE_BACKWARD_ONCE);
        }
        items[i].mFlags = WINDOW_ITEM_CURSOR_SKIP;
    }
    if (ctx->mInfo.mUnk_0a == 0) {
        window->mButtons[6].mEnabled = 0;
        items[6].mFlags              = WINDOW_ITEM_CURSOR_SKIP;
    }
    if (ctx->mInfo.mUnk_0a == 4) {
        window->mButtons[7].mEnabled = 0;
        items[7].mFlags              = WINDOW_ITEM_CURSOR_SKIP;
    }
    if (MYROOM_INFO(ctx).mPage != 0) {
        return;
    }
    if ((s32) ctx->mInfo.mUnk_0a >= 3) {
        window->mButtons[0].mEnabled = 0;
    } else {
        window->mButtons[0].mEnabled = 1;
    }
    if (ctx->mInfo.mUnk_0a >= 2) {
        window->mButtons[4].mEnabled = 1;
    } else {
        window->mButtons[4].mEnabled = 0;
    }
}

void MyroomScene::HandleWindowResult() {
    SceneContext *ctx = mContext;
    switch ((u8) MYROOM_INFO(ctx).mMenuResult) {
        case 2:
            switch (MYROOM_INFO(ctx).mPage) {
                case 0:
                    ctx->mInfo.mState = 0xf;
                    break;
                case 1:
                    if (mWindows[1]->mCellAnims[3]->mFinished == 0) {
                        return;
                    }
                    SwitchPage(0);
                    mWindows[1]->Reset(0);
                    break;
                case 2:
                    if (mWindows[2]->mCellAnims[3]->mFinished == 0) {
                        return;
                    }
                    SwitchPage(0);
                    mWindows[2]->Reset(0);
                    break;
            }
            break;
        case 1:
        case 3: {
            s8 selection = (s8) ctx->mInfo.mUnk_06;
            if (selection != 0 && ctx->mInfo.mSelection != selection) {
                return;
            }
            switch (MYROOM_INFO(ctx).mPage) {
                case 0:
                    switch (selection) {
                        case 0:
                            ctx->mInfo.mUnk_06 = 0;
                            ctx->mInfo.mUnk_0a = 0;
                            SwitchPage(1);
                            mWindows[1]->mCellAnims[4]->ResetAnimation();
                            break;
                        case 1:
                            ctx->mInfo.mState = 0xc;
                            break;
                        case 2:
                            MYROOM_INFO(ctx).mMenuEntry = MYROOM_MENU_LIST_EDIT;
                            ctx->mInfo.mState  = 6;
                            break;
                        case 3:
                            MYROOM_INFO(ctx).mMenuEntry = MYROOM_MENU_CHARACTER_EDIT;
                            ctx->mInfo.mState  = 6;
                            break;
                        case 4:
                            ctx->mInfo.mUnk_06 = 0;
                            ctx->mInfo.mUnk_0a = 0;
                            SwitchPage(2);
                            mWindows[2]->mCellAnims[4]->ResetAnimation();
                            break;
                        case 5:
                            ctx->mInfo.mState = 0xf;
                            break;
                    }
                    break;
                case 1:
                    switch (selection) {
                        case 0:
                            MYROOM_INFO(ctx).mMenuEntry = MYROOM_MENU_TIPS;
                            ctx->mInfo.mState  = 6;
                            break;
                        case 1:
                            MYROOM_INFO(ctx).mMenuEntry = MYROOM_MENU_RANKING;
                            ctx->mInfo.mState  = 6;
                            break;
                        case 2:
                            MYROOM_INFO(ctx).mMenuEntry = MYROOM_MENU_CHARA;
                            ctx->mInfo.mState  = 6;
                            break;
                        case 3:
                            if (mWindows[1]->mCellAnims[3]->mFinished == 0) {
                                return;
                            }
                            SwitchPage(0);
                            mWindows[1]->Reset(0);
                            break;
                    }
                    break;
                case 2:
                    switch (selection) {
                        case 0:
                            MYROOM_INFO(ctx).mMenuEntry = MYROOM_MENU_LIST_GALLERY;
                            ctx->mInfo.mState  = 6;
                            break;
                        case 1:
                            MYROOM_INFO(ctx).mMenuEntry = MYROOM_MENU_SHOOT;
                            ctx->mInfo.mState  = 6;
                            break;
                        case 2:
                            MYROOM_INFO(ctx).mMenuEntry = MYROOM_MENU_GIFT;
                            ctx->mInfo.mState  = 6;
                            break;
                        case 3:
                            if (mWindows[2]->mCellAnims[3]->mFinished == 0) {
                                return;
                            }
                            SwitchPage(0);
                            mWindows[2]->Reset(0);
                            break;
                    }
                    break;
            }
            break;
        }
    }
}

BOOL MyroomScene::UpdateScroll() {
    SceneContext *ctx = mContext;
    BOOL done;
    SelectWindow *window;
    WindowItem *items;
    s32 step;
    step = 0xe;
    done = FALSE;
    if (MYROOM_INFO(ctx).mScrollRemaining < 0xe) {
        step = MYROOM_INFO(ctx).mScrollRemaining;
    }
    MYROOM_INFO(ctx).mScrollRemaining -= step;
    if (MYROOM_INFO(ctx).mScrollDir == MYROOM_SCROLL_NEXT) {
        step = (s16) -step;
    }
    switch (MYROOM_INFO(ctx).mPage) {
        case 0:
            window = mWindows[0];
            break;
        default:
            return FALSE;
    }
    if (window->mState != 1) {
        window->Update(TRUE);
    }
    items = window->mCursor->mItems;
    for (u8 i = 0; i < 5; i++) {
        Vec2s pos = window->GetCellPos(i);
        pos.x += step;
        window->SetCellPos(i, pos);
        items[i].mX = step + items[i].mX;
    }
    if (MYROOM_INFO(ctx).mScrollRemaining == 0) {
        window->mCellAnims[(u8) (s8) ctx->mInfo.mUnk_06]->SetAnimation(sMyroomMenu0AnimIndices[(s8) ctx->mInfo.mUnk_06]);
        items[ctx->mInfo.mUnk_0a].mFlags     = WINDOW_ITEM_NO_SLIDE_OUT;
        items[(s8) ctx->mInfo.mUnk_06].mFlags     = WINDOW_ITEM_CURSOR_SKIP;
        window->mCursor->mSelection          = ctx->mInfo.mUnk_0a;
        ctx->mInfo.mUnk_06                   = ctx->mInfo.mUnk_0a;
        mEvent->func_0206f564(data_ov008_02228764[MYROOM_INFO(ctx).mPage][(s8) ctx->mInfo.mUnk_06], 0xa, -1);
        if (MYROOM_INFO(ctx).mPage == 0) {
            mEvent->SetMapExtraAnim(sMyroomMenu0MapExtraAnimIndices[(s8) ctx->mInfo.mUnk_06]);
        }
        if ((s8) ctx->mInfo.mUnk_06 != 0) {
            window->mButtons[6].mEnabled = 1;
            items[6].mFlags              = WINDOW_ITEM_NO_SLIDE_OUT;
        }
        if ((s8) ctx->mInfo.mUnk_06 != 4) {
            window->mButtons[7].mEnabled = 1;
            items[7].mFlags              = WINDOW_ITEM_NO_SLIDE_OUT;
        }
        window->mCellAnims[6]->Rewind();
        window->mCellAnims[7]->Rewind();
        if (MYROOM_INFO(ctx).mScrollDir == MYROOM_SCROLL_PREV) {
            window->mCellAnims[6]->SetAnimation(0);
        } else {
            window->mCellAnims[7]->SetAnimation(1);
        }
        PrintStatus();
        done = TRUE;
    }
    window->Draw();
    return done;
}

void MyroomScene::RefreshCellLayout() {
    SceneContext *ctx = mContext;
    s16 offset                  = (s8) ctx->mInfo.mUnk_06 * 0x62;
    if (MYROOM_INFO(ctx).mPage != 0) {
        return;
    }
    SelectWindow *window  = mWindows[0];
    WindowItem *items     = window->mCursor->mItems;
    for (u8 i = 0; i < 5; i++) {
        Vec2s pos = window->mCellPos[i];
        pos.x = 0x62 * i + 0x58 - offset;
        window->SetCellPos(i, pos);
        items[i].mX = pos.x;
        if ((s8) ctx->mInfo.mUnk_06 == i) {
            window->mCellAnims[i]->SetAnimation(sMyroomMenu0SelectedAnimIndices[i]);
            items[i].mFlags = WINDOW_ITEM_NO_SLIDE_OUT;
        } else {
            window->mCellAnims[i]->SetAnimation(sMyroomMenu0AnimIndices[i]);
            items[i].mFlags = WINDOW_ITEM_CURSOR_SKIP;
        }
        window->mCellAnims[i]->ResetAnimation();
    }
    if ((s8) ctx->mInfo.mUnk_06 != 0) {
        window->mButtons[6].mEnabled = 1;
        items[6].mFlags              = WINDOW_ITEM_NO_SLIDE_OUT;
    } else {
        window->mButtons[6].mEnabled = 0;
        items[6].mFlags              = WINDOW_ITEM_CURSOR_SKIP;
    }
    if ((s8) ctx->mInfo.mUnk_06 == 4) {
        window->mButtons[7].mEnabled = 0;
        items[7].mFlags              = WINDOW_ITEM_CURSOR_SKIP;
    } else {
        window->mButtons[7].mEnabled = 1;
        items[7].mFlags              = WINDOW_ITEM_NO_SLIDE_OUT;
    }
    if (MYROOM_INFO(ctx).mPage == 0) {
        if ((s32) ctx->mInfo.mUnk_0a >= 3) {
            window->mButtons[0].mEnabled = 0;
        } else {
            window->mButtons[0].mEnabled = 1;
        }
        if (ctx->mInfo.mUnk_0a >= 2) {
            window->mButtons[4].mEnabled = 1;
        } else {
            window->mButtons[4].mEnabled = 0;
        }
    }
    window->mCursor->mSelection = *(s8 *) &ctx->mInfo.mUnk_06;
}

void MyroomScene::SetupText() {
    if (mMessages != NULL) {
        OS_Free(mMessages);
    }
    if (mTextBuffer != NULL) {
        OS_Free(mTextBuffer);
    }
    if (mGlyphBuffer != NULL) {
        OS_Free(mGlyphBuffer);
    }
    mMessages       = (MessageFile *) LoadFile("/font/mes_myroom.mes", 0, 0, 0, 0, 0);
    mTextBufferSize = TextManager::Get().BufferSize(0x20, 2);
    mGlyphCount     = TextManager::Get().GlyphCount(8);
    mTextBuffer     = OS_Alloc(mTextBufferSize);
    ClearTextBuffer();
    mGlyphBuffer = OS_Alloc(mGlyphCount * 0xc);
    TextManager::Get().Init(mGlyphBuffer, 1, 0, 0x20, 2, 0x21);
    TextManager::Get().SetColor(1, 0, 0xf);
    mMessageWindow = new MessageWindow("mes_myroom", 1, 0x1800);
    mMessageWindow->func_02065bd4(0x14, 4, 0);
    mMessageWindow->SetWidth(0xd8);
    switch (MYROOM_INFO(mContext).mPage) {
        case 0:
            mMessageWindow->StartMessage(0x24, 0, 1);
            break;
        case 1:
            mMessageWindow->StartMessage(0x25, 0, 1);
            break;
        case 2:
            mMessageWindow->StartMessage(0x26, 0, 1);
            break;
    }
}

void MyroomScene::func_ov008_0221d2c0() {
    ClearMessageBuffer();
    SceneContext *ctx = mContext;
    switch (MYROOM_INFO(ctx).mPage) {
        case 0:
            mMessageWindow->StartMessage(0x24, 0, 1);
            break;
        case 1:
            mMessageWindow->StartMessage(0x25, 0, 1);
            break;
        case 2:
            mMessageWindow->StartMessage(0x26, 0, 1);
            break;
    }
    PrintStatus();
    s8 selection = (s8) ctx->mInfo.mUnk_06;
    if (selection < 0) {
        return;
    }
    mEvent->func_0206f564(data_ov008_02228764[MYROOM_INFO(ctx).mPage][selection], 0xa, -1);
}

void MyroomScene::DrawText() {
    SceneContext *ctx = mContext;
    if (mGlyphBuffer != NULL) {
        SpriteManager::Get().Screen(1).AddOam(mGlyphBuffer, mGlyphCount,
                                                     (s16) (0x80 - ((u32) ctx->mInfo.mUnk_00 >> 1)), 0x90, 0, 0x60,
                                                     0xf, 0);
    }
    if (mMessageWindow != NULL) {
        mMessageWindow->Update();
    }
}

void MyroomScene::ClearMessageBuffer() {
    if (mMessageWindow != NULL) {
        mMessageWindow->ClearBuffer();
    }
}

void MyroomScene::PrintStatus() {
    u8 index = 0;
    MyroomSceneInfo *info = &MYROOM_INFO(mContext);
    switch (info->mPage) {
        case 0:
            index = sMyroomMenu0StatusTextIndices[(s8) info->mUnk_06];
            break;
        case 1:
            index = sMyroomMenu1StatusTextIndices[(s8) info->mUnk_06];
            break;
        case 2:
            index = sMyroomMenu2StatusTextIndices[(s8) info->mUnk_06];
            break;
    }
    ClearTextBuffer();
    TextManager::Get().mUse4bppFont = 1;
    TextManager &text          = TextManager::Get();
    s16 height;
    s16 width;
    width              = 0;
    height             = 0;
    MessageFile *table = mMessages;
    text.Print(table->GetText(index), table->GetLength(index), mTextBuffer, 0x20, 2, &height, &width, &info->mUnk_00, 0, 2,
               0x21, 0xe, 0, 0, 0, 1, 1);
    TextManager::Get().mUse4bppFont = 0;
    QueueUpload(SpriteManager::Get().Screen(1).mBgIndex, UPLOAD_KIND_OBJ, 0x3000, mTextBuffer, mTextBufferSize, 0);
}

void MyroomScene::ClearTextBuffer() {
    if (mTextBuffer != NULL) {
        MI_CpuClear16(mTextBuffer, mTextBufferSize);
    }
}
