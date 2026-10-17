#include "Battle/BattleMoneyReward.hpp"

// Include order is load-bearing: it sets where each singleton joins the size sort, and so the order of the
// destructor-chain nodes in .bss (TextManager, TouchManager, InputManager, FrameTask, SoundManager).
#include "Game/SaveData.hpp"
#include "Graphics/CellAnimation.hpp"
#include "Graphics/Display.hpp"
#include "System/FrameTask.hpp"
#include "Graphics/SpriteManager.hpp"
#include "System/FileSystem.hpp"
#include "System/InputManager.hpp"
#include "System/SoundManager.hpp"
#include "Text/TextManager.hpp"
#include "System/TouchManager.hpp"
#include "UI/SelectWindow.hpp"
#include <nitro/gx.h>
#include <nitro/mi.h>
#include <nitro/os.h>
#include <stdio.h>

extern u8 data_020d49a1[];

static inline void ClearBit(void *flags, int index) {
    ((u8 *) flags)[index / 8] &= ~(1 << (index % 8));
}

static const u16 sSparkleFrames[6][2] = {
    {0x22, 0x5c}, {0x36, 0x70}, {0x0e, 0x48}, {0x18, 0x52}, {0x2c, 0x66}, {0x40, 0x7a},
};

static const WindowCellRecord sCells[1] = {
    {"/Game/2D/union_b/union_b_obj_cockpit_nce.bin", 0, 0, 0, 0, 1, {0, 1, 1, 0}},
};

static const WindowItemRecord sItems[1] = {
    {0, 5, 201, 170, 52, 20, 0, {-1, -1, -1, -1}, 0, 1},
};

static const u32 sRewards[6][2] = {
    {3000, 25000},
    {8000, 40000},
    {10000, 65000},
    {15000, 35000},
    {20000, 30000},
    {50000, 40000},
};

static const s8 sSparkleSteps[15][4] = {
    {1, 1, 10, 2},
    {1, 1, 10, 3},
    {1, 1, 10, 4},
    {0, 0, 0, 0},
    {0, 0, 0, 0},
    {0, 0, 0, 0},
    {0, 0, 0, 0},
    {0, 0, 0, 0},
    {0, 0, 0, 0},
    {0, -1, 0, 2},
    {0, -1, 0, 3},
    {0, -1, 0, 4},
    {0, -1, 0, 2},
    {0, -1, 0, 3},
    {0, -1, 0, 4},
};

BattleMoneyReward::BattleMoneyReward()
    : SceneBase(0x3e8, 0) {
    mWindow     = 0;
    mTextSize   = 0;
    mGlyphCount = 0;
    mTimer      = 0xf0;
    mSlideA     = 0;
    mSlideB     = 0;
    mScroll     = 0;
    mHold       = 0x78;
    mAmountVisible     = 0;
    mState      = 0;
    mTransferTimer     = 0;
    mFade       = 0x40;
    mUnk_a9     = 0;
    mSparkleSoundTimer     = 0;
    mTransferSoundTimer     = 0;
    for (u8 i = 0; i < 9; i++) {
        mCellAnims[i] = 0;
    }
    for (u8 i = 0; i < 15; i++) {
        mCellAnimsB[i]      = 0;
        mPos[i][0]      = 0;
        mPos[i][1]      = 0;
        mTimers[i][0]   = 0;
        mTimers[i][1]   = 0;
    }
    mMessages    = 0;
    mTextBuffer  = 0;
    mGlyphBuffer = 0;
    MI_CpuClear16(mSparkleFrames, 0xc);
    switch (gSaveData[0].mUnk_42) {
        case 1:
            mKind = 0;
            break;
        case 3:
            mKind = 1;
            break;
        case 6:
            mKind = 2;
            break;
        case 7:
            mKind = 3;
            break;
        case 8:
            mKind = 4;
            break;
        case 9:
            mKind = 5;
            break;
        default:
            mKind = 0;
            break;
    }
    if (gSaveData[0].mUnk_5bf_0) {
        mAmount = sRewards[mKind][1];
    } else {
        mAmount = sRewards[mKind][0];
    }
    mTotal  = gSaveData[0].mMoney;
    u32 add = mAmount;
    if (add > 9999990) {
        add = 9999990;
    }
    gSaveData[0].mMoney += add;
    if (gSaveData[0].mMoney > 9999990) {
        gSaveData[0].mMoney = 9999990;
    }
    add = mAmount;
    if (add > 9999990) {
        add = 9999990;
    }
    gSaveData[0].mUnk_20 += add;
    if (gSaveData[0].mUnk_20 > 9999990) {
        gSaveData[0].mUnk_20 = 9999990;
    }
}

BattleMoneyReward::~BattleMoneyReward() {
    u8 i;
    CellAnimation *a;
    CellAnimation *b;
    delete mWindow;
    i = 0;
    do {
        a = mCellAnims[i];
        if (a) {
            delete a;
        }
        i++;
    } while (i < 9);
    u8 j;
    j = 0;
    do {
        b = mCellAnimsB[j];
        if (b) {
            delete b;
        }
        j++;
    } while (j < 15);
    ClearText();
    if (mMessages) {
        OS_Free(mMessages);
    }
    if (mTextBuffer) {
        OS_Free(mTextBuffer);
    }
    if (mGlyphBuffer) {
        OS_Free(mGlyphBuffer);
    }
    FrameTask::Get().StopAllChannels();
}

void BattleMoneyReward::Update() {
    u8 draw = 1;
    switch (mState) {
        case 0:
            InitDisplay();
            Init();
            mSlideA = 0x108;
            mSlideB = 0x108;
            SpriteManager::Get().StartFade(2);
            mFade = 0x40;
            draw  = 0;
            mState++;
            break;
        case 1:
            SpriteManager::Get();
            if (SpriteManager::sFade[0] <= 1) {
                FrameTask::Get().StartChannel(0, 0x18);
                mAmountVisible = 1;
                mState++;
            }
            break;
        case 2:
            if (mCellAnims[1]->mFinished != 0) {
                mState++;
            }
            break;
        case 3:
            if (mSlideA != 0) {
                if (mSlideA >= 0x18) {
                    mSlideA -= 0x18;
                } else {
                    mSlideA = 0;
                }
            } else {
                mState++;
            }
            break;
        case 4:
            if (mSlideB != 0) {
                if (mSlideB >= 0x18) {
                    mSlideB -= 0x18;
                } else {
                    mSlideB = 0;
                }
            } else {
                mTransferTimer = 0x78;
                mTransferSoundTimer = 0x78;
                mState++;
            }
            break;
        case 5:
            mTransferTimer--;
            if (mTransferTimer <= 0) {
                if (mAmount > 1000) {
                    mTransferTimer = 10;
                    mAmount -= 1000;
                    mTotal += 1000;
                } else {
                    mTotal += mAmount;
                    mAmount = 0;
                    mWindow->mEnabled = draw;
                    mState++;
                }
            }
            mTransferSoundTimer--;
            if (mTransferSoundTimer <= 0) {
                SoundManager::Get().StartSeqArc(0x36);
                mTransferSoundTimer = 10;
            }
            if (InputManager::Get().IsHeld(1) | TouchManager::Get().mPressed) {
                mTotal += mAmount;
                mAmount = 0;
                mWindow->mEnabled = 1;
                if (mHold > 0) {
                    mUnk_a9 = 1;
                    mHold   = 0;
                }
                mState = 7;
            }
            PrintTotal();
            break;
        case 6:
            if (mHold == 0) {
                mState++;
            }
            break;
        case 7:
            if (mUnk_a9 == 0) {
                if (mWindow->Update(draw) == 1) {
                    SpriteManager::Get().StartFade(3);
                    mState++;
                }
            }
            if (mFade != 0 && mUnk_a9 != 1) {
                break;
            }
            if (mTimer > 0) {
                mTimer--;
                u8 n = 3;
                u8 k = 0;
                u8 hit;
                s8 cool = mSparkleSoundTimer;
                if (cool > 0) {
                    mSparkleSoundTimer--;
                }
                hit = mSparkleSoundTimer > 0;
                do {
                    u32 length = mCellAnimsB[n]->GetDuration();
                    mSparkleFrames[k]++;
                    if (mSparkleFrames[k] >= length) {
                        mSparkleFrames[k] = 0;
                    }
                    if (!hit) {
                        u8 m;
                        u16 frame = mSparkleFrames[k];
                        for (m = 0; m < 2; m++) {
                            if (frame == sSparkleFrames[k][m]) {
                                SoundManager::Get().StartSeqArc(0x70);
                                mSparkleSoundTimer = 30;
                                hit     = 1;
                                break;
                            }
                        }
                    }
                    k++;
                    n++;
                } while (n < 9);
            }
            break;
        case 8:
            if (SpriteManager::Get().IsFading()) {
                break;
            }
            G2S_SetBlendBrightness(0x1f, 0);
            draw = 0;
            if (!gSaveData[0].mUnk_5bf_0) {
                ClearBit(data_020d49a1, 13);
            }
            RequestScene(1);
            mFinished = 1;
            break;
    }
    if (mHold != 0) {
        mHold--;
        if (mHold == 0) {
            mUnk_a9 = 1;
        }
    }
    if (mUnk_a9 != 0) {
        if (mFade == 0) {
            mUnk_a9 = 0;
        } else {
            mFade--;
            u8 level = mFade / 4;
            G2S_SetBlendBrightness(0x1f, -level);
        }
    }
    if (draw) {
        Draw();
    }
}

void BattleMoneyReward::InitDisplay() {
    GX_SetDispSelect(1);
    SpriteManager::Get().SetupMode(1, 1);
    GX_SetGraphicsMode(1, 0, 0);
    GXS_SetGraphicsMode(0);
    G2_SetBG0Control(0, 1, 0, 1, 0);
    G2S_SetBG0Control(1, 1, 0, 1, 0);
    G2S_SetBG1Control(1, 1, 2, 5, 1);
    G2S_SetBG3Control(0, 1, 4, 5, 0);
    G2_SetBG0Offset(0, 0);
    G2S_SetBG0Offset(0xa8, 0);
    G2S_SetBG1Offset(0x3e, 0);
    G2S_SetBG3Offset(0x7b, 0);
    G2_SetBG0Priority(3);
    G2S_SetBG0Priority(3);
    G2S_SetBG1Priority(3);
    G2S_SetBG3Priority(3);
    GX_SetVisiblePlane(0x11);
    GXS_SetVisiblePlane(0x1b);
    G2_SetBlendBrightness(0x1f, 0);
    G2S_SetBlendBrightness(0x1f, -16);
}

void BattleMoneyReward::Init() {
    ChangeDir("/game/Tournament/Bg/");
    LoadAndQueueUpload(0, 1, 0, "tournament_u_bg_result_ncl.bin", 0, 0, 0);
    LoadAndQueueUpload(0, 11, 0, "tournament_u_bg_result_ncg.bin", 0, 0, 0);
    LoadAndQueueUpload(0, 7, 0, "tournament_u_bg_result_nsc.bin", 0, 0, 0);
    ChangeDir("/story/");
    LoadAndQueueUpload(1, 1, 0, "map_b_bg_00_ncl.bin", 0, 0, 0);
    LoadAndQueueUpload(1, 11, 0, "map_b_bg_00_ncg.bin", 0, 0, 0);
    LoadAndQueueUpload(1, 7, 0, "map_b_bg_00_nsc.bin", 0, 0, 0);
    LoadAndQueueUpload(1, 1, 0x6000, "map_b_bg_00_ncl.bin", 0, 0, 0);
    LoadAndQueueUpload(1, 12, 0, "map_b_bg_01_ncg.bin", 0, 0, 0);
    LoadAndQueueUpload(1, 8, 0, "map_b_bg_01_a_nsc.bin", 0, 0, 0);
    LoadAndQueueUpload(1, 10, 0, "map_b_bg_01_b_nsc.bin", 0, 0, 0);
    SpriteScreen &screen = SpriteManager::Get().Screen(0);
    ChangeDir("/Game/Tournament/Obj/");
    screen.LoadCharacters("tournament_u_obj_01.nce_ncg1D.bin", 0, 0);
    screen.LoadPalette("tournament_u_obj_01_ncl.bin", 0, 0);
    screen.LoadCharacters("tournament_u_obj_02.nce_ncg1D.bin", 0x7000, 0);
    screen.LoadPalette("tournament_u_obj_02_ncl.bin", 0xc0, 0);
    u32 kind = mKind;
    if (kind == 1) {
        kind = 13;
    }
    mCellAnims[0]            = new CellAnimation("tournament_u_obj_01_nce.bin", kind, 1, 1);
    mCellAnims[0]->mCharBase = screen.GetCharBase(0);
    mCellAnims[0]->mPaletteBase   = 0;
    mCellAnims[0]->mFrameIndex   = mCellAnims[0]->mFrames[mCellAnims[0]->mAnimIndex].mFrameCount - 1;
    for (u8 i = 1; i < 9; i++) {
        mCellAnims[i]            = new CellAnimation("tournament_u_obj_02_nce.bin", 2, 1, 1);
        mCellAnims[i]->mCharBase = screen.GetCharBase(0x7000);
        mCellAnims[i]->mPaletteBase   = 6;
    }
    mCellAnims[1]->SetAnimation(0);
    mCellAnims[2]->SetAnimation(1);
    SpriteScreen &screen2 = SpriteManager::Get().Screen(1);
    ChangeDir("/Game/2D/union_b/");
    screen2.LoadCharacters("union_b_obj_cockpit.nce_ncg1D.bin", 0, 0);
    screen2.LoadPalette("union_b_obj_cockpit_ncl.bin", 0, 0);
    ChangeDir("/story/");
    screen2.LoadCharacters("map_b_obj_02.nce_ncg1D.bin", 0x2000, 0);
    screen2.LoadPalette("map_b_obj_02_ncl.bin", 0x20, 0);
    for (u8 i = 0; i < 15; i++) {
        mCellAnimsB[i]            = new CellAnimation("map_b_obj_02_nce.bin", i, 1, 1);
        mCellAnimsB[i]->mCharBase = screen2.GetCharBase(0x2000);
        mCellAnimsB[i]->mPaletteBase   = 1;
    }
    for (u8 i = 3; i < 9; i++) {
        mCellAnimsB[i]->GetDuration();
        mSparkleFrames[i - 3] = 0;
    }
    ChangeDir("/");
    mMessages = (MessageFile *) LoadFile("/font/mes_result.mes", 0, 0, 0, 0, 0);
    mTextSize = TextManager::Get().BufferSize(0x20, 0x20);
    mGlyphCount = TextManager::Get().GlyphCount(0x80);
    mTextBuffer = OS_Alloc(mTextSize);
    mGlyphBuffer = OS_Alloc(mGlyphCount * 0xc);
    TextManager::Get().Init(mGlyphBuffer, 0, 0, 0x20, 0x20, 0x21);
    TextManager::Get().SetColor(0, 0, 15);
    PrintTotal();
    delete mWindow;
    mWindow = new SelectWindow(1, sItems, -1);
    mWindow->SetCells(1, sCells);
    mWindow->mCursor->mSelection = -1;
    mWindow->mEnabled            = 0;
}

void BattleMoneyReward::Draw() {
    mCellAnims[0]->Update();
    mCellAnims[0]->Draw(0, 0x80, 0x60, 2);
    if (mAmountVisible) {
        mCellAnims[1]->Update();
        mCellAnims[1]->Draw(0, 0x80, 0x60, 2);
        mCellAnims[2]->Update();
        mCellAnims[2]->Draw(0, (s16) (mSlideA + 0x80), 0x60, 2);
        u32 value = mAmount;
        u8 i      = 0;
        do {
            u8 digit = value % 10;
            value    = value / 10;
            mCellAnims[i + 3]->DrawFrame(0, digit, (s16) (mSlideA + (0xaf + i * -0x13)), 0x80, 2);
            if (value == 0) {
                break;
            }
            i++;
        } while (i < 6);
        SpriteManager::Get().Screen(0).AddOam(mGlyphBuffer, 8, (s16) (mSlideB + 8), 0xaa, 2, 0x140, 0xf, 0);
    }
    mWindow->Draw();
    mScroll = (mScroll + 3) % 0x8000;
    G2S_SetBG3Offset(((u32) mScroll >> 7) + 0x7b, 0);
    for (u8 i = 0; i < 15; i++) {
        for (u8 j = 0; j < 2; j++) {
            mTimers[i][j]--;
            if (mTimers[i][j] <= 0) {
                mTimers[i][j] = sSparkleSteps[i][j + 2];
                mPos[i][j]    = mPos[i][j] + sSparkleSteps[i][j];
            }
        }
        mCellAnimsB[i]->Update();
        mCellAnimsB[i]->Draw(1, (s16) (mPos[i][0] + 0x80), (s16) (mPos[i][1] + 0x60), 2);
        mCellAnimsB[i]->Draw(1, (s16) (mPos[i][0] + 0x80), (s16) (mPos[i][1] - 0xa0), 2);
    }
}

void BattleMoneyReward::PrintTotal() {
    ClearText();
    s16 y;
    s16 x;
    s16 y2;
    s16 x2;
    s16 y3;
    s16 x3;
    x = 0;
    y = 0x9c;
    TextManager::Get().mUse4bppFont = 1;
    TextManager::Get().PrintNumber(mTotal, 7, 0, mTextBuffer, 0x20, 2, &y, &x, 0, 2, 0x21, 0, 0, 1, 1);
    TextManager &text = TextManager::Get();
    x2 = x;
    y2 = y;
    text.Print(mMessages->GetText(2), mMessages->GetLength(2), mTextBuffer, 0x20, 2, &y2, &x2, 0, 0, 2, 0x21, 0, 0, 0, 0, 1, 1);
    TextManager &text2 = TextManager::Get();
    x3 = x;
    y3 = 0;
    text2.Print(mMessages->GetText(15), mMessages->GetLength(15), mTextBuffer, 0x20, 2, &y3, &x3, 0, 0, 2, 0x21, 0, 0, 0, 0, 1, 1);
    TextManager::Get().mUse4bppFont = 0;
    QueueUpload(SpriteManager::Get().Screen(0).mBgIndex, 4, 0xa000, mTextBuffer, mTextSize, 0);
}

void BattleMoneyReward::ClearText() {
    if (mTextBuffer) {
        MI_CpuClear16(mTextBuffer, mTextSize);
    }
}
