#pragma once

#include "global.h"
#include "types.h"

class UnkStruct_02078290;

struct BattlePlayer {
    /* 00 */ PAD(0x00, 0xc8);
    /* c8 */ void *mUnk_c8;
    /* cc */ PAD(0xcc, 0xee);
    /* ee */ u8 mUnk_ee;
    /* ef */ PAD(0xef, 0xf2);
    /* f2 */ u8 mUnk_f2;
    /* f3 */ PAD(0xf3, 0xf6);
    /* f6 */ u8 mUnk_f6;
    /* f7 */ u8 mUnk_f7;
    /* f8 */

    bool IsActive() const {
        bool active = false;
        if (mUnk_c8) {
            active = true;
        }
        return active;
    }
};

struct BattlePlayerList {
    /* 000 */ PAD(0x000, 0x00c);
    /* 00c */ BattlePlayer *mPlayers[4];
    /* 01c */ PAD(0x01c, 0x2a9);
    /* 2a9 */ u8 mUnk_2a9;
};

struct UnkStruct_020d433c {
    /* 00 */ BattlePlayerList *mBattlePlayers;
    /* 04 */ PAD(0x04, 0x10);
    /* 10 */ UnkStruct_02078290 *mUnk_10;
    /* 14 */ u16 mUnk_14_0 : 1;
    /* 14 */ u16 mUnk_14_1 : 1;
    /* 14 */ u16 mUnk_14_2 : 14;
    /* 16 */ u16 mNames[4][12]; // UTF-16 player names
    /* 76 */ u8 mUnk_76;
    /* 77 */ u8 mUnk_77;
    /* 78 */ PAD(0x78, 0x79);
    /* 79 */ u8 mUnk_79;
    /* 7a */ u8 mUnk_7a;
    /* 7b */ u8 mUnk_7b;
    /* 7c */ u8 mUnk_7c;
    /* 7d */ u8 mUnk_7d;
    /* 7e */ u8 mUnk_7e_0 : 4;
    /* 7e */ u8 mUnk_7e_4 : 4;
    /* 7f */ u8 mUnk_7f;
    /* 80 */ u8 mUnk_80;
    /* 81 */ u8 mUnk_81;
    /* 82 */ u8 mUnk_82;
    /* 83 */ u8 mUnk_83;
    /* 84 */ u8 mUnk_84;
    /* 85 */ u8 mUnk_85;
    /* 86 */ u8 mUnk_86;
    /* 87 */ u8 mUnk_87;
    /* 88 */ u8 mUnk_88_0 : 4;
    /* 88 */ u8 mUnk_88_4 : 4;
    /* 89 */ u8 mUnk_89;
    /* 8a */ u8 mUnk_8a_0 : 4;
    /* 8a */ u8 mUnk_8a_4 : 4;
    /* 8b */ u8 mUnk_8b_0 : 4;
    /* 8b */ u8 mUnk_8b_4 : 4;
    /* 8c */ u8 mUnk_8c_0 : 4;
    /* 8c */ u8 mUnk_8c_4 : 4;
    /* 8d */ PAD(0x8d, 0x95);
    /* 95 */ u8 mUnk_95;
    /* 96 */ s8 mUnk_96;
    /* 97 */ u8 mPlayerCount;
    /* 98 */ u8 mUnk_98;
    /* 99 */ u8 mUnk_99;
    union {
        /* 9a */ u8 mUnk_9a;
        struct {
            /* 9a */ u8 mUnk_9a_0 : 1;
            /* 9a */ u8 mUnk_9a_1 : 1;
            /* 9a */ u8 mUnk_9a_2 : 1;
            /* 9a */ u8 mUnk_9a_3 : 1;
            /* 9a */ u8 mUnk_9a_4 : 1;
            /* 9a */ u8 mUnk_9a_5 : 2;
            /* 9a */ u8 mUnk_9a_7 : 1;
        };
    };
    /* 9b */ u8 mUnk_9b[4];
    /* 9f */ u8 mUnk_9f[4];
    /* a3 */ u8 mUnk_a3[4];
    /* a7 */ u8 mUnk_a7;
    /* a8 */ u8 mUnk_a8;
    /* a9 */ u8 mUnk_a9[4];
    /* ad */ u8 mUnk_ad;
    /* ae */ u8 mUnk_ae;
    /* af */ u8 mUnk_af[4];
    /* af */
};

extern UnkStruct_020d433c data_020d433c;
