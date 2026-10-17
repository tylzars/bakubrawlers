#include "Battle/BattleMapTypes.hpp"
#include "System/FileSystem.hpp"
#include <stdio.h>

BattleMapPiece::BattleMapPiece(int type) {
    func_ov007_022636f4(type);
}

BattleMapPiece::~BattleMapPiece() {
    ModelInstance *model = mModel;
    if (model) {
        model->mOwnFlags = 0xf;
        delete model;
        mModel = 0;
    }
}

void BattleMapPiece::func_ov007_022636f4(u32 type) {
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
