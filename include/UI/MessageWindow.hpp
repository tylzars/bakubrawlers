#pragma once

#include "global.h"
#include "types.h"

struct MessageFile;

// A text box that shows a message from a .mes file.
class MessageWindow {
public:
    /* 00 (vtable) */
    /* 04 */ MessageFile *mFile;
    /* 08 */ void *mBuffer;
    /* 0c */ void *mGlyphs;
    /* 10 */ u32 mBufferSize;
    /* 14 */ u32 mGlyphCount;
    /* 18 */ u32 mVram; // destination of the text buffer in VRAM
    /* 1c */ s16 mX;
    /* 1e */ s16 mY;
    /* 20 */ u16 mUnk_20;
    /* 22 */ s16 mUnk_22;
    /* 24 */ s16 mMessage; // -1: none
    /* 26 */ u8 mUnk_26;
    /* 27 */ u8 mUnk_27;
    /* 28 */ u8 mScreen;
    /* 29 */ u8 mWidth;
    /* 2a */ u8 mTimer;
    /* 2b */ u8 mUnk_2b;
    /* 2c */

    MessageWindow(const char *name, u32 unk2, u32 unk3);
    virtual ~MessageWindow();
    void func_02065878();
    void func_020658d0();
    void func_02065bd4(u32 unk1, u32 unk2, u32 unk3);
    void func_02065be4(u32 unk1);
    void func_02065890(u32 unk1, u32 unk2, u32 unk3);
    void func_02065bcc(u32 vram);
    void func_02065660(const char *name);
    void func_020656c4();
    void func_02065818();
};

// Looks up a 16-bit value for an id (table of 213 entries; 0 when out of range).
u16 func_02065c20(u32 id);
