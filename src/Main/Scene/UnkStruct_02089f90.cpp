#include "Unknown/UnkStruct_02089f90.hpp"

#include "Graphics/Display.hpp"
#include "Graphics/SpriteManager.hpp"
#include "System/FileSystem.hpp"
#include "System/FrameTask.hpp"
#include "System/SoundManager.hpp"
#include <nitro/gx.h>
#include <nitro/os.h>
#include <stdio.h>

// Gift unlock popup (assets in /Game/Giftunlock): shows the unlocked emblem and a confirm button.

static const u8 sEmblemFiles[12] = {3, 3, 3, 4, 3, 3, 3, 0, 1, 2, 5, 0};

static const WindowItemRecord sOkItems[1] = {
    {0, 5, 201, 170, 52, 20, 0, {-1, -1, -1, -1}, 0, 1},
};

static const WindowCellRecord sOkCells[1] = {
    {"/Game/2D/union_b/union_b_obj_cockpit_nce.bin", 0, 0, 0, 3, 1, {0, 1, 1, 0}},
};

UnkStruct_02089f90::UnkStruct_02089f90(s32 unk1) :
    SceneBase(0x3e8, 0) {
    mMenu   = NULL;
    mOffset = 0x108;
    mUnk_1e = 0;
    mState  = 0;
    mIndex  = unk1;
    mCell   = NULL;
    for (u8 i = 0; i < 2; i++) {
        mCells[i] = NULL;
    }
}

UnkStruct_02089f90::~UnkStruct_02089f90() {
    delete mMenu;
    delete mCell;
    for (u8 i = 0; i < 2; i++) {
        delete mCells[i];
    }
}

void UnkStruct_02089f90::Update() {
    BOOL draw = TRUE;
    switch (mState) {
        case 0:
            func_0208a454();
            func_0208a598();
            SpriteManager::Get().StartFade(2);
            draw = FALSE;
            mState++;
            break;
        case 1:
            if (SpriteManager::Get().sFade.mMain <= 1) {
                FrameTask::Get().func_0203dd90();
                SoundManager::Get().func_020246f8(0x58);
                mUnk_1e = 1;
                mState++;
            }
            break;
        case 2:
            if (mOffset != 0) {
                if (mOffset >= 0x18) {
                    mOffset -= 0x18;
                } else if (mOffset <= -0x18) {
                    mOffset += 0x18;
                } else {
                    mOffset = 0;
                }
            } else {
                mMenu->mUnk_064 = 1;
                mState++;
            }
            break;
        case 3:
            if (mMenu->Update(TRUE) == 1) {
                SpriteManager::Get().StartFade(3);
                mState++;
            }
            break;
        case 4:
            if (!SpriteManager::Get().IsFading()) {
                RequestScene(1);
                draw      = FALSE;
                mFinished = 1;
            }
            break;
    }
    if (draw) {
        func_0208aa5c();
    }
}

void UnkStruct_02089f90::func_0208a454() {
    GX_SetDispSelect(1);
    SpriteManager::Get().func_02026dec(1, 1);
    func_02001e68(1, 0, 0);
    func_02001ed0(0);
    G2_SetBG0Control(0, 1, 0, 1, 0);
    G2S_SetBG0Control(0, 1, 0, 1, 0);
    G2_SetBG0Offset(0, 0);
    G2S_SetBG0Offset(0, 0);
    G2_SetBG0Priority(3);
    G2S_SetBG0Priority(3);
    GX_SetVisiblePlane(0x11);
    GXS_SetVisiblePlane(0x11);
    G2_SetBlendBrightness(0x1f, 0);
    G2S_SetBlendBrightness(0x1f, 0);
}

void UnkStruct_02089f90::func_0208a598() {
    char path[0x40];
    char name[0x40];
    ChangeDir("/game/Giftunlock/Bg/");
    func_02025b64(0, 1, 0, "gift_unlock_u_bg_ncl.bin", 0, 0, 0);
    func_02025b64(0, 11, 0, "gift_unlock_u_bg_ncg.bin", 0, 0, 0);
    func_02025b64(0, 7, 0, "gift_unlock_u_bg_nsc.bin", 0, 0, 0);
    func_02025b64(1, 1, 0, "gift_unlock_b_bg_ncl.bin", 0, 0, 0);
    func_02025b64(1, 11, 0, "gift_unlock_b_bg_ncg.bin", 0, 0, 0);
    func_02025b64(1, 7, 0, "gift_unlock_b_bg_nsc.bin", 0, 0, 0);
    SpriteScreen &top = SpriteManager::Get().Screen(0);
    ChangeDir("/Game/Giftunlock/Obj/");
    sprintf(path, "gift_unlock_u_ob_0%d.nce_ncg1D.bin", sEmblemFiles[mIndex]);
    top.LoadCharacters(path, 0, 0);
    top.LoadPalette("gift_unlock_u_ob_ncl.bin", 0, 0);
    sprintf(path, "gift_unlock_u_ob_0%d_nce.bin", sEmblemFiles[mIndex]);
    mCell            = new CellAnimation(path, 0, 1, 1);
    mCell->mCharBase = top.func_020276f4(0);
    mCell->mUnk_11   = 0;
    SpriteScreen &bottom = SpriteManager::Get().Screen(1);
    ChangeDir("/Game/2D/union_b/");
    bottom.LoadCharacters("union_b_obj_cockpit.nce_ncg1D.bin", 0, 0);
    bottom.LoadPalette("union_b_obj_cockpit_ncl.bin", 0, 0);
    ChangeDir("/Game/Myroom/");
    sprintf(name, "myroom_gift_u_obj_emblem_%02d.nce_ncg1D.bin", mIndex + 1);
    bottom.LoadCharacters(name, 0x2000, 0);
    sprintf(name, "myroom_gift_u_obj_emblem_%02d_ncl.bin", mIndex + 1);
    bottom.func_02027fec(name, 0, 0);
    sprintf(name, "myroom_gift_u_obj_emblem_%02d_nce.bin", mIndex + 1);
    mCells[0]            = new CellAnimation(name, 0, 1, 1);
    mCells[0]->mCharBase = bottom.func_020276f4(0x2000);
    mCells[0]->mUnk_11   = 0;
    ChangeDir("/Game/Giftunlock/Obj/");
    bottom.LoadCharacters("gift_unlock_b_ob.nce_ncg1D.bin", 0x5000, 0);
    bottom.LoadPalette("gift_unlock_b_ob_ncl.bin", 0x20, 0);
    mCells[1]            = new CellAnimation("gift_unlock_b_ob_nce.bin", 0, 1, 1);
    mCells[1]->mCharBase = bottom.func_020276f4(0x5000);
    mCells[1]->mUnk_11   = 1;
    ChangeDir("/");
    delete mMenu;
    mMenu = new SelectWindow(1, sOkItems, -1);
    mMenu->SetCells(1, sOkCells);
    mMenu->mCursor->mSelection = -1;
    mMenu->mUnk_064            = 0;
}

void UnkStruct_02089f90::func_0208aa5c() {
    CellAnimation *cell = mCell;
    if (cell->mUnk_0e == 0 && cell->mUnk_12 != 0) {
        cell->SetAnimation(1);
    }
    mCell->Update();
    mCell->Draw(0, 0x80, 0x60, 2);
    mMenu->Draw();
    mCells[0]->Update();
    mCells[0]->Draw(1, (s16) (mOffset + 0x50), 0, 2);
    cell = mCells[1];
    if (cell->mUnk_0e == 0 && cell->mUnk_12 != 0) {
        cell->SetAnimation(1);
    }
    mCells[1]->Update();
    mCells[1]->Draw(1, 0x80, 0x60, 2);
}
