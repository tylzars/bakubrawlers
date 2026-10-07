#pragma once

#include "global.h"
#include "types.h"

class CellAnimation;

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

// Runtime copy of a WindowItemRecord, kept by the cursor.
struct WindowItem {
    /* 00 */ s8 mId;
    /* 01 */ PAD(0x01, 0x04);
    /* 04 */ u32 mFlags;
    /* 08 */ s16 mX;
    /* 0a */ s16 mY;
    /* 0c */ PAD(0x0c, 0x1c);
    /* 1c */
};

struct SelectWindowCursor {
    /* 00 */ WindowItem *mItems;
    /* 04 */ u8 mCount;
    /* 05 */ PAD(0x05, 0x06);
    /* 06 */ s8 mSelection;
    /* 07 */ PAD(0x07, 0x0b);
    /* 0b */ u8 mUnk_0b;
    /* 0c */ u8 mUnk_0c;
    /* 0d */ u8 mUnk_0d;
    /* 0e */ u8 mUnk_0e;
    /* 0f */ u8 mUnk_0f;
    /* 10 */ u8 mUnk_10;
    /* 11 */ u8 mScroll;
    /* 12 */

    void func_02030474(u32 unk1, u32 unk2, u32 unk3, u32 unk4);
    void SetScroll(u32 scroll); // 020304ec, clamps to mUnk_10
};

// An on-screen button of a window; mEnabled toggles whether it can be pressed.
struct WindowButton {
    /* 0 */ u32 mEnabled;
    /* 4 */ PAD(0x4, 0xc);
    /* c */
};

// A window of selectable items driven by touch or buttons.
class SelectWindow {
public:
    /* 000 (vtable) */
    /* 004 */ SelectWindowCursor *mCursor;
    /* 008 */ CellAnimation *mCells[6];
    /* 020 */ PAD(0x020, 0x061);
    /* 061 */ u8 mUnk_061;
    /* 062 */ PAD(0x062, 0x064);
    /* 064 */ u8 mUnk_064;
    /* 065 */ u8 mUnk_065;
    /* 066 */ PAD(0x066, 0x080);
    /* 080 */ Vec2s mCellPos[6];
    /* 098 */ PAD(0x098, 0x0e8);
    /* 0e8 */ WindowButton mButtons[7];
    /* 13c */ PAD(0x13c, 0x1ec);
    /* 1ec */

    SelectWindow(u32 itemCount, const WindowItemRecord *items, s32 unk3);
    virtual ~SelectWindow();
    void SetCells(u32 count, const WindowCellRecord *cells);
    BOOL Update(BOOL acceptInput);
    void Draw();
    void func_0205c7c8(int selection);
    void func_0205c844(u32 unk1, u32 unk2);

    Vec2s GetCellPos(int index) {
        return mCellPos[index];
    }
    void SetCellPos(int index, Vec2s pos) {
        mCellPos[index] = pos;
    }
    s8 GetSelectedId() {
        if (mCursor->mSelection == -1) {
            return -1;
        }
        return mCursor->mItems[mCursor->mSelection].mId;
    }
};
