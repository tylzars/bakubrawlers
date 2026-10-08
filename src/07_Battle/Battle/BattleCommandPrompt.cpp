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
#include "Unknown/UnkStruct_020651bc.hpp"
#include <nitro/gx.h>
#include <nitro/mi.h>
#include <stdio.h>

extern const WindowCellRecord data_ov007_0227a77c[];
extern const WindowItemRecord data_ov007_0227a7b4[];
extern MtxFx22 data_0220e838;
void func_01ffcef4(MtxFx22 *mtx, fx32 scaleX, fx32 scaleY, u16 angle);



BattleCommandPrompt::BattleCommandPrompt(u32 mode, void *target, u32 flags, u32 charOffset, u8 player)
    : SceneBase(0x3e8, 0) {
    mUnk_3c = 0;
    mUnk_50 = 0x100;
    mUnk_88 = 0;
    mUnk_8a = 0;
    mUnk_8d = 0xff;
    mUnk_8e = 0;
    mUnk_8f = 0;
    mUnk_90 = 0;
    mUnk_91 = 0;
    MI_CpuClear32(mUnk_0c, 0xc);
    MI_CpuClear32(mUnk_20, 0xc);
    MI_CpuClear32(mUnk_40, 0xc);
    mUnk_94[0] = flags;
    u8 i = 0;
    do {
        if (!(mUnk_94[(u32) i >> 3] & (1 << ((int) i % 8)))) {
            mUnk_8f = i;
            break;
        }
        i++;
    } while (i < 3);
    mUnk_18 = 0;
    mUnk_1c = 0;
    mUnk_2c = 0;
    mUnk_4c = charOffset;
    mUnk_8b = mode;
    mUnk_8c = player;
    if (mUnk_8b >= 5) {
        BattlePlayerList *list = data_020d433c.mBattlePlayers;
        if (mUnk_8c == list->mUnk_2a9) {
            mUnk_93 = 1;
        } else if (mUnk_8c == list->mPlayers[list->mUnk_2a9]->mUnk_f2) {
            mUnk_93 = 1;
        } else {
            mUnk_93 = 2;
        }
    } else {
        mUnk_93 = 0;
    }
    switch (mUnk_8b) {
        case 0:
        case 1:
            mUnk_30 = (BattleCommandPage *) target;
            mUnk_8c = data_020d433c.mBattlePlayers->mUnk_2a9;
            mUnk_8d = 0xff;
            func_ov007_0225f0c4((const BattlePromptSlot *) &data_020d433c.mBattlePlayers->mPlayers[mUnk_8c]->Slot54(0));
            break;
        case 3:
            mUnk_30 = (BattleCommandPage *) target;
            mUnk_8c = data_020d433c.mBattlePlayers->mUnk_2a9;
            mUnk_8d = 0xff;
            func_ov007_0225f0c4((const BattlePromptSlot *) &data_020d433c.mBattlePlayers->mPlayers[mUnk_8c]->Slot6c(0));
            break;
        case 2:
            mUnk_34 = (BattleCommandList *) target;
            mUnk_8c = data_020d433c.mBattlePlayers->mUnk_2a9;
            mUnk_8d = 0xff;
            func_ov007_0225f200();
            break;
        case 6:
            mUnk_34 = (BattleCommandList *) target;
            mUnk_8c = player;
            mUnk_8d = player;
            func_ov007_0225f200();
            break;
        case 5:
            mUnk_30 = (BattleCommandPage *) target;
            mUnk_8c = player;
            mUnk_8d = player;
            func_ov007_0225f0c4((const BattlePromptSlot *) &data_020d433c.mBattlePlayers->mPlayers[mUnk_8c]->Slot54(0));
            break;
        case 7:
            mUnk_30 = (BattleCommandPage *) target;
            mUnk_8c = player;
            mUnk_8d = player;
            func_ov007_0225f0c4((const BattlePromptSlot *) &data_020d433c.mBattlePlayers->mPlayers[mUnk_8c]->Slot6c(0));
            break;
    }
    mUnk_38 = new SelectWindow(2, data_ov007_0227a7b4, -1);
    MtxFx22 mtx;
    func_01ffcef4(&mtx, 0x1000, 0x1000, 0);
    G2x_SetBGyAffine_((vu16 *) 0x4001030, &mtx, 0, 0, -mUnk_50, 0);
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
        a = mUnk_0c[i];
        if (a) {
            delete a;
        }
        i++;
    } while (i < 3);
    j = 0;
    do {
        c = mUnk_20[j];
        if (c) {
            delete c;
        }
        j++;
    } while (j < 3);
    if (mUnk_18) {
        delete mUnk_18;
    }
    if (mUnk_1c) {
        delete mUnk_1c;
    }
    if (mUnk_38) {
        delete mUnk_38;
    }
    if (mUnk_2c) {
        delete mUnk_2c;
    }
    if (mUnk_3c) {
        delete mUnk_3c;
    }
    k = 0;
    do {
        d = mUnk_40[k];
        if (d) {
            delete d;
        }
        k++;
    } while (k < 3);
    MtxFx22 mtx;
    func_01ffcef4(&mtx, 0x1000, 0x1000, 0);
    G2x_SetBGyAffine_((vu16 *) 0x4001030, &mtx, 0, 0, 0, 0);
    G2S_SetBG3Priority(3);
    GXS_SetVisibleWnd(0);
    G2S_SetBlendBrightness(0x1f, 0);
}

struct PromptRect {
    u8 x;
    u8 y;
    u8 w;
    u8 h;
};
extern const PromptRect data_ov007_0227a764[];
extern const PromptRect data_ov007_0227a75c[];
extern u8 data_ov007_0227a770[];
void func_ov007_0225f080(s16 *x);
void func_ov007_0225e698();

// non-matching: only the register of the first state-byte load differs (original ldrb r1 / cmp r1 / addls pc,pc,r1; mine uses r0), everything else is identical
void BattleCommandPrompt::Update() {
    switch (mUnk_8a) {
        case 0: {
            func_ov007_0225e698();
            func_ov007_0225e6d4();
            SelectWindow *win = mUnk_38;
            WindowItem *items = win->mCursor->mItems;
            if (mUnk_8b == 0 || data_020d433c.mUnk_83 == 4) {
                win->mButtons[1].mEnabled = 0;
                items[1].mFlags           = 0x8000;
            } else {
                win->mButtons[1].mEnabled = 1;
                items[1].mFlags           = 2;
            }
            if (mUnk_8b >= 5) {
                mUnk_38->mButtons[0].mEnabled = 0;
            } else {
                mUnk_38->mButtons[0].mEnabled = 1;
            }
            G2S_SetBG0Priority(3);
            G2S_SetBG3Priority(1);
            mUnk_88 = 0;
            mUnk_8a++;
            return;
        }
        case 1:
            switch (mUnk_8b) {
                case 0:
                case 1:
                case 3:
                    mUnk_2c->func_02065890(0x11, 0, 1);
                    break;
                case 2:
                    mUnk_2c->func_02065890(0x12, 0, 1);
                    break;
                case 5:
                    if (mUnk_8c == data_020d433c.mBattlePlayers->mUnk_2a9) {
                        mUnk_2c->func_02065890(0x27, 0, 1);
                    } else {
                        mUnk_2c->func_02065890((s16) (mUnk_8c + 0x28), 0, 1);
                    }
                    break;
                case 7:
                    if (mUnk_8c == data_020d433c.mBattlePlayers->mUnk_2a9) {
                        mUnk_2c->func_02065890(0x2c, 0, 1);
                    } else {
                        mUnk_2c->func_02065890((s16) (mUnk_8c + 0x2d), 0, 1);
                    }
                    break;
                case 6:
                    if (mUnk_8c == data_020d433c.mBattlePlayers->mUnk_2a9) {
                        mUnk_2c->func_02065890(0x22, 0, 1);
                    } else {
                        mUnk_2c->func_02065890((s16) (mUnk_8c + 0x23), 0, 1);
                    }
                    break;
            }
            GXS_SetVisiblePlane(0x19);
            mUnk_8a++;
            return;
        case 2:
            switch (mUnk_8e) {
                case 0:
                    mUnk_50 -= 0x18;
                    if (mUnk_50 <= 0) {
                        mUnk_50 = 0;
                        mUnk_8e = 1;
                        mUnk_38->mUnk_064 = 1;
                    }
                    func_020254f0((FrameCallback) func_ov007_0225f080, &mUnk_50);
                    break;
                case 1: {
                    int idx            = mUnk_8f + mUnk_90;
                    SelectWindow *win  = mUnk_38;
                    WindowItem *items  = win->mCursor->mItems;
                    u8 bits = mUnk_94[idx / 8];
                    if ((bits & (1 << (idx % 8))) || mUnk_8b >= 5) {
                        win->mButtons[0].mEnabled = 0;
                        items[0].mFlags           = 0x8000;
                    } else {
                        win->mButtons[0].mEnabled = 1;
                        items[0].mFlags           = 5;
                    }
                    switch ((u8) mUnk_38->Update(1)) {
                        case 1:
                            mUnk_8e = 3;
                            mUnk_88 = 0;
                            break;
                        case 2:
                            if (mUnk_8b != 0) {
                                mUnk_91 = 1;
                                mUnk_8e = 3;
                                mUnk_88 = 0;
                            }
                            break;
                    }
                    if (mUnk_38->mUnk_061 == 1) {
                        if (InputManager::Get().IsTrigger(0x10)) {
                            u8 cur = mUnk_8f + mUnk_90;
                            if (cur < mUnk_92 - 1) {
                                mUnk_8f++;
                                if (mUnk_8f >= 3) {
                                    mUnk_8f = 2;
                                    mUnk_90++;
                                    if (mUnk_90 > 3) {
                                        mUnk_90 = 3;
                                    }
                                }
                                u8 sel = mUnk_8f + mUnk_90;
                                switch (mUnk_8b) {
                                    case 0:
                                    case 1:
                                    case 3:
                                    case 5:
                                    case 7:
                                        func_02055668(mUnk_30, mUnk_52[sel], (s8) mUnk_8d);
                                        break;
                                    case 2:
                                    case 6: {
                                        BattleRecord *rec = mUnk_70[sel];
                                        func_02058260(mUnk_34, rec->mUnk_08, rec->mUnk_09, (s8) mUnk_8d);
                                        rec = mUnk_70[sel];
                                        func_0205afb4(mUnk_34, rec->mUnk_09, (u16) (rec->mUnk_04 + rec->mUnk_06), (BattleCellPair *) rec);
                                        break;
                                    }
                                }
                            }
                            if (cur != mUnk_8f) {
                                SoundManager::Get().func_020246f8(5);
                            }
                        }
                        if (InputManager::Get().IsTrigger(0x20)) {
                            u8 cur = mUnk_8f + mUnk_90;
                            if (cur != 0) {
                                if (mUnk_8f != 0) {
                                    mUnk_8f--;
                                    if (mUnk_8f >= 3) {
                                        mUnk_8f = 0;
                                    }
                                } else {
                                    mUnk_90--;
                                    if (mUnk_90 > 3) {
                                        mUnk_90 = 0;
                                    }
                                }
                                u8 sel = mUnk_8f + mUnk_90;
                                switch (mUnk_8b) {
                                    case 0:
                                    case 1:
                                    case 3:
                                    case 5:
                                    case 7:
                                        func_02055668(mUnk_30, mUnk_52[sel], (s8) mUnk_8d);
                                        break;
                                    case 2:
                                    case 6: {
                                        BattleRecord *rec = mUnk_70[sel];
                                        func_02058260(mUnk_34, rec->mUnk_08, rec->mUnk_09, (s8) mUnk_8d);
                                        rec = mUnk_70[sel];
                                        func_0205afb4(mUnk_34, rec->mUnk_09, (u16) (rec->mUnk_04 + rec->mUnk_06), (BattleCellPair *) rec);
                                        break;
                                    }
                                }
                            }
                            if (cur != mUnk_8f) {
                                SoundManager::Get().func_020246f8(5);
                            }
                        }
                        u8 i = 0;
                        do {
                            u32 tx = TouchManager::Get().mX;
                            u32 ty = TouchManager::Get().mY;
                            if (data_ov007_0227a764[i].x < tx && tx < data_ov007_0227a764[i].x + data_ov007_0227a764[i].w && data_ov007_0227a764[i].y < ty && ty < data_ov007_0227a764[i].y + data_ov007_0227a764[i].h) {
                                if (TouchManager::Get().mTouching) {
                                    if (i <= mUnk_92 - 1) {
                                        if (mUnk_8f != i) {
                                            SoundManager::Get().func_020246f8(5);
                                            mUnk_8f = i;
                                        } else if (mUnk_38->mButtons[0].mEnabled != 0) {
                                            SoundManager::Get().func_020246f8(0);
                                            mUnk_8e = 3;
                                            mUnk_88 = 0;
                                        }
                                        switch (mUnk_8b) {
                                            case 0:
                                            case 1:
                                            case 3:
                                            case 5:
                                            case 7:
                                                func_02055668(mUnk_30, mUnk_52[mUnk_8f + mUnk_90], (s8) mUnk_8d);
                                                break;
                                            case 2:
                                            case 6: {
                                                func_02058260(mUnk_34, mUnk_70[mUnk_8f + mUnk_90]->mUnk_08, mUnk_70[mUnk_8f + mUnk_90]->mUnk_09, (s8) mUnk_8d);
                                                BattleRecord *rec = mUnk_70[mUnk_8f + mUnk_90];
                                                func_0205afb4(mUnk_34, rec->mUnk_09, (u16) (rec->mUnk_04 + rec->mUnk_06), (BattleCellPair *) rec);
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
                    mUnk_50 -= 0x18;
                    if (mUnk_50 <= -0x100) {
                        mUnk_8a = 6;
                    }
                    func_020254f0((FrameCallback) func_ov007_0225f080, &mUnk_50);
                    break;
                case 3:
                    mUnk_88++;
                    if ((s32) mUnk_88 > 0x14) {
                        mUnk_88 = 0;
                        mUnk_8e = 2;
                    }
                    break;
            }
            switch (mUnk_8b) {
                case 3: {
                    u8 n = mUnk_92;
                    if (n > 3) {
                        n = 3;
                    }
                    u8 i = 0;
                    do {
                        if (mUnk_94[(u32) i >> 3] & (1 << ((int) i % 8))) {
                            mUnk_20[2]->Draw(1, (s16) (mUnk_50 + data_ov007_0227a764[i].x), data_ov007_0227a764[i].y, 0);
                        }
                        i++;
                    } while (i < n);
                    break;
                }
                case 5:
                case 7: {
                    u8 n = mUnk_92;
                    if (n > 3) {
                        n = 3;
                    }
                    u8 i = 0;
                    do {
                        if ((mUnk_6a + mUnk_90)[i] != 0) {
                            mUnk_20[0]->Draw(1, (s16) (mUnk_50 + data_ov007_0227a764[i].x), data_ov007_0227a764[i].y, 0);
                        }
                        i++;
                    } while (i < n);
                    break;
                }
                case 6: {
                    u8 n = mUnk_92;
                    if (n > 3) {
                        n = 3;
                    }
                    u8 i = 0;
                    do {
                        u8 v = (mUnk_6a + mUnk_90)[i];
                        if (v == 2) {
                            mUnk_20[0]->Draw(1, (s16) (mUnk_50 + data_ov007_0227a764[i].x), data_ov007_0227a764[i].y, 0);
                        } else if (v == 1) {
                            mUnk_20[1]->Draw(1, (s16) (mUnk_50 + data_ov007_0227a764[i].x), data_ov007_0227a764[i].y, 0);
                        }
                        i++;
                    } while (i < n);
                    break;
                }
            }
            mUnk_18->Update();
            mUnk_18->Draw(1, (s16) (mUnk_50 + data_ov007_0227a764[mUnk_8f].x), data_ov007_0227a764[mUnk_8f].y, 0);
            mUnk_2c->func_02065bd4((s16) (mUnk_50 + 0x20), 0x28, 0);
            mUnk_2c->func_020658d0();
            {
                u8 i = 0;
                do {
                    SelectWindow *w = mUnk_38;
                    w->mCellPos[i].x = mUnk_50 + data_ov007_0227a75c[i].x;
                    w->mCellPos[i].y = data_ov007_0227a75c[i].y;
                    i++;
                } while (i < 2);
            }
            mUnk_38->Draw();
            mUnk_20[0]->Update();
            mUnk_20[1]->Update();
            if (mUnk_3c) {
                mUnk_40[1]->Draw(1, 0x86, 1, 0);
                if (data_020d433c.mBattlePlayers->mPlayers[mUnk_8c]->IsActive()) {
                    mUnk_40[2]->Draw(1, 0x86, 1, 0);
                }
                mUnk_3c->func_020654a0();
                mUnk_40[0]->Draw(1, 0x86, 3, 0);
            }
            switch (mUnk_8b) {
                case 0:
                case 1:
                case 3:
                case 5:
                case 7: {
                    SpriteScreen &screen = SpriteManager::Get().Screen(1);
                    screen.func_02027624(3);
                    u8 n = mUnk_92;
                    if (n > 3) {
                        n = 3;
                    }
                    u8 i = 0;
                    do {
                        screen.func_02027778(data_ov007_0227a770, 1, (s16) (mUnk_50 + data_ov007_0227a764[i].x), data_ov007_0227a764[i].y, 0,
                                             ((mUnk_4c + (i + mUnk_90) * 0x1800) << 9) >> 16, (s8) (mUnk_90 + (i + 10)), 0);
                        i++;
                    } while (i < n);
                    return;
                }
                case 2:
                case 6: {
                    u8 n = mUnk_92;
                    if (n > 3) {
                        n = 3;
                    }
                    u8 i = 0;
                    do {
                        BattleRecord *rec = mUnk_70[i + mUnk_90];
                        u8 q              = (u8) ((rec->mUnk_04 + rec->mUnk_06) / 10);
                        u8 digits[3];
                        digits[0] = q / 10;
                        digits[1] = q % 10;
                        digits[2] = 0;
                        u8 j      = 0;
                        do {
                            mUnk_1c->func_0202a3ec(1, digits[j], (s16) (mUnk_50 + data_ov007_0227a764[i].x + 4 + j * 13),
                                                   (s16) (data_ov007_0227a764[i].y + 0x36), 0);
                            j++;
                        } while (j < 3);
                        mUnk_0c[i + mUnk_90]->Update();
                        mUnk_0c[i + mUnk_90]->func_0202a3ec(1, mUnk_70[i + mUnk_90]->mUnk_09, (s16) (mUnk_50 + data_ov007_0227a764[i].x),
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
                RequestScene(mUnk_5e[mUnk_8f + mUnk_90]);
                mFinished = 1;
            }
            break;
        case 7:
            mFinished = 1;
            break;
    }
}

void func_ov007_0225e68c(BattleCommandPrompt *prompt, u32 flag) {
    prompt->mUnk_8a = 7;
}

void func_ov007_0225e698() {
    G2S_SetBG0Priority(3);
    G2S_SetBG3Priority(0);
    GXS_SetVisiblePlane(0x11);
}

void BattleCommandPrompt::func_ov007_0225e6d4() {
    mUnk_2c = new MessageWindow("mes_Command", 1, 0x1f000);
    mUnk_2c->func_02065bd4(0x20, 0x28, 0);
    mUnk_2c->func_02065bcc(0x1f000);
    SpriteScreen &screen = SpriteManager::Get().Screen(1);
    screen.func_02027624(3);
    ChangeDir("/Game/2D/union_b");
    func_02025b64(1, 4, 0x1c800, "union_b_obj_cockpit.nce_ncg1D.bin", 0, 0, 0);
    func_02025b64(1, 5, 0x1c0, "union_b_obj_cockpit_ncl.bin", 0, 0, 0);
    mUnk_38->SetCells(2, data_ov007_0227a77c);
    func_02025b64(1, 4, 0x1b800, "union_b_obj_cursor.nce_ncg1D.bin", 0, 0, 0);
    func_02025b64(1, 5, 0x1a0, "union_b_obj_cursor_ncl.bin", 0, 0, 0);
    func_02025b64(1, 4, 0x1a000, "union_b_obj_gp.nce_ncg1D.bin", 0, 0, 0);
    func_02025b64(1, 5, 0x180, "union_b_obj_gp_ncl.bin", 0, 0, 0);
    ChangeDir("/Game/Battle/Obj");
    func_02025b64(1, 4, 0x19000, "battle_b_obj_state.nce_ncg1D.bin", 0, 0, 0);
    func_02025b64(1, 5, 0x100, "battle_b_obj_state_ncl.bin", 0, 0, 0);
    mUnk_20[0] = new CellAnimation("battle_b_obj_state_nce.bin", 0, 1, 1);
    mUnk_20[0]->mCharBase = screen.func_020276f4(0x19000);
    mUnk_20[0]->mUnk_11   = 8;
    mUnk_20[1] = new CellAnimation("battle_b_obj_state_nce.bin", 1, 1, 1);
    mUnk_20[1]->mCharBase = screen.func_020276f4(0x19000);
    mUnk_20[1]->mUnk_11   = 8;
    mUnk_20[2] = new CellAnimation("battle_b_obj_state_nce.bin", 2, 1, 1);
    mUnk_20[2]->mCharBase = screen.func_020276f4(0x19000);
    mUnk_20[2]->mUnk_11   = 8;
    switch (mUnk_8b) {
        case 0:
        case 1:
        case 3:
        case 5:
        case 7: {
            ChangeDir("/Game/Battle/Bg");
            func_02025b64(1, 1, 0x6000, "battle_b_bg_window_ncl.bin", 0, 0, 0);
            func_02025b64(1, 0xe, 0, "battle_b_bg_window_ncg.bin", 0, 0, 0);
            func_02025b64(1, 0xa, 0, "battle_b_bg_window_a_nsc.bin", 0, 0, 0);
            ChangeDir("/Game/Card/Graphic/Large");
            u8 i = 0;
            do {
                char name[0x100];
                u32 size;
                sprintf(name, "card_l_%03d.nce_ncg1D.bin", func_02065c20(mUnk_52[i]));
                void *data = func_02023504(name, 0, 0, 0, &size, 0);
                func_02025c1c(1, 4, mUnk_4c + i * 0x1800, screen.func_02028054((u8 *) data), 0x1000, 1);
                sprintf(name, "card_l_%03d_ncl.bin", func_02065c20(mUnk_52[i]));
                func_02025b64(1, 0, 0x1400 + i * 0x200, name, 0, 0, 0);
                i++;
            } while (i < mUnk_92);
            ChangeDir("/Game/2D/union_b");
            mUnk_18 = new CellAnimation("union_b_obj_cursor_nce.bin", 0, 1, 1);
            mUnk_18->mCharBase = screen.func_020276f4(0x1b800);
            mUnk_18->mUnk_11   = 0xd;
            if (mUnk_94[0]) {
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
            func_02025b64(1, 1, 0x6000, "battle_b_bg_window_ncl.bin", 0, 0, 0);
            func_02025b64(1, 0xe, 0, "battle_b_bg_window_ncg.bin", 0, 0, 0);
            func_02025b64(1, 0xa, 0, "battle_b_bg_window_a_nsc.bin", 0, 0, 0);
            ChangeDir("/Game/Battle/Obj");
            func_02025b64(1, 4, 0, "bakugan_sub.nce_ncg1D.bin", 0, 0, 0);
            func_02025b64(1, 0, 0x200, "bakugan_sub_ncl.bin", 0, 0, 0);
            u8 i = 0;
            do {
                mUnk_0c[i] = new CellAnimation("bakugan_sub_nce.bin", 0, 1, 1);
                mUnk_0c[i]->mCharBase = screen.func_020276f4(0);
                mUnk_0c[i]->mUnk_11   = 1;
                i++;
            } while (i < mUnk_92);
            ChangeDir("/Game/2D/union_b");
            mUnk_18 = new CellAnimation("union_b_obj_cursor_nce.bin", 1, 1, 1);
            mUnk_18->mCharBase = screen.func_020276f4(0x1b800);
            mUnk_18->mUnk_11   = 0xd;
            mUnk_1c = new CellAnimation("union_b_obj_gp_nce.bin", 2, 1, 1);
            mUnk_1c->mCharBase = screen.func_020276f4(0x1a000);
            mUnk_1c->mUnk_11   = 0xc;
            break;
        }
    }
    switch (mUnk_8b) {
      case 5:
      case 6:
      case 7: {
        ChangeDir("/Game/Character/Icon");
        func_02025b64(1, 4, 0x14000, "icon_ch_base.nce_ncg1D.bin", 0, 0, 0);
        func_02025b64(1, 5, 0x120, "icon_ch_base_ncl.bin", 0, 0, 0);
        u8 j = 0;
        do {
            mUnk_40[j] = new CellAnimation("icon_ch_base_nce.bin", 0, 1, 1);
            mUnk_40[j]->mCharBase = screen.func_020276f4(0x14000);
            mUnk_40[j]->mUnk_11   = 9;
            j++;
        } while (j < 3);
        mUnk_40[0]->SetAnimation(0);
        mUnk_40[1]->SetAnimation(mUnk_8c + 7);
        mUnk_40[2]->SetAnimation(0xb);
        mUnk_40[2]->mUnk_0f = mUnk_8c;
        ChangeDir("/");
        mUnk_3c = new UnkStruct_020651bc(0x16000, 0xb, 1);
        BattlePlayer *player = data_020d433c.mBattlePlayers->mPlayers[mUnk_8c];
        mUnk_3c->func_02065204(0, (s8) player->mUnk_f6, player->NameInfo(), 0);
        mUnk_3c->func_0206547c(0, 0x88, 5, 0);
    }
    }
    ChangeDir("/");
}

void func_ov007_0225f080(s16 *x) {
    G2x_SetBGyAffine_((vu16 *) 0x4001030, &data_0220e838, 0, 0, -*x, 0);
}

extern "C" u8 func_ov007_0225f0bc(u32 v) {
    return (u8) v;
}

void BattleCommandPrompt::func_ov007_0225f0c4(const BattlePromptSlot *slot) {
    BOOL add;
    u8 i = 0;
    mUnk_92 = 0;
    do {
        add = 0;
        switch (mUnk_93) {
            case 0:
                if (slot->mKind == 0) {
                    mUnk_52[mUnk_92] = slot->mId;
                    add              = 1;
                }
                break;
            case 2:
                if (slot->mKind != 2) {
                    mUnk_52[mUnk_92] = mUnk_8b == 7 ? 0xd6 : 0;
                } else {
                    mUnk_52[mUnk_92] = slot->mId;
                }
                add = 1;
                break;
            case 1:
                mUnk_52[mUnk_92] = slot->mId;
                add              = 1;
                break;
        }
        if (add) {
            mUnk_5e[mUnk_92] = i | 0x100 | (mUnk_8c << 10);
            mUnk_6a[mUnk_92] = slot->mKind;
            mUnk_92++;
        }
        i++;
        slot++;
    } while (i < data_020d433c.mUnk_99);
    func_02055668(mUnk_30, mUnk_52[mUnk_8f + mUnk_90], (s8) mUnk_8d);
}

void BattleCommandPrompt::func_ov007_0225f200() {
    u8 i = 0;
    BOOL ok;
    BattleRecord *rec;
    mUnk_92 = 0;
    do {
        rec = &data_020d433c.mBattlePlayers->mPlayers[mUnk_8c]->mRecords[i];
        ok  = 1;
        if (mUnk_93 == 0) {
            if (rec->mUnk_0a != 0) {
                ok = 0;
            }
        }
        if (ok) {
            mUnk_70[mUnk_92] = rec;
            mUnk_6a[mUnk_92] = rec->mUnk_0a;
            mUnk_5e[mUnk_92] = i | 0x200 | (mUnk_8c << 10);
            mUnk_92++;
        }
        if ((u8) (rec->mUnk_08 + 0xfb) <= 1) {
            if (rec->mUnk_0a == 1) {
                mUnk_34->mUnk_161 = 1;
            }
        }
        i++;
    } while (i < data_020d433c.mUnk_99);
    func_02058260(mUnk_34, mUnk_70[mUnk_8f + mUnk_90]->mUnk_08, mUnk_70[mUnk_8f + mUnk_90]->mUnk_09, (s8) mUnk_8d);
    BattleRecord *r = mUnk_70[mUnk_8f + mUnk_90];
    func_0205afb4(mUnk_34, r->mUnk_09, (u16) (r->mUnk_04 + r->mUnk_06), (BattleCellPair *) r);
}
