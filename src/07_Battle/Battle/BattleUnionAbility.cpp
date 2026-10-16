#include "Battle/BattleUnionAbility.hpp"
#include "Game/UnkStruct_020d433c.hpp"
#include "Graphics/CellAnimation.hpp"
#include "Graphics/SpriteManager.hpp"
#include "System/FileSystem.hpp"
#include "UI/SelectWindow.hpp"
#include "Graphics/AntennaIndicator.hpp"
#include <nitro/gx.h>

static const WindowCellRecord data_ov007_02279a1c[1] = {
    {"/Game/Battle/Obj/battle_b_obj_union_ability_nce.bin", 6, 0x1a000, 0xc, 0, 1, {6, 8, 7, 0}},
};

static const WindowItemRecord data_ov007_02279a00[1] = {
    {0, 0x1, 56, 120, 144, 20, 0, {-1, -1, -1, -1}, 0, 1},
};

static const WindowCellRecord data_ov007_02279a70[2] = {
    {"/Game/Battle/Obj/battle_b_obj_union_ability_nce.bin", 0, 0x1a000, 0xc, 0, 1, {0, 2, 1, 0}},
    {"/Game/Battle/Obj/battle_b_obj_union_ability_nce.bin", 3, 0x1a000, 0xc, 0, 1, {3, 5, 4, 0}},
};

static const WindowItemRecord data_ov007_02279a38[2] = {
    {0, 0x1, 56, 88, 144, 20, 0, {1, 1, 0, 0}, 0, 1},
    {1, 0x1, 56, 120, 144, 20, 0, {0, 0, 1, 1}, 0, 1},
};

static const u8 data_ov007_022799fc[2][2] = {{0x38, 0x58}, {0x38, 0x78}};

BattleUnionAbility::BattleUnionAbility(u8 type, u32 charOffset, u16 palette)
    : SceneBase(0x3e8, 0) {
    mAnimA = 0;
    mAnimB = 0;
    mSlide = 0x104;
    mTimer = 0;
    mState = 0;
    mType  = type;
    mChoice = 0;
    mCancel = 0;
    mCharOffset = charOffset;
    mPalette    = palette;
    if (type != 5) {
        mWindow = new SelectWindow(2, data_ov007_02279a38, 0);
    } else {
        mWindow = new SelectWindow(1, data_ov007_02279a00, 0);
    }
    u8 i = 0;
    do {
        mAnims[i] = 0;
        i++;
    } while (i < 2);
}

BattleUnionAbility::~BattleUnionAbility() {
    if (mWindow) {
        delete mWindow;
    }
    if (mAnimA) {
        delete mAnimA;
    }
    if (mAnimB) {
        delete mAnimB;
    }
    u8 i = 0;
    do {
        if (mAnims[i]) {
            delete mAnims[i];
        }
        i++;
    } while (i < 2);
    GXS_SetVisibleWnd(0);
    G2S_SetBlendBrightness(0x11, 0);
    if (data_020d433c.mUnk_10) {
        data_020d433c.mUnk_10->mSecondVisible = 0;
    }
}

void BattleUnionAbility::Update() {
    if (mCancel != 0 && mState >= 1) {
        mTimer = 0;
        mState = 7;
    }
    switch (mState) {
        case 0:
            func_ov007_02234428();
            func_ov007_0223471c();
            mState++;
            break;
        case 1:
            mTimer++;
            if ((int) mTimer > 5) {
                mTimer = 0;
                mState++;
                G2S_SetBlendBrightness(0x11, -5);
            }
            break;
        case 2:
            mSlide -= 0x18;
            if (mSlide <= 0) {
                mSlide = 0;
                if (mType != 0 && mType != 1 && mType != 3 && mType != 5) {
                    mState = 6;
                } else {
                    mWindow->mUnk_064 = 1;
                    mState++;
                }
            }
            break;
        case 3:
            if ((u8) mWindow->Update(1) == 1) {
                s8 id = mWindow->GetSelectedId();
                if (id != 0) {
                    if (id == 1) {
                        mChoice = 2;
                    }
                } else {
                    mChoice = 1;
                }
                mState++;
                mTimer = 0;
            }
            break;
        case 4:
            mTimer++;
            if ((int) mTimer > 20) {
                mTimer = 0;
                mState++;
            }
            break;
        case 5:
            mSlide -= 0x18;
            if (mSlide <= -0x100) {
                mState = 7;
            }
            break;
        case 7:
            if (mChoice == 0xff) {
                mFinished = 1;
            } else {
                RequestScene(mChoice);
                mFinished = 1;
            }
            break;
    }
    func_ov007_02234b5c();
}

void BattleUnionAbility::func_ov007_02234418(u8 choice) {
    mCancel = 1;
    mChoice = choice;
}

void BattleUnionAbility::func_ov007_02234428() {
    switch (mType) {
        case 0:
        case 1:
        case 3:
        case 4:
        case 5:
            if (data_020d433c.mUnk_10) {
                data_020d433c.mUnk_10->mSecondVisible = 1;
            }
            GXS_SetVisibleWnd(4);
            G2S_SetWndOBJInsidePlane(0x10, FALSE);
            G2S_SetWndOutsidePlane(0x11, TRUE);
            break;
    }
}

void BattleUnionAbility::func_ov007_022344a4() {
    SpriteScreen &screen = SpriteManager::Get().Screen(1);
    screen.func_02027624(3);
    ChangeDir("/Game/Battle/Obj");
    if (mType == 0 || mType == 1 || mType == 3) {
        mWindow->SetCells(2, data_ov007_02279a70);
        mWindow->mCells[0]->SetAnimation(1);
        func_02025b64(1, 4, mCharOffset, "battle_b_obj_union_ability.nce_ncg1D.bin", 0, 0, 0);
        u8 i = 0;
        do {
            mAnims[i]            = new CellAnimation("battle_b_obj_union_ability_nce.bin", 9, 1, 1);
            mAnims[i]->mCharBase = screen.func_020276f4(mCharOffset);
            mAnims[i]->mUnk_11   = mPalette;
            i++;
        } while (i < 2);
    } else if (mType == 5) {
        mWindow->SetCells(1, data_ov007_02279a1c);
        mWindow->mCells[0]->SetAnimation(7);
        func_02025b64(1, 4, mCharOffset, "battle_b_obj_union_ability.nce_ncg1D.bin", 0, 0, 0);
        mAnims[0]            = new CellAnimation("battle_b_obj_union_ability_nce.bin", 9, 1, 1);
        mAnims[0]->mCharBase = screen.func_020276f4(mCharOffset);
        mAnims[0]->mUnk_11   = mPalette;
    }
    func_02025b64(1, 5, mPalette << 5, "battle_b_obj_ability_ncl.bin", 0, 0, 0);
    ChangeDir("/");
    mCharOffset += 0xa00;
}

void BattleUnionAbility::func_ov007_0223471c() {
    SpriteScreen &screen = SpriteManager::Get().Screen(1);
    screen.func_02027624(3);
    func_ov007_022344a4();
    ChangeDir("/Game/Battle/Obj");
    switch (mType) {
        case 0:
            func_02025b64(1, 4, mCharOffset, "battle_b_obj_ability.nce_ncg1D.bin", 0, 0, 0);
            mAnimA = new CellAnimation("battle_b_obj_ability_nce.bin", 0, 1, 1);
            mAnimB = new CellAnimation("battle_b_obj_ability_nce.bin", 3, 1, 1);
            break;
        case 1:
            func_02025b64(1, 4, mCharOffset, "battle_b_obj_ability.nce_ncg1D.bin", 0, 0, 0);
            mAnimA = new CellAnimation("battle_b_obj_ability_nce.bin", 1, 1, 1);
            mAnimB = new CellAnimation("battle_b_obj_ability_nce.bin", 4, 1, 1);
            break;
        case 2:
            func_02025b64(1, 4, mCharOffset, "battle_b_obj_ability.nce_ncg1D.bin", 0, 0, 0);
            mAnimA = new CellAnimation("battle_b_obj_ability_nce.bin", 2, 1, 1);
            mAnimB = new CellAnimation("battle_b_obj_ability_nce.bin", 5, 1, 1);
            break;
        case 3:
            func_02025b64(1, 4, mCharOffset, "battle_b_obj_fie_ability.nce_ncg1D.bin", 0, 0, 0);
            mAnimA = new CellAnimation("battle_b_obj_fie_ability_nce.bin", 0, 1, 1);
            mAnimB = new CellAnimation("battle_b_obj_fie_ability_nce.bin", 2, 1, 1);
            break;
        case 4:
            func_02025b64(1, 4, mCharOffset, "battle_b_obj_wait.nce_ncg1D.bin", 0, 0, 0);
            mAnimA = new CellAnimation("battle_b_obj_wait_nce.bin", 0, 1, 1);
            mAnimB = new CellAnimation("battle_b_obj_wait_nce.bin", 1, 1, 1);
            break;
        case 5:
            func_02025b64(1, 4, mCharOffset, "battle_b_obj_fie_ability.nce_ncg1D.bin", 0, 0, 0);
            mAnimA = new CellAnimation("battle_b_obj_fie_ability_nce.bin", 1, 1, 1);
            mAnimB = new CellAnimation("battle_b_obj_fie_ability_nce.bin", 3, 1, 1);
            break;
    }
    ChangeDir("/");
    mAnimA->mCharBase = screen.func_020276f4(mCharOffset);
    mAnimA->mUnk_11   = mPalette;
    mAnimB->mCharBase = screen.func_020276f4(mCharOffset);
    mAnimB->mUnk_11   = mPalette;
}

void BattleUnionAbility::func_ov007_02234b5c() {
    mAnimA->Update();
    mAnimA->Draw(1, (s16) (mSlide + 0x80), 0x60, 0);
    mAnimB->Update();
    mAnimB->Draw(1, (s16) (mSlide + 0x80), 0x60, 0);
    switch (mType) {
        case 0:
        case 1:
        case 3: {
            SelectWindow *window = mWindow;
            window->mCellPos[0].x = mSlide + 0x38;
            window->mCellPos[0].y = 0x58;
            window = mWindow;
            window->mCellPos[1].x = mSlide + 0x38;
            window->mCellPos[1].y = 0x78;
            u8 i = 0;
            do {
                mAnims[i]->Update();
                mAnims[i]->Draw(1, (s16) (mSlide + data_ov007_022799fc[i][0]), data_ov007_022799fc[i][1], 0);
                i++;
            } while (i < 2);
            mWindow->Draw();
            break;
        }
        case 5:
            SelectWindow *window = mWindow;
            window->mCellPos[0].x = mSlide + 0x38;
            window->mCellPos[0].y = 0x78;
            mAnims[0]->Update();
            mAnims[0]->Draw(1, (s16) (mSlide + 0x38), 0x78, 0);
            mWindow->Draw();
            break;
    }
}
