#include "Scene/MyroomShootScene.hpp"
#include "Game/UnkStruct_020d433c.hpp"
#include "Game/UnkStruct_020d43f0.hpp"
#include "Graphics/Camera.hpp"
#include "Graphics/CellAnimation.hpp"
#include "Graphics/Display.hpp"
#include "Graphics/Effect.hpp"
#include "Graphics/Model.hpp"
#include "Graphics/SpriteManager.hpp"
#include "System/FileSystem.hpp"
#include "System/InputManager.hpp"
#include "System/SoundManager.hpp"
#include "Text/TextManager.hpp"
#include "UI/MessageWindow.hpp"
#include "UI/SelectWindow.hpp"

#include <nitro/gx.h>
#include <nitro/mi.h>
#include <nitro/os.h>
#include <stdio.h>

extern const u32 data_ov008_02229068[];
extern const VecFx32 data_ov008_022290cc[];
extern const u16 data_ov008_02229038[];
extern const u16 data_ov008_02229080[][3];
extern const u16 data_ov008_02229082[][3];
extern const u16 data_ov008_02229084[][3];

extern const VecFx32 data_ov008_02229044;
extern const VecFx32 data_ov008_02229050;

extern const u8 data_ov008_0222901e[2][3];
extern const u8 data_ov008_0222901a[][2];
extern const u32 data_ov008_02229024[];
extern const u8 data_ov008_0222902c[][2];
extern const u8 data_ov008_022290a4[];
extern const WindowItemRecord data_ov008_02229184[];
extern const WindowCellRecord data_ov008_02229114[];
extern WindowCellRecord data_ov008_0222ab2c[];
extern const u8 data_ov008_0222905c[][6];
extern const u8 data_ov008_0222905d[][6];

MyroomShootScene::MyroomShootScene() :
    SceneBase(1000, 0) {
    mCamera      = NULL;
    mLights      = NULL;
    mShooter     = NULL;
    mRoom        = NULL;
    mMessages    = NULL;
    mMessage     = NULL;
    mWindow      = NULL;
    mResult      = 0;
    mEffect      = 0;
    mRot[0]      = 0;
    mRot[1]      = 0;
    mRot[2]      = 0;
    mUnk_5a      = 0x100;
    mUnk_5c      = 0;
    mUnk_5f      = 0;
    mSelected    = -1;
    mUnk_61      = 0;
    mUnk_62      = 0;
    mUnk_63      = 0;
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
    data_020d433c.mEffects = NULL;
}

MyroomShootScene::~MyroomShootScene() {
    CameraManager::Get().func_02026504(0, NULL, 0);
    ModelInstance *room = mRoom;
    if (room != NULL) {
        room->mUnk_68 = 0xf;
        delete room;
    }
    UnkStruct_0203d374 *shooter = mShooter;
    if (shooter != NULL) {
        delete shooter;
    }
    data_020d433c.mEffects->func_0202be68();
    func_0202bc58(data_020d433c.mEffects, 0);
    delete mWindow;
    delete mCamera;
    UnkStruct_02029ca8 *lights = mLights;
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
    delete mMessage;
    if (data_020d433c.mEffects != NULL) {
        data_020d433c.mEffects->mFinished = 1;
        data_020d433c.mEffects            = NULL;
    }
    MI_CpuClearFast((void *) 0x6000000, 0x80000);
    MI_CpuClearFast((void *) 0x6200000, 0x20000);
    SpriteManager::Get().func_02027568(0);
    SpriteManager::Get().func_0202740c();
}

void MyroomShootScene::Update() {
    BOOL draw = TRUE;
    switch (mState) {
        case 0:
            draw = FALSE;
            InitDisplay();
            func_ov008_02221a14();
            mTimer = 10;
            mState++;
            break;
        case 1:
            draw = FALSE;
            mTimer--;
            if (mTimer > 0) {
                break;
            }
            SpriteManager::Get().StartFade(2);
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
    SpriteManager::Get().func_02026dec(0, 7);
    func_02001e68(1, 0, 1);
    func_02001ed0(0);
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

void MyroomShootScene::func_ov008_02221a14() {
    mCamera = new Camera();
    mCamera->SetUp(0, 0x1000, 0);
    mCamera->SetTarget(0x28000, 0x6e000, 0);
    mCamera->SetClip(0x1000, 0x1388000);
    mCamera->func_02026004(0x2aaa);
    mCamera->SetPosition(0x14000, 0x5a000, 0x96000);
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
    mLights = new UnkStruct_02029ca8(data_ov008_022290a4);
    ChangeDir("/Game/Myroom/");
    func_02025b64(1, 7, 0, "myroom_b_bg_00_nsc.bin", 0, 0, 0);
    func_02025b64(1, 11, 0, "myroom_b_bg_00_ncg.bin", 0, 0, 0);
    func_02025b64(1, 1, 0, "myroom_b_bg_00_ncl.bin", 0, 0, 0);
    SpriteScreen &main = SpriteManager::Get().Screen(0);
    main.func_02027624(3);
    main.LoadCharacters("myroom_s_shoot_u_obj_00.nce_ncg1D.bin", 0x4000, 0);
    main.LoadPalette("myroom_s_shoot_u_obj_00_ncl.bin", 0, 0);
    mCellAnim            = new CellAnimation("myroom_s_shoot_u_obj_00_nce.bin", 0, 1, 1);
    mCellAnim->mCharBase = main.func_020276f4(0x4000);
    mCellAnim->mUnk_11   = 0;
    mCellAnim->SetAnimation(0);
    ChangeDir("/");
    mWindow = NULL;
    mWindow = new SelectWindow(10, data_ov008_02229184, -1);
    SpriteScreen &sub = SpriteManager::Get().Screen(1);
    sub.func_02027624(3);
    ChangeDir("/");
    sub.LoadCharacters("Game/Myroom/myroom_s_shoot_b_obj.nce_ncg1D.bin", 0, 0);
    sub.LoadPalette("Game/Myroom/myroom_s_shoot_b_obj_ncl.bin", 0, 0);
    sub.LoadCharacters("Game/2D/union_b/union_b_obj_cockpit.nce_ncg1D.bin", 0x5000, 0);
    sub.LoadPalette("Game/2D/union_b/union_b_obj_cockpit_ncl.bin", 0x140, 0);
    sub.LoadCharacters("Game/2D/union_b/union_b_obj_page.nce_ncg1D.bin", 0x8000, 0);
    sub.LoadPalette("Game/2D/union_b/union_b_obj_page_ncl.bin", 0x160, 0);
    for (u8 i = 0; i < 6; i++) {
        if (mUnlocked[i] != 0) {
            u8 a = data_ov008_0222902c[i][0];
            u8 b = data_ov008_0222902c[i][1];
            *(u8 *) &data_ov008_0222ab2c[i].mUnk_04 = a;
            data_ov008_0222ab2c[i].mUnk_18[0] = a;
            data_ov008_0222ab2c[i].mUnk_18[1] = b;
            data_ov008_0222ab2c[i].mUnk_18[2] = a;
        } else {
            *(u8 *) &data_ov008_0222ab2c[i].mUnk_04 = 1;
            data_ov008_0222ab2c[i].mUnk_18[0] = 1;
            data_ov008_0222ab2c[i].mUnk_18[1] = 1;
            data_ov008_0222ab2c[i].mUnk_18[2] = 1;
        }
    }
    mWindow->SetCells(6, data_ov008_0222ab2c);
    mWindow->SetCells(4, data_ov008_02229114);
    ChangeDir("/Game/Myroom/Map/");
    NitroModel *model     = new NitroModel();
    NitroTexture *texture = new NitroTexture("myroom_s.nsbtx");
    model->func_020289e0("myroom_s.nsbmd", texture);
    texture->func_02028344();
    mRoom = model->func_02028b78(0);
    data_020d433c.mEffects = new EffectManager();
    func_0202ba64(data_020d433c.mEffects, 0, "shoot");
    SpriteManager::Get().func_02027504(0);
    func_ov008_02222474(0);
    ChangeDir("/font/");
    if (mMessages == NULL) {
        mMessages = (MessageFile *) func_02023504("mes_myroom.mes", 0, 0, 0, 0, 0);
    }
    for (u8 i = 0; i < 2; i++) {
        mTextBufferSize[i] = (u16) TextManager::Get().BufferSize((s16) data_ov008_0222901a[i][0], (s16) data_ov008_0222901a[i][1]);
        mGlyphCount[i]     = TextManager::Get().GlyphCount(data_ov008_0222901a[i][0], data_ov008_0222901a[i][1], data_ov008_02229024[i]);
        mTextBuffers[i]    = OS_Alloc(mTextBufferSize[i]);
        ClearTextBuffers(i);
        mGlyphBuffers[i] = OS_Alloc(mGlyphCount[i] * 0xc);
        TextManager::Get().Init(mGlyphBuffers[i], 0, 0, data_ov008_0222901a[i][0], data_ov008_0222901a[i][1], data_ov008_02229024[i]);
    }
    TextManager::Get().SetColor(0, 0, 0xf);
    mMessage = new MessageWindow("mes_myroom", 1, 0x7000);
    mMessage->func_02065bd4(0x14, 4, 0);
    mMessage->func_02065be4(0xd8);
    mMessage->func_02065890(0x23, 0, 1);
    func_ov008_02222a78();
    ChangeDir("/");
}

void MyroomShootScene::func_ov008_02222474(s8 index) {
    if (mSelected != index) {
        mSelected                   = index;
        UnkStruct_0203d374 *shooter = mShooter;
        if (shooter != NULL) {
            delete shooter;
        }
        mShooter = NULL;
        if (mEffect != 0) {
            func_0202beb8(data_020d433c.mEffects, mEffect);
            mEffect = 0;
        }
        if (mUnlocked[mSelected] != 0) {
            u16 modelRot[3];
            u16 effectRot[3];
            VecFx32 pos;
            mShooter = new UnkStruct_0203d374();
            mShooter->func_0203d4dc(data_ov008_02229068[mSelected], (u8) mSelected, 0);
            pos = data_ov008_022290cc[mSelected];
            mShooter->func_0203db70(&pos);
            mRot[0]  = 0;
            mRot[1]  = 0x154;
            mRot[2]  = 0;
            modelRot[0] = (u16) (65536.0f * (float) mRot[0] / 360.0f);
            modelRot[1] = (u16) (65536.0f * (float) mRot[1] / 360.0f);
            modelRot[2] = (u16) (65536.0f * (float) mRot[2] / 360.0f);
            mShooter->func_0203db94(modelRot);
            mEffect = func_0202bd40(data_020d433c.mEffects, 0, (u8) data_ov008_02229038[mSelected], 0, 0);
            func_0202bef4(data_020d433c.mEffects, mEffect, &pos);
            effectRot[0] = (u16) (65536.0f * (float) data_ov008_02229080[mSelected][0] / 360.0f);
            effectRot[1] = (u16) (65536.0f * (float) data_ov008_02229082[mSelected][0] / 360.0f);
            effectRot[2] = (u16) (65536.0f * (float) data_ov008_02229084[mSelected][0] / 360.0f);
            func_0202bf3c(data_020d433c.mEffects, mEffect, effectRot);
        }
        mUnk_63 = 0;
        mUnk_5a = 0x100;
    }
    mUnk_5e = -1;
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

// non-matching: register allocation of the glyph-loop locals (x/first/last and the SpriteManager guard-loop temp rotate), 17 words differ (+0x194)
void MyroomShootScene::DrawSprites() {
    if (mCellAnim != NULL) {
        mCellAnim->Update();
        mCellAnim->Draw(0, 0x80, 0x60, 0);
    }
    MtxFx43 *base = func_020323b8();
    MtxFx43 view  = data_0220e86c;
    VecFx32 scale = data_ov008_02229044;
    VecFx32 offset = data_ov008_02229050;
    func_01ffcf64(&view, &offset);
    func_01ff89c0(&view, base, &view);
    mRoom->func_02028d80();
    mRoom->func_02029174(&view, &scale);
    mUnk_5f--;
    if (mUnk_5f <= 0) {
        mUnk_5f = 0;
        if (mShooter != NULL) {
            mShooter->func_0203d988();
            mShooter->func_0203dab0();
        }
    }
    if (mUnlocked[mSelected] == 0) {
        return;
    }
    SpriteManager::Get().Screen(0).func_02027778(mGlyphBuffers[0], 4, 0x70, 0x1e, 0, 0, 0xf, 0);
    s16 x;
    u8 first;
    u8 k;
    u8 last;
    x = 0x80 - ((u32) mUnk_58 >> 1);
    if ((s32) mUnk_58 > 0xf0) {
        x       = mUnk_5a;
        mUnk_5a = x - 1;
        if (mUnk_5a <= -mUnk_58) {
            mUnk_5a = 0x100;
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
        SpriteManager::Get().Screen(0).func_02027778((u8 *) mGlyphBuffers[1] + k * 0xc, 1, x, 8, 0, 0x60, 0xf, 0);
        k++;
    } while (k < last);
}

void MyroomShootScene::DrawWindows() {
    mMessage->func_020658d0();
    mWindow->Draw();
}

void MyroomShootScene::ClearTextBuffers(u8 index) {
    MI_CpuClear16(mTextBuffers[index], mTextBufferSize[index]);
}

// non-matching: TextManager::mUnk_5b store vs singleton guard-flag load scheduling (known open problem, 10 words at +0x194)
void MyroomShootScene::func_ov008_02222a78() {
    ClearTextBuffers(0);
    TextManager &text = TextManager::Get();
    s16 width;
    s16 height;
    height            = 2 - mUnk_63 * 0x7e;
    width             = 0x10;
    u16 index         = mSelected + 0x16;
    MessageFile *file = mMessages;
    text.Print(file->GetText(index), file->GetLength(index), mTextBuffers[0], 0x10, 0x12, &width, &height, 0, 0, 2, 0x44,
               0xe, 0, 0, 0, 1, 1);
    if (&mUnk_5c != NULL) {
        mUnk_5c = height;
    }
    func_02025c1c(SpriteManager::Get().Screen(0).mBgIndex, 4, 0, mTextBuffers[0], mTextBufferSize[0], 0);
    ClearTextBuffers(1);
    TextManager::Get().mUnk_5b = 1;
    TextManager &text2         = TextManager::Get();
    s16 height2;
    s16 width2;
    width2     = 0;
    height2    = 0;
    u16 index2 = mSelected + 0x1c;
    file       = mMessages;
    text2.Print(file->GetText(index2), file->GetLength(index2), mTextBuffers[1], 0x40, 2, &height2, &width2, &mUnk_58, 0, 2,
                0x21, 0xe, 0, 0, 0, 1, 1);
    TextManager::Get().mUnk_5b = 0;
    func_02025c1c(SpriteManager::Get().Screen(0).mBgIndex, 4, 0x3000, mTextBuffers[1], mTextBufferSize[1], 0);
    if (mUnlocked[mSelected] == 0) {
        mUnk_5c = 0;
    }
}

s32 MyroomShootScene::func_ov008_02222dbc() {
    s32 result = -1;
    s8 previous = mUnk_63;
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
                mUnk_61 = result % 3;
                mUnk_62 = result / 3;
                if (mSelected != (s8) id) {
                    SoundManager::Get().func_020246f8(0);
                }
                break;
            case 8:
                if (previous > 0) {
                    mUnk_63--;
                    SoundManager::Get().func_020246f8(0);
                }
                break;
            case 9:
                if (mUnk_5c > 0x80) {
                    mUnk_63++;
                    SoundManager::Get().func_020246f8(0);
                }
                break;
        }
    }
    if (previous != mUnk_63) {
        func_ov008_02222a78();
    } else if (result == -1) {
        result = func_ov008_02222fa4();
    }
    return result;
}

s8 MyroomShootScene::func_ov008_02222fa4() {
    s8 result  = -1;
    BOOL moved = FALSE;
    if (InputManager::Get().IsTrigger(0x40)) {
        moved = TRUE;
        if (mUnk_62 != 0) {
            mUnk_62--;
        } else {
            mUnk_62 = 1;
            if (data_ov008_0222901e[mUnk_62][mUnk_61] == 0) {
                mUnk_62--;
            }
        }
    } else if (InputManager::Get().IsTrigger(0x80)) {
        moved = TRUE;
        mUnk_62++;
        if (mUnk_62 >= 2) {
            mUnk_62 = 0;
        } else if (data_ov008_0222901e[mUnk_62][mUnk_61] == 0) {
            mUnk_62 = 0;
        }
    }
    if (InputManager::Get().IsTrigger(0x20)) {
        moved = TRUE;
        if (mUnk_61 != 0) {
            mUnk_61--;
        } else {
            mUnk_61 = 2;
            if (data_ov008_0222901e[mUnk_62][mUnk_61] == 0) {
                mUnk_61--;
            }
        }
    } else if (InputManager::Get().IsTrigger(0x10)) {
        moved = TRUE;
        mUnk_61++;
        if (mUnk_61 >= 3) {
            mUnk_61 = 0;
        } else if (data_ov008_0222901e[mUnk_62][mUnk_61] == 0) {
            mUnk_61 = 0;
        }
    }
    if (moved) {
        result = mUnk_61 + mUnk_62 * 3;
        SoundManager::Get().func_020246f8(0);
    }
    return result;
}

void MyroomShootScene::func_ov008_02223264() {
    if (mResult == 2) {
        SoundManager::Get().func_020246f8(1);
        SpriteManager::Get().StartFade(3);
        mState = 3;
    } else if (mUnk_5e != -1) {
        SpriteManager::Get().func_0202753c(0);
        func_ov008_02222474(mUnk_5e);
        func_ov008_02222a78();
        if (mWindow->mUnk_061 != 1) {
            mWindow->mUnk_061 = 4;
            mWindow->func_0205c7c8(-1);
        }
    } else {
        s32 selection = func_ov008_02222dbc();
        if (selection >= 0) {
            mUnk_5e = selection;
            mUnk_5f = 3;
        }
    }
    WindowItem *items = mWindow->mCursor->mItems;
    if (mUnk_63 != 0) {
        if (mWindow->mCells[8]->mUnk_0e == 2) {
            mWindow->mCells[8]->SetAnimation(0);
            items[8].mFlags = 0x208;
        }
    } else {
        if (mWindow->mCells[8]->mUnk_0e != 2) {
            mWindow->mCells[8]->SetAnimation(2);
            items[8].mFlags = 0x8000;
        }
    }
    if (mUnk_5c > 0x80) {
        if (mWindow->mCells[9]->mUnk_0e == 5) {
            mWindow->mCells[9]->SetAnimation(3);
            items[9].mFlags = 0x408;
        }
    } else {
        if (mWindow->mCells[9]->mUnk_0e != 5) {
            mWindow->mCells[9]->SetAnimation(5);
            items[9].mFlags = 0x8000;
        }
    }
    Vec2s pos = {data_ov008_0222905c[mUnk_62][mUnk_61 * 2], data_ov008_0222905d[mUnk_62][mUnk_61 * 2]};
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
    mShooter->func_0203db94(rot);
}
