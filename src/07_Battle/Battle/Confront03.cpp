#include "System/TouchManager.hpp"
#include "System/SoundManager.hpp"
#include "Battle/Confront03.hpp"
#include "Battle/BattleUtil.hpp"
#include "Graphics/Display.hpp"
#include "Graphics/SpriteManager.hpp"
#include "System/FileSystem.hpp"
#include "System/SoundManager.hpp"
#include "System/TouchManager.hpp"
#include <nitro/gx.h>
#include <stdio.h>
#include "Battle/ConfrontEmblem03.hpp"
#include "Battle/BattleBase.hpp"

ConfrontEmblem03::ConfrontEmblem03(u8 index) {
    char path[0x80];
    mState = 2;
    mEmblem = index;
    mTouched = 0;
    mUnk_07_1 = 0;
    mShrinkStarted = 0;
    mHoldFrames = 0;
    mHoldFrameCount = 0;
    mStep = 0;
    mSlot = -1;
    SpriteScreen &screen = SpriteManager::Get().Screen(1);
    screen.SetObjVramMode(3);
    ChangeDir("/Game/Confrontation/Obj/");
    sprintf(path, "vs_emblem_%02d_nce.bin", index);
    mCellAnim = new CellAnimation(path, 0, 1, 1);
    mCellAnim->mCharBase = screen.GetCharBase(0x1c000);
    mCellAnim->mPaletteBase = 11;
    mCellAnim->SetAnimation(0);
    ChangeDir("/");
}

ConfrontEmblem03::~ConfrontEmblem03() {
    delete mCellAnim;
}

void ConfrontEmblem03::Start(u8 slot, int flag) {
    mX = (float) ((slot % 4) * 64 + 0x10 + BattleRand32(0x20));
    mY = (float) ((slot / 4) * 32 + 0x68 + BattleRand32(2));
    mState = 0;
    mStep = 0;
    mUnk_07_1 = 0;
    mSlot = slot;
    mHoldFrames = 0;
    if (flag == 0) {
        mHoldFrameCount = 0x14;
    } else {
        mHoldFrameCount = 0x50;
    }
    mScale = 0.1f;
    mCellAnim->ResetAnimation();
    mCellAnim->SetAnimation(0);
}

void ConfrontEmblem03::Update() {
    switch (mState) {
        case 0:
            switch (mStep) {
                case 0:
                    mScale += 0.05f;
                    if (mScale >= 1.0f) {
                        mScale = 1.0f;
                        mStep++;
                    }
                    break;
                case 1:
                    mHoldFrames++;
                    if (mHoldFrames > mHoldFrameCount) {
                        mStep++;
                    }
                    break;
                case 2:
                    mScale = mScale - 0.05f;
                    if (mScale <= 0.1f) {
                        mState = 2;
                    }
                    break;
            }
            break;
        case 1:
            if (mCellAnim->mFinished == 0) {
                mCellAnim->Update();
            } else {
                mTouched = 0;
                mState = 2;
            }
            break;
        case 2:
            break;
    }
}

void ConfrontEmblem03::StartShrink() {
    if (mShrinkStarted) {
        return;
    }
    if (mState == 0) {
        if (mStep != 2) {
            mStep = 2;
        }
    }
    mShrinkStarted = 1;
}

void ConfrontEmblem03::Draw() {
    switch (mState) {
        case 0: {
            float a;
            if (mScale > 0.0f) {
                a = 0.5f + 4096.0f * mScale;
            } else {
                a = 4096.0f * mScale - 0.5f;
            }
            float b;
            if (mScale > 0.0f) {
                b = 0.5f + 4096.0f * mScale;
            } else {
                b = 4096.0f * mScale - 0.5f;
            }
            mCellAnim->DrawTransformed(1, (s32) mX, (s32) mY, 2, (s32) b, (s32) a, 0);
            break;
        }
        case 1: {
            float a;
            if (mScale > 0.0f) {
                a = 0.5f + 4096.0f * mScale;
            } else {
                a = 4096.0f * mScale - 0.5f;
            }
            float b;
            if (mScale > 0.0f) {
                b = 0.5f + 4096.0f * mScale;
            } else {
                b = 4096.0f * mScale - 0.5f;
            }
            mCellAnim->DrawTransformed(1, (s32) mX, (s32) mY, 2, (s32) b, (s32) a, 0);
            break;
        }
        case 2:
            break;
    }
}

u8 ConfrontEmblem03::IsIdle() {
    return mState == 2 ? 1 : 0;
}

int ConfrontEmblem03::func_ov007_0221f594() {
    if (mState != 0 || mTouched == 1) {
        return -1;
    }
    if (PointDistance((s16) TouchManager::Get().mX, (s16) TouchManager::Get().mY, (s32) mX, (s32) mY) > 0x18) {
        return -1;
    }
    u8 anim;
    s16 result;
    if (mStep <= 1) {
        anim = 1;
        result = 100;
    } else {
        anim = 2;
        result = (s16) (s32) (100.0f * mScale) / 2;
        mUnk_07_1 = 1;
    }
    if (result != -1) {
        mTouched = 1;
        mState = 1;
        mCellAnim->ResetAnimation();
        mCellAnim->SetAnimation(anim);
    }
    return result;
}

int ConfrontEmblem03::GetSlotIfIdle() {
    if (mState == 2) {
        s8 v = mSlot;
        if (v != -1) {
            return v;
        }
    }
    return -1;
}

extern ConfrontRow gConfrontRows[];

Confront03::Confront03()
    : BattleBase() {
    mPenFrames = 0;
    mPenEmblemIndex = 0;
    mLongHold = 0;
    mUsedSlotMask = 1;
    mDelay = 0;
    mData   = 0;
    mConfrontRow = gConfrontRows;
    for (u8 i = 0; i < 4; i++) {
        mEmblems[i] = 0;
    }
}

Confront03::~Confront03() {
    for (u8 i = 0; i < 4; i++) {
        delete mEmblems[i];
    }
}

void Confront03::Init(BattleData *data, u32 mode) {
    mData = data;
    InitSubScreen();
    LoadGraphics();
    u8 side = mUnk_12;
    BOOL spectator = side >= 2;
    if ((u8) spectator) {
        return;
    }
    u8 nibble = side == 0 ? mData->mEntries[0].mUnk_0d_0 : mData->mEntries[1].mUnk_0d_0;
    for (u8 i = 0; i < 4; i++) {
        mEmblems[i] = new ConfrontEmblem03(nibble);
    }
}

void Confront03::InitSubScreen() {
    GXS_SetGraphicsMode(0);
    G2S_SetBG0Control(0, 1, 0, 1, 0);
    G2S_SetBG1Control(0, 1, 2, 4, 0);
    G2S_SetBG2Control(0, 0, 4, 5, 0);
    G2S_SetBG3Control(0, 1, 6, 6, 0);
    G2S_SetBG0Priority(3);
    G2S_SetBG1Priority(3);
    G2S_SetBG2Priority(3);
    G2S_SetBG3Priority(3);
    G2S_BlendNone();
}

void Confront03::LoadGraphics() {
    BOOL spectator = IsSpectator();
    if ((u8) spectator) {
        return;
    }
    BattleData *data = mData;
    SpriteScreen &screen = SpriteManager::Get().Screen(1);
    screen.SetObjVramMode(3);
    ChangeDir("/Game/Confrontation/Obj/");
    char path[0x80];
    u8 nibble = mUnk_12 == 0 ? data->mEntries[0].mUnk_0d_0 : data->mEntries[1].mUnk_0d_0;
    sprintf(path, "vs_emblem_%02d.nce_ncg1D.bin", nibble);
    screen.LoadCharacters(path, 0x1c000, 0);
    sprintf(path, "vs_emblem_%02d_ncl.bin", nibble);
    screen.LoadPalette(path, 0x160, 0x40);
    ChangeDir("/");
}

void Confront03::vfunc_34() {
    if (CountDownDelay()) {
        u8 count = BattleRand32(mConfrontRow->mUnk_04);
        u8 i = 0;
        do {
            BOOL spectator = IsSpectator();
            if (!(u8) spectator) {
                StartIdleEmblem();
            } else {
                BattleRand32(0xffff);
                BattleRand32(0xffff);
                BattleRand32(0xffff);
            }
            i++;
        } while (i < count);
    }
    BOOL spectator = IsSpectator();
    if ((u8) spectator) {
        return;
    }
    u8 j = 0;
    do {
        mEmblems[j]->Update();
        j++;
    } while (j < 4);
}

BOOL Confront03::func_ov007_0221bfa4() {
    BOOL spectator = IsSpectator();
    if ((u8) spectator) {
        return FALSE;
    }
    u8 i = 0;
    do {
        mEmblems[i]->StartShrink();
        mEmblems[i]->Update();
        i++;
    } while (i < 4);
    return FALSE;
}

void Confront03::vfunc_3c() {
    BOOL spectator = IsSpectator();
    if ((u8) spectator) {
        return;
    }
    u8 i = 0;
    do {
        if (!mEmblems[i]->mUnk_07_1) {
            mEmblems[i]->Draw();
        }
        i++;
    } while (i < 4);
    i = 0;
    do {
        if (mEmblems[i]->mUnk_07_1) {
            mEmblems[i]->Draw();
        }
        i++;
    } while (i < 4);
}

u8 Confront03::UpdateTouch(int a) {
    ConfrontEmblem03 *emblem = mEmblems[mPenEmblemIndex];
    if (emblem->mState == 0 && emblem->mStep == 1) {
        mPenFrames++;
        if (mPenFrames >= 0x14) {
            mPenFrames = 0;
            SetPenPosition(-1, -1);
            mPenEmblemIndex++;
            if (mPenEmblemIndex >= 4) {
                mPenEmblemIndex = 0;
            }
        } else {
            ConfrontEmblem03 *cur = mEmblems[mPenEmblemIndex];
            s16 y = (s16) cur->mY;
            s16 x = (s16) cur->mX;
            SetPenPosition(x, y);
        }
    }
    mLongHold = a;
    if (TouchManager::Get().mPressed == 0) {
        return 0;
    }
    int sound = -1;
    int result;
    u8 i = 0;
    do {
        result = mEmblems[i]->func_ov007_0221f594();
        if (result != -1) {
            func_ov007_0221bb3c();
            sound = result == 100 ? 0x28 : 0x29;
            break;
        }
        i++;
    } while (i < 4);
    if (result != -1) {
        SoundManager::Get().StartSeqArc(sound);
    }
    if (result == -1) {
        return 0;
    }
    return result;
}

void Confront03::vfunc_44(int index, u16 count) {
}

BOOL Confront03::CountDownDelay() {
    BOOL result = FALSE;
    if (mParam->mBattleFrameCount - mElapsedFrames >= 0x28) {
        if (mDelay != 0) {
            mDelay--;
        } else {
            mDelay = BattleRand32(mConfrontRow->mMaxDelay - mConfrontRow->mMinDelay);
            mDelay += mConfrontRow->mMinDelay;
            result = TRUE;
        }
    }
    return result;
}

void Confront03::StartIdleEmblem() {
    u8 found = 0;
    u8 i = 0;
    do {
        int v = mEmblems[i]->GetSlotIfIdle();
        if (v != -1) {
            mUsedSlotMask = mUsedSlotMask & ~(u16) (1 << (u16) v);
        }
        i++;
    } while (i < 4);
    u8 free[12];
    u8 n = 0;
    u16 bit = 1;
    u16 mask = mUsedSlotMask;
    for (u8 k = 0; k < 12; k++) {
        if (!(mask & bit)) {
            free[n] = k;
            n++;
        }
        bit = bit << 1;
    }
    if (n != 0) {
        u8 j = 0;
        do {
            if (mEmblems[j]->IsIdle()) {
                int pick = free[BattleRand32(n) & 0xff];
                if (!mUnk_18_1 && (u16) (pick + 0xffff) <= 1) {
                    BattleRand32(0xffff);
                    BattleRand32(0xffff);
                } else {
                    mEmblems[j]->Start(pick, mLongHold);
                    mUsedSlotMask = mUsedSlotMask | (u16) (1 << pick);
                }
                found = 1;
                break;
            }
            j++;
        } while (j < 4);
    }
    if (found) {
        return;
    }
    BattleRand32(0xffff);
    BattleRand32(0xffff);
    BattleRand32(0xffff);
}

BOOL Confront03::func_ov007_0221bd84() {
    return FALSE;
}
