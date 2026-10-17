#include "Graphics/CharacterModel.hpp"
#include "System/FileSystem.hpp"
#include <nitro/mi.h>
#include <nitro/os.h>
#include <stdio.h>

// the original passes a pointer here (the declaration in FileSystem.hpp takes u32)
const char *gCharacterPartNames[6] = {"head", "up", "down", "hair", "glasse", "cap"}; // part names, .data 0x020b24bc-0x020b24fc
extern MtxFx43 gIdentityMtx43;
extern MtxFx43 gHeadNodeMtx; // rotated in place (Y 0x8000, Z 0x4000) before parts 3-5 are drawn with it
extern "C" void func_02031838(void *renderObj, void (*callback)(NNSG3dRS *));
extern "C" MtxFx43 *func_020323b8();
extern "C" void MTX_Concat43(MtxFx43 *a, MtxFx43 *b, MtxFx43 *out);
extern "C" void MI_CpuCopy8(const void *src, void *dst, u32 size);
void InstallHeadNodeMtxCallback(NNSG3dRS *rs);
void StoreHeadNodeMtx(NNSG3dRS *rs);

// The current animation set of a model (read through a const pointer, so it is a load of its own).
static inline u8 GetSet(const ModelInstance *model) {
    return model->mAnim->mSet;
}

CharacterModel::CharacterModel() {
    mUnk_60 = NULL;
    mVisible = 0;
    mExtraAnimSet = 0;
    mExtraAnimEnabled = 0;
    for (u8 i = 0; i < 6; i++) {
        mParts[i] = NULL;
    }
    mUnk_40 = -1;
    mUnk_41 = -1;
    mUnk_44 = 2;
    MI_CpuFill8(mPartFileNumbers, 0, 6);
    mHeadTextureData = NULL;
    mHeadModelData = NULL;
    mMotionData = NULL;
}

CharacterModel::~CharacterModel() {
    if (mParts[0] != NULL) {
        ClearRenderObjCallback((u8 *) mParts[0] + 4);
    }
    FreeParts();
    if (mHeadTextureData != NULL) {
        OS_Free(mHeadTextureData);
    }
    if (mHeadModelData != NULL) {
        OS_Free(mHeadModelData);
    }
    if (mMotionData != NULL) {
        OS_Free(mMotionData);
    }
}

void CharacterModel::StartHeadAnim(u32 a, u32 b) {
    if (mParts[0] == NULL) {
        return;
    }
    mParts[0]->StartAnimWithIncrement(a, b, 0x1000);
}

void CharacterModel::LoadPart(u32 part, u32 motionKind) {
    char buf[0x20];
    switch (part) {
        case 0: {
            StartLoad(motionKind);
            u32 async = 0;
            while (!LoadHeadFiles(async)) {
            }
            CreateHead();
            break;
        }
        case 1: {
            mPartFileNumbers[1] = mTopKind;
            ChangeDir("/Game/Character/EditedCharacter/Male/Up");
            sprintf(buf, "edit_%s_M%02d.nsbtx", gCharacterPartNames[1], mPartFileNumbers[1]);
            TextureFile *texture = new NitroTexture(buf);
            sprintf(buf, "edit_%s_M%02d.nsbmd", gCharacterPartNames[1], mPartFileNumbers[1]);
            NitroModel *model = new NitroModel(buf, texture);
            mParts[1]         = model->CreateInstance(NULL);
            LoadPartTextures(1);
            texture->FreeFile();
            if (mUnk_60 != NULL) {
                mParts[1]->mUnk_38 = mUnk_60;
            }
            mParts[1]->mUseBoxTest = 0;
            break;
        }
        case 2: {
            mPartFileNumbers[2] = mDownKind * 3 + mShoesKind;
            ChangeDir("/Game/Character/EditedCharacter/Male/Down");
            sprintf(buf, "edit_%s_M%02d.nsbtx", gCharacterPartNames[2], mPartFileNumbers[2]);
            TextureFile *texture = new NitroTexture(buf);
            sprintf(buf, "edit_%s_M%02d.nsbmd", gCharacterPartNames[2], mPartFileNumbers[2]);
            NitroModel *model = new NitroModel(buf, texture);
            mParts[2]         = model->CreateInstance(NULL);
            LoadPartTextures(2);
            texture->FreeFile();
            if (mUnk_60 != NULL) {
                mParts[2]->mUnk_38 = mUnk_60;
            }
            mParts[2]->mUseBoxTest = 0;
            break;
        }
        case 3: {
            mPartFileNumbers[3] = mHairKind;
            ChangeDir("/Game/Character/EditedCharacter/Male/Hair");
            sprintf(buf, "edit_%s_M%02d.nsbtx", gCharacterPartNames[3], mPartFileNumbers[3]);
            TextureFile *texture = new NitroTexture(buf);
            sprintf(buf, "edit_%s_M%02d.nsbmd", gCharacterPartNames[3], mPartFileNumbers[3]);
            NitroModel *model = new NitroModel(buf, texture);
            mParts[3]         = model->CreateInstance(NULL);
            LoadPartTextures(3);
            texture->FreeFile();
            mParts[3]->mUseBoxTest = 0;
            break;
        }
        case 4: {
            mPartFileNumbers[4] = mGlasseHandle;
            if (mPartFileNumbers[4] != 0) {
                ChangeDir("/Game/Character/EditedCharacter/Male/Glasse");
                sprintf(buf, "edit_%s_MF%02d.nsbtx", gCharacterPartNames[4], mPartFileNumbers[4] - 1);
                TextureFile *texture = new NitroTexture(buf);
                sprintf(buf, "edit_%s_MF%02d.nsbmd", gCharacterPartNames[4], mPartFileNumbers[4] - 1);
                NitroModel *model = new NitroModel(buf, texture);
                mParts[4]         = model->CreateInstance(NULL);
                LoadPartTextures(4);
                texture->FreeFile();
                mParts[4]->mUseBoxTest = 0;
            }
            break;
        }
        case 5: {
            mPartFileNumbers[5] = mCapHandle;
            if (mPartFileNumbers[5] != 0) {
                ChangeDir("/Game/Character/EditedCharacter/Male/Cap");
                sprintf(buf, "edit_%s_M%02d.nsbtx", gCharacterPartNames[5], mPartFileNumbers[5] - 1);
                TextureFile *texture = new NitroTexture(buf);
                sprintf(buf, "edit_%s_M%02d.nsbmd", gCharacterPartNames[5], mPartFileNumbers[5] - 1);
                NitroModel *model = new NitroModel(buf, texture);
                mParts[5]         = model->CreateInstance(NULL);
                LoadPartTextures(5);
                texture->FreeFile();
                mParts[5]->mUseBoxTest = 0;
            }
            break;
        }
    }
    ChangeDir("/");
}

void CharacterModel::StartLoad(u32 motionKind) {
    mMotionKind = motionKind;
    mHeadLoadStep = 0;
    mUnk_7a = 0;
}

BOOL CharacterModel::UpdateLoad() {
    BOOL result = FALSE;
    if (mUnk_7a == 0) {
        if (LoadHeadFiles(1)) {
            mHeadLoadStep = 0;
            result  = TRUE;
        }
    }
    return result;
}

void CharacterModel::CreateHeadAndLoadParts() {
    CreateHead();
    LoadPart(1, mMotionKind);
    LoadPart(2, mMotionKind);
    LoadPart(3, mMotionKind);
    LoadPart(4, mMotionKind);
    LoadPart(5, mMotionKind);
}

BOOL CharacterModel::LoadHeadFiles(u32 async) {
    char buf[0x20];
    BOOL result = FALSE;
    switch (mHeadLoadStep) {
        case 0:
            mPartFileNumbers[0] = 0;
            ChangeDir("/Game/Character/EditedCharacter/Male/Head");
            sprintf(buf, "edit_%s_M%02d.nsbtx", gCharacterPartNames[0], mPartFileNumbers[0]);
            if (async) {
                LoadFileAsync(buf, 0, 0, 0, 0, 0);
            } else {
                mHeadTextureData = LoadFile(buf, 0, 0, 0, 0, 0);
            }
            ChangeDir("/");
            mHeadLoadStep++;
            break;
        case 1:
            if (IsAsyncLoadActive() == 0) {
                ChangeDir("/Game/Character/EditedCharacter/Male/Head");
                sprintf(buf, "edit_%s_M%02d.nsbmd", gCharacterPartNames[0], mPartFileNumbers[0]);
                if (async) {
                    mHeadTextureData = GetAsyncLoadBuffer();
                    LoadFileAsync(buf, 0, 0, 0, 0, 0);
                } else {
                    mHeadModelData = LoadFile(buf, 0, 0, 0, 0, 0);
                }
                ChangeDir("/");
                mHeadLoadStep++;
            }
            break;
        case 2:
            if (IsAsyncLoadActive() == 0) {
                ChangeDir("/Game/Character/EditedCharacter/Anm");
                if (mMotionKind == 0) {
                    sprintf(buf, "edit_motion_m_cut_data.bin");
                } else {
                    sprintf(buf, "edit_motion_m_data.bin");
                }
                if (async) {
                    mHeadModelData = GetAsyncLoadBuffer();
                    LoadFileAsync(buf, 0, 0, 0, 0, 0);
                } else {
                    mMotionData = LoadFile(buf, 0, 0, 0, 0, 0);
                    result  = TRUE;
                }
                ChangeDir("/");
                mHeadLoadStep++;
            }
            break;
        case 3:
            if (IsAsyncLoadActive() == 0) {
                mMotionData = GetAsyncLoadBuffer();
                result  = TRUE;
            }
            break;
    }
    return result;
}

void CharacterModel::CreateHead() {
    TextureFile *texture = new NitroTextureData(mHeadTextureData);
    NitroModel *model           = new NitroModelData(mHeadModelData, texture);
    ModelAnimData *animData     = new ModelAnimData(mMotionData);
    ModelAnim *anim             = new ModelAnim(animData, model);
    mParts[0]                   = model->CreateInstance(anim);
    LoadPartTextures(0);
    texture->FreeFile();
    OS_Free(mHeadTextureData);
    mHeadTextureData = NULL;
    if (mUnk_60 == NULL) {
        s16 count = *(u8 *) (*(u8 **) ((u8 *) mParts[0] + 8) + 0x17);
        mUnk_60   = OS_Alloc(count * 0x58);
    }
    if (mUnk_60 != NULL) {
        mParts[0]->mUnk_38 = mUnk_60;
    }
    mParts[0]->mUseBoxTest = 0;
}


void CharacterModel::FreeParts() {
    for (u8 i = 0; i < 6; i++) {
        ModelInstance *inst = mParts[i];
        if (inst != NULL) {
            if (inst->mUnk_3c == 0) {
                *(u32 *) ((u8 *) inst + 4) &= ~1;
            }
            inst->mUnk_38 = NULL;
            inst = mParts[i];
            inst->mOwnFlags = MODEL_OWN_ALL;
            delete inst;
            mParts[i] = NULL;
        }
    }
    if (mUnk_60 != NULL) {
        OS_Free(mUnk_60);
        mUnk_60 = NULL;
    }
}

void CharacterModel::DrawParts() {
    MtxFx43 mtx = gIdentityMtx43;
    if (mParts[0] != NULL) {
        mParts[0]->AdvanceAnim();
    }
    if (mVisible == 0) {
        return;
    }
    MTX_SetTranslation43(&mtx, &mPosition);
    MTX_ApplyRotX43(&mtx, mRotX);
    MTX_ApplyRotY43(&mtx, mRotY);
    MTX_ApplyRotZ43(&mtx, mRotZ);
    if (mParts[0] != NULL) {
        func_02031838((u8 *) mParts[0] + 4, InstallHeadNodeMtxCallback);
        *(u32 *) ((u8 *) mParts[0] + 4) |= 1;
        mParts[0]->Draw(&mtx, &mScale);
        if (mExtraAnimEnabled != 0) {
            mParts[0]->AdvanceExtraAnim();
        }
    }
    if (mParts[1] != NULL) {
        mParts[1]->Draw(&mtx, &mScale);
    }
    if (mParts[2] != NULL) {
        mParts[2]->Draw(&mtx, &mScale);
    }
    MTX_ApplyRotY43(&gHeadNodeMtx, 0x8000);
    MTX_ApplyRotZ43(&gHeadNodeMtx, 0x4000);
    if (mParts[5] != NULL) {
        mParts[5]->Draw(&gHeadNodeMtx, &mScale);
    } else if (mParts[3] != NULL) {
        mParts[3]->Draw(&gHeadNodeMtx, &mScale);
    }
    if (mParts[4] != NULL) {
        mParts[4]->Draw(&gHeadNodeMtx, &mScale);
    }
}

void CharacterModel::LoadPartTextures(u32 part) {
    char bufA[0x7f];
    char bufB[0x7f];
    char bufC[0x7f];
    char bufD[0x7f];
    switch (part) {
        case 0: {
            ChangeDir("/Game/Character/EditedCharacter/Male/Tex/Skin");
            sprintf(bufA, "Skin_00");
            sprintf(bufB, "Skin_00_%03d", mSkinPalette);
            LoadPartPalette(part, bufA, bufB);
            ChangeDir("/Game/Character/EditedCharacter/Male/Tex/Mouth");
            sprintf(bufA, "Mouth_00");
            sprintf(bufB, "Mouth_00_%03d", mSkinPalette);
            LoadPartPalette(part, bufA, bufB);
            ChangeDir("/Game/Character/EditedCharacter/Male/Tex/Face");
            sprintf(bufA, "Face_00");
            sprintf(bufB, "Face_%02d_%03d", mFaceKind, mFaceVariant);
            sprintf(bufC, "Face_%02d_%03d", mFaceKind, mSkinPalette);
            u8 table[4] = {3, 2, 2, 3};
            u8 offset = table[mFaceKind];
            LoadPartTexture(part, bufA, bufB);
            LoadMergedPalette(part, bufA, bufB, bufC, offset);
            break;
        }
        case 1: {
            ChangeDir("/Game/Character/EditedCharacter/Male/Tex/Skin");
            sprintf(bufA, "Skin_00");
            sprintf(bufB, "Skin_00_%03d", mSkinPalette);
            LoadPartPalette(part, bufA, bufB);
            ChangeDir("/Game/Character/EditedCharacter/Male/Tex/Mobile");
            sprintf(bufA, "Mobile_00");
            sprintf(bufB, "Mobile_00_%03d", mAttribute);
            LoadPartPalette(part, bufA, bufB);
            ChangeDir("/Game/Character/EditedCharacter/Male/Tex/Gloves");
            sprintf(bufA, "Gloves_00");
            sprintf(bufB, "Gloves_00_%03d", mAttribute);
            LoadPartPalette(part, bufA, bufB);
            ChangeDir("/Game/Character/EditedCharacter/Male/Tex/Top");
            sprintf(bufA, "Top_%02d", mTopKind);
            sprintf(bufB, "Top_%02d_%03d", mTopKind, mAttribute);
            sprintf(bufC, "Top_%02d_%03d", mTopKind, mTopPalette);
            u8 table[6] = {4, 8, 8, 7, 6, 0xe};
            u8 offset = table[mTopKind];
            LoadPartTexture(part, bufA, bufB);
            LoadMergedPalette(part, bufA, bufB, bufC, offset);
            break;
        }
        case 2: {
            if (mDownKind == 0) {
                ChangeDir("/Game/Character/EditedCharacter/Male/Tex/Item");
                sprintf(bufA, "item_00");
                sprintf(bufB, "item_00_%03d", mAttribute);
                sprintf(bufC, "item_00_%03d", mDownPalette);
                LoadMergedPalette(part, bufA, bufB, bufC, 9);
            }
            if (mDownKind == 1 || mDownKind == 5) {
                ChangeDir("/Game/Character/EditedCharacter/Male/Tex/Skin");
                sprintf(bufA, "Skin_00");
                sprintf(bufB, "Skin_00_%03d", mSkinPalette);
                LoadPartPalette(part, bufA, bufB);
            }
            ChangeDir("/Game/Character/EditedCharacter/Male/Tex/Shoes");
            sprintf(bufA, "Shoes_%02d", mShoesKind);
            sprintf(bufB, "Shoes_%02d_%03d", mShoesKind, mShoesVariant);
            LoadPartTexture(part, bufA, bufB);
            LoadPartPalette(part, bufA, bufB);
            ChangeDir("/Game/Character/EditedCharacter/Male/Tex/Down");
            sprintf(bufA, "Down_%02d", mDownKind);
            sprintf(bufB, "Down_%02d_%03d", mDownKind, mAttribute);
            sprintf(bufC, "Down_%02d_%03d", mDownKind, mDownPalette);
            u8 table[6] = {0, 4, 5, 4, 2, 0xe};
            u8 offset = table[mDownKind];
            LoadPartTexture(part, bufA, bufB);
            LoadMergedPalette(part, bufA, bufB, bufC, offset);
            break;
        }
        case 3: {
            ChangeDir("/Game/Character/EditedCharacter/Male/Tex/Hair");
            sprintf(bufA, "Hair_%02d", mHairKind);
            sprintf(bufB, "Hair_%02d_%03d", 0, mHairVariant);
            LoadPartPalette(part, bufA, bufB);
            break;
        }
        case 4: {
            ChangeDir("/Game/Character/EditedCharacter/Male/Tex/Glasse");
            sprintf(bufA, "Glasse_%02d", mGlasseHandle - 1);
            if (mUnk_6e == 0) {
                sprintf(bufD, "Glasse_%02d_%03d", mGlasseHandle - 1, 0);
                sprintf(bufB, "Glasse_%02d_%03d", mGlasseHandle - 1, 0);
                sprintf(bufC, "Glasse_%02d_%03d", mGlasseHandle - 1, mGlassePalette);
            } else {
                sprintf(bufD, "Glasse_%02d_%03d", mGlasseHandle - 1, 1);
                sprintf(bufB, "Glasse_%02d_%03d", mGlasseHandle - 1, mUnk_6e - 1);
                sprintf(bufC, "Glasse_%02d_%03d", mGlasseHandle - 1, mGlassePalette);
            }
            u8 table[4] = {0xf, 0xf, 0xf, 0xf};
            u8 offset = table[mGlasseHandle - 1];
            LoadPartTexture(part, bufA, bufD);
            LoadMergedPalette(part, bufA, bufB, bufC, offset);
            break;
        }
        case 5: {
            ChangeDir("/Game/Character/EditedCharacter/Male/Tex/Cap");
            sprintf(bufA, "Cap_%02d", mCapHandle - 1);
            sprintf(bufB, "Cap_%02d_%03d", mCapHandle - 1, mHairVariant);
            sprintf(bufC, "Cap_%02d_%03d", mCapHandle - 1, mCapPalette);
            u8 table[5] = {3, 5, 0xc, 5, 0xc};
            u8 offset = table[mCapHandle - 1];
            LoadPartTexture(part, bufA, bufB);
            LoadMergedPalette(part, bufA, bufB, bufC, offset);
            break;
        }
    }
    ChangeDir("/");
}

BOOL CharacterModel::LoadPartTexture(u32 part, const char *name, const char *file) {
    return mParts[part]->mModelFile->mTexture->LoadTextureFile(name, file, 0);
}

void CharacterModel::LoadPartPalette(u32 part, const char *name, const char *file) {
    mParts[part]->mModelFile->mTexture->LoadPaletteFile(name, file, 0);
}

void CharacterModel::LoadMergedPalette(u32 part, const char *name, const char *file1, const char *file2, u8 offset) {
    TextureFile *texture = mParts[part]->mModelFile->mTexture;
    char buf1[0x20];
    char buf2[0x20];
    u32 size;
    u8 *data2;
    u8 *data1;
    sprintf(buf1, "%s.ntfp", file1);
    data1 = (u8 *) LoadFile(buf1, 0, 0, 0, &size, 0);
    sprintf(buf2, "%s.ntfp", file2);
    data2 = (u8 *) LoadFile(buf2, 0, 0, 0, &size, 0);
    MI_CpuCopy8(data2 + offset * 2, data1 + offset * 2, size - offset * 2);
    texture->UploadNamedPalette(name, data1, size);
    OS_Free(data1);
    OS_Free(data2);
}

void CharacterModel::SetExtraAnimSetAndStartAnim(u32 a, u32 b) {
    if (mParts[0] == NULL) {
        return;
    }
    mParts[0]->StartAnimWithIncrement(a, 1, 0x1000);
    mParts[0]->StartAnimWithIncrement(b, 1, 0x1000);
    mExtraAnimSet = a;
    mExtraAnimEnabled = 0;
}

void CharacterModel::SetExtraAnimEnabled(u32 flag) {
    if (mParts[0] == NULL) {
        return;
    }
    mExtraAnimEnabled = flag;
    if (!(flag & 0xff)) {
        ModelAnim *anim = mParts[0]->mAnim;
        u8 channel      = GetSet(mParts[0]);
        u32 loop        = anim->mLoop;
        NNSG3dAnmObj *obj;
        s32 frame;
        obj = anim->GetAnimObj(0);
        if (obj == NULL) {
            frame = 0;
        } else {
            frame = obj->mFrame;
        }
        mParts[0]->StartAnimWithIncrement(channel, loop, 0x1000);
        mParts[0]->mAnim->SetFrame(frame, 0);
    } else {
        NNSG3dAnmObj *obj = mParts[0]->mAnim->GetAnimObjForSet(4, mExtraAnimSet);
        mParts[0]->AttachExtraAnim(obj);
    }
}

s32 CharacterModel::GetHeadFrame() {
    NNSG3dAnmObj *obj = mParts[0]->mAnim->GetAnimObj(0);
    if (obj == NULL) {
        return 0;
    }
    return obj->mFrame;
}

void CharacterModel::SetHeadFrame(u32 frame) {
    mParts[0]->mAnim->SetFrame(frame, 0);
}

void InstallHeadNodeMtxCallback(NNSG3dRS *rs) {
    rs->mCallback = StoreHeadNodeMtx;
    rs->mUnk_92   = 3;
}

// Node 0x10 only: gHeadNodeMtx = func_02032cc8's matrix concatenated with func_020323b8()'s.
void StoreHeadNodeMtx(NNSG3dRS *rs) {
    int id;
    if (rs->mFlags & 0x10) {
        id = rs->mUnk_ae;
    } else {
        id = -1;
    }
    if (id != 0x10) {
        return;
    }
    func_02032cc8(&gHeadNodeMtx, NULL);
    MtxFx43 *cam = func_020323b8();
    MTX_Concat43(&gHeadNodeMtx, cam, &gHeadNodeMtx);
}
