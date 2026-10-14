#pragma once

#include "global.h"
#include "types.h"

#include "Scene/SceneBase.hpp"

class CellAnimation;
class MessageWindow;
class SelectWindow;
class UnkStruct_020651bc;
struct MessageFile;

// A cell animation entry (one per button or marker).
struct CellEntry {
    /* 00 */ CellAnimation *mAnim;
    /* 04 */ s16 mX;
    /* 06 */ s16 mY;
    /* 08 */ u8 mVisible;
    /* 09 */ u8 mUnk_09;
    /* 0a */ u8 mUnk_0a;
    /* 0b */ PAD(0x0b, 0x0c);
    /* 0c */
};

// Input state of one player (local or received), 0x10 bytes each.
struct FieldInput {
    /* 00 */ u16 mKeys;
    /* 02 */ u8 mUnk_02;
    /* 03 */ u8 mTouching;
    /* 04 */ u8 mTouchX;
    /* 05 */ u8 mTouchY;
    /* 06 */ u8 mUnk_06;
    /* 07 */ u8 mUnk_07;
    /* 08 */ u8 mUnk_08;
    /* 09 */ PAD(0x09, 0x0a);
    /* 0a */ u8 mUnk_0a;
    /* 0b */ u8 mUnk_0b;
    /* 0c */ u8 mUnk_0c;
    /* 0d */ u8 mUnk_0d;
    /* 0e */ u8 mUnk_0e;
    /* 0f */ u8 mUnk_0f;
    /* 10 */
};

// Overlay 1: battle field selection (select_field_*); a state machine on mState.
class FieldSelectScene : public SceneBase {
public:
    /* 00 (base) */
    /* 0c */ CellEntry mMain;
    /* 18 */ CellEntry mCells[9];
    /* 84 */ CellEntry mCells2[9];
    /* f0 */ SelectWindow *mWindows[4];
    /* 100 */ MessageWindow *mMessage;
    /* 104 */ UnkStruct_020651bc *mPanel[2];
    union {
        /* 10c */ u8 mUnk_10c[0x40];
        /* 10c */ FieldInput mInput[4];
    };
    /* 14c */ u8 mUnk_14c[0x20];
    /* 16c */ void *mFiles[10][3];
    /* 1e4 */ u32 mFileSizes[10][3];
    union {
        /* 25c */ u8 mUnk_25c[10];
        /* 25c */ struct { u8 mValue; u8 mCount; } mOptions[5];
        /* 25c */ u8 mOptionBytes[5][2];
    };
    /* 266 */ u8 mUnk_266[4][0x12];
    /* 2ae */ u8 mUnk_2ae[4][9];
    /* 2d2 */ u8 mUnk_2d2[0x12];
    /* 2e4 */ u8 mUnk_2e4[9];
    /* 2ed */ u8 mUnk_2ed[7];
    /* 2f4 */ MessageFile *mUnk_2f4;
    /* 2f8 */ void *mUnk_2f8;
    /* 2fc */ void *mUnk_2fc;
    /* 300 */ u32 mUnk_300;
    /* 304 */ u32 mUnk_304;
    /* 308 */ MessageFile *mUnk_308;
    /* 30c */ void *mUnk_30c[4];
    /* 31c */ void *mUnk_31c;
    /* 320 */ u32 mUnk_320;
    /* 324 */ u32 mUnk_324;
    /* 328 */ u8 mState;
    /* 329 */ u8 mUnk_329;
    /* 32a */ u8 mUnk_32a;
    /* 32b */ u8 mUnk_32b;
    /* 32c */ u8 mUnk_32c;
    /* 32d */ u8 mUnk_32d;
    /* 32e */ u8 mUnk_32e;
    /* 32f */ u8 mUnk_32f;
    /* 330 */ u8 mUnk_330;
    /* 331 */ u8 mUnk_331;
    /* 332 */ u8 mUnk_332;
    /* 333 */ u8 mUnk_333;
    /* 334 */ u8 mUnk_334;
    /* 335 */ u8 mUnk_335;
    /* 336 */ u8 mUnk_336;
    /* 337 */ u8 mUnk_337;
    /* 338 */ u8 mUnk_338;
    /* 339 */ u8 mUnk_339;
    /* 33a */ u8 mUnk_33a;
    /* 33b */ u8 mUnk_33b;
    /* 33c */ u8 mUnk_33c;
    /* 33d */ u8 mUnk_33d;
    /* 33e */ u8 mUnk_33e;
    /* 33f */ u8 mUnk_33f;
    /* 340 */ u8 mUnk_340;
    /* 341 */ u8 mUnk_341;
    /* 342 */ PAD(0x342, 0x344);
    /* 344 */ s32 mUnk_344;
    /* 348 */ u8 mUnk_348;
    /* 349 */

    FieldSelectScene(u8 unk1);

    /* 00 */ virtual ~FieldSelectScene() override;
    /* 08 */ virtual void Update() override;
    /* 0c */

    void func_ov001_02222040();
    BOOL func_ov001_02221c0c();
    BOOL func_ov001_02221f1c();
    BOOL func_ov001_02221aa8(u32 unk1);
    u32 func_ov001_02222e44();
    void func_ov001_02226cb0();
    void func_ov001_02226d74();
    void func_ov001_02226c38(SelectWindow *window);
    BOOL func_ov001_02225240();
    void func_ov001_02226430();
    u32 func_ov001_02225214();
    u32 func_ov001_02224178();
    u32 func_ov001_02224538();
    u32 func_ov001_02224be0();
    u32 func_ov001_02224df0();
    u32 func_ov001_02223f5c();
    u32 func_ov001_02226d48(u32 unk1);
    // provisional declarations (signatures guessed from callers; fix when decompiling the function)
    void func_ov001_02222e10();
    void func_ov001_0221fa94();
    void func_ov001_022231ec(u32 slot, u32 value, u32 playSound);
    BOOL func_ov001_02223430(u8 slot);
    BOOL func_ov001_022237d0(u32 slot);
    BOOL func_ov001_02223914();
    void func_ov001_02223d70(u32 unk1);
    void func_ov001_02223e5c();
    void func_ov001_02225150(u32 unk1);
    void func_ov001_02225338();
    void func_ov001_022253f0(u32 unk1, u32 unk2, u32 unk3);
    s16 func_ov001_02225628(u32 unk1);
    s16 func_ov001_0222565c(u32 unk1);
    s16 func_ov001_02225698(u32 unk1);
    s16 func_ov001_022256c8(u32 unk1);
    void func_ov001_022262d0(u32 unk1);
    void func_ov001_0222657c();
    void func_ov001_022267d0(u8 unk1);
    void func_ov001_02226b18();
    void func_ov001_02226bf8(u32 unk1);
    void func_ov001_02226d18(u32 unk1);
    void func_ov001_022256e4();
    void func_ov001_022260b8();
    void func_ov001_0222659c();
    void func_ov001_0221f23c();
    void func_ov001_0221f4e8();
};
