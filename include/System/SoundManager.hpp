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
    void func_020245c4(u32 unk1);
    void func_020245d4(u32 unk1);
    void func_020246f8(u32 unk1);
};

// Sound helpers with C linkage that take the manager explicitly.
extern "C" void func_02024764(SoundManager *manager, u32 unk1, u32 unk2);
extern "C" int func_02024790(SoundManager *manager, u32 unk1);
extern "C" void func_020247fc(SoundManager *manager, s16 unk1);
