#include "System/WirelessManager.hpp"
#include "Battle/ConfrontBg06.hpp"
#include "Battle/BattleUtil.hpp"
#include "Game/UnkStruct_020d433c.hpp"
#include "Graphics/Display.hpp"
#include "System/SoundManager.hpp"
#include <nitro/gx.h>
#include "Graphics/SpriteManager.hpp"
#include "System/TouchManager.hpp"
#include "Battle/ConfrontEmblem06.hpp"
#include "Battle/BattleBase.hpp"
#include "Graphics/SpriteManager.hpp"
#include "System/FileSystem.hpp"
#include "System/TouchManager.hpp"
#include "System/WirelessManager.hpp"
#include <nitro/fx.h>
#include <stdio.h>

ConfrontEmblem06::ConfrontEmblem06(u8 index) {
    char path[0x80];
    mUnk_04   = 0;
    mUnk_05_0 = 0;
    mUnk_05_1 = 0;
    mUnk_05_2 = 0;
    mUnk_05_3 = 0;
    mUnk_05_4 = 0;
    mUnk_06   = 0;
    mUnk_08   = 1.0f;
    mUnk_0c   = 12.0f;
    SpriteScreen &screen = SpriteManager::Get().Screen(1);
    screen.SetObjVramMode(3);
    ChangeDir("/Game/Confrontation/Obj/");
    sprintf(path, "vs_emblem_%02d.nce_ncg1D.bin", index);
    screen.LoadCharacters(path, 0x1c000, 0);
    sprintf(path, "vs_emblem_%02d_ncl.bin", index);
    screen.LoadPalette(path, 0x160, 0x20);
    sprintf(path, "vs_emblem_%02d_nce.bin", index);
    mAnims[0]            = new CellAnimation(path, 0, 1, 1);
    mAnims[0]->mCharBase = screen.GetCharBase(0x1c000);
    mAnims[0]->mPaletteBase   = 11;
    mAnims[0]->SetAnimation(0);
    screen.LoadCharacters("vs_b_obj_06_burst.nce_ncg1D.bin", 0x1d000, 0);
    screen.LoadPalette("vs_b_obj_06_burst_ncl.bin", 0x180, 0x20);
    mAnims[1]            = new CellAnimation("vs_b_obj_06_burst_nce.bin", 0, 1, 1);
    mAnims[1]->mCharBase = screen.GetCharBase(0x1d000);
    mAnims[1]->mPaletteBase   = 12;
    mAnims[1]->SetAnimation(0);
    ChangeDir("/");
    mUnk_10 = 128.0f;
    mUnk_14 = 140.0f;
    func_ov007_02221c68();
}

ConfrontEmblem06::~ConfrontEmblem06() {
    for (u8 i = 0; i < 2; i++) {
        delete mAnims[i];
    }
}

void func_ov007_02225354(float dx, float dy, float *out);

extern "C" void func_02024764(SoundManager *manager, u32 a, u32 b);

extern "C" int func_02024790(SoundManager *manager, u32 a);

void ConfrontEmblem06::func_ov007_02221884() {
    switch (mUnk_04) {
        case 0:
            return;
        case 1:
            if (mUnk_0c) {
                func_ov007_02221d44();
                mUnk_10 = mUnk_10 + mUnk_18 * mUnk_0c;
                mUnk_14 = mUnk_14 + mUnk_1c * mUnk_0c;
                if (mUnk_05_0) {
                    return;
                }
                mUnk_08 = mUnk_08 - 0.1f;
                if (mUnk_08 < 1.0f) {
                    mUnk_08 = 1.0f;
                }
            } else {
                mUnk_08 += 0.015f;
                if (mUnk_08 >= 2.0f) {
                    if (!mUnk_05_4) {
                        SoundManager::Get().StartSeqArc(0x2e);
                    }
                    mAnims[1]->ResetAnimation();
                    mUnk_05_2 = 1;
                    mUnk_04++;
                }
            }
            return;
        case 2:
            if (mAnims[1]->mFinished == 0) {
                mAnims[1]->Update();
                mAnims[1]->Draw(1, mUnk_10, mUnk_14, 0);
                return;
            }
            WirelessManager::Get();
            if (!IsWirelessMode(&data_020c4030.mUnk_228)) {
                func_ov007_02221c68();
            }
            mUnk_05_0 = 0;
            mUnk_08   = 1.0f;
            mUnk_0c   = 12.0f;
            mUnk_04   = 1;
            mUnk_05_3 = 0;
            return;
    }
}

void ConfrontEmblem06::func_ov007_02221ad0() {
    s16 x = (s32) mUnk_10;
    s16 y = (s32) mUnk_14;
    if (mUnk_05_0) {
        if (0.0f == mUnk_0c) {
            float q = 1.3f * mUnk_08;
            if (mUnk_05_1) {
                x = (s32) ((float) x + q);
            } else {
                x = (s32) ((float) x - q);
            }
            mUnk_06++;
            if (mUnk_06 >= 2) {
                mUnk_06   = 0;
                mUnk_05_1 ^= 1;
            }
        }
    }
    float a;
    if (mUnk_08 > 0.0f) {
        a = 0.5f + 4096.0f * mUnk_08;
    } else {
        a = 4096.0f * mUnk_08 - 0.5f;
    }
    float b;
    if (mUnk_08 > 0.0f) {
        b = 0.5f + 4096.0f * mUnk_08;
    } else {
        b = 4096.0f * mUnk_08 - 0.5f;
    }
    mAnims[0]->DrawTransformed(1, x, y, 1, (s32) b, (s32) a, 0);
}

void ConfrontEmblem06::func_ov007_02221c68() {
    u8 quadrant = func_ov007_0221d348(4);
    u16 r       = func_ov007_0221d348(0x1555);
    u16 angle   = ((quadrant & 3) << 14) + 0x1555 + r;
    int i       = (angle >> 4) * 2;
    float p[2];
    float dx = FX_SinCosTable_[i + 1];
    dx /= 4096.0f;
    p[0]     = mUnk_10 + dx;
    float dy = FX_SinCosTable_[i];
    dy /= 4096.0f;
    p[1] = mUnk_14 + dy;
    func_ov007_02225354(p[0] - mUnk_10, p[1] - mUnk_14, &mUnk_18);
}

void ConfrontEmblem06::func_ov007_02221d44() {
    u8 bounced = 0;
    float p[2];
    p[0] = mUnk_10 + mUnk_18 * mUnk_0c;
    p[1] = mUnk_14 + mUnk_1c * mUnk_0c;
    if (p[1] <= 112.0f) {
        mUnk_1c *= -1.0f;
        bounced = 1;
    } else if (p[1] >= 176.0f) {
        mUnk_1c *= -1.0f;
        bounced = 1;
    }
    if (p[0] <= 16.0f) {
        mUnk_18 *= -1.0f;
        bounced = 1;
    } else if (p[0] >= 240.0f) {
        mUnk_18 *= -1.0f;
        bounced = 1;
    }
    if (!bounced) {
        return;
    }
    if (mUnk_05_4) {
        return;
    }
    if (func_02024790(&SoundManager::Get(), 0x2c)) {
        func_02024764(&SoundManager::Get(), 0x2c, 1);
    }
    SoundManager::Get().StartSeqArc(0x2c);
}

int ConfrontEmblem06::func_ov007_02221f2c(int a) {
    if (!TouchManager::Get().mDown || mUnk_04 != 1) {
        return -1;
    }
    if (func_ov007_022252d0((s16) TouchManager::Get().mX, (s16) TouchManager::Get().mY, (s32) mUnk_10, (s32) mUnk_14) > 28) {
        return -1;
    }
    mUnk_05_0 = 1;
    float limit;
    if (a == 0) {
        limit = 0.4f;
    } else {
        limit = 1.0f;
    }
    if (mUnk_0c < limit) {
        if (!mUnk_05_3) {
            mUnk_05_3 = 1;
            if (!mUnk_05_4) {
                SoundManager::Get().StartSeqArc(0x2d);
            }
        }
        mUnk_0c = 0.0f;
    } else {
        mUnk_0c = mUnk_0c - limit;
    }
    return 10;
}

int ConfrontEmblem06::func_ov007_0222210c() {
    int result = -1;
    if (mUnk_05_0) {
        mUnk_05_0 = 0;
        if (0.0f == mUnk_0c) {
            result  = (s16) (s32) (10.0f * mUnk_08);
            float t = 12.0f * mUnk_08;
            t /= 2.0f;
            mUnk_0c = t;
            if (!mUnk_05_4) {
                if (func_02024790(&SoundManager::Get(), 0x2d)) {
                    func_02024764(&SoundManager::Get(), 0x2d, 1);
                }
            }
            mUnk_05_3 = 0;
        }
    }
    return result;
}

void ConfrontEmblem06::func_ov007_02222230() {
    if (mUnk_05_0) {
        return;
    }
    if (mUnk_0c > 2.0f) {
        mUnk_0c = mUnk_0c - 0.02f;
        if (mUnk_0c < 2.0f) {
            mUnk_0c = 2.0f;
        }
    } else if (mUnk_0c < 2.0f) {
        mUnk_0c += 0.02f;
        if (mUnk_0c > 2.0f) {
            mUnk_0c = 2.0f;
        }
    }
}

u8 ConfrontEmblem06::func_ov007_022222b0() {
    return mUnk_04 == 2 ? 1 : 0;
}

u8 ConfrontEmblem06::func_ov007_022222c8() {
    if (mUnk_05_2) {
        mUnk_05_2 = 0;
        return 1;
    }
    return 0;
}

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
    GXS_SetGraphicsMode(0);
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
                SoundManager::Get().StartSeqArc(0x2f);
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

void ConfrontBg06::vfunc_44(int index, u16 count) {
    BattleData *data       = mData;
    BattlePlayerList *list = data_020d433c.mBattlePlayers;
    u8 current             = list->mUnk_2a9;
    BattleEntry *entries   = data->mEntries;
    if ((s32) count < 100) {
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
