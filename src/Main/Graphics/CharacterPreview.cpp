#include "Graphics/CharacterModel.hpp"
#include "Graphics/SpriteManager.hpp"
#include "System/FileSystem.hpp"
#include <nitro/os.h>
#include <stdio.h>

extern u8 gCharacterLook[];
extern MtxFx43 gIdentityMtx43;
extern "C" void func_01ffcf64(MtxFx43 *mtx, VecFx32 *pos);

// 0x1bc bytes, built from /Game/Character/Character/charaNN files.
// Custom character appearance as stored in the save data (8 bytes, gCharacterLook).
struct CharacterParams {
    u16 mUnk_00_0 : 1;
    u16 mFaceKind : 3;
    u16 mFaceVariant : 3;
    u16 mHairKind : 3;
    u16 mHairVariant : 3;
    u16 mCapHandle : 3;
    u16 mCapPalette : 3;
    u16 mGlasseHandle : 3;
    u16 mUnk_6d : 3;
    u16 mUnk_6e : 3;
    u16 mTopKind : 3;
    u16 mTopPalette : 3;
    u16 mDownKind : 5;
    u16 mDownPalette : 3;
    u16 mShoesVariant : 3;
    u16 mShoesKind : 2;
    u16 mSkinPalette : 3;
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

// The current animation set of a model (read through a const pointer, so it is a load of its own).
static inline u8 GetSet(const ModelInstance *model) {
    return model->mAnim->mSet;
}

static inline void SetRotation(ModelTransform *model, const u16 *rot) {
    model->mRotX = rot[0];
    model->mRotY = rot[1];
    model->mRotZ = rot[2];
}

UnkStruct_0205401c::UnkStruct_0205401c() {
    mShadow = NULL;
    mCharacter = NULL;
    mStoryCharacter = NULL;
    mBuilt = 0;
}

UnkStruct_0205401c *UnkStruct_0205401c::Release() {
    SpriteManager::Get().func_0202753c(0);
    StoryCharacterModel *story = (StoryCharacterModel *) mStoryCharacter;
    if (story != NULL) {
        delete story;
    }
    mStoryCharacter = NULL;
    CharacterModel *character = (CharacterModel *) mCharacter;
    if (character != NULL) {
        delete character;
    }
    mCharacter = NULL;
    ModelInstance *shadow = mShadow;
    if (shadow != NULL) {
        shadow->mOwnFlags = 0xf;
        delete shadow;
        mShadow = NULL;
    }
    return this;
}

void UnkStruct_0205401c::Create(u32 unk1, u32 unk2, u32 unk3, u32 unk4) {
    if (unk3 == 0) {
        unk3 = (u32) gCharacterLook;
    }
    if (unk2 != 0) {
        SpriteManager::Get().func_02027504(0);
    }
    mUseCharacter = unk1 == 0x15;
    if (mUseCharacter == 0) {
        mStoryCharacter = new StoryCharacterModel();
        ((StoryCharacterModel *) mStoryCharacter)->func_020548a8(unk1, unk4);
        ((StoryCharacterModel *) mStoryCharacter)->mUnk_38 = 1;
    } else {
        CharacterModel *character = new CharacterModel();
        mCharacter                   = (UnkStruct_0205401c_04 *) character;
        SetParams((const CharacterParams *) unk3);
        ((CharacterModel *) mCharacter)->LoadPart(0, unk4);
        ((CharacterModel *) mCharacter)->LoadPart(1, unk4);
        ((CharacterModel *) mCharacter)->LoadPart(2, unk4);
        ((CharacterModel *) mCharacter)->LoadPart(3, unk4);
        ((CharacterModel *) mCharacter)->LoadPart(4, unk4);
        ((CharacterModel *) mCharacter)->LoadPart(5, unk4);
        ((CharacterModel *) mCharacter)->mVisible = 1;
    }
    mBuilt = 1;
}

void UnkStruct_0205401c::SetParams(const CharacterParams *params) {
    ((CharacterModel *) mCharacter)->mSkinPalette = params->mSkinPalette;
    ((CharacterModel *) mCharacter)->mFaceKind = params->mFaceKind;
    ((CharacterModel *) mCharacter)->mFaceVariant = params->mFaceVariant;
    ((CharacterModel *) mCharacter)->mHairKind = params->mHairKind;
    ((CharacterModel *) mCharacter)->mHairVariant = params->mHairVariant;
    ((CharacterModel *) mCharacter)->mCapHandle = params->mCapHandle;
    ((CharacterModel *) mCharacter)->mCapPalette = params->mCapPalette;
    ((CharacterModel *) mCharacter)->mGlasseHandle = params->mGlasseHandle;
    ((CharacterModel *) mCharacter)->mUnk_6d = params->mUnk_6d;
    ((CharacterModel *) mCharacter)->mUnk_6e = params->mUnk_6e;
    ((CharacterModel *) mCharacter)->mTopKind = params->mTopKind;
    ((CharacterModel *) mCharacter)->mTopPalette = params->mTopPalette;
    ((CharacterModel *) mCharacter)->mDownKind = params->mDownKind;
    ((CharacterModel *) mCharacter)->mDownPalette = params->mDownPalette;
    ((CharacterModel *) mCharacter)->mShoesKind = params->mShoesKind;
    ((CharacterModel *) mCharacter)->mShoesVariant = params->mShoesVariant;
    ((CharacterModel *) mCharacter)->mUnk_75 = params->mUnk_75;
}

void UnkStruct_0205401c::StartLoad(u32 unk1, u32 unk2, u32 unk3, u8 unk4) {
    if (unk2 != 0) {
        SpriteManager::Get().func_02027504(0);
    }
    if (unk1 == 0x15) {
        if (unk3 == 0) {
            unk3 = (u32) gCharacterLook;
        }
        mUseCharacter = 1;
        mCharacter = (UnkStruct_0205401c_04 *) new CharacterModel();
        SetParams((const CharacterParams *) unk3);
        ((CharacterModel *) mCharacter)->StartLoad(unk4);
    } else {
        mUseCharacter = 0;
        mStoryCharacter = new StoryCharacterModel();
        ((StoryCharacterModel *) mStoryCharacter)->func_02054990(unk1, unk4);
    }
}

BOOL UnkStruct_0205401c::UpdateLoad() {
    BOOL result = FALSE;
    BOOL done;
    if (mUseCharacter == 1) {
        done = ((CharacterModel *) mCharacter)->UpdateLoad();
    } else {
        done = ((StoryCharacterModel *) mStoryCharacter)->func_02054a0c();
    }
    if (done) {
        result = TRUE;
    }
    return result;
}

void UnkStruct_0205401c::FinishLoad() {
    if (mUseCharacter == 1) {
        ((CharacterModel *) mCharacter)->CreateHeadAndLoadParts();
        ((CharacterModel *) mCharacter)->mVisible = 1;
    } else {
        ((StoryCharacterModel *) mStoryCharacter)->func_02054bd4();
        ((StoryCharacterModel *) mStoryCharacter)->mUnk_38 = 1;
    }
    mBuilt = 1;
}

void UnkStruct_0205401c::Draw() {
    if (mUseCharacter == 0) {
        ((StoryCharacterModel *) mStoryCharacter)->func_02054da8();
    } else {
        ((CharacterModel *) mCharacter)->DrawParts();
    }
    if (mShadow == NULL) {
        return;
    }
    MtxFx43 mtx = gIdentityMtx43;
    VecFx32 scale;
    VecFx32 pos;
    scale.x     = 0x4000;
    scale.y     = 0x4000;
    scale.z     = 0x4000;
    pos.x       = 0;
    pos.y       = -0x800;
    pos.z       = 0;
    func_01ffcf64(&mtx, &pos);
    mShadow->AdvanceAnim();
    mShadow->Draw(&mtx, &scale);
}

void UnkStruct_0205401c::func_0205461c(u32 unk1, u32 unk2) {
    if (mUseCharacter == 0) {
        ((StoryCharacterModel *) mStoryCharacter)->func_02054dd8(unk1, unk2);
    } else {
        ((CharacterModel *) mCharacter)->StartHeadAnim(unk1, unk2);
    }
}

void UnkStruct_0205401c::func_02054644(u32 unk1, u32 unk2) {
    if (mUseCharacter == 0) {
        ((StoryCharacterModel *) mStoryCharacter)->func_02054cc4(unk1, unk2);
    } else {
        ((CharacterModel *) mCharacter)->func_02053ea0(unk1, unk2);
    }
}

void UnkStruct_0205401c::func_0205466c(u32 unk1) {
    if (mUseCharacter == 0) {
        ((StoryCharacterModel *) mStoryCharacter)->func_02054d08(unk1);
    } else {
        ((CharacterModel *) mCharacter)->SetExtraAnimEnabled(unk1);
    }
}

void UnkStruct_0205401c::SetVisible(u8 unk1) {
    if (mUseCharacter == 0) {
        ((StoryCharacterModel *) mStoryCharacter)->mUnk_38 = unk1;
    } else {
        ((CharacterModel *) mCharacter)->mVisible = unk1;
    }
}

u8 UnkStruct_0205401c::IsVisible() {
    if (mUseCharacter == 0) {
        return ((StoryCharacterModel *) mStoryCharacter)->mUnk_38;
    }
    return ((CharacterModel *) mCharacter)->mVisible;
}

u16 *UnkStruct_0205401c::GetModelRotation() {
    ModelTransform *model;
    if (mUseCharacter == 0) {
        model = (StoryCharacterModel *) mStoryCharacter;
    } else {
        model = (CharacterModel *) mCharacter;
    }
    return &model->mRotX;
}

void UnkStruct_0205401c::SetModelRotation(const u16 *rot) {
    if (mUseCharacter == 0) {
        SetRotation((StoryCharacterModel *) mStoryCharacter, rot);
    } else {
        SetRotation((CharacterModel *) mCharacter, rot);
    }
}

s32 UnkStruct_0205401c::IsAnimEndReached() {
    if (mUseCharacter == 0) {
        return ((StoryCharacterModel *) mStoryCharacter)->mModel->IsAnimFinished();
    }
    return ((CharacterModel *) mCharacter)->mParts[0]->IsAnimFinished();
}

void UnkStruct_0205401c::LoadShadow() {
    ChangeDir("/story/mdl");
    NitroTexture *texture = new NitroTexture("cut_shadow.nsbtx");
    NitroModel *model     = new NitroModel("cut_shadow.nsbmd", texture);
    texture->FreeFile();
    mShadow = model->CreateInstance(NULL);
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
        StopAsyncLoad();
        ModelInstance *model = mModel;
        if (model != NULL) {
            model->mOwnFlags = 0xf;
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
    LoadFiles(nsbtx, nsbmd, data, 1);
    mModel->mUseBoxTest = 0;
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
            LoadFileAsync(mUnk_139, 0, 0, 0, 0, 0);
            ChangeDir("/");
            mUnk_1b8++;
            break;
        case 2:
            if (IsAsyncLoadActive() == 0) {
                mUnk_2c = GetAsyncLoadBuffer();
                ChangeDir("/Game/Character/Character");
                LoadFileAsync(mUnk_ba, 0, 0, 0, 0, 0);
                ChangeDir("/");
                mUnk_1b8++;
            }
            break;
        case 3:
            if (IsAsyncLoadActive() == 0) {
                mUnk_30 = GetAsyncLoadBuffer();
                ChangeDir("/Game/Character/Character");
                LoadFileAsync(mUnk_3b, 0, 0, 0, 0, 0);
                ChangeDir("/");
                mUnk_1b8++;
            }
            break;
        case 4:
            if (IsAsyncLoadActive() == 0) {
                mUnk_34 = GetAsyncLoadBuffer();
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
    TextureFile *texture = new NitroTextureData(mUnk_2c);
    NitroModel *model           = new NitroModelData(mUnk_30, texture);
    texture->FreeFile();
    OS_Free(mUnk_2c);
    mUnk_2c                 = NULL;
    ModelAnimData *animData = new ModelAnimData(mUnk_34);
    ModelAnim *anim         = new ModelAnim(animData, model);
    mModel                  = model->CreateInstance(anim);
    mModel->StartAnimWithIncrement(0, 1, 0x1000);
    mAnimEnabled = 1;
    mModel->mUseBoxTest = 0;
}

void StoryCharacterModel::func_02054cc4(u32 a, u32 b) {
    mModel->StartAnimWithIncrement(a, 1, 0x1000);
    mModel->StartAnimWithIncrement(b, 1, 0x1000);
    mUnk_39 = a;
    mUnk_3a = 0;
}

void StoryCharacterModel::func_02054d08(u32 flag) {
    mUnk_3a = flag;
    if (!(flag & 0xff)) {
        ModelAnim *anim = mModel->mAnim;
        u8 channel      = GetSet(mModel);
        u32 loop        = anim->mLoop;
        NNSG3dAnmObj *obj;
        s32 frame;
        obj = anim->GetAnimObj(0);
        if (obj == NULL) {
            frame = 0;
        } else {
            frame = obj->mFrame;
        }
        mModel->StartAnimWithIncrement(channel, loop, 0x1000);
        mModel->mAnim->SetFrame(frame, 0);
        mModel->mAnim->SetFrame(frame, 1);
    } else {
        NNSG3dAnmObj *obj = mModel->mAnim->GetAnimObjForSet(4, mUnk_39);
        mModel->AttachExtraAnim(obj);
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
    mModel->AdvanceExtraAnim();
}

void StoryCharacterModel::func_02054dd8(u32 a, u32 b) {
    if (mUnk_1b9 != 0) {
        a = 0;
    }
    StartAnim(a, b);
}
