#pragma once

#include "global.h"
#include "types.h"

class CellAnimation;

// Cell animation used by a window element.
struct WindowCellRecord {
    /* 00 */ const char *mPath;
    // The first member of each union is the one the code reads (as a byte); initializers still fill a whole word.
    union {
        /* 04 */ u8 mUnk_04;
        u32 mUnk_04_word;
    };
    /* 08 */ u32 mUnk_08;
    union {
        /* 0c */ s8 mUnk_0c;
        u32 mUnk_0c_word;
    };
    union {
        /* 10 */ u8 mUnk_10;
        u32 mUnk_10_word;
    };
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

// Runtime copy of a WindowItemRecord, kept by the cursor.
struct WindowItem {
    /* 00 */ s8 mId;
    /* 01 */ PAD(0x01, 0x04);
    /* 04 */ u32 mFlags;
    /* 08 */ s16 mX;
    /* 0a */ s16 mY;
    /* 0c */ s16 mWidth;
    /* 0e */ s16 mHeight;
    /* 10 */ u8 mSlideIn;  // edge the cell slides in from (1 top, 2 bottom, 3 right, 4 left)
    /* 11 */ u8 mSlideOut; // edge the cell slides out to
    /* 12 */ s8 mNeighbors[4];
    /* 16 */ PAD(0x16, 0x18);
    /* 18 */ u32 mUnk_18;
    /* 1c */
};

// Cursor over the items of a window.
struct SelectWindowCursor {
    /* 00 */ WindowItem *mItems;
    /* 04 */ u8 mCount;
    /* 05 */ s8 mUnk_05;
    /* 06 */ s8 mSelection;
    /* 07 */ s8 mUnk_07;
    /* 08 */ u8 mUnk_08;
    /* 09 */ u8 mUnk_09;
    /* 0a */ u8 mUnk_0a;
    /* 0b */ u8 mUnk_0b;
    /* 0c */ u8 mUnk_0c;
    /* 0d */ u8 mUnk_0d;
    /* 0e */ u8 mUnk_0e;
    /* 0f */ u8 mUnk_0f;
    /* 10 */ u8 mUnk_10;
    /* 11 */ u8 mScroll;
    /* 12 */ PAD(0x12, 0x14);
    /* 14 */

    SelectWindowCursor(u32 count, const void *table, u32 unk3); // 0x0202f420
    u32 func_0202f468(u32 unk1);
    u32 func_0202f61c(u32 playSound);
    void func_02030474(u32 unk1, u32 unk2, u32 unk3, u32 unk4);
    void SetScroll(u32 scroll); // 020304ec, clamps to mUnk_10
    void func_02030508();
    BOOL func_0203055c(int index);
};

// An on-screen button of a window; mEnabled toggles whether it can be pressed.
struct WindowButton {
    /* 00 */ u8 mUnk_00;
    /* 01 */ PAD(0x01, 0x04);
    /* 04 */ u32 mEnabled;
    /* 08 */ u8 mUnk_08; // animation index of the idle cell
    /* 09 */ u8 mUnk_09; // animation index of the selected cell
    /* 0a */ u8 mUnk_0a;
    /* 0b */ PAD(0x0b, 0x0c);
    /* 0c */
};

// A window of selectable items driven by touch or buttons.
class SelectWindow {
public:
    /* 000 (vtable) */
    /* 004 */ SelectWindowCursor *mCursor;
    /* 008 */ CellAnimation *mCells[22];
    /* 060 */ u8 mCellCount;
    /* 061 */ u8 mUnk_061; // state
    /* 062 */ u8 mUnk_062; // slide phase
    /* 063 */ s8 mUnk_063;
    /* 064 */ u8 mUnk_064;
    /* 065 */ u8 mUnk_065;
    /* 066 */ u8 mUnk_066;
    /* 067 */ PAD(0x067, 0x068);
    /* 068 */ u8 mUnk_068[22];
    /* 07e */ u8 mUnk_07e;
    /* 07f */ PAD(0x07f, 0x080);
    /* 080 */ Vec2s mCellPos[22];
    /* 0d8 */ s16 mUnk_0d8;
    /* 0da */ s16 mUnk_0da;
    /* 0dc */ u32 mResult;
    /* 0e0 */ u8 mUnk_0e0;
    /* 0e1 */ PAD(0x0e1, 0x0e4);
    /* 0e4 */ WindowButton mButtons[22];
    /* 1ec */

    SelectWindow(u32 itemCount, const WindowItemRecord *items, s32 unk3);
    virtual ~SelectWindow();
    void SetCells(u32 count, const WindowCellRecord *cells);
    BOOL Update(BOOL acceptInput);
    void Draw();
    void func_0205c7c8(int selection);
    void func_0205c844(u32 unk1, u32 unk2);
    void func_0205c74c(u32 count, WindowItem *items, s32 selection);
    void func_0205bd10();
    void func_0205bcf4();
    BOOL func_0205bdf4();
    void func_0205c2e0();

    Vec2s GetCellPos(int index) {
        return mCellPos[index];
    }
    void SetCellPos(int index, Vec2s pos) {
        mCellPos[index] = pos;
    }
    s8 GetSelectedId() {
        SelectWindowCursor *c = mCursor;
        if (c->mSelection == -1) {
            return -1;
        }
        return c->mItems[c->mSelection].mId;
    }
};
