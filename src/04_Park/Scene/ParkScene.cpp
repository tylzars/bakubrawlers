#include "global.h"
#include "types.h"
#include "Scene/ParkScene.hpp"
#include "Park/ParkMenu.hpp"
#include "Game/UnkStruct_02065c40.hpp"
#include "Game/UnkStruct_020d433c.hpp"
#include "Game/UnkStruct_020d43f0.hpp"
#include "Graphics/Display.hpp"
#include "Graphics/SpriteManager.hpp"
#include "System/FileSystem.hpp"
#include "UI/SelectWindow.hpp"
#include <nitro/gx.h>
#include <nitro/mi.h>
#include <nitro/os.h>


ParkScene::ParkScene() :
    SceneBase(1000, 0) {
    mUnk_0c = 0;
    mUnk_0d = 0;
    mUnk_0e = 0;
    mUnk_0f = 0;
    mWindowVisible = 0;
    mUnk_12 = 0;
    mMenu = NULL;
    mWindow = NULL;
    mUnk_2c = NULL;
    mUnk_1c = NULL;
    mGlyphBuffer = NULL;
    mPlayerCount = data_020d433c.mPlayerCount;
    SpriteManager::Get().SetFade(1, -16);
    G2_BlendNone();
}

ParkScene::~ParkScene() {
    Cleanup();
}

void ParkScene::Init() {
    InitDisplay();
    LoadBackground();
    mEvent = new UnkStruct_02065c40(2, 0, 0);
}

void ParkScene::InitDisplay() {
    GX_SetDispSelect(1);
    SpriteManager::Get().func_02026dec(1, 7);
    func_02001e68(1, 0, 1);
    func_02001ed0(0);
    G2_SetBG0Control(0, 1, 0, 1, 1);
    G2S_SetBG0Control(0, 1, 0, 1, 0);
    G2S_SetBG1Control(0, 1, 2, 4, 0);
    G2S_SetBG2Control(0, 1, 4, 5, 0);
    G2_SetBG0Offset(0, 0);
    G2S_SetBG0Offset(0, 0);
    G2S_SetBG1Offset(0, 0);
    G2S_SetBG2Offset(0, 0);
    G2_SetBG0Priority(3);
    G2S_SetBG0Priority(3);
    G2S_SetBG1Priority(0);
    G2S_SetBG2Priority(0);
    GX_SetVisiblePlane(0x11);
    GXS_SetVisiblePlane(0x15);
    G2_SetBlendBrightness(0x1f, -16);
    G2S_SetBlendBrightness(0x1f, -16);
    GX_SetVisibleWnd(0);
    GXS_SetVisibleWnd(0);
    G2_SetWndOutsidePlane(0x1f, FALSE);
    G2S_SetWndOutsidePlane(0x1f, FALSE);
    G2S_BlendNone();
}

void ParkScene::LoadWindowGraphics() {
    MI_CpuClearFast((void *) 0x6200000, 0x20000);
    MI_CpuClearFast((void *) 0x6600000, 0x20000);
    G2S_SetBG0Control(0, 0, 0, 1, 0);
    G2S_SetBG1Control(0, 1, 2, 4, 0);
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

void ParkScene::func_ov004_0221b5b4() {
    GXS_SetVisiblePlane(0x11);
    GXS_SetVisibleWnd(0);
    G2S_BlendNone();
    LoadBackground();
}

void ParkScene::LoadBackground() {
    ChangeDir("/Game/Park/");
    func_02025b64(1, 1, 0, "park_b_bg_00_ncl.bin", 0, 0, 0);
    func_02025b64(1, 11, 0, "park_b_bg_01_ncg.bin", 0, 0, 0);
    func_02025b64(1, 7, 0, "park_b_bg_01_nsc.bin", 0, 0, 0);
    func_02025b64(1, 1, 0x4000, "park_b_bg_00_ncl.bin", 0, 0, 0);
    func_02025b64(1, 13, 0, "park_b_bg_00_ncg.bin", 0, 0, 0);
    func_02025b64(1, 9, 0, "park_b_bg_00_nsc.bin", 0, 0, 0);
    ChangeDir("/");
    delete mMenu;
    mMenu = new ParkMenu(mUnk_0c);
}

void ParkScene::Cleanup() {
    delete mMenu;
    mMenu = NULL;
    delete mWindow;
    mWindow = NULL;
    if (mUnk_2c != NULL) {
        OS_Free(mUnk_2c);
        mUnk_2c = NULL;
    }
    if (mUnk_1c != NULL) {
        OS_Free(mUnk_1c);
        mUnk_1c = NULL;
    }
    if (mGlyphBuffer != NULL) {
        OS_Free(mGlyphBuffer);
        mGlyphBuffer = NULL;
    }
}

void ParkScene::Draw() {
    if (mMenu != NULL) {
        mMenu->Draw();
    }
    if (mWindowVisible == 0) {
        return;
    }
    mWindow->Draw();
    SpriteManager::Get().Screen(1).func_02027778(mGlyphBuffer, 9, 0x20, 0x28, 0, 0x360, 0xf, 0);
}

u8 ParkScene::func_ov004_0221b8c4(int index) {
    if (TestFlagBit(data_020d4970, index)) {
        return 2;
    }
    return TestFlagBit(data_020d496d, index) ? 1 : 0;
}

int ParkScene::func_ov004_0221b908(int index) {
    if (!TestFlagBit(data_020d4968, index)) {
        index = 2;
        if (!(data_020d43f0[0].mUnk_578 & 4)) {
            index = 0;
        }
    }
    return index;
}

void ParkScene::ShowWindow() {
    GXS_SetVisiblePlane(0x13);
    mWindow->mUnk_064 = 1;
    mWindowVisible = 1;
    G2S_SetWnd0Position(0x10, 0x10, 0xf0, 0xb0);
    GXS_SetVisibleWnd(1);
    G2S_SetWnd0InsidePlane(0x13, FALSE);
    G2S_SetWndOutsidePlane(0x33, FALSE);
    G2S_SetBlendBrightness(0x11, -8);
}

void ParkScene::HideWindow() {
    GXS_SetVisiblePlane(0x11);
    mWindow->mUnk_064 = 0;
    mWindowVisible = 0;
    G2S_SetBlendBrightness(0x11, 0);
    GXS_SetVisibleWnd(0);
    G2S_SetWnd0InsidePlane(0x1f, FALSE);
    G2S_SetWndOutsidePlane(0x3f, FALSE);
}
