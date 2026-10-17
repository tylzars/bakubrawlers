#include "System/InputManager.hpp"
#include "System/SoundManager.hpp"
#include "System/TouchManager.hpp"
#include "Graphics/SpriteManager.hpp"
#include "System/WirelessManager.hpp"
#include "Text/TextManager.hpp"
#include "Scene/ArenaScene.hpp"
#include "Game/UnkStruct_020d433c.hpp"
#include "Game/SaveData.hpp"
#include "Graphics/CellAnimation.hpp"
#include "Graphics/Display.hpp"
#include "System/FileSystem.hpp"
#include "UI/MessageWindow.hpp"
#include "UI/SelectWindow.hpp"
#include "Unknown/UnkFunctions.hpp"
#include "Graphics/AntennaIndicator.hpp"
#include <nitro/gx.h>
#include <nitro/mi.h>
#include <nitro/os.h>

struct UnkStruct_020c4230 {
    /* 00 */ PAD(0x00, 0x20);
    /* 20 */ u16 mUnk_20;
};

struct UnkStruct_020c3938 {
    /* 00 */ PAD(0x00, 0x50);
    /* 50 */ u16 mName[12];
    /* 68 */ u16 mUnk_68;
    /* 6a */ PAD(0x6a, 0xc0);
};
extern UnkStruct_020c3938 data_020c3938[];
extern u8 data_020d43eb[];
extern u8 data_020d441a[];
extern "C" void MIi_CpuCopy16(const void *src, void *dest, u32 size);
extern "C" void MI_CpuCopy8(const void *src, void *dest, u32 size);
void func_020233f8(UnkStruct_020d433c *data);
void func_0202330c(UnkStruct_020d433c *data);

#define MINU(a, b) ((u32) (b) > (u32) (a) ? (u32) (a) : (u32) (b))
extern UnkStruct_020c4230 data_020c4230;


static const WindowItemRecord data_ov001_02226fdc[2][2] = {
    {
        {0, 5, 169, 170, 52, 20, 0, {-1, -1, -1, -1}, 0, 1},
        {1, 2, 225, 170, 30, 20, 0, {-1, -1, -1, -1}, 0, 1},
    },
    {
        {0, 5, 169, 200, 0, 0, 0, {-1, -1, -1, -1}, 0, 1},
        {1, 2, 225, 170, 30, 20, 0, {-1, -1, -1, -1}, 0, 1},
    },
};
static const WindowItemRecord data_ov001_02226e8c[2] = {
    {0, 1, 89, 152, 78, 20, 0, {0, 0, 0, 0}, 0, 1},
    {1, 0x8000, 120, 18, 78, 20, 0, {-1, -1, -1, -1}, 0, 0},
};
static const WindowCellRecord data_ov001_02226f88[3] = {
    {"/Game/2D/union_b/union_b_obj_window_nce.bin", 3, 0xc000, 7, 0, 1, {3, 5, 4, 0}},
    {"/Game/2D/union_b/union_b_obj_window_nce.bin", 7, 0xc000, 7, 0, 1, {6, 8, 7, 0}},
    {"/Game/2D/union_b/union_b_obj_window_nce.bin", 10, 0xc000, 7, 0, 1, {10, 10, 10, 0}},
};
static const WindowCellRecord data_ov001_02226e54[2] = {
    {"/Game/2D/union_b/union_b_obj_cockpit_nce.bin", 0, 0xb000, 6, 2, 1, {0, 1, 1, 0}},
    {"/Game/2D/union_b/union_b_obj_cockpit_nce.bin", 2, 0xb000, 6, 2, 1, {2, 3, 3, 0}},
};
static const WindowItemRecord data_ov001_02226f34[3] = {
    {0, 1, 45, 152, 78, 20, 0, {0, 0, 1, 1}, 0, 2},
    {1, 1, 133, 152, 78, 20, 0, {1, 1, 0, 0}, 0, 2},
    {2, 0x8000, 120, 18, 78, 20, 0, {-1, -1, -1, -1}, 0, 0},
};
static const WindowCellRecord data_ov001_02226ec4[2] = {
    {"/Game/2D/union_b/union_b_obj_window_nce.bin", 1, 0xc000, 7, 0, 1, {0, 2, 1, 0}},
    {"/Game/2D/union_b/union_b_obj_window_nce.bin", 9, 0xc000, 7, 0, 1, {9, 9, 9, 0}},
};
static const WindowItemRecord data_ov001_02226efc[2] = {
    {0, 1, 32, 48, 192, 40, 0, {1, 1, 0, 0}, 0, 2},
    {1, 1, 32, 96, 192, 40, 0, {0, 0, 1, 1}, 0, 2},
};

ArenaScene::ArenaScene() : SceneBase(1000, 0) {
    mGlyphBuffer = 0;
    mTextBufferSize = 0;
    mGlyphCount = 0;
    mMessages = 0;
    mUnk_8c = 0;
    mUnk_90 = 0;
    mUnk_94 = 0;
    mUnk_98 = 0;
    mCursor = 0;
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
    delete mCursor;
    if (mMenu != NULL) {
        delete mMenu;
    }
    if (mSubMenu != NULL) {
        delete mSubMenu;
    }
    if (data_020d433c.mUnk_10 != NULL) {
        data_020d433c.mUnk_10->mFinished = 1;
        data_020d433c.mUnk_10 = NULL;
    }
}

void ArenaScene::func_ov001_022197ec() {
    if (mMode < 2) {
        return;
    }
    WirelessManager::Get().Stop();
    data_020d433c.mWireless = 0;
    mUnk_b3 = 1;
    func_ov001_0221c5b0();
    func_ov001_0221c1a0();
    mMode = 2;
    mVariant = 0;
    func_ov001_0221b994();
    if (data_020d433c.mUnk_10 != NULL) {
        data_020d433c.mUnk_10->mFinished = 1;
        data_020d433c.mUnk_10 = NULL;
    }
    if (mSubMenuMode != 0) {
        func_ov001_0221993c(0);
    }
    SpriteManager::Get().StartFade(2);
}

void ArenaScene::func_ov001_0221993c(u32 mode) {
    if (mSubMenuMode == mode) {
        return;
    }
    mSubMenuMode = mode;
    switch (mSubMenuMode) {
        case 1:
            SoundManager::Get().StartSeqArc(1);
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
            SoundManager::Get().StartSeqArc(2);
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

void ArenaScene::Update() {
    func_ov001_0221bee8();
    WirelessManager::Get();
    const u8 *modePtr = &data_020c4258;
    if (IsWirelessMode(modePtr)) {
        WirelessManager::Get();
        data_0220f540.mUnk_01 = data_020c4230.mUnk_20;
        WirelessManager::Get().QueueSend(&data_0220f540);
        WirelessManager::Get().func_0202f078(&data_0220f540);
    }
    WirelessManager::Get();
    u8 mode = *modePtr;
    if (mUnk_b3 != 0) {
        if (mode != 0) {
            if (mUnk_b4 == 0) {
                return;
            }
            func_ov001_0221caa0();
            return;
        }
        mUnk_b3 = 0;
    }
    if (mode == 9) {
        if (mMode == 10) {
            mUnk_b4 = 1;
        }
        func_ov001_022197ec();
        func_ov001_0221993c(2);
    } else if (mode > 1) {
        if (mMode < 0xc) {
            MIi_CpuCopy16(data_020d441a, (u8 *) &data_0220f540 + 4, 0x18);
            data_0220f540.mUnk_1f = 0;
        }
    }
    if (mSubMenuMode != 0) {
        if ((u8) mSubMenu->Update(1)) {
            if ((u8) mSubMenu->mCursor->mSelection == 0) {
                func_ov001_022197ec();
            }
            func_ov001_0221993c(0);
        }
        if (mSubMenu == NULL) {
            return;
        }
        func_ov001_0221caa0();
        mSubMenu->Draw();
        return;
    }
    switch (mMode) {
        case 0:
            func_ov001_0221b06c();
            func_ov001_0221b24c();
            func_ov001_0221c008();
            func_ov001_0221c710();
            func_ov001_0221b994();
            SpriteManager::Get().StartFade(2);
            mMode++;
            break;
        case 1:
            if (SpriteManager::Get().IsFading()) {
                break;
            }
            mMode++;
            break;
        case 2: {
            mUnk_b0 = func_ov001_0221bca0();
            if (mUnk_b0 == 0) {
                break;
            }
            u8 v;
            if (mUnk_b0 == 1) {
                mSlots[mSelection].mAnim->SetAnimation(mSlots[mSelection].mFrame + 2);
                v = 0x14;
            } else {
                v = 0;
            }
            mUnk_ae = v;
            mMode++;
            break;
        }
        case 3:
            if (mUnk_ae != 0) {
                mUnk_ae--;
                break;
            }
            mSlots[0].mUnk_08 = 0x108;
            mSlots[1].mUnk_08 = -200;
            mUnk_ae           = 10;
            mMode++;
            break;
        case 4:
            if (mUnk_ae != 0) {
                mUnk_ae--;
                break;
            }
            switch (mUnk_b0) {
                case 1:
                    switch (mVariant) {
                        case 0:
                            if (mSelection == 0) {
                                mMode = 0xe;
                            } else {
                                mVariant = 1;
                                func_ov001_0221b994();
                                mMode = 2;
                            }
                            break;
                        case 1:
                            if (mode == 0) {
                                WirelessManager::Get().Start();
                                data_020d433c.mWireless = 1;
                            }
                            data_0220f540.mUnk_00 = 0xb;
                            if (mSelection == 0) {
                                mVariant = 2;
                                mMode    = 5;
                            } else {
                                mVariant = 3;
                                mMode    = 8;
                            }
                            data_020d433c.mUnk_10 = new AntennaIndicator(1);
                            MI_CpuClearFast((void *) 0x6600000, 0x2000);
                            func_ov001_0221c1a0();
                            mPlayerCount = 0;
                            func_ov001_0221b994();
                            break;
                    }
                    break;
                case 2:
                    switch (mVariant) {
                        case 0:
                            mMode = 0xf;
                            break;
                        default:
                            func_ov001_022197ec();
                            break;
                    }
                    break;
            }
            break;
        case 5:
            if (mode == 1) {
                u16 name[13];
                name[12] = 0xfffc;
                for (u8 i = 0; i < 12; i++) {
                    name[i] = gSaveData[0].mName[i];
                }
                WirelessManager::Get().StartParent(3, name, 0x1a, 0);
                mMode++;
            }
            break;
        case 6: {
            u8 menu = func_ov001_0221bdf0();
            if (menu == 2) {
                func_ov001_0221be1c();
                func_ov001_0221993c(1);
                return;
            }
            u8 count = 1;
            for (u8 i = 0; i < 4; i++) {
                u8 *p = WirelessManager::Get().func_0202f0cc(i);
                if (p != NULL) {
                    count = i + 1;
                    MIi_CpuCopy16(p + 4, data_020d433c.mNames[i], 0x18);
                } else if (i != 0) {
                    MI_CpuClear16(data_020d433c.mNames[i], 0x18);
                }
            }
            mPlayerCount = count;
            func_ov001_0221c1d8();
            if (mPlayerCount >= 2) {
                if (mMenu->mButtons[0].mEnabled == 0) {
                    mMenu->mButtons[0].mEnabled = 1;
                    mMessage->StartMessage(2, 0, 1);
                }
            } else {
                if (mMenu->mButtons[0].mEnabled != 0) {
                    mMenu->mButtons[0].mEnabled = 0;
                    mMessage->StartMessage(0x10, 0, 1);
                }
            }
            if (InputManager::Get().IsHeld(1) || menu == 1) {
                if (mPlayerCount >= 2) {
                    SoundManager::Get().StartSeqArc(0);
                    WirelessManager::Get().func_0202f184();
                    mUnk_b6 = 10;
                    mMode   = 7;
                }
            }
            if (menu != 0) {
                mMenu->Reset(-1);
                mMenu->mCursor->mSelection = -1;
            }
            if (menu & (mMode == 6)) {
                func_ov001_0221be1c();
            }
            break;
        }
        case 7:
            if (mUnk_b6-- != 0) {
                break;
            }
            data_0220f540.mUnk_1f = 1;
            mMode                 = 0xb;
            break;
        case 8:
            if (mode >= 1) {
                WirelessManager::Get().StartScan();
                mMode++;
            }
            break;
        case 9: {
            mSlots[2].mVisible = 0;
            u32 menu           = (u8) func_ov001_0221bdf0();
            if (menu == 2) {
                func_ov001_0221be1c();
                func_ov001_0221993c(1);
                return;
            }
            u8 count = WirelessManager::Get().TakeScanSnapshot() <= 4 ? WirelessManager::Get().TakeScanSnapshot() : 4;
            if (count != 0) {
                mMenu->mButtons[0].mEnabled = 1;
                u8 i = 0;
                do {
                    WirelessManager::Get();
                    UnkStruct_020c3938 *e = &data_020c3938[i];
                    if (e != NULL && e->mUnk_68 == 0xfffc) {
                        MIi_CpuCopy16(e->mName, data_020d433c.mNames[i], 0x18);
                    }
                    i++;
                } while (i < count);
                mPlayerCount = count;
                if (mPlayerCount != 0) {
                    int sel = mSelection;
                    if (sel > mPlayerCount - 1) {
                        sel = mPlayerCount - 1;
                    }
                    mSelection = sel;
                    if (mPlayerCount > 1) {
                        u8 old = mSelection;
                        if (InputManager::Get().IsTrigger(KEY_UP)) {
                            mSelection = (mSelection + mPlayerCount - 1) % mPlayerCount;
                        } else if (InputManager::Get().IsTrigger(KEY_DOWN)) {
                            mSelection = (mSelection + 1) % mPlayerCount;
                        }
                        if (old != mSelection) {
                            SoundManager::Get().StartSeqArc(5);
                        }
                    }
                    u32 touched = 0;
                    if (TouchManager::Get().mPressed) {
                        if (TouchManager::Get().IsPressedIn(0x20, 0x30, 0xc0, (s16) (mPlayerCount * 0x18))) {
                            mSelection = MINU(mPlayerCount - 1, (u32) (TouchManager::Get().mY - 0x30) / 0x18);
                        }
                    } else if (TouchManager::Get().mTouching) {
                        if (TouchManager::Get().IsReleasedIn(0x20, 0x30, 0xc0, (s16) (mPlayerCount * 0x18))) {
                            mSelection = MINU(mPlayerCount - 1, (u32) (TouchManager::Get().mY - 0x30) / 0x18);
                            touched = 1;
                        }
                    }
                    if (menu == 1) {
                        touched = 1;
                    }
                    mSlots[2].mVisible = 1;
                    mSlots[2].mUnk_08  = 0x20;
                    mSlots[2].mUnk_0a  = mSelection * 0x18 + 0x34;
                    if (touched) {
                        SoundManager::Get().StartSeqArc(0);
                        mMessage->StartMessage(4, 1, 1);
                        u16 name[13];
                        name[12] = 0xfffc;
                        for (u8 i = 0; i < 12; i++) {
                            name[i] = gSaveData[0].mName[i];
                        }
                        WirelessManager::Get().StartJoin(mSelection, name, 0x1a);
                        mPlayerCount = 0;
                        func_ov001_0221c5b0();
                        mSlots[2].mVisible = 0;
                        mSlots[2].mFrame   = 0xd;
                        mMode++;
                    }
                }
            } else {
                mMenu->mButtons[0].mEnabled = 0;
                func_ov001_0221c5b0();
                mSelection = 0;
            }
            func_ov001_0221c1d8();
            if (menu != 0) {
                func_ov001_0221be1c();
            }
            break;
        }
        case 10: {
            mMenu->mButtons[0].mEnabled = 0;
            u8 menu                     = func_ov001_0221bdf0();
            if (menu == 2) {
                func_ov001_0221be1c();
                func_ov001_022197ec();
                func_ov001_0221993c(2);
                return;
            }
            u8 flag  = 0;
            u8 count = 1;
            for (u8 i = 0; i < 4; i++) {
                u8 *p = WirelessManager::Get().func_0202f0cc(i);
                if (p != NULL) {
                    count = i + 1;
                    MIi_CpuCopy16(p + 4, data_020d433c.mNames[i], 0x18);
                    if (i == 0) {
                        flag = p[0x1f];
                    }
                } else if (i != 0) {
                    MI_CpuClear16(data_020d433c.mNames[i], 0x18);
                }
            }
            mPlayerCount = count;
            func_ov001_0221c1d8();
            if (flag != 0) {
                func_020233f8(&data_020d433c);
                mMode = 0xb;
            }
            break;
        }
        case 11:
            if (func_ov001_0221c614(0xb) != 0) {
                mMode++;
            }
            data_0220f540.mUnk_1f = 1;
            break;
        case 12:
            if (func_ov001_0221c614(0xc) != 0) {
                for (u8 i = 0; i < 4; i++) {
                    u8 *p = WirelessManager::Get().func_0202f0cc(i);
                    if (p != NULL) {
                        MIi_CpuCopy16(p + 4, data_020d433c.mNames[i], 0x18);
                    }
                }
                if (mode == 6) {
                    func_0202330c(&data_020d433c);
                    func_020233f8(&data_020d433c);
                }
                mMode++;
            }
            MIi_CpuCopy16(data_020d441a, (u8 *) &data_0220f540 + 4, 0x18);
            break;
        case 13:
            MI_CpuCopy8(data_020d43eb, (u8 *) &data_0220f540 + 4, 4);
            if (func_ov001_0221c614(0x18) != 0) {
                WirelessManager::Get();
                if (data_020c4230.mUnk_20 != 0) {
                    MI_CpuCopy8(WirelessManager::Get().func_0202f0cc(0) + 4, data_020d43eb, 4);
                }
                mMode++;
            }
            break;
        case 14:
            mUnk_b5 = 1;
            SpriteManager::Get().StartFade(3);
            mMode = 0x10;
            break;
        case 15:
            mUnk_b5 = 2;
            SpriteManager::Get().StartFade(3);
            mMode = 0x10;
            break;
        case 16:
            if (SpriteManager::Get().IsFading()) {
                break;
            }
            mMode++;
            break;
        case 17:
            RequestScene(mUnk_b5);
            mFinished = 1;
            return;
    }
    func_ov001_0221bf40();
}

void ArenaScene::func_ov001_0221b06c() {
    GX_SetDispSelect(1);
    SpriteManager::Get().SetupMode(1, 1);
    GX_SetGraphicsMode(1, 0, 0);
    GXS_SetGraphicsMode(0);
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

void ArenaScene::func_ov001_0221b24c() {
    if (data_020d433c.mUnk_10 != NULL) {
        data_020d433c.mUnk_10->SetupLayout(1);
    }
    ChangeDir("/Game/Battle/bg/");
    LoadAndQueueUpload(0, 1, 0, "select_arena_u_bg_00_ncl.bin", 0, 0, 0);
    LoadAndQueueUpload(0, 11, 0, "select_arena_u_bg_00_ncg.bin", 0, 0, 0);
    LoadAndQueueUpload(0, 7, 0, "select_arena_u_bg_00_nsc.bin", 0, 0, 0);
    LoadAndQueueUpload(1, 1, 0, "select_arena_b_bg_00_ncl.bin", 0, 0, 0);
    LoadAndQueueUpload(1, 11, 0, "select_arena_b_bg_00_ncg.bin", 0, 0, 0);
    LoadAndQueueUpload(1, 7, 0, "select_arena_b_bg_00_nsc.bin", 0, 0, 0);
    LoadAndQueueUpload(1, 1, 0x2000, "select_wireless_b_bg_00_ncl.bin", 0, 0, 0);
    LoadAndQueueUpload(1, 12, 0, "select_wireless_b_bg_00_ncg.bin", 0, 0, 0);
    LoadAndQueueUpload(1, 8, 0, "select_wireless_b_bg_00_nsc.bin", 0, 0, 0);
    ChangeDir("/Game/2D/union_b/");
    LoadAndQueueUpload(1, 1, 0x4000, "union_b_bg_window_ncl.bin", 0, 0, 0);
    LoadAndQueueUpload(1, 13, 0, "union_b_bg_window_ncg.bin", 0, 0, 0);
    LoadAndQueueUpload(1, 9, 0, "union_b_bg_window_b_nsc.bin", 0, 0, 0);
    ChangeDir("/Game/Battle/obj/");
    SpriteScreen &sub = SpriteManager::Get().Screen(1);
    sub.SetObjVramMode(3);
    sub.LoadCharacters("select_wireless_b_obj_00.nce_ncg1D.bin", 0x4000, 0);
    sub.LoadPalette("select_wireless_b_obj_00_ncl.bin", 0x20, 0);
    if (mSlots[0].mAnim != NULL) {
        delete mSlots[0].mAnim;
    }
    mSlots[0].mAnim            = new CellAnimation("select_wireless_b_obj_00_nce.bin", 0, 1, 1);
    mSlots[0].mAnim->mCharBase = sub.GetCharBase(0x4000);
    mSlots[0].mAnim->mPaletteBase   = 1;
    mSlots[0].mX               = 0x20;
    mSlots[0].mY               = 0x30;
    mSlots[0].mUnk_08          = 0x20;
    mSlots[0].mUnk_0a          = 0x30;
    if (mSlots[1].mAnim != NULL) {
        delete mSlots[1].mAnim;
    }
    mSlots[1].mAnim            = new CellAnimation("select_wireless_b_obj_00_nce.bin", 0, 1, 1);
    mSlots[1].mAnim->mCharBase = sub.GetCharBase(0x4000);
    mSlots[1].mAnim->mPaletteBase   = 1;
    mSlots[1].mX               = 0x20;
    mSlots[1].mY               = 0x60;
    mSlots[1].mUnk_08          = 0x20;
    mSlots[1].mUnk_0a          = 0x60;
    if (mSlots[2].mAnim != NULL) {
        delete mSlots[2].mAnim;
    }
    mSlots[2].mAnim            = new CellAnimation("select_wireless_b_obj_00_nce.bin", 12, 1, 1);
    mSlots[2].mAnim->mCharBase = sub.GetCharBase(0x4000);
    mSlots[2].mAnim->mPaletteBase   = 1;
    mSlots[2].mX               = 0x70;
    mSlots[2].mY               = 0x60;
    mSlots[2].mUnk_08          = 0x70;
    mSlots[2].mUnk_0a          = 0x60;
    ChangeDir("/Game/Battle/Obj/");
    SpriteScreen &main = SpriteManager::Get().Screen(0);
    main.SetObjVramMode(3);
    main.LoadCharacters("select_arena_u_obj_00.nce_ncg1D.bin", 0, 0);
    main.LoadPalette("select_arena_u_obj_00_ncl.bin", 0, 0);
    if (mMain.mAnim != NULL) {
        delete mMain.mAnim;
    }
    mMain.mAnim            = new CellAnimation("select_arena_u_obj_00_nce.bin", 1, 1, 1);
    mMain.mAnim->mCharBase = main.GetCharBase(0);
    mMain.mAnim->mPaletteBase   = 0;
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
    delete mCursor;
    mCursor = new SelectWindowCursor(2, data_ov001_02226efc, 0);
    if (mMessage != NULL) {
        delete mMessage;
    }
    mMessage = new MessageWindow("mes_BattleArenaMenu", 1, 0x2000);
    mMessage->func_02065bd4(0x14, 4, 1);
    mMessage->SetWidth(0xd8);
}

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
            mMessage->StartMessage(0, 0, 1);
            break;
        case 1:
            G2S_SetBG0Priority(2);
            mSlots[0].mFrame = 6;
            mSlots[1].mFrame = 9;
            mMessage->StartMessage(1, 0, 1);
            break;
        case 2:
            G2S_SetBG0Priority(3);
            mMessage->StartMessage(0x10, 0, 1);
            break;
        case 3:
            G2S_SetBG0Priority(3);
            mSlots[2].mFrame = 12;
            mMessage->StartMessage(3, 0, 1);
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
        SelectWindowCursor *p = mCursor;
        p->mUnk_05        = -1;
        p->mUnk_07        = -1;
        p->mUnk_08        = 0;
        mCursor->mSelection = mSelection;
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
    mMenu->mEnabled                  = 0;
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

u32 ArenaScene::func_ov001_0221bca0() {
    u32 result = 0;
    u8 key     = mCursor->func_0202f468(0);
    u8 oldSel  = mSelection;
    u8 newSel  = mCursor->mSelection;
    u8 menu    = func_ov001_0221bdf0();
    if (oldSel != newSel) {
        mSelection = newSel;
        if (newSel < 2) {
            func_ov001_0221bc44();
            SoundManager::Get().StartSeqArc(5);
        }
    }
    if (menu == 1) {
        key = 1;
    }
    switch (key) {
        case 3:
            if (oldSel != newSel) {
                break;
            }
        case 1:
            result = 1;
            SoundManager::Get().StartSeqArc(0);
            break;
    }
    if (result == 0 && menu == 2) {
        result = 2;
        SoundManager::Get().StartSeqArc(1);
    }
    return result;
}

u32 ArenaScene::func_ov001_0221bdf0() {
    if (mMenu == NULL) {
        return 0;
    }
    mMenu->mEnabled = 1;
    return mMenu->Update(0);
}

void ArenaScene::func_ov001_0221be1c() {
    u8 state;
    do {
        mMenu->Draw();
        mMenu->Update(0);
        state = mMenu->mState;
    } while (!((state <= 2) | (state == 4)));
    mMenu->Reset(-1);
    mMenu->mCursor->mSelection = -1;
    mMenu->mEnabled            = 0;
    for (u8 i = 0; i < 2; i++) {
        mMenu->mCells[i]->SetAnimation(data_ov001_02226e54[((const WindowItem *) data_ov001_02226fdc)[i].mId].mAnimIndices[0]);
    }
}

void ArenaScene::func_ov001_0221bee8() {
    if (mMessage != NULL) {
        mMessage->Update();
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

void ArenaScene::func_ov001_0221c1d8() {
    func_ov001_0221c1a0();
    for (u8 i = 0; i < 4; i++) {
        s16 height;
        s16 width;
        width = 0;
        if (i < mPlayerCount) {
            TextManager &text = TextManager::Get();
            text.mUse4bppFont = 1;
            height = 0x1c;
            TextManager::Get().Print(&data_020d433c.mNames[i], 0x18, mBuffers[i], 0x20, 2, &height, &width, 0, 0, 2, 0x21, 0x10,
                                     0, 0, 0, 0, 1);
            TextManager::Get().mUse4bppFont = 0;
        }
        height = 0;
        width  = 0;
        TextManager::Get().PrintNumber(i + 1, 0, 0, mBuffers[i], 0x20, 2, &height, &width, 0, 2, 0x21, 0, 0, 0, 1);
        DC_FlushRange(mBuffers[i], mTextBufferSize);
        QueueUpload(SpriteManager::Get().Screen(1).mBgIndex, 4, i << 11, mBuffers[i], mTextBufferSize, 0);
    }
}

void ArenaScene::func_ov001_0221c484() {
    SpriteManager::Get();
    for (u8 i = 0; i < 4; i++) {
        SpriteScreen &screen = SpriteManager::Get().Screen(1);
        screen.AddOam(mGlyphBuffer, (u8) mGlyphCount, 0x24, (s16) (i * 0x18 + 0x38), 0, (u16) (u32) screen.GetCharBase(i << 11), 0xf,
                             0);
    }
}

void ArenaScene::func_ov001_0221c5b0() {
    for (u8 i = 0; i < 4; i++) {
        for (u8 j = 0; j < 12; j++) {
            data_020d433c.mNames[i][j] = gSaveData[0].mName[j];
        }
    }
    mPlayerCount = 0;
}

u32 ArenaScene::func_ov001_0221c614(u32 unk1) {
    data_0220f540.mUnk_00 = unk1;
    u32 needed    = WirelessManager::Get().func_0202f134();
    u8 count      = 0;
    for (u8 i = 0; i < 4; i++) {
        u8 *p = WirelessManager::Get().func_0202f0cc(i);
        if (p != NULL && *p == unk1) {
            count++;
        }
    }
    return count >= needed;
}

void ArenaScene::func_ov001_0221c710() {
    SpriteManager::Get().Screen(1).SetObjVramMode(3);
    ChangeDir("/");
    mMessages = (MessageFile *) LoadFile("font/mes_BattleArenaMenu.mes", 0, 0, 0, 0, 0);
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
    QueueUpload(SpriteManager::Get().Screen(1).mBgIndex, 4, 0xe000, mUnk_8c, mUnk_94, 0);
}

void ArenaScene::func_ov001_0221caa0() {
    SpriteManager::Get().Screen(1).AddOam(mUnk_90, (u8) mUnk_98, 0x20, 0x28, 0, 0x1c0, 0xf, 0);
}
