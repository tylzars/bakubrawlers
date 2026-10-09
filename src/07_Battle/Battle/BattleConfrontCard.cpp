#include "System/InputManager.hpp"
#include "System/SoundManager.hpp"
#include "Battle/BattleConfrontCard.hpp"
#include "Battle/BattleCardSpec.hpp"
#include "Battle/BattleCommandPrompt.hpp"
#include "Battle/BattleCommandList.hpp"
#include "Battle/BattleCommandPage.hpp"
#include "Battle/BattleUnionAbility.hpp"
#include "Game/UnkStruct_020d433c.hpp"
#include "Graphics/CellAnimation.hpp"
#include "Graphics/Display.hpp"
#include "Graphics/SpriteManager.hpp"
#include "System/FileSystem.hpp"
#include "System/FrameTask.hpp"
#include "System/SoundManager.hpp"
#include "Text/TextManager.hpp"
#include "Unknown/UnkFunctions.hpp"
#include "UI/SelectWindow.hpp"
#include "Unknown/UnkStruct_02078290.hpp"
#include "Battle/BattleCharaAct.hpp"
#include "Battle/ConfrontBg01.hpp"
#include "Battle/ConfrontBg02.hpp"
#include "Battle/Confront03.hpp"
#include "Battle/ConfrontBg04.hpp"
#include "Battle/ConfrontBg05.hpp"
#include "Game/UnkStruct_020d48f0.hpp"
#include "Unknown/UnkStruct_02087898.hpp"
#include "Graphics/Camera.hpp"
#include "System/InputManager.hpp"
#include "System/TouchManager.hpp"
#include "Game/Random.hpp"
#include "System/WirelessManager.hpp"
#include <nitro/math.h>
#include <nitro/gx.h>
#include <nitro/mi.h>
#include <nitro/os.h>
#include <stdio.h>

void func_ov007_0226a448(BattlePlayer *player, u8 a, int b);
extern "C" u32 func_ov007_022234f0(int player, u8 mode, u8 ch);
extern const s32 data_ov007_02279df0[][6];

void func_ov007_0226a448(BattlePlayer *player, u8 index, int flag);

extern const s8 data_ov007_02279d3c[];
u8 func_02065bf4(u32 id, u32 index);

extern "C" BOOL func_ov007_02263150(BattlePlayerList *list);
extern "C" void func_ov007_022633f8(BattlePlayerList *list);
BOOL func_ov007_022631bc(BattlePlayerList *list, u32 id);
extern "C" void func_020383f8(u32 heap);
extern "C" void GX_SetGraphicsMode(u32 unk1, u32 unk2, u32 unk3);
extern "C" void GXS_SetGraphicsMode(u32 unk1);

class ConfrontBg06 : public BattleBase {
public:
    PAD(0x68, 0x70);
    ConfrontBg06();
    virtual ~ConfrontBg06();
    virtual void vfunc_28();
    virtual void vfunc_34();
    virtual void vfunc_3c();
    virtual u8 vfunc_40(int a);
};
extern "C" u32 func_ov007_022696a0(u32 a, u32 b);
extern "C" void func_ov007_022626b8(BattlePlayerList *list, u8 index);
u8 func_ov007_0226a700(BattlePlayer *player, u8 mask, s16 x, s16 y);


BattleConfrontCard::BattleConfrontCard(u32 unk)
    : SceneBase(0x3e8, 0) {
    mState    = 0;
    mUnk_0d   = 0;
    mUnk_0e   = 0;
    mUnk_0f   = 0;
    mUnk_10   = 0;
    mUnk_12   = 0;
    mUnk_20   = 0;
    mUnk_21   = -1;
    mUnk_22   = 0;
    mUnk_23   = 0;
    mData     = 0;
    mUnk_3c   = 0;
    mUnk_40   = 0;
    mUnk_44   = 0;
    mUnk_48   = 0;
    mUnk_4c   = 0;
    mUnk_50   = 0;
    mUnk_51   = 0;
    mUnk_52   = 0;
    mUnk_53   = 0;
    mUnk_54   = 0;
    mUnk_56   = 0;
    mUnk_5e   = 0;
    mUnk_5f   = 0;
    mUnk_60   = 0;
    mUnk_61   = 0;
    mUnk_66   = 0;
    mUnk_68   = 0;
    mUnk_69   = 0;
    mUnk_6c   = 0;
    mUnk_70   = 0;
    mUnk_78   = 0;
    mUnk_7c   = 0;
    mUnk_80   = 0;
    mUnk_84   = 0;
    mUnk_74   = 0;
    u8 i = 0;
    do {
        mUnk_14[i] = -0x60;
        mUnk_1a[i] = 0x3c;
        i++;
    } while (i < 3);
    MI_CpuClearFast(mUnk_24, 0x14);
    mData = new BattleData();
    if (unk != -1) {
        mUnk_20 = unk;
    } else {
        mUnk_20 = func_ov007_022433ac();
    }
    mUnk_62 = 0;
    mUnk_63 = 0;
    mUnk_64 = 0;
    mUnk_65 = 0;
    SpriteManager::Get();
    SpriteManager::sFade.mMain       = 1;
    SpriteManager::sBrightness.mMain = -16;
    SpriteManager::Get();
    SpriteManager::sFade.mSub       = 1;
    SpriteManager::sBrightness.mSub = -16;
}

BattleConfrontCard::~BattleConfrontCard() {
    u8 i = 0;
    do {
        CellAnimation *a = mUnk_24[i];
        if (a) {
            delete a;
        }
        i++;
    } while (i < 5);
    if (mData) {
        delete mData;
    }
    func_ov007_02243e7c();
    SoundManager::Get().func_02024688(1);
    SoundManager::Get().func_020245a4();
    if (!func_ov007_02263150(data_020d433c.mBattlePlayers)) {
        if (data_020d433c.mUnk_84) {
            SoundManager::Get().func_020245d4(1);
            func_020383f8(SoundManager::Get().mUnk_9c);
        } else {
            func_ov007_022633f8(data_020d433c.mBattlePlayers);
        }
    }
    SoundManager::Get().func_020246d4(6);
    SpriteManager::Get();
    SpriteManager::sFade.mMain       = 1;
    SpriteManager::sBrightness.mMain = -16;
    SpriteManager::Get();
    SpriteManager::sFade.mSub       = 1;
    SpriteManager::sBrightness.mSub = -16;
    SpriteManager::Get().func_0202740c();
}

void BattleConfrontCard::Update() {
    switch (mState) {
        case 0:
            FrameTask::Get().func_0203dd90();
            mState++;
            break;
        case 1:
            if (func_0203dd0c(&FrameTask::Get(), 0) == 0) {
                SoundManager::Get().func_020245a4();
                SoundManager::Get().func_020245d4(0);
                SoundManager::Get().func_020246d4(2);
                mState++;
            }
            break;
        case 2:
            func_ov007_0223f044();
            SpriteManager::Get().StartFade(2);
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
            SpriteManager::Get().StartFade(3);
            FrameTask::Get().func_0203dd74(1);
            mState++;
            break;
        case 6:
            if (SpriteManager::Get().IsFading()) {
                break;
            }
            if (func_0203dd0c(&FrameTask::Get(), 1) == 0) {
                func_ov007_0223f168();
                mState++;
            }
            break;
        case 7:
            if (func_ov007_022631bc(data_020d433c.mBattlePlayers, 0xf)) {
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
        if (mUnk_69) {
            if (mUnk_80) {
                mUnk_80->Draw();
            } else {
                mUnk_24[1]->Update();
                mUnk_24[1]->Draw(1, 0x80, 0x60, 0);
            }
            SpriteManager::Get().Screen(1).func_02027778(mUnk_7c, 9, 0x20, 0x28, 0, 0x1a0, 0xf, 0);
        }
    }
}

void BattleConfrontCard::func_ov007_0223f044() {
    BattleData *data = mData;
    func_ov007_0223f0c0();
    mData->func_ov007_0223d504();
    func_ov007_0223f398();
    u16 a   = data->mEntries[0].mUnk_0c | (data->mEntries[0].mUnk_0d_0 << 8);
    u16 b   = data->mEntries[1].mUnk_0c | (data->mEntries[1].mUnk_0d_0 << 8);
    mUnk_3c = new BattleCardCamera(a, b);
}

void BattleConfrontCard::func_ov007_0223f0c0() {
    SpriteManager::Get().func_02026dec(1, 7);
    reg_GX_POWCNT |= 0x8000;
    GX_SetGraphicsMode(1, 0, 1);
    GXS_SetGraphicsMode(3);
    func_ov007_0223f168();
}

void BattleConfrontCard::func_ov007_0223f168() {
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
    if (data_020d433c.mUnk_10) {
        data_020d433c.mUnk_10->mUnk_1a = 0;
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

// non-matching: 10 words, register assignment only (screen r7 / loop animation r8 / loop index r9 in the original; r8 / r7 / r8-r9 here).
void BattleConfrontCard::func_ov007_0223f398() {
    char buffer[0x80];
    CellAnimation *a;
    ChangeDir("/Game/Confrontation/Bg");
    func_02025b64(1, 1, 0, "vs_b_bg_00_ncl.bin", 0, 0, 0);
    func_02025b64(1, 0xb, 0, "vs_b_bg_00_ncg.bin", 0, 0, 0);
    func_02025b64(1, 7, 0, "vs_b_bg_00_nsc.bin", 0, 0, 0);
    ChangeDir("/");
    ChangeDir("/Game/Confrontation/Obj");
    SpriteScreen &screen = SpriteManager::Get().Screen(1);
    sprintf(buffer, "vs_b_obj_00.nce_ncg1D.bin");
    screen.LoadCharacters(buffer, 0x13000, 0);
    sprintf(buffer, "vs_b_obj_00_ncl.bin");
    screen.LoadPalette(buffer, 0xa0, 0);
    sprintf(buffer, "vs_b_obj_boost_ncl.bin");
    screen.LoadPalette(buffer, 0x160, 0);
    CellAnimation *old = mUnk_24[4];
    if (old) {
        delete old;
    }
    mUnk_24[4]            = 0;
    mUnk_24[4]            = new CellAnimation("vs_b_obj_00_nce.bin", 0, 1, 1);
    mUnk_24[4]->mCharBase = screen.func_020276f4(0x13000);
    mUnk_24[4]->mUnk_11   = 5;
    mUnk_24[4]->SetAnimation(0);
    func_02025b64(1, 5, 0x120, "cardspec_b_obj_00_ncl.bin", 0, 0, 0);
    func_02025b64(1, 4, 0x18000, "cardspec_b_obj_00.nce_ncg1D.bin", 0, 0, 0);
    u8 i = 2;
    do {
        a = mUnk_24[i];
        if (a) {
            delete a;
        }
        mUnk_24[i]            = 0;
        mUnk_24[i]            = new CellAnimation("cardspec_b_obj_00_nce.bin", 0, 1, 1);
        mUnk_24[i]->mCharBase = screen.func_020276f4(0x18000);
        mUnk_24[i]->mUnk_11   = 9;
        i++;
    } while (i < 4);
    mUnk_24[2]->SetAnimation(0);
    mUnk_24[3]->SetAnimation(1);
    ChangeDir("/Game/2D/union_b");
    func_02025b64(1, 4, 0xe000, "union_b_obj_page.nce_ncg1D.bin", 0, 0, 0);
    func_02025b64(1, 5, 0xe0, "union_b_obj_window_ncl.bin", 0, 0, 0);
    if (mUnk_24[0] == 0) {
        func_ov007_0223f780(0);
    } else {
        func_ov007_0223f780(mData->mUnk_04);
    }
    if (data_020d433c.mUnk_10) {
        data_020d433c.mUnk_10->func_020785d8(10);
    }
}

void BattleConfrontCard::func_ov007_0223f780(u16 card) {
    char buffer[0x80];
    u16 id = func_02065c20(card);
    SpriteScreen &screen = SpriteManager::Get().Screen(1);
    ChangeDir("/Game/Card/Graphic/Large/");
    sprintf(buffer, "card_l_%03d.nce_ncg1D.bin", id);
    screen.LoadCharacters(buffer, 0x10000, 0);
    sprintf(buffer, "card_l_%03d_ncl.bin", id);
    screen.func_02027fec(buffer, 0x400, 0);
    sprintf(buffer, "card_l_%03d_nce.bin", id);
    CellAnimation *old = mUnk_24[0];
    if (old) {
        delete old;
    }
    mUnk_24[0]            = 0;
    mUnk_24[0]            = new CellAnimation(buffer, 0, 1, 1);
    mUnk_24[0]->mCharBase = screen.func_020276f4(0x10000);
    mUnk_24[0]->mUnk_11   = 2;
    ChangeDir("/");
}

void BattleConfrontCard::func_ov007_0223f90c(u16 value) {
    mUnk_12 = value;
    mUnk_10 = value;
}

// non-matching: all 178 calls and the 54-case jump table match, but the size is 0x2c14 vs 0x2c04 and the register assignment differs (the original keeps `mData->mEntries` in r8, here it lands in r6, which shifts the temporaries of cases 15-35 and 51; case 44 loads the bit from a branch instead of predicated loads; case 35 computes i*0x14 once more).
BOOL BattleConfrontCard::func_ov007_0223f918() {
    BattleEntry *e = mData->mEntries;
    switch (mUnk_0d) {
        case 0:
            if (data_020d433c.mUnk_10) {
                data_020d433c.mUnk_10->mUnk_1a = 1;
            }
            GXS_SetVisibleWnd(5);
            G2S_SetWndOutsidePlane(0x1f, FALSE);
            G2S_SetWnd0InsidePlane(0x3f, FALSE);
            G2S_SetWnd0Position(0, 0, 0xff, 0xbf);
            func_ov007_0223f90c(0x100);
            if (data_020d433c.mBattlePlayers->mPlayers[e[0].mUnk_0d_4]->IsActive() == 1
                && data_020d433c.mBattlePlayers->mPlayers[e[1].mUnk_0d_4]->IsActive() == 1) {
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
                        func_ov007_02243f04("/font/mes_confrontation.mes", 0);
                    } else {
                        func_ov007_02243f04("/font/mes_BattleArenaMenu.mes", 4);
                    }
                } else {
                    func_ov007_02243f04("/font/mes_confrontation.mes", 0);
                }
                func_ov007_02243d40();
                if (mUnk_80) {
                    mUnk_80->mCursor->mSelection = -1;
                    mUnk_80->mUnk_061            = 4;
                    mUnk_80->func_0205c7c8(1);
                }
                mUnk_0d = 2;
            } else {
                mUnk_0d = 1;
            }
            break;
        case 1:
            mUnk_10--;
            if (mUnk_10 == 0) {
                func_ov007_022430d8(0);
                reg_G2S_DB_WIN0H = 0x4faf;
                reg_G2S_DB_WIN0V = 0x20a0;
                mUnk_14[2] = 0x4f;
                mUnk_1a[2] = 0x20;
                func_ov007_0223f90c(0x4f);
                mUnk_0d = 3;
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
                u8 result = mUnk_80->Update(1);
                SelectWindow *w = mUnk_80;
                if (w->mUnk_061 == 2 && w->mResult == 1 && w->mCursor->mSelection == 0) {
                    WirelessManager::Get();
                    if (IsWirelessMode(mode)) {
                        data_0220f540.mUnk_00     = 0x16;
                        data_0220f540.mTouchDown = 1;
                    } else {
                        func_ov007_02243dec();
                        func_ov007_02243e7c();
                        SpriteManager::Get().StartFade(3);
                        mUnk_0d = 0x31;
                    }
                    mUnk_68 = 0;
                } else if (result == 1) {
                    if (w->mCursor->mSelection != 0) {
                        WirelessManager::Get();
                        if (IsWirelessMode(mode)) {
                            data_0220f540.mUnk_00     = 0x16;
                            data_0220f540.mTouchDown = 0;
                        } else {
                            func_ov007_02243dec();
                            func_ov007_02243e7c();
                            mUnk_0d = 1;
                        }
                    }
                    mUnk_68 = 0;
                }
            }
            WirelessManager::Get();
            if (IsWirelessMode(mode)) {
                BattlePlayer *p = data_020d433c.mBattlePlayers->mPlayers[0];
                if (p->mMsg.mType == 0x16) {
                    func_ov007_02243dec();
                    func_ov007_02243e7c();
                    mUnk_68 = 0;
                    if (p->mMsg.mTouchDown) {
                        SpriteManager::Get().StartFade(3);
                        mUnk_0d = 0x31;
                    } else {
                        mUnk_0d = 1;
                    }
                }
            }
            break;
        }
        case 3:
            mUnk_10--;
            if (mUnk_10 != 0) {
                func_ov007_022430d8((u8) ((mUnk_10 << 4) / mUnk_12));
            } else {
                BattleCardCamera *cam = mUnk_3c;
                SpriteManager::Get();
                SpriteManager::sFade.mMain = 3;
                data_020d433c.mUnk_14 |= 4;
                cam->mState = 3;
                mUnk_0d++;
            }
            break;
        case 4:
            if (mContext->mUnk_12 != 0) {
                mUnk_0d++;
            }
            break;
        case 5:
            new BattleCardSpec(0, e[0].mUnk_0c, e[0].mUnk_0d_0, e[0].mUnk_0e == 0x15 ? 1 : 0);
            mContext->mUnk_12 = 0;
            func_ov007_0223f90c(0x40);
            mUnk_0d++;
            break;
        case 6:
            mUnk_14[0] = (s16) ((-0x60 * (mUnk_10 * mUnk_10)) / (mUnk_12 * mUnk_12)) + 8;
            if (mUnk_10 != 0) {
                mUnk_10--;
                break;
            }
            if (mContext->mUnk_12 != 0) {
                mUnk_0d++;
            }
            break;
        case 7:
            new BattleCardSpec(0, e[1].mUnk_0c, e[1].mUnk_0d_0, e[1].mUnk_0e == 0x15 ? 1 : 0);
            mContext->mUnk_12 = 0;
            func_ov007_0223f90c(0x40);
            mUnk_0d++;
            break;
        case 8:
            mUnk_14[1] = 0xb7 - (s16) ((-0x60 * (mUnk_10 * mUnk_10)) / (mUnk_12 * mUnk_12));
            if (mUnk_10 != 0) {
                mUnk_10--;
                break;
            }
            if (mContext->mUnk_12 != 0) {
                mUnk_0d++;
            }
            break;
        case 9:
            new BattleCharaAct(data_020d433c.mBattlePlayers->mCamera, e[0].mUnk_0d_4, 5, 0, 0);
            mContext->mUnk_12 = 0;
            func_ov007_0223f90c(0x40);
            mUnk_0d++;
            break;
        case 10:
            mUnk_10--;
            if (mUnk_10 == 0) {
                func_ov007_0223f90c(0x20);
                func_ov007_0223f780(mData->mUnk_04);
                func_ov007_022430bc(0x10);
                mUnk_0d++;
                mUnk_5f = 1;
            }
            break;
        case 11:
            func_ov007_022430bc((s8) ((mUnk_10 << 4) / mUnk_12));
            if (mUnk_10 != 0) {
                mUnk_10--;
                break;
            }
            func_ov007_0223f90c(0x10);
            mUnk_0d++;
            break;
        case 12:
            if (mContext->mUnk_12 == 0) {
                break;
            }
            if (data_020d433c.mUnk_10) {
                data_020d433c.mUnk_10->func_020785d8(10);
            }
            if (func_ov007_022631bc(data_020d433c.mBattlePlayers, 0xf)) {
                mUnk_0d++;
            }
            break;
        case 13:
            mUnk_40 = new BattleCommandPage(mData->mUnk_04, -1);
            mUnk_0d++;
            mUnk_10 = 0;
            break;
        case 14:
            mUnk_10++;
            if (mUnk_10 > 0xf0) {
                func_02055624(mUnk_40, 1);
                mContext->mUnk_12 = 0;
                mUnk_0d++;
                mUnk_10 = 0;
            }
            break;
        case 15: {
            if (mContext->mUnk_12 == 0) {
                break;
            }
            SpriteManager::Get().func_02026dec(0, 7);
            GX_SetGraphicsMode(1, 0, 1);
            mUnk_50 = func_ov007_022428d4(mData->mUnk_04);
            u8 side = data_020d433c.mBattlePlayers->mUnk_2a9 == e[1].mUnk_0d_4 ? 1 : 0;
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
            mUnk_44 = new BattleCardSpec(kind, e[side].mUnk_0c, e[side].mUnk_0d_0, e[side].mUnk_0e == 0x15 ? 1 : 0);
            mContext->mUnk_12 = 0;
            func_ov007_0223f90c(0x40);
            mUnk_0d++;
            break;
        }
        case 16:
            func_ov007_022432dc(0, 0, 0);
            mUnk_10--;
            if (mUnk_10 == 0) {
                mUnk_0d++;
            }
            break;
        case 17:
            if (mData->func_ov007_0223dd6c()) {
                func_ov007_0223f90c(0x40);
                if (mUnk_50 == 2 || mUnk_50 == 3 || mUnk_50 == 5) {
                    BattleCardSpec *spec = mUnk_44;
                    spec->mUnk_1e = 0;
                    func_ov007_02273e3c(spec->mUnk_0c);
                    mContext->mUnk_12 = 0;
                    mUnk_0d           = 0x14;
                } else if (mData->mUnk_04 == 0x55) {
                    BattleCardSpec *spec = mUnk_44;
                    spec->mUnk_1e = 0;
                    func_ov007_02273e3c(spec->mUnk_0c);
                    mContext->mUnk_12 = 0;
                    mUnk_0d           = 0x14;
                } else {
                    mData->mEntries[0].mUnk_02 = e[0].mUnk_00 + e[0].mUnk_08;
                    mData->mEntries[1].mUnk_02 = e[1].mUnk_00 + e[1].mUnk_08;
                    mUnk_0d++;
                }
            }
            func_ov007_022432dc(0, 1, 0);
            break;
        case 18:
            func_ov007_022432dc(1, 0, 1);
            mUnk_10--;
            if (mUnk_10 == 0) {
                mUnk_0d++;
            }
            break;
        case 19:
            if (mData->func_ov007_0223dd6c()) {
                func_ov007_0223f90c(0x40);
                mUnk_0d = 0x19;
            }
            func_ov007_022432dc(1, 1, 1);
            break;
        case 20: {
            if (mContext->mUnk_12 == 0) {
                break;
            }
            u8 side;
            if (data_020d433c.mBattlePlayers->mUnk_2a9 == e[1].mUnk_0d_4) {
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
            if (mData->mUnk_04 == 0x55) {
                make = 1;
                kind = 10;
            }
            if (make) {
                mUnk_44 = new BattleCardSpec(kind, e[side].mUnk_0c, e[side].mUnk_0d_0, e[side].mUnk_0e == 0x15 ? 1 : 0);
            }
            mUnk_0d++;
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
            mUnk_0d++;
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
            mUnk_0d = 0x19;
            if (data_020d433c.mBattlePlayers->mUnk_2a9 == e[1].mUnk_0d_4) {
                if (mUnk_5d == 0) {
                    BattleCardSpec *spec = mUnk_44;
                    spec->mUnk_1e = 0;
                    func_ov007_02273e3c(spec->mUnk_0c);
                    mContext->mUnk_12 = 0;
                    mUnk_0d           = 0x17;
                }
            } else {
                if (mUnk_5c == 0) {
                    BattleCardSpec *spec = mUnk_44;
                    spec->mUnk_1e = 0;
                    func_ov007_02273e3c(spec->mUnk_0c);
                    mContext->mUnk_12 = 0;
                    mUnk_0d           = 0x17;
                }
            }
            break;
        }
        case 23: {
            if (mContext->mUnk_12 == 0) {
                break;
            }
            u8 side = data_020d433c.mBattlePlayers->mUnk_2a9 == e[1].mUnk_0d_4 ? 1 : 0;
            mUnk_44 = new BattleCardSpec(3, e[side].mUnk_0c, e[side].mUnk_0d_0, e[side].mUnk_0e == 0x15 ? 1 : 0);
            func_ov007_0223f90c(0x40);
            mUnk_0d++;
            break;
        }
        case 24:
            mUnk_0d++;
            break;
        case 25:
            if (mUnk_10 != 0) {
                mUnk_10--;
                break;
            }
            mUnk_54 = 0;
            if (data_020d433c.mUnk_83 == 2 || data_020d433c.mUnk_83 == 4) {
                mUnk_0d = 0x1a;
            } else {
                mUnk_0d = 0x1c;
            }
            break;
        case 26:
            if (data_020d433c.mUnk_10) {
                data_020d433c.mUnk_10->mUnk_1a = 0;
            }
            GXS_SetVisibleWnd(0);
            data_020d433c.mBattlePlayers->mUnk_2a4->func_02087eec();
            {
                BattlePlayerList *list = data_020d433c.mBattlePlayers;
                list->mUnk_2a4->func_02088e64(list->mUnk_2af, 0x64, 0, 0);
            }
            G2S_SetBlendBrightness(0x1f, -10);
            mUnk_0d++;
            break;
        case 27:
            if (InputManager::Get().IsHeld(1) || TouchManager::Get().mTouching) {
                data_020d433c.mBattlePlayers->mUnk_2a4->func_02088dac(1);
            }
            if (data_020d433c.mBattlePlayers->mUnk_2a4->func_02087cac()) {
                break;
            }
            data_020d433c.mBattlePlayers->mUnk_2af++;
            G2S_SetBlendBrightness(0x1f, 0);
            mUnk_0d = 0x1c;
            break;
        case 28: {
            mUnk_62 = 0;
            mUnk_63 = 0;
            mUnk_64 = 0;
            mUnk_65 = 0;
            mUnk_5e = 0;
            mUnk_4c = 0;
            if (data_020d433c.mUnk_9a_2) {
                mUnk_0d = 0x20;
                break;
            }
            u8 owner = data_020d433c.mBattlePlayers->mUnk_2a9;
            if (owner == e[0].mUnk_0d_4) {
                mUnk_0d = 0x1d;
                mUnk_52 = 0;
            } else if (owner == e[1].mUnk_0d_4) {
                mUnk_0d = 0x1d;
                mUnk_52 = 1;
            } else {
                mUnk_0d = 0x20;
                mUnk_4c = new BattleUnionAbility(4, 0x1a000, 0xc);
            }
            break;
        }
        case 29: {
            u8 *bits = &mUnk_60;
            u8 j     = 0;
            u8 bit   = 0;
            u8 count = 0;
            mUnk_60  = 0;
            do {
                BattlePlayerSlot *slots = data_020d433c.mBattlePlayers->mPlayers[data_020d433c.mBattlePlayers->mUnk_2a9]->mSlots6c;
                u8 flag                 = slots[j].mFlag;
                u16 id                  = slots[j].mValue;
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
                mUnk_0d = 0x1e;
                if (count == 1) {
                    mUnk_66 = 1;
                }
                mUnk_51 = 0;
            } else {
                mUnk_4c = new BattleUnionAbility(4, 0x1a000, 0xc);
                mUnk_0d = 0x1f;
            }
            break;
        }
        case 30:
            if (e[mUnk_52].mUnk_0f_0) {
                mUnk_4c = new BattleUnionAbility(4, 0x1a000, 0xc);
                mUnk_54 = 0;
                mUnk_0d = 0x20;
                break;
            }
            switch (func_ov007_0224251c()) {
                case 1:
                    if (!data_020d433c.mUnk_9a_2) {
                        mUnk_5e = 1;
                    }
                    mUnk_51 = 0;
                    mUnk_0d = 0x20;
                    mUnk_4c = new BattleUnionAbility(4, 0x1a000, 0xc);
                    break;
                case 2:
                    mUnk_54 = 0;
                    mUnk_0d = 0x20;
                    mUnk_4c = new BattleUnionAbility(4, 0x1a000, 0xc);
                    break;
            }
            break;
        case 31:
            mUnk_54 = 0;
            mUnk_0d = 0x20;
            break;
        case 32:
            if (func_ov007_022631bc(data_020d433c.mBattlePlayers, 0xf)) {
                mUnk_0d++;
                if (mUnk_4c) {
                    ((BattleUnionAbility *) mUnk_4c)->func_ov007_02234418(0);
                }
            }
            break;
        case 33: {
            WirelessManager::Get();
            if (IsWirelessMode(&data_020c4258)) {
                u8 owner = data_020d433c.mBattlePlayers->mUnk_2a9;
                if (e[0].mUnk_0d_4 == owner || e[1].mUnk_0d_4 == owner) {
                    data_0220f540.mUnk_00 = 7;
                    data_0220f540.mUnk_04 = mUnk_54;
                    *((u8 *) &data_0220f540 + 0xa) = mUnk_53;
                }
            }
            u8 i    = 0;
            u8 done = 0;
            do {
                if ((&mUnk_62)[i] == 0) {
                    BattleEntry *ent = &e[i];
                    if (data_020d433c.mBattlePlayers->mPlayers[ent->mUnk_0d_4]->IsActive()) {
                        (&mUnk_62)[i] = 1;
                    } else if (data_020d433c.mUnk_9a_2) {
                        (&mUnk_62)[i] = 1;
                    } else {
                        WirelessManager::Get();
                        if (IsWirelessMode(&data_020c4258)) {
                            BattlePlayer *p = data_020d433c.mBattlePlayers->mPlayers[ent->mUnk_0d_4];
                            if (p->mMsg.mType == 7) {
                                mData->mEntries[i].mUnk_12 = *(u32 *) ((u8 *) p + 0x104);
                                mData->mEntries[i].mUnk_11 = *((u8 *) p + 0x10a);
                                mData->mEntries[i].mUnk_0a = func_ov007_02243b24(i, (u16) *(u32 *) ((u8 *) p + 0x104));
                                (&mUnk_62)[i]              = 1;
                            }
                        } else {
                            mData->mEntries[i].mUnk_12 = mUnk_54;
                            mData->mEntries[i].mUnk_11 = mUnk_53;
                            mData->mEntries[i].mUnk_0a = func_ov007_02243b24(i, mUnk_54);
                            (&mUnk_62)[i]              = 1;
                        }
                    }
                } else {
                    done++;
                }
                i++;
            } while (i < 2);
            if (done == 2) {
                mUnk_0d = 0x22;
            }
            break;
        }
        case 34:
            if (func_ov007_022631bc(data_020d433c.mBattlePlayers, 0xf)) {
                mUnk_0d++;
            }
            break;
        case 35: {
            u8 i = 0;
            do {
                
                if (data_020d433c.mBattlePlayers->mPlayers[e[i].mUnk_0d_4]->IsActive()) {
                    if (e[i].mUnk_0f_0) {
                        mUnk_56 = 0;
                    } else {
                        u8 j = 0;
                        u8 k = 0;
                        u8 mask = 0;
                        do {
                            if (!func_ov007_022433c8(i, data_020d433c.mBattlePlayers->mPlayers[e[i].mUnk_0d_4]->mSlots6c[j].mValue)) {
                                (&mask)[k / 8] |= 1 << (k % 8);
                            }
                            j++;
                            k++;
                        } while (j < data_020d433c.mUnk_99);
                        u8 other = (i + 1) % 2;
                        u8 pick  = func_ov007_0226a700(data_020d433c.mBattlePlayers->mPlayers[e[i].mUnk_0d_4], mask,
                                                       (s16) (e[other].mUnk_04 - e[i].mUnk_04), (s16) (e[other].mUnk_00 - e[i].mUnk_00));
                        if (pick == 0xff) {
                            mUnk_56 = 0;
                        } else {
                            BattlePlayer *p = data_020d433c.mBattlePlayers->mPlayers[e[i].mUnk_0d_4];
                            mData->mEntries[i].mUnk_12 = p->mSlots6c[pick].mValue;
                            mData->mEntries[i].mUnk_11 = pick;
                            mData->mEntries[i].mUnk_0a = func_ov007_02243b24(i, data_020d433c.mBattlePlayers->mPlayers[e[i].mUnk_0d_4]->mSlots6c[pick].mValue);
                        }
                    }
                } else if (data_020d433c.mUnk_9a_2) {
                    if (e[i].mUnk_0f_0) {
                        mUnk_54 = 0;
                    } else {
                        u8 j = 0;
                        u8 k = 0;
                        u8 mask = 0;
                        do {
                            if (!func_ov007_022433c8(i, data_020d433c.mBattlePlayers->mPlayers[e[i].mUnk_0d_4]->mSlots6c[j].mValue)) {
                                (&mask)[k / 8] |= 1 << (k % 8);
                            }
                            j++;
                            k++;
                        } while (j < data_020d433c.mUnk_99);
                        u8 other = (i + 1) % 2;
                        u8 pick  = func_ov007_0226a700(data_020d433c.mBattlePlayers->mPlayers[e[i].mUnk_0d_4], mask,
                                                       (s16) (e[other].mUnk_04 - e[i].mUnk_04), (s16) (e[other].mUnk_00 - e[i].mUnk_00));
                        if (pick == 0xff) {
                            mUnk_54 = 0;
                        } else {
                            mUnk_53 = pick;
                            BattlePlayer *p = data_020d433c.mBattlePlayers->mPlayers[e[i].mUnk_0d_4];
                            mUnk_54 = p->mSlots6c[pick].mValue;
                            mData->mEntries[i].mUnk_12 = p->mSlots6c[pick].mValue;
                            mData->mEntries[i].mUnk_11 = mUnk_53;
                            mData->mEntries[i].mUnk_0a = func_ov007_02243b24(i, p->mSlots6c[pick].mValue);
                        }
                    }
                }
                i++;
            } while (i < 2);
            mUnk_0d = 0x24;
            break;
        }
        case 36:
            if (mUnk_5e) {
                func_02055624(mUnk_40, 1);
            } else {
                BattleCardSpec *spec = mUnk_44;
                spec->mUnk_1e = 0;
                func_ov007_02273e3c(spec->mUnk_0c);
            }
            mContext->mUnk_12 = 0;
            mUnk_0d           = 0x25;
            break;
        case 37:
            if (mContext->mUnk_12 == 0) {
                break;
            }
            if (mUnk_5e) {
                SpriteManager::Get().func_02026dec(0, 7);
                GX_SetGraphicsMode(1, 0, 1);
            }
            mUnk_0d++;
            break;
        case 38: {
            if (!func_ov007_022631bc(data_020d433c.mBattlePlayers, 0xf)) {
                break;
            }
            u8 i = 0;
            do {
                if (e[i].mUnk_12 >= 0x92 && e[i].mUnk_12 <= 0xa5) {
                    u8 o = i ^ 1;
                    if (e[o].mUnk_12 != 0x8c && e[o].mUnk_12 != 0x8d) {
                        mData->mEntries[i].mUnk_0f_4 = 1;
                    }
                }
                i++;
            } while (i < 2);
            mUnk_0d = 0x27;
            SpriteManager::Get().StartFade(3);
            break;
        }
        case 39: {
            if (SpriteManager::Get().IsFading()) {
                break;
            }
            mContext->mUnk_12 = 0;
            func_ov007_0223f168();
            MI_CpuClearFast((void *) 0x6200000, 0x20000);
            MI_CpuClearFast((void *) 0x6607000, 0x19000);
            u8 kind = mData->mUnk_07;
            mData->func_ov007_0223e1a4(0, 0xff);
            switch (kind) {
                case 1:
                    mUnk_20 = 0;
                    break;
                case 2:
                    mUnk_20 = 3;
                    break;
                case 3:
                    mUnk_20 = 1;
                    break;
                case 4:
                    mUnk_20 = 2;
                    break;
                case 5:
                    mUnk_20 = 4;
                    break;
                case 6:
                    mUnk_20 = 5;
                    break;
                case 8:
                    mData->func_ov007_0223e180();
                    break;
            }
            switch (mUnk_20) {
                case 0:
                    mUnk_48 = new ConfrontBg01();
                    break;
                case 1:
                    mUnk_48 = new ConfrontBg02();
                    break;
                case 2:
                    mUnk_48 = new Confront03();
                    break;
                case 3:
                    mUnk_48 = new ConfrontBg04();
                    break;
                case 4:
                    mUnk_48 = new ConfrontBg05();
                    break;
                case 5:
                    mUnk_48 = new ConfrontBg06();
                    break;
            }
            ((BattleBase *) mUnk_48)->BattleBase::func_ov007_02219888(mData, mUnk_20);
            mUnk_22 = 1;
            mUnk_0d++;
            break;
        }
        case 40:
            if (mContext->mUnk_12 != 0) {
                mUnk_0d++;
            }
            break;
        case 41:
            if (mData->mUnk_07 == 0) {
                func_ov007_0223f168();
                MI_CpuClearFast((void *) 0x6200000, 0x20000);
                func_ov007_0223f398();
                SpriteManager::Get().StartFade(2);
                mUnk_22 = 0;
                mUnk_0d++;
            } else {
                mUnk_0d = 0x27;
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
            if (data_020d433c.mUnk_10) {
                data_020d433c.mUnk_10->mUnk_1a = 1;
            }
            GXS_SetVisibleWnd(5);
            G2S_SetWndOutsidePlane(0x1f, FALSE);
            G2S_SetWnd0InsidePlane(0x3f, FALSE);
            mUnk_0d++;
            break;
        case 43:
            if (func_ov007_022631bc(data_020d433c.mBattlePlayers, 0xf)) {
                mUnk_0d++;
            }
            break;
        case 44: {
            u32 kind;
            if (mUnk_21 == 0 ? e[0].mUnk_0f_4 : e[1].mUnk_0f_4) {
                kind = 4;
            } else {
                kind = 1;
            }
            new BattleCardSpec(kind, e[mUnk_21].mUnk_0c, e[mUnk_21].mUnk_0d_0, e[mUnk_21].mUnk_0e == 0x15 ? 1 : 0);
            mContext->mUnk_12 = 0;
            u8 x;
            if (mUnk_21 == 0) {
                x = 8;
            } else if (mUnk_21 == 1) {
                x = 0xb7;
            }
            reg_G2S_DB_WIN0H = (u16) (((x << 8) & 0xff00) | ((x + 0x40) & 0xff));
            reg_G2S_DB_WIN0V = 0x3c9c;
            func_ov007_022430bc(0);
            mUnk_10 = 0;
            mUnk_0d++;
            break;
        }
        case 45: {
            if (mContext->mUnk_12 != 0) {
                func_ov007_022430bc(0);
                mUnk_0d++;
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
            func_ov007_022430bc((s8) t);
            break;
        }
        case 46:
            if (mUnk_21 == 0) {
                mUnk_0e = 1;
            } else {
                mUnk_0e = 0;
            }
            new BattleCardSpec(2, e[mUnk_0e].mUnk_0c, e[mUnk_0e].mUnk_0d_0, e[mUnk_0e].mUnk_0e == 0x15 ? 1 : 0);
            mContext->mUnk_12 = 0;
            mUnk_0f           = mUnk_21 == 0 ? 0xb7 : 8;
            mUnk_10           = 0;
            mUnk_0d++;
            break;
        case 47:
            if (mContext->mUnk_12 != 0) {
                mUnk_0d++;
                SpriteManager::Get().StartFade(3);
                break;
            }
            mUnk_10++;
            if ((mUnk_10 > 4) & (mUnk_10 < 0x40)) {
                mUnk_14[mUnk_0e] = mUnk_0f + MATH_Rand32(&data_020d42e8, 0x20) - 0x10;
                mUnk_1a[mUnk_0e] = MATH_Rand32(&data_020d42e8, 0x18) + 0x30;
            } else {
                mUnk_14[mUnk_0e] = mUnk_0f;
                mUnk_1a[mUnk_0e] = 0x3c;
            }
            break;
        case 48:
            if (SpriteManager::Get().IsFading()) {
                break;
            }
            mUnk_0d = 0x33;
            func_ov007_0223f168();
            break;
        case 49: {
            if (SpriteManager::Get().IsFading()) {
                break;
            }
            BattleCardCamera *cam = mUnk_3c;
            SpriteManager::Get();
            SpriteManager::sFade.mMain = 3;
            data_020d433c.mUnk_14 |= 4;
            cam->mState = 3;
            mContext->mUnk_12 = 0;
            mUnk_0d++;
            break;
        }
        case 50:
            if (mContext->mUnk_12 == 0) {
                break;
            }
            if (func_ov007_022631bc(data_020d433c.mBattlePlayers, 0xf)) {
                mUnk_21 = func_ov007_02244440();
                mUnk_0d++;
            }
            break;
        case 51: {
            if (mUnk_21 != -1) {
                u8 found = 0;
                u8 idx   = 0;
                u8 i     = 0;
                u8 owner = data_020d433c.mBattlePlayers->mUnk_2a9;
                do {
                    if (owner == e[i].mUnk_0d_4) {
                        idx   = i;
                        found = 1;
                    }
                    i++;
                } while (i < 2);
                if (found == 1) {
                    if (idx == mUnk_21) {
                        u32 v = func_ov007_022696a0(e[mUnk_21].mUnk_0c, e[mUnk_21].mUnk_0d_0);
                        new BattleCharaAct(data_020d433c.mBattlePlayers->mCamera, e[mUnk_21].mUnk_0d_4, 10, v, 0);
                        if (data_020d433c.mUnk_7a) {
                            s16 d = (s16) (e[(mUnk_21 + 1) % 2].mUnk_04 - e[mUnk_21].mUnk_04);
                            if (d < 0) {
                                d = 0;
                            }
                            u16 add = d + 0x50;
                            if (e[mUnk_21].mUnk_0f_6) {
                                add = (u16) (add << 1);
                            }
                            if (data_020d433c.mUnk_83 == 0) {
                                u8 sel = data_020d433c.mBattlePlayers->mUnk_28d;
                                if (mUnk_21 == 1) {
                                    sel = data_020d433c.mBattlePlayers->mUnk_28e;
                                }
                                BattlePlayer *p = data_020d433c.mBattlePlayers->mPlayers[e[mUnk_21].mUnk_0d_4];
                                BattleRecord *rec = &p->mRecords[data_020d433c.mBattlePlayers->mUnits[sel].mUnk_0e];
                                if (0xfff - add > rec->mGauge) {
                                    rec->mGauge = rec->mGauge + add;
                                }
                                if (data_020d48f0.mUnk_d4 < 9999) {
                                    data_020d48f0.mUnk_d4++;
                                }
                            }
                        }
                        if (data_020d433c.mUnk_83 == 2) {
                            data_020d433c.mBattlePlayers->mUnk_2b0 = 1;
                        }
                    } else {
                        new BattleCharaAct(data_020d433c.mBattlePlayers->mCamera, e[idx].mUnk_0d_4, 0xb, 0, 0);
                    }
                    if (data_020d433c.mUnk_83 == 4 && e[idx].mUnk_0f_1) {
                        data_020d433c.mBattlePlayers->mUnk_2b0 = 1;
                    }
                } else {
                    u32 v = func_ov007_022696a0(e[mUnk_21].mUnk_0c, e[mUnk_21].mUnk_0d_0);
                    new BattleCharaAct(data_020d433c.mBattlePlayers->mCamera, e[mUnk_21].mUnk_0d_4, e[mUnk_21].mUnk_0d_4 + 0x10, v, 0);
                }
                mContext->mUnk_12 = 0;
            } else {
                mContext->mUnk_12 = 1;
            }
            mUnk_0d++;
            break;
        }
        case 52: {
            if (mContext->mUnk_12 == 0) {
                break;
            }
            if (mUnk_21 != -1) {
                BattlePlayer *p = data_020d433c.mBattlePlayers->mPlayers[e[mUnk_21].mUnk_0d_4];
                u8 other        = 0;
                p->mUnk_fe      = 1;
                p->mUnk_ee++;
                if (mUnk_21 == 0) {
                    other = 1;
                }
                BattlePlayer *q = data_020d433c.mBattlePlayers->mPlayers[e[mUnk_21].mUnk_0d_4];
                *(u16 *) ((u8 *) q + q->mUnk_f4 * 6 + 0x84) = e[other].mUnk_04;
                *((u8 *) q + q->mUnk_f4 * 6 + 0x86) = e[other].mUnk_0d_0;
                q->mUnk_f4++;
            }
            func_ov007_022626b8(data_020d433c.mBattlePlayers, data_020d433c.mBattlePlayers->mUnits[data_020d433c.mBattlePlayers->mUnk_28d].mUnk_11);
            func_ov007_02262828(data_020d433c.mBattlePlayers, data_020d433c.mBattlePlayers->mUnk_28d, 2);
            func_ov007_02262828(data_020d433c.mBattlePlayers, data_020d433c.mBattlePlayers->mUnk_28e, 2);
            mUnk_0d = 0x35;
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
            if (data_020d433c.mUnk_83 == 4) {
                mContext->mUnk_12 = 0;
                mUnk_51           = 2;
                BattleCardSpec *spec = mUnk_44;
                spec->mUnk_1e        = 0;
                func_ov007_02273e3c(spec->mUnk_0c);
            } else {
                new BattleUnionAbility(0, 0x1a000, 0xc);
                mContext->mUnk_12 = 0;
                mUnk_51++;
            }
            break;
        case 1: {
            u8 v = mContext->mUnk_12;
            if (v == 0) {
                break;
            }
            mContext->mUnk_12 = 0;
            switch (v) {
                case 1: {
                    mUnk_51              = 2;
                    BattleCardSpec *spec = mUnk_44;
                    spec->mUnk_1e        = 0;
                    func_ov007_02273e3c(spec->mUnk_0c);
                    break;
                }
                case 2:
                    result = 2;
                    break;
            }
            break;
        }
        case 2:
            if (mContext->mUnk_12 == 0) {
                break;
            }
            mUnk_40 = new BattleCommandPage(0, -1);
            func_0205562c(mUnk_40, 1);
            func_02055738(mUnk_40, 0xe000, 7, 0);
            new BattleCommandPrompt(3, mUnk_40, mUnk_60, 0x7000, 0);
            mContext->mUnk_12 = 0;
            mUnk_51++;
            break;
        case 3: {
            u16 v = mContext->mUnk_12;
            if (v == 0) {
                break;
            }
            if (v != 10) {
                GXS_SetVisiblePlane(0x11);
                mUnk_53 = func_ov007_0225f0bc(v);
                result  = 1;
                mUnk_54 = data_020d433c.mBattlePlayers->mPlayers[e[mUnk_52].mUnk_0d_4]->Slot6c(mUnk_53);
            } else {
                func_02055624(mUnk_40, 1);
                mContext->mUnk_12 = 0;
                mUnk_51           = 6;
            }
            break;
        }
        case 4:
            new BattleUnionAbility(1, 0x1a000, 0xc);
            mContext->mUnk_12 = 0;
            mUnk_51++;
            break;
        case 5: {
            u8 v = mContext->mUnk_12;
            if (v == 0) {
                break;
            }
            switch (v) {
                case 1:
                    result = 1;
                    break;
                case 2:
                    mUnk_51 = 6;
                    func_02055624(mUnk_40, 1);
                    mContext->mUnk_12 = 0;
                    break;
            }
            break;
        }
        case 6: {
            if (mContext->mUnk_12 == 0) {
                break;
            }
            SpriteManager::Get().func_02026dec(0, 7);
            GX_SetGraphicsMode(1, 0, 1);
            mUnk_44 = new BattleCardSpec(3, e[mUnk_52].mUnk_0c, e[mUnk_52].mUnk_0d_0, e[mUnk_52].mUnk_0e == 0x15 ? 1 : 0);
            mContext->mUnk_12 = 0;
            mUnk_51           = 0;
            break;
        }
    }
    return result;
}

// non-matching: 0x7c8 vs 0x7e8; the original keeps side*20 / other*20 as offsets from e and re-adds them in every
// case (`ldrh r0, [r6, r7]`, `add r0, r6, r8`), where this CSEs e+side*20 / e+other*20 into pointer registers.
u8 BattleConfrontCard::func_ov007_022428d4(u16 card) {
    u8 result = 0;
    u8 type;
    u16 power;
    mUnk_5c   = 0;
    mUnk_5d   = 0;
    mUnk_61   = 0;
    BattleEntry *e = mData->mEntries;
    u8 i           = 0;
    do {
        u8 side  = 0;
        u8 other = 1;
        if (i == 1) {
            side  = 1;
            other = 0;
        }
        BattleEntry *self = &e[side];
        u8 selfLevel      = data_020d433c.mBattlePlayers->mPlayers[self->mUnk_0d_4]->mUnk_ee;
        u8 oppLevel       = data_020d433c.mBattlePlayers->mPlayers[e[other].mUnk_0d_4]->mUnk_ee;
        type              = func_02065c0c(card, 3);
        switch (type) {
        case 1: {
            u8 want = card - 1;
            if (card >= 7) {
                want++;
            } else if (card == 6) {
                if (e[i].mUnk_0c == 6) {
                    want = 6;
                }
            }
            if (want == e[i].mUnk_0c) {
                power     = func_02065bf4(card, e[i].mUnk_0d_0) * 10;
                u16 alt   = 0;
                if (e[i].mUnk_0c == 5) {
                    alt = func_02065bf4(card, 3) * 10;
                } else if (e[i].mUnk_0c == 6) {
                    alt = func_02065bf4(card, 0) * 10;
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
                u8 j = 0;
                do {
                    func_ov007_0226a448(data_020d433c.mBattlePlayers->mPlayers[e[side].mUnk_0d_4], j, 0);
                    j++;
                } while (j < data_020d433c.mUnk_99);
                (&mUnk_5c)[i] = 1;
                break;
            }
            case 0x4e:
                if (selfLevel < oppLevel) {
                    u8 j = 0;
                    do {
                        func_ov007_0226a448(data_020d433c.mBattlePlayers->mPlayers[e[side].mUnk_0d_4], j, 0);
                        j++;
                    } while (j < data_020d433c.mUnk_99);
                    (&mUnk_5c)[side] = 1;
                }
                break;
            case 0x4f:
            case 0x50:
            case 0x51:
                if (selfLevel < oppLevel) {
                    u8 j = 0;
                    do {
                        if (j == (u8) (card - 0x4f)) {
                            func_ov007_0226a448(data_020d433c.mBattlePlayers->mPlayers[e[side].mUnk_0d_4], j, 0);
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
                    u8 j = 0;
                    do {
                        if (j == (u8) (card - 0x52)) {
                            func_ov007_0226a448(data_020d433c.mBattlePlayers->mPlayers[e[side].mUnk_0d_4], j, 0);
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
                if (e[i].mUnk_0d_0 == 0 || e[i].mUnk_0c == 6) {
                    mUnk_5c = 1;
                    mUnk_5d = 1;
                }
                break;
            case 0x60:
                if (e[i].mUnk_0d_0 == 3 || e[i].mUnk_0c == 5) {
                    mUnk_5c = 1;
                    mUnk_5d = 1;
                }
                break;
            case 0x61:
                if (e[i].mUnk_0d_0 == 2) {
                    mUnk_5c = 1;
                    mUnk_5d = 1;
                }
                break;
            case 0x63:
                if (e[i].mUnk_0d_0 == 5) {
                    mUnk_5c = 1;
                    mUnk_5d = 1;
                }
                break;
            case 0x64:
                if (e[i].mUnk_0d_0 == 4) {
                    mUnk_5c = 1;
                    mUnk_5d = 1;
                }
                break;
            case 0x65:
                if (e[i].mUnk_0d_0 == 1) {
                    mUnk_5c = 1;
                    mUnk_5d = 1;
                }
                break;
            case 0x62: {
                BattlePlayer *p = data_020d433c.mBattlePlayers->mPlayers[self->mUnk_0d_4];
                u8 j            = 0;
                u8 n            = 0;
                do {
                    if (p->mSlots6c[j].mFlag == 2) {
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
                BattlePlayer *p = data_020d433c.mBattlePlayers->mPlayers[self->mUnk_0d_4];
                u8 j            = 0;
                u8 n            = 0;
                do {
                    if (p->mRecords[j].mUnk_0a == 1) {
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

void BattleConfrontCard::func_ov007_022430bc(int brightness) {
    G2S_SetBlendBrightness(0x1f, brightness);
}

void BattleConfrontCard::func_ov007_022430d8(int blend) {
    G2S_SetBlendAlpha(0x10, 0x2f, 0x10 - blend, blend);
}

// non-matching: register assignment, and the original fuses the call result test (`movs r6, r0`) into the variable.
void BattleConfrontCard::func_ov007_02243100() {
    u8 i = 0;
    do {
        mData->func_ov007_0223de88(i, mUnk_14[i], mUnk_1a[i], 2, 0xffff);
        i++;
    } while (i < 2);
    mData->func_ov007_0223db20(0, mUnk_14[0], (s16) (mUnk_1a[0] - 0x1e));
    mData->func_ov007_0223db20(1, mUnk_14[1], (s16) (mUnk_1a[1] - 0x1e));
    mData->mUnk_70->func_020654a0();
    mUnk_24[0]->Draw(1, mUnk_14[2], mUnk_1a[2], 3);
    if (mUnk_5f == 0) {
        return;
    }
    u8 k = 0;
    do {
        u32 value;
        u8 digits;
        int margin;
        s16 y;
        u8 j;
        value = func_02065bf4(mData->mUnk_04, k);
        if (value != 0) {
            digits = 3;
            if (value < 10) {
                digits--;
            }
            value  = (u16) (value * 10);
            margin = (3 - digits) * 4;
            y      = k * 17 + 0x28;
            j      = 0;
            do {
                mUnk_24[3]->func_0202a3ec(1, (s32) value % 10, (s16) ((digits - j) * 4 + 0x50 + margin / 2), y, 2);
                value /= 10;
                j++;
            } while (j < digits);
        }
        mUnk_24[2]->func_0202a3ec(1, k, 0x53, (s16) (k * 17 + 0x24), 2);
        k++;
    } while (k < 6);
}

void BattleConfrontCard::func_ov007_022432dc(u8 kind, u32 mode, u32 flag) {
    if (flag == 0) {
        mData->mUnk_34[4]->mUnk_11 = data_ov007_02279d3c[0];
        mData->mUnk_34[3]->mUnk_11 = data_ov007_02279d3c[0];
    } else {
        mData->mUnk_34[4]->mUnk_11 = 0xb;
        mData->mUnk_34[3]->mUnk_11 = 0xb;
    }
    mData->func_ov007_0223dfcc(0, kind, mUnk_14[0], 0x54, 2, mode, mUnk_10, mUnk_12);
    mData->func_ov007_0223dfcc(1, kind, mUnk_14[1], 0x54, 2, mode, mUnk_10, mUnk_12);
}

u8 BattleConfrontCard::func_ov007_022433ac() {
    int kind = func_02065c0c(mData->mUnk_04, 2);
    return kind;
}

// non-matching: same instruction sequence and jump table; register assignment differs (result r5/self r6 in the original) and case 42 emits a `mov r1, r6` the original does not.
BOOL BattleConfrontCard::func_ov007_022433c8(u8 side, u16 id) {
    BattleData *data   = mData;
    BattleEntry *self  = &data->mEntries[side];
    u8 opponent        = side ^ 1;
    BattleEntry *other = &data->mEntries[opponent];
    BOOL result        = FALSE;
    u8 owner           = data->mUnk_06;
    u8 i               = 0;
    u8 found           = 0;
    u32 type           = 0;
    u8 pattern[3];
    switch (id) {
        case 0x68:
        case 0x70:
        case 0x80:
            if (self->mUnk_0d_0 == 0 || self->mUnk_0c == 6) {
                result = TRUE;
            }
            break;
        case 0x69:
        case 0x71:
        case 0x81:
            if (self->mUnk_0d_0 == 1) {
                result = TRUE;
            }
            break;
        case 0x6a:
        case 0x72:
        case 0x82:
            if (self->mUnk_0d_0 == 2) {
                result = TRUE;
            }
            break;
        case 0x6b:
        case 0x73:
        case 0x83:
            if (self->mUnk_0d_0 == 3 || self->mUnk_0c == 5) {
                result = TRUE;
            }
            break;
        case 0x6c:
        case 0x74:
        case 0x84:
            if (self->mUnk_0d_0 == 4) {
                result = TRUE;
            }
            break;
        case 0x6d:
        case 0x75:
        case 0x85:
            if (self->mUnk_0d_0 == 5) {
                result = TRUE;
            }
            break;
        case 0x6e:
        case 0x76:
        case 0x86:
            if ((self->mUnk_0c == 5 && other->mUnk_0d_0 == 3) || (self->mUnk_0c == 6 && other->mUnk_0d_0 == 0) ||
                (other->mUnk_0c == 5 && self->mUnk_0d_0 == 3) || (other->mUnk_0c == 6 && self->mUnk_0d_0 == 0)) {
                result = TRUE;
            }
            if (self->mUnk_0d_0 == other->mUnk_0d_0) {
                result = TRUE;
            }
            break;
        case 0x6f:
        case 0x77:
        case 0x87:
            if (self->mUnk_0d_0 != other->mUnk_0d_0) {
                if (!(self->mUnk_0c == 5 && other->mUnk_0d_0 == 3) && !(self->mUnk_0c == 6 && other->mUnk_0d_0 == 0) &&
                    !(other->mUnk_0c == 5 && self->mUnk_0d_0 == 3) && !(other->mUnk_0c == 6 && self->mUnk_0d_0 == 0)) {
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
            if (type == func_02065c0c(data->mUnk_04, 1)) {
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
            if (self->mUnk_0c == type) {
                result = TRUE;
            }
            break;
        case 0xa6:
            if ((u8) (self->mUnk_0c + 0xfb) <= 1) {
                result = TRUE;
            }
            break;
        case 0xac:
        case 0xb5:
            if (owner != self->mUnk_0d_4) {
                break;
            }
            goto abilityCard;
        case 0xad:
        case 0xae:
        case 0xb6:
        case 0xb7:
            if (owner == self->mUnk_0d_4) {
                break;
            }
            if (data_020d433c.mUnk_98 == 0) {
                goto abilityCard;
            }
            if (owner == data_020d433c.mBattlePlayers->mPlayers[self->mUnk_0d_4]->mUnk_f2) {
                break;
            }
            goto abilityCard;
        case 0xb9:
        case 0xba:
            if (data_020d433c.mBattlePlayers->mPlayers[self->mUnk_0d_4]->mUnk_ee == 0) {
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
            if (other->mUnk_0d_0 != 2) {
                break;
            }
            goto abilityCard;
        case 0xc2:
            if (other->mUnk_0d_0 != 3 && other->mUnk_0c != 5) {
                break;
            }
            goto abilityCard;
        case 0xc3:
            if (other->mUnk_0d_0 != 4) {
                break;
            }
            goto abilityCard;
        case 0xc4:
            if (other->mUnk_0d_0 != 1) {
                break;
            }
            goto abilityCard;
        case 0xc5:
            if (other->mUnk_0d_0 != 5) {
                break;
            }
            goto abilityCard;
        case 0xc6:
            if (other->mUnk_0d_0 != 0 && other->mUnk_0c != 6) {
                break;
            }
        abilityCard:
            if (self->mUnk_0c == 5) {
                if (func_02065bf4(id, 3) != 0) {
                    result = TRUE;
                }
            }
            if (self->mUnk_0c == 6) {
                if (func_02065bf4(id, 0) != 0) {
                    result = TRUE;
                }
            }
            if (func_02065bf4(id, self->mUnk_0d_0) != 0) {
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
            BattlePlayer *player = data_020d433c.mBattlePlayers->mPlayers[self->mUnk_0d_4];
            u8 playerCount       = data_020d433c.mPlayerCount;
            do {
                u8 kind = player->mRecords[i].mUnk_09;
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
            BattlePlayer *player = data_020d433c.mBattlePlayers->mPlayers[self->mUnk_0d_4];
            u8 playerCount       = data_020d433c.mPlayerCount;
            do {
                u8 kind = player->mRecords[i].mUnk_09;
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
            value = data_020d433c.mBattlePlayers->mPlayers[e->mUnk_0d_4]->mUnk_ee * 50;
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
            value   = func_02065bf4(id, e->mUnk_0d_0) * 10;
            u16 old = value;
            if (e->mUnk_0c == 5) {
                if (value < func_02065bf4(id, 3) * 10) {
                    value = func_02065bf4(id, 3) * 10;
                }
            }
            if (e->mUnk_0c == 6) {
                if (old < func_02065bf4(id, 0) * 10) {
                    value = func_02065bf4(id, 0) * 10;
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

void BattleConfrontCard::func_ov007_02243d40() {
    GXS_SetVisiblePlane(0x13);
    if (mUnk_80) {
        mUnk_80->mUnk_064 = 1;
    }
    mUnk_69 = 1;
    if (data_020d433c.mUnk_10) {
        data_020d433c.mUnk_10->mUnk_1a = 1;
    }
    G2S_SetWnd0Position(0x10, 0x10, 0xf0, 0xb0);
    GXS_SetVisibleWnd(5);
    G2S_SetWnd0InsidePlane(0x13, FALSE);
    G2S_SetWndOutsidePlane(0x13, TRUE);
    G2S_SetBlendBrightness(0x11, -8);
}

void BattleConfrontCard::func_ov007_02243dec() {
    GXS_SetVisiblePlane(0x11);
    if (mUnk_80) {
        mUnk_80->mUnk_064 = 0;
    }
    mUnk_69 = 0;
    G2S_SetBlendBrightness(0x11, 0);
    if (data_020d433c.mUnk_10) {
        data_020d433c.mUnk_10->mUnk_1a = 0;
    }
    GXS_SetVisibleWnd(0);
    G2S_SetWnd0InsidePlane(0x1f, FALSE);
    G2S_SetWndOutsidePlane(0x3f, FALSE);
}

extern const WindowItemRecord data_ov007_02279d48[3];
extern const WindowCellRecord data_ov007_02279d9c[3];

void BattleConfrontCard::func_ov007_02243e7c() {
    if (mUnk_80) {
        delete mUnk_80;
    }
    mUnk_80 = 0;
    if (mUnk_84) {
        OS_Free(mUnk_84);
        mUnk_84 = 0;
    }
    if (mUnk_78) {
        OS_Free(mUnk_78);
        mUnk_78 = 0;
    }
    if (mUnk_7c) {
        OS_Free(mUnk_7c);
        mUnk_7c = 0;
    }
}

void BattleConfrontCard::func_ov007_02243f04(const char *path, u32 mode) {
    ChangeDir("/Game/2D/union_b/");
    func_02025b64(1, 8, 0, "union_b_bg_window_b_nsc.bin", 0, 0, 0);
    func_02025b64(1, 0xc, 0, "union_b_bg_window_ncg.bin", 0, 0, 0);
    func_02025b64(1, 1, 0x2000, "union_b_bg_window_ncl.bin", 0, 0, 0);
    SpriteScreen &screen = SpriteManager::Get().Screen(1);
    ChangeDir("/Game/2D/union_b/");
    screen.LoadCharacters("union_b_obj_window.nce_ncg1D.bin", 0xc000, 0);
    CellAnimation *old = mUnk_24[1];
    if (old) {
        delete old;
    }
    mUnk_24[1]            = 0;
    mUnk_24[1]            = new CellAnimation("union_b_obj_window_nce.bin", 0, 1, 1);
    mUnk_24[1]->mCharBase = screen.func_020276f4(0xc000);
    mUnk_24[1]->mUnk_11   = 7;
    mUnk_24[1]->SetAnimation(0xc);
    if (mUnk_84) {
        OS_Free(mUnk_84);
    }
    if (mUnk_78) {
        OS_Free(mUnk_78);
    }
    if (mUnk_7c) {
        OS_Free(mUnk_7c);
    }
    mUnk_84 = (MessageFile *) func_02023504(path, 0, 0, 0, 0, 0);
    mUnk_6c = TextManager::Get().BufferSize(0x18, 0xc);
    mUnk_70 = TextManager::Get().GlyphCount(9);
    mUnk_78 = OS_Alloc(mUnk_6c);
    mUnk_7c = OS_Alloc(mUnk_70 * 12);
    TextManager::Get().Init(mUnk_7c, 1, 0, 0x18, 0xc, 0x42);
    func_ov007_022442d4(mode);
    if (mode != 4) {
        if (mUnk_80) {
            delete mUnk_80;
        }
        mUnk_80 = new SelectWindow(3, data_ov007_02279d48, 1);
        mUnk_80->SetCells(3, data_ov007_02279d9c);
        mUnk_80->mCursor->mSelection = -1;
        mUnk_80->mUnk_064            = 0;
    }
    ChangeDir("/");
}

void BattleConfrontCard::func_ov007_022442d4(u32 index) {
    MI_CpuClear16(mUnk_78, mUnk_6c);
    TextManager &text = TextManager::Get();
    s16 height;
    s16 width;
    width              = 0;
    height             = 0;
    MessageFile *file = mUnk_84;
    text.Print(file->GetText(index), file->GetLength(index), mUnk_78, 0x18, 0xc, &height, &width, 0, 0, 2, 0x42, 0x10, 0, 0, 0, 1, 1);
    func_02025c1c(SpriteManager::Get().Screen(1).mBgIndex, 4, 0xd000, mUnk_78, mUnk_6c, 0);
}

u8 BattleConfrontCard::func_ov007_02244440() {
    BattleData *data = mData;
    BattleEntry *e   = data->mEntries;
    mUnk_50          = func_ov007_022428d4(data->mUnk_04);
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
                if (!func_ov007_022433c8(i, data_020d433c.mBattlePlayers->mPlayers[e[i].mUnk_0d_4]->mSlots6c[j].mValue)) {
                    (&mask)[k / 8] |= 1 << (k % 8);
                }
                k++;
                j++;
            } while (j < data_020d433c.mUnk_99);
            u8 other = (i + 1) % 2;
            mUnk_53 = func_ov007_0226a700(data_020d433c.mBattlePlayers->mPlayers[cur->mUnk_0d_4], mask,
                                          (s16) (e[other].mUnk_04 - e[i].mUnk_04), (s16) (e[other].mUnk_00 - e[i].mUnk_00));
            if (mUnk_53 == 0xff) {
                mUnk_56 = 0;
            } else {
                func_ov007_0226a448(data_020d433c.mBattlePlayers->mPlayers[e[i].mUnk_0d_4], mUnk_53, 2);
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
        power[n] = power[n] * func_ov007_022234f0(e[n].mUnk_0d_4, mUnk_20, e[n].mUnk_0e);
        n++;
    } while (n < 2);
    u8 mode = data_020d433c.mUnk_95;
    if (mode != 0 && mode != 7) {
        const s32 *row = data_ov007_02279df0[(u8) (mode - 1)];
        u8 m           = 0;
        do {
            float f  = (float) row[e[m].mUnk_0d_0] / 4096.0f;
            power[m] = (u16) (f * (float) power[m]);
            m++;
        } while (m < 2);
    }
    u16 chance = (u16) (100.0f * ((float) power[0] / (float) (power[0] + power[1])));
    BOOL r = MATH_Rand32(&data_020d42e8, 100) >= chance;
    return r;
}
