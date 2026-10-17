#include "System/TouchManager.hpp"

#include <nitro/fx.h>
#include <nitro/mi.h>

extern "C" {
extern "C" void TP_Init();
extern "C" int OS_GetCpsrIrqBits();
extern "C" void OS_EnableInterrupts();
extern "C" void TP_RequestRaw();
extern "C" int TP_GetRaw(void *raw);
extern "C" void TP_GetCalibratedPoint(TouchSample *out, void *raw);
extern "C" BOOL TP_GetUserInfo(void *panel);
extern "C" void TP_SetCalibrateParam(void *panel);
extern "C" u16 FX_Atan2(fx32 a, fx32 b);
}

void TouchManager::Init() {
    MI_CpuClear32(&mCur, sizeof(mCur));
    MI_CpuClear32(&mPrev, sizeof(mPrev));
    MI_CpuClear32(&pad_0x10, 0x18);
    MI_CpuClear32(&mPanel, sizeof(mPanel));
    MI_CpuClear32(&pad_0x44, 0xc);
    mX = 0;
    mY = 0;
    mMoveDegrees = 0;
    mMoveAngle = 0;
    mDistance = 0;
    mUnk_50 = 0;
    mDoubleTapFrameCount = 0;
    mDoubleTapTimer = 0;
    mDown = 0;
    mPressed = 0;
    mTrigger = 0;
    mDoubleTap = 0;
    mReleased = 0;
    mCountdown = 0;
    mCountdown2 = 0;
    mCountdownStart = 0;
    mCountdown2Start = 0;
    mTapCount = 0;
    mUnk_60 = 0;
    mEnabled = 0;
    mDoubleTapFrameCount = 30;
    mCountdownStart = mCountdown = 12;
    mCountdown2Start = mCountdown2 = 2;
    mUnk_50 = 0x1240 - 12;
}

void TouchManager::InitPanel() {
    TP_Init();
    Init();
}

void TouchManager::Update(BOOL enabled) {
    TouchSample sample;
    u32 raw[2];

    if (mEnabled == 0) {
        return;
    }
    if (OS_GetCpsrIrqBits() != 0) {
        OS_EnableInterrupts();
    }
    do {
        TP_RequestRaw();
    } while (TP_GetRaw(raw) != 0);
    TP_GetCalibratedPoint(&sample, raw);
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
    mReleased = 0;
    mDown = 0;
    mPressed = 0;
    mTrigger = 0;
    if (sample.touch != 0) {
        mX = sample.x;
        mY = sample.y;
        mDown = 1;
        if (mPrev.touch == 0) {
            mPressed = 1;
            mTrigger = 1;
            mCountdown = mCountdownStart;
            mCountdown2 = mCountdown2Start;
        } else if (mCountdown < 0) {
            mCountdown2--;
            if (mCountdown2 < 0) {
                mTrigger = 1;
                mCountdown2 = mCountdown2Start;
            }
        } else {
            mCountdown--;
            if (mCountdown < 0) {
                mTrigger = 1;
                mCountdown2 = mCountdown2Start;
            }
        }
    } else if (mPrev.touch != 0) {
        mReleased = 1;
    }
    UpdateDoubleTap();
    UpdateMotion();
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

BOOL TouchManager::IsDownIn(s32 x, s32 y, s32 width, s16 height) {
    if (mDown != 0) {
        if (HitTest(x, y, width, height)) {
            return TRUE;
        }
    }
    return FALSE;
}

BOOL TouchManager::IsPressedIn(s32 x, s32 y, s32 width, s16 height) {
    if (mPressed != 0) {
        if (HitTest(x, y, width, height)) {
            return TRUE;
        }
    }
    return FALSE;
}

BOOL TouchManager::IsTriggerIn(s32 x, s32 y, s32 width, s16 height) {
    if (mTrigger != 0) {
        if (HitTest(x, y, width, height)) {
            return TRUE;
        }
    }
    return FALSE;
}

BOOL TouchManager::IsReleasedIn(u32 x, s16 y, u32 width, s16 height) {
    if (mReleased != 0) {
        if (HitTest(x, y, width, height)) {
            return TRUE;
        }
    }
    return FALSE;
}

void TouchManager::UpdateDoubleTap() {
    mDoubleTap = 0;
    if (mPressed != 0) {
        if (mTapCount == 0) {
            mDoubleTapTimer = mDoubleTapFrameCount;
        }
    }
    mDoubleTapTimer--;
    if (mDoubleTapTimer > 0) {
        if (mPressed != 0) {
            mTapCount++;
            return;
        }
        if (mDown != 0) {
            return;
        }
        if (mTapCount >= 2) {
            mDoubleTap = 1;
            mDoubleTapTimer = 0;
            mTapCount = 0;
        }
    } else {
        mDoubleTapTimer = 0;
        mTapCount = 0;
    }
}

void TouchManager::UpdateMotion() {
    s32 dy = mY - mPrev.y;
    s32 dx = mX - mPrev.x;
    fx32 fy = dy << 12;
    fx32 fx = dx << 12;
    mDistance = FX_Sqrt(FX_Mul(fy, fy) + FX_Mul(fx, fx)) >> 12;
    u32 a;
    u32 b;
    int idx;
    if (dy < 0) {
        idx = FX_Atan2(-fy, fx);
        a = 0;
        b = 0;
    } else {
        idx = FX_Atan2(fy, -fx);
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
        mMoveDegrees = a;
        mMoveAngle = b;
    } else {
        mMoveDegrees = -1;
        mMoveAngle = -1;
    }
}

BOOL TouchManager::ReadCalibration() {
    return TP_GetUserInfo(&mPanel);
}

void TouchManager::ApplyCalibrationAndEnable() {
    TP_SetCalibrateParam(&mPanel);
    mEnabled = 1;
}
