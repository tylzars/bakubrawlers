// Include and definition order here is load-bearing (docs/decompiling.md, "Data layout within a section"): the seven
// singleton headers come first, in this order, then the three tables. That reproduces the original's .bss order of
// the six destructor nodes (FrameTask, TouchManager, TextManager, SoundManager, InputManager, WirelessManager);
// found with tools/layout_search.py.
#include "System/FrameTask.hpp"
#include "Text/TextManager.hpp"
#include "System/SoundManager.hpp"
#include "System/InputManager.hpp"
#include "Graphics/SpriteManager.hpp"
#include "System/TouchManager.hpp"
#include "System/WirelessManager.hpp"
#include "Battle/BattleConfrontCard.hpp"
#include "Battle/BattleCardSpec.hpp"
#include "Battle/BattleCommandPrompt.hpp"
#include "Battle/BattleCommandList.hpp"
#include "Battle/BattleCommandPage.hpp"
#include "Battle/BattleUnionAbility.hpp"
#include "Game/UnkStruct_020d433c.hpp"
#include "Graphics/CellAnimation.hpp"
#include "Graphics/Display.hpp"
#include "System/FileSystem.hpp"
#include "Unknown/UnkFunctions.hpp"
#include "UI/SelectWindow.hpp"
#include "Graphics/AntennaIndicator.hpp"
#include "Battle/BattleCharaAct.hpp"
#include "Battle/ConfrontBg01.hpp"
#include "Battle/ConfrontBg02.hpp"
#include "Battle/Confront03.hpp"
#include "Battle/ConfrontBg04.hpp"
#include "Battle/ConfrontBg05.hpp"
#include "Game/UnkStruct_020d48f0.hpp"
#include "UI/StorytalkWindow.hpp"
#include "Graphics/Camera.hpp"
#include "Game/Random.hpp"
#include <nitro/math.h>
#include <nitro/gx.h>
#include <nitro/mi.h>
#include <nitro/os.h>
#include <stdio.h>

void SetSlot6cFlag(BattlePlayer *player, u8 a, int b);
extern "C" u32 func_ov007_022234f0(int player, u8 mode, u8 ch);
static const WindowItemRecord sWindowItems[3] = {
    {0, 0x1, 45, 152, 78, 20, 0, {0, 0, 1, 1}, 0, 2},
    {1, 0x1, 133, 152, 78, 20, 0, {1, 1, 0, 0}, 0, 2},
    {2, 0x8000, 120, 18, 78, 20, 0, {-1, -1, -1, -1}, 0, 0},
};
static const WindowCellRecord sWindowCells[3] = {
    {"/Game/2D/union_b/union_b_obj_window_nce.bin", 3, 0xc000, 7, 0, 1, {3, 5, 4, 0}},
    {"/Game/2D/union_b/union_b_obj_window_nce.bin", 7, 0xc000, 7, 0, 1, {6, 8, 7, 0}},
    {"/Game/2D/union_b/union_b_obj_window_nce.bin", 10, 0xc000, 7, 0, 1, {10, 10, 10, 0}},
};
// Power multipliers (fx32) by battle mode (row, mode - 1) and attribute (column).
static const s32 sAttributeRates[6][6] = {
    {0x1333, 0xccd, 0x1000, 0x1000, 0x1000, 0x1000},
    {0xccd, 0x1333, 0x1000, 0x1000, 0x1000, 0x1000},
    {0x1000, 0x1000, 0xccd, 0x1000, 0x1000, 0x1333},
    {0x1000, 0x1000, 0x1333, 0x1000, 0x1000, 0xccd},
    {0x1000, 0x1000, 0x1000, 0x1333, 0xccd, 0x1000},
    {0x1000, 0x1000, 0x1000, 0xccd, 0x1333, 0x1000},
};

void SetSlot6cFlag(BattlePlayer *player, u8 index, int flag);

extern const s8 data_ov007_02279d3c[];
u8 GetCardAttributeValue(u32 id, u32 index);

extern "C" BOOL func_ov007_02263150(BattlePlayerList *list);
extern "C" void func_ov007_022633f8(BattlePlayerList *list);
BOOL SetMessageTypeAndCheckAll(BattlePlayerList *list, u32 id);
extern "C" void RecordSndHeapState(u32 heap);
extern "C" void GX_SetGraphicsMode(u32 unk1, u32 unk2, u32 unk3);
extern "C" void GXS_SetGraphicsMode(u32 unk1);

class ConfrontBg06 : public BattleBase {
public:
    PAD(0x68, 0x70);
    ConfrontBg06();
    virtual ~ConfrontBg06();
    virtual void LoadGraphics();
    virtual void vfunc_34();
    virtual void vfunc_3c();
    virtual u8 UpdateTouch(int a);
    virtual void vfunc_44(int index, u16 count);
};
u32 GetTexVariant(u32 a, u32 b);
void RemoveMarker(BattlePlayerList *list, u8 index);
u8 PickUnflaggedSlot6c(BattlePlayer *player, u8 mask, s16 x, s16 y);


BattleConfrontCard::BattleConfrontCard(u32 unk)
    : SceneBase(0x3e8, 0) {
    mState    = 0;
    mStep   = 0;
    mUnk_0e   = 0;
    mUnk_0f   = 0;
    mUnk_10   = 0;
    mUnk_12   = 0;
    mBattleMode   = 0;
    mUnk_21   = -1;
    mUnk_22   = 0;
    mUnk_23   = 0;
    mData     = 0;
    mCardCamera   = 0;
    mCommandPage   = 0;
    mCardSpec   = 0;
    mConfront   = 0;
    mUnionAbility   = 0;
    mUnk_50   = 0;
    mUnk_51   = 0;
    mUnk_52   = 0;
    mSlot6cIndex   = 0;
    mCardId   = 0;
    mUnk_56   = 0;
    mUnk_5e   = 0;
    mUnk_5f   = 0;
    mDisabledBits   = 0;
    mUnk_61   = 0;
    mUnk_66   = 0;
    mUnk_68   = 0;
    mWindowOpen   = 0;
    mTextBufferSize   = 0;
    mGlyphCount   = 0;
    mTextBuffer   = 0;
    mGlyphBuffer   = 0;
    mWindow   = 0;
    mMessages   = 0;
    mUnk_74   = 0;
    u8 i = 0;
    do {
        mDrawX[i] = -0x60;
        mDrawY[i] = 0x3c;
        i++;
    } while (i < 3);
    MI_CpuClearFast(mCellAnims, 0x14);
    mData = new BattleData();
    if (unk != -1) {
        mBattleMode = unk;
    } else {
        mBattleMode = GetMarkerBattleMode();
    }
    mUnk_62 = 0;
    mUnk_63 = 0;
    mUnk_64 = 0;
    mUnk_65 = 0;
    SpriteManager::Get();
    SpriteManager::sFade[0]       = FADE_MODE_HOLD;
    SpriteManager::sBrightness[0] = -16;
    SpriteManager::Get();
    SpriteManager::sFade[1]       = FADE_MODE_HOLD;
    SpriteManager::sBrightness[1] = -16;
}

BattleConfrontCard::~BattleConfrontCard() {
    u8 i = 0;
    do {
        CellAnimation *a = mCellAnims[i];
        if (a) {
            delete a;
        }
        i++;
    } while (i < 5);
    if (mData) {
        delete mData;
    }
    ReleaseWindow();
    SoundManager::Get().StopSeq(1);
    SoundManager::Get().RestoreHeapState();
    if (!func_ov007_02263150(data_020d433c.mBattlePlayers)) {
        if (data_020d433c.mTutorialPart) {
            SoundManager::Get().LoadAndStartSeq(1);
            RecordSndHeapState(SoundManager::Get().mHeap);
        } else {
            func_ov007_022633f8(data_020d433c.mBattlePlayers);
        }
    }
    SoundManager::Get().LoadSoundGroup(6);
    SpriteManager::Get();
    SpriteManager::sFade[0]       = FADE_MODE_HOLD;
    SpriteManager::sBrightness[0] = -16;
    SpriteManager::Get();
    SpriteManager::sFade[1]       = FADE_MODE_HOLD;
    SpriteManager::sBrightness[1] = -16;
    SpriteManager::Get().ResetSlots();
}

void BattleConfrontCard::Update() {
    switch (mState) {
        case 0:
            FrameTask::Get().StopAllChannels();
            mState++;
            break;
        case 1:
            if (func_0203dd0c(&FrameTask::Get(), 0) == 0) {
                SoundManager::Get().RestoreHeapState();
                SoundManager::Get().LoadAndStartSeq(0);
                SoundManager::Get().LoadSoundGroup(2);
                mState++;
            }
            break;
        case 2:
            Init();
            SpriteManager::Get().StartFade(FADE_MODE_TO_ZERO);
            mState++;
            break;
        case 3:
            if (!SpriteManager::Get().IsFading()) {
                mState++;
            }
            break;
        case 4:
            if (func_ov007_0223f918()) {
                mState++;
            }
            break;
        case 5:
            SpriteManager::Get().StartFade(FADE_MODE_DOWN);
            FrameTask::Get().StopChannel(1);
            mState++;
            break;
        case 6:
            if (SpriteManager::Get().IsFading()) {
                break;
            }
            if (func_0203dd0c(&FrameTask::Get(), 1) == 0) {
                ResetDisplay();
                mState++;
            }
            break;
        case 7:
            if (SetMessageTypeAndCheckAll(data_020d433c.mBattlePlayers, 0xf)) {
                mState++;
            }
            break;
        case 8:
            RequestScene(1);
            mFinished = 1;
            return;
    }
    if ((mUnk_22 == 0) & (mState > 2)) {
        func_ov007_02243100();
        if (mWindowOpen) {
            if (mWindow) {
                mWindow->Draw();
            } else {
                mCellAnims[1]->Update();
                mCellAnims[1]->Draw(1, 0x80, 0x60, 0);
            }
            SpriteManager::Get().Screen(1).AddOam(mGlyphBuffer, 9, 0x20, 0x28, 0, 0x1a0, 0xf, 0);
        }
    }
}

void BattleConfrontCard::Init() {
    BattleData *data = mData;
    InitDisplay();
    mData->Load();
    LoadGraphics();
    u16 a   = data->mEntries[0].mBakuganId | (data->mEntries[0].mAttribute << 8);
    u16 b   = data->mEntries[1].mBakuganId | (data->mEntries[1].mAttribute << 8);
    mCardCamera = new BattleCardCamera(a, b);
}

void BattleConfrontCard::InitDisplay() {
    SpriteManager::Get().SetupMode(1, 7);
    reg_GX_POWCNT |= 0x8000;
    GX_SetGraphicsMode(1, 0, 1);
    GXS_SetGraphicsMode(3);
    ResetDisplay();
}

void BattleConfrontCard::ResetDisplay() {
    G2S_SetBG0Control(0, 1, 0, 1, 0);
    G2S_SetBG1Control(0, 1, 2, 4, 0);
    G2S_SetBG3Control(1, 0, 4, 5, 0);
    G2_SetBG0Offset(0, 0);
    G2_SetBG1Offset(0, 0);
    G2_SetBG2Offset(0, 0);
    G2_SetBG3Offset(0, 0);
    G2S_SetBG0Offset(0, 0);
    G2S_SetBG1Offset(0, 0);
    G2S_SetBG2Offset(0, 0);
    G2S_SetBG3Offset(0, 0);
    G2_SetBG0Priority(2);
    G2_SetBG1Priority(2);
    G2_SetBG2Priority(2);
    G2_SetBG3Priority(2);
    G2S_SetBG0Priority(3);
    G2S_SetBG1Priority(0);
    G2S_SetBG2Priority(3);
    G2S_SetBG3Priority(3);
    GX_SetVisiblePlane(0x1f);
    GXS_SetVisiblePlane(0x1f);
    G2_SetBlendBrightness(0x1f, 0);
    G2S_SetBlendBrightness(0x1f, 0);
    GX_SetVisibleWnd(0);
    G2_SetWndOutsidePlane(0x1f, FALSE);
    G2_SetWnd0InsidePlane(0x1f, FALSE);
    G2_SetWnd1InsidePlane(0x1f, FALSE);
    G2_SetWndOBJInsidePlane(0x1f, FALSE);
    G2_SetWnd0Position(0, 0, 0, 0);
    G2_SetWnd1Position(0, 0, 0, 0);
    if (data_020d433c.mAntenna) {
        data_020d433c.mAntenna->mSecondVisible = 0;
    }
    GXS_SetVisibleWnd(0);
    G2S_SetWndOutsidePlane(0x1f, FALSE);
    G2S_SetWnd0InsidePlane(0x1f, FALSE);
    G2S_SetWnd1InsidePlane(0x1f, FALSE);
    G2S_SetWndOBJInsidePlane(0x1f, FALSE);
    G2S_SetWnd0Position(0, 0, 0, 0);
    G2S_SetWnd1Position(0, 0, 0, 0);
    G2_BlendNone();
    G2S_BlendNone();
}

void BattleConfrontCard::LoadGraphics() {
    char buffer[0x80];
    ChangeDir("/Game/Confrontation/Bg");
    LoadAndQueueUpload(1, UPLOAD_KIND_BG_EXT_PLTT, 0, "vs_b_bg_00_ncl.bin", 0, 0, 0);
    LoadAndQueueUpload(1, UPLOAD_KIND_BG0_CHAR, 0, "vs_b_bg_00_ncg.bin", 0, 0, 0);
    LoadAndQueueUpload(1, UPLOAD_KIND_BG0_SCR, 0, "vs_b_bg_00_nsc.bin", 0, 0, 0);
    ChangeDir("/");
    ChangeDir("/Game/Confrontation/Obj");
    SpriteScreen &screen = SpriteManager::Get().Screen(1);
    sprintf(buffer, "vs_b_obj_00.nce_ncg1D.bin");
    screen.LoadCharacters(buffer, 0x13000, 0);
    sprintf(buffer, "vs_b_obj_00_ncl.bin");
    screen.LoadPalette(buffer, 0xa0, 0);
    sprintf(buffer, "vs_b_obj_boost_ncl.bin");
    screen.LoadPalette(buffer, 0x160, 0);
    if (mCellAnims[4]) {
        delete mCellAnims[4];
    }
    mCellAnims[4]            = 0;
    mCellAnims[4]            = new CellAnimation("vs_b_obj_00_nce.bin", 0, 1, 1);
    mCellAnims[4]->mCharBase = screen.GetCharBase(0x13000);
    mCellAnims[4]->mPaletteBase   = 5;
    mCellAnims[4]->SetAnimation(0);
    LoadAndQueueUpload(1, UPLOAD_KIND_OBJ_PLTT, 0x120, "cardspec_b_obj_00_ncl.bin", 0, 0, 0);
    LoadAndQueueUpload(1, UPLOAD_KIND_OBJ, 0x18000, "cardspec_b_obj_00.nce_ncg1D.bin", 0, 0, 0);
    u8 i = 2;
    do {
        if (mCellAnims[i]) {
            delete mCellAnims[i];
        }
        mCellAnims[i]            = 0;
        mCellAnims[i]            = new CellAnimation("cardspec_b_obj_00_nce.bin", 0, 1, 1);
        mCellAnims[i]->mCharBase = screen.GetCharBase(0x18000);
        mCellAnims[i]->mPaletteBase   = 9;
        i++;
    } while (i < 4);
    mCellAnims[2]->SetAnimation(0);
    mCellAnims[3]->SetAnimation(1);
    ChangeDir("/Game/2D/union_b");
    LoadAndQueueUpload(1, UPLOAD_KIND_OBJ, 0xe000, "union_b_obj_page.nce_ncg1D.bin", 0, 0, 0);
    LoadAndQueueUpload(1, UPLOAD_KIND_OBJ_PLTT, 0xe0, "union_b_obj_window_ncl.bin", 0, 0, 0);
    if (mCellAnims[0] == 0) {
        LoadCardGraphic(0);
    } else {
        LoadCardGraphic(mData->mMarkerCardId);
    }
    if (data_020d433c.mAntenna) {
        data_020d433c.mAntenna->SetupLayout(10);
    }
}

void BattleConfrontCard::LoadCardGraphic(u16 card) {
    char buffer[0x80];
    u16 id = GetCardNumber(card);
    SpriteScreen &screen = SpriteManager::Get().Screen(1);
    ChangeDir("/Game/Card/Graphic/Large/");
    sprintf(buffer, "card_l_%03d.nce_ncg1D.bin", id);
    screen.LoadCharacters(buffer, 0x10000, 0);
    sprintf(buffer, "card_l_%03d_ncl.bin", id);
    screen.LoadExtPalette(buffer, 0x400, 0);
    sprintf(buffer, "card_l_%03d_nce.bin", id);
    CellAnimation *old = mCellAnims[0];
    if (old) {
        delete old;
    }
    mCellAnims[0]            = 0;
    mCellAnims[0]            = new CellAnimation(buffer, 0, 1, 1);
    mCellAnims[0]->mCharBase = screen.GetCharBase(0x10000);
    mCellAnims[0]->mPaletteBase   = 2;
    ChangeDir("/");
}

void BattleConfrontCard::func_ov007_0223f90c(u16 value) {
    mUnk_12 = value;
    mUnk_10 = value;
}

// Placeholder (case 29): `idx = count ? list->mUnk_2a9 : data_020d433c.mBattlePlayers->mUnk_2a9` reads the same byte
// on both arms and compiles to nothing extra. As a two-arm value the player index is not forward-substituted into
// `slots`, and the arms are merged only after the last expression propagation, so idx stays its own statement; with
// `off = j << 2` that numbers list < idx < j*4 < slots, giving the original's registers (`ldr r1,[r5]; lsl r2,r8,#2;
// ...; ldrh r2,[r1,r2]`). The original's own form is not known.
BOOL BattleConfrontCard::func_ov007_0223f918() {
    u8 j;
    u8 k;
    BattleEntry *e = mData->mEntries;
    switch (mStep) {
        case 0:
            if (data_020d433c.mAntenna) {
                data_020d433c.mAntenna->mSecondVisible = 1;
            }
            GXS_SetVisibleWnd(5);
            G2S_SetWndOutsidePlane(0x1f, FALSE);
            G2S_SetWnd0InsidePlane(0x3f, FALSE);
            G2S_SetWnd0Position(0, 0, 0xff, 0xbf);
            func_ov007_0223f90c(0x100);
            if (data_020d433c.mBattlePlayers->mPlayers[e[0].mPlayer]->IsCpu() == 1
                && data_020d433c.mBattlePlayers->mPlayers[e[1].mPlayer]->IsCpu() == 1) {
                mUnk_68 = 1;
                WirelessManager::Get();
                if (IsWirelessMode(&data_020c4030.mUnk_228)) {
                    if (data_020d433c.mBattlePlayers->mUnk_2a9 == 0) {
                        mUnk_23 = 1;
                    }
                }
            }
            if (mUnk_68) {
                WirelessManager::Get();
                if (IsWirelessMode(&data_020c4030.mUnk_228)) {
                    if (mUnk_23) {
                        LoadWindow("/font/mes_confrontation.mes", 0);
                    } else {
                        LoadWindow("/font/mes_BattleArenaMenu.mes", 4);
                    }
                } else {
                    LoadWindow("/font/mes_confrontation.mes", 0);
                }
                OpenWindow();
                if (mWindow) {
                    mWindow->mCursor->mSelection = -1;
                    mWindow->mState            = 4;
                    mWindow->Reset(1);
                }
                mStep = 2;
            } else {
                mStep = 1;
            }
            break;
        case 1:
            mUnk_10--;
            if (mUnk_10 == 0) {
                SetSubBlendAlpha(0);
                reg_G2S_DB_WIN0H = 0x4faf;
                reg_G2S_DB_WIN0V = 0x20a0;
                mDrawX[2] = 0x4f;
                mDrawY[2] = 0x20;
                func_ov007_0223f90c(0x20);
                mStep = 3;
            }
            break;
        case 2: {
            if (mUnk_10 > 1) {
                mUnk_10--;
            }
            u8 active = mUnk_68;
            WirelessManager::Get();
            const u8 *mode = &data_020c4258;
            if (IsWirelessMode(mode)) {
                if (mUnk_23 == 0) {
                    active = 0;
                }
            }
            if (active) {
                u8 result = mWindow->Update(1);
                SelectWindow *w = mWindow;
                if (w->mState == 2 && w->mResult == 1 && w->mCursor->mSelection == 0) {
                    WirelessManager::Get();
                    if (IsWirelessMode(mode)) {
                        data_0220f540.mUnk_00     = 0x16;
                        data_0220f540.mTouchDown = 1;
                    } else {
                        CloseWindow();
                        ReleaseWindow();
                        SpriteManager::Get().StartFade(FADE_MODE_DOWN);
                        mStep = 0x31;
                    }
                    mUnk_68 = 0;
                } else if (result == 1) {
                    if (w->mCursor->mSelection != 0) {
                        WirelessManager::Get();
                        if (IsWirelessMode(mode)) {
                            data_0220f540.mUnk_00     = 0x16;
                            data_0220f540.mTouchDown = 0;
                        } else {
                            CloseWindow();
                            ReleaseWindow();
                            mStep = 1;
                        }
                    }
                    mUnk_68 = 0;
                }
            }
            WirelessManager::Get();
            if (IsWirelessMode(mode)) {
                BattlePlayer *p = data_020d433c.mBattlePlayers->mPlayers[0];
                if (p->mMsg.mType == 0x16) {
                    CloseWindow();
                    ReleaseWindow();
                    mUnk_68 = 0;
                    if (p->mMsg.mTouchDown) {
                        SpriteManager::Get().StartFade(FADE_MODE_DOWN);
                        mStep = 0x31;
                    } else {
                        mStep = 1;
                    }
                }
            }
            break;
        }
        case 3:
            mUnk_10--;
            if (mUnk_10 != 0) {
                SetSubBlendAlpha((u8) ((mUnk_10 << 4) / mUnk_12));
            } else {
                BattleCardCamera *cam = mCardCamera;
                SpriteManager::Get();
                SpriteManager::sFade[0] = FADE_MODE_DOWN;
                data_020d433c.mFlags |= 4;
                cam->mState = 3;
                mStep++;
            }
            break;
        case 4:
            if (mContext->mRequest != 0) {
                mStep++;
            }
            break;
        case 5:
            new BattleCardSpec(0, e[0].mBakuganId, e[0].mAttribute, e[0].mCharacterId == 0x15 ? 1 : 0);
            mContext->mRequest = 0;
            func_ov007_0223f90c(0x40);
            mStep++;
            break;
        case 6:
            mDrawX[0] = (s16) ((-0x60 * (mUnk_10 * mUnk_10)) / (mUnk_12 * mUnk_12)) + 8;
            if (mUnk_10 != 0) {
                mUnk_10--;
                break;
            }
            if (mContext->mRequest != 0) {
                mStep++;
            }
            break;
        case 7:
            new BattleCardSpec(0, e[1].mBakuganId, e[1].mAttribute, e[1].mCharacterId == 0x15 ? 1 : 0);
            mContext->mRequest = 0;
            func_ov007_0223f90c(0x40);
            mStep++;
            break;
        case 8:
            mDrawX[1] = 0xb7 - (s16) ((-0x60 * (mUnk_10 * mUnk_10)) / (mUnk_12 * mUnk_12));
            if (mUnk_10 != 0) {
                mUnk_10--;
                break;
            }
            if (mContext->mRequest != 0) {
                mStep++;
            }
            break;
        case 9:
            new BattleCharaAct(data_020d433c.mBattlePlayers->mCamera, e[0].mPlayer, 5, 0, 0);
            mContext->mRequest = 0;
            func_ov007_0223f90c(0x40);
            mStep++;
            break;
        case 10:
            mUnk_10--;
            if (mUnk_10 == 0) {
                func_ov007_0223f90c(0x20);
                LoadCardGraphic(mData->mMarkerCardId);
                SetSubBlendBrightness(0x10);
                mStep++;
                mUnk_5f = 1;
            }
            break;
        case 11:
            SetSubBlendBrightness((s8) ((mUnk_10 << 4) / mUnk_12));
            if (mUnk_10 != 0) {
                mUnk_10--;
                break;
            }
            func_ov007_0223f90c(0x10);
            mStep++;
            break;
        case 12:
            if (mContext->mRequest == 0) {
                break;
            }
            if (data_020d433c.mAntenna) {
                data_020d433c.mAntenna->SetupLayout(10);
            }
            if (SetMessageTypeAndCheckAll(data_020d433c.mBattlePlayers, 0xf)) {
                mStep++;
            }
            break;
        case 13:
            mCommandPage = new BattleCommandPage(mData->mMarkerCardId, -1);
            mStep++;
            mUnk_10 = 0;
            break;
        case 14:
            mUnk_10++;
            if (mUnk_10 > 0xf0) {
                CloseCommandPage(mCommandPage, 1);
                mContext->mRequest = 0;
                mStep++;
                mUnk_10 = 0;
            }
            break;
        case 15: {
            if (mContext->mRequest == 0) {
                break;
            }
            SpriteManager::Get().SetupMode(0, 7);
            GX_SetGraphicsMode(1, 0, 1);
            mUnk_50 = func_ov007_022428d4(mData->mMarkerCardId);
            u8 side = data_020d433c.mBattlePlayers->Unk2a9() == e[1].mPlayer ? 1 : 0;
            u32 kind = 6;
            switch (mUnk_50) {
                case 1:
                    if (e[side].mUnk_08 != 0) {
                        kind = 7;
                    }
                    break;
                case 4:
                case 6:
                    if (e[side].mUnk_08 == 0) {
                        kind = 6;
                    } else {
                        kind = 8;
                    }
                    break;
            }
            mCardSpec = new BattleCardSpec(kind, e[side].mBakuganId, e[side].mAttribute, e[side].mCharacterId == 0x15 ? 1 : 0);
            mContext->mRequest = 0;
            func_ov007_0223f90c(0x40);
            mStep++;
            break;
        }
        case 16:
            DrawEntryNumbers(0, 0, 0);
            mUnk_10--;
            if (mUnk_10 == 0) {
                mStep++;
            }
            break;
        case 17:
            if (mData->func_ov007_0223dd6c()) {
                func_ov007_0223f90c(0x40);
                if (mUnk_50 == 2 || mUnk_50 == 3 || mUnk_50 == 5) {
                    BattleCardSpec *spec = mCardSpec;
                    spec->mUnk_1e = 0;
                    CloseCardModel(spec->mCardModel);
                    mContext->mRequest = 0;
                    mStep           = 0x14;
                } else if (mData->mMarkerCardId == 0x55) {
                    BattleCardSpec *spec = mCardSpec;
                    spec->mUnk_1e = 0;
                    CloseCardModel(spec->mCardModel);
                    mContext->mRequest = 0;
                    mStep           = 0x14;
                } else {
                    mData->mEntries[0].mUnk_02 = e[0].mUnk_00 + e[0].mUnk_08;
                    mData->mEntries[1].mUnk_02 = e[1].mUnk_00 + e[1].mUnk_08;
                    mStep++;
                }
            }
            DrawEntryNumbers(0, 1, 0);
            break;
        case 18:
            DrawEntryNumbers(1, 0, 1);
            mUnk_10--;
            if (mUnk_10 == 0) {
                mStep++;
            }
            break;
        case 19:
            if (mData->func_ov007_0223dd6c()) {
                func_ov007_0223f90c(0x40);
                mStep = 0x19;
            }
            DrawEntryNumbers(1, 1, 1);
            break;
        case 20: {
            if (mContext->mRequest == 0) {
                break;
            }
            u8 side;
            if (data_020d433c.mBattlePlayers->Unk2a9() == e[1].mPlayer) {
                side = 1;
                if (mUnk_5d == 0) {
                    side = 0;
                }
            } else {
                side = 0;
                if (mUnk_5c == 0) {
                    side = 1;
                }
            }
            u32 kind;
            u8 make = 0;
            switch (mUnk_50) {
                case 2:
                    kind = 9;
                    make = 1;
                    break;
                case 3:
                    kind = 10;
                    make = 1;
                    break;
                case 5:
                    kind = 11;
                    make = 1;
                    break;
            }
            if (mData->mMarkerCardId == 0x55) {
                make = 1;
                kind = 10;
            }
            if (make) {
                mCardSpec = new BattleCardSpec(kind, e[side].mBakuganId, e[side].mAttribute, e[side].mCharacterId == 0x15 ? 1 : 0);
            }
            mStep++;
            break;
        }
        case 21:
            if (mUnk_50 == 5) {
                mData->mEntries[0].mUnk_02 = e[1].mUnk_04 + e[0].mUnk_00 - e[0].mUnk_04;
                mData->mEntries[1].mUnk_02 = e[0].mUnk_04 + e[1].mUnk_00 - e[1].mUnk_04;
                u16 t                      = e[1].mUnk_04;
                mData->mEntries[1].mUnk_04 = e[0].mUnk_04;
                mData->mEntries[0].mUnk_04 = t;
            }
            func_ov007_0223f90c(0x64);
            mStep++;
            break;
        case 22: {
            BOOL done = mData->func_ov007_0223dd6c();
            if (mUnk_10 != 0) {
                mUnk_10--;
                break;
            }
            if (!done) {
                break;
            }
            func_ov007_0223f90c(0x40);
            mStep = 0x19;
            if (data_020d433c.mBattlePlayers->Unk2a9() == e[1].mPlayer) {
                if (mUnk_5d == 0) {
                    BattleCardSpec *spec = mCardSpec;
                    spec->mUnk_1e = 0;
                    CloseCardModel(spec->mCardModel);
                    mContext->mRequest = 0;
                    mStep           = 0x17;
                }
            } else {
                if (mUnk_5c == 0) {
                    BattleCardSpec *spec = mCardSpec;
                    spec->mUnk_1e = 0;
                    CloseCardModel(spec->mCardModel);
                    mContext->mRequest = 0;
                    mStep           = 0x17;
                }
            }
            break;
        }
        case 23: {
            if (mContext->mRequest == 0) {
                break;
            }
            u8 side = data_020d433c.mBattlePlayers->Unk2a9() == e[1].mPlayer ? 1 : 0;
            mCardSpec = new BattleCardSpec(3, e[side].mBakuganId, e[side].mAttribute, e[side].mCharacterId == 0x15 ? 1 : 0);
            func_ov007_0223f90c(0x40);
            mStep++;
            break;
        }
        case 24:
            mStep++;
            break;
        case 25:
            if (mUnk_10 != 0) {
                mUnk_10--;
                break;
            }
            mCardId = 0;
            if (data_020d433c.mTutorialNumber == 2 || data_020d433c.mTutorialNumber == 4) {
                mStep = 0x1a;
            } else {
                mStep = 0x1c;
            }
            break;
        case 26:
            if (data_020d433c.mAntenna) {
                data_020d433c.mAntenna->mSecondVisible = 0;
            }
            GXS_SetVisibleWnd(0);
            data_020d433c.mBattlePlayers->mUnk_2a4->LoadGraphics();
            {
                BattlePlayerList *list = data_020d433c.mBattlePlayers;
                list->mUnk_2a4->SetMessage(list->mUnk_2af, 0x64, 0, 0);
            }
            G2S_SetBlendBrightness(0x1f, -10);
            mStep++;
            break;
        case 27:
            if (InputManager::Get().IsPressed(1) || TouchManager::Get().mReleased) {
                data_020d433c.mBattlePlayers->mUnk_2a4->NextPage(1);
            }
            if (data_020d433c.mBattlePlayers->mUnk_2a4->func_02087cac()) {
                break;
            }
            data_020d433c.mBattlePlayers->mUnk_2af++;
            G2S_SetBlendBrightness(0x1f, 0);
            mStep = 0x1c;
            break;
        case 28: {
            mUnk_62 = 0;
            mUnk_63 = 0;
            mUnk_64 = 0;
            mUnk_65 = 0;
            mUnk_5e = 0;
            mUnionAbility = 0;
            if (data_020d433c.mUnk_9a_2) {
                mStep = 0x20;
                break;
            }
            u8 owner = data_020d433c.mBattlePlayers->mUnk_2a9;
            if (owner == e[0].mPlayer) {
                mStep = 0x1d;
                mUnk_52 = 0;
            } else if (owner == e[1].mPlayer) {
                mStep = 0x1d;
                mUnk_52 = 1;
            } else {
                mStep = 0x20;
                mUnionAbility = new BattleUnionAbility(4, 0x1a000, 0xc);
            }
            break;
        }
        case 29: {
            u8 *bits = &mDisabledBits;
            u8 j     = 0;
            u8 bit   = 0;
            u8 count = 0;
            mDisabledBits  = 0;
            do {
                BattlePlayerList *list  = data_020d433c.mBattlePlayers;
                u8 idx                  = count ? list->mUnk_2a9 : data_020d433c.mBattlePlayers->mUnk_2a9; // placeholder, see above
                u32 off                 = j << 2;
                BattlePlayerSlot *slots = list->mPlayers[idx]->mSlots6c;
                u8 flag                 = *((u8 *) slots + off + 2);
                u16 id                  = *(u16 *) ((u32) slots + off);
                if (flag == 0) {
                    if (func_ov007_022433c8(mUnk_52, id)) {
                        count++;
                    } else {
                        bits[bit / 8] |= 1 << (bit % 8);
                    }
                    bit++;
                }
                j++;
            } while (j < data_020d433c.mUnk_99);
            if (count != 0) {
                mUnk_66 = 0;
                mStep = 0x1e;
                if (count == 1) {
                    mUnk_66 = 1;
                }
                mUnk_51 = 0;
            } else {
                mUnionAbility = new BattleUnionAbility(4, 0x1a000, 0xc);
                mStep = 0x1f;
            }
            break;
        }
        case 30:
            if (e[mUnk_52].mUnk_0f_0) {
                mUnionAbility = new BattleUnionAbility(4, 0x1a000, 0xc);
                mCardId = 0;
                mStep = 0x20;
                break;
            }
            switch (func_ov007_0224251c()) {
                case 1:
                    if (!data_020d433c.mUnk_9a_2) {
                        mUnk_5e = 1;
                    }
                    mUnk_51 = 0;
                    mStep = 0x20;
                    mUnionAbility = new BattleUnionAbility(4, 0x1a000, 0xc);
                    break;
                case 2:
                    mCardId = 0;
                    mStep = 0x20;
                    mUnionAbility = new BattleUnionAbility(4, 0x1a000, 0xc);
                    break;
            }
            break;
        case 31:
            mCardId = 0;
            mStep = 0x20;
            break;
        case 32:
            if (SetMessageTypeAndCheckAll(data_020d433c.mBattlePlayers, 0xf)) {
                mStep++;
                if (mUnionAbility) {
                    ((BattleUnionAbility *) mUnionAbility)->Close(0);
                }
            }
            break;
        case 33: {
            WirelessManager::Get();
            if (IsWirelessMode(&data_020c4258)) {
                u8 owner = data_020d433c.mBattlePlayers->mUnk_2a9;
                if (e[0].mPlayer == owner || e[1].mPlayer == owner) {
                    data_0220f540.mUnk_00 = 7;
                    data_0220f540.mUnk_04 = mCardId;
                    *((u8 *) &data_0220f540 + 0xa) = mSlot6cIndex;
                }
            }
            u8 i    = 0;
            u8 done = 0;
            do {
                if ((&mUnk_62)[i] == 0) {
                    BattlePlayer *p = data_020d433c.mBattlePlayers->mPlayers[e[i].mPlayer];
                    if (p->IsCpu()) {
                        (&mUnk_62)[i] = 1;
                    } else if (data_020d433c.mUnk_9a_2) {
                        (&mUnk_62)[i] = 1;
                    } else {
                        WirelessManager::Get();
                        if (IsWirelessMode(&data_020c4258)) {
                            p = data_020d433c.mBattlePlayers->mPlayers[e[i].mPlayer];
                            if (p->mMsg.mType == 7) {
                                mData->mEntries[i].mCardId = *(u32 *) ((u8 *) p + 0x104);
                                mData->mEntries[i].mSlot6cIndex = *((u8 *) p + 0x10a);
                                mData->mEntries[i].mUnk_0a = func_ov007_02243b24(i, (u16) *(u32 *) ((u8 *) p + 0x104));
                                (&mUnk_62)[i]              = 1;
                            }
                        } else {
                            mData->mEntries[i].mCardId = mCardId;
                            mData->mEntries[i].mSlot6cIndex = mSlot6cIndex;
                            mData->mEntries[i].mUnk_0a = func_ov007_02243b24(i, mCardId);
                            (&mUnk_62)[i]              = 1;
                        }
                    }
                } else {
                    done++;
                }
                i++;
            } while (i < 2);
            if (done == 2) {
                mStep = 0x22;
            }
            break;
        }
        case 34:
            if (SetMessageTypeAndCheckAll(data_020d433c.mBattlePlayers, 0xf)) {
                mStep++;
            }
            break;
        case 35: {
            u8 i = 0;
            do {
                BattleEntry *ent = &e[i];
                
                if (data_020d433c.mBattlePlayers->mPlayers[ent->mPlayer]->IsCpu()) {
                    if (e[i].mUnk_0f_0) {
                        mUnk_56 = 0;
                    } else {
                        j = 0;
                        k = 0;
                        u8 mask = 0;
                        do {
                            if (!func_ov007_022433c8(i, data_020d433c.mBattlePlayers->mPlayers[e[i].mPlayer]->mSlots6c[j].mValue)) {
                                (&mask)[k / 8] |= 1 << (k % 8);
                            }
                            k++;
                            j++;
                        } while (j < data_020d433c.mUnk_99);
                        u8 other = (i + 1) % 2;
                        u8 pick  = PickUnflaggedSlot6c(data_020d433c.mBattlePlayers->mPlayers[ent->mPlayer], mask,
                                                       (s16) (e[other].mUnk_04 - e[i].mUnk_04), (s16) (e[other].mUnk_00 - e[i].mUnk_00));
                        if (pick == 0xff) {
                            mUnk_56 = 0;
                        } else {
                            BattlePlayer *p = data_020d433c.mBattlePlayers->mPlayers[e[i].mPlayer];
                            mData->mEntries[i].mCardId = p->mSlots6c[pick].mValue;
                            mData->mEntries[i].mSlot6cIndex = pick;
                            mData->mEntries[i].mUnk_0a = func_ov007_02243b24(i, data_020d433c.mBattlePlayers->mPlayers[e[i].mPlayer]->mSlots6c[pick].mValue);
                        }
                    }
                } else if (data_020d433c.mUnk_9a_2) {
                    if (e[i].mUnk_0f_0) {
                        mCardId = 0;
                    } else {
                        j = 0;
                        k = 0;
                        u8 mask = 0;
                        do {
                            if (!func_ov007_022433c8(i, data_020d433c.mBattlePlayers->mPlayers[e[i].mPlayer]->mSlots6c[j].mValue)) {
                                (&mask)[k / 8] |= 1 << (k % 8);
                            }
                            k++;
                            j++;
                        } while (j < data_020d433c.mUnk_99);
                        u8 other = (i + 1) % 2;
                        u8 pick  = PickUnflaggedSlot6c(data_020d433c.mBattlePlayers->mPlayers[ent->mPlayer], mask,
                                                       (s16) (e[other].mUnk_04 - e[i].mUnk_04), (s16) (e[other].mUnk_00 - e[i].mUnk_00));
                        if (pick == 0xff) {
                            mCardId = 0;
                        } else {
                            u8 idx = mSlot6cIndex = pick;
                            BattlePlayerSlot *slots = data_020d433c.mBattlePlayers->mPlayers[e[i].mPlayer]->mSlots6c;
                            mCardId = slots[idx].mValue;
                            mData->mEntries[i].mCardId = slots[idx].mValue;
                            mData->mEntries[i].mSlot6cIndex = mSlot6cIndex;
                            mData->mEntries[i].mUnk_0a = func_ov007_02243b24(i, slots[idx].mValue);
                        }
                    }
                }
                i++;
            } while (i < 2);
            mStep = 0x24;
            break;
        }
        case 36:
            if (mUnk_5e) {
                CloseCommandPage(mCommandPage, 1);
            } else {
                BattleCardSpec *spec = mCardSpec;
                spec->mUnk_1e = 0;
                CloseCardModel(spec->mCardModel);
            }
            mContext->mRequest = 0;
            mStep           = 0x25;
            break;
        case 37:
            if (mContext->mRequest == 0) {
                break;
            }
            if (mUnk_5e) {
                SpriteManager::Get().SetupMode(0, 7);
                GX_SetGraphicsMode(1, 0, 1);
            }
            mStep++;
            break;
        case 38: {
            if (!SetMessageTypeAndCheckAll(data_020d433c.mBattlePlayers, 0xf)) {
                break;
            }
            u8 i = 0;
            do {
                if (e[i].mCardId >= 0x92 && e[i].mCardId <= 0xa5) {
                    u8 o = i ^ 1;
                    if (e[o].mCardId != 0x8c && e[o].mCardId != 0x8d) {
                        mData->mEntries[i].mUnk_0f_4 = 1;
                    }
                }
                i++;
            } while (i < 2);
            mStep = 0x27;
            SpriteManager::Get().StartFade(FADE_MODE_DOWN);
            break;
        }
        case 39: {
            if (SpriteManager::Get().IsFading()) {
                break;
            }
            mContext->mRequest = 0;
            ResetDisplay();
            MI_CpuClearFast((void *) 0x6200000, 0x20000);
            MI_CpuClearFast((void *) 0x6607000, 0x19000);
            u8 kind = mData->mUnk_07;
            mData->func_ov007_0223e1a4(0, 0xff);
            switch (kind) {
                case 1:
                    mBattleMode = BATTLE_MODE_CONFRONT_BG01;
                    break;
                case 2:
                    mBattleMode = BATTLE_MODE_CONFRONT_BG04;
                    break;
                case 3:
                    mBattleMode = BATTLE_MODE_CONFRONT_BG02;
                    break;
                case 4:
                    mBattleMode = BATTLE_MODE_CONFRONT_03;
                    break;
                case 5:
                    mBattleMode = BATTLE_MODE_CONFRONT_BG05;
                    break;
                case 6:
                    mBattleMode = BATTLE_MODE_CONFRONT_BG06;
                    break;
                case 8:
                    mData->func_ov007_0223e180();
                    break;
            }
            switch (mBattleMode) {
                case BATTLE_MODE_CONFRONT_BG01:
                    mConfront = new ConfrontBg01();
                    break;
                case BATTLE_MODE_CONFRONT_BG02:
                    mConfront = new ConfrontBg02();
                    break;
                case BATTLE_MODE_CONFRONT_03:
                    mConfront = new Confront03();
                    break;
                case BATTLE_MODE_CONFRONT_BG04:
                    mConfront = new ConfrontBg04();
                    break;
                case BATTLE_MODE_CONFRONT_BG05:
                    mConfront = new ConfrontBg05();
                    break;
                case BATTLE_MODE_CONFRONT_BG06:
                    mConfront = new ConfrontBg06();
                    break;
            }
            ((BattleBase *) mConfront)->BattleBase::Init(mData, mBattleMode);
            mUnk_22 = 1;
            mStep++;
            break;
        }
        case 40:
            if (mContext->mRequest != 0) {
                mStep++;
            }
            break;
        case 41:
            if (mData->mUnk_07 == 0) {
                ResetDisplay();
                MI_CpuClearFast((void *) 0x6200000, 0x20000);
                LoadGraphics();
                SpriteManager::Get().StartFade(FADE_MODE_TO_ZERO);
                mUnk_22 = 0;
                mStep++;
            } else {
                mStep = 0x27;
            }
            break;
        case 42:
            if (SpriteManager::Get().IsFading()) {
                break;
            }
            if (e[0].mUnk_10 == 1) {
                mUnk_21 = 0;
            } else {
                mUnk_21 = 1;
            }
            if (data_020d433c.mAntenna) {
                data_020d433c.mAntenna->mSecondVisible = 1;
            }
            GXS_SetVisibleWnd(5);
            G2S_SetWndOutsidePlane(0x1f, FALSE);
            G2S_SetWnd0InsidePlane(0x3f, FALSE);
            mStep++;
            break;
        case 43:
            if (SetMessageTypeAndCheckAll(data_020d433c.mBattlePlayers, 0xf)) {
                mStep++;
            }
            break;
        case 44: {
            u32 kind;
            if (mUnk_21 == 0) {
                if (e[0].mUnk_0f_4) {
                    kind = 4;
                } else {
                    kind = 1;
                }
            } else {
                if (e[1].mUnk_0f_4) {
                    kind = 4;
                } else {
                    kind = 1;
                }
            }
            new BattleCardSpec(kind, e[mUnk_21].mBakuganId, e[mUnk_21].mAttribute, e[mUnk_21].mCharacterId == 0x15 ? 1 : 0);
            mContext->mRequest = 0;
            u8 x;
            if (mUnk_21 == 0) {
                x = 8;
            } else if (mUnk_21 == 1) {
                x = 0xb7;
            }
            reg_G2S_DB_WIN0H = (u16) (((x << 8) & 0xff00) | ((x + 0x40) & 0xff));
            reg_G2S_DB_WIN0V = 0x3c9c;
            SetSubBlendBrightness(0);
            mUnk_10 = 0;
            mStep++;
            break;
        }
        case 45: {
            if (mContext->mRequest != 0) {
                SetSubBlendBrightness(0);
                mStep++;
                break;
            }
            mUnk_10++;
            u8 t;
            if (mUnk_10 < 0x20) {
                t = 0;
            } else if (mUnk_10 < 0x30) {
                t = ((mUnk_10 - 0x20) * 16) / 16;
            } else if (mUnk_10 < 0x50) {
                t = 0x10;
            } else if (mUnk_10 >= 0x60) {
                t = 0;
            } else {
                t = 0x10 - ((mUnk_10 - 0x50) * 16) / 16;
            }
            SetSubBlendBrightness((s8) t);
            break;
        }
        case 46:
            if (mUnk_21 == 0) {
                mUnk_0e = 1;
            } else {
                mUnk_0e = 0;
            }
            new BattleCardSpec(2, e[mUnk_0e].mBakuganId, e[mUnk_0e].mAttribute, e[mUnk_0e].mCharacterId == 0x15 ? 1 : 0);
            mContext->mRequest = 0;
            mUnk_0f           = mUnk_21 == 0 ? 0xb7 : 8;
            mUnk_10           = 0;
            mStep++;
            break;
        case 47:
            if (mContext->mRequest != 0) {
                mStep++;
                SpriteManager::Get().StartFade(FADE_MODE_DOWN);
                break;
            }
            mUnk_10++;
            if ((mUnk_10 > 4) & (mUnk_10 < 0x40)) {
                mDrawX[mUnk_0e] = mUnk_0f + MATH_Rand32(&gRandom, 0x20) - 0x10;
                mDrawY[mUnk_0e] = MATH_Rand32(&gRandom, 0x18) + 0x30;
            } else {
                mDrawX[mUnk_0e] = mUnk_0f;
                mDrawY[mUnk_0e] = 0x3c;
            }
            break;
        case 48:
            if (SpriteManager::Get().IsFading()) {
                break;
            }
            mStep = 0x33;
            ResetDisplay();
            break;
        case 49: {
            if (SpriteManager::Get().IsFading()) {
                break;
            }
            BattleCardCamera *cam = mCardCamera;
            SpriteManager::Get();
            SpriteManager::sFade[0] = FADE_MODE_DOWN;
            data_020d433c.mFlags |= 4;
            cam->mState = 3;
            mContext->mRequest = 0;
            mStep++;
            break;
        }
        case 50:
            if (mContext->mRequest == 0) {
                break;
            }
            if (SetMessageTypeAndCheckAll(data_020d433c.mBattlePlayers, 0xf)) {
                mUnk_21 = func_ov007_02244440();
                mStep++;
            }
            break;
        case 51: {
            if (mUnk_21 != -1) {
                u8 found = 0;
                u8 idx   = 0;
                u8 i     = 0;
                u8 owner = data_020d433c.mBattlePlayers->mUnk_2a9;
                do {
                    if (owner == e[i].mPlayer) {
                        idx   = i;
                        found = 1;
                    }
                    i++;
                } while (i < 2);
                if (found == 1) {
                    if (idx == mUnk_21) {
                        u32 v = GetTexVariant(e[mUnk_21].mBakuganId, e[mUnk_21].mAttribute);
                        new BattleCharaAct(data_020d433c.mBattlePlayers->mCamera, e[mUnk_21].mPlayer, 10, v, 0);
                        if (data_020d433c.mUnk_7a) {
                            s16 d = (s16) (e[(mUnk_21 + 1) % 2].mUnk_04 - e[mUnk_21].mUnk_04);
                            if (d < 0) {
                                d = 0;
                            }
                            u16 add = d + 0x50;
                            if (e[mUnk_21].mUnk_0f_6) {
                                add = (u16) (add << 1);
                            }
                            if (data_020d433c.mTutorialNumber == 0) {
                                u8 sel = data_020d433c.mBattlePlayers->mUnitA;
                                if (mUnk_21 == 1) {
                                    sel = data_020d433c.mBattlePlayers->mUnitB;
                                }
                                BattlePlayer *p = data_020d433c.mBattlePlayers->mPlayers[e[mUnk_21].mPlayer];
                                BattleRecord *rec = &p->mRecords[data_020d433c.mBattlePlayers->mUnits[sel].mRecord];
                                if ((int) rec->mGauge < 0xfff - add) {
                                    rec->mGauge += add;
                                }
                                if (data_020d48f0.mUnk_d4 < 9999) {
                                    data_020d48f0.mUnk_d4++;
                                }
                            }
                        }
                        if (data_020d433c.mTutorialNumber == 2) {
                            data_020d433c.mBattlePlayers->mUnk_2b0 = 1;
                        }
                    } else {
                        new BattleCharaAct(data_020d433c.mBattlePlayers->mCamera, e[idx].mPlayer, 0xb, 0, 0);
                    }
                    if (data_020d433c.mTutorialNumber == 4 && e[idx].mUnk_0f_1) {
                        data_020d433c.mBattlePlayers->mUnk_2b0 = 1;
                    }
                } else {
                    u32 v = GetTexVariant(e[mUnk_21].mBakuganId, e[mUnk_21].mAttribute);
                    new BattleCharaAct(data_020d433c.mBattlePlayers->mCamera, e[mUnk_21].mPlayer, e[mUnk_21].mPlayer + 0x10, v, 0);
                }
                mContext->mRequest = 0;
            } else {
                mContext->mRequest = 1;
            }
            mStep++;
            break;
        }
        case 52: {
            if (mContext->mRequest == 0) {
                break;
            }
            if (mUnk_21 != -1) {
                BattlePlayer *p = data_020d433c.mBattlePlayers->mPlayers[e[mUnk_21].mPlayer];
                u8 other        = 0;
                p->mUnk_fe      = 1;
                p->mUnk_ee++;
                if (mUnk_21 == 0) {
                    other = 1;
                }
                data_020d433c.mBattlePlayers->mPlayers[e[mUnk_21].mPlayer]->PushHistory(e[other].mUnk_04, e[other].mAttribute);
            }
            RemoveMarker(data_020d433c.mBattlePlayers, data_020d433c.mBattlePlayers->mUnits[data_020d433c.mBattlePlayers->mUnitA].mMarker);
            RemoveUnit(data_020d433c.mBattlePlayers, data_020d433c.mBattlePlayers->mUnitA, 2);
            RemoveUnit(data_020d433c.mBattlePlayers, data_020d433c.mBattlePlayers->mUnitB, 2);
            mStep = 0x35;
            break;
        }
        case 53:
            return TRUE;
    }
    return FALSE;
}

int BattleConfrontCard::func_ov007_0224251c() {
    int result      = 0;
    BattleEntry *e  = mData->mEntries;
    switch (mUnk_51) {
        case 0:
            if (data_020d433c.mTutorialNumber == 4) {
                mContext->mRequest = 0;
                mUnk_51           = 2;
                BattleCardSpec *spec = mCardSpec;
                spec->mUnk_1e        = 0;
                CloseCardModel(spec->mCardModel);
            } else {
                new BattleUnionAbility(0, 0x1a000, 0xc);
                mContext->mRequest = 0;
                mUnk_51++;
            }
            break;
        case 1: {
            u8 v = mContext->mRequest;
            if (v == 0) {
                break;
            }
            mContext->mRequest = 0;
            switch (v) {
                case 1: {
                    mUnk_51              = 2;
                    BattleCardSpec *spec = mCardSpec;
                    spec->mUnk_1e        = 0;
                    CloseCardModel(spec->mCardModel);
                    break;
                }
                case 2:
                    result = 2;
                    break;
            }
            break;
        }
        case 2:
            if (mContext->mRequest == 0) {
                break;
            }
            mCommandPage = new BattleCommandPage(0, -1);
            func_0205562c(mCommandPage, 1);
            func_02055738(mCommandPage, 0xe000, 7, 0);
            new BattleCommandPrompt(3, mCommandPage, mDisabledBits, 0x7000, 0);
            mContext->mRequest = 0;
            mUnk_51++;
            break;
        case 3: {
            u16 v = mContext->mRequest;
            if (v == 0) {
                break;
            }
            if (v != 10) {
                GXS_SetVisiblePlane(0x11);
                mSlot6cIndex = GetLowByte(v);
                result  = 1;
                mCardId = data_020d433c.mBattlePlayers->mPlayers[e[mUnk_52].mPlayer]->Slot6c(mSlot6cIndex);
            } else {
                CloseCommandPage(mCommandPage, 1);
                mContext->mRequest = 0;
                mUnk_51           = 6;
            }
            break;
        }
        case 4:
            new BattleUnionAbility(1, 0x1a000, 0xc);
            mContext->mRequest = 0;
            mUnk_51++;
            break;
        case 5: {
            u8 v = mContext->mRequest;
            if (v == 0) {
                break;
            }
            switch (v) {
                case 1:
                    result = 1;
                    break;
                case 2:
                    mUnk_51 = 6;
                    CloseCommandPage(mCommandPage, 1);
                    mContext->mRequest = 0;
                    break;
            }
            break;
        }
        case 6: {
            if (mContext->mRequest == 0) {
                break;
            }
            SpriteManager::Get().SetupMode(0, 7);
            GX_SetGraphicsMode(1, 0, 1);
            mCardSpec = new BattleCardSpec(3, e[mUnk_52].mBakuganId, e[mUnk_52].mAttribute, e[mUnk_52].mCharacterId == 0x15 ? 1 : 0);
            mContext->mRequest = 0;
            mUnk_51           = 0;
            break;
        }
    }
    return result;
}

u8 BattleConfrontCard::func_ov007_022428d4(u16 card) {
    u8 j;
    u8 result = 0;
    u8 type;
    u16 power;
    mUnk_5c   = 0;
    mUnk_5d   = 0;
    mUnk_61   = 0;
    u8 i           = 0;
    u8 side;
    BattleEntry *e = mData->mEntries;
    do {
        side  = 0;
        u8 other = 1;
        if (i == 1) {
            side  = 1;
            other = 0;
        }
        BattleEntry *opp;
        u8 oppLevel;
        BattleEntry *self = e;
        self += side;
        u8 selfLevel = data_020d433c.mBattlePlayers->mPlayers[self->mPlayer]->mUnk_ee;
        opp          = e;
        opp += other;
        oppLevel = data_020d433c.mBattlePlayers->mPlayers[opp->mPlayer]->mUnk_ee;
        type              = GetCardInfo(card, 3);
        switch (type) {
        case 1: {
            u8 want = card - 1;
            if (card >= 7) {
                want++;
            } else if (card == 6) {
                if (e[i].mBakuganId == 6) {
                    want = 6;
                }
            }
            if (want == e[i].mBakuganId) {
                power     = GetCardAttributeValue(card, e[i].mAttribute) * 10;
                u16 alt   = 0;
                if (e[i].mBakuganId == 5) {
                    alt = GetCardAttributeValue(card, 3) * 10;
                } else if (e[i].mBakuganId == 6) {
                    alt = GetCardAttributeValue(card, 0) * 10;
                }
                if (power < alt) {
                    power = alt;
                }
                mData->mEntries[i].mUnk_08 = power;
                (&mUnk_5c)[i]              = 1;
            }
            break;
        }
        case 4:
            if (e[side].mUnk_00 < e[other].mUnk_00) {
                mData->mEntries[side].mUnk_08 = e[side].mUnk_00;
                (&mUnk_5c)[side]              = 1;
            }
            break;
        }
        switch (card) {
            case 0x48:
                mData->mEntries[i].mUnk_0f_0 = 1;
                (&mUnk_5c)[i]                = 1;
                break;
            case 0x49:
                if (selfLevel > oppLevel) {
                    mData->mEntries[side].mUnk_0f_0 = 1;
                    (&mUnk_5c)[side]                = 1;
                }
                break;
            case 0x4a:
                if (e[side].mUnk_06 > e[other].mUnk_06) {
                    mData->mEntries[side].mUnk_0f_0 = 1;
                    (&mUnk_5c)[side]                = 1;
                }
                break;
            case 0x4b:
                if (e[side].mUnk_04 > e[other].mUnk_04) {
                    mData->mEntries[side].mUnk_0f_0 = 1;
                    (&mUnk_5c)[side]                = 1;
                }
                break;
            case 0x4c:
                if (e[side].mUnk_06 < e[other].mUnk_06) {
                    mData->mEntries[side].mUnk_0f_0 = 1;
                    (&mUnk_5c)[side]                = 1;
                }
                break;
            case 0x4d: {
                j = 0;
                do {
                    SetSlot6cFlag(data_020d433c.mBattlePlayers->mPlayers[e[side].mPlayer], j, 0);
                    j++;
                } while (j < data_020d433c.mUnk_99);
                (&mUnk_5c)[i] = 1;
                break;
            }
            case 0x4e:
                if (selfLevel < oppLevel) {
                    j = 0;
                    do {
                        SetSlot6cFlag(data_020d433c.mBattlePlayers->mPlayers[e[side].mPlayer], j, 0);
                        j++;
                    } while (j < data_020d433c.mUnk_99);
                    (&mUnk_5c)[side] = 1;
                }
                break;
            case 0x4f:
            case 0x50:
            case 0x51:
                if (selfLevel < oppLevel) {
                    j = 0;
                    do {
                        if (j == (u8) (card - 0x4f)) {
                            SetSlot6cFlag(data_020d433c.mBattlePlayers->mPlayers[e[side].mPlayer], j, 0);
                        }
                        j++;
                    } while (j < data_020d433c.mUnk_99);
                    (&mUnk_5c)[side] = 1;
                }
                break;
            case 0x52:
            case 0x53:
            case 0x54:
                if (e[side].mUnk_04 < e[other].mUnk_04) {
                    j = 0;
                    do {
                        if (j == (u8) (card - 0x52)) {
                            SetSlot6cFlag(data_020d433c.mBattlePlayers->mPlayers[e[side].mPlayer], j, 0);
                        }
                        j++;
                    } while (j < data_020d433c.mUnk_99);
                    (&mUnk_5c)[side] = 1;
                }
                break;
            case 0x55:
                mUnk_61       = 1;
                (&mUnk_5c)[i] = 1;
                break;
            case 0x5d:
            case 0x5f:
                (&mUnk_5c)[i] = 1;
                break;
            case 0x5e:
                if (e[i].mAttribute == 0 || e[i].mBakuganId == 6) {
                    mUnk_5c = 1;
                    mUnk_5d = 1;
                }
                break;
            case 0x60:
                if (e[i].mAttribute == 3 || e[i].mBakuganId == 5) {
                    mUnk_5c = 1;
                    mUnk_5d = 1;
                }
                break;
            case 0x61:
                if (e[i].mAttribute == 2) {
                    mUnk_5c = 1;
                    mUnk_5d = 1;
                }
                break;
            case 0x63:
                if (e[i].mAttribute == 5) {
                    mUnk_5c = 1;
                    mUnk_5d = 1;
                }
                break;
            case 0x64:
                if (e[i].mAttribute == 4) {
                    mUnk_5c = 1;
                    mUnk_5d = 1;
                }
                break;
            case 0x65:
                if (e[i].mAttribute == 1) {
                    mUnk_5c = 1;
                    mUnk_5d = 1;
                }
                break;
            case 0x62: {
                u8 j = 0;
                u8 n            = 0;
                do {
                    if (data_020d433c.mBattlePlayers->mPlayers[self->mPlayer]->mSlots6c[j].mFlag == 2) {
                        n++;
                    }
                    j++;
                } while (j < data_020d433c.mUnk_99);
                mData->mEntries[side].mUnk_08 = n * 100;
                (&mUnk_5c)[side]              = 1;
                break;
            }
            case 0x66:
                mData->mEntries[side].mUnk_08 = selfLevel * 100;
                (&mUnk_5c)[side]              = 1;
                break;
            case 0x67: {
                u8 j = 0;
                u8 n            = 0;
                do {
                    if (data_020d433c.mBattlePlayers->mPlayers[self->mPlayer]->mRecords[j].mStatus == 1) {
                        n++;
                    }
                    j++;
                } while (j < data_020d433c.mUnk_99);
                mData->mEntries[side].mUnk_08 = n * 150;
                (&mUnk_5c)[side]              = 1;
                break;
            }
        }
        if (mUnk_5c != 0 || mUnk_5d != 0) {
            result = type;
        }
        i++;
    } while (i < 2);
    return result;
}

void BattleConfrontCard::SetSubBlendBrightness(int brightness) {
    G2S_SetBlendBrightness(0x1f, brightness);
}

void BattleConfrontCard::SetSubBlendAlpha(int blend) {
    G2S_SetBlendAlpha(0x10, 0x2f, 0x10 - blend, blend);
}

void BattleConfrontCard::func_ov007_02243100() {
    u8 i = 0;
    do {
        mData->DrawBakuganAndNumber(i, mDrawX[i], mDrawY[i], 2, 0xffff);
        i++;
    } while (i < 2);
    mData->DrawCharacterIcons(0, mDrawX[0], (s16) (mDrawY[0] - 0x1e));
    mData->DrawCharacterIcons(1, mDrawX[1], (s16) (mDrawY[1] - 0x1e));
    mData->mCharacterIcons->DrawIcons();
    mCellAnims[0]->Draw(1, mDrawX[2], mDrawY[2], 3);
    if (mUnk_5f == 0) {
        return;
    }
    i = 0;
    do {
        u32 value;
        u8 digits;
        int margin;
        u8 j;
        value = GetCardAttributeValue(mData->mMarkerCardId, i);
        if (value != 0) {
            digits = 3;
            if (value < 10) {
                digits--;
            }
            value *= 10;
            value &= 0xffff;
            margin = (3 - digits) * 4;
            j      = 0;
            do {
                mCellAnims[3]->DrawFrame(1, (s32) value % 10, (s16) ((digits - j) * 4 + 0x50 + margin / 2),
                                          (s16) (i * 17 + 0x28), 2);
                value /= 10;
                j++;
            } while (j < digits);
        }
        mCellAnims[2]->DrawFrame(1, i, 0x53, (s16) (i * 17 + 0x24), 2);
        i++;
    } while (i < 6);
}

void BattleConfrontCard::DrawEntryNumbers(u8 kind, u32 mode, u32 flag) {
    if (flag == 0) {
        mData->mCellAnims[4]->mPaletteBase = data_ov007_02279d3c[0];
        mData->mCellAnims[3]->mPaletteBase = data_ov007_02279d3c[0];
    } else {
        mData->mCellAnims[4]->mPaletteBase = 0xb;
        mData->mCellAnims[3]->mPaletteBase = 0xb;
    }
    mData->DrawEntryNumber(0, kind, mDrawX[0], 0x54, 2, mode, mUnk_10, mUnk_12);
    mData->DrawEntryNumber(1, kind, mDrawX[1], 0x54, 2, mode, mUnk_10, mUnk_12);
}

u8 BattleConfrontCard::GetMarkerBattleMode() {
    int kind = GetCardInfo(mData->mMarkerCardId, 2);
    return kind;
}

BOOL BattleConfrontCard::func_ov007_022433c8(u8 side, u16 id) {
    BOOL result        = FALSE;
    u8 i               = 0;
    u8 found           = 0;
    u32 type           = 0;
    BattleEntry *self  = &mData->mEntries[side];
    u8 opponent        = side ^ 1;
    BattleEntry *other = &mData->mEntries[opponent];
    u8 owner           = mData->mMarkerPlayer;
    u8 pattern[3];
    switch (id) {
        case 0x68:
        case 0x70:
        case 0x80:
            if (self->mAttribute == 0 || self->mBakuganId == 6) {
                result = TRUE;
            }
            break;
        case 0x69:
        case 0x71:
        case 0x81:
            if (self->mAttribute == 1) {
                result = TRUE;
            }
            break;
        case 0x6a:
        case 0x72:
        case 0x82:
            if (self->mAttribute == 2) {
                result = TRUE;
            }
            break;
        case 0x6b:
        case 0x73:
        case 0x83:
            if (self->mAttribute == 3 || self->mBakuganId == 5) {
                result = TRUE;
            }
            break;
        case 0x6c:
        case 0x74:
        case 0x84:
            if (self->mAttribute == 4) {
                result = TRUE;
            }
            break;
        case 0x6d:
        case 0x75:
        case 0x85:
            if (self->mAttribute == 5) {
                result = TRUE;
            }
            break;
        case 0x6e:
        case 0x76:
        case 0x86:
            if ((self->mBakuganId == 5 && other->mAttribute == 3) || (self->mBakuganId == 6 && other->mAttribute == 0) ||
                (other->mBakuganId == 5 && self->mAttribute == 3) || (other->mBakuganId == 6 && self->mAttribute == 0)) {
                result = TRUE;
            }
            if (self->mAttribute == other->mAttribute) {
                result = TRUE;
            }
            break;
        case 0x6f:
        case 0x77:
        case 0x87:
            if (self->mAttribute != other->mAttribute) {
                if (!(self->mBakuganId == 5 && other->mAttribute == 3) && !(self->mBakuganId == 6 && other->mAttribute == 0) &&
                    !(other->mBakuganId == 5 && self->mAttribute == 3) && !(other->mBakuganId == 6 && self->mAttribute == 0)) {
                    result = TRUE;
                }
            }
            break;
        case 0x78:
        case 0x79:
        case 0x7a:
        case 0x7b:
        case 0x7c:
        case 0x7d:
        case 0x7e:
        case 0x7f:
        case 0x88:
        case 0x89:
        case 0x8a:
        case 0x8b:
        case 0x8c:
        case 0x8d:
        case 0x8e:
            result = TRUE;
            break;
        case 0x8f:
            type = 0;
            goto kind;
        case 0x90:
            type = 1;
            goto kind;
        case 0x91:
            type = 2;
        kind:
            if (type == GetCardInfo(mData->mMarkerCardId, 1)) {
                result = TRUE;
            }
            break;
        case 0x92:
            type = 0;
            goto matchType;
        case 0x93:
            type = 0x1;
            goto matchType;
        case 0x9f:
            type = 0x10;
            goto matchType;
        case 0x96:
            type = 0x4;
            goto matchType;
        case 0x9e:
            type = 0xf;
            goto matchType;
        case 0x97:
            type = 0x7;
            goto matchType;
        case 0x98:
            type = 0x8;
            goto matchType;
        case 0x9d:
            type = 0xe;
            goto matchType;
        case 0x94:
            type = 0x2;
            goto matchType;
        case 0x95:
            type = 0x3;
            goto matchType;
        case 0xa0:
            type = 0x11;
            goto matchType;
        case 0x9b:
            type = 0xb;
            goto matchType;
        case 0x9c:
            type = 0xc;
            goto matchType;
        case 0x99:
            type = 0x9;
            goto matchType;
        case 0x9a:
            type = 0xa;
            goto matchType;
        case 0xa1:
            type = 0x12;
            goto matchType;
        case 0xa2:
            type = 0x24;
            goto matchType;
        case 0xa3:
            type = 0x25;
            goto matchType;
        case 0xa4:
            type = 0x26;
            goto matchType;
        case 0xa5:
            type = 0x27;
            goto matchType;
        case 0xa7:
            type = 0xd;
            goto matchType;
        matchType:
            if (self->mBakuganId == type) {
                result = TRUE;
            }
            break;
        case 0xa6:
            if ((u8) (self->mBakuganId + 0xfb) <= 1) {
                result = TRUE;
            }
            break;
        case 0xac:
        case 0xb5:
            if (owner != self->mPlayer) {
                break;
            }
            goto abilityCard;
        case 0xad:
        case 0xae:
        case 0xb6:
        case 0xb7:
            if (owner == self->mPlayer) {
                break;
            }
            if (data_020d433c.mHasPartners == 0) {
                goto abilityCard;
            }
            if (owner == data_020d433c.mBattlePlayers->mPlayers[self->mPlayer]->mPartner) {
                break;
            }
            goto abilityCard;
        case 0xb9:
        case 0xba:
            if (data_020d433c.mBattlePlayers->mPlayers[self->mPlayer]->mUnk_ee == 0) {
                break;
            }
            goto abilityCard;
        case 0xbb:
        case 0xbc:
        case 0xbd:
            if (self->mUnk_04 >= other->mUnk_04) {
                break;
            }
            goto abilityCard;
        case 0xc1:
            if (other->mAttribute != 2) {
                break;
            }
            goto abilityCard;
        case 0xc2:
            if (other->mAttribute != 3 && other->mBakuganId != 5) {
                break;
            }
            goto abilityCard;
        case 0xc3:
            if (other->mAttribute != 4) {
                break;
            }
            goto abilityCard;
        case 0xc4:
            if (other->mAttribute != 1) {
                break;
            }
            goto abilityCard;
        case 0xc5:
            if (other->mAttribute != 5) {
                break;
            }
            goto abilityCard;
        case 0xc6:
            if (other->mAttribute != 0 && other->mBakuganId != 6) {
                break;
            }
        case 0xa8:
        case 0xa9:
        case 0xaa:
        case 0xab:
        case 0xaf:
        case 0xb0:
        case 0xb1:
        case 0xb2:
        case 0xb3:
        case 0xb4:
        case 0xb8:
        abilityCard:
            if (self->mBakuganId == 5) {
                if (GetCardAttributeValue(id, 3) != 0) {
                    result = TRUE;
                }
            }
            if (self->mBakuganId == 6) {
                if (GetCardAttributeValue(id, 0) != 0) {
                    result = TRUE;
                }
            }
            if (GetCardAttributeValue(id, self->mAttribute) != 0) {
                result = TRUE;
            }
            break;
        case 0xbe:
            pattern[0] = 0;
            pattern[1] = 4;
            goto scan2;
        case 0xbf:
            pattern[0] = 5;
            pattern[1] = 3;
            goto scan2;
        case 0xc0:
            pattern[0] = 2;
            pattern[1] = 1;
        scan2 : {
            BattlePlayer *player = data_020d433c.mBattlePlayers->mPlayers[self->mPlayer];
            u8 playerCount       = data_020d433c.mUnk_99;
            do {
                u8 kind = player->mRecords[i].mAttribute;
                u8 j    = 0;
                do {
                    if (kind == pattern[j]) {
                        found++;
                        pattern[j] = 0xff;
                        if (found >= 2) {
                            result = TRUE;
                        }
                    }
                    j++;
                } while (j < 2);
                i++;
            } while (i < playerCount);
        }
            break;
        case 0xc7:
            pattern[0] = 0;
            pattern[1] = 1;
            pattern[2] = 3;
            goto scan3;
        case 0xc8:
            pattern[0] = 2;
            pattern[1] = 4;
            pattern[2] = 5;
        scan3 : {
            BattlePlayer *player = data_020d433c.mBattlePlayers->mPlayers[self->mPlayer];
            u8 playerCount       = data_020d433c.mUnk_99;
            do {
                u8 kind = player->mRecords[i].mAttribute;
                u8 j    = 0;
                do {
                    if (kind == pattern[j]) {
                        found++;
                        pattern[j] = 0xff;
                        if (found >= 3) {
                            result = TRUE;
                        }
                    }
                    j++;
                } while (j < 3);
                i++;
            } while (i < playerCount);
        }
            break;
    }
    return result;
}

u16 BattleConfrontCard::func_ov007_02243b24(u8 side, u32 id) {
    u16 value      = 0;
    BattleEntry *e = &mData->mEntries[side];
    switch (id) {
        case 0x92:
        case 0x93:
        case 0x94:
        case 0x95:
        case 0x96:
        case 0x97:
        case 0x98:
        case 0x99:
        case 0x9a:
        case 0x9b:
        case 0x9c:
        case 0x9d:
        case 0x9e:
        case 0x9f:
        case 0xa0:
        case 0xa1:
        case 0xa2:
        case 0xa3:
        case 0xa4:
        case 0xa5:
        case 0xa6:
        case 0xa7:
        case 0xbe:
        case 0xbf:
        case 0xc0:
        case 0xc1:
        case 0xc2:
        case 0xc3:
        case 0xc4:
        case 0xc5:
        case 0xc6:
            value = 200;
            break;
        case 0xc7:
        case 0xc8:
            value = 400;
            break;
        case 0xb9:
        case 0xba:
            value = data_020d433c.mBattlePlayers->mPlayers[e->mPlayer]->mUnk_ee * 50;
            break;
        case 0xa8:
        case 0xa9:
        case 0xaa:
        case 0xab:
        case 0xac:
        case 0xad:
        case 0xae:
        case 0xaf:
        case 0xb0:
        case 0xb1:
        case 0xb2:
        case 0xb3:
        case 0xb4:
        case 0xb5:
        case 0xb6:
        case 0xb7:
        case 0xb8:
        case 0xbb:
        case 0xbc:
        case 0xbd: {
            value   = GetCardAttributeValue(id, e->mAttribute) * 10;
            u16 old = value;
            if (e->mBakuganId == 5) {
                if (value < GetCardAttributeValue(id, 3) * 10) {
                    value = GetCardAttributeValue(id, 3) * 10;
                }
            }
            if (e->mBakuganId == 6) {
                if (old < GetCardAttributeValue(id, 0) * 10) {
                    value = GetCardAttributeValue(id, 0) * 10;
                }
            }
            break;
        }
    }
    if (mUnk_61) {
        value = value << 1;
    }
    return value;
}

void BattleConfrontCard::OpenWindow() {
    GXS_SetVisiblePlane(0x13);
    if (mWindow) {
        mWindow->mEnabled = 1;
    }
    mWindowOpen = 1;
    if (data_020d433c.mAntenna) {
        data_020d433c.mAntenna->mSecondVisible = 1;
    }
    G2S_SetWnd0Position(0x10, 0x10, 0xf0, 0xb0);
    GXS_SetVisibleWnd(5);
    G2S_SetWnd0InsidePlane(0x13, FALSE);
    G2S_SetWndOutsidePlane(0x13, TRUE);
    G2S_SetBlendBrightness(0x11, -8);
}

void BattleConfrontCard::CloseWindow() {
    GXS_SetVisiblePlane(0x11);
    if (mWindow) {
        mWindow->mEnabled = 0;
    }
    mWindowOpen = 0;
    G2S_SetBlendBrightness(0x11, 0);
    if (data_020d433c.mAntenna) {
        data_020d433c.mAntenna->mSecondVisible = 0;
    }
    GXS_SetVisibleWnd(0);
    G2S_SetWnd0InsidePlane(0x1f, FALSE);
    G2S_SetWndOutsidePlane(0x3f, FALSE);
}

void BattleConfrontCard::ReleaseWindow() {
    if (mWindow) {
        delete mWindow;
    }
    mWindow = 0;
    if (mMessages) {
        OS_Free(mMessages);
        mMessages = 0;
    }
    if (mTextBuffer) {
        OS_Free(mTextBuffer);
        mTextBuffer = 0;
    }
    if (mGlyphBuffer) {
        OS_Free(mGlyphBuffer);
        mGlyphBuffer = 0;
    }
}

void BattleConfrontCard::LoadWindow(const char *path, u32 mode) {
    ChangeDir("/Game/2D/union_b/");
    LoadAndQueueUpload(1, UPLOAD_KIND_BG1_SCR, 0, "union_b_bg_window_b_nsc.bin", 0, 0, 0);
    LoadAndQueueUpload(1, UPLOAD_KIND_BG1_CHAR, 0, "union_b_bg_window_ncg.bin", 0, 0, 0);
    LoadAndQueueUpload(1, UPLOAD_KIND_BG_EXT_PLTT, 0x2000, "union_b_bg_window_ncl.bin", 0, 0, 0);
    SpriteScreen &screen = SpriteManager::Get().Screen(1);
    ChangeDir("/Game/2D/union_b/");
    screen.LoadCharacters("union_b_obj_window.nce_ncg1D.bin", 0xc000, 0);
    CellAnimation *old = mCellAnims[1];
    if (old) {
        delete old;
    }
    mCellAnims[1]            = 0;
    mCellAnims[1]            = new CellAnimation("union_b_obj_window_nce.bin", 0, 1, 1);
    mCellAnims[1]->mCharBase = screen.GetCharBase(0xc000);
    mCellAnims[1]->mPaletteBase   = 7;
    mCellAnims[1]->SetAnimation(0xc);
    if (mMessages) {
        OS_Free(mMessages);
    }
    if (mTextBuffer) {
        OS_Free(mTextBuffer);
    }
    if (mGlyphBuffer) {
        OS_Free(mGlyphBuffer);
    }
    mMessages = (MessageFile *) LoadFile(path, 0, 0, 0, 0, 0);
    mTextBufferSize = TextManager::Get().BufferSize(0x18, 0xc);
    mGlyphCount = TextManager::Get().GlyphCount(9);
    mTextBuffer = OS_Alloc(mTextBufferSize);
    mGlyphBuffer = OS_Alloc(mGlyphCount * 12);
    TextManager::Get().Init(mGlyphBuffer, 1, 0, 0x18, 0xc, 0x42);
    PrintMessage(mode);
    if (mode != 4) {
        if (mWindow) {
            delete mWindow;
        }
        mWindow = new SelectWindow(3, sWindowItems, 1);
        mWindow->SetCells(3, sWindowCells);
        mWindow->mCursor->mSelection = -1;
        mWindow->mEnabled            = 0;
    }
    ChangeDir("/");
}

void BattleConfrontCard::PrintMessage(u32 index) {
    MI_CpuClear16(mTextBuffer, mTextBufferSize);
    TextManager &text = TextManager::Get();
    s16 height;
    s16 width;
    width              = 0;
    height             = 0;
    MessageFile *file = mMessages;
    text.Print(file->GetText(index), file->GetLength(index), mTextBuffer, 0x18, 0xc, &height, &width, 0, 0, 2, 0x42, 0x10, 0, 0, 0, 1, 1);
    QueueUpload(SpriteManager::Get().Screen(1).mBgIndex, UPLOAD_KIND_OBJ, 0xd000, mTextBuffer, mTextBufferSize, 0);
}

u8 BattleConfrontCard::func_ov007_02244440() {
    BattleData *data = mData;
    BattleEntry *e   = data->mEntries;
    mUnk_50          = func_ov007_022428d4(data->mMarkerCardId);
    if (mUnk_50 == 5) {
        mData->mEntries[0].mUnk_02 = e[1].mUnk_04 + e[0].mUnk_00 - e[0].mUnk_04;
        mData->mEntries[1].mUnk_02 = e[0].mUnk_04 + e[1].mUnk_00 - e[1].mUnk_04;
        u16 b = e[1].mUnk_04;
        u16 a = e[0].mUnk_04;
        mData->mEntries[1].mUnk_04 = a;
        mData->mEntries[0].mUnk_04 = b;
    }
    u8 i = 0;
    do {
        BattleEntry *cur = &e[i];
        if (e[i].mUnk_0f_0) {
            mUnk_56 = 0;
        } else {
            u8 j    = 0;
            u8 k    = 0;
            u8 mask = 0;
            do {
                if (!func_ov007_022433c8(i, data_020d433c.mBattlePlayers->mPlayers[e[i].mPlayer]->mSlots6c[j].mValue)) {
                    (&mask)[k / 8] |= 1 << (k % 8);
                }
                k++;
                j++;
            } while (j < data_020d433c.mUnk_99);
            u8 other = (i + 1) % 2;
            mSlot6cIndex = PickUnflaggedSlot6c(data_020d433c.mBattlePlayers->mPlayers[cur->mPlayer], mask,
                                          (s16) (e[other].mUnk_04 - e[i].mUnk_04), (s16) (e[other].mUnk_00 - e[i].mUnk_00));
            if (mSlot6cIndex == 0xff) {
                mUnk_56 = 0;
            } else {
                SetSlot6cFlag(data_020d433c.mBattlePlayers->mPlayers[e[i].mPlayer], mSlot6cIndex, 2);
            }
        }
        i++;
    } while (i < 2);
    u16 power[2];
    u8 n = 0;
    do {
        power[n] = e[n].mUnk_04;
        power[n] += e[n].mUnk_06;
        power[n] += e[n].mUnk_08;
        power[n] += e[n].mUnk_0a;
        power[n] = power[n] * func_ov007_022234f0(e[n].mPlayer, mBattleMode, e[n].mCharacterId);
        n++;
    } while (n < 2);
    u8 mode = data_020d433c.mFieldId;
    if (mode != 0 && mode != 7) {
        const s32 *row = sAttributeRates[(u8) (mode - 1)];
        u8 m           = 0;
        do {
            float f  = (float) row[e[m].mAttribute] / 4096.0f;
            power[m] = (u16) (f * (float) power[m]);
            m++;
        } while (m < 2);
    }
    u16 chance = (u16) (100.0f * ((float) power[0] / (float) (power[0] + power[1])));
    BOOL r = MATH_Rand32(&gRandom, 100) >= chance;
    return r;
}
