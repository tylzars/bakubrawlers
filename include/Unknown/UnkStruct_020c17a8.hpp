#pragma once

#include "global.h"
#include "types.h"

// Singleton (instance 0x020c17a8), constructed on first use and destroyed at exit.
class UnkStruct_020c17a8 {
public:
    /* 00 */ u8 mBusy; // set while a save operation is running
    /* 01 */ PAD(0x01, 0x30); // size unknown

    ~UnkStruct_020c17a8();

    static UnkStruct_020c17a8 &Get() {
        static UnkStruct_020c17a8 instance;
        return instance;
    }

    void func_02023b80();
    u16 func_02023bc4(const void *data, u32 size);
    u32 func_02023c38(u32 unk1);
    u32 func_02023d30(u32 unk1);
};
