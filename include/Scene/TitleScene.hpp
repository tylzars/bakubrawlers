#pragma once
#include "global.h"
#include "types.h"
#include "Scene/SceneBase.hpp"

class CellAnimation;
class SelectWindow;
class MessageWindow;
struct SelectWindowCursor;
struct MessageFile;

// Main menu entries: the values of mMenuSelection (the cursor's selection, kept in gGameData.mTitleMenuSelection between
// visits) that Update's case 40 switches on, each named after the scene it requests or, for OPTION, the OptionScene it
// creates. The `i < 5` loops over the entries stay literals.
#define TITLE_MENU_MAP 0
#define TITLE_MENU_ARENA_WINDOW 1
#define TITLE_MENU_TUTORIAL 2
#define TITLE_MENU_MYROOM 3
#define TITLE_MENU_OPTION 4

// Overlay 0 scene with vtable 0x02227920 (title screen: movie bknd_op, main menu, then CharacterEditScene / save scene).
class TitleScene : public SceneBase {
public:
    /* 0c */ CellAnimation *mCellAnims[8];
    /* 2c */ SelectWindowCursor *mCursor;
    /* 30 */ SelectWindow *mWindow30;
    /* 34 */ MessageWindow *mMessageWindow;
    /* 38 */ void *mBuf38;
    /* 3c */ void *mBuf3c;
    /* 40 */ void *mBuf40;
    /* 44 */ void *mBuf44;
    /* 48 */ u32 mTextBufferSize;
    /* 4c */ u32 mGlyphCount;
    /* 50 */ void *mBuffers[5][3];
    /* 8c */ u32 mBufferSizes[5][3];
    /* c8 */ u16 mUnk_c8; // frames in state 6, zeroed when state 5 leads into it; above 0xe10 the scene goes to state 7
    /* ca */ PAD(0xca, 0xcb);
    /* cb */ u8 mState;
    /* cc */ u8 mTimer; // also the slide curve: mScrollY from (mTimer - 3) squared while it counts down from 0x13
    /* cd */ u8 mMenuCellsVisible;
    /* ce */ u8 mMenuSelection;
    /* cf */ u8 mMessageIndex; // last index SetMessage started; 0xff (ctor) = none yet
    /* d0 */ u8 mUnk_d0;
    /* d1 */ u8 mBlendAmplitude;
    // UpdateFade's sine angle, 180 per turn ((mBlendAngle << 16) / 180), counted down; IsFadeDone is FALSE below 0x5a
    /* d2 */ u8 mBlendAngle;
    /* d3 */ u8 mPlaneBlendEnabled;
    /* d4 */ u8 mCursorStep;
    /* d5 */ u8 mSubBgIndex;
    /* d6 */ u8 mUnk_d6;
    /* d7 */ u8 mLogoVisible; // gates DrawLogo, which also steps mMessageWindow once mLogoY reaches 0x60
    /* d8 */ u8 mLogoY;
    /* d9 */ PAD(0xd9, 0xda);
    /* da */ s16 mScrollY;
    /* dc */

    TitleScene();
    /* 00 */ virtual ~TitleScene() override;
    /* 08 */ virtual void Update() override;
    /* 0c */

    void InitDisplay(); // display setup (menu)
    // display setup before each new SaveWindow; unlike InitDisplay: no main BG control, sub BG0 / BG1 only, planes
    // 0x11 / 0x13
    void InitSaveWindowDisplay();
    void LoadGraphics();        // 02223e3c
    void DrawObj03Cell();
    void DrawMenuCells();
    void RefreshMenuAnimations();
    void LoadSystemText();      // 022246c0
    void PrintText(MessageFile *file, u32 index); // 02224908
    void DrawText();            // 02224a68
    void ClearText();           // 02224b14
    void SetMessage(u32 index); // 02224b34
    void SetPlaneBlend(u8 enabled); // 02224b78
    void UpdateFade();           // 02224be0
    BOOL IsFadeDone(BOOL tick);  // 02224ca4
    void UploadSubBg(u32 index); // 02224ce0
    BOOL UpdateCursorCell();     // 02224d64
    static void ApplyScroll(void *arg); // 02224edc, the vblank callback
    void DrawLogo();             // 02224f08
};
