#include "Battle/BattleCardEffect.hpp"
#include "Game/UnkStruct_020d433c.hpp"
#include "Graphics/CellAnimation.hpp"
#include "Graphics/Camera.hpp"
#include "Graphics/SpriteManager.hpp"
#include "System/FileSystem.hpp"
#include <stdio.h>

extern const u8 gCardEffectMiddleFrameCounts[];
extern const BattleCardEffectInfo gCardEffectInfos[];

BattleCardEffect::BattleCardEffect(u32 a, u32 b, u32 c, u32 d)
    : SceneBase(0x3f2, 0) {
    mCellAnim   = 0;
    mState  = 0;
    mCharOffset = a;
    mPalette = b;
    mKind    = c;
    mSetNextScene  = d;
    LoadAnimation();
    if (mKind >= 0x11) {
        BattlePlayerList *list = data_020d433c.mBattlePlayers;
        u8 x = list->mTurnPlayer;
        if (x == list->mUnk_2a9) {
            mAnimPairIndex = 0;
        } else {
            mAnimPairIndex = (x & 0xff) + 1;
        }
    }
    SetFirstAnimation();
    data_020d433c.mUnk_14 &= ~4;
}

BattleCardEffect::~BattleCardEffect() {
    if (mCellAnim) {
        delete mCellAnim;
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
            if (UpdateAnimation()) {
                SetMiddleAnimation();
                mTimer = mKind >= 0x11 ? 0 : gCardEffectMiddleFrameCounts[mKind];
                mState++;
            }
            break;
        case 1:
            UpdateAnimation();
            if (mTimer-- == 0) {
                SetLastAnimation();
                mState++;
            }
            break;
        case 2:
            if (UpdateAnimation()) {
                mState++;
            }
            break;
        case 3:
            if (mSetNextScene == 0) {
                mFinished = 1;
            } else {
                mContext->mList->mHead->mRequest->mRequest = 1;
                mFinished = 1;
            }
            break;
    }
}

void BattleCardEffect::SetFirstAnimation() {
    if (mKind >= 0x11) {
        mCellAnim->SetAnimation(mAnimPairIndex * 2);
    } else {
        mCellAnim->SetAnimation(0);
    }
}

void BattleCardEffect::SetMiddleAnimation() {
    if (mKind < 0x11) {
        mCellAnim->SetAnimation(1);
    }
}

void BattleCardEffect::SetLastAnimation() {
    if (mKind >= 0x11) {
        mCellAnim->SetAnimation(mAnimPairIndex * 2 + 1);
    } else {
        mCellAnim->SetAnimation(2);
    }
}

void BattleCardEffect::LoadAnimation() {
    char path[0x100];
    ChangeDir("/Game/Battle/Obj/");
    SpriteManager &sprites = SpriteManager::Get();
    u8 screenIndex = gCardEffectInfos[mKind].mScreen;
    SpriteScreen &screen = sprites.Screen(screenIndex);
    sprintf(path, "%s.nce_ncg1D.bin", gCardEffectInfos[mKind].mName);
    screen.LoadCharacters(path, mCharOffset, 0);
    sprintf(path, "%s_ncl.bin", gCardEffectInfos[mKind].mName);
    screen.LoadPalette(path, mPalette << 5, 0);
    sprintf(path, "%s_nce.bin", gCardEffectInfos[mKind].mName);
    mCellAnim = new CellAnimation(path, 0, 1, 1);
    mCellAnim->mCharBase = screen.GetCharBase(mCharOffset);
    mCellAnim->mPaletteBase   = mPalette;
    mCellAnim->SetAnimation(0);
    ChangeDir("/");
}

int BattleCardEffect::UpdateAnimation() {
    mCellAnim->Update();
    CameraManager &camB = CameraManager::Get();
    CameraManager &camA = CameraManager::Get();
    CameraMode modeB = (CameraMode) camB.mMode;
    CameraMode modeA = (CameraMode) camA.mMode;
    if ((modeB == CAMERA_MODE_2) | (modeA == CAMERA_MODE_3)) {
        if (CameraManager::Get().mCameraIndex != 0) {
            return 0;
        }
    }
    mCellAnim->Draw(gCardEffectInfos[mKind].mScreen, 0x80, 0x60, 0);
    return mCellAnim->mFinished;
}
