#pragma once

#include "global.h"
#include "types.h"

#include <nitro/fx.h>

// Palette-like 16 byte object (ctor 02029ca8, dtor 02029d18).
class UnkStruct_02029ca8 {
public:
    /* 00 */ PAD(0x00, 0x10);
    /* 10 */

    UnkStruct_02029ca8(const void *data);
    ~UnkStruct_02029ca8();
};

struct UnkStruct_0205401c_04 {
    /* 00 */ PAD(0x00, 0x12);
    /* 12 */ u16 mUnk_12;
    /* 14 */ PAD(0x14, 0x64);
    /* 64 */ u8 mUnk_64;
    /* 65 */
};

// A model of a scene (ctor 0205401c, destroyed with 02054034).
class UnkStruct_0205401c {
public:
    /* 00 */ PAD(0x00, 0x04);
    /* 04 */ UnkStruct_0205401c_04 *mUnk_04;
    /* 08 */ PAD(0x08, 0x10);
    /* 10 */

    UnkStruct_0205401c();
    void func_02054034();
    void func_0205411c(u32 unk1, u32 unk2, u32 unk3, u32 unk4);
    void func_0205461c(u32 unk1, u32 unk2);
    void func_02054568();
};

// A drawable instance of an NSBMD model; deleted through its virtual destructor.
class ModelInstance {
public:
    /* 00 (vtable) */
    /* 04 */ PAD(0x04, 0x68);
    /* 68 */ u8 mUnk_68;
    /* 69 */

    virtual ~ModelInstance();
    void func_02028d80();
    void func_02029174(MtxFx43 *mtx, VecFx32 *pos);
};

// Base of the texture file object (ctor 02028308), 0x1c bytes.
class UnkStruct_02028308 {
public:
    /* 00 */ PAD(0x00, 0x1c);
    /* 1c */

    UnkStruct_02028308();
    void func_02028344();
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
    /* 00 */ PAD(0x00, 0x10);
    /* 10 */

    UnkStruct_020289c8();
    ModelInstance *func_02028b78(u32 unk1);
    void func_020289e0(const char *name, UnkStruct_02028308 *texture);
};

// An NSBMD model file (0x10 bytes).
class NitroModel : public UnkStruct_020289c8 {
public:
    NitroModel(const char *name, UnkStruct_02028308 *texture) {
        func_020289e0(name, texture);
    }
};
