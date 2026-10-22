#pragma once

#include "global.h"
#include "types.h"

#include "Scene/SceneBase.hpp"

#include <nitro/fx.h>

class Camera;
class BattleMapObj;
class UnkStruct_0205401c;
class ModelLightGroup;
class ModelInstance;

// A character (story / edited) shown on the battle field with its own camera path (vtable 0228a2d8, 0x40 bytes).
// Path strings: /Game/Character/Act, /Game/Character/Character, /Game/Character/EditedCharacter/Cam.
class BattleCharaAct : public SceneBase {
public:
    /* 0c */ Camera *mCamera;
    /* 10 */ Camera *mSecondCamera;
    /* 14 */ void *mCamFile;
    /* 18 */ BattleMapObj *mMapObj;
    /* 1c */ UnkStruct_0205401c *mCharacter;
    /* 20 */ ModelLightGroup *mLights;
    /* 24 */ ModelInstance *mActModel;
    /* 28 */ ModelInstance *mFieldopModel;
    /* 2c */ u32 mEdited;
    /* 30 */ u16 mFrameTaskId;
    /* 32 */ u16 mActFrames; // kind 0: +1 per frame while no fade runs, 0xffff = stopped; past 0x5a the brightness rises every 7th
    /* 34 */ u8 mFrameTaskTimer;
    /* 35 */ u8 mState;
    /* 36 */ u8 mKind;
    /* 37 */ u8 mUnclampedKind;
    /* 38 */ u8 mCharacterId;
    /* 39 */ u8 mPlayer;
    /* 3a */ u8 mActDrawFrames;
    /* 3b */ u8 mTexVariant; // T_sphe_NN for kinds 0 / 3 / 15 and 7-10, ab_NN for kinds 1 / 6 only when 1-2; 0 = no swap
    /* 3c */ u8 mSecondTexVariant; // kind 8 only; 1 = no swap (0 swaps in T_sphe_00)
    /* 3d */ s8 mFadeBrightness;
    /* 3e */ u8 mNextScene;
    /* 3f */ PAD(0x3f, 0x40);
    /* 40 */

    BattleCharaAct(Camera *camera, u8 player, u32 kind, u8 unk4, u8 unk5);
    virtual ~BattleCharaAct();
    /* 08 */ virtual void Update();
    void Init();
    u8 CreateCharacter(); // returns the character's mUseCharacter: TRUE for the edited character (id 0x15)
    void LoadActModels(); // kinds 2, 4, 5 and 11-14 load nothing
    void StartCardEffect();
    // Creates and loads mMapObj (type 9 for kind 0, 0xb for kind 15, else 0xa); unless kind 5 / 6 also sets
    // gGameData.mUnk_10's layout 0xd.
    void func_ov007_02269380();
    void Draw(); // also steps mCamera along its path; kind 0 calls it twice a frame while the camera index is 0
    void LoadFieldopModel();
    void DeleteFieldopModel();
    void DrawFieldopModel();
};
