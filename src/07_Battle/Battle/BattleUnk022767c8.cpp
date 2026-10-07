#include "Battle/BattleUnk022767c8.hpp"

#include "Game/UnkStruct_020d43f0.hpp"
#include "Graphics/CellAnimation.hpp"
#include "Graphics/Display.hpp"
#include "Graphics/SpriteManager.hpp"
#include "System/FileSystem.hpp"
#include "System/FrameTask.hpp"
#include "System/InputManager.hpp"
#include "System/SoundManager.hpp"
#include "System/TouchManager.hpp"
#include "Text/TextManager.hpp"
#include "UI/SelectWindow.hpp"
#include <nitro/gx.h>
#include <nitro/mi.h>
#include <nitro/os.h>
#include <stdio.h>

extern u8 data_020d49a1[];

static inline void ClearBit(u8 *flags, int index) {
    flags[index / 8] &= ~(1 << (index % 8));
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
    {20000, 50000},
    {30000, 40000},
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
    {0, 0, 0, 0},
    {0, 0, 0, 0},
    {0, 0, 0, 0},
    {0, -1, 0, 2},
    {0, -1, 0, 3},
    {0, -1, 0, 4},
};

BattleUnk022767c8::BattleUnk022767c8()
    : SceneBase(0x3e8, 0) {
    mWindow     = 0;
    mTextSize   = 0;
    mGlyphCount = 0;
    mTimer      = 0xf0;
    mSlideA     = 0;
    mSlideB     = 0;
    mScroll     = 0;
    mHold       = 0x78;
    mUnk_a4     = 0;
    mState      = 0;
    mUnk_a7     = 0;
    mFade       = 0x40;
    mUnk_a9     = 0;
    mUnk_aa     = 0;
    mUnk_ab     = 0;
    for (u8 i = 0; i < 9; i++) {
        mAnims[i] = 0;
    }
    for (u8 i = 0; i < 15; i++) {
        mAnimsB[i]      = 0;
        mPos[i][0]      = 0;
        mPos[i][1]      = 0;
        mTimers[i][0]   = 0;
        mTimers[i][1]   = 0;
    }
    mMessages    = 0;
    mTextBuffer  = 0;
    mGlyphBuffer = 0;
    MI_CpuClear16(mFrames, 0xc);
    switch (data_020d43f0[0].mUnk_42) {
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
    if (data_020d43f0[0].mUnk_5bf_0) {
        mAmount = sRewards[mKind][1];
    } else {
        mAmount = sRewards[mKind][0];
    }
    mTotal  = data_020d43f0[0].mMoney;
    u32 add = mAmount;
    if (add > 9999990) {
        add = 9999990;
    }
    data_020d43f0[0].mMoney += add;
    if (data_020d43f0[0].mMoney > 9999990) {
        data_020d43f0[0].mMoney = 9999990;
    }
    add = mAmount;
    if (add > 9999990) {
        add = 9999990;
    }
    data_020d43f0[0].mUnk_20 += add;
    if (data_020d43f0[0].mUnk_20 > 9999990) {
        data_020d43f0[0].mUnk_20 = 9999990;
    }
}

BattleUnk022767c8::~BattleUnk022767c8() {
    u8 i;
    CellAnimation *a;
    CellAnimation *b;
    delete mWindow;
    i = 0;
    do {
        a = mAnims[i];
        if (a) {
            delete a;
        }
        i++;
    } while (i < 9);
    u8 j;
    j = 0;
    do {
        b = mAnimsB[j];
        if (b) {
            delete b;
        }
        j++;
    } while (j < 15);
    func_ov007_02278250();
    if (mMessages) {
        OS_Free(mMessages);
    }
    if (mTextBuffer) {
        OS_Free(mTextBuffer);
    }
    if (mGlyphBuffer) {
        OS_Free(mGlyphBuffer);
    }
    FrameTask::Get().func_0203dd90();
}

// non-matching: 6 words, the flag clear in data_020d49a1 and the brightness argument order are scheduled differently
void BattleUnk022767c8::Update() {
    u8 draw = 1;
    switch (mState) {
        case 0:
            func_ov007_02277368();
            func_ov007_02277514();
            mSlideA = 0x108;
            mSlideB = 0x108;
            SpriteManager::Get().StartFade(2);
            mFade = 0x40;
            draw  = 0;
            mState++;
            break;
        case 1:
            SpriteManager::Get();
            if (SpriteManager::sFade.mMain <= 1) {
                FrameTask::Get().func_0203dc34(0, 0x18);
                mUnk_a4 = 1;
                mState++;
            }
            break;
        case 2:
            if (mAnims[1]->mUnk_12 != 0) {
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
                mUnk_a7 = 0x78;
                mUnk_ab = 0x78;
                mState++;
            }
            break;
        case 5:
            mUnk_a7--;
            if (mUnk_a7 <= 0) {
                if (mAmount > 1000) {
                    mUnk_a7 = 10;
                    mAmount -= 1000;
                    mTotal += 1000;
                } else {
                    mTotal += mAmount;
                    mAmount = 0;
                    mWindow->mUnk_064 = draw;
                    mState++;
                }
            }
            mUnk_ab--;
            if (mUnk_ab <= 0) {
                SoundManager::Get().func_020246f8(0x36);
                mUnk_ab = 10;
            }
            if (InputManager::Get().IsHeld(1) | TouchManager::Get().mUnk_57) {
                mTotal += mAmount;
                mAmount = 0;
                mWindow->mUnk_064 = 1;
                if (mHold > 0) {
                    mUnk_a9 = 1;
                    mHold   = 0;
                }
                mState = 7;
            }
            func_ov007_02277f3c();
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
                s8 cool = mUnk_aa;
                if (cool > 0) {
                    mUnk_aa--;
                }
                hit = mUnk_aa > 0;
                do {
                    u32 length = mAnimsB[n]->func_0202a54c();
                    mFrames[k]++;
                    if (mFrames[k] >= length) {
                        mFrames[k] = 0;
                    }
                    if (!hit) {
                        u8 m;
                        u16 frame = mFrames[k];
                        for (m = 0; m < 2; m++) {
                            if (frame == sSparkleFrames[k][m]) {
                                SoundManager::Get().func_020246f8(0x70);
                                mUnk_aa = 30;
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
            if (!data_020d43f0[0].mUnk_5bf_0) {
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
            G2S_SetBlendBrightness(0x1f, -((u32) mFade >> 2));
        }
    }
    if (draw) {
        func_ov007_02277c44();
    }
}

void BattleUnk022767c8::func_ov007_02277368() {
    GX_SetDispSelect(1);
    SpriteManager::Get().func_02026dec(1, 1);
    func_02001e68(1, 0, 0);
    func_02001ed0(0);
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

// non-matching: 2 words, the argument setup of the third /story/ BG load (map_b_bg_00_nsc.bin) is scheduled differently
void BattleUnk022767c8::func_ov007_02277514() {
    ChangeDir("/game/Tournament/Bg/");
    func_02025b64(0, 1, 0, "tournament_u_bg_result_ncl.bin", 0, 0, 0);
    func_02025b64(0, 11, 0, "tournament_u_bg_result_ncg.bin", 0, 0, 0);
    func_02025b64(0, 7, 0, "tournament_u_bg_result_nsc.bin", 0, 0, 0);
    ChangeDir("/story/");
    func_02025b64(1, 1, 0, "map_b_bg_00_ncl.bin", 0, 0, 0);
    func_02025b64(1, 11, 0, "map_b_bg_00_ncg.bin", 0, 0, 0);
    func_02025b64(1, 7, 0, "map_b_bg_00_nsc.bin", 0, 0, 0);
    func_02025b64(1, 1, 0x6000, "map_b_bg_00_ncl.bin", 0, 0, 0);
    func_02025b64(1, 12, 0, "map_b_bg_01_ncg.bin", 0, 0, 0);
    func_02025b64(1, 8, 0, "map_b_bg_01_a_nsc.bin", 0, 0, 0);
    func_02025b64(1, 10, 0, "map_b_bg_01_b_nsc.bin", 0, 0, 0);
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
    mAnims[0]            = new CellAnimation("tournament_u_obj_01_nce.bin", kind, 1, 1);
    mAnims[0]->mCharBase = screen.func_020276f4(0);
    mAnims[0]->mUnk_11   = 0;
    mAnims[0]->mUnk_0f   = mAnims[0]->mFrames[mAnims[0]->mUnk_0e].mUnk_10 - 1;
    for (u8 i = 1; i < 9; i++) {
        mAnims[i]            = new CellAnimation("tournament_u_obj_02_nce.bin", 2, 1, 1);
        mAnims[i]->mCharBase = screen.func_020276f4(0x7000);
        mAnims[i]->mUnk_11   = 6;
    }
    mAnims[1]->SetAnimation(0);
    mAnims[2]->SetAnimation(1);
    SpriteScreen &screen2 = SpriteManager::Get().Screen(1);
    ChangeDir("/Game/2D/union_b/");
    screen2.LoadCharacters("union_b_obj_cockpit.nce_ncg1D.bin", 0, 0);
    screen2.LoadPalette("union_b_obj_cockpit_ncl.bin", 0, 0);
    ChangeDir("/story/");
    screen2.LoadCharacters("map_b_obj_02.nce_ncg1D.bin", 0x2000, 0);
    screen2.LoadPalette("map_b_obj_02_ncl.bin", 0x20, 0);
    for (u8 i = 0; i < 15; i++) {
        mAnimsB[i]            = new CellAnimation("map_b_obj_02_nce.bin", i, 1, 1);
        mAnimsB[i]->mCharBase = screen2.func_020276f4(0x2000);
        mAnimsB[i]->mUnk_11   = 1;
    }
    for (u8 i = 3; i < 9; i++) {
        mAnimsB[i]->func_0202a54c();
        mFrames[i - 3] = 0;
    }
    ChangeDir("/");
    mMessages = (MessageFile *) func_02023504("/font/mes_result.mes", 0, 0, 0, 0, 0);
    mTextSize = TextManager::Get().BufferSize(0x20, 0x20);
    mGlyphCount = TextManager::Get().GlyphCount(0x80);
    mTextBuffer = OS_Alloc(mTextSize);
    mGlyphBuffer = OS_Alloc(mGlyphCount * 0xc);
    TextManager::Get().Init(mGlyphBuffer, 0, 0, 0x20, 0x20, 0x21);
    TextManager::Get().SetColor(0, 0, 15);
    func_ov007_02277f3c();
    delete mWindow;
    mWindow = new SelectWindow(1, sItems, -1);
    mWindow->SetCells(1, sCells);
    mWindow->mCursor->mSelection = -1;
    mWindow->mUnk_064            = 0;
}

void BattleUnk022767c8::func_ov007_02277c44() {
    mAnims[0]->Update();
    mAnims[0]->Draw(0, 0x80, 0x60, 2);
    if (mUnk_a4) {
        mAnims[1]->Update();
        mAnims[1]->Draw(0, 0x80, 0x60, 2);
        mAnims[2]->Update();
        mAnims[2]->Draw(0, (s16) (mSlideA + 0x80), 0x60, 2);
        u32 value = mAmount;
        u8 i      = 0;
        do {
            u8 digit = value % 10;
            value    = value / 10;
            mAnims[i + 3]->func_0202a3ec(0, digit, (s16) (mSlideA + (0xaf + i * -0x13)), 0x80, 2);
            if (value == 0) {
                break;
            }
            i++;
        } while (i < 6);
        SpriteManager::Get().Screen(0).func_02027778(mGlyphBuffer, 8, (s16) (mSlideB + 8), 0xaa, 2, 0x140, 0xf, 0);
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
        mAnimsB[i]->Update();
        mAnimsB[i]->Draw(1, (s16) (mPos[i][0] + 0x80), (s16) (mPos[i][1] + 0x60), 2);
        mAnimsB[i]->Draw(1, (s16) (mPos[i][0] + 0x80), (s16) (mPos[i][1] - 0xa0), 2);
    }
}

// non-matching: the stores to TextManager::mUnk_5b come after the guard load of the following TextManager::Get() in the original
// (known open problem, 8 words)
void BattleUnk022767c8::func_ov007_02277f3c() {
    func_ov007_02278250();
    s16 y;
    s16 x;
    s16 y2;
    s16 x2;
    s16 y3;
    s16 x3;
    x = 0;
    y = 0x9c;
    TextManager::Get().mUnk_5b = 1;
    TextManager::Get().PrintNumber(mTotal, 7, 0, mTextBuffer, 0x20, 2, &y, &x, 0, 2, 0x21, 0, 0, 1, 1);
    TextManager &text = TextManager::Get();
    x2 = x;
    y2 = y;
    text.Print(mMessages->GetText(2), mMessages->GetLength(2), mTextBuffer, 0x20, 2, &y2, &x2, 0, 0, 2, 0x21, 0, 0, 0, 0, 1, 1);
    TextManager &text2 = TextManager::Get();
    x3 = x;
    y3 = 0;
    text2.Print(mMessages->GetText(15), mMessages->GetLength(15), mTextBuffer, 0x20, 2, &y3, &x3, 0, 0, 2, 0x21, 0, 0, 0, 0, 1, 1);
    TextManager::Get().mUnk_5b = 0;
    func_02025c1c(SpriteManager::Get().Screen(0).mBgIndex, 4, 0xa000, mTextBuffer, mTextSize, 0);
}

void BattleUnk022767c8::func_ov007_02278250() {
    if (mTextBuffer) {
        MI_CpuClear16(mTextBuffer, mTextSize);
    }
}
