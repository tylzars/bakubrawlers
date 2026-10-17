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

// Values of BattleCommand::mMode and mNextMode: which sub object of the command screen is open. OpenNextMode creates
// the mNextMode object and copies mNextMode into mMode; CloseMode closes the open MAP / PAGE / LIST object, clears
// mContext->mRequest and sets CLOSING; PollModeClosed sets NONE once mContext->mRequest is non-zero again.
#define COMMAND_MODE_NONE    0 // no sub object open
#define COMMAND_MODE_CLOSING 1 // closed by CloseMode, waiting for mContext->mRequest
#define COMMAND_MODE_MAP     2 // mCommandMap (BattleCommandMap)
#define COMMAND_MODE_PAGE    3 // mCommandPage (BattleCommandPage)
#define COMMAND_MODE_LIST    4 // mCommandList (BattleCommandList)

// What BattleCommand hands back when it ends: mUnk_134, passed to RequestScene and so written into the parent
// BattlePlayerList's mContext->mRequest (not a SCENE_* id), which its state 13 switches on.
// SHOOT: a record was picked in the BattleCommandList prompt; the list opens BattleShoot.
// SHOOT_FROM_MSG: the turn player's mMsg (type 8) gave the record; the list opens BattleShoot with the shot in mMsg.
// CPU_SHOOT: the turn player's BattleCpu picked the record; the list opens BattleShoot with the shot it computes.
#define COMMAND_RESULT_SHOOT          1
// NEXT_TURN: the list goes to its state 26 (turn set-up: mTurnCount++ unless mRepeatTurn is set, then mTurnPlayer).
#define COMMAND_RESULT_NEXT_TURN      2
#define COMMAND_RESULT_SHOOT_FROM_MSG 3
#define COMMAND_RESULT_CPU_SHOOT      4
#define COMMAND_RESULT_BATTLE_RESULT  5 // func_ov007_02263150 held; the list opens BattleResult
#define COMMAND_RESULT_NO_MARKERS     6 // no marker has a non-zero mStatus; the list opens BattleObj00(1)
// 7 (state 50: BattleUnk0227532c returned 2 and data_020d433c.mWireless is 0) is left a literal: the list goes to
// its state 36 (also entered from its state 17), which requests SCENE_MAP or, in wireless mode or when mNextScene is
// SCENE_ARENA_WINDOW, goes to its state 32.

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
    /* 3c */ CellAnimation *mUnitCellAnims[12]; // [k] drawn for the k-th occupied cell each frame, frame = mPlayer
    /* 6c */ SelectWindow *mCommandWindow;
    /* 70 */ SelectWindow *mCockpitWindow;
    /* 74 */ SelectWindow *mPromptWindow;
    /* 78 */ BattleCommandPage *mCommandPage;
    /* 7c */ BattleCommandList *mCommandList;
    /* 80 */ u32 mCamera; // a Camera * held as u32 (BattlePlayerList::mCamera), cast back at each use
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
    /* 12a */ u8 mMode; // COMMAND_MODE_*
    /* 12b */ u8 mNextMode; // COMMAND_MODE_* that OpenNextMode opens
    /* 12c */ u8 mCursorX;
    /* 12d */ u8 mCursorY;
    /* 12e */ u8 mUnk_12e;
    /* 12f */ u8 mUnk_12f;
    /* 130 */ u8 mUnk_130;
    /* 131 */ PAD(0x131, 0x132);
    /* 132 */ u8 mUnk_132;
    /* 133 */ u8 mUnk_133;
    /* 134 */ u8 mUnk_134; // COMMAND_RESULT_* (or 7), requested when the screen ends
    /* 135 */ u8 mFlipped; // both axes (13 - x, 9 - y) and corner sides + 2; also the BG variant of func_ov007_0223ad70
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
    // The window steps when `input` or it is not in state 1; the cursor moves only with `input`; TRUE = `input` and a
    // selection. Also draws the screen and, in COMMAND_MODE_MAP, moves the map camera to the cursor cell.
    BOOL UpdateCommandWindowAndCursor(BOOL input);
    // polled after CloseMode: TRUE once the close has ended and OpenNextMode has run; the page and the list are then
    // filled from the cursor cell
    BOOL SwitchMode();
    // StartMessage(1, or 0x20 when list mUnk_292 is 1) for the local turn player (mTurnPlayer == mUnk_2a9), else 0x33.
    // Other messages are started inline.
    void SetCommandMessage();
    void DrawTurnCount();
    void CloseMode(BOOL flag);
    void PollModeClosed(); // in COMMAND_MODE_CLOSING any non-zero mContext->mRequest ends the close
    void OpenNextMode(u32 flag);
    void Draw(); // the cell highlights (func_ov007_0223b874) and mCockpitWindow are drawn by the callers
    void func_ov007_0223a734();
    void RefreshCommandPageOrList();
    void func_ov007_0223b874();
    void InitGridMask();
    void LoadTurnGraphics();
    BOOL UpdatePrompt();
    // In COMMAND_MODE_MAP also turns the map camera one degree per frame while 0x200 or 0x100 is held, and with the
    // other pressed too calls func_ov007_02246a84(map, 2) (the camera SwitchMode sets up); that call returns early,
    // without sound 5 and the refresh.
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
    /* 50 */ BattleMapModel *mUnk_50[4]; // F00GAT models at cells (one or one per player); freed by 022455c0 / 0224560c
    /* 60 */ ModelLightGroup *mLights; // created and deleted only; never applied by this class
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
    /* 148 */ u8 mCursorX; // with mCursorY: written by SetCursorCellAndMoveCamera and BattleTarget::func_ov007_0225b69c
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
    void func_ov007_022455c0(); // deletes and clears the four mUnk_50 models
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
extern "C" void GetCornerPosition(BattleCommandMap *map, VecFx32 *out); // *out unchanged for mKind >= 8
void GetCellPosition(VecFx32 *out, u8 a, u8 b);
extern "C" u32 FindMarkerCellAt(VecFx32 *v, u8 *outA, u8 *outB);

// Loads the battle_b_bg files of data_020d433c.mFieldId (or the "last" set), then battle_b_obj_00, the union_b cockpit
// and page characters and card_s_000, and sets data_020d433c.mUnk_10's layout 8.
void func_ov007_0223ad70(u32 variant);
