#pragma once

#include <nitro/math.h>

// The game's shared random number state.
extern MATHRandContext32 data_020d42e8;

// Picks an index from a table of byte weights.
s8 func_02021a30(u8 count, const u8 *weights);
