#pragma once

#include "global.h"
#include "types.h"

#include "Scene/SceneBase.hpp"

class CellAnimation;
class MessageWindow;
struct MessageFile;

struct NameEntryObjInner {
    /* 00 */ PAD(0x00, 0xac);
    /* ac */ u32 mUnk_ac;
};

struct NameEntryObjMid {
    /* 00 */ NameEntryObjInner *mInner;
};

// Owned by the scene and deleted through its virtual destructor.
struct NameEntryObject {
    /* 000 (vtable) */
    /* 004 */ NameEntryObjMid *mMid;
    /* 008 */ PAD(0x008, 0x130);
    /* 130 */ u32 mUnk_130;

    virtual ~NameEntryObject() {}
};

// Overlay 0: player name entry (8x4 character grid over two tabs, up to 12 UTF-16 characters).
class NameEntryScene : public SceneBase {
public:
    /* 00 (base) */
    /* 0c */ MessageFile *mCharTable; // mes_Alphabet.mes
    /* 10 */ MessageFile *mMessages;  // mes_name.mes
    /* 14 */ MessageWindow *mMessage;
    /* 18 */ void *mTextBuffer;
    /* 1c */ void *mGlyphBuffer;
    /* 20 */ u32 mTextBufferSize;
    /* 24 */ u32 mGlyphCount;
    /* 28 */ CellAnimation *mCellAnims[3];
    /* 34 */ NameEntryObject *mObj34;
    /* 38 */ NameEntryObject *mObj38;
    /* 3c */ u8 *mBuf3c;
    /* 40 */ u8 *mBuf40;
    /* 44 */ u8 *mBuf44;
    /* 48 */ u8 *mBuf48;
    /* 4c */ u8 mMaxLength;
    /* 4d */ u8 mLength;
    /* 4e */ u16 mName[12];
    /* 66 */ u8 mUnk_66;
    /* 67 */ u8 mState;
    /* 68 */ u8 mCol;
    /* 69 */ u8 mRow;
    /* 6a */ u8 mUnk_6a;
    /* 6b */ u8 mTab;
    /* 6c */ u8 mUnk_6c;
    /* 6d */ u8 mMode;
    /* 6e */ PAD(0x6e, 0x70);
    /* 70 */ s32 mUnk_70;
    /* 74 */ u8 mUnk_74;
    /* 75 */

    NameEntryScene(u8 mode);

    /* 00 */ virtual ~NameEntryScene() override;
    /* 08 */ virtual void Update() override; // not decompiled yet
    /* 0c */

    void InitDisplay(); // 02219844
    void func_ov000_0221b190();
    void func_ov000_0221b22c();
    void func_ov000_0221b2a0();
    void Backspace(); // 0221b438
    void AddChar(s32 index); // 0221b4a8
    void Load(); // 0221b6b4
    void RenderName(); // 0221b9e8
    void func_ov000_0221bc98();
    void ClearTextBuffer(); // 0221bd54
    void CheckSpecialName(); // 0221bd88
    void PrintMessage(); // 0221c024
    void func_ov000_0221c1e4();
};
