#pragma once

#include "global.h"
#include "types.h"

// One touch screen sample (the layout produced by the touch panel library's calibrated read).
struct TouchSample {
    /* 00 */ u16 x;
    /* 02 */ u16 y;
    /* 04 */ u16 touch;
    /* 06 */ u16 valid; // nonzero when the sample is unusable
    /* 08 */
};

// Global touch screen state, constructed on first use and destroyed at exit.
class TouchManager {
public:
    /* 00 */ TouchSample mCur;
    /* 08 */ union {
        TouchSample mPrev; // previous sample
        struct {
            s16 mUnk_08; // previous x
            s16 mUnk_0a; // previous y
        };
    };
    /* 10 */ PAD(0x10, 0x28);
    /* 28 */ u8 mPanel[8]; // handed to the touch panel library (0x0200c17c / 0x0200c210)
    /* 30 */ s32 mX;
    /* 34 */ s32 mY;
    /* 38 */ s32 mUnk_38; // degrees, or -1 when the stylus did not move
    /* 3c */ s32 mUnk_3c; // angle in 1/4096 turns, or -1
    /* 40 */ s32 mDistance; // distance moved since the previous sample, whole pixels
    /* 44 */ PAD(0x44, 0x50);
    /* 50 */ s16 mUnk_50;
    /* 52 */ s16 mRepeatDelay;
    /* 54 */ s16 mRepeatTimer;
    /* 56 */ u8 mUnk_56; // touching
    /* 57 */ u8 mUnk_57; // newly touched
    /* 58 */ u8 mUnk_58; // touched or auto repeat
    /* 59 */ u8 mUnk_59;
    /* 5a */ u8 mTouching; // newly released
    /* 5b */ s8 mCountdown;
    /* 5c */ s8 mCountdown2;
    /* 5d */ s8 mCountdownStart;
    /* 5e */ s8 mCountdown2Start;
    /* 5f */ s8 mUnk_5f;
    /* 60 */ u8 mUnk_60;
    /* 61 */ u8 mEnabled;
    /* 62 */ PAD(0x62, 0x64);
    /* 64 */

    ~TouchManager();

    static TouchManager &Get() {
        static TouchManager instance;
        return instance;
    }

    void Init();
    void func_020225d0();
    void Update(BOOL enabled);
    BOOL HitTest(s32 x, s32 y, s32 width, s16 height);
    BOOL func_02022830(s32 x, s32 y, s32 width, s16 height);
    BOOL func_02022860(s32 x, s32 y, s32 width, s16 height);
    BOOL func_02022890(s32 x, s32 y, s32 width, s16 height);
    BOOL func_020228c0(u32 x, s16 y, u32 width, s16 height);
    void func_020228f0();
    void func_02022988();
    void func_02022ae0();
    void func_02022af0();
};
