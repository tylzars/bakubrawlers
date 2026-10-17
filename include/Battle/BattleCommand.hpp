#pragma once

#include "global.h"
#include "types.h"

#include <nitro/fx.h>

#include "Scene/SceneBase.hpp"

class CellAnimation;
class SelectWindow;
class MessageWindow;
class BattleCommandMap;
class BattleCommandPage;
class BattleCommandList;
class BattleCommandPrompt;

// Main battle command screen (battle_b_obj_00, mes_Command, union_b cockpit): the command windows, the turn
// counter and the sub objects of the current mode. Vtable 02287ad0, at least 0x13c bytes.
class BattleCommand : public SceneBase {
public:
    /* 00 SceneBase */
    /* 0c */ CellAnimation *mCursorAnim;
    /* 10 */ CellAnimation *mUnk_10;
    /* 14 */ CellAnimation *mUnk_14;
    /* 18 */ CellAnimation *mUnk_18;
    /* 1c */ CellAnimation *mUnk_1c;
    /* 20 */ CellAnimation *mTurnCellAnim;
    /* 24 */ CellAnimation *mTurnDigitsCellAnim;
    /* 28 */ CellAnimation *mUnk_28;
    /* 2c */ CellAnimation *mPlayerCellAnims[4];
    /* 3c */ CellAnimation *mUnitCellAnims[12];
    /* 6c */ SelectWindow *mCommandWindow;
    /* 70 */ SelectWindow *mCockpitWindow;
    /* 74 */ SelectWindow *mPromptWindow;
    /* 78 */ BattleCommandPage *mCommandPage;
    /* 7c */ BattleCommandList *mCommandList;
    /* 80 */ u32 mCamera;
    /* 84 */ BattleCommandMap *mCommandMap;
    /* 88 */ MessageWindow *mMessageWindow;
    /* 8c */ BattleCommandPrompt *mCommandPrompt; // ended with CloseCommandPrompt(obj, flag)
    /* 90 */ u8 mGridMask[14][10];
    /* 11c */ u16 mUnk_11c;
    /* 11e */ u16 mUnk_11e;
    /* 120 */ s16 mScrollX;
    /* 122 */ s16 mScrollY;
    /* 124 */ s16 mUnk_124;
    /* 126 */ s16 mUnk_126;
    /* 128 */ u8 mState;
    /* 129 */ u8 mUnk_129;
    /* 12a */ u8 mMode;
    /* 12b */ u8 mNextMode;
    /* 12c */ u8 mCursorX;
    /* 12d */ u8 mCursorY;
    /* 12e */ u8 mUnk_12e;
    /* 12f */ u8 mUnk_12f;
    /* 130 */ u8 mUnk_130;
    /* 131 */ PAD(0x131, 0x132);
    /* 132 */ u8 mUnk_132;
    /* 133 */ u8 mUnk_133;
    /* 134 */ u8 mUnk_134;
    /* 135 */ u8 mFlipped;
    /* 136 */ s8 mSelectedId;
    /* 137 */ PAD(0x137, 0x138);
    /* 138 */ u8 mPromptStep;
    /* 139 */ u8 mOffsetX;
    /* 13a */ u8 mUnk_13a;
    /* 13b */ u8 mReleasedOnCursorCell;
    /* 13c */

    BattleCommand(u32 unk);
    virtual ~BattleCommand();
    /* 08 */ virtual void Update();
    BOOL UpdateCommandWindowAndCursor(BOOL input);
    BOOL SwitchMode();
    void SetCommandMessage();
    void DrawTurnCount();
    void CloseMode(BOOL flag);
    void PollModeClosed();
    void OpenNextMode(u32 flag);
    void Draw();
    void func_ov007_0223a734();
    void RefreshCommandPageOrList();
    void func_ov007_0223b874();
    void InitGridMask();
    void LoadTurnGraphics();
    BOOL UpdatePrompt();
    void UpdateCursorScrollAndCamera(u32 flag);
};

class BattleMapObj;
class BattleMapModel;
class BakuganModel;
class ModelLightGroup;
class BattleStatusPanels;
struct BattleUnk29c;
class BattleMapPiece;

// Battle map of the command screen (vtable 022887e0, 0x158 bytes): the map object, cameras, models of the field,
// the status panels and the objects of the battle.
class BattleCommandMap : public SceneBase {
public:
    /* 0c */ void *mStartCamFile;
    /* 10 */ BattleMapObj *mMapObj;
    /* 14 */ BakuganModel *mUnk_14;
    /* 18 */ void *mUnk_18[12];
    /* 48 */ BattleMapModel *mMarkerModel;
    /* 4c */ BattleMapModel *mCursorModel;
    /* 50 */ BattleMapModel *mUnk_50[4];
    /* 60 */ ModelLightGroup *mLights;
    /* 64 */ BattleMapPiece *mPredictPiece;
    /* 68 */ BattleMapPiece *mLandingPiece;
    /* 6c */ BattleMapPiece *mShadowPiece;
    /* 70 */ BattleStatusPanels *mStatusPanels;
    /* 74 */ BattleUnk29c *mUnk_74;
    /* 78 */ u8 mPredictPositions[0x60];
    /* d8 */ u32 mLandingPosition[3];
    /* e4 */ u32 mCameraEndTarget[3];
    /* f0 */ u32 mCameraEndPosition[3];
    /* fc */ u32 mCameraMoveOffset[3];
    /* 108 */ u32 mUnk_108[3];
    /* 114 */ u32 mVelocity[3];
    /* 120 */ u32 mCornerPosition[3];
    /* 12c */ u32 mStopStepCount;
    /* 130 */ s32 mCameraDistance;
    /* 134 */ float mPredictOffset;
    /* 138 */ s32 mUnk_138;
    /* 13c */ u16 mDirRotY;
    /* 13e */ u16 mDirRotZ;
    /* 140 */ u16 mDirScale;
    /* 142 */ u16 mUnk_142;
    /* 144 */ u16 mCameraMoveTimer;
    /* 146 */ u16 mCameraYawDegrees;
    /* 148 */ u8 mCursorX;
    /* 149 */ u8 mCursorY;
    /* 14a */ u8 mState;
    /* 14b */ u8 mKind;
    /* 14c */ u8 mUnk_14c;
    /* 14d */ u8 mMapModelsEnabled;
    /* 14e */ PAD(0x14e, 0x14f);
    /* 14f */ u8 mPredictVisible;
    /* 150 */ u8 mUnk_150;
    /* 151 */ u8 mUnk_151;
    /* 152 */ u8 mUnk_152;
    /* 153 */ u8 mCursorAndMarkerModelsVisible;
    /* 154 */ u8 mUnk_154;
    /* 155 */ u8 mUnk_155;
    /* 156 */ PAD(0x156, 0x158);
    /* 158 */

    BattleCommandMap(u8 unk);
    virtual ~BattleCommandMap();
    /* 08 */ virtual void Update();
    /* 0c */ virtual void Init();
    void FreeStartCamFile();
    void func_ov007_022455c0();
    void func_ov007_0224560c(u32 index);
    void func_ov007_02245894();
    void func_ov007_022458cc();
    void func_ov007_022458f4();
    void UpdateStartCamPath();
    void UpdateCameraMove();
    void UpdatePredictPositions();
    void func_ov007_02245f08();
    void func_ov007_02246450();
};

// Helpers of the map object implemented in BattleCommandMap.cpp (linkage as defined there).
void CloseCommandMapNow(BattleCommandMap *map);
extern "C" void SetMapModelsEnabled(BattleCommandMap *map, u32 a);
void CreateCursorModel(BattleCommandMap *map);
extern "C" void SetPredictVisible(BattleCommandMap *map, u32 flag);
extern "C" void GetCornerPosition(BattleCommandMap *map, VecFx32 *out);
void GetCellPosition(VecFx32 *out, u8 a, u8 b);
extern "C" u32 FindMarkerCellAt(VecFx32 *v, u8 *outA, u8 *outB);

// Loads the battle_b_bg files of data_020d433c.mFieldId (or the "last" set), then battle_b_obj_00, the union_b cockpit
// and page characters and card_s_000, and sets data_020d433c.mUnk_10's layout 8.
void func_ov007_0223ad70(u32 variant);
