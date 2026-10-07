#pragma once

#include "global.h"
#include "types.h"

#include <nitro/fx.h>

#include "Scene/SceneBase.hpp"

class UnkStruct_02078290;
class Camera;
class BattleTarget;
class UnkStruct_Battle2a4; // object at BattlePlayerList+0x2a4, drives a timed screen effect

// Battle message a player sends over the communication link (type 0xe).
struct BattlePlayerMessage {
    /* 00 */ u8 mType; // 0xe = a battle message is pending
    /* 01 */ PAD(0x01, 0x04);
    union {
        /* 04 */ u16 mUnk_04[2];
        struct {
            /* 04 */ u8 mTouchDown;
            /* 05 */ u8 mTouchX;
            /* 06 */ u8 mTouchY;
        };
    };
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
    /* 02 */ u16 mUnk_02_0 : 3;
    /* 02 */ u16 mUnk_02_3 : 3;
    /* 02 */ u16 mUnk_02_6 : 3;
    /* 02 */ u16 mUnk_02_9 : 3;
    /* 02 */ u16 mUnk_02_12 : 4;
    /* 04 */ u16 mUnk_04;
    /* 06 */ u16 mUnk_06;
    /* 08 */ u8 mUnk_08;
    /* 09 */ u8 mUnk_09;
    /* 0a */ u8 mUnk_0a;
    /* 0b */ u8 mUnk_0b;
};

// The object a player controls on the field (only the timer at +74 is known).
struct BattleFighter {
    /* 00 */ PAD(0x00, 0x74);
    /* 74 */ u16 mUnk_74;
    /* 76 */ PAD(0x76, 0x78);
};

struct BattlePlayer {
    /* 00 */ PAD(0x00, 0x0c);
    /* 0c */ BattleRecord mRecords[15];
    /* c0 */ PAD(0xc0, 0xc8);
    /* c8 */ BattleFighter *mUnk_c8;
    /* cc */ u16 mUnk_cc[3];
    /* d2 */ PAD(0xd2, 0xd4);
    /* d4 */ s32 mUnk_d4;
    /* d8 */ s32 mUnk_d8;
    /* dc */ u32 mUnk_dc;
    /* e0 */ u32 mUnk_e0;
    /* e4 */ u32 mUnk_e4;
    /* e8 */ u32 mUnk_e8;
    /* ec */ u8 mUnk_ec;
    /* ed */ u8 mUnk_ed; // length of the name at +b0
    /* ee */ u8 mUnk_ee;
    /* ef */ PAD(0xef, 0xf1);
    /* f1 */ u8 mUnk_f1;
    /* f2 */ u8 mUnk_f2;
    /* f3 */ u8 mUnk_f3;
    /* f4 */ PAD(0xf4, 0xf5);
    /* f5 */ u8 mRecord;
    /* f6 */ u8 mUnk_f6;
    /* f7 */ u8 mUnk_f7;
    /* f8 */ u8 mUnk_f8;
    /* f9 */ PAD(0xf9, 0xfa);
    /* fa */ u16 mUnk_fa;
    /* fc */ u8 mUnk_fc;
    /* fd */ u8 mUnk_fd;
    /* fe */ u8 mUnk_fe;
    /* ff */ PAD(0xff, 0x100);
    /* 100 */ BattlePlayerMessage mMsg;
    /* 11a */ u8 mUnk_11a[0x26];
    /* 140 */

    BattlePlayer(u32 index); // 022696b4

    // Name text at +b0 and the pointer-sized value at +a8 (both overlap mRecords, which is probably only 4 long).
    const u8 *NameText() {
        return (const u8 *) this + 0xb0;
    }
    u32 NameInfo() {
        return (u32) ((u8 *) this + 0xa8);
    }

    // u16 at +54 + 4 * i (overlaps mRecords).
    u16 &Slot54(int i) {
        return *(u16 *) ((u8 *) this + 0x54 + i * 4);
    }

    // u16 at +6c + 4 * i (overlaps mRecords).
    u16 &Slot6c(int i) {
        return *(u16 *) ((u8 *) this + 0x6c + i * 4);
    }

    // Byte at +56 + 4 * i (a flag per bakugan slot, 4 bytes apart; overlaps mRecords).
    u8 &Flag56(int i) {
        return ((u8 *) this)[0x56 + i * 4];
    }

    bool IsActive() const {
        bool active = false;
        if (mUnk_c8) {
            active = true;
        }
        return active;
    }
};

// 0x14 byte entry of the unit table at BattlePlayerList+0x7c.
struct BattleUnit {
    /* 00 */ VecFx32 mPos;
    /* 0c */ u16 mUnk_0c;
    /* 0e */ u8 mUnk_0e;
    /* 0f */ u8 mUnk_0f;
    /* 10 */ u8 mUnk_10;
    /* 11 */ u8 mUnk_11;
    /* 12 */ u8 mUnk_12;
    /* 13 */ PAD(0x13, 0x14);
};

struct BattleUnk29c {
    /* 00 */ PAD(0x00, 0x9e);
    /* 9e */ u8 mUnk_9e;
    /* 9f */ PAD(0x9f, 0xa0);
    /* a0 */ u8 mUnk_a0;
    /* a1 */ PAD(0xa1, 0xa4);
    /* a4 */

    BattleUnk29c();  // 0226fb74
    ~BattleUnk29c(); // 0226fc18
};

struct BattleMarker {
    /* 0 */ u8 mUnk_0;
    /* 1 */ u8 mUnk_1;
    /* 2 */ u8 mUnk_2;
    /* 3 */ u8 mUnk_3;
    /* 4 */ u8 mUnk_4;
    /* 5 */ u8 mUnk_5;
    /* 6 */ PAD(6, 8);
};

// The battle scene itself (0x2b4 bytes, vtable 02289e78, constructor 0225fd5c); data_020d433c.mBattlePlayers points to it.
struct BattlePlayerList : public SceneBase {
    /* 000 SceneBase */
    /* 00c */ BattlePlayer *mPlayers[4];
    /* 01c */ BattleMarker mUnk_1c[12];
    /* 07c */ BattleUnit mUnits[12];
    /* 16c */ s8 mUnk_16c[14][10];
    /* 1f8 */ s8 mUnk_1f8[14][10];
    /* 284 */ u8 mUnk_284[4];
    /* 288 */ u16 mUnk_288;
    /* 28a */ u8 mUnk_28a;
    /* 28b */ u8 mUnk_28b;
    /* 28c */ u8 mUnk_28c;
    /* 28d */ u8 mUnk_28d;
    /* 28e */ u8 mUnk_28e;
    /* 28f */ u8 mUnk_28f;
    /* 290 */ PAD(0x290, 0x292);
    /* 292 */ u8 mUnk_292;
    /* 293 */ PAD(0x293, 0x294);
    /* 294 */ u8 mUnk_294;
    /* 295 */ u8 mUnk_295;
    /* 296 */ PAD(0x296, 0x298);
    /* 298 */ BattleTarget *mUnk_298;
    /* 29c */ BattleUnk29c *mUnk_29c;
    /* 2a0 */ Camera *mCamera;
    /* 2a4 */ UnkStruct_Battle2a4 *mUnk_2a4;
    /* 2a8 */ u8 mUnk_2a8;
    /* 2a9 */ u8 mUnk_2a9;
    /* 2aa */ u8 mUnk_2aa;
    /* 2ab */ u8 mUnk_2ab;
    /* 2ac */ u8 mUnk_2ac;
    /* 2ad */ u8 mUnk_2ad;
    /* 2ae */ u8 mUnk_2ae;
    /* 2af */ u8 mUnk_2af;
    /* 2b0 */ u8 mUnk_2b0;
    /* 2b1 */ u8 mUnk_2b1;
    /* 2b2 */ PAD(0x2b2, 0x2b4);

    BattlePlayerList(); // 0225fd5c
    virtual ~BattlePlayerList();
    /* 08 */ virtual void Update(); // 0226026c

    BattlePlayer *Current() {
        return mPlayers[mUnk_28a];
    }

    BattleUnit *Units() {
        return mUnits;
    }

    BattleMarker *Entries() {
        return mUnk_1c;
    }
};

void func_ov007_022628e0(BattlePlayerList *list, u8 index, const VecFx32 *pos, u8 marker);
void func_ov007_02262a70(BattlePlayerList *list, u8 slot, u8 player);
void func_ov007_02262828(BattlePlayerList *list, u8 index, u8 value);

class EffectManager; // particle effect list (ctor 0202ab24 creates the 0x61c byte effects)

class GimmickManager;

struct UnkStruct_020d433c_0c { // 0x214 bytes, mostly opaque (passed to func_ov007_02271ca8)
    /* 000 */ PAD(0x000, 0x212);
    /* 212 */ u8 mUnk_212; // set while the shoot screen is shown
    /* 213 */ PAD(0x213, 0x214);

    UnkStruct_020d433c_0c(); // 02271bb4
};

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
