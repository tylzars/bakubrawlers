#pragma once

#include "global.h"
#include "types.h"

#include "Scene/SceneBase.hpp"

#include <nitro/fx.h>

class CellAnimation;
class ModelInstance;
class ModelLightGroup;
class StorytalkWindow;
class Camera;
class UnkStruct_0205401c;
class BakuganModel;
class TextureFile;

// GetCommandSlot's result: an enum, not BOOL (the final compare's moveq/movne order needs it)
enum UnkStruct_02065c40_Kind { UNK_02065C40_KIND_0, UNK_02065C40_KIND_1 };

struct ScriptCommand {
    /* 00 */ s16 mValue;
    /* 02 */ u8 mCommand;
    /* 03 */ PAD(0x03, 0x04);
    /* 04 */
};

struct Vec3s {
    /* 00 */ s16 x;
    /* 02 */ s16 y;
    /* 04 */ s16 z;
    /* 06 */
};

struct ScriptCameraPoint {
    /* 00 */ fx32 mX;
    /* 04 */ fx32 mY;
    /* 08 */ fx32 mZ;
    /* 0c */ s16 mXFrom;
    /* 0e */ s16 mYFrom;
    /* 10 */ s16 mZFrom;
    /* 12 */ s16 mXTo;
    /* 14 */ s16 mYTo;
    /* 16 */ s16 mZTo;
    /* 18 */
};

struct ScriptCameraAngles {
  union {
   Vec3s mVec[3];
   struct {
    /* 00 */ u16 mPitch;
    union {
        /* 02 */ u16 mYaw;
        /* 02 */ s16 mUnk_02s;
    };
    /* 04 */ PAD(0x04, 0x08);
    /* 08 */ s16 mYawFrom;
    /* 0a */ PAD(0x0a, 0x14);
   };
  };
    /* 14 */
};

struct ScriptMap {
    /* 00 */ s32 mX;
    /* 04 */ fx32 mY;
    /* 08 */ fx32 mZ;
    /* 0c */ s16 mPosFrom[3];
    /* 12 */ s16 mPosTo[3];
    /* 18 */ s16 mRot[3];
    /* 1e */ s16 mRotFrom[3];
    /* 24 */ s16 mRotTo[3];
    /* 2a */ PAD(0x2a, 0x2c);
    /* 2c */ ModelInstance *mModel;
    /* 30 */ s16 mMapId;
    /* 32 */ u8 mUnk_32;
    /* 33 */ PAD(0x33, 0x34);
    /* 34 */
};

struct ScriptModel {
    /* 00 */ fx32 mX;
    /* 04 */ fx32 mY;
    /* 08 */ fx32 mZ;
    /* 0c */ s16 mPosFrom[3];
    /* 12 */ s16 mPosTo[3];
    /* 18 */ s16 mRot[3];
    /* 1e */ s16 mRotFrom[3];
    /* 24 */ s16 mRotTo[3];
    /* 2a */ PAD(0x2a, 0x2c);
    /* 2c */ ModelInstance *mModel;
    /* 30 */ void *mFiles[3];
    /* 3c */ void *mNextFiles[3];
    /* 48 */ s16 mModelId;
    /* 4a */ s16 mNextModelId;
    /* 4c */ u8 mScale;
    /* 4d */ u8 mHideAtAnimEnd;
    /* 4e */ u8 mVisible;
    /* 4f */ u8 mStorytalkCell2X;
    /* 50 */ u8 mStorytalkCell2Frame;
    /* 51 */ u8 mSwapPending;
    /* 52 */ u8 mNextAnimIndex;
    /* 53 */ PAD(0x53, 0x54);
    /* 54 */
};

struct ScriptSphere {
    /* 00 */ fx32 mX;
    /* 04 */ fx32 mY;
    /* 08 */ fx32 mZ;
    /* 0c */ s16 mPosFrom[3];
    /* 12 */ s16 mPosTo[3];
    /* 18 */ s16 mRot[3];
    /* 1e */ s16 mRotFrom[3];
    /* 24 */ s16 mRotTo[3];
    /* 2a */ PAD(0x2a, 0x2c);
    /* 2c */ BakuganModel *mSphere;
    /* 30 */ BakuganModel *mNextSphere;
    /* 34 */ s8 mTableIndex;
    /* 35 */ s8 mNextTableIndex;
    /* 36 */ u8 mUnk_36;
    /* 37 */ u8 mVisible;
    /* 38 */ u8 mStorytalkCell2X;
    /* 39 */ u8 mUnk_39;
    /* 3a */ u8 mUnk_3a;
    /* 3b */ u8 mStorytalkCell2Frame;
    /* 3c */ u8 mUnk_3c;
    /* 3d */ u8 mUnk_3d;
    /* 3e */ u8 mUnk_3e;
    /* 3f */ u8 mShadowScale;
    /* 40 */ s16 mShadowY;
    /* 42 */ u8 mSwapPending;
    /* 43 */ PAD(0x43, 0x44);
    /* 44 */
};

struct ScriptCharacter {
    /* 00 */ fx32 mX;
    /* 04 */ fx32 mY;
    /* 08 */ fx32 mZ;
    /* 0c */ s16 mPosFrom[3];
    /* 12 */ s16 mPosTo[3];
    /* 18 */ s16 mRot[3];
    /* 1e */ s16 mRotFrom[3];
    /* 24 */ s16 mRotTo[3];
    /* 2a */ PAD(0x2a, 0x2c);
    /* 2c */ UnkStruct_0205401c *mCharacter;
    /* 30 */ UnkStruct_0205401c *mNextCharacter;
    /* 34 */ s16 mOffsetY;
    /* 36 */ s8 mCharacterId;
    /* 37 */ s8 mNextCharacterId;
    /* 38 */ u8 mUnk_38;
    /* 39 */ u8 mUnk_39;
    /* 3a */ u8 mUnk_3a;
    /* 3b */ u8 mUnk_3b;
    /* 3c */ u8 mExtraAnimEnabled;
    /* 3d */ u8 mShadowScale;
    /* 3e */ u8 mVisible;
    /* 3f */ u8 mStorytalkCell2X;
    /* 40 */ u8 mUnk_40;
    /* 41 */ u8 mUnk_41;
    /* 42 */ u8 mStorytalkCell2Frame;
    /* 43 */ u8 mSwapPending;
    /* 44 */ PAD(0x44, 0x48);
    /* 48 */
};

// Script/event player.
class ScriptPlayer : public SceneBase {
public:
    /* 000c */ ScriptCommand mCommands[0x400];
    /* 100c */ CellAnimation *mCutsceneCellAnim;
    /* 1010 */ ModelInstance *mShadowModel;
    /* 1014 */ ModelLightGroup *mLights;
    /* 1018 */ StorytalkWindow *mStorytalkWindow;
    /* 101c */ ScriptMap mMaps[2];
    /* 1084 */ ScriptModel mModels[2];
    /* 112c */ ScriptSphere mSpheres[2];
    /* 11b4 */ ScriptCharacter mCharacters[2];
    /* 1244 */ ScriptCameraPoint mCameraPoints[2];
    /* 1274 */ ScriptCameraAngles mCameraAngles;
    /* 1288 */ Camera *mCamera;
    /* 128c */ u16 mFov[3];
    /* 1292 */ u8 mCameraMode;
    /* 1293 */ PAD(0x1293, 0x1298);
    /* 1298 */ Vec3s mLightDir;
    /* 129e */ Vec3s mLightDirFrom;
    /* 12a4 */ Vec3s mLightDirTo;
    /* 12aa */ PAD(0x12aa, 0x12ac);
    /* 12ac */ u16 mLightColor;
    /* 12ae */ PAD(0x12ae, 0x12b0);
    /* 12b0 */ u32 mFileData;
    /* 12b4 */ u32 mFileSize;
    /* 12b8 */ u8 mModelLoadStep;
    /* 12b9 */ u8 mState;
    /* 12ba */ u8 mUnk_12ba;
    /* 12bb */ PAD(0x12bb, 0x12bc);
    /* 12bc */ u16 mCommandIndex;
    /* 12be */ u16 mCommandCount;
    /* 12c0 */ u16 mUnk_12c0;
    /* 12c2 */ u16 mUnk_12c2;
    /* 12c4 */ u8 mWaitTimer;
    /* 12c5 */ u8 mWaitKind;
    /* 12c6 */ u8 mCurrentCommand;
    /* 12c7 */ PAD(0x12c7, 0x12c8);
    /* 12c8 */ s16 mCurrentValue;
    /* 12ca */ u8 mMessageState;
    /* 12cb */ u8 mInputTimer;
    /* 12cc */ u16 mMessagePos;
    /* 12ce */ u16 mMessageCount;
    /* 12d0 */ u16 mUnk_12d0;
    /* 12d2 */ s16 mMessageIndices[0x80];
    /* 13d2 */ s16 mUnk_13d2[0x80];
    /* 14d2 */ s8 mUnk_14d2;
    /* 14d3 */ u8 mFadeKind;
    /* 14d4 */ s8 mUnk_14d4;
    /* 14d5 */ s8 mBrightness;
    /* 14d6 */ s8 mBrightnessTo;
    /* 14d7 */ s8 mBrightnessFrom;
    /* 14d8 */ u8 mBrightnessFrames;
    /* 14d9 */ u8 mBrightnessFrameCount;
    /* 14da */ u16 mLerpFrames;
    /* 14dc */ u16 mLerpFrameCount;
    /* 14de */ u16 mFrameCount;
    /* 14e0 */ u8 mFinishRequested;
    /* 14e1 */ u8 mResult;
    /* 14e2 */ u8 mUnk_14e2;
    /* 14e3 */ PAD(0x14e3, 0x14e4);
    /* 14e4 */ u8 mUnk_14e4;
    /* 14e5 */ u8 mUnk_14e5;
    /* 14e6 */ u8 mGraphicsLoaded;
    /* 14e7 */ u8 mUnk_14e7;
    /* 14e8 */ u8 mUnk_14e8;
    /* 14e9 */ u8 mUnk_14e9;
    /* 14ea */ u8 mSaveRequested;
    /* 14eb */ u8 mUnlockPopupHandle;
    /* 14ec */ u8 mSkipped;
    /* 14ed */ u8 mUnk_14ed;
    /* 14ee */ u8 mUnk_14ee;
    /* 14ef */ u8 mUnk_14ef;
    /* 14f0 */ u8 mUnk_14f0;
    /* 14f1 */ u8 mUnk_14f1;
    /* 14f2 */ u8 mCutsceneAnimIndex;
    /* 14f3 */ u8 mBg0Wave;
    /* 14f4 */

    ScriptPlayer(u32 unk1, u32 unk2, u32 unk3);
    virtual ~ScriptPlayer();
    virtual void Update();
    s32 func_02066814(u32 unk1);
    void StopWithResult(u32 unk1);
    void UpdateBrightness();
    void UpdateBrightnessFade();
    void SetCharacterVisible(u32 index, u8 unk2);
    void ApplyPendingSwaps();
    void CreateNextCharacter(u32 index);
    void SwapCharacter(u32 index);
    void SwapSphere(u32 index);
    void SwapModel(u32 index);
    BOOL LoadCharacter(u32 index, u32 flag);
    void func_020682a0(u32 index, u32 kind);
    void ResetCharacterParams(u32 index);
    s8 func_020687a8(s32 kind, u32 index, u32 column);
    void SetCharacterExtraAnimEnabled(u32 index, u32 value);
    void UpdateCharacterExtraAnims();
    void ApplyCharacterRotation(u32 index);
    BOOL SetMap(s32 value);
    BOOL LoadMap();
    BOOL func_02068c54(u32 index, s32 value);
    void LoadRealbakuPalettes(TextureFile *texture, u32 id);
    void StartNextModelLoad(u32 index, u32 value);
    void ReleaseModel(u32 index);
    BOOL func_02069680(u32 index);
    void CreateNextSphere(u32 index);
    u8 GetSphereVariant(u32 index);
    BOOL LoadModel(u32 index);
    BOOL LoadNextModelFiles(u32 index);
    BOOL LoadSphere(u32 index, u32 flag);
    void ResetSphereParams(u32 index);
    void ApplySphereRotation(u32 index);
    void ApplyFov();
    void UpdateCameraAndLight();
    void SetCameraMode(u32 mode);
    void func_0206a01c();
    void SetCutsceneAnimation(u32 value, u32 force);
    void SkipScript();
    void UpdateMessages();
    void DrawSpheres();
    void DrawCharacters();
    void UpdateCharacterAnimation(u32 index);
    void DrawMap();
    void DrawModels();
    UnkStruct_02065c40_Kind GetCommandSlot(u32 id);
    void RunCommands();
    void ExecuteCommand(u32 command, s32 value);
    s32 func_02070554(s32 kind, s32 index);
    void SetBg0Wave(u32 flag);
    BOOL ReadMessages();
    BOOL func_02067f30(u32 index, s32 value);
    u32 func_02066f60();
    void LoadGraphics();
    BOOL ReadScript();
    void UpdateWait();
    void SetupSpriteSlots();
    void ReleaseModels(u32 unk1);
    void ResetCamera();
    void ResetLight();
    u32 func_020667ec();
    BOOL func_02066c7c();
    void SetupBattle(u32 unk1, u32 unk2);
    u32 func_020670b4(u32 id);
    u8 func_02067240(u32 id);
    u8 func_02067120(u32 unk1, u32 unk2, u32 unk3, u32 unk4);
    BOOL IsRunning();
    u32 GetResult();
    BOOL IsGraphicsLoaded();
    void ClearGraphicsLoaded();
    u32 GetSaveRequest();
    void ClearSaveRequest();
    u32 GetUnlockPopupHandle();
    void ClearUnlockPopupHandle();
    BOOL StartScript(const char *path);
    void RequestFinish();
    BOOL func_0206ef4c(const char *name, u32 unk2, u32 unk3, u32 unk4, s32 unk5);
    void func_02067998(u8 unk1);
    void func_0206f564(s32 unk1, u32 unk2, s32 unk3);
    u32 func_0206f9b4(const char *name, u32 unk2, u32 unk3, u32 unk4, u32 unk5, u32 unk6, u32 unk7);
    u8 func_0206fa20(const char *name, const u16 *unk2, const u8 *unk3, u32 unk4, u32 unk5, u32 unk6, u32 unk7,
                     u32 unk8, u32 unk9);
    BOOL func_020702e0(s32 unk1, s32 unk2, s32 unk3, s32 unk4, s32 unk5, s32 unk6);
};
