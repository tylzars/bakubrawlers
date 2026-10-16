#pragma once

#include "global.h"
#include "types.h"

// Constructor argument: the message shown. Kinds 0 and 1 behave the same (mes_system, then GameSystem::Run loops
// forever without running scenes, Game.cpp:289-295); they differ only in the entry printed.
#define MESSAGE_SCREEN_SYSTEM_5_HALT 0 // mes_system entry 5, then halt
#define MESSAGE_SCREEN_SYSTEM_4_HALT 1 // mes_system entry 4, then halt
#define MESSAGE_SCREEN_TITLE_8 2       // mes_title entry 8, no halt

// Shows one message over the sub screen: sets up both screens' display registers, loads mes_system or mes_title, prints
// one entry into a full-screen buffer and loads it as sub-screen BG0. Kinds other than MESSAGE_SCREEN_TITLE_8 set
// GameSystem::mHalted. 4 bytes: just the vtable.
class MessageScreen {
public:
    /* 00 (vtable) */
    /* 04 */

    MessageScreen(u32 unk1);
    virtual ~MessageScreen();
};
