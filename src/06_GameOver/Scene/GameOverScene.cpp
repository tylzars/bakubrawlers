#include "Scene/GameOverScene.hpp"
#include "UI/SelectWindow.hpp"

static const WindowItemRecord sResultItems[2][3] = {
    {
        {0, 0x1, 48, 52, 160, 24, 0, {2, 1, 0, 0}, 0, WINDOW_TOUCH_RELEASE_IF_SELECTED},
        {1, 0x1, 48, 84, 160, 24, 0, {0, 2, 1, 1}, 0, WINDOW_TOUCH_RELEASE_IF_SELECTED},
        {2, 0x1, 48, 116, 160, 24, 0, {1, 0, 2, 2}, 0, WINDOW_TOUCH_RELEASE_IF_SELECTED},
    },
    {
        {0, 0x1, 48, 52, 160, 24, 0, {1, 1, 0, 0}, 0, WINDOW_TOUCH_RELEASE_IF_SELECTED},
        {1, 0x1, 48, 84, 160, 24, 0, {0, 0, 1, 1}, 0, WINDOW_TOUCH_RELEASE_IF_SELECTED},
    },
};

static const WindowCellRecord sResultCells[2][3] = {
    {
        {"/Game/GameOver/gameover_b_obj_00_nce.bin", 1, 0x0, 0, 1, 1, {0, 2, 1, 0}},
        {"/Game/GameOver/gameover_b_obj_00_nce.bin", 3, 0x0, 0, 1, 1, {3, 5, 4, 0}},
        {"/Game/GameOver/gameover_b_obj_00_nce.bin", 6, 0x0, 0, 1, 1, {6, 8, 7, 0}},
    },
    {
        {"/Game/GameOver/gameover_b_obj_00_nce.bin", 1, 0x0, 0, 1, 1, {0, 2, 1, 0}},
        {"/Game/GameOver/gameover_b_obj_00_nce.bin", 6, 0x0, 0, 1, 1, {6, 8, 7, 0}},
    },
};

static const WindowItemRecord sRetryItems[3] = {
    {0, 0x1, 45, 152, 78, 20, 0, {0, 0, 1, 1}, 0, WINDOW_TOUCH_RELEASE_IF_SELECTED},
    {1, 0x1, 133, 152, 78, 20, 0, {1, 1, 0, 0}, 0, WINDOW_TOUCH_RELEASE_IF_SELECTED},
    {2, WINDOW_ITEM_CURSOR_SKIP, 120, 18, 78, 20, 0, {-1, -1, -1, -1}, 0, 0},
};

static const WindowCellRecord sRetryCells[3] = {
    {"/Game/2D/union_b/union_b_obj_window_nce.bin", 3, 0x3000, 2, 0, 1, {3, 5, 4, 0}},
    {"/Game/2D/union_b/union_b_obj_window_nce.bin", 7, 0x3000, 2, 0, 1, {6, 8, 7, 0}},
    {"/Game/2D/union_b/union_b_obj_window_nce.bin", 10, 0x3000, 2, 0, 1, {10, 10, 10, 0}},
};

#include "Game/GameData.hpp"
#include "Graphics/CellAnimation.hpp"
#include "Graphics/Display.hpp"
#include "Graphics/SpriteManager.hpp"
#include "System/FileSystem.hpp"
#include "Text/TextManager.hpp"
#include <nitro/gx.h>
#include <nitro/mi.h>
#include <nitro/os.h>

GameOverScene::GameOverScene() :
    SceneBase(1000, 0) {
    mResultWindow   = NULL;
    mRetryWindow    = NULL;
    mMessages       = NULL;
    mTextBuffer     = NULL;
    mGlyphBuffer    = NULL;
    mTextBufferSize = 0;
    mGlyphCount     = 0;
    mState          = 0;
    mSelection      = 0;
    MI_CpuClear32(mCellAnims, sizeof(mCellAnims));
    if (gGameData.mUnk_8a_4 != 0) {
        mResultWindowKind = GAME_OVER_RESULT_WINDOW_NO_MAP;
    } else {
        mResultWindowKind = GAME_OVER_RESULT_WINDOW_WITH_MAP;
    }
}

GameOverScene::~GameOverScene() {
    if (mCellAnims[0] != NULL) {
        delete mCellAnims[0];
    }
    delete mMessages;
    if (mGlyphBuffer != NULL) {
        OS_Free(mGlyphBuffer);
    }
    if (mTextBuffer != NULL) {
        OS_Free(mTextBuffer);
    }
    if (mResultWindow != NULL) {
        delete mResultWindow;
    }
    if (mRetryWindow != NULL) {
        delete mRetryWindow;
    }
}

void GameOverScene::Update() {
    switch (mState) {
        case 0:
            InitDisplay();
            LoadGraphics();
            CreateResultWindow();
            SetupText();
            SpriteManager::Get().StartFade(FADE_MODE_TO_ZERO);
            mState++;
            break;
        case 1:
            if (!SpriteManager::Get().IsFading()) {
                mState++;
            }
            break;
        case 2: {
            mResultWindow->mEnabled = 1;
            if (mResultWindow->Update(TRUE)) {
                mSelection = mResultWindow->mCursor->mSelection;
                BOOL retry = FALSE;
                if (mResultWindowKind == GAME_OVER_RESULT_WINDOW_WITH_MAP) {
                    if (mSelection == 2) {
                        retry = TRUE;
                    }
                } else {
                    if (mSelection == 1) {
                        retry = TRUE;
                    }
                }
                if (!retry) {
                    mState = 4;
                } else {
                    ShowRetryWindow(TRUE);
                    mState++;
                }
            }
            break;
        }
        case 3:
            if (mRetryWindow->Update(TRUE)) {
                u8 choice = mRetryWindow->mCursor->mSelection;
                ShowRetryWindow(FALSE);
                if (choice == 0) {
                    mState++;
                } else {
                    CreateResultWindow();
                    mState = 2;
                }
            } else {
                mRetryWindow->Draw();
                DrawText();
            }
            break;
        case 4:
            SpriteManager::Get().StartFade(FADE_MODE_DOWN);
            mState++;
            break;
        case 5:
            if (!SpriteManager::Get().IsFading()) {
                mState++;
            }
            break;
        case 6: {
            int next;
            if (mResultWindowKind == GAME_OVER_RESULT_WINDOW_WITH_MAP) {
                switch (mSelection) {
                    case 0:
                        next = SCENE_BATTLE_PLAYER_LIST;
                        break;
                    case 1:
                        next = SCENE_MAP;
                        break;
                    default:
                        next = SCENE_TITLE;
                        break;
                }
            } else {
                next = mSelection == 0 ? SCENE_BATTLE_PLAYER_LIST : SCENE_TITLE;
            }
            if (next == SCENE_TITLE) {
                gGameData.mUnk_76 = 0;
            }
            RequestScene(next);
            mFinished = TRUE;
            return;
        }
    }
    Draw();
}

void GameOverScene::InitDisplay() {
    GX_SetDispSelect(1);
    SpriteManager::Get().SetupMode(1, 1);
    GX_SetGraphicsMode(1, 0, 0);
    GXS_SetGraphicsMode(0);
    G2_SetBG0Control(0, 1, 0, 1, 0);
    G2S_SetBG0Control(0, 1, 0, 1, 0);
    G2S_SetBG1Control(0, 1, 2, 4, 0);
    G2_SetBG0Offset(0, 0);
    G2S_SetBG0Offset(0, 0);
    G2S_SetBG1Offset(0, 0);
    G2_SetBG0Priority(0);
    G2S_SetBG0Priority(1);
    G2S_SetBG1Priority(1);
    GX_SetVisiblePlane(0x11);
    GXS_SetVisiblePlane(0x13);
    G2_SetBlendBrightness(0x1f, 0);
    G2S_SetBlendBrightness(0x1f, 0);
    GX_SetVisibleWnd(0);
    G2_SetWndOutsidePlane(0x1f, FALSE);
    GXS_SetVisibleWnd(0);
    G2S_SetWndOutsidePlane(0x1f, FALSE);
    G2_BlendNone();
    G2S_BlendNone();
}

void GameOverScene::LoadGraphics() {
    ChangeDir("/game/GameOver/");
    LoadAndQueueUpload(0, UPLOAD_KIND_BG_EXT_PLTT, 0, "gameover_u_bg_00_ncl.bin", 0, 0, 0);
    LoadAndQueueUpload(0, UPLOAD_KIND_BG0_CHAR, 0, "gameover_u_bg_00_ncg.bin", 0, 0, 0);
    LoadAndQueueUpload(0, UPLOAD_KIND_BG0_SCR, 0, "gameover_u_bg_00_nsc.bin", 0, 0, 0);
    LoadAndQueueUpload(1, UPLOAD_KIND_BG_EXT_PLTT, 0, "gameover_b_bg_00_ncl.bin", 0, 0, 0);
    LoadAndQueueUpload(1, UPLOAD_KIND_BG0_CHAR, 0, "gameover_b_bg_00_ncg.bin", 0, 0, 0);
    LoadAndQueueUpload(1, UPLOAD_KIND_BG0_SCR, 0, "gameover_b_bg_00_nsc.bin", 0, 0, 0);
    SpriteScreen &sub = SpriteManager::Get().Screen(1);
    sub.LoadCharacters("gameover_b_obj_00.nce_ncg1D.bin", 0, 0);
    sub.LoadPalette("gameover_b_obj_00_ncl.bin", 0, 0);
    SpriteScreen &main = SpriteManager::Get().Screen(0);
    main.LoadCharacters("gameover_u_obj_00.nce_ncg1D.bin", 0, 0);
    main.LoadPalette("gameover_u_obj_00_ncl.bin", 0, 0);
    mCellAnims[0]            = new CellAnimation("gameover_u_obj_00_nce.bin", 0, 1, 1);
    mCellAnims[0]->mCharBase = main.GetCharBase(0);
    mCellAnims[0]->mPaletteBase   = 0;
    mCellAnims[0]->SetAnimation(0);
    ChangeDir("/Game/2D/union_b/");
    LoadAndQueueUpload(1, UPLOAD_KIND_BG_EXT_PLTT, 0x2000, "union_b_bg_window_ncl.bin", 0, 0, 0);
    LoadAndQueueUpload(1, UPLOAD_KIND_BG1_CHAR, 0, "union_b_bg_window_ncg.bin", 0, 0, 0);
    LoadAndQueueUpload(1, UPLOAD_KIND_BG1_SCR, 0, "union_b_bg_window_b_nsc.bin", 0, 0, 0);
    SpriteScreen &window = SpriteManager::Get().Screen(1);
    window.LoadCharacters("union_b_obj_window.nce_ncg1D.bin", 0x3000, 0);
    window.LoadPalette("union_b_obj_window_ncl.bin", 0x40, 0);
    ChangeDir("/");
}

void GameOverScene::CreateResultWindow() {
    int count = mResultWindowKind == GAME_OVER_RESULT_WINDOW_WITH_MAP ? 3 : 2;
    delete mResultWindow;
    mResultWindow = new SelectWindow(count, sResultItems[mResultWindowKind], -1);
    mResultWindow->SetCells(count, sResultCells[mResultWindowKind]);
    mResultWindow->mCursor->mSelection = -1;
    mResultWindow->mEnabled            = 0;
    mResultWindow->mCursor->mSelection = 0;
}

void GameOverScene::SetupText() {
    SpriteManager::Get().Screen(1).SetObjVramMode(3);
    ChangeDir("/");
    mMessages       = (MessageFile *) LoadFile("font/mes_System.mes", 0, 0, 0, 0, 0);
    mTextBufferSize = TextManager::Get().BufferSize(0x18, 8);
    mTextBuffer     = OS_Alloc(mTextBufferSize);
    MI_CpuClear16(mTextBuffer, mTextBufferSize);
    mGlyphCount  = TextManager::Get().GlyphCount(0x30);
    mGlyphBuffer = OS_Alloc((s16) mGlyphCount * (s16) 0xc);
    TextManager::Get().Init(mGlyphBuffer, 1, 0, 0x18, 8, 0x11);
    TextManager::Get().SetColor(1, 0, 0xf);
    MI_CpuClear16(mTextBuffer, mTextBufferSize);
    TextManager &text = TextManager::Get();
    s16 height;
    s16 width;
    width             = 0;
    height            = 0;
    MessageFile *file = mMessages;
    text.Print(file->GetText(10), file->GetLength(10), mTextBuffer, 0x18, 8, &height, &width, 0, 0, 2, 0x11, 0xe, 0, 0, 1, 1, 1);
    QueueUpload(SpriteManager::Get().Screen(1).mBgIndex, UPLOAD_KIND_OBJ, 0x4000, mTextBuffer, mTextBufferSize, 0);
}

void GameOverScene::DrawText() {
    SpriteManager::Get().Screen(1).AddOam(mGlyphBuffer, mGlyphCount, 0x20, 0x38, 0, 0x80, 0xf, 0);
}

void GameOverScene::ShowRetryWindow(BOOL show) {
    delete mRetryWindow;
    mRetryWindow = NULL;
    if (show) {
        G2S_SetBG1Priority(0);
        mRetryWindow = new SelectWindow(3, sRetryItems, -1);
        mRetryWindow->SetCells(3, sRetryCells);
        mRetryWindow->mCursor->mSelection = -1;
        mRetryWindow->mCursor->mSelection = 1;
    } else {
        G2S_SetBG1Priority(1);
    }
}

void GameOverScene::Draw() {
    mCellAnims[0]->Update();
    mCellAnims[0]->Draw(0, 0x80, 0x60, 0);
    mResultWindow->Draw();
}
