#include "Battle/BattleBase.hpp"
#include <nitro/fx.h>
#include "Battle/BattleCardEffect.hpp"
#include "Battle/BattleUnk29c.hpp"
#include "Battle/BattleCpu.hpp"
#include "Battle/BattleCommand.hpp"
#include "Battle/BattleMapTypes.hpp"
#include "Battle/BattleStatusPanels.hpp"
#include "Battle/GimmickTypes.hpp"
#include "Game/UnkStruct_020d433c.hpp"
#include "Graphics/Camera.hpp"
#include "Graphics/Effect.hpp"
#include "Graphics/Model.hpp"
#include "Graphics/SpriteManager.hpp"
#include <nitro/mi.h>
#include <nitro/os.h>
#include "System/FileSystem.hpp"

// The order of these definitions sets the data layout: mwcc sorts a file's objects by size, and the order of
// equal sizes depends on where each was defined (docs/decompiling.md, "Data layout within a section").

// Only the middle angle is read, folded to a constant; the 6 bytes at 0x02279eb4 show the table was there.
static const u16 sCommandCamAngles[3] = {0, 0x2000, 0};
static const VecFx32 data_ov007_02279f24[4] = {
    {0x87000, 0x899a, 0x87000},
    {-0x87000, 0x899a, 0x87000},
    {-0x87000, 0x899a, -0x87000},
    {0x87000, 0x899a, -0x87000},
};
static const VecFx32 data_ov007_02279ed0 = {0x1000, 0x1000, 0x1000};
static const VecFx32 data_ov007_02279f54[4] = {
    {0xaa000, 0x899a, 0xaa000},
    {-0xaa000, 0x899a, 0xaa000},
    {-0xaa000, 0x899a, -0xaa000},
    {0xaa000, 0x899a, -0xaa000},
};
static const VecFx32 data_ov007_02279f84[4] = {
    {0xc8000, 0x899a, 0xc8000},
    {-0xc8000, 0x899a, 0xc8000},
    {-0xc8000, 0x899a, -0xc8000},
    {0xc8000, 0x899a, -0xc8000},
};
static const VecFx32 data_ov007_02279fb4[4] = {
    {0xbe000, 0x899a, 0xbe000},
    {-0xbe000, 0x899a, 0xbe000},
    {-0xbe000, 0x899a, -0xbe000},
    {0xbe000, 0x899a, -0xbe000},
};
static const VecFx32 data_ov007_02279fe4[4] = {
    {0x14a000, 0x899a, 0x14a000},
    {-0x14a000, 0x899a, 0x14a000},
    {-0x14a000, 0x899a, -0x14a000},
    {0x14a000, 0x899a, -0x14a000},
};
static const VecFx32 data_ov007_0227a014[4] = {
    {0x78000, 0x899a, 0x78000},
    {-0x78000, 0x899a, 0x78000},
    {-0x78000, 0x899a, -0x78000},
    {0x78000, 0x899a, -0x78000},
};
static const u8 data_ov007_02279eb0[4] = {0, 1, 3, 2};
static const VecFx32 data_ov007_0227a044[4] = {
    {0xaf000, 0x899a, 0xaf000},
    {-0xaf000, 0x899a, 0xaf000},
    {-0xaf000, 0x899a, -0xaf000},
    {0xaf000, 0x899a, -0xaf000},
};
static const u16 data_ov007_02279efc[20] = {1, 0x7fff, 0xf666, 0xf5ec, 0xf829};
static const VecFx32 data_ov007_0227a074[4] = {
    {0x11db0a, 0x899a, 0x11db0a},
    {-0x11db0a, 0x899a, 0x11db0a},
    {-0x11db0a, 0x899a, -0x11db0a},
    {0x11db0a, 0x899a, -0x11db0a},
};
static const u32 data_ov007_02279edc[8] = {0x4b000, 0x3a000, 0x46000, 0x46000, 0x46000, 0x46000, 0x46000, 0x46000};
static const VecFx32 data_ov007_02279ec4 = {-0xf000, 0, 0};
// Start camera file per stage.
static const char *sStartCamFiles[] = {
    "StaStartCam0.cam", "pyruStartCam0.cam", "aquStartCam0.cam", "VenStartCam0.cam",
    "SubStartCam0.cam", "HaoStartCam0.cam",  "DarkStartCam0.cam", "aniStartCam0.cam",
};
// Map cursor start position per stage, indexed by the player's corner.
static const VecFx32 *sCornerPositions[] = {
    data_ov007_02279f24, data_ov007_02279f54, data_ov007_02279f84, data_ov007_02279fb4,
    data_ov007_02279fe4, data_ov007_0227a014, data_ov007_0227a044, data_ov007_0227a074,
};
static const VecFx32 data_ov007_0227a0a4[4] = {
    {0x78000, 0x46000, 0xc8000},
    {-0x78000, 0x46000, 0xc8000},
    {-0x78000, 0x46000, -0xc8000},
    {0x78000, 0x46000, -0xc8000},
};
// Camera near and far clip per stage.
static const fx32 sClipPlanes[8][2] = {
    {0x2000, 0xe74000}, {0x2000, 0xe74000}, {0x2000, 0xe74000}, {0x2000, 0xe74000},
    {0x2000, 0xe74000}, {0x2000, 0xe74000}, {0x2000, 0xe74000}, {0x2000, 0xe74000},
};

extern "C" void GetCornerPosition(BattleCommandMap *map, VecFx32 *out);
extern "C" void func_ov007_022476e0(BattleCommandMap *map);
extern "C" void func_ov007_02247898(BattleCommandMap *map);
extern "C" void func_ov007_0224796c(BattleCommandMap *map);
extern "C" void func_ov007_022479d4(BattleCommandMap *map, u32 a);
extern "C" void func_ov007_02247a1c(BattleCommandMap *map, u32 a);
extern "C" void func_ov007_02246a6c(BattleCommandMap *map);
extern "C" void func_ov007_02246804(BattleCommandMap *map);
extern "C" void func_ov007_02246758(BattleCommandMap *map);
extern "C" void func_ov007_0224677c(BattleCommandMap *map);
extern "C" void func_ov007_02246730(BattleCommandMap *map);
extern "C" void func_ov007_02246578(BattleCommandMap *map);
extern "C" void func_ov007_02246558(BattleCommandMap *map);
void func_ov007_02233078(VecFx32 *a, VecFx32 *b, int c, VecFx32 *d);


struct Rot16 {
    u16 x, y, z;
};

static inline void ApplyRot(BakuganModel *obj, Rot16 *rot, u16 base);

// Copies the active camera into the 3D engine state and the camera manager's snapshot of it.
static inline void SnapshotCamera() {
    CameraManager &manager        = CameraManager::Get();
    Camera *camera                = manager.mCameras[manager.mActive];
    data_02211374.mCameraPosition = camera->mPosition;
    data_02211374.mCameraUp       = camera->mUp;
    data_02211374.mCameraTarget   = camera->mTarget;
    func_01ff8d34(&camera->mPosition, &camera->mUp, &camera->mTarget, &data_02211374.mView);
    data_02211374.mFlags &= ~0xe8;
    manager.mProjection[manager.mActive] = data_02211374.mProjection;
    manager.mView[manager.mActive]       = data_02211374.mView;
    manager.mMtxA[manager.mActive]       = *func_02032710();
    manager.mMtxB[manager.mActive]       = *func_02032604();
    manager.mMtxC[manager.mActive]       = *func_020323b8();
}

BattleCommandMap::BattleCommandMap(u8 unk)
    : SceneBase(0x7d0, 0) {
    mUnk_155 = 0;
    mState   = 0;
    mKind = unk;
    mCursorX = 0;
    mCursorY = 0;
    mUnk_14c = 0;
    mUnk_14d = 0;
    mUnk_14f = 0;
    mUnk_150 = 0;
    mUnk_151 = 1;
    mUnk_152 = 0;
    mUnk_153 = 0;
    mUnk_154 = 0;
    mUnk_13c = 0;
    mUnk_13e = 0;
    mUnk_140 = 0;
    mUnk_142 = 0;
    mUnk_12c = 0;
    mCamMoveTimer = 0;
    mUnk_130 = 0;
    mUnk_134 = 0;
    MI_CpuClear32(mUnk_114, 0xc);
    MI_CpuClear32(mUnk_d8, 0xc);
    MI_CpuClear32(mUnk_108, 0xc);
    MI_CpuClear32(mCamEndTarget, 0xc);
    MI_CpuClear32(mCamEndPosition, 0xc);
    MI_CpuClear32(mCamMoveOffset, 0xc);
    MI_CpuClear32(mUnk_78, 0x60);
    u8 i = 0;
    do {
        mUnk_18[i] = 0;
        i++;
    } while (i < 12);
    mUnk_14 = 0;
    mUnk_48 = 0;
    mUnk_4c = 0;
    u8 j = 0;
    do {
        mUnk_50[j] = 0;
        j++;
    } while (j < 4);
    mMapObj = 0;
    mUnk_64 = 0;
    mUnk_68 = 0;
    mUnk_6c = 0;
    mStatusPanels = 0;
    mStartCamFile = 0;
    if (data_020d433c.mGimmicks) {
        data_020d433c.mGimmicks->LoadAll();
    }
    mUnk_74 = data_020d433c.mBattlePlayers->mUnk_29c;
    if (mUnk_74) {
        mUnk_74->CreateFontObjs();
    }
    mLights = new ModelLightGroup(data_ov007_02279efc);
    GetCornerPosition(this, (VecFx32 *) mUnk_120);
}

BattleCommandMap::~BattleCommandMap() {
    u8 i = 0;
    do {
        mUnk_74->func_ov007_0226fc5c(i);
        i++;
    } while (i < 12);
    if (mUnk_14) {
        delete mUnk_14;
    }
    if (mUnk_48) {
        delete mUnk_48;
    }
    if (mUnk_4c) {
        delete mUnk_4c;
    }
    func_ov007_022455c0();
    if (mMapObj) {
        delete mMapObj;
    }
    if (mUnk_64) {
        delete mUnk_64;
    }
    if (mUnk_68) {
        delete mUnk_68;
    }
    if (mUnk_6c) {
        delete mUnk_6c;
    }
    if (mStatusPanels) {
        delete mStatusPanels;
    }
    FreeStartCamFile();
    if (data_020d433c.mGimmicks) {
        data_020d433c.mGimmicks->UnloadAll();
    }
    if (mUnk_74) {
        mUnk_74->Release();
    }
    if (mLights) {
        delete mLights;
    }
}

void BattleCommandMap::FreeStartCamFile() {
    if (mStartCamFile) {
        OS_Free(mStartCamFile);
        mStartCamFile = 0;
    }
}

void BattleCommandMap::func_ov007_022455c0() {
    u8 i = 0;
    do {
        BattleMapModel *model = mUnk_50[i];
        if (model) {
            delete model;
        }
        mUnk_50[i] = 0;
        i++;
    } while (i < 4);
}

void BattleCommandMap::func_ov007_0224560c(u32 index) {
    BattleMapModel *model = mUnk_50[index];
    if (model == 0) {
        return;
    }
    if (model->mKind != 3) {
        return;
    }
    if (model) {
        delete model;
    }
    mUnk_50[index] = 0;
}

void BattleCommandMap::Update() {
    switch (mState) {
        case 0:
            UpdateCameraMove();
            UpdateStartCamPath();
            if (mUnk_14f != 0) {
                func_ov007_02245e58();
            }
            func_ov007_022458f4();
            func_ov007_022458cc();
            func_ov007_02246450();
            break;
        case 1:
            func_ov007_02246450();
            if (CameraManager::Get().mMode == 2) {
                SpriteManager::Get().StartFade(3);
            } else {
                SpriteManager::Get();
                SpriteManager::sFade[0] = 3;
            }
            mState++;
            break;
        case 2:
            func_ov007_02246450();
            SpriteManager::Get();
            if (SpriteManager::sFade[0] <= 1) {
                mState++;
            }
            break;
        case 3:
            RequestScene(1);
            mFinished = 1;
            break;
    }
}

void CloseCommandMapNow(BattleCommandMap *map) {
    map->mState = 3;
}

void func_ov007_02245888(BattleCommandMap *map) {
    map->mState = 1;
}

void BattleCommandMap::func_ov007_02245894() {
    func_ov007_022479d4(this, 1);
    func_ov007_02247a1c(this, 1);
}

void BattleCommandMap::Init() {
    func_ov007_02245894();
    func_ov007_022458cc();
}

void BattleCommandMap::func_ov007_022458cc() {
    func_ov007_022476e0(this);
    func_ov007_02247898(this);
    func_ov007_0224796c(this);
    mUnk_74->Update();
}

void BattleCommandMap::func_ov007_022458f4() {
    func_ov007_02245f08();
}

void BattleCommandMap::UpdateStartCamPath() {
    if (mStartCamFile == 0) {
        return;
    }
    Camera *camera = data_020d433c.mBattlePlayers->mCamera;
    if (camera == 0) {
        return;
    }
    camera->AdvanceFrame(0);
    camera->ApplyPath();
    SnapshotCamera();
}

void BattleCommandMap::UpdateCameraMove() {
    if (mCamMoveTimer == 0) {
        return;
    }
    Camera *camera = data_020d433c.mBattlePlayers->mCamera;
    if (camera) {
        VecFx32 target = *(VecFx32 *) mCamEndTarget;
        VecFx32 pos    = *(VecFx32 *) mCamEndPosition;
        VecFx32 delta;
        mCamMoveTimer--;
        if (mCamMoveTimer != 0) {
            float t = (float) (u32) mCamMoveTimer / 30.0f;
            float s = t * t;
            delta.x = (fx32) (s * (float) (s32) mCamMoveOffset[0]);
            delta.y = (fx32) (s * (float) (s32) mCamMoveOffset[1]);
            delta.z = (fx32) (s * (float) (s32) mCamMoveOffset[2]);
            VEC_Subtract(&target, &delta, &target);
            VEC_Subtract(&pos, &delta, &pos);
        }
        camera->mTarget   = target;
        camera->mPosition = pos;
    }
    SnapshotCamera();
}

void BattleCommandMap::func_ov007_02245e58() {
    float step = (float) mUnk_12c / 8.0f;
    u8 i       = 0;
    do {
        func_ov007_02233078((VecFx32 *) mUnk_120, (VecFx32 *) mUnk_114, (u32) (mUnk_134 + (float) i * step), (VecFx32 *) &mUnk_78[i * 0xc]);
        i++;
    } while (i < 8);
    float limit = (float) mUnk_12c / 8.0f;
    mUnk_134    = mUnk_134 + limit / 30.0f;
    if (mUnk_134 >= limit) {
        mUnk_134 = 0;
    }
}


void BattleCommandMap::func_ov007_02245f08() {
    if (mUnk_150 == 0) {
        return;
    }
    BakuganModel *obj; 
    obj = mUnk_14;
    if (obj == 0) {
        return;
    }
    u16 angle = 0; 
    BattlePlayer *player; 
    s32 dy;
    s32 sx;
    s32 sy;
    float fy;
    s32 dx;
    float fx;
    player = data_020d433c.mBattlePlayers->Current();
    dy     = -player->mUnk_d8;
    dx     = player->mUnk_d4;
    s32 ay = dy;
    if (ay < 0) {
        ay = -ay;
    }
    if (ay < 0x31) {
        sy = 0;
    } else if (dy < 0) {
        sy = -1;
    } else if (dy > 0) {
        sy = 1;
    }
    s32 ax = dx;
    if (ax < 0) {
        ax = -ax;
    }
    if (ax < 0x31) {
        sx = 0;
    } else if (dx < 0) {
        sx = -1;
    } else if (dx > 0) {
        sx = 1;
    }

    fy     = (float) dy / 4096.0f;
    fx     = (float) dx / 4096.0f;
    if (sx != 0) {
        if (fx < 0.0f) {
            fx = 0.0f - fx;
        }
        fx *= 14.84058f;
    }
    if (sy != 0) {
        if (fy < 0.0f) {
            fy = 0.0f - fy;
        }
        fy *= 14.84058f;
    }
    if (sx == 0) {
        fx = 0;
        fy *= 2.0f;
    }
    if (sy == 0) {
        fy = 0;
        fx *= 2.0f;
    }
    float m = fy + fx;
    m *= 12.0f;
    Rot16 rot = {0, 0, 0};
    if (sx == 0 && sy == 0) {
    } else if (sy != 0 && sx == 0) {
        if (sy < 0) {
            rot.z = 65536.0f * (360.0f - m) / 360.0f;
        } else {
            rot.z = 65536.0f * m / 360.0f;
        }
        ApplyRot(obj, &rot, 0);
        angle += 90;
    } else if (sx != 0 && sy == 0) {
        if (sx < 0) {
            rot.z = 65536.0f * m / 360.0f;
        } else {
            rot.z = 65536.0f * (360.0f - m) / 360.0f;
        }
        ApplyRot(obj, &rot, 0x4000);
    } else if (sx < 0 && sy < 0) {
        rot.z = 65536.0f * m / 360.0f;
        angle += 130;
        ApplyRot(obj, &rot, 0x78e3);
    } else if (sx > 0 && sy < 0) {
        rot.z = 65536.0f * m / 360.0f;
        angle += 60;
        ApplyRot(obj, &rot, 0x98e3);
    } else if (sx > 0 && sy > 0) {
        rot.z = 65536.0f * (360.0f - m) / 360.0f;
        angle += 130;
        ApplyRot(obj, &rot, 0x78e3);
    } else if (sx < 0 && sy > 0) {
        rot.z = 65536.0f * (360.0f - m) / 360.0f;
        angle += 60;
        ApplyRot(obj, &rot, 0x98e3);
    }
    obj->mRotY = (u16) (65536.0f * (float) (u16) (angle % 360) / 360.0f);
    Camera *camera = data_020d433c.mBattlePlayers->mCamera;
    if (camera) {
        obj->mRotY = obj->mRotY + camera->mYaw;
    }
    BakuganModel *dst = mUnk_14;
    dst->mRotX = obj->mRotX;
    dst->mRotY = obj->mRotY;
    dst->mRotZ = obj->mRotZ;
}




static inline void ApplyRot(BakuganModel *obj, Rot16 *rot, u16 base) {
    u16 x = obj->mRotX + rot->x;
    u16 y = obj->mRotY + rot->y;
    u16 z = obj->mRotZ + rot->z;
    obj->mRotX = x;
    obj->mRotY = y;
    obj->mRotZ = z;
    obj->mRotX = base;
}

void BattleCommandMap::func_ov007_02246450() {
    func_ov007_02246a6c(this);
    func_ov007_02246804(this);
    func_ov007_02246758(this);
    if (mUnk_14f != 0) {
        func_ov007_0224677c(this);
    }
    func_ov007_02246730(this);
    data_020d433c.mEffects->UpdateTagged(1);
    BOOL show = TRUE;
    if (CameraManager::Get().mMode == 2) {
        if (CameraManager::Get().mActive != 0) {
            show = FALSE;
        }
    }
    if (show) {
        func_ov007_02246578(this);
        func_ov007_02246558(this);
    }
}

void GetCellPosition(VecFx32 *out, u8 a, u8 b);

extern "C" void func_ov007_02246558(BattleCommandMap *map) {
    if (map->mUnk_151 != 0 && map->mStatusPanels) {
        map->mStatusPanels->DrawPanels();
    }
}

extern "C" void func_ov007_02246578(BattleCommandMap *map) {
    s8 index               = data_020d433c.mBattlePlayers->mUnitHandles[map->mCursorX][map->mCursorY];
    u8 current             = data_020d433c.mBattlePlayers->mUnk_2a9;
    if (index <= 0) {
        return;
    }
    BattleUnit *unit = &data_020d433c.mBattlePlayers->Units()[(u8) (s8) (index - 1)];
    if (unit->mStatus == 0) {
        return;
    }
    VecFx32 pos;
    pos.x = unit->mPos.x;
    pos.y = unit->mPos.y + 0x2299a;
    pos.z = unit->mPos.z;
    map->mUnk_74->func_ov007_022712c4(unit->mPlayer, &pos);
    pos.y += 0xa000;
    CameraManager::Get();
    MtxFx43 mtx = data_020c1cf4;
    VecFx32 v   = data_ov007_02279ec4;
    mtx._[11]   = 0;
    mtx._[10]   = 0;
    mtx._[9]    = 0;
    MTX_MultVec43(&v, &mtx, &v);
    VEC_Add(&pos, &v, &pos);
    if (unit->mPlayer == current) {
        map->mUnk_74->func_ov007_02271338(&pos);
    } else if (unit->mPlayer == data_020d433c.mBattlePlayers->mPlayers[current]->mPartner) {
        map->mUnk_74->func_ov007_022713a4(&pos);
    }
}

extern "C" void func_ov007_02246730(BattleCommandMap *map) {
    if (map->mUnk_150 != 0 && map->mUnk_154 != 0 && map->mUnk_14) {
        map->mUnk_14->DrawModel();
    }
}

extern "C" void func_ov007_02246758(BattleCommandMap *map) {
    if (map->mUnk_14c != 0 && map->mUnk_152 != 0) {
        map->mUnk_74->Draw();
    }
}

extern "C" void func_ov007_0224677c(BattleCommandMap *map) {
    u8 i = 0;
    do {
        map->mUnk_64->mPosition = *(VecFx32 *) ((u8 *) map + i * 0xc + 0x78);
        map->mUnk_64->func_020526bc();
        i++;
    } while (i < 8);
    VecFx32 v = *(VecFx32 *) map->mUnk_d8;
    map->mUnk_68->mPosition = v;
    map->mUnk_68->func_020526bc();
}


extern "C" void func_ov007_02246804(BattleCommandMap *map) {
    if (map->mUnk_14d == 0) {
        return;
    }
    u8 i = 0;
    do {
        if (map->mUnk_50[i]) {
            map->mUnk_50[i]->func_020526bc();
        }
        i++;
    } while (i < 4);
    if (map->mUnk_153 == 0) {
        return;
    }
    VecFx32 a;
    GetCellPosition(&a, map->mCursorX, map->mCursorY);
    if (map->mUnk_4c) {
        a.y += 0x75c;
        map->mUnk_4c->mPosition = a;
        map->mUnk_4c->func_020526bc();
    }
    s32 neg = -0x6000;
    u8 j    = 0;
    do {
        BattleMarker *marker = &data_020d433c.mBattlePlayers->Entries()[j];
        if (marker->mStatus == 1) {
            if (map->mUnk_48) {
                VecFx32 b;
                GetCellPosition(&b, marker->mCellX, marker->mCellY);
                map->mUnk_48->mPosition = b;
                s32 frame = map->mUnk_48->mModel->GetAnimFrame();
                if (marker->mPlayer == data_020d433c.mBattlePlayers->mUnk_2a9) {
                    map->mUnk_48->DrawPulsing();
                } else {
                    map->mUnk_48->mModel->GetAnimFrame();
                    map->mUnk_48->mModel->SetAnimFrame(0);
                    map->mUnk_48->func_020526bc();
                }
                map->mUnk_48->mModel->SetAnimFrame(frame);
            }
        } else if (marker->mStatus == 3) {
            if (map->mUnk_48) {
                VecFx32 c;
                GetCellPosition(&c, marker->mCellX, marker->mCellY);
                if (map->mUnk_155 == 1) {
                    map->mUnk_138 = map->mUnk_138 - 0x3000;
                    if (!map->mUnk_48->mModel->AdvanceAnim()) {
                        map->mUnk_48->mModel->mAnim->mFramePerFrame = neg;
                        map->mUnk_138 = map->mUnk_138 * (neg >> 15);
                        map->mUnk_155++;
                    }
                } else {
                    map->mUnk_138 = map->mUnk_138 - 0x3000;
                    map->mUnk_48->mModel->AdvanceAnim();
                    if (map->mUnk_48->mModel->GetAnimFrame() <= 0x6000) {
                        data_020d433c.mBattlePlayers->Entries()[j].mStatus = 1;
                    }
                }
                c.x += map->mUnk_138;
                map->mUnk_48->mPosition = c;
                map->mUnk_48->func_020526bc();
            }
        }
        j++;
    } while (j < 12);
}

extern "C" void func_ov007_02246d88(BattleCommandMap *map);
extern "C" void func_ov007_02246ddc(BattleCommandMap *map);
extern "C" void func_ov007_02246efc(BattleCommandMap *map);
extern "C" void func_ov007_0224701c(BattleCommandMap *map, VecFx32 *v);
extern "C" void func_ov007_022470f4(BattleCommandMap *map, u8 a, u8 b, u32 frames);
extern "C" void func_ov007_02247544(BattleCommandMap *map, VecFx32 *target, VecFx32 *pos, u32 frames);
extern "C" void func_ov007_02247618(BattleCommandMap *map);
extern "C" void func_ov007_0224768c(BattleCommandMap *map);

extern "C" void func_ov007_02246a6c(BattleCommandMap *map) {
    if (map->mMapObj) {
        map->mMapObj->Draw();
    }
}

void func_ov007_02246a84(BattleCommandMap *map, u32 mode) {
    Camera *camera = data_020d433c.mBattlePlayers->mCamera;
    camera->mUp.x  = 0;
    camera->mUp.y  = 0x1000;
    camera->mUp.z  = 0;
    camera->mNear  = 0x1000;
    camera->mFar   = 0x800000;
    switch (mode) {
        case 3:
            func_ov007_02246d88(map);
            break;
        case 2:
        case 4:
        case 5:
        case 7:
        case 8:
            func_ov007_02246efc(map);
            break;
    }
    CameraManager::Get().mActive = 0;
    CameraManager::Get().SetCamerasAndRequestMode(1, camera, 0);
    SnapshotCamera();
    func_ov007_02247618(map);
    func_ov007_0224768c(map);
}

extern "C" void func_ov007_02246d88(BattleCommandMap *map) {
    Camera *camera = data_020d433c.mBattlePlayers->mCamera;
    if (camera == 0) {
        return;
    }
    camera->SetFov(0x18e3);
    camera->mTarget = *(VecFx32 *) map->mUnk_d8;
    func_ov007_02246ddc(map);
}

extern "C" void func_ov007_02246ddc(BattleCommandMap *map) {
    Camera *camera = data_020d433c.mBattlePlayers->mCamera;
    if (camera == 0 || map->mUnk_14 == 0) {
        return;
    }
    camera->mPosition = map->mUnk_14->mPosition;
    camera->ComputeDistanceAndAngles();
    VecFx32 v;
    v.x       = camera->mPosition.x;
    v.y       = camera->mPosition.y;
    v.z       = camera->mPosition.z;
    s32 index = (camera->mYaw >> 4) * 2;
    v.x       = (fx32) ((float) v.x + 156.0f * (float) FX_SinCosTable_[index]);
    v.z       = (fx32) ((float) v.z + 156.0f * (float) FX_SinCosTable_[index + 1]);
    func_ov007_0224701c(map, &v);
    fx32 z = v.z;
    camera->mPosition.x = v.x;
    camera->mPosition.y = 0x37000;
    camera->mPosition.z = z;
}


extern "C" void func_ov007_02246efc(BattleCommandMap *map) {
    Camera *camera = data_020d433c.mBattlePlayers->mCamera;
    if (camera) {
        camera->SetFov(sCommandCamAngles[1]);
        VecFx32 v = data_ov007_0227a0a4[data_020d433c.mBattlePlayers->mPlayers[data_020d433c.mBattlePlayers->mUnk_2a9]->mUnk_f1];
        v.y       = data_ov007_02279edc[map->mKind];
        camera->mPosition = v;
        camera->mTarget.x = 0;
        camera->mTarget.y = 0;
        camera->mTarget.z = 0;
    }
    func_ov007_022470f4(map, data_020d433c.mBattlePlayers->mUnk_28b, data_020d433c.mBattlePlayers->mUnk_28c, 0);
}

extern "C" u32 func_ov007_02246fc0() {
    s32 result = 0;
    switch (data_020d433c.mBattlePlayers->Current()->mUnk_f1) {
        case 0:
            return result;
        case 1:
            result = 0x10e;
            break;
        case 2:
            result = 0xb4;
            break;
        case 3:
            result = 0x5a;
            break;
    }
    return result;
}

extern "C" void func_ov007_0224701c(BattleCommandMap *map, VecFx32 *v) {
    switch (data_020d433c.mBattlePlayers->Current()->mUnk_f1) {
        case 0:
            if (v->x < 0) {
                v->x = -v->x;
            }
            if (v->z < 0) {
                v->z = -v->z;
            }
            break;
        case 1:
            if (v->x > 0) {
                v->x = -v->x;
            }
            if (v->z < 0) {
                v->z = -v->z;
            }
            break;
        case 2:
            if (v->x > 0) {
                v->x = -v->x;
            }
            if (v->z > 0) {
                v->z = -v->z;
            }
            break;
        case 3:
            if (v->x < 0) {
                v->x = -v->x;
            }
            if (v->z > 0) {
                v->z = -v->z;
            }
            break;
    }
}

void func_ov007_022470e4(BattleCommandMap *map, u8 a, u8 b) {
    func_ov007_022470f4(map, a, b, 0x1e);
}

extern "C" void func_ov007_0224724c(BattleCommandMap *map);

extern "C" void func_ov007_022470f4(BattleCommandMap *map, u8 a, u8 b, u32 frames) {
    if (a == map->mCursorX && b == map->mCursorY && frames == 0x1e) {
        return;
    }
    map->mCursorX = a;
    map->mCursorY = b;
    Camera *camera = data_020d433c.mBattlePlayers->mCamera;
    if (camera == 0) {
        return;
    }
    VecFx32 pos;
    pos.x = camera->mPosition.x;
    pos.y = camera->mPosition.y;
    pos.z = camera->mPosition.z;
    VecFx32 target;
    target.x = camera->mTarget.x;
    target.y = camera->mTarget.y;
    target.z = camera->mTarget.z;
    VecFx32 w;
    VecFx32 d;
    VecFx32 e;
    GetCellPosition(&w, a, b);
    VEC_Subtract(&pos, &target, &d);
    VEC_Add(&w, &d, &e);
    w.y = target.y;
    e.y = pos.y;
    func_ov007_02247544(map, &w, &e, frames);
}

extern "C" void func_ov007_022471c8(BattleCommandMap *map, u32 direction) {
    if (map->mCamMoveTimer != 0) {
        return;
    }
    s32 delta = 0;
    if (direction != 0) {
        if (direction == 1) {
            delta = 1;
        }
    } else {
        delta = delta - 1;
    }
    u16 old       = map->mUnk_146;
    map->mUnk_146 = old + (delta + 360);
    map->mUnk_146 = map->mUnk_146 % 360;
    if (old != map->mUnk_146) {
        func_ov007_0224724c(map);
    }
}

extern "C" void func_ov007_0224724c(BattleCommandMap *map) {
    Camera *camera = data_020d433c.mBattlePlayers->mCamera;
    if (camera == 0) {
        return;
    }
    u16 angle = (u16) (65536.0f * (float) (s32) map->mUnk_146 / 360.0f);
    s32 index = (angle >> 4) * 2;
    s32 sinv  = FX_SinCosTable_[index];
    s32 cosv  = FX_SinCosTable_[index + 1];
    if (map->mUnk_130 == 0) {
        return;
    }
    s32 radius = (s32) ((float) map->mUnk_130 / 4096.0f);
    s32 x      = (s32) ((float) radius * (float) sinv);
    s32 z      = (s32) ((float) radius * (float) cosv);
    x += camera->mTarget.x;
    z += camera->mTarget.z;
    VecFx32 p;
    p.x = x;
    p.y = camera->mPosition.y;
    p.z = z;
    camera->mPosition = p;
    SnapshotCamera();
}

extern "C" s32 func_01ff9208(VecFx32 *a, VecFx32 *b);
extern "C" void func_ov007_02247794(BattleCommandMap *map);

extern "C" void func_ov007_02247544(BattleCommandMap *map, VecFx32 *target, VecFx32 *pos, u32 frames) {
    Camera *camera = data_020d433c.mBattlePlayers->mCamera;
    if (camera == 0) {
        return;
    }
    VecFx32 cur;
    cur.x         = camera->mPosition.x;
    cur.y         = camera->mPosition.y;
    cur.z         = camera->mPosition.z;
    map->mCamMoveTimer = frames;
    *(VecFx32 *) map->mCamEndTarget = *target;
    *(VecFx32 *) map->mCamEndPosition = *pos;
    VEC_Subtract(pos, &cur, (VecFx32 *) map->mCamMoveOffset);
    if (map->mCamMoveTimer != 0) {
        return;
    }
    camera->mTarget   = *(VecFx32 *) map->mCamEndTarget;
    camera->mPosition = *(VecFx32 *) map->mCamEndPosition;
}

extern "C" void func_ov007_02247618(BattleCommandMap *map) {
    Camera *camera = data_020d433c.mBattlePlayers->mCamera;
    if (camera == 0) {
        return;
    }
    VecFx32 pos;
    VecFx32 target;
    pos.x    = camera->mPosition.x;
    pos.y    = camera->mPosition.y;
    pos.z    = camera->mPosition.z;
    target.x = camera->mTarget.x;
    target.y = camera->mTarget.y;
    target.z = camera->mTarget.z;
    target.y = pos.y;
    map->mUnk_130 = func_01ff9208(&target, &pos);
}

extern "C" void func_ov007_0224768c(BattleCommandMap *map) {
    Camera *camera = data_020d433c.mBattlePlayers->mCamera;
    if (camera == 0) {
        return;
    }
    camera->ComputeDistanceAndAngles();
    map->mUnk_146 = (u16) ((float) (camera->mYaw * 360) / 65536.0f);
}

extern "C" void func_ov007_022476e0(BattleCommandMap *map) {
    if (map->mUnk_151 == 0) {
        return;
    }
    if (map->mStatusPanels != 0) {
        return;
    }
    map->mStatusPanels = new BattleStatusPanels(0, 0, 0, 1);
    u8 i         = 0;
    do {
        u8 index             = data_020d433c.mBattlePlayers->mTurnOrder[i];
        BattlePlayer *player = data_020d433c.mBattlePlayers->mPlayers[index];
        map->mStatusPanels->SetupPanel(index, (s8) player->mCharacterId, data_ov007_02279eb0[i], 2);
        i++;
    } while (i < data_020d433c.mPlayerCount);
}

extern "C" void func_ov007_02247794(BattleCommandMap *map) {
    if (map->mUnk_14c == 0) {
        return;
    }
    VecFx32 pos; 
    u8 i = 0;    
    do {
        BattleUnit *unit = &data_020d433c.mBattlePlayers->Units()[i];
        if (unit->mStatus != 0) {
            pos.x = unit->mPos.x;
            pos.y = unit->mPos.y;
            pos.z = unit->mPos.z;
            Rot16 rot = {0, 0, 0};
            rot.y = unit->mUnk_0c;
            BattleRecord *record = &data_020d433c.mBattlePlayers->mPlayers[unit->mPlayer]->mRecords[unit->mRecord];
            map->mUnk_74->CreateBakugan(i, &pos, &rot.x, record->mBakuganId, record->mAttribute);
        }
        i++;
    } while (i < 12);
    if (map->mUnk_6c == 0) {
        map->mUnk_6c = new BattleMapPiece(2);
    }
}

extern "C" void func_ov007_02247898(BattleCommandMap *map) {
    if (map->mUnk_14d == 0) {
        return;
    }
    u8 i = 0;
    do {
        if (data_020d433c.mBattlePlayers->Entries()[i].mStatus == 1 && map->mUnk_48 == 0) {
            map->mUnk_48 = new BattleMapModel(0);
        }
        i++;
    } while (i < 12);
    u8 j = 0;
    do {
        BattleMapModel *model = map->mUnk_50[j];
        if (model) {
            if (map->mUnk_142 == 0) {
                map->func_ov007_0224560c(j);
            } else if (map->mUnk_142 != 0) {
                model->mAnimEnabled = 1;
            }
        }
        j++;
    } while (j < 4);
    if (map->mUnk_142 != 0) {
        map->mUnk_142--;
    }
}

extern "C" void func_ov007_0224796c(BattleCommandMap *map) {
    if (map->mMapObj != 0) {
        return;
    }
    map->mMapObj = new BattleMapObj(map->mKind);
    map->mMapObj->Load();
}

extern "C" void func_ov007_022479a4(BattleCommandMap *map, u32 a) {
    map->mUnk_14c = a != 0;
    func_ov007_02247794(map);
}

extern "C" void func_ov007_022479c0(BattleCommandMap *map, u32 a) {
    map->mUnk_152 = a != 0;
}

extern "C" void func_ov007_022479d4(BattleCommandMap *map, u32 a) {
    func_ov007_022479a4(map, a);
    func_ov007_022479c0(map, a);
}

extern "C" void func_ov007_022479f4(BattleCommandMap *map, u32 a) {
    map->mUnk_14d = a != 0;
}

extern "C" void func_ov007_02247a08(BattleCommandMap *map, u32 a) {
    map->mUnk_153 = a != 0;
}

extern "C" void func_ov007_02247a1c(BattleCommandMap *map, u32 a) {
    func_ov007_022479f4(map, a);
    func_ov007_02247a08(map, a);
}

void func_ov007_02247a3c(BattleCommandMap *map) {
    if (map->mUnk_14d == 0) {
        return;
    }
    if (map->mUnk_4c != 0) {
        return;
    }
    map->mUnk_4c = new BattleMapModel(1);
}

extern "C" void func_ov007_02247a7c(BattleCommandMap *map) {
    if (map->mUnk_14d == 0) {
        return;
    }
    map->func_ov007_022455c0();
    if (map->mUnk_50[0] != 0) {
        return;
    }
    BattleMapModel *model = new BattleMapModel(3);
    map->mUnk_50[0]       = model;
    model->mAnimEnabled        = 0;
    map->mUnk_50[0]->StartAnim(0, 0);
    VecFx32 a;
    GetCellPosition(&a, data_020d433c.mBattlePlayers->mUnk_28b, data_020d433c.mBattlePlayers->mUnk_28c);
    map->mUnk_50[0]->mPosition = a;
    map->mUnk_142              = 0x97;
    Camera *camera             = data_020d433c.mBattlePlayers->mCamera;
    if (camera) {
        camera->mTarget = a;
        a.y += 0x50000;
        a.z += 0x46000;
        camera->mPosition = a;
    }
    CameraManager::Get().mActive = 0;
    CameraManager::Get().SetCamerasAndRequestMode(1, camera, 0);
    SnapshotCamera();
}

extern "C" void func_ov007_02247e00(BattleCommandMap *map) {
    if (map->mUnk_14d == 0) {
        return;
    }
    map->func_ov007_022455c0();
    u8 count    = data_020d433c.mPlayerCount;
    u8 coords[4][2] = {{6, 4}, {7, 5}, {7, 4}, {6, 5}};
    if (data_020d433c.mUnk_8a_0 != 0) {
        if (map->mUnk_50[0] == 0) {
            BattleMapModel *model = new BattleMapModel(4);
            map->mUnk_50[0]       = model;
            model->mAnimEnabled        = 1;
            map->mUnk_50[0]->StartAnim(0, 0);
            VecFx32 a;
            GetCellPosition(&a, coords[0][0], coords[0][1]);
            map->mUnk_50[0]->mPosition = a;
        }
    } else {
        u8 i = 0;
        do {
            if (data_020d433c.mBattlePlayers->mPlayers[i]->mSlots54Remaining != 0 && map->mUnk_50[i] == 0) {
                BattleMapModel *model = new BattleMapModel((u8) (i + 4));
                map->mUnk_50[i]       = model;
                model->mAnimEnabled        = 1;
                map->mUnk_50[i]->StartAnim(0, 0);
                VecFx32 b;
                GetCellPosition(&b, coords[i][0], coords[i][1]);
                map->mUnk_50[i]->mPosition = b;
            }
            i++;
        } while (i < count);
    }
    Camera *camera = data_020d433c.mBattlePlayers->mCamera;
    if (camera) {
        camera->SetFov(0x2aaa);
        if (data_020d433c.mUnk_8a_0 != 0) {
            camera->mTarget.x   = -0x11800;
            camera->mTarget.y   = 0;
            camera->mTarget.z   = -0x17800;
            camera->mPosition.x = -0x11800;
            camera->mPosition.y = 0x58000;
            camera->mPosition.z = 0x4c800;
        } else {
            camera->mTarget.x   = 0;
            camera->mTarget.y   = 0;
            camera->mTarget.z   = 0;
            camera->mPosition.x = 0;
            camera->mPosition.y = 0x58000;
            camera->mPosition.z = 0x64000;
        }
    }
    CameraManager::Get().mActive = 0;
    CameraManager::Get().SetCamerasAndRequestMode(1, camera, 0);
    SnapshotCamera();
}

extern "C" void func_ov007_022486c4(BattleCommandMap *map, VecFx32 *v, u32 mode);


extern "C" void func_ov007_022482a8(BattleCommandMap *map) {
    ChangeDir("/Game/Battle/3d/MapCam");
    map->mUnk_151  = 0;
    Camera *camera = data_020d433c.mBattlePlayers->mCamera;
    if (camera) {
        map->FreeStartCamFile();
        map->mStartCamFile = LoadFile(sStartCamFiles[map->mKind], 0, 0, 0, 0, 0);
        camera->SetPath(map->mStartCamFile, 0);
        u8 index      = map->mKind;
        camera->mNear = sClipPlanes[index][0];
        camera->mFar  = sClipPlanes[index][1];
        new BattleCardEffect(0, 0, 3, 0);
    }
    CameraManager::Get().mActive = 0;
    CameraManager::Get().SetCamerasAndRequestMode(2, camera, 0);
    SnapshotCamera();
    ChangeDir("/");
}

extern "C" void func_ov007_02248620(BattleCommandMap *map, u32 flag) {
    map->mUnk_14f = flag;
    if ((flag & 0xff) == 0) {
        return;
    }
    if (map->mUnk_64 == 0) {
        map->mUnk_64 = new BattleMapPiece(0);
    }
    if (map->mUnk_68 == 0) {
        map->mUnk_68 = new BattleMapPiece(1);
    }
}

extern "C" void func_ov007_02248688(BattleCommandMap *map, VecFx32 *v) {
    VecFx32 w = {0, 0, 0};
    w.x       = v->x;
    w.z       = v->z;
    func_ov007_022486c4(map, &w, 0);
}

extern "C" void func_ov007_022486c4(BattleCommandMap *map, VecFx32 *v, u32 mode) {
    if (map->mUnk_6c == 0) {
        return;
    }
    fx32 x = v->x;
    fx32 y = v->y + 0x3800;
    fx32 z = v->z;
    VecFx32 scale = data_ov007_02279ed0;
    if (mode != 0 && mode != 1) {
        return;
    }
    map->mUnk_6c->mScale = scale;
    VecFx32 *dst = &map->mUnk_6c->mPosition;
    dst->x       = x;
    dst->y       = y;
    dst->z       = z;
    map->mUnk_6c->func_020526bc();
}

extern "C" void GetCornerPosition(BattleCommandMap *map, VecFx32 *out) {
    if (map->mKind >= 8) {
        return;
    }
    *out = sCornerPositions[map->mKind][data_020d433c.mBattlePlayers->Current()->mUnk_f1];
}

extern "C" void func_ov007_0224882c(BattleCommandMap *map);

extern "C" void func_ov007_022487a8(BattleCommandMap *map) {
    VecFx32 v;
    v.x = 0x1000;
    v.y = 0;
    v.z = 0;
    u16 rot[3];
    u32 degrees = func_ov007_02246fc0();
    rot[0] = 0;
    rot[1] = map->mUnk_13c + (u16) (65536.0f * (float) degrees / 360.0f);
    rot[2] = map->mUnk_13e;
    RotateDirScaled(rot, map->mUnk_140, &v, (VecFx32 *) map->mUnk_114);
}

extern "C" void func_ov007_0224882c(BattleCommandMap *map) {
    VecFx32 tmp;
    PredictLanding((VecFx32 *) map->mUnk_120, (VecFx32 *) map->mUnk_114, (VecFx32 *) map->mUnk_d8);
    map->mUnk_d8[1] = 0x24cd;
    map->mUnk_12c   = PredictStop((VecFx32 *) map->mUnk_120, (VecFx32 *) map->mUnk_114, &tmp, 0, 0);
    Camera *camera  = data_020d433c.mBattlePlayers->mCamera;
    if (camera == 0) {
        return;
    }
    camera->mTarget = *(VecFx32 *) map->mUnk_d8;
}

extern "C" void func_ov007_022488ac(BattleCommandMap *map, u32 a, u32 b, u32 c) {
    u8 changed = 0;
    if (a != map->mUnk_13c) {
        map->mUnk_13c = a;
        changed       = 1;
    }
    if (b != map->mUnk_13e) {
        map->mUnk_13e = b;
        changed       = 1;
    }
    if (c != map->mUnk_140) {
        map->mUnk_140 = c;
        changed       = 1;
    }
    if (changed == 0) {
        return;
    }
    func_ov007_022487a8(map);
    func_ov007_0224882c(map);
}

extern "C" void func_ov007_02248910(BattleCommandMap *map) {
    map->mUnk_48->mModel->StartAnimWithIncrement(0, 0, 0x6000);
    map->mUnk_155 = 1;
    map->mUnk_138 = 0;
}

void GetCellPosition(VecFx32 *out, u8 a, u8 b) {
    if (a >= 14 || b >= 10) {
        return;
    }
    s8 x    = a - 6;
    s8 z    = b - 4;
    out->x = x * 0x23000 - 0x11800;
    out->y = 0x24cd;
    out->z = z * 0x2f000 - 0x17800;
}

extern "C" u32 FindMarkerCellAt(VecFx32 *v, u8 *outA, u8 *outB) {
    u8 i = 0;
    do {
        BattleMarker *marker = &data_020d433c.mBattlePlayers->Entries()[i];
        if (marker->mStatus != 0) {
            VecFx32 p;
            GetCellPosition(&p, marker->mCellX, marker->mCellY);
            s32 x0 = p.x - 0x11800;
            s32 x1 = p.x + 0x11800;
            s32 z0 = p.z - 0x17800;
            s32 z1 = p.z + 0x17800;
            if (v->x >= x0 && v->x <= x1 && v->z >= z0 && v->z <= z1) {
                *outA = marker->mCellX;
                *outB = marker->mCellY;
                return 1;
            }
        }
        i++;
    } while (i < 12);
    return 0;
}
