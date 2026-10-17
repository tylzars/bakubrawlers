#include "Battle/GimmickTypes.hpp"
#include "Game/UnkStruct_020d433c.hpp"
#include "System/FileSystem.hpp"

static ModelResource *sResource;

GimmickBlock::GimmickBlock(u8 a)
    : Gimmick(a, 0) {
    mState  = 0;
    mUnk_3a = 6;
}

GimmickBlock::~GimmickBlock() {
    Unload();
}

void GimmickBlock::Load() {
    if (!sResource) {
        char nsbmd[0x40];
        char nsbtx[0x40];
        u8 n = data_020d433c.mFieldId;
        sprintf(nsbmd, "block_%02d.nsbmd", n);
        sprintf(nsbtx, "block_%02d.nsbtx", n);
        sResource = new ModelResource(nsbmd, 0, nsbtx, 1);
    }
    ChangeDir("/Game/Gimmick");
    sResource->AddRef(1);
    if (sResource->mAnimData) {
        mModel = sResource->mModel->CreateInstance(new ModelAnim(sResource->mAnimData, sResource->mModel));
        mModel->StartAnimWithIncrement(0, 1, 0x1000);
        mAnimEnabled = 1;
    } else {
        mModel = sResource->mModel->CreateInstance(0);
    }
    ChangeDir("/");
}

void GimmickBlock::Unload() {
    ModelInstance *model = mModel;
    if (model) {
        model->mOwnFlags = 1;
        delete model;
        mModel = 0;
    }
    if (sResource && sResource->ReleaseRef()) {
        ModelResource *r = sResource;
        delete r;
        sResource = 0;
    }
}

void GimmickBlock::Finish() {
    mState = 2;
}

void GimmickBlock::Update() {
    switch (mState) {
    case 0:
        func_ov007_0222ea28(0x5a, 0x20);
        mState++;
        break;
    case 1:
        if (!mModel) {
            return;
        }
        if ((int)data_020d433c.mGimmicks->mState < 2) {
            return;
        }
        func_020526bc();
        break;
    case 2:
        mFinished = 1;
        break;
    }
}
