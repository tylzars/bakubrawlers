#pragma once

#include "global.h"
#include "types.h"

#include "Scene/SceneBase.hpp"

class CellAnimation;
class MessageWindow;
class SelectWindow;
class ScriptPlayer;
struct MessageFile;

// mFlags bits: WINDOW_A enables, updates and draws mWindowA with the category cells and cursor; WINDOW_B does the same
// for mWindowB with the four tip rows and their cells. The scene sets one at a time; 0 runs neither.
#define MYROOM_TIPS_WINDOW_A 1
#define MYROOM_TIPS_WINDOW_B 2

// Overlay 8: the tips list screen (mes_TIPS_sys.mes): four tip titles that scroll sideways, a window to pick one and
// a scroll bar. The state is kept in the scene context (mInfo, read as MyroomTipsSceneInfo::mState in the .cpp).
class MyroomTipsScene : public SceneBase {
public:
    /* 00 (base) */
    /* 0c */ MessageFile *mMessages;
    /* 10 */ void *mTextBuffers[4];
    /* 20 */ void *mGlyphBuffer;
    /* 24 */ MessageWindow *mMessageWindow;
    /* 28 */ SelectWindow *mWindowA;
    /* 2c */ SelectWindow *mWindowB;
    /* 30 */ ScriptPlayer *mEvent;
    /* 34 */ CellAnimation *mCellAnims[6];
    /* 4c */ CellAnimation *mCellAnimA;
    /* 50 */ CellAnimation *mCellAnimB;
    /* 54 */ CellAnimation *mNewCells[2];
    /* 5c */ CellAnimation *mRowCells[2];
    /* 64 */ u32 mTextBufferSize;
    /* 68 */ u32 mGlyphCount;
    /* 6c */ u32 mRowGlyphCount;
    /* 70 */ s16 mTextX[4];
    /* 78 */ s16 mTextWidth[4];
    /* 80 */ u8 mFlags;
    /* 81 */ u8 mScroll;
    /* 82 */ u8 mRow;
    /* 83 */ u8 mCount;
    /* 84 */ PAD(0x84, 0x85);
    /* 85 */ u8 mUnk_85;
    /* 86 */ u8 mUnlocked[6];
    /* 8c */ u8 mNew[6];
    /* 92 */ u8 mIndices[4];
    /* 96 */ s8 mTimers[4];
    /* 9a */

    MyroomTipsScene(); // 0x02225a88

    /* 00 */ virtual ~MyroomTipsScene() override; // 0x02225bc4
    /* 08 */ virtual void Update() override;      // 0x02225f0c
    /* 0c */

    void InitDisplay();       // 0x022266b8
    void LoadGraphics();
    void LoadText(); // also prints every row (PrintRows(-1))
    void Draw();              // 0x02226ff0
    void func_ov008_022271f8();
    // WINDOW_A: DrawCategoryCursor; WINDOW_B: mRowCells[1] on row mRow and mRowCells[0] on the other three rows.
    void func_ov008_022273f0();
    void DrawCategoryCursor(const u8 *pos, u32 index);
    // Nothing while mEvent runs (both windows disabled). WINDOW_A: category input while mInfo byte 0x02 (mUnk_02 of the
    // .cpp's MyroomTipsSceneInfo) is 0, which is always in this build; picking the current unlocked category (category
    // 0 not while !mUnk_5bf_0 and mUnk_42 == 6) calls func_0206f88c on mEvent and sets up the row list (mInfo state 8).
    // WINDOW_B: row input while mUnk_85 is 0 (a pick starts mRowCells[1] animation 3 and sets mUnk_85); then, once that
    // animation ends, starts the tip's message (mInfo state 0xb) and sets its data_020d49a1 bit (mNew - 1 when newly
    // set).
    void func_ov008_022274f8();
    // A new touch on an entry returns its index at once (the mInfo category unchanged); otherwise the keys move the
    // mInfo category (rows of 3), returned only with IsPressed(1), else -1.
    s8 UpdateCategorySelection(const u8 *pos, u8 count);
    BOOL UpdateRowSelection();
    void PrintRows(s32 row);
    void CreateMessage();
    void SetMessage(u32 message);
    void DrawMessage();       // 0x02228434
    void ClearMessageBuffer();
    void CountTips();
    u32 GetUnlockedTip(s16 index); // position in the category of the (index + 1)-th unlocked tip; 0 (valid) if none
    // Per row: counts mTimers down; else, when mTextWidth > 0xc0, moves mTextX left by 1 (back to 0xc0 once the text
    // is off, a 0x3c-frame pause at 0) and reprints the row.
    void UpdateTextX();
};
