#pragma once

#include "global.h"
#include "types.h"

#include "Scene/SceneBase.hpp"

class CellAnimation;
class SelectWindow;
struct MessageFile;

// Result window kinds: the values of mResultWindowKind (NO_MAP when data_020d433c.mUnk_8a_4 is set). WITH_MAP builds
// the three-item window whose second item requests SCENE_MAP; NO_MAP builds the two-item window without it.
#define GAME_OVER_RESULT_WINDOW_WITH_MAP 0
#define GAME_OVER_RESULT_WINDOW_NO_MAP 1

// Overlay 6: the game-over screen with its retry prompt.
class GameOverScene : public SceneBase {
public:
    /* 00 (base) */
    /* 0c */ CellAnimation *mCellAnims[1];
    /* 10 */ SelectWindow *mResultWindow;
    /* 14 */ SelectWindow *mRetryWindow;
    /* 18 */ MessageFile *mMessages;
    /* 1c */ void *mTextBuffer;
    /* 20 */ void *mGlyphBuffer;
    /* 24 */ u32 mTextBufferSize;
    /* 28 */ u32 mGlyphCount;
    /* 2c */ u8 mState;
    /* 2d */ u8 mResultWindowKind;
    /* 2e */ u8 mSelection;
    /* 30 */

    GameOverScene();

    /* 00 */ virtual ~GameOverScene() override;
    /* 08 */ virtual void Update() override;
    /* 0c */

    void InitDisplay();
    void LoadGraphics();
    void CreateResultWindow();
    void SetupText();
    void DrawText();
    void ShowRetryWindow(BOOL show);
    void Draw();
};
