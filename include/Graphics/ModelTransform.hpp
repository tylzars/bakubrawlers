#pragma once

#include "global.h"
#include "types.h"

#include "Graphics/Model.hpp"

// Position / rotation / scale of one ModelInstance, drawn with DrawInstance (0x2c bytes).
// ctor 02052548, dtor 02052584 (frees the instance).
class ModelTransform {
public:
    /* 00 */ u8 mAnimEnabled;
    /* 01 */ PAD(0x01, 0x04);
    /* 04 */ VecFx32 mPosition;
    /* 10 */ u16 mRotX;
    /* 12 */ u16 mRotY;
    /* 14 */ u16 mRotZ;
    /* 16 */ PAD(0x16, 0x18);
    /* 18 */ VecFx32 mScale;
    /* 24 */ PAD(0x24, 0x25);
    /* 25 */ u8 mAnimRunning;
    /* 26 */ PAD(0x26, 0x28);
    /* 28 */ ModelInstance *mModel;
    /* 2c */

    ModelTransform();
    ~ModelTransform();
    BOOL DrawInstance(); // builds the matrix and draws mModel
    // loads texture, model and optional animation data files and creates the instance (mModel); unk0 -> mUnk_00
    void LoadFiles(const char *nsbtx, const char *nsbmd, const char *data, u32 unk0);
    void LoadFilesNoAnim(const char *nsbtx, const char *nsbmd); // same without animation data
    void StartAnim(u32 a, u32 b);                         // starts the animation (frame step 1.0)
    void AdvanceAnim();                                     // updates mUnk_25 from the animation state
};
