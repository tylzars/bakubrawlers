#include "Unknown/UnkStruct_02078e14.hpp"
#include "Unknown/UnkStruct_02071d24.hpp"
#include "Battle/BattleCommandList.hpp"
#include "Battle/BattleCommandPage.hpp"
#include "Game/UnkStruct_020d433c.hpp"
#include "Graphics/CellAnimation.hpp"
#include "UI/SelectWindow.hpp"
#include "Game/UnkStruct_020d43f0.hpp"
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
#include "Unknown/UnkStruct_02078290.hpp"
#include "System/WirelessManager.hpp"
#include "System/TouchManager.hpp"

// One object in the original: the cursor records (0x020a3210) and a byte table (0x020a3248) that follows them. As
// separate tables the size sort below would put the 0x29-byte table before the 0x38-byte ones.
struct UnkStruct_020a3210 {
    /* 00 */ WindowItemRecord mCursor[2];
    /* 38 */ u8 mOrder[0x29];
};

struct UnkStruct_020a338c {
    /* 0 */ u16 mUnk_00;
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

static const u16 data_020a3174[10] = {
    40, 80, 120, 160, 200, 240, 280, 320, 400, 0,
};

// Two 12-byte sprite blocks for SpriteScreen::func_02027778 (rows 0x020a3188 and 0x020a3194).
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
    static const u16 data_020a39fc[214] = {
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
    return data_020a39fc;
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

// Ten-byte records read as halfwords. As a flat byte array it gets a section of its own, so the table after it starts on
// a 4-byte boundary as in the original.
static const u8 data_020a3860[0x19a] = {
    0xd8, 0x0e, 0x2a, 0x00, 0x03, 0x01, 0x01, 0x00, 0x02, 0x00,
    0xe0, 0x15, 0x34, 0x00, 0x03, 0x02, 0x02, 0x00, 0x02, 0x00,
    0xac, 0x0d, 0x2a, 0x00, 0x02, 0x01, 0x00, 0x01, 0x02, 0x00,
    0x50, 0x14, 0x34, 0x00, 0x03, 0x02, 0x00, 0x01, 0x02, 0x00,
    0xac, 0x0d, 0x26, 0x00, 0x01, 0x01, 0x03, 0x02, 0x00, 0x00,
    0xc0, 0x12, 0x30, 0x00, 0x00, 0x01, 0x03, 0x02, 0x02, 0x00,
    0xc0, 0x12, 0x30, 0x00, 0x00, 0x01, 0x03, 0x02, 0x02, 0x00,
    0xd8, 0x0e, 0x2a, 0x00, 0x02, 0x03, 0x02, 0x00, 0x00, 0x00,
    0x50, 0x14, 0x34, 0x00, 0x03, 0x03, 0x02, 0x00, 0x00, 0x00,
    0xd8, 0x0e, 0x2a, 0x00, 0x01, 0x00, 0x02, 0x03, 0x01, 0x00,
    0xe0, 0x15, 0x34, 0x00, 0x02, 0x00, 0x03, 0x03, 0x01, 0x00,
    0x5c, 0x12, 0x2f, 0x00, 0x02, 0x02, 0x00, 0x03, 0x01, 0x00,
    0x70, 0x17, 0x39, 0x00, 0x03, 0x02, 0x01, 0x03, 0x01, 0x00,
    0x68, 0x10, 0x22, 0x00, 0x02, 0x01, 0x03, 0x02, 0x00, 0x00,
    0xde, 0x0d, 0x27, 0x02, 0x01, 0x03, 0x01, 0x02, 0x02, 0x00,
    0x0a, 0x0f, 0x27, 0x02, 0x01, 0x02, 0x03, 0x03, 0x01, 0x00,
    0x0a, 0x0f, 0x27, 0x02, 0x02, 0x03, 0x01, 0x01, 0x03, 0x00,
    0xde, 0x0d, 0x27, 0x02, 0x02, 0x02, 0x03, 0x00, 0x02, 0x00,
    0xde, 0x0d, 0x27, 0x02, 0x03, 0x00, 0x02, 0x03, 0x01, 0x00,
    0x78, 0x00, 0x14, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00, 0x00,
    0x78, 0x00, 0x14, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00,
    0x82, 0x00, 0x15, 0x00, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00,
    0x6e, 0x00, 0x13, 0x00, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00,
    0xaa, 0x00, 0x15, 0x00, 0x02, 0x00, 0x00, 0x00, 0x01, 0x00,
    0x40, 0x01, 0x17, 0x01, 0x01, 0x00, 0x00, 0x00, 0x01, 0x00,
    0xb0, 0x04, 0x1e, 0x03, 0x02, 0x01, 0x02, 0x01, 0x01, 0x00,
    0x1c, 0x0c, 0x25, 0x05, 0x03, 0x02, 0x01, 0x02, 0x02, 0x00,
    0x4a, 0x01, 0x18, 0x01, 0x00, 0x01, 0x00, 0x01, 0x00, 0x00,
    0xda, 0x02, 0x1b, 0x02, 0x02, 0x00, 0x00, 0x01, 0x01, 0x00,
    0x2a, 0x03, 0x1b, 0x02, 0x02, 0x02, 0x00, 0x00, 0x01, 0x00,
    0xe0, 0x06, 0x20, 0x04, 0x01, 0x03, 0x01, 0x01, 0x02, 0x00,
    0x90, 0x01, 0x19, 0x01, 0x01, 0x00, 0x01, 0x01, 0x00, 0x00,
    0x00, 0x00, 0x1e, 0x00, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00,
    0x00, 0x00, 0x1e, 0x00, 0x01, 0x00, 0x00, 0x01, 0x00, 0x00,
    0xd8, 0x09, 0x24, 0x04, 0x02, 0x02, 0x02, 0x02, 0x01, 0x00,
    0x28, 0x05, 0x1e, 0x03, 0x01, 0x00, 0x03, 0x02, 0x00, 0x00,
    0xe8, 0x03, 0x2a, 0x00, 0x02, 0x01, 0x01, 0x02, 0x01, 0x00,
    0x40, 0x1f, 0x3c, 0x00, 0x02, 0x01, 0x02, 0x03, 0x01, 0x00,
    0xfe, 0x10, 0x2a, 0x00, 0x01, 0x03, 0x00, 0x03, 0x01, 0x00,
    0x20, 0x1c, 0x3c, 0x00, 0x01, 0x03, 0x01, 0x03, 0x02, 0x00,
    0x00, 0x00, 0x29, 0x00, 0x03, 0x01, 0x01, 0x00, 0x03, 0x00,
};

#include "System/SoundManager.hpp"
#include "Text/TextManager.hpp"
#include "System/InputManager.hpp"
#include "Graphics/SpriteManager.hpp"

extern u8 data_020d43db[];
void operator delete(void *ptr);
extern u8 data_020d4946[];
extern u8 data_020d4404[];

class UnkStruct_02076058 {
public:
    /* 00 */ PAD(0x00, 0x78);
    /* 78 */ u8 mUnk_78;
    /* 79 */ PAD(0x79, 0x80);
    UnkStruct_02076058(u8 unk1);
    void func_02077a30(u32 a, u32 b, u32 c, u8 d);
};
BOOL func_02078270(UnkStruct_02076058 *obj);
u32 func_020779ec(void *obj);

void func_02081a4c(void *arg);

// Which screen a sprite or text goes to (passed to SpriteManager::Screen and the cell animation draws).
enum ScreenId { SCREEN_MAIN, SCREEN_SUB };


u8 func_020230dc(UnkStruct_020d43f0 *d, u32 a, u32 b);



extern u8 data_020a1074[];

static inline void PrintMessage(TextManager &tm, const u8 *data, u32 offset, u32 length, void *buffer, s16 x, s16 y) {
    tm.Print(data + offset, length, buffer, 0x18, 0x10, &y, &x, NULL, 0, 2, 0x44, 0x10, 0, 0, 0, 1, 1);
}


extern u8 data_020d4946[];

extern u8 data_020d4976[];

extern u8 data_020d497b[];

void func_02023134(UnkStruct_020d43f0 *d, u32 a, u32 b, u32 c, u32 e, u32 f);

void func_02086668(UnkStruct_02078e14 *self, u32 unk1);

void func_02086580(UnkStruct_02078e14 *self, u32 unk1, u32 unk2, BattleCellPair *out);

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

void func_02082efc(UnkStruct_02078e14 *self);

static inline void PrintMessageB(TextManager &tm, MessageFile *file, int index, void *buffer, s16 y, s16 x) {
    tm.Print(file->GetText(index), file->GetLength(index), buffer, 0x18, 0x10, &y, &x, NULL, 0, 2, 0x44, 0, 0, 0, 1, 1, 1);
}

void func_020839a0(UnkStruct_02078e14 *self);






u32 func_02086654(UnkStruct_02078e14 *self, u32 unk1, u32 unk2);

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

BOOL func_ov007_022631bc(BattlePlayerList *list, u32 id);

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

u8 func_020230dc(UnkStruct_020d43f0 *d, u32 a, u32 b);

void func_0208671c(UnkStruct_02078e14 *self, u32 unk1);

void func_020867b8(UnkStruct_02078e14 *self);

void func_020858d4(UnkStruct_02078e14 *self);

void func_02085990(UnkStruct_02078e14 *self);

void func_02085b8c(UnkStruct_02078e14 *self);

void func_02085fec(UnkStruct_02078e14 *self);

void func_02086874(UnkStruct_02078e14 *self);

BOOL func_02085a7c(UnkStruct_02078e14 *self, u32 unk1, u32 unk2);

BOOL func_02085da8(UnkStruct_02078e14 *self, u32 unk1, u32 unk2, u32 unk3);

BOOL func_02086044(UnkStruct_02078e14 *self, u32 unk1);

BOOL func_020861f4(UnkStruct_02078e14 *self, u32 unk1);








u8 func_02065bf4(u32 id, u32 index);

extern const u32 data_020a4040[][2];

extern const u8 *const data_020a4044[][2];

extern const u32 data_020a3fe8[][2];

extern const u16 *const data_020a3fec[][2];

void func_02081a4c(void *arg);
void func_02082efc(UnkStruct_02078e14 *self);
void func_020834ec(UnkStruct_02078e14 *self, u32 mode);
void func_020839a0(UnkStruct_02078e14 *self);
void func_0208415c(UnkStruct_02078e14 *self);
void func_020843ac(UnkStruct_02078e14 *self, u32 unk1);
void func_020848c8(UnkStruct_02078e14 *self);
BOOL func_02084a10(UnkStruct_02078e14 *self);
BOOL func_02084c38(UnkStruct_02078e14 *self);
BOOL func_02084e64(UnkStruct_02078e14 *self); // self is passed but unused
BOOL func_02084f40(UnkStruct_02078e14 *self); // self is passed but unused
BOOL func_0208501c(UnkStruct_02078e14 *self);
void func_02085220(UnkStruct_02078e14 *self, u32 mode);
void func_0208550c(UnkStruct_02078e14 *self);
void func_020858d4(UnkStruct_02078e14 *self);
BOOL func_0208596c(UnkStruct_02078e14 *self, u32 unk1, u32 unk2);
void func_02085990(UnkStruct_02078e14 *self);
BOOL func_02085a7c(UnkStruct_02078e14 *self, u32 unk1, u32 unk2);
void func_02085b8c(UnkStruct_02078e14 *self);
BOOL func_02085da8(UnkStruct_02078e14 *self, u32 unk1, u32 unk2, u32 unk3);
void func_02085fec(UnkStruct_02078e14 *self);
BOOL func_02086044(UnkStruct_02078e14 *self, u32 unk1);
BOOL func_020861f4(UnkStruct_02078e14 *self, u32 unk1);
void func_02086580(UnkStruct_02078e14 *self, u32 unk1, u32 unk2, BattleCellPair *out);
u32 func_02086654(UnkStruct_02078e14 *self, u32 unk1, u32 unk2);
void func_02086668(UnkStruct_02078e14 *self, u32 unk1);
void func_0208671c(UnkStruct_02078e14 *self, u32 unk1);
void func_020867b8(UnkStruct_02078e14 *self);
void func_02086874(UnkStruct_02078e14 *self);


UnkStruct_02078e14::UnkStruct_02078e14(u32 unk1, u32 unk2) :
    SceneBase(0x3e8, 0) {
    mUnk_0c   = NULL;
    mUnk_10   = NULL;
    mUnk_14   = NULL;
    mUnk_18   = NULL;
    mUnk_1c   = NULL;
    mUnk_20   = NULL;
    mUnk_48   = NULL;
    mUnk_4c   = NULL;
    mUnk_50   = NULL;
    mUnk_54   = NULL;
    mUnk_58   = NULL;
    mUnk_c0   = NULL;
    mUnk_c4   = NULL;
    mUnk_e0   = unk1;
    mUnk_e1   = 0;
    mUnk_e2   = 0;
    mUnk_1e0  = 0;
    mUnk_1e4  = 0;
    mUnk_1e8  = NULL;
    mUnk_1ec  = NULL;
    mUnk_1f0  = unk2;
    mUnk_3f4  = 0;
    mUnk_3f8  = 0;
    mUnk_3f9  = 0;
    mUnk_3fa  = 0;
    mUnk_3fc  = 0;
    mUnk_3fe  = 0;
    mUnk_3ff  = 0;
    mUnk_400  = 0;
    mUnk_401  = 0;
    mUnk_404  = -1;
    SpriteManager::Get().SetFade(1, -16);
    mUnk_1f1 = mUnk_e0 == 5 ? data_020d43db[mUnk_1f0] : 0x15;
    for (u8 i = 0; i < 2; i++) {
        mUnk_28[i] = NULL;
        mUnk_30[i] = NULL;
        mUnk_38[i] = NULL;
        mUnk_40[i] = NULL;
    }
    for (u8 i = 0; i < 6; i++) {
        mUnk_5c[i] = NULL;
    }
    for (u8 i = 0; i < 0x13; i++) {
        mUnk_74[i] = NULL;
    }
    mUnk_f4 = 0;
    if (mUnk_1f1 != 0x15) {
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
    mUnk_3f9 = 2;
    mUnk_f8  = 0;
    mUnk_fa  = 0;
    mUnk_fb  = 0;
    mUnk_fc  = 0;
    mUnk_fd  = 0;
    mUnk_100 = 0;
    mUnk_104 = 0;
    for (u8 i = 0; i < 0x40; i++) {
        mUnk_106[i] = 0;
    }
    for (u8 i = 0; i < 13; i++) {
        mUnk_186[i] = 0;
    }
    for (u8 i = 0; i < 3; i++) {
        mUnk_194[i] = 1;
    }
    mUnk_1a0 = 4;
    mUnk_1a1 = 0;
    mUnk_1dc = 0;
    MI_CpuClear16(&mUnk_24, 2);
    mUnk_3f4 = data_020d433c.mUnk_8a_0;
}

UnkStruct_02078e14::~UnkStruct_02078e14() {
    u8 i;
    for (i = 0; i < 6; i++) {
        delete mUnk_5c[i];
    }
    for (i = 0; i < 0x13; i++) {
        delete mUnk_74[i];
    }
    delete mUnk_4c;
    delete mUnk_48;
    delete mUnk_50;
    operator delete(mUnk_54);
    operator delete(mUnk_58);
    if (mUnk_0c != NULL) {
        OS_Free(mUnk_0c);
        mUnk_0c = NULL;
    }
    if (mUnk_10 != NULL) {
        OS_Free(mUnk_10);
        mUnk_10 = NULL;
    }
    if (mUnk_14 != NULL) {
        OS_Free(mUnk_14);
        mUnk_14 = NULL;
    }
    if (mUnk_18 != NULL) {
        OS_Free(mUnk_18);
        mUnk_18 = NULL;
    }
    if (mUnk_1c != NULL) {
        OS_Free(mUnk_1c);
        mUnk_1c = NULL;
    }
    if (mUnk_20 != NULL) {
        OS_Free(mUnk_20);
        mUnk_20 = NULL;
    }
    for (u8 i = 0; i < 2; i++) {
        if (mUnk_28[i] != NULL) {
            OS_Free(mUnk_28[i]);
            mUnk_28[i] = NULL;
        }
        if (mUnk_30[i] != NULL) {
            OS_Free(mUnk_30[i]);
            mUnk_30[i] = NULL;
        }
    }
    SpriteManager::Get().SetFade(1, -16);
    if (data_020d433c.mUnk_a8 != 0) {
        mUnk_1e8->mUnk_04 = 1;
    }
    delete mUnk_1ec;
    mUnk_3f9 = 0;
    func_0207ed3c();
}

void UnkStruct_02078e14::func_0207966c(u32 unk1) {
    for (u8 i = 0; i < 6; i++) {
        delete mUnk_5c[i];
        mUnk_5c[i] = NULL;
    }
    for (u8 i = 0; i < 0x13; i++) {
        delete mUnk_74[i];
        mUnk_74[i] = NULL;
    }
    delete mUnk_4c;
    mUnk_4c = NULL;
    delete mUnk_48;
    mUnk_48 = NULL;
    delete mUnk_50;
    mUnk_50 = NULL;
    operator delete(mUnk_54);
    mUnk_54 = NULL;
    operator delete(mUnk_58);
    mUnk_58 = NULL;
    if (mUnk_0c != NULL) {
        OS_Free(mUnk_0c);
        mUnk_0c = NULL;
    }
    if (mUnk_10 != NULL) {
        OS_Free(mUnk_10);
        mUnk_10 = NULL;
    }
    if (mUnk_14 != NULL) {
        OS_Free(mUnk_14);
        mUnk_14 = NULL;
    }
    if (mUnk_18 != NULL) {
        OS_Free(mUnk_18);
        mUnk_18 = NULL;
    }
    if (mUnk_1c != NULL) {
        OS_Free(mUnk_1c);
        mUnk_1c = NULL;
    }
    if (mUnk_20 != NULL) {
        OS_Free(mUnk_20);
        mUnk_20 = NULL;
    }
    for (u8 i = 0; i < 2; i++) {
        if (mUnk_28[i] != NULL) {
            OS_Free(mUnk_28[i]);
            mUnk_28[i] = NULL;
        }
        if (mUnk_30[i] != NULL) {
            OS_Free(mUnk_30[i]);
            mUnk_30[i] = NULL;
        }
    }
    if (data_020d433c.mUnk_a8 != 0) {
        mUnk_1e8->mUnk_04 = 1;
    }
    delete mUnk_1ec;
    mUnk_1ec = NULL;
    if (unk1 != 0) {
        if (mUnk_400 == 0 && mUnk_401 == 0) {
            mContext->mUnk_12 = 1;
        }
        if (mUnk_c0 != NULL && mUnk_400 == 0) {
            func_0205821c(mUnk_c0);
            mUnk_c0           = NULL;
            mContext->mUnk_12 = 0;
        }
        if (mUnk_c4 != NULL && mUnk_401 == 0) {
            func_02055624(mUnk_c4, 1);
            mUnk_c4           = NULL;
            mContext->mUnk_12 = 0;
        }
    }
    mUnk_3f9 = 0;
    func_0207ed3c();
}

#define UPDATE_TAIL()                      \
    func_0207eec4();                       \
    mUnk_74[18]->Update();                 \
    mUnk_74[18]->Draw(1, 0x80, 0x60, 0);   \
    return

void UnkStruct_02078e14::Update() {
    if (mUnk_404 != -1) {
        GXS_SetVisiblePlane(mUnk_404);
        mUnk_404 = -1;
        if (mUnk_e1 == 3) {
            if (mUnk_f0 != 0) {
                mUnk_3f9 = 2;
            } else {
                mUnk_3f9 = 1;
            }
        }
    }
    if (mUnk_3fa != 0) {
        mUnk_3fa--;
        if (mUnk_3fa == 0 && mUnk_4c != NULL) {
            mUnk_4c->mCells[0]->SetAnimation(2);
        }
    }
    if (mUnk_3ff != 0 && mUnk_3fc == 0) {
        SoundManager::Get().func_020246f8(0x66);
        mUnk_3ff = 0;
        u32 one = 1;
        if (mUnk_e6 < 4) {
            mUnk_c8[mUnk_e6 - 1] = mUnk_106[mUnk_fa];
            mUnk_c8[mUnk_e6 + 8] = mUnk_104;
            mUnk_f0              = one;
            mUnk_3f9             = 2;
            mUnk_186[mUnk_e6]    = mUnk_fa;
            mUnk_194[0]          = one;
            mUnk_194[1]          = one;
            mUnk_194[2]          = one;
            func_02081128();
            data_020d433c.mUnk_14 |= 2;
        } else {
            mUnk_c8[mUnk_e6 - 1] = mUnk_106[mUnk_fa];
            mUnk_f0              = one;
            mUnk_3f9             = 2;
            mUnk_186[mUnk_e6]    = mUnk_fa;
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
    switch (mUnk_e1) {
        case 0:
            func_0207b3a8();
            func_0207b6e8();
            func_0207b760();
            func_0207c568();
            mUnk_e1++;
            if (mUnk_e0 == 5) {
                mUnk_e1 = 7;
                func_020851d0();
                return;
            }
            break;
        case 1:
            mUnk_48->mUnk_064 = 0;
            mUnk_4c->mUnk_064 = 0;
            if (!func_020842a0()) {
                SpriteManager::Get().StartFade(2);
                mUnk_e1++;
            }
            break;
        case 2:
            if (!SpriteManager::Get().IsFading()) {
                mUnk_4c->mUnk_064 = 1;
                mUnk_e1++;
            }
            break;
        case 3:
            func_0207cfa4();
            break;
        case 4:
            if (!func_020842a0()) {
                switch (mUnk_48->Update(1)) {
                    case 1:
                        mUnk_e1 = 5;
                        break;
                    case 2:
                        mUnk_e1 = 6;
                        break;
                }
            }
            break;
        case 5:
            mUnk_48->Update(1);
            if (mUnk_48->mUnk_061 == 4) {
                switch (mUnk_1a1) {
                    case 2:
                        switch (mUnk_e0) {
                            case 0:
                                data_020d43f0[0].mMoney -= mUnk_1dc;
                                func_020834ec(this, 4);
                                break;
                            case 1:
                                data_020d43f0[0].mMoney -= mUnk_1dc;
                                func_020834ec(this, 0);
                                mUnk_186[mUnk_e6] = mUnk_fa;
                                mUnk_e1           = 0x21;
                                break;
                            case 2:
                                data_020d43f0[0].mMoney -= mUnk_1dc;
                                func_020834ec(this, 6);
                                break;
                        }
                        SoundManager::Get().func_020246f8(0x69);
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
                        mUnk_e1 = 0x26;
                        break;
                    case 6:
                        SetFlagBit(data_020d4946, mUnk_106[mUnk_fa]);
                        func_020834ec(this, 0);
                        break;
                    case 8:
                        func_02080210();
                        func_020834ec(this, 0);
                        SoundManager::Get().func_020246f8(0x68);
                        break;
                    case 9:
                        func_0208550c(this);
                        data_020d433c.mUnk_14 |= 2;
                        func_020834ec(this, 0);
                        SoundManager::Get().func_020246f8(0x67);
                        break;
                    default:
                        func_020834ec(this, 0);
                        break;
                }
            }
            break;
        case 6:
            mUnk_48->Update(1);
            if (mUnk_48->mUnk_061 == 4) {
                func_020834ec(this, 0);
            }
            break;
        case 7:
            mUnk_48->mUnk_064 = 0;
            mUnk_4c->mUnk_064 = 0;
            mUnk_50->mUnk_064 = 0;
            SpriteManager::Get().StartFade(2);
            mUnk_e1++;
            func_020851d0();
            return;
        case 8:
            if (!SpriteManager::Get().IsFading()) {
                mUnk_50->mUnk_064 = 1;
                mUnk_e1++;
                if (mUnk_3f4 != 0) {
                    func_02085220(this, 0xc);
                }
            }
            func_020851d0();
            return;
        case 9:
            if (!func_020842a0()) {
                if (mUnk_50->Update(1) == 3) {
                    switch (mUnk_50->mCursor->mSelection) {
                        case 0:
                            func_020842e8();
                            func_02085220(this, 0xb);
                            break;
                        case 1:
                            mUnk_e6 = mUnk_f4;
                            mUnk_e7 = mUnk_f4;
                            mUnk_e8 = mUnk_f4;
                            mUnk_ea = mUnk_f4;
                            if (mUnk_1f1 != 0x15) {
                                mUnk_1a0 = 1;
                            }
                            mUnk_e1++;
                            break;
                    }
                }
            }
            func_020851d0();
            return;
        case 10:
            SpriteManager::Get().StartFade(3);
            mUnk_50->mUnk_064 = 0;
            mUnk_e1++;
            func_020851d0();
            return;
        case 11:
            if (!SpriteManager::Get().IsFading()) {
                mUnk_e1++;
            }
            func_020851d0();
            return;
        case 12:
            if (func_020842a0()) {
                return;
            }
            func_020843ac(this, 0);
            mUnk_e1++;
            break;
        case 13:
            if (!func_020842a0()) {
                SpriteManager::Get().StartFade(2);
                mUnk_e1++;
            }
            break;
        case 14:
            if (!SpriteManager::Get().IsFading()) {
                mUnk_4c->mUnk_064 = 1;
                mUnk_e1           = 3;
            }
            break;
        case 15:
            SpriteManager::Get().StartFade(3);
            mUnk_4c->mUnk_064 = 0;
            mUnk_e1++;
            break;
        case 16:
            if (!SpriteManager::Get().IsFading()) {
                mUnk_e1++;
            }
            break;
        case 17:
            if (func_020842a0()) {
                return;
            }
            func_020843ac(this, 1);
            mUnk_e1++;
            func_020851d0();
            return;
        case 18:
            if (!func_020842a0() && mUnk_1a0 == 0) {
                SpriteManager::Get().StartFade(2);
                mUnk_e1++;
            }
            func_020851d0();
            return;
        case 19:
            if (!SpriteManager::Get().IsFading()) {
                mUnk_50->mUnk_064 = 1;
                mUnk_e1           = 9;
            }
            func_020851d0();
            return;
        case 20: {
            switch (mUnk_48->Update(1)) {
                case 1:
                    mUnk_e1 = 0x15;
                    break;
                case 2:
                    mUnk_e1 = 0x16;
                    break;
            }
            func_020851d0();
            return;
        }
        case 21:
            mUnk_48->Update(1);
            if (mUnk_48->mUnk_061 == 4) {
                switch (mUnk_1a1) {
                    case 0xb:
                        SoundManager::Get().func_020246f8(0x6a);
                        func_02085220(this, 0);
                        mUnk_50->mCells[0]->SetAnimation(0);
                        mUnk_e1 = 0x17;
                        WirelessManager::Get();
                        if (IsWirelessMode(&data_020c4030.mUnk_228)) {
                            mUnk_50->mUnk_064 = 0;
                            mUnk_e1           = 0x1b;
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
            if (mUnk_48->mUnk_061 == 4) {
                func_02085220(this, 0);
            }
            func_020851d0();
            return;
        case 23:
            mUnk_50->mUnk_064 = 0;
            if (mUnk_1f1 == 0x15) {
                func_020848c8(this);
            }
            MIi_CpuCopyFast(data_020d4404, (u8 *) ((u32 *) data_020d433c.mBattlePlayers)[3] + 0xa8, 8);
            SpriteManager::Get().StartFade(3);
            mUnk_e1++;
            func_020851d0();
            return;
        case 24:
            if (!SpriteManager::Get().IsFading()) {
                func_0207966c(1);
                mUnk_e1++;
                return;
            }
            func_020851d0();
            return;
        case 25:
            if (mContext->mUnk_12 != 0) {
                mUnk_e1++;
            }
            return;
        case 26:
            RequestScene(1);
            mFinished = 1;
            return;
        case 27:
            if (func_02084a10(this)) {
                mUnk_e1++;
            }
            UPDATE_TAIL();
        case 28:
            if (func_02084c38(this)) {
                mUnk_e1++;
            }
            UPDATE_TAIL();
        case 29:
            if (func_0208501c(this)) {
                mUnk_e1++;
            }
            UPDATE_TAIL();
        case 30:
            if (func_02084f40(this)) {
                mUnk_e1++;
            }
            UPDATE_TAIL();
        case 31:
            if (func_02084e64(this)) {
                SpriteManager::Get().StartFade(3);
                mUnk_e1++;
            }
            UPDATE_TAIL();
        case 32:
            if (!SpriteManager::Get().IsFading()) {
                func_0207966c(1);
                mUnk_e1 = 0x19;
                return;
            }
            UPDATE_TAIL();
        case 33:
            SpriteManager::Get().StartSubFade(3);
            mUnk_e1++;
            break;
        case 34:
            SpriteManager::Get();
            if (SpriteManager::sFade[1] <= 1) {
                if (mUnk_c0 != NULL) {
                    mUnk_c0->func_02058530(0);
                }
                func_0207966c(0);
                mUnk_1e0 = new UnkStruct_02076058(1);
                mUnk_1e0->func_02077a30(1, mUnk_106[mUnk_fa] & 0xff, mUnk_104, 1);
                mContext->mUnk_12 = 0;
                mUnk_e1++;
                return;
            }
            break;
        case 35:
            if (func_02078270(mUnk_1e0)) {
                UnkStruct_02023248 *pair = func_02023248(&data_020d43f0[0], mUnk_106[mUnk_fa] & 0xff, mUnk_104);
                u16 val = func_020231f4(&data_020d43f0[0], 0, mUnk_106[mUnk_fa] & 0xff, mUnk_104);
                func_0205afb4(mUnk_c0, mUnk_104, val, (BattleCellPair *) pair);
            }
            if (func_020779ec(mUnk_1e0) == 0) {
                return;
            }
            if (mUnk_c0 != NULL) {
                func_0205821c(mUnk_c0);
            }
            mUnk_e1++;
            return;
        case 36:
            if (mUnk_c0 != NULL) {
                if (mContext->mUnk_12 == 0) {
                    return;
                }
                mUnk_1e0->mUnk_78 = 1;
                mUnk_c0           = NULL;
            }
            mUnk_e1++;
            return;
        case 37:
            if (mContext->mUnk_12 != 3) {
                return;
            }
            mContext->mUnk_12 = 0;
            mUnk_1e0          = NULL;
            mUnk_e1           = 0;
            mUnk_f0           = 0;
            mUnk_1a0          = 4;
            mUnk_194[0]       = 1;
            mUnk_194[1]       = 1;
            mUnk_194[2]       = 1;
            mUnk_3f9          = 1;
            return;
        case 38:
            SpriteManager::Get().StartFade(3);
            mUnk_e1++;
            break;
        case 39:
            if (!SpriteManager::Get().IsFading()) {
                func_0207966c(1);
                mUnk_e1++;
                return;
            }
            break;
        case 40:
            if (mContext->mUnk_12 == 0) {
                return;
            }
            mUnk_1e4 = new UnkStruct_02071d24(2);
            mUnk_1e4->func_02074148(0, mUnk_106[mUnk_fa] & 0xff, mUnk_104, 1);
            mContext->mUnk_12 = 0;
            mUnk_e1++;
            return;
        case 41:
            if (mContext->mUnk_12 != 2) {
                return;
            }
            mContext->mUnk_12 = 0;
            mUnk_1e4          = NULL;
            mUnk_e1           = 0;
            mUnk_f0           = 1;
            mUnk_1a0          = 4;
            mUnk_194[0]       = 1;
            mUnk_194[1]       = 1;
            mUnk_194[2]       = 1;
            mUnk_3f9          = 2;
            return;
        case 43:
            if (mUnk_e0 == 3 || mUnk_e0 == 5) {
                func_020842e8();
            }
            SpriteManager::Get().StartFade(3);
            mUnk_48->mUnk_064 = 0;
            mUnk_4c->mUnk_064 = 0;
            mUnk_e1++;
            break;
        case 44:
            if (!SpriteManager::Get().IsFading()) {
                func_0207966c(1);
                mUnk_e1++;
                return;
            }
            break;
        case 45:
            if (mContext->mUnk_12 != 0) {
                mUnk_e1++;
            }
            return;
        case 46:
            RequestScene(1);
            mFinished = 1;
            return;
    }
    func_0207ed3c();
    func_0207dea4();
    func_0207e074();
    func_0207e424();
    func_0207ea3c();
    func_0207eec4();
    func_0208403c();
    func_0207ceb0();
    func_02080104();
    func_0207f2f0();
    func_0207f32c();
    func_0207f90c();
}


void UnkStruct_02078e14::func_0207b3a8() {
    GX_SetDispSelect(1);
    SpriteManager::Get().func_02026dec(1, 1);
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
    if (mUnk_e0 == 4) {
        GXS_SetVisiblePlane(0x19);
    } else if (mUnk_e0 == 5) {
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

void UnkStruct_02078e14::func_0207b6e8() {
    switch (mUnk_e0) {
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
            func_020832bc();
            break;
    }
    func_02080210();
    func_02080bdc(1);
}


void UnkStruct_02078e14::func_0207b760() {
    ChangeDir("/Game/2d/union_b/");
    func_02025b64(1, 1, 0x2000, "union_b_bg_window_ncl.bin", 0, 0, 0);
    func_02025b64(1, 0xc, 0, "union_b_bg_window_ncg.bin", 0, 0, 0);
    func_02025b64(1, 8, 0, "union_b_bg_window_b_nsc.bin", 0, 0, 0);
    switch (mUnk_e0) {
        case 0:
        case 1:
        case 2:
            ChangeDir("/Game/List/");
            func_02025b64(1, 1, 0, "list_store_b_bg_00_ncl.bin", 0, 0, 0);
            func_02025b64(1, 1, 0x4000, "list_store_b_bg_00_ncl.bin", 0, 0, 0);
            func_02025b64(1, 1, 0x6000, "list_store_b_bg_00_ncl.bin", 0, 0, 0);
            func_02025b64(1, 0xb, 0, "list_store_b_bg_00_ncg.bin", 0, 0, 0);
            func_02025b64(1, 7, 0, "list_store_b_bg_00_nsc.bin", 0, 0, 0);
            func_02025b64(1, 9, 0, "list_store_b_bg_01_nsc.bin", 0, 0, 0);
            func_02025b64(1, 10, 0, "list_store_b_bg_02_nsc.bin", 0, 0, 0);
            break;
        case 3:
            ChangeDir("/Game/List/");
            func_02025b64(1, 1, 0, "list_edit_b_bg_00_ncl.bin", 0, 0, 0);
            func_02025b64(1, 1, 0x4000, "list_edit_b_bg_00_ncl.bin", 0, 0, 0);
            func_02025b64(1, 1, 0x6000, "list_edit_b_bg_00_ncl.bin", 0, 0, 0);
            func_02025b64(1, 0xb, 0, "list_edit_b_bg_00_ncg.bin", 0, 0, 0);
            func_02025b64(1, 7, 0, "list_edit_b_bg_00_nsc.bin", 0, 0, 0);
            func_02025b64(1, 9, 0, "list_edit_b_bg_01_nsc.bin", 0, 0, 0);
            func_02025b64(1, 10, 0, "list_edit_b_bg_02_nsc.bin", 0, 0, 0);
            break;
        case 4:
            ChangeDir("/Game/List/");
            func_02025b64(1, 1, 0, "list_gallery_b_bg_00_ncl.bin", 0, 0, 0);
            func_02025b64(1, 1, 0x4000, "list_gallery_b_bg_00_ncl.bin", 0, 0, 0);
            func_02025b64(1, 1, 0x6000, "list_gallery_b_bg_00_ncl.bin", 0, 0, 0);
            func_02025b64(1, 0xb, 0, "list_gallery_b_bg_00_ncg.bin", 0, 0, 0);
            func_02025b64(1, 7, 0, "list_gallery_b_bg_00_nsc.bin", 0, 0, 0);
            func_02025b64(1, 9, 0, "list_gallery_b_bg_01_nsc.bin", 0, 0, 0);
            func_02025b64(1, 10, 0, "list_gallery_b_bg_02_nsc.bin", 0, 0, 0);
            break;
        case 5:
            ChangeDir("/Game/Battle/Bg/");
            func_02025b64(1, 1, 0, "select_arena_b_bg_00_ncl.bin", 0, 0, 0);
            func_02025b64(1, 0xb, 0, "select_arena_b_bg_00_ncg.bin", 0, 0, 0);
            func_02025b64(1, 7, 0, "select_arena_b_bg_00_nsc.bin", 0, 0, 0);
            break;
    }
    SpriteScreen &screen = SpriteManager::Get().Screen(1);
    screen.func_02027624(3);
    ChangeDir("/Game/List/");
    screen.LoadCharacters("list_b_obj_00.nce_ncg1D.bin", 0xc000, 0);
    screen.LoadPalette("list_b_obj_00_ncl.bin", 0xc0, 0);
    for (u8 i = 0; i <= 12; i++) {
        mUnk_74[i]            = new CellAnimation("list_b_obj_00_nce.bin", 0, 1, 1);
        mUnk_74[i]->mCharBase = screen.func_020276f4(0xc000);
        mUnk_74[i]->mUnk_11   = 6;
    }
    mUnk_74[0]->SetAnimation(0x23);
    mUnk_74[1]->SetAnimation(0x24);
    mUnk_74[2]->SetAnimation(0x22);
    mUnk_74[3]->SetAnimation(0);
    mUnk_74[4]->SetAnimation(1);
    mUnk_74[5]->SetAnimation(2);
    mUnk_74[6]->SetAnimation(3);
    mUnk_74[7]->SetAnimation(0x10);
    mUnk_74[8]->SetAnimation(0x10);
    mUnk_74[9]->SetAnimation(0x1c);
    mUnk_74[10]->SetAnimation(0x1d);
    mUnk_74[11]->SetAnimation(0x1e);
    mUnk_74[12]->SetAnimation(0x1f);
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
    mUnk_74[13]            = new CellAnimation("union_b_obj_number_nce.bin", 0, 1, 1);
    mUnk_74[13]->mCharBase = screen.func_020276f4(0x9000);
    mUnk_74[13]->mUnk_11   = 3;
    mUnk_74[13]->SetAnimation(0);
    mUnk_74[15]            = new CellAnimation("union_b_obj_new_nce.bin", 0, 1, 1);
    mUnk_74[15]->mCharBase = screen.func_020276f4(0xb000);
    mUnk_74[15]->mUnk_11   = 5;
    mUnk_74[15]->SetAnimation(0);
    mUnk_74[16]            = new CellAnimation("union_b_obj_gp_nce.bin", 0, 1, 1);
    mUnk_74[16]->mCharBase = screen.func_020276f4(0xa000);
    mUnk_74[16]->mUnk_11   = 4;
    mUnk_74[16]->SetAnimation(2);
    mUnk_74[17]            = new CellAnimation("union_b_obj_point_nce.bin", 0, 1, 1);
    mUnk_74[17]->mCharBase = screen.func_020276f4(0x1c000);
    mUnk_74[17]->mUnk_11   = 3;
    mUnk_74[17]->SetAnimation(0);
    ChangeDir("/");
    mUnk_4c = new SelectWindow(7, data_020a3430, -1);
    mUnk_4c->SetCells(7, data_020a34f4);
    mUnk_48 = new SelectWindow(5, data_020a3300, 1);
    mUnk_48->SetCells(5, data_020a3274);
    mUnk_54 = new SelectWindowCursor(8, data_020a3688, -1);
    mUnk_58 = new SelectWindowCursor(2, data_020a3210.mCursor, -1);
    if (mUnk_e0 == 5) {
        ChangeDir("/Game/Deck/");
        screen.LoadCharacters("select_deck_b_obj_01.nce_ncg1D.bin", 0x1c000, 0);
        screen.LoadPalette("select_deck_b_obj_01_ncl.bin", 0xc0, 0);
        ChangeDir("/Game/Battle/Obj/");
        screen.LoadCharacters("battle_b_obj_wait.nce_ncg1D.bin", 0x1e000, 0);
        screen.LoadPalette("battle_b_obj_ability_ncl.bin", 0x100, 0);
        mUnk_74[18]            = new CellAnimation("battle_b_obj_wait_nce.bin", 0, 1, 1);
        mUnk_74[18]->mCharBase = screen.func_020276f4(0x1e000);
        mUnk_74[18]->mUnk_11   = 8;
        mUnk_74[18]->SetAnimation(0);
        ChangeDir("/");
        mUnk_50 = new SelectWindow(2, data_020a31a0, 0);
        mUnk_50->SetCells(2, data_020a31d8);
        mUnk_50->mCells[0]->SetAnimation(1);
        this->func_02081994(3, 0, 8);
        this->func_02081994(3, 1, 8);
        this->func_02081994(0, 0, 0x8000);
        this->func_02081994(0, 1, 0x8000);
        this->func_02081994(0, 2, 0x8000);
        this->func_02081994(0, 3, 0x8000);
        this->func_02081994(0, 4, 0x8000);
        this->func_02081994(0, 5, 0x8000);
        this->func_02081994(0, 6, 0x8000);
    }
    ChangeDir("/Game/Bakugan/mark");
    func_02025b64(1, 0, 0x200, "bakugan_s_000_ncl.bin", 0, 0, 0);
    func_02025b64(1, 0, 0, "bakugan_s_base_ncl.bin", 0, 0, 0);
    func_02025b64(1, 4, 0x12000, "bakugan_s_base.nce_ncg1D.bin", 0, 0, 0);
    mUnk_74[14]            = new CellAnimation("bakugan_s_base_nce.bin", 0, 1, 1);
    mUnk_74[14]->mCharBase = screen.func_020276f4(0x12000);
    mUnk_74[14]->mUnk_11   = 0;
    mUnk_74[14]->SetAnimation(0);
    ChangeDir("/Game/Card/Graphic/Small");
    func_02025b64(1, 0, 0x400, "card_s_000_ncl.bin", 0, 0, 0);
    if (data_020d433c.mUnk_a8 == 0) {
        return;
    }
    if (mUnk_1e8 != NULL) {
        ((UnkStruct_02078290 *) mUnk_1e8)->func_020785d8(3);
        return;
    }
    mUnk_1e8 = (UnkObject_02078e14 *) new UnkStruct_02078290(3);
}

void UnkStruct_02078e14::func_0207c568() {
    SpriteManager::Get().Screen(1).func_02027624(3);
    ChangeDir("/font/");
    mUnk_0c    = func_02023504("mes_storecomment.mes", 0, 0, 0, 0, 0);
    mUnk_10    = func_02023504("mes_BakuganNameShort.mes", 0, 0, 0, 0, 0);
    mUnk_14    = func_02023504("mes_BakuganName.mes", 0, 0, 0, 0, 0);
    mUnk_18    = func_02023504("mes_CardNameShort.mes", 0, 0, 0, 0, 0);
    mUnk_1c    = func_02023504("mes_DeckSelect.mes", 0, 0, 0, 0, 0);
    mUnk_20    = func_02023504("mes_Result.mes", 0, 0, 0, 0, 0);
    mUnk_38[0] = TextManager::Get().BufferSize(0x18, 0x10);
    mUnk_28[0] = OS_Alloc(mUnk_38[0]);
    MI_CpuClear16(mUnk_28[0], mUnk_38[0]);
    mUnk_40[0] = TextManager::Get().GlyphCount(6);
    mUnk_30[0] = OS_Alloc((s16) mUnk_40[0] * (s16) 0xc);
    TextManager::Get().Init(mUnk_30[0], 1, 0, 0x18, 0x10, 0x44);
    TextManager::Get().SetColor(1, 0, 0xf);
    u8 *alphabet = (u8 *) func_02023504("mes_Alphabet.mes", 0, 0, 0, 0, 0);
    TextManager::Get();
    mUnk_24 = *(u16 *) (alphabet + *(u16 *) (alphabet + 2) + 0xb4);
    OS_Free(alphabet);
    switch (mUnk_e0) {
        case 0:
            mUnk_1ec = new MessageWindow("mes_storecomment", 1, 0x1b000);
            mUnk_1ec->func_02065890(0xb, 0, 1);
            break;
        case 1:
            mUnk_1ec = new MessageWindow("mes_storecomment", 1, 0x1b000);
            mUnk_1ec->func_02065890(0xd, 0, 1);
            break;
        case 2:
            mUnk_1ec = new MessageWindow("mes_storecomment", 1, 0x1b000);
            mUnk_1ec->func_02065890(0xc, 0, 1);
            break;
        case 3:
            mUnk_1ec = new MessageWindow("mes_DeckSelect", 1, 0x1b000);
            mUnk_1ec->func_02065890(6, 0, 1);
            break;
        case 5:
            mUnk_1ec = new MessageWindow("mes_DeckSelect", 1, 0x1b000);
            mUnk_1ec->func_02065890(5, 0, 1);
            break;
        case 4:
            mUnk_1ec = new MessageWindow("mes_myroom", 1, 0x1b000);
            mUnk_1ec->func_02065890(0x30, 0, 1);
            break;
    }
    if (mUnk_1ec != NULL) {
        mUnk_1ec->func_02065bd4(0x14, 4, 0);
        mUnk_1ec->func_02065be4(0xd8);
    }
}

void UnkStruct_02078e14::func_0207ca60() {
    u8 i = 0;
    func_0207cf84();
    switch (mUnk_e0) {
        case 0:
            for (; i < 4; i++) {
                s32 index = mUnk_fd + i;
                if (index >= mUnk_fc) {
                    break;
                }
                u32 a = func_02086548((u8) mUnk_106[index], mUnk_104);
                u32 b = func_02086570((u8) mUnk_106[index], mUnk_104);
                func_02081d5c((u8) mUnk_106[mUnk_fd + i], i, a, b);
            }
            break;
        case 1:
            for (; i < 4; i++) {
                if (mUnk_fd + i >= mUnk_fc) {
                    break;
                }
                UnkStruct_02023248 *entry = func_02023248(data_020d43f0, (u8) mUnk_106[mUnk_fd + i], mUnk_104);
                u32 v = func_020231f4(data_020d43f0, 0, (u8) mUnk_106[mUnk_fd + i], mUnk_104);
                func_02081d5c((u8) mUnk_106[mUnk_fd + i], i, v, (u8) entry->mUnk_00_0);
            }
            break;
        case 2:
            for (; i < 4; i++) {
                if (mUnk_fd + i >= mUnk_fc) {
                    break;
                }
                func_0208250c(mUnk_106[mUnk_fd + i], i);
            }
            break;
        case 3:
        case 5:
            if (mUnk_e6 == 0) {
                break;
            }
            if (mUnk_e6 < 4) {
                for (; i < 4; i++) {
                    if (mUnk_fd + i >= mUnk_fc) {
                        break;
                    }
                    UnkStruct_02023248 *entry = func_02023248(data_020d43f0, (u8) mUnk_106[mUnk_fd + i], mUnk_104);
                    u32 v = func_020231f4(data_020d43f0, 0, (u8) mUnk_106[mUnk_fd + i], mUnk_104);
                    func_02081d5c((u8) mUnk_106[mUnk_fd + i], i, v, (u8) entry->mUnk_00_0);
                }
            } else {
                for (; i < 4; i++) {
                    if (mUnk_fd + i >= mUnk_fc) {
                        break;
                    }
                    func_0208250c(mUnk_106[mUnk_fd + i], i);
                }
            }
            break;
        case 4:
            if (mUnk_e6 == 0) {
                break;
            }
            if (mUnk_e6 < 7) {
                for (; i < 4; i++) {
                    if (mUnk_fd + i >= mUnk_fc) {
                        break;
                    }
                    UnkStruct_02023248 *entry = func_02023248(data_020d43f0, (u8) mUnk_106[mUnk_fd + i], mUnk_104);
                    u32 v = func_020231f4(data_020d43f0, 0, (u8) mUnk_106[mUnk_fd + i], mUnk_104);
                    func_02081d5c((u8) mUnk_106[mUnk_fd + i], i, v, (u8) entry->mUnk_00_0);
                }
            } else {
                for (; i < 4; i++) {
                    if (mUnk_fd + i >= mUnk_fc) {
                        break;
                    }
                    func_0208250c(mUnk_106[mUnk_fd + i], i);
                }
            }
            break;
    }
    func_02025c1c(SpriteManager::Get().Screen(1).mBgIndex, 4, 0, mUnk_28[0], mUnk_38[0], 0);
}

void UnkStruct_02078e14::func_0207ceb0() {
    if (mUnk_1a1 != 0) {
        return;
    }
    if (mUnk_1ec != NULL) {
        mUnk_1ec->func_020658d0();
    }
    SpriteManager::Get().Screen(1).func_02027778(mUnk_30[0], (u8) mUnk_40[0], 0x41, (s16) (mUnk_f8 + 0x48), 2, 0, 0xf, 0);
}

void UnkStruct_02078e14::func_0207cf84() {
    if (mUnk_28[0] != NULL) {
        MI_CpuClear16(mUnk_28[0], mUnk_38[0]);
    }
}

void UnkStruct_02078e14::func_0207cfa4() {
    u32 act = 0;
    if (mUnk_ec != 0 || mUnk_e7 != mUnk_e8 || mUnk_100 != 0 || mUnk_fb != mUnk_fa || mUnk_3ff != 0) {
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
                } else if (TouchManager::Get().mUnk_58 != 0) {
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
                } else if (TouchManager::Get().mUnk_58 != 0) {
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
                } else if (TouchManager::Get().mUnk_58 != 0) {
                    act = 5;
                }
            }
            break;
        case 6:
            if (act == 0) {
                if (mUnk_f5 == 0) {
                    mUnk_f5 = 9;
                    act     = 6;
                } else if (TouchManager::Get().mUnk_58 != 0) {
                    act = 6;
                }
            }
            break;
    }
    if (mUnk_4c->mUnk_061 == 1) {
        if (InputManager::Get().IsHeld(1)) {
            if (act == 0) {
                act = 1;
            }
        } else if (InputManager::Get().IsHeld(2)) {
            if (act == 0) {
                act = 2;
                mUnk_4c->mCells[0]->SetAnimation(3);
                mUnk_3fa = 6;
            }
        } else if (InputManager::Get().IsTrigger(0x20)) {
            if (act == 0) {
                if (mUnk_f0 != 0) {
                    act = 3;
                    if (InputManager::Get().IsHeld(0x20)) {
                        mUnk_f5 = 1;
                    } else {
                        mUnk_f5 = 2;
                    }
                } else if (mUnk_4c->mButtons[3].mEnabled != 0) {
                    mUnk_4c->func_0205c844(3, 3);
                }
            }
        } else if (InputManager::Get().IsTrigger(0x10)) {
            if (act == 0) {
                if (mUnk_f0 != 0) {
                    act = 4;
                    if (InputManager::Get().IsHeld(0x10)) {
                        mUnk_f5 = 3;
                    } else {
                        mUnk_f5 = 4;
                    }
                } else if (mUnk_4c->mButtons[4].mEnabled != 0) {
                    mUnk_4c->func_0205c844(4, 3);
                }
            }
        } else if (InputManager::Get().IsTrigger(0x40)) {
            if (act == 0) {
                act = 5;
            }
        } else if (InputManager::Get().IsTrigger(0x80)) {
            if (act == 0) {
                act = 6;
            }
        }
        if (mUnk_58->func_0202f468(0) == 3) {
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
        if (mUnk_54->func_0202f468(0) == 3) {
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
                    mUnk_fb = mUnk_fd + cur - 5;
                }
                SoundManager::Get().func_020246f8(5);
            }
        }
    }
    if (mUnk_f0 != 0) {
        switch (act) {
            case 1:
            case 8:
                if (mUnk_fc == 0) {
                    return;
                }
                mUnk_f0     = 0;
                mUnk_3f9    = 1;
                mUnk_194[0] = 1;
                func_02081128();
                func_02081594();
                if ((mUnk_e0 == 3 || mUnk_e0 == 5) && mUnk_e6 == 0) {
                    func_02085460(-1);
                }
                SoundManager::Get().func_020246f8(0);
                return;
            case 2:
                if (mUnk_e0 == 5) {
                    mUnk_e1 = 0xf;
                } else {
                    mUnk_e1 = 0x2b;
                }
                SoundManager::Get().func_020246f8(1);
                return;
            case 3:
                if (mUnk_e8 > mUnk_f4) {
                    mUnk_e7 = mUnk_e8 - 1;
                    SoundManager::Get().func_020246f8(5);
                } else {
                    mUnk_f5 = 0;
                }
                return;
            case 4:
                if (mUnk_e8 < mUnk_e9 - 1) {
                    mUnk_e7 = mUnk_e8 + 1;
                    SoundManager::Get().func_020246f8(5);
                } else {
                    mUnk_f5 = 0;
                }
                return;
        }
    } else {
        switch (act) {
            case 1:
                if (mUnk_fc == 0) {
                    return;
                }
                switch (mUnk_e0) {
                    case 0:
                        mUnk_1dc = func_020864ec((u8) mUnk_106[mUnk_fa], mUnk_104);
                        if (data_020d43f0[0].mMoney < mUnk_1dc) {
                            func_020834ec(this, 1);
                        } else if (func_020231f4(data_020d43f0, 0, (u8) mUnk_106[mUnk_fa], mUnk_104) != 0) {
                            func_020834ec(this, 3);
                        } else {
                            func_020834ec(this, 2);
                        }
                        SoundManager::Get().func_020246f8(0);
                        return;
                    case 1: {
                        UnkStruct_02023248 *entry = func_02023248(data_020d43f0, (u8) mUnk_106[mUnk_fa], mUnk_104);
                        mUnk_1dc                  = func_02086224((u8) mUnk_106[mUnk_fa], mUnk_104);
                        if ((s32) entry->mUnk_00_0 >= 9) {
                            func_020834ec(this, 7);
                        } else if (data_020d43f0[0].mMoney < mUnk_1dc) {
                            func_020834ec(this, 1);
                        } else {
                            func_020834ec(this, 2);
                        }
                        SoundManager::Get().func_020246f8(0);
                        return;
                    }
                    case 2:
                        mUnk_1dc = Table_020a39fc()[mUnk_106[mUnk_fa]];
                        if (data_020d43f0[0].mMoney < mUnk_1dc) {
                            func_020834ec(this, 1);
                        } else {
                            func_020834ec(this, 2);
                        }
                        SoundManager::Get().func_020246f8(0);
                        return;
                    case 3:
                    case 5:
                        if (mUnk_e6 == 0) {
                            if (mUnk_fa == 0) {
                                func_020834ec(this, 8);
                            } else if (mUnk_fa == 1) {
                                func_020834ec(this, 9);
                            }
                        } else if (mUnk_e6 < 4) {
                            if (func_02082d00(1, (u8) mUnk_106[mUnk_fa], mUnk_104) == 0) {
                                mUnk_3fc = (mUnk_fa - mUnk_fd) * 0x20 + 0x24;
                                mUnk_3fe = mUnk_3fc / 8;
                                mUnk_3ff = 1;
                            }
                        } else {
                            if (func_02082d00(0, (u8) mUnk_106[mUnk_fa], 0) == 0) {
                                mUnk_3fc = (mUnk_fa - mUnk_fd) * 0x20 + 0x24;
                                mUnk_3fe = mUnk_3fc / 8;
                                mUnk_3ff = 1;
                            }
                        }
                        SoundManager::Get().func_020246f8(0);
                        return;
                }
                break;
            case 2:
            case 7:
                mUnk_f0                = 1;
                mUnk_3f9               = 2;
                mUnk_186[mUnk_e6]      = mUnk_fa;
                mUnk_194[0]            = 1;
                func_02081128();
                func_02081594();
                if ((mUnk_e0 == 3 || mUnk_e0 == 5) && mUnk_e6 == 0) {
                    func_02085460(-1);
                }
                if (act == 2) {
                    SoundManager::Get().func_020246f8(1);
                } else {
                    SoundManager::Get().func_020246f8(0);
                }
                return;
            case 3:
            case 4:
                if ((mUnk_e0 == 3 || mUnk_e0 == 5) && mUnk_e6 != 0 && mUnk_e6 < 4) {
                    if (act == 3) {
                        mUnk_104 = (mUnk_104 + 5) % 6;
                    } else {
                        mUnk_104 = (mUnk_104 + 1) % 6;
                    }
                    func_02080bdc(1);
                    mUnk_fa     = 0;
                    mUnk_fb     = 0;
                    mUnk_fd     = 0;
                    mUnk_194[2] = 1;
                    mUnk_194[0] = 1;
                    func_02081128();
                    SoundManager::Get().func_020246f8(0);
                }
                mUnk_f5 = 0;
                return;
            case 5:
                if (mUnk_fc <= 1) {
                    return;
                }
                if (mUnk_fa != 0) {
                    mUnk_fb = mUnk_fa - 1;
                } else {
                    mUnk_fb = mUnk_fc - 1;
                }
                SoundManager::Get().func_020246f8(5);
                return;
            case 6:
                if (mUnk_fc <= 1) {
                    return;
                }
                if (mUnk_fa < mUnk_fc - 1) {
                    mUnk_fb = mUnk_fa + 1;
                } else {
                    mUnk_fb = 0;
                }
                SoundManager::Get().func_020246f8(5);
                return;
        }
    }
}

void UnkStruct_02078e14::func_0207dea4() {
    switch (mUnk_1a0) {
        case 1:
            mUnk_4c->mUnk_064 = 0;
            if (mUnk_c0 != NULL) {
                func_0205821c(mUnk_c0);
                mContext->mUnk_12 = 0;
                mUnk_400          = 1;
                mUnk_1a0          = 3;
            } else if (mUnk_c4 == NULL) {
                mUnk_1a0 = 2;
            } else {
                func_02055624(mUnk_c4, 1);
                mContext->mUnk_12 = 0;
                mUnk_401          = 1;
                mUnk_1a0          = 3;
            }
            SpriteManager::Get().sFade[0] = 3;
            break;
        case 2:
            if (SpriteManager::Get().sFade[0] <= 1) {
                func_02082660();
                mUnk_1a0 = 4;
            }
            break;
        case 3:
            if (mContext->mUnk_12 != 0) {
                if (mUnk_c0 != NULL) {
                    mUnk_c0 = NULL;
                }
                if (mUnk_c4 != NULL) {
                    mUnk_c4 = NULL;
                }
                mUnk_1a0 = 4;
            }
            break;
        case 4:
            func_0208038c();
            mUnk_1a0        = 0;
            mUnk_4c->mUnk_064 = 1;
            break;
    }
}

void UnkStruct_02078e14::func_0207e074() {
    if (mUnk_194[0] != 0) {
        mUnk_194[0] = 0;
        if (mUnk_e8 > mUnk_f4 && mUnk_f0 != 0) {
            func_02081994(0, 1, 8);
            mUnk_4c->mCells[1]->func_02029fc0();
        } else {
            func_02081994(0, 1, 0x8000);
        }
        if (mUnk_e8 < mUnk_e9 - 1 && mUnk_f0 != 0) {
            func_02081994(0, 2, 8);
            mUnk_4c->mCells[2]->func_02029fc0();
        } else {
            func_02081994(0, 2, 0x8000);
        }
        for (u8 i = 0; i <= 4; i++) {
            if (mUnk_e8 + i >= mUnk_e9 || mUnk_f0 == 0) {
                func_02081994(2, i, 0x8000);
            } else {
                func_02081994(2, i, 8);
            }
        }
        for (u8 i = 5; i <= 7; i++) {
            if (i - 5 >= mUnk_fc || mUnk_f0 != 0) {
                func_02081994(2, i, 0x8000);
            } else {
                func_02081994(2, i, 8);
            }
        }
        func_02081994(0, 0, 8);
        if (mUnk_fc < 2 || mUnk_f0 != 0) {
            func_02081994(0, 5, 0x8000);
            func_02081994(0, 6, 0x8000);
        } else {
            func_02081994(0, 5, 8);
            mUnk_4c->mCells[5]->func_02029fc0();
            func_02081994(0, 6, 8);
            mUnk_4c->mCells[6]->func_02029fc0();
        }
        switch (mUnk_e0) {
            case 0:
            case 1:
            case 2:
            case 4:
                func_02081994(0, 3, 0x8000);
                func_02081994(0, 4, 0x8000);
                break;
            case 3:
            case 5:
                if (mUnk_e6 != 0 && mUnk_e6 < 4 && mUnk_f0 == 0) {
                    func_02081994(0, 3, 8);
                    func_02081994(0, 4, 8);
                    mUnk_74[7]->SetAnimation((mUnk_104 + 5) % 6 * 2 + 0x10);
                    mUnk_74[8]->SetAnimation((mUnk_104 + 1) % 6 * 2 + 0x10);
                } else {
                    func_02081994(0, 3, 0x8000);
                    func_02081994(0, 4, 0x8000);
                }
                break;
        }
        if (mUnk_f0 != 0) {
            mUnk_54->mSelection = 0;
        } else {
            mUnk_54->mSelection = mUnk_fa - mUnk_fd + 5;
        }
    }
    mUnk_e2 = mUnk_4c->Update(0);
}

void UnkStruct_02078e14::func_0207e424() {
    switch (mUnk_f5) {
        case 2:
            if (InputManager::Get().IsDown(0x20) == 0 && mUnk_ec == 0) {
                func_0207e7e4();
            }
            break;
        case 4:
            if (InputManager::Get().IsDown(0x10) == 0 && mUnk_ec == 0) {
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
        if (func_020842a0() == 0) {
            func_020807cc();
            mUnk_194[1] = 0;
        }
    }
}

void UnkStruct_02078e14::func_0207e7e4() {
    if (mUnk_f5 != 5 && mUnk_f5 != 7) {
        mUnk_f5 = 0;
    }
    mUnk_e6      = mUnk_e7;
    mUnk_194[2] = 1;
    switch (mUnk_e0) {
        case 0:
        case 1:
        case 2:
            mUnk_104 = mUnk_e6;
            func_02080bdc(1);
            func_02081128();
            func_02081594();
            break;
        case 3:
        case 5:
            func_02085460(-1);
            if (mUnk_e6 == 0) {
                func_02080bdc(1);
                if (mUnk_1a0 == 0) {
                    mUnk_1a0 = 1;
                }
            } else if (mUnk_e6 < 4) {
                mUnk_104 = mUnk_c8[mUnk_e6 + 8];
                func_02080bdc(1);
                if (mUnk_c0 == NULL || mUnk_400 != 0) {
                    if (mUnk_1a0 == 0) {
                        mUnk_1a0 = 1;
                    }
                } else {
                    func_02081128();
                }
            } else {
                mUnk_104 = mUnk_e6 - 4;
                func_02080bdc(1);
                if (mUnk_c4 == NULL || mUnk_401 != 0) {
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
                func_02080bdc(1);
                if (mUnk_1a0 == 0) {
                    mUnk_1a0 = 1;
                }
                GXS_SetVisiblePlane(0x19);
            } else if (mUnk_e6 < 7) {
                mUnk_104 = mUnk_e6 - 1;
                func_02080bdc(1);
                if (mUnk_c0 == NULL || mUnk_400 != 0) {
                    if (mUnk_1a0 == 0) {
                        mUnk_1a0 = 1;
                    }
                } else {
                    func_02081128();
                }
                GXS_SetVisiblePlane(0x1d);
            } else {
                mUnk_104 = mUnk_e6 - 7;
                func_02080bdc(1);
                if (mUnk_c4 == NULL || mUnk_401 != 0) {
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

void UnkStruct_02078e14::func_0207ea3c() {
    if (mUnk_100 != 0 || mUnk_fb != mUnk_fa) {
        if (mUnk_100 == 0 && mUnk_fd <= mUnk_fb && mUnk_fb < mUnk_fd + 3) {
            mUnk_fa = mUnk_fb;
            func_02081128();
            func_02081594();
            func_02085460(-1);
            mUnk_194[0] = 1;
        } else if (func_020842a0() == 0) {
            u32 moved = 0;
            if (mUnk_100 == 0) {
                mUnk_100 = 1;
                if (mUnk_fb > mUnk_fa) {
                    if (mUnk_fb - mUnk_fa > 1) {
                        mUnk_fa = mUnk_fb;
                        mUnk_fd = mUnk_fb;
                        if (mUnk_fd >= mUnk_fc - 3) {
                            mUnk_fd = mUnk_fc - 3;
                        }
                        moved   = 1;
                    } else {
                        func_02081c2c((u8) mUnk_106[mUnk_fd + 3], (u8) ((mUnk_fd + 3) % 4), 2);
                        func_020823d8(mUnk_106[mUnk_fd + 3], (u8) ((mUnk_fd + 3) % 4), 2);
                    }
                } else {
                    if (mUnk_fa - mUnk_fb > 1) {
                        mUnk_fa = mUnk_fb;
                        mUnk_fd = mUnk_fb;
                        if (mUnk_fd >= mUnk_fc - 3) {
                            mUnk_fd = mUnk_fc - 3;
                        }
                        moved   = 1;
                    } else {
                        mUnk_f8 = -0x20;
                        mUnk_fd--;
                        mUnk_fa--;
                        func_02081c2c((u8) mUnk_106[mUnk_fd], (u8) (mUnk_fd % 4), 2);
                        func_020823d8(mUnk_106[mUnk_fd], (u8) (mUnk_fd % 4), 2);
                        func_0207ca60();
                    }
                }
            }
            if (moved == 0) {
                if (mUnk_fb > mUnk_fa) {
                    mUnk_f8 -= 8;
                } else {
                    mUnk_f8 += 8;
                }
                if (mUnk_f8 >= 0 || mUnk_f8 <= -0x20) {
                    mUnk_100 = 0;
                    if (mUnk_fb > mUnk_fa) {
                        mUnk_fd++;
                        mUnk_fa++;
                        func_0207ca60();
                    }
                    mUnk_f8 = 0;
                    func_02081128();
                    func_02081594();
                    mUnk_194[0] = 1;
                }
                func_020254f0(func_02081a4c, this);
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
        if (func_020842a0() == 0) {
            func_02080898();
            mUnk_194[2] = 0;
        }
        func_0207ca60();
    }
}

void UnkStruct_02078e14::func_0207ed3c() {
    if (mUnk_3f8 == mUnk_3f9) {
        return;
    }
    switch (mUnk_3f9) {
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
    mUnk_3f8 = mUnk_3f9;
}

void UnkStruct_02078e14::func_0207eec4() {
    if (mUnk_c0 != NULL || mUnk_c4 != NULL) {
        return;
    }
    if (mUnk_1a0 > 2) {
        return;
    }
    u8 i = 0;
    switch (mUnk_e0) {
        case 3:
        case 5:
            if (mUnk_5c[0] != NULL) {
                mUnk_5c[0]->Draw(0, 0x80, 0x60, 0);
            }
            if (mUnk_5c[3] != NULL) {
                mUnk_5c[3]->func_0202a3ec(0, 0, 0x80, 0x60, 0);
            }
            for (; i < 9; i++) {
                if (i < 3) {
                    if (mUnk_3f4 == 0 || i == 0) {
                        func_02081a74((u8) mUnk_c8[9 + i], i, 0, i * 24 + 0x15, 0x57);
                    }
                } else {
                    func_02082264(i - 3, i - 3, 0, (i + 1) * 24, 0x55);
                }
            }
            {
                s32 n = func_0208644c();
                if ((u32) n < 1000) {
                    mUnk_5c[5]->func_0202a3ec(0, n / 100, 0x6e, 0x94, 0);
                    mUnk_5c[5]->func_0202a3ec(0, n / 10 % 10, 0x7a, 0x94, 0);
                    mUnk_5c[5]->func_0202a3ec(0, n % 10, 0x86, 0x94, 0);
                } else {
                    mUnk_5c[5]->func_0202a3ec(0, n / 1000, 0x68, 0x94, 0);
                    mUnk_5c[5]->func_0202a3ec(0, n / 100 % 10, 0x74, 0x94, 0);
                    mUnk_5c[5]->func_0202a3ec(0, n / 10 % 10, 0x80, 0x94, 0);
                    mUnk_5c[5]->func_0202a3ec(0, n % 10, 0x8c, 0x94, 0);
                }
            }
            break;
        case 4:
            if (mUnk_5c[1] != NULL) {
                mUnk_5c[1]->Draw(i, 0x80, 0x60, i);
            }
            if (mUnk_5c[3] != NULL) {
                mUnk_5c[3]->func_0202a3ec(0, 1, 0x80, 0x60, 0);
            }
            func_020831f4();
            if (mUnk_5c[2] == NULL) {
                return;
            }
            for (; i < 14; i++) {
                s16 x = i / 7 * 0x78 + 0x24;
                s16 y = i % 7 * 0x14 + 0x2c;
                if (i % 7 == 6) {
                    y += 6;
                }
                mUnk_5c[2]->func_0202a3ec(0, func_0208349c(i), x, y, 0);
            }
            break;
    }
}

void UnkStruct_02078e14::func_0207f2f0() {
    if (mUnk_4c != NULL) {
        mUnk_4c->Draw();
    }
    if (mUnk_48 != NULL) {
        mUnk_48->Draw();
    }
    if (mUnk_50 != NULL) {
        mUnk_50->Draw();
    }
}

void UnkStruct_02078e14::func_0207f32c() {
    if (mUnk_3ff != 0) {
        if (mUnk_e6 < 4) {
            func_02081a74(mUnk_104, (u8) (mUnk_fa % 4), 4, 0x1d, mUnk_3fc + 0x25);
        } else {
            func_02082264(mUnk_104, (u8) (mUnk_fa % 4), 4, 0x20, mUnk_3fc + 0x24);
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
        switch (mUnk_e0) {
            case 0:
            case 1:
                mUnk_74[4]->func_0202a3ec(1, (u8) (mUnk_ea + i), mUnk_e4 + 0x1e + i * 0x2c, 0x26, 1);
                break;
            case 2:
                mUnk_74[5]->func_0202a3ec(1, (u8) (mUnk_ea + i), mUnk_e4 + 0x20 + i * 0x2c, 0x24, 1);
                break;
            case 3:
            case 5:
                if ((mUnk_ea + i) == 0) {
                    mUnk_74[10]->SetAnimation(0x1d);
                    mUnk_74[10]->Draw(1, (s16) (mUnk_e4 + 0x19 + i * 0x2c), 0x21, 1);
                } else if ((mUnk_ea + i) < 4) {
                    if (mUnk_3f4 == 0 || (mUnk_ea + i) == 1) {
                        func_02081a74((u8) mUnk_c8[(mUnk_ea + i) + 8], (u8) ((mUnk_ea + i) - 1), 1, mUnk_e4 + 0x1d + i * 0x2c, 0x25);
                    }
                } else {
                    func_02082264((mUnk_ea + i) - 4, (mUnk_ea + i) - 4, 1, mUnk_e4 + 0x20 + i * 0x2c, 0x24);
                }
                break;
            case 4:
                if ((mUnk_ea + i) == 0) {
                    mUnk_74[10]->SetAnimation(0x20);
                    mUnk_74[10]->Draw(1, (s16) (mUnk_e4 + 0x19 + i * 0x2c), 0x21, 1);
                } else if ((mUnk_ea + i) < 7) {
                    mUnk_74[4]->func_0202a3ec(1, (u8) ((mUnk_ea + i) - 1), mUnk_e4 + 0x1e + i * 0x2c, 0x26, 1);
                } else {
                    mUnk_74[5]->func_0202a3ec(1, (u8) ((mUnk_ea + i) - 7), mUnk_e4 + 0x20 + i * 0x2c, 0x24, 1);
                }
                break;
        }
        i++;
    } while (i < limit);
    mUnk_74[0]->Draw(1, 0x80, 0x60, 1);
    switch (mUnk_e0) {
        case 0:
        case 1:
            mUnk_74[1]->func_0202a3ec(1, 1, mUnk_e4 + 0x14 - mUnk_ea * 0x2c, 0x22, 1);
            break;
        case 2:
            mUnk_74[1]->func_0202a3ec(1, 0, mUnk_e4 + 0x14 - mUnk_ea * 0x2c, 0x22, 1);
            mUnk_74[1]->func_0202a3ec(1, 0, mUnk_e4 + 0x98 - mUnk_ea * 0x2c, 0x22, 1);
            break;
        case 3:
        case 5:
            if (mUnk_ea < 6) {
                mUnk_74[1]->func_0202a3ec(1, 0, mUnk_e4 + 0x40 - mUnk_ea * 0x2c, 0x22, 1);
            }
            mUnk_74[1]->func_0202a3ec(1, 0, mUnk_e4 + 0xc4 - mUnk_ea * 0x2c, 0x22, 1);
            if (mUnk_ea != 0) {
                mUnk_74[1]->func_0202a3ec(1, 0, mUnk_e4 + 0x148 - mUnk_ea * 0x2c, 0x22, 1);
            }
            break;
        case 4:
            if (mUnk_ea < 8) {
                mUnk_74[1]->func_0202a3ec(1, 1, mUnk_e4 + 0x40 - mUnk_ea * 0x2c, 0x22, 1);
            }
            if (mUnk_ea != 0) {
                mUnk_74[1]->func_0202a3ec(1, 0, mUnk_e4 + 0x148 - mUnk_ea * 0x2c, 0x22, 1);
            }
            if (mUnk_ea > 3) {
                mUnk_74[1]->func_0202a3ec(1, 0, mUnk_e4 + 0x1cc - mUnk_ea * 0x2c, 0x22, 1);
            }
            break;
    }
}

void UnkStruct_02078e14::func_0207f90c() {
    if (mUnk_e0 == 4 && mUnk_e6 == 0) {
        return;
    }
    mUnk_74[15]->Update();
    u8 count = 4;
    u8 i     = 0;
    if (mUnk_100 == 0) {
        count--;
    }
    do {
        if (mUnk_fd + i >= mUnk_fc) {
            if (mUnk_fc == 0 && i == 0) {
                mUnk_74[12]->SetAnimation(0x21);
                mUnk_74[12]->Draw(1, 0x71, (s16) (mUnk_f8 + 0x4b + i * 32), 2);
            }
        } else {
            switch (mUnk_e0) {
                case 0:
                    if (func_020231f4(data_020d43f0, 0, (u8) mUnk_106[mUnk_fd + i], mUnk_104) == 0) {
                        mUnk_74[15]->Draw(1, 0x19, (s16) (mUnk_f8 + 0x5b + i * 32), 2);
                    }
                    func_02081a74(mUnk_104, (u8) ((mUnk_fd + i) % 4), 2, 0x1d, mUnk_f8 + 0x49 + i * 32);
                    func_020816ec(func_020864ec((u8) mUnk_106[mUnk_fd + i], mUnk_104), i);
                    break;
                case 1:
                    func_02081a74(mUnk_104, (u8) ((mUnk_fd + i) % 4), 2, 0x1d, mUnk_f8 + 0x49 + i * 32);
                    func_020816ec(func_02086224((u8) mUnk_106[mUnk_fd + i], mUnk_104), i);
                    if (func_02082d00(1, (u8) mUnk_106[mUnk_fd + i], mUnk_104) != 0) {
                        mUnk_74[9]->Draw(1, 0x80, (s16) (mUnk_f8 + 0x54 + i * 32), 2);
                    }
                    break;
                case 2:
                    func_02082264(mUnk_104, (u8) ((mUnk_fd + i) % 4), 2, 0x20, mUnk_f8 + 0x48 + i * 32);
                    func_020816ec(Table_020a39fc()[mUnk_106[mUnk_fd + i]], i);
                    break;
                case 3:
                case 5:
                    if (mUnk_e6 == 0) {
                        mUnk_74[11]->func_0202a3ec(1, (u8) (i + 1), 0x19, mUnk_f8 + 0x45 + i * 32, 2);
                        mUnk_74[12]->SetAnimation(0x1f);
                        mUnk_74[12]->func_0202a3ec(1, (u8) (i + 1), 0x71, mUnk_f8 + 0x4b + i * 32, 2);
                    } else if (mUnk_e6 < 4) {
                        func_02081a74(mUnk_104, (u8) ((mUnk_fd + i) % 4), 2, 0x1d, mUnk_f8 + 0x49 + i * 32);
                        if (func_02082d00(1, (u8) mUnk_106[mUnk_fd + i], mUnk_104) != 0) {
                            mUnk_74[9]->Draw(1, 0x80, (s16) (mUnk_f8 + 0x54 + i * 32), 2);
                        }
                    } else {
                        func_02082264(mUnk_104, (u8) ((mUnk_fd + i) % 4), 2, 0x20, mUnk_f8 + 0x48 + i * 32);
                        if (func_02082d00(0, (u8) mUnk_106[mUnk_fd + i], 0) != 0) {
                            mUnk_74[9]->Draw(1, 0x80, (s16) (mUnk_f8 + 0x54 + i * 32), 2);
                        }
                    }
                    break;
                case 4:
                    if (mUnk_e6 < 7) {
                        func_02081a74(mUnk_104, (u8) ((mUnk_fd + i) % 4), 2, 0x1d, mUnk_f8 + 0x49 + i * 32);
                        if (func_02082d00(1, (u8) mUnk_106[mUnk_fd + i], mUnk_104) != 0) {
                            mUnk_74[9]->Draw(1, 0x80, (s16) (mUnk_f8 + 0x54 + i * 32), 2);
                        }
                    } else {
                        func_02082264(mUnk_104, (u8) ((mUnk_fd + i) % 4), 2, 0x20, mUnk_f8 + 0x48 + i * 32);
                        if (func_02082d00(0, (u8) mUnk_106[mUnk_fd + i], 0) != 0) {
                            mUnk_74[9]->Draw(1, 0x80, (s16) (mUnk_f8 + 0x54 + i * 32), 2);
                        }
                    }
                    break;
            }
        }
        i++;
    } while (i < count);
    if ((mUnk_e0 == 3 || mUnk_e0 == 5) && mUnk_e6 == 0) {
        return;
    }
    func_0207ff54();
}

void UnkStruct_02078e14::func_0207ff54() {
    s16 x = 0xb7;
    u8 a  = mUnk_fa + 1;
    if (mUnk_fc == 0) {
        a = 0;
    }
    mUnk_74[13]->SetAnimation(0);
    if (a >= 10) {
        mUnk_74[13]->func_0202a3ec(1, (u8) (a / 10), 0xb7, 0xaf, 1);
    }
    x += 6;
    mUnk_74[13]->func_0202a3ec(1, (u8) (a % 10), x, 0xaf, 1);
    mUnk_74[13]->SetAnimation(2);
    x += 6;
    mUnk_74[13]->Draw(1, x, 0xaf, 1);
    mUnk_74[13]->SetAnimation(0);
    x += 6;
    if (mUnk_fc >= 10) {
        mUnk_74[13]->func_0202a3ec(1, (u8) (mUnk_fc / 10), x, 0xaf, 1);
    }
    mUnk_74[13]->func_0202a3ec(1, (u8) (mUnk_fc % 10), x + 6, 0xaf, 1);
    mUnk_74[2]->Draw(1, 0xaf, 0xaa, 1);
}

void UnkStruct_02078e14::func_02080104() {
    switch (mUnk_e0) {
        case 0:
        case 1:
        case 2:
            func_020816ec(data_020d43f0[0].mMoney, 4);
            break;
        case 3:
        case 5:
            if (mUnk_4c->mButtons[3].mEnabled != 0) {
                mUnk_74[7]->Update();
                mUnk_74[7]->Draw(1, 5, 0x6c, 1);
            }
            if (mUnk_4c->mButtons[4].mEnabled != 0) {
                mUnk_74[8]->Update();
                mUnk_74[8]->Draw(1, 0xed, 0x6c, 1);
            }
            break;
    }
    mUnk_74[3]->Update();
    if (mUnk_f0 != 0) {
        mUnk_74[3]->Draw(1, 0x19, 0x21, 1);
    } else {
        mUnk_74[3]->Draw(1, 0x19, (s16) ((mUnk_fa - mUnk_fd) * 32 + 0x45), 1);
    }
}

void UnkStruct_02078e14::func_02080210() {
    if (mUnk_1f1 != 0x15) {
        for (u8 i = 0; i < 3; i++) {
            u8 *entry = (u8 *) ((u32 *) data_020d433c.mBattlePlayers)[mUnk_1f0 + 3] + 0xc + i * 12;
            mUnk_c8[i] = entry[8];
            mUnk_c8[9 + i] = entry[9];
        }
        for (u8 i = 0; i < 3; i++) {
            mUnk_c8[3 + i] = *(u16 *) ((u8 *) ((u32 *) data_020d433c.mBattlePlayers)[mUnk_1f0 + 3] + i * 4 + 0x54);
        }
        for (u8 i = 0; i < 3; i++) {
            mUnk_c8[6 + i] = *(u16 *) ((u8 *) ((u32 *) data_020d433c.mBattlePlayers)[mUnk_1f0 + 3] + i * 4 + 0x6c);
        }
    } else {
        for (u8 i = 0; i < 3; i++) {
            mUnk_c8[i]     = func_020230dc(data_020d43f0, 0, i);
            mUnk_c8[9 + i] = func_020230a8(data_020d43f0, 0, i);
        }
        for (u8 i = 0; i < 3; i++) {
            mUnk_c8[3 + i] = data_020d43f0[0].mUnk_06[i];
        }
        for (u8 i = 0; i < 3; i++) {
            mUnk_c8[6 + i] = data_020d43f0[0].mUnk_0c[i];
        }
    }
}

void UnkStruct_02078e14::func_0208038c() {
    switch (mUnk_e0) {
        case 0:
        case 1:
            if (mUnk_c0 != NULL) {
                return;
            }
            mUnk_c0 = new BattleCommandList(0, 0, -1, 0);
            func_02058404(mUnk_c0, 0x1d000, 0, 0xc0, 0x14, 0xe0, 0x14, 0);
            mUnk_400 = 0;
            func_02081128();
            break;
        case 2:
            if (mUnk_c4 != NULL) {
                return;
            }
            mUnk_c4 = new BattleCommandPage(0, -1);
            func_02055764(mUnk_c4, 0x1d000, 0, 0xc0, 0x14, 0xe0, 0x14, 0);
            mUnk_401 = 0;
            func_02081594();
            break;
        case 3:
        case 5:
            if (mUnk_e6 == 0) {
                func_020826ec();
                for (u8 i = 0; i < 9; i++) {
                    if (i < 3) {
                        func_02081c2c((u8) mUnk_c8[i], i, 0);
                    } else {
                        func_020823d8(mUnk_c8[i], (u8) (i - 3), 0);
                    }
                }
                SpriteManager::Get().sFade[0] = 2;
                return;
            }
            if (mUnk_e6 < 4) {
                if (mUnk_c0 != NULL) {
                    return;
                }
                mUnk_c0 = new BattleCommandList(0, 0, -1, 0);
                func_02058404(mUnk_c0, 0x1d000, 0, 0xc0, 0x14, 0xe0, 0x14, 0);
                mUnk_400 = 0;
                func_02081128();
                return;
            }
            if (mUnk_c4 != NULL) {
                return;
            }
            mUnk_c4 = new BattleCommandPage(0, -1);
            func_02055764(mUnk_c4, 0x1d000, 0, 0xc0, 0x14, 0xe0, 0x14, 0);
            mUnk_401 = 0;
            func_02081594();
            return;
        case 4:
            if (mUnk_e6 == 0) {
                func_020826ec();
                SpriteManager::Get().sFade[0] = 2;
                return;
            }
            if (mUnk_e6 < 7) {
                if (mUnk_c0 != NULL) {
                    return;
                }
                mUnk_c0 = new BattleCommandList(0, 0, -1, 0);
                func_02058404(mUnk_c0, 0x1d000, 0, 0xc0, 0x14, 0xe0, 0x14, 0);
                mUnk_400 = 0;
                func_02081128();
                return;
            }
            if (mUnk_c4 != NULL) {
                return;
            }
            mUnk_c4 = new BattleCommandPage(0, -1);
            func_02055764(mUnk_c4, 0x1d000, 0, 0xc0, 0x14, 0xe0, 0x14, 0);
            mUnk_401 = 0;
            func_02081594();
            break;
    }
}

void UnkStruct_02078e14::func_020807cc() {
    SpriteManager::Get();
    if (mUnk_e0 != 3 && mUnk_e0 != 5) {
        return;
    }
    for (u8 i = 0; i < 9; i++) {
        if (i < 3) {
            func_02081c2c((u8) mUnk_c8[i], i, 1);
        } else {
            func_020823d8(mUnk_c8[i], (u8) (i - 3), 1);
        }
    }
}

void UnkStruct_02078e14::func_02080898() {
    SpriteManager::Get();
    u8 i = 0;
    switch (mUnk_e0) {
        case 0:
        case 1:
            for (; i < 3; i++) {
                if (mUnk_fd + i >= mUnk_fc) {
                    return;
                }
                func_02081c2c((u8) mUnk_106[mUnk_fd + i], (u8) ((mUnk_fd + i) % 4), 2);
            }
            break;
        case 2:
            for (; i < 3; i++) {
                if (mUnk_fd + i >= mUnk_fc) {
                    return;
                }
                func_020823d8(mUnk_106[mUnk_fd + i], (u8) ((mUnk_fd + i) % 4), 2);
            }
            break;
        case 3:
        case 5:
            if (mUnk_e6 == 0) {
                return;
            }
            if (mUnk_e6 < 4) {
                for (; i < 3; i++) {
                    if (mUnk_fd + i >= mUnk_fc) {
                        return;
                    }
                    func_02081c2c((u8) mUnk_106[mUnk_fd + i], (u8) ((mUnk_fd + i) % 4), 2);
                }
            } else {
                for (; i < 3; i++) {
                    if (mUnk_fd + i >= mUnk_fc) {
                        return;
                    }
                    func_020823d8(mUnk_106[mUnk_fd + i], (u8) ((mUnk_fd + i) % 4), 2);
                }
            }
            break;
        case 4:
            if (mUnk_e6 == 0) {
                return;
            }
            if (mUnk_e6 < 7) {
                for (; i < 3; i++) {
                    if (mUnk_fd + i >= mUnk_fc) {
                        return;
                    }
                    func_02081c2c((u8) mUnk_106[mUnk_fd + i], (u8) ((mUnk_fd + i) % 4), 2);
                }
            } else {
                for (; i < 3; i++) {
                    if (mUnk_fd + i >= mUnk_fc) {
                        return;
                    }
                    func_020823d8(mUnk_106[mUnk_fd + i], (u8) ((mUnk_fd + i) % 4), 2);
                }
            }
            break;
    }
}

BOOL UnkStruct_02078e14::func_02080b48(u32 unk1, u32 unk2) {
    BOOL result = FALSE;
    if (unk1 == 0x22) {
        if (unk2 == 0) {
            if (data_020d43f0[0].mUnk_5d2 != 0) {
                result = TRUE;
            }
        } else {
            result = TRUE;
        }
    } else if ((unk1 != 0x20) & (unk1 != 0x21)) {
        if ((unk1 == 0x24) | (unk1 == 0x25)) {
            if (unk2 == data_020d43f0[0].mUnk_1a_8) {
                result = TRUE;
            }
        } else {
            result = TRUE;
        }
    }
    return result;
}

void UnkStruct_02078e14::func_02080bdc(u32 unk1) {
    u8 i = 0;
    mUnk_fc = 0;
    switch (mUnk_e0) {
        case 0:
        case 1:
            if (mUnk_e0 == 0) {
                for (; i < 0x29; i++) {
                    if (TestFlagBit(data_020d4976, i) && data_020a3768[i][mUnk_104] != 0 && func_02080b48(i, mUnk_104)) {
                        mUnk_106[mUnk_fc] = i;
                        mUnk_fc++;
                    }
                }
                if (!(data_020d43f0[0].mUnk_586[4] & 4) && mUnk_104 == 0 && data_020d43f0[0].mUnk_5d2 != 0) {
                    mUnk_106[mUnk_fc] = 0x22;
                    mUnk_fc++;
                }
            } else if (mUnk_e0 == 1) {
                for (; i < 0x29; i++) {
                    if (func_020231f4(data_020d43f0, 0, i, mUnk_104) != 0) {
                        mUnk_106[mUnk_fc] = i;
                        mUnk_fc++;
                    }
                }
            }
            func_02086668(this, 1);
            break;
        case 2: {
            for (u16 k = 1; k < 0xd5; k++) {
                if (TestFlagBit(data_020d497b, k) && !TestFlagBit(data_020d4946, k) && func_02065c0c(k, 1) == mUnk_104 && func_02065c20(k) < 0x2bc) {
                    mUnk_106[mUnk_fc] = k;
                    mUnk_fc++;
                }
            }
            func_02086668(this, 0);
            break;
        }
        case 3:
        case 5:
            if (mUnk_1f1 != 0x15) {
                mUnk_fc = 0;
            } else if (mUnk_e6 == 0) {
                mUnk_fc = 2;
                mUnk_fa = 0;
            } else if (mUnk_e6 < 4) {
                if (mUnk_3f4 != 0) {
                    mUnk_fc = 0;
                } else {
                    for (; i < 0x29; i++) {
                        if (func_020231f4(data_020d43f0, 0, i, mUnk_104) != 0) {
                            mUnk_106[mUnk_fc] = i;
                            mUnk_fc++;
                        }
                    }
                    func_02086668(this, 1);
                    u8 j = 0;
                    do {
                        if (mUnk_106[j] == mUnk_c8[mUnk_e6 - 1] && mUnk_104 == mUnk_c8[mUnk_e6 + 8]) {
                            mUnk_fa = j;
                        }
                        j++;
                    } while (j < mUnk_fc);
                }
            } else {
                for (u16 k = 1; k < 0xd5; k++) {
                    if (TestFlagBit(data_020d4946, k) && func_02065c0c(k, 1) == mUnk_104 && func_02065c20(k) < 0x2bc) {
                        mUnk_106[mUnk_fc] = k;
                        mUnk_fc++;
                    }
                }
                func_02086668(this, 0);
                u16 j = 0;
                do {
                    if (mUnk_106[j] == mUnk_c8[mUnk_e6 - 1]) {
                        mUnk_fa = j;
                    }
                    j++;
                } while (j < mUnk_fc);
            }
            break;
        case 4:
            if (mUnk_e6 != 0) {
                if (mUnk_e6 < 7) {
                    for (; i < 0x29; i++) {
                        if (func_020231f4(data_020d43f0, 0, i, mUnk_104) != 0) {
                            mUnk_106[mUnk_fc] = i;
                            mUnk_fc++;
                        }
                    }
                    func_02086668(this, 1);
                } else {
                    for (u16 k = 1; k < 0xd5; k++) {
                        if (TestFlagBit(data_020d4946, k) && func_02065c0c(k, 1) == mUnk_104 && func_02065c20(k) < 0x2bc) {
                            mUnk_106[mUnk_fc] = k;
                            mUnk_fc++;
                        }
                    }
                    func_02086668(this, 0);
                }
            }
            break;
    }
    if (mUnk_e0 != 3 && mUnk_e0 != 5) {
        mUnk_fa = mUnk_186[mUnk_e6];
    }
    if (mUnk_fc == 0) {
        mUnk_fa = 0;
    } else if (mUnk_fa > mUnk_fc - 1) {
        mUnk_fa = mUnk_fc - 1;
    }
    mUnk_fb = mUnk_fa;
    if (unk1 != 0) {
        mUnk_fd = mUnk_fa;
    }
    if (mUnk_fc < 3) {
        mUnk_fd = 0;
    } else if (mUnk_fd >= mUnk_fc - 3) {
        mUnk_fd = mUnk_fc - 3;
    }
}

void UnkStruct_02078e14::func_02081128() {
    BattleCellPair *pair;
    u8 id;
    u8 kind;
    u32 value;
    if (mUnk_c0 == NULL) {
        return;
    }
    if (mUnk_400 != 0) {
        return;
    }
    switch (mUnk_e0) {
        case 0: {
            BattleCellPair cell;
            mUnk_c0->func_02058240(0);
            if (mUnk_f0 != 0) {
                func_02058248(mUnk_c0, mUnk_e6);
                return;
            }
            if (mUnk_fc == 0) {
                func_02058228(mUnk_c0, 1);
                return;
            }
            func_02058228(mUnk_c0, 0);
            func_02086580(this, (u8) mUnk_106[mUnk_fa], mUnk_104, &cell);
            func_02058260(mUnk_c0, (u8) mUnk_106[mUnk_fa], mUnk_104, -1);
            func_0205afb4(mUnk_c0, mUnk_104, func_02086548((u8) mUnk_106[mUnk_fa], mUnk_104), &cell);
            return;
        }
        case 1: {
            if (mUnk_f0 != 0) {
                func_02058248(mUnk_c0, mUnk_e6);
                return;
            }
            if (mUnk_fc == 0) {
                func_02058228(mUnk_c0, 1);
                return;
            }
            func_02058228(mUnk_c0, 0);
            pair = (BattleCellPair *) func_02023248(data_020d43f0, (u8) mUnk_106[mUnk_fa], mUnk_104);
            func_02058260(mUnk_c0, (u8) mUnk_106[mUnk_fa], mUnk_104, -1);
            func_0205afb4(mUnk_c0, mUnk_104, func_020231f4(data_020d43f0, 0, (u8) mUnk_106[mUnk_fa], mUnk_104), pair);
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
                    func_02058228(mUnk_c0, 1);
                    return;
                }
                if (mUnk_1f1 != 0x15) {
                    BattleRecord *rec = &data_020d433c.mBattlePlayers->mPlayers[mUnk_1f0]->mRecords[(u8) (mUnk_e6 - 1)];
                    pair  = (BattleCellPair *) rec;
                    value = rec->mUnk_04;
                    id    = mUnk_c8[mUnk_e6 - 1];
                    kind  = mUnk_c8[mUnk_e6 + 8];
                } else {
                    id    = mUnk_c8[mUnk_e6 - 1];
                    kind  = mUnk_c8[mUnk_e6 + 8];
                    pair  = (BattleCellPair *) func_02023248(data_020d43f0, id, kind);
                    value = func_020231f4(data_020d43f0, 0, id, kind);
                }
            } else {
                if (mUnk_fc == 0) {
                    func_02058228(mUnk_c0, 1);
                    return;
                }
                kind  = mUnk_104;
                id    = mUnk_106[mUnk_fa];
                pair  = (BattleCellPair *) func_02023248(data_020d43f0, id, kind);
                value = func_020231f4(data_020d43f0, 0, id, kind);
            }
            func_02058228(mUnk_c0, 0);
            func_02058260(mUnk_c0, id, kind, -1);
            func_0205afb4(mUnk_c0, kind, value, pair);
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
                func_02058248(mUnk_c0, (u8) (mUnk_e6 - 1));
                return;
            }
            if (mUnk_fc == 0) {
                func_02058228(mUnk_c0, 1);
                return;
            }
            func_02058228(mUnk_c0, 0);
            BattleCellPair *pair = (BattleCellPair *) func_02023248(data_020d43f0, (u8) mUnk_106[mUnk_fa], mUnk_104);
            func_02058260(mUnk_c0, (u8) mUnk_106[mUnk_fa], mUnk_104, -1);
            u32 value = func_020231f4(data_020d43f0, 0, (u8) mUnk_106[mUnk_fa], mUnk_104);
            func_0205afb4(mUnk_c0, mUnk_104, value, pair);
            break;
        }
    }
}

void UnkStruct_02078e14::func_02081594() {
    if (mUnk_c4 == NULL) {
        return;
    }
    if (mUnk_401 != 0) {
        return;
    }
    switch (mUnk_e0) {
        case 2:
            if (mUnk_f0 != 0) {
                func_02055668(mUnk_c4, (u16) (mUnk_e6 + 0xd7), -1);
                return;
            }
            if (mUnk_fc == 0) {
                if (mUnk_e6 < 3) {
                    func_02055668(mUnk_c4, 0, -1);
                } else {
                    func_02055668(mUnk_c4, 0xd6, -1);
                }
            } else {
                func_02055668(mUnk_c4, mUnk_106[mUnk_fa], -1);
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
                    v = mUnk_106[mUnk_fa];
                }
                func_02055668(mUnk_c4, (u8) v, -1);
            }
            break;
        case 4:
            if (mUnk_e6 < 7) {
                return;
            }
            if (mUnk_f0 != 0) {
                func_02055668(mUnk_c4, (u16) (mUnk_e6 + 0xd0), -1);
            } else {
                func_02055668(mUnk_c4, mUnk_106[mUnk_fa], -1);
            }
            break;
    }
}

void UnkStruct_02078e14::func_020816ec(u32 unk1, u32 unk2) {
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
    over = data_020d43f0[0].mMoney < unk1 ? 1 : 0;
    if (unk2 != 4) {
        if (unk1 >= 10000000) {
            over = 0;
        }
        if (over != 0) {
            mUnk_74[13]->SetAnimation(5);
        } else {
            mUnk_74[13]->SetAnimation(4);
        }
        mUnk_74[13]->Draw(1, x, y, scale);
    }
    x = x + 12;
    if (unk1 >= 10000000) {
        mUnk_74[13]->SetAnimation(3);
        for (i = 0; i < 7; i++) {
            mUnk_74[13]->Draw(1, (s16) (x + i * 5), y, scale);
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
        mUnk_74[13]->SetAnimation(1);
    } else {
        mUnk_74[13]->SetAnimation(0);
    }
    i = 0;
    if (unk2 == 4) {
        for (; i < 7; i++) {
            if (digits[i] >= 0) {
                mUnk_74[13]->func_0202a3ec(1, digits[i], x + i * 7, y, scale);
            }
        }
        mUnk_74[17]->Draw(1, (s16) (x - 0x2b), (s16) (y - 3), scale);
    } else {
        for (; i < 7; i++) {
            if (digits[i] >= 0) {
                mUnk_74[13]->func_0202a3ec(1, digits[i], x + i * 5, y, scale);
            }
        }
    }
}

void UnkStruct_02078e14::func_02081994(u32 unk1, u32 unk2, u32 unk3) {
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
            items = mUnk_50->mCursor->mItems;
            mUnk_50->mButtons[unk2].mEnabled = unk3 != 0x8000 ? 1 : 0;
            break;
        default:
            return;
    }
    items[unk2].mFlags = unk3;
}

void func_02081a4c(void *arg) {
    UnkStruct_02078e14 *self = (UnkStruct_02078e14 *) arg;
    reg_G2S_DB_BG2OFS = (u32) ((-0x20 - self->mUnk_f8) << 16) & G2_BGOFS_V_MASK;
}

void UnkStruct_02078e14::func_02081a74(u8 unk1, u32 unk2, u32 unk3, s16 unk4, s16 unk5) {
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
    SpriteManager::Get().Screen(second).func_02027778((void *) data_020a3188[1], 1, (s16) (unk4 + 3), (s16) (unk5 + 3), priority, pos, 1, 0);
    if (unk3 == 0) {
        if (mUnk_5c[4] == NULL) {
            return;
        }
        mUnk_5c[4]->func_0202a3ec(second, unk1, unk4, unk5, priority);
    } else {
        mUnk_74[14]->func_0202a3ec(second, unk1, unk4, unk5, priority);
    }
}

void UnkStruct_02078e14::func_02081c2c(u32 unk1, u32 unk2, u32 unk3) {
    switch (mUnk_e0) {
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
    sprintf(path, "bakugan_s_%03d_ncg.bin", data_020a1074[unk1]);
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
    func_02025b64(second, 4, offset, path, 0, 0, 0);
}

static inline void PrintMessageFW(TextManager &tm, MessageFile *file, int index, void *buffer, s16 y, s16 x, u16 *width) {
    tm.Print(file->GetText(index), file->GetLength(index), buffer, 0x18, 0x10, &y, &x, width, 0, 2, 0x44, 0, 0, 0, 0, 1, 1);
}

void UnkStruct_02078e14::func_02081d5c(u32 unk1, u32 unk2, u32 unk3, u32 unk4) {
    s16 b = 0;
    s16 a = 0;
    u16 w;
    TextManager::Get().mUnk_5b = 1;
    a                          = unk2 << 5;
    PrintMessageF(TextManager::Get(), (MessageFile *) mUnk_10, unk1, mUnk_28[0], b, a);
    b = 0x73;
    PrintMessageF(TextManager::Get(), (MessageFile *) mUnk_0c, 0x18, mUnk_28[0], b, a);
    b = 0x87;
    TextManager::Get().PrintNumber(unk3, 3, 0, mUnk_28[0], 0x18, 0x10, &b, &a, 0, 2, 0x44, 0, 0, 1, 1);
    b = 0;
    a += 12;
    PrintMessageFW(TextManager::Get(), (MessageFile *) mUnk_20, 5, mUnk_28[0], b, a, &w);
    b += w;
    if (*(u8 *) &unk4 >= 9) {
        PrintMessageF(TextManager::Get(), (MessageFile *) mUnk_20, 6, mUnk_28[0], b, a);
    } else {
        b += 7;
        TextManager::Get().PrintNumber(*(u8 *) &unk4 + 1, 1, 0, mUnk_28[0], 0x18, 0x10, &b, &a, 0, 2, 0x44, 0, 0, 1, 1);
    }
    TextManager::Get().mUnk_5b = 0;
}

void UnkStruct_02078e14::func_02082264(u8 unk1, u8 unk2, u32 unk3, s16 unk4, s16 unk5) {
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
    SpriteManager::Get().Screen(second).func_02027778((void *) data_020a3188[0], 1, unk4, unk5, priority, pos, 2, 0);
    if (unk3 == 0) {
        return;
    }
    mUnk_74[6]->func_0202a3ec(second, unk1, unk4, unk5, priority);
}

void UnkStruct_02078e14::func_020823d8(u32 unk1, u32 unk2, u32 unk3) {
    switch (mUnk_e0) {
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
    sprintf(path, "card_s_%03d_ncg.bin", func_02065c20(unk1));
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
    func_02025b64(second, 4, offset, path, 0, 0, 0);
}

void UnkStruct_02078e14::func_0208250c(u32 unk1, u32 unk2) {
    TextManager::Get().mUnk_5b = 1;
    s16 x = unk2 << 5;
    PrintMessageF(TextManager::Get(), (MessageFile *) mUnk_18, unk1, mUnk_28[0], 0, x);
    TextManager::Get().mUnk_5b = 0;
}

void UnkStruct_02078e14::func_02082660() {
    for (u8 i = 0; i < 6; i++) {
        delete mUnk_5c[i];
        mUnk_5c[i] = NULL;
    }
    if (mUnk_28[1] != NULL) {
        OS_Free(mUnk_28[1]);
        mUnk_28[1] = NULL;
    }
    if (mUnk_30[1] != NULL) {
        OS_Free(mUnk_30[1]);
        mUnk_30[1] = NULL;
    }
}

void UnkStruct_02078e14::func_020826ec() {
    SpriteManager::Get().func_02026dec(0, 1);
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
    switch (mUnk_e0) {
        case 3:
        case 5:
            func_02025b64(0, 1, 0, "list_edit_u_bg_00_ncl.bin", 0, 0, 0);
            func_02025b64(0, 0xb, 0, "list_edit_u_bg_00_ncg.bin", 0, 0, 0);
            func_02025b64(0, 7, 0, "list_edit_u_bg_00_nsc.bin", 0, 0, 0);
            break;
        case 4:
            func_02025b64(0, 1, 0, "list_gallery_u_bg_00_ncl.bin", 0, 0, 0);
            func_02025b64(0, 0xb, 0, "list_gallery_u_bg_00_ncg.bin", 0, 0, 0);
            func_02025b64(0, 7, 0, "list_gallery_u_bg_00_nsc.bin", 0, 0, 0);
            break;
    }
    SpriteScreen &screen = SpriteManager::Get().Screen(0);
    screen.func_02027624(3);
    ChangeDir("/Game/List/");
    screen.LoadCharacters("list_u_obj_00.nce_ncg1D.bin", 0, 0);
    screen.LoadPalette("list_u_obj_00_ncl.bin", 0, 0);
    for (u8 i = 0; i <= 3; i++) {
        mUnk_5c[i]              = new CellAnimation("list_u_obj_00_nce.bin", 0, 1, 1);
        mUnk_5c[i]->mCharBase   = screen.func_020276f4(0);
        mUnk_5c[i]->mUnk_11     = 0;
    }
    mUnk_5c[0]->SetAnimation(0);
    mUnk_5c[1]->SetAnimation(1);
    mUnk_5c[2]->SetAnimation(2);
    mUnk_5c[3]->SetAnimation(3);
    ChangeDir("/Game/Bakugan/mark");
    func_02025b64(0, 0, 0x200, "bakugan_s_000_ncl.bin", 0, 0, 0);
    func_02025b64(0, 0, 0, "bakugan_s_base_ncl.bin", 0, 0, 0);
    func_02025b64(0, 4, 0x3000, "bakugan_s_base.nce_ncg1D.bin", 0, 0, 0);
    mUnk_5c[4]            = new CellAnimation("bakugan_s_base_nce.bin", 0, 1, 1);
    mUnk_5c[4]->mCharBase = screen.func_020276f4(0x3000);
    mUnk_5c[4]->mUnk_11   = 0;
    mUnk_5c[4]->SetAnimation(0);
    ChangeDir("/Game/2d/union_b/");
    screen.LoadCharacters("union_b_obj_gp.nce_ncg1D.bin", 0x2000, 0);
    screen.LoadPalette("union_b_obj_gp_ncl.bin", 0xa0, 0);
    mUnk_5c[5]            = new CellAnimation("union_b_obj_gp_nce.bin", 0, 1, 1);
    mUnk_5c[5]->mCharBase = screen.func_020276f4(0x2000);
    mUnk_5c[5]->mUnk_11   = 5;
    mUnk_5c[5]->SetAnimation(2);
    ChangeDir("/Game/Card/Graphic/Small");
    func_02025b64(0, 0, 0x400, "card_s_000_ncl.bin", 0, 0, 0);
    func_02082d70();
    func_02082efc(this);
}

BOOL UnkStruct_02078e14::func_02082d00(u32 unk1, u32 unk2, u32 unk3) {
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

void UnkStruct_02078e14::func_02082d70() {
    ChangeDir("/font/");
    mUnk_38[1] = TextManager::Get().BufferSize(0x18, 0x14);
    mUnk_28[1] = OS_Alloc(mUnk_38[1]);
    MI_CpuClear16(mUnk_28[1], mUnk_38[1]);
    mUnk_40[1] = TextManager::Get().GlyphCount(0xf);
    mUnk_30[1] = OS_Alloc((s16) mUnk_40[1] * (s16) 0xc);
    TextManager::Get().Init(mUnk_30[1], 0, 0, 0x18, 0x14, 0x42);
    TextManager::Get().SetColor(0, 0, 0xf);
}

void func_02082efc(UnkStruct_02078e14 *self) {
    s16 b = 0;
    s16 a = 0;
    self->func_0208329c();
    for (u8 i = 0; i < 14; i++) {
        b = (i / 7) * 0x78 + 0x13;
        s32 col = i % 7;
        a       = col * 0x14 + 5;
        if (col == 6) {
            a += 6;
        }
        TextManager::Get().PrintNumber(self->mUnk_1a2[i].mUnk_a, 3, 0, self->mUnk_28[1], 0x18, 0x14, &b, &a, 0, 2, 0x42, 0, 0, 1, 1);
        TextManager::Get().Print(&self->mUnk_24, 2, self->mUnk_28[1], 0x18, 0x14, &b, &a, NULL, 0, 2, 0x42, 0, 0, 0, 0, 1, 1);
        TextManager::Get().PrintNumber(self->mUnk_1a2[i].mUnk_b, 3, 0, self->mUnk_28[1], 0x18, 0x14, &b, &a, 0, 2, 0x42, 0, 0, 1, 1);
    }
    func_02025c1c(SpriteManager::Get().Screen(0).mBgIndex, 4, 0x10000, self->mUnk_28[1], self->mUnk_38[1], 0);
}

void UnkStruct_02078e14::func_020831f4() {
    SpriteManager::Get().Screen(0).func_02027778(mUnk_30[1], (u8) mUnk_40[1], 0x40, 0x22, 0, 0x200, 0xf, 0);
}

void UnkStruct_02078e14::func_0208329c() {
    if (mUnk_28[1] != NULL) {
        MI_CpuClear16(mUnk_28[1], mUnk_38[1]);
    }
}

void UnkStruct_02078e14::func_020832bc() {
    MI_CpuClear16(&mUnk_1a2[0], 0x38);
    mUnk_1a2[0].mUnk_b = 0x11;
    mUnk_1a2[1].mUnk_b = 0x11;
    mUnk_1a2[2].mUnk_b = 0x11;
    mUnk_1a2[3].mUnk_b = 0x11;
    mUnk_1a2[4].mUnk_b = 0x12;
    mUnk_1a2[5].mUnk_b = 0x11;
    mUnk_1a2[6].mUnk_b = 0x67;
    mUnk_1a2[data_020d43f0[0].mUnk_1a_8].mUnk_b += 2;
    mUnk_1a2[6].mUnk_b += 2;
    if (data_020d43f0[0].mUnk_5d2 != 0) {
        mUnk_1a2[0].mUnk_b += 1;
        mUnk_1a2[6].mUnk_b += 1;
    }
    u8 i;
    u8 j;
    for (i = 0; i < 0x29; i++) {
        for (j = 0; j < 6; j++) {
            if (data_020a3768[i][j] != 0) {
                if (func_020231f4(data_020d43f0, 0, i, j) != 0) {
                    mUnk_1a2[j].mUnk_a += 1;
                    mUnk_1a2[6].mUnk_a += 1;
                }
            }
        }
    }
    for (u16 k = 1; k < 0xd5; k++) {
        if (func_02065c20(k) < 0x2bc) {
            u8 sel = func_02065c0c(k, 1) + 7;
            mUnk_1a2[sel].mUnk_b += 1;
            mUnk_1a2[13].mUnk_b += 1;
            if (TestFlagBit(data_020d4946, k)) {
                mUnk_1a2[sel].mUnk_a += 1;
                mUnk_1a2[13].mUnk_a += 1;
            }
        }
    }
}

u8 UnkStruct_02078e14::func_0208349c(u32 unk1) {
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

void func_020834ec(UnkStruct_02078e14 *self, u32 mode) {
    u8 i;
    u8 flag  = 1;
    u8 state = 0;
    for (i = 0; i < 5; i++) {
        self->func_02081994(1, i, 0x8000);
    }
    state = 0;
    switch (mode) {
        case 0:
            if (self->mUnk_c0 != NULL) {
                self->mUnk_c0->func_02058530(1);
            }
            if (self->mUnk_c4 != NULL) {
                self->mUnk_c4->func_02055890(1);
            }
            self->mUnk_404                   = 0x1d;
            self->mUnk_4c->mUnk_064 = 1;
            self->mUnk_48->mUnk_064 = 0;
            state                      = 0;
            if (self->mUnk_1a1 == 10 || self->mUnk_1a1 == 6) {
                self->mUnk_186[self->mUnk_e6] = self->mUnk_fa;
                self->func_02080bdc(0);
                self->func_02081128();
                self->func_02081594();
                self->mUnk_194[2] = 1;
                self->mUnk_194[0] = 1;
            } else if ((u8) (self->mUnk_1a1 + 0xf8) <= 1) {
                for (state = 0; state < 9; state++) {
                    if (state < 3) {
                        self->func_02081c2c((u8) self->mUnk_c8[state], state, 0);
                    } else {
                        self->func_020823d8(self->mUnk_c8[state], (u8) (state - 3), 0);
                    }
                }
                self->mUnk_194[1] = 1;
                self->func_0207ca60();
            } else {
                if (self->mUnk_1a1 == 3) {
                    self->func_02081128();
                }
                self->func_0207ca60();
            }
            self->mUnk_1a1 = mode;
            self->mUnk_e1  = 3;
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
            BattleCellPair *pair = (BattleCellPair *) func_02023248(data_020d43f0, (u8) self->mUnk_106[self->mUnk_fa], self->mUnk_104);
            func_02058260(self->mUnk_c0, (u8) self->mUnk_106[self->mUnk_fa], self->mUnk_104, -1);
            u32 value = func_020231f4(data_020d43f0, state, (u8) self->mUnk_106[self->mUnk_fa], self->mUnk_104);
            func_0205afb4(self->mUnk_c0, self->mUnk_104, value, pair);
            self->mUnk_c0->func_02058240(1);
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
            self->func_02081c2c((u8) self->mUnk_106[self->mUnk_fa], state, 3);
            self->func_02081c2c((u8) self->mUnk_106[self->mUnk_fa], 1, 3);
            break;
        case 6:
            self->func_020823d8(self->mUnk_106[self->mUnk_fa], state, 3);
            break;
    }
    if (self->mUnk_c0 != NULL) {
        self->mUnk_c0->func_02058530(0);
    }
    if (self->mUnk_c4 != NULL) {
        self->mUnk_c4->func_02055890(0);
    }
    GXS_SetVisiblePlane(0x1f);
    self->mUnk_4c->mUnk_064 = 0;
    self->mUnk_48->func_0205c7c8(-1);
    self->mUnk_48->mUnk_064 = 1;
    self->mUnk_48->mButtons[0].mEnabled = 1;
    switch (state) {
        case 0:
            self->func_02081994(1, 3, 1);
            self->mUnk_48->mCursor->mSelection = 3;
            self->mUnk_48->mCells[3]->SetAnimation(1);
            self->mUnk_48->mCells[0]->SetAnimation(9);
            break;
        case 1:
            self->func_02081994(1, 1, 1);
            self->func_02081994(1, 2, 2);
            if (flag != 0) {
                self->mUnk_48->mCursor->mSelection = 1;
                self->mUnk_48->mCells[1]->SetAnimation(4);
            } else {
                self->mUnk_48->mCursor->mSelection = 2;
                self->mUnk_48->mCells[2]->SetAnimation(7);
            }
            self->mUnk_48->mCells[0]->SetAnimation(10);
            break;
        case 2:
            self->func_02081994(1, 1, 1);
            self->func_02081994(1, 2, 2);
            self->mUnk_48->mCursor->mSelection = 1;
            self->mUnk_48->mCells[1]->SetAnimation(4);
            self->mUnk_48->mCells[0]->SetAnimation(10);
            self->mUnk_48->mButtons[4].mEnabled = 1;
            break;
    }
    self->mUnk_1a1 = mode;
    func_020839a0(self);
    self->mUnk_e1  = 4;
    self->mUnk_3f9 = 0;
}

void func_020839a0(UnkStruct_02078e14 *self) {
    const u8 *base;
    u8 index;
    self->func_0207cf84();
    base  = NULL;
    index = 0;
    switch (self->mUnk_1a1) {
        case 0:
            break;
        case 1:
            index = 3;
            base  = (u8 *) self->mUnk_0c;
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
                        value = data_020d43f0[0].mMoney;
                        break;
                    case 1:
                        value = self->mUnk_1dc;
                        x     = 0x28;
                        msg   = 7;
                        break;
                    case 2:
                        msg   = y;
                        value = data_020d43f0[0].mMoney - self->mUnk_1dc;
                        x     = 0x50;
                        break;
                }
                PrintMessageB(TextManager::Get(), (MessageFile *) self->mUnk_0c, msg, self->mUnk_28[0], y, x);
                y += 0x80;
                TextManager::Get().PrintNumber(value, 7, 0, self->mUnk_28[0], 0x18, 0x10, &y, &x, 0, 2, 0x44, 0, 1, 1, 1);
            }
            break;
        }
        case 3:
            index = 4;
            base  = (u8 *) self->mUnk_0c;
            break;
        case 4:
        case 5:
            base  = (u8 *) self->mUnk_0c;
            index = 9;
            TextManager::Get().mUnk_5b = 1;
            PrintMessageF(TextManager::Get(), (MessageFile *) self->mUnk_14, self->mUnk_106[self->mUnk_fa], self->mUnk_28[0], 0x1c, 0x38);
            TextManager::Get().mUnk_5b = 0;
            break;
        case 6:
            base  = (u8 *) self->mUnk_0c;
            index = 0x16;
            TextManager::Get().mUnk_5b = 1;
            PrintMessageF(TextManager::Get(), (MessageFile *) self->mUnk_18, self->mUnk_106[self->mUnk_fa], self->mUnk_28[0], 0x17, 0x38);
            TextManager::Get().mUnk_5b = 0;
            break;
        case 7:
            base  = (u8 *) self->mUnk_0c;
            index = 0xf;
            break;
        case 8:
            base  = (u8 *) self->mUnk_1c;
            index = 0x13;
            break;
        case 9:
            base  = (u8 *) self->mUnk_1c;
            index = 0x15;
            break;
        case 10:
            base  = (u8 *) self->mUnk_0c;
            index = 0x15;
            break;
        case 11:
            base = (u8 *) self->mUnk_1c;
            break;
        case 12:
            base  = (u8 *) self->mUnk_1c;
            index = 0x17;
            break;
    }
    if (base != NULL) {
        PrintMessageF3(TextManager::Get(), (MessageFile *) base, index, self->mUnk_28[0], 0, 0);
    }
    func_02025c1c(SpriteManager::Get().Screen(1).mBgIndex, 4, 0, self->mUnk_28[0], self->mUnk_38[0], 0);
}

void UnkStruct_02078e14::func_0208403c() {
    switch (mUnk_1a1) {
        case 0:
            return;
        case 4:
        case 5:
            func_02081a74(mUnk_104, 0, 3, 0x23, 0x5a);
            break;
        case 6:
            func_02082264(mUnk_104, 0, 3, 0x23, 0x5a);
            break;
    }
    SpriteManager::Get().Screen(1).func_02027778(mUnk_30[0], (u8) mUnk_40[0], 0x20, 0x28, 0, 0, 0xf, 0);
}

void func_0208415c(UnkStruct_02078e14 *self) {
    u8 id     = self->mUnk_106[self->mUnk_fa];
    int value = func_0202317c(data_020d43f0, 0, id, self->mUnk_104);
    func_020231c0(data_020d43f0, value, self->func_02086548(id, self->mUnk_104));
    UnkStruct_02023248 *entry = func_02023248(data_020d43f0, id, self->mUnk_104);
    u32 index                 = func_02086654(self, id, self->mUnk_104);
    entry->mUnk_00_0          = 0;
    entry->mPad               = 0;
    entry->mUnk_0             = data_020a35b8[index][0];
    entry->mUnk_12            = data_020a35b8[index][1];
    entry->mUnk_3             = data_020a35b8[index][2];
    entry->mUnk_6             = data_020a35b8[index][3];
    entry->mUnk_9             = data_020a35b8[index][4];
}

BOOL UnkStruct_02078e14::func_020842a0() {
    if (mUnk_c0 != NULL && mUnk_c0->func_020583f0()) {
        return TRUE;
    }
    if (mUnk_c4 != NULL && mUnk_c4->func_02055724()) {
        return TRUE;
    }
    return FALSE;
}

void UnkStruct_02078e14::func_020842e8() {
    if (mUnk_1f1 != 0x15) {
        return;
    }
    for (u8 i = 0; i < 3; i++) {
        func_02023134(data_020d43f0, 0, i, 0, (u8) mUnk_c8[i], (u8) mUnk_c8[9 + i]);
    }
    for (u8 i = 0; i < 3; i++) {
        data_020d43f0[0].mUnk_06[i] = mUnk_c8[3 + i];
    }
    for (u8 i = 0; i < 3; i++) {
        data_020d43f0[0].mUnk_0c[i] = mUnk_c8[6 + i];
    }
}

void func_020843ac(UnkStruct_02078e14 *self, u32 unk1) {
    SpriteScreen &screen = SpriteManager::Get().Screen(1);
    if (unk1 != 0) {
        ChangeDir("/Game/Battle/Bg/");
        func_02025b64(1, 1, 0, "select_arena_b_bg_00_ncl.bin", 0, 0, 0);
        func_02025b64(1, 0xb, 0, "select_arena_b_bg_00_ncg.bin", 0, 0, 0);
        func_02025b64(1, 7, 0, "select_arena_b_bg_00_nsc.bin", 0, 0, 0);
        ChangeDir("/Game/Deck/");
        screen.LoadCharacters("select_deck_b_obj_01.nce_ncg1D.bin", 0x1c000, 0);
        screen.LoadPalette("select_deck_b_obj_01_ncl.bin", 0xc0, 0);
        ChangeDir("/Game/Battle/Obj/");
        screen.LoadCharacters("battle_b_obj_wait.nce_ncg1D.bin", 0x1e000, 0);
        screen.LoadPalette("battle_b_obj_ability_ncl.bin", 0x100, 0);
        GXS_SetVisiblePlane(0x11);
        self->mUnk_50->mCursor->mSelection = 1;
        self->mUnk_50->mCells[1]->SetAnimation(4);
        self->func_02081994(3, 0, 8);
        self->func_02081994(3, 1, 8);
        self->func_02081994(0, 0, 0x8000);
        self->func_02081994(0, 1, 0x8000);
        self->func_02081994(0, 2, 0x8000);
        self->func_02081994(0, 3, 0x8000);
        self->func_02081994(0, 4, 0x8000);
        self->func_02081994(0, 5, 0x8000);
        self->func_02081994(0, 6, 0x8000);
        if (self->mUnk_e6 != 0) {
            if (self->mUnk_1a0 == 0) {
                self->mUnk_1a0 = 1;
            }
        }
        self->mUnk_e6 = 0;
        self->mUnk_e7 = 0;
        self->mUnk_e8 = 0;
        self->mUnk_ea = 0;
        self->mUnk_fc = 2;
        self->mUnk_fa = 0;
        self->mUnk_fb = 0;
        self->mUnk_fd = 0;
        if (self->mUnk_1f1 != 0x15) {
            self->mUnk_fc = 0;
        }
        self->func_02085460(5);
        self->mUnk_3f9 = 0;
        self->func_0207ed3c();
    } else {
        ChangeDir("/Game/List/");
        func_02025b64(1, 1, 0, "list_edit_b_bg_00_ncl.bin", 0, 0, 0);
        func_02025b64(1, 1, 0x4000, "list_edit_b_bg_00_ncl.bin", 0, 0, 0);
        func_02025b64(1, 1, 0x6000, "list_edit_b_bg_00_ncl.bin", 0, 0, 0);
        func_02025b64(1, 0xb, 0, "list_edit_b_bg_00_ncg.bin", 0, 0, 0);
        func_02025b64(1, 7, 0, "list_edit_b_bg_00_nsc.bin", 0, 0, 0);
        func_02025b64(1, 9, 0, "list_edit_b_bg_01_nsc.bin", 0, 0, 0);
        func_02025b64(1, 0xa, 0, "list_edit_b_bg_02_nsc.bin", 0, 0, 0);
        screen.LoadPalette("list_b_obj_00_ncl.bin", 0xc0, 0);
        ChangeDir("/Game/2d/union_b/");
        screen.LoadCharacters("union_b_obj_point.nce_ncg1D.bin", 0x1c000, 0);
        screen.LoadPalette("union_b_obj_point_ncl.bin", 0x60, 0);
        screen.LoadCharacters("union_b_obj_page.nce_ncg1D.bin", 0x1d000, 0);
        screen.LoadPalette("union_b_obj_page_ncl.bin", 0, 0);
        GXS_SetVisiblePlane(0x1d);
        self->func_02081994(3, 0, 0x8000);
        self->func_02081994(3, 1, 0x8000);
        self->mUnk_194[0] = 1;
        self->mUnk_194[1] = 1;
        self->mUnk_194[2] = 1;
        self->func_02085460(-1);
        self->mUnk_3f9 = 2;
    }
}

void func_020848c8(UnkStruct_02078e14 *self) {
    for (u8 i = 0; i < 3; i++) {
        BattleRecord *record = &data_020d433c.mBattlePlayers->mPlayers[0]->mRecords[i];
        record->mUnk_08      = self->mUnk_c8[i];
        record->mUnk_04      = func_020231f4(data_020d43f0, 0, (u8) self->mUnk_c8[i], (u8) self->mUnk_c8[i + 9]);
        record->mUnk_09      = self->mUnk_c8[i + 9];
        record->mUnk_0a      = 0;
        record->mUnk_0b      = record->mUnk_04 / 10;
        MI_CpuCopy16(func_02023248(data_020d43f0, record->mUnk_08, record->mUnk_09), &data_020d433c.mBattlePlayers->mPlayers[0]->mRecords[i], 4);
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

BOOL func_02084a10(UnkStruct_02078e14 *self) {
    for (u8 i = 0; i < 3; i++) {
        if (self->mUnk_1f1 != 0x15) {
            BattleRecord *record = &data_020d433c.mBattlePlayers->mPlayers[self->mUnk_1f0]->mRecords[i];
            MESSAGE->mUnk_10[i]     = record->mUnk_08;
            MESSAGE->mUnk_04[i]     = record->mUnk_04;
            MESSAGE->mUnk_16[i]     = record->mUnk_09;
        } else {
            MESSAGE->mUnk_10[i] = self->mUnk_c8[i];
            MESSAGE->mUnk_04[i] = func_020231f4(data_020d43f0, 0, (u8) self->mUnk_c8[i], (u8) self->mUnk_c8[i + 9]);
            MESSAGE->mUnk_16[i] = self->mUnk_c8[i + 9];
        }
    }
    BattlePlayerList *players = data_020d433c.mBattlePlayers;
    MESSAGE->mUnk_1c          = 3;
    if (func_ov007_022631bc(players, 3)) {
    u32 count = WirelessManager::Get().func_0202f134();
    u8 j = 0;
    do {
        u8 length = ((BattleListMessage *) &data_020d433c.mBattlePlayers->mPlayers[j]->mMsg)->mUnk_1c;
        for (u8 k = 0; k < 6; k++) {
            BattleRecord *record = &data_020d433c.mBattlePlayers->mPlayers[j]->mRecords[k];
            if (k < length) {
                record->mUnk_08 = ((BattleListMessage *) &data_020d433c.mBattlePlayers->mPlayers[j]->mMsg)->mUnk_10[k];
                record->mUnk_04 = ((BattleListMessage *) &data_020d433c.mBattlePlayers->mPlayers[j]->mMsg)->mUnk_04[k];
                record->mUnk_09 = ((BattleListMessage *) &data_020d433c.mBattlePlayers->mPlayers[j]->mMsg)->mUnk_16[k];
                record->mUnk_0a = 0;
                record->mUnk_0b = record->mUnk_04 / 10;
            } else {
                record->mUnk_08 = 0;
                record->mUnk_04 = 0;
                record->mUnk_09 = 0;
                record->mUnk_0a = 3;
                record->mUnk_0b = 0;
            }
        }
        j++;
    } while (j < count);
    return TRUE;
    }
    return FALSE;
}

BOOL func_02084c38(UnkStruct_02078e14 *self) {
    for (u8 i = 0; i < 3; i++) {
        if (self->mUnk_1f1 != 0x15) {
            SLOT_MESSAGE->mUnk_04[i] = data_020d433c.mBattlePlayers->mPlayers[self->mUnk_1f0]->mSlots54[i].mValue;
        } else {
            SLOT_MESSAGE->mUnk_04[i] = self->mUnk_c8[3 + i];
        }
    }
    for (u8 i = 0; i < 3; i++) {
        if (self->mUnk_1f1 != 0x15) {
            SLOT_MESSAGE->mUnk_10[i] = data_020d433c.mBattlePlayers->mPlayers[self->mUnk_1f0]->mSlots6c[i].mValue;
        } else {
            SLOT_MESSAGE->mUnk_10[i] = self->mUnk_c8[6 + i];
        }
    }
    SLOT_MESSAGE->mUnk_1c = 3;
    SLOT_MESSAGE->mUnk_1d = 3;
    if (func_ov007_022631bc(data_020d433c.mBattlePlayers, 4)) {
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

BOOL func_02084e64(UnkStruct_02078e14 *self) {
    data_0220f540.mUnk_04 = MATH_Rand32(&data_020d42e8, 0);
    if (func_ov007_022631bc(data_020d433c.mBattlePlayers, 2)) {
        u8 *info = WirelessManager::Get().func_0202f0cc(0);
        if (info != NULL) {
            func_020219ec(*(u32 *) (info + 4));
        }
        return TRUE;
    }
    return FALSE;
}

BOOL func_02084f40(UnkStruct_02078e14 *self) {
    MIi_CpuCopyFast(data_020d4404, (u8 *) &data_0220f540 + 4, 8);
    if (func_ov007_022631bc(data_020d433c.mBattlePlayers, 6)) {
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

BOOL func_0208501c(UnkStruct_02078e14 *self) {
    for (u8 i = 0; i < 3; i++) {
        if (self->mUnk_1f1 != 0x15) {
            MIi_CpuCopy16(&data_020d433c.mBattlePlayers->mPlayers[self->mUnk_1f0]->mRecords[i], (u8 *) &data_0220f540 + 4 + i * 4, 4);
        } else {
            u8 id;
            u32 kind;
            u32 invalid = 0;
            id   = func_020230dc(data_020d43f0, 0, i);
            kind = func_020230a8(data_020d43f0, 0, i);
            if (id >= 0x29) {
                invalid = 1;
            }
            if (invalid != 0) {
                MI_CpuClear16((u8 *) &data_0220f540 + 4 + i * 4, 4);
            } else {
                MIi_CpuCopy16(func_02023248(data_020d43f0, id, kind), (u8 *) &data_0220f540 + 4 + i * 4, 4);
            }
        }
    }
    if (func_ov007_022631bc(data_020d433c.mBattlePlayers, 5)) {
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

void UnkStruct_02078e14::func_020851d0() {
    func_0207dea4();
    func_0207eec4();
    func_0208403c();
    if (mUnk_1ec != NULL && mUnk_1a1 == 0) {
        mUnk_1ec->func_020658d0();
    }
    if (mUnk_e1 < 0x17) {
        func_0207f2f0();
    }
}

void func_02085220(UnkStruct_02078e14 *self, u32 mode) {
    u8 i;
    u8 flag  = 1;
    u8 state = 0;
    for (i = 0; i < 5; i++) {
        self->func_02081994(1, i, 0x8000);
    }
    state = 0;
    switch (mode) {
        case 0:
            self->mUnk_404                      = 0x11;
            self->mUnk_50->mUnk_064             = 1;
            self->mUnk_50->mCursor->mSelection = state;
            self->mUnk_50->mCells[0]->SetAnimation(1);
            self->func_02081994(3, state, 8);
            self->func_02081994(3, 1, 8);
            self->mUnk_48->mUnk_064 = state;
            self->mUnk_1a1          = mode;
            self->mUnk_e1           = 9;
            return;
        case 0xb:
            state = 1;
            flag  = 0;
            break;
        case 0xc:
            break;
    }
    GXS_SetVisiblePlane(0x13);
    self->mUnk_50->mUnk_064 = 0;
    self->func_02081994(3, 0, 0x8000);
    self->func_02081994(3, 1, 0x8000);
    self->mUnk_48->func_0205c7c8(-1);
    self->mUnk_48->mUnk_064             = 1;
    self->mUnk_48->mButtons[0].mEnabled = 1;
    switch (state) {
        case 0:
            self->func_02081994(1, 3, 1);
            self->mUnk_48->mCursor->mSelection = 3;
            self->mUnk_48->mCells[3]->SetAnimation(1);
            self->mUnk_48->mCells[0]->SetAnimation(9);
            break;
        case 1:
            self->func_02081994(1, 1, 1);
            self->func_02081994(1, 2, 2);
            if (flag != 0) {
                self->mUnk_48->mCursor->mSelection = 1;
                self->mUnk_48->mCells[1]->SetAnimation(4);
            } else {
                self->mUnk_48->mCursor->mSelection = 2;
                self->mUnk_48->mCells[2]->SetAnimation(7);
            }
            self->mUnk_48->mCells[0]->SetAnimation(10);
            break;
    }
    self->mUnk_1a1 = mode;
    func_020839a0(self);
    self->mUnk_e1 = 0x14;
}

void UnkStruct_02078e14::func_02085460(s32 unk1) {
    if (unk1 >= 0) {
        mUnk_1ec->func_02065890(unk1, 0, 1);
        return;
    }
    switch (mUnk_e0) {
        case 0:
        case 1:
        case 2:
        case 4:
            break;
        case 3:
        case 5:
            if (mUnk_e6 == 0) {
                if (mUnk_f0 != 0) {
                    mUnk_1ec->func_02065890(6, 0, 1);
                } else {
                    mUnk_1ec->func_02065890((s16) (mUnk_fa + 7), 0, 1);
                }
            } else {
                mUnk_1ec->func_02065890((s16) (mUnk_e6 + 8), 0, 1);
            }
            break;
    }
}

void func_0208550c(UnkStruct_02078e14 *self) {
    u32 found = 0;
    u8 i;
    u8 j;
    self->mUnk_3f2 = 0;
    for (i = 0; i < 0x29; i++) {
        for (j = 0; j < 6; j++) {
            if (func_020231f4(data_020d43f0, 0, i, j) != 0) {
                if ((u8) (i + 0xdc) <= 1) {
                    if (j == data_020d43f0[0].mUnk_1a_8) {
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
            u32 va = func_020231f4(data_020d43f0, 0, (u8) (a / 6), (u8) (a % 6));
            u32 vb = func_020231f4(data_020d43f0, 0, (u8) (b / 6), (u8) (b % 6));
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
    if (self->mUnk_1f2[0] == 0x50 && MATH_Rand32(&data_020d42e8, 100) > 0x32) {
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

void func_020858d4(UnkStruct_02078e14 *self) {
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

BOOL func_0208596c(UnkStruct_02078e14 *self, u32 unk1, u32 unk2) {
    u8 value = unk1 - 1;
    if (unk1 >= 7) {
        value = value + 1;
    }
    return unk2 == value ? 1 : 0;
}

void func_02085990(UnkStruct_02078e14 *self) {
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
                if (wanted == func_02065c0c(self->mUnk_1f2[c], 3) && func_02085a7c(self, self->mUnk_1f2[c], (u8) self->mUnk_c8[9 + b])) {
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

BOOL func_02085a7c(UnkStruct_02078e14 *self, u32 unk1, u32 unk2) {
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

void func_02085b8c(UnkStruct_02078e14 *self) {
    u8 c;
    u8 a = 0;
    do {
        c = 0;
        do {
            if (func_02065c0c(self->mUnk_1f2[c], 3) == 5 && func_02085da8(self, self->mUnk_1f2[c], (u8) self->mUnk_c8[a], (u8) self->mUnk_c8[9 + a])) {
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
            if (func_02065c0c(self->mUnk_1f2[c2], 3) == 4 && func_02085da8(self, self->mUnk_1f2[c2], (u8) self->mUnk_c8[a2], (u8) self->mUnk_c8[9 + a2])) {
                self->mUnk_c8[7] = self->mUnk_1f2[c2];
                return;
            }
            c2++;
        } while (c2 < self->mUnk_3f2);
        a2++;
    } while (a2 < 3 && self->mUnk_3f4 == 0);
}

BOOL func_02085da8(UnkStruct_02078e14 *self, u32 unk1, u32 unk2, u32 unk3) {
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

void func_02085fec(UnkStruct_02078e14 *self) {
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

BOOL func_02086044(UnkStruct_02078e14 *self, u32 unk1) {
    switch (unk1) {
        case 0xbe:
            if (self->mUnk_3f4 != 0) {
                return FALSE;
            }
            if (func_020861f4(self, 0) && func_020861f4(self, 4)) {
                return TRUE;
            }
            break;
        case 0xbf:
            if (self->mUnk_3f4 != 0) {
                return FALSE;
            }
            if (func_020861f4(self, 5) && func_020861f4(self, 3)) {
                return TRUE;
            }
            break;
        case 0xc0:
            if (self->mUnk_3f4 != 0) {
                return FALSE;
            }
            if (func_020861f4(self, 2) && func_020861f4(self, 1)) {
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
            if (func_020861f4(self, 0) && func_020861f4(self, 1) && func_020861f4(self, 3)) {
                return TRUE;
            }
            break;
        case 0xc8:
            if (self->mUnk_3f4 != 0) {
                return FALSE;
            }
            if (func_020861f4(self, 2) && func_020861f4(self, 4) && func_020861f4(self, 5)) {
                return TRUE;
            }
            break;
        default:
            return TRUE;
    }
    return FALSE;
}

BOOL func_020861f4(UnkStruct_02078e14 *self, u32 unk1) {
    u8 i = 0;
    do {
        if (unk1 == self->mUnk_c8[9 + i]) {
            return TRUE;
        }
        i++;
    } while (i < 3);
    return FALSE;
}

u32 UnkStruct_02078e14::func_02086224(u32 unk1, u32 unk2) {
    UnkStruct_02023248 *pair = func_02023248(data_020d43f0, unk1, unk2);
    if ((s32) pair->mUnk_00_0 >= 9) {
        return 10000000;
    }
    u32 cost = func_020864ec(unk1, unk2);
    if (cost == 0) {
        cost = 5000;
    }
    u32 price = cost * data_020a3174[pair->mUnk_00_0] / 100;
    return price - price * pair->mPad / ((pair->mUnk_00_0 + 1) * 300);
}

BOOL UnkStruct_02078e14::func_020862d0() {
    switch (mUnk_f5) {
        case 2:
            if (mUnk_e8 <= mUnk_f4) {
                return TRUE;
            }
            if (InputManager::Get().IsDown(0x20) != 0) {
                return FALSE;
            }
            break;
        case 4:
            if (mUnk_e8 >= mUnk_e9 - 1) {
                return TRUE;
            }
            if (InputManager::Get().IsDown(0x10) != 0) {
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

s32 UnkStruct_02078e14::func_0208644c() {
    u16 total = 0;
    u8 i      = 0;
    do {
        if (mUnk_1f1 != 0x15) {
            total += data_020d433c.mBattlePlayers->mPlayers[mUnk_1f0]->mRecords[i].mUnk_04;
        } else {
            total += func_020231f4(data_020d43f0, 0, (u8) mUnk_c8[i], (u8) mUnk_c8[9 + i]);
        }
        i++;
    } while (i < 3 && mUnk_3f4 == 0);
    return total;
}

u32 UnkStruct_02078e14::func_020864ec(u32 unk1, u32 unk2) {
    u32 index = func_02086654(this, unk1, unk2);
    if (data_020d43f0[0].mUnk_5bf_0 == 0 && index == 0x24) {
        return *(const u16 *) (data_020a3860 + index * 10) * 10;
    }
    return data_020a338c[index].mUnk_00 * 10;
}

u32 UnkStruct_02078e14::func_02086548(u32 unk1, u32 unk2) {
    u32 index = func_02086654(this, unk1, unk2);
    return (u16) (data_020a338c[index].mUnk_02 * 10);
}

u32 UnkStruct_02078e14::func_02086570(u32 unk1, u32 unk2) {
    func_02086654(this, unk1, unk2);
    return 0;
}

void func_02086580(UnkStruct_02078e14 *self, u32 unk1, u32 unk2, BattleCellPair *out) {
    u32 index       = func_02086654(self, unk1, unk2);
    out->mUnk_00_0  = 0;
    out->mUnk_00_4  = 0;
    out->mUnk_02_0  = data_020a35b8[index][0];
    out->mUnk_02_12 = data_020a35b8[index][1];
    out->mUnk_02_3  = data_020a35b8[index][2];
    out->mUnk_02_6  = data_020a35b8[index][3];
    out->mUnk_02_9  = data_020a35b8[index][4];
}

u32 func_02086654(UnkStruct_02078e14 *self, u32 unk1, u32 unk2) {
    if (unk1 == 0x22 && unk2 == 0) {
        return 0x28;
    }
    return unk1;
}

void func_02086668(UnkStruct_02078e14 *self, u32 unk1) {
    u8 s;
    u8 t;
    for (s = 0; s < self->mUnk_fc - 1; s++) {
        for (t = s + 1; t < self->mUnk_fc; t++) {
            u16 a;
            u16 b;
            u32 vb;
            u32 va;
            if (unk1 != 0) {
                b  = self->mUnk_106[t];
                a  = self->mUnk_106[s];
                // Placeholder: the casts keep the member offset in the literal-pool word, like an access through a symbol of
                // its own (plain .mOrder[b] changes the code).
                vb = ((const u8 *) data_020a3210.mOrder)[b];
                va = ((const u8 *) data_020a3210.mOrder)[a];
                if (va > vb) {
                    self->mUnk_106[s] = b;
                    self->mUnk_106[t] = a;
                }
            } else {
                b  = self->mUnk_106[t];
                a  = self->mUnk_106[s];
                vb = Table_020a3ba8()[b];
                va = Table_020a3ba8()[a];
                if (va > vb) {
                    self->mUnk_106[s] = b;
                    self->mUnk_106[t] = a;
                }
            }
        }
    }
}

void func_0208671c(UnkStruct_02078e14 *self, u32 unk1) {
    self->mUnk_3f2 = 0;
    for (u16 k = 1; k < 0xd5; k++) {
        if (func_02065c20(k) < 0x2bc && TestFlagBit(data_020d4946, k) && func_02065c0c(k, 1) == unk1) {
            self->mUnk_1f2[self->mUnk_3f2] = k;
            self->mUnk_3f2++;
        }
    }
    func_020867b8(self);
}

void func_020867b8(UnkStruct_02078e14 *self) {
    u8 i;
    for (i = 0; i < self->mUnk_3f2 - 1; i++) {
        u16 value = self->mUnk_1f2[i];
        u16 j     = i + MATH_Rand32(&data_020d42e8, self->mUnk_3f2 - i);
        self->mUnk_1f2[i] = self->mUnk_1f2[j];
        self->mUnk_1f2[j] = value;
    }
}

void func_02086874(UnkStruct_02078e14 *self) {
    u8 s;
    u8 t;
    for (s = 0; s < self->mUnk_3f2 - 1; s++) {
        for (t = s + 1; t < self->mUnk_3f2; t++) {
            u32 va = func_02065bf4(self->mUnk_1f2[s], (u8) data_020d43f0[0].mUnk_1a_8);
            u32 vb = func_02065bf4(self->mUnk_1f2[t], (u8) data_020d43f0[0].mUnk_1a_8);
            if (va < vb) {
                u16 tmp           = self->mUnk_1f2[s];
                self->mUnk_1f2[s] = self->mUnk_1f2[t];
                self->mUnk_1f2[t] = tmp;
            }
        }
    }
}
