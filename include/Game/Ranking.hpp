#pragma once

#include "global.h"
#include "types.h"

// Second view of a RankingEntry (8 bytes, the type FindEntry returns); despite their names, mTextOffset and
// mTextLength hold the rank and the score (TournamentSubScene.cpp:232-233).
struct UnkStruct_020758f4_Record {
    /* 0 */ PAD(0x0, 0x2);
    /* 2 */ u16 mTextOffset;
    /* 4 */ u32 mTextLength;
};

// 8-byte record returned by GetEntry.
struct RankingEntry {
    /* 0 */ u8 mCharacterId;
    /* 1 */ u8 mUnk_1;
    /* 2 */ u16 mRank;
    /* 4 */ u32 mScore;
    /* 8 */
};

// 0x98 bytes.
class Ranking {
public:
    /* 00 (vtable) */
    /* 04 */ RankingEntry mEntries[13];
    /* 6c */ RankingEntry mUnk_6c[4];
    /* 8c */ u8 mPlayerCharacterId;
    /* 8d */ u8 mUnk_8d;
    /* 8e */ u16 mPlayerRank;
    /* 90 */ u32 mPlayerScore;
    /* 94 */ u8 mTableIndex;
    /* 95 */ PAD(0x95, 0x98);
    /* 98 */

    Ranking();
    virtual ~Ranking();
    UnkStruct_020758f4_Record *FindEntry(u32 characterId);
    RankingEntry *GetEntry(u32 index);
    void Build();
    u32 ComputeScore(u8 index);
    u16 GetRankForScore(u32 score);
};

