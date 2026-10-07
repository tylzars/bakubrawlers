#pragma once

#include "global.h"
#include "types.h"

#include <nitro/fx.h>

#include "Graphics/ModelTransform.hpp"

class BattleMapModel : public ModelTransform { // 0x30 bytes, ctor 02248c84 (1 argument), dtor 02248cb0
public:
    /* 2c */ u8 mUnk_2c;
    /* 2d */ PAD(0x2d, 0x30);

    BattleMapModel(u32 unk1);
    ~BattleMapModel();
};

class UnkStruct_0203d374 { // 0x54 bytes, ctor 0203d374, destructor 0203d3e8
public:
    /* 00 */ PAD(0x00, 0x04);
    /* 04 */ VecFx32 mPosition;
    /* 10 */ u16 mUnk_10;
    /* 12 */ u16 mUnk_12;
    /* 14 */ u16 mUnk_14;
    /* 16 */ PAD(0x16, 0x49);
    /* 49 */ u8 mUnk_49;
    /* 4a */ PAD(0x4a, 0x54);
    /* 54 */

    UnkStruct_0203d374();
    ~UnkStruct_0203d374();
    void func_0203d4dc(u32 a, u32 b, u32 c);
    void func_0203d988();
    void func_0203dab0();
};


// Piece of the battle map (ctor 02263xxx, destructor 022636b0), deleted by pointer.
class BattleMapPiece {
public:
    u8 mPad[4];
    ~BattleMapPiece();
};
