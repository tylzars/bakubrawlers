#include "Battle/ConfrontBg05.hpp"

#include "Battle/BattleUtil.hpp"
#include "Graphics/Display.hpp"
#include "Graphics/SpriteManager.hpp"
#include "System/FileSystem.hpp"
#include "System/SoundManager.hpp"
#include "System/TouchManager.hpp"
#include <nitro/gx.h>
#include <stdio.h>

ConfrontBg05::ConfrontBg05()
    : BattleBase() {
    mUnk_68 = 0;
    mUnk_6a = 0;
    mData   = 0;
    mEmblem = 0;
    mBlend  = new BattleBlend(5, 7, 0x26);
}

ConfrontBg05::~ConfrontBg05() {
    delete mEmblem;
    delete mBlend;
}

void ConfrontBg05::func_ov007_02219888(BattleData *data, u32 mode) {
    mData = data;
    func_ov007_0221a624();
    vfunc_28();
    u8 side = mUnk_12;
    BOOL spectator = side >= 2;
    if ((u8) spectator) {
        return;
    }
    u8 n;
    if (side == 0) {
        n = mData->mEntries[0].mUnk_0d_0;
    } else {
        n = mData->mEntries[1].mUnk_0d_0;
    }
    mEmblem = new ConfrontEmblem05(n);
}

void ConfrontBg05::func_ov007_0221a624() {
    func_02001ed0(0);
    G2S_SetBG0Control(0, 1, 0, 1, 0);
    G2S_SetBG1Control(0, 1, 2, 4, 0);
    G2S_SetBG2Control(0, 0, 4, 5, 0);
    G2S_SetBG3Control(0, 1, 6, 6, 0);
    G2S_SetBG0Priority(3);
    G2S_SetBG1Priority(2);
    G2S_SetBG2Priority(3);
    G2S_SetBG3Priority(2);
    G2S_SetBlendAlpha(2, 8, 1, 0);
    mBlend->func_ov007_02223ee4();
}

void ConfrontBg05::vfunc_28() {
    u8 spectator = IsSpectator();
    BattleEntry *entries = mData->mEntries;
    if (spectator) {
        return;
    }
    char path[0x80];
    ChangeDir("/Game/Confrontation/Bg/");
    u8 n = entries[mUnk_12].mUnk_0d_0;
    sprintf(path, "vs_b_bg_05_%02d_ncl.bin", n);
    func_02025b64(1, 1, 0x2000, path, 0, 0, 0);
    sprintf(path, "vs_b_bg_05_%02d_ncg.bin", n);
    func_02025b64(1, 12, 0, path, 0, 0, 0);
    sprintf(path, "vs_b_bg_05_%02d_nsc.bin", n);
    func_02025b64(1, 8, 0, path, 0, 0, 0);
    sprintf(path, "vs_b_bg_05_%02d_ncl.bin", n);
    func_02025b64(1, 1, 0x6000, path, 0, 0, 0);
    func_02025b64(1, 14, 0, "vs_b_bg_05_06_ncg.bin", 0, 0, 0);
    func_02025b64(1, 10, 0, "vs_b_bg_05_06_nsc.bin", 0, 0, 0);
    ChangeDir("/");
    SpriteScreen &screen = SpriteManager::Get().Screen(1);
    screen.func_02027624(3);
    ChangeDir("/Game/Confrontation/Obj/");
    u8 emblem;
    if (mUnk_12 == 0) {
        emblem = entries[0].mUnk_0d_0;
    } else {
        emblem = entries[1].mUnk_0d_0;
    }
    sprintf(path, "vs_emblem_%02d.nce_ncg1D.bin", emblem);
    screen.LoadCharacters(path, 0x1c000, 0);
    sprintf(path, "vs_emblem_%02d_ncl.bin", emblem);
    screen.LoadPalette(path, 0x160, 0x40);
    ChangeDir("/");
}

void ConfrontBg05::vfunc_34() {
    u8 spectator = IsSpectator();
    if (spectator) {
        return;
    }
    mEmblem->func_ov007_02220a00();
}

BOOL ConfrontBg05::func_ov007_0221bfa4() {
    u8 spectator = IsSpectator();
    if (spectator) {
        return FALSE;
    }
    if (mUnk_68 != 0) {
        mUnk_68 = 0;
        func_02024764(&SoundManager::Get(), 0x2b, 1);
    }
    mEmblem->func_ov007_02220a00();
    mEmblem->func_ov007_02220cb0();
    mBlend->func_ov007_02223ea0();
    return FALSE;
}

void ConfrontBg05::vfunc_3c() {
    u8 spectator = IsSpectator();
    if (spectator) {
        return;
    }
    mBlend->func_ov007_02223dc4();
    mEmblem->func_ov007_02220b48();
}

u8 ConfrontBg05::vfunc_40(int a) {
    s32 hit = -1;
    s16 y = (s32) mEmblem->mY;
    s16 x = (s32) mEmblem->mX;
    func_ov007_0221bb28(x, y);
    if (TouchManager::Get().mUnk_56) {
        hit = mEmblem->func_ov007_02220b9c(a);
        if (hit != -1) {
            func_ov007_0221bb3c();
            if (!func_02024790(&SoundManager::Get(), 0x2b)) {
                SoundManager::Get().func_020246f8(0x2b);
            }
            mUnk_68 = 10;
        }
    }
    if (hit == -1) {
        if (mUnk_68 != 0) {
            mUnk_68--;
            if (mUnk_68 == 0) {
                func_02024764(&SoundManager::Get(), 0x2b, 1);
            }
        }
        mBlend->func_ov007_02223ea0();
        mEmblem->func_ov007_02220cb0();
        mUnk_6a = 0;
        return 0;
    }
    if (mUnk_6a < 0x200) {
        mUnk_6a += 2;
        func_020247fc(&SoundManager::Get(), mUnk_6a);
    }
    mBlend->func_ov007_02223d7c();
    return hit;
}

void ConfrontBg05::vfunc_44(int index, u16 count) {
}

BOOL ConfrontBg05::func_ov007_0221bd84() {
    return FALSE;
}
