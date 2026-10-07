#pragma once

#include "global.h"
#include "types.h"

#include <nitro/fx.h>

#include "Graphics/ModelTransform.hpp"
#include "Graphics/UnkStruct_0203d374.hpp"

class BattleMapModel : public ModelTransform { // 0x30 bytes, ctor 02248c84 (1 argument), dtor 02248cb0
public:
    /* 2c */ u8 mUnk_2c;
    /* 2d */ PAD(0x2d, 0x30);

    BattleMapModel(u32 unk1);
    ~BattleMapModel();
};


// Piece of the battle map (ctor 02263xxx, destructor 022636b0), deleted by pointer.
class BattleMapPiece {
public:
    u8 mPad[4];
    ~BattleMapPiece();
};
