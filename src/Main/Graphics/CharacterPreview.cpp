#include "Graphics/CharacterModel.hpp"
#include "Graphics/SpriteManager.hpp"
#include "System/FileSystem.hpp"
#include <nitro/os.h>
#include <stdio.h>

extern u8 gCharacterLook[];
extern MtxFx43 gIdentityMtx43;
extern "C" void MTX_SetTranslation43(MtxFx43 *mtx, VecFx32 *pos);

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
    u16 mGlassePalette : 3;
    u16 mUnk_6e : 3;
    u16 mTopKind : 3;
    u16 mTopPalette : 3;
    u16 mDownKind : 5;
    u16 mDownPalette : 3;
    u16 mShoesVariant : 3;
    u16 mShoesKind : 2;
    u16 mSkinPalette : 3;
    u16 mUnk_06_3 : 5;
    u16 mAttribute : 8;
};

class StoryCharacterModel : public ModelTransform {
public:
    /* 2c */ void *mTextureData;
    /* 30 */ void *mModelData;
    /* 34 */ void *mAnimData;
    /* 38 */ u8 mVisible;
    /* 39 */ u8 mExtraAnimSet;
    /* 3a */ u8 mExtraAnimEnabled;
    /* 3b */ char mAnimPath[0x7f];
    /* ba */ char mModelPath[0x7f];
    /* 139 */ char mTexturePath[0x7f];
    /* 1b8 */ u8 mLoadStep; // 0 until StartLoad; non-zero also tells the destructor to stop the load and free the data
    /* 1b9 */ u8 mVariant;
    /* 1ba */ PAD(0x1ba, 0x1bc);
    /* 1bc */

    StoryCharacterModel();
    ~StoryCharacterModel();
    void LoadChara(u32 id, u32 variant);
    void StartLoad(u32 id, u32 variant);
    BOOL UpdateLoad();
    void CreateModel();
    void SetExtraAnimSetAndStartAnim(u32 a, u32 b);
    void SetExtraAnimEnabled(u32 flag);
    void Draw();
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
    SpriteManager::Get().RestoreSlot(0);
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
        shadow->mOwnFlags = MODEL_OWN_ALL;
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
        SpriteManager::Get().SaveSlot(0);
    }
    mUseCharacter = unk1 == 0x15;
    if (mUseCharacter == 0) {
        mStoryCharacter = new StoryCharacterModel();
        ((StoryCharacterModel *) mStoryCharacter)->LoadChara(unk1, unk4);
        ((StoryCharacterModel *) mStoryCharacter)->mVisible = 1;
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
    ((CharacterModel *) mCharacter)->mGlassePalette = params->mGlassePalette;
    ((CharacterModel *) mCharacter)->mUnk_6e = params->mUnk_6e;
    ((CharacterModel *) mCharacter)->mTopKind = params->mTopKind;
    ((CharacterModel *) mCharacter)->mTopPalette = params->mTopPalette;
    ((CharacterModel *) mCharacter)->mDownKind = params->mDownKind;
    ((CharacterModel *) mCharacter)->mDownPalette = params->mDownPalette;
    ((CharacterModel *) mCharacter)->mShoesKind = params->mShoesKind;
    ((CharacterModel *) mCharacter)->mShoesVariant = params->mShoesVariant;
    ((CharacterModel *) mCharacter)->mAttribute = params->mAttribute;
}

void UnkStruct_0205401c::StartLoad(u32 unk1, u32 unk2, u32 unk3, u8 unk4) {
    if (unk2 != 0) {
        SpriteManager::Get().SaveSlot(0);
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
        ((StoryCharacterModel *) mStoryCharacter)->StartLoad(unk1, unk4);
    }
}

BOOL UnkStruct_0205401c::UpdateLoad() {
    BOOL result = FALSE;
    BOOL done;
    if (mUseCharacter == 1) {
        done = ((CharacterModel *) mCharacter)->UpdateLoad();
    } else {
        done = ((StoryCharacterModel *) mStoryCharacter)->UpdateLoad();
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
        ((StoryCharacterModel *) mStoryCharacter)->CreateModel();
        ((StoryCharacterModel *) mStoryCharacter)->mVisible = 1;
    }
    mBuilt = 1;
}

void UnkStruct_0205401c::Draw() {
    if (mUseCharacter == 0) {
        ((StoryCharacterModel *) mStoryCharacter)->Draw();
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
    MTX_SetTranslation43(&mtx, &pos);
    mShadow->AdvanceAnim();
    mShadow->Draw(&mtx, &scale);
}

void UnkStruct_0205401c::StartAnim(u32 unk1, u32 unk2) {
    if (mUseCharacter == 0) {
        ((StoryCharacterModel *) mStoryCharacter)->func_02054dd8(unk1, unk2);
    } else {
        ((CharacterModel *) mCharacter)->StartHeadAnim(unk1, unk2);
    }
}

void UnkStruct_0205401c::SetExtraAnimSetAndStartAnim(u32 unk1, u32 unk2) {
    if (mUseCharacter == 0) {
        ((StoryCharacterModel *) mStoryCharacter)->SetExtraAnimSetAndStartAnim(unk1, unk2);
    } else {
        ((CharacterModel *) mCharacter)->SetExtraAnimSetAndStartAnim(unk1, unk2);
    }
}

void UnkStruct_0205401c::SetExtraAnimEnabled(u32 unk1) {
    if (mUseCharacter == 0) {
        ((StoryCharacterModel *) mStoryCharacter)->SetExtraAnimEnabled(unk1);
    } else {
        ((CharacterModel *) mCharacter)->SetExtraAnimEnabled(unk1);
    }
}

void UnkStruct_0205401c::SetVisible(u8 unk1) {
    if (mUseCharacter == 0) {
        ((StoryCharacterModel *) mStoryCharacter)->mVisible = unk1;
    } else {
        ((CharacterModel *) mCharacter)->mVisible = unk1;
    }
}

u8 UnkStruct_0205401c::IsVisible() {
    if (mUseCharacter == 0) {
        return ((StoryCharacterModel *) mStoryCharacter)->mVisible;
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
    mTextureData  = NULL;
    mModelData  = NULL;
    mAnimData  = NULL;
    mVisible  = 0;
    mExtraAnimSet  = 0;
    mExtraAnimEnabled  = 0;
    mLoadStep = 0;
}

StoryCharacterModel::~StoryCharacterModel() {
    if (mLoadStep != 0) {
        StopAsyncLoad();
        ModelInstance *model = mModel;
        if (model != NULL) {
            model->mOwnFlags = MODEL_OWN_ALL;
            delete model;
            mModel = NULL;
        }
        if (mTextureData != NULL) {
            OS_Free(mTextureData);
        }
        if (mModelData != NULL) {
            OS_Free(mModelData);
        }
        if (mAnimData != NULL) {
            OS_Free(mAnimData);
        }
    }
}

void StoryCharacterModel::LoadChara(u32 id, u32 variant) {
    char data[0x7f];
    char nsbmd[0x7f];
    char nsbtx[0x7f];
    mVariant = variant;
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

void StoryCharacterModel::StartLoad(u32 id, u32 variant) {
    mVariant = variant;
    if (variant == 0) {
        sprintf(mAnimPath, "chara%02d_cut_data.bin", id);
    } else {
        sprintf(mAnimPath, "chara%02d_%02d_data.bin", id, variant);
    }
    sprintf(mModelPath, "chara%02d.nsbmd", id);
    sprintf(mTexturePath, "chara%02d.nsbtx", id);
    mLoadStep = 1;
}

BOOL StoryCharacterModel::UpdateLoad() {
    BOOL result = FALSE;
    switch (mLoadStep) {
        case 1:
            ChangeDir("/Game/Character/Character");
            LoadFileAsync(mTexturePath, 0, 0, 0, 0, 0);
            ChangeDir("/");
            mLoadStep++;
            break;
        case 2:
            if (IsAsyncLoadActive() == 0) {
                mTextureData = GetAsyncLoadBuffer();
                ChangeDir("/Game/Character/Character");
                LoadFileAsync(mModelPath, 0, 0, 0, 0, 0);
                ChangeDir("/");
                mLoadStep++;
            }
            break;
        case 3:
            if (IsAsyncLoadActive() == 0) {
                mModelData = GetAsyncLoadBuffer();
                ChangeDir("/Game/Character/Character");
                LoadFileAsync(mAnimPath, 0, 0, 0, 0, 0);
                ChangeDir("/");
                mLoadStep++;
            }
            break;
        case 4:
            if (IsAsyncLoadActive() == 0) {
                mAnimData = GetAsyncLoadBuffer();
                mLoadStep++;
            }
            break;
        case 5:
            result = TRUE;
            break;
    }
    return result;
}

void StoryCharacterModel::CreateModel() {
    if (mLoadStep != 5) {
        return;
    }
    TextureFile *texture = new NitroTextureData(mTextureData);
    NitroModel *model           = new NitroModelData(mModelData, texture);
    texture->FreeFile();
    OS_Free(mTextureData);
    mTextureData                 = NULL;
    ModelAnimData *animData = new ModelAnimData(mAnimData);
    ModelAnim *anim         = new ModelAnim(animData, model);
    mModel                  = model->CreateInstance(anim);
    mModel->StartAnimWithIncrement(0, 1, 0x1000);
    mAnimEnabled = 1;
    mModel->mUseBoxTest = 0;
}

void StoryCharacterModel::SetExtraAnimSetAndStartAnim(u32 a, u32 b) {
    mModel->StartAnimWithIncrement(a, 1, 0x1000);
    mModel->StartAnimWithIncrement(b, 1, 0x1000);
    mExtraAnimSet = a;
    mExtraAnimEnabled = 0;
}

void StoryCharacterModel::SetExtraAnimEnabled(u32 flag) {
    mExtraAnimEnabled = flag;
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
        NNSG3dAnmObj *obj = mModel->mAnim->GetAnimObjForSet(4, mExtraAnimSet);
        mModel->AttachExtraAnim(obj);
    }
}

void StoryCharacterModel::Draw() {
    if (mVisible == 0) {
        return;
    }
    DrawInstance();
    if (mExtraAnimEnabled == 0) {
        return;
    }
    mModel->AdvanceExtraAnim();
}

void StoryCharacterModel::func_02054dd8(u32 a, u32 b) {
    if (mVariant != 0) {
        a = 0;
    }
    StartAnim(a, b);
}
