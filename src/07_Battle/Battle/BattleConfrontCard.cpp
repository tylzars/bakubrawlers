#include "Battle/BattleConfrontCard.hpp"
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
#include <nitro/gx.h>
#include <nitro/mi.h>
#include <nitro/os.h>
#include <stdio.h>

extern const s8 data_ov007_02279d3c[];
u8 func_02065bf4(u32 id, u32 index);
// Card detail object (0x20 bytes, ctor 0225f33c).
class BattleCardSpec {
public:
    /* 00 */ PAD(0x00, 0x0c);
    /* 0c */ void *mUnk_0c;
    /* 10 */ PAD(0x10, 0x1e);
    /* 1e */ u8 mUnk_1e;
    /* 1f */ PAD(0x1f, 0x20);
    /* 20 */

    BattleCardSpec(u32 a, u32 b, u32 c, u8 d);
};

class BattleCommandPage { // 0x10c bytes, ctor 02054df0
public:
    u8 mPad[0x10c];
    BattleCommandPage(u32 a, s32 b);
};

class BattleCommandPrompt { // 0x98 bytes, ctor 0225d08c
public:
    u8 mPad[0x98];
    BattleCommandPrompt(u32 a, void *b, u32 c, u32 d, u8 e);
};

void func_02055624(BattleCommandPage *page, u32 a);
void func_0205562c(BattleCommandPage *page, u32 a);
extern "C" void func_02055738(BattleCommandPage *page, u32 a, u32 b, u32 c);
extern "C" void func_ov007_02273e3c(void *p);
extern "C" u8 func_ov007_0225f0bc(u32 v);

extern "C" BOOL func_ov007_02263150(BattlePlayerList *list);
extern "C" void func_ov007_022633f8(BattlePlayerList *list);
BOOL func_ov007_022631bc(BattlePlayerList *list, u32 id);
BOOL func_0203dd0c(FrameTask *task, u32 channel);
void func_020383f8(u32 heap);
void func_02001e68(u32 unk1, u32 unk2, u32 unk3);
void func_02001ed0(u32 unk1);

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
    func_02001e68(1, 0, 1);
    func_02001ed0(3);
    func_ov007_0223f168();
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

void BattleConfrontCard::func_ov007_022430bc(int brightness) {
    G2S_SetBlendBrightness(0x1f, brightness);
}

void BattleConfrontCard::func_ov007_022430d8(int blend) {
    G2S_SetBlendAlpha(0x10, 0x2f, 0x10 - blend, blend);
}

u8 BattleConfrontCard::func_ov007_022433ac() {
    return func_02065c0c(mData->mUnk_04, 2);
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
            func_02001e68(1, 0, 1);
            mUnk_44 = new BattleCardSpec(3, e[mUnk_52].mUnk_0c, e[mUnk_52].mUnk_0d_0, e[mUnk_52].mUnk_0e == 0x15 ? 1 : 0);
            mContext->mUnk_12 = 0;
            mUnk_51           = 0;
            break;
        }
    }
    return result;
}

extern WindowItemRecord data_ov007_02279d48[3];
extern const WindowCellRecord data_ov007_02279d9c[3];

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
