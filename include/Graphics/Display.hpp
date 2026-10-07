#pragma once

#include "types.h"

void func_02001e68(u32 unk1, u32 unk2, u32 unk3);
void func_02001ed0(u32 unk1);

void func_020037e8(s32 offset); // 0x020037e8

void func_02001eec(vu16 *reg, int value); // master brightness of the engine whose DISPCNT block starts at reg - 0x6c
void func_02003ba4(void *data, u32 offset, u32 size);
void func_02003ea4(void *data, u32 offset, u32 size);
void func_02004394();
void func_020043ac(void *data, u32 offset, u32 size);
void func_02004410();
