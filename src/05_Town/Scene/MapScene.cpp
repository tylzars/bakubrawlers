#include "Scene/MapScene.hpp"
#include "Game/ScriptPlayer.hpp"
#include "Game/UnkStruct_020d433c.hpp"
#include "Game/SaveData.hpp"
#include "Game/UnkStruct_020d48f0.hpp"
#include "Graphics/CellAnimation.hpp"
#include "Graphics/Display.hpp"
#include "Graphics/SpriteManager.hpp"
#include "System/FileSystem.hpp"
#include "System/FrameUpdate.hpp"
#include "System/TouchManager.hpp"
#include "Text/TextManager.hpp"
#include "UI/SelectWindow.hpp"
#include "System/FrameTask.hpp"
#include "Unknown/UnkFunctions.hpp"
#include "UI/SaveWindow.hpp"
#include "UI/UnlockPopup.hpp"

#include <nitro/gx.h>
#include <nitro/mi.h>
#include <nitro/os.h>

// Overlay data tables.
struct MapPagePos {
    s16 mX;
    s16 mY;
    s16 mUnk_04;
    s16 mId;
};
static const u8 data_ov005_0221fe90[5] = {1, 2, 0, 3, 0};
static const u32 sMapCurrentTextIndices[6] = {0x12, 0x13, 0x14, 0x15, 0x16, 0x17};
static const MapPagePos sMapPagePos[4] = {
    {147, 134, 147, 3},
    {224, 141, 224, 2},
    {383, 114, 320, 1},
    {471, 159, 471, 0},
};
static const WindowItemRecord sMapPanelWindowItems[4] = {
    {0, 1, 45, 136, 78, 20, 0, {0, 0, 1, 1}, 0, 2},
    {1, 1, 133, 136, 78, 20, 0, {1, 1, 0, 0}, 0, 2},
    {2, 32768, 120, 35, 0, 0, 0, {-1, -1, -1, -1}, 0, 0},
    {3, 32768, 128, 96, 0, 0, 0, {-1, -1, -1, -1}, 0, 0},
};
static const WindowItemRecord sMapMenuItems[5] = {
    {0, 1, 4, 160, 48, 32, 0, {0, 0, 0, 1}, 0, 2},
    {1, 1, 52, 160, 48, 32, 0, {1, 1, 0, 2}, 0, 2},
    {2, 1, 100, 160, 48, 32, 0, {2, 2, 1, 3}, 0, 2},
    {3, 1, 148, 160, 48, 32, 0, {3, 3, 2, 3}, 0, 2},
    {4, 2, 200, 170, 54, 20, 0, {-1, -1, -1, -1}, 0, 1},
};
static const WindowCellRecord sMapMenuCells[5] = {
    {"story/map_b_obj_00_nce.bin", 0, 0x0, 0, 2, 1, {0, 10, 5, 0}},
    {"story/map_b_obj_00_nce.bin", 2, 0x0, 0, 2, 1, {2, 12, 7, 0}},
    {"story/map_b_obj_00_nce.bin", 3, 0x0, 0, 2, 1, {3, 13, 8, 0}},
    {"story/map_b_obj_00_nce.bin", 4, 0x0, 0, 2, 1, {4, 14, 9, 0}},
    {"/Game/2D/union_b/union_b_obj_quit_nce.bin", 0, 0x4000, 12, 2, 1, {0, 1, 1, 0}},
};

MapScene::MapScene() :
    SceneBase(1000, 0) {
    mEvent       = NULL;
    mMenu        = NULL;
    mPanelWindow = NULL;
    mGlyphBuffer = NULL;
    mText[0].mMessages = NULL;
    mText[0].mTextBuffer = NULL;
    mText[0].mGlyphBuffer = NULL;
    mText[0].mTextBufferSize = 0;
    mText[0].mGlyphCount = 0;
    mText[1].mMessages = NULL;
    mText[1].mTextBuffer = NULL;
    mText[1].mGlyphBuffer = NULL;
    mText[1].mTextBufferSize = 0;
    mText[1].mGlyphCount = 0;
    mState      = 0;
    mTimer      = 0;
    mNextScene   = 9;
    mUnk_68      = 0;
    mUnk_69      = 0;
    mScrollPos   = 0;
    mScrollTarget = 0;
    mParallax    = 0;
    mDragging    = 0;
    mLastTouchX  = 0;
    mUnk_74      = -1;
    mSkipDraw      = 0;
    mPage        = 0;
    mPanelVisible      = 0;
    mTextWidth   = 0;
    mUnk_7c      = 0;
    mUnk_7d      = 0;
    mUnk_7e      = 0;
    MI_CpuClear32(mCellAnims, sizeof(mCellAnims));
    mTab = data_020d433c.mUnk_7e_0;
    data_020d433c.mUnk_8a_0 = 0;
    data_020d433c.mUnk_8a_4 = 0;
}

MapScene::~MapScene() {
    for (u8 i = 0; i < 6; i++) {
        if (mCellAnims[i] != NULL) {
            delete mCellAnims[i];
        }
    }
    if (mText[0].mMessages != NULL) {
        OS_Free(mText[0].mMessages);
    }
    if (mText[0].mTextBuffer != NULL) {
        OS_Free(mText[0].mTextBuffer);
    }
    if (mText[0].mGlyphBuffer != NULL) {
        OS_Free(mText[0].mGlyphBuffer);
    }
    if (mText[1].mMessages != NULL) {
        OS_Free(mText[1].mMessages);
    }
    if (mText[1].mTextBuffer != NULL) {
        OS_Free(mText[1].mTextBuffer);
    }
    if (mText[1].mGlyphBuffer != NULL) {
        OS_Free(mText[1].mGlyphBuffer);
    }
    if (mMenu != NULL) {
        delete mMenu;
    }
    if (mPanelWindow != NULL) {
        delete mPanelWindow;
    }
    data_020d433c.mUnk_7e_0 = mTab;
    OS_Free(mGlyphBuffer);
}

void MapScene::Update() {
    if ((mState > 1) & (mState < 0x10)) {
        if (SetNextSceneFromEvent()) {
            mState = 0x10;
        }
    }
    switch (mState) {
        case 0:
            Init();
            mState++;
            break;
        case 1:
            if (!func_ov005_0221b2ac()) {
                mState = 0xb;
                break;
            }
            mState++;
            break;
        case 2:
            SpriteManager::Get().StartFade(2);
            mState++;
            break;
        case 3:
            if (!mEvent->IsRunning()) {
                mState = 0xb;
                break;
            }
            if (mEvent->IsGraphicsLoaded()) {
                GXS_SetVisiblePlane(0x10);
                mEvent->ClearGraphicsLoaded();
            }
            if (mEvent->GetSaveRequest()) {
                SpriteManager::Get().StartSubFade(3);
                mState = 4;
            }
            if (mEvent->GetUnlockPopupHandle()) {
                SpriteManager::Get().StartSubFade(3);
                mState = 8;
            }
            break;
        case 4:
            SpriteManager::Get();
            if (SpriteManager::sFade[1] > 1) {
                break;
            }
            LoadWindowGraphics();
            new SaveWindow(1, 1, 0, 0x2000, 0, 0, 0, 0, 0);
            mContext->mRequest = 0;
            data_020d433c.mUnk_14_1 = 0;
            SpriteManager::Get().StartSubFade(2);
            mState++;
            break;
        case 5:
            if (mContext->mRequest == 0) {
                break;
            }
            SpriteManager::Get().StartSubFade(3);
            mState++;
            break;
        case 6:
            SpriteManager::Get();
            if (SpriteManager::sFade[1] > 1) {
                break;
            }
            ReloadMap();
            mEvent->ClearSaveRequest();
            mTimer = 8;
            mState++;
            break;
        case 7:
            mTimer--;
            if (mTimer != 0) {
                break;
            }
            if (!mEvent->IsRunning()) {
                mSkipDraw = 0;
                mState = 0xb;
                break;
            }
            SpriteManager::Get().StartSubFade(2);
            mState = 3;
            break;
        case 8:
            SpriteManager::Get();
            if (SpriteManager::sFade[1] > 1) {
                break;
            }
            new UnlockPopup(mEvent->GetUnlockPopupHandle() - 1);
            mContext->mRequest = 0;
            mState++;
            break;
        case 9:
            if (mContext->mRequest == 0) {
                break;
            }
            ReloadMap();
            mEvent->ClearUnlockPopupHandle();
            mState++;
            break;
        case 10:
            mState = 3;
            break;
        case 11:
            if (mEvent->IsRunning()) {
                break;
            }
            FrameTask::Get().StartChannel(0, 0x12);
            CreateMenu();
            mEvent->func_0206ef4c("mes_town", data_ov005_0221fe90[mTab], 10, 7, 0x15);
            SpriteManager::Get().StartFade(2);
            mState++;
            break;
        case 12: {
            UpdateDrag();
            QueueFrameCallback((FrameCallback) MapScrollCallback, this);
            u8 r = UpdateMenu();
            if (r != 0) {
                if (r != 1) {
                    CreatePanelWindow();
                    mState = 0xf;
                } else {
                    mState++;
                }
            }
            break;
        }
        case 13:
            mMenu->Update(TRUE);
            if (mMenu->mState == 4) {
                if (mTab == 3) {
                    switch (gSaveData[0].mUnk_42) {
                        case 1:
                        case 3:
                        case 6:
                        case 7:
                        case 8:
                        case 9:
                        case 10:
                            if (mUnk_7c == 0) {
                                mState++;
                                goto done;
                            }
                            break;
                    }
                    mMenu->Reset((s8) mTab);
                    {
                        CellAnimation *cell = mMenu->mCells[mTab];
                        cell->mAnimIndex = sMapMenuCells[mTab].mAnimIndices[2];
                        cell->ResetAnimation();
                    }
                    mState--;
                } else {
                    mState++;
                }
            }
            break;
        case 14:
            if (SetNextSceneFromTab()) {
                mState = 0x10;
            }
            break;
        case 15: {
            u32 r = UpdatePanelWindow();
            if (r == 0) {
                break;
            }
            mPanelVisible = 0;
            if (r == 2) {
                CreateMenu();
                mState = 0xc;
            } else {
                mUnk_7d = 1;
                mNextScene = 4;
                mState++;
            }
            break;
        }
        case 16:
            SpriteManager::Get().StartFade(3);
            mState++;
            break;
        case 17:
            if (SpriteManager::Get().IsFading()) {
                break;
            }
            mEvent->RequestFinish();
            mContext->mRequest = 0;
            mState++;
            break;
        case 18:
            if (mContext->mRequest == 0) {
                break;
            }
            mSkipDraw = 1;
            mState++;
            if (mNextScene == 7 && gSaveData[0].mUnk_42 == 5) {
                mEvent = new ScriptPlayer(7, 0, 1);
                mState = 0x18;
            }
            break;
        case 19:
            if (mUnk_7d == 0) {
                mState = 0x17;
                break;
            }
            LoadWindowGraphics();
            GX_SetVisiblePlane(0);
            mTimer = 8;
            mState++;
            break;
        case 20:
            mTimer--;
            if (mTimer != 0) {
                break;
            }
            new SaveWindow(1, 1, 0, 0x2000, 0, 0, 0, 0, 0);
            mContext->mRequest = 0;
            data_020d433c.mUnk_14_1 = 0;
            mState++;
            break;
        case 21:
            if (mContext->mRequest == 0) {
                break;
            }
            SpriteManager::Get().StartSubFade(3);
            mState++;
            break;
        case 22:
            SpriteManager::Get();
            if (SpriteManager::sFade[1] > 1) {
                break;
            }
            mState++;
            break;
        case 23:
            RequestScene(mNextScene);
            mFinished = 1;
            break;
        case 24:
            if (mEvent->IsRunning()) {
                break;
            }
            mEvent->StartScript("cmd_ST_mev_011.bin");
            SpriteManager::Get().StartFade(2);
            mState++;
            break;
        case 25:
            if (mEvent->IsRunning()) {
                break;
            }
            gSaveData[0].mUnk_42 = 6;
            gSaveData[0].mUnk_43 = 0;
            mState = 0x10;
            break;
    }
done:
    if (mSkipDraw != 0) {
        return;
    }
    if (mPanelVisible != 0) {
        DrawPanelWindow();
        return;
    }
    DrawPopup();
    if ((mState >= 0xc) & (mState < 0x12)) {
        DrawPage();
    }
    DrawMoney();
}

void MapScene::Init() {
    InitDisplay();
    LoadMapBackground();
    func_ov005_0221abf4();
    LoadPanelText();
    mEvent = new ScriptPlayer(0, 0, 0);
    SetupMap();
    mGlyphCount  = TextManager::Get().GlyphCount(8);
    mGlyphBuffer = OS_Alloc((s16) mGlyphCount * (s16) 0xc);
}

void MapScene::SetupMap() {
    switch (mEvent->func_020667ec()) {
        case 3:
            if (gSaveData[0].mUnk_24 & 1) {
                mUnk_74 = 3;
            } else {
                mUnk_74 = 1;
                mUnk_7c = 1;
            }
            break;
        case 2:
            mUnk_74 = 2;
            break;
        default:
            if (gSaveData[0].mUnk_42 == 5) {
                mUnk_74 = 0;
            }
            break;
    }
    mScrollTarget = func_ov005_0221b760(mTab) << 4;
    mScrollPos    = mScrollTarget;
    QueueFrameCallback((FrameCallback) MapScrollCallback, this);
    CreateMenu();
}

void MapScene::InitDisplay() {
    GX_SetDispSelect(1);
    SpriteManager::Get().SetupMode(1, 7);
    GX_SetGraphicsMode(1, 0, 1);
    GXS_SetGraphicsMode(0);
    G2S_SetBG0Control(1, 1, 0, 4, 1);
    G2S_SetBG1Control(1, 1, 2, 1, 1);
    G2S_SetBG2ControlText(0, 1, 4, 1);
    G2_SetBG0Offset(0, 0);
    G2S_SetBG0Offset(0, 0);
    G2S_SetBG1Offset(0, 0);
    G2S_SetBG2Offset(0, 0);
    G2_SetBG0Priority(0);
    G2S_SetBG0Priority(2);
    G2S_SetBG1Priority(2);
    G2S_SetBG2Priority(2);
    GX_SetVisiblePlane(0x11);
    GXS_SetVisiblePlane(0x17);
    G2_SetBlendBrightness(0x1f, -0x10);
    G2S_SetBlendBrightness(0x1f, -0x10);
    GX_SetVisibleWnd(0);
    GXS_SetVisibleWnd(0);
    G2_SetWndOutsidePlane(0x1f, FALSE);
    G2S_SetWndOutsidePlane(0x1f, FALSE);
    G2_BlendNone();
    G2S_BlendNone();
}

void MapScene::LoadWindowGraphics() {
    MI_CpuClearFast((void *) 0x6200000, 0x20000);
    MI_CpuClearFast((void *) 0x6600000, 0x20000);
    G2S_SetBG0Control(0, 0, 0, 1, 0);
    G2S_SetBG1Control(0, 1, 2, 4, 0);
    G2S_SetBG0Offset(0, 0);
    G2S_SetBG1Offset(0, 0);
    G2S_SetBG0Priority(0);
    G2S_SetBG1Priority(0);
    GXS_SetVisiblePlane(0x13);
    ChangeDir("/Game/2D/union_b/");
    LoadAndQueueUpload(1, 1, 0, "union_b_bg_window_ncl.bin", 0, 0, 0);
    LoadAndQueueUpload(1, 11, 0, "union_b_bg_window_ncg.bin", 0, 0, 0);
    LoadAndQueueUpload(1, 7, 0, "union_b_bg_window_b_nsc.bin", 0, 0, 0);
    SpriteScreen &screen = SpriteManager::Get().Screen(1);
    screen.LoadCharacters("union_b_obj_window.nce_ncg1D.bin", 0, 0);
    screen.LoadPalette("union_b_obj_window_ncl.bin", 0, 0);
    ChangeDir("/");
}

void MapScene::ReloadMap() {
    G2S_SetBG0Control(1, 1, 0, 4, 1);
    G2S_SetBG1Control(1, 1, 2, 1, 1);
    G2S_SetBG2ControlText(0, 1, 4, 1);
    GXS_SetVisiblePlane(0x17);
    G2S_SetBG0Priority(2);
    G2S_SetBG1Priority(2);
    G2S_SetBG2Priority(2);
    LoadMapBackground();
    func_ov005_0221abf4();
    LoadPanelText();
    SetupMap();
}

void MapScene::LoadMapBackground() {
    ChangeDir("/story/");
    LoadAndQueueUpload(1, 1, 0x4000, "map_b_bg_00_ncl.bin", 0, 0, 0);
    LoadAndQueueUpload(1, 11, 0, "map_b_bg_00_ncg.bin", 0, 0, 0);
    LoadAndQueueUpload(1, 7, 0, "map_b_bg_00_nsc.bin", 0, 0, 0);
    LoadAndQueueUpload(1, 1, 0x6000, "map_b_bg_00_ncl.bin", 0, 0, 0);
    LoadAndQueueUpload(1, 12, 0, "map_b_bg_01_ncg.bin", 0, 0, 0);
    LoadAndQueueUpload(1, 8, 0, "map_b_bg_01_a_nsc.bin", 0, 0, 0);
    LoadAndQueueUpload(1, 9, 0, "map_b_bg_01_b_nsc.bin", 0, 0, 0);
    ChangeDir("/");
}

void MapScene::func_ov005_0221abf4() {
    SpriteScreen &screen = SpriteManager::Get().Screen(1);
    screen.SetObjVramMode(3);
    ChangeDir("/story/");
    char path[0x80];
    sprintf(path, "map_b_obj_00.nce_ncg1D.bin");
    screen.LoadCharacters(path, 0, 0);
    sprintf(path, "map_b_obj_00_ncl.bin");
    screen.LoadPalette(path, 0, 0);
    for (u8 i = 0; i <= 1; i++) {
        if (mCellAnims[i] != NULL) {
            delete mCellAnims[i];
        }
        mCellAnims[i] = new CellAnimation("map_b_obj_00_nce.bin", 0, 1, 1);
        mCellAnims[i]->mCharBase = screen.GetCharBase(0);
        mCellAnims[i]->mPaletteBase = 0;
    }
    mCellAnims[0]->SetAnimation(0x10);
    mCellAnims[1]->SetAnimation(0xf);
    if (mCellAnims[4] != NULL) {
        delete mCellAnims[4];
    }
    mCellAnims[4] = new CellAnimation("map_b_obj_00_nce.bin", 0x11, 1, 1);
    mCellAnims[4]->mCharBase = screen.GetCharBase(0);
    mCellAnims[4]->mPaletteBase = 0;
    if (mCellAnims[5] != NULL) {
        delete mCellAnims[5];
    }
    mCellAnims[5] = new CellAnimation("map_b_obj_00_nce.bin", 0x12, 1, 1);
    mCellAnims[5]->mCharBase = screen.GetCharBase(0);
    mCellAnims[5]->mPaletteBase = 0;
    ChangeDir("/Game/2D/union_b/");
    screen.LoadCharacters("union_b_obj_point.nce_ncg1D.bin", 0x4800, 0);
    screen.LoadPalette("union_b_obj_point_ncl.bin", 0x1a0, 0);
    if (mCellAnims[2] != NULL) {
        delete mCellAnims[2];
    }
    mCellAnims[2] = new CellAnimation("union_b_obj_point_nce.bin", 0, 1, 1);
    mCellAnims[2]->mCharBase = screen.GetCharBase(0x4800);
    mCellAnims[2]->mPaletteBase = 0xd;
    screen.LoadCharacters("union_b_obj_number.nce_ncg1D.bin", 0x3000, 0);
    screen.LoadPalette("union_b_obj_number_ncl.bin", 0x1c0, 0);
    if (mCellAnims[3] != NULL) {
        delete mCellAnims[3];
    }
    mCellAnims[3] = new CellAnimation("union_b_obj_number_nce.bin", 0, 1, 1);
    mCellAnims[3]->mCharBase = screen.GetCharBase(0x3000);
    mCellAnims[3]->mPaletteBase = 0xe;
    screen.LoadCharacters("union_b_obj_quit.nce_ncg1D.bin", 0x4000, 0);
    screen.LoadPalette("union_b_obj_quit_ncl.bin", 0x180, 0);
    screen.LoadCharacters("union_b_obj_window_a.nce_ncg1D.bin", 0x9000, 0);
    screen.LoadExtPalette("union_b_obj_window_a_ncl.bin", 0, 0);
    screen.LoadCharacters("union_b_obj_window.nce_ncg1D.bin", 0xb000, 0);
    screen.LoadPalette("union_b_obj_window_ncl.bin", 0x140, 0);
    ChangeDir("/");
    if (mText[1].mMessages != NULL) {
        return;
    }
    mText[1].mMessages = (MessageFile *) LoadFile("font/mes_town_info.mes", 0, 0, 0, 0, 0);
    mText[1].mTextBufferSize = TextManager::Get().BufferSize(0x20, 4);
    mText[1].mTextBuffer = OS_Alloc(mText[1].mTextBufferSize);
    MI_CpuClear16(mText[1].mTextBuffer, mText[1].mTextBufferSize);
    mText[1].mGlyphCount = TextManager::Get().GlyphCount(0x10);
    mText[1].mGlyphBuffer = OS_Alloc((s16) mText[1].mGlyphCount * (s16) 12);
    TextManager::Get().Init(mText[1].mGlyphBuffer, 1, 0, 0x20, 4, 0x21);
}

BOOL MapScene::SetNextSceneFromEvent() {
    u32 result = mEvent->GetResult();
    if (result != 0) {
        switch (result) {
            case 1:
                mNextScene            = 7;
                data_020d433c.mUnk_7f = 9;
                break;
            case 2:
                mNextScene = 6;
                break;
            case 3:
                mNextScene = 12;
                break;
            case 4:
                mNextScene = 13;
                break;
            case 5:
                data_020d433c.mUnk_9a   = 0x11;
                data_020d433c.mUnk_9a_0 = gSaveData[0].mUnk_28_6;
                mNextScene              = 5;
                break;
            case 6:
                mNextScene = 9;
                break;
            case 7:
                data_020d433c.mUnk_83 = 1;
                data_020d433c.mUnk_84 = 0;
                data_020d433c.mUnk_82 = 1;
                mNextScene            = 10;
                break;
            default:
                mNextScene = 4;
                break;
        }
        return TRUE;
    }
    return FALSE;
}

BOOL MapScene::func_ov005_0221b2ac() {
    if (!mEvent->func_02066c7c()) {
        return FALSE;
    }
    mSkipDraw = 1;
    mTab    = 2;
    return TRUE;
}

u8 MapScene::UpdateMenu() {
    u8 result       = 0;
    u8 prev         = mMenu->mCursor->mSelection;
    mMenu->mEnabled = 1;
    u8 status       = mMenu->Update(1);
    u8 sel          = mMenu->mCursor->mSelection;
    if (sel < 4) {
        mTab = sel;
    }
    switch (status) {
        case 1:
            result = 1;
            break;
        case 2:
            result = 2;
            break;
        default:
            if (sel <= 3) {
                BOOL changed = FALSE;
                if (prev != sel) {
                    changed = TRUE;
                }
                func_ov005_0221b364(sel, changed);
            }
            break;
    }
    return result;
}

void MapScene::func_ov005_0221b364(u8 index, BOOL changed) {
    mEvent->func_0206f564(data_ov005_0221fe90[index], 10, (s8) index);
    if (changed == 0) {
        return;
    }
    mScrollTarget = func_ov005_0221b760(index) << 4;
    ShowTabInfo(index);
}

void MapScene::CreateMenu() {
    if (mMenu != NULL) {
        delete mMenu;
    }
    mMenu = new SelectWindow(5, sMapMenuItems, -1);
    mMenu->SetCells(5, sMapMenuCells);
    mMenu->mCursor->mSelection = -1;
    mMenu->mEnabled            = 0;
    mMenu->mCursor->mSelection = mTab;
    for (u8 i = 0; i < 4; i++) {
        u32 state           = i == mTab ? 2 : 0;
        CellAnimation *cell = mMenu->mCells[i];
        cell->mAnimIndex       = sMapMenuCells[i].mAnimIndices[state];
        cell->ResetAnimation();
    }
    func_ov005_0221b364(mTab, TRUE);
    LoadPanelText2(mTab);
    mScrollTarget = func_ov005_0221b760(mTab) << 4;
}

void MapScene::UpdateDrag() {
    u8 cur = TouchManager::Get().mX;
    if (mDragging != 0) {
        if (TouchManager::Get().mDown == 0) {
            mDragging = 0;
        }
    } else {
        if (TouchManager::Get().mPressed != 0) {
            mLastTouchX = cur;
        }
        if (TouchManager::Get().mDown != 0 && mLastTouchX != cur) {
            mDragging = 1;
        }
    }
    if (mDragging != 0) {
        s32 value = mScrollTarget + (s16) (-(cur - mLastTouchX) << 4);
        if (value <= 0) {
            mScrollTarget = 0;
        } else {
            mScrollTarget = value;
            if (mScrollTarget > 0x1000) {
                mScrollTarget = 0x1000;
            }
        }
        mLastTouchX = cur;
    }
    mScrollPos = (mScrollPos * 15 + mScrollTarget) / 16;
    mParallax  = (mParallax - 3) % 0x8000;
}

void MapScrollCallback(MapScene *scene) {
    G2S_SetBG0Offset((scene->mScrollPos * 0x100) / 0x1000, 0);
    G2S_SetBG1Offset(0x40 - (scene->mScrollPos * 0x40) / 0x1000, 0);
    G2S_SetBG2Offset(0x80 - ((scene->mScrollPos * 0x80) / 0x1000 + ((u32) scene->mParallax >> 7)), 0);
}

BOOL MapScene::SetNextSceneFromTab() {
    switch (mTab) {
        case 0:
            mNextScene            = 7;
            data_020d433c.mUnk_7f = 9;
            break;
        case 1:
            mNextScene = 6;
            break;
        case 2:
            mNextScene = 12;
            break;
        case 3:
            mNextScene = 13;
            break;
        default:
            mNextScene = 4;
            break;
    }
    return TRUE;
}

s16 MapScene::func_ov005_0221b760(u32 index) {
    u8 found = 0;
    for (u8 i = 0; i < 4; i++) {
        if (index == sMapPagePos[i].mId) {
            found = i;
            break;
        }
    }
    if (mPage != found) {
        mPage = found;
        LoadPanelText2(index);
    }
    s32 pos = sMapPagePos[found].mUnk_04 - 0x80;
    if (pos > 0xff) {
        pos = 0xff;
    } else if (pos < 0) {
        pos = 0;
    }
    return pos;
}

void MapScene::DrawPanelWindow() {
    G2S_SetWndOutsidePlane(0x10, TRUE);
    G2S_SetBlendBrightness(7, -8);
    DrawPanelText();
    mPanelWindow->Draw();
}

void MapScene::DrawPopup() {
    G2S_SetWndOutsidePlane(0x10, FALSE);
    reg_G2S_DB_WINOUT = (u16) ((reg_G2S_DB_WINOUT & ~0x3f00) | 0x3000);
    G2S_SetBlendAlpha(0, 0x17, 0x10, 8);
    mPageVisible = 1;
    switch (mTab) {
        case 0:
            if (mUnk_7e_0) {
                SpriteManager::Get().Screen(1).AddOam(mGlyphBuffer, mGlyphCount, (s16) mPopupTextX, (s16) mPopupTextY, 0,
                                                             0xa0, 0xf, 0);
                mCellAnims[4]->Draw(1, 0x80, 0x60, 1);
                mCellAnims[5]->Draw(1, 0x80, 0x64, 0);
                mPageVisible = 0;
            }
            break;
        case 1:
            if (mUnk_7e_1) {
                SpriteManager::Get().Screen(1).AddOam(mGlyphBuffer, mGlyphCount, (s16) mPopupTextX, (s16) mPopupTextY, 0,
                                                             0xa0, 0xf, 0);
                mCellAnims[4]->Draw(1, 0x80, 0x60, 1);
                mPageVisible = 0;
            }
            break;
        case 2:
            if (mUnk_7e_2) {
                SpriteManager::Get().Screen(1).AddOam(mGlyphBuffer, mGlyphCount, (s16) mPopupTextX, (s16) mPopupTextY, 0,
                                                             0xa0, 0xf, 0);
                mCellAnims[4]->Draw(1, 0x80, 0x60, 1);
                mPageVisible = 0;
            }
            break;
        case 3:
            if (mUnk_7e_3) {
                SpriteManager::Get().Screen(1).AddOam(mGlyphBuffer, mGlyphCount, (s16) mPopupTextX, (s16) mPopupTextY, 0,
                                                             0xa0, 0xf, 0);
                mCellAnims[4]->Draw(1, 0x80, 0x60, 1);
                mPageVisible = 0;
            }
            break;
    }
    if (mMenu != NULL) {
        mMenu->Draw();
    }
    if (mUnk_74 < 0) {
        return;
    }
    mCellAnims[1]->Update();
    mCellAnims[1]->Draw(1, (s16) (sMapMenuItems[mUnk_74].mX + 10), (s16) (sMapMenuItems[mUnk_74].mY - 0x18),
                        0);
}

void MapScene::DrawMoney() {
    u8 i         = 0;
    u32 value    = gSaveData[0].mMoney;
    s8 digits[7] = {-1, -1, -1, -1, -1, -1, -1};
    s8 pos       = 6;
    do {
        digits[pos] = value % 10;
        value /= 10;
        pos--;
    } while (pos >= 0 && value != 0);
    for (; i < 7; i++) {
        s8 digit = digits[i];
        if (digit >= 0 && mCellAnims[3] != NULL) {
            mCellAnims[3]->DrawFrame(1, digit, i * 7 + 0x2c, 3, 2);
        }
    }
    mCellAnims[2]->Draw(1, 0, 0, 2);
}

void MapScene::DrawPage() {
    if (mPageVisible == 0) {
        return;
    }
    s16 x = sMapPagePos[mPage].mX - ((u32) mScrollPos >> 4);
    if ((x > -0x40) & (x < 0x140)) {
        mCellAnims[0]->Update();
        mCellAnims[0]->Draw(1, x, sMapPagePos[mPage].mY, 2);
        DrawPanelText2(x);
    }
}

void MapScene::ShowTabInfo(u32 index) {
    mUnk_7e = 0;
    switch (index) {
        case 0:
            if (data_020d48f0.mRank == data_020d48f0.mPrevRank) {
                return;
            }
            TextManager::Get().mUse4bppFont = 1;
            ShowNumbers();
            data_020d48f0.mPrevRank = data_020d48f0.mRank;
            TextManager::Get().mUse4bppFont = 0;
            mUnk_7e_0 = 1;
            break;
        case 1:
            if (gSaveData[0].mUnk_5b9_0 == 0) {
                return;
            }
            ShowText(2, 0x20);
            mUnk_7e |= 2;
            break;
        case 2:
            func_02089208();
            if (gSaveData[0].mUnk_5b9_1 == 0) {
                return;
            }
            ShowText(0x11, 0x20);
            mUnk_7e |= 4;
            break;
        case 3:
            if (ShowCurrent()) {
                mUnk_7e |= 8;
            }
            break;
    }
}

void MapScene::ShowNumbers() {
    s16 x;
    s16 x2;
    s16 y;
    s16 height;
    s16 width;
    if (mGlyphBuffer != NULL) {
        mGlyphCount = TextManager::Get().GlyphCount(8);
        TextManager::Get().Init(mGlyphBuffer, 1, 0, 0x20, 0x10, 0x44);
    }
    MessageFile *file = (MessageFile *) LoadFile("/font/mes_town_info.mes", 0, 0, 0, 0, 0);
    u32 size          = TextManager::Get().BufferSize(0x20, 0x10);
    void *buffer      = OS_Alloc(size);
    MI_CpuClearFast(buffer, size);
    TextManager &text = TextManager::Get();
    width  = 0;
    height = 0;
    text.Print(file->GetText(15), file->GetLength(15), buffer, 0x18, 0x10, &height, &width, &mPopupTextX, 0, 2,
               0x44, 0x10, 0, 0, 0, 1, 1);
    if (&mPopupTextY != NULL) {
        mPopupTextY = width;
    }
    mPopupTextX = (256 - mPopupTextX) / 2;
    mPopupTextY = 0x20;
    mPopupTextY = (0x6c - mPopupTextY) / 2 + 0x1e;
    y       = 0x20;
    if (data_020d48f0.mPrevRank >= 100) {
        x = 0x58 - mPopupTextX;
    } else if (data_020d48f0.mPrevRank >= 10) {
        x = 0x5c - mPopupTextX;
    } else {
        x = 0x60 - mPopupTextX;
    }
    TextManager::Get().PrintNumber(data_020d48f0.mPrevRank, 0, 0, buffer, 0x18, 0x10, &x, &y, 0, 2, 0x44, 0x10, 0, 1, 1);
    if (data_020d48f0.mRank >= 100) {
        x2 = 0x90 - mPopupTextX;
    } else if (data_020d48f0.mRank >= 10) {
        x2 = 0x94 - mPopupTextX;
    } else {
        x2 = 0x98 - mPopupTextX;
    }
    TextManager::Get().PrintNumber(data_020d48f0.mRank, 0, 0, buffer, 0x18, 0x10, &x2, &y, 0, 2, 0x44, 0x10, 0, 1, 1);
    QueueUpload(1, 4, 0x5000, buffer, size, 1);
    OS_Free(file);
    TextManager::Get().SetColor(1, 0, 0xf);
}

void MapScene::ShowText(u16 index, s32 unk2) {
    u16 cols;
    MessageFile *file;
    void *buffer;
    u16 size;
    cols = (256 - unk2 * 2) / 8;
    if (mGlyphBuffer != NULL) {
        mGlyphCount = TextManager::Get().GlyphCount(cols, 0x10, 0x44);
        TextManager::Get().Init(mGlyphBuffer, 1, 0, cols, 0x10, 0x44);
    }
    file   = (MessageFile *) LoadFile("/font/mes_town_info.mes", 0, 0, 0, 0, 0);
    size   = TextManager::Get().BufferSize(cols, 0x10);
    buffer = OS_Alloc(size);
    MI_CpuClearFast(buffer, size);
    TextManager &text = TextManager::Get();
    s16 height;
    s16 width;
    width  = 0;
    height = 0;
    text.Print(file->GetText(index), file->GetLength(index), buffer, cols, 0x10, &height, &width, &mPopupTextX, 0, 2,
               0x44, 0x10, 0, 0, 0, 1, 1);
    if (&mPopupTextY != NULL) {
        mPopupTextY = width;
    }
    mPopupTextX = (256 - mPopupTextX) / 2;
    mPopupTextY = (0x6c - mPopupTextY) / 2 + 0x1e;
    QueueUpload(1, 4, 0x5000, buffer, size, 1);
    OS_Free(file);
    TextManager::Get().SetColor(1, 0, 0xf);
}

BOOL MapScene::ShowCurrent() {
    u8 type = gSaveData[0].mUnk_42;
    u8 flag = gSaveData[0].mUnk_43;
    if (type == 10) {
        ShowText(3, 0x20);
        return TRUE;
    }
    u32 id = mEvent->func_020670b4(type);
    if ((id == 0) | mUnk_7c) {
        ShowText(12, 0x20);
        return TRUE;
    }
    if (flag == 0) {
        TextManager::Get().mUse4bppFont = 1;
        ShowText((u16) sMapCurrentTextIndices[id - 1], 0);
        TextManager::Get().mUse4bppFont = 0;
        return TRUE;
    }
    return FALSE;
}

static const WindowCellRecord sMapPanelWindowCells[4] = {
    {"/Game/2D/union_b/union_b_obj_window_nce.bin", 3, 0xb000, 10, 0, 1, {3, 5, 4, 0}},
    {"/Game/2D/union_b/union_b_obj_window_nce.bin", 7, 0xb000, 10, 0, 1, {6, 8, 7, 0}},
    {"/Game/2D/union_b/union_b_obj_window_nce.bin", 10, 0xb000, 10, 0, 1, {10, 10, 10, 0}},
    {"/Game/2D/union_b/union_b_obj_window_a_nce.bin", 0, 0x9000, 0, 0, 1, {0, 0, 0, 0}},
};

void MapScene::CreatePanelWindow() {
    delete mPanelWindow;
    mPanelWindow = new SelectWindow(4, sMapPanelWindowItems, -1);
    mPanelWindow->SetCells(4, sMapPanelWindowCells);
    mPanelWindow->mCursor->mSelection = -1;
    mPanelWindow->mCursor->mSelection = 1;
    mPanelWindow->mEnabled            = 0;
    mPanelVisible                           = 1;
}

void MapScene::LoadPanelText() {
    if (mText[0].mMessages == NULL) {
        ChangeDir("/");
        mText[0].mMessages       = (MessageFile *) LoadFile("font/mes_town_info.mes", 0, 0, 0, 0, 0);
        mText[0].mTextBufferSize = TextManager::Get().BufferSize(0x18, 4);
        mText[0].mTextBuffer     = OS_Alloc(mText[0].mTextBufferSize);
        MI_CpuClear16(mText[0].mTextBuffer, mText[0].mTextBufferSize);
        mText[0].mGlyphCount  = TextManager::Get().GlyphCount(0x18);
        mText[0].mGlyphBuffer = OS_Alloc((s16) mText[0].mGlyphCount * (s16) 12);
        TextManager::Get().Init(mText[0].mGlyphBuffer, 1, 0, 0x18, 4, 0x11);
    }
    TextManager &text = TextManager::Get();
    s16 height;
    s16 width;
    width             = 0;
    height            = 0;
    MessageFile *file = mText[0].mMessages;
    text.Print(file->GetText(16), file->GetLength(16), mText[0].mTextBuffer, 0x18, 4, &height, &width, 0, 0,
               2, 0x11, 0xe, 0, 0, 1, 1, 1);
    QueueUpload(SpriteManager::Get().Screen(1).mBgIndex, 4, 0xc000, mText[0].mTextBuffer, mText[0].mTextBufferSize, 0);
    TextManager::Get().SetColor(1, 0, 0xf);
}

void MapScene::DrawPanelText() {
    SpriteManager::Get().Screen(1).AddOam(mText[0].mGlyphBuffer, mText[0].mGlyphCount, 0x20, 0x38, 0, 0x180, 0xf,
                                                 0);
}

void MapScene::LoadPanelText2(int index) {
    int id;
    switch (index) {
        case 0:
            id = 0x19;
            break;
        case 1:
            id = 0x1a;
            break;
        case 2:
            id = 0x18;
            break;
        default:
            id = 0x1b;
            break;
    }
    MI_CpuClear16(mText[1].mTextBuffer, mText[1].mTextBufferSize);
    TextManager::Get().mUse4bppFont = 1;
    TextManager &text          = TextManager::Get();
    s16 height;
    s16 width;
    width             = 0;
    height            = 0;
    MessageFile *file = (MessageFile *) mText[1].mMessages;
    text.Print(file->GetText(id), file->GetLength(id), mText[1].mTextBuffer, 0x20, 4, &height, &width,
               &mTextWidth, 0, 2, 0x21, 0xe, 0, 0, 0, 1, 1);
    TextManager::Get().mUse4bppFont = 0;
    QueueUpload(SpriteManager::Get().Screen(1).mBgIndex, 4, 0xd000, mText[1].mTextBuffer, mText[1].mTextBufferSize, 0);
}

void MapScene::DrawPanelText2(s16 x) {
    u8 type   = mPage;
    u32 width = mTextWidth;
    s16 left  = x - (width >> 1);
    int right = sMapPagePos[type].mX + (width >> 1);
    if (right > 0x1fc) {
        left = left - (right - 0x1fc);
    }
    SpriteManager::Get().Screen(1).AddOam(mText[1].mGlyphBuffer, mText[1].mGlyphCount, left,
                                                 (s16) (sMapPagePos[mPage].mY - 0x28), 2, 0x1a0, 0xf, 0);
}

// NOTE: returns u32 in the original; MapScene.hpp declares `void UpdatePanelWindow()` and needs `u32`.
u32 MapScene::UpdatePanelWindow() {
    mPanelWindow->mEnabled = 1;
    u32 result             = 0;
    if (mPanelWindow->Update(TRUE) == 1) {
        result = mPanelWindow->mCursor->mSelection == 0 ? 1 : 2;
    }
    return result;
}
