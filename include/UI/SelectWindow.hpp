#pragma once

#include "global.h"
#include "types.h"

// Cell animation used by a window element.
struct WindowCellRecord {
    /* 00 */ const char *mPath;
    /* 04 */ u32 mUnk_04;
    /* 08 */ u32 mUnk_08;
    /* 0c */ u32 mUnk_0c;
    /* 10 */ u32 mUnk_10;
    /* 14 */ u32 mUnk_14;
    /* 18 */ u8 mUnk_18[4];
    /* 1c */
};

// Placement of a selectable element and its neighbors for cursor movement (-1: none).
struct WindowItemRecord {
    /* 00 */ u32 mId;
    /* 04 */ u32 mUnk_04;
    /* 08 */ s16 mX;
    /* 0a */ s16 mY;
    /* 0c */ s16 mWidth;
    /* 0e */ s16 mHeight;
    /* 10 */ u16 mUnk_10;
    /* 12 */ s8 mNeighbors[4];
    /* 16 */ u16 mUnk_16;
    /* 18 */ u32 mUnk_18;
    /* 1c */
};

struct SelectWindowCursor {
    /* 0 */ PAD(0x0, 0x6);
    /* 6 */ s8 mSelection;
};

// A window of selectable items driven by touch or buttons.
class SelectWindow {
public:
    /* 000 (vtable) */
    /* 004 */ SelectWindowCursor *mCursor;
    /* 008 */ PAD(0x008, 0x064);
    /* 064 */ u8 mUnk_064;
    /* 065 */ PAD(0x065, 0x1ec);
    /* 1ec */

    SelectWindow(u32 itemCount, const WindowItemRecord *items, s32 unk3);
    virtual ~SelectWindow();
    void SetCells(u32 count, const WindowCellRecord *cells);
    BOOL Update(BOOL acceptInput);
    void Draw();
};
