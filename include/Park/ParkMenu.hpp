#pragma once

#include "global.h"
#include "types.h"

class CellAnimation;
class MessageWindow;
class SelectWindow;
class Ranking;
struct MessageFile;

// Slide directions: the values UpdateSelection and HandleInput return and UpdateSelection stores in mMode (0: none).
// PREV moves mTarget one panel back (mTarget--), NEXT one forward (mTarget++); Slide adds its step to the panels' x
// for PREV and subtracts it for NEXT.
#define PARK_MENU_SLIDE_PREV 1
#define PARK_MENU_SLIDE_NEXT 2

// The park's character carousel: up to four panels slid left/right, with arrows and a number readout.
class ParkMenu {
public:
    /* 00 (vtable) */
    /* 04 */ u8 mCurrent;
    /* 05 */ u8 mTarget;
    /* 06 */ u8 mMode; // PARK_MENU_SLIDE_* stored by UpdateSelection, 0 when that call started no slide
    /* 07 */ u8 mTimer;
    /* 08 */ u8 mCount;
    /* 09 */ PAD(0x09, 0x0c);
    /* 0c */ u8 mUnk_0c[6]; // 0x0c-0x17 hold one 12-byte ParkEntry from GetParkEntry (+0x12 is its byte 6)
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
            /* 38 */ CellAnimation *mCellAnims[8]; // 0-3 characters, 4-5 union cells, 6 point, 7 number
        };
    };
    /* 58 */ SelectWindow *mWindow;
    /* 5c */ Ranking *mRanking;
    /* 60 */ MessageWindow *mMessageWindow;
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
