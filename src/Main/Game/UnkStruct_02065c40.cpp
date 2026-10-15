#include "Game/UnkStruct_02065c40.hpp"
#include "System/TouchManager.hpp"
#include "System/SoundManager.hpp"
#include "System/FrameClock.hpp"
#include "Graphics/SpriteManager.hpp"
#include "Graphics/Camera.hpp"
#include "System/InputManager.hpp"
#include "System/FrameTask.hpp"
#include "Unknown/UnkStruct_02087898.hpp"

#include "Graphics/CellAnimation.hpp"
#include "Graphics/Display.hpp"
#include "Graphics/Model.hpp"
#include "Game/UnkStruct_020d433c.hpp"
#include "Game/UnkStruct_020d43f0.hpp"

#include <nitro/gx.h>
#include <nitro/mi.h>
#include <nitro/os.h>
#include <stdio.h>
#include "Graphics/CharacterModel.hpp"
#include "System/FileSystem.hpp"
#include "Graphics/UnkStruct_0203d374.hpp"
#include "Graphics/ModelInternal.hpp"

UnkStruct_02065c40 *data_02219288;
extern "C" void MI_CpuCopy8(const void *src, void *dest, u32 size);
extern "C" void DC_FlushRange(void *buffer, u32 size);
void func_02088ec4(UnkStruct_02087898 *object);
extern "C" void MIi_CpuCopy32(const void *src, void *dest, u32 size);
extern const u32 data_020a23cc[8][6];
extern "C" void func_020360cc(UnkStruct_SpriteManager_28 *slot);
extern "C" void func_02038444(u32 a, u32 b);

void func_02089278();

extern const u8 data_020a1c34[];

struct UnkStruct_Pair {
    u8 mUnk_0;
    u8 mUnk_1;
};

extern const UnkStruct_Pair data_020a26a4[][5][10][3];

void func_02086938();

BOOL func_02086a58();

BOOL func_02086b74();

BOOL func_02086e08();

extern const u8 data_020a2118[];

struct UnkStruct_020a1b12 {
    /* 0 */ s16 mUnk_0;
    /* 2 */ u8 mUnk_2;
    /* 3 */ s8 mUnk_3;
    /* 4 */
};

extern const UnkStruct_020a1b12 data_020a1b12[];

UnkStruct_02065c40::UnkStruct_02065c40(u32 unk1, u32 unk2, u32 unk3) :
    SceneBase(0x3e8, 0) {
    mUnk_1010 = NULL;
    mUnk_1014 = NULL;
    mUnk_1018 = NULL;
    mUnk_12ac = 0;
    mUnk_12b8 = 0;
    mUnk_12b9 = 0;
    mUnk_12ba = 0;
    mUnk_12bc = 0;
    mUnk_12be = 0;
    mUnk_12c0 = 0;
    mUnk_12c2 = 0;
    mUnk_12c5 = 0;
    mUnk_12ca = 0;
    mUnk_12cb = 0;
    mUnk_12cc = 0;
    mUnk_12ce = 0;
    mUnk_12d0 = 0;
    mUnk_14d2 = -1;
    mUnk_14d3 = 0;
    mUnk_14d4 = 0;
    mUnk_14d5 = -16;
    mUnk_14d6 = -16;
    mUnk_14d7 = 0;
    mUnk_14d8 = 0;
    mUnk_14d9 = 0;
    mUnk_14de = 1;
    mUnk_14e0 = 0;
    mUnk_14e1 = 0;
    mUnk_14e2 = unk1;
    mUnk_14e4 = 0;
    mUnk_14e5 = 0;
    mUnk_14e6 = 0;
    mUnk_14e7 = 0;
    mUnk_14e8 = 0;
    mUnk_14e9 = 0;
    mUnk_14ea = 0;
    mUnk_14eb = 0;
    mUnk_14ec = 0;
    mUnk_14ed = 0;
    mUnk_14ee = 0;
    mUnk_14ef = 0;
    mUnk_14f0 = unk2;
    mUnk_14f1 = unk3;
    mUnk_14f2 = 0;
    mUnk_14f3 = 0;
    data_02219288 = this;
    MI_CpuClear32(&mUnk_100c, 4);
    MI_CpuClear32(mUnk_11b4, 0x90);
    MI_CpuClear32(mUnk_101c, 0x68);
    MI_CpuClear32(mUnk_1084, 0xa8);
    MI_CpuClear32(mUnk_112c, 0x88);
    for (u8 i = 0; i < 2; i++) {
        mUnk_11b4[i].mUnk_36 = -1;
        mUnk_11b4[i].mUnk_37 = -1;
        mUnk_101c[i].mUnk_30 = -1;
        mUnk_1084[i].mUnk_48 = -1;
        mUnk_1084[i].mUnk_4a = -1;
        mUnk_1084[i].mUnk_4c = 1;
        mUnk_1084[i].mUnk_4f = 0x80;
        mUnk_11b4[i].mUnk_3d = 0x28;
        mUnk_11b4[i].mUnk_3f = 0x80;
        mUnk_11b4[i].mUnk_40 = 0x80;
        mUnk_11b4[i].mUnk_41 = 0x80;
        mUnk_112c[i].mUnk_34 = -1;
        mUnk_112c[i].mUnk_35 = -1;
        mUnk_112c[i].mUnk_38 = 0x80;
        mUnk_112c[i].mUnk_39 = 0x80;
        mUnk_112c[i].mUnk_3a = 0x80;
        mUnk_112c[i].mUnk_40 = 0;
        mUnk_112c[i].mUnk_3f = 10;
    }
    MI_CpuClear32(&mUnk_12b0, 4);
    MI_CpuClear32(&mUnk_12b4, 4);
    mUnk_1288 = new Camera();
    mUnk_1288->SetUp(0, 0x1000, 0);
    mUnk_1288->SetClip(0x1000, 0x1388000);
    CameraManager::Get().mActive = 0;
    CameraManager::Get().func_02026504(1, mUnk_1288, 0);
    func_02069bdc();
    func_02069c6c();
    mUnk_1018 = new UnkStruct_02087898();
    G2_SetBlendBrightness(1, -16);
    if (mUnk_14f0 != 0) {
        FrameTask::Get().func_0203dd74(0);
    }
    FrameTask::Get().func_0203dd74(1);
    data_020d433c.mUnk_9a = 0x11;
    data_020d433c.mUnk_9a_0 = data_020d43f0[0].mUnk_28_6;
    data_020d433c.mUnk_7a = 1;
    if (data_020d433c.mUnk_84 != 0) {
        func_02038444(SoundManager::Get().mUnk_9c, 2);
    } else {
        SoundManager::Get().func_020245a4();
    }
    SoundManager::Get().func_020246d4(3);
    func_020666d8();
}

UnkStruct_02065c40::~UnkStruct_02065c40() {
    func_02023a54();
    if (mUnk_100c != NULL) {
        delete mUnk_100c;
    }
    func_02067708(1);
    for (u8 i = 0; i < 2; i++) {
        for (u8 j = 0; j < 3; j++) {
            if (mUnk_1084[i].mUnk_3c[j] != NULL) {
                OS_Free(mUnk_1084[i].mUnk_3c[j]);
            }
        }
    }
    ModelInstance *instance = mUnk_1010;
    if (instance != NULL) {
        instance->mUnk_68 = 0xf;
        delete instance;
    }
    delete mUnk_1288;
    UnkStruct_02029ca8 *lights = mUnk_1014;
    if (lights != NULL) {
        delete lights;
    }
    CameraManager::Get().func_02026504(0, NULL, 0);
    if (mUnk_14f0 != 0) {
        FrameTask::Get().func_0203dd74(0);
    }
    FrameTask::Get().func_0203dd74(1);
    SoundManager::Get().func_02024754(1);
    if (data_020d433c.mUnk_84 == 0) {
        SoundManager::Get().func_02024688(1);
    }
}

void UnkStruct_02065c40::func_020666d8() {
    SpriteManager::Get().func_0202740c();
    UnkStruct_SpriteManager_28 slot;
    func_020360cc(&slot);
    for (u8 i = 0; i < 8; i++) {
        for (u8 j = 0; j < 6; j++) {
            slot.mUnk_00[j] = data_020a23cc[i][j];
        }
        SpriteManager::Get().func_02027578(i, &slot);
    }
}



struct UnkStruct_020b4324_entry {
    UnkStruct_Pair mPairs[3];
    u8 mUnk_06;
    u8 mUnk_07;
    u8 mUnk_08;
    u8 mUnk_09;
    u8 mUnk_0a;
    u8 mUnk_0b;
};

struct UnkStruct_020b4324 {
    UnkStruct_020b4324_entry mEntries[10];
    u8 mUnk_78;
    u8 mUnk_79;
};

extern UnkStruct_020b4324 *data_020b4324[11];
extern const u8 data_020a1c38[];

u32 UnkStruct_02065c40::func_020667ec() {
    u8 type = data_020d43f0[0].mUnk_42;
    if (type >= 11) {
        return 7;
    }
    return data_020b4324[type]->mUnk_78;
}

// `result` as s16 gives the original's reload at the early return (`ldrcs r0, [sp]`); as a spilled s32 the -1 still
// in r0 is reused and the reload dropped. `pending` declared first puts it in the higher stack slot, like the original.
s32 UnkStruct_02065c40::func_02066814(u32 unk1) {
    s32 pending;
    s16 result = -1;
    u8 type    = data_020d43f0[0].mUnk_42;
    if (type >= 10) {
        return result;
    }
    if (mUnk_14e2 == data_020b4324[type]->mUnk_78) {
        u8 saved = data_020d433c.mUnk_7d;
        s32 done = 0, advance = 0;
        pending = 0;
        if (unk1 != 0) {
            data_020d433c.mUnk_7d = 1;
        }
        u8 position = data_020d43f0[0].mUnk_43;
        u8 index    = position % 10;
        if ((index == 0) & (saved == 0)) {
            result = data_020b4324[type]->mUnk_79;
        } else {
            BOOL seen = FALSE;
            UnkStruct_020b4324_entry *entry = &data_020b4324[type]->mEntries[index];
            switch (entry->mUnk_06) {
                case 0:
                    if (unk1 != 0) {
                        done = 1;
                    }
                    break;
                case 1:
                    seen = TRUE;
                    if (unk1 != 0) {
                        advance = seen;
                        if (type == 8 && data_020b4324[type]->mEntries[index + 1].mUnk_06 == 0) {
                            pending = 1;
                            break;
                        }
                        s32 next = index + 1;
                        if (next < 10) {
                            func_02066c9c(type, (u8) next);
                            if (data_020b4324[type]->mEntries[index].mUnk_0a != 0 && data_020b4324[type]->mEntries[next].mUnk_06 != 1) {
                                mUnk_14e9 = 1;
                            }
                        }
                    }
                    break;
                default:
                    if (saved == 2) {
                        seen = TRUE;
                        if (unk1 != 0) {
                            advance = seen;
                            if (entry->mUnk_0b == 0) {
                                pending = 1;
                            }
                        }
                        s32 next = index + 1;
                        if (next < 10) {
                            func_02066c9c(type, (u8) next);
                            if (unk1 != 0) {
                                if (data_020b4324[type]->mEntries[index].mUnk_0a != 0 && data_020b4324[type]->mEntries[next].mUnk_06 != 1) {
                                    mUnk_14e9 = 1;
                                }
                            }
                        }
                    } else {
                        func_02066c9c(type, index);
                        if (mUnk_14e2 == 0 || mUnk_14e9 == 1) {
                            func_0206791c(5);
                        }
                    }
                    break;
            }
            if (data_020d43f0[0].mUnk_5bf_0) {
                mUnk_14e9 = 0;
            }
            if (seen) {
                u8 value = data_020b4324[type]->mEntries[index].mUnk_09;
                if (value != 0) {
                    result = value;
                }
            }
        }
        if (unk1 != 0) {
            if ((advance | done) && pending != 0) {
                mUnk_14ea = 1;
            }
            if (advance != 0) {
                u32 limit = 10;
                if (data_020d43f0[0].mUnk_5bf_0) {
                    limit = func_02067240(type);
                }
                u8 now = ++data_020d43f0[0].mUnk_43;
                if (now >= limit) {
                    done = 1;
                } else if (data_020b4324[type]->mEntries[now].mUnk_06 == 0) {
                    done = 1;
                }
                data_020d433c.mUnk_7d = 0;
            }
            if (done != 0) {
                if (data_020d43f0[0].mUnk_5bf_0) {
                    switch (data_020d43f0[0].mUnk_42) {
                        case 1:
                            data_020d43f0[0].mUnk_5bf |= 0x2;
                            break;
                        case 3:
                            data_020d43f0[0].mUnk_5bf |= 0x4;
                            break;
                        case 6:
                            data_020d43f0[0].mUnk_5bf |= 0x8;
                            break;
                        case 7:
                            data_020d43f0[0].mUnk_5bf |= 0x10;
                            break;
                        case 8:
                            data_020d43f0[0].mUnk_5bf |= 0x20;
                            break;
                        case 9:
                            data_020d43f0[0].mUnk_5bf |= 0x40;
                            break;
                    }
                    data_020d43f0[0].mUnk_42 = 10;
                    func_0206791c(6);
                    mUnk_14ea = 1;
                } else {
                    u32 next = 0;
                    if (data_020b4324[type]->mUnk_78 != 0) {
                        next = 6;
                    }
                    if (next != 0) {
                        func_0206791c(next);
                    }
                    if (++data_020d43f0[0].mUnk_42 == 10) {
                        func_0206791c(8);
                    }
                }
                data_020d43f0[0].mUnk_43 = 0;
                data_020d433c.mUnk_7d = 0;
                mUnk_14e2 = 7;
            }
            if (advance | done) {
                func_02089278();
            }
        }
    }
    if (data_020d43f0[0].mUnk_5bf_0) {
        result = -1;
    }
    return result;
}

BOOL UnkStruct_02065c40::func_02066c7c() {
    return func_02066814(0) != -1;
}

void UnkStruct_02065c40::func_02066c9c(u32 unk1, u32 unk2) {
    data_020d433c.mUnk_99 = 3;
    data_020d433c.mPlayerCount = 2;
    data_020d433c.mUnk_98 = 0;
    for (u8 i = 0; i < 4; i++) {
        data_020d433c.mUnk_9f[i] = i;
        data_020d433c.mUnk_a3[i] = 0;
        data_020d433c.mUnk_9b[i] = i;
    }
    switch (mUnk_14e2) {
        case 3:
            data_020d433c.mUnk_7b = 8;
            break;
        case 2:
            data_020d433c.mUnk_7b = 0xc;
            break;
        default:
            data_020d433c.mUnk_7b = 9;
            break;
    }
    data_020d433c.mUnk_9a   = 0x11;
    data_020d433c.mUnk_9a_0 = data_020d43f0[0].mUnk_28_6;
    for (u8 j = 0; j < 3; j++) {
        data_020d433c.mUnk_9f[j + 1] = func_02067120(unk1, unk2, (u8) (j + 1), 0);
        data_020d433c.mUnk_a3[j + 1] = func_02067120(unk1, unk2, (j + 1) & 0xff, 1);
    }
    UnkStruct_020b4324_entry *entry = &data_020b4324[unk1]->mEntries[unk2];
    data_020d433c.mUnk_95 = entry->mUnk_07;
    data_020d433c.mUnk_96 = entry->mUnk_08;
    if (data_020d43f0[0].mUnk_5bf_0 && (data_020d43f0[0].mUnk_580 & 0x80)) {
        for (u8 k = 0; k < 3; k++) {
            if (data_020d433c.mUnk_9f[k + 1] == 7) {
                data_020d433c.mUnk_a3[k + 1] = 2;
            }
        }
    }
    switch (data_020b4324[unk1]->mEntries[unk2].mUnk_06) {
        case 3:
            data_020d433c.mUnk_98 = 1;
            data_020d433c.mPlayerCount = 4;
            data_020d433c.mUnk_9b[0] = 1;
            data_020d433c.mUnk_9b[1] = 0;
            data_020d433c.mUnk_9b[2] = 3;
            data_020d433c.mUnk_9b[3] = 2;
            break;
        case 4:
            data_020d433c.mUnk_99 = 6;
            break;
        case 5:
            data_020d433c.mPlayerCount = 4;
            break;
    }
    data_020d433c.mUnk_9f[0] = 0x15;
    u8 type = data_020d43f0[0].mUnk_42;
    if ((type == 9) & (data_020d43f0[0].mUnk_43 == 8)) {
        data_020d433c.mUnk_8a_0 = 1;
    } else {
        data_020d433c.mUnk_8a_0 = 0;
    }
    BOOL reached = unk2 >= func_02067240(unk1);
    BOOL known   = func_020670b4(type) != 0;
    if (reached & known) {
        data_020d433c.mUnk_8a_4 = 1;
    } else {
        data_020d433c.mUnk_8a_4 = 0;
    }
}

u32 UnkStruct_02065c40::func_02066f60() {
    char path[0x80];
    if (mUnk_14e2 == 6) {
        return 0;
    }
    s32 command = func_02066814(0);
    if (command > -1) {
        switch (command - 0x64) {
            case 0:
                sprintf(path, "cmd_ST_mev_A001.bin");
                break;
            case 1:
                sprintf(path, "cmd_ST_mev_A002.bin");
                break;
            case 2:
                sprintf(path, "cmd_ST_mev_B001.bin");
                break;
            case 3:
                sprintf(path, "cmd_ST_mev_B002.bin");
                break;
            default:
                sprintf(path, "cmd_ST_mev_%03d.bin", command);
                break;
        }
        for (u8 i = 0; i < 2; i++) {
            if (command == data_020a1c34[i]) {
                GXS_SetVisiblePlane(0);
                break;
            }
        }
        func_0206eb10(path);
        mUnk_14e8 = 1;
        return 1;
    }
    func_02066814(1);
    if (mUnk_14e9 != 0) {
        func_0206791c(5);
    }
    return 0;
}

u32 UnkStruct_02065c40::func_020670b4(u32 id) {
    switch (id) {
        case 1: return 1;
        case 3: return 2;
        case 6: return 3;
        case 7: return 4;
        case 8: return 5;
        case 9: return 6;
    }
    return 0;
}

u8 UnkStruct_02065c40::func_02067120(u32 unk1, u32 unk2, u32 unk3, u32 unk4) {
    if (unk3 != 0) {
        u8 slot = unk3 - 1;
        if (data_020d43f0[0].mUnk_5bf_0) {
            u32 kind = func_020670b4(unk1);
            if (kind == 0) {
                kind = 1;
            }
            u8 group = data_020d43f0[0].mUnk_25;
            u8 row = group % 5;
            u8 flag = *(u8 *) &unk4;
            if (flag == 0) {
                return data_020a26a4[kind - 1][row][unk2][slot].mUnk_0;
            }
            return data_020a26a4[kind - 1][row][unk2][slot].mUnk_1;
        }
        if (*(u8 *) &unk4 == 0) {
            return data_020b4324[unk1]->mEntries[unk2].mPairs[slot].mUnk_0;
        }
        return data_020b4324[unk1]->mEntries[unk2].mPairs[slot].mUnk_1;
    }
    if (*(u8 *) &unk4 == 0) {
        return 0x15;
    }
    return 0;
}

u8 UnkStruct_02065c40::func_02067240(u32 id) {
    return data_020a1c38[func_020670b4(id)];
}

void UnkStruct_02065c40::Update() {
    if (mUnk_14e4 != 0) {
        mUnk_14e5++;
        mUnk_14e5 &= 1;
    }
    func_02067e74();
    func_02069d54();
    CameraManager &manager = CameraManager::Get();
    Camera *camera         = manager.mCameras[manager.mActive];
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
    func_020679d8();
    func_0206a208();
    func_0206a01c();
    func_0206ada4();
    func_02068714();
    func_0206aa34();
    func_0206a850();
    func_0206aecc();
    if (mUnk_14e0 != 0) {
        if (mUnk_12b9 < 10) {
            mUnk_12b9 = 10;
        }
    }
    switch (mUnk_12b9) {
        case 0:
            func_02067b08();
            mUnk_12b9 = 5;
            break;
        case 1:
            if (func_0206ed40()) {
                mUnk_12b9++;
            }
            break;
        case 2:
            if (func_0206ef08()) {
                mUnk_12b9++;
            }
            break;
        case 3:
            func_0206b09c();
            break;
        case 4:
            func_0206d3b8();
            break;
        case 5:
            if (mUnk_14ea != 0) {
                func_02086938();
                if (func_02086a58()) {
                    mUnk_14eb = 1;
                    mUnk_12b9 = 7;
                } else if (func_02086b74()) {
                    mUnk_14eb = 2;
                    mUnk_12b9 = 7;
                } else if (func_02086e08()) {
                    mUnk_14eb = 3;
                    mUnk_12b9 = 7;
                } else {
                    mUnk_12b9++;
                }
            } else {
                mUnk_12b9 = 8;
            }
            break;
        case 6:
            if (mUnk_14ea != 0) {
                break;
            }
            func_02067b08();
            mUnk_12b9 = 8;
            break;
        case 7:
            if (mUnk_14eb != 0) {
                break;
            }
            func_02067b08();
            mUnk_12b9 = 5;
            break;
        case 8:
            if (func_02066f60()) {
                if (mUnk_12b9 == 8) {
                    mUnk_12b9 = 1;
                }
            } else {
                if (mUnk_12b9 == 8) {
                    mUnk_12b9 = 9;
                }
            }
            if (mUnk_14ea != 0) {
                mUnk_12b9 = 5;
            }
            break;
        case 10:
            mUnk_1018->func_02087cc0();
            func_02067708(1);
            RequestScene(1);
            mFinished = 1;
            break;
    }
}

void UnkStruct_02065c40::func_020676f8() {
    mUnk_14e0 = 1;
}

void UnkStruct_02065c40::func_02067708(u32 unk1) {
    for (u8 i = 0; i < 2; i++) {
        UnkStruct_0205401c *model = mUnk_11b4[i].mUnk_2c;
        if (model != NULL) {
            model->func_02054034();
            delete model;
        }
        mUnk_11b4[i].mUnk_2c = NULL;
        model                = mUnk_11b4[i].mUnk_30;
        if (model != NULL) {
            model->func_02054034();
            delete model;
        }
        mUnk_11b4[i].mUnk_30 = NULL;
        UnkStruct_0203d374 *object = mUnk_112c[i].mUnk_2c;
        if (object != NULL) {
            delete object;
        }
        mUnk_112c[i].mUnk_2c = NULL;
        object               = mUnk_112c[i].mUnk_30;
        if (object != NULL) {
            delete object;
        }
        mUnk_112c[i].mUnk_30 = NULL;
        if (unk1 != 0) {
            ModelInstance *instance = mUnk_101c[i].mUnk_2c;
            if (instance != NULL) {
                instance->mUnk_68 = 0xf;
                delete instance;
                mUnk_101c[i].mUnk_2c = NULL;
            }
            mUnk_101c[i].mUnk_30 = -1;
        }
        func_020695e8(i);
        mUnk_11b4[i].mUnk_36 = -1;
        mUnk_11b4[i].mUnk_37 = -1;
        mUnk_1084[i].mUnk_48 = -1;
        mUnk_1084[i].mUnk_4a = -1;
        mUnk_112c[i].mUnk_34 = -1;
        mUnk_112c[i].mUnk_35 = -1;
        mUnk_11b4[i].mUnk_43 = 0;
        mUnk_112c[i].mUnk_42 = 0;
        mUnk_1084[i].mUnk_51 = 0;
    }
    ModelInstance *instance;
    if (unk1 != 0 && (instance = mUnk_1010) != NULL) {
        instance->mUnk_68 = 0xf;
        delete instance;
        mUnk_1010 = NULL;
    }
}

BOOL UnkStruct_02065c40::func_020678e8() {
    return mUnk_12b9 != 9;
}

u32 UnkStruct_02065c40::func_02067900() {
    if (mUnk_14ea | mUnk_14eb) {
        return 0;
    }
    return mUnk_14e1;
}

void UnkStruct_02065c40::func_0206791c(u32 unk1) {
    mUnk_14e1 = unk1;
    mUnk_12c5 = 7;
    mUnk_12b9 = 4;
}

BOOL UnkStruct_02065c40::func_02067938() {
    return mUnk_14e6;
}

void UnkStruct_02065c40::func_02067944() {
    mUnk_14e6 = 0;
}

u32 UnkStruct_02065c40::func_02067954() {
    if (mUnk_12b9 == 6) {
        return mUnk_14ea;
    }
    return 0;
}

void UnkStruct_02065c40::func_0206796c() {
    mUnk_14ea = 0;
}

u32 UnkStruct_02065c40::func_0206797c() {
    return mUnk_14eb;
}

void UnkStruct_02065c40::func_02067988() {
    mUnk_14eb = 0;
}

extern const s8 data_020a1b2a[][5];
extern const s8 data_020a1ac6[][5];
extern const s8 data_020a1ad5[][5];
extern const s8 data_020a1ae4[][5];
extern const s8 data_020a1af3[][5];
extern const s8 data_020a1b02[][5];

void UnkStruct_02065c40::func_02067998(u8 unk1) {
    if (unk1) {
        NNSG3dAnmObj *obj = mUnk_101c[0].mUnk_2c->mAnim->func_02029a58(1, unk1);
        mUnk_101c[0].mUnk_2c->func_02028f54(obj);
    } else {
        mUnk_101c[0].mUnk_2c->func_02028f88();
    }
}

void UnkStruct_02065c40::func_020679d8() {
    if (mUnk_14d4 == 0) {
        func_02067a30();
        return;
    }
    if (mUnk_14d5 == 0) {
        return;
    }
    mUnk_14d7 = mUnk_14d5;
    mUnk_14d6 = 0;
    mUnk_14d8 = 0;
    s32 v = mUnk_14d5;
    if (v < 0) {
        v = -v;
    }
    mUnk_14d9 = v;
    func_02067a30();
}

void UnkStruct_02065c40::func_02067a30() {
    if (mUnk_14d9 == 0) {
        return;
    }
    s32 t = mUnk_14d8 + 1;
    if (t > mUnk_14d9) {
        t = mUnk_14d9;
    }
    mUnk_14d8 = t;
    s8 from = mUnk_14d7;
    s8 to   = mUnk_14d6;
    s8 d = to - from;
    int v = from + d * mUnk_14d8 / mUnk_14d9;
    if (v > 16) {
        v = 16;
    } else if (v < -16) {
        v = -16;
    }
    mUnk_14d5 = v;
    if (mUnk_14d9 <= mUnk_14d8) {
        mUnk_14d5 = mUnk_14d6;
        mUnk_14d7 = mUnk_14d6;
        mUnk_14d8 = 0;
        mUnk_14d9 = 0;
    }
    if (mUnk_14d3 == 0) {
        G2_SetBlendBrightness(0x21, mUnk_14d5);
    }
}

extern const u8 data_020a1bae[];
#define REG_G3_DISP3DCNT (*(vu16 *) 0x4000060)

void UnkStruct_02065c40::func_02067b08() {
    char path[0x80];
    GX_SetVisiblePlane(0x11);
    G2_SetBG0Offset(0, 0);
    G2_SetBG0Priority(3);
    REG_G3_DISP3DCNT = (REG_G3_DISP3DCNT & ~0x3000) | 0x20;
    ChangeDir("/story/");
    if (mUnk_14e4 == 0 && (mUnk_14f1 | func_02066c7c())) {
        ChangeDir("/story/");
        SpriteScreen &screen = SpriteManager::Get().Screen(1);
        screen.func_02027624(3);
        sprintf(path, "cutscene_b_obj.nce_ncg1D.bin");
        screen.LoadCharacters(path, 0xd000, 0);
        sprintf(path, "cutscene_b_obj_ncl.bin");
        screen.func_02027fec(path, 0, 0);
        if (mUnk_100c != NULL) {
            delete mUnk_100c;
        }
        mUnk_14f2 = 0;
        mUnk_100c = new CellAnimation("cutscene_b_obj_nce.bin", mUnk_14f2, 1, 1);
        mUnk_100c->mCharBase = 0x1a0;
        mUnk_100c->mUnk_11   = 0;
        if (mUnk_14e2 == 5) {
            mUnk_14f1 = 0;
            mUnk_14ed = 1;
        }
    }
    ChangeDir("/");
    if (mUnk_1010 == NULL) {
        SpriteManager::Get().func_0202753c(7);
        NitroTexture *texture = new NitroTexture("story/mdl/cut_shadow.nsbtx");
        NitroModel *model     = new NitroModel;
        model->func_020289e0("story/mdl/cut_shadow.nsbmd", texture);
        texture->func_02028344();
        mUnk_1010 = model->func_02028b78(NULL);
    }
    if (mUnk_1014 == NULL) {
        mUnk_1014 = new UnkStruct_02029ca8(data_020a1bae);
        func_02069c6c();
    }
    mUnk_1018->func_02087eec();
}

void UnkStruct_02065c40::func_02067e2c(u32 index, u8 unk2) {
    UnkStruct_0205401c *model = mUnk_11b4[index].mUnk_2c;
    if (model == NULL) {
        return;
    }
    model->func_02054694(unk2);
    mUnk_11b4[index].mUnk_3b = 0;
    mUnk_11b4[index].mUnk_3e = unk2;
}

void UnkStruct_02065c40::func_02067e74() {
    for (u8 i = 0; i < 2; i++) {
        if (mUnk_11b4[i].mUnk_43 != 0) {
            func_0206852c(i);
            mUnk_11b4[i].mUnk_43 = 0;
        }
        if (mUnk_112c[i].mUnk_42 != 0) {
            func_02069a24(i);
            mUnk_112c[i].mUnk_42 = 0;
        }
        if (mUnk_1084[i].mUnk_51 != 0) {
            func_02069340(i);
            mUnk_1084[i].mUnk_51 = 0;
        }
    }
}

BOOL UnkStruct_02065c40::func_02067f30(u32 index, s32 value) {
    if (value < 0) {
        func_02067e2c(index, 0);
        UnkStruct_0205401c *model = mUnk_11b4[index].mUnk_2c;
        if (model != NULL) {
            model->func_02054034();
            delete model;
        }
        mUnk_11b4[index].mUnk_2c = NULL;
        return FALSE;
    }
    mUnk_11b4[index].mUnk_3a = value;
    if (mUnk_11b4[index].mUnk_2c != NULL) {
        if (mUnk_11b4[index].mUnk_3a != mUnk_11b4[index].mUnk_39) {
            if (mUnk_11b4[index].mUnk_2c->func_020546b0()) {
                mUnk_11b4[index].mUnk_3b = 1;
            }
            if (mUnk_11b4[index].mUnk_36 == 0x15) {
                ((CharacterModel *) mUnk_11b4[index].mUnk_2c->mUnk_04)->mParts[0]->mAnim->mUnk_10 = 1;
            } else {
                ((ModelTransform *) mUnk_11b4[index].mUnk_2c->mUnk_08)->mModel->mAnim->mUnk_10 = 1;
            }
            func_0206abf4(index);
        }
        return FALSE;
    }
    if (index == 1) {
        func_020695e8(0);
    }
    func_02067e2c(index, 0);
    UnkStruct_0205401c *model = mUnk_11b4[index].mUnk_2c;
    if (model != NULL) {
        model->func_02054034();
        delete model;
    }
    mUnk_11b4[index].mUnk_2c = NULL;
    return TRUE;
}

void UnkStruct_02065c40::func_02068074(u32 index) {
    UnkStruct_0205401c *model = mUnk_11b4[index].mUnk_30;
    if (model != NULL) {
        model->func_02054034();
        delete model;
    }
    mUnk_11b4[index].mUnk_30 = NULL;
    if (mUnk_11b4[index].mUnk_37 < 0) {
        return;
    }
    mUnk_11b4[index].mUnk_30 = new UnkStruct_0205401c();
    mUnk_11b4[index].mUnk_30->func_020543e8((u8) mUnk_11b4[index].mUnk_37, 0, 0, 0);
}

BOOL UnkStruct_02065c40::func_020680f8(u32 index, u32 flag) {
    u8 variant;
    s8 kind;
    UnkStruct_0205401c **slot;
    if (flag) {
        variant = mUnk_11b4[index].mUnk_38;
        kind    = mUnk_11b4[index].mUnk_37;
        slot    = &mUnk_11b4[index].mUnk_30;
    } else {
        variant = mUnk_11b4[index].mUnk_3a;
        kind    = mUnk_11b4[index].mUnk_36;
        slot    = &mUnk_11b4[index].mUnk_2c;
    }
    if (kind < 0) {
        return TRUE;
    }
    func_020682a0(index, kind);
    if (flag) {
        if (!(*slot)->func_020544e4()) {
            return FALSE;
        }
    } else {
        *slot = new UnkStruct_0205401c();
        u8 id = kind;
        (*slot)->func_0205411c(id, 0, 0, 0);
        u8 a = func_020687a8(id, variant, 0);
        u8 b = func_020687a8(id, variant, 3);
        (*slot)->func_02054644(b, a);
        if (kind == 0x15) {
            ((CharacterModel *) (*slot)->mUnk_04)->mParts[0]->mAnim->mUnk_08 = 1;
        } else {
            ((ModelTransform *) (*slot)->mUnk_08)->mModel->mAnim->mUnk_08 = 1;
        }
        mUnk_11b4[index].mUnk_39 = variant;
        (*slot)->func_02054694(0);
        mUnk_11b4[index].mUnk_3e = 0;
        MI_CpuClear16(&mUnk_11b4[index], 0x18);
        MI_CpuClear16(mUnk_11b4[index].mUnk_18, 0x14);
        func_020683cc(index);
    }
    return TRUE;
}

void UnkStruct_02065c40::func_020682a0(u32 index, u32 kind) {
    if (kind == 0x15) {
        SpriteManager::Get().func_0202753c(3);
    } else if (index == 0) {
        SpriteManager::Get().func_0202753c(2);
    } else {
        SpriteManager::Get().func_0202753c(1);
    }
}

struct UnkStruct_020a22ac {
    /* 0 */ const char *mName;
    /* 4 */ s16 mX;
    /* 6 */ s16 mY;
    /* 8 */
};
extern const UnkStruct_020a22ac data_020a22ac[18];

static inline fx32 FxRound(s32 v) {
    s32 t = v;
    return (fx32) (t > 0 ? t + 0.5f : t - 0.5f);
}

#define FX_FROM_INT_ROUND(x) ((fx32) (((x) > 0) ? (((x) << 12) + 0.5f) : (((x) << 12) - 0.5f)))


struct UnkStruct_020a2554 {
    /* 0 */ const char *mName;
    /* 4 */ u8 mDir;
    /* 5 */ u8 mUnk_05;
    /* 6 */ u8 mData;
    /* 7 */ PAD(0x07, 0x08);
    /* 8 */
};
extern const UnkStruct_020a2554 data_020a2554[42];
extern const char *data_020b431c[];

struct UnkStruct_020a1bd6 {
    /* 0 */ u8 mUnk_0;
    /* 1 */ u8 mUnk_1;
    /* 2 */ u8 mUnk_2;
    /* 3 */
};
extern const UnkStruct_020a1bd6 data_020a1bd6[];

void UnkStruct_02065c40::func_020683cc(u32 index) {
    switch (mUnk_11b4[index].mUnk_36) {
        case 2:
            mUnk_11b4[index].mUnk_34 = 0x3c;
            break;
        case 3:
            mUnk_11b4[index].mUnk_34 = -0x14;
            break;
        case 14:
            mUnk_11b4[index].mUnk_34 = 0x28;
            break;
        case 11:
            mUnk_11b4[index].mUnk_34 = -0x78;
            break;
        case 12:
            mUnk_11b4[index].mUnk_34 = 0x28;
            break;
        case 9:
            mUnk_11b4[index].mUnk_34 = -0x28;
            break;
        default:
            mUnk_11b4[index].mUnk_34 = 0;
            break;
    }
    switch (mUnk_11b4[index].mUnk_36) {
        case 13:
            mUnk_11b4[index].mUnk_3d = 0x3c;
            break;
        case 14:
            mUnk_11b4[index].mUnk_3d = 0x1e;
            break;
        default:
            mUnk_11b4[index].mUnk_3d = 0x28;
            break;
    }
    mUnk_11b4[index].mUnk_3f = 0x80;
    mUnk_11b4[index].mUnk_40 = 0x80;
    mUnk_11b4[index].mUnk_41 = 0x80;
    mUnk_11b4[index].mUnk_42 = 0;
    mUnk_11b4[index].mUnk_3c = 0;
}

void UnkStruct_02065c40::func_0206852c(u32 index) {
    if (mUnk_11b4[index].mUnk_30 == NULL) {
        return;
    }
    UnkStruct_0205401c *old = mUnk_11b4[index].mUnk_2c;
    if (old != NULL) {
        old->func_02054034();
        delete old;
    }
    mUnk_11b4[index].mUnk_2c = mUnk_11b4[index].mUnk_30;
    mUnk_11b4[index].mUnk_30 = NULL;
    mUnk_11b4[index].mUnk_36 = mUnk_11b4[index].mUnk_37;
    mUnk_11b4[index].mUnk_39 = mUnk_11b4[index].mUnk_38;
    mUnk_11b4[index].mUnk_3a = mUnk_11b4[index].mUnk_38;
    func_020683cc(index);
    s8 kind    = mUnk_11b4[index].mUnk_36;
    s8 variant = mUnk_11b4[index].mUnk_3a;
    func_020682a0(index, kind);
    mUnk_11b4[index].mUnk_2c->func_0205451c();
    u8 variantId = variant;
    u8 id        = kind;
    u8 a         = func_020687a8(id, variantId, 0);
    u8 b         = func_020687a8(id, variantId, 3);
    mUnk_11b4[index].mUnk_2c->func_02054644(b, a);
    if (kind == 0x15) {
        ((CharacterModel *) mUnk_11b4[index].mUnk_2c->mUnk_04)->mParts[0]->mAnim->mUnk_08 = 1;
    } else {
        ((ModelTransform *) mUnk_11b4[index].mUnk_2c->mUnk_08)->mModel->mAnim->mUnk_08 = 1;
    }
}

void UnkStruct_02065c40::func_0206867c(u32 index, u32 value) {
    if (value == mUnk_11b4[index].mUnk_3c) {
        return;
    }
    mUnk_11b4[index].mUnk_3c = value;
    if (mUnk_11b4[index].mUnk_2c == NULL) {
        return;
    }
    mUnk_11b4[index].mUnk_2c->func_0205466c(mUnk_11b4[index].mUnk_3c);
    if (mUnk_11b4[index].mUnk_3b == 0) {
        return;
    }
    if (mUnk_11b4[index].mUnk_36 == 0x15) {
        ((CharacterModel *) mUnk_11b4[index].mUnk_2c->mUnk_04)->mParts[0]->mAnim->mUnk_08 = 0;
    } else {
        ((ModelTransform *) mUnk_11b4[index].mUnk_2c->mUnk_08)->mModel->mAnim->mUnk_08 = 0;
    }
}

void UnkStruct_02065c40::func_02068714() {
    for (u8 i = 0; i < 2; i++) {
        s32 kind = mUnk_11b4[i].mUnk_36;
        if (kind < 0) {
            continue;
        }
        if (kind == mUnk_1018->mUnk_42) {
            if (mUnk_1018->func_02088ef4()) {
                func_0206867c(i, 1);
            } else {
                func_0206867c(i, 0);
            }
        } else {
            func_0206867c(i, 0);
        }
    }
}

s8 UnkStruct_02065c40::func_020687a8(s32 kind, u32 index, u32 column) {
    if (index < 6) {
        return data_020a1b2a[index][column];
    }
    u8 row = index - 6;
    switch (kind) {
        case 0:
            return data_020a1ac6[row][column];
        case 0x15:
            return data_020a1ae4[row][column];
        case 6:
            return data_020a1ad5[row][column];
        case 0x12:
            return data_020a1af3[row][column];
        default:
            return data_020a1b02[row][column];
    }
}

void UnkStruct_02065c40::func_02068884(u32 index) {
    if (mUnk_11b4[index].mUnk_2c == NULL) {
        return;
    }
    s16 rot[3];
    rot[0] = mUnk_11b4[index].mUnk_18[0];
    rot[1] = mUnk_11b4[index].mUnk_18[1];
    rot[2] = mUnk_11b4[index].mUnk_18[2];
    mUnk_11b4[index].mUnk_2c->func_020546e4((const u16 *) rot);
}

BOOL UnkStruct_02065c40::func_020688e0(s32 value) {
    if (value == mUnk_101c[0].mUnk_30) {
        return FALSE;
    }
    if (value != -2) {
        mUnk_101c[0].mUnk_30 = value;
    }
    ModelInstance *instance = mUnk_101c[0].mUnk_2c;
    if (instance != NULL) {
        instance->mUnk_68 = 0xf;
        delete instance;
        mUnk_101c[0].mUnk_2c = NULL;
    }
    if (mUnk_101c[0].mUnk_30 < 0) {
        return FALSE;
    }
    return TRUE;
}

BOOL UnkStruct_02065c40::func_02068954() {
    char path[0x80];
    SpriteManager::Get().func_0202753c(0);
    s32 index = mUnk_101c[0].mUnk_30;
    if (index > 0x11) {
        index = 0x11;
    } else if (index < 0) {
        index = 0;
    }
    u8 id = index;
    ChangeDir("/Story/Map/");
    sprintf(path, "%s.nsbtx", data_020a22ac[id].mName);
    NitroTexture *texture = new NitroTexture(path);
    sprintf(path, "%s.nsbmd", data_020a22ac[id].mName);
    NitroModel *model = new NitroModel;
    model->func_020289e0(path, texture);
    texture->func_02028344();
    if ((id != 8) & (id != 9)) {
        sprintf(path, "%s_data.bin", data_020a22ac[id].mName);
        ModelAnimData *animData = new ModelAnimData(path);
        ModelAnim *anim         = new ModelAnim(animData, model);
        mUnk_101c[0].mUnk_2c    = model->func_02028b78(anim);
        mUnk_101c[0].mUnk_2c->func_02028e54(0, 1, 0x1000);
    } else {
        mUnk_101c[0].mUnk_2c = model->func_02028b78(NULL);
    }
    ChangeDir("/");
    func_02069bdc();
    func_02069c6c();
    MI_CpuClear16(mUnk_101c[0].mUnk_18, 0x14);
    MI_CpuClear16(&mUnk_101c[0], 0x18);
    mUnk_101c[0].mUnk_00 = 0;
    mUnk_101c[0].mUnk_04 = FX_FROM_INT_ROUND(data_020a22ac[id].mX);
    mUnk_101c[0].mUnk_08 = FX_FROM_INT_ROUND(data_020a22ac[id].mY);
    mUnk_101c[0].mUnk_0c[0] = 0;
    mUnk_101c[0].mUnk_0c[1] = data_020a22ac[id].mX;
    mUnk_101c[0].mUnk_0c[2] = data_020a22ac[id].mY;
    mUnk_101c[0].mUnk_12[0] = mUnk_101c[0].mUnk_0c[0];
    mUnk_101c[0].mUnk_12[1] = mUnk_101c[0].mUnk_0c[1];
    mUnk_101c[0].mUnk_12[2] = mUnk_101c[0].mUnk_0c[2];
    mUnk_101c[0].mUnk_32 = 0;
    return TRUE;
}


BOOL UnkStruct_02065c40::func_02068c54(u32 index, s32 value) {
    if (value == mUnk_1084[index].mUnk_48) {
        return FALSE;
    }
    if (value != -2) {
        mUnk_1084[index].mUnk_48 = value;
    }
    if (index == 0) {
        UnkStruct_0205401c *model = mUnk_11b4[1].mUnk_2c;
        if (model != NULL) {
            model->func_02054034();
            delete model;
        }
        mUnk_11b4[1].mUnk_2c = NULL;
    } else {
        UnkStruct_0203d374 *object = mUnk_112c[1].mUnk_2c;
        if (object != NULL) {
            delete object;
        }
        mUnk_112c[1].mUnk_2c = NULL;
    }
    func_020695e8(index);
    if (mUnk_1084[index].mUnk_48 < 0) {
        return FALSE;
    }
    return TRUE;
}

void UnkStruct_02065c40::func_02068d0c(UnkStruct_02028308 *texture, u32 id) {
    if ((id == 0x1d) | (id == 0x1c)) {
        char name[0x40];
        char file[0x40];
        ChangeDir("/Game/Realbaku/Tex");
        u8 i;
        const char *prefix = data_020a2554[id].mName;
        u8 variant         = data_020d43f0[0].mUnk_1a_8;
        for (i = 0; i < 11; i++) {
            sprintf(name, "%s_%02d", prefix, i);
            sprintf(file, "%s_%02d_%02d", prefix, i, variant);
            texture->func_02028858(name, file, 0);
        }
        ChangeDir("/");
    }
}

BOOL UnkStruct_02065c40::func_02068e14(u32 index) {
    char path[0x80];
    if (index == 0) {
        SpriteManager::Get().func_0202753c(1);
    } else {
        SpriteManager::Get().func_0202753c(6);
    }
    s32 value = mUnk_1084[index].mUnk_48;
    if (value > 0x29) {
        value = 0x29;
    } else if (value < 0) {
        value = 0;
    }
    u8 id;
    u8 dir;
    NitroTexture *texture;
    NitroModel *model;
    id  = value;
    dir = data_020a2554[id].mDir;
    ChangeDir(data_020b431c[dir]);
    sprintf(path, "%s.nsbtx", data_020a2554[id].mName);
    texture = new NitroTexture(path);
    func_02068d0c(texture, id);
    ChangeDir(data_020b431c[dir]);
    sprintf(path, "%s.nsbmd", data_020a2554[id].mName);
    model = new NitroModel;
    model->func_020289e0(path, texture);
    if (dir != 1) {
        texture->func_02028344();
    }
    if (data_020a2554[id].mData != 0) {
        sprintf(path, "%s_data.bin", data_020a2554[id].mName);
        ModelAnimData *animData = new ModelAnimData(path);
        ModelAnim *anim         = new ModelAnim(animData, model);
        mUnk_1084[index].mUnk_2c = model->func_02028b78(anim);
        mUnk_1084[index].mUnk_2c->func_02028e54(0, 1, 0x1000);
    } else {
        mUnk_1084[index].mUnk_2c = model->func_02028b78(NULL);
    }
    ChangeDir("/");
    MI_CpuClear16(&mUnk_1084[index], 0x18);
    MI_CpuClear16(mUnk_1084[index].mUnk_18, 0x14);
    mUnk_1084[index].mUnk_4d = 0;
    mUnk_1084[index].mUnk_4e = 0;
    mUnk_1084[index].mUnk_4c = data_020a2554[id].mUnk_05;
    mUnk_1084[index].mUnk_4f = 0x80;
    mUnk_1084[index].mUnk_50 = 0;
    return TRUE;
}

void UnkStruct_02065c40::func_02069120(u32 index, u32 value) {
    mUnk_1084[index].mUnk_4a = value;
    mUnk_1084[index].mUnk_52 = 0;
    for (u8 j = 0; j < 3; j++) {
        if (mUnk_1084[index].mUnk_3c[j] != NULL) {
            OS_Free(mUnk_1084[index].mUnk_3c[j]);
            mUnk_1084[index].mUnk_3c[j] = NULL;
        }
    }
    mUnk_12b8 = 0;
}

BOOL UnkStruct_02065c40::func_02069194(u32 index) {
    char path[0x80];
    u8 id = mUnk_1084[index].mUnk_4a;
    if (mUnk_12b8 != 0) {
        if (func_02023874() != 0) {
            return FALSE;
        }
        mUnk_1084[index].mUnk_3c[mUnk_12b8 - 1] = func_02023864();
        if (mUnk_12b8 == 2 && data_020a2554[id].mData == 0) {
            mUnk_12b8 = 3;
        }
    }
    if (mUnk_12b8 < 3) {
        ChangeDir("/");
        switch (mUnk_12b8) {
            case 0:
                sprintf(path, "%s%s.nsbtx", data_020b431c[data_020a2554[id].mDir], data_020a2554[id].mName);
                break;
            case 1:
                sprintf(path, "%s%s.nsbmd", data_020b431c[data_020a2554[id].mDir], data_020a2554[id].mName);
                break;
            case 2:
                sprintf(path, "%s%s_data.bin", data_020b431c[data_020a2554[id].mDir], data_020a2554[id].mName);
                break;
        }
        func_02023708(path, 0, 0, 0, 0, 0);
        mUnk_12b8++;
        return FALSE;
    }
    return TRUE;
}

void UnkStruct_02065c40::func_02069340(u32 index) {
    func_020695e8(index);
    if (index == 0) {
        SpriteManager::Get().func_0202753c(1);
    } else {
        SpriteManager::Get().func_0202753c(6);
    }
    u8 id = mUnk_1084[index].mUnk_4a;
    UnkStruct_02028308 *texture = new NitroTextureData(mUnk_1084[index].mUnk_3c[0]);
    func_02068d0c(texture, id);
    NitroModel *model = new NitroModel;
    model->func_02028a2c(mUnk_1084[index].mUnk_3c[1], texture);
    if (data_020a2554[id].mDir != 1) {
        texture->func_02028344();
    }
    if (data_020a2554[id].mData != 0) {
        ModelAnimData *animData = new ModelAnimData(mUnk_1084[index].mUnk_3c[2]);
        ModelAnim *anim         = new ModelAnim(animData, model);
        mUnk_1084[index].mUnk_2c = model->func_02028b78(anim);
        mUnk_1084[index].mUnk_2c->func_02028e54(0, 1, 0x1000);
    } else {
        mUnk_1084[index].mUnk_2c = model->func_02028b78(NULL);
    }
    for (u8 j = 0; j < 3; j++) {
        mUnk_1084[index].mUnk_30[j] = mUnk_1084[index].mUnk_3c[j];
        mUnk_1084[index].mUnk_3c[j] = NULL;
    }
    mUnk_1084[index].mUnk_2c->func_02028e54(mUnk_1084[index].mUnk_52, 1, 0x1000);
    mUnk_1084[index].mUnk_48 = mUnk_1084[index].mUnk_4a;
    mUnk_1084[index].mUnk_4a = -1;
    mUnk_1084[index].mUnk_4d = 0;
    mUnk_1084[index].mUnk_4c = data_020a2554[id].mUnk_05;
}

void UnkStruct_02065c40::func_020695e8(u32 index) {
    ModelInstance *instance = mUnk_1084[index].mUnk_2c;
    if (instance != NULL) {
        instance->mUnk_68 = 0xf;
        delete instance;
        mUnk_1084[index].mUnk_2c = NULL;
    }
    for (u8 j = 0; j < 3; j++) {
        if (mUnk_1084[index].mUnk_30[j] != NULL) {
            OS_Free(mUnk_1084[index].mUnk_30[j]);
            mUnk_1084[index].mUnk_30[j] = NULL;
        }
    }
}

BOOL UnkStruct_02065c40::func_02069680(u32 index) {
    if (index == 1) {
        func_020695e8(1);
    }
    UnkStruct_0203d374 *object = mUnk_112c[index].mUnk_2c;
    if (object != NULL) {
        delete object;
    }
    mUnk_112c[index].mUnk_2c = NULL;
    if (mUnk_112c[index].mUnk_34 < 0) {
        return FALSE;
    }
    return TRUE;
}

void UnkStruct_02065c40::func_020696e8(u32 index) {
    UnkStruct_0203d374 *object = mUnk_112c[index].mUnk_30;
    if (object != NULL) {
        delete object;
    }
    mUnk_112c[index].mUnk_30 = NULL;
    s32 kind = mUnk_112c[index].mUnk_35;
    if (kind < 0) {
        return;
    }
    mUnk_112c[index].mUnk_30 = new UnkStruct_0203d374();
    u8 value = func_02069780(kind);
    mUnk_112c[index].mUnk_30->func_0203d6e0(data_020a1bd6[kind].mUnk_0, value, 5);
}

u8 UnkStruct_02065c40::func_02069780(u32 index) {
    switch (data_020a1bd6[index].mUnk_0) {
        case 0x24:
        case 0x25:
            return data_020d43f0[0].mUnk_1a_8;
    }
    return data_020a1bd6[index].mUnk_1;
}

extern "C" u16 FX_Atan2Idx(fx32 x, fx32 y);

static inline fx32 SinIdx(int idx) {
    return FX_SinCosTable_[(idx >> 4) * 2];
}
static inline fx32 CosIdx(int idx) {
    return FX_SinCosTable_[(idx >> 4) * 2 + 1];
}

// The mode passed to func_0203d4dc is an enum local: as `u32 mode = c ? 5 : 1;` it colours differently (r6/r7 swapped).
enum UnkStruct_02065c40_ModelMode { UNK_02065C40_MODEL_MODE_1 = 1, UNK_02065C40_MODEL_MODE_5 = 5 };

BOOL UnkStruct_02065c40::func_020697c8(u32 index, u32 flag) {
    if (index == 0) {
        SpriteManager::Get().func_0202753c(4);
    } else {
        SpriteManager::Get().func_0202753c(5);
    }
    if (flag) {
        if (!mUnk_112c[index].mUnk_30->func_0203d6f0()) {
            return FALSE;
        }
    } else {
        s32 kind = mUnk_112c[index].mUnk_34;
        UnkStruct_02065c40_ModelMode mode;
        if (mUnk_112c[index].mUnk_36 != 0) {
            mode = UNK_02065C40_MODEL_MODE_5;
        } else {
            mode = UNK_02065C40_MODEL_MODE_1;
        }
        ChangeDir("/");
        mUnk_112c[index].mUnk_2c = new UnkStruct_0203d374();
        u8 value = func_02069780(kind);
        mUnk_112c[index].mUnk_2c->func_0203d4dc(data_020a1bd6[kind].mUnk_0, value, (u8) mode);
        mUnk_112c[index].mUnk_37 = 0;
        MI_CpuClear16(&mUnk_112c[index], 0x18);
        MI_CpuClear16(mUnk_112c[index].mUnk_18, 0x14);
        func_020699a4(index);
    }
    return TRUE;
}

void UnkStruct_02065c40::func_020699a4(u32 index) {
    mUnk_112c[index].mUnk_38 = 0x80;
    mUnk_112c[index].mUnk_39 = 0x80;
    mUnk_112c[index].mUnk_3a = 0x80;
    mUnk_112c[index].mUnk_3b = 0;
    mUnk_112c[index].mUnk_3c = 0;
    mUnk_112c[index].mUnk_3d = 0;
    mUnk_112c[index].mUnk_3e = 0;
    mUnk_112c[index].mUnk_40 = 0;
    mUnk_112c[index].mUnk_3f = 10;
    if (mUnk_112c[index].mUnk_36 == 0) {
        return;
    }
    mUnk_112c[index].mUnk_2c->mUnk_49 = 1;
    mUnk_112c[index].mUnk_2c->mModel->func_02028e54(0, 0, 0x64000);
}

void UnkStruct_02065c40::func_02069a24(u32 index) {
    if (mUnk_112c[index].mUnk_30 == NULL) {
        return;
    }
    UnkStruct_0203d374 *object = mUnk_112c[index].mUnk_2c;
    if (object != NULL) {
        delete object;
    }
    mUnk_112c[index].mUnk_2c = mUnk_112c[index].mUnk_30;
    mUnk_112c[index].mUnk_30 = NULL;
    mUnk_112c[index].mUnk_34 = mUnk_112c[index].mUnk_35;
    mUnk_112c[index].mUnk_36 = 1;
    if (index == 0) {
        SpriteManager::Get().func_0202753c(4);
    } else {
        SpriteManager::Get().func_0202753c(5);
    }
    mUnk_112c[index].mUnk_2c->func_0203d6f8();
    func_020699a4(index);
}

void UnkStruct_02065c40::func_02069b80(u32 index) {
    if (mUnk_112c[index].mUnk_2c == NULL) {
        return;
    }
    u16 rot[3];
    rot[0] = mUnk_112c[index].mUnk_18[0];
    rot[1] = mUnk_112c[index].mUnk_18[1];
    rot[2] = mUnk_112c[index].mUnk_18[2];
    mUnk_112c[index].mUnk_2c->func_0203db94(rot);
}

void UnkStruct_02065c40::func_02069bdc() {
    for (u8 i = 0; i < 2; i++) {
        MI_CpuClear16(&mUnk_1244[i], 0x18);
        MI_CpuClear16(&mUnk_1274, 0x14);
    }
    mUnk_1244[1].mUnk_08 = -0x64000;
    mUnk_128c[0] = 0x3c;
    mUnk_128c[1] = 0x3c;
    mUnk_128c[2] = 0x3c;
    mUnk_1292    = 1;
    func_02069d0c();
}

extern "C" void func_01ffcf64(MtxFx43 *mtx, VecFx32 *pos);

void UnkStruct_02065c40::func_02069c6c() {
    MI_CpuClear16(&mUnk_1298, 0x14);
    u32 v = 0xfffff23d;
    mUnk_1298.x = -0x452;
    mUnk_1298.y = -0x6e1;
    mUnk_1298.z = v;
    mUnk_129e.x = mUnk_1298.x;
    mUnk_129e.y = mUnk_1298.y;
    mUnk_129e.z = mUnk_1298.z;
    mUnk_12a4.x = mUnk_1298.x;
    mUnk_12a4.y = mUnk_1298.y;
    mUnk_12a4.z = mUnk_1298.z;
    mUnk_12ac = v >> 17;
    if (mUnk_1014 == NULL) {
        return;
    }
    mUnk_1014->mLights[0]->mColor = mUnk_12ac;
    mUnk_1014->func_02029d54();
}

void UnkStruct_02065c40::func_02069d0c() {
    u32 fov = mUnk_128c[0];
    mUnk_1288->func_02026004((u32) (65536.0f * fov / 360.0f));
}

void UnkStruct_02065c40::func_02069d54() {
    if (mUnk_1292 == 0) {
        mUnk_1244[1].mUnk_00 = mUnk_1244[0].mUnk_00 + SinIdx(mUnk_1274.mUnk_02) * 100;
        mUnk_1244[1].mUnk_08 = mUnk_1244[0].mUnk_08 - CosIdx(mUnk_1274.mUnk_02) * 100;
        mUnk_1244[1].mUnk_04 = mUnk_1244[0].mUnk_04 + SinIdx(mUnk_1274.mUnk_00) * 100;
    }
    fx32 z = mUnk_1244[0].mUnk_08;
    fx32 y = mUnk_1244[0].mUnk_04;
    fx32 x = mUnk_1244[0].mUnk_00;
    if ((z == mUnk_1244[1].mUnk_08) & ((x == mUnk_1244[1].mUnk_00) & (y == mUnk_1244[1].mUnk_04))) {
        return;
    }
    mUnk_1288->SetPosition(x, y, z);
    mUnk_1288->SetTarget(mUnk_1244[1].mUnk_00, mUnk_1244[1].mUnk_04, mUnk_1244[1].mUnk_08);
    if (mUnk_1014 == NULL) {
        return;
    }
    s16 dir[3];
    s16 dx = mUnk_1298.x;
    s16 dz = mUnk_1298.z;
    s16 dy = mUnk_1298.y;
    dir[0] = dx;
    dir[1] = dy;
    dir[2] = dz;
    mUnk_1014->mLights[0]->func_02029c44(dir);
    mUnk_1014->func_02029d54();
}

void UnkStruct_02065c40::func_02069eb8(u32 mode) {
    if (mUnk_1292 == mode) {
        return;
    }
    if (mode != 0) {
        mUnk_1244[1].mUnk_00 = mUnk_1244[0].mUnk_00 + SinIdx(mUnk_1274.mUnk_02) * 100;
        mUnk_1244[1].mUnk_08 = mUnk_1244[0].mUnk_08 - CosIdx(mUnk_1274.mUnk_02) * 100;
        mUnk_1244[1].mUnk_0c = mUnk_1244[1].mUnk_00 >> 12;
        mUnk_1244[1].mUnk_10 = mUnk_1244[1].mUnk_08 >> 12;
        mUnk_1244[1].mUnk_12 = mUnk_1244[1].mUnk_0c;
        mUnk_1244[1].mUnk_16 = mUnk_1244[1].mUnk_10;
        mUnk_1244[1].mUnk_04 = mUnk_1244[0].mUnk_04 + SinIdx(mUnk_1274.mUnk_00) * 100;
    } else {
        mUnk_1274.mUnk_02 = FX_Atan2Idx(mUnk_1244[1].mUnk_00 - mUnk_1244[0].mUnk_00, -(mUnk_1244[1].mUnk_08 - mUnk_1244[0].mUnk_08));
        VecFx32 delta = {0, 0, 0};
        delta.x = mUnk_1244[1].mUnk_00 - mUnk_1244[0].mUnk_00;
        delta.z = mUnk_1244[1].mUnk_08 - mUnk_1244[0].mUnk_08;
        mUnk_1274.mUnk_00 = FX_Atan2Idx(mUnk_1244[1].mUnk_04 - mUnk_1244[0].mUnk_04, VEC_Mag(&delta));
        mUnk_1274.mUnk_08 = mUnk_1274.mUnk_02s;
    }
    mUnk_1292 = mode;
}

void UnkStruct_02065c40::func_0206a01c() {
    if ((mUnk_14e4 == 0) & (mUnk_14e8 | mUnk_14f1) && mUnk_100c != NULL) {
        mUnk_100c->Update();
        mUnk_100c->Draw(1, 0x80, 0x60, 0);
    }
    if (mUnk_14e4 != 0 && mUnk_14e5 != 0) {
        return;
    }
    u32 first  = 0;
    u32 second = 0;
    s32 current = mUnk_1018->mUnk_42;
    if (current >= 0) {
        if (current == 0x1a) {
            second = 0x80;
        } else {
            for (u8 i = 0; i < 2; i++) {
                if (mUnk_11b4[i].mUnk_2c != NULL && current == mUnk_11b4[i].mUnk_36) {
                    second = mUnk_11b4[i].mUnk_3f;
                    first  = mUnk_11b4[i].mUnk_42;
                    break;
                }
                if (mUnk_112c[i].mUnk_2c != NULL) {
                    s32 kind = mUnk_112c[i].mUnk_34;
                    if (kind >= 0 && current == data_020a1bd6[kind].mUnk_2) {
                        second = mUnk_112c[i].mUnk_38;
                        first  = mUnk_112c[i].mUnk_3b;
                        break;
                    }
                }
                if (mUnk_1084[i].mUnk_2c != NULL) {
                    BOOL match = FALSE;
                    if (current == 0x16) {
                        s32 v = mUnk_1084[i].mUnk_48;
                        switch (v) {
                            case 0x1c:
                            case 0x1d:
                                match = TRUE;
                        }
                    } else if (current == 0x17) {
                        s32 v = mUnk_1084[i].mUnk_48;
                        switch (v) {
                            case 0x1e:
                            case 0x1f:
                                match = TRUE;
                        }
                    }
                    if (match) {
                        second = mUnk_1084[i].mUnk_4f;
                        first  = mUnk_1084[i].mUnk_50;
                        break;
                    }
                }
            }
        }
    }
    mUnk_1018->func_02088a18(first, second);
}

void UnkStruct_02065c40::func_0206a1c8(u32 value, u32 force) {
    if (force == 0 && mUnk_14f2 == value) {
        return;
    }
    if (mUnk_100c == NULL) {
        return;
    }
    mUnk_100c->SetAnimation(mUnk_14f2 = value);
}

void UnkStruct_02065c40::func_0206a208() {
    if ((mUnk_14ed == 0) & ((mUnk_12ba == 0) & (mUnk_14ec == 0))) {
        u8 step = mUnk_12b9;
        if (((step >= 3) & (step <= 4)) && mUnk_12c5 != 6) {
            if (InputManager::Get().IsHeld(8)) {
                func_02070390();
                return;
            }
        }
    }
    switch (mUnk_12ca) {
        case 0:
            func_0206a1c8(0, 0);
            if (mUnk_12cc >= mUnk_12ce) {
                return;
            }
            u32 flag = 0;
            if (mUnk_14e2 == 5) {
                flag = 1;
            }
            mUnk_1018->func_02088e64(mUnk_12d2[mUnk_12cc], mUnk_13d2[mUnk_12cc], flag, mUnk_14ee);
            mUnk_12ca = 1;
            break;
        case 1:
            if (!mUnk_1018->func_02088f14()) {
                return;
            }
            if (mUnk_12ba == 0) {
                mUnk_12ca = 3;
            } else if (mUnk_12ba == 3) {
                mUnk_1018->mUnk_4a = 1;
                u8 r      = mUnk_1018->func_02088efc();
                mUnk_14f1 = r;
                if (r) {
                    mUnk_1018->func_02088ebc(1);
                    mUnk_12ca = 4;
                } else {
                    mUnk_1018->func_02088ebc(0);
                    mUnk_12ca = 6;
                    mUnk_12b9 = 9;
                }
            } else {
                mUnk_12ca = 5;
                mUnk_12b9 = 9;
            }
            func_0206a1c8(2, 0);
            break;
        case 2:
            if (mUnk_12ba == 3) {
                if (mUnk_1018->func_02088efc()) {
                    func_0206a1c8(2, 0);
                    mUnk_14f1 = 1;
                    mUnk_12ca = 4;
                } else {
                    mUnk_12ca = 6;
                    mUnk_14f1 = 0;
                    mUnk_12b9 = 9;
                }
            } else {
                mUnk_12ca = 5;
            }
            break;
        case 5:
            if (!mUnk_1018->func_02088efc()) {
                return;
            }
            mUnk_12ca = 7;
            mUnk_12d0 = 200;
            mUnk_1018->func_02088ebc(0);
            break;
        case 7:
            if (mUnk_12d0 != 0) {
                mUnk_12d0--;
                return;
            }
            if (!mUnk_1018->func_02088dac(1)) {
                mUnk_1018->mUnk_34 = -1;
                mUnk_1018->func_02088e64(mUnk_12d2[0], mUnk_13d2[0], 0, 0);
                mUnk_12d0 = 200;
            } else {
                if (mUnk_1018->func_02088efc()) {
                    mUnk_12d0 = 200;
                } else {
                    mUnk_12d0 = 300;
                }
            }
            break;
        case 3:
        case 4: {
            if (mUnk_14ee == 2) {
                return;
            }
            BOOL touched = FALSE;
            if (TouchManager::Get().mUnk_57 != 0) {
                mUnk_14ef = 1;
            }
            if (mUnk_14ef != 0) {
                if (TouchManager::Get().mUnk_56 == 0) {
                    touched = TRUE;
                } else {
                    func_0206a1c8(1, 1);
                }
            }
            if (!InputManager::Get().IsHeld(1) && !touched) {
                return;
            }
            mUnk_14ef = 0;
            if (mUnk_14e8 | mUnk_14f1) {
                func_0206a1c8(InputManager::Get().IsHeld(1) ? 3 : 1, 0);
            }
            mUnk_12cb = 4;
            if (mUnk_12ca == 4) {
                mUnk_12ca = 9;
            } else {
                mUnk_12ca = 8;
            }
            if (mUnk_14ee != 0) {
                return;
            }
            SoundManager::Get().func_020246f8(0x6b);
            break;
        }
        case 8:
        case 9:
            if (mUnk_12cb != 0) {
                mUnk_12cb--;
                return;
            }
            if (!mUnk_1018->func_02088dac(0)) {
                if (mUnk_12ca == 9) {
                    mUnk_1018->func_02088ebc(0);
                    mUnk_14f1 = 0;
                    mUnk_12ca = 6;
                    mUnk_12b9 = 9;
                } else {
                    mUnk_12cc++;
                    mUnk_12ca = 0;
                }
                func_0206a1c8(0, 0);
            } else {
                if (mUnk_12ca == 9) {
                    mUnk_12ca = 4;
                } else {
                    mUnk_12ca = 3;
                }
                func_0206a1c8(2, 0);
            }
            break;
    }
}

extern const s16 data_020a248c[][9];
extern const s16 data_020a233c[][12];
void func_02025588(void (*callback)());
void func_020255f8(void (*callback)());
extern "C" void func_0206ea14();
void func_02087ccc(UnkStruct_02087898 *object, void *data);
void func_02088ec4(UnkStruct_02087898 *object);

void UnkStruct_02065c40::func_0206a850() {
    for (u8 i = 0; i < 2; i++) {
        if (mUnk_112c[i].mUnk_2c != NULL && mUnk_112c[i].mUnk_37 != 0) {
            VecFx32 pos;
            fx32 z = mUnk_112c[i].mUnk_08;
            fx32 y = mUnk_112c[i].mUnk_04;
            fx32 x = mUnk_112c[i].mUnk_00;
            pos.z = z / 2 - 0x72000;
            pos.y = y / 2;
            pos.x = x / 2;
            if (mUnk_112c[i].mUnk_3c != 0) {
                if (mUnk_112c[i].mUnk_3e < 1) {
                    mUnk_112c[i].mUnk_3e++;
                }
            } else {
                if (mUnk_112c[i].mUnk_3e != 0) {
                    mUnk_112c[i].mUnk_3e--;
                }
            }
            if (mUnk_112c[i].mUnk_3e != 0) {
                mUnk_112c[i].mUnk_3d = (mUnk_112c[i].mUnk_3d + 1) % 0x80;
                pos.y += mUnk_112c[i].mUnk_3e * SinIdx((u16) (mUnk_112c[i].mUnk_3d * 0x10000 / 0x80));
            }
            mUnk_112c[i].mUnk_2c->func_0203db70(&pos);
            func_02069b80(i);
            mUnk_112c[i].mUnk_2c->func_0203d988();
            mUnk_112c[i].mUnk_2c->func_0203dab0();
            u8 shadow = mUnk_112c[i].mUnk_3f;
            if (shadow != 0 && mUnk_1010 != NULL) {
                s32 scale = shadow * 0x19a;
                if (scale > 0) {
                    pos.y = (mUnk_112c[i].mUnk_40 << 12) - 0x64000;
                    MtxFx43 mtx = data_0220e86c;
                    func_01ffcf64(&mtx, &pos);
                    VecFx32 size;
                    size.x = scale;
                    size.y = scale;
                    size.z = scale;
                    mUnk_1010->func_02029174(&mtx, &size);
                }
            }
        }
    }
}

void UnkStruct_02065c40::func_0206aa34() {
    for (u8 i = 0; i < 2; i++) {
        if (mUnk_11b4[i].mUnk_2c != NULL) {
            s32 z = mUnk_11b4[i].mUnk_08 / 2 - 0x80000;
            s32 h = mUnk_11b4[i].mUnk_34 / 2;
            VecFx32 pos;
            s32 round = FX_FROM_INT_ROUND(h);
            s32 y = mUnk_11b4[i].mUnk_04;
            s32 x = mUnk_11b4[i].mUnk_00;
            y = y / 2 - 0x64000;
            pos.y = y + round;
            pos.x = x / 2;
            pos.z = z;
            if (mUnk_11b4[i].mUnk_36 == 0x15) {
                ModelTransform *transform = (ModelTransform *) mUnk_11b4[i].mUnk_2c->mUnk_04;
                transform->mPosition.x    = pos.x;
                transform->mPosition.y    = pos.y;
                transform->mPosition.z    = pos.z;
            } else {
                ModelTransform *transform = (ModelTransform *) mUnk_11b4[i].mUnk_2c->mUnk_08;
                transform->mPosition.x    = pos.x;
                transform->mPosition.y    = pos.y;
                transform->mPosition.z    = pos.z;
            }
            if (mUnk_11b4[i].mUnk_3e != 0) {
                func_02068884(i);
                mUnk_11b4[i].mUnk_2c->func_02054568();
                func_0206abf4(i);
                u8 shadow = mUnk_11b4[i].mUnk_3d;
                if (shadow != 0 && mUnk_1010 != NULL) {
                    MtxFx43 mtx = data_0220e86c;
                    s32 scale   = shadow * 0x19a;
                    VecFx32 size;
                    size.x = scale;
                    size.y = scale;
                    size.z = scale;
                    pos.y += 0x1000;
                    func_01ffcf64(&mtx, &pos);
                    mUnk_1010->func_02029174(&mtx, &size);
                }
            }
        }
    }
}

void UnkStruct_02065c40::func_0206abf4(u32 index) {
    if (mUnk_11b4[index].mUnk_3b == 0) {
        return;
    }
    s32 kind = mUnk_11b4[index].mUnk_36;
    ModelAnim *anim;
    if (kind == 0x15) {
        anim = ((CharacterModel *) mUnk_11b4[index].mUnk_2c->mUnk_04)->mParts[0]->mAnim;
    } else {
        anim = ((ModelTransform *) mUnk_11b4[index].mUnk_2c->mUnk_08)->mModel->mAnim;
    }
    if (anim->mUnk_10 == 0) {
        return;
    }
    u8 cur = mUnk_11b4[index].mUnk_39;
    u8 target = mUnk_11b4[index].mUnk_3a;
    s32 value;
    BOOL loop;
    if (cur != target) {
        loop = FALSE;
        if (cur == 0) {
            value = func_020687a8((u8) kind, target, 1);
            mUnk_11b4[index].mUnk_39 = target;
        } else {
            value = func_020687a8((u8) kind, cur, 2);
            mUnk_11b4[index].mUnk_39 = 0;
        }
        if (value < 0) {
            value = func_020687a8((u8) mUnk_11b4[index].mUnk_36, target, 0);
            mUnk_11b4[index].mUnk_39 = target;
            mUnk_11b4[index].mUnk_3b = 0;
            if (func_020687a8((u8) mUnk_11b4[index].mUnk_36, target, 4) != 0) {
                loop = FALSE;
            } else {
                loop = TRUE;
            }
        }
    } else {
        value = func_020687a8((u8) kind, cur, 0);
        mUnk_11b4[index].mUnk_3b = 0;
        loop = TRUE;
    }
    s8 other = func_020687a8((u8) mUnk_11b4[index].mUnk_36, target, 3);
    mUnk_11b4[index].mUnk_2c->func_02054644((u8) other, (u8) value);
    mUnk_11b4[index].mUnk_3c = 0;
    anim->mUnk_08 = loop;
}

void UnkStruct_02065c40::func_0206ada4() {
    if (mUnk_101c[0].mUnk_2c == NULL) {
        return;
    }
    MtxFx43 mtx = data_0220e86c;
    VecFx32 size;
    size.x = 0x1000;
    size.y = 0x1000;
    size.z = 0x1000;
    VecFx32 pos;
    fx32 x = mUnk_101c[0].mUnk_00;
    fx32 z = mUnk_101c[0].mUnk_08;
    fx32 y = mUnk_101c[0].mUnk_04;
    pos.x = x;
    pos.y = y;
    pos.z = z;
    if (mUnk_101c[0].mUnk_18[0] != 0) {
        func_01ffd088(&mtx, (u16) mUnk_101c[0].mUnk_18[0]);
    }
    if (mUnk_101c[0].mUnk_18[1] != 0) {
        func_01ffd0e0(&mtx, (u16) mUnk_101c[0].mUnk_18[1]);
    }
    if (mUnk_101c[0].mUnk_18[2] != 0) {
        func_01ffd138(&mtx, (u16) mUnk_101c[0].mUnk_18[2]);
    }
    func_01ffcf64(&mtx, &pos);
    if (mUnk_101c[0].mUnk_32 != 0) {
        mUnk_101c[0].mUnk_2c->mUnk_60->mFrame = 0;
    } else {
        mUnk_101c[0].mUnk_2c->func_02028d80();
    }
    mUnk_101c[0].mUnk_2c->func_02028da4();
    mUnk_101c[0].mUnk_2c->func_02029174(&mtx, &size);
}

void UnkStruct_02065c40::func_0206aecc() {
    for (u8 i = 0; i < 2; i++) {
        if (mUnk_1084[i].mUnk_2c != NULL && mUnk_1084[i].mUnk_4e != 0) {
            MtxFx43 mtx = data_0220e86c;
            s32 scale   = FX_FROM_INT_ROUND(mUnk_1084[i].mUnk_4c);
            VecFx32 size;
            size.x = scale;
            size.y = scale;
            size.z = scale;
            VecFx32 pos;
            pos.x = mUnk_1084[i].mUnk_00;
            pos.y = mUnk_1084[i].mUnk_04;
            pos.z = mUnk_1084[i].mUnk_08;
            func_01ffcf64(&mtx, &pos);
            if (mUnk_1084[i].mUnk_18[0] != 0) {
                func_01ffd088(&mtx, (u16) mUnk_1084[i].mUnk_18[0]);
            }
            if (mUnk_1084[i].mUnk_18[1] != 0) {
                func_01ffd0e0(&mtx, (u16) mUnk_1084[i].mUnk_18[1]);
            }
            if (mUnk_1084[i].mUnk_18[2] != 0) {
                func_01ffd138(&mtx, (u16) mUnk_1084[i].mUnk_18[2]);
            }
            mUnk_1084[i].mUnk_2c->func_02028d80();
            mUnk_1084[i].mUnk_2c->func_02029174(&mtx, &size);
            if (mUnk_1084[i].mUnk_4d != 0) {
                if (mUnk_1084[i].mUnk_2c->func_02028f48() != 0) {
                    mUnk_1084[i].mUnk_4e = 0;
                }
            }
        }
    }
}

struct UnkStruct_020a2198 {
    /* 00 */ s16 mUnk_00;
    /* 02 */ s16 mUnk_02;
    /* 04 */ s16 mUnk_04;
    /* 06 */ s16 mUnk_06;
    /* 08 */ s16 mUnk_08;
    /* 0a */ s16 mUnk_0a;
    /* 0c */ s16 mUnk_0c;
    /* 0e */ s16 mUnk_0e;
    /* 10 */ s16 mUnk_10;
    /* 12 */ s16 mUnk_12;
    /* 14 */ u8 mUnk_14;
    /* 15 */ PAD(0x15, 0x16);
    /* 16 */
};
struct UnkStruct_020a1c04 {
    /* 0 */ s16 mUnk_0;
    /* 2 */ s16 mUnk_2;
    /* 4 */ s16 mUnk_4;
    /* 6 */ u16 mUnk_6;
    /* 8 */
};
extern const UnkStruct_020a2198 data_020a2198[];
extern const UnkStruct_020a1c04 data_020a1c04[];

UnkStruct_02065c40_Kind UnkStruct_02065c40::func_0206b05c(u32 id) {
    if (id < 0x60) {
        return UNK_02065C40_KIND_0;
    }
    if (id < 0x77) {
        return UNK_02065C40_KIND_1;
    }
    if (id < 0x90) {
        return UNK_02065C40_KIND_0;
    }
    if (id < 0xa9) {
        return UNK_02065C40_KIND_1;
    }
    if (id < 0xbf) {
        return UNK_02065C40_KIND_0;
    }
    return UNK_02065C40_KIND_1;
}



void UnkStruct_02065c40::func_0206b09c() {
    mUnk_12b9 = 3;
    mUnk_12c5 = 0;
    while (TRUE) {
        if (mUnk_12bc >= mUnk_12be) {
            if (mUnk_12cc != mUnk_12ce) {
                return;
            }
            func_02066814(1);
            mUnk_12b9 = 5;
            func_02067708(0);
            return;
        }
        func_0206b150(mUnk_0c[mUnk_12bc].mCommand, mUnk_0c[mUnk_12bc].mValue);
        if (mUnk_12c5 == 0) {
            mUnk_12bc++;
        } else {
            mUnk_12b9 = 4;
            return;
        }
    }
}

void func_02088f28(UnkStruct_02087898 *self, u8 unk1);

void UnkStruct_02065c40::func_0206b150(u32 command, s32 value) {
    mUnk_12c6 = command;
    mUnk_12c8 = value;
    s16 angle = (mUnk_12c8 << 16) / 360;
    u32 idx   = func_0206b05c(mUnk_12c6);
    mUnk_12c5 = 0;
    switch (mUnk_12c6) {
        case 1:
            mUnk_12d2[mUnk_12ce] = mUnk_12c8;
            mUnk_12ce++;
            if (mUnk_12ce >= 0x80) {
                mUnk_12ce = 0;
            }
            break;
        case 3:
            func_02088ec4(mUnk_1018);
            mUnk_12ca = 0;
            mUnk_12cc = mUnk_12ce;
            mUnk_12c4 = 0;
            mUnk_12c5 = 1;
            break;
        case 8:
            mUnk_13d2[mUnk_12ce] = mUnk_12c8;
            break;
        case 2:
            mUnk_12c5 = 2;
            break;
        case 9:
            {
                s32 limit = mUnk_12c8;
                if (limit > 3) {
                    limit = 3;
                }
                mUnk_12ba = limit;
                break;
            }
        case 10:
            mUnk_14ee = mUnk_12c8;
            break;
        case 11:
            mUnk_1018->mUnk_3a = mUnk_12c8;
            break;
        case 12:
            mUnk_1018->mUnk_3b = mUnk_12c8;
            break;
        case 13:
            mUnk_12c4 = mUnk_12c8;
            mUnk_12c5 = 1;
            break;
        case 14:
            {
                s32 v = mUnk_12c8;
                if (v < 1) {
                    v = 1;
                }
                mUnk_14de = v;
                break;
            }
        case 15:
            {
                s32 v = mUnk_12c8;
                if (v > 0x10) {
                    v = 0x10;
                } else if (v < -0x10) {
                    v = -0x10;
                }
                mUnk_14d6 = v;
                mUnk_14d7 = mUnk_14d5;
                break;
            }
        case 25:
        case 26:
        case 27:
            mUnk_14d7 = mUnk_14d5;
            mUnk_14d8 = 0;
            mUnk_14d9 = mUnk_14de;
            mUnk_14d4 = 0;
            switch (mUnk_12c6) {
                case 0x1a:
                    mUnk_14d3 = 1;
                    if (mUnk_14d6 < 0) {
                        SpriteManager::Get().StartSubFade(3);
                    } else {
                        SpriteManager::Get().StartSubFade(2);
                    }
                    mUnk_14d9 = 0x10;
                    break;
                case 0x1b:
                    mUnk_14d3 = 2;
                    if (mUnk_14d6 < 0) {
                        SpriteManager::Get().StartFade(3);
                    } else {
                        SpriteManager::Get().StartFade(2);
                    }
                    mUnk_14d9 = 0x10;
                    break;
                default:
                    mUnk_14d3 = 0;
                    break;
            }
            mUnk_12c5 = 3;
            break;
        case 17:
            func_0206e9d0(1);
        case 16:
            mUnk_14da = 0;
            mUnk_14dc = mUnk_14de;
            mUnk_12c5 = 4;
            break;
        case 18:
        case 19:
        case 20:
        case 21:
        case 22:
        case 23:
            switch (mUnk_12c6) {
                case 0x12:
                    func_02067e2c(0, 1);
                    break;
                case 0x13:
                    func_02067e2c(1, 1);
                    break;
                case 0x15:
                    func_02067e2c(0, 0);
                    break;
                case 0x16:
                    func_02067e2c(1, 0);
                    break;
                case 0x17:
                    break;
            }
            break;
        case 28:
            mUnk_1244[0].mUnk_00 = FX_FROM_INT_ROUND(mUnk_12c8);
            mUnk_1244[0].mUnk_0c = mUnk_12c8;
        case 31:
            mUnk_1244[0].mUnk_12 = mUnk_12c8;
            break;
        case 29:
            mUnk_1244[0].mUnk_04 = FX_FROM_INT_ROUND(mUnk_12c8);
            mUnk_1244[0].mUnk_0e = mUnk_12c8;
        case 32:
            mUnk_1244[0].mUnk_14 = mUnk_12c8;
            break;
        case 30:
            mUnk_1244[0].mUnk_08 = FX_FROM_INT_ROUND(mUnk_12c8);
            mUnk_1244[0].mUnk_10 = mUnk_12c8;
        case 33:
            mUnk_1244[0].mUnk_16 = mUnk_12c8;
            break;
        case 34:
            mUnk_1244[1].mUnk_00 = FX_FROM_INT_ROUND(mUnk_12c8);
            mUnk_1244[1].mUnk_0c = mUnk_12c8;
        case 37:
            mUnk_1244[1].mUnk_12 = mUnk_12c8;
            func_02069eb8(1);
            break;
        case 35:
            mUnk_1244[1].mUnk_04 = FX_FROM_INT_ROUND(mUnk_12c8);
            mUnk_1244[1].mUnk_0e = mUnk_12c8;
        case 38:
            mUnk_1244[1].mUnk_14 = mUnk_12c8;
            func_02069eb8(1);
            break;
        case 36:
            mUnk_1244[1].mUnk_08 = FX_FROM_INT_ROUND(mUnk_12c8);
            mUnk_1244[1].mUnk_10 = mUnk_12c8;
        case 39:
            mUnk_1244[1].mUnk_16 = mUnk_12c8;
            func_02069eb8(1);
            break;
        case 40:
            mUnk_1274.mVec[0].x = angle;
            mUnk_1274.mVec[1].x = angle;
        case 41:
            mUnk_1274.mVec[2].x = angle;
            func_02069eb8(0);
            break;
        case 42:
            mUnk_1274.mVec[0].y = angle;
            mUnk_1274.mVec[1].y = angle;
        case 43:
            mUnk_1274.mVec[2].y = angle;
            func_02069eb8(0);
            break;
        case 44:
            mUnk_1274.mVec[0].z = angle;
            mUnk_1274.mVec[1].z = angle;
        case 45:
            mUnk_1274.mVec[2].z = angle;
            func_02069eb8(0);
            break;
        case 46:
            mUnk_128c[0] = 0x3c;
            mUnk_128c[1] = 0x3c;
            mUnk_128c[2] = 0x3c;
            func_02069d0c();
            break;
        case 47:
            mUnk_128c[0] = mUnk_12c8;
            mUnk_128c[1] = mUnk_12c8;
            func_02069d0c();
        case 48:
            mUnk_128c[2] = mUnk_12c8;
            break;
        case 49:
            mUnk_1298.x = mUnk_12c8;
            mUnk_129e.x = mUnk_12c8;
        case 50:
            mUnk_12a4.x = mUnk_12c8;
            break;
        case 51:
            mUnk_1298.y = mUnk_12c8;
            mUnk_129e.y = mUnk_12c8;
        case 52:
            mUnk_12a4.y = mUnk_12c8;
            break;
        case 53:
            mUnk_1298.z = mUnk_12c8;
            mUnk_129e.z = mUnk_12c8;
        case 54:
            mUnk_12a4.z = mUnk_12c8;
            break;
        case 55:
            mUnk_12ac = (mUnk_12ac & 0xffe0) | (mUnk_12c8 & 0x1f);
            break;
        case 56:
            mUnk_12ac = (mUnk_12ac & 0xfc1f) | ((mUnk_12c8 & 0x1f) << 5);
            break;
        case 57:
            mUnk_12ac = (mUnk_12ac & 0x83ff) | ((mUnk_12c8 & 0x1f) << 10);
            break;
        case 58:
            mUnk_1014->mLights[0]->mColor = mUnk_12ac;
            mUnk_1014->func_02029d54();
            break;
        case 169:
        case 191:
            if (func_02068c54(idx, mUnk_12c8)) {
                mUnk_12c4 = 0;
                mUnk_12c5 = 6;
            }
            break;
        case 170:
        case 192:
            func_02069120(idx, mUnk_12c8);
            mUnk_12c4 = 0;
            mUnk_12c5 = 6;
            break;
        case 171:
        case 193:
            mUnk_1084[idx].mUnk_51 = 1;
            break;
        case 188:
        case 210:
            if (mUnk_1084[idx].mUnk_51 != 0) {
                mUnk_1084[idx].mUnk_52 = mUnk_12c8;
                break;
            }
            if (mUnk_1084[idx].mUnk_2c == NULL) {
                break;
            }
            mUnk_1084[idx].mUnk_2c->func_02028e54((u8) mUnk_12c8, 0, 0x1000);
            mUnk_1084[idx].mUnk_4d = 1;
            mUnk_1084[idx].mUnk_4e = 1;
            break;
        case 189:
        case 211:
            if (mUnk_1084[idx].mUnk_51 != 0) {
                mUnk_1084[idx].mUnk_52 = mUnk_12c8;
                break;
            }
            if (mUnk_1084[idx].mUnk_2c == NULL) {
                break;
            }
            mUnk_1084[idx].mUnk_2c->func_02028e54((u8) mUnk_12c8, 1, 0x1000);
            mUnk_1084[idx].mUnk_4d = 0;
            mUnk_1084[idx].mUnk_4e = 1;
            break;
        case 172:
        case 194:
            mUnk_1084[idx].mUnk_4e = 1;
            break;
        case 173:
        case 195:
            mUnk_1084[idx].mUnk_4e = 0;
            break;
        case 174:
            mUnk_1084[0].mUnk_00 = FX_FROM_INT_ROUND(mUnk_12c8);
            mUnk_1084[0].mUnk_0c[0] = mUnk_12c8;
        case 177:
            mUnk_1084[0].mUnk_12[0] = mUnk_12c8;
            break;
        case 175:
            mUnk_1084[0].mUnk_04 = FX_FROM_INT_ROUND(mUnk_12c8);
            mUnk_1084[0].mUnk_0c[1] = mUnk_12c8;
        case 178:
            mUnk_1084[0].mUnk_12[1] = mUnk_12c8;
            break;
        case 176:
            mUnk_1084[0].mUnk_08 = FX_FROM_INT_ROUND(mUnk_12c8);
            mUnk_1084[0].mUnk_0c[2] = mUnk_12c8;
        case 179:
            mUnk_1084[0].mUnk_12[2] = mUnk_12c8;
            break;
        case 180:
            mUnk_1084[0].mUnk_18[0] = angle;
            mUnk_1084[0].mUnk_1e[0] = angle;
        case 183:
            mUnk_1084[0].mUnk_24[0] = angle;
            break;
        case 181:
            mUnk_1084[0].mUnk_18[1] = angle;
            mUnk_1084[0].mUnk_1e[1] = angle;
        case 184:
            mUnk_1084[0].mUnk_24[1] = angle;
            break;
        case 182:
            mUnk_1084[0].mUnk_18[2] = angle;
            mUnk_1084[0].mUnk_1e[2] = angle;
        case 185:
            mUnk_1084[0].mUnk_24[2] = angle;
            break;
        case 196:
            mUnk_1084[1].mUnk_00 = FX_FROM_INT_ROUND(mUnk_12c8);
            mUnk_1084[1].mUnk_0c[0] = mUnk_12c8;
        case 199:
            mUnk_1084[1].mUnk_12[0] = mUnk_12c8;
            break;
        case 197:
            mUnk_1084[1].mUnk_04 = FX_FROM_INT_ROUND(mUnk_12c8);
            mUnk_1084[1].mUnk_0c[1] = mUnk_12c8;
        case 200:
            mUnk_1084[1].mUnk_12[1] = mUnk_12c8;
            break;
        case 198:
            mUnk_1084[1].mUnk_08 = FX_FROM_INT_ROUND(mUnk_12c8);
            mUnk_1084[1].mUnk_0c[2] = mUnk_12c8;
        case 201:
            mUnk_1084[1].mUnk_12[2] = mUnk_12c8;
            break;
        case 202:
            mUnk_1084[1].mUnk_18[0] = angle;
            mUnk_1084[1].mUnk_1e[0] = angle;
        case 205:
            mUnk_1084[1].mUnk_24[0] = angle;
            break;
        case 203:
            mUnk_1084[1].mUnk_18[1] = angle;
            mUnk_1084[1].mUnk_1e[1] = angle;
        case 206:
            mUnk_1084[1].mUnk_24[1] = angle;
            break;
        case 204:
            mUnk_1084[1].mUnk_18[2] = angle;
            mUnk_1084[1].mUnk_1e[2] = angle;
        case 207:
            mUnk_1084[1].mUnk_24[2] = angle;
            break;
        case 186:
        case 208:
            mUnk_1084[idx].mUnk_4f = mUnk_12c8;
            break;
        case 187:
        case 209:
            mUnk_1084[idx].mUnk_50 = mUnk_12c8;
            break;
        case 59:
            if (func_020688e0(mUnk_12c8)) {
                mUnk_12c4 = 0;
                mUnk_12c5 = 6;
            }
            break;
        case 61:
            mUnk_101c[0].mUnk_00 = FX_FROM_INT_ROUND(mUnk_12c8);
            mUnk_101c[0].mUnk_0c[0] = mUnk_12c8;
        case 64:
            mUnk_101c[0].mUnk_12[0] = mUnk_12c8;
            break;
        case 62:
            mUnk_101c[0].mUnk_04 = FX_FROM_INT_ROUND(mUnk_12c8);
            mUnk_101c[0].mUnk_0c[1] = mUnk_12c8;
        case 65:
            mUnk_101c[0].mUnk_12[1] = mUnk_12c8;
            break;
        case 63:
            mUnk_101c[0].mUnk_08 = FX_FROM_INT_ROUND(mUnk_12c8);
            mUnk_101c[0].mUnk_0c[2] = mUnk_12c8;
        case 66:
            mUnk_101c[0].mUnk_12[2] = mUnk_12c8;
            break;
        case 67:
            mUnk_101c[0].mUnk_18[0] = angle;
            mUnk_101c[0].mUnk_1e[0] = angle;
        case 70:
            mUnk_101c[0].mUnk_24[0] = angle;
            break;
        case 68:
            mUnk_101c[0].mUnk_18[1] = angle;
            mUnk_101c[0].mUnk_1e[1] = angle;
        case 71:
            mUnk_101c[0].mUnk_24[1] = angle;
            break;
        case 69:
            mUnk_101c[0].mUnk_18[2] = angle;
            mUnk_101c[0].mUnk_1e[2] = angle;
        case 72:
            mUnk_101c[0].mUnk_24[2] = angle;
            break;
        case 80:
            mUnk_11b4[0].mUnk_00 = FX_FROM_INT_ROUND(mUnk_12c8);
            mUnk_11b4[0].mUnk_0c[0] = mUnk_12c8;
        case 83:
            mUnk_11b4[0].mUnk_12[0] = mUnk_12c8;
            break;
        case 103:
            mUnk_11b4[1].mUnk_00 = FX_FROM_INT_ROUND(mUnk_12c8);
            mUnk_11b4[1].mUnk_0c[0] = mUnk_12c8;
        case 106:
            mUnk_11b4[1].mUnk_12[0] = mUnk_12c8;
            break;
        case 81:
            mUnk_11b4[0].mUnk_04 = FX_FROM_INT_ROUND(mUnk_12c8);
            mUnk_11b4[0].mUnk_0c[1] = mUnk_12c8;
        case 84:
            mUnk_11b4[0].mUnk_12[1] = mUnk_12c8;
            break;
        case 104:
            mUnk_11b4[1].mUnk_04 = FX_FROM_INT_ROUND(mUnk_12c8);
            mUnk_11b4[1].mUnk_0c[1] = mUnk_12c8;
        case 107:
            mUnk_11b4[1].mUnk_12[1] = mUnk_12c8;
            break;
        case 82:
            mUnk_11b4[0].mUnk_08 = FX_FROM_INT_ROUND(mUnk_12c8);
            mUnk_11b4[0].mUnk_0c[2] = mUnk_12c8;
        case 85:
            mUnk_11b4[0].mUnk_12[2] = mUnk_12c8;
            break;
        case 105:
            mUnk_11b4[1].mUnk_08 = FX_FROM_INT_ROUND(mUnk_12c8);
            mUnk_11b4[1].mUnk_0c[2] = mUnk_12c8;
        case 108:
            mUnk_11b4[1].mUnk_12[2] = mUnk_12c8;
            break;
        case 73:
        case 96:
            {
                UnkStruct_0205401c *model = mUnk_11b4[idx].mUnk_2c;
                if (model != NULL) {
                    model->func_02054034();
                    delete model;
                }
                mUnk_11b4[idx].mUnk_2c = NULL;
                s32 v = mUnk_12c8;
                if (v == -2) {
                    v = mUnk_11b4[idx].mUnk_36;
                }
                if (v >= 0) {
                    mUnk_11b4[idx].mUnk_36 = v;
                }
                break;
            }
        case 75:
        case 98:
            {
                UnkStruct_0205401c *model = mUnk_11b4[idx].mUnk_30;
                if (model != NULL) {
                    model->func_02054034();
                    delete model;
                }
                mUnk_11b4[idx].mUnk_30 = NULL;
                mUnk_11b4[idx].mUnk_37 = mUnk_12c8;
                break;
            }
        case 76:
        case 99:
            func_02068074(idx);
            mUnk_11b4[idx].mUnk_38 = mUnk_12c8;
            mUnk_12c4 = 0;
            mUnk_12c5 = 6;
            break;
        case 74:
        case 97:
            mUnk_12c4 = 0;
            mUnk_12c5 = 5;
            break;
        case 77:
        case 100:
            mUnk_11b4[idx].mUnk_43 = 1;
            break;
        case 87:
            mUnk_11b4[0].mUnk_18[1] = angle;
            mUnk_11b4[0].mUnk_1e[1] = angle;
        case 90:
            mUnk_11b4[0].mUnk_24[1] = angle;
            break;
        case 110:
            mUnk_11b4[1].mUnk_18[1] = angle;
            mUnk_11b4[1].mUnk_1e[1] = angle;
        case 113:
            mUnk_11b4[1].mUnk_24[1] = angle;
            break;
        case 78:
        case 101:
            func_02067e2c(idx, 1);
            break;
        case 79:
        case 102:
            func_02067e2c(idx, 0);
            break;
        case 92:
        case 115:
            mUnk_11b4[idx].mUnk_3f = mUnk_12c8;
            mUnk_11b4[idx].mUnk_40 = mUnk_12c8;
            mUnk_11b4[idx].mUnk_41 = mUnk_12c8;
            break;
        case 93:
            mUnk_11b4[idx].mUnk_40 = mUnk_12c8;
            break;
        case 94:
        case 117:
            mUnk_11b4[idx].mUnk_42 = mUnk_12c8;
            break;
        case 95:
        case 118:
            mUnk_11b4[idx].mUnk_3d = mUnk_12c8;
            break;
        case 119:
        case 120:
        case 144:
        case 145:
            {
                mUnk_112c[idx].mUnk_37 = 0;
                s32 v = mUnk_12c8;
                if (v != -2) {
                    mUnk_112c[idx].mUnk_34 = v;
                    switch (mUnk_12c6) {
                        case 0x78:
                        case 0x91:
                            mUnk_112c[idx].mUnk_36 = 1;
                            break;
                        default:
                            mUnk_112c[idx].mUnk_36 = 0;
                            break;
                    }
                }
                UnkStruct_0203d374 *pose = mUnk_112c[idx].mUnk_2c;
                if (pose != NULL) {
                    delete pose;
                }
                mUnk_112c[idx].mUnk_2c = NULL;
                if (func_02069680(idx)) {
                    mUnk_12c4 = 0;
                    mUnk_12c5 = 6;
                }
                break;
            }
        case 121:
        case 146:
            {
                UnkStruct_0203d374 *pose = mUnk_112c[idx].mUnk_30;
                if (pose != NULL) {
                    delete pose;
                }
                mUnk_112c[idx].mUnk_30 = NULL;
                mUnk_112c[idx].mUnk_35 = mUnk_12c8;
                func_020696e8(idx);
                mUnk_12c4 = 0;
                mUnk_12c5 = 6;
                break;
            }
        case 122:
        case 147:
            mUnk_112c[idx].mUnk_42 = 1;
            break;
        case 123:
        case 148:
            if (mUnk_112c[idx].mUnk_2c != NULL) {
                mUnk_112c[idx].mUnk_37 = 1;
            }
            break;
        case 124:
        case 149:
            mUnk_112c[idx].mUnk_37 = 0;
            break;
        case 125:
        case 150:
            mUnk_112c[idx].mUnk_00 = FX_FROM_INT_ROUND(mUnk_12c8);
            mUnk_112c[idx].mUnk_0c[0] = mUnk_12c8;
            mUnk_112c[idx].mUnk_12[0] = mUnk_12c8;
            break;
        case 128:
        case 153:
            mUnk_112c[idx].mUnk_12[0] = mUnk_12c8;
            break;
        case 126:
        case 151:
            mUnk_112c[idx].mUnk_04 = FX_FROM_INT_ROUND(mUnk_12c8);
            mUnk_112c[idx].mUnk_0c[1] = mUnk_12c8;
            mUnk_112c[idx].mUnk_12[1] = mUnk_12c8;
            break;
        case 129:
        case 154:
            mUnk_112c[idx].mUnk_12[1] = mUnk_12c8;
            break;
        case 127:
        case 152:
            mUnk_112c[idx].mUnk_08 = FX_FROM_INT_ROUND(mUnk_12c8);
            mUnk_112c[idx].mUnk_0c[2] = mUnk_12c8;
            mUnk_112c[idx].mUnk_12[2] = mUnk_12c8;
            break;
        case 130:
        case 155:
            mUnk_112c[idx].mUnk_12[2] = mUnk_12c8;
            break;
        case 131:
            mUnk_112c[0].mUnk_18[0] = angle;
            mUnk_112c[0].mUnk_1e[0] = angle;
        case 134:
            mUnk_112c[0].mUnk_24[0] = angle;
            break;
        case 156:
            mUnk_112c[1].mUnk_18[0] = angle;
            mUnk_112c[1].mUnk_1e[0] = angle;
        case 159:
            mUnk_112c[1].mUnk_24[0] = angle;
            break;
        case 132:
            mUnk_112c[0].mUnk_18[1] = angle;
            mUnk_112c[0].mUnk_1e[1] = angle;
        case 135:
            mUnk_112c[0].mUnk_24[1] = angle;
            break;
        case 157:
            mUnk_112c[1].mUnk_18[1] = angle;
            mUnk_112c[1].mUnk_1e[1] = angle;
        case 160:
            mUnk_112c[1].mUnk_24[1] = angle;
            break;
        case 133:
            mUnk_112c[0].mUnk_18[2] = angle;
            mUnk_112c[0].mUnk_1e[2] = angle;
        case 136:
            mUnk_112c[0].mUnk_24[2] = angle;
            break;
        case 158:
            mUnk_112c[1].mUnk_18[2] = angle;
            mUnk_112c[1].mUnk_1e[2] = angle;
        case 161:
            mUnk_112c[1].mUnk_24[2] = angle;
            break;
        case 137:
            mUnk_112c[0].mUnk_38 = mUnk_12c8;
            mUnk_112c[0].mUnk_3a = mUnk_12c8;
        case 138:
            mUnk_112c[0].mUnk_39 = mUnk_12c8;
            break;
        case 162:
            mUnk_112c[1].mUnk_38 = mUnk_12c8;
            mUnk_112c[1].mUnk_3a = mUnk_12c8;
        case 163:
            mUnk_112c[1].mUnk_39 = mUnk_12c8;
            break;
        case 139:
        case 164:
            mUnk_112c[idx].mUnk_3b = mUnk_12c8;
            break;
        case 140:
        case 165:
            mUnk_112c[idx].mUnk_3c = mUnk_12c8;
            break;
        case 141:
        case 166:
            if (mUnk_112c[idx].mUnk_36 == 0) {
                break;
            }
            switch (mUnk_12c8) {
                case 1:
                    mUnk_112c[idx].mUnk_2c->mModel->func_02028e54(0, 0, 0x1000);
                    break;
                case 3:
                    mUnk_112c[idx].mUnk_2c->mModel->func_02028e54(0, 0, 0x64000);
                    break;
                case 4:
                    mUnk_112c[idx].mUnk_2c->mModel->func_02028e54(0, 0, -1);
                    break;
                case 2:
                    mUnk_112c[idx].mUnk_2c->mModel->func_02028e54(0, 0, 0x64000);
                    mUnk_112c[idx].mUnk_2c->mModel->func_02028d80();
                    mUnk_112c[idx].mUnk_2c->mModel->mAnim->mUnk_0c = -0x1000;
                    break;
            }
            break;
        case 142:
        case 167:
            mUnk_112c[idx].mUnk_3f = mUnk_12c8;
            break;
        case 143:
        case 168:
            mUnk_112c[idx].mUnk_40 = mUnk_12c8;
            break;
        case 213:
            if (mUnk_14d2 == mUnk_12c8) {
                break;
            }
            mUnk_14d2 = mUnk_12c8;
            if (mUnk_14d2 > -1) {
                FrameTask::Get().func_0203dc34(0, mUnk_14d2);
            } else {
                FrameTask::Get().func_0203dd74(0);
            }
            break;
        case 214:
            FrameTask::Get().func_0203dd74(0);
            break;
        case 215:
            if (mUnk_12c8 > -1) {
                FrameTask::Get().func_0203dc34(1, (u16) (mUnk_12c8 + 0xa8));
            } else {
                FrameTask::Get().func_0203dd74(1);
            }
            break;
        case 216:
            FrameTask::Get().func_0203dd74(1);
            break;
        case 217:
            {
                if (mUnk_12c8 <= -1) {
                    break;
                }
                s32 id = (u16) mUnk_12c8;
                if (id < 0x11) {
                    SoundManager::Get().func_020246f8(id + 0x12);
                } else {
                    SoundManager::Get().func_020246f8((u16) (id - 0x11) + 0x44);
                }
                break;
            }
        case 218:
            SoundManager::Get().func_02024754(1);
            break;
        case 219:
            data_020d433c.mPlayerCount = 2;
            break;
        case 220:
            {
                s32 v = mUnk_12c8;
                if (v > 4) {
                    v = 4;
                } else if (v < 2) {
                    v = 2;
                }
                data_020d433c.mPlayerCount = v;
                break;
            }
        case 221:
            {
                s32 v = mUnk_12c8;
                if (v < 0) {
                    v = -v;
                }
                data_020d433c.mUnk_95 = v % 8;
                data_020d433c.mUnk_96 = 0;
                break;
            }
        case 222:
            {
                s32 v = mUnk_12c8;
                data_020d433c.mUnk_9f[1] = v % 22;
                break;
            }
        case 223:
            {
                s32 v = mUnk_12c8;
                data_020d433c.mUnk_9f[2] = v % 22;
                break;
            }
        case 224:
            {
                s32 v = mUnk_12c8;
                data_020d433c.mUnk_9f[3] = v % 22;
                break;
            }
        case 225:
            func_0206791c(1);
            break;
        case 226:
            func_0206791c(2);
            break;
        case 227:
            func_0206791c(3);
            break;
        case 228:
            func_0206791c(4);
            break;
        case 229:
            func_0206791c(7);
            break;
        case 230:
            func_0206791c(8);
            break;
        case 231:
            func_0206791c(5);
            {
                s32 v = mUnk_12c8;
                if (v > 4) {
                    v = 4;
                } else if (v < 2) {
                    v = 2;
                }
                data_020d433c.mPlayerCount = v;
            }
            break;
        case 232:
            CameraManager::Get().mActive = 0;
            CameraManager::Get().func_02026504(1, mUnk_1288, 0);
            mUnk_14e4 = 0;
            func_02088f28(mUnk_1018, 0);
            mUnk_12c5 = 9;
            break;
        case 233:
            func_02067708(1);
            SpriteManager::Get().func_02026dec(1, 7);
            func_020666d8();
            CameraManager::Get().func_02026504(2, mUnk_1288, (u32) mUnk_1288);
            SpriteManager::Get().SetFade(1, -16);
            mUnk_14e4 = 1;
            mUnk_14e5 = 1;
            GXS_SetVisiblePlane(1);
            func_02088f28(mUnk_1018, 1);
            mUnk_1018->func_02087eec();
            break;
        case 234:
            mUnk_14e7 = 1;
            mUnk_12c5 = 8;
            mUnk_14e8 = 0;
            mUnk_14f1 = 0;
            break;
    }
}

// FX32_CONST with an integer scale: the original converts after the shift.
#define FXC(x) ((fx32) (((x) > 0) ? ((x) * 4096 + 0.5f) : ((x) * 4096 - 0.5f)))
#define LERP_FX(dst, a, b) do { s32 av = (a); s32 v = av + (mUnk_14da * ((b) - av)) / mUnk_14dc; dst = FXC(v); } while (0)
#define LERP_S(dst, a, b) dst = (a) + (mUnk_14da * (s16) ((b) - (a))) / mUnk_14dc
#define LERP_N(dst, a, b) dst = (a) + (mUnk_14da * ((b) - (a))) / mUnk_14dc

void UnkStruct_02065c40::func_0206d3b8() {
    BOOL done = FALSE;
    switch (mUnk_12c5) {
        case 2:
            if (mUnk_12cc == mUnk_12ce) {
                done = TRUE;
            }
            break;
        case 3:
            if (mUnk_14d4 == 0) {
                if (mUnk_14d9 == 0) {
                    done = TRUE;
                }
            } else {
                done = TRUE;
            }
            break;
        case 5: {
            u32 idx = func_0206b05c(mUnk_12c6);
            s32 value = mUnk_12c8;
            if (value == -2) {
                value = mUnk_11b4[idx].mUnk_3a;
                func_02067e2c(idx, 0);
                UnkStruct_0205401c *model = mUnk_11b4[idx].mUnk_2c;
                if (model != NULL) {
                    model->func_02054034();
                    delete model;
                }
                mUnk_11b4[idx].mUnk_2c = NULL;
            }
            if (func_02067f30(idx, value) == FALSE) {
                done = TRUE;
            } else {
                mUnk_12c4 = 0;
                mUnk_12c5 = 6;
            }
            break;
        }
        case 6:
            if (mUnk_12c4 != 0) {
                mUnk_12c4--;
                break;
            }
            switch (mUnk_12c6) {
                case 0x4a:
                case 0x61:
                    done = func_020680f8(func_0206b05c(mUnk_12c6), 0);
                    break;
                case 0x4c:
                case 0x63:
                    done = func_020680f8(func_0206b05c(mUnk_12c6), 1);
                    break;
                case 0x3b:
                    done = func_02068954();
                    break;
                case 0xa9:
                case 0xbf:
                    done = func_02068e14(func_0206b05c(mUnk_12c6));
                    break;
                case 0xaa:
                case 0xc0:
                    done = func_02069194(func_0206b05c(mUnk_12c6));
                    break;
                case 0x77:
                case 0x78:
                case 0x90:
                case 0x91:
                    done = func_020697c8(func_0206b05c(mUnk_12c6), 0);
                    break;
                case 0x79:
                case 0x92:
                    done = func_020697c8(func_0206b05c(mUnk_12c6), 1);
                    break;
            }
            break;
        case 1:
            if (mUnk_12c4 != 0) {
                mUnk_12c4--;
            } else {
                done = TRUE;
            }
            break;
        case 4: {
            int n = mUnk_14da + 1;
            if (n > mUnk_14dc) {
                n = mUnk_14dc;
            }
            mUnk_14da = n;
            for (u8 i = 0; i < 2; i++) {
                LERP_FX(mUnk_11b4[i].mUnk_00, mUnk_11b4[i].mUnk_0c[0], mUnk_11b4[i].mUnk_12[0]);
                LERP_FX(mUnk_11b4[i].mUnk_04, mUnk_11b4[i].mUnk_0c[1], mUnk_11b4[i].mUnk_12[1]);
                LERP_FX(mUnk_11b4[i].mUnk_08, mUnk_11b4[i].mUnk_0c[2], mUnk_11b4[i].mUnk_12[2]);
                LERP_FX(mUnk_112c[i].mUnk_00, mUnk_112c[i].mUnk_0c[0], mUnk_112c[i].mUnk_12[0]);
                LERP_FX(mUnk_112c[i].mUnk_04, mUnk_112c[i].mUnk_0c[1], mUnk_112c[i].mUnk_12[1]);
                LERP_FX(mUnk_112c[i].mUnk_08, mUnk_112c[i].mUnk_0c[2], mUnk_112c[i].mUnk_12[2]);
                LERP_FX(mUnk_1084[i].mUnk_00, mUnk_1084[i].mUnk_0c[0], mUnk_1084[i].mUnk_12[0]);
                LERP_FX(mUnk_1084[i].mUnk_04, mUnk_1084[i].mUnk_0c[1], mUnk_1084[i].mUnk_12[1]);
                LERP_FX(mUnk_1084[i].mUnk_08, mUnk_1084[i].mUnk_0c[2], mUnk_1084[i].mUnk_12[2]);
                LERP_N(mUnk_11b4[i].mUnk_3f, mUnk_11b4[i].mUnk_41, mUnk_11b4[i].mUnk_40);
                LERP_N(mUnk_112c[i].mUnk_38, mUnk_112c[i].mUnk_3a, mUnk_112c[i].mUnk_39);
                LERP_S(mUnk_11b4[i].mUnk_18[0], mUnk_11b4[i].mUnk_1e[0], mUnk_11b4[i].mUnk_24[0]);
                LERP_S(mUnk_11b4[i].mUnk_18[1], mUnk_11b4[i].mUnk_1e[1], mUnk_11b4[i].mUnk_24[1]);
                LERP_S(mUnk_11b4[i].mUnk_18[2], mUnk_11b4[i].mUnk_1e[2], mUnk_11b4[i].mUnk_24[2]);
                LERP_S(mUnk_112c[i].mUnk_18[0], mUnk_112c[i].mUnk_1e[0], mUnk_112c[i].mUnk_24[0]);
                LERP_S(mUnk_112c[i].mUnk_18[1], mUnk_112c[i].mUnk_1e[1], mUnk_112c[i].mUnk_24[1]);
                LERP_S(mUnk_112c[i].mUnk_18[2], mUnk_112c[i].mUnk_1e[2], mUnk_112c[i].mUnk_24[2]);
                LERP_S(mUnk_1084[i].mUnk_18[0], mUnk_1084[i].mUnk_1e[0], mUnk_1084[i].mUnk_24[0]);
                LERP_S(mUnk_1084[i].mUnk_18[1], mUnk_1084[i].mUnk_1e[1], mUnk_1084[i].mUnk_24[1]);
                LERP_S(mUnk_1084[i].mUnk_18[2], mUnk_1084[i].mUnk_1e[2], mUnk_1084[i].mUnk_24[2]);
                {
                    fx32 q = FXC(mUnk_14da) / mUnk_14dc;
                    s16 a = mUnk_1244[i].mUnk_0c;
                    mUnk_1244[i].mUnk_00 = FX_Mul(FXC(mUnk_1244[i].mUnk_12 - a), q) + FXC(a);
                }
                {
                    fx32 q = FXC(mUnk_14da) / mUnk_14dc;
                    s16 a = mUnk_1244[i].mUnk_0e;
                    mUnk_1244[i].mUnk_04 = FX_Mul(FXC(mUnk_1244[i].mUnk_14 - a), q) + FXC(a);
                }
                {
                    fx32 q = FXC(mUnk_14da) / mUnk_14dc;
                    s16 a = mUnk_1244[i].mUnk_10;
                    mUnk_1244[i].mUnk_08 = FX_Mul(FXC(mUnk_1244[i].mUnk_16 - a), q) + FXC(a);
                }
            }
            LERP_S(mUnk_1274.mVec[0].x, mUnk_1274.mVec[1].x, mUnk_1274.mVec[2].x);
            LERP_S(mUnk_1274.mVec[0].y, mUnk_1274.mVec[1].y, mUnk_1274.mVec[2].y);
            LERP_S(mUnk_1274.mVec[0].z, mUnk_1274.mVec[1].z, mUnk_1274.mVec[2].z);
            LERP_N(mUnk_128c[0], mUnk_128c[1], mUnk_128c[2]);
            func_02069d0c();
            LERP_S(mUnk_1298.x, mUnk_129e.x, mUnk_12a4.x);
            LERP_S(mUnk_1298.y, mUnk_129e.y, mUnk_12a4.y);
            LERP_S(mUnk_1298.z, mUnk_129e.z, mUnk_12a4.z);
            {
                UnkStruct_02065c40_Kind j = UNK_02065C40_KIND_0;
                LERP_FX(mUnk_101c[j].mUnk_00, mUnk_101c[j].mUnk_0c[0], mUnk_101c[j].mUnk_12[0]);
                LERP_FX(mUnk_101c[j].mUnk_04, mUnk_101c[j].mUnk_0c[1], mUnk_101c[j].mUnk_12[1]);
                LERP_FX(mUnk_101c[j].mUnk_08, mUnk_101c[j].mUnk_0c[2], mUnk_101c[j].mUnk_12[2]);
                LERP_N(mUnk_101c[j].mUnk_18[0], mUnk_101c[j].mUnk_1e[0], mUnk_101c[j].mUnk_24[0]);
                LERP_N(mUnk_101c[j].mUnk_18[1], mUnk_101c[j].mUnk_1e[1], mUnk_101c[j].mUnk_24[1]);
                LERP_N(mUnk_101c[j].mUnk_18[2], mUnk_101c[j].mUnk_1e[2], mUnk_101c[j].mUnk_24[2]);
            }
            {
                s8 a = mUnk_14d7;
                s8 d = mUnk_14d6 - a;
                int v = a + (d * mUnk_14da) / mUnk_14dc;
                if (v > 16) {
                    v = 16;
                } else if (v < -16) {
                    v = -16;
                }
                mUnk_14d5 = v;
            }
            if (mUnk_14da >= mUnk_14dc) {
                done = TRUE;
                for (u8 i = 0; i < 2; i++) {
                    mUnk_11b4[i].mUnk_00 = FXC(mUnk_11b4[i].mUnk_12[0]);
                    mUnk_11b4[i].mUnk_04 = FXC(mUnk_11b4[i].mUnk_12[1]);
                    mUnk_11b4[i].mUnk_08 = FXC(mUnk_11b4[i].mUnk_12[2]);
                    mUnk_11b4[i].mUnk_0c[0] = mUnk_11b4[i].mUnk_12[0];
                    mUnk_11b4[i].mUnk_0c[1] = mUnk_11b4[i].mUnk_12[1];
                    mUnk_11b4[i].mUnk_0c[2] = mUnk_11b4[i].mUnk_12[2];
                    mUnk_11b4[i].mUnk_18[0] = mUnk_11b4[i].mUnk_24[0];
                    mUnk_11b4[i].mUnk_18[1] = mUnk_11b4[i].mUnk_24[1];
                    mUnk_11b4[i].mUnk_18[2] = mUnk_11b4[i].mUnk_24[2];
                    mUnk_11b4[i].mUnk_1e[0] = mUnk_11b4[i].mUnk_24[0];
                    mUnk_11b4[i].mUnk_1e[1] = mUnk_11b4[i].mUnk_24[1];
                    mUnk_11b4[i].mUnk_1e[2] = mUnk_11b4[i].mUnk_24[2];
                    mUnk_101c[i].mUnk_00 = FXC(mUnk_101c[i].mUnk_12[0]);
                    mUnk_101c[i].mUnk_04 = FXC(mUnk_101c[i].mUnk_12[1]);
                    mUnk_101c[i].mUnk_08 = FXC(mUnk_101c[i].mUnk_12[2]);
                    mUnk_101c[i].mUnk_0c[0] = mUnk_101c[i].mUnk_12[0];
                    mUnk_101c[i].mUnk_0c[1] = mUnk_101c[i].mUnk_12[1];
                    mUnk_101c[i].mUnk_0c[2] = mUnk_101c[i].mUnk_12[2];
                    mUnk_101c[i].mUnk_18[0] = mUnk_101c[i].mUnk_24[0];
                    mUnk_101c[i].mUnk_18[1] = mUnk_101c[i].mUnk_24[1];
                    mUnk_101c[i].mUnk_18[2] = mUnk_101c[i].mUnk_24[2];
                    mUnk_101c[i].mUnk_1e[0] = mUnk_101c[i].mUnk_24[0];
                    mUnk_101c[i].mUnk_1e[1] = mUnk_101c[i].mUnk_24[1];
                    mUnk_101c[i].mUnk_1e[2] = mUnk_101c[i].mUnk_24[2];
                    mUnk_112c[i].mUnk_00 = FXC(mUnk_112c[i].mUnk_12[0]);
                    mUnk_112c[i].mUnk_04 = FXC(mUnk_112c[i].mUnk_12[1]);
                    mUnk_112c[i].mUnk_08 = FXC(mUnk_112c[i].mUnk_12[2]);
                    mUnk_112c[i].mUnk_0c[0] = mUnk_112c[i].mUnk_12[0];
                    mUnk_112c[i].mUnk_0c[1] = mUnk_112c[i].mUnk_12[1];
                    mUnk_112c[i].mUnk_0c[2] = mUnk_112c[i].mUnk_12[2];
                    mUnk_112c[i].mUnk_18[0] = mUnk_112c[i].mUnk_24[0];
                    mUnk_112c[i].mUnk_18[1] = mUnk_112c[i].mUnk_24[1];
                    mUnk_112c[i].mUnk_18[2] = mUnk_112c[i].mUnk_24[2];
                    mUnk_112c[i].mUnk_1e[0] = mUnk_112c[i].mUnk_24[0];
                    mUnk_112c[i].mUnk_1e[1] = mUnk_112c[i].mUnk_24[1];
                    mUnk_112c[i].mUnk_1e[2] = mUnk_112c[i].mUnk_24[2];
                    mUnk_1084[i].mUnk_00 = FXC(mUnk_1084[i].mUnk_12[0]);
                    mUnk_1084[i].mUnk_04 = FXC(mUnk_1084[i].mUnk_12[1]);
                    mUnk_1084[i].mUnk_08 = FXC(mUnk_1084[i].mUnk_12[2]);
                    mUnk_1084[i].mUnk_0c[0] = mUnk_1084[i].mUnk_12[0];
                    mUnk_1084[i].mUnk_0c[1] = mUnk_1084[i].mUnk_12[1];
                    mUnk_1084[i].mUnk_0c[2] = mUnk_1084[i].mUnk_12[2];
                    mUnk_1084[i].mUnk_18[0] = mUnk_1084[i].mUnk_24[0];
                    mUnk_1084[i].mUnk_18[1] = mUnk_1084[i].mUnk_24[1];
                    mUnk_1084[i].mUnk_18[2] = mUnk_1084[i].mUnk_24[2];
                    mUnk_1084[i].mUnk_1e[0] = mUnk_1084[i].mUnk_24[0];
                    mUnk_1084[i].mUnk_1e[1] = mUnk_1084[i].mUnk_24[1];
                    mUnk_1084[i].mUnk_1e[2] = mUnk_1084[i].mUnk_24[2];
                    mUnk_1244[i].mUnk_00 = FXC(mUnk_1244[i].mUnk_12);
                    mUnk_1244[i].mUnk_04 = FXC(mUnk_1244[i].mUnk_14);
                    mUnk_1244[i].mUnk_08 = FXC(mUnk_1244[i].mUnk_16);
                    mUnk_1244[i].mUnk_0c = mUnk_1244[i].mUnk_12;
                    mUnk_1244[i].mUnk_0e = mUnk_1244[i].mUnk_14;
                    mUnk_1244[i].mUnk_10 = mUnk_1244[i].mUnk_16;
                }
                mUnk_1274.mVec[0].x = mUnk_1274.mVec[2].x;
                mUnk_1274.mVec[1].x = mUnk_1274.mVec[2].x;
                mUnk_1274.mVec[0].y = mUnk_1274.mVec[2].y;
                mUnk_1274.mVec[1].y = mUnk_1274.mVec[2].y;
                mUnk_1274.mVec[0].z = mUnk_1274.mVec[2].z;
                mUnk_1274.mVec[1].z = mUnk_1274.mVec[2].z;
                mUnk_128c[0] = mUnk_128c[2];
                mUnk_128c[1] = mUnk_128c[2];
                func_02069d0c();
                mUnk_12a4.x = mUnk_1298.x;
                mUnk_12a4.y = mUnk_1298.y;
                mUnk_12a4.z = mUnk_1298.z;
                mUnk_129e.x = mUnk_1298.x;
                mUnk_129e.y = mUnk_1298.y;
                mUnk_129e.z = mUnk_1298.z;
                mUnk_14d5 = mUnk_14d6;
                mUnk_14d7 = mUnk_14d6;
                func_0206e9d0(0);
            }
            G2_SetBlendBrightness(0x21, mUnk_14d5);
            break;
        }
        case 7:
            mUnk_12b9 = 4;
            break;
        case 8:
            if (mUnk_14e7 == 0) {
                mUnk_14e8 = 1;
                done = TRUE;
                mUnk_14f1 = 1;
            }
            break;
        case 9:
            func_02067b08();
            mUnk_14e6 = 1;
            mUnk_12c5 = 10;
            break;
        case 10:
            if (mUnk_14e6 == 0) {
                done = TRUE;
            }
            break;
        case 0:
        default:
            done = TRUE;
            break;
    }
    if (done) {
        mUnk_12b9 = 3;
        mUnk_12bc++;
    }
}
#undef FXC
#undef LERP_FX
#undef LERP_S
#undef LERP_N

void UnkStruct_02065c40::func_0206e9d0(u32 flag) {
    if (mUnk_14f3 == flag) {
        return;
    }
    mUnk_14f3 = flag;
    if (flag & 0xff) {
        func_02025588(func_0206ea14);
    } else {
        func_020255f8(func_0206ea14);
        func_020037e8(0);
    }
}

extern "C" void func_0206ea14() {
    s32 line = *(vu16 *) 0x4000006;
    if (line >= 0xc0) {
        return;
    }
    u32 time = line + UnkStruct_020c1884::Get().mFrame;
    UnkStruct_02065c40 *event = data_02219288;
    u16 angle = (time << 16) / 192;
    u16 height = event->mUnk_14dc;
    u16 width  = event->mUnk_14da;
    u16 third  = height / 3;
    u16 span;
    if (width >= height - third) {
        span = height - width;
    } else {
        span = width;
        if (span > third) {
            span = third;
        }
    }
    s32 offset = span * (SinIdx(angle) << 5) / third;
    func_020037e8((offset << 4) >> 16);
}



BOOL UnkStruct_02065c40::func_0206eb10(const char *path) {
    char file[0x80];
    if (mUnk_12bc != mUnk_12be) {
        return FALSE;
    }
    ChangeDir("/");
    sprintf(file, "/script/%s", path);
    func_02023708(file, 0, 0, 0, &mUnk_12b4, 1);
    func_02069bdc();
    func_02069c6c();
    mUnk_12ca = 0;
    mUnk_12cc = 0;
    mUnk_12ce = 0;
    func_02088ec4(mUnk_1018);
    mUnk_12ba = 0;
    mUnk_14ec = 0;
    func_02067e2c(0, 0);
    func_02067e2c(1, 0);
    func_02067708(0);
    for (u8 i = 0; i < 2; i++) {
        mUnk_11b4[i].mUnk_3c = 0;
        mUnk_11b4[i].mUnk_3b = 0;
        mUnk_11b4[i].mUnk_34 = 0;
        mUnk_11b4[i].mUnk_3d = 0x28;
        mUnk_11b4[i].mUnk_3f = 0x80;
        mUnk_11b4[i].mUnk_40 = 0x80;
        mUnk_11b4[i].mUnk_41 = 0x80;
        mUnk_11b4[i].mUnk_42 = 0;
        MI_CpuClear16(&mUnk_11b4[i], 0x18);
        MI_CpuClear16(mUnk_11b4[i].mUnk_18, 0x14);
        mUnk_1084[i].mUnk_4d = 0;
        mUnk_1084[i].mUnk_4e = 0;
        mUnk_1084[i].mUnk_4c = 1;
        mUnk_1084[i].mUnk_4f = 0x80;
        mUnk_112c[i].mUnk_37 = 0;
        mUnk_112c[i].mUnk_38 = 0x80;
        mUnk_112c[i].mUnk_39 = 0x80;
        mUnk_112c[i].mUnk_3a = 0x80;
        mUnk_112c[i].mUnk_3b = 0;
        mUnk_112c[i].mUnk_36 = 0;
        mUnk_112c[i].mUnk_3c = 0;
        mUnk_112c[i].mUnk_3d = 0;
        mUnk_112c[i].mUnk_3e = 0;
        mUnk_112c[i].mUnk_40 = 0;
        mUnk_112c[i].mUnk_3f = 10;
        MI_CpuClear16(&mUnk_112c[i], 0x18);
        MI_CpuClear16(mUnk_112c[i].mUnk_18, 0x14);
    }
    mUnk_12b9 = 1;
    return TRUE;
}


BOOL UnkStruct_02065c40::func_0206ed40() {
    if (func_02023874() != 0) {
        return FALSE;
    }
    mUnk_12b0 = (u32) func_02023864();
    u8 *data = (u8 *) mUnk_12b0;
    u32 size = mUnk_12b4;
    u8 *name = (u8 *) OS_Alloc(0x20);
    MI_CpuCopy8(data, name, 0x20);
    DC_FlushRange(name, 0x20);
    u8 *body = (u8 *) OS_Alloc(size - 0x20);
    MI_CpuCopy8(data + 0x20, body, size - 0x20);
    DC_FlushRange(body, size - 0x20);
    mUnk_12be = (size - 0x20) / 3;
    if (mUnk_12be >= 0x400) {
        mUnk_12be = 0x400;
    }
    mUnk_12bc = 0;
    u16 i = 0;
    do {
        u16 j = i * 3;
        mUnk_0c[i].mCommand = body[j];
        u8 *q = body + j;
        mUnk_0c[i].mValue = (q[1] << 24 >> 16) + q[2];
        i++;
    } while (i < mUnk_12be);
    do {
        mUnk_0c[i].mCommand = 0;
        mUnk_0c[i].mValue   = 0;
        i++;
    } while (i < 0x400);
    ChangeDir("/");
    char file[0x80];
    sprintf(file, "/font/%s.mes", name);
    func_02023708(file, 0, 0, 0, &mUnk_12b4, 0);
    OS_Free(name);
    OS_Free(body);
    OS_Free(data);
    return TRUE;
}



BOOL UnkStruct_02065c40::func_0206ef08() {
    if (func_02023874() != 0) {
        return FALSE;
    }
    mUnk_12b0 = (u32) func_02023864();
    func_02087ccc(mUnk_1018, (void *) mUnk_12b0);
    func_02088ec4(mUnk_1018);
    return TRUE;
}






BOOL UnkStruct_02065c40::func_0206ef4c(const char *name, u32 unk2, u32 unk3, u32 unk4, s32 unk5) {
    BOOL isTwo = mUnk_12ba == 2;
    BOOL isNine = mUnk_12b9 == 9;
    BOOL done = mUnk_12bc == mUnk_12be;
    if (isTwo | (isNine & done)) {
    u8 mode = mUnk_14e2;
    if (mode >= 6) {
        mode = 0;
    }
    u8 n = 0;
    mUnk_0c[n].mCommand = 9;
    mUnk_0c[n].mValue = 2;
    n++;
    mUnk_0c[n].mCommand = 0xe;
    mUnk_0c[n].mValue = 1;
    n++;
    mUnk_0c[n].mCommand = 0xf;
    mUnk_0c[n].mValue = -16;
    n++;
    mUnk_0c[n].mCommand = 0x19;
    mUnk_0c[n].mValue = 0;
    n++;
    s16 extra = *(s16 *) &unk4;
    if (extra >= 0) {
        mUnk_0c[n].mCommand = 0x3b;
        mUnk_0c[n].mValue = extra;
        n++;
    }
    mUnk_0c[n].mCommand = 0x49;
    mUnk_0c[n].mValue = unk5;
    n++;
    if (unk5 >= 0) {
        mUnk_0c[n].mCommand = 0x4a;
        mUnk_0c[n].mValue = 0;
        n++;
        mUnk_0c[n].mCommand = 0x50;
        mUnk_0c[n].mValue = data_020a2198[mode].mUnk_0c;
        n++;
        mUnk_0c[n].mCommand = 0x51;
        mUnk_0c[n].mValue = data_020a2198[mode].mUnk_0e;
        n++;
        mUnk_0c[n].mCommand = 0x52;
        mUnk_0c[n].mValue = data_020a2198[mode].mUnk_10;
        n++;
        mUnk_0c[n].mCommand = 0x57;
        mUnk_0c[n].mValue = data_020a2198[mode].mUnk_12;
        n++;
        mUnk_0c[n].mCommand = 0x5c;
        mUnk_0c[n].mValue = data_020a2198[mode].mUnk_14;
        n++;
        mUnk_0c[n].mCommand = 0x4e;
        mUnk_0c[n].mValue = 0;
        n++;
    }
    mUnk_0c[n].mCommand = 0x31;
    mUnk_0c[n].mValue = data_020a1c04[mode].mUnk_0;
    n++;
    mUnk_0c[n].mCommand = 0x33;
    mUnk_0c[n].mValue = data_020a1c04[mode].mUnk_2;
    n++;
    mUnk_0c[n].mCommand = 0x35;
    mUnk_0c[n].mValue = data_020a1c04[mode].mUnk_4;
    n++;
    mUnk_0c[n].mCommand = 0x3a;
    mUnk_12ac = data_020a1c04[mode].mUnk_6;
    n++;
    mUnk_0c[n].mCommand = 0x1c;
    mUnk_0c[n].mValue = data_020a2198[mode].mUnk_00;
    n++;
    mUnk_0c[n].mCommand = 0x1d;
    mUnk_0c[n].mValue = data_020a2198[mode].mUnk_02;
    n++;
    mUnk_0c[n].mCommand = 0x1e;
    mUnk_0c[n].mValue = data_020a2198[mode].mUnk_04;
    n++;
    mUnk_0c[n].mCommand = 0x22;
    mUnk_0c[n].mValue = data_020a2198[mode].mUnk_06;
    n++;
    mUnk_0c[n].mCommand = 0x23;
    mUnk_0c[n].mValue = data_020a2198[mode].mUnk_08;
    n++;
    mUnk_0c[n].mCommand = 0x24;
    mUnk_0c[n].mValue = data_020a2198[mode].mUnk_0a;
    n++;
    mUnk_0c[n].mCommand = 8;
    mUnk_0c[n].mValue = unk5;
    n++;
    mUnk_0c[n].mCommand = 0xe;
    mUnk_0c[n].mValue = 10;
    n++;
    mUnk_0c[n].mCommand = 0xf;
    mUnk_0c[n].mValue = 0;
    n++;
    mUnk_0c[n].mCommand = 0x19;
    mUnk_0c[n].mValue = 0;
    n++;
    mUnk_12c0 = n;
    mUnk_0c[n].mCommand = 0xd7;
    mUnk_0c[n].mValue = func_02070554((s8) unk5, unk3);
    n++;
    mUnk_0c[n].mCommand = 1;
    mUnk_0c[n].mValue = unk2;
    n++;
    mUnk_12c2 = n;
    if (mUnk_14e2 == 0) {
        mUnk_0c[n].mCommand = 0xa9;
        mUnk_0c[n].mValue = data_020d433c.mUnk_7e_0 + 0x20;
        n++;
        mUnk_0c[n].mCommand = 0xae;
        mUnk_0c[n].mValue = -0x13b;
        n++;
        mUnk_0c[n].mCommand = 0xaf;
        mUnk_0c[n].mValue = data_020a2198[mode].mUnk_0e;
        n++;
        mUnk_0c[n].mCommand = 0xb0;
        mUnk_0c[n].mValue = -0x834;
        n++;
        mUnk_0c[n].mCommand = 0xb5;
        mUnk_0c[n].mValue = 0x140;
        n++;
        mUnk_0c[n].mCommand = 0xb2;
        mUnk_0c[n].mValue = data_020a2198[mode].mUnk_02;
        n++;
        mUnk_0c[n].mCommand = 0xac;
        n++;
        mUnk_0c[n].mCommand = 0x10;
        mUnk_0c[n].mValue = 0;
        n++;
    }
    mUnk_101c[0].mUnk_32 = 0;
    mUnk_12bc = 0;
    mUnk_12be = n;
    ChangeDir("/");
    char file[0x80];
    sprintf(file, "/font/%s.mes", name);
    func_02023708(file, 0, 0, 0, &mUnk_12b4, 0);
    mUnk_1018->func_02088ebc(0);
    mUnk_12ca = 0;
    func_02088ec4(mUnk_1018);
    mUnk_12cc = 0;
    mUnk_12ce = 0;
    mUnk_12b9 = 2;
    mUnk_14e8 = 0;
    return TRUE;
    }
    return FALSE;
}

extern const u8 data_020a1b8a[];
extern "C" void MI_CpuCopy8(const void *src, void *dest, u32 size);
void func_02088ec4(UnkStruct_02087898 *object);

void UnkStruct_02065c40::func_0206f564(s32 unk1, u32 unk2, s32 unk3) {
    BOOL isTwo   = mUnk_12ba == 2;
    BOOL isThree = mUnk_12ba == 3;
    if (!(isTwo | isThree)) {
        return;
    }
    if (mUnk_12bc < mUnk_12be - 1) {
        return;
    }
    if (!(((mUnk_12ca == 6) | ((mUnk_12ca == 5) | (mUnk_12ca == 7))) & (unk1 != mUnk_1018->mUnk_34))) {
        return;
    }
    if (mUnk_14e2 == 5) {
        mUnk_12ca = 0;
        func_02088ec4(mUnk_1018);
        mUnk_12d2[0] = unk1;
        mUnk_12bc    = mUnk_12c0;
        mUnk_0c[mUnk_12c0].mValue = func_02070554(mUnk_11b4[0].mUnk_36, unk2);
        mUnk_12b9 = 3;
        return;
    }
    mUnk_1018->func_02088e64((s16) unk1, mUnk_13d2[0], 0, 0);
    mUnk_12ca = 2;
    if (mUnk_14e2 != 0) {
        return;
    }
    if (unk3 < 0) {
        return;
    }
    u8 value = unk3 + 0x20;
    if (value == mUnk_0c[mUnk_12c2].mValue) {
        return;
    }
    mUnk_0c[mUnk_12c2].mValue = value;
    mUnk_12bc                 = mUnk_12c2;
    mUnk_0c[mUnk_12c0].mValue = -1;
    mUnk_12b9                 = 3;
}

// The four computed entries of the script below (FxScale, Identity16) are filled at run time on the first call, under a
// guard at 0x0221928c (right after data_02219288).
inline s16 FxScale(float v) {
    return (s16) (v * 4096.0f + 0.5f);
}
inline s16 Identity16(s16 v) {
    return v;
}

// The script table at 0x020b467c follows the vtable in .data and is addressed through its own symbol, both for the
// run-time init and for the copy (one literal-pool word). Under `npdata` it gets its own section, which gives that; in
// the shared .data it would be addressed from the section base, and as a function-local static with a dynamic
// initializer the compiler's init addresses it through the section symbol and the copy through its own (two pool
// words). The hand-written guard and the init are stand-ins for the original's form.
#pragma section npdata begin
UnkStruct_02065c40_cmd data_020b467c[17] = {
    {1, 0xe}, {-16, 0xf}, {0, 0x19}, {3, 9}, {9, 0x3b}, {0, 0x3c}, {0x2d, 0x2f}, {0, 0x31},
    {0, 0x33}, {0, 0x35}, {0, 0x3a}, {10, 0xe}, {0, 0xf}, {0, 0x19}, {0, 0xd7}, {0, 8}, {0, 1}};
#pragma section npdata end
static u32 sScriptGuard;

extern "C" BOOL func_0206f6f0(UnkStruct_02065c40 *event, const char *name, u32 id, u32 unk3, u32 unk4) {
    BOOL idle = event->mUnk_12bc == event->mUnk_12be;
    BOOL nine = event->mUnk_12b9 == 9;
    if (nine & idle) {
        if (!(sScriptGuard & 1)) {
            data_020b467c[7].mValue  = FxScale(0.27f);
            data_020b467c[8].mValue  = FxScale(0.43f);
            data_020b467c[9].mValue  = FxScale(0.86f);
            data_020b467c[10].mValue = Identity16(0x7fff);
            sScriptGuard |= 1;
        }
        MIi_CpuCopy32(data_020b467c, event->mUnk_0c, 0x44);
        event->mUnk_0c[14].mValue = event->func_02070554((s8) *(s16 *) &unk4, unk3);
        event->mUnk_0c[15].mValue = *(s16 *) &unk4;
        event->mUnk_0c[16].mValue = id;
        event->mUnk_12bc = 0;
        event->mUnk_12be = 0x11;
        ChangeDir("/");
        char file[0x80];
        sprintf(file, "/font/%s.mes", name);
        func_02023708(file, 0, 0, 0, &event->mUnk_12b4, 0);
        event->mUnk_1018->func_02088ebc(0);
        event->mUnk_12ca = 0;
        func_02088ec4(event->mUnk_1018);
        event->mUnk_12cc = 0;
        event->mUnk_12ce = 0;
        event->mUnk_12b9 = 2;
        return TRUE;
    }
    return FALSE;
}

extern "C" void func_0206f88c(UnkStruct_02065c40 *event, u32 category, s32 value, u32 unk4) {
    BOOL idle    = event->mUnk_12bc >= event->mUnk_12be;
    BOOL waiting = event->mUnk_12ba == 3;
    BOOL changed = value != event->mUnk_1018->mUnk_34;
    if (!(idle & (waiting & changed))) {
        return;
    }
    MIi_CpuCopy32(data_020a2118, event->mUnk_0c, 0x80);
    s32 offset = data_020a1b12[category].mUnk_3;
    if (offset < 0) {
        event->mUnk_0c[5].mValue = -1;
    } else {
        event->mUnk_0c[5].mValue = offset + 0x24;
    }
    u8 kind                  = data_020a1b12[category].mUnk_2;
    s16 extra                = data_020a1b12[category].mUnk_0;
    event->mUnk_0c[10].mValue = kind;
    event->mUnk_0c[13].mValue = extra;
    event->mUnk_0c[29].mValue = event->func_02070554((s8) kind, unk4);
    event->mUnk_0c[30].mValue = kind;
    event->mUnk_0c[31].mValue = value;
    event->mUnk_12c0 = 0x1d;
    event->mUnk_12bc = 0;
    event->mUnk_12be = 0x20;
    event->mUnk_12ca = 0;
    func_02088ec4(event->mUnk_1018);
    event->mUnk_12cc = 0;
    event->mUnk_12ce = 0;
    event->mUnk_12b9 = 3;
}

struct UnkStruct_s8x2 {
    /* 0 */ s8 mUnk_0;
    /* 1 */ s8 mUnk_1;
};
struct UnkStruct_s16x4 {
    /* 0 */ s16 mUnk_0;
    /* 2 */ s16 mUnk_2;
    /* 4 */ s16 mUnk_4;
    /* 6 */ s16 mUnk_6;
};
extern const UnkStruct_s8x2 data_020a1b68[];
extern const UnkStruct_s16x4 data_020a22a4;
extern const UnkStruct_s16x4 data_020a221c[];

u32 UnkStruct_02065c40::func_0206f9b4(const char *name, u32 unk2, u32 unk3, u32 unk4, u32 unk5, u32 unk6, u32 unk7) {
    u16 shorts[2] = {unk2, 0};
    u8 bytes[2]   = {unk3, 0};
    return func_0206fa20(name, shorts, bytes, 1, *(u16 *) &unk4, unk5, *(u8 *) &unk6, unk7, 0);
}



extern "C" void MIi_CpuCopy32(const void *src, void *dest, u32 size);

u8 UnkStruct_02065c40::func_0206fa20(const char *name, const u16 *unk2, const u8 *unk3, u32 unk4, u32 unk5, u32 unk6, u32 unk7,
                                     u32 unk8, u32 unk9) {
    if (mUnk_12b9 != 9 || mUnk_12bc != mUnk_12be) {
        return FALSE;
    }
    u8 n = 0;
    mUnk_0c[n].mCommand = 14;
    mUnk_0c[n].mValue = 0;
    n++;
    mUnk_0c[n].mCommand = 15;
    mUnk_0c[n].mValue = -16;
    n++;
    mUnk_0c[n].mCommand = 25;
    mUnk_0c[n].mValue = 0;
    n++;
    mUnk_0c[n].mCommand = 59;
    mUnk_0c[n].mValue = *(u8 *) &unk7;
    n++;
    mUnk_0c[n].mCommand = 60;
    n++;
    mUnk_0c[n].mCommand = 73;
    mUnk_0c[n].mValue = unk2[0] % 22;
    n++;
    mUnk_0c[n].mCommand = 74;
    mUnk_0c[n].mValue = unk3[0];
    n++;
    mUnk_0c[n].mCommand = 78;
    n++;
    switch (unk6) {
        case 2:
        if (*(u8 *) &unk7 != 2) {
            if (unk2[0] == 0x10) {
                mUnk_0c[n].mCommand = 80;
                mUnk_0c[n].mValue = 0;
                n++;
            }
        }
        mUnk_0c[n].mCommand = 81;
        mUnk_0c[n].mValue = data_020a1b68[unk2[0]].mUnk_0;
        n++;
        mUnk_0c[n].mCommand = 82;
        mUnk_0c[n].mValue = data_020a1b68[unk2[0]].mUnk_1;
        n++;
        mUnk_0c[n].mCommand = 28;
        mUnk_0c[n].mValue = 0;
        n++;
        mUnk_0c[n].mCommand = 29;
        mUnk_0c[n].mValue = 12;
        n++;
        mUnk_0c[n].mCommand = 30;
        mUnk_0c[n].mValue = -1;
        n++;
        mUnk_0c[n].mCommand = 34;
        mUnk_0c[n].mValue = -3;
        n++;
        mUnk_0c[n].mCommand = 35;
        mUnk_0c[n].mValue = 7;
        n++;
        mUnk_0c[n].mCommand = 36;
        mUnk_0c[n].mValue = -101;
        n++;
            break;
        case 0xa:
        mUnk_0c[n].mCommand = 80;
        mUnk_0c[n].mValue = 0;
        n++;
        mUnk_0c[n].mCommand = 81;
        mUnk_0c[n].mValue = 0;
        n++;
        mUnk_0c[n].mCommand = 82;
        mUnk_0c[n].mValue = 0;
        n++;
        mUnk_0c[n].mCommand = 87;
        mUnk_0c[n].mValue = 330;
        n++;
        mUnk_0c[n].mCommand = 92;
        mUnk_0c[n].mValue = 128;
        n++;
        mUnk_0c[n].mCommand = 78;
        mUnk_0c[n].mValue = 0;
        n++;
        mUnk_0c[n].mCommand = 49;
        mUnk_0c[n].mValue = 2580;
        n++;
        mUnk_0c[n].mCommand = 51;
        mUnk_0c[n].mValue = 1761;
        n++;
        mUnk_0c[n].mCommand = 53;
        mUnk_0c[n].mValue = (s16) 0xc0f6;
        n++;
        mUnk_0c[n].mCommand = 58;
        n++;
        mUnk_12ac = 0x7fff;
        mUnk_0c[n].mCommand = 28;
        mUnk_0c[n].mValue = -62;
        n++;
        mUnk_0c[n].mCommand = 29;
        mUnk_0c[n].mValue = 20;
        n++;
        mUnk_0c[n].mCommand = 30;
        mUnk_0c[n].mValue = -21;
        n++;
        mUnk_0c[n].mCommand = 34;
        mUnk_0c[n].mValue = -13;
        n++;
        mUnk_0c[n].mCommand = 35;
        mUnk_0c[n].mValue = 7;
        n++;
        mUnk_0c[n].mCommand = 36;
        mUnk_0c[n].mValue = -108;
        n++;
            break;
        default:
        if (*(u8 *) &unk4 == 1) {
            if (*(u8 *) &unk7 == 2) {
                if (unk2[0] == 6) {
                    mUnk_0c[n].mCommand = 81;
                    mUnk_0c[n].mValue = -20;
                    n++;
                } else if (unk2[0] == 0xe) {
                    mUnk_0c[n].mCommand = 80;
                    mUnk_0c[n].mValue = 15;
                    n++;
                    mUnk_0c[n].mCommand = 81;
                    mUnk_0c[n].mValue = 20;
                    n++;
                }
            }
        } else if (*(u8 *) &unk4 == 2) {
            mUnk_0c[n].mCommand = 80;
            mUnk_0c[n].mValue = data_020a22a4.mUnk_0;
            n++;
            mUnk_0c[n].mCommand = 82;
            mUnk_0c[n].mValue = data_020a22a4.mUnk_4;
            n++;
            mUnk_0c[n].mCommand = 87;
            mUnk_0c[n].mValue = data_020a22a4.mUnk_6;
            n++;
            const UnkStruct_s16x4 *e = data_020a221c;
            u16 id = unk2[1];
            mUnk_0c[n].mCommand = 96;
            mUnk_0c[n].mValue = unk2[1] % 22;
            n++;
            mUnk_0c[n].mCommand = 97;
            mUnk_0c[n].mValue = unk3[1];
            n++;
            mUnk_0c[n].mCommand = 101;
            n++;
            mUnk_0c[n].mCommand = 103;
            mUnk_0c[n].mValue = e[id].mUnk_0;
            e += id;
            n++;
            mUnk_0c[n].mCommand = 104;
            mUnk_0c[n].mValue = e->mUnk_2;
            n++;
            mUnk_0c[n].mCommand = 105;
            mUnk_0c[n].mValue = e->mUnk_4;
            n++;
            mUnk_0c[n].mCommand = 110;
            mUnk_0c[n].mValue = e->mUnk_6;
            n++;
            mUnk_0c[n].mCommand = 117;
            mUnk_0c[n].mValue = 1;
            n++;
            mUnk_0c[n].mCommand = 115;
            mUnk_0c[n].mValue = 148;
            n++;
        }
        if (unk9 != 0) {
            mUnk_0c[n].mCommand = 28;
            mUnk_0c[n].mValue = -11;
            n++;
            mUnk_0c[n].mCommand = 29;
            mUnk_0c[n].mValue = 16;
            n++;
            mUnk_0c[n].mCommand = 30;
            mUnk_0c[n].mValue = 0;
            n++;
            mUnk_0c[n].mCommand = 34;
            mUnk_0c[n].mValue = -1;
            n++;
            mUnk_0c[n].mCommand = 35;
            mUnk_0c[n].mValue = 10;
            n++;
            mUnk_0c[n].mCommand = 36;
            mUnk_0c[n].mValue = -100;
            n++;
        }
    }
    mUnk_0c[n].mCommand = 14;
    mUnk_0c[n].mValue = 10;
    n++;
    mUnk_0c[n].mCommand = 15;
    mUnk_0c[n].mValue = 0;
    n++;
    mUnk_0c[n].mCommand = 25;
    mUnk_0c[n].mValue = 0;
    n++;
    s16 value;
    if (*(u8 *) &unk4 == 1) {
        value = unk2[0];
    } else {
        value = unk2[1];
    }
    mUnk_0c[n].mCommand = 8;
    mUnk_0c[n].mValue = value;
    n++;
    mUnk_0c[n].mCommand = 1;
    mUnk_0c[n].mValue = *(u16 *) &unk5;
    n++;
    mUnk_0c[n].mCommand = 0xd7;
    mUnk_0c[n].mValue = func_02070554((s8) value, unk6);
    n++;
    mUnk_0c[n].mCommand = 2;
    n++;
    if (unk8 != 0) {
        mUnk_0c[n].mCommand = 234;
        n++;
        mUnk_14ed = 1;
    }
    mUnk_0c[n].mCommand = 15;
    mUnk_0c[n].mValue = -16;
    n++;
    mUnk_0c[n].mCommand = 25;
    mUnk_0c[n].mValue = 0;
    n++;
    mUnk_12bc = 0;
    mUnk_12be = n;
    func_02088ec4(mUnk_1018);
    ChangeDir("/");
    char file[0x80];
    sprintf(file, "/font/%s.mes", name);
    func_02023708(file, 0, 0, 0, &mUnk_12b4, 0);
    mUnk_14ec = 0;
    mUnk_12b9 = 2;
    return TRUE;
}

BOOL UnkStruct_02065c40::func_020702e0(s32 unk1, s32 unk2, s32 unk3, s32 unk4, s32 unk5, s32 unk6) {
    if (mUnk_12bc != mUnk_12be) {
        return FALSE;
    }
    MI_CpuCopy8(data_020a1b8a, mUnk_0c, 0x24);
    mUnk_0c[0].mValue = unk1;
    mUnk_0c[1].mValue = unk2;
    mUnk_0c[2].mValue = unk3;
    mUnk_0c[3].mValue = *(s16 *) &unk4;
    mUnk_0c[4].mValue = *(s16 *) &unk5;
    mUnk_0c[5].mValue = *(s16 *) &unk6;
    mUnk_12bc  = 0;
    u16 count  = mUnk_12be = 9;
    mUnk_12ca  = 0;
    mUnk_12ce  = 0;
    mUnk_12cc  = 0;
    mUnk_12d2[0] = count - 10;   // -1 derived from the count, as the original computes it
    func_02088ec4(mUnk_1018);
    mUnk_101c[0].mUnk_32 = 1;
    mUnk_12b9            = 3;
    return TRUE;
}

extern const u8 data_020a1b48[];

void UnkStruct_02065c40::func_02070390() {
    s32 target = -2;
    s32 flag   = 0;
    while (mUnk_12bc < mUnk_12be) {
        switch (mUnk_0c[mUnk_12bc].mCommand) {
            case 0xd5:
                target = (s8) mUnk_0c[mUnk_12bc].mValue;
                break;
            case 0xd6:
                target = -1;
                break;
            case 0xe5:
                flag = 0xe5;
                break;
        }
        mUnk_12bc++;
    }
    MI_CpuCopy8(data_020a1b48, mUnk_0c, 0x20);
    if (mUnk_14e4 != 0) {
        mUnk_0c[3].mCommand = 0xe8;
    } else {
        mUnk_0c[3].mCommand = 0;
    }
    if (target >= -1) {
        if (target != -1) {
            mUnk_0c[5].mCommand = 0xd6;
        } else {
            mUnk_0c[5].mCommand = 0xd5;
            mUnk_0c[5].mValue   = target;
        }
    } else {
        mUnk_0c[5].mCommand = 0;
    }
    if (flag != 0) {
        mUnk_0c[7].mCommand = flag;
    } else {
        mUnk_0c[7].mCommand = 0;
    }
    mUnk_12bc = 0;
    mUnk_12be = 8;
    mUnk_12ca = 0;
    mUnk_14ee = 0;
    mUnk_12ce = mUnk_12cc;
    func_02088ec4(mUnk_1018);
    mUnk_12b9 = 3;
    mUnk_14ec = 1;
    SpriteManager::Get().StartFade(2);
    if (mUnk_14f3 != 0) {
        func_0206e9d0(0);
    }
}

s32 UnkStruct_02065c40::func_02070554(s32 kind, s32 index) {
    if ((kind < 0) | (kind > 0x10)) {
        return -1;
    }
    s32 result = -1;
    if (kind > 5) {
        if (index < 9) {
            result = (s16) (data_020a248c[kind - 6][index] - 0xa8);
        }
    } else {
        if (index < 12) {
            result = (s16) (data_020a233c[kind][index] - 0xa8);
        }
    }
    if (result < 0) {
        return -1;
    }
    return result;
}


extern "C" void func_020705f0(UnkStruct_02065c40 *event) {
    if (event->mUnk_1018 != NULL) {
        event->mUnk_1018->mUnk_34 = -1;
    }
}
