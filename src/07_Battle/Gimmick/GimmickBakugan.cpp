#include "Battle/GimmickTypes.hpp"
#include "Game/UnkStruct_020d433c.hpp"
#include "System/FileSystem.hpp"

static ModelResource *sResources[2];

GimmickBakugan::GimmickBakugan(u8 a, u8 variant)
    : Gimmick(a, 0) {
    mAnimFrames = 0;
    mAnimTimer  = 0;
    mState      = 0;
    mUnk_3a     = 8;
    mVariant    = variant;
}

GimmickBakugan::~GimmickBakugan() {
    Unload();
}

void GimmickBakugan::Load() {
    switch (data_020d433c.mUnk_95) {
    case 1:
        if (!sResources[0]) {
            sResources[0] = new ModelResource("pyrus_dorg00.nsbmd", "pyrus_dorg00_data.bin", "pyrus_dorg00.nsbtx", 1);
        }
        mVariant = 0;
        break;
    case 2:
        switch (mVariant) {
        case 0:
            if (!sResources[0]) {
                sResources[0] = new ModelResource("angelo.nsbmd", "angelo_data.bin", "angelo.nsbtx", 1);
            }
            break;
        case 1:
            if (!sResources[1]) {
                sResources[1] = new ModelResource("diablo.nsbmd", "diablo_data.bin", "diablo.nsbtx", 1);
            }
            break;
        default:
            return;
        }
        break;
    case 3:
        if (!sResources[0]) {
            sResources[0] = new ModelResource("Ventus_skyr_0.nsbmd", "Ventus_skyr_0_data.bin", "Ventus_skyr_0.nsbtx", 1);
        }
        mVariant = 0;
        break;
    case 4:
        if (!sResources[0]) {
            sResources[0] = new ModelResource("subterra_majin.nsbmd", "subterra_majin_data.bin", "subterra_majin.nsbtx", 1);
        }
        mVariant    = 0;
        mAnimFrames = 0x2bc;
        break;
    case 5:
        if (!sResources[0]) {
            sResources[0] = new ModelResource("haos_tig.nsbmd", "haos_tig_data.bin", "haos_tig.nsbtx", 1);
        }
        mVariant    = 0;
        mAnimFrames = 0x2bc;
        break;
    case 6:
        if (!sResources[0]) {
            sResources[0] = new ModelResource("hydra.nsbmd", 0, "hydra.nsbtx", 1);
        }
        mVariant = 0;
        break;
    default:
        return;
    }
    ChangeDir("/Game/Gimmick");
    for (u8 i = 0; i < 2; i++) {
        if (sResources[i]) {
            sResources[i]->AddRef(1);
        }
    }
    if (sResources[mVariant]->mAnimData) {
        mModel = sResources[mVariant]->mModel->CreateInstance(new ModelAnim(sResources[mVariant]->mAnimData, sResources[mVariant]->mModel));
        mModel->StartAnimWithIncrement(0, 1, 0x1000);
        mAnimEnabled = 1;
    } else {
        mModel = sResources[mVariant]->mModel->CreateInstance(0);
    }
    if (data_020d433c.mUnk_95 == 5) {
        mModel->mUseBoxTest = 0;
    }
    ChangeDir("/");
}

void GimmickBakugan::Unload() {
    ModelInstance *model = mModel;
    u8 i                 = 0;
    if (model) {
        model->mOwnFlags = 1;
        delete model;
        mModel = 0;
    }
    ModelResource **res = sResources;
    for (; i < 2; i++) {
        if (res[i] && res[i]->ReleaseRef()) {
            ModelResource *r = res[i];
            delete r;
            res[i] = 0;
        }
    }
}

void GimmickBakugan::Finish() {
    mState = 3;
}

void GimmickBakugan::Update() {
    switch (mState) {
    case 0:
        mState++;
        break;
    case 1: {
        if (!mModel) {
            return;
        }
        func_020526bc();
        ModelAnim *anim;
        if (mAnimFrames && (anim = mModel->mAnim) && anim->mReachedEnd) {
            mState++;
            anim->SetFrame(0, 5);
            anim->mPaused = 1;
            mAnimTimer    = mAnimFrames;
        }
        break;
    }
    case 2: {
        if (!mModel) {
            return;
        }
        func_020526bc();
        if (--mAnimTimer != 0) {
            return;
        }
        mState = 1;
        ModelAnim *anim = mModel->mAnim;
        if (anim) {
            anim->mPaused = 0;
            anim->mReachedEnd = 0;
        }
        break;
    }
    case 3:
        mFinished = 1;
        break;
    }
}
