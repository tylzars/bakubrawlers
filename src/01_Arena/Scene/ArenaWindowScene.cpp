#include "Scene/ArenaWindowScene.hpp"
#include "Game/UnkStruct_020d433c.hpp"
#include "Graphics/Display.hpp"
#include "Graphics/SpriteManager.hpp"
#include "System/FileSystem.hpp"
#include "Text/TextManager.hpp"
#include "UI/SelectWindow.hpp"
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

extern const WindowItemRecord data_ov001_02227084[];
extern const WindowCellRecord data_ov001_0222704c[];
void ArenaWindowScene::func_ov001_0221d858() {
    GX_SetDispSelect(1);
    SpriteManager::Get().func_02026dec(1, 1);
    func_02001ed0(0);
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
