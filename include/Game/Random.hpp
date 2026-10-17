#pragma once

#include <nitro/math.h>

// The game's shared random number state.
extern MATHRandContext32 gRandom;

// Picks an index from a table of byte weights; -1 when all weights are 0.
s8 PickWeighted(u8 count, const u8 *weights);
