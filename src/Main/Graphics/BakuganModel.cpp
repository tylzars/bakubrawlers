#include "Graphics/BakuganModel.hpp"

#include "Graphics/Model.hpp"
#include "Graphics/ModelInternal.hpp"
#include "System/FileSystem.hpp"
#include <stdio.h>

extern MtxFx43 gIdentityMtx43;
extern const u8 gBakuganNumbers[];

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
    mFreeInstanceOnly = 0;
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
        if (mFreeInstanceOnly) {
            model->mOwnFlags = 0;
            delete model;
        } else {
            model->mOwnFlags = MODEL_OWN_ALL;
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
    if (pose->mFreeInstanceOnly) {
        model->mOwnFlags = 0;
        delete model;
    } else {
        model->mOwnFlags = MODEL_OWN_ALL;
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
    mFreeInstanceOnly = 0;
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
    u8 id                       = gBakuganNumbers[(u8) kind];
    if ((u8) (mode + 0xff) <= 1) { // BAKUGAN_MODEL_MODE_M or BAKUGAN_MODEL_MODE_L
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
    if ((u8) (mode + 0xfd) <= 1) { // BAKUGAN_MODEL_MODE_H_HOLD_FRAME or BAKUGAN_MODEL_MODE_H_LOOP
        StartAnim(0, 1);
        if (mode == BAKUGAN_MODEL_MODE_H_LOOP) {
            mAnimUpdateEnabled = 1;
        }
    }
    if ((u8) (mode + 0xfb) <= 1) { // BAKUGAN_MODEL_MODE_H_NO_LOOP or BAKUGAN_MODEL_MODE_H_SPIN
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
    if (mFreeInstanceOnly) {
        return FALSE;
    }
    ModelInstance *old = mModel;
    if (old) {
        old->mOwnFlags = MODEL_OWN_ALL;
        delete old;
        mModel = 0;
    }
    u8 id = gBakuganNumbers[kind];
    if (kind == 0x22 && mVariant == 0) {
        id = 0x53;
    }
    ChangeDir("/Game/Bakugan/mdl");
    NitroTexture *texture;
    if ((u8) (mMode + 0xff) <= 1) { // BAKUGAN_MODEL_MODE_M or BAKUGAN_MODEL_MODE_L
        sprintf(path, "L_sphe%02d.nsbtx", id);
        texture = new NitroTexture(path);
    } else {
        sprintf(path, "H_sphe%02d.nsbtx", id);
        texture = new NitroTexture(path);
    }
    NitroModel *model = new NitroModel();
    if (mMode == BAKUGAN_MODEL_MODE_L) {
        sprintf(path, "L_sphe%02d.nsbmd", id);
        if (!model->Load(path, texture)) {
            return FALSE;
        }
    } else if (mMode == BAKUGAN_MODEL_MODE_M) {
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
    if (mMode != BAKUGAN_MODEL_MODE_L && mMode != BAKUGAN_MODEL_MODE_M) {
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
    if (mMode == BAKUGAN_MODEL_MODE_H_SPIN) {
        UpdateSpin();
    }
    if (!mAnimUpdateEnabled) {
        return;
    }
    if (mMode == BAKUGAN_MODEL_MODE_H_HOLD_FRAME) {
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
    MTX_SetTranslation43(&mtx, &pos);
    if (mRotY) {
        MTX_ApplyRotY43(&mtx, mRotY);
    }
    if (mRotX) {
        MTX_ApplyRotX43(&mtx, mRotX);
    }
    if (mRotZ) {
        MTX_ApplyRotZ43(&mtx, mRotZ);
    }
    if (mModel) {
        mModel->Draw(&mtx, &mScale);
    }
}

void BakuganModel::SetPositionOutOfLine(const VecFx32 *pos) {
    SetPosition(pos);
}

void SetTransformPosition(void *obj, fx32 x, fx32 y, fx32 z) {
    VecFx32 *pos = (VecFx32 *) ((u8 *) obj + 4);
    pos->x       = x;
    pos->y       = y;
    pos->z       = z;
}

void BakuganModel::SetRotationOutOfLine(const u16 *rot) {
    SetRotation(rot);
}
