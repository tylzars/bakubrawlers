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
    /* 32 */ u16 mActFrames;
    /* 34 */ u8 mFrameTaskTimer;
    /* 35 */ u8 mState;
    /* 36 */ u8 mKind;
    /* 37 */ u8 mUnclampedKind;
    /* 38 */ u8 mCharacterId;
    /* 39 */ u8 mPlayer;
    /* 3a */ u8 mActDrawFrames;
    /* 3b */ u8 mTexVariant;
    /* 3c */ u8 mSecondTexVariant;
    /* 3d */ s8 mFadeBrightness;
    /* 3e */ u8 mNextScene;
    /* 3f */ PAD(0x3f, 0x40);
    /* 40 */

    BattleCharaAct(Camera *camera, u8 player, u32 kind, u8 unk4, u8 unk5);
    virtual ~BattleCharaAct();
    /* 08 */ virtual void Update();
    void Init();
    u8 CreateCharacter();
    void LoadActModels();
    void StartCardEffect();
    void func_ov007_02269380();
    void Draw();
    void LoadFieldopModel();
    void DeleteFieldopModel();
    void DrawFieldopModel();
};
