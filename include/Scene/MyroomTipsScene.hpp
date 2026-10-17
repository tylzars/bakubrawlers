#pragma once

#include "global.h"
#include "types.h"

#include "Scene/SceneBase.hpp"

class CellAnimation;
class MessageWindow;
class SelectWindow;
class ScriptPlayer;
struct MessageFile;

// Overlay 8: the tips list screen (mes_TIPS_sys.mes): four tip titles that scroll sideways, a window to pick one and
// a scroll bar. The state is kept in the scene context (mInfo.mStep).
class MyroomTipsScene : public SceneBase {
public:
    /* 00 (base) */
    /* 0c */ MessageFile *mMessages;
    /* 10 */ void *mTextBuffers[4];
    /* 20 */ void *mGlyphBuffer;
    /* 24 */ MessageWindow *mMessage;
    /* 28 */ SelectWindow *mWindowA;
    /* 2c */ SelectWindow *mWindowB;
    /* 30 */ ScriptPlayer *mEvent;
    /* 34 */ CellAnimation *mCells[6];
    /* 4c */ CellAnimation *mCellA;
    /* 50 */ CellAnimation *mCellB;
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
    void LoadText();
    void Draw();              // 0x02226ff0
    void func_ov008_022271f8();
    void func_ov008_022273f0();
    void DrawCategoryCursor(const u8 *pos, u32 index);
    void func_ov008_022274f8();
    s8 UpdateCategorySelection(const u8 *pos, u8 count);
    BOOL UpdateRowSelection();
    void PrintRows(s32 row);
    void CreateMessage();
    void SetMessage(u32 message);
    void DrawMessage();       // 0x02228434
    void ClearMessageBuffer();
    void CountTips();
    u32 GetUnlockedTip(s16 index);
    void UpdateTextX();
};
