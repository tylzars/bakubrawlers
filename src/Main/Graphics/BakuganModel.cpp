#include "Graphics/BakuganModel.hpp"

#include "Graphics/Model.hpp"
#include "Graphics/ModelInternal.hpp"
#include "System/FileSystem.hpp"
#include <stdio.h>

extern MtxFx43 gIdentityMtx43;
extern const u8 data_020a1074[];

BakuganModel::BakuganModel() {
    mUnk_4b = 0;
    mUnk_32 = 0;
    mUnk_33 = 0;
    mUnk_34 = 0;
    mMode = 0;
    mVisible = 0;
    mAnimUpdateEnabled = 0;
    mMode = 0;
    mVariant = 0;
    mKind = 0;
    mUnk_48 = 0;
    mSpinStep = 0;
    mSpinTurnCount = 0;
    mKind = 0;
    u8 i    = 0;
    do {
        mUnk_38[i].mUnk_06 = 0;
        mUnk_38[i].mUnk_00 = 0;
        mUnk_38[i].mUnk_04 = 0;
        i++;
    } while (i < 2);
}

BakuganModel::~BakuganModel() {
    ModelInstance *model = mModel;
    if (model) {
        if (mUnk_48) {
            model->mOwnFlags = 0;
            delete model;
        } else {
            model->mOwnFlags = 0xf;
            delete model;
        }
        mModel = 0;
    }
}

extern "C" void FreeBakuganModelInstance(BakuganModel *pose) {
    ModelInstance *model = pose->mModel;
    if (!model) {
        return;
    }
    if (pose->mUnk_48) {
        model->mOwnFlags = 0;
        delete model;
    } else {
        model->mOwnFlags = 0xf;
        delete model;
    }
    pose->mModel = 0;
}

void BakuganModel::SetupMode(u32 unk1) {
    Setup(mKind, mVariant, unk1);
}

void BakuganModel::Setup(u32 kind, u32 value, u32 mode) {
    char name[0x40];
    char file[0x40];
    mUnk_48 = 0;
    mVisible = 1;
    mMode = mode;
    mVariant = value;
    mKind = kind;
    if ((s32) kind >= 0x29) {
        return;
    }
    LoadModel(kind);
    ChangeDir("/Game/Bakugan/tex");
    TextureFile *texture = mModel->mModelFile->mTexture;
    u8 id                       = data_020a1074[(u8) kind];
    if ((u8) (mode + 0xff) <= 1) {
        if (kind == 0x22 && value == 0) {
            sprintf(name, "L_sphe83");
            sprintf(file, "H_sphe83");
        } else {
            sprintf(name, "L_sphe%02d", id);
            sprintf(file, "H_sphe%02d_%02d", id, value);
        }
        texture->LoadPaletteFile(name, file, 0);
    } else {
        if (kind == 0x22 && value == 0) {
            sprintf(name, "H_sphe83");
            sprintf(file, "%s", name);
        } else {
            sprintf(name, "H_sphe%02d", id);
            sprintf(file, "%s_%02d", name, value);
        }
        texture->LoadPaletteFile(name, file, 0);
        texture->LoadTextureFile(name, file, 0);
    }
    texture->FreeFile();
    ChangeDir("/");
    if ((u8) (mode + 0xfd) <= 1) {
        StartAnim(0, 1);
        if (mode == 4) {
            mAnimUpdateEnabled = 1;
        }
    }
    if ((u8) (mode + 0xfb) <= 1) {
        mAnimUpdateEnabled = 0;
        StartAnim(0, 0);
    }
}

void BakuganModel::StoreSetup(u32 unk1, u32 unk2, u32 unk3) {
    mStoredKind = unk1;
    mStoredVariant = unk2;
    mStoredMode = unk3;
}

BOOL BakuganModel::func_0203d6f0() {
    return TRUE;
}

void BakuganModel::RunStoredSetup() {
    Setup(mStoredKind, mStoredVariant, mStoredMode);
}

BOOL BakuganModel::LoadModel(u8 kind) {
    char path[0x100];
    if (mUnk_48) {
        return FALSE;
    }
    ModelInstance *old = mModel;
    if (old) {
        old->mOwnFlags = 0xf;
        delete old;
        mModel = 0;
    }
    u8 id = data_020a1074[kind];
    if (kind == 0x22 && mVariant == 0) {
        id = 0x53;
    }
    ChangeDir("/Game/Bakugan/mdl");
    NitroTexture *texture;
    if ((u8) (mMode + 0xff) <= 1) {
        sprintf(path, "L_sphe%02d.nsbtx", id);
        texture = new NitroTexture(path);
    } else {
        sprintf(path, "H_sphe%02d.nsbtx", id);
        texture = new NitroTexture(path);
    }
    NitroModel *model = new NitroModel();
    if (mMode == 2) {
        sprintf(path, "L_sphe%02d.nsbmd", id);
        if (!model->Load(path, texture)) {
            return FALSE;
        }
    } else if (mMode == 1) {
        sprintf(path, "M_sphe%02d.nsbmd", id);
        if (!model->Load(path, texture)) {
            return FALSE;
        }
    } else {
        sprintf(path, "H_sphe%02d.nsbmd", id);
        if (!model->Load(path, texture)) {
            return FALSE;
        }
    }
    if (mMode != 2 && mMode != 1) {
        sprintf(path, "H_sphe%02d_data.bin", id);
        ModelAnimData *data = new ModelAnimData(path);
        ModelAnim *anim     = new ModelAnim(data, model);
        mModel              = model->CreateInstance(anim);
    } else {
        mModel = model->CreateInstance(NULL);
    }
    ChangeDir("/");
    return TRUE;
}

void BakuganModel::Update() {
    if (mMode == 6) {
        UpdateSpin();
    }
    if (!mAnimUpdateEnabled) {
        return;
    }
    if (mMode == 3) {
        mModel->mAnim->SetFrame(0x96000, 5);
    }
    if (mModel) {
        mModel->AdvanceAnim();
    }
}

void BakuganModel::UpdateSpin() {
    switch (mSpinStep) {
        case 0:
            if (mSpinTurnCount < 3) {
                mRotX += 0x38e;
                if (mRotX > 0xfc72) {
                    mSpinTurnCount++;
                    mRotX = 0;
                }
            } else {
                mSpinTurnCount = 0;
                mSpinStep++;
            }
            break;
        case 1:
            mAnimUpdateEnabled = 1;
            mSpinStep++;
            break;
        case 2:
            if (mModel->IsAnimFinished()) {
                mSpinStep++;
            }
            break;
        case 3:
            mRotY += 0xb6;
            break;
    }
}

void BakuganModel::DrawModel() {
    if (!mVisible) {
        return;
    }
    VecFx32 pos;
    fx32 z      = mPosition.z;
    fx32 y      = mPosition.y;
    fx32 x      = mPosition.x;
    MtxFx43 mtx = gIdentityMtx43;
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
        mModel->Draw(&mtx, &mScale);
    }
}

void BakuganModel::func_0203db70(const VecFx32 *pos) {
    SetPosition(pos);
}

void SetTransformPosition(void *obj, fx32 x, fx32 y, fx32 z) {
    VecFx32 *pos = (VecFx32 *) ((u8 *) obj + 4);
    pos->x       = x;
    pos->y       = y;
    pos->z       = z;
}

void BakuganModel::func_0203db94(const u16 *rot) {
    SetRotation(rot);
}
