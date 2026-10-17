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

ModelFile::ModelFile() {
    mFile     = NULL;
    mResModel   = NULL;
    mTexture   = NULL;
    mOwnsFile = 0;
}

BOOL ModelFile::Load(const char *name, TextureFile *texture) {
    void *file = LoadFile(name, 0, 0, 0, 0, 0);
    mOwnsFile  = 1;
    return LoadData(file, texture);
}

BOOL ModelFile::LoadData(void *file, TextureFile *texture) {
    if (file == NULL) {
        return FALSE;
    }
    if (mFile != NULL) {
        Release();
    }
    mFile = file;
    mResModel = GetModelByIdx(func_02033598(file), 0);
    if (texture != NULL) {
        BindTexture(texture);
    }
    return TRUE;
}

BOOL ModelFile::func_02028ac4(const char *name) {
    if (!Load(name, NULL)) {
        return FALSE;
    }
    func_02032d7c(mFile);
    return TRUE;
}

void ModelFile::Release() {
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
    mResModel = NULL;
    mTexture = NULL;
}

void ModelFile::BindTexture(TextureFile *texture) {
    if (texture == NULL || texture->mTex == NULL) {
        return;
    }
    mTexture = texture;
    func_02031fec(func_02033598(mFile), texture->mTex);
}

ModelInstance *ModelFile::CreateInstance(ModelAnim *anim) {
    ModelInstance *instance = new ModelInstance(this);
    instance->SetAnim(anim);
    return instance;
}
