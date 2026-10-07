#pragma once

#include "global.h"
#include "types.h"

// Global input state, constructed on first use and destroyed at exit.
class InputManager {
public:
    /* 00 */ u16 mHeld;
    /* 02 */ PAD(0x02, 0x04);
    /* 04 */ u16 mTrigger;
    /* 06 */

    ~InputManager();

    static InputManager &Get() {
        static InputManager instance;
        return instance;
    }

    u16 IsHeld(u16 keys) {
        return mHeld & keys;
    }
    u16 IsTrigger(u16 keys) {
        return mTrigger & keys;
    }
};
