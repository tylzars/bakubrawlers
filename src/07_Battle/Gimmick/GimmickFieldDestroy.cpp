#include "Battle/GimmickTypes.hpp"
#include <nitro/fx.h>
#include "Game/Random.hpp"
#include "Game/UnkStruct_020d433c.hpp"
#include "Graphics/Camera.hpp"
#include "System/FileSystem.hpp"
#include "System/SoundManager.hpp"

void func_ov007_02271d78(UnkStruct_020d433c_0c *data, GimmickSetData *set);
void func_ov007_022719c8(void *buf, u8 id);
GimmickSetData *func_ov007_02271ca8(UnkStruct_020d433c_0c *data, int count, void *buf, VecFx32 *pos, u16 *rot, u8 unk);

static const u8 data_ov007_022793cc[4]  = {3, 4, 5, 0};
static const u8 data_ov007_022793d0[4]  = {1, 3, 5, 0};
static const u16 data_ov007_022793d4[6] = {30, 50, 50, 0, 0x1555, 0};
static const u16 data_ov007_022793e0[3] = {10, 10, 6};
static ModelResource *sResources[3];
static const u16 data_ov007_022793e6[3] = {30, 50, 0};
extern VecFx32 data_ov007_022793f8;

extern MtxFx43 data_020c1cf4;
extern MtxFx43 data_0220e86c;
extern VecFx32 data_ov007_02279404;
extern VecFx32 data_ov007_022793ec;


GimmickFieldDestroy::GimmickFieldDestroy(u8 a)
    : Gimmick(a, 0) {
    mModelB = 0;
    mModelE = 0;
    mState  = 0;
    mUnk_3a = 7;
    mModelB = 0;
    mModelE = 0;
    mKind   = 0;
    mStep   = 0;
    mTimer  = 0;
}

GimmickFieldDestroy::~GimmickFieldDestroy() {
    Unload();
}

void GimmickFieldDestroy::func_ov007_0222accc() {
    char buf[0x804];
    func_ov007_022719c8(buf, 0x32);
    mUnk_3c = func_ov007_02271ca8(data_020d433c.mUnk_0c, 0x20, buf, &mPosition, &mRotX, mUnk_3a);
    VecFx32 v = data_ov007_022793f8;
    mScale    = v;
}

void GimmickFieldDestroy::Load() {
    if (!sResources[2]) {
        char nsbmd[0x40];
        char nsbtx[0x40];
        char data[0x40];
        u8 n = data_020d433c.mUnk_95;
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
    sResources[2]->func_020306e4(0);
    mModel  = sResources[2]->mModel->func_02028b78(new ModelAnim(sResources[2]->mAnimData, sResources[2]->mModel));
    mModel->func_02028e54(0, 1, 0x1000);
    mUnk_00 = 1;
    sResources[0]->func_020306e4(1);
    if (!sResources[0]->mModel->mUnk_08) {
        sResources[0]->mModel->func_02028b4c(sResources[2]->mTexture);
        sResources[2]->mTexture->func_02028344();
    }
    mModelB = sResources[0]->mModel->func_02028b78(new ModelAnim(sResources[0]->mAnimData, sResources[0]->mModel));
    mModelB->func_02028e54(0, 0, 0x1000);
    mModelB->mUnk_69 = 0;
    sResources[1]->func_020306e4(1);
    mModelE = sResources[1]->mModel->func_02028b78(new ModelAnim(sResources[1]->mAnimData, sResources[1]->mModel));
    mModelE->func_02028e54(0, 0, 0x1000);
    mModelE->mUnk_69 = 0;
    ChangeDir("/");
}

void GimmickFieldDestroy::Unload() {
    if (mState == 2) {
        mState++;
    }
    if (mModel) {
        ModelInstance *m = mModelE;
        m->mUnk_68       = 1;
        delete m;
        mModelE = 0;
        m               = mModelB;
        m->mUnk_68      = 1;
        delete m;
        mModelB = 0;
        m               = mModel;
        m->mUnk_68      = 1;
        delete m;
        mModel = 0;
    }
    if (sResources[2]) {
        sResources[0]->func_020307e8();
        sResources[1]->func_020307e8();
        if (sResources[2]->func_020307e8()) {
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
            func_020526bc();
        }
        GimmickSetData *set = mUnk_3c;
        if (set && set->mUnk_823) {
            func_ov007_02271d78(data_020d433c.mUnk_0c, set);
            mUnk_3c = 0;
            func_ov007_0222b8b4();
            mTimer++;
            SoundManager::Get().func_020246f8(4);
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
        mUnk_40 = 1;
        mState++;
        break;
    case 4: {
        u8 players = data_020d433c.mPlayerCount;
        if (players < 2 || players > 4) {
            break;
        }
        u32 limit;
        if (data_020d433c.mUnk_98) {
            limit = 3;
        } else {
            limit = data_ov007_022793cc[players - 2];
        }
        if (mUnk_44 < limit) {
            break;
        }
        func_ov007_0222accc();
        mUnk_40 = 0;
        mState  = 1;
        mUnk_44 = 0;
        break;
    }
    case 5:
        mFinished = 1;
        break;
    }
    func_ov007_0222b410();
}

// non-matching: the original loads the d4 table literal before e6, and the branch shape differs (bhi/pop vs popls/b).
u8 GimmickFieldDestroy::func_ov007_0222b328() {
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
    u32 r = MATH_Rand32(&data_020d42e8, total + 1);
    i     = 0;
    u32 acc = i;
    for (; i < 3; i++) {
        if (flag) {
            acc += data_ov007_022793d4[i];
            if (r <= acc) {
                return i;
            }
        } else {
            acc += data_ov007_022793e6[i];
            if (r <= acc) {
                return i;
            }
        }
    }
    return 0;
}

void GimmickFieldDestroy::func_ov007_0222b410() {
    if (mTimer) {
        BattlePlayer *p;
        u8 kind = mKind;
        if ((mTimer - 1) % data_ov007_022793e0[kind] == 0) {
            BattlePlayerList *plist = data_020d433c.mBattlePlayers;
            p                       = plist->mPlayers[plist->mUnk_28a];
            switch (kind) {
            case 0:
                SoundManager::Get().func_020246f8(0x34);
                break;
            case 1: {
                SoundManager::Get().func_020246f8(0x35);
                func_ov007_0226a380(p, p->mRecord, 0x1e);
                CameraManager::Get();
                MtxFx43 mtx    = data_020c1cf4;
                VecFx32 camPos = data_020d433c.mBattlePlayers->mUnk_298->mUnk_1a4->mPosition;
                VecFx32 v      = data_ov007_022793ec;
                u8 handle      = func_0202bd40(data_020d433c.mEffects, 1, 3, 0, 0);
                if (handle) {
                    mtx._[11] = 0;
                    mtx._[10] = 0;
                    mtx._[9]  = 0;
                    MTX_MultVec43(&v, &mtx, &v);
                    VEC_Add(&camPos, &v, &camPos);
                    func_01ffcf64(&mtx, &camPos);
                    func_0202bf60(data_020d433c.mEffects, handle, &mtx);
                }
                break;
            }
            case 2: {
                SoundManager::Get().func_020246f8(0x36);
                u8 handle = func_0202bd40(data_020d433c.mEffects, 1, 0xe, 0, 1);
                BattleRecord *rec = &p->mRecords[p->mRecord];
                if ((int)rec->mGauge < 0xfd7) {
                    rec->mGauge += 0x28;
                }
                if (handle) {
                    VecFx32 d;
                    VecFx32 camPos = data_020d433c.mBattlePlayers->mUnk_298->mUnk_1a4->mPosition;
                    CameraManager::Get();
                    MtxFx43 mtx = data_020c1cf4;
                    func_01ffcf64(&mtx, &mPosition);
                    Camera *h = data_020d433c.mBattlePlayers->mUnk_298->mUnk_174;
                    d.x                 = h->mPosition.x;
                    d.y                 = h->mPosition.y;
                    d.z                 = h->mPosition.z;
                    VEC_Subtract(&d, &camPos, &d);
                    VEC_Normalize(&d, &d);
                    d.x = FX_Mul(d.x, 0x1e000);
                    d.y = FX_Mul(d.y, 0x1e000);
                    d.z = FX_Mul(d.z, 0x1e000);
                    VEC_Add(&d, &data_020d433c.mBattlePlayers->mUnk_298->mUnk_1a4->mPosition, &d);
                    func_01ffcf64(&mtx, &d);
                    func_0202bf60(data_020d433c.mEffects, handle, &mtx);
                }
                break;
            }
            }
            mStep++;
            if (mStep >= data_ov007_022793d0[mKind]) {
                mStep  = 0;
                mTimer = 0;
                return;
            }
        }
    }
    if (mTimer) {
        mTimer++;
    }
}

void GimmickFieldDestroy::func_ov007_0222b8b4() {
    BattlePlayerList *plist = data_020d433c.mBattlePlayers;
    BattlePlayer *p         = plist->mPlayers[plist->mUnk_28a];
    mKind                   = func_ov007_0222b328();
    switch (mKind) {
    case 0: {
        int v = p->mUnk_f3 + 0xbe;
        if (v >= 0xbe) {
            v = 0xbe;
        }
        p->mUnk_f3 = v;
        u8 handle  = func_0202bd40(data_020d433c.mEffects, 1, 0xc, 0, 1);
        if (!handle) {
            return;
        }
        CameraManager::Get();
        MtxFx43 mtx2           = data_020c1cf4;
        BattlePlayerList *list = data_020d433c.mBattlePlayers;
        MtxFx43 mtx            = data_0220e86c;
        VecFx32 camPos         = list->mUnk_298->mUnk_1a4->mPosition;
        VecFx32 v2             = data_ov007_02279404;
        mtx2._[11]             = 0;
        mtx2._[10]             = 0;
        mtx2._[9]              = 0;
        MTX_MultVec43(&v2, &mtx2, &v2);
        VEC_Add(&camPos, &v2, &camPos);
        func_01ffd088(&mtx, 0x1555);
        func_01ff89c0(&mtx, &mtx2, &mtx2);
        func_01ffcf64(&mtx2, &camPos);
        func_0202bf60(data_020d433c.mEffects, handle, &mtx2);
        break;
    }
    case 1: {
        u8 handle = func_0202bd40(data_020d433c.mEffects, 1, 0xd, 0, 1);
        if (!handle) {
            return;
        }
        VecFx32 d;
        VecFx32 camPos = data_020d433c.mBattlePlayers->mUnk_298->mUnk_1a4->mPosition;
        CameraManager::Get();
        MtxFx43 mtx = data_020c1cf4;
        func_01ffcf64(&mtx, &mPosition);
        Camera *h = data_020d433c.mBattlePlayers->mUnk_298->mUnk_174;
        d.x                 = h->mPosition.x;
        d.y                 = h->mPosition.y;
        d.z                 = h->mPosition.z;
        VEC_Subtract(&d, &camPos, &d);
        VEC_Normalize(&d, &d);
        d.x = FX_Mul(d.x, 0x1e000);
        d.y = FX_Mul(d.y, 0x1e000);
        d.z = FX_Mul(d.z, 0x1e000);
        VEC_Add(&d, &data_020d433c.mBattlePlayers->mUnk_298->mUnk_1a4->mPosition, &d);
        func_01ffcf64(&mtx, &d);
        func_0202bf60(data_020d433c.mEffects, handle, &mtx);
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
    if (!mModelB->func_02028d80()) {
        mState++;
    }
    CameraManager::Get();
    MtxFx43 mtx = data_020c1cf4;
    func_01ffcf64(&mtx, &mPosition);
    mModelB->func_02029174(&mtx, &mScale);
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
    func_01ffcf64(&mtx, &dir);
    mModelE->func_02028d80();
    mModelE->func_02029174(&mtx, &mScale);
}

VecFx32 data_ov007_022793f8 = {0x1000, 0x1000, 0x1000};
VecFx32 data_ov007_022793ec = {0, 0xf000, -0xa000};
VecFx32 data_ov007_02279404 = {0, 0, 0x1e000};
