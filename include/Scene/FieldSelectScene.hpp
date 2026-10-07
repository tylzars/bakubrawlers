#pragma once

#include "global.h"
#include "types.h"

#include "Scene/SceneBase.hpp"

class CellAnimation;
class MessageWindow;
class SelectWindow;
class UnkStruct_020651bc;

// A cell animation entry (one per button or marker).
struct CellEntry {
    /* 00 */ CellAnimation *mAnim;
    /* 04 */ s16 mX;
    /* 06 */ s16 mY;
    /* 08 */ u8 mVisible;
    /* 09 */ u8 mUnk_09;
    /* 0a */ PAD(0x0a, 0x0c);
    /* 0c */
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
    /* 10c */ u8 mUnk_10c[0x40];
    /* 14c */ u8 mUnk_14c[0x20];
    /* 16c */ void *mFiles[10][3];
    /* 1e4 */ u32 mFileSizes[10][3];
    /* 25c */ u8 mUnk_25c[10];
    /* 266 */ u8 mUnk_266[2][0x12];
    /* 28a */ PAD(0x28a, 0x2ae);
    /* 2ae */ u8 mUnk_2ae[2][9];
    /* 2c0 */ PAD(0x2c0, 0x2d2);
    /* 2d2 */ u8 mUnk_2d2[0x12];
    /* 2e4 */ u8 mUnk_2e4[9];
    /* 2ed */ u8 mUnk_2ed[7];
    /* 2f4 */ void *mUnk_2f4;
    /* 2f8 */ void *mUnk_2f8;
    /* 2fc */ void *mUnk_2fc;
    /* 300 */ u32 mUnk_300;
    /* 304 */ u32 mUnk_304;
    /* 308 */ void *mUnk_308;
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
    /* 331 */ PAD(0x331, 0x332);
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
    /* 33c */ PAD(0x33c, 0x33d);
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
    void func_ov001_022256e4();
    void func_ov001_022260b8();
    void func_ov001_0222659c();
    void func_ov001_0221f23c();
    void func_ov001_0221f4e8();
};
