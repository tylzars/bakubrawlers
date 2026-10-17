#include "System/TouchManager.hpp"
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
#include "Graphics/AntennaIndicator.hpp"
#include "Graphics/CharacterIcons.hpp"
#include <nitro/gx.h>
#include <nitro/math.h>
#include <nitro/mi.h>

u32 SetMessageTypeAndCheckAll(BattlePlayerList *list, u32 index);

struct OrderItem {
    /* 00 */ CellAnimation *mCellAnim;
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
    /* ac */ CharacterIcons *mIcons;
    /* b0 */ MessageWindow *mMessageWindow;
    /* b4 */ s8 mCharacterIds[4];
    /* b8 */ u8 mItemTaken[4];
    /* bc */ u8 mItemOrder[4]; // index = item, value = the turn-order slot SetTurnOrder gets
    /* c0 */ s8 mPlayerItemIndex[4];
    /* c4 */ u8 mUnk_c4[4];
    /* c8 */ u8 mIconTimers[4];
    /* cc */ u8 mState;
    /* cd */ u8 mTimer;
    /* ce */ u8 mCount;
    /* cf */ u8 mNeeded;
    /* d0 */

    BattleOrderSelect();
    virtual ~BattleOrderSelect();
    virtual void Update();
    BOOL SetAndCheckMessageType(u32 index); // same result as SetMessageTypeAndCheckAll
    void WriteTouchMessage();
    void ApplyTurnOrder();
    void ShuffleOrder();
    void InitDisplay();
    // Also ShuffleOrder (Update shuffles again right after), CharacterIcons mIcons (hidden), mMessageWindow
    // (mes_Command) and SetupLayout(6) of data_020d433c.mAntenna when set.
    void LoadGraphics();
    void SetBgSlide(u8 timer);
    BOOL UpdateTouchSelection();
    BOOL AssignUntakenItem(); // first player with no item gets the first free one (FALSE); TRUE when all have one
    void Draw();
    BOOL IsInRect(int x, int y, int rx, s16 ry, u8 w, u8 h);
};

BattleOrderSelect::BattleOrderSelect()
    : SceneBase(0x3e8, 0) {
    mIcons   = 0;
    mMessageWindow = 0;
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
        mCharacterIds[i] = data_020d433c.mBattlePlayers->mPlayers[i]->mCharacterId;
        i++;
    } while (i < mCount);
    if (mCount < 4) {
        do {
            mCharacterIds[i] = -1;
            i++;
        } while (i < 4);
    }
    MI_CpuClear32(mA, sizeof(mA));
    MI_CpuClear32(mB, sizeof(mB));
    MI_CpuClear32(mC, sizeof(mC));
    i = 0;
    do {
        mItemTaken[i] = 0;
        mItemOrder[i] = 0;
        mPlayerItemIndex[i] = -1;
        mUnk_c4[i] = 0;
        mIconTimers[i] = 8;
        i++;
    } while (i < mCount);
    WirelessManager::Get();
    if (IsWirelessMode(&data_020c4030.mUnk_228)) {
        mNeeded = WirelessManager::Get().GetRecvBlockCount();
    } else {
        mNeeded = 1;
    }
    data_0220f540.mUnk_00 = 0xf;
}

BattleOrderSelect::~BattleOrderSelect() {
    u8 i = 0;
    do {
        CellAnimation *anim = mA[i].mCellAnim;
        if (anim) {
            delete anim;
        }
        i++;
    } while (i < 2);
    i = 0;
    do {
        CellAnimation *anim = mB[i].mCellAnim;
        if (anim) {
            delete anim;
        }
        i++;
    } while (i < 3);
    i = 0;
    do {
        CellAnimation *anim = mC[i].mCellAnim;
        if (anim) {
            delete anim;
        }
        i++;
    } while (i < 8);
    if (mIcons) {
        delete mIcons;
    }
    if (mMessageWindow) {
        delete mMessageWindow;
    }
}

void BattleOrderSelect::Update() {
    WriteTouchMessage();
    switch (mState) {
        case 0:
            InitDisplay();
            LoadGraphics();
            ShuffleOrder();
            SpriteManager::Get().StartFade(FADE_MODE_TO_ZERO);
            mState++;
            break;
        case 1:
            if (!SpriteManager::Get().IsFading()) {
                mTimer = 0;
                mState++;
            }
            break;
        case 2:
            if (SetAndCheckMessageType(0xf)) {
                mState++;
            }
            break;
        case 3:
            mTimer++;
            if (mTimer >= 0x1e) {
                mMessageWindow->StartMessage(0x1d, 0, 1);
                mState++;
            }
            SetBgSlide(mTimer);
            break;
        case 4:
            if (UpdateTouchSelection()) {
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
                if (AssignUntakenItem()) {
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
                ApplyTurnOrder();
                mState++;
            }
            break;
        case 7:
            SpriteManager::Get().StartFade(FADE_MODE_DOWN);
            mState++;
            break;
        case 8:
            if (!SpriteManager::Get().IsFading()) {
                mState++;
            }
            break;
        case 9:
            if (SetAndCheckMessageType(0x10)) {
                mState++;
            }
            break;
        case 10:
            RequestScene(1);
            mFinished = 1;
            return;
    }
    Draw();
}

BOOL BattleOrderSelect::SetAndCheckMessageType(u32 index) {
    WirelessManager::Get();
    if (IsWirelessMode(&data_020c4030.mUnk_228)) {
        return SetMessageTypeAndCheckAll(data_020d433c.mBattlePlayers, index) != 0;
    }
    return 1;
}

void BattleOrderSelect::WriteTouchMessage() {
    WirelessManager::Get();
    if (IsWirelessMode(&data_020c4030.mUnk_228)) {
        data_0220f540.mUnk_00 = 10;
        data_0220f540.mTouchDown = TouchManager::Get().mPressed;
        data_0220f540.mTouchX    = TouchManager::Get().mX;
        data_0220f540.mTouchY    = TouchManager::Get().mY;
    }
}

void BattleOrderSelect::ApplyTurnOrder() {
    u8 i = 0;
    do {
        SetTurnOrder(data_020d433c.mBattlePlayers, mItemOrder[mPlayerItemIndex[i]], i);
        i++;
    } while (i < mCount);
}

void BattleOrderSelect::ShuffleOrder() {
    u8 i = 0;
    do {
        mItemOrder[i] = MATH_Rand32(&gRandom, mCount);
        if (i != 0) {
            u8 same = 0;
            u8 j    = 0;
            u8 v    = mItemOrder[i];
            do {
                if (v == mItemOrder[j]) {
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

void BattleOrderSelect::InitDisplay() {
    GX_SetDispSelect(1);
    SpriteManager::Get().SetupMode(1, 1);
    GX_SetGraphicsMode(1, 0, 0);
    GXS_SetGraphicsMode(0);
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

void BattleOrderSelect::LoadGraphics() {
    ChangeDir("/Game/Battle/Bg/");
    LoadAndQueueUpload(0, UPLOAD_KIND_BG_EXT_PLTT, 0, "select_order_u_bg_00_ncl.bin", 0, 0, 0);
    LoadAndQueueUpload(0, UPLOAD_KIND_BG0_CHAR, 0, "select_order_u_bg_00_ncg.bin", 0, 0, 0);
    LoadAndQueueUpload(0, UPLOAD_KIND_BG0_SCR, 0, "select_order_u_bg_00_nsc.bin", 0, 0, 0);
    LoadAndQueueUpload(1, UPLOAD_KIND_BG_EXT_PLTT, 0, "select_order_b_bg_01_ncl.bin", 0, 0, 0);
    LoadAndQueueUpload(1, UPLOAD_KIND_BG0_CHAR, 0, "select_order_b_bg_01_ncg.bin", 0, 0, 0);
    LoadAndQueueUpload(1, UPLOAD_KIND_BG0_SCR, 0, "select_order_b_bg_01_a_nsc.bin", 0, 0, 0);
    LoadAndQueueUpload(1, UPLOAD_KIND_BG_EXT_PLTT, 0x2000, "select_order_b_bg_01_ncl.bin", 0, 0, 0);
    LoadAndQueueUpload(1, UPLOAD_KIND_BG1_SCR, 0, "select_order_b_bg_01_b_nsc.bin", 0, 0, 0);
    LoadAndQueueUpload(1, UPLOAD_KIND_BG_EXT_PLTT, 0x4000, "select_order_b_bg_00_ncl.bin", 0, 0, 0);
    LoadAndQueueUpload(1, UPLOAD_KIND_BG2_CHAR, 0, "select_order_b_bg_00_ncg.bin", 0, 0, 0);
    LoadAndQueueUpload(1, UPLOAD_KIND_BG2_SCR, 0, "select_order_b_bg_00_nsc.bin", 0, 0, 0);
    SpriteScreen &screen0 = SpriteManager::Get().mScreens[0];
    ChangeDir("/Game/Character/Icon/");
    screen0.LoadCharacters("icon_ch_base.nce_ncg1D.bin", 0xb000, 0);
    screen0.LoadPalette("icon_ch_base_ncl.bin", 0x160, 0);
    u8 i = 0;
    do {
        CellAnimation *anim = new CellAnimation("icon_ch_base_nce.bin", 0, 1, 1);
        mA[i].mCellAnim         = anim;
        mA[i].mCellAnim->mCharBase = screen0.GetCharBase(0xb000);
        mA[i].mCellAnim->mPaletteBase   = 11;
        mA[i].mX               = 0;
        mA[i].mY               = 0;
        mA[i].mUnk_08          = 0;
        i++;
    } while (i < 2);
    mA[0].mCellAnim->SetAnimation(7);
    mA[1].mCellAnim->SetAnimation(0);
    ChangeDir("/Game/Battle/Obj/");
    SpriteScreen &screen1 = SpriteManager::Get().Screen(1);
    screen0.LoadCharacters("select_order_b_obj_00.nce_ncg1D.bin", 0, 0);
    screen0.LoadPalette("select_order_b_obj_00_ncl.bin", 0, 0);
    screen1.LoadCharacters("select_order_b_obj_00.nce_ncg1D.bin", 0, 0);
    screen1.LoadPalette("select_order_b_obj_00_ncl.bin", 0, 0);
    ShuffleOrder();
    i = 0;
    do {
        u32 a                  = i & 0xff;
        mC[a].mCellAnim            = new CellAnimation("select_order_b_obj_00_nce.bin", 0, 1, 1);
        mC[a].mCellAnim->mCharBase = screen1.GetCharBase(0);
        mC[a].mCellAnim->mPaletteBase   = 0;
        mC[a].mX               = 0x84 - mCount * 0x20 + i * 0x40;
        mC[a].mY               = 0x60;
        mC[a].mUnk_08          = 0;
        mC[a].mCellAnim->SetAnimation(i + 4);
        u8 b = i + 4;
        mC[b].mCellAnim            = new CellAnimation("select_order_b_obj_00_nce.bin", 0, 1, 1);
        mC[b].mCellAnim->mCharBase = screen1.GetCharBase(0);
        mC[b].mCellAnim->mPaletteBase   = 0;
        mC[b].mX               = 0x84 - mCount * 0x20 + i * 0x40;
        mC[b].mY               = 0x40;
        mC[b].mUnk_08          = 2;
        mC[b].mCellAnim->SetAnimation(mItemOrder[i]);
        i++;
    } while (i < mCount);
    ChangeDir("/Game/2D/union_b/");
    screen0.LoadCharacters("union_b_obj_you.nce_ncg1D.bin", 0x10000, 0);
    screen0.LoadPalette("union_b_obj_you_ncl.bin", 0x140, 0);
    mB[0].mCellAnim            = new CellAnimation("union_b_obj_you_nce.bin", 3, 1, 1);
    mB[0].mCellAnim->mCharBase = screen0.GetCharBase(0x10000);
    mB[0].mCellAnim->mPaletteBase   = 10;
    mB[0].mUnk_09          = 0;
    mB[0].mCellAnim->mFrameIndex   = data_020d433c.mBattlePlayers->mLocalPlayer;
    mB[1].mCellAnim            = new CellAnimation("union_b_obj_you_nce.bin", 2, 1, 1);
    mB[1].mCellAnim->mCharBase = screen0.GetCharBase(0x10000);
    mB[1].mCellAnim->mPaletteBase   = 10;
    mB[1].mUnk_09          = 0;
    mB[2].mCellAnim            = new CellAnimation("union_b_obj_you_nce.bin", 4, 1, 1);
    mB[2].mCellAnim->mCharBase = screen0.GetCharBase(0x10000);
    mB[2].mCellAnim->mPaletteBase   = 10;
    mB[2].mUnk_09          = 0;
    ChangeDir("/");
    mIcons = new CharacterIcons(0xa000, 6, 0);
    i      = 0;
    do {
        mIcons->SetCharacter(i, mCharacterIds[i], data_020d433c.mBattlePlayers->mPlayers[i]->NameInfo(), 0);
        mIcons->SetVisible(i, 0);
        i++;
    } while (i < mCount);
    mMessageWindow = new MessageWindow("mes_Command", 1, 0x10000);
    mMessageWindow->func_02065bd4(0x14, 4, 1);
    mMessageWindow->SetWidth(0xd8);
    AntennaIndicator *unk = data_020d433c.mAntenna;
    if (unk) {
        unk->SetupLayout(6);
    }
}

void BattleOrderSelect::SetBgSlide(u8 timer) {
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

BOOL BattleOrderSelect::UpdateTouchSelection() {
    u8 done = 0;
    int hit = 0;
    u8 i = 0;
    do {
        if (mPlayerItemIndex[i] >= 0) {
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
                down = TouchManager::Get().mPressed;
                x    = TouchManager::Get().mX;
                y    = TouchManager::Get().mY;
            }
            if (down) {
                u8 j = 0;
                do {
                    if (IsInRect(x, y, mC[(u8) (j + 4)].mX, mC[(u8) (j + 4)].mY, 0x38, 0x48)) {
                        if (mItemTaken[j] != 0) {
                            SoundManager::Get().StartSeqArc(2);
                        } else {
                            mItemTaken[j] = 1;
                            mPlayerItemIndex[i] = j;
                            mC[(u8) (j + 4)].mCellAnim->SetAnimation(mItemOrder[mPlayerItemIndex[i]] + 8);
                            if (i == data_020d433c.mBattlePlayers->mLocalPlayer) {
                                hit = 1;
                            }
                            SoundManager::Get().StartSeqArc(0);
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
        mMessageWindow->StartMessage(0x1e, 1, 1);
    }
    return 0;
}

BOOL BattleOrderSelect::AssignUntakenItem() {
    u8 i = 0;
    do {
        if (mPlayerItemIndex[i] < 0) {
            u8 j = 0;
            do {
                if (mItemTaken[j] == 0) {
                    mItemTaken[j] = 1;
                    mPlayerItemIndex[i] = j;
                    mC[(u8) (j + 4)].mCellAnim->SetAnimation(mItemOrder[mPlayerItemIndex[i]] + 8);
                    SoundManager::Get().StartSeqArc(0);
                    return 0;
                }
                j++;
            } while (j < mCount);
        }
        i++;
    } while (i < mCount);
    return 1;
}

void BattleOrderSelect::Draw() {
    u8 i = 0;
    do {
        mC[(u8) (i + 4)].mCellAnim->Update();
        mC[(u8) (i + 4)].mCellAnim->Draw(1, mC[(u8) (i + 4)].mX, mC[(u8) (i + 4)].mY, mC[(u8) (i + 4)].mUnk_08);
        mC[i & 0xff].mCellAnim->Update();
        mC[i & 0xff].mCellAnim->Draw(0, mC[i & 0xff].mX, (s16) (mC[i & 0xff].mY + mUnk_c4[i]), mC[i & 0xff].mUnk_08);
        if (mPlayerItemIndex[i] >= 0) {
            u32 slot = mItemOrder[mPlayerItemIndex[i]];
            if (mIconTimers[i] != 0) {
                mIconTimers[i]--;
            } else {
                if (mUnk_c4[slot] < 0x10) {
                    mUnk_c4[slot]++;
                }
                s16 v  = mUnk_c4[slot];
                s8 off = v * v / 8 - 0x2c;
                u8 p   = data_020d433c.mBattlePlayers->mLocalPlayer;
                if (i == p) {
                    mB[0].mCellAnim->Draw(0, (s16) (mC[slot].mX + 0xc), (s16) (mC[slot].mY - 0xe - off), 2);
                } else {
                    bool flag = false;
                    if (data_020d433c.mBattlePlayers->mPlayers[i]->mCpu) {
                        flag = true;
                    }
                    if (flag == 1) {
                        mB[1].mCellAnim->DrawFrame(0, i, mC[slot].mX + 0x1b, mC[slot].mY - 0x31 - off, 2);
                    }
                    if ((data_020d433c.mHasPartners == 1) & (i == data_020d433c.mPartners[p])) {
                        mB[2].mCellAnim->DrawFrame(0, i, mC[slot].mX + 0x10, mC[slot].mY - 0x38 - off, 2);
                    }
                }
                mIcons->SetVisible(i, 1);
                mIcons->SetDrawArgs(i, mC[slot].mX + 0x10, mC[slot].mY - 0x28 - off, 2);
                mA[0].mCellAnim->SetAnimation(i + 7);
                mA[0].mCellAnim->Draw(0, (s16) (mC[slot].mX + 0xc), (s16) (mC[slot].mY - 0x2c - off), 2);
                mA[1].mCellAnim->Draw(0, (s16) (mC[slot].mX + 0xe), (s16) (mC[slot].mY - 0x2a - off), 3);
            }
        }
        i++;
    } while (i < mCount);
    mMessageWindow->Update();
    mIcons->DrawIcons();
}

// ry is volatile as a placeholder: the original reads the stack argument twice (see docs/decompiling.md).
BOOL BattleOrderSelect::IsInRect(int x, int y, int rx, volatile s16 ry, u8 w, u8 h) {
    if ((x >= rx) & (y >= ry)) {
        if ((x < rx + w) & (y < ry + h)) {
            return 1;
        }
    }
    return 0;
}
