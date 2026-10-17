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
    mState   = 0;
    mTouchHeld = 0;
    mShakeRight = 0;
    mBurstStarted = 0;
    mStopped = 0;
    mMuted = 0;
    mShakeFrames   = 0;
    mScale   = 1.0f;
    mSpeed   = 12.0f;
    SpriteScreen &screen = SpriteManager::Get().Screen(1);
    screen.SetObjVramMode(3);
    ChangeDir("/Game/Confrontation/Obj/");
    sprintf(path, "vs_emblem_%02d.nce_ncg1D.bin", index);
    screen.LoadCharacters(path, 0x1c000, 0);
    sprintf(path, "vs_emblem_%02d_ncl.bin", index);
    screen.LoadPalette(path, 0x160, 0x20);
    sprintf(path, "vs_emblem_%02d_nce.bin", index);
    mCellAnims[0]            = new CellAnimation(path, 0, 1, 1);
    mCellAnims[0]->mCharBase = screen.GetCharBase(0x1c000);
    mCellAnims[0]->mPaletteBase   = 11;
    mCellAnims[0]->SetAnimation(0);
    screen.LoadCharacters("vs_b_obj_06_burst.nce_ncg1D.bin", 0x1d000, 0);
    screen.LoadPalette("vs_b_obj_06_burst_ncl.bin", 0x180, 0x20);
    mCellAnims[1]            = new CellAnimation("vs_b_obj_06_burst_nce.bin", 0, 1, 1);
    mCellAnims[1]->mCharBase = screen.GetCharBase(0x1d000);
    mCellAnims[1]->mPaletteBase   = 12;
    mCellAnims[1]->SetAnimation(0);
    ChangeDir("/");
    mX = 128.0f;
    mY = 140.0f;
    PickDir();
}

ConfrontEmblem06::~ConfrontEmblem06() {
    for (u8 i = 0; i < 2; i++) {
        delete mCellAnims[i];
    }
}

void NormalizeXY(float dx, float dy, float *out);

extern "C" void StopSeqArc(SoundManager *manager, u32 a, u32 b);

extern "C" int CountSeqArcSounds(SoundManager *manager, u32 a);

void ConfrontEmblem06::Update() {
    switch (mState) {
        case 0:
            return;
        case 1:
            if (mSpeed) {
                Bounce();
                mX = mX + mDirX * mSpeed;
                mY = mY + mDirY * mSpeed;
                if (mTouchHeld) {
                    return;
                }
                mScale = mScale - 0.1f;
                if (mScale < 1.0f) {
                    mScale = 1.0f;
                }
            } else {
                mScale += 0.015f;
                if (mScale >= 2.0f) {
                    if (!mMuted) {
                        SoundManager::Get().StartSeqArc(0x2e);
                    }
                    mCellAnims[1]->ResetAnimation();
                    mBurstStarted = 1;
                    mState++;
                }
            }
            return;
        case 2:
            if (mCellAnims[1]->mFinished == 0) {
                mCellAnims[1]->Update();
                mCellAnims[1]->Draw(1, mX, mY, 0);
                return;
            }
            WirelessManager::Get();
            if (!IsWirelessMode(&data_020c4030.mUnk_228)) {
                PickDir();
            }
            mTouchHeld = 0;
            mScale   = 1.0f;
            mSpeed   = 12.0f;
            mState   = 1;
            mStopped = 0;
            return;
    }
}

void ConfrontEmblem06::Draw() {
    s16 x = (s32) mX;
    s16 y = (s32) mY;
    if (mTouchHeld) {
        if (0.0f == mSpeed) {
            float q = 1.3f * mScale;
            if (mShakeRight) {
                x = (s32) ((float) x + q);
            } else {
                x = (s32) ((float) x - q);
            }
            mShakeFrames++;
            if (mShakeFrames >= 2) {
                mShakeFrames   = 0;
                mShakeRight ^= 1;
            }
        }
    }
    float a;
    if (mScale > 0.0f) {
        a = 0.5f + 4096.0f * mScale;
    } else {
        a = 4096.0f * mScale - 0.5f;
    }
    float b;
    if (mScale > 0.0f) {
        b = 0.5f + 4096.0f * mScale;
    } else {
        b = 4096.0f * mScale - 0.5f;
    }
    mCellAnims[0]->DrawTransformed(1, x, y, 1, (s32) b, (s32) a, 0);
}

void ConfrontEmblem06::PickDir() {
    u8 quadrant = BattleRand32(4);
    u16 r       = BattleRand32(0x1555);
    u16 angle   = ((quadrant & 3) << 14) + 0x1555 + r;
    int i       = (angle >> 4) * 2;
    float p[2];
    float dx = FX_SinCosTable_[i + 1];
    dx /= 4096.0f;
    p[0]     = mX + dx;
    float dy = FX_SinCosTable_[i];
    dy /= 4096.0f;
    p[1] = mY + dy;
    NormalizeXY(p[0] - mX, p[1] - mY, &mDirX);
}

void ConfrontEmblem06::Bounce() {
    u8 bounced = 0;
    float p[2];
    p[0] = mX + mDirX * mSpeed;
    p[1] = mY + mDirY * mSpeed;
    if (p[1] <= 112.0f) {
        mDirY *= -1.0f;
        bounced = 1;
    } else if (p[1] >= 176.0f) {
        mDirY *= -1.0f;
        bounced = 1;
    }
    if (p[0] <= 16.0f) {
        mDirX *= -1.0f;
        bounced = 1;
    } else if (p[0] >= 240.0f) {
        mDirX *= -1.0f;
        bounced = 1;
    }
    if (!bounced) {
        return;
    }
    if (mMuted) {
        return;
    }
    if (CountSeqArcSounds(&SoundManager::Get(), 0x2c)) {
        StopSeqArc(&SoundManager::Get(), 0x2c, 1);
    }
    SoundManager::Get().StartSeqArc(0x2c);
}

int ConfrontEmblem06::func_ov007_02221f2c(int a) {
    if (!TouchManager::Get().mDown || mState != 1) {
        return -1;
    }
    if (PointDistance((s16) TouchManager::Get().mX, (s16) TouchManager::Get().mY, (s32) mX, (s32) mY) > 28) {
        return -1;
    }
    mTouchHeld = 1;
    float limit;
    if (a == 0) {
        limit = 0.4f;
    } else {
        limit = 1.0f;
    }
    if (mSpeed < limit) {
        if (!mStopped) {
            mStopped = 1;
            if (!mMuted) {
                SoundManager::Get().StartSeqArc(0x2d);
            }
        }
        mSpeed = 0.0f;
    } else {
        mSpeed = mSpeed - limit;
    }
    return 10;
}

int ConfrontEmblem06::func_ov007_0222210c() {
    int result = -1;
    if (mTouchHeld) {
        mTouchHeld = 0;
        if (0.0f == mSpeed) {
            result  = (s16) (s32) (10.0f * mScale);
            float t = 12.0f * mScale;
            t /= 2.0f;
            mSpeed = t;
            if (!mMuted) {
                if (CountSeqArcSounds(&SoundManager::Get(), 0x2d)) {
                    StopSeqArc(&SoundManager::Get(), 0x2d, 1);
                }
            }
            mStopped = 0;
        }
    }
    return result;
}

void ConfrontEmblem06::EaseSpeed() {
    if (mTouchHeld) {
        return;
    }
    if (mSpeed > 2.0f) {
        mSpeed = mSpeed - 0.02f;
        if (mSpeed < 2.0f) {
            mSpeed = 2.0f;
        }
    } else if (mSpeed < 2.0f) {
        mSpeed += 0.02f;
        if (mSpeed > 2.0f) {
            mSpeed = 2.0f;
        }
    }
}

u8 ConfrontEmblem06::IsBursting() {
    return mState == 2 ? 1 : 0;
}

u8 ConfrontEmblem06::TakeBurstStarted() {
    if (mBurstStarted) {
        mBurstStarted = 0;
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

void ConfrontBg06::Init(BattleData *data, u32 mode) {
    mData = data;
    InitSubScreen();
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
        BattleRand32(0xffff);
        BattleRand32(0xffff);
    }
}

void ConfrontBg06::InitSubScreen() {
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
        mEmblem->mState = 1;
    }
    return FALSE;
}

BOOL ConfrontBg06::func_ov007_0221bfa4() {
    BOOL spectator = mUnk_12 >= 2;
    if ((u8) spectator) {
        return FALSE;
    }
    mEmblem->mMuted = 1;
    mEmblem->EaseSpeed();
    mEmblem->Update();
    return FALSE;
}

void ConfrontBg06::vfunc_34() {
    BOOL spectator = mUnk_12 >= 2;
    if ((u8) spectator) {
        return;
    }
    mEmblem->Update();
}

void ConfrontBg06::vfunc_3c() {
    BOOL spectator = mUnk_12 >= 2;
    if ((u8) spectator) {
        return;
    }
    mEmblem->Draw();
}

u8 ConfrontBg06::UpdateTouch(int a) {
    ConfrontEmblem06 *emblem = mEmblem;
    s32 hit                  = -1;
    s16 y                    = (s32) emblem->mY;
    s16 x                    = (s32) emblem->mX;
    SetPenPosition(x, y);
    if (mEmblem->IsBursting()) {
        if (mEmblem->TakeBurstStarted()) {
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
        mEmblem->EaseSpeed();
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
        mEmblem->PickDir();
    } else {
        BattleRand32(0xffff);
        BattleRand32(0xffff);
    }
}

void ConfrontBg06::LoadGraphics() {
}
