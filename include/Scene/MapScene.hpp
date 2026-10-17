#pragma once

#include "global.h"
#include "types.h"

#include "Scene/SceneBase.hpp"

class CellAnimation;
class SelectWindow;
class ScriptPlayer;
struct MessageFile;

// A message plus the buffers it is rendered into.
struct TextSlot {
    /* 00 */ MessageFile *mMessages;
    /* 04 */ void *mTextBuffer;
    /* 08 */ void *mGlyphBuffer;
    /* 0c */ u32 mTextBufferSize;
    /* 10 */ u32 mGlyphCount;
    /* 14 */
};

// Map tabs: the values of mTab (the menu button selected in UpdateMenu), named after the scene SetNextSceneFromTab
// requests for each; ShowTabInfo, LoadPanelText2 and DrawPopup switch on them and sMapPagePos[].mId holds one per
// page. The bounds `sel < 4`, `sel <= 3` and `i < 4` in UpdateMenu and CreateMenu stay literals.
#define MAP_TAB_MYROOM 0
#define MAP_TAB_STORE 1
#define MAP_TAB_PARK 2
#define MAP_TAB_TOURNAMENT 3

// Overlay 5: the story map (town info panels, tabs and drag-scrolled background).
class MapScene : public SceneBase {
public:
    /* 00 (base) */
    /* 0c */ CellAnimation *mCellAnims[6];
    /* 24 */ ScriptPlayer *mEvent;
    /* 28 */ SelectWindow *mMenuWindow;
    /* 2c */ SelectWindow *mPanelWindow;
    /* 30 */ void *mGlyphBuffer;
    /* 34 */ u16 mGlyphCount;
    /* 36 */ u16 mPopupTextX;
    /* 38 */ u16 mPopupTextY;
    /* 3a */ PAD(0x3a, 0x3c);
    /* 3c */ TextSlot mText[2];
    /* 64 */ u8 mState;
    /* 65 */ u8 mTimer;
    /* 66 */ s16 mNextScene;
    /* 68 */ u8 mUnk_68;
    /* 69 */ u8 mUnk_69;
    /* 6a */ u8 mTab;
    /* 6b */ PAD(0x6b, 0x6c);
    /* 6c */ u16 mScrollPos;
    /* 6e */ u16 mScrollTarget;
    /* 70 */ u16 mParallax;
    /* 72 */ u8 mDragging;
    /* 73 */ u8 mLastTouchX;
    /* 74 */ s8 mUnk_74; // MAP_TAB_*: mCellAnims[1] is drawn at that sMapMenuItems entry's x + 10, y - 0x18; -1 = none
    /* 75 */ s8 mSkipDraw;
    /* 76 */ u8 mPage;
    /* 77 */ u8 mPageVisible;
    /* 78 */ PAD(0x78, 0x79);
    /* 79 */ u8 mPanelVisible; // non-zero: Update draws only DrawPanelWindow (no popup, page or money)
    /* 7a */ u16 mTextWidth;
    /* 7c */ u8 mUnk_7c;
    /* 7d */ u8 mSaveWindowPending;
    union {
        /* 7e */ u8 mPopupBits;
        struct {
            /* 7e */ u8 mMyroomPopupVisible : 1;
            /* 7e */ u8 mStorePopupVisible : 1;
            /* 7e */ u8 mParkPopupVisible : 1;
            /* 7e */ u8 mTournamentPopupVisible : 1;
            /* 7e */ u8 mUnk_7e_4 : 4;
        };
    };
    /* 7f */

    MapScene();

    /* 00 */ virtual ~MapScene() override;
    /* 08 */ virtual void Update() override;
    /* 0c */

    void Init();                      // 0221a4e0
    void SetupMap();
    void InitDisplay();               // 0221a63c
    void LoadWindowGraphics();        // 0221a824
    void ReloadMap();
    void LoadMapBackground();         // 0221aaa4
    void func_ov005_0221abf4();
    // TRUE when mEvent->GetResult() is non-zero. Also writes data_020d433c: mUnk_7f = SCENE_MAP (MYROOM), mRules and
    // mMicEnabled (BATTLE), mTutorialNumber / mTutorialPart / mUnk_82 (TUTORIAL).
    BOOL SetNextSceneFromEvent();
    BOOL func_ov005_0221b2ac();
    u8 UpdateMenu();                  // 0221b2dc
    void func_ov005_0221b364(u8 index, BOOL changed);
    void CreateMenu();                // 0221b3c0
    void UpdateDrag();                // 0221b4c8
    BOOL SetNextSceneFromTab();
    s16 func_ov005_0221b760(u32 index);
    void DrawPanelWindow();
    void DrawPopup();                 // 0221b824
    void DrawMoney();                 // 0221bc28
    void DrawPage();
    // Clears mPopupBits; when the tab's condition holds, prints its info and sets the mPopupBits bit DrawPopup tests. MYROOM
    // only when data_020d48f0.mRank differs from mPrevRank (then copies it); PARK calls RefreshParkSlotProgressEntryFlag first.
    void ShowTabInfo(u32 index);
    void ShowNumbers();               // 0221bf60
    void ShowText(u16 index, s32 unk2); // 0221c390
    BOOL ShowCurrent();               // 0221c650
    void CreatePanelWindow();         // 0221c780
    void LoadPanelText();             // 0221c80c
    void DrawPanelText();             // 0221caf8
    void LoadPanelText2(int index);   // 0221cba4
    void DrawPanelText2(s16 x);       // 0221cdbc
    u32 UpdatePanelWindow();          // 0221cec4
};

void MapScrollCallback(MapScene *scene); // scroll callback for QueueFrameCallback
