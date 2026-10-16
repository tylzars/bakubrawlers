#include "Game/Ranking.hpp"

#include "Game/UnkStruct_020d43f0.hpp"

#include <nitro/fx.h>

extern const u8 data_020a3070[][13];
extern const u8 data_020a305c[4];
extern const u32 data_020a3060[4];
extern const u16 data_020b94fc[2][12];
extern const u16 data_020b952c[2][12];
extern const u16 data_020b955c[2][12];
extern const u32 data_020b958c[2][12];
extern const u32 data_020b95ec[2][12];

Ranking::Ranking() {
    mPlayerCharacterId = 0x15;
    mPlayerRank = 0x3e7;
    mPlayerScore = data_020d43f0[0].mUnk_20;
    mTableIndex = 0;
    u32 flag = 0;
    u8 mode = data_020d43f0[0].mUnk_42;
    if (mode == 6) {
        if (data_020d43f0[0].mUnk_43 > 2) {
            flag = 1;
        }
    }
    if (((mode > 6) | flag | data_020d43f0[0].mUnk_5bf_0) != 0) {
        mTableIndex = 1;
    }
    Build();
}

Ranking::~Ranking() {
}

// Searches mEntries then mUnk_6c by character id. When neither matches it returns &mUnk_6c[4], the player group at
// 0x8c, never NULL.
UnkStruct_020758f4_Record *Ranking::FindEntry(u32 characterId) {
    u8 i = 0;
    do {
        if (characterId == mEntries[i].mCharacterId) {
            break;
        }
        i++;
    } while (i < 13);
    if (i >= 13) {
        i = 0;
        do {
            if (characterId == mUnk_6c[i].mCharacterId) {
                break;
            }
            i++;
        } while (i < 4);
        return (UnkStruct_020758f4_Record *) &mUnk_6c[i];
    }
    return (UnkStruct_020758f4_Record *) &mEntries[i];
}

RankingEntry *Ranking::GetEntry(u32 index) {
    return &mEntries[index];
}

// Does not reset mPlayerRank to 0x3e7, so a second call (ParkMenu.cpp:69, after the constructor's own call) starts from
// the first call's value.
void Ranking::Build() {
    s8 i;
    s8 k;
    s8 j;
    for (i = 0; i < 13; i++) {
        u8 id = data_020a3070[mTableIndex][i];
        mEntries[i].mCharacterId = id;
        if (id == 0x15) {
            mEntries[i].mScore = 0;
        } else {
            mEntries[i].mScore = ComputeScore((u8) i);
        }
    }
    for (i = 1; i < 13; i++) {
        j = i;
        k = i - 1;
        do {
            u32 scoreA = mEntries[k].mScore;
            u32 scoreB = mEntries[j].mScore;
            if (scoreB > scoreA) {
                mEntries[k].mScore = scoreB;
                mEntries[j].mScore = scoreA;
                u8 idA = mEntries[k].mCharacterId;
                u8 idB = mEntries[j].mCharacterId;
                mEntries[k].mCharacterId = idB;
                mEntries[j].mCharacterId = idA;
                j = k;
            }
            k--;
        } while (k >= 0);
    }
    u8 rank = 1;
    mEntries[0].mRank = 1;
    for (i = 1; i < 13; i++) {
        if (mEntries[i].mScore < mEntries[i - 1].mScore) {
            rank = i + 1;
        }
        mEntries[i].mRank = rank;
    }
    for (i = 0; (u32) i < 10; i++) {
        if (mPlayerScore >= mEntries[i].mScore) {
            s8 j = 12;
            do {
                mEntries[j].mRank = mEntries[j - 1].mRank;
                if (mPlayerScore > mEntries[j - 1].mScore) {
                    mEntries[j].mRank++;
                }
                mEntries[j].mCharacterId = mEntries[j - 1].mCharacterId;
                mEntries[j].mScore = mEntries[j - 1].mScore;
                j--;
            } while (i < j);
            mEntries[i].mScore = mPlayerScore;
            mEntries[i].mCharacterId = mPlayerCharacterId;
            mPlayerRank = mEntries[i].mRank;
            break;
        }
    }
    if (mPlayerRank > 10) {
        if (mPlayerScore > 0x30bb0) {
            mPlayerRank = 11;
        } else {
            mPlayerRank = GetRankForScore(mPlayerScore);
        }
    }
    u32 base;
    u32 score = mPlayerScore;
    if (score <= 100000) {
        base = score * 10 / 100;
    } else {
        base = 10000;
        if (score > 1000000) {
            base += 9000;
        } else {
            base += (score - 100000) / 100;
        }
    }
    for (i = 0; i < 4; i++) {
        mUnk_6c[i].mCharacterId = data_020a305c[i];
        mUnk_6c[i].mScore = base + data_020a3060[i];
        mUnk_6c[i].mScore /= 10;
        mUnk_6c[i].mScore *= 10;
        mUnk_6c[i].mRank = GetRankForScore(mUnk_6c[i].mScore);
    }
    for (i = 0; i < 4; i++) {
        for (j = 0; j < 4; j++) {
            if (i != j && mUnk_6c[i].mRank == mUnk_6c[j].mRank) {
                if (mUnk_6c[i].mScore > mUnk_6c[j].mScore) {
                    mUnk_6c[i].mRank--;
                } else {
                    mUnk_6c[j].mRank--;
                }
            }
        }
    }
    if (mPlayerRank > 10) {
        for (k = 12; k > 10; k--) {
            if (mEntries[k - 1].mScore != 0) {
                if (mEntries[k - 1].mScore <= mPlayerScore) {
                    mPlayerRank = mEntries[k - 1].mRank;
                    mEntries[k].mCharacterId = mEntries[k - 1].mCharacterId;
                    mEntries[k].mUnk_1 = mEntries[k - 1].mUnk_1;
                    mEntries[k].mRank = mEntries[k - 1].mRank;
                    mEntries[k].mScore = mEntries[k - 1].mScore;
                    if (mEntries[k].mScore < mPlayerScore) {
                        mEntries[k].mRank++;
                    }
                    mEntries[k - 1].mCharacterId = mPlayerCharacterId;
                    mEntries[k - 1].mUnk_1 = mUnk_8d;
                    mEntries[k - 1].mRank = mPlayerRank;
                    mEntries[k - 1].mScore = mPlayerScore;
                } else {
                    u16 prevRank = mEntries[k - 1].mRank;
                    if (prevRank >= mPlayerRank) {
                        mPlayerRank = prevRank + 1;
                    }
                    break;
                }
            }
        }
    }
    for (i = 0; i < 4; i++) {
        if (mPlayerRank == mUnk_6c[i].mRank) {
            if (mUnk_6c[i].mScore > mPlayerScore) {
                mUnk_6c[i].mRank--;
            } else {
                mUnk_6c[i].mRank++;
            }
        }
    }
}

u32 Ranking::ComputeScore(u8 index) {
    u32 base = mPlayerScore / 2000 * 2000;
    u8 mode = mTableIndex;
    s32 x = (base + data_020b955c[mode][index]) * data_020b952c[mode][index];
    double angle = (double) x - (double) (u32) ((double) x / 6.283185308) * 6.283185308;
    s32 step = (u32) (angle * 10430.378349108529);
    s32 sinv = FX_SinCosTable_[(step >> 4) * 2];
    u32 adj;
    if (sinv >= 0) {
        adj = (sinv * (data_020b95ec[mode][index] / 100) >> 12) * 100;
    } else {
        adj = -((-sinv * (data_020b95ec[mode][index] / 100)) >> 12) * 100;
    }
    u32 sub = base * data_020b94fc[mode][index];
    return (base + data_020b958c[mode][index] + adj - sub / 100) / 10 * 10;
}

// (0x30bb0 - score) is unsigned, so a score above 0x30bb0 wraps; Build guards the mPlayerScore call (:120) but not the
// mUnk_6c call (:143).
u16 Ranking::GetRankForScore(u32 score) {
    u32 t = (0x30bb0 - score) / 1000;
    u32 cube = t * t * t;
    u16 rank = cube / 7968 + 11;
    if (rank <= 10) {
        rank = 11;
    }
    if (rank > 999) {
        rank = 999;
    }
    return rank;
}
