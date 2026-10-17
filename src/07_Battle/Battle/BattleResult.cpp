// Include and definition order here is load-bearing (docs/decompiling.md, "Data layout within a section"): the
// SoundManager, SpriteManager and InputManager singletons join the file's data size sort before the eight tables,
// WirelessManager, TouchManager, FrameTask and TextManager after them. That order reproduces the original's
// .rodata (the four 84-byte window tables), .data (the three retry_b_obj_00_nce.bin strings) and .bss (the six
// destructor nodes); found with tools/layout_search.py --climb.
#include "System/SoundManager.hpp"
#include "Graphics/SpriteManager.hpp"
#include "System/InputManager.hpp"
#include "Battle/BattleBase.hpp"
#include "Battle/BattleMoneyReward.hpp"
#include "Battle/BattleCharaAct.hpp"
#include "Game/UnkStruct_020d433c.hpp"
#include "Game/SaveData.hpp"
#include "Graphics/CellAnimation.hpp"
#include "Graphics/Display.hpp"
#include "Scene/SceneBase.hpp"
#include "System/FileSystem.hpp"
#include "UI/MessageWindow.hpp"
#include "UI/SelectWindow.hpp"
#include "Game/Ranking.hpp"
#include "Game/UnkStruct_020d48f0.hpp"
#include "Unknown/UnkFunctions.hpp"
#include "Graphics/AntennaIndicator.hpp"
#include "Graphics/CharacterIcons.hpp"
#include <nitro/gx.h>
#include <nitro/mi.h>
#include <nitro/os.h>
#include "Scene/GiftUnlockScene.hpp"
static const u8 sRankTable[19][3] = {{44, 90, 120}, {19, 38, 82}, {21, 38, 86}, {35, 60, 110}, {27, 47, 89}, {1, 2, 3}, {47, 82, 120}, {70, 140, 180}, {23, 42, 72}, {40, 61, 100}, {33, 54, 92}, {30, 48, 85}, {28, 47, 86}, {9, 25, 50}, {4, 15, 37}, {13, 29, 59}, {14, 31, 59}, {1, 2, 3}, {0, 0, 0}};
static const WindowCellRecord sResultCells[3] = {
    {"/Game/Battle/Obj/retry_b_obj_00_nce.bin", 1, 0, 0, 0, 1, {0, 2, 1, 0}},
    {"/Game/Battle/Obj/retry_b_obj_00_nce.bin", 3, 0, 0, 0, 1, {3, 5, 4, 0}},
    {"/Game/Battle/Obj/retry_b_obj_00_nce.bin", 6, 0, 0, 0, 1, {6, 8, 7, 0}},
};
static const WindowItemRecord sRetryItems[3] = {
    {0, 1, 45, 152, 78, 20, {0}, {0, 0, 1, 1}, 0, 2},
    {1, 1, 133, 152, 78, 20, {0}, {1, 1, 0, 0}, 0, 2},
    {2, 0x8000, 120, 18, 78, 20, {0}, {-1, -1, -1, -1}, 0, 0},
};
static const u8 sRewardRates[3][3] = {{10, 5, 2}, {30, 15, 6}, {10, 5, 2}};
static const WindowCellRecord sRetryCells[3] = {
    {"/Game/2D/union_b/union_b_obj_window_nce.bin", 3, 0x5000, 2, 0, 1, {3, 5, 4, 0}},
    {"/Game/2D/union_b/union_b_obj_window_nce.bin", 7, 0x5000, 2, 0, 1, {6, 8, 7, 0}},
    {"/Game/2D/union_b/union_b_obj_window_nce.bin", 10, 0x5000, 2, 0, 1, {10, 10, 10, 0}},
};
static const u8 sAnimPos[17][2] = {{170, 74}, {170, 90}, {110, 26}, {110, 42}, {110, 58}, {170, 26}, {170, 42}, {170, 58}, {0, 8}, {0, 24}, {0, 72}, {0, 104}, {0, 120}, {0, 136}, {0, 166}, {8, 40}, {16, 56}};
static const WindowItemRecord sResultItems[3] = {
    {0, 1, 48, 58, 160, 20, {0}, {2, 1, -1, -1}, 0, 2},
    {1, 1, 48, 86, 160, 20, {0}, {0, 2, -1, -1}, 0, 2},
    {2, 1, 48, 114, 160, 20, {0}, {1, 0, -1, -1}, 0, 2},
};
static const u16 sPrizeTable[2][6][9] = {
    {
        {10, 15, 20, 0, 0, 0, 0, 0, 0},
        {30, 40, 50, 60, 0, 0, 0, 0, 0},
        {100, 120, 150, 100, 0, 0, 0, 0, 0},
        {50, 70, 90, 110, 130, 150, 0, 0, 0},
        {70, 95, 120, 150, 0, 0, 0, 0, 0},
        {100, 120, 150, 180, 200, 220, 280, 0, 350},
    },
    {
        {120, 140, 200, 0, 0, 0, 0, 0, 0},
        {200, 250, 300, 350, 0, 0, 0, 0, 0},
        {300, 400, 500, 0, 0, 0, 0, 0, 0},
        {100, 130, 160, 180, 250, 0, 0, 0, 0},
        {130, 150, 170, 200, 0, 0, 0, 0, 0},
        {100, 120, 150, 180, 210, 240, 270, 0, 0},
    },
};
#include "System/WirelessManager.hpp"
#include "System/TouchManager.hpp"
#include "System/FrameTask.hpp"
#include "Text/TextManager.hpp"

u32 func_020779ec(void *unk);
BOOL SetMessageTypeAndCheckAll(BattlePlayerList *list, u32 id);
BOOL func_0202326c(SaveData *d);
s32 func_02087100();
s32 func_02087468();
void func_02086d48();
extern u8 data_020d43e5[];
extern UnkStruct_0220f540 data_0220f540;

class Camera;
class UnkStruct_02076058 {
public:
    /* 00 */ PAD(0x00, 0x78);
    /* 78 */ u8 mUnk_78;
    /* 79 */ PAD(0x79, 0x80);
    UnkStruct_02076058(u8 unk1);
};
struct UnkStruct_020c4230 {
    /* 00 */ PAD(0x00, 0x20);
    /* 20 */ u16 mUnk_20;
};
extern UnkStruct_020c4230 data_020c4230;

// Result screen of the battle (result_u_*, retry_*, select_arena_*): shows the outcome and the rewards per player.
class BattleResult : public SceneBase {
public:
    /* 0c */ SelectWindow *mWindow;
    /* 10 */ MessageWindow *mMessageWindow;
    /* 14 */ MessageFile *mResultMessages;
    /* 18 */ CellAnimation *mCellAnims[8];
    /* 38 */ void *mTextBuffers[3];
    /* 44 */ void *mGlyphBuffers[3];
    /* 50 */ u32 mTextBufferSizes[3];
    /* 5c */ u32 mGlyphCounts[3];
    /* 68 */ void *mUnk_68;
    /* 6c */ CharacterIcons *mIcons;
    /* 70 */ CellAnimation *mCellAnimsB[5];
    /* 84 */ u32 mHistoryValues[6];
    /* 9c */ s32 mPrizeMoney;
    /* a0 */ s32 mRewardMoney;
    /* a4 */ s32 mMoneyRemaining;
    /* a8 */ s32 mMoneyTotal;
    /* ac */ u8 mUnk_ac;
    /* ad */ u8 mUnk_ad;
    /* ae */ u8 mUnk_ae;
    /* af */ u8 mDelayTimer;
    /* b0 */ u8 mSoundTimer;
    /* b1 */ u8 mState;
    /* b2 */ u8 mMoneyStep;
    /* b3 */ u8 mUnk_b3;
    /* b4 */ u8 mSelectedId;
    /* b5 */

    BattleResult();
    virtual ~BattleResult();
    virtual void Update();
    void InitDisplay();
    void InitMainScreen();
    void LoadResultGraphics();
    void SetupRetryScreen();
    void DrawRetryText();
    void DrawRetryScreen();
    void DrawResultScreen();
    void LoadResultTextAndAmounts();
    void ComputeAmounts();
    void PrintResultText();
    void PrintMoney();
    void UpdateMoneyCount();
    void DrawResultText();
    void ClearTextBuffers();
};

BattleResult::BattleResult()
    : SceneBase(0x3e8, 0) {
    mWindow = 0;
    mMessageWindow = 0;
    mUnk_68 = 0;
    mPrizeMoney = 0;
    mRewardMoney = 0;
    mMoneyRemaining = 0;
    mMoneyTotal = 0;
    mUnk_ac = 0;
    mUnk_ad = 0;
    mUnk_ae = 0;
    mDelayTimer = 0;
    mSoundTimer = 0;
    mState = 0;
    mMoneyStep = 0;
    mUnk_b3 = 0;
    SpriteManager::Get();
    SpriteManager::sFade[0] = FADE_MODE_HOLD;
    SpriteManager::sBrightness[0] = -16;
    MI_CpuClear32(mCellAnims, sizeof(mCellAnims));
    MI_CpuClear32(mHistoryValues, sizeof(mHistoryValues));
    mResultMessages = 0;
    MI_CpuClear32(mTextBuffers, sizeof(mTextBuffers));
    MI_CpuClear32(mGlyphBuffers, sizeof(mGlyphBuffers));
    MI_CpuClear32(mTextBufferSizes, sizeof(mTextBufferSizes));
    MI_CpuClear32(mGlyphCounts, sizeof(mGlyphCounts));
    mIcons = 0;
    mCellAnimsB[0] = 0;
    mCellAnimsB[1] = 0;
    mCellAnimsB[2] = 0;
    mCellAnimsB[3] = 0;
    mCellAnimsB[4] = 0;
}

BattleResult::~BattleResult() {
    SpriteManager::Get();
    SpriteManager::sFade[0] = FADE_MODE_HOLD;
    SpriteManager::sBrightness[0] = -16;
    delete mWindow;
    delete mMessageWindow;
    FrameTask::Get().StopAllChannels();
    u8 i = 0;
    do {
        delete mCellAnims[i];
        i++;
    } while (i < 8);
    delete mIcons;
    delete mCellAnimsB[0];
    delete mCellAnimsB[1];
    delete mCellAnimsB[2];
    delete mCellAnimsB[3];
    delete mCellAnimsB[4];
    if (mResultMessages) {
        OS_Free(mResultMessages);
        mResultMessages = 0;
    }
    i = 0;
    do {
        if (mTextBuffers[i]) {
            ClearTextBuffers();
            OS_Free(mTextBuffers[i]);
        }
        if (mGlyphBuffers[i]) {
            OS_Free(mGlyphBuffers[i]);
            mGlyphBuffers[i] = 0;
        }
        i++;
    } while (i < 3);
}

void BattleResult::Update() {
    if (data_020d433c.mWireless) {
        WirelessManager::Get();
        if (!IsWirelessMode(&data_020c4030.mUnk_228)) {
            mFinished = 1;
        }
    }
    switch (mState) {
        case 0:
            InitDisplay();
            mState++;
            break;
        case 1: {
            u8 best = 0;
            u8 count = data_020d433c.mUnk_99;
            u8 i = 0;
            u32 v;
            u32 limit;
            BattlePlayerList *list = data_020d433c.mBattlePlayers;
            do {
                BattlePlayer *player = list->mPlayers[i];
                limit = count;
                v = player->mUnk_ee;
                if (data_020d433c.mHasPartners != 0) {
                    v = (u8) (v + list->mPlayers[player->mPartner]->mUnk_ee);
                    limit = 3;
                }
                if (v >= limit) {
                    best = i;
                    break;
                }
                i = (u8) (i + 1);
            } while (i < 4);
            if (data_020d433c.mWireless == 0) {
                u32 v = best == 0 ? 2 : 3;
                if (data_020d433c.mNextScene == SCENE_PARK) {
                    data_020d433c.mUnk_7c = v;
                } else {
                    data_020d433c.mUnk_7d = v;
                }
                if (best == 0) {
                    data_020d433c.mUnk_8c_4 = 0;
                } else if (best == 1) {
                    data_020d433c.mUnk_8c_4 = 1;
                } else {
                    data_020d433c.mUnk_8c_4 = 2;
                }
            }
            u8 me = data_020d433c.mBattlePlayers->mUnk_2a9;
            u8 partner = data_020d433c.mBattlePlayers->mPlayers[me]->mPartner;
            if (best == me || best == partner) {
                new BattleCharaAct(data_020d433c.mBattlePlayers->mCamera, me, 12, 0, 0);
                FrameTask::Get().StartChannel(0, 0x1a);
            } else {
                new BattleCharaAct(data_020d433c.mBattlePlayers->mCamera, me, 13, 0, 0);
                FrameTask::Get().StartChannel(0, 0x19);
            }
            mContext->mRequest = 0;
            if (data_020d43e5[best] < 99) {
                data_020d43e5[best] = data_020d43e5[best] + 1;
            }
            if (me != partner) {
                u8 other = data_020d433c.mBattlePlayers->mPlayers[best]->mPartner;
                if (data_020d43e5[other] < 99) {
                    data_020d43e5[other] = data_020d43e5[other] + 1;
                }
            }
            mState++;
            break;
        }
        case 2:
            if (mContext->mRequest != 0) {
                if (mContext->mRequest == 2) {
                    FrameTask::Get().StopAllChannels();
                    mContext->mRequest = 1;
                }
                if (!func_0203dd0c(&FrameTask::Get(), 0)) {
                    mState++;
                }
            }
            break;
        case 3:
            if (data_020d433c.mUnk_7a != 0) {
                if (data_020d433c.mNextScene == SCENE_TOURNAMENT && data_020d433c.mUnk_7d == 3) {
                    if (gSaveData[0].mUnk_4a < 9999) {
                        gSaveData[0].mUnk_4a++;
                    }
                    mState = 8;
                } else {
                    mState++;
                }
            } else {
                mState = 8;
            }
            break;
        case 4: {
            u8 k;
            BattlePlayerList *list = data_020d433c.mBattlePlayers;
            u8 me = list->mUnk_2a9;
            u8 numPlayers = data_020d433c.mPlayerCount;
            BattlePlayer *player = list->mPlayers[me];
            u8 own = player->mUnk_ee;
            u8 partner = player->mPartner;
            if (own >= data_020d433c.mUnk_99) {
                mUnk_b3 = 1;
            } else if (data_020d433c.mHasPartners != 0) {
                if ((u8) (own + list->mPlayers[partner]->mUnk_ee) >= data_020d433c.mUnk_99) {
                    mUnk_b3 = 1;
                }
            }
            if (mUnk_b3 != 0) {
                if (gSaveData[0].mUnk_48 < 9999) {
                    gSaveData[0].mUnk_48++;
                }
                if (data_020d433c.mHasPartners != 0) {
                    if (gSaveData[0].mUnk_82 < 999) {
                        gSaveData[0].mUnk_82++;
                    }
                } else if (numPlayers >= 4) {
                    if (gSaveData[0].mUnk_84 < 999) {
                        gSaveData[0].mUnk_84++;
                    }
                } else {
                    if (gSaveData[0].mUnk_80 < 999) {
                        gSaveData[0].mUnk_80++;
                    }
                }
                k = 1;
                do {
                    u8 idx = (me + k) % numPlayers;
                    if (data_020d433c.mHasPartners == 0 || idx != partner) {
                        u8 f6 = data_020d433c.mBattlePlayers->mPlayers[idx]->mCharacterId;
                        if (gSaveData[0].mUnk_5e[f6] < 999) {
                            gSaveData[0].mUnk_5e[f6]++;
                        }
                    }
                    k++;
                } while (k < numPlayers);
                if (data_020d433c.mBattlePlayers->mTurnCount != 0) {
                    int rounds = (u16) ((data_020d433c.mBattlePlayers->mTurnCount - 1) / numPlayers + 1);
                    if (rounds > 99) {
                        rounds = 99;
                    }
                    if (gSaveData[0].mFewestRounds != 0) {
                        if (gSaveData[0].mFewestRounds > rounds) {
                            gSaveData[0].mFewestRounds = rounds;
                        }
                    } else {
                        gSaveData[0].mFewestRounds = rounds;
                    }
                    if (gSaveData[0].mMostRounds < rounds) {
                        gSaveData[0].mMostRounds = rounds;
                    }
                }
            } else {
                if (gSaveData[0].mUnk_4a < 9999) {
                    gSaveData[0].mUnk_4a++;
                }
            }
            InitMainScreen();
            LoadResultGraphics();
            LoadResultTextAndAmounts();
            SpriteManager::Get();
            SpriteManager::sFade[0] = FADE_MODE_TO_ZERO;
            FrameTask::Get().StartChannel(0, 0x11);
            mUnk_68 = new UnkStruct_02076058(0);
            mContext->mRequest = 0;
            mState++;
            break;
        }
        case 5:
            SpriteManager::Get();
            if (SpriteManager::sFade[0] <= FADE_MODE_HOLD) {
                mState++;
            }
            DrawResultScreen();
            break;
        case 6:
            UpdateMoneyCount();
            DrawResultScreen();
            break;
        case 7:
            if (mContext->mRequest != 0) {
                if (func_020779ec(mUnk_68) != 0) {
                    mState++;
                } else {
                    mState = 10;
                }
            }
            DrawResultScreen();
            break;
        case 8:
            SpriteManager::Get().StartFade(FADE_MODE_DOWN);
            mState++;
            break;
        case 9:
            if (!SpriteManager::Get().IsFading()) {
                mState++;
            }
            break;
        case 10:
            mContext->mRequest = 1;
            if (data_020d433c.mNextScene != SCENE_ARENA_WINDOW && data_020d433c.mNextScene != SCENE_PARK) {
                if (data_020d433c.mUnk_7d == 2) {
                    if (func_0202326c(&gSaveData[0])) {
                        new BattleMoneyReward();
                        mContext->mRequest = 0;
                    }
                }
            }
            mState++;
            break;
        case 11:
            if (mContext->mRequest != 0) {
                mState++;
            }
            break;
        case 12:
            if (data_020d433c.mNextScene != SCENE_ARENA_WINDOW) {
                data_020d48f0.mPrevRank = data_020d48f0.mRank;
                Ranking *p = new Ranking();
                data_020d48f0.mRank = p->mPlayerRank;
                delete p;
            }
            mState++;
            break;
        case 13: {
            s32 id = -1;
            u8 scene = data_020d433c.mNextScene;
            u8 sub = data_020d433c.mUnk_7d;
            if ((scene == SCENE_TOURNAMENT) & (sub == 2)) {
                id = func_02087100();
            } else if (scene == SCENE_PARK) {
                id = func_02087468();
            }
            if (id >= 0) {
                new GiftUnlockScene(id);
                mContext->mRequest = 0;
                mState++;
            } else {
                mState += 2;
            }
            break;
        }
        case 14:
            if (mContext->mRequest != 0) {
                mState--;
            }
            break;
        case 15:
            if (data_020d433c.mNextScene == SCENE_ARENA_WINDOW) {
                mState++;
                break;
            }
            func_02086d48();
            RequestScene(1);
            mFinished = 1;
            break;
        case 16:
            SetupRetryScreen();
            FrameTask::Get().StartChannel(0, 0x11);
            mState++;
            break;
        case 17: {
            DrawRetryScreen();
            WirelessManager::Get();
            bool solo = data_020d433c.mWireless == 0;
            bool none = data_020c4230.mUnk_20 == 0;
            if (solo | none) {
                if (mWindow) {
                    delete mWindow;
                }
                mWindow = new SelectWindow(3, sResultItems, 0);
                mWindow->SetCells(3, sResultCells);
            }
            GXS_SetVisiblePlane(0x12);
            SpriteManager::Get().StartFade(FADE_MODE_TO_ZERO);
            mState++;
            break;
        }
        case 18:
            DrawRetryScreen();
            if (!SpriteManager::Get().IsFading()) {
                if (data_020d433c.mWireless != 0) {
                    WirelessManager::Get();
                    if (data_020c4230.mUnk_20 != 0) {
                        mState = 22;
                    } else {
                        mState++;
                    }
                } else {
                    mState++;
                }
            }
            break;
        case 19:
            DrawRetryScreen();
            if (mWindow->Update(1) == 1) {
                SelectWindowCursor *cursor = mWindow->mCursor;
                s32 sel = -1;
                if (cursor->mSelection != -1) {
                    sel = cursor->mItems[cursor->mSelection].mId;
                }
                *((u8 *) &data_0220f540 + 4) = mSelectedId = sel;
                if (mSelectedId == 2) {
                    if (data_020d433c.mWireless == 0) {
                        mState = 22;
                    } else {
                        mState++;
                    }
                } else {
                    mState = 22;
                }
            }
            break;
        case 20:
            DrawRetryScreen();
            if (mWindow) {
                delete mWindow;
            }
            mWindow = new SelectWindow(3, sRetryItems, 0);
            mWindow->SetCells(3, sRetryCells);
            mWindow->mCursor->mSelection = 1;
            GXS_SetVisiblePlane(0x13);
            mState++;
            break;
        case 21:
            DrawRetryText();
            DrawRetryScreen();
            if (mWindow->Update(1) == 1) {
                SelectWindowCursor *cursor = mWindow->mCursor;
                s8 sel = -1;
                if (cursor->mSelection != -1) {
                    sel = cursor->mItems[cursor->mSelection].mId;
                }
                if (sel != 0) {
                    mState = 17;
                } else {
                    mState++;
                }
            }
            break;
        case 22:
            DrawRetryScreen();
            if (SetMessageTypeAndCheckAll(data_020d433c.mBattlePlayers, 0x13)) {
                FrameTask::Get().StopAllChannels();
                SpriteManager::Get().StartFade(FADE_MODE_DOWN);
                mState++;
            }
            break;
        case 23:
            DrawRetryScreen();
            if (!func_0203dd0c(&FrameTask::Get(), 0)) {
                if (!SpriteManager::Get().IsFading()) {
                    u8 result;
                    if (data_020d433c.mWireless != 0) {
                        result = data_020d433c.mBattlePlayers->mPlayers[0]->mMsg.mTouchDown;
                    } else {
                        result = mSelectedId;
                    }
                    switch (result) {
                        case 0:
                            RequestScene(2);
                            mFinished = 1;
                            break;
                        case 1:
                            RequestScene(3);
                            mFinished = 1;
                            break;
                        case 2:
                            RequestScene(1);
                            mFinished = 1;
                            break;
                    }
                }
            }
            break;
    }
    if (mWindow) {
        mWindow->Draw();
    }
}

void BattleResult::InitDisplay() {
    SpriteManager::Get().SetupMode(1, 7);
    G2_SetBG0Offset(0, 0);
    G2_BlendNone();
    GX_SetDispSelect(1);
    GX_SetVisiblePlane(0x11);
    GX_SetGraphicsMode(1, 0, 1);
    G2_SetBG0Priority(0);
    G2_SetBG1Priority(1);
    GX_SetVisibleWnd(0);
    G2S_SetBG0Control(0, 1, 0, 1, 0);
    G2S_SetBG1Control(0, 1, 2, 4, 0);
    G2S_SetBG2Control(0, 1, 4, 4, 0);
    G2S_SetBG3Control(0, 1, 6, 4, 0);
    G2S_SetBG0Offset(0, 0);
    G2S_SetBG1Offset(0, 0);
    G2S_SetBG2Offset(0, 0);
    G2S_SetBG3Offset(0, 0);
    G2S_SetBG0Priority(0);
    G2S_SetBG1Priority(0);
    G2S_SetBG2Priority(0);
    G2S_SetBG3Priority(0);
    GXS_SetVisiblePlane(0x11);
    G2S_SetBlendBrightness(0x1f, 0);
    GXS_SetVisibleWnd(0);
    G2S_SetWndOutsidePlane(0x1f, FALSE);
    G2S_SetWnd0InsidePlane(0x1f, FALSE);
    G2S_SetWnd1InsidePlane(0x1f, FALSE);
    G2S_SetWndOBJInsidePlane(0x1f, FALSE);
    G2S_SetWnd0Position(0, 0, 0, 0);
    G2S_SetWnd1Position(0, 0, 0, 0);
    G2_BlendNone();
    G2S_BlendNone();
}

void BattleResult::InitMainScreen() {
    GX_SetDispSelect(1);
    SpriteManager::Get().SetupMode(1, 1);
    GX_SetGraphicsMode(1, 0, 0);
    G2_SetBG0Control(0, 1, 0, 1, 0);
    G2_SetBG0Offset(0, 0);
    G2_SetBG0Priority(3);
    GX_SetVisiblePlane(0x11);
    G2_SetBlendBrightness(0x1f, 0);
    GX_SetVisibleWnd(0);
    G2_BlendNone();
}

void BattleResult::LoadResultGraphics() {
    ChangeDir("/Game/Battle/Bg/");
    LoadAndQueueUpload(0, UPLOAD_KIND_BG_EXT_PLTT, 0, "result_u_bg_00_ncl.bin", 0, 0, 0);
    LoadAndQueueUpload(0, UPLOAD_KIND_BG0_CHAR, 0, "result_u_bg_00_ncg.bin", 0, 0, 0);
    LoadAndQueueUpload(0, UPLOAD_KIND_BG0_SCR, 0, "result_u_bg_00_nsc.bin", 0, 0, 0);
    ChangeDir("/Game/Battle/Obj/");
    SpriteScreen &screen = SpriteManager::Get().Screen(0);
    screen.SetObjVramMode(3);
    screen.LoadCharacters("bakugan_sub_s.nce_ncg1D.bin", 0x2000, 0);
    screen.LoadExtPalette("bakugan_sub_s_ncl.bin", 0, 0);
    screen.LoadCharacters("result_u_obj_00.nce_ncg1D.bin", 0, 0);
    screen.LoadPalette("result_u_obj_00_ncl.bin", 0x180, 0);
    u8 i = 0;
    do {
        mCellAnims[i] = new CellAnimation("result_u_obj_00_nce.bin", 0, 1, 1);
        mCellAnims[i]->mCharBase = screen.GetCharBase(0);
        mCellAnims[i]->mPaletteBase   = 12;
        mCellAnims[i]->SetAnimation(7);
        i++;
    } while (i < 2);
    i = 0;
    do {
        u8 *rec = (u8 *) data_020d433c.mBattlePlayers->mPlayers[data_020d433c.mBattlePlayers->mUnk_2a9] + 0x84;
        if (*(u16 *) (rec + i * 6) != 0) {
            mCellAnims[i + 2] = new CellAnimation("bakugan_sub_s_nce.bin", 0, 1, 1);
            mCellAnims[i + 2]->mCharBase = screen.GetCharBase(0x2000);
            mCellAnims[i + 2]->mPaletteBase   = 0;
            mCellAnims[i + 2]->SetMode(CELL_ANIM_MODE_STOP);
            mCellAnims[i + 2]->mFrameIndex = rec[i * 6 + 2];
        }
        i++;
    } while (i < data_020d433c.mUnk_99);
    AntennaIndicator *unk = data_020d433c.mAntenna;
    if (unk) {
        unk->SetupLayout(12);
    }
    ChangeDir("/");
}

void BattleResult::SetupRetryScreen() {
    SpriteManager::Get().SetupMode(1, 1);
    GX_SetDispSelect(1);
    GX_SetGraphicsMode(1, 0, 0);
    G2_SetBG0Control(0, 1, 0, 1, 0);
    G2_SetBG1Control(0, 0, 1, 4, 0);
    G2S_SetBG0Control(0, 1, 0, 1, 0);
    G2S_SetBG1Control(0, 1, 2, 4, 0);
    GX_SetVisiblePlane(0x13);
    GXS_SetVisiblePlane(0x12);
    G2_SetBG0Priority(3);
    G2_SetBG1Priority(0);
    G2S_SetBG0Priority(3);
    G2S_SetBG1Priority(3);
    G2_SetBG0Offset(0, 0);
    G2_SetBG1Offset(0, 0);
    G2S_SetBG0Offset(0, 0);
    G2S_SetBG1Offset(0, 0);
    ChangeDir("/Game/Battle/Bg/");
    LoadAndQueueUpload(0, UPLOAD_KIND_BG_EXT_PLTT, 0, "select_arena_u_bg_00_ncl.bin", 0, 0, 0);
    LoadAndQueueUpload(0, UPLOAD_KIND_BG0_CHAR, 0, "select_arena_u_bg_00_ncg.bin", 0, 0, 0);
    LoadAndQueueUpload(0, UPLOAD_KIND_BG0_SCR, 0, "select_arena_u_bg_00_nsc.bin", 0, 0, 0);
    LoadAndQueueUpload(1, UPLOAD_KIND_BG_EXT_PLTT, 0x2000, "select_arena_b_bg_00_ncl.bin", 0, 0, 0);
    LoadAndQueueUpload(1, UPLOAD_KIND_BG1_CHAR, 0, "select_arena_b_bg_00_ncg.bin", 0, 0, 0);
    LoadAndQueueUpload(1, UPLOAD_KIND_BG1_SCR, 0, "select_arena_b_bg_00_nsc.bin", 0, 0, 0);
    AntennaIndicator *unk = data_020d433c.mAntenna;
    if (unk) {
        unk->SetupLayout(12);
    }
    ChangeDir("/Game/Battle/Obj/");
    SpriteScreen &screen1 = SpriteManager::Get().Screen(1);
    screen1.LoadCharacters("retry_b_obj_00.nce_ncg1D.bin", 0, 0);
    screen1.LoadPalette("retry_b_obj_00_ncl.bin", 0, 0);
    SpriteScreen &screen0 = SpriteManager::Get().Screen(0);
    screen0.LoadCharacters("retry_u_obj_00.nce_ncg1D.bin", 0x4000, 0);
    screen0.LoadPalette("retry_u_obj_00_ncl.bin", 0x1c0, 0);
    mCellAnimsB[3] = new CellAnimation("retry_u_obj_00_nce.bin", 0, 1, 1);
    mCellAnimsB[3]->mCharBase = screen0.GetCharBase(0x4000);
    mCellAnimsB[3]->mPaletteBase   = 14;
    ChangeDir("/Game/Battle/Obj/");
    screen0.LoadCharacters("select_ch_u_obj_00.nce_ncg1D.bin", 0x1400, 0);
    screen0.LoadPalette("select_ch_u_obj_00_ncl.bin", 0x40, 0);
    mCellAnimsB[0] = new CellAnimation("select_ch_u_obj_00_nce.bin", 0, 1, 1);
    mCellAnimsB[0]->mCharBase = screen0.GetCharBase(0x1400);
    mCellAnimsB[0]->mPaletteBase   = 2;
    mCellAnimsB[4] = new CellAnimation("select_ch_u_obj_00_nce.bin", 2, 1, 1);
    mCellAnimsB[4]->mCharBase = screen0.GetCharBase(0x1400);
    mCellAnimsB[4]->mPaletteBase   = 2;
    mIcons = new CharacterIcons(0x5000, 10, 0);
    u8 i = 0;
    do {
        BattlePlayer *player = data_020d433c.mBattlePlayers->mPlayers[i];
        mIcons->SetCharacter(i, (s8) player->mCharacterId, player->NameInfo(), 0);
        s16 y = i * 0x28 + 0x25;
        mIcons->SetDrawArgs(i, 0x22, y + 2, 0);
        i++;
    } while (i < data_020d433c.mPlayerCount);
    ChangeDir("/Game/Character/Icon/");
    screen0.LoadCharacters("icon_ch_base.nce_ncg1D.bin", 0, 0);
    screen0.LoadPalette("icon_ch_base_ncl.bin", 0, 0);
    mCellAnimsB[1] = new CellAnimation("icon_ch_base_nce.bin", 7, 1, 1);
    mCellAnimsB[1]->mCharBase = screen0.GetCharBase(0);
    mCellAnimsB[1]->mPaletteBase   = 0;
    mCellAnimsB[2] = new CellAnimation("icon_ch_base_nce.bin", 0, 1, 1);
    mCellAnimsB[2]->mCharBase = screen0.GetCharBase(0);
    mCellAnimsB[2]->mPaletteBase   = 0;
    MessageFile *file = (MessageFile *) LoadFile("/font/mes_BattleArenaMenu.mes", 0, 0, 0, 0, 0);
    TextManager::Get().func_0202ce44(0, 0, 0, 0);
    void *map = OS_Alloc(TextManager::Get().GlyphCount(0x600));
    MI_CpuClearFast(map, 0x600);
    TextManager::Get().FillScrChars(0, (u16 *) map, 0, 0, 0x20, 0x18, 0x20);
    QueueUpload(0, UPLOAD_KIND_BG1_SCR, 0, map, 0x600, 1);
    void *buffer = OS_Alloc(TextManager::Get().BufferSize(0x20, 0x18));
    MI_CpuClearFast(buffer, 0x6000);
    TextManager::Get().mUse4bppFont = 1;
    u8 j = 0;
    do {
        s16 x = 0x48;
        u32 base = j * 0x28;
        s16 y = base + 0x2b;
        TextManager::Get().Print(data_020d433c.mBattlePlayers->mPlayers[j]->NameText(), data_020d433c.mBattlePlayers->mPlayers[j]->mNameLength, buffer, 0x20, 0x18, &x, &y, 0, 0, 2, 0, 0x10, 0, 0, 0, 1, 1);
        x = 0x80;
        u32 base2 = j * 0x28 + 0x35;
        y = base2;
        TextManager &t2 = TextManager::Get();
        s16 x2;
        s16 y2;
        y2 = y;
        x2 = x;
        t2.Print(file->GetText(10), file->GetLength(10), buffer, 0x20, 0x18, &x2, &y2, 0, 0, 2, 0, 0x10, 0, 0, 0, 1, 1);
        x = 0xd0;
        y = base2;
        TextManager::Get().PrintNumber(data_020d433c.mUnk_a9[j], 2, 0, buffer, 0x20, 0x18, &x, &y, 0, 2, 0, 0x10, 0, 1, 1);
        j++;
    } while (j < data_020d433c.mPlayerCount);
    TextManager::Get().mUse4bppFont = 0;
    QueueUpload(0, UPLOAD_KIND_BG1_CHAR, 0, buffer, 0x6000, 1);
    mTextBufferSizes[2] = TextManager::Get().BufferSize(0x18, 4);
    mTextBuffers[2] = OS_Alloc(mTextBufferSizes[2]);
    MI_CpuClear16(mTextBuffers[2], mTextBufferSizes[2]);
    mGlyphCounts[2] = TextManager::Get().GlyphCount(0x18);
    mGlyphBuffers[2] = OS_Alloc((s16) mGlyphCounts[2] * (s16) 0xc);
    TextManager::Get().Init(mGlyphBuffers[2], 1, 0, 0x18, 4, 0x11);
    TextManager &text = TextManager::Get();
    s16 height;
    s16 width;
    width  = 0;
    height = 0;
    text.Print(file->GetText(13), file->GetLength(13), mTextBuffers[2], 0x18, 4, &height, &width, 0, 0, 2, 0x11, 0xe, 0, 0, 1, 1, 1);
    QueueUpload(SpriteManager::Get().Screen(1).mBgIndex, UPLOAD_KIND_OBJ, 0x7000, mTextBuffers[2], mTextBufferSizes[2], 0);
    OS_Free(file);
    ChangeDir("/Game/2D/union_b/");
    LoadAndQueueUpload(1, UPLOAD_KIND_BG_EXT_PLTT, 0, "union_b_bg_window_ncl.bin", 0, 0, 0);
    LoadAndQueueUpload(1, UPLOAD_KIND_BG0_CHAR, 0, "union_b_bg_window_ncg.bin", 0, 0, 0);
    LoadAndQueueUpload(1, UPLOAD_KIND_BG0_SCR, 0, "union_b_bg_window_b_nsc.bin", 0, 0, 0);
    SpriteScreen &screen1b = SpriteManager::Get().Screen(1);
    screen1b.LoadCharacters("union_b_obj_window.nce_ncg1D.bin", 0x5000, 0);
    screen1b.LoadPalette("union_b_obj_window_ncl.bin", 0x40, 0);
    ChangeDir("/");
    mMessageWindow = new MessageWindow("mes_BattleArenaMenu", 1, 0x4000);
    mMessageWindow->func_02065bd4(0x14, 4, 0);
    mMessageWindow->SetWidth(0xd8);
    mMessageWindow->StartMessage(9, 0, 1);
    if (data_020d433c.mWireless != 0) {
        WirelessManager::Get();
        if (data_020c4230.mUnk_20 != 0) {
            mMessageWindow->StartMessage(4, 1, 1);
        }
    }
}

void BattleResult::DrawRetryText() {
    SpriteManager::Get().Screen(1).AddOam(mGlyphBuffers[2], mGlyphCounts[2], 0x20, 0x38, 0, 0xe0, 0xf, 0);
}

// The third and fourth Draws take y through the reused `y`. Because `y` is assigned again before the fourth call, its
// assignment for the third stays a statement of its own, so the y computation is emitted before that call's arguments
// and the scheduler keeps it after the stack store and ahead of the register arguments (as the original does).
void BattleResult::DrawRetryScreen() {
    mMessageWindow->Update();
    mCellAnimsB[3]->Draw(0, 0x80, 0x60, 0);
    u8 i = 0;
    do {
        s16 x;
        s16 y;
        s32 base = i * 0x28;
        s32 top  = base + 0x21;
        y        = top;
        x = 0x1e;
        mCellAnimsB[1]->SetAnimation(i + 7);
        mCellAnimsB[1]->mFrameIndex = 0;
        mCellAnimsB[1]->Draw(0, x, y, 0);
        bool flag = false;
        if (data_020d433c.mBattlePlayers->mPlayers[i]->mCpu) {
            flag = true;
        }
        if (flag) {
            y = top;
            x = 0x20;
            mCellAnimsB[1]->SetAnimation(11);
            mCellAnimsB[1]->mFrameIndex = i;
        } else if (i == data_020d433c.mBattlePlayers->mUnk_2a9) {
            y = base + 0x3a;
            x = 0x20;
            mCellAnimsB[1]->SetAnimation(i + 2);
            mCellAnimsB[1]->mFrameIndex = 0;
        }
        mCellAnimsB[1]->Draw(0, x, y, 0);
        y = base + 0x25;
        mCellAnimsB[2]->Draw(0, 0x20, y, 1);
        y = (i + 1) * 0x28;
        mCellAnimsB[0]->Draw(0, 0x20, y, 1);
        if (data_020d433c.mCharacterIds[i] != 0x15) {
            mCellAnimsB[4]->mFrameIndex = data_020d433c.mUnk_a3[i];
            mCellAnimsB[4]->Draw(0, 0x48, base + 0x37, 0);
        }
        i++;
    } while (i < data_020d433c.mPlayerCount);
    mIcons->DrawIcons();
}

void BattleResult::DrawResultScreen() {
    if (func_020779ec(mUnk_68)) {
        return;
    }
    u8 i = 0;
    do {
        if (mCellAnims[i]) {
            mCellAnims[i]->Update();
            switch (i) {
                case 0:
                    if (mMoneyStep >= 9) {
                        mCellAnims[i]->DrawFrame(0, 0, ((const u8 *) sAnimPos)[i * 2], ((const u8 *) sAnimPos)[i * 2 + 1], 1);
                    }
                    break;
                case 1:
                    if (mMoneyStep >= 11) {
                        mCellAnims[i]->Draw(0, ((const u8 *) sAnimPos)[i * 2], ((const u8 *) sAnimPos)[i * 2 + 1], 1);
                    }
                    break;
                case 2:
                case 3:
                case 4:
                case 5:
                case 6:
                case 7:
                    if (mMoneyStep >= (u8) (i - 2) + 3) {
                        if (mHistoryValues[3] != 0) {
                            mCellAnims[i]->Draw(0, ((const u8 *) sAnimPos)[i * 2], ((const u8 *) sAnimPos)[i * 2 + 1], 1);
                        } else {
                            mCellAnims[i]->Draw(0, ((const u8 *) sAnimPos)[i * 2 + 6], ((const u8 *) sAnimPos)[i * 2 + 7], 1);
                        }
                    }
                    break;
            }
        }
        i++;
    } while (i < 8);
    DrawResultText();
}

void BattleResult::LoadResultTextAndAmounts() {
    SpriteManager::Get().Screen(0).SetObjVramMode(3);
    ChangeDir("/font/");
    mResultMessages = (MessageFile *) LoadFile("mes_result.mes", 0, 0, 0, 0, 0);
    mTextBufferSizes[0] = TextManager::Get().BufferSize(0x20, 0x16);
    mTextBuffers[0] = OS_Alloc(mTextBufferSizes[0]);
    MI_CpuClear16(mTextBuffers[0], mTextBufferSizes[0]);
    mGlyphCounts[0] = TextManager::Get().GlyphCount(0x58);
    mGlyphBuffers[0] = OS_Alloc((s16) mGlyphCounts[0] * (s16) 0xc);
    TextManager::Get().Init(mGlyphBuffers[0], 0, 0, 0x20, 0x16, 0x21);
    mTextBufferSizes[1] = TextManager::Get().BufferSize(0x10, 6);
    mTextBuffers[1] = OS_Alloc(mTextBufferSizes[1]);
    MI_CpuClear16(mTextBuffers[1], mTextBufferSizes[1]);
    mGlyphCounts[1] = TextManager::Get().GlyphCount(0xc);
    mGlyphBuffers[1] = OS_Alloc((s16) mGlyphCounts[1] * (s16) 0xc);
    TextManager::Get().Init(mGlyphBuffers[1], 0, 0, 0x10, 6, 0x21);
    TextManager::Get().SetColor(0, 0, 0xe);
    ComputeAmounts();
    ClearTextBuffers();
    PrintResultText();
    PrintMoney();
}

void BattleResult::ComputeAmounts() {
    if (mUnk_b3 != 0) {
        if (data_020d433c.mNextScene == SCENE_TOURNAMENT) {
            switch (gSaveData[0].mUnk_42) {
                case 1:
                    mPrizeMoney = sPrizeTable[gSaveData[0].mUnk_5bf_0][0][gSaveData[0].mUnk_43] * 100;
                    break;
                case 3:
                    mPrizeMoney = sPrizeTable[gSaveData[0].mUnk_5bf_0][1][gSaveData[0].mUnk_43] * 100;
                    break;
                case 6:
                    mPrizeMoney = sPrizeTable[gSaveData[0].mUnk_5bf_0][2][gSaveData[0].mUnk_43] * 100;
                    break;
                case 7:
                    mPrizeMoney = sPrizeTable[gSaveData[0].mUnk_5bf_0][3][gSaveData[0].mUnk_43] * 100;
                    break;
                case 8:
                    mPrizeMoney = sPrizeTable[gSaveData[0].mUnk_5bf_0][4][gSaveData[0].mUnk_43] * 100;
                    break;
                case 9:
                    mPrizeMoney = sPrizeTable[gSaveData[0].mUnk_5bf_0][5][gSaveData[0].mUnk_43] * 100;
                    break;
                case 10:
                    mPrizeMoney = sPrizeTable[gSaveData[0].mUnk_5bf_0][0][gSaveData[0].mUnk_43] * 100;
                    break;
            }
        } else {
            const u8 (*tbl)[3] = sRankTable;
            u32 count = data_020d433c.mPlayerCount;
            u8 i = 1;
            u32 me = data_020d433c.mBattlePlayers->mUnk_2a9;
            do {
                u32 idx = (u8) (me + i);
                u8 rem = idx % count;
                BattlePlayer *player = data_020d433c.mBattlePlayers->mPlayers[rem];
                u8 a = player->mCharacterId;
                u8 b = player->mUnk_f7;
                if (data_020d433c.mHasPartners == 0 || rem != data_020d433c.mBattlePlayers->mPlayers[me]->mPartner) {
                    mPrizeMoney += tbl[a][b % 3] * 100;
                    if (gSaveData[0].mUnk_5bf_0) {
                        mPrizeMoney += 0x308 + 0x400;
                    }
                }
                i++;
            } while (i < count);
        }
    }
    u8 j = 0;
    u8 me2 = data_020d433c.mBattlePlayers->mUnk_2a9;
    do {
        mHistoryValues[j] = *(u16 *) ((u8 *) data_020d433c.mBattlePlayers->mPlayers[me2] + 0x84 + j * 6);
        j++;
    } while (j < data_020d433c.mUnk_99);
    mUnk_ad = data_020d433c.mBattlePlayers->mPlayers[me2]->mUnk_fd;
    mUnk_ae = data_020d433c.mBattlePlayers->mPlayers[me2]->mUnk_fc;
    u32 n2 = data_020d433c.mPlayerCount;
    u16 v = data_020d433c.mBattlePlayers->mTurnCount;
    if (v != 0) {
        v = v - 1;
    }
    if (n2 != 0) {
        v = v / n2;
    }
    if (mUnk_b3 != 0) {
        int k = 0;
        if (v >= 3) {
            k = (u16) (v - 3);
        }
        if (k < 3) {
            if (data_020d433c.mHasPartners != 0) {
                mRewardMoney = sRewardRates[1][k] * 100;
            } else if (data_020d433c.mUnk_99 < 6) {
                mRewardMoney = sRewardRates[0][k] * 100;
            } else {
                mRewardMoney = sRewardRates[2][k] * 100;
            }
        }
    }
    mUnk_ac = 1;
    BattlePlayerList *list2 = data_020d433c.mBattlePlayers;
    u32 count2 = data_020d433c.mPlayerCount;
    u8 m = 1;
    u32 me3 = list2->mUnk_2a9;
    u8 flag98 = data_020d433c.mHasPartners;
    do {
        u32 idx2 = (u8) (me3 + m);
        u8 rem2 = idx2 % count2;
        if (flag98 == 0 || rem2 != list2->mPlayers[me3]->mPartner) {
            if (list2->mPlayers[rem2]->mUnk_ee != 0) {
                mUnk_ac = 0;
                return;
            }
        }
        m++;
    } while (m < count2);
}

void BattleResult::PrintResultText() {
    if (mTextBuffers[0]) {
        MI_CpuClear16(mTextBuffers[0], mTextBufferSizes[0]);
    }
    s16 x = 0;
    s16 y = 0;
    s16 ly;
    TextManager::Get().mUse4bppFont = 1;
    x = 0x94;
    y = 0;
    TextManager::Get().PrintNumber(mPrizeMoney, 7, 0, mTextBuffers[0], 0x20, 0x16, &x, &y, 0, 2, 0x21, 0, 1, 1, 1);
    {
        TextManager &t2 = TextManager::Get();
        s16 x2;
        s16 y2;
        y2 = y;
        x2 = x;
        t2.Print(mResultMessages->GetText(2), mResultMessages->GetLength(2), mTextBuffers[0], 0x20, 0x16, &x2, &y2, 0, 0, 2, 0x21, 0, 0, 0, 1, 1, 1);
    }
    {
        TextManager &t3 = TextManager::Get();
        s16 x3;
        s16 y3;
        y3 = y;
        x3 = 0;
        t3.Print(mResultMessages->GetText(7), mResultMessages->GetLength(7), mTextBuffers[0], 0x20, 0x16, &x3, &y3, 0, 0, 2, 0x21, 0, 0, 0, 1, 1, 1);
    }
    if (mMoneyStep >= 2) {
        u8 n = mMoneyStep - 2;
        if (n > data_020d433c.mUnk_99) {
            n = data_020d433c.mUnk_99;
        }
        u8 i = 0;
        do {
            if (i != 0 && mHistoryValues[i] == 0) {
                break;
            }
            x = 0x94;
            if (i < 3 && mHistoryValues[3] != 0) {
                x -= 0x3c;
            }
            ly = (i % 3) * 16 + 16;
            TextManager::Get().PrintNumber(mHistoryValues[i], 7, 0, mTextBuffers[0], 0x20, 0x16, &x, &ly, 0, 2, 0x21, 0, 1, 1, 1);
            TextManager &t4 = TextManager::Get();
            s16 x2;
            s16 y2;
            y2 = ly;
            x2 = x;
            t4.Print(mResultMessages->GetText(2), mResultMessages->GetLength(2), mTextBuffers[0], 0x20, 0x16, &x2, &y2, 0, 0, 2, 0x21, 0, 0, 0, 1, 1, 1);
            i++;
        } while (i < n);
        {
            TextManager &t5 = TextManager::Get();
            s16 a;
            s16 b;
            b = 0x10;
            a = 0;
            t5.Print(mResultMessages->GetText(8), mResultMessages->GetLength(8), mTextBuffers[0], 0x20, 0x16, &a, &b, 0, 0, 2, 0x21, 0xe, 0, 0, 1, 1, 1);
        }
        {
            TextManager &t6 = TextManager::Get();
            s16 a;
            s16 b;
            b = 0x20;
            a = 8;
            t6.Print(mResultMessages->GetText(9), mResultMessages->GetLength(9), mTextBuffers[0], 0x20, 0x16, &a, &b, 0, 0, 2, 0x21, 0xe, 0, 0, 1, 1, 1);
        }
        {
            TextManager &t7 = TextManager::Get();
            s16 a;
            s16 b;
            b = 0x30;
            a = 0x10;
            t7.Print(mResultMessages->GetText(10), mResultMessages->GetLength(10), mTextBuffers[0], 0x20, 0x16, &a, &b, 0, 0, 2, 0x21, 0xe, 0, 0, 1, 1, 1);
        }
    }
    if (mMoneyStep >= 9) {
        x = 0x94;
        y = 0x40;
        TextManager::Get().PrintNumber(100, 7, 0, mTextBuffers[0], 0x20, 0x16, &x, &y, 0, 2, 0x21, 0, 1, 1, 1);
        {
            TextManager &t8 = TextManager::Get();
            s16 x2;
            s16 y2;
            y2 = y;
            x2 = x;
            t8.Print(mResultMessages->GetText(3), mResultMessages->GetLength(3), mTextBuffers[0], 0x20, 0x16, &x2, &y2, 0, 0, 2, 0x21, 0, 0, 0, 1, 1, 1);
        }
        x += 0x10;
        TextManager::Get().PrintNumber(mUnk_ad, 0, 0, mTextBuffers[0], 0x20, 0x16, &x, &y, 0, 2, 0x21, 0, 1, 1, 1);
        {
            TextManager &t9 = TextManager::Get();
            s16 x3;
            s16 y3;
            y3 = y;
            x3 = 0;
            t9.Print(mResultMessages->GetText(11), mResultMessages->GetLength(11), mTextBuffers[0], 0x20, 0x16, &x3, &y3, 0, 0, 2, 0x21, 0, 0, 0, 1, 1, 1);
        }
    }
    if (mMoneyStep >= 0xb) {
        x = 0x94;
        y = 0x50;
        TextManager::Get().PrintNumber(100, 7, 0, mTextBuffers[0], 0x20, 0x16, &x, &y, 0, 2, 0x21, 0, 1, 1, 1);
        {
            TextManager &t10 = TextManager::Get();
            s16 x2;
            s16 y2;
            y2 = y;
            x2 = x;
            t10.Print(mResultMessages->GetText(3), mResultMessages->GetLength(3), mTextBuffers[0], 0x20, 0x16, &x2, &y2, 0, 0, 2, 0x21, 0, 0, 0, 1, 1, 1);
        }
        x += 0x10;
        TextManager::Get().PrintNumber(mUnk_ae, 0, 0, mTextBuffers[0], 0x20, 0x16, &x, &y, 0, 2, 0x21, 0, 1, 1, 1);
    }
    if (mMoneyStep >= 0xc) {
        x = 0x94;
        y = 0x60;
        TextManager::Get().PrintNumber(mRewardMoney, 7, 0, mTextBuffers[0], 0x20, 0x16, &x, &y, 0, 2, 0x21, 0, 1, 1, 1);
        {
            TextManager &t11 = TextManager::Get();
            s16 x2;
            s16 y2;
            y2 = y;
            x2 = x;
            t11.Print(mResultMessages->GetText(2), mResultMessages->GetLength(2), mTextBuffers[0], 0x20, 0x16, &x2, &y2, 0, 0, 2, 0x21, 0, 0, 0, 1, 1, 1);
        }
        x = 0x94;
        y = 0x60;
        {
            TextManager &t12 = TextManager::Get();
            s16 x3;
            s16 y3;
            y3 = y;
            x3 = 0;
            t12.Print(mResultMessages->GetText(12), mResultMessages->GetLength(12), mTextBuffers[0], 0x20, 0x16, &x3, &y3, 0, 0, 2, 0x21, 0, 0, 0, 1, 1, 1);
        }
    }
    if (mMoneyStep >= 0xe) {
        if (mUnk_ac) {
            x = 0xc4;
            y = 0x70;
            TextManager &t13 = TextManager::Get();
            s16 x2;
            s16 y2;
            y2 = y;
            x2 = x;
            t13.Print(mResultMessages->GetText(4), mResultMessages->GetLength(4), mTextBuffers[0], 0x20, 0x16, &x2, &y2, 0, 0, 2, 0x21, 0, 0, 0, 1, 1, 1);
        }
        x = 0xa2;
        y = 0x70;
        TextManager &t14 = TextManager::Get();
        s16 x3;
        s16 y3;
        y3 = y;
        x3 = 0;
        t14.Print(mResultMessages->GetText(13), mResultMessages->GetLength(13), mTextBuffers[0], 0x20, 0x16, &x3, &y3, 0, 0, 2, 0x21, 0, 0, 0, 1, 1, 1);
    }
    {
        TextManager &t15 = TextManager::Get();
        s16 a;
        s16 b;
        b = 0x80;
        a = 0xcc;
        t15.Print(mResultMessages->GetText(2), mResultMessages->GetLength(2), mTextBuffers[0], 0x20, 0x16, &a, &b, 0, 0, 2, 0x21, 0, 0, 0, 1, 1, 1);
    }
    {
        TextManager &t16 = TextManager::Get();
        s16 a;
        s16 b;
        b = 0x80;
        a = 0;
        t16.Print(mResultMessages->GetText(14), mResultMessages->GetLength(14), mTextBuffers[0], 0x20, 0x16, &a, &b, 0, 0, 2, 0x21, 0, 0, 0, 1, 1, 1);
    }
    {
        TextManager &t17 = TextManager::Get();
        s16 a;
        s16 b;
        b = 0x9e;
        a = 0xcc;
        t17.Print(mResultMessages->GetText(2), mResultMessages->GetLength(2), mTextBuffers[0], 0x20, 0x16, &a, &b, 0, 0, 2, 0x21, 0, 0, 0, 1, 1, 1);
    }
    {
        TextManager &t18 = TextManager::Get();
        s16 a;
        s16 b;
        b = 0x9e;
        a = 0;
        t18.Print(mResultMessages->GetText(15), mResultMessages->GetLength(15), mTextBuffers[0], 0x20, 0x16, &a, &b, 0, 0, 2, 0x21, 0, 0, 0, 1, 1, 1);
    }
    TextManager::Get().mUse4bppFont = 0;
    DC_FlushRange(mTextBuffers[0], mTextBufferSizes[0]);
    QueueUpload(SpriteManager::Get().Screen(0).mBgIndex, UPLOAD_KIND_OBJ, 0x8000, mTextBuffers[0], mTextBufferSizes[0], 0);
}

void BattleResult::PrintMoney() {
    s16 outH = 0;
    s16 outW = 0;
    TextManager::Get().mUse4bppFont = 1;
    if (mMoneyStep >= 0x11) {
        TextManager::Get().PrintNumber(mMoneyRemaining, 7, 0, mTextBuffers[1], 0x10, 6, &outH, &outW, 0, 2, 0x21, 0, 1, 1, 1);
    } else {
        TextManager::Get().PrintNumber(mMoneyTotal, 7, 0, mTextBuffers[1], 0x10, 6, &outH, &outW, 0, 2, 0x21, 0, 1, 1, 1);
    }
    outH = 0;
    outW = 0x1e;
    if (mMoneyStep >= 0x11) {
        TextManager::Get().PrintNumber(gSaveData[0].mMoney - mMoneyRemaining, 7, 0, mTextBuffers[1], 0x10, 6, &outH, &outW, 0, 2, 0x21, 0, 1, 1, 1);
    } else {
        TextManager::Get().PrintNumber(gSaveData[0].mMoney, 7, 0, mTextBuffers[1], 0x10, 6, &outH, &outW, 0, 2, 0x21, 0, 1, 1, 1);
    }
    TextManager::Get().mUse4bppFont = 0;
    DC_FlushRange(mTextBuffers[1], mTextBufferSizes[1]);
    QueueUpload(SpriteManager::Get().Screen(0).mBgIndex, UPLOAD_KIND_OBJ, 0xf000, mTextBuffers[1], mTextBufferSizes[1], 0);
}

#define COUNT_STEP(CLEAR_ONLY)                                                         \
    if ((u32) mMoneyRemaining < 10 || (InputManager::Get().IsPressed(1) | TouchManager::Get().mPressed)) { \
        if (!(CLEAR_ONLY)) {                                                           \
            mMoneyTotal += mMoneyRemaining;                                                        \
        }                                                                              \
        mMoneyRemaining = 0;                                                                   \
    } else {                                                                           \
        mMoneyRemaining -= 10;                                                                 \
        if (!(CLEAR_ONLY)) {                                                           \
            mMoneyTotal += 10;                                                             \
        }                                                                              \
    }                                                                                  \
    if (mSoundTimer != 0) {                                                                \
        mSoundTimer--;                                                                     \
    } else {                                                                           \
        SoundManager::Get().StartSeqArc(8);                                          \
        mSoundTimer = 4;                                                                   \
    }

void BattleResult::UpdateMoneyCount() {
    if (InputManager::Get().IsPressed(1) | TouchManager::Get().mPressed) {
        mDelayTimer = 0;
    }
    if (mDelayTimer != 0) {
        mDelayTimer--;
        return;
    }
    if (mTextBuffers[1]) {
        MI_CpuClear16(mTextBuffers[1], mTextBufferSizes[1]);
    }
    switch (mMoneyStep) {
        case 0:
            mMoneyRemaining += mPrizeMoney;
            mDelayTimer = 30;
            mMoneyStep++;
            PrintResultText();
            break;
        case 1:
            if (mMoneyRemaining != 0) {
                COUNT_STEP(0)
            } else {
                mMoneyStep++;
                mSoundTimer = 0;
            }
            break;
        case 2:
            mMoneyRemaining = mHistoryValues[0];
            mDelayTimer = 30;
            mMoneyStep++;
            PrintResultText();
            break;
        case 3:
        case 4:
        case 5:
        case 6:
        case 7:
        case 8:
            if (mMoneyRemaining != 0) {
                COUNT_STEP(0)
            } else {
                mMoneyStep++;
                mSoundTimer = 0;
                mDelayTimer = 30;
                mMoneyRemaining = mHistoryValues[mMoneyStep - 3];
                if (mMoneyRemaining == 0) {
                    mMoneyStep = 9;
                }
                PrintResultText();
            }
            break;
        case 9:
            mMoneyRemaining = mUnk_ad * 100;
            mDelayTimer = 30;
            mMoneyStep++;
            PrintResultText();
            break;
        case 10:
            if (mMoneyRemaining != 0) {
                COUNT_STEP(0)
            } else {
                mMoneyRemaining = mUnk_ae * 100;
                mDelayTimer = 30;
                mMoneyStep++;
                mSoundTimer = 0;
                PrintResultText();
            }
            break;
        case 11:
            if (mMoneyRemaining != 0) {
                COUNT_STEP(0)
            } else {
                mDelayTimer = 30;
                mMoneyStep++;
                mSoundTimer = 0;
                PrintResultText();
            }
            break;
        case 12:
            mMoneyStep++;
            mMoneyRemaining = mRewardMoney;
            mDelayTimer = 30;
            PrintResultText();
            break;
        case 13:
            if (mMoneyRemaining != 0) {
                COUNT_STEP(0)
            } else {
                mMoneyStep++;
                mSoundTimer = 0;
                mDelayTimer = 30;
            }
            break;
        case 14:
            mMoneyStep++;
            mDelayTimer = 30;
            PrintResultText();
            break;
        case 15:
            data_020d433c.mUnk_8c_0 = 0;
            if (mUnk_ac != 0) {
                mMoneyTotal *= 2;
                if (data_020d433c.mNextScene == SCENE_PARK) {
                    data_020d433c.mUnk_8c_0 = 1;
                }
                SoundManager::Get().StartSeqArc(8);
            }
            mDelayTimer = 30;
            mMoneyStep++;
            break;
        case 16: {
            u32 add = mMoneyTotal;
            if (add > 9999990) {
                add = 9999990;
            }
            gSaveData[0].mMoney += add;
            if (gSaveData[0].mMoney > 9999990) {
                gSaveData[0].mMoney = 9999990;
            }
            add = mMoneyTotal;
            if (add > 9999990) {
                add = 9999990;
            }
            gSaveData[0].mUnk_20 += add;
            if (gSaveData[0].mUnk_20 > 9999990) {
                gSaveData[0].mUnk_20 = 9999990;
            }
            mMoneyRemaining = mMoneyTotal;
            mDelayTimer = 30;
            mMoneyStep++;
            break;
        }
        case 17:
            if (mMoneyRemaining != 0) {
                COUNT_STEP(1)
            } else {
                mState++;
                mSoundTimer = 0;
                *((u8 *) mUnk_68 + 0x78) = 1;
            }
            break;
    }
    PrintMoney();
}

void BattleResult::DrawResultText() {
    SpriteManager::Get().Screen(0).AddOam(mGlyphBuffers[0], mGlyphCounts[0], 10, 12, 0, 0x100, 14, 0);
    SpriteManager::Get().Screen(0).AddOam(mGlyphBuffers[1], mGlyphCounts[1], 0x9e, 0x8c, 0, 0x1e0, 14, 0);
}

void BattleResult::ClearTextBuffers() {
    u8 i = 0;
    do {
        if (mTextBuffers[i]) {
            MI_CpuClear16(mTextBuffers[i], mTextBufferSizes[i]);
        }
        i++;
    } while (i < 3);
}
