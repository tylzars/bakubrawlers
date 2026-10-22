#include "Battle/BattleCommand.hpp"
#include "Battle/BattleCharaAct.hpp"
#include "Battle/BattleCardEffect.hpp"
#include "Graphics/Camera.hpp"
#include "Battle/GimmickTypes.hpp"
#include "UI/StorytalkWindow.hpp"
#include "Battle/BattleCpu.hpp"
#include "Battle/BattleCommandPrompt.hpp"
#include "Battle/BattleCommandList.hpp"
#include "Battle/BattleCommandPage.hpp"
#include "Game/UnkStruct_020d433c.hpp"
#include "Game/SaveData.hpp"
#include "Graphics/CellAnimation.hpp"
#include "Graphics/Display.hpp"
#include "System/FileSystem.hpp"
#include "System/FrameUpdate.hpp"
#include "UI/MessageWindow.hpp"
#include "UI/SelectWindow.hpp"
#include "Graphics/AntennaIndicator.hpp"
#include <nitro/gx.h>
#include <nitro/mi.h>

struct BattleCommandPoint {
    u8 mX;
    u8 mY;
};
struct BattleScrollBounds {
    s16 mMin;
    s16 mMax;
};

static const WindowItemRecord sCockpitItems[2] = {
    {0, 0x5, 169, 170, 52, 20, 0, {-1, -1, -1, -1}, 0, WINDOW_TOUCH_RELEASE},
    {1, 0x2, 225, 170, 30, 20, 0, {-1, -1, -1, -1}, 0, WINDOW_TOUCH_RELEASE},
};

#include "System/SoundManager.hpp"
static const BattleScrollBounds sScrollXBounds[8] = {{0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 40}, {0, 0}, {0, 10}, {0, 0}};

static const BattleCommandPoint sCornerScreenPoints[4] = {{0xc2, 0x86}, {0x1e, 0x86}, {0x1e, 0x22}, {0xc2, 0x22}};

#include "System/GameSystem.hpp"
#include "System/InputManager.hpp"
inline WindowItemRecord *CommandItems() {
    static WindowItemRecord sItems[10] = {
        {0, WINDOW_ITEM_CURSOR_SKIP, 128, 96, 0, 0, 0, {-1, -1, -1, -1}, 0, 0},
        {1, 0x1, 222, 34, 32, 32, 1027, {-1, -1, -1, -1}, 0, WINDOW_TOUCH_RELEASE},
        {2, 0x1, 222, 66, 32, 32, 1027, {-1, -1, -1, -1}, 0, WINDOW_TOUCH_RELEASE},
        {3, WINDOW_ITEM_NO_SLIDE_OUT, 222, 98, 32, 32, 1027, {-1, -1, -1, -1}, 0, WINDOW_TOUCH_RELEASE},
        {4, 0x1, 222, 34, 32, 32, 1027, {-1, -1, -1, -1}, 0, WINDOW_TOUCH_RELEASE},
        {5, WINDOW_ITEM_NO_RESULT, 222, 66, 32, 32, 1027, {-1, -1, -1, -1}, 0, WINDOW_TOUCH_RELEASE},
        {6, WINDOW_ITEM_NO_SLIDE_OUT, 0, 0, 28, 28, 0, {-1, -1, -1, -1}, 0, WINDOW_TOUCH_RELEASE},
        {7, WINDOW_ITEM_NO_SLIDE_OUT, 0, 0, 28, 28, 0, {-1, -1, -1, -1}, 0, WINDOW_TOUCH_RELEASE},
        {8, WINDOW_ITEM_NO_SLIDE_OUT, 0, 0, 28, 28, 0, {-1, -1, -1, -1}, 0, WINDOW_TOUCH_RELEASE},
        {9, WINDOW_ITEM_NO_SLIDE_OUT, 0, 0, 28, 28, 0, {-1, -1, -1, -1}, 0, WINDOW_TOUCH_RELEASE},
    };
    return sItems;
}

#include "System/WirelessManager.hpp"
static const u8 sCommandItemX[2]                 = {0x04, 0xde};

static const BattleCommandPoint sStartCursorPoints[4] = {{6, 4}, {7, 4}, {7, 5}, {6, 5}};

// Cells of BattleCommand::mPromptWindow (its items are sPromptItems below; BattleCommandPrompt's own window uses
// sPromptWindowItems / Cells).
static const WindowCellRecord sPromptCells[3] = {
    {"battle_b_obj_00_nce.bin", 8, 0xa000, 0, 0, 1, {8, 11, 10, 0}},
    {"battle_b_obj_00_nce.bin", 0, 0xa000, 0, 0, 1, {0, 3, 2, 0}},
    {"battle_b_obj_00_nce.bin", 12, 0xa000, 0, 0, 1, {12, 15, 14, 0}},
};

static const WindowCellRecord sCommandCells[10] = {
    {"battle_b_obj_00_nce.bin", 20, 0xa000, 0, 0, 1, {20, 20, 20, 0}},
    {"battle_b_obj_00_nce.bin", 0, 0xa000, 0, 0, 1, {0, 3, 2, 0}},
    {"battle_b_obj_00_nce.bin", 8, 0xa000, 0, 0, 1, {8, 11, 10, 0}},
    {"battle_b_obj_00_nce.bin", 16, 0xa000, 0, 0, 1, {16, 19, 18, 0}},
    {"battle_b_obj_00_nce.bin", 4, 0xa000, 0, 0, 1, {4, 7, 6, 0}},
    {"battle_b_obj_00_nce.bin", 9, 0xa000, 0, 0, 1, {9, 9, 9, 0}},
    {"battle_b_obj_00_nce.bin", 28, 0xa000, 0, 0, 1, {28, 36, 32, 0}},
    {"battle_b_obj_00_nce.bin", 29, 0xa000, 0, 0, 1, {29, 37, 33, 0}},
    {"battle_b_obj_00_nce.bin", 30, 0xa000, 0, 0, 1, {30, 38, 34, 0}},
    {"battle_b_obj_00_nce.bin", 31, 0xa000, 0, 0, 1, {31, 39, 35, 0}},
};

static const WindowItemRecord sPromptItems[3] = {
    {0, WINDOW_ITEM_NO_SLIDE_OUT, 32, 4, 32, 32, 0, {-1, -1, -1, -1}, 0, WINDOW_TOUCH_RELEASE},
    {1, WINDOW_ITEM_NO_SLIDE_OUT, 64, 4, 32, 32, 0, {-1, -1, -1, -1}, 0, WINDOW_TOUCH_RELEASE},
    {2, WINDOW_ITEM_NO_SLIDE_OUT, 96, 4, 32, 32, 0, {-1, -1, -1, -1}, 0, WINDOW_TOUCH_RELEASE},
};

static const u16 sMarkerOam[6] = {0xa000, 0x8000, 0, 0, 0, 0};

static const WindowCellRecord sCockpitCells[2] = {
    {"/Game/2D/union_b/union_b_obj_cockpit_nce.bin", 0, 0x1c800, 14, 0, 1, {0, 1, 1, 0}},
    {"/Game/2D/union_b/union_b_obj_cockpit_nce.bin", 2, 0x1c800, 14, 0, 1, {2, 3, 3, 0}},
};

static const BattleScrollBounds sScrollYBounds[8] = {{0, 0}, {0, 0}, {-20, 20}, {0, 0}, {-40, 40}, {-20, 20}, {-40, 40}, {0, 0}};

#include "Graphics/SpriteManager.hpp"
static const s16 sSubBg0Offset[2] = {0x82, 0x9e};

#include "System/TouchManager.hpp"

void CloseCommandMap(BattleCommandMap *map);

void CloseCommandMapNow(BattleCommandMap *map);

void InitCommandMainScreen(BattleCommand *command, u32 mode);

void InitCommandSubScreen(int x, int y);

u32 GetTexVariant(u32 a, u32 b);

void RemoveMarker(BattlePlayerList *list, u8 index);

extern "C" BOOL func_ov007_02263150(BattlePlayerList *list);

void SetupMapCamera(BattleCommandMap *map, u32 a);

BOOL IsMarkerPlaceable(BattlePlayerList *list, u8 x, u8 y);

extern "C" void func_ov007_022314bc(BattleCpu *cpu);

void ApplyScrollOffset(void *arg);

extern "C" void SetupTimedF00gatModelAndCamera(BattleCommandMap *map);

void AddMarker(BattlePlayerList *list, u8 x, u8 y, u8 player, u8 slot);

BOOL SetMessageTypeAndCheckAll(BattlePlayerList *list, u32 id);

class BattlePopup {
public:
    u8 mPad[0x1c];
    BattlePopup(u8 planes);
};

void func_ov007_022470e4(BattleCommandMap *map, u8 a, u8 b);

u16 *GetMarkerSlotValue(BattlePlayerList *list, u8 index);

// Record of a battle cell returned by GetUnitRecordAt.
struct BattleCellInfo {
    /* 00 */ u16 mUnk_00;
    /* 02 */ u16 mUnk_02;
    /* 04 */ u16 mUnk_04;
    /* 06 */ u16 mUnk_06;
    /* 08 */ u8 mBakuganId;
    /* 09 */ u8 mAttribute;
};

BattleCellInfo *GetUnitRecordAt(BattlePlayerList *list, u8 x, u8 y);

extern "C" void RotateCameraYaw(BattleCommandMap *map, u32 direction);

BattleCommand::BattleCommand(u32 unk)
    : SceneBase(0x3e8, 0) {
    mMessageWindow = 0;
    mCommandPrompt  = 0;
    mUnk_11c = 0;
    mAddMarkerFrames = 0;
    mScrollX = 0;
    mScrollY = 0;
    mDragScrolling = 0;
    mMode    = COMMAND_MODE_MAP;
    mNextMode = COMMAND_MODE_NONE;
    mCursorX = 0;
    mCursorY = 0;
    mUnk_12e = 0;
    mBlendBrightness = 0;
    mPromptStep = 0;
    mOffsetX = 0;
    mUnk_13a = 0;
    mReleasedOnCursorCell = 0;
    if (gSaveData[0].mCommandItemXIndex) {
        mOffsetX = 0;
    } else {
        mOffsetX = 4;
    }
    u8 side = data_020d433c.mBattlePlayers->mPlayers[data_020d433c.mBattlePlayers->mLocalPlayer]->mCorner;
    if (side <= 1) {
        mFlipped = 0;
    } else {
        mFlipped = 1;
    }
    u8 i = 0;
    do {
        u8 f = data_020d433c.mBattlePlayers->mPlayers[i]->mCorner;
        if (mFlipped) {
            f = (f + 2) % 4;
        }
        CommandItems()[i + 6].mX = mOffsetX + sCornerScreenPoints[f].mX;
        CommandItems()[i + 6].mY = sCornerScreenPoints[f].mY;
        i++;
    } while (i < data_020d433c.mPlayerCount);
    if (data_020d433c.mBattlePlayers->mTurnPlayer != data_020d433c.mBattlePlayers->mLocalPlayer) {
        CommandItems()[1].mUnk_04 = WINDOW_ITEM_CURSOR_SKIP;
        CommandItems()[4].mUnk_04 = WINDOW_ITEM_CURSOR_SKIP;
        CommandItems()[2].mUnk_04 = WINDOW_ITEM_CURSOR_SKIP;
        CommandItems()[5].mUnk_04 = WINDOW_ITEM_CURSOR_SKIP;
    } else {
        if (data_020d433c.mBattlePlayers->mUnk_292 == 1) {
            CommandItems()[1].mY      = 0xc0;
            CommandItems()[1].mUnk_04 = WINDOW_ITEM_CURSOR_SKIP;
            CommandItems()[4].mY      = 0x22;
            CommandItems()[4].mUnk_04 = 1;
        } else {
            CommandItems()[1].mY      = 0x22;
            CommandItems()[1].mUnk_04 = 1;
            CommandItems()[4].mY      = 0xc0;
            CommandItems()[4].mUnk_04 = WINDOW_ITEM_CURSOR_SKIP;
        }
        if (data_020d433c.mBattlePlayers->mAddMarkerEnabled != 0) {
            CommandItems()[2].mUnk_04 = 1;
        } else {
            CommandItems()[2].mUnk_04 = WINDOW_ITEM_CURSOR_SKIP;
            CommandItems()[5].mUnk_04 = WINDOW_ITEM_NO_RESULT;
        }
    }
    if (data_020d433c.mUnk_9a_3) {
        CommandItems()[2].mUnk_04 = WINDOW_ITEM_CURSOR_SKIP;
        CommandItems()[5].mUnk_04 = WINDOW_ITEM_CURSOR_SKIP;
    }
    u8 x = sCommandItemX[gSaveData[0].mCommandItemXIndex];
    i = 1;
    do {
        CommandItems()[i].mX = x;
        if (gSaveData[0].mCommandItemXIndex == 0) {
            CommandItems()[i].mSlideIn  = 4;
            CommandItems()[i].mSlideOut = 3;
        }
        i++;
    } while (i <= 5);
    mCommandWindow = new SelectWindow(10, CommandItems(), -1);
    mCockpitWindow = new SelectWindow(2, sCockpitItems, -1);
    mPromptWindow = new SelectWindow(3, sPromptItems, -1);
    mCamera = unk;
    mUnk_14 = 0;
    mUnk_10 = 0;
    mCursorAnim = 0;
    mLocalPlayerCellAnim = 0;
    mPartnerCellAnim = 0;
    mTutorialCellAnim = 0;
    mTurnCellAnim = 0;
    mTurnDigitsCellAnim = 0;
    i = 0;
    do {
        mUnitCellAnims[i] = 0;
        i++;
    } while (i < 12);
    i = 0;
    do {
        mPlayerCellAnims[i] = 0;
        i++;
    } while (i < 4);
    mGridTouchX = 0;
    mGridTouchY = 0;
    mPlayer = data_020d433c.mBattlePlayers->mTurnPlayer;
    mState = 0;
    mCursorX = sStartCursorPoints[side].mX;
    mCursorY = sStartCursorPoints[side].mY;
    data_020d433c.mBattlePlayers->SetCursor(mCursorX, mCursorY);

    InitGridMask();
    if (data_020d433c.mBattlePlayers->mUnitModels) {
        data_020d433c.mBattlePlayers->mUnitModels->mDrawMode = 0;
    }
}

BattleCommand::~BattleCommand() {
    if (mCommandPrompt) {
        CloseCommandPrompt(mCommandPrompt, 1);
    }
    if (mUnk_14) {
        delete mUnk_14;
    }
    if (mUnk_10) {
        delete mUnk_10;
    }
    if (mCursorAnim) {
        delete mCursorAnim;
    }
    if (mLocalPlayerCellAnim) {
        delete mLocalPlayerCellAnim;
    }
    if (mPartnerCellAnim) {
        delete mPartnerCellAnim;
    }
    if (mTutorialCellAnim) {
        delete mTutorialCellAnim;
    }
    if (mTurnCellAnim) {
        delete mTurnCellAnim;
    }
    if (mTurnDigitsCellAnim) {
        delete mTurnDigitsCellAnim;
    }
    u8 i = 0;
    do {
        CellAnimation *a = mUnitCellAnims[i];
        if (a) {
            delete a;
        }
        i++;
    } while (i < 12);
    CellAnimation *b;
    u8 j = 0;
    do {
        b = mPlayerCellAnims[j];
        if (b) {
            delete b;
        }
        j++;
    } while (j < 4);
    if (mCommandWindow) {
        delete mCommandWindow;
    }
    if (mCockpitWindow) {
        delete mCockpitWindow;
    }
    if (mPromptWindow) {
        delete mPromptWindow;
    }
    SpriteManager::Get().ResetSlots();
    if (mMessageWindow) {
        delete mMessageWindow;
    }
}

void BattleCommand::Update() {
    switch (mState) {
        case 0: {
            BattlePlayerList *list = data_020d433c.mBattlePlayers;
            if (list->mTurnCount == 0) {
                list->mTurnCount++;
                if (data_020d433c.mGimmicks) {
                    data_020d433c.mGimmicks->CountTurnAll();
                }
            } else {
                InitCommandMainScreen(this, 1);
                InitCommandSubScreen((s16) (mScrollX - mOffsetX), mScrollY);
            }
            func_ov007_0223a734();
            LoadCommandGraphics(mFlipped);
            LoadTurnGraphics();
            SpriteManager::Get().StartFade(FADE_MODE_TO_ZERO);
            mState = 1;
            return;
        }
        case 1:
            if (SpriteManager::Get().IsFading()) {
                return;
            }
            mState++;
            return;
        case 2: {
            BattlePlayerList *list = data_020d433c.mBattlePlayers;
            if (list->mUnk_293 == 0) {
                list->mUnk_293 = 1;
                new BattleCardEffect(0, 10, 0x11, 1);
                mContext->mRequest = 0;
            } else {
                mContext->mRequest = 1;
            }
            if (data_020d433c.mBattlePlayers->mUnk_292 == 2) {
                mState = 3;
                return;
            }
            mState = 7;
            return;
        }
        case 3:
            if (mContext->mRequest == 0) {
                return;
            }
            CloseMode(1);
            SpriteManager::Get().StartFade(FADE_MODE_DOWN);
            mState++;
            return;
        case 4: {
            if (SpriteManager::Get().IsFading()) {
                return;
            }
            PollModeClosed();
            if (mMode != COMMAND_MODE_NONE) {
                return;
            }
            u32 unk4 = 0;
            BattlePlayerList *list = data_020d433c.mBattlePlayers;
            if (list->mUnk_292 == 2) {
                u8 player = mPlayer;
                u8 i = 0;
                do {
                    BattleUnit *unit = &list->mUnits[i];
                    if (player == unit->mPlayer && unit->mUnk_12 != 0) {
                        BattleUnit *u2 = &data_020d433c.mBattlePlayers->mUnits[i];
                        u8 slot = u2->mRecord;
                        u8 marker = u2->mMarker;
                        RemoveUnit(data_020d433c.mBattlePlayers, i, 2);
                        BattlePlayer *p = data_020d433c.mBattlePlayers->mPlayers[mPlayer];
                        p->mUnk_fe = 1;
                        p->mUnk_ee++;
                        p->mRecords[slot].mStatus = 0;
                        BattleRecord *rec = &p->mRecords[slot];
                        unk4 = GetTexVariant(rec->mBakuganId, rec->mAttribute);
                        RemoveMarker(data_020d433c.mBattlePlayers, marker);
                        break;
                    }
                    i++;
                } while (i < 12);
            }
            new BattleCharaAct((Camera *) mCamera, mPlayer, 7, unk4, 0);
            mContext->mRequest = 0;
            mState++;
            return;
        }
        case 5: {
            if (mContext->mRequest == 0) {
                return;
            }
            if (data_020d433c.mBattlePlayers->mUnk_292 == 2) {
                data_020d433c.mBattlePlayers->mUnk_292 = 0;
                if (func_ov007_02263150(data_020d433c.mBattlePlayers)) {
                    mState = 0x37;
                    SpriteManager::Get().StartFade(FADE_MODE_DOWN);
                    return;
                }
                u8 i = 0;
                u8 found = 0;
                BattlePlayerList *list = data_020d433c.mBattlePlayers;
                do {
                    if (list->mMarkers[i].mStatus != 0) {
                        found = 1;
                        break;
                    }
                    i++;
                } while (i < 12);
                if (found == 0) {
                    mState = 0x38;
                    SpriteManager::Get().StartFade(FADE_MODE_DOWN);
                    return;
                }
            }
            mNextMode = COMMAND_MODE_MAP;
            OpenNextMode(0);
            InitCommandSubScreen((s16) (mScrollX - mOffsetX), mScrollY);
            LoadCommandGraphics(mFlipped);
            LoadTurnGraphics();
            CreateCursorModel(mCommandMap);
            SetupMapCamera(mCommandMap, 2);
            SpriteManager::Get().StartFade(FADE_MODE_TO_ZERO);
            mState++;
            return;
        }
        case 6:
            if (SpriteManager::Get().IsFading()) {
                return;
            }
            mState++;
            mContext->mRequest = 1;
            return;
        case 7: {
            if (mContext->mRequest == 0) {
                return;
            }
            BattlePlayerList *list = data_020d433c.mBattlePlayers;
            u8 cur                 = list->mLocalPlayer;
            if (list->mTurnPlayer == cur) {
                if (data_020d433c.mTutorialId == 3 || data_020d433c.mTutorialId == 7) {
                    list->mStorytalkWindow->LoadGraphics();
                    data_020d433c.mBattlePlayers->mStorytalkWindow->SetMessage(data_020d433c.mBattlePlayers->mStorytalkMessageIndex, 0x64, 0, 0);
                    mState++;
                    G2S_SetBlendBrightness(0x1f, -10);
                    return;
                }
                if (data_020d433c.mHasPartners != 0 && data_020d433c.mUnk_9a_3) {
                    u8 i = 0;
                    u8 count = 0;
                    u8 need = list->mMarkerCount;
                    u8 partner = list->mPlayers[cur]->mPartner;
                    do {
                        BattleUnit *unit = &list->mUnits[i];
                        if (unit->mStatus != 0) {
                            if (partner == unit->mPlayer) {
                                count++;
                            }
                        }
                        i++;
                    } while (i < 12);
                    if (count == need) {
                        if (list->mAddMarkerEnabled == 0) {
                            mState = 10;
                        } else {
                            mState = 9;
                        }
                    } else {
                        mState = 10;
                    }
                } else {
                    mState = 10;
                }
            } else {
                BOOL active = list->mPlayers[mPlayer]->IsCpu();
                if (!active) {
                    mState = 0xe;
                } else {
                    mState = 0x10;
                    mUnk_11c = 0;
                }
            }
            SetCommandMessage();
            mMessageWindow->UploadTextPalette();
            mCommandWindow->mEnabled = 1;
            return;
        }

        case 8: {
            if (InputManager::Get().IsPressed(1) || TouchManager::Get().mReleased) {
                data_020d433c.mBattlePlayers->mStorytalkWindow->NextPage(1);
            }
            if (data_020d433c.mBattlePlayers->mStorytalkWindow->func_02087cac()) {
                return;
            }
            G2S_SetBlendBrightness(0x1f, 0);
            data_020d433c.mBattlePlayers->mStorytalkMessageIndex++;
            SetCommandMessage();
            mMessageWindow->UploadTextPalette();
            mCommandWindow->mEnabled = 1;
            BattlePlayerList *list = data_020d433c.mBattlePlayers;
            if (mPlayer == list->mLocalPlayer) {
                mState = 10;
                return;
            }
            BOOL active = list->mPlayers[mPlayer]->IsCpu();
            if (!active) {
                mState = 0xe;
                return;
            }
            mState = 0x10;
            mUnk_11c = 0;
            return;
        }
        case 9: {
            u8 i = 0;
            BattlePlayer *p = data_020d433c.mBattlePlayers->mPlayers[mPlayer];
            u8 count = data_020d433c.mSlotsPerPlayer;
            do {
                if (p->mSlots54[i].mFlag == 0) {
                    mPickIndex = i;
                    break;
                }
                i++;
            } while (i < count);
            u8 x = 0;
            do {
                u8 y = 0;
                do {
                    if (IsMarkerPlaceable(data_020d433c.mBattlePlayers, x, y)) {
                        mCursorX = x;
                        mCursorY = y;
                        break;
                    }
                    y++;
                } while (y < 9);
                x++;
            } while (x < 14);
            u8 b                   = mCursorY;
            u8 a                   = mCursorX;
            BattlePlayerList *list = data_020d433c.mBattlePlayers;
            list->mCursorX         = a;
            list->mCursorY         = b;
            SpriteManager::Get().StartFade(FADE_MODE_DOWN);
            mState = 0x1d;
            return;
        }
        case 10: {
            BOOL ok = TRUE;
            if (mMode == COMMAND_MODE_PAGE) {
                if (mCommandPage->IsSettingUp()) {
                    ok = FALSE;
                }
            }
            if (mMode == COMMAND_MODE_LIST) {
                if (mCommandList->IsSettingUp()) {
                    ok = FALSE;
                }
            }
            if (data_020d433c.mTutorialNumber != 0) {
                ok = FALSE;
            }
            if (ok) {
                if (InputManager::Get().IsPressed(8)) {
                    mState = 0x31;
                    return;
                }
            }
            if (!UpdateCommandWindowAndCursor(1)) {
                return;
            }
            switch (mSelectedId) {
                case 3:
                    mState = 0xb;
                    return;
                case 6:
                case 7:
                case 8:
                case 9:
                    mState = 0xc;
                    return;
                default:
                    mState = 0xd;
                    return;
            }
        }
        case 11:
            UpdateCommandWindowAndCursor(0);
            if (!SwitchMode()) {
                return;
            }
            mState = 10;
            mDragScrolling = 0;
            return;
        case 12:
            if (UpdatePrompt()) {
                mState = 10;
            }
            return;
        case 13: {
            mCommandWindow->Update(1);
            if (mCommandWindow->mState == 4) {
                switch (mSelectedId) {
                    case 1:
                        mState = 0x25;
                        CloseMode(1);
                        mNextMode = COMMAND_MODE_LIST;
                        break;
                    case 2: {
                        BattlePlayerList *list = data_020d433c.mBattlePlayers;
                        if (list->mAddMarkerEnabled != 0) {
                            u8 cur   = list->mLocalPlayer;
                            u8 i     = 0;
                            u8 count = data_020d433c.mSlotsPerPlayer;
                            BattlePlayer *p = list->mPlayers[cur];
                            do {
                                if (p->mSlots54[i].mFlag == 0) {
                                    CloseMode(1);
                                    mCockpitWindow->mEnabled = 0;
                                    mState          = 0x17;
                                    break;
                                }
                                i++;
                            } while (i < count);
                        }
                        break;
                    }
                    case 4:
                        CloseMode(1);
                        mNextMode = COMMAND_MODE_LIST;
                        mCockpitWindow->mEnabled = 0;
                        mState          = 0x29;
                        break;
                }
            }
            u8 b                   = mCursorY;
            BattlePlayerList *list = data_020d433c.mBattlePlayers;
            u8 a                   = mCursorX;
            list->mCursorX         = a;
            list->mCursorY         = b;
            Draw();
            mMessageWindow->Update();
            return;
        }
        case 14: {
            BattlePlayerList *list = data_020d433c.mBattlePlayers;
            BattlePlayer *p        = list->mPlayers[list->mTurnPlayer];
            switch (p->mMsg.mType) {
                case 7: {
                    if (mPromptStep != 0) {
                        mPromptStep = 9;
                        if (mCommandPrompt) {
                            CloseCommandPrompt(mCommandPrompt, 1);
                            mCommandPrompt = 0;
                        }
                    }
                    mPickIndex = *(u32 *) p->mMsg.mUnk_04;
                    mCursorX = p->mMsg.mUnk_08;
                    u8 b     = p->mMsg.mUnk_09;
                    mCursorY = b;
                    u8 a     = mCursorX;
                    BattlePlayerList *l = data_020d433c.mBattlePlayers;
                    l->mCursorX = a;
                    l->mCursorY = b;
                    mNextMode = COMMAND_MODE_NONE;
                    CloseMode(1);
                    SpriteManager::Get().StartFade(FADE_MODE_DOWN);
                    mState = 0xf;
                    return;
                }
                case 8:
                    if (mPromptStep != 0) {
                        mPromptStep = 0;
                        if (mCommandPrompt) {
                            CloseCommandPrompt(mCommandPrompt, 1);
                            mCommandPrompt = 0;
                        }
                    }
                    mNextMode = COMMAND_MODE_NONE;
                    CloseMode(0);
                    SpriteManager::Get().SetFade(FADE_MODE_HOLD, -16);
                    {
                        BattlePlayerList *l = data_020d433c.mBattlePlayers;
                        l->mPlayers[l->mTurnPlayer]->mRecord = p->mMsg.mUnk_0e;
                    }
                    mState = 0x35;
                    return;
                case 0x12:
                    if (mPromptStep != 0) {
                        mPromptStep = 0;
                        if (mCommandPrompt) {
                            CloseCommandPrompt(mCommandPrompt, 1);
                            mCommandPrompt = 0;
                        }
                    }
                    mCursorX  = p->mMsg.mCursorX;
                    mCursorY  = p->mMsg.mCursorY;
                    mNextMode = COMMAND_MODE_NONE;
                    mState  = 0x2d;
                    return;
                default:
                    mCommandWindow->mButtons[1].mEnabled = 0;
                    mCommandWindow->mButtons[2].mEnabled = 0;
                    mCommandWindow->mButtons[5].mEnabled = 0;
                    mCommandWindow->mButtons[4].mEnabled = 0;
                    mCommandWindow->mButtons[3].mEnabled = 1;
                    if (mPromptStep == 0) {
                        if ((mMode == COMMAND_MODE_NONE) | (mMode == COMMAND_MODE_CLOSING)) {
                            UpdateCommandWindowAndCursor(0);
                            SwitchMode();
                            return;
                        }
                        if (!UpdateCommandWindowAndCursor(1)) {
                            return;
                        }
                        switch (mSelectedId) {
                            case 6:
                            case 7:
                            case 8:
                            case 9:
                                UpdatePrompt();
                                return;
                        }
                        break;
                    }
                    UpdatePrompt();
                    return;
            }
            break;
        }
        case 15:
            PollModeClosed();
            if (mMode != COMMAND_MODE_NONE) {
                return;
            }
            Draw();
            func_ov007_0223b874();
            mCockpitWindow->Draw();
            if (SpriteManager::Get().IsFading()) {
                return;
            }
            mCommandWindow->mButtons[3].mEnabled = 0;
            mCommandWindow->mState = 4;
            mCommandWindow->mSlideInRequested = 0;
            mMode     = COMMAND_MODE_NONE;
            mAddMarkerFrames  = 0;
            mState  = 0x1f;
            return;
        case 16: {
            mUnk_11c++;
            if (mUnk_11c == 0) {
                return;
            }
            mCommandWindow->mButtons[1].mEnabled = 0;
            mCommandWindow->mButtons[2].mEnabled = 0;
            mCommandWindow->mButtons[5].mEnabled = 0;
            mCommandWindow->mButtons[4].mEnabled = 0;
            mUnk_11c                      = 0;
            u8 r = data_020d433c.mBattlePlayers->Current()->mCpu->TryPickUnflaggedSlot54();
            if (r == 0xff || data_020d433c.mBattlePlayers->mAddMarkerEnabled == 0) {
                mState = 0x12;
                return;
            }
            mPickIndex = r;
            u8 v     = data_020d433c.mBattlePlayers->Current()->mCpu->PickMarkerCell();
            u8 hi    = v >> 4;
            u8 lo    = v & 0xf;
            if (mGridMask[hi][lo] != 0) {
                u8 b                   = mCursorY;
                BattlePlayerList *list = data_020d433c.mBattlePlayers;
                u8 a                   = mCursorX;
                list->mCursorX         = a;
                list->mCursorY         = b;
                CloseMode(1);
                SpriteManager::Get().StartFade(FADE_MODE_DOWN);
                mCursorX = hi;
                mCursorY = lo;
                mState++;
                return;
            }
            mState = 0x12;
            return;
        }
        case 17:
            if (SpriteManager::Get().IsFading()) {
                return;
            }
            PollModeClosed();
            if (mMode != COMMAND_MODE_NONE) {
                return;
            }
            Draw();
            func_ov007_0223b874();
            mCockpitWindow->Draw();
            if (SpriteManager::Get().IsFading()) {
                return;
            }
            mMode    = COMMAND_MODE_NONE;
            mAddMarkerFrames = 0;
            mState = 0x1f;
            return;
        case 18:
            Draw();
            mUnk_11c++;
            if (mUnk_11c == 0) {
                return;
            }
            mUnk_11c = 0;
            CloseMode(1);
            SpriteManager::Get().StartFade(FADE_MODE_DOWN);
            if (data_020d433c.mBattlePlayers->mUnk_292 != 0) {
                mState = 0x15;
                return;
            }
            mState++;
            return;
        case 19: {
            Draw();
            if (SpriteManager::Get().IsFading()) {
                return;
            }
            PollModeClosed();
            if (mMode != COMMAND_MODE_NONE) {
                return;
            }
            u8 cur          = data_020d433c.mBattlePlayers->mTurnPlayer;
            BattlePlayer *p = data_020d433c.mBattlePlayers->mPlayers[cur];
            u8 sel          = p->mCpu->PickRecord();
            func_ov007_022314bc(p->mCpu);
            BattleRecord *rec = &p->mRecords[sel];
            p->mRecord = sel;
            u32 act    = GetTexVariant(rec->mBakuganId, rec->mAttribute);
            u8 kind    = 3;
            if (p->mCpu->mSpKind != 0) {
                kind = 0xf;
            }
            new BattleCharaAct((Camera *) mCamera, cur, kind, act, 0);
            mContext->mRequest = 0;
            mState++;
            return;
        }
        case 20:
            if (mContext->mRequest == 0) {
                return;
            }
            mNextScene = COMMAND_RESULT_CPU_SHOOT;
            mState = 0x39;
            return;
        case 21: {
            u8 i                   = 0;
            BattlePlayerList *list = data_020d433c.mBattlePlayers;
            u32 cur                 = list->mTurnPlayer;
            do {
                if ((u8) cur == list->mUnits[i].mPlayer) {
                    list->mUnits[i].mUnk_12 = 1;
                    BattlePlayerList *l = data_020d433c.mBattlePlayers;
                    u8 idx              = l->mUnits[i].mMarker;
                    mCursorX            = l->mMarkers[idx].mCellX;
                    mCursorY            = data_020d433c.mBattlePlayers->mMarkers[idx].mCellY;
                    break;
                }
                i++;
            } while (i < 12);
            mState++;
            return;
        }
        case 22:
            mState = 0x2d;
            return;
        case 23:
            PollModeClosed();
            if (mMode != COMMAND_MODE_NONE) {
                return;
            }
            mNextMode = COMMAND_MODE_PAGE;
            OpenNextMode(0);
            new BattleCommandPrompt(1, mCommandPage, 0, 0, 0);
            mContext->mRequest = 0;
            mState++;
            return;
        case 24: {
            u16 v = mContext->mRequest;
            if (v == 0) {
                return;
            }
            if (v != 10) {
                GXS_SetVisiblePlane(0x11);
                mPickIndex = GetLowByte(v);
                mState++;
                return;
            }
            CloseMode(1);
            mState = 0x23;
            return;
        }
        case 25:
            mCommandWindow->Reset(-1);
            mCockpitWindow->mEnabled = 1;
            mMessageWindow->StartMessage(0x13, 0, 1);
            mMessageWindow->UploadTextPalette();
            mState++;
            return;
        case 26: {
            if (mCockpitWindow->mState == 1) {
                UpdateCursorScrollAndCamera(1);
            }
            WindowItem *item = mCockpitWindow->mCursor->mItems;
            u8 flag          = 0;
            if (IsMarkerPlaceable(data_020d433c.mBattlePlayers, mCursorX, mCursorY)) {
                item->mFlags = 5;
                if (mReleasedOnCursorCell != 0) {
                    flag = 1;
                    SoundManager::Get().StartSeqArc(0);
                }
            } else {
                item->mFlags = 0x804;
                if (mReleasedOnCursorCell != 0) {
                    SoundManager::Get().StartSeqArc(2);
                }
            }
            u8 sel    = mCockpitWindow->Update(1);
            mUnk_13a = mCockpitWindow->mUnk_0e0;
            if (sel == 1 || flag != 0) {
                mBlendBrightness = 0;
                mAddMarkerFrames = 0;
                SpriteManager::Get().StartFade(FADE_MODE_DOWN);
                mState = 0x1c;
                if (flag != 0) {
                    mCockpitWindow->mState = 4;
                    mCockpitWindow->mSlideInRequested = 0;
                }
                mDragScrolling = 0;
            } else if (sel == 2) {
                mState++;
                mDragScrolling = 0;
            }
            mMessageWindow->Update();
            u8 b                   = mCursorY;
            BattlePlayerList *list = data_020d433c.mBattlePlayers;
            u8 a                   = mCursorX;
            list->mCursorX         = a;
            list->mCursorY         = b;
            Draw();
            func_ov007_0223b874();
            mCockpitWindow->Draw();
            QueueFrameCallback(ApplyScrollOffset, this);
            return;
        }

        case 27:
            mCockpitWindow->Update(1);
            if (mCockpitWindow->mState == 4) {
                new BattleCommandPrompt(1, mCommandPage, 0, 0, 0);
                mContext->mRequest = 0;
                mCockpitWindow->Reset(-1);
                mCockpitWindow->mEnabled = 0;
                mState          = 0x18;
            }
            mMessageWindow->Update();
            {
                u8 b                   = mCursorY;
                BattlePlayerList *list = data_020d433c.mBattlePlayers;
                u8 a                   = mCursorX;
                list->mCursorX         = a;
                list->mCursorY         = b;
            }
            Draw();
            func_ov007_0223b874();
            mCockpitWindow->Draw();
            QueueFrameCallback(ApplyScrollOffset, this);
            return;
        case 28:
            mCockpitWindow->Update(1);
            if (mCockpitWindow->mState == 4) {
                mState++;
            }
            Draw();
            func_ov007_0223b874();
            return;
        case 29:
            data_0220f540.mUnk_00 = 7;
            data_0220f540.mUnk_04 = mPickIndex;
            data_0220f540.mUnk_08 = mCursorX;
            data_0220f540.mUnk_09 = mCursorY;
            mState++;
            Draw();
            func_ov007_0223b874();
            return;
        case 30:
            Draw();
            func_ov007_0223b874();
            mCockpitWindow->Draw();
            if (SpriteManager::Get().IsFading()) {
                return;
            }
            CloseMode(1);
            mAddMarkerFrames = 0;
            mState++;
            mCockpitWindow->mEnabled = 0;
            return;
        case 31: {
            PollModeClosed();
            if (mMode != COMMAND_MODE_NONE) {
                return;
            }
            u8 cur = data_020d433c.mBattlePlayers->mTurnPlayer;
            new BattleCharaAct((Camera *) mCamera, cur, 1, 0, 0);
            mContext->mRequest = 0;
            GXS_SetVisibleWnd(0);
            mState++;
            return;
        }
        case 32: {
            if (mContext->mRequest == 0) {
                return;
            }
            mNextMode = COMMAND_MODE_MAP;
            OpenNextMode(0);
            InitCommandSubScreen((s16) (mScrollX - mOffsetX), mScrollY);
            LoadCommandGraphics(mFlipped);
            LoadTurnGraphics();
            u8 b                   = mCursorY;
            BattlePlayerList *list = data_020d433c.mBattlePlayers;
            u8 a                   = mCursorX;
            list->mCursorX         = a;
            list->mCursorY         = b;
            SetupTimedF00gatModelAndCamera(mCommandMap);
            mUnk_12e = 1;
            data_020d433c.mGimmicks->mState = 0;
            SpriteManager::Get().StartFade(FADE_MODE_TO_ZERO);
            mState++;
            return;
        }
        case 33: {
            Draw();
            func_ov007_0223b874();
            mAddMarkerFrames++;
            if ((mAddMarkerFrames % 4) == 0) {
                if (mBlendBrightness < 16) {
                    mBlendBrightness++;
                }
            }
            if (mAddMarkerFrames == 1) {
                SoundManager::Get().StartSeqArc(6);
            }
            if ((s32) mAddMarkerFrames <= 0x96) {
                return;
            }
            mBlendBrightness = 0;
            mAddMarkerFrames = 0;
            mState++;
            AddMarker(data_020d433c.mBattlePlayers, mCursorX, mCursorY, mPlayer, mPickIndex);
            mUnk_130 = 1;
            SpriteManager::Get().sFade[0] = FADE_MODE_DOWN;
            data_020d433c.mBattlePlayers->mAddMarkerEnabled = 0;
            WindowItem *items = mCommandWindow->mCursor->mItems;
            items[2].mFlags   = WINDOW_ITEM_CURSOR_SKIP;
            BattlePlayerList *list = data_020d433c.mBattlePlayers;
            if (list->mTurnPlayer != list->mLocalPlayer) {
                items[5].mFlags = WINDOW_ITEM_CURSOR_SKIP;
            } else {
                items[5].mFlags = WINDOW_ITEM_NO_RESULT;
            }
            return;
        }
        case 34:
            if (SpriteManager::Get().sFade[0] <= FADE_MODE_HOLD) {
                GXS_SetVisibleWnd(0);
                mUnk_12e = 0;
                G2S_SetBlendBrightness(0x11, 0);
                CloseMode(1);
                mState++;
            }
            Draw();
            return;
        case 35:
            Draw();
            PollModeClosed();
            if (mMode != COMMAND_MODE_NONE) {
                return;
            }
            mNextMode = COMMAND_MODE_MAP;
            OpenNextMode(0);
            CreateCursorModel(mCommandMap);
            SetupMapCamera(mCommandMap, 2);
            mState++;
            return;
        case 36: {
            Draw();
            if (SpriteManager::Get().sFade[0] > FADE_MODE_HOLD) {
                return;
            }
            mCommandWindow->Reset(-1);
            mCommandWindow->Update(1);
            mMessageWindow->UploadTextPalette();
            SetCommandMessage();
            mUnk_13a = 0;
            BattlePlayerList *list = data_020d433c.mBattlePlayers;
            u32 cur                 = list->mTurnPlayer;
            if (cur == list->mLocalPlayer) {
                mState = 10;
                return;
            }
            BOOL active = list->mPlayers[(u8) cur]->IsCpu();
            if (!active) {
                mState = 0xe;
                return;
            }
            mState = 0x12;
            mUnk_11c = 0;
            return;
        }
        case 37:
            Draw();
            PollModeClosed();
            if (mMode != COMMAND_MODE_NONE) {
                return;
            }
            OpenNextMode(0);
            SetCommandListBakuganHidden(mCommandList, 1);
            new BattleCommandPrompt(2, mCommandList, 0, 0, 0);
            mContext->mRequest = 0;
            mState++;
            return;
        case 38: {
            u16 v = mContext->mRequest;
            if (v == 0) {
                return;
            }
            if (v == 10) {
                CloseMode(1);
                mState++;
                return;
            }
            CloseMode(1);
            SpriteManager::Get().StartSubFade(FADE_MODE_DOWN);
            mPickIndex = GetLowByte(v);
            data_020d433c.mBattlePlayers->mPlayers[mPlayer]->mRecord = mPickIndex;
            GXS_SetVisiblePlane(0x11);
            mState = 0x34;
            return;
        }
        case 39:
            Draw();
            PollModeClosed();
            if (mMode != COMMAND_MODE_NONE) {
                return;
            }
            mNextMode = COMMAND_MODE_MAP;
            OpenNextMode(0);
            CreateCursorModel(mCommandMap);
            SetupMapCamera(mCommandMap, 2);
            mState++;
            return;
        case 40:
            Draw();
            if (SpriteManager::Get().sFade[0] > FADE_MODE_HOLD) {
                return;
            }
            SetCommandMessage();
            mCommandWindow->Reset(-1);
            mState = 10;
            return;
        case 41:
            Draw();
            PollModeClosed();
            if (mMode != COMMAND_MODE_NONE) {
                return;
            }
            OpenNextMode(1);
            SetCommandListBakuganHidden(mCommandList, 1);
            mState++;
            mCockpitWindow->Reset(-1);
            mCockpitWindow->mEnabled = 1;
            RefreshCommandPageOrList();
            mMessageWindow->StartMessage(0x21, 0, 1);
            return;
        case 42: {
            if (mCockpitWindow->mState == 1) {
                UpdateCursorScrollAndCamera(0);
            }
            WindowItem *item       = mCockpitWindow->mCursor->mItems;
            u8 idx;
            BattlePlayerList *list = data_020d433c.mBattlePlayers;
            s8 v                   = list->mUnitHandles[mCursorX][mCursorY];
            u8 flag                = 0;
            if (v > 0) {
                idx = v - 1;
                if (list->LocalPlayer() == list->mUnits[idx].mPlayer) {
                    item->mFlags = 5;
                    if (mReleasedOnCursorCell != 0) {
                        flag = 1;
                        SoundManager::Get().StartSeqArc(0);
                    }
                } else {
                    item->mFlags = 0x804;
                    if (mReleasedOnCursorCell != 0) {
                        SoundManager::Get().StartSeqArc(2);
                    }
                }
            } else {
                item->mFlags = 0x804;
            }
            u8 sel   = mCockpitWindow->Update(1);
            mUnk_13a = mCockpitWindow->mUnk_0e0;
            if (sel == 1 || flag != 0) {
                mBlendBrightness = 0;
                mAddMarkerFrames = 0;
                SpriteManager::Get().StartFade(FADE_MODE_DOWN);
                mCockpitWindow->mEnabled = 0;
                mState          = 0x2d;
                data_0220f540.mUnk_00 = 0x12;
                data_0220f540.mCursorX = mCursorX;
                data_0220f540.mCursorY = mCursorY;
                mDragScrolling              = 0;
            } else if (sel == 2) {
                mState++;
                CloseMode(1);
                mDragScrolling = 0;
            }
            mMessageWindow->Update();
            u8 b                = mCursorY;
            u8 a                = mCursorX;
            BattlePlayerList *l = data_020d433c.mBattlePlayers;
            l->mCursorX         = a;
            l->mCursorY         = b;
            Draw();
            u8 y = 0;
            mUnk_10->Update();
            do {
                u8 x = 0;
                do {
                    s8 cell = data_020d433c.mBattlePlayers->mUnitHandles[y][x];
                    if (cell > 0) {
                        BattlePlayerList *l2 = data_020d433c.mBattlePlayers;
                        if (l2->LocalPlayer() == l2->mUnits[(u8) (cell - 1)].mPlayer) {
                            if (mFlipped == 0) {
                                mUnk_10->Draw(1, (s16) (mOffsetX + (y * 16 + 14 - mScrollX)), (s16) (x * 24 - 22 - mScrollY), 0);
                            } else {
                                mUnk_10->Draw(1, (s16) (mOffsetX + ((u8) (13 - y) * 16 + 14 - mScrollX)), (s16) ((u8) (9 - x) * 24 - 22 - mScrollY), 0);
                            }
                        }
                    }
                    x++;
                } while (x < 10);
                y++;
            } while (y < 14);
            mCockpitWindow->Draw();
            QueueFrameCallback(ApplyScrollOffset, this);
            return;
        }
        case 43:
            mMessageWindow->Update();
            Draw();
            PollModeClosed();
            if (mMode != COMMAND_MODE_NONE) {
                return;
            }
            mCockpitWindow->Update(1);
            if (mCockpitWindow->mState != 4) {
                return;
            }
            mNextMode = COMMAND_MODE_MAP;
            OpenNextMode(0);
            CreateCursorModel(mCommandMap);
            mCockpitWindow->Reset(-1);
            mCockpitWindow->mEnabled = 0;
            SetupMapCamera(mCommandMap, 2);
            mState++;
            return;
        case 44:
            mMessageWindow->Update();
            Draw();
            if (SpriteManager::Get().sFade[0] > FADE_MODE_HOLD) {
                return;
            }
            SetCommandMessage();
            mCommandWindow->Reset(-1);
            mState = 10;
            SetCommandMessage();
            return;
        case 45: {
            u8 b                   = mCursorY;
            u8 a                   = mCursorX;
            BattlePlayerList *list = data_020d433c.mBattlePlayers;
            list->mCursorX         = a;
            list->mCursorY         = b;
            Draw();
            mCockpitWindow->Draw();
            if (SpriteManager::Get().IsFading()) {
                return;
            }
            CloseMode(1);
            mAddMarkerFrames = 0;
            mState++;
            return;
        }
        case 46:
            Draw();
            PollModeClosed();
            if (mMode != COMMAND_MODE_NONE) {
                return;
            }
            if (!SetMessageTypeAndCheckAll(data_020d433c.mBattlePlayers, 0xf)) {
                return;
            }
            mNextMode = COMMAND_MODE_MAP;
            OpenNextMode(0);
            CreateCursorModel(mCommandMap);
            SetupMapCamera(mCommandMap, 2);
            mCommandMap->mUseStatusPanels = 0;
            SpriteManager::Get().StartSubFade(FADE_MODE_TO_ZERO);
            data_020d433c.mGimmicks->mState = 0;
            new BattleCardEffect(0, 0xb, 0xe, 1);
            mContext->mRequest = 0;
            mState++;
            return;
        case 47: {
            if (mContext->mRequest == 0) {
                return;
            }
            SpriteManager::Get().StartFade(FADE_MODE_DOWN);
            CloseMode(0);
            BattlePlayerList *list = data_020d433c.mBattlePlayers;
            list->mUnits[(u8) (list->mUnitHandles[mCursorX][mCursorY] - 1)].mUnk_12 = 1;
            mState++;
            return;
        }
        case 48:
            if (!SpriteManager::Get().IsFading()) {
                mState = 0x36;
            }
            return;
        case 49:
            new BattlePopup(1);
            mContext->mRequest = 0;
            mState++;
            return;
        case 50:
            switch (mContext->mRequest) {
                case 1:
                    mState = 10;
                    return;
                case 2:
                    if (data_020d433c.mWireless != 0) {
                        WirelessManager::Get();
                        data_020c4030.mUnk_22a = WM_CALL_FAILED;
                        mState++;
                        return;
                    }
                    SpriteManager::Get().SetFade(FADE_MODE_HOLD, -16);
                    mNextScene = 7;
                    mState = 0x39;
                    return;
            }
            return;
        case 52:
            PollModeClosed();
            if (mMode == COMMAND_MODE_NONE) {
                SpriteManager::Get().SetFade(FADE_MODE_HOLD, -16);
                mState = 0x39;
                mNextScene = COMMAND_RESULT_SHOOT;
            }
            Draw();
            return;
        case 53:
            PollModeClosed();
            if (mMode == COMMAND_MODE_NONE) {
                mNextScene = COMMAND_RESULT_SHOOT_FROM_MSG;
                mState = 0x39;
            }
            Draw();
            return;
        case 54:
            PollModeClosed();
            if (mMode == COMMAND_MODE_NONE) {
                SpriteManager::Get().SetFade(FADE_MODE_HOLD, -16);
                mNextScene = COMMAND_RESULT_NEXT_TURN;
                mState = 0x39;
            }
            Draw();
            return;
        case 55:
            SpriteManager::Get().SetFade(FADE_MODE_HOLD, -16);
            mNextScene = COMMAND_RESULT_BATTLE_RESULT;
            mState = 0x39;
            return;
        case 56:
            SpriteManager::Get().SetFade(FADE_MODE_HOLD, -16);
            mNextScene = COMMAND_RESULT_NO_MARKERS;
            mState = 0x39;
            return;
        case 57:
            RequestScene(mNextScene);
            mFinished = 1;
            return;
    }
}

BOOL BattleCommand::UpdateCommandWindowAndCursor(BOOL input) {
    BOOL result = FALSE;
    u8 sel      = 0;
    BOOL poll   = mCommandWindow->mState != 1;
    if (input | poll) {
        sel = mCommandWindow->Update(1);
    }
    mUnk_13a = mCommandWindow->mUnk_0e0;
    if (data_020d433c.mBattlePlayers->mAddMarkerEnabled == 0) {
        mCommandWindow->mButtons[2].mEnabled = 0;
    }
    if (data_020d433c.mUnk_9a_3) {
        mCommandWindow->mButtons[2].mEnabled = 0;
        mCommandWindow->mButtons[5].mEnabled = 0;
    }
    if (input) {
        if (sel) {
            mSelectedId = mCommandWindow->GetSelectedId();
            result   = TRUE;
            if (mSelectedId == 3) {
                switch (mMode) {
                    case COMMAND_MODE_MAP:
                        mNextMode = COMMAND_MODE_PAGE;
                        break;
                    case COMMAND_MODE_PAGE:
                        if (data_020d433c.mBattlePlayers->mUnitHandles[mCursorX][mCursorY] > 0) {
                            mNextMode = COMMAND_MODE_LIST;
                        } else {
                            mNextMode = COMMAND_MODE_MAP;
                        }
                        break;
                    case COMMAND_MODE_LIST:
                        mNextMode = COMMAND_MODE_MAP;
                        break;
                }
                CloseMode(1);
            }
        }
        UpdateCursorScrollAndCamera(0);
    }
    mMessageWindow->Update();
    Draw();
    u8 b                   = mCursorY;
    u8 a                   = mCursorX;
    BattlePlayerList *list = data_020d433c.mBattlePlayers;
    list->mCursorX         = a;
    list->mCursorY         = b;
    if (mMode == COMMAND_MODE_MAP) {
        func_ov007_022470e4(mCommandMap, mCursorX, mCursorY);
    }
    QueueFrameCallback(ApplyScrollOffset, this);
    return result;
}

BOOL BattleCommand::SwitchMode() {
    BOOL result = FALSE;
    PollModeClosed();
    if (mMode == COMMAND_MODE_NONE) {
        OpenNextMode(1);
        switch (mMode) {
            case COMMAND_MODE_MAP:
                CreateCursorModel(mCommandMap);
                SetupMapCamera(mCommandMap, 2);
                break;
            case COMMAND_MODE_PAGE: {
                BattlePlayerList *list = data_020d433c.mBattlePlayers;
                s8 cell                = list->mMarkerHandles[mCursorX][mCursorY];
                if (cell > 0) {
                    u8 owner  = list->mLocalPlayer;
                    u8 who    = list->mMarkers[(u8) (cell - 1)].mPlayer;
                    u16 value = 0;
                    if (owner == who || who == list->mPlayers[owner]->mPartner) {
                        value = *GetMarkerSlotValue(list, (u8) (cell - 1));
                    }
                    SetCommandPageCard(mCommandPage, value, (s8) who);
                } else {
                    SetCommandPageCardHidden(mCommandPage, 1);
                }
                break;
            }
            case COMMAND_MODE_LIST: {
                BattleCellInfo *a = GetUnitRecordAt(data_020d433c.mBattlePlayers, mCursorX, mCursorY);
                u16 sum           = a->mUnk_04 + GetUnitRecordAt(data_020d433c.mBattlePlayers, mCursorX, mCursorY)->mUnk_06;
                u8 c              = GetUnitRecordAt(data_020d433c.mBattlePlayers, mCursorX, mCursorY)->mAttribute;
                u8 d              = GetUnitRecordAt(data_020d433c.mBattlePlayers, mCursorX, mCursorY)->mBakuganId;
                BattlePlayerList *list = data_020d433c.mBattlePlayers;
                u8 slot                = list->mUnitHandles[mCursorX][mCursorY] - 1;
                BattleUnit *unit       = &list->Units()[slot];
                u8 playerIndex         = unit->mPlayer;
                u8 recordIndex         = unit->mRecord;
                SetCommandListBakugan(mCommandList, d, c, (s8) playerIndex);
                SetCommandListValues(mCommandList, c, sum, (BattleCellPair *) &data_020d433c.mBattlePlayers->mPlayers[playerIndex]->mRecords[recordIndex]);
                break;
            }
        }
        result = TRUE;
    }
    return result;
}

BOOL BattleCommand::UpdatePrompt() {
    switch (mPromptStep) {
        case 0:
            mPromptStep++;
        case 1:
            CloseMode(1);
            mPromptWindow->Reset(-1);
            mPromptWindow->mCellAnims[0]->SetAnimation(8);
            mPromptWindow->mCellAnims[1]->SetAnimation(0);
            mPromptWindow->mCellAnims[2]->SetAnimation(12);
            mPromptStep++;
            break;
        case 2:
            PollModeClosed();
            if (mMode == COMMAND_MODE_NONE) {
                mNextMode = COMMAND_MODE_PAGE;
                OpenNextMode(0);
                mCommandPrompt                = new BattleCommandPrompt(5, mCommandPage, 0, 0, (u8) (mSelectedId - 6));
                mContext->mRequest = 0;
                mPromptStep               = 7;
            }
            break;
        case 3:
            CloseMode(1);
            mPromptStep++;
            break;
        case 4:
            PollModeClosed();
            if (mMode == COMMAND_MODE_NONE) {
                mNextMode = COMMAND_MODE_LIST;
                OpenNextMode(0);
                mCommandPrompt                = new BattleCommandPrompt(6, mCommandList, 0, 0, (u8) (mSelectedId - 6));
                mContext->mRequest = 0;
                mPromptStep               = 7;
            }
            break;
        case 5:
            CloseMode(1);
            mPromptStep++;
            break;
        case 6:
            PollModeClosed();
            if (mMode == COMMAND_MODE_NONE) {
                mNextMode = COMMAND_MODE_PAGE;
                OpenNextMode(0);
                mCommandPrompt                = new BattleCommandPrompt(7, mCommandPage, 0, 0, (u8) (mSelectedId - 6));
                mContext->mRequest = 0;
                mPromptStep               = 7;
            }
            break;
        case 7:
            if (mContext->mRequest != 0) {
                mCommandPrompt = 0;
                CloseMode(1);
                mPromptStep++;
            } else {
                if (mPromptWindow->Update(1)) {
                    u8 id = mPromptWindow->GetSelectedId();
                    switch (id) {
                        case 0:
                            CloseCommandPrompt(mCommandPrompt, 0);
                            mCommandPrompt  = 0;
                            mPromptStep = 1;
                            break;
                        case 1:
                            CloseCommandPrompt(mCommandPrompt, 0);
                            mCommandPrompt  = 0;
                            mPromptStep = 3;
                            break;
                        case 2:
                            CloseCommandPrompt(mCommandPrompt, 0);
                            mCommandPrompt  = 0;
                            mPromptStep = 5;
                            break;
                    }
                }
            }
            mPromptWindow->Draw();
            break;
        case 8:
            PollModeClosed();
            if (mMode == COMMAND_MODE_NONE) {
                mPromptWindow->mState = 4;
                mPromptWindow->mSlideInRequested = 0;
                mNextMode         = COMMAND_MODE_MAP;
                OpenNextMode(0);
                CreateCursorModel(mCommandMap);
                SetupMapCamera(mCommandMap, 2);
                SetCommandMessage();
                mCommandWindow->Reset(-1);
                LoadTurnGraphics();
                mPromptStep = 0;
                return TRUE;
            }
            break;
        case 9:
            mPromptWindow->mState = 4;
            mPromptWindow->mSlideInRequested = 0;
            mPromptStep          = 0;
            break;
    }
    return FALSE;
}

void InitCommandMainScreen(BattleCommand *command, u32 mode) {
    SpriteManager::Get().SetupMode(mode, 7);
    G2_SetBG0Offset(0, 0);
    G2_BlendNone();
    GX_SetDispSelect(1);
    GX_SetVisiblePlane(0x11);
    GX_SetGraphicsMode(1, 0, 1);
    G2_SetBG0Priority(0);
    G2_SetBG1Priority(1);
    GX_SetVisibleWnd(0);
}

void InitCommandSubScreen(int x, int y) {
    G2S_SetBG0Control(3, 1, 0, 1, 1);
    G2S_SetBG3Control(1, 0, 4, 5, 0);
    G2S_SetBG0Offset(x + sSubBg0Offset[0], y + sSubBg0Offset[1]);
    G2S_SetBG2Offset(0, 0);
    G2S_SetBG3Offset(0, 0);
    G2S_BlendNone();
    GXS_SetVisiblePlane(0x11);
    GXS_SetVisibleWnd(0);
    GXS_SetGraphicsMode(3);
    G2S_SetBG0Priority(3);
}

void BattleCommand::func_ov007_0223a734() {
    ChangeDir("/Game/Battle/Obj");
    LoadAndQueueUpload(0, UPLOAD_KIND_OBJ, 0, "battle_u.nce_ncg1D.bin", 0, 0, 0);
    LoadAndQueueUpload(0, UPLOAD_KIND_OBJ_PLTT, 0, "battle_u_ncl.bin", 0, 0, 0);
    mCommandMap = new BattleCommandMap(data_020d433c.mFieldId);
    mCommandMap->Init();
    CreateCursorModel(mCommandMap);
    SetupMapCamera(mCommandMap, 2);
    ChangeDir("/Game/Battle/Obj");
    u8 i;
    SpriteScreen &screen = SpriteManager::Get().Screen(1);
    screen.SetObjVramMode(3);
    mCommandWindow->SetCells(10, sCommandCells);
    for (i = data_020d433c.mPlayerCount; i < 4; i++) {
        mCommandWindow->mButtons[i + 6].mEnabled = 0;
    }
    mCockpitWindow->SetCells(2, sCockpitCells);
    mPromptWindow->SetCells(3, sPromptCells);
    for (i = 0; i < 12; i++) {
        mUnitCellAnims[i] = new CellAnimation("battle_b_obj_00_nce.bin", 0x44, 1, 1);
        mUnitCellAnims[i]->mCharBase = screen.GetCharBase(0xa000);
        mUnitCellAnims[i]->mPaletteBase   = 0;
    }
    mCursorAnim = new CellAnimation("battle_b_obj_00_nce.bin", 0x15, 1, 1);
    mCursorAnim->mCharBase = screen.GetCharBase(0xa000);
    mCursorAnim->mPaletteBase   = 0;
    mUnk_10 = new CellAnimation("battle_b_obj_00_nce.bin", 0x16, 1, 1);
    mUnk_10->mCharBase = screen.GetCharBase(0xa000);
    mUnk_10->mPaletteBase   = 0;
    mUnk_14 = new CellAnimation("battle_b_obj_00_nce.bin", 0x1b, 1, 1);
    mUnk_14->mCharBase = screen.GetCharBase(0xa000);
    mUnk_14->mPaletteBase   = 0;
    if (data_020d433c.mTutorialId) {
        mTutorialCellAnim = new CellAnimation("battle_b_obj_00_nce.bin", 0x45, 1, 1);
        mTutorialCellAnim->mCharBase = screen.GetCharBase(0xa000);
        mTutorialCellAnim->mPaletteBase   = 0;
    }
    u8 cur = data_020d433c.mBattlePlayers->mLocalPlayer;
    mLocalPlayerCellAnim = new CellAnimation("battle_b_obj_00_nce.bin", (u8) (cur + 0x38), 1, 1);
    mLocalPlayerCellAnim->mCharBase = screen.GetCharBase(0xa000);
    mLocalPlayerCellAnim->mPaletteBase   = 0;
    if (data_020d433c.mHasPartners) {
        u8 partner = data_020d433c.mBattlePlayers->mPlayers[cur]->mPartner;
        mPartnerCellAnim = new CellAnimation("battle_b_obj_00_nce.bin", (u8) (partner + 0x40), 1, 1);
        mPartnerCellAnim->mCharBase = screen.GetCharBase(0xa000);
        mPartnerCellAnim->mPaletteBase   = 0;
    }
    mTurnCellAnim = new CellAnimation("battle_b_obj_turn_nce.bin", 0, 1, 1);
    mTurnCellAnim->mCharBase = screen.GetCharBase(0x17000);
    mTurnCellAnim->mPaletteBase   = 9;
    mTurnDigitsCellAnim = new CellAnimation("battle_b_obj_turn_nce.bin", 1, 1, 1);
    mTurnDigitsCellAnim->mCharBase = screen.GetCharBase(0x17000);
    mTurnDigitsCellAnim->mPaletteBase   = 9;
    u8 p = 0;
    do {
        BattlePlayerList *list = data_020d433c.mBattlePlayers;
        if (mFlipped == 0) {
            u8 frame   = list->mPlayers[p]->mCorner + (p * 4 + 0x28);
            mPlayerCellAnims[p] = new CellAnimation("battle_b_obj_00_nce.bin", frame, 1, 1);
        } else {
            u8 frame   = (list->mPlayers[p]->mCorner + 2) % 4 + (p * 4 + 0x28);
            mPlayerCellAnims[p] = new CellAnimation("battle_b_obj_00_nce.bin", frame, 1, 1);
        }
        mPlayerCellAnims[p]->mCharBase = screen.GetCharBase(0xa000);
        mPlayerCellAnims[p]->mPaletteBase   = 0;
        p++;
    } while (p < data_020d433c.mPlayerCount);
    ChangeDir("/");
    mMessageWindow = new MessageWindow("mes_Command", 1, 0x1f000);
    mMessageWindow->func_02065bd4(0x14, 4, 0);
    mMessageWindow->SetWidth(0xd8);
    SetCommandMessage();
}

// Loads the battle background; `variant` is the screen variant (BattleObj00 passes its mUnk_3c), overridden for some
// field types.
void LoadCommandGraphics(u32 variant) {
    char path[0x100];
    MI_CpuClearFast((void *) 0x6609000, 0x1000);
    reg_GX_POWCNT |= 0x8000;
    ChangeDir("/Game/Battle/Bg");
    if (data_020d433c.mLastBattle != 0) {
        sprintf(path, "battle_b_bg_last_ncl.bin", data_020d433c.mFieldId);
        LoadAndQueueUpload(1, UPLOAD_KIND_BG_EXT_PLTT, 0x4000, path, 0, 0, 0);
        sprintf(path, "battle_b_bg_last_ncg.bin", data_020d433c.mFieldId);
        LoadAndQueueUpload(1, UPLOAD_KIND_BG0_CHAR, 0, path, 0, 0, 0);
        sprintf(path, "battle_b_bg_last_a_nsc.bin", data_020d433c.mFieldId);
        LoadAndQueueUpload(1, UPLOAD_KIND_BG0_SCR, 0, path, 0, 0, 0);
        sprintf(path, "battle_b_bg_last_b_nsc.bin", data_020d433c.mFieldId);
        LoadAndQueueUpload(1, UPLOAD_KIND_BG0_SCR, 0x800, path, 0, 0, 0);
        sprintf(path, "battle_b_bg_last_c_nsc.bin", data_020d433c.mFieldId);
        LoadAndQueueUpload(1, UPLOAD_KIND_BG0_SCR, 0x1000, path, 0, 0, 0);
        sprintf(path, "battle_b_bg_last_d_nsc.bin", data_020d433c.mFieldId);
        LoadAndQueueUpload(1, UPLOAD_KIND_BG0_SCR, 0x1800, path, 0, 0, 0);
    } else {
        switch (data_020d433c.mFieldId) {
            case 1:
            case 2:
            case 3:
            case 7:
                variant = 0;
                break;
        }
        sprintf(path, "battle_b_bg_%02d_ncl.bin", data_020d433c.mFieldId);
        LoadAndQueueUpload(1, UPLOAD_KIND_BG_EXT_PLTT, 0x4000, path, 0, 0, 0);
        sprintf(path, "battle_b_bg_%02d_ncg.bin", data_020d433c.mFieldId);
        LoadAndQueueUpload(1, UPLOAD_KIND_BG0_CHAR, 0, path, 0, 0, 0);
        sprintf(path, "battle_b_bg_%02d_a_%02d_nsc.bin", data_020d433c.mFieldId, variant);
        LoadAndQueueUpload(1, UPLOAD_KIND_BG0_SCR, 0, path, 0, 0, 0);
        sprintf(path, "battle_b_bg_%02d_b_%02d_nsc.bin", data_020d433c.mFieldId, variant);
        LoadAndQueueUpload(1, UPLOAD_KIND_BG0_SCR, 0x800, path, 0, 0, 0);
        sprintf(path, "battle_b_bg_%02d_c_%02d_nsc.bin", data_020d433c.mFieldId, variant);
        LoadAndQueueUpload(1, UPLOAD_KIND_BG0_SCR, 0x1000, path, 0, 0, 0);
        sprintf(path, "battle_b_bg_%02d_d_%02d_nsc.bin", data_020d433c.mFieldId, variant);
        LoadAndQueueUpload(1, UPLOAD_KIND_BG0_SCR, 0x1800, path, 0, 0, 0);
    }
    SpriteManager::Get().Screen(1).SetObjVramMode(3);
    ChangeDir("/Game/Battle/Obj");
    LoadAndQueueUpload(1, UPLOAD_KIND_OBJ, 0xa000, "battle_b_obj_00.nce_ncg1D.bin", 0, 0, 0);
    LoadAndQueueUpload(1, UPLOAD_KIND_OBJ_PLTT, 0, "battle_b_obj_00_ncl.bin", 0, 0, 0);
    ChangeDir("/Game/2D/union_b");
    LoadAndQueueUpload(1, UPLOAD_KIND_OBJ, 0x1c800, "union_b_obj_cockpit.nce_ncg1D.bin", 0, 0, 0);
    LoadAndQueueUpload(1, UPLOAD_KIND_OBJ_PLTT, 0x1c0, "union_b_obj_cockpit_ncl.bin", 0, 0, 0);
    ChangeDir("/Game/Card/Graphic/Small");
    LoadAndQueueUpload(1, UPLOAD_KIND_OBJ, 0x9000, "card_s_000_ncg.bin", 0, 0, 0);
    LoadAndQueueUpload(1, UPLOAD_KIND_OBJ_EXT_PLTT, 0, "card_s_000_ncl.bin", 0, 0, 0);
    ChangeDir("/Game/2D/union_b");
    LoadAndQueueUpload(1, UPLOAD_KIND_OBJ, 0x18000, "union_b_obj_page.nce_ncg1D.bin", 0, 0, 0);
    if (data_020d433c.mAntenna) {
        data_020d433c.mAntenna->SetupLayout(8);
    }
    ChangeDir("/");
}

class Camera;

void CreateCursorModel(BattleCommandMap *map);

void BattleCommand::LoadTurnGraphics() {
    ChangeDir("/Game/Battle/Obj");
    LoadAndQueueUpload(1, UPLOAD_KIND_OBJ, 0x17000, "battle_b_obj_turn.nce_ncg1D.bin", 0, 0, 0);
    LoadAndQueueUpload(1, UPLOAD_KIND_OBJ_PLTT, 0x120, "battle_b_obj_turn_ncl.bin", 0, 0, 0);
    ChangeDir("/");
}

void BattleCommand::Draw() {
    SpriteScreen &screen = SpriteManager::Get().Screen(1);
    screen.SetObjVramMode(3);
    u8 col = mCursorX;
    u8 row = mCursorY;
    if (mFlipped) {
        col = 13 - col;
        row = 9 - row;
    }
    mCursorAnim->Update();
    mCursorAnim->Draw(1, (s16) (col * 16 + 14 - mScrollX + mOffsetX), (s16) (row * 24 - 22 - mScrollY), 1);
    u8 i = 0;
    u8 k = 0;
    do {
        u8 j = 0;
        do {
            u8 x = i;
            u8 y = j;
            if (mFlipped) {
                y = 9 - j;
                x = 13 - i;
            }
            BOOL show;
            s8 cell = data_020d433c.mBattlePlayers->mUnitHandles[i][j];
            if (cell > 0) {
                GetUnitRecordAt(data_020d433c.mBattlePlayers, i, j);
                mUnitCellAnims[k]->DrawFrame(1, data_020d433c.mBattlePlayers->Units()[(u8) (cell - 1)].mPlayer, (s16) (x * 16 + 14 - mScrollX + mOffsetX), (s16) (y * 24 - 22 - mScrollY), 1);
                k++;
            }
            s8 mark = data_020d433c.mBattlePlayers->mMarkerHandles[i][j];
            if (mark > 0) {
                u8 who    = data_020d433c.mBattlePlayers->mMarkers[(u8) (mark - 1)].mPlayer;
                show      = TRUE;
                if (data_020d433c.mBattlePlayers->mLocalPlayer == who) {
                    if (!(GameSystem::Get().mFrame & 0x20)) {
                        show = FALSE;
                    }
                }
                if (show) {
                    screen.AddOam((void *) sMarkerOam, 1, (s16) (x * 16 + 14 - mScrollX + mOffsetX), (s16) (y * 24 - 22 - mScrollY), 1, 0x120, 0, 0);
                }
            }
            j++;
        } while (j < 10);
        i++;
    } while (i < 14);
    u8 p = 0;
    do {
        u8 side = data_020d433c.mBattlePlayers->mPlayers[p]->mCorner;
        if (mFlipped) {
            side = (side + 2) % 4;
        }
        if (p == data_020d433c.mBattlePlayers->mLocalPlayer) {
            mLocalPlayerCellAnim->Update();
            mLocalPlayerCellAnim->Draw(1, (s16) (mOffsetX + sCornerScreenPoints[side].mX), sCornerScreenPoints[side].mY, 0);
            if (data_020d433c.mHasPartners) {
                u8 partner = data_020d433c.mBattlePlayers->mPlayers[data_020d433c.mBattlePlayers->mPlayers[p]->mPartner]->mCorner;
                if (mFlipped) {
                    partner = (partner + 2) % 4;
                }
                mPartnerCellAnim->Update();
                mPartnerCellAnim->Draw(1, (s16) (mOffsetX + sCornerScreenPoints[partner].mX), sCornerScreenPoints[partner].mY, 0);
            }
        }
        mPlayerCellAnims[p]->Update();
        mPlayerCellAnims[p]->Draw(1, (s16) (mOffsetX + sCornerScreenPoints[side].mX), sCornerScreenPoints[side].mY, 0);
        p++;
    } while (p < data_020d433c.mPlayerCount);
    if (data_020d433c.mTutorialId && mCommandWindow->mState == 1) {
        mTutorialCellAnim->Update();
        mTutorialCellAnim->Draw(1, sCommandItemX[gSaveData[0].mCommandItemXIndex], 0x22, 0);
    }
    mCommandWindow->Draw();
    DrawTurnCount();
}

void CreateCursorModel(BattleCommandMap *map);

void SetupMapCamera(BattleCommandMap *map, u32 a);

void BattleCommand::func_ov007_0223b874() {
    if (mUnk_12e != 0) {
        GXS_SetVisibleWnd(4);
        G2S_SetWndOBJInsidePlane(0x31, FALSE);
        G2S_SetWndOutsidePlane(0x11, FALSE);
        if (mFlipped == 0) {
            mUnk_14->Draw(1, (s16) (mOffsetX + ((mCursorX * 16 + 14) - mScrollX)), (s16) ((mCursorY * 24 - 22) - mScrollY), 0);
        } else {
            u8 row = 13 - mCursorX;
            u8 col = 9 - mCursorY;
            mUnk_14->Draw(1, (s16) (mOffsetX + ((row * 16 + 14) - mScrollX)), (s16) ((col * 24 - 22) - mScrollY), 0);
        }
        G2S_SetBlendBrightness(1, mBlendBrightness);
    } else {
        u8 row = 0;
        if (mFlipped == 0) {
            mUnk_10->Update();
            do {
                u8 col = 0;
                do {
                    if (IsMarkerPlaceable(data_020d433c.mBattlePlayers, row, col) && mGridMask[row][col] != 0) {
                        mUnk_10->Draw(1, (s16) (mOffsetX + ((row * 16 + 14) - mScrollX)), (s16) ((col * 24 - 22) - mScrollY), 1);
                    }
                    col++;
                } while (col < 10);
                row++;
            } while (row < 14);
        } else {
            mUnk_10->Update();
            do {
                u8 col = 0;
                do {
                    if (IsMarkerPlaceable(data_020d433c.mBattlePlayers, row, col) && mGridMask[row][col] != 0) {
                        u8 r = 13 - row;
                        u8 c = 9 - col;
                        mUnk_10->Draw(1, (s16) (mOffsetX + ((r * 16 + 14) - mScrollX)), (s16) ((c * 24 - 22) - mScrollY), 1);
                    }
                    col++;
                } while (col < 10);
                row++;
            } while (row < 14);
        }
    }
}

void BattleCommand::UpdateCursorScrollAndCamera(u32 flag) {
    u32 moved = 0;
    mReleasedOnCursorCell  = 0;
    if (mUnk_13a == 0) {
        if (mFlipped != 0) {
            if (InputManager::Get().IsTrigger(KEY_RIGHT) && mCursorX != 0) {
                u32 r = mCursorX - 1;
                if (mGridMask[r][mCursorY] != 0) {
                    mCursorX = r;
                    moved    = 1;
                    if (mCursorX <= 2) {
                        mScrollX += 0x10;
                    }
                }
            }
            if (InputManager::Get().IsTrigger(KEY_LEFT) && mCursorX < 13) {
                u32 r = mCursorX + 1;
                if (mGridMask[r][mCursorY] != 0) {
                    mCursorX = r;
                    moved    = 1;
                    if (mCursorX >= 11) {
                        mScrollX -= 0x10;
                    }
                }
            }
            if (InputManager::Get().IsTrigger(KEY_UP) && mCursorY < 9) {
                u32 c = mCursorY;
                c++;
                if (mGridMask[mCursorX][c] != 0) {
                    mCursorY = c;
                    moved    = 1;
                    if (mCursorY >= 6) {
                        mScrollY -= 0x18;
                    }
                }
            }
            if (InputManager::Get().IsTrigger(KEY_DOWN) && mCursorY != 0) {
                u32 c = mCursorY;
                c--;
                if (mGridMask[mCursorX][c] != 0) {
                    mCursorY = c;
                    moved    = 1;
                    if (mCursorY <= 3) {
                        mScrollY += 0x18;
                    }
                }
            }
        } else {
            if (InputManager::Get().IsTrigger(KEY_RIGHT) && mCursorX < 13) {
                u32 r = mCursorX + 1;
                if (mGridMask[r][mCursorY] != 0) {
                    mCursorX = r;
                    moved    = 1;
                    if (mCursorX >= 11) {
                        mScrollX += 0x10;
                    }
                }
            }
            if (InputManager::Get().IsTrigger(KEY_LEFT) && mCursorX != 0) {
                u32 r = mCursorX - 1;
                if (mGridMask[r][mCursorY] != 0) {
                    mCursorX = r;
                    moved    = 1;
                    if (mCursorX <= 2) {
                        mScrollX -= 0x10;
                    }
                }
            }
            if (InputManager::Get().IsTrigger(KEY_UP) && mCursorY != 0) {
                u32 c = mCursorY;
                c--;
                if (mGridMask[mCursorX][c] != 0) {
                    mCursorY = c;
                    moved    = 1;
                    if (mCursorY <= 3) {
                        mScrollY -= 0x18;
                    }
                }
            }
            if (InputManager::Get().IsTrigger(KEY_DOWN) && mCursorY < 9) {
                u32 c = mCursorY;
                c++;
                if (mGridMask[mCursorX][c] != 0) {
                    mCursorY = c;
                    moved    = 1;
                    if (mCursorY >= 6) {
                        mScrollY += 0x18;
                    }
                }
            }
        }
    }
    s32 tx = TouchManager::Get().mX;
    s32 ty = TouchManager::Get().mY;
    if (mUnk_13a == 0) {
        u8 field = data_020d433c.mFieldId;
        if (data_020d433c.mLastBattle != 0) {
            field = 0;
        }
        s32 off = 0;
        switch (field) {
            case 0:
            case 1:
            case 2:
            case 3:
            case 5:
                break;
            case 6:
                if (gSaveData[0].mCommandItemXIndex == 0) {
                    off = 0xa;
                }
                break;
            case 4:
                if (gSaveData[0].mCommandItemXIndex == 0) {
                    off = 0x20;
                }
                break;
        }
        if (mDragScrolling != 0) {
            mScrollX = mGridTouchX - tx;
            mScrollY = mGridTouchY - ty;
        }
        if (mScrollX > sScrollXBounds[field].mMax) {
            mScrollX = sScrollXBounds[field].mMax;
        } else if (mScrollX < sScrollXBounds[field].mMin - off) {
            mScrollX = sScrollXBounds[field].mMin - off;
        }
        if (mScrollY > sScrollYBounds[field].mMax) {
            mScrollY = sScrollYBounds[field].mMax;
        } else if (mScrollY < sScrollYBounds[field].mMin) {
            mScrollY = sScrollYBounds[field].mMin;
        }
        if (TouchManager::Get().mPressed != 0) {
            mDragScrolling = 1;
            mGridTouchX = tx + mScrollX;
            mGridTouchY = ty + mScrollY;
        }
        if (TouchManager::Get().mReleased != 0) {
            mGridTouchX = tx + mScrollX;
            mGridTouchY = ty + mScrollY;
            s16 col  = (mGridTouchX - 0xe - mOffsetX) / 16;
            s16 row  = (mGridTouchY + 0x16) / 24;
            if (col >= 0 && col <= 13 && row >= 0 && row <= 9 && mGridMask[col][row] != 0) {
                if (mFlipped == 0) {
                    if (mCursorX != col || mCursorY != row) {
                        mCursorX = col;
                        mCursorY = row;
                        moved    = 1;
                    } else if (mCursorX == col && mCursorY == row) {
                        mReleasedOnCursorCell = 1;
                    }
                } else {
                    if (mCursorX != 13 - col || mCursorY != 9 - row) {
                        mCursorX = 13 - col;
                        mCursorY = 9 - row;
                        moved    = 1;
                    } else if (mCursorX == 13 - col && mCursorY == 9 - row) {
                        mReleasedOnCursorCell = 1;
                    }
                }
            }
        }
        if (TouchManager::Get().mReleased != 0) {
            mDragScrolling = 0;
        }
    }
    if (mMode == COMMAND_MODE_MAP) {
        if (InputManager::Get().IsDown(0x200)) {
            if (InputManager::Get().IsPressed(0x100)) {
                SetupMapCamera(mCommandMap, 2);
                return;
            }
            RotateCameraYaw(mCommandMap, 0);
        }
        if (InputManager::Get().IsDown(0x100)) {
            if (InputManager::Get().IsPressed(0x200)) {
                SetupMapCamera(mCommandMap, 2);
                return;
            }
            RotateCameraYaw(mCommandMap, 1);
        }
    }
    if (moved == 1) {
        SoundManager::Get().StartSeqArc(5);
        if (flag == 0) {
            RefreshCommandPageOrList();
        }
    }
}

BOOL IsMarkerPlaceable(BattlePlayerList *list, u8 x, u8 y);

void BattleCommand::RefreshCommandPageOrList() {
    switch (mMode) {
        case COMMAND_MODE_MAP:
            return;
        case COMMAND_MODE_PAGE: {
            BattlePlayerList *list = data_020d433c.mBattlePlayers;
            s8 cell                = list->mMarkerHandles[mCursorX][mCursorY];
            u8 owner               = list->mLocalPlayer;
            if (cell > 0) {
                u8 who    = list->mMarkers[(u8) (cell - 1)].mPlayer;
                u16 value;
                if (owner == who || who == list->mPlayers[owner]->mPartner) {
                    value = *GetMarkerSlotValue(list, (u8) (cell - 1));
                } else {
                    value = 0;
                }
                SetCommandPageCard(mCommandPage, value, (s8) who);
            } else {
                SetCommandPageCardHidden(mCommandPage, 1);
            }
            break;
        }
        case COMMAND_MODE_LIST: {
            BattlePlayerList *list;
            u8 b;
            u8 a;
            s8 cell;
            a    = mCursorX;
            list = data_020d433c.mBattlePlayers;
            b    = mCursorY;
            cell = list->CellAt(a, b);
            if (cell > 0) {
                BattleCellInfo *info = GetUnitRecordAt(list, a, b);
                u16 sum              = info->mUnk_04 + GetUnitRecordAt(list, a, b)->mUnk_06;
                u8 c                 = GetUnitRecordAt(data_020d433c.mBattlePlayers, mCursorX, mCursorY)->mAttribute;
                u32 d                = GetUnitRecordAt(data_020d433c.mBattlePlayers, mCursorX, mCursorY)->mBakuganId;
                BattleCellInfo *e    = GetUnitRecordAt(data_020d433c.mBattlePlayers, mCursorX, mCursorY);
                BattleCellPair pair = *(BattleCellPair *) e;
                if (mCommandList) {
                    SetCommandListBakugan(mCommandList, d, c, (s8) data_020d433c.mBattlePlayers->Units()[(u8) (cell - 1)].mPlayer);
                    SetCommandListValues(mCommandList, c, sum, &pair);
                }
            } else {
                SetCommandListBakuganHidden(mCommandList, 1);
            }
            break;
        }
    }
}

void BattleCommand::CloseMode(BOOL flag) {
    switch (mMode) {
        case COMMAND_MODE_MAP:
            if (flag) {
                CloseCommandMap(mCommandMap);
            } else {
                CloseCommandMapNow(mCommandMap);
            }
            break;
        case COMMAND_MODE_PAGE:
            CloseCommandPage(mCommandPage, 1);
            mCommandPage = 0;
            break;
        case COMMAND_MODE_LIST:
            CloseCommandList(mCommandList);
            mCommandList = 0;
            break;
        default:
            return;
    }
    if (flag) {
        SpriteManager::Get().sFade[0] = FADE_MODE_DOWN;
    } else {
        SpriteManager::Get();
        SpriteManager::sFade[0]       = FADE_MODE_HOLD;
        SpriteManager::sBrightness[0] = -16;
    }
    mMode                = COMMAND_MODE_CLOSING;
    mContext->mRequest = 0;
}

void BattleCommand::PollModeClosed() {
    if (mMode != COMMAND_MODE_CLOSING) {
        return;
    }
    SceneContext *context = mContext;
    if (context->mRequest != 0) {
        context->mRequest = 0;
        mMode            = COMMAND_MODE_NONE;
    }
}

// Window items of the command screen (modified at run time, so not const): slot 0 the main button, 1..5 the
// commands, 6..9 the player markers.

void BattleCommand::OpenNextMode(u32 flag) {
    if (mMode != COMMAND_MODE_NONE) {
        return;
    }
    InitCommandMainScreen(this, 0);
    switch (mNextMode) {
        case COMMAND_MODE_MAP:
            mCommandMap = new BattleCommandMap(data_020d433c.mFieldId);
            mCommandMap->Init();
            SpriteManager::Get().sFade[0] = FADE_MODE_TO_ZERO;
            break;
        case COMMAND_MODE_PAGE:
            mCommandPage = new BattleCommandPage(0, -1);
            SetCommandPageCardHidden(mCommandPage, 1);
            CreateCommandPageWindow(mCommandPage, 0x18000, 0xe, 0xc0, 0x14, 0xe0, 0x14, 0);
            break;
        case COMMAND_MODE_LIST:
            mCommandList = new BattleCommandList(0, 0, 0, flag);
            CreateCommandListWindow(mCommandList, 0x18000, 0xe, 0xc0, 0x14, 0xe0, 0x14, 0);
            break;
    }
    mMode     = mNextMode;
    mNextMode = COMMAND_MODE_NONE;
}

// Marks the cells of the battle grid that exist on the current field (by data_020d433c.mFieldId).
void BattleCommand::InitGridMask() {
    if (data_020d433c.mLastBattle != 0) {
        MI_CpuFill8(mGridMask, 0, 0x8c);
        mGridMask[6][4] = 1;
        return;
    }
    MI_CpuFill8(mGridMask, 1, 0x8c);
    u8 row;
    u8 col;
    switch (data_020d433c.mFieldId) {
        case 2: {
            row = 0;
            do {
                col = 0;
                do {
                    if (col == 0 || col >= 9 || row <= 1 || row >= 12) {
                        mGridMask[row][col] = 0;
                    }
                    if (((col == 1 || col == 8) && (row <= 3 || row >= 10)) ||
                        ((col == 2 || col == 7) && (row <= 2 || row >= 11))) {
                        mGridMask[row][col] = 0;
                    }
                    col++;
                } while (col < 10);
                row++;
            } while (row < 14);
            break;
        }
        case 1:
            mGridMask[3][2]  = 0;
            mGridMask[10][2] = 0;
            mGridMask[3][7]  = 0;
            mGridMask[10][7] = 0;
        case 0:
        case 3:
        case 7: {
            row = 0;
            do {
                col = 0;
                do {
                    if (col <= 1 || col >= 8 || row <= 2 || row >= 11) {
                        mGridMask[row][col] = 0;
                    }
                    col++;
                } while (col < 10);
                row++;
            } while (row < 14);
            break;
        }
        case 4: {
            row = 0;
            do {
                col = 0;
                do {
                    if (((col == 0 || col == 9) && (row <= 2 || row >= 11)) ||
                        ((col == 1 || col == 8) && (row <= 1 || row >= 12)) ||
                        ((col == 2 || col == 7) && (row == 0 || row == 13))) {
                        mGridMask[row][col] = 0;
                    }
                    col++;
                } while (col < 10);
                row++;
            } while (row < 14);
            break;
        }
        case 5: {
            row = 0;
            do {
                col = 0;
                do {
                    if (col == 0 || col >= 9 || row <= 1 || row >= 12) {
                        mGridMask[row][col] = 0;
                    }
                    if (((col == 1 || col == 8) && (row <= 4 || row >= 9)) ||
                        ((col == 2 || col == 7) && (row <= 3 || row >= 10)) ||
                        ((col == 3 || col == 6) && (row <= 2 || row >= 11))) {
                        mGridMask[row][col] = 0;
                    }
                    col++;
                } while (col < 10);
                row++;
            } while (row < 14);
            break;
        }
        case 6: {
            row = 0;
            do {
                col = 0;
                do {
                    if (row == 0 || row >= 13) {
                        mGridMask[row][col] = 0;
                    }
                    if (((col == 0 || col == 9) && (row <= 4 || row >= 9)) ||
                        ((col == 1 || col == 8) && (row <= 2 || row >= 11)) ||
                        ((col == 2 || col == 7) && (row <= 1 || row >= 12))) {
                        mGridMask[row][col] = 0;
                    }
                    col++;
                } while (col < 10);
                row++;
            } while (row < 14);
            break;
        }
    }
}

void BattleCommand::SetCommandMessage() {
    BattlePlayerList *list = data_020d433c.mBattlePlayers;
    if (list->mTurnPlayer == list->mLocalPlayer) {
        if (list->mUnk_292 == 1) {
            mMessageWindow->StartMessage(0x20, 0, 1);
        } else {
            mMessageWindow->StartMessage(1, 0, 1);
        }
    } else {
        mMessageWindow->StartMessage(0x33, 0, 1);
    }
}

void BattleCommand::DrawTurnCount() {
    u16 turn = (u16) (data_020d433c.mBattlePlayers->mTurnCount - 1);
    u16 count = (u16) (turn / data_020d433c.mPlayerCount + 1);
    if (count > 99) {
        count = 99;
    }
    mTurnDigitsCellAnim->Update();
    mTurnDigitsCellAnim->DrawFrame(1, count / 10, 0x3e, 0xae, 0);
    mTurnDigitsCellAnim->Update();
    mTurnDigitsCellAnim->DrawFrame(1, count % 10, 0x48, 0xae, 0);
    mTurnCellAnim->Update();
    mTurnCellAnim->Draw(1, 0x80, 0x60, 0);
}

void ApplyScrollOffset(void *arg) {
    BattleCommand *command = (BattleCommand *) arg;
    int adjust             = gSaveData[0].mCommandItemXIndex ? 0 : 4;
    G2S_SetBG0Offset(command->mScrollX + sSubBg0Offset[0] - adjust, command->mScrollY + sSubBg0Offset[1]);
}
