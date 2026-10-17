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
    /* 20 */ u16 mTextWidth;
    /* 22 */ s16 mTextX;
    /* 24 */ s16 mMessage; // -1: none
    /* 26 */ u8 mUnk_26;
    /* 27 */ u8 mUnk_27;
    /* 28 */ u8 mScreen;
    /* 29 */ u8 mWidth;
    /* 2a */ u8 mTimer;
    /* 2b */ u8 mLoopShortText;
    /* 2c */

    MessageWindow(const char *name, u32 unk2, u32 unk3);
    virtual ~MessageWindow();
    void ClearBuffer();
    void Update();
    void func_02065bd4(u32 unk1, u32 unk2, u32 unk3);
    void SetWidth(u32 unk1);
    void StartMessage(u32 unk1, u32 unk2, u32 unk3);
    void SetVram(u32 vram);
    void LoadMessageFile(const char *name);
    void InitText(); // also uploads the text palette and clears 0x800 bytes at mVram + 0x6400000 (0x6600000 for screen 1)
    void UploadTextPalette();
};

// Looks up a 16-bit value for an id (table of 213 entries; 0 when out of range).
u16 GetCardNumber(u32 id);
