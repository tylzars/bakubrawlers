#include "Scene/TutorialScene.hpp"
#include "Game/ScriptPlayer.hpp"
#include "Game/UnkStruct_020d433c.hpp"
#include "Game/SaveData.hpp"
#include "Graphics/CellAnimation.hpp"
#include "Graphics/Display.hpp"
#include "System/FileSystem.hpp"
#include "UI/MessageWindow.hpp"
#include "UI/SelectWindow.hpp"
#include <nitro/gx.h>
#include <nitro/mi.h>
#include <nitro/os.h>

static const u8 sTutorialFirstIds[8] = {0, 1, 7, 12, 14, 16, 18, 0};
static const s8 data_ov005_022200d8[8][2] = {{0, 0}, {13, 3}, {13, 3}, {0, 4}, {0, 3}, {13, 3}, {13, 3}, {0, 0}};
static const WindowItemRecord sTutorialListItems[6] = {
    {0, 5, 169, 200, 0, 0, 0, {-1, -1, -1, -1}, 0, 1},
    {1, 2, 225, 170, 30, 20, 0, {-1, -1, -1, -1}, 0, 1},
    {2, 16, 228, 48, 32, 12, 0, {-1, -1, -1, -1}, 0, 0},
    {3, 32, 228, 28, 32, 20, 0, {-1, -1, -1, -1}, 0, 3},
    {4, 64, 228, 144, 32, 20, 0, {-1, -1, -1, -1}, 0, 3},
    {5, 32768, 238, 36, 0, 0, 0, {-1, -1, -1, -1}, 0, 0},
};
static const WindowCellRecord sTutorialListCells[6] = {
    {"union_b_obj_cockpit_nce.bin", 0, 24576, 3, 2, 1, {0, 1, 1, 0}},
    {"union_b_obj_cockpit_nce.bin", 2, 24576, 3, 2, 1, {2, 3, 3, 0}},
    {"union_b_obj_scroll_nce.bin", 4, 20480, 3, 2, 1, {4, 4, 4, 0}},
    {"union_b_obj_scroll_nce.bin", 0, 20480, 3, 2, 1, {0, 1, 1, 0}},
    {"union_b_obj_scroll_nce.bin", 2, 20480, 3, 2, 1, {2, 3, 3, 0}},
    {"union_b_obj_scroll_nce.bin", 5, 20480, 3, 2, 1, {5, 5, 5, 0}},
};
// Record 2 (flag 0x8000) is skipped by the cursor and by touch: two items are selectable.
static const WindowItemRecord sTutorialThreeButtonItems[3] = {
    {0, 1, 45, 152, 78, 20, 0, {0, 0, 1, 1}, 0, 2},
    {1, 1, 133, 152, 78, 20, 0, {1, 1, 0, 0}, 0, 2},
    {2, 32768, 120, 18, 78, 20, 0, {-1, -1, -1, -1}, 0, 0},
};
static const WindowCellRecord sTutorialThreeButtonCells[3] = {
    {"/Game/2D/union_b/union_b_obj_window_nce.bin", 3, 0, 0, 0, 1, {3, 5, 4, 0}},
    {"/Game/2D/union_b/union_b_obj_window_nce.bin", 7, 0, 0, 0, 1, {6, 8, 7, 0}},
    {"/Game/2D/union_b/union_b_obj_window_nce.bin", 10, 0, 0, 0, 1, {10, 10, 10, 0}},
};

#include "Graphics/SpriteManager.hpp"
#include "System/SoundManager.hpp"
#include "System/TouchManager.hpp"
#include "Text/TextManager.hpp"
#include "System/InputManager.hpp"



extern "C" void RecordSndHeapState(u32 a);
void FillParkSlots();
void SetupFourPlayerBattle();
class TutorialScene;
BOOL func_ov005_0221fd64(TutorialScene *scene);
void InitScriptDisplay(TutorialScene *scene);
void InitWindowDisplay(TutorialScene *scene);
u8 GetTutorialId(TutorialScene *scene);

TutorialScene::TutorialScene() :
    SceneBase(1000, 0) {
    mEvent        = NULL;
    mWindow        = NULL;
    mMessageWindow = NULL;
    mMessages[0]      = NULL;
    mMessages[1]     = NULL;
    mGlyphs[0].mBuffer   = NULL;
    mGlyphs[0].mCount    = 0;
    mGlyphs[1].mBuffer  = NULL;
    mGlyphs[1].mCount   = 0;
    mState        = 0;
    mUnk_48        = 0;
    mSelection     = 0;
    mScroll        = 0;
    mUnk_4b        = 0;
    mUnk_4c        = 0;
    mUnk_4d        = 0;
    MI_CpuClear32(mCellAnims, sizeof(mCellAnims));
    for (u8 i = 0; i < 7; i++) {
        if (TestFlagBit(data_020d49ae, i)) {
            mFlags[i] = 1;
        } else {
            mFlags[i] = 0;
        }
    }
    data_020d433c.mRules   = 0x11;
    data_020d433c.mMicEnabled = gSaveData[0].mMicEnabled;
}

TutorialScene::~TutorialScene() {
    if (mMessageWindow != NULL) {
        delete mMessageWindow;
    }
    if (mWindow != NULL) {
        delete mWindow;
    }
    for (u8 i = 0; i < 4; i++) {
        if (mCellAnims[i] != NULL) {
            delete mCellAnims[i];
        }
    }
    if (mMessages[0] != NULL) {
        OS_Free(mMessages[0]);
    }
    if (mGlyphs[0].mBuffer != NULL) {
        OS_Free(mGlyphs[0].mBuffer);
    }
    if (mMessages[1] != NULL) {
        OS_Free(mMessages[1]);
    }
    if (mGlyphs[1].mBuffer != NULL) {
        OS_Free(mGlyphs[1].mBuffer);
    }
    for (u8 i = 0; i < 7; i++) {
        if (mFlags[i] != 0) {
            SetFlagBit(data_020d49ae, i);
        }
    }
}

void TutorialScene::Update() {
    switch (mState) {
        case 0: {
            BOOL a = FALSE;
            BOOL b = FALSE;
            switch (data_020d433c.mTutorialPart) {
                case 0:
                    a = TRUE;
                    if (data_020d433c.mUnk_82 == 0) {
                        b = TRUE;
                    }
                    break;
            }
            if (a) {
                SoundManager::Get().RestoreHeapState();
                SoundManager::Get().LoadAndStartSeq(1);
                RecordSndHeapState(SoundManager::Get().mHeap);
            }
            if (b) {
                mState = 1;
                return;
            }
            u8 c = data_020d433c.mUnk_86;
            BOOL d = c == 0;
            if (d | (func_ov005_0221fd64(this) == 0)) {
                data_020d433c.mTutorialPart++;
                data_020d433c.mUnk_86 = 0;
            } else if (c >= 2) {
                data_020d433c.mUnk_86 = 1;
                mUnk_4c = 1;
            }
            mState = 6;
            break;
        }
        case 1:
            InitWindowDisplay(this);
            LoadListText();
            LoadGraphics();
            SpriteManager::Get().StartFade(FADE_MODE_TO_ZERO);
            mState++;
            break;
        case 2:
            DrawList();
            if (SpriteManager::Get().IsFading()) {
                return;
            }
            mWindow->mEnabled = 1;
            mState++;
            break;
        case 3: {
            u8 r = UpdateList();
            if (r != 0) {
                if (r == 1) {
                    SoundManager::Get().StartSeqArc(0);
                    data_020d433c.mTutorialNumber = mSelection + 1;
                    data_020d433c.mTutorialPart = 1;
                    mState++;
                } else {
                    SoundManager::Get().StartSeqArc(1);
                    mUnk_4b = 1;
                    mState++;
                }
            }
            DrawList();
            break;
        }
        case 4:
            DrawList();
            SpriteManager::Get().StartFade(FADE_MODE_DOWN);
            mState++;
            break;
        case 5:
            DrawList();
            if (SpriteManager::Get().IsFading()) {
                return;
            }
            if (mUnk_4b != 0) {
                mState = 0x1c;
                return;
            }
            mState++;
            break;
        case 6: {
            BOOL x = FALSE;
            switch (GetTutorialId(this)) {
                case 0xc:
                    if (data_020d433c.mUnk_86 != 0) {
                        break;
                    }
                case 9:
                case 0x11:
                case 0x13:
                    x = TRUE;
                    break;
            }
            if (x) {
                mState = 0x10;
                return;
            }
            InitScriptDisplay(this);
            mEvent = new ScriptPlayer(6, 0, 1);
            mState++;
            break;
        }
        case 7: {
            if (mEvent->IsRunning()) {
                return;
            }
            u8 x = 0;
            if (func_ov005_0221fd64(this)) {
                x = data_020d433c.mUnk_86;
            }
            char second[0x7f];
            char first[0x7f];
            if (x != 0) {
                sprintf(first, "T%03d_%d_f", data_020d433c.mTutorialNumber, data_020d433c.mTutorialPart);
            } else {
                sprintf(first, "T%03d_%d", data_020d433c.mTutorialNumber, data_020d433c.mTutorialPart);
            }
            if (data_020d433c.mUnk_82 == 0) {
                sprintf(second, "cmd_%s_Ali.bin", first);
            } else {
                sprintf(second, "cmd_%s.bin", first);
            }
            mEvent->StartScript(second);
            SpriteManager::Get().StartFade(FADE_MODE_TO_ZERO);
            mState++;
            break;
        }
        case 8:
            if (mEvent->IsRunning()) {
                return;
            }
            mState++;
            break;
        case 9:
            SpriteManager::Get().StartFade(FADE_MODE_DOWN);
            mState++;
            break;
        case 10:
            if (SpriteManager::Get().IsFading()) {
                return;
            }
            mEvent->RequestFinish();
            mContext->mRequest = 0;
            mState++;
            break;
        case 11:
            if (mContext->mRequest == 0) {
                return;
            }
            if (mUnk_4c == 0) {
                mState = 0x10;
                return;
            }
            mUnk_4c = 0;
            mState = 0xc;
            break;
        case 12:
            InitWindowDisplay(this);
            LoadListWindow();
            SpriteManager::Get().StartFade(FADE_MODE_TO_ZERO);
            mState++;
            break;
        case 13:
            if (SpriteManager::Get().IsFading()) {
                return;
            }
            ShowText(0);
            mWindow->mEnabled = 1;
            mState++;
            break;
        case 14:
            if (mWindow->Update(TRUE)) {
                SpriteManager::Get().StartFade(FADE_MODE_DOWN);
                mState++;
            }
            SpriteManager::Get().Screen(1).AddOam(mGlyphs[1].mBuffer, mGlyphs[1].mCount, 0x20, 0x28, 2, 0x20, 0xf, 0);
            mWindow->Draw();
            break;
        case 15:
            if (SpriteManager::Get().IsFading()) {
                return;
            }
            if ((u8) mWindow->mCursor->mSelection != 0) {
                mState = 0x10;
                return;
            }
            data_020d433c.mTutorialPart++;
            data_020d433c.mUnk_86 = 0;
            mState = 6;
            break;
        case 16:
            mUnk_4d = 1;
            mState = 0x15;
            switch (data_020d433c.mTutorialNumber) {
                case 1:
                    if (data_020d433c.mTutorialPart >= 5) {
                        return;
                    }
                    if (data_020d433c.mTutorialPart < 4) {
                        mState = 0x1a;
                        return;
                    }
                    data_020d433c.mTutorialPart++;
                    data_020d433c.mUnk_86 = 0;
                    mState = 6;
                    break;
                case 2:
                    if (data_020d433c.mTutorialPart < 3) {
                        mState = 0x1a;
                    }
                    break;
                case 3:
                case 4:
                    if (data_020d433c.mTutorialPart < 2) {
                        mState = 0x1a;
                    }
                    break;
                case 5:
                case 6:
                    if (data_020d433c.mTutorialPart < 2) {
                        mState = 0x11;
                    }
                    break;
            }
            break;
        case 17:
            if (data_020d433c.mUnk_82 == 0) {
                mUnk_4d = 0;
                mState = 0x15;
                return;
            }
            InitWindowDisplay(this);
            LoadListWindow();
            SpriteManager::Get().StartFade(FADE_MODE_TO_ZERO);
            mState++;
            break;
        case 18:
            if (SpriteManager::Get().IsFading()) {
                return;
            }
            if (data_020d433c.mTutorialNumber == 5) {
                ShowText(3);
            } else {
                ShowText(4);
            }
            mWindow->mEnabled = 1;
            mState++;
            break;
        case 19:
            if (mWindow->Update(TRUE)) {
                SpriteManager::Get().StartFade(FADE_MODE_DOWN);
                mState++;
            }
            SpriteManager::Get().Screen(1).AddOam(mGlyphs[1].mBuffer, mGlyphs[1].mCount, 0x20, 0x28, 2, 0x20, 0xf, 0);
            mWindow->Draw();
            break;
        case 20:
            if (SpriteManager::Get().IsFading()) {
                return;
            }
            if ((u8) mWindow->mCursor->mSelection == 0) {
                mState = 0x1b;
                return;
            }
            mUnk_4d = 0;
            mState = 0x15;
            break;
        case 21:
            mFlags[data_020d433c.mTutorialNumber] = 1;
            if (mUnk_4d == 0) {
                mState = 0x19;
                return;
            }
            InitWindowDisplay(this);
            LoadListWindow();
            SpriteManager::Get().StartFade(FADE_MODE_TO_ZERO);
            mState++;
            break;
        case 22:
            if (SpriteManager::Get().IsFading()) {
                return;
            }
            ShowText(1);
            mWindow->mEnabled = 1;
            mState++;
            break;
        case 23:
            if (mWindow->Update(TRUE)) {
                SpriteManager::Get().StartFade(FADE_MODE_DOWN);
                mState++;
            }
            SpriteManager::Get().Screen(1).AddOam(mGlyphs[1].mBuffer, mGlyphs[1].mCount, 0x20, 0x28, 2, 0x20, 0xf, 0);
            mWindow->Draw();
            break;
        case 24:
            if (SpriteManager::Get().IsFading()) {
                return;
            }
            if ((u8) mWindow->mCursor->mSelection == 0) {
                data_020d433c.mTutorialPart = 1;
                mState = 6;
                return;
            }
            mState++;
            break;
        case 25:
            mState = 0x1c;
            if (data_020d433c.mTutorialNumber != 1) {
                return;
            }
            if (data_020d433c.mUnk_82 == 0) {
                return;
            }
            data_020d433c.mTutorialNumber = 2;
            data_020d433c.mTutorialPart = 1;
            data_020d433c.mUnk_86 = 0;
            mState = 6;
            break;
        case 26: {
            data_020d433c.mUnk_99 = 3;
            data_020d433c.mPlayerCount = 2;
            data_020d433c.mHasPartners = 0;
            data_020d433c.mCharacterIds[0] = 0x15;
            u8 idx = data_020d433c.mTutorialNumber;
            data_020d433c.mCharacterIds[1] = data_ov005_022200d8[idx][0];
            data_020d433c.mUnk_a3[1] = 0;
            data_020d433c.mPartners[0] = 0;
            data_020d433c.mPartners[1] = 1;
            data_020d433c.mFieldId = 0;
            data_020d433c.mUnk_96 = data_ov005_022200d8[idx][1];
            u8 id = GetTutorialId(this);
            if (id == 9) {
                id = 10;
            }
            data_020d433c.mTutorialId = id;
            RequestScene(SCENE_BATTLE_PLAYER_LIST);
            mFinished = 1;
            break;
        }
        case 27:
            SetupFourPlayerBattle();
            RequestScene(SCENE_BATTLE_PLAYER_LIST);
            mFinished = 1;
            break;
        case 28: {
            u8 prev = data_020d433c.mTutorialNumber;
            data_020d433c.mTutorialNumber = 0;
            data_020d433c.mTutorialPart = 0;
            data_020d433c.mTutorialId = 0;
            if (data_020d433c.mUnk_82 != 0) {
                SoundManager::Get().RestoreHeapState();
                switch (prev) {
                    case 1:
                    case 2: {
                        gSaveData[0].mUnk_42 = 0;
                        gSaveData[0].mUnk_43 = 2;
                        data_020d433c.mUnk_7d = 2;
                        u8 id = gSaveData[0].mAttribute;
                        int slot = GetBakuganEntryIndex(gSaveData, 0, 0x24, id);
                        func_020231c0(gSaveData, slot, 0x1b8);
                        SetSlotEntryIndex(gSaveData, 0, 0, slot);
                        data_020d4478[slot].mUnk_0 = 2;
                        UnkStruct_02023248 *entry = GetBakuganEntry(gSaveData, 0x24, id);
                        entry->mUnk_0 = 2;
                        entry->mUnk_3 = 1;
                        entry->mUnk_6 = 3;
                        entry->mUnk_9 = 2;
                        entry->mUnk_12 = 1;
                        SetFlagBit(data_020d49b0, id);
                        break;
                    }
                    case 3:
                    case 4:
                    case 5:
                    case 6:
                        data_020d433c.mUnk_7c = 2;
                        break;
                }
                RequestScene(prev <= 2 ? SCENE_MAP : SCENE_PARK);
                mFinished = 1;
                if (prev == 3 && prev == 4) {
                    FillParkSlots();
                }
            } else {
                if (mUnk_4b != 0) {
                    mState = 0x1d;
                } else {
                    mState = 1;
                }
            }
            break;
        }
        case 29:
            data_020d433c.mTutorialNumber = 0;
            data_020d433c.mTutorialPart = 0;
            data_020d433c.mTutorialId = 0;
            SoundManager::Get().RestoreHeapState();
            RequestScene(SCENE_TITLE);
            mFinished = 1;
            break;
    }
}

// Display registers only: SetupMode(1, 7), GX_SetGraphicsMode(1, 0, 1), main BG0 control last argument 1, sub
// planes 0x10 (InitWindowDisplay: SetupMode(1, 1), mode (1, 0, 0), BG0 on both screens, planes 0x11 on both).
void InitScriptDisplay(TutorialScene *scene) {
    GX_SetDispSelect(1);
    SpriteManager::Get().SetupMode(1, 7);
    GX_SetGraphicsMode(1, 0, 1);
    GXS_SetGraphicsMode(0);
    G2_SetBG0Control(0, 1, 0, 1, 1);
    G2_SetBG0Offset(0, 0);
    G2_SetBG0Priority(2);
    GX_SetVisiblePlane(0x11);
    GXS_SetVisiblePlane(0x10);
    G2_SetBlendBrightness(0x1f, -16);
    G2S_SetBlendBrightness(0x1f, -16);
    GX_SetVisibleWnd(0);
    GXS_SetVisibleWnd(0);
    G2_SetWndOutsidePlane(0x1f, FALSE);
    G2S_SetWndOutsidePlane(0x1f, FALSE);
    G2_BlendNone();
    G2S_BlendNone();
}

void InitWindowDisplay(TutorialScene *scene) {
    GX_SetDispSelect(1);
    SpriteManager::Get().SetupMode(1, 1);
    GX_SetGraphicsMode(1, 0, 0);
    GXS_SetGraphicsMode(0);
    G2_SetBG0Control(0, 1, 0, 1, 0);
    G2S_SetBG0Control(0, 1, 0, 1, 0);
    G2_SetBG0Offset(0, 0);
    G2S_SetBG0Offset(0, 0);
    G2_SetBG0Priority(2);
    G2S_SetBG0Priority(2);
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

void TutorialScene::LoadGraphics() {
    ChangeDir("/Game/Tutorial/bg/");
    LoadAndQueueUpload(0, UPLOAD_KIND_BG_EXT_PLTT, 0, "tutorial_u_bg_00_ncl.bin", 0, 0, 0);
    LoadAndQueueUpload(0, UPLOAD_KIND_BG0_CHAR, 0, "tutorial_u_bg_00_ncg.bin", 0, 0, 0);
    LoadAndQueueUpload(0, UPLOAD_KIND_BG0_SCR, 0, "tutorial_u_bg_00_nsc.bin", 0, 0, 0);
    LoadAndQueueUpload(1, UPLOAD_KIND_BG_EXT_PLTT, 0, "tutorial_b_bg_00_ncl.bin", 0, 0, 0);
    LoadAndQueueUpload(1, UPLOAD_KIND_BG0_CHAR, 0, "tutorial_b_bg_00_ncg.bin", 0, 0, 0);
    LoadAndQueueUpload(1, UPLOAD_KIND_BG0_SCR, 0, "tutorial_b_bg_00_nsc.bin", 0, 0, 0);
    ChangeDir("/Game/Tutorial/obj/");
    SpriteScreen &main = SpriteManager::Get().Screen(0);
    main.SetObjVramMode(3);
    main.LoadCharacters("tutorial_u_obj_00.nce_ncg1D.bin", 0, 0);
    main.LoadPalette("tutorial_u_obj_00_ncl.bin", 0, 0);
    if (mCellAnims[3] != NULL) {
        delete mCellAnims[3];
    }
    mCellAnims[3]            = new CellAnimation("tutorial_u_obj_00_nce.bin", 0, 1, 1);
    mCellAnims[3]->mCharBase = main.GetCharBase(0);
    mCellAnims[3]->mPaletteBase   = 0;
    SpriteScreen &sub    = SpriteManager::Get().Screen(1);
    sub.SetObjVramMode(3);
    sub.LoadCharacters("tutorial_b_obj_00.nce_ncg1D.bin", 0, 0);
    sub.LoadPalette("tutorial_b_obj_00_ncl.bin", 0, 0);
    for (u8 i = 0; i < 3; i++) {
        if (mCellAnims[i] != NULL) {
            delete mCellAnims[i];
        }
        mCellAnims[i]            = new CellAnimation("tutorial_b_obj_00_nce.bin", 0, 1, 1);
        mCellAnims[i]->mCharBase = sub.GetCharBase(0);
        mCellAnims[i]->mPaletteBase   = 0;
    }
    CellAnimation *cell = mCellAnims[1];
    cell->mAnimIndex       = 2;
    cell->ResetAnimation();
    cell          = mCellAnims[0];
    cell->mAnimIndex = 1;
    cell->ResetAnimation();
    cell          = mCellAnims[2];
    cell->mAnimIndex = 0;
    cell->ResetAnimation();
    ChangeDir("/Game/2D/union_b/");
    SpriteScreen &window = SpriteManager::Get().Screen(1);
    window.LoadCharacters("union_b_obj_scroll.nce_ncg1D.bin", 0x5000, 0);
    window.LoadPalette("union_b_obj_scroll_ncl.bin", 0x60, 0);
    window.LoadCharacters("union_b_obj_cockpit.nce_ncg1D.bin", 0x6000, 0);
    delete mWindow;
    mWindow = new SelectWindow(6, sTutorialListItems, -1);
    mWindow->SetCells(6, sTutorialListCells);
    mWindow->mCursor->mSelection = -1;
    mWindow->mCursor->mSelection = 0;
    mWindow->mCursor->SetupScroll(0x30, 0x90, 4, 6);
    mWindow->mEnabled = 0;
    if (data_020d433c.mTutorialNumber != 0) {
        mSelection = data_020d433c.mTutorialNumber - 1;
    } else {
        mSelection = 0;
        for (u8 i = 1; i < 7; i++) {
            if (mFlags[i] == 0) {
                mSelection = i - 1;
                break;
            }
        }
    }
    int scroll = mSelection - 3;
    if (scroll < 0) {
        scroll = 0;
    }
    mScroll = scroll;
    UpdateScroll();
    ChangeDir("/");
    delete mMessageWindow;
    mMessageWindow = new MessageWindow("mes_Tutorial", 1, 0x4000);
    mMessageWindow->func_02065bd4(0x14, 4, 2);
    mMessageWindow->SetWidth(0xd8);
    mMessageWindow->StartMessage(2, 0, 1);
}

void TutorialScene::LoadListWindow() {
    G2S_SetBG0Control(0, 1, 0, 1, 0);
    G2S_SetBG0Offset(0, 0);
    G2S_SetBG0Priority(2);
    GXS_SetVisiblePlane(0x11);
    ChangeDir("/Game/2D/union_b/");
    LoadAndQueueUpload(1, UPLOAD_KIND_BG_EXT_PLTT, 0, "union_b_bg_window_ncl.bin", 0, 0, 0);
    LoadAndQueueUpload(1, UPLOAD_KIND_BG0_CHAR, 0, "union_b_bg_window_ncg.bin", 0, 0, 0);
    LoadAndQueueUpload(1, UPLOAD_KIND_BG0_SCR, 0, "union_b_bg_window_b_nsc.bin", 0, 0, 0);
    SpriteScreen &sub = SpriteManager::Get().Screen(1);
    sub.LoadCharacters("union_b_obj_window.nce_ncg1D.bin", 0, 0);
    sub.LoadPalette("union_b_obj_window_ncl.bin", 0, 0);
    ChangeDir("/");
    if (mMessages[1] != NULL) {
        OS_Free(mMessages[1]);
    }
    if (mGlyphs[1].mBuffer != NULL) {
        OS_Free(mGlyphs[1].mBuffer);
    }
    mMessages[1]     = (MessageFile *) LoadFile("font/mes_Tutorial.mes", 0, 0, 0, 0, 0);
    mGlyphs[1].mCount   = TextManager::Get().GlyphCount(12);
    mGlyphs[1].mBuffer  = OS_Alloc((s16) mGlyphs[1].mCount * (s16) 12);
    TextManager::Get().Init(mGlyphs[1].mBuffer, 1, 0, 0x18, 4, 0x21);
    TextManager::Get().SetColor(1, 0, 0xf);
    delete mWindow;
    mWindow = new SelectWindow(3, sTutorialThreeButtonItems, -1);
    mWindow->SetCells(3, sTutorialThreeButtonCells);
    mWindow->mCursor->mSelection = -1;
    mWindow->mEnabled             = 0;
    mWindow->mCursor->mSelection = 1;
}

void TutorialScene::ShowText(int index) {
    u32 size     = TextManager::Get().BufferSize(0x18, 4);
    void *buffer = OS_Alloc(size);
    MI_CpuClear16(buffer, size);
    TextManager &text = TextManager::Get();
    s16 height;
    s16 width;
    width             = 0;
    height            = 0;
    MessageFile *file = mMessages[1];
    text.Print(file->GetText(index), file->GetLength(index), buffer, 0x18, 4, &height,
                             &width, 0, 0, 2, 0x21, 0x10, 0, 0, 1, 1, 1);
    QueueUpload(1, UPLOAD_KIND_OBJ, 0x1000, buffer, size, 1);
}

void TutorialScene::LoadListText() {
    u8 i;
    ChangeDir("/");
    mMessages[0]     = (MessageFile *) LoadFile("font/mes_Tutorial.mes", 0, 0, 0, 0, 0);
    mGlyphs[0].mCount   = TextManager::Get().GlyphCount(8);
    mGlyphs[0].mBuffer  = OS_Alloc((s16) mGlyphs[0].mCount * (s16) 12);
    TextManager::Get().Init(mGlyphs[0].mBuffer, 1, 0, 0x20, 2, 0x21);
    for (i = 0; i < 6; i++) {
        u32 size     = TextManager::Get().BufferSize(0x20, 2);
        void *buffer = OS_Alloc(size);
        MI_CpuClear16(buffer, size);
        TextManager &text = TextManager::Get();
        s16 height;
        s16 width;
        width  = 0;
        height = 0;
        MessageFile *file = mMessages[0];
        text.Print(file->GetText((u16) (i + 5)), file->GetLength((u16) (i + 5)), buffer, 0x20, 2, &height,
                   &width, 0, 0, 2, 0x21, 0, 0, 0, 1, 1, 1);
        QueueUpload(1, UPLOAD_KIND_OBJ, 0x8000 + i * 0x800, buffer, size, 1);
    }
    TextManager::Get().SetColor(1, 0, 0xf);
}

u8 TutorialScene::UpdateList() {
    u8 result = mWindow->Update(FALSE);
    u8 i;
    if (result != 0) {
        if (result == 1) {
            CellAnimation *cell = mCellAnims[1];
            cell->mAnimIndex       = 3;
            cell->ResetAnimation();
        }
        return result;
    }
    u8 prevScroll = mScroll;
    u8 scroll     = mWindow->mCursor->mScroll;
    mScroll       = scroll;
    u8 prevSel    = mSelection;
    int bottom    = scroll + 3;
    if (prevSel <= bottom) {
        if (prevSel >= scroll) {
            scroll = prevSel;
        }
        bottom = scroll;
    }
    mSelection = bottom;
    if (InputManager::Get().IsTrigger(KEY_UP)) {
        if (mSelection != 0) {
            mSelection--;
        }
        if (mScroll > mSelection) {
            mScroll = mSelection;
        }
    } else if (InputManager::Get().IsTrigger(KEY_DOWN)) {
        if (mSelection < 5) {
            mSelection++;
        }
        if (mScroll + 4 <= mSelection) {
            mScroll = mSelection - 3;
        }
    }
    if (TouchManager::Get().mReleased != 0) {
        for (i = 0; i < 4; i++) {
            if (TouchManager::Get().IsReleasedIn(0x20, (s16) (i * 32 + 0x20), 0xc0, 0x1c)) {
                u8 index = mScroll + i;
                if (mSelection == index) {
                    result = 1;
                } else {
                    mSelection = index;
                }
                break;
            }
        }
    }
    if ((prevSel != mSelection) | (prevScroll != mScroll)) {
        UpdateScroll();
        SoundManager::Get().StartSeqArc(5);
    }
    return result;
}

void TutorialScene::UpdateScroll() {
    mWindow->mCursor->SetScroll(mScroll);
    SelectWindow *window = mWindow;
    u8 y                 = window->mCursor->mScrollItemY;
    window->mCellPos[2].x = 0xe4;
    window->mCellPos[2].y = y;
    CellAnimation *cell = mCellAnims[1];
    cell->mAnimIndex       = 2;
    cell->ResetAnimation();
}

void TutorialScene::DrawList() {
    mWindow->Draw();
    mMessageWindow->Update();
    u8 base = mWindow->mCursor->mScroll;
    for (u8 i = 0; i < 4; i++) {
        u8 index = base + i;
        SpriteManager::Get().Screen(1).AddOam(mGlyphs[0].mBuffer, mGlyphs[0].mCount, 0x28, (s16) (i * 32 + 0x29), 2,
                                                     (u16) ((index * 0x800 + 0x8000) >> 7), 0xf, 0);
        if (mFlags[index + 1] != 0) {
            mCellAnims[2]->Draw(1, 8, (s16) (i * 32 + 0x24), 2);
        }
        u8 selected;
        if (mSelection != index) {
            selected = 0;
        } else {
            selected = 1;
            mCellAnims[1]->Update();
        }
        mCellAnims[selected]->Draw(1, 0x20, (s16) (i * 32 + 0x20), 2);
    }
    mCellAnims[3]->Draw(0, 0x80, 0x60, 0);
}

// sTutorialFirstIds[mTutorialNumber] + mTutorialPart - 1 (for mTutorialNumber 2: doubled, + 1 when mUnk_86).
u8 GetTutorialId(TutorialScene *scene) {
    s8 a = data_020d433c.mTutorialPart - 1;
    u8 b = data_020d433c.mTutorialNumber;
    if (b == 2) {
        a = a * 2;
        if (data_020d433c.mUnk_86 != 0) {
            a = a + 1;
        }
    }
    return sTutorialFirstIds[b] + a;
}

BOOL func_ov005_0221fd64(TutorialScene *scene) {
    switch (GetTutorialId(scene)) {
        case 3:
        case 7:
        case 8:
        case 9:
        case 10:
        case 12:
        case 14:
            return TRUE;
        default:
            return FALSE;
    }
}

// Also sets data_020d433c.mNextScene = SCENE_TUTORIAL and mUnk_99 = 3.
void SetupFourPlayerBattle() {
    data_020d433c.mUnk_99 = 3;
    data_020d433c.mPlayerCount = 4;
    u8 i;
    for (i = 0; i < 4; i++) {
        data_020d433c.mCharacterIds[i] = i;
        data_020d433c.mUnk_a3[i] = 0;
        data_020d433c.mPartners[i] = i;
    }
    data_020d433c.mNextScene = SCENE_TUTORIAL;
    switch (data_020d433c.mTutorialNumber) {
        case 5:
            data_020d433c.mHasPartners = 1;
            data_020d433c.mPartners[0] = 1;
            data_020d433c.mPartners[1] = 0;
            data_020d433c.mPartners[2] = 3;
            data_020d433c.mPartners[3] = 2;
            data_020d433c.mCharacterIds[0] = 0x15;
            data_020d433c.mCharacterIds[1] = 0;
            data_020d433c.mCharacterIds[2] = 0xe;
            data_020d433c.mCharacterIds[3] = 0xd;
            break;
        case 6:
            data_020d433c.mHasPartners = 0;
            data_020d433c.mCharacterIds[0] = 0x15;
            data_020d433c.mCharacterIds[1] = 0xd;
            data_020d433c.mCharacterIds[2] = 0xf;
            data_020d433c.mCharacterIds[3] = 0x10;
            break;
    }
}
