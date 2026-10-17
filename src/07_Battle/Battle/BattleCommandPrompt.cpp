#include "System/InputManager.hpp"
#include "System/SoundManager.hpp"
#include "Battle/BattleCommandPrompt.hpp"
#include "Battle/BattleCommandList.hpp"
#include "Battle/BattleCommandPage.hpp"
#include "Game/UnkStruct_020d433c.hpp"
#include "Graphics/CellAnimation.hpp"
#include "Graphics/SpriteManager.hpp"
#include "System/InputManager.hpp"
#include "System/SoundManager.hpp"
#include "System/TouchManager.hpp"
#include "System/FrameUpdate.hpp"
#include "Scene/SceneBase.hpp"
#include "System/FileSystem.hpp"
#include "UI/MessageWindow.hpp"
#include "UI/SelectWindow.hpp"
#include "Graphics/CharacterIcons.hpp"
#include <nitro/gx.h>
#include <nitro/mi.h>
#include <stdio.h>

struct PromptRect {
    u8 x;
    u8 y;
    u8 w;
    u8 h;
};
static const WindowItemRecord sPromptWindowItems[2] = {
    {0, 5, 0xa1, 0x85, 0x36, 0x14, 0, {-1, -1, -1, -1}, 0, 1},
    {1, 2, 0xd9, 0x85, 0x1e, 0x14, 0, {-1, -1, -1, -1}, 0, 1},
};
static const WindowCellRecord sPromptWindowCells[2] = {
    {"/Game/2D/union_b/union_b_obj_cockpit_nce.bin", 0, 0x1c800, 14, 0, 1, {0, 1, 1, 0}},
    {"/Game/2D/union_b/union_b_obj_cockpit_nce.bin", 2, 0x1c800, 14, 0, 1, {2, 3, 3, 0}},
};
static const PromptRect data_ov007_0227a764[3] = {{0x28, 0x3f, 0x30, 0x40}, {0x68, 0x3f, 0x30, 0x40}, {0xa8, 0x3f, 0x30, 0x40}};
static const PromptRect data_ov007_0227a75c[2] = {{0xa1, 0x85, 0x36, 0x14}, {0xd9, 0x85, 0x1e, 0x14}};
static const u16 data_ov007_0227a770[6] = {0x2000, 0xc000, 0, 0, 0, 0};
extern MtxFx22 gIdentityMtx22;
extern "C" void MTX_ScaleRot22(MtxFx22 *mtx, fx32 scaleX, fx32 scaleY, u16 angle);



BattleCommandPrompt::BattleCommandPrompt(u32 mode, void *target, u32 flags, u32 charOffset, u8 player)
    : SceneBase(0x3e8, 0) {
    mCharacterIcons = 0;
    mSlideX = 0x100;
    mSlideOutDelayFrames = 0;
    mState = 0;
    mUnk_8d = 0xff;
    mStep = 0;
    mCursorPos = 0;
    mScroll = 0;
    mUnk_91 = 0;
    MI_CpuClear32(mBakuganSubCellAnims, 0xc);
    MI_CpuClear32(mStateCellAnims, 0xc);
    MI_CpuClear32(mCharacterIconCellAnims, 0xc);
    mDisabledBits[0] = flags;
    u8 i = 0;
    do {
        if (!(mDisabledBits[(u32) i >> 3] & (1 << ((int) i % 8)))) {
            mCursorPos = i;
            break;
        }
        i++;
    } while (i < 3);
    mCursorAnim = 0;
    mDigitCellAnim = 0;
    mMessageWindow = 0;
    mCharOffset = charOffset;
    mMode = mode;
    mPlayer = player;
    if (mMode >= 5) {
        BattlePlayerList *list = data_020d433c.mBattlePlayers;
        if (mPlayer == list->mUnk_2a9) {
            mCollectMode = 1;
        } else if (mPlayer == list->mPlayers[list->mUnk_2a9]->mPartner) {
            mCollectMode = 1;
        } else {
            mCollectMode = 2;
        }
    } else {
        mCollectMode = 0;
    }
    switch (mMode) {
        case 0:
        case 1:
            mCommandPage = (BattleCommandPage *) target;
            mPlayer = data_020d433c.mBattlePlayers->mUnk_2a9;
            mUnk_8d = 0xff;
            CollectCards((const BattlePromptSlot *) &data_020d433c.mBattlePlayers->mPlayers[mPlayer]->Slot54(0));
            break;
        case 3:
            mCommandPage = (BattleCommandPage *) target;
            mPlayer = data_020d433c.mBattlePlayers->mUnk_2a9;
            mUnk_8d = 0xff;
            CollectCards((const BattlePromptSlot *) &data_020d433c.mBattlePlayers->mPlayers[mPlayer]->Slot6c(0));
            break;
        case 2:
            mCommandList = (BattleCommandList *) target;
            mPlayer = data_020d433c.mBattlePlayers->mUnk_2a9;
            mUnk_8d = 0xff;
            CollectRecords();
            break;
        case 6:
            mCommandList = (BattleCommandList *) target;
            mPlayer = player;
            mUnk_8d = player;
            CollectRecords();
            break;
        case 5:
            mCommandPage = (BattleCommandPage *) target;
            mPlayer = player;
            mUnk_8d = player;
            CollectCards((const BattlePromptSlot *) &data_020d433c.mBattlePlayers->mPlayers[mPlayer]->Slot54(0));
            break;
        case 7:
            mCommandPage = (BattleCommandPage *) target;
            mPlayer = player;
            mUnk_8d = player;
            CollectCards((const BattlePromptSlot *) &data_020d433c.mBattlePlayers->mPlayers[mPlayer]->Slot6c(0));
            break;
    }
    mWindow = new SelectWindow(2, sPromptWindowItems, -1);
    MtxFx22 mtx;
    MTX_ScaleRot22(&mtx, 0x1000, 0x1000, 0);
    G2x_SetBGyAffine_((vu16 *) 0x4001030, &mtx, 0, 0, -mSlideX, 0);
}

BattleCommandPrompt::~BattleCommandPrompt() {
    CellAnimation *c;
    CellAnimation *d;
    u8 i;
    CellAnimation *a;
    u8 j;
    u8 k;
    i = 0;
    do {
        a = mBakuganSubCellAnims[i];
        if (a) {
            delete a;
        }
        i++;
    } while (i < 3);
    j = 0;
    do {
        c = mStateCellAnims[j];
        if (c) {
            delete c;
        }
        j++;
    } while (j < 3);
    if (mCursorAnim) {
        delete mCursorAnim;
    }
    if (mDigitCellAnim) {
        delete mDigitCellAnim;
    }
    if (mWindow) {
        delete mWindow;
    }
    if (mMessageWindow) {
        delete mMessageWindow;
    }
    if (mCharacterIcons) {
        delete mCharacterIcons;
    }
    k = 0;
    do {
        d = mCharacterIconCellAnims[k];
        if (d) {
            delete d;
        }
        k++;
    } while (k < 3);
    MtxFx22 mtx;
    MTX_ScaleRot22(&mtx, 0x1000, 0x1000, 0);
    G2x_SetBGyAffine_((vu16 *) 0x4001030, &mtx, 0, 0, 0, 0);
    G2S_SetBG3Priority(3);
    GXS_SetVisibleWnd(0);
    G2S_SetBlendBrightness(0x1f, 0);
}

void ApplySlideX(s16 *x);
void InitPromptSubScreen(BattleCommandPrompt *prompt);

void BattleCommandPrompt::Update() {
    switch (mState) {
        case 0: {
            InitPromptSubScreen(this);
            Load();
            SelectWindow *win = mWindow;
            WindowItem *items = win->mCursor->mItems;
            if (mMode == 0 || data_020d433c.mUnk_83 == 4) {
                win->mButtons[1].mEnabled = 0;
                items[1].mFlags           = 0x8000;
            } else {
                win->mButtons[1].mEnabled = 1;
                items[1].mFlags           = 2;
            }
            if (mMode >= 5) {
                mWindow->mButtons[0].mEnabled = 0;
            } else {
                mWindow->mButtons[0].mEnabled = 1;
            }
            G2S_SetBG0Priority(3);
            G2S_SetBG3Priority(1);
            mSlideOutDelayFrames = 0;
            mState++;
            return;
        }
        case 1:
            switch (mMode) {
                case 0:
                case 1:
                case 3:
                    mMessageWindow->StartMessage(0x11, 0, 1);
                    break;
                case 2:
                    mMessageWindow->StartMessage(0x12, 0, 1);
                    break;
                case 5:
                    if (mPlayer == data_020d433c.mBattlePlayers->mUnk_2a9) {
                        mMessageWindow->StartMessage(0x27, 0, 1);
                    } else {
                        mMessageWindow->StartMessage((s16) (mPlayer + 0x28), 0, 1);
                    }
                    break;
                case 7:
                    if (mPlayer == data_020d433c.mBattlePlayers->mUnk_2a9) {
                        mMessageWindow->StartMessage(0x2c, 0, 1);
                    } else {
                        mMessageWindow->StartMessage((s16) (mPlayer + 0x2d), 0, 1);
                    }
                    break;
                case 6:
                    if (mPlayer == data_020d433c.mBattlePlayers->mUnk_2a9) {
                        mMessageWindow->StartMessage(0x22, 0, 1);
                    } else {
                        mMessageWindow->StartMessage((s16) (mPlayer + 0x23), 0, 1);
                    }
                    break;
            }
            GXS_SetVisiblePlane(0x19);
            mState++;
            return;
        case 2:
            switch (mStep) {
                case 0:
                    mSlideX -= 0x18;
                    if (mSlideX <= 0) {
                        mSlideX = 0;
                        mStep = 1;
                        mWindow->mEnabled = 1;
                    }
                    QueueFrameCallback((FrameCallback) ApplySlideX, &mSlideX);
                    break;
                case 1: {
                    int idx            = mCursorPos + mScroll;
                    SelectWindow *win  = mWindow;
                    WindowItem *items  = win->mCursor->mItems;
                    u8 bits = mDisabledBits[idx / 8];
                    if ((bits & (1 << (idx % 8))) || mMode >= 5) {
                        win->mButtons[0].mEnabled = 0;
                        items[0].mFlags           = 0x8000;
                    } else {
                        win->mButtons[0].mEnabled = 1;
                        items[0].mFlags           = 5;
                    }
                    switch ((u8) mWindow->Update(1)) {
                        case 1:
                            mStep = 3;
                            mSlideOutDelayFrames = 0;
                            break;
                        case 2:
                            if (mMode != 0) {
                                mUnk_91 = 1;
                                mStep = 3;
                                mSlideOutDelayFrames = 0;
                            }
                            break;
                    }
                    if (mWindow->mState == 1) {
                        if (InputManager::Get().IsTrigger(KEY_RIGHT)) {
                            u8 cur = mCursorPos + mScroll;
                            if (cur < mCount - 1) {
                                mCursorPos++;
                                if (mCursorPos >= 3) {
                                    mCursorPos = 2;
                                    mScroll++;
                                    if (mScroll > 3) {
                                        mScroll = 3;
                                    }
                                }
                                u8 sel = mCursorPos + mScroll;
                                switch (mMode) {
                                    case 0:
                                    case 1:
                                    case 3:
                                    case 5:
                                    case 7:
                                        SetCommandPageCard(mCommandPage, mCardIds[sel], (s8) mUnk_8d);
                                        break;
                                    case 2:
                                    case 6: {
                                        BattleRecord *rec = mRecords[sel];
                                        SetCommandListBakugan(mCommandList, rec->mBakuganId, rec->mAttribute, (s8) mUnk_8d);
                                        rec = mRecords[sel];
                                        func_0205afb4(mCommandList, rec->mAttribute, (u16) (rec->mUnk_04 + rec->mUnk_06), (BattleCellPair *) rec);
                                        break;
                                    }
                                }
                            }
                            if (cur != mCursorPos) {
                                SoundManager::Get().StartSeqArc(5);
                            }
                        }
                        if (InputManager::Get().IsTrigger(KEY_LEFT)) {
                            u8 cur = mCursorPos + mScroll;
                            if (cur != 0) {
                                if (mCursorPos != 0) {
                                    mCursorPos--;
                                    if (mCursorPos >= 3) {
                                        mCursorPos = 0;
                                    }
                                } else {
                                    mScroll--;
                                    if (mScroll > 3) {
                                        mScroll = 0;
                                    }
                                }
                                u8 sel = mCursorPos + mScroll;
                                switch (mMode) {
                                    case 0:
                                    case 1:
                                    case 3:
                                    case 5:
                                    case 7:
                                        SetCommandPageCard(mCommandPage, mCardIds[sel], (s8) mUnk_8d);
                                        break;
                                    case 2:
                                    case 6: {
                                        BattleRecord *rec = mRecords[sel];
                                        SetCommandListBakugan(mCommandList, rec->mBakuganId, rec->mAttribute, (s8) mUnk_8d);
                                        rec = mRecords[sel];
                                        func_0205afb4(mCommandList, rec->mAttribute, (u16) (rec->mUnk_04 + rec->mUnk_06), (BattleCellPair *) rec);
                                        break;
                                    }
                                }
                            }
                            if (cur != mCursorPos) {
                                SoundManager::Get().StartSeqArc(5);
                            }
                        }
                        u8 i = 0;
                        do {
                            u32 tx = TouchManager::Get().mX;
                            u32 ty = TouchManager::Get().mY;
                            if (data_ov007_0227a764[i].x < tx && tx < data_ov007_0227a764[i].x + data_ov007_0227a764[i].w && data_ov007_0227a764[i].y < ty && ty < data_ov007_0227a764[i].y + data_ov007_0227a764[i].h) {
                                if (TouchManager::Get().mReleased) {
                                    if (i <= mCount - 1) {
                                        if (mCursorPos != i) {
                                            SoundManager::Get().StartSeqArc(5);
                                            mCursorPos = i;
                                        } else if (mWindow->mButtons[0].mEnabled != 0) {
                                            SoundManager::Get().StartSeqArc(0);
                                            mStep = 3;
                                            mSlideOutDelayFrames = 0;
                                        }
                                        switch (mMode) {
                                            case 0:
                                            case 1:
                                            case 3:
                                            case 5:
                                            case 7:
                                                SetCommandPageCard(mCommandPage, mCardIds[mCursorPos + mScroll], (s8) mUnk_8d);
                                                break;
                                            case 2:
                                            case 6: {
                                                SetCommandListBakugan(mCommandList, mRecords[mCursorPos + mScroll]->mBakuganId, mRecords[mCursorPos + mScroll]->mAttribute, (s8) mUnk_8d);
                                                BattleRecord *rec = mRecords[mCursorPos + mScroll];
                                                func_0205afb4(mCommandList, rec->mAttribute, (u16) (rec->mUnk_04 + rec->mUnk_06), (BattleCellPair *) rec);
                                                break;
                                            }
                                        }
                                    }
                                }
                            }
                            i++;
                        } while (i < 3);
                    }
                    break;
                }
                case 2:
                    mSlideX -= 0x18;
                    if (mSlideX <= -0x100) {
                        mState = 6;
                    }
                    QueueFrameCallback((FrameCallback) ApplySlideX, &mSlideX);
                    break;
                case 3:
                    mSlideOutDelayFrames++;
                    if ((s32) mSlideOutDelayFrames > 0x14) {
                        mSlideOutDelayFrames = 0;
                        mStep = 2;
                    }
                    break;
            }
            switch (mMode) {
                case 3: {
                    u8 n = mCount;
                    if (n > 3) {
                        n = 3;
                    }
                    u8 i = 0;
                    do {
                        if (mDisabledBits[(u32) i >> 3] & (1 << ((int) i % 8))) {
                            mStateCellAnims[2]->Draw(1, (s16) (mSlideX + data_ov007_0227a764[i].x), data_ov007_0227a764[i].y, 0);
                        }
                        i++;
                    } while (i < n);
                    break;
                }
                case 5:
                case 7: {
                    u8 n = mCount;
                    if (n > 3) {
                        n = 3;
                    }
                    u8 i = 0;
                    do {
                        if ((mKinds + mScroll)[i] != 0) {
                            mStateCellAnims[0]->Draw(1, (s16) (mSlideX + data_ov007_0227a764[i].x), data_ov007_0227a764[i].y, 0);
                        }
                        i++;
                    } while (i < n);
                    break;
                }
                case 6: {
                    u8 n = mCount;
                    if (n > 3) {
                        n = 3;
                    }
                    u8 i = 0;
                    do {
                        u8 v = (mKinds + mScroll)[i];
                        if (v == 2) {
                            mStateCellAnims[0]->Draw(1, (s16) (mSlideX + data_ov007_0227a764[i].x), data_ov007_0227a764[i].y, 0);
                        } else if (v == 1) {
                            mStateCellAnims[1]->Draw(1, (s16) (mSlideX + data_ov007_0227a764[i].x), data_ov007_0227a764[i].y, 0);
                        }
                        i++;
                    } while (i < n);
                    break;
                }
            }
            mCursorAnim->Update();
            mCursorAnim->Draw(1, (s16) (mSlideX + data_ov007_0227a764[mCursorPos].x), data_ov007_0227a764[mCursorPos].y, 0);
            mMessageWindow->func_02065bd4((s16) (mSlideX + 0x20), 0x28, 0);
            mMessageWindow->Update();
            {
                u8 i = 0;
                do {
                    SelectWindow *w = mWindow;
                    w->mCellPos[i].x = mSlideX + data_ov007_0227a75c[i].x;
                    w->mCellPos[i].y = data_ov007_0227a75c[i].y;
                    i++;
                } while (i < 2);
            }
            mWindow->Draw();
            mStateCellAnims[0]->Update();
            mStateCellAnims[1]->Update();
            if (mCharacterIcons) {
                mCharacterIconCellAnims[1]->Draw(1, 0x86, 1, 0);
                if (data_020d433c.mBattlePlayers->mPlayers[mPlayer]->IsCpu()) {
                    mCharacterIconCellAnims[2]->Draw(1, 0x86, 1, 0);
                }
                mCharacterIcons->DrawIcons();
                mCharacterIconCellAnims[0]->Draw(1, 0x86, 3, 0);
            }
            switch (mMode) {
                case 0:
                case 1:
                case 3:
                case 5:
                case 7: {
                    SpriteScreen &screen = SpriteManager::Get().Screen(1);
                    screen.SetObjVramMode(3);
                    u8 n = mCount;
                    if (n > 3) {
                        n = 3;
                    }
                    u8 i = 0;
                    do {
                        screen.AddOam((void *)data_ov007_0227a770, 1, (s16) (mSlideX + data_ov007_0227a764[i].x), data_ov007_0227a764[i].y, 0,
                                             ((mCharOffset + (i + mScroll) * 0x1800) << 9) >> 16, (s8) (mScroll + (i + 10)), 0);
                        i++;
                    } while (i < n);
                    return;
                }
                case 2:
                case 6: {
                    u8 n = mCount;
                    if (n > 3) {
                        n = 3;
                    }
                    u8 i = 0;
                    do {
                        BattleRecord *rec = mRecords[i + mScroll];
                        u8 q              = (u8) ((rec->mUnk_04 + rec->mUnk_06) / 10);
                        u8 digits[3];
                        digits[0] = q / 10;
                        digits[1] = q % 10;
                        digits[2] = 0;
                        u8 j      = 0;
                        do {
                            mDigitCellAnim->DrawFrame(1, digits[j], (s16) (mSlideX + data_ov007_0227a764[i].x + 4 + j * 13),
                                                   (s16) (data_ov007_0227a764[i].y + 0x36), 0);
                            j++;
                        } while (j < 3);
                        mBakuganSubCellAnims[i + mScroll]->Update();
                        mBakuganSubCellAnims[i + mScroll]->DrawFrame(1, mRecords[i + mScroll]->mAttribute, (s16) (mSlideX + data_ov007_0227a764[i].x),
                                                            data_ov007_0227a764[i].y, 0);
                        i++;
                    } while (i < n);
                    return;
                }
            }
            break;
        case 6:
            if (mUnk_91) {
                RequestScene(10);
                mFinished = 1;
            } else {
                RequestScene(mNextScenes[mCursorPos + mScroll]);
                mFinished = 1;
            }
            break;
        case 7:
            mFinished = 1;
            break;
    }
}

void CloseCommandPrompt(BattleCommandPrompt *prompt, u32 flag) {
    prompt->mState = 7;
}

void InitPromptSubScreen(BattleCommandPrompt *prompt) {
    G2S_SetBG0Priority(3);
    G2S_SetBG3Priority(0);
    GXS_SetVisiblePlane(0x11);
}

void BattleCommandPrompt::Load() {
    mMessageWindow = new MessageWindow("mes_Command", 1, 0x1f000);
    mMessageWindow->func_02065bd4(0x20, 0x28, 0);
    mMessageWindow->SetVram(0x1f000);
    SpriteScreen &screen = SpriteManager::Get().Screen(1);
    screen.SetObjVramMode(3);
    ChangeDir("/Game/2D/union_b");
    LoadAndQueueUpload(1, 4, 0x1c800, "union_b_obj_cockpit.nce_ncg1D.bin", 0, 0, 0);
    LoadAndQueueUpload(1, 5, 0x1c0, "union_b_obj_cockpit_ncl.bin", 0, 0, 0);
    mWindow->SetCells(2, sPromptWindowCells);
    LoadAndQueueUpload(1, 4, 0x1b800, "union_b_obj_cursor.nce_ncg1D.bin", 0, 0, 0);
    LoadAndQueueUpload(1, 5, 0x1a0, "union_b_obj_cursor_ncl.bin", 0, 0, 0);
    LoadAndQueueUpload(1, 4, 0x1a000, "union_b_obj_gp.nce_ncg1D.bin", 0, 0, 0);
    LoadAndQueueUpload(1, 5, 0x180, "union_b_obj_gp_ncl.bin", 0, 0, 0);
    ChangeDir("/Game/Battle/Obj");
    LoadAndQueueUpload(1, 4, 0x19000, "battle_b_obj_state.nce_ncg1D.bin", 0, 0, 0);
    LoadAndQueueUpload(1, 5, 0x100, "battle_b_obj_state_ncl.bin", 0, 0, 0);
    mStateCellAnims[0] = new CellAnimation("battle_b_obj_state_nce.bin", 0, 1, 1);
    mStateCellAnims[0]->mCharBase = screen.GetCharBase(0x19000);
    mStateCellAnims[0]->mPaletteBase   = 8;
    mStateCellAnims[1] = new CellAnimation("battle_b_obj_state_nce.bin", 1, 1, 1);
    mStateCellAnims[1]->mCharBase = screen.GetCharBase(0x19000);
    mStateCellAnims[1]->mPaletteBase   = 8;
    mStateCellAnims[2] = new CellAnimation("battle_b_obj_state_nce.bin", 2, 1, 1);
    mStateCellAnims[2]->mCharBase = screen.GetCharBase(0x19000);
    mStateCellAnims[2]->mPaletteBase   = 8;
    switch (mMode) {
        case 0:
        case 1:
        case 3:
        case 5:
        case 7: {
            ChangeDir("/Game/Battle/Bg");
            LoadAndQueueUpload(1, 1, 0x6000, "battle_b_bg_window_ncl.bin", 0, 0, 0);
            LoadAndQueueUpload(1, 0xe, 0, "battle_b_bg_window_ncg.bin", 0, 0, 0);
            LoadAndQueueUpload(1, 0xa, 0, "battle_b_bg_window_a_nsc.bin", 0, 0, 0);
            ChangeDir("/Game/Card/Graphic/Large");
            u8 i = 0;
            do {
                char name[0x100];
                u32 size;
                sprintf(name, "card_l_%03d.nce_ncg1D.bin", GetCardNumber(mCardIds[i]));
                void *data = LoadFile(name, 0, 0, 0, &size, 0);
                QueueUpload(1, 4, mCharOffset + i * 0x1800, screen.DownsampleCharacters((u8 *) data), 0x1000, 1);
                sprintf(name, "card_l_%03d_ncl.bin", GetCardNumber(mCardIds[i]));
                LoadAndQueueUpload(1, 0, 0x1400 + i * 0x200, name, 0, 0, 0);
                i++;
            } while (i < mCount);
            ChangeDir("/Game/2D/union_b");
            mCursorAnim = new CellAnimation("union_b_obj_cursor_nce.bin", 0, 1, 1);
            mCursorAnim->mCharBase = screen.GetCharBase(0x1b800);
            mCursorAnim->mPaletteBase   = 0xd;
            if (mDisabledBits[0]) {
                GXS_SetVisibleWnd(4);
                G2S_SetWndOBJInsidePlane(0x10, TRUE);
                G2S_SetWndOutsidePlane(0x1b, FALSE);
                G2S_SetBlendBrightness(0x10, -8);
            }
            break;
        }
        case 2:
        case 6: {
            ChangeDir("/Game/Battle/Bg");
            LoadAndQueueUpload(1, 1, 0x6000, "battle_b_bg_window_ncl.bin", 0, 0, 0);
            LoadAndQueueUpload(1, 0xe, 0, "battle_b_bg_window_ncg.bin", 0, 0, 0);
            LoadAndQueueUpload(1, 0xa, 0, "battle_b_bg_window_a_nsc.bin", 0, 0, 0);
            ChangeDir("/Game/Battle/Obj");
            LoadAndQueueUpload(1, 4, 0, "bakugan_sub.nce_ncg1D.bin", 0, 0, 0);
            LoadAndQueueUpload(1, 0, 0x200, "bakugan_sub_ncl.bin", 0, 0, 0);
            u8 i = 0;
            do {
                mBakuganSubCellAnims[i] = new CellAnimation("bakugan_sub_nce.bin", 0, 1, 1);
                mBakuganSubCellAnims[i]->mCharBase = screen.GetCharBase(0);
                mBakuganSubCellAnims[i]->mPaletteBase   = 1;
                i++;
            } while (i < mCount);
            ChangeDir("/Game/2D/union_b");
            mCursorAnim = new CellAnimation("union_b_obj_cursor_nce.bin", 1, 1, 1);
            mCursorAnim->mCharBase = screen.GetCharBase(0x1b800);
            mCursorAnim->mPaletteBase   = 0xd;
            mDigitCellAnim = new CellAnimation("union_b_obj_gp_nce.bin", 2, 1, 1);
            mDigitCellAnim->mCharBase = screen.GetCharBase(0x1a000);
            mDigitCellAnim->mPaletteBase   = 0xc;
            break;
        }
    }
    switch (mMode) {
      case 5:
      case 6:
      case 7: {
        ChangeDir("/Game/Character/Icon");
        LoadAndQueueUpload(1, 4, 0x14000, "icon_ch_base.nce_ncg1D.bin", 0, 0, 0);
        LoadAndQueueUpload(1, 5, 0x120, "icon_ch_base_ncl.bin", 0, 0, 0);
        u8 j = 0;
        do {
            mCharacterIconCellAnims[j] = new CellAnimation("icon_ch_base_nce.bin", 0, 1, 1);
            mCharacterIconCellAnims[j]->mCharBase = screen.GetCharBase(0x14000);
            mCharacterIconCellAnims[j]->mPaletteBase   = 9;
            j++;
        } while (j < 3);
        mCharacterIconCellAnims[0]->SetAnimation(0);
        mCharacterIconCellAnims[1]->SetAnimation(mPlayer + 7);
        mCharacterIconCellAnims[2]->SetAnimation(0xb);
        mCharacterIconCellAnims[2]->mFrameIndex = mPlayer;
        ChangeDir("/");
        mCharacterIcons = new CharacterIcons(0x16000, 0xb, 1);
        BattlePlayer *player = data_020d433c.mBattlePlayers->mPlayers[mPlayer];
        mCharacterIcons->SetCharacter(0, (s8) player->mCharacterId, player->NameInfo(), 0);
        mCharacterIcons->SetDrawArgs(0, 0x88, 5, 0);
    }
    }
    ChangeDir("/");
}

void ApplySlideX(s16 *x) {
    G2x_SetBGyAffine_((vu16 *) 0x4001030, &gIdentityMtx22, 0, 0, -*x, 0);
}

extern "C" u8 GetLowByte(u32 v) {
    return (u8) v;
}

void BattleCommandPrompt::CollectCards(const BattlePromptSlot *slot) {
    BOOL add;
    u8 i = 0;
    mCount = 0;
    do {
        add = 0;
        switch (mCollectMode) {
            case 0:
                if (slot->mKind == 0) {
                    mCardIds[mCount] = slot->mId;
                    add              = 1;
                }
                break;
            case 2:
                if (slot->mKind != 2) {
                    mCardIds[mCount] = mMode == 7 ? 0xd6 : 0;
                } else {
                    mCardIds[mCount] = slot->mId;
                }
                add = 1;
                break;
            case 1:
                mCardIds[mCount] = slot->mId;
                add              = 1;
                break;
        }
        if (add) {
            mNextScenes[mCount] = i | 0x100 | (mPlayer << 10);
            mKinds[mCount] = slot->mKind;
            mCount++;
        }
        i++;
        slot++;
    } while (i < data_020d433c.mUnk_99);
    SetCommandPageCard(mCommandPage, mCardIds[mCursorPos + mScroll], (s8) mUnk_8d);
}

void BattleCommandPrompt::CollectRecords() {
    u8 i = 0;
    BOOL ok;
    BattleRecord *rec;
    mCount = 0;
    do {
        rec = &data_020d433c.mBattlePlayers->mPlayers[mPlayer]->mRecords[i];
        ok  = 1;
        if (mCollectMode == 0) {
            if (rec->mStatus != 0) {
                ok = 0;
            }
        }
        if (ok) {
            mRecords[mCount] = rec;
            mKinds[mCount] = rec->mStatus;
            mNextScenes[mCount] = i | 0x200 | (mPlayer << 10);
            mCount++;
        }
        if ((u8) (rec->mBakuganId + 0xfb) <= 1) {
            if (rec->mStatus == 1) {
                mCommandList->mUnk_161 = 1;
            }
        }
        i++;
    } while (i < data_020d433c.mUnk_99);
    SetCommandListBakugan(mCommandList, mRecords[mCursorPos + mScroll]->mBakuganId, mRecords[mCursorPos + mScroll]->mAttribute, (s8) mUnk_8d);
    BattleRecord *r = mRecords[mCursorPos + mScroll];
    func_0205afb4(mCommandList, r->mAttribute, (u16) (r->mUnk_04 + r->mUnk_06), (BattleCellPair *) r);
}
