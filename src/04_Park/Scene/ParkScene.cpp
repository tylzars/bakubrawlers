#include "Text/TextManager.hpp"
#include "System/FrameTask.hpp"
#include "Graphics/SpriteManager.hpp"
#include "global.h"
#include "types.h"
#include "Scene/ParkScene.hpp"
#include "Park/ParkMenu.hpp"
#include "Park/ParkPopup.hpp"
#include "Game/Random.hpp"
#include "Unknown/UnkFunctions.hpp"
#include "UI/SaveWindow.hpp"
#include "UI/UnlockPopup.hpp"
#include "Game/ScriptPlayer.hpp"
#include "Game/UnkStruct_020d433c.hpp"
#include "Game/SaveData.hpp"
#include "Graphics/Display.hpp"
#include "System/FileSystem.hpp"
#include "UI/SelectWindow.hpp"
#include <nitro/gx.h>
#include <nitro/mi.h>
#include <nitro/os.h>
#include <stdio.h>


// One record of the park character table, filled by GetParkEntry.
struct ParkEntry {
    /* 00 */ u32 mType;
    /* 04 */ u8 mUnk_04;
    /* 05 */ u8 mUnk_05;
    /* 06 */ u8 mProgressEntryIndex;
    /* 07 */ u8 mMinCategory;
    /* 08 */ u8 mFieldId;
    /* 09 */ u8 mUnk_09;
    /* 0a */ union {
        u16 mUnk_0a_h;
        struct {
            u8 mUnk_0a;
            u8 mUnk_0b;
        };
    };
};

struct ParkRandEntry {
    u16 mMessage;
    u8 mKind;
    u8 mPad;
};
static const ParkRandEntry sParkWinLoseMessages[17][6] = {
    {{6, 1, 0}, {7, 1, 0}, {8, 1, 0}, {6, 3, 0}, {7, 3, 0}, {8, 3, 0}},
    {{9, 1, 0}, {10, 1, 0}, {11, 1, 0}, {9, 3, 0}, {10, 3, 0}, {11, 3, 0}},
    {{12, 1, 0}, {13, 1, 0}, {14, 1, 0}, {12, 3, 0}, {13, 3, 0}, {14, 3, 0}},
    {{15, 1, 0}, {16, 1, 0}, {17, 1, 0}, {15, 3, 0}, {16, 3, 0}, {17, 3, 0}},
    {{18, 1, 0}, {19, 1, 0}, {20, 1, 0}, {18, 3, 0}, {19, 3, 0}, {20, 3, 0}},
    {{0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}},
    {{21, 1, 0}, {22, 1, 0}, {23, 1, 0}, {21, 3, 0}, {22, 3, 0}, {23, 3, 0}},
    {{3, 1, 0}, {4, 1, 0}, {5, 1, 0}, {3, 3, 0}, {4, 3, 0}, {5, 3, 0}},
    {{24, 1, 0}, {25, 1, 0}, {26, 1, 0}, {24, 3, 0}, {25, 3, 0}, {26, 3, 0}},
    {{27, 1, 0}, {28, 1, 0}, {29, 1, 0}, {27, 3, 0}, {28, 3, 0}, {29, 3, 0}},
    {{30, 1, 0}, {31, 1, 0}, {32, 1, 0}, {30, 3, 0}, {31, 3, 0}, {32, 3, 0}},
    {{33, 1, 0}, {34, 1, 0}, {35, 1, 0}, {33, 3, 0}, {34, 3, 0}, {35, 3, 0}},
    {{36, 1, 0}, {37, 1, 0}, {38, 1, 0}, {36, 3, 0}, {37, 3, 0}, {38, 3, 0}},
    {{39, 1, 0}, {40, 1, 0}, {41, 1, 0}, {39, 3, 0}, {40, 3, 0}, {41, 3, 0}},
    {{42, 1, 0}, {43, 1, 0}, {44, 1, 0}, {42, 3, 0}, {43, 3, 0}, {44, 3, 0}},
    {{45, 1, 0}, {46, 1, 0}, {47, 1, 0}, {45, 3, 0}, {46, 3, 0}, {47, 3, 0}},
    {{48, 1, 0}, {49, 1, 0}, {50, 1, 0}, {48, 3, 0}, {49, 3, 0}, {50, 3, 0}},
};

static const WindowItemRecord sTwoButtonItems[2] = {
    {0, 0x1, 89, 152, 78, 20, 0, {-1, -1, -1, -1}, 0, 1},
    {1, 0x8000, 120, 18, 78, 20, 0, {-1, -1, -1, -1}, 0, 0},
};

static const WindowCellRecord sThreeButtonCells[3] = {
    {"/Game/2D/union_b/union_b_obj_window_nce.bin", 3, 0x10000, 0, 0, 1, {3, 5, 4, 0}},
    {"/Game/2D/union_b/union_b_obj_window_nce.bin", 7, 0x10000, 0, 0, 1, {6, 8, 7, 0}},
    {"/Game/2D/union_b/union_b_obj_window_nce.bin", 10, 0x10000, 0, 0, 1, {10, 10, 10, 0}},
};

static const WindowCellRecord sTwoButtonCells[2] = {
    {"/Game/2D/union_b/union_b_obj_window_nce.bin", 1, 0x10000, 0, 0, 1, {0, 2, 1, 0}},
    {"/Game/2D/union_b/union_b_obj_window_nce.bin", 9, 0x10000, 0, 0, 1, {9, 9, 9, 0}},
};

static const WindowItemRecord sThreeButtonItems[3] = {
    {0, 0x1, 45, 152, 78, 20, 0, {0, 0, 1, 1}, 0, 2},
    {1, 0x1, 133, 152, 78, 20, 0, {1, 1, 0, 0}, 0, 2},
    {2, 0x8000, 120, 18, 78, 20, 0, {-1, -1, -1, -1}, 0, 0},
};


ParkScene::ParkScene() :
    SceneBase(1000, 0) {
    mEntryIndex = 0;
    mState = 0;
    mTimer = 0;
    mUnk_0f = 0;
    mWindowVisible = 0;
    mRequestParkScene = 0;
    mMenu = NULL;
    mWindow = NULL;
    mUnk_2c = NULL;
    mUnk_1c = NULL;
    mGlyphBuffer = NULL;
    mPlayerCount = data_020d433c.mPlayerCount;
    SpriteManager::Get().SetFade(FADE_MODE_HOLD, -16);
    G2_BlendNone();
}

ParkScene::~ParkScene() {
    Cleanup();
}

void ParkScene::Update() {
    switch (mState) {
        case 0: {
            u8 saved                 = data_020d433c.mUnk_7d;
            data_020d433c.mUnk_7d    = 0;
            Init();
            if (mEvent->func_02066c7c()) {
                switch (data_020d433c.mUnk_7c) {
                    case 2:
                    case 3:
                        data_020d433c.mUnk_7d = data_020d433c.mUnk_7c;
                        break;
                    default:
                        data_020d433c.mUnk_7d = 0;
                        break;
                }
                mTimer = 8;
                mState++;
            } else {
                data_020d433c.mUnk_7d = saved;
                mState               = 10;
            }
            break;
        }
        case 1:
            if (--mTimer != 0) {
                break;
            }
            data_020d433c.mUnk_7c = 1;
            SpriteManager::Get().StartFade(FADE_MODE_TO_ZERO);
            mState++;
            break;
        case 2:
            if (mEvent->GetResult() | mEvent->GetSaveRequest()) {
                SpriteManager::Get().StartSubFade(FADE_MODE_DOWN);
                mState = 4;
            }
            if (mEvent->GetUnlockPopupHandle()) {
                SpriteManager::Get().StartSubFade(FADE_MODE_DOWN);
                mState = 7;
            }
            if (mEvent->IsRunning()) {
                break;
            }
            SpriteManager::Get().StartSubFade(FADE_MODE_DOWN);
            mState++;
            break;
        case 3:
            if (SpriteManager::Get().sFade[1] > FADE_MODE_HOLD) {
                break;
            }
            {
                u8 v = gSaveData[0].mUnk_42;
                if ((v == 2) | (v == 4)) {
                    if (v == 2) {
                        data_020d433c.mUnk_83 = 5;
                    } else {
                        data_020d433c.mUnk_83 = 6;
                    }
                    data_020d433c.mUnk_84 = 0;
                    data_020d433c.mUnk_82 = 1;
                    RequestScene(SCENE_TUTORIAL);
                    mFinished = TRUE;
                    return;
                }
            }
            RequestScene(SCENE_BATTLE_PLAYER_LIST);
            mFinished = TRUE;
            return;
        case 4:
            if (SpriteManager::Get().sFade[1] > FADE_MODE_HOLD) {
                break;
            }
            LoadWindowGraphics();
            new SaveWindow(1, 1, 0, 0x2000, 0, 0, 0, 0, 0);
            mContext->mRequest = 0;
            data_020d433c.mUnk_14_1 = 0;
            SpriteManager::Get().StartSubFade(FADE_MODE_TO_ZERO);
            mState++;
            break;
        case 5:
            if (mContext->mRequest == 0) {
                break;
            }
            SpriteManager::Get().StartSubFade(FADE_MODE_DOWN);
            FrameTask::Get().StartChannel(0, 0x14);
            mState++;
            break;
        case 6:
            if (SpriteManager::Get().sFade[1] > FADE_MODE_HOLD) {
                break;
            }
            if (mRequestParkScene == 0) {
                RequestScene(SCENE_MAP);
                mFinished = TRUE;
            } else {
                RequestScene(SCENE_PARK);
                mFinished = TRUE;
            }
            return;
        case 7:
            if (SpriteManager::Get().sFade[1] > FADE_MODE_HOLD) {
                break;
            }
            new UnlockPopup(mEvent->GetUnlockPopupHandle() - 1);
            mContext->mRequest = 0;
            mState++;
            break;
        case 8:
            if (mContext->mRequest == 0) {
                break;
            }
            ReloadBackground();
            mEvent->ClearUnlockPopupHandle();
            mState++;
            break;
        case 9:
            mState = 2;
            break;
        case 10:
            switch (data_020d433c.mUnk_7c) {
                case 2:
                case 3:
                    mEvent->RequestFinish();
                    mContext->mRequest = 0;
                    mState           = 11;
                    break;
                default:
                    FrameTask::Get().StartChannel(0, 0x14);
                    mState = 0x11;
                    break;
            }
            break;
        case 11: {
            if (mContext->mRequest == 0) {
                break;
            }
            ParkEntry entry;
            GetParkEntry(data_020d433c.mUnk_8b_0, &entry);
            if (entry.mType == PARK_ENTRY_TYPE_TUTORIAL) {
                SpriteManager::Get().StartSubFade(FADE_MODE_DOWN);
                mState++;
                break;
            }
            if (data_020d433c.mHasPartners != 0) {
                BOOL rules = FALSE;
                if (data_020d433c.mUnk_7c == 2) {
                    rules = reinterpret_cast<ParkRules *>(this)->func_ov004_0221bf5c();
                }
                func_02089a9c(data_020d433c.mUnk_8b_0);
                data_020d433c.mUnk_7c = 1;
                if (rules == 0) {
                    Init();
                    FrameTask::Get().StartChannel(0, 0x14);
                    mState = 0x11;
                } else {
                    mRequestParkScene = 1;
                    mState = 4;
                }
            } else {
                mEvent = new ScriptPlayer(7, 0, 1);
                FrameTask::Get().StartChannel(0, 0x14);
                mState = 0xd;
            }
            break;
        }
        case 12:
            if (mContext->mRequest == 0) {
                break;
            }
            data_020d433c.mUnk_7c = 1;
            func_02089a9c(data_020d433c.mUnk_8b_0);
            mRequestParkScene = 1;
            mState = 4;
            break;
        case 13: {
            ParkEntry entry;
            GetParkEntry(data_020d433c.mUnk_8b_0, &entry);
            u32 idx = GetParkGroupId(data_020d433c.mUnk_8b_0);
            u8 pick = MATH_Rand32(&gRandom, 3);
            u8 nib  = data_020d433c.mUnk_8c_4;
            if (nib == 0) {
                pick += 3;
            }
            u8 kind     = sParkWinLoseMessages[idx][pick].mKind;
            u16 message = sParkWinLoseMessages[idx][pick].mMessage;
            u8 ok;
            if (nib != 0) {
                ok = mEvent->func_0206f9b4("mes_pm_win", idx, kind, message, 0, 2, 0);
            } else {
                ok = mEvent->func_0206f9b4("mes_pm_lose", idx, kind, message, 1, 2, 0);
            }
            if (ok == 0) {
                break;
            }
            SpriteManager::Get().StartFade(FADE_MODE_TO_ZERO);
            mState++;
            break;
        }
        case 14:
            if (!mEvent->IsRunning()) {
                data_020d433c.mUnk_7c = 1;
                SpriteManager::Get().StartSubFade(FADE_MODE_DOWN);
                mState = 0xf;
                break;
            }
            if (mEvent->IsGraphicsLoaded()) {
                ReloadBackground();
                mEvent->ClearGraphicsLoaded();
            }
            break;
        case 15:
            if (SpriteManager::Get().sFade[1] > FADE_MODE_HOLD) {
                break;
            }
            mEvent->RequestFinish();
            mContext->mRequest = 0;
            mState++;
            break;
        case 16: {
            if (mContext->mRequest == 0) {
                break;
            }
            BOOL rules = FALSE;
            if (data_020d433c.mUnk_8c_4 == 0) {
                rules = reinterpret_cast<ParkRules *>(this)->func_ov004_0221bf5c();
            }
            func_02089a9c(data_020d433c.mUnk_8b_0);
            if (rules == 0) {
                Init();
                mState = 0x11;
            } else {
                mRequestParkScene = 1;
                mState = 4;
            }
            break;
        }
        case 17:
            SpriteManager::Get().StartFade(FADE_MODE_TO_ZERO);
            mState++;
            break;
        case 18:
            if (mEvent->IsRunning()) {
                break;
            }
            data_020d433c.mUnk_7c = 1;
            mEvent->func_0206ef4c("mes_pm_button", 5, 0xa, 2, 0x15);
            mState++;
            break;
        case 19: {
            u8 result = mMenu->mWindow->Update(TRUE);
            switch (result) {
                case 1:
                    mUnk_0f = 1;
                    mState = 0x15;
                    break;
                case 2:
                    mUnk_0f = 0;
                    mState = 0x15;
                    break;
                default:
                    if (mMenu->UpdateSelection(result)) {
                        u8 target = mMenu->mTarget;
                        if (mEntryIndex != target) {
                            mEntryIndex = target;
                            u32 n   = GetParkGroupId(target);
                            if (n >= 5) {
                                n = (u8) (n - 1);
                            }
                            mEvent->func_0206f564((u8) (n + 6), 10, -1);
                        }
                        mState = 0x14;
                    }
                    break;
            }
            break;
        }
        case 20:
            if (mMenu->mWindow->mState != 1) {
                mMenu->mWindow->Update(TRUE);
            }
            if (mMenu->Slide()) {
                mState = 0x13;
            }
            break;
        case 21:
            SpriteManager::Get().StartFade(FADE_MODE_DOWN);
            mState++;
            break;
        case 22:
            if (SpriteManager::Get().IsFading()) {
                break;
            }
            mEvent->RequestFinish();
            mContext->mRequest = 0;
            mState++;
            break;
        case 23:
            if (mContext->mRequest == 0) {
                break;
            }
            Cleanup();
            if (mUnk_0f != 0) {
                mState++;
                mEvent = new ScriptPlayer(7, 0, 1);
            } else {
                mState = 0x29;
            }
            break;
        case 24: {
            u32 shown;
            u32 idx = GetParkGroupId(mEntryIndex);
            shown = idx;
            ParkEntry entry;
            GetParkEntry(mEntryIndex, &entry);
            if (idx >= 5) {
                shown = (u16) (idx - 1);
            }
            u32 kind = 2;
            char name[0x20];
            switch (entry.mType) {
                case PARK_ENTRY_TYPE_BATTLE:
                    sprintf(name, "mes_pm_battle");
                    kind = 3;
                    break;
                case PARK_ENTRY_TYPE_TEAMBATTLE:
                    sprintf(name, "mes_pm_Teambattle");
                    kind = 4;
                    break;
                case PARK_ENTRY_TYPE_BATTLE_ROYALE:
                    sprintf(name, "mes_pm_BattleRoyale");
                    kind = 5;
                    break;
                case PARK_ENTRY_TYPE_MES_ONLY_1:
                    sprintf(name, "mes_pm_mes_only_1");
                    kind = 6;
                    break;
                case PARK_ENTRY_TYPE_MES_ONLY_2:
                    sprintf(name, "mes_pm_mes_only_2");
                    kind = 7;
                    break;
                case PARK_ENTRY_TYPE_MES_ONLY_3:
                    sprintf(name, "mes_pm_mes_only_3");
                    kind = 8;
                    break;
                case PARK_ENTRY_TYPE_TUTORIAL:
                    sprintf(name, "mes_pm_Tutorial");
                    shown = entry.mUnk_0a_h;
                    if (shown == 0 && (gSaveData[0].mUnk_5be & 8)) {
                        shown = 1;
                    }
                    kind = 9;
                    break;
            }
            u16 halves[2] = {0x15, idx};
            u8 bytes[2]   = {entry.mUnk_04, entry.mUnk_05};
            if (!mEvent->func_0206fa20(name, halves, bytes, 2, shown, kind, 2, 1, 1)) {
                break;
            }
            GXS_SetVisiblePlane(0x11);
            SpriteManager::Get().StartFade(FADE_MODE_TO_ZERO);
            mContext->mRequest = 0;
            mState++;
            break;
        }
        case 25:
            if (mEvent->mUnk_14e7 != 0) {
                ParkEntry entry;
                GetParkEntry(mEntryIndex, &entry);
                switch (entry.mType) {
                    case PARK_ENTRY_TYPE_BATTLE:
                    case PARK_ENTRY_TYPE_TEAMBATTLE:
                    case PARK_ENTRY_TYPE_BATTLE_ROYALE:
                        mEvent->mUnk_14f1 = 0;
                        if (entry.mProgressEntryIndex != 0xff) {
                            mState = 0x1c;
                        } else {
                            mState = 0x1f;
                        }
                        break;
                    case PARK_ENTRY_TYPE_TUTORIAL:
                        mEvent->mUnk_14f1 = 0;
                        mState           = 0x24;
                        break;
                    default:
                        SpriteManager::Get().StartFade(FADE_MODE_DOWN);
                        mState++;
                        break;
                }
            } else if (mEvent->IsGraphicsLoaded()) {
                ReloadBackground();
                mEvent->ClearGraphicsLoaded();
            }
            break;
        case 26:
            if (SpriteManager::Get().IsFading()) {
                break;
            }
            mEvent->RequestFinish();
            mContext->mRequest = 0;
            mState++;
            break;
        case 27:
            if (mContext->mRequest == 0) {
                break;
            }
            func_02089a9c(mEntryIndex);
            Init();
            mState = 0x11;
            break;
        case 28: {
            ParkEntry entry;
            GetParkEntry(mEntryIndex, &entry);
            reinterpret_cast<ParkPopup *>(this)->Open("/font/mes_park.mes", (u8) (entry.mProgressEntryIndex + 5), PARK_POPUP_TWO_ITEMS);
            ShowWindow();
            mState++;
            break;
        }
        case 29:
            mWindow->mCursor->mSelection = -1;
            mWindow->mState            = 4;
            mWindow->Reset(0);
            mState++;
            break;
        case 30:
            if ((u8) mWindow->Update(TRUE) != 1) {
                break;
            }
            HideWindow();
            mState++;
            break;
        case 31:
            reinterpret_cast<ParkPopup *>(this)->Open("/font/mes_park.mes", 2, PARK_POPUP_THREE_ITEMS);
            ShowWindow();
            mState++;
            break;
        case 32:
            mWindow->mCursor->mSelection = -1;
            mWindow->mState            = 4;
            mWindow->Reset(1);
            mState++;
            break;
        case 33: {
            if ((u8) mWindow->Update(TRUE) != 1) {
                break;
            }
            HideWindow();
            if (mWindow->mCursor->mSelection == 0) {
                u32 idx = GetParkGroupId(mEntryIndex);
                ParkEntry entry;
                GetParkEntry(mEntryIndex, &entry);
                switch (entry.mType) {
                    case PARK_ENTRY_TYPE_BATTLE:
                        data_020d433c.mPlayerCount = 2;
                        data_020d433c.mUnk_99      = 3;
                        data_020d433c.mFieldId      = GetUnlockedFieldId(entry.mFieldId);
                        data_020d433c.mUnk_96      = entry.mUnk_09;
                        data_020d433c.mHasPartners      = 0;
                        data_020d433c.mCharacterIds[0]  = 0x15;
                        data_020d433c.mUnk_a3[0]      = 0;
                        data_020d433c.mCharacterIds[1]      = idx;
                        data_020d433c.mUnk_a3[1]      = func_ov004_0221b8c4(idx);
                        break;
                    case PARK_ENTRY_TYPE_TEAMBATTLE:
                        data_020d433c.mPlayerCount = 4;
                        data_020d433c.mUnk_99      = 3;
                        data_020d433c.mFieldId      = GetUnlockedFieldId(entry.mFieldId);
                        data_020d433c.mUnk_96      = entry.mUnk_09;
                        data_020d433c.mHasPartners      = 1;
                        data_020d433c.mCharacterIds[0]  = 0x15;
                        data_020d433c.mUnk_a3[0]      = 0;
                        data_020d433c.mCharacterIds[1]      = idx;
                        data_020d433c.mUnk_a3[1]      = func_ov004_0221b8c4(idx);
                        data_020d433c.mCharacterIds[2]      = entry.mUnk_0a;
                        data_020d433c.mUnk_a3[2]      = func_ov004_0221b8c4(entry.mUnk_0a);
                        data_020d433c.mCharacterIds[3]      = entry.mUnk_0b;
                        data_020d433c.mUnk_a3[3]      = func_ov004_0221b8c4(entry.mUnk_0b);
                        data_020d433c.mPartners[0]      = 1;
                        data_020d433c.mPartners[1]      = 0;
                        data_020d433c.mPartners[2]      = 3;
                        data_020d433c.mPartners[3]      = 2;
                        break;
                    case PARK_ENTRY_TYPE_BATTLE_ROYALE:
                        data_020d433c.mPlayerCount = 4;
                        data_020d433c.mUnk_99      = 3;
                        data_020d433c.mFieldId      = GetUnlockedFieldId(entry.mFieldId);
                        data_020d433c.mUnk_96      = entry.mUnk_09;
                        data_020d433c.mHasPartners      = 0;
                        data_020d433c.mCharacterIds[0]  = 0x15;
                        data_020d433c.mUnk_a3[0]      = 0;
                        data_020d433c.mCharacterIds[1]      = idx;
                        data_020d433c.mUnk_a3[1]      = func_ov004_0221b8c4(idx);
                        data_020d433c.mCharacterIds[2]      = entry.mUnk_0a;
                        data_020d433c.mUnk_a3[2]      = func_ov004_0221b8c4(entry.mUnk_0a);
                        data_020d433c.mCharacterIds[3]      = entry.mUnk_0b;
                        data_020d433c.mUnk_a3[3]      = func_ov004_0221b8c4(entry.mUnk_0b);
                        break;
                }
                data_020d433c.mUnk_9a = 0x11;
                data_020d433c.mUnk_9a_0 = gSaveData[0].mUnk_28_6;
                if (gSaveData[0].mUnk_5bf_0) {
                    if (entry.mProgressEntryIndex == 0x19 && idx == 7) {
                        data_020d433c.mUnk_a3[1] = 2;
                    }
                }
                data_020d433c.mUnk_8b_0 = mEntryIndex;
                data_020d433c.mUnk_8b_4 = gSaveData[0].mUnk_5b8;
                mState++;
            } else {
                mUnk_0f = 0;
                mState++;
            }
            SpriteManager::Get().StartFade(FADE_MODE_DOWN);
            break;
        }
        case 34:
            if (SpriteManager::Get().IsFading()) {
                break;
            }
            mEvent->RequestFinish();
            mContext->mRequest = 0;
            mState++;
            break;
        case 35:
            if (mContext->mRequest == 0) {
                break;
            }
            if (mUnk_0f != 0) {
                mState = 0x29;
                break;
            }
            Init();
            mState = 0x11;
            break;
        case 36:
            reinterpret_cast<ParkPopup *>(this)->Open("/font/mes_park.mes", 4, PARK_POPUP_THREE_ITEMS);
            ShowWindow();
            mState++;
            break;
        case 37:
            mWindow->mCursor->mSelection = -1;
            mWindow->mState            = 4;
            mWindow->Reset(1);
            mState++;
            break;
        case 38: {
            if ((u8) mWindow->Update(TRUE) != 1) {
                break;
            }
            HideWindow();
            if (mWindow->mCursor->mSelection == 0) {
                ParkEntry entry;
                GetParkEntry(mEntryIndex, &entry);
                if (entry.mUnk_0a_h == 0) {
                    if (gSaveData[0].mUnk_5be & 8) {
                        data_020d433c.mUnk_83 = 4;
                    } else {
                        data_020d433c.mUnk_83 = 3;
                    }
                } else {
                    data_020d433c.mUnk_83 = 4;
                }
                data_020d433c.mUnk_84   = 0;
                data_020d433c.mUnk_82   = 1;
                data_020d433c.mUnk_8b_0 = mEntryIndex;
                mUnk_0f                 = 0xff;
                mState++;
            } else {
                mUnk_0f = 0;
                mState++;
            }
            SpriteManager::Get().StartFade(FADE_MODE_DOWN);
            break;
        }
        case 39:
            if (SpriteManager::Get().IsFading()) {
                break;
            }
            mEvent->RequestFinish();
            mContext->mRequest = 0;
            mState++;
            break;
        case 40:
            if (mContext->mRequest == 0) {
                break;
            }
            if (mUnk_0f != 0) {
                mState = 0x29;
                break;
            }
            Init();
            mState = 0x11;
            break;
        case 41:
            if (mUnk_0f != 0) {
                data_020d433c.mNextScene = SCENE_PARK;
                data_020d433c.mUnk_7c = 1;
                if (mUnk_0f == 0xff) {
                    RequestScene(SCENE_TUTORIAL);
                    mFinished = TRUE;
                    return;
                }
                RequestScene(SCENE_BATTLE_PLAYER_LIST);
                mFinished = TRUE;
                return;
            }
            RequestScene(SCENE_MAP);
            mFinished = TRUE;
            return;
    }
    if (mState >= 0x13) {
        Draw();
    }
}

void ParkScene::Init() {
    InitDisplay();
    LoadBackground();
    mEvent = new ScriptPlayer(2, 0, 0);
}

void ParkScene::InitDisplay() {
    GX_SetDispSelect(1);
    SpriteManager::Get().SetupMode(1, 7);
    GX_SetGraphicsMode(1, 0, 1);
    GXS_SetGraphicsMode(0);
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
    LoadAndQueueUpload(1, UPLOAD_KIND_BG_EXT_PLTT, 0, "union_b_bg_window_ncl.bin", 0, 0, 0);
    LoadAndQueueUpload(1, UPLOAD_KIND_BG0_CHAR, 0, "union_b_bg_window_ncg.bin", 0, 0, 0);
    LoadAndQueueUpload(1, UPLOAD_KIND_BG0_SCR, 0, "union_b_bg_window_b_nsc.bin", 0, 0, 0);
    SpriteScreen &sub = SpriteManager::Get().Screen(1);
    ChangeDir("/Game/2D/union_b/");
    sub.LoadCharacters("union_b_obj_window.nce_ncg1D.bin", 0, 0);
    sub.LoadPalette("union_b_obj_window_ncl.bin", 0, 0);
    ChangeDir("/");
}

void ParkScene::ReloadBackground() {
    GXS_SetVisiblePlane(0x11);
    GXS_SetVisibleWnd(0);
    G2S_BlendNone();
    LoadBackground();
}

void ParkScene::LoadBackground() {
    ChangeDir("/Game/Park/");
    LoadAndQueueUpload(1, UPLOAD_KIND_BG_EXT_PLTT, 0, "park_b_bg_00_ncl.bin", 0, 0, 0);
    LoadAndQueueUpload(1, UPLOAD_KIND_BG0_CHAR, 0, "park_b_bg_01_ncg.bin", 0, 0, 0);
    LoadAndQueueUpload(1, UPLOAD_KIND_BG0_SCR, 0, "park_b_bg_01_nsc.bin", 0, 0, 0);
    LoadAndQueueUpload(1, UPLOAD_KIND_BG_EXT_PLTT, 0x4000, "park_b_bg_00_ncl.bin", 0, 0, 0);
    LoadAndQueueUpload(1, UPLOAD_KIND_BG2_CHAR, 0, "park_b_bg_00_ncg.bin", 0, 0, 0);
    LoadAndQueueUpload(1, UPLOAD_KIND_BG2_SCR, 0, "park_b_bg_00_nsc.bin", 0, 0, 0);
    ChangeDir("/");
    delete mMenu;
    mMenu = new ParkMenu(mEntryIndex);
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
    SpriteManager::Get().Screen(1).AddOam(mGlyphBuffer, 9, 0x20, 0x28, 0, 0x360, 0xf, 0);
}

u8 ParkScene::func_ov004_0221b8c4(int index) {
    if (TestFlagBit(data_020d4970, index)) {
        return 2;
    }
    return TestFlagBit(data_020d496d, index) ? 1 : 0;
}

int ParkScene::GetUnlockedFieldId(int index) {
    if (!TestFlagBit(gUnlockedFieldBits, index)) {
        index = 2;
        if (!(gSaveData[0].mUnk_578 & 4)) {
            index = 0;
        }
    }
    return index;
}

void ParkScene::ShowWindow() {
    GXS_SetVisiblePlane(0x13);
    mWindow->mEnabled = 1;
    mWindowVisible = 1;
    G2S_SetWnd0Position(0x10, 0x10, 0xf0, 0xb0);
    GXS_SetVisibleWnd(1);
    G2S_SetWnd0InsidePlane(0x13, FALSE);
    G2S_SetWndOutsidePlane(0x33, FALSE);
    G2S_SetBlendBrightness(0x11, -8);
}


void ParkScene::HideWindow() {
    GXS_SetVisiblePlane(0x11);
    mWindow->mEnabled = 0;
    mWindowVisible = 0;
    G2S_SetBlendBrightness(0x11, 0);
    GXS_SetVisibleWnd(0);
    G2S_SetWnd0InsidePlane(0x1f, FALSE);
    G2S_SetWndOutsidePlane(0x3f, FALSE);
}

void ParkPopup::Open(const char *mesPath, u32 index, u32 mode) {
    ChangeDir("/Game/2D/union_b/");
    LoadAndQueueUpload(1, UPLOAD_KIND_BG1_SCR, 0, "union_b_bg_window_b_nsc.bin", 0, 0, 0);
    LoadAndQueueUpload(1, UPLOAD_KIND_BG1_CHAR, 0, "union_b_bg_window_ncg.bin", 0, 0, 0);
    LoadAndQueueUpload(1, UPLOAD_KIND_BG_EXT_PLTT, 0x2000, "union_b_bg_window_ncl.bin", 0, 0, 0);
    SpriteScreen &screen = SpriteManager::Get().Screen(1);
    ChangeDir("/Game/2D/union_b/");
    screen.LoadCharacters("union_b_obj_window.nce_ncg1D.bin", 0x10000, 0);
    screen.LoadPalette("union_b_obj_window_ncl.bin", 0, 0);
    if (mMessages != NULL) {
        OS_Free(mMessages);
    }
    if (mTextBuffer != NULL) {
        OS_Free(mTextBuffer);
    }
    if (mGlyphBuffer != NULL) {
        OS_Free(mGlyphBuffer);
    }
    mMessages       = (MessageFile *) LoadFile(mesPath, 0, 0, 0, 0, 0);
    mTextBufferSize = TextManager::Get().BufferSize(0x18, 0xc);
    mGlyphCount     = TextManager::Get().GlyphCount(9);
    mTextBuffer     = OS_Alloc(mTextBufferSize);
    mGlyphBuffer    = OS_Alloc(mGlyphCount * 0xc);
    TextManager::Get().Init(mGlyphBuffer, 1, 0, 0x18, 0xc, 0x42);
    PrintMessage(index);
    delete mWindow;
    switch (mode) {
        case PARK_POPUP_THREE_ITEMS:
            mWindow = new SelectWindow(3, sThreeButtonItems, 1);
            mWindow->SetCells(3, sThreeButtonCells);
            break;
        case PARK_POPUP_TWO_ITEMS:
            mWindow = new SelectWindow(2, sTwoButtonItems, 0);
            mWindow->SetCells(2, sTwoButtonCells);
            break;
    }
    mWindow->mCursor->mSelection = -1;
    mWindow->mEnabled            = 0;
    ChangeDir("/");
}

void ParkPopup::PrintMessage(u32 index) {
    MI_CpuClear16(mTextBuffer, mTextBufferSize);
    TextManager &text = TextManager::Get();
    s16 height;
    s16 width;
    width             = 0;
    height            = 0;
    MessageFile *file = mMessages;
    text.Print(file->GetText(index), file->GetLength(index), mTextBuffer, 0x18, 0xc, &height, &width, 0, 0, 2, 0x42,
               0x10, 0, 0, 0, 1, 1);
    QueueUpload(SpriteManager::Get().Screen(1).mBgIndex, UPLOAD_KIND_OBJ, 0x1b000, mTextBuffer, mTextBufferSize, 0);
}

struct ParkRulesBuf {
    u8 pad[6];
    u8 mId;
    u8 pad2[5];
};

BOOL ParkRules::func_ov004_0221bf5c() {
    u8 lo = data_020d433c.mUnk_8b_0;
    u8 hi = data_020d433c.mUnk_8b_4;
    u8 saved = gSaveData[0].mUnk_5b8;
    ParkRulesBuf buf;
    gSaveData[0].mUnk_5b8 = hi;
    GetParkEntry(lo, &buf);
    u8 id = buf.mId;
    gSaveData[0].mUnk_5b8 = saved;
    BOOL ok = 1;
    if (id == 0xff) {
        return 0;
    }
    switch (id) {
        case 11: if (CountSlotsWith(0) != 3) ok = 0; break;
        case 12: if (CountSlotsWith(1) != 3) ok = 0; break;
        case 13: if (CountSlotsWith(2) != 3) ok = 0; break;
        case 14: if (CountSlotsWith(3) != 3) ok = 0; break;
        case 15: if (CountSlotsWith(4) != 3) ok = 0; break;
        case 16: if (CountSlotsWith(5) != 3) ok = 0; break;
        case 17: if (CountSlotsWith(0) != 0) ok = 0; break;
        case 18: if (CountSlotsWith(3) != 0) ok = 0; break;
        case 19: if (CountSlotsWith(1) != 0) ok = 0; break;
        case 20: if (CountSlotsWith(2) != 0) ok = 0; break;
        case 21: if (CountSlotsWith(4) != 0) ok = 0; break;
        case 22: if (CountSlotsWith(5) != 0) ok = 0; break;
        case 24: if (CountSlotsWith(4) != 0) ok = 0; break;
        case 25: if (data_020d433c.mUnk_8c_0 == 0) ok = 0; break;
    }
    if (ok) {
        UnlockProgressEntry(buf.mId);
        if (buf.mId == 0x19) {
            UnlockProgressEntry(0x1a);
        }
    }
    return ok;
}

u8 ParkRules::CountSlotsWith(u32 value) {
    u8 count = 0;
    for (u8 i = 0; i < 3; i++) {
        if (value == GetSlotBakuganAttribute(&gSaveData[0], 0, i)) {
            count++;
        }
    }
    return count;
}
