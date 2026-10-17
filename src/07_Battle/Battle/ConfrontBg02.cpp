#include "System/TouchManager.hpp"
#include "System/SoundManager.hpp"
#include "Battle/ConfrontBg02.hpp"
#include "Battle/BattleUtil.hpp"
#include "Battle/ConfrontEmblem02.hpp"
#include "Graphics/CellAnimation.hpp"
#include "Graphics/Display.hpp"
#include "Graphics/SpriteManager.hpp"
#include "System/FileSystem.hpp"
#include "System/SoundManager.hpp"
#include "System/TouchManager.hpp"
#include <nitro/gx.h>
#include <stdio.h>
#include "Battle/BattleBase.hpp"

ConfrontEmblem02::ConfrontEmblem02(u8 emblem) {
    char path[0x80];
    mState = 2;
    mSide = 0;
    mEmblem = emblem;
    mTouched = 0;
    mUnk_07_1 = 0;
    mUnk_07_2 = 0;
    mShrinkStarted = 0;
    mStepFrames = 0;
    mStep = 0;
    mPos = 0;
    mSpeed = 0;
    mAccel = 0;
    SpriteScreen &screen = SpriteManager::Get().Screen(1);
    screen.SetObjVramMode(3);
    ChangeDir("/Game/Confrontation/Obj/");
    sprintf(path, "vs_emblem_%02d_nce.bin", emblem);
    mCellAnim = new CellAnimation(path, 0, 1, 1);
    mCellAnim->mCharBase = screen.GetCharBase(0x1c000);
    mCellAnim->mPaletteBase = 0xb;
    mCellAnim->SetAnimation(0);
    ChangeDir("/");
}

ConfrontEmblem02::~ConfrontEmblem02() {
    delete mCellAnim;
}

void ConfrontEmblem02::Start(u8 side) {
    mSide = BattleRand32(2);
    mState = 0;
    mUnk_07_1 = 0;
    mStep = 0;
    mStepFrames = 0;
    mAngle = mSide == 0 ? -32.0f : 288.0f;
    mPos = 0;
    mSpeed = 0x148;
    mAccel = 0x148;
    mScale = 1.0f;
    mUnk_07_2 = 0;
    mCellAnim->ResetAnimation();
    mCellAnim->SetAnimation(0);
}

void ConfrontEmblem02::Update() {
    switch (mState) {
        case 0:
            switch (mStep) {
                case 0:
                    if (mSide == 0) {
                        mAngle = mAngle + (float) mSpeed / 4096.0f;
                    } else {
                        mAngle = mAngle - (float) mSpeed / 4096.0f;
                    }
                    mSpeed += mAccel;
                    mPos += mSpeed;
                    if (mPos >= 0xa0000) {
                        mAngle = 128.0f;
                        mStep++;
                    }
                    break;
                case 1:
                    mStepFrames++;
                    if (mStepFrames > 5) {
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

void ConfrontEmblem02::StartShrink() {
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

void ConfrontEmblem02::Draw() {
    switch (mState) {
        case 0:
            {
                float a = mScale > 0.0f ? 0.5f + 4096.0f * mScale : 4096.0f * mScale - 0.5f;
                float b = mScale > 0.0f ? 0.5f + 4096.0f * mScale : 4096.0f * mScale - 0.5f;
                s32 x = (s32) mAngle;
                s32 sy = (s32) b;
                s32 sx = (s32) a;
                mCellAnim->DrawTransformed(1, x, 0x7b, 2, sy, sx, 0);
            }
            break;
        case 1:
            {
                float a = mScale > 0.0f ? 0.5f + 4096.0f * mScale : 4096.0f * mScale - 0.5f;
                float b = mScale > 0.0f ? 0.5f + 4096.0f * mScale : 4096.0f * mScale - 0.5f;
                s32 x = (s32) mAngle;
                s32 sy = (s32) b;
                s32 sx = (s32) a;
                mCellAnim->DrawTransformed(1, x, 0x7b, 2, sy, sx, 0);
            }
            break;
        case 2:
            break;
    }
}

u8 ConfrontEmblem02::IsIdle() {
    return (mState == 2) ? 1 : 0;
}

int ConfrontEmblem02::func_ov007_0221e268(u8 flag) {
    if (mState != 0 || mTouched == 1) {
        return -1;
    }
    int anim = 2;
    s16 result = -1;
    if ((u32) mAngle != 0x80) {
        s16 diff;
        if ((u32) mAngle > 0x80) {
            diff = (int) mAngle - 0x80;
        } else {
            diff = 0x80 - (int) mAngle;
        }
        if (diff < 0x1c) {
            if (flag == 0) {
                mUnk_07_1 = 1;
            }
            result = 0x1e;
        }
    } else {
        result = (int) (100.0f * mScale);
        if (result == 100) {
            anim = 1;
        } else {
            result = result / 2;
            if (flag == 0) {
                mUnk_07_1 = 1;
            }
        }
    }
    if (flag == 0) {
        if (result != -1) {
            mTouched = 1;
            mState = 1;
            mCellAnim->ResetAnimation();
            mCellAnim->SetAnimation(anim);
        }
    } else if (result == 100) {
        if (mUnk_07_2) {
            result = -1;
        }
        mUnk_07_2 = 1;
    }
    return result;
}

ConfrontBg02::ConfrontBg02()
    : BattleBase() {
    mDelay = 0;
    mSide  = 0;
    mData  = 0;
    mEmblemStep = gConfrontEmblemSteps;
    for (u8 i = 0; i < 3; i++) {
        mCellAnims[i] = 0;
    }
    for (u8 i = 0; i < 4; i++) {
        mEmblems[i] = 0;
    }
}

ConfrontBg02::~ConfrontBg02() {
    for (u8 i = 0; i < 4; i++) {
        delete mEmblems[i];
    }
    CellAnimation *anim;
    for (u8 i = 0; i < 3; i++) {
        anim = mCellAnims[i];
        delete anim;
    }
}

void ConfrontBg02::Init(BattleData *data, u32 mode) {
    mData = data;
    InitSubScreen();
    LoadGraphics();
    u8 side = mUnk_12;
    BOOL spectator = side >= 2;
    if ((u8) spectator) {
        return;
    }
    u8 emblem;
    if (side == 0) {
        emblem = mData->mEntries[0].mUnk_0d_0;
    } else {
        emblem = mData->mEntries[1].mUnk_0d_0;
    }
    for (u8 i = 0; i < 4; i++) {
        mEmblems[i] = new ConfrontEmblem02(emblem);
    }
}

void ConfrontBg02::InitSubScreen() {
    GXS_SetGraphicsMode(0);
    G2S_SetBG0Control(0, 1, 0, 1, 0);
    G2S_SetBG1Control(0, 1, 2, 4, 0);
    G2S_SetBG2Control(0, 0, 4, 5, 0);
    G2S_SetBG3Control(0, 1, 6, 6, 0);
    G2S_SetBG0Priority(3);
    G2S_SetBG1Priority(2);
    G2S_SetBG2Priority(3);
    G2S_SetBG3Priority(3);
    G2S_BlendNone();
}

void ConfrontBg02::LoadGraphics() {
    char path[0x80];
    BattleData *data = mData;
    u8 spectator = IsSpectator();
    if (spectator) {
        return;
    }
    ChangeDir("/Game/Confrontation/Bg/");
    LoadAndQueueUpload(1, UPLOAD_KIND_BG_EXT_PLTT, 0x2000, "vs_b_bg_02_ncl.bin", 0, 0, 0);
    LoadAndQueueUpload(1, UPLOAD_KIND_BG1_CHAR, 0, "vs_b_bg_02_ncg.bin", 0, 0, 0);
    LoadAndQueueUpload(1, UPLOAD_KIND_BG1_SCR, 0, "vs_b_bg_02_nsc.bin", 0, 0, 0);
    ChangeDir("/");
    SpriteScreen &screen = SpriteManager::Get().Screen(1);
    screen.SetObjVramMode(3);
    ChangeDir("/Game/Confrontation/Obj/");
    u8 emblem;
    if (mUnk_12 == 0) {
        emblem = data->mEntries[0].mUnk_0d_0;
    } else {
        emblem = data->mEntries[1].mUnk_0d_0;
    }
    sprintf(path, "vs_emblem_%02d.nce_ncg1D.bin", emblem);
    screen.LoadCharacters(path, 0x1c000, 0);
    sprintf(path, "vs_emblem_%02d_ncl.bin", emblem);
    screen.LoadPalette(path, 0x160, 0x40);
    sprintf(path, "vs_emblem_%02d_nce.bin", emblem);
    mCellAnims[0] = new CellAnimation(path, 0, 1, 1);
    mCellAnims[0]->mCharBase = screen.GetCharBase(0x1c000);
    mCellAnims[0]->mPaletteBase = 12;
    mCellAnims[0]->SetAnimation(0);
    screen.LoadCharacters("vs_b_obj_02_line.nce_ncg1D.bin", 0x1d000, 0);
    mCellAnims[2] = new CellAnimation("vs_b_obj_02_line_nce.bin", 0, 1, 1);
    mCellAnims[2]->mCharBase = screen.GetCharBase(0x1d000);
    mCellAnims[2]->mPaletteBase = 11;
    mCellAnims[2]->SetAnimation(0);
    screen.LoadCharacters("vs_b_obj_02_marker.nce_ncg1D.bin", 0x1e000, 0);
    screen.LoadPalette("vs_b_obj_02_marker_ncl.bin", 0x1a0, 0x20);
    mCellAnims[1] = new CellAnimation("vs_b_obj_02_marker_nce.bin", 0, 1, 1);
    mCellAnims[1]->mCharBase = screen.GetCharBase(0x1e000);
    mCellAnims[1]->mPaletteBase = 13;
    mCellAnims[1]->SetAnimation(0);
    ChangeDir("/");
}

void ConfrontBg02::vfunc_34() {
    UpdateEmblemStep();
    if (CountDownDelay()) {
        u8 spectator = IsSpectator();
        if (!spectator) {
            StartIdleEmblem();
        } else {
            BattleRand32(0xffff);
        }
    }
    u8 spectator = IsSpectator();
    if (spectator) {
        return;
    }
    for (u8 i = 0; i < 4; i++) {
        mEmblems[i]->Update();
    }
}

BOOL ConfrontBg02::func_ov007_0221bfa4() {
    u8 spectator = IsSpectator();
    if (spectator) {
        return FALSE;
    }
    for (u8 i = 0; i < 4; i++) {
        mEmblems[i]->StartShrink();
        mEmblems[i]->Update();
    }
    return FALSE;
}

void ConfrontBg02::vfunc_3c() {
    u8 spectator = IsSpectator();
    if (spectator) {
        return;
    }
    for (u8 i = 0; i < 4; i++) {
        if (!mEmblems[i]->mUnk_07_1) {
            mEmblems[i]->Draw();
        }
    }
    if (mCellAnims[1]->mAnimIndex == 1) {
        if (mCellAnims[1]->mFinished == 0) {
            mCellAnims[1]->Update();
        } else {
            mCellAnims[1]->ResetAnimation();
            mCellAnims[1]->SetAnimation(0);
        }
    }
    mCellAnims[1]->Draw(1, 0x69, 0x64, 2);
    for (u8 i = 0; i < 4; i++) {
        if (mEmblems[i]->mUnk_07_1) {
            mEmblems[i]->Draw();
        }
    }
    mCellAnims[0]->Draw(1, 0x80, 0x7b, 2);
    if (mCellAnims[2]->mFinished == 0) {
        mCellAnims[2]->Update();
        mCellAnims[2]->Draw(1, 0x80, 0x7b, 2);
    }
}

u8 ConfrontBg02::UpdateTouch(int a) {
    mSide = a;
    s32 result = -1;
    for (u8 i = 0; i < 4; i++) {
        if (mEmblems[i]->func_ov007_0221e268(1) == 100) {
            mCellAnims[1]->ResetAnimation();
            mCellAnims[1]->SetAnimation(1);
            break;
        }
    }
    if (TouchManager::Get().mPressed == 0) {
        return 0;
    }
    if (PointDistance((s16) TouchManager::Get().mX, (s16) TouchManager::Get().mY, 0x80, 0x7b) < 0x10) {
        func_ov007_0221bb3c();
        for (u8 i = 0; i < 4; i++) {
            result = mEmblems[i]->func_ov007_0221e268(0);
            if (result != -1) {
                break;
            }
        }
        if (result != -1) {
            if (result == 100) {
                mCellAnims[2]->ResetAnimation();
                mCellAnims[2]->SetAnimation(1);
                SoundManager::Get().StartSeqArc(0x28);
            } else {
                SoundManager::Get().StartSeqArc(0x29);
            }
        }
    }
    if (result == -1) {
        return 0;
    }
    if (a) {
        result = (s16) (result + 0x14);
    }
    return result;
}

void ConfrontBg02::vfunc_44(int index, u16 count) {
}

void ConfrontBg02::UpdateEmblemStep() {
    u8 i = 0;
    u16 total = 0;
    ConfrontEmblemStep *step = gConfrontEmblemSteps;
    for (; i < 5; i++) {
        total += step->mUntil;
        if (mElapsedFrames < total) {
            if (mEmblemStep != step) {
                mEmblemStep = step;
            }
            return;
        }
        step++;
    }
}

BOOL ConfrontBg02::CountDownDelay() {
    BOOL result = FALSE;
    if (mParam->mBattleFrameCount - mElapsedFrames >= 10) {
        if (mDelay != 0) {
            mDelay--;
        } else {
            mDelay = BattleRand32(mEmblemStep->mMaxDelay - mEmblemStep->mMinDelay);
            mDelay += mEmblemStep->mMinDelay;
            result = TRUE;
        }
    }
    return result;
}

void ConfrontBg02::StartIdleEmblem() {
    BOOL found = FALSE;
    for (u8 i = 0; i < 4; i++) {
        if (mEmblems[i]->IsIdle()) {
            mEmblems[i]->Start(mSide);
            found = TRUE;
            break;
        }
    }
    if (!found) {
        BattleRand32(0xffff);
    }
}

BOOL ConfrontBg02::func_ov007_0221bd84() {
    return FALSE;
}
