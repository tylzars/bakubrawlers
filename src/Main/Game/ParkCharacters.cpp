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
void func_02089924(u32 index, void *out);
BOOL func_02089c7c(u32 id);
BOOL func_02089ec8();
BOOL func_02089f2c();

// Moved here from UnkStruct_02087898.cpp, where it currently sits past the end of that unit (0x02088f38).
s32 func_02088f38() {
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
    return stage == 9 && data_020d43f0[0].mUnk_43 == 5 ? 26 : 27;
}

void func_02089208() {
    u8 i = 0;
    u8 buf[12];
    data_020d43f0[0].mUnk_5b9_1 = 0;
    do {
        func_02089924(i, buf);
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

// non-matching: same code shape, register allocation differs (the original keeps the 5c1 count spilled at [sp] and
// the loop variables in r5-r9/sl/fp in a different order); 0x6a4 vs 0x6ac bytes.
void func_02089278() {
    if (data_020d43f0[0].mUnk_5bf_0) {
        ParkPick picks[26] = {0};
        u8 count           = 0;
        u8 cat             = 0;
        do {
            ParkCategory *category = &data_020a4ccc[cat];
            ParkGroup *group       = category->mGroups;
            u8 g                   = 0;
            do {
                ParkEntry *entry = group->mEntries;
                u8 e             = 0;
                do {
                    if (entry->mUnk_06 != 0xff && func_02089c7c(entry->mUnk_06)) {
                        BOOL duplicate = FALSE;
                        u8 k;
                        for (k = 0; k < count; k++) {
                            if (group->mId == picks[k].mId) {
                                duplicate = TRUE;
                                break;
                            }
                        }
                        if (!duplicate) {
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
            s32 last = count - 1;
            u8 i;
            u8 j;
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
        u8 i;
        for (i = 0; i < count; i++) {
            data_020d43f0[0].mUnk_5c2[i].mCategory = picks[i].mCategory;
            data_020d43f0[0].mUnk_5c2[i].mGroup    = picks[i].mGroup;
        }
        if (count < 8) {
            u8 pool[16] = {0, 1, 2, 3, 4, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16};
            u8 chosen[16] = {0};
            u8 used       = 0;
            u8 p          = 0;
            do {
                u8 j;
                for (j = 0; j < count; j++) {
                    if (pool[p] == picks[j].mId) {
                        pool[p] = 0xff;
                    }
                }
                u8 v = pool[p];
                p++;
                if (v != 0xff) {
                    chosen[used] = v;
                    used++;
                }
            } while (p < 16);
            u8 s = 0;
            do {
                u8 r = MATH_Rand32(&data_020d42e8, used);
                u8 t = chosen[s];
                chosen[s] = chosen[r];
                chosen[r] = t;
                s++;
            } while (s < used);
            u8 w = 0;
            do {
                u8 c = 0;
                do {
                    ParkCategory *category = &data_020a4ccc[c];
                    ParkGroup *group       = category->mGroups;
                    u8 g                   = 0;
                    do {
                        if (chosen[w] == group->mId) {
                            ParkEntry *entry = group->mEntries;
                            u8 e             = 0;
                            do {
                                if (entry->mType <= 2) {
                                    data_020d43f0[0].mUnk_5c2[count].mCategory = c;
                                    data_020d43f0[0].mUnk_5c2[count].mGroup    = g;
                                    count++;
                                    g = category->mCount;
                                    c = 0x1c;
                                    if (count >= 8) {
                                        w = used;
                                    }
                                    break;
                                }
                                e++;
                                entry++;
                            } while (e < group->mCount);
                        }
                        g++;
                        group++;
                    } while (g < category->mCount);
                    c++;
                } while (c < 0x1c);
                w++;
            } while (w < used);
        }
        data_020d43f0[0].mUnk_5c1 = 8;
        u8 s = 0;
        do {
            u8 r = MATH_Rand32(&data_020d42e8, 8);
            u8 t0 = data_020d43f0[0].mUnk_5c2[s].mCategory;
            u8 t1 = data_020d43f0[0].mUnk_5c2[s].mGroup;
            data_020d43f0[0].mUnk_5c2[s].mCategory = data_020d43f0[0].mUnk_5c2[r].mCategory;
            data_020d43f0[0].mUnk_5c2[s].mGroup    = data_020d43f0[0].mUnk_5c2[r].mGroup;
            data_020d43f0[0].mUnk_5c2[r].mCategory = t0;
            data_020d43f0[0].mUnk_5c2[r].mGroup    = t1;
            s++;
        } while (s < data_020d43f0[0].mUnk_5c1);
        u8 perm[4] = {0, 0, 1, 2};
        u8 n = 0;
        do {
            ParkCategory *category = &data_020a4ccc[data_020d43f0[0].mUnk_5c2[n].mCategory];
            ParkGroup *group       = &category->mGroups[data_020d43f0[0].mUnk_5c2[n].mGroup];
            ParkEntry *entry       = group->mEntries;
            u8 k                   = 0;
            do {
                u8 r = MATH_Rand32(&data_020d42e8, 4);
                u8 t = perm[k];
                perm[k] = perm[r];
                perm[r] = t;
                k++;
            } while (k < 4);
            u8 e = 0;
            do {
                if (entry->mType == perm[0]) {
                    data_020d433c.mUnk_8d[n] = e;
                    break;
                }
                e++;
                entry++;
            } while (e < group->mCount);
            n++;
        } while (n < data_020d43f0[0].mUnk_5c1);
    } else {
        s32 index              = func_02088f38();
        ParkCategory *category = &data_020a4ccc[index];
        ParkGroup *group       = category->mGroups;
        u8 count               = 0;
        u8 g                   = 0;
        do {
            ParkEntry *entry = &group->mEntries[group->mDefault];
            BOOL skip        = FALSE;
            if (entry->mType == 6) {
                if (entry->mUnk_0a == 0) {
                    if ((data_020d43f0[0].mUnk_5be & 8) && (data_020d43f0[0].mUnk_5be & 0x10)) {
                        skip = TRUE;
                    }
                } else if (data_020d43f0[0].mUnk_5be & 0x10) {
                    skip = TRUE;
                }
            }
            if (!skip) {
                data_020d43f0[0].mUnk_5c2[count].mCategory = index;
                data_020d43f0[0].mUnk_5c2[count].mGroup    = g;
                count++;
            }
            g++;
            group++;
        } while (g < category->mCount);
        data_020d43f0[0].mUnk_5c1 = count;
    }
    func_02089208();
}

// non-matching: register allocation only (group/entries come out in r9/r8 where the original has r8/r9, and
// category/groups are swapped); exhaustive permutation of the six locals gives 10 differing instructions.
void func_02089924(u32 index, void *out) {
    u8 group;
    ParkGroup *groups;
    BOOL found;
    u8 i;
    u8 category;
    ParkEntry *entries;
    category = data_020d43f0[0].mUnk_5c2[index].mCategory;
    ParkCategory *cat = &data_020a4ccc[category];
    group  = data_020d43f0[0].mUnk_5c2[index].mGroup;
    groups  = cat->mGroups;
    entries = groups[group].mEntries;
    found         = FALSE;
    u32 type           = entries[groups[group].mDefault].mType;
    if ((type != 3 && type != 4 && type != 5) || data_020d43f0[0].mUnk_5bf_0) {
        i = 0;
        if (data_020d43f0[0].mUnk_5bf_0) {
            category = 0xff;
        }
        do {
            if (entries->mUnk_06 != 0xff && category >= entries->mUnk_07 && func_02089c7c(entries->mUnk_06)) {
                MI_CpuCopy8(entries, out, 12);
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
    entries = groups[group].mEntries;
    if (data_020d43f0[0].mUnk_5bf_0) {
        MI_CpuCopy8(&entries[data_020d433c.mUnk_8d[index]], out, 12);
    } else {
        MI_CpuCopy8(&entries[groups[group].mDefault], out, 12);
    }
}

u32 func_02089a64(u32 index) {
    ParkCategory *category = &data_020a4ccc[data_020d43f0[0].mUnk_5c2[index].mCategory];
    return (u8) category->mGroups[data_020d43f0[0].mUnk_5c2[index].mGroup].mId;
}

// non-matching: the original re-loads the data_020d43f0+0x5c2 pool constants before the stores into the slot list
// (one extra ldr, 0x1dc vs 0x1e0 bytes) and numbers flag/slotGroup r2/r3 where this gets r1/r1.
void func_02089a9c(u32 index) {
    ParkEntry entry;
    func_02089924(index, &entry);
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
            u8 slotGroup = data_020d43f0[0].mUnk_5c2[index].mGroup;
            u8 charSlot;
            if (flag) {
                charSlot = data_020d433c.mUnk_8d[index];
            }
            u8 slotCat = data_020d43f0[0].mUnk_5c2[index].mCategory;
            data_020d43f0[0].mUnk_5c2[index].mCategory = data_020d43f0[0].mUnk_5c2[4].mCategory;
            data_020d43f0[0].mUnk_5c2[index].mGroup = data_020d43f0[0].mUnk_5c2[4].mGroup;
            if (data_020d43f0[0].mUnk_5bf_0) {
                data_020d433c.mUnk_8d[index] = data_020d433c.mUnk_8d[4];
            }
            u8 i = 4;
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
