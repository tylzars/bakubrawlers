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
    u32 func_0202f238();
    void func_0202f024(void *unk1);
    void func_0202f078(void *unk1);
    void func_0202ed3c();
    void func_0202ee10(u32 unk1, void *unk2, u32 unk3, u32 unk4);
    void func_0202eefc(u32 unk1, void *unk2, u32 unk3);
    u32 func_0202ef80();
    void func_0202eeb0();
    void func_0202f184();
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
    /* 01 */ u8 mUnk_01;
    /* 02 */ PAD(0x02, 0x04);
    union {
        /* 04 */ u32 mUnk_04;
        struct {
            /* 04 */ u16 mKeys;
            /* 06 */ u8 mUnk_06;
            /* 07 */ u8 mUnk_07;
            /* 08 */ u8 mUnk_08;
            /* 09 */ u8 mUnk_09;
            /* 0a */ PAD(0x0a, 0x0b);
            /* 0b */ u8 mUnk_0b;
            /* 0c */ u8 mUnk_0c;
            /* 0d */ u8 mUnk_0d;
            /* 0e */ u8 mUnk_0e;
            /* 0f */ u8 mUnk_0f;
            /* 10 */ PAD(0x10, 0x1f);
            /* 1f */ u8 mUnk_1f;
        };
    };
};

extern UnkStruct_0220f540 data_0220f540;
