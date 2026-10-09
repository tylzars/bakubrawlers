#include "Battle/BattleBase.hpp"
#include "Game/Random.hpp"
#include "Game/UnkStruct_020d433c.hpp"
#include "System/SoundManager.hpp"
#include "System/WirelessManager.hpp"
#include <nitro/math.h>

int func_02065c0c(u32 id, u32 kind);

int func_ov007_02222764(int side, BattleEntry *entry) {
    int result = -1;
    u16 *ids   = &entry->mUnk_12;
    int off    = side * sizeof(BattleEntry);
    if (func_02065c0c(*(u16 *) ((u8 *) ids + off), 3) == 4) {
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

u32 func_ov007_0221d348(u32 max);

extern u8 *data_ov007_02286570[6];

extern u8 data_ov007_02278c34[];

extern u8 data_ov007_02278d34[];

extern u8 data_ov007_02278d74[];

extern u8 data_ov007_02278d7a[];

extern u8 data_ov007_02278d7b[];

extern const u8 data_ov007_02278d7c[];

extern const u8 data_ov007_02278d7d[];

extern const u8 data_ov007_02278d7e[];

// non-matching: this and the vtable temporary have swapped registers (r5/r4).
BattleSlot::BattleSlot(BattleData *data, u8 index, u8 mode, u8 unk4, u8 unk5, u8 unk6, BattleParam *param, u8 unk8) {
    mMode = mode;
    mCharacter = unk4;
    mUnk_06 = 0;
    mUnk_07 = unk8;
    mUnk_08 = 0;
    mUnk_0a = 0;
    mUnk_0c = 0;
    mUnk_0e = 0;
    mUnk_10 = 0;
    mUnk_12 = 0;
    mUnk_14_0 = 0;
    mUnk_14_1 = 0;
    mUnk_14_2 = 0;
    mUnk_15 = 0;
    mUnk_16 = 0;
    mIndex = index;
    mUnk_1c = 0;
    mUnk_1e = 0;
    mParam = param;
    mHelper = 0;
    mData = data;
    mUnk_24 = data_ov007_02286570[mode];
    int row = unk4 * 12;
    BattlePlayer *player = data_020d433c.mBattlePlayers->mPlayers[index];
    u8 base = player->mUnk_f7;
    u8 *rowp = data_ov007_02278d74 + row;
    u32 a = (u8) (rowp[mode] + base);
    u32 b = (u8) (data_ov007_02278d7c[row] + base);
    if (data_020d433c.mUnk_7a) {
        if (data_020d433c.mUnk_98) {
            if (player->mUnk_f2 == 0 && player->mUnk_f6 != 13 && player->mUnk_f6 != 14) {
                a = (u8) (a + 1);
                b = (u8) (b + 1);
            }
        }
    } else if (base == 1) {
        a = (u8) (a + 1);
    } else if (base == 2) {
        a = (u8) (a + 2);
    }
    u8 *p = mUnk_24;
    if (a >= 10) {
        a = 9;
    }
    if (b >= 8) {
        b = 7;
    }
    mUnk_24 = p + a * 4;
    mUnk_28 = data_ov007_02278c34 + b * 2;
    if (mMode == 0 || mMode == 3) {
        u8 *q = mMode == 0 ? data_ov007_02278d7a : data_ov007_02278d7b;
        u32 c = (u8) (q[row] + base);
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
    } else if (data_020d433c.mUnk_98) {
        if (unk5 == list->mPlayers[index]->mUnk_f2) {
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
        mUnk_0a = mUnk_28[0];
        mUnk_0a += func_ov007_0221d348(mUnk_28[1]);
        return;
    }
    switch (mMode) {
        case 1:
            if (mMode == 1) {
                mUnk_06 = 50;
            }
        case 2:
            mUnk_0a = mUnk_24[0];
            mUnk_0a += func_ov007_0221d348(mUnk_24[1]);
            break;
        case 5:
            mUnk_12 = mUnk_24[1];
            mUnk_12 += func_ov007_0221d348(mUnk_24[2]);
            break;
    }
    u8 side = mRole - 2;
    mHelper = new BattleSideHelper(side, unk4, data->mEntries);
}

BattleSlot::~BattleSlot() {
    if (mHelper) {
        delete mHelper;
    }
}

BOOL BattleSlot::func_ov007_02222bac() {
    BOOL ret = 0;
    switch (mMode) {
        case 1:
        case 2:
            mUnk_08++;
            if (mUnk_08 < mUnk_0a) {
                return ret;
            }
            mUnk_08 = 0;
            mUnk_0a = mUnk_24[0];
            mUnk_0a += func_ov007_0221d348(mUnk_24[1]);
            ret = 1;
            break;
        case 0:
        case 3:
        case 4:
        case 5:
            ret = 1;
            break;
    }
    return ret;
}

int BattleSlot::func_ov007_02222c34(u8 n, u16 *a, u16 *b) {
    u32 ret = 0;
    if (mRole == 2 || mRole == 3) {
        mUnk_0e++;
        if (mUnk_0e >= mParam->mUnk_08) {
            mUnk_0e = 0;
            switch (mMode) {
                case 0:
                case 3: {
                    u8 *p = mUnk_24;
                    if (p[2] >= func_ov007_0221d348(100)) {
                        mUnk_0c = mUnk_0c - p[3];
                        if (mUnk_0c < 0) {
                            mUnk_0c = 0;
                        }
                    } else {
                        mUnk_0c = mUnk_0c + p[1];
                        if (mUnk_0c >= mUnk_24[0]) {
                            mUnk_0c = mUnk_24[0];
                        }
                    }
                    break;
                }
                case 4: {
                    if (mUnk_1e != 0) {
                        mUnk_1e--;
                    }
                    u8 *p = mUnk_24;
                    if (p[2] >= func_ov007_0221d348(100)) {
                        mUnk_0c = mUnk_0c - p[3];
                        if (mUnk_0c < 0) {
                            mUnk_0c = 0;
                        }
                    } else {
                        mUnk_0c = mUnk_0c + p[1];
                        if (mUnk_0c >= mUnk_24[0]) {
                            mUnk_0c = mUnk_24[0];
                        }
                        if (mUnk_1e == 0) {
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
                            if (mUnk_0c - sum > 80) {
                                SoundManager::Get().func_020246f8(0x6c);
                                func_020247fc(&SoundManager::Get(), 0x312);
                                mUnk_1e = 60;
                            }
                        }
                    }
                    break;
                }
                case 1:
                case 2:
                    mUnk_0c = mUnk_24[2];
                    mUnk_0c += func_ov007_0221d348(mUnk_24[3]);
                    if (mUnk_0c >= 50) {
                        SoundManager::Get().func_020246f8(0x6c);
                        func_020247fc(&SoundManager::Get(), 0x312);
                        mUnk_1e = 60;
                    }
                    break;
                case 5:
                    mUnk_0c = 0;
                    mUnk_10++;
                    if (mUnk_10 >= mUnk_12) {
                        mUnk_10 = 0;
                        mUnk_12 = mUnk_24[1];
                        mUnk_12 += func_ov007_0221d348(mUnk_24[2]);
                        if (mUnk_24[0] >= func_ov007_0221d348(100)) {
                            mUnk_0c = 100;
                            SoundManager::Get().func_020246f8(0x6c);
                            func_020247fc(&SoundManager::Get(), 0x312);
                        } else {
                            mUnk_0c = 10;
                        }
                    } else if (mUnk_24[3] >= func_ov007_0221d348(100)) {
                        mUnk_0c = 10;
                    }
                    break;
            }
            ret = mUnk_0c;
        }
    } else {
        mUnk_08++;
        if (mUnk_08 >= mUnk_0a) {
            mUnk_08 = 0;
            mUnk_0a = mUnk_28[0];
            mUnk_0a += func_ov007_0221d348(mUnk_28[1]);
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
                    if (mUnk_2c[1] >= func_ov007_0221d348(100)) {
                        mUnk_14_1 = 1;
                        mUnk_16 = mUnk_2c[2];
                        if (!mUnk_14_2) {
                            SoundManager::Get().func_020246f8(0x6c);
                            func_020247fc(&SoundManager::Get(), 0x312);
                        }
                    }
                }
            } else {
                mUnk_15 = 0;
            }
        } else {
            value = value + mUnk_2c[3];
            value = func_ov007_0221d348(mUnk_2c[4]) + value;
            if (mUnk_16 != 0) {
                mUnk_16--;
            } else {
                mUnk_14_1 = 0;
                mUnk_0c = mUnk_24[0];
            }
        }
    }
    return value;
}

// non-matching: the original spills data and keeps choice in r6; here it is the reverse.
int BattleSlot::func_ov007_02223260(u16 *a, u16 *b, u16 *c, s8 *choice, u8 n, u8 *d) {
    int role = mRole;
    if (role == 4) {
        return -1;
    }
    BattleData *data = mData;
    int ret = -1;
    if (role == 2 || role == 3) {
        if (mUnk_1c) {
            mUnk_1c--;
            if (mUnk_1c == 0) {
                u8 other = (u8) (mRole - 2) ^ 1;
                WirelessManager::Get();
                if (!IsWirelessMode(&data_020c4030.mUnk_228)) {
                    func_ov007_0223e214(mData, other, 0);
                } else {
                    *d = other + 1;
                }
            }
        } else if (mUnk_06) {
            mUnk_06--;
        } else {
            u8 side = role - 2;
            if (mHelper->func_ov007_02223638(side, n)) {
                c[side] = 1;
                ret = 1;
            }
            if (c[side] == 0) {
                if (func_ov007_02222bac()) {
                    u16 v = func_ov007_02222c34(n, a, b);
                    u16 w = func_ov007_022230b4(v, n);
                    if (mIndex == data->mEntries[0].mUnk_0d_4) {
                        a[mIndex] = w;
                    } else {
                        b[mIndex] = w;
                    }
                }
            }
        }
    } else {
        u16 v = func_ov007_02222c34(n, a, b);
        if (mRole == 0) {
            a[mIndex] = v;
        } else {
            b[mIndex] = v;
        }
    }
    if (mRole <= 1) {
        if (choice[mRole * 2] == -1) {
            choice[mRole * 2] = mIndex;
        } else {
            choice[mRole * 2 + 1] = mIndex;
        }
    }
    return ret;
}

void BattleSlot::func_ov007_02223484(int sel) {
    if ((sel == 0 && mRole == 3) || (sel == 1 && mRole == 2)) {
        mUnk_1c = data_ov007_02278d7d[mCharacter * 12];
        mUnk_1c += func_ov007_0221d348(data_ov007_02278d7e[mCharacter * 12]);
    }
}

u32 func_ov007_022234f0(int player, u8 mode, u8 ch) {
    u8 *rowp = data_ov007_02278d74 + ch * 12;
    u32 v = (u8) (data_020d433c.mBattlePlayers->mPlayers[player]->mUnk_f7 + rowp[mode]);
    if (v >= 8) {
        v = 7;
    }
    return v;
}
