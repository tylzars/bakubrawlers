#pragma once
#include "global.h"
#include "types.h"
#include "Scene/SceneBase.hpp"

class CellAnimation;
class SelectWindow;
struct MessageFile;

// Main menu entries: the values of mSelection (the main window's GetSelectedId) that Update state 2 switches on.
// VOLUME goes to BuildSettings: three rows of six levels stored in gSaveData mVolume0Index / mVolume1Index / mVolume2Index,
// which ApplyVolumeOptions turns into FrameTask::mVolume0 / mVolume1 / mVolume2 and the SoundManager volumes. RESET
// goes to BuildMenu3, whose first button calls ResetOptions, and CREDITS to a CreditsScene. Left as literals: 1 (BuildMenu2, sets gSaveData mMicEnabled and
// mUnk_27_0) and 2 (BuildMenu4, sets mUnk_28_5); nothing in the tree names those flags.
#define OPTION_MENU_VOLUME 0
#define OPTION_MENU_RESET 3
#define OPTION_MENU_CREDITS 4

// Overlay 0 scene with vtable 0x022273dc (title-side option menu).
class OptionScene : public SceneBase {
public:
    /* 0c */ void *mBuf0c;
    /* 10 */ SelectWindow *mWindow;
    /* 14 */ MessageFile *mFile;
    /* 18 */ CellAnimation *mCellAnims[5];
    /* 2c */ u16 mBufSize;
    /* 2e */ u16 mSaved;
    /* 30 */ s16 mPosX[5];
    /* 3a */ s16 mPosY[5];
    /* 44 */ u8 mCur[3];
    /* 47 */ u8 mOld[3];
    /* 4a */ u8 mState;
    /* 4b */ u8 mSlideTimer; // set to 8; counted down inside DrawSettings / DrawMenu2 / DrawMenu3 / SlideStep
    /* 4c */ s8 mTouchRow;
    /* 4d */ s8 mSelection;
    /* 4e */ u8 mRow;
    /* 4f */ u8 mFlagSelectedId; // id < 2 of the BuildMenu2 / BuildMenu4 windows; 0 stores 1 in mMicEnabled / mUnk_28_5
    /* 50 */

    OptionScene();
    /* 00 */ virtual ~OptionScene() override;
    /* 08 */ virtual void Update() override;
    /* 0c */

    void Release(); // 0221f5a4
    void Init();    // 02220124
    void BuildMainMenu();  // 02220738
    void BuildSettings();  // 022207d4
    void DrawSettings();   // 022209c8
    void UpdateSettings(); // 02220adc
    void BuildMenu2();     // 022210f8
    void DrawMenu2();      // 0222128c, 02221814 (identical)
    void DrawMenu3();
    void SlideStep();      // 02221534
    void ResetDisplay(); // 0222163c
    void BuildMenu3();     // 022212d4
    void BuildMenu4();     // 02221680
    void ClearText(u32 index); // 0222185c
};
