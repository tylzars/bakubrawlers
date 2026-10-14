#include "System/SoundManager.hpp"
#include "Graphics/SpriteManager.hpp"
#include "Battle/BattleCpu.hpp"
#include <nitro/fx.h>
#include "Battle/BattleCardEffect.hpp"
#include "Battle/BattleUnk29c.hpp"
#include "Battle/BattleStatusPanels.hpp"
#include "Battle/BattleTarget.hpp"
#include "Battle/BattleCommand.hpp"
#include "Battle/BattleUnk022637ac.hpp"
#include "Game/Random.hpp"
#include "System/TouchManager.hpp"
#include "System/WirelessManager.hpp"
#include "Battle/GimmickTypes.hpp"
#include "Game/UnkStruct_020d433c.hpp"
#include "Graphics/Camera.hpp"
#include "Graphics/CellAnimation.hpp"
#include "Graphics/Display.hpp"
#include "Graphics/Effect.hpp"
#include "Graphics/SpriteManager.hpp"
#include "Graphics/UnkStruct_0203d374.hpp"
#include "System/FileSystem.hpp"
#include "System/SoundManager.hpp"
#include <nitro/gx.h>
#include <nitro/mi.h>
#include <nitro/os.h>

struct GimmickBuffer {
    u8 mData[0x804];
};

#define ABS(x) ((x) < 0 ? -(x) : (x))
#define INT_TO_FX(n) ((fx32) ((n) > 0 ? 0.5f + (float) ((n) << 12) : (float) ((n) << 12) - 0.5f))

u8 func_ov007_02262768(BattlePlayerList *list, VecFx32 *pos, u16 rot, u8 player, u8 record, u8 cell, u32 flag);
void func_ov007_02262a64(BattlePlayerList *list, u8 a, u8 b);
void func_02028e54(void *model, u32 a, u32 b, u32 c);
extern const u16 data_ov007_0227a5e0[];
extern const u16 data_ov007_0227a5e4[];
void func_ov007_0227238c(BattleSphere *sphere, BattleHitBox *box, VecFx32 *out);
extern "C" void func_01ffcf80(MtxFx33 *m, u16 angle);
extern "C" void DC_FlushRange(void *buf, u32 size);
extern u8 data_ov007_0227a630[];
extern const u8 data_ov007_0227a750[];
extern const s32 data_ov007_0227a6d0[];
extern const u16 data_ov007_0227a5f6[];
extern "C" void func_01ffd030(MtxFx33 *m, u16 angle);
// Two callers leave r2 as it is (the original passes a value already in r2); call through a two-argument type.
static inline void ReleaseSlot(BattlePlayerList *list, u8 index) {
    ((void (*)(BattlePlayerList *, u8)) func_ov007_02262828)(list, index);
}
extern const VecFx32 data_ov007_0227a60c;
void func_ov007_022337ec(VecFx32 *A, VecFx32 *B, VecFx32 *C, VecFx32 *D, VecFx32 *E, fx32 F, fx32 G, int flag);
extern "C" fx32 FX_Atan2(fx32 x, fx32 z);
extern const u16 data_ov007_0227a5ee[];
extern const s32 data_ov007_0227a6bc[];
extern const s8 data_ov007_0227a754[];
extern const u16 data_ov007_0227a660[];
extern const u16 data_ov007_0227a670[];
void func_ov007_02233f38(const VecFx32 *a, const VecFx32 *b, int unused, VecFx32 *out);
extern const VecFx32 data_ov007_0227a648;
void func_ov007_0226a380(BattlePlayer *player, u8 record, int unk);
void func_ov007_02233730(VecFx32 *a, VecFx32 *b, VecFx32 *speed, VecFx32 *out);
extern "C" u32 FX_AcosIdx(s32 value);
extern const s32 data_ov007_0227a694[];
extern const VecFx32 data_ov007_0227a624;
extern const s32 data_ov007_0227a6a8[];
extern const u16 data_ov007_0227a600[];

extern "C" void func_ov007_022488ac(BattleCommandMap *map, u32 a, u32 b, u32 c);
extern "C" void func_ov007_02248688(BattleCommandMap *map, VecFx32 *v);
extern "C" void func_0203d45c(UnkStruct_0203d374 *pose);
BOOL func_ov007_022631bc(BattlePlayerList *list, u32 id);
extern const u32 data_ov007_0227a680[];
extern const u8 data_ov007_0227a74c[];

static inline void SetSphere(BattleSphere *s, const VecFx32 *pos, fx32 radius) {
    s->mPos   = *pos;
    s->mRadius = radius;
}

static inline void SetVec(VecFx32 *v, fx32 x, fx32 y, fx32 z) {
    v->x = x;
    v->y = y;
    v->z = z;
}

// A rotation (three u16 angles) copied as one 6-byte object; see func_ov007_0225a278 / func_ov007_0225b6d0.
struct Rot3 {
    u16 r[3];
};

// True when the effect with this handle (slot number + 1) is still running.
enum EffectState { EFFECT_DEAD, EFFECT_ALIVE };
static inline BOOL EffectAlive(EffectManager *manager, u16 handle) {
    EffectState alive = EFFECT_DEAD;
    if (handle != 0) {
        if (manager->mEffects[handle - 1] != 0) {
            alive = EFFECT_ALIVE;
        }
    }
    return alive;
}

// Drops the status panels and starts the effect of a result (kind).
static inline void StartResult(BattleTarget *t, u32 kind) {
    BattleStatusPanels *panels = t->mUnk_28;
    if (panels) {
        delete panels;
    }
    t->mUnk_28                      = 0;
    data_020d433c.mGimmicks->mState = 0;
    new BattleCardEffect(0x1000, 0, kind, 0);
}

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

// Bits 12-14 of the halfword at +2 of a record.
struct RecordBits {
    u16 mUnk_0 : 12;
    u16 mIndex : 3;
    u16 mUnk_15 : 1;
};

union ObstacleBuf {
    BattleObstacle o;
    u32 w[8];
};
#define OBST(set, idx) ((BattleObstacle *) ((u8 *) (set) + 0x10 + (idx) * 0x20))

static inline void AddRot(BattleObstacle *o, GimmickSetData *s) {
    u16 *r = &s->mRotX;
    o->mRotY += r[1];
    o->mRotZ += r[2];
    o->mRotX += r[0];
}

// Like AddRot, but all three sums are computed before any store (func_ov007_02254ae8, func_ov007_02257f3c).
static inline void AddRotAtOnce(BattleObstacle *o, GimmickSetData *s) {
    u16 *r = &s->mRotX;
    u16 y  = o->mRotY + r[1];
    u16 z  = o->mRotZ + r[2];
    u16 x  = o->mRotX + r[0];
    o->mRotX = x;
    o->mRotY = y;
    o->mRotZ = z;
}

// Same, reading the set's angles as fields rather than through a pointer.
static inline void AddRotAtOnceFields(BattleObstacle *o, GimmickSetData *s) {
    u16 y = o->mRotY + s->mRotY;
    u16 z = o->mRotZ + s->mRotZ;
    u16 x = o->mRotX + s->mRotX;
    o->mRotX = x;
    o->mRotY = y;
    o->mRotZ = z;
}

BattleTarget::BattleTarget(Camera *camera, u8 unk2, u8 unk3)
    : SceneBase(0x3e8, 0) {
    mUnk_28 = 0;
    mUnk_88 = 0;
    mUnk_1c6 = 0;
    mUnk_1c8 = 0;
    mUnk_1cd = 1;
    mUnk_1ce = 0;
    mUnk_1cf = 0;
    mUnk_1d0 = 0;
    mUnk_1d1 = 0;
    mUnk_1d2 = 0;
    mUnk_1d6 = 0xe;
    mUnk_1e2 = 0;
    mUnk_1e7_0 = 1;
    mUnk_1e7_1 = 0;
    mUnk_1eb = 0;
    mUnk_1ec = 0;
    mUnk_174 = camera;
    CameraManager::Get().mActive = 0;
    CameraManager::Get().func_02026504(1, mUnk_174, 0);
    mUnk_118 = 0;
    mUnk_114 = 0;
    mUnk_d4.x = 0;
    mUnk_d4.y = 0;
    mUnk_d4.z = 0;
    mUnk_1b2 = 0;
    mUnk_1b4 = 0;
    mUnk_1b6 = 0;
    mUnk_58.x = 0;
    mUnk_58.y = 0;
    mUnk_58.z = 0;
    mUnk_64.x = 0;
    mUnk_64.y = 0;
    mUnk_64.z = 0;
    mUnk_70.x = 0;
    mUnk_70.y = 0;
    mUnk_70.z = 0;
    mUnk_7c.x = 0;
    mUnk_7c.y = 0;
    mUnk_7c.z = 0;
    mUnk_94.x = 0;
    mUnk_94.y = 0;
    mUnk_94.z = 0;
    mUnk_a0.x = 0;
    mUnk_a0.y = 0;
    mUnk_a0.z = 0;
    mUnk_b8.x = 0;
    mUnk_b8.y = 0;
    mUnk_b8.z = 0;
    mUnk_ac.x = 0;
    mUnk_ac.y = 0;
    mUnk_ac.z = 0;
    mUnk_c8.x = 0;
    mUnk_c8.y = 0;
    mUnk_c8.z = 0;
    mUnk_c4 = 0x1c3;
    mUnk_8c = 0x32000;
    mUnk_40 = mUnk_3c = mUnk_38 = 1;
    mUnk_1ca = 0;
    mUnk_48 = 0;
    mUnk_34 = 0;
    mUnk_4c = 0;
    mUnk_50 = 1;
    mUnk_54 = 1;
    mUnk_1ea = 0;
    mUnk_44 = 0;
    mUnk_90 = 0;
    mUnk_1d4 = unk2;
    mUnk_1d3 = unk3;
    mUnk_1cb = 0;
    mUnk_1d7 = 0;
    mUnk_1d5 = data_020d433c.mBattlePlayers->mPlayers[mUnk_1d4]->mUnk_f2;
    mUnk_1b8 = 0;
    mUnk_1ba = 0;
    mUnk_184 = 0;
    mUnk_11c.x = 0x5000;
    mUnk_11c.y = 0x5000;
    mUnk_11c.z = 0x5000;
    mUnk_128.x = 0;
    mUnk_128.y = 0;
    mUnk_128.z = 0;
    mUnk_134.x = 0;
    mUnk_134.y = 0;
    mUnk_134.z = 0;
    mUnk_168.x = 0;
    mUnk_168.y = 0;
    mUnk_168.z = 0;
    mUnk_15c.x = 0;
    mUnk_15c.y = 0;
    mUnk_15c.z = 0;
    mUnk_2c = 0;
    mUnk_30 = 0;
    mUnk_1db = 0;
    mUnk_1dc = 0;
    mUnk_140 = 0;
    mUnk_1bc = 0;
    mUnk_1d8 = 0;
    mUnk_1dd = 0;
    mUnk_144 = 0;
    mUnk_148 = 0x899a;
    mUnk_1c0 = 0;
    mUnk_1c2 = 0;
    mUnk_1c4 = 0;
    mUnk_1e1 = 0;
    mUnk_1de = 0;
    mUnk_1cc = 0;
    mUnk_1e4 = 0;
    mUnk_1e6 = 0;
    mUnk_17c = 0;
    mUnk_1b0 = 0;
    mUnk_180 = 0;
    mUnk_188.x = 0;
    mUnk_188.y = 0;
    mUnk_188.z = 0;
    mUnk_14c = 0;
    mUnk_1df = 0;
    mUnk_1e0 = 0;
    mUnk_1be = 0;
    mUnk_1a8 = 0;
    mUnk_1e3 = 0;
    mUnk_e0 = 0;
    u8 i = 0;
    do {
        mUnk_e4[i] = 0;
        i++;
    } while (i < 12);
    u8 j = 0;
    do {
        mAnims[j] = 0;
        j++;
    } while (j < 7);
    mUnk_1a0 = 0;
    mUnk_1e8 = 0;
    mUnk_1a4 = 0;
    if (data_020d433c.mBattlePlayers->mUnk_29c) {
        data_020d433c.mBattlePlayers->mUnk_29c->mUnk_a0 = 1;
        data_020d433c.mBattlePlayers->mUnk_29c->mUnk_9e = 1;
    }
    if (data_020d433c.mUnk_95 == 7) {
        mUnk_1e9 = 1;
    } else {
        mUnk_1e9 = 0;
    }
    data_020d433c.mUnk_0c->mUnk_212 = 1;
}

// non-matching: 2 words (+0x4/+0x8): the original loads the vtable pointer before `mov r4, r0`; all other instructions are identical.
BattleTarget::~BattleTarget() {
    data_020d433c.mUnk_0c->mUnk_212 = 0;
    data_020d433c.mEffects->func_0202be68();
    func_0202bc58(data_020d433c.mEffects, 1);
    UnkStruct_0203d374 *pose = mUnk_1a4;
    if (pose) {
        delete pose;
    }
    if (mUnk_e0) {
        delete mUnk_e0;
    }
    u8 i = 0;
    do {
        CellAnimation *anim = mAnims[i];
        if (anim) {
            delete anim;
        }
        i++;
    } while (i < 7);
    u8 j = 0;
    do {
        if (mUnk_e4[j]) {
            func_ov007_02271d78(data_020d433c.mUnk_0c, mUnk_e4[j]);
            mUnk_e4[j] = 0;
        }
        j++;
    } while (j < 12);
    if (mUnk_1a8) {
        func_ov007_02271d78(data_020d433c.mUnk_0c, mUnk_1a8);
    }
    if (mUnk_1ac) {
        func_ov007_02271d78(data_020d433c.mUnk_0c, mUnk_1ac);
    }
    BattleStatusPanels *panels = mUnk_28;
    if (panels) {
        delete panels;
    }
    if (mUnk_2c) {
        OS_Free(mUnk_2c);
    }
    SpriteManager::Get().func_0202740c();
}

void BattleTarget::Update() {
    data_020d433c.mUnk_14 &= 8;
    if (data_020d433c.mUnk_14 == 0) {
        switch (mUnk_1e2) {
            case 0:
                func_ov007_02254020();
                SpriteManager::Get();
                SpriteManager::sFade[0] = 2;
                mUnk_1e2++;
                break;
            case 1:
                mUnk_1eb = 1;
                switch (mUnk_1df) {
                    case 0:
                    case 2:
                    case 4:
                    case 5:
                    case 6:
                        func_ov007_022488ac(mUnk_178, (u16) (0x6000 - mUnk_1b8), (u16) (mUnk_1ba + 0x31c + 0x400), 0x50);
                        break;
                    case 1:
                        func_ov007_022488ac(mUnk_178, (u16) (0x6000 - mUnk_1b8), (u16) (mUnk_1ba + 0x8e + 0x300), 0x78);
                        break;
                    case 3: {
                        u32 idx = data_020d433c.mUnk_95;
                        func_ov007_022488ac(mUnk_178, (u16) (0x6000 - mUnk_1b8), data_ov007_0227a670[idx], (u16) (data_ov007_0227a660[idx] + 0x50));
                        break;
                    }
                }
                break;
            case 2:
                mUnk_1eb = 1;
                if (mUnk_1c6 == 0) {
                    func_ov007_02255640();
                } else {
                    mUnk_50                                         = 0;
                    data_020d433c.mBattlePlayers->mUnk_29c->mUnk_9e = 0;
                    mUnk_1e2++;
                    mUnk_1eb = 0;
                    if (mUnk_1d2 >= 2) {
                        BattlePlayerList *list = data_020d433c.mBattlePlayers;
                        u8 unit                = list->Units()[(u8) (mUnk_1e8 - 1)].mUnk_11;
                        BattleMarker *marker   = &list->Entries()[unit];
                        func_ov007_0225b69c(marker->mUnk_3, marker->mUnk_4);
                    }
                }
                {
                    GimmickSetData *set = mUnk_1a8;
                    if (set) {
                        UnkStruct_0203d374 *pose = mUnk_1a4;
                        set->mUnk_810.x          = pose->mPosition.x;
                        set->mUnk_810.y          = pose->mPosition.y;
                        set->mUnk_810.z          = pose->mPosition.z;
                        UnkStruct_0203d374 *src = mUnk_1a4;
                        GimmickSetData *dst = mUnk_1a8;
                        u16 *rot = &dst->mRotX;
                        rot[0] = src->mRotX;
                        rot[1] = src->mRotY;
                        rot[2] = src->mRotZ;
                    }
                }
                break;
            case 3:
                if (mUnk_1c6 != 0) {
                    if (mUnk_1cd == 0) {
                        func_ov007_02255640();
                        GimmickSetData *set = mUnk_1a8;
                        if (set) {
                            UnkStruct_0203d374 *pose = mUnk_1a4;
                            set->mUnk_810.x          = pose->mPosition.x;
                            set->mUnk_810.y          = pose->mPosition.y;
                            set->mUnk_810.z          = pose->mPosition.z;
                            UnkStruct_0203d374 *src = mUnk_1a4;
                            GimmickSetData *dst = mUnk_1a8;
                            u16 *rot = &dst->mRotX;
                            rot[0] = src->mRotX;
                            rot[1] = src->mRotY;
                            rot[2] = src->mRotZ;
                        }
                    }
                    mUnk_1c6 = mUnk_1c6 + 1;
                }
                mUnk_1cb = 1;
                mUnk_1d7 = 1;
                if (mUnk_1d2 == 2 && mUnk_1e8 != 0 && mUnk_1c6 == 0x32) {
                    switch (mUnk_1df) {
                        case 6:
                            data_020d433c.mBattlePlayers->mUnk_29c->func_ov007_02270a7c(mUnk_1e8 - 1, 0);
                            break;
                        case 5:
                            data_020d433c.mBattlePlayers->mUnk_29c->func_ov007_02270a7c(mUnk_1e8 - 1, 1);
                            break;
                    }
                }
                if (mUnk_1c6 > data_ov007_0227a680[mUnk_1d2]) {
                    SoundManager::Get().func_02024754(0x3c);
                    BattlePlayerList *list = data_020d433c.mBattlePlayers;
                    u8 kind                = list->mPlayers[mUnk_1d4]->mRecords[mUnk_1d3].mUnk_08;
                    u8 a                   = list->mUnk_28d;
                    u8 b                   = list->mUnk_28e;
                    if (kind == 4 && (mUnk_1d2 == 2 || (mUnk_1d2 == 3 && list->Units()[a].mUnk_0f != list->Units()[b].mUnk_0f))) {
                        mUnk_1e2++;
                    } else {
                        mUnk_1e2 = 0xb;
                    }
                    mUnk_1eb = 0;
                }
                break;
            case 4:
                func_02025c1c(SpriteManager::Get().Screen(1).mBgIndex, 5, 0, mUnk_2c, mUnk_30, 0);
                func_ov007_0225cd6c();
                mUnk_34 = 0;
                mUnk_1e2++;
                break;
            case 5: {
                u32 n   = mUnk_34 + 1;
                mUnk_34 = n;
                if (n % 3 == 0) {
                    SoundManager::Get().func_020246f8(0x59);
                }
                WirelessManager::Get();
                if (IsWirelessMode(&data_020c4030.mUnk_228)) {
                    if (mUnk_1d4 == data_020d433c.mBattlePlayers->mUnk_2a9) {
                        if (TouchManager::Get().mUnk_57) {
                            mUnk_1e2++;
                        }
                    } else {
                        mUnk_1e2++;
                    }
                } else {
                    if ((mUnk_1d4 == data_020d433c.mBattlePlayers->mUnk_2a9 && TouchManager::Get().mUnk_57) || data_020d433c.mBattlePlayers->mPlayers[mUnk_1d4]->IsActive()) {
                        mUnk_1e2++;
                    }
                }
                func_ov007_0225cd6c();
                break;
            }
            case 6: {
                u8 pick;
                u32 n   = mUnk_34 + 1;
                mUnk_34 = n;
                if (n % 3 == 0) {
                    SoundManager::Get().func_020246f8(0x59);
                }
                if (func_ov007_022631bc(data_020d433c.mBattlePlayers, 0x15)) {
                    BattleRecord *record = &data_020d433c.mBattlePlayers->mPlayers[mUnk_1d4]->mRecords[mUnk_1d3];
                    u8 values[3]         = {1, 2, 4};
                    pick                 = MATH_Rand32(&data_020d42e8, 3);
                    mAnims[4]->SetAnimation(pick + 3);
                    mAnims[5]->SetAnimation(1);
                    record->mUnk_09 = values[pick];
                    mUnk_34         = 0;
                    mUnk_1e2++;
                }
                func_ov007_0225cd6c();
                break;
            }
            case 7: {
                if (mAnims[4]->mUnk_12 != 0) {
                    if (data_020d433c.mBattlePlayers->mPlayers[mUnk_1d4]->mRecords[mUnk_1d3].mUnk_09 == 1) {
                        mUnk_1e2 = 0xb;
                    } else {
                        mUnk_1c8 = 0x78;
                        mUnk_1e2++;
                    }
                }
                func_ov007_0225cd6c();
                u8 frame = mAnims[4]->mUnk_0f;
                if (mUnk_34 != frame) {
                    u8 t = data_ov007_0227a74c[mAnims[4]->mUnk_0e - 3];
                    if (t <= frame) {
                        if ((t - frame) % 2 == 0) {
                            SoundManager::Get().func_020246f8(0x59);
                        }
                    } else {
                        SoundManager::Get().func_020246f8(0x59);
                    }
                }
                mUnk_34 = mAnims[4]->mUnk_0f;
                break;
            }
            case 8:
                mUnk_1c8 = mUnk_1c8 - 1;
                if (mUnk_1c8 == 0) {
                    SpriteManager::Get();
                    SpriteManager::sFade[0] = 4;
                    SoundManager::Get().func_020246f8(6);
                    mUnk_1e2++;
                }
                func_ov007_0225cd6c();
                break;
            case 9: {
                if (SpriteManager::Get().IsFading()) {
                    break;
                }
                BattlePlayerList *list = data_020d433c.mBattlePlayers;
                BattleRecord *record   = &list->mPlayers[mUnk_1d4]->mRecords[mUnk_1d3];
                mUnk_1a4->func_0203d4dc(record->mUnk_08, record->mUnk_09, 3);
                SpriteManager::Get();
                SpriteManager::sFade[0] = 2;
                mUnk_1c8                   = 0x78;
                mUnk_1e2++;
                break;
            }
            case 10:
                if (SpriteManager::Get().IsFading()) {
                    break;
                }
                mUnk_1c8 = mUnk_1c8 - 1;
                if (mUnk_1c8 == 0) {
                    mUnk_1e2++;
                }
                break;
            case 11: {
                data_020d433c.mEffects->func_0202be68();
                func_0202bc58(data_020d433c.mEffects, 0);
                func_0202bc58(data_020d433c.mEffects, 1);
                BattlePlayerList *list = data_020d433c.mBattlePlayers;
                u8 cur                 = list->mUnk_28a;
                u8 a                   = list->mUnk_28d;
                u8 b                   = list->mUnk_28e;
                if (list->mPlayers[cur]->mUnk_fa != 0 && (mUnk_1d2 == 2 || (mUnk_1d2 == 3 && list->Units()[a].mUnk_0f != list->Units()[b].mUnk_0f))) {
                    SpriteManager::Get();
                    SpriteManager::sFade[0] = 3;
                    mUnk_1e2++;
                    break;
                }
                SpriteManager::Get().StartFade(3);
                mUnk_1e2 = 0xf;
                break;
            }
            case 12:
                if (SpriteManager::Get().IsFading()) {
                    break;
                }
                mContext->mUnk_12 = 0;
                func_ov007_0224587c(mUnk_178);
                func_0203d45c(mUnk_1a4);
                mUnk_1e7_0 = 0;
                mUnk_1e2++;
                break;
            case 13: {
                UnkStruct_SceneContext *context = mContext;
                if (context->mUnk_12 == 0) {
                    break;
                }
                context->mUnk_12 = 0;
                new BattleUnk022637ac();
                mUnk_1e2++;
                break;
            }
            case 14: {
                UnkStruct_SceneContext *context = mContext;
                switch (context->mUnk_12) {
                    case 1:
                        context->mUnk_12 = 0;
                        func_ov007_0224587c(mUnk_178);
                        mUnk_1e7_0 = 0;
                        mUnk_1e2   = 0x10;
                        break;
                    case 2:
                        context->mUnk_12 = 1;
                        mUnk_1e2         = 0x10;
                        break;
                    default:
                        if (mUnk_1e7_1) {
                            mUnk_1e7_1 = 0;
                            func_ov007_0225ce08();
                        }
                        break;
                }
                break;
            }
            case 15:
                if (SpriteManager::Get().IsFading()) {
                    break;
                }
                mContext->mUnk_12 = 0;
                func_ov007_0224587c(mUnk_178);
                mUnk_1e7_0 = 0;
                mUnk_1e2++;
                break;
            case 16: {
                mUnk_1eb = 0;
                if (mContext->mUnk_12 == 0) {
                    break;
                }
                mUnk_1c6                                                  = 0;
                data_020d433c.mBattlePlayers->mPlayers[mUnk_1d4]->mUnk_fa = 0;
                if (mUnk_1df != 0) {
                    data_020d433c.mBattlePlayers->mPlayers[mUnk_1d4]->mUnk_f3 = 0;
                }
                switch (mUnk_1d2) {
                    case 0:
                    case 1:
                        RequestScene(1);
                        mFinished = 1;
                        break;
                    case 2:
                        RequestScene(2);
                        mFinished = 1;
                        break;
                    case 3:
                        RequestScene(3);
                        mFinished = 1;
                        break;
                    case 4:
                        RequestScene(4);
                        mFinished = 1;
                        break;
                }
                break;
            }
            case 17: {
                UnkStruct_SceneContext *context = mContext;
                if (context->mUnk_12 == 0) {
                    break;
                }
                if (mUnk_1ec != 0) {
                    RequestScene(6);
                    mFinished = 1;
                } else {
                    RequestScene(5);
                    mFinished = 1;
                }
                break;
            }
        }
    }
    if (!mUnk_1e7_0) {
        return;
    }
    if (mUnk_1a4) {
        mUnk_1a4->func_0203d988();
    }
    UnkStruct_0203d374 *pose = mUnk_1a4;
    if (pose && pose->mUnk_4a && (mUnk_1cd || mUnk_1e9) && mUnk_178) {
        if (mUnk_1cb == 0) {
            func_ov007_02248688(mUnk_178, &mUnk_58);
        } else {
            func_ov007_02248688(mUnk_178, &pose->mPosition);
        }
    }
    func_ov007_0225c4dc();
    func_ov007_022573e4();
}

void BattleTarget::func_ov007_02253fbc() {
    if (mUnk_1e2 == 0x11) {
        return;
    }
    mUnk_1e2 = 0x11;
    mContext->mUnk_12 = 0;
    func_ov007_0224587c(mUnk_178);
    mUnk_1a4->mUnk_4a = 0;
    data_020d433c.mUnk_14 &= 8;
    if (data_020d433c.mUnk_14) {
        mUnk_1ec = 1;
    }
}

void BattleTarget::func_ov007_02254020() {
    BattlePlayerList *list = data_020d433c.mBattlePlayers;
    BattleRecord *record   = &list->mPlayers[mUnk_1d4]->mRecords[mUnk_1d3];
    func_0202ba64(data_020d433c.mEffects, 1, "shoot");
    mUnk_178 = new BattleCommandMap(data_020d433c.mUnk_95);
    mUnk_178->mUnk_151 = 0;
    mUnk_178->vfunc_0c();
    func_ov007_02248620(mUnk_178, 1);
    func_ov007_022547e4();
    func_ov007_02254898();
    mUnk_94.x = 0;
    mUnk_94.y = 0;
    mUnk_94.z = -0x1000;
    mUnk_1a4 = new UnkStruct_0203d374();
    if ((u8) (record->mUnk_08 + 0xfb) <= 1) {
        record->mUnk_08 = list->mUnk_2b1 % 2 + 5;
    }
    mUnk_1a4->func_0203d4dc(record->mUnk_08, record->mUnk_09, 5);
    mUnk_1a4->mUnk_4a = 0;
    func_ov007_02254790();
    mUnk_28 = new BattleStatusPanels(0x1000, 0, 1, 1);
    u8 i = 0;
    do {
        u8 slot = data_020d433c.mBattlePlayers->mUnk_284[i];
        if (slot == mUnk_1d4) {
            mUnk_28->func_ov007_02235280(slot, (s8) data_020d433c.mBattlePlayers->mPlayers[slot]->mUnk_f6, data_ov007_0227a750[i], 2);
        }
        i++;
    } while (i < data_020d433c.mPlayerCount);
    mUnk_174->SetUp(0, 0x1000, 0);
    mUnk_174->SetClip(0x1000, 0x1388000);
    mUnk_174->func_02026004(0x2aaa);
    CameraManager::Get().mActive = 0;
    CameraManager::Get().func_02026504(1, mUnk_174, 0);
    SnapshotCamera();
    Camera *camera = mUnk_174;
    mUnk_194.x     = camera->mPosition.x;
    mUnk_194.y     = camera->mPosition.y;
    mUnk_194.z     = camera->mPosition.z;
    ChangeDir("/Game/Battle/Obj/");
    SpriteScreen &screen = SpriteManager::Get().Screen(0);
    screen.LoadCharacters("battle_shoot_b_gp.nce_ncg1D.bin", 0, 0);
    screen.LoadPalette("battle_shoot_b_gp_ncl.bin", 0x140, 0);
    u8 j = 0;
    do {
        mAnims[j] = new CellAnimation("battle_shoot_b_gp_nce.bin", 0, 1, 1);
        mAnims[j]->mCharBase = screen.func_020276f4(0);
        mAnims[j]->mUnk_11   = 10;
        j++;
    } while (j <= 3);
    mAnims[1]->SetAnimation(0);
    mAnims[0]->SetAnimation(1);
    mAnims[2]->SetAnimation(2);
    mAnims[3]->SetAnimation(3);
    SpriteScreen &screen1 = SpriteManager::Get().Screen(1);
    screen1.LoadCharacters("battle_b_obj_roulette.nce_ncg1D.bin", 0xa000, 0);
    mUnk_2c = func_02023504("battle_b_obj_roulette_ncl.bin", 0, 0, 0, &mUnk_30, 0);
    mAnims[4] = new CellAnimation("battle_b_obj_roulette_nce.bin", 2, 1, 1);
    mAnims[4]->mCharBase = screen1.func_020276f4(0xa000);
    mAnims[4]->mUnk_11   = 0;
    mAnims[5] = new CellAnimation("battle_b_obj_roulette_nce.bin", 0, 1, 1);
    mAnims[5]->mCharBase = screen1.func_020276f4(0xa000);
    mAnims[5]->mUnk_11   = 0;
    mAnims[6] = new CellAnimation("battle_b_obj_roulette_nce.bin", 6, 1, 1);
    mAnims[6]->mCharBase = screen1.func_020276f4(0xa000);
    mAnims[6]->mUnk_11   = 0;
    ChangeDir("/");
}

void BattleTarget::func_ov007_02254790() {
    VecFx32 pos;
    func_ov007_02248748(mUnk_178, &pos);
    VecFx32 *dst = &mUnk_1a4->mPosition;
    dst->x = pos.x;
    dst->y = pos.y;
    dst->z = pos.z;
    mUnk_58.x = pos.x;
    mUnk_58.y = pos.y;
    mUnk_58.z = pos.z;
}

void BattleTarget::func_ov007_022547e4() {
    u16 rot[3];
    BattleUnit *units = data_020d433c.mBattlePlayers->mUnits;
    u8 i              = 0;
    do {
        BattleUnit *unit = &units[i];
        if (unit->mUnk_10 != 0) {
            if (!mUnk_e0) {
                mUnk_e0 = (u8 *) new GimmickBuffer;
                func_ov007_022719c8(mUnk_e0, 0xb);
            }
            rot[1] = unit->mUnk_0c;
            rot[0] = 0;
            rot[2] = 0;
            mUnk_e4[i] = func_ov007_02271ca8(data_020d433c.mUnk_0c, 2, mUnk_e0, &unit->mPos, rot, i + 1);
        }
        i++;
    } while (i < 12);
}

void BattleTarget::func_ov007_02254898() {
    char buf[0x804];
    VecFx32 pos;
    u16 rot[3];
    pos.x = 0;
    pos.y = 0;
    pos.z = 0;
    rot[0] = 0;
    rot[1] = 0;
    rot[2] = 0;
    UnkStruct_020d433c *data = &data_020d433c;
    u8 id                    = data->mUnk_95;
    if (data->mUnk_9a_4) {
        func_ov007_022719c8(buf, id);
    } else {
        func_ov007_022719c8(buf, id + 0x64);
    }
    mUnk_1ac = func_ov007_02271ca8(data_020d433c.mUnk_0c, 4, buf, &pos, rot, 0);
}

void BattleTarget::func_ov007_02254928(u32 unk) {
    func_ov007_02248620(mUnk_178, unk);
}

BOOL BattleTarget::func_ov007_02254938(VecFx32 *a, VecFx32 *b, VecFx32 *vel, u8 index) {
    VecFx32 diff;
    VecFx32 dir = *vel;
    if (vel->x == 0 && vel->y == 0 && vel->z == 0) {
        return 0;
    }
    VEC_Subtract(b, a, &diff);
    dir.y  = 0;
    diff.y = 0;
    fx32 mag = VEC_Mag(vel);
    if (mag > data_ov007_0227a694[index]) {
        VEC_Normalize(&diff, &diff);
        VEC_Normalize(&dir, &dir);
        fx32 dot = VEC_DotProduct(&diff, &dir);
        if (dot > 0x1000) {
            dot = 0x1000;
        }
        if (FX_AcosIdx(dot) <= data_ov007_0227a600[index]) {
            return 1;
        }
    }
    return 0;
}

void BattleTarget::func_ov007_02254a14(GimmickSetData *set) {
    BattleMoveState state;
    state.mVelocity.x = 0;
    state.mVelocity.y = 0;
    state.mVelocity.z = 0;
    state.mRot[0]     = 0;
    state.mRot[1]     = 0;
    state.mRot[2]     = 0;
    state.mMode       = 3;
    if (!set || !set->mUnk_834) {
        return;
    }
    BattlePlayerList *list = data_020d433c.mBattlePlayers;
    ReleaseSlot(list, set->mUnk_834 - 1);
    UnkStruct_0203d374 *other = list->mUnk_29c->func_ov007_0226fc4c(set->mUnk_834 - 1);
    func_ov007_02233730(&mUnk_1a4->mPosition, &other->mPosition, &mUnk_64, &state.mVelocity);
    list->mUnk_29c->func_ov007_02270ae0(set->mUnk_834 - 1, &state);
    func_ov007_02271d78(data_020d433c.mUnk_0c, set);
    mUnk_e4[set->mUnk_834 - 1] = 0;
}




void BattleTarget::func_ov007_02254ae8(VecFx32 *a, VecFx32 *b, u32 *c, u32 *d) {
    VecFx32 move   = {0, 0, 0};
    VecFx32 hitOut = {0, 0, 0};
    VecFx32 vel    = {0, 0, 0};
    ObstacleBuf self;
    ObstacleBuf obs;
    u8 hit;
    u32 none = 0xffff;
    s32 wallFree;
    u16 marker;
    s32 moved;
    s32 ceilingHit;
    u16 handle;
    BattleRecord *record;
    u8 i;
    marker     = 0;
    hit        = 0;
    mUnk_1d8   = 0;
    moved      = 0;
    ceilingHit = 0;
    wallFree   = 1;
    if (mUnk_4c != 0) {
        mUnk_1cd = 0;
    }
    record = &data_020d433c.mBattlePlayers->mPlayers[mUnk_1d4]->mRecords[mUnk_1d3];
    if (mUnk_1a8->mUnk_823 != 0) {
        i = 0;
        do {
            GimmickSetData *set = &data_020d433c.mUnk_0c->mUnk_0c[mUnk_1a8->mUnk_824[i]];
            if (!set) {
                continue;
            }
            self = *(ObstacleBuf *) ((u8 *) mUnk_1a8 + 0x10);
            obs = *(ObstacleBuf *) OBST(set, mUnk_1a8->mUnk_82c[i]);
            switch (set->mUnk_08) {
                case 2: {
                    if (!set->mUnk_834) {
                        continue;
                    }
                    BattleUnit *unit = &data_020d433c.mBattlePlayers->mUnits[(u8) (set->mUnk_834 - 1)];
                    if (unit->mUnk_10 == 2 || mUnk_1df == 5) {
                        continue;
                    }
                    if (mUnk_1df == 6 && unit->mUnk_0f != mUnk_1d4 && unit->mUnk_0f != mUnk_1d5 && unit->mUnk_10 != 3) {
                        SoundManager::Get().func_020246f8(0x10);
                        func_ov007_02254a14(set);
                        continue;
                    }
                    if (mUnk_1df == 4 && unit->mUnk_0f != mUnk_1d4 && unit->mUnk_0f != mUnk_1d5 && unit->mUnk_10 != 3) {
                        mUnk_1dd = 3;
                        marker   = none;
                        continue;
                    }
                    if (mUnk_1e0 == 3 && unit->mUnk_10 != 3) {
                        VecFx32 v           = unit->mPos;
                        v.y                 = 0x8999;
                        mUnk_1a4->mPosition = v;
                        mUnk_64.x           = 0;
                        mUnk_64.y           = 0;
                        mUnk_64.z           = 0;
                        mUnk_a0.x           = 0;
                        mUnk_a0.y           = 0;
                        mUnk_a0.z           = 0;
                        mUnk_c8.x           = 0;
                        mUnk_c8.y           = 0;
                        mUnk_c8.z           = 0;
                        mUnk_ac.x           = 0;
                        mUnk_ac.y           = 0;
                        mUnk_ac.z           = 0;
                        continue;
                    }
                    *c = 0x666;
                    *d = 0x4cd;
                    VEC_Add(&self.o.mPos, &mUnk_1a8->mUnk_810, &self.o.mPos);
                    AddRotAtOnce(&self.o, mUnk_1a8);
                    VEC_Add(&obs.o.mPos, &set->mUnk_810, &obs.o.mPos);
                    AddRotAtOnceFields(&obs.o, set);
                    marker = 0;
                    if (data_020d433c.mUnk_85 == 0 && unit->mUnk_0f != mUnk_1d4 && unit->mUnk_0f != mUnk_1d5 && unit->mUnk_10 != 3 &&
                        (mUnk_1e1 != 0 || func_ov007_02254938(&self.o.mPos, &obs.o.mPos, a, ((RecordBits *) ((u8 *) record + 2))->mIndex))) {
                        mUnk_1dd = 6;
                        mUnk_1db = 0;
                        mUnk_15c = mUnk_1a4->mPosition;
                        mUnk_168 = obs.o.mPos;
                        mUnk_1de = set->mUnk_834;
                        mUnk_134 = *a;
                        mUnk_1cb = 1;
                    } else {
                        func_ov007_02257c80((BattleSphere *) &self.o, a, &obs.o, &hitOut, 0, 0);
                        SoundManager::Get().func_020246f8(4);
                    }
                    VEC_Add(&move, &hitOut, &move);
                    if (!(move.x == 0 && move.y == 0 && move.z == 0)) {
                        VEC_Normalize(&move, &move);
                    }
                    moved = 1;
                    break;
                }
                case 4: {
                    u8 kind = OBST(set, mUnk_1a8->mUnk_82c[i])->mUnk_1f;
                    switch (kind) {
                        case 0:
                        case 2: {
                            if (kind == 2) {
                                wallFree = 0;
                            }
                            if (kind == 2 && mUnk_1ce == 0) {
                                continue;
                            }
                            if (kind == 2) {
                                if (VEC_Mag(&mUnk_64) >= 0x2000) {
                                    *c = 0xccd;
                                } else {
                                    *c = 0x1333;
                                }
                            } else {
                                *c = 0x800;
                            }
                            *d = 0x4cd;
                            VEC_Add(&self.o.mPos, &mUnk_1a8->mUnk_810, &self.o.mPos);
                            AddRotAtOnce(&self.o, mUnk_1a8);
                            VEC_Add(&obs.o.mPos, &set->mUnk_810, &obs.o.mPos);
                            AddRotAtOnceFields(&obs.o, set);
                            vel.x = a->x;
                            vel.y = a->y;
                            vel.z = a->z;
                            vel.y = vel.y * (none - 0x10000);
                            if (OBST(set, mUnk_1a8->mUnk_82c[i])->mUnk_1f == 2) {
                                func_ov007_02257c80((BattleSphere *) &self.o, &vel, &obs.o, &move, 1, &hit);
                            } else {
                                func_ov007_02257c80((BattleSphere *) &self.o, &vel, &obs.o, &move, 0, &hit);
                            }
                            if (move.x != 0 || move.z != 0) {
                                SoundManager::Get().func_020246f8(4);
                            }
                            marker = 0;
                            if (move.y != 0 && OBST(set, mUnk_1a8->mUnk_82c[i])->mUnk_1f == 0) {
                                mUnk_58.y = set->mUnk_810.y + obs.o.mPos.y + obs.o.mHalfY + self.o.mHalfY;
                            }
                            if (OBST(set, mUnk_1a8->mUnk_82c[i])->mUnk_1f == 2) {
                                ceilingHit = 1;
                                if (move.x != 0 || move.z != 0) {
                                    handle = func_0202bd40(data_020d433c.mEffects, 1, 0xd, 0, 1);
                                    if (handle != 0) {
                                        CameraManager::Get();
                                        MtxFx43 mtx = data_020c1cf4;
                                        VecFx32 pos = mUnk_1a4->mPosition;
                                        func_01ffcf64(&mtx, &pos);
                                        func_0202bf60(data_020d433c.mEffects, handle, &mtx);
                                    }
                                }
                            }
                            break;
                        }
                        case 1:
                            mUnk_1cd = 1;
                            break;
                    }
                    break;
                }
                case 8:
                case 0x20:
                    *c = 0x4cd;
                    *d = 0x29;
                    vel.x = a->x;
                    vel.y = a->y;
                    vel.z = a->z;
                    func_ov007_02257f3c(set, mUnk_1a8->mUnk_82c[i], &vel, &move);
                    marker = 0;
                    break;
            }
        } while (++i < mUnk_1a8->mUnk_823);
    }
    if (wallFree != 0) {
        if (mUnk_1ce == 0) {
            mUnk_1ce = 1;
        }
    }
    if (ceilingHit != 0) {
        if (mUnk_1cd == 0) {
            mUnk_1cd = 1;
        }
    }
    if (a->y > 0 && move.y > 0) {
        move.y = 0;
    }
    if (!(move.x == 0 && move.y == 0 && move.z == 0)) {
        VecFx32 pos = mUnk_1a4->mPosition;
        mUnk_1e3    = 1;
        if (move.x != 0) {
            pos.x = mUnk_58.x;
        }
        s32 movedNow = moved;
        if (move.y != 0) {
            pos.y = mUnk_58.y;
        }
        if (move.z != 0) {
            pos.z = mUnk_58.z;
        }
        func_ov007_022337ec(&mUnk_64, &mUnk_a0, &mUnk_ac, &mUnk_b8, &move, *c, *d, movedNow);
        fx32 mag = VEC_Mag(&mUnk_a0);
        if (moved != 0 && mag <= 0x1000 && move.y != 0) {
            if (ABS(move.x) <= 0x800) {
                if (move.x < 0) {
                    mUnk_a0.x = -0x800;
                } else {
                    mUnk_a0.x = 0x800;
                }
            } else {
                mUnk_a0.x = move.x;
            }
            if (ABS(move.z) <= 0x800) {
                if (move.z < 0) {
                    mUnk_a0.z = -0x800;
                } else {
                    mUnk_a0.z = 0x800;
                }
            } else {
                mUnk_a0.z = move.z;
            }
            mUnk_a0.y += 0x1000;
        }
        if (move.y != 0 && marker != none) {
            mUnk_38 = none - 0x10000;
        }
        mUnk_1a4->mPosition = pos;
    }
    if (move.y != 0) {
        mUnk_148 = mUnk_58.y;
    }
    if (mUnk_1cd == 0) {
        mUnk_1d2 = 0;
        mUnk_1cb = 1;
        if (mUnk_1c6 == 0) {
            mUnk_1c6 = 1;
        }
    }
    *b = move;
}

void BattleTarget::func_ov007_02255640() {
    u32 outA = 0;
    u32 outB = 0;
    mUnk_90 = 0x533;
    if (func_ov007_022583b8()) {
        if (!func_ov007_02259af0()) {
            if (func_ov007_0225a278()) {
                return;
            }
            func_ov007_02254ae8(&mUnk_64, &mUnk_7c, &outA, &outB);
        }
        func_ov007_02255a9c();
        mUnk_1ca = 0;
        if (data_020d433c.mUnk_85 == 0 && mUnk_1dd != 4) {
            if ((mUnk_38 == 0 && mUnk_40 != -1) || mUnk_38 == -1) {
                if (mUnk_50 != 0) {
                    BattlePlayer *player = data_020d433c.mBattlePlayers->mPlayers[mUnk_1d4];
                    if (player->mUnk_dc != 0) {
                        u8 n = player->mUnk_e0;
                        if (n < 3) {
                            mUnk_a0.y += (3 - n) * 0x2666 / 3;
                            SoundManager::Get().func_020246f8(0x43);
                            n        = n + 1;
                            mUnk_1ca = 1;
                        }
                        data_020d433c.mBattlePlayers->mPlayers[mUnk_1d4]->mUnk_e0 = n;
                        VEC_Add(&mUnk_a0, &mUnk_ac, &mUnk_a0);
                        mUnk_ac.x = 0;
                        mUnk_ac.y = 0;
                        mUnk_ac.z = 0;
                    }
                    player = data_020d433c.mBattlePlayers->mPlayers[mUnk_1d4];
                    if (player->mUnk_e4 != 0) {
                        u8 n = player->mUnk_e8;
                        if (n < 3) {
                            mUnk_a0.y += (3 - n) * 0x2666 / 3;
                            SoundManager::Get().func_020246f8(0x43);
                            n        = n + 1;
                            mUnk_1ca = 1;
                        }
                        data_020d433c.mBattlePlayers->mPlayers[mUnk_1d4]->mUnk_e8 = n;
                        VEC_Add(&mUnk_a0, &mUnk_ac, &mUnk_a0);
                        mUnk_ac.x = 0;
                        mUnk_ac.y = 0;
                        mUnk_ac.z = 0;
                    }
                }
            }
        }
        UnkStruct_0203d374 *pose = mUnk_1a4;
        mUnk_58.x                = pose->mPosition.x;
        mUnk_58.y                = pose->mPosition.y;
        mUnk_58.z                = pose->mPosition.z;
        mUnk_70.x                = mUnk_64.x;
        mUnk_70.y                = mUnk_64.y;
        mUnk_70.z                = mUnk_64.z;
        mUnk_1d1                 = mUnk_1d0;
        mUnk_1d9                 = mUnk_1d8;
        mUnk_40                  = mUnk_38;
        if (mUnk_1dd != 6) {
            pose = mUnk_1a4;
            func_ov007_0225745c(&pose->mPosition, &pose->mRotX, &mUnk_64, &mUnk_a0, &mUnk_ac, &mUnk_38, mUnk_1ca, 0);
        }
    } else {
        UnkStruct_0203d374 *pose = mUnk_1a4;
        mUnk_58.x                = pose->mPosition.x;
        mUnk_58.y                = pose->mPosition.y;
        mUnk_58.z                = pose->mPosition.z;
    }
    func_ov007_02258448();
    func_ov007_022599fc();
    func_ov007_02255b0c();
    mUnk_3c = mUnk_38;
    mUnk_4c++;
    if (mUnk_50 != 0) {
        if (mUnk_1d0 != 0 && mUnk_4c % 30 == 0 && mUnk_1df != 2) {
            BattleRecord *record = &data_020d433c.mBattlePlayers->mPlayers[mUnk_1d4]->mRecords[mUnk_1d3];
            mUnk_8c              = FX_Mul(mUnk_8c, data_ov007_0227a6a8[record->mUnk_02_6]);
        }
        if (mUnk_1df != 2) {
            mUnk_50--;
        }
    } else {
        mUnk_44++;
        mUnk_8c = 0;
    }
}

void BattleTarget::func_ov007_02255a9c() {
    if (mUnk_1d9 != 0x33) {
        return;
    }
    if (mUnk_1d8 == 0x33) {
        return;
    }
    func_02024764(&SoundManager::Get(), 0xf, 0x3c);
}

void BattleTarget::func_ov007_02255b0c() {
    if (mUnk_38 != 0) {
        return;
    }
    if (mUnk_3c < 10) {
        return;
    }
    SoundManager::Get().func_020246f8(4);
}

extern const VecFx32 data_ov007_0227a704[];
extern const VecFx32 data_ov007_0227a728[];
extern const s32 data_ov007_0227a6e4[];
void BattleTarget::func_ov007_02255b78(VecFx32 *b, VecFx32 *c, VecFx32 *in, VecFx32 *out) {
    VecFx32 diff;
    MtxFx33 mtx;
    diff.x = 0;
    diff.z = 0;
    diff.y = 0;
    VEC_Subtract(c, b, &diff);
    int angle = -(FX_Atan2Idx(diff.z, diff.x) + 0x4000);
    mtx       = data_0220e848;
    func_01ffcfd8(&mtx, angle);
    func_01ff84d0(in, &mtx, out);
}




void BattleTarget::func_ov007_02255c04(VecFx32 *vec, s32 *out) {
    VecFx32 target;
    VecFx32 diff;
    VecFx32 rotated;
    MtxFx33 mtx = data_0220e848;
    Camera *camera = mUnk_174;
    target.x = camera->mTarget.x;
    target.y = camera->mTarget.y;
    target.z = camera->mTarget.z;
    VEC_Subtract(&mUnk_194, &target, &diff);
    int angle = -(FX_Atan2Idx(diff.x, diff.z) + 0x4000);
    func_01ffcfd8(&mtx, angle);
    func_01ff84d0(vec, &mtx, &rotated);
    out[1] = rotated.x;
    out[0] = rotated.z;
}

// non-matching: 6 words, case 0 only: instruction order inside the two SetVec(&cam, ...) arms (target == 0: the original adds pos.y + pose.y before loading pose.x; else: it loads pose.y before pos.y and adds z before y). Same instructions and registers; operand orders, temporaries and field-store orders tried.
void BattleTarget::func_ov007_02255cb4(VecFx32 *target) {
    VecFx32 pos;
    VecFx32 u;
    VecFx32 cam;
    VecFx32 curr;
    VecFx32 diff;
    MtxFx33 rot;
    MtxFx33 ident;
    VecFx32 v;
    MtxFx33 m0;
    m0    = data_0220e848;
    rot   = m0;
    ident = m0;
    BattlePlayer *player = data_020d433c.mBattlePlayers->mPlayers[mUnk_1d4];
    u8 step = mUnk_1e2;
    if (step >= 0xb) {
        return;
    }
    switch (mUnk_1e4) {
        case 0:
            if (step > 1) {
                mUnk_11c.x = mUnk_11c.x * 15 / 16;
                mUnk_11c.y = mUnk_11c.y * 15 / 16;
                mUnk_11c.z = mUnk_11c.z * 15 / 16;
                mUnk_1ba   = mUnk_1ba * 15 / 16;
            }
            switch (mUnk_1d2) {
                case 0:
                case 1: {
                    s32 k = mUnk_14c;
                    u.x   = k << 13;
                    u.y   = k * 0x333;
                    u.z   = k << 13;
                    VecFx32 tp;
                    tp.z = mUnk_11c.z + 0x37000 + (k << 13);
                    tp.y = u.y + (mUnk_11c.y + 0x19000);
                    tp.x = mUnk_11c.x + 0x37000 + (k << 13);
                    pos = tp;
                    func_01ffcfd8(&rot, (u16) (data_ov007_0227a5ee[player->mUnk_f1] - mUnk_1b8));
                    func_01ff84d0(&pos, &rot, &pos);
                    func_01ff84d0(&mUnk_188, &rot, &mUnk_188);
                    pos.x = FX_Mul(pos.x, ident._[4]);
                    pos.z = FX_Mul(pos.z, ident._[8]);
                    UnkStruct_0203d374 *pose = mUnk_1a4;
                    if (target == 0) {
                        SetVec(&cam, pos.x + pose->mPosition.x, pos.y + pose->mPosition.y + 0x4000, pos.z + pose->mPosition.z);
                    } else {
                        SetVec(&cam, target->x + pos.x, pos.y + pose->mPosition.y + 0x4000, target->z + pos.z);
                    }
                    curr = mUnk_194;
                    VEC_Subtract(&curr, &cam, &diff);
                    s32 scale = mUnk_184;
                    diff.x    = FX_Mul(diff.x, scale);
                    diff.y    = FX_Mul(diff.y, scale);
                    diff.z    = FX_Mul(diff.z, scale);
                    VEC_Add(&cam, &diff, &mUnk_194);
                    VecFx32 t;
                    t.z = mUnk_188.z + (cam.z + diff.z);
                    t.y = mUnk_188.y + (cam.y + diff.y);
                    t.x = mUnk_188.x + (cam.x + diff.x);
                    mUnk_174->mPosition = t;
                    return;
                }
                case 2:
                case 3: {
                    pos.x = 0x46000;
                    pos.y = 0x32000;
                    pos.z = 0x46000;
                    func_01ffcfd8(&rot, mUnk_1a4->mRotY);
                    func_01ff84d0(&pos, &rot, &pos);
                    if (target == 0) {
                        SetVec(&mUnk_174->mPosition, pos.x + mUnk_1a4->mPosition.x, pos.y, pos.z + mUnk_1a4->mPosition.z);
                    } else {
                        SetVec(&mUnk_174->mPosition, target->x + pos.x, pos.y, target->z + pos.z);
                    }
                    Camera *c = mUnk_174;
                    mUnk_194.x = c->mPosition.x;
                    mUnk_194.y = c->mPosition.y;
                    mUnk_194.z = c->mPosition.z;
                    return;
                }
                case 4: {
                    u8 slot = mUnk_1dc;
                    if (slot == 0) {
                        return;
                    }
                    if (mUnk_1df == 1) {
                        pos.x = data_ov007_0227a704[slot - 1].x;
                        pos.y = data_ov007_0227a704[slot - 1].y;
                        pos.z = data_ov007_0227a704[slot - 1].z;
                    } else {
                        pos.x = data_ov007_0227a728[slot - 1].x;
                        pos.y = data_ov007_0227a728[slot - 1].y;
                        pos.z = data_ov007_0227a728[slot - 1].z;
                    }
                    func_01ffcfd8(&rot, mUnk_1a4->mRotY);
                    func_01ff84d0(&pos, &rot, &pos);
                    if (target == 0) {
                        SetVec(&mUnk_174->mPosition, pos.x + mUnk_1a4->mPosition.x, pos.y, pos.z + mUnk_1a4->mPosition.z);
                    } else {
                        SetVec(&mUnk_174->mPosition, target->x + pos.x, pos.y, target->z + pos.z);
                    }
                    Camera *c = mUnk_174;
                    mUnk_194.x = c->mPosition.x;
                    mUnk_194.y = c->mPosition.y;
                    mUnk_194.z = c->mPosition.z;
                    return;
                }
                default:
                    break;
            }
            break;
        case 1: {
            VecFx32 tp;
            tp.z = mUnk_11c.z + 0x37000;
            tp.y = mUnk_11c.y + 0x19000;
            tp.x = mUnk_11c.x + 0x37000;
            pos = tp;
            func_01ffcfd8(&rot, (u16) (data_ov007_0227a5ee[player->mUnk_f1] - mUnk_1b8));
            func_01ff84d0(&pos, &rot, &pos);
            pos.x = FX_Mul(pos.x, ident._[4]);
            pos.z = FX_Mul(pos.z, ident._[8]);
            VecFx32 t;
            t.z = pos.z + mUnk_1a4->mPosition.z;
            t.y = pos.y + mUnk_1a4->mPosition.y + 0x4000;
            t.x = pos.x + mUnk_1a4->mPosition.x;
            cam = t;
            SetVec(&v, mUnk_1a4->mPosition.x, data_ov007_0227a6e4[data_020d433c.mUnk_95], mUnk_1a4->mPosition.z);
            if (v.x < 0) {
                v.x = v.x - 0x14000;
            } else {
                v.x = v.x + 0x14000;
            }
            if (v.z < 0) {
                v.z = v.z - 0x14000;
            } else {
                v.z = v.z + 0x14000;
            }
            func_ov007_02233ed8(&v, mUnk_1b0, &v);
            VEC_Subtract(&v, &cam, &diff);
            fx32 dz = FX_Div(diff.z, 0x14000);
            fx32 dy = FX_Div(diff.y, 0x14000);
            fx32 dx = FX_Div(diff.x, 0x14000);
            u.x     = dx;
            u.y     = dy;
            u.z     = dz;
            int scale = INT_TO_FX(mUnk_17c);
            u.x       = FX_Mul(u.x, scale);
            u.y       = FX_Mul(u.y, scale);
            u.z       = FX_Mul(u.z, scale);
            VEC_Add(&cam, &u, &cam);
            Camera *c      = mUnk_174;
            c->mPosition.x = cam.x;
            c->mPosition.y = cam.y;
            c->mPosition.z = cam.z;
            return;
        }
        case 2: {
            pos.x = -0x32000;
            pos.y = 0x5000;
            pos.z = 0x32000;
            func_01ffcfd8(&rot, mUnk_1a4->mRotY);
            func_01ff84d0(&pos, &rot, &pos);
            VecFx32 t;
            t.z = pos.z + mUnk_1a4->mPosition.z;
            t.y = pos.y;
            t.x = pos.x + mUnk_1a4->mPosition.x;
            mUnk_174->mPosition = t;
            return;
        }
        case 3: {
            if (mUnk_64.x == 0 && mUnk_64.z == 0) {
                return;
            }
            pos.x = 0x37000;
            pos.y = 0xf000;
            pos.z = 0x37000;
            func_01ffcfd8(&rot, (u16) (FX_Atan2Idx(mUnk_64.x, mUnk_64.z) - 0xa000));
            func_01ff84d0(&pos, &rot, &pos);
            Camera *c = mUnk_174;
            cam.x     = c->mPosition.x;
            cam.y     = c->mPosition.y;
            cam.z     = c->mPosition.z;
            VEC_Add(&pos, &mUnk_1a4->mPosition, &pos);
            VEC_Subtract(&cam, &pos, &u);
            s32 scale = mUnk_184;
            u.x       = FX_Mul(u.x, scale);
            u.y       = FX_Mul(u.y, scale);
            u.z       = FX_Mul(u.z, scale);
            SetVec(&mUnk_174->mPosition, cam.x - u.x, cam.y - u.y, cam.z - u.z);
            return;
        }
        case 4: {
            if (mUnk_70.x == 0 && mUnk_70.y == 0 && mUnk_70.z == 0) {
                return;
            }
            pos.x = 0x14000;
            pos.y = 0x14000;
            pos.z = 0x14000;
            func_01ffcfd8(&rot, (u16) (FX_Atan2Idx(mUnk_70.x, mUnk_70.z) - 0xa000));
            s32 a = FX_Atan2Idx(mUnk_70.y, 0x1000);
            func_01ff84d0(&pos, &rot, &pos);
            func_01ff84d0(&mUnk_188, &rot, &mUnk_188);
            pos.y = FX_Mul(pos.y, -FX_SinCosTable_[(a >> 4) * 2]) + 0x5a000;
            cam.x = mUnk_194.x;
            cam.y = mUnk_194.y;
            cam.z = mUnk_194.z;
            VEC_Add(&pos, &mUnk_1a4->mPosition, &pos);
            VEC_Subtract(&cam, &pos, &u);
            s32 scale = mUnk_184;
            u.x       = FX_Mul(u.x, scale);
            u.y       = FX_Mul(u.y, scale);
            u.z       = FX_Mul(u.z, scale);
            SetVec(&mUnk_194, cam.x - u.x, mUnk_1a4->mPosition.y + 0x5a000, cam.z - u.z);
            VEC_Add(&cam, &mUnk_188, &cam);
            SetVec(&mUnk_174->mPosition, cam.x - u.x, mUnk_1a4->mPosition.y + 0x5a000, cam.z - u.z);
            return;
        }
        case 5:
            func_ov007_022567ac();
            return;
        case 6:
            return;
    }
}

void BattleTarget::func_ov007_022567ac() {
    VecFx32 c;
    VecFx32 b;
    VecFx32 a;
    VecFx32 dir;
    MtxFx33 mtx = data_0220e848;
    switch (mUnk_1e6) {
        case 0: {
            mUnk_184 = 0x333;
            a.x     = mUnk_194.x;
            a.y     = mUnk_194.y;
            a.z     = mUnk_194.z;
            VecFx32 t;
            t.z = -mUnk_64.z;
            t.y = -mUnk_64.y;
            t.x = -mUnk_64.x;
            dir = t;
            if (VEC_Mag(&dir) == 0) {
                dir.x = -1;
                dir.y = -1;
                dir.z = -1;
            }
            VEC_Normalize(&dir, &dir);
            b = mUnk_1a4->mPosition;
            b.x += FX_Mul(dir.x, 0x28000);
            b.y += FX_Mul(dir.y, 0x28000);
            b.z += FX_Mul(dir.z, 0x28000);
            VEC_Subtract(&a, &b, &c);
            VecFx32 t0;
            t0.z = FX_Mul(c.z, mUnk_184);
            t0.y = FX_Mul(c.y, mUnk_184);
            t0.x = FX_Mul(c.x, mUnk_184);
            b = t0;
            VecFx32 t1;
            t1.z = a.z - b.z;
            t1.y = a.y - b.y;
            t1.x = a.x - b.x;
            mUnk_174->mPosition = t1;
            VecFx32 t2;
            t2.z = a.z - b.z;
            t2.y = a.y - b.y;
            t2.x = a.x - b.x;
            mUnk_194 = t2;
            mUnk_17c--;
            if (mUnk_17c == 0) {
                mUnk_1e6++;
            }
            break;
        }
        case 1: {
            mUnk_184 = 0x148;
            a.x = mUnk_194.x;
            a.y = mUnk_194.y;
            a.z = mUnk_194.z;
            VEC_Subtract(&mUnk_1a4->mPosition, &a, &b);
            if (VEC_Mag(&b) >= 0x5f000) {
                mUnk_1e6++;
                mUnk_1e5 = 1;
            }
            break;
        }
        case 2: {
            mUnk_184 = 0x148;
            a.x = mUnk_194.x;
            a.y = mUnk_194.y;
            a.z = mUnk_194.z;
            VEC_Subtract(&mUnk_1a4->mPosition, &a, &b);
            c.x = 0x2f800;
            c.y = 0x2f800;
            c.z = 0x2f800;
            func_01ffcfd8(&mtx, FX_Atan2Idx(mUnk_64.x, mUnk_64.z) - 0xa000);
            func_01ff84d0(&c, &mtx, &c);
            func_01ff84d0(&mUnk_188, &mtx, &mUnk_188);
            u16 pitch = FX_Atan2Idx(mUnk_64.y, 0x1000);
            c.y       = FX_Mul(c.y, -FX_SinCosTable_[(pitch >> 4) * 2]);
            VEC_Add(&c, &mUnk_1a4->mPosition, &c);
            VEC_Add(&c, &data_ov007_0227a624, &c);
            VEC_Subtract(&a, &c, &b);
            fx32 k = mUnk_184;
            b.x = FX_Mul(b.x, k);
            b.y = FX_Mul(b.y, k);
            b.z = FX_Mul(b.z, k);
            SetVec(&mUnk_194, a.x - b.x, a.y - b.y, a.z - b.z);
            VEC_Subtract(&b, &mUnk_188, &b);
            VecFx32 t;
            t.z = a.z - b.z;
            t.y = a.y - b.y;
            t.x = a.x - b.x;
            mUnk_174->mPosition = t;
            break;
        }
        case 3:
        case 4:
        case 5:
        case 6:
            break;
    }
}

void BattleTarget::func_ov007_02256c4c(VecFx32 *target) {
    VecFx32 cam;
    VecFx32 diff;
    VecFx32 move;
    MtxFx33 mtx;
    if (mUnk_1e2 >= 0xb) {
        return;
    }
    switch (mUnk_1e4) {
        case 0:
            if (target == 0) {
                target = &mUnk_1a4->mPosition;
            }
            if (mUnk_1e2 > 1) {
                mtx    = data_0220e848;
                move.x = 0;
                move.y = 0;
                move.z = 0;
                diff.x = 0;
                diff.y = 0;
                diff.z = 0;
                Camera *camera = mUnk_174;
                cam.x          = camera->mPosition.x;
                cam.y          = camera->mPosition.y;
                cam.z          = camera->mPosition.z;
                VEC_Subtract(&cam, &mUnk_1a4->mPosition, &diff);
                int angle = -(FX_Atan2Idx(diff.z, diff.x) + 0x4000);
                func_01ffcfd8(&mtx, angle);
                func_01ff84d0(&move, &mtx, &move);
                if (mUnk_1d6 != 0) {
                    mUnk_1d6--;
                }
                mUnk_128.x = (target->x + mUnk_128.x * mUnk_1d6) / (mUnk_1d6 + 1);
                mUnk_128.y = (target->y + mUnk_128.y * mUnk_1d6) / (mUnk_1d6 + 1);
                mUnk_128.z = (target->z + mUnk_128.z * mUnk_1d6) / (mUnk_1d6 + 1);
                mUnk_128.x += move.x;
                mUnk_128.z += move.z;
                mUnk_128.y += move.y;
                Camera *cameraOut = mUnk_174;
                cameraOut->mTarget.x = mUnk_128.x;
                cameraOut->mTarget.y = mUnk_128.y;
                cameraOut->mTarget.z = mUnk_128.z;
                return;
            }
            if (mUnk_1e2 != 1) {
                return;
            }
            VecFx32 off0 = {0, 0, 0};
            off0.x = -mUnk_1a4->mPosition.x;
            off0.z = -mUnk_1a4->mPosition.z;
            func_ov007_02233ed8(&off0, -mUnk_1b8, &mUnk_128);
            VEC_Add(&mUnk_1a4->mPosition, &mUnk_128, &mUnk_128);
            mUnk_128.y = FX_Mul(FX_Div(INT_TO_FX(mUnk_1ba), 0xb6000), 0x2000);
            Camera *c = mUnk_174;
            c->mTarget.x = mUnk_128.x;
            c->mTarget.y = mUnk_128.y;
            c->mTarget.z = mUnk_128.z;
            return;
        case 1: {
            VecFx32 off1 = {0, 0, 0};
            VecFx32 v;
            off1.x = -mUnk_1a4->mPosition.x;
            off1.z = -mUnk_1a4->mPosition.z;
            func_ov007_02233ed8(&off1, -mUnk_1b8, &mUnk_128);
            VEC_Add(&mUnk_1a4->mPosition, &mUnk_128, &mUnk_128);
            mUnk_128.y = FX_Mul(FX_Div(INT_TO_FX(mUnk_1ba), 0xb6000), 0x2000);
            v.x = mUnk_128.x;
            v.y = mUnk_128.y;
            v.z = mUnk_128.z;
            fx32 vz = FX_Div(v.z, 0x14000);
            fx32 vy = FX_Div(v.y, 0x14000);
            fx32 vx = FX_Div(v.x, 0x14000);
            v.x     = vx;
            v.y     = vy;
            v.z     = vz;
            int scale = INT_TO_FX(mUnk_17c);
            v.x       = FX_Mul(v.x, scale);
            v.y       = FX_Mul(v.y, scale);
            v.z       = FX_Mul(v.z, scale);
            VEC_Subtract(&mUnk_128, &v, &mUnk_128);
            Camera *c = mUnk_174;
            c->mTarget.x = mUnk_128.x;
            c->mTarget.y = mUnk_128.y;
            c->mTarget.z = mUnk_128.z;
            return;
        }
        case 2: {
            UnkStruct_0203d374 *pose = mUnk_1a4;
            SetVec(&mUnk_174->mTarget, pose->mPosition.x, mUnk_180 + pose->mPosition.y, pose->mPosition.z);
            mUnk_180                 = FX_Mul(mUnk_180, -0xf33);
            return;
        }
        case 3: {
            UnkStruct_0203d374 *pose = mUnk_1a4;
            SetVec(&mUnk_174->mTarget, pose->mPosition.x, pose->mPosition.y, pose->mPosition.z);
            return;
        }
        case 4: {
            UnkStruct_0203d374 *pose = mUnk_1a4;
            SetVec(&mUnk_174->mTarget, pose->mPosition.x, pose->mPosition.y + 0x64cd, pose->mPosition.z);
            return;
        }
        case 5: {
            UnkStruct_0203d374 *pose = mUnk_1a4;
            SetVec(&mUnk_174->mTarget, pose->mPosition.x, pose->mPosition.y + 0x64cd, pose->mPosition.z);
            mUnk_128 = mUnk_174->mTarget;
            return;
        }
        case 6:
            return;
    }
}

void BattleTarget::func_ov007_022571d4() {
    func_ov007_02255cb4(0);
    func_ov007_02256c4c(0);
    SnapshotCamera();
}

void BattleTarget::func_ov007_022573e4() {
    data_020d433c.mUnk_14 &= 8;
    if (data_020d433c.mUnk_14 == 0) {
        func_ov007_022571d4();
    }
    if (mUnk_28) {
        mUnk_28->func_ov007_02235480();
        func_ov007_0225cb7c();
    }
    if (mUnk_1e2 != 0xd && mUnk_1e2 != 0xe) {
        func_ov007_0225ca48(0xff, 0xff);
    }
    if (mUnk_1a4) {
        mUnk_1a4->func_0203dab0();
    }
}

void BattleTarget::func_ov007_0225745c(VecFx32 *pos, u16 *rot, VecFx32 *vel, VecFx32 *a, VecFx32 *b, u32 *counter, u8 flag,
                                       u32 mode) {
    float angle = 0;
    VecFx32 tmp = {0, 0, 0};
    (*counter)++;
    int speed = 0;
    if (mode == 0) {
        fx32 len;
        if (VEC_Mag(b) > 0x800 || VEC_Mag(a) <= 0) {
            len = FX_Atan2(b->x, b->z);
            speed = ABS(b->z) + ABS(b->x);
        } else {
            len = FX_Atan2(vel->x, vel->z);
            speed = ABS(vel->z) + ABS(vel->x);
        }
        fx32 q = FX_Div(len, 0x47);
        angle  = (float) q / 4096.0f;
        if (angle < 0.0f) {
            angle += 360.0f;
        }
    }
    func_ov007_0225bf70(&mUnk_ac, &mUnk_c8);
    VEC_Add(&mUnk_ac, &mUnk_c8, &mUnk_ac);
    mUnk_c8.x = 0;
    mUnk_c8.y = 0;
    mUnk_c8.z = 0;
    if (flag == 0) {
        if (pos->y >= 0x899a && *counter != 0) {
            goto airborne;
        }
        if (mUnk_1cd != 0 || mUnk_1e9 != 0) {
            b->y = 0;
            if (a->y < 0) {
                a->y = FX_Mul(a->y, -mUnk_90);
            }
            if (ABS(a->y) <= 0x19a) {
                a->y = 0;
            }
            if (ABS(a->x) <= 0xcd) {
                a->x = 0;
            } else {
                a->x = FX_Mul(a->x, 0xf85);
            }
            if (ABS(a->z) <= 0xcd) {
                a->z = 0;
            } else {
                a->z = FX_Mul(a->z, 0xf85);
            }
            if (mUnk_4c % 5 == 0) {
                if (ABS(b->x) <= 0xcd) {
                    b->x = 0;
                } else {
                    b->x = FX_Mul(b->x, 0xecd);
                }
                if (ABS(b->z) <= 0xcd) {
                    b->z = 0;
                } else {
                    b->z = FX_Mul(b->z, 0xecd);
                }
            }
            if (ABS(mUnk_b8.x) <= 0xcd) {
                mUnk_b8.x = 0;
            } else {
                mUnk_b8.x = FX_Mul(mUnk_b8.x, 0xfae);
            }
            if (ABS(mUnk_b8.y) <= 0xcd) {
                mUnk_b8.y = 0;
            } else {
                mUnk_b8.y = FX_Mul(mUnk_b8.y, 0xfae);
            }
            if (ABS(mUnk_b8.z) <= 0xcd) {
                mUnk_b8.z = 0;
            } else {
                mUnk_b8.z = FX_Mul(mUnk_b8.z, 0xfae);
            }
            if (pos->y < 0x899a) {
                mUnk_148 = 0x899a;
                pos->y   = 0x8999;
            }
            *counter = 0;
        } else {
            a->y = a->y - mUnk_c4;
        }
        if (a->y > 0x29) {
            VecFx32 vt;
            vt.z = a->z + FX_Mul(b->z, 0x1000);
            vt.y = a->y + FX_Mul(b->y, 0x1000);
            vt.x = a->x + FX_Mul(b->x, 0x1000);
            *vel = vt;
        } else {
            VecFx32 vt;
            vt.z = a->z + b->z;
            vt.y = a->y + b->y;
            vt.x = a->x + b->x;
            *vel = vt;
        }
        mUnk_1d0 = 1;
    } else {
    airborne:
        a->y = a->y - mUnk_c4;
        switch (mUnk_1cc) {
            case 0:
                { VecFx32 vt; vt.z = a->z; vt.y = a->y; vt.x = a->x; *vel = vt; }
                break;
            case 1:
                VecFx32 vt;
                vt.z = a->z + b->z;
                vt.y = a->y + b->y;
                vt.x = a->x + b->x;
                *vel = vt;
                break;
        }
    }
    VEC_Add(vel, &mUnk_b8, vel);
    if (b->x != 0 || b->z != 0 || a->x != 0 || a->y != 0 || a->z != 0 || mUnk_b8.x != 0 || mUnk_b8.z != 0) {
        if (mode == 0) {
            fx32 q = FX_Div(INT_TO_FX(speed), 0x5dc000);
            q      = FX_Mul(q, 0x280000);
            rot[0] = rot[0] + (fx32) ((float) q / 4096.0f);
            rot[1] = (u32) (angle * 65535.0f / 360.0f);
        }
        if (a->y != -mUnk_c4) {
            mUnk_48 = 0;
        } else {
            mUnk_48++;
        }
    } else {
        mUnk_48++;
        if (mUnk_48 >= 0x28) {
            vel->x = 0;
            vel->y = 0;
            vel->z = 0;
            a->x   = 0;
            a->y   = 0;
            a->z   = 0;
            b->x   = 0;
            b->y   = 0;
            b->z   = 0;
            if (mUnk_1d2 == 0) {
                mUnk_1d2 = 1;
            }
            mUnk_1c6 = 1;
            mUnk_48  = 0;
        }
    }
    BattlePlayer *player = data_020d433c.mBattlePlayers->mPlayers[mUnk_1d4];
    BOOL charged         = FALSE;
    if (player->mUnk_dc != 0) {
        if ((u8) player->mUnk_e0 < 3) {
            charged = TRUE;
        }
    }
    if (player->mUnk_e4 != 0) {
        if ((u8) player->mUnk_e8 < 3) {
            charged = TRUE;
        }
    }
    if (mUnk_1dd != 4 && data_020d433c.mUnk_85 == 0 && charged) {
        s32 sum = data_020d433c.mBattlePlayers->mPlayers[mUnk_1d4]->mUnk_dc + data_020d433c.mBattlePlayers->mPlayers[mUnk_1d4]->mUnk_e4;
        if (sum != 0) {
            tmp.x = vel->x;
            tmp.z = vel->z;
            fx32 m = VEC_Mag(&tmp);
            if (m > 0) {
                if (sum > 0) {
                    if (sum > m) {
                        sum = m;
                    }
                } else if (sum < 0) {
                    if (-sum > m) {
                        sum = -m;
                    }
                }
                VEC_Normalize(&tmp, &tmp);
                tmp.x = FX_Mul(tmp.x, sum);
                tmp.y = FX_Mul(tmp.y, sum);
                tmp.z = FX_Mul(tmp.z, sum);
            }
        }
    }
    pos->y += vel->y + tmp.y;
    pos->x += vel->x + tmp.x;
    pos->z += vel->z + tmp.z;
}

BOOL BattleTarget::func_ov007_02257c80(BattleSphere *sphere, VecFx32 *dir, BattleObstacle *obj, VecFx32 *out, u8 flag, u8 *outFlag) {
    BattleHitBox box;
    VecFx32 center = sphere->mPos;
    BattleSphere ball;
    u8 tries = 0;
    switch (obj->mType) {
        case 0:
            do {
                box.mPos.x = obj->mPos.x;
                box.mPos.y = obj->mPos.y;
                box.mPos.z = obj->mPos.z;
                MI_CpuClear32(box.pad_0x0c, 0x24);
                box.mHalfX = obj->mHalfX;
                box.mHalfY = obj->mHalfY;
                box.mHalfZ = obj->mHalfZ;
                box.mRotX  = obj->mRotX;
                box.mRotY  = obj->mRotY;
                box.mRotZ  = obj->mRotZ;
                SetSphere(&ball, &center, sphere->mRadius);
                if (func_ov007_0225ac28(&box, &ball, dir, out, flag, outFlag)) {
                    return 1;
                }
                if (outFlag == 0) {
                    return 0;
                }
                center.x = sphere->mPos.x;
                center.y = sphere->mPos.y;
                center.z = sphere->mPos.z;
                center.x -= (tries + 1) * ((center.x - mUnk_58.x) * 2 / 10);
                center.z -= (tries + 1) * ((center.z - mUnk_58.z) * 2 / 10);
                tries++;
                sphere->mPos.x = center.x;
                sphere->mPos.y = center.y;
                sphere->mPos.z = center.z;
            } while (tries < 10);
            return 0;
        case 1: {
            BattleSphere a;
            BattleSphere b;
            SetSphere(&a, &obj->mPos, obj->mHalfX);
            SetSphere(&b, &sphere->mPos, sphere->mRadius);
            func_ov007_02233f38(&b.mPos, &a.mPos, (int) dir, out);
            return 1;
        }
        case 4: {
            BattleSphere a;
            BattleSphere b;
            SetSphere(&a, &obj->mPos, obj->mHalfX);
            SetSphere(&b, &center, sphere->mRadius);
            fx32 top = obj->mPos.y + obj->mHalfY;
            if (top >= center.y && obj->mPos.y - obj->mHalfY <= center.y) {
                func_ov007_02233f38(&b.mPos, &a.mPos, (int) dir, out);
                out->y = 0;
            } else if (top <= center.y) {
                out->y = -0x1000;
            }
            return 1;
        }
    }
    return tries;
}


void BattleTarget::func_ov007_02257f3c(GimmickSetData *set, u32 index, VecFx32 *vel, VecFx32 *out) {
    fx32 mag = VEC_Mag(vel);
    BattleObstacle self = *(BattleObstacle *) ((u8 *) mUnk_1a8 + 0x10);
    BattleObstacle *selfp = &self;
    BattleObstacle other = *(BattleObstacle *) ((u8 *) set + 0x10 + index * 0x20);
    BattleRecord *record = &data_020d433c.mBattlePlayers->mPlayers[mUnk_1d4]->mRecords[mUnk_1d3];
    mUnk_1d8 = set->mUnk_834;
    switch (set->mUnk_834) {
        case 0:
        case 1:
        case 2:
        case 3:
        case 5:
        case 7:
            break;
        case 6:
            VEC_Add(&selfp->mPos, &mUnk_1a8->mUnk_810, &selfp->mPos);
            AddRotAtOnce(&self, mUnk_1a8);
            VEC_Add(&other.mPos, &set->mUnk_810, &other.mPos);
            AddRotAtOnceFields(&other, set);
            func_ov007_02257c80((BattleSphere *) &self, vel, &other, out, 0, 0);
            if (out->x != 0 || out->z != 0) {
                SoundManager::Get().func_020246f8(4);
            }
            if (out->y > 0) {
                if (mUnk_38 > 10) {
                    SoundManager::Get().func_020246f8(4);
                }
                mUnk_58.y = other.mPos.y + other.mHalfY + self.mHalfY;
            }
            break;
        case 4:
            if (mUnk_1e0 == 3) {
                mUnk_1e0 = 0;
            }
            break;
        case 0x46:
            if (record->mUnk_09 == 3) {
                return;
            }
            if (mUnk_1dd != 4) {
                return;
            }
            if (mUnk_1db >= 2) {
                return;
            }
            mUnk_140 = 0x19;
            mUnk_1db = 2;
            mUnk_c4  = 0x1dc - 0x19;
            break;
        case 0x32:
            if (record->mUnk_09 != 5) {
                mUnk_144 = &set->mUnk_810;
                mUnk_1dd = 1;
            }
            if (mUnk_1e0 == 3) {
                mUnk_1e0 = 0;
            }
            break;
        case 0x33:
            if (mUnk_1d9 == 0x33) {
                return;
            }
            if (mUnk_1d8 != 0x33) {
                return;
            }
            SoundManager::Get().func_020246f8(0xf);
            break;
        case 0x3c:
            VEC_Add(&selfp->mPos, &mUnk_1a8->mUnk_810, &selfp->mPos);
            AddRotAtOnce(&self, mUnk_1a8);
            VEC_Add(&other.mPos, &set->mUnk_810, &other.mPos);
            AddRotAtOnceFields(&other, set);
            if (mag < 0x6800) {
                func_ov007_02257c80((BattleSphere *) &self, vel, &other, out, 0, 0);
            }
            break;
        case 0x47:
            mUnk_1cd = 1;
            break;
    }
}

BOOL BattleTarget::func_ov007_022583b8() {
    switch (mUnk_1dd) {
        case 0:
            return 1;
        case 1:
            return mUnk_1db == 0;
        case 2:
            return 1;
        case 3:
            return 0;
        case 4:
            return 1;
        case 5:
            return 0;
        case 6:
            return 0;
        case 7:
            return 0;
        case 8:
            return 0;
        default:
            return 1;
    }
}

void BattleTarget::func_ov007_02258448() {
    if (mUnk_1bc != 0) {
        mUnk_1bc--;
    }
    switch (mUnk_1dd) {
        case 0:
            if (mUnk_14c != 0) {
                mUnk_14c--;
            }
            mUnk_1e4 = 0;
            mUnk_1cc = 0;
            mUnk_c4  = 0x1c3;
            mUnk_184 = 0;
            mUnk_1db = 0;
            break;
        case 1:
            func_ov007_02259494(mUnk_144);
            break;
        case 2:
            func_ov007_022585e0();
            break;
        case 3:
            func_ov007_02258edc();
            mUnk_184 = 0x19a;
            break;
        case 4:
            func_ov007_022593b4();
            break;
        case 5: {
            VecFx32 pos = mUnk_1a4->mPosition;
            mUnk_1cd    = 0;
            mUnk_c4     = 0x29;
            mUnk_1e4    = 6;
            mUnk_64.y -= mUnk_c4;
            pos.y += mUnk_64.y;
            UnkStruct_0203d374 *pose = mUnk_1a4;
            pose->mPosition.x        = pos.x;
            pose->mPosition.y        = pos.y;
            pose->mPosition.z        = pos.z;
            break;
        }
        case 6:
            mUnk_1e4 = 0;
            mUnk_1d2 = 4;
            data_020d433c.mGimmicks->mState = 0;
            func_ov007_02258660();
            break;
        case 7:
            func_ov007_0225897c();
            break;
        case 8:
            func_ov007_02258a34();
            break;
    }
}

void BattleTarget::func_ov007_022585e0() {
    switch (mUnk_1db) {
        case 0:
            mUnk_1e4 = 4;
            mUnk_1cc = 1;
            mUnk_184 = 0x19a;
            break;
        case 1:
            mUnk_1e4 = 2;
            mUnk_184 = 0;
            mUnk_1cc = 0;
            mUnk_1dd = 0;
            mUnk_1db = 0;
            break;
        case 2:
            mUnk_184 = 0;
            mUnk_1e4 = 0;
            mUnk_1d6 = 0;
            mUnk_1cc = 0;
            mUnk_1dd = 0;
            mUnk_1db = 0;
            break;
    }
}

void BattleTarget::func_ov007_02258660() {
    switch (mUnk_1db) {
        case 0: {
            mUnk_1db++;
            func_ov007_02258cec(&mUnk_15c, &mUnk_168, &mUnk_134, mUnk_1de);
            if (mUnk_1df == 1) {
                mUnk_140 = data_ov007_0227a5e0[mUnk_1dc];
            } else {
                mUnk_140 = data_ov007_0227a5e4[mUnk_1dc];
            }
            mUnk_64.x = 0;
            mUnk_64.y = 0;
            mUnk_64.z = 0;
            mUnk_a0.x = 0;
            mUnk_a0.y = 0;
            mUnk_a0.z = 0;
            mUnk_ac.x = 0;
            mUnk_ac.y = 0;
            mUnk_ac.z = 0;
            mUnk_b8.x = 0;
            mUnk_b8.y = 0;
            mUnk_b8.z = 0;
            SoundManager::Get().func_020246f8(0x3c);
            mUnk_1c2 = func_0202bd40(data_020d433c.mEffects, 1, 1, 0, 0);
            if (mUnk_1c2 != 0) {
                MtxFx43 mtx = CameraManager::Get().mMtxC[0];
                mtx._[11]   = 0;
                mtx._[10]   = 0;
                mtx._[9]    = 0;
                func_01ffcf64(&mtx, &mUnk_1a4->mPosition);
                func_0202bf60(data_020d433c.mEffects, mUnk_1c2, &mtx);
            }
            mUnk_1dc++;
            if (mUnk_1dc <= 2) {
                return;
            }
            BattlePlayerList *list = data_020d433c.mBattlePlayers;
            u8 a                   = 0;
            u8 b                   = 0;
            mUnk_1db               = 2;
            mUnk_140               = 0;
            mUnk_1cf               = 1;
            mUnk_1c6               = 1;
            func_ov007_02248998(&mUnk_168, &a, &b);
            mUnk_1e8 = func_ov007_02262768(list, &mUnk_1a4->mPosition, mUnk_1a4->mRotY, mUnk_1d4, mUnk_1d3,
                                           list->mUnk_16c[a][b] - 1, 1);
            func_ov007_02262a64(list, mUnk_1de - 1, *(u8 *) &list->mUnk_1f8[a][b] - 1);
            new BattleCardEffect(0x1000, 0, 1, 0);
            break;
        }
        case 1:
            mUnk_140--;
            if (mUnk_140 == 0) {
                mUnk_1db = 0;
                func_02028e54(mUnk_1a4->mModel, 0, 0, 0x1000);
            }
            break;
        case 2:
            break;
    }
}

void BattleTarget::func_ov007_0225897c() {
    switch (mUnk_1db) {
        case 0:
            mUnk_1a4->mUnk_4a = 0;
            mUnk_140          = 0x1e;
            mUnk_1db++;
            break;
        case 1:
            mUnk_140--;
            if (mUnk_140 == 0) {
                mUnk_1db++;
            }
            break;
        case 2:
            mUnk_1a4->mUnk_4a = 1;
            mUnk_1db          = 0;
            mUnk_1dd          = 0;
            if (mUnk_1e0 == 3) {
                mUnk_1dd = 2;
            }
            BattleCpu *fighter = data_020d433c.mBattlePlayers->mPlayers[mUnk_1d4]->mUnk_c8;
            if (fighter) {
                fighter->mUnk_74 = 0x3c;
            }
            break;
    }
}

void BattleTarget::func_ov007_02258a34() {
    switch (mUnk_1db) {
        case 0: {
            mUnk_1e4 = 6;
            if (mUnk_1e0 == 3) {
                mUnk_1e0 = 0;
            }
            mUnk_1db++;
            u16 handle = func_0202bd40(data_020d433c.mEffects, 1, 5, 0, 0);
            if (handle == 0) {
                return;
            }
            MtxFx43 mtx = CameraManager::Get().mMtxC[0];
            VecFx32 a   = data_020d433c.mBattlePlayers->mUnk_298->mUnk_1a4->mPosition;
            VecFx32 b   = data_ov007_0227a60c;
            mtx._[11]   = 0;
            mtx._[10]   = 0;
            mtx._[9]    = 0;
            MTX_MultVec43(&b, &mtx, &b);
            VEC_Add(&a, &b, &a);
            func_01ffcf64(&mtx, &a);
            func_0202bf60(data_020d433c.mEffects, handle, &mtx);
            break;
        }
        case 1: {
            mUnk_140--;
            if (mUnk_140 == 0) {
                mUnk_1db++;
            }
            UnkStruct_0203d374 *pose = mUnk_1a4;
            VecFx32 pos              = pose->mPosition;
            if ((mUnk_140 & 1) == 0) {
                pos.y += 0x800;
            } else {
                pos.y -= 0x800;
            }
            pose->SetPosition(&pos);
            break;
        }
        case 2: {
            mUnk_1db = 0;
            mUnk_1dd = 0;
            mUnk_1bc = 0x3c;
            SoundManager::Get().func_020246f8(0x60);
            BattlePlayer *player = data_020d433c.mBattlePlayers->mPlayers[mUnk_1d4];
            u8 record            = player->mRecord;
            int v = player->mRecords[record].mUnk_04;
            if (v > 0x32) {
                player->mRecords[record].mUnk_04 -= 0x32;
                return;
            }
            int amount                       = (s16) -((u8) (0x32 - player->mRecords[record].mUnk_04) + 10);
            player->mRecords[record].mUnk_04 = 10;
            func_ov007_0226a380(player, record, amount);
            break;
        }
    }
}

BOOL BattleTarget::func_ov007_02258cec(VecFx32 *a, VecFx32 *b, VecFx32 *c, u8 d) {
    BattleMoveState state;
    u8 y = 0;
    u8 x = 0;
    state.mVelocity.x = 0;
    state.mVelocity.y = 0;
    state.mVelocity.z = 0;
    state.mRot[0]     = 0;
    state.mRot[1]     = 0;
    state.mRot[2]     = 0;
    state.mMode       = 2;
    BattlePlayerList *list   = data_020d433c.mBattlePlayers;
    UnkStruct_0203d374 *pose = mUnk_1a4;
    u16 rotY                 = pose->mRotY;
    u16 rotZ                 = pose->mRotZ;
    func_ov007_02248998(b, &y, &x);
    UnkStruct_0203d374 *other = list->mUnk_29c->func_ov007_0226fc4c(d - 1);
    if (other) {
        VecFx32 tmp = {0, 0, 0};
        func_ov007_02233730(a, b, c, &tmp);
        pose              = mUnk_1a4;
        pose->mPosition.x = b->x;
        pose->mPosition.y = b->y;
        pose->mPosition.z = b->z;
        mUnk_58.x         = b->x;
        mUnk_58.y         = b->y;
        mUnk_58.z         = b->z;
        pose              = mUnk_1a4;
        pose->mRotX       = 0;
        pose->mRotY       = rotY;
        pose->mRotZ       = rotZ;
        mUnk_1a4->mUnk_49 = 1;
        mUnk_1d2          = 4;
        other->mPosition.x = b->x;
        other->mPosition.y = b->y;
        other->mPosition.z = b->z;
        pose              = mUnk_1a4;
        u16 r0            = pose->mRotX;
        u16 r1            = pose->mRotY;
        u16 r2            = pose->mRotZ;
        state.mVelocity = tmp;
        other->mRotX      = r0 + 0x4000;
        other->mRotY      = r1;
        other->mRotZ      = r2;
        list->mUnk_29c->func_ov007_02270ae0(d - 1, &state);
        func_ov007_02256c4c(&mUnk_1a4->mPosition);
        func_ov007_02255cb4(&mUnk_1a4->mPosition);
        if (mUnk_1c0 != 0) {
            func_0202beb8(data_020d433c.mEffects, mUnk_1c0);
            mUnk_1c0 = 0;
        }
        BattleStatusPanels *panels = mUnk_28;
        if (panels) {
            delete panels;
        }
        mUnk_28  = 0;
        mUnk_1cf = 1;
        return 1;
    }
    return 0;
}

void BattleTarget::func_ov007_02258edc() {
    VecFx32 v3c;
    VecFx32 v30;
    VecFx32 pos = mUnk_1a4->mPosition;
    BattleMoveState state;
    u16 rot[3];
    u16 rot2[3];
    u8 i                     = 0;
    fx32 best                = -1;
    *(Rot3 *) rot = *(Rot3 *) &mUnk_1a4->mRotX;
    state.mVelocity.x        = 0;
    state.mVelocity.y        = 0;
    state.mVelocity.z        = 0;
    state.mRot[0]            = 0;
    state.mRot[1]            = 0;
    state.mRot[2]            = 0;
    state.mMode              = 3;
    mUnk_1e4                 = 3;
    switch (mUnk_1db) {
        case 0: {
            mUnk_140 = i;
            do {
                GimmickSetData *set = mUnk_e4[i];
                if (set && set->mUnk_834) {
                    BattleUnit *unit = &data_020d433c.mBattlePlayers->mUnits[(u8) (set->mUnk_834 - 1)];
                    if (unit->mUnk_0f != mUnk_1d4 && unit->mUnk_0f != mUnk_1d5 && unit->mUnk_10 != 2 &&
                        unit->mUnk_10 != 3) {
                        VEC_Subtract(&mUnk_1a4->mPosition, &set->mUnk_810, &v30);
                        fx32 dist = VEC_Mag(&v30);
                        if (best == -1 || best > dist) {
                            best  = dist;
                            set   = mUnk_e4[i];
                            v3c.x = set->mUnk_810.x;
                            v3c.y = set->mUnk_810.y;
                            v3c.z = set->mUnk_810.z;
                        }
                        set = mUnk_e4[i];
                        if (set->mUnk_823 != 0) {
                            func_ov007_02271d78(data_020d433c.mUnk_0c, set);
                            func_ov007_02262828(data_020d433c.mBattlePlayers, mUnk_e4[i]->mUnk_834 - 1, 0);
                            mUnk_e4[i] = 0;
                            mUnk_1db   = 1;
                            mUnk_140   = i;
                            break;
                        }
                    }
                }
                i++;
            } while (i < 12);
            if (best == -1) {
                mUnk_1db = 2;
                break;
            }
            VEC_Subtract(&v3c, &mUnk_1a4->mPosition, &v30);
            v30.x = FX_Mul(v30.x, 0x19a);
            v30.y = FX_Mul(v30.y, 0x19a);
            v30.z = FX_Mul(v30.z, 0x19a);
            mUnk_a0.x = v30.x;
            mUnk_a0.y = v30.y;
            mUnk_a0.z = v30.z;
            mUnk_64.x = mUnk_a0.x;
            mUnk_64.y = mUnk_a0.y;
            mUnk_64.z = mUnk_a0.z;
            VEC_Add(&pos, &v30, &pos);
            break;
        }
        case 1: {
            VEC_Subtract(&mUnk_1a4->mPosition, &v3c, &v30);
            VEC_Normalize(&v30, &v30);
            state.mVelocity.x = v30.x;
            state.mVelocity.y = v30.y;
            state.mVelocity.z = v30.z;
            data_020d433c.mBattlePlayers->mUnk_29c->func_ov007_02270ae0(mUnk_140, &state);
            mUnk_1db = i;
            u8 handle = func_0202bd40(data_020d433c.mEffects, 1, 10, 0, i);
            u16 yaw   = FX_Atan2Idx(mUnk_64.x, mUnk_64.z);
            SoundManager::Get().func_020246f8(0x10);
            rot2[0] = 0;
            rot2[1] = yaw + 0x4000;
            rot2[2] = 0;
            func_0202bf3c(data_020d433c.mEffects, handle, rot2);
            func_0202bef4(data_020d433c.mEffects, handle, &mUnk_1a4->mPosition);
            break;
        }
        case 2: {
            mUnk_64.x = i;
            mUnk_64.y = i;
            mUnk_64.z = i;
            mUnk_94.x = i;
            mUnk_94.y = i;
            mUnk_94.z = i;
            mUnk_b8.x = i;
            mUnk_b8.y = i;
            mUnk_b8.z = i;
            mUnk_ac.x = i;
            mUnk_ac.y = i;
            mUnk_ac.z = i;
            VEC_Normalize(&mUnk_a0, &mUnk_a0);
            mUnk_a0.x = FX_Mul(mUnk_a0.x, 0x3000);
            mUnk_a0.y = FX_Mul(mUnk_a0.y, 0x3000);
            mUnk_a0.z = FX_Mul(mUnk_a0.z, 0x3000);
            mUnk_64.x = mUnk_a0.x;
            mUnk_64.y = mUnk_a0.y;
            mUnk_64.z = mUnk_a0.z;
            if (mUnk_1c0 != 0) {
                func_0202beb8(data_020d433c.mEffects, mUnk_1c0);
            }
            mUnk_1dd = 0;
            mUnk_1db = 0;
            mUnk_140 = 0;
            mUnk_1e4 = 0;
            break;
        }
    }
    mUnk_1a4->SetPosition(&pos);
    mUnk_1a4->SetRotation(rot);
}

void BattleTarget::func_ov007_022593b4() {
    switch (mUnk_1db) {
        case 0:
            mUnk_1cc = 1;
            mUnk_1e4 = 5;
            mUnk_1e6 = 0;
            mUnk_17c = 10;
            mUnk_c4  = 0xa4;
            mUnk_1db++;
            break;
        case 1:
            mUnk_140--;
            if (mUnk_140 != 0) {
                if (mUnk_1cc == 0) {
                    return;
                }
                if (mUnk_38 != 0) {
                    return;
                }
            }
            mUnk_1db++;
            mUnk_140 = 0x19;
            break;
        case 2:
            mUnk_1cc = 0;
            mUnk_1e4 = 0;
            mUnk_184 = 0xb9a;
            mUnk_140--;
            if (mUnk_140 == 0) {
                mUnk_1db++;
            }
            break;
        case 3:
            mUnk_1dd = 0;
            mUnk_1db = 0;
            break;
    }
}

void BattleTarget::func_ov007_02259494(VecFx32 *target) {
    VecFx32 pos     = mUnk_1a4->mPosition;
    VecFx32 tgt     = *target;
    VecFx32 d;
    VecFx32 v20;
    VecFx32 v14;
    VecFx32 oldPos;
    u16 rot[3];
    *(Rot3 *) rot = *(Rot3 *) &mUnk_1a4->mRotX;
    fx32 mag = 0;
    mUnk_1e4 = mag;
    switch (mUnk_1db) {
        case 0:
            target->y = 0x899a;
            VEC_Subtract(target, &mUnk_1a4->mPosition, &d);
            VEC_Normalize(&d, &d);
            d.y = 0;
            d.x = FX_Mul(d.x, 0x666);
            d.y = FX_Mul(d.y, 0x666);
            d.z = FX_Mul(d.z, 0x666);
            VEC_Add(&mUnk_a0, &d, &mUnk_a0);
            if (!(mUnk_64.x == 0 && mUnk_64.y == 0 && mUnk_64.z == 0)) {
                SetVec(&v20, mUnk_64.x, 0, mUnk_64.z);
                mag   = VEC_Mag(&v20);
            }
            if (mag <= 0x4800) {
                mUnk_1db++;
                SoundManager::Get().func_020246f8(0xf);
            }
            if (mUnk_1d8 != 0x32) {
                mUnk_1d8 = 0;
                mUnk_1dd = 0;
                mUnk_140 = 0;
                mUnk_1db = 0;
            }
            break;
        case 1: {
            fx32 y = 0x899a;
            tgt.y  = y;
            VEC_Subtract(&pos, &tgt, &d);
            fx32 m = VEC_Mag(&d);
            fx32 k = 0x9800 - y;
            d.x    = FX_Mul(d.x, k);
            d.y    = FX_Mul(d.y, k);
            d.z    = FX_Mul(d.z, k);
            pos.x  = tgt.x + d.x;
            pos.y  = tgt.y + d.y;
            pos.z  = tgt.z + d.z;
            mUnk_140++;
            if (m <= 0xb000) {
                mUnk_140 = 0;
                mUnk_1db++;
            }
            break;
        }
        case 2:
            tgt.y = 0x899a;
            VEC_Subtract(&pos, &tgt, &d);
            oldPos = pos;
            d.x = FX_Mul(d.x, 0x105a);
            d.y = FX_Mul(d.y, 0x105a);
            d.z = FX_Mul(d.z, 0x105a);
            func_ov007_02233ed8(&d, 0x18e3, &d);
            pos.x  = tgt.x + d.x;
            pos.y  = tgt.y + d.y + 0x4cd;
            pos.z  = tgt.z + d.z;
            rot[1] = rot[1] + 0xaaa;
            mag    = VEC_Mag(&d);
            if (mUnk_14c <= 0x14) {
                mUnk_14c++;
            }
            if (mag >= 0x32000) {
                mUnk_140 = 0;
                mUnk_1db = 0;
                mUnk_1d8 = 0;
                VEC_Subtract(&pos, &oldPos, &v14);
                mUnk_64.x = 0;
                mUnk_64.y = 1;
                mUnk_64.z = 0;
                mUnk_94.x = 0;
                mUnk_94.y = 0;
                mUnk_94.z = 0;
                mUnk_b8.x = 0;
                mUnk_b8.y = 0;
                mUnk_b8.z = 0;
                mUnk_ac.x = 0;
                mUnk_ac.y = 0;
                mUnk_ac.z = 0;
                VEC_Normalize(&v14, &v14);
                v14.x     = FX_Mul(v14.x, 0x3b33);
                v14.y     = FX_Mul(v14.y, 0x3b33);
                v14.z     = FX_Mul(v14.z, 0x3b33);
                mUnk_148  = pos.y;
                mUnk_a0.x = v14.x;
                mUnk_a0.y = v14.y;
                mUnk_a0.z = v14.z;
                mUnk_1d8  = 0;
                mUnk_1dd  = 0;
                func_02024764(&SoundManager::Get(), 0xf, 0x14);
            }
            break;
    }
    mUnk_1a4->SetPosition(&pos);
    mUnk_1a4->SetRotation(rot);
}

void BattleTarget::func_ov007_022599fc() {
    u8 a = 0;
    u8 b = 0;
    if (func_ov007_02248998(&mUnk_1a4->mPosition, &a, &b)) {
        mUnk_1da = 1;
    } else {
        mUnk_1da = 0;
    }
}

void BattleTarget::func_ov007_02259a40(u8 a, u8 b, VecFx32 *out, VecFx32 *ref) {
    VecFx32 pos;
    VecFx32 diff;
    VEC_Subtract(out, ref, &diff);
    if (VEC_Mag(&diff) >= 0xc99a) {
        return;
    }
    func_ov007_02248944(&pos, a, b);
    out->z = pos.z < ref->z ? ref->z - 0xc99a : ref->z + 0xc99a;
    if (pos.x < ref->x) {
        out->x = ref->x - 0x64cd;
    } else {
        out->x = ref->x + 0x64cd;
    }
}

BOOL BattleTarget::func_ov007_02259af0() {
    u8 a = 0;
    u8 b = 0;
    BattleMoveState state;
    VecFx32 d;
    u8 i;
    u8 j;
    state.mVelocity.x = 0;
    state.mVelocity.y = 0;
    state.mVelocity.z = 0;
    state.mRot[0]     = 0;
    state.mRot[1]     = 0;
    state.mRot[2]     = 0;
    state.mMode       = 4;
    BattlePlayerList *list = data_020d433c.mBattlePlayers;
    if (mUnk_1e0 != 3) {
        return 0;
    }
    if (mUnk_7c.x != 0 || mUnk_7c.z != 0) {
        mUnk_1db = 2;
        mUnk_1e0 = 0;
        return 0;
    }
    if (mUnk_38 == 0 && mUnk_4c != 0) {
        mUnk_1e0 = 0;
        mUnk_1d6 = 0;
        if (mUnk_148 != 0x899a || !func_ov007_02248998(&mUnk_1a4->mPosition, &a, &b)) {
            if (mUnk_1dd == 2) {
                mUnk_1db = 2;
            }
            return 0;
        }
        SoundManager::Get().func_020246f8(0x10);
        mUnk_1db = 1;
        u8 owner = list->mUnk_1f8[a][b];
        u8 cell  = list->mUnk_16c[a][b];
        mUnk_1e4 = 2;
        mUnk_1dd = 2;
        SetVec(&mUnk_1a4->mPosition, mUnk_1a4->mPosition.x, 0x8999, mUnk_1a4->mPosition.z);
        UnkStruct_0203d374 *pose = mUnk_1a4;
        {
            u16 r1 = pose->mRotY;
            u16 r2 = pose->mRotZ;
            pose->mRotX = 0;
            pose->mRotY = r1;
            pose->mRotZ = r2;
        }
        mUnk_1a4->mUnk_49 = 1;
        if (owner != 0) {
            int idx          = owner - 1;
            BattleUnit *unit = &list->mUnits[(u8) idx];
            if (data_020d433c.mUnk_85 != 0 || (u8) (unit->mUnk_10 + 0xfe) <= 1) {
                if (unit->mUnk_0f == mUnk_1d4) {
                    mUnk_1d2 = 3;
                    StartResult(this, 2);
                } else if (unit->mUnk_0f == mUnk_1d5) {
                    mUnk_1d2 = 1;
                    func_ov007_0225b69c(a, b);
                    StartResult(this, 0x10);
                } else {
                    mUnk_1d2 = 3;
                    StartResult(this, 0xa);
                }
            } else if (unit->mUnk_0f == mUnk_1d4 || unit->mUnk_0f == mUnk_1d5) {
                mUnk_1d2 = 2;
                func_ov007_02262828(data_020d433c.mBattlePlayers, idx, 2);
                StartResult(this, 0xa);
                VEC_Subtract(&list->mUnk_29c->func_ov007_0226fc4c(idx)->mPosition, &mUnk_1a4->mPosition, &d);
                if (d.x == 0 && d.z == 0) {
                    state.mVelocity.z = 0x1000;
                    state.mVelocity.x = 0x1000;
                } else {
                    state.mVelocity = d;
                }
                list->mUnk_29c->func_ov007_02270ae0(idx, &state);
            } else {
                mUnk_1d2 = 4;
                StartResult(this, 1);
                VEC_Subtract(&list->mUnk_29c->func_ov007_0226fc4c(idx)->mPosition, &mUnk_1a4->mPosition, &d);
                if (d.x == 0 && d.z == 0) {
                    state.mVelocity.z = 0x1000;
                    state.mVelocity.x = 0x1000;
                } else {
                    state.mVelocity = d;
                }
                list->mUnk_29c->func_ov007_02270ae0(idx, &state);
            }
            if (mUnk_1d2 != 1) {
                pose     = mUnk_1a4;
                mUnk_1e8 = func_ov007_02262768(list, &pose->mPosition, pose->mRotY, mUnk_1d4, mUnk_1d3, cell - 1, 1);
                if (mUnk_1d2 != 2) {
                    func_ov007_02262a64(list, idx, (u8) list->mUnk_1f8[a][b] - 1);
                }
            }
        } else {
            mUnk_1d2 = 2;
            pose     = mUnk_1a4;
            mUnk_1e8 = func_ov007_02262768(list, &pose->mPosition, pose->mRotY, mUnk_1d4, mUnk_1d3, cell - 1, 1);
            StartResult(this, 0xa);
            func_ov007_02262a64(list, mUnk_1e8 - 1, mUnk_1e8 - 1);
        }
        i = 0;
        do {
            j = 0;
            do {
                s8 ni = a + i - 1;
                s8 nj = b + j - 1;
                if (ni >= 0 && nj >= 0 && ni < 14 && nj < 10 && (ni == a || nj == b) && !(ni == a && nj == b)) {
                    u8 other = list->mUnk_1f8[(u8) ni][(u8) nj];
                    if (other != 0) {
                        u8 kind = list->mUnits[(u8) (other - 1)].mUnk_10;
                        if (kind != 2 && kind != 3) {
                            func_ov007_02262828(data_020d433c.mBattlePlayers, other - 1, 2);
                            VEC_Subtract(&list->mUnk_29c->func_ov007_0226fc4c(other - 1)->mPosition, &mUnk_1a4->mPosition, &d);
                            state.mVelocity.x = d.x;
                            state.mVelocity.y = d.y;
                            state.mVelocity.z = d.z;
                            list->mUnk_29c->func_ov007_02270ae0(other - 1, &state);
                        }
                    }
                }
                j++;
            } while (j < 3);
            i++;
        } while (i < 3);
        mUnk_180  = 0x3000;
        mUnk_1c6  = 1;
        mUnk_64.x = 0;
        mUnk_64.y = 0;
        mUnk_64.z = 0;
        mUnk_a0.x = 0;
        mUnk_a0.y = 0;
        mUnk_a0.z = 0;
        mUnk_ac.x = 0;
        mUnk_ac.y = 0;
        mUnk_ac.z = 0;
        mUnk_b8.x = 0;
        mUnk_b8.y = 0;
        mUnk_b8.z = 0;
        return 1;
    }
    return 0;
}

BOOL BattleTarget::func_ov007_0225a278() {
    u8 a = 0;
    u8 b = 0;
    BattlePlayerList *list = data_020d433c.mBattlePlayers;
    if (mUnk_1cf == 0) {
        UnkStruct_0203d374 *pose = mUnk_1a4;
        if (pose->mPosition.y <= 0x899a) {
            if (func_ov007_0225b4a0(&pose->mPosition, &pose->mRotX, &mUnk_64, &a, &b)) {
                u8 cell = list->mUnk_16c[a][b];
                if (cell != 0) {
                    pose        = mUnk_1a4;
                    Rot3 rot    = *(Rot3 *) &pose->mRotX;
                    VecFx32 pos = pose->mPosition;
                    SoundManager::Get().func_020246f8(3);
                    mUnk_64.z = 0;
                    mUnk_64.y = 0;
                    mUnk_64.x = 0;
                    rot.r[0] = 0;
                    mUnk_1a4->SetRotation(rot.r);
                    pos.y             = 0x899a;
                    pose              = mUnk_1a4;
                    pose->mPosition.x = pos.x;
                    pose->mPosition.y = pos.y;
                    pose->mPosition.z = pos.z;
                    mUnk_1cf          = 1;
                    mUnk_1c6          = 1;
                    mUnk_1e4          = 0;
                    if (mUnk_1c0 != 0) {
                        func_0202beb8(data_020d433c.mEffects, mUnk_1c0);
                        mUnk_1c0 = 0;
                    }
                    mUnk_1a4->mUnk_49 = 1;
                    u8 owner = list->mUnk_1f8[a][b];
                    if (owner != 0) {
                        BattleUnit *unit = &list->mUnits[(u8) (owner - 1)];
                        VecFx32 pos2     = mUnk_1a4->mPosition;
                        VecFx32 ref      = unit->mPos;
                        func_ov007_02259a40(a, b, &pos2, &ref);
                        pose              = mUnk_1a4;
                        pose->mPosition.x = pos2.x;
                        pose->mPosition.y = pos2.y;
                        pose->mPosition.z = pos2.z;
                        mUnk_1d2          = 3;
                        func_ov007_02255cb4(0);
                        if (unit->mUnk_0f == mUnk_1d4) {
                            StartResult(this, 2);
                        } else if (unit->mUnk_0f == mUnk_1d5) {
                            StartResult(this, 0x10);
                            func_ov007_0225b69c(a, b);
                            mUnk_1d2 = 1;
                            return 1;
                        } else {
                            StartResult(this, 0xa);
                        }
                        pose     = mUnk_1a4;
                        mUnk_1e8 = func_ov007_02262768(list, &pose->mPosition, pose->mRotY, mUnk_1d4, mUnk_1d3, cell - 1, 1);
                        func_ov007_02262a64(list, owner - 1, (u8) list->mUnk_1f8[a][b] - 1);
                    } else {
                        VecFx32 p3 = mUnk_1a4->mPosition;
                        p3.y -= 0x64cd;
                        mUnk_1d2 = 2;
                        func_ov007_02255cb4(0);
                        switch (mUnk_1df) {
                            case 5: {
                                pose     = mUnk_1a4;
                                mUnk_1e8 = func_ov007_02262768(list, &pose->mPosition, pose->mRotY, mUnk_1d4, mUnk_1d3, cell - 1, 2);
                                s16 h    = func_0202bd40(data_020d433c.mEffects, 1, 0x13, 0, 1);
                                if (h > 0) {
                                    func_0202bef4(data_020d433c.mEffects, h, &p3);
                                }
                                SoundManager::Get().func_020246f8(0x5e);
                                break;
                            }
                            case 6: {
                                pose     = mUnk_1a4;
                                mUnk_1e8 = func_ov007_02262768(list, &pose->mPosition, pose->mRotY, mUnk_1d4, mUnk_1d3, cell - 1, 3);
                                s16 h    = func_0202bd40(data_020d433c.mEffects, 1, 0x14, 0, 1);
                                if (h > 0) {
                                    func_0202bef4(data_020d433c.mEffects, h, &p3);
                                }
                                SoundManager::Get().func_020246f8(0x5e);
                                break;
                            }
                            default:
                                pose     = mUnk_1a4;
                                mUnk_1e8 = func_ov007_02262768(list, &pose->mPosition, pose->mRotY, mUnk_1d4, mUnk_1d3, cell - 1, 1);
                                break;
                        }
                        StartResult(this, 0xa);
                        func_ov007_02262a64(list, mUnk_1e8 - 1, mUnk_1e8 - 1);
                    }
                    return 1;
                }
            }
        }
    }
    return 0;
}

BOOL BattleTarget::func_ov007_0225a8d4(VecFx32 *a, VecFx32 *b, BattlePlane *plane, fx32 *t, VecFx32 *hit) {
    VecFx32 d;
    VEC_Subtract(b, a, &d);
    fx32 dot  = VEC_DotProduct(&plane->mNormal, a);
    fx32 den  = VEC_DotProduct(&plane->mNormal, &d);
    if (den == 0) {
        *t = plane->mDist - dot;
    } else {
        *t = FX_Div(plane->mDist - dot, den);
    }
    fx32 tt = *t;
    if ((float) tt >= 0.0f && tt <= 0x1000) {
        d.x = FX_Mul(d.x, tt);
        d.y = FX_Mul(d.y, tt);
        d.z = FX_Mul(d.z, tt);
        VEC_Add(a, &d, hit);
        return 1;
    }
    return 0;
}

BOOL BattleTarget::func_ov007_0225a9c8(VecFx32 *a, VecFx32 *b, VecFx32 *c, VecFx32 *d, VecFx32 *e, fx32 *t, VecFx32 *hit) {
    BattlePlane p;
    VecFx32 eb;
    VecFx32 ea;
    VecFx32 ab;
    VEC_Subtract(d, c, &ea);
    VEC_Subtract(e, c, &eb);
    VEC_Subtract(a, b, &ab);
    VEC_CrossProduct(&ea, &eb, &p.mNormal);
    p.mDist = VEC_DotProduct(&p.mNormal, c);
    if ((float) VEC_DotProduct(&p.mNormal, &ab) <= 0.0f) {
        return 0;
    }
    return func_ov007_0225a8d4(a, b, &p, t, hit);
}

BOOL BattleTarget::func_ov007_0225aa80(BattleHitBox *box, VecFx32 *point) {
    VecFx32 d;
    MtxFx33 mtx;
    VEC_Subtract(point, &box->mPos, &d);
    mtx = data_0220e848;
    if (box->mRotY) {
        func_01ffcfd8(&mtx, -box->mRotY);
    }
    if (box->mRotX) {
        func_01ffcf80(&mtx, -box->mRotX);
    }
    if (box->mRotZ) {
        func_01ffd030(&mtx, -box->mRotZ);
    }
    func_01ff84d0(&d, &mtx, &d);
    VEC_Add(&d, &box->mPos, &d);
    fx32 pz    = box->mPos.z;
    fx32 hz    = box->mHalfZ;
    fx32 lowX  = box->mPos.x - box->mHalfX;
    fx32 lowZ  = pz - hz;
    fx32 highZ = hz + pz;
    fx32 highX = box->mHalfX + box->mPos.x;
    if (lowX >= d.x || highX <= d.x || lowZ >= d.z || highZ <= d.z) {
        return 0;
    }
    return 1;
}

BOOL BattleTarget::func_ov007_0225aba0(VecFx32 *a, VecFx32 *b, VecFx32 *tri, fx32 *t, VecFx32 *out) {
    VecFx32 e1;
    VecFx32 e0;
    VecFx32 hit;
    VecFx32 n;
    if (!func_ov007_0225a9c8(a, b, &tri[0], &tri[1], &tri[2], t, &hit)) {
        return 0;
    }
    VEC_Subtract(&tri[1], &tri[0], &e0);
    VEC_Subtract(&tri[2], &tri[1], &e1);
    VEC_CrossProduct(&e0, &e1, &n);
    out->y = n.y;
    return 1;
}

// non-matching: 20 words, register allocation only: in the second loop the original keeps the counter in r6 and f[i] (and the pos.y / FX_Mul temporaries) in r7; here r7 / r6.
BOOL BattleTarget::func_ov007_0225ac28(BattleHitBox *box, BattleSphere *sphere, VecFx32 *dir, VecFx32 *out, u8 flag, u8 *outFlag) {
    VecFx32 c[8];
    VecFx32 e1;
    VecFx32 e0;
    VecFx32 n = {0, 0, 0};
    VecFx32 tri[3];
    VecFx32 qa;
    if (VEC_Mag(dir) <= 0) {
        return 0;
    }
    SetVec(&c[0], box->mHalfX, box->mHalfY, box->mHalfZ);
    SetVec(&c[1], -box->mHalfX, box->mHalfY, box->mHalfZ);
    SetVec(&c[2], -box->mHalfX, box->mHalfY, -box->mHalfZ);
    SetVec(&c[3], box->mHalfX, box->mHalfY, -box->mHalfZ);
    SetVec(&c[4], box->mHalfX, -box->mHalfY, box->mHalfZ);
    SetVec(&c[5], -box->mHalfX, -box->mHalfY, box->mHalfZ);
    SetVec(&c[6], -box->mHalfX, -box->mHalfY, -box->mHalfZ);
    SetVec(&c[7], box->mHalfX, -box->mHalfY, -box->mHalfZ);
    func_ov007_02233ed8(&c[0], box->mRotY, &c[0]);
    func_ov007_02233ed8(&c[1], box->mRotY, &c[1]);
    func_ov007_02233ed8(&c[2], box->mRotY, &c[2]);
    func_ov007_02233ed8(&c[3], box->mRotY, &c[3]);
    func_ov007_02233ed8(&c[4], box->mRotY, &c[4]);
    func_ov007_02233ed8(&c[5], box->mRotY, &c[5]);
    func_ov007_02233ed8(&c[6], box->mRotY, &c[6]);
    func_ov007_02233ed8(&c[7], box->mRotY, &c[7]);
    u8 i = 0;
    do {
        VEC_Add(&c[i], &box->mPos, &c[i]);
        i++;
    } while (i < 8);
    if (outFlag) {
        *outFlag = 0;
    }
    if (func_ov007_0225aa80(box, &sphere->mPos)) {
        fx32 t2;
        if (outFlag) {
            *outFlag = 1;
        }
        tri[0] = c[1];
        tri[1] = c[0];
        tri[2] = c[3];
        VecFx32 ta;
        ta.z = sphere->mPos.z;
        ta.y = sphere->mPos.y - sphere->mRadius;
        ta.x = sphere->mPos.x;
        qa = ta;
        if (func_ov007_0225aba0(&sphere->mPos, &qa, tri, &t2, out)) {
            return 1;
        }
        tri[0] = c[7];
        tri[1] = c[4];
        tri[2] = c[5];
        VecFx32 tb;
        tb.z = sphere->mPos.z;
        tb.y = sphere->mPos.y + sphere->mRadius;
        tb.x = sphere->mPos.x;
        qa = tb;
        if (func_ov007_0225aba0(&sphere->mPos, &qa, tri, &t2, out)) {
            return 1;
        }
    }
    fx32 f[4] = {0, 0, 0, 0};
    VecFx32 va0;
    VecFx32 nearest;
    VecFx32 p2;
    VecFx32 p1;
    VecFx32 p0;
    VecFx32 q;
    VecFx32 hit;
    fx32 t;
    u8 idx;
    fx32 best = 0xffff;
    i = 0;
    idx = i;
    func_ov007_0227238c(sphere, box, &nearest);
    VEC_Subtract(&nearest, &sphere->mPos, &va0);
    if (VEC_Mag(&va0) == 0) {
        if (outFlag) {
            *outFlag = 1;
        }
        return 0;
    }
    do {
        p0 = c[i + 4];
        p1 = c[i];
        p2 = c[i + 1];
        VecFx32 d1 = *dir;
        VecFx32 pos = sphere->mPos;
        if (flag == 0) {
            VEC_Normalize(&d1, &d1);
            SetVec(&q, pos.x + FX_Mul(d1.x, sphere->mRadius), pos.y, pos.z + FX_Mul(d1.z, sphere->mRadius));
        } else {
            SetVec(&q, pos.x + va0.x, pos.y, pos.z + va0.z);
        }
        if (func_ov007_0225a9c8(&pos, &q, &p0, &p1, &p2, &t, &hit)) {
            VEC_Subtract(&p1, &p0, &e0);
            VEC_Subtract(&p2, &p0, &e1);
            VEC_CrossProduct(&e1, &e0, &n);
            f[i] = t;
        }
        fx32 fk = f[i];
        if (0.0f != (float) fk && best >= fk) {
            best = fk;
            idx  = (u8) (i + 1);
        }
        i++;
    } while (i < 4);
    if (idx == 0) {
        return 0;
    }
    VecFx32 pc;
    VecFx32 pb;
    VecFx32 pa;
    pa = c[idx + 3];
    pb = c[idx - 1];
    pc = c[idx];
    VEC_Subtract(&pb, &pa, &e0);
    VEC_Subtract(&pc, &pa, &e1);
    VEC_CrossProduct(&e1, &e0, &n);
    out->x += n.x;
    out->z += n.z;
    return 1;
}

void BattleTarget::func_ov007_0225b298(s32 *pair, u8 mode, VecFx32 *out) {
    out->x = 0;
    out->y = 0;
    out->z = 0;
    if ((pair[1] | pair[0]) == 0) {
        return;
    }
    switch (mode) {
        case 0: {
        if (pair[1] > 0) {
            pair[1] = FX_Mul(pair[1], 0x8f);
        } else if (pair[1] < 0) {
            pair[1] = FX_Mul(pair[1], 0xcd);
        }
        s32 x = pair[0];
        if (0.0f != (float) x) {
            pair[0] = FX_Mul(x, 0xcd);
        }
        break;
        }
        case 1: {
        if (pair[1] > 0) {
            pair[1] = FX_Mul(pair[1], 0x66);
        } else if (pair[1] < 0) {
            pair[1] = FX_Mul(pair[1], 0x66);
        }
        s32 x = pair[0];
        if (0.0f != (float) x) {
            pair[0] = FX_Mul(x, 0x1ec);
        }
        break;
        }
    }
    s32 px = pair[0];
    s32 pz = pair[1];
    out->x = px;
    out->y = 0;
    out->z = pz;
    out->x = FX_Mul(out->x, 0x1000);
    out->y = FX_Mul(out->y, 0x1000);
    out->z = FX_Mul(out->z, 0x1000);
}

BOOL BattleTarget::func_ov007_0225b4a0(VecFx32 *pos, u16 *angle, VecFx32 *vel, u8 *outA, u8 *outB) {
    u8 a = 0;
    u8 b = 0;
    s32 speed;
    BattleRecord *record;
    s32 x = vel->x;
    s32 z = vel->z;
    if (x < 0) {
        x = -x;
    }
    if (z < 0) {
        z = -z;
    }
    speed = z + x;
    record = &data_020d433c.mBattlePlayers->mPlayers[mUnk_1d4]->mRecords[mUnk_1d3];
    if (mUnk_1d1 != 0 && pos->y <= 0x899a) {
        u32 k = record->mUnk_02_9;
        if (speed < data_ov007_0227a6d0[k]) {
            u16 lim = data_ov007_0227a5f6[k];
            if (!(*angle > lim && *angle < (u16) -lim)) {
                if (func_ov007_02248998(pos, &a, &b)) {
                    BattlePlayerList *list = data_020d433c.mBattlePlayers;
                    BattlePlayer *p0 = list->mPlayers[mUnk_1d4];
                    BattlePlayer *p1 = list->mPlayers[mUnk_1d5];
                    u8 cell = *(u8 *) &list->mUnk_16c[a][b];
                    if (cell != 0) {
                        *outA = a;
                        *outB = b;
                        u8 m = list->mUnk_1c[(u8) (cell - 1)].mUnk_5;
                        if (m != 0) {
                            BattleUnit *unit = &data_020d433c.mBattlePlayers->mUnits[(u8) (m - 1)];
                            if (p0->IsActive() && !p1->IsActive() && unit->mUnk_0f == mUnk_1d5) {
                                if (speed < 0x1000) {
                                    if (!(*angle > 0xe38 && *angle < 0x10000 - 0xe38)) {
                                        return 1;
                                    }
                                }
                            } else {
                                return 1;
                            }
                        } else {
                            return 1;
                        }
                    }
                }
            }
        }
    }
    return 0;
}

void BattleTarget::func_ov007_0225b69c(u8 a, u8 b) {
    func_ov007_022479f4(mUnk_178, 1);
    func_ov007_02247a3c(mUnk_178);
    BattleCommandMap *map = mUnk_178;
    map->mUnk_148         = a;
    map->mUnk_149         = b;
}

void BattleTarget::func_ov007_0225b6d0() {
    char buf[0x804];
    VecFx32 vel;
    VecFx32 dir;
    u16 rot[3];
    u16 rot2[3];
    u16 r3[3];
    if (mUnk_1e2 != 1) {
        return;
    }
    vel.x                = 0;
    vel.y                = 0;
    vel.z                = 0;
    int power;
    BattlePlayer *player = data_020d433c.mBattlePlayers->mPlayers[mUnk_1d4];
    BattleRecord *record = &player->mRecords[mUnk_1d3];
    dir.x                = 0x1000;
    dir.y                = 0;
    dir.z                = 0;
    *(Rot3 *) rot = *(Rot3 *) player->mUnk_cc;
    rot[1]               = rot[1] + data_ov007_0227a5ee[player->mUnk_f1];
    power             = (u8) (data_ov007_0227a754[record->mUnk_02_0] + player->mUnk_ec);
    mUnk_50 = mUnk_54 = data_ov007_0227a6bc[record->mUnk_02_6];
    if (player->mUnk_ec == 0) {
        power = 10;
    }
    rot[1]               = rot[1] - mUnk_1b8;
    func_ov007_0225c360(&rot[2], mUnk_1ba, &rot[2]);
    rot[2] = rot[2] + mUnk_1ba;
    switch (mUnk_1df) {
        case 0:
            func_ov007_022334f4(rot, power, &dir, &vel);
            break;
        case 1:
            rot[2] = mUnk_1ba + 0x38e;
            func_ov007_022334f4(rot, 0x78, &dir, &vel);
            mUnk_1c0 = func_0202bd40(data_020d433c.mEffects, 1, 0, 0, 1);
            SoundManager::Get().func_020246f8(0x3d);
            mUnk_1e1 = 1;
            break;
        case 2:
            func_ov007_022334f4(rot, power, &dir, &vel);
            mUnk_1c0 = func_0202bd40(data_020d433c.mEffects, 1, 2, 0, 1);
            SoundManager::Get().func_020246f8(0x3e);
            break;
        case 5:
            func_ov007_022334f4(rot, power, &dir, &vel);
            mUnk_1c0 = func_0202bd40(data_020d433c.mEffects, 1, 7, 0, 1);
            SoundManager::Get().func_020246f8(0x41);
            break;
        case 6:
            func_ov007_022334f4(rot, power, &dir, &vel);
            mUnk_1c0 = func_0202bd40(data_020d433c.mEffects, 1, 8, 0, 1);
            SoundManager::Get().func_020246f8(0x3f);
            break;
        case 4: {
            func_ov007_022334f4(rot, power, &dir, &vel);
            mUnk_1c0  = func_0202bd40(data_020d433c.mEffects, 1, 9, 0, 1);
            u8 handle = func_0202bd40(data_020d433c.mEffects, 1, 10, 0, 0);
            u16 t = FX_Atan2Idx(vel.x, vel.z);
            rot2[0]   = 0;
            rot2[1]   = t + 0x4000;
            rot2[2]   = 0;
            func_0202bf3c(data_020d433c.mEffects, handle, rot2);
            func_0202bef4(data_020d433c.mEffects, handle, &mUnk_1a4->mPosition);
            SoundManager::Get().func_020246f8(0x42);
            break;
        }
        case 3: {
            mUnk_1dd = 2;
            u8 k     = data_020d433c.mUnk_95;
            u16 a    = data_ov007_0227a660[k];
            u16 b    = data_ov007_0227a670[k];
            rot[2]   = b;
            func_ov007_022334f4(rot, (u16) (a + 0x50), &dir, &vel);
            mUnk_1c0  = func_0202bd40(data_020d433c.mEffects, 1, 0xb, 0, 0);
            mUnk_11c.x = 0;
            mUnk_11c.y = 0;
            mUnk_11c.z = 0;
            SoundManager::Get().func_020246f8(0x40);
            break;
        }
    }
    mUnk_1e0  = mUnk_1df;
    mUnk_1be  = rot[1];
    SetVec(&mUnk_64, vel.x, vel.y, vel.z);
    mUnk_a0.x = mUnk_64.x;
    mUnk_a0.y = mUnk_64.y;
    mUnk_a0.z = mUnk_64.z;
    mUnk_a0.x = FX_Mul(mUnk_a0.x, 0xb800);
    mUnk_a0.y = FX_Mul(mUnk_a0.y, 0xb800);
    mUnk_a0.z = FX_Mul(mUnk_a0.z, 0xb800);
    mUnk_ac.x = 0;
    mUnk_ac.y = 0;
    mUnk_ac.z = 0;
    mUnk_118  = player->mUnk_d8;
    mUnk_114  = -player->mUnk_d4;
    func_ov007_0225b298((s32 *) &mUnk_114, mUnk_1cc, &mUnk_94);
    mUnk_ac.x = mUnk_94.x;
    mUnk_ac.y = mUnk_94.y;
    mUnk_ac.z = mUnk_94.z;
    func_ov007_022719c8(buf, 0xa);
    if (mUnk_1df != 5) {
        mUnk_1a8 = func_ov007_02271ca8(data_020d433c.mUnk_0c, 1, buf, &mUnk_1a4->mPosition, &mUnk_1a4->mRotX, 0);
    } else {
        mUnk_1a8 = func_ov007_02271ca8(data_020d433c.mUnk_0c, 0x10, buf, &mUnk_1a4->mPosition, &mUnk_1a4->mRotX, 0);
    }
    func_ov007_02248620(mUnk_178, 0);
    mUnk_1a4->mUnk_4a = 1;
    if (mUnk_1df == 0) {
        if (power > 0x59) {
            mUnk_1c0 = func_0202bd40(data_020d433c.mEffects, 1, 0x11, 0, 0);
            SoundManager::Get().func_020246f8(0x39);
        } else if (power > 0x4f) {
            mUnk_1c0 = func_0202bd40(data_020d433c.mEffects, 1, 0xf, 0, 0);
            SoundManager::Get().func_020246f8(0x3a);
        } else if (power > 0x3c) {
            mUnk_1c0 = func_0202bd40(data_020d433c.mEffects, 1, 0x10, 0, 0);
            SoundManager::Get().func_020246f8(0x3b);
        }
        if (mUnk_1c0 != 0) {
            r3[1] = FX_Atan2Idx(mUnk_64.x, mUnk_64.z) + 0x4000;
            r3[0] = 0;
            r3[2] = 0;
            func_0202bf3c(data_020d433c.mEffects, mUnk_1c0, r3);
            func_0202bef4(data_020d433c.mEffects, mUnk_1c0, &mUnk_1a4->mPosition);
        }
    }
    player->mUnk_dc = 0;
    player->mUnk_e0 = 0;
    player->mUnk_e4 = 0;
    player->mUnk_e8 = 0;
    mUnk_1e2++;
}

void BattleTarget::func_ov007_0225bf38() {
    BattlePlayer *player = data_020d433c.mBattlePlayers->mPlayers[mUnk_1d4];
    mUnk_118             = player->mUnk_d8;
    mUnk_114             = player->mUnk_d4;
    func_ov007_0225c098(&mUnk_114);
}

void BattleTarget::func_ov007_0225bf70(VecFx32 *a, VecFx32 *b) {
    VecFx32 sum  = {0, 0, 0};
    VecFx32 unused = {0, 0, 0};
    VecFx32 rest = {0, 0, 0};
    VecFx32 full = {0, 0, 0};
    VEC_Add(a, b, &sum);
    full.x = sum.x;
    full.y = sum.y;
    full.z = sum.z;
    if (VEC_Mag(&sum) >= 0x3000) {
        VEC_Normalize(&sum, &sum);
        sum.x = FX_Mul(sum.x, 0x3000);
        sum.y = FX_Mul(sum.y, 0x3000);
        sum.z = FX_Mul(sum.z, 0x3000);
    }
    VEC_Subtract(&full, &sum, &rest);
    VEC_Subtract(b, &rest, b);
}

// non-matching: 44 words, register allocation: the original keeps the scale in r0 (3 words saved less) and computes &mUnk_c8 for VEC_Add after the third product; here the scale is in r3 and the address is scheduled early, which pushes the products into r5/ip/lr and the camera vector through r5 instead of ip.
void BattleTarget::func_ov007_0225c098(s32 *pair) {
    VecFx32 target;
    VecFx32 cam;
    mUnk_118 = pair[1];
    mUnk_114 = -pair[0];
    if (mUnk_1d4 != data_020d433c.mBattlePlayers->mUnk_2a9 && data_020d433c.mUnk_85 != 0) {
        return;
    }
    if (mUnk_38 > 3) {
        func_ov007_0225b298((s32 *) &mUnk_114, mUnk_1cc, &mUnk_94);
        Camera *camera = mUnk_174;
        target.x       = camera->mTarget.x;
        target.y       = camera->mTarget.y;
        target.z       = camera->mTarget.z;
        cam.x          = mUnk_194.x;
        cam.y          = mUnk_194.y;
        cam.z          = mUnk_194.z;
        func_ov007_02255b78(&target, &cam, &mUnk_94, &mUnk_94);
        u32 scale = mUnk_1dd == 2 ? 0x14000 : 0x5000;
        mUnk_94.x = (fx32) (((fx64) mUnk_94.x * scale + 0x800LL) >> 12);
        mUnk_94.y = (fx32) (((fx64) mUnk_94.y * scale + 0x800LL) >> 12);
        mUnk_94.z = (fx32) (((fx64) mUnk_94.z * scale + 0x800LL) >> 12);
        VEC_Add(&mUnk_c8, &mUnk_94, &mUnk_c8);
    } else if (mUnk_8c >= 0x800) {
        if (mUnk_118 == 0 && mUnk_114 == 0) {
            return;
        }
        func_ov007_0225b298((s32 *) &mUnk_114, mUnk_1cc, &mUnk_94);
        Camera *camera = mUnk_174;
        target.x       = camera->mTarget.x;
        target.y       = camera->mTarget.y;
        target.z       = camera->mTarget.z;
        cam.x          = mUnk_194.x;
        cam.y          = mUnk_194.y;
        cam.z          = mUnk_194.z;
        func_ov007_02255b78(&target, &cam, &mUnk_94, &mUnk_94);
        fx32 s    = mUnk_8c;
        mUnk_94.x = FX_Mul(mUnk_94.x, s);
        mUnk_94.y = FX_Mul(mUnk_94.y, s);
        mUnk_94.z = FX_Mul(mUnk_94.z, s);
        VEC_Add(&mUnk_c8, &mUnk_94, &mUnk_c8);
    } else {
        mUnk_8c = 0;
    }
}

void BattleTarget::func_ov007_0225c2e4(VecFx32 *v) {
    VecFx32 d;
    fx32 scale = mUnk_8c;
    d.x = FX_Mul(v->x, scale);
    d.y = FX_Mul(v->y, scale);
    d.z = FX_Mul(v->z, scale);
    VEC_Add(&mUnk_c8, &d, &mUnk_c8);
}

void BattleTarget::func_ov007_0225c360(u16 *in, s32 a, u16 *out) {
    u16 n    = *in - 910;
    int base = 0x1c71;
    fx32 q   = FX_Div(INT_TO_FX(n), INT_TO_FX(base));
    fx32 d   = FX_Div(0x4000000, 0x2aaa000);
    fx32 m   = FX_Mul(a / 182, d);
    int rem  = 100 - m;
    fx32 r   = INT_TO_FX(rem) / 100;
    fx32 t1  = FX_Mul(INT_TO_FX(base), r);
    fx32 res = FX_Mul(t1, q);
    *out     = (u16) (u32) (910.0f + (float) res / 4096.0f);
}

void BattleTarget::func_ov007_0225c4dc() {
    switch (mUnk_1df) {
        case 0: {
            if (mUnk_1c0 == 0) {
                break;
            }
            if (EffectAlive(data_020d433c.mEffects, mUnk_1c0)) {
                u16 rot[3];
                rot[1] = FX_Atan2Idx(mUnk_64.x, mUnk_64.z) + 0x4000;
                rot[0] = 0;
                rot[2] = 0;
                func_0202bf3c(data_020d433c.mEffects, mUnk_1c0, rot);
                func_0202bef4(data_020d433c.mEffects, mUnk_1c0, &mUnk_1a4->mPosition);
            } else {
                mUnk_1c0 = 0;
            }
            break;
        }
        case 1: {
            if (mUnk_1c0 == 0) {
                break;
            }
            if (VEC_Mag(&mUnk_64) <= 0x1000) {
                func_0202beb8(data_020d433c.mEffects, mUnk_1c0);
                mUnk_1e1 = 0;
                mUnk_1c0 = 0;
            } else {
                u16 rot[3];
                rot[1] = FX_Atan2Idx(mUnk_64.x, mUnk_64.z) + 0x4000;
                rot[0] = 0;
                rot[2] = 0;
                func_0202bf3c(data_020d433c.mEffects, mUnk_1c0, rot);
                func_0202bef4(data_020d433c.mEffects, mUnk_1c0, &mUnk_1a4->mPosition);
            }
            break;
        }
        case 2: {
            if (mUnk_1c0 == 0) {
                break;
            }
            u16 rot[3];
            rot[1] = FX_Atan2Idx(mUnk_64.x, mUnk_64.z) + 0x4000;
            rot[0] = 0;
            rot[2] = 0;
            func_0202bf3c(data_020d433c.mEffects, mUnk_1c0, rot);
            func_0202bef4(data_020d433c.mEffects, mUnk_1c0, &mUnk_1a4->mPosition);
            break;
        }
        case 5:
        case 6:
            if (mUnk_1c0 == 0) {
                break;
            }
            VEC_Mag(&mUnk_64);
            func_0202bef4(data_020d433c.mEffects, mUnk_1c0, &mUnk_1a4->mPosition);
            break;
        case 3:
            if (mUnk_1c0 == 0) {
                break;
            }
            func_0202bef4(data_020d433c.mEffects, mUnk_1c0, &mUnk_1a4->mPosition);
            if (mUnk_1e0 != 3) {
                func_0202beb8(data_020d433c.mEffects, mUnk_1c0);
                mUnk_1c0 = 0;
            }
            break;
        case 4: {
            if (mUnk_1c0 == 0) {
                break;
            }
            fx32 mag = VEC_Mag(&mUnk_64);
            func_0202bef4(data_020d433c.mEffects, mUnk_1c0, &mUnk_1a4->mPosition);
            if ((float) mag <= 0.0f) {
                func_0202beb8(data_020d433c.mEffects, mUnk_1c0);
                mUnk_1c0 = 0;
            } else {
                MtxFx43 mtx = CameraManager::Get().mMtxC[0];
                VecFx32 pos = mUnk_1a4->mPosition;
                func_01ffcf64(&mtx, &pos);
                func_0202bf60(data_020d433c.mEffects, mUnk_1c0, &mtx);
            }
            break;
        }
    }
    if (mUnk_1c2 == 0) {
        return;
    }
    MtxFx43 mtx = CameraManager::Get().mMtxC[0];
    VecFx32 pos = mUnk_1a4->mPosition;
    VecFx32 off = data_ov007_0227a648;
    mtx._[11]   = 0;
    mtx._[10]   = 0;
    mtx._[9]    = 0;
    MTX_MultVec43(&off, &mtx, &off);
    VEC_Add(&pos, &off, &pos);
    func_01ffcf64(&mtx, &pos);
    func_0202bf60(data_020d433c.mEffects, mUnk_1c2, &mtx);
}

void func_ov007_0225c960(BattleTarget *target, VecFx32 *dir, int unk) {
    target->mUnk_a0.x = dir->x;
    target->mUnk_a0.y = dir->y;
    target->mUnk_a0.z = dir->z;
    target->mUnk_64.x = target->mUnk_a0.x;
    target->mUnk_64.y = target->mUnk_a0.y;
    target->mUnk_64.z = target->mUnk_a0.z;
    target->mUnk_b8.x = 0;
    target->mUnk_b8.y = 0;
    target->mUnk_b8.z = 0;
    target->mUnk_ac.x = 0;
    target->mUnk_ac.y = 0;
    target->mUnk_ac.z = 0;
    if (unk != 0) {
        target->mUnk_c8.x = 0;
        target->mUnk_c8.y = 0;
        target->mUnk_c8.z = 0;
    }
}

void func_ov007_0225c9c0(BattleTarget *target, int a, int b, int c, int d) {
    target->mUnk_64.x = a;
    target->mUnk_64.y = b;
    target->mUnk_64.z = c;
    target->mUnk_a0.x = target->mUnk_64.x;
    target->mUnk_a0.y = target->mUnk_64.y;
    target->mUnk_a0.z = target->mUnk_64.z;
    target->mUnk_b8.x = 0;
    target->mUnk_b8.y = 0;
    target->mUnk_b8.z = 0;
    target->mUnk_ac.x = 0;
    target->mUnk_ac.y = 0;
    target->mUnk_ac.z = 0;
    if (d != 0) {
        target->mUnk_c8.x = 0;
        target->mUnk_c8.y = 0;
        target->mUnk_c8.z = 0;
    }
}

void func_ov007_0225ca18(BattleTarget *target, int a, int b) {
    if (!target->func_ov007_022583b8()) {
        return;
    }
    target->mUnk_1dd = a;
    target->mUnk_140 = b;
    target->mUnk_1db = 0;
}

// non-matching: 14 words; the original sets r0 to 12 (the record size) before the branch and keeps pl in r1. Computing the record pointer in both arms hoists the 12, but arm 1 then colours pl/record as r2/r1 and the two tails are not merged.
void BattleTarget::func_ov007_0225ca48(u32 player, u32 record) {
    BattlePlayer *pl;
    if (player == 0xff) {
        player = data_020d433c.mBattlePlayers->mUnk_28a;
        pl     = data_020d433c.mBattlePlayers->mPlayers[player];
        record = pl->mRecord;
    } else {
        pl = data_020d433c.mBattlePlayers->mPlayers[player];
    }
    BattleRecord *rec = &pl->mRecords[record];
    u16 total         = rec->mUnk_04 + rec->mUnk_06;
    u8 i              = 0;
    do {
        if (total == 0) {
            break;
        }
        mAnims[1]->func_0202a3ec(0, total % 10, 0xc0 + (3 - i) * 12, 0xaa, 1);
        i++;
        total = total / 10;
    } while (i < 4);
    if (data_020d433c.mBattlePlayers->mPlayers[player]->mUnk_fa == 0) {
        return;
    }
    mAnims[0]->Draw(0, 0xf2, 0xab, 1);
}

void BattleTarget::func_ov007_0225cb7c() {
    u8 old = mUnk_1ea;
    mUnk_1ea = (mUnk_50 * 32 / mUnk_54 + old * 3) / 4;
    if (old != mUnk_1ea) {
        u8 hi  = mUnk_50 * 6 / mUnk_54 + 2;
        u8 *buf = (u8 *) OS_Alloc(0x80);
        MI_CpuFill8(buf, 0, 0x80);
        u16 i = 0;
        do {
            if (!(i & 0x10)) {
                u8 v   = (i % 32) % 4 + (i / 32) * 8;
                u8 lo  = 1;
                u8 up  = 1;
                if (v < mUnk_1ea) {
                    lo = hi;
                    if (v + 1 < mUnk_1ea) {
                        up = hi;
                    }
                }
                buf[i] = lo + (up << 4);
            }
            i++;
        } while (i < 0x80);
        DC_FlushRange(buf, 0x80);
        func_02025c1c(0, 4, 0xe00, buf, 0x80, 1);
    }
    mAnims[2]->Draw(0, 0xbe, 0xb7, 1);
    mAnims[3]->Draw(0, 0xce, 0xba, 1);
    SpriteManager::Get().Screen(0).func_02027778(data_ov007_0227a630, 1, 0xce, 0xba, 1, 0x70, 0xb, 0);
}

void BattleTarget::func_ov007_0225cd6c() {
    mAnims[4]->Update();
    mAnims[4]->Draw(1, 0x80, 0x62, 0);
    if (mUnk_1d4 == data_020d433c.mBattlePlayers->mUnk_2a9) {
        mAnims[5]->Update();
        mAnims[5]->Draw(1, 0x80, 0x62, 0);
    }
    mAnims[6]->Update();
    mAnims[6]->Draw(1, 0x80, 0x62, 0);
}

void BattleTarget::func_ov007_0225ce08() {
    GX_SetDispSelect(1);
    SpriteManager::Get().func_02026dec(0, 7);
    GX_SetGraphicsMode(1, 0, 1);
    GXS_SetGraphicsMode(3);
    G2_SetBG0Offset(0, 0);
    G2_SetBG0Priority(2);
    GX_SetVisiblePlane(0x11);
    G2_SetBlendBrightness(0x1f, 0);
    GX_SetVisibleWnd(0);
    G2_SetWndOutsidePlane(0x1f, FALSE);
    G2_BlendNone();
    ChangeDir("/Game/Battle/Obj/");
    SpriteScreen &screen = SpriteManager::Get().Screen(0);
    screen.LoadCharacters("battle_shoot_b_gp.nce_ncg1D.bin", 0, 0);
    screen.LoadPalette("battle_shoot_b_gp_ncl.bin", 0x140, 0);
    ChangeDir("/");
    mUnk_178 = new BattleCommandMap(data_020d433c.mUnk_95);
    mUnk_178->mUnk_151 = 0;
    mUnk_178->vfunc_0c();
    data_020d433c.mGimmicks->mState = 0;
    mUnk_1a4->func_0203d4c4(3);
    if (mUnk_1e8 != 0) {
        UnkStruct_0203d374 *pose = data_020d433c.mBattlePlayers->mUnk_29c->func_ov007_0226fc4c(mUnk_1e8 - 1);
        pose->mUnk_4a            = 0;
    }
    func_ov007_02255cb4(0);
    func_ov007_02256c4c(0);
    mUnk_1e7_0 = 1;
}

BOOL BattleTarget::func_ov007_0225d078() {
    return mUnk_1e2 == 0xc;
}
