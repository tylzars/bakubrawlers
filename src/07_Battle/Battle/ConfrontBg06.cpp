#include "Battle/ConfrontBg06.hpp"

#include "Battle/BattleUtil.hpp"
#include "Game/UnkStruct_020d433c.hpp"
#include "Graphics/Display.hpp"
#include "System/SoundManager.hpp"
#include <nitro/gx.h>

ConfrontBg06::ConfrontBg06()
    : BattleBase() {
    mData   = 0;
    mEmblem = 0;
}

ConfrontBg06::~ConfrontBg06() {
    delete mEmblem;
}

void ConfrontBg06::func_ov007_02219888(BattleData *data, u32 mode) {
    mData = data;
    func_ov007_0221a624();
    u8 side = mUnk_12;
    BOOL spectator = side >= 2;
    if (!(u8) spectator) {
        u8 n;
        if (side == 0) {
            n = mData->mEntries[0].mUnk_0d_0;
        } else {
            n = mData->mEntries[1].mUnk_0d_0;
        }
        mEmblem = new ConfrontEmblem06(n);
    } else {
        func_ov007_0221d348(0xffff);
        func_ov007_0221d348(0xffff);
    }
}

void ConfrontBg06::func_ov007_0221a624() {
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

BOOL ConfrontBg06::func_ov007_0221bd84() {
    BOOL spectator = mUnk_12 >= 2;
    if (!(u8) spectator) {
        mEmblem->mUnk_04 = 1;
    }
    return FALSE;
}

BOOL ConfrontBg06::func_ov007_0221bfa4() {
    BOOL spectator = mUnk_12 >= 2;
    if ((u8) spectator) {
        return FALSE;
    }
    mEmblem->mUnk_05_4 = 1;
    mEmblem->func_ov007_02222230();
    mEmblem->func_ov007_02221884();
    return FALSE;
}

void ConfrontBg06::vfunc_34() {
    BOOL spectator = mUnk_12 >= 2;
    if ((u8) spectator) {
        return;
    }
    mEmblem->func_ov007_02221884();
}

void ConfrontBg06::vfunc_3c() {
    BOOL spectator = mUnk_12 >= 2;
    if ((u8) spectator) {
        return;
    }
    mEmblem->func_ov007_02221ad0();
}

u8 ConfrontBg06::vfunc_40(int a) {
    ConfrontEmblem06 *emblem = mEmblem;
    s32 hit                  = -1;
    s16 y                    = (s32) emblem->mUnk_14;
    s16 x                    = (s32) emblem->mUnk_10;
    func_ov007_0221bb28(x, y);
    if (mEmblem->func_ov007_022222b0()) {
        if (mEmblem->func_ov007_022222c8()) {
            hit = 100;
            func_ov007_0221bb68();
        }
    } else {
        hit = mEmblem->func_ov007_02221f2c(a);
        if (hit != -1) {
            func_ov007_0221bb3c();
        }
        if (hit == -1) {
            hit = mEmblem->func_ov007_0222210c();
            if (hit != -1) {
                func_ov007_0221bb68();
                SoundManager::Get().func_020246f8(0x2f);
            }
        }
    }
    if (hit == -1) {
        mEmblem->func_ov007_02222230();
    }
    if (hit == -1) {
        return 0;
    }
    return hit;
}

void ConfrontBg06::vfunc_44(int index, int count) {
    BattleData *data       = mData;
    BattlePlayerList *list = data_020d433c.mBattlePlayers;
    u8 current             = list->mUnk_2a9;
    BattleEntry *entries   = data->mEntries;
    if (count < 100) {
        return;
    }
    if (current == entries[index].mUnk_0d_4) {
        mEmblem->func_ov007_02221c68();
    } else {
        func_ov007_0221d348(0xffff);
        func_ov007_0221d348(0xffff);
    }
}

void ConfrontBg06::vfunc_28() {
}
