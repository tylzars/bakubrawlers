#pragma once

#include "global.h"
#include "types.h"

#include "Scene/SceneBase.hpp"

class Camera;
class CellAnimation;
class ModelInstance;
class ModelLightGroup;
class Ranking;
class UnkStruct_0205401c;
struct MessageFile;

// Overlay 8: the player status screen (name, level, money, points, play time) over a 3D room.
class MyroomStatusScene : public SceneBase {
public:
    /* 00 (base) */
    /* 0c */ CellAnimation *mCellAnim;
    /* 10 */ UnkStruct_0205401c *mModel;
    /* 14 */ ModelInstance *mMyroombgModel;
    /* 18 */ Camera *mCamera;
    /* 1c */ ModelLightGroup *mPalette;
    /* 20 */ MessageFile *mMessages;
    /* 24 */ void *mTextBuffers[5];
    /* 38 */ void *mGlyphBuffer;
    /* 3c */ u32 mTextBufferSize;
    /* 40 */ u32 mGlyphCount;
    /* 44 */ u32 mReady;
    /* 48 */ u8 mState;
    /* 4c */ u32 mUnk_4c;
    /* 50 */ Ranking *mRanking;
    /* 54 */ u16 mHours;
    /* 56 */ u8 mMinutes;
    /* 57 */ u8 mSeconds;
    /* 58 */

    MyroomStatusScene(u32 unk); // 0x02219440

    /* 00 */ virtual ~MyroomStatusScene() override; // 0x02219564
    /* 08 */ virtual void Update() override;        // 0x022197cc
    /* 0c */

    void SetReady();         // 0x02219894
    void InitDisplay();      // 0x022198a0
    void CreateModel();      // 0x02219938
    void Setup3D();          // 0x022199b4
    void Draw3D();           // 0x02219e88
    void DrawCell();         // 0x02219f3c
    void LoadText();         // 0x02219f7c
    void ClearTextBuffers(); // 0x0221a15c
    void RenderLine(u8 line); // 0x0221a19c
    void DrawLines();        // 0x0221aa40
    void FreeText();         // 0x0221ab1c
    void UpdateTime();       // 0x0221abb0
};
