#include "Battle/BattleBase.hpp"
#include "Battle/BattleUnk022767c8.hpp"
#include "Battle/BattleCharaAct.hpp"
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
#include "UI/SelectWindow.hpp"
#include "System/WirelessManager.hpp"
#include "Unknown/UnkStruct_020758f4.hpp"
#include "Game/UnkStruct_020d48f0.hpp"
#include "Unknown/UnkFunctions.hpp"
#include "Unknown/UnkStruct_02078290.hpp"
#include "Unknown/UnkStruct_020651bc.hpp"
#include <nitro/gx.h>
#include <nitro/mi.h>
#include <nitro/os.h>
#include "Unknown/UnkStruct_02089f90.hpp"

u32 func_020779ec(void *unk);
BOOL func_ov007_022631bc(BattlePlayerList *list, u32 id);
BOOL func_0202326c(UnkStruct_020d43f0 *d);
s32 func_02087100();
s32 func_02087468();
void func_02086d48();
extern u8 data_020d43e5[];
extern UnkStruct_0220f540 data_0220f540;

class Camera;
class UnkStruct_02076058 {
public:
    /* 00 */ PAD(0x00, 0x78);
    /* 78 */ u8 mUnk_78;
    /* 79 */ PAD(0x79, 0x80);
    UnkStruct_02076058(u32 unk1);
};
struct UnkStruct_020c4230 {
    /* 00 */ PAD(0x00, 0x20);
    /* 20 */ u16 mUnk_20;
};
extern UnkStruct_020c4230 data_020c4230;
extern const WindowItemRecord data_ov007_0227a3b4[3];
extern const WindowCellRecord data_ov007_0227a408[3];
extern const WindowItemRecord data_ov007_0227a45c[3];
extern const WindowCellRecord data_ov007_0227a4b0[3];

// Result screen of the battle (result_u_*, retry_*, select_arena_*): shows the outcome and the rewards per player.
class BattleResult : public SceneBase {
public:
    /* 0c */ SelectWindow *mUnk_0c;
    /* 10 */ MessageWindow *mMessage;
    /* 14 */ MessageFile *mUnk_14;
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
    /* b4 */ u8 mUnk_b4;
    /* b5 */

    BattleResult();
    virtual ~BattleResult();
    virtual void Update();
    void func_ov007_0224e3ec();
    void func_ov007_0224e5f8();
    void func_ov007_0224e6f4();
    void func_ov007_0224e9f4();
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
extern const u8 data_ov007_0227a359[];
extern const u16 data_ov007_0227a504[2][6][9];
extern const u8 data_ov007_0227a37b[][3];
extern const u8 data_ov007_0227a350[3][3];





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

// non-matching: register allocation only (about 50 words): case 1 (winner loop: original has count/team in r6/r7, loop counter r0, list r3), case 3 (r1/r2 swapped for the data_020d43e5 base/index) and case 4 (locals shifted by one register)
void BattleResult::Update() {
    if (data_020d433c.mUnk_a8) {
        WirelessManager::Get();
        if (!IsWirelessMode(&data_020c4030.mUnk_228)) {
            mFinished = 1;
        }
    }
    switch (mUnk_b1) {
        case 0:
            func_ov007_0224e3ec();
            mUnk_b1++;
            break;
        case 1: {
            u8 best = 0;
            u8 count = data_020d433c.mUnk_99;
            u8 team = data_020d433c.mUnk_98;
            BattlePlayerList *list = data_020d433c.mBattlePlayers;
            u8 i = 0;
            do {
                BattlePlayer *player = list->mPlayers[i];
                u32 limit = count;
                u32 v = player->mUnk_ee;
                if (team != 0) {
                    limit = 3;
                    v = (u8) (v + list->mPlayers[player->mUnk_f2]->mUnk_ee);
                }
                if (v >= limit) {
                    best = i;
                    break;
                }
                i = (u8) (i + 1);
            } while (i < 4);
            if (data_020d433c.mUnk_a8 == 0) {
                u32 v = best == 0 ? 2 : 3;
                if (data_020d433c.mUnk_7b == 12) {
                    data_020d433c.mUnk_7c = v;
                } else {
                    data_020d433c.mUnk_7d = v;
                }
                if (best == 0) {
                    data_020d433c.mUnk_8c_4 = 0;
                } else if (best == 1) {
                    data_020d433c.mUnk_8c_4 = 1;
                } else {
                    data_020d433c.mUnk_8c_4 = 2;
                }
            }
            u8 me = data_020d433c.mBattlePlayers->mUnk_2a9;
            u8 partner = data_020d433c.mBattlePlayers->mPlayers[me]->mUnk_f2;
            if (best == me || best == partner) {
                new BattleCharaAct(data_020d433c.mBattlePlayers->mCamera, me, 12, 0, 0);
                FrameTask::Get().func_0203dc34(0, 0x1a);
            } else {
                new BattleCharaAct(data_020d433c.mBattlePlayers->mCamera, me, 13, 0, 0);
                FrameTask::Get().func_0203dc34(0, 0x19);
            }
            mContext->mUnk_12 = 0;
            if (data_020d43e5[best] < 99) {
                data_020d43e5[best]++;
            }
            if (me != partner) {
                u8 other = data_020d433c.mBattlePlayers->mPlayers[best]->mUnk_f2;
                if (data_020d43e5[other] < 99) {
                    data_020d43e5[other]++;
                }
            }
            mUnk_b1++;
            break;
        }
        case 2:
            if (mContext->mUnk_12 != 0) {
                if (mContext->mUnk_12 == 2) {
                    FrameTask::Get().func_0203dd90();
                    mContext->mUnk_12 = 1;
                }
                if (!func_0203dd0c(&FrameTask::Get(), 0)) {
                    mUnk_b1++;
                }
            }
            break;
        case 3:
            if (data_020d433c.mUnk_7a != 0) {
                if (data_020d433c.mUnk_7b == 13 && data_020d433c.mUnk_7d == 3) {
                    if (data_020d43f0[0].mUnk_4a < 9999) {
                        data_020d43f0[0].mUnk_4a++;
                    }
                    mUnk_b1 = 8;
                } else {
                    mUnk_b1++;
                }
            } else {
                mUnk_b1 = 8;
            }
            break;
        case 4: {
            BattlePlayerList *list = data_020d433c.mBattlePlayers;
            u8 count = data_020d433c.mUnk_99;
            u8 me = list->mUnk_2a9;
            u8 numPlayers = data_020d433c.mPlayerCount;
            BattlePlayer *player = list->mPlayers[me];
            u8 own = player->mUnk_ee;
            u8 partner = player->mUnk_f2;
            if (own >= count) {
                mUnk_b3 = 1;
            } else if (data_020d433c.mUnk_98 != 0) {
                if ((u8) (own + list->mPlayers[partner]->mUnk_ee) >= count) {
                    mUnk_b3 = 1;
                }
            }
            if (mUnk_b3 != 0) {
                if (data_020d43f0[0].mUnk_48 < 9999) {
                    data_020d43f0[0].mUnk_48++;
                }
                u8 team = data_020d433c.mUnk_98;
                if (team != 0) {
                    if (data_020d43f0[0].mUnk_82 < 999) {
                        data_020d43f0[0].mUnk_82++;
                    }
                } else if (numPlayers >= 4) {
                    if (data_020d43f0[0].mUnk_84 < 999) {
                        data_020d43f0[0].mUnk_84++;
                    }
                } else {
                    if (data_020d43f0[0].mUnk_80 < 999) {
                        data_020d43f0[0].mUnk_80++;
                    }
                }
                BattlePlayerList *list2 = data_020d433c.mBattlePlayers;
                u8 k = 1;
                do {
                    u8 idx = (me + k) % numPlayers;
                    if (team == 0 || idx != partner) {
                        u8 f6 = list2->mPlayers[idx]->mUnk_f6;
                        if (data_020d43f0[0].mUnk_5e[f6] < 999) {
                            data_020d43f0[0].mUnk_5e[f6]++;
                        }
                    }
                    k = (u8) (k + 1);
                } while (k < numPlayers);
                if (list2->mUnk_288 != 0) {
                    int rounds = (u16) ((list2->mUnk_288 - 1) / numPlayers + 1);
                    if (rounds > 99) {
                        rounds = 99;
                    }
                    if (data_020d43f0[0].mUnk_5a != 0) {
                        if (data_020d43f0[0].mUnk_5a > rounds) {
                            data_020d43f0[0].mUnk_5a = rounds;
                        }
                    } else {
                        data_020d43f0[0].mUnk_5a = rounds;
                    }
                    if (data_020d43f0[0].mUnk_5b < rounds) {
                        data_020d43f0[0].mUnk_5b = rounds;
                    }
                }
            } else {
                if (data_020d43f0[0].mUnk_4a < 9999) {
                    data_020d43f0[0].mUnk_4a++;
                }
            }
            func_ov007_0224e5f8();
            func_ov007_0224e6f4();
            func_ov007_0224fcdc();
            SpriteManager::Get();
            SpriteManager::sFade.mMain = 2;
            FrameTask::Get().func_0203dc34(0, 0x11);
            mUnk_68 = new UnkStruct_02076058(0);
            mContext->mUnk_12 = 0;
            mUnk_b1++;
            break;
        }
        case 5:
            SpriteManager::Get();
            if (SpriteManager::sFade.mMain <= 1) {
                mUnk_b1++;
            }
            func_ov007_0224fb70();
            break;
        case 6:
            func_ov007_02251ae4();
            func_ov007_0224fb70();
            break;
        case 7:
            if (mContext->mUnk_12 != 0) {
                if (func_020779ec(mUnk_68) != 0) {
                    mUnk_b1++;
                } else {
                    mUnk_b1 = 10;
                }
            }
            func_ov007_0224fb70();
            break;
        case 8:
            SpriteManager::Get().StartFade(3);
            mUnk_b1++;
            break;
        case 9:
            if (!SpriteManager::Get().IsFading()) {
                mUnk_b1++;
            }
            break;
        case 10:
            mContext->mUnk_12 = 1;
            if (data_020d433c.mUnk_7b != 8 && data_020d433c.mUnk_7b != 12) {
                if (data_020d433c.mUnk_7d == 2) {
                    if (func_0202326c(&data_020d43f0[0])) {
                        new BattleUnk022767c8();
                        mContext->mUnk_12 = 0;
                    }
                }
            }
            mUnk_b1++;
            break;
        case 11:
            if (mContext->mUnk_12 != 0) {
                mUnk_b1++;
            }
            break;
        case 12:
            if (data_020d433c.mUnk_7b != 8) {
                data_020d48f0.mUnk_bc = data_020d48f0.mUnk_ba;
                UnkStruct_020758f4 *p = new UnkStruct_020758f4();
                data_020d48f0.mUnk_ba = p->mUnk_8e;
                delete p;
            }
            mUnk_b1++;
            break;
        case 13: {
            s32 id = -1;
            u8 mode = data_020d433c.mUnk_7b;
            u8 sub = data_020d433c.mUnk_7d;
            if ((mode == 13) & (sub == 2)) {
                id = func_02087100();
            } else if (mode == 12) {
                id = func_02087468();
            }
            if (id >= 0) {
                new UnkStruct_02089f90(id);
                mContext->mUnk_12 = 0;
                mUnk_b1++;
            } else {
                mUnk_b1 += 2;
            }
            break;
        }
        case 14:
            if (mContext->mUnk_12 != 0) {
                mUnk_b1--;
            }
            break;
        case 15:
            if (data_020d433c.mUnk_7b == 8) {
                mUnk_b1++;
                break;
            }
            func_02086d48();
            RequestScene(1);
            mFinished = 1;
            break;
        case 16:
            func_ov007_0224e9f4();
            FrameTask::Get().func_0203dc34(0, 0x11);
            mUnk_b1++;
            break;
        case 17: {
            func_ov007_0224f9a0();
            WirelessManager::Get();
            bool solo = data_020d433c.mUnk_a8 == 0;
            bool none = data_020c4230.mUnk_20 == 0;
            if (solo | none) {
                if (mUnk_0c) {
                    delete mUnk_0c;
                }
                mUnk_0c = new SelectWindow(3, data_ov007_0227a3b4, 0);
                mUnk_0c->SetCells(3, data_ov007_0227a408);
            }
            GXS_SetVisiblePlane(0x12);
            SpriteManager::Get().StartFade(2);
            mUnk_b1++;
            break;
        }
        case 18:
            func_ov007_0224f9a0();
            if (!SpriteManager::Get().IsFading()) {
                if (data_020d433c.mUnk_a8 != 0) {
                    WirelessManager::Get();
                    if (data_020c4230.mUnk_20 != 0) {
                        mUnk_b1 = 22;
                    } else {
                        mUnk_b1++;
                    }
                } else {
                    mUnk_b1++;
                }
            }
            break;
        case 19:
            func_ov007_0224f9a0();
            if (mUnk_0c->Update(1) == 1) {
                SelectWindowCursor *cursor = mUnk_0c->mCursor;
                s32 sel = -1;
                if (cursor->mSelection != -1) {
                    sel = cursor->mItems[cursor->mSelection].mId;
                }
                *((u8 *) &data_0220f540 + 4) = mUnk_b4 = sel;
                if (mUnk_b4 == 2) {
                    if (data_020d433c.mUnk_a8 == 0) {
                        mUnk_b1 = 22;
                    } else {
                        mUnk_b1++;
                    }
                } else {
                    mUnk_b1 = 22;
                }
            }
            break;
        case 20:
            func_ov007_0224f9a0();
            if (mUnk_0c) {
                delete mUnk_0c;
            }
            mUnk_0c = new SelectWindow(3, data_ov007_0227a45c, 0);
            mUnk_0c->SetCells(3, data_ov007_0227a4b0);
            mUnk_0c->mCursor->mSelection = 1;
            GXS_SetVisiblePlane(0x13);
            mUnk_b1++;
            break;
        case 21:
            func_ov007_0224f8f4();
            func_ov007_0224f9a0();
            if (mUnk_0c->Update(1) == 1) {
                SelectWindowCursor *cursor = mUnk_0c->mCursor;
                s8 sel = -1;
                if (cursor->mSelection != -1) {
                    sel = cursor->mItems[cursor->mSelection].mId;
                }
                if (sel != 0) {
                    mUnk_b1 = 17;
                } else {
                    mUnk_b1++;
                }
            }
            break;
        case 22:
            func_ov007_0224f9a0();
            if (func_ov007_022631bc(data_020d433c.mBattlePlayers, 0x13)) {
                FrameTask::Get().func_0203dd90();
                SpriteManager::Get().StartFade(3);
                mUnk_b1++;
            }
            break;
        case 23:
            func_ov007_0224f9a0();
            if (!func_0203dd0c(&FrameTask::Get(), 0)) {
                if (!SpriteManager::Get().IsFading()) {
                    u8 result;
                    if (data_020d433c.mUnk_a8 != 0) {
                        result = data_020d433c.mBattlePlayers->mPlayers[0]->mMsg.mTouchDown;
                    } else {
                        result = mUnk_b4;
                    }
                    switch (result) {
                        case 0:
                            RequestScene(2);
                            mFinished = 1;
                            break;
                        case 1:
                            RequestScene(3);
                            mFinished = 1;
                            break;
                        case 2:
                            RequestScene(1);
                            mFinished = 1;
                            break;
                    }
                }
            }
            break;
    }
    if (mUnk_0c) {
        mUnk_0c->Draw();
    }
}

void BattleResult::func_ov007_0224e3ec() {
    SpriteManager::Get().func_02026dec(1, 7);
    G2_SetBG0Offset(0, 0);
    G2_BlendNone();
    GX_SetDispSelect(1);
    GX_SetVisiblePlane(0x11);
    GX_SetGraphicsMode(1, 0, 1);
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
    GX_SetGraphicsMode(1, 0, 0);
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

void BattleResult::func_ov007_0224e9f4() {
    SpriteManager::Get().func_02026dec(1, 1);
    GX_SetDispSelect(1);
    GX_SetGraphicsMode(1, 0, 0);
    G2_SetBG0Control(0, 1, 0, 1, 0);
    G2_SetBG1Control(0, 0, 1, 4, 0);
    G2S_SetBG0Control(0, 1, 0, 1, 0);
    G2S_SetBG1Control(0, 1, 2, 4, 0);
    GX_SetVisiblePlane(0x13);
    GXS_SetVisiblePlane(0x12);
    G2_SetBG0Priority(3);
    G2_SetBG1Priority(0);
    G2S_SetBG0Priority(3);
    G2S_SetBG1Priority(3);
    G2_SetBG0Offset(0, 0);
    G2_SetBG1Offset(0, 0);
    G2S_SetBG0Offset(0, 0);
    G2S_SetBG1Offset(0, 0);
    ChangeDir("/Game/Battle/Bg/");
    func_02025b64(0, 1, 0, "select_arena_u_bg_00_ncl.bin", 0, 0, 0);
    func_02025b64(0, 11, 0, "select_arena_u_bg_00_ncg.bin", 0, 0, 0);
    func_02025b64(0, 7, 0, "select_arena_u_bg_00_nsc.bin", 0, 0, 0);
    func_02025b64(1, 1, 0x2000, "select_arena_b_bg_00_ncl.bin", 0, 0, 0);
    func_02025b64(1, 12, 0, "select_arena_b_bg_00_ncg.bin", 0, 0, 0);
    func_02025b64(1, 8, 0, "select_arena_b_bg_00_nsc.bin", 0, 0, 0);
    UnkStruct_02078290 *unk = data_020d433c.mUnk_10;
    if (unk) {
        unk->func_020785d8(12);
    }
    ChangeDir("/Game/Battle/Obj/");
    SpriteScreen &screen1 = SpriteManager::Get().Screen(1);
    screen1.LoadCharacters("retry_b_obj_00.nce_ncg1D.bin", 0, 0);
    screen1.LoadPalette("retry_b_obj_00_ncl.bin", 0, 0);
    SpriteScreen &screen0 = SpriteManager::Get().Screen(0);
    screen0.LoadCharacters("retry_u_obj_00.nce_ncg1D.bin", 0x4000, 0);
    screen0.LoadPalette("retry_u_obj_00_ncl.bin", 0x1c0, 0);
    mAnimsB[3] = new CellAnimation("retry_u_obj_00_nce.bin", 0, 1, 1);
    mAnimsB[3]->mCharBase = screen0.func_020276f4(0x4000);
    mAnimsB[3]->mUnk_11   = 14;
    ChangeDir("/Game/Battle/Obj/");
    screen0.LoadCharacters("select_ch_u_obj_00.nce_ncg1D.bin", 0x1400, 0);
    screen0.LoadPalette("select_ch_u_obj_00_ncl.bin", 0x40, 0);
    mAnimsB[0] = new CellAnimation("select_ch_u_obj_00_nce.bin", 0, 1, 1);
    mAnimsB[0]->mCharBase = screen0.func_020276f4(0x1400);
    mAnimsB[0]->mUnk_11   = 2;
    mAnimsB[4] = new CellAnimation("select_ch_u_obj_00_nce.bin", 2, 1, 1);
    mAnimsB[4]->mCharBase = screen0.func_020276f4(0x1400);
    mAnimsB[4]->mUnk_11   = 2;
    mIcons = new UnkStruct_020651bc(0x5000, 10, 0);
    u8 i = 0;
    do {
        BattlePlayer *player = data_020d433c.mBattlePlayers->mPlayers[i];
        mIcons->func_02065204(i, (s8) player->mUnk_f6, player->NameInfo(), 0);
        s16 y = i * 0x28 + 0x25;
        mIcons->func_0206547c(i, 0x22, y + 2, 0);
        i++;
    } while (i < data_020d433c.mPlayerCount);
    ChangeDir("/Game/Character/Icon/");
    screen0.LoadCharacters("icon_ch_base.nce_ncg1D.bin", 0, 0);
    screen0.LoadPalette("icon_ch_base_ncl.bin", 0, 0);
    mAnimsB[1] = new CellAnimation("icon_ch_base_nce.bin", 7, 1, 1);
    mAnimsB[1]->mCharBase = screen0.func_020276f4(0);
    mAnimsB[1]->mUnk_11   = 0;
    mAnimsB[2] = new CellAnimation("icon_ch_base_nce.bin", 0, 1, 1);
    mAnimsB[2]->mCharBase = screen0.func_020276f4(0);
    mAnimsB[2]->mUnk_11   = 0;
    MessageFile *file = (MessageFile *) func_02023504("/font/mes_BattleArenaMenu.mes", 0, 0, 0, 0, 0);
    TextManager::Get().func_0202ce44(0, 0, 0, 0);
    void *map = OS_Alloc(TextManager::Get().GlyphCount(0x600));
    MI_CpuClearFast(map, 0x600);
    TextManager::Get().func_0202cd48(0, (u16 *) map, 0, 0, 0x20, 0x18, 0x20);
    func_02025c1c(0, 8, 0, map, 0x600, 1);
    void *buffer = OS_Alloc(TextManager::Get().BufferSize(0x20, 0x18));
    MI_CpuClearFast(buffer, 0x6000);
    TextManager::Get().mUnk_5b = 1;
    u8 j = 0;
    do {
        s16 x = 0x48;
        u32 base = j * 0x28;
        s16 y = base + 0x2b;
        TextManager::Get().Print(data_020d433c.mBattlePlayers->mPlayers[j]->NameText(), data_020d433c.mBattlePlayers->mPlayers[j]->mUnk_ed, buffer, 0x20, 0x18, &x, &y, 0, 0, 2, 0, 0x10, 0, 0, 0, 1, 1);
        x = 0x80;
        u32 base2 = j * 0x28 + 0x35;
        y = base2;
        TextManager &t2 = TextManager::Get();
        s16 x2;
        s16 y2;
        y2 = y;
        x2 = x;
        t2.Print(file->GetText(10), file->GetLength(10), buffer, 0x20, 0x18, &x2, &y2, 0, 0, 2, 0, 0x10, 0, 0, 0, 1, 1);
        x = 0xd0;
        y = base2;
        TextManager::Get().PrintNumber(data_020d433c.mUnk_a9[j], 2, 0, buffer, 0x20, 0x18, &x, &y, 0, 2, 0, 0x10, 0, 1, 1);
        j++;
    } while (j < data_020d433c.mPlayerCount);
    TextManager::Get().mUnk_5b = 0;
    func_02025c1c(0, 12, 0, buffer, 0x6000, 1);
    mUnk_50[2] = TextManager::Get().BufferSize(0x18, 4);
    mUnk_38[2] = OS_Alloc(mUnk_50[2]);
    MI_CpuClear16(mUnk_38[2], mUnk_50[2]);
    mUnk_5c[2] = TextManager::Get().GlyphCount(0x18);
    mUnk_44[2] = OS_Alloc((s16) mUnk_5c[2] * (s16) 0xc);
    TextManager::Get().Init(mUnk_44[2], 1, 0, 0x18, 4, 0x11);
    TextManager &text = TextManager::Get();
    s16 height;
    s16 width;
    width  = 0;
    height = 0;
    text.Print(file->GetText(13), file->GetLength(13), mUnk_38[2], 0x18, 4, &height, &width, 0, 0, 2, 0x11, 0xe, 0, 0, 1, 1, 1);
    func_02025c1c(SpriteManager::Get().Screen(1).mBgIndex, 4, 0x7000, mUnk_38[2], mUnk_50[2], 0);
    OS_Free(file);
    ChangeDir("/Game/2D/union_b/");
    func_02025b64(1, 1, 0, "union_b_bg_window_ncl.bin", 0, 0, 0);
    func_02025b64(1, 11, 0, "union_b_bg_window_ncg.bin", 0, 0, 0);
    func_02025b64(1, 7, 0, "union_b_bg_window_b_nsc.bin", 0, 0, 0);
    SpriteScreen &screen1b = SpriteManager::Get().Screen(1);
    screen1b.LoadCharacters("union_b_obj_window.nce_ncg1D.bin", 0x5000, 0);
    screen1b.LoadPalette("union_b_obj_window_ncl.bin", 0x40, 0);
    ChangeDir("/");
    mMessage = new MessageWindow("mes_BattleArenaMenu", 1, 0x4000);
    mMessage->func_02065bd4(0x14, 4, 0);
    mMessage->func_02065be4(0xd8);
    mMessage->func_02065890(9, 0, 1);
    if (data_020d433c.mUnk_a8 != 0) {
        WirelessManager::Get();
        if (data_020c4230.mUnk_20 != 0) {
            mMessage->func_02065890(4, 1, 1);
        }
    }
}

void BattleResult::func_ov007_0224f8f4() {
    SpriteManager::Get().Screen(1).func_02027778(mUnk_44[2], mUnk_5c[2], 0x20, 0x38, 0, 0xe0, 0xf, 0);
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
    mUnk_14 = (MessageFile *) func_02023504("mes_result.mes", 0, 0, 0, 0, 0);
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

// non-matching: the loop prologue sets the counter and its zero copy (r9, r5) after the clamp; the original sets
// them before it and loads the pool constants one slot later (6 words).
void BattleResult::func_ov007_0225047c() {
    if (mUnk_38[0]) {
        MI_CpuClear16(mUnk_38[0], mUnk_50[0]);
    }
    s16 x = 0;
    s16 y = 0;
    s16 ly;
    TextManager::Get().mUnk_5b = 1;
    x = 0x94;
    y = 0;
    TextManager::Get().PrintNumber(mUnk_9c, 7, 0, mUnk_38[0], 0x20, 0x16, &x, &y, 0, 2, 0x21, 0, 1, 1, 1);
    {
        TextManager &t2 = TextManager::Get();
        s16 x2;
        s16 y2;
        y2 = y;
        x2 = x;
        t2.Print(mUnk_14->GetText(2), mUnk_14->GetLength(2), mUnk_38[0], 0x20, 0x16, &x2, &y2, 0, 0, 2, 0x21, 0, 0, 0, 1, 1, 1);
    }
    {
        TextManager &t3 = TextManager::Get();
        s16 x3;
        s16 y3;
        y3 = y;
        x3 = 0;
        t3.Print(mUnk_14->GetText(7), mUnk_14->GetLength(7), mUnk_38[0], 0x20, 0x16, &x3, &y3, 0, 0, 2, 0x21, 0, 0, 0, 1, 1, 1);
    }
    if (mUnk_b2 >= 2) {
        u8 n = mUnk_b2 - 2;
        if (n > data_020d433c.mUnk_99) {
            n = data_020d433c.mUnk_99;
        }
        u8 i = 0;
        do {
            if (i != 0 && mUnk_84[i] == 0) {
                break;
            }
            x = 0x94;
            if (i < 3 && mUnk_84[3] != 0) {
                x -= 0x3c;
            }
            ly = (i % 3) * 16 + 16;
            TextManager::Get().PrintNumber(mUnk_84[i], 7, 0, mUnk_38[0], 0x20, 0x16, &x, &ly, 0, 2, 0x21, 0, 1, 1, 1);
            TextManager &t4 = TextManager::Get();
            s16 x2;
            s16 y2;
            y2 = ly;
            x2 = x;
            t4.Print(mUnk_14->GetText(2), mUnk_14->GetLength(2), mUnk_38[0], 0x20, 0x16, &x2, &y2, 0, 0, 2, 0x21, 0, 0, 0, 1, 1, 1);
            i++;
        } while (i < n);
        {
            TextManager &t5 = TextManager::Get();
            s16 a;
            s16 b;
            b = 0x10;
            a = 0;
            t5.Print(mUnk_14->GetText(8), mUnk_14->GetLength(8), mUnk_38[0], 0x20, 0x16, &a, &b, 0, 0, 2, 0x21, 0xe, 0, 0, 1, 1, 1);
        }
        {
            TextManager &t6 = TextManager::Get();
            s16 a;
            s16 b;
            b = 0x20;
            a = 8;
            t6.Print(mUnk_14->GetText(9), mUnk_14->GetLength(9), mUnk_38[0], 0x20, 0x16, &a, &b, 0, 0, 2, 0x21, 0xe, 0, 0, 1, 1, 1);
        }
        {
            TextManager &t7 = TextManager::Get();
            s16 a;
            s16 b;
            b = 0x30;
            a = 0x10;
            t7.Print(mUnk_14->GetText(10), mUnk_14->GetLength(10), mUnk_38[0], 0x20, 0x16, &a, &b, 0, 0, 2, 0x21, 0xe, 0, 0, 1, 1, 1);
        }
    }
    if (mUnk_b2 >= 9) {
        x = 0x94;
        y = 0x40;
        TextManager::Get().PrintNumber(100, 7, 0, mUnk_38[0], 0x20, 0x16, &x, &y, 0, 2, 0x21, 0, 1, 1, 1);
        {
            TextManager &t8 = TextManager::Get();
            s16 x2;
            s16 y2;
            y2 = y;
            x2 = x;
            t8.Print(mUnk_14->GetText(3), mUnk_14->GetLength(3), mUnk_38[0], 0x20, 0x16, &x2, &y2, 0, 0, 2, 0x21, 0, 0, 0, 1, 1, 1);
        }
        x += 0x10;
        TextManager::Get().PrintNumber(mUnk_ad, 0, 0, mUnk_38[0], 0x20, 0x16, &x, &y, 0, 2, 0x21, 0, 1, 1, 1);
        {
            TextManager &t9 = TextManager::Get();
            s16 x3;
            s16 y3;
            y3 = y;
            x3 = 0;
            t9.Print(mUnk_14->GetText(11), mUnk_14->GetLength(11), mUnk_38[0], 0x20, 0x16, &x3, &y3, 0, 0, 2, 0x21, 0, 0, 0, 1, 1, 1);
        }
    }
    if (mUnk_b2 >= 0xb) {
        x = 0x94;
        y = 0x50;
        TextManager::Get().PrintNumber(100, 7, 0, mUnk_38[0], 0x20, 0x16, &x, &y, 0, 2, 0x21, 0, 1, 1, 1);
        {
            TextManager &t10 = TextManager::Get();
            s16 x2;
            s16 y2;
            y2 = y;
            x2 = x;
            t10.Print(mUnk_14->GetText(3), mUnk_14->GetLength(3), mUnk_38[0], 0x20, 0x16, &x2, &y2, 0, 0, 2, 0x21, 0, 0, 0, 1, 1, 1);
        }
        x += 0x10;
        TextManager::Get().PrintNumber(mUnk_ae, 0, 0, mUnk_38[0], 0x20, 0x16, &x, &y, 0, 2, 0x21, 0, 1, 1, 1);
    }
    if (mUnk_b2 >= 0xc) {
        x = 0x94;
        y = 0x60;
        TextManager::Get().PrintNumber(mUnk_a0, 7, 0, mUnk_38[0], 0x20, 0x16, &x, &y, 0, 2, 0x21, 0, 1, 1, 1);
        {
            TextManager &t11 = TextManager::Get();
            s16 x2;
            s16 y2;
            y2 = y;
            x2 = x;
            t11.Print(mUnk_14->GetText(2), mUnk_14->GetLength(2), mUnk_38[0], 0x20, 0x16, &x2, &y2, 0, 0, 2, 0x21, 0, 0, 0, 1, 1, 1);
        }
        x = 0x94;
        y = 0x60;
        {
            TextManager &t12 = TextManager::Get();
            s16 x3;
            s16 y3;
            y3 = y;
            x3 = 0;
            t12.Print(mUnk_14->GetText(12), mUnk_14->GetLength(12), mUnk_38[0], 0x20, 0x16, &x3, &y3, 0, 0, 2, 0x21, 0, 0, 0, 1, 1, 1);
        }
    }
    if (mUnk_b2 >= 0xe) {
        if (mUnk_ac) {
            x = 0xc4;
            y = 0x70;
            TextManager &t13 = TextManager::Get();
            s16 x2;
            s16 y2;
            y2 = y;
            x2 = x;
            t13.Print(mUnk_14->GetText(4), mUnk_14->GetLength(4), mUnk_38[0], 0x20, 0x16, &x2, &y2, 0, 0, 2, 0x21, 0, 0, 0, 1, 1, 1);
        }
        x = 0xa2;
        y = 0x70;
        TextManager &t14 = TextManager::Get();
        s16 x3;
        s16 y3;
        y3 = y;
        x3 = 0;
        t14.Print(mUnk_14->GetText(13), mUnk_14->GetLength(13), mUnk_38[0], 0x20, 0x16, &x3, &y3, 0, 0, 2, 0x21, 0, 0, 0, 1, 1, 1);
    }
    {
        TextManager &t15 = TextManager::Get();
        s16 a;
        s16 b;
        b = 0x80;
        a = 0xcc;
        t15.Print(mUnk_14->GetText(2), mUnk_14->GetLength(2), mUnk_38[0], 0x20, 0x16, &a, &b, 0, 0, 2, 0x21, 0, 0, 0, 1, 1, 1);
    }
    {
        TextManager &t16 = TextManager::Get();
        s16 a;
        s16 b;
        b = 0x80;
        a = 0;
        t16.Print(mUnk_14->GetText(14), mUnk_14->GetLength(14), mUnk_38[0], 0x20, 0x16, &a, &b, 0, 0, 2, 0x21, 0, 0, 0, 1, 1, 1);
    }
    {
        TextManager &t17 = TextManager::Get();
        s16 a;
        s16 b;
        b = 0x9e;
        a = 0xcc;
        t17.Print(mUnk_14->GetText(2), mUnk_14->GetLength(2), mUnk_38[0], 0x20, 0x16, &a, &b, 0, 0, 2, 0x21, 0, 0, 0, 1, 1, 1);
    }
    {
        TextManager &t18 = TextManager::Get();
        s16 a;
        s16 b;
        b = 0x9e;
        a = 0;
        t18.Print(mUnk_14->GetText(15), mUnk_14->GetLength(15), mUnk_38[0], 0x20, 0x16, &a, &b, 0, 0, 2, 0x21, 0, 0, 0, 1, 1, 1);
    }
    TextManager::Get().mUnk_5b = 0;
    func_02005e9c(mUnk_38[0], mUnk_50[0]);
    func_02025c1c(SpriteManager::Get().Screen(0).mBgIndex, 4, 0x8000, mUnk_38[0], mUnk_50[0], 0);
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
