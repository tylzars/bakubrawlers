#pragma once

#include "global.h"
#include "types.h"

class SelectWindow;
struct MessageFile;

// A popup with a message from a .mes file and two or three buttons.
class ParkPopup {
public:
    /* 00 */ PAD(0x00, 0x1c);
    /* 1c */ void *mTextBuffer;
    /* 20 */ void *mGlyphBuffer;
    /* 24 */ u32 mTextBufferSize;
    /* 28 */ u32 mGlyphCount;
    /* 2c */ MessageFile *mMessages;
    /* 30 */ SelectWindow *mWindow;
    /* 34 */

    void Open(const char *mesPath, u32 index, u32 mode); // 0221ba3c
    void PrintMessage(u32 index);                        // 0221bdf0
};

// Functions next to the popup whose object isn't known yet (`this` is only passed through).
class ParkRules {
public:
    BOOL func_ov004_0221bf5c();
    u8 CountSlotsWith(u32 value); // 0221c184
};
