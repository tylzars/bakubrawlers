#include "Battle/BattleCommand.hpp"
#include "Battle/BattleCharaAct.hpp"
#include "Battle/BattleCardEffect.hpp"
#include "Graphics/Camera.hpp"
#include "Battle/GimmickTypes.hpp"
#include "UI/StorytalkWindow.hpp"
#include "Battle/BattleCpu.hpp"
#include "Battle/BattleCommandPrompt.hpp"
#include "Battle/BattleCommandList.hpp"
#include "Battle/BattleCommandPage.hpp"
#include "Game/UnkStruct_020d433c.hpp"
#include "Game/UnkStruct_020d43f0.hpp"
#include "Graphics/CellAnimation.hpp"
#include "Graphics/Display.hpp"
#include "System/FileSystem.hpp"
#include "System/FrameUpdate.hpp"
#include "UI/MessageWindow.hpp"
#include "UI/SelectWindow.hpp"
#include "Graphics/AntennaIndicator.hpp"
#include <nitro/gx.h>
#include <nitro/mi.h>

struct BattleCommandPoint {
    u8 mX;
    u8 mY;
};
struct BattleScrollBounds {
    s16 mMin;
    s16 mMax;
};

static const WindowItemRecord data_ov007_02279b0c[2] = {
    {0, 0x5, 169, 170, 52, 20, 0, {-1, -1, -1, -1}, 0, 1},
    {1, 0x2, 225, 170, 30, 20, 0, {-1, -1, -1, -1}, 0, 1},
};

#include "System/SoundManager.hpp"
static const BattleScrollBounds data_ov007_02279acc[8] = {{0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 40}, {0, 0}, {0, 10}, {0, 0}};

static const BattleCommandPoint data_ov007_02279ab0[4] = {{0xc2, 0x86}, {0x1e, 0x86}, {0x1e, 0x22}, {0xc2, 0x22}};

#include "System/GameSystem.hpp"
#include "System/InputManager.hpp"
inline WindowItemRecord *CommandItems() {
    static WindowItemRecord sItems[10] = {
        {0, 0x8000, 128, 96, 0, 0, 0, {-1, -1, -1, -1}, 0, 0},
        {1, 0x1, 222, 34, 32, 32, 1027, {-1, -1, -1, -1}, 0, 1},
        {2, 0x1, 222, 66, 32, 32, 1027, {-1, -1, -1, -1}, 0, 1},
        {3, 0x8, 222, 98, 32, 32, 1027, {-1, -1, -1, -1}, 0, 1},
        {4, 0x1, 222, 34, 32, 32, 1027, {-1, -1, -1, -1}, 0, 1},
        {5, 0x800, 222, 66, 32, 32, 1027, {-1, -1, -1, -1}, 0, 1},
        {6, 0x8, 0, 0, 28, 28, 0, {-1, -1, -1, -1}, 0, 1},
        {7, 0x8, 0, 0, 28, 28, 0, {-1, -1, -1, -1}, 0, 1},
        {8, 0x8, 0, 0, 28, 28, 0, {-1, -1, -1, -1}, 0, 1},
        {9, 0x8, 0, 0, 28, 28, 0, {-1, -1, -1, -1}, 0, 1},
    };
    return sItems;
}

#include "System/WirelessManager.hpp"
static const u8 data_ov007_02279aa8[2]                 = {0x04, 0xde};

static const BattleCommandPoint data_ov007_02279ab8[4] = {{6, 4}, {7, 4}, {7, 5}, {6, 5}};

static const WindowCellRecord data_ov007_02279bd0[3] = {
    {"battle_b_obj_00_nce.bin", 8, 0xa000, 0, 0, 1, {8, 11, 10, 0}},
    {"battle_b_obj_00_nce.bin", 0, 0xa000, 0, 0, 1, {0, 3, 2, 0}},
    {"battle_b_obj_00_nce.bin", 12, 0xa000, 0, 0, 1, {12, 15, 14, 0}},
};

static const WindowCellRecord data_ov007_02279c24[10] = {
    {"battle_b_obj_00_nce.bin", 20, 0xa000, 0, 0, 1, {20, 20, 20, 0}},
    {"battle_b_obj_00_nce.bin", 0, 0xa000, 0, 0, 1, {0, 3, 2, 0}},
    {"battle_b_obj_00_nce.bin", 8, 0xa000, 0, 0, 1, {8, 11, 10, 0}},
    {"battle_b_obj_00_nce.bin", 16, 0xa000, 0, 0, 1, {16, 19, 18, 0}},
    {"battle_b_obj_00_nce.bin", 4, 0xa000, 0, 0, 1, {4, 7, 6, 0}},
    {"battle_b_obj_00_nce.bin", 9, 0xa000, 0, 0, 1, {9, 9, 9, 0}},
    {"battle_b_obj_00_nce.bin", 28, 0xa000, 0, 0, 1, {28, 36, 32, 0}},
    {"battle_b_obj_00_nce.bin", 29, 0xa000, 0, 0, 1, {29, 37, 33, 0}},
    {"battle_b_obj_00_nce.bin", 30, 0xa000, 0, 0, 1, {30, 38, 34, 0}},
    {"battle_b_obj_00_nce.bin", 31, 0xa000, 0, 0, 1, {31, 39, 35, 0}},
};

static const WindowItemRecord data_ov007_02279b7c[3] = {
    {0, 0x8, 32, 4, 32, 32, 0, {-1, -1, -1, -1}, 0, 1},
    {1, 0x8, 64, 4, 32, 32, 0, {-1, -1, -1, -1}, 0, 1},
    {2, 0x8, 96, 4, 32, 32, 0, {-1, -1, -1, -1}, 0, 1},
};

static const u16 data_ov007_02279ac0[6] = {0xa000, 0x8000, 0, 0, 0, 0};

static const WindowCellRecord data_ov007_02279b44[2] = {
    {"/Game/2D/union_b/union_b_obj_cockpit_nce.bin", 0, 0x1c800, 14, 0, 1, {0, 1, 1, 0}},
    {"/Game/2D/union_b/union_b_obj_cockpit_nce.bin", 2, 0x1c800, 14, 0, 1, {2, 3, 3, 0}},
};

static const BattleScrollBounds data_ov007_02279aec[8] = {{0, 0}, {0, 0}, {-20, 20}, {0, 0}, {-40, 40}, {-20, 20}, {-40, 40}, {0, 0}};

#include "Graphics/SpriteManager.hpp"
static const s16 data_ov007_02279aac[2] = {0x82, 0x9e};

#include "System/TouchManager.hpp"

void func_ov007_02245888(BattleCommandMap *map);

void func_ov007_0224587c(BattleCommandMap *map);

void func_ov007_0223a5a4(BattleCommand *command, u32 mode);

void func_ov007_0223a690(int x, int y);

u32 func_ov007_022696a0(u32 a, u32 b);

void func_ov007_022626b8(BattlePlayerList *list, u8 index);

extern "C" BOOL func_ov007_02263150(BattlePlayerList *list);

void func_ov007_02246a84(BattleCommandMap *map, u32 a);

BOOL func_ov007_02262574(BattlePlayerList *list, u8 row, u8 col);

extern "C" void func_ov007_022314bc(BattleCpu *cpu);

void func_ov007_0223cf8c(void *arg);

extern "C" void func_ov007_02247a7c(BattleCommandMap *map);

void func_ov007_02262638(BattlePlayerList *list, u8 row, u8 col, u8 player, u8 slot);

BOOL func_ov007_022631bc(BattlePlayerList *list, u32 id);

class BattleUnk0227532c {
public:
    u8 mPad[0x1c];
    BattleUnk0227532c(u8 planes);
};

void func_ov007_022470e4(BattleCommandMap *map, u8 a, u8 b);

u16 *func_ov007_02262510(BattlePlayerList *list, u8 index);

struct BattleCellInfo {
    /* 00 */ u16 mUnk_00;
    /* 02 */ u16 mUnk_02;
    /* 04 */ u16 mUnk_04;
    /* 06 */ u16 mUnk_06;
    /* 08 */ u8 mUnk_08;
    /* 09 */ u8 mUnk_09;
};

BattleCellInfo *func_ov007_02262530(BattlePlayerList *list, u8 row, u8 col);

extern "C" void func_ov007_022471c8(BattleCommandMap *map, u32 direction);

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
    if (side <= 1) {
        mUnk_135 = 0;
    } else {
        mUnk_135 = 1;
    }
    u8 i = 0;
    do {
        u8 f = data_020d433c.mBattlePlayers->mPlayers[i]->mUnk_f1;
        if (mUnk_135) {
            f = (f + 2) % 4;
        }
        CommandItems()[i + 6].mX = mUnk_139 + data_ov007_02279ab0[f].mX;
        CommandItems()[i + 6].mY = data_ov007_02279ab0[f].mY;
        i++;
    } while (i < data_020d433c.mPlayerCount);
    if (data_020d433c.mBattlePlayers->mUnk_28a != data_020d433c.mBattlePlayers->mUnk_2a9) {
        CommandItems()[1].mUnk_04 = 0x8000;
        CommandItems()[4].mUnk_04 = 0x8000;
        CommandItems()[2].mUnk_04 = 0x8000;
        CommandItems()[5].mUnk_04 = 0x8000;
    } else {
        if (data_020d433c.mBattlePlayers->mUnk_292 == 1) {
            CommandItems()[1].mY      = 0xc0;
            CommandItems()[1].mUnk_04 = 0x8000;
            CommandItems()[4].mY      = 0x22;
            CommandItems()[4].mUnk_04 = 1;
        } else {
            CommandItems()[1].mY      = 0x22;
            CommandItems()[1].mUnk_04 = 1;
            CommandItems()[4].mY      = 0xc0;
            CommandItems()[4].mUnk_04 = 0x8000;
        }
        if (data_020d433c.mBattlePlayers->mUnk_28f != 0) {
            CommandItems()[2].mUnk_04 = 1;
        } else {
            CommandItems()[2].mUnk_04 = 0x8000;
            CommandItems()[5].mUnk_04 = 0x800;
        }
    }
    if (data_020d433c.mUnk_9a_3) {
        CommandItems()[2].mUnk_04 = 0x8000;
        CommandItems()[5].mUnk_04 = 0x8000;
    }
    u8 x = data_ov007_02279aa8[data_020d43f0[0].mUnk_28_5];
    i = 1;
    do {
        CommandItems()[i].mX = x;
        if (data_020d43f0[0].mUnk_28_5 == 0) {
            CommandItems()[i].mSlideIn  = 4;
            CommandItems()[i].mSlideOut = 3;
        }
        i++;
    } while (i <= 5);
    mUnk_6c = new SelectWindow(10, CommandItems(), -1);
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
    i = 0;
    do {
        mUnk_3c[i] = 0;
        i++;
    } while (i < 12);
    i = 0;
    do {
        mUnk_2c[i] = 0;
        i++;
    } while (i < 4);
    mUnk_124 = 0;
    mUnk_126 = 0;
    mUnk_133 = data_020d433c.mBattlePlayers->mUnk_28a;
    mUnk_128 = 0;
    mUnk_12c = data_ov007_02279ab8[side].mX;
    mUnk_12d = data_ov007_02279ab8[side].mY;
    data_020d433c.mBattlePlayers->SetCursor(mUnk_12c, mUnk_12d);

    func_ov007_0223ca80();
    if (data_020d433c.mBattlePlayers->mUnk_29c) {
        data_020d433c.mBattlePlayers->mUnk_29c->mUnk_a0 = 0;
    }
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

void BattleCommand::Update() {
    switch (mUnk_128) {
        case 0: {
            BattlePlayerList *list = data_020d433c.mBattlePlayers;
            if (list->mUnk_288 == 0) {
                list->mUnk_288++;
                if (data_020d433c.mGimmicks) {
                    data_020d433c.mGimmicks->func_ov007_0222e25c();
                }
            } else {
                func_ov007_0223a5a4(this, 1);
                func_ov007_0223a690((s16) (mUnk_120 - mUnk_139), mUnk_122);
            }
            func_ov007_0223a734();
            func_ov007_0223ad70(mUnk_135);
            func_ov007_0223b2f0();
            SpriteManager::Get().StartFade(2);
            mUnk_128 = 1;
            return;
        }
        case 1:
            if (SpriteManager::Get().IsFading()) {
                return;
            }
            mUnk_128++;
            return;
        case 2: {
            BattlePlayerList *list = data_020d433c.mBattlePlayers;
            if (list->mUnk_293 == 0) {
                list->mUnk_293 = 1;
                new BattleCardEffect(0, 10, 0x11, 1);
                mContext->mUnk_12 = 0;
            } else {
                mContext->mUnk_12 = 1;
            }
            if (data_020d433c.mBattlePlayers->mUnk_292 == 2) {
                mUnk_128 = 3;
                return;
            }
            mUnk_128 = 7;
            return;
        }
        case 3:
            if (mContext->mUnk_12 == 0) {
                return;
            }
            func_ov007_0223c74c(1);
            SpriteManager::Get().StartFade(3);
            mUnk_128++;
            return;
        case 4: {
            if (SpriteManager::Get().IsFading()) {
                return;
            }
            func_ov007_0223c8ac();
            if (mMode != 0) {
                return;
            }
            u32 unk4 = 0;
            BattlePlayerList *list = data_020d433c.mBattlePlayers;
            if (list->mUnk_292 == 2) {
                u8 player = mUnk_133;
                u8 i = 0;
                do {
                    BattleUnit *unit = &list->mUnits[i];
                    if (player == unit->mUnk_0f && unit->mUnk_12 != 0) {
                        BattleUnit *u2 = &data_020d433c.mBattlePlayers->mUnits[i];
                        u8 slot = u2->mUnk_0e;
                        u8 marker = u2->mUnk_11;
                        func_ov007_02262828(data_020d433c.mBattlePlayers, i, 2);
                        BattlePlayer *p = data_020d433c.mBattlePlayers->mPlayers[mUnk_133];
                        p->mUnk_fe = 1;
                        p->mUnk_ee++;
                        p->mRecords[slot].mUnk_0a = 0;
                        BattleRecord *rec = &p->mRecords[slot];
                        unk4 = func_ov007_022696a0(rec->mUnk_08, rec->mUnk_09);
                        func_ov007_022626b8(data_020d433c.mBattlePlayers, marker);
                        break;
                    }
                    i++;
                } while (i < 12);
            }
            new BattleCharaAct((Camera *) mUnk_80, mUnk_133, 7, unk4, 0);
            mContext->mUnk_12 = 0;
            mUnk_128++;
            return;
        }
        case 5: {
            if (mContext->mUnk_12 == 0) {
                return;
            }
            if (data_020d433c.mBattlePlayers->mUnk_292 == 2) {
                data_020d433c.mBattlePlayers->mUnk_292 = 0;
                if (func_ov007_02263150(data_020d433c.mBattlePlayers)) {
                    mUnk_128 = 0x37;
                    SpriteManager::Get().StartFade(3);
                    return;
                }
                u8 i = 0;
                u8 found = 0;
                BattlePlayerList *list = data_020d433c.mBattlePlayers;
                do {
                    if (list->mUnk_1c[i].mUnk_2 != 0) {
                        found = 1;
                        break;
                    }
                    i++;
                } while (i < 12);
                if (found == 0) {
                    mUnk_128 = 0x38;
                    SpriteManager::Get().StartFade(3);
                    return;
                }
            }
            mNextMode = 2;
            func_ov007_0223c8d4(0);
            func_ov007_0223a690((s16) (mUnk_120 - mUnk_139), mUnk_122);
            func_ov007_0223ad70(mUnk_135);
            func_ov007_0223b2f0();
            func_ov007_02247a3c(mUnk_84);
            func_ov007_02246a84(mUnk_84, 2);
            SpriteManager::Get().StartFade(2);
            mUnk_128++;
            return;
        }
        case 6:
            if (SpriteManager::Get().IsFading()) {
                return;
            }
            mUnk_128++;
            mContext->mUnk_12 = 1;
            return;
        case 7: {
            if (mContext->mUnk_12 == 0) {
                return;
            }
            BattlePlayerList *list = data_020d433c.mBattlePlayers;
            u8 cur                 = list->mUnk_2a9;
            if (list->mUnk_28a == cur) {
                if (data_020d433c.mUnk_85 == 3 || data_020d433c.mUnk_85 == 7) {
                    list->mUnk_2a4->LoadGraphics();
                    data_020d433c.mBattlePlayers->mUnk_2a4->SetMessage(data_020d433c.mBattlePlayers->mUnk_2af, 0x64, 0, 0);
                    mUnk_128++;
                    G2S_SetBlendBrightness(0x1f, -10);
                    return;
                }
                if (data_020d433c.mUnk_98 != 0 && data_020d433c.mUnk_9a_3) {
                    u8 i = 0;
                    u8 count = 0;
                    u8 need = list->mUnk_294;
                    u8 partner = list->mPlayers[cur]->mUnk_f2;
                    do {
                        BattleUnit *unit = &list->mUnits[i];
                        if (unit->mUnk_10 != 0) {
                            if (partner == unit->mUnk_0f) {
                                count++;
                            }
                        }
                        i++;
                    } while (i < 12);
                    if (count == need) {
                        if (list->mUnk_28f == 0) {
                            mUnk_128 = 10;
                        } else {
                            mUnk_128 = 9;
                        }
                    } else {
                        mUnk_128 = 10;
                    }
                } else {
                    mUnk_128 = 10;
                }
            } else {
                BOOL active = list->mPlayers[mUnk_133]->IsActive();
                if (!active) {
                    mUnk_128 = 0xe;
                } else {
                    mUnk_128 = 0x10;
                    mUnk_11c = 0;
                }
            }
            func_ov007_0223ce24();
            mMessage->func_02065818();
            mUnk_6c->mUnk_064 = 1;
            return;
        }

        case 8: {
            if (InputManager::Get().IsHeld(1) || TouchManager::Get().mTouching) {
                data_020d433c.mBattlePlayers->mUnk_2a4->NextPage(1);
            }
            if (data_020d433c.mBattlePlayers->mUnk_2a4->func_02087cac()) {
                return;
            }
            G2S_SetBlendBrightness(0x1f, 0);
            data_020d433c.mBattlePlayers->mUnk_2af++;
            func_ov007_0223ce24();
            mMessage->func_02065818();
            mUnk_6c->mUnk_064 = 1;
            BattlePlayerList *list = data_020d433c.mBattlePlayers;
            if (mUnk_133 == list->mUnk_2a9) {
                mUnk_128 = 10;
                return;
            }
            BOOL active = list->mPlayers[mUnk_133]->IsActive();
            if (!active) {
                mUnk_128 = 0xe;
                return;
            }
            mUnk_128 = 0x10;
            mUnk_11c = 0;
            return;
        }
        case 9: {
            u8 i = 0;
            BattlePlayer *p = data_020d433c.mBattlePlayers->mPlayers[mUnk_133];
            u8 count = data_020d433c.mUnk_99;
            do {
                if (p->mSlots54[i].mFlag == 0) {
                    mUnk_132 = i;
                    break;
                }
                i++;
            } while (i < count);
            u8 x = 0;
            do {
                u8 y = 0;
                do {
                    if (func_ov007_02262574(data_020d433c.mBattlePlayers, x, y)) {
                        mUnk_12c = x;
                        mUnk_12d = y;
                        break;
                    }
                    y++;
                } while (y < 9);
                x++;
            } while (x < 14);
            u8 b                   = mUnk_12d;
            u8 a                   = mUnk_12c;
            BattlePlayerList *list = data_020d433c.mBattlePlayers;
            list->mUnk_28b         = a;
            list->mUnk_28c         = b;
            SpriteManager::Get().StartFade(3);
            mUnk_128 = 0x1d;
            return;
        }
        case 10: {
            BOOL ok = TRUE;
            if (mMode == 3) {
                if (mUnk_78->func_02055724()) {
                    ok = FALSE;
                }
            }
            if (mMode == 4) {
                if (mUnk_7c->func_020583f0()) {
                    ok = FALSE;
                }
            }
            if (data_020d433c.mUnk_83 != 0) {
                ok = FALSE;
            }
            if (ok) {
                if (InputManager::Get().IsHeld(8)) {
                    mUnk_128 = 0x31;
                    return;
                }
            }
            if (!func_ov007_02239e1c(1)) {
                return;
            }
            switch (mUnk_136) {
                case 3:
                    mUnk_128 = 0xb;
                    return;
                case 6:
                case 7:
                case 8:
                case 9:
                    mUnk_128 = 0xc;
                    return;
                default:
                    mUnk_128 = 0xd;
                    return;
            }
        }
        case 11:
            func_ov007_02239e1c(0);
            if (!func_ov007_02239fd8()) {
                return;
            }
            mUnk_128 = 10;
            mUnk_129 = 0;
            return;
        case 12:
            if (func_ov007_0223a1c8()) {
                mUnk_128 = 10;
            }
            return;
        case 13: {
            mUnk_6c->Update(1);
            if (mUnk_6c->mUnk_061 == 4) {
                switch (mUnk_136) {
                    case 1:
                        mUnk_128 = 0x25;
                        func_ov007_0223c74c(1);
                        mNextMode = 4;
                        break;
                    case 2: {
                        BattlePlayerList *list = data_020d433c.mBattlePlayers;
                        if (list->mUnk_28f != 0) {
                            u8 cur   = list->mUnk_2a9;
                            u8 i     = 0;
                            u8 count = data_020d433c.mUnk_99;
                            BattlePlayer *p = list->mPlayers[cur];
                            do {
                                if (p->mSlots54[i].mFlag == 0) {
                                    func_ov007_0223c74c(1);
                                    mUnk_70->mUnk_064 = 0;
                                    mUnk_128          = 0x17;
                                    break;
                                }
                                i++;
                            } while (i < count);
                        }
                        break;
                    }
                    case 4:
                        func_ov007_0223c74c(1);
                        mNextMode = 4;
                        mUnk_70->mUnk_064 = 0;
                        mUnk_128          = 0x29;
                        break;
                }
            }
            u8 b                   = mUnk_12d;
            BattlePlayerList *list = data_020d433c.mBattlePlayers;
            u8 a                   = mUnk_12c;
            list->mUnk_28b         = a;
            list->mUnk_28c         = b;
            func_ov007_0223b38c();
            mMessage->func_020658d0();
            return;
        }
        case 14: {
            BattlePlayerList *list = data_020d433c.mBattlePlayers;
            BattlePlayer *p        = list->mPlayers[list->mUnk_28a];
            switch (p->mMsg.mType) {
                case 7: {
                    if (mUnk_138 != 0) {
                        mUnk_138 = 9;
                        if (mUnk_8c) {
                            func_ov007_0225e68c(mUnk_8c, 1);
                            mUnk_8c = 0;
                        }
                    }
                    mUnk_132 = *(u32 *) p->mMsg.mUnk_04;
                    mUnk_12c = p->mMsg.mUnk_08;
                    u8 b     = p->mMsg.mUnk_09;
                    mUnk_12d = b;
                    u8 a     = mUnk_12c;
                    BattlePlayerList *l = data_020d433c.mBattlePlayers;
                    l->mUnk_28b = a;
                    l->mUnk_28c = b;
                    mNextMode = 0;
                    func_ov007_0223c74c(1);
                    SpriteManager::Get().StartFade(3);
                    mUnk_128 = 0xf;
                    return;
                }
                case 8:
                    if (mUnk_138 != 0) {
                        mUnk_138 = 0;
                        if (mUnk_8c) {
                            func_ov007_0225e68c(mUnk_8c, 1);
                            mUnk_8c = 0;
                        }
                    }
                    mNextMode = 0;
                    func_ov007_0223c74c(0);
                    SpriteManager::Get().SetFade(1, -16);
                    {
                        BattlePlayerList *l = data_020d433c.mBattlePlayers;
                        l->mPlayers[l->mUnk_28a]->mRecord = p->mMsg.mUnk_0e;
                    }
                    mUnk_128 = 0x35;
                    return;
                case 0x12:
                    if (mUnk_138 != 0) {
                        mUnk_138 = 0;
                        if (mUnk_8c) {
                            func_ov007_0225e68c(mUnk_8c, 1);
                            mUnk_8c = 0;
                        }
                    }
                    mUnk_12c  = p->mMsg.mTouchDown;
                    mUnk_12d  = p->mMsg.mTouchX;
                    mNextMode = 0;
                    mUnk_128  = 0x2d;
                    return;
                default:
                    mUnk_6c->mButtons[1].mEnabled = 0;
                    mUnk_6c->mButtons[2].mEnabled = 0;
                    mUnk_6c->mButtons[5].mEnabled = 0;
                    mUnk_6c->mButtons[4].mEnabled = 0;
                    mUnk_6c->mButtons[3].mEnabled = 1;
                    if (mUnk_138 == 0) {
                        if ((mMode == 0) | (mMode == 1)) {
                            func_ov007_02239e1c(0);
                            func_ov007_02239fd8();
                            return;
                        }
                        if (!func_ov007_02239e1c(1)) {
                            return;
                        }
                        switch (mUnk_136) {
                            case 6:
                            case 7:
                            case 8:
                            case 9:
                                func_ov007_0223a1c8();
                                return;
                        }
                        break;
                    }
                    func_ov007_0223a1c8();
                    return;
            }
            break;
        }
        case 15:
            func_ov007_0223c8ac();
            if (mMode != 0) {
                return;
            }
            func_ov007_0223b38c();
            func_ov007_0223b874();
            mUnk_70->Draw();
            if (SpriteManager::Get().IsFading()) {
                return;
            }
            mUnk_6c->mButtons[3].mEnabled = 0;
            mUnk_6c->mUnk_061 = 4;
            mUnk_6c->mUnk_066 = 0;
            mMode     = 0;
            mUnk_11e  = 0;
            mUnk_128  = 0x1f;
            return;
        case 16: {
            mUnk_11c++;
            if (mUnk_11c == 0) {
                return;
            }
            mUnk_6c->mButtons[1].mEnabled = 0;
            mUnk_6c->mButtons[2].mEnabled = 0;
            mUnk_6c->mButtons[5].mEnabled = 0;
            mUnk_6c->mButtons[4].mEnabled = 0;
            mUnk_11c                      = 0;
            u8 r = data_020d433c.mBattlePlayers->Current()->mUnk_c8->func_ov007_022310f0();
            if (r == 0xff || data_020d433c.mBattlePlayers->mUnk_28f == 0) {
                mUnk_128 = 0x12;
                return;
            }
            mUnk_132 = r;
            u8 v     = data_020d433c.mBattlePlayers->Current()->mUnk_c8->func_ov007_02231300();
            u8 hi    = v >> 4;
            u8 lo    = v & 0xf;
            if (mUnk_90[hi][lo] != 0) {
                u8 b                   = mUnk_12d;
                BattlePlayerList *list = data_020d433c.mBattlePlayers;
                u8 a                   = mUnk_12c;
                list->mUnk_28b         = a;
                list->mUnk_28c         = b;
                func_ov007_0223c74c(1);
                SpriteManager::Get().StartFade(3);
                mUnk_12c = hi;
                mUnk_12d = lo;
                mUnk_128++;
                return;
            }
            mUnk_128 = 0x12;
            return;
        }
        case 17:
            if (SpriteManager::Get().IsFading()) {
                return;
            }
            func_ov007_0223c8ac();
            if (mMode != 0) {
                return;
            }
            func_ov007_0223b38c();
            func_ov007_0223b874();
            mUnk_70->Draw();
            if (SpriteManager::Get().IsFading()) {
                return;
            }
            mMode    = 0;
            mUnk_11e = 0;
            mUnk_128 = 0x1f;
            return;
        case 18:
            func_ov007_0223b38c();
            mUnk_11c++;
            if (mUnk_11c == 0) {
                return;
            }
            mUnk_11c = 0;
            func_ov007_0223c74c(1);
            SpriteManager::Get().StartFade(3);
            if (data_020d433c.mBattlePlayers->mUnk_292 != 0) {
                mUnk_128 = 0x15;
                return;
            }
            mUnk_128++;
            return;
        case 19: {
            func_ov007_0223b38c();
            if (SpriteManager::Get().IsFading()) {
                return;
            }
            func_ov007_0223c8ac();
            if (mMode != 0) {
                return;
            }
            u8 cur          = data_020d433c.mBattlePlayers->mUnk_28a;
            BattlePlayer *p = data_020d433c.mBattlePlayers->mPlayers[cur];
            u8 sel          = p->mUnk_c8->func_ov007_02231dd0();
            func_ov007_022314bc(p->mUnk_c8);
            BattleRecord *rec = &p->mRecords[sel];
            p->mRecord = sel;
            u32 act    = func_ov007_022696a0(rec->mUnk_08, rec->mUnk_09);
            u8 kind    = 3;
            if (p->mUnk_c8->mUnk_7e != 0) {
                kind = 0xf;
            }
            new BattleCharaAct((Camera *) mUnk_80, cur, kind, act, 0);
            mContext->mUnk_12 = 0;
            mUnk_128++;
            return;
        }
        case 20:
            if (mContext->mUnk_12 == 0) {
                return;
            }
            mUnk_134 = 4;
            mUnk_128 = 0x39;
            return;
        case 21: {
            u8 i                   = 0;
            BattlePlayerList *list = data_020d433c.mBattlePlayers;
            u32 cur                 = list->mUnk_28a;
            do {
                if ((u8) cur == list->mUnits[i].mUnk_0f) {
                    list->mUnits[i].mUnk_12 = 1;
                    BattlePlayerList *l = data_020d433c.mBattlePlayers;
                    u8 idx              = l->mUnits[i].mUnk_11;
                    mUnk_12c            = l->mUnk_1c[idx].mUnk_3;
                    mUnk_12d            = data_020d433c.mBattlePlayers->mUnk_1c[idx].mUnk_4;
                    break;
                }
                i++;
            } while (i < 12);
            mUnk_128++;
            return;
        }
        case 22:
            mUnk_128 = 0x2d;
            return;
        case 23:
            func_ov007_0223c8ac();
            if (mMode != 0) {
                return;
            }
            mNextMode = 3;
            func_ov007_0223c8d4(0);
            new BattleCommandPrompt(1, mUnk_78, 0, 0, 0);
            mContext->mUnk_12 = 0;
            mUnk_128++;
            return;
        case 24: {
            u16 v = mContext->mUnk_12;
            if (v == 0) {
                return;
            }
            if (v != 10) {
                GXS_SetVisiblePlane(0x11);
                mUnk_132 = func_ov007_0225f0bc(v);
                mUnk_128++;
                return;
            }
            func_ov007_0223c74c(1);
            mUnk_128 = 0x23;
            return;
        }
        case 25:
            mUnk_6c->func_0205c7c8(-1);
            mUnk_70->mUnk_064 = 1;
            mMessage->func_02065890(0x13, 0, 1);
            mMessage->func_02065818();
            mUnk_128++;
            return;
        case 26: {
            if (mUnk_70->mUnk_061 == 1) {
                func_ov007_0223bb30(1);
            }
            WindowItem *item = mUnk_70->mCursor->mItems;
            u8 flag          = 0;
            if (func_ov007_02262574(data_020d433c.mBattlePlayers, mUnk_12c, mUnk_12d)) {
                item->mFlags = 5;
                if (mUnk_13b != 0) {
                    flag = 1;
                    SoundManager::Get().func_020246f8(0);
                }
            } else {
                item->mFlags = 0x804;
                if (mUnk_13b != 0) {
                    SoundManager::Get().func_020246f8(2);
                }
            }
            u8 sel    = mUnk_70->Update(1);
            mUnk_13a = mUnk_70->mUnk_0e0;
            if (sel == 1 || flag != 0) {
                mUnk_12f = 0;
                mUnk_11e = 0;
                SpriteManager::Get().StartFade(3);
                mUnk_128 = 0x1c;
                if (flag != 0) {
                    mUnk_70->mUnk_061 = 4;
                    mUnk_70->mUnk_066 = 0;
                }
                mUnk_129 = 0;
            } else if (sel == 2) {
                mUnk_128++;
                mUnk_129 = 0;
            }
            mMessage->func_020658d0();
            u8 b                   = mUnk_12d;
            BattlePlayerList *list = data_020d433c.mBattlePlayers;
            u8 a                   = mUnk_12c;
            list->mUnk_28b         = a;
            list->mUnk_28c         = b;
            func_ov007_0223b38c();
            func_ov007_0223b874();
            mUnk_70->Draw();
            func_020254f0(func_ov007_0223cf8c, this);
            return;
        }

        case 27:
            mUnk_70->Update(1);
            if (mUnk_70->mUnk_061 == 4) {
                new BattleCommandPrompt(1, mUnk_78, 0, 0, 0);
                mContext->mUnk_12 = 0;
                mUnk_70->func_0205c7c8(-1);
                mUnk_70->mUnk_064 = 0;
                mUnk_128          = 0x18;
            }
            mMessage->func_020658d0();
            {
                u8 b                   = mUnk_12d;
                BattlePlayerList *list = data_020d433c.mBattlePlayers;
                u8 a                   = mUnk_12c;
                list->mUnk_28b         = a;
                list->mUnk_28c         = b;
            }
            func_ov007_0223b38c();
            func_ov007_0223b874();
            mUnk_70->Draw();
            func_020254f0(func_ov007_0223cf8c, this);
            return;
        case 28:
            mUnk_70->Update(1);
            if (mUnk_70->mUnk_061 == 4) {
                mUnk_128++;
            }
            func_ov007_0223b38c();
            func_ov007_0223b874();
            return;
        case 29:
            data_0220f540.mUnk_00 = 7;
            data_0220f540.mUnk_04 = mUnk_132;
            data_0220f540.mUnk_08 = mUnk_12c;
            data_0220f540.mUnk_09 = mUnk_12d;
            mUnk_128++;
            func_ov007_0223b38c();
            func_ov007_0223b874();
            return;
        case 30:
            func_ov007_0223b38c();
            func_ov007_0223b874();
            mUnk_70->Draw();
            if (SpriteManager::Get().IsFading()) {
                return;
            }
            func_ov007_0223c74c(1);
            mUnk_11e = 0;
            mUnk_128++;
            mUnk_70->mUnk_064 = 0;
            return;
        case 31: {
            func_ov007_0223c8ac();
            if (mMode != 0) {
                return;
            }
            u8 cur = data_020d433c.mBattlePlayers->mUnk_28a;
            new BattleCharaAct((Camera *) mUnk_80, cur, 1, 0, 0);
            mContext->mUnk_12 = 0;
            GXS_SetVisibleWnd(0);
            mUnk_128++;
            return;
        }
        case 32: {
            if (mContext->mUnk_12 == 0) {
                return;
            }
            mNextMode = 2;
            func_ov007_0223c8d4(0);
            func_ov007_0223a690((s16) (mUnk_120 - mUnk_139), mUnk_122);
            func_ov007_0223ad70(mUnk_135);
            func_ov007_0223b2f0();
            u8 b                   = mUnk_12d;
            BattlePlayerList *list = data_020d433c.mBattlePlayers;
            u8 a                   = mUnk_12c;
            list->mUnk_28b         = a;
            list->mUnk_28c         = b;
            func_ov007_02247a7c(mUnk_84);
            mUnk_12e = 1;
            data_020d433c.mGimmicks->mState = 0;
            SpriteManager::Get().StartFade(2);
            mUnk_128++;
            return;
        }
        case 33: {
            func_ov007_0223b38c();
            func_ov007_0223b874();
            mUnk_11e++;
            if ((mUnk_11e % 4) == 0) {
                if (mUnk_12f < 16) {
                    mUnk_12f++;
                }
            }
            if (mUnk_11e == 1) {
                SoundManager::Get().func_020246f8(6);
            }
            if ((s32) mUnk_11e <= 0x96) {
                return;
            }
            mUnk_12f = 0;
            mUnk_11e = 0;
            mUnk_128++;
            func_ov007_02262638(data_020d433c.mBattlePlayers, mUnk_12c, mUnk_12d, mUnk_133, mUnk_132);
            mUnk_130 = 1;
            SpriteManager::Get().sFade[0] = 3;
            data_020d433c.mBattlePlayers->mUnk_28f = 0;
            WindowItem *items = mUnk_6c->mCursor->mItems;
            items[2].mFlags   = 0x8000;
            BattlePlayerList *list = data_020d433c.mBattlePlayers;
            if (list->mUnk_28a != list->mUnk_2a9) {
                items[5].mFlags = 0x8000;
            } else {
                items[5].mFlags = 0x800;
            }
            return;
        }
        case 34:
            if (SpriteManager::Get().sFade[0] <= 1) {
                GXS_SetVisibleWnd(0);
                mUnk_12e = 0;
                G2S_SetBlendBrightness(0x11, 0);
                func_ov007_0223c74c(1);
                mUnk_128++;
            }
            func_ov007_0223b38c();
            return;
        case 35:
            func_ov007_0223b38c();
            func_ov007_0223c8ac();
            if (mMode != 0) {
                return;
            }
            mNextMode = 2;
            func_ov007_0223c8d4(0);
            func_ov007_02247a3c(mUnk_84);
            func_ov007_02246a84(mUnk_84, 2);
            mUnk_128++;
            return;
        case 36: {
            func_ov007_0223b38c();
            if (SpriteManager::Get().sFade[0] > 1) {
                return;
            }
            mUnk_6c->func_0205c7c8(-1);
            mUnk_6c->Update(1);
            mMessage->func_02065818();
            func_ov007_0223ce24();
            mUnk_13a = 0;
            BattlePlayerList *list = data_020d433c.mBattlePlayers;
            u32 cur                 = list->mUnk_28a;
            if (cur == list->mUnk_2a9) {
                mUnk_128 = 10;
                return;
            }
            BOOL active = list->mPlayers[(u8) cur]->IsActive();
            if (!active) {
                mUnk_128 = 0xe;
                return;
            }
            mUnk_128 = 0x12;
            mUnk_11c = 0;
            return;
        }
        case 37:
            func_ov007_0223b38c();
            func_ov007_0223c8ac();
            if (mMode != 0) {
                return;
            }
            func_ov007_0223c8d4(0);
            func_02058228(mUnk_7c, 1);
            new BattleCommandPrompt(2, mUnk_7c, 0, 0, 0);
            mContext->mUnk_12 = 0;
            mUnk_128++;
            return;
        case 38: {
            u16 v = mContext->mUnk_12;
            if (v == 0) {
                return;
            }
            if (v == 10) {
                func_ov007_0223c74c(1);
                mUnk_128++;
                return;
            }
            func_ov007_0223c74c(1);
            SpriteManager::Get().StartSubFade(3);
            mUnk_132 = func_ov007_0225f0bc(v);
            data_020d433c.mBattlePlayers->mPlayers[mUnk_133]->mRecord = mUnk_132;
            GXS_SetVisiblePlane(0x11);
            mUnk_128 = 0x34;
            return;
        }
        case 39:
            func_ov007_0223b38c();
            func_ov007_0223c8ac();
            if (mMode != 0) {
                return;
            }
            mNextMode = 2;
            func_ov007_0223c8d4(0);
            func_ov007_02247a3c(mUnk_84);
            func_ov007_02246a84(mUnk_84, 2);
            mUnk_128++;
            return;
        case 40:
            func_ov007_0223b38c();
            if (SpriteManager::Get().sFade[0] > 1) {
                return;
            }
            func_ov007_0223ce24();
            mUnk_6c->func_0205c7c8(-1);
            mUnk_128 = 10;
            return;
        case 41:
            func_ov007_0223b38c();
            func_ov007_0223c8ac();
            if (mMode != 0) {
                return;
            }
            func_ov007_0223c8d4(1);
            func_02058228(mUnk_7c, 1);
            mUnk_128++;
            mUnk_70->func_0205c7c8(-1);
            mUnk_70->mUnk_064 = 1;
            func_ov007_0223c578();
            mMessage->func_02065890(0x21, 0, 1);
            return;
        case 42: {
            if (mUnk_70->mUnk_061 == 1) {
                func_ov007_0223bb30(0);
            }
            WindowItem *item       = mUnk_70->mCursor->mItems;
            u8 idx;
            BattlePlayerList *list = data_020d433c.mBattlePlayers;
            s8 v                   = list->mUnk_1f8[mUnk_12c][mUnk_12d];
            u8 flag                = 0;
            if (v > 0) {
                idx = v - 1;
                if (list->CurrentPlayer() == list->mUnits[idx].mUnk_0f) {
                    item->mFlags = 5;
                    if (mUnk_13b != 0) {
                        flag = 1;
                        SoundManager::Get().func_020246f8(0);
                    }
                } else {
                    item->mFlags = 0x804;
                    if (mUnk_13b != 0) {
                        SoundManager::Get().func_020246f8(2);
                    }
                }
            } else {
                item->mFlags = 0x804;
            }
            u8 sel   = mUnk_70->Update(1);
            mUnk_13a = mUnk_70->mUnk_0e0;
            if (sel == 1 || flag != 0) {
                mUnk_12f = 0;
                mUnk_11e = 0;
                SpriteManager::Get().StartFade(3);
                mUnk_70->mUnk_064 = 0;
                mUnk_128          = 0x2d;
                data_0220f540.mUnk_00 = 0x12;
                data_0220f540.mTouchDown = mUnk_12c;
                data_0220f540.mTouchX    = mUnk_12d;
                mUnk_129              = 0;
            } else if (sel == 2) {
                mUnk_128++;
                func_ov007_0223c74c(1);
                mUnk_129 = 0;
            }
            mMessage->func_020658d0();
            u8 b                = mUnk_12d;
            u8 a                = mUnk_12c;
            BattlePlayerList *l = data_020d433c.mBattlePlayers;
            l->mUnk_28b         = a;
            l->mUnk_28c         = b;
            func_ov007_0223b38c();
            u8 y = 0;
            mUnk_10->Update();
            do {
                u8 x = 0;
                do {
                    s8 cell = data_020d433c.mBattlePlayers->mUnk_1f8[y][x];
                    if (cell > 0) {
                        BattlePlayerList *l2 = data_020d433c.mBattlePlayers;
                        if (l2->CurrentPlayer() == l2->mUnits[(u8) (cell - 1)].mUnk_0f) {
                            if (mUnk_135 == 0) {
                                mUnk_10->Draw(1, (s16) (mUnk_139 + (y * 16 + 14 - mUnk_120)), (s16) (x * 24 - 22 - mUnk_122), 0);
                            } else {
                                mUnk_10->Draw(1, (s16) (mUnk_139 + ((u8) (13 - y) * 16 + 14 - mUnk_120)), (s16) ((u8) (9 - x) * 24 - 22 - mUnk_122), 0);
                            }
                        }
                    }
                    x++;
                } while (x < 10);
                y++;
            } while (y < 14);
            mUnk_70->Draw();
            func_020254f0(func_ov007_0223cf8c, this);
            return;
        }
        case 43:
            mMessage->func_020658d0();
            func_ov007_0223b38c();
            func_ov007_0223c8ac();
            if (mMode != 0) {
                return;
            }
            mUnk_70->Update(1);
            if (mUnk_70->mUnk_061 != 4) {
                return;
            }
            mNextMode = 2;
            func_ov007_0223c8d4(0);
            func_ov007_02247a3c(mUnk_84);
            mUnk_70->func_0205c7c8(-1);
            mUnk_70->mUnk_064 = 0;
            func_ov007_02246a84(mUnk_84, 2);
            mUnk_128++;
            return;
        case 44:
            mMessage->func_020658d0();
            func_ov007_0223b38c();
            if (SpriteManager::Get().sFade[0] > 1) {
                return;
            }
            func_ov007_0223ce24();
            mUnk_6c->func_0205c7c8(-1);
            mUnk_128 = 10;
            func_ov007_0223ce24();
            return;
        case 45: {
            u8 b                   = mUnk_12d;
            u8 a                   = mUnk_12c;
            BattlePlayerList *list = data_020d433c.mBattlePlayers;
            list->mUnk_28b         = a;
            list->mUnk_28c         = b;
            func_ov007_0223b38c();
            mUnk_70->Draw();
            if (SpriteManager::Get().IsFading()) {
                return;
            }
            func_ov007_0223c74c(1);
            mUnk_11e = 0;
            mUnk_128++;
            return;
        }
        case 46:
            func_ov007_0223b38c();
            func_ov007_0223c8ac();
            if (mMode != 0) {
                return;
            }
            if (!func_ov007_022631bc(data_020d433c.mBattlePlayers, 0xf)) {
                return;
            }
            mNextMode = 2;
            func_ov007_0223c8d4(0);
            func_ov007_02247a3c(mUnk_84);
            func_ov007_02246a84(mUnk_84, 2);
            mUnk_84->mUnk_151 = 0;
            SpriteManager::Get().StartSubFade(2);
            data_020d433c.mGimmicks->mState = 0;
            new BattleCardEffect(0, 0xb, 0xe, 1);
            mContext->mUnk_12 = 0;
            mUnk_128++;
            return;
        case 47: {
            if (mContext->mUnk_12 == 0) {
                return;
            }
            SpriteManager::Get().StartFade(3);
            func_ov007_0223c74c(0);
            BattlePlayerList *list = data_020d433c.mBattlePlayers;
            list->mUnits[(u8) (list->mUnk_1f8[mUnk_12c][mUnk_12d] - 1)].mUnk_12 = 1;
            mUnk_128++;
            return;
        }
        case 48:
            if (!SpriteManager::Get().IsFading()) {
                mUnk_128 = 0x36;
            }
            return;
        case 49:
            new BattleUnk0227532c(1);
            mContext->mUnk_12 = 0;
            mUnk_128++;
            return;
        case 50:
            switch (mContext->mUnk_12) {
                case 1:
                    mUnk_128 = 10;
                    return;
                case 2:
                    if (data_020d433c.mUnk_a8 != 0) {
                        WirelessManager::Get();
                        data_020c4030.mUnk_22a = 6;
                        mUnk_128++;
                        return;
                    }
                    SpriteManager::Get().SetFade(1, -16);
                    mUnk_134 = 7;
                    mUnk_128 = 0x39;
                    return;
            }
            return;
        case 52:
            func_ov007_0223c8ac();
            if (mMode == 0) {
                SpriteManager::Get().SetFade(1, -16);
                mUnk_128 = 0x39;
                mUnk_134 = 1;
            }
            func_ov007_0223b38c();
            return;
        case 53:
            func_ov007_0223c8ac();
            if (mMode == 0) {
                mUnk_134 = 3;
                mUnk_128 = 0x39;
            }
            func_ov007_0223b38c();
            return;
        case 54:
            func_ov007_0223c8ac();
            if (mMode == 0) {
                SpriteManager::Get().SetFade(1, -16);
                mUnk_134 = 2;
                mUnk_128 = 0x39;
            }
            func_ov007_0223b38c();
            return;
        case 55:
            SpriteManager::Get().SetFade(1, -16);
            mUnk_134 = 5;
            mUnk_128 = 0x39;
            return;
        case 56:
            SpriteManager::Get().SetFade(1, -16);
            mUnk_134 = 6;
            mUnk_128 = 0x39;
            return;
        case 57:
            RequestScene(mUnk_134);
            mFinished = 1;
            return;
    }
}

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

void func_ov007_0223a5a4(BattleCommand *command, u32 mode) {
    SpriteManager::Get().func_02026dec(mode, 7);
    G2_SetBG0Offset(0, 0);
    G2_BlendNone();
    GX_SetDispSelect(1);
    GX_SetVisiblePlane(0x11);
    GX_SetGraphicsMode(1, 0, 1);
    G2_SetBG0Priority(0);
    G2_SetBG1Priority(1);
    GX_SetVisibleWnd(0);
}

void func_ov007_0223a690(int x, int y) {
    G2S_SetBG0Control(3, 1, 0, 1, 1);
    G2S_SetBG3Control(1, 0, 4, 5, 0);
    G2S_SetBG0Offset(x + data_ov007_02279aac[0], y + data_ov007_02279aac[1]);
    G2S_SetBG2Offset(0, 0);
    G2S_SetBG3Offset(0, 0);
    G2S_BlendNone();
    GXS_SetVisiblePlane(0x11);
    GXS_SetVisibleWnd(0);
    GXS_SetGraphicsMode(3);
    G2S_SetBG0Priority(3);
}

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
        BattlePlayerList *list = data_020d433c.mBattlePlayers;
        if (mUnk_135 == 0) {
            u8 frame   = list->mPlayers[p]->mUnk_f1 + (p * 4 + 0x28);
            mUnk_2c[p] = new CellAnimation("battle_b_obj_00_nce.bin", frame, 1, 1);
        } else {
            u8 frame   = (list->mPlayers[p]->mUnk_f1 + 2) % 4 + (p * 4 + 0x28);
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
        data_020d433c.mUnk_10->SetupLayout(8);
    }
    ChangeDir("/");
}

class Camera;

void func_ov007_02247a3c(BattleCommandMap *map);

void BattleCommand::func_ov007_0223b2f0() {
    ChangeDir("/Game/Battle/Obj");
    func_02025b64(1, 4, 0x17000, "battle_b_obj_turn.nce_ncg1D.bin", 0, 0, 0);
    func_02025b64(1, 5, 0x120, "battle_b_obj_turn_ncl.bin", 0, 0, 0);
    ChangeDir("/");
}

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
                    if (!(GameSystem::Get().mFrame & 0x20)) {
                        show = FALSE;
                    }
                }
                if (show) {
                    screen.func_02027778((void *) data_ov007_02279ac0, 1, (s16) (x * 16 + 14 - mUnk_120 + mUnk_139), (s16) (y * 24 - 22 - mUnk_122), 1, 0x120, 0, 0);
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

// Record of a battle cell returned by func_ov007_02262530.

void func_ov007_02247a3c(BattleCommandMap *map);

void func_ov007_02246a84(BattleCommandMap *map, u32 a);

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
        if (mUnk_120 > data_ov007_02279acc[field].mMax) {
            mUnk_120 = data_ov007_02279acc[field].mMax;
        } else if (mUnk_120 < data_ov007_02279acc[field].mMin - off) {
            mUnk_120 = data_ov007_02279acc[field].mMin - off;
        }
        if (mUnk_122 > data_ov007_02279aec[field].mMax) {
            mUnk_122 = data_ov007_02279aec[field].mMax;
        } else if (mUnk_122 < data_ov007_02279aec[field].mMin) {
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
            s16 col  = (mUnk_124 - 0xe - mUnk_139) / 16;
            s16 row  = (mUnk_126 + 0x16) / 24;
            if (col >= 0 && col <= 13 && row >= 0 && row <= 9 && mUnk_90[col][row] != 0) {
                if (mUnk_135 == 0) {
                    if (mUnk_12c != col || mUnk_12d != row) {
                        mUnk_12c = col;
                        mUnk_12d = row;
                        moved    = 1;
                    } else if (mUnk_12c == col && mUnk_12d == row) {
                        mUnk_13b = 1;
                    }
                } else {
                    if (mUnk_12c != 13 - col || mUnk_12d != 9 - row) {
                        mUnk_12c = 13 - col;
                        mUnk_12d = 9 - row;
                        moved    = 1;
                    } else if (mUnk_12c == 13 - col && mUnk_12d == 9 - row) {
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

BOOL func_ov007_02262574(BattlePlayerList *list, u8 row, u8 col);

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
            cell = list->CellAt(a, b);
            if (cell > 0) {
                BattleCellInfo *info = func_ov007_02262530(list, a, b);
                u16 sum              = info->mUnk_04 + func_ov007_02262530(list, a, b)->mUnk_06;
                u8 c                 = func_ov007_02262530(data_020d433c.mBattlePlayers, mUnk_12c, mUnk_12d)->mUnk_09;
                u32 d                = func_ov007_02262530(data_020d433c.mBattlePlayers, mUnk_12c, mUnk_12d)->mUnk_08;
                BattleCellInfo *e    = func_ov007_02262530(data_020d433c.mBattlePlayers, mUnk_12c, mUnk_12d);
                BattleCellPair pair = *(BattleCellPair *) e;
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
        SpriteManager::Get().sFade[0] = 3;
    } else {
        SpriteManager::Get();
        SpriteManager::sFade[0]       = 1;
        SpriteManager::sBrightness[0] = -16;
    }
    mMode                = 1;
    mContext->mUnk_12 = 0;
}

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

// Window items of the command screen (modified at run time, so not const): slot 0 the main button, 1..5 the
// commands, 6..9 the player markers.

void BattleCommand::func_ov007_0223c8d4(u32 flag) {
    if (mMode != 0) {
        return;
    }
    func_ov007_0223a5a4(this, 0);
    switch (mNextMode) {
        case 2:
            mUnk_84 = new BattleCommandMap(data_020d433c.mUnk_95);
            mUnk_84->vfunc_0c();
            SpriteManager::Get().sFade[0] = 2;
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

void func_ov007_0223cf8c(void *arg) {
    BattleCommand *command = (BattleCommand *) arg;
    int adjust             = data_020d43f0[0].mUnk_28_5 ? 0 : 4;
    G2S_SetBG0Offset(command->mUnk_120 + data_ov007_02279aac[0] - adjust, command->mUnk_122 + data_ov007_02279aac[1]);
}
