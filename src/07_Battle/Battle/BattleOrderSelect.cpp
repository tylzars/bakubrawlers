#include "Game/Random.hpp"
#include "Game/UnkStruct_020d433c.hpp"
#include "Graphics/CellAnimation.hpp"
#include "Graphics/Display.hpp"
#include "Graphics/SpriteManager.hpp"
#include "Scene/SceneBase.hpp"
#include "System/FileSystem.hpp"
#include "System/SoundManager.hpp"
#include "System/TouchManager.hpp"
#include "System/WirelessManager.hpp"
#include "UI/MessageWindow.hpp"
#include "Unknown/UnkStruct_02078290.hpp"
#include "Unknown/UnkStruct_020651bc.hpp"
#include <nitro/gx.h>
#include <nitro/math.h>
#include <nitro/mi.h>

u32 func_ov007_022631bc(BattlePlayerList *list, u32 index);
void func_ov007_02262a70(BattlePlayerList *list, u32 a, u32 b);

struct OrderItem {
    /* 00 */ CellAnimation *mAnim;
    /* 04 */ s16 mX;
    /* 06 */ s16 mY;
    /* 08 */ u8 mUnk_08;
    /* 09 */ u8 mUnk_09;
    /* 0a */ PAD(0x0a, 0x0c);
    /* 0c */
};

// Order selection of the battle (select_order_*): each player picks one of the numbered slots.
class BattleOrderSelect : public SceneBase {
public:
    /* 0c */ PAD(0x0c, 0x10);
    /* 10 */ OrderItem mA[2];
    /* 28 */ OrderItem mB[3];
    /* 4c */ OrderItem mC[8];
    /* ac */ UnkStruct_020651bc *mIcons;
    /* b0 */ MessageWindow *mMessage;
    /* b4 */ s8 mUnk_b4[4];
    /* b8 */ u8 mUnk_b8[4];
    /* bc */ u8 mUnk_bc[4];
    /* c0 */ s8 mUnk_c0[4];
    /* c4 */ u8 mUnk_c4[4];
    /* c8 */ u8 mUnk_c8[4];
    /* cc */ u8 mState;
    /* cd */ u8 mTimer;
    /* ce */ u8 mCount;
    /* cf */ u8 mNeeded;
    /* d0 */

    BattleOrderSelect();
    virtual ~BattleOrderSelect();
    virtual void Update();
    BOOL func_ov007_0224b710(u32 index);
    void func_ov007_0224b7bc();
    void func_ov007_0224b914();
    void func_ov007_0224b95c();
    void func_ov007_0224ba0c();
    void func_ov007_0224bbf8();
    void func_ov007_0224c368(u8 timer);
    BOOL func_ov007_0224c3d4();
    BOOL func_ov007_0224c720();
    void func_ov007_0224c810();
    BOOL func_ov007_0224cb20(int x, int y, int rx, s16 ry, u8 w, u8 h);
};

BattleOrderSelect::BattleOrderSelect()
    : SceneBase(0x3e8, 0) {
    mIcons   = 0;
    mMessage = 0;
    mState   = 0;
    mTimer   = 0;
    u8 n     = data_020d433c.mPlayerCount;
    if (n > 4) {
        n = 4;
    } else if (n < 1) {
        n = 1;
    }
    mCount = n;
    u8 i   = 0;
    do {
        mUnk_b4[i] = data_020d433c.mBattlePlayers->mPlayers[i]->mUnk_f6;
        i++;
    } while (i < mCount);
    if (mCount < 4) {
        do {
            mUnk_b4[i] = -1;
            i++;
        } while (i < 4);
    }
    MI_CpuClear32(mA, sizeof(mA));
    MI_CpuClear32(mB, sizeof(mB));
    MI_CpuClear32(mC, sizeof(mC));
    i = 0;
    do {
        mUnk_b8[i] = 0;
        mUnk_bc[i] = 0;
        mUnk_c0[i] = -1;
        mUnk_c4[i] = 0;
        mUnk_c8[i] = 8;
        i++;
    } while (i < mCount);
    WirelessManager::Get();
    if (IsWirelessMode(&data_020c4030.mUnk_228)) {
        mNeeded = WirelessManager::Get().func_0202f134();
    } else {
        mNeeded = 1;
    }
    data_0220f540.mUnk_00 = 0xf;
}

BattleOrderSelect::~BattleOrderSelect() {
    u8 i = 0;
    do {
        CellAnimation *anim = mA[i].mAnim;
        if (anim) {
            delete anim;
        }
        i++;
    } while (i < 2);
    i = 0;
    do {
        CellAnimation *anim = mB[i].mAnim;
        if (anim) {
            delete anim;
        }
        i++;
    } while (i < 3);
    i = 0;
    do {
        CellAnimation *anim = mC[i].mAnim;
        if (anim) {
            delete anim;
        }
        i++;
    } while (i < 8);
    if (mIcons) {
        delete mIcons;
    }
    if (mMessage) {
        delete mMessage;
    }
}

void BattleOrderSelect::Update() {
    func_ov007_0224b7bc();
    switch (mState) {
        case 0:
            func_ov007_0224ba0c();
            func_ov007_0224bbf8();
            func_ov007_0224b95c();
            SpriteManager::Get().StartFade(2);
            mState++;
            break;
        case 1:
            if (!SpriteManager::Get().IsFading()) {
                mTimer = 0;
                mState++;
            }
            break;
        case 2:
            if (func_ov007_0224b710(0xf)) {
                mState++;
            }
            break;
        case 3:
            mTimer++;
            if (mTimer >= 0x1e) {
                mMessage->func_02065890(0x1d, 0, 1);
                mState++;
            }
            func_ov007_0224c368(mTimer);
            break;
        case 4:
            if (func_ov007_0224c3d4()) {
                if (mCount - mNeeded != 0) {
                    mTimer = 0x1e;
                    mState = 5;
                } else {
                    mTimer = 0x5a;
                    mState = 6;
                }
            }
            break;
        case 5:
            mTimer--;
            if (mTimer == 0) {
                if (func_ov007_0224c720()) {
                    mTimer = 0x5a;
                    mState++;
                } else {
                    mTimer = 0xf;
                }
            }
            break;
        case 6:
            mTimer--;
            if (mTimer == 0) {
                func_ov007_0224b914();
                mState++;
            }
            break;
        case 7:
            SpriteManager::Get().StartFade(3);
            mState++;
            break;
        case 8:
            if (!SpriteManager::Get().IsFading()) {
                mState++;
            }
            break;
        case 9:
            if (func_ov007_0224b710(0x10)) {
                mState++;
            }
            break;
        case 10:
            RequestScene(1);
            mFinished = 1;
            return;
    }
    func_ov007_0224c810();
}

BOOL BattleOrderSelect::func_ov007_0224b710(u32 index) {
    WirelessManager::Get();
    if (IsWirelessMode(&data_020c4030.mUnk_228)) {
        return func_ov007_022631bc(data_020d433c.mBattlePlayers, index) != 0;
    }
    return 1;
}

void BattleOrderSelect::func_ov007_0224b7bc() {
    WirelessManager::Get();
    if (IsWirelessMode(&data_020c4030.mUnk_228)) {
        data_0220f540.mUnk_00 = 10;
        data_0220f540.mTouchDown = TouchManager::Get().mUnk_57;
        data_0220f540.mTouchX    = TouchManager::Get().mX;
        data_0220f540.mTouchY    = TouchManager::Get().mY;
    }
}

void BattleOrderSelect::func_ov007_0224b914() {
    u8 i = 0;
    do {
        func_ov007_02262a70(data_020d433c.mBattlePlayers, mUnk_bc[mUnk_c0[i]], i);
        i++;
    } while (i < mCount);
}

void BattleOrderSelect::func_ov007_0224b95c() {
    u8 i = 0;
    do {
        mUnk_bc[i] = MATH_Rand32(&data_020d42e8, mCount);
        if (i != 0) {
            u8 same = 0;
            u8 j    = 0;
            u8 v    = mUnk_bc[i];
            do {
                if (v == mUnk_bc[j]) {
                    same = 1;
                }
                j++;
            } while (j < i);
            if (same != 0) {
                continue;
            }
        }
        i++;
    } while (i < mCount);
}

void BattleOrderSelect::func_ov007_0224ba0c() {
    GX_SetDispSelect(1);
    SpriteManager::Get().func_02026dec(1, 1);
    func_02001e68(1, 0, 0);
    func_02001ed0(0);
    G2_SetBG0Control(0, 1, 0, 1, 0);
    G2S_SetBG0Control(1, 1, 0, 4, 0);
    G2S_SetBG1Control(1, 1, 2, 4, 0);
    G2S_SetBG2ControlText(0, 1, 4, 1);
    G2_SetBG0Offset(0, 0);
    G2S_SetBG0Offset(0, 0);
    G2S_SetBG1Offset(0, 0);
    G2S_SetBG2Offset(0, 0);
    G2_SetBG0Priority(3);
    G2S_SetBG0Priority(0);
    G2S_SetBG1Priority(0);
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

// non-matching: register allocation only: the original keeps `this` in r10 and the loop counter/constants in r4-r9; same instruction sequence otherwise (29 words differ)
void BattleOrderSelect::func_ov007_0224bbf8() {
    ChangeDir("/Game/Battle/Bg/");
    func_02025b64(0, 1, 0, "select_order_u_bg_00_ncl.bin", 0, 0, 0);
    func_02025b64(0, 11, 0, "select_order_u_bg_00_ncg.bin", 0, 0, 0);
    func_02025b64(0, 7, 0, "select_order_u_bg_00_nsc.bin", 0, 0, 0);
    func_02025b64(1, 1, 0, "select_order_b_bg_01_ncl.bin", 0, 0, 0);
    func_02025b64(1, 11, 0, "select_order_b_bg_01_ncg.bin", 0, 0, 0);
    func_02025b64(1, 7, 0, "select_order_b_bg_01_a_nsc.bin", 0, 0, 0);
    func_02025b64(1, 1, 0x2000, "select_order_b_bg_01_ncl.bin", 0, 0, 0);
    func_02025b64(1, 8, 0, "select_order_b_bg_01_b_nsc.bin", 0, 0, 0);
    func_02025b64(1, 1, 0x4000, "select_order_b_bg_00_ncl.bin", 0, 0, 0);
    func_02025b64(1, 13, 0, "select_order_b_bg_00_ncg.bin", 0, 0, 0);
    func_02025b64(1, 9, 0, "select_order_b_bg_00_nsc.bin", 0, 0, 0);
    SpriteScreen &screen0 = SpriteManager::Get().Screen(0);
    ChangeDir("/Game/Character/Icon/");
    screen0.LoadCharacters("icon_ch_base.nce_ncg1D.bin", 0xb000, 0);
    screen0.LoadPalette("icon_ch_base_ncl.bin", 0x160, 0);
    u8 i = 0;
    do {
        CellAnimation *anim = new CellAnimation("icon_ch_base_nce.bin", 0, 1, 1);
        mA[i].mAnim         = anim;
        mA[i].mAnim->mCharBase = screen0.func_020276f4(0xb000);
        mA[i].mAnim->mUnk_11   = 11;
        mA[i].mX               = 0;
        mA[i].mY               = 0;
        mA[i].mUnk_08          = 0;
        i++;
    } while (i < 2);
    mA[0].mAnim->SetAnimation(7);
    mA[1].mAnim->SetAnimation(0);
    ChangeDir("/Game/Battle/Obj/");
    SpriteScreen &screen1 = SpriteManager::Get().Screen(1);
    screen0.LoadCharacters("select_order_b_obj_00.nce_ncg1D.bin", 0, 0);
    screen0.LoadPalette("select_order_b_obj_00_ncl.bin", 0, 0);
    screen1.LoadCharacters("select_order_b_obj_00.nce_ncg1D.bin", 0, 0);
    screen1.LoadPalette("select_order_b_obj_00_ncl.bin", 0, 0);
    func_ov007_0224b95c();
    i = 0;
    do {
        u32 a                  = i & 0xff;
        mC[a].mAnim            = new CellAnimation("select_order_b_obj_00_nce.bin", 0, 1, 1);
        mC[a].mAnim->mCharBase = screen1.func_020276f4(0);
        mC[a].mAnim->mUnk_11   = 0;
        mC[a].mX               = 0x84 - mCount * 0x20 + i * 0x40;
        mC[a].mY               = 0x60;
        mC[a].mUnk_08          = 0;
        u32 n                  = i + 4;
        mC[a].mAnim->SetAnimation(n);
        u32 b                  = n & 0xff;
        mC[b].mAnim            = new CellAnimation("select_order_b_obj_00_nce.bin", 0, 1, 1);
        mC[b].mAnim->mCharBase = screen1.func_020276f4(0);
        mC[b].mAnim->mUnk_11   = 0;
        mC[b].mX               = 0x84 - mCount * 0x20 + i * 0x40;
        mC[b].mY               = 0x40;
        mC[b].mUnk_08          = 2;
        mC[b].mAnim->SetAnimation(mUnk_bc[i]);
        i++;
    } while (i < mCount);
    ChangeDir("/Game/2D/union_b/");
    screen0.LoadCharacters("union_b_obj_you.nce_ncg1D.bin", 0x10000, 0);
    screen0.LoadPalette("union_b_obj_you_ncl.bin", 0x140, 0);
    mB[0].mAnim            = new CellAnimation("union_b_obj_you_nce.bin", 3, 1, 1);
    mB[0].mAnim->mCharBase = screen0.func_020276f4(0x10000);
    mB[0].mAnim->mUnk_11   = 10;
    mB[0].mUnk_09          = 0;
    mB[0].mAnim->mUnk_0f   = data_020d433c.mBattlePlayers->mUnk_2a9;
    mB[1].mAnim            = new CellAnimation("union_b_obj_you_nce.bin", 2, 1, 1);
    mB[1].mAnim->mCharBase = screen0.func_020276f4(0x10000);
    mB[1].mAnim->mUnk_11   = 10;
    mB[1].mUnk_09          = 0;
    mB[2].mAnim            = new CellAnimation("union_b_obj_you_nce.bin", 4, 1, 1);
    mB[2].mAnim->mCharBase = screen0.func_020276f4(0x10000);
    mB[2].mAnim->mUnk_11   = 10;
    mB[2].mUnk_09          = 0;
    ChangeDir("/");
    mIcons = new UnkStruct_020651bc(0xa000, 6, 0);
    i      = 0;
    do {
        mIcons->func_02065204(i, mUnk_b4[i], data_020d433c.mBattlePlayers->mPlayers[i]->NameInfo(), 0);
        mIcons->func_020654f8(i, 0);
        i++;
    } while (i < mCount);
    mMessage = new MessageWindow("mes_Command", 1, 0x10000);
    mMessage->func_02065bd4(0x14, 4, 1);
    mMessage->func_02065be4(0xd8);
    UnkStruct_02078290 *unk = data_020d433c.mUnk_10;
    if (unk) {
        unk->func_020785d8(6);
    }
}

void BattleOrderSelect::func_ov007_0224c368(u8 timer) {
    int v = (timer * timer * 0xa0) / 900;
    if (v > 0xa0) {
        v = 0xa0;
    } else if (v < 0) {
        v = 0;
    }
    s16 h = v;
    G2S_SetBG0Offset(h, 0);
    G2S_SetBG1Offset(-h, 0);
}

BOOL BattleOrderSelect::func_ov007_0224c3d4() {
    u8 done = 0;
    int hit = 0;
    u8 i = 0;
    do {
        if (mUnk_c0[i] >= 0) {
            done++;
        } else {
            u8 down;
            u8 x;
            u8 y;
            WirelessManager::Get();
            if (IsWirelessMode(&data_020c4258)) {
                BattlePlayer *p = data_020d433c.mBattlePlayers->mPlayers[i];
                down            = p->mMsg.mTouchDown;
                x               = p->mMsg.mTouchX;
                y               = p->mMsg.mTouchY;
            } else {
                down = TouchManager::Get().mUnk_57;
                x    = TouchManager::Get().mX;
                y    = TouchManager::Get().mY;
            }
            if (down) {
                u8 j = 0;
                do {
                    if (func_ov007_0224cb20(x, y, mC[(u8) (j + 4)].mX, mC[(u8) (j + 4)].mY, 0x38, 0x48)) {
                        if (mUnk_b8[j] != 0) {
                            SoundManager::Get().func_020246f8(2);
                        } else {
                            mUnk_b8[j] = 1;
                            mUnk_c0[i] = j;
                            mC[(u8) (j + 4)].mAnim->SetAnimation(mUnk_bc[mUnk_c0[i]] + 8);
                            if (i == data_020d433c.mBattlePlayers->mUnk_2a9) {
                                hit = 1;
                            }
                            SoundManager::Get().func_020246f8(0);
                            done++;
                            break;
                        }
                    }
                    j++;
                } while (j < mCount);
            }
        }
        i++;
    } while (i < mCount);
    if (done >= mNeeded) {
        return 1;
    }
    if (hit) {
        mMessage->func_02065890(0x1e, 1, 1);
    }
    return 0;
}

BOOL BattleOrderSelect::func_ov007_0224c720() {
    u8 i = 0;
    do {
        if (mUnk_c0[i] < 0) {
            u8 j = 0;
            do {
                if (mUnk_b8[j] == 0) {
                    mUnk_b8[j] = 1;
                    mUnk_c0[i] = j;
                    mC[(u8) (j + 4)].mAnim->SetAnimation(mUnk_bc[mUnk_c0[i]] + 8);
                    SoundManager::Get().func_020246f8(0);
                    return 0;
                }
                j++;
            } while (j < mCount);
        }
        i++;
    } while (i < mCount);
    return 1;
}

// non-matching: register allocation only: the original uses r5 for `this` and r4 for the item pointer, r6 for this+i and r7 for &data_020d433c (mine r4/r5/r5/r6); same instruction sequence (50 words differ)
void BattleOrderSelect::func_ov007_0224c810() {
    u8 i = 0;
    do {
        mC[(u8) (i + 4)].mAnim->Update();
        mC[(u8) (i + 4)].mAnim->Draw(1, mC[(u8) (i + 4)].mX, mC[(u8) (i + 4)].mY, mC[(u8) (i + 4)].mUnk_08);
        mC[i & 0xff].mAnim->Update();
        mC[i & 0xff].mAnim->Draw(0, mC[i & 0xff].mX, (s16) (mC[i & 0xff].mY + mUnk_c4[i]), mC[i & 0xff].mUnk_08);
        if (mUnk_c0[i] >= 0) {
            u8 slot = mUnk_bc[mUnk_c0[i]];
            if (mUnk_c8[i] != 0) {
                mUnk_c8[i]--;
            } else {
                if (mUnk_c4[slot] < 0x10) {
                    mUnk_c4[slot]++;
                }
                s16 v  = mUnk_c4[slot];
                s8 off = v * v / 8 - 0x2c;
                u8 p   = data_020d433c.mBattlePlayers->mUnk_2a9;
                if (i == p) {
                    mB[0].mAnim->Draw(0, (s16) (mC[slot].mX + 0xc), (s16) (mC[slot].mY - 0xe - off), 2);
                } else {
                    bool flag = false;
                    if (data_020d433c.mBattlePlayers->mPlayers[i]->mUnk_c8) {
                        flag = true;
                    }
                    if (flag == 1) {
                        mB[1].mAnim->func_0202a3ec(0, i, mC[slot].mX + 0x1b, mC[slot].mY - 0x31 - off, 2);
                    }
                    if ((data_020d433c.mUnk_98 == 1) & (i == data_020d433c.mUnk_9b[p])) {
                        mB[2].mAnim->func_0202a3ec(0, i, mC[slot].mX + 0x10, mC[slot].mY - 0x38 - off, 2);
                    }
                }
                mIcons->func_020654f8(i, 1);
                mIcons->func_0206547c(i, mC[slot].mX + 0x10, mC[slot].mY - 0x28 - off, 2);
                mA[0].mAnim->SetAnimation(i + 7);
                mA[0].mAnim->Draw(0, (s16) (mC[slot].mX + 0xc), (s16) (mC[slot].mY - 0x2c - off), 2);
                mA[1].mAnim->Draw(0, (s16) (mC[slot].mX + 0xe), (s16) (mC[slot].mY - 0x2a - off), 3);
            }
        }
        i++;
    } while (i < mCount);
    mMessage->func_020658d0();
    mIcons->func_020654a0();
}

// non-matching: the original reads the s16 stack argument twice (ldrsh at entry and again before the bottom edge) and stays within r0-r3/ip (no push); mine keeps it in ip and pushes lr (20 words differ)
BOOL BattleOrderSelect::func_ov007_0224cb20(int x, int y, int rx, s16 ry, u8 w, u8 h) {
    if ((x >= rx) & (y >= ry)) {
        if ((x < rx + w) & (y < ry + h)) {
            return 1;
        }
    }
    return 0;
}
