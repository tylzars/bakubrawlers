#pragma once

#include "global.h"
#include "types.h"

#include "Scene/SceneBase.hpp"

class CellAnimation;
class MessageWindow;
class MyroomStatusScene;
class SelectWindow;
struct MessageFile;

// Overlay 8: the character list screen. It shows the battle record of the player and of the story characters
// (names from mes_StoryCharaName, labels from mes_myroom_status) and creates the MyroomStatusScene of the player.
class MyroomCharaScene : public SceneBase {
public:
    /* 00 (base) */
    /* 0c */ MyroomStatusScene *mStatus;
    /* 10 */ u8 mState;
    /* 11 */ s8 mTab;
    /* 12 */ s8 mShownTab;
    /* 13 */ PAD(0x13, 0x14);
    /* 14 */ u32 mCellsVisible[2];
    /* 1c */ u32 mCellVisible;
    /* 20 */ SelectWindow *mWindow;
    /* 24 */ CellAnimation *mCells[2];
    /* 2c */ CellAnimation *mCell;
    /* 30 */ u32 mResult;
    /* 34 */ MessageFile *mStoryCharaNameMessages;
    /* 38 */ MessageFile *mMessages;
    /* 3c */ void *mTextBuffers[7];
    /* 58 */ void *mGlyphBuffer;
    /* 5c */ u32 mTextBufferSize;
    /* 60 */ u32 mGlyphCount;
    /* 64 */ u32 mLinesVisible[7];
    /* 80 */ u8 mPage;
    /* 81 */ u8 mPageCount;
    /* 82 */ u8 mList[17];
    /* 93 */ u8 mListCount;
    /* 94 */ MessageWindow *mMessageWindow;
    /* 98 */

    MyroomCharaScene(); // 0x02223594

    /* 00 */ virtual ~MyroomCharaScene() override; // 0x02223638
    /* 08 */ virtual void Update() override;       // 0x0222384c
    /* 0c */

    void Cleanup();          // 0x02223780
    void FreeCells();        // 0x02223800
    void SetReady();         // 0x02223fa8
    void InitDisplay();      // 0x02223fc0
    void LoadSubGraphics();
    void LoadMainGraphics();
    void SetupTabCells();
    void Draw();             // 0x02224914
    void DrawWindow();       // 0x02224944
    void DrawCells();        // 0x02224954
    void DrawCell();         // 0x022249ec
    u8 func_ov008_02224a4c();
    void LoadText();
    void ClearTextBuffers(); // 0x02224c60
    void PrintValue(u32 line, u32 value, u8 digits, s16 x, s16 suffix);
    void RenderLine(u32 line, u32 kind);
    void DrawText();         // 0x02225544
    void FreeText();         // 0x0222563c
    void RenderLines();
    void BuildList();
    void RefreshPageButtons();
};
