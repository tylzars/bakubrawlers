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
    /* 10 */ u32 mFrameCount; // frame count
    /* 14 */
};

// One OAM entry of a cel (12 bytes); the low 10 bits of mChar are scaled when a file is loaded at another size.
struct CellAnimOam {
    /* 00 */ u16 mUnk_00;
    /* 02 */ u16 mUnk_02;
    /* 04 */ u16 mChar;
    /* 06 */ PAD(0x06, 0x0c);
    /* 0c */
};

// Header of an NCE file. The offsets at +04..+0c are relocated to pointers when the file is loaded.
struct CellAnimData {
    /* 00 */ u16 mUnk_00;
    /* 02 */ u16 mNumFrames;
    /* 04 */ CellAnimFrame *mFrames;
    /* 08 */ CellAnimCel *mCels;
    /* 0c */ CellAnimOam *mOams;
    /* 10 */
};

// Loaded NCE file.
struct CellAnimFile {
    /* 00 */ CellAnimData *mData;
    /* 04 */

    CellAnimFile(const char *path, u16 unk2, u16 unk3); // 0x0202a710 (0x0202a594 is the base-object constructor)
    ~CellAnimFile();                                    // 0x0202a88c
};

// A file taken from memory (already loaded), not through the cache's path lookup.
struct CellAnimFileData : public CellAnimFile {
    CellAnimFileData(void *data); // 0x0202aa24
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
    /* 0e */ u8 mAnimIndex; // animation index
    /* 0f */ u8 mFrameIndex; // frame index
    /* 10 */ u8 mMode; // Update: 0 and 4 forward once, 1 forward loop, 2 backward once, 3 backward loop, 5 stopped
    /* 11 */ s8 mPaletteBase;
    /* 12 */ u8 mFinished; // set when mode 0, 2 or 4 reaches its end; cleared by ResetAnimation
    /* 13 */ PAD(0x13, 0x14);
    /* 14 */ CellAnimFile *mFile;
    /* 18 */

    CellAnimation(const char *path, u32 unk2, u16 unk3, u32 unk4);
    CellAnimation(CellAnimFile *file, u32 unk2); // 0x02029d7c, does not own the file
    ~CellAnimation();
    void Update();
    void Draw(u8 unk1, s16 x, s16 y, u8 unk4);
    void ResetAnimation();
    void SetAnimation(u8 index) {
        mAnimIndex = index;
        ResetAnimation();
    }
    void SetMode(u32 unk1);
    void Rewind(); // back to the start frame and its duration; leaves mFinished (ResetAnimation clears it)
    void DrawFrame(u8 unk1, u8 frame, s16 x, s16 y, u8 unk5);
    void Init(CellAnimFrame *entries, u32 index);
    void DrawToScreen(SpriteScreen *screen, s32 x, s32 y, u8 unk4);
    void DrawFrameToScreen(SpriteScreen *screen, u32 frame, s32 x, s16 y, u8 unk5);
    // scaleX / scaleY are multiplied by the cel's scale and its rotation is added
    void DrawTransformedToScreen(SpriteScreen *screen, s32 x, s32 y, u8 unk4, s32 scaleX, s32 scaleY, u16 rotation);
    u32 GetDuration(); // total duration of the current animation
    void DrawTransformed(u32 screen, s32 x, s32 y, u32 mode, s32 scaleX, s32 scaleY, u32 unk7); // scaled draw
};
