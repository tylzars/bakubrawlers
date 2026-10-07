#include "Battle/BattleBase.hpp"
#include "Game/UnkStruct_020d433c.hpp"
#include "Game/UnkStruct_020d43f0.hpp"
#include "Graphics/CellAnimation.hpp"
#include "Graphics/Display.hpp"
#include "Graphics/SpriteManager.hpp"
#include "Scene/SceneBase.hpp"
#include "System/FileSystem.hpp"
#include "System/FrameTask.hpp"
#include "System/InputManager.hpp"
#include "System/SoundManager.hpp"
#include "System/TouchManager.hpp"
#include "Text/TextManager.hpp"
#include "UI/MessageWindow.hpp"
#include "Unknown/UnkFunctions.hpp"
#include "Unknown/UnkStruct_02078290.hpp"
#include "Unknown/UnkStruct_020651bc.hpp"
#include <nitro/gx.h>
#include <nitro/mi.h>
#include <nitro/os.h>

u32 func_020779ec(void *unk);

// Result screen of the battle (result_u_*, retry_*, select_arena_*): shows the outcome and the rewards per player.
class BattleResult : public SceneBase {
public:
    /* 0c */ BattleObject *mUnk_0c;
    /* 10 */ MessageWindow *mMessage;
    /* 14 */ void *mUnk_14;
    /* 18 */ CellAnimation *mAnims[8];
    /* 38 */ void *mUnk_38[3];
    /* 44 */ void *mUnk_44[3];
    /* 50 */ u32 mUnk_50[3];
    /* 5c */ u32 mUnk_5c[3];
    /* 68 */ void *mUnk_68;
    /* 6c */ UnkStruct_020651bc *mIcons;
    /* 70 */ CellAnimation *mAnimsB[5];
    /* 84 */ u32 mUnk_84[6];
    /* 9c */ s32 mUnk_9c;
    /* a0 */ s32 mUnk_a0;
    /* a4 */ s32 mUnk_a4;
    /* a8 */ s32 mUnk_a8;
    /* ac */ u8 mUnk_ac;
    /* ad */ u8 mUnk_ad;
    /* ae */ u8 mUnk_ae;
    /* af */ u8 mUnk_af;
    /* b0 */ u8 mUnk_b0;
    /* b1 */ u8 mUnk_b1;
    /* b2 */ u8 mUnk_b2;
    /* b3 */ u8 mUnk_b3;
    /* b4 */

    BattleResult();
    virtual ~BattleResult();
    virtual void Update();
    void func_ov007_0224e3ec();
    void func_ov007_0224e5f8();
    void func_ov007_0224e6f4();
    void func_ov007_0224f8f4();
    void func_ov007_0224f9a0();
    void func_ov007_0224fb70();
    void func_ov007_0224fcdc();
    void func_ov007_02250010();
    void func_ov007_0225047c();
    void func_ov007_02251724();
    void func_ov007_02251ae4();
    void func_ov007_022525c4();
    void func_ov007_022526f0();
};

BattleResult::BattleResult()
    : SceneBase(0x3e8, 0) {
    mUnk_0c = 0;
    mMessage = 0;
    mUnk_68 = 0;
    mUnk_9c = 0;
    mUnk_a0 = 0;
    mUnk_a4 = 0;
    mUnk_a8 = 0;
    mUnk_ac = 0;
    mUnk_ad = 0;
    mUnk_ae = 0;
    mUnk_af = 0;
    mUnk_b0 = 0;
    mUnk_b1 = 0;
    mUnk_b2 = 0;
    mUnk_b3 = 0;
    SpriteManager::Get();
    SpriteManager::sFade.mMain = 1;
    SpriteManager::sBrightness.mMain = -16;
    MI_CpuClear32(mAnims, sizeof(mAnims));
    MI_CpuClear32(mUnk_84, sizeof(mUnk_84));
    mUnk_14 = 0;
    MI_CpuClear32(mUnk_38, sizeof(mUnk_38));
    MI_CpuClear32(mUnk_44, sizeof(mUnk_44));
    MI_CpuClear32(mUnk_50, sizeof(mUnk_50));
    MI_CpuClear32(mUnk_5c, sizeof(mUnk_5c));
    mIcons = 0;
    mAnimsB[0] = 0;
    mAnimsB[1] = 0;
    mAnimsB[2] = 0;
    mAnimsB[3] = 0;
    mAnimsB[4] = 0;
}

// non-matching: 2 words: the original schedules `mov r3, #1` before the sFade address load in the SpriteManager::Get() block (D1 and D0)
BattleResult::~BattleResult() {
    SpriteManager::Get();
    SpriteManager::sFade.mMain = 1;
    SpriteManager::sBrightness.mMain = -16;
    if (mUnk_0c) {
        delete mUnk_0c;
    }
    if (mMessage) {
        delete mMessage;
    }
    FrameTask::Get().func_0203dd90();
    u8 i = 0;
    do {
        CellAnimation *anim = mAnims[i];
        if (anim) {
            delete anim;
        }
        i++;
    } while (i < 8);
    if (mIcons) {
        delete mIcons;
    }
    if (mAnimsB[0]) {
        delete mAnimsB[0];
    }
    if (mAnimsB[1]) {
        delete mAnimsB[1];
    }
    if (mAnimsB[2]) {
        delete mAnimsB[2];
    }
    if (mAnimsB[3]) {
        delete mAnimsB[3];
    }
    if (mAnimsB[4]) {
        delete mAnimsB[4];
    }
    if (mUnk_14) {
        OS_Free(mUnk_14);
        mUnk_14 = 0;
    }
    i = 0;
    do {
        if (mUnk_38[i]) {
            func_ov007_022526f0();
            OS_Free(mUnk_38[i]);
        }
        if (mUnk_44[i]) {
            OS_Free(mUnk_44[i]);
            mUnk_44[i] = 0;
        }
        i++;
    } while (i < 3);
}

void BattleResult::func_ov007_0224e3ec() {
    SpriteManager::Get().func_02026dec(1, 7);
    G2_SetBG0Offset(0, 0);
    G2_BlendNone();
    GX_SetDispSelect(1);
    GX_SetVisiblePlane(0x11);
    func_02001e68(1, 0, 1);
    G2_SetBG0Priority(0);
    G2_SetBG1Priority(1);
    GX_SetVisibleWnd(0);
    G2S_SetBG0Control(0, 1, 0, 1, 0);
    G2S_SetBG1Control(0, 1, 2, 4, 0);
    G2S_SetBG2Control(0, 1, 4, 4, 0);
    G2S_SetBG3Control(0, 1, 6, 4, 0);
    G2S_SetBG0Offset(0, 0);
    G2S_SetBG1Offset(0, 0);
    G2S_SetBG2Offset(0, 0);
    G2S_SetBG3Offset(0, 0);
    G2S_SetBG0Priority(0);
    G2S_SetBG1Priority(0);
    G2S_SetBG2Priority(0);
    G2S_SetBG3Priority(0);
    GXS_SetVisiblePlane(0x11);
    G2S_SetBlendBrightness(0x1f, 0);
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

void BattleResult::func_ov007_0224e5f8() {
    GX_SetDispSelect(1);
    SpriteManager::Get().func_02026dec(1, 1);
    func_02001e68(1, 0, 0);
    G2_SetBG0Control(0, 1, 0, 1, 0);
    G2_SetBG0Offset(0, 0);
    G2_SetBG0Priority(3);
    GX_SetVisiblePlane(0x11);
    G2_SetBlendBrightness(0x1f, 0);
    GX_SetVisibleWnd(0);
    G2_BlendNone();
}

void BattleResult::func_ov007_0224e6f4() {
    ChangeDir("/Game/Battle/Bg/");
    func_02025b64(0, 1, 0, "result_u_bg_00_ncl.bin", 0, 0, 0);
    func_02025b64(0, 11, 0, "result_u_bg_00_ncg.bin", 0, 0, 0);
    func_02025b64(0, 7, 0, "result_u_bg_00_nsc.bin", 0, 0, 0);
    ChangeDir("/Game/Battle/Obj/");
    SpriteScreen &screen = SpriteManager::Get().Screen(0);
    screen.func_02027624(3);
    screen.LoadCharacters("bakugan_sub_s.nce_ncg1D.bin", 0x2000, 0);
    screen.func_02027fec("bakugan_sub_s_ncl.bin", 0, 0);
    screen.LoadCharacters("result_u_obj_00.nce_ncg1D.bin", 0, 0);
    screen.LoadPalette("result_u_obj_00_ncl.bin", 0x180, 0);
    u8 i = 0;
    do {
        mAnims[i] = new CellAnimation("result_u_obj_00_nce.bin", 0, 1, 1);
        mAnims[i]->mCharBase = screen.func_020276f4(0);
        mAnims[i]->mUnk_11   = 12;
        mAnims[i]->SetAnimation(7);
        i++;
    } while (i < 2);
    i = 0;
    do {
        u8 *rec = (u8 *) data_020d433c.mBattlePlayers->mPlayers[data_020d433c.mBattlePlayers->mUnk_2a9] + 0x84;
        if (*(u16 *) (rec + i * 6) != 0) {
            mAnims[i + 2] = new CellAnimation("bakugan_sub_s_nce.bin", 0, 1, 1);
            mAnims[i + 2]->mCharBase = screen.func_020276f4(0x2000);
            mAnims[i + 2]->mUnk_11   = 0;
            mAnims[i + 2]->func_02029fb8(5);
            mAnims[i + 2]->mUnk_0f = rec[i * 6 + 2];
        }
        i++;
    } while (i < data_020d433c.mUnk_99);
    UnkStruct_02078290 *unk = data_020d433c.mUnk_10;
    if (unk) {
        unk->func_020785d8(12);
    }
    ChangeDir("/");
}

void BattleResult::func_ov007_0224f8f4() {
    SpriteManager::Get().Screen(1).func_02027778(mUnk_44[2], mUnk_5c[2], 0x20, 0x38, 0, 0xe0, 0xf, 0);
}

void BattleResult::func_ov007_022525c4() {
    SpriteManager::Get().Screen(0).func_02027778(mUnk_44[0], mUnk_5c[0], 10, 12, 0, 0x100, 14, 0);
    SpriteManager::Get().Screen(0).func_02027778(mUnk_44[1], mUnk_5c[1], 0x9e, 0x8c, 0, 0x1e0, 14, 0);
}

void BattleResult::func_ov007_022526f0() {
    u8 i = 0;
    do {
        if (mUnk_38[i]) {
            MI_CpuClear16(mUnk_38[i], mUnk_50[i]);
        }
        i++;
    } while (i < 3);
}

// non-matching: 5 words: original emits `mov r0,#1; str r0,[sp]` before computing (s16)(base + 0x25) for the third Draw
void BattleResult::func_ov007_0224f9a0() {
    mMessage->func_020658d0();
    mAnimsB[3]->Draw(0, 0x80, 0x60, 0);
    u8 i = 0;
    do {
        u32 x;
        s16 y;
        s32 base = i * 0x28;
        s32 top  = base + 0x21;
        y        = top;
        x = 0x1e;
        mAnimsB[1]->SetAnimation(i + 7);
        mAnimsB[1]->mUnk_0f = 0;
        mAnimsB[1]->Draw(0, x, y, 0);
        bool flag = false;
        if (data_020d433c.mBattlePlayers->mPlayers[i]->mUnk_c8) {
            flag = true;
        }
        if (flag) {
            y = top;
            x = 0x20;
            mAnimsB[1]->SetAnimation(11);
            mAnimsB[1]->mUnk_0f = i;
        } else if (i == data_020d433c.mBattlePlayers->mUnk_2a9) {
            y = base + 0x3a;
            x = 0x20;
            mAnimsB[1]->SetAnimation(i + 2);
            mAnimsB[1]->mUnk_0f = 0;
        }
        mAnimsB[1]->Draw(0, x, y, 0);
        mAnimsB[2]->Draw(0, 0x20, (s16) (top + 4), 1);
        mAnimsB[0]->Draw(0, 0x20, (s16) ((i + 1) * 0x28), 1);
        if (data_020d433c.mUnk_9f[i] != 0x15) {
            mAnimsB[4]->mUnk_0f = data_020d433c.mUnk_a3[i];
            mAnimsB[4]->Draw(0, 0x48, (s16) (base + 0x37), 0);
        }
        i++;
    } while (i < data_020d433c.mPlayerCount);
    mIcons->func_020654a0();
}

extern const u8 data_ov007_0227a359[];

void BattleResult::func_ov007_0224fb70() {
    if (func_020779ec(mUnk_68)) {
        return;
    }
    u8 i = 0;
    do {
        if (mAnims[i]) {
            mAnims[i]->Update();
            switch (i) {
                case 0:
                    if (mUnk_b2 >= 9) {
                        mAnims[i]->func_0202a3ec(0, 0, data_ov007_0227a359[i * 2], data_ov007_0227a359[i * 2 + 1], 1);
                    }
                    break;
                case 1:
                    if (mUnk_b2 >= 11) {
                        mAnims[i]->Draw(0, data_ov007_0227a359[i * 2], data_ov007_0227a359[i * 2 + 1], 1);
                    }
                    break;
                case 2:
                case 3:
                case 4:
                case 5:
                case 6:
                case 7:
                    if (mUnk_b2 >= (u8) (i - 2) + 3) {
                        if (mUnk_84[3] != 0) {
                            mAnims[i]->Draw(0, data_ov007_0227a359[i * 2], data_ov007_0227a359[i * 2 + 1], 1);
                        } else {
                            mAnims[i]->Draw(0, data_ov007_0227a359[i * 2 + 6], data_ov007_0227a359[i * 2 + 7], 1);
                        }
                    }
                    break;
            }
        }
        i++;
    } while (i < 8);
    func_ov007_022525c4();
}

void BattleResult::func_ov007_0224fcdc() {
    SpriteManager::Get().Screen(0).func_02027624(3);
    ChangeDir("/font/");
    mUnk_14 = func_02023504("mes_result.mes", 0, 0, 0, 0, 0);
    mUnk_50[0] = TextManager::Get().BufferSize(0x20, 0x16);
    mUnk_38[0] = OS_Alloc(mUnk_50[0]);
    MI_CpuClear16(mUnk_38[0], mUnk_50[0]);
    mUnk_5c[0] = TextManager::Get().GlyphCount(0x58);
    mUnk_44[0] = OS_Alloc((s16) mUnk_5c[0] * (s16) 0xc);
    TextManager::Get().Init(mUnk_44[0], 0, 0, 0x20, 0x16, 0x21);
    mUnk_50[1] = TextManager::Get().BufferSize(0x10, 6);
    mUnk_38[1] = OS_Alloc(mUnk_50[1]);
    MI_CpuClear16(mUnk_38[1], mUnk_50[1]);
    mUnk_5c[1] = TextManager::Get().GlyphCount(0xc);
    mUnk_44[1] = OS_Alloc((s16) mUnk_5c[1] * (s16) 0xc);
    TextManager::Get().Init(mUnk_44[1], 0, 0, 0x10, 6, 0x21);
    TextManager::Get().SetColor(0, 0, 0xe);
    func_ov007_02250010();
    func_ov007_022526f0();
    func_ov007_0225047c();
    func_ov007_02251724();
}

void BattleResult::func_ov007_02251724() {
    s16 outH = 0;
    s16 outW = 0;
    TextManager::Get().mUnk_5b = 1;
    if (mUnk_b2 >= 0x11) {
        TextManager::Get().PrintNumber(mUnk_a4, 7, 0, mUnk_38[1], 0x10, 6, &outH, &outW, 0, 2, 0x21, 0, 1, 1, 1);
    } else {
        TextManager::Get().PrintNumber(mUnk_a8, 7, 0, mUnk_38[1], 0x10, 6, &outH, &outW, 0, 2, 0x21, 0, 1, 1, 1);
    }
    outH = 0;
    outW = 0x1e;
    if (mUnk_b2 >= 0x11) {
        TextManager::Get().PrintNumber(data_020d43f0[0].mMoney - mUnk_a4, 7, 0, mUnk_38[1], 0x10, 6, &outH, &outW, 0, 2, 0x21, 0, 1, 1, 1);
    } else {
        TextManager::Get().PrintNumber(data_020d43f0[0].mMoney, 7, 0, mUnk_38[1], 0x10, 6, &outH, &outW, 0, 2, 0x21, 0, 1, 1, 1);
    }
    TextManager::Get().mUnk_5b = 0;
    func_02005e9c(mUnk_38[1], mUnk_50[1]);
    func_02025c1c(SpriteManager::Get().Screen(0).mBgIndex, 4, 0xf000, mUnk_38[1], mUnk_50[1], 0);
}

extern const u16 data_ov007_0227a504[2][6][9];
extern const u8 data_ov007_0227a350[3][3];
extern const u8 data_ov007_0227a37b[][3];

// non-matching: register allocation only (15 words): the original keeps &data_020d433c in r6 and &data_020d43f0 in r5 during the loop
void BattleResult::func_ov007_02250010() {
    if (mUnk_b3 != 0) {
        if (data_020d433c.mUnk_7b == 0xd) {
            switch (data_020d43f0[0].mUnk_42) {
                case 1:
                    mUnk_9c = data_ov007_0227a504[data_020d43f0[0].mUnk_5bf_0][0][data_020d43f0[0].mUnk_43] * 100;
                    break;
                case 3:
                    mUnk_9c = data_ov007_0227a504[data_020d43f0[0].mUnk_5bf_0][1][data_020d43f0[0].mUnk_43] * 100;
                    break;
                case 6:
                    mUnk_9c = data_ov007_0227a504[data_020d43f0[0].mUnk_5bf_0][2][data_020d43f0[0].mUnk_43] * 100;
                    break;
                case 7:
                    mUnk_9c = data_ov007_0227a504[data_020d43f0[0].mUnk_5bf_0][3][data_020d43f0[0].mUnk_43] * 100;
                    break;
                case 8:
                    mUnk_9c = data_ov007_0227a504[data_020d43f0[0].mUnk_5bf_0][4][data_020d43f0[0].mUnk_43] * 100;
                    break;
                case 9:
                    mUnk_9c = data_ov007_0227a504[data_020d43f0[0].mUnk_5bf_0][5][data_020d43f0[0].mUnk_43] * 100;
                    break;
                case 10:
                    mUnk_9c = data_ov007_0227a504[data_020d43f0[0].mUnk_5bf_0][0][data_020d43f0[0].mUnk_43] * 100;
                    break;
            }
        } else {
            u32 me = data_020d433c.mBattlePlayers->mUnk_2a9;
            const u8 (*tbl)[3] = data_ov007_0227a37b;
            u32 count = data_020d433c.mPlayerCount;
            UnkStruct_020d43f0 *save = &data_020d43f0[0];
            u8 i = 1;
            do {
                u32 idx = (u8) (me + i);
                u8 rem = idx % count;
                BattlePlayer *player = data_020d433c.mBattlePlayers->mPlayers[rem];
                u8 a = player->mUnk_f6;
                u8 b = player->mUnk_f7;
                if (data_020d433c.mUnk_98 == 0 || rem != data_020d433c.mBattlePlayers->mPlayers[me]->mUnk_f2) {
                    mUnk_9c += tbl[a][b % 3] * 100;
                    if (save->mUnk_5bf_0) {
                        mUnk_9c += 0x308 + 0x400;
                    }
                }
                i++;
            } while (i < count);
        }
    }
    u8 j = 0;
    u8 me2 = data_020d433c.mBattlePlayers->mUnk_2a9;
    do {
        mUnk_84[j] = *(u16 *) ((u8 *) data_020d433c.mBattlePlayers->mPlayers[me2] + 0x84 + j * 6);
        j++;
    } while (j < data_020d433c.mUnk_99);
    mUnk_ad = data_020d433c.mBattlePlayers->mPlayers[me2]->mUnk_fd;
    mUnk_ae = data_020d433c.mBattlePlayers->mPlayers[me2]->mUnk_fc;
    u32 n2 = data_020d433c.mPlayerCount;
    u16 v = data_020d433c.mBattlePlayers->mUnk_288;
    if (v != 0) {
        v = v - 1;
    }
    if (n2 != 0) {
        v = v / n2;
    }
    if (mUnk_b3 != 0) {
        int k = 0;
        if (v >= 3) {
            k = (u16) (v - 3);
        }
        if (k < 3) {
            if (data_020d433c.mUnk_98 != 0) {
                mUnk_a0 = data_ov007_0227a350[1][k] * 100;
            } else if (data_020d433c.mUnk_99 < 6) {
                mUnk_a0 = data_ov007_0227a350[0][k] * 100;
            } else {
                mUnk_a0 = data_ov007_0227a350[2][k] * 100;
            }
        }
    }
    mUnk_ac = 1;
    BattlePlayerList *list2 = data_020d433c.mBattlePlayers;
    u32 count2 = data_020d433c.mPlayerCount;
    u8 m = 1;
    u32 me3 = list2->mUnk_2a9;
    u8 flag98 = data_020d433c.mUnk_98;
    do {
        u32 idx2 = (u8) (me3 + m);
        u8 rem2 = idx2 % count2;
        if (flag98 == 0 || rem2 != list2->mPlayers[me3]->mUnk_f2) {
            if (list2->mPlayers[rem2]->mUnk_ee != 0) {
                mUnk_ac = 0;
                return;
            }
        }
        m++;
    } while (m < count2);
}

#define COUNT_STEP(CLEAR_ONLY)                                                         \
    if ((u32) mUnk_a4 < 10 || (InputManager::Get().IsHeld(1) | TouchManager::Get().mUnk_57)) { \
        if (!(CLEAR_ONLY)) {                                                           \
            mUnk_a8 += mUnk_a4;                                                        \
        }                                                                              \
        mUnk_a4 = 0;                                                                   \
    } else {                                                                           \
        mUnk_a4 -= 10;                                                                 \
        if (!(CLEAR_ONLY)) {                                                           \
            mUnk_a8 += 10;                                                             \
        }                                                                              \
    }                                                                                  \
    if (mUnk_b0 != 0) {                                                                \
        mUnk_b0--;                                                                     \
    } else {                                                                           \
        SoundManager::Get().func_020246f8(8);                                          \
        mUnk_b0 = 4;                                                                   \
    }

void BattleResult::func_ov007_02251ae4() {
    if (InputManager::Get().IsHeld(1) | TouchManager::Get().mUnk_57) {
        mUnk_af = 0;
    }
    if (mUnk_af != 0) {
        mUnk_af--;
        return;
    }
    if (mUnk_38[1]) {
        MI_CpuClear16(mUnk_38[1], mUnk_50[1]);
    }
    switch (mUnk_b2) {
        case 0:
            mUnk_a4 += mUnk_9c;
            mUnk_af = 30;
            mUnk_b2++;
            func_ov007_0225047c();
            break;
        case 1:
            if (mUnk_a4 != 0) {
                COUNT_STEP(0)
            } else {
                mUnk_b2++;
                mUnk_b0 = 0;
            }
            break;
        case 2:
            mUnk_a4 = mUnk_84[0];
            mUnk_af = 30;
            mUnk_b2++;
            func_ov007_0225047c();
            break;
        case 3:
        case 4:
        case 5:
        case 6:
        case 7:
        case 8:
            if (mUnk_a4 != 0) {
                COUNT_STEP(0)
            } else {
                mUnk_b2++;
                mUnk_b0 = 0;
                mUnk_af = 30;
                mUnk_a4 = mUnk_84[mUnk_b2 - 3];
                if (mUnk_a4 == 0) {
                    mUnk_b2 = 9;
                }
                func_ov007_0225047c();
            }
            break;
        case 9:
            mUnk_a4 = mUnk_ad * 100;
            mUnk_af = 30;
            mUnk_b2++;
            func_ov007_0225047c();
            break;
        case 10:
            if (mUnk_a4 != 0) {
                COUNT_STEP(0)
            } else {
                mUnk_a4 = mUnk_ae * 100;
                mUnk_af = 30;
                mUnk_b2++;
                mUnk_b0 = 0;
                func_ov007_0225047c();
            }
            break;
        case 11:
            if (mUnk_a4 != 0) {
                COUNT_STEP(0)
            } else {
                mUnk_af = 30;
                mUnk_b2++;
                mUnk_b0 = 0;
                func_ov007_0225047c();
            }
            break;
        case 12:
            mUnk_b2++;
            mUnk_a4 = mUnk_a0;
            mUnk_af = 30;
            func_ov007_0225047c();
            break;
        case 13:
            if (mUnk_a4 != 0) {
                COUNT_STEP(0)
            } else {
                mUnk_b2++;
                mUnk_b0 = 0;
                mUnk_af = 30;
            }
            break;
        case 14:
            mUnk_b2++;
            mUnk_af = 30;
            func_ov007_0225047c();
            break;
        case 15:
            data_020d433c.mUnk_8c_0 = 0;
            if (mUnk_ac != 0) {
                mUnk_a8 *= 2;
                if (data_020d433c.mUnk_7b == 12) {
                    data_020d433c.mUnk_8c_0 = 1;
                }
                SoundManager::Get().func_020246f8(8);
            }
            mUnk_af = 30;
            mUnk_b2++;
            break;
        case 16: {
            u32 add = mUnk_a8;
            if (add > 9999990) {
                add = 9999990;
            }
            data_020d43f0[0].mMoney += add;
            if (data_020d43f0[0].mMoney > 9999990) {
                data_020d43f0[0].mMoney = 9999990;
            }
            add = mUnk_a8;
            if (add > 9999990) {
                add = 9999990;
            }
            data_020d43f0[0].mUnk_20 += add;
            if (data_020d43f0[0].mUnk_20 > 9999990) {
                data_020d43f0[0].mUnk_20 = 9999990;
            }
            mUnk_a4 = mUnk_a8;
            mUnk_af = 30;
            mUnk_b2++;
            break;
        }
        case 17:
            if (mUnk_a4 != 0) {
                COUNT_STEP(1)
            } else {
                mUnk_b1++;
                mUnk_b0 = 0;
                *((u8 *) mUnk_68 + 0x78) = 1;
            }
            break;
    }
    func_ov007_02251724();
}
