#include "Graphics/SpriteManager.hpp"
#include "Scene/OptionScene.hpp"
#include "Game/UnkStruct_020d43f0.hpp"
#include "Graphics/CellAnimation.hpp"
#include "Scene/CreditsScene.hpp"
#include "Unknown/UnkStruct_02070608.hpp"
#include "UI/SelectWindow.hpp"
#include <nitro/mi.h>
#include <nitro/os.h>
#include "Graphics/Display.hpp"
#include "System/FileSystem.hpp"
#include <nitro/gx.h>

static const WindowItemRecord data_ov000_02226340[3] = {
    {0, 0x1, 45, 136, 78, 20, 0x0, {-1, -1, 1, 1}, 0, 2},
    {1, 0x1, 133, 136, 78, 20, 0x0, {-1, -1, 0, 0}, 0, 2},
    {2, 0x8000, 120, 35, 0, 0, 0x0, {-1, -1, -1, -1}, 0, 0},
};
static const WindowCellRecord data_ov000_02226650[6] = {
    {"option_b_obj_00_nce.bin", 3, 0x0, 0, 0, 1, {3, 5, 4, 0}},
    {"option_b_obj_00_nce.bin", 6, 0x0, 0, 0, 1, {6, 8, 7, 0}},
    {"option_b_obj_00_nce.bin", 9, 0x0, 0, 0, 1, {9, 11, 10, 0}},
    {"option_b_obj_00_nce.bin", 12, 0x0, 0, 0, 1, {12, 14, 13, 0}},
    {"option_b_obj_00_nce.bin", 15, 0x0, 0, 0, 1, {15, 17, 16, 0}},
    {"/Game/2D/union_b/union_b_obj_cockpit_nce.bin", 2, 0xb000, 9, 0, 1, {2, 3, 3, 0}},
};
static const Vec2s data_ov000_02226298[4] = {
    {301, 136}, {389, 136}, {376, 35}, {0, 0},
};
static const Vec2s data_ov000_02226288[4] = {
    {-32, 0}, {-32, 0}, {-32, 0}, {32, 0},
};
static const u8 data_ov000_02226274[4] = {0x34, 0x54, 0x74, 0x0};
static const WindowItemRecord data_ov000_022262d0[2] = {
    {0, 0x5, 169, 170, 52, 20, 0x201, {-1, -1, -1, -1}, 0, 1},
    {1, 0x2, 225, 170, 30, 20, 0x201, {-1, -1, -1, -1}, 0, 1},
};
static const WindowCellRecord data_ov000_02226308[2] = {
    {"/Game/2D/union_b/union_b_obj_cockpit_nce.bin", 0, 0xb000, 9, 0, 1, {0, 1, 1, 0}},
    {"/Game/2D/union_b/union_b_obj_cockpit_nce.bin", 2, 0xb000, 9, 0, 1, {2, 3, 3, 0}},
};
static const WindowCellRecord data_ov000_02226538[4] = {
    {"option_b_obj_00_nce.bin", 24, 0x0, 0, 0, 1, {24, 26, 25, 0}},
    {"option_b_obj_00_nce.bin", 27, 0x0, 0, 0, 1, {27, 29, 28, 0}},
    {"/Game/2D/union_b/union_b_obj_cockpit_nce.bin", 0, 0xb000, 9, 0, 1, {0, 1, 1, 0}},
    {"/Game/2D/union_b/union_b_obj_cockpit_nce.bin", 2, 0xb000, 9, 0, 1, {2, 3, 3, 0}},
};
static const u8 data_ov000_02226278[6] = {0x60, 0x7c, 0x98, 0xb4, 0xd0, 0xec};
static const Vec2s data_ov000_022262bc[5] = {
    {0, 2}, {0, 0}, {-32, 0}, {-32, 0}, {-32, 0},
};
static const Vec2s data_ov000_022262a8[5] = {
    {128, 80}, {256, 0}, {256, 52}, {256, 84}, {256, 116},
};
static const u8 data_ov000_02226280[8] = {0x0, 0x24, 0x12, 0x14, 0x16, 0x13, 0x15, 0x17};
static const WindowItemRecord data_ov000_022263e8[4] = {
    {0, 0x1, 48, 64, 160, 24, 0x303, {1, 1, -1, -1}, 0, 2},
    {1, 0x1, 48, 96, 160, 24, 0x303, {0, 0, -1, -1}, 0, 2},
    {2, 0x1, 169, 170, 52, 20, 0x201, {-1, -1, -1, -1}, 0, 1},
    {3, 0x2, 225, 170, 30, 20, 0x201, {-1, -1, -1, -1}, 0, 1},
};
static const WindowCellRecord data_ov000_02226394[3] = {
    {"/Game/2D/union_b/union_b_obj_window_nce.bin", 3, 0xd000, 10, 0, 1, {3, 5, 4, 0}},
    {"/Game/2D/union_b/union_b_obj_window_nce.bin", 7, 0xd000, 10, 0, 1, {6, 8, 7, 0}},
    {"/Game/2D/union_b/union_b_obj_window_nce.bin", 10, 0xd000, 10, 0, 1, {10, 10, 10, 0}},
};
static const WindowItemRecord data_ov000_022265a8[6] = {
    {0, 0x1, 48, 38, 160, 24, 0x303, {4, 1, -1, -1}, 0, 2},
    {1, 0x1, 48, 62, 160, 24, 0x303, {0, 2, -1, -1}, 0, 2},
    {2, 0x1, 48, 86, 160, 24, 0x303, {1, 3, -1, -1}, 0, 2},
    {3, 0x1, 48, 110, 160, 24, 0x303, {2, 4, -1, -1}, 0, 2},
    {4, 0x1, 48, 134, 160, 24, 0x303, {3, 0, -1, -1}, 0, 2},
    {5, 0x2, 225, 170, 30, 20, 0x201, {-1, -1, -1, -1}, 0, 1},
};
static const WindowCellRecord data_ov000_02226458[4] = {
    {"option_b_obj_00_nce.bin", 30, 0x0, 0, 0, 1, {30, 32, 31, 0}},
    {"option_b_obj_00_nce.bin", 33, 0x0, 0, 0, 1, {33, 35, 34, 0}},
    {"/Game/2D/union_b/union_b_obj_cockpit_nce.bin", 0, 0xb000, 9, 0, 1, {0, 1, 1, 0}},
    {"/Game/2D/union_b/union_b_obj_cockpit_nce.bin", 2, 0xb000, 9, 0, 1, {2, 3, 3, 0}},
};
static const WindowItemRecord data_ov000_022264c8[4] = {
    {0, 0x1, 48, 64, 160, 24, 0x303, {1, 1, -1, -1}, 0, 2},
    {1, 0x1, 48, 96, 160, 24, 0x303, {0, 0, -1, -1}, 0, 2},
    {2, 0x1, 169, 170, 52, 20, 0x201, {-1, -1, -1, -1}, 0, 1},
    {3, 0x2, 225, 170, 30, 20, 0x201, {-1, -1, -1, -1}, 0, 1},
};
#include "System/TouchManager.hpp"
#include "Text/TextManager.hpp"
#include "System/InputManager.hpp"
#include "System/SoundManager.hpp"
#include "System/FrameTask.hpp"


OptionScene::OptionScene() :
    SceneBase(2, 0) {
    mWindow = NULL;
    mSelection = 0;
    mFile = NULL;
    mBuf0c = NULL;
    MI_CpuClear32(mCells, sizeof(mCells));
    mState = 0;
    mSaved = *(u16 *) ((u8 *) data_020d43f0 + 0x28);
}

OptionScene::~OptionScene() {
    Release();
}

void func_02022d48(UnkStruct_020d43f0 *d);
void func_02022db4(UnkStruct_020d43f0 *d);

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
        cell = mCells[i];
        if (cell != NULL) {
            delete cell;
        }
        mCells[i] = NULL;
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
            SpriteManager::Get().StartFade(2);
            mState++;
            break;
        case 2: {
            if (SpriteManager::Get().IsFading()) {
                break;
            }
            BOOL result = mWindow->Update(1);
            switch (result) {
                case 1:
                    mSelection = mWindow->GetSelectedId();
                    switch ((u8) mSelection) {
                        case 0:
                            mState = 3;
                            break;
                        case 1:
                            mState = 7;
                            break;
                        case 2:
                            mState = 0x11;
                            break;
                        case 3:
                            mState = 0xb;
                            break;
                        case 4:
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
            if (mWindow->mUnk_061 == 4) {
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
                    UnkStruct_020d43f0 *d = data_020d43f0;
                    d->mUnk_28_7 = mOld[0];
                    d->mUnk_28_10 = mOld[1];
                    d->mUnk_28_13 = mOld[2];
                    func_02022db4(d);
                    mState++;
                    break;
                }
            }
            break;
        }
        case 5:
            DrawSettings();
            mUnk_4b = 8;
            mState++;
            break;
        case 6:
            mPosY[0] -= 4;
            DrawSettings();
            if (mUnk_4b == 0) {
                func_02024764(&SoundManager::Get(), 5, 1);
                FrameTask::Get().func_0203dd74(1);
                mState = 1;
            }
            break;
        case 7:
            mWindow->Update(1);
            if (mWindow->mUnk_061 == 4) {
                BuildMenu2();
                mState++;
            }
            break;
        case 8: {
            BOOL result = mWindow->Update(1);
            DrawMenu2();
            switch (result) {
                case 1:
                    data_020d43f0[0].mUnk_28_6 = mUnk_4f == 0 ? 1 : 0;
                    data_020d43f0[0].mUnk_27_0 = data_020d43f0[0].mUnk_28_6;
                    mState++;
                    break;
                case 2:
                    mState++;
                    break;
                default:
                    if (mWindow->GetSelectedId() < 2) {
                        mUnk_4f = mWindow->GetSelectedId();
                    }
                    break;
            }
            break;
        }
        case 9:
            DrawMenu2();
            mUnk_4b = 8;
            mState++;
            break;
        case 10:
            mPosY[0] -= 4;
            DrawMenu2();
            if (mUnk_4b == 0) {
                mState = 1;
            }
            break;
        case 11:
            mWindow->Update(1);
            if (mWindow->mUnk_061 == 4) {
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
                        func_02022d48(data_020d43f0);
                        break;
                    case 1:
                        break;
                }
                mState++;
                mUnk_4b = 8;
            }
            break;
        }
        case 13:
            SlideStep();
            if (mUnk_4b == 0) {
                ResetDisplay();
                mState = 1;
            }
            break;
        case 17:
            mWindow->Update(1);
            if (mWindow->mUnk_061 == 4) {
                BuildMenu4();
                mState++;
            }
            break;
        case 18: {
            BOOL result = mWindow->Update(1);
            DrawMenu3();
            switch (result) {
                case 1:
                    data_020d43f0[0].mUnk_28_5 = mUnk_4f == 0 ? 1 : 0;
                    mState++;
                    break;
                case 2:
                    mState++;
                    break;
                default:
                    if (mWindow->GetSelectedId() < 2) {
                        mUnk_4f = mWindow->GetSelectedId();
                    }
                    break;
            }
            break;
        }
        case 19:
            DrawMenu3();
            mUnk_4b = 8;
            mState++;
            break;
        case 20:
            mPosY[0] -= 4;
            DrawMenu3();
            if (mUnk_4b == 0) {
                mState = 1;
            }
            break;
        case 14:
            SpriteManager::Get().StartFade(3);
            mState++;
            break;
        case 15:
            if (SpriteManager::Get().IsFading()) {
                break;
            }
            Release();
            new CreditsScene();
            mContext->mUnk_12 = 0;
            mState++;
            break;
        case 16:
            if (mContext->mUnk_12 == 0) {
                break;
            }
            FrameTask::Get().func_0203dc34(0, 0x10);
            mState = 0;
            break;
        case 21:
            mWindow->Update(1);
            if (mWindow->mUnk_061 == 4) {
                if (mSaved == *(u16 *) ((u8 *) data_020d43f0 + 0x28)) {
                    SpriteManager::Get().StartFade(3);
                    mState = 0x17;
                } else {
                    GX_SetVisiblePlane(0x11);
                    new UnkStruct_02070608(1, 1, 0, 0x2000, 2, 0, 0, 0xd000, 0x120);
                    mContext->mUnk_12 = 0;
                    mState++;
                }
            }
            break;
        case 22:
            if (mContext->mUnk_12 != 0) {
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
    SpriteManager::Get().func_02026dec(1, 1);
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
    func_02025b64(0, 1, 0, "option_u_bg_00_ncl.bin", 0, 0, 0);
    func_02025b64(0, 11, 0, "option_u_bg_00_ncg.bin", 0, 0, 0);
    func_02025b64(0, 7, 0, "option_u_bg_00_nsc.bin", 0, 0, 0);
    func_02025b64(1, 1, 0, "option_b_bg_00_ncl.bin", 0, 0, 0);
    func_02025b64(1, 11, 0, "option_b_bg_00_ncg.bin", 0, 0, 0);
    func_02025b64(1, 7, 0, "option_b_bg_00_nsc.bin", 0, 0, 0);
    func_02025b64(1, 1, 0x2000, "/Game/2D/union_b/union_b_bg_window_ncl.bin", 0, 0, 0);
    func_02025b64(1, 12, 0, "/Game/2D/union_b/union_b_bg_window_ncg.bin", 0, 0, 0);
    func_02025b64(1, 8, 0x800, "/Game/2D/union_b/union_b_bg_window_a_nsc.bin", 0, 0, 0);
    SpriteScreen &screen = SpriteManager::Get().Screen(1);
    screen.LoadCharacters("option_b_obj_00.nce_ncg1D.bin", 0, 0);
    screen.LoadPalette("option_b_obj_00_ncl.bin", 0, 0);
    screen.LoadCharacters("/Game/2D/union_b/union_b_obj_cockpit.nce_ncg1D.bin", 0xb000, 0);
    screen.LoadPalette("/Game/2D/union_b/union_b_obj_cockpit_ncl.bin", 0x120, 0);
    screen.LoadCharacters("/Game/2D/union_b/union_b_obj_window.nce_ncg1D.bin", 0xd000, 0);
    screen.LoadPalette("/Game/2D/union_b/union_b_obj_window_ncl.bin", 0x140, 0);
    TextManager::Get().func_0202ce44(0, 0, 0, 0);
    TextManager::Get().func_0202ce44(1, 0, 0, 0);
    mFile = (MessageFile *) func_02023504("/font/mes_option.mes", 0, 0, 0, 0, 0);
    mBufSize = TextManager::Get().BufferSize(0x18, 0x20);
    mBuf0c = OS_Alloc(mBufSize);
    u16 mapSize = TextManager::Get().BufferSize(0x18, 2);
    u16 *map = (u16 *) OS_Alloc(mapSize);
    MI_CpuClearFast(map, mapSize);
    TextManager::Get().func_0202cd48(0, map, 0, 0, 0x18, 0x18, 0x20);
    func_02025c1c(0, 8, 0, map, mapSize, 0);
    func_02025c1c(1, 9, 0x800, map, mapSize, 1);
}

void OptionScene::BuildMainMenu() {
    ClearText(0);
    delete mWindow;
    mWindow = new SelectWindow(6, data_ov000_022265a8, mSelection);
    mWindow->SetCells(6, data_ov000_02226650);
    mWindow->Update(1);
    u8 sel = mSelection;
    mWindow->mCells[sel]->SetAnimation(data_ov000_02226650[sel].mUnk_18[2]);
}

void OptionScene::BuildSettings() {
    ClearText(1);
    SpriteManager::Get();
    u8 i;
    CellAnimation *cell;
    for (i = 0; i < 5; i++) {
        cell = mCells[i];
        if (cell != NULL) {
            delete cell;
        }
        mCells[i] = new CellAnimation("option_b_obj_00_nce.bin", data_ov000_02226280[i], 1, 1);
        mPosX[i] = data_ov000_022262a8[i].x;
        mPosY[i] = data_ov000_022262a8[i].y;
    }
    UnkStruct_020d43f0 *data = data_020d43f0;
    mUnk_4e = 0;
    mCells[2]->SetAnimation(0x13);
    mUnk_4b = 8;
    mOld[0] = data->mUnk_28_7;
    mOld[1] = data->mUnk_28_10;
    mOld[2] = data->mUnk_28_13;
    mCur[0] = data->mUnk_28_7;
    mCur[1] = data->mUnk_28_10;
    mCur[2] = data->mUnk_28_13;
    mUnk_4c = -1;
    delete mWindow;
    mWindow = new SelectWindow(2, data_ov000_022262d0, -1);
    mWindow->SetCells(2, data_ov000_02226308);
    mWindow->Update(1);
}

void OptionScene::DrawSettings() {
    if (mUnk_4b != 0) {
        for (u8 i = 0; i < 5; i++) {
            mPosX[i] += data_ov000_022262bc[i].x;
            mPosY[i] += data_ov000_022262bc[i].y;
        }
        mUnk_4b--;
    }
    for (u8 i = 0; i < 3; i++) {
        mCells[1]->Draw(1, (s16) (mPosX[2 + i] + data_ov000_02226278[mCur[i]]), data_ov000_02226274[i], 0);
    }
    for (u8 i = 0; i < 5; i++) {
        mCells[i]->Update();
        mCells[i]->Draw(1, mPosX[i], mPosY[i], 0);
    }
}

void func_02022db4(UnkStruct_020d43f0 *d);


void OptionScene::UpdateSettings() {
    BOOL changed;
    u8 prev;
    prev = mUnk_4e;
    changed = FALSE;
    if (TouchManager::Get().mUnk_56) {
        if (TouchManager::Get().mUnk_57) {
            mUnk_4c = -1;
            for (u8 i = 0; i < 3; i++) {
                if (TouchManager::Get().func_02022830(0, data_ov000_02226274[i], 0x100, 0x18)) {
                    mUnk_4c = i;
                    break;
                }
            }
        }
        if (mUnk_4c < 0) {
            return;
        }
        for (u8 i = 0; i < 6; i++) {
            if (TouchManager::Get().func_02022830(data_ov000_02226278[i], data_ov000_02226274[mUnk_4c], 0x10, 0x18)) {
                mCur[mUnk_4c] = i;
            }
        }
        mUnk_4e = mUnk_4c;
    } else if (TouchManager::Get().mTouching) {
        if (mUnk_4c < 0) {
            return;
        }
        changed = TRUE;
    } else if (InputManager::Get().IsTrigger(0x40)) {
        if (mUnk_4e == 0) {
            mUnk_4e = 2;
        } else {
            mUnk_4e--;
        }
    } else if (InputManager::Get().IsTrigger(0x80)) {
        if (mUnk_4e == 2) {
            mUnk_4e = 0;
        } else {
            mUnk_4e++;
        }
    } else if (InputManager::Get().IsTrigger(0x20)) {
        mCur[mUnk_4e]--;
        if (mCur[mUnk_4e] >= 6) {
            mCur[mUnk_4e] = 0;
        }
        changed = TRUE;
    } else if (InputManager::Get().IsTrigger(0x10)) {
        mCur[mUnk_4e]++;
        if (mCur[mUnk_4e] >= 6) {
            mCur[mUnk_4e] = 5;
        }
        changed = TRUE;
    }
    if (prev != mUnk_4e) {
        mCells[prev + 2]->SetAnimation(data_ov000_02226280[prev + 2]);
        mCells[mUnk_4e + 2]->SetAnimation(data_ov000_02226280[mUnk_4e + 5]);
    }
    if (changed) {
        switch (mUnk_4e) {
            case 0:
                data_020d43f0[0].mUnk_28_7 = mCur[0];
                func_02022db4(data_020d43f0);
                break;
            case 1:
                data_020d43f0[0].mUnk_28_10 = mCur[1];
                func_02022db4(data_020d43f0);
                SoundManager::Get().func_02024754(1);
                SoundManager::Get().func_020246f8(5);
                break;
            case 2:
                data_020d43f0[0].mUnk_28_13 = mCur[2];
                func_02022db4(data_020d43f0);
                FrameTask::Get().func_0203dd74(1);
                FrameTask::Get().func_0203dc34(1, 0x183);
                break;
        }
    }
}

void OptionScene::BuildMenu2() {
    ClearText(2);
    SpriteManager::Get();
    if (mCells[0] != NULL) {
        delete mCells[0];
    }
    mCells[0] = new CellAnimation("option_b_obj_00_nce.bin", 1, 1, 1);
    mPosX[0] = 0x80;
    mPosY[0] = 0x50;
    mUnk_4b = 8;
    delete mWindow;
    mWindow = new SelectWindow(4, data_ov000_022264c8, (s8) (data_020d43f0[0].mUnk_28_6 == 0 ? 1 : 0));
    mWindow->SetCells(4, data_ov000_02226538);
    mWindow->Update(1);
    int sel = data_020d43f0[0].mUnk_28_6 == 0 ? 1 : 0;
    mWindow->mCells[(u8) sel]->SetAnimation(data_ov000_02226538[sel].mUnk_18[2]);
}

void OptionScene::DrawMenu2() {
    if (mUnk_4b != 0) {
        mPosY[0] += 2;
        mUnk_4b--;
    }
    mCells[0]->Draw(1, mPosX[0], mPosY[0], 0);
}

void OptionScene::BuildMenu3() {
    GX_SetVisiblePlane(0x11);
    delete mWindow;
    mWindow = new SelectWindow(3, data_ov000_02226340, 1);
    mWindow->SetCells(3, data_ov000_02226394);
    u8 i;
    for (i = 0; i < 3; i++) {
        mPosX[i] = data_ov000_02226298[i].x;
        s16 y = data_ov000_02226298[i].y;
        mPosY[i] = y;
        SelectWindow *w = mWindow;
        w->mCellPos[i].x = mPosX[i];
        w->mCellPos[i].y = y;
    }
    mPosX[i] = data_ov000_02226298[i].x;
    s16 y = data_ov000_02226298[i].y;
    mPosY[i] = y;
    G2S_SetBG1Offset(mPosX[i], mPosY[i]);
    G2S_SetBG2Offset(mPosX[i], mPosY[i]);
    GXS_SetVisiblePlane(0x17);
    G2_SetBlendBrightness(3, -8);
    G2S_SetBlendBrightness(1, -8);
    mWindow->Update(1);
    mUnk_4b = 8;
    MI_CpuClearFast(mBuf0c, mBufSize);
    TextManager &text = TextManager::Get();
    s16 outWidth;
    s16 outHeight;
    outHeight = 0x38;
    outWidth = 0;
    MessageFile *file = mFile;
    text.Print(file->GetText(3), file->GetLength(3), mBuf0c, 0x18, 0x18, &outWidth, &outHeight, NULL, 0, 2, 0, 0x10, 0, 0, 1, 1, 1);
    func_02025c1c(1, 0xd, 0, mBuf0c, mBufSize, 0);
}

void OptionScene::SlideStep() {
    if (mUnk_4b == 0) {
        return;
    }
    u8 i;
    for (i = 0; i < 3; i++) {
        mPosX[i] += data_ov000_02226288[i].x;
        mPosY[i] += data_ov000_02226288[i].y;
        SelectWindow *w = mWindow;
        s16 y = mPosY[i];
        w->mCellPos[i].x = mPosX[i];
        w->mCellPos[i].y = y;
    }
    mPosX[i] += data_ov000_02226288[i].x;
    mPosY[i] += data_ov000_02226288[i].y;
    G2S_SetBG1Offset(mPosX[i], mPosY[i]);
    G2S_SetBG2Offset(mPosX[i] - 0x20, mPosY[i]);
    mUnk_4b--;
}

void OptionScene::ResetDisplay() {
    G2_BlendNone();
    G2S_BlendNone();
    GX_SetVisiblePlane(0x13);
    GXS_SetVisiblePlane(0x11);
}







void OptionScene::BuildMenu4() {
    ClearText(4);
    SpriteManager::Get();
    if (mCells[0] != NULL) {
        delete mCells[0];
    }
    mCells[0] = new CellAnimation("option_b_obj_00_nce.bin", 2, 1, 1);
    mPosX[0] = 0x80;
    mPosY[0] = 0x50;
    mUnk_4b = 8;
    delete mWindow;
    mWindow = new SelectWindow(4, data_ov000_022263e8, (s8) (data_020d43f0[0].mUnk_28_5 == 0 ? 1 : 0));
    mWindow->SetCells(4, data_ov000_02226458);
    mWindow->Update(1);
    int sel = data_020d43f0[0].mUnk_28_5 == 0 ? 1 : 0;
    mWindow->mCells[(u8) sel]->SetAnimation(data_ov000_02226458[sel].mUnk_18[2]);
}





void OptionScene::DrawMenu3() {
    if (mUnk_4b != 0) {
        mPosY[0] += 2;
        mUnk_4b--;
    }
    mCells[0]->Draw(1, mPosX[0], mPosY[0], 0);
}

void OptionScene::ClearText(u32 index) {
    MI_CpuClearFast(mBuf0c, mBufSize);
    TextManager &text = TextManager::Get();
    u16 pixelWidth;
    s16 outWidth;
    s16 outHeight;
    outHeight = 0x38;
    outWidth = 0;
    MessageFile *file = mFile;
    text.Print(file->GetText(index), file->GetLength(index), mBuf0c, 0x18, 0x18, &outWidth, &outHeight, &pixelWidth, 0, 2, 0, 0x10, 0, 0, 0, 1, 1);
    func_02025c1c(0, 0xc, 0, mBuf0c, mBufSize, 0);
    G2_SetBG1Offset((pixelWidth - 0x100) / 2, 0);
}
