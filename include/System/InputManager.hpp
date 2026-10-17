#pragma once

#include "global.h"
#include "types.h"

#define INPUT_KEY_COUNT 12

// Direction bits of the key masks passed to IsPressed / IsDown / IsTrigger (and copied into FieldSelectScene's
// mInput[].mKeys), named from code that acts on them: NameEntryScene moves its cursor, drawn at
// x = mCol * 24 + 0x20, y = mRow * 24 + 0x38, by mRow-- for KEY_UP, mRow++ for KEY_DOWN, mCol - 1 (mod 8) for
// KEY_LEFT and mCol + 1 (mod 8) for KEY_RIGHT. Larger y is further down the screen and larger x further right
// (TextManager::Print adds each glyph's width to x and starts a wrapped line at the starting x and y plus the
// caller's line spacing, 0x10 at MessageWindow's call). The other bits of the set (1, 2, 4, 8, 0x100, 0x200, 0x400,
// 0x800) are left as literals.
#define KEY_RIGHT 0x10
#define KEY_LEFT 0x20
#define KEY_UP 0x40
#define KEY_DOWN 0x80

// Global input state, constructed on first use and destroyed at exit.
// Offsets 00/04 are what IsPressed/IsTrigger read; the update (Update) shows the roles below.
class InputManager {
public:
    /* 00 */ u16 mPressed; // keys that went down this frame (Update: keys & (keys ^ old))
    /* 02 */ u16 mKeys;    // raw key state of this frame
    /* 04 */ u16 mTrigger; // pressed this frame, or auto-repeat firing
    /* 06 */ u16 mReleased;
    /* 08 */ u16 mRepeatCount[INPUT_KEY_COUNT];
    /* 20 */ u16 mHoldCount[INPUT_KEY_COUNT];
    /* 38 */ u16 mRepeatDelay;
    /* 3a */ u8 mRepeatInterval;
    /* 3b */ PAD(0x3b, 0x3c);
    /* 3c */

#ifdef GAME_SINGLETON_CTORS
    // As Game.cpp saw the class: an empty constructor, so its copies of the singleton objects join guard first.
    InputManager() {}
#endif
    ~InputManager();

    void Init();
    void Update(BOOL enabled);

    static InputManager &Get() {
        static InputManager instance;
        return instance;
    }

    u16 IsPressed(u16 keys) {
        return mPressed & keys;
    }
    u16 IsDown(u16 keys) {
        return mKeys & keys;
    }
    u16 IsTrigger(u16 keys) {
        return mTrigger & keys;
    }
};
