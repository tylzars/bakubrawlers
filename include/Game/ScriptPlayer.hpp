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

// Results (StopWithResult, GetResult), named for the scene MapScene::SetNextSceneFromEvent requests for each. 0 = no
// result yet. 8 (command 230, or func_02066814 when gSaveData mUnk_42 reaches 10) stays a literal: MapScene handles
// it only in its default case, TournamentScene::HandleEventResult sets mCreditsPending for it.
#define SCRIPT_RESULT_MYROOM     1 // command 225
#define SCRIPT_RESULT_STORE      2 // command 226
#define SCRIPT_RESULT_PARK       3 // command 227
#define SCRIPT_RESULT_TOURNAMENT 4 // command 228
#define SCRIPT_RESULT_BATTLE     5 // command 231 (also sets the player count), and func_02066814 after SetupBattle
#define SCRIPT_RESULT_MAP        6 // func_02066814 at the end of a table
#define SCRIPT_RESULT_TUTORIAL   7 // command 229

// Kinds: the ctor's first argument (mKind), each named after the one scene class that passes it; func_02066814
// runs the story table entry only while it equals data_020b4324[gSaveData mUnk_42]->mScriptKind, which func_020667ec
// returns (MapScene::SetupMap marks the PARK tab for SCRIPT_KIND_PARK, and for SCRIPT_KIND_TOURNAMENT the TOURNAMENT
// tab when gSaveData[0].mUnk_24 bit 0 is set, otherwise the STORE tab with mUnk_7c = 1). SetupBattle sets
// gGameData.mNextScene to SCENE_PARK for PARK, SCENE_ARENA_WINDOW for TOURNAMENT and SCENE_MAP for the others.
// After SetupBattle, MAP always stops with SCRIPT_RESULT_BATTLE (the other kinds only with mBattlePending set); MAP
// also gets func_0206ef4c's commands from mModelCommandIndex on; MYROOM_TIPS sets mSkipDisabled; for TUTORIAL
// func_02066f60 returns 0 without reading the table. 7 stays a literal: MapScene, ParkScene and TournamentScene all
// pass it, func_02066814 writes it at the end of a table and func_020667ec returns it for mUnk_42 >= 11.
#define SCRIPT_KIND_MAP         0
#define SCRIPT_KIND_MYROOM      1
#define SCRIPT_KIND_PARK        2
#define SCRIPT_KIND_TOURNAMENT  3
#define SCRIPT_KIND_STORE       4
#define SCRIPT_KIND_MYROOM_TIPS 5
#define SCRIPT_KIND_TUTORIAL    6

// GetCommandSlot's result, 0 or 1 by command id (not a SCRIPT_KIND_ value): an enum, not BOOL (the final compare's
// moveq/movne order needs it)
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

// mCameraPoints[0] is the eye (Camera::SetPosition), [1] the target (SetTarget). mX..mZ are fx32; the From / To
// values are whole units, converted with FXC when UpdateWait interpolates.
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

// mVec[0] is the current value, [1] the start and [2] the end of UpdateWait's interpolation (x pitch, y yaw).
// In camera mode 0 UpdateCameraAndLight sets the target to eye + 100 * (sin(yaw), sin(pitch), -cos(yaw)).
struct ScriptCameraAngles {
  union {
   Vec3s mVec[3];
   struct {
    /* 00 */ u16 mPitch;
    union {
        /* 02 */ u16 mYaw;
        /* 02 */ s16 mYawSigned; // s16 view of mYaw
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
    /* 32 */ u8 mHoldAnims; // DrawMap: extra anim to frame 0 then one step, main anim not advanced; needs an extra anim
    /* 33 */ PAD(0x33, 0x34);
    /* 34 */
};

// A model slot (gScriptModelEntries entries). In ScriptPlayer, the Model methods (LoadModel, SwapModel, ReleaseModel,
// DrawModels, mModels) mean this slot kind; ReleaseModels frees the models of every kind.
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

// mX..mZ are drawn at half size (x / 2, y / 2, z / 2 - 0x72000).
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
    /* 39 */ u8 mStorytalkCell2XTo;
    /* 3a */ u8 mStorytalkCell2XFrom;
    /* 3b */ u8 mStorytalkCell2Frame;
    /* 3c */ u8 mSineYEnabled; // while non-zero DrawSpheres steps mSineYScale up to 1, else down to 0
    /* 3d */ u8 mSineYAngle; // 0..0x7f position of the sine DrawSpheres adds to y
    /* 3e */ u8 mSineYScale; // 0 or 1: multiplier of that sine offset
    /* 3f */ u8 mShadowScale;
    /* 40 */ s16 mShadowY; // whole units; drawn at (mShadowY << 12) - 0x64000
    /* 42 */ u8 mSwapPending;
    /* 43 */ PAD(0x43, 0x44);
    /* 44 */
};

// mX..mZ are drawn at half size with offsets (x / 2, y / 2 - 0x64000 plus mOffsetY / 2 whole units,
// z / 2 - 0x80000).
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
    /* 36 */ s8 mCharacterId; // -1 = none
    /* 37 */ s8 mNextCharacterId;
    // +38..+3a are rows of GetAnimTableValue's per-id tables: +39 the row playing, +3a the row asked for
    // (func_02067f30), +38 the row for the next character (SwapCharacter copies it into both).
    /* 38 */ u8 mNextAnimTableIndex;
    /* 39 */ u8 mAnimTableIndex; // the row the playing animation leads to (already 0 while the move back to 0 plays)
    /* 3a */ u8 mTargetAnimTableIndex;
    /* 3b */ u8 mAnimChangePending; // only set while the character is visible
    /* 3c */ u8 mExtraAnimEnabled;
    /* 3d */ u8 mShadowScale;
    /* 3e */ u8 mVisible; // draw gate; DrawCharacters still writes the position while 0
    /* 3f */ u8 mStorytalkCell2X; // also eased from mStorytalkCell2XFrom to mStorytalkCell2XTo by UpdateWait
    /* 40 */ u8 mStorytalkCell2XTo;
    /* 41 */ u8 mStorytalkCell2XFrom;
    /* 42 */ u8 mStorytalkCell2Frame;
    /* 43 */ u8 mSwapPending;
    /* 44 */ PAD(0x44, 0x48);
    /* 48 */
};

// Script player: StartScript starts the read of "/script/%s"; ReadScript takes the 0x20-byte .mes name and the 3-byte
// commands (into mCommands) from that file and starts the read of "/font/<name>.mes"; Update runs the commands
// (RunCommands, UpdateWait) and draws the map, model, sphere and character slots and the storytalk window's texts.
class ScriptPlayer : public SceneBase {
public:
    /* 000c */ ScriptCommand mCommands[0x400];
    /* 100c */ CellAnimation *mCutsceneCellAnim; // made by LoadGraphics when mUnk_14e4 == 0 and (mUnk_14f1 or func_02066c7c())
    /* 1010 */ ModelInstance *mShadowModel;
    /* 1014 */ ModelLightGroup *mLights;
    /* 1018 */ StorytalkWindow *mStorytalkWindow;
    /* 101c */ ScriptMap mMaps[2]; // only [0] is loaded and drawn
    /* 1084 */ ScriptModel mModels[2];
    /* 112c */ ScriptSphere mSpheres[2];
    /* 11b4 */ ScriptCharacter mCharacters[2];
    /* 1244 */ ScriptCameraPoint mCameraPoints[2];
    /* 1274 */ ScriptCameraAngles mCameraAngles;
    /* 1288 */ Camera *mCamera;
    /* 128c */ u16 mFov[3]; // degrees: [0] current, [1] start, [2] end of the interpolation; ApplyFov uses [0]
    /* 1292 */ u8 mCameraMode;
    /* 1293 */ PAD(0x1293, 0x1298);
    /* 1298 */ Vec3s mLightDir;
    /* 129e */ Vec3s mLightDirFrom;
    /* 12a4 */ Vec3s mLightDirTo;
    /* 12aa */ PAD(0x12aa, 0x12ac);
    /* 12ac */ u16 mLightColor;
    /* 12ae */ PAD(0x12ae, 0x12b0);
    /* 12b0 */ u32 mFileData; // buffer of the last finished read; ReadScript frees the script data but keeps the pointer
    /* 12b4 */ u32 mFileSize;
    /* 12b8 */ u8 mModelLoadStep;
    /* 12b9 */ u8 mState;
    // command 9, at most 3 (a negative value is kept and acts like 1): 0 waits for input, 3 keeps the last page,
    // others go on at once (message state 5)
    /* 12ba */ u8 mMessageMode;
    /* 12bb */ PAD(0x12bb, 0x12bc);
    /* 12bc */ u16 mCommandIndex;
    /* 12be */ u16 mCommandCount;
    // written only by func_0206ef4c and func_0206f88c (mModelCommandIndex: func_0206ef4c); other script builders
    // leave them as they were
    /* 12c0 */ u16 mChannel1CommandIndex;
    /* 12c2 */ u16 mModelCommandIndex;
    /* 12c4 */ u8 mWaitTimer; // command 13 sets it for SCRIPT_WAIT_KIND_TIMER; every SCRIPT_WAIT_KIND_LOAD wait starts it at 0, so that countdown never runs
    /* 12c5 */ u8 mWaitKind;
    /* 12c6 */ u8 mCurrentCommand;
    /* 12c7 */ PAD(0x12c7, 0x12c8);
    /* 12c8 */ s16 mCurrentValue;
    /* 12ca */ u8 mMessageState;
    /* 12cb */ u8 mInputTimer; // frames between an accepted input and the page step
    /* 12cc */ u16 mMessagePos;
    /* 12ce */ u16 mMessageCount; // texts queued by command 1; wraps to 0 at 0x80 while mMessagePos does not
    // 200 on entering message state 7 and after a restart, 300 after a page step (HasMorePages is FALSE right after
    // NextPage, so the 200 there never runs)
    /* 12d0 */ u16 mNextPageTimer;
    /* 12d2 */ s16 mMessageIndices[0x80];
    /* 13d2 */ s16 mMessageCharacterIds[0x80]; // value command 8 gives the next queued text; StorytalkWindow::SetMessage's second argument
    // the last value command 213 handled; command 214 stops channel 0 without clearing it
    /* 14d2 */ s8 mChannel0FrameTaskId;
    /* 14d3 */ u8 mFadeKind;
    /* 14d4 */ s8 mUnk_14d4;
    /* 14d5 */ s8 mBrightness;
    /* 14d6 */ s8 mBrightnessTo;
    /* 14d7 */ s8 mBrightnessFrom;
    /* 14d8 */ u8 mBrightnessFrames;
    /* 14d9 */ u8 mBrightnessFrameCount;
    /* 14da */ u16 mLerpFrames; // also the envelope position Bg0WaveCallback reads
    /* 14dc */ u16 mLerpFrameCount;
    /* 14de */ u16 mFrameCount; // set by command 14; copied into mBrightnessFrameCount and mLerpFrameCount
    /* 14e0 */ u8 mFinishRequested;
    /* 14e1 */ u8 mResult;
    /* 14e2 */ u8 mKind; // SCRIPT_KIND_*, the ctor argument; func_02066814 sets 7 at the end of a table
    /* 14e3 */ PAD(0x14e3, 0x14e4);
    /* 14e4 */ u8 mUnk_14e4;
    /* 14e5 */ u8 mUnk_14e5;
    /* 14e6 */ u8 mGraphicsLoaded; // set only after wait kind 9's LoadGraphics (command 232); the scene clears it
    /* 14e7 */ u8 mUnk_14e7;
    /* 14e8 */ u8 mUnk_14e8;
    // set by some SetupBattle paths only; the readers do not clear it, only the gSaveData[0].mCreditsDone test does
    /* 14e9 */ u8 mBattlePending;
    /* 14ea */ u8 mSaveRequested; // also makes state 5 run the unlock checks; state 6 waits until it is cleared
    /* 14eb */ u8 mUnlockPopupHandle; // 1 characters, 2 fields, 3 tips (state 5); callers open UnlockPopup(it - 1)
    /* 14ec */ u8 mSkipped;
    /* 14ed */ u8 mSkipDisabled;
    // passed only to texts started from message state 0 (the restarts pass 0); 2 blocks input, non-zero skips seq 0x6b
    /* 14ee */ u8 mTextOnly;
    /* 14ef */ u8 mTouchLatched;
    // every new ScriptPlayer passes 0 for it: the ctor / dtor StopChannel(0) does not run in this build
    /* 14f0 */ u8 mStopChannel0;
    /* 14f1 */ u8 mUnk_14f1;
    /* 14f2 */ u8 mCutsceneAnimIndex;
    /* 14f3 */ u8 mBg0Wave;
    /* 14f4 */

    ScriptPlayer(u32 unk1, u32 unk2, u32 unk3);
    virtual ~ScriptPlayer();
    virtual void Update();
    // Script number for the save position (gSaveData mUnk_42 / mUnk_43) or -1; may call SetupBattle and
    // StopWithResult even with unk1 == 0; unk1 != 0 also advances the position.
    s32 func_02066814(u32 unk1);
    void StopWithResult(u32 unk1); // stores the result and waits in state 4 with SCRIPT_WAIT_KIND_STOP
    void UpdateBrightness(); // the code after 'mUnk_14d4 == 0' never runs: mUnk_14d4 is only ever 0
    void UpdateBrightnessFade(); // interpolates mBrightness; applies it only while mFadeKind == SCRIPT_FADE_KIND_BLEND
    void SetCharacterVisible(u32 index, u8 unk2); // also clears the slot's mAnimChangePending
    void ApplyPendingSwaps();
    void CreateNextCharacter(u32 index);
    void SwapCharacter(u32 index);
    void SwapSphere(u32 index);
    void SwapModel(u32 index);
    // flag 0 creates the current character; flag 1 only waits for the next one's load (CreateNextCharacter).
    // TRUE at once for an id < 0; both first call RestoreCharacterSpriteSlot.
    BOOL LoadCharacter(u32 index, u32 flag);
    void RestoreCharacterSpriteSlot(u32 index, u32 characterId);
    void ResetCharacterParams(u32 index);
    s8 GetAnimTableValue(s32 kind, u32 index, u32 column);
    void SetCharacterExtraAnimEnabled(u32 index, u32 value); // with mAnimChangePending also clears its anim's mLoop
    void UpdateCharacterExtraAnims();
    void ApplyCharacterRotation(u32 index);
    // FALSE at once when value is the current map id; else -2 keeps the id, the map model is deleted and TRUE = a load
    // is needed.
    BOOL SetMap(s32 value);
    BOOL LoadMap(); // id clamped to 0..0x11; also resets the camera and the light
    // FALSE at once when value is the slot's model id; else stores it (-2 keeps it), deletes character 1's model
    // (index 0) or sphere 1's (index 1), releases the slot; TRUE = a load is needed.
    BOOL func_02068c54(u32 index, s32 value);
    void LoadRealbakuPalettes(TextureFile *texture, u32 id); // only for ids 0x1c / 0x1d: eleven palettes
    void StartNextModelLoad(u32 index, u32 value);
    void ReleaseModel(u32 index); // the mNextFiles buffers are not freed
    BOOL func_02069680(u32 index);
    void CreateNextSphere(u32 index); // reads nothing: the model is loaded when SwapSphere runs its stored setup
    u8 GetSphereVariant(u32 index);
    BOOL LoadModel(u32 index);
    BOOL LoadNextModelFiles(u32 index); // staged
    BOOL LoadSphere(u32 index, u32 flag); // flag 1 loads nothing and is TRUE at once (func_0203d6f0)
    void ResetSphereParams(u32 index); // with mUnk_36 set also restarts the sphere's animation
    void ApplySphereRotation(u32 index);
    void ApplyFov(); // only mFov[0]
    void UpdateCameraAndLight(); // nothing when the eye equals the target
    void SetCameraMode(u32 mode);
    void UpdateCutsceneAndStorytalkCell2(); // the cell-2 part is skipped every other frame while mUnk_14e4 is set
    void SetCutsceneAnimation(u32 value, u32 force);
    void SkipScript();
    void UpdateMessages(); // starts with the skip check (key 8 in states 3-4 outside SCRIPT_WAIT_KIND_LOAD calls SkipScript)
    void DrawSpheres();
    void DrawCharacters();
    void UpdateCharacterAnimation(u32 index);
    void DrawMap();
    void DrawModels();
    UnkStruct_02065c40_Kind GetCommandSlot(u32 id);
    void RunCommands(); // at the end of the list: func_02066814(1), state 5, ReleaseModels(0)
    void ExecuteCommand(u32 command, s32 value);
    s32 LookupChannel1CommandValue(s32 kind, s32 index);
    void SetBg0Wave(u32 flag);
    BOOL ReadMessages();
    BOOL func_02067f30(u32 index, s32 value);
    // 0 for mKind == SCRIPT_KIND_TUTORIAL; with a script number from func_02066814(0) starts "cmd_ST_mev_..." and
    // returns 1; otherwise func_02066814(1) (and StopWithResult(SCRIPT_RESULT_BATTLE) when mBattlePending is set)
    // and 0.
    u32 func_02066f60();
    // Also writes display registers; makes the cutscene cells when mUnk_14e4 == 0 and (mUnk_14f1 or
    // func_02066c7c()), and then with mKind == SCRIPT_KIND_MYROOM_TIPS clears mUnk_14f1 and sets
    // mSkipDisabled.
    void LoadGraphics();
    BOOL ReadScript(); // also starts the read of the script's .mes file
    void UpdateWait(); // kind 4 runs the interpolation of every slot, the camera, light and FOV; kind 9 calls LoadGraphics
    void SetupSpriteSlots();
    // Every slot kind's models; with unk1 == 0 the map and shadow models stay. The mNextFiles buffers are not freed.
    void ReleaseModels(u32 unk1);
    void ResetCamera();
    void ResetLight();
    u32 func_020667ec();
    BOOL func_02066c7c(); // func_02066814(0) != -1; that call's side effects (SetupBattle, StopWithResult) still happen
    void SetupBattle(u32 unk1, u32 unk2);
    u32 func_020670b4(u32 id);
    u8 func_02067240(u32 id);
    u8 func_02067120(u32 unk1, u32 unk2, u32 unk3, u32 unk4);
    BOOL IsRunning(); // mState != 9: also TRUE in state 0, 5-8 and 10
    u32 GetResult(); // 0 while mSaveRequested or mUnlockPopupHandle is set
    BOOL IsGraphicsLoaded();
    void ClearGraphicsLoaded();
    u32 GetSaveRequest(); // mSaveRequested in state 6, else 0
    void ClearSaveRequest();
    u32 GetUnlockPopupHandle();
    void ClearUnlockPopupHandle();
    BOOL StartScript(const char *path);
    void RequestFinish();
    BOOL func_0206ef4c(const char *name, u32 unk2, u32 unk3, u32 unk4, s32 unk5);
    void SetMapExtraAnim(u8 unk1);
    void func_0206f564(s32 unk1, u32 unk2, s32 unk3);
    u32 func_0206f9b4(const char *name, u32 unk2, u32 unk3, u32 unk4, u32 unk5, u32 unk6, u32 unk7);
    u8 func_0206fa20(const char *name, const u16 *unk2, const u8 *unk3, u32 unk4, u32 unk5, u32 unk6, u32 unk7,
                     u32 unk8, u32 unk9);
    BOOL func_020702e0(s32 unk1, s32 unk2, s32 unk3, s32 unk4, s32 unk5, s32 unk6);
};
