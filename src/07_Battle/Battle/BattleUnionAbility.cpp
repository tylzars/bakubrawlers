#include "Battle/BattleUnionAbility.hpp"
#include "Game/UnkStruct_020d433c.hpp"
#include "Graphics/CellAnimation.hpp"
#include "Graphics/SpriteManager.hpp"
#include "System/FileSystem.hpp"
#include "UI/SelectWindow.hpp"
#include "Graphics/AntennaIndicator.hpp"
#include <nitro/gx.h>

static const WindowCellRecord sUnionOneButtonCells[1] = {
    {"/Game/Battle/Obj/battle_b_obj_union_ability_nce.bin", 6, 0x1a000, 0xc, 0, 1, {6, 8, 7, 0}},
};

static const WindowItemRecord sUnionOneButtonItems[1] = {
    {0, 0x1, 56, 120, 144, 20, 0, {-1, -1, -1, -1}, 0, WINDOW_TOUCH_RELEASE},
};

static const WindowCellRecord sUnionTwoButtonCells[2] = {
    {"/Game/Battle/Obj/battle_b_obj_union_ability_nce.bin", 0, 0x1a000, 0xc, 0, 1, {0, 2, 1, 0}},
    {"/Game/Battle/Obj/battle_b_obj_union_ability_nce.bin", 3, 0x1a000, 0xc, 0, 1, {3, 5, 4, 0}},
};

static const WindowItemRecord sUnionTwoButtonItems[2] = {
    {0, 0x1, 56, 88, 144, 20, 0, {1, 1, 0, 0}, 0, WINDOW_TOUCH_RELEASE},
    {1, 0x1, 56, 120, 144, 20, 0, {0, 0, 1, 1}, 0, WINDOW_TOUCH_RELEASE},
};

static const u8 sTwoButtonAnimPositions[2][2] = {{0x38, 0x58}, {0x38, 0x78}};

BattleUnionAbility::BattleUnionAbility(u8 kind, u32 charOffset, u16 palette)
    : SceneBase(0x3e8, 0) {
    mCellAnimA = 0;
    mCellAnimB = 0;
    mSlide = 0x104;
    mStateFrames = 0;
    mState = 0;
    mKind  = kind;
    mNextScene = 0;
    mCancel = 0;
    mCharOffset = charOffset;
    mPalette    = palette;
    if (kind != 5) {
        mWindow = new SelectWindow(2, sUnionTwoButtonItems, 0);
    } else {
        mWindow = new SelectWindow(1, sUnionOneButtonItems, 0);
    }
    u8 i = 0;
    do {
        mCellAnims[i] = 0;
        i++;
    } while (i < 2);
}

BattleUnionAbility::~BattleUnionAbility() {
    if (mWindow) {
        delete mWindow;
    }
    if (mCellAnimA) {
        delete mCellAnimA;
    }
    if (mCellAnimB) {
        delete mCellAnimB;
    }
    u8 i = 0;
    do {
        if (mCellAnims[i]) {
            delete mCellAnims[i];
        }
        i++;
    } while (i < 2);
    GXS_SetVisibleWnd(0);
    G2S_SetBlendBrightness(0x11, 0);
    if (data_020d433c.mAntenna) {
        data_020d433c.mAntenna->mSecondVisible = 0;
    }
}

void BattleUnionAbility::Update() {
    if (mCancel != 0 && mState >= 1) {
        mStateFrames = 0;
        mState = 7;
    }
    switch (mState) {
        case 0:
            SetupSubWndAndAntenna();
            LoadGraphics();
            mState++;
            break;
        case 1:
            mStateFrames++;
            if ((int) mStateFrames > 5) {
                mStateFrames = 0;
                mState++;
                G2S_SetBlendBrightness(0x11, -5);
            }
            break;
        case 2:
            mSlide -= 0x18;
            if (mSlide <= 0) {
                mSlide = 0;
                if (mKind != 0 && mKind != 1 && mKind != 3 && mKind != 5) {
                    mState = 6;
                } else {
                    mWindow->mEnabled = 1;
                    mState++;
                }
            }
            break;
        case 3:
            if ((u8) mWindow->Update(1) == 1) {
                s8 id = mWindow->GetSelectedId();
                if (id != 0) {
                    if (id == 1) {
                        mNextScene = 2;
                    }
                } else {
                    mNextScene = 1;
                }
                mState++;
                mStateFrames = 0;
            }
            break;
        case 4:
            mStateFrames++;
            if ((int) mStateFrames > 20) {
                mStateFrames = 0;
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
            if (mNextScene == 0xff) {
                mFinished = 1;
            } else {
                RequestScene(mNextScene);
                mFinished = 1;
            }
            break;
    }
    Draw();
}

void BattleUnionAbility::Close(u8 nextScene) {
    mCancel = 1;
    mNextScene = nextScene;
}

void BattleUnionAbility::SetupSubWndAndAntenna() {
    switch (mKind) {
        case 0:
        case 1:
        case 3:
        case 4:
        case 5:
            if (data_020d433c.mAntenna) {
                data_020d433c.mAntenna->mSecondVisible = 1;
            }
            GXS_SetVisibleWnd(4);
            G2S_SetWndOBJInsidePlane(0x10, FALSE);
            G2S_SetWndOutsidePlane(0x11, TRUE);
            break;
    }
}

void BattleUnionAbility::func_ov007_022344a4() {
    SpriteScreen &screen = SpriteManager::Get().Screen(1);
    screen.SetObjVramMode(3);
    ChangeDir("/Game/Battle/Obj");
    if (mKind == 0 || mKind == 1 || mKind == 3) {
        mWindow->SetCells(2, sUnionTwoButtonCells);
        mWindow->mCellAnims[0]->SetAnimation(1);
        LoadAndQueueUpload(1, UPLOAD_KIND_OBJ, mCharOffset, "battle_b_obj_union_ability.nce_ncg1D.bin", 0, 0, 0);
        u8 i = 0;
        do {
            mCellAnims[i]            = new CellAnimation("battle_b_obj_union_ability_nce.bin", 9, 1, 1);
            mCellAnims[i]->mCharBase = screen.GetCharBase(mCharOffset);
            mCellAnims[i]->mPaletteBase   = mPalette;
            i++;
        } while (i < 2);
    } else if (mKind == 5) {
        mWindow->SetCells(1, sUnionOneButtonCells);
        mWindow->mCellAnims[0]->SetAnimation(7);
        LoadAndQueueUpload(1, UPLOAD_KIND_OBJ, mCharOffset, "battle_b_obj_union_ability.nce_ncg1D.bin", 0, 0, 0);
        mCellAnims[0]            = new CellAnimation("battle_b_obj_union_ability_nce.bin", 9, 1, 1);
        mCellAnims[0]->mCharBase = screen.GetCharBase(mCharOffset);
        mCellAnims[0]->mPaletteBase   = mPalette;
    }
    LoadAndQueueUpload(1, UPLOAD_KIND_OBJ_PLTT, mPalette << 5, "battle_b_obj_ability_ncl.bin", 0, 0, 0);
    ChangeDir("/");
    mCharOffset += 0xa00;
}

void BattleUnionAbility::LoadGraphics() {
    SpriteScreen &screen = SpriteManager::Get().Screen(1);
    screen.SetObjVramMode(3);
    func_ov007_022344a4();
    ChangeDir("/Game/Battle/Obj");
    switch (mKind) {
        case 0:
            LoadAndQueueUpload(1, UPLOAD_KIND_OBJ, mCharOffset, "battle_b_obj_ability.nce_ncg1D.bin", 0, 0, 0);
            mCellAnimA = new CellAnimation("battle_b_obj_ability_nce.bin", 0, 1, 1);
            mCellAnimB = new CellAnimation("battle_b_obj_ability_nce.bin", 3, 1, 1);
            break;
        case 1:
            LoadAndQueueUpload(1, UPLOAD_KIND_OBJ, mCharOffset, "battle_b_obj_ability.nce_ncg1D.bin", 0, 0, 0);
            mCellAnimA = new CellAnimation("battle_b_obj_ability_nce.bin", 1, 1, 1);
            mCellAnimB = new CellAnimation("battle_b_obj_ability_nce.bin", 4, 1, 1);
            break;
        case 2:
            LoadAndQueueUpload(1, UPLOAD_KIND_OBJ, mCharOffset, "battle_b_obj_ability.nce_ncg1D.bin", 0, 0, 0);
            mCellAnimA = new CellAnimation("battle_b_obj_ability_nce.bin", 2, 1, 1);
            mCellAnimB = new CellAnimation("battle_b_obj_ability_nce.bin", 5, 1, 1);
            break;
        case 3:
            LoadAndQueueUpload(1, UPLOAD_KIND_OBJ, mCharOffset, "battle_b_obj_fie_ability.nce_ncg1D.bin", 0, 0, 0);
            mCellAnimA = new CellAnimation("battle_b_obj_fie_ability_nce.bin", 0, 1, 1);
            mCellAnimB = new CellAnimation("battle_b_obj_fie_ability_nce.bin", 2, 1, 1);
            break;
        case 4:
            LoadAndQueueUpload(1, UPLOAD_KIND_OBJ, mCharOffset, "battle_b_obj_wait.nce_ncg1D.bin", 0, 0, 0);
            mCellAnimA = new CellAnimation("battle_b_obj_wait_nce.bin", 0, 1, 1);
            mCellAnimB = new CellAnimation("battle_b_obj_wait_nce.bin", 1, 1, 1);
            break;
        case 5:
            LoadAndQueueUpload(1, UPLOAD_KIND_OBJ, mCharOffset, "battle_b_obj_fie_ability.nce_ncg1D.bin", 0, 0, 0);
            mCellAnimA = new CellAnimation("battle_b_obj_fie_ability_nce.bin", 1, 1, 1);
            mCellAnimB = new CellAnimation("battle_b_obj_fie_ability_nce.bin", 3, 1, 1);
            break;
    }
    ChangeDir("/");
    mCellAnimA->mCharBase = screen.GetCharBase(mCharOffset);
    mCellAnimA->mPaletteBase   = mPalette;
    mCellAnimB->mCharBase = screen.GetCharBase(mCharOffset);
    mCellAnimB->mPaletteBase   = mPalette;
}

void BattleUnionAbility::Draw() {
    mCellAnimA->Update();
    mCellAnimA->Draw(1, (s16) (mSlide + 0x80), 0x60, 0);
    mCellAnimB->Update();
    mCellAnimB->Draw(1, (s16) (mSlide + 0x80), 0x60, 0);
    switch (mKind) {
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
                mCellAnims[i]->Update();
                mCellAnims[i]->Draw(1, (s16) (mSlide + sTwoButtonAnimPositions[i][0]), sTwoButtonAnimPositions[i][1], 0);
                i++;
            } while (i < 2);
            mWindow->Draw();
            break;
        }
        case 5:
            SelectWindow *window = mWindow;
            window->mCellPos[0].x = mSlide + 0x38;
            window->mCellPos[0].y = 0x78;
            mCellAnims[0]->Update();
            mCellAnims[0]->Draw(1, (s16) (mSlide + 0x38), 0x78, 0);
            mWindow->Draw();
            break;
    }
}
