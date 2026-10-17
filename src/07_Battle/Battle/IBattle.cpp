#include "Battle/BattleBase.hpp"
#include "Battle/BattleUtil.hpp"

#include "Game/UnkStruct_020d433c.hpp"
#include "System/TouchManager.hpp"
#include "System/WirelessManager.hpp"

extern "C" s32 FX_Atan2Idx(s32 y, s32 x);

// Battle message of the communication buffer data_0220f540 (the layout used by the battle).
struct BattleNetMessage {
    /* 00 */ u8 mType;
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

void WriteNetMessage(BattleBase *scene, u16 *a, u16 *b, u16 *c, u8 flag) {
    BattleNetMessage *msg = (BattleNetMessage *) &data_0220f540;
    msg->mType = 0xe;
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

void ApplyNetMessages(BattleBase *scene) {
    u8 i = 0;
    u32 count = WirelessManager::Get().GetRecvBlockCount();
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
    scene->UpdateSlotsFromMessages(first, second, third, &flag);
    if (flag != 0) {
        scene->func_ov007_0221cebc((u8) (flag - 1));
    }
    scene->TryStartCardSequenceFromMessages(third);
    scene->func_ov007_0221d088(0, first);
    scene->func_ov007_0221d088(1, second);
}

