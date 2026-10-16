#include "Scene/TournamentScene.hpp"

#include "Unknown/UnkFunctions.hpp"
#include "Game/Random.hpp"
#include "Game/UnkStruct_02065c40.hpp"
#include "Game/UnkStruct_020d433c.hpp"
#include "Game/UnkStruct_020d43f0.hpp"
#include "Graphics/CellAnimation.hpp"
#include "Scene/TournamentSubScene.hpp"
#include "System/FrameTask.hpp"
#include "System/SoundManager.hpp"
#include "System/InputManager.hpp"
#include "Text/TextManager.hpp"
#include "Unknown/UnkStruct_0208ab40.hpp"
#include "Graphics/SpriteManager.hpp"
#include "Unknown/UnkStruct_02070608.hpp"
#include "Graphics/Display.hpp"
#include "System/FileSystem.hpp"
#include "UI/MessageWindow.hpp"
#include "UI/SelectWindow.hpp"
#include "Unknown/UnkStruct_020651bc.hpp"

#include <nitro/gx.h>
#include <nitro/mi.h>
#include <nitro/os.h>
#include <stdio.h>

struct TourPoint {
    u8 x;
    u8 y;
};

static const u8 data_ov002_02220a18[6] = {0, 1, 2, 3, 4, 5};
static const u8 data_ov002_022209fc[3] = {4, 1, 10};
static const u8 data_ov002_02220a08[3] = {0, 2, 5};
static const u8 data_ov002_02220a00[3] = {5, 2, 11};
static const u8 data_ov002_02220a04[3] = {9, 10, 8};
static const u8 data_ov002_02220a0c[3] = {1, 0, 2};
static const TourPoint data_ov002_02220b58[6][9] = {
    {{0x36, 0x3b}, {0x71, 0x53}, {0xad, 0x3b}, {0x0, 0x0}, {0x0, 0x0}, {0x0, 0x0}, {0x0, 0x0}, {0x0, 0x0}, {0x0, 0x0}},
    {{0xa, 0x37}, {0x2a, 0x37}, {0x44, 0x58}, {0x64, 0x58}, {0x7e, 0x37}, {0x9e, 0x37}, {0xb8, 0x58}, {0xd8, 0x58}, {0x0, 0x0}},
    {{0x13, 0x47}, {0x30, 0x38}, {0x30, 0x56}, {0x63, 0x47}, {0x80, 0x38}, {0x80, 0x56}, {0xb3, 0x47}, {0xd0, 0x38}, {0xd0, 0x56}},
    {{0x22, 0x40}, {0x4a, 0x50}, {0x72, 0x40}, {0x9a, 0x50}, {0xc2, 0x40}, {0x0, 0x0}, {0x0, 0x0}, {0x0, 0x0}, {0x0, 0x0}},
    {{0x12, 0x4c}, {0x52, 0x44}, {0x92, 0x4c}, {0xd2, 0x44}, {0x0, 0x0}, {0x0, 0x0}, {0x0, 0x0}, {0x0, 0x0}, {0x0, 0x0}},
    {{0x1b, 0x40}, {0x38, 0x4f}, {0x55, 0x40}, {0x72, 0x4f}, {0x8f, 0x40}, {0xac, 0x4f}, {0xc9, 0x40}, {0x0, 0x0}, {0x0, 0x0}},
};
static const u8 sTotals[6] = {3, 8, 9, 5, 4, 7};
static const u8 sCounts[6] = {1, 2, 3, 1, 1, 1};
static const WindowItemRecord data_ov002_02220bc4[6] = {
    {0, 0x8000, 88, 56, 80, 80, 0, {-1, -1, -1, -1}, 0, 1},
    {1, 0x8000, 186, 56, 80, 80, 0, {-1, -1, -1, -1}, 0, 1},
    {2, 0x8000, 284, 56, 80, 80, 0, {-1, -1, -1, -1}, 0, 1},
    {3, 0x8, 0, 72, 80, 48, 0, {-1, -1, -1, -1}, 0, 1},
    {4, 0x8, 176, 72, 80, 48, 0, {-1, -1, -1, -1}, 0, 1},
    {5, 0x2, 225, 170, 30, 20, 0, {-1, -1, -1, -1}, 0, 1},
};
static const TourPoint data_ov002_02220a2e[7] = {{0x72, 0x51}, {0x62, 0x51}, {0x63, 0x51}, {0x72, 0x51}, {0x72, 0x51}, {0x72, 0x51}, {0, 0}};
static const u8 data_ov002_02220a6c[22][3] = {
    {3, 4, 5}, {6, 7, 8}, {9, 10, 11}, {12, 13, 14}, {15, 16, 17}, {0, 0, 0}, {18, 19, 20}, {0, 1, 2},
    {21, 22, 23}, {24, 25, 26}, {27, 28, 29}, {30, 31, 32}, {33, 34, 35}, {36, 37, 38}, {39, 40, 41},
    {42, 43, 44}, {45, 46, 47}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0},
};
static const TourPoint data_ov002_02220a28[3] = {{0x80, 0x60}, {0x80, 0x60}, {0x80, 0x60}};
static const WindowCellRecord data_ov002_02220dd8[7] = {
    {"/Game/Tournament/Obj/tournament_b_obj_02_nce.bin", 1, 0x0, 0, 3, 1, {1, 2, 1, 0}},
    {"/Game/Tournament/Obj/tournament_b_obj_02_nce.bin", 3, 0x0, 0, 3, 1, {3, 4, 3, 0}},
    {"/Game/Tournament/Obj/tournament_b_obj_02_nce.bin", 5, 0x0, 0, 3, 1, {5, 6, 5, 0}},
    {"/Game/Tournament/Obj/tournament_b_obj_02_nce.bin", 7, 0x0, 0, 3, 1, {7, 8, 7, 0}},
    {"/Game/Tournament/Obj/tournament_b_obj_02_nce.bin", 9, 0x0, 0, 3, 1, {9, 10, 9, 0}},
    {"/Game/Tournament/Obj/tournament_b_obj_02_nce.bin", 11, 0x0, 0, 3, 1, {11, 12, 11, 0}},
    {"/Game/2D/union_b/union_b_obj_cockpit_nce.bin", 2, 0x4000, 3, 3, 1, {2, 3, 3, 0}},
};
static const u8 data_ov002_02220a54[1][24] = {{2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2}};
static const WindowCellRecord data_ov002_02220c6c[6] = {
    {"/Game/Tournament/Obj/tournament_b_obj_00_nce.bin", 4, 0x0, 0, 3, 1, {5, 6, 5, 0}},
    {"/Game/Tournament/Obj/tournament_b_obj_00_nce.bin", 1, 0x0, 0, 3, 1, {2, 3, 2, 0}},
    {"/Game/Tournament/Obj/tournament_b_obj_00_nce.bin", 10, 0x0, 0, 3, 1, {11, 12, 11, 0}},
    {"/Game/2D/union_b/union_b_obj_arrow_nce.bin", 0, 0x6000, 4, 3, 0, {0, 2, 0, 0}},
    {"/Game/2D/union_b/union_b_obj_arrow_nce.bin", 1, 0x6000, 4, 3, 1, {1, 3, 1, 0}},
    {"/Game/2D/union_b/union_b_obj_cockpit_nce.bin", 2, 0x4000, 3, 3, 1, {2, 3, 3, 0}},
};
static const u8 sModeTables[3][8] = {{0x12, 0x13, 0x14, 0x15, 0x16, 0x17}, {1, 3, 6, 7, 8, 9}, {6, 1, 2, 5, 0, 3}};
static const WindowItemRecord data_ov002_02220d14[7] = {
    {0, 0x1, 16, 24, 64, 42, 0, {3, 3, 2, 1}, 0, 2},
    {1, 0x1, 96, 24, 64, 42, 0, {4, 4, 0, 2}, 0, 2},
    {2, 0x1, 176, 24, 64, 42, 0, {5, 5, 1, 0}, 0, 2},
    {3, 0x1, 16, 80, 64, 42, 0, {0, 0, 5, 4}, 0, 2},
    {4, 0x1, 96, 80, 64, 42, 0, {1, 1, 3, 5}, 0, 2},
    {5, 0x1, 176, 80, 64, 42, 0, {2, 2, 4, 3}, 0, 2},
    {6, 0x2, 225, 170, 30, 20, 0, {-1, -1, -1, -1}, 0, 1},
};
static const WindowCellRecord sWindowCells[3] = {
    {"/Game/2D/union_b/union_b_obj_window_nce.bin", 3, 0x10000, 13, 0, 1, {3, 5, 4, 0}},
    {"/Game/2D/union_b/union_b_obj_window_nce.bin", 7, 0x10000, 13, 0, 1, {6, 8, 7, 0}},
    {"/Game/2D/union_b/union_b_obj_window_nce.bin", 10, 0x10000, 13, 0, 1, {10, 10, 10, 0}},
};
static const WindowItemRecord sWindowItems[3] = {
    {0, 0x1, 45, 152, 78, 20, 0, {0, 0, 1, 1}, 0, 2},
    {1, 0x1, 133, 152, 78, 20, 0, {1, 1, 0, 0}, 0, 2},
    {2, 0x8000, 120, 18, 78, 20, 0, {-1, -1, -1, -1}, 0, 0},
};

extern const u8 data_020d441a[];

 // text shown in the lower box

TournamentScene::TournamentScene() :
    SceneBase(1000, 0) {
    mObj38  = NULL;
    mEvent  = NULL;
    mMenu   = NULL;
    mWindow = NULL;
    mMessage = NULL;
    mTextWidth = 0;
    mTextHeight = 0;
    mUnk_a0 = 0;
    mUnk_a2 = 0;
    mUnk_a4 = 0;
    mUnk_a6 = 0;
    mUnk_a8 = 0;
    mUnk_aa = 0;
    mUnk_ac = 0;
    mUnk_ae = 0;
    mUnk_b0 = 0;
    mMode = 0;
    mSelection = 0;
    mUnk_b5 = 0;
    mUnk_b6 = 0;
    mUnk_b7 = 0;
    mUnk_b8 = 0;
    mUnk_b9 = 0;
    mLastIndex = 0;
    mUnk_be = 0;
    mState = 0;
    mUnk_c0 = 0;
    mUnk_c1 = 0;
    mUnk_cb = 0;
    mUnk_cc = 0;
    mUnk_cd = 0;
    mUnk_ce = 0;
    mStep = 0;
    mUnk_d0 = 0;
    mTextRows = 2;
    mUnk_d2 = 0;
    mUnk_d3 = 1;
    mUnk_d4 = 0;
    mBlend = 0;
    mUnk_d6 = 0;
    mUnk_d7 = 0;
    for (u8 i = 0; i < 5; i++) {
        mCellsA[i] = NULL;
        mCellsB[i] = NULL;
    }
    mCell20 = NULL;
    for (u8 i = 0; i < 4; i++) {
        mMessages[i]    = NULL;
        mTextBuffers[i] = NULL;
        mGlyphBuffers[i] = NULL;
    }
    if (data_020d43f0[0].mUnk_5bf_0) {
        mLastIndex = 1;
    } else {
        mLastIndex = 0;
    }
    MI_CpuClear8(mUnk_c2, 9);
    Cleanup();
}

TournamentScene::~TournamentScene() {
    Cleanup();
}

void TournamentScene::Cleanup() {
    for (u8 i = 0; i < 5; i++) {
        delete mCellsA[i];
        mCellsA[i] = NULL;
        delete mCellsB[i];
        mCellsB[i] = NULL;
    }
    delete mCell20;
    mCell20 = NULL;
    delete mObj38;
    mObj38 = NULL;
    delete mMenu;
    mMenu = NULL;
    delete mWindow;
    mWindow = NULL;
    func_ov002_0221ddb8();
    func_ov002_0221e270();
    for (u8 i = 0; i < 4; i++) {
        if (mMessages[i] != NULL) {
            OS_Free(mMessages[i]);
            mMessages[i] = NULL;
        }
        if (mTextBuffers[i] != NULL) {
            OS_Free(mTextBuffers[i]);
            mTextBuffers[i] = NULL;
        }
        if (mGlyphBuffers[i] != NULL) {
            OS_Free(mGlyphBuffers[i]);
            mGlyphBuffers[i] = NULL;
        }
    }
    delete mMessage;
    mMessage = NULL;
    MI_CpuClearFast((void *) 0x06000000, 0x80000);
    MI_CpuClearFast((void *) 0x06200000, 0x20000);
    MI_CpuClearFast((void *) 0x06400000, 0x40000);
    MI_CpuClearFast((void *) 0x06600000, 0x20000);
}

void TournamentScene::Update() {
    switch (mUnk_b0) {
        case 0:
            func_ov002_0221b424();
            func_ov002_0221c1b0();
            mEvent = new UnkStruct_02065c40(3, 0, 0);
            mUnk_b5 = 0;
            mTimer  = 0x10;
            mUnk_b0++;
            break;
        case 1:
            if (--mTimer != 0) {
                break;
            }
            mEvent->func_02066c9c(data_020d43f0[0].mUnk_42, data_020d43f0[0].mUnk_43);
            if (mEvent->func_02067954()) {
                mUnk_b0 = 5;
                break;
            }
            if (mEvent->func_0206797c()) {
                mUnk_b0 = 9;
                break;
            }
            if (mEvent->func_020678e8()) {
                SpriteManager::Get().StartFade(2);
                mUnk_b0++;
            } else {
                SpriteManager::Get().StartFade(3);
                if (data_020d43f0[0].mUnk_42 == 10) {
                    if (mEvent != NULL) {
                        mEvent->func_020676f8();
                        mEvent = NULL;
                    }
                    mUnk_b0 = 0x1d;
                } else {
                    mUnk_b0 = 0xe;
                }
            }
            break;
        case 2:
            if (SpriteManager::Get().IsFading()) {
                break;
            }
            mUnk_b0++;
            break;
        case 3:
            if (!mEvent->func_020678e8()) {
                mUnk_d3 = 1;
                SpriteManager::Get().StartFade(3);
                if (data_020d43f0[0].mUnk_42 == 10) {
                    if (mEvent != NULL) {
                        mEvent->func_020676f8();
                        mEvent = NULL;
                    }
                    mUnk_b0 = 0x1d;
                } else {
                    mUnk_b0 = 0xe;
                }
                break;
            }
            if (mEvent->func_02067938()) {
                func_ov002_0221c154();
                mEvent->func_02067944();
                break;
            }
            if (mEvent->func_02067954()) {
                SpriteManager::Get().StartSubFade(3);
                mUnk_b0 = 4;
                break;
            }
            if (mEvent->func_0206797c()) {
                SpriteManager::Get().StartSubFade(3);
                mUnk_b0 = 9;
                break;
            }
            if (func_ov002_0221d7e4()) {
                if (mUnk_d0 != 0) {
                    SpriteManager::Get().StartFade(3);
                    mUnk_b0 = 0xc;
                } else {
                    mUnk_b0 = 0x18;
                }
            }
            break;
        case 4:
            if (SpriteManager::Get().sFade[1] > 1) {
                break;
            }
            mUnk_b0++;
            break;
        case 5:
            func_ov002_0221bf88();
            new UnkStruct_02070608(1, 1, 0, 0x2000, 0, 0, 0, 0, 0);
            mContext->mUnk_12       = 0;
            data_020d433c.mUnk_14_1 = 0;
            SpriteManager::Get().StartSubFade(2);
            mUnk_b0++;
            break;
        case 6:
            if (mContext->mUnk_12 == 0) {
                break;
            }
            SpriteManager::Get().StartSubFade(3);
            mUnk_b0++;
            break;
        case 7:
            if (SpriteManager::Get().sFade[1] > 1) {
                break;
            }
            func_ov002_0221c154();
            mEvent->func_0206796c();
            mTimer = 8;
            mUnk_b0++;
            break;
        case 8:
            if (--mTimer != 0) {
                break;
            }
            if (!mEvent->func_020678e8()) {
                if (func_ov002_0221d7e4()) {
                    mUnk_b0 = 0x19;
                    break;
                }
                mUnk_be = 0;
                mUnk_b0 = 0x29;
            } else {
                SpriteManager::Get().StartFade(2);
                mUnk_b0 = 2;
            }
            break;
        case 9:
            if (SpriteManager::Get().sFade[1] > 1) {
                break;
            }
            new UnkStruct_0208ab40(mEvent->func_0206797c() - 1);
            mContext->mUnk_12 = 0;
            mUnk_b0++;
            break;
        case 10:
            if (mContext->mUnk_12 == 0) {
                break;
            }
            func_ov002_0221c154();
            mEvent->func_02067988();
            mUnk_b0++;
            break;
        case 11:
            mUnk_b0 = 2;
            break;
        case 12:
            if (SpriteManager::Get().IsFading()) {
                break;
            }
            mContext->mUnk_12 = 0;
            func_02086938();
            if (func_02086a58()) {
                new UnkStruct_0208ab40(0);
                mUnk_b0++;
            } else if (func_02086b74()) {
                new UnkStruct_0208ab40(1);
                mUnk_b0++;
            } else {
                mUnk_b0 = 0x18;
            }
            break;
        case 13:
            if (mContext->mUnk_12 != 0) {
                mUnk_b0--;
            }
            break;
        case 14:
            if (SpriteManager::Get().IsFading()) {
                break;
            }
            if (mUnk_d2 != 0) {
                mUnk_d2 = 0;
                SpriteManager::Get().StartFade(3);
                mUnk_b0 = 0x12;
            } else {
                if (mEvent != NULL) {
                    mEvent->func_020676f8();
                    mEvent = NULL;
                }
                mUnk_b0++;
            }
            if (mUnk_d3 != 0) {
                mUnk_d3 = 0;
                FrameTask::Get().func_0203dc34(0, 0x15);
            }
            break;
        case 15:
            func_ov002_0221b424();
            mEvent = new UnkStruct_02065c40(7, 0, 1);
            mUnk_b0++;
            break;
        case 16:
            if (mEvent->func_0206f9b4("mes_Im_button", 0x15, 0, 6, 10, 4, 0)) {
                SpriteManager::Get().StartFade(2);
                mUnk_b0++;
            }
            break;
        case 17:
            if (mEvent->func_020678e8()) {
                break;
            }
            SpriteManager::Get().StartFade(3);
            mUnk_b0++;
            break;
        case 18:
            if (SpriteManager::Get().IsFading()) {
                break;
            }
            func_ov002_0221e984();
            if (mEvent != NULL) {
                mEvent->func_020676f8();
                mEvent = NULL;
            }
            mUnk_b9 = 0;
            func_ov002_0221b78c(TRUE, TRUE);
            func_ov002_0221c154();
            func_ov002_0221ebe4("/font/mes_tornament.mes", 0);
            mUnk_a0 = 0x100;
            mUnk_b0++;
            break;
        case 19:
            mMenu->func_0205c7c8((s8) mSelection);
            func_ov002_0221dbd4(data_ov002_02220a04[mSelection]);
            func_ov002_0221ddf8(data_ov002_02220a08[mSelection]);
            mUnk_b5 = 1;
            SpriteManager::Get().StartFade(2);
            mUnk_b0 = 0x15;
            break;
        case 20:
            mMenu->mUnk_061 = 4;
            mMenu->func_0205c7c8((s8) mSelection);
            func_ov002_0221da5c();
            mUnk_b0++;
            break;
        case 21: {
            s16 a = mUnk_a0;
            if (a <= -0x20) {
                mUnk_a0 = a + 0x20;
            } else if (a >= 0x20) {
                mUnk_a0 = a - 0x20;
            } else {
                mUnk_a0 = 0;
            }
            u8 r = mMenu->Update(TRUE);
            switch (r) {
                case 1:
                    switch (mSelection) {
                        case 0:
                            mState = 0;
                            func_ov002_0221ef34(4);
                            mUnk_cc = 0x17;
                            mUnk_cd = 0x14;
                            mUnk_b0 = 0x24;
                            break;
                        case 2:
                            func_ov002_0221ef34(3);
                            mUnk_cc = 0x27;
                            mUnk_cd = 0x14;
                            mUnk_b0 = 0x24;
                            break;
                        default:
                            mUnk_b0 = 0x18;
                            break;
                    }
                    break;
                case 2: {
                    CellAnimation *cell = mMenu->mCells[mSelection];
                    cell->mUnk_0f       = cell->mFrames[cell->mUnk_0e].mUnk_10 - 1;
                    mSelection          = mMenu->mCursor->mSelection;
                    mUnk_b0             = 0x18;
                    break;
                }
                default:
                    if (func_ov002_0221d4b0(r)) {
                        mUnk_b0++;
                    }
                    break;
            }
            break;
        }
        case 22:
            if (mMenu->mUnk_061 != 1) {
                mMenu->Update(TRUE);
            }
            if (func_ov002_0221d848()) {
                mUnk_b0 = 0x15;
            }
            break;
        case 23:
            if (func_ov002_0221e2b0()) {
                mUnk_b0 = 0x19;
            }
            break;
        case 24:
            SpriteManager::Get().StartFade(3);
            mUnk_b0++;
            break;
        case 25:
            if (SpriteManager::Get().IsFading()) {
                break;
            }
            mUnk_b5 = 0;
            mUnk_b9 = 0;
            mUnk_b6 = 0;
            mUnk_b7 = 0;
            mUnk_b8 = 0;
            if (mEvent != NULL) {
                mEvent->func_020676f8();
                mEvent            = NULL;
                mContext->mUnk_12 = 0;
            } else {
                mContext->mUnk_12 = 1;
            }
            mUnk_b0++;
            break;
        case 26: {
            if (mContext->mUnk_12 == 0) {
                break;
            }
            Cleanup();
            BOOL go = FALSE;
            switch (mSelection) {
                case 0:
                    go      = TRUE;
                    mUnk_be = 1;
                    break;
                case 1:
                    new TournamentSubScene();
                    break;
                case 5:
                    mUnk_be = 0;
                    go      = TRUE;
                    break;
            }
            if (go) {
                mUnk_b0 = 0x29;
                break;
            }
            mContext->mUnk_12 = 0;
            mTimer            = 0x10;
            mUnk_b0++;
            break;
        }
        case 27:
            if (--mTimer != 0) {
                break;
            }
            SpriteManager::Get().StartFade(2);
            mUnk_b0++;
            break;
        case 28:
            if (mContext->mUnk_12 == 0) {
                break;
            }
            mUnk_d2 = 1;
            mUnk_b0 = 0;
            break;
        case 29:
            if (SpriteManager::Get().IsFading()) {
                break;
            }
            mMode   = 0;
            mUnk_b9 = 1;
            func_ov002_0221b78c(FALSE, FALSE);
            func_ov002_0221c4a0();
            func_ov002_0221ebe4("/font/mes_tornament.mes", 2);
            mUnk_d4 = 1;
            func_ov002_0221ddf8(data_ov002_02220a18[mMode]);
            func_ov002_0221dbd4(sModeTables[0][mMode]);
            SpriteManager::Get().StartFade(2);
            mUnk_b0 = 0x1f;
            if (mUnk_d3 != 0) {
                mUnk_d3 = 0;
                FrameTask::Get().func_0203dc34(0, 0x15);
            }
            break;
        case 30:
            mMenu->mUnk_061 = 4;
            mMenu->func_0205c7c8((s8) mMode);
            mUnk_b0++;
            break;
        case 31: {
            u8 prev = mMenu->mCursor->mSelection;
            u8 r    = mMenu->Update(TRUE);
            switch (r) {
                case 1:
                    mSelection = mMenu->mCursor->mSelection;
                    mUnk_cc    = 0x22;
                    mUnk_cd    = 0x1e;
                    mUnk_b0    = 0x24;
                    break;
                case 2:
                    mSelection = 5;
                    mUnk_b0    = 0x18;
                    break;
                default: {
                    u32 cur = (u8) mMenu->mCursor->mSelection;
                    if (cur <= 5) {
                        mSelection = cur;
                        if (prev != mSelection) {
                            func_ov002_0221f098(mSelection);
                            mUnk_b0++;
                        }
                    }
                    break;
                }
            }
            if (mUnk_d7 > 3) {
                mUnk_ae--;
                if (mUnk_ae <= 0) {
                    mUnk_ae = 200;
                    if (mUnk_d6 < (mUnk_d7 - 1) / 3) {
                        mUnk_d6++;
                        if (mUnk_d6 >= (mUnk_d7 - 1) / 3) {
                            mUnk_ae = 300;
                        }
                    } else {
                        mUnk_d6 = 0;
                    }
                    func_ov002_0221df48(data_ov002_02220a18[mMode]);
                }
            }
            break;
        }
        case 32: {
            func_ov002_0221f250();
            s16 a = mUnk_a0;
            if (a <= -0x120) {
                mUnk_a0 = a + 0x20;
                mUnk_a2 = mUnk_a0;
            } else if (a >= -0xe0) {
                mUnk_a0 = a - 0x20;
                mUnk_a2 = mUnk_a0;
            } else {
                mUnk_a0 = 0x100;
                u8 sel  = mSelection;
                mMode   = sel;
                if (mUnk_b9 != 0) {
                    if (sel == 1) {
                        sel = 0xd;
                    }
                }
                mCellsA[0]->SetAnimation(sel);
                CellAnimation *cell = mCellsA[0];
                cell->mUnk_0f       = cell->mFrames[cell->mUnk_0e].mUnk_10 - 1;
                func_ov002_0221ddf8(data_ov002_02220a18[mMode]);
                func_ov002_0221dbd4(sModeTables[0][mMode]);
                mUnk_b0++;
            }
            break;
        }
        case 33: {
            BOOL done = func_ov002_0221f250();
            s16 a     = mUnk_a0;
            if (a <= -0x20) {
                mUnk_a0 = a + 0x20;
                mUnk_a2 = mUnk_a0;
            } else if (a >= 0x20) {
                mUnk_a0 = a - 0x20;
                mUnk_a2 = mUnk_a0;
            } else {
                mUnk_a0 = 0;
                if (done) {
                    mUnk_b0 = 0x1f;
                }
            }
            break;
        }
        case 34:
            SpriteManager::Get().StartFade(3);
            mUnk_b0++;
            break;
        case 35:
            if (SpriteManager::Get().IsFading()) {
                break;
            }
            data_020d43f0[0].mUnk_42 = sModeTables[1][mMode];
            data_020d43f0[0].mUnk_43 = 0;
            data_020d43f0[0].mUnk_25 = MATH_Rand32(&data_020d42e8, 5);
            mSelection = 0;
            mUnk_b5    = 0;
            mUnk_b9    = 0;
            mUnk_b6    = 0;
            mUnk_b7    = 0;
            mUnk_b8    = 0;
            mUnk_d2    = 1;
            mUnk_b0    = 0;
            break;
        case 36:
            func_ov002_0221ead8();
            mUnk_b0++;
            break;
        case 37:
            mWindow->mUnk_061 = 4;
            mWindow->func_0205c7c8(1);
            mUnk_b0++;
            break;
        case 38:
            u8 r = mWindow->Update(TRUE);
            if (r == 1) {
                func_ov002_0221eb78();
                if (mWindow->mCursor->mSelection == 0) {
                    mUnk_b0 = mUnk_cc;
                } else {
                    mUnk_b0 = mUnk_cd;
                }
            }
            break;
        case 39:
            SpriteManager::Get().StartFade(3);
            mUnk_b0++;
            break;
        case 40:
            if (SpriteManager::Get().IsFading()) {
                break;
            }
            data_020d43f0[0].mUnk_42 = 10;
            data_020d43f0[0].mUnk_43 = 0;
            mUnk_b5                  = 0;
            mUnk_b6                  = 0;
            mUnk_b7                  = 0;
            mUnk_b8                  = 0;
            mUnk_b9                  = 0;
            mUnk_b0                  = 0x1d;
            break;
        case 41:
            if (mUnk_be != 0) {
                data_020d433c.mUnk_7b   = 0xd;
                data_020d433c.mUnk_9a   = 0x11;
                data_020d433c.mUnk_9a_0 = data_020d43f0[0].mUnk_28_6;
                if (data_020d433c.mUnk_8a_0 != 0) {
                    int idx; u8 i; u32 id = (u8) data_020d43f0[0].mUnk_1a_8; int slot; s8 pos;
                    slot = func_0202317c(data_020d43f0, 0, 0x25, id);
                    func_020231c0(data_020d43f0, slot, 0x29e);
                    idx = func_0202317c(data_020d43f0, 0, 0x24, id);
                    for (i = 0, pos = -1; i < 3; i++) {
                        if (idx == data_020d43f0[0].mUnk_00[i]) {
                            pos = i;
                            break;
                        }
                    }
                    if (pos > 0) {
                        do {
                            func_02023168(data_020d43f0, 0, (u8) pos, data_020d43f0[0].mUnk_00[pos - 1]);
                            pos--;
                        } while (pos >= 1);
                    }
                    func_02023168(data_020d43f0, 0, 0, slot);
                    data_020d4478[slot].mUnk_0 = 2;
                    UnkStruct_02023248 *entry           = func_02023248(data_020d43f0, 0x25, id);
                    entry->mUnk_0              = 3;
                    entry->mUnk_3              = 2;
                    entry->mUnk_6              = 3;
                    entry->mUnk_9              = 1;
                    entry->mUnk_12             = 2;
                    data_020d4850[func_0202317c(data_020d43f0, 0, 0x24, (u8) data_020d43f0[0].mUnk_1a_8)] = 0;
                }
                RequestScene(5);
                mFinished = TRUE;
                return;
            }
            if (mUnk_d0 != 0) {
                data_020d433c.mUnk_ad = 1;
                RequestScene(4);
                mFinished = TRUE;
                return;
            }
            RequestScene(9);
            mFinished = TRUE;
            return;
    }
    func_ov002_0221cd10();
}

void TournamentScene::func_ov002_0221b424() {
    GX_SetDispSelect(1);
    SpriteManager::Get().func_02026dec(1, 7);
    GX_SetGraphicsMode(1, 0, 1);
    GXS_SetGraphicsMode(0);
    G2S_SetBG0Control(0, 1, 0, 1, 0);
    G2_SetBG0Offset(0, 0);
    G2S_SetBG0Offset(0, 0);
    G2_SetBG0Priority(0);
    G2S_SetBG0Priority(3);
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


void TournamentScene::func_ov002_0221b5b0() {
    GX_SetDispSelect(1);
    SpriteManager::Get().func_02026dec(1, 1);
    GX_SetGraphicsMode(1, 0, 0);
    GXS_SetGraphicsMode(0);
    G2_SetBG0Control(0, 1, 0, 1, 0);
    G2_SetBG1Control(0, 1, 2, 4, 0);
    G2S_SetBG0Control(0, 1, 0, 1, 0);
    G2S_SetBG1Control(0, 1, 2, 5, 0);
    G2_SetBG0Offset(0, 0);
    G2_SetBG1Offset(0, 0);
    G2S_SetBG0Offset(0, 0);
    G2S_SetBG1Offset(0, 0);
    G2_SetBG0Priority(3);
    G2_SetBG1Priority(3);
    G2S_SetBG0Priority(3);
    G2S_SetBG1Priority(0);
    GX_SetVisiblePlane(0x11);
    GXS_SetVisiblePlane(0x11);
    G2_SetBlendBrightness(0x1f, 0);
    G2S_SetBlendBrightness(0x1f, 0);
    GX_SetVisibleWnd(0);
    GXS_SetVisibleWnd(0);
    G2_SetWndOutsidePlane(0x1f, FALSE);
    G2S_SetWndOutsidePlane(0x1f, FALSE);
    G2_BlendNone();
    G2S_BlendNone();
}

void TournamentScene::func_ov002_0221b78c(BOOL a, BOOL b) {
    func_ov002_0221b5b0();
    MI_CpuClearFast((void *) 0x06200000, 0x20000);
    MI_CpuClearFast((void *) 0x06600000, 0x20000);
    ChangeDir("/game/Tournament/Bg/");
    if (b) {
        char name[0x40];
        sprintf(name, "tournament_u_bg_0%d_nsc.bin", sModeTables[2][mMode]);
        func_02025b64(0, 7, 0, name, 0, 0, 0);
        sprintf(name, "tournament_u_bg_0%d_ncg.bin", sModeTables[2][mMode]);
        func_02025b64(0, 0xb, 0, name, 0, 0, 0);
        sprintf(name, "tournament_u_bg_0%d_ncl.bin", sModeTables[2][mMode]);
        func_02025b64(0, 1, 0, name, 0, 0, 0);
    } else {
        func_02025b64(0, 7, 0, "tournament_u_bg_06_nsc.bin", 0, 0, 0);
        func_02025b64(0, 0xb, 0, "tournament_u_bg_06_ncg.bin", 0, 0, 0);
        func_02025b64(0, 1, 0, "tournament_u_bg_06_ncl.bin", 0, 0, 0);
    }
    u8 idx;
    int count = sCounts[mMode];
    int x     = data_020d43f0[0].mUnk_43;
    idx       = x * count;
    if (x >= sTotals[mMode] / count - 1) {
        x = 6;
    }
    ChangeDir("/Game/Tournament/Obj/");
    SpriteScreen &screen = SpriteManager::Get().Screen(0);
    screen.LoadCharacters("tournament_u_obj_00.nce_ncg1D.bin", 0, 0);
    screen.LoadPalette("tournament_u_obj_00_ncl.bin", 0, 0);
    screen.LoadCharacters("tournament_u_obj_01.nce_ncg1D.bin", 0x10000, 0);
    screen.LoadPalette("tournament_u_obj_01_ncl.bin", 0x20, 0);
    delete mCellsA[0];
    mCellsA[0]            = new CellAnimation("tournament_u_obj_01_nce.bin", mMode, 1, 1);
    mCellsA[0]->mCharBase = screen.func_020276f4(0x10000);
    mCellsA[0]->mUnk_11   = 1;
    if (mUnk_b9 != 0 && mMode == 1) {
        mCellsA[0]->SetAnimation(0xd);
    }
    delete mCellsA[1];
    mCellsA[1]            = new CellAnimation("tournament_u_obj_00_nce.bin", (u8) x, 1, 1);
    mCellsA[1]->mCharBase = screen.func_020276f4(0);
    mCellsA[1]->mUnk_11   = 0;
    mCellsA[1]->func_02029fb8(5);
    delete mCellsA[3];
    u8 sel = a != 0 ? 6 : 8;
    mCellsA[3]            = new CellAnimation("tournament_u_obj_01_nce.bin", sel, 1, 1);
    mCellsA[3]->mCharBase = screen.func_020276f4(0x10000);
    mCellsA[3]->mUnk_11   = 1;
    mCellsA[3]->func_02029fb8(0);
    delete mCellsA[4];
    mCellsA[4]            = new CellAnimation("tournament_u_obj_01_nce.bin", 7, 1, 1);
    mCellsA[4]->mCharBase = screen.func_020276f4(0x10000);
    mCellsA[4]->mUnk_11   = 1;
    mCellsA[4]->func_02029fb8(0);
    delete mCellsB[0];
    mCellsB[0]            = new CellAnimation("tournament_u_obj_01_nce.bin", 0xa, 1, 1);
    mCellsB[0]->mCharBase = screen.func_020276f4(0x10000);
    mCellsB[0]->func_02029fb8(0);
    delete mCellsB[1];
    mCellsB[1]            = new CellAnimation("tournament_u_obj_00_nce.bin", (u8) (x + 7), 1, 1);
    mCellsB[1]->mCharBase = screen.func_020276f4(0);
    mCellsB[1]->func_02029fb8(5);
    delete mCellsB[3];
    mCellsB[3]            = new CellAnimation("tournament_u_obj_01_nce.bin", 0xb, 1, 1);
    mCellsB[3]->mCharBase = screen.func_020276f4(0x10000);
    mCellsB[3]->func_02029fb8(0);
    ChangeDir("/");
    if (a) {
        delete mObj38;
        mObj38 = new UnkStruct_020651bc(0x19000, 7, 0);
        u8 i = 0;
        do {
            mObj38->func_02065204(i, (s8) mUnk_c2[idx + i], 0, 0);
            mObj38->func_020654f8(i, 1);
            mObj38->func_0206547c(i, data_ov002_02220b58[mMode][idx + i].x, data_ov002_02220b58[mMode][idx + i].y,
                                  0);
            i++;
        } while (i < sCounts[mMode]);
        if (mMode == 1) {
            mObj38->func_02065204(3, (s8) mUnk_cb, 0, 0);
            mObj38->func_020654f8(3, 1);
            mObj38->func_0206547c(3, 8, 0xa, 0);
        }
    }
    ChangeDir("/story/");
    screen.LoadCharacters("storytalk_ob_128k.nce_ncg1D.bin", 0x1a000, 0);
    screen.LoadPalette("storytalk_ob_ncl.bin", 0x160, 0);
    delete mCellsA[2];
    mCellsA[2]            = new CellAnimation("storytalk_ob_128k_nce.bin", 0, 1, 1);
    mCellsA[2]->mCharBase = screen.func_020276f4(0x1a000);
    mCellsA[2]->mUnk_11   = 0xb;
    mCellsA[2]->func_02029fb8(0);
    mUnk_b6 = 1;
    ChangeDir("/");
}

void TournamentScene::func_ov002_0221bf88() {
    MI_CpuClearFast((void *) 0x06200000, 0x20000);
    MI_CpuClearFast((void *) 0x06600000, 0x20000);
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
    mUnk_b5 = 0;
}

void TournamentScene::func_ov002_0221c154() {
    G2S_SetBG0Control(0, 1, 0, 1, 0);
    GXS_SetVisiblePlane(0x11);
    G2S_SetBG0Priority(3);
    GXS_SetVisibleWnd(0);
    G2S_BlendNone();
    func_ov002_0221c1b0();
}

void TournamentScene::func_ov002_0221c1b0() {
    ChangeDir("/Game/Tournament/bg/");
    func_02025b64(1, 1, 0, "tournament_b_bg_00_ncl.bin", 0, 0, 0);
    func_02025b64(1, 11, 0, "tournament_b_bg_00_ncg.bin", 0, 0, 0);
    func_02025b64(1, 7, 0, "tournament_b_bg_00_nsc.bin", 0, 0, 0);
    SpriteScreen &sub = SpriteManager::Get().Screen(1);
    ChangeDir("/Game/Tournament/Obj/");
    sub.LoadCharacters("tournament_b_obj_00.nce_ncg1D.bin", 0, 0);
    sub.LoadPalette("tournament_b_obj_00_ncl.bin", 0, 0);
    ChangeDir("/Game/2D/union_b/");
    sub.LoadCharacters("union_b_obj_cockpit.nce_ncg1D.bin", 0x4000, 0);
    sub.LoadPalette("union_b_obj_cockpit_ncl.bin", 0x60, 0);
    sub.LoadCharacters("union_b_obj_arrow.nce_ncg1D.bin", 0x6000, 0);
    sub.LoadPalette("union_b_obj_arrow_ncl.bin", 0x80, 0);
    ChangeDir("/");
    func_ov002_0221cabc("/font/mes_tornament.mes", 1);
    delete mMessage;
    mMessage = new MessageWindow("mes_tornament", 1, 0x9000);
    mMessage->func_02065bd4(0x14, 4, 0);
    mMessage->func_02065be4(0xd8);
    mMessage->func_02065890(5, 0, 1);
    func_ov002_0221c7a0("/font/mes_Im_button.mes");
    func_ov002_0221ddb8();
    func_ov002_0221e270();
    delete mMenu;
    mMenu = new SelectWindow(6, data_ov002_02220bc4, 0);
    mMenu->SetCells(6, data_ov002_02220c6c);
    mMenu->mCursor->mSelection = -1;
    func_ov002_0221da5c();
}

void TournamentScene::func_ov002_0221c4a0() {
    ChangeDir("/Game/Tournament/bg/");
    func_02025b64(1, 1, 0, "tournament_b_bg_00_ncl.bin", 0, 0, 0);
    func_02025b64(1, 11, 0, "tournament_b_bg_00_ncg.bin", 0, 0, 0);
    func_02025b64(1, 7, 0, "tournament_b_bg_00_b_nsc.bin", 0, 0, 0);
    SpriteScreen &sub = SpriteManager::Get().Screen(1);
    ChangeDir("/Game/Tournament/Obj/");
    sub.LoadCharacters("tournament_b_obj_02.nce_ncg1D.bin", 0, 0);
    sub.LoadPalette("tournament_b_obj_02_ncl.bin", 0, 0);
    delete mCell20;
    mCell20            = new CellAnimation("tournament_b_obj_02_nce.bin", 0, 1, 1);
    mCell20->mCharBase = sub.func_020276f4(0);
    mCell20->mUnk_11   = 0;
    ChangeDir("/Game/2D/union_b/");
    sub.LoadCharacters("union_b_obj_cockpit.nce_ncg1D.bin", 0x4000, 0);
    sub.LoadPalette("union_b_obj_cockpit_ncl.bin", 0x60, 0);
    func_ov002_0221cabc("/font/mes_town_info.mes", 2);
    delete mMessage;
    mMessage = new MessageWindow("mes_tornament", 1, 0x9000);
    mMessage->func_02065bd4(0x14, 4, 0);
    mMessage->func_02065be4(0xd8);
    mMessage->func_02065890(7, 0, 1);
    func_ov002_0221c7a0("/font/mes_T_clear.mes");
    ChangeDir("/");
    delete mMenu;
    mMenu = new SelectWindow(7, data_ov002_02220d14, 0);
    mMenu->SetCells(7, data_ov002_02220dd8);
}

void TournamentScene::func_ov002_0221c7a0(const char *mes) {
    for (u8 i = 2; i < 4; i++) {
        if (mMessages[i] != NULL) {
            OS_Free(mMessages[i]);
            mMessages[i] = NULL;
        }
        if (mTextBuffers[i] != NULL) {
            OS_Free(mTextBuffers[i]);
            mTextBuffers[i] = NULL;
        }
        if (mGlyphBuffers[i] != NULL) {
            OS_Free(mGlyphBuffers[i]);
            mGlyphBuffers[i] = NULL;
        }
    }
    mMessages[2]     = (MessageFile *) func_02023504(mes, 0, 0, 0, 0, 0);
    mTextSizes[2]    = TextManager::Get().BufferSize(0x1c, 6);
    mGlyphCounts[2]  = TextManager::Get().GlyphCount(0x15);
    mTextBuffers[2]  = OS_Alloc(mTextSizes[2]);
    mGlyphBuffers[2] = OS_Alloc(mGlyphCounts[2] * 0xc);
    TextManager::Get().Init(mGlyphBuffers[2], 0, 0, 0x1c, 6, 0x21);
    mTextSizes[3]    = TextManager::Get().BufferSize(0x18, 2);
    mGlyphCounts[3]  = TextManager::Get().GlyphCount(6);
    mTextBuffers[3]  = OS_Alloc(mTextSizes[3]);
    mGlyphBuffers[3] = OS_Alloc(mGlyphCounts[3] * 0xc);
    TextManager::Get().Init(mGlyphBuffers[3], 0, 0, 0x18, 2, 0x21);
    TextManager::Get().SetColor(0, 0, 0xf);
}


void TournamentScene::func_ov002_0221cabc(const char *mes, u8 rows) {
    for (u8 i = 0; i < 2; i++) {
        if (mMessages[i] != NULL) {
            OS_Free(mMessages[i]);
            mMessages[i] = NULL;
        }
        if (mTextBuffers[i] != NULL) {
            OS_Free(mTextBuffers[i]);
            mTextBuffers[i] = NULL;
        }
        if (mGlyphBuffers[i] != NULL) {
            OS_Free(mGlyphBuffers[i]);
            mGlyphBuffers[i] = NULL;
        }
    }
    mTextRows         = rows << 1;
    mMessages[0]    = (MessageFile *) func_02023504(mes, 0, 0, 0, 0, 0);
    mTextSizes[0]   = (u16) TextManager::Get().BufferSize(0x20, mTextRows);
    mGlyphCounts[0] = (u16) TextManager::Get().GlyphCount((mTextRows * 8 + 15) / 16 * 8);
    mTextBuffers[0] = OS_Alloc(mTextSizes[0]);
    mGlyphBuffers[0] = OS_Alloc(mGlyphCounts[0] * 0xc);
    TextManager::Get().Init(mGlyphBuffers[0], 1, 0, 0x20, mTextRows, 0x21);
    TextManager::Get().SetColor(1, 0, 0xf);
}

void TournamentScene::func_ov002_0221cd10() {
    if (mUnk_b5 != 0) {
        mMenu->Draw();
        if (mUnk_b8 == 0) {
            mMessage->func_020658d0();
        }
    } else if (mUnk_b9 != 0) {
        mMenu->Draw();
        if (mUnk_b8 == 0) {
            mMessage->func_020658d0();
        }
        mCell20->Update();
        mCell20->Draw(1, data_ov002_02220d14[mMode].mX, data_ov002_02220d14[mMode].mY, 2);
    }
    if (mUnk_b5 | mUnk_b9) {
        s16 a = 0x90;
        u8 b  = 8;
        if (mTextRows > 2) {
            b = 0x10;
            if ((int) mTextHeight <= 0xc) {
                a += 6;
            }
        }
        SpriteManager::Get().Screen(1).func_02027778(mGlyphBuffers[0], b, (s16) (0x80 - ((u32) mTextWidth >> 1)), a, 2, 0x100,
                                                     0xf, 0);
    }
    if (mUnk_b6 != 0) {
        s16 off = mUnk_a2;
        u8 idx  = data_020d43f0[0].mUnk_43 * sCounts[mMode];
        for (u8 i = 0; i < 3; i++) {
            if (i == 2) {
                off = 0;
            }
            mCellsA[i]->Update();
            mCellsA[i]->Draw(0, (s16) (off + data_ov002_02220a28[i].x), data_ov002_02220a28[i].y, data_ov002_02220a0c[i]);
        }
        if (mObj38 != NULL) {
            u8 j = 0;
            do {
                mObj38->func_0206547c(j, mUnk_a0 + (mUnk_a4 + data_ov002_02220b58[mMode][idx + j].x),
                                      mUnk_a6 + data_ov002_02220b58[mMode][idx + j].y, 0);
                j++;
            } while (j < sCounts[mMode]);
            if (mMode == 1) {
                mObj38->func_0206547c(3, mUnk_a2 + 8, 0xa, 1);
            }
            mObj38->func_020654a0();
        }
        u8 k = 0;
        if (mUnk_b9 != 0) {
            do {
                mCellsA[3]->Draw(0, (s16) (mUnk_a0 + data_ov002_02220b58[mMode][k].x), data_ov002_02220b58[mMode][k].y, 1);
                k++;
            } while (k < sTotals[mMode]);
        } else {
            do {
                s16 dx   = 0;
                s16 dy   = 0;
                u8 flag = 1;
                if (k >= idx) {
                    if (k < idx + sTotals[mMode]) {
                        dx   = mUnk_a4;
                        dy   = mUnk_a6;
                        flag = 0;
                    }
                }
                mCellsA[3]->func_0202a3ec(0, mUnk_c2[k], mUnk_a0 + (dx + data_ov002_02220b58[mMode][k].x),
                                          dy + data_ov002_02220b58[mMode][k].y, flag);
                k++;
            } while (k < sTotals[mMode]);
        }
        if (idx != 0) {
            u8 m = 0;
            do {
                mCellsA[4]->Draw(0, (s16) (mUnk_a0 + data_ov002_02220b58[mMode][m].x), data_ov002_02220b58[mMode][m].y, 0);
                m++;
            } while (m < idx);
        }
        if (mUnk_b7 != 0) {
            for (u8 n = 0; n < 3; n++) {
                if (mCellsB[n] != NULL) {
                    mCellsB[n]->Update();
                    mCellsB[n]->Draw(0, (s16) (mUnk_a2 + data_ov002_02220a28[n].x), data_ov002_02220a28[n].y, 0);
                }
            }
            if (mCellsB[3] != NULL) {
                u8 p = 0;
                do {
                    mCellsB[3]->Draw(0, (s16) (mUnk_a0 + (mUnk_a4 + data_ov002_02220b58[mMode][idx + p].x)),
                                     (s16) (mUnk_a6 + data_ov002_02220b58[mMode][idx + p].y), 0);
                    p++;
                } while (p < sCounts[mMode]);
            }
            if (mMode == 1) {
                mCellsB[3]->Draw(0, (s16) (mUnk_a2 + 8), 0xa, 0);
            }
        }
        u8 row;
        int base = 0x93;
        if (mUnk_ac < 0x23) {
            base = 0x96;
        }
        for (row = 0; row < 3; row++) {
            int y = row * 0xc + base;
            u8 col = 0;
            do {
                SpriteManager::Get().Screen(0).func_02027778(mGlyphBuffers[2], 1, (s16) (col * 0x20 + 0x14), y, 0,
                                                             (u16) ((col + row * 7) * 2 + 0x3a0), 0xf, 0);
                col++;
            } while (col < 7);
        }
        SpriteManager::Get().Screen(0).func_02027778(mGlyphBuffers[3], 6, 0x12, 0x82, 0, 0x3e0, 0xf, 0);
    }
    if (mUnk_b8 != 0) {
        mWindow->Draw();
        SpriteManager::Get().Screen(1).func_02027778(mGlyphBuffers[1], 9, 0x20, 0x28, 0, 0x360, 0xf, 0);
    }
}

u8 TournamentScene::func_ov002_0221d4b0(u8 input) {
    mNewSelection = mSelection;
    u8 result     = 0;
    if (input == 3) {
        switch (mMenu->GetSelectedId()) {
            case 3:
                if (mSelection != 0) {
                    result = 1;
                    mNewSelection--;
                }
                break;
            case 4:
                if (mSelection < mLastIndex + 1) {
                    result = 2;
                    mNewSelection++;
                }
                break;
        }
    }
    if (result == 0 && mMenu->mUnk_061 == 1) {
        u8 r = func_ov002_0221d69c();
        switch (r) {
            case 1:
                mMenu->func_0205c844(3, 3);
                break;
            case 2:
                mMenu->func_0205c844(4, 3);
                break;
        }
    }
    if (result != 0) {
        u8 i              = 0;
        WindowItem *items = mMenu->mCursor->mItems;
        do {
            if (mNewSelection == i) {
                mMenu->mCells[i]->func_02029f6c();
                mMenu->mCells[i]->SetAnimation(data_ov002_02220a00[i]);
            } else if (mSelection == i) {
                mMenu->mCells[i]->func_02029fb8(2);
            }
            items[i].mFlags = 0x8000;
            i++;
        } while (i <= mLastIndex + 1);
        if (mNewSelection == 0) {
            mMenu->mButtons[3].mEnabled = 0;
            items[3].mFlags             = 0x8000;
        }
        if (mNewSelection == mLastIndex + 1) {
            mMenu->mButtons[4].mEnabled = 0;
            items[4].mFlags             = 0x8000;
        }
        mScrollLeft = 0x62;
    }
    mDirection = result;
    return result;
}

u8 TournamentScene::func_ov002_0221d69c() {
    u8 result = 0;
    if (InputManager::Get().IsTrigger(0x20)) {
        if (mSelection != 0) {
            result = 1;
            mNewSelection--;
        }
    } else if (InputManager::Get().IsTrigger(0x10)) {
        if (mSelection < mLastIndex + 1) {
            result = 2;
            mNewSelection++;
        }
    }
    if (result != 0) {
        SoundManager::Get().func_020246f8(0);
    }
    return result;
}

BOOL TournamentScene::func_ov002_0221d7e4() {
    int r = mEvent->func_02067900();
    if (r != 0) {
        switch (r) {
            case 5:
                mSelection = 0;
                break;
            case 8:
                mUnk_d0    = 1;
                mSelection = 5;
                break;
            default:
                mSelection = 5;
                break;
        }
        return TRUE;
    }
    return FALSE;
}

BOOL TournamentScene::func_ov002_0221d848() {
    BOOL done = FALSE;
    s16 step  = 0xe;
    if (mScrollLeft < 0xe) {
        step = mScrollLeft;
    }
    mScrollLeft -= step;
    if (mDirection == 2) {
        step = -step;
    }
    u8 i              = 0;
    WindowItem *items = mMenu->mCursor->mItems;
    do {
        Vec2s pos = mMenu->GetCellPos(i);
        pos.x += step;
        mMenu->SetCellPos(i, pos);
        items[i].mX = step + items[i].mX;
        i++;
    } while (i <= mLastIndex + 1);
    if (mScrollLeft == 0) {
        mMenu->mCells[mSelection]->SetAnimation(data_ov002_022209fc[mSelection]);
        items[mNewSelection].mFlags = 5;
        items[mSelection].mFlags    = 0x8000;
        mMenu->mCursor->mSelection  = mNewSelection;
        mSelection                  = mNewSelection;
        if (mSelection == 0) {
            mMenu->mButtons[3].mEnabled = 0;
            items[3].mFlags             = 0x8000;
        } else {
            mMenu->mButtons[3].mEnabled = 1;
            items[3].mFlags             = 8;
        }
        if (mSelection == mLastIndex + 1) {
            mMenu->mButtons[4].mEnabled = 0;
            items[4].mFlags             = 0x8000;
        } else {
            mMenu->mButtons[4].mEnabled = 1;
            items[4].mFlags             = 8;
        }
        mMenu->mCells[4]->func_02029fc0();
        mMenu->mCells[3]->func_02029fc0();
        if (mDirection == 1) {
            mMenu->mCells[3]->SetAnimation(0);
        } else {
            mMenu->mCells[4]->SetAnimation(1);
        }
        func_ov002_0221dbd4(data_ov002_02220a04[mSelection]);
        func_ov002_0221ddf8(data_ov002_02220a08[mSelection]);
        done = TRUE;
    }
    return done;
}


void TournamentScene::func_ov002_0221da5c() {
    WindowItem *items = mMenu->mCursor->mItems;
    s16 base          = mSelection * 0x62;
    u8 i              = 0;
    do {
        Vec2s pos = mMenu->GetCellPos(i);
        pos.x     = i * 0x62 + 0x58 - base;
        mMenu->SetCellPos(i, pos);
        items[i].mX = pos.x;
        if (i == mSelection) {
            mMenu->mCells[i]->SetAnimation(data_ov002_02220a00[i]);
            items[i].mFlags = 5;
        } else {
            mMenu->mCells[i]->SetAnimation(data_ov002_022209fc[i]);
            items[i].mFlags = 0x8000;
        }
        mMenu->mCells[i]->func_02029f6c();
        i++;
    } while (i <= mLastIndex + 1);
    u32 flags;
    if (mSelection == 0) {
        mMenu->mButtons[3].mEnabled = 0;
        flags                       = 0x8000;
    } else {
        mMenu->mButtons[3].mEnabled = 1;
        flags                       = 8;
    }
    items[3].mFlags = flags;
    if (mSelection == mLastIndex + 1) {
        mMenu->mButtons[4].mEnabled = 0;
        flags                       = 0x8000;
    } else {
        mMenu->mButtons[4].mEnabled = 1;
        flags                       = 8;
    }
    items[4].mFlags            = flags;
    mMenu->mCursor->mSelection = mSelection;
}

void TournamentScene::func_ov002_0221dbd4(u8 index) {
    func_ov002_0221ddb8();
    TextManager::Get().mUnk_5b = 1;
    TextManager &tm   = TextManager::Get();
    s16 width;
    s16 height;
    height            = 0;
    width             = 0;
    MessageFile *file = mMessages[0];
    tm.Print(file->GetText(index), file->GetLength(index), mTextBuffers[0], 0x20, mTextRows, &width, &height, &mTextWidth, 0, 2, 0x21, 0xc, 0, 0, 0, 1, 1);
    if (&mTextHeight != NULL) {
        mTextHeight = height;
    }
    TextManager::Get().mUnk_5b = 0;
    func_02025c1c(SpriteManager::Get().Screen(1).mBgIndex, 4, 0x8000, mTextBuffers[0], mTextSizes[0], 0);
}

void TournamentScene::func_ov002_0221ddb8() {
    for (u8 i = 0; i < 2; i++) {
        if (mTextBuffers[i] != NULL) {
            MI_CpuClear16(mTextBuffers[i], mTextSizes[i]);
        }
    }
}


void TournamentScene::func_ov002_0221ddf8(u8 index) {
    mUnk_ae = 0xc8;
    mUnk_d6 = 0;
    TextManager &tm = TextManager::Get();
    u16 pixelWidth;
    s16 width;
    s16 height;
    height          = 0;
    width           = 0;
    MessageFile *file;
    void *buffer = mTextBuffers[2];
    file         = mMessages[2];
    MI_CpuClear16(tm.mPalette, 0x20);
    tm.mUnk_58 = 1;
    tm.Print(file->GetText(index), file->GetLength(index), buffer, 0x1c, 6, &width, &height, &pixelWidth, 0, 2, 0x21, 0x10, 0, 1, 0, 1, 1);
    u32 h = (u16) height;
    for (u8 i = 1; i < 16; i++) {
        if (tm.mPalette[i] == 0) {
            tm.mPalette[i] = tm.mPalette[i - 1];
        }
    }
    mUnk_d7 = h >> 4;
    func_ov002_0221df48(index);
}

void TournamentScene::func_ov002_0221df48(u32 index) {
    s16 height2;
    s16 width2;
    s16 height1;
    s16 width1;
    func_ov002_0221e270();
    TextManager &text = TextManager::Get();
    width1            = -(mUnk_d6 * 0x30);
    height1           = 0;
    MessageFile *file = mMessages[2];
    u16 length = file->GetLength(index);
    const u8 *message = file->GetText(index);
    void *buffer = mTextBuffers[2];
    u16 half = 0xffff;
    if (length < half * 2) {
        half = length / 2;
    }
    text.mUnk_58 = 0x10;
    text.Print(message, half * 2, buffer, 0x1c, 6, &height1, &width1, 0, 0, 2, 0x21, 0x10, 0, 0, 0,
               1, 1);
    if (&mUnk_ac != NULL) {
        mUnk_ac = width1;
    }
    func_02025c1c(SpriteManager::Get().Screen(0).mBgIndex, 4, 0x1d000, mTextBuffers[2], mTextSizes[2], 0);
    height2 = 0;
    width2  = 0;
    TextManager::Get().mUnk_5b = 1;
    TextManager::Get().Print(data_020d441a, 0x18, mTextBuffers[3], 0x18, 2, &height2, &width2, 0, 0, 2, 0x21, 0, 0, 0, 0, 1, 1);
    TextManager::Get().mUnk_5b = 0;
    func_02025c1c(SpriteManager::Get().Screen(0).mBgIndex, 4, 0x1f000, mTextBuffers[3], mTextSizes[3], 0);
}

void TournamentScene::func_ov002_0221e270() {
    for (u8 i = 2; i < 4; i++) {
        if (mTextBuffers[i] != NULL) {
            MI_CpuClear16(mTextBuffers[i], mTextSizes[i]);
        }
    }
}

BOOL TournamentScene::func_ov002_0221e2b0() {
    BOOL result = FALSE;
    switch (mState) {
        case 0:
            func_ov002_0221e8f0();
            mState++;
            break;
        case 1:
            if (mUnk_c0 <= -16) {
                mState++;
            } else {
                mUnk_c0--;
                G2_SetBlendBrightness(0x11, mUnk_c0);
                G2S_SetBlendBrightness(0x11, mUnk_c0);
            }
            break;
        case 2: {
            u8 mode = mMode;
            u8 idx  = data_020d43f0[0].mUnk_43 * sCounts[mode];
            mUnk_a8 = data_ov002_02220a2e[mode].x - (mUnk_a0 + data_ov002_02220b58[mode][idx].x);
            mUnk_aa = data_ov002_02220a2e[mMode].y - data_ov002_02220b58[mMode][idx].y;
            mStep = 0;
            mState++;
            break;
        }
        case 3:
            mStep++;
            mUnk_a4 = mUnk_a8 * mStep / 5;
            mUnk_a6 = mUnk_aa * mStep / 5;
            if (mStep >= 5) {
                mCellsA[1]->func_02029fb8(4);
                mCellsB[1]->func_02029fb8(4);
                mState++;
            }
            break;
        case 4:
            if (mCellsA[1]->mUnk_12 != 0) {
                mUnk_c1 = 0x3c;
                mState++;
            }
            break;
        case 5:
            if (mUnk_c1 != 0) {
                mUnk_c1--;
            } else {
                mState++;
            }
            break;
        case 6:
            if (mUnk_a0 <= 0xe0) {
                mUnk_a0 += 0x20;
            } else if (mUnk_a0 >= 0x120) {
                mUnk_a0 -= 0x20;
            } else {
                mUnk_a0 = 0x100;
                mState++;
            }
            mUnk_a2 = -mUnk_a0;
            break;
        case 7:
            SpriteManager::Get().StartFade(3);
            mState++;
            break;
        case 8:
            if (!SpriteManager::Get().IsFading()) {
                mUnk_b6 = 0;
                mUnk_b7 = 0;
                G2_SetBlendBrightness(0x11, mUnk_c0);
                G2S_SetBlendBrightness(0x11, mUnk_c0);
                mState++;
            }
            break;
        case 9:
            func_ov002_0221b424();
            if (mEvent == NULL) {
                mEvent = new UnkStruct_02065c40(7, 0, 1);
            }
            mUnk_c1 = 0;
            mState++;
            break;
        case 10:
            if (!mEvent->func_020678e8()) {
                u8 mode = mMode;
                u8 idx = data_020d43f0[0].mUnk_43 * sCounts[mode];
                u32 pick = MATH_Rand32(&data_020d42e8, 3);
                u8 id   = mUnk_c2[mUnk_c1 + idx];
                if (mEvent->func_0206f9b4("mes_tm_battle", id, data_ov002_02220a54[0][id], data_ov002_02220a6c[id][pick], 2, 3,
                                  0)) {
                    SpriteManager::Get().StartFade(2);
                    mState++;
                }
            }
            break;
        case 11:
            if (!mEvent->func_020678e8()) {
                mUnk_c1++;
                if (mUnk_c1 < sCounts[mMode]) {
                    mState = 10;
                } else {
                    mState++;
                }
            }
            break;
        case 12:
            SpriteManager::Get().StartFade(3);
            mState++;
            break;
        case 13:
            if (!SpriteManager::Get().IsFading()) {
                result = TRUE;
            }
            break;
    }
    return result;
}

void TournamentScene::func_ov002_0221e8f0() {
    GX_SetVisibleWnd(4);
    G2_SetWndOutsidePlane(0x31, FALSE);
    reg_G2_WINOUT  = (reg_G2_WINOUT & ~0x3f00) | 0x1100;
    mUnk_c0        = 0;
    mUnk_b7        = 1;
    if (mMode == 1) {
        mCellsB[0]->SetAnimation(12);
        mCellsB[0]->func_02029fb8(0);
    } else {
        mCellsB[0]->SetAnimation(10);
        mCellsB[0]->func_02029fb8(0);
    }
}

void TournamentScene::func_ov002_0221e984() {
    switch (data_020d43f0[0].mUnk_42) {
        case 1:
            mMode = 0;
            break;
        case 3:
            mMode = 1;
            break;
        case 6:
            mMode = 2;
            break;
        case 7:
            mMode = 3;
            break;
        case 8:
            mMode = 4;
            break;
        case 9:
            mMode = 5;
            break;
        default:
            mMode = 0;
            break;
    }
    if (mEvent == NULL) {
        return;
    }
    u8 a = 0;
    u8 b = 0;
    u8 c = 1;
    if (mMode == 1) {
        c       = 2;
        mUnk_cb = mEvent->func_02067120(data_020d43f0[0].mUnk_42, data_020d43f0[0].mUnk_43, 1, 0);
    }
    u8 i = 0;
    do {
        mUnk_c2[i] = mEvent->func_02067120(data_020d43f0[0].mUnk_42, a, (u8) (b + c), 0);
        b++;
        if (b >= sCounts[mMode]) {
            a++;
            b = 0;
        }
        i++;
    } while (i < sTotals[mMode]);
}

void TournamentScene::func_ov002_0221ead8() {
    GXS_SetVisiblePlane(0x13);
    mWindow->func_0205c7c8(1);
    mWindow->mUnk_064 = 1;
    mUnk_b8           = 1;
    G2S_SetWnd0Position(0x10, 0x10, 0xf0, 0xb0);
    GXS_SetVisibleWnd(1);
    G2S_SetWnd0InsidePlane(0x13, FALSE);
    G2S_SetWndOutsidePlane(0x33, FALSE);
    G2S_SetBlendBrightness(0x11, -8);
}

void TournamentScene::func_ov002_0221eb78() {
    GXS_SetVisiblePlane(0x11);
    mWindow->mUnk_064 = 0;
    mUnk_b8           = 0;
    G2S_SetBlendBrightness(0x11, 0);
    GXS_SetVisibleWnd(0);
    G2S_SetWnd0InsidePlane(0x1f, FALSE);
    G2S_SetWndOutsidePlane(0x3f, FALSE);
}

void TournamentScene::func_ov002_0221ebe4(const char *path, u32 index) {
    ChangeDir("/Game/2D/union_b/");
    func_02025b64(1, 8, 0, "union_b_bg_window_b_nsc.bin", 0, 0, 0);
    func_02025b64(1, 12, 0, "union_b_bg_window_ncg.bin", 0, 0, 0);
    func_02025b64(1, 1, 0x2000, "union_b_bg_window_ncl.bin", 0, 0, 0);
    SpriteScreen &screen = SpriteManager::Get().Screen(1);
    ChangeDir("/Game/2D/union_b/");
    screen.LoadCharacters("union_b_obj_window.nce_ncg1D.bin", 0x10000, 0);
    screen.LoadPalette("union_b_obj_window_ncl.bin", 0x1a0, 0);
    if (mMessages[1] != NULL) {
        OS_Free(mMessages[1]);
    }
    if (mTextBuffers[1] != NULL) {
        OS_Free(mTextBuffers[1]);
    }
    if (mGlyphBuffers[1] != NULL) {
        OS_Free(mGlyphBuffers[1]);
    }
    mMessages[1] = (MessageFile *) func_02023504(path, 0, 0, 0, 0, 0);
    mTextSizes[1] = TextManager::Get().BufferSize(0x18, 0x10);
    mGlyphCounts[1]     = TextManager::Get().GlyphCount(0xc);
    mTextBuffers[1]     = OS_Alloc(mTextSizes[1]);
    mGlyphBuffers[1]    = OS_Alloc(mGlyphCounts[1] * 0xc);
    TextManager::Get().Init(mGlyphBuffers[1], 1, 0, 0x18, 0x10, 0x42);
    func_ov002_0221ef34(index);
    delete mWindow;
    mWindow = new SelectWindow(3, sWindowItems, 1);
    mWindow->SetCells(3, sWindowCells);
    mWindow->mUnk_064 = 0;
    ChangeDir("/");
}

void TournamentScene::func_ov002_0221ef34(u32 index) {
    MI_CpuClear16(mTextBuffers[1], mTextSizes[1]);
    TextManager &text = TextManager::Get();
    s16 height;
    s16 width;
    width  = 0;
    height = 0;
    MessageFile *file = mMessages[1];
    text.Print(file->GetText(index), file->GetLength(index), mTextBuffers[1], 0x18, 0x10, &height, &width, 0, 0, 2, 0x42, 0x10, 0, 0, 0, 1, 1);
    func_02025c1c(SpriteManager::Get().Screen(1).mBgIndex, 4, 0x1b000, mTextBuffers[1], mTextSizes[1], 0);
}

void TournamentScene::func_ov002_0221f098(u32 index) {
    ChangeDir("/game/Tournament/Bg/");
    index      = sModeTables[2][index];
    u32 nsc    = 7;
    u32 ncg    = 0xb;
    u32 offset = 0;
    if (mUnk_d4 != 0) {
        nsc    = 8;
        ncg    = 0xc;
        offset = 0x2000;
    }
    char path[64];
    sprintf(path, "tournament_u_bg_0%d_nsc.bin", index);
    func_02025b64(0, nsc, 0, path, 0, 0, 0);
    sprintf(path, "tournament_u_bg_0%d_ncg.bin", index);
    func_02025b64(0, ncg, 0, path, 0, 0, 0);
    sprintf(path, "tournament_u_bg_0%d_ncl.bin", index);
    func_02025b64(0, 1, offset, path, 0, 0, 0);
    mBlend = 0;
    if (mUnk_d4 == 0) {
        G2_SetBG0Priority(2);
        G2_SetBG1Priority(3);
        G2x_SetBlendAlpha_(&reg_G2_BLDCNT, 1, 2, mBlend, 0x1f - mBlend);
        mUnk_d4 = 1;
    } else {
        G2_SetBG0Priority(3);
        G2_SetBG1Priority(2);
        G2x_SetBlendAlpha_(&reg_G2_BLDCNT, 2, 1, mBlend, 0x1f - mBlend);
        mUnk_d4 = 0;
    }
    GX_SetVisiblePlane(0x13);
}

BOOL TournamentScene::func_ov002_0221f250() {
    BOOL done = TRUE;
    if (mBlend < 0x1f) {
        mBlend++;
        u8 level = mBlend;
        reg_G2_BLDALPHA = level | ((0x1f - level) << 8);
        done = FALSE;
        if (mBlend >= 0x1f) {
            if (mUnk_d4 == 0) {
                GX_SetVisiblePlane(0x12);
            } else {
                GX_SetVisiblePlane(0x11);
            }
            G2_BlendNone();
            mBlend        = 0x1f;
        }
    }
    return done;
}
