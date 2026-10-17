#pragma once

#include "global.h"
#include "types.h"

#include "Scene/SceneBase.hpp"

class CellAnimation;
class MessageWindow;
class SelectWindow;
struct MessageFile;

extern u8 data_020d49a6[];

// Overlay 8: the gift screen, showing the emblems the player has received.
class MyroomGiftScene : public SceneBase {
public:
    /* 00 (base) */
    /* 0c */ MessageFile *mMessages;
    /* 10 */ CellAnimation *mCellAnim;
    /* 14 */ void *mTextBuffers[2];
    /* 1c */ void *mGlyphBuffers[2];
    /* 24 */ MessageWindow *mMessage;
    /* 28 */ SelectWindow *mWindow;
    /* 2c */ u32 mResult;
    /* 30 */ u32 mTextBufferSize[2];
    /* 38 */ u32 mGlyphCount[2];
    /* 40 */ u16 mTextWidth;
    /* 42 */ s16 mTextX;
    /* 44 */ u16 mTextEndY;
    /* 46 */ u8 mHasEmblem[10];
    /* 50 */ s8 mEmblem;
    /* 51 */ u8 mCol;
    /* 52 */ u8 mRow;
    /* 53 */ u8 mState;
    /* 54 */ s8 mTimer;
    /* 55 */ s8 mTextPage;
    /* 56 */

    MyroomGiftScene(); // 0x0221d6b0

    /* 00 */ virtual ~MyroomGiftScene() override; // 0x0221d74c
    /* 08 */ virtual void Update() override;      // 0x0221da24
    /* 0c */

    void InitDisplay();              // 0x0221dbe0
    void LoadGraphics();
    void InitEmblems();              // 0x0221e508
    void LoadEmblem(u8 index);       // 0x0221e560
    void Draw();                     // 0x0221e658
    void DrawMainScreen();
    void DrawWindows();              // 0x0221e968
    void ClearTextBuffers(u8 index); // 0x0221e984
    void PrintEmblemText();
    s32 UpdateInput();
    s8 UpdateKeySelection();
    void func_ov008_0221f194();
};
