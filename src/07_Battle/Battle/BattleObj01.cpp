
#include "Battle/BattleObj01.hpp"

#include "Battle/BattleBase.hpp"

#include <nitro/fx.h>

#include "Graphics/CellAnimation.hpp"
#include "Graphics/SpriteManager.hpp"
#include "System/FileSystem.hpp"

// One 0x14-byte motion record of the table at 0x02279080.
extern BattleObj01Motion data_ov007_02279080[];

void func_ov007_02225354(float x, float y, float *out);
int func_ov007_022252d0(int x1, int y1, int x2, int y2);


BattleObj01::BattleObj01(u8 layer, u32 charBase) {
    mActive = 0;
    mUnk_05 = 0;
    mMotion = 0;
    SpriteScreen &screen = SpriteManager::Get().Screen(1);
    screen.SetObjVramMode(3);
    ChangeDir("/Game/Confrontation/Obj");
    mAnims[0] = new CellAnimation("vs_b_obj_01_nce.bin", 0, 1, 1);
    mAnims[0]->mCharBase = screen.GetCharBase(charBase);
    mAnims[0]->mPaletteBase = layer;
    mAnims[1] = new CellAnimation("vs_b_obj_01_nce.bin", 0, 1, 1);
    mAnims[1]->mCharBase = screen.GetCharBase(charBase);
    mAnims[1]->mPaletteBase = layer;
    ChangeDir("/");
}

BattleObj01::~BattleObj01() {
    u8 i = 0;
    do {
        CellAnimation *anim = mAnims[i];
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
    mMotion = &data_ov007_02279080[motion];
    u8 anim;
    if (fast != 0) {
        anim = 15;
    } else {
        anim = 13;
    }
    mAnims[0]->SetAnimation(anim);
    mAnims[1]->SetAnimation(anim + 1);
    mActive = 1;
    mUnk_05 = fast;
    mSpeed = mMotion->mUnk_00;
    mX = x;
    mY = y;
    mScreenX = x;
    mScreenY = y;
    mTargetX = targetX;
    mTargetY = targetY;
    func_ov007_02225354(mTargetX - mX, mTargetY - mY, &mDirX);
    int step = angle >> 4;
    float p[2];
    p[0] = mX + (float) FX_SinCosTable_[step * 2 + 1] / 4096.0f;
    p[1] = mY + (float) FX_SinCosTable_[step * 2] / 4096.0f;
    func_ov007_02225354(p[0] - mX, p[1] - mY, &mSideX);
    mUnk_0c = mMotion->mUnk_0c;
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
    if (mUnk_0c > 0.0f) {
        mX = mX + mUnk_0c * mSideX;
        mY = mY + mUnk_0c * mSideY;
        mUnk_0c = mUnk_0c - mMotion->mUnk_10;
        func_ov007_02225354(mTargetX - mX, mTargetY - mY, &mDirX);
    }
    mAnims[1]->Update();
    mAnims[1]->Draw(1, mScreenX, mScreenY, 1);
    mAnims[0]->Update();
    mAnims[0]->Draw(1, mX, mY, 1);
    mScreenX = (s32) mX;
    mScreenY = (s32) mY;
    if (func_ov007_022252d0((s32) mTargetX, (s32) mTargetY, (s32) mX, (s32) mY) > 2) {
        return 0;
    }
    mActive = 0;
    return 1;
}
