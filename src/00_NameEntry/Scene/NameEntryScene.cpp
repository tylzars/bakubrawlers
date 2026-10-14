#include "System/TouchManager.hpp"
#include "Text/TextManager.hpp"
#include "System/InputManager.hpp"
#include "Scene/NameEntryScene.hpp"

#include "Game/UnkStruct_020d43f0.hpp"
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

extern const u16 data_ov000_02225fc8[]; // {0, 10, 50, 100, 1000, 5000}
extern u8 data_ov000_02226044[6][12];
extern const WindowItemRecord data_ov000_0222608c[];
extern const WindowCellRecord data_ov000_02226150[];
extern const WindowItemRecord data_ov000_02225fd4[];
extern const WindowCellRecord data_ov000_0222600c[];


NameEntryScene::NameEntryScene(u8 mode) :
    SceneBase(2, 0) {
    mCharTable = NULL;
    mMessages = NULL;
    mMessage = NULL;
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
    mUnk_66 = 0;
    mState = 0;
    mCol = 0;
    mRow = 0;
    mUnk_6a = 0;
    mTab = 0;
    mUnk_6c = 0;
    mMode = mode;
    mUnk_70 = -1;
    mUnk_74 = 0;
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
    delete mMessage;
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
    SpriteManager::Get().func_02026dec(1, 1);
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
        func_02025b64(0, 7, 0, "title_u_bg_00_nsc.bin", 0, 0, 0);
        func_02025b64(0, 11, 0, "title_u_bg_00_ncg.bin", 0, 0, 0);
        func_02025b64(0, 1, 0, "title_u_bg_00_ncl.bin", 0, 0, 0);
    }
    ChangeDir("/Game/Nameentry");
    func_02025b64(1, 8, 0, "keyboard_b_bg_00_nsc.bin", 0, 0, 0);
    func_02025b64(1, 12, 0, "keyboard_b_bg_00_ncg.bin", 0, 0, 0);
    func_02025b64(1, 1, 0x2000, "keyboard_b_bg_00_ncl.bin", 0, 0, 0);
    mBuf3c = (u8 *) func_02023504("keyboard_b_bg_font_a_nsc.bin", 0, 0, 0, 0, 0);
    mBuf40 = (u8 *) func_02023504("keyboard_b_bg_font_b_nsc.bin", 0, 0, 0, 0, 0);
    mBuf48 = (u8 *) func_02023504("keyboard_b_bg_font_b_ncg.bin", 0, 0, 0, 0, 0);
    mBuf44 = (u8 *) func_02023504("keyboard_b_bg_font_a_ncg.bin", 0, 0, 0, 0, 0);
    func_02025b64(1, 1, 0, "keyboard_b_bg_font_ncl.bin", 0, 0, 0);
    func_ov000_0221b22c();
    func_ov000_0221b190();
    SpriteScreen &screen = SpriteManager::Get().Screen(1);
    screen.func_02027624(3);
    screen.LoadCharacters("keyboard_b_obj_00.nce_ncg1D.bin", 0, 0);
    screen.LoadPalette("keyboard_b_obj_00_ncl.bin", 0x140, 0);
    mCellAnims[2] = new CellAnimation("keyboard_b_obj_00_nce.bin", 0, 1, 1);
    mCellAnims[2]->mCharBase = screen.func_020276f4(0);
    mCellAnims[2]->mUnk_11   = 10;
    mCellAnims[2]->SetAnimation(8);
    mCellAnims[1]            = new CellAnimation("keyboard_b_obj_00_nce.bin", 0, 1, 1);
    mCellAnims[1]->mCharBase = screen.func_020276f4(0);
    mCellAnims[1]->mUnk_11   = 10;
    mCellAnims[1]->SetAnimation(7);
    mCellAnims[0]            = new CellAnimation("keyboard_b_obj_00_nce.bin", 0, 1, 1);
    mCellAnims[0]->mCharBase = screen.func_020276f4(0);
    mCellAnims[0]->mUnk_11   = 10;
    mCellAnims[0]->SetAnimation(6);
    u32 n = mCellAnims[0]->mFrames[mCellAnims[0]->mUnk_0e].mUnk_10;
    if (n != 0) {
        n--;
    }
    mCellAnims[0]->mUnk_0f = n;
    ChangeDir("/Game/2D/union_b");
    SpriteScreen &screen2 = SpriteManager::Get().Screen(1);
    screen2.func_02027624(3);
    screen2.LoadCharacters("union_b_obj_scroll.nce_ncg1D.bin", 0x2000, 0);
    screen2.LoadPalette("union_b_obj_scroll_ncl.bin", 0x1a0, 0);
    mKeyboard = new SelectWindow(7, data_ov000_0222608c, 0);
    mKeyboard->SetCells(7, data_ov000_02226150);
    mKeyboard->mCursor->mSelection = -1;
    mKeyboard->mCursor->SetScroll(0);
    mKeyboard->mCursor->func_02030474(0x4c, 0x84, 4, 0xc);
    if (mMode != 0) {
        mKeyboard->mButtons[6].mEnabled = 0;
        mKeyboard->mCursor->mItems[6].mFlags = 0x8000;
    }
    SpriteScreen &screen3 = SpriteManager::Get().Screen(1);
    screen3.LoadCharacters("union_b_obj_window.nce_ncg1D.bin", 0x10000, 0);
    screen3.LoadPalette("union_b_obj_window_ncl.bin", 0, 0);
    mConfirm = new SelectWindow(2, data_ov000_02225fd4, 1);
    mConfirm->SetCells(2, data_ov000_0222600c);
    mConfirm->mUnk_064 = 0;
    func_02025b64(1, 1, 0x4000, "union_b_bg_window_b_ncl.bin", 0, 0, 0);
    func_02025b64(1, 13, 0, "union_b_bg_window_ncg.bin", 0, 0, 0);
    func_02025b64(1, 9, 0, "union_b_bg_window_a_nsc.bin", 0, 0, 0);
}

void NameEntryScene::Update() {
    if (mUnk_70 != -1) {
        GXS_SetVisiblePlane(mUnk_70);
        mUnk_70 = -1;
    }
    switch (mUnk_6c) {
        case 0:
            InitDisplay();
            LoadGraphics();
            Load();
            SpriteManager::Get().StartFade(2);
            mUnk_6c++;
            break;
        case 1:
            if (SpriteManager::Get().IsFading()) {
                return;
            }
            mUnk_6c++;
            break;
        case 2: {
            u8 prevScroll = mUnk_66;
            if (InputManager::Get().IsHeld(1)) {
                switch (mState) {
                    case 0:
                        AddChar((u8) (mCol + (mRow + mUnk_66) * 8 + mTab * 0x60));
                        break;
                    case 1:
                        SoundManager::Get().func_020246f8(0);
                        mKeyboard->func_0205c844(4, 3);
                        break;
                    case 2:
                        SoundManager::Get().func_020246f8(0);
                        mKeyboard->func_0205c844(5, 3);
                        break;
                    case 3:
                        SoundManager::Get().func_020246f8(0);
                        mKeyboard->func_0205c844(6, 3);
                        break;
                }
            }
            if (InputManager::Get().IsHeld(2)) {
                SoundManager::Get().func_020246f8(0);
                mKeyboard->func_0205c844(5, 3);
            }
            switch (mState) {
                case 0:
                    if (InputManager::Get().IsTrigger(0x40)) {
                        SoundManager::Get().func_020246f8(5);
                        if (mRow != 0) {
                            mRow--;
                        } else if (mUnk_66 != 0) {
                            mUnk_66--;
                            if (mKeyboard != NULL) {
                                mKeyboard->mCursor->SetScroll(mUnk_66);
                            }
                        } else {
                            if (mCol < 2) {
                                mState = 1;
                            } else if (mCol < 5 || mKeyboard->mButtons[6].mEnabled == 0) {
                                mState = 2;
                            } else {
                                mState = 3;
                            }
                            mCellAnims[2]->SetAnimation(9);
                        }
                    } else if (InputManager::Get().IsTrigger(0x80)) {
                        SoundManager::Get().func_020246f8(5);
                        mRow++;
                        if (mRow >= 4) {
                            mRow = 3;
                            mUnk_66++;
                            if (mUnk_66 > 8) {
                                mUnk_66 = 8;
                                if (mCol < 2) {
                                    mState = 1;
                                } else if (mCol < 5 || mKeyboard->mButtons[6].mEnabled == 0) {
                                    mState = 2;
                                } else {
                                    mState = 3;
                                }
                                mCellAnims[2]->SetAnimation(9);
                            }
                            if (mKeyboard != NULL) {
                                mKeyboard->mCursor->SetScroll(mUnk_66);
                            }
                        }
                    }
                    if (InputManager::Get().IsTrigger(0x20)) {
                        SoundManager::Get().func_020246f8(5);
                        mCol = (u8) (mCol + 7);
                        mCol &= 7;
                    } else if (InputManager::Get().IsTrigger(0x10)) {
                        SoundManager::Get().func_020246f8(5);
                        mCol = (u8) (mCol + 1);
                        mCol &= 7;
                    }
                    break;
                case 1:
                case 2:
                case 3:
                    if (InputManager::Get().IsTrigger(0x40)) {
                        SoundManager::Get().func_020246f8(5);
                        switch (mState) {
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
                        mUnk_66 = 8;
                        mKeyboard->mCursor->SetScroll(mUnk_66);
                        mCellAnims[2]->SetAnimation(8);
                        mState = 0;
                    } else if (InputManager::Get().IsTrigger(0x80)) {
                        SoundManager::Get().func_020246f8(5);
                        switch (mState) {
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
                        mUnk_66 = 0;
                        mKeyboard->mCursor->SetScroll(0);
                        mCellAnims[2]->SetAnimation(8);
                        mState = 0;
                    }
                    if (InputManager::Get().IsTrigger(0x20)) {
                        SoundManager::Get().func_020246f8(5);
                        mState = (u8) (mState + 3);
                        mState &= 3;
                        if (mState == 0) {
                            if (mKeyboard->mButtons[6].mEnabled != 0) {
                                mState = 3;
                            } else {
                                mState = 2;
                            }
                        }
                    } else if (InputManager::Get().IsTrigger(0x10)) {
                        SoundManager::Get().func_020246f8(5);
                        mState = (u8) (mState + 1);
                        mState &= 3;
                        switch (mState) {
                            case 0:
                                mState = 1;
                                break;
                            case 3:
                                if (mKeyboard->mButtons[6].mEnabled == 0) {
                                    mState = 1;
                                }
                                break;
                        }
                    }
                    break;
            }
            if (prevScroll != mUnk_66) {
                func_ov000_0221b190();
            }
            if (InputManager::Get().IsHeld(0x400)) {
                SoundManager::Get().func_020246f8(0);
                mKeyboard->func_0205c844(4, 3);
            }
            if (mKeyboard != NULL) {
                if (mKeyboard->mCursor->mScroll != mUnk_66) {
                    mUnk_66 = mKeyboard->mCursor->mScroll;
                    func_ov000_0221b190();
                }
                u8 result = mKeyboard->Update(1);
                mKeyboard->Draw();
                SelectWindow *w = mKeyboard;
                s16 posY = w->mCursor->mUnk_0b;
                w->mCellPos[0].x = 0xe6;
                w->mCellPos[0].y = posY;
                if (result == 3) {
                    switch ((u8) mKeyboard->GetSelectedId()) {
                        case 4:
                            mTab = (u8) (mTab + 1);
                            mTab &= 1;
                            func_ov000_0221b22c();
                            func_ov000_0221b190();
                            break;
                        case 5:
                            Backspace();
                            break;
                        case 6:
                            CheckSpecialName();
                            if (mUnk_74 == 0) {
                                mUnk_6c = 4;
                            } else {
                                GXS_SetVisiblePlane(0x17);
                                mKeyboard->mUnk_064 = 0;
                                mConfirm->mUnk_064 = 1;
                                MI_CpuClear16(mName, sizeof(mName));
                                mLength = 0;
                                PrintMessage();
                                mUnk_6c = 3;
                            }
                            break;
                    }
                }
            }
            if (mCellAnims[0] != NULL) {
                func_ov000_0221b2a0();
                mCellAnims[0]->Update();
                mCellAnims[0]->Draw(1, (u8) (mCol * 24 + 0x20), (u8) (mRow * 24 + 0x38), 2);
            }
            if (mCellAnims[2] != NULL) {
                mCellAnims[2]->Update();
                switch (mState) {
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
            if (mUnk_6c == 3) {
                func_ov000_0221c1e4();
            } else {
                func_ov000_0221bc98();
            }
            if (InputManager::Get().IsHeld(8) && mKeyboard->mButtons[6].mEnabled != 0) {
                SoundManager::Get().func_020246f8(0);
                mKeyboard->func_0205c844(6, 3);
            }
            break;
        }
        case 3:
            func_ov000_0221c1e4();
            if (mConfirm->Update(1) == 3) {
                mUnk_70 = 0x13;
                mKeyboard->mUnk_064 = 1;
                mConfirm->mUnk_064 = 0;
                mKeyboard->mButtons[6].mEnabled = 0;
                mKeyboard->mCursor->mItems[6].mFlags = 0x8000;
                if (mState == 3) {
                    mState = 2;
                }
                RenderName();
                if (mUnk_74 == 1) {
                    data_020d43f0[0].mUnk_5d2 = 1;
                } else {
                    u32 amount = data_ov000_02225fc8[mUnk_74 - 1] * 100;
                    if (amount > 9999990) {
                        amount = 9999990;
                    }
                    data_020d43f0[0].mMoney += amount;
                    if (data_020d43f0[0].mMoney > 9999990) {
                        data_020d43f0[0].mMoney = 9999990;
                    }
                }
                mUnk_74 = 0;
                mUnk_6c = 2;
            }
            mKeyboard->Draw();
            mConfirm->Draw();
            break;
        case 4:
            switch (mMode) {
                case 0: {
                    u8 i = 0;
                    do {
                        data_020d43f0[0].mName[i] = mName[i];
                        i++;
                    } while (i < mMaxLength);
                    break;
                }
                case 1:
                    break;
            }
            mUnk_6c++;
            break;
        case 5:
            SpriteManager::Get().StartFade(3);
            mUnk_6c++;
            break;
        case 6:
            if (!SpriteManager::Get().IsFading()) {
                return;
            }
            mUnk_6c++;
            break;
        case 7:
            RequestScene(1);
            mFinished = 1;
            break;
    }
}

void NameEntryScene::func_ov000_0221b190() {
    switch (mTab) {
        case 0:
            func_02025c1c(1, 7, 0x1c0, mBuf3c + 0x1c0 + mUnk_66 * 0xc0, 0x300, 0);
            break;
        case 1:
            func_02025c1c(1, 7, 0x1c0, mBuf40 + 0x1c0 + mUnk_66 * 0xc0, 0x300, 0);
            break;
    }
}

void NameEntryScene::func_ov000_0221b22c() {
    switch (mTab) {
        case 0:
            func_02025c1c(1, 0xb, 0, mBuf44, 0x5000, 0);
            break;
        case 1:
            func_02025c1c(1, 0xb, 0, mBuf48, 0x5000, 0);
            break;
    }
}

void NameEntryScene::func_ov000_0221b2a0() {
    if (TouchManager::Get().mTouching == 0) {
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
    AddChar((u8) (mCol + (mRow + mUnk_66) * 8 + mTab * 0x60));
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
        if (mState == 3) {
            mState = 2;
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
    SoundManager::Get().func_020246f8(0);
    mKeyboard->mButtons[6].mEnabled = 1;
    mKeyboard->mCursor->mItems[6].mFlags = 8;
    mCellAnims[0]->func_02029fc0();
    if (mLength >= mMaxLength) {
        mCellAnims[2]->SetAnimation(9);
        mState = 3;
    } else {
        mCellAnims[2]->SetAnimation(8);
        mState = 0;
    }
}

void NameEntryScene::Load() {
    SpriteManager::Get().Screen(1).func_02027624(3);
    ChangeDir("/font/");
    mMessage = new MessageWindow("mes_name", 1, 0x3000);
    mMessage->func_02065bd4(0x14, 4, 0);
    mMessage->func_02065be4(0xd8);
    mMessage->func_02065890(0, 0, 1);
    mCharTable = (MessageFile *) func_02023504("mes_Alphabet.mes", 0, 0, 0, 0, 0);
    mMessages = (MessageFile *) func_02023504("mes_name.mes", 0, 0, 0, 0, 0);
    mTextBufferSize = TextManager::Get().BufferSize(0x18, 8);
    mTextBuffer = OS_Alloc(mTextBufferSize);
    MI_CpuClear16(mTextBuffer, mTextBufferSize);
    if (mMode == 0) {
        if (data_020d43f0[0].mName[0] != 0) {
            u8 i = 0;
            do {
                u16 c = data_020d43f0[0].mName[i];
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
            if (mState == 3) {
                mState = 2;
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
    TextManager::Get().mUnk_5b = 1;
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
    endText.mUnk_5b = 0;
    func_02025c1c(SpriteManager::Get().Screen(1).mBgIndex, 4, 0x4000, mTextBuffer, mTextBufferSize, 0);
}

void NameEntryScene::func_ov000_0221bc98() {
    if (mMessage != NULL) {
        mMessage->func_020658d0();
    }
    SpriteManager::Get().Screen(1).func_02027778(mGlyphBuffer, mGlyphCount, 0x33, 0x1e, 0, 0x80, 0xe, 0);
}

void NameEntryScene::ClearTextBuffer() {
    if (mMessage != NULL) {
        mMessage->func_02065878();
    }
    if (mTextBuffer != NULL) {
        MI_CpuClear16(mTextBuffer, mTextBufferSize);
    }
}

void NameEntryScene::CheckSpecialName() {
    BOOL match;
    mUnk_74 = 0;
    u32 value = data_020d43f0[0].mMoney - 3000;
    u32 digit1k = (value / 1000) % 10;
    u32 digit10k = (value / 10000) % 10;
    u32 digit100k = (value / 100000) % 10;
    for (u8 i = 0; i < 6; i++) {
        switch (i) {
            case 0:
                if (data_020d43f0[0].mUnk_5d2 != 0) {
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
        u8 *row = data_ov000_02226044[i];
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
            mUnk_74 = i + 1;
            return;
        }
    }
}


void NameEntryScene::PrintMessage() {
    ClearTextBuffer();
    int index;
    if (mUnk_74 == 1) {
        index = 1;
    } else {
        TextManager::Get().mUnk_10 = data_ov000_02225fc8[mUnk_74 - 1];
        index = 2;
    }
    TextManager &text = TextManager::Get();
    s16 height;
    s16 width;
    width = 0;
    height = 0;
    MessageFile *file = mMessages;
    text.Print(file->GetText(index), file->GetLength(index), mTextBuffer, 0x18, 8, &height, &width, 0, 0, 2, 0x44, 0x10, 0, 0, 1, 1, 1);
    func_02025c1c(SpriteManager::Get().Screen(1).mBgIndex, 4, 0x4000, mTextBuffer, mTextBufferSize, 0);
}

void NameEntryScene::func_ov000_0221c1e4() {
    SpriteManager::Get().Screen(1).func_02027778(mGlyphBuffer, mGlyphCount, 0x20, 0x38, 0, 0x80, 0xe, 0);
}
