#pragma once

#include "global.h"
#include "types.h"

class CellAnimation;
class MessageWindow;
class SelectWindow;
class Ranking;
struct MessageFile;

// The park's character carousel: up to four panels slid left/right, with arrows and a number readout.
class ParkMenu {
public:
    /* 00 (vtable) */
    /* 04 */ u8 mCurrent;
    /* 05 */ u8 mTarget;
    /* 06 */ u8 mMode; // 1: sliding left, 2: sliding right
    /* 07 */ u8 mTimer;
    /* 08 */ u8 mCount;
    /* 09 */ PAD(0x09, 0x0c);
    /* 0c */ u8 mUnk_0c[6]; // filled by GetParkEntry
    /* 12 */ u8 mProgressEntryIndex;    // 0xff: none
    /* 13 */ PAD(0x13, 0x18);
    union {
        // The constructor clears this region as pointer slots in one loop.
        /* 18 */ void *mSlots[16];
        struct {
            /* 18 */ void *mTextBuffers[2];
            /* 20 */ void *mGlyphBuffer;
            /* 24 */ u32 mTextBufferSize;
            /* 28 */ u32 mGlyphCount;
            /* 2c */ u16 mTextWidth[2];
            /* 30 */ MessageFile *mMessages[2];
            /* 38 */ CellAnimation *mCells[8]; // 0-3 characters, 4-5 union cells, 6 point, 7 number
        };
    };
    /* 58 */ SelectWindow *mWindow;
    /* 5c */ Ranking *mRanking;
    /* 60 */ MessageWindow *mMessage;
    /* 64 */

    ParkMenu(u8 current);
    virtual ~ParkMenu();

    void LoadGraphics();         // 0221c4bc
    u32 UpdateSelection(u32 mode); // 0221ca84
    int HandleInput();           // 0221cd10
    void Setup();                // 0221ce58
    BOOL Slide();                // 0221d014
    void DrawText();             // 0221d1fc
    u32 TextEntry(u8 i);         // inline, DrawText
    void ClearBuffers();         // 0221d460
    void Draw();                 // 0221d4a0
    void DrawNumber();           // 0221d6d4
};
