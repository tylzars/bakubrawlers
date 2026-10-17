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
    mSoundTimer = 0;
    mUnk_69 = 0;
    mUnk_6a = 0;
    mDistanceSum = 0;
    mData   = 0;
    mBlend  = 0;
    mBlend  = new BattleBlend(1, 5, 0x28);
}

ConfrontBg01::~ConfrontBg01() {
    delete mBlend;
}

void ConfrontBg01::Init(BattleData *data, u32 mode) {
    mData = data;
    InitSubScreen();
    LoadGraphics();
}

void ConfrontBg01::InitSubScreen() {
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
    mBlend->WriteBlendAlpha();
}

void ConfrontBg01::LoadGraphics() {
    u8 spectator = IsSpectator();
    if (spectator) {
        return;
    }
    char path[0x80];
    const char *dir = "/Game/Confrontation/Bg/";
    BattleEntry *entries = mData->mEntries;
    sprintf(gCurrentDir, dir);
    FS_ChangeDir(dir);
    u8 n = entries[mLocalSide].mAttribute;
    sprintf(path, "vs_b_bg_01_%02d_ncl.bin", n);
    LoadAndQueueUpload(1, UPLOAD_KIND_BG_EXT_PLTT, 0x2000, path, 0, 0, 0);
    LoadAndQueueUpload(1, UPLOAD_KIND_BG1_CHAR, 0, "vs_b_bg_01_06_ncg.bin", 0, 0, 0);
    LoadAndQueueUpload(1, UPLOAD_KIND_BG1_SCR, 0, "vs_b_bg_01_06_nsc.bin", 0, 0, 0);
    sprintf(path, "vs_b_bg_01_%02d_ncl.bin", n);
    LoadAndQueueUpload(1, UPLOAD_KIND_BG_EXT_PLTT, 0x6000, path, 0, 0, 0);
    sprintf(path, "vs_b_bg_01_%02d_ncg.bin", n);
    LoadAndQueueUpload(1, UPLOAD_KIND_BG3_CHAR, 0, path, 0, 0, 0);
    sprintf(path, "vs_b_bg_01_%02d_nsc.bin", n);
    LoadAndQueueUpload(1, UPLOAD_KIND_BG3_SCR, 0, path, 0, 0, 0);
    ChangeDir("/");
}

BOOL ConfrontBg01::func_ov007_0221bfa4() {
    u8 spectator = IsSpectator();
    if (spectator) {
        return FALSE;
    }
    if (mSoundTimer != 0) {
        mSoundTimer = 0;
        StopSeqArc(&SoundManager::Get(), 0x27, 1);
    }
    mBlend->LowerLevel();
    return FALSE;
}

void ConfrontBg01::Draw() {
    mBlend->UpdateWobble();
}

u8 ConfrontBg01::UpdateTouch(int a) {
    u8 hit = func_ov007_02229ee4(0x50, 0x58, 0x60, 0x60, 5, &mUnk_69);
    if (hit) {
        u32 limit = 0x28;
        if (a) {
            limit = 0x14;
        }
        mDistanceSum += PointDistance(TouchManager::Get().mUnk_08, TouchManager::Get().mUnk_0a,
                                       (s16) TouchManager::Get().mX, (s16) TouchManager::Get().mY);
        if (mDistanceSum >= limit) {
            mDistanceSum = 0;
            SetTouched();
            if (!CountSeqArcSounds(&SoundManager::Get(), 0x27)) {
                SoundManager::Get().StartSeqArc(0x27);
            }
            mSoundTimer = 10;
            if (mUnk_6a < 0x200) {
                mUnk_6a += 2;
                func_020247fc(&SoundManager::Get(), mUnk_6a);
            }
            mBlend->RaiseLevel();
            hit = mBlend->mLevel * 100 / 40;
        }
    } else {
        if (mSoundTimer != 0) {
            mSoundTimer--;
            if (mSoundTimer == 0) {
                StopSeqArc(&SoundManager::Get(), 0x27, 1);
                mUnk_6a = 0;
            }
        }
        if (mBlend->LowerLevel()) {
            mDistanceSum = 0;
        }
    }
    return hit;
}

void ConfrontBg01::vfunc_44(int index, u16 count) {
}

void ConfrontBg01::UpdateEmblems() {
}

BOOL ConfrontBg01::func_ov007_0221bd84() {
    return FALSE;
}
