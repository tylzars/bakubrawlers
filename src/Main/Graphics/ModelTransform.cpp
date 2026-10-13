#include "Graphics/ModelTransform.hpp"
#include "Graphics/ModelInternal.hpp"

extern MtxFx43 data_0220e86c;

ModelTransform::ModelTransform() {
    mUnk_00 = 0;
    mUnk_25 = 0;
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
        model->mUnk_68 = 0xf;
        delete model;
        mModel = NULL;
    }
}

void ModelTransform::func_020525c0(const char *nsbtx, const char *nsbmd, const char *data, u32 unk0) {
    NitroTexture *texture = new NitroTexture(nsbtx);
    NitroModel *model     = new NitroModel(nsbmd, texture);
    texture->func_02028344();
    if (data != NULL) {
        ModelAnimData *animData = new ModelAnimData(data);
        ModelAnim *anim         = new ModelAnim(animData, model);
        mModel                  = model->func_02028b78(anim);
        mModel->func_02028e54(0, 1, 0x1000);
        mUnk_00 = unk0;
    } else {
        mModel  = model->func_02028b78(NULL);
        mUnk_00 = 0;
    }
}

void ModelTransform::func_020526a8(const char *nsbtx, const char *nsbmd) {
    func_020525c0(nsbtx, nsbmd, NULL, 0);
}

BOOL ModelTransform::func_020526bc() {
    if (mModel == NULL) {
        return FALSE;
    }
    func_0205277c();
    MtxFx43 mtx = data_0220e86c;
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
    return mModel->func_02029174(&mtx, &mScale);
}

void ModelTransform::func_02052768(u32 a, u32 b) {
    mModel->func_02028e54(a, b, 0x1000);
}

void ModelTransform::func_0205277c() {
    if (mModel != NULL && mUnk_00 != 0) {
        mUnk_25 = mModel->func_02028d80();
    }
}
