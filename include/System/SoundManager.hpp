#pragma once

#include "global.h"
#include "types.h"

// Global sound player, constructed on first use and destroyed at exit.
class SoundManager {
public:
    /* 00 */ PAD(0x00, 0x9c);
    /* 9c */ u32 mUnk_9c;
    /* a0 */

    ~SoundManager();

    static SoundManager &Get() {
        static SoundManager instance;
        return instance;
    }

    void func_020245a4();
    void func_020245d4(u32 unk1);
    void func_020246f8(u32 unk1);
};
