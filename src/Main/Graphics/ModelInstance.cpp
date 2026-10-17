#include "Graphics/Model.hpp"
#include "Graphics/Camera.hpp"
#include "Graphics/ModelInternal.hpp"
#include <nitro/os.h>

extern MtxFx33 data_02211430; // NNS_G3dGlb base rotation

ModelInstance::ModelInstance(ModelFile *model) {
    mUseBoxTest = 1;
    mModelFile = model;
    func_02031554(mRenderObj, model->mResModel);
    mUserData = this;
    mExtraAnim = NULL;
    mAnim   = NULL;
    mCallbackMtx = NULL;
    mAnimStarted = 0;
}

ModelInstance::~ModelInstance() {
    DisableCallbackMtx();
    DetachAllAnims();
    if (mAnim != NULL) {
        if (mOwnFlags & 8) {
            mAnim->FreeAnimData();
        }
        if ((mOwnFlags & 1) && mAnim != NULL) {
            delete mAnim;
        }
    }
    TextureFile *texture;
    ModelFile *file = mModelFile;
    if (file != NULL) {
        texture = file->mTexture;
        if ((mOwnFlags & 2) && file != NULL) {
            file->Release();
            delete file;
        }
        if (texture != NULL) {
            if ((mOwnFlags & 4) && texture != NULL) {
                texture->Release();
                delete texture;
            }
        }
    }
}

BOOL ModelInstance::AdvanceAnim() {
    BOOL result = TRUE;
    if (mAnim != NULL) {
        result = mAnim->Advance();
    }
    return result;
}

BOOL ModelInstance::AdvanceExtraAnim() {
    BOOL result = TRUE;
    if (mExtraAnim != NULL) {
        ModelAnim *anim = mAnim;
        if (anim->mPaused == 0) {
            result = anim->StepAnimObj(mExtraAnim) == 0;
        }
    }
    return result;
}

void ModelInstance::SetAnim(ModelAnim *anim) {
    mAnim = anim;
}

void ModelInstance::StartAnim(u32 a, u32 b) {
    DetachAllAnims();
    mAnim->SelectSet(a);
    mAnim->ResetFrames();
    for (u8 i = 0; i < 5; i++) {
        NNSG3dAnmObj *obj = mAnim->GetAnimObj(i);
        if (obj != NULL) {
            func_020316c8(mRenderObj, obj);
        }
    }
    mAnim->mLoop = b;
    mAnimStarted        = 1;
}

void ModelInstance::StartAnimWithIncrement(u32 a, u32 b, fx32 c) {
    StartAnim(a, b);
    mAnim->mFramePerFrame = c;
}

void ModelInstance::SetAnimFrame(s32 frame) {
    for (u8 i = 0; i < 5; i++) {
        mAnim->SetFrame(frame, i);
    }
}

s32 ModelInstance::GetAnimFrame() {
    s32 max = 0;
    for (u8 i = 0; i < 5; i++) {
        NNSG3dAnmObj *obj = mAnim->GetAnimObj(i);
        s32 frame;
        if (obj == NULL) {
            frame = 0;
        } else {
            frame = obj->mFrame;
        }
        if (frame > max) {
            max = frame;
        }
    }
    return max;
}

void ModelInstance::DetachAllAnims() {
    if (mAnimStarted != 0 && mAnim != NULL) {
        for (u8 i = 0; i < 5; i++) {
            NNSG3dAnmObj *obj = mAnim->GetAnimObj(i);
            if (obj != NULL) {
                func_020317bc(mRenderObj, obj);
            }
        }
    }
    DetachExtraAnim();
}

s32 ModelInstance::IsAnimFinished() {
    return mAnim->mReachedEnd;
}

void ModelInstance::AttachExtraAnim(NNSG3dAnmObj *obj) {
    DetachExtraAnim();
    if (obj == NULL) {
        return;
    }
    func_020316c8(mRenderObj, obj);
    mExtraAnim     = obj;
    obj->mFrame = 0;
}

void ModelInstance::DetachExtraAnim() {
    if (mExtraAnim == NULL) {
        return;
    }
    func_020317bc(mRenderObj, mExtraAnim);
    mExtraAnim = NULL;
}

void ModelInstance::StoreCallbackMtx(NNSG3dRS *rs) {
    ModelInstance *instance = (ModelInstance *) rs->mRenderObj->mUserData;
    s32 index;
    if (rs->mFlags & 0x10) {
        index = rs->mUnk_ae;
    } else {
        index = -1;
    }
    func_02032cc8(&instance->mCallbackMtx[index], NULL);
    instance->mCallbackMtxStored = 1;
}

void ModelInstance::EnableCallbackMtx() {
    if (mCallbackMtx != NULL) {
        return;
    }
    mCallbackMtx = new MtxFx43[((u8 *) mModelFile->mResModel)[0x17]];
    func_02031810(mRenderObj, StoreCallbackMtx, 0, 6, 3);
    mCallbackMtxStored = 0;
}

void ModelInstance::DisableCallbackMtx() {
    if (mCallbackMtx == NULL) {
        return;
    }
    func_02031824(mRenderObj);
    delete[] mCallbackMtx;
    mCallbackMtx = NULL;
}

MtxFx43 *ModelInstance::GetCallbackMtx(s32 index) {
    if (mCallbackMtxStored != 0 && index <= ((u8 *) mModelFile->mResModel)[0x17] && mCallbackMtx != NULL) {
        return &mCallbackMtx[index];
    }
    return NULL;
}

BOOL ModelInstance::BoxTest() {
    if (mUseBoxTest == 0) {
        return TRUE;
    }
    func_01ff92a4();
    u8 *model = (u8 *) mModelFile->mResModel;
    *(vu32 *) 0x040004a4 = 0x30c1;
    *(vu32 *) 0x04000500 = 0;
    *(vu32 *) 0x04000504 = 0;
    *(vu32 *) 0x04000444 = 0;
    u32 scale = *(u32 *) (model + 0x38);
    *(vu32 *) 0x0400046c = scale;
    *(vu32 *) 0x0400046c = scale;
    *(vu32 *) 0x0400046c = scale;
    *(vu32 *) 0x040005c0 = *(u32 *) (model + 0x2c);
    *(vu32 *) 0x040005c0 = *(u32 *) (model + 0x30);
    *(vu32 *) 0x040005c0 = *(u32 *) (model + 0x34);
    s32 result;
    while (G3X_GetBoxTestResult(&result) != 0) {
    }
    *(vu32 *) 0x04000448 = 1;
    return result != 0;
}

BOOL ModelInstance::Draw(MtxFx43 *mtx, VecFx32 *pos) {
    if (mtx != NULL) {
        MI_Copy36B(mtx, &data_02211430);
        data_02211374.mFlags &= ~0xa4;
        func_020322c0((VecFx32 *) &mtx->_[9]);
    }
    if (pos != NULL) {
        func_020322f0(pos);
    }
    func_02032284();
    if (!BoxTest()) {
        return FALSE;
    }
    if (!CameraManager::Get().mModelsVisible) {
        return TRUE;
    }
    func_01ffb0ec(mRenderObj);
    return TRUE;
}
