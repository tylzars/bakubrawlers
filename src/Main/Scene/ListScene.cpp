#include "Scene/ListScene.hpp"
#include "Unknown/UnkStruct_02071d24.hpp"
#include "Battle/BattleCommandList.hpp"
#include "Battle/BattleCommandPage.hpp"
#include "Game/GameData.hpp"
#include "Graphics/CellAnimation.hpp"
#include "UI/SelectWindow.hpp"
#include "Game/SaveData.hpp"
#include <nitro/gx.h>
#include <nitro/mi.h>
#include <nitro/os.h>
#include "Unknown/UnkFunctions.hpp"
#include "Graphics/Display.hpp"
#include "System/FileSystem.hpp"
#include "System/FrameUpdate.hpp"
#include "UI/MessageWindow.hpp"
#include "Game/Random.hpp"
#include "Game/UnkStruct_020d48f0.hpp"
#include "Graphics/AntennaIndicator.hpp"
#include "System/WirelessManager.hpp"
#include "System/TouchManager.hpp"

// One object in the original: the cursor records (0x020a3210) and a byte table (0x020a3248) that follows them. As
// separate tables the size sort below would put the 0x29-byte table before the 0x38-byte ones.
struct AreaCursorAndBakuganOrder {
    /* 00 */ WindowItemRecord mCursor[2];
    /* 38 */ u8 mOrder[0x29];
};

struct BakuganPriceEntry {
    /* 0 */ u16 mPriceTens;
    /* 2 */ u8 mUnk_02; // func_02086548 returns it * 10 (ResetCursorBakuganEntry stores that with func_020231c0)
    /* 3 */ u8 mUnk_03; // never read
};

// mwcc emits a file's objects sorted by size; the order of equal sizes (the window tables and their path strings, and
// the destructor-chain nodes in .bss) follows from the whole declaration sequence. This definition order, with the
// WirelessManager and TouchManager headers before the tables and the other singleton headers after them, gives the
// original layout of all three sections.
static const WindowItemRecord sListWindowItems[7] = {
    {0, WINDOW_ITEM_NO_SLIDE_OUT, 225, 170, 30, 20, 0, {-1, -1, -1, -1}, 0, WINDOW_TOUCH_RELEASE},
    {1, WINDOW_ITEM_NO_SLIDE_OUT, 3, 33, 18, 30, 0, {-1, -1, -1, -1}, 0, 3},
    {2, WINDOW_ITEM_NO_SLIDE_OUT, 235, 33, 18, 30, 0, {-1, -1, -1, -1}, 0, 3},
    {3, WINDOW_ITEM_NO_SLIDE_OUT, 3, 88, 18, 56, 0, {-1, -1, -1, -1}, 0, WINDOW_TOUCH_RELEASE},
    {4, WINDOW_ITEM_NO_SLIDE_OUT, 235, 88, 18, 56, 0, {-1, -1, -1, -1}, 0, WINDOW_TOUCH_RELEASE},
    {5, WINDOW_ITEM_NO_SLIDE_OUT, 3, 66, 18, 20, 0, {-1, -1, -1, -1}, 0, 3},
    {6, WINDOW_ITEM_NO_SLIDE_OUT, 3, 146, 18, 20, 0, {-1, -1, -1, -1}, 0, 3},
};

static const WindowCellRecord sListWindowCells[7] = {
    {"/Game/2D/union_b/union_b_obj_cockpit_nce.bin", 2, 0x6000, 0, 1, 1, {2, 3, 3, 0}},
    {"/Game/List/list_b_obj_00_nce.bin", 4, 0xc000, 6, 1, 1, {4, 5, 5, 0}},
    {"/Game/List/list_b_obj_00_nce.bin", 6, 0xc000, 6, 1, 1, {6, 7, 7, 0}},
    {"/Game/List/list_b_obj_00_nce.bin", 8, 0xc000, 6, 1, 1, {8, 9, 8, 0}},
    {"/Game/List/list_b_obj_00_nce.bin", 10, 0xc000, 6, 1, 1, {10, 11, 10, 0}},
    {"/Game/List/list_b_obj_00_nce.bin", 12, 0xc000, 6, 1, 1, {12, 13, 13, 0}},
    {"/Game/List/list_b_obj_00_nce.bin", 14, 0xc000, 6, 1, 1, {14, 15, 15, 0}},
};

static const u16 sPricePercents[10] = {
    40, 80, 120, 160, 200, 240, 280, 320, 400, 0,
};

// Two 12-byte sprite blocks for SpriteScreen::AddOam (rows 0x020a3188 and 0x020a3194).
static const u8 sCardAndMarkOam[2][12] = {
    {0x00, 0xa0, 0x00, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},
    {0x00, 0x20, 0x00, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},
};

// Five-byte rows. A two-dimensional byte array stays in the shared .rodata section; a flat one would get a section
// of its own.
static const u8 sInitialEntryFields[41][5] = {
    {0x03, 0x01, 0x01, 0x00, 0x02},
    {0x03, 0x02, 0x02, 0x00, 0x02},
    {0x02, 0x01, 0x00, 0x01, 0x02},
    {0x03, 0x02, 0x00, 0x01, 0x02},
    {0x01, 0x01, 0x03, 0x02, 0x00},
    {0x00, 0x01, 0x03, 0x02, 0x02},
    {0x00, 0x01, 0x03, 0x02, 0x02},
    {0x02, 0x03, 0x02, 0x00, 0x00},
    {0x03, 0x03, 0x02, 0x00, 0x00},
    {0x01, 0x00, 0x02, 0x03, 0x01},
    {0x02, 0x00, 0x03, 0x03, 0x01},
    {0x02, 0x02, 0x00, 0x03, 0x01},
    {0x03, 0x02, 0x01, 0x02, 0x01},
    {0x02, 0x01, 0x03, 0x02, 0x00},
    {0x01, 0x03, 0x01, 0x00, 0x02},
    {0x00, 0x01, 0x03, 0x03, 0x01},
    {0x02, 0x01, 0x01, 0x01, 0x03},
    {0x02, 0x02, 0x03, 0x00, 0x00},
    {0x03, 0x00, 0x00, 0x03, 0x01},
    {0x00, 0x01, 0x00, 0x00, 0x00},
    {0x00, 0x00, 0x01, 0x00, 0x00},
    {0x00, 0x00, 0x00, 0x01, 0x00},
    {0x01, 0x00, 0x00, 0x00, 0x00},
    {0x02, 0x00, 0x00, 0x00, 0x01},
    {0x00, 0x00, 0x00, 0x00, 0x01},
    {0x02, 0x01, 0x00, 0x01, 0x00},
    {0x03, 0x01, 0x00, 0x01, 0x00},
    {0x00, 0x01, 0x00, 0x00, 0x00},
    {0x00, 0x00, 0x00, 0x01, 0x01},
    {0x00, 0x02, 0x00, 0x00, 0x01},
    {0x00, 0x03, 0x01, 0x00, 0x00},
    {0x00, 0x00, 0x01, 0x01, 0x00},
    {0x00, 0x00, 0x00, 0x01, 0x00},
    {0x01, 0x00, 0x00, 0x01, 0x00},
    {0x00, 0x01, 0x02, 0x01, 0x01},
    {0x01, 0x00, 0x00, 0x02, 0x00},
    {0x02, 0x01, 0x01, 0x02, 0x01},
    {0x02, 0x01, 0x02, 0x03, 0x01},
    {0x01, 0x03, 0x00, 0x03, 0x01},
    {0x01, 0x02, 0x01, 0x03, 0x02},
    {0x03, 0x01, 0x01, 0x00, 0x03},
};

static const WindowCellRecord sPromptWindowCells[5] = {
    {"/Game/2D/union_b/union_b_obj_window_nce.bin", 10, 0x7800, 1, 0, 0, {10, 10, 10, 0}},
    {"/Game/2D/union_b/union_b_obj_window_nce.bin", 3, 0x7800, 1, 0, 0, {3, 5, 4, 0}},
    {"/Game/2D/union_b/union_b_obj_window_nce.bin", 6, 0x7800, 1, 0, 0, {6, 8, 7, 0}},
    {"/Game/2D/union_b/union_b_obj_window_nce.bin", 0, 0x7800, 1, 0, 0, {0, 2, 1, 0}},
    {"/Game/2D/union_b/union_b_obj_window_nce.bin", 11, 0x7800, 1, 0, 0, {11, 11, 11, 0}},
};

static const AreaCursorAndBakuganOrder sAreaCursorAndBakuganOrder = {
    {
        {0, WINDOW_ITEM_NO_SLIDE_OUT, 0, 33, 256, 30, 0, {-1, -1, -1, -1}, 0, WINDOW_TOUCH_RELEASE},
        {1, WINDOW_ITEM_NO_SLIDE_OUT, 0, 69, 256, 90, 0, {-1, -1, -1, -1}, 0, WINDOW_TOUCH_RELEASE},
    },
    {
        0x17, 0x1d, 0x1a, 0x20, 0x18, 0x1e, 0x1e, 0x19, 0x1f, 0x1c, 0x22, 0x1b, 0x21, 0x11, 0x14, 0x13,
        0x12, 0x15, 0x16, 0x01, 0x02, 0x03, 0x00, 0x04, 0x08, 0x0e, 0x10, 0x09, 0x0b, 0x0c, 0x0f, 0x0a,
        0x06, 0x07, 0x05, 0x0d, 0x23, 0x25, 0x24, 0x26, 0x28,
    },
};

static const BakuganPriceEntry sBakuganPriceEntries[41] = {
    {4800, 40, 0},
    {6700, 50, 0},
    {4400, 40, 0},
    {6200, 50, 0},
    {4400, 38, 0},
    {5800, 48, 0},
    {5800, 48, 0},
    {4800, 40, 0},
    {6200, 50, 0},
    {4800, 40, 0},
    {6700, 50, 0},
    {5700, 45, 0},
    {7200, 55, 0},
    {3800, 38, 0},
    {4000, 39, 0},
    {4200, 39, 0},
    {4200, 39, 0},
    {4000, 39, 0},
    {4000, 39, 0},
    {120, 20, 0},
    {120, 20, 0},
    {130, 21, 0},
    {110, 19, 0},
    {170, 21, 0},
    {320, 23, 0},
    {1200, 30, 0},
    {3100, 37, 0},
    {330, 24, 0},
    {730, 27, 0},
    {810, 27, 0},
    {1760, 32, 0},
    {400, 25, 0},
    {0, 30, 0},
    {0, 30, 0},
    {2520, 36, 0},
    {1320, 30, 0},
    {5000, 42, 0},
    {8000, 65, 0},
    {5400, 42, 0},
    {8400, 65, 0},
    {0, 41, 0},
};

static const WindowItemRecord sPageAndRowCursorItems[8] = {
    {0, WINDOW_ITEM_NO_SLIDE_OUT, 25, 33, 30, 30, 0, {-1, -1, -1, -1}, 0, WINDOW_TOUCH_RELEASE},
    {1, WINDOW_ITEM_NO_SLIDE_OUT, 69, 33, 30, 30, 0, {-1, -1, -1, -1}, 0, WINDOW_TOUCH_RELEASE},
    {2, WINDOW_ITEM_NO_SLIDE_OUT, 113, 33, 30, 30, 0, {-1, -1, -1, -1}, 0, WINDOW_TOUCH_RELEASE},
    {3, WINDOW_ITEM_NO_SLIDE_OUT, 157, 33, 30, 30, 0, {-1, -1, -1, -1}, 0, WINDOW_TOUCH_RELEASE},
    {4, WINDOW_ITEM_NO_SLIDE_OUT, 201, 33, 30, 30, 0, {-1, -1, -1, -1}, 0, WINDOW_TOUCH_RELEASE},
    {5, WINDOW_ITEM_NO_SLIDE_OUT, 25, 69, 206, 30, 0, {-1, -1, -1, -1}, 0, WINDOW_TOUCH_RELEASE},
    {6, WINDOW_ITEM_NO_SLIDE_OUT, 25, 101, 206, 30, 0, {-1, -1, -1, -1}, 0, WINDOW_TOUCH_RELEASE},
    {7, WINDOW_ITEM_NO_SLIDE_OUT, 25, 133, 206, 30, 0, {-1, -1, -1, -1}, 0, WINDOW_TOUCH_RELEASE},
};

static const WindowItemRecord sPromptWindowItems[5] = {
    {0, WINDOW_ITEM_CURSOR_SKIP, 120, 18, 0, 0, 0, {-1, -1, -1, -1}, 0, 0},
    {1, WINDOW_ITEM_CURSOR_SKIP, 45, 152, 78, 20, 0, {1, 1, 2, 2}, 0, WINDOW_TOUCH_RELEASE_IF_SELECTED},
    {2, WINDOW_ITEM_CURSOR_SKIP, 133, 152, 78, 20, 0, {2, 2, 1, 1}, 0, WINDOW_TOUCH_RELEASE_IF_SELECTED},
    {3, WINDOW_ITEM_CURSOR_SKIP, 89, 152, 78, 20, 0, {3, 3, 3, 3}, 0, WINDOW_TOUCH_RELEASE_IF_SELECTED},
    {4, WINDOW_ITEM_CURSOR_SKIP, 128, 96, 0, 0, 0, {-1, -1, -1, -1}, 0, 0},
};

static const WindowItemRecord sDeckWindowItems[2] = {
    {0, WINDOW_ITEM_NO_SLIDE_OUT, 48, 68, 160, 24, 0, {1, 1, 0, 0}, 0, WINDOW_TOUCH_RELEASE_IF_SELECTED},
    {1, WINDOW_ITEM_NO_SLIDE_OUT, 48, 100, 160, 24, 0, {0, 0, 1, 1}, 0, WINDOW_TOUCH_RELEASE_IF_SELECTED},
};

static const u8 sBakuganAttributeFlags[0x29][6] = {
    {0x01, 0x00, 0x00, 0x00, 0x00, 0x00},
    {0x01, 0x00, 0x00, 0x00, 0x00, 0x00},
    {0x00, 0x00, 0x00, 0x01, 0x00, 0x00},
    {0x00, 0x00, 0x00, 0x01, 0x00, 0x00},
    {0x00, 0x01, 0x00, 0x00, 0x00, 0x00},
    {0x00, 0x01, 0x00, 0x00, 0x00, 0x00},
    {0x00, 0x00, 0x00, 0x00, 0x00, 0x00},
    {0x00, 0x00, 0x01, 0x00, 0x00, 0x00},
    {0x00, 0x00, 0x01, 0x00, 0x00, 0x00},
    {0x00, 0x00, 0x00, 0x00, 0x00, 0x01},
    {0x00, 0x00, 0x00, 0x00, 0x00, 0x01},
    {0x00, 0x00, 0x00, 0x00, 0x01, 0x00},
    {0x00, 0x00, 0x00, 0x00, 0x01, 0x00},
    {0x00, 0x01, 0x01, 0x01, 0x01, 0x01},
    {0x00, 0x00, 0x01, 0x00, 0x00, 0x00},
    {0x00, 0x01, 0x00, 0x00, 0x00, 0x00},
    {0x01, 0x00, 0x00, 0x00, 0x00, 0x00},
    {0x00, 0x00, 0x00, 0x01, 0x00, 0x00},
    {0x00, 0x00, 0x00, 0x00, 0x00, 0x01},
    {0x01, 0x01, 0x01, 0x01, 0x01, 0x01},
    {0x01, 0x01, 0x01, 0x01, 0x01, 0x01},
    {0x01, 0x01, 0x01, 0x01, 0x01, 0x01},
    {0x01, 0x01, 0x01, 0x01, 0x01, 0x01},
    {0x01, 0x01, 0x01, 0x01, 0x01, 0x01},
    {0x01, 0x01, 0x01, 0x01, 0x01, 0x01},
    {0x01, 0x01, 0x01, 0x01, 0x01, 0x01},
    {0x01, 0x01, 0x01, 0x01, 0x01, 0x01},
    {0x01, 0x01, 0x01, 0x01, 0x01, 0x01},
    {0x01, 0x01, 0x01, 0x01, 0x01, 0x01},
    {0x01, 0x01, 0x01, 0x01, 0x01, 0x01},
    {0x01, 0x01, 0x01, 0x01, 0x01, 0x00},
    {0x01, 0x01, 0x01, 0x00, 0x01, 0x01},
    {0x00, 0x00, 0x01, 0x01, 0x00, 0x01},
    {0x01, 0x00, 0x00, 0x01, 0x00, 0x01},
    {0x01, 0x00, 0x00, 0x00, 0x00, 0x00},
    {0x01, 0x00, 0x00, 0x01, 0x00, 0x01},
    {0x01, 0x01, 0x01, 0x01, 0x01, 0x01},
    {0x01, 0x01, 0x01, 0x01, 0x01, 0x01},
    {0x00, 0x00, 0x00, 0x00, 0x01, 0x00},
    {0x00, 0x00, 0x00, 0x00, 0x01, 0x00},
    {0x00, 0x00, 0x00, 0x00, 0x00, 0x00},
};

// The two halfword tables are sections of their own in the original (after the byte table above). Function-local
// statics of inline accessors get their own sections and keep their place in the size sort.
inline const u16 *Table_020a39fc() {
    static const u16 sItemPrices[214] = {
        1000, 5000, 10000, 5000, 10000, 5000, 10000, 5000, 10000, 5000, 10000, 5000,
        10000, 4000, 4000, 4000, 4000, 4000, 4000, 250, 250, 250, 250, 250,
        350, 600, 1200, 350, 450, 450, 900, 350, 450, 600, 900, 600,
        3000, 10000, 15000, 25000, 200, 200, 200, 200, 200, 200, 560, 280,
        400, 280, 560, 1150, 400, 2000, 1150, 890, 280, 890, 280, 400,
        560, 2000, 2000, 560, 2000, 890, 2000, 400, 560, 2000, 2000, 560,
        300, 2500, 2500, 2500, 660, 300, 5000, 2500, 7500, 2500, 2500, 2500,
        2500, 300, 1020, 2000, 1500, 2000, 420, 660, 420, 300, 1500, 1020,
        2000, 660, 300, 1020, 5000, 420, 300, 10000, 3000, 1000, 1600, 4100,
        5000, 2600, 750, 7500, 3000, 1000, 1600, 4100, 3200, 2600, 750, 7500,
        500, 750, 1100, 1700, 2200, 2700, 250, 5000, 4500, 1500, 2200, 4500,
        3500, 3500, 1000, 10000, 500, 750, 750, 750, 500, 5000, 3000, 500,
        500, 500, 2500, 5000, 2500, 5000, 2500, 2500, 5000, 2500, 5000, 2500,
        5000, 2500, 2500, 2500, 2500, 2500, 5000, 10000, 5000, 10000, 5000, 2500,
        300, 300, 300, 300, 1000, 1000, 1000, 600, 900, 2000, 1000, 600,
        1000, 1200, 1000, 1000, 900, 1000, 1000, 1000, 1000, 1000, 1200, 900,
        450, 1200, 2000, 2000, 450, 900, 1200, 450, 600, 1000, 1000, 1000,
        1000, 1000, 1000, 1000, 1000, 1000, 1000, 1000, 1000, 0,
    };
    return sItemPrices;
}

static const WindowCellRecord sDeckWindowCells[2] = {
    {"/Game/Deck/select_deck_b_obj_01_nce.bin", 0, 0x1c000, 6, 0, 1, {0, 2, 1, 0}},
    {"/Game/Deck/select_deck_b_obj_01_nce.bin", 3, 0x1c000, 6, 0, 1, {3, 5, 4, 0}},
};

inline const u16 *Table_020a3ba8() {
    static const u16 sCardOrder[214] = {
        0, 22, 23, 31, 32, 25, 26, 28, 29, 37, 38, 34, 35, 36, 30, 27,
        24, 33, 39, 2, 3, 4, 1, 5, 9, 15, 17, 10, 12, 13, 16, 11,
        7, 8, 6, 14, 18, 19, 20, 21, 101, 102, 103, 104, 105, 106, 116, 107,
        111, 110, 117, 125, 112, 129, 126, 122, 108, 123, 109, 114, 121, 130, 131, 118,
        127, 124, 128, 115, 119, 132, 133, 120, 204, 222, 223, 226, 212, 205, 221, 232,
        227, 228, 225, 231, 224, 201, 215, 220, 217, 218, 207, 211, 208, 202, 216, 214,
        219, 210, 206, 213, 229, 209, 203, 230, 318, 306, 310, 322, 330, 314, 301, 326,
        319, 307, 311, 323, 331, 315, 303, 327, 305, 309, 313, 317, 321, 325, 304, 328,
        320, 308, 312, 324, 332, 316, 302, 329, 408, 405, 406, 407, 404, 410, 409, 401,
        402, 403, 415, 416, 424, 425, 418, 421, 422, 430, 431, 427, 428, 423, 419, 417,
        426, 432, 411, 412, 413, 414, 420, 429, 501, 502, 503, 504, 526, 530, 524, 508,
        511, 519, 527, 509, 531, 517, 533, 523, 512, 525, 522, 528, 529, 532, 515, 514,
        506, 516, 520, 521, 505, 513, 518, 507, 510, 601, 602, 603, 604, 701, 702, 703,
        704, 801, 802, 803, 804, 0,
    };
    return sCardOrder;
}

// Ten-byte records (0x020a3860); GetPrice reads the first halfword. As a function-local static of an inline
// accessor the table gets a section of its own, so the table after it starts on a 4-byte boundary as in the original.
struct UnkStruct_020a3860 {
    /* 0 */ u16 mPriceTens;
    /* 2 */ u8 mUnk_02;
    /* 3 */ u8 mUnk_03;
    /* 4 */ u8 mUnk_04[6];
    /* a */
};
inline const UnkStruct_020a3860 *Table_020a3860() {
    static const UnkStruct_020a3860 data_020a3860[41] = {
        {3800, 42, 0, {3, 1, 1, 0, 2, 0}},
        {5600, 52, 0, {3, 2, 2, 0, 2, 0}},
        {3500, 42, 0, {2, 1, 0, 1, 2, 0}},
        {5200, 52, 0, {3, 2, 0, 1, 2, 0}},
        {3500, 38, 0, {1, 1, 3, 2, 0, 0}},
        {4800, 48, 0, {0, 1, 3, 2, 2, 0}},
        {4800, 48, 0, {0, 1, 3, 2, 2, 0}},
        {3800, 42, 0, {2, 3, 2, 0, 0, 0}},
        {5200, 52, 0, {3, 3, 2, 0, 0, 0}},
        {3800, 42, 0, {1, 0, 2, 3, 1, 0}},
        {5600, 52, 0, {2, 0, 3, 3, 1, 0}},
        {4700, 47, 0, {2, 2, 0, 3, 1, 0}},
        {6000, 57, 0, {3, 2, 1, 3, 1, 0}},
        {4200, 34, 0, {2, 1, 3, 2, 0, 0}},
        {3550, 39, 2, {1, 3, 1, 2, 2, 0}},
        {3850, 39, 2, {1, 2, 3, 3, 1, 0}},
        {3850, 39, 2, {2, 3, 1, 1, 3, 0}},
        {3550, 39, 2, {2, 2, 3, 0, 2, 0}},
        {3550, 39, 2, {3, 0, 2, 3, 1, 0}},
        {120, 20, 0, {0, 1, 0, 0, 0, 0}},
        {120, 20, 0, {0, 0, 1, 0, 0, 0}},
        {130, 21, 0, {0, 0, 0, 1, 0, 0}},
        {110, 19, 0, {1, 0, 0, 0, 0, 0}},
        {170, 21, 0, {2, 0, 0, 0, 1, 0}},
        {320, 23, 1, {1, 0, 0, 0, 1, 0}},
        {1200, 30, 3, {2, 1, 2, 1, 1, 0}},
        {3100, 37, 5, {3, 2, 1, 2, 2, 0}},
        {330, 24, 1, {0, 1, 0, 1, 0, 0}},
        {730, 27, 2, {2, 0, 0, 1, 1, 0}},
        {810, 27, 2, {2, 2, 0, 0, 1, 0}},
        {1760, 32, 4, {1, 3, 1, 1, 2, 0}},
        {400, 25, 1, {1, 0, 1, 1, 0, 0}},
        {0, 30, 0, {0, 0, 0, 1, 0, 0}},
        {0, 30, 0, {1, 0, 0, 1, 0, 0}},
        {2520, 36, 4, {2, 2, 2, 2, 1, 0}},
        {1320, 30, 3, {1, 0, 3, 2, 0, 0}},
        {1000, 42, 0, {2, 1, 1, 2, 1, 0}},
        {8000, 60, 0, {2, 1, 2, 3, 1, 0}},
        {4350, 42, 0, {1, 3, 0, 3, 1, 0}},
        {7200, 60, 0, {1, 3, 1, 3, 2, 0}},
        {0, 41, 0, {3, 1, 1, 0, 3, 0}},
    };
    return data_020a3860;
}

#include "System/SoundManager.hpp"
#include "Text/TextManager.hpp"
#include "System/InputManager.hpp"
#include "Graphics/SpriteManager.hpp"

extern u8 data_020d43db[];
void operator delete(void *ptr);
extern u8 data_020d4946[];
extern u8 gCharacterLook[];

class UnkStruct_02076058 {
public:
    /* 00 */ PAD(0x00, 0x78);
    /* 78 */ u8 mUnk_78;
    /* 79 */ PAD(0x79, 0x80);
    UnkStruct_02076058(u8 unk1);
    void SetBakugan(u32 a, u32 b, u32 c, u8 d);
};
BOOL func_02078270(UnkStruct_02076058 *obj);
u32 func_020779ec(void *obj);

void ListSceneScrollCallback(void *arg);

// Which screen a sprite or text goes to (passed to SpriteManager::Screen and the cell animation draws).
enum ScreenId { SCREEN_MAIN, SCREEN_SUB };


u8 GetSlotBakuganId(SaveData *d, u32 a, u32 b);



extern u8 gBakuganNumbers[];

static inline void PrintMessage(TextManager &tm, const u8 *data, u32 offset, u32 length, void *buffer, s16 x, s16 y) {
    tm.Print(data + offset, length, buffer, 0x18, 0x10, &y, &x, NULL, 0, 2, 0x44, 0x10, 0, 0, 0, 1, 1);
}


extern u8 data_020d4946[];

extern u8 data_020d4976[];

extern u8 data_020d497b[];

void SetSlotBakugan(SaveData *d, u32 a, u32 b, u32 c, u32 e, u32 f);

void SortListSceneItems(ListScene *self, u32 unk1);

void FillInitialEntry(ListScene *self, u32 unk1, u32 unk2, BattleCellPair *out);

static inline void PrintMessageF3(TextManager &tm, MessageFile *file, int index, void *buffer, s16 x, s16 y) {
    tm.Print(file->GetText(index), file->GetLength(index), buffer, 0x18, 0x10, &x, &y, NULL, 0, 2, 0x44, 0x10, 0, 0, 1, 1, 1);
}

static inline void PrintMessageF(TextManager &tm, MessageFile *file, int index, void *buffer, s16 x, s16 y) {
    tm.Print(file->GetText(index), file->GetLength(index), buffer, 0x18, 0x10, &x, &y, NULL, 0, 2, 0x44, 0x10, 0, 0, 0, 1, 1);
}

static inline void PrintMessageWidth(TextManager &tm, const u8 *data, u32 offset, u32 length, void *buffer, s16 y, s16 x, u16 *width) {
    tm.Print(data + offset, length, buffer, 0x18, 0x10, &x, &y, width, 0, 2, 0x44, 0, 0, 0, 0, 1, 1);
}

static inline u16 *MessageEntry(void *base, u32 index) {
    return (u16 *) ((u8 *) base + index * 4);
}

void PrintListSceneMainScreenText(ListScene *self);

static inline void PrintMessageB(TextManager &tm, MessageFile *file, int index, void *buffer, s16 x, s16 y) {
    tm.Print(file->GetText(index), file->GetLength(index), buffer, 0x18, 0x10, &x, &y, NULL, 0, 2, 0x44, 0, 0, 0, 1, 1, 1);
}

void PrintListSceneSubScreenText(ListScene *self);






u32 GetBakuganTableIndex(ListScene *self, u32 unk1, u32 unk2);

extern "C" void MIi_CpuCopy16(const void *src, void *dst, u32 size);

// Inline wrapper around MIi_CpuCopy16 (MIi_CpuCopy16).
static inline void MI_CpuCopy16(const void *src, void *dst, u32 size) {
    MIi_CpuCopy16(src, dst, size);
}

// Layout of the list message of a battle player (BattlePlayer::mMsg) and of the communication buffer gSendMessage.

struct BattleListMessage {
    /* 00 */ u8 mUnk_00;
    /* 01 */ PAD(0x01, 0x04);
    /* 04 */ u16 mUnk_04[6]; // each record's BattleRecord::mUnk_04 (func_020231f4 of the slot for mCharacterId 0x15)
    /* 10 */ u8 mBakuganIds[6];
    /* 16 */ u8 mAttributes[6];
    /* 1c */ u8 mRecordCount;
};

BOOL SetMessageTypeAndCheckAll(BattlePlayerList *list, u32 id);

#define MESSAGE ((BattleListMessage *) &gSendMessage)

// Layout of the slot message (type 4) of a battle player and of the communication buffer.

struct BattleSlotMessage {
    /* 00 */ u8 mUnk_00;
    /* 01 */ PAD(0x01, 0x04);
    /* 04 */ u16 mSlots54Values[6];
    /* 10 */ u16 mSlots6cValues[6];
    /* 1c */ u8 mSlots54Count;
    /* 1d */ u8 mSlots6cCount;
};

#define SLOT_MESSAGE ((BattleSlotMessage *) &gSendMessage)

u8 GetSlotBakuganId(SaveData *d, u32 a, u32 b);

void FillPickListWithShuffledCards(ListScene *self, u32 unk1);

void ShufflePickList(ListScene *self);

void PickSlot3Card(ListScene *self);

void PickSlot6Card(ListScene *self);

void PickSlot7Card(ListScene *self);

void PickSlot8Card(ListScene *self);

void SortPickListByAttributeValue(ListScene *self);

BOOL CardMatchesAttribute(ListScene *self, u32 unk1, u32 unk2);

BOOL CardMatchesBakuganIdAndAttribute(ListScene *self, u32 unk1, u32 unk2, u32 unk3);

BOOL CardMatchesSlotAttributes(ListScene *self, u32 unk1);

BOOL ListSceneHasAttribute(ListScene *self, u32 unk1);








u8 GetCardAttributeValue(u32 id, u32 index);

void ListSceneScrollCallback(void *arg);
void PrintListSceneMainScreenText(ListScene *self);
void SetupListTextKind(ListScene *self, u32 mode);
void PrintListSceneSubScreenText(ListScene *self);
void ResetCursorBakuganEntry(ListScene *self);
void func_020843ac(ListScene *self, u32 unk1);
void CopySlotsToFirstPlayer(ListScene *self);
BOOL ExchangeRecords(ListScene *self);
BOOL ExchangeSlots(ListScene *self);
BOOL ExchangeRandomSeed(ListScene *self); // self is passed but unused
BOOL ExchangeCharacterLooks(ListScene *self); // self is passed but unused
BOOL ExchangeRecordHeads(ListScene *self);
void SetupDeckTextKind(ListScene *self, u32 mode);
void PickSlots(ListScene *self);
void PickSlot3Card(ListScene *self);
BOOL CardMatchesBakuganId(ListScene *self, u32 unk1, u32 unk2);
void PickSlot6Card(ListScene *self);
BOOL CardMatchesAttribute(ListScene *self, u32 unk1, u32 unk2);
void PickSlot7Card(ListScene *self);
BOOL CardMatchesBakuganIdAndAttribute(ListScene *self, u32 unk1, u32 unk2, u32 unk3);
void PickSlot8Card(ListScene *self);
BOOL CardMatchesSlotAttributes(ListScene *self, u32 unk1);
BOOL ListSceneHasAttribute(ListScene *self, u32 unk1);
void FillInitialEntry(ListScene *self, u32 unk1, u32 unk2, BattleCellPair *out);
u32 GetBakuganTableIndex(ListScene *self, u32 unk1, u32 unk2);
void SortListSceneItems(ListScene *self, u32 unk1);
void FillPickListWithShuffledCards(ListScene *self, u32 unk1);
void ShufflePickList(ListScene *self);
void SortPickListByAttributeValue(ListScene *self);


ListScene::ListScene(u32 unk1, u32 unk2) :
    SceneBase(0x3e8, 0) {
    mStorecommentMessages   = NULL;
    mBakuganNameShortMessages   = NULL;
    mBakuganNameMessages   = NULL;
    mCardNameShortMessages   = NULL;
    mDeckSelectMessages   = NULL;
    mResultMessages   = NULL;
    mPromptWindow   = NULL;
    mListWindow   = NULL;
    mDeckWindow   = NULL;
    mPageAndRowCursor   = NULL;
    mAreaCursor   = NULL;
    mCommandList   = NULL;
    mCommandPage   = NULL;
    mMode   = unk1;
    mState   = 0;
    mListWindowResult   = 0;
    mUnk_1e0  = 0;
    mUnk_1e4  = 0;
    mAntenna  = NULL;
    mMessageWindow  = NULL;
    mPlayer  = unk2;
    mFirstSlotOnly  = 0;
    mAppliedWndMode  = 0;
    mWndMode  = 0;
    mListWindowCell0AnimTimer  = 0;
    mMoveY  = 0;
    mMoveYPerFrame  = 0;
    mItemMoving  = 0;
    mCommandListClosed  = 0;
    mCommandPageClosed  = 0;
    mPendingPlanes  = -1;
    SpriteManager::Get().SetFade(FADE_MODE_HOLD, -16);
    mCharacterId = mMode == LIST_MODE_BATTLE_EDIT ? data_020d43db[mPlayer] : 0x15;
    for (u8 i = 0; i < 2; i++) {
        mTextBuffers[i] = NULL;
        mGlyphBuffers[i] = NULL;
        mTextBufferSizes[i] = NULL;
        mGlyphCounts[i] = NULL;
    }
    for (u8 i = 0; i < 6; i++) {
        mMainScreenCellAnims[i] = NULL;
    }
    for (u8 i = 0; i < 0x13; i++) {
        mSubScreenCellAnims[i] = NULL;
    }
    mFirstPage = 0;
    if (mCharacterId != 0x15) {
        mFirstPage = 1;
    }
    mPageSlideX = 0;
    mPage = 0;
    mTargetPage = 0;
    mCursorPage = mPage;
    mPageCount = 0;
    mPageScroll = 0;
    mPageSliding = 0;
    mSelectingPage = 1;
    mListInputState = 0;
    mWndMode = 2;
    mRowSlideY  = 0;
    mCursorItem  = 0;
    mTargetItem  = 0;
    mItemCount  = 0;
    mScroll  = 0;
    mListScrolling = 0;
    mListFilter = 0;
    for (u8 i = 0; i < 0x40; i++) {
        mItemIds[i] = 0;
    }
    for (u8 i = 0; i < 13; i++) {
        mSavedCursorItems[i] = 0;
    }
    for (u8 i = 0; i < 3; i++) {
        mDirtyFlags[i] = 1;
    }
    mMainScreenState = 4;
    mTextKind = LIST_TEXT_KIND_NONE;
    mPrice = 0;
    MI_CpuClear16(&mAlphabetChar, 2);
    mFirstSlotOnly = gGameData.mLastBattle;
}

ListScene::~ListScene() {
    u8 i;
    for (i = 0; i < 6; i++) {
        delete mMainScreenCellAnims[i];
    }
    for (i = 0; i < 0x13; i++) {
        delete mSubScreenCellAnims[i];
    }
    delete mListWindow;
    delete mPromptWindow;
    delete mDeckWindow;
    operator delete(mPageAndRowCursor);
    operator delete(mAreaCursor);
    if (mStorecommentMessages != NULL) {
        OS_Free(mStorecommentMessages);
        mStorecommentMessages = NULL;
    }
    if (mBakuganNameShortMessages != NULL) {
        OS_Free(mBakuganNameShortMessages);
        mBakuganNameShortMessages = NULL;
    }
    if (mBakuganNameMessages != NULL) {
        OS_Free(mBakuganNameMessages);
        mBakuganNameMessages = NULL;
    }
    if (mCardNameShortMessages != NULL) {
        OS_Free(mCardNameShortMessages);
        mCardNameShortMessages = NULL;
    }
    if (mDeckSelectMessages != NULL) {
        OS_Free(mDeckSelectMessages);
        mDeckSelectMessages = NULL;
    }
    if (mResultMessages != NULL) {
        OS_Free(mResultMessages);
        mResultMessages = NULL;
    }
    for (u8 i = 0; i < 2; i++) {
        if (mTextBuffers[i] != NULL) {
            OS_Free(mTextBuffers[i]);
            mTextBuffers[i] = NULL;
        }
        if (mGlyphBuffers[i] != NULL) {
            OS_Free(mGlyphBuffers[i]);
            mGlyphBuffers[i] = NULL;
        }
    }
    SpriteManager::Get().SetFade(FADE_MODE_HOLD, -16);
    if (gGameData.mWireless != 0) {
        mAntenna->mFinished = 1;
    }
    delete mMessageWindow;
    mWndMode = 0;
    ApplyWndMode();
}

// Frees what LoadSubScreenGraphics, LoadMessages and InitMainScreenText built; in wireless mode also sets
// mAntenna->mUnk_04 = 1; turns the sub-screen windows off (mWndMode 0). With releaseCommands it also closes
// mCommandList / mCommandPage (those not already closed) and writes mContext->mRequest.
void ListScene::Release(u32 releaseCommands) {
    for (u8 i = 0; i < 6; i++) {
        delete mMainScreenCellAnims[i];
        mMainScreenCellAnims[i] = NULL;
    }
    for (u8 i = 0; i < 0x13; i++) {
        delete mSubScreenCellAnims[i];
        mSubScreenCellAnims[i] = NULL;
    }
    delete mListWindow;
    mListWindow = NULL;
    delete mPromptWindow;
    mPromptWindow = NULL;
    delete mDeckWindow;
    mDeckWindow = NULL;
    operator delete(mPageAndRowCursor);
    mPageAndRowCursor = NULL;
    operator delete(mAreaCursor);
    mAreaCursor = NULL;
    if (mStorecommentMessages != NULL) {
        OS_Free(mStorecommentMessages);
        mStorecommentMessages = NULL;
    }
    if (mBakuganNameShortMessages != NULL) {
        OS_Free(mBakuganNameShortMessages);
        mBakuganNameShortMessages = NULL;
    }
    if (mBakuganNameMessages != NULL) {
        OS_Free(mBakuganNameMessages);
        mBakuganNameMessages = NULL;
    }
    if (mCardNameShortMessages != NULL) {
        OS_Free(mCardNameShortMessages);
        mCardNameShortMessages = NULL;
    }
    if (mDeckSelectMessages != NULL) {
        OS_Free(mDeckSelectMessages);
        mDeckSelectMessages = NULL;
    }
    if (mResultMessages != NULL) {
        OS_Free(mResultMessages);
        mResultMessages = NULL;
    }
    for (u8 i = 0; i < 2; i++) {
        if (mTextBuffers[i] != NULL) {
            OS_Free(mTextBuffers[i]);
            mTextBuffers[i] = NULL;
        }
        if (mGlyphBuffers[i] != NULL) {
            OS_Free(mGlyphBuffers[i]);
            mGlyphBuffers[i] = NULL;
        }
    }
    if (gGameData.mWireless != 0) {
        mAntenna->mFinished = 1;
    }
    delete mMessageWindow;
    mMessageWindow = NULL;
    if (releaseCommands != 0) {
        if (mCommandListClosed == 0 && mCommandPageClosed == 0) {
            mContext->mRequest = 1;
        }
        if (mCommandList != NULL && mCommandListClosed == 0) {
            CloseCommandList(mCommandList);
            mCommandList           = NULL;
            mContext->mRequest = 0;
        }
        if (mCommandPage != NULL && mCommandPageClosed == 0) {
            CloseCommandPage(mCommandPage, 1);
            mCommandPage           = NULL;
            mContext->mRequest = 0;
        }
    }
    mWndMode = 0;
    ApplyWndMode();
}

#define UPDATE_TAIL()                      \
    DrawMainScreen();                       \
    mSubScreenCellAnims[18]->Update();                 \
    mSubScreenCellAnims[18]->Draw(1, 0x80, 0x60, 0);   \
    return

void ListScene::Update() {
    if (mPendingPlanes != -1) {
        GXS_SetVisiblePlane(mPendingPlanes);
        mPendingPlanes = -1;
        if (mState == 3) {
            if (mSelectingPage != 0) {
                mWndMode = 2;
            } else {
                mWndMode = 1;
            }
        }
    }
    if (mListWindowCell0AnimTimer != 0) {
        mListWindowCell0AnimTimer--;
        if (mListWindowCell0AnimTimer == 0 && mListWindow != NULL) {
            mListWindow->mCellAnims[0]->SetAnimation(2);
        }
    }
    if (mItemMoving != 0 && mMoveY == 0) {
        SoundManager::Get().StartSeqArc(0x66);
        mItemMoving = 0;
        u32 one = 1;
        if (mPage < 4) {
            mSlotValues[mPage - 1] = mItemIds[mCursorItem];
            mSlotValues[mPage + 8] = mListFilter;
            mSelectingPage              = one;
            mWndMode             = 2;
            mSavedCursorItems[mPage]    = mCursorItem;
            mDirtyFlags[0]          = one;
            mDirtyFlags[1]          = one;
            mDirtyFlags[2]          = one;
            RefreshCommandList();
            gGameData.mFlags |= 2;
        } else {
            mSlotValues[mPage - 1] = mItemIds[mCursorItem];
            mSelectingPage              = one;
            mWndMode             = 2;
            mSavedCursorItems[mPage]    = mCursorItem;
            mDirtyFlags[0]          = one;
            mDirtyFlags[1]          = one;
            mDirtyFlags[2]          = one;
            RefreshCommandPage();
            gGameData.mFlags |= 2;
        }
    }
    if (mMoveY > 0) {
        mMoveY -= mMoveYPerFrame;
        if (mMoveY <= 0) {
            mMoveY = 0;
        }
    }
    switch (mState) {
        case 0:
            InitDisplay();
            SetupPagesAndList();
            LoadSubScreenGraphics();
            LoadMessages();
            mState++;
            if (mMode == LIST_MODE_BATTLE_EDIT) {
                mState = 7;
                UpdateMainScreenAndDraw();
                return;
            }
            break;
        case 1:
            mPromptWindow->mEnabled = 0;
            mListWindow->mEnabled = 0;
            if (!IsCommandListOrPageSettingUp()) {
                SpriteManager::Get().StartFade(FADE_MODE_TO_ZERO);
                mState++;
            }
            break;
        case 2:
            if (!SpriteManager::Get().IsFading()) {
                mListWindow->mEnabled = 1;
                mState++;
            }
            break;
        case 3:
            UpdateListInput();
            break;
        case 4:
            if (!IsCommandListOrPageSettingUp()) {
                switch (mPromptWindow->Update(1)) {
                    case 1:
                        mState = 5;
                        break;
                    case 2:
                        mState = 6;
                        break;
                }
            }
            break;
        case 5:
            mPromptWindow->Update(1);
            if (mPromptWindow->mState == 4) {
                switch (mTextKind) {
                    case LIST_TEXT_KIND_PRICE:
                        switch (mMode) {
                            case LIST_MODE_STORE_BAKUGAN:
                                gSaveData[0].mMoney -= mPrice;
                                SetupListTextKind(this, LIST_TEXT_KIND_BAKUGAN_NAME);
                                break;
                            case LIST_MODE_STORE_GAUGE:
                                gSaveData[0].mMoney -= mPrice;
                                SetupListTextKind(this, LIST_TEXT_KIND_NONE);
                                mSavedCursorItems[mPage] = mCursorItem;
                                mState           = 0x21;
                                break;
                            case LIST_MODE_STORE_CARD:
                                gSaveData[0].mMoney -= mPrice;
                                SetupListTextKind(this, LIST_TEXT_KIND_CARD_NAME);
                                break;
                        }
                        SoundManager::Get().StartSeqArc(0x69);
                        break;
                    case 3:
                        SetupListTextKind(this, LIST_TEXT_KIND_PRICE);
                        RefreshCommandList();
                        break;
                    case LIST_TEXT_KIND_BAKUGAN_NAME:
                    case 5:
                        ResetCursorBakuganEntry(this);
                        SetupListTextKind(this, 10);
                        break;
                    case 10:
                        SetupListTextKind(this, LIST_TEXT_KIND_NONE);
                        mState = 0x26;
                        break;
                    case LIST_TEXT_KIND_CARD_NAME:
                        SetFlagBit(data_020d4946, mItemIds[mCursorItem]);
                        SetupListTextKind(this, LIST_TEXT_KIND_NONE);
                        break;
                    case LIST_TEXT_KIND_READ_SLOTS:
                        ReadSlots();
                        SetupListTextKind(this, LIST_TEXT_KIND_NONE);
                        SoundManager::Get().StartSeqArc(0x68);
                        break;
                    case LIST_TEXT_KIND_PICK_SLOTS:
                        PickSlots(this);
                        gGameData.mFlags |= 2;
                        SetupListTextKind(this, LIST_TEXT_KIND_NONE);
                        SoundManager::Get().StartSeqArc(0x67);
                        break;
                    default:
                        SetupListTextKind(this, LIST_TEXT_KIND_NONE);
                        break;
                }
            }
            break;
        case 6:
            mPromptWindow->Update(1);
            if (mPromptWindow->mState == 4) {
                SetupListTextKind(this, LIST_TEXT_KIND_NONE);
            }
            break;
        case 7:
            mPromptWindow->mEnabled = 0;
            mListWindow->mEnabled = 0;
            mDeckWindow->mEnabled = 0;
            SpriteManager::Get().StartFade(FADE_MODE_TO_ZERO);
            mState++;
            UpdateMainScreenAndDraw();
            return;
        case 8:
            if (!SpriteManager::Get().IsFading()) {
                mDeckWindow->mEnabled = 1;
                mState++;
                if (mFirstSlotOnly != 0) {
                    SetupDeckTextKind(this, 0xc);
                }
            }
            UpdateMainScreenAndDraw();
            return;
        case 9:
            if (!IsCommandListOrPageSettingUp()) {
                if (mDeckWindow->Update(1) == 3) {
                    switch (mDeckWindow->mCursor->mSelection) {
                        case 0:
                            StoreSlotsInSaveData();
                            SetupDeckTextKind(this, 0xb);
                            break;
                        case 1:
                            mPage = mFirstPage;
                            mTargetPage = mFirstPage;
                            mCursorPage = mFirstPage;
                            mPageScroll = mFirstPage;
                            if (mCharacterId != 0x15) {
                                mMainScreenState = 1;
                            }
                            mState++;
                            break;
                    }
                }
            }
            UpdateMainScreenAndDraw();
            return;
        case 10:
            SpriteManager::Get().StartFade(FADE_MODE_DOWN);
            mDeckWindow->mEnabled = 0;
            mState++;
            UpdateMainScreenAndDraw();
            return;
        case 11:
            if (!SpriteManager::Get().IsFading()) {
                mState++;
            }
            UpdateMainScreenAndDraw();
            return;
        case 12:
            if (IsCommandListOrPageSettingUp()) {
                return;
            }
            func_020843ac(this, 0);
            mState++;
            break;
        case 13:
            if (!IsCommandListOrPageSettingUp()) {
                SpriteManager::Get().StartFade(FADE_MODE_TO_ZERO);
                mState++;
            }
            break;
        case 14:
            if (!SpriteManager::Get().IsFading()) {
                mListWindow->mEnabled = 1;
                mState           = 3;
            }
            break;
        case 15:
            SpriteManager::Get().StartFade(FADE_MODE_DOWN);
            mListWindow->mEnabled = 0;
            mState++;
            break;
        case 16:
            if (!SpriteManager::Get().IsFading()) {
                mState++;
            }
            break;
        case 17:
            if (IsCommandListOrPageSettingUp()) {
                return;
            }
            func_020843ac(this, 1);
            mState++;
            UpdateMainScreenAndDraw();
            return;
        case 18:
            if (!IsCommandListOrPageSettingUp() && mMainScreenState == 0) {
                SpriteManager::Get().StartFade(FADE_MODE_TO_ZERO);
                mState++;
            }
            UpdateMainScreenAndDraw();
            return;
        case 19:
            if (!SpriteManager::Get().IsFading()) {
                mDeckWindow->mEnabled = 1;
                mState           = 9;
            }
            UpdateMainScreenAndDraw();
            return;
        case 20: {
            switch (mPromptWindow->Update(1)) {
                case 1:
                    mState = 0x15;
                    break;
                case 2:
                    mState = 0x16;
                    break;
            }
            UpdateMainScreenAndDraw();
            return;
        }
        case 21:
            mPromptWindow->Update(1);
            if (mPromptWindow->mState == 4) {
                switch (mTextKind) {
                    case 0xb:
                        SoundManager::Get().StartSeqArc(0x6a);
                        SetupDeckTextKind(this, LIST_TEXT_KIND_NONE);
                        mDeckWindow->mCellAnims[0]->SetAnimation(0);
                        mState = 0x17;
                        WirelessManager::Get();
                        if (IsWirelessMode(&data_020c4030.mUnk_228)) {
                            mDeckWindow->mEnabled = 0;
                            mState           = 0x1b;
                            UPDATE_TAIL();
                        }
                        break;
                    case 0xc:
                        SetupDeckTextKind(this, LIST_TEXT_KIND_NONE);
                        break;
                }
            }
            UpdateMainScreenAndDraw();
            return;
        case 22:
            mPromptWindow->Update(1);
            if (mPromptWindow->mState == 4) {
                SetupDeckTextKind(this, LIST_TEXT_KIND_NONE);
            }
            UpdateMainScreenAndDraw();
            return;
        case 23:
            mDeckWindow->mEnabled = 0;
            if (mCharacterId == 0x15) {
                CopySlotsToFirstPlayer(this);
            }
            MIi_CpuCopyFast(gCharacterLook, (u8 *) ((u32 *) gGameData.mBattlePlayers)[3] + 0xa8, 8);
            SpriteManager::Get().StartFade(FADE_MODE_DOWN);
            mState++;
            UpdateMainScreenAndDraw();
            return;
        case 24:
            if (!SpriteManager::Get().IsFading()) {
                Release(1);
                mState++;
                return;
            }
            UpdateMainScreenAndDraw();
            return;
        case 25:
            if (mContext->mRequest != 0) {
                mState++;
            }
            return;
        case 26:
            RequestScene(1);
            mFinished = 1;
            return;
        case 27:
            if (ExchangeRecords(this)) {
                mState++;
            }
            UPDATE_TAIL();
        case 28:
            if (ExchangeSlots(this)) {
                mState++;
            }
            UPDATE_TAIL();
        case 29:
            if (ExchangeRecordHeads(this)) {
                mState++;
            }
            UPDATE_TAIL();
        case 30:
            if (ExchangeCharacterLooks(this)) {
                mState++;
            }
            UPDATE_TAIL();
        case 31:
            if (ExchangeRandomSeed(this)) {
                SpriteManager::Get().StartFade(FADE_MODE_DOWN);
                mState++;
            }
            UPDATE_TAIL();
        case 32:
            if (!SpriteManager::Get().IsFading()) {
                Release(1);
                mState = 0x19;
                return;
            }
            UPDATE_TAIL();
        case 33:
            SpriteManager::Get().StartSubFade(FADE_MODE_DOWN);
            mState++;
            break;
        case 34:
            SpriteManager::Get();
            if (SpriteManager::sFade[1] <= FADE_MODE_HOLD) {
                if (mCommandList != NULL) {
                    mCommandList->SetWindowVisible(0);
                }
                Release(0);
                mUnk_1e0 = new UnkStruct_02076058(1);
                mUnk_1e0->SetBakugan(1, mItemIds[mCursorItem] & 0xff, mListFilter, 1);
                mContext->mRequest = 0;
                mState++;
                return;
            }
            break;
        case 35:
            if (func_02078270(mUnk_1e0)) {
                BakuganEntry *pair = GetBakuganEntry(&gSaveData[0], mItemIds[mCursorItem] & 0xff, mListFilter);
                u16 val = func_020231f4(&gSaveData[0], 0, mItemIds[mCursorItem] & 0xff, mListFilter);
                SetCommandListValues(mCommandList, mListFilter, val, (BattleCellPair *) pair);
            }
            if (func_020779ec(mUnk_1e0) == 0) {
                return;
            }
            if (mCommandList != NULL) {
                CloseCommandList(mCommandList);
            }
            mState++;
            return;
        case 36:
            if (mCommandList != NULL) {
                if (mContext->mRequest == 0) {
                    return;
                }
                mUnk_1e0->mUnk_78 = 1;
                mCommandList           = NULL;
            }
            mState++;
            return;
        case 37:
            if (mContext->mRequest != 3) {
                return;
            }
            mContext->mRequest = 0;
            mUnk_1e0          = NULL;
            mState           = 0;
            mSelectingPage           = 0;
            mMainScreenState          = 4;
            mDirtyFlags[0]       = 1;
            mDirtyFlags[1]       = 1;
            mDirtyFlags[2]       = 1;
            mWndMode          = 1;
            return;
        case 38:
            SpriteManager::Get().StartFade(FADE_MODE_DOWN);
            mState++;
            break;
        case 39:
            if (!SpriteManager::Get().IsFading()) {
                Release(1);
                mState++;
                return;
            }
            break;
        case 40:
            if (mContext->mRequest == 0) {
                return;
            }
            mUnk_1e4 = new UnkStruct_02071d24(2);
            mUnk_1e4->SetBakugan(0, mItemIds[mCursorItem] & 0xff, mListFilter, 1);
            mContext->mRequest = 0;
            mState++;
            return;
        case 41:
            if (mContext->mRequest != 2) {
                return;
            }
            mContext->mRequest = 0;
            mUnk_1e4          = NULL;
            mState           = 0;
            mSelectingPage           = 1;
            mMainScreenState          = 4;
            mDirtyFlags[0]       = 1;
            mDirtyFlags[1]       = 1;
            mDirtyFlags[2]       = 1;
            mWndMode          = 2;
            return;
        case 43:
            if (mMode == LIST_MODE_EDIT || mMode == LIST_MODE_BATTLE_EDIT) {
                StoreSlotsInSaveData();
            }
            SpriteManager::Get().StartFade(FADE_MODE_DOWN);
            mPromptWindow->mEnabled = 0;
            mListWindow->mEnabled = 0;
            mState++;
            break;
        case 44:
            if (!SpriteManager::Get().IsFading()) {
                Release(1);
                mState++;
                return;
            }
            break;
        case 45:
            if (mContext->mRequest != 0) {
                mState++;
            }
            return;
        case 46:
            RequestScene(1);
            mFinished = 1;
            return;
    }
    ApplyWndMode();
    UpdateMainScreen();
    UpdateListWindow();
    UpdatePages();
    UpdateListScroll();
    DrawMainScreen();
    DrawPromptWindowContents();
    DrawSubScreenText();
    DrawMoneyFilterButtonsAndCursor();
    DrawWindows();
    DrawPagesAndMovingItem();
    DrawRows();
}


void ListScene::InitDisplay() {
    GX_SetDispSelect(1);
    SpriteManager::Get().SetupMode(1, 1);
    GX_SetGraphicsMode(1, 0, 0);
    GXS_SetGraphicsMode(0);
    G2_SetBG0Control(0, 1, 0, 1, 0);
    G2_SetBG1Control(0, 1, 2, 4, 0);
    G2_SetBG2Control(0, 1, 4, 8, 0);
    G2_SetBG3Control(0, 1, 6, 0xc, 0);
    G2S_SetBG0Control(0, 1, 0, 4, 0);
    G2S_SetBG1Control(0, 1, 2, 1, 0);
    G2S_SetBG2Control(0, 1, 4, 4, 0);
    G2S_SetBG3Control(0, 1, 6, 4, 0);
    G2_SetBG0Offset(0, 0);
    G2_SetBG1Offset(0, 0);
    G2_SetBG2Offset(0, 0);
    G2_SetBG3Offset(0, 0);
    G2S_SetBG0Offset(0, 0);
    G2S_SetBG1Offset(0, 0);
    G2S_SetBG2Offset(0, 0x1e0);
    G2S_SetBG3Offset(0, 0);
    G2_SetBG0Priority(3);
    G2_SetBG1Priority(3);
    G2_SetBG2Priority(3);
    G2_SetBG3Priority(3);
    G2S_SetBG0Priority(3);
    G2S_SetBG1Priority(0);
    G2S_SetBG2Priority(2);
    G2S_SetBG3Priority(1);
    GX_SetVisiblePlane(0x11);
    if (mMode == LIST_MODE_GALLERY) {
        GXS_SetVisiblePlane(0x19);
    } else if (mMode == LIST_MODE_BATTLE_EDIT) {
        GXS_SetVisiblePlane(0x11);
    } else {
        GXS_SetVisiblePlane(0x1d);
    }
    G2_SetBlendBrightness(0x1f, 0);
    G2S_SetBlendBrightness(0x1f, 0);
    GX_SetVisibleWnd(0);
    G2_SetWndOutsidePlane(0x1f, FALSE);
    G2_SetWnd0InsidePlane(0x1f, FALSE);
    G2_SetWnd1InsidePlane(0x1f, FALSE);
    G2_SetWndOBJInsidePlane(0x1f, FALSE);
    G2_SetWnd0Position(0, 0, 0, 0);
    G2_SetWnd1Position(0, 0, 0, 0);
    GXS_SetVisibleWnd(0);
    G2S_SetWndOutsidePlane(0x1f, FALSE);
    G2S_SetWnd0InsidePlane(0x1f, FALSE);
    G2S_SetWnd1InsidePlane(0x1f, FALSE);
    G2S_SetWndOBJInsidePlane(0x1f, FALSE);
    G2S_SetWnd0Position(0, 0, 0, 0);
    G2S_SetWnd1Position(0, 0, 0, 0);
    G2_BlendNone();
    G2S_BlendNone();
}

void ListScene::SetupPagesAndList() {
    switch (mMode) {
        case LIST_MODE_STORE_BAKUGAN:
        case LIST_MODE_STORE_GAUGE:
            mPageCount = 6;
            break;
        case LIST_MODE_STORE_CARD:
            mPageCount = 6;
            break;
        case LIST_MODE_EDIT:
        case LIST_MODE_BATTLE_EDIT:
            mPageCount = 10;
            break;
        case LIST_MODE_GALLERY:
            mPageCount = 13;
            RefreshCounts();
            break;
    }
    ReadSlots();
    BuildList(1);
}


void ListScene::LoadSubScreenGraphics() {
    ChangeDir("/Game/2d/union_b/");
    LoadAndQueueUpload(1, UPLOAD_KIND_BG_EXT_PLTT, 0x2000, "union_b_bg_window_ncl.bin", 0, 0, 0);
    LoadAndQueueUpload(1, UPLOAD_KIND_BG1_CHAR, 0, "union_b_bg_window_ncg.bin", 0, 0, 0);
    LoadAndQueueUpload(1, UPLOAD_KIND_BG1_SCR, 0, "union_b_bg_window_b_nsc.bin", 0, 0, 0);
    switch (mMode) {
        case LIST_MODE_STORE_BAKUGAN:
        case LIST_MODE_STORE_GAUGE:
        case LIST_MODE_STORE_CARD:
            ChangeDir("/Game/List/");
            LoadAndQueueUpload(1, UPLOAD_KIND_BG_EXT_PLTT, 0, "list_store_b_bg_00_ncl.bin", 0, 0, 0);
            LoadAndQueueUpload(1, UPLOAD_KIND_BG_EXT_PLTT, 0x4000, "list_store_b_bg_00_ncl.bin", 0, 0, 0);
            LoadAndQueueUpload(1, UPLOAD_KIND_BG_EXT_PLTT, 0x6000, "list_store_b_bg_00_ncl.bin", 0, 0, 0);
            LoadAndQueueUpload(1, UPLOAD_KIND_BG0_CHAR, 0, "list_store_b_bg_00_ncg.bin", 0, 0, 0);
            LoadAndQueueUpload(1, UPLOAD_KIND_BG0_SCR, 0, "list_store_b_bg_00_nsc.bin", 0, 0, 0);
            LoadAndQueueUpload(1, UPLOAD_KIND_BG2_SCR, 0, "list_store_b_bg_01_nsc.bin", 0, 0, 0);
            LoadAndQueueUpload(1, UPLOAD_KIND_BG3_SCR, 0, "list_store_b_bg_02_nsc.bin", 0, 0, 0);
            break;
        case LIST_MODE_EDIT:
            ChangeDir("/Game/List/");
            LoadAndQueueUpload(1, UPLOAD_KIND_BG_EXT_PLTT, 0, "list_edit_b_bg_00_ncl.bin", 0, 0, 0);
            LoadAndQueueUpload(1, UPLOAD_KIND_BG_EXT_PLTT, 0x4000, "list_edit_b_bg_00_ncl.bin", 0, 0, 0);
            LoadAndQueueUpload(1, UPLOAD_KIND_BG_EXT_PLTT, 0x6000, "list_edit_b_bg_00_ncl.bin", 0, 0, 0);
            LoadAndQueueUpload(1, UPLOAD_KIND_BG0_CHAR, 0, "list_edit_b_bg_00_ncg.bin", 0, 0, 0);
            LoadAndQueueUpload(1, UPLOAD_KIND_BG0_SCR, 0, "list_edit_b_bg_00_nsc.bin", 0, 0, 0);
            LoadAndQueueUpload(1, UPLOAD_KIND_BG2_SCR, 0, "list_edit_b_bg_01_nsc.bin", 0, 0, 0);
            LoadAndQueueUpload(1, UPLOAD_KIND_BG3_SCR, 0, "list_edit_b_bg_02_nsc.bin", 0, 0, 0);
            break;
        case LIST_MODE_GALLERY:
            ChangeDir("/Game/List/");
            LoadAndQueueUpload(1, UPLOAD_KIND_BG_EXT_PLTT, 0, "list_gallery_b_bg_00_ncl.bin", 0, 0, 0);
            LoadAndQueueUpload(1, UPLOAD_KIND_BG_EXT_PLTT, 0x4000, "list_gallery_b_bg_00_ncl.bin", 0, 0, 0);
            LoadAndQueueUpload(1, UPLOAD_KIND_BG_EXT_PLTT, 0x6000, "list_gallery_b_bg_00_ncl.bin", 0, 0, 0);
            LoadAndQueueUpload(1, UPLOAD_KIND_BG0_CHAR, 0, "list_gallery_b_bg_00_ncg.bin", 0, 0, 0);
            LoadAndQueueUpload(1, UPLOAD_KIND_BG0_SCR, 0, "list_gallery_b_bg_00_nsc.bin", 0, 0, 0);
            LoadAndQueueUpload(1, UPLOAD_KIND_BG2_SCR, 0, "list_gallery_b_bg_01_nsc.bin", 0, 0, 0);
            LoadAndQueueUpload(1, UPLOAD_KIND_BG3_SCR, 0, "list_gallery_b_bg_02_nsc.bin", 0, 0, 0);
            break;
        case LIST_MODE_BATTLE_EDIT:
            ChangeDir("/Game/Battle/Bg/");
            LoadAndQueueUpload(1, UPLOAD_KIND_BG_EXT_PLTT, 0, "select_arena_b_bg_00_ncl.bin", 0, 0, 0);
            LoadAndQueueUpload(1, UPLOAD_KIND_BG0_CHAR, 0, "select_arena_b_bg_00_ncg.bin", 0, 0, 0);
            LoadAndQueueUpload(1, UPLOAD_KIND_BG0_SCR, 0, "select_arena_b_bg_00_nsc.bin", 0, 0, 0);
            break;
    }
    SpriteScreen &screen = SpriteManager::Get().Screen(1);
    screen.SetObjVramMode(3);
    ChangeDir("/Game/List/");
    screen.LoadCharacters("list_b_obj_00.nce_ncg1D.bin", 0xc000, 0);
    screen.LoadPalette("list_b_obj_00_ncl.bin", 0xc0, 0);
    for (u8 i = 0; i <= 12; i++) {
        mSubScreenCellAnims[i]            = new CellAnimation("list_b_obj_00_nce.bin", 0, 1, 1);
        mSubScreenCellAnims[i]->mCharBase = screen.GetCharBase(0xc000);
        mSubScreenCellAnims[i]->mPaletteBase   = 6;
    }
    mSubScreenCellAnims[0]->SetAnimation(0x23);
    mSubScreenCellAnims[1]->SetAnimation(0x24);
    mSubScreenCellAnims[2]->SetAnimation(0x22);
    mSubScreenCellAnims[3]->SetAnimation(0);
    mSubScreenCellAnims[4]->SetAnimation(1);
    mSubScreenCellAnims[5]->SetAnimation(2);
    mSubScreenCellAnims[6]->SetAnimation(3);
    mSubScreenCellAnims[7]->SetAnimation(0x10);
    mSubScreenCellAnims[8]->SetAnimation(0x10);
    mSubScreenCellAnims[9]->SetAnimation(0x1c);
    mSubScreenCellAnims[10]->SetAnimation(0x1d);
    mSubScreenCellAnims[11]->SetAnimation(0x1e);
    mSubScreenCellAnims[12]->SetAnimation(0x1f);
    ChangeDir("/Game/2d/union_b/");
    screen.LoadCharacters("union_b_obj_cockpit.nce_ncg1D.bin", 0x6000, 0);
    screen.LoadPalette("union_b_obj_cockpit_ncl.bin", 0, 0);
    screen.LoadCharacters("union_b_obj_window.nce_ncg1D.bin", 0x7800, 0);
    screen.LoadPalette("union_b_obj_window_ncl.bin", 0x20, 0);
    screen.LoadCharacters("union_b_obj_number.nce_ncg1D.bin", 0x9000, 0);
    screen.LoadPalette("union_b_obj_number_ncl.bin", 0x60, 0);
    screen.LoadCharacters("union_b_obj_gp.nce_ncg1D.bin", 0xa000, 0);
    screen.LoadPalette("union_b_obj_gp_ncl.bin", 0x80, 0);
    screen.LoadCharacters("union_b_obj_new.nce_ncg1D.bin", 0xb000, 0);
    screen.LoadPalette("union_b_obj_new_ncl.bin", 0xa0, 0);
    screen.LoadCharacters("union_b_obj_point.nce_ncg1D.bin", 0x1c000, 0);
    screen.LoadPalette("union_b_obj_point_ncl.bin", 0x60, 0);
    screen.LoadCharacters("union_b_obj_page.nce_ncg1D.bin", 0x1d000, 0);
    screen.LoadPalette("union_b_obj_page_ncl.bin", 0, 0);
    mSubScreenCellAnims[13]            = new CellAnimation("union_b_obj_number_nce.bin", 0, 1, 1);
    mSubScreenCellAnims[13]->mCharBase = screen.GetCharBase(0x9000);
    mSubScreenCellAnims[13]->mPaletteBase   = 3;
    mSubScreenCellAnims[13]->SetAnimation(0);
    mSubScreenCellAnims[15]            = new CellAnimation("union_b_obj_new_nce.bin", 0, 1, 1);
    mSubScreenCellAnims[15]->mCharBase = screen.GetCharBase(0xb000);
    mSubScreenCellAnims[15]->mPaletteBase   = 5;
    mSubScreenCellAnims[15]->SetAnimation(0);
    mSubScreenCellAnims[16]            = new CellAnimation("union_b_obj_gp_nce.bin", 0, 1, 1);
    mSubScreenCellAnims[16]->mCharBase = screen.GetCharBase(0xa000);
    mSubScreenCellAnims[16]->mPaletteBase   = 4;
    mSubScreenCellAnims[16]->SetAnimation(2);
    mSubScreenCellAnims[17]            = new CellAnimation("union_b_obj_point_nce.bin", 0, 1, 1);
    mSubScreenCellAnims[17]->mCharBase = screen.GetCharBase(0x1c000);
    mSubScreenCellAnims[17]->mPaletteBase   = 3;
    mSubScreenCellAnims[17]->SetAnimation(0);
    ChangeDir("/");
    mListWindow = new SelectWindow(7, sListWindowItems, -1);
    mListWindow->SetCells(7, sListWindowCells);
    mPromptWindow = new SelectWindow(5, sPromptWindowItems, 1);
    mPromptWindow->SetCells(5, sPromptWindowCells);
    mPageAndRowCursor = new SelectWindowCursor(8, sPageAndRowCursorItems, -1);
    mAreaCursor = new SelectWindowCursor(2, sAreaCursorAndBakuganOrder.mCursor, -1);
    if (mMode == LIST_MODE_BATTLE_EDIT) {
        ChangeDir("/Game/Deck/");
        screen.LoadCharacters("select_deck_b_obj_01.nce_ncg1D.bin", 0x1c000, 0);
        screen.LoadPalette("select_deck_b_obj_01_ncl.bin", 0xc0, 0);
        ChangeDir("/Game/Battle/Obj/");
        screen.LoadCharacters("battle_b_obj_wait.nce_ncg1D.bin", 0x1e000, 0);
        screen.LoadPalette("battle_b_obj_ability_ncl.bin", 0x100, 0);
        mSubScreenCellAnims[18]            = new CellAnimation("battle_b_obj_wait_nce.bin", 0, 1, 1);
        mSubScreenCellAnims[18]->mCharBase = screen.GetCharBase(0x1e000);
        mSubScreenCellAnims[18]->mPaletteBase   = 8;
        mSubScreenCellAnims[18]->SetAnimation(0);
        ChangeDir("/");
        mDeckWindow = new SelectWindow(2, sDeckWindowItems, 0);
        mDeckWindow->SetCells(2, sDeckWindowCells);
        mDeckWindow->mCellAnims[0]->SetAnimation(1);
        this->SetItemFlags(3, 0, WINDOW_ITEM_NO_SLIDE_OUT);
        this->SetItemFlags(3, 1, WINDOW_ITEM_NO_SLIDE_OUT);
        this->SetItemFlags(0, 0, WINDOW_ITEM_CURSOR_SKIP);
        this->SetItemFlags(0, 1, WINDOW_ITEM_CURSOR_SKIP);
        this->SetItemFlags(0, 2, WINDOW_ITEM_CURSOR_SKIP);
        this->SetItemFlags(0, 3, WINDOW_ITEM_CURSOR_SKIP);
        this->SetItemFlags(0, 4, WINDOW_ITEM_CURSOR_SKIP);
        this->SetItemFlags(0, 5, WINDOW_ITEM_CURSOR_SKIP);
        this->SetItemFlags(0, 6, WINDOW_ITEM_CURSOR_SKIP);
    }
    ChangeDir("/Game/Bakugan/mark");
    LoadAndQueueUpload(1, UPLOAD_KIND_OBJ_EXT_PLTT, 0x200, "bakugan_s_000_ncl.bin", 0, 0, 0);
    LoadAndQueueUpload(1, UPLOAD_KIND_OBJ_EXT_PLTT, 0, "bakugan_s_base_ncl.bin", 0, 0, 0);
    LoadAndQueueUpload(1, UPLOAD_KIND_OBJ, 0x12000, "bakugan_s_base.nce_ncg1D.bin", 0, 0, 0);
    mSubScreenCellAnims[14]            = new CellAnimation("bakugan_s_base_nce.bin", 0, 1, 1);
    mSubScreenCellAnims[14]->mCharBase = screen.GetCharBase(0x12000);
    mSubScreenCellAnims[14]->mPaletteBase   = 0;
    mSubScreenCellAnims[14]->SetAnimation(0);
    ChangeDir("/Game/Card/Graphic/Small");
    LoadAndQueueUpload(1, UPLOAD_KIND_OBJ_EXT_PLTT, 0x400, "card_s_000_ncl.bin", 0, 0, 0);
    if (gGameData.mWireless == 0) {
        return;
    }
    if (mAntenna != NULL) {
        ((AntennaIndicator *) mAntenna)->SetupLayout(3);
        return;
    }
    mAntenna = (UnkObject_02078e14 *) new AntennaIndicator(3);
}

void ListScene::LoadMessages() {
    SpriteManager::Get().Screen(1).SetObjVramMode(3);
    ChangeDir("/font/");
    mStorecommentMessages    = LoadFile("mes_storecomment.mes", 0, 0, 0, 0, 0);
    mBakuganNameShortMessages    = LoadFile("mes_BakuganNameShort.mes", 0, 0, 0, 0, 0);
    mBakuganNameMessages    = LoadFile("mes_BakuganName.mes", 0, 0, 0, 0, 0);
    mCardNameShortMessages    = LoadFile("mes_CardNameShort.mes", 0, 0, 0, 0, 0);
    mDeckSelectMessages    = LoadFile("mes_DeckSelect.mes", 0, 0, 0, 0, 0);
    mResultMessages    = LoadFile("mes_Result.mes", 0, 0, 0, 0, 0);
    mTextBufferSizes[0] = TextManager::Get().BufferSize(0x18, 0x10);
    mTextBuffers[0] = OS_Alloc(mTextBufferSizes[0]);
    MI_CpuClear16(mTextBuffers[0], mTextBufferSizes[0]);
    mGlyphCounts[0] = TextManager::Get().GlyphCount(6);
    mGlyphBuffers[0] = OS_Alloc((s16) mGlyphCounts[0] * (s16) 0xc);
    TextManager::Get().Init(mGlyphBuffers[0], 1, 0, 0x18, 0x10, 0x44);
    TextManager::Get().SetColor(1, 0, 0xf);
    u8 *alphabet = (u8 *) LoadFile("mes_Alphabet.mes", 0, 0, 0, 0, 0);
    TextManager::Get();
    mAlphabetChar = *(u16 *) (alphabet + *(u16 *) (alphabet + 2) + 0xb4);
    OS_Free(alphabet);
    switch (mMode) {
        case LIST_MODE_STORE_BAKUGAN:
            mMessageWindow = new MessageWindow("mes_storecomment", 1, 0x1b000);
            mMessageWindow->StartMessage(0xb, 0, 1);
            break;
        case LIST_MODE_STORE_GAUGE:
            mMessageWindow = new MessageWindow("mes_storecomment", 1, 0x1b000);
            mMessageWindow->StartMessage(0xd, 0, 1);
            break;
        case LIST_MODE_STORE_CARD:
            mMessageWindow = new MessageWindow("mes_storecomment", 1, 0x1b000);
            mMessageWindow->StartMessage(0xc, 0, 1);
            break;
        case LIST_MODE_EDIT:
            mMessageWindow = new MessageWindow("mes_DeckSelect", 1, 0x1b000);
            mMessageWindow->StartMessage(6, 0, 1);
            break;
        case LIST_MODE_BATTLE_EDIT:
            mMessageWindow = new MessageWindow("mes_DeckSelect", 1, 0x1b000);
            mMessageWindow->StartMessage(5, 0, 1);
            break;
        case LIST_MODE_GALLERY:
            mMessageWindow = new MessageWindow("mes_myroom", 1, 0x1b000);
            mMessageWindow->StartMessage(0x30, 0, 1);
            break;
    }
    if (mMessageWindow != NULL) {
        mMessageWindow->func_02065bd4(0x14, 4, 0);
        mMessageWindow->SetWidth(0xd8);
    }
}

void ListScene::PrintRows() {
    u8 i = 0;
    ClearSubScreenText();
    switch (mMode) {
        case LIST_MODE_STORE_BAKUGAN:
            for (; i < 4; i++) {
                s32 index = mScroll + i;
                if (index >= mItemCount) {
                    break;
                }
                u32 a = func_02086548((u8) mItemIds[index], mListFilter);
                u32 b = func_02086570((u8) mItemIds[index], mListFilter);
                PrintBakuganRow((u8) mItemIds[mScroll + i], i, a, b);
            }
            break;
        case LIST_MODE_STORE_GAUGE:
            for (; i < 4; i++) {
                if (mScroll + i >= mItemCount) {
                    break;
                }
                BakuganEntry *entry = GetBakuganEntry(gSaveData, (u8) mItemIds[mScroll + i], mListFilter);
                u32 v = func_020231f4(gSaveData, 0, (u8) mItemIds[mScroll + i], mListFilter);
                PrintBakuganRow((u8) mItemIds[mScroll + i], i, v, (u8) entry->mLevel);
            }
            break;
        case LIST_MODE_STORE_CARD:
            for (; i < 4; i++) {
                if (mScroll + i >= mItemCount) {
                    break;
                }
                PrintCardNameShort(mItemIds[mScroll + i], i);
            }
            break;
        case LIST_MODE_EDIT:
        case LIST_MODE_BATTLE_EDIT:
            if (mPage == 0) {
                break;
            }
            if (mPage < 4) {
                for (; i < 4; i++) {
                    if (mScroll + i >= mItemCount) {
                        break;
                    }
                    BakuganEntry *entry = GetBakuganEntry(gSaveData, (u8) mItemIds[mScroll + i], mListFilter);
                    u32 v = func_020231f4(gSaveData, 0, (u8) mItemIds[mScroll + i], mListFilter);
                    PrintBakuganRow((u8) mItemIds[mScroll + i], i, v, (u8) entry->mLevel);
                }
            } else {
                for (; i < 4; i++) {
                    if (mScroll + i >= mItemCount) {
                        break;
                    }
                    PrintCardNameShort(mItemIds[mScroll + i], i);
                }
            }
            break;
        case LIST_MODE_GALLERY:
            if (mPage == 0) {
                break;
            }
            if (mPage < 7) {
                for (; i < 4; i++) {
                    if (mScroll + i >= mItemCount) {
                        break;
                    }
                    BakuganEntry *entry = GetBakuganEntry(gSaveData, (u8) mItemIds[mScroll + i], mListFilter);
                    u32 v = func_020231f4(gSaveData, 0, (u8) mItemIds[mScroll + i], mListFilter);
                    PrintBakuganRow((u8) mItemIds[mScroll + i], i, v, (u8) entry->mLevel);
                }
            } else {
                for (; i < 4; i++) {
                    if (mScroll + i >= mItemCount) {
                        break;
                    }
                    PrintCardNameShort(mItemIds[mScroll + i], i);
                }
            }
            break;
    }
    QueueUpload(SpriteManager::Get().Screen(1).mBgIndex, UPLOAD_KIND_OBJ, 0, mTextBuffers[0], mTextBufferSizes[0], 0);
}

void ListScene::DrawSubScreenText() {
    if (mTextKind != LIST_TEXT_KIND_NONE) {
        return;
    }
    if (mMessageWindow != NULL) {
        mMessageWindow->Update();
    }
    SpriteManager::Get().Screen(1).AddOam(mGlyphBuffers[0], (u8) mGlyphCounts[0], 0x41, (s16) (mRowSlideY + 0x48), 2, 0, 0xf, 0);
}

void ListScene::ClearSubScreenText() {
    if (mTextBuffers[0] != NULL) {
        MI_CpuClear16(mTextBuffers[0], mTextBufferSizes[0]);
    }
}

void ListScene::UpdateListInput() {
    u32 act = 0;
    if (mPageSliding != 0 || mTargetPage != mCursorPage || mListScrolling != 0 || mTargetItem != mCursorItem || mItemMoving != 0) {
        return;
    }
    if (mListWindowResult == 3) {
        switch (mListWindow->mCursor->mSelection) {
            case 0:
                act = 2;
                break;
            case 3:
                act = 3;
                break;
            case 4:
                act = 4;
                break;
        }
    }
    switch (mListWindow->mCursor->mInputItem) {
        case 1:
            if (act == 0) {
                if (mListInputState == 0) {
                    mListInputState = 5;
                    act     = 3;
                } else if (TouchManager::Get().mTrigger != 0) {
                    mListInputState = 6;
                    act     = 3;
                }
            }
            break;
        case 2:
            if (act == 0) {
                if (mListInputState == 0) {
                    mListInputState = 7;
                    act     = 4;
                } else if (TouchManager::Get().mTrigger != 0) {
                    mListInputState = 8;
                    act     = 4;
                }
            }
            break;
        case 5:
            if (act == 0) {
                if (mListInputState == 0) {
                    mListInputState = 9;
                    act     = 5;
                } else if (TouchManager::Get().mTrigger != 0) {
                    act = 5;
                }
            }
            break;
        case 6:
            if (act == 0) {
                if (mListInputState == 0) {
                    mListInputState = 9;
                    act     = 6;
                } else if (TouchManager::Get().mTrigger != 0) {
                    act = 6;
                }
            }
            break;
    }
    if (mListWindow->mState == 1) {
        if (InputManager::Get().IsPressed(1)) {
            if (act == 0) {
                act = 1;
            }
        } else if (InputManager::Get().IsPressed(2)) {
            if (act == 0) {
                act = 2;
                mListWindow->mCellAnims[0]->SetAnimation(3);
                mListWindowCell0AnimTimer = 6;
            }
        } else if (InputManager::Get().IsTrigger(KEY_LEFT)) {
            if (act == 0) {
                if (mSelectingPage != 0) {
                    act = 3;
                    if (InputManager::Get().IsPressed(KEY_LEFT)) {
                        mListInputState = 1;
                    } else {
                        mListInputState = 2;
                    }
                } else if (mListWindow->mButtons[3].mEnabled != 0) {
                    mListWindow->SetResultItem(3, 3);
                }
            }
        } else if (InputManager::Get().IsTrigger(KEY_RIGHT)) {
            if (act == 0) {
                if (mSelectingPage != 0) {
                    act = 4;
                    if (InputManager::Get().IsPressed(KEY_RIGHT)) {
                        mListInputState = 3;
                    } else {
                        mListInputState = 4;
                    }
                } else if (mListWindow->mButtons[4].mEnabled != 0) {
                    mListWindow->SetResultItem(4, 3);
                }
            }
        } else if (InputManager::Get().IsTrigger(KEY_UP)) {
            if (act == 0) {
                act = 5;
            }
        } else if (InputManager::Get().IsTrigger(KEY_DOWN)) {
            if (act == 0) {
                act = 6;
            }
        }
        if (mAreaCursor->Update(0) == 3) {
            switch (mAreaCursor->mSelection) {
                case 0:
                    if (act == 0 && mSelectingPage == 0) {
                        act = 7;
                    }
                    break;
                case 1:
                    if (act == 0) {
                        if (mSelectingPage != 0) {
                            act = 8;
                        }
                    }
                    break;
            }
        }
        s8 prev = mPageAndRowCursor->mSelection;
        if (mPageAndRowCursor->Update(0) == 3) {
            s8 cur = mPageAndRowCursor->mSelection;
            if (prev == cur) {
                if (act == 0) {
                    act = 1;
                }
            } else if (act == 0) {
                if (mSelectingPage != 0) {
                    mTargetPage                = mPageScroll + cur;
                    mPageAndRowCursor->mSelection = 0;
                } else {
                    mTargetItem = mScroll + cur - 5;
                }
                SoundManager::Get().StartSeqArc(5);
            }
        }
    }
    if (mSelectingPage != 0) {
        switch (act) {
            case 1:
            case 8:
                if (mItemCount == 0) {
                    return;
                }
                mSelectingPage     = 0;
                mWndMode    = 1;
                mDirtyFlags[0] = 1;
                RefreshCommandList();
                RefreshCommandPage();
                if ((mMode == LIST_MODE_EDIT || mMode == LIST_MODE_BATTLE_EDIT) && mPage == 0) {
                    SetMessage(-1);
                }
                SoundManager::Get().StartSeqArc(0);
                return;
            case 2:
                if (mMode == LIST_MODE_BATTLE_EDIT) {
                    mState = 0xf;
                } else {
                    mState = 0x2b;
                }
                SoundManager::Get().StartSeqArc(1);
                return;
            case 3:
                if (mCursorPage > mFirstPage) {
                    mTargetPage = mCursorPage - 1;
                    SoundManager::Get().StartSeqArc(5);
                } else {
                    mListInputState = 0;
                }
                return;
            case 4:
                if (mCursorPage < mPageCount - 1) {
                    mTargetPage = mCursorPage + 1;
                    SoundManager::Get().StartSeqArc(5);
                } else {
                    mListInputState = 0;
                }
                return;
        }
    } else {
        switch (act) {
            case 1:
                if (mItemCount == 0) {
                    return;
                }
                switch (mMode) {
                    case LIST_MODE_STORE_BAKUGAN:
                        mPrice = GetPrice((u8) mItemIds[mCursorItem], mListFilter);
                        if (gSaveData[0].mMoney < mPrice) {
                            SetupListTextKind(this, LIST_TEXT_KIND_PRICE_ABOVE_MONEY);
                        } else if (func_020231f4(gSaveData, 0, (u8) mItemIds[mCursorItem], mListFilter) != 0) {
                            SetupListTextKind(this, 3);
                        } else {
                            SetupListTextKind(this, LIST_TEXT_KIND_PRICE);
                        }
                        SoundManager::Get().StartSeqArc(0);
                        return;
                    case LIST_MODE_STORE_GAUGE: {
                        BakuganEntry *entry = GetBakuganEntry(gSaveData, (u8) mItemIds[mCursorItem], mListFilter);
                        mPrice                  = GetEntryPrice((u8) mItemIds[mCursorItem], mListFilter);
                        if ((s32) entry->mLevel >= 9) {
                            SetupListTextKind(this, 7);
                        } else if (gSaveData[0].mMoney < mPrice) {
                            SetupListTextKind(this, LIST_TEXT_KIND_PRICE_ABOVE_MONEY);
                        } else {
                            SetupListTextKind(this, LIST_TEXT_KIND_PRICE);
                        }
                        SoundManager::Get().StartSeqArc(0);
                        return;
                    }
                    case LIST_MODE_STORE_CARD:
                        mPrice = Table_020a39fc()[mItemIds[mCursorItem]];
                        if (gSaveData[0].mMoney < mPrice) {
                            SetupListTextKind(this, LIST_TEXT_KIND_PRICE_ABOVE_MONEY);
                        } else {
                            SetupListTextKind(this, LIST_TEXT_KIND_PRICE);
                        }
                        SoundManager::Get().StartSeqArc(0);
                        return;
                    case LIST_MODE_EDIT:
                    case LIST_MODE_BATTLE_EDIT:
                        if (mPage == 0) {
                            if (mCursorItem == 0) {
                                SetupListTextKind(this, LIST_TEXT_KIND_READ_SLOTS);
                            } else if (mCursorItem == 1) {
                                SetupListTextKind(this, LIST_TEXT_KIND_PICK_SLOTS);
                            }
                        } else if (mPage < 4) {
                            if (IsInSlots(1, (u8) mItemIds[mCursorItem], mListFilter) == 0) {
                                mMoveY = (mCursorItem - mScroll) * 0x20 + 0x24;
                                mMoveYPerFrame = mMoveY / 8;
                                mItemMoving = 1;
                            }
                        } else {
                            if (IsInSlots(0, (u8) mItemIds[mCursorItem], 0) == 0) {
                                mMoveY = (mCursorItem - mScroll) * 0x20 + 0x24;
                                mMoveYPerFrame = mMoveY / 8;
                                mItemMoving = 1;
                            }
                        }
                        SoundManager::Get().StartSeqArc(0);
                        return;
                }
                break;
            case 2:
            case 7:
                mSelectingPage                = 1;
                mWndMode               = 2;
                mSavedCursorItems[mPage]      = mCursorItem;
                mDirtyFlags[0]            = 1;
                RefreshCommandList();
                RefreshCommandPage();
                if ((mMode == LIST_MODE_EDIT || mMode == LIST_MODE_BATTLE_EDIT) && mPage == 0) {
                    SetMessage(-1);
                }
                if (act == 2) {
                    SoundManager::Get().StartSeqArc(1);
                } else {
                    SoundManager::Get().StartSeqArc(0);
                }
                return;
            case 3:
            case 4:
                if ((mMode == LIST_MODE_EDIT || mMode == LIST_MODE_BATTLE_EDIT) && mPage != 0 && mPage < 4) {
                    if (act == 3) {
                        mListFilter = (mListFilter + 5) % 6;
                    } else {
                        mListFilter = (mListFilter + 1) % 6;
                    }
                    BuildList(1);
                    mCursorItem     = 0;
                    mTargetItem     = 0;
                    mScroll     = 0;
                    mDirtyFlags[2] = 1;
                    mDirtyFlags[0] = 1;
                    RefreshCommandList();
                    SoundManager::Get().StartSeqArc(0);
                }
                mListInputState = 0;
                return;
            case 5:
                if (mItemCount <= 1) {
                    return;
                }
                if (mCursorItem != 0) {
                    mTargetItem = mCursorItem - 1;
                } else {
                    mTargetItem = mItemCount - 1;
                }
                SoundManager::Get().StartSeqArc(5);
                return;
            case 6:
                if (mItemCount <= 1) {
                    return;
                }
                if (mCursorItem < mItemCount - 1) {
                    mTargetItem = mCursorItem + 1;
                } else {
                    mTargetItem = 0;
                }
                SoundManager::Get().StartSeqArc(5);
                return;
        }
    }
}

void ListScene::UpdateMainScreen() {
    switch (mMainScreenState) {
        case 1:
            mListWindow->mEnabled = 0;
            if (mCommandList != NULL) {
                CloseCommandList(mCommandList);
                mContext->mRequest = 0;
                mCommandListClosed          = 1;
                mMainScreenState          = 3;
            } else if (mCommandPage == NULL) {
                mMainScreenState = 2;
            } else {
                CloseCommandPage(mCommandPage, 1);
                mContext->mRequest = 0;
                mCommandPageClosed          = 1;
                mMainScreenState          = 3;
            }
            SpriteManager::Get().sFade[0] = FADE_MODE_DOWN;
            break;
        case 2:
            if (SpriteManager::Get().sFade[0] <= FADE_MODE_HOLD) {
                ReleaseMainScreenGraphics();
                mMainScreenState = 4;
            }
            break;
        case 3:
            if (mContext->mRequest != 0) {
                if (mCommandList != NULL) {
                    mCommandList = NULL;
                }
                if (mCommandPage != NULL) {
                    mCommandPage = NULL;
                }
                mMainScreenState = 4;
            }
            break;
        case 4:
            SetupMainScreenForPage();
            mMainScreenState        = 0;
            mListWindow->mEnabled = 1;
            break;
    }
}

void ListScene::UpdateListWindow() {
    if (mDirtyFlags[0] != 0) {
        mDirtyFlags[0] = 0;
        if (mCursorPage > mFirstPage && mSelectingPage != 0) {
            SetItemFlags(0, 1, WINDOW_ITEM_NO_SLIDE_OUT);
            mListWindow->mCellAnims[1]->Rewind();
        } else {
            SetItemFlags(0, 1, WINDOW_ITEM_CURSOR_SKIP);
        }
        if (mCursorPage < mPageCount - 1 && mSelectingPage != 0) {
            SetItemFlags(0, 2, WINDOW_ITEM_NO_SLIDE_OUT);
            mListWindow->mCellAnims[2]->Rewind();
        } else {
            SetItemFlags(0, 2, WINDOW_ITEM_CURSOR_SKIP);
        }
        for (u8 i = 0; i <= 4; i++) {
            if (mCursorPage + i >= mPageCount || mSelectingPage == 0) {
                SetItemFlags(2, i, WINDOW_ITEM_CURSOR_SKIP);
            } else {
                SetItemFlags(2, i, WINDOW_ITEM_NO_SLIDE_OUT);
            }
        }
        for (u8 i = 5; i <= 7; i++) {
            if (i - 5 >= mItemCount || mSelectingPage != 0) {
                SetItemFlags(2, i, WINDOW_ITEM_CURSOR_SKIP);
            } else {
                SetItemFlags(2, i, WINDOW_ITEM_NO_SLIDE_OUT);
            }
        }
        SetItemFlags(0, 0, WINDOW_ITEM_NO_SLIDE_OUT);
        if (mItemCount < 2 || mSelectingPage != 0) {
            SetItemFlags(0, 5, WINDOW_ITEM_CURSOR_SKIP);
            SetItemFlags(0, 6, WINDOW_ITEM_CURSOR_SKIP);
        } else {
            SetItemFlags(0, 5, WINDOW_ITEM_NO_SLIDE_OUT);
            mListWindow->mCellAnims[5]->Rewind();
            SetItemFlags(0, 6, WINDOW_ITEM_NO_SLIDE_OUT);
            mListWindow->mCellAnims[6]->Rewind();
        }
        switch (mMode) {
            case LIST_MODE_STORE_BAKUGAN:
            case LIST_MODE_STORE_GAUGE:
            case LIST_MODE_STORE_CARD:
            case LIST_MODE_GALLERY:
                SetItemFlags(0, 3, WINDOW_ITEM_CURSOR_SKIP);
                SetItemFlags(0, 4, WINDOW_ITEM_CURSOR_SKIP);
                break;
            case LIST_MODE_EDIT:
            case LIST_MODE_BATTLE_EDIT:
                if (mPage != 0 && mPage < 4 && mSelectingPage == 0) {
                    SetItemFlags(0, 3, WINDOW_ITEM_NO_SLIDE_OUT);
                    SetItemFlags(0, 4, WINDOW_ITEM_NO_SLIDE_OUT);
                    mSubScreenCellAnims[7]->SetAnimation((mListFilter + 5) % 6 * 2 + 0x10);
                    mSubScreenCellAnims[8]->SetAnimation((mListFilter + 1) % 6 * 2 + 0x10);
                } else {
                    SetItemFlags(0, 3, WINDOW_ITEM_CURSOR_SKIP);
                    SetItemFlags(0, 4, WINDOW_ITEM_CURSOR_SKIP);
                }
                break;
        }
        if (mSelectingPage != 0) {
            mPageAndRowCursor->mSelection = 0;
        } else {
            mPageAndRowCursor->mSelection = mCursorItem - mScroll + 5;
        }
    }
    mListWindowResult = mListWindow->Update(0);
}

void ListScene::UpdatePages() {
    switch (mListInputState) {
        case 2:
            if (InputManager::Get().IsDown(KEY_LEFT) == 0 && mPageSliding == 0) {
                ApplyTargetPage();
            }
            break;
        case 4:
            if (InputManager::Get().IsDown(KEY_RIGHT) == 0 && mPageSliding == 0) {
                ApplyTargetPage();
            }
            break;
        case 5:
            if (TouchManager::Get().mReleased != 0) {
                mListInputState = 0;
            }
            break;
        case 6:
            if (TouchManager::Get().mReleased != 0 && mPageSliding == 0) {
                ApplyTargetPage();
            }
            break;
        case 7:
            if (TouchManager::Get().mReleased != 0) {
                mListInputState = 0;
            }
            break;
        case 8:
            if (TouchManager::Get().mReleased != 0 && mPageSliding == 0) {
                ApplyTargetPage();
            }
            break;
        case 9:
            if (TouchManager::Get().mReleased != 0) {
                mListInputState = 0;
            }
            break;
    }
    if (mPageSliding != 0 || mTargetPage != mCursorPage) {
        if (mPageSliding == 0) {
            mPageSliding = 1;
            if (mTargetPage <= mCursorPage) {
                mPageSlideX = -0x2c;
                mPageScroll--;
                mCursorPage--;
            }
        }
        if (mTargetPage > mCursorPage) {
            mPageSlideX -= 0xb;
        } else {
            mPageSlideX += 0xb;
        }
        if (mPageSlideX >= 0 || mPageSlideX <= -0x2c) {
            mPageSliding = 0;
            if (mTargetPage > mCursorPage) {
                mPageScroll++;
                mCursorPage++;
            }
            mPageSlideX = 0;
            if (mTargetPage == mCursorPage) {
                mDirtyFlags[0] = 1;
                if (ShouldApplyTargetPage()) {
                    ApplyTargetPage();
                }
            }
        }
    }
    if (mDirtyFlags[1] != 0) {
        if (IsCommandListOrPageSettingUp() == 0) {
            LoadSlotGraphics();
            mDirtyFlags[1] = 0;
        }
    }
}

void ListScene::ApplyTargetPage() {
    if (mListInputState != 5 && mListInputState != 7) {
        mListInputState = 0;
    }
    mPage      = mTargetPage;
    mDirtyFlags[2] = 1;
    switch (mMode) {
        case LIST_MODE_STORE_BAKUGAN:
        case LIST_MODE_STORE_GAUGE:
        case LIST_MODE_STORE_CARD:
            mListFilter = mPage;
            BuildList(1);
            RefreshCommandList();
            RefreshCommandPage();
            break;
        case LIST_MODE_EDIT:
        case LIST_MODE_BATTLE_EDIT:
            SetMessage(-1);
            if (mPage == 0) {
                BuildList(1);
                if (mMainScreenState == 0) {
                    mMainScreenState = 1;
                }
            } else if (mPage < 4) {
                mListFilter = mSlotValues[mPage + 8];
                BuildList(1);
                if (mCommandList == NULL || mCommandListClosed != 0) {
                    if (mMainScreenState == 0) {
                        mMainScreenState = 1;
                    }
                } else {
                    RefreshCommandList();
                }
            } else {
                mListFilter = mPage - 4;
                BuildList(1);
                if (mCommandPage == NULL || mCommandPageClosed != 0) {
                    if (mMainScreenState == 0) {
                        mMainScreenState = 1;
                    }
                } else {
                    RefreshCommandPage();
                }
            }
            break;
        case LIST_MODE_GALLERY:
            if (mPage == 0) {
                BuildList(1);
                if (mMainScreenState == 0) {
                    mMainScreenState = 1;
                }
                GXS_SetVisiblePlane(0x19);
            } else if (mPage < 7) {
                mListFilter = mPage - 1;
                BuildList(1);
                if (mCommandList == NULL || mCommandListClosed != 0) {
                    if (mMainScreenState == 0) {
                        mMainScreenState = 1;
                    }
                } else {
                    RefreshCommandList();
                }
                GXS_SetVisiblePlane(0x1d);
            } else {
                mListFilter = mPage - 7;
                BuildList(1);
                if (mCommandPage == NULL || mCommandPageClosed != 0) {
                    if (mMainScreenState == 0) {
                        mMainScreenState = 1;
                    }
                } else {
                    RefreshCommandPage();
                }
                GXS_SetVisiblePlane(0x1d);
            }
            break;
    }
}

void ListScene::UpdateListScroll() {
    if (mListScrolling != 0 || mTargetItem != mCursorItem) {
        if (mListScrolling == 0 && mScroll <= mTargetItem && mTargetItem < mScroll + 3) {
            mCursorItem = mTargetItem;
            RefreshCommandList();
            RefreshCommandPage();
            SetMessage(-1);
            mDirtyFlags[0] = 1;
        } else if (IsCommandListOrPageSettingUp() == 0) {
            u32 moved = 0;
            if (mListScrolling == 0) {
                mListScrolling = 1;
                if (mTargetItem > mCursorItem) {
                    if (mTargetItem - mCursorItem > 1) {
                        mCursorItem = mTargetItem;
                        mScroll = mTargetItem;
                        if (mScroll >= mItemCount - 3) {
                            mScroll = mItemCount - 3;
                        }
                        moved   = 1;
                    } else {
                        LoadBakuganMark((u8) mItemIds[mScroll + 3], (u8) ((mScroll + 3) % 4), 2);
                        LoadSmallCard(mItemIds[mScroll + 3], (u8) ((mScroll + 3) % 4), 2);
                    }
                } else {
                    if (mCursorItem - mTargetItem > 1) {
                        mCursorItem = mTargetItem;
                        mScroll = mTargetItem;
                        if (mScroll >= mItemCount - 3) {
                            mScroll = mItemCount - 3;
                        }
                        moved   = 1;
                    } else {
                        mRowSlideY = -0x20;
                        mScroll--;
                        mCursorItem--;
                        LoadBakuganMark((u8) mItemIds[mScroll], (u8) (mScroll % 4), 2);
                        LoadSmallCard(mItemIds[mScroll], (u8) (mScroll % 4), 2);
                        PrintRows();
                    }
                }
            }
            if (moved == 0) {
                if (mTargetItem > mCursorItem) {
                    mRowSlideY -= 8;
                } else {
                    mRowSlideY += 8;
                }
                if (mRowSlideY >= 0 || mRowSlideY <= -0x20) {
                    mListScrolling = 0;
                    if (mTargetItem > mCursorItem) {
                        mScroll++;
                        mCursorItem++;
                        PrintRows();
                    }
                    mRowSlideY = 0;
                    RefreshCommandList();
                    RefreshCommandPage();
                    mDirtyFlags[0] = 1;
                }
                QueueFrameCallback(ListSceneScrollCallback, this);
            } else {
                mListScrolling = 0;
                RefreshCommandList();
                RefreshCommandPage();
                mDirtyFlags[0] = 1;
                mDirtyFlags[2] = 1;
            }
        }
    }
    if (mDirtyFlags[2] != 0) {
        if (IsCommandListOrPageSettingUp() == 0) {
            LoadRowGraphics();
            mDirtyFlags[2] = 0;
        }
        PrintRows();
    }
}

void ListScene::ApplyWndMode() {
    if (mAppliedWndMode == mWndMode) {
        return;
    }
    switch (mWndMode) {
        case 0:
            GXS_SetVisibleWnd(0);
            G2S_SetWndOutsidePlane(0x1f, FALSE);
            G2S_SetWnd0InsidePlane(0x1f, FALSE);
            G2S_SetWnd1InsidePlane(0x1f, FALSE);
            G2S_SetBlendBrightness(0x1f, 0);
            break;
        case 1:
            GXS_SetVisibleWnd(3);
            G2S_SetWnd0Position(0x18, 0x21, 0x38, 0x3f);
            G2S_SetWnd1Position(1, 0x21, 0xff, 0x3f);
            G2S_SetWndOutsidePlane(0x1f, FALSE);
            G2S_SetWnd0InsidePlane(0x1f, FALSE);
            G2S_SetWnd1InsidePlane(0x3f, FALSE);
            G2S_SetBlendBrightness(0x1f, -8);
            break;
        case 2:
            GXS_SetVisibleWnd(3);
            G2S_SetWnd0Position(0, 0x41, 0x80, 0xa8);
            G2S_SetWnd1Position(0x80, 0x41, 0, 0xa8);
            G2S_SetWndOutsidePlane(0x1f, FALSE);
            G2S_SetWnd0InsidePlane(0x3f, FALSE);
            G2S_SetWnd1InsidePlane(0x3f, FALSE);
            G2S_SetBlendBrightness(0x1f, -8);
            break;
    }
    mAppliedWndMode = mWndMode;
}

void ListScene::DrawMainScreen() {
    if (mCommandList != NULL || mCommandPage != NULL) {
        return;
    }
    if (mMainScreenState > 2) {
        return;
    }
    u8 i = 0;
    switch (mMode) {
        case LIST_MODE_EDIT:
        case LIST_MODE_BATTLE_EDIT:
            if (mMainScreenCellAnims[0] != NULL) {
                mMainScreenCellAnims[0]->Draw(0, 0x80, 0x60, 0);
            }
            if (mMainScreenCellAnims[3] != NULL) {
                mMainScreenCellAnims[3]->DrawFrame(0, 0, 0x80, 0x60, 0);
            }
            for (; i < 9; i++) {
                if (i < 3) {
                    if (mFirstSlotOnly == 0 || i == 0) {
                        DrawBakuganMark((u8) mSlotValues[9 + i], i, 0, i * 24 + 0x15, 0x57);
                    }
                } else {
                    DrawSmallCard(i - 3, i - 3, 0, (i + 1) * 24, 0x55);
                }
            }
            {
                s32 n = func_0208644c();
                if ((u32) n < 1000) {
                    mMainScreenCellAnims[5]->DrawFrame(0, n / 100, 0x6e, 0x94, 0);
                    mMainScreenCellAnims[5]->DrawFrame(0, n / 10 % 10, 0x7a, 0x94, 0);
                    mMainScreenCellAnims[5]->DrawFrame(0, n % 10, 0x86, 0x94, 0);
                } else {
                    mMainScreenCellAnims[5]->DrawFrame(0, n / 1000, 0x68, 0x94, 0);
                    mMainScreenCellAnims[5]->DrawFrame(0, n / 100 % 10, 0x74, 0x94, 0);
                    mMainScreenCellAnims[5]->DrawFrame(0, n / 10 % 10, 0x80, 0x94, 0);
                    mMainScreenCellAnims[5]->DrawFrame(0, n % 10, 0x8c, 0x94, 0);
                }
            }
            break;
        case LIST_MODE_GALLERY:
            if (mMainScreenCellAnims[1] != NULL) {
                mMainScreenCellAnims[1]->Draw(i, 0x80, 0x60, i);
            }
            if (mMainScreenCellAnims[3] != NULL) {
                mMainScreenCellAnims[3]->DrawFrame(0, 1, 0x80, 0x60, 0);
            }
            DrawMainScreenText();
            if (mMainScreenCellAnims[2] == NULL) {
                return;
            }
            for (; i < 14; i++) {
                s16 x = i / 7 * 0x78 + 0x24;
                s16 y = i % 7 * 0x14 + 0x2c;
                if (i % 7 == 6) {
                    y += 6;
                }
                mMainScreenCellAnims[2]->DrawFrame(0, GetRatioFrame(i), x, y, 0);
            }
            break;
    }
}

void ListScene::DrawWindows() {
    if (mListWindow != NULL) {
        mListWindow->Draw();
    }
    if (mPromptWindow != NULL) {
        mPromptWindow->Draw();
    }
    if (mDeckWindow != NULL) {
        mDeckWindow->Draw();
    }
}

void ListScene::DrawPagesAndMovingItem() {
    if (mItemMoving != 0) {
        if (mPage < 4) {
            DrawBakuganMark(mListFilter, (u8) (mCursorItem % 4), 4, 0x1d, mMoveY + 0x25);
        } else {
            DrawSmallCard(mListFilter, (u8) (mCursorItem % 4), 4, 0x20, mMoveY + 0x24);
        }
    }
    u8 limit = 6;
    u8 i     = 0;
    if (mPageSliding == 0) {
        limit--;
    }
    do {
        if (mPageScroll + i >= mPageCount) {
            break;
        }
        switch (mMode) {
            case LIST_MODE_STORE_BAKUGAN:
            case LIST_MODE_STORE_GAUGE:
                mSubScreenCellAnims[4]->DrawFrame(1, (u8) (mPageScroll + i), mPageSlideX + 0x1e + i * 0x2c, 0x26, 1);
                break;
            case LIST_MODE_STORE_CARD:
                mSubScreenCellAnims[5]->DrawFrame(1, (u8) (mPageScroll + i), mPageSlideX + 0x20 + i * 0x2c, 0x24, 1);
                break;
            case LIST_MODE_EDIT:
            case LIST_MODE_BATTLE_EDIT:
                if ((mPageScroll + i) == 0) {
                    mSubScreenCellAnims[10]->SetAnimation(0x1d);
                    mSubScreenCellAnims[10]->Draw(1, (s16) (mPageSlideX + 0x19 + i * 0x2c), 0x21, 1);
                } else if ((mPageScroll + i) < 4) {
                    if (mFirstSlotOnly == 0 || (mPageScroll + i) == 1) {
                        DrawBakuganMark((u8) mSlotValues[(mPageScroll + i) + 8], (u8) ((mPageScroll + i) - 1), 1, mPageSlideX + 0x1d + i * 0x2c, 0x25);
                    }
                } else {
                    DrawSmallCard((mPageScroll + i) - 4, (mPageScroll + i) - 4, 1, mPageSlideX + 0x20 + i * 0x2c, 0x24);
                }
                break;
            case LIST_MODE_GALLERY:
                if ((mPageScroll + i) == 0) {
                    mSubScreenCellAnims[10]->SetAnimation(0x20);
                    mSubScreenCellAnims[10]->Draw(1, (s16) (mPageSlideX + 0x19 + i * 0x2c), 0x21, 1);
                } else if ((mPageScroll + i) < 7) {
                    mSubScreenCellAnims[4]->DrawFrame(1, (u8) ((mPageScroll + i) - 1), mPageSlideX + 0x1e + i * 0x2c, 0x26, 1);
                } else {
                    mSubScreenCellAnims[5]->DrawFrame(1, (u8) ((mPageScroll + i) - 7), mPageSlideX + 0x20 + i * 0x2c, 0x24, 1);
                }
                break;
        }
        i++;
    } while (i < limit);
    mSubScreenCellAnims[0]->Draw(1, 0x80, 0x60, 1);
    switch (mMode) {
        case LIST_MODE_STORE_BAKUGAN:
        case LIST_MODE_STORE_GAUGE:
            mSubScreenCellAnims[1]->DrawFrame(1, 1, mPageSlideX + 0x14 - mPageScroll * 0x2c, 0x22, 1);
            break;
        case LIST_MODE_STORE_CARD:
            mSubScreenCellAnims[1]->DrawFrame(1, 0, mPageSlideX + 0x14 - mPageScroll * 0x2c, 0x22, 1);
            mSubScreenCellAnims[1]->DrawFrame(1, 0, mPageSlideX + 0x98 - mPageScroll * 0x2c, 0x22, 1);
            break;
        case LIST_MODE_EDIT:
        case LIST_MODE_BATTLE_EDIT:
            if (mPageScroll < 6) {
                mSubScreenCellAnims[1]->DrawFrame(1, 0, mPageSlideX + 0x40 - mPageScroll * 0x2c, 0x22, 1);
            }
            mSubScreenCellAnims[1]->DrawFrame(1, 0, mPageSlideX + 0xc4 - mPageScroll * 0x2c, 0x22, 1);
            if (mPageScroll != 0) {
                mSubScreenCellAnims[1]->DrawFrame(1, 0, mPageSlideX + 0x148 - mPageScroll * 0x2c, 0x22, 1);
            }
            break;
        case LIST_MODE_GALLERY:
            if (mPageScroll < 8) {
                mSubScreenCellAnims[1]->DrawFrame(1, 1, mPageSlideX + 0x40 - mPageScroll * 0x2c, 0x22, 1);
            }
            if (mPageScroll != 0) {
                mSubScreenCellAnims[1]->DrawFrame(1, 0, mPageSlideX + 0x148 - mPageScroll * 0x2c, 0x22, 1);
            }
            if (mPageScroll > 3) {
                mSubScreenCellAnims[1]->DrawFrame(1, 0, mPageSlideX + 0x1cc - mPageScroll * 0x2c, 0x22, 1);
            }
            break;
    }
}

void ListScene::DrawRows() {
    if (mMode == LIST_MODE_GALLERY && mPage == 0) {
        return;
    }
    mSubScreenCellAnims[15]->Update();
    u8 count = 4;
    u8 i     = 0;
    if (mListScrolling == 0) {
        count--;
    }
    do {
        if (mScroll + i >= mItemCount) {
            if (mItemCount == 0 && i == 0) {
                mSubScreenCellAnims[12]->SetAnimation(0x21);
                mSubScreenCellAnims[12]->Draw(1, 0x71, (s16) (mRowSlideY + 0x4b + i * 32), 2);
            }
        } else {
            switch (mMode) {
                case LIST_MODE_STORE_BAKUGAN:
                    if (func_020231f4(gSaveData, 0, (u8) mItemIds[mScroll + i], mListFilter) == 0) {
                        mSubScreenCellAnims[15]->Draw(1, 0x19, (s16) (mRowSlideY + 0x5b + i * 32), 2);
                    }
                    DrawBakuganMark(mListFilter, (u8) ((mScroll + i) % 4), 2, 0x1d, mRowSlideY + 0x49 + i * 32);
                    DrawMoneyAmount(GetPrice((u8) mItemIds[mScroll + i], mListFilter), i);
                    break;
                case LIST_MODE_STORE_GAUGE:
                    DrawBakuganMark(mListFilter, (u8) ((mScroll + i) % 4), 2, 0x1d, mRowSlideY + 0x49 + i * 32);
                    DrawMoneyAmount(GetEntryPrice((u8) mItemIds[mScroll + i], mListFilter), i);
                    if (IsInSlots(1, (u8) mItemIds[mScroll + i], mListFilter) != 0) {
                        mSubScreenCellAnims[9]->Draw(1, 0x80, (s16) (mRowSlideY + 0x54 + i * 32), 2);
                    }
                    break;
                case LIST_MODE_STORE_CARD:
                    DrawSmallCard(mListFilter, (u8) ((mScroll + i) % 4), 2, 0x20, mRowSlideY + 0x48 + i * 32);
                    DrawMoneyAmount(Table_020a39fc()[mItemIds[mScroll + i]], i);
                    break;
                case LIST_MODE_EDIT:
                case LIST_MODE_BATTLE_EDIT:
                    if (mPage == 0) {
                        mSubScreenCellAnims[11]->DrawFrame(1, (u8) (i + 1), 0x19, mRowSlideY + 0x45 + i * 32, 2);
                        mSubScreenCellAnims[12]->SetAnimation(0x1f);
                        mSubScreenCellAnims[12]->DrawFrame(1, (u8) (i + 1), 0x71, mRowSlideY + 0x4b + i * 32, 2);
                    } else if (mPage < 4) {
                        DrawBakuganMark(mListFilter, (u8) ((mScroll + i) % 4), 2, 0x1d, mRowSlideY + 0x49 + i * 32);
                        if (IsInSlots(1, (u8) mItemIds[mScroll + i], mListFilter) != 0) {
                            mSubScreenCellAnims[9]->Draw(1, 0x80, (s16) (mRowSlideY + 0x54 + i * 32), 2);
                        }
                    } else {
                        DrawSmallCard(mListFilter, (u8) ((mScroll + i) % 4), 2, 0x20, mRowSlideY + 0x48 + i * 32);
                        if (IsInSlots(0, (u8) mItemIds[mScroll + i], 0) != 0) {
                            mSubScreenCellAnims[9]->Draw(1, 0x80, (s16) (mRowSlideY + 0x54 + i * 32), 2);
                        }
                    }
                    break;
                case LIST_MODE_GALLERY:
                    if (mPage < 7) {
                        DrawBakuganMark(mListFilter, (u8) ((mScroll + i) % 4), 2, 0x1d, mRowSlideY + 0x49 + i * 32);
                        if (IsInSlots(1, (u8) mItemIds[mScroll + i], mListFilter) != 0) {
                            mSubScreenCellAnims[9]->Draw(1, 0x80, (s16) (mRowSlideY + 0x54 + i * 32), 2);
                        }
                    } else {
                        DrawSmallCard(mListFilter, (u8) ((mScroll + i) % 4), 2, 0x20, mRowSlideY + 0x48 + i * 32);
                        if (IsInSlots(0, (u8) mItemIds[mScroll + i], 0) != 0) {
                            mSubScreenCellAnims[9]->Draw(1, 0x80, (s16) (mRowSlideY + 0x54 + i * 32), 2);
                        }
                    }
                    break;
            }
        }
        i++;
    } while (i < count);
    if ((mMode == LIST_MODE_EDIT || mMode == LIST_MODE_BATTLE_EDIT) && mPage == 0) {
        return;
    }
    DrawItemNumberAndCount();
}

void ListScene::DrawItemNumberAndCount() {
    s16 x = 0xb7;
    u8 a  = mCursorItem + 1;
    if (mItemCount == 0) {
        a = 0;
    }
    mSubScreenCellAnims[13]->SetAnimation(0);
    if (a >= 10) {
        mSubScreenCellAnims[13]->DrawFrame(1, (u8) (a / 10), 0xb7, 0xaf, 1);
    }
    x += 6;
    mSubScreenCellAnims[13]->DrawFrame(1, (u8) (a % 10), x, 0xaf, 1);
    mSubScreenCellAnims[13]->SetAnimation(2);
    x += 6;
    mSubScreenCellAnims[13]->Draw(1, x, 0xaf, 1);
    mSubScreenCellAnims[13]->SetAnimation(0);
    x += 6;
    if (mItemCount >= 10) {
        mSubScreenCellAnims[13]->DrawFrame(1, (u8) (mItemCount / 10), x, 0xaf, 1);
    }
    mSubScreenCellAnims[13]->DrawFrame(1, (u8) (mItemCount % 10), x + 6, 0xaf, 1);
    mSubScreenCellAnims[2]->Draw(1, 0xaf, 0xaa, 1);
}

void ListScene::DrawMoneyFilterButtonsAndCursor() {
    switch (mMode) {
        case LIST_MODE_STORE_BAKUGAN:
        case LIST_MODE_STORE_GAUGE:
        case LIST_MODE_STORE_CARD:
            DrawMoneyAmount(gSaveData[0].mMoney, 4);
            break;
        case LIST_MODE_EDIT:
        case LIST_MODE_BATTLE_EDIT:
            if (mListWindow->mButtons[3].mEnabled != 0) {
                mSubScreenCellAnims[7]->Update();
                mSubScreenCellAnims[7]->Draw(1, 5, 0x6c, 1);
            }
            if (mListWindow->mButtons[4].mEnabled != 0) {
                mSubScreenCellAnims[8]->Update();
                mSubScreenCellAnims[8]->Draw(1, 0xed, 0x6c, 1);
            }
            break;
    }
    mSubScreenCellAnims[3]->Update();
    if (mSelectingPage != 0) {
        mSubScreenCellAnims[3]->Draw(1, 0x19, 0x21, 1);
    } else {
        mSubScreenCellAnims[3]->Draw(1, 0x19, (s16) ((mCursorItem - mScroll) * 32 + 0x45), 1);
    }
}

void ListScene::ReadSlots() {
    if (mCharacterId != 0x15) {
        for (u8 i = 0; i < 3; i++) {
            u8 *entry = (u8 *) ((u32 *) gGameData.mBattlePlayers)[mPlayer + 3] + 0xc + i * 12;
            mSlotValues[i] = entry[8];
            mSlotValues[9 + i] = entry[9];
        }
        for (u8 i = 0; i < 3; i++) {
            mSlotValues[3 + i] = *(u16 *) ((u8 *) ((u32 *) gGameData.mBattlePlayers)[mPlayer + 3] + i * 4 + 0x54);
        }
        for (u8 i = 0; i < 3; i++) {
            mSlotValues[6 + i] = *(u16 *) ((u8 *) ((u32 *) gGameData.mBattlePlayers)[mPlayer + 3] + i * 4 + 0x6c);
        }
    } else {
        for (u8 i = 0; i < 3; i++) {
            mSlotValues[i]     = GetSlotBakuganId(gSaveData, 0, i);
            mSlotValues[9 + i] = GetSlotBakuganAttribute(gSaveData, 0, i);
        }
        for (u8 i = 0; i < 3; i++) {
            mSlotValues[3 + i] = gSaveData[0].mSlots54Values[i];
        }
        for (u8 i = 0; i < 3; i++) {
            mSlotValues[6 + i] = gSaveData[0].mSlots6cValues[i];
        }
    }
}

void ListScene::SetupMainScreenForPage() {
    switch (mMode) {
        case LIST_MODE_STORE_BAKUGAN:
        case LIST_MODE_STORE_GAUGE:
            if (mCommandList != NULL) {
                return;
            }
            mCommandList = new BattleCommandList(0, 0, -1, 0);
            CreateCommandListWindow(mCommandList, 0x1d000, 0, 0xc0, 0x14, 0xe0, 0x14, 0);
            mCommandListClosed = 0;
            RefreshCommandList();
            break;
        case LIST_MODE_STORE_CARD:
            if (mCommandPage != NULL) {
                return;
            }
            mCommandPage = new BattleCommandPage(0, -1);
            CreateCommandPageWindow(mCommandPage, 0x1d000, 0, 0xc0, 0x14, 0xe0, 0x14, 0);
            mCommandPageClosed = 0;
            RefreshCommandPage();
            break;
        case LIST_MODE_EDIT:
        case LIST_MODE_BATTLE_EDIT:
            if (mPage == 0) {
                InitAndLoadMainScreen();
                for (u8 i = 0; i < 9; i++) {
                    if (i < 3) {
                        LoadBakuganMark((u8) mSlotValues[i], i, 0);
                    } else {
                        LoadSmallCard(mSlotValues[i], (u8) (i - 3), 0);
                    }
                }
                SpriteManager::Get().sFade[0] = FADE_MODE_TO_ZERO;
                return;
            }
            if (mPage < 4) {
                if (mCommandList != NULL) {
                    return;
                }
                mCommandList = new BattleCommandList(0, 0, -1, 0);
                CreateCommandListWindow(mCommandList, 0x1d000, 0, 0xc0, 0x14, 0xe0, 0x14, 0);
                mCommandListClosed = 0;
                RefreshCommandList();
                return;
            }
            if (mCommandPage != NULL) {
                return;
            }
            mCommandPage = new BattleCommandPage(0, -1);
            CreateCommandPageWindow(mCommandPage, 0x1d000, 0, 0xc0, 0x14, 0xe0, 0x14, 0);
            mCommandPageClosed = 0;
            RefreshCommandPage();
            return;
        case LIST_MODE_GALLERY:
            if (mPage == 0) {
                InitAndLoadMainScreen();
                SpriteManager::Get().sFade[0] = FADE_MODE_TO_ZERO;
                return;
            }
            if (mPage < 7) {
                if (mCommandList != NULL) {
                    return;
                }
                mCommandList = new BattleCommandList(0, 0, -1, 0);
                CreateCommandListWindow(mCommandList, 0x1d000, 0, 0xc0, 0x14, 0xe0, 0x14, 0);
                mCommandListClosed = 0;
                RefreshCommandList();
                return;
            }
            if (mCommandPage != NULL) {
                return;
            }
            mCommandPage = new BattleCommandPage(0, -1);
            CreateCommandPageWindow(mCommandPage, 0x1d000, 0, 0xc0, 0x14, 0xe0, 0x14, 0);
            mCommandPageClosed = 0;
            RefreshCommandPage();
            break;
    }
}

void ListScene::LoadSlotGraphics() {
    SpriteManager::Get();
    if (mMode != LIST_MODE_EDIT && mMode != LIST_MODE_BATTLE_EDIT) {
        return;
    }
    for (u8 i = 0; i < 9; i++) {
        if (i < 3) {
            LoadBakuganMark((u8) mSlotValues[i], i, 1);
        } else {
            LoadSmallCard(mSlotValues[i], (u8) (i - 3), 1);
        }
    }
}

void ListScene::LoadRowGraphics() {
    SpriteManager::Get();
    u8 i = 0;
    switch (mMode) {
        case LIST_MODE_STORE_BAKUGAN:
        case LIST_MODE_STORE_GAUGE:
            for (; i < 3; i++) {
                if (mScroll + i >= mItemCount) {
                    return;
                }
                LoadBakuganMark((u8) mItemIds[mScroll + i], (u8) ((mScroll + i) % 4), 2);
            }
            break;
        case LIST_MODE_STORE_CARD:
            for (; i < 3; i++) {
                if (mScroll + i >= mItemCount) {
                    return;
                }
                LoadSmallCard(mItemIds[mScroll + i], (u8) ((mScroll + i) % 4), 2);
            }
            break;
        case LIST_MODE_EDIT:
        case LIST_MODE_BATTLE_EDIT:
            if (mPage == 0) {
                return;
            }
            if (mPage < 4) {
                for (; i < 3; i++) {
                    if (mScroll + i >= mItemCount) {
                        return;
                    }
                    LoadBakuganMark((u8) mItemIds[mScroll + i], (u8) ((mScroll + i) % 4), 2);
                }
            } else {
                for (; i < 3; i++) {
                    if (mScroll + i >= mItemCount) {
                        return;
                    }
                    LoadSmallCard(mItemIds[mScroll + i], (u8) ((mScroll + i) % 4), 2);
                }
            }
            break;
        case LIST_MODE_GALLERY:
            if (mPage == 0) {
                return;
            }
            if (mPage < 7) {
                for (; i < 3; i++) {
                    if (mScroll + i >= mItemCount) {
                        return;
                    }
                    LoadBakuganMark((u8) mItemIds[mScroll + i], (u8) ((mScroll + i) % 4), 2);
                }
            } else {
                for (; i < 3; i++) {
                    if (mScroll + i >= mItemCount) {
                        return;
                    }
                    LoadSmallCard(mItemIds[mScroll + i], (u8) ((mScroll + i) % 4), 2);
                }
            }
            break;
    }
}

BOOL ListScene::func_02080b48(u32 unk1, u32 unk2) {
    BOOL result = FALSE;
    if (unk1 == 0x22) {
        if (unk2 == 0) {
            if (gSaveData[0].mSpecialNameCode0Used != 0) {
                result = TRUE;
            }
        } else {
            result = TRUE;
        }
    } else if ((unk1 != 0x20) & (unk1 != 0x21)) {
        if ((unk1 == 0x24) | (unk1 == 0x25)) {
            if (unk2 == gSaveData[0].mAttribute) {
                result = TRUE;
            }
        } else {
            result = TRUE;
        }
    }
    return result;
}

// Fills mItemIds for the mode and sorts it, then puts the cursor back (mSavedCursorItems[mPage], or the item already in the slot in
// LIST_MODE_EDIT / BATTLE_EDIT) and clamps mScroll; unk1 != 0 scrolls to the cursor. Page 0 of those two modes sets
// mItemCount = 2 with no ids.
void ListScene::BuildList(u32 unk1) {
    u8 i = 0;
    mItemCount = 0;
    switch (mMode) {
        case LIST_MODE_STORE_BAKUGAN:
        case LIST_MODE_STORE_GAUGE:
            if (mMode == LIST_MODE_STORE_BAKUGAN) {
                for (; i < 0x29; i++) {
                    if (TestFlagBit(data_020d4976, i) && sBakuganAttributeFlags[i][mListFilter] != 0 && func_02080b48(i, mListFilter)) {
                        mItemIds[mItemCount] = i;
                        mItemCount++;
                    }
                }
                if (!(gSaveData[0].mUnlockedBakuganBits[4] & 4) && mListFilter == 0 && gSaveData[0].mSpecialNameCode0Used != 0) {
                    mItemIds[mItemCount] = 0x22;
                    mItemCount++;
                }
            } else if (mMode == LIST_MODE_STORE_GAUGE) {
                for (; i < 0x29; i++) {
                    if (func_020231f4(gSaveData, 0, i, mListFilter) != 0) {
                        mItemIds[mItemCount] = i;
                        mItemCount++;
                    }
                }
            }
            SortListSceneItems(this, 1);
            break;
        case LIST_MODE_STORE_CARD: {
            for (u16 k = 1; k < 0xd5; k++) {
                if (TestFlagBit(data_020d497b, k) && !TestFlagBit(data_020d4946, k) && GetCardInfo(k, 1) == mListFilter && GetCardNumber(k) < 0x2bc) {
                    mItemIds[mItemCount] = k;
                    mItemCount++;
                }
            }
            SortListSceneItems(this, 0);
            break;
        }
        case LIST_MODE_EDIT:
        case LIST_MODE_BATTLE_EDIT:
            if (mCharacterId != 0x15) {
                mItemCount = 0;
            } else if (mPage == 0) {
                mItemCount = 2;
                mCursorItem = 0;
            } else if (mPage < 4) {
                if (mFirstSlotOnly != 0) {
                    mItemCount = 0;
                } else {
                    for (; i < 0x29; i++) {
                        if (func_020231f4(gSaveData, 0, i, mListFilter) != 0) {
                            mItemIds[mItemCount] = i;
                            mItemCount++;
                        }
                    }
                    SortListSceneItems(this, 1);
                    u8 j = 0;
                    do {
                        if (mItemIds[j] == mSlotValues[mPage - 1] && mListFilter == mSlotValues[mPage + 8]) {
                            mCursorItem = j;
                        }
                        j++;
                    } while (j < mItemCount);
                }
            } else {
                for (u16 k = 1; k < 0xd5; k++) {
                    if (TestFlagBit(data_020d4946, k) && GetCardInfo(k, 1) == mListFilter && GetCardNumber(k) < 0x2bc) {
                        mItemIds[mItemCount] = k;
                        mItemCount++;
                    }
                }
                SortListSceneItems(this, 0);
                u16 j = 0;
                do {
                    if (mItemIds[j] == mSlotValues[mPage - 1]) {
                        mCursorItem = j;
                    }
                    j++;
                } while (j < mItemCount);
            }
            break;
        case LIST_MODE_GALLERY:
            if (mPage != 0) {
                if (mPage < 7) {
                    for (; i < 0x29; i++) {
                        if (func_020231f4(gSaveData, 0, i, mListFilter) != 0) {
                            mItemIds[mItemCount] = i;
                            mItemCount++;
                        }
                    }
                    SortListSceneItems(this, 1);
                } else {
                    for (u16 k = 1; k < 0xd5; k++) {
                        if (TestFlagBit(data_020d4946, k) && GetCardInfo(k, 1) == mListFilter && GetCardNumber(k) < 0x2bc) {
                            mItemIds[mItemCount] = k;
                            mItemCount++;
                        }
                    }
                    SortListSceneItems(this, 0);
                }
            }
            break;
    }
    if (mMode != LIST_MODE_EDIT && mMode != LIST_MODE_BATTLE_EDIT) {
        mCursorItem = mSavedCursorItems[mPage];
    }
    if (mItemCount == 0) {
        mCursorItem = 0;
    } else if (mCursorItem > mItemCount - 1) {
        mCursorItem = mItemCount - 1;
    }
    mTargetItem = mCursorItem;
    if (unk1 != 0) {
        mScroll = mCursorItem;
    }
    if (mItemCount < 3) {
        mScroll = 0;
    } else if (mScroll >= mItemCount - 3) {
        mScroll = mItemCount - 3;
    }
}

void ListScene::RefreshCommandList() {
    BattleCellPair *pair;
    u8 id;
    u8 kind;
    u32 value;
    if (mCommandList == NULL) {
        return;
    }
    if (mCommandListClosed != 0) {
        return;
    }
    switch (mMode) {
        case LIST_MODE_STORE_BAKUGAN: {
            BattleCellPair cell;
            mCommandList->func_02058240(0);
            if (mSelectingPage != 0) {
                func_02058248(mCommandList, mPage);
                return;
            }
            if (mItemCount == 0) {
                SetCommandListBakuganHidden(mCommandList, 1);
                return;
            }
            SetCommandListBakuganHidden(mCommandList, 0);
            FillInitialEntry(this, (u8) mItemIds[mCursorItem], mListFilter, &cell);
            SetCommandListBakugan(mCommandList, (u8) mItemIds[mCursorItem], mListFilter, -1);
            SetCommandListValues(mCommandList, mListFilter, func_02086548((u8) mItemIds[mCursorItem], mListFilter), &cell);
            return;
        }
        case LIST_MODE_STORE_GAUGE: {
            if (mSelectingPage != 0) {
                func_02058248(mCommandList, mPage);
                return;
            }
            if (mItemCount == 0) {
                SetCommandListBakuganHidden(mCommandList, 1);
                return;
            }
            SetCommandListBakuganHidden(mCommandList, 0);
            pair = (BattleCellPair *) GetBakuganEntry(gSaveData, (u8) mItemIds[mCursorItem], mListFilter);
            SetCommandListBakugan(mCommandList, (u8) mItemIds[mCursorItem], mListFilter, -1);
            SetCommandListValues(mCommandList, mListFilter, func_020231f4(gSaveData, 0, (u8) mItemIds[mCursorItem], mListFilter), pair);
            return;
        }
        case LIST_MODE_EDIT:
        case LIST_MODE_BATTLE_EDIT: {
            if (mPage == 0) {
                return;
            }
            if (mPage >= 4) {
                return;
            }
            if (mSelectingPage != 0) {
                if (mFirstSlotOnly != 0 && mPage != 1) {
                    SetCommandListBakuganHidden(mCommandList, 1);
                    return;
                }
                if (mCharacterId != 0x15) {
                    BattleRecord *rec = &gGameData.mBattlePlayers->mPlayers[mPlayer]->mRecords[(u8) (mPage - 1)];
                    pair  = (BattleCellPair *) rec;
                    value = rec->mUnk_04;
                    id    = mSlotValues[mPage - 1];
                    kind  = mSlotValues[mPage + 8];
                } else {
                    id    = mSlotValues[mPage - 1];
                    kind  = mSlotValues[mPage + 8];
                    pair  = (BattleCellPair *) GetBakuganEntry(gSaveData, id, kind);
                    value = func_020231f4(gSaveData, 0, id, kind);
                }
            } else {
                if (mItemCount == 0) {
                    SetCommandListBakuganHidden(mCommandList, 1);
                    return;
                }
                kind  = mListFilter;
                id    = mItemIds[mCursorItem];
                pair  = (BattleCellPair *) GetBakuganEntry(gSaveData, id, kind);
                value = func_020231f4(gSaveData, 0, id, kind);
            }
            SetCommandListBakuganHidden(mCommandList, 0);
            SetCommandListBakugan(mCommandList, id, kind, -1);
            SetCommandListValues(mCommandList, kind, value, pair);
            return;
        }
        case LIST_MODE_GALLERY: {
            if (mPage == 0) {
                return;
            }
            if (mPage >= 7) {
                return;
            }
            if (mSelectingPage != 0) {
                func_02058248(mCommandList, (u8) (mPage - 1));
                return;
            }
            if (mItemCount == 0) {
                SetCommandListBakuganHidden(mCommandList, 1);
                return;
            }
            SetCommandListBakuganHidden(mCommandList, 0);
            BattleCellPair *pair = (BattleCellPair *) GetBakuganEntry(gSaveData, (u8) mItemIds[mCursorItem], mListFilter);
            SetCommandListBakugan(mCommandList, (u8) mItemIds[mCursorItem], mListFilter, -1);
            u32 value = func_020231f4(gSaveData, 0, (u8) mItemIds[mCursorItem], mListFilter);
            SetCommandListValues(mCommandList, mListFilter, value, pair);
            break;
        }
    }
}

void ListScene::RefreshCommandPage() {
    if (mCommandPage == NULL) {
        return;
    }
    if (mCommandPageClosed != 0) {
        return;
    }
    switch (mMode) {
        case LIST_MODE_STORE_CARD:
            if (mSelectingPage != 0) {
                SetCommandPageCard(mCommandPage, (u16) (mPage + 0xd7), -1);
                return;
            }
            if (mItemCount == 0) {
                if (mPage < 3) {
                    SetCommandPageCard(mCommandPage, 0, -1);
                } else {
                    SetCommandPageCard(mCommandPage, 0xd6, -1);
                }
            } else {
                SetCommandPageCard(mCommandPage, mItemIds[mCursorItem], -1);
            }
            break;
        case LIST_MODE_EDIT:
        case LIST_MODE_BATTLE_EDIT:
            if (mPage < 4) {
                return;
            }
            {
                u16 v;
                if (mSelectingPage != 0) {
                    v = mSlotValues[mPage - 1];
                } else {
                    v = mItemIds[mCursorItem];
                }
                SetCommandPageCard(mCommandPage, (u8) v, -1);
            }
            break;
        case LIST_MODE_GALLERY:
            if (mPage < 7) {
                return;
            }
            if (mSelectingPage != 0) {
                SetCommandPageCard(mCommandPage, (u16) (mPage + 0xd0), -1);
            } else {
                SetCommandPageCard(mCommandPage, mItemIds[mCursorItem], -1);
            }
            break;
    }
}

void ListScene::DrawMoneyAmount(u32 unk1, u32 unk2) {
    u32 over;
    s16 x;
    u8 scale;
    s16 y;
    u8 i;
    if (unk2 == 4) {
        scale = 1;
        x     = 0x5a;
        y     = 0xaf;
    } else {
        y     = mRowSlideY + (unk2 * 0x20 + 0x56);
        scale = 2;
        x     = 0xb0;
    }
    over = gSaveData[0].mMoney < unk1 ? 1 : 0;
    if (unk2 != 4) {
        if (unk1 >= 10000000) {
            over = 0;
        }
        if (over != 0) {
            mSubScreenCellAnims[13]->SetAnimation(5);
        } else {
            mSubScreenCellAnims[13]->SetAnimation(4);
        }
        mSubScreenCellAnims[13]->Draw(1, x, y, scale);
    }
    x = x + 12;
    if (unk1 >= 10000000) {
        mSubScreenCellAnims[13]->SetAnimation(3);
        for (i = 0; i < 7; i++) {
            mSubScreenCellAnims[13]->Draw(1, (s16) (x + i * 5), y, scale);
        }
        return;
    }
    s8 digits[7] = {-1, -1, -1, -1, -1, -1, -1};
    s8 n         = 6;
    do {
        digits[n] = unk1 % 10;
        unk1      = unk1 / 10;
        n--;
    } while (n >= 0 && unk1 != 0);
    if (over != 0) {
        mSubScreenCellAnims[13]->SetAnimation(1);
    } else {
        mSubScreenCellAnims[13]->SetAnimation(0);
    }
    i = 0;
    if (unk2 == 4) {
        for (; i < 7; i++) {
            if (digits[i] >= 0) {
                mSubScreenCellAnims[13]->DrawFrame(1, digits[i], x + i * 7, y, scale);
            }
        }
        mSubScreenCellAnims[17]->Draw(1, (s16) (x - 0x2b), (s16) (y - 3), scale);
    } else {
        for (; i < 7; i++) {
            if (digits[i] >= 0) {
                mSubScreenCellAnims[13]->DrawFrame(1, digits[i], x + i * 5, y, scale);
            }
        }
    }
}

void ListScene::SetItemFlags(u32 unk1, u32 unk2, u32 unk3) {
    WindowItem *items;
    switch (unk1) {
        case 0:
            items = mListWindow->mCursor->mItems;
            mListWindow->mButtons[unk2].mEnabled = unk3 != WINDOW_ITEM_CURSOR_SKIP ? 1 : 0;
            break;
        case 1:
            items = mPromptWindow->mCursor->mItems;
            mPromptWindow->mButtons[unk2].mEnabled = unk3 != WINDOW_ITEM_CURSOR_SKIP ? 1 : 0;
            break;
        case 2:
            items = mPageAndRowCursor->mItems;
            break;
        case 3:
            items = mDeckWindow->mCursor->mItems;
            mDeckWindow->mButtons[unk2].mEnabled = unk3 != WINDOW_ITEM_CURSOR_SKIP ? 1 : 0;
            break;
        default:
            return;
    }
    items[unk2].mFlags = unk3;
}

void ListSceneScrollCallback(void *arg) {
    ListScene *self = (ListScene *) arg;
    reg_G2S_DB_BG2OFS = (u32) ((-0x20 - self->mRowSlideY) << 16) & G2_BGOFS_V_MASK;
}

void ListScene::DrawBakuganMark(u8 unk1, u32 unk2, u32 unk3, s16 unk4, s16 unk5) {
    BOOL second = unk3 == 0 ? SCREEN_MAIN : SCREEN_SUB;
    u16 pos     = 0;
    u8 priority = 0;
    switch (unk3) {
        case 0:
            pos = unk2 * 8 + 0xa0;
            break;
        case 1:
            pos      = unk2 * 8 + 0x280;
            priority = 1;
            break;
        case 2:
            pos      = unk2 * 8 + 0x2a0;
            priority = 2;
            break;
        case 3:
            pos = unk2 * 8 + 0x320;
            break;
        case 4:
            pos = unk2 * 8 + 0x2a0;
            break;
    }
    SpriteManager::Get().Screen(second).AddOam((void *) sCardAndMarkOam[1], 1, (s16) (unk4 + 3), (s16) (unk5 + 3), priority, pos, 1, 0);
    if (unk3 == 0) {
        if (mMainScreenCellAnims[4] == NULL) {
            return;
        }
        mMainScreenCellAnims[4]->DrawFrame(second, unk1, unk4, unk5, priority);
    } else {
        mSubScreenCellAnims[14]->DrawFrame(second, unk1, unk4, unk5, priority);
    }
}

void ListScene::LoadBakuganMark(u32 unk1, u32 unk2, u32 unk3) {
    switch (mMode) {
        case LIST_MODE_STORE_CARD:
            return;
        case LIST_MODE_EDIT:
        case LIST_MODE_BATTLE_EDIT:
            if (mPage >= 4) {
                return;
            }
            break;
        case LIST_MODE_GALLERY:
            if (mPage == 0) {
                return;
            }
            if (mPage >= 7) {
                return;
            }
            break;
    }
    BOOL second = unk3 == 0 ? SCREEN_MAIN : SCREEN_SUB;
    ChangeDir("/Game/Bakugan/mark");
    char path[0x20];
    sprintf(path, "bakugan_s_%03d_ncg.bin", gBakuganNumbers[unk1]);
    u32 offset = 0;
    switch (unk3) {
        case 0:
            offset = unk2 * 0x400 + 0x5000;
            break;
        case 1:
            offset = unk2 * 0x400 + 0x14000;
            break;
        case 2:
            offset = unk2 * 0x400 + 0x15000;
            break;
        case 3:
            offset = unk2 * 0x400 + 0x19000;
            break;
    }
    LoadAndQueueUpload(second, UPLOAD_KIND_OBJ, offset, path, 0, 0, 0);
}

static inline void PrintMessageFW(TextManager &tm, MessageFile *file, int index, void *buffer, s16 x, s16 y, u16 *width) {
    tm.Print(file->GetText(index), file->GetLength(index), buffer, 0x18, 0x10, &x, &y, width, 0, 2, 0x44, 0, 0, 0, 0, 1, 1);
}

void ListScene::PrintBakuganRow(u32 unk1, u32 unk2, u32 unk3, u32 unk4) {
    s16 b = 0;
    s16 a = 0;
    u16 w;
    TextManager::Get().mUse4bppFont = 1;
    a                          = unk2 << 5;
    PrintMessageF(TextManager::Get(), (MessageFile *) mBakuganNameShortMessages, unk1, mTextBuffers[0], b, a);
    b = 0x73;
    PrintMessageF(TextManager::Get(), (MessageFile *) mStorecommentMessages, 0x18, mTextBuffers[0], b, a);
    b = 0x87;
    TextManager::Get().PrintNumber(unk3, 3, 0, mTextBuffers[0], 0x18, 0x10, &b, &a, 0, 2, 0x44, 0, 0, 1, 1);
    b = 0;
    a += 12;
    PrintMessageFW(TextManager::Get(), (MessageFile *) mResultMessages, 5, mTextBuffers[0], b, a, &w);
    b += w;
    if (*(u8 *) &unk4 >= 9) {
        PrintMessageF(TextManager::Get(), (MessageFile *) mResultMessages, 6, mTextBuffers[0], b, a);
    } else {
        b += 7;
        TextManager::Get().PrintNumber(*(u8 *) &unk4 + 1, 1, 0, mTextBuffers[0], 0x18, 0x10, &b, &a, 0, 2, 0x44, 0, 0, 1, 1);
    }
    TextManager::Get().mUse4bppFont = 0;
}

void ListScene::DrawSmallCard(u8 unk1, u8 unk2, u32 unk3, s16 unk4, s16 unk5) {
    BOOL second = unk3 == 0 ? SCREEN_MAIN : SCREEN_SUB;
    u16 pos      = 0;
    u8 priority = 0;
    switch (unk3) {
        case 0:
            pos = unk2 * 4 + 0xc0;
            break;
        case 1:
            pos      = unk2 * 4 + 0x2c0;
            priority = 1;
            break;
        case 2:
            pos      = unk2 * 4 + 0x300;
            priority = 2;
            break;
        case 3:
            pos = unk2 * 4 + 0x340;
            break;
        case 4:
            pos = unk2 * 4 + 0x300;
            break;
    }
    SpriteManager::Get().Screen(second).AddOam((void *) sCardAndMarkOam[0], 1, unk4, unk5, priority, pos, 2, 0);
    if (unk3 == 0) {
        return;
    }
    mSubScreenCellAnims[6]->DrawFrame(second, unk1, unk4, unk5, priority);
}

void ListScene::LoadSmallCard(u32 unk1, u32 unk2, u32 unk3) {
    switch (mMode) {
        case LIST_MODE_STORE_BAKUGAN:
        case LIST_MODE_STORE_GAUGE:
            return;
        case LIST_MODE_EDIT:
        case LIST_MODE_BATTLE_EDIT:
            if (mPage != 0 && mPage < 4 && unk3 != 1) {
                return;
            }
            break;
        case LIST_MODE_GALLERY:
            if (mPage < 7) {
                return;
            }
            break;
    }
    BOOL second = unk3 == 0 ? SCREEN_MAIN : SCREEN_SUB;
    ChangeDir("/Game/Card/Graphic/Small");
    char path[0x20];
    sprintf(path, "card_s_%03d_ncg.bin", GetCardNumber(unk1));
    u32 offset = 0;
    switch (unk3) {
        case 0:
            offset = unk2 * 0x200 + 0x6000;
            break;
        case 1:
            offset = unk2 * 0x200 + 0x16000;
            break;
        case 2:
            offset = unk2 * 0x200 + 0x18000;
            break;
        case 3:
            offset = unk2 * 0x200 + 0x1a000;
            break;
    }
    LoadAndQueueUpload(second, UPLOAD_KIND_OBJ, offset, path, 0, 0, 0);
}

void ListScene::PrintCardNameShort(u32 unk1, u32 unk2) {
    TextManager::Get().mUse4bppFont = 1;
    s16 y = unk2 << 5;
    PrintMessageF(TextManager::Get(), (MessageFile *) mCardNameShortMessages, unk1, mTextBuffers[0], 0, y);
    TextManager::Get().mUse4bppFont = 0;
}

void ListScene::ReleaseMainScreenGraphics() {
    for (u8 i = 0; i < 6; i++) {
        delete mMainScreenCellAnims[i];
        mMainScreenCellAnims[i] = NULL;
    }
    if (mTextBuffers[1] != NULL) {
        OS_Free(mTextBuffers[1]);
        mTextBuffers[1] = NULL;
    }
    if (mGlyphBuffers[1] != NULL) {
        OS_Free(mGlyphBuffers[1]);
        mGlyphBuffers[1] = NULL;
    }
}

void ListScene::InitAndLoadMainScreen() {
    SpriteManager::Get().SetupMode(0, 1);
    GX_SetGraphicsMode(1, 0, 0);
    G2_SetBG0Control(0, 1, 0, 1, 0);
    G2_SetBG1Control(0, 1, 2, 4, 0);
    G2_SetBG2Control(0, 1, 4, 8, 0);
    G2_SetBG3Control(0, 1, 6, 0xc, 0);
    G2_SetBG0Offset(0, 0);
    G2_SetBG1Offset(0, 0);
    G2_SetBG2Offset(0, 0);
    G2_SetBG3Offset(0, 0);
    G2_SetBG0Priority(3);
    G2_SetBG1Priority(3);
    G2_SetBG2Priority(3);
    G2_SetBG3Priority(3);
    GX_SetVisiblePlane(0x11);
    G2_SetBlendBrightness(0x1f, 0);
    GX_SetVisibleWnd(0);
    G2_SetWndOutsidePlane(0x1f, FALSE);
    G2_SetWnd0InsidePlane(0x1f, FALSE);
    G2_SetWnd1InsidePlane(0x1f, FALSE);
    G2_SetWndOBJInsidePlane(0x1f, FALSE);
    G2_SetWnd0Position(0, 0, 0, 0);
    G2_SetWnd1Position(0, 0, 0, 0);
    G2_BlendNone();
    MI_CpuClearFast((void *) 0x6000000, 0x80000);
    MI_CpuClearFast((void *) 0x6400000, 0x40000);
    ChangeDir("/Game/List/");
    switch (mMode) {
        case LIST_MODE_EDIT:
        case LIST_MODE_BATTLE_EDIT:
            LoadAndQueueUpload(0, UPLOAD_KIND_BG_EXT_PLTT, 0, "list_edit_u_bg_00_ncl.bin", 0, 0, 0);
            LoadAndQueueUpload(0, UPLOAD_KIND_BG0_CHAR, 0, "list_edit_u_bg_00_ncg.bin", 0, 0, 0);
            LoadAndQueueUpload(0, UPLOAD_KIND_BG0_SCR, 0, "list_edit_u_bg_00_nsc.bin", 0, 0, 0);
            break;
        case LIST_MODE_GALLERY:
            LoadAndQueueUpload(0, UPLOAD_KIND_BG_EXT_PLTT, 0, "list_gallery_u_bg_00_ncl.bin", 0, 0, 0);
            LoadAndQueueUpload(0, UPLOAD_KIND_BG0_CHAR, 0, "list_gallery_u_bg_00_ncg.bin", 0, 0, 0);
            LoadAndQueueUpload(0, UPLOAD_KIND_BG0_SCR, 0, "list_gallery_u_bg_00_nsc.bin", 0, 0, 0);
            break;
    }
    SpriteScreen &screen = SpriteManager::Get().Screen(0);
    screen.SetObjVramMode(3);
    ChangeDir("/Game/List/");
    screen.LoadCharacters("list_u_obj_00.nce_ncg1D.bin", 0, 0);
    screen.LoadPalette("list_u_obj_00_ncl.bin", 0, 0);
    for (u8 i = 0; i <= 3; i++) {
        mMainScreenCellAnims[i]              = new CellAnimation("list_u_obj_00_nce.bin", 0, 1, 1);
        mMainScreenCellAnims[i]->mCharBase   = screen.GetCharBase(0);
        mMainScreenCellAnims[i]->mPaletteBase     = 0;
    }
    mMainScreenCellAnims[0]->SetAnimation(0);
    mMainScreenCellAnims[1]->SetAnimation(1);
    mMainScreenCellAnims[2]->SetAnimation(2);
    mMainScreenCellAnims[3]->SetAnimation(3);
    ChangeDir("/Game/Bakugan/mark");
    LoadAndQueueUpload(0, UPLOAD_KIND_OBJ_EXT_PLTT, 0x200, "bakugan_s_000_ncl.bin", 0, 0, 0);
    LoadAndQueueUpload(0, UPLOAD_KIND_OBJ_EXT_PLTT, 0, "bakugan_s_base_ncl.bin", 0, 0, 0);
    LoadAndQueueUpload(0, UPLOAD_KIND_OBJ, 0x3000, "bakugan_s_base.nce_ncg1D.bin", 0, 0, 0);
    mMainScreenCellAnims[4]            = new CellAnimation("bakugan_s_base_nce.bin", 0, 1, 1);
    mMainScreenCellAnims[4]->mCharBase = screen.GetCharBase(0x3000);
    mMainScreenCellAnims[4]->mPaletteBase   = 0;
    mMainScreenCellAnims[4]->SetAnimation(0);
    ChangeDir("/Game/2d/union_b/");
    screen.LoadCharacters("union_b_obj_gp.nce_ncg1D.bin", 0x2000, 0);
    screen.LoadPalette("union_b_obj_gp_ncl.bin", 0xa0, 0);
    mMainScreenCellAnims[5]            = new CellAnimation("union_b_obj_gp_nce.bin", 0, 1, 1);
    mMainScreenCellAnims[5]->mCharBase = screen.GetCharBase(0x2000);
    mMainScreenCellAnims[5]->mPaletteBase   = 5;
    mMainScreenCellAnims[5]->SetAnimation(2);
    ChangeDir("/Game/Card/Graphic/Small");
    LoadAndQueueUpload(0, UPLOAD_KIND_OBJ_EXT_PLTT, 0x400, "card_s_000_ncl.bin", 0, 0, 0);
    InitMainScreenText();
    PrintListSceneMainScreenText(this);
}

BOOL ListScene::IsInSlots(u32 unk1, u32 unk2, u32 unk3) {
    if (unk1 != 0) {
        for (u8 i = 0; i <= 2; i++) {
            if (unk2 == mSlotValues[i] && unk3 == mSlotValues[9 + i]) {
                return TRUE;
            }
        }
    } else {
        for (u8 i = 3; i <= 8; i++) {
            if (unk2 == mSlotValues[i]) {
                return TRUE;
            }
        }
    }
    return FALSE;
}

void ListScene::InitMainScreenText() {
    ChangeDir("/font/");
    mTextBufferSizes[1] = TextManager::Get().BufferSize(0x18, 0x14);
    mTextBuffers[1] = OS_Alloc(mTextBufferSizes[1]);
    MI_CpuClear16(mTextBuffers[1], mTextBufferSizes[1]);
    mGlyphCounts[1] = TextManager::Get().GlyphCount(0xf);
    mGlyphBuffers[1] = OS_Alloc((s16) mGlyphCounts[1] * (s16) 0xc);
    TextManager::Get().Init(mGlyphBuffers[1], 0, 0, 0x18, 0x14, 0x42);
    TextManager::Get().SetColor(0, 0, 0xf);
}

void PrintListSceneMainScreenText(ListScene *self) {
    s16 b = 0;
    s16 a = 0;
    self->ClearMainScreenText();
    for (u8 i = 0; i < 14; i++) {
        b = (i / 7) * 0x78 + 0x13;
        s32 col = i % 7;
        a       = col * 0x14 + 5;
        if (col == 6) {
            a += 6;
        }
        TextManager::Get().PrintNumber(self->mGalleryCounts[i].mCount, 3, 0, self->mTextBuffers[1], 0x18, 0x14, &b, &a, 0, 2, 0x42, 0, 0, 1, 1);
        TextManager::Get().Print(&self->mAlphabetChar, 2, self->mTextBuffers[1], 0x18, 0x14, &b, &a, NULL, 0, 2, 0x42, 0, 0, 0, 0, 1, 1);
        TextManager::Get().PrintNumber(self->mGalleryCounts[i].mTotal, 3, 0, self->mTextBuffers[1], 0x18, 0x14, &b, &a, 0, 2, 0x42, 0, 0, 1, 1);
    }
    QueueUpload(SpriteManager::Get().Screen(0).mBgIndex, UPLOAD_KIND_OBJ, 0x10000, self->mTextBuffers[1], self->mTextBufferSizes[1], 0);
}

void ListScene::DrawMainScreenText() {
    SpriteManager::Get().Screen(0).AddOam(mGlyphBuffers[1], (u8) mGlyphCounts[1], 0x40, 0x22, 0, 0x200, 0xf, 0);
}

void ListScene::ClearMainScreenText() {
    if (mTextBuffers[1] != NULL) {
        MI_CpuClear16(mTextBuffers[1], mTextBufferSizes[1]);
    }
}

void ListScene::RefreshCounts() {
    MI_CpuClear16(&mGalleryCounts[0], 0x38);
    mGalleryCounts[0].mTotal = 0x11;
    mGalleryCounts[1].mTotal = 0x11;
    mGalleryCounts[2].mTotal = 0x11;
    mGalleryCounts[3].mTotal = 0x11;
    mGalleryCounts[4].mTotal = 0x12;
    mGalleryCounts[5].mTotal = 0x11;
    mGalleryCounts[6].mTotal = 0x67;
    mGalleryCounts[gSaveData[0].mAttribute].mTotal += 2;
    mGalleryCounts[6].mTotal += 2;
    if (gSaveData[0].mSpecialNameCode0Used != 0) {
        mGalleryCounts[0].mTotal += 1;
        mGalleryCounts[6].mTotal += 1;
    }
    u8 i;
    u8 j;
    for (i = 0; i < 0x29; i++) {
        for (j = 0; j < 6; j++) {
            if (sBakuganAttributeFlags[i][j] != 0) {
                if (func_020231f4(gSaveData, 0, i, j) != 0) {
                    mGalleryCounts[j].mCount += 1;
                    mGalleryCounts[6].mCount += 1;
                }
            }
        }
    }
    for (u16 k = 1; k < 0xd5; k++) {
        if (GetCardNumber(k) < 0x2bc) {
            u8 sel = GetCardInfo(k, 1) + 7;
            mGalleryCounts[sel].mTotal += 1;
            mGalleryCounts[13].mTotal += 1;
            if (TestFlagBit(data_020d4946, k)) {
                mGalleryCounts[sel].mCount += 1;
                mGalleryCounts[13].mCount += 1;
            }
        }
    }
}

u8 ListScene::GetRatioFrame(u32 unk1) {
    s32 a = mGalleryCounts[unk1].mCount;
    if (a == 0) {
        return 0;
    }
    s32 b = mGalleryCounts[unk1].mTotal;
    if (b == 0) {
        return 0;
    }
    u8 v = a * 0x28 / b;
    if (v == 0) {
        return 1;
    }
    if (v >= 0x28) {
        v = 0x28;
    }
    return v;
}

void SetupListTextKind(ListScene *self, u32 mode) {
    u8 i;
    u8 flag  = 1;
    u8 state = 0;
    for (i = 0; i < 5; i++) {
        self->SetItemFlags(1, i, WINDOW_ITEM_CURSOR_SKIP);
    }
    state = 0;
    switch (mode) {
        case LIST_TEXT_KIND_NONE:
            if (self->mCommandList != NULL) {
                self->mCommandList->SetWindowVisible(1);
            }
            if (self->mCommandPage != NULL) {
                self->mCommandPage->SetWindowVisible(1);
            }
            self->mPendingPlanes                   = 0x1d;
            self->mListWindow->mEnabled = 1;
            self->mPromptWindow->mEnabled = 0;
            state                      = 0;
            if (self->mTextKind == 10 || self->mTextKind == LIST_TEXT_KIND_CARD_NAME) {
                self->mSavedCursorItems[self->mPage] = self->mCursorItem;
                self->BuildList(0);
                self->RefreshCommandList();
                self->RefreshCommandPage();
                self->mDirtyFlags[2] = 1;
                self->mDirtyFlags[0] = 1;
            } else if ((u8) (self->mTextKind + 0xf8) <= 1) { // LIST_TEXT_KIND_READ_SLOTS or PICK_SLOTS
                for (state = 0; state < 9; state++) {
                    if (state < 3) {
                        self->LoadBakuganMark((u8) self->mSlotValues[state], state, 0);
                    } else {
                        self->LoadSmallCard(self->mSlotValues[state], (u8) (state - 3), 0);
                    }
                }
                self->mDirtyFlags[1] = 1;
                self->PrintRows();
            } else {
                if (self->mTextKind == 3) {
                    self->RefreshCommandList();
                }
                self->PrintRows();
            }
            self->mTextKind = mode;
            self->mState  = 3;
            return;
        case LIST_TEXT_KIND_PRICE_ABOVE_MONEY:
        case 7:
        case 10:
            state = 0;
            break;
        case LIST_TEXT_KIND_PRICE:
            state = 2;
            break;
        case 3: {
            BattleCellPair *pair = (BattleCellPair *) GetBakuganEntry(gSaveData, (u8) self->mItemIds[self->mCursorItem], self->mListFilter);
            SetCommandListBakugan(self->mCommandList, (u8) self->mItemIds[self->mCursorItem], self->mListFilter, -1);
            u32 value = func_020231f4(gSaveData, state, (u8) self->mItemIds[self->mCursorItem], self->mListFilter);
            SetCommandListValues(self->mCommandList, self->mListFilter, value, pair);
            self->mCommandList->func_02058240(1);
            state = 1;
            flag  = 0;
            break;
        }
        case LIST_TEXT_KIND_READ_SLOTS:
        case LIST_TEXT_KIND_PICK_SLOTS:
            state = 1;
            break;
        case LIST_TEXT_KIND_BAKUGAN_NAME:
        case 5:
            self->LoadBakuganMark((u8) self->mItemIds[self->mCursorItem], state, 3);
            self->LoadBakuganMark((u8) self->mItemIds[self->mCursorItem], 1, 3);
            break;
        case LIST_TEXT_KIND_CARD_NAME:
            self->LoadSmallCard(self->mItemIds[self->mCursorItem], state, 3);
            break;
    }
    if (self->mCommandList != NULL) {
        self->mCommandList->SetWindowVisible(0);
    }
    if (self->mCommandPage != NULL) {
        self->mCommandPage->SetWindowVisible(0);
    }
    GXS_SetVisiblePlane(0x1f);
    self->mListWindow->mEnabled = 0;
    self->mPromptWindow->Reset(-1);
    self->mPromptWindow->mEnabled = 1;
    self->mPromptWindow->mButtons[0].mEnabled = 1;
    switch (state) {
        case 0:
            self->SetItemFlags(1, 3, 1);
            self->mPromptWindow->mCursor->mSelection = 3;
            self->mPromptWindow->mCellAnims[3]->SetAnimation(1);
            self->mPromptWindow->mCellAnims[0]->SetAnimation(9);
            break;
        case 1:
            self->SetItemFlags(1, 1, 1);
            self->SetItemFlags(1, 2, 2);
            if (flag != 0) {
                self->mPromptWindow->mCursor->mSelection = 1;
                self->mPromptWindow->mCellAnims[1]->SetAnimation(4);
            } else {
                self->mPromptWindow->mCursor->mSelection = 2;
                self->mPromptWindow->mCellAnims[2]->SetAnimation(7);
            }
            self->mPromptWindow->mCellAnims[0]->SetAnimation(10);
            break;
        case 2:
            self->SetItemFlags(1, 1, 1);
            self->SetItemFlags(1, 2, 2);
            self->mPromptWindow->mCursor->mSelection = 1;
            self->mPromptWindow->mCellAnims[1]->SetAnimation(4);
            self->mPromptWindow->mCellAnims[0]->SetAnimation(10);
            self->mPromptWindow->mButtons[4].mEnabled = 1;
            break;
    }
    self->mTextKind = mode;
    PrintListSceneSubScreenText(self);
    self->mState  = 4;
    self->mWndMode = 0;
}

void PrintListSceneSubScreenText(ListScene *self) {
    const u8 *base;
    u8 index;
    self->ClearSubScreenText();
    base  = NULL;
    index = 0;
    switch (self->mTextKind) {
        case LIST_TEXT_KIND_NONE:
            break;
        case LIST_TEXT_KIND_PRICE_ABOVE_MONEY:
            index = 3;
            base  = (u8 *) self->mStorecommentMessages;
            break;
        case LIST_TEXT_KIND_PRICE: {
            u8 i;
            u32 msg;
            u32 value;
            s16 x     = 0;
            s16 y     = 0;
            i = 0;
            msg = 0;
            value = 0;
            for (; i < 3; i++) {
                x = 8;
                switch (i) {
                    case 0:
                        y     = 10;
                        msg   = 6;
                        value = gSaveData[0].mMoney;
                        break;
                    case 1:
                        value = self->mPrice;
                        y     = 0x28;
                        msg   = 7;
                        break;
                    case 2:
                        msg   = x;
                        value = gSaveData[0].mMoney - self->mPrice;
                        y     = 0x50;
                        break;
                }
                PrintMessageB(TextManager::Get(), (MessageFile *) self->mStorecommentMessages, msg, self->mTextBuffers[0], x, y);
                x += 0x80;
                TextManager::Get().PrintNumber(value, 7, 0, self->mTextBuffers[0], 0x18, 0x10, &x, &y, 0, 2, 0x44, 0, 1, 1, 1);
            }
            break;
        }
        case 3:
            index = 4;
            base  = (u8 *) self->mStorecommentMessages;
            break;
        case LIST_TEXT_KIND_BAKUGAN_NAME:
        case 5:
            base  = (u8 *) self->mStorecommentMessages;
            index = 9;
            TextManager::Get().mUse4bppFont = 1;
            PrintMessageF(TextManager::Get(), (MessageFile *) self->mBakuganNameMessages, self->mItemIds[self->mCursorItem], self->mTextBuffers[0], 0x1c, 0x38);
            TextManager::Get().mUse4bppFont = 0;
            break;
        case LIST_TEXT_KIND_CARD_NAME:
            base  = (u8 *) self->mStorecommentMessages;
            index = 0x16;
            TextManager::Get().mUse4bppFont = 1;
            PrintMessageF(TextManager::Get(), (MessageFile *) self->mCardNameShortMessages, self->mItemIds[self->mCursorItem], self->mTextBuffers[0], 0x17, 0x38);
            TextManager::Get().mUse4bppFont = 0;
            break;
        case 7:
            base  = (u8 *) self->mStorecommentMessages;
            index = 0xf;
            break;
        case LIST_TEXT_KIND_READ_SLOTS:
            base  = (u8 *) self->mDeckSelectMessages;
            index = 0x13;
            break;
        case LIST_TEXT_KIND_PICK_SLOTS:
            base  = (u8 *) self->mDeckSelectMessages;
            index = 0x15;
            break;
        case 10:
            base  = (u8 *) self->mStorecommentMessages;
            index = 0x15;
            break;
        case 11:
            base = (u8 *) self->mDeckSelectMessages;
            break;
        case 12:
            base  = (u8 *) self->mDeckSelectMessages;
            index = 0x17;
            break;
    }
    if (base != NULL) {
        PrintMessageF3(TextManager::Get(), (MessageFile *) base, index, self->mTextBuffers[0], 0, 0);
    }
    QueueUpload(SpriteManager::Get().Screen(1).mBgIndex, UPLOAD_KIND_OBJ, 0, self->mTextBuffers[0], self->mTextBufferSizes[0], 0);
}

void ListScene::DrawPromptWindowContents() {
    switch (mTextKind) {
        case LIST_TEXT_KIND_NONE:
            return;
        case LIST_TEXT_KIND_BAKUGAN_NAME:
        case 5:
            DrawBakuganMark(mListFilter, 0, 3, 0x23, 0x5a);
            break;
        case LIST_TEXT_KIND_CARD_NAME:
            DrawSmallCard(mListFilter, 0, 3, 0x23, 0x5a);
            break;
    }
    SpriteManager::Get().Screen(1).AddOam(mGlyphBuffers[0], (u8) mGlyphCounts[0], 0x20, 0x28, 0, 0, 0xf, 0);
}

// Also stores func_02086548's value with func_020231c0 for the entry's index, outside the entry itself.
void ResetCursorBakuganEntry(ListScene *self) {
    u8 id     = self->mItemIds[self->mCursorItem];
    int value = GetBakuganEntryIndex(gSaveData, 0, id, self->mListFilter);
    func_020231c0(gSaveData, value, self->func_02086548(id, self->mListFilter));
    BakuganEntry *entry = GetBakuganEntry(gSaveData, id, self->mListFilter);
    u32 index                 = GetBakuganTableIndex(self, id, self->mListFilter);
    entry->mLevel          = 0;
    entry->mGauge             = 0;
    entry->mShotSpeedAdjustIndex             = sInitialEntryFields[index][0];
    entry->mUnk_12            = sInitialEntryFields[index][1];
    entry->mShootCurveIndex             = sInitialEntryFields[index][2];
    entry->mPushTableIndex             = sInitialEntryFields[index][3];
    entry->mRestLimitIndex             = sInitialEntryFields[index][4];
}

BOOL ListScene::IsCommandListOrPageSettingUp() {
    if (mCommandList != NULL && mCommandList->IsSettingUp()) {
        return TRUE;
    }
    if (mCommandPage != NULL && mCommandPage->IsSettingUp()) {
        return TRUE;
    }
    return FALSE;
}

void ListScene::StoreSlotsInSaveData() {
    if (mCharacterId != 0x15) {
        return;
    }
    for (u8 i = 0; i < 3; i++) {
        SetSlotBakugan(gSaveData, 0, i, 0, (u8) mSlotValues[i], (u8) mSlotValues[9 + i]);
    }
    for (u8 i = 0; i < 3; i++) {
        gSaveData[0].mSlots54Values[i] = mSlotValues[3 + i];
    }
    for (u8 i = 0; i < 3; i++) {
        gSaveData[0].mSlots6cValues[i] = mSlotValues[6 + i];
    }
}

void func_020843ac(ListScene *self, u32 unk1) {
    SpriteScreen &screen = SpriteManager::Get().Screen(1);
    if (unk1 != 0) {
        ChangeDir("/Game/Battle/Bg/");
        LoadAndQueueUpload(1, UPLOAD_KIND_BG_EXT_PLTT, 0, "select_arena_b_bg_00_ncl.bin", 0, 0, 0);
        LoadAndQueueUpload(1, UPLOAD_KIND_BG0_CHAR, 0, "select_arena_b_bg_00_ncg.bin", 0, 0, 0);
        LoadAndQueueUpload(1, UPLOAD_KIND_BG0_SCR, 0, "select_arena_b_bg_00_nsc.bin", 0, 0, 0);
        ChangeDir("/Game/Deck/");
        screen.LoadCharacters("select_deck_b_obj_01.nce_ncg1D.bin", 0x1c000, 0);
        screen.LoadPalette("select_deck_b_obj_01_ncl.bin", 0xc0, 0);
        ChangeDir("/Game/Battle/Obj/");
        screen.LoadCharacters("battle_b_obj_wait.nce_ncg1D.bin", 0x1e000, 0);
        screen.LoadPalette("battle_b_obj_ability_ncl.bin", 0x100, 0);
        GXS_SetVisiblePlane(0x11);
        self->mDeckWindow->mCursor->mSelection = 1;
        self->mDeckWindow->mCellAnims[1]->SetAnimation(4);
        self->SetItemFlags(3, 0, WINDOW_ITEM_NO_SLIDE_OUT);
        self->SetItemFlags(3, 1, WINDOW_ITEM_NO_SLIDE_OUT);
        self->SetItemFlags(0, 0, WINDOW_ITEM_CURSOR_SKIP);
        self->SetItemFlags(0, 1, WINDOW_ITEM_CURSOR_SKIP);
        self->SetItemFlags(0, 2, WINDOW_ITEM_CURSOR_SKIP);
        self->SetItemFlags(0, 3, WINDOW_ITEM_CURSOR_SKIP);
        self->SetItemFlags(0, 4, WINDOW_ITEM_CURSOR_SKIP);
        self->SetItemFlags(0, 5, WINDOW_ITEM_CURSOR_SKIP);
        self->SetItemFlags(0, 6, WINDOW_ITEM_CURSOR_SKIP);
        if (self->mPage != 0) {
            if (self->mMainScreenState == 0) {
                self->mMainScreenState = 1;
            }
        }
        self->mPage = 0;
        self->mTargetPage = 0;
        self->mCursorPage = 0;
        self->mPageScroll = 0;
        self->mItemCount = 2;
        self->mCursorItem = 0;
        self->mTargetItem = 0;
        self->mScroll = 0;
        if (self->mCharacterId != 0x15) {
            self->mItemCount = 0;
        }
        self->SetMessage(5);
        self->mWndMode = 0;
        self->ApplyWndMode();
    } else {
        ChangeDir("/Game/List/");
        LoadAndQueueUpload(1, UPLOAD_KIND_BG_EXT_PLTT, 0, "list_edit_b_bg_00_ncl.bin", 0, 0, 0);
        LoadAndQueueUpload(1, UPLOAD_KIND_BG_EXT_PLTT, 0x4000, "list_edit_b_bg_00_ncl.bin", 0, 0, 0);
        LoadAndQueueUpload(1, UPLOAD_KIND_BG_EXT_PLTT, 0x6000, "list_edit_b_bg_00_ncl.bin", 0, 0, 0);
        LoadAndQueueUpload(1, UPLOAD_KIND_BG0_CHAR, 0, "list_edit_b_bg_00_ncg.bin", 0, 0, 0);
        LoadAndQueueUpload(1, UPLOAD_KIND_BG0_SCR, 0, "list_edit_b_bg_00_nsc.bin", 0, 0, 0);
        LoadAndQueueUpload(1, UPLOAD_KIND_BG2_SCR, 0, "list_edit_b_bg_01_nsc.bin", 0, 0, 0);
        LoadAndQueueUpload(1, UPLOAD_KIND_BG3_SCR, 0, "list_edit_b_bg_02_nsc.bin", 0, 0, 0);
        screen.LoadPalette("list_b_obj_00_ncl.bin", 0xc0, 0);
        ChangeDir("/Game/2d/union_b/");
        screen.LoadCharacters("union_b_obj_point.nce_ncg1D.bin", 0x1c000, 0);
        screen.LoadPalette("union_b_obj_point_ncl.bin", 0x60, 0);
        screen.LoadCharacters("union_b_obj_page.nce_ncg1D.bin", 0x1d000, 0);
        screen.LoadPalette("union_b_obj_page_ncl.bin", 0, 0);
        GXS_SetVisiblePlane(0x1d);
        self->SetItemFlags(3, 0, WINDOW_ITEM_CURSOR_SKIP);
        self->SetItemFlags(3, 1, WINDOW_ITEM_CURSOR_SKIP);
        self->mDirtyFlags[0] = 1;
        self->mDirtyFlags[1] = 1;
        self->mDirtyFlags[2] = 1;
        self->SetMessage(-1);
        self->mWndMode = 2;
    }
}

void CopySlotsToFirstPlayer(ListScene *self) {
    for (u8 i = 0; i < 3; i++) {
        BattleRecord *record = &gGameData.mBattlePlayers->mPlayers[0]->mRecords[i];
        record->mBakuganId      = self->mSlotValues[i];
        record->mUnk_04      = func_020231f4(gSaveData, 0, (u8) self->mSlotValues[i], (u8) self->mSlotValues[i + 9]);
        record->mAttribute      = self->mSlotValues[i + 9];
        record->mStatus      = 0;
        record->mUnk_0b      = record->mUnk_04 / 10;
        MI_CpuCopy16(GetBakuganEntry(gSaveData, record->mBakuganId, record->mAttribute), &gGameData.mBattlePlayers->mPlayers[0]->mRecords[i], 4);
    }
    for (u8 i = 0; i < 3; i++) {
        BattlePlayerSlot *slot = &gGameData.mBattlePlayers->mPlayers[0]->mSlots54[i];
        slot->mValue         = self->mSlotValues[3 + i];
        slot->mFlag          = 0;
    }
    for (u8 i = 0; i < 3; i++) {
        BattlePlayerSlot *slot = &gGameData.mBattlePlayers->mPlayers[0]->mSlots6c[i];
        slot->mValue         = self->mSlotValues[6 + i];
        slot->mFlag          = 0;
    }
}

BOOL ExchangeRecords(ListScene *self) {
    for (u8 i = 0; i < 3; i++) {
        if (self->mCharacterId != 0x15) {
            BattleRecord *record = &gGameData.mBattlePlayers->mPlayers[self->mPlayer]->mRecords[i];
            MESSAGE->mBakuganIds[i]     = record->mBakuganId;
            MESSAGE->mUnk_04[i]     = record->mUnk_04;
            MESSAGE->mAttributes[i]     = record->mAttribute;
        } else {
            MESSAGE->mBakuganIds[i] = self->mSlotValues[i];
            MESSAGE->mUnk_04[i] = func_020231f4(gSaveData, 0, (u8) self->mSlotValues[i], (u8) self->mSlotValues[i + 9]);
            MESSAGE->mAttributes[i] = self->mSlotValues[i + 9];
        }
    }
    BattlePlayerList *players = gGameData.mBattlePlayers;
    MESSAGE->mRecordCount          = 3;
    if (SetMessageTypeAndCheckAll(players, 3)) {
    u32 count = WirelessManager::Get().GetRecvBlockCount();
    u8 j = 0;
    do {
        u8 length = ((BattleListMessage *) &gGameData.mBattlePlayers->mPlayers[j]->mMsg)->mRecordCount;
        for (u8 k = 0; k < 6; k++) {
            BattleRecord *record = &gGameData.mBattlePlayers->mPlayers[j]->mRecords[k];
            if (k < length) {
                record->mBakuganId = ((BattleListMessage *) &gGameData.mBattlePlayers->mPlayers[j]->mMsg)->mBakuganIds[k];
                record->mUnk_04 = ((BattleListMessage *) &gGameData.mBattlePlayers->mPlayers[j]->mMsg)->mUnk_04[k];
                record->mAttribute = ((BattleListMessage *) &gGameData.mBattlePlayers->mPlayers[j]->mMsg)->mAttributes[k];
                record->mStatus = 0;
                record->mUnk_0b = record->mUnk_04 / 10;
            } else {
                record->mBakuganId = 0;
                record->mUnk_04 = 0;
                record->mAttribute = 0;
                record->mStatus = 3;
                record->mUnk_0b = 0;
            }
        }
        j++;
    } while (j < count);
    return TRUE;
    }
    return FALSE;
}

BOOL ExchangeSlots(ListScene *self) {
    for (u8 i = 0; i < 3; i++) {
        if (self->mCharacterId != 0x15) {
            SLOT_MESSAGE->mSlots54Values[i] = gGameData.mBattlePlayers->mPlayers[self->mPlayer]->mSlots54[i].mValue;
        } else {
            SLOT_MESSAGE->mSlots54Values[i] = self->mSlotValues[3 + i];
        }
    }
    for (u8 i = 0; i < 3; i++) {
        if (self->mCharacterId != 0x15) {
            SLOT_MESSAGE->mSlots6cValues[i] = gGameData.mBattlePlayers->mPlayers[self->mPlayer]->mSlots6c[i].mValue;
        } else {
            SLOT_MESSAGE->mSlots6cValues[i] = self->mSlotValues[6 + i];
        }
    }
    SLOT_MESSAGE->mSlots54Count = 3;
    SLOT_MESSAGE->mSlots6cCount = 3;
    if (SetMessageTypeAndCheckAll(gGameData.mBattlePlayers, 4)) {
        u32 count = WirelessManager::Get().GetRecvBlockCount();
        u8 j      = 0;
        do {
            u8 length = ((BattleSlotMessage *) &gGameData.mBattlePlayers->mPlayers[j]->mMsg)->mSlots54Count;
            for (u8 k = 0; k < 6; k++) {
                BattlePlayer *player    = gGameData.mBattlePlayers->mPlayers[j];
                BattlePlayerSlot *slot = &player->mSlots54[k];
                if (k < length) {
                    slot->mValue = ((BattleSlotMessage *) &player->mMsg)->mSlots54Values[k];
                    slot->mFlag  = 0;
                } else {
                    slot->mValue = 0;
                    slot->mFlag  = 3;
                }
            }
            u8 length2 = ((BattleSlotMessage *) &gGameData.mBattlePlayers->mPlayers[j]->mMsg)->mSlots6cCount;
            for (u8 k = 0; k < 6; k++) {
                BattlePlayer *player    = gGameData.mBattlePlayers->mPlayers[j];
                BattlePlayerSlot *slot = &player->mSlots6c[k];
                if (k < length2) {
                    slot->mValue = ((BattleSlotMessage *) &player->mMsg)->mSlots6cValues[k];
                    slot->mFlag  = 0;
                } else {
                    slot->mValue = 0;
                    slot->mFlag  = 3;
                }
            }
            j++;
        } while (j < count);
        return TRUE;
    }
    return FALSE;
}

BOOL ExchangeRandomSeed(ListScene *self) {
    gSendMessage.mUnk_04 = MATH_Rand32(&gRandom, 0);
    if (SetMessageTypeAndCheckAll(gGameData.mBattlePlayers, 2)) {
        u8 *info = WirelessManager::Get().GetRecvBlock(0);
        if (info != NULL) {
            SeedRandom(*(u32 *) (info + 4));
        }
        return TRUE;
    }
    return FALSE;
}

BOOL ExchangeCharacterLooks(ListScene *self) {
    MIi_CpuCopyFast(gCharacterLook, (u8 *) &gSendMessage + 4, 8);
    if (SetMessageTypeAndCheckAll(gGameData.mBattlePlayers, 6)) {
        u32 count = WirelessManager::Get().GetRecvBlockCount();
        u8 j      = 0;
        do {
            BattlePlayer *player = gGameData.mBattlePlayers->mPlayers[j];
            MIi_CpuCopyFast((u8 *) player + 0x104, (u8 *) player + 0xa8, 8);
            j++;
        } while (j < count);
        return TRUE;
    }
    return FALSE;
}

BOOL ExchangeRecordHeads(ListScene *self) {
    for (u8 i = 0; i < 3; i++) {
        if (self->mCharacterId != 0x15) {
            MIi_CpuCopy16(&gGameData.mBattlePlayers->mPlayers[self->mPlayer]->mRecords[i], (u8 *) &gSendMessage + 4 + i * 4, 4);
        } else {
            u8 id;
            u32 kind;
            u32 invalid = 0;
            id   = GetSlotBakuganId(gSaveData, 0, i);
            kind = GetSlotBakuganAttribute(gSaveData, 0, i);
            if (id >= 0x29) {
                invalid = 1;
            }
            if (invalid != 0) {
                MI_CpuClear16((u8 *) &gSendMessage + 4 + i * 4, 4);
            } else {
                MIi_CpuCopy16(GetBakuganEntry(gSaveData, id, kind), (u8 *) &gSendMessage + 4 + i * 4, 4);
            }
        }
    }
    if (SetMessageTypeAndCheckAll(gGameData.mBattlePlayers, 5)) {
        u32 count = WirelessManager::Get().GetRecvBlockCount();
        u8 j      = 0;
        do {
            u8 k = 0;
            do {
                BattlePlayer *player = gGameData.mBattlePlayers->mPlayers[j];
                MIi_CpuCopyFast((u8 *) player + 0x104 + k * 4, &player->mRecords[k], 4);
                k++;
            } while (k < 3);
            j++;
        } while (j < count);
        return TRUE;
    }
    return FALSE;
}

void ListScene::UpdateMainScreenAndDraw() {
    UpdateMainScreen();
    DrawMainScreen();
    DrawPromptWindowContents();
    if (mMessageWindow != NULL && mTextKind == LIST_TEXT_KIND_NONE) {
        mMessageWindow->Update();
    }
    if (mState < 0x17) {
        DrawWindows();
    }
}

void SetupDeckTextKind(ListScene *self, u32 mode) {
    u8 i;
    u8 flag  = 1;
    u8 state = 0;
    for (i = 0; i < 5; i++) {
        self->SetItemFlags(1, i, WINDOW_ITEM_CURSOR_SKIP);
    }
    state = 0;
    switch (mode) {
        case LIST_TEXT_KIND_NONE:
            self->mPendingPlanes                      = 0x11;
            self->mDeckWindow->mEnabled             = 1;
            self->mDeckWindow->mCursor->mSelection = state;
            self->mDeckWindow->mCellAnims[0]->SetAnimation(1);
            self->SetItemFlags(3, state, WINDOW_ITEM_NO_SLIDE_OUT);
            self->SetItemFlags(3, 1, WINDOW_ITEM_NO_SLIDE_OUT);
            self->mPromptWindow->mEnabled = state;
            self->mTextKind          = mode;
            self->mState           = 9;
            return;
        case 0xb:
            state = 1;
            flag  = 0;
            break;
        case 0xc:
            break;
    }
    GXS_SetVisiblePlane(0x13);
    self->mDeckWindow->mEnabled = 0;
    self->SetItemFlags(3, 0, WINDOW_ITEM_CURSOR_SKIP);
    self->SetItemFlags(3, 1, WINDOW_ITEM_CURSOR_SKIP);
    self->mPromptWindow->Reset(-1);
    self->mPromptWindow->mEnabled             = 1;
    self->mPromptWindow->mButtons[0].mEnabled = 1;
    switch (state) {
        case 0:
            self->SetItemFlags(1, 3, 1);
            self->mPromptWindow->mCursor->mSelection = 3;
            self->mPromptWindow->mCellAnims[3]->SetAnimation(1);
            self->mPromptWindow->mCellAnims[0]->SetAnimation(9);
            break;
        case 1:
            self->SetItemFlags(1, 1, 1);
            self->SetItemFlags(1, 2, 2);
            if (flag != 0) {
                self->mPromptWindow->mCursor->mSelection = 1;
                self->mPromptWindow->mCellAnims[1]->SetAnimation(4);
            } else {
                self->mPromptWindow->mCursor->mSelection = 2;
                self->mPromptWindow->mCellAnims[2]->SetAnimation(7);
            }
            self->mPromptWindow->mCellAnims[0]->SetAnimation(10);
            break;
    }
    self->mTextKind = mode;
    PrintListSceneSubScreenText(self);
    self->mState = 0x14;
}

void ListScene::SetMessage(s32 unk1) {
    if (unk1 >= 0) {
        mMessageWindow->StartMessage(unk1, 0, 1);
        return;
    }
    switch (mMode) {
        case LIST_MODE_STORE_BAKUGAN:
        case LIST_MODE_STORE_GAUGE:
        case LIST_MODE_STORE_CARD:
        case LIST_MODE_GALLERY:
            break;
        case LIST_MODE_EDIT:
        case LIST_MODE_BATTLE_EDIT:
            if (mPage == 0) {
                if (mSelectingPage != 0) {
                    mMessageWindow->StartMessage(6, 0, 1);
                } else {
                    mMessageWindow->StartMessage((s16) (mCursorItem + 7), 0, 1);
                }
            } else {
                mMessageWindow->StartMessage((s16) (mPage + 8), 0, 1);
            }
            break;
    }
}

// Fills mSlotValues: [0..2] / [9..11] the bakugan id / attribute pairs with a non-zero func_020231f4, largest first
// (ids 0x24 / 0x25 are taken only with gSaveData's mAttribute, and then into [0]), then [3..8] from the cards
// FillPickListWithShuffledCards(0..5) lists. [3..5] are always written; [6..8] only when PickSlot6/7/8Card finds a
// match, else they keep their value.
void PickSlots(ListScene *self) {
    u32 found = 0;
    u8 i;
    u8 j;
    self->mPickCount = 0;
    for (i = 0; i < 0x29; i++) {
        for (j = 0; j < 6; j++) {
            if (func_020231f4(gSaveData, 0, i, j) != 0) {
                if ((u8) (i + 0xdc) <= 1) {
                    if (j == gSaveData[0].mAttribute) {
                        self->mSlotValues[0] = i;
                        found            = 1;
                        self->mSlotValues[9] = j;
                    }
                } else {
                    self->mPickList[self->mPickCount] = j + i * 6;
                    self->mPickCount++;
                }
            }
        }
    }
    ShufflePickList(self);
    u8 s;
    for (s = 0; s < self->mPickCount - 1; s++) {
        u8 t;
        for (t = s + 1; t < self->mPickCount; t++) {
            u16 a = self->mPickList[s];
            u16 b = self->mPickList[t];
            u32 va = func_020231f4(gSaveData, 0, (u8) (a / 6), (u8) (a % 6));
            u32 vb = func_020231f4(gSaveData, 0, (u8) (b / 6), (u8) (b % 6));
            if (va < vb) {
                u16 tmp           = self->mPickList[s];
                self->mPickList[s] = self->mPickList[t];
                self->mPickList[t] = tmp;
            }
        }
    }
    if (found != 0) {
        self->mSlotValues[1]  = self->mPickList[0] / 6;
        self->mSlotValues[10] = self->mPickList[0] % 6;
        self->mSlotValues[2]  = self->mPickList[1] / 6;
        self->mSlotValues[11] = self->mPickList[1] % 6;
    } else {
        self->mSlotValues[0]  = self->mPickList[0] / 6;
        self->mSlotValues[9]  = self->mPickList[0] % 6;
        self->mSlotValues[1]  = self->mPickList[1] / 6;
        self->mSlotValues[10] = self->mPickList[1] % 6;
        self->mSlotValues[2]  = self->mPickList[2] / 6;
        self->mSlotValues[11] = self->mPickList[2] % 6;
    }
    FillPickListWithShuffledCards(self, 0);
    PickSlot3Card(self);
    FillPickListWithShuffledCards(self, 1);
    SortPickListByAttributeValue(self);
    self->mSlotValues[4] = self->mPickList[0];
    FillPickListWithShuffledCards(self, 2);
    SortPickListByAttributeValue(self);
    if (self->mPickList[0] == 0x50 && MATH_Rand32(&gRandom, 100) > 0x32) {
        self->mSlotValues[5] = self->mPickList[1];
    } else {
        self->mSlotValues[5] = self->mPickList[0];
    }
    FillPickListWithShuffledCards(self, 3);
    PickSlot6Card(self);
    FillPickListWithShuffledCards(self, 4);
    PickSlot7Card(self);
    FillPickListWithShuffledCards(self, 5);
    PickSlot8Card(self);
}

void PickSlot3Card(ListScene *self) {
    u8 j;
    u8 i = 0;
    do {
        j = 0;
        do {
            if (CardMatchesBakuganId(self, self->mPickList[j], (u8) self->mSlotValues[i])) {
                self->mSlotValues[3] = self->mPickList[j];
                return;
            }
            j++;
        } while (j < self->mPickCount);
        i++;
    } while (i < 3 && self->mFirstSlotOnly == 0);
    SortPickListByAttributeValue(self);
    self->mSlotValues[3] = self->mPickList[0];
}

BOOL CardMatchesBakuganId(ListScene *self, u32 unk1, u32 unk2) {
    u8 value = unk1 - 1;
    if (unk1 >= 7) {
        value = value + 1;
    }
    return unk2 == value ? 1 : 0;
}

void PickSlot6Card(ListScene *self) {
    const u8 order[4] = {3, 0, 1, 2};
    u8 c;
    u8 b;
    u8 a = 0;
    do {
        u8 wanted = order[a];
        b         = 0;
        do {
            c = 0;
            do {
                if (wanted == GetCardInfo(self->mPickList[c], 3) && CardMatchesAttribute(self, self->mPickList[c], (u8) self->mSlotValues[9 + b])) {
                    self->mSlotValues[6] = self->mPickList[c];
                    return;
                }
                c++;
            } while (c < self->mPickCount);
            b++;
        } while (b < 3 && self->mFirstSlotOnly == 0);
        a++;
    } while (a < 4);
}

// TRUE for card 0x68 + k, 0x70 + k or 0x80 + k with k == unk2 (0..5), and for 0x6e / 0x6f / 0x76 / 0x77 / 0x86 / 0x87
// and 0x78..0x7f whatever unk2 is; FALSE for every other card.
BOOL CardMatchesAttribute(ListScene *self, u32 unk1, u32 unk2) {
    switch (unk1) {
        case 0x68:
        case 0x70:
        case 0x80:
            if (unk2 == 0) {
                return TRUE;
            }
            break;
        case 0x69:
        case 0x71:
        case 0x81:
            if (unk2 == 1) {
                return TRUE;
            }
            break;
        case 0x6a:
        case 0x72:
        case 0x82:
            if (unk2 == 2) {
                return TRUE;
            }
            break;
        case 0x6b:
        case 0x73:
        case 0x83:
            if (unk2 == 3) {
                return TRUE;
            }
            break;
        case 0x6c:
        case 0x74:
        case 0x84:
            if (unk2 == 4) {
                return TRUE;
            }
            break;
        case 0x6d:
        case 0x75:
        case 0x85:
            if (unk2 == 5) {
                return TRUE;
            }
            break;
        case 0x6e:
        case 0x76:
        case 0x86:
            return TRUE;
        case 0x6f:
        case 0x77:
        case 0x87:
            return TRUE;
        case 0x78:
        case 0x79:
        case 0x7a:
        case 0x7b:
        case 0x7c:
        case 0x7d:
        case 0x7e:
        case 0x7f:
            return TRUE;
    }
    return FALSE;
}

void PickSlot7Card(ListScene *self) {
    u8 c;
    u8 a = 0;
    do {
        c = 0;
        do {
            if (GetCardInfo(self->mPickList[c], 3) == 5 && CardMatchesBakuganIdAndAttribute(self, self->mPickList[c], (u8) self->mSlotValues[a], (u8) self->mSlotValues[9 + a])) {
                self->mSlotValues[7] = self->mPickList[c];
                return;
            }
            c++;
        } while (c < self->mPickCount);
        a++;
    } while (a < 3 && self->mFirstSlotOnly == 0);
    u8 k = 0;
    do {
        if (self->mPickList[k] == 0x8e) {
            self->mSlotValues[7] = self->mPickList[k];
            return;
        }
        k++;
    } while (k < self->mPickCount);
    k = 0;
    do {
        if (self->mPickList[k] == 0x8d) {
            self->mSlotValues[7] = self->mPickList[k];
            return;
        }
        k++;
    } while (k < self->mPickCount);
    k = 0;
    do {
        if ((u16) (self->mPickList[k] + 0xff71) <= 2) {
            self->mSlotValues[7] = self->mPickList[k];
            return;
        }
        k++;
    } while (k < self->mPickCount);
    k = 0;
    do {
        if (self->mPickList[k] == 0x8c) {
            self->mSlotValues[7] = self->mPickList[k];
            return;
        }
        k++;
    } while (k < self->mPickCount);
    u8 a2 = 0;
    do {
        u8 c2 = 0;
        do {
            if (GetCardInfo(self->mPickList[c2], 3) == 4 && CardMatchesBakuganIdAndAttribute(self, self->mPickList[c2], (u8) self->mSlotValues[a2], (u8) self->mSlotValues[9 + a2])) {
                self->mSlotValues[7] = self->mPickList[c2];
                return;
            }
            c2++;
        } while (c2 < self->mPickCount);
        a2++;
    } while (a2 < 3 && self->mFirstSlotOnly == 0);
}

BOOL CardMatchesBakuganIdAndAttribute(ListScene *self, u32 unk1, u32 unk2, u32 unk3) {
    switch (unk1) {
        case 0x88:
        case 0x89:
        case 0x8a:
        case 0x8b:
        case 0x8c:
        case 0x8d:
        case 0x8e:
        case 0x8f:
        case 0x90:
        case 0x91:
            return TRUE;
        case 0x92:
            if (unk2 == 0x0 && unk3 == 0) {
                return TRUE;
            }
            break;
        case 0x93:
            if (unk2 == 0x1 && unk3 == 0) {
                return TRUE;
            }
            break;
        case 0x9f:
            if (unk2 == 0x10 && unk3 == 0) {
                return TRUE;
            }
            break;
        case 0x96:
            if (unk2 == 0x4 && unk3 == 1) {
                return TRUE;
            }
            break;
        case 0x9e:
            if (unk2 == 0xf && unk3 == 1) {
                return TRUE;
            }
            break;
        case 0x97:
            if (unk2 == 0x7 && unk3 == 2) {
                return TRUE;
            }
            break;
        case 0x98:
            if (unk2 == 0x8 && unk3 == 2) {
                return TRUE;
            }
            break;
        case 0x9d:
            if (unk2 == 0xe && unk3 == 2) {
                return TRUE;
            }
            break;
        case 0x94:
            if (unk2 == 0x2 && unk3 == 3) {
                return TRUE;
            }
            break;
        case 0x95:
            if (unk2 == 0x3 && unk3 == 3) {
                return TRUE;
            }
            break;
        case 0xa0:
            if (unk2 == 0x11 && unk3 == 3) {
                return TRUE;
            }
            break;
        case 0x9b:
            if (unk2 == 0xb && unk3 == 4) {
                return TRUE;
            }
            break;
        case 0x9c:
            if (unk2 == 0xc && unk3 == 4) {
                return TRUE;
            }
            break;
        case 0x99:
            if (unk2 == 0x9 && unk3 == 5) {
                return TRUE;
            }
            break;
        case 0x9a:
            if (unk2 == 0xa && unk3 == 5) {
                return TRUE;
            }
            break;
        case 0xa1:
            if (unk2 == 0x12 && unk3 == 5) {
                return TRUE;
            }
            break;
        case 0xa2:
            if (unk2 == 0x24) {
                return TRUE;
            }
            break;
        case 0xa3:
            if (unk2 == 0x25) {
                return TRUE;
            }
            break;
        case 0xa4:
            if (unk2 == 0x26) {
                return TRUE;
            }
            break;
        case 0xa5:
            if (unk2 == 0x27) {
                return TRUE;
            }
            break;
        case 0xa7:
            if (unk2 == 0xd) {
                return TRUE;
            }
            break;
        case 0xa6:
            if ((u8) (unk2 + 0xfb) <= 1) {
                return TRUE;
            }
            break;
    }
    return FALSE;
}

void PickSlot8Card(ListScene *self) {
    SortPickListByAttributeValue(self);
    u8 i = 0;
    do {
        if (CardMatchesSlotAttributes(self, self->mPickList[i])) {
            self->mSlotValues[8] = self->mPickList[i];
            return;
        }
        i++;
    } while (i < self->mPickCount);
}

// TRUE for any card not listed (default); FALSE for 0xc1..0xc6; 0xbe..0xc0, 0xc7 and 0xc8 only when the slot
// attributes contain their two or three values, and never while mFirstSlotOnly.
BOOL CardMatchesSlotAttributes(ListScene *self, u32 unk1) {
    switch (unk1) {
        case 0xbe:
            if (self->mFirstSlotOnly != 0) {
                return FALSE;
            }
            if (ListSceneHasAttribute(self, 0) && ListSceneHasAttribute(self, 4)) {
                return TRUE;
            }
            break;
        case 0xbf:
            if (self->mFirstSlotOnly != 0) {
                return FALSE;
            }
            if (ListSceneHasAttribute(self, 5) && ListSceneHasAttribute(self, 3)) {
                return TRUE;
            }
            break;
        case 0xc0:
            if (self->mFirstSlotOnly != 0) {
                return FALSE;
            }
            if (ListSceneHasAttribute(self, 2) && ListSceneHasAttribute(self, 1)) {
                return TRUE;
            }
            break;
        case 0xc1:
        case 0xc2:
        case 0xc3:
        case 0xc4:
        case 0xc5:
        case 0xc6:
            return FALSE;
        case 0xc7:
            if (self->mFirstSlotOnly != 0) {
                return FALSE;
            }
            if (ListSceneHasAttribute(self, 0) && ListSceneHasAttribute(self, 1) && ListSceneHasAttribute(self, 3)) {
                return TRUE;
            }
            break;
        case 0xc8:
            if (self->mFirstSlotOnly != 0) {
                return FALSE;
            }
            if (ListSceneHasAttribute(self, 2) && ListSceneHasAttribute(self, 4) && ListSceneHasAttribute(self, 5)) {
                return TRUE;
            }
            break;
        default:
            return TRUE;
    }
    return FALSE;
}

BOOL ListSceneHasAttribute(ListScene *self, u32 unk1) {
    u8 i = 0;
    do {
        if (unk1 == self->mSlotValues[9 + i]) {
            return TRUE;
        }
        i++;
    } while (i < 3);
    return FALSE;
}

u32 ListScene::GetEntryPrice(u32 unk1, u32 unk2) {
    BakuganEntry *pair = GetBakuganEntry(gSaveData, unk1, unk2);
    if ((s32) pair->mLevel >= 9) {
        return 10000000;
    }
    u32 cost = GetPrice(unk1, unk2);
    if (cost == 0) {
        cost = 5000;
    }
    u32 price = cost * sPricePercents[pair->mLevel] / 100;
    return price - price * pair->mGauge / ((pair->mLevel + 1) * 300);
}

BOOL ListScene::ShouldApplyTargetPage() {
    switch (mListInputState) {
        case 2:
            if (mCursorPage <= mFirstPage) {
                return TRUE;
            }
            if (InputManager::Get().IsDown(KEY_LEFT) != 0) {
                return FALSE;
            }
            break;
        case 4:
            if (mCursorPage >= mPageCount - 1) {
                return TRUE;
            }
            if (InputManager::Get().IsDown(KEY_RIGHT) != 0) {
                return FALSE;
            }
            break;
        case 6:
            if (mCursorPage <= mFirstPage) {
                return TRUE;
            }
            if (mListWindow->mCursor->mInputItem == 1) {
                return FALSE;
            }
            break;
        case 8:
            if (mCursorPage >= mPageCount - 1) {
                return TRUE;
            }
            if (mListWindow->mCursor->mInputItem == 2) {
                return FALSE;
            }
            break;
    }
    return TRUE;
}

s32 ListScene::func_0208644c() {
    u16 total = 0;
    u8 i      = 0;
    do {
        if (mCharacterId != 0x15) {
            total += gGameData.mBattlePlayers->mPlayers[mPlayer]->mRecords[i].mUnk_04;
        } else {
            total += func_020231f4(gSaveData, 0, (u8) mSlotValues[i], (u8) mSlotValues[9 + i]);
        }
        i++;
    } while (i < 3 && mFirstSlotOnly == 0);
    return total;
}

u32 ListScene::GetPrice(u32 unk1, u32 unk2) {
    u32 index = GetBakuganTableIndex(this, unk1, unk2);
    if (gSaveData[0].mCreditsDone == 0 && index == 0x24) {
        return Table_020a3860()[index].mPriceTens * 10;
    }
    return sBakuganPriceEntries[index].mPriceTens * 10;
}

u32 ListScene::func_02086548(u32 unk1, u32 unk2) {
    u32 index = GetBakuganTableIndex(this, unk1, unk2);
    return (u16) (sBakuganPriceEntries[index].mUnk_02 * 10);
}

u32 ListScene::func_02086570(u32 unk1, u32 unk2) {
    GetBakuganTableIndex(this, unk1, unk2);
    return 0;
}

void FillInitialEntry(ListScene *self, u32 unk1, u32 unk2, BattleCellPair *out) {
    u32 index       = GetBakuganTableIndex(self, unk1, unk2);
    out->mUnk_00_0  = 0;
    out->mGauge  = 0;
    out->mUnk_02_0  = sInitialEntryFields[index][0];
    out->mUnk_02_12 = sInitialEntryFields[index][1];
    out->mUnk_02_3  = sInitialEntryFields[index][2];
    out->mUnk_02_6  = sInitialEntryFields[index][3];
    out->mUnk_02_9  = sInitialEntryFields[index][4];
}

u32 GetBakuganTableIndex(ListScene *self, u32 unk1, u32 unk2) {
    if (unk1 == 0x22 && unk2 == 0) {
        return 0x28;
    }
    return unk1;
}

void SortListSceneItems(ListScene *self, u32 unk1) {
    u8 s;
    u8 t;
    for (s = 0; s < self->mItemCount - 1; s++) {
        for (t = s + 1; t < self->mItemCount; t++) {
            u16 a;
            u16 b;
            u32 vb;
            u32 va;
            if (unk1 != 0) {
                b  = self->mItemIds[t];
                a  = self->mItemIds[s];
                // Placeholder: the casts keep the member offset in the literal-pool word, like an access through a symbol of
                // its own (plain .mOrder[b] changes the code).
                vb = ((const u8 *) sAreaCursorAndBakuganOrder.mOrder)[b];
                va = ((const u8 *) sAreaCursorAndBakuganOrder.mOrder)[a];
                if (va > vb) {
                    self->mItemIds[s] = b;
                    self->mItemIds[t] = a;
                }
            } else {
                b  = self->mItemIds[t];
                a  = self->mItemIds[s];
                vb = Table_020a3ba8()[b];
                va = Table_020a3ba8()[a];
                if (va > vb) {
                    self->mItemIds[s] = b;
                    self->mItemIds[t] = a;
                }
            }
        }
    }
}

// Cards 1..0xd4 with GetCardNumber < 0x2bc, their data_020d4946 bit set and GetCardInfo(k, 1) == unk1, shuffled.
void FillPickListWithShuffledCards(ListScene *self, u32 unk1) {
    self->mPickCount = 0;
    for (u16 k = 1; k < 0xd5; k++) {
        if (GetCardNumber(k) < 0x2bc && TestFlagBit(data_020d4946, k) && GetCardInfo(k, 1) == unk1) {
            self->mPickList[self->mPickCount] = k;
            self->mPickCount++;
        }
    }
    ShufflePickList(self);
}

void ShufflePickList(ListScene *self) {
    u8 i;
    for (i = 0; i < self->mPickCount - 1; i++) {
        u16 value = self->mPickList[i];
        u16 j     = i + MATH_Rand32(&gRandom, self->mPickCount - i);
        self->mPickList[i] = self->mPickList[j];
        self->mPickList[j] = value;
    }
}

// Largest GetCardAttributeValue for gSaveData[0].mAttribute first.
void SortPickListByAttributeValue(ListScene *self) {
    u8 s;
    u8 t;
    for (s = 0; s < self->mPickCount - 1; s++) {
        for (t = s + 1; t < self->mPickCount; t++) {
            u32 va = GetCardAttributeValue(self->mPickList[s], (u8) gSaveData[0].mAttribute);
            u32 vb = GetCardAttributeValue(self->mPickList[t], (u8) gSaveData[0].mAttribute);
            if (va < vb) {
                u16 tmp           = self->mPickList[s];
                self->mPickList[s] = self->mPickList[t];
                self->mPickList[t] = tmp;
            }
        }
    }
}
