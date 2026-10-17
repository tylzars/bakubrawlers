#include "Battle/ConfrontBg04.hpp"

#include "Battle/BattleUtil.hpp"
#include "Graphics/Display.hpp"
#include "System/FileSystem.hpp"
#include "System/FrameUpdate.hpp"
#include "System/SoundManager.hpp"
#include "System/TouchManager.hpp"
#include <nitro/fx.h>
#include <nitro/gx.h>
#include <stdio.h>

ConfrontBg04::ConfrontBg04()
    : BattleBase() {
    mSpeedSum = 0;
    mAngle  = 0;
    mSpeed  = 0;
    mData   = 0;
    mBlend  = new BattleBlend(1, 7, 0x2f);
}

ConfrontBg04::~ConfrontBg04() {
    delete mBlend;
}

void ConfrontBg04::Init(BattleData *data, u32 mode) {
    mData = data;
    InitSubScreen();
    LoadGraphics();
    MtxFx22 mtx;
    MTX_ScaleRot22(&mtx, 0x1000, 0x1000, 0);
    G2x_SetBGyAffine_((vu16 *) 0x4001030, &mtx, 0, 0, 0, 0);
}

void ConfrontBg04::InitSubScreen() {
    GXS_SetGraphicsMode(3);
    G2S_SetBG0Control(0, 1, 0, 1, 0);
    G2S_SetBG1Control(0, 1, 2, 4, 0);
    G2S_SetBG2Control(0, 0, 4, 5, 0);
    G2S_SetBG3Control(1, 0, 6, 6, 0);
    G2S_SetBG0Priority(3);
    G2S_SetBG1Priority(2);
    G2S_SetBG2Priority(3);
    G2S_SetBG3Priority(1);
    G2S_SetBlendAlpha(8, 2, 1, 0);
    mBlend->WriteBlendAlpha();
}

void ConfrontBg04::LoadGraphics() {
    char path[0x80];
    const char *dir = "/Game/Confrontation/Bg/";
    BattleEntry *entries = mData->mEntries;
    sprintf(gCurrentDir, dir);
    FS_ChangeDir(dir);
    u8 side = mLocalSide;
    BOOL spectator = side >= 2;
    if (!(u8) spectator) {
        u8 n = entries[side].mAttribute;
        sprintf(path, "vs_b_bg_04_%02d_ncl.bin", n);
        LoadAndQueueUpload(1, UPLOAD_KIND_BG_EXT_PLTT, 0x2000, path, 0, 0, 0);
        LoadAndQueueUpload(1, UPLOAD_KIND_BG1_CHAR, 0, "vs_b_bg_04_06_ncg.bin", 0, 0, 0);
        LoadAndQueueUpload(1, UPLOAD_KIND_BG1_SCR, 0, "vs_b_bg_04_06_nsc.bin", 0, 0, 0);
        sprintf(path, "vs_b_bg_04_%02d_ncl.bin", n);
        LoadAndQueueUpload(1, UPLOAD_KIND_BG_EXT_PLTT, 0x6000, path, 0, 0, 0);
        sprintf(path, "vs_b_bg_04_%02d_ncg.bin", n);
        LoadAndQueueUpload(1, UPLOAD_KIND_BG3_CHAR, 0, path, 0, 0, 0);
        sprintf(path, "vs_b_bg_04_%02d_nsc.bin", n);
        LoadAndQueueUpload(1, UPLOAD_KIND_BG3_SCR, 0, path, 0, 0, 0);
    }
    ChangeDir("/");
}

BOOL ConfrontBg04::func_ov007_0221bfa4() {
    u8 spectator = IsSpectator();
    if (spectator) {
        return FALSE;
    }
    SlowDown();
    mBlend->LowerLevel();
    return FALSE;
}

void ConfrontBg04::vfunc_3c() {
    mBlend->UpdateWobble();
    if (mSpeed != 0) {
        QueueFrameCallback((FrameCallback) ConfrontBg04Rotate, this);
    }
}

u8 ConfrontBg04::UpdateTouch(int a) {
    u8 touching = 1;
    s32 value;
    u16 result = 0;
    if (TouchManager::Get().mDown == 0) {
        touching = 0;
    } else {
        value = TouchTurnAngle(0x80, 0x88, 0xf4);
        s32 limit;
        if (a) {
            limit = 0x1000;
        } else {
            limit = 0x1300;
        }
        if (value <= limit) {
            if (value >= -limit) {
                touching = 0;
            }
        }
    }
    if (!touching) {
        SlowDown();
        mBlend->LowerLevel();
        return 0;
    }
    if (value > 0) {
        mSpeed += 0x80;
        if (mSpeed > 10000) {
            mSpeed = 10000;
        }
        mSpeedSum += mSpeed;
    } else {
        mSpeed -= 0x80;
        if (mSpeed < -10000) {
            mSpeed = -10000;
        }
    }
    mSpeedSum += mSpeed;
    if (mSpeedSum >= 0xffff || mSpeedSum <= -0xffff) {
        mSpeedSum = 0;
        SoundManager::Get().StartSeqArc(0x2a);
    }
    s32 speed = 0;
    if (mSpeed > 0) {
        speed = mSpeed;
    } else if (mSpeed < 0) {
        speed = -mSpeed;
    }
    if (speed != 0) {
        result = (u32) (speed * 100) / 10000;
    }
    if (result != 0) {
        mBlend->RaiseLevel();
        SetTouched();
    }
    return result;
}

void ConfrontBg04::vfunc_44(int index, u16 count) {
}

void ConfrontBg04Rotate(ConfrontBg04 *scene) {
    MtxFx22 mtx;
    u16 speed = scene->mSpeed;
    scene->mAngle += speed;
    s32 idx = scene->mAngle >> 4;
    s32 s   = FX_SinCosTable_[idx * 2];
    MTX_Identity22_(&mtx);
    s32 c    = FX_SinCosTable_[idx * 2 + 1];
    mtx._[2] = -s;
    mtx._[1] = s;
    mtx._[0] = c;
    mtx._[3] = c;
    G2x_SetBGyAffine_((vu16 *) 0x4001030, &mtx, 0x80, 0x88, 0, 0);
}

void ConfrontBg04::SlowDown() {
    if (mSpeed == 0) {
        return;
    }
    if (mSpeed > 0) {
        mSpeed -= 0x40;
        if (mSpeed < 0) {
            mSpeed = 0;
        }
    } else if (mSpeed < 0) {
        mSpeed += 0x40;
        if (mSpeed > 0) {
            mSpeed = 0;
        }
    }
}

void ConfrontBg04::vfunc_34() {
}

BOOL ConfrontBg04::func_ov007_0221bd84() {
    return FALSE;
}
