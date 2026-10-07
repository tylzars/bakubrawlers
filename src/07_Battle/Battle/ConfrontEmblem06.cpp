#include "Battle/ConfrontEmblem06.hpp"

#include "Battle/BattleUtil.hpp"
#include "Battle/BattleBase.hpp"
#include "Graphics/SpriteManager.hpp"
#include "System/FileSystem.hpp"
#include "System/SoundManager.hpp"
#include "System/TouchManager.hpp"
#include "Unknown/UnkStruct_020c3030.hpp"
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
    screen.func_02027624(3);
    ChangeDir("/Game/Confrontation/Obj/");
    sprintf(path, "vs_emblem_%02d.nce_ncg1D.bin", index);
    screen.LoadCharacters(path, 0x1c000, 0);
    sprintf(path, "vs_emblem_%02d_ncl.bin", index);
    screen.LoadPalette(path, 0x160, 0x20);
    sprintf(path, "vs_emblem_%02d_nce.bin", index);
    mAnims[0]            = new CellAnimation(path, 0, 1, 1);
    mAnims[0]->mCharBase = screen.func_020276f4(0x1c000);
    mAnims[0]->mUnk_11   = 11;
    mAnims[0]->SetAnimation(0);
    screen.LoadCharacters("vs_b_obj_06_burst.nce_ncg1D.bin", 0x1d000, 0);
    screen.LoadPalette("vs_b_obj_06_burst_ncl.bin", 0x180, 0x20);
    mAnims[1]            = new CellAnimation("vs_b_obj_06_burst_nce.bin", 0, 1, 1);
    mAnims[1]->mCharBase = screen.func_020276f4(0x1d000);
    mAnims[1]->mUnk_11   = 12;
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
                        SoundManager::Get().func_020246f8(0x2e);
                    }
                    mAnims[1]->func_02029f6c();
                    mUnk_05_2 = 1;
                    mUnk_04++;
                }
            }
            return;
        case 2:
            if (mAnims[1]->mUnk_12 == 0) {
                mAnims[1]->Update();
                mAnims[1]->Draw(1, (s32) mUnk_10, (s32) mUnk_14, 0);
                return;
            }
            UnkStruct_020c3030::Get();
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
    SoundManager::Get().func_020246f8(0x2c);
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
    mAnims[0]->func_0202a494(1, x, y, 1, (s32) b, (s32) a, 0);
}

int ConfrontEmblem06::func_ov007_02221f2c(int a) {
    if (!TouchManager::Get().mUnk_56 || mUnk_04 != 1) {
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
                SoundManager::Get().func_020246f8(0x2d);
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
