#pragma once

#include "global.h"
#include "types.h"

#define INPUT_KEY_COUNT 12

// Global input state, constructed on first use and destroyed at exit.
// Offsets 00/04 are what IsHeld/IsTrigger read; the update (Update) shows the roles below.
class InputManager {
public:
    /* 00 */ u16 mHeld;    // keys that went down this frame (Update: keys & (keys ^ old))
    /* 02 */ u16 mKeys;    // raw key state of this frame
    /* 04 */ u16 mTrigger; // pressed this frame, or auto-repeat firing
    /* 06 */ u16 mReleased;
    /* 08 */ u16 mRepeatCount[INPUT_KEY_COUNT];
    /* 20 */ u16 mHoldCount[INPUT_KEY_COUNT];
    /* 38 */ u16 mRepeatDelay;
    /* 3a */ u8 mRepeatInterval;
    /* 3b */ PAD(0x3b, 0x3c);
    /* 3c */

    ~InputManager();

    void Init();
    void Update(BOOL enabled);

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
