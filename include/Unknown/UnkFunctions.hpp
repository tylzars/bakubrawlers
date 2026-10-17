#pragma once

#include "types.h"

// Free functions in main that have no header of their own yet.

struct SaveData;
s32 func_020230a8(SaveData *data, u32 unk2, u32 unk3);
extern "C" void DC_FlushRange(void *buffer, u32 size); // probably DC_FlushRange
void SeedRandom(u32 unk1);
u8 GetCardInfo(u32 id, u32 kind);
BOOL UnlockProgressEntry(u32 unk1);
BOOL UnlockStoreItems();
BOOL UnlockCharacters();
BOOL func_02086b74();
void func_02089208();
void GetParkEntry(u32 index, void *out);
u8 GetParkGroupId(u32 index);
void func_02089a9c(u32 index);
