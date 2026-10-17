#include "Battle/BattleMapTypes.hpp"
#include "System/FileSystem.hpp"
#include <stdio.h>

BattleMapPiece::BattleMapPiece(int type) {
    LoadModel(type);
}

BattleMapPiece::~BattleMapPiece() {
    ModelInstance *model = mModel;
    if (model) {
        model->mOwnFlags = MODEL_OWN_ALL;
        delete model;
        mModel = 0;
    }
}

void BattleMapPiece::LoadModel(u32 type) {
    ChangeDir("/Game/Battle/3d/MapObj");
    switch (type) {
        case MAP_PIECE_KIND_LPOINT_01_00:
            LoadFilesNoAnim("Lpoint_01_00.nsbtx", "Lpoint_01_00.nsbmd");
            break;
        case MAP_PIECE_KIND_LPOINT_00_00:
            LoadFilesNoAnim("Lpoint_00_00.nsbtx", "Lpoint_00_00.nsbmd");
            break;
        case MAP_PIECE_KIND_SPHERE_SHADOW:
            LoadFilesNoAnim("sphere_shadow.nsbtx", "sphere_shadow.nsbmd");
            break;
    }
    ChangeDir("/");
}
