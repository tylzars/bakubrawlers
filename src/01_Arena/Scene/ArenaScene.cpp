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
    /* 20 */ u16 mUnk_20; // 0x020c4250 = WirelessManager::mSendBlockIndex (instance 0x020c3030 + 0x1220)
};

// Private view of WmEntry for the name field: data_020c3938 is WirelessManager::mScanSnapshot (0x020c3030 + 0x908).
struct ScanSnapshotEntry {
    /* 00 */ PAD(0x00, 0x50);
    /* 50 */ u16 mName[12];
    /* 68 */ u16 mUnk_68;
    /* 6a */ PAD(0x6a, 0xc0);
};
extern ScanSnapshotEntry data_020c3938[];
extern u8 data_020d43eb[];
extern u8 data_020d441a[];
extern "C" void MIi_CpuCopy16(const void *src, void *dest, u32 size);
extern "C" void MI_CpuCopy8(const void *src, void *dest, u32 size);
void func_020233f8(UnkStruct_020d433c *data);
void func_0202330c(UnkStruct_020d433c *data);

#define MINU(a, b) ((u32) (b) > (u32) (a) ? (u32) (a) : (u32) (b))
extern UnkStruct_020c4230 data_020c4230;


static const WindowItemRecord sArenaMenuItems[2][2] = {
    {
        {0, 5, 169, 170, 52, 20, 0, {-1, -1, -1, -1}, 0, WINDOW_TOUCH_RELEASE},
        {1, 2, 225, 170, 30, 20, 0, {-1, -1, -1, -1}, 0, WINDOW_TOUCH_RELEASE},
    },
    {
        {0, 5, 169, 200, 0, 0, 0, {-1, -1, -1, -1}, 0, WINDOW_TOUCH_RELEASE},
        {1, 2, 225, 170, 30, 20, 0, {-1, -1, -1, -1}, 0, WINDOW_TOUCH_RELEASE},
    },
};
// Record 1 (WINDOW_ITEM_CURSOR_SKIP) is skipped by the cursor and by touch: one item is selectable.
static const WindowItemRecord sArenaTwoItems[2] = {
    {0, 1, 89, 152, 78, 20, 0, {0, 0, 0, 0}, 0, WINDOW_TOUCH_RELEASE},
    {1, WINDOW_ITEM_CURSOR_SKIP, 120, 18, 78, 20, 0, {-1, -1, -1, -1}, 0, 0},
};
static const WindowCellRecord sArenaThreeItemCells[3] = {
    {"/Game/2D/union_b/union_b_obj_window_nce.bin", 3, 0xc000, 7, 0, 1, {3, 5, 4, 0}},
    {"/Game/2D/union_b/union_b_obj_window_nce.bin", 7, 0xc000, 7, 0, 1, {6, 8, 7, 0}},
    {"/Game/2D/union_b/union_b_obj_window_nce.bin", 10, 0xc000, 7, 0, 1, {10, 10, 10, 0}},
};
static const WindowCellRecord sArenaMenuCells[2] = {
    {"/Game/2D/union_b/union_b_obj_cockpit_nce.bin", 0, 0xb000, 6, 2, 1, {0, 1, 1, 0}},
    {"/Game/2D/union_b/union_b_obj_cockpit_nce.bin", 2, 0xb000, 6, 2, 1, {2, 3, 3, 0}},
};
// Record 2 (WINDOW_ITEM_CURSOR_SKIP) is skipped by the cursor and by touch: two items are selectable.
static const WindowItemRecord sArenaThreeItems[3] = {
    {0, 1, 45, 152, 78, 20, 0, {0, 0, 1, 1}, 0, WINDOW_TOUCH_RELEASE_IF_SELECTED},
    {1, 1, 133, 152, 78, 20, 0, {1, 1, 0, 0}, 0, WINDOW_TOUCH_RELEASE_IF_SELECTED},
    {2, WINDOW_ITEM_CURSOR_SKIP, 120, 18, 78, 20, 0, {-1, -1, -1, -1}, 0, 0},
};
static const WindowCellRecord sArenaTwoItemCells[2] = {
    {"/Game/2D/union_b/union_b_obj_window_nce.bin", 1, 0xc000, 7, 0, 1, {0, 2, 1, 0}},
    {"/Game/2D/union_b/union_b_obj_window_nce.bin", 9, 0xc000, 7, 0, 1, {9, 9, 9, 0}},
};
static const WindowItemRecord sArenaSlotItems[2] = {
    {0, 1, 32, 48, 192, 40, 0, {1, 1, 0, 0}, 0, WINDOW_TOUCH_RELEASE_IF_SELECTED},
    {1, 1, 32, 96, 192, 40, 0, {0, 0, 1, 1}, 0, WINDOW_TOUCH_RELEASE_IF_SELECTED},
};

ArenaScene::ArenaScene() : SceneBase(1000, 0) {
    mGlyphBuffer = 0;
    mTextBufferSize = 0;
    mGlyphCount = 0;
    mMessages = 0;
    mMessageTextBuffer = 0;
    mMessageGlyphBuffer = 0;
    mMessageTextBufferSize = 0;
    mMessageGlyphCount = 0;
    mCursor = 0;
    mMenuWindow = 0;
    mSubMenuWindow = 0;
    mMessageWindow = 0;
    mPlayerCount = 0;
    mState = 0;
    mTimer = 0;
    mVariant = 0;
    mResult = 0;
    mSelection = 0;
    mSubMenuMode = 0;
    mWirelessStopping = 0;
    mStoppingMessageVisible = 0;
    mRequest = 0;
    data_020d433c.mAntenna = 0;
    MI_CpuClear32(&mMain, sizeof(mMain));
    MI_CpuClear32(mSlots, sizeof(mSlots));
    MI_CpuClear32(mBuffers, sizeof(mBuffers));
    for (u8 i = 0; i < 4; i++) {
        data_020d433c.mUnk_a9[i] = 0;
    }
    ResetNames();
    data_020d433c.mPlayerCount = 2;
}

ArenaScene::~ArenaScene() {
    if (mMain.mCellAnim != NULL) {
        delete mMain.mCellAnim;
    }
    for (u8 i = 0; i < 3; i++) {
        if (mSlots[i].mCellAnim != NULL) {
            delete mSlots[i].mCellAnim;
        }
    }
    for (u8 i = 0; i < 4; i++) {
        if (mBuffers[i] != NULL) {
            OS_Free(mBuffers[i]);
        }
    }
    delete (u8 *) mGlyphBuffer;
    if (mMessageGlyphBuffer != NULL) {
        OS_Free(mMessageGlyphBuffer);
    }
    if (mMessageTextBuffer != NULL) {
        OS_Free(mMessageTextBuffer);
    }
    if (mMessageWindow != NULL) {
        delete mMessageWindow;
    }
    delete (u8 *) mMessages;
    delete mCursor;
    if (mMenuWindow != NULL) {
        delete mMenuWindow;
    }
    if (mSubMenuWindow != NULL) {
        delete mSubMenuWindow;
    }
    if (data_020d433c.mAntenna != NULL) {
        data_020d433c.mAntenna->mFinished = 1;
        data_020d433c.mAntenna = NULL;
    }
}

void ArenaScene::StopWirelessAndReset() {
    if (mState < 2) {
        return;
    }
    WirelessManager::Get().Stop();
    data_020d433c.mWireless = 0;
    mWirelessStopping = 1;
    ResetNames();
    ClearNameText();
    mState = 2;
    mVariant = 0;
    SetupVariant();
    if (data_020d433c.mAntenna != NULL) {
        data_020d433c.mAntenna->mFinished = 1;
        data_020d433c.mAntenna = NULL;
    }
    if (mSubMenuMode != 0) {
        SetSubMenuMode(0);
    }
    SpriteManager::Get().StartFade(FADE_MODE_TO_ZERO);
}

void ArenaScene::SetSubMenuMode(u32 mode) {
    if (mSubMenuMode == mode) {
        return;
    }
    mSubMenuMode = mode;
    switch (mSubMenuMode) {
        case 1:
            SoundManager::Get().StartSeqArc(1);
            G2S_SetBG2Priority(0);
            if (mSubMenuWindow != NULL) {
                delete mSubMenuWindow;
            }
            mSubMenuWindow = new SelectWindow(3, sArenaThreeItems, -1);
            mSubMenuWindow->SetCells(3, sArenaThreeItemCells);
            mSubMenuWindow->mCursor->mSelection = -1;
            mSubMenuWindow->mCursor->mSelection = 1;
            PrintMessage(12);
            break;
        case 2:
            SoundManager::Get().StartSeqArc(2);
            G2S_SetBG2Priority(0);
            if (mSubMenuWindow != NULL) {
                delete mSubMenuWindow;
            }
            mSubMenuWindow = new SelectWindow(2, sArenaTwoItems, -1);
            mSubMenuWindow->SetCells(2, sArenaTwoItemCells);
            mSubMenuWindow->mCursor->mSelection = -1;
            mSubMenuWindow->mCursor->mSelection = 0;
            PrintMessage(11);
            break;
        case 0:
            delete mSubMenuWindow;
            mSubMenuWindow = NULL;
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
        WirelessManager::Get().SetSendBlock(&data_0220f540);
    }
    WirelessManager::Get();
    u8 mode = *modePtr;
    if (mWirelessStopping != 0) {
        if (mode != 0) {
            if (mStoppingMessageVisible == 0) {
                return;
            }
            DrawMessage();
            return;
        }
        mWirelessStopping = 0;
    }
    if (mode == 9) {
        if (mState == 10) {
            mStoppingMessageVisible = 1;
        }
        StopWirelessAndReset();
        SetSubMenuMode(2);
    } else if (mode > 1) {
        if (mState < 0xc) {
            MIi_CpuCopy16(data_020d441a, (u8 *) &data_0220f540 + 4, 0x18);
            data_0220f540.mUnk_1f = 0;
        }
    }
    if (mSubMenuMode != 0) {
        if ((u8) mSubMenuWindow->Update(1)) {
            if ((u8) mSubMenuWindow->mCursor->mSelection == 0) {
                StopWirelessAndReset();
            }
            SetSubMenuMode(0);
        }
        if (mSubMenuWindow == NULL) {
            return;
        }
        DrawMessage();
        mSubMenuWindow->Draw();
        return;
    }
    switch (mState) {
        case 0:
            InitDisplay();
            LoadGraphics();
            InitNameText();
            LoadMessages();
            SetupVariant();
            SpriteManager::Get().StartFade(FADE_MODE_TO_ZERO);
            mState++;
            break;
        case 1:
            if (SpriteManager::Get().IsFading()) {
                break;
            }
            mState++;
            break;
        case 2: {
            mResult = UpdateSelection();
            if (mResult == 0) {
                break;
            }
            u8 v;
            if (mResult == 1) {
                mSlots[mSelection].mCellAnim->SetAnimation(mSlots[mSelection].mFrame + 2);
                v = 0x14;
            } else {
                v = 0;
            }
            mTimer = v;
            mState++;
            break;
        }
        case 3:
            if (mTimer != 0) {
                mTimer--;
                break;
            }
            mSlots[0].mTargetX = 0x108;
            mSlots[1].mTargetX = -200;
            mTimer           = 10;
            mState++;
            break;
        case 4:
            if (mTimer != 0) {
                mTimer--;
                break;
            }
            switch (mResult) {
                case 1:
                    switch (mVariant) {
                        case 0:
                            if (mSelection == 0) {
                                mState = 0xe;
                            } else {
                                mVariant = 1;
                                SetupVariant();
                                mState = 2;
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
                                mState    = 5;
                            } else {
                                mVariant = 3;
                                mState    = 8;
                            }
                            data_020d433c.mAntenna = new AntennaIndicator(1);
                            MI_CpuClearFast((void *) 0x6600000, 0x2000);
                            ClearNameText();
                            mPlayerCount = 0;
                            SetupVariant();
                            break;
                    }
                    break;
                case 2:
                    switch (mVariant) {
                        case 0:
                            mState = 0xf;
                            break;
                        default:
                            StopWirelessAndReset();
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
                mState++;
            }
            break;
        case 6: {
            u8 menu = UpdateMenu();
            if (menu == 2) {
                ResetMenu();
                SetSubMenuMode(1);
                return;
            }
            u8 count = 1;
            for (u8 i = 0; i < 4; i++) {
                u8 *p = WirelessManager::Get().GetRecvBlock(i);
                if (p != NULL) {
                    count = i + 1;
                    MIi_CpuCopy16(p + 4, data_020d433c.mNames[i], 0x18);
                } else if (i != 0) {
                    MI_CpuClear16(data_020d433c.mNames[i], 0x18);
                }
            }
            mPlayerCount = count;
            PrintNames();
            if (mPlayerCount >= 2) {
                if (mMenuWindow->mButtons[0].mEnabled == 0) {
                    mMenuWindow->mButtons[0].mEnabled = 1;
                    mMessageWindow->StartMessage(2, 0, 1);
                }
            } else {
                if (mMenuWindow->mButtons[0].mEnabled != 0) {
                    mMenuWindow->mButtons[0].mEnabled = 0;
                    mMessageWindow->StartMessage(0x10, 0, 1);
                }
            }
            if (InputManager::Get().IsPressed(1) || menu == 1) {
                if (mPlayerCount >= 2) {
                    SoundManager::Get().StartSeqArc(0);
                    WirelessManager::Get().func_0202f184();
                    mUnk_b6 = 10;
                    mState   = 7;
                }
            }
            if (menu != 0) {
                mMenuWindow->Reset(-1);
                mMenuWindow->mCursor->mSelection = -1;
            }
            if (menu & (mState == 6)) {
                ResetMenu();
            }
            break;
        }
        case 7:
            if (mUnk_b6-- != 0) {
                break;
            }
            data_0220f540.mUnk_1f = 1;
            mState                 = 0xb;
            break;
        case 8:
            if (mode >= 1) {
                WirelessManager::Get().StartScan();
                mState++;
            }
            break;
        case 9: {
            mSlots[2].mVisible = 0;
            u32 menu           = (u8) UpdateMenu();
            if (menu == 2) {
                ResetMenu();
                SetSubMenuMode(1);
                return;
            }
            u8 count = WirelessManager::Get().TakeScanSnapshot() <= 4 ? WirelessManager::Get().TakeScanSnapshot() : 4;
            if (count != 0) {
                mMenuWindow->mButtons[0].mEnabled = 1;
                u8 i = 0;
                do {
                    WirelessManager::Get();
                    ScanSnapshotEntry *e = &data_020c3938[i];
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
                    } else if (TouchManager::Get().mReleased) {
                        if (TouchManager::Get().IsReleasedIn(0x20, 0x30, 0xc0, (s16) (mPlayerCount * 0x18))) {
                            mSelection = MINU(mPlayerCount - 1, (u32) (TouchManager::Get().mY - 0x30) / 0x18);
                            touched = 1;
                        }
                    }
                    if (menu == 1) {
                        touched = 1;
                    }
                    mSlots[2].mVisible = 1;
                    mSlots[2].mTargetX  = 0x20;
                    mSlots[2].mTargetY  = mSelection * 0x18 + 0x34;
                    if (touched) {
                        SoundManager::Get().StartSeqArc(0);
                        mMessageWindow->StartMessage(4, 1, 1);
                        u16 name[13];
                        name[12] = 0xfffc;
                        for (u8 i = 0; i < 12; i++) {
                            name[i] = gSaveData[0].mName[i];
                        }
                        WirelessManager::Get().StartJoin(mSelection, name, 0x1a);
                        mPlayerCount = 0;
                        ResetNames();
                        mSlots[2].mVisible = 0;
                        mSlots[2].mFrame   = 0xd;
                        mState++;
                    }
                }
            } else {
                mMenuWindow->mButtons[0].mEnabled = 0;
                ResetNames();
                mSelection = 0;
            }
            PrintNames();
            if (menu != 0) {
                ResetMenu();
            }
            break;
        }
        case 10: {
            mMenuWindow->mButtons[0].mEnabled = 0;
            u8 menu                     = UpdateMenu();
            if (menu == 2) {
                ResetMenu();
                StopWirelessAndReset();
                SetSubMenuMode(2);
                return;
            }
            u8 flag  = 0;
            u8 count = 1;
            for (u8 i = 0; i < 4; i++) {
                u8 *p = WirelessManager::Get().GetRecvBlock(i);
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
            PrintNames();
            if (flag != 0) {
                func_020233f8(&data_020d433c);
                mState = 0xb;
            }
            break;
        }
        case 11:
            if (SetMessageTypeAndCheckAll(0xb) != 0) {
                mState++;
            }
            data_0220f540.mUnk_1f = 1;
            break;
        case 12:
            if (SetMessageTypeAndCheckAll(0xc) != 0) {
                for (u8 i = 0; i < 4; i++) {
                    u8 *p = WirelessManager::Get().GetRecvBlock(i);
                    if (p != NULL) {
                        MIi_CpuCopy16(p + 4, data_020d433c.mNames[i], 0x18);
                    }
                }
                if (mode == 6) {
                    func_0202330c(&data_020d433c);
                    func_020233f8(&data_020d433c);
                }
                mState++;
            }
            MIi_CpuCopy16(data_020d441a, (u8 *) &data_0220f540 + 4, 0x18);
            break;
        case 13:
            MI_CpuCopy8(data_020d43eb, (u8 *) &data_0220f540 + 4, 4);
            if (SetMessageTypeAndCheckAll(0x18) != 0) {
                WirelessManager::Get();
                if (data_020c4230.mUnk_20 != 0) {
                    MI_CpuCopy8(WirelessManager::Get().GetRecvBlock(0) + 4, data_020d43eb, 4);
                }
                mState++;
            }
            break;
        case 14:
            mRequest = ARENA_REQUEST_FIELD_SELECT;
            SpriteManager::Get().StartFade(FADE_MODE_DOWN);
            mState = 0x10;
            break;
        case 15:
            mRequest = ARENA_REQUEST_TITLE;
            SpriteManager::Get().StartFade(FADE_MODE_DOWN);
            mState = 0x10;
            break;
        case 16:
            if (SpriteManager::Get().IsFading()) {
                break;
            }
            mState++;
            break;
        case 17:
            RequestScene(mRequest);
            mFinished = 1;
            return;
    }
    func_ov001_0221bf40();
}

void ArenaScene::InitDisplay() {
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

void ArenaScene::LoadGraphics() {
    if (data_020d433c.mAntenna != NULL) {
        data_020d433c.mAntenna->SetupLayout(1);
    }
    ChangeDir("/Game/Battle/bg/");
    LoadAndQueueUpload(0, UPLOAD_KIND_BG_EXT_PLTT, 0, "select_arena_u_bg_00_ncl.bin", 0, 0, 0);
    LoadAndQueueUpload(0, UPLOAD_KIND_BG0_CHAR, 0, "select_arena_u_bg_00_ncg.bin", 0, 0, 0);
    LoadAndQueueUpload(0, UPLOAD_KIND_BG0_SCR, 0, "select_arena_u_bg_00_nsc.bin", 0, 0, 0);
    LoadAndQueueUpload(1, UPLOAD_KIND_BG_EXT_PLTT, 0, "select_arena_b_bg_00_ncl.bin", 0, 0, 0);
    LoadAndQueueUpload(1, UPLOAD_KIND_BG0_CHAR, 0, "select_arena_b_bg_00_ncg.bin", 0, 0, 0);
    LoadAndQueueUpload(1, UPLOAD_KIND_BG0_SCR, 0, "select_arena_b_bg_00_nsc.bin", 0, 0, 0);
    LoadAndQueueUpload(1, UPLOAD_KIND_BG_EXT_PLTT, 0x2000, "select_wireless_b_bg_00_ncl.bin", 0, 0, 0);
    LoadAndQueueUpload(1, UPLOAD_KIND_BG1_CHAR, 0, "select_wireless_b_bg_00_ncg.bin", 0, 0, 0);
    LoadAndQueueUpload(1, UPLOAD_KIND_BG1_SCR, 0, "select_wireless_b_bg_00_nsc.bin", 0, 0, 0);
    ChangeDir("/Game/2D/union_b/");
    LoadAndQueueUpload(1, UPLOAD_KIND_BG_EXT_PLTT, 0x4000, "union_b_bg_window_ncl.bin", 0, 0, 0);
    LoadAndQueueUpload(1, UPLOAD_KIND_BG2_CHAR, 0, "union_b_bg_window_ncg.bin", 0, 0, 0);
    LoadAndQueueUpload(1, UPLOAD_KIND_BG2_SCR, 0, "union_b_bg_window_b_nsc.bin", 0, 0, 0);
    ChangeDir("/Game/Battle/obj/");
    SpriteScreen &sub = SpriteManager::Get().Screen(1);
    sub.SetObjVramMode(3);
    sub.LoadCharacters("select_wireless_b_obj_00.nce_ncg1D.bin", 0x4000, 0);
    sub.LoadPalette("select_wireless_b_obj_00_ncl.bin", 0x20, 0);
    if (mSlots[0].mCellAnim != NULL) {
        delete mSlots[0].mCellAnim;
    }
    mSlots[0].mCellAnim            = new CellAnimation("select_wireless_b_obj_00_nce.bin", 0, 1, 1);
    mSlots[0].mCellAnim->mCharBase = sub.GetCharBase(0x4000);
    mSlots[0].mCellAnim->mPaletteBase   = 1;
    mSlots[0].mX               = 0x20;
    mSlots[0].mY               = 0x30;
    mSlots[0].mTargetX          = 0x20;
    mSlots[0].mTargetY          = 0x30;
    if (mSlots[1].mCellAnim != NULL) {
        delete mSlots[1].mCellAnim;
    }
    mSlots[1].mCellAnim            = new CellAnimation("select_wireless_b_obj_00_nce.bin", 0, 1, 1);
    mSlots[1].mCellAnim->mCharBase = sub.GetCharBase(0x4000);
    mSlots[1].mCellAnim->mPaletteBase   = 1;
    mSlots[1].mX               = 0x20;
    mSlots[1].mY               = 0x60;
    mSlots[1].mTargetX          = 0x20;
    mSlots[1].mTargetY          = 0x60;
    if (mSlots[2].mCellAnim != NULL) {
        delete mSlots[2].mCellAnim;
    }
    mSlots[2].mCellAnim            = new CellAnimation("select_wireless_b_obj_00_nce.bin", 12, 1, 1);
    mSlots[2].mCellAnim->mCharBase = sub.GetCharBase(0x4000);
    mSlots[2].mCellAnim->mPaletteBase   = 1;
    mSlots[2].mX               = 0x70;
    mSlots[2].mY               = 0x60;
    mSlots[2].mTargetX          = 0x70;
    mSlots[2].mTargetY          = 0x60;
    ChangeDir("/Game/Battle/Obj/");
    SpriteScreen &main = SpriteManager::Get().Screen(0);
    main.SetObjVramMode(3);
    main.LoadCharacters("select_arena_u_obj_00.nce_ncg1D.bin", 0, 0);
    main.LoadPalette("select_arena_u_obj_00_ncl.bin", 0, 0);
    if (mMain.mCellAnim != NULL) {
        delete mMain.mCellAnim;
    }
    mMain.mCellAnim            = new CellAnimation("select_arena_u_obj_00_nce.bin", 1, 1, 1);
    mMain.mCellAnim->mCharBase = main.GetCharBase(0);
    mMain.mCellAnim->mPaletteBase   = 0;
    mMain.mX               = 0x80;
    mMain.mY               = 0x60;
    mMain.mTargetX          = 0x80;
    mMain.mTargetY          = 0x60;
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
    mCursor = new SelectWindowCursor(2, sArenaSlotItems, 0);
    if (mMessageWindow != NULL) {
        delete mMessageWindow;
    }
    mMessageWindow = new MessageWindow("mes_BattleArenaMenu", 1, 0x2000);
    mMessageWindow->func_02065bd4(0x14, 4, 1);
    mMessageWindow->SetWidth(0xd8);
}

void ArenaScene::SetupVariant() {
    for (u8 i = 0; i < 3; i++) {
        mSlots[i].mVisible = 0;
        mSlots[i].mFrame   = -1;
    }
    mResult = 0;
    mSelection = 0;
    if (mVariant == 0) {
        mMain.mCellAnim->SetAnimation(0);
    } else {
        mMain.mCellAnim->SetAnimation(1);
    }
    switch (mVariant) {
        case 0:
            G2S_SetBG0Priority(2);
            mSlots[0].mFrame = 0;
            mSlots[1].mFrame = 3;
            mMessageWindow->StartMessage(0, 0, 1);
            break;
        case 1:
            G2S_SetBG0Priority(2);
            mSlots[0].mFrame = 6;
            mSlots[1].mFrame = 9;
            mMessageWindow->StartMessage(1, 0, 1);
            break;
        case 2:
            G2S_SetBG0Priority(3);
            mMessageWindow->StartMessage(0x10, 0, 1);
            break;
        case 3:
            G2S_SetBG0Priority(3);
            mSlots[2].mFrame = 12;
            mMessageWindow->StartMessage(3, 0, 1);
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
        mSlots[0].mTargetX = 0x20;
        mSlots[0].mTargetY = 0x30;
        mSlots[1].mTargetX = 0x20;
        mSlots[1].mTargetY = 0x60;
        SelectWindowCursor *p = mCursor;
        p->mUnk_05        = -1;
        p->mPressedItem        = -1;
        p->mPressedOnSelection        = 0;
        mCursor->mSelection = mSelection;
        RefreshSlotAnimations();
        break;
        case 3:
        mSlots[2].mVisible = 0;
        mSlots[2].mX       = 0x20;
        mSlots[2].mY       = 0x34;
        mSlots[2].mTargetX  = 0x20;
        mSlots[2].mTargetY  = 0x34;
        break;
    }
    u8 sel = mVariant != 2 ? 1 : 0;
    if (mMenuWindow != NULL) {
        delete mMenuWindow;
    }
    mMenuWindow = new SelectWindow(2, sArenaMenuItems[sel], 0);
    mMenuWindow->SetCells(2, sArenaMenuCells);
    mMenuWindow->mCursor->mSelection       = -1;
    mMenuWindow->mEnabled                  = 0;
    mMenuWindow->mButtons[0].mEnabled      = 0;
}

void ArenaScene::RefreshSlotAnimations() {
    for (u8 i = 0; i < 2; i++) {
        if (mSlots[i].mVisible != 0) {
            if (mSelection == i) {
                mSlots[i].mCellAnim->SetAnimation(mSlots[i].mFrame + 1);
            } else {
                mSlots[i].mCellAnim->SetAnimation(mSlots[i].mFrame);
            }
        }
    }
}

u32 ArenaScene::UpdateSelection() {
    u32 result = 0;
    u8 key     = mCursor->Update(0);
    u8 oldSel  = mSelection;
    u8 newSel  = mCursor->mSelection;
    u8 menu    = UpdateMenu();
    if (oldSel != newSel) {
        mSelection = newSel;
        if (newSel < 2) {
            RefreshSlotAnimations();
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

u32 ArenaScene::UpdateMenu() {
    if (mMenuWindow == NULL) {
        return 0;
    }
    mMenuWindow->mEnabled = 1;
    return mMenuWindow->Update(0);
}

void ArenaScene::ResetMenu() {
    u8 state;
    do {
        mMenuWindow->Draw();
        mMenuWindow->Update(0);
        state = mMenuWindow->mState;
    } while (!((state <= 2) | (state == 4)));
    mMenuWindow->Reset(-1);
    mMenuWindow->mCursor->mSelection = -1;
    mMenuWindow->mEnabled            = 0;
    for (u8 i = 0; i < 2; i++) {
        mMenuWindow->mCellAnims[i]->SetAnimation(sArenaMenuCells[((const WindowItem *) sArenaMenuItems)[i].mId].mAnimIndices[0]);
    }
}

void ArenaScene::func_ov001_0221bee8() {
    if (mMessageWindow != NULL) {
        mMessageWindow->Update();
    }
    if (mMain.mVisible == 0) {
        return;
    }
    mMain.mCellAnim->Update();
    mMain.mCellAnim->Draw(0, mMain.mX, mMain.mY, mMain.mUnk_0c);
}

void ArenaScene::func_ov001_0221bf40() {
    if ((mVariant == 2) | (mVariant == 3)) {
        DrawNames();
    }
    for (u8 i = 0; i < 3; i++) {
        if (mSlots[i].mVisible != 0) {
            mSlots[i].mX = (mSlots[i].mX + mSlots[i].mTargetX) / 2;
            mSlots[i].mY = (mSlots[i].mY + mSlots[i].mTargetY) / 2;
            mSlots[i].mCellAnim->Update();
            mSlots[i].mCellAnim->Draw(1, mSlots[i].mX, mSlots[i].mY, mSlots[i].mUnk_0c);
        }
    }
    mMenuWindow->Draw();
}

void ArenaScene::InitNameText() {
    mTextBufferSize = TextManager::Get().BufferSize(0x20, 2);
    for (u8 i = 0; i < 4; i++) {
        mBuffers[i] = OS_Alloc(mTextBufferSize);
    }
    mGlyphCount = TextManager::Get().GlyphCount(8);
    mGlyphBuffer = OS_Alloc(mGlyphCount * 12);
    TextManager::Get().Init(mGlyphBuffer, 0, 0, 0x20, 2, 0x21);
    TextManager::Get().SetColor(0, 0, 0xf);
    MI_CpuClearFast((void *) 0x6600000, 0x2000);
    PrintNames();
}

void ArenaScene::ClearNameText() {
    for (u8 i = 0; i < 4; i++) {
        MI_CpuClear16(mBuffers[i], mTextBufferSize);
    }
}

void ArenaScene::PrintNames() {
    ClearNameText();
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
        QueueUpload(SpriteManager::Get().Screen(1).mBgIndex, UPLOAD_KIND_OBJ, i << 11, mBuffers[i], mTextBufferSize, 0);
    }
}

void ArenaScene::DrawNames() {
    SpriteManager::Get();
    for (u8 i = 0; i < 4; i++) {
        SpriteScreen &screen = SpriteManager::Get().Screen(1);
        screen.AddOam(mGlyphBuffer, (u8) mGlyphCount, 0x24, (s16) (i * 0x18 + 0x38), 0, (u16) (u32) screen.GetCharBase(i << 11), 0xf,
                             0);
    }
}

void ArenaScene::ResetNames() {
    for (u8 i = 0; i < 4; i++) {
        for (u8 j = 0; j < 12; j++) {
            data_020d433c.mNames[i][j] = gSaveData[0].mName[j];
        }
    }
    mPlayerCount = 0;
}

u32 ArenaScene::SetMessageTypeAndCheckAll(u32 unk1) {
    data_0220f540.mUnk_00 = unk1;
    u32 needed    = WirelessManager::Get().GetRecvBlockCount();
    u8 count      = 0;
    for (u8 i = 0; i < 4; i++) {
        u8 *p = WirelessManager::Get().GetRecvBlock(i);
        if (p != NULL && *p == unk1) {
            count++;
        }
    }
    return count >= needed;
}

void ArenaScene::LoadMessages() {
    SpriteManager::Get().Screen(1).SetObjVramMode(3);
    ChangeDir("/");
    mMessages = (MessageFile *) LoadFile("font/mes_BattleArenaMenu.mes", 0, 0, 0, 0, 0);
    mMessageTextBufferSize = TextManager::Get().BufferSize(0x18, 8);
    mMessageTextBuffer = OS_Alloc(mMessageTextBufferSize);
    MI_CpuClear16(mMessageTextBuffer, mMessageTextBufferSize);
    mMessageGlyphCount = TextManager::Get().GlyphCount(0x30);
    mMessageGlyphBuffer = OS_Alloc(mMessageGlyphCount * 12);
    TextManager::Get().Init(mMessageGlyphBuffer, 1, 0, 0x18, 8, 0x11);
    TextManager::Get().SetColor(1, 0, 0xf);
    PrintMessage(12);
}

void ArenaScene::PrintMessage(u32 unk1) {
    MI_CpuClear16(mMessageTextBuffer, mMessageTextBufferSize);
    TextManager &text = TextManager::Get();
    s16 height;
    s16 width;
    width             = 0;
    height            = 0;
    MessageFile *file = mMessages;
    text.Print(file->GetText(unk1), file->GetLength(unk1), mMessageTextBuffer, 0x18, 8, &height, &width, 0, 0, 2, 0x11, 0xe, 0, 0, 1, 1, 1);
    QueueUpload(SpriteManager::Get().Screen(1).mBgIndex, UPLOAD_KIND_OBJ, 0xe000, mMessageTextBuffer, mMessageTextBufferSize, 0);
}

void ArenaScene::DrawMessage() {
    SpriteManager::Get().Screen(1).AddOam(mMessageGlyphBuffer, (u8) mMessageGlyphCount, 0x20, 0x28, 0, 0x1c0, 0xf, 0);
}
