#include "System/TouchManager.hpp"
#include "System/InputManager.hpp"



#include "Battle/BattleBase.hpp"
#include "UI/StorytalkWindow.hpp"
#include "UI/MessageWindow.hpp"
#include "Battle/BattleObj01.hpp"

#include "Game/UnkStruct_020d433c.hpp"
#include "Graphics/CellAnimation.hpp"
#include "Graphics/SpriteManager.hpp"
#include "System/FileSystem.hpp"
#include "System/InputManager.hpp"
#include "System/SoundManager.hpp"
#include "System/TouchManager.hpp"
#include "Graphics/CharacterIcons.hpp"
#include <nitro/gx.h>
#include <nitro/fx.h>
#include <nitro/mi.h>
#include <nitro/os.h>
#include <stdio.h>

// Externals of other battle files.
u32 BattleRand32(u32 max);
// BattlePlayerList::mUnk_2a4 is a StorytalkWindow; BattleHud::Update and BattleObj00 drive it with SetMessage / NextPage.
extern const u8 gSmallCardX[];
extern const u32 gSmallCardCharBases[];
extern const s32 gPushPercentScales[];

BattleHud::BattleHud(BattleData *data, BattleParam *param, u8 mode, s8 *sidePlayers)
    : SceneBase(2, 0) {
    mLocalSide = mode;
    mUnk_10 = 0;
    mUnk_12 = 0;
    mPushPercent = 0;
    mPushPercentFixed = 0;
    mCutinDelayTimer = 0;
    mFrame5SoundPlayed = 0;
    mFrame15SoundPlayed = 0;
    mFrame25SoundPlayed = 0;
    mBarFillDone = 0;
    mCutinVisible = 0;
    mPerfectCutinVisible = 0;
    mSidesVisible = 1;
    mSlideSidesOutEnabled = 0;
    mSwapSide = 0;
    mClosed = 0;
    mTouched = 0;
    mPenToggleFrames = 0;
    mSidePlayers = (s8 (*)[2]) sidePlayers;
    mCountdownFrames = 0;
    mCountdownFrameCount = 0;
    mStartPushPercent = 0;
    mUnk_c0 = 0;
    mUnk_c3 = 0;
    mShakeSign = 0;
    mShakeFrames = 0;
    mShakesRemaining = 0;
    mUnk_117_0 = 0;
    mUnk_117_1 = 0;
    mUnk_117_2 = 0;
    mUnk_117_3 = 0;
    mAbilityXPerFrame = 0;
    mLargeCardYPerFrame = 0;
    mLargeCardY = 0;
    mLargeCardOpen = 0;
    mLargeCardClosing = 0;
    mState = 0;
    mPenX = 0;
    mPenY = 0;
    mData = data;
    mParam = param;
    mSide0X = 8;
    mSide1X = 0xb8;
    mPushAnimTimer[1] = 0;
    mPushAnimTimer[0] = mPushAnimTimer[1];
    mLastPush[0] = mLastPush[1] = 0;
    mUnk_26 = mUnk_27 = 0;
    mUnk_122[0] = mUnk_122[1] = 0;
    mBarFillPerFrame[0] = mBarFillPerFrame[1] = 0;
    mBarFill[0] = mBarFill[1] = 0;
    u8 i = 0;
    do {
        mCellAnims[i] = 0;
        i++;
    } while (i < 23);
    mObjectsRemaining[0] = mObjectsRemaining[1] = 0;
    mObjectIntervalFrameCount[0] = mObjectIntervalFrameCount[1] = 0;
    mObjectIntervalFrames[0] = mObjectIntervalFrames[1] = 0;
    i = 0;
    do {
        mObjs[i] = new BattleObj01(5, 0x7000);
        i++;
    } while (i < 15);
    mSide0PlayerAnimX = 32.0f;
    mSide1PlayerAnimX = 224.0f;
    mPlayerAnimXPerFrame = 96.0f / (float) mParam->mBattleFrameCount;
    mUnk_118[0] = 1;
    mUnk_118[1] = 1;
    mAbilityX[0] = -32;
    mAbilityX[1] = 0x120;
    mAbility0SlideState = mAbility1SlideState = 0;
    Init();
}

BattleHud::~BattleHud() {
    u8 i = 0;
    do {
        CellAnimation *anim = mCellAnims[i];
        if (anim) {
            delete anim;
        }
        i++;
    } while (i < 23);
    i = 0;
    do {
        BattleObj01 *obj = mObjs[i];
        if (obj) {
            delete obj;
        }
        i++;
    } while (i < 15);
    mData->mMessageWindow->mUnk_27 = 0;
}

extern const u32 gIconCharBase;
extern const s8 gIconPaletteBase;
u16 GetCardNumber(u32 id);
u8 GetCardInfo(u32 id, u32 kind);

void BattleHud::Init() {
    BattleEntry *entries = mData->mEntries;
    ChangeDir("/Game/Confrontation/Bg/");
    LoadAndQueueUpload(1, UPLOAD_KIND_BG_EXT_PLTT, 0, "vs_b_bg_01_ncl.bin", 0, 0, 0);
    LoadAndQueueUpload(1, UPLOAD_KIND_BG0_CHAR, 0, "vs_b_bg_01_ncg.bin", 0, 0, 0);
    LoadAndQueueUpload(1, UPLOAD_KIND_BG0_SCR, 0, "vs_b_bg_01_nsc.bin", 0, 0, 0);
    ChangeDir("/");
    MI_CpuClearFast((void *) 0x5000400, 0x20);
    void *fill = OS_Alloc(0x20);
    MI_CpuFill8(fill, 0x11, 0x20);
    QueueUpload(1, UPLOAD_KIND_BG2_CHAR, 0, fill, 0x20, 1);
    SpriteScreen &screen = SpriteManager::Get().mScreens[1];
    screen.SetObjVramMode(3);
    ChangeDir("/Game/Confrontation/Obj/");
    screen.LoadCharacters("vs_b_obj_01.nce_ncg1D.bin", 0x7000, 0);
    u8 *pal = (u8 *) LoadFile("vs_b_obj_01_ncl.bin", 0, 0, 0, 0, 0);
    QueueUpload(screen.mBgIndex, UPLOAD_KIND_OBJ_PLTT, 0xe0, pal + (u8) (entries[0].mAttribute + 2) * 0x20, 0x20, 0);
    QueueUpload(screen.mBgIndex, UPLOAD_KIND_OBJ_PLTT, 0x100, pal + (u8) (entries[1].mAttribute + 2) * 0x20, 0x20, 0);
    QueueUpload(screen.mBgIndex, UPLOAD_KIND_OBJ_PLTT, 0xa0, pal, 0x40, 1);
    u8 i = 0;
    do {
        mCellAnims[i] = new CellAnimation("vs_b_obj_01_nce.bin", 0, 1, 1);
        mCellAnims[i]->mCharBase = screen.GetCharBase(0x7000);
        mCellAnims[i]->mPaletteBase = 5;
        i++;
    } while (i < 8);
    mCellAnims[0]->SetAnimation(0);
    mCellAnims[1]->SetAnimation(2);
    mCellAnims[2]->SetAnimation(6);
    mCellAnims[3]->SetAnimation(8);
    mCellAnims[4]->SetAnimation(9);
    mUnk_c0 = mCellAnims[3]->GetDuration();
    mCellAnims[4]->SetAnimation(7);
    mCellAnims[5]->SetAnimation(0x12);
    mCellAnims[6]->SetAnimation(entries[0].mPlayer + 0x17);
    mCellAnims[7]->SetAnimation(entries[1].mPlayer + 0x1b);
    char path[0x80];
    u8 n;
    if (mLocalSide >= 2) {
        n = 1;
    } else {
        n = mParam->mKind + 1;
    }
    sprintf(path, "vs_cutin_cbattle%02d.nce_ncg1D.bin", n);
    screen.LoadCharacters(path, 0xe000, 0);
    sprintf(path, "vs_cutin_cbattle%02d_nce.bin", n);
    mCellAnims[9] = new CellAnimation(path, 0, 1, 1);
    mCellAnims[9]->mCharBase = screen.GetCharBase(0xe000);
    mCellAnims[9]->mPaletteBase = 9;
    mCellAnims[9]->SetAnimation(0);
    screen.LoadCharacters("vs_cutin_perfect.nce_ncg1D.bin", 0xd000, 0);
    screen.LoadPalette("vs_cutin_perfect_ncl.bin", 0x120, 0x20);
    mCellAnims[8] = new CellAnimation("vs_cutin_perfect_nce.bin", 0, 1, 1);
    mCellAnims[8]->mCharBase = screen.GetCharBase(0xd000);
    mCellAnims[8]->mPaletteBase = 9;
    mCellAnims[8]->SetAnimation(0);
    screen.LoadCharacters("vs_cutin_cbattle01.nce_ncg1D.bin", 0xf000, 0);
    mCellAnims[10] = new CellAnimation("vs_cutin_cbattle01_nce.bin", 0, 1, 1);
    mCellAnims[10]->mCharBase = screen.GetCharBase(0xf000);
    mCellAnims[10]->mPaletteBase = 9;
    mCellAnims[10]->SetAnimation(3);
    mCellAnims[11] = new CellAnimation("vs_cutin_cbattle01_nce.bin", 0, 1, 1);
    mCellAnims[11]->mCharBase = screen.GetCharBase(0xf000);
    mCellAnims[11]->mPaletteBase = 9;
    mCellAnims[11]->SetAnimation(4);
    screen.LoadCharacters("vs_b_obj_ability.nce_ncg1D.bin", 0x10000, 0);
    screen.LoadPalette("vs_b_obj_ability_ncl.bin", 0x140, 0x20);
    i = 0;
    u8 slot = 14;
    do {
        mCellAnims[slot] = new CellAnimation("vs_b_obj_ability_nce.bin", 0, 1, 1);
        mCellAnims[slot]->mCharBase = screen.GetCharBase(0x10000);
        mCellAnims[slot]->mPaletteBase = 10;
        switch (GetCardInfo(entries[i].mCardId, 3)) {
            case 0:
                mCellAnims[slot]->SetAnimation(0);
                break;
            case 1:
                mCellAnims[slot]->SetAnimation(1);
                break;
            case 2:
                mCellAnims[slot]->SetAnimation(2);
                break;
            case 3:
                mCellAnims[slot]->SetAnimation(3);
                break;
            case 5:
                mCellAnims[slot]->SetAnimation(4);
                break;
            case 4:
                if (entries[i].mCardId >= 0x8e && entries[i].mCardId <= 0x91) {
                    mCellAnims[slot]->SetAnimation(9);
                } else if (func_ov007_02222764(i, entries) == 5) {
                    if (GetCardInfo(entries[(u8) (i ^ 1)].mCardId, 3) != 5) {
                        mCellAnims[slot]->SetAnimation(6);
                    } else {
                        mCellAnims[slot]->SetAnimation(5);
                    }
                } else {
                    mCellAnims[slot]->SetAnimation(5);
                }
                break;
        }
        slot++;
        i++;
    } while (i < 2);
    mCellAnims[20] = new CellAnimation("vs_b_obj_ability_nce.bin", 0, 1, 1);
    mCellAnims[20]->mCharBase = screen.GetCharBase(0x10000);
    mCellAnims[20]->mPaletteBase = 10;
    mCellAnims[20]->SetAnimation(8);
    mCellAnims[16] = new CellAnimation("vs_b_obj_ability_nce.bin", 0, 1, 1);
    mCellAnims[16]->mCharBase = screen.GetCharBase(0x10000);
    mCellAnims[16]->mPaletteBase = 10;
    mCellAnims[16]->SetAnimation(7);
    screen.LoadPalette("vs_b_obj_down_ncl.bin", 0x1c0, 0);
    screen.LoadCharacters("vs_b_obj_pen.nce_ncg1D.bin", 0x14000, 0);
    mCellAnims[21] = new CellAnimation("vs_b_obj_pen_nce.bin", 0, 1, 1);
    mCellAnims[21]->mCharBase = screen.GetCharBase(0x14000);
    mCellAnims[21]->mPaletteBase = 9;
    if (mLocalSide < 2) {
        switch (mParam->mKind) {
            case 0:
                mPenX = 0x80;
                mPenY = 0x82;
                mCellAnims[21]->SetAnimation(0);
                break;
            case 3:
                mPenX = 0x80;
                mPenY = 0x82;
                mCellAnims[21]->SetAnimation(1);
                break;
            case 1:
                mPenX = 0x80;
                mPenY = 0x82;
            case 2:
                mCellAnims[21]->SetAnimation(2);
                break;
            case 4:
            case 5:
                mCellAnims[21]->SetAnimation(3);
                break;
        }
    } else {
        if (mLocalSide == 4) {
            mPenX = 0xd2;
        } else {
            mPenX = 0x26;
        }
        mPenY = 0x82;
        mCellAnims[21]->SetAnimation(0);
    }
    ChangeDir("/");
    ChangeDir("/Game/Character/Icon");
    mCellAnims[12] = new CellAnimation("icon_ch_base_nce.bin", 0, 1, 1);
    u32 base = gIconCharBase;
    mCellAnims[12]->mCharBase = screen.GetCharBase(base);
    s8 layer = gIconPaletteBase;
    mCellAnims[12]->mPaletteBase = layer;
    mCellAnims[13] = new CellAnimation("icon_ch_base_nce.bin", 0, 1, 1);
    mCellAnims[13]->mCharBase = screen.GetCharBase(base);
    mCellAnims[13]->mPaletteBase = layer;
    mCellAnims[12]->SetAnimation(7);
    mCellAnims[13]->SetAnimation(7);
    ChangeDir("/");
    i = 0;
    do {
        if (data_020d433c.mBattlePlayers->mPlayers[entries[i].mPlayer]->mSlots6cRemaining == 0) {
            mUnk_118[i] = 0xff;
        }
        BattleEntry *e = entries;
        e += i;
        u32 card;
        if (e->mCardId == 0) {
            if (mLocalSide == i) {
                mUnk_117_1 = 1;
            }
            card = 0;
        } else {
            int kind = GetCardInfo(entries[(u8) mLocalSide].mCardId, 3);
            if ((u8) (kind + 0xfc) <= 1) {
                mUnk_117_1 = 1;
            }
            card = GetCardNumber(e->mCardId);
        }
        LoadSmallCard(i, card);
        i++;
    } while (i < 2);
    if (mLocalSide >= 2) {
        mUnk_117_1 = 1;
    }
    LoadCountdown();
    mData->mMessageWindow->LoadMessageFile("mes_ControlBattle");
    if ((u32) mLocalSide <= 1) {
        mData->mMessageWindow->StartMessage(mParam->mKind, 0, 1);
    } else {
        mData->mMessageWindow->StartMessage(6, 0, 1);
    }
    mData->mMessageWindow->mUnk_27 = 1;
    SetupCountdown();
    i = 0;
    do {
        if (entries[i].mUnk_0f_1) {
            mUnk_118[i] = 0xff;
        }
        i++;
    } while (i < 2);
}

void BattleHud::LoadSmallCard(u8 side, int card) {
    u8 index;
    SpriteScreen &screen = SpriteManager::Get().mScreens[1];
    BattleEntry *entries = mData->mEntries;
    u8 selected = data_020d433c.mBattlePlayers->mUnk_2a9;
    u8 i;
    screen.SetObjVramMode(3);
    index = side + 0x11;
    ChangeDir("/Game/Card/Graphic/Small");
    char path[0x80];
    if (selected == entries[side].mPlayer) {
        sprintf(path, "card_s_%03d_ncg.bin", card);
    } else {
        sprintf(path, "card_s_000_ncg.bin", card);
    }
    u32 base = gSmallCardCharBases[side];
    screen.LoadCharacters(path, base, 0);
    if (side == 0) {
        screen.LoadExtPalette("card_s_000_ncl.bin", 0x600, 0);
    }
    mCellAnims[index] = new CellAnimation("card_s_000_nce.bin", 0, 1, 1);
    mCellAnims[index]->mCharBase = screen.GetCharBase(base);
    mCellAnims[index]->mPaletteBase = 3;
    mCellAnims[index]->SetAnimation(0);
    ChangeDir("/");
}

void BattleHud::LoadCountdown() {
    CellAnimation *old = mCellAnims[22];
    if (old) {
        delete old;
    }
    mCellAnims[22] = 0;
    SpriteScreen &screen = SpriteManager::Get().Screen(0);
    screen.SetObjVramMode(1);
    ChangeDir("/Game/Confrontation/Obj/");
    screen.LoadCharacters("vs_b_obj_countdown.nce_ncg1D.bin", 0, 0);
    screen.LoadPalette("vs_b_obj_countdown_ncl.bin", 0, 0x20);
    mCellAnims[22] = new CellAnimation("vs_b_obj_countdown_nce.bin", 0, 1, 1);
    mCellAnims[22]->mCharBase = screen.GetCharBase(0);
    mCellAnims[22]->mPaletteBase = 0;
    mCellAnims[22]->SetAnimation(0);
    ChangeDir("/");
}

void BattleHud::Update() {
    if (mClosed) {
        mFinished = 1;
        return;
    }
    switch (mState) {
        case 1:
            if (data_020d433c.mTutorialId == 0xe || (data_020d433c.mTutorialId >= 7 && data_020d433c.mTutorialId <= 10)) {
                mState = 2;
                data_020d433c.mBattlePlayers->mUnk_2a4->LoadGraphics();
                data_020d433c.mBattlePlayers->mUnk_2a4->SetMessage(data_020d433c.mBattlePlayers->mUnk_2af, 0x64, 0, 0);
                G2S_SetBlendBrightness(0x1f, -10);
            } else {
                SetupCountdown();
                mState = 3;
            }
            break;
        case 2: {
            if (InputManager::Get().IsPressed(1) || TouchManager::Get().mReleased) {
                data_020d433c.mBattlePlayers->mUnk_2a4->NextPage(1);
            }
            u32 limit = data_020d433c.mTutorialNumber == 4 ? 0 : 5;
            if (!data_020d433c.mBattlePlayers->mUnk_2a4->func_02087cac()) {
                data_020d433c.mBattlePlayers->mUnk_2af++;
                u8 n = data_020d433c.mBattlePlayers->mUnk_2af;
                if (n > limit) {
                    SetupCountdown();
                    mState = 3;
                    G2S_SetBlendBrightness(0x1f, 0);
                } else {
                    data_020d433c.mBattlePlayers->mUnk_2a4->SetMessage(n, 0x64, 0, 0);
                }
            }
            break;
        }
        case 3:
            if (UpdateBarFillCountdown()) {
                mState = 0;
            }
            break;
        case 4:
            StartSwap();
            mState++;
        case 5:
            if (UpdateSwap()) {
                mState = 0;
            }
            break;
        case 6:
            StartBarSlide();
            mState++;
        case 7:
            if (UpdateBarSlide()) {
                mState = 0;
            }
            break;
        case 8:
            StartCountdown();
            mState++;
        case 9:
            if (UpdateCountdown()) {
                mState = 0;
            }
            break;
    }
    Draw();
}

void CloseHud(void *obj) {
    ((BattleHud *) obj)->mUnk_23 |= 2;
}

void SetHudSequence(BattleHud *obj, u32 sequence) {
    switch (sequence) {
        case HUD_SEQUENCE_NONE:
            obj->mState = 0;
            break;
        case HUD_SEQUENCE_BAR_FILL_COUNTDOWN:
            obj->mState = 1;
            break;
        case HUD_SEQUENCE_SWAP:
            obj->mState = 4;
            break;
        case HUD_SEQUENCE_BAR_SLIDE:
            obj->mState = 6;
            break;
        case HUD_SEQUENCE_COUNTDOWN:
            obj->mState = 8;
            break;
    }
}

u8 IsHudSequenceRunning(BattleHud *obj) {
    return obj->mState ? 1 : 0;
}

void BattleHud::SetupCountdown() {
    u32 hi;
    u32 lo;
    int swapped;
    BattleData *data;
    BattleEntry *entries;
    u32 a;
    swapped = 0;
    data = mData;
    entries = data->mEntries;
    lo = entries[1].mUnk_00;
    a = data->mEntries[0].mUnk_00;
    if (a > lo) {
        hi = a;
    } else {
        hi = lo;
        lo = a;
        swapped = 1;
    }
    if ((int) hi / (int) lo >= 2) {
        if (swapped == 0) {
            mStartPushPercent = 0x59;
        } else {
            mStartPushPercent = 0xb;
        }
    } else {
        mStartPushPercent = (int) (lo * 100) / (int) hi;
        mStartPushPercent >>= 1;
        if (swapped == 0) {
            mStartPushPercent = 100 - mStartPushPercent;
        }
    }
    mCountdownFrameCount = mCellAnims[22]->GetDuration();
    u8 pct = mStartPushPercent;
    mBarFillPerFrame[0] = (float) pct / (float) mCountdownFrameCount / 100.0f;
    mBarFillPerFrame[1] = (float) (u8) (100 - pct) / (float) mCountdownFrameCount / 100.0f;
    u8 i = 0;
    do {
        int v = entries[i].mUnk_00;
        if (v > 20) {
            mObjectsRemaining[i] = v / 20;
        }
        mObjectIntervalFrameCount[i] = mCountdownFrameCount / mObjectsRemaining[i];
        i++;
    } while (i < 2);
    mObjectIntervalFrames[0] = mObjectIntervalFrameCount[0];
    mObjectIntervalFrames[1] = mObjectIntervalFrameCount[1];
    mDrawnValues[0] = entries[0].mUnk_00;
    mDrawnValues[1] = entries[1].mUnk_00;
    mDrawnValuesPerFrame[0] = mDrawnValues[0] / (float) mCountdownFrameCount;
    mDrawnValuesPerFrame[1] = mDrawnValues[1] / (float) mCountdownFrameCount;
    mCutinDelayTimer = mParam->mCutinDelayFrameCount;
}

BOOL BattleHud::UpdateBarFillCountdown() {
    u8 done = 0;
    mBarFill[0] = mBarFill[0] + mBarFillPerFrame[0];
    mBarFill[1] = mBarFill[1] + mBarFillPerFrame[1];
    if (mCountdownFrames == mCountdownFrameCount - mUnk_c0) {
        mCellAnims[3]->SetAnimation(10);
        mCellAnims[4]->SetAnimation(9);
    }
    u8 i = 0;
    do {
        if (mObjectsRemaining[i] != 0) {
            mObjectIntervalFrames[i]++;
            if (mObjectIntervalFrames[i] >= mObjectIntervalFrameCount[i]) {
                mObjectIntervalFrames[i] = 0;
                mObjectsRemaining[i]--;
                if (i == 0) {
                    StartObject(0, 0x28, 0xb4, 6);
                } else {
                    StartObject(1, 0xd8, 0xb4, 6);
                }
            }
        }
        if (mDrawnValues[i] - mDrawnValuesPerFrame[i] < 0.0f) {
            mDrawnValues[i] = 0;
        } else {
            mDrawnValues[i] -= mDrawnValuesPerFrame[i];
        }
        i++;
    } while (i < 2);
    UpdateCountdown();
    mCountdownFrames++;
    if (mCountdownFrames >= mCountdownFrameCount) {
        mPushPercent = mStartPushPercent;
        int fixed = mPushPercent << 12;
        float rounded;
        if (mPushPercent != 0) {
            rounded = 0.5f + (float) fixed;
        } else {
            rounded = (float) fixed - 0.5f;
        }
        mPushPercentFixed = (s32) rounded;
        mBarFillDone = 1;
    }
    if (mBarFill[0] > 0.5f) {
        func_ov007_02227460(0);
    } else if (mBarFill[1] > 0.5f) {
        func_ov007_02227460(1);
    }
    if (mBarFillDone == 1 && mUnk_c3 == 0) {
        done = 1;
    }
    return done;
}

void BattleHud::StartSwap() {
    BattleData *data = mData;
    mBarFillPerFrame[0] = mBarFillPerFrame[1] = 0;
    mCountdownFrames = 0;
    u32 pos = mPushPercent;
    u8 other = 100 - pos;
    u16 diff;
    if (pos > other) {
        diff = pos - other;
        mSwapSide = 1;
    } else {
        diff = other - pos;
        mSwapSide = 0;
    }
    mBarFillPerFrame[mSwapSide] = (float) diff / 20.0f;
    u16 first = data->mEntries[0].mUnk_00;
    u16 second = data->mEntries[1].mUnk_00;
    mData->mEntries[0].mUnk_00 = second;
    mData->mEntries[1].mUnk_00 = first;
    StartCountdown();
}

BOOL BattleHud::UpdateSwap() {
    mCountdownFrames++;
    if (mCountdownFrames > 0x14) {
        mCountdownFrames = 0x15;
    } else {
        u8 idx = mSwapSide;
        float v = mBarFillPerFrame[idx];
        float f;
        if (v > 0.0f) {
            f = 0.5f + 4096.0f * mBarFillPerFrame[idx];
        } else {
            f = 4096.0f * mBarFillPerFrame[idx] - 0.5f;
        }
        Push(idx, (s32) f, 1, 0, 0);
    }
    return UpdateCountdown();
}

void BattleHud::StartBarSlide() {
    BattleData *data = mData;
    mBarFillDone = 0;
    mBarFill[0] = (float) mPushPercent / 100.0f;
    mBarFill[1] = 1.0f - mBarFill[0];
    if (data->mEntries[0].mUnk_10) {
        mCellAnims[3]->SetAnimation(8);
        mUnk_27 = 1;
    } else {
        mCellAnims[4]->SetAnimation(7);
        mUnk_26 = 1;
    }
    mUnk_10 = 0;
    if (mLargeCardOpen) {
        CloseLargeCard();
    }
}

BOOL BattleHud::UpdateBarSlide() {
    BattleData *data = mData;
    if (data->mEntries[0].mUnk_10) {
        mBarFill[0] += 0.025f;
        mBarFill[1] -= 0.025f;
        if (mBarFill[0] >= 1.0f) {
            mBarFill[0] = 1.0f;
            mBarFill[1] = 0;
            mUnk_26 = 1;
        }
    } else {
        mBarFill[0] -= 0.025f;
        mBarFill[1] += 0.025f;
        if (mBarFill[1] >= 1.0f) {
            mBarFill[0] = 0;
            mBarFill[1] = 1.0f;
            mUnk_27 = 1;
        }
    }
    return mUnk_26 == 1 && mUnk_27 == 1;
}

void BattleHud::StartCountdown() {
    mFrame25SoundPlayed = 0;
    mFrame15SoundPlayed = 0;
    mFrame5SoundPlayed = 0;
    mCellAnims[22]->ResetAnimation();
}

BOOL BattleHud::UpdateCountdown() {
    switch (mCellAnims[22]->mFrameIndex) {
        case 5:
            if (!mFrame5SoundPlayed) {
                mFrame5SoundPlayed = 1;
                SoundManager::Get().StartSeqArc(0x26);
                func_020247fc(&SoundManager::Get(), 0x400);
            }
            break;
        case 15:
            if (!mFrame15SoundPlayed) {
                mFrame15SoundPlayed = 1;
                SoundManager::Get().StartSeqArc(0x26);
                func_020247fc(&SoundManager::Get(), 0x400);
            }
            break;
        case 25:
            if (!mFrame25SoundPlayed) {
                mFrame25SoundPlayed = 1;
                SoundManager::Get().StartSeqArc(0x26);
                func_020247fc(&SoundManager::Get(), 0x400);
            }
            break;
    }
    if (mCellAnims[22]->mFinished == 0) {
        mCellAnims[22]->Update();
        mCellAnims[22]->Draw(0, 0x80, 0x60, 0);
        return 0;
    }
    if (mLargeCardOpen) {
        mUnk_117_0 = 1;
    }
    return 1;
}

BOOL BattleHud::SlideSidesOut() {
    if (mLocalSide != 2 && mLocalSide != 3) {
        mSide0X -= 10;
    }
    if (mLocalSide != 2 && mLocalSide != 4) {
        mSide1X += 10;
    }
    mUnk_10 += 10;
    if (mUnk_10 >= 0x48) {
        mUnk_10 = 0;
        if (mLocalSide < 2) {
            mSidesVisible = 0;
        }
        mSlideSidesOutEnabled = 0;
        return 1;
    }
    return 0;
}

void MovePlayerAnimX(BattleHud *obj) {
    obj->mSide0PlayerAnimX = obj->mSide0PlayerAnimX + obj->mPlayerAnimXPerFrame;
    obj->mSide1PlayerAnimX = obj->mSide1PlayerAnimX - obj->mPlayerAnimXPerFrame;
}

void BattleHud::func_ov007_02227460(int side) {
    mCellAnims[5]->SetAnimation(0x13 + mData->mEntries[side].mPlayer);
}

void BattleHud::Push(int mode, s32 value, s32 count, u8 boost, bool useLevel) {
    s32 delta = value * count;
    if (boost) {
        delta = FX_Mul(delta, 0x1800);
    }
    if (useLevel) {
        u8 pos;
        if (mode == 0) {
            pos = mPushPercent;
        } else {
            pos = 100 - mPushPercent;
        }
        u8 idx = pos / 10;
        if (idx >= 5) {
            idx = 9 - idx;
        }
        delta = FX_Mul(delta, gPushPercentScales[idx]);
    }
    mLastPush[mode] = delta;
    if (mode == 0) {
        mPushPercentFixed += delta;
        if (mPushPercentFixed >= 0x64000) {
            mPushPercentFixed = 0x63000;
        }
        mPushPercent = mPushPercentFixed >> 12;
    } else if (mode == 1) {
        if (mPushPercentFixed <= delta) {
            mPushPercentFixed = 0x1000;
        } else {
            mPushPercentFixed -= delta;
            if (mPushPercentFixed < 0x1000) {
                mPushPercentFixed = 0x1000;
            }
        }
        mPushPercent = mPushPercentFixed >> 12;
    }
}

u8 BattleHud::ComputeObjectCount(u32 value, u8 a, u8 b) {
    u32 scaled = value * b;
    u8 result = scaled / 100;
    if (result == 0) {
        result = a;
    }
    return result;
}

BOOL BattleHud::StartObject(u8 side, s16 x, s16 y, u8 flag) {
    BOOL result = 0;
    s16 ty;
    u16 angle;
    int motion;
    u8 i;
    if (!mBarFillDone) {
        if (side == 0) {
            ty = (s16) (s32) (32.0f + 192.0f * mBarFill[0]);
        } else {
            ty = (s16) (s32) (32.0f + 192.0f * (1.0f - mBarFill[1]) - 20.0f);
        }
    } else {
        s16 base = mPushPercent * 0xc0 / 100;
        if (side == 0) {
            ty = base + 0x1e;
        } else {
            ty = base + 0x32;
        }
    }
    angle = BattleRand32(0xffff);
    motion = 0;
    if (flag == BATTLE_MODE_CONFRONT_BG06) {
        motion = 1;
    } else if (flag == BATTLE_MODE_SPECTATOR) {
        motion = 2;
    }
    i = 0;
    do {
        if (mObjs[i]->Start(side, x, y, ty, 0x42, angle, motion)) {
            result = 1;
            break;
        }
        i++;
    } while (i < 15);
    return result;
}

void BattleHud::Draw() {
    u8 selected = data_020d433c.mBattlePlayers->mUnk_2a9;
    u8 i;
    mData->DrawCharacterIcons(0, 8, 0x1e);
    mData->DrawCharacterIcons(1, 0xb8, 0x1e);
    mData->mCharacterIcons->DrawIcons();
    if (mSidesVisible) {
        s16 pos[2];
        pos[0] = mSide0X;
        pos[1] = mSide1X;
        i = 0;
        do {
                        if (selected == mSidePlayers[i][0] || selected == mSidePlayers[i][1]) {
                if (mShakeSign != 0) {
                    pos[i] += 4;
                } else {
                    pos[i] -= 4;
                }
                mShakeFrames++;
                if (mShakeFrames >= 2) {
                    mShakeFrames = 0;
                    mShakeSign ^= 1;
                }
            }
            i++;
        } while (i < 2);
        mData->DrawBakuganAndNumber(0, pos[0], 0x5a, 2, mDrawnValues[0]);
        mData->DrawBakuganAndNumber(1, pos[1], 0x5a, 2, mDrawnValues[1]);
        if (mSlideSidesOutEnabled) {
            SlideSidesOut();
        }
    }
    mCellAnims[5]->Draw(1, 0x80, 0x22, 1);
    mCellAnims[6]->Draw(1, mSide0PlayerAnimX, 0x54, 1);
    mCellAnims[7]->Draw(1, mSide1PlayerAnimX, 0x54, 1);
    if (mBarFillDone) {
        u8 j = 12;
        i = 0;
        do {
            s16 x;
            if (i == 0) {
                x = 0xb;
            } else {
                x = 0xe7;
            }
            s8 first = mSidePlayers[i][0];
            if (first != -1) {
                mCellAnims[j]->SetAnimation(first + 7);
                mCellAnims[j++]->Draw(1, x, 0x3d, 0);
                s8 second = mSidePlayers[i][1];
                if (second != -1) {
                    mCellAnims[j]->SetAnimation(second + 7);
                    mCellAnims[j]->Draw(1, x, 0x4b, 0);
                }
            }
            i++;
        } while (i < 2);
    }
    func_ov007_02227c54();
    if (mCutinVisible) {
        if (!mTouched) {
            if (mCellAnims[9]->mAnimIndex == 0 && mCellAnims[9]->mFinished != 0) {
                mCellAnims[9]->ResetAnimation();
                mCellAnims[9]->SetAnimation(1);
            }
        } else if (mCellAnims[9]->mAnimIndex != 2) {
            if (mCutinDelayTimer != 0) {
                mCutinDelayTimer--;
            } else {
                mCellAnims[9]->ResetAnimation();
                mCellAnims[9]->SetAnimation(2);
            }
        }
        if (mCellAnims[9]->mAnimIndex == 1 && mPenX >= 0) {
            if (mLocalSide == 2) {
                mPenToggleFrames++;
                if (mPenToggleFrames >= 0x28) {
                    mPenToggleFrames = 0;
                    if (mPenX == 0x26) {
                        mPenX = 0xd2;
                    } else {
                        mPenX = 0x26;
                    }
                }
            }
            mCellAnims[21]->Update();
            mCellAnims[21]->Draw(1, mPenX, mPenY, 0);
        }
        mCellAnims[9]->Update();
        mCellAnims[9]->Draw(1, 0x80, 0x6e, 0);
        if (mCellAnims[9]->mAnimIndex == 1) {
            if (mLocalSide == 2 || mLocalSide == 3) {
                mCellAnims[10]->Update();
                mCellAnims[10]->Draw(1, 0x48, 0x82, 0);
            }
            if (mLocalSide == 2 || mLocalSide == 4) {
                mCellAnims[11]->Update();
                mCellAnims[11]->Draw(1, 0x98, 0x82, 0);
            }
        }
    }
    if (mPerfectCutinVisible) {
        mCellAnims[8]->Update();
        mCellAnims[8]->Draw(1, 0x80, 0x6e, 0);
    }
    DrawAbilitiesAndCards();
    mData->mMessageWindow->Update();
}

void BattleHud::func_ov007_02227bbc(u8 index) {
    u8 j = 3;
    u8 i = 0;
    do {
        if (i == index) {
            mCellAnims[j]->ResetAnimation();
            mCellAnims[j]->SetAnimation(12 - i);
            mUnk_122[i] = 1;
        } else if (mUnk_122[i] == 0) {
            mCellAnims[j]->ResetAnimation();
            mCellAnims[j]->SetAnimation(8 - i);
        }
        j++;
        i++;
    } while (i < 2);
}

void BattleHud::func_ov007_02227c54() {
    u8 i;
    u8 j;
    if (!mBarFillDone) {
        if (mUnk_26 == 0 && mBarFill[0] != 0.0f) {
            s16 x = (s16) (s32) (192.0f * mBarFill[0] + 32.0f + 8.0f);
            mCellAnims[3]->Update();
            mCellAnims[3]->Draw(1, x, 0x42, 1);
        }
        if (mUnk_27 == 0 && mBarFill[1] != 0.0f) {
            s16 x = (s16) (s32) (192.0f * (1.0f - mBarFill[1]) + 32.0f - 8.0f);
            mCellAnims[4]->Update();
            mCellAnims[4]->Draw(1, x, 0x42, 1);
        }
        i = 0;
        do {
            mObjs[i]->Update();
            i++;
        } while (i < 15);
        i = 0;
        j = 0;
        int x = 0x20;
        int frame = 0;
        do {
            if (mBarFill[i] >= 0.01f) {
                float f;
                if (mBarFill[i] >= 0.5f) {
                    mCellAnims[j]->SetAnimation(frame);
                    if (mBarFill[i] > 0.0f) {
                        f = 0.5f + 4096.0f * mBarFill[i];
                    } else {
                        f = 4096.0f * mBarFill[i] - 0.5f;
                    }
                    mCellAnims[j]->DrawTransformed(1, x, 0x40, 3, (s32) f, 0x1000, 0);
                } else {
                    mCellAnims[j]->SetAnimation(frame + 1);
                    if (mBarFill[i] > 0.0f) {
                        f = 0.5f + 4096.0f * mBarFill[i];
                    } else {
                        f = 4096.0f * mBarFill[i] - 0.5f;
                    }
                    mCellAnims[j]->DrawTransformed(1, x, 0x40, 3, (s32) f << 1, 0x1000, 0);
                }
            }
            frame = 2;
            x = 0xe0;
            j++;
            i++;
        } while (i < 2);
        return;
    }
    if (mPushPercent > 0x32) {
        func_ov007_02227460(0);
    } else if (mPushPercent < 0x32) {
        func_ov007_02227460(1);
    } else {
        func_ov007_02227460(mUnk_12);
        mUnk_12 ^= 1;
    }
    if (mUnk_122[0] != 0 || mUnk_122[1] != 0) {
        s16 x = (u16) (mPushPercent * 0xc0 / 100 + 0x20);
        j = 3;
        i = 0;
        do {
            if (!mUnk_117_3) {
                if (mCellAnims[j]->mFinished == 0) {
                    mCellAnims[j]->Update();
                } else {
                    mCellAnims[j]->ResetAnimation();
                }
            }
            mCellAnims[j]->Draw(1, x, 0x42, 1);
            j++;
            i++;
        } while (i < 2);
    } else {
        if (!mUnk_117_3) {
            u8 cur = mCellAnims[2]->mAnimIndex;
            if (mLastPush[0] != 0 && mLastPush[1] != 0) {
                u8 want;
                if (mLastPush[0] > mLastPush[1]) {
                    want = 5;
                } else if (mLastPush[0] < mLastPush[1]) {
                    want = 4;
                } else {
                    want = 6;
                }
                if (cur != want) {
                    mCellAnims[2]->ResetAnimation();
                    mCellAnims[2]->SetAnimation(want);
                }
            } else if (mLastPush[0] == 0 && mLastPush[1] == 0) {
                if (cur != 6) {
                    if (mPushAnimTimer[0] == 0 && mPushAnimTimer[1] == 0) {
                        mCellAnims[2]->ResetAnimation();
                        mCellAnims[2]->SetAnimation(6);
                    }
                }
            } else {
                i = 0;
                j = 5;
                do {
                    if (mLastPush[i] != 0) {
                        if (mPushAnimTimer[(u8) (i ^ 1)] != 0) {
                            mCellAnims[2]->ResetAnimation();
                            mCellAnims[2]->SetAnimation(6);
                        } else {
                            if (cur != j) {
                                mCellAnims[2]->ResetAnimation();
                                mCellAnims[2]->SetAnimation(j);
                            }
                            mPushAnimTimer[i] = mCellAnims[2]->GetDuration();
                        }
                    }
                    j--;
                    i++;
                } while (i < 2);
            }
            i = 0;
            do {
                if (mPushAnimTimer[i] != 0) {
                    mPushAnimTimer[i]--;
                }
                i++;
            } while (i < 2);
            mCellAnims[2]->Update();
        }
        mCellAnims[2]->Draw(1, (s16) (u16) (mPushPercent * 0xc0 / 100 + 0x20), 0x42, 1);
    }
    mLastPush[0] = mLastPush[1] = 0;
    i = 0;
    do {
        mObjs[i]->Update();
        i++;
    } while (i < 15);
    float p = (float) mPushPercent / 100.0f;
    float f;
    if (1.0f - p >= 0.5f) {
        mCellAnims[1]->SetAnimation(2);
        if (1.0f - p > 0.0f) {
            f = 0.5f + 4096.0f * (1.0f - p);
        } else {
            f = 4096.0f * (1.0f - p) - 0.5f;
        }
        mCellAnims[1]->DrawTransformed(1, 0xe0, 0x40, 3, (s32) f, 0x1000, 0);
    } else {
        mCellAnims[1]->SetAnimation(3);
        if (1.0f - p > 0.0f) {
            f = 0.5f + 4096.0f * (1.0f - p);
        } else {
            f = 4096.0f * (1.0f - p) - 0.5f;
        }
        mCellAnims[1]->DrawTransformed(1, 0xe0, 0x40, 3, (s32) f << 1, 0x1000, 0);
    }
    if (p >= 0.5f) {
        mCellAnims[0]->SetAnimation(0);
        if (p > 0.0f) {
            f = 0.5f + 4096.0f * p;
        } else {
            f = 4096.0f * p - 0.5f;
        }
        mCellAnims[0]->DrawTransformed(1, 0x20, 0x40, 3, (s32) f, 0x1000, 0);
    } else {
        mCellAnims[0]->SetAnimation(1);
        if (p > 0.0f) {
            f = 0.5f + 4096.0f * p;
        } else {
            f = 4096.0f * p - 0.5f;
        }
        mCellAnims[0]->DrawTransformed(1, 0x20, 0x40, 3, (s32) f << 1, 0x1000, 0);
    }
}

void func_ov007_0222844c(BattleHud *obj, u8 a, u8 b) {
    obj->mUnk_118[a] = 0;
    obj->mAbilityX[0] = -32;
    obj->mAbilityX[1] = 0x120;
    obj->mUnk_117 |= 0xc;
}

void func_ov007_0222847c(BattleHud *obj) {
    CellAnimation *anim = obj->mCellAnims[9];
    if (anim->mAnimIndex != 2) {
        anim->SetAnimation(2);
    }
}

void BattleHud::StartAbilitySlideIn(int side) {
    if (side == 0) {
        mAbility0SlideState = 1;
    } else {
        mAbility1SlideState = 1;
    }
    mAbilityXPerFrame = 20;
}

void BattleHud::StartAbilitySlideOut(int side) {
    if (side == 0) {
        mAbility0SlideState = 2;
    } else {
        mAbility1SlideState = 2;
    }
    mAbilityXPerFrame = 20;
}

void BattleHud::StartAbilitySlideAcross(int side) {
    if (side == 0) {
        mAbility0SlideState = 2;
        mAbility1SlideState = 3;
    } else {
        mAbility0SlideState = 3;
        mAbility1SlideState = 2;
    }
    mAbilityXPerFrame = 20;
}

BOOL BattleHud::IsAbilitySlideDone() {
    return mAbility0SlideState == 0 && mAbility1SlideState == 0;
}

void BattleHud::OpenLargeCard(int card) {
    mLargeCardOpen = 1;
    mLargeCardY = 0xb4;
    mLargeCardYPerFrame = 0x14;
    SpriteScreen &screen = SpriteManager::Get().Screen(1);
    u32 id = GetCardNumber(card);
    ChangeDir("/Game/Card/Graphic/Large");
    char path[0x80];
    sprintf(path, "card_l_%03d.nce_ncg1D.bin", id);
    screen.LoadCharacters(path, 0x17000, 0);
    sprintf(path, "card_l_%03d_ncl.bin", id);
    screen.LoadExtPalette(path, 0xa00, 0);
    sprintf(path, "card_l_%03d_nce.bin", id);
    mCellAnims[19] = new CellAnimation(path, 0, 1, 1);
    mCellAnims[19]->mCharBase = screen.GetCharBase(0x17000);
    mCellAnims[19]->mPaletteBase = 5;
    mCellAnims[19]->SetAnimation(0);
    ChangeDir("/");
}

void BattleHud::CloseLargeCard() {
    mLargeCardClosing = 1;
    mLargeCardYPerFrame = 0x14;
}

void BattleHud::DrawAbilitiesAndCards() {
    s16 y;
    u8 side;
    BattleEntry *entries = mData->mEntries;
    if (mAbility0SlideState != 0 || mAbility1SlideState != 0) {
        mAbilityXPerFrame -= 1;
        if (mAbilityXPerFrame < 4) {
            mAbilityXPerFrame = 4;
        }
    }
    switch (mAbility0SlideState) {
        case 1:
            mAbilityX[0] += mAbilityXPerFrame;
            if (mAbilityX[0] >= 0x80) {
                mAbilityX[0] = 0x80;
                mAbility0SlideState = 0;
            }
            break;
        case 2:
            mAbilityX[0] -= mAbilityXPerFrame;
            if (mAbilityX[0] <= -32) {
                mAbilityX[0] = -32;
                mAbility0SlideState = 0;
            }
            break;
        case 3:
            mAbilityX[0] += mAbilityXPerFrame;
            if (mAbilityX[0] >= 0x120) {
                mAbilityX[0] = 0x120;
                mAbility0SlideState = 0;
            }
            break;
    }
    switch (mAbility1SlideState) {
        case 1:
            mAbilityX[1] -= mAbilityXPerFrame;
            if (mAbilityX[1] <= 0x80) {
                mAbilityX[1] = 0x80;
                mAbility1SlideState = 0;
            }
            break;
        case 2:
            mAbilityX[1] += mAbilityXPerFrame;
            if (mAbilityX[1] >= 0x120) {
                mAbilityX[1] = 0x120;
                mAbility1SlideState = 0;
            }
            break;
        case 3:
            mAbilityX[1] -= mAbilityXPerFrame;
            if (mAbilityX[1] <= -32) {
                mAbilityX[1] = -32;
                mAbility1SlideState = 0;
            }
            break;
    }
    if (mLargeCardOpen | mLargeCardClosing) {
        mLargeCardYPerFrame -= 2;
        if (mLargeCardYPerFrame < 3) {
            mLargeCardYPerFrame = 3;
        }
    }
    y = 0x62;
    if (mLargeCardClosing) {
        mLargeCardY += mLargeCardYPerFrame;
        if (mLargeCardY > 0xb4) {
            mLargeCardY = 0xb4;
            mLargeCardClosing = 0;
            mLargeCardOpen = 0;
        }
    } else if (mLargeCardOpen) {
        mLargeCardY -= mLargeCardYPerFrame;
        if (mLargeCardY < 0x5f) {
            mLargeCardY = 0x5f;
            if (mShakesRemaining) {
                if (mShakeSign != 0) { y += 3; } else { y -= 3; }
                mShakeFrames++;
                if (mShakeFrames >= 1) {
                    mShakeFrames = 0;
                    mShakeSign ^= 1;
                    mShakesRemaining--;
                }
            }
        }
    }
    side = 0;
    do {
        if (!entries[side].mUnk_0f_0) {
            if (mUnk_118[side] != 0xff) {
                u8 x = gSmallCardX[side];
                mCellAnims[17 + side]->Draw(1, x, 0x20, mUnk_118[side]);
                if (!data_020d433c.mUnk_9a_2 && mLocalSide == side) {
                    if (!mUnk_117_1 && !mUnk_117_2 && mUnk_118[side] == 1) {
                        if (!entries[(u8) (side ^ 1)].mUnk_0f_3) {
                            mCellAnims[16]->Draw(1, x, 0x17, 0);
                        }
                    }
                }
            }
        }
        side++;
    } while (side < 2);
    if (mCellAnims[15]->mAnimIndex == 5) {
        s8 s = 1;
        do {
            if (mAbilityX[s] > -32 && mAbilityX[s] < 0x120) {
                mCellAnims[14 + s]->Draw(1, mAbilityX[s], 0x78, 0);
            }
            s--;
        } while (s >= 0);
    } else {
        u8 s = 0;
        do {
            if (mAbilityX[s] > -32 && mAbilityX[s] < 0x120) {
                mCellAnims[14 + s]->Draw(1, mAbilityX[s], 0x78, 0);
            }
            s++;
        } while (s < 2);
    }
    if (mLargeCardOpen | mLargeCardClosing) {
        if (mUnk_117_0 && !mLargeCardClosing) {
            mCellAnims[20]->Update();
            mCellAnims[20]->Draw(1, 0x82, 0x5a, 0);
        }
        mCellAnims[19]->DrawTransformed(1, y, mLargeCardY, 0, 0xb33, 0xb33, 0);
    }
}
