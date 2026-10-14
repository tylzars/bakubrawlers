#pragma once

#include "global.h"
#include "types.h"

// Game object (instance 0x020c1884, destructor 0x020219c0): start-up (func_02020328) and the main loop
// (func_02020a48); mFrame is the running frame count.
class UnkStruct_020c1884 {
public:
    /* 00 */ s32 mUnk_00;
    /* 04 */ s32 mUnk_04;
    /* 08 */ u8 mUnk_08;
    /* 09 */ u8 mUnk_09;
    /* 0a */ u8 mUnk_0a;
    /* 0b */ u8 mUnk_0b;
    /* 0c */ u8 mUnk_0c;
    /* 0d */ u8 mUnk_0d;
    /* 0e */ PAD(0x0e, 0x10);
    /* 10 */ u32 mFrame;
    /* 14 */ u8 mUnk_14;
    /* 15 */ u8 mUnk_15;
    /* 16 */ PAD(0x16, 0x18);
    /* 18 */

    ~UnkStruct_020c1884(); // out of line in Game.cpp (0x020219c0)

    static UnkStruct_020c1884 &Get() {
        static UnkStruct_020c1884 instance;
        return instance;
    }
    const char *func_0202030c(); // language suffix of file names
    void func_02020328();
    void func_02020a48();
    void func_02021700();
};
