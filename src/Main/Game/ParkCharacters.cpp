#include "Game/UnkStruct_020d43f0.hpp"
#include "Game/UnkStruct_020d433c.hpp"
#include "Game/Random.hpp"
#include <nitro/mi.h>

// One record of the park character table (12 bytes).
struct ParkEntry {
    /* 00 */ s32 mType;
    /* 04 */ u8 mUnk_04;
    /* 05 */ u8 mUnk_05;
    /* 06 */ u8 mUnk_06;
    /* 07 */ u8 mUnk_07;
    /* 08 */ u8 mUnk_08;
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

extern ParkCategory data_020a4ccc[];
extern BOOL func_020875ac(u32 index);
void GetParkEntry(u32 index, void *out);
BOOL func_02089c7c(u32 id);
BOOL func_02089ec8();
BOOL func_02089f2c();

// Moved here from StorytalkWindow.cpp, where it currently sits past the end of that unit (0x02088f38).
u8 func_02088f38() {
    u8 stage = data_020d43f0[0].mUnk_42;
    if (stage == 0 || (stage == 1 && data_020d43f0[0].mUnk_43 == 0)) {
        return 0;
    }
    if (stage == 1 && data_020d43f0[0].mUnk_43 == 1) {
        return 1;
    }
    if (stage == 1 && data_020d43f0[0].mUnk_43 == 2) {
        return 2;
    }
    if (stage == 2) {
        return 3;
    }
    if (stage == 3 && data_020d43f0[0].mUnk_43 == 0) {
        return 4;
    }
    if (stage == 3 && data_020d43f0[0].mUnk_43 == 1) {
        return 5;
    }
    if (stage == 3 && data_020d43f0[0].mUnk_43 == 2) {
        return 6;
    }
    if (stage == 3 && data_020d43f0[0].mUnk_43 == 3) {
        return 7;
    }
    if (stage == 4 && data_020d43f0[0].mUnk_43 == 0) {
        return 8;
    }
    if (stage == 5 || (stage == 6 && data_020d43f0[0].mUnk_43 == 0)) {
        return 9;
    }
    if (stage == 6 && data_020d43f0[0].mUnk_43 == 1) {
        return 10;
    }
    if (stage == 6 && data_020d43f0[0].mUnk_43 == 2) {
        return 11;
    }
    if (stage == 7 && data_020d43f0[0].mUnk_43 == 0) {
        return 12;
    }
    if (stage == 7 && data_020d43f0[0].mUnk_43 == 1) {
        return 13;
    }
    if (stage == 7 && data_020d43f0[0].mUnk_43 == 2) {
        return 14;
    }
    if (stage == 7 && data_020d43f0[0].mUnk_43 == 3) {
        return 15;
    }
    if (stage == 7 && data_020d43f0[0].mUnk_43 == 4) {
        return 16;
    }
    if ((stage == 7 && data_020d43f0[0].mUnk_43 == 5) || (stage == 8 && data_020d43f0[0].mUnk_43 == 0)) {
        return 17;
    }
    if (stage == 8 && data_020d43f0[0].mUnk_43 == 1) {
        return 18;
    }
    if (stage == 8 && data_020d43f0[0].mUnk_43 == 2) {
        return 19;
    }
    if (stage == 8 && data_020d43f0[0].mUnk_43 == 3) {
        return 20;
    }
    if ((stage == 8 && data_020d43f0[0].mUnk_43 == 4) || (stage == 9 && data_020d43f0[0].mUnk_43 == 0)) {
        return 21;
    }
    if (stage == 9 && data_020d43f0[0].mUnk_43 == 1) {
        return 22;
    }
    if (stage == 9 && data_020d43f0[0].mUnk_43 == 2) {
        return 23;
    }
    if (stage == 9 && data_020d43f0[0].mUnk_43 == 3) {
        return 24;
    }
    if (stage == 9 && data_020d43f0[0].mUnk_43 == 4) {
        return 25;
    }
    if (stage == 9 && data_020d43f0[0].mUnk_43 == 5) {
        return 26;
    }
    return 27;
}

void func_02089208() {
    u8 i = 0;
    u8 buf[12];
    data_020d43f0[0].mUnk_5b9_1 = 0;
    do {
        GetParkEntry(i, buf);
        if (buf[6] != 0xff) {
            data_020d43f0[0].mUnk_5b9_1 = 1;
            return;
        }
        i++;
    } while (i < data_020d43f0[0].mUnk_5c1);
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
void func_02089278() {
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
    if (data_020d43f0[0].mUnk_5bf_0) {
        ParkPick picks[26] = {0};
        count = 0;
        cat   = 0;
        do {
            g        = 0;
            category = &data_020a4ccc[cat];
            group    = category->mGroups;
            do {
                entry = group->mEntries;
                e     = 0;
                do {
                    if (entry->mUnk_06 != 0xff && func_02089c7c(entry->mUnk_06)) {
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
                            pick->mValue = entry->mUnk_06;
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
            data_020d43f0[0].mUnk_5c2[i].mCategory = picks[i].mCategory;
            data_020d43f0[0].mUnk_5c2[i].mGroup    = picks[i].mGroup;
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
                u8 r = MATH_Rand32(&data_020d42e8, used);
                u8 t = chosen[i];
                chosen[i] = chosen[r];
                chosen[r] = t;
                i++;
            } while (i < used);
            j = 0;
            do {
                cat = 0;
                do {
                    category = &data_020a4ccc[cat];
                    group    = category->mGroups;
                    g        = 0;
                    do {
                        if (chosen[j] == group->mId) {
                            entry = group->mEntries;
                            n     = 0;
                            do {
                                if (entry->mType <= 2) {
                                    data_020d43f0[0].mUnk_5c2[count].mCategory = cat;
                                    data_020d43f0[0].mUnk_5c2[count].mGroup    = g;
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
        data_020d43f0[0].mUnk_5c1 = 8;
        i = 0;
        do {
            u8 r = MATH_Rand32(&data_020d42e8, 8);
            u8 t0 = data_020d43f0[0].mUnk_5c2[i].mCategory;
            u8 t1 = data_020d43f0[0].mUnk_5c2[i].mGroup;
            data_020d43f0[0].mUnk_5c2[i].mCategory = data_020d43f0[0].mUnk_5c2[r].mCategory;
            data_020d43f0[0].mUnk_5c2[i].mGroup    = data_020d43f0[0].mUnk_5c2[r].mGroup;
            data_020d43f0[0].mUnk_5c2[r].mCategory = t0;
            data_020d43f0[0].mUnk_5c2[r].mGroup    = t1;
            i++;
        } while (i < data_020d43f0[0].mUnk_5c1);
        u8 perm[4] = {0, 0, 1, 2};
        g = 0;
        do {
            u8 grp;
            cat   = data_020d43f0[0].mUnk_5c2[g].mCategory;
            grp   = data_020d43f0[0].mUnk_5c2[g].mGroup;
            entry = data_020a4ccc[cat].mGroups[grp].mEntries;
            j = 0;
            do {
                u8 r = MATH_Rand32(&data_020d42e8, 4);
                u8 t = perm[j];
                perm[j] = perm[r];
                perm[r] = t;
                j++;
            } while (j < 4);
            e = 0;
            do {
                if (entry->mType == perm[0]) {
                    data_020d433c.mUnk_8d[g] = e;
                    break;
                }
                e++;
                entry++;
            } while (e < data_020a4ccc[cat].mGroups[grp].mCount);
            g++;
        } while (g < data_020d43f0[0].mUnk_5c1);
    } else {
        ParkGroup *stageGroup;
        ParkEntry *stageEntry;
        u8 stage;
        stage      = func_02088f38();
        category   = &data_020a4ccc[stage];
        stageGroup = category->mGroups;
        count      = 0;
        g          = 0;
        do {
            stageEntry = &stageGroup->mEntries[stageGroup->mDefault];
            flag       = FALSE;
            if (stageEntry->mType == 6) {
                if (stageEntry->mUnk_0a == 0) {
                    if ((data_020d43f0[0].mUnk_5be & 8) && (data_020d43f0[0].mUnk_5be & 0x10)) {
                        flag = TRUE;
                    }
                } else if (data_020d43f0[0].mUnk_5be & 0x10) {
                    flag = TRUE;
                }
            }
            if (!flag) {
                data_020d43f0[0].mUnk_5c2[count].mCategory = stage;
                data_020d43f0[0].mUnk_5c2[count].mGroup    = g;
                count++;
            }
            g++;
            stageGroup++;
        } while (g < category->mCount);
        data_020d43f0[0].mUnk_5c1 = count;
    }
    func_02089208();
}

void GetParkEntry(u32 index, void *out) {
    u8 group;
    ParkGroup *groups;
    BOOL found;
    u8 i;
    u8 category;
    category          = data_020d43f0[0].mUnk_5c2[index].mCategory;
    ParkCategory *cat = &data_020a4ccc[category];
    group             = data_020d43f0[0].mUnk_5c2[index].mGroup;
    groups            = cat->mGroups;
    found             = FALSE;
    u32 type          = groups[group].mEntries[groups[group].mDefault].mType;
    if ((type != 3 && type != 4 && type != 5) || data_020d43f0[0].mUnk_5bf_0) {
        ParkEntry *entries = groups[group].mEntries;
        i                  = 0;
        if (data_020d43f0[0].mUnk_5bf_0) {
            category = 0xff;
        }
        do {
            if (entries->mUnk_06 != 0xff && category >= entries->mUnk_07 && func_02089c7c(entries->mUnk_06)) {
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
    if (data_020d43f0[0].mUnk_5bf_0) {
        MIi_CpuCopyFast(&list[data_020d433c.mUnk_8d[index]], out, 12);
    } else {
        MIi_CpuCopyFast(&list[groups[group].mDefault], out, 12);
    }
}

u8 GetParkGroupId(u32 index) {
    ParkCategory *category = &data_020a4ccc[data_020d43f0[0].mUnk_5c2[index].mCategory];
    return (u8) category->mGroups[data_020d43f0[0].mUnk_5c2[index].mGroup].mId;
}

void func_02089a9c(u32 index) {
    ParkEntry entry;
    GetParkEntry(index, &entry);
    u32 flag = data_020d43f0[0].mUnk_5bf_0;
    if (flag && entry.mType != 0 && entry.mType != 1 && entry.mType != 2) {
        entry.mType = 0;
    }
    switch (entry.mType) {
        case 0:
        case 1:
        case 2: {
            if (data_020d43f0[0].mUnk_5c1 <= 4) {
                return;
            }
            u8 i;
            u8 slotCat = data_020d43f0[0].mUnk_5c2[index].mCategory;
            u8 slotGroup = data_020d43f0[0].mUnk_5c2[index].mGroup;
            u8 charSlot;
            if (flag) {
                charSlot = data_020d433c.mUnk_8d[index];
            }
            data_020d43f0[0].mUnk_5c2[index].mCategory = data_020d43f0[0].mUnk_5c2[4].mCategory;
            data_020d43f0[0].mUnk_5c2[index].mGroup = data_020d43f0[0].mUnk_5c2[4].mGroup;
            if (data_020d43f0[0].mUnk_5bf_0) {
                data_020d433c.mUnk_8d[index] = data_020d433c.mUnk_8d[4];
            }
            i = 4;
            do {
                data_020d43f0[0].mUnk_5c2[i].mCategory = data_020d43f0[0].mUnk_5c2[i + 1].mCategory;
                data_020d43f0[0].mUnk_5c2[i].mGroup    = data_020d43f0[0].mUnk_5c2[i + 1].mGroup;
                if (data_020d43f0[0].mUnk_5bf_0) {
                    data_020d433c.mUnk_8d[i] = data_020d433c.mUnk_8d[i + 1];
                }
                i++;
            } while (i < data_020d43f0[0].mUnk_5c1 - 1);
            data_020d43f0[0].mUnk_5c2[data_020d43f0[0].mUnk_5c1 - 1].mCategory = slotCat;
            data_020d43f0[0].mUnk_5c2[data_020d43f0[0].mUnk_5c1 - 1].mGroup = slotGroup;
            if (data_020d43f0[0].mUnk_5bf_0) {
                data_020d433c.mUnk_8d[data_020d43f0[0].mUnk_5c1 - 1] = charSlot;
            }
            break;
        }
        case 6: {
            if (entry.mUnk_0a == 0 && !(data_020d43f0[0].mUnk_5be & 0x10)) {
                return;
            }
            do {
                data_020d43f0[0].mUnk_5c2[index].mCategory = data_020d43f0[0].mUnk_5c2[index + 1].mCategory;
                data_020d43f0[0].mUnk_5c2[index].mGroup    = data_020d43f0[0].mUnk_5c2[index + 1].mGroup;
                index = (index + 1) & 0xff;
            } while (index < data_020d43f0[0].mUnk_5c1);
            data_020d43f0[0].mUnk_5c1--;
            break;
        }
    }
}

BOOL func_02089c7c(u32 id) {
    BOOL result = TRUE;
    if (func_020875ac(id)) {
        return FALSE;
    }
    switch (id) {
        case 6:
        case 7:
        case 8:
        case 9:
        case 10:
            if (data_020d43f0[0].mUnk_5b8 < 4) {
                result = FALSE;
            }
            break;
        case 11:
        case 12:
        case 13:
        case 14:
        case 15:
        case 16:
            if (!data_020d43f0[0].mUnk_5bf_0) {
                result = FALSE;
            }
            break;
        case 17:
            if (!(((u8 *) data_020d43f0)[0x586] & 1) || !func_02089ec8() || data_020d43f0[0].mUnk_5b8 < 5) {
                result = FALSE;
            }
            break;
        case 18:
            if (!(((u8 *) data_020d43f0)[0x586] & 4) || !func_02089ec8() || data_020d43f0[0].mUnk_5b8 < 5) {
                result = FALSE;
            }
            break;
        case 19:
            if (!(((u8 *) data_020d43f0)[0x586] & 0x10) || !func_02089ec8() || data_020d43f0[0].mUnk_5b8 < 5) {
                result = FALSE;
            }
            break;
        case 20:
            if (!(((u8 *) data_020d43f0)[0x586] & 0x80) || !func_02089ec8() || data_020d43f0[0].mUnk_5b8 < 5) {
                result = FALSE;
            }
            break;
        case 21:
            if (!(((u8 *) data_020d43f0)[0x587] & 0x8) || !func_02089ec8() || data_020d43f0[0].mUnk_5b8 < 5) {
                result = FALSE;
            }
            break;
        case 22:
            if (!(((u8 *) data_020d43f0)[0x587] & 0x2) || !func_02089ec8() || data_020d43f0[0].mUnk_5b8 < 5) {
                result = FALSE;
            }
            break;
        case 23:
        case 24:
            if (data_020d43f0[0].mUnk_5b8 < 6 || !func_02089f2c()) {
                result = FALSE;
            }
            break;
        case 25: {
            u8 i = 6;
            do {
                if (!func_020875ac(i)) {
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

BOOL func_02089ec8() {
    if (!func_020875ac(6) || !func_020875ac(7) || !func_020875ac(8) || !func_020875ac(9) || !func_020875ac(10)) {
        return FALSE;
    }
    return TRUE;
}

BOOL func_02089f2c() {
    if (!func_020875ac(11) || !func_020875ac(12) || !func_020875ac(13) || !func_020875ac(14) ||
        !func_020875ac(15)) {
        return FALSE;
    }
    return TRUE;
}
