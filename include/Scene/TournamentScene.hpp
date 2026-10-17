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
    /* 0c */ CellAnimation *mCellAnimsA[5];
    /* 20 */ CellAnimation *mCell20;
    /* 24 */ CellAnimation *mCellAnimsB[5];
    /* 38 */ CharacterIcons *mObj38;
    /* 3c */ ScriptPlayer *mEvent;
    /* 40 */ SelectWindow *mMenuWindow;
    /* 44 */ SelectWindow *mWindow;
    /* 48 */ MessageFile *mMessages[4];
    /* 58 */ MessageWindow *mMessageWindow;
    /* 5c */ void *mTextBuffers[4];
    /* 6c */ void *mGlyphBuffers[4];
    /* 7c */ u32 mTextBufferSizes[4];
    /* 8c */ u32 mGlyphCounts[4];
    /* 9c */ u16 mTextWidth;
    /* 9e */ u16 mTextHeight;
    /* a0 */ s16 mUnk_a0;
    /* a2 */ s16 mUnk_a2;
    /* a4 */ s16 mMoveX; // offset moved so far (mMoveDistanceX * mMoveFrames / 5), kept after the move
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
    /* b5 */ u8 mMenuVisible; // draws mMenuWindow for the LoadMenu page; mUnk_b9 does so for the LoadModeMenu page
    /* b6 */ u8 mMainScreenVisible;
    /* b7 */ u8 mCellsBVisible; // mCellAnimsB are drawn only while this and mMainScreenVisible are both set
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
    /* cd */ u8 mOtherItemState; // next mState when mWindow closes on an item other than 0 (item 1; item 2 is 0x8000)
    /* ce */ u8 mUnk_ce;
    /* cf */ u8 mMoveFrames;
    /* d0 */ u8 mUnk_d0;
    /* d1 */ u8 mTextRows;
    /* d2 */ u8 mUnk_d2;
    /* d3 */ u8 mUnk_d3;
    /* d4 */ u8 mNextBgLayer; // layer StartBgBlend loads into: 0 BG0 kinds, 1 BG1 kinds (then toggled)
    /* d5 */ u8 mBlend;
    /* d6 */ s8 mMainTextPage;
    /* d7 */ s8 mMainTextLineCount; // Print's y after the message >> 4 (line advances of 0x10); read as the line count
    /* d8 */

    TournamentScene();

    /* 00 */ virtual ~TournamentScene() override;
    /* 08 */ virtual void Update() override;
    /* 0c */

    void Cleanup(); // 022195e0
    // unlike TutorialScene's: also sets up sub BG0 (sub planes 0x11, not 0x10), but no main BG0 control and main BG0
    // priority 0, not 2
    void InitScriptDisplay();
    void func_ov002_0221b5b0();
    // Screen 0 set. First resets both engines' display registers (func_ov002_0221b5b0) and clears 0x20000 bytes at
    // 0x06200000 and 0x06600000; sets mMainScreenVisible.
    void LoadMainGraphics(BOOL unk1, BOOL unk2);
    void LoadWindowGraphics(); // also clears mMenuVisible
    void ReloadMenu();
    // Also LoadSubScreenText, a new mMessageWindow (mes_tornament), LoadMainScreenText and RefreshCellLayout.
    void LoadMenu();
    void LoadModeMenu();
    void LoadMainScreenText(const char *mes); // `mes` into [2]; buffer [3] is for the data_020d441a text
    void LoadSubScreenText(const char *mes, u8 rows); // also frees entry [1] (LoadWindow's text)
    void Draw();
    u8 UpdateScrollInput(u8 input);
    u8 UpdateScrollKeys(); // its mNewSelection step is reset by UpdateScrollInput's next call
    BOOL HandleEventResult();
    // Moves the menu cells by up to 0xe per call out of mScrollLeft; at 0 commits mNewSelection to mSelection and the
    // cursor, sets buttons 3 / 4 and prints both screens' texts, and returns TRUE.
    BOOL UpdateScroll();
    void RefreshCellLayout();
    void PrintSubScreenText(u8 index); // prints and uploads [0] only, after ClearSubScreenText zeroed [0] and [1]
    void ClearSubScreenText(); // zeroes both screen-1 text buffers, [0] and the window text [1]; no upload
    void SetupMainScreenText(u8 index);
    void PrintMainScreenText(u32 index);
    void ClearMainScreenText();
    BOOL func_ov002_0221e2b0();
    void func_ov002_0221e8f0();
    void func_ov002_0221e984();
    void OpenWindow();
    void CloseWindow();
    void LoadWindow(const char *path, u32 index); // builds mWindow: union_b window files, `path` into [1], text, cells
    void PrintWindowText(u32 index);
    // loads BG set sModeTables[2][index] into the layer mNextBgLayer picks, starts the blend and toggles that member
    void StartBgBlend(u32 index);
    BOOL UpdateBgBlend(); // FALSE on each call that raises mBlend (also the one reaching 0x1f), TRUE after
};
