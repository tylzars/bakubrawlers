#pragma once

#include "global.h"
#include "types.h"

// Text-record lookup object (0x98 bytes).
struct UnkStruct_020758f4_Record {
    /* 0 */ PAD(0x0, 0x2);
    /* 2 */ u16 mTextOffset;
    /* 4 */ u32 mTextLength;
};

// 8-byte record returned by func_02075a00.
struct UnkStruct_020758f4_Entry {
    /* 0 */ u8 mUnk_0;
    /* 1 */ u8 mUnk_1;
    /* 2 */ u16 mUnk_2;
    /* 4 */ u32 mUnk_4;
    /* 8 */
};

class UnkStruct_020758f4 {
public:
    /* 00 (vtable) */
    /* 04 */ UnkStruct_020758f4_Entry mUnk_04[13];
    /* 6c */ UnkStruct_020758f4_Entry mUnk_6c[4];
    /* 8c */ u8 mUnk_8c;
    /* 8d */ u8 mUnk_8d;
    /* 8e */ u16 mUnk_8e;
    /* 90 */ u32 mUnk_90;
    /* 94 */ u8 mUnk_94;
    /* 95 */ PAD(0x95, 0x98);
    /* 98 */

    UnkStruct_020758f4();
    virtual ~UnkStruct_020758f4();
    UnkStruct_020758f4_Record *func_02075998(u32 index);
    UnkStruct_020758f4_Entry *func_02075a00(u32 index);
    void func_02075a0c();
    u32 func_02075e40(u8 index);
    u16 func_02075ff4(u32 score);
};

