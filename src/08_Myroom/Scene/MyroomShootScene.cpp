#include "Game/GameData.hpp"
#include "Game/SaveData.hpp"
#include "Graphics/CellAnimation.hpp"
#include "Graphics/Display.hpp"
#include "Graphics/Effect.hpp"
#include "Graphics/Model.hpp"
#include "System/FileSystem.hpp"
#include "UI/MessageWindow.hpp"
#include "UI/SelectWindow.hpp"

#include <nitro/gx.h>
#include <nitro/mi.h>
#include <nitro/os.h>
#include <stdio.h>

struct ShootPoint {
    u8 x;
    u8 y;
};

static const WindowCellRecord sMyroomShootCellsFromItem6[4] = {
    {"Game/Myroom/myroom_s_shoot_b_obj_nce.bin", 0, 0x0, 0, 0, 1, {0, 0, 0, 0}},
    {"Game/2D/union_b/union_b_obj_cockpit_nce.bin", 2, 0x5000, 10, 0, 1, {2, 3, 3, 0}},
    {"Game/2D/union_b/union_b_obj_page_nce.bin", 0, 0x8000, 11, 0, 1, {0, 1, 1, 0}},
    {"Game/2D/union_b/union_b_obj_page_nce.bin", 3, 0x8000, 11, 0, 1, {3, 4, 4, 0}},
};
static const ShootPoint sMyroomShootGridPos[2][3] = {
    {{0x28, 0x28}, {0x68, 0x28}, {0xa8, 0x28}},
    {{0x28, 0x68}, {0x68, 0x68}, {0xa8, 0x68}},
};
static const VecFx32 sMyroomShootShooterPos[6] = {
    {0xf000, 0x5c000, 0x64000}, {0xf000, 0x5f000, 0x64000}, {0xf000, 0x5f000, 0x64000},
    {0xf000, 0x5f000, 0x64000}, {0xf000, 0x5f000, 0x64000}, {0xf000, 0x5f000, 0x64000},
};
static const VecFx32 sMyroomShootRoomScale = {0x1000, 0x1000, 0x1000};
static const VecFx32 sMyroomShootRoomOffset = {0, 0, -0xe6000};
static const u32 sMyroomShootTextUnitSizes[2] = {0x44, 0x21};
static const u16 sMyroomShootEffectRotDegrees[6][3] = {{0, 15, 15}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}};
static const u8 sMyroomShootGridUsed[2][3] = {{1, 1, 1}, {1, 1, 1}}; // all 1: the == 0 skips never run in this build
// Init rewrites the animations from sMyroomShootShooterAnimIndices (1 for a locked shooter) before SetCells.
static WindowCellRecord sMyroomShootShooterCells[6] = {
    {"Game/Myroom/myroom_s_shoot_b_obj_nce.bin", 2, 0x0, 0, 1, 1, {2, 2, 2, 0}},
    {"Game/Myroom/myroom_s_shoot_b_obj_nce.bin", 3, 0x0, 0, 1, 1, {3, 3, 3, 0}},
    {"Game/Myroom/myroom_s_shoot_b_obj_nce.bin", 4, 0x0, 0, 1, 1, {4, 4, 4, 0}},
    {"Game/Myroom/myroom_s_shoot_b_obj_nce.bin", 5, 0x0, 0, 1, 1, {5, 5, 5, 0}},
    {"Game/Myroom/myroom_s_shoot_b_obj_nce.bin", 6, 0x0, 0, 1, 1, {6, 6, 6, 0}},
    {"Game/Myroom/myroom_s_shoot_b_obj_nce.bin", 7, 0x0, 0, 1, 1, {7, 7, 7, 0}},
};
static const u8 sMyroomShootTextDimensions[2][2] = {{0x10, 0x12}, {0x40, 0x02}};
static const WindowItemRecord sMyroomShootWindowItems[10] = {
    {0, WINDOW_ITEM_NO_SLIDE_OUT, 40, 40, 48, 48, 0, {-1, -1, -1, -1}, 0, WINDOW_TOUCH_RELEASE},
    {1, WINDOW_ITEM_NO_SLIDE_OUT, 104, 40, 48, 48, 0, {-1, -1, -1, -1}, 0, WINDOW_TOUCH_RELEASE},
    {2, WINDOW_ITEM_NO_SLIDE_OUT, 168, 40, 48, 48, 0, {-1, -1, -1, -1}, 0, WINDOW_TOUCH_RELEASE},
    {3, WINDOW_ITEM_NO_SLIDE_OUT, 40, 104, 48, 48, 0, {-1, -1, -1, -1}, 0, WINDOW_TOUCH_RELEASE},
    {4, WINDOW_ITEM_NO_SLIDE_OUT, 104, 104, 48, 48, 0, {-1, -1, -1, -1}, 0, WINDOW_TOUCH_RELEASE},
    {5, WINDOW_ITEM_NO_SLIDE_OUT, 168, 104, 48, 48, 0, {-1, -1, -1, -1}, 0, WINDOW_TOUCH_RELEASE},
    {6, WINDOW_ITEM_CURSOR_SKIP, 40, 40, 0, 0, 0, {-1, -1, -1, -1}, 0, 0},
    {7, 0x2, 225, 170, 30, 20, 0, {-1, -1, -1, -1}, 0, WINDOW_TOUCH_RELEASE},
    {8, 0x208, 192, 20, 32, 16, 0, {-1, -1, -1, -1}, 0, WINDOW_TOUCH_RELEASE},
    {9, 0x408, 224, 20, 32, 16, 0, {-1, -1, -1, -1}, 0, WINDOW_TOUCH_RELEASE},
};

#include "System/InputManager.hpp"
#include "System/SoundManager.hpp"
#include "Graphics/SpriteManager.hpp"
#include "Scene/MyroomShootScene.hpp"
#include "Text/TextManager.hpp"

static const ShootPoint sMyroomShootCellAnimPos = {0x80, 0x60};
static const u16 sMyroomShootLightData[20] = {1, 0x7fff, 0xfbae, 0xf91f, 0xf23d};
static const u32 sMyroomShootBakuganKinds[6] = {0, 4, 7, 2, 11, 25};
static const u8 sMyroomShootShooterAnimIndices[6][2] = {{2, 2}, {3, 3}, {4, 4}, {5, 5}, {6, 6}, {7, 7}};
static const u16 sMyroomShootEffectScriptIndices[6] = {0, 2, 11, 9, 7, 8};

MyroomShootScene::MyroomShootScene() :
    SceneBase(1000, 0) {
    mCamera      = NULL;
    mLights      = NULL;
    mShooter     = NULL;
    mRoom        = NULL;
    mMessages    = NULL;
    mMessageWindow     = NULL;
    mWindow      = NULL;
    mResult      = 0;
    mEffect      = 0;
    mRot[0]      = 0;
    mRot[1]      = 0;
    mRot[2]      = 0;
    mTextX      = 0x100;
    mTextEndY      = 0;
    mShooterTimer      = 0;
    mShooterIndex    = -1;
    mCol      = 0;
    mRow      = 0;
    mTextPage      = 0;
    mState       = 0;
    mTimer       = 0;
    mCellAnim    = NULL;
    for (u8 i = 0; i < 2; i++) {
        mTextBuffers[i]    = NULL;
        mGlyphBuffers[i]   = NULL;
        mTextBufferSize[i] = 0;
        mGlyphCount[i]     = 0;
    }
    InitUnlocked();
    gGameData.mEffects = NULL;
}

MyroomShootScene::~MyroomShootScene() {
    CameraManager::Get().SetCamerasAndRequestMode(0, NULL, 0);
    ModelInstance *room = mRoom;
    if (room != NULL) {
        room->mOwnFlags = MODEL_OWN_ALL;
        delete room;
    }
    BakuganModel *shooter = mShooter;
    if (shooter != NULL) {
        delete shooter;
    }
    gGameData.mEffects->FreeAllEffects();
    FreeEffectGroup(gGameData.mEffects, 0);
    delete mWindow;
    delete mCamera;
    ModelLightGroup *lights = mLights;
    if (lights != NULL) {
        delete lights;
    }
    CellAnimation *cell = mCellAnim;
    if (cell != NULL) {
        delete cell;
    }
    if (mMessages != NULL) {
        OS_Free(mMessages);
    }
    ClearTextBuffers(0);
    ClearTextBuffers(1);
    for (u8 i = 0; i < 2; i++) {
        OS_Free(mTextBuffers[i]);
        if (mGlyphBuffers[i] != NULL) {
            OS_Free(mGlyphBuffers[i]);
        }
    }
    delete mMessageWindow;
    if (gGameData.mEffects != NULL) {
        gGameData.mEffects->mFinished = 1;
        gGameData.mEffects            = NULL;
    }
    MI_CpuClearFast((void *) 0x6000000, 0x80000);
    MI_CpuClearFast((void *) 0x6200000, 0x20000);
    SpriteManager::Get().ClearSlotUsed(0);
    SpriteManager::Get().ResetSlots();
}

void MyroomShootScene::Update() {
    BOOL draw = TRUE;
    switch (mState) {
        case 0:
            draw = FALSE;
            InitDisplay();
            Init();
            mTimer = 10;
            mState++;
            break;
        case 1:
            draw = FALSE;
            mTimer--;
            if (mTimer > 0) {
                break;
            }
            SpriteManager::Get().StartFade(FADE_MODE_TO_ZERO);
            mState++;
            break;
        case 2:
            func_ov008_02223264();
            break;
        case 3:
            if (!SpriteManager::Get().IsFading()) {
                mState = 4;
            }
            break;
        case 4:
            draw = FALSE;
            RequestScene(1);
            mFinished = TRUE;
            break;
    }
    if (draw) {
        mResult = mWindow->Update(FALSE);
        Draw();
    }
}

void MyroomShootScene::InitDisplay() {
    MI_CpuClearFast((void *) 0x6000000, 0x80000);
    MI_CpuClearFast((void *) 0x6200000, 0x20000);
    GX_SetDispSelect(1);
    SpriteManager::Get().SetupMode(0, 7);
    GX_SetGraphicsMode(1, 0, 1);
    GXS_SetGraphicsMode(0);
    G2S_SetBG0Control(0, 1, 0, 1, 0);
    G2S_SetBG0Offset(0, 0);
    G2S_SetBG1Offset(0, 0);
    G2S_SetBG2Offset(0, 0);
    G2S_SetBG3Offset(0, 0);
    G2S_SetBG0Priority(3);
    G2S_SetBG1Priority(3);
    G2S_SetBG2Priority(3);
    G2S_SetBG3Priority(3);
    GX_SetVisiblePlane(0x1f);
    GXS_SetVisiblePlane(0x11);
    G2_SetBlendBrightness(0x1f, 0);
    G2S_SetBlendBrightness(0x1f, 0);
    GXS_SetVisibleWnd(0);
    G2S_SetWndOutsidePlane(0x1f, FALSE);
    G2S_SetWnd0InsidePlane(0x1f, FALSE);
    G2S_SetWnd1InsidePlane(0x1f, FALSE);
    G2S_SetWndOBJInsidePlane(0x1f, FALSE);
    G2S_SetWnd0Position(0, 0, 0, 0);
    G2S_SetWnd1Position(0, 0, 0, 0);
    G2_BlendNone();
    G2S_BlendNone();
}

void MyroomShootScene::Init() {
    mCamera = new Camera();
    mCamera->SetUp(0, 0x1000, 0);
    mCamera->SetTarget(0x28000, 0x6e000, 0);
    mCamera->SetClip(0x1000, 0x1388000);
    mCamera->SetFov(0x2aaa);
    mCamera->SetPosition(0x14000, 0x5a000, 0x96000);
    CameraManager::Get().mCameraIndex = 0;
    CameraManager::Get().SetCamerasAndRequestMode(1, mCamera, 0);
    CameraManager &manager = CameraManager::Get();
    Camera *camera    = manager.mCameras[manager.mCameraIndex];
    data_02211374.mCameraPosition = camera->mPosition;
    data_02211374.mCameraUp       = camera->mUp;
    data_02211374.mCameraTarget   = camera->mTarget;
    MTX_LookAt(&camera->mPosition, &camera->mUp, &camera->mTarget, &data_02211374.mView);
    data_02211374.mFlags &= ~0xe8;
    manager.mProjection[manager.mCameraIndex] = data_02211374.mProjection;
    manager.mView[manager.mCameraIndex]       = data_02211374.mView;
    manager.mMtxA[manager.mCameraIndex]       = *func_02032710();
    manager.mMtxB[manager.mCameraIndex]       = *func_02032604();
    manager.mMtxC[manager.mCameraIndex]       = *func_020323b8();
    mLights = new ModelLightGroup(sMyroomShootLightData);
    ChangeDir("/Game/Myroom/");
    LoadAndQueueUpload(1, UPLOAD_KIND_BG0_SCR, 0, "myroom_b_bg_00_nsc.bin", 0, 0, 0);
    LoadAndQueueUpload(1, UPLOAD_KIND_BG0_CHAR, 0, "myroom_b_bg_00_ncg.bin", 0, 0, 0);
    LoadAndQueueUpload(1, UPLOAD_KIND_BG_EXT_PLTT, 0, "myroom_b_bg_00_ncl.bin", 0, 0, 0);
    SpriteScreen &main = SpriteManager::Get().Screen(0);
    main.SetObjVramMode(3);
    main.LoadCharacters("myroom_s_shoot_u_obj_00.nce_ncg1D.bin", 0x4000, 0);
    main.LoadPalette("myroom_s_shoot_u_obj_00_ncl.bin", 0, 0);
    mCellAnim            = new CellAnimation("myroom_s_shoot_u_obj_00_nce.bin", 0, 1, 1);
    mCellAnim->mCharBase = main.GetCharBase(0x4000);
    mCellAnim->mPaletteBase   = 0;
    mCellAnim->SetAnimation(0);
    ChangeDir("/");
    mWindow = NULL;
    mWindow = new SelectWindow(10, sMyroomShootWindowItems, -1);
    SpriteScreen &sub = SpriteManager::Get().Screen(1);
    sub.SetObjVramMode(3);
    ChangeDir("/");
    sub.LoadCharacters("Game/Myroom/myroom_s_shoot_b_obj.nce_ncg1D.bin", 0, 0);
    sub.LoadPalette("Game/Myroom/myroom_s_shoot_b_obj_ncl.bin", 0, 0);
    sub.LoadCharacters("Game/2D/union_b/union_b_obj_cockpit.nce_ncg1D.bin", 0x5000, 0);
    sub.LoadPalette("Game/2D/union_b/union_b_obj_cockpit_ncl.bin", 0x140, 0);
    sub.LoadCharacters("Game/2D/union_b/union_b_obj_page.nce_ncg1D.bin", 0x8000, 0);
    sub.LoadPalette("Game/2D/union_b/union_b_obj_page_ncl.bin", 0x160, 0);
    for (u8 i = 0; i < 6; i++) {
        if (mUnlocked[i] != 0) {
            u8 a = sMyroomShootShooterAnimIndices[i][0];
            u8 b = sMyroomShootShooterAnimIndices[i][1];
            *(u8 *) &sMyroomShootShooterCells[i].mAnimIndex = a;
            sMyroomShootShooterCells[i].mAnimIndices[0] = a;
            sMyroomShootShooterCells[i].mAnimIndices[1] = b;
            sMyroomShootShooterCells[i].mAnimIndices[2] = a;
        } else {
            *(u8 *) &sMyroomShootShooterCells[i].mAnimIndex = 1;
            sMyroomShootShooterCells[i].mAnimIndices[0] = 1;
            sMyroomShootShooterCells[i].mAnimIndices[1] = 1;
            sMyroomShootShooterCells[i].mAnimIndices[2] = 1;
        }
    }
    mWindow->SetCells(6, sMyroomShootShooterCells);
    mWindow->SetCells(4, sMyroomShootCellsFromItem6);
    ChangeDir("/Game/Myroom/Map/");
    NitroModel *model     = new NitroModel();
    NitroTexture *texture = new NitroTexture("myroom_s.nsbtx");
    model->Load("myroom_s.nsbmd", texture);
    texture->FreeFile();
    mRoom = model->CreateInstance(0);
    gGameData.mEffects = new EffectManager();
    LoadEffectGroup(gGameData.mEffects, 0, "shoot");
    SpriteManager::Get().SaveSlot(0);
    SelectShooter(0);
    ChangeDir("/font/");
    if (mMessages == NULL) {
        mMessages = (MessageFile *) LoadFile("mes_myroom.mes", 0, 0, 0, 0, 0);
    }
    for (u8 i = 0; i < 2; i++) {
        mTextBufferSize[i] = (u16) TextManager::Get().BufferSize((s16) sMyroomShootTextDimensions[i][0], (s16) sMyroomShootTextDimensions[i][1]);
        mGlyphCount[i]     = TextManager::Get().GlyphCount(sMyroomShootTextDimensions[i][0], sMyroomShootTextDimensions[i][1], sMyroomShootTextUnitSizes[i]);
        mTextBuffers[i]    = OS_Alloc(mTextBufferSize[i]);
        ClearTextBuffers(i);
        mGlyphBuffers[i] = OS_Alloc(mGlyphCount[i] * 0xc);
        TextManager::Get().Init(mGlyphBuffers[i], 0, 0, sMyroomShootTextDimensions[i][0], sMyroomShootTextDimensions[i][1], sMyroomShootTextUnitSizes[i]);
    }
    TextManager::Get().SetColor(0, 0, 0xf);
    mMessageWindow = new MessageWindow("mes_myroom", 1, 0x7000);
    mMessageWindow->func_02065bd4(0x14, 4, 0);
    mMessageWindow->SetWidth(0xd8);
    mMessageWindow->StartMessage(0x23, 0, 1);
    PrintShooterText();
    ChangeDir("/");
}

void MyroomShootScene::SelectShooter(s8 index) {
    if (mShooterIndex != index) {
        mShooterIndex                   = index;
        BakuganModel *shooter = mShooter;
        if (shooter != NULL) {
            delete shooter;
        }
        mShooter = NULL;
        if (mEffect != 0) {
            FreeEffect(gGameData.mEffects, mEffect);
            mEffect = 0;
        }
        if (mUnlocked[mShooterIndex] != 0) {
            u16 modelRot[3];
            u16 effectRot[3];
            VecFx32 pos;
            mShooter = new BakuganModel();
            mShooter->Setup(sMyroomShootBakuganKinds[mShooterIndex], (u8) mShooterIndex, BAKUGAN_MODEL_MODE_H);
            pos = sMyroomShootShooterPos[mShooterIndex];
            mShooter->SetPositionOutOfLine(&pos);
            mRot[0]  = 0;
            mRot[1]  = 0x154;
            mRot[2]  = 0;
            modelRot[0] = (u16) (65536.0f * (float) mRot[0] / 360.0f);
            modelRot[1] = (u16) (65536.0f * (float) mRot[1] / 360.0f);
            modelRot[2] = (u16) (65536.0f * (float) mRot[2] / 360.0f);
            mShooter->SetRotationOutOfLine(modelRot);
            mEffect = StartEffect(gGameData.mEffects, 0, (u8) sMyroomShootEffectScriptIndices[mShooterIndex], 0, 0);
            SetEffectPosition(gGameData.mEffects, mEffect, &pos);
            effectRot[0] = (u16) (65536.0f * (float) sMyroomShootEffectRotDegrees[mShooterIndex][0] / 360.0f);
            effectRot[1] = (u16) (65536.0f * (float) sMyroomShootEffectRotDegrees[mShooterIndex][1] / 360.0f);
            effectRot[2] = (u16) (65536.0f * (float) sMyroomShootEffectRotDegrees[mShooterIndex][2] / 360.0f);
            SetEffectRotation(gGameData.mEffects, mEffect, effectRot);
        }
        mTextPage = 0;
        mTextX = 0x100;
    }
    mPendingSelection = -1;
}

void MyroomShootScene::InitUnlocked() {
    MI_CpuFill8(mUnlocked, 0, sizeof(mUnlocked));
    for (u8 i = 0; i < 6; i++) {
        if (TestFlagBit(data_020d49b0, i)) {
            mUnlocked[i] = 1;
        }
    }
}

void MyroomShootScene::Draw() {
    DrawSprites();
    DrawWindows();
}

void MyroomShootScene::DrawSprites() {
    if (mCellAnim != NULL) {
        mCellAnim->Update();
        mCellAnim->Draw(0, sMyroomShootCellAnimPos.x, sMyroomShootCellAnimPos.y, 0);
    }
    MtxFx43 *base = func_020323b8();
    MtxFx43 view  = gIdentityMtx43;
    VecFx32 scale = sMyroomShootRoomScale;
    VecFx32 offset = sMyroomShootRoomOffset;
    MTX_SetTranslation43(&view, &offset);
    MTX_Concat43(&view, base, &view);
    mRoom->AdvanceAnim();
    mRoom->Draw(&view, &scale);
    mShooterTimer--;
    if (mShooterTimer <= 0) {
        mShooterTimer = 0;
        if (mShooter != NULL) {
            mShooter->Update();
            mShooter->DrawModel();
        }
    }
    if (mUnlocked[mShooterIndex] == 0) {
        return;
    }
    SpriteManager::Get().Screen(0).AddOam(mGlyphBuffers[0], 4, 0x70, 0x1e, 0, 0, 0xf, 0);
    u8 first;
    u8 k;
    s16 x;
    u8 last;
    x = 0x80 - ((u32) mTextWidth >> 1);
    if ((s32) mTextWidth > 0xf0) {
        x       = mTextX;
        mTextX = x - 1;
        if (mTextX <= -mTextWidth) {
            mTextX = 0x100;
        }
    }
    first = 0;
    if (x < 0) {
        first = (-x) / 32;
    }
    last = first + 9 - x / 32;
    if (last > 0x10) {
        last = 0x10;
    }
    k = first;
    do {
        SpriteManager::Get().Screen(0).AddOam((u8 *) mGlyphBuffers[1] + k * 0xc, 1, x, 8, 0, 0x60, 0xf, 0);
        k++;
    } while (k < last);
}

void MyroomShootScene::DrawWindows() {
    mMessageWindow->Update();
    mWindow->Draw();
}

void MyroomShootScene::ClearTextBuffers(u8 index) {
    MI_CpuClear16(mTextBuffers[index], mTextBufferSize[index]);
}

void MyroomShootScene::PrintShooterText() {
    ClearTextBuffers(0);
    TextManager &text = TextManager::Get();
    s16 width;
    s16 height;
    height            = 2 - mTextPage * 0x7e;
    width             = 0x10;
    u16 index         = mShooterIndex + 0x16;
    MessageFile *file = mMessages;
    text.Print(file->GetText(index), file->GetLength(index), mTextBuffers[0], 0x10, 0x12, &width, &height, 0, 0, 2, 0x44,
               0xe, 0, 0, 0, 1, 1);
    if (&mTextEndY != NULL) {
        mTextEndY = height;
    }
    QueueUpload(SpriteManager::Get().Screen(0).mBgIndex, UPLOAD_KIND_OBJ, 0, mTextBuffers[0], mTextBufferSize[0], 0);
    ClearTextBuffers(1);
    TextManager::Get().mUse4bppFont = 1;
    TextManager &text2         = TextManager::Get();
    s16 height2;
    s16 width2;
    width2     = 0;
    height2    = 0;
    u16 index2 = mShooterIndex + 0x1c;
    file       = mMessages;
    text2.Print(file->GetText(index2), file->GetLength(index2), mTextBuffers[1], 0x40, 2, &height2, &width2, &mTextWidth, 0, 2,
                0x21, 0xe, 0, 0, 0, 1, 1);
    TextManager::Get().mUse4bppFont = 0;
    QueueUpload(SpriteManager::Get().Screen(0).mBgIndex, UPLOAD_KIND_OBJ, 0x3000, mTextBuffers[1], mTextBufferSize[1], 0);
    if (mUnlocked[mShooterIndex] == 0) {
        mTextEndY = 0;
    }
}

s32 MyroomShootScene::UpdateInput() {
    s32 result = -1;
    s8 previous = mTextPage;
    if (mResult == 3) {
        s32 id = mWindow->GetSelectedId();
        switch (id) {
            case 0:
            case 1:
            case 2:
            case 3:
            case 4:
            case 5:
                result  = (s8) id;
                mCol = result % 3;
                mRow = result / 3;
                if (mShooterIndex != (s8) id) {
                    SoundManager::Get().StartSeqArc(0);
                }
                break;
            case 8:
                if (previous > 0) {
                    mTextPage--;
                    SoundManager::Get().StartSeqArc(0);
                }
                break;
            case 9:
                if (mTextEndY > 0x80) {
                    mTextPage++;
                    SoundManager::Get().StartSeqArc(0);
                }
                break;
        }
    }
    if (previous != mTextPage) {
        PrintShooterText();
    } else if (result == -1) {
        result = UpdateKeySelection();
    }
    return result;
}

s8 MyroomShootScene::UpdateKeySelection() {
    s8 result  = -1;
    BOOL moved = FALSE;
    if (InputManager::Get().IsTrigger(KEY_UP)) {
        moved = TRUE;
        if (mRow != 0) {
            mRow--;
        } else {
            mRow = 1;
            if (sMyroomShootGridUsed[mRow][mCol] == 0) {
                mRow--;
            }
        }
    } else if (InputManager::Get().IsTrigger(KEY_DOWN)) {
        moved = TRUE;
        mRow++;
        if (mRow >= 2) {
            mRow = 0;
        } else if (sMyroomShootGridUsed[mRow][mCol] == 0) {
            mRow = 0;
        }
    }
    if (InputManager::Get().IsTrigger(KEY_LEFT)) {
        moved = TRUE;
        if (mCol != 0) {
            mCol--;
        } else {
            mCol = 2;
            if (sMyroomShootGridUsed[mRow][mCol] == 0) {
                mCol--;
            }
        }
    } else if (InputManager::Get().IsTrigger(KEY_RIGHT)) {
        moved = TRUE;
        mCol++;
        if (mCol >= 3) {
            mCol = 0;
        } else if (sMyroomShootGridUsed[mRow][mCol] == 0) {
            mCol = 0;
        }
    }
    if (moved) {
        result = mCol + mRow * 3;
        SoundManager::Get().StartSeqArc(0);
    }
    return result;
}

void MyroomShootScene::func_ov008_02223264() {
    if (mResult == 2) {
        SoundManager::Get().StartSeqArc(1);
        SpriteManager::Get().StartFade(FADE_MODE_DOWN);
        mState = 3;
    } else if (mPendingSelection != -1) {
        SpriteManager::Get().RestoreSlot(0);
        SelectShooter(mPendingSelection);
        PrintShooterText();
        if (mWindow->mState != 1) {
            mWindow->mState = 4;
            mWindow->Reset(-1);
        }
    } else {
        s32 selection = UpdateInput();
        if (selection >= 0) {
            mPendingSelection = selection;
            mShooterTimer = 3;
        }
    }
    WindowItem *items = mWindow->mCursor->mItems;
    if (mTextPage != 0) {
        if (mWindow->mCellAnims[8]->mAnimIndex == 2) {
            mWindow->mCellAnims[8]->SetAnimation(0);
            items[8].mFlags = 0x208;
        }
    } else {
        if (mWindow->mCellAnims[8]->mAnimIndex != 2) {
            mWindow->mCellAnims[8]->SetAnimation(2);
            items[8].mFlags = WINDOW_ITEM_CURSOR_SKIP;
        }
    }
    if (mTextEndY > 0x80) {
        if (mWindow->mCellAnims[9]->mAnimIndex == 5) {
            mWindow->mCellAnims[9]->SetAnimation(3);
            items[9].mFlags = 0x408;
        }
    } else {
        if (mWindow->mCellAnims[9]->mAnimIndex != 5) {
            mWindow->mCellAnims[9]->SetAnimation(5);
            items[9].mFlags = WINDOW_ITEM_CURSOR_SKIP;
        }
    }
    Vec2s pos = {sMyroomShootGridPos[mRow][mCol].x, sMyroomShootGridPos[mRow][mCol].y};
    mWindow->mCellPos[6] = pos;
    if (mShooter == NULL) {
        return;
    }
    mRot[0] += 0x14;
    mRot[2] += 0x65;
    u16 rot[3];
    rot[0] = (u16) (65536.0f * (float) mRot[0] / 360.0f);
    rot[1] = (u16) (65536.0f * (float) mRot[1] / 360.0f);
    rot[2] = (u16) (65536.0f * (float) mRot[2] / 360.0f);
    mShooter->SetRotationOutOfLine(rot);
}
