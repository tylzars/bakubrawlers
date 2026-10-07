#include "Graphics/Model.hpp"
#include "Graphics/ModelInternal.hpp"
#include <nitro/os.h>

void operator delete(void *ptr);

ModelAnim::ModelAnim(const void *data, NitroModel *model) {
    func_02029594();
    mUnk_14 = model;
    mUnk_18 = (ModelAnimData *) data;
    for (u8 i = 0; i < 5; i++) {
        ModelAnimData *d = (ModelAnimData *) data;
        func_0202964c(i, d->mAnims[i], (u8 *) d->mFile[i].mTable);
    }
}

ModelAnim::~ModelAnim() {
    func_02029914();
}

void ModelAnim::func_02029594() {
    for (u8 i = 0; i < 5; i++) {
        mUnk_1c[i] = NULL;
        mUnk_30[i] = NULL;
        mUnk_44[i] = NULL;
        mUnk_5d    = 0;
    }
    mUnk_04 = 0;
    mUnk_08 = 1;
    mUnk_0c = 0x1000;
    mUnk_14 = NULL;
    mUnk_18 = NULL;
}

BOOL ModelAnim::func_020295e0() {
    if (mUnk_04 != 0) {
        return TRUE;
    }
    for (u8 i = 0; i < 5; i++) {
        NNSG3dAnmObj *obj = func_02029a48(i);
        if (obj != NULL) {
            func_0202982c(obj);
        }
    }
    return !((mUnk_08 == 0) & mUnk_10);
}

void ModelAnim::func_0202964c(u32 channel, UnkStruct_02029248 *anim, u8 *table) {
    if (anim == NULL || table == NULL) {
        return;
    }
    mUnk_1c[channel] = anim;
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
    mUnk_58[channel] = count;
    mUnk_44[channel] = table;
    mUnk_30[channel] = (NNSG3dAnmObj **) OS_Alloc(count * 4);
    u8 i = 0;
    do {
        mUnk_30[channel][i] = NULL;
        i++;
    } while (i < count);
}

void ModelAnim::func_02029734() {
    mUnk_10 = 0;
    for (u8 channel = 0; channel < 5; channel++) {
        NNSG3dAnmObj *obj = func_02029a48(channel);
        if (obj == NULL) {
            continue;
        }
        obj->mFrame = 0;
        switch (channel) {
        case 0:
            func_020299ec();
            break;
        case 2:
            for (u8 i = 0; i < ((u8 *) mUnk_14->mUnk_04)[0x18]; i++) {
                func_02031518(obj, i);
            }
            break;
        }
    }
}

s32 ModelAnim::func_020297d4(u32 set) {
    s32 max = 0;
    for (u8 channel = 0; channel < 5; channel++) {
        NNSG3dAnmObj *obj = func_02029a58(channel, set);
        if (obj != NULL) {
            s32 end = obj->mRes->mFrames << 12;
            if (max < end) {
                max = end;
            }
        }
    }
    return max;
}

void ModelAnim::func_0202982c(NNSG3dAnmObj *obj) {
    mUnk_10 = (func_02029854(obj) != 0 || mUnk_10 != 0);
}

BOOL ModelAnim::func_02029854(NNSG3dAnmObj *obj) {
    s32 frame = obj->mFrame + mUnk_0c;
    BOOL done = FALSE;
    obj->mFrame = frame;
    s32 end = obj->mRes->mFrames << 12;
    if (frame >= end) {
        done = TRUE;
        if (mUnk_08 != 0) {
            obj->mFrame = frame - end;
        } else {
            obj->mFrame = end - 1;
        }
    } else if (frame < 0) {
        if (mUnk_08 != 0) {
            obj->mFrame = frame + end;
        } else {
            obj->mFrame = done;
        }
    }
    return done;
}

void ModelAnim::func_020298c0(u32 value, u32 index) {
    if (index == 5) {
        for (u8 i = 0; i < 5; i++) {
            NNSG3dAnmObj *obj = func_02029a48(i);
            if (obj != NULL) {
                obj->mFrame = value;
            }
        }
    } else {
        NNSG3dAnmObj *obj = func_02029a48(index);
        if (obj != NULL) {
            obj->mFrame = value;
        }
    }
}

void ModelAnim::func_02029914() {
    for (u8 i = 0; i < 5; i++) {
        if (mUnk_30[i] != NULL) {
            for (u8 j = 0; j < mUnk_58[i]; j++) {
                if (mUnk_30[i][j] != NULL) {
                    OS_Free(mUnk_30[i][j]);
                }
            }
            OS_Free(mUnk_30[i]);
        }
        mUnk_30[i] = NULL;
        mUnk_44[i] = NULL;
        mUnk_5d    = 0;
    }
}

void ModelAnim::func_020299b8() {
    ModelAnimData *data = mUnk_18;
    if (data == NULL) {
        return;
    }
    delete data;
    mUnk_18 = NULL;
}

void ModelAnim::func_020299ec() {
    NNSG3dAnmObj *obj = func_02029a48(0);
    if (obj == NULL) {
        return;
    }
    for (u8 i = 0; i < ((u8 *) mUnk_14->mUnk_04)[0x17]; i++) {
        func_02031518(obj, i);
    }
}

NNSG3dAnmObj *ModelAnim::func_02029a48(u32 channel) {
    return func_02029a58(channel, mUnk_5d);
}

NNSG3dAnmObj *ModelAnim::func_02029a58(u32 channel, u32 set) {
    if (mUnk_44[channel] == NULL) {
        return NULL;
    }
    u8 id = mUnk_44[channel][set];
    if (id == 0) {
        return NULL;
    }
    u8 index = id - 1;
    if (mUnk_30[channel][index] == NULL) {
        mUnk_30[channel][index] =
            (NNSG3dAnmObj *) mUnk_18->mAnims[channel]->func_02029288(index, mUnk_14, mUnk_14->mUnk_08);
    }
    return mUnk_30[channel][index];
}

void ModelAnim::func_02029ad8(u32 set) {
    mUnk_5d = set;
    mUnk_10 = 0;
    for (u8 channel = 0; channel < 5; channel++) {
        if (mUnk_44[channel] != NULL && mUnk_44[channel][set] != 0) {
            u8 index = mUnk_44[channel][set] - 1;
            if (mUnk_30[channel][index] == NULL) {
                mUnk_30[channel][index] =
                    (NNSG3dAnmObj *) mUnk_18->mAnims[channel]->func_02029288(index, mUnk_14, mUnk_14->mUnk_08);
            }
        }
    }
}
