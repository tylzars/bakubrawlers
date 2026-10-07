#pragma once
#include "global.h"
#include "types.h"
#include "Scene/SceneBase.hpp"
#include "UI/SelectWindow.hpp"
#include "UI/MessageWindow.hpp"
#include "Graphics/CellAnimation.hpp"
#include "Graphics/Camera.hpp"
#include "System/FileSystem.hpp"
#include "Graphics/Model.hpp"

struct TextSlot {
    /* 00 */ s8 mUnk_00;
    /* 01 */ PAD(0x01, 0x04);
    /* 04 */ u32 mActive;
    /* 08 */ u16 mWidth;
    /* 0a */ u16 mHeight;
    /* 0c */
};

// Appearance of the player's character: the bit fields stored in the save data at +0x14.
struct CharacterLook {
    /* 00 */ u16 mUnk_00_0 : 1;
    /* 00 */ u16 mUnk_00_1 : 3;
    /* 00 */ u16 mUnk_00_4 : 3;
    /* 00 */ u16 mUnk_00_7 : 3;
    /* 00 */ u16 mUnk_00_10 : 3;
    /* 00 */ u16 mUnk_00_13 : 3;
    /* 02 */ u16 mUnk_02_0 : 3;
    /* 02 */ u16 mUnk_02_3 : 3;
    /* 02 */ u16 mUnk_02_6 : 3;
    /* 02 */ u16 mUnk_02_9 : 3;
    /* 02 */ u16 mUnk_02_12 : 3;
    /* 02 */ u16 mUnk_02_15 : 1;
    /* 04 */ u16 mUnk_04_0 : 3;
    /* 04 */ u16 mUnk_04_3 : 5;
    /* 04 */ u16 mUnk_04_8 : 3;
    /* 04 */ u16 mUnk_04_11 : 3;
    /* 04 */ u16 mUnk_04_14 : 2;
    /* 06 */ u16 mUnk_06_0 : 3;
    /* 06 */ u16 mUnk_06_3 : 5;
    /* 06 */ u16 mUnk_06_8 : 8;
    /* 08 */
};

// A list of choices (hair styles, colours...) and the one that is selected.
struct LookChoices {
    /* 00 */ u8 mIndex;
    /* 01 */ u8 mCount;
    /* 02 */ u8 mValues[8];
    /* 0a */
};

class CharacterEditScene : public SceneBase {
public:
    /* 0c */ SelectWindow *mWindows[3];
    /* 18 */ u32 mUnk_18;
    /* 1c */ u32 mUnk_1c;
    /* 20 */ u32 mUnk_20;
    /* 24 */ UnkStruct_0205401c *mModel;
    /* 28 */ Camera *mCamera;
    /* 2c */ UnkStruct_02029ca8 *mPalette;
    /* 30 */ u8 mState;
    /* 31 */ u8 mUnk_31;
    /* 32 */ u16 mUnk_32;
    /* 34 */ LookChoices mLists[16];
    /* d4 */ TextSlot mTexts[6];
    /* 11c */ u32 *mTextBuffers[6];
    /* 134 */ void *mUnk_134;
    /* 138 */ u32 mUnk_138;
    /* 13c */ u32 mUnk_13c;
    /* 140 */ void *mUnk_140;
    /* 144 */ MessageWindow *mMessage;
    /* 148 */ u8 mUnk_148;
    /* 14c */ u32 mUnk_14c;
    /* 150 */ ModelInstance *mUnk_150;
    /* 154 */ u8 mUnk_154;
    /* 155 */ u8 mUnk_155[3];
    /* 158 */ u8 mUnk_158;
    /* 159 */ u8 mUnk_159;
    /* 15a */ u8 mUnk_15a;
    /* 15b */ u8 mUnk_15b;
    /* 15c */ u8 mUnk_15c;
    /* 15d */ u8 mUnk_15d;
    /* 15e */ u8 mUnk_15e;
    /* 15f */ u8 mUnk_15f;
    /* 160 */ u8 *mUnk_160;
    /* 164 */ SelectWindowCursor *mCursor;
    /* 168 */ CellAnimation *mCells[16];
    /* 1a8 */ u32 mUnk_1a8;
    /* 1ac */ s8 mUnk_1ac;
    /* 1ad */ PAD(0x1ad, 0x1b0);
    /* 1b0 */ u32 mUnk_1b0;
    /* 1b4 */ u8 mUnk_1b4;
    /* 1b5 */ PAD(0x1b5, 0x1b8);
    /* 1b8 */ u32 mUnk_1b8;
    /* 1bc */ void *mUnk_1bc;
    /* 1c0 */ void *mUnk_1c0;
    /* 1c4 */ u32 mUnk_1c4; // size of mUnk_1bc
    /* 1c8 */ u32 mUnk_1c8;
    /* 1cc */ u32 mUnk_1cc;
    /* 1d0 */ u8 mUnk_1d0;
    /* 1d1 */ PAD(0x1d1, 0x1d2);
    /* 1d2 */ u16 mUnk_1d2;
    /* 1d4 */ void *mUnk_1d4;
    /* 1d8 */ void *mUnk_1d8;
    /* 1dc */ u32 mUnk_1dc;
    /* 1e0 */ u32 mUnk_1e0;
    /* 1e4 */ CellAnimation *mUnk_1e4;
    /* 1e8 */

    CharacterEditScene(u32 unk);
    virtual ~CharacterEditScene();
    virtual void Update();

    void func_020638a8(u32 v);
    void func_0205d5fc();
    void func_0205e6ac();
    void func_0205e958();
    void func_0205f3c0();
    void func_0205f698();
    void func_0205f970();
    void func_0205fc48();
    void func_0205ff20();
    void func_020604e8();
    void func_02060cd4();
    void func_02060fd0();
    void func_020612cc();
    void func_0206183c();
    void func_020642ac();
    void func_02062fe4(const char *chars, const char *palette, const char *cells, const char *palette2);
    void func_02063218();
    BOOL func_0206332c();
    void func_02063570(CharacterLook *look);
    void func_020645fc();
    void func_02064964();
    void func_020649e4();
    void func_02064b58();
    void func_02064bb8();
    void func_02064cd4(u32 flag);
    void func_02064d7c();
    void func_02064ed8();
    void func_0205d0fc(u32 slot, u32 message, u32 value);
    void func_0205d2b4();
    void func_0205d4cc();
    void func_02061c3c();
    void func_02062194();
    void func_02064f98();
    void func_0205d090();
    void func_02064024();
    void func_02064f80();
    void func_02063a6c(u32 i, u32 v);
    void func_02063a88(u32 i, u32 a, u32 b);
    void func_02063aa8(u32 i, u32 a, u32 b, u32 c, u32 d);
    void func_02063ad8(u32 v);
    void func_020638b8();
    void func_02063b10(u32 arg);
    void func_02063c64();
    void func_02063d10();
    void func_02063dac();
    BOOL func_02063e20();
    void func_020637a8();
    void func_020637dc();
};
