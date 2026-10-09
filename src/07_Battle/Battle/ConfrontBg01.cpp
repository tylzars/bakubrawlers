#include "Battle/ConfrontBg01.hpp"

#include "Battle/BattleUtil.hpp"
#include "Graphics/Display.hpp"
#include "System/FileSystem.hpp"
#include "System/TouchManager.hpp"
#include "System/SoundManager.hpp"
#include <nitro/gx.h>
#include <stdio.h>

ConfrontBg01::ConfrontBg01()
    : BattleBase() {
    mUnk_68 = 0;
    mUnk_69 = 0;
    mUnk_6a = 0;
    mUnk_6c = 0;
    mData   = 0;
    mBlend  = 0;
    mBlend  = new BattleBlend(1, 5, 0x28);
}

ConfrontBg01::~ConfrontBg01() {
    delete mBlend;
}

void ConfrontBg01::func_ov007_02219888(BattleData *data, u32 mode) {
    mData = data;
    func_ov007_0221a624();
    vfunc_28();
}

void ConfrontBg01::func_ov007_0221a624() {
    GXS_SetGraphicsMode(0);
    G2S_SetBG0Control(0, 1, 0, 1, 0);
    G2S_SetBG1Control(0, 1, 2, 4, 0);
    G2S_SetBG2Control(0, 0, 4, 5, 0);
    G2S_SetBG3Control(0, 1, 6, 6, 0);
    G2S_SetBG0Priority(3);
    G2S_SetBG1Priority(2);
    G2S_SetBG2Priority(3);
    G2S_SetBG3Priority(1);
    G2S_SetBlendAlpha(8, 2, 1, 0);
    mBlend->func_ov007_02223ee4();
}

void ConfrontBg01::vfunc_28() {
    u8 spectator = IsSpectator();
    if (spectator) {
        return;
    }
    char path[0x80];
    const char *dir = "/Game/Confrontation/Bg/";
    BattleEntry *entries = mData->mEntries;
    sprintf(data_020d5488, dir);
    func_0200acac(dir);
    u8 n = entries[mUnk_12].mUnk_0d_0;
    sprintf(path, "vs_b_bg_01_%02d_ncl.bin", n);
    func_02025b64(1, 1, 0x2000, path, 0, 0, 0);
    func_02025b64(1, 12, 0, "vs_b_bg_01_06_ncg.bin", 0, 0, 0);
    func_02025b64(1, 8, 0, "vs_b_bg_01_06_nsc.bin", 0, 0, 0);
    sprintf(path, "vs_b_bg_01_%02d_ncl.bin", n);
    func_02025b64(1, 1, 0x6000, path, 0, 0, 0);
    sprintf(path, "vs_b_bg_01_%02d_ncg.bin", n);
    func_02025b64(1, 14, 0, path, 0, 0, 0);
    sprintf(path, "vs_b_bg_01_%02d_nsc.bin", n);
    func_02025b64(1, 10, 0, path, 0, 0, 0);
    ChangeDir("/");
}

BOOL ConfrontBg01::func_ov007_0221bfa4() {
    u8 spectator = IsSpectator();
    if (spectator) {
        return FALSE;
    }
    if (mUnk_68 != 0) {
        mUnk_68 = 0;
        func_02024764(&SoundManager::Get(), 0x27, 1);
    }
    mBlend->func_ov007_02223ea0();
    return FALSE;
}

void ConfrontBg01::vfunc_3c() {
    mBlend->func_ov007_02223dc4();
}

u8 ConfrontBg01::vfunc_40(int a) {
    u8 hit = func_ov007_02229ee4(0x50, 0x58, 0x60, 0x60, 5, &mUnk_69);
    if (hit) {
        u32 limit = 0x28;
        if (a) {
            limit = 0x14;
        }
        mUnk_6c += func_ov007_022252d0(TouchManager::Get().mUnk_08, TouchManager::Get().mUnk_0a,
                                       (s16) TouchManager::Get().mX, (s16) TouchManager::Get().mY);
        if (mUnk_6c >= limit) {
            mUnk_6c = 0;
            func_ov007_0221bb3c();
            if (!func_02024790(&SoundManager::Get(), 0x27)) {
                SoundManager::Get().func_020246f8(0x27);
            }
            mUnk_68 = 10;
            if (mUnk_6a < 0x200) {
                mUnk_6a += 2;
                func_020247fc(&SoundManager::Get(), mUnk_6a);
            }
            mBlend->func_ov007_02223d7c();
            hit = mBlend->mLevel * 100 / 40;
        }
    } else {
        if (mUnk_68 != 0) {
            mUnk_68--;
            if (mUnk_68 == 0) {
                func_02024764(&SoundManager::Get(), 0x27, 1);
                mUnk_6a = 0;
            }
        }
        if (mBlend->func_ov007_02223ea0()) {
            mUnk_6c = 0;
        }
    }
    return hit;
}

void ConfrontBg01::vfunc_44(int index, u16 count) {
}

void ConfrontBg01::vfunc_34() {
}

BOOL ConfrontBg01::func_ov007_0221bd84() {
    return FALSE;
}
