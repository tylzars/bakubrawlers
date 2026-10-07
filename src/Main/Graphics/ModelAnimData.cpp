#include "Graphics/Model.hpp"
#include "Graphics/ModelInternal.hpp"
#include <nitro/os.h>
#include <nitro/mi.h>

void *operator new(size_t size);

void UnkStruct_02029248::func_02029248() {
    mFile     = NULL;
    mOwnsFile = 0;
}

BOOL UnkStruct_02029248::func_02029258(void *file) {
    if (file == NULL) {
        return FALSE;
    }
    if (mFile != NULL) {
        func_020292e4();
    }
    mFile = file;
    return TRUE;
}

void *UnkStruct_02029248::func_02029288(u32 index, UnkStruct_020289c8 *model, UnkStruct_02028308 *texture) {
    void *modelRes = NULL;
    void *texRes   = NULL;
    if (model != NULL) {
        modelRes = model->mUnk_04;
    }
    if (texture != NULL) {
        texRes = texture->mTex;
    }
    void *anm = func_020335e8(mFile, index);
    void *mem = OS_AllocFromHeap(OS_ARENA_MAIN, OS_CURRENT_HEAP_HANDLE, func_02031418(anm, modelRes));
    func_02031478(mem, anm, modelRes, texRes);
    return mem;
}

void UnkStruct_02029248::func_020292e4() {
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
    ModelAnimEntry *file = (ModelAnimEntry *) func_02023504(path, 0, 0, 0, 0, 0);
    mOwnsFile = 1;
    for (u8 i = 0; i < 5; i++) {
        if (file[i].mFile != 0) {
            file[i].mFile = (u32) file + (u32) file[i].mFile;
            file[i].mTable += (u32) file;
        }
    }
    mFile = file;
    func_02029488();
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
    func_02029488();
}

ModelAnimData::~ModelAnimData() {
    for (u8 i = 0; i < 5; i++) {
        UnkStruct_02029248 *anim = mAnims[i];
        if (anim != NULL) {
            anim->func_020292e4();
            delete anim;
        }
    }
    if (mFile != NULL && mOwnsFile) {
        OS_Free(mFile);
    }
}

void ModelAnimData::func_02029488() {
    // note: the original keeps the new object in a callee-saved register instead of using the constructor's
    // returned `this`; calling func_02029248 as a plain method after operator new(8) reproduces that.
    for (u8 i = 0; i < 5; i++) {
        if (mFile[i].mFile != 0) {
            UnkStruct_02029248 *anim = (UnkStruct_02029248 *) operator new(8);
            if (anim != NULL) {
                anim->func_02029248();
            }
            mAnims[i]                = anim;
            anim->func_02029258((void *) mFile[i].mFile);
        }
    }
}
