#include "Scene/GameOverScene.hpp"
#include "Game/UnkStruct_020d433c.hpp"
#include "Graphics/CellAnimation.hpp"
#include "Graphics/Display.hpp"
#include "Graphics/SpriteManager.hpp"
#include "System/FileSystem.hpp"
#include "Text/TextManager.hpp"
#include "UI/SelectWindow.hpp"

#include <nitro/gx.h>
#include <nitro/mi.h>
#include <nitro/os.h>

static const WindowCellRecord sRetryCells[3] = {
    {"/Game/2D/union_b/union_b_obj_window_nce.bin", 3, 0x3000, 2, 0, 1, {3, 5, 4, 0}},
    {"/Game/2D/union_b/union_b_obj_window_nce.bin", 7, 0x3000, 2, 0, 1, {6, 8, 7, 0}},
    {"/Game/2D/union_b/union_b_obj_window_nce.bin", 10, 0x3000, 2, 0, 1, {10, 10, 10, 0}},
};

static const WindowItemRecord sRetryItems[3] = {
    {0, 0x1, 45, 152, 78, 20, 0, {0, 0, 1, 1}, 0, 2},
    {1, 0x1, 133, 152, 78, 20, 0, {1, 1, 0, 0}, 0, 2},
    {2, 0x8000, 120, 18, 78, 20, 0, {-1, -1, -1, -1}, 0, 0},
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

static const WindowItemRecord sResultItems[2][3] = {
    {
        {0, 0x1, 48, 52, 160, 24, 0, {2, 1, 0, 0}, 0, 2},
        {1, 0x1, 48, 84, 160, 24, 0, {0, 2, 1, 1}, 0, 2},
        {2, 0x1, 48, 116, 160, 24, 0, {1, 0, 2, 2}, 0, 2},
    },
    {
        {0, 0x1, 48, 52, 160, 24, 0, {1, 1, 0, 0}, 0, 2},
        {1, 0x1, 48, 84, 160, 24, 0, {0, 0, 1, 1}, 0, 2},
    },
};

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
    if (data_020d433c.mUnk_8a_4 != 0) {
        mUnk_2d = 1;
    } else {
        mUnk_2d = 0;
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
            SpriteManager::Get().StartFade(2);
            mState++;
            break;
        case 1:
            if (!SpriteManager::Get().IsFading()) {
                mState++;
            }
            break;
        case 2: {
            mResultWindow->mUnk_064 = 1;
            if (mResultWindow->Update(TRUE)) {
                mSelection = mResultWindow->mCursor->mSelection;
                BOOL retry = FALSE;
                if (mUnk_2d == 0) {
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
            SpriteManager::Get().StartFade(3);
            mState++;
            break;
        case 5:
            if (!SpriteManager::Get().IsFading()) {
                mState++;
            }
            break;
        case 6: {
            int next;
            if (mUnk_2d == 0) {
                switch (mSelection) {
                    case 0:
                        next = 5;
                        break;
                    case 1:
                        next = 9;
                        break;
                    default:
                        next = 4;
                        break;
                }
            } else {
                next = mSelection == 0 ? 5 : 4;
            }
            if (next == 4) {
                data_020d433c.mUnk_76 = 0;
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
    SpriteManager::Get().func_02026dec(1, 1);
    func_02001e68(1, 0, 0);
    func_02001ed0(0);
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
    func_02025b64(0, 1, 0, "gameover_u_bg_00_ncl.bin", 0, 0, 0);
    func_02025b64(0, 11, 0, "gameover_u_bg_00_ncg.bin", 0, 0, 0);
    func_02025b64(0, 7, 0, "gameover_u_bg_00_nsc.bin", 0, 0, 0);
    func_02025b64(1, 1, 0, "gameover_b_bg_00_ncl.bin", 0, 0, 0);
    func_02025b64(1, 11, 0, "gameover_b_bg_00_ncg.bin", 0, 0, 0);
    func_02025b64(1, 7, 0, "gameover_b_bg_00_nsc.bin", 0, 0, 0);
    SpriteScreen &sub = SpriteManager::Get().Screen(1);
    sub.LoadCharacters("gameover_b_obj_00.nce_ncg1D.bin", 0, 0);
    sub.LoadPalette("gameover_b_obj_00_ncl.bin", 0, 0);
    SpriteScreen &main = SpriteManager::Get().Screen(0);
    main.LoadCharacters("gameover_u_obj_00.nce_ncg1D.bin", 0, 0);
    main.LoadPalette("gameover_u_obj_00_ncl.bin", 0, 0);
    mCellAnims[0]            = new CellAnimation("gameover_u_obj_00_nce.bin", 0, 1, 1);
    mCellAnims[0]->mCharBase = main.func_020276f4(0);
    mCellAnims[0]->mUnk_11   = 0;
    mCellAnims[0]->mUnk_0e   = 0;
    func_02029f6c();
    ChangeDir("/Game/2D/union_b/");
    func_02025b64(1, 1, 0x2000, "union_b_bg_window_ncl.bin", 0, 0, 0);
    func_02025b64(1, 12, 0, "union_b_bg_window_ncg.bin", 0, 0, 0);
    func_02025b64(1, 8, 0, "union_b_bg_window_b_nsc.bin", 0, 0, 0);
    SpriteScreen &window = SpriteManager::Get().Screen(1);
    window.LoadCharacters("union_b_obj_window.nce_ncg1D.bin", 0x3000, 0);
    window.LoadPalette("union_b_obj_window_ncl.bin", 0x40, 0);
    ChangeDir("/");
}

void GameOverScene::CreateResultWindow() {
    int count = mUnk_2d == 0 ? 3 : 2;
    delete mResultWindow;
    mResultWindow = new SelectWindow(count, sResultItems[mUnk_2d], -1);
    mResultWindow->SetCells(count, sResultCells[mUnk_2d]);
    mResultWindow->mCursor->mSelection = -1;
    mResultWindow->mUnk_064            = 0;
    mResultWindow->mCursor->mSelection = 0;
}

// non-matching: 6 words around the Print call. The original stores both size outputs before loading
// mMessages, keeping the zero in r5; identical on every 2.0 build.
void GameOverScene::SetupText() {
    SpriteManager::Get().Screen(1).func_02027624(3);
    ChangeDir("/");
    mMessages       = (MessageFile *) func_02023504("font/mes_System.mes", 0, 0, 0, 0, 0);
    mTextBufferSize = TextManager::Get().BufferSize();
    mTextBuffer     = OS_Alloc(mTextBufferSize);
    MI_CpuClear16(mTextBuffer, mTextBufferSize);
    mGlyphCount  = TextManager::Get().GlyphCount();
    mGlyphBuffer = OS_Alloc((s16) mGlyphCount * (s16) 0xc);
    TextManager::Get().Init(mGlyphBuffer, 1, 0, 0x18, 8, 0x11);
    TextManager::Get().SetColor(1, 0, 0xf);
    MI_CpuClear16(mTextBuffer, mTextBufferSize);
    TextManager &text = TextManager::Get();
    s16 height;
    s16 width;
    width                = 0;
    MessageFile *file    = mMessages;
    const u8 *message    = (const u8 *) file + file->mTextOffset;
    height               = 0;
    text.Print(message, file->mTextLength, mTextBuffer, 0x18, 8, &height, &width, 0, 0, 2, 0x11, 0xe, 0, 0, 1, 1, 1);
    func_02025c1c(SpriteManager::Get().Screen(1).mBgIndex, 4, 0x4000, mTextBuffer, mTextBufferSize, 0);
}

void GameOverScene::DrawText() {
    SpriteManager::Get().Screen(1).func_02027778(mGlyphBuffer, mGlyphCount, 0x20, 0x38, 0, 0x80, 0xf, 0);
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
