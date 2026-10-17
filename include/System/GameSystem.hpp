#pragma once

#include "global.h"
#include "types.h"

// Game system singleton (instance 0x020c1884, destructor 0x020219c0). main() calls Init (one-time start-up) and then
// Run (the endless main loop); mFrame counts scene updates since Run started.
// func_02021700, called every pass of Run and in its halt loop, tests bit 15 of KEY_EXT_SHARED (0x027fffa8): while
// it is set, if a save is running (SaveManager::mBusy) or WirelessManager::mState is non-zero it calls
// func_0200d1e0(2, 0) once and clears mInputEnabled, otherwise it calls func_0200d3e0(0xc, 0, 0); when the bit is
// clear and mInputEnabled is 0 it passes mUnk_00 / mUnk_04 back to func_0200d1e0(0 / 1, ...) and sets
// mInputEnabled; then it calls TerminateAfterCardPullout if IsCardPulledOut (card_pullout.c) returns non-zero.
class GameSystem {
public:
    /* 00 */ s32 mUnk_00; // (st & 8) != 0 of the status func_0200d354 reads in Init (stored through this); func_02021700 passes it back as func_0200d1e0(0, mUnk_00)
    /* 04 */ s32 mUnk_04; // (st & 4) != 0 of the same read; func_02021700 passes it back as func_0200d1e0(1, mUnk_04)
    /* 08 */ u8 mResetHoldFrames; // passes of Run with every RESET_KEYS key down (IsDown), capped at 3; at 3 Run resets (see RESET_KEYS)
    /* 09 */ u8 mInputEnabled; // passed to InputManager/TouchManager::Update (0 = no keys or touch); func_02021700 also clears and sets it (see above)
    /* 0a */ u8 mExtraFrames; // extra frames (full scene updates) done this pass before the regular one, up to mFrameInterval - 1; never runs in this build (mMinInterval = mMaxInterval = 1)
    /* 0b */ u8 mMinInterval; // lower bound of mFrameInterval; Init sets this and mMaxInterval to 1 and nothing writes them again, so mFrameInterval is always 1
    /* 0c */ u8 mMaxInterval; // upper bound of mFrameInterval (1 in this build, see mMinInterval)
    /* 0d */ u8 mFrameInterval;
    /* 0e */ PAD(0x0e, 0x10);
    /* 10 */ u32 mFrame;
    /* 14 */ u8 mHalted; // set by MessageScreen's constructor (kinds 0 and 1); Run then stops updating scenes and loops on func_02021700, the frame wait and the wireless and sound updates
    /* 15 */ u8 mWirelessWaitFrames; // frame waits (FrameTask::WaitFrame) Run spent this pass for WirelessManager::mUnk_124e in a wireless mode; BattleCardModel::Update runs this + 1 passes
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
    const char *GetLanguageSuffix(); // suffix appended to file names ("%s_%s"): gLanguageSuffixes[data_020d433c.mUnk_79], NULL (no suffix) for 0
    void Init();
    void Run();
    void func_02021700();
};
