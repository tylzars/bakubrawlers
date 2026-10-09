#include "Battle/BattleCpu.hpp"
#include "Battle/GimmickTypes.hpp"
#include "Game/Random.hpp"
#include "Game/UnkStruct_020d433c.hpp"
#include "Unknown/UnkFunctions.hpp"

u8 func_02065bf4(u32 id, u32 index);
extern const u8 data_ov007_022796e4[];
extern const u8 data_ov007_022796f8[];

BOOL func_ov007_02262574(BattlePlayerList *list, u8 row, u8 col);

struct BattleCpuRegion {
    u8 mRow;
    u8 mCol;
};

static const BattleCpuRegion data_ov007_022796d8[6] = {{3, 1}, {7, 1}, {7, 5}, {3, 5}, {5, 3}, {0, 0}};

struct BattleCpuRot {
    u16 x;
    u16 y;
    u16 z;
};

struct BattleCpuShot {
    s8 mTilt;
    u8 mPitch;
    u8 mPower;
    u8 mUnk_3;
};

static const u8 data_ov007_022796d0[5] = {95, 110, 125, 140, 155};

static const u8 data_ov007_0227970c[17] = {10, 60, 80, 40, 10, 10, 60, 20, 30, 70, 10, 10, 10, 5, 10, 50, 40};

static const u8 data_ov007_022797ac[17][5] = {
    {5, 10, 70, 10, 5},
    {10, 30, 20, 30, 10},
    {20, 20, 20, 20, 20},
    {15, 20, 30, 20, 15},
    {10, 10, 60, 10, 10},
    {20, 20, 20, 20, 20},
    {10, 30, 20, 30, 10},
    {0, 30, 40, 30, 0},
    {0, 45, 10, 45, 0},
    {25, 20, 10, 20, 25},
    {20, 30, 0, 30, 20},
    {20, 20, 20, 20, 20},
    {15, 30, 10, 30, 15},
    {0, 40, 20, 40, 0},
    {0, 25, 50, 25, 0},
    {0, 0, 30, 50, 20},
    {20, 50, 30, 0, 0}
};

static const u8 data_ov007_02279804[17][7] = {
    {10, 20, 20, 0, 20, 20, 10},
    {0, 5, 5, 25, 30, 25, 10},
    {20, 20, 0, 30, 20, 0, 10},
    {10, 10, 10, 20, 20, 20, 10},
    {0, 20, 30, 0, 20, 20, 10},
    {20, 20, 20, 10, 10, 10, 10},
    {10, 10, 20, 20, 20, 20, 0},
    {20, 20, 20, 10, 10, 10, 10},
    {10, 20, 30, 20, 10, 0, 10},
    {0, 10, 30, 20, 30, 10, 0},
    {0, 5, 45, 0, 5, 40, 5},
    {10, 10, 40, 10, 10, 10, 10},
    {10, 20, 15, 15, 20, 10, 10},
    {30, 10, 0, 20, 10, 10, 20},
    {10, 20, 0, 20, 10, 10, 30},
    {0, 0, 20, 10, 30, 30, 10},
    {10, 10, 0, 30, 30, 10, 10}
};

static const BattleCpuShot data_ov007_0227987c[8][4][3] = {
    {
        {{23, 15, 70, 0}, {26, 25, 70, 0}, {-21, 25, 70, 0}},
        {{23, 15, 70, 0}, {-7, 25, 70, 0}, {-21, 25, 70, 0}},
        {{23, 15, 70, 0}, {26, 25, 70, 0}, {-21, 25, 70, 0}},
        {{23, 15, 70, 0}, {-7, 25, 70, 0}, {-21, 25, 70, 0}},
    },
    {
        {{13, 30, 70, 0}, {16, 35, 75, 0}, {17, 43, 70, 0}},
        {{-13, 30, 70, 0}, {-28, 35, 75, 0}, {-17, 43, 70, 0}},
        {{13, 30, 70, 0}, {16, 35, 75, 0}, {17, 43, 70, 0}},
        {{-13, 30, 70, 0}, {-28, 35, 75, 0}, {-17, 43, 70, 0}},
    },
    {
        {{18, 36, 78, 0}, {27, 40, 78, 0}, {16, 45, 78, 0}},
        {{18, 36, 78, 0}, {-9, 45, 80, 0}, {-16, 40, 78, 0}},
        {{18, 36, 78, 0}, {27, 40, 78, 0}, {16, 45, 78, 0}},
        {{18, 36, 78, 0}, {-9, 45, 80, 0}, {-16, 40, 78, 0}},
    },
    {
        {{-4, 30, 70, 0}, {5, 30, 70, 0}, {-16, 30, 70, 0}},
        {{-4, 30, 70, 0}, {-3, 30, 70, 0}, {16, 30, 70, 0}},
        {{-4, 30, 70, 0}, {5, 30, 70, 0}, {-16, 30, 70, 0}},
        {{-4, 30, 70, 0}, {-3, 30, 70, 0}, {16, 30, 70, 0}},
    },
    {
        {{25, 30, 70, 0}, {20, 20, 70, 0}, {8, 23, 73, 0}},
        {{25, 25, 70, 0}, {30, 40, 70, 0}, {10, 20, 70, 0}},
        {{-27, 30, 70, 0}, {33, 40, 75, 0}, {8, 22, 70, 0}},
        {{25, 30, 70, 0}, {22, 20, 70, 0}, {-8, 20, 70, 0}},
    },
    {
        {{-20, 20, 70, 0}, {-15, 15, 70, 0}, {12, 25, 75, 0}},
        {{-21, 20, 70, 0}, {38, 10, 60, 0}, {-30, 15, 70, 0}},
        {{-20, 20, 70, 0}, {28, 10, 60, 0}, {12, 25, 75, 0}},
        {{-21, 20, 70, 0}, {15, 15, 70, 0}, {-30, 10, 70, 0}},
    },
    {
        {{-20, 17, 80, 0}, {-30, 25, 80, 0}, {9, 30, 80, 0}},
        {{20, 17, 80, 0}, {30, 25, 80, 0}, {9, 30, 80, 0}},
        {{-20, 17, 80, 0}, {-30, 25, 80, 0}, {9, 30, 80, 0}},
        {{20, 17, 80, 0}, {30, 25, 80, 0}, {9, 30, 80, 0}},
    },
    {
        {{40, 10, 80, 0}, {25, 15, 75, 0}, {23, 20, 70, 0}},
        {{40, 10, 80, 0}, {25, 15, 75, 0}, {-23, 20, 70, 0}},
        {{40, 10, 80, 0}, {25, 15, 75, 0}, {23, 20, 70, 0}},
        {{40, 10, 80, 0}, {25, 15, 75, 0}, {-23, 20, 70, 0}},
    },
};
static inline fx32 Abs(fx32 v);


BattleCpu::BattleCpu(u8 player) {
    mUnk_78 = 0;
    mUnk_7a = 0xff;
    mUnk_79 = player;
    mUnk_54 = 0;
    mUnk_76 = 0;
    mPos.x  = 0;
    mPos.y  = 0;
    mPos.z  = 0;
    mSavedPos.x = 0;
    mSavedPos.y = 0;
    mSavedPos.z = 0;
    for (u8 i = 0; i < 4; i++) {
        mTargets[i].x = 0;
        mTargets[i].y = 0;
        mTargets[i].z = 0;
    }
    for (u8 i = 0; i < 8; i++) {
        mUnk_85[i] = 0;
        mUnk_60[i] = 0;
    }
    mUnk_8d = 0;
    mUnk_70 = 0;
    mUnk_83 = 0;
    mUnk_84 = 0;
    mUnk_58 = 0;
    mUnk_5c = 3000;
    mUnk_74 = 0;
    mUnk_4c = 0;
    mUnk_50 = 0;
    mUnk_72 = 20;
}

BattleCpu::~BattleCpu() {
}

void BattleCpu::func_ov007_02231038() {
    mUnk_7b = 3;
    mUnk_7d = 0;
    if (data_020d433c.mUnk_98) {
        return;
    }
    u8 i = 0;
    u8 n = 0;
    do {
        mUnk_7d += data_020d433c.mBattlePlayers->mPlayers[i]->mUnk_ee;
        if (data_020d433c.mUnk_99 - 1 == data_020d433c.mBattlePlayers->mPlayers[i]->mUnk_ee) {
            if (i == mUnk_79) {
                mUnk_7b = 0;
            } else {
                n++;
            }
        }
        i++;
    } while (i < data_020d433c.mPlayerCount);
    if (n == 0) {
        return;
    }
    if (mUnk_7b == 0) {
        mUnk_7b = 1;
    } else {
        mUnk_7b = 5;
    }
}

// non-matching: registers only: the two hoisted loads land in ip/lr in the original.
u8 BattleCpu::func_ov007_022310f0() {
    u8 result = 0xff;
    if (data_020d433c.mUnk_85 != 0 || data_020d433c.mUnk_8a_0 != 0) {
        return 0xff;
    }
    BOOL c = FALSE;
    if (data_020d433c.mUnk_98) {
        BattlePlayerList *list = data_020d433c.mBattlePlayers;
        u8 a                   = list->mUnk_294;
        u8 b                   = list->mPlayers[mUnk_79]->mUnk_f2;
        u8 n                   = 0;
        u8 i                   = 0;
        BattleUnit *units      = list->Units();
        do {
            if (units[i].mUnk_10 != 0 && b == units[i].mUnk_0f) {
                n++;
            }
            i++;
        } while (i < 12);
        if (n == a) {
            c = TRUE;
        }
    }
    if (!data_020d433c.mUnk_9a_3) {
        if (MATH_Rand32(&data_020d42e8, 100) < 20) {
            c = TRUE;
        }
    }
    if (c) {
        BattlePlayer *pl       = data_020d433c.mBattlePlayers->mPlayers[mUnk_79];
        u8 count               = data_020d433c.mUnk_99;
        u8 free                = 0;
        u8 i                   = 0;
        do {
            if (pl->Flag56(i) == 0) {
                free++;
            }
            i++;
        } while (i < count);
        i = 0;
        do {
            if (pl->Flag56(i) == 0) {
                result = i;
                break;
            }
            i++;
        } while (i < count);
        if (free != 0) {
            u8 pick          = MATH_Rand32(&data_020d42e8, free);
            u8 k             = 0;
            u8 j             = 0;
            BattlePlayer *p2 = data_020d433c.mBattlePlayers->mPlayers[mUnk_79];
            do {
                if (p2->Flag56(j) == 0) {
                    if (pick == k) {
                        result = j;
                        break;
                    }
                    k++;
                }
                j++;
            } while (j < count);
        }
    }
    return result;
}

// non-matching: 15 words, registers only.
u8 BattleCpu::func_ov007_02231300() {
    u8 roll = MATH_Rand32(&data_020d42e8, 100);
    u8 t    = data_020d433c.mBattlePlayers->mPlayers[mUnk_79]->mUnk_f1;
    if (roll < 50) {
        t = 4;
    } else if (roll < 75) {
        if (t <= 1) {
            t = t + 2;
        } else {
            t = t - 2;
        }
    }
    const BattleCpuRegion *region = &data_ov007_022796d8[t];
    u8 row;
    u8 col;
    u8 n;
    u8 row0;
    u8 col0;
    u8 cells[12];
    row0 = region->mRow;
    col0 = region->mCol;
    n = 0;
    row = row0;
    do {
        col = col0;
        do {
            if (func_ov007_02262574(data_020d433c.mBattlePlayers, row, col)) {
                cells[n++] = col | (row << 4);
            }
            col++;
        } while (col < col0 + 3);
        row++;
    } while (row < row0 + 4);
    if (n != 0) {
        return cells[MATH_Rand32(&data_020d42e8, n) & 0xff];
    }
    u8 r = 0;
    do {
        u8 c = 0;
        do {
            if (func_ov007_02262574(data_020d433c.mBattlePlayers, r, c)) {
                return c | (r << 4);
            }
            c++;
        } while (c < 9);
        r++;
    } while (r < 14);
    return 0;
}

// non-matching: same size; registers, the add scheduled before the byte store in own[nOwn++], blt vs bcc, and a cmp/cmpne chain for type == 2 || type == 3.
extern "C" void func_ov007_022314bc(BattleCpu *cpu) {
    cpu->mPos.x = 0;
    cpu->mPos.y = 0;
    cpu->mPos.z = 0;
    u8 own[6]   = {0, 0, 0, 0, 0, 0};
    u8 other[9] = {0, 0, 0, 0, 0, 0, 0, 0, 0};
    u8 nOwn     = 0;
    u8 nOther   = 0;
    u8 cntA     = 0;
    u8 cntB     = 0;
    BattlePlayerList *list = data_020d433c.mBattlePlayers;
    u8 kind                = list->mPlayers[cpu->mUnk_79]->mUnk_f6;
    u8 i                   = 0;
    do {
        BattleMarker *m = &list->Entries()[i];
        if (m->mUnk_2 != 0) {
            if (cpu->mUnk_79 == m->mUnk_1) {
                own[nOwn++] = i;
            } else {
                other[nOther++] = i;
            }
            if (m->mUnk_5 == 0) {
                cntA++;
            } else {
                u8 owner = list->Units()[(u8) (m->mUnk_5 - 1)].mUnk_0f;
                if (owner != cpu->mUnk_79 && owner != list->mPlayers[cpu->mUnk_79]->mUnk_f2) {
                    cntB++;
                }
            }
        }
        i++;
    } while (i < 12);
    if (nOwn + nOther == 0) {
        return;
    }
    u8 type = list->mPlayers[cpu->mUnk_79]->mRecords[cpu->mUnk_7a].mUnk_09;
    cpu->mUnk_7e = 0;
    BattlePlayerList *list2 = data_020d433c.mBattlePlayers;
    BattlePlayer *pl        = list2->mPlayers[cpu->mUnk_79];
    if (pl->mRecords[cpu->mUnk_7a].mUnk_00_0 >= 2 && pl->mUnk_f3 == 0xbe) {
        u8 roll = MATH_Rand32(&data_020d42e8, 100);
        if (cpu->mUnk_7b == 0) {
            roll = 99;
        } else if (cpu->mUnk_7b == 1) {
            roll = 0;
            if (type == 0 || type == 2) {
                roll = 99;
            }
        } else if (cpu->mUnk_7b == 5) {
            roll = 0;
        }
        if (list2->mPlayers[cpu->mUnk_79]->mUnk_f7 == 2) {
            roll = 0;
        }
        if (roll < data_ov007_022796e4[kind]) {
            cpu->mUnk_7e = type + 1;
            if (cntB == 0) {
                switch (type) {
                    case 0:
                    case 2:
                    case 3:
                        cpu->mUnk_7e = 0;
                        break;
                    case 5:
                        if (cntA == 0) {
                            cpu->mUnk_7e = 0;
                        }
                        break;
                }
            }
        }
    }
    u8 state = 4;
    switch (data_ov007_022796f8[kind]) {
        case 0:
            if (cpu->mUnk_7b != 0) {
                state = 0;
            }
            break;
        case 1:
            if (cpu->mUnk_7b == 0 || cpu->mUnk_7b == 3) {
                state = 4;
            } else {
                state = 0;
            }
            break;
        case 2:
            state = 2;
            break;
        case 3:
            if (cpu->mUnk_7b == 0 || cpu->mUnk_7b == 3) {
                state = 1;
            } else {
                state = 0;
            }
            break;
        case 4:
            state = 5;
            break;
    }
    if (cpu->mUnk_7e != 0 && cntB != 0) {
        if (type == 0 || type == 2 || type == 3) {
            state = 3;
        }
    }
    u8 pick = 12;
    do {
        switch (state) {
            case 0: {
                u8 idx   = MATH_Rand32(&data_020d42e8, 12);
                u8 found = 0;
                u8 n     = 0;
                BattlePlayerList *l = data_020d433c.mBattlePlayers;
                do {
                    BattleUnit *u = &l->Units()[idx];
                    if (u->mUnk_10 != 0 && cpu->mUnk_79 == u->mUnk_0f) {
                        found++;
                        break;
                    }
                    idx++;
                    n++;
                    if (idx >= 12) {
                        idx = 0;
                    }
                } while (n < 12);
                if (found == 0) {
                    state = 4;
                } else {
                    pick = l->Units()[idx].mUnk_11;
                }
                break;
            }
            case 1:
                if (cntA != 0) {
                    u8 j = 0;
                    BattlePlayerList *l = data_020d433c.mBattlePlayers;
                    do {
                        BattleMarker *m = &l->Entries()[j];
                        if (m->mUnk_2 != 0 && m->mUnk_5 == 0) {
                            if (cpu->mUnk_79 == m->mUnk_1) {
                                pick = j;
                                break;
                            }
                            pick = j;
                        }
                        j++;
                    } while (j < 12);
                } else {
                    state = 0;
                }
                break;
            case 2:
            case 3:
                if (cntB != 0) {
                    u8 j = 0;
                    BattlePlayerList *l = data_020d433c.mBattlePlayers;
                    do {
                        BattleMarker *m = &l->Entries()[j];
                        if (m->mUnk_2 != 0 && m->mUnk_5 != 0) {
                            u8 owner = l->Units()[(u8) (m->mUnk_5 - 1)].mUnk_0f;
                            if (owner != cpu->mUnk_79 && owner != l->mPlayers[cpu->mUnk_79]->mUnk_f2) {
                                if (state == 2) {
                                    if (!l->mPlayers[owner]->IsActive()) {
                                        pick = j;
                                        break;
                                    }
                                } else {
                                    pick = j;
                                    break;
                                }
                            }
                        }
                        j++;
                    } while (j < 12);
                    if (pick == 12) {
                        if (state == 2) {
                            state = 3;
                        } else {
                            state = 0;
                        }
                    }
                } else {
                    state = 5;
                }
                break;
            case 4: {
                BattleRecord *rec = &data_020d433c.mBattlePlayers->mPlayers[cpu->mUnk_79]->mRecords[cpu->mUnk_7a];
                u8 recKind        = rec->mUnk_08;
                u16 recValue      = rec->mUnk_04;
                u8 j              = 0;
                do {
                    BattlePlayerList *l = data_020d433c.mBattlePlayers;
                    BattleMarker *m     = &l->Entries()[j];
                    if (m->mUnk_2 != 0) {
                        u8 owner = m->mUnk_1;
                        u16 val  = l->mPlayers[owner]->Slot54(m->mUnk_0);
                        bool ok  = false;
                        if (cpu->mUnk_79 == owner || owner == l->mPlayers[cpu->mUnk_79]->mUnk_f2) {
                            ok = cpu->func_ov007_02232e04(val, recValue, recKind);
                        }
                        if (ok == 1) {
                            pick = j;
                            break;
                        }
                    }
                    j++;
                } while (j < 12);
                if (pick >= 12) {
                    u8 bestIdx = 0;
                    u32 best   = 0;
                    u8 k       = 0;
                    do {
                        BattlePlayerList *l = data_020d433c.mBattlePlayers;
                        BattleMarker *m     = &l->Entries()[k];
                        if (m->mUnk_2 != 0) {
                            if (cpu->mUnk_79 == m->mUnk_1 || m->mUnk_1 == l->mPlayers[cpu->mUnk_79]->mUnk_f2) {
                                u32 v = func_02065bf4(m->mUnk_0, recKind);
                                if (v > best) {
                                    best    = v;
                                    bestIdx = k;
                                }
                            }
                        }
                        k++;
                    } while (k < 12);
                    if (best != 0) {
                        pick = bestIdx;
                    } else {
                        state = 5;
                    }
                }
                break;
            }
            case 5: {
                BOOL pickOwn = FALSE;
                if (MATH_Rand32(&data_020d42e8, 100) < 75) {
                    if (nOwn != 0) {
                        pickOwn = TRUE;
                    }
                } else {
                    if (nOther == 0) {
                        pickOwn = TRUE;
                    }
                }
                if (pickOwn) {
                    pick = own[MATH_Rand32(&data_020d42e8, nOwn) & 0xff];
                } else {
                    pick = other[MATH_Rand32(&data_020d42e8, nOther) & 0xff];
                }
                break;
            }
        }
    } while (pick >= 12);
    if (data_020d433c.mUnk_98 != 0) {
        BattlePlayerList *l = data_020d433c.mBattlePlayers;
        BattleMarker *m     = &l->Entries()[pick];
        if (m->mUnk_5 != 0) {
            u8 owner = l->Units()[(u8) (m->mUnk_5 - 1)].mUnk_0f;
            BattlePlayer *me = l->mPlayers[cpu->mUnk_79];
            if (owner == me->mUnk_f2) {
                u8 j = 0;
                do {
                    BattleMarker *m2 = &l->Entries()[j];
                    if (m2->mUnk_2 != 0) {
                        if (m2->mUnk_5 != 0) {
                            if (l->Units()[(u8) (m2->mUnk_5 - 1)].mUnk_0f != me->mUnk_f2) {
                                pick = j;
                                break;
                            }
                        } else {
                            pick = j;
                            break;
                        }
                    }
                    j++;
                } while (j < 12);
            }
        }
    }
    if (pick >= 12) {
        pick = 0;
    }
    BattleMarker *chosen = &data_020d433c.mBattlePlayers->Entries()[pick];
    cpu->mPos.x          = chosen->mUnk_3 * 0x23000 - 0xe3800;
    cpu->mPos.y          = 0;
    cpu->mPos.z          = chosen->mUnk_4 * 0x2f000 - 0xd3800;
    cpu->func_ov007_022328dc();
}

u8 BattleCpu::func_ov007_02231dd0() {
    u8 found         = 0;
    u8 i             = 0;
    BattlePlayer *pl = data_020d433c.mBattlePlayers->mPlayers[mUnk_79];
    u8 count         = data_020d433c.mUnk_99;
    do {
        if (pl->mRecords[i].mUnk_0a == 0) {
            found = i;
            break;
        }
        i++;
    } while (i < count);
    mUnk_7a = found;
    return found;
}

// non-matching: register allocation and stack slots only (396 of 684 words differ by register or
// spill slot; structure, call targets and literal pool match; the original spills mUnk_95 to sp+0x10).
void BattleCpu::func_ov007_02231e2c(BattleCpuRot *rot, u16 *outA, u16 *outB, u8 *outC) {
    u8 a    = data_020d433c.mBattlePlayers->mPlayers[mUnk_79]->mUnk_f6;
    u8 idx2 = 0;
    s8 tilt;
    u8 idx1 = 6;
    u8 power;
    u8 i    = 0;
    u8 spin;
    u8 roll = MATH_Rand32(&data_020d42e8, 100);
    u8 sum  = 0;
    u8 i2;
    u8 sum2;
    u8 roll2;
    do {
        sum += data_ov007_02279804[a][i];
        if (sum > roll) {
            idx1 = i;
            break;
        }
        i++;
    } while (i < 7);
    roll2 = MATH_Rand32(&data_020d42e8, 100);
    i2   = 0;
    sum2 = 0;
    do {
        sum2 += data_ov007_022797ac[a][i2];
        if (sum2 > roll2) {
            idx2 = i2;
            break;
        }
        i2++;
    } while (i2 < 5);
    BattleCpuRot r = {0, 0, 0};
    r.y = (u16) ((float) (MATH_Rand32(&data_020d42e8, 20) + 125) * 65536.0f / 360.0f);
    r.z = (u16) ((float) (MATH_Rand32(&data_020d42e8, 20) + 10) * 65536.0f / 360.0f);
    power = MATH_Rand32(&data_020d42e8, 30) + 50;
    spin  = 0;
    tilt  = 0;
    switch (idx1) {
    case 0:
        power = MATH_Rand32(&data_020d42e8, 20) + 60;
        spin  = MATH_Rand32(&data_020d42e8, 30) + 30;
        r.z  = (u16) ((float) (MATH_Rand32(&data_020d42e8, 20) + 20) * 65536.0f / 360.0f);
        break;
    case 1:
        power = MATH_Rand32(&data_020d42e8, 20) + 60;
        spin  = MATH_Rand32(&data_020d42e8, 30);
        r.z  = (u16) ((float) (MATH_Rand32(&data_020d42e8, 20) + 5) * 65536.0f / 360.0f);
        break;
    case 2:
        power = MATH_Rand32(&data_020d42e8, 20) + 60;
        spin  = 0;
        r.z  = (u16) ((float) (MATH_Rand32(&data_020d42e8, 10) + 5) * 65536.0f / 360.0f);
        break;
    case 3:
        power = MATH_Rand32(&data_020d42e8, 30) + 40;
        spin  = MATH_Rand32(&data_020d42e8, 30) + 30;
        r.z  = (u16) ((float) (MATH_Rand32(&data_020d42e8, 20) + 20) * 65536.0f / 360.0f);
        break;
    case 4:
        power = MATH_Rand32(&data_020d42e8, 30) + 40;
        spin  = MATH_Rand32(&data_020d42e8, 30);
        r.z  = (u16) ((float) (MATH_Rand32(&data_020d42e8, 20) + 5) * 65536.0f / 360.0f);
        break;
    case 5:
        power = MATH_Rand32(&data_020d42e8, 30) + 40;
        spin  = 0;
        r.z  = (u16) ((float) (MATH_Rand32(&data_020d42e8, 10) + 5) * 65536.0f / 360.0f);
        break;
    case 6:
        power = MATH_Rand32(&data_020d42e8, 30) + 10;
        spin  = MATH_Rand32(&data_020d42e8, 30);
        r.z  = (u16) ((float) (MATH_Rand32(&data_020d42e8, 20) + 5) * 65536.0f / 360.0f);
        break;
    }
    u8 attr = data_020d433c.mUnk_95;
    if (attr == 3) {
        r.y = (u16) ((float) (MATH_Rand32(&data_020d42e8, 30) + 120) * 65536.0f / 360.0f);
    } else {
        r.y = (u16) ((float) (data_ov007_022796d0[idx2] + MATH_Rand32(&data_020d42e8, 20)) * 65536.0f / 360.0f);
    }
    if (MATH_Rand32(&data_020d42e8, 100) < data_ov007_0227970c[a]) {
        u8 sel = data_020d433c.mUnk_96;
        u8 f1  = data_020d433c.mBattlePlayers->mPlayers[mUnk_79]->mUnk_f1;
        switch (sel) {
        case 0:
        case 1:
        case 2:
            if (MATH_Rand32(&data_020d42e8, 100) >= 100) {
                sel = MATH_Rand32(&data_020d42e8, 3);
            }
            break;
        default:
            sel = MATH_Rand32(&data_020d42e8, 3);
            break;
        }
        r.y   = 0x6000;
        power = data_ov007_0227987c[attr][f1][sel].mPower;
        r.z   = (u16) ((float) data_ov007_0227987c[attr][f1][sel].mPitch * 65536.0f / 360.0f);
        tilt  = data_ov007_0227987c[attr][f1][sel].mTilt;
        spin  = 0;
    }
    if (data_020d433c.mUnk_85 == 2) {
        spin  = 0;
        tilt  = 0;
        r.y  = 0x6000;
        r.z  = 0x1777;
        power = 0x3e;
    } else if (data_020d433c.mUnk_85 == 7) {
        spin  = 0;
        tilt  = 0;
        r.y  = 0x6000;
        r.z  = 0x1555;
        power = 0x32;
    }
    *rot = r;
    if (tilt < 0) {
        *outA = (u16) ((float) (tilt + 360) * 65536.0f / 360.0f);
    } else {
        *outA = (u16) ((float) tilt * 65536.0f / 360.0f);
    }
    *outB = (u16) ((float) spin * 65536.0f / 360.0f);
    *outC = power;
}

struct BattleCpuOdds {
    u8 mFirst[2];
    u8 mSecond[2];
};

static const BattleCpuOdds data_ov007_02279720[17] = {
    {{5, 50}, {40, 40}},
    {{0, 30}, {0, 50}},
    {{0, 40}, {0, 60}},
    {{0, 70}, {30, 50}},
    {{10, 30}, {30, 30}},
    {{70, 20}, {70, 20}},
    {{50, 30}, {70, 30}},
    {{45, 50}, {48, 52}},
    {{20, 10}, {40, 0}},
    {{20, 65}, {30, 70}},
    {{40, 25}, {70, 20}},
    {{30, 30}, {40, 40}},
    {{50, 0}, {70, 0}},
    {{0, 30}, {0, 40}},
    {{0, 10}, {0, 20}},
    {{0, 50}, {0, 90}},
    {{1, 20}, {5, 80}},
};

void BattleCpu::func_ov007_022328dc() {
    u8 roll;
    u8 b;
    u8 a;
    a    = data_020d433c.mBattlePlayers->mPlayers[mUnk_79]->mUnk_f6;
    roll = MATH_Rand32(&data_020d42e8, 100);
    b    = data_020d433c.mBattlePlayers->mPlayers[mUnk_79]->mUnk_f7;
    if (b >= 2) {
        b = 1;
    }
    if (data_020d433c.mUnk_7a != 0 && mUnk_79 == data_020d433c.mBattlePlayers->mPlayers[0]->mUnk_f2) {
        if (a != 0xd && a != 0xe) {
            b = 1;
        }
    }
    u8 low = data_ov007_02279720[a].mFirst[b];
    if (roll < low) {
        mUnk_77 = 0;
        return;
    }
    if (roll < low + data_ov007_02279720[a].mSecond[b]) {
        mUnk_77 = 1;
    } else {
        mUnk_77 = 2;
    }
}

// non-matching: registers and 4 bytes of size.
void BattleCpu::func_ov007_022329cc(int limit) {
    u8 cand[0x40];
    u8 i = 0;
    u8 n = 0;
    u8 k;
    do {
        GimmickNode *node = &data_020d433c.mGimmicks->mNodes[i];
        if (node && node->mGimmick) {
            Gimmick *g = node->mGimmick;
            if (g->mUnk_3a == 4 && (limit == 0 || g->mPosition.y < limit)) {
                cand[n++] = i;
            }
        }
        i++;
    } while (i < 0x40);
    if (n == 0) {
        return;
    }
    mUnk_84 = n;
    k = 0;
    do {
        if (n == k) {
            break;
        }
        u8 pick  = MATH_Rand32(&data_020d42e8, n);
        mUnk_7f[k] = cand[pick];
        if (k != 0) {
            u8 j = 0;
            do {
                if (mUnk_7f[j] == mUnk_7f[k]) {
                    j = k + 1;
                    break;
                }
                j++;
            } while (j < k);
            if (j == k + 1) {
                k--;
                goto next;
            }
        }
        {
            GimmickNode *node = &data_020d433c.mGimmicks->mNodes[mUnk_7f[k]];
            if (node && node->mGimmick) {
                mTargets[k] = node->mGimmick->mPosition;
            }
        }
    next:
        k++;
    } while (k < 4);
    mUnk_76    = 1;
    mSavedPos  = mPos;
    mPos       = mTargets[mUnk_83];
    mUnk_54    = MATH_Rand32(&data_020d42e8, 301) + 0x47e;
    mUnk_58    = 0xe10;
}

void BattleCpu::func_ov007_02232bfc() {
    if (mUnk_76 != 1) {
        return;
    }
    if (mUnk_84 == 0) {
        return;
    }
    mUnk_83 = (mUnk_83 + 1) % mUnk_84;
    mPos    = mTargets[mUnk_83];
}

void BattleCpu::func_ov007_02232c5c() {
    mUnk_76   = 0;
    mPos      = mSavedPos;
    mUnk_54   = 0;
    mUnk_84   = 0;
}

void BattleCpu::func_ov007_02232c88() {
    mUnk_70 = 0;
    u8 i    = 0;
    do {
        mUnk_85[i] = MATH_Rand32(&data_020d42e8, 10);
        if (mUnk_85[i] == 0) {
            mUnk_60[i] = MATH_Rand32(&data_020d42e8, 60) + 90;
        } else {
            mUnk_60[i] = MATH_Rand32(&data_020d42e8, 60) + 60;
        }
        i++;
    } while (i < 8);
    func_ov007_02232d8c();
}

void BattleCpu::func_ov007_02232d8c() {
    if (mUnk_85[mUnk_8d] >= 9) {
        return;
    }
    s32 offsets[9][2] = {
        {0, 0}, {0, 0x59a}, {0x333, 0x2cd}, {0x666, 0}, {0x333, -0x2cd}, {0, -0x59a}, {-0x333, -0x2cd}, {-0x666, 0}, {-0x333, 0x2cd},
    };
    mUnk_4c = offsets[mUnk_85[mUnk_8d]][0];
    mUnk_50 = offsets[mUnk_85[mUnk_8d]][1];
}

bool BattleCpu::func_ov007_02232e04(u32 a, u32 b, u32 c) {
    bool result = false;
    if (func_02065c0c(a, 3) == 1) {
        u8 t = a - 1;
        if (a >= 7) {
            t++;
        }
        if (c == t) {
            result = true;
        }
    } else {
        switch (a) {
        case 0x4a:
        case 0x52:
        case 0x53:
        case 0x54:
        case 0x55:
        case 0x56:
        case 0x57:
        case 0x58:
        case 0x59:
        case 0x5a:
        case 0x5b:
        case 0x5c:
            if (b < 200) {
                result = true;
            }
            break;
        }
    }
    return result;
}

void BattleCpu::func_ov007_02232eb8(s32 *x, s32 *y) {
    if (data_020d433c.mBattlePlayers->mPlayers[mUnk_79]->mUnk_f7 != 0 && mUnk_70 % mUnk_72 == 0) {
        *x = mUnk_4c;
        *y = mUnk_50;
        return;
    }
    *x = 0;
    *y = 0;
}

void func_01ffcf80(MtxFx33 *m, u16 angle);

void func_01ffcfd8(MtxFx33 *m, u16 angle);

void func_01ffd030(MtxFx33 *m, u16 angle);

BOOL func_ov007_02232f24(VecFx32 *pos, VecFx32 *vel);

BOOL func_ov007_02232f24(VecFx32 *pos, VecFx32 *vel) {
    VecFx32 p = *pos;
    VecFx32 v = *vel;
    BOOL r    = TRUE;
    if (p.y < 0x899a) {
        if ((v.x < 0 ? -v.x : v.x) <= 0xcd) {
            v.x = 0;
        } else {
            v.x = FX_Mul(v.x, 0xf85);
        }
        if ((v.z < 0 ? -v.z : v.z) <= 0xcd) {
            v.z = 0;
        } else {
            v.z = FX_Mul(v.z, 0xf85);
        }
        p.y = 0x8999;
    } else {
        v.y -= 0x1c4 - r;
    }
    if (v.x == 0 && v.y == 0 && v.z == 0) {
        r = FALSE;
    }
    VEC_Add(&p, &v, &p);
    *pos = p;
    *vel = v;
    return r;
}

void func_ov007_02233078(VecFx32 *pos, VecFx32 *vel, int steps, VecFx32 *out) {
    VecFx32 v = *vel;
    v.x       = FX_Mul(v.x, 0xb800);
    v.y       = FX_Mul(v.y, 0xb800);
    v.z       = FX_Mul(v.z, 0xb800);
    VecFx32 p = *pos;
    if (steps != 0) {
        do {
            if (!func_ov007_02232f24(&p, &v)) {
                break;
            }
            steps--;
        } while (steps != 0);
    }
    *out = p;
}

int func_ov007_02233158(VecFx32 *pos, VecFx32 *vel, VecFx32 *out) {
    VecFx32 p = *pos;
    VecFx32 v = *vel;
    v.x       = FX_Mul(v.x, 0xb800);
    v.y       = FX_Mul(v.y, 0xb800);
    v.z       = FX_Mul(v.z, 0xb800);
    int n     = 0;
    if (vel->x == 0 && vel->y == 0 && vel->z == 0) {
        return 0;
    }
    do {
        func_ov007_02232f24(&p, &v);
    } while (n++ == 0 || p.y >= 0x899a);
    *out = p;
    return n;
}

int func_ov007_02233270(VecFx32 *pos, VecFx32 *vel, VecFx32 *out, u32 unk4, u32 unk5) {
    VecFx32 p = *pos;
    VecFx32 v = *vel;
    v.x       = FX_Mul(v.x, 0xb800);
    v.y       = FX_Mul(v.y, 0xb800);
    v.z       = FX_Mul(v.z, 0xb800);
    u32 t     = 0;
    int n     = 0;
    for (;;) {
        fx32 vy = v.y;
        p.y     = (s32) (25805.0f + (9421.0f + ((float) (u32) (vy * t) - 430.0f * (float) (u32) (t * t) / 2.0f)));
        t++;
        if (p.y < 0x899a || vy == 0) {
            t = 0;
            if ((vy < 0 ? -vy : vy) <= 0x19a) {
                v.y = 0;
            } else {
                v.y = FX_Mul(vy, 0x533);
            }
            if ((v.x < 0 ? -v.x : v.x) <= 0xcd) {
                v.x = 0;
            } else {
                v.x = FX_Mul(v.x, 0xf85);
            }
            if ((v.z < 0 ? -v.z : v.z) <= 0xcd) {
                v.z = 0;
            } else {
                v.z = FX_Mul(v.z, 0xf85);
            }
        }
        if (v.x == 0 && v.y == 0 && v.z == 0) {
            *out = p;
            return n;
        }
        p.x += v.x;
        p.z += v.z;
        n++;
    }
}

void func_ov007_022334f4(u16 *rot, u32 s, VecFx32 *dir, VecFx32 *out) {
    VEC_Normalize(dir, out);
    MtxFx33 m = data_0220e848;
    if (rot[1]) {
        func_01ffcfd8(&m, rot[1]);
    }
    if (rot[0]) {
        func_01ffcf80(&m, rot[0]);
    }
    if (rot[2]) {
        func_01ffd030(&m, rot[2]);
    }
    func_01ff84d0(out, &m, out);
    out->x = FX_Mul(out->x, FX32_CONST((float) s / 150.0f));
    out->y = FX_Mul(out->y, FX32_CONST((float) s / 150.0f));
    out->z = FX_Mul(out->z, FX32_CONST((float) s / 150.0f));
}

void func_ov007_02233730(VecFx32 *a, VecFx32 *b, VecFx32 *speed, VecFx32 *out) {
    fx32 dx = b->x - a->x;
    fx32 dz = b->z - a->z;
    VecFx32 d;
    d.x = dx;
    d.y = 0;
    d.z = dz;
    VEC_Normalize(&d, &d);
    fx32 len = VEC_Mag(speed);
    d.x      = FX_Mul(d.x, len);
    d.y      = FX_Mul(d.y, len);
    d.z      = FX_Mul(d.z, len);
    out->x   = d.x;
    out->y   = d.y;
    out->z   = d.z;
}

// non-matching: registers in the flag != 0 branch (r2 vs r3 for the FX_Mul temporary).
void func_ov007_022337ec(VecFx32 *A, VecFx32 *B, VecFx32 *C, VecFx32 *D, VecFx32 *E, fx32 F, fx32 G, int flag) {
    if (flag) {
        fx32 len = VEC_Mag(B);
        fx32 t;
        t    = FX_Mul(E->x, len);
        B->x = FX_Mul(t, F);
        t    = FX_Mul(E->y, len);
        B->y = FX_Mul(t, G);
        t    = FX_Mul(E->z, len);
        B->z = FX_Mul(t, F);
        t    = FX_Mul(E->x, Abs(C->x));
        C->x = FX_Mul(t, F);
        t    = FX_Mul(E->y, Abs(C->y));
        C->y = FX_Mul(t, G);
        t    = FX_Mul(E->z, Abs(C->z));
        C->z = FX_Mul(t, F);
        len  = VEC_Mag(D);
        t    = FX_Mul(E->x, len);
        D->x = FX_Mul(t, F);
        t    = FX_Mul(E->z, len);
        D->z = FX_Mul(t, F);
        return;
    }
    VecFx32 e = *E;
    VecFx32 a = *A;
    VecFx32 ratio = {0, 0, 0};
    if (VEC_Mag(&a) > 0) {
        e.x = FX_Mul(e.x, 0x29);
        e.y = FX_Mul(e.y, 0x29);
        e.z = FX_Mul(e.z, 0x29);
        VEC_Normalize(&e, &e);
        VEC_Normalize(&a, &a);
        if (a.x != 0) {
            float fa = a.x > 0 ? 0.5f + (float) (a.x << 12) : (float) (a.x << 12) - 0.5f;
            float fb = A->x > 0 ? 0.5f + (float) (A->x << 12) : (float) (A->x << 12) - 0.5f;
            ratio.x  = func_01ff8000((s32) fb, (s32) fa);
        }
        if (a.y != 0) {
            float fa = a.y > 0 ? 0.5f + (float) (a.y << 12) : (float) (a.y << 12) - 0.5f;
            float fb = A->y > 0 ? 0.5f + (float) (A->y << 12) : (float) (A->y << 12) - 0.5f;
            ratio.y  = func_01ff8000((s32) fb, (s32) fa);
        }
        if (a.z != 0) {
            float fa = a.z > 0 ? 0.5f + (float) (a.z << 12) : (float) (a.z << 12) - 0.5f;
            float fb = A->z > 0 ? 0.5f + (float) (A->z << 12) : (float) (A->z << 12) - 0.5f;
            ratio.z  = func_01ff8000((s32) fb, (s32) fa);
        }
        a.x = FX_Mul(a.x, FX32_CONST(-1.0f));
        a.y = FX_Mul(a.y, FX32_CONST(-1.0f));
        a.z = FX_Mul(a.z, FX32_CONST(-1.0f));
        fx32 dot = VEC_DotProduct(&a, &e) * 2;
        e.x      = FX_Mul(e.x, dot);
        e.y      = FX_Mul(e.y, dot);
        e.z      = FX_Mul(e.z, dot);
        VEC_Subtract(&e, &a, &a);
        *B = a;
        u8 i = 0;
        do {
            fx32 r = (&ratio.x)[i];
            if (r != 0) {
                B->x = FX_Mul(B->x, r);
                B->y = FX_Mul(B->y, r);
                B->z = FX_Mul(B->z, r);
                break;
            }
            i++;
        } while (i < 3);
        if (E->y == 0) {
            B->x = FX_Mul(B->x, F);
            B->y = FX_Mul(B->y, F);
            B->z = FX_Mul(B->z, F);
        } else {
            B->y = FX_Mul(B->y, G);
        }
    }
    C->x = 0;
    C->y = 0;
    C->z = 0;
    D->x = 0;
    D->y = 0;
    D->z = 0;
}

void func_ov007_02233ed8(VecFx32 *in, u16 angle, VecFx32 *out) {
    MtxFx33 m = data_0220e848;
    func_01ffcfd8(&m, angle);
    func_01ff84d0(in, &m, out);
}


static inline fx32 Abs(fx32 v) {
    return v < 0 ? -v : v;
}

void func_ov007_02233f38(const VecFx32 *a, const VecFx32 *b, int unused, VecFx32 *out) {
    VEC_Subtract(a, b, out);
    VEC_Normalize(out, out);
}
