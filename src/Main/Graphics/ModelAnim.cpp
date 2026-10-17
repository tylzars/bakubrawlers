#include "Graphics/Model.hpp"
#include "Graphics/ModelInternal.hpp"
#include <nitro/os.h>

void operator delete(void *ptr);

ModelAnim::ModelAnim(const void *data, NitroModel *model) {
    Init();
    mNitroModel = model;
    mAnimData = (ModelAnimData *) data;
    for (u8 i = 0; i < 5; i++) {
        ModelAnimData *d = (ModelAnimData *) data;
        InitChannel(i, d->mAnims[i], (u8 *) d->mFile[i].mTable);
    }
}

ModelAnim::~ModelAnim() {
    FreeAnimObjs();
}

void ModelAnim::Init() {
    for (u8 i = 0; i < 5; i++) {
        mAnimFiles[i] = NULL;
        mAnimObjs[i] = NULL;
        mAnimObjHandles[i] = NULL;
        mSet    = 0;
    }
    mPaused = 0;
    mLoop = 1;
    mFramePerFrame = 0x1000;
    mNitroModel = NULL;
    mAnimData = NULL;
}

BOOL ModelAnim::Advance() {
    if (mPaused != 0) {
        return TRUE;
    }
    for (u8 i = 0; i < 5; i++) {
        NNSG3dAnmObj *obj = GetAnimObj(i);
        if (obj != NULL) {
            StepAnimObjAndRecordEnd(obj);
        }
    }
    return !((mLoop == 0) & mReachedEnd);
}

void ModelAnim::InitChannel(u32 channel, ModelAnimFile *anim, u8 *table) {
    if (anim == NULL || table == NULL) {
        return;
    }
    mAnimFiles[channel] = anim;
    u8 count         = 0;
    switch (channel) {
    case 0:
        count = func_02033810(anim->mFile)->mCount;
        break;
    case 1:
        count = func_020336a0(anim->mFile)->mCount;
        break;
    case 2:
        count = func_020337f4(anim->mFile)->mCount;
        break;
    case 3:
        count = func_020337bc(anim->mFile)->mCount;
        break;
    case 4:
        count = func_020337d8(anim->mFile)->mCount;
        break;
    }
    if (count == 0) {
        return;
    }
    mAnimObjCounts[channel] = count;
    mAnimObjHandles[channel] = table;
    mAnimObjs[channel] = (NNSG3dAnmObj **) OS_Alloc(count * 4);
    u8 i = 0;
    do {
        mAnimObjs[channel][i] = NULL;
        i++;
    } while (i < count);
}

void ModelAnim::ResetFrames() {
    mReachedEnd = 0;
    for (u8 channel = 0; channel < 5; channel++) {
        NNSG3dAnmObj *obj = GetAnimObj(channel);
        if (obj == NULL) {
            continue;
        }
        obj->mFrame = 0;
        switch (channel) {
        case 0:
            func_020299ec();
            break;
        case 2:
            for (u8 i = 0; i < ((u8 *) mNitroModel->mResModel)[0x18]; i++) {
                func_02031518(obj, i);
            }
            break;
        }
    }
}

s32 ModelAnim::GetFrameCountForSet(u32 set) {
    s32 max = 0;
    for (u8 channel = 0; channel < 5; channel++) {
        NNSG3dAnmObj *obj = GetAnimObjForSet(channel, set);
        if (obj != NULL) {
            s32 end = obj->mRes->mFrames << 12;
            if (max < end) {
                max = end;
            }
        }
    }
    return max;
}

void ModelAnim::StepAnimObjAndRecordEnd(NNSG3dAnmObj *obj) {
    mReachedEnd = (StepAnimObj(obj) != 0 || mReachedEnd != 0);
}

BOOL ModelAnim::StepAnimObj(NNSG3dAnmObj *obj) {
    s32 frame = obj->mFrame + mFramePerFrame;
    BOOL done = FALSE;
    obj->mFrame = frame;
    s32 end = obj->mRes->mFrames << 12;
    if (frame >= end) {
        done = TRUE;
        if (mLoop != 0) {
            obj->mFrame = frame - end;
        } else {
            obj->mFrame = end - 1;
        }
    } else if (frame < 0) {
        if (mLoop != 0) {
            obj->mFrame = frame + end;
        } else {
            obj->mFrame = done;
        }
    }
    return done;
}

void ModelAnim::SetFrame(u32 value, u32 index) {
    if (index == 5) {
        for (u8 i = 0; i < 5; i++) {
            NNSG3dAnmObj *obj = GetAnimObj(i);
            if (obj != NULL) {
                obj->mFrame = value;
            }
        }
    } else {
        NNSG3dAnmObj *obj = GetAnimObj(index);
        if (obj != NULL) {
            obj->mFrame = value;
        }
    }
}

void ModelAnim::FreeAnimObjs() {
    for (u8 i = 0; i < 5; i++) {
        if (mAnimObjs[i] != NULL) {
            for (u8 j = 0; j < mAnimObjCounts[i]; j++) {
                if (mAnimObjs[i][j] != NULL) {
                    OS_Free(mAnimObjs[i][j]);
                }
            }
            OS_Free(mAnimObjs[i]);
        }
        mAnimObjs[i] = NULL;
        mAnimObjHandles[i] = NULL;
        mSet    = 0;
    }
}

void ModelAnim::FreeAnimData() {
    ModelAnimData *data = mAnimData;
    if (data == NULL) {
        return;
    }
    delete data;
    mAnimData = NULL;
}

void ModelAnim::func_020299ec() {
    NNSG3dAnmObj *obj = GetAnimObj(0);
    if (obj == NULL) {
        return;
    }
    for (u8 i = 0; i < ((u8 *) mNitroModel->mResModel)[0x17]; i++) {
        func_02031518(obj, i);
    }
}

NNSG3dAnmObj *ModelAnim::GetAnimObj(u32 channel) {
    return GetAnimObjForSet(channel, mSet);
}

NNSG3dAnmObj *ModelAnim::GetAnimObjForSet(u32 channel, u32 set) {
    if (mAnimObjHandles[channel] == NULL) {
        return NULL;
    }
    u8 id = mAnimObjHandles[channel][set];
    if (id == 0) {
        return NULL;
    }
    u8 index = id - 1;
    if (mAnimObjs[channel][index] == NULL) {
        mAnimObjs[channel][index] =
            (NNSG3dAnmObj *) mAnimData->mAnims[channel]->CreateAnimObj(index, mNitroModel, mNitroModel->mTexture);
    }
    return mAnimObjs[channel][index];
}

void ModelAnim::SelectSet(u32 set) {
    mSet = set;
    mReachedEnd = 0;
    for (u8 channel = 0; channel < 5; channel++) {
        if (mAnimObjHandles[channel] != NULL && mAnimObjHandles[channel][set] != 0) {
            u8 index = mAnimObjHandles[channel][set] - 1;
            if (mAnimObjs[channel][index] == NULL) {
                mAnimObjs[channel][index] =
                    (NNSG3dAnmObj *) mAnimData->mAnims[channel]->CreateAnimObj(index, mNitroModel, mNitroModel->mTexture);
            }
        }
    }
}
