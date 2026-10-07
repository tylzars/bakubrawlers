#include "System/TouchManager.hpp"

#include <nitro/fx.h>
#include <nitro/mi.h>

extern "C" {
void func_0200c104();
int func_02007164();
void func_020070f8();
void func_0200c338();
int func_0200c3cc(void *raw);
void func_0200c608(TouchSample *out, void *raw);
BOOL func_0200c17c(void *panel);
void func_0200c210(void *panel);
u16 func_020017d4(fx32 a, fx32 b);
}

void TouchManager::Init() {
    MI_CpuClear32(&mCur, sizeof(mCur));
    MI_CpuClear32(&mPrev, sizeof(mPrev));
    MI_CpuClear32(&pad_0x10, 0x18);
    MI_CpuClear32(&mPanel, sizeof(mPanel));
    MI_CpuClear32(&pad_0x44, 0xc);
    mX = 0;
    mY = 0;
    mUnk_38 = 0;
    mUnk_3c = 0;
    mDistance = 0;
    mUnk_50 = 0;
    mRepeatDelay = 0;
    mRepeatTimer = 0;
    mUnk_56 = 0;
    mUnk_57 = 0;
    mUnk_58 = 0;
    mUnk_59 = 0;
    mTouching = 0;
    mCountdown = 0;
    mCountdown2 = 0;
    mCountdownStart = 0;
    mCountdown2Start = 0;
    mUnk_5f = 0;
    mUnk_60 = 0;
    mEnabled = 0;
    mRepeatDelay = 30;
    mCountdownStart = mCountdown = 12;
    mCountdown2Start = mCountdown2 = 2;
    mUnk_50 = 0x1240 - 12;
}

void TouchManager::func_020225d0() {
    func_0200c104();
    Init();
}

void TouchManager::Update(BOOL enabled) {
    TouchSample sample;
    u32 raw[2];

    if (mEnabled == 0) {
        return;
    }
    if (func_02007164() != 0) {
        func_020070f8();
    }
    do {
        func_0200c338();
    } while (func_0200c3cc(raw) != 0);
    func_0200c608(&sample, raw);
    if (enabled == 0) {
        sample.touch = 0;
        sample.valid = 3;
    }
    mPrev.x = mCur.x;
    mPrev.y = mCur.y;
    mPrev.touch = mCur.touch;
    mPrev.valid = mCur.valid;
    if (sample.valid != 0) {
        sample.valid = 0;
        sample.touch = 0;
        sample.x = mPrev.x;
        sample.y = mPrev.y;
    }
    mCur.x = sample.x;
    mCur.y = sample.y;
    mCur.touch = sample.touch;
    mCur.valid = sample.valid;
    mTouching = 0;
    mUnk_56 = 0;
    mUnk_57 = 0;
    mUnk_58 = 0;
    if (sample.touch != 0) {
        mX = sample.x;
        mY = sample.y;
        mUnk_56 = 1;
        if (mPrev.touch == 0) {
            mUnk_57 = 1;
            mUnk_58 = 1;
            mCountdown = mCountdownStart;
            mCountdown2 = mCountdown2Start;
        } else if (mCountdown < 0) {
            mCountdown2--;
            if (mCountdown2 < 0) {
                mUnk_58 = 1;
                mCountdown2 = mCountdown2Start;
            }
        } else {
            mCountdown--;
            if (mCountdown < 0) {
                mUnk_58 = 1;
                mCountdown2 = mCountdown2Start;
            }
        }
    } else if (mPrev.touch != 0) {
        mTouching = 1;
    }
    func_020228f0();
    func_02022988();
}

BOOL TouchManager::HitTest(s32 x, s32 y, s32 width, s16 height) {
    BOOL inside = TRUE;
    if (width >= 0) {
        if (!(x + width >= (s16) mX && (s16) mX >= x)) {
            inside = FALSE;
        }
    } else {
        if (!(x + width <= (s16) mX && (s16) mX <= x)) {
            inside = FALSE;
        }
    }
    if (height >= 0) {
        if (!(y + height >= (s16) mY && (s16) mY >= y)) {
            inside = FALSE;
        }
    } else {
        if (!(y + height <= (s16) mY && (s16) mY <= y)) {
            inside = FALSE;
        }
    }
    return inside;
}

BOOL TouchManager::func_02022830(s32 x, s32 y, s32 width, s16 height) {
    if (mUnk_56 != 0) {
        if (HitTest(x, y, width, height)) {
            return TRUE;
        }
    }
    return FALSE;
}

BOOL TouchManager::func_02022860(s32 x, s32 y, s32 width, s16 height) {
    if (mUnk_57 != 0) {
        if (HitTest(x, y, width, height)) {
            return TRUE;
        }
    }
    return FALSE;
}

BOOL TouchManager::func_02022890(s32 x, s32 y, s32 width, s16 height) {
    if (mUnk_58 != 0) {
        if (HitTest(x, y, width, height)) {
            return TRUE;
        }
    }
    return FALSE;
}

BOOL TouchManager::func_020228c0(u32 x, s16 y, u32 width, s16 height) {
    if (mTouching != 0) {
        if (HitTest(x, y, width, height)) {
            return TRUE;
        }
    }
    return FALSE;
}

void TouchManager::func_020228f0() {
    mUnk_59 = 0;
    if (mUnk_57 != 0) {
        if (mUnk_5f == 0) {
            mRepeatTimer = mRepeatDelay;
        }
    }
    mRepeatTimer--;
    if (mRepeatTimer > 0) {
        if (mUnk_57 != 0) {
            mUnk_5f++;
            return;
        }
        if (mUnk_56 != 0) {
            return;
        }
        if (mUnk_5f >= 2) {
            mUnk_59 = 1;
            mRepeatTimer = 0;
            mUnk_5f = 0;
        }
    } else {
        mRepeatTimer = 0;
        mUnk_5f = 0;
    }
}

void TouchManager::func_02022988() {
    s32 dy = mY - mPrev.y;
    s32 dx = mX - mPrev.x;
    fx32 fy = dy << 12;
    fx32 fx = dx << 12;
    mDistance = FX_Sqrt(FX_Mul(fy, fy) + FX_Mul(fx, fx)) >> 12;
    u32 a;
    u32 b;
    int idx;
    if (dy < 0) {
        idx = func_020017d4(-fy, fx);
        a = 0;
        b = 0;
    } else {
        idx = func_020017d4(fy, -fx);
        a = 180;
        b = 0x800;
    }
    a += (double) (idx * 180) / 12861.44;
    if (a >= 360) {
        a -= 360;
    }
    b += (double) (idx << 11) / 12861.44;
    if (a >= 0x1000) {
        a = 0;
    }
    if (mDistance != 0) {
        mUnk_38 = a;
        mUnk_3c = b;
    } else {
        mUnk_38 = -1;
        mUnk_3c = -1;
    }
}

BOOL TouchManager::func_02022ae0() {
    return func_0200c17c(&mPanel);
}

void TouchManager::func_02022af0() {
    func_0200c210(&mPanel);
    mEnabled = 1;
}
