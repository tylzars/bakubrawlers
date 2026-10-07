#pragma once

#include "global.h"
#include "types.h"

// Singleton (instance 0x020c3030, vtable 0x020a5738), constructed on first use.
class UnkStruct_020c3030 {
public:
    virtual ~UnkStruct_020c3030();

    static UnkStruct_020c3030 &Get() {
        static UnkStruct_020c3030 instance;
        return instance;
    }

    void func_0202edd0();
    u8 *func_0202f0cc(u32 unk1);
    u32 func_0202f134();
};

struct UnkStruct_020c4030 {
    /* 000 */ PAD(0x000, 0x228);
    /* 228 */ u8 mUnk_228;
};

extern UnkStruct_020c4030 data_020c4030;
extern u8 data_020c4258; // the same byte as data_020c4030.mUnk_228

// Communication mode 6 and 7 are the wireless modes.
static inline BOOL IsWirelessMode(const u8 *mode) {
    switch (*mode) {
        case 6:
        case 7:
            return TRUE;
    }
    return FALSE;
}

struct UnkStruct_0220f540 {
    /* 00 */ u8 mUnk_00;
    /* 01 */ PAD(0x01, 0x04);
    /* 04 */ u32 mUnk_04;
};

extern UnkStruct_0220f540 data_0220f540;
