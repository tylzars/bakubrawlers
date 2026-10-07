#include "Battle/BattleCommand.hpp"
#include "Battle/BattleCommandPrompt.hpp"
#include "Battle/BattleCommandList.hpp"
#include "Battle/BattleCommandPage.hpp"
#include "Game/UnkStruct_020d433c.hpp"
#include "Game/UnkStruct_020d43f0.hpp"
#include "Graphics/CellAnimation.hpp"
#include "Graphics/Display.hpp"
#include "Graphics/SpriteManager.hpp"
#include "System/FileSystem.hpp"
#include "System/FrameUpdate.hpp"
#include "System/InputManager.hpp"
#include "System/SoundManager.hpp"
#include "System/TouchManager.hpp"
#include "UI/MessageWindow.hpp"
#include "UI/SelectWindow.hpp"
#include "Unknown/UnkStruct_02078290.hpp"
#include <nitro/gx.h>
#include <nitro/mi.h>

void func_ov007_02245888(BattleCommandMap *map);

void func_ov007_0224587c(BattleCommandMap *map);

void BattleCommand::func_ov007_0223c8ac() {
    if (mMode != 1) {
        return;
    }
    UnkStruct_SceneContext *context = mContext;
    if (context->mUnk_12 != 0) {
        context->mUnk_12 = 0;
        mMode            = 0;
    }
}

void func_ov007_0223cf8c(void *arg) {
    BattleCommand *command = (BattleCommand *) arg;
    int adjust             = data_020d43f0[0].mUnk_28_5 ? 0 : 4;
    G2S_SetBG0Offset(command->mUnk_120 + 0x82 - adjust, command->mUnk_122 + 0x9e);
}

void BattleCommand::func_ov007_0223ce24() {
    BattlePlayerList *list = data_020d433c.mBattlePlayers;
    if (list->mUnk_28a == list->mUnk_2a9) {
        if (list->mUnk_292 == 1) {
            mMessage->func_02065890(0x20, 0, 1);
        } else {
            mMessage->func_02065890(1, 0, 1);
        }
    } else {
        mMessage->func_02065890(0x33, 0, 1);
    }
}

void BattleCommand::func_ov007_0223b2f0() {
    ChangeDir("/Game/Battle/Obj");
    func_02025b64(1, 4, 0x17000, "battle_b_obj_turn.nce_ncg1D.bin", 0, 0, 0);
    func_02025b64(1, 5, 0x120, "battle_b_obj_turn_ncl.bin", 0, 0, 0);
    ChangeDir("/");
}

void func_ov007_0223a690(int x, int y) {
    G2S_SetBG0Control(3, 1, 0, 1, 1);
    G2S_SetBG3Control(1, 0, 4, 5, 0);
    G2S_SetBG0Offset(x + 0x82, y + 0x9e);
    G2S_SetBG2Offset(0, 0);
    G2S_SetBG3Offset(0, 0);
    G2S_BlendNone();
    GXS_SetVisiblePlane(0x11);
    GXS_SetVisibleWnd(0);
    func_02001ed0(3);
    G2S_SetBG0Priority(3);
}

void func_ov007_0223a5a4(BattleCommand *command, u32 mode) {
    SpriteManager::Get().func_02026dec(mode, 7);
    G2_SetBG0Offset(0, 0);
    G2_BlendNone();
    GX_SetDispSelect(1);
    GX_SetVisiblePlane(0x11);
    func_02001e68(1, 0, 1);
    G2_SetBG0Priority(0);
    G2_SetBG1Priority(1);
    GX_SetVisibleWnd(0);
}

void BattleCommand::func_ov007_0223ce90() {
    u16 turn = (u16) (data_020d433c.mBattlePlayers->mUnk_288 - 1);
    u16 count = (u16) (turn / data_020d433c.mPlayerCount + 1);
    if (count > 99) {
        count = 99;
    }
    mTurnDigits->Update();
    mTurnDigits->func_0202a3ec(1, count / 10, 0x3e, 0xae, 0);
    mTurnDigits->Update();
    mTurnDigits->func_0202a3ec(1, count % 10, 0x48, 0xae, 0);
    mUnk_20->Update();
    mUnk_20->Draw(1, 0x80, 0x60, 0);
}

void BattleCommand::func_ov007_0223c74c(BOOL flag) {
    switch (mMode) {
        case 2:
            if (flag) {
                func_ov007_02245888(mUnk_84);
            } else {
                func_ov007_0224587c(mUnk_84);
            }
            break;
        case 3:
            func_02055624(mUnk_78, 1);
            mUnk_78 = 0;
            break;
        case 4:
            func_0205821c(mUnk_7c);
            mUnk_7c = 0;
            break;
        default:
            return;
    }
    if (flag) {
        SpriteManager::Get().sFade.mMain = 3;
    } else {
        SpriteManager::Get();
        SpriteManager::sFade.mMain       = 1;
        SpriteManager::sBrightness.mMain = -16;
    }
    mMode                = 1;
    mContext->mUnk_12 = 0;
}

void BattleCommand::func_ov007_0223c8d4(u32 flag) {
    if (mMode != 0) {
        return;
    }
    func_ov007_0223a5a4(this, 0);
    switch (mNextMode) {
        case 2:
            mUnk_84 = new BattleCommandMap(data_020d433c.mUnk_95);
            mUnk_84->vfunc_0c();
            SpriteManager::Get().sFade.mMain = 2;
            break;
        case 3:
            mUnk_78 = new BattleCommandPage(0, -1);
            func_0205562c(mUnk_78, 1);
            func_02055764(mUnk_78, 0x18000, 0xe, 0xc0, 0x14, 0xe0, 0x14, 0);
            break;
        case 4:
            mUnk_7c = new BattleCommandList(0, 0, 0, flag);
            func_02058404(mUnk_7c, 0x18000, 0xe, 0xc0, 0x14, 0xe0, 0x14, 0);
            break;
    }
    mMode     = mNextMode;
    mNextMode = 0;
}


BattleCommand::~BattleCommand() {
    if (mUnk_8c) {
        func_ov007_0225e68c(mUnk_8c, 1);
    }
    if (mUnk_14) {
        delete mUnk_14;
    }
    if (mUnk_10) {
        delete mUnk_10;
    }
    if (mUnk_0c) {
        delete mUnk_0c;
    }
    if (mUnk_18) {
        delete mUnk_18;
    }
    if (mUnk_1c) {
        delete mUnk_1c;
    }
    if (mUnk_28) {
        delete mUnk_28;
    }
    if (mUnk_20) {
        delete mUnk_20;
    }
    if (mTurnDigits) {
        delete mTurnDigits;
    }
    u8 i = 0;
    do {
        CellAnimation *a = mUnk_3c[i];
        if (a) {
            delete a;
        }
        i++;
    } while (i < 12);
    CellAnimation *b;
    u8 j = 0;
    do {
        b = mUnk_2c[j];
        if (b) {
            delete b;
        }
        j++;
    } while (j < 4);
    if (mUnk_6c) {
        delete mUnk_6c;
    }
    if (mUnk_70) {
        delete mUnk_70;
    }
    if (mUnk_74) {
        delete mUnk_74;
    }
    SpriteManager::Get().func_0202740c();
    if (mMessage) {
        delete mMessage;
    }
}

void func_ov007_022470e4(BattleCommandMap *map, u8 a, u8 b);

void func_ov007_0223cf8c(void *arg);

BOOL BattleCommand::func_ov007_02239e1c(BOOL input) {
    BOOL result = FALSE;
    u8 sel      = 0;
    BOOL poll   = mUnk_6c->mUnk_061 != 1;
    if (input | poll) {
        sel = mUnk_6c->Update(1);
    }
    mUnk_13a = mUnk_6c->mUnk_0e0;
    if (data_020d433c.mBattlePlayers->mUnk_28f == 0) {
        mUnk_6c->mButtons[2].mEnabled = 0;
    }
    if (data_020d433c.mUnk_9a_3) {
        mUnk_6c->mButtons[2].mEnabled = 0;
        mUnk_6c->mButtons[5].mEnabled = 0;
    }
    if (input) {
        if (sel) {
            mUnk_136 = mUnk_6c->GetSelectedId();
            result   = TRUE;
            if (mUnk_136 == 3) {
                switch (mMode) {
                    case 2:
                        mNextMode = 3;
                        break;
                    case 3:
                        if (data_020d433c.mBattlePlayers->mUnk_1f8[mUnk_12c][mUnk_12d] > 0) {
                            mNextMode = 4;
                        } else {
                            mNextMode = 2;
                        }
                        break;
                    case 4:
                        mNextMode = 2;
                        break;
                }
                func_ov007_0223c74c(1);
            }
        }
        func_ov007_0223bb30(0);
    }
    mMessage->func_020658d0();
    func_ov007_0223b38c();
    u8 b                   = mUnk_12d;
    u8 a                   = mUnk_12c;
    BattlePlayerList *list = data_020d433c.mBattlePlayers;
    list->mUnk_28b         = a;
    list->mUnk_28c         = b;
    if (mMode == 2) {
        func_ov007_022470e4(mUnk_84, mUnk_12c, mUnk_12d);
    }
    func_020254f0(func_ov007_0223cf8c, this);
    return result;
}

// Record of a battle cell returned by func_ov007_02262530.
struct BattleCellInfo {
    /* 00 */ u16 mUnk_00;
    /* 02 */ u16 mUnk_02;
    /* 04 */ u16 mUnk_04;
    /* 06 */ u16 mUnk_06;
    /* 08 */ u8 mUnk_08;
    /* 09 */ u8 mUnk_09;
};

BattleCellInfo *func_ov007_02262530(BattlePlayerList *list, u8 row, u8 col);

u16 *func_ov007_02262510(BattlePlayerList *list, u8 index);

void func_ov007_02247a3c(BattleCommandMap *map);

void func_ov007_02246a84(BattleCommandMap *map, u32 a);

BOOL BattleCommand::func_ov007_02239fd8() {
    BOOL result = FALSE;
    func_ov007_0223c8ac();
    if (mMode == 0) {
        func_ov007_0223c8d4(1);
        switch (mMode) {
            case 2:
                func_ov007_02247a3c(mUnk_84);
                func_ov007_02246a84(mUnk_84, 2);
                break;
            case 3: {
                BattlePlayerList *list = data_020d433c.mBattlePlayers;
                s8 cell                = list->mUnk_16c[mUnk_12c][mUnk_12d];
                if (cell > 0) {
                    u8 owner  = list->mUnk_2a9;
                    u8 who    = list->mUnk_1c[(u8) (cell - 1)].mUnk_1;
                    u16 value = 0;
                    if (owner == who || who == list->mPlayers[owner]->mUnk_f2) {
                        value = *func_ov007_02262510(list, (u8) (cell - 1));
                    }
                    func_02055668(mUnk_78, value, (s8) who);
                } else {
                    func_0205562c(mUnk_78, 1);
                }
                break;
            }
            case 4: {
                BattleCellInfo *a = func_ov007_02262530(data_020d433c.mBattlePlayers, mUnk_12c, mUnk_12d);
                u16 sum           = a->mUnk_04 + func_ov007_02262530(data_020d433c.mBattlePlayers, mUnk_12c, mUnk_12d)->mUnk_06;
                u8 c              = func_ov007_02262530(data_020d433c.mBattlePlayers, mUnk_12c, mUnk_12d)->mUnk_09;
                u8 d              = func_ov007_02262530(data_020d433c.mBattlePlayers, mUnk_12c, mUnk_12d)->mUnk_08;
                BattlePlayerList *list = data_020d433c.mBattlePlayers;
                u8 slot                = list->mUnk_1f8[mUnk_12c][mUnk_12d] - 1;
                BattleUnit *unit       = &list->Units()[slot];
                u8 playerIndex         = unit->mUnk_0f;
                u8 recordIndex         = unit->mUnk_0e;
                func_02058260(mUnk_7c, d, c, (s8) playerIndex);
                func_0205afb4(mUnk_7c, c, sum, (BattleCellPair *) &data_020d433c.mBattlePlayers->mPlayers[playerIndex]->mRecords[recordIndex]);
                break;
            }
        }
        result = TRUE;
    }
    return result;
}

// non-matching: 11 words, registers and the order of the two ldrh copies.
void BattleCommand::func_ov007_0223c578() {
    switch (mMode) {
        case 2:
            return;
        case 3: {
            BattlePlayerList *list = data_020d433c.mBattlePlayers;
            s8 cell                = list->mUnk_16c[mUnk_12c][mUnk_12d];
            u8 owner               = list->mUnk_2a9;
            if (cell > 0) {
                u8 who    = list->mUnk_1c[(u8) (cell - 1)].mUnk_1;
                u16 value;
                if (owner == who || who == list->mPlayers[owner]->mUnk_f2) {
                    value = *func_ov007_02262510(list, (u8) (cell - 1));
                } else {
                    value = 0;
                }
                func_02055668(mUnk_78, value, (s8) who);
            } else {
                func_0205562c(mUnk_78, 1);
            }
            break;
        }
        case 4: {
            BattlePlayerList *list;
            u8 b;
            u8 a;
            s8 cell;
            a    = mUnk_12c;
            list = data_020d433c.mBattlePlayers;
            b    = mUnk_12d;
            cell = list->mUnk_1f8[a][b];
            if (cell > 0) {
                BattleCellInfo *info = func_ov007_02262530(list, a, b);
                u16 sum              = info->mUnk_04 + func_ov007_02262530(list, a, b)->mUnk_06;
                u8 c                 = func_ov007_02262530(data_020d433c.mBattlePlayers, mUnk_12c, mUnk_12d)->mUnk_09;
                u32 d                = func_ov007_02262530(data_020d433c.mBattlePlayers, mUnk_12c, mUnk_12d)->mUnk_08;
                BattleCellInfo *e    = func_ov007_02262530(data_020d433c.mBattlePlayers, mUnk_12c, mUnk_12d);
                BattleCellPair pair;
                pair.mUnk_00 = e->mUnk_00;
                pair.mUnk_02 = e->mUnk_02;
                if (mUnk_7c) {
                    func_02058260(mUnk_7c, d, c, (s8) data_020d433c.mBattlePlayers->Units()[(u8) (cell - 1)].mUnk_0f);
                    func_0205afb4(mUnk_7c, c, sum, &pair);
                }
            } else {
                func_02058228(mUnk_7c, 1);
            }
            break;
        }
    }
}

BOOL func_ov007_02262574(BattlePlayerList *list, u8 row, u8 col);

void BattleCommand::func_ov007_0223b874() {
    if (mUnk_12e != 0) {
        GXS_SetVisibleWnd(4);
        G2S_SetWndOBJInsidePlane(0x31, FALSE);
        G2S_SetWndOutsidePlane(0x11, FALSE);
        if (mUnk_135 == 0) {
            mUnk_14->Draw(1, (s16) (mUnk_139 + ((mUnk_12c * 16 + 14) - mUnk_120)), (s16) ((mUnk_12d * 24 - 22) - mUnk_122), 0);
        } else {
            u8 row = 13 - mUnk_12c;
            u8 col = 9 - mUnk_12d;
            mUnk_14->Draw(1, (s16) (mUnk_139 + ((row * 16 + 14) - mUnk_120)), (s16) ((col * 24 - 22) - mUnk_122), 0);
        }
        G2S_SetBlendBrightness(1, mUnk_12f);
    } else {
        u8 row = 0;
        if (mUnk_135 == 0) {
            mUnk_10->Update();
            do {
                u8 col = 0;
                do {
                    if (func_ov007_02262574(data_020d433c.mBattlePlayers, row, col) && mUnk_90[row][col] != 0) {
                        mUnk_10->Draw(1, (s16) (mUnk_139 + ((row * 16 + 14) - mUnk_120)), (s16) ((col * 24 - 22) - mUnk_122), 1);
                    }
                    col++;
                } while (col < 10);
                row++;
            } while (row < 14);
        } else {
            mUnk_10->Update();
            do {
                u8 col = 0;
                do {
                    if (func_ov007_02262574(data_020d433c.mBattlePlayers, row, col) && mUnk_90[row][col] != 0) {
                        u8 r = 13 - row;
                        u8 c = 9 - col;
                        mUnk_10->Draw(1, (s16) (mUnk_139 + ((r * 16 + 14) - mUnk_120)), (s16) ((c * 24 - 22) - mUnk_122), 1);
                    }
                    col++;
                } while (col < 10);
                row++;
            } while (row < 14);
        }
    }
}

struct BattleScrollBounds {
    s16 mMin;
    s16 mMax;
};
extern BattleScrollBounds data_ov007_02279acc[];
extern BattleScrollBounds data_ov007_02279aec[];

void func_ov007_022471c8(BattleCommandMap *map, u32 a);

// non-matching: register allocation in the bounds clamp (field/off/data ptr) and the touch cell select (col/row/flip/cur); control flow and calls match
void BattleCommand::func_ov007_0223bb30(u32 flag) {
    u32 moved = 0;
    mUnk_13b  = 0;
    if (mUnk_13a == 0) {
        if (mUnk_135 != 0) {
            if (InputManager::Get().IsTrigger(0x10) && mUnk_12c != 0) {
                u32 r = mUnk_12c - 1;
                if (mUnk_90[r][mUnk_12d] != 0) {
                    mUnk_12c = r;
                    moved    = 1;
                    if (mUnk_12c <= 2) {
                        mUnk_120 += 0x10;
                    }
                }
            }
            if (InputManager::Get().IsTrigger(0x20) && mUnk_12c < 13) {
                u32 r = mUnk_12c + 1;
                if (mUnk_90[r][mUnk_12d] != 0) {
                    mUnk_12c = r;
                    moved    = 1;
                    if (mUnk_12c >= 11) {
                        mUnk_120 -= 0x10;
                    }
                }
            }
            if (InputManager::Get().IsTrigger(0x40) && mUnk_12d < 9) {
                u32 c = mUnk_12d;
                c++;
                if (mUnk_90[mUnk_12c][c] != 0) {
                    mUnk_12d = c;
                    moved    = 1;
                    if (mUnk_12d >= 6) {
                        mUnk_122 -= 0x18;
                    }
                }
            }
            if (InputManager::Get().IsTrigger(0x80) && mUnk_12d != 0) {
                u32 c = mUnk_12d;
                c--;
                if (mUnk_90[mUnk_12c][c] != 0) {
                    mUnk_12d = c;
                    moved    = 1;
                    if (mUnk_12d <= 3) {
                        mUnk_122 += 0x18;
                    }
                }
            }
        } else {
            if (InputManager::Get().IsTrigger(0x10) && mUnk_12c < 13) {
                u32 r = mUnk_12c + 1;
                if (mUnk_90[r][mUnk_12d] != 0) {
                    mUnk_12c = r;
                    moved    = 1;
                    if (mUnk_12c >= 11) {
                        mUnk_120 += 0x10;
                    }
                }
            }
            if (InputManager::Get().IsTrigger(0x20) && mUnk_12c != 0) {
                u32 r = mUnk_12c - 1;
                if (mUnk_90[r][mUnk_12d] != 0) {
                    mUnk_12c = r;
                    moved    = 1;
                    if (mUnk_12c <= 2) {
                        mUnk_120 -= 0x10;
                    }
                }
            }
            if (InputManager::Get().IsTrigger(0x40) && mUnk_12d != 0) {
                u32 c = mUnk_12d;
                c--;
                if (mUnk_90[mUnk_12c][c] != 0) {
                    mUnk_12d = c;
                    moved    = 1;
                    if (mUnk_12d <= 3) {
                        mUnk_122 -= 0x18;
                    }
                }
            }
            if (InputManager::Get().IsTrigger(0x80) && mUnk_12d < 9) {
                u32 c = mUnk_12d;
                c++;
                if (mUnk_90[mUnk_12c][c] != 0) {
                    mUnk_12d = c;
                    moved    = 1;
                    if (mUnk_12d >= 6) {
                        mUnk_122 += 0x18;
                    }
                }
            }
        }
    }
    s32 tx = TouchManager::Get().mX;
    s32 ty = TouchManager::Get().mY;
    if (mUnk_13a == 0) {
        u8 field = data_020d433c.mUnk_95;
        if (data_020d433c.mUnk_8a_0 != 0) {
            field = 0;
        }
        s32 off = 0;
        switch (field) {
            case 0:
            case 1:
            case 2:
            case 3:
            case 5:
                break;
            case 6:
                if (data_020d43f0[0].mUnk_28_5 == 0) {
                    off = 0xa;
                }
                break;
            case 4:
                if (data_020d43f0[0].mUnk_28_5 == 0) {
                    off = 0x20;
                }
                break;
        }
        if (mUnk_129 != 0) {
            mUnk_120 = mUnk_124 - tx;
            mUnk_122 = mUnk_126 - ty;
        }
        s16 cx = mUnk_120;
        if (cx > data_ov007_02279acc[field].mMax) {
            mUnk_120 = data_ov007_02279acc[field].mMax;
        } else if (cx < data_ov007_02279acc[field].mMin - off) {
            mUnk_120 = data_ov007_02279acc[field].mMin - off;
        }
        s16 cy = mUnk_122;
        if (cy > data_ov007_02279aec[field].mMax) {
            mUnk_122 = data_ov007_02279aec[field].mMax;
        } else if (cy < data_ov007_02279aec[field].mMin) {
            mUnk_122 = data_ov007_02279aec[field].mMin;
        }
        if (TouchManager::Get().mUnk_57 != 0) {
            mUnk_129 = 1;
            mUnk_124 = tx + mUnk_120;
            mUnk_126 = ty + mUnk_122;
        }
        if (TouchManager::Get().mTouching != 0) {
            mUnk_124 = tx + mUnk_120;
            mUnk_126 = ty + mUnk_122;
            u8 flip;
            u8 cur;
            s16 col  = (mUnk_124 - 0xe - mUnk_139) / 16;
            s16 row  = (mUnk_126 + 0x16) / 10;
            if (col >= 0 && col <= 13 && row >= 0 && row <= 9 && mUnk_90[col][row] != 0) {
                flip = mUnk_135;
                cur  = mUnk_12c;
                if (flip == 0) {
                    if (cur != col || mUnk_12d != row) {
                        mUnk_12c = col;
                        mUnk_12d = row;
                        moved    = 1;
                    } else if (cur == col && mUnk_12d == row) {
                        mUnk_13b = 1;
                    }
                } else {
                    int fc = 13 - col;
                    int fr = 9 - row;
                    if (cur != fc || mUnk_12d != fr) {
                        mUnk_12c = fc;
                        mUnk_12d = fr;
                        moved    = 1;
                    } else if (cur == fc && mUnk_12d == fr) {
                        mUnk_13b = 1;
                    }
                }
            }
        }
        if (TouchManager::Get().mTouching != 0) {
            mUnk_129 = 0;
        }
    }
    if (mMode == 2) {
        if (InputManager::Get().IsDown(0x200)) {
            if (InputManager::Get().IsHeld(0x100)) {
                func_ov007_02246a84(mUnk_84, 2);
                return;
            }
            func_ov007_022471c8(mUnk_84, 0);
        }
        if (InputManager::Get().IsDown(0x100)) {
            if (InputManager::Get().IsHeld(0x200)) {
                func_ov007_02246a84(mUnk_84, 2);
                return;
            }
            func_ov007_022471c8(mUnk_84, 1);
        }
    }
    if (moved == 1) {
        SoundManager::Get().func_020246f8(5);
        if (flag == 0) {
            func_ov007_0223c578();
        }
    }
}

// Marks the cells of the battle grid that exist on the current field (data_020d433c.mUnk_95 is the field type).
void BattleCommand::func_ov007_0223ca80() {
    if (data_020d433c.mUnk_8a_0 != 0) {
        MI_CpuFill8(mUnk_90, 0, 0x8c);
        mUnk_90[6][4] = 1;
        return;
    }
    MI_CpuFill8(mUnk_90, 1, 0x8c);
    u8 row;
    u8 col;
    switch (data_020d433c.mUnk_95) {
        case 2: {
            row = 0;
            do {
                col = 0;
                do {
                    if (col == 0 || col >= 9 || row <= 1 || row >= 12) {
                        mUnk_90[row][col] = 0;
                    }
                    if (((col == 1 || col == 8) && (row <= 3 || row >= 10)) ||
                        ((col == 2 || col == 7) && (row <= 2 || row >= 11))) {
                        mUnk_90[row][col] = 0;
                    }
                    col++;
                } while (col < 10);
                row++;
            } while (row < 14);
            break;
        }
        case 1:
            mUnk_90[3][2]  = 0;
            mUnk_90[10][2] = 0;
            mUnk_90[3][7]  = 0;
            mUnk_90[10][7] = 0;
        case 0:
        case 3:
        case 7: {
            row = 0;
            do {
                col = 0;
                do {
                    if (col <= 1 || col >= 8 || row <= 2 || row >= 11) {
                        mUnk_90[row][col] = 0;
                    }
                    col++;
                } while (col < 10);
                row++;
            } while (row < 14);
            break;
        }
        case 4: {
            row = 0;
            do {
                col = 0;
                do {
                    if (((col == 0 || col == 9) && (row <= 2 || row >= 11)) ||
                        ((col == 1 || col == 8) && (row <= 1 || row >= 12)) ||
                        ((col == 2 || col == 7) && (row == 0 || row == 13))) {
                        mUnk_90[row][col] = 0;
                    }
                    col++;
                } while (col < 10);
                row++;
            } while (row < 14);
            break;
        }
        case 5: {
            row = 0;
            do {
                col = 0;
                do {
                    if (col == 0 || col >= 9 || row <= 1 || row >= 12) {
                        mUnk_90[row][col] = 0;
                    }
                    if (((col == 1 || col == 8) && (row <= 4 || row >= 9)) ||
                        ((col == 2 || col == 7) && (row <= 3 || row >= 10)) ||
                        ((col == 3 || col == 6) && (row <= 2 || row >= 11))) {
                        mUnk_90[row][col] = 0;
                    }
                    col++;
                } while (col < 10);
                row++;
            } while (row < 14);
            break;
        }
        case 6: {
            row = 0;
            do {
                col = 0;
                do {
                    if (row == 0 || row >= 13) {
                        mUnk_90[row][col] = 0;
                    }
                    if (((col == 0 || col == 9) && (row <= 4 || row >= 9)) ||
                        ((col == 1 || col == 8) && (row <= 2 || row >= 11)) ||
                        ((col == 2 || col == 7) && (row <= 1 || row >= 12))) {
                        mUnk_90[row][col] = 0;
                    }
                    col++;
                } while (col < 10);
                row++;
            } while (row < 14);
            break;
        }
    }
}

// Window items of the command screen (modified at run time, so not const): slot 0 the main button, 1..5 the
// commands, 6..9 the player markers.
extern WindowItemRecord data_ov007_02287c74[10];

extern const WindowItemRecord data_ov007_02279b0c[2];

extern const WindowItemRecord data_ov007_02279b7c[3];

struct BattleCommandPoint {
    u8 mX;
    u8 mY;
};

static const BattleCommandPoint data_ov007_02279ab0[4] = {{0xc2, 0x86}, {0x1e, 0x86}, {0x1e, 0x22}, {0xc2, 0x22}};

static const u8 data_ov007_02279aa8[2]                 = {0x04, 0xde};

static const BattleCommandPoint data_ov007_02279ab8[4] = {{6, 4}, {7, 4}, {7, 5}, {6, 5}};

// non-matching: 170 words, registers and the CSE of the data_020d43f0[0].mUnk_28_5 load.
BattleCommand::BattleCommand(u32 unk)
    : SceneBase(0x3e8, 0) {
    mMessage = 0;
    mUnk_8c  = 0;
    mUnk_11c = 0;
    mUnk_11e = 0;
    mUnk_120 = 0;
    mUnk_122 = 0;
    mUnk_129 = 0;
    mMode    = 2;
    mNextMode = 0;
    mUnk_12c = 0;
    mUnk_12d = 0;
    mUnk_12e = 0;
    mUnk_12f = 0;
    mUnk_138 = 0;
    mUnk_139 = 0;
    mUnk_13a = 0;
    mUnk_13b = 0;
    if (data_020d43f0[0].mUnk_28_5) {
        mUnk_139 = 0;
    } else {
        mUnk_139 = 4;
    }
    u8 side = data_020d433c.mBattlePlayers->mPlayers[data_020d433c.mBattlePlayers->mUnk_2a9]->mUnk_f1;
    if (side > 1) {
        mUnk_135 = 1;
    } else {
        mUnk_135 = 0;
    }
    u8 i = 0;
    do {
        u8 f = data_020d433c.mBattlePlayers->mPlayers[i]->mUnk_f1;
        if (mUnk_135) {
            f = (f + 2) % 4;
        }
        data_ov007_02287c74[i + 6].mX = mUnk_139 + data_ov007_02279ab0[f].mX;
        data_ov007_02287c74[i + 6].mY = data_ov007_02279ab0[f].mY;
        i++;
    } while (i < data_020d433c.mPlayerCount);
    BattlePlayerList *list = data_020d433c.mBattlePlayers;
    if (list->mUnk_28a != list->mUnk_2a9) {
        data_ov007_02287c74[1].mUnk_04 = 0x8000;
        data_ov007_02287c74[4].mUnk_04 = 0x8000;
        data_ov007_02287c74[2].mUnk_04 = 0x8000;
        data_ov007_02287c74[5].mUnk_04 = 0x8000;
    } else {
        if (list->mUnk_292 == 1) {
            data_ov007_02287c74[1].mY      = 0xc0;
            data_ov007_02287c74[1].mUnk_04 = 0x8000;
            data_ov007_02287c74[4].mY      = 0x22;
            data_ov007_02287c74[4].mUnk_04 = 1;
        } else {
            data_ov007_02287c74[1].mY      = 0x22;
            data_ov007_02287c74[1].mUnk_04 = 1;
            data_ov007_02287c74[4].mY      = 0xc0;
            data_ov007_02287c74[4].mUnk_04 = 0x8000;
        }
        if (list->mUnk_28f != 0) {
            data_ov007_02287c74[2].mUnk_04 = 1;
        } else {
            data_ov007_02287c74[2].mUnk_04 = 0x8000;
            data_ov007_02287c74[5].mUnk_04 = 0x800;
        }
    }
    if (data_020d433c.mUnk_9a_3) {
        data_ov007_02287c74[2].mUnk_04 = 0x8000;
        data_ov007_02287c74[5].mUnk_04 = 0x8000;
    }
    u8 x = data_ov007_02279aa8[data_020d43f0[0].mUnk_28_5];
    u8 k = 1;
    do {
        data_ov007_02287c74[k].mX = x;
        if (data_020d43f0[0].mUnk_28_5 == 0) {
            ((u8 *) &data_ov007_02287c74[k].mUnk_10)[0] = 4;
            ((u8 *) &data_ov007_02287c74[k].mUnk_10)[1] = 3;
        }
        k++;
    } while (k <= 5);
    mUnk_6c = new SelectWindow(10, data_ov007_02287c74, -1);
    mUnk_70 = new SelectWindow(2, data_ov007_02279b0c, -1);
    mUnk_74 = new SelectWindow(3, data_ov007_02279b7c, -1);
    mUnk_80 = unk;
    mUnk_14 = 0;
    mUnk_10 = 0;
    mUnk_0c = 0;
    mUnk_18 = 0;
    mUnk_1c = 0;
    mUnk_28 = 0;
    mUnk_20 = 0;
    mTurnDigits = 0;
    u8 m = 0;
    do {
        mUnk_3c[m] = 0;
        m++;
    } while (m < 12);
    u8 n = 0;
    do {
        mUnk_2c[n] = 0;
        n++;
    } while (n < 4);
    mUnk_124 = 0;
    mUnk_126 = 0;
    mUnk_133 = data_020d433c.mBattlePlayers->mUnk_28a;
    mUnk_128 = 0;
    mUnk_12c = data_ov007_02279ab8[side].mX;
    mUnk_12d = data_ov007_02279ab8[side].mY;
    data_020d433c.mBattlePlayers->mUnk_28b = mUnk_12c;
    data_020d433c.mBattlePlayers->mUnk_28c = mUnk_12d;
    func_ov007_0223ca80();
    if (data_020d433c.mBattlePlayers->mUnk_29c) {
        data_020d433c.mBattlePlayers->mUnk_29c->mUnk_a0 = 0;
    }
}


BOOL BattleCommand::func_ov007_0223a1c8() {
    switch (mUnk_138) {
        case 0:
            mUnk_138++;
        case 1:
            func_ov007_0223c74c(1);
            mUnk_74->func_0205c7c8(-1);
            mUnk_74->mCells[0]->SetAnimation(8);
            mUnk_74->mCells[1]->SetAnimation(0);
            mUnk_74->mCells[2]->SetAnimation(12);
            mUnk_138++;
            break;
        case 2:
            func_ov007_0223c8ac();
            if (mMode == 0) {
                mNextMode = 3;
                func_ov007_0223c8d4(0);
                mUnk_8c                = new BattleCommandPrompt(5, mUnk_78, 0, 0, (u8) (mUnk_136 - 6));
                mContext->mUnk_12 = 0;
                mUnk_138               = 7;
            }
            break;
        case 3:
            func_ov007_0223c74c(1);
            mUnk_138++;
            break;
        case 4:
            func_ov007_0223c8ac();
            if (mMode == 0) {
                mNextMode = 4;
                func_ov007_0223c8d4(0);
                mUnk_8c                = new BattleCommandPrompt(6, mUnk_7c, 0, 0, (u8) (mUnk_136 - 6));
                mContext->mUnk_12 = 0;
                mUnk_138               = 7;
            }
            break;
        case 5:
            func_ov007_0223c74c(1);
            mUnk_138++;
            break;
        case 6:
            func_ov007_0223c8ac();
            if (mMode == 0) {
                mNextMode = 3;
                func_ov007_0223c8d4(0);
                mUnk_8c                = new BattleCommandPrompt(7, mUnk_78, 0, 0, (u8) (mUnk_136 - 6));
                mContext->mUnk_12 = 0;
                mUnk_138               = 7;
            }
            break;
        case 7:
            if (mContext->mUnk_12 != 0) {
                mUnk_8c = 0;
                func_ov007_0223c74c(1);
                mUnk_138++;
            } else {
                if (mUnk_74->Update(1)) {
                    u8 id = mUnk_74->GetSelectedId();
                    switch (id) {
                        case 0:
                            func_ov007_0225e68c(mUnk_8c, 0);
                            mUnk_8c  = 0;
                            mUnk_138 = 1;
                            break;
                        case 1:
                            func_ov007_0225e68c(mUnk_8c, 0);
                            mUnk_8c  = 0;
                            mUnk_138 = 3;
                            break;
                        case 2:
                            func_ov007_0225e68c(mUnk_8c, 0);
                            mUnk_8c  = 0;
                            mUnk_138 = 5;
                            break;
                    }
                }
            }
            mUnk_74->Draw();
            break;
        case 8:
            func_ov007_0223c8ac();
            if (mMode == 0) {
                mUnk_74->mUnk_061 = 4;
                mUnk_74->mUnk_066 = 0;
                mNextMode         = 2;
                func_ov007_0223c8d4(0);
                func_ov007_02247a3c(mUnk_84);
                func_ov007_02246a84(mUnk_84, 2);
                func_ov007_0223ce24();
                mUnk_6c->func_0205c7c8(-1);
                func_ov007_0223b2f0();
                mUnk_138 = 0;
                return TRUE;
            }
            break;
        case 9:
            mUnk_74->mUnk_061 = 4;
            mUnk_74->mUnk_066 = 0;
            mUnk_138          = 0;
            break;
    }
    return FALSE;
}

extern const WindowCellRecord data_ov007_02279c24[];
extern const WindowCellRecord data_ov007_02279b44[];
extern const WindowCellRecord data_ov007_02279bd0[];

// non-matching: 16 words. In the player-animation loop the original loads mUnk_135 before the mPlayers[p] pointer (cmp scheduled before the add/ldr); every variant tried loads the pointer first.
void BattleCommand::func_ov007_0223a734() {
    ChangeDir("/Game/Battle/Obj");
    func_02025b64(0, 4, 0, "battle_u.nce_ncg1D.bin", 0, 0, 0);
    func_02025b64(0, 5, 0, "battle_u_ncl.bin", 0, 0, 0);
    mUnk_84 = new BattleCommandMap(data_020d433c.mUnk_95);
    mUnk_84->vfunc_0c();
    func_ov007_02247a3c(mUnk_84);
    func_ov007_02246a84(mUnk_84, 2);
    ChangeDir("/Game/Battle/Obj");
    u8 i;
    SpriteScreen &screen = SpriteManager::Get().Screen(1);
    screen.func_02027624(3);
    mUnk_6c->SetCells(10, data_ov007_02279c24);
    for (i = data_020d433c.mPlayerCount; i < 4; i++) {
        mUnk_6c->mButtons[i + 6].mEnabled = 0;
    }
    mUnk_70->SetCells(2, data_ov007_02279b44);
    mUnk_74->SetCells(3, data_ov007_02279bd0);
    for (i = 0; i < 12; i++) {
        mUnk_3c[i] = new CellAnimation("battle_b_obj_00_nce.bin", 0x44, 1, 1);
        mUnk_3c[i]->mCharBase = screen.func_020276f4(0xa000);
        mUnk_3c[i]->mUnk_11   = 0;
    }
    mUnk_0c = new CellAnimation("battle_b_obj_00_nce.bin", 0x15, 1, 1);
    mUnk_0c->mCharBase = screen.func_020276f4(0xa000);
    mUnk_0c->mUnk_11   = 0;
    mUnk_10 = new CellAnimation("battle_b_obj_00_nce.bin", 0x16, 1, 1);
    mUnk_10->mCharBase = screen.func_020276f4(0xa000);
    mUnk_10->mUnk_11   = 0;
    mUnk_14 = new CellAnimation("battle_b_obj_00_nce.bin", 0x1b, 1, 1);
    mUnk_14->mCharBase = screen.func_020276f4(0xa000);
    mUnk_14->mUnk_11   = 0;
    if (data_020d433c.mUnk_85) {
        mUnk_28 = new CellAnimation("battle_b_obj_00_nce.bin", 0x45, 1, 1);
        mUnk_28->mCharBase = screen.func_020276f4(0xa000);
        mUnk_28->mUnk_11   = 0;
    }
    u8 cur = data_020d433c.mBattlePlayers->mUnk_2a9;
    mUnk_18 = new CellAnimation("battle_b_obj_00_nce.bin", (u8) (cur + 0x38), 1, 1);
    mUnk_18->mCharBase = screen.func_020276f4(0xa000);
    mUnk_18->mUnk_11   = 0;
    if (data_020d433c.mUnk_98) {
        u8 partner = data_020d433c.mBattlePlayers->mPlayers[cur]->mUnk_f2;
        mUnk_1c = new CellAnimation("battle_b_obj_00_nce.bin", (u8) (partner + 0x40), 1, 1);
        mUnk_1c->mCharBase = screen.func_020276f4(0xa000);
        mUnk_1c->mUnk_11   = 0;
    }
    mUnk_20 = new CellAnimation("battle_b_obj_turn_nce.bin", 0, 1, 1);
    mUnk_20->mCharBase = screen.func_020276f4(0x17000);
    mUnk_20->mUnk_11   = 9;
    mTurnDigits = new CellAnimation("battle_b_obj_turn_nce.bin", 1, 1, 1);
    mTurnDigits->mCharBase = screen.func_020276f4(0x17000);
    mTurnDigits->mUnk_11   = 9;
    u8 p = 0;
    do {
        BattlePlayer *pl = data_020d433c.mBattlePlayers->mPlayers[p];
        if (mUnk_135 == 0) {
            u8 frame   = pl->mUnk_f1 + (p * 4 + 0x28);
            mUnk_2c[p] = new CellAnimation("battle_b_obj_00_nce.bin", frame, 1, 1);
        } else {
            u8 side    = (pl->mUnk_f1 + 2) % 4;
            u8 frame   = side + (p * 4 + 0x28);
            mUnk_2c[p] = new CellAnimation("battle_b_obj_00_nce.bin", frame, 1, 1);
        }
        mUnk_2c[p]->mCharBase = screen.func_020276f4(0xa000);
        mUnk_2c[p]->mUnk_11   = 0;
        p++;
    } while (p < data_020d433c.mPlayerCount);
    ChangeDir("/");
    mMessage = new MessageWindow("mes_Command", 1, 0x1f000);
    mMessage->func_02065bd4(0x14, 4, 0);
    mMessage->func_02065be4(0xd8);
    func_ov007_0223ce24();
}

// Loads the battle background; `variant` is the screen variant (BattleObj00 passes its mUnk_3c), overridden for some
// field types.
void func_ov007_0223ad70(u32 variant) {
    char path[0x100];
    MI_CpuClearFast((void *) 0x6609000, 0x1000);
    reg_GX_POWCNT |= 0x8000;
    ChangeDir("/Game/Battle/Bg");
    if (data_020d433c.mUnk_8a_0 != 0) {
        sprintf(path, "battle_b_bg_last_ncl.bin", data_020d433c.mUnk_95);
        func_02025b64(1, 1, 0x4000, path, 0, 0, 0);
        sprintf(path, "battle_b_bg_last_ncg.bin", data_020d433c.mUnk_95);
        func_02025b64(1, 0xb, 0, path, 0, 0, 0);
        sprintf(path, "battle_b_bg_last_a_nsc.bin", data_020d433c.mUnk_95);
        func_02025b64(1, 7, 0, path, 0, 0, 0);
        sprintf(path, "battle_b_bg_last_b_nsc.bin", data_020d433c.mUnk_95);
        func_02025b64(1, 7, 0x800, path, 0, 0, 0);
        sprintf(path, "battle_b_bg_last_c_nsc.bin", data_020d433c.mUnk_95);
        func_02025b64(1, 7, 0x1000, path, 0, 0, 0);
        sprintf(path, "battle_b_bg_last_d_nsc.bin", data_020d433c.mUnk_95);
        func_02025b64(1, 7, 0x1800, path, 0, 0, 0);
    } else {
        switch (data_020d433c.mUnk_95) {
            case 1:
            case 2:
            case 3:
            case 7:
                variant = 0;
                break;
        }
        sprintf(path, "battle_b_bg_%02d_ncl.bin", data_020d433c.mUnk_95);
        func_02025b64(1, 1, 0x4000, path, 0, 0, 0);
        sprintf(path, "battle_b_bg_%02d_ncg.bin", data_020d433c.mUnk_95);
        func_02025b64(1, 0xb, 0, path, 0, 0, 0);
        sprintf(path, "battle_b_bg_%02d_a_%02d_nsc.bin", data_020d433c.mUnk_95, variant);
        func_02025b64(1, 7, 0, path, 0, 0, 0);
        sprintf(path, "battle_b_bg_%02d_b_%02d_nsc.bin", data_020d433c.mUnk_95, variant);
        func_02025b64(1, 7, 0x800, path, 0, 0, 0);
        sprintf(path, "battle_b_bg_%02d_c_%02d_nsc.bin", data_020d433c.mUnk_95, variant);
        func_02025b64(1, 7, 0x1000, path, 0, 0, 0);
        sprintf(path, "battle_b_bg_%02d_d_%02d_nsc.bin", data_020d433c.mUnk_95, variant);
        func_02025b64(1, 7, 0x1800, path, 0, 0, 0);
    }
    SpriteManager::Get().Screen(1).func_02027624(3);
    ChangeDir("/Game/Battle/Obj");
    func_02025b64(1, 4, 0xa000, "battle_b_obj_00.nce_ncg1D.bin", 0, 0, 0);
    func_02025b64(1, 5, 0, "battle_b_obj_00_ncl.bin", 0, 0, 0);
    ChangeDir("/Game/2D/union_b");
    func_02025b64(1, 4, 0x1c800, "union_b_obj_cockpit.nce_ncg1D.bin", 0, 0, 0);
    func_02025b64(1, 5, 0x1c0, "union_b_obj_cockpit_ncl.bin", 0, 0, 0);
    ChangeDir("/Game/Card/Graphic/Small");
    func_02025b64(1, 4, 0x9000, "card_s_000_ncg.bin", 0, 0, 0);
    func_02025b64(1, 0, 0, "card_s_000_ncl.bin", 0, 0, 0);
    ChangeDir("/Game/2D/union_b");
    func_02025b64(1, 4, 0x18000, "union_b_obj_page.nce_ncg1D.bin", 0, 0, 0);
    if (data_020d433c.mUnk_10) {
        data_020d433c.mUnk_10->func_020785d8(8);
    }
    ChangeDir("/");
}

// Frame clock singleton (instance 0x020c1884); mFrame is the running frame count.
class UnkStruct_020c1884 {
public:
    /* 00 */ PAD(0x00, 0x10);
    /* 10 */ u32 mFrame;
    /* 14 */ PAD(0x14, 0x18);
    /* 18 */

    ~UnkStruct_020c1884();

    static UnkStruct_020c1884 &Get() {
        static UnkStruct_020c1884 instance;
        return instance;
    }
};

extern u8 data_ov007_02279ac0[];

void BattleCommand::func_ov007_0223b38c() {
    SpriteScreen &screen = SpriteManager::Get().Screen(1);
    screen.func_02027624(3);
    u8 col = mUnk_12c;
    u8 row = mUnk_12d;
    if (mUnk_135) {
        col = 13 - col;
        row = 9 - row;
    }
    mUnk_0c->Update();
    mUnk_0c->Draw(1, (s16) (col * 16 + 14 - mUnk_120 + mUnk_139), (s16) (row * 24 - 22 - mUnk_122), 1);
    u8 i = 0;
    u8 k = 0;
    do {
        u8 j = 0;
        do {
            u8 x = i;
            u8 y = j;
            if (mUnk_135) {
                y = 9 - j;
                x = 13 - i;
            }
            BOOL show;
            s8 cell = data_020d433c.mBattlePlayers->mUnk_1f8[i][j];
            if (cell > 0) {
                func_ov007_02262530(data_020d433c.mBattlePlayers, i, j);
                mUnk_3c[k]->func_0202a3ec(1, data_020d433c.mBattlePlayers->Units()[(u8) (cell - 1)].mUnk_0f, (s16) (x * 16 + 14 - mUnk_120 + mUnk_139), (s16) (y * 24 - 22 - mUnk_122), 1);
                k++;
            }
            s8 mark = data_020d433c.mBattlePlayers->mUnk_16c[i][j];
            if (mark > 0) {
                u8 who    = data_020d433c.mBattlePlayers->mUnk_1c[(u8) (mark - 1)].mUnk_1;
                show      = TRUE;
                if (data_020d433c.mBattlePlayers->mUnk_2a9 == who) {
                    if (!(UnkStruct_020c1884::Get().mFrame & 0x20)) {
                        show = FALSE;
                    }
                }
                if (show) {
                    screen.func_02027778(data_ov007_02279ac0, 1, (s16) (x * 16 + 14 - mUnk_120 + mUnk_139), (s16) (y * 24 - 22 - mUnk_122), 1, 0x120, 0, 0);
                }
            }
            j++;
        } while (j < 10);
        i++;
    } while (i < 14);
    u8 p = 0;
    do {
        u8 side = data_020d433c.mBattlePlayers->mPlayers[p]->mUnk_f1;
        if (mUnk_135) {
            side = (side + 2) % 4;
        }
        if (p == data_020d433c.mBattlePlayers->mUnk_2a9) {
            mUnk_18->Update();
            mUnk_18->Draw(1, (s16) (mUnk_139 + data_ov007_02279ab0[side].mX), data_ov007_02279ab0[side].mY, 0);
            if (data_020d433c.mUnk_98) {
                u8 partner = data_020d433c.mBattlePlayers->mPlayers[data_020d433c.mBattlePlayers->mPlayers[p]->mUnk_f2]->mUnk_f1;
                if (mUnk_135) {
                    partner = (partner + 2) % 4;
                }
                mUnk_1c->Update();
                mUnk_1c->Draw(1, (s16) (mUnk_139 + data_ov007_02279ab0[partner].mX), data_ov007_02279ab0[partner].mY, 0);
            }
        }
        mUnk_2c[p]->Update();
        mUnk_2c[p]->Draw(1, (s16) (mUnk_139 + data_ov007_02279ab0[side].mX), data_ov007_02279ab0[side].mY, 0);
        p++;
    } while (p < data_020d433c.mPlayerCount);
    if (data_020d433c.mUnk_85 && mUnk_6c->mUnk_061 == 1) {
        mUnk_28->Update();
        mUnk_28->Draw(1, data_ov007_02279aa8[data_020d43f0[0].mUnk_28_5], 0x22, 0);
    }
    mUnk_6c->Draw();
    func_ov007_0223ce90();
}
