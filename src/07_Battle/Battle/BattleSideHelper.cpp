
#include "Battle/BattleBase.hpp"

#include "Game/Random.hpp"
#include "Unknown/UnkFunctions.hpp"
#include <nitro/mi.h>

u32 func_ov007_0221d348(u32 max);

// Per-character table of four 8-byte records (0x20 bytes per character).
struct BattleSideRow {
    u8 mUnk_00[0x20];
};
extern BattleSideRow data_ov007_02278e40[];
extern BattleSideRow data_ov007_02279060;

BattleSideHelper::BattleSideHelper(u8 index, u8 unk2, BattleEntry *entries) {
    mUnk_04 = 0;
    mUnk_05 = 0;
    mUnk_06 = 0;
    mUnk_07 = 0;
    mUnk_08 = 0;
    u16 *id = (u16 *) ((u8 *) &entries->mUnk_12 + index * 0x14);
    if (*id != 0) {
        BattleSideRow *row;
        if (unk2 == 0x15) {
            row = &data_ov007_02279060;
        } else {
            row = &data_ov007_02278e40[unk2];
        }
        mUnk_04 = 1;
        switch (func_02065c0c(*id, 3)) {
            case 0:
                MIi_CpuCopyFast(row->mUnk_00, &mUnk_0c, 8);
                break;
            case 1:
                MIi_CpuCopyFast(row->mUnk_00 + 8, &mUnk_0c, 8);
                break;
            case 2:
                MIi_CpuCopyFast(row->mUnk_00 + 0x10, &mUnk_0c, 8);
                break;
            case 3:
                MIi_CpuCopyFast(row->mUnk_00 + 0x18, &mUnk_0c, 8);
                break;
            default:
                mUnk_04 = 0;
                break;
        }
    }
}

// non-matching: 4 bytes short: the original materialises a fresh zero before each zero store.
BOOL BattleSideHelper::func_ov007_02223638(int side, u8 n) {
    if (mUnk_04 == 0) {
        return 0;
    }
    if (side == 1) {
        n = 100 - n;
    }
    u8 q = n / 25;
    if (n == 1 || n == 99) {
        return 0;
    }
    if (mUnk_06 == q) {
        mUnk_05++;
        if (mUnk_05 >= mUnk_0c) {
            mUnk_05 = 0;
            mUnk_08++;
        }
    } else {
        mUnk_05 = 0;
        mUnk_08 = 0;
    }
    mUnk_06 = q;
    mUnk_07++;
    BOOL ret = 0;
    if (mUnk_07 >= mUnk_0e) {
        mUnk_07 = 0;
        u8 *p = mUnk_10;
        if (p[q] != 0) {
            u8 roll = func_ov007_0221d348(100);
            u16 lim = mUnk_08 * mUnk_0d + p[q];
            if (lim > 100) {
                lim = 100;
            }
            if (roll <= lim) {
                mUnk_04 = 0;
                ret = 1;
            }
        }
    }
    return ret;
}
