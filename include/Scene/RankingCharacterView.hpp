#pragma once

#include "global.h"
#include "types.h"

#include "Scene/SceneBase.hpp"

class CellAnimation;
class ModelInstance;
class Camera;
class ModelLightGroup;
class UnkStruct_0205401c;
struct MessageFile;

// Character preview scene object: a 3D model on a stage that slides between positions, with a name text and
// sprite cells (vtable 0x020b938c).
class RankingCharacterView : public SceneBase {
public:
    /* 0c */ CellAnimation *mRankingCellAnims[4];
    /* 1c */ CellAnimation *mDigitCellAnims[10];
    /* 44 */ ModelInstance *mMyroombgModel;
    /* 48 */ Camera *mCamera;
    /* 4c */ ModelLightGroup *mLights;
    // UnkStruct_0205401c, not a CharacterModel: holds a CharacterModel for id 0x15, a StoryCharacterModel otherwise
    // (Model.hpp:54-55)
    /* 50 */ UnkStruct_0205401c *mCharacterModel;
    /* 54 */ MessageFile *mStoryCharaNameMessages;
    /* 58 */ u8 mCharacterId;
    /* 59 */ u8 mUnk_59;
    /* 5a */ u16 mRank;
    /* 5c */ u32 mScore;
    /* 60 */ u8 mNextCharacterId;
    /* 61 */ u8 mUnk_61;
    /* 62 */ u16 mNextRank;
    /* 64 */ u32 mNextScore;
    /* 68 */ void *mTextBuffer;
    /* 6c */ void *mGlyphBuffer;
    /* 70 */ u32 mTextBufferSize;
    /* 74 */ u32 mGlyphCount;
    /* 78 */ s32 mModelX;
    /* 7c */ u8 mState;
    /* 7d */ PAD(0x7d, 0x7e);
    /* 7e */ u8 mKind;
    /* 7f */ PAD(0x7f, 0x80);
    /* 80 */ u32 mChanging;
    /* 84 */ u32 mFinishRequested;
    /* 88 */

    RankingCharacterView(u8 unk1, u16 unk2, u32 unk3, u32 unk4); // 0x020741b4
    virtual ~RankingCharacterView();
    virtual void Update();
    BOOL ChangeCharacter(u8 unk1, u16 unk2, u32 unk3);
    void Load();
    void CreateCharacterModel(u32 kind);
    void Draw();
    void DrawSprites();
    void PrintName();
    void ClearNameText();
    void ApplyNextCharacter();
    BOOL SlideTo(s32 target);
};
