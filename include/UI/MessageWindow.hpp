#pragma once

#include "global.h"
#include "types.h"

// A text box that shows a message from a .mes file.
class MessageWindow {
public:
    /* 00 (vtable) */
    /* 04 */ PAD(0x04, 0x2c);
    /* 2c */

    MessageWindow(const char *name, u32 unk2, u32 unk3);
    virtual ~MessageWindow();
    void func_020658d0();
    void func_02065bd4(u32 unk1, u32 unk2, u32 unk3);
    void func_02065be4(u32 unk1);
    void func_02065890(u32 unk1, u32 unk2, u32 unk3);
};
