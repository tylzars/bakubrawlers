#include "Game/SaveData.hpp"
#include "Game/GameData.hpp"
#include "Game/Random.hpp"
#include "Unknown/UnkFunctions.hpp"
#include <nitro/mi.h>

// One record of the park character table (12 bytes).
struct ParkEntry {
    /* 00 */ s32 mType;
    /* 04 */ u8 mUnk_04;
    /* 05 */ u8 mUnk_05;
    /* 06 */ u8 mProgressEntryIndex; // 0xff = none; same byte as ParkScene.cpp's copy and ParkMenu's +0x12
    /* 07 */ u8 mMinCategory;
    /* 08 */ u8 mFieldId;
    /* 09 */ u8 mUnk_09;
    /* 0a */ u16 mUnk_0a;
};

struct ParkGroup {
    /* 00 */ u16 mId;
    /* 02 */ u8 mDefault;
    /* 03 */ u8 mCount;
    /* 04 */ ParkEntry *mEntries;
};

struct ParkCategory {
    /* 00 */ u8 mCount;
    /* 04 */ ParkGroup *mGroups;
};

extern ParkCategory gParkCategories[];
extern BOOL IsProgressEntryUnlocked(u32 index);
void GetParkEntry(u32 index, void *out);
BOOL CanUnlockProgressEntry(u32 id);
BOOL AreProgressEntries6To10Unlocked();
BOOL AreProgressEntries11To15Unlocked();

// Moved here from StorytalkWindow.cpp, where it currently sits past the end of that unit (0x02088f38).
u8 GetStageParkCategory() {
    u8 stage = gSaveData[0].mUnk_42;
    if (stage == 0 || (stage == 1 && gSaveData[0].mUnk_43 == 0)) {
        return 0;
    }
    if (stage == 1 && gSaveData[0].mUnk_43 == 1) {
        return 1;
    }
    if (stage == 1 && gSaveData[0].mUnk_43 == 2) {
        return 2;
    }
    if (stage == 2) {
        return 3;
    }
    if (stage == 3 && gSaveData[0].mUnk_43 == 0) {
        return 4;
    }
    if (stage == 3 && gSaveData[0].mUnk_43 == 1) {
        return 5;
    }
    if (stage == 3 && gSaveData[0].mUnk_43 == 2) {
        return 6;
    }
    if (stage == 3 && gSaveData[0].mUnk_43 == 3) {
        return 7;
    }
    if (stage == 4 && gSaveData[0].mUnk_43 == 0) {
        return 8;
    }
    if (stage == 5 || (stage == 6 && gSaveData[0].mUnk_43 == 0)) {
        return 9;
    }
    if (stage == 6 && gSaveData[0].mUnk_43 == 1) {
        return 10;
    }
    if (stage == 6 && gSaveData[0].mUnk_43 == 2) {
        return 11;
    }
    if (stage == 7 && gSaveData[0].mUnk_43 == 0) {
        return 12;
    }
    if (stage == 7 && gSaveData[0].mUnk_43 == 1) {
        return 13;
    }
    if (stage == 7 && gSaveData[0].mUnk_43 == 2) {
        return 14;
    }
    if (stage == 7 && gSaveData[0].mUnk_43 == 3) {
        return 15;
    }
    if (stage == 7 && gSaveData[0].mUnk_43 == 4) {
        return 16;
    }
    if ((stage == 7 && gSaveData[0].mUnk_43 == 5) || (stage == 8 && gSaveData[0].mUnk_43 == 0)) {
        return 17;
    }
    if (stage == 8 && gSaveData[0].mUnk_43 == 1) {
        return 18;
    }
    if (stage == 8 && gSaveData[0].mUnk_43 == 2) {
        return 19;
    }
    if (stage == 8 && gSaveData[0].mUnk_43 == 3) {
        return 20;
    }
    if ((stage == 8 && gSaveData[0].mUnk_43 == 4) || (stage == 9 && gSaveData[0].mUnk_43 == 0)) {
        return 21;
    }
    if (stage == 9 && gSaveData[0].mUnk_43 == 1) {
        return 22;
    }
    if (stage == 9 && gSaveData[0].mUnk_43 == 2) {
        return 23;
    }
    if (stage == 9 && gSaveData[0].mUnk_43 == 3) {
        return 24;
    }
    if (stage == 9 && gSaveData[0].mUnk_43 == 4) {
        return 25;
    }
    if (stage == 9 && gSaveData[0].mUnk_43 == 5) {
        return 26;
    }
    return 27;
}

void RefreshParkSlotProgressEntryFlag() {
    u8 i = 0;
    u8 buf[12];
    gSaveData[0].mParkSlotHasProgressEntry = 0;
    do {
        GetParkEntry(i, buf);
        if (buf[6] != 0xff) {
            gSaveData[0].mParkSlotHasProgressEntry = 1;
            return;
        }
        i++;
    } while (i < gSaveData[0].mParkSlotCount);
}

// One chosen character group while the park list is built (4 bytes).
struct ParkPick {
    /* 00 */ u8 mCategory;
    /* 01 */ u8 mGroup;
    /* 02 */ u8 mId;
    /* 03 */ u8 mValue;
};

// The register allocator orders each loop's reused locals by where the variable first appears in the function, so
// some placements here are load-bearing: `j = 0` at its declaration puts j (the fill loop's index) first, `g = 0` comes
// before `category`/`group` in the first loop, the perm loop counts with g, `flag` is shared by the duplicate check and
// the else branch, and `k` is declared right after `group`.
// Also writes gGameData.mParkGroupEntryIndices (the entry picked for each slot).
void FillParkSlots() {
    ParkGroup *group;
    u8 k;
    ParkEntry *entry;
    ParkCategory *category;
    u8 count;
    u8 cat;
    u8 g;
    u8 e;
    BOOL flag;
    s32 last;
    u8 i;
    u8 j = 0;
    if (gSaveData[0].mCreditsDone) {
        ParkPick picks[26] = {0};
        count = 0;
        cat   = 0;
        do {
            g        = 0;
            category = &gParkCategories[cat];
            group    = category->mGroups;
            do {
                entry = group->mEntries;
                e     = 0;
                do {
                    if (entry->mProgressEntryIndex != 0xff && CanUnlockProgressEntry(entry->mProgressEntryIndex)) {
                        flag = FALSE;
                        for (k = 0; k < count; k++) {
                            if (group->mId == picks[k].mId) {
                                flag = TRUE;
                                break;
                            }
                        }
                        if (!flag) {
                            ParkPick *pick = &picks[count];
                            pick->mCategory = cat;
                            pick->mGroup    = g;
                            pick->mId       = group->mId;
                            count++;
                            pick->mValue = entry->mProgressEntryIndex;
                            if (count >= 0x1a) {
                                cat = 0x1c;
                                g   = category->mCount;
                                break;
                            }
                        }
                    }
                    e++;
                    entry++;
                } while (e < group->mCount);
                g++;
                group++;
            } while (g < category->mCount);
            cat++;
        } while (cat < 0x1c);
        if (count > 1) {
            last = count - 1;
            for (i = 0; i < last; i++) {
                for (j = last; j > i; j--) {
                    u8 d = picks[j].mValue;
                    if (d < picks[j - 1].mValue) {
                        u8 a = picks[j].mCategory;
                        u8 b = picks[j].mGroup;
                        u8 c = picks[j].mId;
                        picks[j].mCategory     = picks[j - 1].mCategory;
                        picks[j].mGroup        = picks[j - 1].mGroup;
                        picks[j].mId           = picks[j - 1].mId;
                        picks[j].mValue        = picks[j - 1].mValue;
                        picks[j - 1].mCategory = a;
                        picks[j - 1].mGroup    = b;
                        picks[j - 1].mId       = c;
                        picks[j - 1].mValue    = d;
                    }
                }
            }
        }
        if (count > 8) {
            count = 8;
        }
        for (i = 0; i < count; i++) {
            gSaveData[0].mParkSlots[i].mCategory = picks[i].mCategory;
            gSaveData[0].mParkSlots[i].mGroup    = picks[i].mGroup;
        }
        if (count < 8) {
            u8 pool[16]   = {0, 1, 2, 3, 4, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16};
            u8 chosen[16] = {0};
            u8 used       = 0;
            u8 n;
            i = 0;
            do {
                for (j = 0; j < count; j++) {
                    if (pool[i] == picks[j].mId) {
                        pool[i] = 0xff;
                    }
                }
                u8 v = pool[i];
                i++;
                if (v != 0xff) {
                    chosen[used++] = v;
                }
            } while (i < 16);
            i = 0;
            do {
                u8 r = MATH_Rand32(&gRandom, used);
                u8 t = chosen[i];
                chosen[i] = chosen[r];
                chosen[r] = t;
                i++;
            } while (i < used);
            j = 0;
            do {
                cat = 0;
                do {
                    category = &gParkCategories[cat];
                    group    = category->mGroups;
                    g        = 0;
                    do {
                        if (chosen[j] == group->mId) {
                            entry = group->mEntries;
                            n     = 0;
                            do {
                                if (entry->mType <= PARK_ENTRY_TYPE_BATTLE_ROYALE) { // one of the three battle types
                                    gSaveData[0].mParkSlots[count].mCategory = cat;
                                    gSaveData[0].mParkSlots[count].mGroup    = g;
                                    count++;
                                    g   = category->mCount;
                                    cat = 0x1c;
                                    if (count >= 8) {
                                        j = used;
                                    }
                                    break;
                                }
                                entry++;
                                n++;
                            } while (n < group->mCount);
                        }
                        group++;
                        g++;
                    } while (g < category->mCount);
                    cat++;
                } while (cat < 0x1c);
                j++;
            } while (j < used);
        }
        gSaveData[0].mParkSlotCount = 8;
        i = 0;
        do {
            u8 r = MATH_Rand32(&gRandom, 8);
            u8 t0 = gSaveData[0].mParkSlots[i].mCategory;
            u8 t1 = gSaveData[0].mParkSlots[i].mGroup;
            gSaveData[0].mParkSlots[i].mCategory = gSaveData[0].mParkSlots[r].mCategory;
            gSaveData[0].mParkSlots[i].mGroup    = gSaveData[0].mParkSlots[r].mGroup;
            gSaveData[0].mParkSlots[r].mCategory = t0;
            gSaveData[0].mParkSlots[r].mGroup    = t1;
            i++;
        } while (i < gSaveData[0].mParkSlotCount);
        // Entry types to pick from (BATTLE twice); after the shuffle perm[0] is the type looked for.
        u8 perm[4] = {PARK_ENTRY_TYPE_BATTLE, PARK_ENTRY_TYPE_BATTLE, PARK_ENTRY_TYPE_TEAMBATTLE,
                      PARK_ENTRY_TYPE_BATTLE_ROYALE};
        g = 0;
        do {
            u8 grp;
            cat   = gSaveData[0].mParkSlots[g].mCategory;
            grp   = gSaveData[0].mParkSlots[g].mGroup;
            entry = gParkCategories[cat].mGroups[grp].mEntries;
            j = 0;
            do {
                u8 r = MATH_Rand32(&gRandom, 4);
                u8 t = perm[j];
                perm[j] = perm[r];
                perm[r] = t;
                j++;
            } while (j < 4);
            e = 0;
            do {
                if (entry->mType == perm[0]) {
                    gGameData.mParkGroupEntryIndices[g] = e;
                    break;
                }
                e++;
                entry++;
            } while (e < gParkCategories[cat].mGroups[grp].mCount);
            g++;
        } while (g < gSaveData[0].mParkSlotCount);
    } else {
        ParkGroup *stageGroup;
        ParkEntry *stageEntry;
        u8 stage;
        stage      = GetStageParkCategory();
        category   = &gParkCategories[stage];
        stageGroup = category->mGroups;
        count      = 0;
        g          = 0;
        do {
            stageEntry = &stageGroup->mEntries[stageGroup->mDefault];
            flag       = FALSE;
            if (stageEntry->mType == PARK_ENTRY_TYPE_TUTORIAL) {
                if (stageEntry->mUnk_0a == 0) {
                    if ((gSaveData[0].mTutorialBits & 8) && (gSaveData[0].mTutorialBits & 0x10)) {
                        flag = TRUE;
                    }
                } else if (gSaveData[0].mTutorialBits & 0x10) {
                    flag = TRUE;
                }
            }
            if (!flag) {
                gSaveData[0].mParkSlots[count].mCategory = stage;
                gSaveData[0].mParkSlots[count].mGroup    = g;
                count++;
            }
            g++;
            stageGroup++;
        } while (g < category->mCount);
        gSaveData[0].mParkSlotCount = count;
    }
    RefreshParkSlotProgressEntryFlag();
}

void GetParkEntry(u32 index, void *out) {
    u8 group;
    ParkGroup *groups;
    BOOL found;
    u8 i;
    u8 category;
    category          = gSaveData[0].mParkSlots[index].mCategory;
    ParkCategory *cat = &gParkCategories[category];
    group             = gSaveData[0].mParkSlots[index].mGroup;
    groups            = cat->mGroups;
    found             = FALSE;
    u32 type          = groups[group].mEntries[groups[group].mDefault].mType;
    if ((type != PARK_ENTRY_TYPE_MES_ONLY_1 && type != PARK_ENTRY_TYPE_MES_ONLY_2 && type != PARK_ENTRY_TYPE_MES_ONLY_3) ||
        gSaveData[0].mCreditsDone) {
        ParkEntry *entries = groups[group].mEntries;
        i                  = 0;
        if (gSaveData[0].mCreditsDone) {
            category = 0xff;
        }
        do {
            if (entries->mProgressEntryIndex != 0xff && category >= entries->mMinCategory && CanUnlockProgressEntry(entries->mProgressEntryIndex)) {
                MIi_CpuCopyFast(entries, out, 12);
                found = TRUE;
                break;
            }
            i++;
            entries++;
        } while (i < groups[group].mCount);
    }
    if (found) {
        return;
    }
    ParkEntry *list = groups[group].mEntries;
    if (gSaveData[0].mCreditsDone) {
        MIi_CpuCopyFast(&list[gGameData.mParkGroupEntryIndices[index]], out, 12);
    } else {
        MIi_CpuCopyFast(&list[groups[group].mDefault], out, 12);
    }
}

u8 GetParkGroupId(u32 index) {
    ParkCategory *category = &gParkCategories[gSaveData[0].mParkSlots[index].mCategory];
    return (u8) category->mGroups[gSaveData[0].mParkSlots[index].mGroup].mId;
}

void func_02089a9c(u32 index) {
    ParkEntry entry;
    GetParkEntry(index, &entry);
    u32 flag = gSaveData[0].mCreditsDone;
    if (flag && entry.mType != PARK_ENTRY_TYPE_BATTLE && entry.mType != PARK_ENTRY_TYPE_TEAMBATTLE &&
        entry.mType != PARK_ENTRY_TYPE_BATTLE_ROYALE) {
        entry.mType = PARK_ENTRY_TYPE_BATTLE; // with the flag set, every other type is handled as a battle
    }
    switch (entry.mType) {
        case PARK_ENTRY_TYPE_BATTLE:
        case PARK_ENTRY_TYPE_TEAMBATTLE:
        case PARK_ENTRY_TYPE_BATTLE_ROYALE: {
            if (gSaveData[0].mParkSlotCount <= 4) {
                return;
            }
            u8 i;
            u8 slotCat = gSaveData[0].mParkSlots[index].mCategory;
            u8 slotGroup = gSaveData[0].mParkSlots[index].mGroup;
            u8 charSlot;
            if (flag) {
                charSlot = gGameData.mParkGroupEntryIndices[index];
            }
            gSaveData[0].mParkSlots[index].mCategory = gSaveData[0].mParkSlots[4].mCategory;
            gSaveData[0].mParkSlots[index].mGroup = gSaveData[0].mParkSlots[4].mGroup;
            if (gSaveData[0].mCreditsDone) {
                gGameData.mParkGroupEntryIndices[index] = gGameData.mParkGroupEntryIndices[4];
            }
            i = 4;
            do {
                gSaveData[0].mParkSlots[i].mCategory = gSaveData[0].mParkSlots[i + 1].mCategory;
                gSaveData[0].mParkSlots[i].mGroup    = gSaveData[0].mParkSlots[i + 1].mGroup;
                if (gSaveData[0].mCreditsDone) {
                    gGameData.mParkGroupEntryIndices[i] = gGameData.mParkGroupEntryIndices[i + 1];
                }
                i++;
            } while (i < gSaveData[0].mParkSlotCount - 1);
            gSaveData[0].mParkSlots[gSaveData[0].mParkSlotCount - 1].mCategory = slotCat;
            gSaveData[0].mParkSlots[gSaveData[0].mParkSlotCount - 1].mGroup = slotGroup;
            if (gSaveData[0].mCreditsDone) {
                gGameData.mParkGroupEntryIndices[gSaveData[0].mParkSlotCount - 1] = charSlot;
            }
            break;
        }
        case PARK_ENTRY_TYPE_TUTORIAL: {
            if (entry.mUnk_0a == 0 && !(gSaveData[0].mTutorialBits & 0x10)) {
                return;
            }
            do {
                gSaveData[0].mParkSlots[index].mCategory = gSaveData[0].mParkSlots[index + 1].mCategory;
                gSaveData[0].mParkSlots[index].mGroup    = gSaveData[0].mParkSlots[index + 1].mGroup;
                index = (index + 1) & 0xff;
            } while (index < gSaveData[0].mParkSlotCount);
            gSaveData[0].mParkSlotCount--;
            break;
        }
    }
}

BOOL CanUnlockProgressEntry(u32 id) {
    BOOL result = TRUE;
    if (IsProgressEntryUnlocked(id)) {
        return FALSE;
    }
    switch (id) {
        case 6:
        case 7:
        case 8:
        case 9:
        case 10:
            if (gSaveData[0].mUnk_5b8 < 4) {
                result = FALSE;
            }
            break;
        case 11:
        case 12:
        case 13:
        case 14:
        case 15:
        case 16:
            if (!gSaveData[0].mCreditsDone) {
                result = FALSE;
            }
            break;
        case 17:
            if (!(((u8 *) gSaveData)[0x586] & 1) || !AreProgressEntries6To10Unlocked() || gSaveData[0].mUnk_5b8 < 5) {
                result = FALSE;
            }
            break;
        case 18:
            if (!(((u8 *) gSaveData)[0x586] & 4) || !AreProgressEntries6To10Unlocked() || gSaveData[0].mUnk_5b8 < 5) {
                result = FALSE;
            }
            break;
        case 19:
            if (!(((u8 *) gSaveData)[0x586] & 0x10) || !AreProgressEntries6To10Unlocked() || gSaveData[0].mUnk_5b8 < 5) {
                result = FALSE;
            }
            break;
        case 20:
            if (!(((u8 *) gSaveData)[0x586] & 0x80) || !AreProgressEntries6To10Unlocked() || gSaveData[0].mUnk_5b8 < 5) {
                result = FALSE;
            }
            break;
        case 21:
            if (!(((u8 *) gSaveData)[0x587] & 0x8) || !AreProgressEntries6To10Unlocked() || gSaveData[0].mUnk_5b8 < 5) {
                result = FALSE;
            }
            break;
        case 22:
            if (!(((u8 *) gSaveData)[0x587] & 0x2) || !AreProgressEntries6To10Unlocked() || gSaveData[0].mUnk_5b8 < 5) {
                result = FALSE;
            }
            break;
        case 23:
        case 24:
            if (gSaveData[0].mUnk_5b8 < 6 || !AreProgressEntries11To15Unlocked()) {
                result = FALSE;
            }
            break;
        case 25: {
            u8 i = 6;
            do {
                if (!IsProgressEntryUnlocked(i)) {
                    result = FALSE;
                    break;
                }
                i++;
            } while (i < 0x19);
            break;
        }
    }
    return result;
}

BOOL AreProgressEntries6To10Unlocked() {
    if (!IsProgressEntryUnlocked(6) || !IsProgressEntryUnlocked(7) || !IsProgressEntryUnlocked(8) || !IsProgressEntryUnlocked(9) || !IsProgressEntryUnlocked(10)) {
        return FALSE;
    }
    return TRUE;
}

BOOL AreProgressEntries11To15Unlocked() {
    if (!IsProgressEntryUnlocked(11) || !IsProgressEntryUnlocked(12) || !IsProgressEntryUnlocked(13) || !IsProgressEntryUnlocked(14) ||
        !IsProgressEntryUnlocked(15)) {
        return FALSE;
    }
    return TRUE;
}
