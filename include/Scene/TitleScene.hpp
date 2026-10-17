#pragma once
#include "global.h"
#include "types.h"
#include "Scene/SceneBase.hpp"

class CellAnimation;
class SelectWindow;
class MessageWindow;
struct SelectWindowCursor;
struct MessageFile;

// Overlay 0 scene with vtable 0x02227920 (title screen: movie bknd_op, main menu, then CharacterEditScene / save scene).
class TitleScene : public SceneBase {
public:
    /* 0c */ CellAnimation *mCells[8];
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
    /* c8 */ u16 mUnk_c8;
    /* ca */ PAD(0xca, 0xcb);
    /* cb */ u8 mState;
    /* cc */ u8 mTimer;
    /* cd */ u8 mMenuVisible;
    /* ce */ u8 mMenuSelection;
    /* cf */ u8 mMessageIndex;
    /* d0 */ u8 mUnk_d0;
    /* d1 */ u8 mBlendAmplitude;
    /* d2 */ u8 mBlendAngle;
    /* d3 */ u8 mWindowMode;
    /* d4 */ u8 mCursorStep;
    /* d5 */ u8 mSubBgIndex;
    /* d6 */ u8 mUnk_d6;
    /* d7 */ u8 mLogoVisible;
    /* d8 */ u8 mLogoY;
    /* d9 */ PAD(0xd9, 0xda);
    /* da */ s16 mScrollY;
    /* dc */

    TitleScene();
    /* 00 */ virtual ~TitleScene() override;
    /* 08 */ virtual void Update() override;
    /* 0c */

    void InitDisplay(); // display setup (menu)
    void func_ov000_02223cdc(); // display setup (movie)
    void LoadGraphics();        // 02223e3c
    void func_ov000_022245e8();
    void DrawMenuCells();
    void RefreshMenuAnimations();
    void LoadSystemText();      // 022246c0
    void PrintText(MessageFile *file, u32 index); // 02224908
    void DrawText();            // 02224a68
    void ClearText();           // 02224b14
    void SetMessage(u32 index); // 02224b34
    void SetWindowMode(u8 mode); // 02224b78
    void UpdateFade();           // 02224be0
    BOOL IsFadeDone(BOOL tick);  // 02224ca4
    void UploadSubBg(u32 index); // 02224ce0
    BOOL UpdateCursorCell();     // 02224d64
    static void ApplyScroll(void *arg); // 02224edc, the vblank callback
    void DrawLogo();             // 02224f08
};
