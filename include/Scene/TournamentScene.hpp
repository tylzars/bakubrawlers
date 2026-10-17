#pragma once

#include "global.h"
#include "types.h"

#include "Scene/SceneBase.hpp"

class CellAnimation;
class MessageWindow;
class SelectWindow;
class CharacterIcons;
class ScriptPlayer;
class TournamentSubScene;
struct MessageFile;

// Overlay 2: the tournament bracket.
class TournamentScene : public SceneBase {
public:
    /* 00 (base) */
    /* 0c */ CellAnimation *mCellsA[5];
    /* 20 */ CellAnimation *mCell20;
    /* 24 */ CellAnimation *mCellsB[5];
    /* 38 */ CharacterIcons *mObj38;
    /* 3c */ ScriptPlayer *mEvent;
    /* 40 */ SelectWindow *mMenu;
    /* 44 */ SelectWindow *mWindow;
    /* 48 */ MessageFile *mMessages[4];
    /* 58 */ MessageWindow *mMessage;
    /* 5c */ void *mTextBuffers[4];
    /* 6c */ void *mGlyphBuffers[4];
    /* 7c */ u32 mTextBufferSizes[4];
    /* 8c */ u32 mGlyphCounts[4];
    /* 9c */ u16 mTextWidth;
    /* 9e */ u16 mTextHeight;
    /* a0 */ s16 mUnk_a0;
    /* a2 */ s16 mUnk_a2;
    /* a4 */ s16 mMoveX;
    /* a6 */ s16 mMoveY;
    /* a8 */ s16 mMoveDistanceX;
    /* aa */ s16 mMoveDistanceY;
    /* ac */ u16 mUnk_ac;
    /* ae */ s16 mMainTextTimer;
    /* b0 */ u8 mState;
    /* b1 */ u8 mTimer;
    /* b2 */ u8 mMode;
    /* b3 */ u8 mSelection;
    /* b4 */ PAD(0xb4, 0xb5);
    /* b5 */ u8 mMenuVisible;
    /* b6 */ u8 mMainScreenVisible;
    /* b7 */ u8 mCellsBVisible;
    /* b8 */ u8 mWindowOpen;
    /* b9 */ u8 mUnk_b9;
    /* ba */ u8 mLastIndex;
    /* bb */ u8 mNewSelection;
    /* bc */ u8 mDirection;
    /* bd */ u8 mScrollLeft;
    /* be */ u8 mUnk_be;
    /* bf */ u8 mBattleStep;
    /* c0 */ s8 mBrightness;
    /* c1 */ u8 mUnk_c1;
    /* c2 */ u8 mCharacterIds[9];
    /* cb */ u8 mUnk_cb;
    /* cc */ u8 mFirstItemState;
    /* cd */ u8 mOtherItemState;
    /* ce */ u8 mUnk_ce;
    /* cf */ u8 mMoveFrames;
    /* d0 */ u8 mUnk_d0;
    /* d1 */ u8 mTextRows;
    /* d2 */ u8 mUnk_d2;
    /* d3 */ u8 mUnk_d3;
    /* d4 */ u8 mNextBgLayer;
    /* d5 */ u8 mBlend;
    /* d6 */ s8 mMainTextPage;
    /* d7 */ s8 mMainTextLineCount;
    /* d8 */

    TournamentScene();

    /* 00 */ virtual ~TournamentScene() override;
    /* 08 */ virtual void Update() override;
    /* 0c */

    void Cleanup(); // 022195e0
    void func_ov002_0221b424();
    void func_ov002_0221b5b0();
    void LoadMainGraphics(BOOL unk1, BOOL unk2);
    void LoadWindowGraphics();
    void ReloadMenu();
    void LoadMenu();
    void LoadModeMenu();
    void LoadMainScreenText(const char *mes);
    void LoadSubScreenText(const char *mes, u8 rows);
    void Draw();
    u8 UpdateScrollInput(u8 input);
    u8 UpdateScrollKeys();
    BOOL HandleEventResult();
    BOOL UpdateScroll();
    void RefreshCellLayout();
    void PrintSubScreenText(u8 index);
    void ClearSubScreenText();
    void SetupMainScreenText(u8 index);
    void PrintMainScreenText(u32 index);
    void ClearMainScreenText();
    BOOL func_ov002_0221e2b0();
    void func_ov002_0221e8f0();
    void func_ov002_0221e984();
    void OpenWindow();
    void CloseWindow();
    void LoadWindow(const char *path, u32 index);
    void PrintWindowText(u32 index);
    void StartBgBlend(u32 index);
    BOOL UpdateBgBlend();
};
