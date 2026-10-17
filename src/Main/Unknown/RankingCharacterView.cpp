#include "Scene/RankingCharacterView.hpp"

#include "Game/UnkStruct_020d433c.hpp"
#include "Graphics/Camera.hpp"
#include "Graphics/CellAnimation.hpp"
#include "Graphics/Model.hpp"
#include "Graphics/SpriteManager.hpp"
#include "System/FileSystem.hpp"
#include "Text/TextManager.hpp"

#include <nitro/mi.h>
#include <nitro/os.h>
#include <stdio.h>

extern const u8 gRankingCellPositions[][2];
extern const VecFx32 data_020a2fd8;
extern const VecFx32 data_020a2fe4;
extern const u8 gRankingDigitCellPositions[][2];
extern const u8 data_020a3004[];
extern const s8 gRankingCharacterModelZ[];
extern const s8 gRankingCharacterModelY[];
extern char data_020d441a[];

// Position of a model object (CharacterModel / StoryCharacterModel): the VecFx32 at +4.
struct UnkStruct_ModelPos {
    /* 00 */ PAD(0x00, 0x04);
    /* 04 */ VecFx32 mPosition;
    /* 10 */
};

// Sets the position of a model object; the pointer is the first argument so the three values are computed before it.
static inline void SetModelPos(UnkStruct_ModelPos *model, fx32 x, fx32 y, fx32 z) {
    model->mPosition.x = x;
    model->mPosition.y = y;
    model->mPosition.z = z;
}

RankingCharacterView::RankingCharacterView(u8 unk1, u16 unk2, u32 unk3, u32 unk4) :
    SceneBase(0x3e8, 0) {
    mMyroomBgModel = NULL;
    mCamera = NULL;
    mUnk_4c = NULL;
    mCharacterModel = NULL;
    mStoryCharaNames = NULL;
    mModelX = 0;
    mState = 0;
    mUnk_7e = unk4;
    mChanging = 0;
    mFinishRequested = 0;
    mCharacterId = unk1;
    mRank = unk2;
    mScore = unk3;
    for (u8 i = 0; i < 4; i++) {
        mRankingCellAnims[i] = NULL;
    }
    for (u8 i = 0; i < 10; i++) {
        mDigitCellAnims[i] = NULL;
    }
    mTextBuffer = NULL;
    mGlyphBuffer = NULL;
    mTextBufferSize = 0;
    mGlyphCount = 0;
}

RankingCharacterView::~RankingCharacterView() {
    CameraManager::Get().SetCamerasAndRequestMode(0, NULL, 0);
    ModelInstance *instance = mMyroomBgModel;
    if (instance != NULL) {
        instance->mOwnFlags = 0xf;
        delete instance;
    }
    UnkStruct_0205401c *model = mCharacterModel;
    if (model != NULL) {
        model->Release();
        delete model;
    }
    delete mCamera;
    ModelLightGroup *lights = mUnk_4c;
    if (lights != NULL) {
        delete lights;
    }
    u8 i = 0;
    CellAnimation *cellA;
    do {
        cellA = mRankingCellAnims[i];
        if (cellA != NULL) {
            delete cellA;
        }
        i++;
    } while (i < 4);
    CellAnimation *cellB;
    u8 j = 0;
    do {
        cellB = mDigitCellAnims[j];
        if (cellB != NULL) {
            delete cellB;
        }
        j++;
    } while (j < 10);
    if (mStoryCharaNames != NULL) {
        OS_Free(mStoryCharaNames);
    }
    ClearNameText();
    if (mTextBuffer != NULL) {
        OS_Free(mTextBuffer);
    }
    if (mGlyphBuffer != NULL) {
        OS_Free(mGlyphBuffer);
    }
    MI_CpuClearFast((void *) 0x6000000, 0x80000);
    MI_CpuClearFast((void *) 0x6200000, 0x20000);
    SpriteManager::Get().func_0202740c();
}

void RankingCharacterView::Update() {
    u8 run = 1;
    switch (mState) {
        case 0:
            run = 0;
            Load();
            mState++;
            break;
        case 1:
            if (mFinishRequested != 0) {
                mState = 5;
            } else if (mChanging != 0) {
                mState = 2;
            }
            break;
        case 2:
            if (SlideTo(-0x64000)) {
                mCharacterModel->SetVisible(0);
                ApplyNextCharacter();
                PrintName();
                mState = 3;
            }
            break;
        case 3:
            mState = 4;
            break;
        case 4:
            if (SlideTo(0)) {
                mChanging = 0;
                mState = run;
            }
            break;
        case 5:
            if (!SpriteManager::Get().IsFading()) {
                mState = 6;
            }
            break;
        case 6:
            mFinished = run;
            run       = 0;
            break;
    }
    if (run != 0) {
        Draw();
    }
}

// News an UnkStruct_0205401c for the id: a CharacterModel for 0x15, a StoryCharacterModel otherwise (Model.hpp:54-55);
// not itself a CharacterModel.
void RankingCharacterView::CreateCharacterModel(u32 kind) {
    mCharacterModel = new UnkStruct_0205401c();
    SpriteManager::Get().func_02027504(0);
    mCharacterModel->Create(kind & 0xff, 0, 0, 0);
    mCharacterModel->func_0205461c(0, 1);
    mCharacterModel->SetVisible(1);
    fx32 z = gRankingCharacterModelZ[kind] << 12;
    fx32 y = gRankingCharacterModelY[kind] << 12;
    if (kind == 0x15) {
        SetModelPos((UnkStruct_ModelPos *) mCharacterModel->mCharacter, 0, y, z);
    } else {
        SetModelPos((UnkStruct_ModelPos *) mCharacterModel->mStoryCharacter, 0, y, z);
    }
    u16 *rot = mCharacterModel->GetModelRotation();
    rot[1] += 0x4cd;
}

BOOL RankingCharacterView::ChangeCharacter(u8 unk1, u16 unk2, u32 unk3) {
    if (mChanging != 0) {
        return FALSE;
    }
    mNextCharacterId = unk1;
    mNextRank = unk2;
    mNextScore = unk3;
    mChanging = 1;
    return TRUE;
}

// Also creates the camera and sets CameraManager's active camera and matrices (:191-210), and creates mUnk_4c.
void RankingCharacterView::Load() {
    mCamera = new Camera();
    mCamera->SetUp(0, 0x1000, 0);
    mCamera->SetTarget(0x28000, 0x6e000, 0);
    mCamera->SetClip(0x1000, 0x1388000);
    mCamera->SetFov(0x2aaa);
    mCamera->SetPosition(0x14000, 0x5a000, 0x96000);
    CameraManager::Get().mActive = 0;
    CameraManager::Get().SetCamerasAndRequestMode(1, mCamera, 0);
    CameraManager &manager = CameraManager::Get();
    Camera *camera         = manager.mCameras[manager.mActive];
    data_02211374.mCameraPosition = camera->mPosition;
    data_02211374.mCameraUp       = camera->mUp;
    data_02211374.mCameraTarget   = camera->mTarget;
    func_01ff8d34(&camera->mPosition, &camera->mUp, &camera->mTarget, &data_02211374.mView);
    data_02211374.mFlags &= ~0xe8;
    manager.mProjection[manager.mActive] = data_02211374.mProjection;
    manager.mView[manager.mActive]       = data_02211374.mView;
    manager.mMtxA[manager.mActive]       = *func_02032710();
    manager.mMtxB[manager.mActive]       = *func_02032604();
    manager.mMtxC[manager.mActive]       = *func_020323b8();
    mUnk_4c = new ModelLightGroup(data_020a3004);
    SpriteScreen &screen = SpriteManager::Get().Screen(0);
    screen.SetObjVramMode(1);
    ChangeDir("/Game/Myroom/");
    screen.LoadCharacters("myroom_ranking_u_obj_00.nce_ncg1D.bin", 0, 0);
    screen.LoadPalette("myroom_ranking_u_obj_00_ncl.bin", 0, 0);
    for (u8 i = 0; i < 4; i++) {
        mRankingCellAnims[i]            = new CellAnimation("myroom_ranking_u_obj_00_nce.bin", 0, 1, 1);
        mRankingCellAnims[i]->mCharBase = screen.GetCharBase(0);
        mRankingCellAnims[i]->mPaletteBase   = 0;
    }
    switch (mUnk_7e) {
        case 0:
            mRankingCellAnims[0]->SetAnimation(0);
            break;
        case 1:
            mRankingCellAnims[0]->SetAnimation(1);
            break;
        default:
            mRankingCellAnims[0]->SetAnimation(0);
            break;
    }
    mRankingCellAnims[1]->SetAnimation(2);
    ChangeDir("/Game/2d/union_u/");
    screen.LoadCharacters("union_u_obj_number_32k.nce_ncg1D.bin", 0x5000, 0);
    screen.LoadPalette("union_u_obj_number_32k_ncl.bin", 0xa0, 0);
    for (u8 i = 0; i < 10; i++) {
        mDigitCellAnims[i]            = new CellAnimation("union_u_obj_number_32k_nce.bin", 0, 1, 1);
        mDigitCellAnims[i]->mCharBase = screen.GetCharBase(0x5000);
        mDigitCellAnims[i]->mPaletteBase   = 5;
        mDigitCellAnims[i]->SetAnimation(0);
    }
    ChangeDir("/");
    mStoryCharaNames = (MessageFile *) LoadFile("/font/mes_StoryCharaName.mes", 0, 0, 0, 0, 0);
    TextManager::Get();
    mTextBufferSize = 0x400;
    TextManager::Get();
    mGlyphCount = 4;
    mTextBuffer = OS_Alloc(mTextBufferSize);
    MI_CpuClear16(mTextBuffer, mTextBufferSize);
    mGlyphBuffer = OS_Alloc(mGlyphCount * 0xc);
    TextManager::Get().Init(mGlyphBuffer, 0, 0, 0x10, 2, 0x21);
    TextManager::Get().SetColor(0, 0, 0xf);
    PrintName();
    ChangeDir("/Game/Myroom/Map/");
    NitroModel *model     = new NitroModel;
    NitroTexture *texture = new NitroTexture("myroombg.nsbtx");
    model->Load("myroombg.nsbmd", texture);
    texture->FreeFile();
    mMyroomBgModel = model->CreateInstance(NULL);
    ChangeDir("/");
    CreateCharacterModel(mCharacterId);
    ChangeDir("/");
}

void RankingCharacterView::Draw() {
    func_0207520c();
    MtxFx43 *camera = func_020323b8();
    MtxFx43 mtx     = gIdentityMtx43;
    VecFx32 pos     = data_020a2fd8;
    VecFx32 offset  = data_020a2fe4;
    func_01ffcf64(&mtx, &offset);
    func_01ff89c0(&mtx, camera, &mtx);
    mMyroomBgModel->AdvanceAnim();
    mMyroomBgModel->Draw(&mtx, &pos);
    mCharacterModel->Draw();
}

void RankingCharacterView::func_0207520c() {
    for (u8 i = 0; i < 4; i++) {
        SpriteManager::Get().Screen(0).AddOam(mGlyphBuffer, 1, (s16) (i * 0x20 + 0x6a), 0x36, 0,
                                                     (u16) (i * 8 + 0x300), 0xf, 0);
    }
    u32 high = mRank;
    u32 low  = mScore;
    for (u8 i = 0; i < 10; i++) {
        CellAnimation *cell = mDigitCellAnims[i];
        if (cell == NULL) {
            continue;
        }
        u32 special = 0;
        u8 digit    = 0;
        if (i < 3) {
            if (high != 0) {
                digit = high % 10;
                high /= 10;
            } else if (i != 0) {
                continue;
            }
        } else if (i < 10) {
            if (low != 0) {
                digit = low % 10;
                low /= 10;
            } else if (i != 3) {
                continue;
            }
        } else if (mCharacterId == 0x15) {
            special = 1;
        }
        if (special != 0) {
            cell->mAnimIndex = 2;
            cell->ResetAnimation();
            mDigitCellAnims[i]->SetMode(5);
            mDigitCellAnims[i]->Update();
            mDigitCellAnims[i]->Draw(0, gRankingDigitCellPositions[i][0], gRankingDigitCellPositions[i][1], 0);
        } else {
            cell->mAnimIndex = 0;
            cell->ResetAnimation();
            mDigitCellAnims[i]->SetMode(5);
            mDigitCellAnims[i]->Update();
            mDigitCellAnims[i]->DrawFrame(0, digit, gRankingDigitCellPositions[i][0], gRankingDigitCellPositions[i][1], 0);
        }
    }
    for (u8 i = 0; i < 2; i++) {
        if (mRankingCellAnims[i] != NULL) {
            mRankingCellAnims[i]->Update();
            mRankingCellAnims[i]->Draw(0, gRankingCellPositions[i][0], gRankingCellPositions[i][1], 0);
        }
    }
}

void RankingCharacterView::PrintName() {
    MI_CpuClear16(mTextBuffer, mTextBufferSize);
    TextManager::Get().mUse4bppFont = 1;
    if (mCharacterId == 0x15) {
        s16 width  = 0;
        s16 height = 0;
        TextManager::Get().Print(data_020d441a, 0x18, mTextBuffer, 0x10, 2, &width, &height, 0, 0, 2, 0x21, 0, 0, 0, 0, 1, 1);
    } else {
        TextManager &text = TextManager::Get();
        s16 height;
        s16 width;
        width  = 0;
        height = 0;
        u32 kind          = mCharacterId;
        MessageFile *file = mStoryCharaNames;
        text.Print(file->GetText(kind), file->GetLength(kind), mTextBuffer, 0x10, 2, &height, &width, 0, 0, 2, 0x21, 0, 0, 0, 0, 1, 1);
    }
    TextManager::Get().mUse4bppFont = 0;
    QueueUpload(SpriteManager::Get().Screen(0).mBgIndex, 4, 0x6000, mTextBuffer, mTextBufferSize, 0);
}

void RankingCharacterView::ClearNameText() {
    MI_CpuClear16(mTextBuffer, mTextBufferSize);
}

void RankingCharacterView::ApplyNextCharacter() {
    UnkStruct_0205401c *old = mCharacterModel;
    if (old != NULL) {
        old->Release();
        delete old;
    }
    mCharacterModel = new UnkStruct_0205401c();
    mCharacterId = mNextCharacterId;
    mUnk_59 = mUnk_61;
    mRank = mNextRank;
    mScore = mNextScore;
    u32 kind = mCharacterId;
    SpriteManager::Get().func_0202753c(0);
    mCharacterModel->Create(kind & 0xff, 0, 0, 0);
    mCharacterModel->func_0205461c(0, 1);
    mCharacterModel->SetVisible(1);
    u16 *rot = mCharacterModel->GetModelRotation();
    rot[1] += 0x4cd;
    SlideTo(0);
}

BOOL RankingCharacterView::SlideTo(s32 target) {
    if (mModelX == target) {
        return TRUE;
    }
    if (mModelX > target) {
        mModelX -= 0x14000;
        if (mModelX < target) {
            mModelX = target;
        }
    } else {
        mModelX += 0x14000;
        if (mModelX > target) {
            mModelX = target;
        }
    }
    u8 kind = mCharacterId;
    SetModelPos(mCharacterId == 0x15 ? (UnkStruct_ModelPos *) mCharacterModel->mCharacter : (UnkStruct_ModelPos *) mCharacterModel->mStoryCharacter, mModelX,
                gRankingCharacterModelY[kind] << 12, gRankingCharacterModelZ[kind] << 12);
    return FALSE;
}
