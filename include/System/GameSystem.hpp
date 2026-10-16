#pragma once

#include "global.h"
#include "types.h"

// Game object (instance 0x020c1884, destructor 0x020219c0): start-up (Init) and the main loop
// (Run); mFrame is the running frame count.
class GameSystem {
public:
    /* 00 */ s32 mLed0;
    /* 04 */ s32 mLed1;
    /* 08 */ u8 mResetHoldFrames;
    /* 09 */ u8 mInputEnabled;
    /* 0a */ u8 mSubFrame;
    /* 0b */ u8 mMinInterval;
    /* 0c */ u8 mMaxInterval;
    /* 0d */ u8 mFrameInterval;
    /* 0e */ PAD(0x0e, 0x10);
    /* 10 */ u32 mFrame;
    /* 14 */ u8 mHalted;
    /* 15 */ u8 mWirelessWaitFrames;
    /* 16 */ PAD(0x16, 0x18);
    /* 18 */

#ifdef GAME_SINGLETON_CTORS
    // As Game.cpp saw the class: an empty constructor, so its copies of the singleton objects join guard first.
    GameSystem() {}
#endif
    ~GameSystem(); // out of line in Game.cpp (0x020219c0)

    static GameSystem &Get() {
        static GameSystem instance;
        return instance;
    }
    const char *GetLanguageSuffix(); // language suffix of file names
    void Init();
    void Run();
    void func_02021700();
};
