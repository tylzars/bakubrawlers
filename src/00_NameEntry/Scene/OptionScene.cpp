#include "Graphics/SpriteManager.hpp"
#include "Scene/OptionScene.hpp"
#include "Game/SaveData.hpp"
#include "Graphics/CellAnimation.hpp"
#include "Scene/CreditsScene.hpp"
#include "UI/SaveWindow.hpp"
#include "UI/SelectWindow.hpp"
#include <nitro/mi.h>
#include <nitro/os.h>
#include "Graphics/Display.hpp"
#include "System/FileSystem.hpp"
#include <nitro/gx.h>

static const WindowItemRecord sOptionMenu3Items[3] = {
    {0, 0x1, 45, 136, 78, 20, 0x0, {-1, -1, 1, 1}, 0, WINDOW_TOUCH_RELEASE_IF_SELECTED},
    {1, 0x1, 133, 136, 78, 20, 0x0, {-1, -1, 0, 0}, 0, WINDOW_TOUCH_RELEASE_IF_SELECTED},
    {2, WINDOW_ITEM_CURSOR_SKIP, 120, 35, 0, 0, 0x0, {-1, -1, -1, -1}, 0, 0},
};
static const WindowCellRecord sOptionMainMenuCells[6] = {
    {"option_b_obj_00_nce.bin", 3, 0x0, 0, 0, 1, {3, 5, 4, 0}},
    {"option_b_obj_00_nce.bin", 6, 0x0, 0, 0, 1, {6, 8, 7, 0}},
    {"option_b_obj_00_nce.bin", 9, 0x0, 0, 0, 1, {9, 11, 10, 0}},
    {"option_b_obj_00_nce.bin", 12, 0x0, 0, 0, 1, {12, 14, 13, 0}},
    {"option_b_obj_00_nce.bin", 15, 0x0, 0, 0, 1, {15, 17, 16, 0}},
    {"/Game/2D/union_b/union_b_obj_cockpit_nce.bin", 2, 0xb000, 9, 0, 1, {2, 3, 3, 0}},
};
// [0..2] the cells; [3] the sub BG1 / BG2 offset (SlideStep then gives BG2 an x offset 0x20 below BG1's).
static const Vec2s sOptionMenu3StartPos[4] = {
    {301, 136}, {389, 136}, {376, 35}, {0, 0},
};
static const Vec2s sOptionMenu3SlidePerFrame[4] = {
    {-32, 0}, {-32, 0}, {-32, 0}, {32, 0},
};
static const u8 sOptionSettingsRowY[4] = {0x34, 0x54, 0x74, 0x0};
static const WindowItemRecord sOptionSettingsItems[2] = {
    {0, 0x5, 169, 170, 52, 20, 0x201, {-1, -1, -1, -1}, 0, WINDOW_TOUCH_RELEASE},
    {1, 0x2, 225, 170, 30, 20, 0x201, {-1, -1, -1, -1}, 0, WINDOW_TOUCH_RELEASE},
};
static const WindowCellRecord sOptionSettingsCells[2] = {
    {"/Game/2D/union_b/union_b_obj_cockpit_nce.bin", 0, 0xb000, 9, 0, 1, {0, 1, 1, 0}},
    {"/Game/2D/union_b/union_b_obj_cockpit_nce.bin", 2, 0xb000, 9, 0, 1, {2, 3, 3, 0}},
};
static const WindowCellRecord sOptionMenu2Cells[4] = {
    {"option_b_obj_00_nce.bin", 24, 0x0, 0, 0, 1, {24, 26, 25, 0}},
    {"option_b_obj_00_nce.bin", 27, 0x0, 0, 0, 1, {27, 29, 28, 0}},
    {"/Game/2D/union_b/union_b_obj_cockpit_nce.bin", 0, 0xb000, 9, 0, 1, {0, 1, 1, 0}},
    {"/Game/2D/union_b/union_b_obj_cockpit_nce.bin", 2, 0xb000, 9, 0, 1, {2, 3, 3, 0}},
};
static const u8 sOptionSettingsColumnX[6] = {0x60, 0x7c, 0x98, 0xb4, 0xd0, 0xec};
static const Vec2s sOptionSettingsSlidePerFrame[5] = {
    {0, 2}, {0, 0}, {-32, 0}, {-32, 0}, {-32, 0},
};
static const Vec2s sOptionSettingsStartPos[5] = {
    {128, 80}, {256, 0}, {256, 52}, {256, 84}, {256, 116},
};
// [0..4] the five cells; [5..7] the animation of row mRow while selected (BuildSettings sets [5]'s 0x13 directly).
static const u8 sOptionSettingsAnimIndices[8] = {0x0, 0x24, 0x12, 0x14, 0x16, 0x13, 0x15, 0x17};
static const WindowItemRecord sOptionMenu4Items[4] = {
    {0, 0x1, 48, 64, 160, 24, 0x303, {1, 1, -1, -1}, 0, WINDOW_TOUCH_RELEASE_IF_SELECTED},
    {1, 0x1, 48, 96, 160, 24, 0x303, {0, 0, -1, -1}, 0, WINDOW_TOUCH_RELEASE_IF_SELECTED},
    {2, 0x1, 169, 170, 52, 20, 0x201, {-1, -1, -1, -1}, 0, WINDOW_TOUCH_RELEASE},
    {3, 0x2, 225, 170, 30, 20, 0x201, {-1, -1, -1, -1}, 0, WINDOW_TOUCH_RELEASE},
};
static const WindowCellRecord sOptionMenu3Cells[3] = {
    {"/Game/2D/union_b/union_b_obj_window_nce.bin", 3, 0xd000, 10, 0, 1, {3, 5, 4, 0}},
    {"/Game/2D/union_b/union_b_obj_window_nce.bin", 7, 0xd000, 10, 0, 1, {6, 8, 7, 0}},
    {"/Game/2D/union_b/union_b_obj_window_nce.bin", 10, 0xd000, 10, 0, 1, {10, 10, 10, 0}},
};
static const WindowItemRecord sOptionMainMenuItems[6] = {
    {0, 0x1, 48, 38, 160, 24, 0x303, {4, 1, -1, -1}, 0, WINDOW_TOUCH_RELEASE_IF_SELECTED},
    {1, 0x1, 48, 62, 160, 24, 0x303, {0, 2, -1, -1}, 0, WINDOW_TOUCH_RELEASE_IF_SELECTED},
    {2, 0x1, 48, 86, 160, 24, 0x303, {1, 3, -1, -1}, 0, WINDOW_TOUCH_RELEASE_IF_SELECTED},
    {3, 0x1, 48, 110, 160, 24, 0x303, {2, 4, -1, -1}, 0, WINDOW_TOUCH_RELEASE_IF_SELECTED},
    {4, 0x1, 48, 134, 160, 24, 0x303, {3, 0, -1, -1}, 0, WINDOW_TOUCH_RELEASE_IF_SELECTED},
    {5, 0x2, 225, 170, 30, 20, 0x201, {-1, -1, -1, -1}, 0, WINDOW_TOUCH_RELEASE},
};
static const WindowCellRecord sOptionMenu4Cells[4] = {
    {"option_b_obj_00_nce.bin", 30, 0x0, 0, 0, 1, {30, 32, 31, 0}},
    {"option_b_obj_00_nce.bin", 33, 0x0, 0, 0, 1, {33, 35, 34, 0}},
    {"/Game/2D/union_b/union_b_obj_cockpit_nce.bin", 0, 0xb000, 9, 0, 1, {0, 1, 1, 0}},
    {"/Game/2D/union_b/union_b_obj_cockpit_nce.bin", 2, 0xb000, 9, 0, 1, {2, 3, 3, 0}},
};
static const WindowItemRecord sOptionMenu2Items[4] = {
    {0, 0x1, 48, 64, 160, 24, 0x303, {1, 1, -1, -1}, 0, WINDOW_TOUCH_RELEASE_IF_SELECTED},
    {1, 0x1, 48, 96, 160, 24, 0x303, {0, 0, -1, -1}, 0, WINDOW_TOUCH_RELEASE_IF_SELECTED},
    {2, 0x1, 169, 170, 52, 20, 0x201, {-1, -1, -1, -1}, 0, WINDOW_TOUCH_RELEASE},
    {3, 0x2, 225, 170, 30, 20, 0x201, {-1, -1, -1, -1}, 0, WINDOW_TOUCH_RELEASE},
};
#include "System/TouchManager.hpp"
#include "Text/TextManager.hpp"
#include "System/InputManager.hpp"
#include "System/SoundManager.hpp"
#include "System/FrameTask.hpp"


OptionScene::OptionScene() :
    SceneBase(2, 0) {
    mWindow = NULL;
    mSelectedId = 0;
    mFile = NULL;
    mBuf0c = NULL;
    MI_CpuClear32(mCellAnims, sizeof(mCellAnims));
    mState = 0;
    mSaved = *(u16 *) ((u8 *) gSaveData + 0x28);
}

OptionScene::~OptionScene() {
    Release();
}

void ResetOptions(SaveData *d);
void ApplyVolumeOptions(SaveData *d);

void OptionScene::Release() {
    if (mFile != NULL) {
        OS_Free(mFile);
        mFile = NULL;
    }
    if (mBuf0c != NULL) {
        OS_Free(mBuf0c);
        mBuf0c = NULL;
    }
    CellAnimation *cell;
    u8 i;
    for (i = 0; i < 5; i++) {
        cell = mCellAnims[i];
        if (cell != NULL) {
            delete cell;
        }
        mCellAnims[i] = NULL;
    }
    if (mWindow != NULL) {
        delete mWindow;
    }
    mWindow = NULL;
}

void OptionScene::Update() {
    switch (mState) {
        case 0:
            Init();
            mState++;
            break;
        case 1:
            BuildMainMenu();
            SpriteManager::Get().StartFade(FADE_MODE_TO_ZERO);
            mState++;
            break;
        case 2: {
            if (SpriteManager::Get().IsFading()) {
                break;
            }
            BOOL result = mWindow->Update(1);
            switch (result) {
                case 1:
                    mSelectedId = mWindow->GetSelectedId();
                    switch ((u8) mSelectedId) {
                        case OPTION_MENU_VOLUME:
                            mState = 3;
                            break;
                        case 1:
                            mState = 7;
                            break;
                        case 2:
                            mState = 0x11;
                            break;
                        case OPTION_MENU_RESET:
                            mState = 0xb;
                            break;
                        case OPTION_MENU_CREDITS:
                            mState = 0xe;
                            break;
                    }
                    break;
                case 2:
                    mState = 0x15;
                    break;
            }
            break;
        }
        case 3:
            mWindow->Update(1);
            if (mWindow->mState == 4) {
                BuildSettings();
                mState++;
            }
            break;
        case 4: {
            UpdateSettings();
            DrawSettings();
            BOOL result = mWindow->Update(1);
            switch (result) {
                case 1:
                    mState++;
                    break;
                case 2: {
                    SaveData *d = gSaveData;
                    d->mVolume0Index = mOld[0];
                    d->mVolume1Index = mOld[1];
                    d->mVolume2Index = mOld[2];
                    ApplyVolumeOptions(d);
                    mState++;
                    break;
                }
            }
            break;
        }
        case 5:
            DrawSettings();
            mSlideTimer = 8;
            mState++;
            break;
        case 6:
            mPosY[0] -= 4;
            DrawSettings();
            if (mSlideTimer == 0) {
                StopSeqArc(&SoundManager::Get(), 5, 1);
                FrameTask::Get().StopChannel(1);
                mState = 1;
            }
            break;
        case 7:
            mWindow->Update(1);
            if (mWindow->mState == 4) {
                BuildMenu2();
                mState++;
            }
            break;
        case 8: {
            BOOL result = mWindow->Update(1);
            DrawMenu2();
            switch (result) {
                case 1:
                    gSaveData[0].mMicEnabled = mFlagSelectedId == 0 ? 1 : 0;
                    gSaveData[0].mUnk_27_0 = gSaveData[0].mMicEnabled;
                    mState++;
                    break;
                case 2:
                    mState++;
                    break;
                default:
                    if (mWindow->GetSelectedId() < 2) {
                        mFlagSelectedId = mWindow->GetSelectedId();
                    }
                    break;
            }
            break;
        }
        case 9:
            DrawMenu2();
            mSlideTimer = 8;
            mState++;
            break;
        case 10:
            mPosY[0] -= 4;
            DrawMenu2();
            if (mSlideTimer == 0) {
                mState = 1;
            }
            break;
        case 11:
            mWindow->Update(1);
            if (mWindow->mState == 4) {
                BuildMenu3();
                mState++;
            }
            break;
        case 12: {
            SlideStep();
            BOOL result = mWindow->Update(1);
            if (result == 1) {
                s8 id = mWindow->GetSelectedId();
                switch (id) {
                    case 0:
                        ResetOptions(gSaveData);
                        break;
                    case 1:
                        break;
                }
                mState++;
                mSlideTimer = 8;
            }
            break;
        }
        case 13:
            SlideStep();
            if (mSlideTimer == 0) {
                ResetDisplay();
                mState = 1;
            }
            break;
        case 17:
            mWindow->Update(1);
            if (mWindow->mState == 4) {
                BuildMenu4();
                mState++;
            }
            break;
        case 18: {
            BOOL result = mWindow->Update(1);
            DrawMenu4();
            switch (result) {
                case 1:
                    gSaveData[0].mUnk_28_5 = mFlagSelectedId == 0 ? 1 : 0;
                    mState++;
                    break;
                case 2:
                    mState++;
                    break;
                default:
                    if (mWindow->GetSelectedId() < 2) {
                        mFlagSelectedId = mWindow->GetSelectedId();
                    }
                    break;
            }
            break;
        }
        case 19:
            DrawMenu4();
            mSlideTimer = 8;
            mState++;
            break;
        case 20:
            mPosY[0] -= 4;
            DrawMenu4();
            if (mSlideTimer == 0) {
                mState = 1;
            }
            break;
        case 14:
            SpriteManager::Get().StartFade(FADE_MODE_DOWN);
            mState++;
            break;
        case 15:
            if (SpriteManager::Get().IsFading()) {
                break;
            }
            Release();
            new CreditsScene();
            mContext->mRequest = 0;
            mState++;
            break;
        case 16:
            if (mContext->mRequest == 0) {
                break;
            }
            FrameTask::Get().StartChannel(0, 0x10);
            mState = 0;
            break;
        case 21:
            mWindow->Update(1);
            if (mWindow->mState == 4) {
                if (mSaved == *(u16 *) ((u8 *) gSaveData + 0x28)) {
                    SpriteManager::Get().StartFade(FADE_MODE_DOWN);
                    mState = 0x17;
                } else {
                    GX_SetVisiblePlane(0x11);
                    new SaveWindow(1, 1, 0, 0x2000, 2, 0, 0, 0xd000, 0x120);
                    mContext->mRequest = 0;
                    mState++;
                }
            }
            break;
        case 22:
            if (mContext->mRequest != 0) {
                mState = 0x17;
            }
            break;
        case 23:
            if (SpriteManager::Get().IsFading()) {
                break;
            }
            RequestScene(1);
            mFinished = 1;
            return;
    }
    if (mWindow != NULL) {
        mWindow->Draw();
    }
}

void OptionScene::Init() {
    GX_SetDispSelect(1);
    SpriteManager::Get().SetupMode(1, 1);
    GX_SetGraphicsMode(1, 0, 0);
    GXS_SetGraphicsMode(0);
    G2_SetBG0Control(0, 1, 0, 1, 0);
    G2_SetBG1Control(0, 0, 1, 4, 0);
    G2S_SetBG0Control(0, 1, 0, 1, 0);
    G2S_SetBG1Control(1, 1, 1, 4, 0);
    G2S_SetBG2Control(1, 0, 3, 6, 0);
    G2_SetBG0Offset(0, 0);
    G2S_SetBG0Offset(0, 0);
    G2S_SetBG1Offset(0, 0);
    G2S_SetBG2Offset(0, 0);
    G2_SetBG0Priority(3);
    G2_SetBG1Priority(2);
    G2S_SetBG0Priority(3);
    G2S_SetBG1Priority(2);
    G2S_SetBG2Priority(1);
    GX_SetVisiblePlane(0x13);
    GXS_SetVisiblePlane(0x11);
    GX_SetVisibleWnd(0);
    GXS_SetVisibleWnd(0);
    G2_BlendNone();
    G2S_BlendNone();
    ChangeDir("/Game/Title/");
    LoadAndQueueUpload(0, UPLOAD_KIND_BG_EXT_PLTT, 0, "option_u_bg_00_ncl.bin", 0, 0, 0);
    LoadAndQueueUpload(0, UPLOAD_KIND_BG0_CHAR, 0, "option_u_bg_00_ncg.bin", 0, 0, 0);
    LoadAndQueueUpload(0, UPLOAD_KIND_BG0_SCR, 0, "option_u_bg_00_nsc.bin", 0, 0, 0);
    LoadAndQueueUpload(1, UPLOAD_KIND_BG_EXT_PLTT, 0, "option_b_bg_00_ncl.bin", 0, 0, 0);
    LoadAndQueueUpload(1, UPLOAD_KIND_BG0_CHAR, 0, "option_b_bg_00_ncg.bin", 0, 0, 0);
    LoadAndQueueUpload(1, UPLOAD_KIND_BG0_SCR, 0, "option_b_bg_00_nsc.bin", 0, 0, 0);
    LoadAndQueueUpload(1, UPLOAD_KIND_BG_EXT_PLTT, 0x2000, "/Game/2D/union_b/union_b_bg_window_ncl.bin", 0, 0, 0);
    LoadAndQueueUpload(1, UPLOAD_KIND_BG1_CHAR, 0, "/Game/2D/union_b/union_b_bg_window_ncg.bin", 0, 0, 0);
    LoadAndQueueUpload(1, UPLOAD_KIND_BG1_SCR, 0x800, "/Game/2D/union_b/union_b_bg_window_a_nsc.bin", 0, 0, 0);
    SpriteScreen &screen = SpriteManager::Get().Screen(1);
    screen.LoadCharacters("option_b_obj_00.nce_ncg1D.bin", 0, 0);
    screen.LoadPalette("option_b_obj_00_ncl.bin", 0, 0);
    screen.LoadCharacters("/Game/2D/union_b/union_b_obj_cockpit.nce_ncg1D.bin", 0xb000, 0);
    screen.LoadPalette("/Game/2D/union_b/union_b_obj_cockpit_ncl.bin", 0x120, 0);
    screen.LoadCharacters("/Game/2D/union_b/union_b_obj_window.nce_ncg1D.bin", 0xd000, 0);
    screen.LoadPalette("/Game/2D/union_b/union_b_obj_window_ncl.bin", 0x140, 0);
    TextManager::Get().func_0202ce44(0, 0, 0, 0);
    TextManager::Get().func_0202ce44(1, 0, 0, 0);
    mFile = (MessageFile *) LoadFile("/font/mes_option.mes", 0, 0, 0, 0, 0);
    mBufSize = TextManager::Get().BufferSize(0x18, 0x20);
    mBuf0c = OS_Alloc(mBufSize);
    u16 mapSize = TextManager::Get().BufferSize(0x18, 2);
    u16 *map = (u16 *) OS_Alloc(mapSize);
    MI_CpuClearFast(map, mapSize);
    TextManager::Get().FillScrChars(0, map, 0, 0, 0x18, 0x18, 0x20);
    QueueUpload(0, UPLOAD_KIND_BG1_SCR, 0, map, mapSize, 0);
    QueueUpload(1, UPLOAD_KIND_BG2_SCR, 0x800, map, mapSize, 1);
}

void OptionScene::BuildMainMenu() {
    PrintMessage(0);
    delete mWindow;
    mWindow = new SelectWindow(6, sOptionMainMenuItems, mSelectedId);
    mWindow->SetCells(6, sOptionMainMenuCells);
    mWindow->Update(1);
    u8 sel = mSelectedId;
    mWindow->mCellAnims[sel]->SetAnimation(sOptionMainMenuCells[sel].mAnimIndices[2]);
}

void OptionScene::BuildSettings() {
    PrintMessage(1);
    SpriteManager::Get();
    u8 i;
    CellAnimation *cell;
    for (i = 0; i < 5; i++) {
        cell = mCellAnims[i];
        if (cell != NULL) {
            delete cell;
        }
        mCellAnims[i] = new CellAnimation("option_b_obj_00_nce.bin", sOptionSettingsAnimIndices[i], 1, 1);
        mPosX[i] = sOptionSettingsStartPos[i].x;
        mPosY[i] = sOptionSettingsStartPos[i].y;
    }
    SaveData *data = gSaveData;
    mRow = 0;
    mCellAnims[2]->SetAnimation(0x13);
    mSlideTimer = 8;
    mOld[0] = data->mVolume0Index;
    mOld[1] = data->mVolume1Index;
    mOld[2] = data->mVolume2Index;
    mCur[0] = data->mVolume0Index;
    mCur[1] = data->mVolume1Index;
    mCur[2] = data->mVolume2Index;
    mTouchRow = -1;
    delete mWindow;
    mWindow = new SelectWindow(2, sOptionSettingsItems, -1);
    mWindow->SetCells(2, sOptionSettingsCells);
    mWindow->Update(1);
}

void OptionScene::DrawSettings() {
    if (mSlideTimer != 0) {
        for (u8 i = 0; i < 5; i++) {
            mPosX[i] += sOptionSettingsSlidePerFrame[i].x;
            mPosY[i] += sOptionSettingsSlidePerFrame[i].y;
        }
        mSlideTimer--;
    }
    for (u8 i = 0; i < 3; i++) {
        mCellAnims[1]->Draw(1, (s16) (mPosX[2 + i] + sOptionSettingsColumnX[mCur[i]]), sOptionSettingsRowY[i], 0);
    }
    for (u8 i = 0; i < 5; i++) {
        mCellAnims[i]->Update();
        mCellAnims[i]->Draw(1, mPosX[i], mPosY[i], 0);
    }
}

void ApplyVolumeOptions(SaveData *d);


void OptionScene::UpdateSettings() {
    BOOL changed;
    u8 prev;
    prev = mRow;
    changed = FALSE;
    if (TouchManager::Get().mDown) {
        if (TouchManager::Get().mPressed) {
            mTouchRow = -1;
            for (u8 i = 0; i < 3; i++) {
                if (TouchManager::Get().IsDownIn(0, sOptionSettingsRowY[i], 0x100, 0x18)) {
                    mTouchRow = i;
                    break;
                }
            }
        }
        if (mTouchRow < 0) {
            return;
        }
        for (u8 i = 0; i < 6; i++) {
            if (TouchManager::Get().IsDownIn(sOptionSettingsColumnX[i], sOptionSettingsRowY[mTouchRow], 0x10, 0x18)) {
                mCur[mTouchRow] = i;
            }
        }
        mRow = mTouchRow;
    } else if (TouchManager::Get().mReleased) {
        if (mTouchRow < 0) {
            return;
        }
        changed = TRUE;
    } else if (InputManager::Get().IsTrigger(KEY_UP)) {
        if (mRow == 0) {
            mRow = 2;
        } else {
            mRow--;
        }
    } else if (InputManager::Get().IsTrigger(KEY_DOWN)) {
        if (mRow == 2) {
            mRow = 0;
        } else {
            mRow++;
        }
    } else if (InputManager::Get().IsTrigger(KEY_LEFT)) {
        mCur[mRow]--;
        if (mCur[mRow] >= 6) {
            mCur[mRow] = 0;
        }
        changed = TRUE;
    } else if (InputManager::Get().IsTrigger(KEY_RIGHT)) {
        mCur[mRow]++;
        if (mCur[mRow] >= 6) {
            mCur[mRow] = 5;
        }
        changed = TRUE;
    }
    if (prev != mRow) {
        mCellAnims[prev + 2]->SetAnimation(sOptionSettingsAnimIndices[prev + 2]);
        mCellAnims[mRow + 2]->SetAnimation(sOptionSettingsAnimIndices[mRow + 5]);
    }
    if (changed) {
        switch (mRow) {
            case 0:
                gSaveData[0].mVolume0Index = mCur[0];
                ApplyVolumeOptions(gSaveData);
                break;
            case 1:
                gSaveData[0].mVolume1Index = mCur[1];
                ApplyVolumeOptions(gSaveData);
                SoundManager::Get().StopSeqArcPlayerSounds(1);
                SoundManager::Get().StartSeqArc(5);
                break;
            case 2:
                gSaveData[0].mVolume2Index = mCur[2];
                ApplyVolumeOptions(gSaveData);
                FrameTask::Get().StopChannel(1);
                FrameTask::Get().StartChannel(1, 0x183);
                break;
        }
    }
}

void OptionScene::BuildMenu2() {
    PrintMessage(2);
    SpriteManager::Get();
    if (mCellAnims[0] != NULL) {
        delete mCellAnims[0];
    }
    mCellAnims[0] = new CellAnimation("option_b_obj_00_nce.bin", 1, 1, 1);
    mPosX[0] = 0x80;
    mPosY[0] = 0x50;
    mSlideTimer = 8;
    delete mWindow;
    mWindow = new SelectWindow(4, sOptionMenu2Items, (s8) (gSaveData[0].mMicEnabled == 0 ? 1 : 0));
    mWindow->SetCells(4, sOptionMenu2Cells);
    mWindow->Update(1);
    int sel = gSaveData[0].mMicEnabled == 0 ? 1 : 0;
    mWindow->mCellAnims[(u8) sel]->SetAnimation(sOptionMenu2Cells[sel].mAnimIndices[2]);
}

void OptionScene::DrawMenu2() {
    if (mSlideTimer != 0) {
        mPosY[0] += 2;
        mSlideTimer--;
    }
    mCellAnims[0]->Draw(1, mPosX[0], mPosY[0], 0);
}

void OptionScene::BuildMenu3() {
    GX_SetVisiblePlane(0x11);
    delete mWindow;
    mWindow = new SelectWindow(3, sOptionMenu3Items, 1);
    mWindow->SetCells(3, sOptionMenu3Cells);
    u8 i;
    for (i = 0; i < 3; i++) {
        mPosX[i] = sOptionMenu3StartPos[i].x;
        s16 y = sOptionMenu3StartPos[i].y;
        mPosY[i] = y;
        SelectWindow *w = mWindow;
        w->mCellPos[i].x = mPosX[i];
        w->mCellPos[i].y = y;
    }
    mPosX[i] = sOptionMenu3StartPos[i].x;
    s16 y = sOptionMenu3StartPos[i].y;
    mPosY[i] = y;
    G2S_SetBG1Offset(mPosX[i], mPosY[i]);
    G2S_SetBG2Offset(mPosX[i], mPosY[i]);
    GXS_SetVisiblePlane(0x17);
    G2_SetBlendBrightness(3, -8);
    G2S_SetBlendBrightness(1, -8);
    mWindow->Update(1);
    mSlideTimer = 8;
    MI_CpuClearFast(mBuf0c, mBufSize);
    TextManager &text = TextManager::Get();
    s16 outWidth;
    s16 outHeight;
    outHeight = 0x38;
    outWidth = 0;
    MessageFile *file = mFile;
    text.Print(file->GetText(3), file->GetLength(3), mBuf0c, 0x18, 0x18, &outWidth, &outHeight, NULL, 0, 2, 0, 0x10, 0, 0, 1, 1, 1);
    QueueUpload(1, UPLOAD_KIND_BG2_CHAR, 0, mBuf0c, mBufSize, 0);
}

void OptionScene::SlideStep() {
    if (mSlideTimer == 0) {
        return;
    }
    u8 i;
    for (i = 0; i < 3; i++) {
        mPosX[i] += sOptionMenu3SlidePerFrame[i].x;
        mPosY[i] += sOptionMenu3SlidePerFrame[i].y;
        SelectWindow *w = mWindow;
        s16 y = mPosY[i];
        w->mCellPos[i].x = mPosX[i];
        w->mCellPos[i].y = y;
    }
    mPosX[i] += sOptionMenu3SlidePerFrame[i].x;
    mPosY[i] += sOptionMenu3SlidePerFrame[i].y;
    G2S_SetBG1Offset(mPosX[i], mPosY[i]);
    G2S_SetBG2Offset(mPosX[i] - 0x20, mPosY[i]);
    mSlideTimer--;
}

void OptionScene::ResetDisplay() {
    G2_BlendNone();
    G2S_BlendNone();
    GX_SetVisiblePlane(0x13);
    GXS_SetVisiblePlane(0x11);
}







void OptionScene::BuildMenu4() {
    PrintMessage(4);
    SpriteManager::Get();
    if (mCellAnims[0] != NULL) {
        delete mCellAnims[0];
    }
    mCellAnims[0] = new CellAnimation("option_b_obj_00_nce.bin", 2, 1, 1);
    mPosX[0] = 0x80;
    mPosY[0] = 0x50;
    mSlideTimer = 8;
    delete mWindow;
    mWindow = new SelectWindow(4, sOptionMenu4Items, (s8) (gSaveData[0].mUnk_28_5 == 0 ? 1 : 0));
    mWindow->SetCells(4, sOptionMenu4Cells);
    mWindow->Update(1);
    int sel = gSaveData[0].mUnk_28_5 == 0 ? 1 : 0;
    mWindow->mCellAnims[(u8) sel]->SetAnimation(sOptionMenu4Cells[sel].mAnimIndices[2]);
}





void OptionScene::DrawMenu4() {
    if (mSlideTimer != 0) {
        mPosY[0] += 2;
        mSlideTimer--;
    }
    mCellAnims[0]->Draw(1, mPosX[0], mPosY[0], 0);
}

void OptionScene::PrintMessage(u32 index) {
    MI_CpuClearFast(mBuf0c, mBufSize);
    TextManager &text = TextManager::Get();
    u16 pixelWidth;
    s16 outWidth;
    s16 outHeight;
    outHeight = 0x38;
    outWidth = 0;
    MessageFile *file = mFile;
    text.Print(file->GetText(index), file->GetLength(index), mBuf0c, 0x18, 0x18, &outWidth, &outHeight, &pixelWidth, 0, 2, 0, 0x10, 0, 0, 0, 1, 1);
    QueueUpload(0, UPLOAD_KIND_BG1_CHAR, 0, mBuf0c, mBufSize, 0);
    G2_SetBG1Offset((pixelWidth - 0x100) / 2, 0);
}
