#pragma once

#include "global.h"
#include "types.h"

// Singleton (instance 0x020c17a8), constructed on first use and destroyed at exit.
class UnkStruct_020c17a8 {
public:
    /* 00 */ PAD(0x00, 0x30); // size unknown

    ~UnkStruct_020c17a8();

    static UnkStruct_020c17a8 &Get() {
        static UnkStruct_020c17a8 instance;
        return instance;
    }

    u32 func_02023c38(u32 unk1);
};
