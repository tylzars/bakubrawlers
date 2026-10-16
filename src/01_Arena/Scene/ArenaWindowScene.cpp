#include "Graphics/SpriteManager.hpp"
#include "System/WirelessManager.hpp"
#include "System/FrameTask.hpp"

extern void func_020219dc();
#include "Scene/ArenaWindowScene.hpp"
#include "Game/UnkStruct_020d433c.hpp"
#include "Graphics/Display.hpp"
#include "System/FileSystem.hpp"
#include "UI/SelectWindow.hpp"

static const WindowCellRecord data_ov001_0222704c[2] = {
    {"/Game/2D/union_b/union_b_obj_window_nce.bin", 1, 8192, 0, 0, 1, {0, 2, 1, 0}},
    {"/Game/2D/union_b/union_b_obj_window_nce.bin", 9, 8192, 0, 0, 1, {9, 9, 9, 0}},
};
static const WindowItemRecord data_ov001_02227084[2] = {
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
    mMenu = NULL;
    mMessages = NULL;
    mTextBuffer = NULL;
    mGlyphBuffer = NULL;
    mTextBufferSize = 0;
    mGlyphCount = 0;
    mUnk_24 = 0;
    data_020d433c.mUnk_7a = 0;
}

ArenaWindowScene::~ArenaWindowScene() {
    if (mMenu != NULL) {
        delete mMenu;
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
    switch (mUnk_24) {
        case 0: {
            func_ov001_0221d6dc();
            BOOL done = FALSE;
            if (data_020d433c.mUnk_a8 != 0) {
                WirelessManager::Get();
                if (!IsWirelessMode(&data_020c4030.mUnk_228)) {
                    mUnk_24 = 5;
                    done    = TRUE;
                }
            }
            if (!done) {
                if (data_020d433c.mUnk_88_4 != 0) {
                    done = TRUE;
                    if (data_020d433c.mUnk_87 != 0) {
                        mUnk_24 = 0xa;
                    } else {
                        mUnk_24 = 0x10;
                    }
                }
            }
            if (!done) {
                if (data_020d433c.mUnk_88_0 != 0 && data_020d433c.mUnk_87 != 0) {
                    mUnk_24 = 0xa;
                    done    = TRUE;
                }
            }
            if (!done) {
                if (data_020d433c.mUnk_89 != 0) {
                    mUnk_24 = 3;
                } else {
                    mUnk_24++;
                }
            }
            if (mUnk_24 == 0xa) {
                return;
            }
            FrameTask::Get().func_0203dc34(0, 0x10);
            break;
        }
        case 1:
            new ArenaScene();
            mContext->mUnk_12 = 0;
            mUnk_24++;
            break;
        case 2: {
            u8 r = (u8) mContext->mUnk_12;
            if (r == 0) {
                return;
            }
            if (r != 1) {
                mUnk_24 = 0x10;
                return;
            }
            data_020d433c.mUnk_87 = 0;
            mUnk_24++;
            break;
        }
        case 3:
            new FieldSelectScene(0);
            mContext->mUnk_12 = 0;
            mUnk_24++;
            break;
        case 4: {
            u8 r = (u8) mContext->mUnk_12;
            if (r == 0) {
                return;
            }
            switch (r) {
                case 1:
                    mUnk_24 = 0xe;
                    break;
                case 2:
                    if (data_020d433c.mUnk_a8 != 0) {
                        mUnk_24 = 5;
                    } else if (data_020d433c.mUnk_87 != 0) {
                        mUnk_24 = 0xa;
                    } else {
                        mUnk_24 = 1;
                    }
                    break;
                case 3:
                    mUnk_24 = 5;
                    break;
            }
            break;
        }
        case 5:
            func_ov001_0221d858();
            SpriteManager::Get().StartFade(2);
            WirelessManager::Get().func_0202edd0();
            mUnk_24++;
            break;
        case 6:
            if (SpriteManager::Get().IsFading()) {
                return;
            }
            WirelessManager::Get();
            if (data_020c4030.mUnk_228 != 0) {
                return;
            }
            data_020d433c.mUnk_a8 = 0;
            mMenu->mUnk_064       = 1;
            mUnk_24++;
            break;
        case 7:
            if (mMenu->Update(1) == 1) {
                SpriteManager::Get().StartFade(3);
                mUnk_24++;
            }
            mMenu->Draw();
            func_ov001_0221e0b4();
            break;
        case 8:
            if (SpriteManager::Get().IsFading()) {
                return;
            }
            mUnk_24++;
            break;
        case 9:
            func_020219dc();
            if (data_020d433c.mUnk_87 != 0) {
                mUnk_24 = 0xa;
                return;
            }
            if (data_020d433c.mUnk_88_4 != 0) {
                mUnk_24 = 0x10;
            } else {
                mUnk_24 = 1;
            }
            break;
        case 10:
            SpriteManager::Get().StartFade(3);
            mUnk_24++;
            break;
        case 11:
            if (SpriteManager::Get().IsFading()) {
                return;
            }
            func_ov001_0221db10();
            new SaveWindow(1, 1, 0, 0x2000, 0, 0, 0, 0, 0);
            mContext->mUnk_12     = 0;
            data_020d433c.mUnk_87 = 0;
            data_020d433c.mUnk_14_1 = 0;
            mUnk_24++;
            break;
        case 12:
            if (mContext->mUnk_12 == 0) {
                return;
            }
            FrameTask::Get().func_0203dc34(0, 0x10);
            SpriteManager::Get().StartFade(3);
            mUnk_24++;
            break;
        case 13:
            if (SpriteManager::Get().IsFading()) {
                return;
            }
            if (data_020d433c.mUnk_88_4 != 0) {
                mUnk_24 = 0x10;
            } else {
                mUnk_24 = 1;
            }
            break;
        case 14:
            FrameTask::Get().func_0203dd90();
            mUnk_24++;
            break;
        case 15:
            if (func_0203dd0c(&FrameTask::Get(), 0) != 0) {
                return;
            }
            data_020d433c.mUnk_7b = 8;
            RequestScene(5);
            mFinished = 1;
            break;
        case 16:
            RequestScene(4);
            mFinished = 1;
            break;
    }
}

void ArenaWindowScene::func_ov001_0221d6dc() {
    GX_SetDispSelect(1);
    SpriteManager::Get().func_02026dec(1, 1);
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

void ArenaWindowScene::func_ov001_0221d858() {
    GX_SetDispSelect(1);
    SpriteManager::Get().func_02026dec(1, 1);
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
    func_02025b64(1, 1, 0, "union_b_bg_window_ncl.bin", 0, 0, 0);
    func_02025b64(1, 11, 0, "union_b_bg_window_ncg.bin", 0, 0, 0);
    func_02025b64(1, 7, 0, "union_b_bg_window_b_nsc.bin", 0, 0, 0);
    SpriteScreen &sub = SpriteManager::Get().Screen(1);
    sub.LoadCharacters("union_b_obj_window.nce_ncg1D.bin", 0x2000, 0);
    sub.LoadPalette("union_b_obj_window_ncl.bin", 0, 0);
    ChangeDir("/");
    func_ov001_0221dd40();
    func_ov001_0221df64();
    if (mMenu != NULL) {
        delete mMenu;
    }
    mMenu = new SelectWindow(2, data_ov001_02227084, -1);
    mMenu->SetCells(2, data_ov001_0222704c);
    mMenu->mCursor->mSelection = -1;
    mMenu->mUnk_064            = 0;
    mMenu->mCursor->mSelection = 0;
}

void ArenaWindowScene::func_ov001_0221db10() {
    GX_SetDispSelect(1);
    SpriteManager::Get().func_02026dec(1, 1);
    G2S_SetBG0Control(0, 0, 0, 1, 0);
    G2S_SetBG1Control(0, 1, 2, 4, 0);
    G2S_SetBG0Offset(0, 0);
    G2S_SetBG1Offset(0, 0);
    G2S_SetBG0Priority(0);
    G2S_SetBG1Priority(0);
    GXS_SetVisiblePlane(0x13);
    ChangeDir("/Game/2D/union_b/");
    func_02025b64(1, 1, 0, "union_b_bg_window_ncl.bin", 0, 0, 0);
    func_02025b64(1, 11, 0, "union_b_bg_window_ncg.bin", 0, 0, 0);
    func_02025b64(1, 7, 0, "union_b_bg_window_b_nsc.bin", 0, 0, 0);
    SpriteScreen &sub = SpriteManager::Get().Screen(1);
    ChangeDir("/Game/2D/union_b/");
    sub.LoadCharacters("union_b_obj_window.nce_ncg1D.bin", 0, 0);
    sub.LoadPalette("union_b_obj_window_ncl.bin", 0, 0);
    ChangeDir("/");
}

void ArenaWindowScene::func_ov001_0221dd40() {
    SpriteManager::Get().Screen(1).func_02027624(3);
    if (mMessages == NULL) {
        ChangeDir("/");
        mMessages       = (MessageFile *) func_02023504("font/mes_BattleArenaMenu.mes", 0, 0, 0, 0, 0);
        mTextBufferSize = TextManager::Get().BufferSize(0x18, 4);
        mTextBuffer     = OS_Alloc(mTextBufferSize);
        MI_CpuClear16(mTextBuffer, mTextBufferSize);
        mGlyphCount = TextManager::Get().GlyphCount(0x18);
        mGlyphBuffer = OS_Alloc(mGlyphCount * 12);
        TextManager::Get().Init(mGlyphBuffer, 1, 0, 0x18, 4, 0x11);
    }
    TextManager::Get().SetColor(1, 0, 0xf);
}

void ArenaWindowScene::func_ov001_0221df64() {
    func_ov001_0221e15c();
    TextManager &text = TextManager::Get();
    s16 height;
    s16 width;
    width             = 0;
    height            = 0;
    MessageFile *file = mMessages;
    text.Print(file->GetText(11), file->GetLength(11), mTextBuffer, 0x18, 4, &height, &width, 0, 0, 2, 0x11, 0xe, 0, 0,
               1, 1, 1);
    func_02025c1c(SpriteManager::Get().Screen(1).mBgIndex, 4, 0, mTextBuffer, mTextBufferSize, 0);
}

void ArenaWindowScene::func_ov001_0221e0b4() {
    SpriteManager::Get().Screen(1).func_02027778(mGlyphBuffer, (u8) mGlyphCount, 0x20, 0x28, 0, 0, 0xf, 0);
}


void ArenaWindowScene::func_ov001_0221e15c() {
    if (mTextBuffer != NULL) {
        MI_CpuClear16(mTextBuffer, mTextBufferSize);
    }
}
