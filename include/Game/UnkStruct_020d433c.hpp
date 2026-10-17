#pragma once

#include "global.h"
#include "types.h"

#include <nitro/fx.h>

#include "Scene/SceneBase.hpp"

class AntennaIndicator;
class Camera;
class BattleTarget;
class StorytalkWindow; // UI/StorytalkWindow.hpp

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
    /* 08 */ u8 mUnk_08;
    /* 09 */ u8 mUnk_09;
    /* 0a */ PAD(0x0a, 0x0c);
    union {
        /* 0c */ u16 mUnk_0c[2];
        struct {
            /* 0c */ PAD(0x0c, 0x0e);
            /* 0e */ u8 mUnk_0e;
        };
    };
    /* 10 */ PAD(0x10, 0x14);
    /* 14 */ u16 mUnk_14;
    /* 16 */ u16 mUnk_16;
    /* 18 */ u8 mUnk_18;
    /* 19 */
};

// 4 byte per-slot entry of a player.
struct BattlePlayerSlot {
    /* 00 */ u16 mValue;
    /* 02 */ u8 mFlag;
    /* 03 */ u8 mUnk_03;
};

// 12 byte per-slot record of a player, the second field is a 12 bit gauge.
// The first four bytes as a copyable unit (same layout as the save entry, UnkStruct_02023248).
struct BattleRecordHead {
    /* 00 */ u16 mUnk_00_0 : 4;
    /* 00 */ u16 mGauge : 12;
    /* 02 */ u16 mUnk_02_0 : 3;
    /* 02 */ u16 mUnk_02_3 : 3;
    /* 02 */ u16 mUnk_02_6 : 3;
    /* 02 */ u16 mUnk_02_9 : 3;
    /* 02 */ u16 mUnk_02_12 : 4;
};

struct BattleRecord {
    union {
        /* 00 */ BattleRecordHead mHead;
        struct {
            /* 00 */ u16 mUnk_00_0 : 4;
            /* 00 */ u16 mGauge : 12;
            /* 02 */ u16 mUnk_02_0 : 3;
            /* 02 */ u16 mUnk_02_3 : 3;
            /* 02 */ u16 mUnk_02_6 : 3;
            /* 02 */ u16 mUnk_02_9 : 3;
            /* 02 */ u16 mUnk_02_12 : 4;
        };
    };
    /* 04 */ u16 mUnk_04;
    /* 06 */ u16 mUnk_06;
    /* 08 */ u8 mBakuganId;
    /* 09 */ u8 mAttribute;
    /* 0a */ u8 mStatus;
    /* 0b */ u8 mUnk_0b;
};

class BattleCpu; // the object a player controls on the field (ctor 02230f58)

struct BattlePlayer : public SceneBase {
    /* 00 SceneBase */
    union {
        /* 0c */ BattleRecord mRecords[15];
        struct {
            /* 0c */ PAD(0x0c, 0x54);
            /* 54 */ BattlePlayerSlot mSlots54[3]; // value, flag and a byte per slot (second slot table at +6c)
            /* 60 */ PAD(0x60, 0x6c);
            /* 6c */ BattlePlayerSlot mSlots6c[3];
        };
    };
    /* c0 */ PAD(0xc0, 0xc8);
    /* c8 */ BattleCpu *mCpu;
    /* cc */ u16 mUnk_cc[3];
    /* d2 */ PAD(0xd2, 0xd4);
    /* d4 */ s32 mUnk_d4;
    /* d8 */ s32 mUnk_d8;
    /* dc */ u32 mUnk_dc;
    /* e0 */ u32 mUnk_e0;
    /* e4 */ u32 mUnk_e4;
    /* e8 */ u32 mUnk_e8;
    /* ec */ u8 mUnk_ec;
    /* ed */ u8 mNameLength; // length of the name at +b0
    /* ee */ u8 mUnk_ee;
    /* ef */ u8 mState;
    /* f0 */ u8 mIndex;
    /* f1 */ u8 mUnk_f1;
    /* f2 */ u8 mPartner;
    /* f3 */ u8 mUnk_f3;
    /* f4 */ u8 mHistoryCount;
    /* f5 */ u8 mRecord;
    /* f6 */ u8 mCharacterId;
    /* f7 */ u8 mUnk_f7;
    /* f8 */ u8 mSlots54Remaining;
    /* f9 */ u8 mSlots6cRemaining;
    /* fa */ u16 mUnk_fa;
    /* fc */ u8 mUnk_fc;
    /* fd */ u8 mUnk_fd;
    /* fe */ u8 mUnk_fe;
    /* ff */ PAD(0xff, 0x100);
    /* 100 */ BattlePlayerMessage mMsg;
    /* 11a */ u8 mUnk_11a[0x26];
    /* 140 */

    BattlePlayer(u32 index); // 022696b4
    virtual ~BattlePlayer();
    /* 08 */ virtual void Update(); // 02269c98

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

    // Appends (id, kind) to the 6-byte history entries at +84; mHistoryCount counts them.
    void PushHistory(u16 id, u8 kind) {
        *(u16 *) ((u8 *) this + mHistoryCount * 6 + 0x84) = id;
        *((u8 *) this + mHistoryCount * 6 + 0x86)          = kind;
        mHistoryCount++;
    }

    u8 RecordIndex() {
        return mRecord;
    }
    BattleRecord *CurrentRecord() {
        return &mRecords[mRecord];
    }

    bool IsActive() const {
        bool active = false;
        if (mCpu) {
            active = true;
        }
        return active;
    }

    // Index of the allied player (own side = this player or its partner).
    u8 Partner() {
        return mPartner;
    }
};

// 0x14 byte entry of the unit table at BattlePlayerList+0x7c.
struct BattleUnit {
    /* 00 */ VecFx32 mPos;
    /* 0c */ u16 mUnk_0c;
    /* 0e */ u8 mRecord;
    /* 0f */ u8 mPlayer;
    /* 10 */ u8 mStatus;
    /* 11 */ u8 mMarker;
    /* 12 */ u8 mUnk_12;
    /* 13 */ PAD(0x13, 0x14);
};

#include "Battle/BattleUnk29c.hpp"

struct BattleMarker {
    /* 0 */ u8 mSlot;
    /* 1 */ u8 mPlayer;
    /* 2 */ u8 mStatus;
    /* 3 */ u8 mCellX;
    /* 4 */ u8 mCellY;
    /* 5 */ u8 mUnitHandle;
    /* 6 */ PAD(6, 8);
};

// The battle scene itself (0x2b4 bytes, vtable 02289e78, constructor 0225fd5c); data_020d433c.mBattlePlayers points to it.
struct BattlePlayerList : public SceneBase {
    /* 000 SceneBase */
    /* 00c */ BattlePlayer *mPlayers[4];
    /* 01c */ BattleMarker mMarkers[12];
    /* 07c */ BattleUnit mUnits[12];
    /* 16c */ s8 mMarkerHandles[14][10];
    /* 1f8 */ s8 mUnitHandles[14][10];
    /* 284 */ u8 mTurnOrder[4];
    /* 288 */ u16 mTurnCount;
    /* 28a */ u8 mTurnPlayer;
    /* 28b */ u8 mUnk_28b;
    /* 28c */ u8 mUnk_28c;
    /* 28d */ u8 mUnitA;
    /* 28e */ u8 mUnitB;
    /* 28f */ u8 mUnk_28f;
    /* 290 */ PAD(0x290, 0x291);
    /* 291 */ u8 mUnk_291;
    /* 292 */ u8 mUnk_292;
    /* 293 */ u8 mUnk_293;
    /* 294 */ u8 mMarkerCount;
    /* 295 */ u8 mUnitCount;
    /* 296 */ PAD(0x296, 0x298);
    /* 298 */ BattleTarget *mBattleTarget;
    /* 29c */ BattleUnk29c *mUnk_29c;
    /* 2a0 */ Camera *mCamera;
    /* 2a4 */ StorytalkWindow *mUnk_2a4;
    /* 2a8 */ u8 mState;
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
        return mPlayers[mTurnPlayer];
    }

    // Index of the player whose units are being commanded (compared with BattleUnit::mUnk_0f).
    u8 CurrentPlayer() {
        return mUnk_2a9;
    }

    u8 Unk294() {
        return mMarkerCount;
    }
    BattleUnit *Units() {
        return mUnits;
    }

    BattleMarker *Entries() {
        return mMarkers;
    }

    s8 CellAt(u8 row, u8 col) {
        return mUnitHandles[row][col];
    }

    void SetCursor(u8 x, u8 y) {
        mUnk_28b = x;
        mUnk_28c = y;
    }
};

void MoveUnit(BattlePlayerList *list, u8 index, const VecFx32 *pos, u8 marker);
void SetTurnOrder(BattlePlayerList *list, u8 slot, u8 player);
void RemoveUnit(BattlePlayerList *list, u8 index, u8 value);

class EffectManager; // particle effect list (ctor 0202ab24 creates the 0x61c byte effects)

class GimmickManager;

struct GimmickSetData;
struct BattleObstacle;

// Collision manager of the gimmick set files (0x214 bytes, vtable 0228abfc, ctor 02271bb4): a list of up to 0x7f loaded
// sets (slot 0 is the list head) handed out through a ring of slot pointers; Update tests the sets against each other.
struct GimmickCollision : public SceneBase {
    /* 00 SceneBase */
    /* 0c */ GimmickSetData *mSets; // 0x80 slots of 0x838 bytes
    /* 10 */ GimmickSetData *mSetRing[0x80]; // slot ring
    /* 210 */ u8 mRingWriteIndex; // ring position where a released slot is put back
    /* 211 */ u8 mRingReadIndex; // ring position of the next slot to hand out
    /* 212 */ u8 mEnabled; // set while the shoot screen is shown
    /* 213 */ PAD(0x213, 0x214);

    GimmickCollision(); // 02271bb4
    virtual ~GimmickCollision();
    /* 08 */ virtual void Update(); // 02271db0

    void TestSets(GimmickSetData *a, GimmickSetData *b);
    BOOL TestObstacles(BattleObstacle *a, BattleObstacle *b);
};

struct UnkStruct_020d433c {
    /* 00 */ BattlePlayerList *mBattlePlayers;
    /* 04 */ EffectManager *mEffects;
    /* 08 */ GimmickManager *mGimmicks;
    /* 0c */ GimmickCollision *mGimmickCollision;
    /* 10 */ AntennaIndicator *mUnk_10;
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
    /* 79 */ u8 mLanguage;
    /* 7a */ u8 mUnk_7a;
    /* 7b */ u8 mNextScene;
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
    /* 85 */ u8 mTutorialId;
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
    /* 8d */ u8 mUnk_8d[8];
    /* 95 */ u8 mFieldId;
    /* 96 */ s8 mUnk_96;
    /* 97 */ u8 mPlayerCount;
    /* 98 */ u8 mHasPartners;
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
    /* 9b */ u8 mPartners[4];
    /* 9f */ u8 mCharacterIds[4];
    /* a3 */ u8 mUnk_a3[4];
    /* a7 */ u8 mUnk_a7;
    /* a8 */ u8 mWireless;
    /* a9 */ u8 mUnk_a9[4];
    /* ad */ u8 mUnk_ad;
    /* ae */ u8 mUnk_ae;
    /* af */ u8 mUnk_af[4];
    /* af */
};

extern UnkStruct_020d433c data_020d433c;
