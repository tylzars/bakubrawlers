#include "Graphics/Model.hpp"
#include "Graphics/ModelInternal.hpp"
#include <nitro/os.h>

void *operator new(size_t size);


static inline u32 *GetResData(NNSG3dResMdlSet *set, u32 index) {
    NNSG3dResDict *dict = &set->mDict;
    if (dict != NULL && index < set->mDict.mCount) {
        return (u32 *) ((u8 *) dict + set->mDict.mTableOfs + 4 + index * 4);
    }
    return NULL;
}

static inline void *GetModelByIdx(NNSG3dResMdlSet *set, u32 index) {
    if (set != NULL) {
        u32 *data = GetResData(set, index);
        if (data != NULL) {
            return (u8 *) set + *data;
        }
    }
    return NULL;
}

UnkStruct_020289c8::UnkStruct_020289c8() {
    mFile     = NULL;
    mUnk_04   = NULL;
    mUnk_08   = NULL;
    mOwnsFile = 0;
}

BOOL UnkStruct_020289c8::func_020289e0(const char *name, UnkStruct_02028308 *texture) {
    void *file = func_02023504(name, 0, 0, 0, 0, 0);
    mOwnsFile  = 1;
    return func_02028a2c(file, texture);
}

BOOL UnkStruct_020289c8::func_02028a2c(void *file, UnkStruct_02028308 *texture) {
    if (file == NULL) {
        return FALSE;
    }
    if (mFile != NULL) {
        func_02028af0();
    }
    mFile = file;
    mUnk_04 = GetModelByIdx(func_02033598(file), 0);
    if (texture != NULL) {
        func_02028b4c(texture);
    }
    return TRUE;
}

BOOL UnkStruct_020289c8::func_02028ac4(const char *name) {
    if (!func_020289e0(name, NULL)) {
        return FALSE;
    }
    func_02032d7c(mFile);
    return TRUE;
}

void UnkStruct_020289c8::func_02028af0() {
    if (mFile == NULL) {
        return;
    }
    func_0203209c(func_02033598(mFile));
    func_02032fb4(mFile);
    if (mOwnsFile) {
        OS_Free(mFile);
        mOwnsFile = 0;
    }
    mFile   = NULL;
    mUnk_04 = NULL;
    mUnk_08 = NULL;
}

void UnkStruct_020289c8::func_02028b4c(UnkStruct_02028308 *texture) {
    if (texture == NULL || texture->mTex == NULL) {
        return;
    }
    mUnk_08 = texture;
    func_02031fec(func_02033598(mFile), texture->mTex);
}

ModelInstance *UnkStruct_020289c8::func_02028b78(ModelAnim *anim) {
    ModelInstance *instance = new ModelInstance(this);
    instance->func_02028de0(anim);
    return instance;
}
