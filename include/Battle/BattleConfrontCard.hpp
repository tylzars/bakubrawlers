#pragma once

#include "global.h"
#include "types.h"

#include "Battle/BattleBase.hpp"
#include "Scene/SceneBase.hpp"

class CellAnimation;
class SelectWindow;
class BattleCommandPage;
class BattleCardCamera;
class BattleCardSpec;

class BattleMapObj;
class BattleMapModel;
class BakuganModel;
class ModelLightGroup;

// Camera of the card screen (vtable 02288700, 0x30 bytes): the map objects, two cameras and a model; built from
// bakugan id | attribute << 8 of each side (mSide0BakuganId / mSide0Variant, ...).
class BattleCardCamera : public SceneBase {
public:
    /* 00 SceneBase */
    /* 0c */ BattleMapObj *mMapObj;
    /* 10 */ BakuganModel *mBakugans[2];
    /* 18 */ BattleMapModel *mMapModel;
    /* 1c */ ModelLightGroup *mLights;
    /* 20 */ void *mCamFile;
    /* 24 */ u16 mUnk_24;
    /* 26 */ u8 mState;
    /* 27 */ u8 mMapModelVisible; // always 1 in this build
    /* 28 */ PAD(0x28, 0x29);
    /* 29 */ u8 mUnk_29;
    /* 2a */ u8 mUnk_2a;
    /* 2b */ u8 mSide0BakuganId;
    /* 2c */ u8 mSide1BakuganId;
    /* 2d */ u8 mSide0Variant;
    /* 2e */ u8 mSide1Variant;
    /* 2f */ PAD(0x2f, 0x30);
    /* 30 */

    BattleCardCamera(u16 a, u16 b);
    virtual ~BattleCardCamera();
    /* 08 */ virtual void Update();
    void Init();
    void UpdateCameraAndDraw();
};

// Confrontation card screen (vtable 02288378): shows the cards of both sides before a battle (battle data,
// card graphics, power values, mes_confrontation text). Created with a BATTLE_MODE_ value (-1: from the marker card).
class BattleConfrontCard : public SceneBase {
public:
    /* 00 SceneBase */
    /* 0c */ u8 mState;
    /* 0d */ u8 mStep;
    /* 0e */ u8 mShakeSide;
    /* 0f */ u8 mShakeBaseX;
    /* 10 */ u16 mUnk_10;
    /* 12 */ u16 mUnk_12;
    /* 14 */ s16 mDrawX[3];
    /* 1a */ s16 mDrawY[3];
    /* 20 */ u8 mBattleMode; // BATTLE_MODE_* (BattleBase.hpp)
    /* 21 */ s8 mUnk_21;
    /* 22 */ u8 mHidden;
    /* 23 */ u8 mUnk_23;
    /* 24 */ CellAnimation *mCellAnims[5];
    /* 38 */ BattleData *mData;
    /* 3c */ BattleCardCamera *mCardCamera;
    /* 40 */ BattleCommandPage *mCommandPage;
    /* 44 */ BattleCardSpec *mCardSpec;
    /* 48 */ void *mConfront;
    /* 4c */ void *mUnionAbility;
    /* 50 */ u8 mMarkerCardKind;
    /* 51 */ u8 mCardPickStep;
    /* 52 */ u8 mLocalSide;
    /* 53 */ u8 mSlot6cIndex;
    /* 54 */ u16 mCardId;
    /* 56 */ u16 mUnk_56;
    /* 58 */ PAD(0x58, 0x5c);
    /* 5c */ u8 mUnk_5c;
    /* 5d */ u8 mUnk_5d;
    /* 5e */ u8 mPromptCardPicked;
    /* 5f */ u8 mAttributeValuesVisible;
    /* 60 */ u8 mDisabledBits;
    /* 61 */ u8 mUnk_61;
    /* 62 */ u8 mSide0CardDone;
    /* 63 */ u8 mSide1CardDone;
    /* 64 */ u8 mUnk_64;
    /* 65 */ u8 mUnk_65;
    /* 66 */ u16 mUnk_66;
    /* 68 */ u8 mUnk_68;
    /* 69 */ u8 mWindowOpen;
    /* 6a */ PAD(0x6a, 0x6c);
    /* 6c */ u32 mTextBufferSize;
    /* 70 */ u32 mGlyphCount;
    /* 74 */ void *mUnk_74;
    /* 78 */ void *mTextBuffer;
    /* 7c */ void *mGlyphBuffer;
    /* 80 */ SelectWindow *mWindow;
    /* 84 */ struct MessageFile *mMessages;
    /* 88 */

    BattleConfrontCard(u32 unk);
    virtual ~BattleConfrontCard();
    /* 08 */ virtual void Update();
    int UpdateCardPick();
    u8 func_ov007_022428d4(u16 card);
    u8 func_ov007_02244440();
    BOOL func_ov007_022433c8(u8 side, u16 id);
    void LoadWindow(const char *path, u32 mode);
    void PrintMessage(u32 index);
    void Init();
    void InitDisplay();
    void ResetDisplay(); // also clears data_020d433c.mAntenna->mSecondVisible (when set)
    void LoadGraphics();
    void LoadCardGraphic(u16 card);
    void func_ov007_0223f90c(u16 value);
    BOOL UpdateConfrontation();
    void SetSubBlendBrightness(int brightness);
    void SetSubBlendAlpha(int blend);
    void DrawEntriesAndCard();
    void DrawEntryNumbers(u8 kind, u32 mode, u32 flag);
    u8 GetMarkerBattleMode();
    u16 func_ov007_02243b24(u8 side, u32 id);
    void OpenWindow();
    void CloseWindow();
    void ReleaseWindow();
};
