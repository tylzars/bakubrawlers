#include "Battle/ConfrontEmblem02.hpp"

#include "Battle/BattleUtil.hpp"
#include "Battle/BattleBase.hpp"
#include "Graphics/CellAnimation.hpp"
#include "Graphics/SpriteManager.hpp"
#include "System/FileSystem.hpp"
#include <stdio.h>


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
