#pragma once
#include "global.h"
#include "types.h"
#include "Scene/SceneBase.hpp"

class CellAnimation;
class MessageWindow;
class SelectWindow;
struct MessageFile;

// Requests: the values AttrSelScene passes its parent TitleScene through RequestScene (mRequest). NAME_ENTRY is set
// when the cockpit button ends the scene (window result 2) and the parent creates a NameEntryScene again;
// CHARACTER_EDIT is set after the attribute is stored in the save data and the parent creates a CharacterEditScene.
#define ATTR_SEL_REQUEST_NAME_ENTRY 1
#define ATTR_SEL_REQUEST_CHARACTER_EDIT 2

// Overlay 0 scene with vtable 0x022270cc (files in /Game/AttrSel): choosing the player's attribute.
void AttrSelTransitionCallback(void *scene); // frame callback of AttrSelScene::TransitionStep

class AttrSelScene : public SceneBase {
public:
    /* 0c */ u8 mState;
    /* 0d */ u8 mAttrib;
    /* 0e */ u8 mPulseState;
    /* 0f */ u8 mPulseAlpha; // written to reg_G2S_DB_BLDALPHA with 0x1f00 at the end of PulseBlend
    /* 10 */ u8 mPulseDecreasing : 1; // PulseBlend steps mPulseAlpha down while set, up while clear; flips at 0x1f / 7
    /* 10 */ u8 mUnk_10_1 : 7;
    /* 11 */ u8 mPulseFrames; // frames in pulse state 1; at 4 mPulseAlpha steps and this restarts at 0
    /* 12 */ s16 mFade;
    /* 14 */ u8 mTransitionState;
    /* 15 */ u8 mRequest;
    /* 16 */ u8 mBlendAlphaLowByte;
    /* 17 */ u8 mBlendAlphaHighByte;
    // times mPulseAlpha fell back to 7 in pulse state 2; at 6 PulseBlend goes back to state 1; only the ctor zeroes it
    /* 18 */ u8 mPulseCount;
    /* 19 */ PAD(0x19, 0x1c);
    /* 1c */ u32 mGlyphCount[2];
    /* 24 */ u32 mBufferSize[2];
    /* 2c */ void *mGlyphBuffer[2];
    /* 34 */ void *mTextBuffer[2];
    /* 3c */ MessageFile *mMessages;
    /* 40 */ CellAnimation *mCellAnims[8];
    /* 60 */ MessageWindow *mMessageWindow;
    /* 64 */ SelectWindow *mSelectWindow;
    /* 68 */

    AttrSelScene();
    /* 00 */ virtual ~AttrSelScene() override;
    /* 08 */ virtual void Update() override;
    /* 0c */

    void InitDisplay();   // 0221e298
    void LoadGraphics();  // 0221e3fc
    void PrintName(void *buffer, u32 size, u32 vram, u16 width, u16 height, u32 unk); // 0221eae8
    void PrintMessage(MessageFile *file, u16 index, void *buffer, u32 size, u32 vram, u16 width, u16 height, u32 unk); // 0221ecbc
    void LoadSubBg();     // 0221ee28
    void TransitionStep(); // 0221ef68
    void PulseBlend();     // 0221f0e0
    void DrawText();       // 0221f2b8
    void Draw();           // 0221f3fc
    int Distance(s16 x1, s16 y1, s16 x2, s16 y2); // 0221f474
};
