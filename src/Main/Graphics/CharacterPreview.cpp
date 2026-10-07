#include "Graphics/CharacterModel.hpp"
#include "Graphics/SpriteManager.hpp"
#include "System/FileSystem.hpp"
#include <nitro/os.h>
#include <stdio.h>

extern u8 data_020d4404[];
extern MtxFx43 data_0220e86c;
void func_01ffcf64(MtxFx43 *mtx, VecFx32 *pos);
void func_02023708(const char *path, u32 a, u32 b, u32 c, u32 d, u32 e);
int func_02023874();
void *func_02023864();
void func_02023a54();

// 0x1bc bytes, built from /Game/Character/Character/charaNN files.
// Custom character appearance as stored in the save data (8 bytes, data_020d4404).
struct CharacterParams {
    u16 mUnk_00_0 : 1;
    u16 mUnk_66 : 3;
    u16 mUnk_67 : 3;
    u16 mUnk_68 : 3;
    u16 mUnk_69 : 3;
    u16 mUnk_6a : 3;
    u16 mUnk_6b : 3;
    u16 mUnk_6c : 3;
    u16 mUnk_6d : 3;
    u16 mUnk_6e : 3;
    u16 mUnk_6f : 3;
    u16 mUnk_70 : 3;
    u16 mUnk_71 : 5;
    u16 mUnk_72 : 3;
    u16 mUnk_74 : 3;
    u16 mUnk_73 : 2;
    u16 mUnk_65 : 3;
    u16 mUnk_06_3 : 5;
    u16 mUnk_75 : 8;
};

class StoryCharacterModel : public ModelTransform {
public:
    /* 2c */ void *mUnk_2c;
    /* 30 */ void *mUnk_30;
    /* 34 */ void *mUnk_34;
    /* 38 */ u8 mUnk_38;
    /* 39 */ u8 mUnk_39;
    /* 3a */ u8 mUnk_3a;
    /* 3b */ char mUnk_3b[0x7f];
    /* ba */ char mUnk_ba[0x7f];
    /* 139 */ char mUnk_139[0x7f];
    /* 1b8 */ u8 mUnk_1b8;
    /* 1b9 */ u8 mUnk_1b9;
    /* 1ba */ PAD(0x1ba, 0x1bc);
    /* 1bc */

    StoryCharacterModel();
    ~StoryCharacterModel();
    void func_020548a8(u32 id, u32 variant);
    void func_02054990(u32 id, u32 variant);
    BOOL func_02054a0c();
    void func_02054bd4();
    void func_02054cc4(u32 a, u32 b);
    void func_02054d08(u32 flag);
    void func_02054da8();
    void func_02054dd8(u32 a, u32 b);
};

static inline u8 GetSet(ModelAnim *a) {
    return a->mUnk_5d;
}

static inline void SetRotation(ModelTransform *model, const u16 *rot) {
    model->mRotX = rot[0];
    model->mRotY = rot[1];
    model->mRotZ = rot[2];
}

UnkStruct_0205401c::UnkStruct_0205401c() {
    mUnk_00 = NULL;
    mUnk_04 = NULL;
    mUnk_08 = NULL;
    mUnk_0d = 0;
}

UnkStruct_0205401c *UnkStruct_0205401c::func_02054034() {
    SpriteManager::Get().func_0202753c(0);
    StoryCharacterModel *story = (StoryCharacterModel *) mUnk_08;
    if (story != NULL) {
        delete story;
    }
    mUnk_08 = NULL;
    CharacterModel *character = (CharacterModel *) mUnk_04;
    if (character != NULL) {
        delete character;
    }
    mUnk_04 = NULL;
    ModelInstance *shadow = mUnk_00;
    if (shadow != NULL) {
        shadow->mUnk_68 = 0xf;
        delete shadow;
        mUnk_00 = NULL;
    }
    return this;
}

void UnkStruct_0205401c::func_0205411c(u32 unk1, u32 unk2, u32 unk3, u32 unk4) {
    if (unk3 == 0) {
        unk3 = (u32) data_020d4404;
    }
    if (unk2 != 0) {
        SpriteManager::Get().func_02027504(0);
    }
    mUnk_0c = unk1 == 0x15;
    if (mUnk_0c == 0) {
        mUnk_08 = new StoryCharacterModel();
        ((StoryCharacterModel *) mUnk_08)->func_020548a8(unk1, unk4);
        ((StoryCharacterModel *) mUnk_08)->mUnk_38 = 1;
    } else {
        CharacterModel *character = new CharacterModel();
        mUnk_04                   = (UnkStruct_0205401c_04 *) character;
        func_02054290((const CharacterParams *) unk3);
        ((CharacterModel *) mUnk_04)->func_020528b0(0, unk4);
        ((CharacterModel *) mUnk_04)->func_020528b0(1, unk4);
        ((CharacterModel *) mUnk_04)->func_020528b0(2, unk4);
        ((CharacterModel *) mUnk_04)->func_020528b0(3, unk4);
        ((CharacterModel *) mUnk_04)->func_020528b0(4, unk4);
        ((CharacterModel *) mUnk_04)->func_020528b0(5, unk4);
        ((CharacterModel *) mUnk_04)->mUnk_64 = 1;
    }
    mUnk_0d = 1;
}

void UnkStruct_0205401c::func_02054290(const CharacterParams *params) {
    ((CharacterModel *) mUnk_04)->mUnk_65 = params->mUnk_65;
    ((CharacterModel *) mUnk_04)->mUnk_66 = params->mUnk_66;
    ((CharacterModel *) mUnk_04)->mUnk_67 = params->mUnk_67;
    ((CharacterModel *) mUnk_04)->mUnk_68 = params->mUnk_68;
    ((CharacterModel *) mUnk_04)->mUnk_69 = params->mUnk_69;
    ((CharacterModel *) mUnk_04)->mUnk_6a = params->mUnk_6a;
    ((CharacterModel *) mUnk_04)->mUnk_6b = params->mUnk_6b;
    ((CharacterModel *) mUnk_04)->mUnk_6c = params->mUnk_6c;
    ((CharacterModel *) mUnk_04)->mUnk_6d = params->mUnk_6d;
    ((CharacterModel *) mUnk_04)->mUnk_6e = params->mUnk_6e;
    ((CharacterModel *) mUnk_04)->mUnk_6f = params->mUnk_6f;
    ((CharacterModel *) mUnk_04)->mUnk_70 = params->mUnk_70;
    ((CharacterModel *) mUnk_04)->mUnk_71 = params->mUnk_71;
    ((CharacterModel *) mUnk_04)->mUnk_72 = params->mUnk_72;
    ((CharacterModel *) mUnk_04)->mUnk_73 = params->mUnk_73;
    ((CharacterModel *) mUnk_04)->mUnk_74 = params->mUnk_74;
    ((CharacterModel *) mUnk_04)->mUnk_75 = params->mUnk_75;
}

void UnkStruct_0205401c::func_020543e8(u32 unk1, u32 unk2, u32 unk3, u8 unk4) {
    if (unk2 != 0) {
        SpriteManager::Get().func_02027504(0);
    }
    if (unk1 == 0x15) {
        if (unk3 == 0) {
            unk3 = (u32) data_020d4404;
        }
        mUnk_0c = 1;
        mUnk_04 = (UnkStruct_0205401c_04 *) new CharacterModel();
        func_02054290((const CharacterParams *) unk3);
        ((CharacterModel *) mUnk_04)->func_02052da4(unk4);
    } else {
        mUnk_0c = 0;
        mUnk_08 = new StoryCharacterModel();
        ((StoryCharacterModel *) mUnk_08)->func_02054990(unk1, unk4);
    }
}

BOOL UnkStruct_0205401c::func_020544e4() {
    BOOL result = FALSE;
    BOOL done;
    if (mUnk_0c == 1) {
        done = ((CharacterModel *) mUnk_04)->func_02052db8();
    } else {
        done = ((StoryCharacterModel *) mUnk_08)->func_02054a0c();
    }
    if (done) {
        result = TRUE;
    }
    return result;
}

void UnkStruct_0205401c::func_0205451c() {
    if (mUnk_0c == 1) {
        ((CharacterModel *) mUnk_04)->func_02052df0();
        ((CharacterModel *) mUnk_04)->mUnk_64 = 1;
    } else {
        ((StoryCharacterModel *) mUnk_08)->func_02054bd4();
        ((StoryCharacterModel *) mUnk_08)->mUnk_38 = 1;
    }
    mUnk_0d = 1;
}

void UnkStruct_0205401c::func_02054568() {
    if (mUnk_0c == 0) {
        ((StoryCharacterModel *) mUnk_08)->func_02054da8();
    } else {
        ((CharacterModel *) mUnk_04)->func_02053298();
    }
    if (mUnk_00 == NULL) {
        return;
    }
    MtxFx43 mtx = data_0220e86c;
    VecFx32 scale;
    VecFx32 pos;
    scale.x     = 0x4000;
    scale.y     = 0x4000;
    scale.z     = 0x4000;
    pos.x       = 0;
    pos.y       = -0x800;
    pos.z       = 0;
    func_01ffcf64(&mtx, &pos);
    mUnk_00->func_02028d80();
    mUnk_00->func_02029174(&mtx, &scale);
}

void UnkStruct_0205401c::func_0205461c(u32 unk1, u32 unk2) {
    if (mUnk_0c == 0) {
        ((StoryCharacterModel *) mUnk_08)->func_02054dd8(unk1, unk2);
    } else {
        ((CharacterModel *) mUnk_04)->func_02052894(unk1, unk2);
    }
}

void UnkStruct_0205401c::func_02054644(u32 unk1, u32 unk2) {
    if (mUnk_0c == 0) {
        ((StoryCharacterModel *) mUnk_08)->func_02054cc4(unk1, unk2);
    } else {
        ((CharacterModel *) mUnk_04)->func_02053ea0(unk1, unk2);
    }
}

void UnkStruct_0205401c::func_0205466c(u32 unk1) {
    if (mUnk_0c == 0) {
        ((StoryCharacterModel *) mUnk_08)->func_02054d08(unk1);
    } else {
        ((CharacterModel *) mUnk_04)->func_02053eec(unk1);
    }
}

void UnkStruct_0205401c::func_02054694(u8 unk1) {
    if (mUnk_0c == 0) {
        ((StoryCharacterModel *) mUnk_08)->mUnk_38 = unk1;
    } else {
        ((CharacterModel *) mUnk_04)->mUnk_64 = unk1;
    }
}

u8 UnkStruct_0205401c::func_020546b0() {
    if (mUnk_0c == 0) {
        return ((StoryCharacterModel *) mUnk_08)->mUnk_38;
    }
    return ((CharacterModel *) mUnk_04)->mUnk_64;
}

u16 *UnkStruct_0205401c::func_020546cc() {
    ModelTransform *model;
    if (mUnk_0c == 0) {
        model = (StoryCharacterModel *) mUnk_08;
    } else {
        model = (CharacterModel *) mUnk_04;
    }
    return &model->mRotX;
}

// non-matching: 6 words, the original keeps the model pointer in r3 and loads rot[0] between the conditional loads.
void UnkStruct_0205401c::func_020546e4(const u16 *rot) {
    ModelTransform *model;
    if (mUnk_0c == 0) {
        model = (StoryCharacterModel *) mUnk_08;
    } else {
        model = (CharacterModel *) mUnk_04;
    }
    model->mRotX = rot[0];
    model->mRotY = rot[1];
    model->mRotZ = rot[2];
}

s32 UnkStruct_0205401c::func_02054710() {
    if (mUnk_0c == 0) {
        return ((StoryCharacterModel *) mUnk_08)->mModel->func_02028f48();
    }
    return ((CharacterModel *) mUnk_04)->mParts[0]->func_02028f48();
}

void UnkStruct_0205401c::func_02054740() {
    ChangeDir("/story/mdl");
    NitroTexture *texture = new NitroTexture("cut_shadow.nsbtx");
    NitroModel *model     = new NitroModel("cut_shadow.nsbmd", texture);
    texture->func_02028344();
    mUnk_00 = model->func_02028b78(NULL);
}

StoryCharacterModel::StoryCharacterModel() {
    mUnk_2c  = NULL;
    mUnk_30  = NULL;
    mUnk_34  = NULL;
    mUnk_38  = 0;
    mUnk_39  = 0;
    mUnk_3a  = 0;
    mUnk_1b8 = 0;
}

StoryCharacterModel::~StoryCharacterModel() {
    if (mUnk_1b8 != 0) {
        func_02023a54();
        ModelInstance *model = mModel;
        if (model != NULL) {
            model->mUnk_68 = 0xf;
            delete model;
            mModel = NULL;
        }
        if (mUnk_2c != NULL) {
            OS_Free(mUnk_2c);
        }
        if (mUnk_30 != NULL) {
            OS_Free(mUnk_30);
        }
        if (mUnk_34 != NULL) {
            OS_Free(mUnk_34);
        }
    }
}

void StoryCharacterModel::func_020548a8(u32 id, u32 variant) {
    char data[0x7f];
    char nsbmd[0x7f];
    char nsbtx[0x7f];
    mUnk_1b9 = variant;
    ChangeDir("/Game/Character/Character");
    if (variant == 0) {
        sprintf(data, "chara%02d_cut_data.bin", id);
    } else {
        sprintf(data, "chara%02d_%02d_data.bin", id, variant);
    }
    sprintf(nsbmd, "chara%02d.nsbmd", id);
    sprintf(nsbtx, "chara%02d.nsbtx", id);
    func_020525c0(nsbtx, nsbmd, data, 1);
    mModel->mUnk_69 = 0;
    ChangeDir("/");
}

void StoryCharacterModel::func_02054990(u32 id, u32 variant) {
    mUnk_1b9 = variant;
    if (variant == 0) {
        sprintf(mUnk_3b, "chara%02d_cut_data.bin", id);
    } else {
        sprintf(mUnk_3b, "chara%02d_%02d_data.bin", id, variant);
    }
    sprintf(mUnk_ba, "chara%02d.nsbmd", id);
    sprintf(mUnk_139, "chara%02d.nsbtx", id);
    mUnk_1b8 = 1;
}

BOOL StoryCharacterModel::func_02054a0c() {
    BOOL result = FALSE;
    switch (mUnk_1b8) {
        case 1:
            ChangeDir("/Game/Character/Character");
            func_02023708(mUnk_139, 0, 0, 0, 0, 0);
            ChangeDir("/");
            mUnk_1b8++;
            break;
        case 2:
            if (func_02023874() == 0) {
                mUnk_2c = func_02023864();
                ChangeDir("/Game/Character/Character");
                func_02023708(mUnk_ba, 0, 0, 0, 0, 0);
                ChangeDir("/");
                mUnk_1b8++;
            }
            break;
        case 3:
            if (func_02023874() == 0) {
                mUnk_30 = func_02023864();
                ChangeDir("/Game/Character/Character");
                func_02023708(mUnk_3b, 0, 0, 0, 0, 0);
                ChangeDir("/");
                mUnk_1b8++;
            }
            break;
        case 4:
            if (func_02023874() == 0) {
                mUnk_34 = func_02023864();
                mUnk_1b8++;
            }
            break;
        case 5:
            result = TRUE;
            break;
    }
    return result;
}

void StoryCharacterModel::func_02054bd4() {
    if (mUnk_1b8 != 5) {
        return;
    }
    UnkStruct_02028308 *texture = new NitroTextureData(mUnk_2c);
    NitroModel *model           = new NitroModelData(mUnk_30, texture);
    texture->func_02028344();
    OS_Free(mUnk_2c);
    mUnk_2c                 = NULL;
    ModelAnimData *animData = new ModelAnimData(mUnk_34);
    ModelAnim *anim         = new ModelAnim(animData, model);
    mModel                  = model->func_02028b78(anim);
    mModel->func_02028e54(0, 1, 0x1000);
    mUnk_00 = 1;
    mModel->mUnk_69 = 0;
}

void StoryCharacterModel::func_02054cc4(u32 a, u32 b) {
    mModel->func_02028e54(a, 1, 0x1000);
    mModel->func_02028e54(b, 1, 0x1000);
    mUnk_39 = a;
    mUnk_3a = 0;
}

// non-matching: 0x98 vs 0xa0, same problem as CharacterModel::func_02053eec.
void StoryCharacterModel::func_02054d08(u32 flag) {
    mUnk_3a = flag;
    if (!(flag & 0xff)) {
        ModelAnim *anim;
        ModelAnim *anim2;
        u32 loop;
        u8 channel;
        s32 frame;
        NNSG3dAnmObj *obj;
        anim    = mModel->mAnim;
        anim2   = anim;
        channel = anim2->mUnk_5d;
        loop    = anim->mUnk_08;
        obj     = anim->func_02029a48(0);
        if (obj == NULL) {
            frame = 0;
        } else {
            frame = obj->mFrame;
        }
        mModel->func_02028e54(channel, loop, 0x1000);
        mModel->mAnim->func_020298c0(frame, 0);
        mModel->mAnim->func_020298c0(frame, 1);
    } else {
        NNSG3dAnmObj *obj = mModel->mAnim->func_02029a58(4, mUnk_39);
        mModel->func_02028f54(obj);
    }
}

void StoryCharacterModel::func_02054da8() {
    if (mUnk_38 == 0) {
        return;
    }
    func_020526bc();
    if (mUnk_3a == 0) {
        return;
    }
    mModel->func_02028da4();
}

void StoryCharacterModel::func_02054dd8(u32 a, u32 b) {
    if (mUnk_1b9 != 0) {
        a = 0;
    }
    func_02052768(a, b);
}
