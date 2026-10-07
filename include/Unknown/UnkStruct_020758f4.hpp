#pragma once

#include "global.h"
#include "types.h"

// Text-record lookup object (0x98 bytes).
struct UnkStruct_020758f4_Record {
    /* 0 */ PAD(0x0, 0x2);
    /* 2 */ u16 mTextOffset;
    /* 4 */ u32 mTextLength;
};

class UnkStruct_020758f4 {
public:
    /* 00 (vtable) */
    /* 04 */ PAD(0x04, 0x98);
    /* 98 */

    UnkStruct_020758f4();
    virtual ~UnkStruct_020758f4();
    UnkStruct_020758f4_Record *func_02075998(u32 index);
};

void func_02075a0c();
