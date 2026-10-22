#include "Battle/GimmickTypes.hpp"
#include "Game/Random.hpp"
#include "System/FileSystem.hpp"
#include "System/SoundManager.hpp"

extern "C" void MTX_ApplyRotY33(MtxFx33 *m, u16 angle);
extern "C" void MTX_ApplyRotX33(MtxFx33 *m, u16 angle);
extern "C" void MTX_ApplyRotZ33(MtxFx33 *m, u16 angle);

static ModelResource *sResource;

GimmickJumpObj::GimmickJumpObj(u8 a)
    : Gimmick(a, 0) {
    mTimer  = 0;
    mState  = 0;
    mType = GIMMICK_TYPE_JUMP_OBJ;
    mDir.x  = 0;
    mDir.y  = 0x3000;
    mDir.z  = 0;
}

GimmickJumpObj::~GimmickJumpObj() {
    Unload();
}

void GimmickJumpObj::Init() {
    char buf[0x804];
    LoadCollisionFile(buf, 0x5d);
    mCollision = AddCollisionSet(gGameData.mGimmickCollision, COLLISION_GROUP_GIMMICK_HIT_BY_SP5, buf, &mPosition, &mRotX, mType);
    RotateDir(0);
}

void GimmickJumpObj::Load() {
    if (!sResource) {
        sResource = new ModelResource("jumpobj_00.nsbmd", "jumpobj_00_data.bin", "jumpobj_00.nsbtx", 1);
    }
    ChangeDir("/Game/Gimmick");
    sResource->AddRef(1);
    mModel = sResource->mModel->CreateInstance(new ModelAnim(sResource->mAnimData, sResource->mModel));
    mModel->StartAnimWithIncrement(0, 1, 0x1000);
    mAnimEnabled = 1;
    ChangeDir("/");
}

void GimmickJumpObj::Unload() {
    if (mModel) {
        ModelInstance *m = mModel;
        m->mOwnFlags       = MODEL_OWN_ANIM;
        delete m;
        mModel = 0;
    }
    if (sResource && sResource->ReleaseRef()) {
        ModelResource *r = sResource;
        delete r;
        sResource = 0;
    }
}

void GimmickJumpObj::Finish() {
    mState = 3;
}

void GimmickJumpObj::Update() {
    MtxFx33 m = gIdentityMtx33;
    switch (mState) {
    case 0:
        Init();
        mState++;
        break;
    case 1: {
        if (mModel && (int)gGameData.mGimmicks->mState >= 2) {
            if (mTimer == 0) {
                DrawInstance();
            } else {
                gGameData.mFlags &= 8;
                if (gGameData.mFlags == 0) {
                    mTimer--;
                }
            }
        }
        if (!(mCollision && mCollision->mHitCount)) {
            return;
        }
        BattleTarget *target = gGameData.mBattlePlayers->mBattleTarget;
        if (!target) {
            return;
        }
        if (target->mResultFrames) {
            return;
        }
        RotateDir(1);
        u32 r = MATH_Rand32(&gRandom, 0x444);
        MTX_ApplyRotY33(&m, r - 0x222);
        MTX_MultVec33(&mDir, &m, &mDir);
        SetTargetVelocity(gGameData.mBattlePlayers->mBattleTarget, &mDir, 1);
        func_ov007_0225ca18(gGameData.mBattlePlayers->mBattleTarget, 4, 0xb4);
        mTimer = 0x3c;
        SoundManager::Get().StartSeqArc(0x37);
        mState++;
        break;
    }
    case 2:
        mTimer = 0x3c;
        if (!mCollision) {
            return;
        }
        if (mCollision->mHitCount == 0) {
            mState = 1;
        }
        break;
    case 3:
        mFinished = 1;
        break;
    }
}

void GimmickJumpObj::RotateDir(int reset) {
    MtxFx33 m = gIdentityMtx33;
    if (mRotY) {
        MTX_ApplyRotY33(&m, mRotY);
    }
    if (mRotX) {
        MTX_ApplyRotX33(&m, mRotX);
    }
    if (mRotZ) {
        MTX_ApplyRotZ33(&m, mRotZ);
    }
    if (reset) {
        mDir.x = 0;
        mDir.y = 0x3000;
        mDir.z = 0;
    }
    MTX_MultVec33(&mDir, &m, &mDir);
}
