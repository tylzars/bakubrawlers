#include "Graphics/SpriteManager.hpp"
#include "System/WirelessManager.hpp"
#include "System/FrameTask.hpp"

extern void SeedRandomFromTick();
#include "Scene/ArenaWindowScene.hpp"
#include "Game/UnkStruct_020d433c.hpp"
#include "Graphics/Display.hpp"
#include "System/FileSystem.hpp"
#include "UI/SelectWindow.hpp"

static const WindowCellRecord sArenaWindowTwoButtonCells[2] = {
    {"/Game/2D/union_b/union_b_obj_window_nce.bin", 1, 8192, 0, 0, 1, {0, 2, 1, 0}},
    {"/Game/2D/union_b/union_b_obj_window_nce.bin", 9, 8192, 0, 0, 1, {9, 9, 9, 0}},
};
// Record 1 (flag 0x8000) is skipped by the cursor and by touch: one item is selectable.
static const WindowItemRecord sArenaWindowTwoButtonItems[2] = {
    {0, 1, 89, 152, 78, 20, 0, {0, 0, 0, 0}, 0, 1},
    {1, 32768, 120, 18, 78, 20, 0, {-1, -1, -1, -1}, 0, 0},
};

#include "Text/TextManager.hpp"
#include "Scene/ArenaScene.hpp"
#include "Scene/FieldSelectScene.hpp"
#include "System/FrameTask.hpp"
#include "UI/SaveWindow.hpp"
#include "System/WirelessManager.hpp"
#include "Unknown/UnkFunctions.hpp"
#include <nitro/gx.h>
#include <nitro/mi.h>
#include <nitro/os.h>

ArenaWindowScene::ArenaWindowScene() : SceneBase(1000, 0) {
    mWindow = NULL;
    mMessages = NULL;
    mTextBuffer = NULL;
    mGlyphBuffer = NULL;
    mTextBufferSize = 0;
    mGlyphCount = 0;
    mState = 0;
    data_020d433c.mUnk_7a = 0;
}

ArenaWindowScene::~ArenaWindowScene() {
    if (mWindow != NULL) {
        delete mWindow;
    }
    if (mMessages != NULL) {
        OS_Free(mMessages);
    }
    if (mTextBuffer != NULL) {
        OS_Free(mTextBuffer);
    }
    if (mGlyphBuffer != NULL) {
        OS_Free(mGlyphBuffer);
    }
}

void ArenaWindowScene::Update() {
    switch (mState) {
        case 0: {
            InitDisplay();
            BOOL done = FALSE;
            if (data_020d433c.mWireless != 0) {
                WirelessManager::Get();
                if (!IsWirelessMode(&data_020c4030.mUnk_228)) {
                    mState = 5;
                    done    = TRUE;
                }
            }
            if (!done) {
                if (data_020d433c.mUnk_88_4 != 0) {
                    done = TRUE;
                    if (data_020d433c.mUnk_87 != 0) {
                        mState = 0xa;
                    } else {
                        mState = 0x10;
                    }
                }
            }
            if (!done) {
                if (data_020d433c.mUnk_88_0 != 0 && data_020d433c.mUnk_87 != 0) {
                    mState = 0xa;
                    done    = TRUE;
                }
            }
            if (!done) {
                if (data_020d433c.mUnk_89 != 0) {
                    mState = 3;
                } else {
                    mState++;
                }
            }
            if (mState == 0xa) {
                return;
            }
            FrameTask::Get().StartChannel(0, 0x10);
            break;
        }
        case 1:
            new ArenaScene();
            mContext->mRequest = 0;
            mState++;
            break;
        case 2: {
            u8 r = (u8) mContext->mRequest;
            if (r == 0) {
                return;
            }
            if (r != ARENA_REQUEST_FIELD_SELECT) {
                mState = 0x10;
                return;
            }
            data_020d433c.mUnk_87 = 0;
            mState++;
            break;
        }
        case 3:
            new FieldSelectScene(0);
            mContext->mRequest = 0;
            mState++;
            break;
        case 4: {
            u8 r = (u8) mContext->mRequest;
            if (r == 0) {
                return;
            }
            switch (r) {
                case 1:
                    mState = 0xe;
                    break;
                case 2:
                    if (data_020d433c.mWireless != 0) {
                        mState = 5;
                    } else if (data_020d433c.mUnk_87 != 0) {
                        mState = 0xa;
                    } else {
                        mState = 1;
                    }
                    break;
                case 3:
                    mState = 5;
                    break;
            }
            break;
        }
        case 5:
            SetupWindow();
            SpriteManager::Get().StartFade(FADE_MODE_TO_ZERO);
            WirelessManager::Get().Stop();
            mState++;
            break;
        case 6:
            if (SpriteManager::Get().IsFading()) {
                return;
            }
            WirelessManager::Get();
            if (data_020c4030.mUnk_228 != 0) {
                return;
            }
            data_020d433c.mWireless = 0;
            mWindow->mEnabled       = 1;
            mState++;
            break;
        case 7:
            if (mWindow->Update(1) == 1) {
                SpriteManager::Get().StartFade(FADE_MODE_DOWN);
                mState++;
            }
            mWindow->Draw();
            DrawMessage();
            break;
        case 8:
            if (SpriteManager::Get().IsFading()) {
                return;
            }
            mState++;
            break;
        case 9:
            SeedRandomFromTick();
            if (data_020d433c.mUnk_87 != 0) {
                mState = 0xa;
                return;
            }
            if (data_020d433c.mUnk_88_4 != 0) {
                mState = 0x10;
            } else {
                mState = 1;
            }
            break;
        case 10:
            SpriteManager::Get().StartFade(FADE_MODE_DOWN);
            mState++;
            break;
        case 11:
            if (SpriteManager::Get().IsFading()) {
                return;
            }
            LoadWindowGraphics();
            new SaveWindow(1, 1, 0, 0x2000, 0, 0, 0, 0, 0);
            mContext->mRequest     = 0;
            data_020d433c.mUnk_87 = 0;
            data_020d433c.mSaveDirty = 0;
            mState++;
            break;
        case 12:
            if (mContext->mRequest == 0) {
                return;
            }
            FrameTask::Get().StartChannel(0, 0x10);
            SpriteManager::Get().StartFade(FADE_MODE_DOWN);
            mState++;
            break;
        case 13:
            if (SpriteManager::Get().IsFading()) {
                return;
            }
            if (data_020d433c.mUnk_88_4 != 0) {
                mState = 0x10;
            } else {
                mState = 1;
            }
            break;
        case 14:
            FrameTask::Get().StopAllChannels();
            mState++;
            break;
        case 15:
            if (func_0203dd0c(&FrameTask::Get(), 0) != 0) {
                return;
            }
            data_020d433c.mNextScene = SCENE_ARENA_WINDOW;
            RequestScene(SCENE_BATTLE_PLAYER_LIST);
            mFinished = 1;
            break;
        case 16:
            RequestScene(SCENE_TITLE);
            mFinished = 1;
            break;
    }
}

void ArenaWindowScene::InitDisplay() {
    GX_SetDispSelect(1);
    SpriteManager::Get().SetupMode(1, 1);
    G2_SetBG0Control(0, 1, 0, 1, 0);
    G2S_SetBG0Control(0, 1, 0, 1, 0);
    G2_SetBG0Offset(0, 0);
    G2S_SetBG0Offset(0, 0);
    G2_SetBG0Priority(0);
    G2S_SetBG0Priority(0);
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

void ArenaWindowScene::SetupWindow() {
    GX_SetDispSelect(1);
    SpriteManager::Get().SetupMode(1, 1);
    GXS_SetGraphicsMode(0);
    G2S_SetBG0Control(0, 1, 0, 1, 0);
    G2S_SetBG0Offset(0, 0);
    G2S_SetBG0Priority(0);
    GX_SetVisiblePlane(0);
    GXS_SetVisiblePlane(0x11);
    GXS_SetVisibleWnd(0);
    G2S_SetWndOutsidePlane(0x1f, FALSE);
    G2S_BlendNone();
    ChangeDir("/Game/2D/union_b/");
    LoadAndQueueUpload(1, UPLOAD_KIND_BG_EXT_PLTT, 0, "union_b_bg_window_ncl.bin", 0, 0, 0);
    LoadAndQueueUpload(1, UPLOAD_KIND_BG0_CHAR, 0, "union_b_bg_window_ncg.bin", 0, 0, 0);
    LoadAndQueueUpload(1, UPLOAD_KIND_BG0_SCR, 0, "union_b_bg_window_b_nsc.bin", 0, 0, 0);
    SpriteScreen &sub = SpriteManager::Get().Screen(1);
    sub.LoadCharacters("union_b_obj_window.nce_ncg1D.bin", 0x2000, 0);
    sub.LoadPalette("union_b_obj_window_ncl.bin", 0, 0);
    ChangeDir("/");
    LoadMessages();
    PrintMessage();
    if (mWindow != NULL) {
        delete mWindow;
    }
    mWindow = new SelectWindow(2, sArenaWindowTwoButtonItems, -1);
    mWindow->SetCells(2, sArenaWindowTwoButtonCells);
    mWindow->mCursor->mSelection = -1;
    mWindow->mEnabled            = 0;
    mWindow->mCursor->mSelection = 0;
}

void ArenaWindowScene::LoadWindowGraphics() {
    GX_SetDispSelect(1);
    SpriteManager::Get().SetupMode(1, 1);
    G2S_SetBG0Control(0, 0, 0, 1, 0);
    G2S_SetBG1Control(0, 1, 2, 4, 0);
    G2S_SetBG0Offset(0, 0);
    G2S_SetBG1Offset(0, 0);
    G2S_SetBG0Priority(0);
    G2S_SetBG1Priority(0);
    GXS_SetVisiblePlane(0x13);
    ChangeDir("/Game/2D/union_b/");
    LoadAndQueueUpload(1, UPLOAD_KIND_BG_EXT_PLTT, 0, "union_b_bg_window_ncl.bin", 0, 0, 0);
    LoadAndQueueUpload(1, UPLOAD_KIND_BG0_CHAR, 0, "union_b_bg_window_ncg.bin", 0, 0, 0);
    LoadAndQueueUpload(1, UPLOAD_KIND_BG0_SCR, 0, "union_b_bg_window_b_nsc.bin", 0, 0, 0);
    SpriteScreen &sub = SpriteManager::Get().Screen(1);
    ChangeDir("/Game/2D/union_b/");
    sub.LoadCharacters("union_b_obj_window.nce_ncg1D.bin", 0, 0);
    sub.LoadPalette("union_b_obj_window_ncl.bin", 0, 0);
    ChangeDir("/");
}

void ArenaWindowScene::LoadMessages() {
    SpriteManager::Get().Screen(1).SetObjVramMode(3);
    if (mMessages == NULL) {
        ChangeDir("/");
        mMessages       = (MessageFile *) LoadFile("font/mes_BattleArenaMenu.mes", 0, 0, 0, 0, 0);
        mTextBufferSize = TextManager::Get().BufferSize(0x18, 4);
        mTextBuffer     = OS_Alloc(mTextBufferSize);
        MI_CpuClear16(mTextBuffer, mTextBufferSize);
        mGlyphCount = TextManager::Get().GlyphCount(0x18);
        mGlyphBuffer = OS_Alloc(mGlyphCount * 12);
        TextManager::Get().Init(mGlyphBuffer, 1, 0, 0x18, 4, 0x11);
    }
    TextManager::Get().SetColor(1, 0, 0xf);
}

void ArenaWindowScene::PrintMessage() {
    ClearTextBuffer();
    TextManager &text = TextManager::Get();
    s16 height;
    s16 width;
    width             = 0;
    height            = 0;
    MessageFile *file = mMessages;
    text.Print(file->GetText(11), file->GetLength(11), mTextBuffer, 0x18, 4, &height, &width, 0, 0, 2, 0x11, 0xe, 0, 0,
               1, 1, 1);
    QueueUpload(SpriteManager::Get().Screen(1).mBgIndex, UPLOAD_KIND_OBJ, 0, mTextBuffer, mTextBufferSize, 0);
}

void ArenaWindowScene::DrawMessage() {
    SpriteManager::Get().Screen(1).AddOam(mGlyphBuffer, (u8) mGlyphCount, 0x20, 0x28, 0, 0, 0xf, 0);
}


void ArenaWindowScene::ClearTextBuffer() {
    if (mTextBuffer != NULL) {
        MI_CpuClear16(mTextBuffer, mTextBufferSize);
    }
}
