#pragma once

#include "global.h"
#include "types.h"

#include <nitro/fx.h>

#include "Graphics/ModelTransform.hpp"
#include "Graphics/UnkStruct_0203d374.hpp"

class BattleMapModel : public ModelTransform { // 0x30 bytes, ctor 02248c84 (1 argument), dtor 02248cb0
public:
    /* 2c */ u8 mUnk_2c; // model type (0..7)
    /* 2d */ u8 mUnk_2d; // brightness of the model (0xf..0x1f)
    /* 2e */ PAD(0x2e, 0x30);

    BattleMapModel(u32 unk1);
    ~BattleMapModel();
    void func_ov007_02248cf4(); // loads the model of type mUnk_2c from /Game/Battle/3d/MapObj
    void func_ov007_02248eb8(); // updates the brightness and draws
};

// Piece of the battle map (ctor 02263xxx, destructor 022636b0), deleted by pointer.
class BattleMapPiece : public ModelTransform {
public:
    BattleMapPiece(int unk); // 0226368c
    ~BattleMapPiece();
};

// Model of the player name plates, status icons and character markers (/Game/Battle/3d/FontObj), 0x2c bytes.
// Ctor 02248a5c loads the model of the given type (0..14), dtor 02248a80.
class BattleFontObj : public ModelTransform {
public:
    BattleFontObj(u32 type);
    ~BattleFontObj();
    void func_ov007_02248ac4(u32 type); // loads the texture and the model of the type
};

class UnkStruct_02028308;

// One model of a BattleMapObj (0x2c bytes, no own members): dtor 02249068, loader 022490ac.
class BattleMapObjPart : public ModelTransform {
public:
    ~BattleMapObjPart();
    void func_ov007_022490ac(UnkStruct_02028308 *texture, const char *nsbmd, const char *data, u32 unk0);
};

// A model file entry of the tables of BattleMapObj (8 bytes).
struct BattleMapObjEntry {
    /* 00 */ const char *mName;
    /* 04 */ u8 mFlag;
    /* 05 */ u8 mHasData;
    /* 06 */ PAD(0x06, 0x08);
    /* 08 */
};

struct BattleMapObjTable {
    /* 00 */ u32 mCount;
    /* 04 */ const BattleMapObjEntry *mEntries;
    /* 08 */
};
