#pragma once

#include "types.h"

extern "C" void GX_SetGraphicsMode(u32 unk1, u32 unk2, u32 unk3);
extern "C" void GXS_SetGraphicsMode(u32 unk1);

extern "C" void WriteBG0Offset(s32 offset); // 0x020037e8

extern "C" void G2x_SetMasterBrightness_(vu16 *reg, int value); // master brightness of the engine whose DISPCNT block starts at reg - 0x6c
extern "C" void GXS_LoadBG0Scr(void *data, u32 offset, u32 size);
extern "C" void GXS_LoadBG0Char(void *data, u32 offset, u32 size);
extern "C" void GX_BeginLoadSubBGExtPltt();
extern "C" void GX_LoadSubBGExtPltt(void *data, u32 offset, u32 size);
extern "C" void GX_EndLoadSubBGExtPltt();
