#include "Battle/BattleBase.hpp"
#include "Game/Random.hpp"
#include "Game/UnkStruct_020d433c.hpp"

extern u8 gMode1Rows[];
extern u8 gMode2Rows[];
extern u8 gMode5Rows[];
extern u8 gMode0Rows[];
extern u8 gMode3Rows[];
extern u8 gMode4Rows[];

// One 0x28-byte record per battle mode (BattleSlot::mUnk_24). Defined between the includes on purpose
// (docs/decompiling.md, "Data layout within a section"): here, before the SoundManager and WirelessManager headers, it
// gives the original's .bss order of their destructor nodes (WirelessManager's first).
u8 *gModeRowTables[6] = {
    gMode0Rows, gMode1Rows, gMode2Rows,
    gMode3Rows, gMode4Rows, gMode5Rows,
};

#include "System/SoundManager.hpp"
#include "System/WirelessManager.hpp"
#include <nitro/math.h>

u8 GetCardInfo(u32 id, u32 kind);

int func_ov007_02222764(int side, BattleEntry *entry) {
    int result = -1;
    u16 *ids   = &entry->mCardId;
    int off    = side * sizeof(BattleEntry);
    if (GetCardInfo(*(u16 *) ((u8 *) ids + off), 3) == 4) {
        switch (*(u16 *) ((u8 *) ids + off)) {
            case 0x88:
                result = 2;
                break;
            case 0x89:
                result = 0;
                break;
            case 0x8a:
                result = 1;
                break;
            case 0x8b:
                result = 3;
                break;
            case 0x8c:
            case 0x8d:
                result = 5;
                break;
        }
    }
    return result;
}

u32 BattleRand32(u32 max);

extern u8 gIntervalRows[];

extern u8 data_ov007_02278d34[];

extern u8 gCharacterModeBases[];

extern u8 data_ov007_02278d7a[];

extern u8 data_ov007_02278d7b[];

extern const u8 gCharacterIntervalRowBases[];

extern const u8 gCharacterCardDelayFrameCounts[];

extern const u8 gCharacterCardDelayRandomRanges[];

BattleSlot::BattleSlot(BattleData *data, u8 index, u8 mode, u8 unk4, u8 unk5, u8 unk6, BattleParam *param, u8 unk8) {
    mMode = mode;
    mCharacterId = unk4;
    mStartTimer = 0;
    mUnk_07 = unk8;
    mIntervalFrames = 0;
    mIntervalFrameCount = 0;
    mValue = 0;
    mValueFrames = 0;
    mRollFrames = 0;
    mRollFrameCount = 0;
    mUnk_14_0 = 0;
    mUnk_14_1 = 0;
    mUnk_14_2 = 0;
    mUnk_15 = 0;
    mUnk_16 = 0;
    mIndex = index;
    mCardDelayTimer = 0;
    mSoundTimer = 0;
    mParam = param;
    mHelper = 0;
    mData = data;
    BattleEntry *entries = mData->mEntries;
    mModeRow = gModeRowTables[mode];
    int row = unk4 * 12;
    BattlePlayer *player = data_020d433c.mBattlePlayers->mPlayers[index];
    u8 base = player->mUnk_f7;
    u8 *rowp = gCharacterModeBases + row;
    u32 a = rowp[mode];
    a = (u8) (a + base);
    u32 b = (u8) (gCharacterIntervalRowBases[row] + base);
    if (data_020d433c.mUnk_7a) {
        if (data_020d433c.mHasPartners) {
            if (player->mPartner == 0 && player->mCharacterId != 13 && player->mCharacterId != 14) {
                a = (u8) (a + 1);
                b = (u8) (b + 1);
            }
        }
    } else if (base == 1) {
        a = (u8) (a + 1);
    } else if (base == 2) {
        a = (u8) (a + 2);
    }
    u8 *p = mModeRow;
    if (a >= 10) {
        a = 9;
    }
    if (b >= 8) {
        b = 7;
    }
    mModeRow = p + a * 4;
    mIntervalRow = gIntervalRows + b * 2;
    if (mMode == BATTLE_MODE_CONFRONT_BG01 || mMode == BATTLE_MODE_CONFRONT_BG04) {
        u8 c;
        if (mMode == BATTLE_MODE_CONFRONT_BG01) {
            c = data_ov007_02278d7a[row];
        } else {
            c = data_ov007_02278d7b[row];
        }
        c += base;
        if (c >= 8) {
            c = 7;
        }
        mUnk_2c = data_ov007_02278d34 + c * 8;
        mUnk_14_0 = 1;
    }
    BattlePlayerList *list = data_020d433c.mBattlePlayers;
    u8 la = list->mPlayers[unk5]->mUnk_ee;
    u8 lb = list->mPlayers[unk6]->mUnk_ee;
    if (index == unk5) {
        mRole = 2;
    } else if (index == unk6) {
        mRole = 3;
    } else if (data_020d433c.mHasPartners) {
        if (unk5 == list->mPlayers[index]->mPartner) {
            mRole = 0;
        } else {
            mRole = 1;
        }
    } else if (la > lb) {
        mRole = 1;
    } else if (lb > la) {
        mRole = 0;
    } else {
        mRole = 4;
    }
    if (mRole != 2 && mRole != 3) {
        mIntervalFrameCount = mIntervalRow[0];
        mIntervalFrameCount += BattleRand32(mIntervalRow[1]);
        return;
    }
    switch (mMode) {
        case BATTLE_MODE_CONFRONT_BG02:
            if (mMode == BATTLE_MODE_CONFRONT_BG02) {
                mStartTimer = 50;
            }
        case BATTLE_MODE_CONFRONT_03:
            mIntervalFrameCount = mModeRow[0];
            mIntervalFrameCount += BattleRand32(mModeRow[1]);
            break;
        case BATTLE_MODE_CONFRONT_BG06:
            mRollFrameCount = mModeRow[1];
            mRollFrameCount += BattleRand32(mModeRow[2]);
            break;
    }
    u8 side = mRole - 2;
    mHelper = new BattleSideHelper(side, unk4, entries);
}

BattleSlot::~BattleSlot() {
    if (mHelper) {
        delete mHelper;
    }
}

BOOL BattleSlot::UpdateInterval() {
    BOOL ret = 0;
    switch (mMode) {
        case BATTLE_MODE_CONFRONT_BG02:
        case BATTLE_MODE_CONFRONT_03:
            mIntervalFrames++;
            if (mIntervalFrames < mIntervalFrameCount) {
                return ret;
            }
            mIntervalFrames = 0;
            mIntervalFrameCount = mModeRow[0];
            mIntervalFrameCount += BattleRand32(mModeRow[1]);
            ret = 1;
            break;
        case BATTLE_MODE_CONFRONT_BG01:
        case BATTLE_MODE_CONFRONT_BG04:
        case BATTLE_MODE_CONFRONT_BG05:
        case BATTLE_MODE_CONFRONT_BG06:
            ret = 1;
            break;
    }
    return ret;
}

int BattleSlot::ComputeValue(u8 n, u16 *a, u16 *b) {
    u32 ret = 0;
    if (mRole == 2 || mRole == 3) {
        mValueFrames++;
        if (mValueFrames >= mParam->mUnk_08) {
            mValueFrames = 0;
            switch (mMode) {
                case BATTLE_MODE_CONFRONT_BG01:
                case BATTLE_MODE_CONFRONT_BG04: {
                    u8 *p = mModeRow;
                    if (p[2] >= BattleRand32(100)) {
                        mValue = mValue - p[3];
                        if (mValue < 0) {
                            mValue = 0;
                        }
                    } else {
                        mValue = mValue + p[1];
                        if (mValue >= mModeRow[0]) {
                            mValue = mModeRow[0];
                        }
                    }
                    break;
                }
                case BATTLE_MODE_CONFRONT_BG05: {
                    if (mSoundTimer != 0) {
                        mSoundTimer--;
                    }
                    u8 *p = mModeRow;
                    if (p[2] >= BattleRand32(100)) {
                        mValue = mValue - p[3];
                        if (mValue < 0) {
                            mValue = 0;
                        }
                    } else {
                        mValue = mValue + p[1];
                        if (mValue >= mModeRow[0]) {
                            mValue = mModeRow[0];
                        }
                        if (mSoundTimer == 0) {
                            u8 i = 0;
                            s16 sum = 0;
                            if (mRole == 3) {
                                do {
                                    sum += a[i];
                                    i++;
                                } while (i < data_020d433c.mPlayerCount);
                            } else {
                                do {
                                    sum += b[i];
                                    i++;
                                } while (i < data_020d433c.mPlayerCount);
                            }
                            if (mValue - sum > 80) {
                                SoundManager::Get().StartSeqArc(0x6c);
                                func_020247fc(&SoundManager::Get(), 0x312);
                                mSoundTimer = 60;
                            }
                        }
                    }
                    break;
                }
                case BATTLE_MODE_CONFRONT_BG02:
                case BATTLE_MODE_CONFRONT_03:
                    mValue = mModeRow[2];
                    mValue += BattleRand32(mModeRow[3]);
                    if (mValue >= 50) {
                        SoundManager::Get().StartSeqArc(0x6c);
                        func_020247fc(&SoundManager::Get(), 0x312);
                        mSoundTimer = 60;
                    }
                    break;
                case BATTLE_MODE_CONFRONT_BG06:
                    mValue = 0;
                    mRollFrames++;
                    if (mRollFrames >= mRollFrameCount) {
                        mRollFrames = 0;
                        mRollFrameCount = mModeRow[1];
                        mRollFrameCount += BattleRand32(mModeRow[2]);
                        if (mModeRow[0] >= BattleRand32(100)) {
                            mValue = 100;
                            SoundManager::Get().StartSeqArc(0x6c);
                            func_020247fc(&SoundManager::Get(), 0x312);
                        } else {
                            mValue = 10;
                        }
                    } else if (mModeRow[3] >= BattleRand32(100)) {
                        mValue = 10;
                    }
                    break;
            }
            ret = mValue;
        }
    } else {
        mIntervalFrames++;
        if (mIntervalFrames >= mIntervalFrameCount) {
            mIntervalFrames = 0;
            mIntervalFrameCount = mIntervalRow[0];
            mIntervalFrameCount += BattleRand32(mIntervalRow[1]);
            ret = mUnk_07;
        }
    }
    if (ret > 100) {
        ret = 100;
    }
    return ret;
}

u32 BattleSlot::func_ov007_022230b4(u16 value, u8 n) {
    if (mUnk_14_0) {
        if (!mUnk_14_1) {
            if (mRole == 3) {
                n = 100 - n;
            }
            if (mUnk_14_2) {
                n = 0;
            }
            if (n < 50) {
                mUnk_15++;
                if (mUnk_15 >= mUnk_2c[0]) {
                    mUnk_15 = 0;
                    if (mUnk_2c[1] >= BattleRand32(100)) {
                        mUnk_14_1 = 1;
                        mUnk_16 = mUnk_2c[2];
                        if (!mUnk_14_2) {
                            SoundManager::Get().StartSeqArc(0x6c);
                            func_020247fc(&SoundManager::Get(), 0x312);
                        }
                    }
                }
            } else {
                mUnk_15 = 0;
            }
        } else {
            value = value + mUnk_2c[3];
            value = BattleRand32(mUnk_2c[4]) + value;
            if (mUnk_16 != 0) {
                mUnk_16--;
            } else {
                mUnk_14_1 = 0;
                mValue = mModeRow[0];
            }
        }
    }
    return value;
}

int BattleSlot::Update(u16 *a, u16 *b, u16 *c, s8 (*choice)[2], u8 n, u8 *d) {
    int role = mRole;
    if (role == 4) {
        return -1;
    }
    BattleData *data = mData;
    int ret = -1;
    if (role == 2 || role == 3) {
        if (mCardDelayTimer) {
            mCardDelayTimer--;
            if (mCardDelayTimer == 0) {
                u8 other = (u8) (mRole - 2) ^ 1;
                WirelessManager::Get();
                if (!IsWirelessMode(&data_020c4030.mUnk_228)) {
                    func_ov007_0223e214(mData, other, 0);
                } else {
                    *d = other + 1;
                }
            }
        } else if (mStartTimer) {
            mStartTimer--;
        } else {
            u8 side = role - 2;
            if (mHelper->RollForCard(side, n)) {
                c[side] = 1;
                ret = 1;
            }
            if (c[side] == 0) {
                if (UpdateInterval()) {
                    u16 v = ComputeValue(n, a, b);
                    u32 w = func_ov007_022230b4(v, n);
                    if (mIndex == data->mEntries[0].mPlayer) {
                        a[mIndex] = w;
                    } else {
                        b[mIndex] = w;
                    }
                }
            }
        }
    } else {
        u16 v = ComputeValue(n, a, b);
        if (mRole == 0) {
            a[mIndex] = v;
        } else {
            b[mIndex] = v;
        }
    }
    if (mRole <= 1) {
        if (choice[mRole][0] == -1) {
            choice[mRole][0] = mIndex;
        } else {
            choice[mRole][1] = mIndex;
        }
    }
    return ret;
}

void BattleSlot::StartCardDelay(int sel) {
    if ((sel == 0 && mRole == 3) || (sel == 1 && mRole == 2)) {
        mCardDelayTimer = gCharacterCardDelayFrameCounts[mCharacterId * 12];
        mCardDelayTimer += BattleRand32(gCharacterCardDelayRandomRanges[mCharacterId * 12]);
    }
}

u32 func_ov007_022234f0(int player, u8 mode, u8 ch) {
    u8 *rowp = gCharacterModeBases + ch * 12;
    u32 v = (u8) (data_020d433c.mBattlePlayers->mPlayers[player]->mUnk_f7 + rowp[mode]);
    if (v >= 8) {
        v = 7;
    }
    return v;
}
