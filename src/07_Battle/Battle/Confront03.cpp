#include "Battle/Confront03.hpp"

#include "Battle/BattleUtil.hpp"
#include "Graphics/Display.hpp"
#include "Graphics/SpriteManager.hpp"
#include "System/FileSystem.hpp"
#include "System/SoundManager.hpp"
#include "System/TouchManager.hpp"
#include <nitro/gx.h>
#include <stdio.h>

extern ConfrontRow data_ov007_02278c0c[];

Confront03::Confront03()
    : BattleBase() {
    mUnk_68 = 0;
    mUnk_6c = 0;
    mUnk_6d = 0;
    mUnk_6e = 1;
    mUnk_74 = 0;
    mData   = 0;
    mUnk_70 = data_ov007_02278c0c;
    for (u8 i = 0; i < 4; i++) {
        mEmblems[i] = 0;
    }
}

Confront03::~Confront03() {
    for (u8 i = 0; i < 4; i++) {
        delete mEmblems[i];
    }
}

void Confront03::func_ov007_02219888(BattleData *data, u32 mode) {
    mData = data;
    func_ov007_0221a624();
    vfunc_28();
    u8 side = mUnk_12;
    BOOL spectator = side >= 2;
    if ((u8) spectator) {
        return;
    }
    u8 nibble = side == 0 ? mData->mEntries[0].mUnk_0d_0 : mData->mEntries[1].mUnk_0d_0;
    for (u8 i = 0; i < 4; i++) {
        mEmblems[i] = new ConfrontEmblem03(nibble);
    }
}

void Confront03::func_ov007_0221a624() {
    func_02001ed0(0);
    G2S_SetBG0Control(0, 1, 0, 1, 0);
    G2S_SetBG1Control(0, 1, 2, 4, 0);
    G2S_SetBG2Control(0, 0, 4, 5, 0);
    G2S_SetBG3Control(0, 1, 6, 6, 0);
    G2S_SetBG0Priority(3);
    G2S_SetBG1Priority(3);
    G2S_SetBG2Priority(3);
    G2S_SetBG3Priority(3);
    G2S_BlendNone();
}

void Confront03::vfunc_28() {
    BOOL spectator = IsSpectator();
    if ((u8) spectator) {
        return;
    }
    BattleData *data = mData;
    SpriteScreen &screen = SpriteManager::Get().Screen(1);
    screen.func_02027624(3);
    ChangeDir("/Game/Confrontation/Obj/");
    char path[0x80];
    u8 nibble = mUnk_12 == 0 ? data->mEntries[0].mUnk_0d_0 : data->mEntries[1].mUnk_0d_0;
    sprintf(path, "vs_emblem_%02d.nce_ncg1D.bin", nibble);
    screen.LoadCharacters(path, 0x1c000, 0);
    sprintf(path, "vs_emblem_%02d_ncl.bin", nibble);
    screen.LoadPalette(path, 0x160, 0x40);
    ChangeDir("/");
}

BOOL Confront03::func_ov007_0221bd84() {
    return FALSE;
}

void Confront03::vfunc_44(int index, u16 count) {
}

BOOL Confront03::func_ov007_0221fe4c() {
    BOOL result = FALSE;
    if (mUnk_20->mUnk_02 - mUnk_16 >= 0x28) {
        if (mUnk_74 != 0) {
            mUnk_74--;
        } else {
            mUnk_74 = func_ov007_0221d348(mUnk_70->mUnk_03 - mUnk_70->mUnk_02);
            mUnk_74 += mUnk_70->mUnk_02;
            result = TRUE;
        }
    }
    return result;
}

void Confront03::func_ov007_0221feb8() {
    u8 found = 0;
    u8 i = 0;
    do {
        int v = mEmblems[i]->func_ov007_0221f70c();
        if (v != -1) {
            mUnk_6e = mUnk_6e & ~(u16) (1 << (u16) v);
        }
        i++;
    } while (i < 4);
    u8 free[12];
    u8 n = 0;
    u16 bit = 1;
    u16 mask = mUnk_6e;
    for (u8 k = 0; k < 12; k++) {
        if (!(mask & bit)) {
            free[n] = k;
            n++;
        }
        bit = bit << 1;
    }
    if (n != 0) {
        u8 j = 0;
        do {
            if (mEmblems[j]->func_ov007_0221f57c()) {
                int pick = free[func_ov007_0221d348(n) & 0xff];
                if (!mUnk_18_1 && (u16) (pick + 0xffff) <= 1) {
                    func_ov007_0221d348(0xffff);
                    func_ov007_0221d348(0xffff);
                } else {
                    mEmblems[j]->func_ov007_0221f1b8(pick, mUnk_6d);
                    mUnk_6e = mUnk_6e | (u16) (1 << pick);
                }
                found = 1;
                break;
            }
            j++;
        } while (j < 4);
    }
    if (found) {
        return;
    }
    func_ov007_0221d348(0xffff);
    func_ov007_0221d348(0xffff);
    func_ov007_0221d348(0xffff);
}

void Confront03::vfunc_34() {
    if (func_ov007_0221fe4c()) {
        u8 count = func_ov007_0221d348(mUnk_70->mUnk_04);
        u8 i = 0;
        do {
            BOOL spectator = IsSpectator();
            if (!(u8) spectator) {
                func_ov007_0221feb8();
            } else {
                func_ov007_0221d348(0xffff);
                func_ov007_0221d348(0xffff);
                func_ov007_0221d348(0xffff);
            }
            i++;
        } while (i < count);
    }
    BOOL spectator = IsSpectator();
    if ((u8) spectator) {
        return;
    }
    u8 j = 0;
    do {
        mEmblems[j]->func_ov007_0221f268();
        j++;
    } while (j < 4);
}

BOOL Confront03::func_ov007_0221bfa4() {
    BOOL spectator = IsSpectator();
    if ((u8) spectator) {
        return FALSE;
    }
    u8 i = 0;
    do {
        mEmblems[i]->func_ov007_0221f368();
        mEmblems[i]->func_ov007_0221f268();
        i++;
    } while (i < 4);
    return FALSE;
}

void Confront03::vfunc_3c() {
    BOOL spectator = IsSpectator();
    if ((u8) spectator) {
        return;
    }
    u8 i = 0;
    do {
        if (!mEmblems[i]->mUnk_07_1) {
            mEmblems[i]->func_ov007_0221f3a4();
        }
        i++;
    } while (i < 4);
    i = 0;
    do {
        if (mEmblems[i]->mUnk_07_1) {
            mEmblems[i]->func_ov007_0221f3a4();
        }
        i++;
    } while (i < 4);
}

u8 Confront03::vfunc_40(int a) {
    ConfrontEmblem03 *emblem = mEmblems[mUnk_6c];
    if (emblem->mUnk_04 == 0 && emblem->mUnk_0a == 1) {
        mUnk_68++;
        if (mUnk_68 >= 0x14) {
            mUnk_68 = 0;
            func_ov007_0221bb28(-1, -1);
            mUnk_6c++;
            if (mUnk_6c >= 4) {
                mUnk_6c = 0;
            }
        } else {
            ConfrontEmblem03 *cur = mEmblems[mUnk_6c];
            s16 y = (s16) cur->mUnk_18;
            s16 x = (s16) cur->mUnk_14;
            func_ov007_0221bb28(x, y);
        }
    }
    mUnk_6d = a;
    if (TouchManager::Get().mUnk_57 == 0) {
        return 0;
    }
    int sound = -1;
    int result;
    u8 i = 0;
    do {
        result = mEmblems[i]->func_ov007_0221f594();
        if (result != -1) {
            func_ov007_0221bb3c();
            sound = result == 100 ? 0x28 : 0x29;
            break;
        }
        i++;
    } while (i < 4);
    if (result != -1) {
        SoundManager::Get().func_020246f8(sound);
    }
    if (result == -1) {
        return 0;
    }
    return result;
}
