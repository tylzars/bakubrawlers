#pragma once

#include "global.h"
#include "types.h"

#include <nitro/fx.h>

#include "Scene/SceneBase.hpp"
#include "UI/MessageWindow.hpp"
#include "Graphics/CharacterIcons.hpp"

class CellAnimation;
struct BattleData;
struct BattleEntry;
struct BattleParam;
class BattleSideHelper;

// Generic polymorphic object owned by the battle scene (deleted through vtable slot 1).
class BattleObject {
public:
    virtual ~BattleObject() {}
};

// Battle modes: BattleConfrontCard::mBattleMode, passed to BattleBase::Init (BattleBase::mMode, the gBattleParams
// row) and to each BattleSlot (BattleSlot::mMode). BattleConfrontCard creates the battle screen class of the mode.
#define BATTLE_MODE_CONFRONT_BG01 0 // ConfrontBg01
#define BATTLE_MODE_CONFRONT_BG02 1 // ConfrontBg02
#define BATTLE_MODE_CONFRONT_03   2 // Confront03
#define BATTLE_MODE_CONFRONT_BG04 3 // ConfrontBg04
#define BATTLE_MODE_CONFRONT_BG05 4 // ConfrontBg05
#define BATTLE_MODE_CONFRONT_BG06 5 // ConfrontBg06
// Not a screen: BattleBase::Init stores it in mMode (after choosing mParam) when BattlePlayerList::mLocalPlayer is
// neither entry's player (mEntries[0/1].mPlayer), the case where IsSpectator() holds; only BattleHud::StartObject
// reads it.
#define BATTLE_MODE_SPECTATOR     7

// Per-player helper object (0x38 bytes, created by func_ov007_022227e4).
class BattleSlot : public BattleObject {
public:
    /* 04 */ u8 mMode; // BATTLE_MODE_* (0..5)
    /* 05 */ u8 mCharacterId;
    /* 06 */ u8 mStartTimer; // 50 in BATTLE_MODE_CONFRONT_BG02 only (roles 2 / 3); Update waits while non-zero
    /* 07 */ u8 mSidePlayerValue;
    /* 08 */ u16 mIntervalFrames;
    // re-rolled at each end: mModeRow[0] + BattleRand32(mModeRow[1]) for roles 2 / 3, the same from mIntervalRow for
    // the other roles
    /* 0a */ u16 mIntervalFrameCount;
    /* 0c */ s16 mValue;
    // +1 per ComputeValue call (mRole 2 / 3); at mParam->mValueFrameCount the mMode step runs
    /* 0e */ u16 mValueFrames;
    /* 10 */ u16 mRollFrames; // +1 per CONFRONT_BG06 step of ComputeValue (not per frame); rolls at mRollFrameCount
    /* 12 */ u16 mRollFrameCount;
    union {
        /* 14 */ u8 mUnk_14;
        struct {
            /* 14 */ u8 mExtraEnabled : 1;
            /* 14 */ u8 mExtraRunning : 1;
            /* 14 */ u8 mBothEntriesCpu : 1; // set in BG01 / BG04 only
        };
    };
    /* 15 */ u8 mBelowHalfFrames;
    /* 16 */ u8 mExtraTimer;
    /* 17 */ u8 mIndex;
    /* 18 */ int mRole;
    /* 1c */ u16 mCardDelayTimer;
    /* 1e */ u8 mSoundTimer; // counted per recompute (mValueFrames >= mParam->mValueFrameCount), read in BG05 only
    /* 20 */ BattleData *mData;
    /* 24 */ u8 *mModeRow;
    /* 28 */ u8 *mIntervalRow;
    /* 2c */ u8 *mExtraRow;
    /* 30 */ BattleParam *mParam;
    /* 34 */ BattleSideHelper *mHelper;
    /* 38 */

    BattleSlot(BattleData *data, u8 index, u8 mode, u8 unk4, u8 unk5, u8 unk6, BattleParam *param, u8 sidePlayerValue);
    ~BattleSlot();
    int Update(u16 *a, u16 *b, u16 *c, s8 (*sidePlayers)[2], u8 n, u8 *d); // 1 when mHelper->RollForCard fired, else -1
    BOOL UpdateInterval(); // TRUE every call in BG01 / BG04-BG06; in BG02 / CONFRONT_03 when the interval ends
    int ComputeValue(u8 n, u16 *a, u16 *b); // 0 between recomputes
    u32 UpdateExtraValue(u16 value, u8 n);
    // Sets mCardDelayTimer on the slot across from `sel`; at 0 Update calls func_ov007_0223e214(mData, other, 0)
    // (in wireless mode it writes other + 1 to *d instead).
    void StartCardDelay(int sel);
};

u32 func_ov007_022234f0(int player, u8 mode, u8 ch);

// Per-side helper object (0x14 bytes, created by func_ov007_02223534).
class BattleSideHelper : public BattleObject {
public:
    /* 04 */ u8 mRollEnabled; // one-shot: set by the ctor for a card of kind 0-3, cleared by the first roll that hits
    /* 05 */ u8 mPeriodFrames; // calls (one per frame) while mQuarter stays the same
    /* 06 */ u8 mQuarter; // n / 25 of the last call, n mirrored (100 - n) for side 1
    /* 07 */ u8 mRollFrames; // calls since the last roll attempt
    /* 08 */ u16 mQuarterPeriodCount;
    /* 0a */ PAD(0x0a, 0x0c);
    /* 0c */ u8 mPeriodFrameCount;
    /* 0d */ u8 mPeriodChance;
    /* 0e */ u16 mRollFrameCount;
    /* 10 */ u8 mQuarterChances[4]; // base chance per quarter (0: no roll), + mQuarterPeriodCount * mPeriodChance
    /* 14 */

    BattleSideHelper(u8 index, u8 unk2, BattleEntry *entries);
    virtual ~BattleSideHelper();
    BOOL RollForCard(int side, u8 n);
};

// Interface base with vtable 02286c5c (slot 0: empty, slot 1: delete this).
class IBattle {
public:
    IBattle();
    virtual ~IBattle();
    /* 08 */ virtual void func_ov007_0221cebc(int a) = 0;
    /* 0c */ virtual void RollHelpersForCard(u16 *a, u16 *b, u16 *c) = 0;
    /* 10 */ virtual void ApplySidePlayersFromMessages(u16 *a, u16 *b) = 0;
    /* 14 */ virtual int UpdateSlotsFromMessages(u16 *a, u16 *b, u16 *c, u8 *d) = 0;
    // Slot 18: forwards to TryStartCardSequence with the array ApplyNetMessages built (UpdateSlotsFromMessages runs first
    // and its CPU slots can also set entries of it).
    /* 18 */ virtual void TryStartCardSequenceFromMessages(u16 *p) = 0;
    /* 1c */ virtual void PushSideFromMessages(u8 a, u16 *counts) = 0;
};

// One 0x14-byte player/slot record inside BattleData (array starts at +0x0c).
struct BattleEntry {
    /* 00 */ u16 mUnk_00; // power value compared by the gauge
    /* 02 */ u16 mUnk_02;
    /* 04 */ u16 mUnk_04;
    /* 06 */ u16 mUnk_06;
    /* 08 */ u16 mUnk_08;
    /* 0a */ u16 mUnk_0a;
    /* 0c */ u8 mBakuganId;
    /* 0d */ u8 mAttribute : 4; // the record's mAttribute (BattleData ctor)
    /* 0d */ u8 mPlayer : 4; // the unit's mPlayer: index into BattlePlayerList::mPlayers
    /* 0e */ u8 mCharacterId;
    /* 0f */ u8 mSkipCardPick : 1;
    /* 0f */ u8 mCardRemoved : 1;
    /* 0f */ u8 mKind3CardRemoved : 1;
    /* 0f */ u8 mUnk_0f_3 : 1;
    /* 0f */ u8 mUnk_0f_4 : 1;
    /* 0f */ u8 mNextBattleRequested : 1;
    /* 0f */ u8 mPushedToEnd : 1;
    /* 0f */ u8 mUnk_0f_7 : 1;
    /* 10 */ u8 mUnk_10;
    /* 11 */ u8 mSlot6cIndex; // kept when the pick returns 0xff (no pick)
    /* 12 */ u16 mCardId; // 0 = no card
    /* 14 */
};

// Data of a pairing of two bakugan about to battle (vtable 022881e8): per-side entries, the animations of both
// sides and the windows that show them.
struct BattleData {
    /* 00 (vtable) */
    /* 04 */ u16 mMarkerCardId;
    /* 06 */ u8 mMarkerPlayer;
    /* 07 */ u8 mNextBattleRequest;
    /* 08 */ u8 mUnk_08[2];
    /* 0a */ u8 mUnk_0a;
    /* 0b */ u8 mUnk_0b;
    /* 0c */ BattleEntry mEntries[2];
    /* 34 */ CellAnimation *mCellAnims[14];
    /* 6c */ MessageWindow *mMessageWindow;
    /* 70 */ CharacterIcons *mCharacterIcons;
    /* 74 */

    BattleData();
    virtual ~BattleData();
    BattleEntry *Entries() { return mEntries; }
    void Load();
    void DrawCharacterIcons(u8 side, s16 x, s16 y); // icon_ch_base cells; mCharacterIcons only gets the position here
    BOOL func_ov007_0223dd6c();
    BOOL func_ov007_0223ddac(u8 side);
    void DrawBakuganAndNumber(u8 side, s16 x, s16 y, u8 flag, u16 value); // value 0xffff: the entry's mUnk_00
    // kind 0 / 1 / 2: the entry's mUnk_06 / mUnk_08 / mUnk_0a (nothing drawn for 0); mode 0 offsets by a - b * 2 / 3.
    void DrawEntryNumber(u8 side, u8 kind, s32 x, s16 y, u8 flag, u32 mode, u16 a, u16 b);
    void func_ov007_0223e180();
    void SetNextBattleRequest(u32 request, u8 side);
    void RemoveCard(u8 side, u8 kind);
};

// 12-byte per-mode parameter record (table at 0x02278b20).
struct BattleParam {
    /* 00 */ u8 mKind;
    /* 01 */ u8 mCutinDelayFrameCount; // frames after the mTouched request before the cut-in cell plays animation 2
    /* 02 */ u16 mBattleFrameCount;
    /* 04 */ u32 mPushScale;
    /* 08 */ u8 mValueFrameCount;
    /* 09 */ u8 mDefaultObjectCount;
    /* 0a */ u8 mObjectCountPercent;
    /* 0b */ u8 mTouchObjectFrameCount;
    /* 0c */
};
extern BattleParam gBattleParams[];

class CellAnimation;
class BattleObj01;

// Confrontation screen overlay (0x80 bytes): the two players' emblems and gauges on the main screen.
class BattleSub48 : public SceneBase {
public:
    /* 00 SceneBase */
    /* 0c */ u8 mState; // 0 nothing drawn, 1 DrawCircleAndSides, 2 the abilities slide up (0xc0 to 0x8c), 3 DrawAbilitiesAndSides, 4 finished
    /* 0d */ u8 mMoveStep; // step of UpdateCircleAndSidesMove; never reset
    union {
        /* 0e */ u8 mUnk_0e;
        struct {
            /* 0e */ u8 mUnk_0e_0 : 1;
            /* 0e */ u8 mCircleUpDownVisible : 1;
            /* 0e */ u8 mSideUpDownVisible : 1;
            /* 0e */ u8 mNoEmblem : 1; // set by the ctor for mFieldId 0 or 7 (no vs_emblem file is loaded for them)
            /* 0e */ u8 mUnk_0e_4 : 1;
        };
    };
    /* 0f */ u8 mFieldIndex;
    /* 10 */ PAD(0x10, 0x12);
    /* 12 */ u8 mSideAttributes[2];
    /* 14 */ u16 mStepFrames; // frames in mUnk_0d steps 1 (to 0x78) and 3 (to 0x3c); not counted in step 2
    // 90, set only when gGameData.mFieldId is 1-6 (otherwise mState never becomes 1, the state DrawCircleAndSides
    // runs in); step 2 raises it by 3 to 250
    /* 16 */ u16 mCircleY;
    /* 18 */ float mPos[2][2];
    /* 28 */ float mVel[2][2];
    /* 38 */ s16 mAbilitiesY;
    /* 3a */ s16 mAbilitiesYPerFrame; // starts at 10, shrinks by 2 per state-2 frame down to 2
    // 0 / 1: this console plays entry 0 / 1; 2..4 neither entry (copy of BattleBase::mLocalSide, the ctor mode)
    /* 3c */ int mLocalSide;
    /* 40 */ BattleData *mData;
    /* 44 */ CellAnimation *mCellAnims[15];
    /* 80 */

    BattleSub48(BattleData *data, u8 mode);
    virtual ~BattleSub48();
    virtual void Update();
    void LoadCharactersAndPalettes();
    void LoadGraphics();
    u8 UpdateCircleAndSidesMove();
    void DrawCircleAndSides(); // cells on circles (radius 70 / 50) around the emblem (0x6e, mCircleY), then DrawSides
    void DrawSides();
    void DrawAbilitiesAndSides(); // returns at once while mAbilitiesY >= 0xc0; skips DrawSides when mNoEmblem
};

class Camera;
class ModelInstance;
class ModelAnim;
class ModelLightGroup;
class EffectManager;

class BattleMapObjPart;
class TextureFile;
struct BattleMapObjTable;

// Kinds of a BattleMapObj with their own branch in Load (mKind). Other kinds load the row mKind of gMapObjKindTables
// (BattleCommandMap passes its mKind, which its constructors set to gGameData.mFieldId).
#define MAP_OBJ_KIND_THROWFIELD00    9  // "throwfield00" model; Draw places it with gThrowfield00Scale / Offset
#define MAP_OBJ_KIND_FIELD_TABLE     10 // the row gGameData.mFieldId of gMapObjFieldTables
#define MAP_OBJ_KIND_THROWFIELD01    11 // "throwfield01" model
#define MAP_OBJ_KIND_REALPOP         12 // "realpop" model
#define MAP_OBJ_KIND_COMMANDFILED0   13 // "commandfiled0" model

// Helper of the battle map scene (0x24 bytes): ctor 0x0224916c, destructor 0x02249198 (non-virtual).
// Loads the 3D model parts of the map field of the given kind (MAP_OBJ_KIND_*; other
// kinds use the table gMapObjKindTables).
class BattleMapObj {
public:
    /* 00 */ BattleMapObjPart *mParts[8];
    /* 20 */ u8 mKind; // kind
    /* 21 */ u8 mCount;  // used entries of mParts
    /* 22 */ PAD(0x22, 0x24);
    /* 24 */

    BattleMapObj(int unk1);
    ~BattleMapObj();
    void AddPart(const char *nsbtx, const char *nsbmd, const char *data, u32 flag);
    void AddPartWithTexture(TextureFile *texture, const char *nsbmd, const char *data, u32 flag); // on a shared texture
    void AddTableParts(const BattleMapObjTable *table); // all parts share one texture, entry 0's .nsbtx
    void Load();
    void Draw();
};

// A Real Bakugan model (0x10 bytes, ctor 0x0227430c), deleted through its vtable.
class BattleRealBaku : public BattleObject {
public:
    /* 04 */ u8 mId;
    /* 05 */ u8 mAnimDataIndex;
    union {
        /* 06 */ u8 mUnk_06;
        struct {
            /* 06 */ u8 mVariant : 3; // variant number in the file names
            /* 06 */ u8 mAnimLoop : 1;
            /* 06 */ u8 mUseBoxTest : 1; // copied to mModel->mUseBoxTest; all three calls pass 0
            /* 06 */ u8 mUseCallbackMtx : 1;
            /* 06 */ u8 mUseRealb20 : 1;
        };
    };
    /* 07 */ PAD(0x07, 0x08);
    /* 08 */ ModelInstance *mModel;
    /* 0c */ ModelAnim *mAnim;
    /* 10 */

    BattleRealBaku(u8 id, int unk2, u8 unk3, int unk4, int unk5, int unk6, int unk7);
    virtual ~BattleRealBaku();
    void LoadModel();
};

// Rows of model positions of the 3D battle field (12 bytes, table at 0x022791ec).
struct BattleCamRow {
    /* 00 */ s32 mX;
    /* 04 */ s32 mY;
    /* 08 */ s32 mZ;
    /* 0c */
};

// 3D side of the battle (0x54 bytes, formerly BattleField3D): the camera animation of the map (commandfiled0.cam),
// the two Real Bakugan models and the two "aura%02d" effects. Deleted through its vtable.
class BattleField3D : public BattleObject {
public:
    union {
        /* 04 */ u8 mUnk_04;
        struct {
            /* 04 */ u8 mFollowEnabled : 1;
            /* 04 */ u8 mSide0ModelHidden : 1;
            /* 04 */ u8 mSide1ModelHidden : 1;
        };
    };
    /* 05 */ PAD(0x05, 0x08);
    /* 08 */ s16 mAura[2]; // effect handles, -1 when none
    /* 0c */ s32 mSide0EndX;
    /* 10 */ s32 mSide1EndX;
    /* 14 */ s32 mAuraX; // x of aura 0; aura 1 sits at mAuraX + 0xccd
    /* 18 */ s32 mFollowOffset;
    /* 1c */ VecFx32 mPos[2];
    // 10240 at start, - 1229 per call; once negative the models move the other way, and below -4096 it becomes 0, which
    // ends the move
    /* 34 */ float mMovePerFrame;
    /* 38 */ BattleMapObj *mMapObj;
    /* 3c */ ModelLightGroup *mLights; // created and deleted only; never applied by this class
    /* 40 */ void *mCamFile;
    /* 44 */ EffectManager *mEffects;
    /* 48 */ BattleRealBaku *mSide0RealBaku;
    /* 4c */ BattleRealBaku *mSide1RealBaku;
    /* 50 */ BattleData *mData;
    /* 54 */

    BattleField3D(BattleData *data, int unk2);
    virtual ~BattleField3D();
    void MoveModels(float a, float b); // subtracts a from model 0's x and b from model 1's x
    void CreateAuras();
    void FollowPushPercent(u32 percent); // percent clamped to 0x14..0x50: the models and auras track only that range
    void Draw(u32 percent); // FollowPushPercent first when mFollowEnabled, then the two models and mMapObj
};

// Arguments of SetHudSequence (BattleHud.cpp): each starts a run of BattleHud::mState steps that ends back at mState 0
// (IsHudSequenceRunning reads mState != 0). The argument is not the mState value it sets (0, 1, 4, 6, 8).
#define HUD_SEQUENCE_NONE               0 // mState 0: Update runs no step, only Draw
#define HUD_SEQUENCE_BAR_FILL_COUNTDOWN 1 // mState 1: tutorial pages in some tutorials, then SetupCountdown, and each
                                          // frame mBarFillPerFrame added to mBarFill with the countdown animation
                                          // (UpdateBarFillCountdown)
#define HUD_SEQUENCE_SWAP               2 // mState 4: StartSwap / UpdateSwap
#define HUD_SEQUENCE_BAR_SLIDE          3 // mState 6: StartBarSlide / UpdateBarSlide
#define HUD_SEQUENCE_COUNTDOWN          4 // mState 8: StartCountdown / UpdateCountdown, the countdown animation only

// Battle HUD scene on the sub screen (0x130 bytes, formerly BattleHud): the power gauge that tugs between the two
// players, the countdown, the small card images and the cut-in animations.
class BattleHud : public SceneBase {
public:
    /* 00 SceneBase */
    /* 0c */ int mLocalSide; // 0/1: this console plays entry 0/1; 2: spectator with no side; 3/4: partner on side 0/1 (>= 2: spectator)
    /* 10 */ u16 mSlideSidesOutDistance;
    /* 12 */ u8 mUnk_12;
    /* 13 */ u8 mPushPercent; // gauge position in percent
    /* 14 */ s32 mPushPercentFixed; // same, 20.12 fixed point
    /* 18 */ s16 mSide0X;
    /* 1a */ s16 mSide1X;
    /* 1c */ u16 mPushAnimTimer[2];
    /* 20 */ u16 mCutinDelayTimer;
    union {
        /* 22 */ u8 mUnk_22;
        struct {
            /* 22 */ u8 mFrame5SoundPlayed : 1;
            /* 22 */ u8 mFrame15SoundPlayed : 1;
            /* 22 */ u8 mFrame25SoundPlayed : 1;
            /* 22 */ u8 mBarFillDone : 1;
            /* 22 */ u8 mCutinVisible : 1;
            /* 22 */ u8 mPerfectCutinVisible : 1;
            /* 22 */ u8 mSidesVisible : 1;
            /* 22 */ u8 mSlideSidesOutEnabled : 1;
        };
    };
    union {
        /* 23 */ u8 mUnk_23;
        struct {
            /* 23 */ u8 mSwapSide : 1;
            /* 23 */ u8 mClosed : 1;
            /* 23 */ u8 mTouched : 1;
        };
    };
    /* 24 */ u16 mPenToggleFrames;
    /* 26 */ u8 mSide0BarEndHidden;
    /* 27 */ u8 mSide1BarEndHidden;
    /* 28 */ s32 mLastPush[2]; // latest Push delta per side (20.12); cleared after the marker animation reads it
    /* 30 */ CellAnimation *mCellAnims[23];
    /* 8c */ BattleData *mData;
    /* 90 */ BattleParam *mParam;
    // per side: up to two player indices (BattlePlayerList::mPlayers) other than the two entries' players, filled by
    // BattleBase::ApplySidePlayersFromMessages, BattleSlot::Update (through UpdateSlots) and the spectator touch path;
    // -1 = none. Points at BattleBase::mSide0FirstPlayer..mSide1SecondPlayer
    /* 94 */ s8 (*mSidePlayers)[2];
    /* 98 */ u32 mCountdownFrames; // also counts UpdateSwap's 20 push frames
    /* 9c */ u16 mCountdownFrameCount;
    /* 9e */ PAD(0x9e, 0x9f);
    /* 9f */ u8 mStartPushPercent;
    /* a0 */ float mBarFillPerFrame[2]; // also the per-frame Push amount of the swap (StartSwap / UpdateSwap)
    /* a8 */ float mBarFill[2];
    /* b0 */ float mDrawnValues[2];
    /* b8 */ float mDrawnValuesPerFrame[2];
    /* c0 */ u16 mUnk_c0;
    /* c2 */ PAD(0xc2, 0xc3);
    /* c3 */ u8 mUnk_c3;
    /* c4 */ BattleObj01 *mObjs[15];
    /* 100 */ u8 mObjectsRemaining[2];
    /* 102 */ u8 mObjectIntervalFrameCount[2];
    /* 104 */ u8 mObjectIntervalFrames[2];
    /* 106 */ PAD(0x106, 0x108);
    /* 108 */ float mSide0PlayerAnimX;
    /* 10c */ float mSide1PlayerAnimX;
    /* 110 */ float mPlayerAnimXPerFrame; // 96 / mParam->mBattleFrameCount: the cells meet at x 128 at the time limit
    /* 114 */ u8 mShakeSign; // shared by the bakugan shake and the large-card shake
    /* 115 */ u8 mShakeFrames;
    /* 116 */ u8 mShakesRemaining;
    union {
        /* 117 */ u8 mUnk_117;
        struct {
            /* 117 */ u8 mUnk_117_0 : 1;
            /* 117 */ u8 mUnk_117_1 : 1;
            /* 117 */ u8 mCardSequenceRunning : 1; // stays set when the sequence ends the battle through state 12
            /* 117 */ u8 mPushAnimPaused : 1;
        };
    };
    /* 118 */ u8 mUnk_118[2];
    /* 11a */ u8 mAbility0SlideState; // with mAbility1SlideState: 0 idle, 1 in, 2 out, 3 across
    /* 11b */ u8 mAbility1SlideState;
    /* 11c */ s16 mAbilityX[2];
    /* 120 */ s16 mAbilityXPerFrame; // one speed for both cells: 20 at a start, - 1 per frame down to 4
    /* 122 */ u8 mUnk_122[2];
    /* 124 */ s16 mLargeCardYPerFrame;
    /* 126 */ s16 mLargeCardY;
    /* 128 */ u8 mLargeCardOpen; // stays 1 while closing (mLargeCardClosing) until the close slide ends
    /* 129 */ u8 mLargeCardClosing;
    /* 12a */ PAD(0x12a, 0x12b);
    /* 12b */ u8 mState; // state of Update
    /* 12c */ s16 mPenX; // negative: the pen is not drawn
    /* 12e */ s16 mPenY;
    /* 130 */

    BattleHud(BattleData *data, BattleParam *param, u8 mode, s8 *sidePlayers);
    virtual ~BattleHud();
    virtual void Update();
    void Init();
    void LoadSmallCard(u8 side, int card); // card_s_000 instead unless the side's player is the list's mLocalPlayer
    void LoadCountdown();
    // Computes mStartPushPercent from the two entries' mUnk_00, the per-frame rates over mCountdownFrameCount (the
    // countdown cell's length) and mUnk_20 = mCutinDelayFrameCount; StartCountdown, not this, restarts the cell.
    void SetupCountdown();
    BOOL UpdateBarFillCountdown();
    void StartSwap(); // swaps the two entries' mUnk_00 at once and calls StartCountdown
    BOOL UpdateSwap(); // pushes for 20 calls; returns UpdateCountdown's done flag
    void StartBarSlide(); // also CloseLargeCard when mLargeCardOpen
    // side 0's bar grows to 1.0 when mEntries[0].mUnk_10 is set, else side 1's; TRUE when mSide0BarEndHidden and
    // mSide1BarEndHidden are 1 (both bar-end cells hidden)
    BOOL UpdateBarSlide();
    void StartCountdown();
    BOOL UpdateCountdown(); // TRUE once the cell finished; then sets mUnk_117_0 when mLargeCardOpen
    BOOL SlideSidesOut(); // mUnk_0c 2 moves neither side, 3 only side 1, 4 only side 0; the caller ignores the result
    void func_ov007_02227460(int side);
    void Push(int mode, s32 value, s32 count, u8 boost, bool useLevel);
    u8 ComputeObjectCount(u32 value, u8 a, u8 b);
    BOOL StartObject(u8 side, s16 x, s16 y, u8 flag); // flag: BattleBase::mMode, or 6 from BattleHud itself
    void Draw();
    void func_ov007_02227bbc(u8 index);
    void DrawBarsAndObjects();
    void StartAbilitySlideIn(int side);
    void StartAbilitySlideOut(int side);
    void StartAbilitySlideAcross(int side); // side's cell slides out (2); the other side's cell slides across (3)
    BOOL IsAbilitySlideDone();
    void OpenLargeCard(int card);
    void CloseLargeCard();
    void DrawAbilitiesAndCards();
};

// Battle scene base: IBattle at +0, SceneBase at +4. Derived classes are the per-field battle screens.
class BattleBase : public IBattle, public SceneBase {
public:
    /* 00 IBattle */
    /* 04 SceneBase */
    /* 10 */ u8 mState;
    /* 11 */ u8 mStep;
    /* 12 */ u8 mLocalSide;
    /* 13 */ u8 mSidePlayerValue;
    /* 14 */ s8 mMultiplierIndex; // row of gAttributeRates (mFieldId - 1); -1 (fields 0, 7) = factor 0x1000 (1.0)
    /* 15 */ u8 mMode; // BATTLE_MODE_*
    /* 16 */ u16 mElapsedFrames;
    /* 18 */ u8 mBattleEnded : 1;
    /* 18 */ u8 mTouched : 1;
    /* 18 */ u8 mFlashEnabled : 1;
    /* 19 */ u8 mSpectatorTouchSide;
    /* 1a */ u8 mSpectatorTouchTimer;
    /* 1b */ u8 mSpectatorHoldFrames;
    /* 1c */ s8 mSide0FirstPlayer;
    /* 1d */ s8 mSide0SecondPlayer;
    /* 1e */ s8 mSide1FirstPlayer;
    /* 1f */ s8 mSide1SecondPlayer;
    /* 20 */ BattleParam *mParam;
    /* 24 */ u8 mTouchValueFrames;
    /* 25 */ u8 mTouchObjectFrames;
    /* 26 */ s16 mFlashBrightness;
    /* 28 */ u8 mFlashFalling;
    /* 29 */ u8 mRateFloorSide;
    /* 2a */ u16 mUnk_2a; // counter: up to 0xf0 (card step 5) / 0xa0 (12), down from 0x40 (15); 0x3c, 0x64 in others
    /* 2c */ BattleData *mBattleData;
    /* 30 */ BattleSlot *mSlots[4];
    /* 40 */ BattleField3D *mField3D;
    /* 44 */ BattleHud *mHud;
    /* 48 */ BattleSub48 *mSub48;
    /* 4c */ u8 mReturnState; // only ever 5 in this build (states 3 and 4)
    /* 4d */ u8 mCardStep;
    /* 4e */ u8 mCardSide;
    // GetCardInfo(card, 3); set to 4 from 0 after the card sequence when mHud->mPushPercent is 0x32, and from 3 in card
    // step 9
    /* 4f */ u8 mCardKind;
    /* 50 */ u8 mPrevCardKind;
    /* 51 */ u8 mBlendAlpha;
    /* 52 */ u8 mUnk_52[2];
    union {
        /* 54 */ u8 mUnk_54;
        struct {
            // read at the state end (mState = mReturnState); also switches card-sequence branches
            /* 54 */ u8 mReturnPending : 1;
            /* 54 */ u8 mBothSides : 1;
            /* 54 */ u8 mUnk_54_2 : 1;
            /* 54 */ u8 mSkipDraw : 1;
            /* 54 */ u8 mSlideDoneSoundPending : 1;
            /* 54 */ u8 mUnk_54_5 : 1;
        };
    };
    /* 55 */ u8 mLargeCardPressesRemaining;
    /* 56 */ u16 mCardId;
    /* 58 */ u32 mCommandPage; // a BattleCommandPage * held as u32
    /* 5c */ u32 mCardSpec;
    /* 60 */ BattleSideHelper *mHelpers[2];
    /* 68 */

    // Players 2 and 3 only watch (no touch input). Kept as BOOL: callers test it as a byte.
    BOOL IsSpectator() const {
        return mLocalSide >= 2;
    }

    BattleBase();
    virtual ~BattleBase();
    /* 08 */ virtual void func_ov007_0221cebc(int a);
    /* 0c */ virtual void RollHelpersForCard(u16 *a, u16 *b, u16 *c);
    /* 10 */ virtual void ApplySidePlayersFromMessages(u16 *a, u16 *b);
    /* 14 */ virtual int UpdateSlotsFromMessages(u16 *a, u16 *b, u16 *c, u8 *d);
    /* 18 */ virtual void TryStartCardSequenceFromMessages(u16 *p);
    /* 1c */ virtual void PushSideFromMessages(u8 a, u16 *counts);
    /* 20 */ virtual void Update();
    /* 24 */ virtual void Init(BattleData *data, u32 mode);
    /* 28 */ virtual void LoadGraphics() = 0;
    /* 2c */ virtual void InitSubScreen();
    /* 30 */ virtual BOOL func_ov007_0221bd84();
    /* 34 */ virtual void UpdateEmblems() = 0; // state 7 only; empty for ConfrontBg01 / ConfrontBg04 (no emblems)
    /* 38 */ virtual BOOL func_ov007_0221bfa4();
    /* 3c */ virtual void Draw() = 0; // skipped while mSkipDraw; ConfrontBg01 / ConfrontBg04 draw no cells
    // The side's amount (0 = none); called for the local player's side (mLocalSide, from BattlePlayerList::mLocalPlayer) while
    // its c[] is 0; low: that side's push percent is below 0x32.
    /* 40 */ virtual u8 UpdateTouch(int low) = 0;
    /* 44 */ virtual void vfunc_44(int index, u16 count) = 0;

    BOOL RaiseBlendAlpha(); // sub engine: G2S_SetBlendAlpha(4, 0x1b, 0, ++mBlendAlpha); TRUE at 0x1f
    void SetPenPosition(s16 a, s16 b); // mHud->mPenX / Y; a negative x hides the pen
    void SetTouched();
    void SetTouchFramesToLimits();
    void TryStartCardSequence(u16 *p);
    int UpdateSlots(u16 *a, u16 *b, u16 *c, u8 *d); // CPU players' slots up to the first != -1; returns -1
    // VRAM banks of both engines (SetupMode(0, 7)), then the main engine's mode and BG priorities.
    void InitMainScreen();
    void UpdatePush();
    int UpdateLocalPlayer(u16 *a, u16 *b, u16 *c, u8 *flag);
    // Master brightness of both screens up by 3 to 15, then down to 0; at the peak with mBattleEnded also FreeAuras,
    // seq 0x25 and ClearSub48State.
    BOOL UpdateFlash();
    void SetupCardSequence();
    void UpdateCardSequenceReturn();
    // Also counts mElapsedFrames; on an end writes the BattleData entries' mUnk_10 / mPushedToEnd (and mHud->mUnk_22 |=
    // 0x20 at either end of the push), then func_ov007_0222847c, mBattleEnded = 1 and StopSeqArc 0x24; otherwise
    // MovePlayerAnimX while mBattleEnded is clear.
    BOOL CheckBattleEnd();
    BOOL UpdateCardSequence();
};

BOOL func_ov007_0221b998(int unk1, int mode, BattleEntry *entry);
BOOL func_ov007_0221b9d0(int unk1, int mode, BattleEntry *entry);
BOOL func_ov007_0221ba0c(int unk1, int mode, BattleEntry *entry);
BOOL func_ov007_0221ba38(int unk1, u8 side, int value, BattleEntry *entries);
int GetNextBattleRequest(int unk1, int index, BattleEntry *entries);
void func_ov007_0223e214(BattleData *data, int side, u8 bit); // sets mEntries[side].mUnk_0f_3
u32 BattleRand32(u32 max);
int func_ov007_02222764(int side, BattleEntry *entry);
