#pragma once

#include "global.h"
#include "types.h"

#include "Scene/SceneBase.hpp"

class CellAnimation;
class SelectWindow;
class UnkStruct_02065c40;
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

// Overlay 5: the story map (town info panels, tabs and drag-scrolled background).
class MapScene : public SceneBase {
public:
    /* 00 (base) */
    /* 0c */ CellAnimation *mCellAnims[6];
    /* 24 */ UnkStruct_02065c40 *mEvent;
    /* 28 */ SelectWindow *mMenu;
    /* 2c */ SelectWindow *mPanelWindow;
    /* 30 */ void *mGlyphBuffer;
    /* 34 */ u16 mGlyphCount;
    /* 36 */ u16 mUnk_36;
    /* 38 */ u16 mUnk_38;
    /* 3a */ PAD(0x3a, 0x3c);
    /* 3c */ TextSlot mText[2];
    /* 64 */ u8 mUnk_64;
    /* 65 */ u8 mUnk_65;
    /* 66 */ s16 mNextState;
    /* 68 */ u8 mUnk_68;
    /* 69 */ u8 mUnk_69;
    /* 6a */ u8 mTab;
    /* 6b */ PAD(0x6b, 0x6c);
    /* 6c */ u16 mScrollPos;
    /* 6e */ u16 mScrollTarget;
    /* 70 */ u16 mParallax;
    /* 72 */ u8 mDragging;
    /* 73 */ u8 mLastTouchX;
    /* 74 */ s8 mUnk_74;
    /* 75 */ s8 mUnk_75;
    /* 76 */ u8 mPage;
    /* 77 */ u8 mPageVisible;
    /* 78 */ PAD(0x78, 0x79);
    /* 79 */ u8 mUnk_79;
    /* 7a */ u16 mTextWidth;
    /* 7c */ u8 mUnk_7c;
    /* 7d */ u8 mUnk_7d;
    union {
        /* 7e */ u8 mUnk_7e;
        struct {
            /* 7e */ u8 mUnk_7e_0 : 1;
            /* 7e */ u8 mUnk_7e_1 : 1;
            /* 7e */ u8 mUnk_7e_2 : 1;
            /* 7e */ u8 mUnk_7e_3 : 1;
            /* 7e */ u8 mUnk_7e_4 : 4;
        };
    };
    /* 7f */

    MapScene();

    /* 00 */ virtual ~MapScene() override;
    /* 08 */ virtual void Update() override;
    /* 0c */

    void Init();                      // 0221a4e0
    void func_ov005_0221a598();
    void InitDisplay();               // 0221a63c
    void LoadWindowGraphics();        // 0221a824
    void func_ov005_0221a9ec();
    void LoadMapBackground();         // 0221aaa4
    void func_ov005_0221abf4();
    BOOL func_ov005_0221b1a0();
    BOOL func_ov005_0221b2ac();
    u8 UpdateMenu();                  // 0221b2dc
    void func_ov005_0221b364(u8 index, BOOL changed);
    void CreateMenu();                // 0221b3c0
    void UpdateDrag();                // 0221b4c8
    BOOL func_ov005_0221b6f0();
    s16 func_ov005_0221b760(u32 index);
    void func_ov005_0221b7e0();
    void DrawPopup();                 // 0221b824
    void DrawMoney();                 // 0221bc28
    void func_ov005_0221bd48();
    void func_ov005_0221bde8(u32 index);
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

void func_ov005_0221b67c(MapScene *scene); // scroll callback for func_020254f0
