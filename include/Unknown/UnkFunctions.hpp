#pragma once

#include "types.h"

// Free functions in main that have no header of their own yet.

struct SaveData;
s32 GetSlotBakuganAttribute(SaveData *data, u32 unk2, u32 unk3);
extern "C" void DC_FlushRange(void *buffer, u32 size); // probably DC_FlushRange
void SeedRandom(u32 unk1);
u8 GetCardInfo(u32 id, u32 kind);
BOOL UnlockProgressEntry(u32 unk1);
BOOL UnlockStoreItems();
// For each UnlockPair of the save position's list sets the id's bit in the level arrays up to its mLevel
// (data_020d496a, 496d, 4970); TRUE when an id had no bit in any of them before.
BOOL UnlockCharacters();
BOOL UnlockFields();
void func_02089208();
void GetParkEntry(u32 index, void *out);
u8 GetParkGroupId(u32 index);
void func_02089a9c(u32 index);
