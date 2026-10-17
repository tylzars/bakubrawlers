#include "Battle/BattleMapTypes.hpp"
#include "Graphics/ModelTransform.hpp"
#include "System/FileSystem.hpp"
#include <stdio.h>

BattleFontObj::BattleFontObj(u32 type) {
    LoadModel(type);
}

BattleFontObj::~BattleFontObj() {
    ModelInstance *model = mModel;
    if (model) {
        model->mOwnFlags = 0xf;
        delete model;
        mModel = 0;
    }
}

void BattleFontObj::LoadModel(u32 type) {
    char model[0x40];
    char texture[0x40];
    u32 index;
    ChangeDir("/Game/Battle/3d/FontObj");
    switch (type) {
        case 0:
        case 1:
        case 2:
        case 3:
            sprintf(texture, "pname%02d.nsbtx", type + 1);
            sprintf(model, "pname%02d.nsbmd", type + 1);
            LoadFilesNoAnim(texture, model);
            break;
        case 4:
        case 5:
        case 6:
            index = type - 4;
            sprintf(texture, "st_icon%02d.nsbtx", index);
            sprintf(model, "st_icon%02d.nsbmd", index);
            LoadFilesNoAnim(texture, model);
            break;
        case 7:
        case 8:
        case 9:
        case 10:
            index = type - 6;
            sprintf(texture, "py%d.nsbtx", index);
            sprintf(model, "py%d.nsbmd", index);
            LoadFilesNoAnim(texture, model);
            break;
        case 11:
        case 12:
        case 13:
        case 14:
            index = type - 10;
            sprintf(texture, "pt%d.nsbtx", index);
            sprintf(model, "pt%d.nsbmd", index);
            LoadFilesNoAnim(texture, model);
            break;
    }
    if (mModel && mModel->mAnim) {
        mAnimEnabled = 1;
        StartAnim(0, 0);
    }
    ChangeDir("/");
}
