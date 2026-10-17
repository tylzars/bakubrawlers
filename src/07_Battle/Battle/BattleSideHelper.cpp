
#include "Battle/BattleBase.hpp"

#include "Game/Random.hpp"
#include "Unknown/UnkFunctions.hpp"
#include <nitro/mi.h>

u32 BattleRand32(u32 max);

// Per-character table of four 8-byte records (0x20 bytes per character).
struct BattleSideRow {
    u8 mUnk_00[0x20];
};
extern BattleSideRow data_ov007_02278e40[];
extern BattleSideRow data_ov007_02279060;

BattleSideHelper::BattleSideHelper(u8 index, u8 unk2, BattleEntry *entries) {
    mRollEnabled = 0;
    mPeriodFrames = 0;
    mQuarter = 0;
    mRollFrames = 0;
    mQuarterPeriodCount = 0;
    u16 *id = (u16 *) ((u8 *) &entries->mCardId + index * 0x14);
    if (*id != 0) {
        BattleSideRow *row;
        if (unk2 == 0x15) {
            row = &data_ov007_02279060;
        } else {
            row = &data_ov007_02278e40[unk2];
        }
        mRollEnabled = 1;
        switch (GetCardInfo(*id, 3)) {
            case 0:
                MIi_CpuCopyFast(row->mUnk_00, &mPeriodFrameCount, 8);
                break;
            case 1:
                MIi_CpuCopyFast(row->mUnk_00 + 8, &mPeriodFrameCount, 8);
                break;
            case 2:
                MIi_CpuCopyFast(row->mUnk_00 + 0x10, &mPeriodFrameCount, 8);
                break;
            case 3:
                MIi_CpuCopyFast(row->mUnk_00 + 0x18, &mPeriodFrameCount, 8);
                break;
            default:
                mRollEnabled = 0;
                break;
        }
    }
}

BattleSideHelper::~BattleSideHelper() {}

BOOL BattleSideHelper::RollForCard(int side, u8 n) {
    if (mRollEnabled == 0) {
        return 0;
    }
    u8 q;
    BOOL ret = 0;
    if (side == 1) {
        n = 100 - n;
    }
    q = n / 25;
    if (n == 1 || n == 99) {
        return 0;
    }
    if (mQuarter == q) {
        mPeriodFrames++;
        if (mPeriodFrames >= mPeriodFrameCount) {
            mPeriodFrames = 0;
            mQuarterPeriodCount++;
        }
    } else {
        mPeriodFrames = 0;
        mQuarterPeriodCount = 0;
    }
    mQuarter = q;
    mRollFrames++;
    if (mRollFrames >= mRollFrameCount) {
        mRollFrames = 0;
        u8 *p = mQuarterChances;
        if (p[q] != 0) {
            u8 roll = BattleRand32(100);
            u16 lim = mQuarterPeriodCount * mPeriodChance + p[q];
            if (lim > 100) {
                lim = 100;
            }
            if (roll <= lim) {
                mRollEnabled = 0;
                ret = 1;
            }
        }
    }
    return ret;
}
