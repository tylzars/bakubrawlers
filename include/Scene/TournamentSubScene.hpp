#pragma once

#include "global.h"
#include "types.h"

#include "Scene/SceneBase.hpp"

class MessageWindow;
class SelectWindow;
class UnkStruct_020651bc;
class UnkStruct_02074938;
class UnkStruct_020758f4;
struct MessageFile;

struct TournamentEntry {
    /* 0 */ u8 mId;
    /* 2 */ u16 mUnk_02;
    /* 4 */ u32 mUnk_04;
    /* 8 */
};

// Overlay 2: a choice screen created by TournamentScene.
class TournamentSubScene : public SceneBase {
public:
    /* 00 (base) */
    /* 0c */ UnkStruct_020758f4 *mUnk_0c;
    /* 10 */ UnkStruct_02074938 *mUnk_10;
    /* 14 */ SelectWindow *mWindow;
    /* 18 */ TournamentEntry mEntries[3];
    /* 30 */ MessageFile *mMessages;
    /* 34 */ void *mTextBuffer;
    /* 38 */ void *mGlyphBuffer;
    /* 3c */ u32 mTextBufferSize;
    /* 40 */ u32 mGlyphCount;
    /* 44 */ MessageWindow *mMessage;
    /* 48 */ UnkStruct_020651bc *mUnk_48;
    /* 4c */ s32 mResult;
    /* 50 */ u8 mUnk_50;
    /* 51 */ s8 mUnk_51;
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

    void func_ov002_0221f930();
    void func_ov002_0221fb24();
    void Draw(); // 02220178
    void func_ov002_02220184();
    s32 func_ov002_022202cc();
    s32 func_ov002_022203ec();
    void func_ov002_02220618();
    void func_ov002_02220680();
    void func_ov002_02220830();
    void ClearTextBuffer(); // 022209e4
};
