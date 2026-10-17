#pragma once

#include "global.h"
#include "types.h"

#include "Graphics/Model.hpp"
#include "Graphics/ModelInternal.hpp"
#include "Graphics/ModelTransform.hpp"

// A texture/model file object built from a file already in memory.
class NitroTextureData : public TextureFile {
public:
    NitroTextureData(void *file) {
        LoadData(file);
    }
};

class NitroModelData : public NitroModel {
public:
    NitroModelData(void *file, TextureFile *texture) {
        LoadData(file, texture);
    }
};

// Editable character built from separately loaded parts (head, up, down, hair, glasses, cap), 0x7c bytes.
class CharacterModel : public ModelTransform {
public:
    /* 2c */ void *mHeadTextureData;
    /* 30 */ void *mHeadModelData;
    /* 34 */ void *mMotionData;
    /* 38 */ u8 mPartFileNumbers[6];
    /* 3e */ PAD(0x3e, 0x40);
    /* 40 */ s8 mUnk_40;
    /* 41 */ s8 mUnk_41;
    /* 42 */ PAD(0x42, 0x44);
    /* 44 */ s32 mUnk_44;
    /* 48 */ ModelInstance *mParts[6];
    /* 60 */ void *mUnk_60;
    /* 64 */ u8 mVisible; // also reached as UnkStruct_0205401c_04::mVisible (MyroomStatusScene)
    /* 65 */ u8 mSkinPalette; // 0x65-0x74 are also written as UnkStruct_0205401c_04::mUnk_65[16] (CharacterEditScene)
    /* 66 */ u8 mFaceKind;
    /* 67 */ u8 mFaceVariant;
    /* 68 */ u8 mHairKind;
    /* 69 */ u8 mHairVariant;
    /* 6a */ u8 mCapHandle;
    /* 6b */ u8 mCapPalette;
    /* 6c */ u8 mGlasseHandle;
    /* 6d */ u8 mGlassePalette;
    /* 6e */ u8 mUnk_6e;
    /* 6f */ u8 mTopKind;
    /* 70 */ u8 mTopPalette;
    /* 71 */ u8 mDownKind;
    /* 72 */ u8 mDownPalette;
    /* 73 */ u8 mShoesKind;
    /* 74 */ u8 mShoesVariant;
    /* 75 */ u8 mAttribute;
    /* 76 */ u8 mExtraAnimSet;
    /* 77 */ u8 mExtraAnimEnabled;
    /* 78 */ u8 mMotionKind;
    /* 79 */ u8 mHeadLoadStep;
    /* 7a */ u8 mUnk_7a; // only ever 0 (StartLoad), so UpdateLoad's test always passes in this build
    /* 7b */ PAD(0x7b, 0x7c);
    /* 7c */

    CharacterModel();
    ~CharacterModel();
    void StartHeadAnim(u32 a, u32 b);
    void StartLoad(u32 motionKind);
    BOOL UpdateLoad();
    void CreateHeadAndLoadParts();
    void LoadPart(u32 part, u32 unk);
    BOOL LoadHeadFiles(u32 async);
    void CreateHead(); // also loads the head's texture and palette files (LoadPartTextures(0)) and allocates mUnk_60
    void FreeParts(); // deletes the six parts and frees mUnk_60
    void DrawParts();
    void LoadPartTextures(u32 part);
    BOOL LoadPartTexture(u32 part, const char *name, const char *file);
    void LoadPartPalette(u32 part, const char *name, const char *file);
    void LoadMergedPalette(u32 part, const char *name, const char *file1, const char *file2, u8 offset);
    void SetExtraAnimSetAndStartAnim(u32 a, u32 b);
    void SetExtraAnimEnabled(u32 flag);
    s32 GetHeadFrame();
    void SetHeadFrame(u32 frame);
};

extern const char *gCharacterPartNames[6]; // part names: head, up, down, hair, glasse, cap
