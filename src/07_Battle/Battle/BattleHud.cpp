


#include "Battle/BattleBase.hpp"
#include "UI/MessageWindow.hpp"
#include "Battle/BattleObj01.hpp"

#include "Game/UnkStruct_020d433c.hpp"
#include "Graphics/CellAnimation.hpp"
#include "Graphics/SpriteManager.hpp"
#include "System/FileSystem.hpp"
#include "System/InputManager.hpp"
#include "System/SoundManager.hpp"
#include "System/TouchManager.hpp"
#include "Unknown/UnkStruct_020651bc.hpp"
#include <nitro/gx.h>
#include <nitro/fx.h>
#include <stdio.h>

// Externals of other battle files.
u32 func_ov007_0221d348(u32 max);
// Object at BattlePlayerList+0x2a4: a timed screen effect (fade) the battle shows before the gauge starts.
void func_02087eec(UnkStruct_Battle2a4 *effect);
void func_02088e64(UnkStruct_Battle2a4 *effect, u32 unk2, u32 unk3, u32 unk4, u32 unk5);
void func_02088dac(UnkStruct_Battle2a4 *effect, u32 unk2);
BOOL func_02087cac(UnkStruct_Battle2a4 *effect);
extern u8 data_ov007_02279178[];
extern u32 data_ov007_02279184[];
extern s32 data_ov007_0227918c[];

BattleHud::BattleHud(BattleData *data, BattleParam *param, u8 mode, s8 *choice)
    : SceneBase(2, 0) {
    mUnk_0c = mode;
    mUnk_10 = 0;
    mUnk_12 = 0;
    mUnk_13 = 0;
    mUnk_14 = 0;
    mUnk_20 = 0;
    mUnk_22_0 = 0;
    mUnk_22_1 = 0;
    mUnk_22_2 = 0;
    mUnk_22_3 = 0;
    mUnk_22_4 = 0;
    mUnk_22_5 = 0;
    mUnk_22_6 = 1;
    mUnk_22_7 = 0;
    mUnk_23_0 = 0;
    mUnk_23_1 = 0;
    mUnk_23_2 = 0;
    mUnk_24 = 0;
    mChoice = (s8 (*)[2]) choice;
    mUnk_98 = 0;
    mUnk_9c = 0;
    mUnk_9f = 0;
    mUnk_c0 = 0;
    mUnk_c3 = 0;
    mUnk_114 = 0;
    mUnk_115 = 0;
    mUnk_116 = 0;
    mUnk_117_0 = 0;
    mUnk_117_1 = 0;
    mUnk_117_2 = 0;
    mUnk_117_3 = 0;
    mUnk_120 = 0;
    mUnk_124 = 0;
    mUnk_126 = 0;
    mUnk_128 = 0;
    mUnk_129 = 0;
    mUnk_12b = 0;
    mUnk_12c = 0;
    mUnk_12e = 0;
    mData = data;
    mParam = param;
    mUnk_18 = 8;
    mUnk_1a = 0xb8;
    mUnk_1c[1] = 0;
    mUnk_1c[0] = mUnk_1c[1];
    mUnk_28[0] = mUnk_28[1] = 0;
    mUnk_26 = mUnk_27 = 0;
    mUnk_122[0] = mUnk_122[1] = 0;
    mUnk_a0[0] = mUnk_a0[1] = 0;
    mUnk_a8[0] = mUnk_a8[1] = 0;
    u8 i = 0;
    do {
        mAnims[i] = 0;
        i++;
    } while (i < 23);
    mUnk_100[0] = mUnk_100[1] = 0;
    mUnk_102[0] = mUnk_102[1] = 0;
    mUnk_104[0] = mUnk_104[1] = 0;
    i = 0;
    do {
        mObjs[i] = new BattleObj01(5, 0x7000);
        i++;
    } while (i < 15);
    mUnk_108 = 32.0f;
    mUnk_10c = 224.0f;
    mUnk_110 = 96.0f / (float) mParam->mUnk_02;
    mUnk_118[0] = 1;
    mUnk_118[1] = 1;
    mUnk_11c[0] = -32;
    mUnk_11c[1] = 0x120;
    mUnk_11a = mUnk_11b = 0;
    func_ov007_022257e0();
}

BattleHud::~BattleHud() {
    u8 i = 0;
    do {
        CellAnimation *anim = mAnims[i];
        if (anim) {
            delete anim;
        }
        i++;
    } while (i < 23);
    i = 0;
    do {
        BattleObj01 *obj = mObjs[i];
        if (obj) {
            delete obj;
        }
        i++;
    } while (i < 15);
    mData->mUnk_6c->mUnk_27 = 0;
}

void BattleHud::Update() {
    if (mUnk_23_1) {
        mFinished = 1;
        return;
    }
    switch (mUnk_12b) {
        case 1:
            if (data_020d433c.mUnk_85 == 0xe || (data_020d433c.mUnk_85 >= 7 && data_020d433c.mUnk_85 <= 10)) {
                mUnk_12b = 2;
                func_02087eec(data_020d433c.mBattlePlayers->mUnk_2a4);
                func_02088e64(data_020d433c.mBattlePlayers->mUnk_2a4, data_020d433c.mBattlePlayers->mUnk_2af, 0x64, 0, 0);
                G2S_SetBlendBrightness(0x1f, -10);
            } else {
                func_ov007_02226ab8();
                mUnk_12b = 3;
            }
            break;
        case 2: {
            if (InputManager::Get().IsHeld(1) || TouchManager::Get().mTouching) {
                func_02088dac(data_020d433c.mBattlePlayers->mUnk_2a4, 1);
            }
            u32 limit = data_020d433c.mUnk_83 == 4 ? 0 : 5;
            if (!func_02087cac(data_020d433c.mBattlePlayers->mUnk_2a4)) {
                data_020d433c.mBattlePlayers->mUnk_2af++;
                u8 n = data_020d433c.mBattlePlayers->mUnk_2af;
                if (n > limit) {
                    func_ov007_02226ab8();
                    mUnk_12b = 3;
                    G2S_SetBlendBrightness(0x1f, 0);
                } else {
                    func_02088e64(data_020d433c.mBattlePlayers->mUnk_2a4, n, 0x64, 0, 0);
                }
            }
            break;
        }
        case 3:
            if (func_ov007_02226c70()) {
                mUnk_12b = 0;
            }
            break;
        case 4:
            func_ov007_02226e7c();
            mUnk_12b++;
        case 5:
            if (func_ov007_02226f1c()) {
                mUnk_12b = 0;
            }
            break;
        case 6:
            func_ov007_02226fc8();
            mUnk_12b++;
        case 7:
            if (func_ov007_02227068()) {
                mUnk_12b = 0;
            }
            break;
        case 8:
            func_ov007_02227128();
            mUnk_12b++;
        case 9:
            if (func_ov007_02227158()) {
                mUnk_12b = 0;
            }
            break;
    }
    func_ov007_02227730();
}

void func_ov007_02226a34(void *obj) {
    ((BattleHud *) obj)->mUnk_23 |= 2;
}

void func_ov007_02226a44(BattleHud *obj, u32 state) {
    switch (state) {
        case 0:
            obj->mUnk_12b = 0;
            break;
        case 1:
            obj->mUnk_12b = 1;
            break;
        case 2:
            obj->mUnk_12b = 4;
            break;
        case 3:
            obj->mUnk_12b = 6;
            break;
        case 4:
            obj->mUnk_12b = 8;
            break;
    }
}

u8 func_ov007_02226aa0(BattleHud *obj) {
    return obj->mUnk_12b ? 1 : 0;
}

void func_ov007_02227434(BattleHud *obj) {
    obj->mUnk_108 = obj->mUnk_108 + obj->mUnk_110;
    obj->mUnk_10c = obj->mUnk_10c - obj->mUnk_110;
}

void func_ov007_0222844c(BattleHud *obj, u8 a, u8 b) {
    obj->mUnk_118[a] = 0;
    obj->mUnk_11c[0] = -32;
    obj->mUnk_11c[1] = 0x120;
    obj->mUnk_117 |= 0xc;
}

void func_ov007_0222847c(BattleHud *obj) {
    CellAnimation *anim = obj->mAnims[9];
    if (anim->mUnk_0e != 2) {
        anim->SetAnimation(2);
    }
}

// non-matching: register order: the original has screen r5, entries r6, selected r7.
void BattleHud::func_ov007_0222644c(u8 side, int card) {
    u8 index;
    SpriteScreen &screen = SpriteManager::Get().Screen(1);
    BattleEntry *entries = mData->mEntries;
    u8 selected = data_020d433c.mBattlePlayers->mUnk_2a9;
    screen.func_02027624(3);
    index = side + 0x11;
    ChangeDir("/Game/Card/Graphic/Small");
    char path[0x80];
    if (selected == entries[side].mUnk_0d_4) {
        sprintf(path, "card_s_%03d_ncg.bin", card);
    } else {
        sprintf(path, "card_s_000_ncg.bin", card);
    }
    u32 base = data_ov007_02279184[side];
    screen.LoadCharacters(path, base, 0);
    if (side == 0) {
        screen.func_02027fec("card_s_000_ncl.bin", 0x600, 0);
    }
    mAnims[index] = new CellAnimation("card_s_000_nce.bin", 0, 1, 1);
    mAnims[index]->mCharBase = screen.func_020276f4(base);
    mAnims[index]->mUnk_11 = 3;
    mAnims[index]->SetAnimation(0);
    ChangeDir("/");
}

void BattleHud::func_ov007_02226614() {
    CellAnimation *old = mAnims[22];
    if (old) {
        delete old;
    }
    mAnims[22] = 0;
    SpriteScreen &screen = SpriteManager::Get().Screen(0);
    screen.func_02027624(1);
    ChangeDir("/Game/Confrontation/Obj/");
    screen.LoadCharacters("vs_b_obj_countdown.nce_ncg1D.bin", 0, 0);
    screen.LoadPalette("vs_b_obj_countdown_ncl.bin", 0, 0x20);
    mAnims[22] = new CellAnimation("vs_b_obj_countdown_nce.bin", 0, 1, 1);
    mAnims[22]->mCharBase = screen.func_020276f4(0);
    mAnims[22]->mUnk_11 = 0;
    mAnims[22]->SetAnimation(0);
    ChangeDir("/");
}

// non-matching: 2 words: temporary register naming in the and/asr pair.
void BattleHud::func_ov007_02226ab8() {
    u32 hi;
    u32 lo;
    int swapped;
    BattleData *data;
    BattleEntry *entries;
    u32 a;
    swapped = 0;
    data = mData;
    entries = data->mEntries;
    lo = entries[1].mUnk_00;
    a = data->mEntries[0].mUnk_00;
    if (a > lo) {
        hi = a;
    } else {
        hi = lo;
        lo = a;
        swapped = 1;
    }
    if ((int) hi / (int) lo >= 2) {
        if (swapped == 0) {
            mUnk_9f = 0x59;
        } else {
            mUnk_9f = 0xb;
        }
    } else {
        u8 t = (int) (lo * 100) / (int) hi;
        mUnk_9f = t >> 1;
        if (swapped == 0) {
            mUnk_9f = 100 - mUnk_9f;
        }
    }
    mUnk_9c = mAnims[22]->func_0202a54c();
    u8 pct = mUnk_9f;
    mUnk_a0[0] = (float) pct / (float) mUnk_9c / 100.0f;
    mUnk_a0[1] = (float) (u8) (100 - pct) / (float) mUnk_9c / 100.0f;
    u8 i = 0;
    do {
        int v = entries[i].mUnk_00;
        if (v > 20) {
            mUnk_100[i] = v / 20;
        }
        mUnk_102[i] = mUnk_9c / mUnk_100[i];
        i++;
    } while (i < 2);
    mUnk_104[0] = mUnk_102[0];
    mUnk_104[1] = mUnk_102[1];
    mUnk_b0[0] = entries[0].mUnk_00;
    mUnk_b0[1] = entries[1].mUnk_00;
    mUnk_b8[0] = mUnk_b0[0] / (float) mUnk_9c;
    mUnk_b8[1] = mUnk_b0[1] / (float) mUnk_9c;
    mUnk_20 = mParam->mUnk_01;
}

// non-matching: 2 words: the mov r2, #0 is scheduled before the second float load.
BOOL BattleHud::func_ov007_02226c70() {
    BOOL done = 0;
    mUnk_a8[0] = mUnk_a8[0] + mUnk_a0[0];
    mUnk_a8[1] = mUnk_a8[1] + mUnk_a0[1];
    if (mUnk_98 == mUnk_9c - mUnk_c0) {
        mAnims[3]->SetAnimation(10);
        mAnims[4]->SetAnimation(9);
    }
    u8 i = 0;
    do {
        if (mUnk_100[i] != 0) {
            mUnk_104[i]++;
            if (mUnk_104[i] >= mUnk_102[i]) {
                mUnk_104[i] = 0;
                mUnk_100[i]--;
                if (i == 0) {
                    func_ov007_022275c0(0, 0x28, 0xb4, 6);
                } else {
                    func_ov007_022275c0(1, 0xd8, 0xb4, 6);
                }
            }
        }
        if (mUnk_b0[i] - mUnk_b8[i] < 0.0f) {
            mUnk_b0[i] = 0;
        } else {
            mUnk_b0[i] -= mUnk_b8[i];
        }
        i++;
    } while (i < 2);
    func_ov007_02227158();
    mUnk_98++;
    if (mUnk_98 >= mUnk_9c) {
        mUnk_13 = mUnk_9f;
        int fixed = mUnk_13 << 12;
        float rounded;
        if (mUnk_13 != 0) {
            rounded = 0.5f + (float) fixed;
        } else {
            rounded = (float) fixed - 0.5f;
        }
        mUnk_14 = (s32) rounded;
        mUnk_22_3 = 1;
    }
    if (mUnk_a8[0] > 0.5f) {
        func_ov007_02227460(0);
    } else if (mUnk_a8[1] > 0.5f) {
        func_ov007_02227460(1);
    }
    if (mUnk_22_3 == 1 && mUnk_c3 == 0) {
        done = 1;
    }
    return done;
}

void BattleHud::func_ov007_02226e7c() {
    BattleData *data = mData;
    mUnk_a0[0] = mUnk_a0[1] = 0;
    mUnk_98 = 0;
    u32 pos = mUnk_13;
    u8 other = 100 - pos;
    u16 diff;
    if (pos > other) {
        diff = pos - other;
        mUnk_23_0 = 1;
    } else {
        diff = other - pos;
        mUnk_23_0 = 0;
    }
    mUnk_a0[mUnk_23_0] = (float) diff / 20.0f;
    u16 first = data->mEntries[0].mUnk_00;
    u16 second = data->mEntries[1].mUnk_00;
    mData->mEntries[0].mUnk_00 = second;
    mData->mEntries[1].mUnk_00 = first;
    func_ov007_02227128();
}

BOOL BattleHud::func_ov007_02226f1c() {
    mUnk_98++;
    if (mUnk_98 > 0x14) {
        mUnk_98 = 0x15;
    } else {
        u8 idx = mUnk_23_0;
        float v = mUnk_a0[idx];
        float f;
        if (v > 0.0f) {
            f = 0.5f + 4096.0f * mUnk_a0[idx];
        } else {
            f = 4096.0f * mUnk_a0[idx] - 0.5f;
        }
        func_ov007_02227490(idx, (s32) f, 1, 0, 0);
    }
    return func_ov007_02227158();
}

void BattleHud::func_ov007_02226fc8() {
    BattleData *data = mData;
    mUnk_22_3 = 0;
    mUnk_a8[0] = (float) mUnk_13 / 100.0f;
    mUnk_a8[1] = 1.0f - mUnk_a8[0];
    if (data->mEntries[0].mUnk_10) {
        mAnims[3]->SetAnimation(8);
        mUnk_27 = 1;
    } else {
        mAnims[4]->SetAnimation(7);
        mUnk_26 = 1;
    }
    mUnk_10 = 0;
    if (mUnk_128) {
        func_ov007_022286c8();
    }
}

BOOL BattleHud::func_ov007_02227068() {
    BattleData *data = mData;
    if (data->mEntries[0].mUnk_10) {
        mUnk_a8[0] += 0.025f;
        mUnk_a8[1] -= 0.025f;
        if (mUnk_a8[0] >= 1.0f) {
            mUnk_a8[0] = 1.0f;
            mUnk_a8[1] = 0;
            mUnk_26 = 1;
        }
    } else {
        mUnk_a8[0] -= 0.025f;
        mUnk_a8[1] += 0.025f;
        if (mUnk_a8[1] >= 1.0f) {
            mUnk_a8[0] = 0;
            mUnk_a8[1] = 1.0f;
            mUnk_27 = 1;
        }
    }
    return mUnk_26 == 1 && mUnk_27 == 1;
}

void BattleHud::func_ov007_02227128() {
    mUnk_22_2 = 0;
    mUnk_22_1 = 0;
    mUnk_22_0 = 0;
    mAnims[22]->func_02029f6c();
}

BOOL BattleHud::func_ov007_02227158() {
    switch (mAnims[22]->mUnk_0f) {
        case 5:
            if (!mUnk_22_0) {
                mUnk_22_0 = 1;
                SoundManager::Get().func_020246f8(0x26);
                func_020247fc(&SoundManager::Get(), 0x400);
            }
            break;
        case 15:
            if (!mUnk_22_1) {
                mUnk_22_1 = 1;
                SoundManager::Get().func_020246f8(0x26);
                func_020247fc(&SoundManager::Get(), 0x400);
            }
            break;
        case 25:
            if (!mUnk_22_2) {
                mUnk_22_2 = 1;
                SoundManager::Get().func_020246f8(0x26);
                func_020247fc(&SoundManager::Get(), 0x400);
            }
            break;
    }
    if (mAnims[22]->mUnk_12 == 0) {
        mAnims[22]->Update();
        mAnims[22]->Draw(0, 0x80, 0x60, 0);
        return 0;
    }
    if (mUnk_128) {
        mUnk_117_0 = 1;
    }
    return 1;
}

BOOL BattleHud::func_ov007_022273b4() {
    if (mUnk_0c != 2 && mUnk_0c != 3) {
        mUnk_18 -= 10;
    }
    if (mUnk_0c != 2 && mUnk_0c != 4) {
        mUnk_1a += 10;
    }
    mUnk_10 += 10;
    if (mUnk_10 >= 0x48) {
        mUnk_10 = 0;
        if (mUnk_0c < 2) {
            mUnk_22_6 = 0;
        }
        mUnk_22_7 = 0;
        return 1;
    }
    return 0;
}

void BattleHud::func_ov007_02227460(int side) {
    mAnims[5]->SetAnimation(0x13 + mData->mEntries[side].mUnk_0d_4);
}

void BattleHud::func_ov007_02227490(int mode, s32 value, s32 count, bool boost, bool useLevel) {
    s32 delta = value * count;
    if (boost) {
        delta = FX_Mul(delta, 0x1800);
    }
    if (useLevel) {
        u8 pos;
        if (mode == 0) {
            pos = mUnk_13;
        } else {
            pos = 100 - mUnk_13;
        }
        u8 idx = pos / 10;
        if (idx >= 5) {
            idx = 9 - idx;
        }
        delta = FX_Mul(delta, data_ov007_0227918c[idx]);
    }
    mUnk_28[mode] = delta;
    if (mode == 0) {
        mUnk_14 += delta;
        if (mUnk_14 >= 0x64000) {
            mUnk_14 = 0x63000;
        }
        mUnk_13 = mUnk_14 >> 12;
    } else if (mode == 1) {
        if (mUnk_14 <= delta) {
            mUnk_14 = 0x1000;
        } else {
            mUnk_14 -= delta;
            if (mUnk_14 < 0x1000) {
                mUnk_14 = 0x1000;
            }
        }
        mUnk_13 = mUnk_14 >> 12;
    }
}

u8 BattleHud::func_ov007_022275a0(u16 value, u8 a, u8 b) {
    u32 scaled = value * b;
    u8 result = scaled / 100;
    if (result == 0) {
        result = a;
    }
    return result;
}

BOOL BattleHud::func_ov007_022275c0(u8 side, s16 x, s16 y, u8 flag) {
    BOOL result = 0;
    s16 ty;
    u16 angle;
    int motion;
    u8 i;
    if (!mUnk_22_3) {
        if (side == 0) {
            ty = (s16) (s32) (32.0f + 192.0f * mUnk_a8[0]);
        } else {
            ty = (s16) (s32) (32.0f + 192.0f * (1.0f - mUnk_a8[1]) - 20.0f);
        }
    } else {
        s16 base = mUnk_13 * 0xc0 / 100;
        if (side == 0) {
            ty = base + 0x1e;
        } else {
            ty = base + 0x32;
        }
    }
    angle = func_ov007_0221d348(0xffff);
    motion = 0;
    if (flag == 5) {
        motion = 1;
    } else if (flag == 7) {
        motion = 2;
    }
    i = 0;
    do {
        if (mObjs[i]->Start(side, x, y, ty, 0x42, angle, motion)) {
            result = 1;
            break;
        }
        i++;
    } while (i < 15);
    return result;
}

void BattleHud::func_ov007_02227bbc(u8 index) {
    u8 j = 3;
    u8 i = 0;
    do {
        if (i == index) {
            mAnims[j]->func_02029f6c();
            mAnims[j]->SetAnimation(12 - i);
            mUnk_122[i] = 1;
        } else if (mUnk_122[i] == 0) {
            mAnims[j]->func_02029f6c();
            mAnims[j]->SetAnimation(8 - i);
        }
        j++;
        i++;
    } while (i < 2);
}

void BattleHud::func_ov007_022284a0(int side) {
    if (side == 0) {
        mUnk_11a = 1;
    } else {
        mUnk_11b = 1;
    }
    mUnk_120 = 20;
}

void BattleHud::func_ov007_022284c0(int side) {
    if (side == 0) {
        mUnk_11a = 2;
    } else {
        mUnk_11b = 2;
    }
    mUnk_120 = 20;
}

void BattleHud::func_ov007_022284e0(int side) {
    if (side == 0) {
        mUnk_11a = 2;
        mUnk_11b = 3;
    } else {
        mUnk_11a = 3;
        mUnk_11b = 2;
    }
    mUnk_120 = 20;
}

BOOL BattleHud::func_ov007_02228514() {
    return mUnk_11a == 0 && mUnk_11b == 0;
}

void BattleHud::func_ov007_02228530(int card) {
    mUnk_128 = 1;
    mUnk_126 = 0xb4;
    mUnk_124 = 0x14;
    SpriteScreen &screen = SpriteManager::Get().Screen(1);
    u32 id = func_02065c20(card);
    ChangeDir("/Game/Card/Graphic/Large");
    char path[0x80];
    sprintf(path, "card_l_%03d.nce_ncg1D.bin", id);
    screen.LoadCharacters(path, 0x17000, 0);
    sprintf(path, "card_l_%03d_ncl.bin", id);
    screen.func_02027fec(path, 0xa00, 0);
    sprintf(path, "card_l_%03d_nce.bin", id);
    mAnims[19] = new CellAnimation(path, 0, 1, 1);
    mAnims[19]->mCharBase = screen.func_020276f4(0x17000);
    mAnims[19]->mUnk_11 = 5;
    mAnims[19]->SetAnimation(0);
    ChangeDir("/");
}

void BattleHud::func_ov007_022286c8() {
    mUnk_129 = 1;
    mUnk_124 = 0x14;
}

// non-matching: i and j swap registers (r8/r7).
void BattleHud::func_ov007_02227730() {
    u8 selected = data_020d433c.mBattlePlayers->mUnk_2a9;
    mData->func_ov007_0223db20(0, 8, 0x1e);
    mData->func_ov007_0223db20(1, 0xb8, 0x1e);
    mData->mUnk_70->func_020654a0();
    if (mUnk_22_6) {
        s16 pos[2];
        pos[0] = mUnk_18;
        pos[1] = mUnk_1a;
        u8 i = 0;
        do {
                        if (selected == mChoice[i][0] || selected == mChoice[i][1]) {
                if (mUnk_114 != 0) {
                    pos[i] += 4;
                } else {
                    pos[i] -= 4;
                }
                mUnk_115++;
                if (mUnk_115 >= 2) {
                    mUnk_115 = 0;
                    mUnk_114 ^= 1;
                }
            }
            i++;
        } while (i < 2);
        mData->func_ov007_0223de88(0, pos[0], 0x5a, 2, (u32) mUnk_b0[0]);
        mData->func_ov007_0223de88(1, pos[1], 0x5a, 2, (u32) mUnk_b0[1]);
        if (mUnk_22_7) {
            func_ov007_022273b4();
        }
    }
    mAnims[5]->Draw(1, 0x80, 0x22, 1);
    mAnims[6]->Draw(1, (s32) mUnk_108, 0x54, 1);
    mAnims[7]->Draw(1, (s32) mUnk_10c, 0x54, 1);
    if (mUnk_22_3) {
        u8 j = 12;
        u8 i = 0;
        do {
            int x = i == 0 ? 0xb : 0xe7;
            s8 first = mChoice[i][0];
            if (first != -1) {
                mAnims[j]->SetAnimation(first + 7);
                mAnims[j++]->Draw(1, x, 0x3d, 0);
                s8 second = mChoice[i][1];
                if (second != -1) {
                    mAnims[j]->SetAnimation(second + 7);
                    mAnims[j]->Draw(1, x, 0x4b, 0);
                }
            }
            i++;
        } while (i < 2);
    }
    func_ov007_02227c54();
    if (mUnk_22_4) {
        if (!mUnk_23_2) {
            if (mAnims[9]->mUnk_0e == 0 && mAnims[9]->mUnk_12 != 0) {
                mAnims[9]->func_02029f6c();
                mAnims[9]->SetAnimation(1);
            }
        } else if (mAnims[9]->mUnk_0e != 2) {
            if (mUnk_20 != 0) {
                mUnk_20--;
            } else {
                mAnims[9]->func_02029f6c();
                mAnims[9]->SetAnimation(2);
            }
        }
        if (mAnims[9]->mUnk_0e == 1 && mUnk_12c >= 0) {
            if (mUnk_0c == 2) {
                mUnk_24++;
                if (mUnk_24 >= 0x28) {
                    mUnk_24 = 0;
                    if (mUnk_12c == 0x26) {
                        mUnk_12c = 0xd2;
                    } else {
                        mUnk_12c = 0x26;
                    }
                }
            }
            mAnims[21]->Update();
            mAnims[21]->Draw(1, mUnk_12c, mUnk_12e, 0);
        }
        mAnims[9]->Update();
        mAnims[9]->Draw(1, 0x80, 0x6e, 0);
        if (mAnims[9]->mUnk_0e == 1) {
            if (mUnk_0c == 2 || mUnk_0c == 3) {
                mAnims[10]->Update();
                mAnims[10]->Draw(1, 0x48, 0x82, 0);
            }
            if (mUnk_0c == 2 || mUnk_0c == 4) {
                mAnims[11]->Update();
                mAnims[11]->Draw(1, 0x98, 0x82, 0);
            }
        }
    }
    if (mUnk_22_5) {
        mAnims[8]->Update();
        mAnims[8]->Draw(1, 0x80, 0x6e, 0);
    }
    func_ov007_022286e0();
    mData->mUnk_6c->func_020658d0();
}

// non-matching: the original keeps the card x position in r5 and does not fold x + 3.
void BattleHud::func_ov007_022286e0() {
    BattleEntry *entries = mData->mEntries;
    if (mUnk_11a != 0 || mUnk_11b != 0) {
        mUnk_120 -= 1;
        if (mUnk_120 < 4) {
            mUnk_120 = 4;
        }
    }
    switch (mUnk_11a) {
        case 1:
            mUnk_11c[0] += mUnk_120;
            if (mUnk_11c[0] >= 0x80) {
                mUnk_11c[0] = 0x80;
                mUnk_11a = 0;
            }
            break;
        case 2:
            mUnk_11c[0] -= mUnk_120;
            if (mUnk_11c[0] <= -32) {
                mUnk_11c[0] = -32;
                mUnk_11a = 0;
            }
            break;
        case 3:
            mUnk_11c[0] += mUnk_120;
            if (mUnk_11c[0] >= 0x120) {
                mUnk_11c[0] = 0x120;
                mUnk_11a = 0;
            }
            break;
    }
    switch (mUnk_11b) {
        case 1:
            mUnk_11c[1] -= mUnk_120;
            if (mUnk_11c[1] <= 0x80) {
                mUnk_11c[1] = 0x80;
                mUnk_11b = 0;
            }
            break;
        case 2:
            mUnk_11c[1] += mUnk_120;
            if (mUnk_11c[1] >= 0x120) {
                mUnk_11c[1] = 0x120;
                mUnk_11b = 0;
            }
            break;
        case 3:
            mUnk_11c[1] -= mUnk_120;
            if (mUnk_11c[1] <= -32) {
                mUnk_11c[1] = -32;
                mUnk_11b = 0;
            }
            break;
    }
    if (mUnk_128 | mUnk_129) {
        mUnk_124 -= 2;
        if (mUnk_124 < 3) {
            mUnk_124 = 3;
        }
    }
    s16 y = 0x62;
    if (mUnk_129) {
        mUnk_126 += mUnk_124;
        if (mUnk_126 > 0xb4) {
            mUnk_126 = 0xb4;
            mUnk_128 = mUnk_129 = 0;
        }
    } else if (mUnk_128) {
        mUnk_126 -= mUnk_124;
        if (mUnk_126 < 0x5f) {
            mUnk_126 = 0x5f;
            if (mUnk_116) {
                y = mUnk_114 != 0 ? y + 3 : y - 3;
                mUnk_115++;
                if (mUnk_115 >= 1) {
                    mUnk_115 = 0;
                    mUnk_114 ^= 1;
                    mUnk_116--;
                }
            }
        }
    }
    u8 side = 0;
    do {
        if (!entries[side].mUnk_0f_0) {
            if (mUnk_118[side] != 0xff) {
                u8 x = data_ov007_02279178[side];
                mAnims[17 + side]->Draw(1, x, 0x20, mUnk_118[side]);
                if (!data_020d433c.mUnk_9a_2 && mUnk_0c == side) {
                    if (!mUnk_117_1 && !mUnk_117_2 && mUnk_118[side] == 1) {
                        if (!entries[(u8) (side ^ 1)].mUnk_0f_3) {
                            mAnims[16]->Draw(1, x, 0x17, 0);
                        }
                    }
                }
            }
        }
        side++;
    } while (side < 2);
    if (mAnims[15]->mUnk_0e == 5) {
        s8 s = 1;
        do {
            if (mUnk_11c[s] > -32 && mUnk_11c[s] < 0x120) {
                mAnims[14 + s]->Draw(1, mUnk_11c[s], 0x78, 0);
            }
            s--;
        } while (s >= 0);
    } else {
        u8 s = 0;
        do {
            if (mUnk_11c[s] > -32 && mUnk_11c[s] < 0x120) {
                mAnims[14 + s]->Draw(1, mUnk_11c[s], 0x78, 0);
            }
            s++;
        } while (s < 2);
    }
    if (mUnk_128 | mUnk_129) {
        if (mUnk_117_0 && !mUnk_129) {
            mAnims[20]->Update();
            mAnims[20]->Draw(1, 0x82, 0x5a, 0);
        }
        mAnims[19]->func_0202a494(1, y, mUnk_126, 0, 0xb33, 0xb33, 0);
    }
}

// non-matching: register order in the aura loop (original i=r9, j=r8, frame=r7, x=r6).
void BattleHud::func_ov007_02227c54() {
    if (!mUnk_22_3) {
        if (mUnk_26 == 0 && mUnk_a8[0] != 0.0f) {
            s16 x = (s16) (s32) (192.0f * mUnk_a8[0] + 32.0f + 8.0f);
            mAnims[3]->Update();
            mAnims[3]->Draw(1, x, 0x42, 1);
        }
        if (mUnk_27 == 0 && mUnk_a8[1] != 0.0f) {
            s16 x = (s16) (s32) (192.0f * (1.0f - mUnk_a8[1]) + 32.0f - 8.0f);
            mAnims[4]->Update();
            mAnims[4]->Draw(1, x, 0x42, 1);
        }
        u8 n = 0;
        do {
            mObjs[n]->Update();
            n++;
        } while (n < 15);
        u8 i = 0;
        u8 j = 0;
        int frame = 0;
        int x = 0x20;
        do {
            if (mUnk_a8[i] >= 0.01f) {
                float f;
                if (mUnk_a8[i] >= 0.5f) {
                    mAnims[j]->SetAnimation(frame);
                    if (mUnk_a8[i] > 0.0f) {
                        f = 0.5f + 4096.0f * mUnk_a8[i];
                    } else {
                        f = 4096.0f * mUnk_a8[i] - 0.5f;
                    }
                    mAnims[j]->func_0202a494(1, x, 0x40, 3, (s32) f, 0x1000, 0);
                } else {
                    mAnims[j]->SetAnimation(frame + 1);
                    if (mUnk_a8[i] > 0.0f) {
                        f = 0.5f + 4096.0f * mUnk_a8[i];
                    } else {
                        f = 4096.0f * mUnk_a8[i] - 0.5f;
                    }
                    mAnims[j]->func_0202a494(1, x, 0x40, 3, (s32) f << 1, 0x1000, 0);
                }
            }
            j++;
            i++;
            frame = 2;
            x = 0xe0;
        } while (i < 2);
        return;
    }
    if (mUnk_13 > 0x32) {
        func_ov007_02227460(0);
    } else if (mUnk_13 < 0x32) {
        func_ov007_02227460(1);
    } else {
        func_ov007_02227460(mUnk_12);
        mUnk_12 ^= 1;
    }
    if (mUnk_122[0] != 0 || mUnk_122[1] != 0) {
        s16 x = (u16) (mUnk_13 * 0xc0 / 100 + 0x20);
        u8 j = 3;
        u8 i = 0;
        do {
            if (!mUnk_117_3) {
                if (mAnims[j]->mUnk_12 == 0) {
                    mAnims[j]->Update();
                } else {
                    mAnims[j]->func_02029f6c();
                }
            }
            mAnims[j]->Draw(1, x, 0x42, 1);
            i++;
            j++;
        } while (i < 2);
    } else {
        if (!mUnk_117_3) {
            u8 cur = mAnims[2]->mUnk_0e;
            if (mUnk_28[0] != 0 && mUnk_28[1] != 0) {
                u8 want;
                if (mUnk_28[0] > mUnk_28[1]) {
                    want = 5;
                } else if (mUnk_28[0] < mUnk_28[1]) {
                    want = 4;
                } else {
                    want = 6;
                }
                if (cur != want) {
                    mAnims[2]->func_02029f6c();
                    mAnims[2]->SetAnimation(want);
                }
            } else if (mUnk_28[0] == 0 && mUnk_28[1] == 0) {
                if (cur != 6) {
                    if (mUnk_1c[0] == 0 && mUnk_1c[1] == 0) {
                        mAnims[2]->func_02029f6c();
                        mAnims[2]->SetAnimation(6);
                    }
                }
            } else {
                u8 w = 5;
                u8 k = 0;
                do {
                    if (mUnk_28[k] != 0) {
                        if (mUnk_1c[(u8) (k ^ 1)] != 0) {
                            mAnims[2]->func_02029f6c();
                            mAnims[2]->SetAnimation(6);
                        } else {
                            if (cur != w) {
                                mAnims[2]->func_02029f6c();
                                mAnims[2]->SetAnimation(w);
                            }
                            mUnk_1c[k] = mAnims[2]->func_0202a54c();
                        }
                    }
                    k++;
                    w--;
                } while (k < 2);
            }
            u8 m = 0;
            do {
                if (mUnk_1c[m] != 0) {
                    mUnk_1c[m]--;
                }
                m++;
            } while (m < 2);
            mAnims[2]->Update();
        }
        mAnims[2]->Draw(1, (s16) (u16) (mUnk_13 * 0xc0 / 100 + 0x20), 0x42, 1);
    }
    mUnk_28[0] = mUnk_28[1] = 0;
    u8 n = 0;
    do {
        mObjs[n]->Update();
        n++;
    } while (n < 15);
    float p = (float) mUnk_13 / 100.0f;
    float f;
    if (1.0f - p >= 0.5f) {
        mAnims[1]->SetAnimation(2);
        if (1.0f - p > 0.0f) {
            f = 0.5f + 4096.0f * (1.0f - p);
        } else {
            f = 4096.0f * (1.0f - p) - 0.5f;
        }
        mAnims[1]->func_0202a494(1, 0xe0, 0x40, 3, (s32) f, 0x1000, 0);
    } else {
        mAnims[1]->SetAnimation(3);
        if (1.0f - p > 0.0f) {
            f = 0.5f + 4096.0f * (1.0f - p);
        } else {
            f = 4096.0f * (1.0f - p) - 0.5f;
        }
        mAnims[1]->func_0202a494(1, 0xe0, 0x40, 3, (s32) f << 1, 0x1000, 0);
    }
    if (p >= 0.5f) {
        mAnims[0]->SetAnimation(0);
        if (p > 0.0f) {
            f = 0.5f + 4096.0f * p;
        } else {
            f = 4096.0f * p - 0.5f;
        }
        mAnims[0]->func_0202a494(1, 0x20, 0x40, 3, (s32) f, 0x1000, 0);
    } else {
        mAnims[0]->SetAnimation(1);
        if (p > 0.0f) {
            f = 0.5f + 4096.0f * p;
        } else {
            f = 4096.0f * p - 0.5f;
        }
        mAnims[0]->func_0202a494(1, 0x20, 0x40, 3, (s32) f << 1, 0x1000, 0);
    }
}
