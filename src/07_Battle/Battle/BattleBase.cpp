#include "Battle/BattleBase.hpp"

#include "Game/Random.hpp"
#include "Game/UnkStruct_020d433c.hpp"
#include "Graphics/Display.hpp"
#include "Graphics/SpriteManager.hpp"
#include "System/SoundManager.hpp"
#include "System/TouchManager.hpp"
#include "Unknown/UnkFunctions.hpp"
#include "Unknown/UnkStruct_020c3030.hpp"
#include "Unknown/UnkStruct_02078290.hpp"
#include <nitro/fx.h>
#include <nitro/gx.h>

// Functions of other battle files.
int func_ov007_0223e214(BattleData *data, int a, int b);
void func_ov007_02229998(BattleField3D *obj);
void func_ov007_022252c4(BattleSub48 *obj);
void func_ov007_0226a448(BattlePlayer *player, u8 a, int b);
void func_ov007_0222844c(BattleHud *obj, u8 a, u8 b);
BOOL func_ov007_02226aa0(BattleHud *obj);
BOOL func_ov007_022631bc(BattlePlayerList *list, u32 id);
void func_ov007_022252a8(BattleSub48 *obj);
void func_ov007_02229758(BattleField3D *obj);
BOOL func_ov007_02229770(BattleField3D *obj);
BOOL func_ov007_022297f0(BattleField3D *obj);
void func_ov007_02226a44(BattleHud *obj, u32 a);
void func_ov007_0222847c(BattleHud *obj);
void func_ov007_02227434(BattleHud *obj);
extern u8 data_ov007_0227915c;
extern u8 data_ov007_02279170;
struct BattleRow {
    s32 mUnk[6];
};
extern BattleRow data_ov007_02278b68[];
extern s32 data_ov007_02278b74;
void func_ov007_02229d08(BattleBase *scene);
void func_ov007_02229c88(BattleBase *scene, u16 *a, u16 *b, u16 *c, u8 flag);

void func_ov007_02226a34(void *);

// non-matching: 4 bytes short. The original truncates a zero (`and r2, r1, #0xff`) for the mUnk_52 store.
BattleBase::BattleBase()
    : IBattle(), SceneBase(0x3e8, 0) {
    u8 zero = 0;
    mUnk_10 = 0;
    mUnk_11 = 0;
    mUnk_12 = 0;
    mUnk_13 = 0;
    mUnk_14 = -1;
    mUnk_15 = 0;
    mUnk_16 = 0;
    mUnk_18_0 = 0;
    mUnk_18_1 = 0;
    mUnk_18_2 = 0;
    mUnk_19 = 2;
    mUnk_1a = 0;
    mUnk_1b = 0;
    mUnk_24 = 0;
    mUnk_25 = 0;
    mUnk_26 = 0;
    mUnk_28 = 0;
    mUnk_29 = 2;
    mUnk_2a = 0;
    mUnk_2c = 0;
    mUnk_40 = 0;
    mUnk_44 = 0;
    mUnk_48 = 0;
    mUnk_4c = 0;
    mUnk_4d = 0;
    mUnk_4e = 0;
    mUnk_4f = 0;
    mUnk_50 = 0xff;
    mUnk_51 = 0;
    mUnk_54_0 = 0;
    mUnk_54_1 = 0;
    mUnk_54_2 = 0;
    mUnk_54_3 = 0;
    mUnk_54_4 = 0;
    mUnk_54_5 = 0;
    mUnk_55 = 0;
    mUnk_56 = 0;
    mUnk_58 = 0;
    mUnk_5c = 0;
    mSlots[3] = 0;
    mSlots[2] = 0;
    mSlots[1] = 0;
    mSlots[0] = 0;
    mUnk_1d = -1;
    mUnk_1c = mUnk_1d;
    mUnk_1f = -1;
    mUnk_1e = mUnk_1f;
    mUnk_52[1] = 0;
    mUnk_52[0] = zero;
    mHelpers[1] = 0;
    mHelpers[0] = 0;
    if (data_020d433c.mUnk_95 != 0 && data_020d433c.mUnk_95 != 7) {
        mUnk_14 = data_020d433c.mUnk_95 - 1;
    }
}

BattleBase::~BattleBase() {
    if (mUnk_40) delete mUnk_40;
    mUnk_48->mUnk_0c = 4;
    func_ov007_02226a34(mUnk_44);
    if (mSlots[3]) delete mSlots[3];
    if (mSlots[2]) delete mSlots[2];
    if (mSlots[1]) delete mSlots[1];
    if (mSlots[0]) delete mSlots[0];
    if (mHelpers[1]) delete mHelpers[1];
    if (mHelpers[0]) delete mHelpers[0];
}

void BattleBase::func_ov007_0221a624() {
    mUnk_51 = 7;
    G2S_SetBG2Priority(0);
    G2S_SetBlendAlpha(4, 0x1b, 0, mUnk_51);
}

BOOL BattleBase::func_ov007_0221a664() {
    u8 ev = ++mUnk_51;
    if (ev >= 0x1f) {
        mUnk_51 = 0x1f;
        return 1;
    }
    G2S_SetBlendAlpha(4, 0x1b, 0, ev);
    return 0;
}

void BattleBase::func_ov007_0221bb28(s16 a, s16 b) {
    BattleHud *p = mUnk_44;
    p->mUnk_12c = a;
    p->mUnk_12e = b;
}

void BattleBase::func_ov007_0221bb3c() {
    if (mUnk_18_1) {
        return;
    }
    mUnk_18_1 = 1;
    mUnk_44->mUnk_23 |= 4;
}

void BattleBase::func_ov007_0221bb68() {
    mUnk_25 = mUnk_20->mUnk_0b;
    mUnk_24 = mUnk_20->mUnk_08;
}

void BattleBase::func_ov007_0221c8f8(u16 *p) {
    if (p[0] != 0) {
        mUnk_4e = 0;
        mUnk_10 = 8;
    } else if (p[1] != 0) {
        mUnk_4e = 1;
        mUnk_10 = 8;
    }
}

BOOL func_ov007_0221b998(int a, int mode, BattleEntry *entry) {
    if (mode != 1) {
        return 0;
    }
    BOOL result = 0;
    if (entry->mUnk_12 != 0) {
        if (func_02065c0c(entry->mUnk_12, 3) == 5) {
            result = 1;
        }
    }
    return result;
}

// non-matching: the original sets the result register after the entry load; this variant differs only there.
BOOL func_ov007_0221b9d0(int a, int mode, BattleEntry *entry) {
    BOOL result = 0;
    if (mode != 1) {
        return 0;
    }
    if (entry->mUnk_12 != 0) {
        if (func_ov007_02222764(result, entry) == 5) {
            result = 1;
        }
    }
    return result;
}

BOOL func_ov007_0221ba0c(int a, int mode, BattleEntry *entry) {
    if (mode != 1) {
        return 0;
    }
    if (entry->mUnk_12 >= 0x8e && entry->mUnk_12 <= 0x91) {
        return 1;
    }
    return 0;
}

BOOL func_ov007_0221ba38(int a, u8 side, int value, BattleEntry *entries) {
    u8 other = side ^ 1;
    if (entries[other].mUnk_12 == 0) {
        return 0;
    }
    BOOL result = 0;
    int r = func_ov007_02222764(other, entries);
    if (r != -1) {
        if (value == r) {
            result = 1;
        }
    }
    return result;
}

int func_ov007_0221ba88(int a, int index, BattleEntry *entries) {
    int result = 0;
    if (func_02065c0c(entries[index].mUnk_12, 3) == 2) {
        switch (entries[index].mUnk_12) {
        case 0x78: result = 1; break;
        case 0x79: result = 2; break;
        case 0x7a: result = 3; break;
        case 0x7b: result = 4; break;
        case 0x7c: result = 5; break;
        case 0x7d: result = 6; break;
        case 0x7e: result = 7; break;
        case 0x7f: result = 8; break;
        }
    }
    return result;
}

void BattleBase::func_ov007_0221a55c() {
    SpriteManager::Get().func_02026dec(0, 7);
    func_02001e68(1, 0, 1);
    G2_SetBG0Priority(2);
    G2_SetBG1Priority(2);
    G2_SetBG2Priority(2);
    G2_SetBG3Priority(2);
}

int BattleBase::func_ov007_0221c850(u16 *a, u16 *b, u16 *c, u8 *d) {
    u8 i = 0;
    do {
        if (data_020d433c.mBattlePlayers->mPlayers[i]->IsActive()) {
            if (mSlots[i]->func_ov007_02223260(a, b, c, (s8 *) &mUnk_1c, mUnk_44->mUnk_13, d) != -1) {
                break;
            }
        }
        i++;
    } while (i < data_020d433c.mPlayerCount);
    return -1;
}

int BattleBase::func_ov007_0221cebc(int a) {
    return func_ov007_0223e214(mUnk_2c, a, 0);
}

void func_02001eec(vu16 *reg, int value);

// Differs from the shared MATH_Rand32: max == 0 returns the raw high word.

u32 func_ov007_0221d348(u32 max) {
    return MATH_Rand32(&data_020d42e8, max);
}

void BattleBase::func_ov007_0221d488(u16 *p) {
    func_ov007_0221c8f8(p);
}

int BattleBase::func_ov007_0221d494(u16 *a, u16 *b, u16 *c, u8 *d) {
    return func_ov007_0221c850(a, b, c, d);
}

void BattleBase::func_ov007_0221ced0(u16 *a, u16 *b, u16 *c) {
    if (!data_020d433c.mUnk_9a_2) {
        return;
    }
    u8 i = 0;
    BattleEntry *entries = mUnk_2c->mEntries;
    do {
        if (mHelpers[i]) {
            u8 other = i ^ 1;
            if (!entries[other].mUnk_0f_3) {
                if (mHelpers[i]->func_ov007_02223638(i, mUnk_44->mUnk_13)) {
                    c[i] = 1;
                    if (i == 0) {
                        a[entries[0].mUnk_0d_4] = 0;
                    } else {
                        b[entries[1].mUnk_0d_4] = 0;
                    }
                }
            }
        }
        i++;
    } while (i < 2);
}

void BattleBase::func_ov007_0221cfac(u16 *a, u16 *b) {
    BattleData *data = mUnk_2c;
    u8 i = 0;
    do {
        if (i != data->mEntries[0].mUnk_0d_4 && i != data->mEntries[1].mUnk_0d_4) {
            u8 flag = a[i];
            if (flag != 0) {
                if (mUnk_1c == -1) {
                    mUnk_1c = i;
                } else {
                    mUnk_1d = i;
                }
                a[i] = a[i] & 0xff00;
                if (flag == 2) {
                    a[i] |= mUnk_13;
                }
            } else {
                flag = b[i];
                if (flag != 0) {
                    if (mUnk_1e == -1) {
                        mUnk_1e = i;
                    } else {
                        mUnk_1f = i;
                    }
                    b[i] = b[i] & 0xff00;
                    if (flag == 2) {
                        b[i] |= mUnk_13;
                    }
                }
            }
        }
        i++;
    } while (i < data_020d433c.mPlayerCount);
}

BOOL BattleBase::func_ov007_0221d398() {
    BOOL done = 0;
    if (mUnk_28 == 0) {
        mUnk_26 += 3;
        if (mUnk_26 >= 15) {
            if (mUnk_18_0) {
                func_ov007_02229998(mUnk_40);
                SoundManager::Get().func_020246f8(0x25);
                func_ov007_022252c4(mUnk_48);
            }
            mUnk_28++;
        }
    } else {
        mUnk_26 -= 3;
        if (mUnk_26 <= 0) {
            mUnk_26 = 0;
            mUnk_28 = 0;
            done = 1;
        }
    }
    func_02001eec((vu16 *) 0x400006c, mUnk_26);
    func_02001eec((vu16 *) 0x400106c, mUnk_26);
    return done;
}

void BattleBase::func_ov007_0221a6b4() {
    BattleEntry *entries = mUnk_2c->mEntries;
    mUnk_56 = entries[mUnk_4e].mUnk_12;
    mUnk_4f = func_02065c0c(entries[mUnk_4e].mUnk_12, 3);
    mUnk_2a = 0;
    func_ov007_0226a448(data_020d433c.mBattlePlayers->mPlayers[entries[mUnk_4e].mUnk_0d_4], entries[mUnk_4e].mUnk_11, 2);
    BattleBase::func_ov007_0221a624();
    mUnk_54_3 = 1;
    if (!mUnk_54_0) {
        mUnk_44->mUnk_22 &= ~0x10;
    }
    func_ov007_0222844c(mUnk_44, mUnk_4e, mUnk_4f);
    func_ov007_022252c4(mUnk_48);
    mUnk_1d = -1;
    mUnk_1c = mUnk_1d;
    mUnk_1f = -1;
    mUnk_1e = mUnk_1f;
    if (entries[0].mUnk_12 == 0x8d && entries[1].mUnk_12 == 0x8d) {
        mUnk_54_2 = 1;
    }
    if (entries[0].mUnk_12 == 0x8c || entries[1].mUnk_12 == 0x8c) {
        mUnk_54_2 = 1;
    }
}

void BattleBase::func_ov007_0221b8e8() {
    BOOL done = 0;
    if (mUnk_54_0) {
        done = 1;
        mUnk_10 = mUnk_4c;
    } else {
        switch (mUnk_4f) {
        case 0:
            if (!func_ov007_02226aa0(mUnk_44)) {
                mUnk_10 = 7;
                done = 1;
            }
            break;
        case 1:
        case 3:
        case 4:
            if (!func_ov007_02226aa0(mUnk_44)) {
                mUnk_10 = 7;
                done = 1;
            }
            break;
        }
    }
    if (done) {
        mUnk_44->mUnk_117 &= ~4;
        mUnk_54_0 = 0;
    }
}

BOOL BattleBase::func_ov007_0221bd84() {
    BOOL done = 0;
    switch (mUnk_11) {
    case 0:
        if (func_ov007_022631bc(data_020d433c.mBattlePlayers, 0xf)) {
            func_ov007_022252a8(mUnk_48);
            SpriteManager::Get().StartFade(2);
            mUnk_11++;
        }
        break;
    case 1:
        mUnk_2a++;
        if (mUnk_2a >= 0x3c) {
            mUnk_2a = 0;
            mUnk_11++;
        }
        break;
    case 2:
        if (!func_ov007_02226aa0(mUnk_44)) {
            func_ov007_02229758(mUnk_40);
            mUnk_11++;
        }
        break;
    case 3:
        if (func_ov007_02229770(mUnk_40)) {
            SoundManager::Get().func_020246f8(0x23);
            SoundManager::Get().func_020246f8(0x24);
            mUnk_18_2 = 1;
            mUnk_44->mUnk_22 |= 0x80;
            mUnk_44->mUnk_22 |= 0x10;
            mUnk_11++;
        }
        break;
    case 4:
        if (func_ov007_022631bc(data_020d433c.mBattlePlayers, 0x10)) {
            mUnk_11 = 0;
            done = 1;
        }
        break;
    }
    return done;
}

BOOL BattleBase::func_ov007_0221bfa4() {
    BOOL done = 0;
    switch (mUnk_11) {
    case 0:
        func_ov007_02226a44(mUnk_44, 3);
        mUnk_18_2 = 1;
        mUnk_11++;
        break;
    case 1:
        if (!func_ov007_02226aa0(mUnk_44)) {
            if (func_ov007_022297f0(mUnk_40)) {
                mUnk_2a = 0;
                mUnk_11++;
            }
        }
        break;
    case 2:
        mUnk_2a++;
        if (mUnk_2a >= 0x64) {
            mUnk_2a = 0;
            SpriteManager::Get().StartFade(3);
            mUnk_11++;
        }
        break;
    case 3:
        if (!SpriteManager::Get().IsFading()) {
            mUnk_11++;
        }
        break;
    case 4:
        if (func_ov007_022631bc(data_020d433c.mBattlePlayers, 0xf)) {
            done = 1;
        }
        break;
    }
    return done;
}

extern "C" void func_02024764(SoundManager *manager, u32 a, u32 b);

BOOL BattleBase::func_ov007_0221bb84() {
    BattleHud *sub = mUnk_44;
    u8 n = sub->mUnk_13;
    BOOL done = 0;
    mUnk_16++;
    if (mUnk_16 >= mUnk_20->mUnk_02) {
        if (n == data_ov007_0227915c) {
            u8 idx = MATH_Rand32(&data_020d42e8, 2);
            BattleData *d = mUnk_2c;
            d->mEntries[idx].mUnk_10 = 1;
            d->mEntries[idx].mUnk_0f_6 = 0;
        } else if (n > data_ov007_0227915c) {
            BattleData *data = mUnk_2c;
            data->mEntries[0].mUnk_10 = 1;
            data->mEntries[0].mUnk_0f_6 = 0;
        } else {
            BattleData *data = mUnk_2c;
            data->mEntries[1].mUnk_10 = 1;
            data->mEntries[1].mUnk_0f_6 = 0;
        }
        done = 1;
    } else if (n >= data_ov007_02279170 - 1) {
        BattleData *data = mUnk_2c;
        done = 1;
        data->mEntries[0].mUnk_10 = 1;
        data->mEntries[0].mUnk_0f_6 = 1;
        mUnk_44->mUnk_22 |= 0x20;
    } else if (n <= 1) {
        BattleData *data = mUnk_2c;
        done = 1;
        data->mEntries[1].mUnk_10 = 1;
        data->mEntries[1].mUnk_0f_6 = 1;
        mUnk_44->mUnk_22 |= 0x20;
    }
    if (done) {
        func_ov007_0222847c(mUnk_44);
        mUnk_18_0 = 1;
        func_02024764(&SoundManager::Get(), 0x24, 1);
    } else if (!mUnk_18_0) {
        func_ov007_02227434(mUnk_44);
    }
    return done;
}

void BattleBase::func_ov007_02219888(BattleData *data, u32 mode) {
    mUnk_2c = data;
    mUnk_20 = &data_ov007_02278b20[mode];
    mUnk_15 = mode;
    BattlePlayerList *list = data_020d433c.mBattlePlayers;
    u8 sel = list->mUnk_2a9;
    BattleEntry *entries = data->mEntries;
    u8 first = entries[0].mUnk_0d_4;
    if (sel == first) {
        mUnk_12 = 0;
    } else if (sel == entries[1].mUnk_0d_4) {
        mUnk_12 = 1;
    } else {
        if (data_020d433c.mUnk_98 != 0) {
            if (sel == list->mPlayers[first]->mUnk_f2) {
                mUnk_12 = 3;
                if (data_020d433c.mUnk_7a != 0) {
                    mUnk_29 = 0;
                }
            } else {
                mUnk_12 = 4;
                if (data_020d433c.mUnk_7a != 0) {
                    mUnk_29 = 1;
                }
            }
        } else {
            mUnk_12 = 2;
        }
        mUnk_15 = 7;
    }
    if (data_020d433c.mPlayerCount == 3) {
        mUnk_13 = 30;
    } else if (data_020d433c.mPlayerCount == 4) {
        mUnk_13 = 15;
    }
    u8 order[2] = {0, 0};
    u8 count = 0;
    u8 i = 0;
    do {
        if (data_020d433c.mBattlePlayers->mPlayers[i]->IsActive()) {
            BattleSlot *slot = new BattleSlot(data, i, mode, data_020d433c.mBattlePlayers->mPlayers[i]->mUnk_f6,
                                              entries[0].mUnk_0d_4, entries[1].mUnk_0d_4, mUnk_20, mUnk_13);
            mSlots[i] = slot;
            if (slot->mRole == 2 || slot->mRole == 3) {
                order[count++] = i;
            }
        }
        i++;
    } while (i < data_020d433c.mPlayerCount);
    if (data_020d433c.mUnk_9a_2) {
        u8 j = 0;
        do {
            u8 nib = entries[j].mUnk_0d_4;
            if (!data_020d433c.mBattlePlayers->mPlayers[nib]->IsActive()) {
                mHelpers[j] = new BattleSideHelper(j, data_020d433c.mBattlePlayers->mPlayers[nib]->mUnk_f6, entries);
            }
            j++;
        } while (j < 2);
    }
    if (count == 2 && (mode == 0 || mode == 3)) {
        mSlots[order[0]]->mUnk_14 |= 4;
        mSlots[order[1]]->mUnk_14 |= 4;
    }
    func_ov007_02219888(data, mode);
    mUnk_44 = new BattleHud(data, mUnk_20, mUnk_12, (s8 *) &mUnk_1c);
    mUnk_48 = new BattleSub48(data, mUnk_12);
    mUnk_40 = new BattleField3D(data, 0);
    if (data_020d433c.mUnk_10) {
        data_020d433c.mUnk_10->func_020785d8(0xb);
    }
}

// non-matching: 4 bytes long. The original reloads counts[i] after the hi[i] store.
void BattleBase::func_ov007_0221d088(u8 a, u16 *counts) {
    BattlePlayerList *list = data_020d433c.mBattlePlayers;
    u8 sel = list->mUnk_2a9;
    u16 hi[4] = {0, 0, 0, 0};
    u8 i = 0;
    do {
        hi[i] = counts[i] >> 8;
        counts[i] = counts[i] & 0xff;
        i++;
    } while (i < data_020d433c.mPlayerCount);
    u8 p = 0;
    do {
        if (hi[p] != 0) {
            u8 k = 0;
            do {
                if (sel == p) {
                    mUnk_44->func_ov007_022275c0(a, TouchManager::Get().mX, TouchManager::Get().mY, mUnk_15);
                } else if (!data_020d433c.mBattlePlayers->mPlayers[p]->IsActive()) {
                    func_ov007_0221d348(0xffff);
                }
                k++;
            } while (k < hi[p]);
        }
        p++;
    } while (p < data_020d433c.mPlayerCount);
    s32 total = 0;
    u8 j = 0;
    do {
        total += counts[j];
        j++;
    } while (j < data_020d433c.mPlayerCount);
    if (total != 0) {
        s32 v = 0x1000;
        s8 cur = mUnk_14;
        if (cur != -1) {
            BattleEntry *e = &mUnk_2c->mEntries[a];
            v = data_ov007_02278b68[cur].mUnk[e->mUnk_0d_4];
            if (e->mUnk_0c == 5) {
                if (v < data_ov007_02278b68[cur].mUnk[3]) {
                    v = data_ov007_02278b68[cur].mUnk[3];
                }
            } else if (e->mUnk_0c == 6) {
                if (v < data_ov007_02278b68[cur].mUnk[0]) {
                    v = data_ov007_02278b68[cur].mUnk[0];
                }
            }
        }
        mUnk_44->func_ov007_02227490(a, FX_Mul(v, mUnk_20->mUnk_04), total, mUnk_52[a], 1);
    }
    if (mUnk_20->mUnk_00 == 5) {
        vfunc_44(a, total);
    }
}

// non-matching: 8 bytes long; register choice for a[] and value/side, and the wireless (u8) n << 8 form.
void BattleBase::func_ov007_0221c938() {
    u16 a[4] = {0, 0, 0, 0};
    u16 b[4] = {0, 0, 0, 0};
    s32 sums[2] = {0, 0};
    u16 c[2] = {0, 0};
    u8 flag = 0;
    u32 sel = data_020d433c.mBattlePlayers->mUnk_2a9;
    mUnk_1d = -1;
    mUnk_1c = mUnk_1d;
    mUnk_1f = -1;
    mUnk_1e = mUnk_1f;
    UnkStruct_020c3030::Get();
    if (IsWirelessMode(&data_020c4258)) {
        func_ov007_02229d08(this);
    }
    int found = func_ov007_0221c178(a, b, c, &flag);
    BOOL doneB = 1;
    BOOL doneA = 1;
    if (found != -1) {
        if (mUnk_12 <= 1) {
            mUnk_25++;
            if (mUnk_25 >= mUnk_20->mUnk_0b) {
                mUnk_25 = 0;
            } else {
                doneB = 0;
            }
            mUnk_24++;
            if (mUnk_24 >= mUnk_20->mUnk_08) {
                mUnk_24 = 0;
            } else {
                doneA = 0;
            }
        }
        u8 side;
        u16 value = 0;
        if (a[sel] != 0) {
            side = 0;
            value = a[sel];
        } else if (b[sel] != 0) {
            value = b[sel];
            side = 1;
        }
        UnkStruct_020c3030::Get();
        if (!IsWirelessMode(&data_020c4258)) {
            if (doneB) {
                u8 k = 0;
                u32 n;
                do {
                    mUnk_44->func_ov007_022275c0(side, TouchManager::Get().mX, TouchManager::Get().mY, mUnk_15);
                    n = mUnk_44->func_ov007_022275a0(value, mUnk_20->mUnk_09, mUnk_20->mUnk_0a);
                    k++;
                } while (k < n);
            }
            if (!doneA) {
                a[sel] = 0;
                b[sel] = 0;
            }
        } else {
            if (doneB) {
                u8 n = mUnk_44->func_ov007_022275a0(value, mUnk_20->mUnk_09, mUnk_20->mUnk_0a);
                if (a[sel] != 0) {
                    a[sel] |= n << 8;
                } else {
                    b[sel] |= n << 8;
                }
            }
            if (!doneA) {
                a[sel] &= 0xff00;
                b[sel] &= 0xff00;
            }
        }
    }
    UnkStruct_020c3030::Get();
    if (!IsWirelessMode(&data_020c4258)) {
        func_ov007_0221c850(a, b, c, &flag);
        u8 count = data_020d433c.mPlayerCount;
        u8 i = 0;
        do {
            sums[0] += a[i];
            sums[1] += b[i];
            i++;
        } while (i < count);
        BattleEntry *entries = mUnk_2c->mEntries;
        u8 s = 0;
        do {
            if (sums[s] != 0) {
                s32 v = 0x1000;
                s8 cur = mUnk_14;
                if (cur != -1) {
                    v = data_ov007_02278b68[cur].mUnk[entries[s].mUnk_0d_4];
                    if (entries[s].mUnk_0c == 5) {
                        if (v < data_ov007_02278b68[cur].mUnk[3]) {
                            v = data_ov007_02278b68[cur].mUnk[3];
                        }
                    } else if (entries[s].mUnk_0c == 6) {
                        if (v < data_ov007_02278b68[cur].mUnk[0]) {
                            v = data_ov007_02278b68[cur].mUnk[0];
                        }
                    }
                }
                if (mUnk_29 == s && v < 0x1000) {
                    v = 0x1000;
                }
                mUnk_44->func_ov007_02227490(s, FX_Mul(v, mUnk_20->mUnk_04), sums[s], mUnk_52[s], 1);
            }
            s++;
        } while (s < 2);
        func_ov007_0221c8f8(c);
    } else {
        func_ov007_02229c88(this, a, b, c, flag);
    }
}
