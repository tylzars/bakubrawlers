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
        case 0:
            LoadFilesNoAnim("Lpoint_01_00.nsbtx", "Lpoint_01_00.nsbmd");
            break;
        case 1:
            LoadFilesNoAnim("Lpoint_00_00.nsbtx", "Lpoint_00_00.nsbmd");
            break;
        case 2:
            LoadFilesNoAnim("sphere_shadow.nsbtx", "sphere_shadow.nsbmd");
            break;
    }
    ChangeDir("/");
}
