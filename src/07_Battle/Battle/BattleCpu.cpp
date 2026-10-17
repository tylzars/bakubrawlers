#include "Battle/BattleCpu.hpp"
#include "Battle/GimmickTypes.hpp"
#include "Game/Random.hpp"
#include "Game/UnkStruct_020d433c.hpp"
#include "Unknown/UnkFunctions.hpp"

u8 GetCardAttributeValue(u32 id, u32 index);

BOOL IsMarkerPlaceable(BattlePlayerList *list, u8 x, u8 y);

// Row and column pairs of the six target regions. One-dimensional u8 tables get a .rodata section each, which is
// what puts each table of this file on a 4-byte boundary in the original; a struct or 2-D array would share one
// section with the others.
static const u8 sMarkerBlockStarts[12] = {3, 1, 7, 1, 7, 5, 3, 5, 5, 3, 0, 0};

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

static const u8 data_ov007_022796f8[17] = {2, 4, 3, 0, 2, 0, 1, 0, 2, 0, 2, 4, 2, 2, 1, 2, 1};

static const u8 data_ov007_022796e4[17] = {90, 60, 70, 75, 50, 30, 60, 100, 80, 40, 80, 90, 60, 30, 20, 90, 60};

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

// Seven weights per row, one row per value of the player's mCharacterId, summed until they pass a random roll. A
// function-local static of an inline accessor gets a section of its own; defined ahead of ShotTable() it keeps its
// place before that table.
typedef u8 RollWeightRow[7];
inline const RollWeightRow *RollWeights() {
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
        {10, 10, 0, 30, 30, 10, 10},
    };
    return data_ov007_02279804;
}

typedef BattleCpuShot BattleCpuShotTable[8][4][3];

// A static of an inline function gets its own section too (the table comes last in the original's .rodata, after the
// one-dimensional ones), and indexing it still folds each field offset into the address word.
inline const BattleCpuShotTable &ShotTable() {
    static const BattleCpuShot sTable[8][4][3] = {
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
    return sTable;
}
static inline fx32 Abs(fx32 v);


BattleCpu::BattleCpu(u32 player) {
    mUnk_78 = 0;
    mRecord = 0xff;
    mPlayer = player;
    mUnk_54 = 0;
    mUsingTargets = 0;
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
        mOffsetEntries[i] = 0;
        mOffsetFrameCounts[i] = 0;
    }
    mOffsetEntryIndex = 0;
    mOffsetFrames = 0;
    mTargetIndex = 0;
    mJumpObjCount = 0;
    mUnk_58 = 0;
    mUnk_5c = 3000;
    mTimer = 0;
    mUnk_4c = 0;
    mUnk_50 = 0;
    mOffsetInterval = 20;
}

BattleCpu::~BattleCpu() {
}

void BattleCpu::func_ov007_02231038() {
    mUnk_7b = 3;
    mUnk_7d = 0;
    if (data_020d433c.mHasPartners) {
        return;
    }
    u8 i = 0;
    u8 n = 0;
    do {
        mUnk_7d += data_020d433c.mBattlePlayers->mPlayers[i]->mUnk_ee;
        if (data_020d433c.mUnk_99 - 1 == data_020d433c.mBattlePlayers->mPlayers[i]->mUnk_ee) {
            if (i == mPlayer) {
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

u8 BattleCpu::func_ov007_022310f0() {
    u8 result = 0xff;
    if (data_020d433c.mTutorialId != 0 || data_020d433c.mUnk_8a_0 != 0) {
        return 0xff;
    }
    BOOL c = FALSE;
    if (data_020d433c.mHasPartners) {
        BattlePlayerList *list = data_020d433c.mBattlePlayers;
        u8 a                   = list->MarkerCount();
        u8 b                   = list->mPlayers[mPlayer]->Partner();
        u8 i                   = 0;
        BattleUnit *u;
        u8 n                   = 0;
        do {
            u = &list->Units()[i];
            if (u->mStatus != 0 && b == u->mPlayer) {
                n++;
            }
            i++;
        } while (i < 12);
        if (n == a) {
            c = TRUE;
        }
    }
    if (!data_020d433c.mUnk_9a_3) {
        if (MATH_Rand32(&gRandom, 100) < 20) {
            c = TRUE;
        }
    }
    if (c) {
        u8 free;
        u8 count;
        u32 i;
        u8 m;
        BattlePlayer *pl;
        i     = 0;
        count = data_020d433c.mUnk_99;
        pl    = data_020d433c.mBattlePlayers->mPlayers[mPlayer];
        free  = 0;
        do {
            if (pl->Flag56(i) == 0) {
                free++;
            }
            i = (u8) (i + 1);
        } while (i < count);
        m = 0;
        do {
            if (pl->Flag56(m) == 0) {
                result = m;
                break;
            }
            m++;
        } while (m < count);
        if (free != 0) {
            u8 pick          = MATH_Rand32(&gRandom, free);
            u8 k             = 0;
            BattlePlayer *p2 = data_020d433c.mBattlePlayers->mPlayers[mPlayer];
            u8 j             = 0;
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

u8 BattleCpu::PickMarkerCell() {
    u8 roll = MATH_Rand32(&gRandom, 100);
    u8 t    = data_020d433c.mBattlePlayers->mPlayers[mPlayer]->mUnk_f1;
    if (roll < 50) {
        t = 4;
    } else if (roll < 75) {
        if (t <= 1) {
            t = t + 2;
        } else {
            t = t - 2;
        }
    }
    u8 row;
    u8 col;
    u8 n;
    u8 row0;
    u8 cells[12];
    row0 = sMarkerBlockStarts[t * 2];
    n = 0;
    row = row0;
    do {
        col = sMarkerBlockStarts[t * 2 + 1];
        do {
            if (IsMarkerPlaceable(data_020d433c.mBattlePlayers, row, col)) {
                cells[n] = col | (row << 4);
                n++;
            }
            col++;
        } while (col < sMarkerBlockStarts[t * 2 + 1] + 3);
        row++;
    } while (row < row0 + 4);
    if (n != 0) {
        return cells[MATH_Rand32(&gRandom, n) & 0xff];
    }
    u8 r = 0;
    do {
        u8 c = 0;
        do {
            if (IsMarkerPlaceable(data_020d433c.mBattlePlayers, r, c)) {
                return c | (r << 4);
            }
            c++;
        } while (c < 9);
        r++;
    } while (r < 14);
    return 0;
}

// Record index read into an enum-typed local: a real load (mwcc does not substitute enum locals), so the index is
// coloured after the player and record base (case 4).
enum RecordIndex { RECORD_INDEX_0 };

// The search loop of the final mHasPartners pass reads mMarkers directly (m2), not through the Markers() accessor.
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
    u8 type;
    BattlePlayerList *list = data_020d433c.mBattlePlayers;
    u8 kind                = list->mPlayers[cpu->mPlayer]->mCharacterId;
    u8 i                   = 0;
    do {
        BattleMarker *m = &list->Markers()[i];
        if (m->mStatus != 0) {
            if (cpu->mPlayer == m->mPlayer) {
                own[nOwn] = i;
                nOwn++;
            } else {
                other[nOther] = i;
                nOther++;
            }
            if (m->mUnitHandle == 0) {
                cntA++;
            } else {
                u8 owner = list->Units()[(u8) (m->mUnitHandle - 1)].mPlayer;
                if (owner != cpu->mPlayer && owner != list->mPlayers[cpu->mPlayer]->Partner()) {
                    cntB++;
                }
            }
        }
        i++;
    } while (i < 12);
    if (nOwn + nOther == 0) {
        return;
    }
    type = list->mPlayers[cpu->mPlayer]->mRecords[cpu->mRecord].mAttribute;
    cpu->mUnk_7e = 0;
    BattlePlayerList *list2 = data_020d433c.mBattlePlayers;
    BattlePlayer *pl        = list2->mPlayers[cpu->mPlayer];
    if ((s32) pl->mRecords[cpu->mRecord].mUnk_00_0 >= 2 && pl->mUnk_f3 == 0xbe) {
        u8 roll = MATH_Rand32(&gRandom, 100);
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
        if (list2->mPlayers[cpu->mPlayer]->mUnk_f7 == 2) {
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
            if (cpu->mUnk_7b == 3 || cpu->mUnk_7b == 0) {
                state = 4;
            } else {
                state = 0;
            }
            break;
        case 2:
            state = 2;
            break;
        case 3:
            if (cpu->mUnk_7b == 3 || cpu->mUnk_7b == 0) {
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
        switch (type) {
            case 0:
            case 2:
            case 3:
                state = 3;
                break;
        }
    }
    u8 j;
    u8 pick = 12;
    do {
        switch (state) {
            case 0: {
                u8 idx   = MATH_Rand32(&gRandom, 12);
                u8 found = 0;
                BattlePlayerList *l;
                u8 n     = 0;
                l = data_020d433c.mBattlePlayers;
                do {
                    BattleUnit *u = &l->Units()[idx];
                    if (u->mStatus != 0 && cpu->mPlayer == u->mPlayer) {
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
                    pick = l->Units()[idx].mMarker;
                }
                break;
            }
            case 1:
                if (cntA != 0) {
                    j = 0;
                    BattlePlayerList *l = data_020d433c.mBattlePlayers;
                    do {
                        BattleMarker *m = &l->Markers()[j];
                        if (m->mStatus != 0 && m->mUnitHandle == 0) {
                            if (cpu->mPlayer == m->mPlayer) {
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
                    BattlePlayerList *l;
                    j = 0;
                    l = data_020d433c.mBattlePlayers;
                    do {
                        BattleMarker *m = &l->Markers()[j];
                        if (m->mStatus != 0 && m->mUnitHandle != 0) {
                            u8 owner = l->Units()[(u8) (m->mUnitHandle - 1)].mPlayer;
                            if (owner != cpu->mPlayer && owner != l->mPlayers[cpu->mPlayer]->Partner()) {
                                if (state == 2) {
                                    if (!l->mPlayers[owner]->IsCpu()) {
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
                RecordIndex index = (RecordIndex) cpu->mRecord;
                BattleRecord *rec = &data_020d433c.mBattlePlayers->mPlayers[cpu->mPlayer]->mRecords[index];
                u32 recKind       = rec->mBakuganId;
                u16 recValue      = rec->mUnk_04;
                j                 = 0;
                do {
                    BattlePlayerList *l = data_020d433c.mBattlePlayers;
                    BattleMarker *m     = &l->Markers()[j];
                    if (m->mStatus != 0) {
                        bool ok  = false;
                        u8 owner = m->mPlayer;
                        u16 val  = l->mPlayers[owner]->Slot54(m->mSlot);
                        if (cpu->mPlayer == owner || owner == l->mPlayers[cpu->mPlayer]->Partner()) {
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
                    u32 best;
                    u8 bestIdx;
                    j       = 0;
                    best    = 0;
                    bestIdx = 0;
                    do {
                        BattlePlayerList *l = data_020d433c.mBattlePlayers;
                        BattleMarker *m     = &l->Markers()[j];
                        if (m->mStatus != 0) {
                            u8 me = cpu->mPlayer;
                            if (me == m->mPlayer || m->mPlayer == l->mPlayers[me]->Partner()) {
                                u32 v = GetCardAttributeValue(m->mSlot, recKind);
                                if (v > best) {
                                    best    = v;
                                    bestIdx = j;
                                }
                            }
                        }
                        j++;
                    } while (j < 12);
                    if (best != 0) {
                        pick = bestIdx;
                    } else {
                        state = 5;
                    }
                }
                break;
            }
            case 5: {
                u32 roll2 = MATH_Rand32(&gRandom, 100);
                BOOL pickOwn = FALSE;
                if (roll2 < 75) {
                    if (nOwn != 0) {
                        pickOwn = TRUE;
                    }
                } else {
                    if (nOther == 0) {
                        pickOwn = TRUE;
                    }
                }
                if (pickOwn) {
                    pick = own[MATH_Rand32(&gRandom, nOwn) & 0xff];
                } else {
                    pick = other[MATH_Rand32(&gRandom, nOther) & 0xff];
                }
                break;
            }
        }
    } while (pick >= 12);
    if (data_020d433c.mHasPartners != 0) {
        BattleMarker *m     = &data_020d433c.mBattlePlayers->Markers()[pick];
        if (m->mUnitHandle != 0) {
            u8 owner = data_020d433c.mBattlePlayers->Units()[(u8) (m->mUnitHandle - 1)].mPlayer;
            if (owner == data_020d433c.mBattlePlayers->mPlayers[cpu->mPlayer]->Partner()) {
                j = 0;
                do {
                    BattleMarker *m2 = &data_020d433c.mBattlePlayers->mMarkers[j];
                    if (m2->mStatus != 0) {
                        if (m2->mUnitHandle != 0) {
                            if (data_020d433c.mBattlePlayers->Units()[(u8) (m2->mUnitHandle - 1)].mPlayer != data_020d433c.mBattlePlayers->mPlayers[cpu->mPlayer]->Partner()) {
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
    cpu->mPos.x = data_020d433c.mBattlePlayers->Markers()[pick].mCellX * 0x23000 - 0xe3800;
    cpu->mPos.y = 0;
    cpu->mPos.z = data_020d433c.mBattlePlayers->Markers()[pick].mCellY * 0x2f000 - 0xd3800;
    cpu->func_ov007_022328dc();
}

u8 BattleCpu::PickRecord() {
    u8 found         = 0;
    u8 i             = 0;
    BattlePlayer *pl = data_020d433c.mBattlePlayers->mPlayers[mPlayer];
    u8 count         = data_020d433c.mUnk_99;
    do {
        if (pl->mRecords[i].mStatus == 0) {
            found = i;
            break;
        }
        i++;
    } while (i < count);
    mRecord = found;
    return found;
}

void BattleCpu::func_ov007_02231e2c(BattleCpuRot *rot, u16 *outA, u16 *outB, u8 *outC) {
    u8 a    = data_020d433c.mBattlePlayers->mPlayers[mPlayer]->mCharacterId;
    u8 idx2 = 0;
    u8 spin;
    u8 idx1 = 6;
    u8 power;
    u8 i    = 0;
    s8 tilt;
    u8 roll = MATH_Rand32(&gRandom, 100);
    u8 sum  = 0;
    do {
        sum += RollWeights()[a][i];
        if (sum > roll) {
            idx1 = i;
            break;
        }
        i++;
    } while (i < 7);
    roll = MATH_Rand32(&gRandom, 100);
    i   = 0;
    sum = 0;
    do {
        sum += data_ov007_022797ac[a][i];
        if (sum > roll) {
            idx2 = i;
            break;
        }
        i++;
    } while (i < 5);
    BattleCpuRot r = {0, 0, 0};
    r.y = (u16) ((float) (MATH_Rand32(&gRandom, 20) + 125) * 65536.0f / 360.0f);
    r.z = (u16) ((float) (MATH_Rand32(&gRandom, 20) + 10) * 65536.0f / 360.0f);
    power = MATH_Rand32(&gRandom, 30) + 50;
    spin  = 30; // never read (idx1 is always 0..6), but the original keeps 30 in spin's register
    tilt  = 0;
    switch (idx1) {
    case 0:
        power = MATH_Rand32(&gRandom, 20) + 60;
        spin  = MATH_Rand32(&gRandom, 30) + 30;
        r.z  = (u16) ((float) (MATH_Rand32(&gRandom, 20) + 20) * 65536.0f / 360.0f);
        break;
    case 1:
        power = MATH_Rand32(&gRandom, 20) + 60;
        spin  = MATH_Rand32(&gRandom, 30);
        r.z  = (u16) ((float) (MATH_Rand32(&gRandom, 20) + 5) * 65536.0f / 360.0f);
        break;
    case 2:
        power = MATH_Rand32(&gRandom, 20) + 60;
        spin  = 0;
        r.z  = (u16) ((float) (MATH_Rand32(&gRandom, 10) + 5) * 65536.0f / 360.0f);
        break;
    case 3:
        power = MATH_Rand32(&gRandom, 30) + 40;
        spin  = MATH_Rand32(&gRandom, 30) + 30;
        r.z  = (u16) ((float) (MATH_Rand32(&gRandom, 20) + 20) * 65536.0f / 360.0f);
        break;
    case 4:
        power = MATH_Rand32(&gRandom, 30) + 40;
        spin  = MATH_Rand32(&gRandom, 30);
        r.z  = (u16) ((float) (MATH_Rand32(&gRandom, 20) + 5) * 65536.0f / 360.0f);
        break;
    case 5:
        power = MATH_Rand32(&gRandom, 30) + 40;
        spin  = 0;
        r.z  = (u16) ((float) (MATH_Rand32(&gRandom, 10) + 5) * 65536.0f / 360.0f);
        break;
    case 6:
        power = MATH_Rand32(&gRandom, 30) + 10;
        spin  = MATH_Rand32(&gRandom, 30);
        r.z  = (u16) ((float) (MATH_Rand32(&gRandom, 20) + 5) * 65536.0f / 360.0f);
        break;
    }
    if (data_020d433c.mFieldId == 3) {
        r.y = (u16) ((float) (MATH_Rand32(&gRandom, 30) + 120) * 65536.0f / 360.0f);
    } else {
        r.y = (u16) ((float) (data_ov007_022796d0[idx2] + MATH_Rand32(&gRandom, 20)) * 65536.0f / 360.0f);
    }
    roll = MATH_Rand32(&gRandom, 100);
    if (roll < data_ov007_0227970c[a]) {
        u8 f1  = data_020d433c.mBattlePlayers->mPlayers[mPlayer]->mUnk_f1;
        u8 sel = data_020d433c.mUnk_96;
        switch (sel) {
        case 0:
        case 1:
        case 2:
            if (MATH_Rand32(&gRandom, 100) >= 100) {
                sel = MATH_Rand32(&gRandom, 3);
            }
            break;
        default:
            sel = MATH_Rand32(&gRandom, 3);
            break;
        }
        power = ShotTable()[data_020d433c.mFieldId][f1][sel].mPower;
        r.y   = 0x6000;
        r.z   = (u16) ((float) ShotTable()[data_020d433c.mFieldId][f1][sel].mPitch * 65536.0f / 360.0f);
        tilt  = ShotTable()[data_020d433c.mFieldId][f1][sel].mTilt;
        spin  = 0;
    }
    if (data_020d433c.mTutorialId == 2) {
        r.y  = 0x6000;
        spin  = 0;
        tilt  = 0;
        r.z  = 0x1777;
        power = 0x3e;
    } else if (data_020d433c.mTutorialId == 7) {
        r.y  = 0x6000;
        spin  = 0;
        tilt  = 0;
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
    a    = data_020d433c.mBattlePlayers->mPlayers[mPlayer]->mCharacterId;
    roll = MATH_Rand32(&gRandom, 100);
    b    = data_020d433c.mBattlePlayers->mPlayers[mPlayer]->mUnk_f7;
    if (b >= 2) {
        b = 1;
    }
    if (data_020d433c.mUnk_7a != 0 && mPlayer == data_020d433c.mBattlePlayers->mPlayers[0]->mPartner) {
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

void BattleCpu::StartJumpObjTargets(int limit) {
    u8 cand[0x40];
    u8 i = 0;
    u32 n = 0;
    u32 k;
    do {
        GimmickNode *node = &data_020d433c.mGimmicks->mNodes[i];
        if (node && node->mGimmick) {
            Gimmick *g = node->mGimmick;
            if (g->mType == GIMMICK_TYPE_JUMP_OBJ && (limit == 0 || g->mPosition.y < limit)) {
                cand[n] = i;
                n = (u8) (n + 1);
            }
        }
        i++;
    } while (i < 0x40);
    if (n == 0) {
        return;
    }
    mJumpObjCount = n;
    k = 0;
    do {
        if (n == k) {
            break;
        }
        mTargetNodeIndices[k] = cand[MATH_Rand32(&gRandom, n)];
        if (k != 0) {
            u8 j = 0;
            do {
                if (mTargetNodeIndices[j] == mTargetNodeIndices[k]) {
                    j = k + 1;
                    break;
                }
                j++;
            } while (j < k);
            if (j == k + 1) {
                k = (u8) (k - 1);
                goto next;
            }
        }
        {
            GimmickNode *node = &data_020d433c.mGimmicks->mNodes[mTargetNodeIndices[k]];
            if (node && node->mGimmick) {
                mTargets[k] = node->mGimmick->mPosition;
            }
        }
    next:
        k = (u8) (k + 1);
    } while (k < 4);
    mUsingTargets    = 1;
    mSavedPos  = mPos;
    mPos       = mTargets[mTargetIndex];
    mUnk_54    = MATH_Rand32(&gRandom, 301) + 0x47e;
    mUnk_58    = 0xe10;
}

void BattleCpu::NextTarget() {
    if (mUsingTargets != 1) {
        return;
    }
    if (mJumpObjCount == 0) {
        return;
    }
    mTargetIndex = (mTargetIndex + 1) % mJumpObjCount;
    mPos    = mTargets[mTargetIndex];
}

void BattleCpu::StopJumpObjTargets() {
    mUsingTargets   = 0;
    mPos      = mSavedPos;
    mUnk_54   = 0;
    mJumpObjCount   = 0;
}

void BattleCpu::RandomizeOffsets() {
    mOffsetFrames = 0;
    u8 i    = 0;
    do {
        mOffsetEntries[i] = MATH_Rand32(&gRandom, 10);
        if (mOffsetEntries[i] == 0) {
            mOffsetFrameCounts[i] = MATH_Rand32(&gRandom, 60) + 90;
        } else {
            mOffsetFrameCounts[i] = MATH_Rand32(&gRandom, 60) + 60;
        }
        i++;
    } while (i < 8);
    SetOffset();
}

void BattleCpu::SetOffset() {
    if (mOffsetEntries[mOffsetEntryIndex] >= 9) {
        return;
    }
    s32 offsets[9][2] = {
        {0, 0}, {0, 0x59a}, {0x333, 0x2cd}, {0x666, 0}, {0x333, -0x2cd}, {0, -0x59a}, {-0x333, -0x2cd}, {-0x666, 0}, {-0x333, 0x2cd},
    };
    mUnk_4c = offsets[mOffsetEntries[mOffsetEntryIndex]][0];
    mUnk_50 = offsets[mOffsetEntries[mOffsetEntryIndex]][1];
}

bool BattleCpu::func_ov007_02232e04(u32 a, u32 b, u32 c) {
    bool result = false;
    if (GetCardInfo(a, 3) == 1) {
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

void BattleCpu::GetOffset(s32 *x, s32 *y) {
    if (data_020d433c.mBattlePlayers->mPlayers[mPlayer]->mUnk_f7 != 0 && mOffsetFrames % mOffsetInterval == 0) {
        *x = mUnk_4c;
        *y = mUnk_50;
        return;
    }
    *x = 0;
    *y = 0;
}

extern "C" void MTX_ApplyRotX33(MtxFx33 *m, u16 angle);

extern "C" void MTX_ApplyRotY33(MtxFx33 *m, u16 angle);

extern "C" void MTX_ApplyRotZ33(MtxFx33 *m, u16 angle);

BOOL SimulateStep(VecFx32 *pos, VecFx32 *vel);

BOOL SimulateStep(VecFx32 *pos, VecFx32 *vel) {
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

void PredictPosition(VecFx32 *pos, VecFx32 *vel, int steps, VecFx32 *out) {
    VecFx32 v = *vel;
    v.x       = FX_Mul(v.x, 0xb800);
    v.y       = FX_Mul(v.y, 0xb800);
    v.z       = FX_Mul(v.z, 0xb800);
    VecFx32 p = *pos;
    if (steps != 0) {
        do {
            if (!SimulateStep(&p, &v)) {
                break;
            }
            steps--;
        } while (steps != 0);
    }
    *out = p;
}

int PredictLanding(VecFx32 *pos, VecFx32 *vel, VecFx32 *out) {
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
        SimulateStep(&p, &v);
    } while (n++ == 0 || p.y >= 0x899a);
    *out = p;
    return n;
}

int PredictStop(VecFx32 *pos, VecFx32 *vel, VecFx32 *out, u32 unk4, u32 unk5) {
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

void RotateDirScaled(u16 *rot, u32 s, VecFx32 *dir, VecFx32 *out) {
    VEC_Normalize(dir, out);
    MtxFx33 m = data_0220e848;
    if (rot[1]) {
        MTX_ApplyRotY33(&m, rot[1]);
    }
    if (rot[0]) {
        MTX_ApplyRotX33(&m, rot[0]);
    }
    if (rot[2]) {
        MTX_ApplyRotZ33(&m, rot[2]);
    }
    MTX_MultVec33(out, &m, out);
    out->x = FX_Mul(out->x, FX32_CONST((float) s / 150.0f));
    out->y = FX_Mul(out->y, FX32_CONST((float) s / 150.0f));
    out->z = FX_Mul(out->z, FX32_CONST((float) s / 150.0f));
}

void GetVelocityTowardXZ(VecFx32 *a, VecFx32 *b, VecFx32 *speed, VecFx32 *out) {
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

void func_ov007_022337ec(VecFx32 *A, VecFx32 *B, VecFx32 *C, VecFx32 *D, VecFx32 *E, fx32 F, fx32 G, int flag) {
    if (flag) {
        fx32 len = VEC_Mag(B);
        B->x = FX_Mul(E->x, len);
        B->x = FX_Mul(B->x, F);
        B->y = FX_Mul(E->y, len);
        B->y = FX_Mul(B->y, G);
        B->z = FX_Mul(E->z, len);
        B->z = FX_Mul(B->z, F);
        C->x = FX_Mul(E->x, Abs(C->x));
        C->x = FX_Mul(C->x, F);
        C->y = FX_Mul(E->y, Abs(C->y));
        C->y = FX_Mul(C->y, G);
        C->z = FX_Mul(E->z, Abs(C->z));
        C->z = FX_Mul(C->z, F);
        len  = VEC_Mag(D);
        D->x = FX_Mul(E->x, len);
        D->x = FX_Mul(D->x, F);
        D->z = FX_Mul(E->z, len);
        D->z = FX_Mul(D->z, F);
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
            ratio.x  = FX_Div((s32) fb, (s32) fa);
        }
        if (a.y != 0) {
            float fa = a.y > 0 ? 0.5f + (float) (a.y << 12) : (float) (a.y << 12) - 0.5f;
            float fb = A->y > 0 ? 0.5f + (float) (A->y << 12) : (float) (A->y << 12) - 0.5f;
            ratio.y  = FX_Div((s32) fb, (s32) fa);
        }
        if (a.z != 0) {
            float fa = a.z > 0 ? 0.5f + (float) (a.z << 12) : (float) (a.z << 12) - 0.5f;
            float fb = A->z > 0 ? 0.5f + (float) (A->z << 12) : (float) (A->z << 12) - 0.5f;
            ratio.z  = FX_Div((s32) fb, (s32) fa);
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

void RotateVecY(VecFx32 *in, u16 angle, VecFx32 *out) {
    MtxFx33 m = data_0220e848;
    MTX_ApplyRotY33(&m, angle);
    MTX_MultVec33(in, &m, out);
}


static inline fx32 Abs(fx32 v) {
    return v < 0 ? -v : v;
}

void NormalizeDiff(const VecFx32 *a, const VecFx32 *b, int unused, VecFx32 *out) {
    VEC_Subtract(a, b, out);
    VEC_Normalize(out, out);
}
