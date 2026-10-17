#include "Battle/BattleBlend.hpp"

#include "Battle/BattleBase.hpp"

#include <nitro/gx.h>

// Alpha blend level on the sub screen, built up over time and wobbled around its value.
BattleBlend::BattleBlend(u16 period, u8 min, u8 max) {
    mRaiseCount = 0;
    mLevel = min;
    mUnk_06 = 0;
    mWobble = 0;
    mWobbleFrames = 0;
    mFalling = 0;
    mPeriod = period;
    mMin = min;
    mMax = max;
}

BattleBlend::~BattleBlend() {
    G2S_BlendNone();
}

void BattleBlend::RaiseLevel() {
    mUnk_06 = 0;
    mRaiseCount++;
    if (mRaiseCount < mPeriod) {
        return;
    }
    mRaiseCount = 0;
    mLevel++;
    if (mLevel > mMax) {
        mLevel = mMax;
    }
}

void BattleBlend::UpdateWobble() {
    u8 period;
    u8 peak;
    if (mLevel >= mMax - 3) {
        period = 2;
        peak = period;
    } else {
        period = 20;
        peak = 2;
    }
    mWobbleFrames++;
    if (mWobbleFrames >= period) {
        mWobbleFrames = 0;
        if (mFalling == 0) {
            mWobble++;
            if (mWobble >= peak) {
                mFalling ^= 1;
            }
        } else {
            mWobble--;
            if (mWobble == 0) {
                mFalling ^= 1;
            }
        }
    }
    s16 level = mLevel + mWobble;
    if (level > 0x3e) {
        level = 0x3e;
    }
    if (level < 0x1f) {
        reg_G2S_DB_BLDALPHA = level;
    } else {
        s16 rest = level - 0x1f;
        reg_G2S_DB_BLDALPHA = (rest << 8) | 0x1f;
    }
}

BOOL BattleBlend::LowerLevel() {
    BOOL done = 0;
    mUnk_06++;
    if ((int) mUnk_06 >= 1) {
        mUnk_06 = 0;
        mRaiseCount = 0;
        if (mLevel > mMin) {
            mLevel--;
        }
        done = 1;
    }
    return done;
}

void BattleBlend::WriteBlendAlpha() {
    s16 level = mLevel + mWobble;
    if (level < 0x1f) {
        reg_G2S_DB_BLDALPHA = level;
    } else {
        s16 rest = level - 0x1f;
        reg_G2S_DB_BLDALPHA = (rest << 8) | 0x1f;
    }
}
