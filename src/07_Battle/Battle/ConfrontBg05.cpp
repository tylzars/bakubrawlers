#include "System/TouchManager.hpp"
#include "Battle/ConfrontBg05.hpp"
#include "Battle/BattleUtil.hpp"
#include "Graphics/Display.hpp"
#include "Graphics/SpriteManager.hpp"
#include "System/FileSystem.hpp"
#include "System/SoundManager.hpp"
#include "System/TouchManager.hpp"
#include <nitro/gx.h>
#include <stdio.h>
#include "Battle/ConfrontEmblem05.hpp"
#include <nitro/fx.h>

ConfrontEmblem05::ConfrontEmblem05(u8 emblem) {
    char path[0x80];
    mMirrored = 0;
    mAngle    = 0;
    mSpeed    = 0x50;
    SpriteScreen &sub = SpriteManager::Get().Screen(1);
    sub.SetObjVramMode(3);
    ChangeDir("/Game/Confrontation/Obj/");
    sprintf(path, "vs_emblem_%02d_nce.bin", emblem);
    mCellAnim          = new CellAnimation(path, 0, 1, 1);
    mCellAnim->mCharBase = sub.GetCharBase(0x1c000);
    mCellAnim->mPaletteBase = 0xb;
    mCellAnim->SetAnimation(0);
    ChangeDir("/");
    mCellAnim->ResetAnimation();
    mCellAnim->SetAnimation(0);
    mX = 78.0f + 50.0f * ((float) FX_SinCosTable_[(mAngle >> 4) * 2 + 1] / 4096.0f);
    mY = 136.0f + 32.0f * ((float) FX_SinCosTable_[(mAngle >> 4) * 2] / 4096.0f);
}

ConfrontEmblem05::~ConfrontEmblem05() {
    delete mCellAnim;
}

void ConfrontEmblem05::Update() {
    if (!mMirrored) {
        mX = 78.0f + 50.0f * ((float) FX_SinCosTable_[(mAngle >> 4) * 2 + 1] / 4096.0f);
    } else {
        mX = 178.0f + 50.0f * (0.0f - (float) FX_SinCosTable_[(mAngle >> 4) * 2 + 1] / 4096.0f);
    }
    mY = 136.0f + 32.0f * ((float) FX_SinCosTable_[(mAngle >> 4) * 2] / 4096.0f);
    if (mAngle + mSpeed >= 0x10000) {
        mMirrored ^= 1;
    }
    mAngle += mSpeed;
}

void ConfrontEmblem05::Draw() {
    s32 x = (s32) mX;
    s32 y = (s32) mY;
    mCellAnim->DrawTransformed(1, x, y, 0, 0x1000, 0x1000, 0);
}

int ConfrontEmblem05::func_ov007_02220b9c(int fast) {
    s32 dist = PointDistance((s16) TouchManager::Get().mX, (s16) TouchManager::Get().mY, (s32) mX, (s32) mY);
    if (dist > 0x20) {
        return -1;
    }
    s16 scaled = mSpeed * 100 / 0x950;
    if (fast == 0) {
        mSpeed += 9;
    } else {
        mSpeed += 12;
    }
    if (mSpeed >= 0x950) {
        mSpeed = 0x950;
    }
    return scaled;
}

void ConfrontEmblem05::SlowDown() {
    s32 speed = mSpeed - 0x10;
    if (speed < 0x50) {
        speed = 0x50;
    }
    mSpeed = speed;
}

ConfrontBg05::ConfrontBg05()
    : BattleBase() {
    mSoundTimer = 0;
    mUnk_6a = 0;
    mData   = 0;
    mEmblem = 0;
    mBlend  = new BattleBlend(5, 7, 0x26);
}

ConfrontBg05::~ConfrontBg05() {
    delete mEmblem;
    delete mBlend;
}

void ConfrontBg05::Init(BattleData *data, u32 mode) {
    mData = data;
    InitSubScreen();
    LoadGraphics();
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

void ConfrontBg05::InitSubScreen() {
    GXS_SetGraphicsMode(0);
    G2S_SetBG0Control(0, 1, 0, 1, 0);
    G2S_SetBG1Control(0, 1, 2, 4, 0);
    G2S_SetBG2Control(0, 0, 4, 5, 0);
    G2S_SetBG3Control(0, 1, 6, 6, 0);
    G2S_SetBG0Priority(3);
    G2S_SetBG1Priority(2);
    G2S_SetBG2Priority(3);
    G2S_SetBG3Priority(2);
    G2S_SetBlendAlpha(2, 8, 1, 0);
    mBlend->WriteBlendAlpha();
}

void ConfrontBg05::LoadGraphics() {
    u8 spectator = IsSpectator();
    BattleEntry *entries = mData->mEntries;
    if (spectator) {
        return;
    }
    char path[0x80];
    ChangeDir("/Game/Confrontation/Bg/");
    u8 n = entries[mUnk_12].mUnk_0d_0;
    sprintf(path, "vs_b_bg_05_%02d_ncl.bin", n);
    LoadAndQueueUpload(1, UPLOAD_KIND_BG_EXT_PLTT, 0x2000, path, 0, 0, 0);
    sprintf(path, "vs_b_bg_05_%02d_ncg.bin", n);
    LoadAndQueueUpload(1, UPLOAD_KIND_BG1_CHAR, 0, path, 0, 0, 0);
    sprintf(path, "vs_b_bg_05_%02d_nsc.bin", n);
    LoadAndQueueUpload(1, UPLOAD_KIND_BG1_SCR, 0, path, 0, 0, 0);
    sprintf(path, "vs_b_bg_05_%02d_ncl.bin", n);
    LoadAndQueueUpload(1, UPLOAD_KIND_BG_EXT_PLTT, 0x6000, path, 0, 0, 0);
    LoadAndQueueUpload(1, UPLOAD_KIND_BG3_CHAR, 0, "vs_b_bg_05_06_ncg.bin", 0, 0, 0);
    LoadAndQueueUpload(1, UPLOAD_KIND_BG3_SCR, 0, "vs_b_bg_05_06_nsc.bin", 0, 0, 0);
    ChangeDir("/");
    SpriteScreen &screen = SpriteManager::Get().Screen(1);
    screen.SetObjVramMode(3);
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
    mEmblem->Update();
}

BOOL ConfrontBg05::func_ov007_0221bfa4() {
    u8 spectator = IsSpectator();
    if (spectator) {
        return FALSE;
    }
    if (mSoundTimer != 0) {
        mSoundTimer = 0;
        StopSeqArc(&SoundManager::Get(), 0x2b, 1);
    }
    mEmblem->Update();
    mEmblem->SlowDown();
    mBlend->LowerLevel();
    return FALSE;
}

void ConfrontBg05::vfunc_3c() {
    u8 spectator = IsSpectator();
    if (spectator) {
        return;
    }
    mBlend->UpdateWobble();
    mEmblem->Draw();
}

u8 ConfrontBg05::UpdateTouch(int a) {
    s32 hit = -1;
    s16 y = (s32) mEmblem->mY;
    s16 x = (s32) mEmblem->mX;
    SetPenPosition(x, y);
    if (TouchManager::Get().mDown) {
        hit = mEmblem->func_ov007_02220b9c(a);
        if (hit != -1) {
            func_ov007_0221bb3c();
            if (!CountSeqArcSounds(&SoundManager::Get(), 0x2b)) {
                SoundManager::Get().StartSeqArc(0x2b);
            }
            mSoundTimer = 10;
        }
    }
    if (hit == -1) {
        if (mSoundTimer != 0) {
            mSoundTimer--;
            if (mSoundTimer == 0) {
                StopSeqArc(&SoundManager::Get(), 0x2b, 1);
            }
        }
        mBlend->LowerLevel();
        mEmblem->SlowDown();
        mUnk_6a = 0;
        return 0;
    }
    if (mUnk_6a < 0x200) {
        mUnk_6a += 2;
        func_020247fc(&SoundManager::Get(), mUnk_6a);
    }
    mBlend->RaiseLevel();
    return hit;
}

void ConfrontBg05::vfunc_44(int index, u16 count) {
}

BOOL ConfrontBg05::func_ov007_0221bd84() {
    return FALSE;
}
