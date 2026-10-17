#include "Scene/TournamentScene.hpp"

#include "Unknown/UnkFunctions.hpp"
#include "Game/Random.hpp"
#include "Game/ScriptPlayer.hpp"
#include "Game/UnkStruct_020d433c.hpp"
#include "Game/SaveData.hpp"
#include "Graphics/CellAnimation.hpp"
#include "Scene/TournamentSubScene.hpp"
#include "System/FrameTask.hpp"
#include "System/SoundManager.hpp"
#include "System/InputManager.hpp"
#include "Text/TextManager.hpp"
#include "UI/UnlockPopup.hpp"
#include "Graphics/SpriteManager.hpp"
#include "UI/SaveWindow.hpp"
#include "Graphics/Display.hpp"
#include "System/FileSystem.hpp"
#include "UI/MessageWindow.hpp"
#include "UI/SelectWindow.hpp"
#include "Graphics/CharacterIcons.hpp"

#include <nitro/gx.h>
#include <nitro/mi.h>
#include <nitro/os.h>
#include <stdio.h>

struct TourPoint {
    u8 x;
    u8 y;
};

static const u8 sTournamentModeMainTextIndices[6] = {0, 1, 2, 3, 4, 5};
static const u8 sTournamentMenuAnimIndices[3] = {4, 1, 10};
static const u8 sTournamentMenuMainTextIndices[3] = {0, 2, 5};
static const u8 sTournamentMenuSelectedAnimIndices[3] = {5, 2, 11};
static const u8 sTournamentMenuSubTextIndices[3] = {9, 10, 8};
static const u8 data_ov002_02220a0c[3] = {1, 0, 2};
static const TourPoint sTournamentCharacterPos[6][9] = {
    {{0x36, 0x3b}, {0x71, 0x53}, {0xad, 0x3b}, {0x0, 0x0}, {0x0, 0x0}, {0x0, 0x0}, {0x0, 0x0}, {0x0, 0x0}, {0x0, 0x0}},
    {{0xa, 0x37}, {0x2a, 0x37}, {0x44, 0x58}, {0x64, 0x58}, {0x7e, 0x37}, {0x9e, 0x37}, {0xb8, 0x58}, {0xd8, 0x58}, {0x0, 0x0}},
    {{0x13, 0x47}, {0x30, 0x38}, {0x30, 0x56}, {0x63, 0x47}, {0x80, 0x38}, {0x80, 0x56}, {0xb3, 0x47}, {0xd0, 0x38}, {0xd0, 0x56}},
    {{0x22, 0x40}, {0x4a, 0x50}, {0x72, 0x40}, {0x9a, 0x50}, {0xc2, 0x40}, {0x0, 0x0}, {0x0, 0x0}, {0x0, 0x0}, {0x0, 0x0}},
    {{0x12, 0x4c}, {0x52, 0x44}, {0x92, 0x4c}, {0xd2, 0x44}, {0x0, 0x0}, {0x0, 0x0}, {0x0, 0x0}, {0x0, 0x0}, {0x0, 0x0}},
    {{0x1b, 0x40}, {0x38, 0x4f}, {0x55, 0x40}, {0x72, 0x4f}, {0x8f, 0x40}, {0xac, 0x4f}, {0xc9, 0x40}, {0x0, 0x0}, {0x0, 0x0}},
};
static const u8 sTotals[6] = {3, 8, 9, 5, 4, 7};
static const u8 sCounts[6] = {1, 2, 3, 1, 1, 1};
static const WindowItemRecord sTournamentMenuItems[6] = {
    {0, 0x8000, 88, 56, 80, 80, 0, {-1, -1, -1, -1}, 0, 1},
    {1, 0x8000, 186, 56, 80, 80, 0, {-1, -1, -1, -1}, 0, 1},
    {2, 0x8000, 284, 56, 80, 80, 0, {-1, -1, -1, -1}, 0, 1},
    {3, 0x8, 0, 72, 80, 48, 0, {-1, -1, -1, -1}, 0, 1},
    {4, 0x8, 176, 72, 80, 48, 0, {-1, -1, -1, -1}, 0, 1},
    {5, 0x2, 225, 170, 30, 20, 0, {-1, -1, -1, -1}, 0, 1},
};
static const TourPoint sTournamentMoveTargets[7] = {{0x72, 0x51}, {0x62, 0x51}, {0x63, 0x51}, {0x72, 0x51}, {0x72, 0x51}, {0x72, 0x51}, {0, 0}};
static const u8 data_ov002_02220a6c[22][3] = {
    {3, 4, 5}, {6, 7, 8}, {9, 10, 11}, {12, 13, 14}, {15, 16, 17}, {0, 0, 0}, {18, 19, 20}, {0, 1, 2},
    {21, 22, 23}, {24, 25, 26}, {27, 28, 29}, {30, 31, 32}, {33, 34, 35}, {36, 37, 38}, {39, 40, 41},
    {42, 43, 44}, {45, 46, 47}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0},
};
static const TourPoint sTournamentCellPos[3] = {{0x80, 0x60}, {0x80, 0x60}, {0x80, 0x60}};
static const WindowCellRecord sTournamentModeMenuCells[7] = {
    {"/Game/Tournament/Obj/tournament_b_obj_02_nce.bin", 1, 0x0, 0, 3, 1, {1, 2, 1, 0}},
    {"/Game/Tournament/Obj/tournament_b_obj_02_nce.bin", 3, 0x0, 0, 3, 1, {3, 4, 3, 0}},
    {"/Game/Tournament/Obj/tournament_b_obj_02_nce.bin", 5, 0x0, 0, 3, 1, {5, 6, 5, 0}},
    {"/Game/Tournament/Obj/tournament_b_obj_02_nce.bin", 7, 0x0, 0, 3, 1, {7, 8, 7, 0}},
    {"/Game/Tournament/Obj/tournament_b_obj_02_nce.bin", 9, 0x0, 0, 3, 1, {9, 10, 9, 0}},
    {"/Game/Tournament/Obj/tournament_b_obj_02_nce.bin", 11, 0x0, 0, 3, 1, {11, 12, 11, 0}},
    {"/Game/2D/union_b/union_b_obj_cockpit_nce.bin", 2, 0x4000, 3, 3, 1, {2, 3, 3, 0}},
};
static const u8 data_ov002_02220a54[1][24] = {{2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2}};
static const WindowCellRecord sTournamentMenuCells[6] = {
    {"/Game/Tournament/Obj/tournament_b_obj_00_nce.bin", 4, 0x0, 0, 3, 1, {5, 6, 5, 0}},
    {"/Game/Tournament/Obj/tournament_b_obj_00_nce.bin", 1, 0x0, 0, 3, 1, {2, 3, 2, 0}},
    {"/Game/Tournament/Obj/tournament_b_obj_00_nce.bin", 10, 0x0, 0, 3, 1, {11, 12, 11, 0}},
    {"/Game/2D/union_b/union_b_obj_arrow_nce.bin", 0, 0x6000, 4, 3, 0, {0, 2, 0, 0}},
    {"/Game/2D/union_b/union_b_obj_arrow_nce.bin", 1, 0x6000, 4, 3, 1, {1, 3, 1, 0}},
    {"/Game/2D/union_b/union_b_obj_cockpit_nce.bin", 2, 0x4000, 3, 3, 1, {2, 3, 3, 0}},
};
static const u8 sModeTables[3][8] = {{0x12, 0x13, 0x14, 0x15, 0x16, 0x17}, {1, 3, 6, 7, 8, 9}, {6, 1, 2, 5, 0, 3}};
static const WindowItemRecord sTournamentModeMenuItems[7] = {
    {0, 0x1, 16, 24, 64, 42, 0, {3, 3, 2, 1}, 0, 2},
    {1, 0x1, 96, 24, 64, 42, 0, {4, 4, 0, 2}, 0, 2},
    {2, 0x1, 176, 24, 64, 42, 0, {5, 5, 1, 0}, 0, 2},
    {3, 0x1, 16, 80, 64, 42, 0, {0, 0, 5, 4}, 0, 2},
    {4, 0x1, 96, 80, 64, 42, 0, {1, 1, 3, 5}, 0, 2},
    {5, 0x1, 176, 80, 64, 42, 0, {2, 2, 4, 3}, 0, 2},
    {6, 0x2, 225, 170, 30, 20, 0, {-1, -1, -1, -1}, 0, 1},
};
static const WindowCellRecord sWindowCells[3] = {
    {"/Game/2D/union_b/union_b_obj_window_nce.bin", 3, 0x10000, 13, 0, 1, {3, 5, 4, 0}},
    {"/Game/2D/union_b/union_b_obj_window_nce.bin", 7, 0x10000, 13, 0, 1, {6, 8, 7, 0}},
    {"/Game/2D/union_b/union_b_obj_window_nce.bin", 10, 0x10000, 13, 0, 1, {10, 10, 10, 0}},
};
static const WindowItemRecord sWindowItems[3] = {
    {0, 0x1, 45, 152, 78, 20, 0, {0, 0, 1, 1}, 0, 2},
    {1, 0x1, 133, 152, 78, 20, 0, {1, 1, 0, 0}, 0, 2},
    {2, 0x8000, 120, 18, 78, 20, 0, {-1, -1, -1, -1}, 0, 0},
};

extern const u8 data_020d441a[];

 // text shown in the lower box

TournamentScene::TournamentScene() :
    SceneBase(1000, 0) {
    mObj38  = NULL;
    mEvent  = NULL;
    mMenuWindow   = NULL;
    mWindow = NULL;
    mMessageWindow = NULL;
    mTextWidth = 0;
    mTextHeight = 0;
    mUnk_a0 = 0;
    mUnk_a2 = 0;
    mMoveX = 0;
    mMoveY = 0;
    mMoveDistanceX = 0;
    mMoveDistanceY = 0;
    mUnk_ac = 0;
    mMainTextTimer = 0;
    mState = 0;
    mMode = 0;
    mSelection = 0;
    mMenuVisible = 0;
    mMainScreenVisible = 0;
    mCellsBVisible = 0;
    mWindowOpen = 0;
    mUnk_b9 = 0;
    mLastIndex = 0;
    mUnk_be = 0;
    mBattleStep = 0;
    mBrightness = 0;
    mUnk_c1 = 0;
    mUnk_cb = 0;
    mFirstItemState = 0;
    mOtherItemState = 0;
    mUnk_ce = 0;
    mMoveFrames = 0;
    mUnk_d0 = 0;
    mTextRows = 2;
    mUnk_d2 = 0;
    mUnk_d3 = 1;
    mNextBgLayer = 0;
    mBlend = 0;
    mMainTextPage = 0;
    mMainTextLineCount = 0;
    for (u8 i = 0; i < 5; i++) {
        mCellsA[i] = NULL;
        mCellsB[i] = NULL;
    }
    mCell20 = NULL;
    for (u8 i = 0; i < 4; i++) {
        mMessages[i]    = NULL;
        mTextBuffers[i] = NULL;
        mGlyphBuffers[i] = NULL;
    }
    if (gSaveData[0].mUnk_5bf_0) {
        mLastIndex = 1;
    } else {
        mLastIndex = 0;
    }
    MI_CpuClear8(mCharacterIds, 9);
    Cleanup();
}

TournamentScene::~TournamentScene() {
    Cleanup();
}

void TournamentScene::Cleanup() {
    for (u8 i = 0; i < 5; i++) {
        delete mCellsA[i];
        mCellsA[i] = NULL;
        delete mCellsB[i];
        mCellsB[i] = NULL;
    }
    delete mCell20;
    mCell20 = NULL;
    delete mObj38;
    mObj38 = NULL;
    delete mMenuWindow;
    mMenuWindow = NULL;
    delete mWindow;
    mWindow = NULL;
    ClearSubScreenText();
    ClearMainScreenText();
    for (u8 i = 0; i < 4; i++) {
        if (mMessages[i] != NULL) {
            OS_Free(mMessages[i]);
            mMessages[i] = NULL;
        }
        if (mTextBuffers[i] != NULL) {
            OS_Free(mTextBuffers[i]);
            mTextBuffers[i] = NULL;
        }
        if (mGlyphBuffers[i] != NULL) {
            OS_Free(mGlyphBuffers[i]);
            mGlyphBuffers[i] = NULL;
        }
    }
    delete mMessageWindow;
    mMessageWindow = NULL;
    MI_CpuClearFast((void *) 0x06000000, 0x80000);
    MI_CpuClearFast((void *) 0x06200000, 0x20000);
    MI_CpuClearFast((void *) 0x06400000, 0x40000);
    MI_CpuClearFast((void *) 0x06600000, 0x20000);
}

void TournamentScene::Update() {
    switch (mState) {
        case 0:
            InitScriptDisplay();
            LoadMenu();
            mEvent = new ScriptPlayer(3, 0, 0);
            mMenuVisible = 0;
            mTimer  = 0x10;
            mState++;
            break;
        case 1:
            if (--mTimer != 0) {
                break;
            }
            mEvent->SetupBattle(gSaveData[0].mUnk_42, gSaveData[0].mUnk_43);
            if (mEvent->GetSaveRequest()) {
                mState = 5;
                break;
            }
            if (mEvent->GetUnlockPopupHandle()) {
                mState = 9;
                break;
            }
            if (mEvent->IsRunning()) {
                SpriteManager::Get().StartFade(2);
                mState++;
            } else {
                SpriteManager::Get().StartFade(3);
                if (gSaveData[0].mUnk_42 == 10) {
                    if (mEvent != NULL) {
                        mEvent->RequestFinish();
                        mEvent = NULL;
                    }
                    mState = 0x1d;
                } else {
                    mState = 0xe;
                }
            }
            break;
        case 2:
            if (SpriteManager::Get().IsFading()) {
                break;
            }
            mState++;
            break;
        case 3:
            if (!mEvent->IsRunning()) {
                mUnk_d3 = 1;
                SpriteManager::Get().StartFade(3);
                if (gSaveData[0].mUnk_42 == 10) {
                    if (mEvent != NULL) {
                        mEvent->RequestFinish();
                        mEvent = NULL;
                    }
                    mState = 0x1d;
                } else {
                    mState = 0xe;
                }
                break;
            }
            if (mEvent->IsGraphicsLoaded()) {
                ReloadMenu();
                mEvent->ClearGraphicsLoaded();
                break;
            }
            if (mEvent->GetSaveRequest()) {
                SpriteManager::Get().StartSubFade(3);
                mState = 4;
                break;
            }
            if (mEvent->GetUnlockPopupHandle()) {
                SpriteManager::Get().StartSubFade(3);
                mState = 9;
                break;
            }
            if (HandleEventResult()) {
                if (mUnk_d0 != 0) {
                    SpriteManager::Get().StartFade(3);
                    mState = 0xc;
                } else {
                    mState = 0x18;
                }
            }
            break;
        case 4:
            if (SpriteManager::Get().sFade[1] > 1) {
                break;
            }
            mState++;
            break;
        case 5:
            LoadWindowGraphics();
            new SaveWindow(1, 1, 0, 0x2000, 0, 0, 0, 0, 0);
            mContext->mRequest       = 0;
            data_020d433c.mUnk_14_1 = 0;
            SpriteManager::Get().StartSubFade(2);
            mState++;
            break;
        case 6:
            if (mContext->mRequest == 0) {
                break;
            }
            SpriteManager::Get().StartSubFade(3);
            mState++;
            break;
        case 7:
            if (SpriteManager::Get().sFade[1] > 1) {
                break;
            }
            ReloadMenu();
            mEvent->ClearSaveRequest();
            mTimer = 8;
            mState++;
            break;
        case 8:
            if (--mTimer != 0) {
                break;
            }
            if (!mEvent->IsRunning()) {
                if (HandleEventResult()) {
                    mState = 0x19;
                    break;
                }
                mUnk_be = 0;
                mState = 0x29;
            } else {
                SpriteManager::Get().StartFade(2);
                mState = 2;
            }
            break;
        case 9:
            if (SpriteManager::Get().sFade[1] > 1) {
                break;
            }
            new UnlockPopup(mEvent->GetUnlockPopupHandle() - 1);
            mContext->mRequest = 0;
            mState++;
            break;
        case 10:
            if (mContext->mRequest == 0) {
                break;
            }
            ReloadMenu();
            mEvent->ClearUnlockPopupHandle();
            mState++;
            break;
        case 11:
            mState = 2;
            break;
        case 12:
            if (SpriteManager::Get().IsFading()) {
                break;
            }
            mContext->mRequest = 0;
            UnlockStoreItems();
            if (UnlockCharacters()) {
                new UnlockPopup(0);
                mState++;
            } else if (UnlockFields()) {
                new UnlockPopup(1);
                mState++;
            } else {
                mState = 0x18;
            }
            break;
        case 13:
            if (mContext->mRequest != 0) {
                mState--;
            }
            break;
        case 14:
            if (SpriteManager::Get().IsFading()) {
                break;
            }
            if (mUnk_d2 != 0) {
                mUnk_d2 = 0;
                SpriteManager::Get().StartFade(3);
                mState = 0x12;
            } else {
                if (mEvent != NULL) {
                    mEvent->RequestFinish();
                    mEvent = NULL;
                }
                mState++;
            }
            if (mUnk_d3 != 0) {
                mUnk_d3 = 0;
                FrameTask::Get().StartChannel(0, 0x15);
            }
            break;
        case 15:
            InitScriptDisplay();
            mEvent = new ScriptPlayer(7, 0, 1);
            mState++;
            break;
        case 16:
            if (mEvent->func_0206f9b4("mes_Im_button", 0x15, 0, 6, 10, 4, 0)) {
                SpriteManager::Get().StartFade(2);
                mState++;
            }
            break;
        case 17:
            if (mEvent->IsRunning()) {
                break;
            }
            SpriteManager::Get().StartFade(3);
            mState++;
            break;
        case 18:
            if (SpriteManager::Get().IsFading()) {
                break;
            }
            func_ov002_0221e984();
            if (mEvent != NULL) {
                mEvent->RequestFinish();
                mEvent = NULL;
            }
            mUnk_b9 = 0;
            LoadMainGraphics(TRUE, TRUE);
            ReloadMenu();
            LoadWindow("/font/mes_tornament.mes", 0);
            mUnk_a0 = 0x100;
            mState++;
            break;
        case 19:
            mMenuWindow->Reset((s8) mSelection);
            PrintSubScreenText(sTournamentMenuSubTextIndices[mSelection]);
            SetupMainScreenText(sTournamentMenuMainTextIndices[mSelection]);
            mMenuVisible = 1;
            SpriteManager::Get().StartFade(2);
            mState = 0x15;
            break;
        case 20:
            mMenuWindow->mState = 4;
            mMenuWindow->Reset((s8) mSelection);
            RefreshCellLayout();
            mState++;
            break;
        case 21: {
            s16 a = mUnk_a0;
            if (a <= -0x20) {
                mUnk_a0 = a + 0x20;
            } else if (a >= 0x20) {
                mUnk_a0 = a - 0x20;
            } else {
                mUnk_a0 = 0;
            }
            u8 r = mMenuWindow->Update(TRUE);
            switch (r) {
                case 1:
                    switch (mSelection) {
                        case 0:
                            mBattleStep = 0;
                            PrintWindowText(4);
                            mFirstItemState = 0x17;
                            mOtherItemState = 0x14;
                            mState = 0x24;
                            break;
                        case 2:
                            PrintWindowText(3);
                            mFirstItemState = 0x27;
                            mOtherItemState = 0x14;
                            mState = 0x24;
                            break;
                        default:
                            mState = 0x18;
                            break;
                    }
                    break;
                case 2: {
                    CellAnimation *cell = mMenuWindow->mCells[mSelection];
                    cell->mFrameIndex       = cell->mFrames[cell->mAnimIndex].mFrameCount - 1;
                    mSelection          = mMenuWindow->mCursor->mSelection;
                    mState             = 0x18;
                    break;
                }
                default:
                    if (UpdateScrollInput(r)) {
                        mState++;
                    }
                    break;
            }
            break;
        }
        case 22:
            if (mMenuWindow->mState != 1) {
                mMenuWindow->Update(TRUE);
            }
            if (UpdateScroll()) {
                mState = 0x15;
            }
            break;
        case 23:
            if (func_ov002_0221e2b0()) {
                mState = 0x19;
            }
            break;
        case 24:
            SpriteManager::Get().StartFade(3);
            mState++;
            break;
        case 25:
            if (SpriteManager::Get().IsFading()) {
                break;
            }
            mMenuVisible = 0;
            mUnk_b9 = 0;
            mMainScreenVisible = 0;
            mCellsBVisible = 0;
            mWindowOpen = 0;
            if (mEvent != NULL) {
                mEvent->RequestFinish();
                mEvent            = NULL;
                mContext->mRequest = 0;
            } else {
                mContext->mRequest = 1;
            }
            mState++;
            break;
        case 26: {
            if (mContext->mRequest == 0) {
                break;
            }
            Cleanup();
            BOOL go = FALSE;
            switch (mSelection) {
                case 0:
                    go      = TRUE;
                    mUnk_be = 1;
                    break;
                case 1:
                    new TournamentSubScene();
                    break;
                case 5:
                    mUnk_be = 0;
                    go      = TRUE;
                    break;
            }
            if (go) {
                mState = 0x29;
                break;
            }
            mContext->mRequest = 0;
            mTimer            = 0x10;
            mState++;
            break;
        }
        case 27:
            if (--mTimer != 0) {
                break;
            }
            SpriteManager::Get().StartFade(2);
            mState++;
            break;
        case 28:
            if (mContext->mRequest == 0) {
                break;
            }
            mUnk_d2 = 1;
            mState = 0;
            break;
        case 29:
            if (SpriteManager::Get().IsFading()) {
                break;
            }
            mMode   = 0;
            mUnk_b9 = 1;
            LoadMainGraphics(FALSE, FALSE);
            LoadModeMenu();
            LoadWindow("/font/mes_tornament.mes", 2);
            mNextBgLayer = 1;
            SetupMainScreenText(sTournamentModeMainTextIndices[mMode]);
            PrintSubScreenText(sModeTables[0][mMode]);
            SpriteManager::Get().StartFade(2);
            mState = 0x1f;
            if (mUnk_d3 != 0) {
                mUnk_d3 = 0;
                FrameTask::Get().StartChannel(0, 0x15);
            }
            break;
        case 30:
            mMenuWindow->mState = 4;
            mMenuWindow->Reset((s8) mMode);
            mState++;
            break;
        case 31: {
            u8 prev = mMenuWindow->mCursor->mSelection;
            u8 r    = mMenuWindow->Update(TRUE);
            switch (r) {
                case 1:
                    mSelection = mMenuWindow->mCursor->mSelection;
                    mFirstItemState    = 0x22;
                    mOtherItemState    = 0x1e;
                    mState    = 0x24;
                    break;
                case 2:
                    mSelection = 5;
                    mState    = 0x18;
                    break;
                default: {
                    u32 cur = (u8) mMenuWindow->mCursor->mSelection;
                    if (cur <= 5) {
                        mSelection = cur;
                        if (prev != mSelection) {
                            StartBgBlend(mSelection);
                            mState++;
                        }
                    }
                    break;
                }
            }
            if (mMainTextLineCount > 3) {
                mMainTextTimer--;
                if (mMainTextTimer <= 0) {
                    mMainTextTimer = 200;
                    if (mMainTextPage < (mMainTextLineCount - 1) / 3) {
                        mMainTextPage++;
                        if (mMainTextPage >= (mMainTextLineCount - 1) / 3) {
                            mMainTextTimer = 300;
                        }
                    } else {
                        mMainTextPage = 0;
                    }
                    PrintMainScreenText(sTournamentModeMainTextIndices[mMode]);
                }
            }
            break;
        }
        case 32: {
            UpdateBgBlend();
            s16 a = mUnk_a0;
            if (a <= -0x120) {
                mUnk_a0 = a + 0x20;
                mUnk_a2 = mUnk_a0;
            } else if (a >= -0xe0) {
                mUnk_a0 = a - 0x20;
                mUnk_a2 = mUnk_a0;
            } else {
                mUnk_a0 = 0x100;
                u8 sel  = mSelection;
                mMode   = sel;
                if (mUnk_b9 != 0) {
                    if (sel == 1) {
                        sel = 0xd;
                    }
                }
                mCellsA[0]->SetAnimation(sel);
                CellAnimation *cell = mCellsA[0];
                cell->mFrameIndex       = cell->mFrames[cell->mAnimIndex].mFrameCount - 1;
                SetupMainScreenText(sTournamentModeMainTextIndices[mMode]);
                PrintSubScreenText(sModeTables[0][mMode]);
                mState++;
            }
            break;
        }
        case 33: {
            BOOL done = UpdateBgBlend();
            s16 a     = mUnk_a0;
            if (a <= -0x20) {
                mUnk_a0 = a + 0x20;
                mUnk_a2 = mUnk_a0;
            } else if (a >= 0x20) {
                mUnk_a0 = a - 0x20;
                mUnk_a2 = mUnk_a0;
            } else {
                mUnk_a0 = 0;
                if (done) {
                    mState = 0x1f;
                }
            }
            break;
        }
        case 34:
            SpriteManager::Get().StartFade(3);
            mState++;
            break;
        case 35:
            if (SpriteManager::Get().IsFading()) {
                break;
            }
            gSaveData[0].mUnk_42 = sModeTables[1][mMode];
            gSaveData[0].mUnk_43 = 0;
            gSaveData[0].mUnk_25 = MATH_Rand32(&gRandom, 5);
            mSelection = 0;
            mMenuVisible    = 0;
            mUnk_b9    = 0;
            mMainScreenVisible    = 0;
            mCellsBVisible    = 0;
            mWindowOpen    = 0;
            mUnk_d2    = 1;
            mState    = 0;
            break;
        case 36:
            OpenWindow();
            mState++;
            break;
        case 37:
            mWindow->mState = 4;
            mWindow->Reset(1);
            mState++;
            break;
        case 38:
            u8 r = mWindow->Update(TRUE);
            if (r == 1) {
                CloseWindow();
                if (mWindow->mCursor->mSelection == 0) {
                    mState = mFirstItemState;
                } else {
                    mState = mOtherItemState;
                }
            }
            break;
        case 39:
            SpriteManager::Get().StartFade(3);
            mState++;
            break;
        case 40:
            if (SpriteManager::Get().IsFading()) {
                break;
            }
            gSaveData[0].mUnk_42 = 10;
            gSaveData[0].mUnk_43 = 0;
            mMenuVisible                  = 0;
            mMainScreenVisible                  = 0;
            mCellsBVisible                  = 0;
            mWindowOpen                  = 0;
            mUnk_b9                  = 0;
            mState                  = 0x1d;
            break;
        case 41:
            if (mUnk_be != 0) {
                data_020d433c.mNextScene   = 0xd;
                data_020d433c.mUnk_9a   = 0x11;
                data_020d433c.mUnk_9a_0 = gSaveData[0].mUnk_28_6;
                if (data_020d433c.mUnk_8a_0 != 0) {
                    int idx; u8 i; u32 id = (u8) gSaveData[0].mAttribute; int slot; s8 pos;
                    slot = GetBakuganEntryIndex(gSaveData, 0, 0x25, id);
                    func_020231c0(gSaveData, slot, 0x29e);
                    idx = GetBakuganEntryIndex(gSaveData, 0, 0x24, id);
                    for (i = 0, pos = -1; i < 3; i++) {
                        if (idx == gSaveData[0].mUnk_00[i]) {
                            pos = i;
                            break;
                        }
                    }
                    if (pos > 0) {
                        do {
                            SetSlotEntryIndex(gSaveData, 0, (u8) pos, gSaveData[0].mUnk_00[pos - 1]);
                            pos--;
                        } while (pos >= 1);
                    }
                    SetSlotEntryIndex(gSaveData, 0, 0, slot);
                    data_020d4478[slot].mUnk_0 = 2;
                    UnkStruct_02023248 *entry           = GetBakuganEntry(gSaveData, 0x25, id);
                    entry->mUnk_0              = 3;
                    entry->mUnk_3              = 2;
                    entry->mUnk_6              = 3;
                    entry->mUnk_9              = 1;
                    entry->mUnk_12             = 2;
                    data_020d4850[GetBakuganEntryIndex(gSaveData, 0, 0x24, (u8) gSaveData[0].mAttribute)] = 0;
                }
                RequestScene(5);
                mFinished = TRUE;
                return;
            }
            if (mUnk_d0 != 0) {
                data_020d433c.mUnk_ad = 1;
                RequestScene(4);
                mFinished = TRUE;
                return;
            }
            RequestScene(9);
            mFinished = TRUE;
            return;
    }
    Draw();
}

void TournamentScene::InitScriptDisplay() {
    GX_SetDispSelect(1);
    SpriteManager::Get().SetupMode(1, 7);
    GX_SetGraphicsMode(1, 0, 1);
    GXS_SetGraphicsMode(0);
    G2S_SetBG0Control(0, 1, 0, 1, 0);
    G2_SetBG0Offset(0, 0);
    G2S_SetBG0Offset(0, 0);
    G2_SetBG0Priority(0);
    G2S_SetBG0Priority(3);
    GX_SetVisiblePlane(0x11);
    GXS_SetVisiblePlane(0x11);
    G2_SetBlendBrightness(0x1f, -16);
    G2S_SetBlendBrightness(0x1f, -16);
    GX_SetVisibleWnd(0);
    GXS_SetVisibleWnd(0);
    G2_SetWndOutsidePlane(0x1f, FALSE);
    G2S_SetWndOutsidePlane(0x1f, FALSE);
    G2_BlendNone();
    G2S_BlendNone();
}


void TournamentScene::func_ov002_0221b5b0() {
    GX_SetDispSelect(1);
    SpriteManager::Get().SetupMode(1, 1);
    GX_SetGraphicsMode(1, 0, 0);
    GXS_SetGraphicsMode(0);
    G2_SetBG0Control(0, 1, 0, 1, 0);
    G2_SetBG1Control(0, 1, 2, 4, 0);
    G2S_SetBG0Control(0, 1, 0, 1, 0);
    G2S_SetBG1Control(0, 1, 2, 5, 0);
    G2_SetBG0Offset(0, 0);
    G2_SetBG1Offset(0, 0);
    G2S_SetBG0Offset(0, 0);
    G2S_SetBG1Offset(0, 0);
    G2_SetBG0Priority(3);
    G2_SetBG1Priority(3);
    G2S_SetBG0Priority(3);
    G2S_SetBG1Priority(0);
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
}

void TournamentScene::LoadMainGraphics(BOOL a, BOOL b) {
    func_ov002_0221b5b0();
    MI_CpuClearFast((void *) 0x06200000, 0x20000);
    MI_CpuClearFast((void *) 0x06600000, 0x20000);
    ChangeDir("/game/Tournament/Bg/");
    if (b) {
        char name[0x40];
        sprintf(name, "tournament_u_bg_0%d_nsc.bin", sModeTables[2][mMode]);
        LoadAndQueueUpload(0, 7, 0, name, 0, 0, 0);
        sprintf(name, "tournament_u_bg_0%d_ncg.bin", sModeTables[2][mMode]);
        LoadAndQueueUpload(0, 0xb, 0, name, 0, 0, 0);
        sprintf(name, "tournament_u_bg_0%d_ncl.bin", sModeTables[2][mMode]);
        LoadAndQueueUpload(0, 1, 0, name, 0, 0, 0);
    } else {
        LoadAndQueueUpload(0, 7, 0, "tournament_u_bg_06_nsc.bin", 0, 0, 0);
        LoadAndQueueUpload(0, 0xb, 0, "tournament_u_bg_06_ncg.bin", 0, 0, 0);
        LoadAndQueueUpload(0, 1, 0, "tournament_u_bg_06_ncl.bin", 0, 0, 0);
    }
    u8 idx;
    int count = sCounts[mMode];
    int x     = gSaveData[0].mUnk_43;
    idx       = x * count;
    if (x >= sTotals[mMode] / count - 1) {
        x = 6;
    }
    ChangeDir("/Game/Tournament/Obj/");
    SpriteScreen &screen = SpriteManager::Get().Screen(0);
    screen.LoadCharacters("tournament_u_obj_00.nce_ncg1D.bin", 0, 0);
    screen.LoadPalette("tournament_u_obj_00_ncl.bin", 0, 0);
    screen.LoadCharacters("tournament_u_obj_01.nce_ncg1D.bin", 0x10000, 0);
    screen.LoadPalette("tournament_u_obj_01_ncl.bin", 0x20, 0);
    delete mCellsA[0];
    mCellsA[0]            = new CellAnimation("tournament_u_obj_01_nce.bin", mMode, 1, 1);
    mCellsA[0]->mCharBase = screen.GetCharBase(0x10000);
    mCellsA[0]->mPaletteBase   = 1;
    if (mUnk_b9 != 0 && mMode == 1) {
        mCellsA[0]->SetAnimation(0xd);
    }
    delete mCellsA[1];
    mCellsA[1]            = new CellAnimation("tournament_u_obj_00_nce.bin", (u8) x, 1, 1);
    mCellsA[1]->mCharBase = screen.GetCharBase(0);
    mCellsA[1]->mPaletteBase   = 0;
    mCellsA[1]->SetMode(5);
    delete mCellsA[3];
    u8 sel = a != 0 ? 6 : 8;
    mCellsA[3]            = new CellAnimation("tournament_u_obj_01_nce.bin", sel, 1, 1);
    mCellsA[3]->mCharBase = screen.GetCharBase(0x10000);
    mCellsA[3]->mPaletteBase   = 1;
    mCellsA[3]->SetMode(0);
    delete mCellsA[4];
    mCellsA[4]            = new CellAnimation("tournament_u_obj_01_nce.bin", 7, 1, 1);
    mCellsA[4]->mCharBase = screen.GetCharBase(0x10000);
    mCellsA[4]->mPaletteBase   = 1;
    mCellsA[4]->SetMode(0);
    delete mCellsB[0];
    mCellsB[0]            = new CellAnimation("tournament_u_obj_01_nce.bin", 0xa, 1, 1);
    mCellsB[0]->mCharBase = screen.GetCharBase(0x10000);
    mCellsB[0]->SetMode(0);
    delete mCellsB[1];
    mCellsB[1]            = new CellAnimation("tournament_u_obj_00_nce.bin", (u8) (x + 7), 1, 1);
    mCellsB[1]->mCharBase = screen.GetCharBase(0);
    mCellsB[1]->SetMode(5);
    delete mCellsB[3];
    mCellsB[3]            = new CellAnimation("tournament_u_obj_01_nce.bin", 0xb, 1, 1);
    mCellsB[3]->mCharBase = screen.GetCharBase(0x10000);
    mCellsB[3]->SetMode(0);
    ChangeDir("/");
    if (a) {
        delete mObj38;
        mObj38 = new CharacterIcons(0x19000, 7, 0);
        u8 i = 0;
        do {
            mObj38->SetCharacter(i, (s8) mCharacterIds[idx + i], 0, 0);
            mObj38->SetVisible(i, 1);
            mObj38->SetDrawArgs(i, sTournamentCharacterPos[mMode][idx + i].x, sTournamentCharacterPos[mMode][idx + i].y,
                                  0);
            i++;
        } while (i < sCounts[mMode]);
        if (mMode == 1) {
            mObj38->SetCharacter(3, (s8) mUnk_cb, 0, 0);
            mObj38->SetVisible(3, 1);
            mObj38->SetDrawArgs(3, 8, 0xa, 0);
        }
    }
    ChangeDir("/story/");
    screen.LoadCharacters("storytalk_ob_128k.nce_ncg1D.bin", 0x1a000, 0);
    screen.LoadPalette("storytalk_ob_ncl.bin", 0x160, 0);
    delete mCellsA[2];
    mCellsA[2]            = new CellAnimation("storytalk_ob_128k_nce.bin", 0, 1, 1);
    mCellsA[2]->mCharBase = screen.GetCharBase(0x1a000);
    mCellsA[2]->mPaletteBase   = 0xb;
    mCellsA[2]->SetMode(0);
    mMainScreenVisible = 1;
    ChangeDir("/");
}

void TournamentScene::LoadWindowGraphics() {
    MI_CpuClearFast((void *) 0x06200000, 0x20000);
    MI_CpuClearFast((void *) 0x06600000, 0x20000);
    G2S_SetBG0Control(0, 0, 0, 1, 0);
    G2S_SetBG1Control(0, 1, 2, 4, 0);
    GXS_SetVisiblePlane(0x13);
    ChangeDir("/Game/2D/union_b/");
    LoadAndQueueUpload(1, 1, 0, "union_b_bg_window_ncl.bin", 0, 0, 0);
    LoadAndQueueUpload(1, 11, 0, "union_b_bg_window_ncg.bin", 0, 0, 0);
    LoadAndQueueUpload(1, 7, 0, "union_b_bg_window_b_nsc.bin", 0, 0, 0);
    SpriteScreen &sub = SpriteManager::Get().Screen(1);
    ChangeDir("/Game/2D/union_b/");
    sub.LoadCharacters("union_b_obj_window.nce_ncg1D.bin", 0, 0);
    sub.LoadPalette("union_b_obj_window_ncl.bin", 0, 0);
    ChangeDir("/");
    mMenuVisible = 0;
}

void TournamentScene::ReloadMenu() {
    G2S_SetBG0Control(0, 1, 0, 1, 0);
    GXS_SetVisiblePlane(0x11);
    G2S_SetBG0Priority(3);
    GXS_SetVisibleWnd(0);
    G2S_BlendNone();
    LoadMenu();
}

void TournamentScene::LoadMenu() {
    ChangeDir("/Game/Tournament/bg/");
    LoadAndQueueUpload(1, 1, 0, "tournament_b_bg_00_ncl.bin", 0, 0, 0);
    LoadAndQueueUpload(1, 11, 0, "tournament_b_bg_00_ncg.bin", 0, 0, 0);
    LoadAndQueueUpload(1, 7, 0, "tournament_b_bg_00_nsc.bin", 0, 0, 0);
    SpriteScreen &sub = SpriteManager::Get().Screen(1);
    ChangeDir("/Game/Tournament/Obj/");
    sub.LoadCharacters("tournament_b_obj_00.nce_ncg1D.bin", 0, 0);
    sub.LoadPalette("tournament_b_obj_00_ncl.bin", 0, 0);
    ChangeDir("/Game/2D/union_b/");
    sub.LoadCharacters("union_b_obj_cockpit.nce_ncg1D.bin", 0x4000, 0);
    sub.LoadPalette("union_b_obj_cockpit_ncl.bin", 0x60, 0);
    sub.LoadCharacters("union_b_obj_arrow.nce_ncg1D.bin", 0x6000, 0);
    sub.LoadPalette("union_b_obj_arrow_ncl.bin", 0x80, 0);
    ChangeDir("/");
    LoadSubScreenText("/font/mes_tornament.mes", 1);
    delete mMessageWindow;
    mMessageWindow = new MessageWindow("mes_tornament", 1, 0x9000);
    mMessageWindow->func_02065bd4(0x14, 4, 0);
    mMessageWindow->SetWidth(0xd8);
    mMessageWindow->StartMessage(5, 0, 1);
    LoadMainScreenText("/font/mes_Im_button.mes");
    ClearSubScreenText();
    ClearMainScreenText();
    delete mMenuWindow;
    mMenuWindow = new SelectWindow(6, sTournamentMenuItems, 0);
    mMenuWindow->SetCells(6, sTournamentMenuCells);
    mMenuWindow->mCursor->mSelection = -1;
    RefreshCellLayout();
}

void TournamentScene::LoadModeMenu() {
    ChangeDir("/Game/Tournament/bg/");
    LoadAndQueueUpload(1, 1, 0, "tournament_b_bg_00_ncl.bin", 0, 0, 0);
    LoadAndQueueUpload(1, 11, 0, "tournament_b_bg_00_ncg.bin", 0, 0, 0);
    LoadAndQueueUpload(1, 7, 0, "tournament_b_bg_00_b_nsc.bin", 0, 0, 0);
    SpriteScreen &sub = SpriteManager::Get().Screen(1);
    ChangeDir("/Game/Tournament/Obj/");
    sub.LoadCharacters("tournament_b_obj_02.nce_ncg1D.bin", 0, 0);
    sub.LoadPalette("tournament_b_obj_02_ncl.bin", 0, 0);
    delete mCell20;
    mCell20            = new CellAnimation("tournament_b_obj_02_nce.bin", 0, 1, 1);
    mCell20->mCharBase = sub.GetCharBase(0);
    mCell20->mPaletteBase   = 0;
    ChangeDir("/Game/2D/union_b/");
    sub.LoadCharacters("union_b_obj_cockpit.nce_ncg1D.bin", 0x4000, 0);
    sub.LoadPalette("union_b_obj_cockpit_ncl.bin", 0x60, 0);
    LoadSubScreenText("/font/mes_town_info.mes", 2);
    delete mMessageWindow;
    mMessageWindow = new MessageWindow("mes_tornament", 1, 0x9000);
    mMessageWindow->func_02065bd4(0x14, 4, 0);
    mMessageWindow->SetWidth(0xd8);
    mMessageWindow->StartMessage(7, 0, 1);
    LoadMainScreenText("/font/mes_T_clear.mes");
    ChangeDir("/");
    delete mMenuWindow;
    mMenuWindow = new SelectWindow(7, sTournamentModeMenuItems, 0);
    mMenuWindow->SetCells(7, sTournamentModeMenuCells);
}

void TournamentScene::LoadMainScreenText(const char *mes) {
    for (u8 i = 2; i < 4; i++) {
        if (mMessages[i] != NULL) {
            OS_Free(mMessages[i]);
            mMessages[i] = NULL;
        }
        if (mTextBuffers[i] != NULL) {
            OS_Free(mTextBuffers[i]);
            mTextBuffers[i] = NULL;
        }
        if (mGlyphBuffers[i] != NULL) {
            OS_Free(mGlyphBuffers[i]);
            mGlyphBuffers[i] = NULL;
        }
    }
    mMessages[2]     = (MessageFile *) LoadFile(mes, 0, 0, 0, 0, 0);
    mTextBufferSizes[2]    = TextManager::Get().BufferSize(0x1c, 6);
    mGlyphCounts[2]  = TextManager::Get().GlyphCount(0x15);
    mTextBuffers[2]  = OS_Alloc(mTextBufferSizes[2]);
    mGlyphBuffers[2] = OS_Alloc(mGlyphCounts[2] * 0xc);
    TextManager::Get().Init(mGlyphBuffers[2], 0, 0, 0x1c, 6, 0x21);
    mTextBufferSizes[3]    = TextManager::Get().BufferSize(0x18, 2);
    mGlyphCounts[3]  = TextManager::Get().GlyphCount(6);
    mTextBuffers[3]  = OS_Alloc(mTextBufferSizes[3]);
    mGlyphBuffers[3] = OS_Alloc(mGlyphCounts[3] * 0xc);
    TextManager::Get().Init(mGlyphBuffers[3], 0, 0, 0x18, 2, 0x21);
    TextManager::Get().SetColor(0, 0, 0xf);
}


void TournamentScene::LoadSubScreenText(const char *mes, u8 rows) {
    for (u8 i = 0; i < 2; i++) {
        if (mMessages[i] != NULL) {
            OS_Free(mMessages[i]);
            mMessages[i] = NULL;
        }
        if (mTextBuffers[i] != NULL) {
            OS_Free(mTextBuffers[i]);
            mTextBuffers[i] = NULL;
        }
        if (mGlyphBuffers[i] != NULL) {
            OS_Free(mGlyphBuffers[i]);
            mGlyphBuffers[i] = NULL;
        }
    }
    mTextRows         = rows << 1;
    mMessages[0]    = (MessageFile *) LoadFile(mes, 0, 0, 0, 0, 0);
    mTextBufferSizes[0]   = (u16) TextManager::Get().BufferSize(0x20, mTextRows);
    mGlyphCounts[0] = (u16) TextManager::Get().GlyphCount((mTextRows * 8 + 15) / 16 * 8);
    mTextBuffers[0] = OS_Alloc(mTextBufferSizes[0]);
    mGlyphBuffers[0] = OS_Alloc(mGlyphCounts[0] * 0xc);
    TextManager::Get().Init(mGlyphBuffers[0], 1, 0, 0x20, mTextRows, 0x21);
    TextManager::Get().SetColor(1, 0, 0xf);
}

void TournamentScene::Draw() {
    if (mMenuVisible != 0) {
        mMenuWindow->Draw();
        if (mWindowOpen == 0) {
            mMessageWindow->Update();
        }
    } else if (mUnk_b9 != 0) {
        mMenuWindow->Draw();
        if (mWindowOpen == 0) {
            mMessageWindow->Update();
        }
        mCell20->Update();
        mCell20->Draw(1, sTournamentModeMenuItems[mMode].mX, sTournamentModeMenuItems[mMode].mY, 2);
    }
    if (mMenuVisible | mUnk_b9) {
        s16 a = 0x90;
        u8 b  = 8;
        if (mTextRows > 2) {
            b = 0x10;
            if ((int) mTextHeight <= 0xc) {
                a += 6;
            }
        }
        SpriteManager::Get().Screen(1).AddOam(mGlyphBuffers[0], b, (s16) (0x80 - ((u32) mTextWidth >> 1)), a, 2, 0x100,
                                                     0xf, 0);
    }
    if (mMainScreenVisible != 0) {
        s16 off = mUnk_a2;
        u8 idx  = gSaveData[0].mUnk_43 * sCounts[mMode];
        for (u8 i = 0; i < 3; i++) {
            if (i == 2) {
                off = 0;
            }
            mCellsA[i]->Update();
            mCellsA[i]->Draw(0, (s16) (off + sTournamentCellPos[i].x), sTournamentCellPos[i].y, data_ov002_02220a0c[i]);
        }
        if (mObj38 != NULL) {
            u8 j = 0;
            do {
                mObj38->SetDrawArgs(j, mUnk_a0 + (mMoveX + sTournamentCharacterPos[mMode][idx + j].x),
                                      mMoveY + sTournamentCharacterPos[mMode][idx + j].y, 0);
                j++;
            } while (j < sCounts[mMode]);
            if (mMode == 1) {
                mObj38->SetDrawArgs(3, mUnk_a2 + 8, 0xa, 1);
            }
            mObj38->DrawIcons();
        }
        u8 k = 0;
        if (mUnk_b9 != 0) {
            do {
                mCellsA[3]->Draw(0, (s16) (mUnk_a0 + sTournamentCharacterPos[mMode][k].x), sTournamentCharacterPos[mMode][k].y, 1);
                k++;
            } while (k < sTotals[mMode]);
        } else {
            do {
                s16 dx   = 0;
                s16 dy   = 0;
                u8 flag = 1;
                if (k >= idx) {
                    if (k < idx + sTotals[mMode]) {
                        dx   = mMoveX;
                        dy   = mMoveY;
                        flag = 0;
                    }
                }
                mCellsA[3]->DrawFrame(0, mCharacterIds[k], mUnk_a0 + (dx + sTournamentCharacterPos[mMode][k].x),
                                          dy + sTournamentCharacterPos[mMode][k].y, flag);
                k++;
            } while (k < sTotals[mMode]);
        }
        if (idx != 0) {
            u8 m = 0;
            do {
                mCellsA[4]->Draw(0, (s16) (mUnk_a0 + sTournamentCharacterPos[mMode][m].x), sTournamentCharacterPos[mMode][m].y, 0);
                m++;
            } while (m < idx);
        }
        if (mCellsBVisible != 0) {
            for (u8 n = 0; n < 3; n++) {
                if (mCellsB[n] != NULL) {
                    mCellsB[n]->Update();
                    mCellsB[n]->Draw(0, (s16) (mUnk_a2 + sTournamentCellPos[n].x), sTournamentCellPos[n].y, 0);
                }
            }
            if (mCellsB[3] != NULL) {
                u8 p = 0;
                do {
                    mCellsB[3]->Draw(0, (s16) (mUnk_a0 + (mMoveX + sTournamentCharacterPos[mMode][idx + p].x)),
                                     (s16) (mMoveY + sTournamentCharacterPos[mMode][idx + p].y), 0);
                    p++;
                } while (p < sCounts[mMode]);
            }
            if (mMode == 1) {
                mCellsB[3]->Draw(0, (s16) (mUnk_a2 + 8), 0xa, 0);
            }
        }
        u8 row;
        int base = 0x93;
        if (mUnk_ac < 0x23) {
            base = 0x96;
        }
        for (row = 0; row < 3; row++) {
            int y = row * 0xc + base;
            u8 col = 0;
            do {
                SpriteManager::Get().Screen(0).AddOam(mGlyphBuffers[2], 1, (s16) (col * 0x20 + 0x14), y, 0,
                                                             (u16) ((col + row * 7) * 2 + 0x3a0), 0xf, 0);
                col++;
            } while (col < 7);
        }
        SpriteManager::Get().Screen(0).AddOam(mGlyphBuffers[3], 6, 0x12, 0x82, 0, 0x3e0, 0xf, 0);
    }
    if (mWindowOpen != 0) {
        mWindow->Draw();
        SpriteManager::Get().Screen(1).AddOam(mGlyphBuffers[1], 9, 0x20, 0x28, 0, 0x360, 0xf, 0);
    }
}

u8 TournamentScene::UpdateScrollInput(u8 input) {
    mNewSelection = mSelection;
    u8 result     = 0;
    if (input == 3) {
        switch (mMenuWindow->GetSelectedId()) {
            case 3:
                if (mSelection != 0) {
                    result = 1;
                    mNewSelection--;
                }
                break;
            case 4:
                if (mSelection < mLastIndex + 1) {
                    result = 2;
                    mNewSelection++;
                }
                break;
        }
    }
    if (result == 0 && mMenuWindow->mState == 1) {
        u8 r = UpdateScrollKeys();
        switch (r) {
            case 1:
                mMenuWindow->SetResultItem(3, 3);
                break;
            case 2:
                mMenuWindow->SetResultItem(4, 3);
                break;
        }
    }
    if (result != 0) {
        u8 i              = 0;
        WindowItem *items = mMenuWindow->mCursor->mItems;
        do {
            if (mNewSelection == i) {
                mMenuWindow->mCells[i]->ResetAnimation();
                mMenuWindow->mCells[i]->SetAnimation(sTournamentMenuSelectedAnimIndices[i]);
            } else if (mSelection == i) {
                mMenuWindow->mCells[i]->SetMode(2);
            }
            items[i].mFlags = 0x8000;
            i++;
        } while (i <= mLastIndex + 1);
        if (mNewSelection == 0) {
            mMenuWindow->mButtons[3].mEnabled = 0;
            items[3].mFlags             = 0x8000;
        }
        if (mNewSelection == mLastIndex + 1) {
            mMenuWindow->mButtons[4].mEnabled = 0;
            items[4].mFlags             = 0x8000;
        }
        mScrollLeft = 0x62;
    }
    mDirection = result;
    return result;
}

u8 TournamentScene::UpdateScrollKeys() {
    u8 result = 0;
    if (InputManager::Get().IsTrigger(KEY_LEFT)) {
        if (mSelection != 0) {
            result = 1;
            mNewSelection--;
        }
    } else if (InputManager::Get().IsTrigger(KEY_RIGHT)) {
        if (mSelection < mLastIndex + 1) {
            result = 2;
            mNewSelection++;
        }
    }
    if (result != 0) {
        SoundManager::Get().StartSeqArc(0);
    }
    return result;
}

BOOL TournamentScene::HandleEventResult() {
    int r = mEvent->GetResult();
    if (r != 0) {
        switch (r) {
            case 5:
                mSelection = 0;
                break;
            case 8:
                mUnk_d0    = 1;
                mSelection = 5;
                break;
            default:
                mSelection = 5;
                break;
        }
        return TRUE;
    }
    return FALSE;
}

BOOL TournamentScene::UpdateScroll() {
    BOOL done = FALSE;
    s16 step  = 0xe;
    if (mScrollLeft < 0xe) {
        step = mScrollLeft;
    }
    mScrollLeft -= step;
    if (mDirection == 2) {
        step = -step;
    }
    u8 i              = 0;
    WindowItem *items = mMenuWindow->mCursor->mItems;
    do {
        Vec2s pos = mMenuWindow->GetCellPos(i);
        pos.x += step;
        mMenuWindow->SetCellPos(i, pos);
        items[i].mX = step + items[i].mX;
        i++;
    } while (i <= mLastIndex + 1);
    if (mScrollLeft == 0) {
        mMenuWindow->mCells[mSelection]->SetAnimation(sTournamentMenuAnimIndices[mSelection]);
        items[mNewSelection].mFlags = 5;
        items[mSelection].mFlags    = 0x8000;
        mMenuWindow->mCursor->mSelection  = mNewSelection;
        mSelection                  = mNewSelection;
        if (mSelection == 0) {
            mMenuWindow->mButtons[3].mEnabled = 0;
            items[3].mFlags             = 0x8000;
        } else {
            mMenuWindow->mButtons[3].mEnabled = 1;
            items[3].mFlags             = 8;
        }
        if (mSelection == mLastIndex + 1) {
            mMenuWindow->mButtons[4].mEnabled = 0;
            items[4].mFlags             = 0x8000;
        } else {
            mMenuWindow->mButtons[4].mEnabled = 1;
            items[4].mFlags             = 8;
        }
        mMenuWindow->mCells[4]->Rewind();
        mMenuWindow->mCells[3]->Rewind();
        if (mDirection == 1) {
            mMenuWindow->mCells[3]->SetAnimation(0);
        } else {
            mMenuWindow->mCells[4]->SetAnimation(1);
        }
        PrintSubScreenText(sTournamentMenuSubTextIndices[mSelection]);
        SetupMainScreenText(sTournamentMenuMainTextIndices[mSelection]);
        done = TRUE;
    }
    return done;
}


void TournamentScene::RefreshCellLayout() {
    WindowItem *items = mMenuWindow->mCursor->mItems;
    s16 base          = mSelection * 0x62;
    u8 i              = 0;
    do {
        Vec2s pos = mMenuWindow->GetCellPos(i);
        pos.x     = i * 0x62 + 0x58 - base;
        mMenuWindow->SetCellPos(i, pos);
        items[i].mX = pos.x;
        if (i == mSelection) {
            mMenuWindow->mCells[i]->SetAnimation(sTournamentMenuSelectedAnimIndices[i]);
            items[i].mFlags = 5;
        } else {
            mMenuWindow->mCells[i]->SetAnimation(sTournamentMenuAnimIndices[i]);
            items[i].mFlags = 0x8000;
        }
        mMenuWindow->mCells[i]->ResetAnimation();
        i++;
    } while (i <= mLastIndex + 1);
    u32 flags;
    if (mSelection == 0) {
        mMenuWindow->mButtons[3].mEnabled = 0;
        flags                       = 0x8000;
    } else {
        mMenuWindow->mButtons[3].mEnabled = 1;
        flags                       = 8;
    }
    items[3].mFlags = flags;
    if (mSelection == mLastIndex + 1) {
        mMenuWindow->mButtons[4].mEnabled = 0;
        flags                       = 0x8000;
    } else {
        mMenuWindow->mButtons[4].mEnabled = 1;
        flags                       = 8;
    }
    items[4].mFlags            = flags;
    mMenuWindow->mCursor->mSelection = mSelection;
}

void TournamentScene::PrintSubScreenText(u8 index) {
    ClearSubScreenText();
    TextManager::Get().mUse4bppFont = 1;
    TextManager &tm   = TextManager::Get();
    s16 width;
    s16 height;
    height            = 0;
    width             = 0;
    MessageFile *file = mMessages[0];
    tm.Print(file->GetText(index), file->GetLength(index), mTextBuffers[0], 0x20, mTextRows, &width, &height, &mTextWidth, 0, 2, 0x21, 0xc, 0, 0, 0, 1, 1);
    if (&mTextHeight != NULL) {
        mTextHeight = height;
    }
    TextManager::Get().mUse4bppFont = 0;
    QueueUpload(SpriteManager::Get().Screen(1).mBgIndex, 4, 0x8000, mTextBuffers[0], mTextBufferSizes[0], 0);
}

void TournamentScene::ClearSubScreenText() {
    for (u8 i = 0; i < 2; i++) {
        if (mTextBuffers[i] != NULL) {
            MI_CpuClear16(mTextBuffers[i], mTextBufferSizes[i]);
        }
    }
}


void TournamentScene::SetupMainScreenText(u8 index) {
    mMainTextTimer = 0xc8;
    mMainTextPage = 0;
    TextManager &tm = TextManager::Get();
    u16 pixelWidth;
    s16 width;
    s16 height;
    height          = 0;
    width           = 0;
    MessageFile *file;
    void *buffer = mTextBuffers[2];
    file         = mMessages[2];
    MI_CpuClear16(tm.mLineStarts, 0x20);
    tm.mLineStartCount = 1;
    tm.Print(file->GetText(index), file->GetLength(index), buffer, 0x1c, 6, &width, &height, &pixelWidth, 0, 2, 0x21, 0x10, 0, 1, 0, 1, 1);
    u32 h = (u16) height;
    for (u8 i = 1; i < 16; i++) {
        if (tm.mLineStarts[i] == 0) {
            tm.mLineStarts[i] = tm.mLineStarts[i - 1];
        }
    }
    mMainTextLineCount = h >> 4;
    PrintMainScreenText(index);
}

void TournamentScene::PrintMainScreenText(u32 index) {
    s16 height2;
    s16 width2;
    s16 height1;
    s16 width1;
    ClearMainScreenText();
    TextManager &text = TextManager::Get();
    width1            = -(mMainTextPage * 0x30);
    height1           = 0;
    MessageFile *file = mMessages[2];
    u16 length = file->GetLength(index);
    const u8 *message = file->GetText(index);
    void *buffer = mTextBuffers[2];
    u16 half = 0xffff;
    if (length < half * 2) {
        half = length / 2;
    }
    text.mLineStartCount = 0x10;
    text.Print(message, half * 2, buffer, 0x1c, 6, &height1, &width1, 0, 0, 2, 0x21, 0x10, 0, 0, 0,
               1, 1);
    if (&mUnk_ac != NULL) {
        mUnk_ac = width1;
    }
    QueueUpload(SpriteManager::Get().Screen(0).mBgIndex, 4, 0x1d000, mTextBuffers[2], mTextBufferSizes[2], 0);
    height2 = 0;
    width2  = 0;
    TextManager::Get().mUse4bppFont = 1;
    TextManager::Get().Print(data_020d441a, 0x18, mTextBuffers[3], 0x18, 2, &height2, &width2, 0, 0, 2, 0x21, 0, 0, 0, 0, 1, 1);
    TextManager::Get().mUse4bppFont = 0;
    QueueUpload(SpriteManager::Get().Screen(0).mBgIndex, 4, 0x1f000, mTextBuffers[3], mTextBufferSizes[3], 0);
}

void TournamentScene::ClearMainScreenText() {
    for (u8 i = 2; i < 4; i++) {
        if (mTextBuffers[i] != NULL) {
            MI_CpuClear16(mTextBuffers[i], mTextBufferSizes[i]);
        }
    }
}

BOOL TournamentScene::func_ov002_0221e2b0() {
    BOOL result = FALSE;
    switch (mBattleStep) {
        case 0:
            func_ov002_0221e8f0();
            mBattleStep++;
            break;
        case 1:
            if (mBrightness <= -16) {
                mBattleStep++;
            } else {
                mBrightness--;
                G2_SetBlendBrightness(0x11, mBrightness);
                G2S_SetBlendBrightness(0x11, mBrightness);
            }
            break;
        case 2: {
            u8 mode = mMode;
            u8 idx  = gSaveData[0].mUnk_43 * sCounts[mode];
            mMoveDistanceX = sTournamentMoveTargets[mode].x - (mUnk_a0 + sTournamentCharacterPos[mode][idx].x);
            mMoveDistanceY = sTournamentMoveTargets[mMode].y - sTournamentCharacterPos[mMode][idx].y;
            mMoveFrames = 0;
            mBattleStep++;
            break;
        }
        case 3:
            mMoveFrames++;
            mMoveX = mMoveDistanceX * mMoveFrames / 5;
            mMoveY = mMoveDistanceY * mMoveFrames / 5;
            if (mMoveFrames >= 5) {
                mCellsA[1]->SetMode(4);
                mCellsB[1]->SetMode(4);
                mBattleStep++;
            }
            break;
        case 4:
            if (mCellsA[1]->mFinished != 0) {
                mUnk_c1 = 0x3c;
                mBattleStep++;
            }
            break;
        case 5:
            if (mUnk_c1 != 0) {
                mUnk_c1--;
            } else {
                mBattleStep++;
            }
            break;
        case 6:
            if (mUnk_a0 <= 0xe0) {
                mUnk_a0 += 0x20;
            } else if (mUnk_a0 >= 0x120) {
                mUnk_a0 -= 0x20;
            } else {
                mUnk_a0 = 0x100;
                mBattleStep++;
            }
            mUnk_a2 = -mUnk_a0;
            break;
        case 7:
            SpriteManager::Get().StartFade(3);
            mBattleStep++;
            break;
        case 8:
            if (!SpriteManager::Get().IsFading()) {
                mMainScreenVisible = 0;
                mCellsBVisible = 0;
                G2_SetBlendBrightness(0x11, mBrightness);
                G2S_SetBlendBrightness(0x11, mBrightness);
                mBattleStep++;
            }
            break;
        case 9:
            InitScriptDisplay();
            if (mEvent == NULL) {
                mEvent = new ScriptPlayer(7, 0, 1);
            }
            mUnk_c1 = 0;
            mBattleStep++;
            break;
        case 10:
            if (!mEvent->IsRunning()) {
                u8 mode = mMode;
                u8 idx = gSaveData[0].mUnk_43 * sCounts[mode];
                u32 pick = MATH_Rand32(&gRandom, 3);
                u8 id   = mCharacterIds[mUnk_c1 + idx];
                if (mEvent->func_0206f9b4("mes_tm_battle", id, data_ov002_02220a54[0][id], data_ov002_02220a6c[id][pick], 2, 3,
                                  0)) {
                    SpriteManager::Get().StartFade(2);
                    mBattleStep++;
                }
            }
            break;
        case 11:
            if (!mEvent->IsRunning()) {
                mUnk_c1++;
                if (mUnk_c1 < sCounts[mMode]) {
                    mBattleStep = 10;
                } else {
                    mBattleStep++;
                }
            }
            break;
        case 12:
            SpriteManager::Get().StartFade(3);
            mBattleStep++;
            break;
        case 13:
            if (!SpriteManager::Get().IsFading()) {
                result = TRUE;
            }
            break;
    }
    return result;
}

void TournamentScene::func_ov002_0221e8f0() {
    GX_SetVisibleWnd(4);
    G2_SetWndOutsidePlane(0x31, FALSE);
    reg_G2_WINOUT  = (reg_G2_WINOUT & ~0x3f00) | 0x1100;
    mBrightness        = 0;
    mCellsBVisible        = 1;
    if (mMode == 1) {
        mCellsB[0]->SetAnimation(12);
        mCellsB[0]->SetMode(0);
    } else {
        mCellsB[0]->SetAnimation(10);
        mCellsB[0]->SetMode(0);
    }
}

void TournamentScene::func_ov002_0221e984() {
    switch (gSaveData[0].mUnk_42) {
        case 1:
            mMode = 0;
            break;
        case 3:
            mMode = 1;
            break;
        case 6:
            mMode = 2;
            break;
        case 7:
            mMode = 3;
            break;
        case 8:
            mMode = 4;
            break;
        case 9:
            mMode = 5;
            break;
        default:
            mMode = 0;
            break;
    }
    if (mEvent == NULL) {
        return;
    }
    u8 a = 0;
    u8 b = 0;
    u8 c = 1;
    if (mMode == 1) {
        c       = 2;
        mUnk_cb = mEvent->func_02067120(gSaveData[0].mUnk_42, gSaveData[0].mUnk_43, 1, 0);
    }
    u8 i = 0;
    do {
        mCharacterIds[i] = mEvent->func_02067120(gSaveData[0].mUnk_42, a, (u8) (b + c), 0);
        b++;
        if (b >= sCounts[mMode]) {
            a++;
            b = 0;
        }
        i++;
    } while (i < sTotals[mMode]);
}

void TournamentScene::OpenWindow() {
    GXS_SetVisiblePlane(0x13);
    mWindow->Reset(1);
    mWindow->mEnabled = 1;
    mWindowOpen           = 1;
    G2S_SetWnd0Position(0x10, 0x10, 0xf0, 0xb0);
    GXS_SetVisibleWnd(1);
    G2S_SetWnd0InsidePlane(0x13, FALSE);
    G2S_SetWndOutsidePlane(0x33, FALSE);
    G2S_SetBlendBrightness(0x11, -8);
}

void TournamentScene::CloseWindow() {
    GXS_SetVisiblePlane(0x11);
    mWindow->mEnabled = 0;
    mWindowOpen           = 0;
    G2S_SetBlendBrightness(0x11, 0);
    GXS_SetVisibleWnd(0);
    G2S_SetWnd0InsidePlane(0x1f, FALSE);
    G2S_SetWndOutsidePlane(0x3f, FALSE);
}

void TournamentScene::LoadWindow(const char *path, u32 index) {
    ChangeDir("/Game/2D/union_b/");
    LoadAndQueueUpload(1, 8, 0, "union_b_bg_window_b_nsc.bin", 0, 0, 0);
    LoadAndQueueUpload(1, 12, 0, "union_b_bg_window_ncg.bin", 0, 0, 0);
    LoadAndQueueUpload(1, 1, 0x2000, "union_b_bg_window_ncl.bin", 0, 0, 0);
    SpriteScreen &screen = SpriteManager::Get().Screen(1);
    ChangeDir("/Game/2D/union_b/");
    screen.LoadCharacters("union_b_obj_window.nce_ncg1D.bin", 0x10000, 0);
    screen.LoadPalette("union_b_obj_window_ncl.bin", 0x1a0, 0);
    if (mMessages[1] != NULL) {
        OS_Free(mMessages[1]);
    }
    if (mTextBuffers[1] != NULL) {
        OS_Free(mTextBuffers[1]);
    }
    if (mGlyphBuffers[1] != NULL) {
        OS_Free(mGlyphBuffers[1]);
    }
    mMessages[1] = (MessageFile *) LoadFile(path, 0, 0, 0, 0, 0);
    mTextBufferSizes[1] = TextManager::Get().BufferSize(0x18, 0x10);
    mGlyphCounts[1]     = TextManager::Get().GlyphCount(0xc);
    mTextBuffers[1]     = OS_Alloc(mTextBufferSizes[1]);
    mGlyphBuffers[1]    = OS_Alloc(mGlyphCounts[1] * 0xc);
    TextManager::Get().Init(mGlyphBuffers[1], 1, 0, 0x18, 0x10, 0x42);
    PrintWindowText(index);
    delete mWindow;
    mWindow = new SelectWindow(3, sWindowItems, 1);
    mWindow->SetCells(3, sWindowCells);
    mWindow->mEnabled = 0;
    ChangeDir("/");
}

void TournamentScene::PrintWindowText(u32 index) {
    MI_CpuClear16(mTextBuffers[1], mTextBufferSizes[1]);
    TextManager &text = TextManager::Get();
    s16 height;
    s16 width;
    width  = 0;
    height = 0;
    MessageFile *file = mMessages[1];
    text.Print(file->GetText(index), file->GetLength(index), mTextBuffers[1], 0x18, 0x10, &height, &width, 0, 0, 2, 0x42, 0x10, 0, 0, 0, 1, 1);
    QueueUpload(SpriteManager::Get().Screen(1).mBgIndex, 4, 0x1b000, mTextBuffers[1], mTextBufferSizes[1], 0);
}

void TournamentScene::StartBgBlend(u32 index) {
    ChangeDir("/game/Tournament/Bg/");
    index      = sModeTables[2][index];
    u32 nsc    = 7;
    u32 ncg    = 0xb;
    u32 offset = 0;
    if (mNextBgLayer != 0) {
        nsc    = 8;
        ncg    = 0xc;
        offset = 0x2000;
    }
    char path[64];
    sprintf(path, "tournament_u_bg_0%d_nsc.bin", index);
    LoadAndQueueUpload(0, nsc, 0, path, 0, 0, 0);
    sprintf(path, "tournament_u_bg_0%d_ncg.bin", index);
    LoadAndQueueUpload(0, ncg, 0, path, 0, 0, 0);
    sprintf(path, "tournament_u_bg_0%d_ncl.bin", index);
    LoadAndQueueUpload(0, 1, offset, path, 0, 0, 0);
    mBlend = 0;
    if (mNextBgLayer == 0) {
        G2_SetBG0Priority(2);
        G2_SetBG1Priority(3);
        G2x_SetBlendAlpha_(&reg_G2_BLDCNT, 1, 2, mBlend, 0x1f - mBlend);
        mNextBgLayer = 1;
    } else {
        G2_SetBG0Priority(3);
        G2_SetBG1Priority(2);
        G2x_SetBlendAlpha_(&reg_G2_BLDCNT, 2, 1, mBlend, 0x1f - mBlend);
        mNextBgLayer = 0;
    }
    GX_SetVisiblePlane(0x13);
}

BOOL TournamentScene::UpdateBgBlend() {
    BOOL done = TRUE;
    if (mBlend < 0x1f) {
        mBlend++;
        u8 level = mBlend;
        reg_G2_BLDALPHA = level | ((0x1f - level) << 8);
        done = FALSE;
        if (mBlend >= 0x1f) {
            if (mNextBgLayer == 0) {
                GX_SetVisiblePlane(0x12);
            } else {
                GX_SetVisiblePlane(0x11);
            }
            G2_BlendNone();
            mBlend        = 0x1f;
        }
    }
    return done;
}
