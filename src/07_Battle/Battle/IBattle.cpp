#include "Battle/BattleBase.hpp"
#include "Battle/BattleUtil.hpp"

#include "Game/UnkStruct_020d433c.hpp"
#include "System/TouchManager.hpp"
#include "System/WirelessManager.hpp"

extern "C" s32 FX_Atan2Idx(s32 y, s32 x);

// Battle message of the communication buffer data_0220f540 (the layout used by the battle).
struct BattleNetMessage {
    /* 00 */ u8 mUnk_00;
    /* 01 */ PAD(0x01, 0x04);
    /* 04 */ u16 mUnk_04[4];
    /* 0c */ u16 mUnk_0c[4];
    /* 14 */ u16 mUnk_14[2];
    /* 18 */ u8 mUnk_18;
};

IBattle::IBattle() {
}

IBattle::~IBattle() {
}

void func_ov007_02229c88(BattleBase *scene, u16 *a, u16 *b, u16 *c, u8 flag) {
    BattleNetMessage *msg = (BattleNetMessage *) &data_0220f540;
    msg->mUnk_00 = 0xe;
    msg->mUnk_04[0] = a[0];
    msg->mUnk_04[1] = a[1];
    msg->mUnk_04[2] = a[2];
    msg->mUnk_04[3] = a[3];
    msg->mUnk_0c[0] = b[0];
    msg->mUnk_0c[1] = b[1];
    msg->mUnk_0c[2] = b[2];
    msg->mUnk_0c[3] = b[3];
    msg->mUnk_14[0] = c[0];
    msg->mUnk_14[1] = c[1];
    msg->mUnk_14[0] = c[0];
    msg->mUnk_14[1] = c[1];
    msg->mUnk_18 = flag;
}

void func_ov007_02229d08(BattleBase *scene) {
    u8 i = 0;
    u32 count = WirelessManager::Get().func_0202f134();
    u16 first[4] = {0, 0, 0, 0};
    u16 second[4] = {0, 0, 0, 0};
    u16 third[2] = {0, 0};
    u8 flag = 0;
    BattlePlayerList *list = data_020d433c.mBattlePlayers;
    do {
        BattlePlayer *player = list->mPlayers[i];
        if (player->mMsg.mType != 0xe) {
            break;
        }
        BattlePlayerMessage *msg = &player->mMsg;
        first[i] = msg->mUnk_04[i];
        second[i] = msg->mUnk_0c[i];
        if (msg->mUnk_14 != 0) {
            third[0] = msg->mUnk_14;
        } else if (msg->mUnk_16 != 0) {
            third[1] = msg->mUnk_16;
        }
        if (msg->mUnk_18 != 0) {
            flag = msg->mUnk_18;
        }
        i++;
    } while (i < count);
    scene->func_ov007_0221ced0(first, second, third);
    scene->func_ov007_0221cfac(first, second);
    scene->func_ov007_0221d494(first, second, third, &flag);
    if (flag != 0) {
        scene->func_ov007_0221cebc((u8) (flag - 1));
    }
    scene->func_ov007_0221d488(third);
    scene->func_ov007_0221d088(0, first);
    scene->func_ov007_0221d088(1, second);
}

u8 func_ov007_02229ee4(s32 x, s32 y, s32 width, s32 height, u16 unk5, u8 *flag) {
    u8 hit = 0;
    if (TouchManager::Get().mUnk_56 == 0) {
        *flag = 0;
        return 0;
    }
    s32 touchX = (s16) TouchManager::Get().mX;
    s32 touchY = (s16) TouchManager::Get().mY;
    s32 downX = TouchManager::Get().mUnk_08;
    if (func_ov007_022252d0(downX, TouchManager::Get().mUnk_0a, touchX, touchY) < unk5) {
        if (*flag != 0) {
            (*flag)--;
            hit = 1;
        }
        return hit;
    }
    if (!TouchManager::Get().func_02022830((s16) x, (s16) y, (s16) width, (s16) height)) {
        if (*flag != 0) {
            (*flag)--;
            hit = 1;
        }
        return hit;
    }
    *flag = 5;
    return 1;
}

static u32 sTouchTicks;

s32 func_ov007_0222a118(s32 x, s32 y, s32 radius) {
    s32 angle = 0;
    if (TouchManager::Get().mUnk_56 != 0) {
        sTouchTicks++;
    } else {
        sTouchTicks = 0;
    }
    if (sTouchTicks >= 2) {
        s32 dx = TouchManager::Get().mX - x;
        s32 dy = TouchManager::Get().mY - y;
        if (dx * dx + dy * dy <= radius * radius) {
            angle = FX_Atan2Idx(dy << 12, dx << 12);
            s32 startX = (u16) TouchManager::Get().mUnk_08 - x;
            angle -= FX_Atan2Idx(((u16) TouchManager::Get().mUnk_0a - y) << 12, startX << 12);
            if (angle > 0x8000) {
                angle -= 0x10000;
            } else if (angle < -0x8000) {
                angle += 0x10000;
            }
        } else {
            sTouchTicks = 0;
        }
    }
    return angle;
}
