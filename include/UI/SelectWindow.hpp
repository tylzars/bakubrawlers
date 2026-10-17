#pragma once

#include "global.h"
#include "types.h"

class CellAnimation;

// Cell animation used by a window element.
struct WindowCellRecord {
    /* 00 */ const char *mPath;
    // The first member of each union is the one the code reads (as a byte); initializers still fill a whole word.
    union {
        /* 04 */ u8 mAnimIndex;
        u32 mUnk_04_word;
    };
    /* 08 */ u32 mCharOffset;
    union {
        /* 0c */ s8 mPaletteBase;
        u32 mUnk_0c_word;
    };
    union {
        /* 10 */ u8 mUnk_10;
        u32 mUnk_10_word;
    };
    /* 14 */ u32 mEnabled;
    /* 18 */ u8 mAnimIndices[4];
    /* 1c */
};

// Bits of WindowItem::mFlags (WindowItemRecord::mUnk_04 in the tables), named from the SelectWindowCursor and
// SelectWindow code that tests them. When an item is chosen (key mask 1, the key-mask searches below, or a touch
// release), UpdateInput gives event 1 for bit 1, else 2 for bit 2, else 6 for WINDOW_ITEM_NO_RESULT, else 3.
// Left as literals: bits 1 and 2 (events 1 / 2 differ only in the sound, seq 0 / 1, and in mResult 1 / 2, whose
// meaning is each caller's); bit 4, the item key mask 1 chooses wherever the cursor is; bit 2 and then 0x100, the
// items key mask 2 chooses; 0x200 and 0x400, the items key masks 0x200 and 0x100 choose (those key masks are literals
// too); and the composites holding them (5, 0x208, 0x408, 0x804).
// After its result animation SelectWindow::Update goes to state 5 (mSelection = mSavedSelection, which the event-3
// choose records as the selection before it; then state 1) instead of state 3 (UpdateSlideOut). Only SelectWindow
// tests it: on a SelectWindowCursor used without a SelectWindow (in ListScene and CharacterEditScene) nothing reads it.
#define WINDOW_ITEM_NO_SLIDE_OUT 0x8
// SetupScroll makes the first such item mScrollItem; with mScrollMax != 0 a touch press inside its x range over the
// scroll area sets mDragging, and until the release the touch y sets mScroll.
#define WINDOW_ITEM_SCROLL_DRAG 0x10
#define WINDOW_ITEM_SCROLL_PREV 0x20 // a touch on it (with mScrollMax != 0) does mScroll-- while mScroll != 0
#define WINDOW_ITEM_SCROLL_NEXT 0x40 // a touch on it (with mScrollMax != 0) does mScroll++ while mScroll < mScrollMax
// Choosing it gives event 6 (SelectWindowCursor::Update with a non-zero argument also plays seq 2 and sets
// mSelection = -1), for which SelectWindow::Update sets no result; a touch on it gives event 7 instead of 4 / 5.
#define WINDOW_ITEM_NO_RESULT 0x800
// Key moves step past it to its neighbour in the same direction (or stay put); touch press, release and hold
// ignore it.
#define WINDOW_ITEM_CURSOR_SKIP 0x8000

// Values of WindowItem::mUnk_18 (the tables' last column). Any non-zero value lets the keys choose the item
// (UpdateInput tests mUnk_18 != 0 first). Left as literals: 0 (neither the keys nor a touch release choose it; only
// key mask 2's search for bit 2 skips the test) and 3 (a touch release never chooses it; SelectWindow::Update's
// event-5 case sets its mUnk_0a animation when a held touch moves onto it). CreateCommandListWindow and
// CreateCommandPageWindow rewrite the column at run time.
// A touch released inside the item it was pressed on chooses it (a held touch that leaves the item clears
// mPressedItem, so the release then chooses nothing).
#define WINDOW_TOUCH_RELEASE 1
// The same, but only when the item was mSelection as the press began (mPressedOnSelection).
#define WINDOW_TOUCH_RELEASE_IF_SELECTED 2

// Placement of a selectable element and its neighbors for cursor movement (-1: none).
struct WindowItemRecord {
    /* 00 */ u32 mId;
    /* 04 */ u32 mUnk_04;
    /* 08 */ s16 mX;
    /* 0a */ s16 mY;
    /* 0c */ s16 mWidth;
    /* 0e */ s16 mHeight;
    union {
        /* 10 */ u16 mUnk_10;
        struct {
            /* 10 */ u8 mSlideIn;
            /* 11 */ u8 mSlideOut;
        };
    };
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
    /* 10 */ u8 mSlideIn;  // direction the cell moves while sliding in (1 up, 2 down, 3 left, 4 right)
    /* 11 */ u8 mSlideOut; // direction the cell moves while sliding out (same values)
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
    /* 07 */ s8 mPressedItem;
    /* 08 */ u8 mPressedOnSelection;
    /* 09 */ u8 mScrollItem;
    /* 0a */ u8 mDragging;
    /* 0b */ u8 mScrollItemY;
    /* 0c */ u8 mScrollAreaY;
    /* 0d */ u8 mScrollAreaHeight;
    /* 0e */ u8 mShownRowCount;
    /* 0f */ u8 mRowCount;
    /* 10 */ u8 mScrollMax;
    /* 11 */ u8 mScroll;
    /* 12 */ PAD(0x12, 0x14);
    /* 14 */

    SelectWindowCursor(u32 count, const void *table, u32 unk3); // 0x0202f420
    u32 Update(u32 unk1); // UpdateInput(unk1); with unk1 != 0 also the event's sound, and event 6 sets mSelection -1
    u32 UpdateInput(u32 playSound);
    void SetupScroll(u32 unk1, u32 unk2, u32 unk3, u32 unk4);
    void SetScroll(u32 scroll); // 020304ec, clamps to mScrollMax
    void RefreshScrollItemY();
    BOOL HasNeighbor(int index);
};

// An on-screen button of a window; mEnabled toggles whether it can be pressed.
struct WindowButton {
    /* 00 */ u8 mUnk_00;
    /* 01 */ PAD(0x01, 0x04);
    /* 04 */ u32 mEnabled;
    /* 08 */ u8 mUnk_08; // animation index of the idle cell
    /* 09 */ u8 mResultAnimIndex; // animation index set when the item gives a result (Update events 1-3, SetResultItem)
    /* 0a */ u8 mUnk_0a;
    /* 0b */ PAD(0x0b, 0x0c);
    /* 0c */
};

// A window of selectable items driven by touch or buttons.
class SelectWindow {
public:
    /* 000 (vtable) */
    /* 004 */ SelectWindowCursor *mCursor;
    /* 008 */ CellAnimation *mCellAnims[22];
    /* 060 */ u8 mCellCount;
    /* 061 */ u8 mState; // state
    /* 062 */ u8 mSlideStep; // step of UpdateSlideIn (state 0) and of UpdateSlideOut (state 3)
    /* 063 */ s8 mSavedSelection;
    /* 064 */ u8 mEnabled; // Update does nothing while 0; Draw does not test it
    /* 065 */ u8 mInputLocked; // set by results 1 and 2, cleared in state 4; an item with WINDOW_ITEM_NO_SLIDE_OUT returns to state 1 through state 5 with it still set
    /* 066 */ u8 mSlideInRequested; // set by SetItems / Reset; cleared when the slide-out ends (state 3, and by BattleCommand); state 4 waits for it
    /* 067 */ PAD(0x067, 0x068);
    /* 068 */ u8 mSlideToTarget[22]; // per cell id: 1 = moves to (mSlideTargetX, mSlideTargetY) (SlideCellTo)
    /* 07e */ u8 mUnk_07e;
    /* 07f */ PAD(0x07f, 0x080);
    /* 080 */ Vec2s mCellPos[22];
    /* 0d8 */ s16 mSlideTargetX;
    /* 0da */ s16 mSlideTargetY;
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
    void Reset(int selection); // SetItems with the current items; also clears mUnk_0e0
    void SetResultItem(u32 unk1, u32 unk2); // selects item unk1 as if chosen: its result animation, mResult = unk2, state 2
    void SetItems(u32 count, WindowItem *items, s32 selection); // new items and selection; cells to the items' positions, slide-in requested
    void FreeCells(); // deletes the 22 cells; mCellCount is left as it is
    void FreeCursor();
    BOOL UpdateSlideIn();
    void UpdateSlideOut();

    // Starts sliding cell `index` towards (x, y).
    void SlideCellTo(u8 index, s16 x, s16 y) {
        mSlideToTarget[index] = 1;
        mSlideTargetX        = x;
        mSlideTargetY        = y;
    }
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
