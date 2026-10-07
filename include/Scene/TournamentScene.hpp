#pragma once

#include "global.h"
#include "types.h"

#include "Scene/SceneBase.hpp"

class CellAnimation;
class MessageWindow;
class SelectWindow;
class UnkStruct_020651bc;
class UnkStruct_02065c40;
struct MessageFile;

// Overlay 2: the tournament bracket.
class TournamentScene : public SceneBase {
public:
    /* 00 (base) */
    /* 0c */ CellAnimation *mCellsA[5];
    /* 20 */ CellAnimation *mCell20;
    /* 24 */ CellAnimation *mCellsB[5];
    /* 38 */ UnkStruct_020651bc *mObj38;
    /* 3c */ UnkStruct_02065c40 *mEvent;
    /* 40 */ SelectWindow *mMenu;
    /* 44 */ SelectWindow *mWindow;
    /* 48 */ MessageFile *mMessages[4];
    /* 58 */ MessageWindow *mMessage;
    /* 5c */ void *mTextBuffers[4];
    /* 6c */ void *mGlyphBuffers[4];
    /* 7c */ u32 mTextSizes[4];
    /* 8c */ u32 mGlyphCounts[4];
    /* 9c */ u16 mTextWidth;
    /* 9e */ u16 mTextHeight;
    /* a0 */ s16 mUnk_a0;
    /* a2 */ s16 mUnk_a2;
    /* a4 */ s16 mUnk_a4;
    /* a6 */ s16 mUnk_a6;
    /* a8 */ s16 mUnk_a8;
    /* aa */ s16 mUnk_aa;
    /* ac */ u16 mUnk_ac;
    /* ae */ s16 mUnk_ae;
    /* b0 */ u8 mUnk_b0;
    /* b1 */ PAD(0xb1, 0xb2);
    /* b2 */ u8 mMode;
    /* b3 */ u8 mSelection;
    /* b4 */ PAD(0xb4, 0xb5);
    /* b5 */ u8 mUnk_b5;
    /* b6 */ u8 mUnk_b6;
    /* b7 */ u8 mUnk_b7;
    /* b8 */ u8 mUnk_b8;
    /* b9 */ u8 mUnk_b9;
    /* ba */ u8 mLastIndex;
    /* bb */ u8 mNewSelection;
    /* bc */ u8 mDirection;
    /* bd */ u8 mScrollLeft;
    /* be */ u8 mUnk_be;
    /* bf */ u8 mState;
    /* c0 */ s8 mUnk_c0;
    /* c1 */ u8 mUnk_c1;
    /* c2 */ u8 mUnk_c2[9];
    /* cb */ u8 mUnk_cb;
    /* cc */ u8 mUnk_cc;
    /* cd */ u8 mUnk_cd;
    /* ce */ u8 mUnk_ce;
    /* cf */ u8 mStep;
    /* d0 */ u8 mUnk_d0;
    /* d1 */ u8 mTextRows;
    /* d2 */ u8 mUnk_d2;
    /* d3 */ u8 mUnk_d3;
    /* d4 */ u8 mUnk_d4;
    /* d5 */ u8 mBlend;
    /* d6 */ s8 mUnk_d6;
    /* d7 */ u8 mUnk_d7;
    /* d8 */

    TournamentScene();

    /* 00 */ virtual ~TournamentScene() override;
    /* 08 */ virtual void Update() override; // 022197e0, not decompiled yet
    /* 0c */

    void Cleanup(); // 022195e0
    void func_ov002_0221b424();
    void func_ov002_0221b5b0();
    void func_ov002_0221b78c(BOOL unk1, BOOL unk2);
    void func_ov002_0221bf88();
    void func_ov002_0221c154();
    void func_ov002_0221c1b0();
    void func_ov002_0221c4a0();
    void func_ov002_0221c7a0(const char *mes);
    void func_ov002_0221cabc(const char *mes, u8 rows);
    void func_ov002_0221cd10();
    u8 func_ov002_0221d4b0(u8 input);
    u8 func_ov002_0221d69c();
    BOOL func_ov002_0221d7e4();
    BOOL func_ov002_0221d848();
    void func_ov002_0221da5c();
    void func_ov002_0221dbd4(u8 index);
    void func_ov002_0221ddb8();
    void func_ov002_0221ddf8(u8 index);
    void func_ov002_0221df48(u32 index);
    void func_ov002_0221e270();
    BOOL func_ov002_0221e2b0();
    void func_ov002_0221e8f0();
    void func_ov002_0221e984();
    void func_ov002_0221ead8();
    void func_ov002_0221eb78();
    void func_ov002_0221ebe4(const char *path, u32 index);
    void func_ov002_0221ef34(u32 index);
    void func_ov002_0221f098(u32 index);
    BOOL func_ov002_0221f250();
};
