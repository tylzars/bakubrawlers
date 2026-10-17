#include "Game/ScriptPlayer.hpp"
#include "System/TouchManager.hpp"
#include "System/SoundManager.hpp"
#include "System/GameSystem.hpp"
#include "Graphics/SpriteManager.hpp"
#include "Graphics/Camera.hpp"
#include "System/InputManager.hpp"
#include "System/FrameTask.hpp"
#include "UI/StorytalkWindow.hpp"

#include "Graphics/CellAnimation.hpp"
#include "Graphics/Display.hpp"
#include "Graphics/Model.hpp"
#include "Game/UnkStruct_020d433c.hpp"
#include "Game/SaveData.hpp"

#include <nitro/gx.h>
#include <nitro/mi.h>
#include <nitro/os.h>
#include <stdio.h>
#include "Graphics/CharacterModel.hpp"
#include "System/FileSystem.hpp"
#include "Graphics/BakuganModel.hpp"
#include "Graphics/ModelInternal.hpp"

// Wait kinds (mWaitKind): set by ExecuteCommand, run by UpdateWait while mState is 4; RunCommands goes on to the
// next command at once only for SCRIPT_WAIT_KIND_NONE. Kinds 5 (commands 74 / 97: func_02067f30 one frame later,
// then kind 6 when it returns TRUE) and 8 (command 234: until mUnk_14e7 is 0 again) stay literals.
#define SCRIPT_WAIT_KIND_NONE                  0  // UpdateWait ends the wait at once
#define SCRIPT_WAIT_KIND_TIMER                 1  // until mWaitTimer has counted down to 0 (commands 3 and 13)
#define SCRIPT_WAIT_KIND_MESSAGES              2  // until mMessagePos reaches mMessageCount (command 2)
#define SCRIPT_WAIT_KIND_BRIGHTNESS            3  // until mBrightnessFrameCount is 0 (commands 25-27)
#define SCRIPT_WAIT_KIND_LERP                  4  // steps mLerpFrames up to mLerpFrameCount, interpolating every slot
                                                  // (commands 16, 17)
#define SCRIPT_WAIT_KIND_LOAD                  6  // the command's LoadCharacter / LoadMap / LoadModel /
                                                  // LoadNextModelFiles / LoadSphere until TRUE (the mWaitTimer
                                                  // countdown before it never runs: every kind-6 wait starts at 0)
#define SCRIPT_WAIT_KIND_STOP                  7  // StopWithResult: holds mState at 4, never ends
#define SCRIPT_WAIT_KIND_LOAD_GRAPHICS         9  // LoadGraphics, sets mGraphicsLoaded, then kind 10 (command 232)
#define SCRIPT_WAIT_KIND_CLEAR_GRAPHICS_LOADED 10 // until mGraphicsLoaded is 0 again (the scene's ClearGraphicsLoaded)

// Fade kind (mFadeKind), set by commands 25-27. UpdateBrightnessFade, its only reader, calls G2_SetBlendBrightness
// only for SCRIPT_FADE_KIND_BLEND (command 25). 1 (command 26, stored with SpriteManager::StartSubFade) and 2
// (command 27, with StartFade) stay literals: nothing reads them apart.
#define SCRIPT_FADE_KIND_BLEND 0

// Camera modes (mCameraMode, SetCameraMode).
// SCRIPT_CAMERA_MODE_ANGLES: UpdateCameraAndLight puts the target 100 units from the eye along mCameraAngles
// (commands 40-45); SCRIPT_CAMERA_MODE_TARGET: the target stays mCameraPoints[1] (commands 34-39; SetCameraMode first
// places it from the angles when it switches to this mode).
#define SCRIPT_CAMERA_MODE_ANGLES 0
#define SCRIPT_CAMERA_MODE_TARGET 1

ScriptPlayer *gScriptPlayer;
extern "C" void MI_CpuCopy8(const void *src, void *dest, u32 size);
extern "C" void DC_FlushRange(void *buffer, u32 size);
void SetNoStorytalkMessage(StorytalkWindow *object);
extern "C" void MIi_CpuCopy32(const void *src, void *dest, u32 size);
extern const u32 gScriptSpriteSlots[8][6];
extern "C" void SaveTexSlotRanges(TexSlotRanges *slot);
extern "C" void RestoreSndHeapState(u32 a, u32 b);

void FillParkSlots();

extern const u8 data_020a1c34[];

struct UnkStruct_Pair {
    u8 mUnk_0;
    u8 mUnk_1;
};

extern const UnkStruct_Pair data_020a26a4[][5][10][3];

void UnlockStoreItems();

BOOL UnlockCharacters();

BOOL UnlockFields();

BOOL UnlockTips();

extern const u8 data_020a2118[];

struct UnkStruct_020a1b12 {
    /* 0 */ s16 mUnk_0;
    /* 2 */ u8 mUnk_2;
    /* 3 */ s8 mUnk_3;
    /* 4 */
};

extern const UnkStruct_020a1b12 data_020a1b12[];

ScriptPlayer::ScriptPlayer(u32 unk1, u32 unk2, u32 unk3) :
    SceneBase(0x3e8, 0) {
    mShadowModel = NULL;
    mLights = NULL;
    mStorytalkWindow = NULL;
    mLightColor = 0;
    mModelLoadStep = 0;
    mState = 0;
    mUnk_12ba = 0;
    mCommandIndex = 0;
    mCommandCount = 0;
    mUnk_12c0 = 0;
    mUnk_12c2 = 0;
    mWaitKind = SCRIPT_WAIT_KIND_NONE;
    mMessageState = 0;
    mInputTimer = 0;
    mMessagePos = 0;
    mMessageCount = 0;
    mUnk_12d0 = 0;
    mUnk_14d2 = -1;
    mFadeKind = SCRIPT_FADE_KIND_BLEND;
    mUnk_14d4 = 0;
    mBrightness = -16;
    mBrightnessTo = -16;
    mBrightnessFrom = 0;
    mBrightnessFrames = 0;
    mBrightnessFrameCount = 0;
    mFrameCount = 1;
    mFinishRequested = 0;
    mResult = 0;
    mUnk_14e2 = unk1;
    mUnk_14e4 = 0;
    mUnk_14e5 = 0;
    mGraphicsLoaded = 0;
    mUnk_14e7 = 0;
    mUnk_14e8 = 0;
    mUnk_14e9 = 0;
    mSaveRequested = 0;
    mUnlockPopupHandle = 0;
    mSkipped = 0;
    mUnk_14ed = 0;
    mUnk_14ee = 0;
    mUnk_14ef = 0;
    mUnk_14f0 = unk2;
    mUnk_14f1 = unk3;
    mCutsceneAnimIndex = 0;
    mBg0Wave = 0;
    gScriptPlayer = this;
    MI_CpuClear32(&mCutsceneCellAnim, 4);
    MI_CpuClear32(mCharacters, 0x90);
    MI_CpuClear32(mMaps, 0x68);
    MI_CpuClear32(mModels, 0xa8);
    MI_CpuClear32(mSpheres, 0x88);
    for (u8 i = 0; i < 2; i++) {
        mCharacters[i].mCharacterId = -1;
        mCharacters[i].mNextCharacterId = -1;
        mMaps[i].mMapId = -1;
        mModels[i].mModelId = -1;
        mModels[i].mNextModelId = -1;
        mModels[i].mScale = 1;
        mModels[i].mStorytalkCell2X = 0x80;
        mCharacters[i].mShadowScale = 0x28;
        mCharacters[i].mStorytalkCell2X = 0x80;
        mCharacters[i].mUnk_40 = 0x80;
        mCharacters[i].mUnk_41 = 0x80;
        mSpheres[i].mTableIndex = -1;
        mSpheres[i].mNextTableIndex = -1;
        mSpheres[i].mStorytalkCell2X = 0x80;
        mSpheres[i].mUnk_39 = 0x80;
        mSpheres[i].mUnk_3a = 0x80;
        mSpheres[i].mShadowY = 0;
        mSpheres[i].mShadowScale = 10;
    }
    MI_CpuClear32(&mFileData, 4);
    MI_CpuClear32(&mFileSize, 4);
    mCamera = new Camera();
    mCamera->SetUp(0, 0x1000, 0);
    mCamera->SetClip(0x1000, 0x1388000);
    CameraManager::Get().mCameraIndex = 0;
    CameraManager::Get().SetCamerasAndRequestMode(1, mCamera, 0);
    ResetCamera();
    ResetLight();
    mStorytalkWindow = new StorytalkWindow();
    G2_SetBlendBrightness(1, -16);
    if (mUnk_14f0 != 0) {
        FrameTask::Get().StopChannel(0);
    }
    FrameTask::Get().StopChannel(1);
    data_020d433c.mRules = 0x11;
    data_020d433c.mMicEnabled = gSaveData[0].mMicEnabled;
    data_020d433c.mUnk_7a = 1;
    if (data_020d433c.mTutorialPart != 0) {
        RestoreSndHeapState(SoundManager::Get().mHeap, 2);
    } else {
        SoundManager::Get().RestoreHeapState();
    }
    SoundManager::Get().LoadSoundGroup(3);
    SetupSpriteSlots();
}

ScriptPlayer::~ScriptPlayer() {
    StopAsyncLoad();
    if (mCutsceneCellAnim != NULL) {
        delete mCutsceneCellAnim;
    }
    ReleaseModels(1);
    for (u8 i = 0; i < 2; i++) {
        for (u8 j = 0; j < 3; j++) {
            if (mModels[i].mNextFiles[j] != NULL) {
                OS_Free(mModels[i].mNextFiles[j]);
            }
        }
    }
    ModelInstance *instance = mShadowModel;
    if (instance != NULL) {
        instance->mOwnFlags = MODEL_OWN_ALL;
        delete instance;
    }
    delete mCamera;
    ModelLightGroup *lights = mLights;
    if (lights != NULL) {
        delete lights;
    }
    CameraManager::Get().SetCamerasAndRequestMode(0, NULL, 0);
    if (mUnk_14f0 != 0) {
        FrameTask::Get().StopChannel(0);
    }
    FrameTask::Get().StopChannel(1);
    SoundManager::Get().StopSeqArcPlayerSounds(1);
    if (data_020d433c.mTutorialPart == 0) {
        SoundManager::Get().StopSeq(1);
    }
}

void ScriptPlayer::SetupSpriteSlots() {
    SpriteManager::Get().ResetSlots();
    TexSlotRanges slot;
    SaveTexSlotRanges(&slot);
    for (u8 i = 0; i < 8; i++) {
        for (u8 j = 0; j < 6; j++) {
            slot.mUnk_00[j] = gScriptSpriteSlots[i][j];
        }
        SpriteManager::Get().SetSlot(i, &slot);
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

u32 ScriptPlayer::func_020667ec() {
    u8 type = gSaveData[0].mUnk_42;
    if (type >= 11) {
        return 7;
    }
    return data_020b4324[type]->mUnk_78;
}

// `result` as s16 gives the original's reload at the early return (`ldrcs r0, [sp]`); as a spilled s32 the -1 still
// in r0 is reused and the reload dropped. `pending` declared first puts it in the higher stack slot, like the original.
s32 ScriptPlayer::func_02066814(u32 unk1) {
    s32 pending;
    s16 result = -1;
    u8 type    = gSaveData[0].mUnk_42;
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
        u8 position = gSaveData[0].mUnk_43;
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
                            SetupBattle(type, (u8) next);
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
                            SetupBattle(type, (u8) next);
                            if (unk1 != 0) {
                                if (data_020b4324[type]->mEntries[index].mUnk_0a != 0 && data_020b4324[type]->mEntries[next].mUnk_06 != 1) {
                                    mUnk_14e9 = 1;
                                }
                            }
                        }
                    } else {
                        SetupBattle(type, index);
                        if (mUnk_14e2 == 0 || mUnk_14e9 == 1) {
                            StopWithResult(SCRIPT_RESULT_BATTLE);
                        }
                    }
                    break;
            }
            if (gSaveData[0].mUnk_5bf_0) {
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
                mSaveRequested = 1;
            }
            if (advance != 0) {
                u32 limit = 10;
                if (gSaveData[0].mUnk_5bf_0) {
                    limit = func_02067240(type);
                }
                u8 now = ++gSaveData[0].mUnk_43;
                if (now >= limit) {
                    done = 1;
                } else if (data_020b4324[type]->mEntries[now].mUnk_06 == 0) {
                    done = 1;
                }
                data_020d433c.mUnk_7d = 0;
            }
            if (done != 0) {
                if (gSaveData[0].mUnk_5bf_0) {
                    switch (gSaveData[0].mUnk_42) {
                        case 1:
                            gSaveData[0].mUnk_5bf |= 0x2;
                            break;
                        case 3:
                            gSaveData[0].mUnk_5bf |= 0x4;
                            break;
                        case 6:
                            gSaveData[0].mUnk_5bf |= 0x8;
                            break;
                        case 7:
                            gSaveData[0].mUnk_5bf |= 0x10;
                            break;
                        case 8:
                            gSaveData[0].mUnk_5bf |= 0x20;
                            break;
                        case 9:
                            gSaveData[0].mUnk_5bf |= 0x40;
                            break;
                    }
                    gSaveData[0].mUnk_42 = 10;
                    StopWithResult(SCRIPT_RESULT_MAP);
                    mSaveRequested = 1;
                } else {
                    u32 next = 0;
                    if (data_020b4324[type]->mUnk_78 != 0) {
                        next = SCRIPT_RESULT_MAP;
                    }
                    if (next != 0) {
                        StopWithResult(next);
                    }
                    if (++gSaveData[0].mUnk_42 == 10) {
                        StopWithResult(8);
                    }
                }
                gSaveData[0].mUnk_43 = 0;
                data_020d433c.mUnk_7d = 0;
                mUnk_14e2 = 7;
            }
            if (advance | done) {
                FillParkSlots();
            }
        }
    }
    if (gSaveData[0].mUnk_5bf_0) {
        result = -1;
    }
    return result;
}

BOOL ScriptPlayer::func_02066c7c() {
    return func_02066814(0) != -1;
}

void ScriptPlayer::SetupBattle(u32 unk1, u32 unk2) {
    data_020d433c.mUnk_99 = 3;
    data_020d433c.mPlayerCount = 2;
    data_020d433c.mHasPartners = 0;
    for (u8 i = 0; i < 4; i++) {
        data_020d433c.mCharacterIds[i] = i;
        data_020d433c.mUnk_a3[i] = 0;
        data_020d433c.mPartners[i] = i;
    }
    switch (mUnk_14e2) {
        case 3:
            data_020d433c.mNextScene = SCENE_ARENA_WINDOW;
            break;
        case 2:
            data_020d433c.mNextScene = SCENE_PARK;
            break;
        default:
            data_020d433c.mNextScene = SCENE_MAP;
            break;
    }
    data_020d433c.mRules   = 0x11;
    data_020d433c.mMicEnabled = gSaveData[0].mMicEnabled;
    for (u8 j = 0; j < 3; j++) {
        data_020d433c.mCharacterIds[j + 1] = func_02067120(unk1, unk2, (u8) (j + 1), 0);
        data_020d433c.mUnk_a3[j + 1] = func_02067120(unk1, unk2, (j + 1) & 0xff, 1);
    }
    UnkStruct_020b4324_entry *entry = &data_020b4324[unk1]->mEntries[unk2];
    data_020d433c.mFieldId = entry->mUnk_07;
    data_020d433c.mUnk_96 = entry->mUnk_08;
    if (gSaveData[0].mUnk_5bf_0 && (gSaveData[0].mUnk_580 & 0x80)) {
        for (u8 k = 0; k < 3; k++) {
            if (data_020d433c.mCharacterIds[k + 1] == 7) {
                data_020d433c.mUnk_a3[k + 1] = 2;
            }
        }
    }
    switch (data_020b4324[unk1]->mEntries[unk2].mUnk_06) {
        case 3:
            data_020d433c.mHasPartners = 1;
            data_020d433c.mPlayerCount = 4;
            data_020d433c.mPartners[0] = 1;
            data_020d433c.mPartners[1] = 0;
            data_020d433c.mPartners[2] = 3;
            data_020d433c.mPartners[3] = 2;
            break;
        case 4:
            data_020d433c.mUnk_99 = 6;
            break;
        case 5:
            data_020d433c.mPlayerCount = 4;
            break;
    }
    data_020d433c.mCharacterIds[0] = 0x15;
    u8 type = gSaveData[0].mUnk_42;
    if ((type == 9) & (gSaveData[0].mUnk_43 == 8)) {
        data_020d433c.mLastBattle = 1;
    } else {
        data_020d433c.mLastBattle = 0;
    }
    BOOL reached = unk2 >= func_02067240(unk1);
    BOOL known   = func_020670b4(type) != 0;
    if (reached & known) {
        data_020d433c.mUnk_8a_4 = 1;
    } else {
        data_020d433c.mUnk_8a_4 = 0;
    }
}

u32 ScriptPlayer::func_02066f60() {
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
        StartScript(path);
        mUnk_14e8 = 1;
        return 1;
    }
    func_02066814(1);
    if (mUnk_14e9 != 0) {
        StopWithResult(SCRIPT_RESULT_BATTLE);
    }
    return 0;
}

u32 ScriptPlayer::func_020670b4(u32 id) {
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

u8 ScriptPlayer::func_02067120(u32 unk1, u32 unk2, u32 unk3, u32 unk4) {
    if (unk3 != 0) {
        u8 slot = unk3 - 1;
        if (gSaveData[0].mUnk_5bf_0) {
            u32 kind = func_020670b4(unk1);
            if (kind == 0) {
                kind = 1;
            }
            u8 group = gSaveData[0].mUnk_25;
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

u8 ScriptPlayer::func_02067240(u32 id) {
    return data_020a1c38[func_020670b4(id)];
}

void ScriptPlayer::Update() {
    if (mUnk_14e4 != 0) {
        mUnk_14e5++;
        mUnk_14e5 &= 1;
    }
    ApplyPendingSwaps();
    UpdateCameraAndLight();
    CameraManager &manager = CameraManager::Get();
    Camera *camera         = manager.mCameras[manager.mCameraIndex];
    data_02211374.mCameraPosition = camera->mPosition;
    data_02211374.mCameraUp       = camera->mUp;
    data_02211374.mCameraTarget   = camera->mTarget;
    MTX_LookAt(&camera->mPosition, &camera->mUp, &camera->mTarget, &data_02211374.mView);
    data_02211374.mFlags &= ~0xe8;
    manager.mProjection[manager.mCameraIndex] = data_02211374.mProjection;
    manager.mView[manager.mCameraIndex]       = data_02211374.mView;
    manager.mMtxA[manager.mCameraIndex]       = *func_02032710();
    manager.mMtxB[manager.mCameraIndex]       = *func_02032604();
    manager.mMtxC[manager.mCameraIndex]       = *func_020323b8();
    UpdateBrightness();
    UpdateMessages();
    func_0206a01c();
    DrawMap();
    UpdateCharacterExtraAnims();
    DrawCharacters();
    DrawSpheres();
    DrawModels();
    if (mFinishRequested != 0) {
        if (mState < 10) {
            mState = 10;
        }
    }
    switch (mState) {
        case 0:
            LoadGraphics();
            mState = 5;
            break;
        case 1:
            if (ReadScript()) {
                mState++;
            }
            break;
        case 2:
            if (ReadMessages()) {
                mState++;
            }
            break;
        case 3:
            RunCommands();
            break;
        case 4:
            UpdateWait();
            break;
        case 5:
            if (mSaveRequested != 0) {
                UnlockStoreItems();
                if (UnlockCharacters()) {
                    mUnlockPopupHandle = 1;
                    mState = 7;
                } else if (UnlockFields()) {
                    mUnlockPopupHandle = 2;
                    mState = 7;
                } else if (UnlockTips()) {
                    mUnlockPopupHandle = 3;
                    mState = 7;
                } else {
                    mState++;
                }
            } else {
                mState = 8;
            }
            break;
        case 6:
            if (mSaveRequested != 0) {
                break;
            }
            LoadGraphics();
            mState = 8;
            break;
        case 7:
            if (mUnlockPopupHandle != 0) {
                break;
            }
            LoadGraphics();
            mState = 5;
            break;
        case 8:
            if (func_02066f60()) {
                if (mState == 8) {
                    mState = 1;
                }
            } else {
                if (mState == 8) {
                    mState = 9;
                }
            }
            if (mSaveRequested != 0) {
                mState = 5;
            }
            break;
        case 10:
            mStorytalkWindow->Close();
            ReleaseModels(1);
            RequestScene(1);
            mFinished = 1;
            break;
    }
}

void ScriptPlayer::RequestFinish() {
    mFinishRequested = 1;
}

void ScriptPlayer::ReleaseModels(u32 unk1) {
    for (u8 i = 0; i < 2; i++) {
        UnkStruct_0205401c *model = mCharacters[i].mCharacter;
        if (model != NULL) {
            model->Release();
            delete model;
        }
        mCharacters[i].mCharacter = NULL;
        model                = mCharacters[i].mNextCharacter;
        if (model != NULL) {
            model->Release();
            delete model;
        }
        mCharacters[i].mNextCharacter = NULL;
        BakuganModel *object = mSpheres[i].mSphere;
        if (object != NULL) {
            delete object;
        }
        mSpheres[i].mSphere = NULL;
        object               = mSpheres[i].mNextSphere;
        if (object != NULL) {
            delete object;
        }
        mSpheres[i].mNextSphere = NULL;
        if (unk1 != 0) {
            ModelInstance *instance = mMaps[i].mModel;
            if (instance != NULL) {
                instance->mOwnFlags = MODEL_OWN_ALL;
                delete instance;
                mMaps[i].mModel = NULL;
            }
            mMaps[i].mMapId = -1;
        }
        ReleaseModel(i);
        mCharacters[i].mCharacterId = -1;
        mCharacters[i].mNextCharacterId = -1;
        mModels[i].mModelId = -1;
        mModels[i].mNextModelId = -1;
        mSpheres[i].mTableIndex = -1;
        mSpheres[i].mNextTableIndex = -1;
        mCharacters[i].mSwapPending = 0;
        mSpheres[i].mSwapPending = 0;
        mModels[i].mSwapPending = 0;
    }
    ModelInstance *instance;
    if (unk1 != 0 && (instance = mShadowModel) != NULL) {
        instance->mOwnFlags = MODEL_OWN_ALL;
        delete instance;
        mShadowModel = NULL;
    }
}

BOOL ScriptPlayer::IsRunning() {
    return mState != 9;
}

u32 ScriptPlayer::GetResult() {
    if (mSaveRequested | mUnlockPopupHandle) {
        return 0;
    }
    return mResult;
}

void ScriptPlayer::StopWithResult(u32 unk1) {
    mResult = unk1;
    mWaitKind = SCRIPT_WAIT_KIND_STOP;
    mState = 4;
}

BOOL ScriptPlayer::IsGraphicsLoaded() {
    return mGraphicsLoaded;
}

void ScriptPlayer::ClearGraphicsLoaded() {
    mGraphicsLoaded = 0;
}

u32 ScriptPlayer::GetSaveRequest() {
    if (mState == 6) {
        return mSaveRequested;
    }
    return 0;
}

void ScriptPlayer::ClearSaveRequest() {
    mSaveRequested = 0;
}

u32 ScriptPlayer::GetUnlockPopupHandle() {
    return mUnlockPopupHandle;
}

void ScriptPlayer::ClearUnlockPopupHandle() {
    mUnlockPopupHandle = 0;
}

extern const s8 data_020a1b2a[][5];
extern const s8 data_020a1ac6[][5];
extern const s8 data_020a1ad5[][5];
extern const s8 data_020a1ae4[][5];
extern const s8 data_020a1af3[][5];
extern const s8 data_020a1b02[][5];

void ScriptPlayer::func_02067998(u8 unk1) {
    if (unk1) {
        NNSG3dAnmObj *obj = mMaps[0].mModel->mAnim->GetAnimObjForSet(1, unk1);
        mMaps[0].mModel->AttachExtraAnim(obj);
    } else {
        mMaps[0].mModel->DetachExtraAnim();
    }
}

void ScriptPlayer::UpdateBrightness() {
    if (mUnk_14d4 == 0) {
        UpdateBrightnessFade();
        return;
    }
    if (mBrightness == 0) {
        return;
    }
    mBrightnessFrom = mBrightness;
    mBrightnessTo = 0;
    mBrightnessFrames = 0;
    s32 v = mBrightness;
    if (v < 0) {
        v = -v;
    }
    mBrightnessFrameCount = v;
    UpdateBrightnessFade();
}

void ScriptPlayer::UpdateBrightnessFade() {
    if (mBrightnessFrameCount == 0) {
        return;
    }
    s32 t = mBrightnessFrames + 1;
    if (t > mBrightnessFrameCount) {
        t = mBrightnessFrameCount;
    }
    mBrightnessFrames = t;
    s8 from = mBrightnessFrom;
    s8 to   = mBrightnessTo;
    s8 d = to - from;
    int v = from + d * mBrightnessFrames / mBrightnessFrameCount;
    if (v > 16) {
        v = 16;
    } else if (v < -16) {
        v = -16;
    }
    mBrightness = v;
    if (mBrightnessFrameCount <= mBrightnessFrames) {
        mBrightness = mBrightnessTo;
        mBrightnessFrom = mBrightnessTo;
        mBrightnessFrames = 0;
        mBrightnessFrameCount = 0;
    }
    if (mFadeKind == SCRIPT_FADE_KIND_BLEND) {
        G2_SetBlendBrightness(0x21, mBrightness);
    }
}

extern const u8 data_020a1bae[];
#define REG_G3_DISP3DCNT (*(vu16 *) 0x4000060)

void ScriptPlayer::LoadGraphics() {
    char path[0x80];
    GX_SetVisiblePlane(0x11);
    G2_SetBG0Offset(0, 0);
    G2_SetBG0Priority(3);
    REG_G3_DISP3DCNT = (REG_G3_DISP3DCNT & ~0x3000) | 0x20;
    ChangeDir("/story/");
    if (mUnk_14e4 == 0 && (mUnk_14f1 | func_02066c7c())) {
        ChangeDir("/story/");
        SpriteScreen &screen = SpriteManager::Get().Screen(1);
        screen.SetObjVramMode(3);
        sprintf(path, "cutscene_b_obj.nce_ncg1D.bin");
        screen.LoadCharacters(path, 0xd000, 0);
        sprintf(path, "cutscene_b_obj_ncl.bin");
        screen.LoadExtPalette(path, 0, 0);
        if (mCutsceneCellAnim != NULL) {
            delete mCutsceneCellAnim;
        }
        mCutsceneAnimIndex = 0;
        mCutsceneCellAnim = new CellAnimation("cutscene_b_obj_nce.bin", mCutsceneAnimIndex, 1, 1);
        mCutsceneCellAnim->mCharBase = 0x1a0;
        mCutsceneCellAnim->mPaletteBase   = 0;
        if (mUnk_14e2 == 5) {
            mUnk_14f1 = 0;
            mUnk_14ed = 1;
        }
    }
    ChangeDir("/");
    if (mShadowModel == NULL) {
        SpriteManager::Get().RestoreSlot(7);
        NitroTexture *texture = new NitroTexture("story/mdl/cut_shadow.nsbtx");
        NitroModel *model     = new NitroModel;
        model->Load("story/mdl/cut_shadow.nsbmd", texture);
        texture->FreeFile();
        mShadowModel = model->CreateInstance(NULL);
    }
    if (mLights == NULL) {
        mLights = new ModelLightGroup(data_020a1bae);
        ResetLight();
    }
    mStorytalkWindow->LoadGraphics();
}

void ScriptPlayer::SetCharacterVisible(u32 index, u8 unk2) {
    UnkStruct_0205401c *model = mCharacters[index].mCharacter;
    if (model == NULL) {
        return;
    }
    model->SetVisible(unk2);
    mCharacters[index].mUnk_3b = 0;
    mCharacters[index].mVisible = unk2;
}

void ScriptPlayer::ApplyPendingSwaps() {
    for (u8 i = 0; i < 2; i++) {
        if (mCharacters[i].mSwapPending != 0) {
            SwapCharacter(i);
            mCharacters[i].mSwapPending = 0;
        }
        if (mSpheres[i].mSwapPending != 0) {
            SwapSphere(i);
            mSpheres[i].mSwapPending = 0;
        }
        if (mModels[i].mSwapPending != 0) {
            SwapModel(i);
            mModels[i].mSwapPending = 0;
        }
    }
}

BOOL ScriptPlayer::func_02067f30(u32 index, s32 value) {
    if (value < 0) {
        SetCharacterVisible(index, 0);
        UnkStruct_0205401c *model = mCharacters[index].mCharacter;
        if (model != NULL) {
            model->Release();
            delete model;
        }
        mCharacters[index].mCharacter = NULL;
        return FALSE;
    }
    mCharacters[index].mUnk_3a = value;
    if (mCharacters[index].mCharacter != NULL) {
        if (mCharacters[index].mUnk_3a != mCharacters[index].mUnk_39) {
            if (mCharacters[index].mCharacter->IsVisible()) {
                mCharacters[index].mUnk_3b = 1;
            }
            if (mCharacters[index].mCharacterId == 0x15) {
                ((CharacterModel *) mCharacters[index].mCharacter->mCharacter)->mParts[0]->mAnim->mReachedEnd = 1;
            } else {
                ((ModelTransform *) mCharacters[index].mCharacter->mStoryCharacter)->mModel->mAnim->mReachedEnd = 1;
            }
            UpdateCharacterAnimation(index);
        }
        return FALSE;
    }
    if (index == 1) {
        ReleaseModel(0);
    }
    SetCharacterVisible(index, 0);
    UnkStruct_0205401c *model = mCharacters[index].mCharacter;
    if (model != NULL) {
        model->Release();
        delete model;
    }
    mCharacters[index].mCharacter = NULL;
    return TRUE;
}

void ScriptPlayer::CreateNextCharacter(u32 index) {
    UnkStruct_0205401c *model = mCharacters[index].mNextCharacter;
    if (model != NULL) {
        model->Release();
        delete model;
    }
    mCharacters[index].mNextCharacter = NULL;
    if (mCharacters[index].mNextCharacterId < 0) {
        return;
    }
    mCharacters[index].mNextCharacter = new UnkStruct_0205401c();
    mCharacters[index].mNextCharacter->StartLoad((u8) mCharacters[index].mNextCharacterId, 0, 0, 0);
}

BOOL ScriptPlayer::LoadCharacter(u32 index, u32 flag) {
    u8 variant;
    s8 characterId;
    UnkStruct_0205401c **slot;
    if (flag) {
        variant = mCharacters[index].mUnk_38;
        characterId = mCharacters[index].mNextCharacterId;
        slot    = &mCharacters[index].mNextCharacter;
    } else {
        variant = mCharacters[index].mUnk_3a;
        characterId = mCharacters[index].mCharacterId;
        slot    = &mCharacters[index].mCharacter;
    }
    if (characterId < 0) {
        return TRUE;
    }
    func_020682a0(index, characterId);
    if (flag) {
        if (!(*slot)->UpdateLoad()) {
            return FALSE;
        }
    } else {
        *slot = new UnkStruct_0205401c();
        u8 id = characterId;
        (*slot)->Create(id, 0, 0, 0);
        u8 a = func_020687a8(id, variant, 0);
        u8 b = func_020687a8(id, variant, 3);
        (*slot)->func_02054644(b, a);
        if (characterId == 0x15) {
            ((CharacterModel *) (*slot)->mCharacter)->mParts[0]->mAnim->mLoop = 1;
        } else {
            ((ModelTransform *) (*slot)->mStoryCharacter)->mModel->mAnim->mLoop = 1;
        }
        mCharacters[index].mUnk_39 = variant;
        (*slot)->SetVisible(0);
        mCharacters[index].mVisible = 0;
        MI_CpuClear16(&mCharacters[index], 0x18);
        MI_CpuClear16(mCharacters[index].mRot, 0x14);
        ResetCharacterParams(index);
    }
    return TRUE;
}

void ScriptPlayer::func_020682a0(u32 index, u32 characterId) {
    if (characterId == 0x15) {
        SpriteManager::Get().RestoreSlot(3);
    } else if (index == 0) {
        SpriteManager::Get().RestoreSlot(2);
    } else {
        SpriteManager::Get().RestoreSlot(1);
    }
}

struct UnkStruct_020a22ac {
    /* 0 */ const char *mName;
    /* 4 */ s16 mY;
    /* 6 */ s16 mZ;
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

struct ScriptSphereEntry {
    /* 0 */ u8 mBakuganId;
    /* 1 */ u8 mVariant;
    /* 2 */ u8 mCharacterId;
    /* 3 */
};
extern const ScriptSphereEntry gScriptSphereEntries[];

void ScriptPlayer::ResetCharacterParams(u32 index) {
    switch (mCharacters[index].mCharacterId) {
        case 2:
            mCharacters[index].mOffsetY = 0x3c;
            break;
        case 3:
            mCharacters[index].mOffsetY = -0x14;
            break;
        case 14:
            mCharacters[index].mOffsetY = 0x28;
            break;
        case 11:
            mCharacters[index].mOffsetY = -0x78;
            break;
        case 12:
            mCharacters[index].mOffsetY = 0x28;
            break;
        case 9:
            mCharacters[index].mOffsetY = -0x28;
            break;
        default:
            mCharacters[index].mOffsetY = 0;
            break;
    }
    switch (mCharacters[index].mCharacterId) {
        case 13:
            mCharacters[index].mShadowScale = 0x3c;
            break;
        case 14:
            mCharacters[index].mShadowScale = 0x1e;
            break;
        default:
            mCharacters[index].mShadowScale = 0x28;
            break;
    }
    mCharacters[index].mStorytalkCell2X = 0x80;
    mCharacters[index].mUnk_40 = 0x80;
    mCharacters[index].mUnk_41 = 0x80;
    mCharacters[index].mStorytalkCell2Frame = 0;
    mCharacters[index].mExtraAnimEnabled = 0;
}

void ScriptPlayer::SwapCharacter(u32 index) {
    if (mCharacters[index].mNextCharacter == NULL) {
        return;
    }
    UnkStruct_0205401c *old = mCharacters[index].mCharacter;
    if (old != NULL) {
        old->Release();
        delete old;
    }
    mCharacters[index].mCharacter = mCharacters[index].mNextCharacter;
    mCharacters[index].mNextCharacter = NULL;
    mCharacters[index].mCharacterId = mCharacters[index].mNextCharacterId;
    mCharacters[index].mUnk_39 = mCharacters[index].mUnk_38;
    mCharacters[index].mUnk_3a = mCharacters[index].mUnk_38;
    ResetCharacterParams(index);
    s8 characterId = mCharacters[index].mCharacterId;
    s8 variant = mCharacters[index].mUnk_3a;
    func_020682a0(index, characterId);
    mCharacters[index].mCharacter->FinishLoad();
    u8 variantId = variant;
    u8 id        = characterId;
    u8 a         = func_020687a8(id, variantId, 0);
    u8 b         = func_020687a8(id, variantId, 3);
    mCharacters[index].mCharacter->func_02054644(b, a);
    if (characterId == 0x15) {
        ((CharacterModel *) mCharacters[index].mCharacter->mCharacter)->mParts[0]->mAnim->mLoop = 1;
    } else {
        ((ModelTransform *) mCharacters[index].mCharacter->mStoryCharacter)->mModel->mAnim->mLoop = 1;
    }
}

void ScriptPlayer::SetCharacterExtraAnimEnabled(u32 index, u32 value) {
    if (value == mCharacters[index].mExtraAnimEnabled) {
        return;
    }
    mCharacters[index].mExtraAnimEnabled = value;
    if (mCharacters[index].mCharacter == NULL) {
        return;
    }
    mCharacters[index].mCharacter->SetExtraAnimEnabled(mCharacters[index].mExtraAnimEnabled);
    if (mCharacters[index].mUnk_3b == 0) {
        return;
    }
    if (mCharacters[index].mCharacterId == 0x15) {
        ((CharacterModel *) mCharacters[index].mCharacter->mCharacter)->mParts[0]->mAnim->mLoop = 0;
    } else {
        ((ModelTransform *) mCharacters[index].mCharacter->mStoryCharacter)->mModel->mAnim->mLoop = 0;
    }
}

void ScriptPlayer::UpdateCharacterExtraAnims() {
    for (u8 i = 0; i < 2; i++) {
        s32 characterId = mCharacters[i].mCharacterId;
        if (characterId < 0) {
            continue;
        }
        if (characterId == mStorytalkWindow->mCharacterId) {
            if (mStorytalkWindow->GetPageTextTimer()) {
                SetCharacterExtraAnimEnabled(i, 1);
            } else {
                SetCharacterExtraAnimEnabled(i, 0);
            }
        } else {
            SetCharacterExtraAnimEnabled(i, 0);
        }
    }
}

s8 ScriptPlayer::func_020687a8(s32 kind, u32 index, u32 column) {
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

void ScriptPlayer::ApplyCharacterRotation(u32 index) {
    if (mCharacters[index].mCharacter == NULL) {
        return;
    }
    s16 rot[3];
    rot[0] = mCharacters[index].mRot[0];
    rot[1] = mCharacters[index].mRot[1];
    rot[2] = mCharacters[index].mRot[2];
    mCharacters[index].mCharacter->SetModelRotation((const u16 *) rot);
}

BOOL ScriptPlayer::SetMap(s32 value) {
    if (value == mMaps[0].mMapId) {
        return FALSE;
    }
    if (value != -2) {
        mMaps[0].mMapId = value;
    }
    ModelInstance *instance = mMaps[0].mModel;
    if (instance != NULL) {
        instance->mOwnFlags = MODEL_OWN_ALL;
        delete instance;
        mMaps[0].mModel = NULL;
    }
    if (mMaps[0].mMapId < 0) {
        return FALSE;
    }
    return TRUE;
}

BOOL ScriptPlayer::LoadMap() {
    char path[0x80];
    SpriteManager::Get().RestoreSlot(0);
    s32 index = mMaps[0].mMapId;
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
    model->Load(path, texture);
    texture->FreeFile();
    if ((id != 8) & (id != 9)) {
        sprintf(path, "%s_data.bin", data_020a22ac[id].mName);
        ModelAnimData *animData = new ModelAnimData(path);
        ModelAnim *anim         = new ModelAnim(animData, model);
        mMaps[0].mModel    = model->CreateInstance(anim);
        mMaps[0].mModel->StartAnimWithIncrement(0, 1, 0x1000);
    } else {
        mMaps[0].mModel = model->CreateInstance(NULL);
    }
    ChangeDir("/");
    ResetCamera();
    ResetLight();
    MI_CpuClear16(mMaps[0].mRot, 0x14);
    MI_CpuClear16(&mMaps[0], 0x18);
    mMaps[0].mX = 0;
    mMaps[0].mY = FX_FROM_INT_ROUND(data_020a22ac[id].mY);
    mMaps[0].mZ = FX_FROM_INT_ROUND(data_020a22ac[id].mZ);
    mMaps[0].mPosFrom[0] = 0;
    mMaps[0].mPosFrom[1] = data_020a22ac[id].mY;
    mMaps[0].mPosFrom[2] = data_020a22ac[id].mZ;
    mMaps[0].mPosTo[0] = mMaps[0].mPosFrom[0];
    mMaps[0].mPosTo[1] = mMaps[0].mPosFrom[1];
    mMaps[0].mPosTo[2] = mMaps[0].mPosFrom[2];
    mMaps[0].mUnk_32 = 0;
    return TRUE;
}


BOOL ScriptPlayer::func_02068c54(u32 index, s32 value) {
    if (value == mModels[index].mModelId) {
        return FALSE;
    }
    if (value != -2) {
        mModels[index].mModelId = value;
    }
    if (index == 0) {
        UnkStruct_0205401c *model = mCharacters[1].mCharacter;
        if (model != NULL) {
            model->Release();
            delete model;
        }
        mCharacters[1].mCharacter = NULL;
    } else {
        BakuganModel *object = mSpheres[1].mSphere;
        if (object != NULL) {
            delete object;
        }
        mSpheres[1].mSphere = NULL;
    }
    ReleaseModel(index);
    if (mModels[index].mModelId < 0) {
        return FALSE;
    }
    return TRUE;
}

void ScriptPlayer::LoadRealbakuPalettes(TextureFile *texture, u32 id) {
    if ((id == 0x1d) | (id == 0x1c)) {
        char name[0x40];
        char file[0x40];
        ChangeDir("/Game/Realbaku/Tex");
        u8 i;
        const char *prefix = data_020a2554[id].mName;
        u8 variant         = gSaveData[0].mAttribute;
        for (i = 0; i < 11; i++) {
            sprintf(name, "%s_%02d", prefix, i);
            sprintf(file, "%s_%02d_%02d", prefix, i, variant);
            texture->LoadPaletteFile(name, file, 0);
        }
        ChangeDir("/");
    }
}

BOOL ScriptPlayer::LoadModel(u32 index) {
    char path[0x80];
    if (index == 0) {
        SpriteManager::Get().RestoreSlot(1);
    } else {
        SpriteManager::Get().RestoreSlot(6);
    }
    s32 value = mModels[index].mModelId;
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
    LoadRealbakuPalettes(texture, id);
    ChangeDir(data_020b431c[dir]);
    sprintf(path, "%s.nsbmd", data_020a2554[id].mName);
    model = new NitroModel;
    model->Load(path, texture);
    if (dir != 1) {
        texture->FreeFile();
    }
    if (data_020a2554[id].mData != 0) {
        sprintf(path, "%s_data.bin", data_020a2554[id].mName);
        ModelAnimData *animData = new ModelAnimData(path);
        ModelAnim *anim         = new ModelAnim(animData, model);
        mModels[index].mModel = model->CreateInstance(anim);
        mModels[index].mModel->StartAnimWithIncrement(0, 1, 0x1000);
    } else {
        mModels[index].mModel = model->CreateInstance(NULL);
    }
    ChangeDir("/");
    MI_CpuClear16(&mModels[index], 0x18);
    MI_CpuClear16(mModels[index].mRot, 0x14);
    mModels[index].mHideAtAnimEnd = 0;
    mModels[index].mVisible = 0;
    mModels[index].mScale = data_020a2554[id].mUnk_05;
    mModels[index].mStorytalkCell2X = 0x80;
    mModels[index].mStorytalkCell2Frame = 0;
    return TRUE;
}

void ScriptPlayer::StartNextModelLoad(u32 index, u32 value) {
    mModels[index].mNextModelId = value;
    mModels[index].mNextAnimIndex = 0;
    for (u8 j = 0; j < 3; j++) {
        if (mModels[index].mNextFiles[j] != NULL) {
            OS_Free(mModels[index].mNextFiles[j]);
            mModels[index].mNextFiles[j] = NULL;
        }
    }
    mModelLoadStep = 0;
}

BOOL ScriptPlayer::LoadNextModelFiles(u32 index) {
    char path[0x80];
    u8 id = mModels[index].mNextModelId;
    if (mModelLoadStep != 0) {
        if (IsAsyncLoadActive() != 0) {
            return FALSE;
        }
        mModels[index].mNextFiles[mModelLoadStep - 1] = GetAsyncLoadBuffer();
        if (mModelLoadStep == 2 && data_020a2554[id].mData == 0) {
            mModelLoadStep = 3;
        }
    }
    if (mModelLoadStep < 3) {
        ChangeDir("/");
        switch (mModelLoadStep) {
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
        LoadFileAsync(path, 0, 0, 0, 0, 0);
        mModelLoadStep++;
        return FALSE;
    }
    return TRUE;
}

void ScriptPlayer::SwapModel(u32 index) {
    ReleaseModel(index);
    if (index == 0) {
        SpriteManager::Get().RestoreSlot(1);
    } else {
        SpriteManager::Get().RestoreSlot(6);
    }
    u8 id = mModels[index].mNextModelId;
    TextureFile *texture = new NitroTextureData(mModels[index].mNextFiles[0]);
    LoadRealbakuPalettes(texture, id);
    NitroModel *model = new NitroModel;
    model->LoadData(mModels[index].mNextFiles[1], texture);
    if (data_020a2554[id].mDir != 1) {
        texture->FreeFile();
    }
    if (data_020a2554[id].mData != 0) {
        ModelAnimData *animData = new ModelAnimData(mModels[index].mNextFiles[2]);
        ModelAnim *anim         = new ModelAnim(animData, model);
        mModels[index].mModel = model->CreateInstance(anim);
        mModels[index].mModel->StartAnimWithIncrement(0, 1, 0x1000);
    } else {
        mModels[index].mModel = model->CreateInstance(NULL);
    }
    for (u8 j = 0; j < 3; j++) {
        mModels[index].mFiles[j] = mModels[index].mNextFiles[j];
        mModels[index].mNextFiles[j] = NULL;
    }
    mModels[index].mModel->StartAnimWithIncrement(mModels[index].mNextAnimIndex, 1, 0x1000);
    mModels[index].mModelId = mModels[index].mNextModelId;
    mModels[index].mNextModelId = -1;
    mModels[index].mHideAtAnimEnd = 0;
    mModels[index].mScale = data_020a2554[id].mUnk_05;
}

void ScriptPlayer::ReleaseModel(u32 index) {
    ModelInstance *instance = mModels[index].mModel;
    if (instance != NULL) {
        instance->mOwnFlags = MODEL_OWN_ALL;
        delete instance;
        mModels[index].mModel = NULL;
    }
    for (u8 j = 0; j < 3; j++) {
        if (mModels[index].mFiles[j] != NULL) {
            OS_Free(mModels[index].mFiles[j]);
            mModels[index].mFiles[j] = NULL;
        }
    }
}

BOOL ScriptPlayer::func_02069680(u32 index) {
    if (index == 1) {
        ReleaseModel(1);
    }
    BakuganModel *object = mSpheres[index].mSphere;
    if (object != NULL) {
        delete object;
    }
    mSpheres[index].mSphere = NULL;
    if (mSpheres[index].mTableIndex < 0) {
        return FALSE;
    }
    return TRUE;
}

void ScriptPlayer::CreateNextSphere(u32 index) {
    BakuganModel *object = mSpheres[index].mNextSphere;
    if (object != NULL) {
        delete object;
    }
    mSpheres[index].mNextSphere = NULL;
    s32 kind = mSpheres[index].mNextTableIndex;
    if (kind < 0) {
        return;
    }
    mSpheres[index].mNextSphere = new BakuganModel();
    u8 value = GetSphereVariant(kind);
    mSpheres[index].mNextSphere->StoreSetup(gScriptSphereEntries[kind].mBakuganId, value, 5);
}

u8 ScriptPlayer::GetSphereVariant(u32 index) {
    switch (gScriptSphereEntries[index].mBakuganId) {
        case 0x24:
        case 0x25:
            return gSaveData[0].mAttribute;
    }
    return gScriptSphereEntries[index].mVariant;
}

extern "C" u16 FX_Atan2Idx(fx32 x, fx32 y);

static inline fx32 SinIdx(int idx) {
    return FX_SinCosTable_[(idx >> 4) * 2];
}
static inline fx32 CosIdx(int idx) {
    return FX_SinCosTable_[(idx >> 4) * 2 + 1];
}

// The mode passed to Setup is an enum local: as `u32 mode = c ? 5 : 1;` it colours differently (r6/r7 swapped).
enum UnkStruct_02065c40_ModelMode { UNK_02065C40_MODEL_MODE_1 = 1, UNK_02065C40_MODEL_MODE_5 = 5 };

BOOL ScriptPlayer::LoadSphere(u32 index, u32 flag) {
    if (index == 0) {
        SpriteManager::Get().RestoreSlot(4);
    } else {
        SpriteManager::Get().RestoreSlot(5);
    }
    if (flag) {
        if (!mSpheres[index].mNextSphere->func_0203d6f0()) {
            return FALSE;
        }
    } else {
        s32 kind = mSpheres[index].mTableIndex;
        UnkStruct_02065c40_ModelMode mode;
        if (mSpheres[index].mUnk_36 != 0) {
            mode = UNK_02065C40_MODEL_MODE_5;
        } else {
            mode = UNK_02065C40_MODEL_MODE_1;
        }
        ChangeDir("/");
        mSpheres[index].mSphere = new BakuganModel();
        u8 value = GetSphereVariant(kind);
        mSpheres[index].mSphere->Setup(gScriptSphereEntries[kind].mBakuganId, value, (u8) mode);
        mSpheres[index].mVisible = 0;
        MI_CpuClear16(&mSpheres[index], 0x18);
        MI_CpuClear16(mSpheres[index].mRot, 0x14);
        ResetSphereParams(index);
    }
    return TRUE;
}

void ScriptPlayer::ResetSphereParams(u32 index) {
    mSpheres[index].mStorytalkCell2X = 0x80;
    mSpheres[index].mUnk_39 = 0x80;
    mSpheres[index].mUnk_3a = 0x80;
    mSpheres[index].mStorytalkCell2Frame = 0;
    mSpheres[index].mUnk_3c = 0;
    mSpheres[index].mUnk_3d = 0;
    mSpheres[index].mUnk_3e = 0;
    mSpheres[index].mShadowY = 0;
    mSpheres[index].mShadowScale = 10;
    if (mSpheres[index].mUnk_36 == 0) {
        return;
    }
    mSpheres[index].mSphere->mAnimUpdateEnabled = 1;
    mSpheres[index].mSphere->mModel->StartAnimWithIncrement(0, 0, 0x64000);
}

void ScriptPlayer::SwapSphere(u32 index) {
    if (mSpheres[index].mNextSphere == NULL) {
        return;
    }
    BakuganModel *object = mSpheres[index].mSphere;
    if (object != NULL) {
        delete object;
    }
    mSpheres[index].mSphere = mSpheres[index].mNextSphere;
    mSpheres[index].mNextSphere = NULL;
    mSpheres[index].mTableIndex = mSpheres[index].mNextTableIndex;
    mSpheres[index].mUnk_36 = 1;
    if (index == 0) {
        SpriteManager::Get().RestoreSlot(4);
    } else {
        SpriteManager::Get().RestoreSlot(5);
    }
    mSpheres[index].mSphere->RunStoredSetup();
    ResetSphereParams(index);
}

void ScriptPlayer::ApplySphereRotation(u32 index) {
    if (mSpheres[index].mSphere == NULL) {
        return;
    }
    u16 rot[3];
    rot[0] = mSpheres[index].mRot[0];
    rot[1] = mSpheres[index].mRot[1];
    rot[2] = mSpheres[index].mRot[2];
    mSpheres[index].mSphere->func_0203db94(rot);
}

void ScriptPlayer::ResetCamera() {
    for (u8 i = 0; i < 2; i++) {
        MI_CpuClear16(&mCameraPoints[i], 0x18);
        MI_CpuClear16(&mCameraAngles, 0x14);
    }
    mCameraPoints[1].mZ = -0x64000;
    mFov[0] = 0x3c;
    mFov[1] = 0x3c;
    mFov[2] = 0x3c;
    mCameraMode    = SCRIPT_CAMERA_MODE_TARGET;
    ApplyFov();
}

extern "C" void MTX_SetTranslation43(MtxFx43 *mtx, VecFx32 *pos);

void ScriptPlayer::ResetLight() {
    MI_CpuClear16(&mLightDir, 0x14);
    u32 v = 0xfffff23d;
    mLightDir.x = -0x452;
    mLightDir.y = -0x6e1;
    mLightDir.z = v;
    mLightDirFrom.x = mLightDir.x;
    mLightDirFrom.y = mLightDir.y;
    mLightDirFrom.z = mLightDir.z;
    mLightDirTo.x = mLightDir.x;
    mLightDirTo.y = mLightDir.y;
    mLightDirTo.z = mLightDir.z;
    mLightColor = v >> 17;
    if (mLights == NULL) {
        return;
    }
    mLights->mLights[0]->mColor = mLightColor;
    mLights->ApplyAll();
}

void ScriptPlayer::ApplyFov() {
    u32 fov = mFov[0];
    mCamera->SetFov((u32) (65536.0f * fov / 360.0f));
}

void ScriptPlayer::UpdateCameraAndLight() {
    if (mCameraMode == SCRIPT_CAMERA_MODE_ANGLES) {
        mCameraPoints[1].mX = mCameraPoints[0].mX + SinIdx(mCameraAngles.mYaw) * 100;
        mCameraPoints[1].mZ = mCameraPoints[0].mZ - CosIdx(mCameraAngles.mYaw) * 100;
        mCameraPoints[1].mY = mCameraPoints[0].mY + SinIdx(mCameraAngles.mPitch) * 100;
    }
    fx32 z = mCameraPoints[0].mZ;
    fx32 y = mCameraPoints[0].mY;
    fx32 x = mCameraPoints[0].mX;
    if ((z == mCameraPoints[1].mZ) & ((x == mCameraPoints[1].mX) & (y == mCameraPoints[1].mY))) {
        return;
    }
    mCamera->SetPosition(x, y, z);
    mCamera->SetTarget(mCameraPoints[1].mX, mCameraPoints[1].mY, mCameraPoints[1].mZ);
    if (mLights == NULL) {
        return;
    }
    s16 dir[3];
    s16 dx = mLightDir.x;
    s16 dz = mLightDir.z;
    s16 dy = mLightDir.y;
    dir[0] = dx;
    dir[1] = dy;
    dir[2] = dz;
    mLights->mLights[0]->SetDirection(dir);
    mLights->ApplyAll();
}

void ScriptPlayer::SetCameraMode(u32 mode) {
    if (mCameraMode == mode) {
        return;
    }
    if (mode != SCRIPT_CAMERA_MODE_ANGLES) {
        mCameraPoints[1].mX = mCameraPoints[0].mX + SinIdx(mCameraAngles.mYaw) * 100;
        mCameraPoints[1].mZ = mCameraPoints[0].mZ - CosIdx(mCameraAngles.mYaw) * 100;
        mCameraPoints[1].mXFrom = mCameraPoints[1].mX >> 12;
        mCameraPoints[1].mZFrom = mCameraPoints[1].mZ >> 12;
        mCameraPoints[1].mXTo = mCameraPoints[1].mXFrom;
        mCameraPoints[1].mZTo = mCameraPoints[1].mZFrom;
        mCameraPoints[1].mY = mCameraPoints[0].mY + SinIdx(mCameraAngles.mPitch) * 100;
    } else {
        mCameraAngles.mYaw = FX_Atan2Idx(mCameraPoints[1].mX - mCameraPoints[0].mX, -(mCameraPoints[1].mZ - mCameraPoints[0].mZ));
        VecFx32 delta = {0, 0, 0};
        delta.x = mCameraPoints[1].mX - mCameraPoints[0].mX;
        delta.z = mCameraPoints[1].mZ - mCameraPoints[0].mZ;
        mCameraAngles.mPitch = FX_Atan2Idx(mCameraPoints[1].mY - mCameraPoints[0].mY, VEC_Mag(&delta));
        mCameraAngles.mYawFrom = mCameraAngles.mUnk_02s;
    }
    mCameraMode = mode;
}

void ScriptPlayer::func_0206a01c() {
    if ((mUnk_14e4 == 0) & (mUnk_14e8 | mUnk_14f1) && mCutsceneCellAnim != NULL) {
        mCutsceneCellAnim->Update();
        mCutsceneCellAnim->Draw(1, 0x80, 0x60, 0);
    }
    if (mUnk_14e4 != 0 && mUnk_14e5 != 0) {
        return;
    }
    u32 first  = 0;
    u32 second = 0;
    s32 current = mStorytalkWindow->mCharacterId;
    if (current >= 0) {
        if (current == 0x1a) {
            second = 0x80;
        } else {
            for (u8 i = 0; i < 2; i++) {
                if (mCharacters[i].mCharacter != NULL && current == mCharacters[i].mCharacterId) {
                    second = mCharacters[i].mStorytalkCell2X;
                    first  = mCharacters[i].mStorytalkCell2Frame;
                    break;
                }
                if (mSpheres[i].mSphere != NULL) {
                    s32 kind = mSpheres[i].mTableIndex;
                    if (kind >= 0 && current == gScriptSphereEntries[kind].mCharacterId) {
                        second = mSpheres[i].mStorytalkCell2X;
                        first  = mSpheres[i].mStorytalkCell2Frame;
                        break;
                    }
                }
                if (mModels[i].mModel != NULL) {
                    BOOL match = FALSE;
                    if (current == 0x16) {
                        s32 v = mModels[i].mModelId;
                        switch (v) {
                            case 0x1c:
                            case 0x1d:
                                match = TRUE;
                        }
                    } else if (current == 0x17) {
                        s32 v = mModels[i].mModelId;
                        switch (v) {
                            case 0x1e:
                            case 0x1f:
                                match = TRUE;
                        }
                    }
                    if (match) {
                        second = mModels[i].mStorytalkCell2X;
                        first  = mModels[i].mStorytalkCell2Frame;
                        break;
                    }
                }
            }
        }
    }
    mStorytalkWindow->SetCell2FrameAndX(first, second);
}

void ScriptPlayer::SetCutsceneAnimation(u32 value, u32 force) {
    if (force == 0 && mCutsceneAnimIndex == value) {
        return;
    }
    if (mCutsceneCellAnim == NULL) {
        return;
    }
    mCutsceneCellAnim->SetAnimation(mCutsceneAnimIndex = value);
}

void ScriptPlayer::UpdateMessages() {
    if ((mUnk_14ed == 0) & ((mUnk_12ba == 0) & (mSkipped == 0))) {
        u8 state = mState;
        if (((state >= 3) & (state <= 4)) && mWaitKind != SCRIPT_WAIT_KIND_LOAD) {
            if (InputManager::Get().IsPressed(8)) {
                SkipScript();
                return;
            }
        }
    }
    switch (mMessageState) {
        case 0:
            SetCutsceneAnimation(0, 0);
            if (mMessagePos >= mMessageCount) {
                return;
            }
            u32 flag = 0;
            if (mUnk_14e2 == 5) {
                flag = 1;
            }
            mStorytalkWindow->SetMessage(mMessageIndices[mMessagePos], mUnk_13d2[mMessagePos], flag, mUnk_14ee);
            mMessageState = 1;
            break;
        case 1:
            if (!mStorytalkWindow->IsTextShown()) {
                return;
            }
            if (mUnk_12ba == 0) {
                mMessageState = 3;
            } else if (mUnk_12ba == 3) {
                mStorytalkWindow->mKeepLastPage = 1;
                u8 r      = mStorytalkWindow->HasMorePages();
                mUnk_14f1 = r;
                if (r) {
                    mStorytalkWindow->SetCell1Visible(1);
                    mMessageState = 4;
                } else {
                    mStorytalkWindow->SetCell1Visible(0);
                    mMessageState = 6;
                    mState = 9;
                }
            } else {
                mMessageState = 5;
                mState = 9;
            }
            SetCutsceneAnimation(2, 0);
            break;
        case 2:
            if (mUnk_12ba == 3) {
                if (mStorytalkWindow->HasMorePages()) {
                    SetCutsceneAnimation(2, 0);
                    mUnk_14f1 = 1;
                    mMessageState = 4;
                } else {
                    mMessageState = 6;
                    mUnk_14f1 = 0;
                    mState = 9;
                }
            } else {
                mMessageState = 5;
            }
            break;
        case 5:
            if (!mStorytalkWindow->HasMorePages()) {
                return;
            }
            mMessageState = 7;
            mUnk_12d0 = 200;
            mStorytalkWindow->SetCell1Visible(0);
            break;
        case 7:
            if (mUnk_12d0 != 0) {
                mUnk_12d0--;
                return;
            }
            if (!mStorytalkWindow->NextPage(1)) {
                mStorytalkWindow->mMessageIndex = -1;
                mStorytalkWindow->SetMessage(mMessageIndices[0], mUnk_13d2[0], 0, 0);
                mUnk_12d0 = 200;
            } else {
                if (mStorytalkWindow->HasMorePages()) {
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
            if (TouchManager::Get().mPressed != 0) {
                mUnk_14ef = 1;
            }
            if (mUnk_14ef != 0) {
                if (TouchManager::Get().mDown == 0) {
                    touched = TRUE;
                } else {
                    SetCutsceneAnimation(1, 1);
                }
            }
            if (!InputManager::Get().IsPressed(1) && !touched) {
                return;
            }
            mUnk_14ef = 0;
            if (mUnk_14e8 | mUnk_14f1) {
                SetCutsceneAnimation(InputManager::Get().IsPressed(1) ? 3 : 1, 0);
            }
            mInputTimer = 4;
            if (mMessageState == 4) {
                mMessageState = 9;
            } else {
                mMessageState = 8;
            }
            if (mUnk_14ee != 0) {
                return;
            }
            SoundManager::Get().StartSeqArc(0x6b);
            break;
        }
        case 8:
        case 9:
            if (mInputTimer != 0) {
                mInputTimer--;
                return;
            }
            if (!mStorytalkWindow->NextPage(0)) {
                if (mMessageState == 9) {
                    mStorytalkWindow->SetCell1Visible(0);
                    mUnk_14f1 = 0;
                    mMessageState = 6;
                    mState = 9;
                } else {
                    mMessagePos++;
                    mMessageState = 0;
                }
                SetCutsceneAnimation(0, 0);
            } else {
                if (mMessageState == 9) {
                    mMessageState = 4;
                } else {
                    mMessageState = 3;
                }
                SetCutsceneAnimation(2, 0);
            }
            break;
    }
}

extern const s16 data_020a248c[][9];
extern const s16 data_020a233c[][12];
void AddHBlankHook(void (*callback)());
void RemoveHBlankHook(void (*callback)());
extern "C" void Bg0WaveCallback();
void ReplaceStorytalkMessages(StorytalkWindow *object, void *data);
void SetNoStorytalkMessage(StorytalkWindow *object);

void ScriptPlayer::DrawSpheres() {
    for (u8 i = 0; i < 2; i++) {
        if (mSpheres[i].mSphere != NULL && mSpheres[i].mVisible != 0) {
            VecFx32 pos;
            fx32 z = mSpheres[i].mZ;
            fx32 y = mSpheres[i].mY;
            fx32 x = mSpheres[i].mX;
            pos.z = z / 2 - 0x72000;
            pos.y = y / 2;
            pos.x = x / 2;
            if (mSpheres[i].mUnk_3c != 0) {
                if (mSpheres[i].mUnk_3e < 1) {
                    mSpheres[i].mUnk_3e++;
                }
            } else {
                if (mSpheres[i].mUnk_3e != 0) {
                    mSpheres[i].mUnk_3e--;
                }
            }
            if (mSpheres[i].mUnk_3e != 0) {
                mSpheres[i].mUnk_3d = (mSpheres[i].mUnk_3d + 1) % 0x80;
                pos.y += mSpheres[i].mUnk_3e * SinIdx((u16) (mSpheres[i].mUnk_3d * 0x10000 / 0x80));
            }
            mSpheres[i].mSphere->func_0203db70(&pos);
            ApplySphereRotation(i);
            mSpheres[i].mSphere->Update();
            mSpheres[i].mSphere->DrawModel();
            u8 shadow = mSpheres[i].mShadowScale;
            if (shadow != 0 && mShadowModel != NULL) {
                s32 scale = shadow * 0x19a;
                if (scale > 0) {
                    pos.y = (mSpheres[i].mShadowY << 12) - 0x64000;
                    MtxFx43 mtx = gIdentityMtx43;
                    MTX_SetTranslation43(&mtx, &pos);
                    VecFx32 size;
                    size.x = scale;
                    size.y = scale;
                    size.z = scale;
                    mShadowModel->Draw(&mtx, &size);
                }
            }
        }
    }
}

void ScriptPlayer::DrawCharacters() {
    for (u8 i = 0; i < 2; i++) {
        if (mCharacters[i].mCharacter != NULL) {
            s32 z = mCharacters[i].mZ / 2 - 0x80000;
            s32 h = mCharacters[i].mOffsetY / 2;
            VecFx32 pos;
            s32 round = FX_FROM_INT_ROUND(h);
            s32 y = mCharacters[i].mY;
            s32 x = mCharacters[i].mX;
            y = y / 2 - 0x64000;
            pos.y = y + round;
            pos.x = x / 2;
            pos.z = z;
            if (mCharacters[i].mCharacterId == 0x15) {
                ModelTransform *transform = (ModelTransform *) mCharacters[i].mCharacter->mCharacter;
                transform->mPosition.x    = pos.x;
                transform->mPosition.y    = pos.y;
                transform->mPosition.z    = pos.z;
            } else {
                ModelTransform *transform = (ModelTransform *) mCharacters[i].mCharacter->mStoryCharacter;
                transform->mPosition.x    = pos.x;
                transform->mPosition.y    = pos.y;
                transform->mPosition.z    = pos.z;
            }
            if (mCharacters[i].mVisible != 0) {
                ApplyCharacterRotation(i);
                mCharacters[i].mCharacter->Draw();
                UpdateCharacterAnimation(i);
                u8 shadow = mCharacters[i].mShadowScale;
                if (shadow != 0 && mShadowModel != NULL) {
                    MtxFx43 mtx = gIdentityMtx43;
                    s32 scale   = shadow * 0x19a;
                    VecFx32 size;
                    size.x = scale;
                    size.y = scale;
                    size.z = scale;
                    pos.y += 0x1000;
                    MTX_SetTranslation43(&mtx, &pos);
                    mShadowModel->Draw(&mtx, &size);
                }
            }
        }
    }
}

void ScriptPlayer::UpdateCharacterAnimation(u32 index) {
    if (mCharacters[index].mUnk_3b == 0) {
        return;
    }
    s32 characterId = mCharacters[index].mCharacterId;
    ModelAnim *anim;
    if (characterId == 0x15) {
        anim = ((CharacterModel *) mCharacters[index].mCharacter->mCharacter)->mParts[0]->mAnim;
    } else {
        anim = ((ModelTransform *) mCharacters[index].mCharacter->mStoryCharacter)->mModel->mAnim;
    }
    if (anim->mReachedEnd == 0) {
        return;
    }
    u8 cur = mCharacters[index].mUnk_39;
    u8 target = mCharacters[index].mUnk_3a;
    s32 value;
    BOOL loop;
    if (cur != target) {
        loop = FALSE;
        if (cur == 0) {
            value = func_020687a8((u8) characterId, target, 1);
            mCharacters[index].mUnk_39 = target;
        } else {
            value = func_020687a8((u8) characterId, cur, 2);
            mCharacters[index].mUnk_39 = 0;
        }
        if (value < 0) {
            value = func_020687a8((u8) mCharacters[index].mCharacterId, target, 0);
            mCharacters[index].mUnk_39 = target;
            mCharacters[index].mUnk_3b = 0;
            if (func_020687a8((u8) mCharacters[index].mCharacterId, target, 4) != 0) {
                loop = FALSE;
            } else {
                loop = TRUE;
            }
        }
    } else {
        value = func_020687a8((u8) characterId, cur, 0);
        mCharacters[index].mUnk_3b = 0;
        loop = TRUE;
    }
    s8 other = func_020687a8((u8) mCharacters[index].mCharacterId, target, 3);
    mCharacters[index].mCharacter->func_02054644((u8) other, (u8) value);
    mCharacters[index].mExtraAnimEnabled = 0;
    anim->mLoop = loop;
}

void ScriptPlayer::DrawMap() {
    if (mMaps[0].mModel == NULL) {
        return;
    }
    MtxFx43 mtx = gIdentityMtx43;
    VecFx32 size;
    size.x = 0x1000;
    size.y = 0x1000;
    size.z = 0x1000;
    VecFx32 pos;
    fx32 x = mMaps[0].mX;
    fx32 z = mMaps[0].mZ;
    fx32 y = mMaps[0].mY;
    pos.x = x;
    pos.y = y;
    pos.z = z;
    if (mMaps[0].mRot[0] != 0) {
        MTX_ApplyRotX43(&mtx, (u16) mMaps[0].mRot[0]);
    }
    if (mMaps[0].mRot[1] != 0) {
        MTX_ApplyRotY43(&mtx, (u16) mMaps[0].mRot[1]);
    }
    if (mMaps[0].mRot[2] != 0) {
        MTX_ApplyRotZ43(&mtx, (u16) mMaps[0].mRot[2]);
    }
    MTX_SetTranslation43(&mtx, &pos);
    if (mMaps[0].mUnk_32 != 0) {
        mMaps[0].mModel->mExtraAnim->mFrame = 0;
    } else {
        mMaps[0].mModel->AdvanceAnim();
    }
    mMaps[0].mModel->AdvanceExtraAnim();
    mMaps[0].mModel->Draw(&mtx, &size);
}

void ScriptPlayer::DrawModels() {
    for (u8 i = 0; i < 2; i++) {
        if (mModels[i].mModel != NULL && mModels[i].mVisible != 0) {
            MtxFx43 mtx = gIdentityMtx43;
            s32 scale   = FX_FROM_INT_ROUND(mModels[i].mScale);
            VecFx32 size;
            size.x = scale;
            size.y = scale;
            size.z = scale;
            VecFx32 pos;
            pos.x = mModels[i].mX;
            pos.y = mModels[i].mY;
            pos.z = mModels[i].mZ;
            MTX_SetTranslation43(&mtx, &pos);
            if (mModels[i].mRot[0] != 0) {
                MTX_ApplyRotX43(&mtx, (u16) mModels[i].mRot[0]);
            }
            if (mModels[i].mRot[1] != 0) {
                MTX_ApplyRotY43(&mtx, (u16) mModels[i].mRot[1]);
            }
            if (mModels[i].mRot[2] != 0) {
                MTX_ApplyRotZ43(&mtx, (u16) mModels[i].mRot[2]);
            }
            mModels[i].mModel->AdvanceAnim();
            mModels[i].mModel->Draw(&mtx, &size);
            if (mModels[i].mHideAtAnimEnd != 0) {
                if (mModels[i].mModel->IsAnimFinished() != 0) {
                    mModels[i].mVisible = 0;
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

UnkStruct_02065c40_Kind ScriptPlayer::GetCommandSlot(u32 id) {
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



void ScriptPlayer::RunCommands() {
    mState = 3;
    mWaitKind = SCRIPT_WAIT_KIND_NONE;
    while (TRUE) {
        if (mCommandIndex >= mCommandCount) {
            if (mMessagePos != mMessageCount) {
                return;
            }
            func_02066814(1);
            mState = 5;
            ReleaseModels(0);
            return;
        }
        ExecuteCommand(mCommands[mCommandIndex].mCommand, mCommands[mCommandIndex].mValue);
        if (mWaitKind == SCRIPT_WAIT_KIND_NONE) {
            mCommandIndex++;
        } else {
            mState = 4;
            return;
        }
    }
}

void SetStorytalkBlink(StorytalkWindow *self, u8 unk1);

void ScriptPlayer::ExecuteCommand(u32 command, s32 value) {
    mCurrentCommand = command;
    mCurrentValue = value;
    s16 angle = (mCurrentValue << 16) / 360;
    u32 idx   = GetCommandSlot(mCurrentCommand);
    mWaitKind = SCRIPT_WAIT_KIND_NONE;
    switch (mCurrentCommand) {
        case 1:
            mMessageIndices[mMessageCount] = mCurrentValue;
            mMessageCount++;
            if (mMessageCount >= 0x80) {
                mMessageCount = 0;
            }
            break;
        case 3:
            SetNoStorytalkMessage(mStorytalkWindow);
            mMessageState = 0;
            mMessagePos = mMessageCount;
            mWaitTimer = 0;
            mWaitKind = SCRIPT_WAIT_KIND_TIMER;
            break;
        case 8:
            mUnk_13d2[mMessageCount] = mCurrentValue;
            break;
        case 2:
            mWaitKind = SCRIPT_WAIT_KIND_MESSAGES;
            break;
        case 9:
            {
                s32 limit = mCurrentValue;
                if (limit > 3) {
                    limit = 3;
                }
                mUnk_12ba = limit;
                break;
            }
        case 10:
            mUnk_14ee = mCurrentValue;
            break;
        case 11:
            mStorytalkWindow->mTextOnlyX = mCurrentValue;
            break;
        case 12:
            mStorytalkWindow->mTextOnlyY = mCurrentValue;
            break;
        case 13:
            mWaitTimer = mCurrentValue;
            mWaitKind = SCRIPT_WAIT_KIND_TIMER;
            break;
        case 14:
            {
                s32 v = mCurrentValue;
                if (v < 1) {
                    v = 1;
                }
                mFrameCount = v;
                break;
            }
        case 15:
            {
                s32 v = mCurrentValue;
                if (v > 0x10) {
                    v = 0x10;
                } else if (v < -0x10) {
                    v = -0x10;
                }
                mBrightnessTo = v;
                mBrightnessFrom = mBrightness;
                break;
            }
        case 25:
        case 26:
        case 27:
            mBrightnessFrom = mBrightness;
            mBrightnessFrames = 0;
            mBrightnessFrameCount = mFrameCount;
            mUnk_14d4 = 0;
            switch (mCurrentCommand) {
                case 0x1a:
                    mFadeKind = 1;
                    if (mBrightnessTo < 0) {
                        SpriteManager::Get().StartSubFade(FADE_MODE_DOWN);
                    } else {
                        SpriteManager::Get().StartSubFade(FADE_MODE_TO_ZERO);
                    }
                    mBrightnessFrameCount = 0x10;
                    break;
                case 0x1b:
                    mFadeKind = 2;
                    if (mBrightnessTo < 0) {
                        SpriteManager::Get().StartFade(FADE_MODE_DOWN);
                    } else {
                        SpriteManager::Get().StartFade(FADE_MODE_TO_ZERO);
                    }
                    mBrightnessFrameCount = 0x10;
                    break;
                default:
                    mFadeKind = SCRIPT_FADE_KIND_BLEND;
                    break;
            }
            mWaitKind = SCRIPT_WAIT_KIND_BRIGHTNESS;
            break;
        case 17:
            SetBg0Wave(1);
        case 16:
            mLerpFrames = 0;
            mLerpFrameCount = mFrameCount;
            mWaitKind = SCRIPT_WAIT_KIND_LERP;
            break;
        case 18:
        case 19:
        case 20:
        case 21:
        case 22:
        case 23:
            switch (mCurrentCommand) {
                case 0x12:
                    SetCharacterVisible(0, 1);
                    break;
                case 0x13:
                    SetCharacterVisible(1, 1);
                    break;
                case 0x15:
                    SetCharacterVisible(0, 0);
                    break;
                case 0x16:
                    SetCharacterVisible(1, 0);
                    break;
                case 0x17:
                    break;
            }
            break;
        case 28:
            mCameraPoints[0].mX = FX_FROM_INT_ROUND(mCurrentValue);
            mCameraPoints[0].mXFrom = mCurrentValue;
        case 31:
            mCameraPoints[0].mXTo = mCurrentValue;
            break;
        case 29:
            mCameraPoints[0].mY = FX_FROM_INT_ROUND(mCurrentValue);
            mCameraPoints[0].mYFrom = mCurrentValue;
        case 32:
            mCameraPoints[0].mYTo = mCurrentValue;
            break;
        case 30:
            mCameraPoints[0].mZ = FX_FROM_INT_ROUND(mCurrentValue);
            mCameraPoints[0].mZFrom = mCurrentValue;
        case 33:
            mCameraPoints[0].mZTo = mCurrentValue;
            break;
        case 34:
            mCameraPoints[1].mX = FX_FROM_INT_ROUND(mCurrentValue);
            mCameraPoints[1].mXFrom = mCurrentValue;
        case 37:
            mCameraPoints[1].mXTo = mCurrentValue;
            SetCameraMode(SCRIPT_CAMERA_MODE_TARGET);
            break;
        case 35:
            mCameraPoints[1].mY = FX_FROM_INT_ROUND(mCurrentValue);
            mCameraPoints[1].mYFrom = mCurrentValue;
        case 38:
            mCameraPoints[1].mYTo = mCurrentValue;
            SetCameraMode(SCRIPT_CAMERA_MODE_TARGET);
            break;
        case 36:
            mCameraPoints[1].mZ = FX_FROM_INT_ROUND(mCurrentValue);
            mCameraPoints[1].mZFrom = mCurrentValue;
        case 39:
            mCameraPoints[1].mZTo = mCurrentValue;
            SetCameraMode(SCRIPT_CAMERA_MODE_TARGET);
            break;
        case 40:
            mCameraAngles.mVec[0].x = angle;
            mCameraAngles.mVec[1].x = angle;
        case 41:
            mCameraAngles.mVec[2].x = angle;
            SetCameraMode(SCRIPT_CAMERA_MODE_ANGLES);
            break;
        case 42:
            mCameraAngles.mVec[0].y = angle;
            mCameraAngles.mVec[1].y = angle;
        case 43:
            mCameraAngles.mVec[2].y = angle;
            SetCameraMode(SCRIPT_CAMERA_MODE_ANGLES);
            break;
        case 44:
            mCameraAngles.mVec[0].z = angle;
            mCameraAngles.mVec[1].z = angle;
        case 45:
            mCameraAngles.mVec[2].z = angle;
            SetCameraMode(SCRIPT_CAMERA_MODE_ANGLES);
            break;
        case 46:
            mFov[0] = 0x3c;
            mFov[1] = 0x3c;
            mFov[2] = 0x3c;
            ApplyFov();
            break;
        case 47:
            mFov[0] = mCurrentValue;
            mFov[1] = mCurrentValue;
            ApplyFov();
        case 48:
            mFov[2] = mCurrentValue;
            break;
        case 49:
            mLightDir.x = mCurrentValue;
            mLightDirFrom.x = mCurrentValue;
        case 50:
            mLightDirTo.x = mCurrentValue;
            break;
        case 51:
            mLightDir.y = mCurrentValue;
            mLightDirFrom.y = mCurrentValue;
        case 52:
            mLightDirTo.y = mCurrentValue;
            break;
        case 53:
            mLightDir.z = mCurrentValue;
            mLightDirFrom.z = mCurrentValue;
        case 54:
            mLightDirTo.z = mCurrentValue;
            break;
        case 55:
            mLightColor = (mLightColor & 0xffe0) | (mCurrentValue & 0x1f);
            break;
        case 56:
            mLightColor = (mLightColor & 0xfc1f) | ((mCurrentValue & 0x1f) << 5);
            break;
        case 57:
            mLightColor = (mLightColor & 0x83ff) | ((mCurrentValue & 0x1f) << 10);
            break;
        case 58:
            mLights->mLights[0]->mColor = mLightColor;
            mLights->ApplyAll();
            break;
        case 169:
        case 191:
            if (func_02068c54(idx, mCurrentValue)) {
                mWaitTimer = 0;
                mWaitKind = SCRIPT_WAIT_KIND_LOAD;
            }
            break;
        case 170:
        case 192:
            StartNextModelLoad(idx, mCurrentValue);
            mWaitTimer = 0;
            mWaitKind = SCRIPT_WAIT_KIND_LOAD;
            break;
        case 171:
        case 193:
            mModels[idx].mSwapPending = 1;
            break;
        case 188:
        case 210:
            if (mModels[idx].mSwapPending != 0) {
                mModels[idx].mNextAnimIndex = mCurrentValue;
                break;
            }
            if (mModels[idx].mModel == NULL) {
                break;
            }
            mModels[idx].mModel->StartAnimWithIncrement((u8) mCurrentValue, 0, 0x1000);
            mModels[idx].mHideAtAnimEnd = 1;
            mModels[idx].mVisible = 1;
            break;
        case 189:
        case 211:
            if (mModels[idx].mSwapPending != 0) {
                mModels[idx].mNextAnimIndex = mCurrentValue;
                break;
            }
            if (mModels[idx].mModel == NULL) {
                break;
            }
            mModels[idx].mModel->StartAnimWithIncrement((u8) mCurrentValue, 1, 0x1000);
            mModels[idx].mHideAtAnimEnd = 0;
            mModels[idx].mVisible = 1;
            break;
        case 172:
        case 194:
            mModels[idx].mVisible = 1;
            break;
        case 173:
        case 195:
            mModels[idx].mVisible = 0;
            break;
        case 174:
            mModels[0].mX = FX_FROM_INT_ROUND(mCurrentValue);
            mModels[0].mPosFrom[0] = mCurrentValue;
        case 177:
            mModels[0].mPosTo[0] = mCurrentValue;
            break;
        case 175:
            mModels[0].mY = FX_FROM_INT_ROUND(mCurrentValue);
            mModels[0].mPosFrom[1] = mCurrentValue;
        case 178:
            mModels[0].mPosTo[1] = mCurrentValue;
            break;
        case 176:
            mModels[0].mZ = FX_FROM_INT_ROUND(mCurrentValue);
            mModels[0].mPosFrom[2] = mCurrentValue;
        case 179:
            mModels[0].mPosTo[2] = mCurrentValue;
            break;
        case 180:
            mModels[0].mRot[0] = angle;
            mModels[0].mRotFrom[0] = angle;
        case 183:
            mModels[0].mRotTo[0] = angle;
            break;
        case 181:
            mModels[0].mRot[1] = angle;
            mModels[0].mRotFrom[1] = angle;
        case 184:
            mModels[0].mRotTo[1] = angle;
            break;
        case 182:
            mModels[0].mRot[2] = angle;
            mModels[0].mRotFrom[2] = angle;
        case 185:
            mModels[0].mRotTo[2] = angle;
            break;
        case 196:
            mModels[1].mX = FX_FROM_INT_ROUND(mCurrentValue);
            mModels[1].mPosFrom[0] = mCurrentValue;
        case 199:
            mModels[1].mPosTo[0] = mCurrentValue;
            break;
        case 197:
            mModels[1].mY = FX_FROM_INT_ROUND(mCurrentValue);
            mModels[1].mPosFrom[1] = mCurrentValue;
        case 200:
            mModels[1].mPosTo[1] = mCurrentValue;
            break;
        case 198:
            mModels[1].mZ = FX_FROM_INT_ROUND(mCurrentValue);
            mModels[1].mPosFrom[2] = mCurrentValue;
        case 201:
            mModels[1].mPosTo[2] = mCurrentValue;
            break;
        case 202:
            mModels[1].mRot[0] = angle;
            mModels[1].mRotFrom[0] = angle;
        case 205:
            mModels[1].mRotTo[0] = angle;
            break;
        case 203:
            mModels[1].mRot[1] = angle;
            mModels[1].mRotFrom[1] = angle;
        case 206:
            mModels[1].mRotTo[1] = angle;
            break;
        case 204:
            mModels[1].mRot[2] = angle;
            mModels[1].mRotFrom[2] = angle;
        case 207:
            mModels[1].mRotTo[2] = angle;
            break;
        case 186:
        case 208:
            mModels[idx].mStorytalkCell2X = mCurrentValue;
            break;
        case 187:
        case 209:
            mModels[idx].mStorytalkCell2Frame = mCurrentValue;
            break;
        case 59:
            if (SetMap(mCurrentValue)) {
                mWaitTimer = 0;
                mWaitKind = SCRIPT_WAIT_KIND_LOAD;
            }
            break;
        case 61:
            mMaps[0].mX = FX_FROM_INT_ROUND(mCurrentValue);
            mMaps[0].mPosFrom[0] = mCurrentValue;
        case 64:
            mMaps[0].mPosTo[0] = mCurrentValue;
            break;
        case 62:
            mMaps[0].mY = FX_FROM_INT_ROUND(mCurrentValue);
            mMaps[0].mPosFrom[1] = mCurrentValue;
        case 65:
            mMaps[0].mPosTo[1] = mCurrentValue;
            break;
        case 63:
            mMaps[0].mZ = FX_FROM_INT_ROUND(mCurrentValue);
            mMaps[0].mPosFrom[2] = mCurrentValue;
        case 66:
            mMaps[0].mPosTo[2] = mCurrentValue;
            break;
        case 67:
            mMaps[0].mRot[0] = angle;
            mMaps[0].mRotFrom[0] = angle;
        case 70:
            mMaps[0].mRotTo[0] = angle;
            break;
        case 68:
            mMaps[0].mRot[1] = angle;
            mMaps[0].mRotFrom[1] = angle;
        case 71:
            mMaps[0].mRotTo[1] = angle;
            break;
        case 69:
            mMaps[0].mRot[2] = angle;
            mMaps[0].mRotFrom[2] = angle;
        case 72:
            mMaps[0].mRotTo[2] = angle;
            break;
        case 80:
            mCharacters[0].mX = FX_FROM_INT_ROUND(mCurrentValue);
            mCharacters[0].mPosFrom[0] = mCurrentValue;
        case 83:
            mCharacters[0].mPosTo[0] = mCurrentValue;
            break;
        case 103:
            mCharacters[1].mX = FX_FROM_INT_ROUND(mCurrentValue);
            mCharacters[1].mPosFrom[0] = mCurrentValue;
        case 106:
            mCharacters[1].mPosTo[0] = mCurrentValue;
            break;
        case 81:
            mCharacters[0].mY = FX_FROM_INT_ROUND(mCurrentValue);
            mCharacters[0].mPosFrom[1] = mCurrentValue;
        case 84:
            mCharacters[0].mPosTo[1] = mCurrentValue;
            break;
        case 104:
            mCharacters[1].mY = FX_FROM_INT_ROUND(mCurrentValue);
            mCharacters[1].mPosFrom[1] = mCurrentValue;
        case 107:
            mCharacters[1].mPosTo[1] = mCurrentValue;
            break;
        case 82:
            mCharacters[0].mZ = FX_FROM_INT_ROUND(mCurrentValue);
            mCharacters[0].mPosFrom[2] = mCurrentValue;
        case 85:
            mCharacters[0].mPosTo[2] = mCurrentValue;
            break;
        case 105:
            mCharacters[1].mZ = FX_FROM_INT_ROUND(mCurrentValue);
            mCharacters[1].mPosFrom[2] = mCurrentValue;
        case 108:
            mCharacters[1].mPosTo[2] = mCurrentValue;
            break;
        case 73:
        case 96:
            {
                UnkStruct_0205401c *model = mCharacters[idx].mCharacter;
                if (model != NULL) {
                    model->Release();
                    delete model;
                }
                mCharacters[idx].mCharacter = NULL;
                s32 v = mCurrentValue;
                if (v == -2) {
                    v = mCharacters[idx].mCharacterId;
                }
                if (v >= 0) {
                    mCharacters[idx].mCharacterId = v;
                }
                break;
            }
        case 75:
        case 98:
            {
                UnkStruct_0205401c *model = mCharacters[idx].mNextCharacter;
                if (model != NULL) {
                    model->Release();
                    delete model;
                }
                mCharacters[idx].mNextCharacter = NULL;
                mCharacters[idx].mNextCharacterId = mCurrentValue;
                break;
            }
        case 76:
        case 99:
            CreateNextCharacter(idx);
            mCharacters[idx].mUnk_38 = mCurrentValue;
            mWaitTimer = 0;
            mWaitKind = SCRIPT_WAIT_KIND_LOAD;
            break;
        case 74:
        case 97:
            mWaitTimer = 0;
            mWaitKind = 5;
            break;
        case 77:
        case 100:
            mCharacters[idx].mSwapPending = 1;
            break;
        case 87:
            mCharacters[0].mRot[1] = angle;
            mCharacters[0].mRotFrom[1] = angle;
        case 90:
            mCharacters[0].mRotTo[1] = angle;
            break;
        case 110:
            mCharacters[1].mRot[1] = angle;
            mCharacters[1].mRotFrom[1] = angle;
        case 113:
            mCharacters[1].mRotTo[1] = angle;
            break;
        case 78:
        case 101:
            SetCharacterVisible(idx, 1);
            break;
        case 79:
        case 102:
            SetCharacterVisible(idx, 0);
            break;
        case 92:
        case 115:
            mCharacters[idx].mStorytalkCell2X = mCurrentValue;
            mCharacters[idx].mUnk_40 = mCurrentValue;
            mCharacters[idx].mUnk_41 = mCurrentValue;
            break;
        case 93:
            mCharacters[idx].mUnk_40 = mCurrentValue;
            break;
        case 94:
        case 117:
            mCharacters[idx].mStorytalkCell2Frame = mCurrentValue;
            break;
        case 95:
        case 118:
            mCharacters[idx].mShadowScale = mCurrentValue;
            break;
        case 119:
        case 120:
        case 144:
        case 145:
            {
                mSpheres[idx].mVisible = 0;
                s32 v = mCurrentValue;
                if (v != -2) {
                    mSpheres[idx].mTableIndex = v;
                    switch (mCurrentCommand) {
                        case 0x78:
                        case 0x91:
                            mSpheres[idx].mUnk_36 = 1;
                            break;
                        default:
                            mSpheres[idx].mUnk_36 = 0;
                            break;
                    }
                }
                BakuganModel *pose = mSpheres[idx].mSphere;
                if (pose != NULL) {
                    delete pose;
                }
                mSpheres[idx].mSphere = NULL;
                if (func_02069680(idx)) {
                    mWaitTimer = 0;
                    mWaitKind = SCRIPT_WAIT_KIND_LOAD;
                }
                break;
            }
        case 121:
        case 146:
            {
                BakuganModel *pose = mSpheres[idx].mNextSphere;
                if (pose != NULL) {
                    delete pose;
                }
                mSpheres[idx].mNextSphere = NULL;
                mSpheres[idx].mNextTableIndex = mCurrentValue;
                CreateNextSphere(idx);
                mWaitTimer = 0;
                mWaitKind = SCRIPT_WAIT_KIND_LOAD;
                break;
            }
        case 122:
        case 147:
            mSpheres[idx].mSwapPending = 1;
            break;
        case 123:
        case 148:
            if (mSpheres[idx].mSphere != NULL) {
                mSpheres[idx].mVisible = 1;
            }
            break;
        case 124:
        case 149:
            mSpheres[idx].mVisible = 0;
            break;
        case 125:
        case 150:
            mSpheres[idx].mX = FX_FROM_INT_ROUND(mCurrentValue);
            mSpheres[idx].mPosFrom[0] = mCurrentValue;
            mSpheres[idx].mPosTo[0] = mCurrentValue;
            break;
        case 128:
        case 153:
            mSpheres[idx].mPosTo[0] = mCurrentValue;
            break;
        case 126:
        case 151:
            mSpheres[idx].mY = FX_FROM_INT_ROUND(mCurrentValue);
            mSpheres[idx].mPosFrom[1] = mCurrentValue;
            mSpheres[idx].mPosTo[1] = mCurrentValue;
            break;
        case 129:
        case 154:
            mSpheres[idx].mPosTo[1] = mCurrentValue;
            break;
        case 127:
        case 152:
            mSpheres[idx].mZ = FX_FROM_INT_ROUND(mCurrentValue);
            mSpheres[idx].mPosFrom[2] = mCurrentValue;
            mSpheres[idx].mPosTo[2] = mCurrentValue;
            break;
        case 130:
        case 155:
            mSpheres[idx].mPosTo[2] = mCurrentValue;
            break;
        case 131:
            mSpheres[0].mRot[0] = angle;
            mSpheres[0].mRotFrom[0] = angle;
        case 134:
            mSpheres[0].mRotTo[0] = angle;
            break;
        case 156:
            mSpheres[1].mRot[0] = angle;
            mSpheres[1].mRotFrom[0] = angle;
        case 159:
            mSpheres[1].mRotTo[0] = angle;
            break;
        case 132:
            mSpheres[0].mRot[1] = angle;
            mSpheres[0].mRotFrom[1] = angle;
        case 135:
            mSpheres[0].mRotTo[1] = angle;
            break;
        case 157:
            mSpheres[1].mRot[1] = angle;
            mSpheres[1].mRotFrom[1] = angle;
        case 160:
            mSpheres[1].mRotTo[1] = angle;
            break;
        case 133:
            mSpheres[0].mRot[2] = angle;
            mSpheres[0].mRotFrom[2] = angle;
        case 136:
            mSpheres[0].mRotTo[2] = angle;
            break;
        case 158:
            mSpheres[1].mRot[2] = angle;
            mSpheres[1].mRotFrom[2] = angle;
        case 161:
            mSpheres[1].mRotTo[2] = angle;
            break;
        case 137:
            mSpheres[0].mStorytalkCell2X = mCurrentValue;
            mSpheres[0].mUnk_3a = mCurrentValue;
        case 138:
            mSpheres[0].mUnk_39 = mCurrentValue;
            break;
        case 162:
            mSpheres[1].mStorytalkCell2X = mCurrentValue;
            mSpheres[1].mUnk_3a = mCurrentValue;
        case 163:
            mSpheres[1].mUnk_39 = mCurrentValue;
            break;
        case 139:
        case 164:
            mSpheres[idx].mStorytalkCell2Frame = mCurrentValue;
            break;
        case 140:
        case 165:
            mSpheres[idx].mUnk_3c = mCurrentValue;
            break;
        case 141:
        case 166:
            if (mSpheres[idx].mUnk_36 == 0) {
                break;
            }
            switch (mCurrentValue) {
                case 1:
                    mSpheres[idx].mSphere->mModel->StartAnimWithIncrement(0, 0, 0x1000);
                    break;
                case 3:
                    mSpheres[idx].mSphere->mModel->StartAnimWithIncrement(0, 0, 0x64000);
                    break;
                case 4:
                    mSpheres[idx].mSphere->mModel->StartAnimWithIncrement(0, 0, -1);
                    break;
                case 2:
                    mSpheres[idx].mSphere->mModel->StartAnimWithIncrement(0, 0, 0x64000);
                    mSpheres[idx].mSphere->mModel->AdvanceAnim();
                    mSpheres[idx].mSphere->mModel->mAnim->mFramePerFrame = -0x1000;
                    break;
            }
            break;
        case 142:
        case 167:
            mSpheres[idx].mShadowScale = mCurrentValue;
            break;
        case 143:
        case 168:
            mSpheres[idx].mShadowY = mCurrentValue;
            break;
        case 213:
            if (mUnk_14d2 == mCurrentValue) {
                break;
            }
            mUnk_14d2 = mCurrentValue;
            if (mUnk_14d2 > -1) {
                FrameTask::Get().StartChannel(0, mUnk_14d2);
            } else {
                FrameTask::Get().StopChannel(0);
            }
            break;
        case 214:
            FrameTask::Get().StopChannel(0);
            break;
        case 215:
            if (mCurrentValue > -1) {
                FrameTask::Get().StartChannel(1, (u16) (mCurrentValue + 0xa8));
            } else {
                FrameTask::Get().StopChannel(1);
            }
            break;
        case 216:
            FrameTask::Get().StopChannel(1);
            break;
        case 217:
            {
                if (mCurrentValue <= -1) {
                    break;
                }
                s32 id = (u16) mCurrentValue;
                if (id < 0x11) {
                    SoundManager::Get().StartSeqArc(id + 0x12);
                } else {
                    SoundManager::Get().StartSeqArc((u16) (id - 0x11) + 0x44);
                }
                break;
            }
        case 218:
            SoundManager::Get().StopSeqArcPlayerSounds(1);
            break;
        case 219:
            data_020d433c.mPlayerCount = 2;
            break;
        case 220:
            {
                s32 v = mCurrentValue;
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
                s32 v = mCurrentValue;
                if (v < 0) {
                    v = -v;
                }
                data_020d433c.mFieldId = v % 8;
                data_020d433c.mUnk_96 = 0;
                break;
            }
        case 222:
            {
                s32 v = mCurrentValue;
                data_020d433c.mCharacterIds[1] = v % 22;
                break;
            }
        case 223:
            {
                s32 v = mCurrentValue;
                data_020d433c.mCharacterIds[2] = v % 22;
                break;
            }
        case 224:
            {
                s32 v = mCurrentValue;
                data_020d433c.mCharacterIds[3] = v % 22;
                break;
            }
        case 225:
            StopWithResult(SCRIPT_RESULT_MYROOM);
            break;
        case 226:
            StopWithResult(SCRIPT_RESULT_STORE);
            break;
        case 227:
            StopWithResult(SCRIPT_RESULT_PARK);
            break;
        case 228:
            StopWithResult(SCRIPT_RESULT_TOURNAMENT);
            break;
        case 229:
            StopWithResult(SCRIPT_RESULT_TUTORIAL);
            break;
        case 230:
            StopWithResult(8);
            break;
        case 231:
            StopWithResult(SCRIPT_RESULT_BATTLE);
            {
                s32 v = mCurrentValue;
                if (v > 4) {
                    v = 4;
                } else if (v < 2) {
                    v = 2;
                }
                data_020d433c.mPlayerCount = v;
            }
            break;
        case 232:
            CameraManager::Get().mCameraIndex = 0;
            CameraManager::Get().SetCamerasAndRequestMode(1, mCamera, 0);
            mUnk_14e4 = 0;
            SetStorytalkBlink(mStorytalkWindow, 0);
            mWaitKind = SCRIPT_WAIT_KIND_LOAD_GRAPHICS;
            break;
        case 233:
            ReleaseModels(1);
            SpriteManager::Get().SetupMode(1, 7);
            SetupSpriteSlots();
            CameraManager::Get().SetCamerasAndRequestMode(2, mCamera, (u32) mCamera);
            SpriteManager::Get().SetFade(FADE_MODE_HOLD, -16);
            mUnk_14e4 = 1;
            mUnk_14e5 = 1;
            GXS_SetVisiblePlane(1);
            SetStorytalkBlink(mStorytalkWindow, 1);
            mStorytalkWindow->LoadGraphics();
            break;
        case 234:
            mUnk_14e7 = 1;
            mWaitKind = 8;
            mUnk_14e8 = 0;
            mUnk_14f1 = 0;
            break;
    }
}

// FX32_CONST with an integer scale: the original converts after the shift.
#define FXC(x) ((fx32) (((x) > 0) ? ((x) * 4096 + 0.5f) : ((x) * 4096 - 0.5f)))
#define LERP_FX(dst, a, b) do { s32 av = (a); s32 v = av + (mLerpFrames * ((b) - av)) / mLerpFrameCount; dst = FXC(v); } while (0)
#define LERP_S(dst, a, b) dst = (a) + (mLerpFrames * (s16) ((b) - (a))) / mLerpFrameCount
#define LERP_N(dst, a, b) dst = (a) + (mLerpFrames * ((b) - (a))) / mLerpFrameCount

void ScriptPlayer::UpdateWait() {
    BOOL done = FALSE;
    switch (mWaitKind) {
        case SCRIPT_WAIT_KIND_MESSAGES:
            if (mMessagePos == mMessageCount) {
                done = TRUE;
            }
            break;
        case SCRIPT_WAIT_KIND_BRIGHTNESS:
            if (mUnk_14d4 == 0) {
                if (mBrightnessFrameCount == 0) {
                    done = TRUE;
                }
            } else {
                done = TRUE;
            }
            break;
        case 5: {
            u32 idx = GetCommandSlot(mCurrentCommand);
            s32 value = mCurrentValue;
            if (value == -2) {
                value = mCharacters[idx].mUnk_3a;
                SetCharacterVisible(idx, 0);
                UnkStruct_0205401c *model = mCharacters[idx].mCharacter;
                if (model != NULL) {
                    model->Release();
                    delete model;
                }
                mCharacters[idx].mCharacter = NULL;
            }
            if (func_02067f30(idx, value) == FALSE) {
                done = TRUE;
            } else {
                mWaitTimer = 0;
                mWaitKind = SCRIPT_WAIT_KIND_LOAD;
            }
            break;
        }
        case SCRIPT_WAIT_KIND_LOAD:
            if (mWaitTimer != 0) {
                mWaitTimer--;
                break;
            }
            switch (mCurrentCommand) {
                case 0x4a:
                case 0x61:
                    done = LoadCharacter(GetCommandSlot(mCurrentCommand), 0);
                    break;
                case 0x4c:
                case 0x63:
                    done = LoadCharacter(GetCommandSlot(mCurrentCommand), 1);
                    break;
                case 0x3b:
                    done = LoadMap();
                    break;
                case 0xa9:
                case 0xbf:
                    done = LoadModel(GetCommandSlot(mCurrentCommand));
                    break;
                case 0xaa:
                case 0xc0:
                    done = LoadNextModelFiles(GetCommandSlot(mCurrentCommand));
                    break;
                case 0x77:
                case 0x78:
                case 0x90:
                case 0x91:
                    done = LoadSphere(GetCommandSlot(mCurrentCommand), 0);
                    break;
                case 0x79:
                case 0x92:
                    done = LoadSphere(GetCommandSlot(mCurrentCommand), 1);
                    break;
            }
            break;
        case SCRIPT_WAIT_KIND_TIMER:
            if (mWaitTimer != 0) {
                mWaitTimer--;
            } else {
                done = TRUE;
            }
            break;
        case SCRIPT_WAIT_KIND_LERP: {
            int n = mLerpFrames + 1;
            if (n > mLerpFrameCount) {
                n = mLerpFrameCount;
            }
            mLerpFrames = n;
            for (u8 i = 0; i < 2; i++) {
                LERP_FX(mCharacters[i].mX, mCharacters[i].mPosFrom[0], mCharacters[i].mPosTo[0]);
                LERP_FX(mCharacters[i].mY, mCharacters[i].mPosFrom[1], mCharacters[i].mPosTo[1]);
                LERP_FX(mCharacters[i].mZ, mCharacters[i].mPosFrom[2], mCharacters[i].mPosTo[2]);
                LERP_FX(mSpheres[i].mX, mSpheres[i].mPosFrom[0], mSpheres[i].mPosTo[0]);
                LERP_FX(mSpheres[i].mY, mSpheres[i].mPosFrom[1], mSpheres[i].mPosTo[1]);
                LERP_FX(mSpheres[i].mZ, mSpheres[i].mPosFrom[2], mSpheres[i].mPosTo[2]);
                LERP_FX(mModels[i].mX, mModels[i].mPosFrom[0], mModels[i].mPosTo[0]);
                LERP_FX(mModels[i].mY, mModels[i].mPosFrom[1], mModels[i].mPosTo[1]);
                LERP_FX(mModels[i].mZ, mModels[i].mPosFrom[2], mModels[i].mPosTo[2]);
                LERP_N(mCharacters[i].mStorytalkCell2X, mCharacters[i].mUnk_41, mCharacters[i].mUnk_40);
                LERP_N(mSpheres[i].mStorytalkCell2X, mSpheres[i].mUnk_3a, mSpheres[i].mUnk_39);
                LERP_S(mCharacters[i].mRot[0], mCharacters[i].mRotFrom[0], mCharacters[i].mRotTo[0]);
                LERP_S(mCharacters[i].mRot[1], mCharacters[i].mRotFrom[1], mCharacters[i].mRotTo[1]);
                LERP_S(mCharacters[i].mRot[2], mCharacters[i].mRotFrom[2], mCharacters[i].mRotTo[2]);
                LERP_S(mSpheres[i].mRot[0], mSpheres[i].mRotFrom[0], mSpheres[i].mRotTo[0]);
                LERP_S(mSpheres[i].mRot[1], mSpheres[i].mRotFrom[1], mSpheres[i].mRotTo[1]);
                LERP_S(mSpheres[i].mRot[2], mSpheres[i].mRotFrom[2], mSpheres[i].mRotTo[2]);
                LERP_S(mModels[i].mRot[0], mModels[i].mRotFrom[0], mModels[i].mRotTo[0]);
                LERP_S(mModels[i].mRot[1], mModels[i].mRotFrom[1], mModels[i].mRotTo[1]);
                LERP_S(mModels[i].mRot[2], mModels[i].mRotFrom[2], mModels[i].mRotTo[2]);
                {
                    fx32 q = FXC(mLerpFrames) / mLerpFrameCount;
                    s16 a = mCameraPoints[i].mXFrom;
                    mCameraPoints[i].mX = FX_Mul(FXC(mCameraPoints[i].mXTo - a), q) + FXC(a);
                }
                {
                    fx32 q = FXC(mLerpFrames) / mLerpFrameCount;
                    s16 a = mCameraPoints[i].mYFrom;
                    mCameraPoints[i].mY = FX_Mul(FXC(mCameraPoints[i].mYTo - a), q) + FXC(a);
                }
                {
                    fx32 q = FXC(mLerpFrames) / mLerpFrameCount;
                    s16 a = mCameraPoints[i].mZFrom;
                    mCameraPoints[i].mZ = FX_Mul(FXC(mCameraPoints[i].mZTo - a), q) + FXC(a);
                }
            }
            LERP_S(mCameraAngles.mVec[0].x, mCameraAngles.mVec[1].x, mCameraAngles.mVec[2].x);
            LERP_S(mCameraAngles.mVec[0].y, mCameraAngles.mVec[1].y, mCameraAngles.mVec[2].y);
            LERP_S(mCameraAngles.mVec[0].z, mCameraAngles.mVec[1].z, mCameraAngles.mVec[2].z);
            LERP_N(mFov[0], mFov[1], mFov[2]);
            ApplyFov();
            LERP_S(mLightDir.x, mLightDirFrom.x, mLightDirTo.x);
            LERP_S(mLightDir.y, mLightDirFrom.y, mLightDirTo.y);
            LERP_S(mLightDir.z, mLightDirFrom.z, mLightDirTo.z);
            {
                UnkStruct_02065c40_Kind j = UNK_02065C40_KIND_0;
                LERP_FX(mMaps[j].mX, mMaps[j].mPosFrom[0], mMaps[j].mPosTo[0]);
                LERP_FX(mMaps[j].mY, mMaps[j].mPosFrom[1], mMaps[j].mPosTo[1]);
                LERP_FX(mMaps[j].mZ, mMaps[j].mPosFrom[2], mMaps[j].mPosTo[2]);
                LERP_N(mMaps[j].mRot[0], mMaps[j].mRotFrom[0], mMaps[j].mRotTo[0]);
                LERP_N(mMaps[j].mRot[1], mMaps[j].mRotFrom[1], mMaps[j].mRotTo[1]);
                LERP_N(mMaps[j].mRot[2], mMaps[j].mRotFrom[2], mMaps[j].mRotTo[2]);
            }
            {
                s8 a = mBrightnessFrom;
                s8 d = mBrightnessTo - a;
                int v = a + (d * mLerpFrames) / mLerpFrameCount;
                if (v > 16) {
                    v = 16;
                } else if (v < -16) {
                    v = -16;
                }
                mBrightness = v;
            }
            if (mLerpFrames >= mLerpFrameCount) {
                done = TRUE;
                for (u8 i = 0; i < 2; i++) {
                    mCharacters[i].mX = FXC(mCharacters[i].mPosTo[0]);
                    mCharacters[i].mY = FXC(mCharacters[i].mPosTo[1]);
                    mCharacters[i].mZ = FXC(mCharacters[i].mPosTo[2]);
                    mCharacters[i].mPosFrom[0] = mCharacters[i].mPosTo[0];
                    mCharacters[i].mPosFrom[1] = mCharacters[i].mPosTo[1];
                    mCharacters[i].mPosFrom[2] = mCharacters[i].mPosTo[2];
                    mCharacters[i].mRot[0] = mCharacters[i].mRotTo[0];
                    mCharacters[i].mRot[1] = mCharacters[i].mRotTo[1];
                    mCharacters[i].mRot[2] = mCharacters[i].mRotTo[2];
                    mCharacters[i].mRotFrom[0] = mCharacters[i].mRotTo[0];
                    mCharacters[i].mRotFrom[1] = mCharacters[i].mRotTo[1];
                    mCharacters[i].mRotFrom[2] = mCharacters[i].mRotTo[2];
                    mMaps[i].mX = FXC(mMaps[i].mPosTo[0]);
                    mMaps[i].mY = FXC(mMaps[i].mPosTo[1]);
                    mMaps[i].mZ = FXC(mMaps[i].mPosTo[2]);
                    mMaps[i].mPosFrom[0] = mMaps[i].mPosTo[0];
                    mMaps[i].mPosFrom[1] = mMaps[i].mPosTo[1];
                    mMaps[i].mPosFrom[2] = mMaps[i].mPosTo[2];
                    mMaps[i].mRot[0] = mMaps[i].mRotTo[0];
                    mMaps[i].mRot[1] = mMaps[i].mRotTo[1];
                    mMaps[i].mRot[2] = mMaps[i].mRotTo[2];
                    mMaps[i].mRotFrom[0] = mMaps[i].mRotTo[0];
                    mMaps[i].mRotFrom[1] = mMaps[i].mRotTo[1];
                    mMaps[i].mRotFrom[2] = mMaps[i].mRotTo[2];
                    mSpheres[i].mX = FXC(mSpheres[i].mPosTo[0]);
                    mSpheres[i].mY = FXC(mSpheres[i].mPosTo[1]);
                    mSpheres[i].mZ = FXC(mSpheres[i].mPosTo[2]);
                    mSpheres[i].mPosFrom[0] = mSpheres[i].mPosTo[0];
                    mSpheres[i].mPosFrom[1] = mSpheres[i].mPosTo[1];
                    mSpheres[i].mPosFrom[2] = mSpheres[i].mPosTo[2];
                    mSpheres[i].mRot[0] = mSpheres[i].mRotTo[0];
                    mSpheres[i].mRot[1] = mSpheres[i].mRotTo[1];
                    mSpheres[i].mRot[2] = mSpheres[i].mRotTo[2];
                    mSpheres[i].mRotFrom[0] = mSpheres[i].mRotTo[0];
                    mSpheres[i].mRotFrom[1] = mSpheres[i].mRotTo[1];
                    mSpheres[i].mRotFrom[2] = mSpheres[i].mRotTo[2];
                    mModels[i].mX = FXC(mModels[i].mPosTo[0]);
                    mModels[i].mY = FXC(mModels[i].mPosTo[1]);
                    mModels[i].mZ = FXC(mModels[i].mPosTo[2]);
                    mModels[i].mPosFrom[0] = mModels[i].mPosTo[0];
                    mModels[i].mPosFrom[1] = mModels[i].mPosTo[1];
                    mModels[i].mPosFrom[2] = mModels[i].mPosTo[2];
                    mModels[i].mRot[0] = mModels[i].mRotTo[0];
                    mModels[i].mRot[1] = mModels[i].mRotTo[1];
                    mModels[i].mRot[2] = mModels[i].mRotTo[2];
                    mModels[i].mRotFrom[0] = mModels[i].mRotTo[0];
                    mModels[i].mRotFrom[1] = mModels[i].mRotTo[1];
                    mModels[i].mRotFrom[2] = mModels[i].mRotTo[2];
                    mCameraPoints[i].mX = FXC(mCameraPoints[i].mXTo);
                    mCameraPoints[i].mY = FXC(mCameraPoints[i].mYTo);
                    mCameraPoints[i].mZ = FXC(mCameraPoints[i].mZTo);
                    mCameraPoints[i].mXFrom = mCameraPoints[i].mXTo;
                    mCameraPoints[i].mYFrom = mCameraPoints[i].mYTo;
                    mCameraPoints[i].mZFrom = mCameraPoints[i].mZTo;
                }
                mCameraAngles.mVec[0].x = mCameraAngles.mVec[2].x;
                mCameraAngles.mVec[1].x = mCameraAngles.mVec[2].x;
                mCameraAngles.mVec[0].y = mCameraAngles.mVec[2].y;
                mCameraAngles.mVec[1].y = mCameraAngles.mVec[2].y;
                mCameraAngles.mVec[0].z = mCameraAngles.mVec[2].z;
                mCameraAngles.mVec[1].z = mCameraAngles.mVec[2].z;
                mFov[0] = mFov[2];
                mFov[1] = mFov[2];
                ApplyFov();
                mLightDirTo.x = mLightDir.x;
                mLightDirTo.y = mLightDir.y;
                mLightDirTo.z = mLightDir.z;
                mLightDirFrom.x = mLightDir.x;
                mLightDirFrom.y = mLightDir.y;
                mLightDirFrom.z = mLightDir.z;
                mBrightness = mBrightnessTo;
                mBrightnessFrom = mBrightnessTo;
                SetBg0Wave(0);
            }
            G2_SetBlendBrightness(0x21, mBrightness);
            break;
        }
        case SCRIPT_WAIT_KIND_STOP:
            mState = 4;
            break;
        case 8:
            if (mUnk_14e7 == 0) {
                mUnk_14e8 = 1;
                done = TRUE;
                mUnk_14f1 = 1;
            }
            break;
        case SCRIPT_WAIT_KIND_LOAD_GRAPHICS:
            LoadGraphics();
            mGraphicsLoaded = 1;
            mWaitKind = SCRIPT_WAIT_KIND_CLEAR_GRAPHICS_LOADED;
            break;
        case SCRIPT_WAIT_KIND_CLEAR_GRAPHICS_LOADED:
            if (mGraphicsLoaded == 0) {
                done = TRUE;
            }
            break;
        case SCRIPT_WAIT_KIND_NONE:
        default:
            done = TRUE;
            break;
    }
    if (done) {
        mState = 3;
        mCommandIndex++;
    }
}
#undef FXC
#undef LERP_FX
#undef LERP_S
#undef LERP_N

void ScriptPlayer::SetBg0Wave(u32 flag) {
    if (mBg0Wave == flag) {
        return;
    }
    mBg0Wave = flag;
    if (flag & 0xff) {
        AddHBlankHook(Bg0WaveCallback);
    } else {
        RemoveHBlankHook(Bg0WaveCallback);
        WriteBG0Offset(0);
    }
}

extern "C" void Bg0WaveCallback() {
    s32 line = *(vu16 *) 0x4000006;
    if (line >= 0xc0) {
        return;
    }
    u32 time = line + GameSystem::Get().mFrame;
    ScriptPlayer *event = gScriptPlayer;
    u16 angle = (time << 16) / 192;
    u16 frameCount = event->mLerpFrameCount;
    u16 frames     = event->mLerpFrames;
    u16 third      = frameCount / 3;
    u16 span;
    if (frames >= frameCount - third) {
        span = frameCount - frames;
    } else {
        span = frames;
        if (span > third) {
            span = third;
        }
    }
    s32 offset = span * (SinIdx(angle) << 5) / third;
    WriteBG0Offset((offset << 4) >> 16);
}



BOOL ScriptPlayer::StartScript(const char *path) {
    char file[0x80];
    if (mCommandIndex != mCommandCount) {
        return FALSE;
    }
    ChangeDir("/");
    sprintf(file, "/script/%s", path);
    LoadFileAsync(file, 0, 0, 0, &mFileSize, 1);
    ResetCamera();
    ResetLight();
    mMessageState = 0;
    mMessagePos = 0;
    mMessageCount = 0;
    SetNoStorytalkMessage(mStorytalkWindow);
    mUnk_12ba = 0;
    mSkipped = 0;
    SetCharacterVisible(0, 0);
    SetCharacterVisible(1, 0);
    ReleaseModels(0);
    for (u8 i = 0; i < 2; i++) {
        mCharacters[i].mExtraAnimEnabled = 0;
        mCharacters[i].mUnk_3b = 0;
        mCharacters[i].mOffsetY = 0;
        mCharacters[i].mShadowScale = 0x28;
        mCharacters[i].mStorytalkCell2X = 0x80;
        mCharacters[i].mUnk_40 = 0x80;
        mCharacters[i].mUnk_41 = 0x80;
        mCharacters[i].mStorytalkCell2Frame = 0;
        MI_CpuClear16(&mCharacters[i], 0x18);
        MI_CpuClear16(mCharacters[i].mRot, 0x14);
        mModels[i].mHideAtAnimEnd = 0;
        mModels[i].mVisible = 0;
        mModels[i].mScale = 1;
        mModels[i].mStorytalkCell2X = 0x80;
        mSpheres[i].mVisible = 0;
        mSpheres[i].mStorytalkCell2X = 0x80;
        mSpheres[i].mUnk_39 = 0x80;
        mSpheres[i].mUnk_3a = 0x80;
        mSpheres[i].mStorytalkCell2Frame = 0;
        mSpheres[i].mUnk_36 = 0;
        mSpheres[i].mUnk_3c = 0;
        mSpheres[i].mUnk_3d = 0;
        mSpheres[i].mUnk_3e = 0;
        mSpheres[i].mShadowY = 0;
        mSpheres[i].mShadowScale = 10;
        MI_CpuClear16(&mSpheres[i], 0x18);
        MI_CpuClear16(mSpheres[i].mRot, 0x14);
    }
    mState = 1;
    return TRUE;
}


BOOL ScriptPlayer::ReadScript() {
    if (IsAsyncLoadActive() != 0) {
        return FALSE;
    }
    mFileData = (u32) GetAsyncLoadBuffer();
    u8 *data = (u8 *) mFileData;
    u32 size = mFileSize;
    u8 *name = (u8 *) OS_Alloc(0x20);
    MI_CpuCopy8(data, name, 0x20);
    DC_FlushRange(name, 0x20);
    u8 *body = (u8 *) OS_Alloc(size - 0x20);
    MI_CpuCopy8(data + 0x20, body, size - 0x20);
    DC_FlushRange(body, size - 0x20);
    mCommandCount = (size - 0x20) / 3;
    if (mCommandCount >= 0x400) {
        mCommandCount = 0x400;
    }
    mCommandIndex = 0;
    u16 i = 0;
    do {
        u16 j = i * 3;
        mCommands[i].mCommand = body[j];
        u8 *q = body + j;
        mCommands[i].mValue = (q[1] << 24 >> 16) + q[2];
        i++;
    } while (i < mCommandCount);
    do {
        mCommands[i].mCommand = 0;
        mCommands[i].mValue   = 0;
        i++;
    } while (i < 0x400);
    ChangeDir("/");
    char file[0x80];
    sprintf(file, "/font/%s.mes", name);
    LoadFileAsync(file, 0, 0, 0, &mFileSize, 0);
    OS_Free(name);
    OS_Free(body);
    OS_Free(data);
    return TRUE;
}



BOOL ScriptPlayer::ReadMessages() {
    if (IsAsyncLoadActive() != 0) {
        return FALSE;
    }
    mFileData = (u32) GetAsyncLoadBuffer();
    ReplaceStorytalkMessages(mStorytalkWindow, (void *) mFileData);
    SetNoStorytalkMessage(mStorytalkWindow);
    return TRUE;
}






BOOL ScriptPlayer::func_0206ef4c(const char *name, u32 unk2, u32 unk3, u32 unk4, s32 unk5) {
    BOOL isTwo = mUnk_12ba == 2;
    BOOL isNine = mState == 9;
    BOOL done = mCommandIndex == mCommandCount;
    if (isTwo | (isNine & done)) {
    u8 mode = mUnk_14e2;
    if (mode >= 6) {
        mode = 0;
    }
    u8 n = 0;
    mCommands[n].mCommand = 9;
    mCommands[n].mValue = 2;
    n++;
    mCommands[n].mCommand = 0xe;
    mCommands[n].mValue = 1;
    n++;
    mCommands[n].mCommand = 0xf;
    mCommands[n].mValue = -16;
    n++;
    mCommands[n].mCommand = 0x19;
    mCommands[n].mValue = 0;
    n++;
    s16 extra = *(s16 *) &unk4;
    if (extra >= 0) {
        mCommands[n].mCommand = 0x3b;
        mCommands[n].mValue = extra;
        n++;
    }
    mCommands[n].mCommand = 0x49;
    mCommands[n].mValue = unk5;
    n++;
    if (unk5 >= 0) {
        mCommands[n].mCommand = 0x4a;
        mCommands[n].mValue = 0;
        n++;
        mCommands[n].mCommand = 0x50;
        mCommands[n].mValue = data_020a2198[mode].mUnk_0c;
        n++;
        mCommands[n].mCommand = 0x51;
        mCommands[n].mValue = data_020a2198[mode].mUnk_0e;
        n++;
        mCommands[n].mCommand = 0x52;
        mCommands[n].mValue = data_020a2198[mode].mUnk_10;
        n++;
        mCommands[n].mCommand = 0x57;
        mCommands[n].mValue = data_020a2198[mode].mUnk_12;
        n++;
        mCommands[n].mCommand = 0x5c;
        mCommands[n].mValue = data_020a2198[mode].mUnk_14;
        n++;
        mCommands[n].mCommand = 0x4e;
        mCommands[n].mValue = 0;
        n++;
    }
    mCommands[n].mCommand = 0x31;
    mCommands[n].mValue = data_020a1c04[mode].mUnk_0;
    n++;
    mCommands[n].mCommand = 0x33;
    mCommands[n].mValue = data_020a1c04[mode].mUnk_2;
    n++;
    mCommands[n].mCommand = 0x35;
    mCommands[n].mValue = data_020a1c04[mode].mUnk_4;
    n++;
    mCommands[n].mCommand = 0x3a;
    mLightColor = data_020a1c04[mode].mUnk_6;
    n++;
    mCommands[n].mCommand = 0x1c;
    mCommands[n].mValue = data_020a2198[mode].mUnk_00;
    n++;
    mCommands[n].mCommand = 0x1d;
    mCommands[n].mValue = data_020a2198[mode].mUnk_02;
    n++;
    mCommands[n].mCommand = 0x1e;
    mCommands[n].mValue = data_020a2198[mode].mUnk_04;
    n++;
    mCommands[n].mCommand = 0x22;
    mCommands[n].mValue = data_020a2198[mode].mUnk_06;
    n++;
    mCommands[n].mCommand = 0x23;
    mCommands[n].mValue = data_020a2198[mode].mUnk_08;
    n++;
    mCommands[n].mCommand = 0x24;
    mCommands[n].mValue = data_020a2198[mode].mUnk_0a;
    n++;
    mCommands[n].mCommand = 8;
    mCommands[n].mValue = unk5;
    n++;
    mCommands[n].mCommand = 0xe;
    mCommands[n].mValue = 10;
    n++;
    mCommands[n].mCommand = 0xf;
    mCommands[n].mValue = 0;
    n++;
    mCommands[n].mCommand = 0x19;
    mCommands[n].mValue = 0;
    n++;
    mUnk_12c0 = n;
    mCommands[n].mCommand = 0xd7;
    mCommands[n].mValue = func_02070554((s8) unk5, unk3);
    n++;
    mCommands[n].mCommand = 1;
    mCommands[n].mValue = unk2;
    n++;
    mUnk_12c2 = n;
    if (mUnk_14e2 == 0) {
        mCommands[n].mCommand = 0xa9;
        mCommands[n].mValue = data_020d433c.mMapTab + 0x20;
        n++;
        mCommands[n].mCommand = 0xae;
        mCommands[n].mValue = -0x13b;
        n++;
        mCommands[n].mCommand = 0xaf;
        mCommands[n].mValue = data_020a2198[mode].mUnk_0e;
        n++;
        mCommands[n].mCommand = 0xb0;
        mCommands[n].mValue = -0x834;
        n++;
        mCommands[n].mCommand = 0xb5;
        mCommands[n].mValue = 0x140;
        n++;
        mCommands[n].mCommand = 0xb2;
        mCommands[n].mValue = data_020a2198[mode].mUnk_02;
        n++;
        mCommands[n].mCommand = 0xac;
        n++;
        mCommands[n].mCommand = 0x10;
        mCommands[n].mValue = 0;
        n++;
    }
    mMaps[0].mUnk_32 = 0;
    mCommandIndex = 0;
    mCommandCount = n;
    ChangeDir("/");
    char file[0x80];
    sprintf(file, "/font/%s.mes", name);
    LoadFileAsync(file, 0, 0, 0, &mFileSize, 0);
    mStorytalkWindow->SetCell1Visible(0);
    mMessageState = 0;
    SetNoStorytalkMessage(mStorytalkWindow);
    mMessagePos = 0;
    mMessageCount = 0;
    mState = 2;
    mUnk_14e8 = 0;
    return TRUE;
    }
    return FALSE;
}

extern const u8 data_020a1b8a[];
extern "C" void MI_CpuCopy8(const void *src, void *dest, u32 size);
void SetNoStorytalkMessage(StorytalkWindow *object);

void ScriptPlayer::func_0206f564(s32 unk1, u32 unk2, s32 unk3) {
    BOOL isTwo   = mUnk_12ba == 2;
    BOOL isThree = mUnk_12ba == 3;
    if (!(isTwo | isThree)) {
        return;
    }
    if (mCommandIndex < mCommandCount - 1) {
        return;
    }
    if (!(((mMessageState == 6) | ((mMessageState == 5) | (mMessageState == 7))) & (unk1 != mStorytalkWindow->mMessageIndex))) {
        return;
    }
    if (mUnk_14e2 == 5) {
        mMessageState = 0;
        SetNoStorytalkMessage(mStorytalkWindow);
        mMessageIndices[0] = unk1;
        mCommandIndex    = mUnk_12c0;
        mCommands[mUnk_12c0].mValue = func_02070554(mCharacters[0].mCharacterId, unk2);
        mState = 3;
        return;
    }
    mStorytalkWindow->SetMessage((s16) unk1, mUnk_13d2[0], 0, 0);
    mMessageState = 2;
    if (mUnk_14e2 != 0) {
        return;
    }
    if (unk3 < 0) {
        return;
    }
    u8 value = unk3 + 0x20;
    if (value == mCommands[mUnk_12c2].mValue) {
        return;
    }
    mCommands[mUnk_12c2].mValue = value;
    mCommandIndex                 = mUnk_12c2;
    mCommands[mUnk_12c0].mValue = -1;
    mState                 = 3;
}

// The four computed entries of the script below (FxScale, Identity16) are filled at run time on the first call, under a
// guard at 0x0221928c (right after gScriptPlayer).
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
ScriptCommand gMessageScriptCommands[17] = {
    {1, 0xe}, {-16, 0xf}, {0, 0x19}, {3, 9}, {9, 0x3b}, {0, 0x3c}, {0x2d, 0x2f}, {0, 0x31},
    {0, 0x33}, {0, 0x35}, {0, 0x3a}, {10, 0xe}, {0, 0xf}, {0, 0x19}, {0, 0xd7}, {0, 8}, {0, 1}};
#pragma section npdata end
static u32 sScriptGuard;

extern "C" BOOL StartMessageScript(ScriptPlayer *event, const char *name, u32 id, u32 unk3, u32 unk4) {
    BOOL idle = event->mCommandIndex == event->mCommandCount;
    BOOL nine = event->mState == 9;
    if (nine & idle) {
        if (!(sScriptGuard & 1)) {
            gMessageScriptCommands[7].mValue  = FxScale(0.27f);
            gMessageScriptCommands[8].mValue  = FxScale(0.43f);
            gMessageScriptCommands[9].mValue  = FxScale(0.86f);
            gMessageScriptCommands[10].mValue = Identity16(0x7fff);
            sScriptGuard |= 1;
        }
        MIi_CpuCopy32(gMessageScriptCommands, event->mCommands, 0x44);
        event->mCommands[14].mValue = event->func_02070554((s8) *(s16 *) &unk4, unk3);
        event->mCommands[15].mValue = *(s16 *) &unk4;
        event->mCommands[16].mValue = id;
        event->mCommandIndex = 0;
        event->mCommandCount = 0x11;
        ChangeDir("/");
        char file[0x80];
        sprintf(file, "/font/%s.mes", name);
        LoadFileAsync(file, 0, 0, 0, &event->mFileSize, 0);
        event->mStorytalkWindow->SetCell1Visible(0);
        event->mMessageState = 0;
        SetNoStorytalkMessage(event->mStorytalkWindow);
        event->mMessagePos = 0;
        event->mMessageCount = 0;
        event->mState = 2;
        return TRUE;
    }
    return FALSE;
}

extern "C" void func_0206f88c(ScriptPlayer *event, u32 category, s32 value, u32 unk4) {
    BOOL idle    = event->mCommandIndex >= event->mCommandCount;
    BOOL waiting = event->mUnk_12ba == 3;
    BOOL changed = value != event->mStorytalkWindow->mMessageIndex;
    if (!(idle & (waiting & changed))) {
        return;
    }
    MIi_CpuCopy32(data_020a2118, event->mCommands, 0x80);
    s32 offset = data_020a1b12[category].mUnk_3;
    if (offset < 0) {
        event->mCommands[5].mValue = -1;
    } else {
        event->mCommands[5].mValue = offset + 0x24;
    }
    u8 kind                  = data_020a1b12[category].mUnk_2;
    s16 extra                = data_020a1b12[category].mUnk_0;
    event->mCommands[10].mValue = kind;
    event->mCommands[13].mValue = extra;
    event->mCommands[29].mValue = event->func_02070554((s8) kind, unk4);
    event->mCommands[30].mValue = kind;
    event->mCommands[31].mValue = value;
    event->mUnk_12c0 = 0x1d;
    event->mCommandIndex = 0;
    event->mCommandCount = 0x20;
    event->mMessageState = 0;
    SetNoStorytalkMessage(event->mStorytalkWindow);
    event->mMessagePos = 0;
    event->mMessageCount = 0;
    event->mState = 3;
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

u32 ScriptPlayer::func_0206f9b4(const char *name, u32 unk2, u32 unk3, u32 unk4, u32 unk5, u32 unk6, u32 unk7) {
    u16 shorts[2] = {unk2, 0};
    u8 bytes[2]   = {unk3, 0};
    return func_0206fa20(name, shorts, bytes, 1, *(u16 *) &unk4, unk5, *(u8 *) &unk6, unk7, 0);
}



extern "C" void MIi_CpuCopy32(const void *src, void *dest, u32 size);

u8 ScriptPlayer::func_0206fa20(const char *name, const u16 *unk2, const u8 *unk3, u32 unk4, u32 unk5, u32 unk6, u32 unk7,
                                     u32 unk8, u32 unk9) {
    if (mState != 9 || mCommandIndex != mCommandCount) {
        return FALSE;
    }
    u8 n = 0;
    mCommands[n].mCommand = 14;
    mCommands[n].mValue = 0;
    n++;
    mCommands[n].mCommand = 15;
    mCommands[n].mValue = -16;
    n++;
    mCommands[n].mCommand = 25;
    mCommands[n].mValue = 0;
    n++;
    mCommands[n].mCommand = 59;
    mCommands[n].mValue = *(u8 *) &unk7;
    n++;
    mCommands[n].mCommand = 60;
    n++;
    mCommands[n].mCommand = 73;
    mCommands[n].mValue = unk2[0] % 22;
    n++;
    mCommands[n].mCommand = 74;
    mCommands[n].mValue = unk3[0];
    n++;
    mCommands[n].mCommand = 78;
    n++;
    switch (unk6) {
        case 2:
        if (*(u8 *) &unk7 != 2) {
            if (unk2[0] == 0x10) {
                mCommands[n].mCommand = 80;
                mCommands[n].mValue = 0;
                n++;
            }
        }
        mCommands[n].mCommand = 81;
        mCommands[n].mValue = data_020a1b68[unk2[0]].mUnk_0;
        n++;
        mCommands[n].mCommand = 82;
        mCommands[n].mValue = data_020a1b68[unk2[0]].mUnk_1;
        n++;
        mCommands[n].mCommand = 28;
        mCommands[n].mValue = 0;
        n++;
        mCommands[n].mCommand = 29;
        mCommands[n].mValue = 12;
        n++;
        mCommands[n].mCommand = 30;
        mCommands[n].mValue = -1;
        n++;
        mCommands[n].mCommand = 34;
        mCommands[n].mValue = -3;
        n++;
        mCommands[n].mCommand = 35;
        mCommands[n].mValue = 7;
        n++;
        mCommands[n].mCommand = 36;
        mCommands[n].mValue = -101;
        n++;
            break;
        case 0xa:
        mCommands[n].mCommand = 80;
        mCommands[n].mValue = 0;
        n++;
        mCommands[n].mCommand = 81;
        mCommands[n].mValue = 0;
        n++;
        mCommands[n].mCommand = 82;
        mCommands[n].mValue = 0;
        n++;
        mCommands[n].mCommand = 87;
        mCommands[n].mValue = 330;
        n++;
        mCommands[n].mCommand = 92;
        mCommands[n].mValue = 128;
        n++;
        mCommands[n].mCommand = 78;
        mCommands[n].mValue = 0;
        n++;
        mCommands[n].mCommand = 49;
        mCommands[n].mValue = 2580;
        n++;
        mCommands[n].mCommand = 51;
        mCommands[n].mValue = 1761;
        n++;
        mCommands[n].mCommand = 53;
        mCommands[n].mValue = (s16) 0xc0f6;
        n++;
        mCommands[n].mCommand = 58;
        n++;
        mLightColor = 0x7fff;
        mCommands[n].mCommand = 28;
        mCommands[n].mValue = -62;
        n++;
        mCommands[n].mCommand = 29;
        mCommands[n].mValue = 20;
        n++;
        mCommands[n].mCommand = 30;
        mCommands[n].mValue = -21;
        n++;
        mCommands[n].mCommand = 34;
        mCommands[n].mValue = -13;
        n++;
        mCommands[n].mCommand = 35;
        mCommands[n].mValue = 7;
        n++;
        mCommands[n].mCommand = 36;
        mCommands[n].mValue = -108;
        n++;
            break;
        default:
        if (*(u8 *) &unk4 == 1) {
            if (*(u8 *) &unk7 == 2) {
                if (unk2[0] == 6) {
                    mCommands[n].mCommand = 81;
                    mCommands[n].mValue = -20;
                    n++;
                } else if (unk2[0] == 0xe) {
                    mCommands[n].mCommand = 80;
                    mCommands[n].mValue = 15;
                    n++;
                    mCommands[n].mCommand = 81;
                    mCommands[n].mValue = 20;
                    n++;
                }
            }
        } else if (*(u8 *) &unk4 == 2) {
            mCommands[n].mCommand = 80;
            mCommands[n].mValue = data_020a22a4.mUnk_0;
            n++;
            mCommands[n].mCommand = 82;
            mCommands[n].mValue = data_020a22a4.mUnk_4;
            n++;
            mCommands[n].mCommand = 87;
            mCommands[n].mValue = data_020a22a4.mUnk_6;
            n++;
            const UnkStruct_s16x4 *e = data_020a221c;
            u16 id = unk2[1];
            mCommands[n].mCommand = 96;
            mCommands[n].mValue = unk2[1] % 22;
            n++;
            mCommands[n].mCommand = 97;
            mCommands[n].mValue = unk3[1];
            n++;
            mCommands[n].mCommand = 101;
            n++;
            mCommands[n].mCommand = 103;
            mCommands[n].mValue = e[id].mUnk_0;
            e += id;
            n++;
            mCommands[n].mCommand = 104;
            mCommands[n].mValue = e->mUnk_2;
            n++;
            mCommands[n].mCommand = 105;
            mCommands[n].mValue = e->mUnk_4;
            n++;
            mCommands[n].mCommand = 110;
            mCommands[n].mValue = e->mUnk_6;
            n++;
            mCommands[n].mCommand = 117;
            mCommands[n].mValue = 1;
            n++;
            mCommands[n].mCommand = 115;
            mCommands[n].mValue = 148;
            n++;
        }
        if (unk9 != 0) {
            mCommands[n].mCommand = 28;
            mCommands[n].mValue = -11;
            n++;
            mCommands[n].mCommand = 29;
            mCommands[n].mValue = 16;
            n++;
            mCommands[n].mCommand = 30;
            mCommands[n].mValue = 0;
            n++;
            mCommands[n].mCommand = 34;
            mCommands[n].mValue = -1;
            n++;
            mCommands[n].mCommand = 35;
            mCommands[n].mValue = 10;
            n++;
            mCommands[n].mCommand = 36;
            mCommands[n].mValue = -100;
            n++;
        }
    }
    mCommands[n].mCommand = 14;
    mCommands[n].mValue = 10;
    n++;
    mCommands[n].mCommand = 15;
    mCommands[n].mValue = 0;
    n++;
    mCommands[n].mCommand = 25;
    mCommands[n].mValue = 0;
    n++;
    s16 value;
    if (*(u8 *) &unk4 == 1) {
        value = unk2[0];
    } else {
        value = unk2[1];
    }
    mCommands[n].mCommand = 8;
    mCommands[n].mValue = value;
    n++;
    mCommands[n].mCommand = 1;
    mCommands[n].mValue = *(u16 *) &unk5;
    n++;
    mCommands[n].mCommand = 0xd7;
    mCommands[n].mValue = func_02070554((s8) value, unk6);
    n++;
    mCommands[n].mCommand = 2;
    n++;
    if (unk8 != 0) {
        mCommands[n].mCommand = 234;
        n++;
        mUnk_14ed = 1;
    }
    mCommands[n].mCommand = 15;
    mCommands[n].mValue = -16;
    n++;
    mCommands[n].mCommand = 25;
    mCommands[n].mValue = 0;
    n++;
    mCommandIndex = 0;
    mCommandCount = n;
    SetNoStorytalkMessage(mStorytalkWindow);
    ChangeDir("/");
    char file[0x80];
    sprintf(file, "/font/%s.mes", name);
    LoadFileAsync(file, 0, 0, 0, &mFileSize, 0);
    mSkipped = 0;
    mState = 2;
    return TRUE;
}

BOOL ScriptPlayer::func_020702e0(s32 unk1, s32 unk2, s32 unk3, s32 unk4, s32 unk5, s32 unk6) {
    if (mCommandIndex != mCommandCount) {
        return FALSE;
    }
    MI_CpuCopy8(data_020a1b8a, mCommands, 0x24);
    mCommands[0].mValue = unk1;
    mCommands[1].mValue = unk2;
    mCommands[2].mValue = unk3;
    mCommands[3].mValue = *(s16 *) &unk4;
    mCommands[4].mValue = *(s16 *) &unk5;
    mCommands[5].mValue = *(s16 *) &unk6;
    mCommandIndex  = 0;
    u16 count  = mCommandCount = 9;
    mMessageState  = 0;
    mMessageCount  = 0;
    mMessagePos  = 0;
    mMessageIndices[0] = count - 10;   // -1 derived from the count, as the original computes it
    SetNoStorytalkMessage(mStorytalkWindow);
    mMaps[0].mUnk_32 = 1;
    mState            = 3;
    return TRUE;
}

extern const u8 data_020a1b48[];

void ScriptPlayer::SkipScript() {
    s32 target = -2;
    s32 flag   = 0;
    while (mCommandIndex < mCommandCount) {
        switch (mCommands[mCommandIndex].mCommand) {
            case 0xd5:
                target = (s8) mCommands[mCommandIndex].mValue;
                break;
            case 0xd6:
                target = -1;
                break;
            case 0xe5:
                flag = 0xe5;
                break;
        }
        mCommandIndex++;
    }
    MI_CpuCopy8(data_020a1b48, mCommands, 0x20);
    if (mUnk_14e4 != 0) {
        mCommands[3].mCommand = 0xe8;
    } else {
        mCommands[3].mCommand = 0;
    }
    if (target >= -1) {
        if (target != -1) {
            mCommands[5].mCommand = 0xd6;
        } else {
            mCommands[5].mCommand = 0xd5;
            mCommands[5].mValue   = target;
        }
    } else {
        mCommands[5].mCommand = 0;
    }
    if (flag != 0) {
        mCommands[7].mCommand = flag;
    } else {
        mCommands[7].mCommand = 0;
    }
    mCommandIndex = 0;
    mCommandCount = 8;
    mMessageState = 0;
    mUnk_14ee = 0;
    mMessageCount = mMessagePos;
    SetNoStorytalkMessage(mStorytalkWindow);
    mState = 3;
    mSkipped = 1;
    SpriteManager::Get().StartFade(FADE_MODE_TO_ZERO);
    if (mBg0Wave != 0) {
        SetBg0Wave(0);
    }
}

s32 ScriptPlayer::func_02070554(s32 kind, s32 index) {
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


extern "C" void InvalidateStorytalkMessageIndex(ScriptPlayer *event) {
    if (event->mStorytalkWindow != NULL) {
        event->mStorytalkWindow->mMessageIndex = -1;
    }
}
