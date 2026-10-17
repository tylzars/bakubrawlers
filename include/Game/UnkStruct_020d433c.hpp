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
    /* 09 */ u8 mAttribute; // also written raw by BattleAbility (rec2[0x15])
    /* 0a */ u8 mStatus; // 0 free, 1 placed, 2 (BattlePlayer set-up, RemoveUnit), 3 (ListScene: past the received list);
                         // BattlePlayer may set 2 back to 0 when none of the first mUnk_99 is 0
    /* 0b */ u8 mUnk_0b;
};

class BattleCpu; // created by BattlePlayer's ctor for an index at or past the limit (1, or GetRecvBlockCount() in wireless mode); ctor 02230f58

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
    /* ed */ u8 mNameLength; // length in bytes of the name at +b0 (NameText)
    /* ee */ u8 mUnk_ee; // raised by BattlePlayerList, BattleCommand and BattleConfrontCard; func_ov007_02263150 tests
                         // it (plus the partner's) >= 3, BattleResult against data_020d433c.mUnk_99
    /* ef */ u8 mState; // Update's case 3 never runs: nothing writes 3
    /* f0 */ u8 mIndex;
    /* f1 */ u8 mCorner;
    /* f2 */ u8 mPartner; // equals mIndex while data_020d433c.mHasPartners is 0
    /* f3 */ u8 mUnk_f3;
    /* f4 */ u8 mHistoryCount;
    /* f5 */ u8 mRecord;
    /* f6 */ u8 mCharacterId;
    /* f7 */ u8 mUnk_f7; // 0..2, copied from data_020d433c.mUnk_a3; see there
    /* f8 */ u8 mSlots54Remaining; // mSlots54 entries (of the first data_020d433c.mUnk_99) with mFlag 0; also read raw at +0xf8 (BattleAbility)
    /* f9 */ u8 mSlots6cRemaining; // the same for mSlots6c
    /* fa */ u16 mAbilityId;
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

    // TRUE when the player has a BattleCpu (mCpu).
    bool IsCpu() const {
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
    /* 0e */ u8 mRecord; // the view BattleUnk022637acUnit keeps the name mUnk_0e
    /* 0f */ u8 mPlayer;
    /* 10 */ u8 mStatus; // 0 free, else AddUnit's flag (ReplaceUnitRecord sets 1); BattleAbility writes 3
    /* 11 */ u8 mMarker;
    /* 12 */ u8 mUnk_12;
    /* 13 */ PAD(0x13, 0x14);
};

#include "Battle/BattleUnk29c.hpp"

struct BattleMarker {
    /* 0 */ u8 mSlot;
    /* 1 */ u8 mPlayer;
    /* 2 */ u8 mStatus; // 0 free, 1 placed; BattleAbility writes 3, BattleCommandMap sets 1 back
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
    /* 16c */ s8 mMarkerHandles[14][10]; // index + 1 into mMarkers, 0 = none; [cell x][cell y]
    /* 1f8 */ s8 mUnitHandles[14][10]; // index + 1 into mUnits, 0 = none; [cell x][cell y]
    /* 284 */ u8 mTurnOrder[4]; // read at (mTurnCount - 1) % mPlayerCount, positions 1 and 2 swapped while mHasPartners is set
    /* 288 */ u16 mTurnCount; // +1 per new turn player (not for an extra turn, mRepeatTurn); BattleCommand draws (mTurnCount - 1) / mPlayerCount + 1
    /* 28a */ u8 mTurnPlayer;
    /* 28b */ u8 mUnk_28b;
    /* 28c */ u8 mUnk_28c;
    /* 28d */ u8 mUnitA; // SetUnitAB's first unit, read as BattleData's entry 0 (set by BattleTarget, BattleAbility)
    /* 28e */ u8 mUnitB; // SetUnitAB's second unit, read as BattleData's entry 1; sometimes the same unit as mUnitA
    /* 28f */ u8 mAddMarkerEnabled;
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
    /* 2a9 */ u8 mUnk_2a9; // player index: data_020d433c.mUnk_ae in wireless mode, else 0 (CreateObjects); compared with mTurnPlayer and units' mPlayer
    /* 2aa */ u8 mUnk_2aa;
    /* 2ab */ u8 mUnk_2ab;
    /* 2ac */ u8 mUnk_2ac;
    /* 2ad */ u8 mRepeatTurn;
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

    u8 Unk2a9() {
        return mUnk_2a9;
    }

    u8 MarkerCount() {
        return mMarkerCount;
    }
    BattleUnit *Units() {
        return mUnits;
    }

    BattleMarker *Markers() {
        return mMarkers;
    }

    s8 CellAt(u8 x, u8 y) {
        return mUnitHandles[x][y];
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

// Collision manager (0x214 bytes, vtable 0228abfc, ctor 02271bb4): a list of up to 0x7f sets (slot 0 is the list head)
// handed out through a ring of slot pointers; Update tests the sets against each other. Besides the gimmicks' sets,
// BattleTarget adds the units', the field's (mMapCollision) and its mBakugan's (mCollision).
struct GimmickCollision : public SceneBase {
    /* 00 SceneBase */
    /* 0c */ GimmickSetData *mSets; // 0x80 slots of 0x838 bytes; slot 0 is the list head, not a loaded set
    /* 10 */ GimmickSetData *mSetRing[0x80]; // slot ring
    /* 210 */ u8 mRingWriteIndex; // ring position where a released slot is put back
    /* 211 */ u8 mRingReadIndex; // ring position of the next slot to hand out
    /* 212 */ u8 mEnabled; // Update tests the sets only while set (BattleTarget sets it, its destructor clears it); the hit counts are cleared either way
    /* 213 */ PAD(0x213, 0x214);

    GimmickCollision(); // 02271bb4
    virtual ~GimmickCollision();
    /* 08 */ virtual void Update(); // 02271db0

    // Records each overlap in both sets (mHitSlots, mHitObstacleIndices = j for both, mHitCount, at most 8). j is not
    // reset per obstacle of a, so after a's first obstacle each further one is tested once, against b's entry
    // j = count, count + 1, ... (past b's list).
    void TestSets(GimmickSetData *a, GimmickSetData *b);
    BOOL TestObstacles(BattleObstacle *a, BattleObstacle *b); // 0 for type 0 against a non-zero type and for a first type >= 2
};

struct UnkStruct_020d433c {
    /* 00 */ BattlePlayerList *mBattlePlayers;
    /* 04 */ EffectManager *mEffects;
    /* 08 */ GimmickManager *mGimmicks;
    /* 0c */ GimmickCollision *mGimmickCollision;
    /* 10 */ AntennaIndicator *mAntenna;
    union {
        /* 14 */ u16 mFlags;
        struct {
            /* 14 */ u16 mUnk_14_0 : 1;
            /* 14 */ u16 mSaveDirty : 1;
            /* 14 */ u16 mUnk_14_2 : 14;
        };
    };
    /* 16 */ u16 mNames[4][12]; // UTF-16 player names
    /* 76 */ u8 mUnk_76;
    /* 77 */ u8 mSaveSlotContent;
    /* 78 */ PAD(0x78, 0x79);
    /* 79 */ u8 mLanguage;
    /* 7a */ u8 mUnk_7a;
    /* 7b */ u8 mNextScene; // SCENE_* id BattlePlayerList requests when the battle ends, clearing it first (0
                            // requests SCENE_TITLE; SCENE_TOURNAMENT becomes SCENE_GAME_OVER, not cleared, with
                            // mUnk_7a and mUnk_7d == 3); SCENE_ARENA_WINDOW takes the wireless-mode path instead;
                            // BattleResult, BattleUnk0227532c and func_ov007_022633f8 also test it
    /* 7c */ u8 mUnk_7c;
    /* 7d */ u8 mUnk_7d;
    /* 7e */ u8 mMapTab : 4;
    /* 7e */ u8 mTitleMenuSelection : 4;
    /* 7f */ u8 mUnk_7f; // SCENE_* id MyroomScene requests when it ends (TitleScene stores SCENE_TITLE, MapScene
                         // SCENE_MAP); with SCENE_TITLE and mUnk_14 & 2 it opens a SaveWindow first
    /* 80 */ u8 mUnk_80;
    /* 81 */ u8 mUnk_81;
    /* 82 */ u8 mUnk_82;
    /* 83 */ u8 mTutorialNumber;
    /* 84 */ u8 mTutorialPart;
    /* 85 */ u8 mTutorialId;
    /* 86 */ u8 mUnk_86;
    /* 87 */ u8 mUnk_87;
    /* 88 */ u8 mUnk_88_0 : 4;
    /* 88 */ u8 mUnk_88_4 : 4;
    /* 89 */ u8 mUnk_89;
    /* 8a */ u8 mLastBattle : 4;
    /* 8a */ u8 mUnk_8a_4 : 4;
    /* 8b */ u8 mParkEntryIndex : 4;
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
        /* 9a */ u8 mRules;
        struct {
            /* 9a */ u8 mMicEnabled : 1;
            /* 9a */ u8 mUnk_9a_1 : 1;
            /* 9a */ u8 mUnk_9a_2 : 1;
            /* 9a */ u8 mUnk_9a_3 : 1;
            /* 9a */ u8 mUnk_9a_4 : 1;
            /* 9a */ u8 mUnk_9a_5 : 2;
            /* 9a */ u8 mUnk_9a_7 : 1;
        };
    };
    /* 9b */ u8 mPartners[4]; // also data_020d43d7; read only while mHasPartners is set
    /* 9f */ u8 mCharacterIds[4]; // also data_020d43db
    /* a3 */ u8 mUnk_a3[4]; // also data_020d43df: per player 0..2, copied to BattlePlayer::mUnk_f7; second index of the
                            // player templates, a BattleShoot multiplier index, BattleCpu tests, a cell frame drawn by
                            // FieldSelectScene / BattleResult
    /* a7 */ u8 mUnk_a7;
    /* a8 */ u8 mWireless; // set by ArenaScene when it starts the wireless session; cleared by ArenaScene, ArenaWindowScene, BattlePlayerList
    /* a9 */ u8 mUnk_a9[4];
    /* ad */ u8 mCreditsPending;
    /* ae */ u8 mUnk_ae;
    /* af */ u8 mUnk_af[4];
    /* af */
};

extern UnkStruct_020d433c data_020d433c;
