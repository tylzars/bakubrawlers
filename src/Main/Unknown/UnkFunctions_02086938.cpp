#include "Game/UnkStruct_020d43f0.hpp"
#include "Game/UnkStruct_020d48f0.hpp"

// Bit tests written as macros (as in MyroomTipsScene): unlike the TestFlagBit/SetFlagBit inlines they let mwcc reuse the
// loaded byte for the following set, and they colour the registers of func_020875ac's loops like the original.
#define TEST_BIT(flags, index) ((flags)[(index) / 8] & (1 << ((index) % 8)))
#define SET_BIT(flags, index) ((flags)[(index) / 8] |= 1 << ((index) % 8))

extern u8 data_020d4946[];

extern u8 data_020d4976[];

extern u8 data_020d497b[];

extern u8 data_020d499c[];

extern u8 data_020d49a6[];

extern u8 data_020d496a[];

extern u8 data_020d496d[];

extern u8 data_020d4970[];

s32 func_02087468();

BOOL func_020875ac(u32 index);

BOOL UnlockProgressEntry(u32 index);

#define VD (((volatile UnkStruct_020d43f0 *) data_020d43f0)[0])

static inline BOOL StageReached(u32 greater, u32 equal, u32 sub) {
    u8 stage = VD.mUnk_42;
    return (stage > greater) | ((stage == equal) & (VD.mUnk_43 == sub));
}


#define SET_FLAG_LOOP(first, last)                                      \
    {                                                                   \
        u8 i = first;                                                   \
        do {                                                            \
            u8 byte = data_020d499c[(u32) i >> 3];                      \
            if (!(byte & (1 << (i % 8)))) {                             \
                data_020d499c[(u32) i >> 3] = byte | (1 << (i % 8));   \
                result                      = TRUE;                     \
            }                                                           \
            i++;                                                        \
        } while (i <= last);                                            \
    }

// One unlock: an id and the level it is unlocked at. Lists of them end with {0xff, 0xff}. As two-byte structs (not byte
// arrays) the lists share one section and sit two bytes apart, as in the original.
struct UnlockPair {
    /* 0 */ u8 mId;
    /* 1 */ u8 mLevel;
};

struct UnlockTable {
    /* 0 */ u32 mCount;
    /* 4 */ const UnlockPair *const *mLists;
};

struct FlagList8 {
    /* 0 */ u32 mCount;
    /* 4 */ const u8 *mList;
};

struct FlagList16 {
    /* 0 */ u32 mCount;
    /* 4 */ const u16 *mList;
};

struct ProgressEntry {
    /* 0 */ const void *mList;
    /* 4 */ u8 mType;
    /* 5 */ u8 mCount;
    /* 6 */ PAD(0x6, 0x8);
};

BOOL UnlockStoreItems();
BOOL UnlockCharacters();
BOOL func_02086b74();
BOOL func_02086d48();
BOOL func_02086e08();
s32 func_02087100();
s32 func_02087468();
BOOL func_020875ac(u32 index);
BOOL UnlockProgressEntry(u32 index);

// mwcc emits a file's objects sorted by size, byte arrays and the inline accessors' tables as sections of their own;
// the order of equal sizes follows from the whole declaration sequence. This definition order gives the original
// layout of .rodata and .data.
static const UnlockPair data_020a3db2[1] = {{0xff, 0xff}};
static const u8 data_020a3d88[1] = {38};
static const UnlockPair data_020a3dac[1] = {{7, 2}};
static const UnlockPair data_020a3df6[3] = {{7, 1}, {6, 2}, {0xff, 0xff}};

static const u16 data_020a3f66[21] = {
    33, 35, 46, 50, 63, 68, 71, 60, 99, 95, 86, 109, 117, 133, 123, 141,
    176, 184, 197, 191, 25,
};

static const UnlockPair data_020a3db4[1] = {{0xff, 0xff}};
static const UnlockPair data_020a3e02[3] = {{8, 0}, {12, 0}, {0xff, 0xff}};
static const u16 data_020a3e3e[8] = {3, 4, 17, 81, 148, 149, 160, 187};
static const UnlockPair data_020a3db6[1] = {{0xff, 0xff}};
static const u8 data_020a3d78[1] = {10};
static const UnlockPair data_020a3e08[3] = {{10, 0}, {11, 0}, {0xff, 0xff}};
static const u8 data_020a3d9c[1] = {30};
static const UnlockPair data_020a3db8[1] = {{0xff, 0xff}};
static const UnlockPair data_020a3e0e[4] = {{14, 1}, {13, 1}, {8, 1}, {0xff, 0xff}};
static const UnlockPair data_020a3e30[7] = {{9, 1}, {3, 1}, {14, 2}, {13, 2}, {15, 2}, {16, 2}, {0xff, 0xff}};
static const u8 data_020a3d68[1] = {4};
static const UnlockPair data_020a3dae[1] = {{0xff, 0xff}};
static const u8 data_020a3fd8[2] = {13, 37};
static const u8 data_020a3d74[1] = {0};
static const u16 data_020a3ed4[15] = {26, 36, 51, 54, 89, 96, 87, 107, 115, 131, 125, 162, 177, 194, 195};
static const UnlockPair data_020a3dbc[1] = {{0xff, 0xff}};
static const u16 data_020a3eb8[14] = {30, 55, 57, 65, 94, 88, 104, 112, 128, 124, 190, 193, 181, 198};
static const UnlockPair data_020a3dbe[1] = {{0xff, 0xff}};
static const u8 data_020a3fd4[2] = {35, 25};

static const u16 data_020a3f90[31] = {
    19, 20, 21, 22, 23, 40, 41, 42, 43, 44, 45, 85, 93, 102, 72, 77,
    98, 110, 134, 118, 126, 120, 143, 144, 145, 140, 168, 169, 170, 171, 34,
};

static const u8 data_020a3d6c[1] = {26};
static const u8 data_020a3d84[1] = {16};
static const UnlockPair data_020a3dba[1] = {{0xff, 0xff}};
static const u8 data_020a3d5c[1] = {39};
static const UnlockPair data_020a3e26[5] = {{14, 0}, {13, 0}, {0, 0}, {1, 0}, {0xff, 0xff}};
static const UnlockPair *data_020ba9d0[1] = {data_020a3dbc};
static const UnlockPair data_020a3de4[3] = {{8, 2}, {4, 2}, {0xff, 0xff}};
static const UnlockPair data_020a3dc4[2] = {{15, 1}, {0xff, 0xff}};
static const UnlockPair data_020a3dc8[2] = {{12, 1}, {0xff, 0xff}};
static const UnlockPair data_020a3dcc[2] = {{10, 2}, {0xff, 0xff}};
static const UnlockPair data_020a3dd0[2] = {{9, 2}, {0xff, 0xff}};
static const UnlockPair *data_020ba9cc[1] = {data_020a3e26};
static const u8 data_020a3da8[1] = {12};
static const UnlockPair data_020a3db0[1] = {{0xff, 0xff}};
static const UnlockPair *data_020ba9f0[4] = {data_020a3e30, data_020a3dc8, data_020a3dcc, data_020a3dd0};
static const UnlockPair data_020a3ddc[2] = {{0, 2}, {0xff, 0xff}};
static const UnlockPair data_020a3de0[2] = {{2, 0}, {0xff, 0xff}};
static const UnlockPair data_020a3dea[3] = {{11, 2}, {1, 2}, {0xff, 0xff}};
static const UnlockPair data_020a3df0[3] = {{12, 2}, {3, 2}, {0xff, 0xff}};
static const u16 data_020a3ea2[11] = {1, 2, 16, 73, 74, 84, 146, 147, 159, 186, 183};
static const UnlockPair data_020a3dfc[3] = {{15, 0}, {16, 0}, {0xff, 0xff}};
static const UnlockPair *data_020baa00[4] = {NULL, data_020a3dfc, data_020a3e02, data_020a3e08};
static const u8 data_020a3d94[1] = {7};
static const u8 data_020a3d7c[1] = {3};
static const UnlockPair data_020a3e16[4] = {{9, 0}, {11, 1}, {10, 1}, {0xff, 0xff}};
static const UnlockPair data_020a3e1e[4] = {{2, 1}, {1, 1}, {4, 1}, {0xff, 0xff}};
static const UnlockPair *data_020ba9d4[3] = {NULL, data_020a3db8, data_020a3db6};
static const u8 data_020a3d70[1] = {11};
static const u8 data_020a3d98[1] = {14};
static const u16 data_020a3e76[11] = {37, 38, 39, 78, 111, 119, 127, 135, 163, 164, 165};
static const u16 data_020a3e4e[10] = {9, 10, 18, 83, 79, 153, 154, 189, 182, 161};
static const UnlockPair data_020a3dc0[2] = {{16, 1}, {0xff, 0xff}};
static const u8 data_020a3fd0[2] = {28, 29};
static const UnlockPair *data_020ba9c8[1] = {data_020a3de0};
static const u16 data_020a3e8c[11] = {7, 8, 14, 66, 80, 151, 152, 157, 185, 172, 178};
static const u8 data_020a3d8c[1] = {9};
static const u8 data_020a3da0[1] = {1};
static const UnlockPair data_020a3dd4[2] = {{0, 1}, {0xff, 0xff}};

static const u16 data_020a3f3c[21] = {
    27, 31, 47, 56, 58, 49, 90, 92, 101, 105, 113, 129, 121, 137, 138, 139,
    136, 196, 192, 199, 24,
};

static const u8 data_020a3d60[1] = {2};
static const UnlockPair data_020a3dd8[2] = {{2, 2}, {0xff, 0xff}};
static const u8 data_020a3d80[1] = {18};
static const u8 data_020a3d54[1] = {5};
static const u8 data_020a3d90[1] = {36};
static const u8 data_020a3fdc[3] = {24, 27, 31};
static const UnlockPair *data_020ba9e0[4] = {NULL, data_020a3e0e, data_020a3e16, data_020a3dbe};
static const u16 data_020a3e62[10] = {5, 6, 15, 64, 82, 75, 150, 158, 174, 166};
static const u8 data_020a3da4[1] = {8};

static const UnlockPair *data_020baa10[6] = {
    data_020a3e1e, data_020a3dc4, data_020a3dc0, data_020a3dba, data_020a3dae,
    data_020a3db0,
};

static const UnlockPair *data_020baa28[10] = {
    data_020a3dd4, data_020a3dd8, data_020a3de4, data_020a3dea, data_020a3df0,
    data_020a3ddc, data_020a3db2, data_020a3db4, NULL, NULL,
};

static const u8 data_020a3d58[1] = {17};
static const u8 data_020a3d64[1] = {15};
static const u8 data_020a3fe0[8] = {19, 20, 21, 22, 23, 33, 32, 34};
static const UnlockPair *data_020ba9c4[1] = {data_020a3df6};

static const u16 data_020a3f16[19] = {
    11, 12, 13, 53, 61, 62, 69, 70, 100, 103, 108, 116, 132, 155, 156, 188,
    173, 180, 167,
};

static const u16 data_020a3ef2[18] = {28, 32, 29, 48, 52, 59, 67, 97, 91, 76, 106, 114, 130, 122, 142, 175, 179, 200};

// The four tables come after the byte lists in the original, so they are sections of their own: function-local
// statics of inline accessors.
inline const FlagList8 *Table_020a4040() {
    static const FlagList8 data_020a4040[11] = {
        {8, data_020a3fe0},
        {0, NULL},
        {3, data_020a3fdc},
        {0, NULL},
        {2, data_020a3fd0},
        {0, NULL},
        {0, NULL},
        {2, data_020a3fd4},
        {1, data_020a3d9c},
        {1, data_020a3d6c},
        {2, data_020a3fd8},
    };
    return data_020a4040;
}

inline const FlagList16 *Table_020a3fe8() {
    static const FlagList16 data_020a3fe8[11] = {
        {31, data_020a3f90},
        {0, NULL},
        {21, data_020a3f3c},
        {0, NULL},
        {18, data_020a3ef2},
        {0, NULL},
        {0, NULL},
        {21, data_020a3f66},
        {14, data_020a3eb8},
        {15, data_020a3ed4},
        {11, data_020a3e76},
    };
    return data_020a3fe8;
}

inline const UnlockTable *Table_020a4098() {
    static const UnlockTable data_020a4098[11] = {
        {1, data_020ba9cc},
        {3, data_020ba9d4},
        {1, data_020ba9c8},
        {4, data_020baa00},
        {1, data_020ba9d0},
        {0, NULL},
        {4, data_020ba9e0},
        {6, data_020baa10},
        {4, data_020ba9f0},
        {10, data_020baa28},
        {1, data_020ba9c4},
    };
    return data_020a4098;
}

inline const ProgressEntry *Table_020a40f0() {
    static const ProgressEntry data_020a40f0[27] = {
        {data_020a3f16, 0, 19},
        {data_020a3ea2, 0, 11},
        {data_020a3e8c, 0, 11},
        {data_020a3e3e, 0, 8},
        {data_020a3e4e, 0, 10},
        {data_020a3e62, 0, 10},
        {data_020a3d84, 1, 1},
        {data_020a3d64, 1, 1},
        {data_020a3d98, 1, 1},
        {data_020a3d58, 1, 1},
        {data_020a3d80, 1, 1},
        {data_020a3d74, 1, 1},
        {data_020a3d68, 1, 1},
        {data_020a3d94, 1, 1},
        {data_020a3d60, 1, 1},
        {data_020a3d70, 1, 1},
        {data_020a3d8c, 1, 1},
        {data_020a3da0, 1, 1},
        {data_020a3d7c, 1, 1},
        {data_020a3d54, 1, 1},
        {data_020a3da4, 1, 1},
        {data_020a3da8, 1, 1},
        {data_020a3d78, 1, 1},
        {data_020a3d90, 1, 1},
        {data_020a3d88, 1, 1},
        {data_020a3d5c, 1, 1},
        {data_020a3dac, 2, 2},
    };
    return data_020a40f0;
}



// The (u8 *) casts give each flag array its own literal-pool word, hoisted after the count table as in the original
// (through a pointer local it is hoisted first and trades registers with the constant 1; a plain member folds into the
// data_020d43f0 base). Unlike the data_020d4976/data_020d497b externs, the cast form still aliases data_020d43f0, which
// keeps the mUnk_5b9 load after the flag store.
BOOL UnlockStoreItems() {
    BOOL result;
    u8 kind;
    u8 i;
    const u16 *list2;
    const u8 *list;
    result = FALSE;
    kind   = data_020d43f0[0].mUnk_42;
    list   = Table_020a4040()[kind].mList;
    if (list != NULL) {
        i = 0;
        do {
            if (!TEST_BIT((u8 *) data_020d43f0[0].mUnk_586, list[i])) {
                SET_BIT((u8 *) data_020d43f0[0].mUnk_586, list[i]);
                data_020d43f0[0].mUnk_5b9_0 = 1;
                result                      = TRUE;
            }
            i++;
        } while (i < Table_020a4040()[kind = data_020d43f0[0].mUnk_42].mCount);
    }
    list2 = Table_020a3fe8()[kind].mList;
    if (list2 != NULL) {
        i = 0;
        do {
            if (!TEST_BIT((u8 *) data_020d43f0[0].mUnk_58b, list2[i])) {
                SET_BIT((u8 *) data_020d43f0[0].mUnk_58b, list2[i]);
                data_020d43f0[0].mUnk_5b9_0 = 1;
                result                      = TRUE;
            }
            i++;
        } while (i < Table_020a3fe8()[kind = data_020d43f0[0].mUnk_42].mCount);
    }
    return result;
}

BOOL UnlockCharacters() {
    BOOL result = FALSE;
    const UnlockTable *table = &Table_020a4098()[data_020d43f0[0].mUnk_42];
    u8 sub      = data_020d43f0[0].mUnk_43;
    if (table->mCount <= sub) {
        return result;
    }
    const u8 *list = (const u8 *) table->mLists[sub];
    if (list != NULL && list[0] != 0xff) {
    do {
        u8 id = list[0];
        if (!TestFlagBit(data_020d496a + list[1] * 3, id)) {
            u32 index = (u32) id >> 3;
            if (!((data_020d496a[index] | data_020d496d[index] | data_020d4970[index]) & (1 << (id % 8)))) {
                result = TRUE;
            }
            data_020d496a[index] |= 1 << (id % 8);
            if (list[1] >= 1) {
                data_020d496d[(u32) list[0] >> 3] |= 1 << (list[0] % 8);
            }
            if (list[1] >= 2) {
                data_020d4970[(u32) list[0] >> 3] |= 1 << (list[0] % 8);
            }
        }
        list += 2;
    } while (list[0] != 0xff);
    }
    return result;
}

BOOL func_02086b74() {
    BOOL result = FALSE;
    if (!(data_020d43f0[0].mUnk_578 & 0x1)) {
        result = TRUE;
        ((volatile u8 *) data_020d4968)[0] |= 0x1;
    }
    if (!(data_020d43f0[0].mUnk_578 & 0x80)) {
        result = TRUE;
        ((volatile u8 *) data_020d4968)[0] |= 0x80;
    }
    if (data_020d43f0[0].mUnk_42 >= 4 && !(data_020d43f0[0].mUnk_578 & 0x4)) {
        result = TRUE;
        ((volatile u8 *) data_020d4968)[0] |= 0x4;
    }
    if (data_020d43f0[0].mUnk_42 >= 7 && !(data_020d43f0[0].mUnk_578 & 0x10)) {
        result = TRUE;
        ((volatile u8 *) data_020d4968)[0] |= 0x10;
    }
    if (data_020d43f0[0].mUnk_42 >= 8 && !(data_020d43f0[0].mUnk_578 & 0x8)) {
        result = TRUE;
        ((volatile u8 *) data_020d4968)[0] |= 0x8;
    }
    u8 stage = data_020d43f0[0].mUnk_42;
    if ((stage > 8) | ((stage == 8) & (data_020d43f0[0].mUnk_43 == 4))) {
        if (!(data_020d43f0[0].mUnk_578 & 0x2)) {
            result = TRUE;
            ((volatile u8 *) data_020d4968)[0] |= 0x2;
        }
    }
    stage = data_020d43f0[0].mUnk_42;
    if ((stage > 9) | ((stage == 9) & (data_020d43f0[0].mUnk_43 == 9))) {
        if (!(data_020d43f0[0].mUnk_578 & 0x20)) {
            result = TRUE;
            ((volatile u8 *) data_020d4968)[0] |= 0x20;
        }
        if (!(data_020d43f0[0].mUnk_578 & 0x40)) {
            result = TRUE;
            ((volatile u8 *) data_020d4968)[0] |= 0x40;
        }
    }
    return result;
}

BOOL func_02086d48() {
    BOOL result = FALSE;
    u32 level   = 0;
    u16 points  = data_020d48f0.mUnk_ba;
    if (points <= 500) {
        level = 1;
    }
    if (points <= 100) {
        level = 2;
    }
    if (points <= 30) {
        level = 3;
    }
    if (points <= 10) {
        level = 4;
    }
    if (points <= 3) {
        level = 5;
    }
    if (points <= 1) {
        if (data_020d43f0[0].mUnk_5bf_0 != 0) {
            level = 6;
        }
    }
    u8 count = 0;
    u8 i     = 0;
    do {
        if (TestFlagBit(data_020d49a6, i)) {
            count++;
        }
        i++;
    } while (i <= 9);
    if (count == 10) {
        level = 8;
    }
    if (data_020d43f0[0].mUnk_5b8 < level) {
        data_020d43f0[0].mUnk_5b8 = level;
        result                    = TRUE;
    }
    return result;
}

BOOL func_02086e08() {
    BOOL result = FALSE;
    if (VD.mUnk_42 != 0) {
        SET_FLAG_LOOP(0, 3)
        SET_FLAG_LOOP(10, 13)
        if (VD.mUnk_43 != 0) {
            if (!(VD.mUnk_5ac & 0x10)) {
                result = TRUE;
                ((volatile u8 *) data_020d499c)[0] |= 0x10;
            }
        }
        if (VD.mUnk_43 > 1) {
            if (!(VD.mUnk_5ac & 0x20)) {
                result = TRUE;
                ((volatile u8 *) data_020d499c)[0] |= 0x20;
            }
            if (!(VD.mUnk_5ac & 0x40)) {
                result = TRUE;
                ((volatile u8 *) data_020d499c)[0] |= 0x40;
            }
        }
    }
    if (VD.mUnk_42 > 1) {
        if (!(VD.mUnk_5ac & 0x80)) {
            result = TRUE;
            ((volatile u8 *) data_020d499c)[0] |= 0x80;
        }
        if (!(VD.mUnk_5ad & 0x40)) {
            result = TRUE;
            ((volatile u8 *) data_020d499c)[1] |= 0x40;
        }
        if (!(VD.mUnk_5ad & 0x80)) {
            result = TRUE;
            ((volatile u8 *) data_020d499c)[1] |= 0x80;
        }
    }
    if (VD.mUnk_42 > 2) {
        if (!(VD.mUnk_5ad & 0x1)) {
            result = TRUE;
            ((volatile u8 *) data_020d499c)[1] |= 0x1;
        }
    }
    if (VD.mUnk_42 > 3) {
        SET_FLAG_LOOP(0x10, 0x1a)
    }
    if (VD.mUnk_42 > 5 && VD.mUnk_43 > 2) {
        if (!(VD.mUnk_5ad & 0x2)) {
            result = TRUE;
            ((volatile u8 *) data_020d499c)[1] |= 0x2;
        }
    }
    if (VD.mUnk_42 > 6) {
        SET_FLAG_LOOP(0x1b, 0x20)
    }
    if (VD.mUnk_42 > 7) {
        SET_FLAG_LOOP(0x21, 0x26)
    }
    return result;
}

s32 func_02087100() {
    s32 result = -1;
    if (StageReached(1, 1, 2)) {
        if (!(VD.mUnk_5b6 & 0x40)) {
            result = 6;
            ((volatile u8 *) data_020d49a6)[0] |= 0x40;
        }
    }
    if (result < 0) {
        if (StageReached(3, 3, 3)) {
            if (!(VD.mUnk_5b6 & 0x2)) {
                result = 1;
                ((volatile u8 *) data_020d49a6)[0] |= 0x2;
            }
        }
    }
    if (result < 0) {
        if (StageReached(9, 6, 2)) {
            if (!(VD.mUnk_5b6 & 0x10)) {
                result = 4;
                ((volatile u8 *) data_020d49a6)[0] |= 0x10;
            }
        }
    }
    if (result < 0) {
        if (StageReached(7, 7, 4)) {
            if (!(VD.mUnk_5b6 & 0x20)) {
                result = 5;
                ((volatile u8 *) data_020d49a6)[0] |= 0x20;
            }
        }
    }
    if (result < 0) {
        if (StageReached(8, 8, 3)) {
            if (!(VD.mUnk_5b6 & 0x1)) {
                result = 0;
                ((volatile u8 *) data_020d49a6)[0] |= 0x1;
            }
        }
    }
    if (result < 0) {
        if (StageReached(9, 9, 6)) {
            if (!(VD.mUnk_5b6 & 0x4)) {
                result = 2;
                ((volatile u8 *) data_020d49a6)[0] |= 0x4;
            }
        }
    }
    if (result < 0) {
        u8 stage = VD.mUnk_42;
        if ((stage > 6) | ((stage == 6) & (VD.mUnk_43 == 2)) | VD.mUnk_5bf_0) {
            if (data_020d48f0.mUnk_ba <= 1) {
                if (!(VD.mUnk_5b6 & 0x80)) {
                    result = 7;
                    ((volatile u8 *) data_020d49a6)[0] |= 0x80;
                }
            }
        }
    }
    if (result < 0) {
        if (VD.mUnk_5bf_0 != 0) {
            if (!(VD.mUnk_5b6 & 0x8)) {
                result = 3;
                ((volatile u8 *) data_020d49a6)[0] |= 0x8;
            }
        }
    }
    if (result < 0) {
        result = func_02087468();
    }
    return result;
}

s32 func_02087468() {
    s32 result = -1;
    if (!(data_020d43f0[0].mUnk_5b7 & 0x1)) {
        u16 count = 0;
        u16 i     = 0;
        do {
            u16 j = 0;
            do {
                if (func_020231f4(data_020d43f0, 0, i, j) != 0) {
                    count++;
                    break;
                }
                j++;
            } while (j < 6);
            i++;
        } while (i < 0x29);
        if (count >= (u16) (data_020d43f0[0].mUnk_5d2 + 0x24)) {
            result = 8;
            ((u8 *) data_020d49a6)[1] |= 0x1;
        }
    }
    if (result < 0) {
        if (!(data_020d43f0[0].mUnk_5b7 & 0x2)) {
            u16 count = 0;
            u16 i     = 0;
            do {
                if (TestFlagBit(data_020d4946, i)) {
                    count++;
                }
                i++;
            } while (i < 0x102);
            if (count >= 200) {
                result = 9;
                ((u8 *) data_020d49a6)[1] |= 0x2;
            }
        }
    }
    return result;
}

BOOL func_020875ac(u32 index) {
    BOOL result = TRUE;
    u8 i;
    switch (Table_020a40f0()[index].mType) {
        case 0: {
            i               = 0;
            const u16 *list = (const u16 *) Table_020a40f0()[index].mList;
            do {
                if (!TEST_BIT(data_020d497b, list[i])) {
                    result = FALSE;
                    break;
                }
                i++;
            } while (i < Table_020a40f0()[index].mCount);
            break;
        }
        case 1: {
            const u8 *list = (const u8 *) Table_020a40f0()[index].mList;
            i              = 0;
            do {
                if (!TEST_BIT(data_020d4976, list[i])) {
                    result = FALSE;
                    break;
                }
                i++;
            } while (i < Table_020a40f0()[index].mCount);
            break;
        }
        case 2: {
            const u8 *list = (const u8 *) Table_020a40f0()[index].mList;
            i              = 0;
            do {
                if (!TEST_BIT(data_020d496a + list[1] * 3, list[0])) {
                    result = FALSE;
                }
                i++;
            } while (i < Table_020a40f0()[index].mCount);
            break;
        }
    }
    return result;
}

BOOL UnlockProgressEntry(u32 index) {
    BOOL result = TRUE;
    switch (Table_020a40f0()[index].mType) {
        case 0: {
            u8 i            = 0;
            const u16 *list = (const u16 *) Table_020a40f0()[index].mList;
            do {
                if (!TEST_BIT((u8 *) data_020d43f0[0].mUnk_58b, list[i])) {
                    SET_BIT((u8 *) data_020d43f0[0].mUnk_58b, list[i]);
                    data_020d43f0[0].mUnk_5b9_0 = 1;
                    result                       = TRUE;
                }
                i++;
            } while (i < Table_020a40f0()[index].mCount);
            break;
        }
        case 1: {
            const u8 *list;
            u8 count;
            u8 i;
            const ProgressEntry *e = &Table_020a40f0()[index];
            count = e->mCount;
            list  = (const u8 *) Table_020a40f0()[index].mList;
            i     = 0;
            do {
                if (!TEST_BIT((u8 *) data_020d43f0[0].mUnk_586, list[i])) {
                    SET_BIT((u8 *) data_020d43f0[0].mUnk_586, list[i]);
                    data_020d43f0[0].mUnk_5b9_0 = 1;
                    result                       = TRUE;
                }
                i++;
            } while (i < count);
            break;
        }
        case 2: {
            const u8 *list;
            u8 count;
            u8 i;
            const ProgressEntry *e = &Table_020a40f0()[index];
            count = e->mCount;
            list  = (const u8 *) Table_020a40f0()[index].mList;
            i     = 0;
            do {
                u8 id = list[0];
                if (!((data_020d496a[(u32) id >> 3] | data_020d496d[(u32) id >> 3] | data_020d4970[(u32) id >> 3]) & (1 << (id % 8)))) {
                    result = TRUE;
                }
                (data_020d496a + list[1] * 3)[(u32) id >> 3] |= 1 << (id % 8);
                i++;
            } while (i < count);
            break;
        }
    }
    return result;
}
