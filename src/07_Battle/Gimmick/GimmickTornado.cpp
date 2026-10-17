#include "Battle/GimmickTypes.hpp"
#include "System/FileSystem.hpp"

GimmickTornado::GimmickTornado(u8 id, u8 variant)
    : Gimmick(id, 0) {
    mAngle  = 0;
    mState  = 0;
    mUnk_3a = 0x32;
    mUnk_3b = variant;
}

GimmickTornado::~GimmickTornado() {
    Unload();
}

void GimmickTornado::Finish() {
    mState = 2;
}

void GimmickTornado::Load() {
    if (mModel) {
        return;
    }
    ChangeDir("/Game/Gimmick");
    LoadFiles("vne_tornado.nsbtx", "vne_tornado.nsbmd", "vne_tornado_data.bin", 1);
    ChangeDir("/");
}

void GimmickTornado::Unload() {
    if (mModel) {
        ModelInstance *m = mModel;
        m->mOwnFlags       = 0xf;
        delete m;
        mModel = 0;
    }
}

void GimmickTornado::Update() {
    VecFx32 in;
    VecFx32 out;
    switch (mState) {
    case 0:
        func_ov007_0222ea28(0x33, 8);
        mState++;
        break;
    case 1:
        data_020d433c.mUnk_14 &= 8;
        if (!data_020d433c.mUnk_14) {
            switch (mUnk_3b) {
            case 0:
                break;
            case 1:
                mAngle -= 0x5b;
                in.x = 0x66000;
                in.y = 0;
                in.z = 0;
                func_ov007_02233ed8(&in, mAngle, &out);
                mPosition.x = out.x;
                mPosition.y = out.y;
                mPosition.z = out.z;
                break;
            case 2:
                break;
            case 3:
                break;
            }
            mUnk_3c->mUnk_810 = mPosition;
        }
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
