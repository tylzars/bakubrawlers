#include "Scene/ArenaScene.hpp"
#include "Game/UnkStruct_020d433c.hpp"
#include "Game/UnkStruct_020d43f0.hpp"
#include "Graphics/CellAnimation.hpp"
#include "Graphics/Display.hpp"
#include "Graphics/SpriteManager.hpp"
#include "System/FileSystem.hpp"
#include "System/SoundManager.hpp"
#include "Text/TextManager.hpp"
#include "UI/MessageWindow.hpp"
#include "UI/SelectWindow.hpp"
#include "Unknown/UnkFunctions.hpp"
#include "Unknown/UnkStruct_02078290.hpp"
#include "Unknown/UnkStruct_0202f420.hpp"
#include "Unknown/UnkStruct_020c3030.hpp"
#include <nitro/gx.h>
#include <nitro/mi.h>
#include <nitro/os.h>

ArenaScene::ArenaScene() : SceneBase(1000, 0) {
    mGlyphBuffer = 0;
    mTextBufferSize = 0;
    mGlyphCount = 0;
    mMessages = 0;
    mUnk_8c = 0;
    mUnk_90 = 0;
    mUnk_94 = 0;
    mUnk_98 = 0;
    mUnk_9c = 0;
    mMenu = 0;
    mSubMenu = 0;
    mMessage = 0;
    mPlayerCount = 0;
    mMode = 0;
    mUnk_ae = 0;
    mVariant = 0;
    mUnk_b0 = 0;
    mSelection = 0;
    mSubMenuMode = 0;
    mUnk_b3 = 0;
    mUnk_b4 = 0;
    mUnk_b5 = 0;
    data_020d433c.mUnk_10 = 0;
    MI_CpuClear32(&mMain, sizeof(mMain));
    MI_CpuClear32(mSlots, sizeof(mSlots));
    MI_CpuClear32(mBuffers, sizeof(mBuffers));
    for (u8 i = 0; i < 4; i++) {
        data_020d433c.mUnk_a9[i] = 0;
    }
    func_ov001_0221c5b0();
    data_020d433c.mPlayerCount = 2;
}

ArenaScene::~ArenaScene() {
    if (mMain.mAnim != NULL) {
        delete mMain.mAnim;
    }
    for (u8 i = 0; i < 3; i++) {
        if (mSlots[i].mAnim != NULL) {
            delete mSlots[i].mAnim;
        }
    }
    for (u8 i = 0; i < 4; i++) {
        if (mBuffers[i] != NULL) {
            OS_Free(mBuffers[i]);
        }
    }
    delete (u8 *) mGlyphBuffer;
    if (mUnk_90 != NULL) {
        OS_Free(mUnk_90);
    }
    if (mUnk_8c != NULL) {
        OS_Free(mUnk_8c);
    }
    if (mMessage != NULL) {
        delete mMessage;
    }
    delete (u8 *) mMessages;
    delete mUnk_9c;
    if (mMenu != NULL) {
        delete mMenu;
    }
    if (mSubMenu != NULL) {
        delete mSubMenu;
    }
    if (data_020d433c.mUnk_10 != NULL) {
        data_020d433c.mUnk_10->mUnk_04 = 1;
        data_020d433c.mUnk_10 = NULL;
    }
}

void ArenaScene::func_ov001_022197ec() {
    if (mMode < 2) {
        return;
    }
    UnkStruct_020c3030::Get().func_0202edd0();
    data_020d433c.mUnk_a8 = 0;
    mUnk_b3 = 1;
    func_ov001_0221c5b0();
    func_ov001_0221c1a0();
    mMode = 2;
    mVariant = 0;
    func_ov001_0221b994();
    if (data_020d433c.mUnk_10 != NULL) {
        data_020d433c.mUnk_10->mUnk_04 = 1;
        data_020d433c.mUnk_10 = NULL;
    }
    if (mSubMenuMode != 0) {
        func_ov001_0221993c(0);
    }
    SpriteManager::Get().StartFade(2);
}

extern const WindowItemRecord data_ov001_02226f34[];
extern const WindowCellRecord data_ov001_02226f88[];
extern const WindowItemRecord data_ov001_02226e8c[];
extern const WindowCellRecord data_ov001_02226ec4[];
void ArenaScene::func_ov001_0221993c(u32 mode) {
    if (mSubMenuMode == mode) {
        return;
    }
    mSubMenuMode = mode;
    switch (mSubMenuMode) {
        case 1:
            SoundManager::Get().func_020246f8(1);
            G2S_SetBG2Priority(0);
            if (mSubMenu != NULL) {
                delete mSubMenu;
            }
            mSubMenu = new SelectWindow(3, data_ov001_02226f34, -1);
            mSubMenu->SetCells(3, data_ov001_02226f88);
            mSubMenu->mCursor->mSelection = -1;
            mSubMenu->mCursor->mSelection = 1;
            func_ov001_0221c934(12);
            break;
        case 2:
            SoundManager::Get().func_020246f8(2);
            G2S_SetBG2Priority(0);
            if (mSubMenu != NULL) {
                delete mSubMenu;
            }
            mSubMenu = new SelectWindow(2, data_ov001_02226e8c, -1);
            mSubMenu->SetCells(2, data_ov001_02226ec4);
            mSubMenu->mCursor->mSelection = -1;
            mSubMenu->mCursor->mSelection = 0;
            func_ov001_0221c934(11);
            break;
        case 0:
            delete mSubMenu;
            mSubMenu = NULL;
            G2S_SetBG2Priority(3);
            break;
    }
}

void ArenaScene::func_ov001_0221b06c() {
    GX_SetDispSelect(1);
    SpriteManager::Get().func_02026dec(1, 1);
    func_02001e68(1, 0, 0);
    func_02001ed0(0);
    G2_SetBG0Control(0, 1, 0, 1, 0);
    G2S_SetBG0Control(0, 1, 0, 1, 0);
    G2S_SetBG1Control(0, 1, 2, 4, 0);
    G2S_SetBG2Control(0, 1, 4, 7, 0);
    G2_SetBG0Offset(0, 0);
    G2S_SetBG0Offset(0, 0);
    G2S_SetBG1Offset(0, 0);
    G2S_SetBG2Offset(0, 0);
    G2_SetBG0Priority(0);
    G2S_SetBG0Priority(2);
    G2S_SetBG1Priority(2);
    G2S_SetBG2Priority(3);
    GX_SetVisiblePlane(0x11);
    GXS_SetVisiblePlane(0x17);
    G2_SetBlendBrightness(0x1f, 0);
    G2S_SetBlendBrightness(0x1f, 0);
    GX_SetVisibleWnd(0);
    G2_SetWndOutsidePlane(0x1f, FALSE);
    GXS_SetVisibleWnd(0);
    G2S_SetWndOutsidePlane(0x1f, FALSE);
    G2_BlendNone();
    G2S_BlendNone();
}

// non-matching: 15 words, the original keeps one zero in a callee-saved register (mov r4, #0) for the stack arguments of the func_02025b64 calls (see docs/decompiling.md)
extern const WindowItemRecord data_ov001_02226efc[];
void ArenaScene::func_ov001_0221b24c() {
    if (data_020d433c.mUnk_10 != NULL) {
        data_020d433c.mUnk_10->func_020785d8(1);
    }
    ChangeDir("/Game/Battle/bg/");
    func_02025b64(0, 1, 0, "select_arena_u_bg_00_ncl.bin", 0, 0, 0);
    func_02025b64(0, 11, 0, "select_arena_u_bg_00_ncg.bin", 0, 0, 0);
    func_02025b64(0, 7, 0, "select_arena_u_bg_00_nsc.bin", 0, 0, 0);
    func_02025b64(1, 1, 0, "select_arena_b_bg_00_ncl.bin", 0, 0, 0);
    func_02025b64(1, 11, 0, "select_arena_b_bg_00_ncg.bin", 0, 0, 0);
    func_02025b64(1, 7, 0, "select_arena_b_bg_00_nsc.bin", 0, 0, 0);
    func_02025b64(1, 1, 0x2000, "select_wireless_b_bg_00_ncl.bin", 0, 0, 0);
    func_02025b64(1, 12, 0, "select_wireless_b_bg_00_ncg.bin", 0, 0, 0);
    func_02025b64(1, 8, 0, "select_wireless_b_bg_00_nsc.bin", 0, 0, 0);
    ChangeDir("/Game/2D/union_b/");
    func_02025b64(1, 1, 0x4000, "union_b_bg_window_ncl.bin", 0, 0, 0);
    func_02025b64(1, 13, 0, "union_b_bg_window_ncg.bin", 0, 0, 0);
    func_02025b64(1, 9, 0, "union_b_bg_window_b_nsc.bin", 0, 0, 0);
    ChangeDir("/Game/Battle/obj/");
    SpriteScreen &sub = SpriteManager::Get().Screen(1);
    sub.func_02027624(3);
    sub.LoadCharacters("select_wireless_b_obj_00.nce_ncg1D.bin", 0x4000, 0);
    sub.LoadPalette("select_wireless_b_obj_00_ncl.bin", 0x20, 0);
    if (mSlots[0].mAnim != NULL) {
        delete mSlots[0].mAnim;
    }
    mSlots[0].mAnim            = new CellAnimation("select_wireless_b_obj_00_nce.bin", 0, 1, 1);
    mSlots[0].mAnim->mCharBase = sub.func_020276f4(0x4000);
    mSlots[0].mAnim->mUnk_11   = 1;
    mSlots[0].mX               = 0x20;
    mSlots[0].mY               = 0x30;
    mSlots[0].mUnk_08          = 0x20;
    mSlots[0].mUnk_0a          = 0x30;
    if (mSlots[1].mAnim != NULL) {
        delete mSlots[1].mAnim;
    }
    mSlots[1].mAnim            = new CellAnimation("select_wireless_b_obj_00_nce.bin", 0, 1, 1);
    mSlots[1].mAnim->mCharBase = sub.func_020276f4(0x4000);
    mSlots[1].mAnim->mUnk_11   = 1;
    mSlots[1].mX               = 0x20;
    mSlots[1].mY               = 0x60;
    mSlots[1].mUnk_08          = 0x20;
    mSlots[1].mUnk_0a          = 0x60;
    if (mSlots[2].mAnim != NULL) {
        delete mSlots[2].mAnim;
    }
    mSlots[2].mAnim            = new CellAnimation("select_wireless_b_obj_00_nce.bin", 12, 1, 1);
    mSlots[2].mAnim->mCharBase = sub.func_020276f4(0x4000);
    mSlots[2].mAnim->mUnk_11   = 1;
    mSlots[2].mX               = 0x70;
    mSlots[2].mY               = 0x60;
    mSlots[2].mUnk_08          = 0x70;
    mSlots[2].mUnk_0a          = 0x60;
    ChangeDir("/Game/Battle/Obj/");
    SpriteScreen &main = SpriteManager::Get().Screen(0);
    main.func_02027624(3);
    main.LoadCharacters("select_arena_u_obj_00.nce_ncg1D.bin", 0, 0);
    main.LoadPalette("select_arena_u_obj_00_ncl.bin", 0, 0);
    if (mMain.mAnim != NULL) {
        delete mMain.mAnim;
    }
    mMain.mAnim            = new CellAnimation("select_arena_u_obj_00_nce.bin", 1, 1, 1);
    mMain.mAnim->mCharBase = main.func_020276f4(0);
    mMain.mAnim->mUnk_11   = 0;
    mMain.mX               = 0x80;
    mMain.mY               = 0x60;
    mMain.mUnk_08          = 0x80;
    mMain.mUnk_0a          = 0x60;
    mMain.mUnk_0c = 0;
    mMain.mVisible = 1;
    ChangeDir("/Game/2D/union_b/");
    SpriteScreen &sub2 = SpriteManager::Get().Screen(1);
    sub2.LoadCharacters("union_b_obj_cockpit.nce_ncg1D.bin", 0xb000, 0);
    sub2.LoadPalette("union_b_obj_cockpit_ncl.bin", 0xc0, 0);
    SpriteScreen &sub3 = SpriteManager::Get().Screen(1);
    sub3.LoadCharacters("union_b_obj_window.nce_ncg1D.bin", 0xc000, 0);
    sub3.LoadPalette("union_b_obj_window_ncl.bin", 0xe0, 0);
    ChangeDir("/");
    delete mUnk_9c;
    mUnk_9c = new UnkStruct_0202f420(2, data_ov001_02226efc, 0);
    if (mMessage != NULL) {
        delete mMessage;
    }
    mMessage = new MessageWindow("mes_BattleArenaMenu", 1, 0x2000);
    mMessage->func_02065bd4(0x14, 4, 1);
    mMessage->func_02065be4(0xd8);
}

extern const WindowItemRecord data_ov001_02226fdc[][2];
extern const WindowCellRecord data_ov001_02226e54[];
void ArenaScene::func_ov001_0221b994() {
    for (u8 i = 0; i < 3; i++) {
        mSlots[i].mVisible = 0;
        mSlots[i].mFrame   = -1;
    }
    mUnk_b0 = 0;
    mSelection = 0;
    if (mVariant == 0) {
        mMain.mAnim->SetAnimation(0);
    } else {
        mMain.mAnim->SetAnimation(1);
    }
    switch (mVariant) {
        case 0:
            G2S_SetBG0Priority(2);
            mSlots[0].mFrame = 0;
            mSlots[1].mFrame = 3;
            mMessage->func_02065890(0, 0, 1);
            break;
        case 1:
            G2S_SetBG0Priority(2);
            mSlots[0].mFrame = 6;
            mSlots[1].mFrame = 9;
            mMessage->func_02065890(1, 0, 1);
            break;
        case 2:
            G2S_SetBG0Priority(3);
            mMessage->func_02065890(0x10, 0, 1);
            break;
        case 3:
            G2S_SetBG0Priority(3);
            mSlots[2].mFrame = 12;
            mMessage->func_02065890(3, 0, 1);
            break;
    }
    for (u8 i = 0; i < 3; i++) {
        if (mSlots[i].mFrame >= 0) {
            mSlots[i].mVisible = 1;
        }
    }
    switch (mVariant) {
        case 0:
        case 1:
        mSlots[0].mX      = -200;
        mSlots[0].mY      = 0x30;
        mSlots[1].mX      = 0x108;
        mSlots[1].mY      = 0x60;
        mSlots[0].mUnk_08 = 0x20;
        mSlots[0].mUnk_0a = 0x30;
        mSlots[1].mUnk_08 = 0x20;
        mSlots[1].mUnk_0a = 0x60;
        UnkStruct_0202f420 *p = mUnk_9c;
        p->mUnk_05        = -1;
        p->mUnk_07        = -1;
        p->mUnk_08        = 0;
        mUnk_9c->mUnk_06  = mSelection;
        func_ov001_0221bc44();
        break;
        case 3:
        mSlots[2].mVisible = 0;
        mSlots[2].mX       = 0x20;
        mSlots[2].mY       = 0x34;
        mSlots[2].mUnk_08  = 0x20;
        mSlots[2].mUnk_0a  = 0x34;
        break;
    }
    u8 sel = mVariant != 2 ? 1 : 0;
    if (mMenu != NULL) {
        delete mMenu;
    }
    mMenu = new SelectWindow(2, data_ov001_02226fdc[sel], 0);
    mMenu->SetCells(2, data_ov001_02226e54);
    mMenu->mCursor->mSelection       = -1;
    mMenu->mUnk_064                  = 0;
    mMenu->mButtons[0].mEnabled      = 0;
}

void ArenaScene::func_ov001_0221bc44() {
    for (u8 i = 0; i < 2; i++) {
        if (mSlots[i].mVisible != 0) {
            if (mSelection == i) {
                mSlots[i].mAnim->SetAnimation(mSlots[i].mFrame + 1);
            } else {
                mSlots[i].mAnim->SetAnimation(mSlots[i].mFrame);
            }
        }
    }
}

// non-matching: 1 word, `subs r0, r6, r7` where the original has `cmp r6, r7` (every spelling of oldSel == newSel tried yields either cmpeq predication or subs)
u32 ArenaScene::func_ov001_0221bca0() {
    u32 result = 0;
    u8 key     = mUnk_9c->func_0202f468(0);
    u8 oldSel  = mSelection;
    u8 newSel  = mUnk_9c->mUnk_06;
    u8 menu    = func_ov001_0221bdf0();
    if (oldSel != newSel) {
        mSelection = newSel;
        if (newSel < 2) {
            func_ov001_0221bc44();
            SoundManager::Get().func_020246f8(5);
        }
    }
    if (menu == 1) {
        key = 1;
    }
    if (key == 1 || (key == 3 && (oldSel - newSel) == 0)) {
        result = 1;
        SoundManager::Get().func_020246f8(0);
    }
    if (result == 0 && menu == 2) {
        result = 2;
        SoundManager::Get().func_020246f8(1);
    }
    return result;
}

u32 ArenaScene::func_ov001_0221bdf0() {
    if (mMenu == NULL) {
        return 0;
    }
    mMenu->mUnk_064 = 1;
    return mMenu->Update(0);
}

extern const WindowCellRecord data_ov001_02226e54[];
void ArenaScene::func_ov001_0221be1c() {
    u8 state;
    do {
        mMenu->Draw();
        mMenu->Update(0);
        state = mMenu->mUnk_061;
    } while (!((state <= 2) | (state == 4)));
    mMenu->func_0205c7c8(-1);
    mMenu->mCursor->mSelection = -1;
    mMenu->mUnk_064            = 0;
    for (u8 i = 0; i < 2; i++) {
        mMenu->mCells[i]->SetAnimation(data_ov001_02226e54[((const WindowItem *) data_ov001_02226fdc)[i].mId].mUnk_18[0]);
    }
}

void ArenaScene::func_ov001_0221bee8() {
    if (mMessage != NULL) {
        mMessage->func_020658d0();
    }
    if (mMain.mVisible == 0) {
        return;
    }
    mMain.mAnim->Update();
    mMain.mAnim->Draw(0, mMain.mX, mMain.mY, mMain.mUnk_0c);
}

void ArenaScene::func_ov001_0221bf40() {
    if ((mVariant == 2) | (mVariant == 3)) {
        func_ov001_0221c484();
    }
    for (u8 i = 0; i < 3; i++) {
        if (mSlots[i].mVisible != 0) {
            mSlots[i].mX = (mSlots[i].mX + mSlots[i].mUnk_08) / 2;
            mSlots[i].mY = (mSlots[i].mY + mSlots[i].mUnk_0a) / 2;
            mSlots[i].mAnim->Update();
            mSlots[i].mAnim->Draw(1, mSlots[i].mX, mSlots[i].mY, mSlots[i].mUnk_0c);
        }
    }
    mMenu->Draw();
}

void ArenaScene::func_ov001_0221c008() {
    mTextBufferSize = TextManager::Get().BufferSize(0x20, 2);
    for (u8 i = 0; i < 4; i++) {
        mBuffers[i] = OS_Alloc(mTextBufferSize);
    }
    mGlyphCount = TextManager::Get().GlyphCount(8);
    mGlyphBuffer = OS_Alloc(mGlyphCount * 12);
    TextManager::Get().Init(mGlyphBuffer, 0, 0, 0x20, 2, 0x21);
    TextManager::Get().SetColor(0, 0, 0xf);
    MI_CpuClearFast((void *) 0x6600000, 0x2000);
    func_ov001_0221c1d8();
}

void ArenaScene::func_ov001_0221c1a0() {
    for (u8 i = 0; i < 4; i++) {
        MI_CpuClear16(mBuffers[i], mTextBufferSize);
    }
}

// non-matching: 6 words, the original loads the TextManager guard before the mUnk_5b store (see docs/decompiling.md)
void ArenaScene::func_ov001_0221c1d8() {
    func_ov001_0221c1a0();
    for (u8 i = 0; i < 4; i++) {
        s16 height;
        s16 width;
        width = 0;
        if (i < mPlayerCount) {
            TextManager &text = TextManager::Get();
            text.mUnk_5b = 1;
            height = 0x1c;
            TextManager::Get().Print(&data_020d433c.mNames[i], 0x18, mBuffers[i], 0x20, 2, &height, &width, 0, 0, 2, 0x21, 0x10,
                                     0, 0, 0, 0, 1);
            TextManager::Get().mUnk_5b = 0;
        }
        height = 0;
        width  = 0;
        TextManager::Get().PrintNumber(i + 1, 0, 0, mBuffers[i], 0x20, 2, &height, &width, 0, 2, 0x21, 0, 0, 0, 1);
        func_02005e9c(mBuffers[i], mTextBufferSize);
        func_02025c1c(SpriteManager::Get().Screen(1).mBgIndex, 4, i << 11, mBuffers[i], mTextBufferSize, 0);
    }
}

void ArenaScene::func_ov001_0221c484() {
    SpriteManager::Get();
    for (u8 i = 0; i < 4; i++) {
        SpriteScreen &screen = SpriteManager::Get().Screen(1);
        screen.func_02027778(mGlyphBuffer, (u8) mGlyphCount, 0x24, (s16) (i * 0x18 + 0x38), 0, (u16) (u32) screen.func_020276f4(i << 11), 0xf,
                             0);
    }
}

void ArenaScene::func_ov001_0221c5b0() {
    for (u8 i = 0; i < 4; i++) {
        for (u8 j = 0; j < 12; j++) {
            data_020d433c.mNames[i][j] = data_020d43f0[0].mName[j];
        }
    }
    mPlayerCount = 0;
}

u32 ArenaScene::func_ov001_0221c614(u32 unk1) {
    data_0220f540.mUnk_00 = unk1;
    u32 needed    = UnkStruct_020c3030::Get().func_0202f134();
    u8 count      = 0;
    for (u8 i = 0; i < 4; i++) {
        u8 *p = UnkStruct_020c3030::Get().func_0202f0cc(i);
        if (p != NULL && *p == unk1) {
            count++;
        }
    }
    return count >= needed;
}

void ArenaScene::func_ov001_0221c710() {
    SpriteManager::Get().Screen(1).func_02027624(3);
    ChangeDir("/");
    mMessages = (MessageFile *) func_02023504("font/mes_BattleArenaMenu.mes", 0, 0, 0, 0, 0);
    mUnk_94 = TextManager::Get().BufferSize(0x18, 8);
    mUnk_8c = OS_Alloc(mUnk_94);
    MI_CpuClear16(mUnk_8c, mUnk_94);
    mUnk_98 = TextManager::Get().GlyphCount(0x30);
    mUnk_90 = OS_Alloc(mUnk_98 * 12);
    TextManager::Get().Init(mUnk_90, 1, 0, 0x18, 8, 0x11);
    TextManager::Get().SetColor(1, 0, 0xf);
    func_ov001_0221c934(12);
}

void ArenaScene::func_ov001_0221c934(u32 unk1) {
    MI_CpuClear16(mUnk_8c, mUnk_94);
    TextManager &text = TextManager::Get();
    s16 height;
    s16 width;
    width             = 0;
    height            = 0;
    MessageFile *file = mMessages;
    text.Print(file->GetText(unk1), file->GetLength(unk1), mUnk_8c, 0x18, 8, &height, &width, 0, 0, 2, 0x11, 0xe, 0, 0, 1, 1, 1);
    func_02025c1c(SpriteManager::Get().Screen(1).mBgIndex, 4, 0xe000, mUnk_8c, mUnk_94, 0);
}

void ArenaScene::func_ov001_0221caa0() {
    SpriteManager::Get().Screen(1).func_02027778(mUnk_90, (u8) mUnk_98, 0x20, 0x28, 0, 0x1c0, 0xf, 0);
}
