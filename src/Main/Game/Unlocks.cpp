#include "Game/SaveData.hpp"
#include "Game/UnkStruct_020d48f0.hpp"

// Bit tests written as macros (as in MyroomTipsScene): unlike the TestFlagBit/SetFlagBit inlines they let mwcc reuse the
// loaded byte for the following set, and they colour the registers of IsProgressEntryUnlocked's loops like the original.
#define TEST_BIT(flags, index) ((flags)[(index) / 8] & (1 << ((index) % 8)))
#define SET_BIT(flags, index) ((flags)[(index) / 8] |= 1 << ((index) % 8))

extern u8 data_020d4946[];

extern u8 data_020d4976[];

extern u8 data_020d497b[];

extern u8 gUnlockedTipBits[]; // inside gSaveData at +0x5ac (SaveData::mUnlockedTipBits0)

extern u8 data_020d49a6[];

extern u8 gUnlockedCharacterLevel0Bits[];

extern u8 gUnlockedCharacterLevel1Bits[];

extern u8 gUnlockedCharacterLevel2Bits[];

s32 UnlockEmblem8Or9();

BOOL IsProgressEntryUnlocked(u32 index);

BOOL UnlockProgressEntry(u32 index);

#define VD (((volatile SaveData *) gSaveData)[0])

static inline BOOL StageReached(u32 greater, u32 equal, u32 sub) {
    u8 stage = VD.mUnk_42;
    return (stage > greater) | ((stage == equal) & (VD.mUnk_43 == sub));
}


#define SET_FLAG_LOOP(first, last)                                      \
    {                                                                   \
        u8 i = first;                                                   \
        do {                                                            \
            u8 byte = gUnlockedTipBits[(u32) i >> 3];                      \
            if (!(byte & (1 << (i % 8)))) {                             \
                gUnlockedTipBits[(u32) i >> 3] = byte | (1 << (i % 8));   \
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
BOOL UnlockFields();
BOOL func_02086d48();
BOOL UnlockTips();
s32 UnlockEmblem();
s32 UnlockEmblem8Or9();
BOOL IsProgressEntryUnlocked(u32 index);
BOOL UnlockProgressEntry(u32 index);

// mwcc emits a file's objects sorted by size, byte arrays and the inline accessors' tables as sections of their own;
// the order of equal sizes follows from the whole declaration sequence. This definition order gives the original
// layout of .rodata and .data.
static const UnlockPair sCharacterUnlockList_09_6[1] = {{0xff, 0xff}};
static const u8 sProgressEntryList_24[1] = {38};
static const UnlockPair sProgressEntryList_26[1] = {{7, 2}};
static const UnlockPair sCharacterUnlockList_10_0[3] = {{7, 1}, {6, 2}, {0xff, 0xff}};

static const u16 sStoreItemList16_07[21] = {
    33, 35, 46, 50, 63, 68, 71, 60, 99, 95, 86, 109, 117, 133, 123, 141,
    176, 184, 197, 191, 25,
};

static const UnlockPair sCharacterUnlockList_09_7[1] = {{0xff, 0xff}};
static const UnlockPair sCharacterUnlockList_03_2[3] = {{8, 0}, {12, 0}, {0xff, 0xff}};
static const u16 sProgressEntryList_03[8] = {3, 4, 17, 81, 148, 149, 160, 187};
static const UnlockPair sCharacterUnlockList_01_2[1] = {{0xff, 0xff}};
static const u8 sProgressEntryList_22[1] = {10};
static const UnlockPair sCharacterUnlockList_03_3[3] = {{10, 0}, {11, 0}, {0xff, 0xff}};
static const u8 sStoreItemList8_08[1] = {30};
static const UnlockPair sCharacterUnlockList_01_1[1] = {{0xff, 0xff}};
static const UnlockPair sCharacterUnlockList_06_1[4] = {{14, 1}, {13, 1}, {8, 1}, {0xff, 0xff}};
static const UnlockPair sCharacterUnlockList_08_0[7] = {{9, 1}, {3, 1}, {14, 2}, {13, 2}, {15, 2}, {16, 2}, {0xff, 0xff}};
static const u8 sProgressEntryList_12[1] = {4};
static const UnlockPair sCharacterUnlockList_07_4[1] = {{0xff, 0xff}};
static const u8 sStoreItemList8_10[2] = {13, 37};
static const u8 sProgressEntryList_11[1] = {0};
static const u16 sStoreItemList16_09[15] = {26, 36, 51, 54, 89, 96, 87, 107, 115, 131, 125, 162, 177, 194, 195};
static const UnlockPair sCharacterUnlockList_04_0[1] = {{0xff, 0xff}};
static const u16 sStoreItemList16_08[14] = {30, 55, 57, 65, 94, 88, 104, 112, 128, 124, 190, 193, 181, 198};
static const UnlockPair sCharacterUnlockList_06_3[1] = {{0xff, 0xff}};
static const u8 sStoreItemList8_07[2] = {35, 25};

static const u16 sStoreItemList16_00[31] = {
    19, 20, 21, 22, 23, 40, 41, 42, 43, 44, 45, 85, 93, 102, 72, 77,
    98, 110, 134, 118, 126, 120, 143, 144, 145, 140, 168, 169, 170, 171, 34,
};

static const u8 sStoreItemList8_09[1] = {26};
static const u8 sProgressEntryList_06[1] = {16};
static const UnlockPair sCharacterUnlockList_07_3[1] = {{0xff, 0xff}};
static const u8 sProgressEntryList_25[1] = {39};
static const UnlockPair sCharacterUnlockList_00_0[5] = {{14, 0}, {13, 0}, {0, 0}, {1, 0}, {0xff, 0xff}};
static const UnlockPair *sCharacterUnlockLists_04[1] = {sCharacterUnlockList_04_0};
static const UnlockPair sCharacterUnlockList_09_2[3] = {{8, 2}, {4, 2}, {0xff, 0xff}};
static const UnlockPair sCharacterUnlockList_07_1[2] = {{15, 1}, {0xff, 0xff}};
static const UnlockPair sCharacterUnlockList_08_1[2] = {{12, 1}, {0xff, 0xff}};
static const UnlockPair sCharacterUnlockList_08_2[2] = {{10, 2}, {0xff, 0xff}};
static const UnlockPair sCharacterUnlockList_08_3[2] = {{9, 2}, {0xff, 0xff}};
static const UnlockPair *sCharacterUnlockLists_00[1] = {sCharacterUnlockList_00_0};
static const u8 sProgressEntryList_21[1] = {12};
static const UnlockPair sCharacterUnlockList_07_5[1] = {{0xff, 0xff}};
static const UnlockPair *sCharacterUnlockLists_08[4] = {sCharacterUnlockList_08_0, sCharacterUnlockList_08_1, sCharacterUnlockList_08_2, sCharacterUnlockList_08_3};
static const UnlockPair sCharacterUnlockList_09_5[2] = {{0, 2}, {0xff, 0xff}};
static const UnlockPair sCharacterUnlockList_02_0[2] = {{2, 0}, {0xff, 0xff}};
static const UnlockPair sCharacterUnlockList_09_3[3] = {{11, 2}, {1, 2}, {0xff, 0xff}};
static const UnlockPair sCharacterUnlockList_09_4[3] = {{12, 2}, {3, 2}, {0xff, 0xff}};
static const u16 sProgressEntryList_01[11] = {1, 2, 16, 73, 74, 84, 146, 147, 159, 186, 183};
static const UnlockPair sCharacterUnlockList_03_1[3] = {{15, 0}, {16, 0}, {0xff, 0xff}};
static const UnlockPair *sCharacterUnlockLists_03[4] = {NULL, sCharacterUnlockList_03_1, sCharacterUnlockList_03_2, sCharacterUnlockList_03_3};
static const u8 sProgressEntryList_13[1] = {7};
static const u8 sProgressEntryList_18[1] = {3};
static const UnlockPair sCharacterUnlockList_06_2[4] = {{9, 0}, {11, 1}, {10, 1}, {0xff, 0xff}};
static const UnlockPair sCharacterUnlockList_07_0[4] = {{2, 1}, {1, 1}, {4, 1}, {0xff, 0xff}};
static const UnlockPair *sCharacterUnlockLists_01[3] = {NULL, sCharacterUnlockList_01_1, sCharacterUnlockList_01_2};
static const u8 sProgressEntryList_15[1] = {11};
static const u8 sProgressEntryList_08[1] = {14};
static const u16 sStoreItemList16_10[11] = {37, 38, 39, 78, 111, 119, 127, 135, 163, 164, 165};
static const u16 sProgressEntryList_04[10] = {9, 10, 18, 83, 79, 153, 154, 189, 182, 161};
static const UnlockPair sCharacterUnlockList_07_2[2] = {{16, 1}, {0xff, 0xff}};
static const u8 sStoreItemList8_04[2] = {28, 29};
static const UnlockPair *sCharacterUnlockLists_02[1] = {sCharacterUnlockList_02_0};
static const u16 sProgressEntryList_02[11] = {7, 8, 14, 66, 80, 151, 152, 157, 185, 172, 178};
static const u8 sProgressEntryList_16[1] = {9};
static const u8 sProgressEntryList_17[1] = {1};
static const UnlockPair sCharacterUnlockList_09_0[2] = {{0, 1}, {0xff, 0xff}};

static const u16 sStoreItemList16_02[21] = {
    27, 31, 47, 56, 58, 49, 90, 92, 101, 105, 113, 129, 121, 137, 138, 139,
    136, 196, 192, 199, 24,
};

static const u8 sProgressEntryList_14[1] = {2};
static const UnlockPair sCharacterUnlockList_09_1[2] = {{2, 2}, {0xff, 0xff}};
static const u8 sProgressEntryList_10[1] = {18};
static const u8 sProgressEntryList_19[1] = {5};
static const u8 sProgressEntryList_23[1] = {36};
static const u8 sStoreItemList8_02[3] = {24, 27, 31};
static const UnlockPair *sCharacterUnlockLists_06[4] = {NULL, sCharacterUnlockList_06_1, sCharacterUnlockList_06_2, sCharacterUnlockList_06_3};
static const u16 sProgressEntryList_05[10] = {5, 6, 15, 64, 82, 75, 150, 158, 174, 166};
static const u8 sProgressEntryList_20[1] = {8};

static const UnlockPair *sCharacterUnlockLists_07[6] = {
    sCharacterUnlockList_07_0, sCharacterUnlockList_07_1, sCharacterUnlockList_07_2, sCharacterUnlockList_07_3, sCharacterUnlockList_07_4,
    sCharacterUnlockList_07_5,
};

static const UnlockPair *sCharacterUnlockLists_09[10] = {
    sCharacterUnlockList_09_0, sCharacterUnlockList_09_1, sCharacterUnlockList_09_2, sCharacterUnlockList_09_3, sCharacterUnlockList_09_4,
    sCharacterUnlockList_09_5, sCharacterUnlockList_09_6, sCharacterUnlockList_09_7, NULL, NULL,
};

static const u8 sProgressEntryList_09[1] = {17};
static const u8 sProgressEntryList_07[1] = {15};
static const u8 sStoreItemList8_00[8] = {19, 20, 21, 22, 23, 33, 32, 34};
static const UnlockPair *sCharacterUnlockLists_10[1] = {sCharacterUnlockList_10_0};

static const u16 sProgressEntryList_00[19] = {
    11, 12, 13, 53, 61, 62, 69, 70, 100, 103, 108, 116, 132, 155, 156, 188,
    173, 180, 167,
};

static const u16 sStoreItemList16_04[18] = {28, 32, 29, 48, 52, 59, 67, 97, 91, 76, 106, 114, 130, 122, 142, 175, 179, 200};

// The four tables come after the byte lists in the original, so they are sections of their own: function-local
// statics of inline accessors.
inline const FlagList8 *StoreItemLists8() {
    static const FlagList8 sStoreItemLists8[11] = {
        {8, sStoreItemList8_00},
        {0, NULL},
        {3, sStoreItemList8_02},
        {0, NULL},
        {2, sStoreItemList8_04},
        {0, NULL},
        {0, NULL},
        {2, sStoreItemList8_07},
        {1, sStoreItemList8_08},
        {1, sStoreItemList8_09},
        {2, sStoreItemList8_10},
    };
    return sStoreItemLists8;
}

inline const FlagList16 *StoreItemLists16() {
    static const FlagList16 sStoreItemLists16[11] = {
        {31, sStoreItemList16_00},
        {0, NULL},
        {21, sStoreItemList16_02},
        {0, NULL},
        {18, sStoreItemList16_04},
        {0, NULL},
        {0, NULL},
        {21, sStoreItemList16_07},
        {14, sStoreItemList16_08},
        {15, sStoreItemList16_09},
        {11, sStoreItemList16_10},
    };
    return sStoreItemLists16;
}

inline const UnlockTable *CharacterUnlockTables() {
    static const UnlockTable sCharacterUnlockTables[11] = {
        {1, sCharacterUnlockLists_00},
        {3, sCharacterUnlockLists_01},
        {1, sCharacterUnlockLists_02},
        {4, sCharacterUnlockLists_03},
        {1, sCharacterUnlockLists_04},
        {0, NULL},
        {4, sCharacterUnlockLists_06},
        {6, sCharacterUnlockLists_07},
        {4, sCharacterUnlockLists_08},
        {10, sCharacterUnlockLists_09},
        {1, sCharacterUnlockLists_10},
    };
    return sCharacterUnlockTables;
}

inline const ProgressEntry *ProgressEntries() {
    static const ProgressEntry sProgressEntries[27] = {
        {sProgressEntryList_00, 0, 19},
        {sProgressEntryList_01, 0, 11},
        {sProgressEntryList_02, 0, 11},
        {sProgressEntryList_03, 0, 8},
        {sProgressEntryList_04, 0, 10},
        {sProgressEntryList_05, 0, 10},
        {sProgressEntryList_06, 1, 1},
        {sProgressEntryList_07, 1, 1},
        {sProgressEntryList_08, 1, 1},
        {sProgressEntryList_09, 1, 1},
        {sProgressEntryList_10, 1, 1},
        {sProgressEntryList_11, 1, 1},
        {sProgressEntryList_12, 1, 1},
        {sProgressEntryList_13, 1, 1},
        {sProgressEntryList_14, 1, 1},
        {sProgressEntryList_15, 1, 1},
        {sProgressEntryList_16, 1, 1},
        {sProgressEntryList_17, 1, 1},
        {sProgressEntryList_18, 1, 1},
        {sProgressEntryList_19, 1, 1},
        {sProgressEntryList_20, 1, 1},
        {sProgressEntryList_21, 1, 1},
        {sProgressEntryList_22, 1, 1},
        {sProgressEntryList_23, 1, 1},
        {sProgressEntryList_24, 1, 1},
        {sProgressEntryList_25, 1, 1},
        {sProgressEntryList_26, 2, 2},
    };
    return sProgressEntries;
}



// The (u8 *) casts give each flag array its own literal-pool word, hoisted after the count table as in the original
// (through a pointer local it is hoisted first and trades registers with the constant 1; a plain member folds into the
// gSaveData base). Unlike the data_020d4976/data_020d497b externs, the cast form still aliases gSaveData, which
// keeps the mUnk_5b9 load after the flag store.
BOOL UnlockStoreItems() {
    BOOL result;
    u8 kind;
    u8 i;
    const u16 *list2;
    const u8 *list;
    result = FALSE;
    kind   = gSaveData[0].mUnk_42;
    list   = StoreItemLists8()[kind].mList;
    if (list != NULL) {
        i = 0;
        do {
            if (!TEST_BIT((u8 *) gSaveData[0].mUnlockedBakuganBits, list[i])) {
                SET_BIT((u8 *) gSaveData[0].mUnlockedBakuganBits, list[i]);
                gSaveData[0].mStoreItemsUnlocked = 1;
                result                      = TRUE;
            }
            i++;
        } while (i < StoreItemLists8()[kind = gSaveData[0].mUnk_42].mCount);
    }
    list2 = StoreItemLists16()[kind].mList;
    if (list2 != NULL) {
        i = 0;
        do {
            if (!TEST_BIT((u8 *) gSaveData[0].mUnlockedCardBits, list2[i])) {
                SET_BIT((u8 *) gSaveData[0].mUnlockedCardBits, list2[i]);
                gSaveData[0].mStoreItemsUnlocked = 1;
                result                      = TRUE;
            }
            i++;
        } while (i < StoreItemLists16()[kind = gSaveData[0].mUnk_42].mCount);
    }
    return result;
}

BOOL UnlockCharacters() {
    BOOL result = FALSE;
    const UnlockTable *table = &CharacterUnlockTables()[gSaveData[0].mUnk_42];
    u8 sub      = gSaveData[0].mUnk_43;
    if (table->mCount <= sub) {
        return result;
    }
    const u8 *list = (const u8 *) table->mLists[sub];
    if (list != NULL && list[0] != 0xff) {
    do {
        u8 id = list[0];
        if (!TestFlagBit(gUnlockedCharacterLevel0Bits + list[1] * 3, id)) {
            u32 index = (u32) id >> 3;
            if (!((gUnlockedCharacterLevel0Bits[index] | gUnlockedCharacterLevel1Bits[index] | gUnlockedCharacterLevel2Bits[index]) & (1 << (id % 8)))) {
                result = TRUE;
            }
            gUnlockedCharacterLevel0Bits[index] |= 1 << (id % 8);
            if (list[1] >= 1) {
                gUnlockedCharacterLevel1Bits[(u32) list[0] >> 3] |= 1 << (list[0] % 8);
            }
            if (list[1] >= 2) {
                gUnlockedCharacterLevel2Bits[(u32) list[0] >> 3] |= 1 << (list[0] % 8);
            }
        }
        list += 2;
    } while (list[0] != 0xff);
    }
    return result;
}

BOOL UnlockFields() {
    BOOL result = FALSE;
    if (!(gSaveData[0].mUnlockedFieldBits & 0x1)) {
        result = TRUE;
        ((volatile u8 *) gUnlockedFieldBits)[0] |= 0x1;
    }
    if (!(gSaveData[0].mUnlockedFieldBits & 0x80)) {
        result = TRUE;
        ((volatile u8 *) gUnlockedFieldBits)[0] |= 0x80;
    }
    if (gSaveData[0].mUnk_42 >= 4 && !(gSaveData[0].mUnlockedFieldBits & 0x4)) {
        result = TRUE;
        ((volatile u8 *) gUnlockedFieldBits)[0] |= 0x4;
    }
    if (gSaveData[0].mUnk_42 >= 7 && !(gSaveData[0].mUnlockedFieldBits & 0x10)) {
        result = TRUE;
        ((volatile u8 *) gUnlockedFieldBits)[0] |= 0x10;
    }
    if (gSaveData[0].mUnk_42 >= 8 && !(gSaveData[0].mUnlockedFieldBits & 0x8)) {
        result = TRUE;
        ((volatile u8 *) gUnlockedFieldBits)[0] |= 0x8;
    }
    u8 stage = gSaveData[0].mUnk_42;
    if ((stage > 8) | ((stage == 8) & (gSaveData[0].mUnk_43 == 4))) {
        if (!(gSaveData[0].mUnlockedFieldBits & 0x2)) {
            result = TRUE;
            ((volatile u8 *) gUnlockedFieldBits)[0] |= 0x2;
        }
    }
    stage = gSaveData[0].mUnk_42;
    if ((stage > 9) | ((stage == 9) & (gSaveData[0].mUnk_43 == 9))) {
        if (!(gSaveData[0].mUnlockedFieldBits & 0x20)) {
            result = TRUE;
            ((volatile u8 *) gUnlockedFieldBits)[0] |= 0x20;
        }
        if (!(gSaveData[0].mUnlockedFieldBits & 0x40)) {
            result = TRUE;
            ((volatile u8 *) gUnlockedFieldBits)[0] |= 0x40;
        }
    }
    return result;
}

BOOL func_02086d48() {
    BOOL result = FALSE;
    u32 level   = 0;
    u16 rank    = data_020d48f0.mRank;
    if (rank <= 500) {
        level = 1;
    }
    if (rank <= 100) {
        level = 2;
    }
    if (rank <= 30) {
        level = 3;
    }
    if (rank <= 10) {
        level = 4;
    }
    if (rank <= 3) {
        level = 5;
    }
    if (rank <= 1) {
        if (gSaveData[0].mCreditsDone != 0) {
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
    if (gSaveData[0].mUnk_5b8 < level) {
        gSaveData[0].mUnk_5b8 = level;
        result                    = TRUE;
    }
    return result;
}

BOOL UnlockTips() {
    BOOL result = FALSE;
    if (VD.mUnk_42 != 0) {
        SET_FLAG_LOOP(0, 3)
        SET_FLAG_LOOP(10, 13)
        if (VD.mUnk_43 != 0) {
            if (!(VD.mUnlockedTipBits0 & 0x10)) {
                result = TRUE;
                ((volatile u8 *) gUnlockedTipBits)[0] |= 0x10;
            }
        }
        if (VD.mUnk_43 > 1) {
            if (!(VD.mUnlockedTipBits0 & 0x20)) {
                result = TRUE;
                ((volatile u8 *) gUnlockedTipBits)[0] |= 0x20;
            }
            if (!(VD.mUnlockedTipBits0 & 0x40)) {
                result = TRUE;
                ((volatile u8 *) gUnlockedTipBits)[0] |= 0x40;
            }
        }
    }
    if (VD.mUnk_42 > 1) {
        if (!(VD.mUnlockedTipBits0 & 0x80)) {
            result = TRUE;
            ((volatile u8 *) gUnlockedTipBits)[0] |= 0x80;
        }
        if (!(VD.mUnlockedTipBits1 & 0x40)) {
            result = TRUE;
            ((volatile u8 *) gUnlockedTipBits)[1] |= 0x40;
        }
        if (!(VD.mUnlockedTipBits1 & 0x80)) {
            result = TRUE;
            ((volatile u8 *) gUnlockedTipBits)[1] |= 0x80;
        }
    }
    if (VD.mUnk_42 > 2) {
        if (!(VD.mUnlockedTipBits1 & 0x1)) {
            result = TRUE;
            ((volatile u8 *) gUnlockedTipBits)[1] |= 0x1;
        }
    }
    if (VD.mUnk_42 > 3) {
        SET_FLAG_LOOP(0x10, 0x1a)
    }
    if (VD.mUnk_42 > 5 && VD.mUnk_43 > 2) {
        if (!(VD.mUnlockedTipBits1 & 0x2)) {
            result = TRUE;
            ((volatile u8 *) gUnlockedTipBits)[1] |= 0x2;
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

s32 UnlockEmblem() {
    s32 result = -1;
    if (StageReached(1, 1, 2)) {
        if (!(VD.mUnlockedEmblemBits0 & 0x40)) {
            result = 6;
            ((volatile u8 *) data_020d49a6)[0] |= 0x40;
        }
    }
    if (result < 0) {
        if (StageReached(3, 3, 3)) {
            if (!(VD.mUnlockedEmblemBits0 & 0x2)) {
                result = 1;
                ((volatile u8 *) data_020d49a6)[0] |= 0x2;
            }
        }
    }
    if (result < 0) {
        if (StageReached(9, 6, 2)) {
            if (!(VD.mUnlockedEmblemBits0 & 0x10)) {
                result = 4;
                ((volatile u8 *) data_020d49a6)[0] |= 0x10;
            }
        }
    }
    if (result < 0) {
        if (StageReached(7, 7, 4)) {
            if (!(VD.mUnlockedEmblemBits0 & 0x20)) {
                result = 5;
                ((volatile u8 *) data_020d49a6)[0] |= 0x20;
            }
        }
    }
    if (result < 0) {
        if (StageReached(8, 8, 3)) {
            if (!(VD.mUnlockedEmblemBits0 & 0x1)) {
                result = 0;
                ((volatile u8 *) data_020d49a6)[0] |= 0x1;
            }
        }
    }
    if (result < 0) {
        if (StageReached(9, 9, 6)) {
            if (!(VD.mUnlockedEmblemBits0 & 0x4)) {
                result = 2;
                ((volatile u8 *) data_020d49a6)[0] |= 0x4;
            }
        }
    }
    if (result < 0) {
        u8 stage = VD.mUnk_42;
        if ((stage > 6) | ((stage == 6) & (VD.mUnk_43 == 2)) | VD.mCreditsDone) {
            if (data_020d48f0.mRank <= 1) {
                if (!(VD.mUnlockedEmblemBits0 & 0x80)) {
                    result = 7;
                    ((volatile u8 *) data_020d49a6)[0] |= 0x80;
                }
            }
        }
    }
    if (result < 0) {
        if (VD.mCreditsDone != 0) {
            if (!(VD.mUnlockedEmblemBits0 & 0x8)) {
                result = 3;
                ((volatile u8 *) data_020d49a6)[0] |= 0x8;
            }
        }
    }
    if (result < 0) {
        result = UnlockEmblem8Or9();
    }
    return result;
}

s32 UnlockEmblem8Or9() {
    s32 result = -1;
    if (!(gSaveData[0].mUnlockedEmblemBits1 & 0x1)) {
        u16 count = 0;
        u16 i     = 0;
        do {
            u16 j = 0;
            do {
                if (func_020231f4(gSaveData, 0, i, j) != 0) {
                    count++;
                    break;
                }
                j++;
            } while (j < 6);
            i++;
        } while (i < 0x29);
        if (count >= (u16) (gSaveData[0].mSpecialNameCode0Used + 0x24)) {
            result = 8;
            ((u8 *) data_020d49a6)[1] |= 0x1;
        }
    }
    if (result < 0) {
        if (!(gSaveData[0].mUnlockedEmblemBits1 & 0x2)) {
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

BOOL IsProgressEntryUnlocked(u32 index) {
    BOOL result = TRUE;
    u8 i;
    switch (ProgressEntries()[index].mType) {
        case 0: {
            i               = 0;
            const u16 *list = (const u16 *) ProgressEntries()[index].mList;
            do {
                if (!TEST_BIT(data_020d497b, list[i])) {
                    result = FALSE;
                    break;
                }
                i++;
            } while (i < ProgressEntries()[index].mCount);
            break;
        }
        case 1: {
            const u8 *list = (const u8 *) ProgressEntries()[index].mList;
            i              = 0;
            do {
                if (!TEST_BIT(data_020d4976, list[i])) {
                    result = FALSE;
                    break;
                }
                i++;
            } while (i < ProgressEntries()[index].mCount);
            break;
        }
        case 2: {
            const u8 *list = (const u8 *) ProgressEntries()[index].mList;
            i              = 0;
            do {
                if (!TEST_BIT(gUnlockedCharacterLevel0Bits + list[1] * 3, list[0])) {
                    result = FALSE;
                }
                i++;
            } while (i < ProgressEntries()[index].mCount);
            break;
        }
    }
    return result;
}

BOOL UnlockProgressEntry(u32 index) {
    BOOL result = TRUE;
    switch (ProgressEntries()[index].mType) {
        case 0: {
            u8 i            = 0;
            const u16 *list = (const u16 *) ProgressEntries()[index].mList;
            do {
                if (!TEST_BIT((u8 *) gSaveData[0].mUnlockedCardBits, list[i])) {
                    SET_BIT((u8 *) gSaveData[0].mUnlockedCardBits, list[i]);
                    gSaveData[0].mStoreItemsUnlocked = 1;
                    result                       = TRUE;
                }
                i++;
            } while (i < ProgressEntries()[index].mCount);
            break;
        }
        case 1: {
            const u8 *list;
            u8 count;
            u8 i;
            const ProgressEntry *e = &ProgressEntries()[index];
            count = e->mCount;
            list  = (const u8 *) ProgressEntries()[index].mList;
            i     = 0;
            do {
                if (!TEST_BIT((u8 *) gSaveData[0].mUnlockedBakuganBits, list[i])) {
                    SET_BIT((u8 *) gSaveData[0].mUnlockedBakuganBits, list[i]);
                    gSaveData[0].mStoreItemsUnlocked = 1;
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
            const ProgressEntry *e = &ProgressEntries()[index];
            count = e->mCount;
            list  = (const u8 *) ProgressEntries()[index].mList;
            i     = 0;
            do {
                u8 id = list[0];
                if (!((gUnlockedCharacterLevel0Bits[(u32) id >> 3] | gUnlockedCharacterLevel1Bits[(u32) id >> 3] | gUnlockedCharacterLevel2Bits[(u32) id >> 3]) & (1 << (id % 8)))) {
                    result = TRUE;
                }
                (gUnlockedCharacterLevel0Bits + list[1] * 3)[(u32) id >> 3] |= 1 << (id % 8);
                i++;
            } while (i < count);
            break;
        }
    }
    return result;
}
