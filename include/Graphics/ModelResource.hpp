#pragma once

#include "global.h"
#include "types.h"

#include "Graphics/Model.hpp"

class ModelAnimData; // 0x1c bytes, ctor 02029324, dtor 02029424

// Reference counted set of model files: .nsbmd, .nsbtx and an optional animation data .bin (0x1c bytes).
class ModelResource {
public:
    /* 00 */ char *mModelFile;
    /* 04 */ char *mTextureFile;
    /* 08 */ char *mDataFile;
    /* 0c */ NitroTexture *mTexture;
    /* 10 */ NitroModel *mModel;
    /* 14 */ ModelAnimData *mAnimData;
    /* 18 */ u16 mRefs;
    /* 1a */ u8 mUnk_1a;
    /* 1c */

    ModelResource(const char *nsbmd, const char *data, const char *nsbtx, u32 unk4); // 020305ac
    ~ModelResource();                                                               // 02030684
    u16 func_020306e4(u8 freeTexture); // load the files (first reference) and count a reference; returns the count before
    BOOL func_020307e8();               // drop a reference, TRUE when the files were released
};
