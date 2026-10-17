#include "UI/SelectWindow.hpp"

static const u16 sSpecialNameMoney[6] = {0, 10, 50, 100, 1000, 5000};
static const u8 sSpecialNameCodes[6][12] = {
    {0x44, 0x44, 0x49, 0x48, 0x42, 0x44, 0x49, 0x43, 0x44, 0x40, 0x47, 0x41},
    {0x41, 0x48, 0x40, 0x49, 0x47, 0x48, 0x47, 0x47, 0x42, 0x42, 0x46, 0x49},
    {0x43, 0x43, 0x42, 0x40, 0x44, 0x44, 0x42, 0x49, 0x42, 0x49, 0x42, 0x45},
    {0x44, 0x42, 0x43, 0x44, 0x48, 0x42, 0x49, 0x44, 0x42, 0x49, 0x46, 0x48},
    {0x44, 0x46, 0x48, 0x43, 0x46, 0x44, 0x47, 0x48, 0x43, 0x41, 0x45, 0x47},
    {0x48, 0x47, 0x48, 0x42, 0x48, 0x49, 0x48, 0x41, 0x42, 0x41, 0x41, 0x40},
};
static const WindowItemRecord sNameEntryKeyboardItems[7] = {
    {0, 16, 230, 76, 28, 12, 0, {-1, -1, -1, -1}, 0, 0},
    {1, 32, 230, 56, 32, 16, 0, {-1, -1, -1, -1}, 0, 3},
    {2, 64, 230, 132, 32, 16, 0, {-1, -1, -1, -1}, 0, 3},
    {3, 32768, 239, 72, 0, 0, 0, {-1, -1, -1, -1}, 0, 0},
    {4, 8, 32, 160, 52, 20, 0, {-1, -1, -1, -1}, 0, 1},
    {5, 8, 80, 160, 52, 20, 0, {-1, -1, -1, -1}, 0, 1},
    {6, 8, 176, 160, 52, 20, 0, {-1, -1, -1, -1}, 0, 1},
};
static const WindowCellRecord sNameEntryKeyboardCells[7] = {
    {"/Game/2D/union_b/union_b_obj_scroll_nce.bin", 4, 0x2000, 13, 2, 1, {4, 4, 4, 0}},
    {"/Game/2D/union_b/union_b_obj_scroll_nce.bin", 0, 0x2000, 13, 2, 1, {0, 1, 1, 0}},
    {"/Game/2D/union_b/union_b_obj_scroll_nce.bin", 2, 0x2000, 13, 2, 1, {2, 3, 3, 0}},
    {"/Game/2D/union_b/union_b_obj_scroll_nce.bin", 6, 0x2000, 13, 2, 1, {6, 6, 6, 0}},
    {"/Game/NameEntry/keyboard_b_obj_00_nce.bin", 0, 0x0, 10, 2, 1, {0, 1, 0, 0}},
    {"/Game/NameEntry/keyboard_b_obj_00_nce.bin", 2, 0x0, 10, 2, 1, {2, 3, 2, 0}},
    {"/Game/NameEntry/keyboard_b_obj_00_nce.bin", 4, 0x0, 10, 2, 1, {4, 5, 4, 0}},
};
static const WindowItemRecord sNameEntryConfirmItems[2] = {
    {0, 32768, 120, 35, 0, 0, 0, {-1, -1, -1, -1}, 0, 0},
    {1, 8, 89, 136, 78, 20, 0, {1, 1, 1, 1}, 0, 2},
};
static const WindowCellRecord sNameEntryConfirmCells[2] = {
    {"/Game/2D/union_b/union_b_obj_window_nce.bin", 9, 0x10000, 0, 0, 1, {9, 9, 9, 0}},
    {"/Game/2D/union_b/union_b_obj_window_nce.bin", 1, 0x10000, 0, 0, 1, {0, 2, 1, 0}},
};

#include "Text/TextManager.hpp"
#include "System/TouchManager.hpp"
#include "System/InputManager.hpp"
#include "Scene/NameEntryScene.hpp"

#include "Game/SaveData.hpp"
#include "Graphics/CellAnimation.hpp"
#include "Graphics/Display.hpp"
#include "Graphics/SpriteManager.hpp"
#include "System/FileSystem.hpp"
#include "System/InputManager.hpp"
#include "System/SoundManager.hpp"
#include "System/TouchManager.hpp"
#include "Text/TextManager.hpp"
#include "UI/MessageWindow.hpp"
#include "UI/SelectWindow.hpp"
#include "Unknown/UnkFunctions.hpp"
#include <nitro/gx.h>
#include <nitro/mi.h>
#include <nitro/os.h>


NameEntryScene::NameEntryScene(u8 mode) :
    SceneBase(2, 0) {
    mCharTable = NULL;
    mMessages = NULL;
    mMessageWindow = NULL;
    mTextBuffer = NULL;
    mGlyphBuffer = NULL;
    mTextBufferSize = 0;
    mGlyphCount = 0;
    mCellAnims[0] = NULL;
    mCellAnims[1] = NULL;
    mCellAnims[2] = NULL;
    mKeyboard = NULL;
    mConfirm = NULL;
    mBuf3c = NULL;
    mBuf40 = NULL;
    mBuf44 = NULL;
    mBuf48 = NULL;
    mMaxLength = 12;
    mLength = 0;
    mScroll = 0;
    mCursorArea = 0;
    mCol = 0;
    mRow = 0;
    mUnk_6a = 0;
    mTab = NAME_ENTRY_TAB_FONT_A;
    mState = 0;
    mMode = mode;
    mPendingSubPlanes = -1;
    mSpecialNameHandle = 0;
    MI_CpuClear16(mName, sizeof(mName));
}

NameEntryScene::~NameEntryScene() {
    if (mBuf3c != NULL) {
        OS_Free(mBuf3c);
    }
    if (mBuf40 != NULL) {
        OS_Free(mBuf40);
    }
    if (mBuf44 != NULL) {
        OS_Free(mBuf44);
    }
    if (mBuf48 != NULL) {
        OS_Free(mBuf48);
    }
    delete mKeyboard;
    delete mConfirm;
    if (mCellAnims[0] != NULL) {
        delete mCellAnims[0];
    }
    if (mCellAnims[1] != NULL) {
        delete mCellAnims[1];
    }
    if (mCellAnims[2] != NULL) {
        delete mCellAnims[2];
    }
    delete mMessageWindow;
    if (mCharTable != NULL) {
        OS_Free(mCharTable);
        mCharTable = NULL;
    }
    if (mMessages != NULL) {
        OS_Free(mMessages);
        mMessages = NULL;
    }
    if (mTextBuffer != NULL) {
        ClearTextBuffer();
        OS_Free(mTextBuffer);
        mTextBuffer = NULL;
    }
    if (mGlyphBuffer != NULL) {
        OS_Free(mGlyphBuffer);
        mGlyphBuffer = NULL;
    }
}

void NameEntryScene::InitDisplay() {
    GX_SetDispSelect(1);
    SpriteManager::Get().SetupMode(1, 1);
    GX_SetGraphicsMode(1, 0, 0);
    GXS_SetGraphicsMode(0);
    G2S_SetBG0Control(0, 1, 0, 2, 0);
    G2S_SetBG1Control(0, 1, 2, 4, 0);
    G2S_SetBG2Control(0, 1, 4, 1, 0);
    G2S_SetBG3Control(0, 1, 6, 0xc, 0);
    G2S_SetBG0Offset(0, 0);
    G2S_SetBG1Offset(0, 0);
    G2S_SetBG2Offset(0, 0);
    G2S_SetBG3Offset(0, 0);
    G2S_SetBG0Priority(1);
    G2S_SetBG1Priority(3);
    G2S_SetBG2Priority(0);
    G2S_SetBG3Priority(3);
    GXS_SetVisiblePlane(0x13);
    G2S_SetBlendBrightness(0x1f, 0);
    GXS_SetVisibleWnd(0);
    G2S_SetWndOutsidePlane(0x1f, FALSE);
    G2S_SetWnd0InsidePlane(0x1f, FALSE);
    G2S_SetWnd1InsidePlane(0x1f, FALSE);
    G2S_SetWndOBJInsidePlane(0x1f, FALSE);
    G2S_SetWnd0Position(0, 0, 0, 0);
    G2S_SetWnd1Position(0, 0, 0, 0);
    G2S_BlendNone();
}

void NameEntryScene::LoadGraphics() {
    if (mMode == 0) {
        ChangeDir("/Game/Title");
        LoadAndQueueUpload(0, 7, 0, "title_u_bg_00_nsc.bin", 0, 0, 0);
        LoadAndQueueUpload(0, 11, 0, "title_u_bg_00_ncg.bin", 0, 0, 0);
        LoadAndQueueUpload(0, 1, 0, "title_u_bg_00_ncl.bin", 0, 0, 0);
    }
    ChangeDir("/Game/Nameentry");
    LoadAndQueueUpload(1, 8, 0, "keyboard_b_bg_00_nsc.bin", 0, 0, 0);
    LoadAndQueueUpload(1, 12, 0, "keyboard_b_bg_00_ncg.bin", 0, 0, 0);
    LoadAndQueueUpload(1, 1, 0x2000, "keyboard_b_bg_00_ncl.bin", 0, 0, 0);
    mBuf3c = (u8 *) LoadFile("keyboard_b_bg_font_a_nsc.bin", 0, 0, 0, 0, 0);
    mBuf40 = (u8 *) LoadFile("keyboard_b_bg_font_b_nsc.bin", 0, 0, 0, 0, 0);
    mBuf48 = (u8 *) LoadFile("keyboard_b_bg_font_b_ncg.bin", 0, 0, 0, 0, 0);
    mBuf44 = (u8 *) LoadFile("keyboard_b_bg_font_a_ncg.bin", 0, 0, 0, 0, 0);
    LoadAndQueueUpload(1, 1, 0, "keyboard_b_bg_font_ncl.bin", 0, 0, 0);
    UploadTabNcg();
    UploadTabNsc();
    SpriteScreen &screen = SpriteManager::Get().Screen(1);
    screen.SetObjVramMode(3);
    screen.LoadCharacters("keyboard_b_obj_00.nce_ncg1D.bin", 0, 0);
    screen.LoadPalette("keyboard_b_obj_00_ncl.bin", 0x140, 0);
    mCellAnims[2] = new CellAnimation("keyboard_b_obj_00_nce.bin", 0, 1, 1);
    mCellAnims[2]->mCharBase = screen.GetCharBase(0);
    mCellAnims[2]->mPaletteBase   = 10;
    mCellAnims[2]->SetAnimation(8);
    mCellAnims[1]            = new CellAnimation("keyboard_b_obj_00_nce.bin", 0, 1, 1);
    mCellAnims[1]->mCharBase = screen.GetCharBase(0);
    mCellAnims[1]->mPaletteBase   = 10;
    mCellAnims[1]->SetAnimation(7);
    mCellAnims[0]            = new CellAnimation("keyboard_b_obj_00_nce.bin", 0, 1, 1);
    mCellAnims[0]->mCharBase = screen.GetCharBase(0);
    mCellAnims[0]->mPaletteBase   = 10;
    mCellAnims[0]->SetAnimation(6);
    u32 n = mCellAnims[0]->mFrames[mCellAnims[0]->mAnimIndex].mFrameCount;
    if (n != 0) {
        n--;
    }
    mCellAnims[0]->mFrameIndex = n;
    ChangeDir("/Game/2D/union_b");
    SpriteScreen &screen2 = SpriteManager::Get().Screen(1);
    screen2.SetObjVramMode(3);
    screen2.LoadCharacters("union_b_obj_scroll.nce_ncg1D.bin", 0x2000, 0);
    screen2.LoadPalette("union_b_obj_scroll_ncl.bin", 0x1a0, 0);
    mKeyboard = new SelectWindow(7, sNameEntryKeyboardItems, 0);
    mKeyboard->SetCells(7, sNameEntryKeyboardCells);
    mKeyboard->mCursor->mSelection = -1;
    mKeyboard->mCursor->SetScroll(0);
    mKeyboard->mCursor->SetupScroll(0x4c, 0x84, 4, 0xc);
    if (mMode != 0) {
        mKeyboard->mButtons[6].mEnabled = 0;
        mKeyboard->mCursor->mItems[6].mFlags = 0x8000;
    }
    SpriteScreen &screen3 = SpriteManager::Get().Screen(1);
    screen3.LoadCharacters("union_b_obj_window.nce_ncg1D.bin", 0x10000, 0);
    screen3.LoadPalette("union_b_obj_window_ncl.bin", 0, 0);
    mConfirm = new SelectWindow(2, sNameEntryConfirmItems, 1);
    mConfirm->SetCells(2, sNameEntryConfirmCells);
    mConfirm->mEnabled = 0;
    LoadAndQueueUpload(1, 1, 0x4000, "union_b_bg_window_b_ncl.bin", 0, 0, 0);
    LoadAndQueueUpload(1, 13, 0, "union_b_bg_window_ncg.bin", 0, 0, 0);
    LoadAndQueueUpload(1, 9, 0, "union_b_bg_window_a_nsc.bin", 0, 0, 0);
}

void NameEntryScene::Update() {
    if (mPendingSubPlanes != -1) {
        GXS_SetVisiblePlane(mPendingSubPlanes);
        mPendingSubPlanes = -1;
    }
    switch (mState) {
        case 0:
            InitDisplay();
            LoadGraphics();
            Load();
            SpriteManager::Get().StartFade(2);
            mState++;
            break;
        case 1:
            if (SpriteManager::Get().IsFading()) {
                return;
            }
            mState++;
            break;
        case 2: {
            u8 prevScroll = mScroll;
            if (InputManager::Get().IsPressed(1)) {
                switch (mCursorArea) {
                    case 0:
                        AddChar((u8) (mCol + (mRow + mScroll) * 8 + mTab * 0x60));
                        break;
                    case 1:
                        SoundManager::Get().StartSeqArc(0);
                        mKeyboard->SetResultItem(4, 3);
                        break;
                    case 2:
                        SoundManager::Get().StartSeqArc(0);
                        mKeyboard->SetResultItem(5, 3);
                        break;
                    case 3:
                        SoundManager::Get().StartSeqArc(0);
                        mKeyboard->SetResultItem(6, 3);
                        break;
                }
            }
            if (InputManager::Get().IsPressed(2)) {
                SoundManager::Get().StartSeqArc(0);
                mKeyboard->SetResultItem(5, 3);
            }
            switch (mCursorArea) {
                case 0:
                    if (InputManager::Get().IsTrigger(KEY_UP)) {
                        SoundManager::Get().StartSeqArc(5);
                        if (mRow != 0) {
                            mRow--;
                        } else if (mScroll != 0) {
                            mScroll--;
                            if (mKeyboard != NULL) {
                                mKeyboard->mCursor->SetScroll(mScroll);
                            }
                        } else {
                            if (mCol < 2) {
                                mCursorArea = 1;
                            } else if (mCol < 5 || mKeyboard->mButtons[6].mEnabled == 0) {
                                mCursorArea = 2;
                            } else {
                                mCursorArea = 3;
                            }
                            mCellAnims[2]->SetAnimation(9);
                        }
                    } else if (InputManager::Get().IsTrigger(KEY_DOWN)) {
                        SoundManager::Get().StartSeqArc(5);
                        mRow++;
                        if (mRow >= 4) {
                            mRow = 3;
                            mScroll++;
                            if (mScroll > 8) {
                                mScroll = 8;
                                if (mCol < 2) {
                                    mCursorArea = 1;
                                } else if (mCol < 5 || mKeyboard->mButtons[6].mEnabled == 0) {
                                    mCursorArea = 2;
                                } else {
                                    mCursorArea = 3;
                                }
                                mCellAnims[2]->SetAnimation(9);
                            }
                            if (mKeyboard != NULL) {
                                mKeyboard->mCursor->SetScroll(mScroll);
                            }
                        }
                    }
                    if (InputManager::Get().IsTrigger(KEY_LEFT)) {
                        SoundManager::Get().StartSeqArc(5);
                        mCol = (u8) (mCol + 7);
                        mCol &= 7;
                    } else if (InputManager::Get().IsTrigger(KEY_RIGHT)) {
                        SoundManager::Get().StartSeqArc(5);
                        mCol = (u8) (mCol + 1);
                        mCol &= 7;
                    }
                    break;
                case 1:
                case 2:
                case 3:
                    if (InputManager::Get().IsTrigger(KEY_UP)) {
                        SoundManager::Get().StartSeqArc(5);
                        switch (mCursorArea) {
                            case 1:
                                mCol = 0;
                                break;
                            case 2:
                                mCol = 2;
                                break;
                            case 3:
                                mCol = 6;
                                break;
                        }
                        mRow    = 3;
                        mScroll = 8;
                        mKeyboard->mCursor->SetScroll(mScroll);
                        mCellAnims[2]->SetAnimation(8);
                        mCursorArea = 0;
                    } else if (InputManager::Get().IsTrigger(KEY_DOWN)) {
                        SoundManager::Get().StartSeqArc(5);
                        switch (mCursorArea) {
                            case 1:
                                mCol = 0;
                                break;
                            case 2:
                                mCol = 2;
                                break;
                            case 3:
                                mCol = 6;
                                break;
                        }
                        mRow    = 0;
                        mScroll = 0;
                        mKeyboard->mCursor->SetScroll(0);
                        mCellAnims[2]->SetAnimation(8);
                        mCursorArea = 0;
                    }
                    if (InputManager::Get().IsTrigger(KEY_LEFT)) {
                        SoundManager::Get().StartSeqArc(5);
                        mCursorArea = (u8) (mCursorArea + 3);
                        mCursorArea &= 3;
                        if (mCursorArea == 0) {
                            if (mKeyboard->mButtons[6].mEnabled != 0) {
                                mCursorArea = 3;
                            } else {
                                mCursorArea = 2;
                            }
                        }
                    } else if (InputManager::Get().IsTrigger(KEY_RIGHT)) {
                        SoundManager::Get().StartSeqArc(5);
                        mCursorArea = (u8) (mCursorArea + 1);
                        mCursorArea &= 3;
                        switch (mCursorArea) {
                            case 0:
                                mCursorArea = 1;
                                break;
                            case 3:
                                if (mKeyboard->mButtons[6].mEnabled == 0) {
                                    mCursorArea = 1;
                                }
                                break;
                        }
                    }
                    break;
            }
            if (prevScroll != mScroll) {
                UploadTabNsc();
            }
            if (InputManager::Get().IsPressed(0x400)) {
                SoundManager::Get().StartSeqArc(0);
                mKeyboard->SetResultItem(4, 3);
            }
            if (mKeyboard != NULL) {
                if (mKeyboard->mCursor->mScroll != mScroll) {
                    mScroll = mKeyboard->mCursor->mScroll;
                    UploadTabNsc();
                }
                u8 result = mKeyboard->Update(1);
                mKeyboard->Draw();
                SelectWindow *w = mKeyboard;
                s16 posY = w->mCursor->mScrollItemY;
                w->mCellPos[0].x = 0xe6;
                w->mCellPos[0].y = posY;
                if (result == 3) {
                    switch ((u8) mKeyboard->GetSelectedId()) {
                        case 4:
                            mTab = (u8) (mTab + 1);
                            mTab &= 1;
                            UploadTabNcg();
                            UploadTabNsc();
                            break;
                        case 5:
                            Backspace();
                            break;
                        case 6:
                            CheckSpecialName();
                            if (mSpecialNameHandle == 0) {
                                mState = 4;
                            } else {
                                GXS_SetVisiblePlane(0x17);
                                mKeyboard->mEnabled = 0;
                                mConfirm->mEnabled = 1;
                                MI_CpuClear16(mName, sizeof(mName));
                                mLength = 0;
                                PrintMessage();
                                mState = 3;
                            }
                            break;
                    }
                }
            }
            if (mCellAnims[0] != NULL) {
                AddTouchedChar();
                mCellAnims[0]->Update();
                mCellAnims[0]->Draw(1, (u8) (mCol * 24 + 0x20), (u8) (mRow * 24 + 0x38), 2);
            }
            if (mCellAnims[2] != NULL) {
                mCellAnims[2]->Update();
                switch (mCursorArea) {
                    case 0:
                        mCellAnims[2]->Draw(1, (u8) (mCol * 24 + 0x20), (u8) (mRow * 24 + 0x38), 1);
                        break;
                    case 1:
                        mCellAnims[2]->Draw(1, 0x20, 0xa0, 1);
                        break;
                    case 2:
                        mCellAnims[2]->Draw(1, 0x50, 0xa0, 1);
                        break;
                    case 3:
                        mCellAnims[2]->Draw(1, 0xb0, 0xa0, 1);
                        break;
                }
            }
            if (mCellAnims[1] != NULL) {
                mCellAnims[1]->Update();
                u8 pos = mLength;
                if (pos >= mMaxLength) {
                    pos = mMaxLength - 1;
                }
                mCellAnims[1]->Draw(1, (u8) (pos * 13 + 0x30), 0x1c, 2);
            }
            if (mState == 3) {
                DrawConfirmText();
            } else {
                func_ov000_0221bc98();
            }
            if (InputManager::Get().IsPressed(8) && mKeyboard->mButtons[6].mEnabled != 0) {
                SoundManager::Get().StartSeqArc(0);
                mKeyboard->SetResultItem(6, 3);
            }
            break;
        }
        case 3:
            DrawConfirmText();
            if (mConfirm->Update(1) == 3) {
                mPendingSubPlanes = 0x13;
                mKeyboard->mEnabled = 1;
                mConfirm->mEnabled = 0;
                mKeyboard->mButtons[6].mEnabled = 0;
                mKeyboard->mCursor->mItems[6].mFlags = 0x8000;
                if (mCursorArea == 3) {
                    mCursorArea = 2;
                }
                RenderName();
                if (mSpecialNameHandle == 1) {
                    gSaveData[0].mUnk_5d2 = 1;
                } else {
                    u32 amount = sSpecialNameMoney[mSpecialNameHandle - 1] * 100;
                    if (amount > 9999990) {
                        amount = 9999990;
                    }
                    gSaveData[0].mMoney += amount;
                    if (gSaveData[0].mMoney > 9999990) {
                        gSaveData[0].mMoney = 9999990;
                    }
                }
                mSpecialNameHandle = 0;
                mState = 2;
            }
            mKeyboard->Draw();
            mConfirm->Draw();
            break;
        case 4:
            switch (mMode) {
                case 0: {
                    u8 i = 0;
                    do {
                        gSaveData[0].mName[i] = mName[i];
                        i++;
                    } while (i < mMaxLength);
                    break;
                }
                case 1:
                    break;
            }
            mState++;
            break;
        case 5:
            SpriteManager::Get().StartFade(3);
            mState++;
            break;
        case 6:
            if (!SpriteManager::Get().IsFading()) {
                return;
            }
            mState++;
            break;
        case 7:
            RequestScene(1);
            mFinished = 1;
            break;
    }
}

void NameEntryScene::UploadTabNsc() {
    switch (mTab) {
        case NAME_ENTRY_TAB_FONT_A:
            QueueUpload(1, 7, 0x1c0, mBuf3c + 0x1c0 + mScroll * 0xc0, 0x300, 0);
            break;
        case NAME_ENTRY_TAB_FONT_B:
            QueueUpload(1, 7, 0x1c0, mBuf40 + 0x1c0 + mScroll * 0xc0, 0x300, 0);
            break;
    }
}

void NameEntryScene::UploadTabNcg() {
    switch (mTab) {
        case NAME_ENTRY_TAB_FONT_A:
            QueueUpload(1, 0xb, 0, mBuf44, 0x5000, 0);
            break;
        case NAME_ENTRY_TAB_FONT_B:
            QueueUpload(1, 0xb, 0, mBuf48, 0x5000, 0);
            break;
    }
}

void NameEntryScene::AddTouchedChar() {
    if (TouchManager::Get().mReleased == 0) {
        return;
    }
    s16 px = TouchManager::Get().mX;
    s16 py = TouchManager::Get().mY;
    px -= 0x20;
    py -= 0x38;
    if (px < 0 || py < 0) {
        return;
    }
    u8 rx = px % 24;
    u8 ry = py % 24;
    if (rx > 0x18 || ry > 0x16) {
        return;
    }
    u8 col = px / 24;
    u8 row = py / 24;
    if (col >= 8 || row >= 4) {
        return;
    }
    mCol = col;
    mRow = row;
    AddChar((u8) (mCol + (mRow + mScroll) * 8 + mTab * 0x60));
}

void NameEntryScene::Backspace() {
    if (mLength == 0) {
        return;
    }
    mLength--;
    mName[mLength] = 0;
    RenderName();
    if (mLength == 0) {
        mKeyboard->mButtons[6].mEnabled = 0;
        mKeyboard->mCursor->mItems[6].mFlags = 0x8000;
        if (mCursorArea == 3) {
            mCursorArea = 2;
        }
    }
}

void NameEntryScene::AddChar(s32 index) {
    if (mLength == 0) {
        switch ((u8) (index % 0x60)) {
            case 0x1a:
            case 0x1b:
            case 0x1c:
            case 0x1d:
            case 0x1e:
            case 0x1f:
            case 0x3b:
            case 0x3c:
            case 0x3d:
            case 0x3e:
            case 0x3f:
            case 0x5f:
                return;
        }
    }
    if (mLength >= mMaxLength) {
        mLength = mMaxLength - 1;
    }
    u8 c = index % 0x60;
    TextManager::Get();
    const u16 *chars = (const u16 *) mCharTable->GetText(mTab);
    chars += c;
    mName[mLength] = *chars;
    mLength++;
    RenderName();
    SoundManager::Get().StartSeqArc(0);
    mKeyboard->mButtons[6].mEnabled = 1;
    mKeyboard->mCursor->mItems[6].mFlags = 8;
    mCellAnims[0]->Rewind();
    if (mLength >= mMaxLength) {
        mCellAnims[2]->SetAnimation(9);
        mCursorArea = 3;
    } else {
        mCellAnims[2]->SetAnimation(8);
        mCursorArea = 0;
    }
}

void NameEntryScene::Load() {
    SpriteManager::Get().Screen(1).SetObjVramMode(3);
    ChangeDir("/font/");
    mMessageWindow = new MessageWindow("mes_name", 1, 0x3000);
    mMessageWindow->func_02065bd4(0x14, 4, 0);
    mMessageWindow->SetWidth(0xd8);
    mMessageWindow->StartMessage(0, 0, 1);
    mCharTable = (MessageFile *) LoadFile("mes_Alphabet.mes", 0, 0, 0, 0, 0);
    mMessages = (MessageFile *) LoadFile("mes_name.mes", 0, 0, 0, 0, 0);
    mTextBufferSize = TextManager::Get().BufferSize(0x18, 8);
    mTextBuffer = OS_Alloc(mTextBufferSize);
    MI_CpuClear16(mTextBuffer, mTextBufferSize);
    if (mMode == 0) {
        if (gSaveData[0].mName[0] != 0) {
            u8 i = 0;
            do {
                u16 c = gSaveData[0].mName[i];
                if (c == 0) {
                    break;
                }
                mName[i] = c;
                i++;
            } while (i < 12);
            mLength = i;
        } else {
            mLength = 0;
            mKeyboard->mButtons[6].mEnabled = 0;
            mKeyboard->mCursor->mItems[6].mFlags = 0x8000;
            if (mCursorArea == 3) {
                mCursorArea = 2;
            }
        }
    }
    mGlyphCount = TextManager::Get().GlyphCount(3);
    mGlyphBuffer = OS_Alloc(mGlyphCount * 0xc);
    TextManager::Get().Init(mGlyphBuffer, 1, 0, 0x18, 8, 0x44);
    TextManager::Get().SetColor(1, 0, 0xe);
    if (mMode == 0) {
        RenderName();
    }
}

void NameEntryScene::RenderName() {
    ClearTextBuffer();
    TextManager::Get().mUse4bppFont = 1;
    u8 i = 0;
    do {
        s16 x;
        s16 y;
        u16 width;
        y = 0;
        width = 0;
        int base = i * 13;
        x = base;
        TextManager::Get().Print(&mName[i], 2, mTextBuffer, 0x18, 8, &x, &y, &width, 0, 2, 0x44, 0, 0, 0, 0, 1, 0);
        y = 0;
        x = base + (11 - width) / 2;
        TextManager::Get().Print(&mName[i], 2, mTextBuffer, 0x18, 8, &x, &y, 0, 0, 2, 0x44, 0, 0, 0, 0, 1, 1);
        i++;
    } while (i < mMaxLength);
    DC_FlushRange(mTextBuffer, mTextBufferSize);
    TextManager &endText = TextManager::Get();
    endText.mUse4bppFont = 0;
    QueueUpload(SpriteManager::Get().Screen(1).mBgIndex, 4, 0x4000, mTextBuffer, mTextBufferSize, 0);
}

void NameEntryScene::func_ov000_0221bc98() {
    if (mMessageWindow != NULL) {
        mMessageWindow->Update();
    }
    SpriteManager::Get().Screen(1).AddOam(mGlyphBuffer, mGlyphCount, 0x33, 0x1e, 0, 0x80, 0xe, 0);
}

void NameEntryScene::ClearTextBuffer() {
    if (mMessageWindow != NULL) {
        mMessageWindow->ClearBuffer();
    }
    if (mTextBuffer != NULL) {
        MI_CpuClear16(mTextBuffer, mTextBufferSize);
    }
}

void NameEntryScene::CheckSpecialName() {
    BOOL match;
    mSpecialNameHandle = 0;
    u32 value = gSaveData[0].mMoney - 3000;
    u32 digit1k = (value / 1000) % 10;
    u32 digit10k = (value / 10000) % 10;
    u32 digit100k = (value / 100000) % 10;
    for (u8 i = 0; i < 6; i++) {
        switch (i) {
            case 0:
                if (gSaveData[0].mUnk_5d2 != 0) {
                    continue;
                }
                break;
            case 1:
                if ((u8) digit1k == 1 || (u8) digit1k == 6) {
                    continue;
                }
                break;
            case 2:
                if ((u8) digit1k == 5 || (u8) digit1k == 6) {
                    continue;
                }
                break;
            case 3:
                if ((u8) digit10k == 1) {
                    continue;
                }
                break;
            case 4:
                if ((u8) digit100k == 1 || (u8) digit100k == 6) {
                    continue;
                }
                break;
            case 5:
                if ((u8) digit100k == 5 || (u8) digit100k == 6) {
                    continue;
                }
                break;
        }
        u8 c;
        const u8 *row = sSpecialNameCodes[i];
        u8 k = 0;
        match = TRUE;
        do {
            if (mName[k] == 0) {
                match = FALSE;
                break;
            }
            c = row[k] % 0x60;
            TextManager::Get();
            MessageFile *table = mCharTable;
            u16 tab = row[k] >= 0x60 ? 1 : 0;
            const u16 *chars = (const u16 *) table->GetText(tab);
            if (mName[k] != chars[c]) {
                match = FALSE;
                break;
            }
            k++;
        } while (k < 12);
        if (match) {
            mSpecialNameHandle = i + 1;
            return;
        }
    }
}


void NameEntryScene::PrintMessage() {
    ClearTextBuffer();
    int index;
    if (mSpecialNameHandle == 1) {
        index = 1;
    } else {
        TextManager::Get().mUnk_10 = sSpecialNameMoney[mSpecialNameHandle - 1];
        index = 2;
    }
    TextManager &text = TextManager::Get();
    s16 height;
    s16 width;
    width = 0;
    height = 0;
    MessageFile *file = mMessages;
    text.Print(file->GetText(index), file->GetLength(index), mTextBuffer, 0x18, 8, &height, &width, 0, 0, 2, 0x44, 0x10, 0, 0, 1, 1, 1);
    QueueUpload(SpriteManager::Get().Screen(1).mBgIndex, 4, 0x4000, mTextBuffer, mTextBufferSize, 0);
}

void NameEntryScene::DrawConfirmText() {
    SpriteManager::Get().Screen(1).AddOam(mGlyphBuffer, mGlyphCount, 0x20, 0x38, 0, 0x80, 0xe, 0);
}
