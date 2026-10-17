#pragma once

#include "global.h"
#include "types.h"

#include <nitro/fx.h>

// One of the four lights of a ModelLights set (0x14 bytes, ctor 02029b5c; vtable 020a569c).
class ModelLight {
public:
    /* 00 (vtable) */
    /* 04 */ u32 mEnabled;
    /* 08 */ u32 mIndex;
    /* 0c */ u16 mColor;
    /* 0e */ s16 mDirection[3];
    /* 14 */

    ModelLight(u32 index, u32 enabled, u16 color, const s16 *direction);
    virtual ~ModelLight();
    void Apply(); // sends the light to the 3D engine state
    void SetDirection(const s16 *direction);
};

// Four lights read from a 10 byte per light table (ctor 02029ca8, dtor 02029d18).
class ModelLightGroup {
public:
    /* 00 */ ModelLight *mLights[4];
    /* 10 */

    ModelLightGroup(const void *data);
    ~ModelLightGroup();
    void ApplyAll(); // apply all lights
};

class ModelInstance;
struct CharacterParams;

// The CharacterModel (Graphics/CharacterModel.hpp) seen through UnkStruct_0205401c::mUnk_04; still to be merged
// into that class (CharacterEditScene reads 0x65-0x74 as an array).
struct UnkStruct_0205401c_04 {
    /* 00 */ PAD(0x00, 0x12);
    /* 12 */ u16 mRotY;
    /* 14 */ PAD(0x14, 0x64);
    /* 64 */ u8 mVisible;
    /* 65 */ u8 mUnk_65[16];
    /* 75 */ u8 mUnk_75;
    /* 76 */
};

// A model of a scene (ctor 0205401c, destroyed with 02054034).
class UnkStruct_0205401c {
public:
    /* 00 */ ModelInstance *mShadow; // shadow
    /* 04 */ UnkStruct_0205401c_04 *mCharacter; // CharacterModel (editable character, kind 0x15)
    /* 08 */ void *mStoryCharacter;                  // StoryCharacterModel (every other kind)
    /* 0c */ volatile u8 mUseCharacter;                   // 1: mCharacter is used
    /* 0d */ u8 mBuilt;                     // set up
    /* 0e */ PAD(0x0e, 0x10);
    /* 10 */

    UnkStruct_0205401c();
    UnkStruct_0205401c *Release();
    void Create(u32 unk1, u32 unk2, u32 unk3, u32 unk4);
    void SetParams(const CharacterParams *params);
    void func_0205461c(u32 unk1, u32 unk2);
    void Draw();
    void StartLoad(u32 unk1, u32 unk2, u32 unk3, u8 unk4);
    BOOL UpdateLoad();
    void FinishLoad();
    void func_02054644(u32 unk1, u32 unk2);
    void SetExtraAnimEnabled(u32 unk1);
    void SetVisible(u8 unk1);
    u8 IsVisible();
    u16 *GetModelRotation();
    void SetModelRotation(const u16 *rot);
    s32 IsAnimEndReached();
    void LoadShadow();
};

class NitroModel;
class ModelInstance;
class ModelAnimData;
class TextureFile;
class ModelFile;

struct NNSG3dRS;
struct NNSG3dAnmObj;

// One animation file (8 bytes, ctor 02029248): the file data and whether it was allocated by us.
class ModelAnimFile {
public:
    /* 00 */ void *mFile;
    /* 04 */ u8 mOwnsFile;
    /* 05 */ PAD(0x05, 0x08);
    /* 08 */

    void Init();
    BOOL SetFile(void *file);
    void *CreateAnimObj(u32 index, ModelFile *model, TextureFile *texture); // creates an animation object
    void Release();
};

// The set of animation files of a model (one per channel; 0x1c bytes). Ctor 02029324 loads the files by name,
// ctor 020293b8 takes a file in memory.
struct ModelAnimEntry {
    /* 00 */ u32 mFile; // offset in the data, relocated to a pointer when loaded
    /* 04 */ u32 mTable; // likewise
};

class ModelAnimData {
public:
    /* 00 */ ModelAnimEntry *mFile;
    /* 04 */ ModelAnimFile *mAnims[5];
    /* 18 */ u8 mOwnsFile;
    /* 19 */ PAD(0x19, 0x1c);
    /* 1c */

    ModelAnimData(const char *path);
    ModelAnimData(void *data);
    ~ModelAnimData();
    void CreateAnimFiles(); // creates the ModelAnimFile of every file entry
};

// Animation controller of a ModelInstance (0x60 bytes, ctor 020294e8).
class ModelAnim {
public:
    /* 00 (vtable) */
    /* 04 */ u32 mPaused;
    /* 08 */ u32 mLoop; // loop
    /* 0c */ s32 mFramePerFrame;
    /* 10 */ u32 mReachedEnd;
    /* 14 */ NitroModel *mNitroModel;
    /* 18 */ ModelAnimData *mAnimData;
    /* 1c */ ModelAnimFile *mAnimFiles[5];
    /* 30 */ NNSG3dAnmObj **mAnimObjs[5];
    /* 44 */ u8 *mAnimObjHandles[5];
    /* 58 */ u8 mAnimObjCounts[5];
    /* 5d */ u8 mSet;
    /* 5e */ PAD(0x5e, 0x60);
    /* 60 */

    ModelAnim(const void *data, NitroModel *model);
    virtual ~ModelAnim();
    void SetFrame(u32 value, u32 index); // index 5 = all channels
    void Init();
    BOOL Advance();
    void InitChannel(u32 channel, ModelAnimFile *anim, u8 *table);
    void ResetFrames();
    s32 GetFrameCountForSet(u32 set);
    void StepAnimObjAndRecordEnd(NNSG3dAnmObj *obj);
    BOOL StepAnimObj(NNSG3dAnmObj *obj);
    void FreeAnimObjs();
    void FreeAnimData();
    void func_020299ec();
    NNSG3dAnmObj *GetAnimObj(u32 channel);
    NNSG3dAnmObj *GetAnimObjForSet(u32 channel, u32 set);
    void SelectSet(u32 set);
};

// Render data of a ModelInstance (the model file object: only the pointer at +04 is used outside this module).
typedef ModelFile ModelInstanceData;

// A drawable instance of an NSBMD model; deleted through its virtual destructor.
class ModelInstance {
public:
    /* 00 (vtable) */
    /* 04 */ u8 mRenderObj[0x2c]; // NNSG3dRenderObj
    /* 30 */ ModelInstance *mUserData;
    /* 34 */ PAD(0x34, 0x38);
    /* 38 */ void *mUnk_38;
    /* 3c */ u32 mUnk_3c;
    /* 40 */ PAD(0x40, 0x58);
    /* 58 */ ModelFile *mModelFile;
    /* 5c */ ModelAnim *mAnim;
    /* 60 */ NNSG3dAnmObj *mExtraAnim;
    /* 64 */ MtxFx43 *mCallbackMtx;
    /* 68 */ u8 mOwnFlags;
    /* 69 */ u8 mUseBoxTest;
    /* 6a */ u8 mAnimStarted;
    /* 6b */ u8 mCallbackMtxStored;
    /* 6c */

    ModelInstance(ModelFile *model);
    virtual ~ModelInstance();
    BOOL AdvanceAnim();
    BOOL AdvanceExtraAnim();
    void SetAnim(ModelAnim *anim);
    void StartAnim(u32 a, u32 b);
    void StartAnimWithIncrement(u32 a, u32 b, fx32 c);
    void SetAnimFrame(s32 frame); // sets the animation frame of all five channels
    s32 GetAnimFrame();
    void DetachAllAnims();
    s32 IsAnimFinished();
    void AttachExtraAnim(NNSG3dAnmObj *obj);
    void DetachExtraAnim();
    static void StoreCallbackMtx(NNSG3dRS *rs);
    void EnableCallbackMtx();
    void DisableCallbackMtx();
    MtxFx43 *GetCallbackMtx(s32 index);
    BOOL BoxTest();
    BOOL Draw(MtxFx43 *mtx, VecFx32 *pos);
};

struct NNSG3dResTex;

// Base of the texture file object (ctor 02028308), 0x1c bytes.
class TextureFile {
public:
    /* 00 */ void *mFile;
    /* 04 */ NNSG3dResTex *mTex;
    /* 08 */ u32 mLoaded;
    /* 0c */ u32 mTexHandle;
    /* 10 */ u32 m4x4Handle;
    /* 14 */ u32 mPlttHandle;
    /* 18 */ u8 mOwnsFile;
    /* 19 */ PAD(0x19, 0x1c);
    /* 1c */

    TextureFile();
    void FreeFile();
    BOOL LoadData(void *file);
    void Release(); // release VRAM and file
    void FreeVram(); // free VRAM
    void AllocVramAndUpload(); // allocate VRAM and upload
    void UploadTextures();
    void UploadPalettes();
    BOOL LoadTextureFile(const char *name, const char *file, u32 size); // upload part of the textures from file.ntft
    BOOL UploadNamedTexture(const char *name, void *data, u32 size);
    BOOL LoadPaletteFile(const char *name, const char *file, u32 size); // upload a palette from file.ntfp
    BOOL UploadNamedPalette(const char *name, void *data, u32 size);
};

// An NSBTX texture file (0x1c bytes).
class NitroTexture : public TextureFile {
public:
    NitroTexture(const char *name) {
        Load(name);
    }
    void Load(const char *name);
};

// Base of the model file object (ctor 020289c8), 0x10 bytes.
class ModelFile {
public:
    /* 00 */ void *mFile;     // NSBMD file data
    /* 04 */ void *mResModel;   // NNSG3dResMdl in the file
    /* 08 */ TextureFile *mTexture; // texture bound to the model
    /* 0c */ u8 mOwnsFile;
    /* 0d */ PAD(0x0d, 0x10);
    /* 10 */

    ModelFile();
    ModelInstance *CreateInstance(ModelAnim *anim);
    void BindTexture(TextureFile *texture);
    BOOL Load(const char *name, TextureFile *texture);
    BOOL LoadData(void *file, TextureFile *texture);
    BOOL LoadAndSetupResFile(const char *name);
    void Release();
};

// An NSBMD model file (0x10 bytes).
class NitroModel : public ModelFile {
public:
    NitroModel() {}
    NitroModel(const char *name, TextureFile *texture) {
        Load(name, texture);
    }
};
