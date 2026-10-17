#pragma once

#include "global.h"
#include "types.h"

#include "Scene/SceneBase.hpp"

class MessageWindow;
class SelectWindow;
class CharacterIcons;
class RankingCharacterView;
class Ranking;
struct MessageFile;

struct TournamentEntry {
    /* 0 */ u8 mId;
    /* 2 */ u16 mRank;
    /* 4 */ u32 mScore;
    /* 8 */
};

// Overlay 2: a choice screen created by TournamentScene.
class TournamentSubScene : public SceneBase {
public:
    /* 00 (base) */
    /* 0c */ Ranking *mRanking;
    /* 10 */ RankingCharacterView *mCharacterView;
    /* 14 */ SelectWindow *mWindow;
    /* 18 */ TournamentEntry mEntries[3];
    /* 30 */ MessageFile *mMessages;
    /* 34 */ void *mTextBuffer;
    /* 38 */ void *mGlyphBuffer;
    /* 3c */ u32 mTextBufferSize;
    /* 40 */ u32 mGlyphCount;
    /* 44 */ MessageWindow *mMessage;
    /* 48 */ CharacterIcons *mCharacterIcons;
    /* 4c */ s32 mResult;
    /* 50 */ u8 mTextLineCount;
    /* 51 */ s8 mScroll;
    /* 52 */ u8 mCount;
    /* 53 */ s8 mSelected;
    /* 54 */ u8 mUnk_54;
    /* 55 */ u8 mState;
    /* 56 */ s8 mCounter;
    /* 57 */ PAD(0x57, 0x58);
    /* 58 */ u32 mUnk_58;
    /* 5c */

    TournamentSubScene();

    /* 00 */ virtual ~TournamentSubScene() override;
    /* 08 */ virtual void Update() override;
    /* 0c */

    void InitDisplay();
    void Init();
    void Draw(); // 02220178
    void DrawSubScreen();
    s32 UpdateEntrySelection();
    s32 UpdateEntryKeySelection();
    void func_ov002_02220618();
    void PrintProfile();
    void SetupProfile();
    void ClearTextBuffer(); // 022209e4
};
