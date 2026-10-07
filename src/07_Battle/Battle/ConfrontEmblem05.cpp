#include "Battle/ConfrontEmblem05.hpp"

#include "Battle/BattleUtil.hpp"
#include "Graphics/SpriteManager.hpp"
#include "System/FileSystem.hpp"
#include "System/TouchManager.hpp"
#include <nitro/fx.h>
#include <stdio.h>

ConfrontEmblem05::ConfrontEmblem05(u8 emblem) {
    char path[0x80];
    mUnk_04_0 = 0;
    mAngle    = 0;
    mSpeed    = 0x50;
    SpriteScreen &sub = SpriteManager::Get().Screen(1);
    sub.func_02027624(3);
    ChangeDir("/Game/Confrontation/Obj/");
    sprintf(path, "vs_emblem_%02d_nce.bin", emblem);
    mAnim          = new CellAnimation(path, 0, 1, 1);
    mAnim->mCharBase = sub.func_020276f4(0x1c000);
    mAnim->mUnk_11 = 0xb;
    mAnim->SetAnimation(0);
    ChangeDir("/");
    mAnim->func_02029f6c();
    mAnim->SetAnimation(0);
    mX = 78.0f + 50.0f * ((float) FX_SinCosTable_[(mAngle >> 4) * 2 + 1] / 4096.0f);
    mY = 136.0f + 32.0f * ((float) FX_SinCosTable_[(mAngle >> 4) * 2] / 4096.0f);
}

ConfrontEmblem05::~ConfrontEmblem05() {
    delete mAnim;
}

void ConfrontEmblem05::func_ov007_02220a00() {
    if (!mUnk_04_0) {
        mX = 78.0f + 50.0f * ((float) FX_SinCosTable_[(mAngle >> 4) * 2 + 1] / 4096.0f);
    } else {
        mX = 178.0f + 50.0f * (0.0f - (float) FX_SinCosTable_[(mAngle >> 4) * 2 + 1] / 4096.0f);
    }
    mY = 136.0f + 32.0f * ((float) FX_SinCosTable_[(mAngle >> 4) * 2] / 4096.0f);
    if (mAngle + mSpeed >= 0x10000) {
        mUnk_04_0 ^= 1;
    }
    mAngle += mSpeed;
}

void ConfrontEmblem05::func_ov007_02220b48() {
    s32 x = (s32) mX;
    s32 y = (s32) mY;
    mAnim->func_0202a494(1, x, y, 0, 0x1000, 0x1000, 0);
}

int ConfrontEmblem05::func_ov007_02220b9c(int fast) {
    s32 dist = func_ov007_022252d0((s16) TouchManager::Get().mX, (s16) TouchManager::Get().mY, (s32) mX, (s32) mY);
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

void ConfrontEmblem05::func_ov007_02220cb0() {
    s32 speed = mSpeed - 0x10;
    if (speed < 0x50) {
        speed = 0x50;
    }
    mSpeed = speed;
}
