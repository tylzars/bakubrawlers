#include "Battle/ConfrontEmblem03.hpp"

#include "Battle/BattleBase.hpp"
#include "Battle/BattleUtil.hpp"
#include "Graphics/SpriteManager.hpp"
#include "System/FileSystem.hpp"
#include "System/TouchManager.hpp"
#include <stdio.h>

ConfrontEmblem03::ConfrontEmblem03(u8 index) {
    char path[0x80];
    mUnk_04 = 2;
    mUnk_05 = index;
    mUnk_07_0 = 0;
    mUnk_07_1 = 0;
    mUnk_07_2 = 0;
    mUnk_08 = 0;
    mUnk_09 = 0;
    mUnk_0a = 0;
    mUnk_0b = -1;
    SpriteScreen &screen = SpriteManager::Get().Screen(1);
    screen.func_02027624(3);
    ChangeDir("/Game/Confrontation/Obj/");
    sprintf(path, "vs_emblem_%02d_nce.bin", index);
    mAnim = new CellAnimation(path, 0, 1, 1);
    mAnim->mCharBase = screen.func_020276f4(0x1c000);
    mAnim->mUnk_11 = 11;
    mAnim->SetAnimation(0);
    ChangeDir("/");
}

ConfrontEmblem03::~ConfrontEmblem03() {
    delete mAnim;
}

void ConfrontEmblem03::func_ov007_0221f1b8(u8 slot, int flag) {
    mUnk_14 = (float) ((slot % 4) * 64 + 0x10 + func_ov007_0221d348(0x20));
    mUnk_18 = (float) ((slot / 4) * 32 + 0x68 + func_ov007_0221d348(2));
    mUnk_04 = 0;
    mUnk_0a = 0;
    mUnk_07_1 = 0;
    mUnk_0b = slot;
    mUnk_08 = 0;
    if (flag == 0) {
        mUnk_09 = 0x14;
    } else {
        mUnk_09 = 0x50;
    }
    mUnk_1c = 0.1f;
    mAnim->func_02029f6c();
    mAnim->SetAnimation(0);
}

void ConfrontEmblem03::func_ov007_0221f268() {
    switch (mUnk_04) {
        case 0:
            switch (mUnk_0a) {
                case 0:
                    mUnk_1c += 0.05f;
                    if (mUnk_1c >= 1.0f) {
                        mUnk_1c = 1.0f;
                        mUnk_0a++;
                    }
                    break;
                case 1:
                    mUnk_08++;
                    if (mUnk_08 > mUnk_09) {
                        mUnk_0a++;
                    }
                    break;
                case 2:
                    mUnk_1c = mUnk_1c - 0.05f;
                    if (mUnk_1c <= 0.1f) {
                        mUnk_04 = 2;
                    }
                    break;
            }
            break;
        case 1:
            if (mAnim->mUnk_12 == 0) {
                mAnim->Update();
            } else {
                mUnk_07_0 = 0;
                mUnk_04 = 2;
            }
            break;
        case 2:
            break;
    }
}

void ConfrontEmblem03::func_ov007_0221f368() {
    if (mUnk_07_2) {
        return;
    }
    if (mUnk_04 == 0) {
        if (mUnk_0a != 2) {
            mUnk_0a = 2;
        }
    }
    mUnk_07_2 = 1;
}

void ConfrontEmblem03::func_ov007_0221f3a4() {
    switch (mUnk_04) {
        case 0: {
            float a;
            if (mUnk_1c > 0.0f) {
                a = 0.5f + 4096.0f * mUnk_1c;
            } else {
                a = 4096.0f * mUnk_1c - 0.5f;
            }
            float b;
            if (mUnk_1c > 0.0f) {
                b = 0.5f + 4096.0f * mUnk_1c;
            } else {
                b = 4096.0f * mUnk_1c - 0.5f;
            }
            mAnim->func_0202a494(1, (s32) mUnk_14, (s32) mUnk_18, 2, (s32) b, (s32) a, 0);
            break;
        }
        case 1: {
            float a;
            if (mUnk_1c > 0.0f) {
                a = 0.5f + 4096.0f * mUnk_1c;
            } else {
                a = 4096.0f * mUnk_1c - 0.5f;
            }
            float b;
            if (mUnk_1c > 0.0f) {
                b = 0.5f + 4096.0f * mUnk_1c;
            } else {
                b = 4096.0f * mUnk_1c - 0.5f;
            }
            mAnim->func_0202a494(1, (s32) mUnk_14, (s32) mUnk_18, 2, (s32) b, (s32) a, 0);
            break;
        }
        case 2:
            break;
    }
}

u8 ConfrontEmblem03::func_ov007_0221f57c() {
    return mUnk_04 == 2 ? 1 : 0;
}

int ConfrontEmblem03::func_ov007_0221f594() {
    if (mUnk_04 != 0 || mUnk_07_0 == 1) {
        return -1;
    }
    if (func_ov007_022252d0((s16) TouchManager::Get().mX, (s16) TouchManager::Get().mY, (s32) mUnk_14, (s32) mUnk_18) > 0x18) {
        return -1;
    }
    u8 anim;
    s16 result;
    if (mUnk_0a <= 1) {
        anim = 1;
        result = 100;
    } else {
        anim = 2;
        result = (s16) (s32) (100.0f * mUnk_1c) / 2;
        mUnk_07_1 = 1;
    }
    if (result != -1) {
        mUnk_07_0 = 1;
        mUnk_04 = 1;
        mAnim->func_02029f6c();
        mAnim->SetAnimation(anim);
    }
    return result;
}

int ConfrontEmblem03::func_ov007_0221f70c() {
    if (mUnk_04 == 2) {
        s8 v = mUnk_0b;
        if (v != -1) {
            return v;
        }
    }
    return -1;
}
