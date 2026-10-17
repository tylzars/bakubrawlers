
#include "Battle/BattleObj01.hpp"

#include "Battle/BattleBase.hpp"

#include <nitro/fx.h>

#include "Graphics/CellAnimation.hpp"
#include "Graphics/SpriteManager.hpp"
#include "System/FileSystem.hpp"

// One 0x14-byte motion record of the table at 0x02279080.
extern BattleObj01Motion gBattleObj01Motions[];

void NormalizeXY(float x, float y, float *out);
int PointDistance(int x1, int y1, int x2, int y2);


BattleObj01::BattleObj01(u8 layer, u32 charBase) {
    mActive = 0;
    mUnk_05 = 0;
    mMotion = 0;
    SpriteScreen &screen = SpriteManager::Get().Screen(1);
    screen.SetObjVramMode(3);
    ChangeDir("/Game/Confrontation/Obj");
    mCellAnims[0] = new CellAnimation("vs_b_obj_01_nce.bin", 0, 1, 1);
    mCellAnims[0]->mCharBase = screen.GetCharBase(charBase);
    mCellAnims[0]->mPaletteBase = layer;
    mCellAnims[1] = new CellAnimation("vs_b_obj_01_nce.bin", 0, 1, 1);
    mCellAnims[1]->mCharBase = screen.GetCharBase(charBase);
    mCellAnims[1]->mPaletteBase = layer;
    ChangeDir("/");
}

BattleObj01::~BattleObj01() {
    u8 i = 0;
    do {
        CellAnimation *anim = mCellAnims[i];
        if (anim) {
            delete anim;
        }
        i++;
    } while (i < 2);
}

BOOL BattleObj01::Start(u8 fast, int x, int y, s16 targetX, s16 targetY, u16 angle, int motion) {
    if (mActive) {
        return 0;
    }
    mMotion = &gBattleObj01Motions[motion];
    u8 anim;
    if (fast != 0) {
        anim = 15;
    } else {
        anim = 13;
    }
    mCellAnims[0]->SetAnimation(anim);
    mCellAnims[1]->SetAnimation(anim + 1);
    mActive = 1;
    mUnk_05 = fast;
    mSpeed = mMotion->mStartSpeed;
    mX = x;
    mY = y;
    mScreenX = x;
    mScreenY = y;
    mTargetX = targetX;
    mTargetY = targetY;
    NormalizeXY(mTargetX - mX, mTargetY - mY, &mDirX);
    int step = angle >> 4;
    float p[2];
    p[0] = mX + (float) FX_SinCosTable_[step * 2 + 1] / 4096.0f;
    p[1] = mY + (float) FX_SinCosTable_[step * 2] / 4096.0f;
    NormalizeXY(p[0] - mX, p[1] - mY, &mSideX);
    mSideSpeed = mMotion->mSideSpeed;
    return 1;
}

BOOL BattleObj01::Update() {
    if (!mActive) {
        return 0;
    }
    BattleObj01Motion *motion = mMotion;
    if (mSpeed - motion->mDecel > motion->mLimit) {
        mSpeed = mSpeed - motion->mDecel;
    }
    mX = mX + mSpeed * mDirX;
    mY = mY + mSpeed * mDirY;
    if (mSideSpeed > 0.0f) {
        mX = mX + mSideSpeed * mSideX;
        mY = mY + mSideSpeed * mSideY;
        mSideSpeed = mSideSpeed - mMotion->mSideDecel;
        NormalizeXY(mTargetX - mX, mTargetY - mY, &mDirX);
    }
    mCellAnims[1]->Update();
    mCellAnims[1]->Draw(1, mScreenX, mScreenY, 1);
    mCellAnims[0]->Update();
    mCellAnims[0]->Draw(1, mX, mY, 1);
    mScreenX = (s32) mX;
    mScreenY = (s32) mY;
    if (PointDistance((s32) mTargetX, (s32) mTargetY, (s32) mX, (s32) mY) > 2) {
        return 0;
    }
    mActive = 0;
    return 1;
}
