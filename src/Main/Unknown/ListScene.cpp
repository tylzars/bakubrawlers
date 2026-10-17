#include "Scene/ListScene.hpp"
#include "Unknown/UnkStruct_02071d24.hpp"
#include "Battle/BattleCommandList.hpp"
#include "Battle/BattleCommandPage.hpp"
#include "Game/UnkStruct_020d433c.hpp"
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
struct UnkStruct_020a3210 {
    /* 00 */ WindowItemRecord mCursor[2];
    /* 38 */ u8 mOrder[0x29];
};

struct UnkStruct_020a338c {
    /* 0 */ u16 mPriceTens;
    /* 2 */ u8 mUnk_02;
    /* 3 */ u8 mUnk_03;
};

// mwcc emits a file's objects sorted by size; the order of equal sizes (the window tables and their path strings, and
// the destructor-chain nodes in .bss) follows from the whole declaration sequence. This definition order, with the
// WirelessManager and TouchManager headers before the tables and the other singleton headers after them, gives the
// original layout of all three sections.
static const WindowItemRecord data_020a3430[7] = {
    {0, 0x8, 225, 170, 30, 20, 0, {-1, -1, -1, -1}, 0, 1},
    {1, 0x8, 3, 33, 18, 30, 0, {-1, -1, -1, -1}, 0, 3},
    {2, 0x8, 235, 33, 18, 30, 0, {-1, -1, -1, -1}, 0, 3},
    {3, 0x8, 3, 88, 18, 56, 0, {-1, -1, -1, -1}, 0, 1},
    {4, 0x8, 235, 88, 18, 56, 0, {-1, -1, -1, -1}, 0, 1},
    {5, 0x8, 3, 66, 18, 20, 0, {-1, -1, -1, -1}, 0, 3},
    {6, 0x8, 3, 146, 18, 20, 0, {-1, -1, -1, -1}, 0, 3},
};

static const WindowCellRecord data_020a34f4[7] = {
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
static const u8 data_020a3188[2][12] = {
    {0x00, 0xa0, 0x00, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},
    {0x00, 0x20, 0x00, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},
};

// Five-byte rows. A two-dimensional byte array stays in the shared .rodata section; a flat one would get a section
// of its own.
static const u8 data_020a35b8[41][5] = {
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

static const WindowCellRecord data_020a3274[5] = {
    {"/Game/2D/union_b/union_b_obj_window_nce.bin", 10, 0x7800, 1, 0, 0, {10, 10, 10, 0}},
    {"/Game/2D/union_b/union_b_obj_window_nce.bin", 3, 0x7800, 1, 0, 0, {3, 5, 4, 0}},
    {"/Game/2D/union_b/union_b_obj_window_nce.bin", 6, 0x7800, 1, 0, 0, {6, 8, 7, 0}},
    {"/Game/2D/union_b/union_b_obj_window_nce.bin", 0, 0x7800, 1, 0, 0, {0, 2, 1, 0}},
    {"/Game/2D/union_b/union_b_obj_window_nce.bin", 11, 0x7800, 1, 0, 0, {11, 11, 11, 0}},
};

static const UnkStruct_020a3210 data_020a3210 = {
    {
        {0, 0x8, 0, 33, 256, 30, 0, {-1, -1, -1, -1}, 0, 1},
        {1, 0x8, 0, 69, 256, 90, 0, {-1, -1, -1, -1}, 0, 1},
    },
    {
        0x17, 0x1d, 0x1a, 0x20, 0x18, 0x1e, 0x1e, 0x19, 0x1f, 0x1c, 0x22, 0x1b, 0x21, 0x11, 0x14, 0x13,
        0x12, 0x15, 0x16, 0x01, 0x02, 0x03, 0x00, 0x04, 0x08, 0x0e, 0x10, 0x09, 0x0b, 0x0c, 0x0f, 0x0a,
        0x06, 0x07, 0x05, 0x0d, 0x23, 0x25, 0x24, 0x26, 0x28,
    },
};

static const UnkStruct_020a338c data_020a338c[41] = {
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

static const WindowItemRecord data_020a3688[8] = {
    {0, 0x8, 25, 33, 30, 30, 0, {-1, -1, -1, -1}, 0, 1},
    {1, 0x8, 69, 33, 30, 30, 0, {-1, -1, -1, -1}, 0, 1},
    {2, 0x8, 113, 33, 30, 30, 0, {-1, -1, -1, -1}, 0, 1},
    {3, 0x8, 157, 33, 30, 30, 0, {-1, -1, -1, -1}, 0, 1},
    {4, 0x8, 201, 33, 30, 30, 0, {-1, -1, -1, -1}, 0, 1},
    {5, 0x8, 25, 69, 206, 30, 0, {-1, -1, -1, -1}, 0, 1},
    {6, 0x8, 25, 101, 206, 30, 0, {-1, -1, -1, -1}, 0, 1},
    {7, 0x8, 25, 133, 206, 30, 0, {-1, -1, -1, -1}, 0, 1},
};

static const WindowItemRecord data_020a3300[5] = {
    {0, 0x8000, 120, 18, 0, 0, 0, {-1, -1, -1, -1}, 0, 0},
    {1, 0x8000, 45, 152, 78, 20, 0, {1, 1, 2, 2}, 0, 2},
    {2, 0x8000, 133, 152, 78, 20, 0, {2, 2, 1, 1}, 0, 2},
    {3, 0x8000, 89, 152, 78, 20, 0, {3, 3, 3, 3}, 0, 2},
    {4, 0x8000, 128, 96, 0, 0, 0, {-1, -1, -1, -1}, 0, 0},
};

static const WindowItemRecord data_020a31a0[2] = {
    {0, 0x8, 48, 68, 160, 24, 0, {1, 1, 0, 0}, 0, 2},
    {1, 0x8, 48, 100, 160, 24, 0, {0, 0, 1, 1}, 0, 2},
};

static const u8 data_020a3768[0x29][6] = {
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

static const WindowCellRecord data_020a31d8[2] = {
    {"/Game/Deck/select_deck_b_obj_01_nce.bin", 0, 0x1c000, 6, 0, 1, {0, 2, 1, 0}},
    {"/Game/Deck/select_deck_b_obj_01_nce.bin", 3, 0x1c000, 6, 0, 1, {3, 5, 4, 0}},
};

inline const u16 *Table_020a3ba8() {
    static const u16 data_020a3ba8[214] = {
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
    return data_020a3ba8;
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

void func_02086580(ListScene *self, u32 unk1, u32 unk2, BattleCellPair *out);

static inline void PrintMessageF3(TextManager &tm, MessageFile *file, int index, void *buffer, s16 y, s16 x) {
    tm.Print(file->GetText(index), file->GetLength(index), buffer, 0x18, 0x10, &y, &x, NULL, 0, 2, 0x44, 0x10, 0, 0, 1, 1, 1);
}

static inline void PrintMessageF(TextManager &tm, MessageFile *file, int index, void *buffer, s16 y, s16 x) {
    tm.Print(file->GetText(index), file->GetLength(index), buffer, 0x18, 0x10, &y, &x, NULL, 0, 2, 0x44, 0x10, 0, 0, 0, 1, 1);
}

static inline void PrintMessageWidth(TextManager &tm, const u8 *data, u32 offset, u32 length, void *buffer, s16 x, s16 y, u16 *width) {
    tm.Print(data + offset, length, buffer, 0x18, 0x10, &y, &x, width, 0, 2, 0x44, 0, 0, 0, 0, 1, 1);
}

static inline u16 *MessageEntry(void *base, u32 index) {
    return (u16 *) ((u8 *) base + index * 4);
}

void PrintListSceneMainScreenText(ListScene *self);

static inline void PrintMessageB(TextManager &tm, MessageFile *file, int index, void *buffer, s16 y, s16 x) {
    tm.Print(file->GetText(index), file->GetLength(index), buffer, 0x18, 0x10, &y, &x, NULL, 0, 2, 0x44, 0, 0, 0, 1, 1, 1);
}

void PrintListSceneSubScreenText(ListScene *self);






u32 func_02086654(ListScene *self, u32 unk1, u32 unk2);

extern "C" void MIi_CpuCopy16(const void *src, void *dst, u32 size);

// Inline wrapper around MIi_CpuCopy16 (MIi_CpuCopy16).
static inline void MI_CpuCopy16(const void *src, void *dst, u32 size) {
    MIi_CpuCopy16(src, dst, size);
}

// Layout of the list message of a battle player (BattlePlayer::mMsg) and of the communication buffer data_0220f540.

struct BattleListMessage {
    /* 00 */ u8 mUnk_00;
    /* 01 */ PAD(0x01, 0x04);
    /* 04 */ u16 mUnk_04[6];
    /* 10 */ u8 mUnk_10[6];
    /* 16 */ u8 mUnk_16[6];
    /* 1c */ u8 mUnk_1c;
};

BOOL SetMessageTypeAndCheckAll(BattlePlayerList *list, u32 id);

#define MESSAGE ((BattleListMessage *) &data_0220f540)

// Layout of the slot message (type 4) of a battle player and of the communication buffer.

struct BattleSlotMessage {
    /* 00 */ u8 mUnk_00;
    /* 01 */ PAD(0x01, 0x04);
    /* 04 */ u16 mUnk_04[6];
    /* 10 */ u16 mUnk_10[6];
    /* 1c */ u8 mUnk_1c;
    /* 1d */ u8 mUnk_1d;
};

#define SLOT_MESSAGE ((BattleSlotMessage *) &data_0220f540)

u8 GetSlotBakuganId(SaveData *d, u32 a, u32 b);

void func_0208671c(ListScene *self, u32 unk1);

void func_020867b8(ListScene *self);

void func_020858d4(ListScene *self);

void func_02085990(ListScene *self);

void func_02085b8c(ListScene *self);

void func_02085fec(ListScene *self);

void func_02086874(ListScene *self);

BOOL func_02085a7c(ListScene *self, u32 unk1, u32 unk2);

BOOL func_02085da8(ListScene *self, u32 unk1, u32 unk2, u32 unk3);

BOOL func_02086044(ListScene *self, u32 unk1);

BOOL ListSceneHasAttribute(ListScene *self, u32 unk1);








u8 GetCardAttributeValue(u32 id, u32 index);

void ListSceneScrollCallback(void *arg);
void PrintListSceneMainScreenText(ListScene *self);
void func_020834ec(ListScene *self, u32 mode);
void PrintListSceneSubScreenText(ListScene *self);
void func_0208415c(ListScene *self);
void func_020843ac(ListScene *self, u32 unk1);
void func_020848c8(ListScene *self);
BOOL func_02084a10(ListScene *self);
BOOL func_02084c38(ListScene *self);
BOOL func_02084e64(ListScene *self); // self is passed but unused
BOOL func_02084f40(ListScene *self); // self is passed but unused
BOOL func_0208501c(ListScene *self);
void func_02085220(ListScene *self, u32 mode);
void func_0208550c(ListScene *self);
void func_020858d4(ListScene *self);
BOOL func_0208596c(ListScene *self, u32 unk1, u32 unk2);
void func_02085990(ListScene *self);
BOOL func_02085a7c(ListScene *self, u32 unk1, u32 unk2);
void func_02085b8c(ListScene *self);
BOOL func_02085da8(ListScene *self, u32 unk1, u32 unk2, u32 unk3);
void func_02085fec(ListScene *self);
BOOL func_02086044(ListScene *self, u32 unk1);
BOOL ListSceneHasAttribute(ListScene *self, u32 unk1);
void func_02086580(ListScene *self, u32 unk1, u32 unk2, BattleCellPair *out);
u32 func_02086654(ListScene *self, u32 unk1, u32 unk2);
void SortListSceneItems(ListScene *self, u32 unk1);
void func_0208671c(ListScene *self, u32 unk1);
void func_020867b8(ListScene *self);
void func_02086874(ListScene *self);


ListScene::ListScene(u32 unk1, u32 unk2) :
    SceneBase(0x3e8, 0) {
    mStorecommentMessages   = NULL;
    mBakuganNameShortMessages   = NULL;
    mBakuganNameMessages   = NULL;
    mCardNameShortMessages   = NULL;
    mDeckSelectMessages   = NULL;
    mResultMessages   = NULL;
    mUnk_48   = NULL;
    mUnk_4c   = NULL;
    mDeckWindow   = NULL;
    mUnk_54   = NULL;
    mUnk_58   = NULL;
    mCommandList   = NULL;
    mCommandPage   = NULL;
    mMode   = unk1;
    mState   = 0;
    mUnk_e2   = 0;
    mUnk_1e0  = 0;
    mUnk_1e4  = 0;
    mAntenna  = NULL;
    mMessageWindow  = NULL;
    mPlayer  = unk2;
    mUnk_3f4  = 0;
    mAppliedWndMode  = 0;
    mWndMode  = 0;
    mUnk_3fa  = 0;
    mUnk_3fc  = 0;
    mUnk_3fe  = 0;
    mUnk_3ff  = 0;
    mUnk_400  = 0;
    mUnk_401  = 0;
    mPendingPlanes  = -1;
    SpriteManager::Get().SetFade(1, -16);
    mCharacterId = mMode == 5 ? data_020d43db[mPlayer] : 0x15;
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
    mUnk_f4 = 0;
    if (mCharacterId != 0x15) {
        mUnk_f4 = 1;
    }
    mUnk_e4 = 0;
    mUnk_e6 = 0;
    mUnk_e7 = 0;
    mUnk_e8 = mUnk_e6;
    mUnk_e9 = 0;
    mUnk_ea = 0;
    mUnk_ec = 0;
    mUnk_f0 = 1;
    mUnk_f5 = 0;
    mWndMode = 2;
    mUnk_f8  = 0;
    mCursorItem  = 0;
    mUnk_fb  = 0;
    mItemCount  = 0;
    mScroll  = 0;
    mUnk_100 = 0;
    mUnk_104 = 0;
    for (u8 i = 0; i < 0x40; i++) {
        mItemIds[i] = 0;
    }
    for (u8 i = 0; i < 13; i++) {
        mUnk_186[i] = 0;
    }
    for (u8 i = 0; i < 3; i++) {
        mUnk_194[i] = 1;
    }
    mUnk_1a0 = 4;
    mUnk_1a1 = 0;
    mPrice = 0;
    MI_CpuClear16(&mAlphabetChar, 2);
    mUnk_3f4 = data_020d433c.mUnk_8a_0;
}

ListScene::~ListScene() {
    u8 i;
    for (i = 0; i < 6; i++) {
        delete mMainScreenCellAnims[i];
    }
    for (i = 0; i < 0x13; i++) {
        delete mSubScreenCellAnims[i];
    }
    delete mUnk_4c;
    delete mUnk_48;
    delete mDeckWindow;
    operator delete(mUnk_54);
    operator delete(mUnk_58);
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
    SpriteManager::Get().SetFade(1, -16);
    if (data_020d433c.mWireless != 0) {
        mAntenna->mUnk_04 = 1;
    }
    delete mMessageWindow;
    mWndMode = 0;
    ApplyWndMode();
}

// Frees what LoadSubScreenGraphics, LoadMessages and InitMainScreenText built; also finishes the antenna task
// (mAntenna, :736-738) and turns the sub-screen windows off. With releaseCommands it also frees mCommandList /
// mCommandPage and writes mContext->mUnk_12 (:741-754).
void ListScene::Release(u32 releaseCommands) {
    for (u8 i = 0; i < 6; i++) {
        delete mMainScreenCellAnims[i];
        mMainScreenCellAnims[i] = NULL;
    }
    for (u8 i = 0; i < 0x13; i++) {
        delete mSubScreenCellAnims[i];
        mSubScreenCellAnims[i] = NULL;
    }
    delete mUnk_4c;
    mUnk_4c = NULL;
    delete mUnk_48;
    mUnk_48 = NULL;
    delete mDeckWindow;
    mDeckWindow = NULL;
    operator delete(mUnk_54);
    mUnk_54 = NULL;
    operator delete(mUnk_58);
    mUnk_58 = NULL;
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
    if (data_020d433c.mWireless != 0) {
        mAntenna->mUnk_04 = 1;
    }
    delete mMessageWindow;
    mMessageWindow = NULL;
    if (releaseCommands != 0) {
        if (mUnk_400 == 0 && mUnk_401 == 0) {
            mContext->mRequest = 1;
        }
        if (mCommandList != NULL && mUnk_400 == 0) {
            CloseCommandList(mCommandList);
            mCommandList           = NULL;
            mContext->mRequest = 0;
        }
        if (mCommandPage != NULL && mUnk_401 == 0) {
            CloseCommandPage(mCommandPage, 1);
            mCommandPage           = NULL;
            mContext->mRequest = 0;
        }
    }
    mWndMode = 0;
    ApplyWndMode();
}

#define UPDATE_TAIL()                      \
    func_0207eec4();                       \
    mSubScreenCellAnims[18]->Update();                 \
    mSubScreenCellAnims[18]->Draw(1, 0x80, 0x60, 0);   \
    return

void ListScene::Update() {
    if (mPendingPlanes != -1) {
        GXS_SetVisiblePlane(mPendingPlanes);
        mPendingPlanes = -1;
        if (mState == 3) {
            if (mUnk_f0 != 0) {
                mWndMode = 2;
            } else {
                mWndMode = 1;
            }
        }
    }
    if (mUnk_3fa != 0) {
        mUnk_3fa--;
        if (mUnk_3fa == 0 && mUnk_4c != NULL) {
            mUnk_4c->mCellAnims[0]->SetAnimation(2);
        }
    }
    if (mUnk_3ff != 0 && mUnk_3fc == 0) {
        SoundManager::Get().StartSeqArc(0x66);
        mUnk_3ff = 0;
        u32 one = 1;
        if (mUnk_e6 < 4) {
            mUnk_c8[mUnk_e6 - 1] = mItemIds[mCursorItem];
            mUnk_c8[mUnk_e6 + 8] = mUnk_104;
            mUnk_f0              = one;
            mWndMode             = 2;
            mUnk_186[mUnk_e6]    = mCursorItem;
            mUnk_194[0]          = one;
            mUnk_194[1]          = one;
            mUnk_194[2]          = one;
            func_02081128();
            data_020d433c.mUnk_14 |= 2;
        } else {
            mUnk_c8[mUnk_e6 - 1] = mItemIds[mCursorItem];
            mUnk_f0              = one;
            mWndMode             = 2;
            mUnk_186[mUnk_e6]    = mCursorItem;
            mUnk_194[0]          = one;
            mUnk_194[1]          = one;
            mUnk_194[2]          = one;
            func_02081594();
            data_020d433c.mUnk_14 |= 2;
        }
    }
    if (mUnk_3fc > 0) {
        mUnk_3fc -= mUnk_3fe;
        if (mUnk_3fc <= 0) {
            mUnk_3fc = 0;
        }
    }
    switch (mState) {
        case 0:
            InitDisplay();
            func_0207b6e8();
            LoadSubScreenGraphics();
            LoadMessages();
            mState++;
            if (mMode == 5) {
                mState = 7;
                func_020851d0();
                return;
            }
            break;
        case 1:
            mUnk_48->mEnabled = 0;
            mUnk_4c->mEnabled = 0;
            if (!IsCommandListOrPageSettingUp()) {
                SpriteManager::Get().StartFade(2);
                mState++;
            }
            break;
        case 2:
            if (!SpriteManager::Get().IsFading()) {
                mUnk_4c->mEnabled = 1;
                mState++;
            }
            break;
        case 3:
            func_0207cfa4();
            break;
        case 4:
            if (!IsCommandListOrPageSettingUp()) {
                switch (mUnk_48->Update(1)) {
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
            mUnk_48->Update(1);
            if (mUnk_48->mState == 4) {
                switch (mUnk_1a1) {
                    case 2:
                        switch (mMode) {
                            case 0:
                                gSaveData[0].mMoney -= mPrice;
                                func_020834ec(this, 4);
                                break;
                            case 1:
                                gSaveData[0].mMoney -= mPrice;
                                func_020834ec(this, 0);
                                mUnk_186[mUnk_e6] = mCursorItem;
                                mState           = 0x21;
                                break;
                            case 2:
                                gSaveData[0].mMoney -= mPrice;
                                func_020834ec(this, 6);
                                break;
                        }
                        SoundManager::Get().StartSeqArc(0x69);
                        break;
                    case 3:
                        func_020834ec(this, 2);
                        func_02081128();
                        break;
                    case 4:
                    case 5:
                        func_0208415c(this);
                        func_020834ec(this, 10);
                        break;
                    case 10:
                        func_020834ec(this, 0);
                        mState = 0x26;
                        break;
                    case 6:
                        SetFlagBit(data_020d4946, mItemIds[mCursorItem]);
                        func_020834ec(this, 0);
                        break;
                    case 8:
                        ReadSlots();
                        func_020834ec(this, 0);
                        SoundManager::Get().StartSeqArc(0x68);
                        break;
                    case 9:
                        func_0208550c(this);
                        data_020d433c.mUnk_14 |= 2;
                        func_020834ec(this, 0);
                        SoundManager::Get().StartSeqArc(0x67);
                        break;
                    default:
                        func_020834ec(this, 0);
                        break;
                }
            }
            break;
        case 6:
            mUnk_48->Update(1);
            if (mUnk_48->mState == 4) {
                func_020834ec(this, 0);
            }
            break;
        case 7:
            mUnk_48->mEnabled = 0;
            mUnk_4c->mEnabled = 0;
            mDeckWindow->mEnabled = 0;
            SpriteManager::Get().StartFade(2);
            mState++;
            func_020851d0();
            return;
        case 8:
            if (!SpriteManager::Get().IsFading()) {
                mDeckWindow->mEnabled = 1;
                mState++;
                if (mUnk_3f4 != 0) {
                    func_02085220(this, 0xc);
                }
            }
            func_020851d0();
            return;
        case 9:
            if (!IsCommandListOrPageSettingUp()) {
                if (mDeckWindow->Update(1) == 3) {
                    switch (mDeckWindow->mCursor->mSelection) {
                        case 0:
                            StoreSlotsInSaveData();
                            func_02085220(this, 0xb);
                            break;
                        case 1:
                            mUnk_e6 = mUnk_f4;
                            mUnk_e7 = mUnk_f4;
                            mUnk_e8 = mUnk_f4;
                            mUnk_ea = mUnk_f4;
                            if (mCharacterId != 0x15) {
                                mUnk_1a0 = 1;
                            }
                            mState++;
                            break;
                    }
                }
            }
            func_020851d0();
            return;
        case 10:
            SpriteManager::Get().StartFade(3);
            mDeckWindow->mEnabled = 0;
            mState++;
            func_020851d0();
            return;
        case 11:
            if (!SpriteManager::Get().IsFading()) {
                mState++;
            }
            func_020851d0();
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
                SpriteManager::Get().StartFade(2);
                mState++;
            }
            break;
        case 14:
            if (!SpriteManager::Get().IsFading()) {
                mUnk_4c->mEnabled = 1;
                mState           = 3;
            }
            break;
        case 15:
            SpriteManager::Get().StartFade(3);
            mUnk_4c->mEnabled = 0;
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
            func_020851d0();
            return;
        case 18:
            if (!IsCommandListOrPageSettingUp() && mUnk_1a0 == 0) {
                SpriteManager::Get().StartFade(2);
                mState++;
            }
            func_020851d0();
            return;
        case 19:
            if (!SpriteManager::Get().IsFading()) {
                mDeckWindow->mEnabled = 1;
                mState           = 9;
            }
            func_020851d0();
            return;
        case 20: {
            switch (mUnk_48->Update(1)) {
                case 1:
                    mState = 0x15;
                    break;
                case 2:
                    mState = 0x16;
                    break;
            }
            func_020851d0();
            return;
        }
        case 21:
            mUnk_48->Update(1);
            if (mUnk_48->mState == 4) {
                switch (mUnk_1a1) {
                    case 0xb:
                        SoundManager::Get().StartSeqArc(0x6a);
                        func_02085220(this, 0);
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
                        func_02085220(this, 0);
                        break;
                }
            }
            func_020851d0();
            return;
        case 22:
            mUnk_48->Update(1);
            if (mUnk_48->mState == 4) {
                func_02085220(this, 0);
            }
            func_020851d0();
            return;
        case 23:
            mDeckWindow->mEnabled = 0;
            if (mCharacterId == 0x15) {
                func_020848c8(this);
            }
            MIi_CpuCopyFast(gCharacterLook, (u8 *) ((u32 *) data_020d433c.mBattlePlayers)[3] + 0xa8, 8);
            SpriteManager::Get().StartFade(3);
            mState++;
            func_020851d0();
            return;
        case 24:
            if (!SpriteManager::Get().IsFading()) {
                Release(1);
                mState++;
                return;
            }
            func_020851d0();
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
            if (func_02084a10(this)) {
                mState++;
            }
            UPDATE_TAIL();
        case 28:
            if (func_02084c38(this)) {
                mState++;
            }
            UPDATE_TAIL();
        case 29:
            if (func_0208501c(this)) {
                mState++;
            }
            UPDATE_TAIL();
        case 30:
            if (func_02084f40(this)) {
                mState++;
            }
            UPDATE_TAIL();
        case 31:
            if (func_02084e64(this)) {
                SpriteManager::Get().StartFade(3);
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
            SpriteManager::Get().StartSubFade(3);
            mState++;
            break;
        case 34:
            SpriteManager::Get();
            if (SpriteManager::sFade[1] <= 1) {
                if (mCommandList != NULL) {
                    mCommandList->SetWindowVisible(0);
                }
                Release(0);
                mUnk_1e0 = new UnkStruct_02076058(1);
                mUnk_1e0->SetBakugan(1, mItemIds[mCursorItem] & 0xff, mUnk_104, 1);
                mContext->mRequest = 0;
                mState++;
                return;
            }
            break;
        case 35:
            if (func_02078270(mUnk_1e0)) {
                UnkStruct_02023248 *pair = GetBakuganEntry(&gSaveData[0], mItemIds[mCursorItem] & 0xff, mUnk_104);
                u16 val = func_020231f4(&gSaveData[0], 0, mItemIds[mCursorItem] & 0xff, mUnk_104);
                func_0205afb4(mCommandList, mUnk_104, val, (BattleCellPair *) pair);
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
            mUnk_f0           = 0;
            mUnk_1a0          = 4;
            mUnk_194[0]       = 1;
            mUnk_194[1]       = 1;
            mUnk_194[2]       = 1;
            mWndMode          = 1;
            return;
        case 38:
            SpriteManager::Get().StartFade(3);
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
            mUnk_1e4->SetBakugan(0, mItemIds[mCursorItem] & 0xff, mUnk_104, 1);
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
            mUnk_f0           = 1;
            mUnk_1a0          = 4;
            mUnk_194[0]       = 1;
            mUnk_194[1]       = 1;
            mUnk_194[2]       = 1;
            mWndMode          = 2;
            return;
        case 43:
            if (mMode == 3 || mMode == 5) {
                StoreSlotsInSaveData();
            }
            SpriteManager::Get().StartFade(3);
            mUnk_48->mEnabled = 0;
            mUnk_4c->mEnabled = 0;
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
    func_0207dea4();
    func_0207e074();
    func_0207e424();
    func_0207ea3c();
    func_0207eec4();
    func_0208403c();
    DrawSubScreenText();
    func_02080104();
    DrawWindows();
    func_0207f32c();
    func_0207f90c();
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
    if (mMode == 4) {
        GXS_SetVisiblePlane(0x19);
    } else if (mMode == 5) {
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

void ListScene::func_0207b6e8() {
    switch (mMode) {
        case 0:
        case 1:
            mUnk_e9 = 6;
            break;
        case 2:
            mUnk_e9 = 6;
            break;
        case 3:
        case 5:
            mUnk_e9 = 10;
            break;
        case 4:
            mUnk_e9 = 13;
            RefreshCounts();
            break;
    }
    ReadSlots();
    BuildList(1);
}


void ListScene::LoadSubScreenGraphics() {
    ChangeDir("/Game/2d/union_b/");
    LoadAndQueueUpload(1, 1, 0x2000, "union_b_bg_window_ncl.bin", 0, 0, 0);
    LoadAndQueueUpload(1, 0xc, 0, "union_b_bg_window_ncg.bin", 0, 0, 0);
    LoadAndQueueUpload(1, 8, 0, "union_b_bg_window_b_nsc.bin", 0, 0, 0);
    switch (mMode) {
        case 0:
        case 1:
        case 2:
            ChangeDir("/Game/List/");
            LoadAndQueueUpload(1, 1, 0, "list_store_b_bg_00_ncl.bin", 0, 0, 0);
            LoadAndQueueUpload(1, 1, 0x4000, "list_store_b_bg_00_ncl.bin", 0, 0, 0);
            LoadAndQueueUpload(1, 1, 0x6000, "list_store_b_bg_00_ncl.bin", 0, 0, 0);
            LoadAndQueueUpload(1, 0xb, 0, "list_store_b_bg_00_ncg.bin", 0, 0, 0);
            LoadAndQueueUpload(1, 7, 0, "list_store_b_bg_00_nsc.bin", 0, 0, 0);
            LoadAndQueueUpload(1, 9, 0, "list_store_b_bg_01_nsc.bin", 0, 0, 0);
            LoadAndQueueUpload(1, 10, 0, "list_store_b_bg_02_nsc.bin", 0, 0, 0);
            break;
        case 3:
            ChangeDir("/Game/List/");
            LoadAndQueueUpload(1, 1, 0, "list_edit_b_bg_00_ncl.bin", 0, 0, 0);
            LoadAndQueueUpload(1, 1, 0x4000, "list_edit_b_bg_00_ncl.bin", 0, 0, 0);
            LoadAndQueueUpload(1, 1, 0x6000, "list_edit_b_bg_00_ncl.bin", 0, 0, 0);
            LoadAndQueueUpload(1, 0xb, 0, "list_edit_b_bg_00_ncg.bin", 0, 0, 0);
            LoadAndQueueUpload(1, 7, 0, "list_edit_b_bg_00_nsc.bin", 0, 0, 0);
            LoadAndQueueUpload(1, 9, 0, "list_edit_b_bg_01_nsc.bin", 0, 0, 0);
            LoadAndQueueUpload(1, 10, 0, "list_edit_b_bg_02_nsc.bin", 0, 0, 0);
            break;
        case 4:
            ChangeDir("/Game/List/");
            LoadAndQueueUpload(1, 1, 0, "list_gallery_b_bg_00_ncl.bin", 0, 0, 0);
            LoadAndQueueUpload(1, 1, 0x4000, "list_gallery_b_bg_00_ncl.bin", 0, 0, 0);
            LoadAndQueueUpload(1, 1, 0x6000, "list_gallery_b_bg_00_ncl.bin", 0, 0, 0);
            LoadAndQueueUpload(1, 0xb, 0, "list_gallery_b_bg_00_ncg.bin", 0, 0, 0);
            LoadAndQueueUpload(1, 7, 0, "list_gallery_b_bg_00_nsc.bin", 0, 0, 0);
            LoadAndQueueUpload(1, 9, 0, "list_gallery_b_bg_01_nsc.bin", 0, 0, 0);
            LoadAndQueueUpload(1, 10, 0, "list_gallery_b_bg_02_nsc.bin", 0, 0, 0);
            break;
        case 5:
            ChangeDir("/Game/Battle/Bg/");
            LoadAndQueueUpload(1, 1, 0, "select_arena_b_bg_00_ncl.bin", 0, 0, 0);
            LoadAndQueueUpload(1, 0xb, 0, "select_arena_b_bg_00_ncg.bin", 0, 0, 0);
            LoadAndQueueUpload(1, 7, 0, "select_arena_b_bg_00_nsc.bin", 0, 0, 0);
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
    mUnk_4c = new SelectWindow(7, data_020a3430, -1);
    mUnk_4c->SetCells(7, data_020a34f4);
    mUnk_48 = new SelectWindow(5, data_020a3300, 1);
    mUnk_48->SetCells(5, data_020a3274);
    mUnk_54 = new SelectWindowCursor(8, data_020a3688, -1);
    mUnk_58 = new SelectWindowCursor(2, data_020a3210.mCursor, -1);
    if (mMode == 5) {
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
        mDeckWindow = new SelectWindow(2, data_020a31a0, 0);
        mDeckWindow->SetCells(2, data_020a31d8);
        mDeckWindow->mCellAnims[0]->SetAnimation(1);
        this->SetItemFlags(3, 0, 8);
        this->SetItemFlags(3, 1, 8);
        this->SetItemFlags(0, 0, 0x8000);
        this->SetItemFlags(0, 1, 0x8000);
        this->SetItemFlags(0, 2, 0x8000);
        this->SetItemFlags(0, 3, 0x8000);
        this->SetItemFlags(0, 4, 0x8000);
        this->SetItemFlags(0, 5, 0x8000);
        this->SetItemFlags(0, 6, 0x8000);
    }
    ChangeDir("/Game/Bakugan/mark");
    LoadAndQueueUpload(1, 0, 0x200, "bakugan_s_000_ncl.bin", 0, 0, 0);
    LoadAndQueueUpload(1, 0, 0, "bakugan_s_base_ncl.bin", 0, 0, 0);
    LoadAndQueueUpload(1, 4, 0x12000, "bakugan_s_base.nce_ncg1D.bin", 0, 0, 0);
    mSubScreenCellAnims[14]            = new CellAnimation("bakugan_s_base_nce.bin", 0, 1, 1);
    mSubScreenCellAnims[14]->mCharBase = screen.GetCharBase(0x12000);
    mSubScreenCellAnims[14]->mPaletteBase   = 0;
    mSubScreenCellAnims[14]->SetAnimation(0);
    ChangeDir("/Game/Card/Graphic/Small");
    LoadAndQueueUpload(1, 0, 0x400, "card_s_000_ncl.bin", 0, 0, 0);
    if (data_020d433c.mWireless == 0) {
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
        case 0:
            mMessageWindow = new MessageWindow("mes_storecomment", 1, 0x1b000);
            mMessageWindow->StartMessage(0xb, 0, 1);
            break;
        case 1:
            mMessageWindow = new MessageWindow("mes_storecomment", 1, 0x1b000);
            mMessageWindow->StartMessage(0xd, 0, 1);
            break;
        case 2:
            mMessageWindow = new MessageWindow("mes_storecomment", 1, 0x1b000);
            mMessageWindow->StartMessage(0xc, 0, 1);
            break;
        case 3:
            mMessageWindow = new MessageWindow("mes_DeckSelect", 1, 0x1b000);
            mMessageWindow->StartMessage(6, 0, 1);
            break;
        case 5:
            mMessageWindow = new MessageWindow("mes_DeckSelect", 1, 0x1b000);
            mMessageWindow->StartMessage(5, 0, 1);
            break;
        case 4:
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
        case 0:
            for (; i < 4; i++) {
                s32 index = mScroll + i;
                if (index >= mItemCount) {
                    break;
                }
                u32 a = func_02086548((u8) mItemIds[index], mUnk_104);
                u32 b = func_02086570((u8) mItemIds[index], mUnk_104);
                PrintBakuganRow((u8) mItemIds[mScroll + i], i, a, b);
            }
            break;
        case 1:
            for (; i < 4; i++) {
                if (mScroll + i >= mItemCount) {
                    break;
                }
                UnkStruct_02023248 *entry = GetBakuganEntry(gSaveData, (u8) mItemIds[mScroll + i], mUnk_104);
                u32 v = func_020231f4(gSaveData, 0, (u8) mItemIds[mScroll + i], mUnk_104);
                PrintBakuganRow((u8) mItemIds[mScroll + i], i, v, (u8) entry->mUnk_00_0);
            }
            break;
        case 2:
            for (; i < 4; i++) {
                if (mScroll + i >= mItemCount) {
                    break;
                }
                PrintCardNameShort(mItemIds[mScroll + i], i);
            }
            break;
        case 3:
        case 5:
            if (mUnk_e6 == 0) {
                break;
            }
            if (mUnk_e6 < 4) {
                for (; i < 4; i++) {
                    if (mScroll + i >= mItemCount) {
                        break;
                    }
                    UnkStruct_02023248 *entry = GetBakuganEntry(gSaveData, (u8) mItemIds[mScroll + i], mUnk_104);
                    u32 v = func_020231f4(gSaveData, 0, (u8) mItemIds[mScroll + i], mUnk_104);
                    PrintBakuganRow((u8) mItemIds[mScroll + i], i, v, (u8) entry->mUnk_00_0);
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
        case 4:
            if (mUnk_e6 == 0) {
                break;
            }
            if (mUnk_e6 < 7) {
                for (; i < 4; i++) {
                    if (mScroll + i >= mItemCount) {
                        break;
                    }
                    UnkStruct_02023248 *entry = GetBakuganEntry(gSaveData, (u8) mItemIds[mScroll + i], mUnk_104);
                    u32 v = func_020231f4(gSaveData, 0, (u8) mItemIds[mScroll + i], mUnk_104);
                    PrintBakuganRow((u8) mItemIds[mScroll + i], i, v, (u8) entry->mUnk_00_0);
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
    QueueUpload(SpriteManager::Get().Screen(1).mBgIndex, 4, 0, mTextBuffers[0], mTextBufferSizes[0], 0);
}

void ListScene::DrawSubScreenText() {
    if (mUnk_1a1 != 0) {
        return;
    }
    if (mMessageWindow != NULL) {
        mMessageWindow->Update();
    }
    SpriteManager::Get().Screen(1).AddOam(mGlyphBuffers[0], (u8) mGlyphCounts[0], 0x41, (s16) (mUnk_f8 + 0x48), 2, 0, 0xf, 0);
}

void ListScene::ClearSubScreenText() {
    if (mTextBuffers[0] != NULL) {
        MI_CpuClear16(mTextBuffers[0], mTextBufferSizes[0]);
    }
}

void ListScene::func_0207cfa4() {
    u32 act = 0;
    if (mUnk_ec != 0 || mUnk_e7 != mUnk_e8 || mUnk_100 != 0 || mUnk_fb != mCursorItem || mUnk_3ff != 0) {
        return;
    }
    if (mUnk_e2 == 3) {
        switch (mUnk_4c->mCursor->mSelection) {
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
    switch (mUnk_4c->mCursor->mUnk_05) {
        case 1:
            if (act == 0) {
                if (mUnk_f5 == 0) {
                    mUnk_f5 = 5;
                    act     = 3;
                } else if (TouchManager::Get().mTrigger != 0) {
                    mUnk_f5 = 6;
                    act     = 3;
                }
            }
            break;
        case 2:
            if (act == 0) {
                if (mUnk_f5 == 0) {
                    mUnk_f5 = 7;
                    act     = 4;
                } else if (TouchManager::Get().mTrigger != 0) {
                    mUnk_f5 = 8;
                    act     = 4;
                }
            }
            break;
        case 5:
            if (act == 0) {
                if (mUnk_f5 == 0) {
                    mUnk_f5 = 9;
                    act     = 5;
                } else if (TouchManager::Get().mTrigger != 0) {
                    act = 5;
                }
            }
            break;
        case 6:
            if (act == 0) {
                if (mUnk_f5 == 0) {
                    mUnk_f5 = 9;
                    act     = 6;
                } else if (TouchManager::Get().mTrigger != 0) {
                    act = 6;
                }
            }
            break;
    }
    if (mUnk_4c->mState == 1) {
        if (InputManager::Get().IsHeld(1)) {
            if (act == 0) {
                act = 1;
            }
        } else if (InputManager::Get().IsHeld(2)) {
            if (act == 0) {
                act = 2;
                mUnk_4c->mCellAnims[0]->SetAnimation(3);
                mUnk_3fa = 6;
            }
        } else if (InputManager::Get().IsTrigger(KEY_LEFT)) {
            if (act == 0) {
                if (mUnk_f0 != 0) {
                    act = 3;
                    if (InputManager::Get().IsHeld(KEY_LEFT)) {
                        mUnk_f5 = 1;
                    } else {
                        mUnk_f5 = 2;
                    }
                } else if (mUnk_4c->mButtons[3].mEnabled != 0) {
                    mUnk_4c->SetResultItem(3, 3);
                }
            }
        } else if (InputManager::Get().IsTrigger(KEY_RIGHT)) {
            if (act == 0) {
                if (mUnk_f0 != 0) {
                    act = 4;
                    if (InputManager::Get().IsHeld(KEY_RIGHT)) {
                        mUnk_f5 = 3;
                    } else {
                        mUnk_f5 = 4;
                    }
                } else if (mUnk_4c->mButtons[4].mEnabled != 0) {
                    mUnk_4c->SetResultItem(4, 3);
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
        if (mUnk_58->Update(0) == 3) {
            switch (mUnk_58->mSelection) {
                case 0:
                    if (act == 0 && mUnk_f0 == 0) {
                        act = 7;
                    }
                    break;
                case 1:
                    if (act == 0) {
                        if (mUnk_f0 != 0) {
                            act = 8;
                        }
                    }
                    break;
            }
        }
        s8 prev = mUnk_54->mSelection;
        if (mUnk_54->Update(0) == 3) {
            s8 cur = mUnk_54->mSelection;
            if (prev == cur) {
                if (act == 0) {
                    act = 1;
                }
            } else if (act == 0) {
                if (mUnk_f0 != 0) {
                    mUnk_e7                = mUnk_ea + cur;
                    mUnk_54->mSelection = 0;
                } else {
                    mUnk_fb = mScroll + cur - 5;
                }
                SoundManager::Get().StartSeqArc(5);
            }
        }
    }
    if (mUnk_f0 != 0) {
        switch (act) {
            case 1:
            case 8:
                if (mItemCount == 0) {
                    return;
                }
                mUnk_f0     = 0;
                mWndMode    = 1;
                mUnk_194[0] = 1;
                func_02081128();
                func_02081594();
                if ((mMode == 3 || mMode == 5) && mUnk_e6 == 0) {
                    SetMessage(-1);
                }
                SoundManager::Get().StartSeqArc(0);
                return;
            case 2:
                if (mMode == 5) {
                    mState = 0xf;
                } else {
                    mState = 0x2b;
                }
                SoundManager::Get().StartSeqArc(1);
                return;
            case 3:
                if (mUnk_e8 > mUnk_f4) {
                    mUnk_e7 = mUnk_e8 - 1;
                    SoundManager::Get().StartSeqArc(5);
                } else {
                    mUnk_f5 = 0;
                }
                return;
            case 4:
                if (mUnk_e8 < mUnk_e9 - 1) {
                    mUnk_e7 = mUnk_e8 + 1;
                    SoundManager::Get().StartSeqArc(5);
                } else {
                    mUnk_f5 = 0;
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
                    case 0:
                        mPrice = GetPrice((u8) mItemIds[mCursorItem], mUnk_104);
                        if (gSaveData[0].mMoney < mPrice) {
                            func_020834ec(this, 1);
                        } else if (func_020231f4(gSaveData, 0, (u8) mItemIds[mCursorItem], mUnk_104) != 0) {
                            func_020834ec(this, 3);
                        } else {
                            func_020834ec(this, 2);
                        }
                        SoundManager::Get().StartSeqArc(0);
                        return;
                    case 1: {
                        UnkStruct_02023248 *entry = GetBakuganEntry(gSaveData, (u8) mItemIds[mCursorItem], mUnk_104);
                        mPrice                  = GetEntryPrice((u8) mItemIds[mCursorItem], mUnk_104);
                        if ((s32) entry->mUnk_00_0 >= 9) {
                            func_020834ec(this, 7);
                        } else if (gSaveData[0].mMoney < mPrice) {
                            func_020834ec(this, 1);
                        } else {
                            func_020834ec(this, 2);
                        }
                        SoundManager::Get().StartSeqArc(0);
                        return;
                    }
                    case 2:
                        mPrice = Table_020a39fc()[mItemIds[mCursorItem]];
                        if (gSaveData[0].mMoney < mPrice) {
                            func_020834ec(this, 1);
                        } else {
                            func_020834ec(this, 2);
                        }
                        SoundManager::Get().StartSeqArc(0);
                        return;
                    case 3:
                    case 5:
                        if (mUnk_e6 == 0) {
                            if (mCursorItem == 0) {
                                func_020834ec(this, 8);
                            } else if (mCursorItem == 1) {
                                func_020834ec(this, 9);
                            }
                        } else if (mUnk_e6 < 4) {
                            if (IsInSlots(1, (u8) mItemIds[mCursorItem], mUnk_104) == 0) {
                                mUnk_3fc = (mCursorItem - mScroll) * 0x20 + 0x24;
                                mUnk_3fe = mUnk_3fc / 8;
                                mUnk_3ff = 1;
                            }
                        } else {
                            if (IsInSlots(0, (u8) mItemIds[mCursorItem], 0) == 0) {
                                mUnk_3fc = (mCursorItem - mScroll) * 0x20 + 0x24;
                                mUnk_3fe = mUnk_3fc / 8;
                                mUnk_3ff = 1;
                            }
                        }
                        SoundManager::Get().StartSeqArc(0);
                        return;
                }
                break;
            case 2:
            case 7:
                mUnk_f0                = 1;
                mWndMode               = 2;
                mUnk_186[mUnk_e6]      = mCursorItem;
                mUnk_194[0]            = 1;
                func_02081128();
                func_02081594();
                if ((mMode == 3 || mMode == 5) && mUnk_e6 == 0) {
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
                if ((mMode == 3 || mMode == 5) && mUnk_e6 != 0 && mUnk_e6 < 4) {
                    if (act == 3) {
                        mUnk_104 = (mUnk_104 + 5) % 6;
                    } else {
                        mUnk_104 = (mUnk_104 + 1) % 6;
                    }
                    BuildList(1);
                    mCursorItem     = 0;
                    mUnk_fb     = 0;
                    mScroll     = 0;
                    mUnk_194[2] = 1;
                    mUnk_194[0] = 1;
                    func_02081128();
                    SoundManager::Get().StartSeqArc(0);
                }
                mUnk_f5 = 0;
                return;
            case 5:
                if (mItemCount <= 1) {
                    return;
                }
                if (mCursorItem != 0) {
                    mUnk_fb = mCursorItem - 1;
                } else {
                    mUnk_fb = mItemCount - 1;
                }
                SoundManager::Get().StartSeqArc(5);
                return;
            case 6:
                if (mItemCount <= 1) {
                    return;
                }
                if (mCursorItem < mItemCount - 1) {
                    mUnk_fb = mCursorItem + 1;
                } else {
                    mUnk_fb = 0;
                }
                SoundManager::Get().StartSeqArc(5);
                return;
        }
    }
}

void ListScene::func_0207dea4() {
    switch (mUnk_1a0) {
        case 1:
            mUnk_4c->mEnabled = 0;
            if (mCommandList != NULL) {
                CloseCommandList(mCommandList);
                mContext->mRequest = 0;
                mUnk_400          = 1;
                mUnk_1a0          = 3;
            } else if (mCommandPage == NULL) {
                mUnk_1a0 = 2;
            } else {
                CloseCommandPage(mCommandPage, 1);
                mContext->mRequest = 0;
                mUnk_401          = 1;
                mUnk_1a0          = 3;
            }
            SpriteManager::Get().sFade[0] = 3;
            break;
        case 2:
            if (SpriteManager::Get().sFade[0] <= 1) {
                ReleaseMainScreenGraphics();
                mUnk_1a0 = 4;
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
                mUnk_1a0 = 4;
            }
            break;
        case 4:
            func_0208038c();
            mUnk_1a0        = 0;
            mUnk_4c->mEnabled = 1;
            break;
    }
}

void ListScene::func_0207e074() {
    if (mUnk_194[0] != 0) {
        mUnk_194[0] = 0;
        if (mUnk_e8 > mUnk_f4 && mUnk_f0 != 0) {
            SetItemFlags(0, 1, 8);
            mUnk_4c->mCellAnims[1]->Rewind();
        } else {
            SetItemFlags(0, 1, 0x8000);
        }
        if (mUnk_e8 < mUnk_e9 - 1 && mUnk_f0 != 0) {
            SetItemFlags(0, 2, 8);
            mUnk_4c->mCellAnims[2]->Rewind();
        } else {
            SetItemFlags(0, 2, 0x8000);
        }
        for (u8 i = 0; i <= 4; i++) {
            if (mUnk_e8 + i >= mUnk_e9 || mUnk_f0 == 0) {
                SetItemFlags(2, i, 0x8000);
            } else {
                SetItemFlags(2, i, 8);
            }
        }
        for (u8 i = 5; i <= 7; i++) {
            if (i - 5 >= mItemCount || mUnk_f0 != 0) {
                SetItemFlags(2, i, 0x8000);
            } else {
                SetItemFlags(2, i, 8);
            }
        }
        SetItemFlags(0, 0, 8);
        if (mItemCount < 2 || mUnk_f0 != 0) {
            SetItemFlags(0, 5, 0x8000);
            SetItemFlags(0, 6, 0x8000);
        } else {
            SetItemFlags(0, 5, 8);
            mUnk_4c->mCellAnims[5]->Rewind();
            SetItemFlags(0, 6, 8);
            mUnk_4c->mCellAnims[6]->Rewind();
        }
        switch (mMode) {
            case 0:
            case 1:
            case 2:
            case 4:
                SetItemFlags(0, 3, 0x8000);
                SetItemFlags(0, 4, 0x8000);
                break;
            case 3:
            case 5:
                if (mUnk_e6 != 0 && mUnk_e6 < 4 && mUnk_f0 == 0) {
                    SetItemFlags(0, 3, 8);
                    SetItemFlags(0, 4, 8);
                    mSubScreenCellAnims[7]->SetAnimation((mUnk_104 + 5) % 6 * 2 + 0x10);
                    mSubScreenCellAnims[8]->SetAnimation((mUnk_104 + 1) % 6 * 2 + 0x10);
                } else {
                    SetItemFlags(0, 3, 0x8000);
                    SetItemFlags(0, 4, 0x8000);
                }
                break;
        }
        if (mUnk_f0 != 0) {
            mUnk_54->mSelection = 0;
        } else {
            mUnk_54->mSelection = mCursorItem - mScroll + 5;
        }
    }
    mUnk_e2 = mUnk_4c->Update(0);
}

void ListScene::func_0207e424() {
    switch (mUnk_f5) {
        case 2:
            if (InputManager::Get().IsDown(KEY_LEFT) == 0 && mUnk_ec == 0) {
                func_0207e7e4();
            }
            break;
        case 4:
            if (InputManager::Get().IsDown(KEY_RIGHT) == 0 && mUnk_ec == 0) {
                func_0207e7e4();
            }
            break;
        case 5:
            if (TouchManager::Get().mTouching != 0) {
                mUnk_f5 = 0;
            }
            break;
        case 6:
            if (TouchManager::Get().mTouching != 0 && mUnk_ec == 0) {
                func_0207e7e4();
            }
            break;
        case 7:
            if (TouchManager::Get().mTouching != 0) {
                mUnk_f5 = 0;
            }
            break;
        case 8:
            if (TouchManager::Get().mTouching != 0 && mUnk_ec == 0) {
                func_0207e7e4();
            }
            break;
        case 9:
            if (TouchManager::Get().mTouching != 0) {
                mUnk_f5 = 0;
            }
            break;
    }
    if (mUnk_ec != 0 || mUnk_e7 != mUnk_e8) {
        if (mUnk_ec == 0) {
            mUnk_ec = 1;
            if (mUnk_e7 <= mUnk_e8) {
                mUnk_e4 = -0x2c;
                mUnk_ea--;
                mUnk_e8--;
            }
        }
        if (mUnk_e7 > mUnk_e8) {
            mUnk_e4 -= 0xb;
        } else {
            mUnk_e4 += 0xb;
        }
        if (mUnk_e4 >= 0 || mUnk_e4 <= -0x2c) {
            mUnk_ec = 0;
            if (mUnk_e7 > mUnk_e8) {
                mUnk_ea++;
                mUnk_e8++;
            }
            mUnk_e4 = 0;
            if (mUnk_e7 == mUnk_e8) {
                mUnk_194[0] = 1;
                if (func_020862d0()) {
                    func_0207e7e4();
                }
            }
        }
    }
    if (mUnk_194[1] != 0) {
        if (IsCommandListOrPageSettingUp() == 0) {
            func_020807cc();
            mUnk_194[1] = 0;
        }
    }
}

void ListScene::func_0207e7e4() {
    if (mUnk_f5 != 5 && mUnk_f5 != 7) {
        mUnk_f5 = 0;
    }
    mUnk_e6      = mUnk_e7;
    mUnk_194[2] = 1;
    switch (mMode) {
        case 0:
        case 1:
        case 2:
            mUnk_104 = mUnk_e6;
            BuildList(1);
            func_02081128();
            func_02081594();
            break;
        case 3:
        case 5:
            SetMessage(-1);
            if (mUnk_e6 == 0) {
                BuildList(1);
                if (mUnk_1a0 == 0) {
                    mUnk_1a0 = 1;
                }
            } else if (mUnk_e6 < 4) {
                mUnk_104 = mUnk_c8[mUnk_e6 + 8];
                BuildList(1);
                if (mCommandList == NULL || mUnk_400 != 0) {
                    if (mUnk_1a0 == 0) {
                        mUnk_1a0 = 1;
                    }
                } else {
                    func_02081128();
                }
            } else {
                mUnk_104 = mUnk_e6 - 4;
                BuildList(1);
                if (mCommandPage == NULL || mUnk_401 != 0) {
                    if (mUnk_1a0 == 0) {
                        mUnk_1a0 = 1;
                    }
                } else {
                    func_02081594();
                }
            }
            break;
        case 4:
            if (mUnk_e6 == 0) {
                BuildList(1);
                if (mUnk_1a0 == 0) {
                    mUnk_1a0 = 1;
                }
                GXS_SetVisiblePlane(0x19);
            } else if (mUnk_e6 < 7) {
                mUnk_104 = mUnk_e6 - 1;
                BuildList(1);
                if (mCommandList == NULL || mUnk_400 != 0) {
                    if (mUnk_1a0 == 0) {
                        mUnk_1a0 = 1;
                    }
                } else {
                    func_02081128();
                }
                GXS_SetVisiblePlane(0x1d);
            } else {
                mUnk_104 = mUnk_e6 - 7;
                BuildList(1);
                if (mCommandPage == NULL || mUnk_401 != 0) {
                    if (mUnk_1a0 == 0) {
                        mUnk_1a0 = 1;
                    }
                } else {
                    func_02081594();
                }
                GXS_SetVisiblePlane(0x1d);
            }
            break;
    }
}

void ListScene::func_0207ea3c() {
    if (mUnk_100 != 0 || mUnk_fb != mCursorItem) {
        if (mUnk_100 == 0 && mScroll <= mUnk_fb && mUnk_fb < mScroll + 3) {
            mCursorItem = mUnk_fb;
            func_02081128();
            func_02081594();
            SetMessage(-1);
            mUnk_194[0] = 1;
        } else if (IsCommandListOrPageSettingUp() == 0) {
            u32 moved = 0;
            if (mUnk_100 == 0) {
                mUnk_100 = 1;
                if (mUnk_fb > mCursorItem) {
                    if (mUnk_fb - mCursorItem > 1) {
                        mCursorItem = mUnk_fb;
                        mScroll = mUnk_fb;
                        if (mScroll >= mItemCount - 3) {
                            mScroll = mItemCount - 3;
                        }
                        moved   = 1;
                    } else {
                        LoadBakuganMark((u8) mItemIds[mScroll + 3], (u8) ((mScroll + 3) % 4), 2);
                        LoadSmallCard(mItemIds[mScroll + 3], (u8) ((mScroll + 3) % 4), 2);
                    }
                } else {
                    if (mCursorItem - mUnk_fb > 1) {
                        mCursorItem = mUnk_fb;
                        mScroll = mUnk_fb;
                        if (mScroll >= mItemCount - 3) {
                            mScroll = mItemCount - 3;
                        }
                        moved   = 1;
                    } else {
                        mUnk_f8 = -0x20;
                        mScroll--;
                        mCursorItem--;
                        LoadBakuganMark((u8) mItemIds[mScroll], (u8) (mScroll % 4), 2);
                        LoadSmallCard(mItemIds[mScroll], (u8) (mScroll % 4), 2);
                        PrintRows();
                    }
                }
            }
            if (moved == 0) {
                if (mUnk_fb > mCursorItem) {
                    mUnk_f8 -= 8;
                } else {
                    mUnk_f8 += 8;
                }
                if (mUnk_f8 >= 0 || mUnk_f8 <= -0x20) {
                    mUnk_100 = 0;
                    if (mUnk_fb > mCursorItem) {
                        mScroll++;
                        mCursorItem++;
                        PrintRows();
                    }
                    mUnk_f8 = 0;
                    func_02081128();
                    func_02081594();
                    mUnk_194[0] = 1;
                }
                QueueFrameCallback(ListSceneScrollCallback, this);
            } else {
                mUnk_100 = 0;
                func_02081128();
                func_02081594();
                mUnk_194[0] = 1;
                mUnk_194[2] = 1;
            }
        }
    }
    if (mUnk_194[2] != 0) {
        if (IsCommandListOrPageSettingUp() == 0) {
            func_02080898();
            mUnk_194[2] = 0;
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

void ListScene::func_0207eec4() {
    if (mCommandList != NULL || mCommandPage != NULL) {
        return;
    }
    if (mUnk_1a0 > 2) {
        return;
    }
    u8 i = 0;
    switch (mMode) {
        case 3:
        case 5:
            if (mMainScreenCellAnims[0] != NULL) {
                mMainScreenCellAnims[0]->Draw(0, 0x80, 0x60, 0);
            }
            if (mMainScreenCellAnims[3] != NULL) {
                mMainScreenCellAnims[3]->DrawFrame(0, 0, 0x80, 0x60, 0);
            }
            for (; i < 9; i++) {
                if (i < 3) {
                    if (mUnk_3f4 == 0 || i == 0) {
                        DrawBakuganMark((u8) mUnk_c8[9 + i], i, 0, i * 24 + 0x15, 0x57);
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
        case 4:
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
    if (mUnk_4c != NULL) {
        mUnk_4c->Draw();
    }
    if (mUnk_48 != NULL) {
        mUnk_48->Draw();
    }
    if (mDeckWindow != NULL) {
        mDeckWindow->Draw();
    }
}

void ListScene::func_0207f32c() {
    if (mUnk_3ff != 0) {
        if (mUnk_e6 < 4) {
            DrawBakuganMark(mUnk_104, (u8) (mCursorItem % 4), 4, 0x1d, mUnk_3fc + 0x25);
        } else {
            DrawSmallCard(mUnk_104, (u8) (mCursorItem % 4), 4, 0x20, mUnk_3fc + 0x24);
        }
    }
    u8 limit = 6;
    u8 i     = 0;
    if (mUnk_ec == 0) {
        limit--;
    }
    do {
        if (mUnk_ea + i >= mUnk_e9) {
            break;
        }
        switch (mMode) {
            case 0:
            case 1:
                mSubScreenCellAnims[4]->DrawFrame(1, (u8) (mUnk_ea + i), mUnk_e4 + 0x1e + i * 0x2c, 0x26, 1);
                break;
            case 2:
                mSubScreenCellAnims[5]->DrawFrame(1, (u8) (mUnk_ea + i), mUnk_e4 + 0x20 + i * 0x2c, 0x24, 1);
                break;
            case 3:
            case 5:
                if ((mUnk_ea + i) == 0) {
                    mSubScreenCellAnims[10]->SetAnimation(0x1d);
                    mSubScreenCellAnims[10]->Draw(1, (s16) (mUnk_e4 + 0x19 + i * 0x2c), 0x21, 1);
                } else if ((mUnk_ea + i) < 4) {
                    if (mUnk_3f4 == 0 || (mUnk_ea + i) == 1) {
                        DrawBakuganMark((u8) mUnk_c8[(mUnk_ea + i) + 8], (u8) ((mUnk_ea + i) - 1), 1, mUnk_e4 + 0x1d + i * 0x2c, 0x25);
                    }
                } else {
                    DrawSmallCard((mUnk_ea + i) - 4, (mUnk_ea + i) - 4, 1, mUnk_e4 + 0x20 + i * 0x2c, 0x24);
                }
                break;
            case 4:
                if ((mUnk_ea + i) == 0) {
                    mSubScreenCellAnims[10]->SetAnimation(0x20);
                    mSubScreenCellAnims[10]->Draw(1, (s16) (mUnk_e4 + 0x19 + i * 0x2c), 0x21, 1);
                } else if ((mUnk_ea + i) < 7) {
                    mSubScreenCellAnims[4]->DrawFrame(1, (u8) ((mUnk_ea + i) - 1), mUnk_e4 + 0x1e + i * 0x2c, 0x26, 1);
                } else {
                    mSubScreenCellAnims[5]->DrawFrame(1, (u8) ((mUnk_ea + i) - 7), mUnk_e4 + 0x20 + i * 0x2c, 0x24, 1);
                }
                break;
        }
        i++;
    } while (i < limit);
    mSubScreenCellAnims[0]->Draw(1, 0x80, 0x60, 1);
    switch (mMode) {
        case 0:
        case 1:
            mSubScreenCellAnims[1]->DrawFrame(1, 1, mUnk_e4 + 0x14 - mUnk_ea * 0x2c, 0x22, 1);
            break;
        case 2:
            mSubScreenCellAnims[1]->DrawFrame(1, 0, mUnk_e4 + 0x14 - mUnk_ea * 0x2c, 0x22, 1);
            mSubScreenCellAnims[1]->DrawFrame(1, 0, mUnk_e4 + 0x98 - mUnk_ea * 0x2c, 0x22, 1);
            break;
        case 3:
        case 5:
            if (mUnk_ea < 6) {
                mSubScreenCellAnims[1]->DrawFrame(1, 0, mUnk_e4 + 0x40 - mUnk_ea * 0x2c, 0x22, 1);
            }
            mSubScreenCellAnims[1]->DrawFrame(1, 0, mUnk_e4 + 0xc4 - mUnk_ea * 0x2c, 0x22, 1);
            if (mUnk_ea != 0) {
                mSubScreenCellAnims[1]->DrawFrame(1, 0, mUnk_e4 + 0x148 - mUnk_ea * 0x2c, 0x22, 1);
            }
            break;
        case 4:
            if (mUnk_ea < 8) {
                mSubScreenCellAnims[1]->DrawFrame(1, 1, mUnk_e4 + 0x40 - mUnk_ea * 0x2c, 0x22, 1);
            }
            if (mUnk_ea != 0) {
                mSubScreenCellAnims[1]->DrawFrame(1, 0, mUnk_e4 + 0x148 - mUnk_ea * 0x2c, 0x22, 1);
            }
            if (mUnk_ea > 3) {
                mSubScreenCellAnims[1]->DrawFrame(1, 0, mUnk_e4 + 0x1cc - mUnk_ea * 0x2c, 0x22, 1);
            }
            break;
    }
}

void ListScene::func_0207f90c() {
    if (mMode == 4 && mUnk_e6 == 0) {
        return;
    }
    mSubScreenCellAnims[15]->Update();
    u8 count = 4;
    u8 i     = 0;
    if (mUnk_100 == 0) {
        count--;
    }
    do {
        if (mScroll + i >= mItemCount) {
            if (mItemCount == 0 && i == 0) {
                mSubScreenCellAnims[12]->SetAnimation(0x21);
                mSubScreenCellAnims[12]->Draw(1, 0x71, (s16) (mUnk_f8 + 0x4b + i * 32), 2);
            }
        } else {
            switch (mMode) {
                case 0:
                    if (func_020231f4(gSaveData, 0, (u8) mItemIds[mScroll + i], mUnk_104) == 0) {
                        mSubScreenCellAnims[15]->Draw(1, 0x19, (s16) (mUnk_f8 + 0x5b + i * 32), 2);
                    }
                    DrawBakuganMark(mUnk_104, (u8) ((mScroll + i) % 4), 2, 0x1d, mUnk_f8 + 0x49 + i * 32);
                    func_020816ec(GetPrice((u8) mItemIds[mScroll + i], mUnk_104), i);
                    break;
                case 1:
                    DrawBakuganMark(mUnk_104, (u8) ((mScroll + i) % 4), 2, 0x1d, mUnk_f8 + 0x49 + i * 32);
                    func_020816ec(GetEntryPrice((u8) mItemIds[mScroll + i], mUnk_104), i);
                    if (IsInSlots(1, (u8) mItemIds[mScroll + i], mUnk_104) != 0) {
                        mSubScreenCellAnims[9]->Draw(1, 0x80, (s16) (mUnk_f8 + 0x54 + i * 32), 2);
                    }
                    break;
                case 2:
                    DrawSmallCard(mUnk_104, (u8) ((mScroll + i) % 4), 2, 0x20, mUnk_f8 + 0x48 + i * 32);
                    func_020816ec(Table_020a39fc()[mItemIds[mScroll + i]], i);
                    break;
                case 3:
                case 5:
                    if (mUnk_e6 == 0) {
                        mSubScreenCellAnims[11]->DrawFrame(1, (u8) (i + 1), 0x19, mUnk_f8 + 0x45 + i * 32, 2);
                        mSubScreenCellAnims[12]->SetAnimation(0x1f);
                        mSubScreenCellAnims[12]->DrawFrame(1, (u8) (i + 1), 0x71, mUnk_f8 + 0x4b + i * 32, 2);
                    } else if (mUnk_e6 < 4) {
                        DrawBakuganMark(mUnk_104, (u8) ((mScroll + i) % 4), 2, 0x1d, mUnk_f8 + 0x49 + i * 32);
                        if (IsInSlots(1, (u8) mItemIds[mScroll + i], mUnk_104) != 0) {
                            mSubScreenCellAnims[9]->Draw(1, 0x80, (s16) (mUnk_f8 + 0x54 + i * 32), 2);
                        }
                    } else {
                        DrawSmallCard(mUnk_104, (u8) ((mScroll + i) % 4), 2, 0x20, mUnk_f8 + 0x48 + i * 32);
                        if (IsInSlots(0, (u8) mItemIds[mScroll + i], 0) != 0) {
                            mSubScreenCellAnims[9]->Draw(1, 0x80, (s16) (mUnk_f8 + 0x54 + i * 32), 2);
                        }
                    }
                    break;
                case 4:
                    if (mUnk_e6 < 7) {
                        DrawBakuganMark(mUnk_104, (u8) ((mScroll + i) % 4), 2, 0x1d, mUnk_f8 + 0x49 + i * 32);
                        if (IsInSlots(1, (u8) mItemIds[mScroll + i], mUnk_104) != 0) {
                            mSubScreenCellAnims[9]->Draw(1, 0x80, (s16) (mUnk_f8 + 0x54 + i * 32), 2);
                        }
                    } else {
                        DrawSmallCard(mUnk_104, (u8) ((mScroll + i) % 4), 2, 0x20, mUnk_f8 + 0x48 + i * 32);
                        if (IsInSlots(0, (u8) mItemIds[mScroll + i], 0) != 0) {
                            mSubScreenCellAnims[9]->Draw(1, 0x80, (s16) (mUnk_f8 + 0x54 + i * 32), 2);
                        }
                    }
                    break;
            }
        }
        i++;
    } while (i < count);
    if ((mMode == 3 || mMode == 5) && mUnk_e6 == 0) {
        return;
    }
    func_0207ff54();
}

void ListScene::func_0207ff54() {
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

void ListScene::func_02080104() {
    switch (mMode) {
        case 0:
        case 1:
        case 2:
            func_020816ec(gSaveData[0].mMoney, 4);
            break;
        case 3:
        case 5:
            if (mUnk_4c->mButtons[3].mEnabled != 0) {
                mSubScreenCellAnims[7]->Update();
                mSubScreenCellAnims[7]->Draw(1, 5, 0x6c, 1);
            }
            if (mUnk_4c->mButtons[4].mEnabled != 0) {
                mSubScreenCellAnims[8]->Update();
                mSubScreenCellAnims[8]->Draw(1, 0xed, 0x6c, 1);
            }
            break;
    }
    mSubScreenCellAnims[3]->Update();
    if (mUnk_f0 != 0) {
        mSubScreenCellAnims[3]->Draw(1, 0x19, 0x21, 1);
    } else {
        mSubScreenCellAnims[3]->Draw(1, 0x19, (s16) ((mCursorItem - mScroll) * 32 + 0x45), 1);
    }
}

void ListScene::ReadSlots() {
    if (mCharacterId != 0x15) {
        for (u8 i = 0; i < 3; i++) {
            u8 *entry = (u8 *) ((u32 *) data_020d433c.mBattlePlayers)[mPlayer + 3] + 0xc + i * 12;
            mUnk_c8[i] = entry[8];
            mUnk_c8[9 + i] = entry[9];
        }
        for (u8 i = 0; i < 3; i++) {
            mUnk_c8[3 + i] = *(u16 *) ((u8 *) ((u32 *) data_020d433c.mBattlePlayers)[mPlayer + 3] + i * 4 + 0x54);
        }
        for (u8 i = 0; i < 3; i++) {
            mUnk_c8[6 + i] = *(u16 *) ((u8 *) ((u32 *) data_020d433c.mBattlePlayers)[mPlayer + 3] + i * 4 + 0x6c);
        }
    } else {
        for (u8 i = 0; i < 3; i++) {
            mUnk_c8[i]     = GetSlotBakuganId(gSaveData, 0, i);
            mUnk_c8[9 + i] = GetSlotBakuganAttribute(gSaveData, 0, i);
        }
        for (u8 i = 0; i < 3; i++) {
            mUnk_c8[3 + i] = gSaveData[0].mUnk_06[i];
        }
        for (u8 i = 0; i < 3; i++) {
            mUnk_c8[6 + i] = gSaveData[0].mUnk_0c[i];
        }
    }
}

void ListScene::func_0208038c() {
    switch (mMode) {
        case 0:
        case 1:
            if (mCommandList != NULL) {
                return;
            }
            mCommandList = new BattleCommandList(0, 0, -1, 0);
            CreateCommandListWindow(mCommandList, 0x1d000, 0, 0xc0, 0x14, 0xe0, 0x14, 0);
            mUnk_400 = 0;
            func_02081128();
            break;
        case 2:
            if (mCommandPage != NULL) {
                return;
            }
            mCommandPage = new BattleCommandPage(0, -1);
            CreateCommandPageWindow(mCommandPage, 0x1d000, 0, 0xc0, 0x14, 0xe0, 0x14, 0);
            mUnk_401 = 0;
            func_02081594();
            break;
        case 3:
        case 5:
            if (mUnk_e6 == 0) {
                InitAndLoadMainScreen();
                for (u8 i = 0; i < 9; i++) {
                    if (i < 3) {
                        LoadBakuganMark((u8) mUnk_c8[i], i, 0);
                    } else {
                        LoadSmallCard(mUnk_c8[i], (u8) (i - 3), 0);
                    }
                }
                SpriteManager::Get().sFade[0] = 2;
                return;
            }
            if (mUnk_e6 < 4) {
                if (mCommandList != NULL) {
                    return;
                }
                mCommandList = new BattleCommandList(0, 0, -1, 0);
                CreateCommandListWindow(mCommandList, 0x1d000, 0, 0xc0, 0x14, 0xe0, 0x14, 0);
                mUnk_400 = 0;
                func_02081128();
                return;
            }
            if (mCommandPage != NULL) {
                return;
            }
            mCommandPage = new BattleCommandPage(0, -1);
            CreateCommandPageWindow(mCommandPage, 0x1d000, 0, 0xc0, 0x14, 0xe0, 0x14, 0);
            mUnk_401 = 0;
            func_02081594();
            return;
        case 4:
            if (mUnk_e6 == 0) {
                InitAndLoadMainScreen();
                SpriteManager::Get().sFade[0] = 2;
                return;
            }
            if (mUnk_e6 < 7) {
                if (mCommandList != NULL) {
                    return;
                }
                mCommandList = new BattleCommandList(0, 0, -1, 0);
                CreateCommandListWindow(mCommandList, 0x1d000, 0, 0xc0, 0x14, 0xe0, 0x14, 0);
                mUnk_400 = 0;
                func_02081128();
                return;
            }
            if (mCommandPage != NULL) {
                return;
            }
            mCommandPage = new BattleCommandPage(0, -1);
            CreateCommandPageWindow(mCommandPage, 0x1d000, 0, 0xc0, 0x14, 0xe0, 0x14, 0);
            mUnk_401 = 0;
            func_02081594();
            break;
    }
}

void ListScene::func_020807cc() {
    SpriteManager::Get();
    if (mMode != 3 && mMode != 5) {
        return;
    }
    for (u8 i = 0; i < 9; i++) {
        if (i < 3) {
            LoadBakuganMark((u8) mUnk_c8[i], i, 1);
        } else {
            LoadSmallCard(mUnk_c8[i], (u8) (i - 3), 1);
        }
    }
}

void ListScene::func_02080898() {
    SpriteManager::Get();
    u8 i = 0;
    switch (mMode) {
        case 0:
        case 1:
            for (; i < 3; i++) {
                if (mScroll + i >= mItemCount) {
                    return;
                }
                LoadBakuganMark((u8) mItemIds[mScroll + i], (u8) ((mScroll + i) % 4), 2);
            }
            break;
        case 2:
            for (; i < 3; i++) {
                if (mScroll + i >= mItemCount) {
                    return;
                }
                LoadSmallCard(mItemIds[mScroll + i], (u8) ((mScroll + i) % 4), 2);
            }
            break;
        case 3:
        case 5:
            if (mUnk_e6 == 0) {
                return;
            }
            if (mUnk_e6 < 4) {
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
        case 4:
            if (mUnk_e6 == 0) {
                return;
            }
            if (mUnk_e6 < 7) {
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
            if (gSaveData[0].mUnk_5d2 != 0) {
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

// Fills mItemIds for the mode and sorts it, then puts the cursor back (mUnk_186[mUnk_e6], or the stored selection in
// modes 3/5) and clamps mScroll; unk1 != 0 scrolls to the cursor. Page 0 of modes 3/5 sets mItemCount = 2 with no ids.
void ListScene::BuildList(u32 unk1) {
    u8 i = 0;
    mItemCount = 0;
    switch (mMode) {
        case 0:
        case 1:
            if (mMode == 0) {
                for (; i < 0x29; i++) {
                    if (TestFlagBit(data_020d4976, i) && data_020a3768[i][mUnk_104] != 0 && func_02080b48(i, mUnk_104)) {
                        mItemIds[mItemCount] = i;
                        mItemCount++;
                    }
                }
                if (!(gSaveData[0].mUnk_586[4] & 4) && mUnk_104 == 0 && gSaveData[0].mUnk_5d2 != 0) {
                    mItemIds[mItemCount] = 0x22;
                    mItemCount++;
                }
            } else if (mMode == 1) {
                for (; i < 0x29; i++) {
                    if (func_020231f4(gSaveData, 0, i, mUnk_104) != 0) {
                        mItemIds[mItemCount] = i;
                        mItemCount++;
                    }
                }
            }
            SortListSceneItems(this, 1);
            break;
        case 2: {
            for (u16 k = 1; k < 0xd5; k++) {
                if (TestFlagBit(data_020d497b, k) && !TestFlagBit(data_020d4946, k) && GetCardInfo(k, 1) == mUnk_104 && GetCardNumber(k) < 0x2bc) {
                    mItemIds[mItemCount] = k;
                    mItemCount++;
                }
            }
            SortListSceneItems(this, 0);
            break;
        }
        case 3:
        case 5:
            if (mCharacterId != 0x15) {
                mItemCount = 0;
            } else if (mUnk_e6 == 0) {
                mItemCount = 2;
                mCursorItem = 0;
            } else if (mUnk_e6 < 4) {
                if (mUnk_3f4 != 0) {
                    mItemCount = 0;
                } else {
                    for (; i < 0x29; i++) {
                        if (func_020231f4(gSaveData, 0, i, mUnk_104) != 0) {
                            mItemIds[mItemCount] = i;
                            mItemCount++;
                        }
                    }
                    SortListSceneItems(this, 1);
                    u8 j = 0;
                    do {
                        if (mItemIds[j] == mUnk_c8[mUnk_e6 - 1] && mUnk_104 == mUnk_c8[mUnk_e6 + 8]) {
                            mCursorItem = j;
                        }
                        j++;
                    } while (j < mItemCount);
                }
            } else {
                for (u16 k = 1; k < 0xd5; k++) {
                    if (TestFlagBit(data_020d4946, k) && GetCardInfo(k, 1) == mUnk_104 && GetCardNumber(k) < 0x2bc) {
                        mItemIds[mItemCount] = k;
                        mItemCount++;
                    }
                }
                SortListSceneItems(this, 0);
                u16 j = 0;
                do {
                    if (mItemIds[j] == mUnk_c8[mUnk_e6 - 1]) {
                        mCursorItem = j;
                    }
                    j++;
                } while (j < mItemCount);
            }
            break;
        case 4:
            if (mUnk_e6 != 0) {
                if (mUnk_e6 < 7) {
                    for (; i < 0x29; i++) {
                        if (func_020231f4(gSaveData, 0, i, mUnk_104) != 0) {
                            mItemIds[mItemCount] = i;
                            mItemCount++;
                        }
                    }
                    SortListSceneItems(this, 1);
                } else {
                    for (u16 k = 1; k < 0xd5; k++) {
                        if (TestFlagBit(data_020d4946, k) && GetCardInfo(k, 1) == mUnk_104 && GetCardNumber(k) < 0x2bc) {
                            mItemIds[mItemCount] = k;
                            mItemCount++;
                        }
                    }
                    SortListSceneItems(this, 0);
                }
            }
            break;
    }
    if (mMode != 3 && mMode != 5) {
        mCursorItem = mUnk_186[mUnk_e6];
    }
    if (mItemCount == 0) {
        mCursorItem = 0;
    } else if (mCursorItem > mItemCount - 1) {
        mCursorItem = mItemCount - 1;
    }
    mUnk_fb = mCursorItem;
    if (unk1 != 0) {
        mScroll = mCursorItem;
    }
    if (mItemCount < 3) {
        mScroll = 0;
    } else if (mScroll >= mItemCount - 3) {
        mScroll = mItemCount - 3;
    }
}

void ListScene::func_02081128() {
    BattleCellPair *pair;
    u8 id;
    u8 kind;
    u32 value;
    if (mCommandList == NULL) {
        return;
    }
    if (mUnk_400 != 0) {
        return;
    }
    switch (mMode) {
        case 0: {
            BattleCellPair cell;
            mCommandList->func_02058240(0);
            if (mUnk_f0 != 0) {
                func_02058248(mCommandList, mUnk_e6);
                return;
            }
            if (mItemCount == 0) {
                func_02058228(mCommandList, 1);
                return;
            }
            func_02058228(mCommandList, 0);
            func_02086580(this, (u8) mItemIds[mCursorItem], mUnk_104, &cell);
            SetCommandListBakugan(mCommandList, (u8) mItemIds[mCursorItem], mUnk_104, -1);
            func_0205afb4(mCommandList, mUnk_104, func_02086548((u8) mItemIds[mCursorItem], mUnk_104), &cell);
            return;
        }
        case 1: {
            if (mUnk_f0 != 0) {
                func_02058248(mCommandList, mUnk_e6);
                return;
            }
            if (mItemCount == 0) {
                func_02058228(mCommandList, 1);
                return;
            }
            func_02058228(mCommandList, 0);
            pair = (BattleCellPair *) GetBakuganEntry(gSaveData, (u8) mItemIds[mCursorItem], mUnk_104);
            SetCommandListBakugan(mCommandList, (u8) mItemIds[mCursorItem], mUnk_104, -1);
            func_0205afb4(mCommandList, mUnk_104, func_020231f4(gSaveData, 0, (u8) mItemIds[mCursorItem], mUnk_104), pair);
            return;
        }
        case 3:
        case 5: {
            if (mUnk_e6 == 0) {
                return;
            }
            if (mUnk_e6 >= 4) {
                return;
            }
            if (mUnk_f0 != 0) {
                if (mUnk_3f4 != 0 && mUnk_e6 != 1) {
                    func_02058228(mCommandList, 1);
                    return;
                }
                if (mCharacterId != 0x15) {
                    BattleRecord *rec = &data_020d433c.mBattlePlayers->mPlayers[mPlayer]->mRecords[(u8) (mUnk_e6 - 1)];
                    pair  = (BattleCellPair *) rec;
                    value = rec->mUnk_04;
                    id    = mUnk_c8[mUnk_e6 - 1];
                    kind  = mUnk_c8[mUnk_e6 + 8];
                } else {
                    id    = mUnk_c8[mUnk_e6 - 1];
                    kind  = mUnk_c8[mUnk_e6 + 8];
                    pair  = (BattleCellPair *) GetBakuganEntry(gSaveData, id, kind);
                    value = func_020231f4(gSaveData, 0, id, kind);
                }
            } else {
                if (mItemCount == 0) {
                    func_02058228(mCommandList, 1);
                    return;
                }
                kind  = mUnk_104;
                id    = mItemIds[mCursorItem];
                pair  = (BattleCellPair *) GetBakuganEntry(gSaveData, id, kind);
                value = func_020231f4(gSaveData, 0, id, kind);
            }
            func_02058228(mCommandList, 0);
            SetCommandListBakugan(mCommandList, id, kind, -1);
            func_0205afb4(mCommandList, kind, value, pair);
            return;
        }
        case 4: {
            if (mUnk_e6 == 0) {
                return;
            }
            if (mUnk_e6 >= 7) {
                return;
            }
            if (mUnk_f0 != 0) {
                func_02058248(mCommandList, (u8) (mUnk_e6 - 1));
                return;
            }
            if (mItemCount == 0) {
                func_02058228(mCommandList, 1);
                return;
            }
            func_02058228(mCommandList, 0);
            BattleCellPair *pair = (BattleCellPair *) GetBakuganEntry(gSaveData, (u8) mItemIds[mCursorItem], mUnk_104);
            SetCommandListBakugan(mCommandList, (u8) mItemIds[mCursorItem], mUnk_104, -1);
            u32 value = func_020231f4(gSaveData, 0, (u8) mItemIds[mCursorItem], mUnk_104);
            func_0205afb4(mCommandList, mUnk_104, value, pair);
            break;
        }
    }
}

void ListScene::func_02081594() {
    if (mCommandPage == NULL) {
        return;
    }
    if (mUnk_401 != 0) {
        return;
    }
    switch (mMode) {
        case 2:
            if (mUnk_f0 != 0) {
                SetCommandPageCard(mCommandPage, (u16) (mUnk_e6 + 0xd7), -1);
                return;
            }
            if (mItemCount == 0) {
                if (mUnk_e6 < 3) {
                    SetCommandPageCard(mCommandPage, 0, -1);
                } else {
                    SetCommandPageCard(mCommandPage, 0xd6, -1);
                }
            } else {
                SetCommandPageCard(mCommandPage, mItemIds[mCursorItem], -1);
            }
            break;
        case 3:
        case 5:
            if (mUnk_e6 < 4) {
                return;
            }
            {
                u16 v;
                if (mUnk_f0 != 0) {
                    v = mUnk_c8[mUnk_e6 - 1];
                } else {
                    v = mItemIds[mCursorItem];
                }
                SetCommandPageCard(mCommandPage, (u8) v, -1);
            }
            break;
        case 4:
            if (mUnk_e6 < 7) {
                return;
            }
            if (mUnk_f0 != 0) {
                SetCommandPageCard(mCommandPage, (u16) (mUnk_e6 + 0xd0), -1);
            } else {
                SetCommandPageCard(mCommandPage, mItemIds[mCursorItem], -1);
            }
            break;
    }
}

void ListScene::func_020816ec(u32 unk1, u32 unk2) {
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
        y     = mUnk_f8 + (unk2 * 0x20 + 0x56);
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
            items = mUnk_4c->mCursor->mItems;
            mUnk_4c->mButtons[unk2].mEnabled = unk3 != 0x8000 ? 1 : 0;
            break;
        case 1:
            items = mUnk_48->mCursor->mItems;
            mUnk_48->mButtons[unk2].mEnabled = unk3 != 0x8000 ? 1 : 0;
            break;
        case 2:
            items = mUnk_54->mItems;
            break;
        case 3:
            items = mDeckWindow->mCursor->mItems;
            mDeckWindow->mButtons[unk2].mEnabled = unk3 != 0x8000 ? 1 : 0;
            break;
        default:
            return;
    }
    items[unk2].mFlags = unk3;
}

void ListSceneScrollCallback(void *arg) {
    ListScene *self = (ListScene *) arg;
    reg_G2S_DB_BG2OFS = (u32) ((-0x20 - self->mUnk_f8) << 16) & G2_BGOFS_V_MASK;
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
    SpriteManager::Get().Screen(second).AddOam((void *) data_020a3188[1], 1, (s16) (unk4 + 3), (s16) (unk5 + 3), priority, pos, 1, 0);
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
        case 2:
            return;
        case 3:
        case 5:
            if (mUnk_e6 >= 4) {
                return;
            }
            break;
        case 4:
            if (mUnk_e6 == 0) {
                return;
            }
            if (mUnk_e6 >= 7) {
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
    LoadAndQueueUpload(second, 4, offset, path, 0, 0, 0);
}

static inline void PrintMessageFW(TextManager &tm, MessageFile *file, int index, void *buffer, s16 y, s16 x, u16 *width) {
    tm.Print(file->GetText(index), file->GetLength(index), buffer, 0x18, 0x10, &y, &x, width, 0, 2, 0x44, 0, 0, 0, 0, 1, 1);
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
    SpriteManager::Get().Screen(second).AddOam((void *) data_020a3188[0], 1, unk4, unk5, priority, pos, 2, 0);
    if (unk3 == 0) {
        return;
    }
    mSubScreenCellAnims[6]->DrawFrame(second, unk1, unk4, unk5, priority);
}

void ListScene::LoadSmallCard(u32 unk1, u32 unk2, u32 unk3) {
    switch (mMode) {
        case 0:
        case 1:
            return;
        case 3:
        case 5:
            if (mUnk_e6 != 0 && mUnk_e6 < 4 && unk3 != 1) {
                return;
            }
            break;
        case 4:
            if (mUnk_e6 < 7) {
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
    LoadAndQueueUpload(second, 4, offset, path, 0, 0, 0);
}

void ListScene::PrintCardNameShort(u32 unk1, u32 unk2) {
    TextManager::Get().mUse4bppFont = 1;
    s16 x = unk2 << 5;
    PrintMessageF(TextManager::Get(), (MessageFile *) mCardNameShortMessages, unk1, mTextBuffers[0], 0, x);
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
        case 3:
        case 5:
            LoadAndQueueUpload(0, 1, 0, "list_edit_u_bg_00_ncl.bin", 0, 0, 0);
            LoadAndQueueUpload(0, 0xb, 0, "list_edit_u_bg_00_ncg.bin", 0, 0, 0);
            LoadAndQueueUpload(0, 7, 0, "list_edit_u_bg_00_nsc.bin", 0, 0, 0);
            break;
        case 4:
            LoadAndQueueUpload(0, 1, 0, "list_gallery_u_bg_00_ncl.bin", 0, 0, 0);
            LoadAndQueueUpload(0, 0xb, 0, "list_gallery_u_bg_00_ncg.bin", 0, 0, 0);
            LoadAndQueueUpload(0, 7, 0, "list_gallery_u_bg_00_nsc.bin", 0, 0, 0);
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
    LoadAndQueueUpload(0, 0, 0x200, "bakugan_s_000_ncl.bin", 0, 0, 0);
    LoadAndQueueUpload(0, 0, 0, "bakugan_s_base_ncl.bin", 0, 0, 0);
    LoadAndQueueUpload(0, 4, 0x3000, "bakugan_s_base.nce_ncg1D.bin", 0, 0, 0);
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
    LoadAndQueueUpload(0, 0, 0x400, "card_s_000_ncl.bin", 0, 0, 0);
    InitMainScreenText();
    PrintListSceneMainScreenText(this);
}

BOOL ListScene::IsInSlots(u32 unk1, u32 unk2, u32 unk3) {
    if (unk1 != 0) {
        for (u8 i = 0; i <= 2; i++) {
            if (unk2 == mUnk_c8[i] && unk3 == mUnk_c8[9 + i]) {
                return TRUE;
            }
        }
    } else {
        for (u8 i = 3; i <= 8; i++) {
            if (unk2 == mUnk_c8[i]) {
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
        TextManager::Get().PrintNumber(self->mUnk_1a2[i].mUnk_a, 3, 0, self->mTextBuffers[1], 0x18, 0x14, &b, &a, 0, 2, 0x42, 0, 0, 1, 1);
        TextManager::Get().Print(&self->mAlphabetChar, 2, self->mTextBuffers[1], 0x18, 0x14, &b, &a, NULL, 0, 2, 0x42, 0, 0, 0, 0, 1, 1);
        TextManager::Get().PrintNumber(self->mUnk_1a2[i].mUnk_b, 3, 0, self->mTextBuffers[1], 0x18, 0x14, &b, &a, 0, 2, 0x42, 0, 0, 1, 1);
    }
    QueueUpload(SpriteManager::Get().Screen(0).mBgIndex, 4, 0x10000, self->mTextBuffers[1], self->mTextBufferSizes[1], 0);
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
    MI_CpuClear16(&mUnk_1a2[0], 0x38);
    mUnk_1a2[0].mUnk_b = 0x11;
    mUnk_1a2[1].mUnk_b = 0x11;
    mUnk_1a2[2].mUnk_b = 0x11;
    mUnk_1a2[3].mUnk_b = 0x11;
    mUnk_1a2[4].mUnk_b = 0x12;
    mUnk_1a2[5].mUnk_b = 0x11;
    mUnk_1a2[6].mUnk_b = 0x67;
    mUnk_1a2[gSaveData[0].mAttribute].mUnk_b += 2;
    mUnk_1a2[6].mUnk_b += 2;
    if (gSaveData[0].mUnk_5d2 != 0) {
        mUnk_1a2[0].mUnk_b += 1;
        mUnk_1a2[6].mUnk_b += 1;
    }
    u8 i;
    u8 j;
    for (i = 0; i < 0x29; i++) {
        for (j = 0; j < 6; j++) {
            if (data_020a3768[i][j] != 0) {
                if (func_020231f4(gSaveData, 0, i, j) != 0) {
                    mUnk_1a2[j].mUnk_a += 1;
                    mUnk_1a2[6].mUnk_a += 1;
                }
            }
        }
    }
    for (u16 k = 1; k < 0xd5; k++) {
        if (GetCardNumber(k) < 0x2bc) {
            u8 sel = GetCardInfo(k, 1) + 7;
            mUnk_1a2[sel].mUnk_b += 1;
            mUnk_1a2[13].mUnk_b += 1;
            if (TestFlagBit(data_020d4946, k)) {
                mUnk_1a2[sel].mUnk_a += 1;
                mUnk_1a2[13].mUnk_a += 1;
            }
        }
    }
}

u8 ListScene::GetRatioFrame(u32 unk1) {
    s32 a = mUnk_1a2[unk1].mUnk_a;
    if (a == 0) {
        return 0;
    }
    s32 b = mUnk_1a2[unk1].mUnk_b;
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

void func_020834ec(ListScene *self, u32 mode) {
    u8 i;
    u8 flag  = 1;
    u8 state = 0;
    for (i = 0; i < 5; i++) {
        self->SetItemFlags(1, i, 0x8000);
    }
    state = 0;
    switch (mode) {
        case 0:
            if (self->mCommandList != NULL) {
                self->mCommandList->SetWindowVisible(1);
            }
            if (self->mCommandPage != NULL) {
                self->mCommandPage->SetWindowVisible(1);
            }
            self->mPendingPlanes                   = 0x1d;
            self->mUnk_4c->mEnabled = 1;
            self->mUnk_48->mEnabled = 0;
            state                      = 0;
            if (self->mUnk_1a1 == 10 || self->mUnk_1a1 == 6) {
                self->mUnk_186[self->mUnk_e6] = self->mCursorItem;
                self->BuildList(0);
                self->func_02081128();
                self->func_02081594();
                self->mUnk_194[2] = 1;
                self->mUnk_194[0] = 1;
            } else if ((u8) (self->mUnk_1a1 + 0xf8) <= 1) {
                for (state = 0; state < 9; state++) {
                    if (state < 3) {
                        self->LoadBakuganMark((u8) self->mUnk_c8[state], state, 0);
                    } else {
                        self->LoadSmallCard(self->mUnk_c8[state], (u8) (state - 3), 0);
                    }
                }
                self->mUnk_194[1] = 1;
                self->PrintRows();
            } else {
                if (self->mUnk_1a1 == 3) {
                    self->func_02081128();
                }
                self->PrintRows();
            }
            self->mUnk_1a1 = mode;
            self->mState  = 3;
            return;
        case 1:
        case 7:
        case 10:
            state = 0;
            break;
        case 2:
            state = 2;
            break;
        case 3: {
            BattleCellPair *pair = (BattleCellPair *) GetBakuganEntry(gSaveData, (u8) self->mItemIds[self->mCursorItem], self->mUnk_104);
            SetCommandListBakugan(self->mCommandList, (u8) self->mItemIds[self->mCursorItem], self->mUnk_104, -1);
            u32 value = func_020231f4(gSaveData, state, (u8) self->mItemIds[self->mCursorItem], self->mUnk_104);
            func_0205afb4(self->mCommandList, self->mUnk_104, value, pair);
            self->mCommandList->func_02058240(1);
            state = 1;
            flag  = 0;
            break;
        }
        case 8:
        case 9:
            state = 1;
            break;
        case 4:
        case 5:
            self->LoadBakuganMark((u8) self->mItemIds[self->mCursorItem], state, 3);
            self->LoadBakuganMark((u8) self->mItemIds[self->mCursorItem], 1, 3);
            break;
        case 6:
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
    self->mUnk_4c->mEnabled = 0;
    self->mUnk_48->Reset(-1);
    self->mUnk_48->mEnabled = 1;
    self->mUnk_48->mButtons[0].mEnabled = 1;
    switch (state) {
        case 0:
            self->SetItemFlags(1, 3, 1);
            self->mUnk_48->mCursor->mSelection = 3;
            self->mUnk_48->mCellAnims[3]->SetAnimation(1);
            self->mUnk_48->mCellAnims[0]->SetAnimation(9);
            break;
        case 1:
            self->SetItemFlags(1, 1, 1);
            self->SetItemFlags(1, 2, 2);
            if (flag != 0) {
                self->mUnk_48->mCursor->mSelection = 1;
                self->mUnk_48->mCellAnims[1]->SetAnimation(4);
            } else {
                self->mUnk_48->mCursor->mSelection = 2;
                self->mUnk_48->mCellAnims[2]->SetAnimation(7);
            }
            self->mUnk_48->mCellAnims[0]->SetAnimation(10);
            break;
        case 2:
            self->SetItemFlags(1, 1, 1);
            self->SetItemFlags(1, 2, 2);
            self->mUnk_48->mCursor->mSelection = 1;
            self->mUnk_48->mCellAnims[1]->SetAnimation(4);
            self->mUnk_48->mCellAnims[0]->SetAnimation(10);
            self->mUnk_48->mButtons[4].mEnabled = 1;
            break;
    }
    self->mUnk_1a1 = mode;
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
    switch (self->mUnk_1a1) {
        case 0:
            break;
        case 1:
            index = 3;
            base  = (u8 *) self->mStorecommentMessages;
            break;
        case 2: {
            u8 i;
            u32 msg;
            u32 value;
            s16 y     = 0;
            s16 x     = 0;
            i = 0;
            msg = 0;
            value = 0;
            for (; i < 3; i++) {
                y = 8;
                switch (i) {
                    case 0:
                        x     = 10;
                        msg   = 6;
                        value = gSaveData[0].mMoney;
                        break;
                    case 1:
                        value = self->mPrice;
                        x     = 0x28;
                        msg   = 7;
                        break;
                    case 2:
                        msg   = y;
                        value = gSaveData[0].mMoney - self->mPrice;
                        x     = 0x50;
                        break;
                }
                PrintMessageB(TextManager::Get(), (MessageFile *) self->mStorecommentMessages, msg, self->mTextBuffers[0], y, x);
                y += 0x80;
                TextManager::Get().PrintNumber(value, 7, 0, self->mTextBuffers[0], 0x18, 0x10, &y, &x, 0, 2, 0x44, 0, 1, 1, 1);
            }
            break;
        }
        case 3:
            index = 4;
            base  = (u8 *) self->mStorecommentMessages;
            break;
        case 4:
        case 5:
            base  = (u8 *) self->mStorecommentMessages;
            index = 9;
            TextManager::Get().mUse4bppFont = 1;
            PrintMessageF(TextManager::Get(), (MessageFile *) self->mBakuganNameMessages, self->mItemIds[self->mCursorItem], self->mTextBuffers[0], 0x1c, 0x38);
            TextManager::Get().mUse4bppFont = 0;
            break;
        case 6:
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
        case 8:
            base  = (u8 *) self->mDeckSelectMessages;
            index = 0x13;
            break;
        case 9:
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
    QueueUpload(SpriteManager::Get().Screen(1).mBgIndex, 4, 0, self->mTextBuffers[0], self->mTextBufferSizes[0], 0);
}

void ListScene::func_0208403c() {
    switch (mUnk_1a1) {
        case 0:
            return;
        case 4:
        case 5:
            DrawBakuganMark(mUnk_104, 0, 3, 0x23, 0x5a);
            break;
        case 6:
            DrawSmallCard(mUnk_104, 0, 3, 0x23, 0x5a);
            break;
    }
    SpriteManager::Get().Screen(1).AddOam(mGlyphBuffers[0], (u8) mGlyphCounts[0], 0x20, 0x28, 0, 0, 0xf, 0);
}

void func_0208415c(ListScene *self) {
    u8 id     = self->mItemIds[self->mCursorItem];
    int value = GetBakuganEntryIndex(gSaveData, 0, id, self->mUnk_104);
    func_020231c0(gSaveData, value, self->func_02086548(id, self->mUnk_104));
    UnkStruct_02023248 *entry = GetBakuganEntry(gSaveData, id, self->mUnk_104);
    u32 index                 = func_02086654(self, id, self->mUnk_104);
    entry->mUnk_00_0          = 0;
    entry->mPad               = 0;
    entry->mUnk_0             = data_020a35b8[index][0];
    entry->mUnk_12            = data_020a35b8[index][1];
    entry->mUnk_3             = data_020a35b8[index][2];
    entry->mUnk_6             = data_020a35b8[index][3];
    entry->mUnk_9             = data_020a35b8[index][4];
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
        SetSlotBakugan(gSaveData, 0, i, 0, (u8) mUnk_c8[i], (u8) mUnk_c8[9 + i]);
    }
    for (u8 i = 0; i < 3; i++) {
        gSaveData[0].mUnk_06[i] = mUnk_c8[3 + i];
    }
    for (u8 i = 0; i < 3; i++) {
        gSaveData[0].mUnk_0c[i] = mUnk_c8[6 + i];
    }
}

void func_020843ac(ListScene *self, u32 unk1) {
    SpriteScreen &screen = SpriteManager::Get().Screen(1);
    if (unk1 != 0) {
        ChangeDir("/Game/Battle/Bg/");
        LoadAndQueueUpload(1, 1, 0, "select_arena_b_bg_00_ncl.bin", 0, 0, 0);
        LoadAndQueueUpload(1, 0xb, 0, "select_arena_b_bg_00_ncg.bin", 0, 0, 0);
        LoadAndQueueUpload(1, 7, 0, "select_arena_b_bg_00_nsc.bin", 0, 0, 0);
        ChangeDir("/Game/Deck/");
        screen.LoadCharacters("select_deck_b_obj_01.nce_ncg1D.bin", 0x1c000, 0);
        screen.LoadPalette("select_deck_b_obj_01_ncl.bin", 0xc0, 0);
        ChangeDir("/Game/Battle/Obj/");
        screen.LoadCharacters("battle_b_obj_wait.nce_ncg1D.bin", 0x1e000, 0);
        screen.LoadPalette("battle_b_obj_ability_ncl.bin", 0x100, 0);
        GXS_SetVisiblePlane(0x11);
        self->mDeckWindow->mCursor->mSelection = 1;
        self->mDeckWindow->mCellAnims[1]->SetAnimation(4);
        self->SetItemFlags(3, 0, 8);
        self->SetItemFlags(3, 1, 8);
        self->SetItemFlags(0, 0, 0x8000);
        self->SetItemFlags(0, 1, 0x8000);
        self->SetItemFlags(0, 2, 0x8000);
        self->SetItemFlags(0, 3, 0x8000);
        self->SetItemFlags(0, 4, 0x8000);
        self->SetItemFlags(0, 5, 0x8000);
        self->SetItemFlags(0, 6, 0x8000);
        if (self->mUnk_e6 != 0) {
            if (self->mUnk_1a0 == 0) {
                self->mUnk_1a0 = 1;
            }
        }
        self->mUnk_e6 = 0;
        self->mUnk_e7 = 0;
        self->mUnk_e8 = 0;
        self->mUnk_ea = 0;
        self->mItemCount = 2;
        self->mCursorItem = 0;
        self->mUnk_fb = 0;
        self->mScroll = 0;
        if (self->mCharacterId != 0x15) {
            self->mItemCount = 0;
        }
        self->SetMessage(5);
        self->mWndMode = 0;
        self->ApplyWndMode();
    } else {
        ChangeDir("/Game/List/");
        LoadAndQueueUpload(1, 1, 0, "list_edit_b_bg_00_ncl.bin", 0, 0, 0);
        LoadAndQueueUpload(1, 1, 0x4000, "list_edit_b_bg_00_ncl.bin", 0, 0, 0);
        LoadAndQueueUpload(1, 1, 0x6000, "list_edit_b_bg_00_ncl.bin", 0, 0, 0);
        LoadAndQueueUpload(1, 0xb, 0, "list_edit_b_bg_00_ncg.bin", 0, 0, 0);
        LoadAndQueueUpload(1, 7, 0, "list_edit_b_bg_00_nsc.bin", 0, 0, 0);
        LoadAndQueueUpload(1, 9, 0, "list_edit_b_bg_01_nsc.bin", 0, 0, 0);
        LoadAndQueueUpload(1, 0xa, 0, "list_edit_b_bg_02_nsc.bin", 0, 0, 0);
        screen.LoadPalette("list_b_obj_00_ncl.bin", 0xc0, 0);
        ChangeDir("/Game/2d/union_b/");
        screen.LoadCharacters("union_b_obj_point.nce_ncg1D.bin", 0x1c000, 0);
        screen.LoadPalette("union_b_obj_point_ncl.bin", 0x60, 0);
        screen.LoadCharacters("union_b_obj_page.nce_ncg1D.bin", 0x1d000, 0);
        screen.LoadPalette("union_b_obj_page_ncl.bin", 0, 0);
        GXS_SetVisiblePlane(0x1d);
        self->SetItemFlags(3, 0, 0x8000);
        self->SetItemFlags(3, 1, 0x8000);
        self->mUnk_194[0] = 1;
        self->mUnk_194[1] = 1;
        self->mUnk_194[2] = 1;
        self->SetMessage(-1);
        self->mWndMode = 2;
    }
}

void func_020848c8(ListScene *self) {
    for (u8 i = 0; i < 3; i++) {
        BattleRecord *record = &data_020d433c.mBattlePlayers->mPlayers[0]->mRecords[i];
        record->mBakuganId      = self->mUnk_c8[i];
        record->mUnk_04      = func_020231f4(gSaveData, 0, (u8) self->mUnk_c8[i], (u8) self->mUnk_c8[i + 9]);
        record->mAttribute      = self->mUnk_c8[i + 9];
        record->mStatus      = 0;
        record->mUnk_0b      = record->mUnk_04 / 10;
        MI_CpuCopy16(GetBakuganEntry(gSaveData, record->mBakuganId, record->mAttribute), &data_020d433c.mBattlePlayers->mPlayers[0]->mRecords[i], 4);
    }
    for (u8 i = 0; i < 3; i++) {
        BattlePlayerSlot *slot = &data_020d433c.mBattlePlayers->mPlayers[0]->mSlots54[i];
        slot->mValue         = self->mUnk_c8[3 + i];
        slot->mFlag          = 0;
    }
    for (u8 i = 0; i < 3; i++) {
        BattlePlayerSlot *slot = &data_020d433c.mBattlePlayers->mPlayers[0]->mSlots6c[i];
        slot->mValue         = self->mUnk_c8[6 + i];
        slot->mFlag          = 0;
    }
}

BOOL func_02084a10(ListScene *self) {
    for (u8 i = 0; i < 3; i++) {
        if (self->mCharacterId != 0x15) {
            BattleRecord *record = &data_020d433c.mBattlePlayers->mPlayers[self->mPlayer]->mRecords[i];
            MESSAGE->mUnk_10[i]     = record->mBakuganId;
            MESSAGE->mUnk_04[i]     = record->mUnk_04;
            MESSAGE->mUnk_16[i]     = record->mAttribute;
        } else {
            MESSAGE->mUnk_10[i] = self->mUnk_c8[i];
            MESSAGE->mUnk_04[i] = func_020231f4(gSaveData, 0, (u8) self->mUnk_c8[i], (u8) self->mUnk_c8[i + 9]);
            MESSAGE->mUnk_16[i] = self->mUnk_c8[i + 9];
        }
    }
    BattlePlayerList *players = data_020d433c.mBattlePlayers;
    MESSAGE->mUnk_1c          = 3;
    if (SetMessageTypeAndCheckAll(players, 3)) {
    u32 count = WirelessManager::Get().func_0202f134();
    u8 j = 0;
    do {
        u8 length = ((BattleListMessage *) &data_020d433c.mBattlePlayers->mPlayers[j]->mMsg)->mUnk_1c;
        for (u8 k = 0; k < 6; k++) {
            BattleRecord *record = &data_020d433c.mBattlePlayers->mPlayers[j]->mRecords[k];
            if (k < length) {
                record->mBakuganId = ((BattleListMessage *) &data_020d433c.mBattlePlayers->mPlayers[j]->mMsg)->mUnk_10[k];
                record->mUnk_04 = ((BattleListMessage *) &data_020d433c.mBattlePlayers->mPlayers[j]->mMsg)->mUnk_04[k];
                record->mAttribute = ((BattleListMessage *) &data_020d433c.mBattlePlayers->mPlayers[j]->mMsg)->mUnk_16[k];
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

BOOL func_02084c38(ListScene *self) {
    for (u8 i = 0; i < 3; i++) {
        if (self->mCharacterId != 0x15) {
            SLOT_MESSAGE->mUnk_04[i] = data_020d433c.mBattlePlayers->mPlayers[self->mPlayer]->mSlots54[i].mValue;
        } else {
            SLOT_MESSAGE->mUnk_04[i] = self->mUnk_c8[3 + i];
        }
    }
    for (u8 i = 0; i < 3; i++) {
        if (self->mCharacterId != 0x15) {
            SLOT_MESSAGE->mUnk_10[i] = data_020d433c.mBattlePlayers->mPlayers[self->mPlayer]->mSlots6c[i].mValue;
        } else {
            SLOT_MESSAGE->mUnk_10[i] = self->mUnk_c8[6 + i];
        }
    }
    SLOT_MESSAGE->mUnk_1c = 3;
    SLOT_MESSAGE->mUnk_1d = 3;
    if (SetMessageTypeAndCheckAll(data_020d433c.mBattlePlayers, 4)) {
        u32 count = WirelessManager::Get().func_0202f134();
        u8 j      = 0;
        do {
            u8 length = ((BattleSlotMessage *) &data_020d433c.mBattlePlayers->mPlayers[j]->mMsg)->mUnk_1c;
            for (u8 k = 0; k < 6; k++) {
                BattlePlayer *player    = data_020d433c.mBattlePlayers->mPlayers[j];
                BattlePlayerSlot *slot = &player->mSlots54[k];
                if (k < length) {
                    slot->mValue = ((BattleSlotMessage *) &player->mMsg)->mUnk_04[k];
                    slot->mFlag  = 0;
                } else {
                    slot->mValue = 0;
                    slot->mFlag  = 3;
                }
            }
            u8 length2 = ((BattleSlotMessage *) &data_020d433c.mBattlePlayers->mPlayers[j]->mMsg)->mUnk_1d;
            for (u8 k = 0; k < 6; k++) {
                BattlePlayer *player    = data_020d433c.mBattlePlayers->mPlayers[j];
                BattlePlayerSlot *slot = &player->mSlots6c[k];
                if (k < length2) {
                    slot->mValue = ((BattleSlotMessage *) &player->mMsg)->mUnk_10[k];
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

BOOL func_02084e64(ListScene *self) {
    data_0220f540.mUnk_04 = MATH_Rand32(&gRandom, 0);
    if (SetMessageTypeAndCheckAll(data_020d433c.mBattlePlayers, 2)) {
        u8 *info = WirelessManager::Get().func_0202f0cc(0);
        if (info != NULL) {
            SeedRandom(*(u32 *) (info + 4));
        }
        return TRUE;
    }
    return FALSE;
}

BOOL func_02084f40(ListScene *self) {
    MIi_CpuCopyFast(gCharacterLook, (u8 *) &data_0220f540 + 4, 8);
    if (SetMessageTypeAndCheckAll(data_020d433c.mBattlePlayers, 6)) {
        u32 count = WirelessManager::Get().func_0202f134();
        u8 j      = 0;
        do {
            BattlePlayer *player = data_020d433c.mBattlePlayers->mPlayers[j];
            MIi_CpuCopyFast((u8 *) player + 0x104, (u8 *) player + 0xa8, 8);
            j++;
        } while (j < count);
        return TRUE;
    }
    return FALSE;
}

BOOL func_0208501c(ListScene *self) {
    for (u8 i = 0; i < 3; i++) {
        if (self->mCharacterId != 0x15) {
            MIi_CpuCopy16(&data_020d433c.mBattlePlayers->mPlayers[self->mPlayer]->mRecords[i], (u8 *) &data_0220f540 + 4 + i * 4, 4);
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
                MI_CpuClear16((u8 *) &data_0220f540 + 4 + i * 4, 4);
            } else {
                MIi_CpuCopy16(GetBakuganEntry(gSaveData, id, kind), (u8 *) &data_0220f540 + 4 + i * 4, 4);
            }
        }
    }
    if (SetMessageTypeAndCheckAll(data_020d433c.mBattlePlayers, 5)) {
        u32 count = WirelessManager::Get().func_0202f134();
        u8 j      = 0;
        do {
            u8 k = 0;
            do {
                BattlePlayer *player = data_020d433c.mBattlePlayers->mPlayers[j];
                MIi_CpuCopyFast((u8 *) player + 0x104 + k * 4, &player->mRecords[k], 4);
                k++;
            } while (k < 3);
            j++;
        } while (j < count);
        return TRUE;
    }
    return FALSE;
}

void ListScene::func_020851d0() {
    func_0207dea4();
    func_0207eec4();
    func_0208403c();
    if (mMessageWindow != NULL && mUnk_1a1 == 0) {
        mMessageWindow->Update();
    }
    if (mState < 0x17) {
        DrawWindows();
    }
}

void func_02085220(ListScene *self, u32 mode) {
    u8 i;
    u8 flag  = 1;
    u8 state = 0;
    for (i = 0; i < 5; i++) {
        self->SetItemFlags(1, i, 0x8000);
    }
    state = 0;
    switch (mode) {
        case 0:
            self->mPendingPlanes                      = 0x11;
            self->mDeckWindow->mEnabled             = 1;
            self->mDeckWindow->mCursor->mSelection = state;
            self->mDeckWindow->mCellAnims[0]->SetAnimation(1);
            self->SetItemFlags(3, state, 8);
            self->SetItemFlags(3, 1, 8);
            self->mUnk_48->mEnabled = state;
            self->mUnk_1a1          = mode;
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
    self->SetItemFlags(3, 0, 0x8000);
    self->SetItemFlags(3, 1, 0x8000);
    self->mUnk_48->Reset(-1);
    self->mUnk_48->mEnabled             = 1;
    self->mUnk_48->mButtons[0].mEnabled = 1;
    switch (state) {
        case 0:
            self->SetItemFlags(1, 3, 1);
            self->mUnk_48->mCursor->mSelection = 3;
            self->mUnk_48->mCellAnims[3]->SetAnimation(1);
            self->mUnk_48->mCellAnims[0]->SetAnimation(9);
            break;
        case 1:
            self->SetItemFlags(1, 1, 1);
            self->SetItemFlags(1, 2, 2);
            if (flag != 0) {
                self->mUnk_48->mCursor->mSelection = 1;
                self->mUnk_48->mCellAnims[1]->SetAnimation(4);
            } else {
                self->mUnk_48->mCursor->mSelection = 2;
                self->mUnk_48->mCellAnims[2]->SetAnimation(7);
            }
            self->mUnk_48->mCellAnims[0]->SetAnimation(10);
            break;
    }
    self->mUnk_1a1 = mode;
    PrintListSceneSubScreenText(self);
    self->mState = 0x14;
}

void ListScene::SetMessage(s32 unk1) {
    if (unk1 >= 0) {
        mMessageWindow->StartMessage(unk1, 0, 1);
        return;
    }
    switch (mMode) {
        case 0:
        case 1:
        case 2:
        case 4:
            break;
        case 3:
        case 5:
            if (mUnk_e6 == 0) {
                if (mUnk_f0 != 0) {
                    mMessageWindow->StartMessage(6, 0, 1);
                } else {
                    mMessageWindow->StartMessage((s16) (mCursorItem + 7), 0, 1);
                }
            } else {
                mMessageWindow->StartMessage((s16) (mUnk_e6 + 8), 0, 1);
            }
            break;
    }
}

void func_0208550c(ListScene *self) {
    u32 found = 0;
    u8 i;
    u8 j;
    self->mUnk_3f2 = 0;
    for (i = 0; i < 0x29; i++) {
        for (j = 0; j < 6; j++) {
            if (func_020231f4(gSaveData, 0, i, j) != 0) {
                if ((u8) (i + 0xdc) <= 1) {
                    if (j == gSaveData[0].mAttribute) {
                        self->mUnk_c8[0] = i;
                        found            = 1;
                        self->mUnk_c8[9] = j;
                    }
                } else {
                    self->mUnk_1f2[self->mUnk_3f2] = j + i * 6;
                    self->mUnk_3f2++;
                }
            }
        }
    }
    func_020867b8(self);
    u8 s;
    for (s = 0; s < self->mUnk_3f2 - 1; s++) {
        u8 t;
        for (t = s + 1; t < self->mUnk_3f2; t++) {
            u16 a = self->mUnk_1f2[s];
            u16 b = self->mUnk_1f2[t];
            u32 va = func_020231f4(gSaveData, 0, (u8) (a / 6), (u8) (a % 6));
            u32 vb = func_020231f4(gSaveData, 0, (u8) (b / 6), (u8) (b % 6));
            if (va < vb) {
                u16 tmp           = self->mUnk_1f2[s];
                self->mUnk_1f2[s] = self->mUnk_1f2[t];
                self->mUnk_1f2[t] = tmp;
            }
        }
    }
    if (found != 0) {
        self->mUnk_c8[1]  = self->mUnk_1f2[0] / 6;
        self->mUnk_c8[10] = self->mUnk_1f2[0] % 6;
        self->mUnk_c8[2]  = self->mUnk_1f2[1] / 6;
        self->mUnk_c8[11] = self->mUnk_1f2[1] % 6;
    } else {
        self->mUnk_c8[0]  = self->mUnk_1f2[0] / 6;
        self->mUnk_c8[9]  = self->mUnk_1f2[0] % 6;
        self->mUnk_c8[1]  = self->mUnk_1f2[1] / 6;
        self->mUnk_c8[10] = self->mUnk_1f2[1] % 6;
        self->mUnk_c8[2]  = self->mUnk_1f2[2] / 6;
        self->mUnk_c8[11] = self->mUnk_1f2[2] % 6;
    }
    func_0208671c(self, 0);
    func_020858d4(self);
    func_0208671c(self, 1);
    func_02086874(self);
    self->mUnk_c8[4] = self->mUnk_1f2[0];
    func_0208671c(self, 2);
    func_02086874(self);
    if (self->mUnk_1f2[0] == 0x50 && MATH_Rand32(&gRandom, 100) > 0x32) {
        self->mUnk_c8[5] = self->mUnk_1f2[1];
    } else {
        self->mUnk_c8[5] = self->mUnk_1f2[0];
    }
    func_0208671c(self, 3);
    func_02085990(self);
    func_0208671c(self, 4);
    func_02085b8c(self);
    func_0208671c(self, 5);
    func_02085fec(self);
}

void func_020858d4(ListScene *self) {
    u8 j;
    u8 i = 0;
    do {
        j = 0;
        do {
            if (func_0208596c(self, self->mUnk_1f2[j], (u8) self->mUnk_c8[i])) {
                self->mUnk_c8[3] = self->mUnk_1f2[j];
                return;
            }
            j++;
        } while (j < self->mUnk_3f2);
        i++;
    } while (i < 3 && self->mUnk_3f4 == 0);
    func_02086874(self);
    self->mUnk_c8[3] = self->mUnk_1f2[0];
}

BOOL func_0208596c(ListScene *self, u32 unk1, u32 unk2) {
    u8 value = unk1 - 1;
    if (unk1 >= 7) {
        value = value + 1;
    }
    return unk2 == value ? 1 : 0;
}

void func_02085990(ListScene *self) {
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
                if (wanted == GetCardInfo(self->mUnk_1f2[c], 3) && func_02085a7c(self, self->mUnk_1f2[c], (u8) self->mUnk_c8[9 + b])) {
                    self->mUnk_c8[6] = self->mUnk_1f2[c];
                    return;
                }
                c++;
            } while (c < self->mUnk_3f2);
            b++;
        } while (b < 3 && self->mUnk_3f4 == 0);
        a++;
    } while (a < 4);
}

BOOL func_02085a7c(ListScene *self, u32 unk1, u32 unk2) {
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

void func_02085b8c(ListScene *self) {
    u8 c;
    u8 a = 0;
    do {
        c = 0;
        do {
            if (GetCardInfo(self->mUnk_1f2[c], 3) == 5 && func_02085da8(self, self->mUnk_1f2[c], (u8) self->mUnk_c8[a], (u8) self->mUnk_c8[9 + a])) {
                self->mUnk_c8[7] = self->mUnk_1f2[c];
                return;
            }
            c++;
        } while (c < self->mUnk_3f2);
        a++;
    } while (a < 3 && self->mUnk_3f4 == 0);
    u8 k = 0;
    do {
        if (self->mUnk_1f2[k] == 0x8e) {
            self->mUnk_c8[7] = self->mUnk_1f2[k];
            return;
        }
        k++;
    } while (k < self->mUnk_3f2);
    k = 0;
    do {
        if (self->mUnk_1f2[k] == 0x8d) {
            self->mUnk_c8[7] = self->mUnk_1f2[k];
            return;
        }
        k++;
    } while (k < self->mUnk_3f2);
    k = 0;
    do {
        if ((u16) (self->mUnk_1f2[k] + 0xff71) <= 2) {
            self->mUnk_c8[7] = self->mUnk_1f2[k];
            return;
        }
        k++;
    } while (k < self->mUnk_3f2);
    k = 0;
    do {
        if (self->mUnk_1f2[k] == 0x8c) {
            self->mUnk_c8[7] = self->mUnk_1f2[k];
            return;
        }
        k++;
    } while (k < self->mUnk_3f2);
    u8 a2 = 0;
    do {
        u8 c2 = 0;
        do {
            if (GetCardInfo(self->mUnk_1f2[c2], 3) == 4 && func_02085da8(self, self->mUnk_1f2[c2], (u8) self->mUnk_c8[a2], (u8) self->mUnk_c8[9 + a2])) {
                self->mUnk_c8[7] = self->mUnk_1f2[c2];
                return;
            }
            c2++;
        } while (c2 < self->mUnk_3f2);
        a2++;
    } while (a2 < 3 && self->mUnk_3f4 == 0);
}

BOOL func_02085da8(ListScene *self, u32 unk1, u32 unk2, u32 unk3) {
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

void func_02085fec(ListScene *self) {
    func_02086874(self);
    u8 i = 0;
    do {
        if (func_02086044(self, self->mUnk_1f2[i])) {
            self->mUnk_c8[8] = self->mUnk_1f2[i];
            return;
        }
        i++;
    } while (i < self->mUnk_3f2);
}

BOOL func_02086044(ListScene *self, u32 unk1) {
    switch (unk1) {
        case 0xbe:
            if (self->mUnk_3f4 != 0) {
                return FALSE;
            }
            if (ListSceneHasAttribute(self, 0) && ListSceneHasAttribute(self, 4)) {
                return TRUE;
            }
            break;
        case 0xbf:
            if (self->mUnk_3f4 != 0) {
                return FALSE;
            }
            if (ListSceneHasAttribute(self, 5) && ListSceneHasAttribute(self, 3)) {
                return TRUE;
            }
            break;
        case 0xc0:
            if (self->mUnk_3f4 != 0) {
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
            if (self->mUnk_3f4 != 0) {
                return FALSE;
            }
            if (ListSceneHasAttribute(self, 0) && ListSceneHasAttribute(self, 1) && ListSceneHasAttribute(self, 3)) {
                return TRUE;
            }
            break;
        case 0xc8:
            if (self->mUnk_3f4 != 0) {
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
        if (unk1 == self->mUnk_c8[9 + i]) {
            return TRUE;
        }
        i++;
    } while (i < 3);
    return FALSE;
}

u32 ListScene::GetEntryPrice(u32 unk1, u32 unk2) {
    UnkStruct_02023248 *pair = GetBakuganEntry(gSaveData, unk1, unk2);
    if ((s32) pair->mUnk_00_0 >= 9) {
        return 10000000;
    }
    u32 cost = GetPrice(unk1, unk2);
    if (cost == 0) {
        cost = 5000;
    }
    u32 price = cost * sPricePercents[pair->mUnk_00_0] / 100;
    return price - price * pair->mPad / ((pair->mUnk_00_0 + 1) * 300);
}

BOOL ListScene::func_020862d0() {
    switch (mUnk_f5) {
        case 2:
            if (mUnk_e8 <= mUnk_f4) {
                return TRUE;
            }
            if (InputManager::Get().IsDown(KEY_LEFT) != 0) {
                return FALSE;
            }
            break;
        case 4:
            if (mUnk_e8 >= mUnk_e9 - 1) {
                return TRUE;
            }
            if (InputManager::Get().IsDown(KEY_RIGHT) != 0) {
                return FALSE;
            }
            break;
        case 6:
            if (mUnk_e8 <= mUnk_f4) {
                return TRUE;
            }
            if (mUnk_4c->mCursor->mUnk_05 == 1) {
                return FALSE;
            }
            break;
        case 8:
            if (mUnk_e8 >= mUnk_e9 - 1) {
                return TRUE;
            }
            if (mUnk_4c->mCursor->mUnk_05 == 2) {
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
            total += data_020d433c.mBattlePlayers->mPlayers[mPlayer]->mRecords[i].mUnk_04;
        } else {
            total += func_020231f4(gSaveData, 0, (u8) mUnk_c8[i], (u8) mUnk_c8[9 + i]);
        }
        i++;
    } while (i < 3 && mUnk_3f4 == 0);
    return total;
}

u32 ListScene::GetPrice(u32 unk1, u32 unk2) {
    u32 index = func_02086654(this, unk1, unk2);
    if (gSaveData[0].mUnk_5bf_0 == 0 && index == 0x24) {
        return Table_020a3860()[index].mPriceTens * 10;
    }
    return data_020a338c[index].mPriceTens * 10;
}

u32 ListScene::func_02086548(u32 unk1, u32 unk2) {
    u32 index = func_02086654(this, unk1, unk2);
    return (u16) (data_020a338c[index].mUnk_02 * 10);
}

u32 ListScene::func_02086570(u32 unk1, u32 unk2) {
    func_02086654(this, unk1, unk2);
    return 0;
}

void func_02086580(ListScene *self, u32 unk1, u32 unk2, BattleCellPair *out) {
    u32 index       = func_02086654(self, unk1, unk2);
    out->mUnk_00_0  = 0;
    out->mUnk_00_4  = 0;
    out->mUnk_02_0  = data_020a35b8[index][0];
    out->mUnk_02_12 = data_020a35b8[index][1];
    out->mUnk_02_3  = data_020a35b8[index][2];
    out->mUnk_02_6  = data_020a35b8[index][3];
    out->mUnk_02_9  = data_020a35b8[index][4];
}

u32 func_02086654(ListScene *self, u32 unk1, u32 unk2) {
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
                vb = ((const u8 *) data_020a3210.mOrder)[b];
                va = ((const u8 *) data_020a3210.mOrder)[a];
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

void func_0208671c(ListScene *self, u32 unk1) {
    self->mUnk_3f2 = 0;
    for (u16 k = 1; k < 0xd5; k++) {
        if (GetCardNumber(k) < 0x2bc && TestFlagBit(data_020d4946, k) && GetCardInfo(k, 1) == unk1) {
            self->mUnk_1f2[self->mUnk_3f2] = k;
            self->mUnk_3f2++;
        }
    }
    func_020867b8(self);
}

void func_020867b8(ListScene *self) {
    u8 i;
    for (i = 0; i < self->mUnk_3f2 - 1; i++) {
        u16 value = self->mUnk_1f2[i];
        u16 j     = i + MATH_Rand32(&gRandom, self->mUnk_3f2 - i);
        self->mUnk_1f2[i] = self->mUnk_1f2[j];
        self->mUnk_1f2[j] = value;
    }
}

void func_02086874(ListScene *self) {
    u8 s;
    u8 t;
    for (s = 0; s < self->mUnk_3f2 - 1; s++) {
        for (t = s + 1; t < self->mUnk_3f2; t++) {
            u32 va = GetCardAttributeValue(self->mUnk_1f2[s], (u8) gSaveData[0].mAttribute);
            u32 vb = GetCardAttributeValue(self->mUnk_1f2[t], (u8) gSaveData[0].mAttribute);
            if (va < vb) {
                u16 tmp           = self->mUnk_1f2[s];
                self->mUnk_1f2[s] = self->mUnk_1f2[t];
                self->mUnk_1f2[t] = tmp;
            }
        }
    }
}
