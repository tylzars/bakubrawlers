#pragma once

#include "global.h"
#include "types.h"

#include "Scene/SceneBase.hpp"

class CellAnimation;
class MessageWindow;
class SelectWindow;
class Ranking;
class RankingCharacterView;
struct MessageFile;

// One row of cell animations of the ranking list.
struct MyroomRankingRow {
    /* 00 */ CellAnimation *mCells[10];
    /* 28 */ u8 mUnk_28;
    /* 29 */ PAD(0x29, 0x2a);
    /* 2a */ u16 mUnk_2a;
    /* 2c */ u32 mUnk_2c;
    /* 30 */
};

// Overlay 8: the ranking list screen (myroom_ranking_b_obj_00, mes_StoryCharaName).
class MyroomRankingScene : public SceneBase {
public:
    /* 00 (base) */
    /* 0c */ Ranking *mUnk_0c;
    /* 10 */ RankingCharacterView *mUnk_10;
    /* 14 */ SelectWindow *mWindow;
    /* 18 */ PAD(0x18, 0x40);
    /* 40 */ u8 mUnk_40;
    /* 41 */ PAD(0x41, 0x42);
    /* 42 */ u16 mUnk_42;
    /* 44 */ u32 mUnk_44;
    /* 48 */ MyroomRankingRow mRows[6];
    /* 168 */ u8 mUnk_168;
    /* 169 */ PAD(0x169, 0x16a);
    /* 16a */ u16 mUnk_16a;
    /* 16c */ u32 mUnk_16c;
    /* 170 */ u32 mResult;
    /* 174 */ MessageFile *mMessages;
    /* 178 */ PAD(0x178, 0x17c);
    /* 17c */ MessageWindow *mMessage;
    /* 180 */ void *mTextBuffers[6];
    /* 198 */ void *mGlyphBuffers[6];
    /* 1b0 */ u32 mTextBufferSize;
    /* 1b4 */ u32 mGlyphCount;
    /* 1b8 */ u8 mUnk_1b8;
    /* 1b9 */ u8 mUnk_1b9;
    /* 1ba */ u8 mUnk_1ba;
    /* 1bb */ u8 mUnk_1bb;
    /* 1bc */ s8 mUnk_1bc;
    /* 1bd */ u8 mUnk_1bd;
    /* 1be */ PAD(0x1be, 0x1bf);
    /* 1bf */ s8 mUnk_1bf;
    /* 1c0 */ u32 mUnk_1c0;
    /* 1c4 */

    MyroomRankingScene(); // 0x0221f3a8

    /* 00 */ virtual ~MyroomRankingScene() override; // 0x0221f4a4
    /* 08 */ virtual void Update() override;         // 0x0221f7fc
    /* 0c */

    void InitDisplay();       // 0x0221fca4
    void func_ov008_0221fe9c();
    void func_ov008_022205ac();
    void func_ov008_022205b8();
    s32 func_ov008_02220848();
    s32 func_ov008_022209d4();
    void func_ov008_02220b88();
    void func_ov008_02220d14();
    void ClearGlyphBuffers(); // 0x02220ff8

    // Inlined into Update and func_ov008_02220b88.
    void UpdateRowAnimations();
};
