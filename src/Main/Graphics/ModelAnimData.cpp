#include "Graphics/Model.hpp"
#include "Graphics/ModelInternal.hpp"
#include <nitro/os.h>
#include <nitro/mi.h>

void *operator new(size_t size);

void ModelAnimFile::Init() {
    mFile     = NULL;
    mOwnsFile = 0;
}

BOOL ModelAnimFile::SetFile(void *file) {
    if (file == NULL) {
        return FALSE;
    }
    if (mFile != NULL) {
        Release();
    }
    mFile = file;
    return TRUE;
}

void *ModelAnimFile::CreateAnimObj(u32 index, ModelFile *model, TextureFile *texture) {
    void *modelRes = NULL;
    void *texRes   = NULL;
    if (model != NULL) {
        modelRes = model->mResModel;
    }
    if (texture != NULL) {
        texRes = texture->mTex;
    }
    void *anm = GetFirstBlockEntryTarget(mFile, index);
    void *mem = OS_AllocFromHeap(OS_ARENA_MAIN, OS_CURRENT_HEAP_HANDLE, GetAnmObjSize(anm, modelRes));
    InitAnmObj(mem, anm, modelRes, texRes);
    return mem;
}

void ModelAnimFile::Release() {
    if (mFile == NULL) {
        return;
    }
    if (mOwnsFile) {
        OS_Free(mFile);
        mOwnsFile = 0;
    }
    mFile = NULL;
}

ModelAnimData::ModelAnimData(const char *path) {
    MI_CpuClear32(mAnims, 0x14);
    ModelAnimEntry *file = (ModelAnimEntry *) LoadFile(path, 0, 0, 0, 0, 0);
    mOwnsFile = 1;
    for (u8 i = 0; i < 5; i++) {
        if (file[i].mFile != 0) {
            file[i].mFile = (u32) file + (u32) file[i].mFile;
            file[i].mTable += (u32) file;
        }
    }
    mFile = file;
    CreateAnimFiles();
}

ModelAnimData::ModelAnimData(void *data) {
    MI_CpuClear32(mAnims, 0x14);
    mOwnsFile = 0;
    ModelAnimEntry *file = (ModelAnimEntry *) data;
    for (u8 i = 0; i < 5; i++) {
        if (file[i].mFile != 0) {
            file[i].mFile = (u32) file + (u32) file[i].mFile;
            file[i].mTable += (u32) file;
        }
    }
    mFile = file;
    CreateAnimFiles();
}

ModelAnimData::~ModelAnimData() {
    for (u8 i = 0; i < 5; i++) {
        ModelAnimFile *anim = mAnims[i];
        if (anim != NULL) {
            anim->Release();
            delete anim;
        }
    }
    if (mFile != NULL && mOwnsFile) {
        OS_Free(mFile);
    }
}

void ModelAnimData::CreateAnimFiles() {
    // note: the original keeps the new object in a callee-saved register instead of using the constructor's
    // returned `this`; calling Init as a plain method after operator new(8) reproduces that.
    for (u8 i = 0; i < 5; i++) {
        if (mFile[i].mFile != 0) {
            ModelAnimFile *anim = (ModelAnimFile *) operator new(8);
            if (anim != NULL) {
                anim->Init();
            }
            mAnims[i]                = anim;
            anim->SetFile((void *) mFile[i].mFile);
        }
    }
}
