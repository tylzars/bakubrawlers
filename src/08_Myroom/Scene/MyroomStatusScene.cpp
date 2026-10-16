#include "Scene/MyroomStatusScene.hpp"
#include "Game/UnkStruct_020d43f0.hpp"
#include "Graphics/Camera.hpp"
#include "Graphics/CellAnimation.hpp"
#include "Graphics/Display.hpp"
#include "Graphics/Model.hpp"
#include "Graphics/SpriteManager.hpp"
#include "System/FileSystem.hpp"
#include "Text/TextManager.hpp"
#include "Game/Ranking.hpp"

#include <nitro/mi.h>
#include <nitro/os.h>

extern const u16 data_ov008_02228660[];
extern const VecFx32 data_ov008_02228634;
extern const VecFx32 data_ov008_02228640;

extern const u8 data_ov008_02228688[5][3];
extern const u32 data_ov008_0222864c[5];

MyroomStatusScene::MyroomStatusScene(u32 unk) :
    SceneBase(2, 0) {
    mUnk_14 = NULL;
    mCamera = NULL;
    mReady  = 0;
    mState  = 0;
    mUnk_4c = unk;
    mUnk_50 = NULL;
    mCellAnim = NULL;
    for (u8 i = 0; i < 5; i++) {
        mTextBuffers[i] = NULL;
    }
    mMessages = NULL;
    mGlyphBuffer = NULL;
    mTextBufferSize = 0;
    mGlyphCount = 0;
    mPalette = new UnkStruct_02029ca8(data_ov008_02228660);
    mUnk_50 = new Ranking();
    u32 frames = data_020d43f0[0].mPlayFrames;
    mHours     = frames / 216000;
    frames -= mHours * 216000;
    mMinutes = frames / 3600;
    frames -= mMinutes * 3600;
    mSeconds = frames / 60;
}

MyroomStatusScene::~MyroomStatusScene() {
    CameraManager::Get().func_02026504(0, NULL, 0);
    ModelInstance *p14 = mUnk_14;
    if (p14 != NULL) {
        p14->mUnk_68 = 0xf;
        delete p14;
        mUnk_14 = NULL;
    }
    UnkStruct_0205401c *model = mModel;
    if (model != NULL) {
        model->func_02054034();
        delete model;
    }
    delete mCamera;
    UnkStruct_02029ca8 *palette = mPalette;
    if (palette != NULL) {
        delete palette;
    }
    CellAnimation *cell = mCellAnim;
    if (cell != NULL) {
        delete cell;
    }
    if (mUnk_50 != NULL) {
        delete mUnk_50;
    }
    FreeText();
    MI_CpuClearFast((void *) 0x6000000, 0x80000);
}

void MyroomStatusScene::Update() {
    BOOL draw = TRUE;
    switch (mState) {
        case 0:
            draw = FALSE;
            InitDisplay();
            Setup3D();
            mState++;
            break;
        case 1:
            draw = FALSE;
            for (u8 i = 0; i < 5; i++) {
                RenderLine(i);
            }
            mState++;
            break;
        case 2:
            if (mReady != 0) {
                mState = 3;
            }
            break;
        case 3:
            RequestScene(1);
            mFinished = TRUE;
            break;
    }
    if (draw) {
        UpdateTime();
        Draw3D();
    }
}

void MyroomStatusScene::SetReady() {
    mReady = 1;
}

void MyroomStatusScene::InitDisplay() {
    MI_CpuClearFast((void *) 0x6000000, 0x80000);
    SpriteManager::Get().func_02026dec(0, 7);
    GX_SetGraphicsMode(1, 0, 1);
    GXS_SetGraphicsMode(0);
}

void MyroomStatusScene::CreateModel() {
    mModel = new UnkStruct_0205401c();
    mModel->func_0205411c(0x15, 0, 0, 0);
    mModel->func_0205461c(0, 1);
    mModel->mUnk_04->mUnk_64 = 1;
    mModel->mUnk_04->mUnk_12 += 0x4cd;
}

void MyroomStatusScene::Setup3D() {
    CreateModel();
    mCamera = new Camera();
    mCamera->SetUp(0, 0x1000, 0);
    mCamera->SetTarget(0x2b000, 0x6e000, 0);
    mCamera->SetClip(0x1000, 0x1388000);
    mCamera->func_02026004(0x2aaa);
    mCamera->SetPosition(0x19000, 0x5a000, 0x96000);
    CameraManager::Get().mActive = 0;
    CameraManager::Get().func_02026504(1, mCamera, 0);
    CameraManager &manager = CameraManager::Get();
    Camera *camera    = manager.mCameras[manager.mActive];
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
    ChangeDir("/Game/Myroom/Map/");
    NitroTexture *texture = new NitroTexture("myroombg.nsbtx");
    NitroModel *model     = new NitroModel("myroombg.nsbmd", texture);
    texture->func_02028344();
    mUnk_14 = model->func_02028b78(0);
    SpriteScreen &screen = SpriteManager::Get().Screen(0);
    screen.func_02027624(1);
    ChangeDir("/Game/Myroom/");
    screen.LoadCharacters("myroom_status_u_obj_32k.nce_ncg1D.bin", 0, 0);
    screen.LoadPalette("myroom_status_u_obj_32k_ncl.bin", 0, 0);
    mCellAnim = new CellAnimation("myroom_status_u_obj_32k_nce.bin", 0, 1, 1);
    mCellAnim->mCharBase = screen.func_020276f4(0);
    mCellAnim->mUnk_11   = 0;
    mCellAnim->SetAnimation(0);
    LoadText();
    ChangeDir("/");
}

void MyroomStatusScene::Draw3D() {
    DrawLines();
    DrawCell();
    MtxFx43 *view = func_020323b8();
    MtxFx43 mtx   = data_0220e86c;
    VecFx32 b      = data_ov008_02228640;
    VecFx32 a      = data_ov008_02228634;
    func_01ffcf64(&mtx, &a);
    func_01ff89c0(&mtx, view, &mtx);
    mUnk_14->func_02028d80();
    mUnk_14->func_02029174(&mtx, &b);
    mModel->func_02054568();
}

void MyroomStatusScene::DrawCell() {
    if (mCellAnim == NULL) {
        return;
    }
    mCellAnim->Update();
    mCellAnim->Draw(0, 0x80, 0x60, 0);
}

void MyroomStatusScene::LoadText() {
    ChangeDir("/font/");
    mMessages = (MessageFile *) func_02023504("mes_myroom_status.mes", 0, 0, 0, 0, 0);
    mTextBufferSize = TextManager::Get().BufferSize(0x20, 2);
    for (u8 i = 0; i < 5; i++) {
        mTextBuffers[i] = OS_Alloc(mTextBufferSize);
        MI_CpuClear16(mTextBuffers[i], mTextBufferSize);
    }
    mGlyphCount  = TextManager::Get().GlyphCount(8);
    mGlyphBuffer = OS_Alloc((s16) mGlyphCount * (s16) 0xc);
    TextManager::Get().Init(mGlyphBuffer, 0, 0, 0x20, 2, 0x21);
    TextManager::Get().SetColor(0, 0, 0xf);
    ClearTextBuffers();
}

void MyroomStatusScene::ClearTextBuffers() {
    for (u8 i = 0; i < 5; i++) {
        if (mTextBuffers[i] != NULL) {
            MI_CpuClear16(mTextBuffers[i], mTextBufferSize);
        }
    }
}

void MyroomStatusScene::RenderLine(u8 line) {
    Ranking *info = mUnk_50;
    s16 x;
    s16 y;
    s16 zeroMin;
    s16 zeroSec;
    s16 hoursW;
    s16 hoursH;
    s16 minW;
    s16 minH;
    x = 0;
    y = 0;
    switch (line) {
        case 0:
            TextManager::Get().mUnk_5b = 1;
            TextManager::Get().Print(data_020d43f0[0].mName, 0x18, mTextBuffers[line], 0x20, 2, &x, &y, 0, 0, 2, 0x21, 0,
                                     0, 0, 0, 1, 1);
            TextManager::Get().mUnk_5b = 0;
            break;
        case 1:
            TextManager::Get().PrintNumber(info->mPlayerRank, 3, 0, mTextBuffers[line], 0x20, 2, &x, &y, 0, 2, 0x21, 0, 0, 1, 1);
            break;
        case 2:
            TextManager::Get().PrintNumber(data_020d43f0[0].mMoney, 7, 0, mTextBuffers[line], 0x20, 2, &x, &y, 0, 2,
                                           0x21, 0, 0, 1, 1);
            break;
        case 3:
            TextManager::Get().PrintNumber(info->mPlayerScore, 7, 0, mTextBuffers[line], 0x20, 2, &x, &y, 0, 2, 0x21, 0, 0,
                                           1, 1);
            break;
        case 4:
            if (mHours == 0) {
                x += 0x18;
            } else {
                TextManager::Get().PrintNumber(mHours, 3, 0, mTextBuffers[line], 0x20, 2, &x, &y, 0, 2, 0x21, 0, 0, 1, 1);
                TextManager &text = TextManager::Get();
                hoursH = y;
                hoursW = x;
                MessageFile *file = mMessages;
                text.Print(file->GetText(0), file->GetLength(0), mTextBuffers[line], 0x20, 2, &hoursW, &hoursH, 0, 0, 2, 0x21, 0, 0, 0,
                           0, 1, 1);
                x += 6;
            }
            if (mHours == 0 && mMinutes == 0) {
                x += 0x12;
            } else {
                if (mHours != 0 && mMinutes < 10) {
                    zeroMin = x;
                    TextManager::Get().PrintNumber(0, 1, 0, mTextBuffers[line], 0x20, 2, &zeroMin, &y, 0, 2, 0x21, 0, 0, 1, 1);
                }
                TextManager::Get().PrintNumber(mMinutes, 2, 0, mTextBuffers[line], 0x20, 2, &x, &y, 0, 2, 0x21, 0, 0, 1, 1);
                TextManager &text = TextManager::Get();
                minH = y;
                minW = x;
                MessageFile *file = mMessages;
                text.Print(file->GetText(0), file->GetLength(0), mTextBuffers[line], 0x20, 2, &minW, &minH, 0, 0, 2, 0x21, 0, 0, 0,
                           0, 1, 1);
                x += 6;
            }
            if ((mHours != 0 || mMinutes != 0) && mSeconds < 10) {
                zeroSec = x;
                TextManager::Get().PrintNumber(0, 1, 0, mTextBuffers[line], 0x20, 2, &zeroSec, &y, 0, 2, 0x21, 0, 0, 1, 1);
            }
            TextManager::Get().PrintNumber(mSeconds, 2, 0, mTextBuffers[line], 0x20, 2, &x, &y, 0, 2, 0x21, 0, 0, 1, 1);
            break;
    }
    SpriteManager &sprites = SpriteManager::Get();
    func_02025c1c(sprites.Screen(0).mBgIndex, 4, data_ov008_0222864c[line], mTextBuffers[line], mTextBufferSize, 0);
}

void MyroomStatusScene::DrawLines() {
    for (u8 i = 0; i < 5; i++) {
        SpriteManager::Get().Screen(0).func_02027778(mGlyphBuffer, mGlyphCount, data_ov008_02228688[i][0],
                                                     data_ov008_02228688[i][1], 0,
                                                     (data_ov008_0222864c[i] << 11) >> 16, 0xf, 0);
    }
}

void MyroomStatusScene::FreeText() {
    u8 i = 0;
    ClearTextBuffers();
    for (; i < 5; i++) {
        if (mTextBuffers[i] != NULL) {
            OS_Free(mTextBuffers[i]);
            mTextBuffers[i] = NULL;
        }
    }
    if (mMessages != NULL) {
        OS_Free(mMessages);
        mMessages = NULL;
    }
    if (mGlyphBuffer != NULL) {
        OS_Free(mGlyphBuffer);
        mGlyphBuffer = NULL;
    }
}

void MyroomStatusScene::UpdateTime() {
    u32 frames = data_020d43f0[0].mPlayFrames;
    mHours = frames / 216000;
    frames -= mHours * 216000;
    mMinutes = frames / 3600;
    frames -= mMinutes * 3600;
    u8 seconds = frames / 60;
    if (seconds != mSeconds) {
        mSeconds = seconds;
        if (mTextBuffers[4] != NULL) {
            MI_CpuClear16(mTextBuffers[4], mTextBufferSize);
        }
        RenderLine(4);
    }
}
