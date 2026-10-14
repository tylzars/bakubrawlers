#pragma once

#include "global.h"
#include "types.h"

struct UnkStruct_02023248 {
    /* 0 */ u16 mUnk_00_0 : 4;
    /* 0 */ u16 mPad : 12;
    /* 2 */ u16 mUnk_0 : 3;
    /* 2 */ u16 mUnk_3 : 3;
    /* 2 */ u16 mUnk_6 : 3;
    /* 2 */ u16 mUnk_9 : 3;
    /* 2 */ u16 mUnk_12 : 3;
    /* 2 */ u16 mUnk_15 : 1;
    /* 4 */
};

struct ParkSlot {
    /* 0 */ u8 mCategory;
    /* 1 */ u8 mGroup;
};

struct UnkStruct_020d43f0 {
    /* 000 */ u16 mUnk_00[3];
    /* 006 */ u16 mUnk_06[3];
    /* 00c */ u16 mUnk_0c[3];
    /* 012 */ PAD(0x012, 0x014);
    /* 014 */ u16 mUnk_14_0 : 1;
    /* 014 */ u16 mUnk_14_1 : 3;
    /* 014 */ u16 mUnk_14_4 : 3;
    /* 014 */ u16 mUnk_14_7 : 3;
    /* 014 */ u16 mUnk_14_10 : 3;
    /* 014 */ u16 mUnk_14_13 : 3;
    /* 016 */ u16 mUnk_16_0 : 12;
    /* 016 */ u16 mUnk_16_12 : 3;
    /* 016 */ u16 mUnk_16_15 : 1;
    /* 018 */ u16 mUnk_18_0 : 3;
    /* 018 */ u16 mUnk_18_3 : 5;
    /* 018 */ u16 mUnk_18_8 : 3;
    /* 018 */ u16 mUnk_18_11 : 3;
    /* 018 */ u16 mUnk_18_14 : 2;
    /* 01a */ u16 mUnk_1a_0 : 3;
    /* 01a */ u16 mUnk_1a_3 : 5;
    /* 01a */ u16 mUnk_1a_8 : 8;
    /* 01c */ u32 mMoney;
    /* 020 */ u32 mUnk_20;
    /* 024 */ u8 mUnk_24;
    /* 025 */ u8 mUnk_25;
    /* 026 */ u8 mUnk_26;
    union {
        /* 027 */ u8 mUnk_27;
        struct {
            /* 027 */ u8 mUnk_27_0 : 1;
            /* 027 */ u8 mUnk_27_1 : 1;
            /* 027 */ u8 mUnk_27_2 : 3;
            /* 027 */ u8 mUnk_27_5 : 2;
            /* 027 */ u8 mUnk_27_7 : 1;
        };
    };
    /* 028 */ u16 mUnk_28_0 : 5;
    /* 028 */ u16 mUnk_28_5 : 1;
    /* 028 */ u16 mUnk_28_6 : 1;
    /* 028 */ u16 mUnk_28_7 : 3;
    /* 028 */ u16 mUnk_28_10 : 3;
    /* 028 */ u16 mUnk_28_13 : 3;
    /* 02a */ u16 mName[12]; // player name, UTF-16
    /* 042 */ volatile u8 mUnk_42; // reloaded after a switch on it (ov002 0221e984)
    /* 043 */ u8 mUnk_43;
    /* 044 */ u32 mPlayFrames; // play time in frames (60 per second)
    /* 048 */ u16 mUnk_48;
    /* 04a */ u16 mUnk_4a;
    /* 04c */ PAD(0x04c, 0x04e);
    /* 04e */ u16 mUnk_4e;
    /* 050 */ u16 mUnk_50;
    /* 052 */ u16 mUnk_52;
    /* 054 */ PAD(0x054, 0x05a);
    /* 05a */ u8 mUnk_5a;
    /* 05b */ u8 mUnk_5b;
    /* 05c */ u16 mUnk_5c;
    /* 05e */ u16 mUnk_5e[17];
    /* 080 */ u16 mUnk_80;
    /* 082 */ u16 mUnk_82;
    /* 084 */ u16 mUnk_84;
    /* 086 */ PAD(0x086, 0x088);
    /* 088 */ u16 mUnk_88[0xf6][2];
    /* 460 */ u8 mUnk_460[0xf6];
    /* 556 */ PAD(0x556, 0x578);
    /* 578 */ volatile u8 mUnk_578;
    /* 579 */ PAD(0x579, 0x580);
    /* 580 */ u8 mUnk_580;
    /* 581 */ PAD(0x581, 0x586);
    // Flag arrays read and written as `(u8 *) data_020d43f0[0].mUnk_586` (func_02086938, func_020876f0): the cast gives
    // them their own pool word, hoisted after the loop's other tables, while still aliasing data_020d43f0 so the
    // following mUnk_5b9 load stays after the store. Some callers address the same bytes through the
    // data_020d4976/data_020d497b externs and match that way.
    /* 586 */ u8 mUnk_586[5];
    /* 58b */ u8 mUnk_58b[0x21];
    /* 5ac */ u8 mUnk_5ac;
    /* 5ad */ u8 mUnk_5ad;
    /* 5ae */ PAD(0x5ae, 0x5b6);
    /* 5b6 */ u8 mUnk_5b6;
    /* 5b7 */ u8 mUnk_5b7;
    /* 5b8 */ u8 mUnk_5b8;
    /* 5b9 */ u8 mUnk_5b9_0 : 1;
    /* 5b9 */ u8 mUnk_5b9_1 : 1;
    /* 5b9 */ u8 mUnk_5b9_2 : 6;
    /* 5ba */ PAD(0x5ba, 0x5be);
    /* 5be */ u8 mUnk_5be;
    union {
        /* 5bf */ u8 mUnk_5bf;
        struct {
            /* 5bf */ u8 mUnk_5bf_0 : 1;
            /* 5bf */ u8 mUnk_5bf_1 : 7;
        };
    };
    /* 5c0 */ PAD(0x5c0, 0x5c1);
    /* 5c1 */ u8 mUnk_5c1;
    /* 5c2 */ ParkSlot mUnk_5c2[8];
    /* 5d2 */ u8 mUnk_5d2;
    /* 5d3 */ PAD(0x5d3, 0xff4);
};

// Declared as an array: code that stores through it only matches when indexed (`data_020d43f0[0]`).
extern UnkStruct_020d43f0 data_020d43f0[1];

// Bit-flag arrays inside data_020d43f0 (at +0x578, +0x57d, +0x580) that code addresses directly.
extern u8 data_020d4968[];
extern u8 data_020d496d[];
extern u8 data_020d4970[];
extern u8 data_020d4969[];
extern u8 data_020d496a[];
extern u8 data_020d4973[];
extern u8 data_020d49ae[];
extern u8 data_020d4404[];

// Records reached through data_020d43f0 by the helpers below.
struct UnkStruct_020d4478 {
    /* 0 */ u16 mUnk_0 : 4;
    /* 0 */ u16 mUnk_4 : 12;
    /* 2 */ u16 mPad;
    /* 4 */
};


extern UnkStruct_020d4478 data_020d4478[];
extern u8 data_020d4850[];
extern u8 data_020d49b0[];

int func_0202317c(UnkStruct_020d43f0 *base, u32 unk1, u32 unk2, u32 unk3);
u16 func_020231f4(UnkStruct_020d43f0 *base, u32 unk1, u8 unk2, u8 unk3);
void func_02023168(UnkStruct_020d43f0 *base, u32 unk1, u32 unk2, int unk3);
void func_020231c0(UnkStruct_020d43f0 *base, int unk1, u32 unk2);
UnkStruct_02023248 *func_02023248(UnkStruct_020d43f0 *base, u32 unk1, u32 unk2);

static inline BOOL TestFlagBit(const u8 *flags, int index) {
    return flags[(u32) index >> 3] & (1 << (index % 8));
}

static inline void SetFlagBit(u8 *flags, int index) {
    flags[(u32) index >> 3] |= 1 << (index % 8);
}
