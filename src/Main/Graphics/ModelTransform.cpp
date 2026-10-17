#include "Graphics/ModelTransform.hpp"
#include "Graphics/ModelInternal.hpp"

extern MtxFx43 gIdentityMtx43;

ModelTransform::ModelTransform() {
    mAnimEnabled = 0;
    mAnimRunning = 0;
    mModel  = NULL;
    mPosition.x = 0;
    mPosition.y = 0;
    mPosition.z = 0;
    mRotX = 0;
    mRotY = 0;
    mRotZ = 0;
    mScale.x = 0x1000;
    mScale.y = 0x1000;
    mScale.z = 0x1000;
}

ModelTransform::~ModelTransform() {
    ModelInstance *model = mModel;
    if (model != NULL) {
        model->mOwnFlags = 0xf;
        delete model;
        mModel = NULL;
    }
}

void ModelTransform::LoadFiles(const char *nsbtx, const char *nsbmd, const char *data, u32 unk0) {
    NitroTexture *texture = new NitroTexture(nsbtx);
    NitroModel *model     = new NitroModel(nsbmd, texture);
    texture->FreeFile();
    if (data != NULL) {
        ModelAnimData *animData = new ModelAnimData(data);
        ModelAnim *anim         = new ModelAnim(animData, model);
        mModel                  = model->CreateInstance(anim);
        mModel->StartAnimWithIncrement(0, 1, 0x1000);
        mAnimEnabled = unk0;
    } else {
        mModel  = model->CreateInstance(NULL);
        mAnimEnabled = 0;
    }
}

void ModelTransform::LoadFilesNoAnim(const char *nsbtx, const char *nsbmd) {
    LoadFiles(nsbtx, nsbmd, NULL, 0);
}

BOOL ModelTransform::func_020526bc() {
    if (mModel == NULL) {
        return FALSE;
    }
    AdvanceAnim();
    MtxFx43 mtx = gIdentityMtx43;
    if (mRotY != 0) {
        func_01ffd0e0(&mtx, mRotY);
    }
    if (mRotX != 0) {
        func_01ffd088(&mtx, mRotX);
    }
    if (mRotZ != 0) {
        func_01ffd138(&mtx, mRotZ);
    }
    func_01ffcf64(&mtx, &mPosition);
    return mModel->Draw(&mtx, &mScale);
}

void ModelTransform::StartAnim(u32 a, u32 b) {
    mModel->StartAnimWithIncrement(a, b, 0x1000);
}

void ModelTransform::AdvanceAnim() {
    if (mModel != NULL && mAnimEnabled != 0) {
        mAnimRunning = mModel->AdvanceAnim();
    }
}
