#pragma once

#include "global.h"
#include "types.h"

#include <nitro/fx.h>

#include "Graphics/ModelTransform.hpp"
#include "Graphics/BakuganModel.hpp"

// BattleMapModel::mKind: the /Game/Battle/3d/MapObj model LoadModel loads.
#define MAP_MODEL_KIND_CARD_00_00       0 // "card_00_00"
#define MAP_MODEL_KIND_CARD_FUTI        1 // "card_futi"
#define MAP_MODEL_KIND_O00BTL_000       2 // "o00btl_000"
// "f00gat_000" as well; the only kind BattleCommandMap::FreeTimedF00gatModel deletes, which it does once mF00gatTimer
// (0x97 when this model is created) has counted down to 0.
#define MAP_MODEL_KIND_F00GAT_000_TIMED 3
#define MAP_MODEL_KIND_F00GAT_000       4 // "f00gat_000"; SetupF00gatModelsAndCamera creates this kind + player index
#define MAP_MODEL_KIND_F00GAT_001       5 // "f00gat_001"
#define MAP_MODEL_KIND_F00GAT_002       6 // "f00gat_002"
#define MAP_MODEL_KIND_F00GAT_003       7 // "f00gat_003"

class BattleMapModel : public ModelTransform { // 0x30 bytes, ctor 02248c84 (1 argument), dtor 02248cb0
public:
    /* 2c */ u8 mKind; // MAP_MODEL_KIND_*
    /* 2d */ u8 mColorLevel; // 5-bit material colour level (0xf..0x1f), packed 5:5:5 for both colour pairs
    /* 2e */ PAD(0x2e, 0x30);

    BattleMapModel(u32 unk1);
    ~BattleMapModel();
    void LoadModel(); // loads the model of kind mKind from /Game/Battle/3d/MapObj
    void DrawPulsing(); // steps mColorLevel down / up by GameSystem::mFrame bit 0x20, then draws
};

// Kinds of a BattleMapPiece (constructor / LoadModel argument `type`): the /Game/Battle/3d/MapObj model it loads.
#define MAP_PIECE_KIND_LPOINT_01_00  0 // "Lpoint_01_00" (BattleCommandMap::mPredictPiece)
#define MAP_PIECE_KIND_LPOINT_00_00  1 // "Lpoint_00_00" (BattleCommandMap::mLandingPiece)
#define MAP_PIECE_KIND_SPHERE_SHADOW 2 // "sphere_shadow"

// Piece of the battle map (ctor 02263xxx, destructor 022636b0), deleted by pointer.
class BattleMapPiece : public ModelTransform {
public:
    BattleMapPiece(int unk); // 0226368c
    ~BattleMapPiece();
    void LoadModel(u32 type);
};

// Model of the player name plates, status icons and character markers (/Game/Battle/3d/FontObj), 0x2c bytes.
// Ctor 02248a5c loads the model of the given type (0..14), dtor 02248a80.
class BattleFontObj : public ModelTransform {
public:
    BattleFontObj(u32 type);
    ~BattleFontObj();
    void LoadModel(u32 type); // loads the texture and the model of the type
};

class TextureFile;

// One model of a BattleMapObj (0x2c bytes, no own members): dtor 02249068, loader 022490ac.
class BattleMapObjPart : public ModelTransform {
public:
    ~BattleMapObjPart();
    void LoadModel(TextureFile *texture, const char *nsbmd, const char *data, u32 unk0);
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
