#include "Battle/BattleMapTypes.hpp"
#include "System/FileSystem.hpp"
#include <stdio.h>

BattleMapPiece::BattleMapPiece(int type) {
    func_ov007_022636f4(type);
}

BattleMapPiece::~BattleMapPiece() {
    ModelInstance *model = mModel;
    if (model) {
        model->mUnk_68 = 0xf;
        delete model;
        mModel = 0;
    }
}

void BattleMapPiece::func_ov007_022636f4(u32 type) {
    ChangeDir("/Game/Battle/3d/MapObj");
    switch (type) {
        case 0:
            func_020526a8("Lpoint_01_00.nsbtx", "Lpoint_01_00.nsbmd");
            break;
        case 1:
            func_020526a8("Lpoint_00_00.nsbtx", "Lpoint_00_00.nsbmd");
            break;
        case 2:
            func_020526a8("sphere_shadow.nsbtx", "sphere_shadow.nsbmd");
            break;
    }
    ChangeDir("/");
}
