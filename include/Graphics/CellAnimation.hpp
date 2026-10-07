#pragma once

#include "global.h"
#include "types.h"

struct SpriteScreen;

// One frame of a cell animation (OAM list + timing + optional transform).
struct CellAnimCel {
    /* 00 */ void *mOam;
    /* 04 */ u16 mOamCount;
    /* 06 */ u16 mDuration;
    /* 08 */ s32 mRotation;
    /* 0c */ s32 mScaleX;
    /* 10 */ s32 mScaleY;
    /* 14 */ s32 mOffsetX;
    /* 18 */ s32 mOffsetY;
    /* 1c */
};

// One animation of a cell animation file.
struct CellAnimFrame {
    /* 00 */ CellAnimCel *mCels;
    /* 04 */ u32 mMode;
    /* 08 */ u32 mStartFrame;
    /* 0c */ float mSpeed;
    /* 10 */ u32 mUnk_10; // frame count
    /* 14 */
};

struct CellAnimData {
    /* 00 */ PAD(0x00, 0x04);
    /* 04 */ CellAnimFrame *mFrames;
    /* 08 */
};

// Loaded NCE file.
struct CellAnimFile {
    /* 00 */ CellAnimData *mData;
    /* 04 */

    CellAnimFile(const char *path, u32 unk2, u16 unk3); // 0x0202a710
    ~CellAnimFile();                                    // 0x0202a88c
};

// Rotation/scale applied to a drawn frame.
struct SpriteTransform {
    /* 00 */ s32 mScaleX;
    /* 04 */ s32 mScaleY;
    /* 08 */ u16 mRotation;
    /* 0a */ PAD(0x0a, 0x0c);
    /* 0c */

    SpriteTransform(s32 scaleX, s32 scaleY, u16 rotation); // 0x02027f10
};

// A cell animation loaded from an NCE file.
class CellAnimation {
public:
    /* 00 */ CellAnimFrame *mFrames;
    /* 04 */ s32 mStep; // timer decrement per update, 4096 per tick
    /* 08 */ s32 mTimer;
    /* 0c */ u16 mCharBase;
    /* 0e */ u8 mUnk_0e; // animation index
    /* 0f */ u8 mUnk_0f; // frame index
    /* 10 */ u8 mUnk_10; // playback mode
    /* 11 */ s8 mUnk_11;
    /* 12 */ u8 mUnk_12; // finished
    /* 13 */ PAD(0x13, 0x14);
    /* 14 */ CellAnimFile *mFile;
    /* 18 */

    CellAnimation(const char *path, u32 unk2, u32 unk3, u32 unk4);
    CellAnimation(CellAnimFile *file, u32 unk2); // 0x02029d7c, does not own the file
    ~CellAnimation();
    void Update();
    void Draw(u32 unk1, s32 x, s32 y, u32 unk4);
    void func_02029f6c();
    void SetAnimation(u8 index) {
        mUnk_0e = index;
        func_02029f6c();
    }
    void func_02029fb8(u32 unk1);
    void func_02029fc0();
    void func_0202a3ec(u32 unk1, u8 frame, s16 x, s16 y, u32 unk5);
    void func_02029df8(CellAnimFrame *entries, u32 index);
    void func_02029ffc(SpriteScreen *screen, s32 x, s32 y, u8 unk4);
    void func_0202a120(SpriteScreen *screen, u32 frame, s32 x, s16 y, u8 unk5);
    void func_0202a244(SpriteScreen *screen, s32 x, s32 y, u8 unk4, s32 scaleX, s32 scaleY, u16 rotation);
    u32 func_0202a54c(); // total duration of the current animation
    void func_0202a494(u32 screen, s32 x, s32 y, u32 mode, s32 scaleX, s32 scaleY, u32 unk7); // scaled draw
};
