#include "Battle/BattleCardEffect.hpp"
#include "Game/UnkStruct_020d433c.hpp"
#include "Graphics/CellAnimation.hpp"
#include "Graphics/Camera.hpp"
#include "Graphics/SpriteManager.hpp"
#include "System/FileSystem.hpp"
#include <stdio.h>

extern const u8 data_ov007_0227c018[];
extern const BattleCardEffectInfo data_ov007_0227c02c[];

BattleCardEffect::BattleCardEffect(u32 a, u32 b, u32 c, u32 d)
    : SceneBase(0x3f2, 0) {
    mAnim   = 0;
    mState  = 0;
    mUnk_14 = a;
    mPalette = b;
    mKind    = c;
    mUnk_1b  = d;
    func_ov007_022716e8();
    if (mKind >= 0x11) {
        BattlePlayerList *list = data_020d433c.mBattlePlayers;
        u8 x = list->mUnk_28a;
        if (x == list->mUnk_2a9) {
            mUnk_11 = 0;
        } else {
            mUnk_11 = (x & 0xff) + 1;
        }
    }
    func_ov007_02271648();
    data_020d433c.mUnk_14 &= ~4;
}

BattleCardEffect::~BattleCardEffect() {
    if (mAnim) {
        delete mAnim;
    }
}

void BattleCardEffect::Update() {
    data_020d433c.mUnk_14 &= 4;
    if (data_020d433c.mUnk_14) {
        mFinished = 1;
        return;
    }
    switch (mState) {
        case 0:
            if (func_ov007_02271888()) {
                func_ov007_02271684();
                mTimer = mKind >= 0x11 ? 0 : data_ov007_0227c018[mKind];
                mState++;
            }
            break;
        case 1:
            func_ov007_02271888();
            if (mTimer-- == 0) {
                func_ov007_022716a8();
                mState++;
            }
            break;
        case 2:
            if (func_ov007_02271888()) {
                mState++;
            }
            break;
        case 3:
            if (mUnk_1b == 0) {
                mFinished = 1;
            } else {
                mContext->mList->mHead->mRequest->mNextScene = 1;
                mFinished = 1;
            }
            break;
    }
}

void BattleCardEffect::func_ov007_02271648() {
    if (mKind >= 0x11) {
        mAnim->SetAnimation(mUnk_11 * 2);
    } else {
        mAnim->SetAnimation(0);
    }
}

void BattleCardEffect::func_ov007_02271684() {
    if (mKind < 0x11) {
        mAnim->SetAnimation(1);
    }
}

void BattleCardEffect::func_ov007_022716a8() {
    if (mKind >= 0x11) {
        mAnim->SetAnimation(mUnk_11 * 2 + 1);
    } else {
        mAnim->SetAnimation(2);
    }
}

void BattleCardEffect::func_ov007_022716e8() {
    char path[0x100];
    ChangeDir("/Game/Battle/Obj/");
    SpriteManager &sprites = SpriteManager::Get();
    u8 screenIndex = data_ov007_0227c02c[mKind].mScreen;
    SpriteScreen &screen = sprites.Screen(screenIndex);
    sprintf(path, "%s.nce_ncg1D.bin", data_ov007_0227c02c[mKind].mName);
    screen.LoadCharacters(path, mUnk_14, 0);
    sprintf(path, "%s_ncl.bin", data_ov007_0227c02c[mKind].mName);
    screen.LoadPalette(path, mPalette << 5, 0);
    sprintf(path, "%s_nce.bin", data_ov007_0227c02c[mKind].mName);
    mAnim = new CellAnimation(path, 0, 1, 1);
    mAnim->mCharBase = screen.func_020276f4(mUnk_14);
    mAnim->mUnk_11   = mPalette;
    mAnim->SetAnimation(0);
    ChangeDir("/");
}

int BattleCardEffect::func_ov007_02271888() {
    mAnim->Update();
    CameraManager &camB = CameraManager::Get();
    CameraManager &camA = CameraManager::Get();
    CameraMode modeB = (CameraMode) camB.mMode;
    CameraMode modeA = (CameraMode) camA.mMode;
    if ((modeB == CAMERA_MODE_2) | (modeA == CAMERA_MODE_3)) {
        if (CameraManager::Get().mActive != 0) {
            return 0;
        }
    }
    mAnim->Draw(data_ov007_0227c02c[mKind].mScreen, 0x80, 0x60, 0);
    return mAnim->mUnk_12;
}
