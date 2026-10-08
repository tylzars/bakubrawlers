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
    mUnk_07_0 = 0;
    mUnk_07_1 = 0;
    mUnk_07_2 = 0;
    mUnk_07_3 = 0;
    mTimer = 0;
    mPhase = 0;
    mPos = 0;
    mSpeed = 0;
    mAccel = 0;
    SpriteScreen &screen = SpriteManager::Get().Screen(1);
    screen.func_02027624(3);
    ChangeDir("/Game/Confrontation/Obj/");
    sprintf(path, "vs_emblem_%02d_nce.bin", emblem);
    mAnim = new CellAnimation(path, 0, 1, 1);
    mAnim->mCharBase = screen.func_020276f4(0x1c000);
    mAnim->mUnk_11 = 0xb;
    mAnim->SetAnimation(0);
    ChangeDir("/");
}

ConfrontEmblem02::~ConfrontEmblem02() {
    delete mAnim;
}

void ConfrontEmblem02::func_ov007_0221de78(u8 side) {
    mSide = func_ov007_0221d348(2);
    mState = 0;
    mUnk_07_1 = 0;
    mPhase = 0;
    mTimer = 0;
    mAngle = mSide == 0 ? -32.0f : 288.0f;
    mPos = 0;
    mSpeed = 0x148;
    mAccel = 0x148;
    mScale = 1.0f;
    mUnk_07_2 = 0;
    mAnim->func_02029f6c();
    mAnim->SetAnimation(0);
}

void ConfrontEmblem02::func_ov007_0221df04() {
    switch (mState) {
        case 0:
            switch (mPhase) {
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
                        mPhase++;
                    }
                    break;
                case 1:
                    mTimer++;
                    if (mTimer > 5) {
                        mPhase++;
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
            if (mAnim->mUnk_12 == 0) {
                mAnim->Update();
            } else {
                mUnk_07_0 = 0;
                mState = 2;
            }
            break;
        case 2:
            break;
    }
}

void ConfrontEmblem02::func_ov007_0221e054() {
    if (mUnk_07_3) {
        return;
    }
    if (mState == 0) {
        if (mPhase != 2) {
            mPhase = 2;
        }
    }
    mUnk_07_3 = 1;
}

void ConfrontEmblem02::func_ov007_0221e090() {
    switch (mState) {
        case 0:
            {
                float a = mScale > 0.0f ? 0.5f + 4096.0f * mScale : 4096.0f * mScale - 0.5f;
                float b = mScale > 0.0f ? 0.5f + 4096.0f * mScale : 4096.0f * mScale - 0.5f;
                s32 x = (s32) mAngle;
                s32 sy = (s32) b;
                s32 sx = (s32) a;
                mAnim->func_0202a494(1, x, 0x7b, 2, sy, sx, 0);
            }
            break;
        case 1:
            {
                float a = mScale > 0.0f ? 0.5f + 4096.0f * mScale : 4096.0f * mScale - 0.5f;
                float b = mScale > 0.0f ? 0.5f + 4096.0f * mScale : 4096.0f * mScale - 0.5f;
                s32 x = (s32) mAngle;
                s32 sy = (s32) b;
                s32 sx = (s32) a;
                mAnim->func_0202a494(1, x, 0x7b, 2, sy, sx, 0);
            }
            break;
        case 2:
            break;
    }
}

u8 ConfrontEmblem02::func_ov007_0221e250() {
    return (mState == 2) ? 1 : 0;
}

int ConfrontEmblem02::func_ov007_0221e268(u8 flag) {
    if (mState != 0 || mUnk_07_0 == 1) {
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
            mUnk_07_0 = 1;
            mState = 1;
            mAnim->func_02029f6c();
            mAnim->SetAnimation(anim);
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
    mStep  = data_ov007_02278bf8;
    for (u8 i = 0; i < 3; i++) {
        mAnims[i] = 0;
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
        anim = mAnims[i];
        delete anim;
    }
}

void ConfrontBg02::func_ov007_02219888(BattleData *data, u32 mode) {
    mData = data;
    func_ov007_0221a624();
    vfunc_28();
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

void ConfrontBg02::func_ov007_0221a624() {
    func_02001ed0(0);
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

void ConfrontBg02::vfunc_28() {
    char path[0x80];
    BattleData *data = mData;
    u8 spectator = IsSpectator();
    if (spectator) {
        return;
    }
    ChangeDir("/Game/Confrontation/Bg/");
    func_02025b64(1, 1, 0x2000, "vs_b_bg_02_ncl.bin", 0, 0, 0);
    func_02025b64(1, 12, 0, "vs_b_bg_02_ncg.bin", 0, 0, 0);
    func_02025b64(1, 8, 0, "vs_b_bg_02_nsc.bin", 0, 0, 0);
    ChangeDir("/");
    SpriteScreen &screen = SpriteManager::Get().Screen(1);
    screen.func_02027624(3);
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
    mAnims[0] = new CellAnimation(path, 0, 1, 1);
    mAnims[0]->mCharBase = screen.func_020276f4(0x1c000);
    mAnims[0]->mUnk_11 = 12;
    mAnims[0]->SetAnimation(0);
    screen.LoadCharacters("vs_b_obj_02_line.nce_ncg1D.bin", 0x1d000, 0);
    mAnims[2] = new CellAnimation("vs_b_obj_02_line_nce.bin", 0, 1, 1);
    mAnims[2]->mCharBase = screen.func_020276f4(0x1d000);
    mAnims[2]->mUnk_11 = 11;
    mAnims[2]->SetAnimation(0);
    screen.LoadCharacters("vs_b_obj_02_marker.nce_ncg1D.bin", 0x1e000, 0);
    screen.LoadPalette("vs_b_obj_02_marker_ncl.bin", 0x1a0, 0x20);
    mAnims[1] = new CellAnimation("vs_b_obj_02_marker_nce.bin", 0, 1, 1);
    mAnims[1]->mCharBase = screen.func_020276f4(0x1e000);
    mAnims[1]->mUnk_11 = 13;
    mAnims[1]->SetAnimation(0);
    ChangeDir("/");
}

void ConfrontBg02::vfunc_34() {
    func_ov007_0221ee70();
    if (func_ov007_0221eec8()) {
        u8 spectator = IsSpectator();
        if (!spectator) {
            func_ov007_0221ef34();
        } else {
            func_ov007_0221d348(0xffff);
        }
    }
    u8 spectator = IsSpectator();
    if (spectator) {
        return;
    }
    for (u8 i = 0; i < 4; i++) {
        mEmblems[i]->func_ov007_0221df04();
    }
}

BOOL ConfrontBg02::func_ov007_0221bfa4() {
    u8 spectator = IsSpectator();
    if (spectator) {
        return FALSE;
    }
    for (u8 i = 0; i < 4; i++) {
        mEmblems[i]->func_ov007_0221e054();
        mEmblems[i]->func_ov007_0221df04();
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
            mEmblems[i]->func_ov007_0221e090();
        }
    }
    if (mAnims[1]->mUnk_0e == 1) {
        if (mAnims[1]->mUnk_12 == 0) {
            mAnims[1]->Update();
        } else {
            mAnims[1]->func_02029f6c();
            mAnims[1]->SetAnimation(0);
        }
    }
    mAnims[1]->Draw(1, 0x69, 0x64, 2);
    for (u8 i = 0; i < 4; i++) {
        if (mEmblems[i]->mUnk_07_1) {
            mEmblems[i]->func_ov007_0221e090();
        }
    }
    mAnims[0]->Draw(1, 0x80, 0x7b, 2);
    if (mAnims[2]->mUnk_12 == 0) {
        mAnims[2]->Update();
        mAnims[2]->Draw(1, 0x80, 0x7b, 2);
    }
}

u8 ConfrontBg02::vfunc_40(int a) {
    mSide = a;
    s32 result = -1;
    for (u8 i = 0; i < 4; i++) {
        if (mEmblems[i]->func_ov007_0221e268(1) == 100) {
            mAnims[1]->func_02029f6c();
            mAnims[1]->SetAnimation(1);
            break;
        }
    }
    if (TouchManager::Get().mUnk_57 == 0) {
        return 0;
    }
    if (func_ov007_022252d0((s16) TouchManager::Get().mX, (s16) TouchManager::Get().mY, 0x80, 0x7b) < 0x10) {
        func_ov007_0221bb3c();
        for (u8 i = 0; i < 4; i++) {
            result = mEmblems[i]->func_ov007_0221e268(0);
            if (result != -1) {
                break;
            }
        }
        if (result != -1) {
            if (result == 100) {
                mAnims[2]->func_02029f6c();
                mAnims[2]->SetAnimation(1);
                SoundManager::Get().func_020246f8(0x28);
            } else {
                SoundManager::Get().func_020246f8(0x29);
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

void ConfrontBg02::func_ov007_0221ee70() {
    u8 i = 0;
    u16 total = 0;
    ConfrontEmblemStep *step = data_ov007_02278bf8;
    for (; i < 5; i++) {
        total += step->mUntil;
        if (mUnk_16 < total) {
            if (mStep != step) {
                mStep = step;
            }
            return;
        }
        step++;
    }
}

BOOL ConfrontBg02::func_ov007_0221eec8() {
    BOOL result = FALSE;
    if (mUnk_20->mUnk_02 - mUnk_16 >= 10) {
        if (mDelay != 0) {
            mDelay--;
        } else {
            mDelay = func_ov007_0221d348(mStep->mMaxDelay - mStep->mMinDelay);
            mDelay += mStep->mMinDelay;
            result = TRUE;
        }
    }
    return result;
}

void ConfrontBg02::func_ov007_0221ef34() {
    BOOL found = FALSE;
    for (u8 i = 0; i < 4; i++) {
        if (mEmblems[i]->func_ov007_0221e250()) {
            mEmblems[i]->func_ov007_0221de78(mSide);
            found = TRUE;
            break;
        }
    }
    if (!found) {
        func_ov007_0221d348(0xffff);
    }
}

BOOL ConfrontBg02::func_ov007_0221bd84() {
    return FALSE;
}
