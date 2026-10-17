#pragma once

#include "global.h"
#include "types.h"

struct UnkStruct_02023248 {
    /* 0 */ u16 mUnk_00_0 : 4;
    /* 0 */ u16 mGauge : 12; // BattlePlayer copies the entry into BattleRecord::mHead, whose mGauge this is
    /* 2 */ u16 mUnk_0 : 3;
    /* 2 */ u16 mUnk_3 : 3;
    /* 2 */ u16 mUnk_6 : 3;
    /* 2 */ u16 mUnk_9 : 3;
    /* 2 */ u16 mUnk_12 : 3;
    /* 2 */ u16 mUnk_15 : 1;
    /* 4 */
};

struct ParkSlot {
    /* 0 */ u8 mCategory;
    /* 1 */ u8 mGroup;
};

struct SaveData {
    /* 000 */ u16 mSlotEntryIndices[3];
    /* 006 */ u16 mSlots54Values[3]; // 54 / 6c: the BattlePlayer offsets of mSlots54 / mSlots6c it is copied to
    /* 00c */ u16 mSlots6cValues[3];
    /* 012 */ PAD(0x012, 0x014);
    /* 014 */ u16 mUnk_14_0 : 1;
    /* 014 */ u16 mUnk_14_1 : 3;
    /* 014 */ u16 mUnk_14_4 : 3;
    /* 014 */ u16 mUnk_14_7 : 3;
    /* 014 */ u16 mUnk_14_10 : 3;
    /* 014 */ u16 mUnk_14_13 : 3;
    /* 016 */ u16 mUnk_16_0 : 12;
    /* 016 */ u16 mUnk_16_12 : 3;
    /* 016 */ u16 mUnk_16_15 : 1;
    /* 018 */ u16 mUnk_18_0 : 3;
    /* 018 */ u16 mUnk_18_3 : 5;
    /* 018 */ u16 mUnk_18_8 : 3;
    /* 018 */ u16 mUnk_18_11 : 3;
    /* 018 */ u16 mUnk_18_14 : 2;
    /* 01a */ u16 mUnk_1a_0 : 3;
    /* 01a */ u16 mUnk_1a_3 : 5;
    /* 01a */ u16 mAttribute : 8; // AttrSelScene's choice (sAttribToSaved[mAttrib]); 0..5
    /* 01c */ u32 mMoney;
    /* 020 */ u32 mUnk_20;
    /* 024 */ u8 mUnk_24;
    /* 025 */ u8 mUnk_25;
    /* 026 */ u8 mLanguage; // written by SaveWindow; nothing in this build reads it back
    union {
        /* 027 */ u8 mRules; // copied to and from data_020d433c.mRules; mUnk_27_0 mirrors mMicEnabled
        struct {
            /* 027 */ u8 mUnk_27_0 : 1;
            /* 027 */ u8 mUnk_27_1 : 1;
            /* 027 */ u8 mUnk_27_2 : 3;
            /* 027 */ u8 mUnk_27_5 : 2;
            /* 027 */ u8 mUnk_27_7 : 1;
        };
    };
    /* 028 */ u16 mUnk_28_0 : 5;
    /* 028 */ u16 mUnk_28_5 : 1;
    /* 028 */ u16 mMicEnabled : 1;
    /* 028 */ u16 mVolume0Index : 3; // also selects gSeqVolumes -> SoundManager::mSeqVolume
    /* 028 */ u16 mVolume1Index : 3; // also selects gSeqArcVolumes -> SoundManager::mSeqArcVolume
    /* 028 */ u16 mVolume2Index : 3;
    /* 02a */ u16 mName[12]; // player name, UTF-16
    /* 042 */ u8 mUnk_42; // indexes data_020b4324 (ScriptPlayer::func_02066814) and the unlock tables; 10 = past the last
    /* 043 */ u8 mUnk_43; // entry position inside data_020b4324[mUnk_42], reset with each mUnk_42 step
    /* 044 */ u32 mPlayFrames; // play time in frames (60 per second)
    // Counters BattleResult raises (capped): its mUnk_b3 is set when BattlePlayer::mUnk_ee of player
    // BattlePlayerList::mUnk_2a9 (plus the partner's) reaches data_020d433c.mUnk_99.
    /* 048 */ u16 mUnk_48; // +1 when mUnk_b3 is set
    /* 04a */ u16 mUnk_4a; // +1 when mUnk_b3 is clear
    /* 04c */ PAD(0x04c, 0x04e);
    /* 04e */ u16 mUnk_4e;
    /* 050 */ u16 mUnk_50;
    /* 052 */ u16 mUnk_52;
    /* 054 */ PAD(0x054, 0x05a);
    // Smallest / largest (mTurnCount - 1) / players + 1 (at most 99) of a battle with mUnk_b3 set; 0 = none yet.
    /* 05a */ u8 mFewestRounds;
    /* 05b */ u8 mMostRounds;
    /* 05c */ u16 mUnk_5c;
    /* 05e */ u16 mUnk_5e[17]; // [character id] +1 for each opponent that is not a partner, with mUnk_b3 set; also data_020d444e
    /* 080 */ u16 mUnk_80; // +1 with mUnk_b3 set, no partners, fewer than 4 players
    /* 082 */ u16 mUnk_82; // +1 with mUnk_b3 set and partners (data_020d433c.mHasPartners)
    /* 084 */ u16 mUnk_84; // +1 with mUnk_b3 set, no partners, 4 players
    /* 086 */ PAD(0x086, 0x088);
    /* 088 */ u16 mBakuganEntries[0xf6][2]; // also reached as data_020d4478 (gSaveData + 0x88)
    /* 460 */ u8 mUnk_460[0xf6];
    /* 556 */ u8 mUnk_556[0x22];
    /* 578 */ volatile u8 mUnk_578;
    /* 579 */ PAD(0x579, 0x580);
    /* 580 */ u8 mUnk_580;
    /* 581 */ PAD(0x581, 0x586);
    // Flag arrays read and written as `(u8 *) gSaveData[0].mUnk_586` (UnlockStoreItems, UnlockProgressEntry): the
    // cast gives them their own pool word, hoisted after the loop's other tables, while still aliasing gSaveData so
    // the following mUnk_5b9 load stays after the store. Some callers address the same bytes through the
    // data_020d4976/data_020d497b externs and match that way.
    /* 586 */ u8 mUnk_586[5];
    /* 58b */ u8 mUnk_58b[0x21];
    /* 5ac */ u8 mUnk_5ac;
    /* 5ad */ u8 mUnk_5ad;
    /* 5ae */ PAD(0x5ae, 0x5b6);
    /* 5b6 */ u8 mUnk_5b6;
    /* 5b7 */ u8 mUnk_5b7;
    /* 5b8 */ u8 mUnk_5b8;
    /* 5b9 */ u8 mUnk_5b9_0 : 1;
    /* 5b9 */ u8 mUnk_5b9_1 : 1;
    /* 5b9 */ u8 mUnk_5b9_2 : 6;
    /* 5ba */ PAD(0x5ba, 0x5be); // UnkStruct_020d48f0::mRank / mPrevRank (data_020d48f0 = gSaveData + 0x500)
    /* 5be */ u8 mUnk_5be;
    union {
        /* 5bf */ u8 mUnk_5bf;
        struct {
            /* 5bf */ u8 mUnk_5bf_0 : 1;
            /* 5bf */ u8 mUnk_5bf_1 : 7;
        };
    };
    /* 5c0 */ PAD(0x5c0, 0x5c1);
    /* 5c1 */ u8 mParkSlotCount;
    /* 5c2 */ ParkSlot mParkSlots[8];
    /* 5d2 */ u8 mSpecialNameCode0Used;
    /* 5d3 */ PAD(0x5d3, 0xff4);
};

// Declared as an array: code that stores through it only matches when indexed (`gSaveData[0]`).
extern SaveData gSaveData[1];

// Bit-flag arrays inside gSaveData (at +0x578, +0x57d, +0x580) that code addresses directly.
extern u8 gUnlockedFieldBits[]; // SaveData::mUnk_578 onward; bit = field id (UnlockFields, FieldSelectScene, ParkScene)
extern u8 data_020d496d[];
extern u8 data_020d4970[];
extern u8 data_020d4969[];
extern u8 data_020d496a[];
extern u8 data_020d4973[];
extern u8 data_020d49ae[];
extern u8 gCharacterLook[]; // gSaveData + 0x14, read as a CharacterLook (a different bit layout from the fields above)

// Records reached through gSaveData by the helpers below.
struct UnkStruct_020d4478 {
    /* 0 */ u16 mUnk_0 : 4;
    /* 0 */ u16 mUnk_4 : 12;
    /* 2 */ u16 mPad;
    /* 4 */
};


extern UnkStruct_020d4478 data_020d4478[];
extern u8 data_020d4850[];
extern u8 data_020d49b0[];

// unk3 + unk2 * 6 + unk1 * 0xf6 (id, attribute); 0, entry 0, for unk1 >= 1, unk2 >= 0x29 or unk3 >= 6
int GetBakuganEntryIndex(SaveData *base, u32 unk1, u32 unk2, u32 unk3);
u16 func_020231f4(SaveData *base, u32 unk1, u8 unk2, u8 unk3);
void SetSlotEntryIndex(SaveData *base, u32 unk1, u32 unk2, int unk3); // slot unk2 = unk3; 0xffff = empty
void func_020231c0(SaveData *base, int unk1, u32 unk2);
UnkStruct_02023248 *GetBakuganEntry(SaveData *base, u32 unk1, u32 unk2);

static inline BOOL TestFlagBit(const u8 *flags, int index) {
    return flags[(u32) index >> 3] & (1 << (index % 8));
}

static inline void SetFlagBit(u8 *flags, int index) {
    flags[(u32) index >> 3] |= 1 << (index % 8);
}
