#pragma once
#include "global.h"
#include "types.h"
#include "Scene/SceneBase.hpp"

class CellAnimation;
class MessageWindow;
class SelectWindow;
struct MessageFile;

// Overlay 0 scene with vtable 0x022270cc (files in /Game/AttrSel): choosing the player's attribute.
void func_ov000_0221f080(void *scene); // frame callback of AttrSelScene::TransitionStep

class AttrSelScene : public SceneBase {
public:
    /* 0c */ u8 mUnk_0c;
    /* 0d */ u8 mAttrib;
    /* 0e */ u8 mUnk_0e;
    /* 0f */ u8 mUnk_0f;
    /* 10 */ u8 mUnk_10_0 : 1;
    /* 10 */ u8 mUnk_10_1 : 7;
    /* 11 */ u8 mUnk_11;
    /* 12 */ s16 mFade;
    /* 14 */ u8 mState;
    /* 15 */ u8 mUnk_15;
    /* 16 */ u8 mUnk_16;
    /* 17 */ u8 mUnk_17;
    /* 18 */ u8 mUnk_18;
    /* 19 */ PAD(0x19, 0x1c);
    /* 1c */ u32 mGlyphCount[2];
    /* 24 */ u32 mBufferSize[2];
    /* 2c */ void *mGlyphBuffer[2];
    /* 34 */ void *mTextBuffer[2];
    /* 3c */ MessageFile *mMessages;
    /* 40 */ CellAnimation *mCells[8];
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
