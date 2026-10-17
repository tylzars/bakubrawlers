#pragma once

#include "global.h"
#include "types.h"

#include "Scene/SceneBase.hpp"

class CellAnimation;
class MessageWindow;
class SelectWindow;
class ScriptPlayer;
struct MessageFile;

struct GlyphSlot {
    /* 0 */ void *mBuffer;
    /* 4 */ u32 mCount;
    /* 8 */
};

// Overlay 5: the tutorial list.
class TutorialScene : public SceneBase {
public:
    /* 00 (base) */
    /* 0c */ CellAnimation *mCellAnims[4];
    /* 1c */ ScriptPlayer *mEvent;
    /* 20 */ SelectWindow *mWindow;
    /* 24 */ MessageWindow *mMessageWindow;
    /* 28 */ MessageFile *mMessages[2];
    /* 30 */ GlyphSlot mGlyphs[2];
    /* 40 */ u8 mFlags[7];
    /* 47 */ u8 mState;
    /* 48 */ u8 mUnk_48;
    /* 49 */ u8 mSelection;
    /* 4a */ u8 mScroll;
    /* 4b */ u8 mUnk_4b;
    /* 4c */ u8 mUnk_4c;
    /* 4d */ u8 mUnk_4d;
    /* 4e */

    TutorialScene();

    /* 00 */ virtual ~TutorialScene() override;
    /* 08 */ virtual void Update() override;
    /* 0c */

    void LoadGraphics();      // 0221eb64
    void LoadListWindow();    // 0221f118; builds mWindow from sTutorialThreeItems (two choices), not a list
    void ShowText(int index); // 0221f474
    void LoadListText();      // 0221f5c4
    u8 UpdateList();          // 0221f860
    void UpdateScroll();      // 0221fb34
    void DrawList();          // 0221fb78
};
