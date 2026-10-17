#pragma once

#include "global.h"
#include "types.h"

#include "Scene/SceneBase.hpp"

class BattleCommandList;
class CellAnimation;
class MessageWindow;
class SelectWindow;
struct BattleCellPair;
struct MessageFile;

// Scene with cockpit cells, a BattleCommandList and a SelectWindow of seven buttons (0-4 and 5-6 enabled in turn)
// (vtable 0x020b9038, 0x6c bytes); created by UnkStruct_02076058 and by ListScene (ListScene.cpp:1199).
class UnkStruct_02071d24 : public SceneBase {
public:
    /* 0c */ MessageFile *mBakuganNameMessages;
    /* 10 */ MessageFile *mResultMessages;
    /* 14 */ MessageWindow *mMessageWindow;
    /* 18 */ BattleCommandList *mCommandList;
    /* 1c */ CellAnimation *mBakuganSubCellAnim;
    /* 20 */ SelectWindow *mWindow;
    /* 24 */ void *mTextBuffer;
    /* 28 */ void *mGlyphBuffer;
    /* 2c */ u32 mTextBufferSize;
    /* 30 */ u32 mGlyphCount;
    /* 34 */ BattleCellPair *mUnk_34[6];
    /* 4c */ u8 mUnk_4c;
    /* 4d */ u8 mState;
    /* 4e */ u8 mBakuganIndex;
    /* 4f */ u8 mUnk_4f;
    /* 50 */ u8 mUnk_50;
    /* 51 */ u8 mUnk_51;
    /* 52 */ u8 mUnk_52;
    /* 53 */ u8 mUnk_53;
    /* 54 */ s32 mPendingPlanes;
    /* 58 */ u8 mBakuganIds[6];
    /* 5e */ u8 mAttributes[6];
    // per entry: the gain passed to SetBakugan (UnkStruct_02076058.cpp:230-232), capped at 9; loop bound of the
    // func_02072b60 sum (:303-306) and tested in func_02072d20 (:354); decremented once per confirmed press in
    // func_020733d4 (:571)
    /* 64 */ u8 mUnk_64[6];
    /* 6a */ PAD(0x6a, 0x6c);
    /* 6c */

    UnkStruct_02071d24(u32 unk1);
    virtual ~UnkStruct_02071d24();
    virtual void Update();
    void LoadGraphics();
    void func_02072b14();
    void func_02072b60();
    void func_02072c40();
    BOOL func_02072d20();
    BOOL func_02072d84();
    void func_02072dfc();
    void func_02072e7c();
    void RefreshItems();
    void func_020733d4();
    void LoadMessages();
    void PrintText();
    void DrawText();
    void ClearText();
    void SetBakugan(u32 index, u32 unk2, u32 unk3, u8 unk4);
};
