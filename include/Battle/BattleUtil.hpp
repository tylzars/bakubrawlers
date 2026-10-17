#pragma once

#include "global.h"
#include "types.h"

// Battle-overlay functions used across several battle files, from files not decompiled yet.

// Touch-rectangle test; sets *flag while the stylus stays inside.
u8 func_ov007_02229ee4(s32 x, s32 y, s32 width, s32 height, u16 unk5, u8 *flag);
// Distance between two points.
s32 PointDistance(s32 x1, s32 y1, s32 x2, s32 y2);
s32 TouchTurnAngle(s32 unk1, s32 unk2, s32 unk3);
