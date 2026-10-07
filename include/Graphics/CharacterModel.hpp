#pragma once

#include "global.h"
#include "types.h"

#include "Graphics/Model.hpp"
#include "Graphics/ModelInternal.hpp"
#include "Graphics/ModelTransform.hpp"

// A texture/model file object built from a file already in memory.
class NitroTextureData : public UnkStruct_02028308 {
public:
    NitroTextureData(void *file) {
        func_020282c4(file);
    }
};

class NitroModelData : public NitroModel {
public:
    NitroModelData(void *file, UnkStruct_02028308 *texture) {
        func_02028a2c(file, texture);
    }
};

// Editable character built from separately loaded parts (head, up, down, hair, glasses, cap), 0x7c bytes.
class CharacterModel : public ModelTransform {
public:
    /* 2c */ void *mUnk_2c;
    /* 30 */ void *mUnk_30;
    /* 34 */ void *mUnk_34;
    /* 38 */ u8 mUnk_38[6];
    /* 3e */ PAD(0x3e, 0x40);
    /* 40 */ s8 mUnk_40;
    /* 41 */ s8 mUnk_41;
    /* 42 */ PAD(0x42, 0x44);
    /* 44 */ s32 mUnk_44;
    /* 48 */ ModelInstance *mParts[6];
    /* 60 */ void *mUnk_60;
    /* 64 */ u8 mUnk_64;
    /* 65 */ u8 mUnk_65;
    /* 66 */ u8 mUnk_66;
    /* 67 */ u8 mUnk_67;
    /* 68 */ u8 mUnk_68;
    /* 69 */ u8 mUnk_69;
    /* 6a */ u8 mUnk_6a;
    /* 6b */ u8 mUnk_6b;
    /* 6c */ u8 mUnk_6c;
    /* 6d */ u8 mUnk_6d;
    /* 6e */ u8 mUnk_6e;
    /* 6f */ u8 mUnk_6f;
    /* 70 */ u8 mUnk_70;
    /* 71 */ u8 mUnk_71;
    /* 72 */ u8 mUnk_72;
    /* 73 */ u8 mUnk_73;
    /* 74 */ u8 mUnk_74;
    /* 75 */ u8 mUnk_75;
    /* 76 */ u8 mUnk_76;
    /* 77 */ u8 mUnk_77;
    /* 78 */ u8 mUnk_78;
    /* 79 */ u8 mUnk_79;
    /* 7a */ u8 mUnk_7a;
    /* 7b */ PAD(0x7b, 0x7c);
    /* 7c */

    CharacterModel();
    ~CharacterModel();
    void func_02052894(u32 a, u32 b);
    void func_02052da4(u32 sex);
    BOOL func_02052db8();
    void func_02052df0();
    void func_020528b0(u32 part, u32 unk);
    BOOL func_02052e50(u32 async);
    void func_020530e8();
    void func_020531fc();
    void func_02053298();
    void func_0205340c(u32 part);
    BOOL func_02053d74(u32 part, const char *name, const char *file);
    BOOL func_02053d9c(u32 part, const char *name, const char *file);
    void func_02053dc4(u32 part, const char *name, const char *file1, const char *file2, u8 offset);
    void func_02053ea0(u32 a, u32 b);
    void func_02053eec(u32 flag);
    s32 func_02053f84();
    void func_02053fa8(u32 frame);
};

extern const char *data_020b24e4[6]; // part names: head, up, down, hair, glasse, cap
