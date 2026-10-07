#pragma once

#define ARRAY_COUNT(arr) (s32)(sizeof(arr) / sizeof((arr)[0]))

// Byte range filler for struct fields that haven't been identified: PAD(0x10, 0x18).
#define PAD(start, end) u8 pad_##start[(end) - (start)]

// mwcc predates C++11; `override` only documents intent here.
#if !defined(__clang__) && !defined(__INTELLISENSE__)
    #define override
#endif
