#include "Battle/BattleBlend.hpp"

#include "Battle/BattleBase.hpp"

#include <nitro/gx.h>

// Alpha blend level on the sub screen, built up over time and wobbled around its value.
BattleBlend::BattleBlend(u16 period, u8 min, u8 max) {
    mUnk_04 = 0;
    mLevel = min;
    mUnk_06 = 0;
    mWobble = 0;
    mUnk_0a = 0;
    mFalling = 0;
    mPeriod = period;
    mMin = min;
    mMax = max;
}

BattleBlend::~BattleBlend() {
    G2S_BlendNone();
}

void BattleBlend::func_ov007_02223d7c() {
    mUnk_06 = 0;
    mUnk_04++;
    if (mUnk_04 < mPeriod) {
        return;
    }
    mUnk_04 = 0;
    mLevel++;
    if (mLevel > mMax) {
        mLevel = mMax;
    }
}

void BattleBlend::func_ov007_02223dc4() {
    u8 period;
    u8 peak;
    if (mLevel >= mMax - 3) {
        period = 2;
        peak = period;
    } else {
        period = 20;
        peak = 2;
    }
    mUnk_0a++;
    if (mUnk_0a >= period) {
        mUnk_0a = 0;
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

BOOL BattleBlend::func_ov007_02223ea0() {
    BOOL done = 0;
    mUnk_06++;
    if ((int) mUnk_06 >= 1) {
        mUnk_06 = 0;
        mUnk_04 = 0;
        if (mLevel > mMin) {
            mLevel--;
        }
        done = 1;
    }
    return done;
}

void BattleBlend::func_ov007_02223ee4() {
    s16 level = mLevel + mWobble;
    if (level < 0x1f) {
        reg_G2S_DB_BLDALPHA = level;
    } else {
        s16 rest = level - 0x1f;
        reg_G2S_DB_BLDALPHA = (rest << 8) | 0x1f;
    }
}
