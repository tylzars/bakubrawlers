#include "Unknown/UnkStruct_02071d24.hpp"

#include "Battle/BattleCommandList.hpp"
#include "Game/SaveData.hpp"
#include "Graphics/CellAnimation.hpp"
#include "Graphics/Display.hpp"
#include "System/FileSystem.hpp"
#include "System/GameSystem.hpp"
#include "UI/MessageWindow.hpp"
#include "UI/SelectWindow.hpp"

#include <nitro/gx.h>
#include <nitro/mi.h>
#include <nitro/os.h>

// The singleton headers around the tables give the original node order (TextManager's node first).
#include "Graphics/SpriteManager.hpp"
#include "System/TouchManager.hpp"
#include "System/InputManager.hpp"
#include "System/SoundManager.hpp"

static const u8 data_020a2e3c[12] = {0, 10, 10, 20, 20, 30, 30, 40, 40, 50, 0, 0};

static const WindowItemRecord data_020a2e48[7] = {
    {0, 8, 76, 48, 160, 20, 0, {4, 1, -1, -1}, 0, 2},
    {1, 8, 76, 72, 160, 20, 0, {0, 2, -1, -1}, 0, 2},
    {2, 8, 76, 96, 160, 20, 0, {1, 3, -1, -1}, 0, 2},
    {3, 8, 76, 120, 160, 20, 0, {2, 4, -1, -1}, 0, 2},
    {4, 8, 76, 144, 160, 20, 0, {3, 0, -1, -1}, 0, 2},
    {5, 0x8000, 120, 18, 78, 20, 0, {-1, -1, -1, -1}, 0, 1},
    {6, 8, 89, 152, 78, 20, 0, {-1, -1, -1, -1}, 0, 2},
};

static const WindowCellRecord data_020a2f0c[7] = {
    {"/Game/Battle/Obj/result_b_obj_01_nce.bin", 1, 0x4000, 0, 1, 1, {0, 2, 1, 0}},
    {"/Game/Battle/Obj/result_b_obj_01_nce.bin", 3, 0x4000, 0, 1, 1, {3, 5, 4, 0}},
    {"/Game/Battle/Obj/result_b_obj_01_nce.bin", 6, 0x4000, 0, 1, 1, {6, 8, 7, 0}},
    {"/Game/Battle/Obj/result_b_obj_01_nce.bin", 9, 0x4000, 0, 1, 1, {9, 11, 10, 0}},
    {"/Game/Battle/Obj/result_b_obj_01_nce.bin", 12, 0x4000, 0, 1, 1, {12, 14, 13, 0}},
    {"/Game/2D/union_b/union_b_obj_window_nce.bin", 9, 0xf000, 11, 0, 1, {9, 9, 9, 0}},
    {"/Game/2D/union_b/union_b_obj_window_nce.bin", 1, 0xf000, 11, 0, 1, {1, 2, 1, 0}},
};

#include "Text/TextManager.hpp"

extern "C" void DC_FlushRange(void *buffer, u32 size);
void func_02072614();

UnkStruct_02071d24::UnkStruct_02071d24(u32 unk1) :
    SceneBase(2, 0) {
    mBakuganNameMessages = NULL;
    mResultMessages = NULL;
    mMessageWindow = NULL;
    mCommandList = NULL;
    mWindow = NULL;
    mUnk_4c = unk1;
    mState = 0;
    mBakuganIndex = 0;
    mUnk_4f = 0;
    mUnk_50 = 0;
    mUnk_51 = 0;
    mUnk_52 = 0;
    mUnk_53 = 0;
    mPendingPlanes = -1;
    mBakuganSubCellAnim = NULL;
    mTextBuffer = NULL;
    mGlyphBuffer = NULL;
    mTextBufferSize = 0;
    mGlyphCount = 0;
    MI_CpuClear8(mBakuganIds, 6);
    MI_CpuClear8(mAttributes, 6);
    MI_CpuClear8(mUnk_64, 6);
    for (u8 i = 0; i < 6; i++) {
        mUnk_34[i] = NULL;
    }
}

UnkStruct_02071d24::~UnkStruct_02071d24() {
    delete mBakuganSubCellAnim;
    delete mWindow;
    if (mBakuganNameMessages != NULL) {
        OS_Free(mBakuganNameMessages);
        mBakuganNameMessages = NULL;
    }
    if (mResultMessages != NULL) {
        OS_Free(mResultMessages);
        mResultMessages = NULL;
    }
    delete mMessageWindow;
    if (mTextBuffer != NULL) {
        ClearText();
        OS_Free(mTextBuffer);
    }
    if (mGlyphBuffer != NULL) {
        OS_Free(mGlyphBuffer);
        mGlyphBuffer = NULL;
    }
}

void UnkStruct_02071d24::Update() {
    switch (mState) {
        case 0:
            func_02072614();
            LoadGraphics();
            LoadMessages();
            RefreshItems();
            func_02072c40();
            mState++;
            break;
        case 1:
            if (mCommandList->IsSettingUp() != 0) {
                break;
            }
            SpriteManager::Get();
            SpriteManager::sFade[1] = FADE_MODE_TO_ZERO;
            mState++;
            break;
        case 2:
            if (!SpriteManager::Get().IsFading()) {
                mState++;
            }
            func_02072e7c();
            break;
        case 3:
            if (func_02072d20()) {
                SoundManager::Get().StartSeqArc(0x34);
                SetFlagBit(data_020d49b0, mAttributes[mBakuganIndex]);
            }
            mState++;
            if (mUnk_4c != 2) {
                func_02072b60();
            }
            func_02072e7c();
            break;
        case 4: {
            WindowItem *items;
            if (mCommandList->IsCountingUp() != 0) {
                mUnk_52 = 1;
            } else {
                items = mWindow->mCursor->mItems;
                mUnk_52 = 0;
                if (mUnk_50 == 3) {
                    if ((u8) mWindow->GetSelectedId() == 6) {
                        if (mUnk_53 == 1) {
                            mUnk_50 = 0;
                            mUnk_53 = 2;
                            PrintText();
                        } else {
                            mPendingPlanes = 0x11;
                            mWindow->mButtons[0].mEnabled = 1;
                            mWindow->mButtons[1].mEnabled = 1;
                            mWindow->mButtons[2].mEnabled = 1;
                            mWindow->mButtons[3].mEnabled = 1;
                            mWindow->mButtons[4].mEnabled = 1;
                            items[0].mFlags = 8;
                            items[1].mFlags = 8;
                            items[2].mFlags = 8;
                            items[3].mFlags = 8;
                            items[4].mFlags = 8;
                            mWindow->mButtons[5].mEnabled = 0;
                            mWindow->mButtons[6].mEnabled = 0;
                            items[6].mFlags = 0x8000;
                            items[6].mY     = 0xc0;
                            items[4].mY     = 0x90;
                            mWindow->Update(1);
                            mWindow->Reset(-1);
                            mUnk_50 = 0;
                            RefreshItems();
                            mUnk_53 = 0;
                            PrintText();
                            mState++;
                            mUnk_52 = 0x1e;
                        }
                    }
                }
            }
            func_02072e7c();
            break;
        }
        case 5: {
            s32 plane = mPendingPlanes;
            if (plane != -1) {
                GXS_SetVisiblePlane(plane);
                mPendingPlanes = -1;
            }
            mUnk_52--;
            if (mUnk_52 == 0) {
                mState++;
            }
            func_02072e7c();
            break;
        }
        case 6:
            if (mCommandList->IsCountingUp() != 0) {
                mUnk_52 = 1;
            } else {
                mUnk_52 = 0;
                if (mUnk_51 != 0) {
                    if (InputManager::Get().IsPressed(1) | TouchManager::Get().mPressed) {
                        mUnk_51 = 0;
                    } else {
                        mUnk_51--;
                    }
                } else {
                    func_020733d4();
                }
            }
            func_02072e7c();
            break;
        case 7:
            func_02072dfc();
            if (mCommandList != NULL) {
                CloseCommandList(mCommandList);
                mContext->mRequest = 0;
            }
            mState++;
            SpriteManager::Get().StartFade(FADE_MODE_DOWN);
            break;
        case 8:
            func_02072dfc();
            if (SpriteManager::Get().IsFading()) {
                break;
            }
            if (mCommandList != NULL) {
                if (mContext->mRequest != 0) {
                    mCommandList = NULL;
                } else {
                    return;
                }
            }
            mState++;
            break;
        case 9:
            RequestScene(2);
            mFinished = 1;
            break;
    }
}

void func_02072614() {
    GX_SetDispSelect(1);
    SpriteManager::Get().SetupMode(0, 1);
    GX_SetGraphicsMode(1, 0, 0);
    G2S_SetBG0Control(0, 1, 0, 1, 0);
    G2S_SetBG1Control(0, 1, 2, 4, 0);
    G2S_SetBG0Offset(0, 0);
    G2S_SetBG1Offset(0, 0);
    G2S_SetBG0Priority(3);
    G2S_SetBG1Priority(0);
    GXS_SetVisiblePlane(0x11);
    G2S_SetBlendBrightness(0x1f, 0);
    GXS_SetVisibleWnd(0);
    G2S_BlendNone();
}

// Also creates the BattleCommandList for the current entry (:286) and passes it the entry's save value (func_02072b14)
// when it has a record.
void UnkStruct_02071d24::LoadGraphics() {
    ChangeDir("/Game/Battle/Bg/");
    LoadAndQueueUpload(1, UPLOAD_KIND_BG0_SCR, 0, "result_b_bg_01_nsc.bin", 0, 0, 0);
    LoadAndQueueUpload(1, UPLOAD_KIND_BG0_CHAR, 0, "result_b_bg_01_ncg.bin", 0, 0, 0);
    LoadAndQueueUpload(1, UPLOAD_KIND_BG_EXT_PLTT, 0, "result_b_bg_01_ncl.bin", 0, 0, 0);
    ChangeDir("/Game/2d/union_b/");
    LoadAndQueueUpload(1, UPLOAD_KIND_BG1_CHAR, 0, "union_b_bg_window_ncg.bin", 0, 0, 0);
    LoadAndQueueUpload(1, UPLOAD_KIND_BG1_SCR, 0, "union_b_bg_window_b_nsc.bin", 0, 0, 0);
    LoadAndQueueUpload(1, UPLOAD_KIND_BG_EXT_PLTT, 0x2000, "union_b_bg_window_ncl.bin", 0, 0, 0);
    SpriteScreen &screen = SpriteManager::Get().Screen(1);
    screen.SetObjVramMode(3);
    screen.LoadCharacters("union_b_obj_window.nce_ncg1D.bin", 0xf000, 0);
    screen.LoadPalette("union_b_obj_window_ncl.bin", 0x160, 0);
    screen.LoadCharacters("union_b_obj_cockpit.nce_ncg1D.bin", 0x11000, 0);
    screen.LoadPalette("union_b_obj_cockpit_ncl.bin", 0x1a0, 0);
    ChangeDir("/Game/Battle/Obj/");
    SpriteScreen &screen2 = SpriteManager::Get().Screen(1);
    screen2.SetObjVramMode(3);
    screen2.LoadCharacters("bakugan_sub.nce_ncg1D.bin", 0x9000, 0);
    screen2.LoadExtPalette("bakugan_sub_ncl.bin", 0, 0);
    screen2.LoadCharacters("result_b_obj_01.nce_ncg1D.bin", 0x4000, 0);
    screen2.LoadPalette("result_b_obj_01_ncl.bin", 0, 0);
    mBakuganSubCellAnim           = new CellAnimation("bakugan_sub_nce.bin", 0, 1, 1);
    mBakuganSubCellAnim->mCharBase = screen2.GetCharBase(0x9000);
    mBakuganSubCellAnim->mPaletteBase   = 0;
    mBakuganSubCellAnim->SetMode(CELL_ANIM_MODE_STOP);
    mBakuganSubCellAnim->mFrameIndex = mAttributes[mBakuganIndex];
    mWindow          = new SelectWindow(7, data_020a2e48, 0);
    mWindow->SetCells(7, data_020a2f0c);
    mWindow->Update(1);
    mCommandList = new BattleCommandList(mBakuganIds[mBakuganIndex], mAttributes[mBakuganIndex], -1, 0);
    if (mUnk_34[mBakuganIndex] != NULL) {
        func_02072b14();
    } else {
        SetCommandListBakuganHidden(mCommandList, 1);
    }
}

void UnkStruct_02071d24::func_02072b14() {
    int value = func_020231f4(gSaveData, 0, mBakuganIds[mBakuganIndex], mAttributes[mBakuganIndex]);
    SetCommandListValues(mCommandList, mAttributes[mBakuganIndex], value, mUnk_34[mBakuganIndex]);
}

void UnkStruct_02071d24::func_02072b60() {
    u16 total = 0;
    BattleCellPair *pair = mUnk_34[mBakuganIndex];
    u8 i = 0;
    do {
        total += data_020a2e3c[pair->mUnk_00_0 - i];
        i++;
    } while (i < mUnk_64[mBakuganIndex]);
    int current = GetBakuganEntryIndex(gSaveData, 0, mBakuganIds[mBakuganIndex], mAttributes[mBakuganIndex]);
    u16 base    = func_020231f4(gSaveData, 0, mBakuganIds[mBakuganIndex], mAttributes[mBakuganIndex]);
    if (base + total > 0x3de) {
        total = 0x3de - base;
    }
    base += total;
    mCommandList->StartCountUp(total);
    func_020231c0(gSaveData, current, base);
}

void UnkStruct_02071d24::func_02072c40() {
    WindowItem *items = mWindow->mCursor->mItems;
    if (func_02072d20()) {
        mUnk_53 = 1;
    } else {
        mUnk_53 = 2;
    }
    GXS_SetVisiblePlane(0x13);
    mWindow->mButtons[0].mEnabled = 0;
    mWindow->mButtons[1].mEnabled = 0;
    mWindow->mButtons[2].mEnabled = 0;
    mWindow->mButtons[3].mEnabled = 0;
    mWindow->mButtons[4].mEnabled = 0;
    items[0].mFlags = 0x8000;
    items[1].mFlags = 0x8000;
    items[2].mFlags = 0x8000;
    items[3].mFlags = 0x8000;
    items[4].mFlags = 0x8000;
    mWindow->mButtons[5].mEnabled = 1;
    mWindow->mButtons[6].mEnabled = 1;
    items[6].mFlags = 8;
    items[4].mY     = 0xc0;
    items[6].mY     = 0x98;
    mWindow->Update(1);
    mWindow->Reset(-1);
    mWindow->mCursor->mSelection = 6;
    PrintText();
}

BOOL UnkStruct_02071d24::func_02072d20() {
    u8 index = mBakuganIndex;
    BattleCellPair *pair = mUnk_34[index];
    if (pair == NULL) {
        return FALSE;
    }
    s32 count = pair->mUnk_00_0;
    if (count >= 2) {
        if (count - mUnk_64[index] < 2) {
            return TRUE;
        }
    }
    if (count >= 2) {
        if (mUnk_4c == 2) {
            return TRUE;
        }
    }
    return FALSE;
}

BOOL UnkStruct_02071d24::func_02072d84() {
    BattleCellPair *pair = mUnk_34[mBakuganIndex];
    if ((s32) pair->mUnk_02_0 < 4) {
        return FALSE;
    }
    if ((s32) pair->mUnk_02_12 < 4) {
        return FALSE;
    }
    if ((s32) pair->mUnk_02_3 < 4) {
        return FALSE;
    }
    if ((s32) pair->mUnk_02_6 < 4) {
        return FALSE;
    }
    return (s32) pair->mUnk_02_9 >= 4;
}

void UnkStruct_02071d24::func_02072dfc() {
    if (mUnk_34[mBakuganIndex] == NULL) {
        return;
    }
    if (mBakuganSubCellAnim != NULL) {
        mBakuganSubCellAnim->Update();
        mBakuganSubCellAnim->DrawTransformed(1, 0x20, 0x3c, 1, 0x800, 0x800, 0);
    }
    if (mWindow != NULL) {
        mWindow->Draw();
    }
    DrawText();
}

void UnkStruct_02071d24::func_02072e7c() {
    if (mUnk_34[mBakuganIndex] == NULL) {
        return;
    }
    if (mBakuganSubCellAnim != NULL) {
        mBakuganSubCellAnim->Update();
        mBakuganSubCellAnim->DrawTransformed(1, 0x20, 0x3c, 1, 0x800, 0x800, 0);
    }
    if (mWindow != NULL) {
        BOOL enabled = TRUE;
        if ((func_02072d84() && (InputManager::Get().IsDown(KEY_UP) | InputManager::Get().IsDown(KEY_DOWN))) || mUnk_51 != 0) {
            enabled = FALSE;
        }
        if (mUnk_52 != 0) {
            enabled = FALSE;
        }
        if (mState != 6 && mState != 4) {
            enabled = FALSE;
        }
        if (enabled) {
            s8 previous = mWindow->mCursor->mSelection;
            u8 result   = mWindow->Update(1);
            s8 current  = mWindow->mCursor->mSelection;
            if (current < 5 && previous != current) {
                mMessageWindow->StartMessage((s16) (current + 0x10), 0, 1);
            }
            if (mUnk_50 != 3) {
                mUnk_50 = result;
            }
        }
        mWindow->Draw();
    }
    DrawText();
}

void UnkStruct_02071d24::RefreshItems() {
    if (mUnk_34[mBakuganIndex] == NULL) {
        return;
    }
    u8 i;
    WindowItem *items = mWindow->mCursor->mItems;
    for (i = 0; i < 5; i++) {
        switch (i) {
            case 0:
                if ((s32) mUnk_34[mBakuganIndex]->mUnk_02_0 < 4) {
                    items[i].mFlags = 8;
                    mWindow->mCellAnims[i]->SetAnimation(data_020a2f0c[i].mAnimIndices[0]);
                } else {
                    items[i].mFlags = 0x8000;
                    mWindow->mCellAnims[i]->SetAnimation(0xf);
                }
                break;
            case 1:
                if ((s32) mUnk_34[mBakuganIndex]->mUnk_02_12 < 4) {
                    items[i].mFlags = 8;
                    mWindow->mCellAnims[i]->SetAnimation(data_020a2f0c[i].mAnimIndices[0]);
                } else {
                    items[i].mFlags = 0x8000;
                    mWindow->mCellAnims[i]->SetAnimation(0x10);
                }
                break;
            case 2:
                if ((s32) mUnk_34[mBakuganIndex]->mUnk_02_3 < 4) {
                    items[i].mFlags = 8;
                    mWindow->mCellAnims[i]->SetAnimation(data_020a2f0c[i].mAnimIndices[0]);
                } else {
                    items[i].mFlags = 0x8000;
                    mWindow->mCellAnims[i]->SetAnimation(0x11);
                }
                break;
            case 3:
                if ((s32) mUnk_34[mBakuganIndex]->mUnk_02_6 < 4) {
                    items[i].mFlags = 8;
                    mWindow->mCellAnims[i]->SetAnimation(data_020a2f0c[i].mAnimIndices[0]);
                } else {
                    items[i].mFlags = 0x8000;
                    mWindow->mCellAnims[i]->SetAnimation(0x12);
                }
                break;
            case 4:
                if ((s32) mUnk_34[mBakuganIndex]->mUnk_02_9 < 4) {
                    items[i].mFlags = 8;
                    mWindow->mCellAnims[i]->SetAnimation(data_020a2f0c[i].mAnimIndices[0]);
                } else {
                    items[i].mFlags = 0x8000;
                    mWindow->mCellAnims[i]->SetAnimation(0x13);
                }
                break;
        }
    }
    u8 j;
    WindowItem *selectItems = mWindow->mCursor->mItems;
    for (j = 0; j < 5; j++) {
        u8 index = j + mUnk_4f;
        index %= 5;
        if (selectItems[index].mFlags == 8) {
            mMessageWindow->StartMessage((s16) (index + 0x10), 0, 1);
            mWindow->mCursor->mSelection = index;
            mWindow->mSavedSelection            = index;
            mWindow->mCellAnims[index]->SetAnimation(data_020a2f0c[index].mAnimIndices[2]);
            return;
        }
    }
}

void UnkStruct_02071d24::func_020733d4() {
    u8 index = mBakuganIndex;
    if (mUnk_64[index] == 0) {
        u8 next;
        next = mBakuganIndex = index + 1;
        if (next >= 6) {
            mState++;
            return;
        }
        if (mUnk_64[next] != 0) {
            if (mCommandList != NULL) {
                SetCommandListBakugan(mCommandList, mBakuganIds[next], mAttributes[next], -1);
            }
            mUnk_4f = 0;
            mBakuganSubCellAnim->mFrameIndex = mAttributes[mBakuganIndex];
            func_02072b14();
            RefreshItems();
            func_02072c40();
            mState = 3;
        } else {
            mState++;
            mBakuganIndex--;
        }
        return;
    }
    if (mWindow == NULL || mUnk_34[index] == NULL) {
        return;
    }
    if (func_02072d84()) {
        if (InputManager::Get().IsPressed(1) | TouchManager::Get().mPressed) {
            mUnk_50 = 3;
        }
    }
    mUnk_4f = mWindow->GetSelectedId();
    if (mUnk_50 != 3) {
        return;
    }
    mUnk_50 = 0;
    switch (mUnk_4f) {
        case 0:
            if ((s32) mUnk_34[mBakuganIndex]->mUnk_02_0 < 4) {
                mUnk_34[mBakuganIndex]->mUnk_02_0++;
            }
            break;
        case 1:
            if ((s32) mUnk_34[mBakuganIndex]->mUnk_02_12 < 4) {
                mUnk_34[mBakuganIndex]->mUnk_02_12++;
            }
            break;
        case 2:
            if ((s32) mUnk_34[mBakuganIndex]->mUnk_02_3 < 4) {
                mUnk_34[mBakuganIndex]->mUnk_02_3++;
            }
            break;
        case 3:
            if ((s32) mUnk_34[mBakuganIndex]->mUnk_02_6 < 4) {
                mUnk_34[mBakuganIndex]->mUnk_02_6++;
            }
            break;
        case 4:
            if ((s32) mUnk_34[mBakuganIndex]->mUnk_02_9 < 4) {
                mUnk_34[mBakuganIndex]->mUnk_02_9++;
            }
            break;
    }
    func_02072b14();
    if (mUnk_64[mBakuganIndex] == 0) {
        return;
    }
    mUnk_64[mBakuganIndex]--;
    mUnk_51 = 0x78;
    if (mUnk_64[mBakuganIndex] == 0) {
        return;
    }
    RefreshItems();
}

void UnkStruct_02071d24::LoadMessages() {
    SpriteManager::Get().Screen(1).SetObjVramMode(3);
    ChangeDir("/font/");
    mBakuganNameMessages = (MessageFile *) LoadFile("mes_BakuganName.mes", 0, 0, 0, 0, 0);
    mResultMessages = (MessageFile *) LoadFile("mes_Result.mes", 0, 0, 0, 0, 0);
    mTextBufferSize = TextManager::Get().BufferSize(0x18, 0xc);
    mTextBuffer = OS_Alloc(mTextBufferSize);
    MI_CpuClear16(mTextBuffer, mTextBufferSize);
    mMessageWindow = new MessageWindow("mes_Result", 1, 0x2800);
    mMessageWindow->func_02065bd4(0x14, 4, 0);
    mMessageWindow->SetWidth(0xd8);
    mMessageWindow->StartMessage(0x10, 0, 1);
    mGlyphCount = TextManager::Get().GlyphCount(0x24);
    mGlyphBuffer = OS_Alloc((s16) mGlyphCount * (s16) 0xc);
    TextManager::Get().Init(mGlyphBuffer, 1, 0, 0x18, 0xc, 0x21);
    TextManager::Get().SetColor(1, 0, 0xe);
}

static inline void PrintResultText(TextManager &text, MessageFile *file, u32 index, void *buffer, s16 w, s16 h,
                                   u16 *pixelWidth) {
    text.Print(file->GetText(index), file->GetLength(index), buffer, 0x18, 0xc, &w, &h, pixelWidth, 0, 2, 0x21, 0, 0, 0, 0, 1,
               1);
}

void UnkStruct_02071d24::PrintText() {
    s16 curW;
    s16 curH;
    u16 pixelWidth;
    s16 width1;
    s16 height1;
    s16 width2;
    s16 height2;
    ClearText();
    if (mUnk_34[mBakuganIndex] == NULL) {
        return;
    }
    if (mUnk_53 == 1) {
        TextManager &text = TextManager::Get();
        height1 = 0;
        width1  = 0;
        MessageFile *file = mResultMessages;
        text.Print(file->GetText(1), file->GetLength(1), mTextBuffer, 0x18, 0xc, &width1, &height1, NULL, 0, 2, 0x21, 0x10, 0,
                   0, 1, 1, 1);
    } else if (mUnk_53 == 2) {
        TextManager &text = TextManager::Get();
        height2 = 0;
        width2  = 0;
        MessageFile *file = mResultMessages;
        text.Print(file->GetText(0), file->GetLength(0), mTextBuffer, 0x18, 0xc, &width2, &height2, NULL, 0, 2, 0x21, 0x10, 0,
                   0, 1, 1, 1);
    } else {
        TextManager::Get().mUse4bppFont = 1;
        curW = 0;
        curH = 0;
        PrintResultText(TextManager::Get(), mBakuganNameMessages, mBakuganIds[mBakuganIndex], mTextBuffer, curW, curH, NULL);
        curH += 0x48;
        PrintResultText(TextManager::Get(), mResultMessages, 5, mTextBuffer, curW, curH, &pixelWidth);
        curW += pixelWidth;
        u8 count = mUnk_34[mBakuganIndex]->mUnk_00_0;
        if (count < 9) {
            curW += 7;
            TextManager::Get().PrintNumber(count + 1, 1, 0, mTextBuffer, 0x18, 0xc, &curW, &curH, 0, 2, 0x21, 0, 0, 1, 1);
        } else {
            PrintResultText(TextManager::Get(), mResultMessages, 6, mTextBuffer, curW, curH, NULL);
        }
        TextManager::Get().mUse4bppFont = 0;
    }
    DC_FlushRange(mTextBuffer, mTextBufferSize);
    QueueUpload(SpriteManager::Get().Screen(1).mBgIndex, UPLOAD_KIND_OBJ, 0, mTextBuffer, mTextBufferSize, 0);
}

// Outside states 2-4 also steps the MessageWindow's text (Update).
void UnkStruct_02071d24::DrawText() {
    BOOL show = FALSE;
    switch (mState) {
        case 2:
        case 3:
            show = TRUE;
            break;
        case 4:
            show = TRUE;
            break;
    }
    if (show) {
        SpriteManager::Get().Screen(1).AddOam(mGlyphBuffer, (u8) mGlyphCount, 0x20, 0x28, 0, 0, 0xe, 0);
    } else {
        SpriteManager::Get().Screen(1).AddOam(mGlyphBuffer, (u8) mGlyphCount, 0x14, 0x1c, 0, 0, 0xe, 0);
    }
    if (mMessageWindow == NULL) {
        return;
    }
    if (show) {
        return;
    }
    mMessageWindow->Update();
}

void UnkStruct_02071d24::ClearText() {
    if (mTextBuffer != NULL) {
        MI_CpuClear16(mTextBuffer, mTextBufferSize);
    }
}

void UnkStruct_02071d24::SetBakugan(u32 index, u32 unk2, u32 unk3, u8 unk4) {
    if (index >= 6 || unk2 >= 0x29 || unk3 >= 6) {
        return;
    }
    mBakuganIndex = 0;
    mBakuganIds[index] = unk2;
    mAttributes[index] = unk3;
    mUnk_34[index] = (BattleCellPair *) GetBakuganEntry(gSaveData, unk2, unk3);
    if (unk4 > 9) {
        unk4 = 9;
    }
    mUnk_64[index] = unk4;
}
