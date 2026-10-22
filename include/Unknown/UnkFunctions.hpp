#pragma once

#include "types.h"

// Free functions in main that have no header of their own yet.

struct SaveData;
// The slot's entry index % 6. No empty check, unlike GetSlotBakuganId: an empty slot (0xffff) gives 3.
s32 GetSlotBakuganAttribute(SaveData *data, u32 unk2, u32 unk3);
extern "C" void DC_FlushRange(void *buffer, u32 size); // probably DC_FlushRange
void SeedRandom(u32 unk1);
u8 GetCardInfo(u32 id, u32 kind);
BOOL UnlockProgressEntry(u32 unk1);
BOOL UnlockStoreItems();
// For each UnlockPair of the save position's list sets the id's bit in the level arrays up to its mLevel
// (gUnlockedCharacterLevel0Bits, 496d, 4970); TRUE when an id had no bit in any of them before.
BOOL UnlockCharacters();
// Sets gUnlockedFieldBits bits (bit = field id): 0x1 and 0x80 always, the others by gSaveData[0].mUnk_42 / mUnk_43;
// TRUE when one of them was clear.
BOOL UnlockFields();
void RefreshParkSlotProgressEntryFlag();
// Park entry types: the values of ParkEntry::mType in the 12-byte park table records GetParkEntry copies out (read
// through the ParkEntry copies in ParkScene.cpp and ParkCharacters.cpp). ParkScene's Update state 24 opens
// /font/mes_pm_<stem>.mes for each (mes_pm_battle, mes_pm_Teambattle, mes_pm_BattleRoyale, mes_pm_mes_only_1..3,
// mes_pm_Tutorial). The three battle types set up the battle in state 33: BATTLE two players, TEAMBATTLE four with
// partners, BATTLE_ROYALE four without; MES_ONLY_1..3 show their message and return to the menu; TUTORIAL opens the
// mes_park entry 4 popup (state 36), whose first button leads to SCENE_TUTORIAL. FillParkSlots' random fill takes
// only battle types and draws each slot's type from BATTLE (twice), TEAMBATTLE and BATTLE_ROYALE; GetParkEntry skips
// its unlock search for MES_ONLY_1..3 (unless gSaveData mCreditsDone), and func_02089a9c treats every non-battle type as
// BATTLE when that flag is set.
#define PARK_ENTRY_TYPE_BATTLE 0
#define PARK_ENTRY_TYPE_TEAMBATTLE 1
#define PARK_ENTRY_TYPE_BATTLE_ROYALE 2
#define PARK_ENTRY_TYPE_MES_ONLY_1 3
#define PARK_ENTRY_TYPE_MES_ONLY_2 4
#define PARK_ENTRY_TYPE_MES_ONLY_3 5
#define PARK_ENTRY_TYPE_TUTORIAL 6

void GetParkEntry(u32 index, void *out);
u8 GetParkGroupId(u32 index);
void func_02089a9c(u32 index);
