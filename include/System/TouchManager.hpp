#pragma once

#include "global.h"
#include "types.h"

// Global touch screen state, constructed on first use and destroyed at exit.
class TouchManager {
public:
    /* 00 */ PAD(0x00, 0x30);
    /* 30 */ u32 mUnk_30;
    /* 34 */ PAD(0x34, 0x56);
    /* 56 */ u8 mUnk_56;
    /* 57 */ u8 mUnk_57;
    /* 58 */ PAD(0x58, 0x5a);
    /* 5a */ u8 mUnk_5a;
    /* 5b */ PAD(0x5b, 0x64);
    /* 64 */

    ~TouchManager();

    static TouchManager &Get() {
        static TouchManager instance;
        return instance;
    }

    BOOL func_020228c0(u32 unk1, s16 unk2, u32 unk3, u32 unk4);
};
