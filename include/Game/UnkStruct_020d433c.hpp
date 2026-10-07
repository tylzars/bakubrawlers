#pragma once

#include "global.h"
#include "types.h"

#include <nitro/fx.h>

class UnkStruct_02078290;
class Camera;
class UnkStruct_Battle2a4; // object at BattlePlayerList+0x2a4, drives a timed screen effect

// Battle message a player sends over the communication link (type 0xe).
struct BattlePlayerMessage {
    /* 00 */ u8 mType; // 0xe = a battle message is pending
    /* 01 */ PAD(0x01, 0x04);
    /* 04 */ u16 mUnk_04[2];
    /* 08 */ PAD(0x08, 0x0c);
    /* 0c */ u16 mUnk_0c[2];
    /* 10 */ PAD(0x10, 0x14);
    /* 14 */ u16 mUnk_14;
    /* 16 */ u16 mUnk_16;
    /* 18 */ u8 mUnk_18;
    /* 19 */
};

// 12 byte per-slot record of a player, the second field is a 12 bit gauge.
struct BattleRecord {
    /* 00 */ u16 mUnk_00_0 : 4;
    /* 00 */ u16 mGauge : 12;
    /* 02 */ PAD(0x02, 0x04);
    /* 04 */ u16 mUnk_04;
    /* 06 */ PAD(0x06, 0x09);
    /* 09 */ u8 mUnk_09;
    /* 0a */ PAD(0x0a, 0x0c);
};

struct BattlePlayer {
    /* 00 */ PAD(0x00, 0x0c);
    /* 0c */ BattleRecord mRecords[15];
    /* c0 */ PAD(0xc0, 0xc8);
    /* c8 */ void *mUnk_c8;
    /* cc */ PAD(0xcc, 0xee);
    /* ee */ u8 mUnk_ee;
    /* ef */ PAD(0xef, 0xf2);
    /* f2 */ u8 mUnk_f2;
    /* f3 */ u8 mUnk_f3;
    /* f4 */ PAD(0xf4, 0xf5);
    /* f5 */ u8 mRecord;
    /* f6 */ u8 mUnk_f6;
    /* f7 */ u8 mUnk_f7;
    /* f8 */ PAD(0xf8, 0xfa);
    /* fa */ u16 mUnk_fa;
    /* fc */ u8 mUnk_fc;
    /* fd */ u8 mUnk_fd;
    /* fe */ PAD(0xfe, 0x100);
    /* 100 */ BattlePlayerMessage mMsg;
    /* 119 */

    bool IsActive() const {
        bool active = false;
        if (mUnk_c8) {
            active = true;
        }
        return active;
    }
};

struct BattleVecHolder4 {
    /* 00 */ PAD(0x00, 0x04);
    /* 04 */ VecFx32 mPos;
    /* 10 */ PAD(0x10, 0x4a);
    /* 4a */ u8 mUnk_4a;
};

struct BattleVecHolder8 {
    /* 00 */ PAD(0x00, 0x08);
    /* 08 */ VecFx32 mPos;
};

struct BattleTarget {
    /* 00 */ PAD(0x00, 0x38);
    /* 38 */ u32 mUnk_38;
    /* 3c */ PAD(0x3c, 0x44);
    /* 44 */ u32 mUnk_44;
    /* 48 */ PAD(0x48, 0x4c);
    /* 4c */ u32 mUnk_4c;
    /* 50 */ PAD(0x50, 0x64);
    /* 64 */ VecFx32 mUnk_64;
    /* 70 */ PAD(0x70, 0xa0);
    /* a0 */ VecFx32 mUnk_a0;
    /* ac */ PAD(0xac, 0x174);
    /* 174 */ BattleVecHolder8 *mUnk_174; // vector at +08
    /* 178 */ PAD(0x178, 0x1a4);
    /* 1a4 */ BattleVecHolder4 *mUnk_1a4; // vector at +04
    /* 1a8 */ PAD(0x1a8, 0x1bc);
    /* 1bc */ u16 mUnk_1bc;
    /* 1be */ PAD(0x1be, 0x1c6);
    /* 1c6 */ u16 mUnk_1c6;
    /* 1c8 */ PAD(0x1c8, 0x1da);
    /* 1da */ u8 mUnk_1da;
    /* 1db */ PAD(0x1db, 0x1dd);
    /* 1dd */ u8 mUnk_1dd;
};

struct BattleMarker {
    /* 0 */ PAD(0, 2);
    /* 2 */ u8 mUnk_2;
    /* 3 */ u8 mUnk_3;
    /* 4 */ u8 mUnk_4;
    /* 5 */ PAD(5, 8);
};

struct BattlePlayerList {
    /* 000 */ PAD(0x000, 0x00c);
    /* 00c */ BattlePlayer *mPlayers[4];
    /* 01c */ BattleMarker mUnk_1c[12];
    /* 07c */ PAD(0x07c, 0x28a);
    /* 28a */ u8 mUnk_28a;
    /* 28b */ PAD(0x28b, 0x298);
    /* 298 */ BattleTarget *mUnk_298;
    /* 29c */ PAD(0x29c, 0x2a0);
    /* 2a0 */ Camera *mCamera;
    /* 2a4 */ UnkStruct_Battle2a4 *mUnk_2a4;
    /* 2a8 */ PAD(0x2a8, 0x2a9);
    /* 2a9 */ u8 mUnk_2a9;
    /* 2aa */ PAD(0x2aa, 0x2af);
    /* 2af */ u8 mUnk_2af;
    /* 2b0 */ u8 mUnk_2b0;

    BattlePlayer *Current() {
        return mPlayers[mUnk_28a];
    }

    BattleMarker *Entries() {
        return mUnk_1c;
    }
};

class EffectManager; // particle effect list (ctor 0202ab24 creates the 0x61c byte effects)

class GimmickManager;

struct UnkStruct_020d433c_0c; // opaque, only passed to func_ov007_02271ca8

struct UnkStruct_020d433c {
    /* 00 */ BattlePlayerList *mBattlePlayers;
    /* 04 */ EffectManager *mEffects;
    /* 08 */ GimmickManager *mGimmicks;
    /* 0c */ UnkStruct_020d433c_0c *mUnk_0c;
    /* 10 */ UnkStruct_02078290 *mUnk_10;
    union {
        /* 14 */ u16 mUnk_14;
        struct {
            /* 14 */ u16 mUnk_14_0 : 1;
            /* 14 */ u16 mUnk_14_1 : 1;
            /* 14 */ u16 mUnk_14_2 : 14;
        };
    };
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
