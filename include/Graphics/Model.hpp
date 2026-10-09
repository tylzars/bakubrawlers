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
    void func_02029c0c(); // sends the light to the 3D engine state
    void func_02029c44(const s16 *direction);
};

// Four lights read from a 10 byte per light table (ctor 02029ca8, dtor 02029d18).
class UnkStruct_02029ca8 {
public:
    /* 00 */ ModelLight *mLights[4];
    /* 10 */

    UnkStruct_02029ca8(const void *data);
    ~UnkStruct_02029ca8();
    void func_02029d54(); // apply all lights
};

class ModelInstance;
struct CharacterParams;

// The CharacterModel (Graphics/CharacterModel.hpp) seen through UnkStruct_0205401c::mUnk_04; still to be merged
// into that class (CharacterEditScene reads 0x65-0x74 as an array).
struct UnkStruct_0205401c_04 {
    /* 00 */ PAD(0x00, 0x12);
    /* 12 */ u16 mUnk_12;
    /* 14 */ PAD(0x14, 0x64);
    /* 64 */ u8 mUnk_64;
    /* 65 */ u8 mUnk_65[16];
    /* 75 */ u8 mUnk_75;
    /* 76 */
};

// A model of a scene (ctor 0205401c, destroyed with 02054034).
class UnkStruct_0205401c {
public:
    /* 00 */ ModelInstance *mUnk_00; // shadow
    /* 04 */ UnkStruct_0205401c_04 *mUnk_04; // CharacterModel (editable character, kind 0x15)
    /* 08 */ void *mUnk_08;                  // StoryCharacterModel (every other kind)
    /* 0c */ volatile u8 mUnk_0c;                   // 1: mUnk_04 is used
    /* 0d */ u8 mUnk_0d;                     // set up
    /* 0e */ PAD(0x0e, 0x10);
    /* 10 */

    UnkStruct_0205401c();
    UnkStruct_0205401c *func_02054034();
    void func_0205411c(u32 unk1, u32 unk2, u32 unk3, u32 unk4);
    void func_02054290(const CharacterParams *params);
    void func_0205461c(u32 unk1, u32 unk2);
    void func_02054568();
    void func_020543e8(u32 unk1, u32 unk2, u32 unk3, u8 unk4);
    BOOL func_020544e4();
    void func_0205451c();
    void func_02054644(u32 unk1, u32 unk2);
    void func_0205466c(u32 unk1);
    void func_02054694(u8 unk1);
    u8 func_020546b0();
    u16 *func_020546cc();
    void func_020546e4(const u16 *rot);
    s32 func_02054710();
    void func_02054740();
};

class NitroModel;
class ModelInstance;
class ModelAnimData;
class UnkStruct_02028308;
class UnkStruct_020289c8;

struct NNSG3dRS;
struct NNSG3dAnmObj;

// One animation file (8 bytes, ctor 02029248): the file data and whether it was allocated by us.
class UnkStruct_02029248 {
public:
    /* 00 */ void *mFile;
    /* 04 */ u8 mOwnsFile;
    /* 05 */ PAD(0x05, 0x08);
    /* 08 */

    void func_02029248();
    BOOL func_02029258(void *file);
    void *func_02029288(u32 index, UnkStruct_020289c8 *model, UnkStruct_02028308 *texture); // creates an animation object
    void func_020292e4();
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
    /* 04 */ UnkStruct_02029248 *mAnims[5];
    /* 18 */ u8 mOwnsFile;
    /* 19 */ PAD(0x19, 0x1c);
    /* 1c */

    ModelAnimData(const char *path);
    ModelAnimData(void *data);
    ~ModelAnimData();
    void func_02029488(); // creates the UnkStruct_02029248 of every file entry
};

// Animation controller of a ModelInstance (0x60 bytes, ctor 020294e8).
class ModelAnim {
public:
    /* 00 (vtable) */
    /* 04 */ u32 mUnk_04;
    /* 08 */ u32 mUnk_08; // loop
    /* 0c */ s32 mUnk_0c;
    /* 10 */ u32 mUnk_10;
    /* 14 */ NitroModel *mUnk_14;
    /* 18 */ ModelAnimData *mUnk_18;
    /* 1c */ UnkStruct_02029248 *mUnk_1c[5];
    /* 30 */ NNSG3dAnmObj **mUnk_30[5];
    /* 44 */ u8 *mUnk_44[5];
    /* 58 */ u8 mUnk_58[5];
    /* 5d */ u8 mUnk_5d;
    /* 5e */ PAD(0x5e, 0x60);
    /* 60 */

    ModelAnim(const void *data, NitroModel *model);
    virtual ~ModelAnim();
    void func_020298c0(u32 value, u32 index); // index 5 = all channels
    void func_02029594();
    BOOL func_020295e0();
    void func_0202964c(u32 channel, UnkStruct_02029248 *anim, u8 *table);
    void func_02029734();
    s32 func_020297d4(u32 set);
    void func_0202982c(NNSG3dAnmObj *obj);
    BOOL func_02029854(NNSG3dAnmObj *obj);
    void func_02029914();
    void func_020299b8();
    void func_020299ec();
    NNSG3dAnmObj *func_02029a48(u32 channel);
    NNSG3dAnmObj *func_02029a58(u32 channel, u32 set);
    void func_02029ad8(u32 set);
};

// Render data of a ModelInstance (the model file object: only the pointer at +04 is used outside this module).
typedef UnkStruct_020289c8 ModelInstanceData;

// A drawable instance of an NSBMD model; deleted through its virtual destructor.
class ModelInstance {
public:
    /* 00 (vtable) */
    /* 04 */ u8 mRenderObj[0x2c]; // NNSG3dRenderObj
    /* 30 */ ModelInstance *mUnk_30;
    /* 34 */ PAD(0x34, 0x38);
    /* 38 */ void *mUnk_38;
    /* 3c */ u32 mUnk_3c;
    /* 40 */ PAD(0x40, 0x58);
    /* 58 */ UnkStruct_020289c8 *mUnk_58;
    /* 5c */ ModelAnim *mAnim;
    /* 60 */ NNSG3dAnmObj *mUnk_60;
    /* 64 */ MtxFx43 *mUnk_64;
    /* 68 */ u8 mUnk_68;
    /* 69 */ u8 mUnk_69;
    /* 6a */ u8 mUnk_6a;
    /* 6b */ u8 mUnk_6b;
    /* 6c */

    ModelInstance(UnkStruct_020289c8 *model);
    virtual ~ModelInstance();
    BOOL func_02028d80();
    BOOL func_02028da4();
    void func_02028de0(ModelAnim *anim);
    void func_02028de8(u32 a, u32 b);
    void func_02028e54(u32 a, u32 b, fx32 c);
    void func_02028e70(s32 frame); // sets the animation frame of all five channels
    s32 func_02028ea4();
    void func_02028ef0();
    s32 func_02028f48();
    void func_02028f54(NNSG3dAnmObj *obj);
    void func_02028f88();
    static void func_02028fb0(NNSG3dRS *rs);
    void func_02028fec();
    void func_02029054();
    MtxFx43 *func_02029084(s32 index);
    BOOL func_020290c0();
    BOOL func_02029174(MtxFx43 *mtx, VecFx32 *pos);
};

struct NNSG3dResTex;

// Base of the texture file object (ctor 02028308), 0x1c bytes.
class UnkStruct_02028308 {
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

    UnkStruct_02028308();
    void func_02028344();
    BOOL func_020282c4(void *file);
    void func_0202832c(); // release VRAM and file
    void func_02028388(); // free VRAM
    void func_020283f8(); // allocate VRAM and upload
    void func_020284e8();
    void func_0202866c();
    BOOL func_020286fc(const char *name, const char *file, u32 size); // upload part of the textures from file.ntft
    BOOL func_02028778(const char *name, void *data, u32 size);
    BOOL func_02028858(const char *name, const char *file, u32 size); // upload a palette from file.ntfp
    BOOL func_020288d4(const char *name, void *data, u32 size);
};

// An NSBTX texture file (0x1c bytes).
class NitroTexture : public UnkStruct_02028308 {
public:
    NitroTexture(const char *name) {
        func_02028280(name);
    }
    void func_02028280(const char *name);
};

// Base of the model file object (ctor 020289c8), 0x10 bytes.
class UnkStruct_020289c8 {
public:
    /* 00 */ void *mFile;     // NSBMD file data
    /* 04 */ void *mUnk_04;   // NNSG3dResMdl in the file
    /* 08 */ UnkStruct_02028308 *mUnk_08; // texture bound to the model
    /* 0c */ u8 mOwnsFile;
    /* 0d */ PAD(0x0d, 0x10);
    /* 10 */

    UnkStruct_020289c8();
    ModelInstance *func_02028b78(ModelAnim *anim);
    void func_02028b4c(UnkStruct_02028308 *texture);
    BOOL func_020289e0(const char *name, UnkStruct_02028308 *texture);
    BOOL func_02028a2c(void *file, UnkStruct_02028308 *texture);
    BOOL func_02028ac4(const char *name);
    void func_02028af0();
};

// An NSBMD model file (0x10 bytes).
class NitroModel : public UnkStruct_020289c8 {
public:
    NitroModel() {}
    NitroModel(const char *name, UnkStruct_02028308 *texture) {
        func_020289e0(name, texture);
    }
};
