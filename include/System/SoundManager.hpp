#pragma once

#include "global.h"
#include "types.h"

// Global sound player, constructed on first use and destroyed at exit.
class SoundManager {
public:
    ~SoundManager();

    static SoundManager &Get() {
        static SoundManager instance;
        return instance;
    }

    void func_020246f8(u32 unk1);
};
