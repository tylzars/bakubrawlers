#include "System/SoundManager.hpp"
#include "Battle/GimmickTypes.hpp"
#include <nitro/fx.h>
#include "Game/Random.hpp"
#include "Game/UnkStruct_020d433c.hpp"
#include "Graphics/Camera.hpp"
#include "System/FileSystem.hpp"

void RemoveCollisionSet(GimmickCollision *data, GimmickSetData *set);
void LoadCollisionFile(void *buf, u8 id);
GimmickSetData *AddCollisionSet(GimmickCollision *data, int count, void *buf, VecFx32 *pos, u16 *rot, u8 unk);

static const u8 sDestroyTurnLimits[4]  = {3, 4, 5, 0};
static const u16 data_ov007_022793d4[3] = {30, 50, 50};
static const u8 sKindStepCounts[4]  = {1, 3, 5, 0};
static const u16 data_ov007_022793da[3] = {0, 0x1555, 0};
static const u16 data_ov007_022793e6[3] = {30, 50, 0};
static ModelResource *sResources[3];
static const u16 sKindStepIntervals[3] = {10, 10, 6};

extern MtxFx43 data_020c1cf4;
extern MtxFx43 gIdentityMtx43;


GimmickFieldDestroy::GimmickFieldDestroy(u8 a)
    : Gimmick(a, 0) {
    mModelB = 0;
    mModelE = 0;
    mState  = 0;
    mType = GIMMICK_TYPE_FIELD_DESTROY;
    mModelB = 0;
    mModelE = 0;
    mKind   = 0;
    mStep   = 0;
    mSequenceFrames  = 0;
}

GimmickFieldDestroy::~GimmickFieldDestroy() {
    Unload();
}

void GimmickFieldDestroy::func_ov007_0222accc() {
    char buf[0x804];
    LoadCollisionFile(buf, 0x32);
    mCollision = AddCollisionSet(data_020d433c.mGimmickCollision, 0x20, buf, &mPosition, &mRotX, mType);
    VecFx32 v = {0x1000, 0x1000, 0x1000};
    mScale    = v;
}

void GimmickFieldDestroy::Load() {
    if (!sResources[2]) {
        char nsbmd[0x40];
        char nsbtx[0x40];
        char data[0x40];
        u8 n = data_020d433c.mFieldId;
        sprintf(nsbmd, "fielddestroy_n_%d.nsbmd", n);
        sprintf(data, "fielddestroy_n_%d_data.bin", n);
        sprintf(nsbtx, "fielddestroy_n_%d.nsbtx", n);
        sResources[2] = new ModelResource(nsbmd, data, nsbtx, 1);
        sprintf(nsbmd, "fielddestroy_b_%d.nsbmd", n);
        sprintf(data, "fielddestroy_b_%d_data.bin", n);
        sResources[0] = new ModelResource(nsbmd, data, 0, 0);
        sResources[1] = new ModelResource("fielddestroy_e_0.nsbmd", "fielddestroy_e_0_data.bin", "fielddestroy_e_0.nsbtx", 1);
    }
    ChangeDir("/Game/Gimmick");
    sResources[2]->AddRef(0);
    mModel  = sResources[2]->mModel->CreateInstance(new ModelAnim(sResources[2]->mAnimData, sResources[2]->mModel));
    mModel->StartAnimWithIncrement(0, 1, 0x1000);
    mAnimEnabled = 1;
    sResources[0]->AddRef(1);
    if (!sResources[0]->mModel->mTexture) {
        sResources[0]->mModel->BindTexture(sResources[2]->mTexture);
        sResources[2]->mTexture->FreeFile();
    }
    mModelB = sResources[0]->mModel->CreateInstance(new ModelAnim(sResources[0]->mAnimData, sResources[0]->mModel));
    mModelB->StartAnimWithIncrement(0, 0, 0x1000);
    mModelB->mUseBoxTest = 0;
    sResources[1]->AddRef(1);
    mModelE = sResources[1]->mModel->CreateInstance(new ModelAnim(sResources[1]->mAnimData, sResources[1]->mModel));
    mModelE->StartAnimWithIncrement(0, 0, 0x1000);
    mModelE->mUseBoxTest = 0;
    ChangeDir("/");
}

void GimmickFieldDestroy::Unload() {
    if (mState == 2) {
        mState++;
    }
    if (mModel) {
        ModelInstance *m = mModelE;
        m->mOwnFlags       = 1;
        delete m;
        mModelE = 0;
        m               = mModelB;
        m->mOwnFlags      = 1;
        delete m;
        mModelB = 0;
        m               = mModel;
        m->mOwnFlags      = 1;
        delete m;
        mModel = 0;
    }
    if (sResources[2]) {
        sResources[0]->ReleaseRef();
        sResources[1]->ReleaseRef();
        if (sResources[2]->ReleaseRef()) {
            ModelResource *r = sResources[2];
            delete r;
            sResources[2] = 0;
            r             = sResources[0];
            delete r;
            sResources[0] = 0;
            r             = sResources[1];
            delete r;
            sResources[1] = 0;
        }
    }
}

void GimmickFieldDestroy::Finish() {
    mState = 5;
}

void GimmickFieldDestroy::Update() {
    switch (mState) {
    case 0:
        func_ov007_0222accc();
        mState++;
        break;
    case 1: {
        if ((int)data_020d433c.mGimmicks->mState >= 2) {
            DrawInstance();
        }
        GimmickSetData *set = mCollision;
        if (set && set->mHitCount) {
            RemoveCollisionSet(data_020d433c.mGimmickCollision, set);
            mCollision = 0;
            func_ov007_0222b8b4();
            mSequenceFrames++;
            SoundManager::Get().StartSeqArc(4);
            mState++;
        }
        break;
    }
    case 2:
        if ((int)data_020d433c.mGimmicks->mState >= 2) {
            func_ov007_0222bc24();
        }
        break;
    case 3:
        mCountingTurns = 1;
        mState++;
        break;
    case 4: {
        u8 players = data_020d433c.mPlayerCount;
        if (players < 2 || players > 4) {
            break;
        }
        u32 limit;
        if (data_020d433c.mHasPartners) {
            limit = 3;
        } else {
            limit = sDestroyTurnLimits[players - 2];
        }
        if (mTurnCount < limit) {
            break;
        }
        func_ov007_0222accc();
        mCountingTurns = 0;
        mState  = 1;
        mTurnCount = 0;
        break;
    }
    case 5:
        mFinished = 1;
        break;
    }
    UpdateKindSequence();
}

u8 GimmickFieldDestroy::PickKind() {
    u8 flag   = data_020d433c.mUnk_7a;
    u32 total = 0;
    u8 i;
    for (i = 0; i < 3; i++) {
        if (flag) {
            total += data_ov007_022793d4[i];
        } else {
            total += data_ov007_022793e6[i];
        }
    }
    u32 r = MATH_Rand32(&gRandom, total + 1);
    i     = 0;
    u32 acc = i;
    for (; i < 3; i++) {
        if (flag) {
            acc += data_ov007_022793d4[i];
            if (r <= acc) {
                goto found;
            }
        } else {
            acc += data_ov007_022793e6[i];
            if (r <= acc) {
                goto found;
            }
        }
    }
    return 0;
found:
    return i;
}

void GimmickFieldDestroy::UpdateKindSequence() {
    if (mSequenceFrames) {
        BattlePlayer *p;
        u8 kind = mKind;
        if ((mSequenceFrames - 1) % sKindStepIntervals[kind] == 0) {
            BattlePlayerList *plist = data_020d433c.mBattlePlayers;
            p                       = plist->mPlayers[plist->mTurnPlayer];
            switch (kind) {
            case 0:
                SoundManager::Get().StartSeqArc(0x34);
                break;
            case 1: {
                SoundManager::Get().StartSeqArc(0x35);
                func_ov007_0226a380(p, p->mRecord, 0x1e);
                CameraManager::Get();
                MtxFx43 mtx    = data_020c1cf4;
                VecFx32 camPos = data_020d433c.mBattlePlayers->mBattleTarget->mBakugan->mPosition;
                VecFx32 v      = {0, 0xf000, -0xa000};
                u8 handle      = StartEffect(data_020d433c.mEffects, 1, 3, 0, 0);
                if (handle) {
                    mtx._[11] = 0;
                    mtx._[10] = 0;
                    mtx._[9]  = 0;
                    MTX_MultVec43(&v, &mtx, &v);
                    VEC_Add(&camPos, &v, &camPos);
                    MTX_SetTranslation43(&mtx, &camPos);
                    SetEffectMatrix(data_020d433c.mEffects, handle, &mtx);
                }
                break;
            }
            case 2: {
                SoundManager::Get().StartSeqArc(0x36);
                u8 handle = StartEffect(data_020d433c.mEffects, 1, 0xe, 0, 1);
                BattleRecord *rec = &p->mRecords[p->mRecord];
                if ((int)rec->mGauge < 0xfd7) {
                    rec->mGauge += 0x28;
                }
                if (handle) {
                    VecFx32 d;
                    VecFx32 camPos = data_020d433c.mBattlePlayers->mBattleTarget->mBakugan->mPosition;
                    CameraManager::Get();
                    MtxFx43 mtx = data_020c1cf4;
                    MTX_SetTranslation43(&mtx, &mPosition);
                    Camera *h = data_020d433c.mBattlePlayers->mBattleTarget->mCamera;
                    d.x                 = h->mPosition.x;
                    d.y                 = h->mPosition.y;
                    d.z                 = h->mPosition.z;
                    VEC_Subtract(&d, &camPos, &d);
                    VEC_Normalize(&d, &d);
                    d.x = FX_Mul(d.x, 0x1e000);
                    d.y = FX_Mul(d.y, 0x1e000);
                    d.z = FX_Mul(d.z, 0x1e000);
                    VEC_Add(&d, &data_020d433c.mBattlePlayers->mBattleTarget->mBakugan->mPosition, &d);
                    MTX_SetTranslation43(&mtx, &d);
                    SetEffectMatrix(data_020d433c.mEffects, handle, &mtx);
                }
                break;
            }
            }
            mStep++;
            if (mStep >= sKindStepCounts[mKind]) {
                mStep  = 0;
                mSequenceFrames = 0;
                return;
            }
        }
    }
    if (mSequenceFrames) {
        mSequenceFrames++;
    }
}

void GimmickFieldDestroy::func_ov007_0222b8b4() {
    BattlePlayerList *plist = data_020d433c.mBattlePlayers;
    BattlePlayer *p         = plist->mPlayers[plist->mTurnPlayer];
    mKind                   = PickKind();
    switch (mKind) {
    case 0: {
        int v = p->mUnk_f3 + 0xbe;
        if (v >= 0xbe) {
            v = 0xbe;
        }
        p->mUnk_f3 = v;
        u8 handle  = StartEffect(data_020d433c.mEffects, 1, 0xc, 0, 1);
        if (!handle) {
            return;
        }
        CameraManager::Get();
        MtxFx43 mtx2           = data_020c1cf4;
        BattlePlayerList *list = data_020d433c.mBattlePlayers;
        MtxFx43 mtx            = gIdentityMtx43;
        VecFx32 camPos         = list->mBattleTarget->mBakugan->mPosition;
        VecFx32 v2             = {0, 0, 0x1e000};
        mtx2._[11]             = 0;
        mtx2._[10]             = 0;
        mtx2._[9]              = 0;
        MTX_MultVec43(&v2, &mtx2, &v2);
        VEC_Add(&camPos, &v2, &camPos);
        MTX_ApplyRotX43(&mtx, data_ov007_022793da[1]);
        MTX_Concat43(&mtx, &mtx2, &mtx2);
        MTX_SetTranslation43(&mtx2, &camPos);
        SetEffectMatrix(data_020d433c.mEffects, handle, &mtx2);
        break;
    }
    case 1: {
        u8 handle = StartEffect(data_020d433c.mEffects, 1, 0xd, 0, 1);
        if (!handle) {
            return;
        }
        VecFx32 d;
        VecFx32 camPos = data_020d433c.mBattlePlayers->mBattleTarget->mBakugan->mPosition;
        CameraManager::Get();
        MtxFx43 mtx = data_020c1cf4;
        MTX_SetTranslation43(&mtx, &mPosition);
        Camera *h = data_020d433c.mBattlePlayers->mBattleTarget->mCamera;
        d.x                 = h->mPosition.x;
        d.y                 = h->mPosition.y;
        d.z                 = h->mPosition.z;
        VEC_Subtract(&d, &camPos, &d);
        VEC_Normalize(&d, &d);
        d.x = FX_Mul(d.x, 0x1e000);
        d.y = FX_Mul(d.y, 0x1e000);
        d.z = FX_Mul(d.z, 0x1e000);
        VEC_Add(&d, &data_020d433c.mBattlePlayers->mBattleTarget->mBakugan->mPosition, &d);
        MTX_SetTranslation43(&mtx, &d);
        SetEffectMatrix(data_020d433c.mEffects, handle, &mtx);
        break;
    }
    case 2:
        break;
    }
}

int GimmickFieldDestroy::func_ov007_0222bc24() {
    if (!mModelB) {
        return 0;
    }
    if (!mModelB->AdvanceAnim()) {
        mState++;
    }
    CameraManager::Get();
    MtxFx43 mtx = data_020c1cf4;
    MTX_SetTranslation43(&mtx, &mPosition);
    mModelB->Draw(&mtx, &mScale);
    VecFx32 dir;
    VecFx32 cp;
    Camera *cam = CameraManager::Get().mCameras[0];
    cp          = cam->mPosition;
    cp          = cam->mTarget;
    VEC_Subtract(&dir, &cp, &dir);
    VEC_Normalize(&dir, &dir);
    dir.x = 0;
    dir.y = 0;
    dir.z = 0;
    VEC_Add(&dir, &mPosition, &dir);
    MTX_SetTranslation43(&mtx, &dir);
    mModelE->AdvanceAnim();
    mModelE->Draw(&mtx, &mScale);
}

