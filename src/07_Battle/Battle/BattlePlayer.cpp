#include "global.h"
#include "types.h"
#include "Battle/BattleCpu.hpp"
#include "Game/UnkStruct_020d433c.hpp"
#include "Game/UnkStruct_020d43f0.hpp"
#include "Game/Random.hpp"
#include "Unknown/UnkFunctions.hpp"
#include <nitro/mi.h>
#include "System/InputManager.hpp"
#include "System/FileSystem.hpp"
#include "Text/TextManager.hpp"
#include <nitro/os.h>
#include "System/WirelessManager.hpp"
#include <nitro/math.h>

struct UnkStruct_020c4230 {
    /* 00 */ PAD(0x00, 0x20);
    /* 20 */ u16 mUnk_20;
};
extern UnkStruct_020c4230 data_020c4230;
extern u8 data_020d43eb[];
void func_01ff9510(const void *src, void *dst, u32 size);
void func_01ff94e0(const void *src, void *dst, u32 size);
extern u8 data_020d441a[];
extern u8 data_020d4352[];

void func_ov007_0226a1d8(BattlePlayer *player);

struct BattlePlayerTemplate {
    /* 00 */ u16 mUnk_00;
    /* 02 */ u16 mUnk_02;
    /* 04 */ u16 mUnk_04;
    /* 06 */ PAD(0x06, 0x08);
    /* 08 */ u8 mUnk_08;
    /* 09 */ u8 mUnk_09;
    /* 0a */ PAD(0x0a, 0x0c);
};
struct BattlePlayerTemplateSet {
    /* 00 */ BattlePlayerTemplate mUnk_00[3];
    /* 24 */ u16 mUnk_24[3][2];
    /* 3c */
};
extern BattlePlayerTemplateSet data_ov007_0227b0a8[][3];
extern u8 data_020d43df[];
extern u8 data_020d43db[];
extern u8 data_020d43d7[];
extern u8 data_020d43e3[];
s32 func_020230dc(UnkStruct_020d43f0 *base, u32 unk1, u32 unk2);
int func_020231f4(UnkStruct_020d43f0 *base, u32 unk1, u32 unk2, u32 unk3);

// non-matching: 0x54c vs 0x574 bytes. The original keeps `index` in r11 for the whole constructor and passes it to the BattleCpu constructor without masking it to u8; here it is spilled and masked.
BattlePlayer::BattlePlayer(u32 index)
    : SceneBase(2, 0) {
    mUnk_d4 = 0;
    mUnk_d8 = 0;
    mUnk_dc = 0;
    mUnk_ee = 0;
    mUnk_ef = 0;
    mUnk_f3 = 0;
    mUnk_f4 = 0;
    mRecord = 0;
    mUnk_fa = 0;
    mUnk_fc = 0;
    mUnk_fd = 0;
    mUnk_fe = 0;
    MI_CpuFill8((u8 *) this + 0xc, 0, 0x78);
    MI_CpuFill8((u8 *) this + 0x84, 0, 0x24);
    MI_CpuFill8((u8 *) this + 0xa8, 0, 8);
    mUnk_f0 = index;
    if (data_020d433c.mUnk_98 == 0) {
        mUnk_f2 = mUnk_f0;
    } else {
        mUnk_f2 = data_020d43d7[mUnk_f0];
    }
    mUnk_c8 = 0;
    u32 limit = 1;
    WirelessManager::Get();
    if (IsWirelessMode(&data_020c4258)) {
        limit = WirelessManager::Get().func_0202f134();
    }
    if (limit <= index) {
        mUnk_c8 = new BattleCpu(index);
        mUnk_ef = 2;
    }
    mUnk_f7 = data_020d43df[index];
    mUnk_f6 = data_020d43db[index];
    UnkStruct_020d43f0 *save = data_020d43f0;
    mSlots54[0].mValue = save->mUnk_06[0];
    mSlots54[1].mValue = save->mUnk_06[1];
    mSlots54[2].mValue = save->mUnk_06[2];
    mSlots6c[0].mValue = save->mUnk_0c[0];
    mSlots6c[1].mValue = save->mUnk_0c[1];
    mSlots6c[2].mValue = save->mUnk_0c[2];
    if (mUnk_f6 == 0x15) {
        u8 k = 0;
        do {
            BattleRecord *rec = &mRecords[k];
            rec->mUnk_09      = func_020230a8(save, 0, k);
            rec->mUnk_08      = func_020230dc(save, 0, k);
            rec->mUnk_04      = func_020231f4(save, 0, rec->mUnk_08, rec->mUnk_09);
            rec->mUnk_0b      = rec->mUnk_04 / 10;
            UnkStruct_02023248 *entry = func_02023248(save, rec->mUnk_08, rec->mUnk_09);
            ((u16 *) rec)[0]  = entry->mPad;
            ((u16 *) rec)[1]  = *(u16 *) ((u8 *) entry + 2);
            k++;
        } while (k < 3);
    }
    if (mUnk_f6 <= 0x10) {
        u8 k = 0;
        do {
            u32 extra = 0;
            WirelessManager::Get();
            BOOL bit;
            if (IsWirelessMode(&data_020c4258)) {
                bit = TestFlagBit(data_020d43e3, data_020d433c.mUnk_af[index]);
            } else {
                bit = save->mUnk_5bf_0;
            }
            if (bit) {
                extra = 0x64;
            }
            BattlePlayerTemplate *tmpl = &data_ov007_0227b0a8[mUnk_f6][mUnk_f7].mUnk_00[k];
            BattleRecord *rec          = &mRecords[k];
            rec->mUnk_04               = tmpl->mUnk_04 * 10 + extra;
            rec->mUnk_0b               = rec->mUnk_04 / 10;
            rec->mUnk_09               = tmpl->mUnk_09;
            rec->mUnk_08               = tmpl->mUnk_08;
            ((u16 *) rec)[0]           = tmpl->mUnk_00;
            ((u16 *) rec)[1]           = tmpl->mUnk_02;
            mSlots54[k].mValue         = data_ov007_0227b0a8[mUnk_f6][mUnk_f7].mUnk_24[k][0];
            mSlots6c[k].mValue         = data_ov007_0227b0a8[mUnk_f6][mUnk_f7].mUnk_24[k][1];
            k++;
        } while (k < 3);
    }
    if (data_020d433c.mPlayerCount == 2) {
        if (index == 0) {
            mUnk_f1 = 0;
        } else if (index == 1) {
            mUnk_f1 = 2;
        }
    } else {
        switch (index) {
            case 0:
                mUnk_f1 = 0;
                break;
            case 1:
                mUnk_f1 = 1;
                break;
            case 2:
                mUnk_f1 = 2;
                break;
            case 3:
                mUnk_f1 = 3;
                break;
        }
    }
    mUnk_f8 = data_020d433c.mUnk_99;
    mUnk_f9 = data_020d433c.mUnk_99;
    MI_CpuClear32((u8 *) this + 0x100, 0x20);
    MI_CpuClear32((u8 *) this + 0x120, 0x20);
    func_ov007_0226a1d8(this);
    if (data_020d433c.mUnk_8a_0 != 0) {
        mRecords[1].mUnk_0a = 2;
        mRecords[2].mUnk_0a = 2;
    }
}

BattlePlayer::~BattlePlayer() {
    if (mUnk_c8) {
        delete mUnk_c8;
    }
}

void BattlePlayer::Update() {
    if (data_020d433c.mUnk_14 & 1) {
        mFinished = 1;
        return;
    }
    switch (mUnk_ef) {
        case 0:
            mUnk_ef = 1;
            break;
        case 1:
            WirelessManager::Get();
            if (IsWirelessMode(&data_020c4030.mUnk_228)) {
                WirelessManager::Get();
                if (data_020c4230.mUnk_20 == data_020d43eb[mUnk_f0]) {
                    WirelessManager::Get().func_0202f078(&data_0220f540);
                    data_0220f540.mUnk_00 = 0;
                }
                u8 *received = WirelessManager::Get().func_0202f0cc(data_020d43eb[mUnk_f0]);
                if (!received) {
                    mMsg.mType = 0;
                } else {
                    func_01ff9510(received, &mMsg, 0x20);
                }
                if (mMsg.mType != 0) {
                    func_01ff9510(&mMsg, (u8 *) this + 0x120, 0x20);
                }
            }
            InputManager::Get();
            break;
        case 2:
            break;
        case 3:
            mFinished = 1;
            break;
    }
}

// non-matching: 0x300 vs 0x2f4 bytes, register allocation of the loop variables and the player table walk.
void func_ov007_02269ee4(BattlePlayer *player, u32 reset) {
    player->mUnk_d4 = 0;
    player->mUnk_d8 = 0;
    u8 n            = data_020d433c.mUnk_99;
    u32 hasZero     = 0;
    u8 ones         = 0;
    u8 i            = 0;
    do {
        u8 state = player->mRecords[i].mUnk_0a;
        if (state == 0) {
            hasZero = 1;
            break;
        }
        if (state == 1) {
            ones++;
        }
        i++;
    } while (i < n);
    data_020d433c.mBattlePlayers->mUnk_292 = 0;
    if (!hasZero) {
        if (data_020d433c.mUnk_8a_0 != 0) {
            if (ones == 1) {
                data_020d433c.mBattlePlayers->mUnk_292 = 1;
            } else {
                i = 0;
                do {
                    if (player->mRecords[i].mUnk_0a == 2) {
                        player->mRecords[i].mUnk_0a = 0;
                    }
                    i++;
                } while (i < data_020d433c.mUnk_99);
            }
        } else {
            if (ones == data_020d433c.mUnk_99) {
                data_020d433c.mBattlePlayers->mUnk_292 = 1;
            } else {
                i = 0;
                do {
                    if (player->mRecords[i].mUnk_0a == 2) {
                        player->mRecords[i].mUnk_0a = 0;
                    }
                    i++;
                } while (i < data_020d433c.mUnk_99);
            }
        }
    }
    if (player->mUnk_f8 == 0) {
        data_020d433c.mBattlePlayers->mUnk_28f = 0;
    }
    if (player->mUnk_c8) {
        player->mUnk_c8->func_ov007_02231038();
    }
    player->mUnk_fa = 0;
    if (reset == 0) {
        u32 flagA = 0;
        u32 best  = 0;
        if (data_020d433c.mUnk_98) {
            BattlePlayerList *list = data_020d433c.mBattlePlayers;
            u8 mine                = player->mUnk_ee;
            u8 self                = player->mUnk_f0;
            u8 total               = mine + list->mPlayers[player->mUnk_f2]->mUnk_ee;
            u8 count               = data_020d433c.mPlayerCount;
            u8 other               = 0;
            u8 sum                 = 0;
            u8 j                   = 0;
            do {
                u8 partner = list->mPlayers[j]->mUnk_f2;
                if (j != self && partner != self) {
                    sum = data_020d433c.mBattlePlayers->mPlayers[j]->mUnk_ee + data_020d433c.mBattlePlayers->mPlayers[partner]->mUnk_ee;
                    break;
                }
                j++;
            } while (j < count);
            s8 diff = sum - total;
            if (sum == 2) {
                flagA = 1;
            }
            if (diff > 0) {
                best = (u8) diff;
            }
        } else {
            u8 count   = data_020d433c.mPlayerCount;
            u8 mine    = player->mUnk_ee;
            BattlePlayerList *list = data_020d433c.mBattlePlayers;
            u8 j       = 0;
            u8 top     = data_020d433c.mUnk_99 - 1;
            do {
                u8 level = list->mPlayers[j]->mUnk_ee;
                if (top == level) {
                    if (j != player->mUnk_f0) {
                        flagA = 1;
                    }
                }
                s8 diff = level - mine;
                if ((s8) best < diff) {
                    best = (u8) diff;
                }
                j++;
            } while (j < count);
        }
        u8 gain = best * 0x28 + 0x1e + flagA * 0x19;
        u32 t   = player->mUnk_f3 + gain;
        if (t < 0xbe) {
            player->mUnk_f3 = t;
        } else {
            player->mUnk_f3 = 0xbe;
        }
    }
    if (data_020d433c.mUnk_8a_0 != 0) {
        player->mRecords[1].mUnk_0a = 2;
        player->mRecords[2].mUnk_0a = 2;
        if (player->mUnk_f0 != 0) {
            u32 t = player->mUnk_f3 + 0xbe;
            if (t < 0xbe) {
                player->mUnk_f3 = t;
            } else {
                player->mUnk_f3 = 0xbe;
            }
        }
        data_020d433c.mBattlePlayers->mUnk_28f = 0;
    }
}

// non-matching: 11 words, `length` is kept in r2 instead of r4 (the original recomputes length * 2 after the copy call).
void func_ov007_0226a1d8(BattlePlayer *player) {
    if (player->mUnk_f6 == 0x15) {
        WirelessManager::Get();
        const u16 *name;
        if (!IsWirelessMode(&data_020c4030.mUnk_228)) {
            name = (const u16 *) data_020d441a;
        } else {
            name = (const u16 *) (data_020d4352 + data_020d43eb[player->mUnk_f0] * 0x18);
        }
        u8 length = 0;
        do {
            if (name[length] == 0) {
                break;
            }
            length++;
        } while (length < 12);
        if (length == 0) {
            length = 1;
        }
        func_01ff94e0(name, (u8 *) player + 0xb0, length * 2);
        player->mUnk_ed = length * 2;
    } else {
        MessageFile *file = (MessageFile *) func_02023504("/font/mes_StoryCharaName.mes", 0, 0, 0, 0, 0);
        player->mUnk_ed   = file->GetLength(player->mUnk_f6);
        TextManager::Get();
        func_01ff94e0(file->GetText(player->mUnk_f6), (u8 *) player + 0xb0, player->mUnk_ed);
        OS_Free(file);
    }
}

struct BattleRecordWord {
    u16 mValue;
    u8 mPad[10];
};

void func_ov007_0226a380(BattlePlayer *player, u8 record, int amount) {
    if (amount > 0) {
        BattleRecordWord *words = (BattleRecordWord *) ((u8 *) player + 0x12);
        BattleRecordWord *own = (BattleRecordWord *) ((u8 *) player + 0x10);
        u16 value = words[record].mValue;
        u16 base  = own[record].mValue;
        words[record].mValue = value + amount;
        u16 limit = 0x3de - base;
        if (words[record].mValue > limit) {
            words[record].mValue = limit;
        }
    } else {
        BattleRecordWord *words = (BattleRecordWord *) ((u8 *) player + 0x12);
        u16 value = words[record].mValue;
        if ((s16) ((s16) value + amount) <= 0) {
            words[record].mValue = 0;
        } else {
            words[record].mValue = value + amount;
        }
    }
}

void func_ov007_0226a404(BattlePlayer *player, u32 index, u32 flag) {
    if (player->mSlots54[index].mFlag == 0) {
        if (flag != 0 && player->mUnk_f8 != 0) {
            player->mUnk_f8--;
        }
    } else {
        if (flag == 0) {
            player->mUnk_f8++;
        }
    }
    player->mSlots54[index].mFlag = flag;
}

void func_ov007_0226a448(BattlePlayer *player, u8 index, int flag) {
    if (player->mSlots6c[index].mFlag == 0) {
        if (flag != 0 && player->mUnk_f9 != 0) {
            player->mUnk_f9--;
        }
    } else {
        if (flag == 0) {
            player->mUnk_f9++;
        }
    }
    player->mSlots6c[index].mFlag = flag;
}

int func_020231f4(UnkStruct_020d43f0 *base, u32 unk1, u32 unk2, u32 unk3);

// non-matching: 0x154 vs 0x15c bytes. The original reads data_020d43f0[0].mUnk_1a_8 again for the +0x6c store (here it is reused) and keeps the constant 2 in r2 across the second flag group.
void func_ov007_0226a48c(BattlePlayer *player) {
    if (data_020d433c.mUnk_85 != 0xe) {
        u8 two = 2;
        player->mSlots6c[0].mFlag = two;
        player->mSlots6c[1].mFlag = two;
        player->mSlots6c[2].mFlag = two;
        if (data_020d433c.mUnk_85 == 1) {
            player->mSlots54[0].mFlag = 0;
        } else {
            player->mSlots54[0].mFlag = two;
        }
        player->mSlots54[1].mFlag = two;
        player->mSlots54[2].mFlag = two;
        if (player->mUnk_f0 != 0) {
            player->mRecords[0].mUnk_04 = 0xe6;
            player->mRecords[0].mUnk_0b = 0x17;
        } else {
            player->mRecords[0].mUnk_08 = 0x16;
            player->mRecords[0].mUnk_04 = 0xbe;
            player->mRecords[0].mUnk_0b = 0x13;
            player->mRecords[0].mUnk_09 = 0;
        }
        player->mRecords[1].mUnk_0a = 2;
        player->mRecords[2].mUnk_0a = 2;
        player->mUnk_f8 = 0;
    } else if (player->mUnk_f0 == 0) {
        player->mRecords[0].mUnk_08 = 0x24;
        int v = func_020231f4(data_020d43f0, 0, 0x24, (u8) data_020d43f0[0].mUnk_1a_8);
        if (v == 0) {
            v = 300;
        }
        player->mRecords[0].mUnk_04 = v;
        player->mRecords[0].mUnk_0b = v / 10;
        player->mRecords[0].mUnk_09 = data_020d43f0[0].mUnk_1a_8;
        player->mSlots6c[0].mValue  = data_020d43f0[0].mUnk_1a_8 + 0x68;
        player->mSlots6c[1].mValue  = 0x8c;
        player->mSlots6c[2].mValue  = 0xac;
        player->mSlots6c[1].mFlag   = 2;
        player->mSlots6c[2].mFlag   = 2;
    } else {
        player->mSlots54[0].mValue = 0x16;
        player->mSlots6c[0].mFlag  = 2;
        player->mSlots6c[1].mFlag  = 2;
        player->mSlots6c[2].mFlag  = 2;
    }
}

// non-matching: 0x114 vs 0x118 bytes, register allocation (original: list in lr, n in ip, owner in r3, i in r5, count in r2) and the random number state is stored as two words instead of with stm.
u8 func_ov007_0226a5e8(BattlePlayer *player) {
    BattlePlayerList *list = data_020d433c.mBattlePlayers;
    u8 n                   = data_020d433c.mUnk_99;
    BattlePlayer *owner    = list->mPlayers[player->mUnk_f0];
    u8 i                   = 0;
    u8 count               = 0;
    u8 first               = 0xff;
    do {
        if (owner->mSlots54[i].mFlag == 0) {
            count++;
        }
        i++;
    } while (i < n);
    i = 0;
    do {
        if (owner->mSlots54[i].mFlag == 0) {
            first = i;
            break;
        }
        i++;
    } while (i < n);
    if (count != 0) {
        u8 pick = MATH_Rand32(&data_020d42e8, count);
        u8 k    = 0;
        i       = 0;
        owner   = list->mPlayers[player->mUnk_f0];
        do {
            if (owner->mSlots54[i].mFlag == 0) {
                if (pick == k) {
                    first = i;
                    break;
                }
                k++;
            }
            i++;
        } while (i < n);
    }
    return first;
}

// non-matching: 0x214 vs 0x21c bytes, register allocation (original: list in r10, owner in r8, n in r9, i in r6, count in r7, result in r11; `1` is loaded into r0 before the compare).
u8 func_ov007_0226a700(BattlePlayer *player, u8 mask, s16 x, s16 y) {
    BattlePlayerList *list = data_020d433c.mBattlePlayers;
    BattlePlayer *owner    = list->mPlayers[player->mUnk_f0];
    u8 n                   = data_020d433c.mUnk_99;
    u8 i                   = 0;
    u8 count               = 0;
    u8 result              = 0xff;
    u8 ret                 = 1;
    do {
        if (owner->mSlots6c[i].mFlag == 0) {
            if (!TestFlagBit(&mask, i)) {
                count++;
            }
        }
        i++;
    } while (i < n);
    if (!(mask & 2) && owner->mSlots6c[1].mFlag == 0) {
        u16 value = owner->mSlots6c[1].mValue;
        if (value >= 0x92 && value <= 0xa7) {
            if ((mask & 1) || owner->mSlots6c[0].mFlag != 0) {
                return ret;
            }
            if (MATH_Rand32(&data_020d42e8, 2) != 0) {
                ret = 0;
            }
            return ret;
        }
    }
    if (count != 0) {
        u8 pick = MATH_Rand32(&data_020d42e8, count);
        u8 k    = 0;
        i       = 0;
        BattlePlayerSlot *slots = owner->mSlots6c;
        do {
            if (slots[i].mFlag == 0 && !TestFlagBit(&mask, i)) {
                if (pick == k) {
                    u16 value = slots[i].mValue;
                    if (value == 0x8c) {
                        if (x > y) {
                            return 0xff;
                        }
                    }
                    if (value >= 0x8e && value <= 0x91) {
                        if (y >= 0) {
                            return 0xff;
                        }
                    }
                    result = i;
                    break;
                }
                k++;
            }
            i++;
        } while (i < n);
    }
    return result;
}

