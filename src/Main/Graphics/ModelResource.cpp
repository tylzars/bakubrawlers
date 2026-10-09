#include "Graphics/ModelResource.hpp"
#include "Graphics/ModelInternal.hpp"

extern "C" u32 func_02012328(const char *str);
extern "C" void MI_CpuCopy8(const void *src, void *dst, u32 size);
void *operator new[](size_t size);

// non-matching: 1 word. The original reads the stack argument with `ldr`, we emit `ldrb` for u8 (the callee-side
// narrow-argument problem); with a u32 parameter (different mangling) it matches 4/4.
ModelResource::ModelResource(const char *nsbmd, const char *data, const char *nsbtx, u32 unk4) {
    mModelFile   = NULL;
    mTextureFile = NULL;
    mDataFile    = NULL;
    mTexture     = NULL;
    mModel       = NULL;
    mAnimData    = NULL;
    mRefs        = 0;
    mUnk_1a      = unk4;
    if (nsbmd != NULL) {
        u32 len    = func_02012328(nsbmd);
        mModelFile = new char[len + 1];
        MI_CpuCopy8(nsbmd, mModelFile, len + 1);
    }
    if (nsbtx != NULL) {
        u32 len      = func_02012328(nsbtx);
        mTextureFile = new char[len + 1];
        MI_CpuCopy8(nsbtx, mTextureFile, len + 1);
        mUnk_1a = 0;
    }
    if (data != NULL) {
        u32 len   = func_02012328(data);
        mDataFile = new char[len + 1];
        MI_CpuCopy8(data, mDataFile, len + 1);
    }
}

ModelResource::~ModelResource() {
    while (mRefs != 0) {
        func_020307e8();
    }
    delete mModelFile;
    mModelFile = NULL;
    delete mTextureFile;
    mTextureFile = NULL;
    delete mDataFile;
    mDataFile = NULL;
}

u16 ModelResource::func_020306e4(u8 freeTexture) {
    if (mRefs == 0) {
        if (mTextureFile != NULL) {
            mTexture = new NitroTexture(mTextureFile);
        }
        if (mDataFile != NULL) {
            mAnimData = new ModelAnimData(mDataFile);
        }
        if (mModelFile != NULL) {
            if (mUnk_1a != 0) {
                NitroModel *model = new NitroModel;
                mModel            = model;
                model->func_02028ac4(mModelFile);
            } else {
                mModel = new NitroModel(mModelFile, mTexture);
            }
        }
        if (mTexture != NULL && freeTexture != 0) {
            mTexture->func_02028344();
        }
    }
    return mRefs++;
}

BOOL ModelResource::func_020307e8() {
    BOOL result = FALSE;
    mRefs--;
    if (mRefs == 0) {
        NitroTexture *texture = mTexture;
        if (texture != NULL) {
            texture->func_0202832c();
            delete texture;
        }
        mTexture = NULL;
        NitroModel *model = mModel;
        if (model != NULL) {
            model->func_02028af0();
            delete model;
        }
        mModel = NULL;
        ModelAnimData *anim = mAnimData;
        if (anim != NULL) {
            delete anim;
        }
        mAnimData = NULL;
        result    = TRUE;
    }
    return result;
}
