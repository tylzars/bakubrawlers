#include "Graphics/UnkStruct_0203d374.hpp"

#include "Graphics/Model.hpp"
#include "Graphics/ModelInternal.hpp"
#include "System/FileSystem.hpp"
#include <stdio.h>

extern MtxFx43 data_0220e86c;
extern u8 data_020a1074[];

UnkStruct_0203d374::UnkStruct_0203d374() {
    mUnk_4b = 0;
    mUnk_32 = 0;
    mUnk_33 = 0;
    mUnk_34 = 0;
    mUnk_4c = 0;
    mUnk_4a = 0;
    mUnk_49 = 0;
    mUnk_4c = 0;
    mUnk_4d = 0;
    mUnk_4f = 0;
    mUnk_48 = 0;
    mUnk_4e = 0;
    mUnk_2c = 0;
    mUnk_4f = 0;
    u8 i    = 0;
    do {
        mUnk_38[i].mUnk_06 = 0;
        mUnk_38[i].mUnk_00 = 0;
        mUnk_38[i].mUnk_04 = 0;
        i++;
    } while (i < 2);
}

UnkStruct_0203d374::~UnkStruct_0203d374() {
    ModelInstance *model = mModel;
    if (model) {
        if (mUnk_48) {
            model->mUnk_68 = 0;
            delete model;
        } else {
            model->mUnk_68 = 0xf;
            delete model;
        }
        mModel = 0;
    }
}

extern "C" void func_0203d45c(UnkStruct_0203d374 *pose) {
    ModelInstance *model = pose->mModel;
    if (!model) {
        return;
    }
    if (pose->mUnk_48) {
        model->mUnk_68 = 0;
        delete model;
    } else {
        model->mUnk_68 = 0xf;
        delete model;
    }
    pose->mModel = 0;
}

void UnkStruct_0203d374::func_0203d4c4(u32 unk1) {
    func_0203d4dc(mUnk_4f, mUnk_4d, unk1);
}

void UnkStruct_0203d374::func_0203d4dc(u32 kind, u32 value, u32 mode) {
    char name[0x40];
    char file[0x40];
    mUnk_48 = 0;
    mUnk_4a = 1;
    mUnk_4c = mode;
    mUnk_4d = value;
    mUnk_4f = kind;
    if ((s32) kind >= 0x29) {
        return;
    }
    func_0203d710(kind);
    ChangeDir("/Game/Bakugan/tex");
    UnkStruct_02028308 *texture = mModel->mUnk_58->mUnk_08;
    u8 id                       = data_020a1074[(u8) kind];
    if ((u8) (mode + 0xff) <= 1) {
        if (kind == 0x22 && value == 0) {
            sprintf(name, "L_sphe83");
            sprintf(file, "H_sphe83");
        } else {
            sprintf(name, "L_sphe%02d", id);
            sprintf(file, "H_sphe%02d_%02d", id, value);
        }
        texture->func_02028858(name, file, 0);
    } else {
        if (kind == 0x22 && value == 0) {
            sprintf(name, "H_sphe83");
            sprintf(file, "%s", name);
        } else {
            sprintf(name, "H_sphe%02d", id);
            sprintf(file, "%s_%02d", name, value);
        }
        texture->func_02028858(name, file, 0);
        texture->func_020286fc(name, file, 0);
    }
    texture->func_02028344();
    ChangeDir("/");
    if ((u8) (mode + 0xfd) <= 1) {
        func_02052768(0, 1);
        if (mode == 4) {
            mUnk_49 = 1;
        }
    }
    if ((u8) (mode + 0xfb) <= 1) {
        mUnk_49 = 0;
        func_02052768(0, 0);
    }
}

void UnkStruct_0203d374::func_0203d6e0(u32 unk1, u32 unk2, u32 unk3) {
    mUnk_50 = unk1;
    mUnk_51 = unk2;
    mUnk_52 = unk3;
}

BOOL UnkStruct_0203d374::func_0203d6f0() {
    return TRUE;
}

void UnkStruct_0203d374::func_0203d6f8() {
    func_0203d4dc(mUnk_50, mUnk_51, mUnk_52);
}

BOOL UnkStruct_0203d374::func_0203d710(u8 kind) {
    char path[0x100];
    if (mUnk_48) {
        return FALSE;
    }
    ModelInstance *old = mModel;
    if (old) {
        old->mUnk_68 = 0xf;
        delete old;
        mModel = 0;
    }
    u8 id = data_020a1074[kind];
    if (kind == 0x22 && mUnk_4d == 0) {
        id = 0x53;
    }
    ChangeDir("/Game/Bakugan/mdl");
    NitroTexture *texture;
    if ((u8) (mUnk_4c + 0xff) <= 1) {
        sprintf(path, "L_sphe%02d.nsbtx", id);
        texture = new NitroTexture(path);
    } else {
        sprintf(path, "H_sphe%02d.nsbtx", id);
        texture = new NitroTexture(path);
    }
    NitroModel *model = new NitroModel();
    if (mUnk_4c == 2) {
        sprintf(path, "L_sphe%02d.nsbmd", id);
        if (!model->func_020289e0(path, texture)) {
            return FALSE;
        }
    } else if (mUnk_4c == 1) {
        sprintf(path, "M_sphe%02d.nsbmd", id);
        if (!model->func_020289e0(path, texture)) {
            return FALSE;
        }
    } else {
        sprintf(path, "H_sphe%02d.nsbmd", id);
        if (!model->func_020289e0(path, texture)) {
            return FALSE;
        }
    }
    if (mUnk_4c != 2 && mUnk_4c != 1) {
        sprintf(path, "H_sphe%02d_data.bin", id);
        ModelAnimData *data = new ModelAnimData(path);
        ModelAnim *anim     = new ModelAnim(data, model);
        mModel              = model->func_02028b78(anim);
    } else {
        mModel = model->func_02028b78(NULL);
    }
    ChangeDir("/");
    return TRUE;
}

void UnkStruct_0203d374::func_0203d988() {
    if (mUnk_4c == 6) {
        func_0203d9e0();
    }
    if (!mUnk_49) {
        return;
    }
    if (mUnk_4c == 3) {
        mModel->mAnim->func_020298c0(0x96000, 5);
    }
    if (mModel) {
        mModel->func_02028d80();
    }
}

void UnkStruct_0203d374::func_0203d9e0() {
    switch (mUnk_4e) {
        case 0:
            if (mUnk_2c < 3) {
                mRotX += 0x38e;
                if (mRotX > 0xfc72) {
                    mUnk_2c++;
                    mRotX = 0;
                }
            } else {
                mUnk_2c = 0;
                mUnk_4e++;
            }
            break;
        case 1:
            mUnk_49 = 1;
            mUnk_4e++;
            break;
        case 2:
            if (mModel->func_02028f48()) {
                mUnk_4e++;
            }
            break;
        case 3:
            mRotY += 0xb6;
            break;
    }
}

void UnkStruct_0203d374::func_0203dab0() {
    if (!mUnk_4a) {
        return;
    }
    VecFx32 pos;
    fx32 z      = mPosition.z;
    fx32 y      = mPosition.y;
    fx32 x      = mPosition.x;
    MtxFx43 mtx = data_0220e86c;
    pos.x = x;
    pos.y = y;
    pos.z = z;
    func_01ffcf64(&mtx, &pos);
    if (mRotY) {
        func_01ffd0e0(&mtx, mRotY);
    }
    if (mRotX) {
        func_01ffd088(&mtx, mRotX);
    }
    if (mRotZ) {
        func_01ffd138(&mtx, mRotZ);
    }
    if (mModel) {
        mModel->func_02029174(&mtx, &mScale);
    }
}

void UnkStruct_0203d374::func_0203db70(const VecFx32 *pos) {
    SetPosition(pos);
}

void func_0203db8c(void *obj, fx32 x, fx32 y, fx32 z) {
    VecFx32 *pos = (VecFx32 *) ((u8 *) obj + 4);
    pos->x       = x;
    pos->y       = y;
    pos->z       = z;
}

void UnkStruct_0203d374::func_0203db94(const u16 *rot) {
    SetRotation(rot);
}
