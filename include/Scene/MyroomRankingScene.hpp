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
    /* 28 */ u8 mCharacterId;
    /* 29 */ PAD(0x29, 0x2a);
    /* 2a */ u16 mRank;
    /* 2c */ u32 mScore;
    /* 30 */
};

// Overlay 8: the ranking list screen (myroom_ranking_b_obj_00, mes_StoryCharaName).
class MyroomRankingScene : public SceneBase {
public:
    /* 00 (base) */
    /* 0c */ Ranking *mRanking;
    /* 10 */ RankingCharacterView *mCharacterView;
    /* 14 */ SelectWindow *mWindow;
    /* 18 */ PAD(0x18, 0x40);
    /* 40 */ u8 mPlayerCharacterId;
    /* 41 */ PAD(0x41, 0x42);
    /* 42 */ u16 mPlayerRank;
    /* 44 */ u32 mPlayerScore;
    /* 48 */ MyroomRankingRow mRows[6];
    /* 168 */ u8 mSelectedCharacterId;
    /* 169 */ PAD(0x169, 0x16a);
    /* 16a */ u16 mSelectedRank;
    /* 16c */ u32 mSelectedScore;
    /* 170 */ u32 mResult;
    /* 174 */ MessageFile *mMessages;
    /* 178 */ PAD(0x178, 0x17c);
    /* 17c */ MessageWindow *mMessageWindow;
    /* 180 */ void *mTextBuffers[6];
    /* 198 */ void *mGlyphBuffers[6];
    /* 1b0 */ u32 mTextBufferSize;
    /* 1b4 */ u32 mGlyphCount;
    /* 1b8 */ u8 mState;
    /* 1b9 */ u8 mScroll;
    /* 1ba */ u8 mRow;
    /* 1bb */ u8 mEntryIndex;
    /* 1bc */ s8 mPrintedScroll;
    /* 1bd */ u8 mEntryCount;
    /* 1be */ PAD(0x1be, 0x1bf);
    /* 1bf */ s8 mTimer;
    /* 1c0 */ u32 mUnk_1c0;
    /* 1c4 */

    MyroomRankingScene(); // 0x0221f3a8

    /* 00 */ virtual ~MyroomRankingScene() override; // 0x0221f4a4
    /* 08 */ virtual void Update() override;         // 0x0221f7fc
    /* 0c */

    void InitDisplay();       // 0x0221fca4
    void Init();
    void Draw();
    void DrawSubScreen();
    s32 UpdateSelection();
    s32 UpdateKeySelection();
    void UpdateRows();
    void PrintNames();
    void ClearGlyphBuffers(); // 0x02220ff8

    // Inlined into Update and UpdateRows.
    void UpdateRowAnimations();
};
