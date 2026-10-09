#include "Scene/CharacterEditScene.hpp"
#include "Graphics/CharacterModel.hpp"
#include "System/InputManager.hpp"
#include "System/FrameTask.hpp"
#include "Game/UnkStruct_020d433c.hpp"
#include "Graphics/Display.hpp"
#include "Graphics/SpriteManager.hpp"
#include <nitro/gx.h>
#include <nitro/os.h>
#include <nitro/mi.h>
#include "Text/TextManager.hpp"
#include "System/FileSystem.hpp"
#include "System/SoundManager.hpp"

extern const u32 data_0209e784[6];

// The look fields of the save data (+0x14..+0x1c of data_020d43f0) as CharacterLook.
#define SAVE_LOOK ((CharacterLook *) ((u8 *) data_020d43f0 + 0x14))
extern WindowItem data_0209e9f4[];
extern const WindowItemRecord data_0209f5fc[];
extern const WindowItemRecord data_0209facc[];
extern const u8 data_0209f864[];
extern const WindowCellRecord data_0209e834[];
extern const WindowCellRecord data_0209e888[];
extern const WindowCellRecord data_020b3a28[];
extern const WindowCellRecord data_020b3a44[];
extern WindowItem data_0209ec5c[];
extern WindowItem data_0209eec4[];
extern WindowItem data_0209f12c[];
extern WindowItem data_0209f394[];

// Rounds an integer through float, the way the camera set-up code does (the temporary keeps it from being folded).
static inline fx32 FxRound(s32 v) {
    s32 t = v;
    return (fx32) (t > 0 ? t + 0.5f : t - 0.5f);
}


extern const Vec2s data_0209e7f4[16];

extern MtxFx43 data_0220e86c;

#include "Game/UnkStruct_020d43f0.hpp"
#include "Game/Random.hpp"
#include <nitro/math.h>

extern const u8 data_0209e79c[];

CharacterEditScene::CharacterEditScene(u32 unk) :
    SceneBase(2, 0) {
    mModel   = NULL;
    mCamera  = NULL;
    mState   = 0;
    mUnk_31  = 0;
    mUnk_32  = 0;
    mUnk_140 = NULL;
    mMessage = NULL;
    mUnk_148 = 0;
    mUnk_14c = 0;
    mUnk_150 = NULL;
    mUnk_158 = 0;
    mUnk_1b4 = unk;
    mUnk_1d2 = 0x2cec;
    GX_SetVisiblePlane(0);
    GXS_SetVisiblePlane(0);
    for (u8 i = 0; i < 6; i++) {
        mTextBuffers[i]    = NULL;
        mTexts[i].mActive  = 0;
        mTexts[i].mUnk_00  = -1;
    }
    mUnk_134 = NULL;
    mUnk_138 = 0;
    mUnk_13c = 0;
    mUnk_18  = 0;
    mUnk_1c  = 0;
    mUnk_20  = 0;
    mPalette = new UnkStruct_02029ca8(data_0209e79c);
    mUnk_154 = 0;
    for (u8 i = 0; i < 3; i++) {
        mUnk_155[i] = 0;
    }
    func_02064024();
    for (u8 i = 0; i < 16; i++) {
        mCells[i] = NULL;
    }
    mUnk_1a8 = 0;
    mUnk_1b0 = 0;
    mUnk_1b8 = 0;
    mUnk_1cc = 0;
    mUnk_1bc = NULL;
    mUnk_1c0 = NULL;
    mUnk_1c4 = 0;
    mUnk_1c8 = 0;
    mUnk_1d0 = 2;
    mUnk_1ac = -1;
    mUnk_1d4 = NULL;
    mUnk_1d8 = NULL;
    mUnk_1dc = 0;
    mUnk_1e0 = 0;
    mUnk_1e4 = NULL;
}

CharacterEditScene::~CharacterEditScene() {
    UnkStruct_0205401c *model = mModel;
    if (model != NULL) {
        model->func_02054034();
        delete model;
    }
    ModelInstance *instance = mUnk_150;
    if (instance != NULL) {
        instance->mUnk_68 = 0xf;
        delete instance;
        mUnk_150 = NULL;
    }
    CameraManager::Get().func_02026504(0, NULL, 0);
    delete mCamera;
    if (mWindows[0] != NULL) {
        delete mWindows[0];
    }
    if (mWindows[1] != NULL) {
        delete mWindows[1];
    }
    if (mWindows[2] != NULL) {
        delete mWindows[2];
    }
    delete mCursor;
    func_0205d090();
    if (mMessage != NULL) {
        delete mMessage;
    }
    for (u8 i = 0; i < 6; i++) {
        if (mTextBuffers[i] != NULL) {
            OS_Free(mTextBuffers[i]);
            mTextBuffers[i] = NULL;
        }
    }
    if (mUnk_1bc != NULL) {
        OS_Free(mUnk_1bc);
        mUnk_1bc = NULL;
    }
    if (mUnk_140 != NULL) {
        OS_Free(mUnk_140);
    }
    if (mUnk_134 != NULL) {
        OS_Free(mUnk_134);
        mUnk_134 = NULL;
    }
    if (mUnk_1c0 != NULL) {
        OS_Free(mUnk_1c0);
        mUnk_1c0 = NULL;
    }
    UnkStruct_02029ca8 *palette = mPalette;
    if (palette != NULL) {
        delete palette;
    }
    CellAnimation *cell;
    u8 i = 0;
    do {
        cell = mCells[i];
        if (cell != NULL) {
            delete cell;
        }
        i++;
    } while (i < 16);
    func_02064f80();
    if (mUnk_1d4 != NULL) {
        OS_Free(mUnk_1d4);
        mUnk_1d4 = NULL;
    }
    if (mUnk_1d8 != NULL) {
        OS_Free(mUnk_1d8);
        mUnk_1d8 = NULL;
    }
    cell = mUnk_1e4;
    if (cell != NULL) {
        delete cell;
    }
    SpriteManager::Get().func_0202740c();
}


void CharacterEditScene::func_0205d090() {
    u8 i = 0;
    do {
        mTexts[i].mActive = 0;
        mTexts[i].mUnk_00 = -1;
        MI_CpuClear16(mTextBuffers[i], mUnk_138);
        i++;
    } while (i < 6);
    MI_CpuClear16(mUnk_1bc, mUnk_1c4);
    mUnk_1cc = 0;
    mMessage->func_02065878();
}

void CharacterEditScene::func_0205d0fc(u32 slot, u32 message, u32 value) {
    if (slot == 0) {
        mMessage->func_02065890(message, 0, 1);
    } else {
        TextManager &text = TextManager::Get();
        s16 a;
        s16 b;
        b = 0;
        a = 0;
        MessageFile *file = (MessageFile *) mUnk_140;
        text.Print(file->GetText(message), file->GetLength(message), mTextBuffers[slot], 0x20, 2, &a, &b,
                   &mTexts[slot].mWidth, 0, 2, 0x21, 0xc, 0, 0, 1, 1, 1);
        if (&mTexts[slot].mHeight != NULL) {
            mTexts[slot].mHeight = b;
        }
        func_02025c1c(SpriteManager::Get().Screen(1).mBgIndex, 4, data_0209e784[slot], mTextBuffers[slot],
                      mUnk_138, 0);
    }
    mTexts[slot].mUnk_00 = value;
    mTexts[slot].mActive = 1;
}

void CharacterEditScene::func_0205d2b4() {
    if (mState == 0xd) {
        return;
    }
    WindowItem *items = mWindows[0]->mCursor->mItems;
    u8 i = 0;
    do {
        if (mTexts[i].mActive != 0) {
            if (i == 0) {
                mMessage->func_020658d0();
            } else {
                Vec2s pos = mWindows[0]->mCellPos[(u8) items[mTexts[i].mUnk_00].mId];
                s32 y = pos.y;
                SpriteScreen &screen = SpriteManager::Get().Screen(1);
                WindowItem *item = &items[mTexts[i].mUnk_00];
                screen.func_02027778(mUnk_134, (u8) mUnk_13c, (s16) (pos.x + item->mWidth / 2 - mTexts[i].mWidth / 2),
                                     (s16) (y + item->mHeight / 2 - mTexts[i].mHeight / 2), 0,
                                     (u16) (data_0209e784[i] >> 7), 0xf, 0);
            }
        }
        i++;
    } while (i < 6);
    if (mUnk_1cc != 0) {
        SpriteManager::Get().Screen(1).func_02027778(mUnk_1c0, (u8) mUnk_1c8, 0x20, 0x28, 0, 0x20, 0xf, 0);
    }
}

void CharacterEditScene::func_0205d4cc() {
    switch (mUnk_31) {
    case 0:
        func_02061c3c();
        func_02062194();
        mWindows[0]->mUnk_064 = 0;
        mWindows[1]->mUnk_064 = 0;
        mWindows[2]->mUnk_064 = 0;
        mWindows[1]->mButtons[0].mEnabled = 1;
        GX_SetVisiblePlane(0x1f);
        GXS_SetVisiblePlane(0x15);
        func_02064f98();
        mUnk_31++;
        break;
    case 1:
        mUnk_32++;
        if (mUnk_32 >= 3) {
            SpriteManager::Get().StartFade(2);
            mUnk_32 = 0;
            mState = 1;
            mUnk_31 = 2;
        }
        break;
    }
}

void CharacterEditScene::func_0205d5fc() {
    switch (mUnk_31) {
    case 2:
        SpriteManager::Get();
        if (SpriteManager::sFade[0] > 1 | SpriteManager::sFade[1] > 1) {
            return;
        }
        mWindows[0]->mUnk_064 = 1;
        mWindows[1]->mUnk_064 = 1;
        if (mUnk_158 != 2) {
            mUnk_155[mUnk_154] = 0;
        }
        mWindows[0]->func_0205c74c(10, data_0209e9f4, (s8) mUnk_155[mUnk_154]);
        if (mUnk_155[mUnk_154] == 0) {
            mWindows[0]->mCells[(u8) mWindows[0]->GetSelectedId()]->SetAnimation(0x19);
        } else {
            mWindows[0]->mCells[(u8) mWindows[0]->GetSelectedId()]->SetAnimation(4);
        }
        mWindows[1]->func_0205c7c8(-1);
        func_02063b10(0);
        func_02063c64();
        func_020637a8();
        func_020637dc();
        func_020638a8(0);
        GXS_SetVisiblePlane(0x15);
        {
            fx32 posZ = FxRound(0x5a000);
            fx32 posY = FxRound(0x6e000);
            Camera *cam = mCamera;
            cam->SetPosition(FxRound(0xb4000), posY, posZ);
            mCamera->SetTarget(FxRound(0x5000), FxRound(0x50000), FxRound(0));
        }
        {
            CameraManager &manager = CameraManager::Get();
            Camera *camera = manager.mCameras[manager.mActive];
            data_02211374.mCameraPosition = camera->mPosition;
            data_02211374.mCameraUp = camera->mUp;
            data_02211374.mCameraTarget = camera->mTarget;
            func_01ff8d34(&camera->mPosition, &camera->mUp, &camera->mTarget, &data_02211374.mView);
            data_02211374.mFlags &= ~0xe8;
            manager.mProjection[manager.mActive] = data_02211374.mProjection;
            manager.mView[manager.mActive] = data_02211374.mView;
            manager.mMtxA[manager.mActive] = *func_02032710();
            manager.mMtxB[manager.mActive] = *func_02032604();
            manager.mMtxC[manager.mActive] = *func_020323b8();
        }
        func_0205d090();
        func_0205d0fc(0, 2, 0);
        switch (mUnk_1ac) {
        case 0:
            func_0205d0fc(1, 7, 0);
            break;
        case 1:
            switch (mUnk_1b4) {
            case 0:
                func_0205d0fc(2, 0x17, 1);
                break;
            case 1:
                func_0205d0fc(2, 0xf, 1);
                break;
            }
            break;
        case 2:
            switch (mUnk_1b4) {
            case 0:
                func_0205d0fc(3, 0xf, 2);
                break;
            case 1:
                func_0205d0fc(3, 0x10, 2);
                break;
            }
            break;
        case 3:
            switch (mUnk_1b4) {
            case 0:
                func_0205d0fc(4, 0x10, 3);
                break;
            case 1:
                func_0205d0fc(4, 0x11, 3);
                break;
            }
            break;
        case 4:
            func_0205d0fc(5, 0x11, 4);
            break;
        }
        mUnk_31 = 3;
        break;
    case 3:
        if (mWindows[0]->mUnk_062 != 1) {
            return;
        }
        if (mUnk_1ac != 0) {
            func_0205d0fc(1, 7, 0);
        }
        switch (mUnk_1b4) {
        case 0:
            if (mUnk_1ac != 1) {
                func_0205d0fc(2, 0x17, 1);
            }
            if (mUnk_1ac != 2) {
                func_0205d0fc(3, 0xf, 2);
            }
            if (mUnk_1ac != 3) {
                func_0205d0fc(4, 0x10, 3);
            }
            if (mUnk_1ac != 4) {
                func_0205d0fc(5, 0x11, 4);
            }
            break;
        case 1:
            if (mUnk_1ac != 1) {
                func_0205d0fc(2, 0xf, 1);
            }
            if (mUnk_1ac != 2) {
                func_0205d0fc(3, 0x10, 2);
            }
            if (mUnk_1ac != 3) {
                func_0205d0fc(4, 0x11, 3);
            }
            break;
        }
        mUnk_31 = 4;
        break;
    case 4:
        if (mWindows[0]->mUnk_061 == 1) {
            mUnk_31 = 5;
        }
        break;
    case 5:
        if (mUnk_1c == 3) {
            if (func_0206332c()) {
                mWindows[0]->mUnk_061 = 3;
                mWindows[1]->mUnk_061 = 3;
                mUnk_31 = 10;
                return;
            }
        } else if (mUnk_18 == 3) {
            if (mWindows[0]->GetSelectedId() == 7) {
                SpriteManager::Get();
                SpriteManager::sFade[0] = 3;
                mUnk_31 = 6;
                mWindows[0]->mUnk_064 = 0;
                mWindows[1]->mUnk_064 = 0;
                return;
            }
        } else if (mUnk_18 == 1) {
            func_02063b10(1);
            func_02063dac();
            mUnk_31 = 10;
            return;
        } else if (mUnk_18 == 2) {
            if (mUnk_1b8 != 0 && mUnk_1b4 == 1) {
                mUnk_31 = 0xb;
                func_02064cd4(1);
                return;
            }
            if (mUnk_1b4 == 0) {
                func_02063570(NULL);
            }
            mUnk_31 = 9;
            func_02063b10(1);
            return;
        } else if (InputManager::Get().IsHeld(8) && mWindows[0]->mUnk_061 == 1) {
            mUnk_1b0 = 1;
            func_02063b10(1);
            mWindows[0]->mUnk_061 = 3;
            mUnk_31 = 10;
            SoundManager::Get().func_020246f8(0);
            return;
        }
        func_020638b8();
        break;
    case 6:
        SpriteManager::Get();
        if (SpriteManager::sFade[0] > 1) {
            return;
        }
        func_02064bb8();
        func_020649e4();
        mUnk_32 = 0;
        mUnk_31 = 7;
        break;
    case 7:
        mUnk_32++;
        if (mUnk_32 != 4) {
            return;
        }
        SpriteManager::Get();
        SpriteManager::sFade[0] = 2;
        mUnk_31 = 8;
        break;
    case 8:
        if (SpriteManager::Get().IsFading()) {
            return;
        }
        mWindows[0]->mCells[7]->SetAnimation(0x19);
        mUnk_31 = 5;
        mWindows[0]->mUnk_064 = 1;
        mWindows[1]->mUnk_064 = 1;
        break;
    case 9:
        if (mWindows[0]->mUnk_061 != 4) {
            return;
        }
        mState = 0xd;
        mUnk_31 = 0x5a;
        break;
    case 10:
        if (mWindows[0]->mUnk_061 != 4) {
            return;
        }
        if (func_02063e20()) {
            return;
        }
        if (mUnk_1b0 != 0) {
            mState = 0xc;
            mUnk_31 = 0x53;
            return;
        }
        if (mWindows[0]->GetSelectedId() == 3) {
            mState = 2;
            mUnk_31 = 0x10;
            if (mUnk_1b4 == 1) {
                mState = 3;
                mUnk_31 = 0x16;
            }
            mUnk_155[mUnk_154] = mWindows[0]->mCursor->mSelection;
            func_02063ad8(1);
            return;
        }
        if (mWindows[0]->GetSelectedId() == 4) {
            mState = 3;
            mUnk_31 = 0x16;
            if (mUnk_1b4 == 1) {
                mState = 4;
                mUnk_31 = 0x38;
            }
            mUnk_155[mUnk_154] = mWindows[0]->mCursor->mSelection;
            func_02063ad8(1);
            return;
        }
        if (mWindows[0]->GetSelectedId() == 5) {
            mState = 4;
            mUnk_31 = 0x38;
            if (mUnk_1b4 == 1) {
                mState = 5;
                mUnk_31 = 0x3f;
            }
            mUnk_155[mUnk_154] = mWindows[0]->mCursor->mSelection;
            func_02063ad8(1);
            return;
        }
        if (mWindows[0]->GetSelectedId() == 6) {
            mState = 5;
            mUnk_31 = 0x3f;
            mUnk_155[mUnk_154] = mWindows[0]->mCursor->mSelection;
            func_02063ad8(1);
            return;
        }
        if (mWindows[0]->GetSelectedId() == 8) {
            mState = 0xc;
            mUnk_31 = 0x53;
        }
        break;
    case 11:
        if (mWindows[0]->mUnk_061 != 4) {
            return;
        }
        mWindows[0]->mUnk_064 = 0;
        mWindows[1]->mUnk_064 = 0;
        mWindows[2]->mUnk_064 = 1;
        mWindows[2]->mCursor->mSelection = 1;
        mWindows[2]->mCells[1]->SetAnimation(7);
        GXS_SetVisiblePlane(0x1d);
        mWindows[2]->mButtons[0].mEnabled = 1;
        mWindows[2]->mButtons[1].mEnabled = 1;
        mWindows[2]->mButtons[2].mEnabled = 1;
        func_0205d090();
        func_02064d7c();
        mUnk_31 = 0xc;
        mUnk_148 = 0;
        mWindows[0]->mCells[9]->SetAnimation(0xc);
        mWindows[0]->mCells[10]->SetAnimation(0xe);
        break;
    case 12:
        if (mUnk_20 == 1) {
            mUnk_31 = 0xd;
        } else if (mUnk_20 == 2) {
            mUnk_31 = 0xe;
        }
        break;
    case 13:
        if (mWindows[2]->mUnk_061 != 4) {
            return;
        }
        func_02064cd4(0);
        mState = 0xd;
        mUnk_31 = 0x5a;
        break;
    case 14:
        if (mWindows[2]->mUnk_061 != 4) {
            return;
        }
        mWindows[2]->func_0205c7c8(-1);
        mWindows[2]->mUnk_064 = 0;
        mWindows[0]->func_0205c7c8(0);
        mWindows[0]->mCells[(u8) mWindows[0]->GetSelectedId()]->SetAnimation(0x19);
        mWindows[0]->mUnk_064 = 1;
        mWindows[1]->mUnk_064 = 1;
        func_0205d090();
        mUnk_31 = 0xf;
        break;
    case 15:
        if (mWindows[0]->mUnk_061 != 1) {
            return;
        }
        func_02064cd4(0);
        GXS_SetVisiblePlane(0x15);
        mWindows[2]->mButtons[0].mEnabled = 0;
        mWindows[2]->mButtons[1].mEnabled = 0;
        mWindows[2]->mButtons[2].mEnabled = 0;
        func_0205d0fc(0, 2, 0);
        func_0205d0fc(1, 7, 0);
        switch (mUnk_1b4) {
        case 0:
            func_0205d0fc(2, 0x17, 1);
            func_0205d0fc(3, 0xf, 2);
            func_0205d0fc(4, 0x10, 3);
            func_0205d0fc(5, 0x11, 4);
            break;
        case 1:
            func_0205d0fc(2, 0xf, 1);
            func_0205d0fc(3, 0x10, 2);
            func_0205d0fc(4, 0x11, 3);
            break;
        }
        mUnk_31 = 5;
        break;
    }
}

void CharacterEditScene::func_0205e6ac() {
    switch (mUnk_31) {
    case 0x10:
        mWindows[0]->func_0205c74c(6, data_0209ec5c, -1);
        mWindows[1]->func_0205c7c8(-1);
        func_02063b10(0);
        func_02063c64();
        func_020637a8();
        func_020637dc();
        func_020638a8(1);
        func_020642ac();
        func_02062fe4("sub_skin.nce_ncg1D.bin", "sub_skin_ncl.bin", "sub_skin_nce.bin", "color_skin_ncl.bin");
        func_0205d090();
        func_0205d0fc(0, 0x17, 0);
        if (mUnk_1ac == 0) {
            func_0205d0fc(1, 0x17, 0);
        }
        mUnk_31 = 0x11;
        break;
    case 0x11:
        if (mWindows[0]->mUnk_062 != 1) {
            return;
        }
        if (mUnk_1ac != 0) {
            func_0205d0fc(1, 0x17, 0);
        }
        mUnk_31 = 0x12;
        break;
    case 0x12:
        if (mWindows[0]->mUnk_061 == 1) {
            mUnk_31 = 0x13;
        }
        break;
    case 0x13:
        func_02064964();
        if (mUnk_1c == 3) {
            if (func_0206332c()) {
                mWindows[0]->mUnk_061 = 3;
                mWindows[1]->mUnk_061 = 3;
                mUnk_31 = 0x15;
                return;
            }
        } else if (mUnk_18 == 1) {
            mUnk_31 = 0x15;
            func_02063b10(1);
            func_02063d10();
            func_020638a8(0);
            return;
        } else if (mUnk_18 == 2) {
            mUnk_31 = 0x14;
            func_02063b10(1);
            func_02063d10();
            func_020638a8(0);
            return;
        }
        func_020638b8();
        break;
    case 0x14:
        if (mWindows[0]->mUnk_061 != 4) {
            return;
        }
        mState = 1;
        mUnk_31 = 2;
        func_02063ad8(2);
        break;
    case 0x15:
        if (mWindows[0]->mUnk_061 != 4) {
            return;
        }
        if (func_02063e20()) {
            return;
        }
        mState = 1;
        mUnk_31 = 2;
        func_02063ad8(2);
        break;
    }
}

void CharacterEditScene::func_0205e958() {
    switch (mUnk_31) {
    case 0x16: {
        if (mUnk_158 != 2) {
            mUnk_155[mUnk_154] = 1;
        }
        mWindows[0]->func_0205c74c(10, data_0209eec4, (s8) mUnk_155[mUnk_154]);
        mWindows[0]->mCells[(u8) mWindows[0]->GetSelectedId()]->SetAnimation(4);
        mWindows[1]->func_0205c7c8(-1);
        func_02063b10(0);
        func_02063c64();
        func_020637a8();
        func_020637dc();
        func_020638a8(0);
        fx32 posZ = FxRound(0x1e000);
        fx32 posY = FxRound(0x82000);
        mCamera->SetPosition(FxRound(0x3c000), posY, posZ);
        fx32 tgtZ = FxRound(0);
        fx32 tgtY = FxRound(0x82000);
        mCamera->SetTarget(FxRound(0x5000), tgtY, tgtZ);
        CameraManager &manager = CameraManager::Get();
        Camera *camera = manager.mCameras[manager.mActive];
        data_02211374.mCameraPosition = camera->mPosition;
        data_02211374.mCameraUp = camera->mUp;
        data_02211374.mCameraTarget = camera->mTarget;
        func_01ff8d34(&camera->mPosition, &camera->mUp, &camera->mTarget, &data_02211374.mView);
        data_02211374.mFlags &= ~0xe8;
        manager.mProjection[manager.mActive] = data_02211374.mProjection;
        manager.mView[manager.mActive] = data_02211374.mView;
        manager.mMtxA[manager.mActive] = *func_02032710();
        manager.mMtxB[manager.mActive] = *func_02032604();
        manager.mMtxC[manager.mActive] = *func_020323b8();
        func_0205d090();
        func_0205d0fc(0, 0xf, 0);
        switch (mUnk_1ac) {
        case 0:
            func_0205d0fc(1, 0xf, 0);
            break;
        case 1:
            switch (mUnk_1b4) {
            case 0:
                func_0205d0fc(2, 0x12, 1);
                break;
            case 1:
                func_0205d0fc(2, 0x13, 1);
                break;
            }
            break;
        case 2:
            switch (mUnk_1b4) {
            case 0:
                func_0205d0fc(3, 0x13, 2);
                break;
            case 1:
                func_0205d0fc(3, 0x18, 2);
                break;
            }
            break;
        case 3:
            switch (mUnk_1b4) {
            case 0:
                func_0205d0fc(4, 0x18, 3);
                break;
            case 1:
                func_0205d0fc(4, 0x14, 3);
                break;
            }
            break;
        case 4:
            func_0205d0fc(5, 0x14, 4);
            break;
        }
        mUnk_31 = 0x17;
        break;
    }
    case 0x17:
        if (mWindows[0]->mUnk_062 != 1) {
            return;
        }
        if (mUnk_1ac != 0) {
            func_0205d0fc(1, 0xf, 0);
        }
        switch (mUnk_1b4) {
        case 0:
            if (mUnk_1ac != 1) {
                func_0205d0fc(2, 0x12, 1);
            }
            if (mUnk_1ac != 2) {
                func_0205d0fc(3, 0x13, 2);
            }
            if (mUnk_1ac != 3) {
                func_0205d0fc(4, 0x18, 3);
            }
            if (mUnk_1ac != 4) {
                func_0205d0fc(5, 0x14, 4);
            }
            break;
        case 1:
            if (mUnk_1ac != 1) {
                func_0205d0fc(2, 0x13, 1);
            }
            if (mUnk_1ac != 2) {
                func_0205d0fc(3, 0x18, 2);
            }
            if (mUnk_1ac != 3) {
                func_0205d0fc(4, 0x14, 3);
            }
            break;
        }
        mUnk_31 = 0x18;
        break;
    case 0x18:
        if (mWindows[0]->mUnk_061 == 1) {
            mUnk_31 = 0x19;
        }
        break;
    case 0x19:
        if (mUnk_1c == 3) {
            if (func_0206332c()) {
                if (mWindows[1]->GetSelectedId() != 1) {
                    mWindows[0]->mUnk_061 = 3;
                    mWindows[1]->mUnk_061 = 3;
                    mUnk_31 = 0x1b;
                    return;
                }
            }
        } else if (mUnk_18 == 1) {
            if (mWindows[0]->GetSelectedId() == 2) {
                mUnk_31 = 0x1a;
                func_02063b10(1);
                func_02063d10();
                return;
            }
            mUnk_31 = 0x1b;
            func_02063b10(1);
            func_02063dac();
            return;
        } else if (mUnk_18 == 2) {
            mUnk_31 = 0x1a;
            func_02063b10(1);
            func_02063d10();
            return;
        } else if (InputManager::Get().IsHeld(8) && mWindows[0]->mUnk_061 == 1) {
            mUnk_1b0 = 1;
            func_02063b10(1);
            mWindows[0]->mUnk_061 = 3;
            mUnk_31 = 0x1b;
            return;
        }
        func_020638b8();
        break;
    case 0x1a:
        if (mWindows[0]->mUnk_061 != 4) {
            return;
        }
        mState = 1;
        mUnk_31 = 2;
        func_02063ad8(2);
        break;
    case 0x1b:
        if (mWindows[0]->mUnk_061 != 4) {
            return;
        }
        if (func_02063e20()) {
            return;
        }
        if (mUnk_1b0 != 0) {
            mState = 0xc;
            mUnk_31 = 0x53;
            return;
        }
        if (mWindows[0]->GetSelectedId() == 3) {
            mState = 6;
            mUnk_31 = 0x1c;
            if (mUnk_1b4 == 1) {
                mState = 7;
                mUnk_31 = 0x23;
            }
            mUnk_155[mUnk_154] = mWindows[0]->mCursor->mSelection;
            func_02063ad8(1);
            return;
        }
        if (mWindows[0]->GetSelectedId() == 4) {
            mState = 7;
            mUnk_31 = 0x23;
            if (mUnk_1b4 == 1) {
                mState = 8;
                mUnk_31 = 0x2a;
            }
            mUnk_155[mUnk_154] = mWindows[0]->mCursor->mSelection;
            func_02063ad8(1);
            return;
        }
        if (mWindows[0]->GetSelectedId() == 5) {
            mState = 8;
            mUnk_31 = 0x2a;
            if (mUnk_1b4 == 1) {
                mState = 9;
                mUnk_31 = 0x31;
            }
            mUnk_155[mUnk_154] = mWindows[0]->mCursor->mSelection;
            func_02063ad8(1);
            return;
        }
        if (mWindows[0]->GetSelectedId() == 6) {
            mState = 9;
            mUnk_31 = 0x31;
            mUnk_155[mUnk_154] = mWindows[0]->mCursor->mSelection;
            func_02063ad8(1);
            return;
        }
        if (mWindows[0]->GetSelectedId() == 8) {
            mState = 0xc;
            mUnk_31 = 0x53;
        }
        break;
    }
}

void CharacterEditScene::func_0205f3c0() {
    switch (mUnk_31) {
    case 0x1c:
        mWindows[0]->func_0205c74c(6, data_0209ec5c, -1);
        mWindows[1]->func_0205c7c8(-1);
        func_02063b10(0);
        func_02063c64();
        func_020637a8();
        func_020637dc();
        func_020638a8(1);
        func_020642ac();
        func_02062fe4("sub_face_male.nce_ncg1D.bin", "sub_face_male_ncl.bin", "sub_face_male_nce.bin", "color_face_male_ncl.bin");
        func_0205d090();
        func_0205d0fc(0, 0x12, 0);
        if (mUnk_1ac == 0) {
            func_0205d0fc(1, 0x12, 0);
        }
        mUnk_31 = 0x1d;
        break;
    case 0x1d:
        if (mWindows[0]->mUnk_062 != 1) {
            return;
        }
        if (mUnk_1ac != 0) {
            func_0205d0fc(1, 0x12, 0);
        }
        mUnk_31 = 0x1e;
        break;
    case 0x1e:
        if (mWindows[0]->mUnk_061 == 1) {
            mUnk_31 = 0x1f;
        }
        break;
    case 0x1f:
        func_02064964();
        if (mUnk_1c == 3) {
            if (func_0206332c()) {
                if (mWindows[1]->GetSelectedId() != 1) {
                    mWindows[0]->mUnk_061 = 3;
                    mWindows[1]->mUnk_061 = 3;
                    mUnk_31 = 0x22;
                    return;
                }
            }
        } else if (mUnk_18 == 1) {
            mUnk_31 = 0x21;
            func_02063b10(1);
            func_02063d10();
            func_020638a8(0);
            return;
        } else if (mUnk_18 == 2) {
            mUnk_31 = 0x21;
            func_02063b10(1);
            func_02063d10();
            func_020638a8(0);
            return;
        }
        func_020638b8();
        break;
    case 0x20:
        if (mWindows[0]->mUnk_061 != 4) {
            return;
        }
        mState = 3;
        mUnk_31 = 0x16;
        func_02063ad8(2);
        break;
    case 0x21:
        mUnk_31 = 0x20;
        break;
    case 0x22:
        if (mWindows[0]->mUnk_061 != 4) {
            return;
        }
        mUnk_14c = 0;
        func_02063e20();
        break;
    }
}

void CharacterEditScene::func_0205f698() {
    switch (mUnk_31) {
    case 0x23:
        mWindows[0]->func_0205c74c(6, data_0209ec5c, -1);
        mWindows[1]->func_0205c7c8(-1);
        func_02063b10(0);
        func_02063c64();
        func_020637a8();
        func_020637dc();
        func_020638a8(1);
        func_020642ac();
        func_02062fe4("sub_hair_male.nce_ncg1D.bin", "sub_hair_male_ncl.bin", "sub_hair_male_nce.bin", "color_hair_male_ncl.bin");
        func_0205d090();
        func_0205d0fc(0, 0x13, 0);
        if (mUnk_1ac == 0) {
            func_0205d0fc(1, 0x13, 0);
        }
        mUnk_31 = 0x24;
        break;
    case 0x24:
        if (mWindows[0]->mUnk_062 != 1) {
            return;
        }
        if (mUnk_1ac != 0) {
            func_0205d0fc(1, 0x13, 0);
        }
        mUnk_31 = 0x25;
        break;
    case 0x25:
        if (mWindows[0]->mUnk_061 == 1) {
            mUnk_31 = 0x26;
        }
        break;
    case 0x26:
        func_02064964();
        if (mUnk_1c == 3) {
            if (func_0206332c()) {
                if (mWindows[1]->GetSelectedId() != 1) {
                    mWindows[0]->mUnk_061 = 3;
                    mWindows[1]->mUnk_061 = 3;
                    mUnk_31 = 0x29;
                    return;
                }
            }
        } else if (mUnk_18 == 1) {
            mUnk_31 = 0x28;
            func_02063b10(1);
            func_02063d10();
            func_020638a8(0);
            return;
        } else if (mUnk_18 == 2) {
            mUnk_31 = 0x27;
            func_02063b10(1);
            func_02063d10();
            func_020638a8(0);
            return;
        }
        func_020638b8();
        break;
    case 0x27:
        if (mWindows[0]->mUnk_061 != 4) {
            return;
        }
        mState = 3;
        mUnk_31 = 0x16;
        func_02063ad8(2);
        break;
    case 0x28:
        mUnk_31 = 0x27;
        break;
    case 0x29:
        if (mWindows[0]->mUnk_061 != 4) {
            return;
        }
        mUnk_14c = 0;
        func_02063e20();
        break;
    }
}

void CharacterEditScene::func_0205f970() {
    switch (mUnk_31) {
    case 0x2a:
        mWindows[0]->func_0205c74c(6, data_0209ec5c, -1);
        mWindows[1]->func_0205c7c8(-1);
        func_02063b10(0);
        func_02063c64();
        func_020637a8();
        func_020637dc();
        func_020638a8(1);
        func_020642ac();
        func_02062fe4("sub_cap_male.nce_ncg1D.bin", "sub_cap_male_ncl.bin", "sub_cap_male_nce.bin", "color_cap_male_ncl.bin");
        func_0205d090();
        func_0205d0fc(0, 0x18, 0);
        if (mUnk_1ac == 0) {
            func_0205d0fc(1, 0x18, 0);
        }
        mUnk_31 = 0x2b;
        break;
    case 0x2b:
        if (mWindows[0]->mUnk_062 != 1) {
            return;
        }
        if (mUnk_1ac != 0) {
            func_0205d0fc(1, 0x18, 0);
        }
        mUnk_31 = 0x2c;
        break;
    case 0x2c:
        if (mWindows[0]->mUnk_061 == 1) {
            mUnk_31 = 0x2d;
        }
        break;
    case 0x2d:
        func_02064964();
        if (mUnk_1c == 3) {
            if (func_0206332c()) {
                if (mWindows[1]->GetSelectedId() != 1) {
                    mWindows[0]->mUnk_061 = 3;
                    mWindows[1]->mUnk_061 = 3;
                    mUnk_31 = 0x30;
                    return;
                }
            }
        } else if (mUnk_18 == 1) {
            mUnk_31 = 0x2f;
            func_02063b10(1);
            func_02063d10();
            func_020638a8(0);
            return;
        } else if (mUnk_18 == 2) {
            mUnk_31 = 0x2e;
            func_02063b10(1);
            func_02063d10();
            func_020638a8(0);
            return;
        }
        func_020638b8();
        break;
    case 0x2e:
        if (mWindows[0]->mUnk_061 != 4) {
            return;
        }
        mState = 3;
        mUnk_31 = 0x16;
        func_02063ad8(2);
        break;
    case 0x2f:
        mUnk_31 = 0x2e;
        break;
    case 0x30:
        if (mWindows[0]->mUnk_061 != 4) {
            return;
        }
        mUnk_14c = 0;
        func_02063e20();
        break;
    }
}

void CharacterEditScene::func_0205fc48() {
    switch (mUnk_31) {
    case 0x31:
        mWindows[0]->func_0205c74c(6, data_0209ec5c, -1);
        mWindows[1]->func_0205c7c8(-1);
        func_02063b10(0);
        func_02063c64();
        func_020637a8();
        func_020637dc();
        func_020638a8(1);
        func_020642ac();
        func_02062fe4("sub_glasses.nce_ncg1D.bin", "sub_glasses_ncl.bin", "sub_glasses_nce.bin", "color_glasses_fr_ncl.bin");
        func_0205d090();
        func_0205d0fc(0, 0x14, 0);
        if (mUnk_1ac == 0) {
            func_0205d0fc(1, 0x14, 0);
        }
        mUnk_31 = 0x32;
        break;
    case 0x32:
        if (mWindows[0]->mUnk_062 != 1) {
            return;
        }
        if (mUnk_1ac != 0) {
            func_0205d0fc(1, 0x14, 0);
        }
        mUnk_31 = 0x33;
        break;
    case 0x33:
        if (mWindows[0]->mUnk_061 == 1) {
            mUnk_31 = 0x34;
        }
        break;
    case 0x34:
        func_02064964();
        if (mUnk_1c == 3) {
            if (func_0206332c()) {
                if (mWindows[1]->GetSelectedId() != 1) {
                    mWindows[0]->mUnk_061 = 3;
                    mWindows[1]->mUnk_061 = 3;
                    mUnk_31 = 0x37;
                    return;
                }
            }
        } else if (mUnk_18 == 1) {
            mUnk_31 = 0x36;
            func_02063b10(1);
            func_02063d10();
            func_020638a8(0);
            return;
        } else if (mUnk_18 == 2) {
            mUnk_31 = 0x35;
            func_02063b10(1);
            func_02063d10();
            func_020638a8(0);
            return;
        }
        func_020638b8();
        break;
    case 0x35:
        if (mWindows[0]->mUnk_061 != 4) {
            return;
        }
        mState = 3;
        mUnk_31 = 0x16;
        func_02063ad8(2);
        break;
    case 0x36:
        mUnk_31 = 0x35;
        break;
    case 0x37:
        if (mWindows[0]->mUnk_061 != 4) {
            return;
        }
        mUnk_14c = 0;
        func_02063e20();
        break;
    }
}

void CharacterEditScene::func_0205ff20() {
    switch (mUnk_31) {
    case 0x38: {
        mWindows[0]->func_0205c74c(6, data_0209ec5c, -1);
        mWindows[1]->func_0205c7c8(-1);
        func_02063b10(0);
        func_02063c64();
        func_020637a8();
        func_020637dc();
        func_020638a8(1);
        func_020642ac();
        func_02062fe4("sub_top_male.nce_ncg1D.bin", "sub_top_male_ncl.bin", "sub_top_male_nce.bin",
                      "color_top_male_ncl.bin");
        mCamera->SetPosition(FxRound(0x46000), FxRound(0x6e000), FxRound(0x23000));
        mCamera->SetTarget(FxRound(0x5000), FxRound(0x5a000), FxRound(0));
        CameraManager &manager = CameraManager::Get();
        Camera *camera = manager.mCameras[manager.mActive];
        data_02211374.mCameraPosition = camera->mPosition;
        data_02211374.mCameraUp = camera->mUp;
        data_02211374.mCameraTarget = camera->mTarget;
        func_01ff8d34(&camera->mPosition, &camera->mUp, &camera->mTarget, &data_02211374.mView);
        data_02211374.mFlags &= ~0xe8;
        manager.mProjection[manager.mActive] = data_02211374.mProjection;
        manager.mView[manager.mActive] = data_02211374.mView;
        manager.mMtxA[manager.mActive] = *func_02032710();
        manager.mMtxB[manager.mActive] = *func_02032604();
        manager.mMtxC[manager.mActive] = *func_020323b8();
        func_0205d090();
        func_0205d0fc(0, 0x10, 0);
        if (mUnk_1ac == 0) {
            func_0205d0fc(1, 0x10, 0);
        }
        mUnk_31 = 0x39;
        break;
    }
    case 0x39:
        if (mWindows[0]->mUnk_062 != 1) {
            return;
        }
        if (mUnk_1ac != 0) {
            func_0205d0fc(1, 0x10, 0);
        }
        mUnk_31 = 0x3a;
        break;
    case 0x3a:
        if (mWindows[0]->mUnk_061 == 1) {
            mUnk_31 = 0x3b;
        }
        break;
    case 0x3b:
        func_02064964();
        if (mUnk_1c == 3) {
            if (func_0206332c()) {
                if (mWindows[1]->GetSelectedId() != 2 && mWindows[1]->GetSelectedId() != 3 &&
                    mWindows[1]->GetSelectedId() != 4) {
                    mWindows[0]->mUnk_061 = 3;
                    mWindows[1]->mUnk_061 = 3;
                    mUnk_31 = 0x3e;
                    return;
                }
            }
        } else if (mUnk_18 == 1) {
            mUnk_31 = 0x3d;
            func_02063b10(1);
            func_02063d10();
            func_020638a8(0);
            return;
        } else if (mUnk_18 == 2) {
            mUnk_31 = 0x3c;
            func_02063b10(1);
            func_02063d10();
            func_020638a8(0);
            return;
        }
        func_020638b8();
        break;
    case 0x3c:
        if (mWindows[0]->mUnk_061 != 4) {
            return;
        }
        mState = 1;
        mUnk_31 = 2;
        func_02063ad8(2);
        break;
    case 0x3d:
        mUnk_31 = 0x3c;
        break;
    case 0x3e:
        if (mWindows[0]->mUnk_061 != 4) {
            return;
        }
        mUnk_14c = 0;
        func_02063e20();
        break;
    }
}

void CharacterEditScene::func_020604e8() {
    switch (mUnk_31) {
    case 0x3f: {
        if (mUnk_158 != 2) {
            mUnk_155[mUnk_154] = 1;
        }
        mWindows[0]->func_0205c74c(8, data_0209f394, (s8) mUnk_155[mUnk_154]);
        mWindows[0]->mCells[(u8) mWindows[0]->GetSelectedId()]->SetAnimation(4);
        mWindows[1]->func_0205c7c8(-1);
        func_02063b10(0);
        func_02063c64();
        func_020637a8();
        func_020637dc();
        func_020638a8(0);
        mCamera->SetPosition(FxRound(0x64000), FxRound(0x64000), FxRound(0x32000));
        mCamera->SetTarget(FxRound(0x5000), FxRound(0x28000), FxRound(0));
        CameraManager &manager = CameraManager::Get();
        Camera *camera = manager.mCameras[manager.mActive];
        data_02211374.mCameraPosition = camera->mPosition;
        data_02211374.mCameraUp = camera->mUp;
        data_02211374.mCameraTarget = camera->mTarget;
        func_01ff8d34(&camera->mPosition, &camera->mUp, &camera->mTarget, &data_02211374.mView);
        data_02211374.mFlags &= ~0xe8;
        manager.mProjection[manager.mActive] = data_02211374.mProjection;
        manager.mView[manager.mActive] = data_02211374.mView;
        manager.mMtxA[manager.mActive] = *func_02032710();
        manager.mMtxB[manager.mActive] = *func_02032604();
        manager.mMtxC[manager.mActive] = *func_020323b8();
        func_0205d090();
        func_0205d0fc(0, 0x11, 0);
        switch (mUnk_1ac) {
        case 0:
            func_0205d0fc(1, 0x11, 0);
            break;
        case 1:
            func_0205d0fc(2, 0x15, 1);
            break;
        case 2:
            func_0205d0fc(3, 0x16, 2);
            break;
        }
        mUnk_31 = 0x40;
        break;
    }
    case 0x40:
        if (mWindows[0]->mUnk_062 != 1) {
            return;
        }
        if (mUnk_1ac != 0) {
            func_0205d0fc(1, 0x11, 0);
        }
        if (mUnk_1ac != 1) {
            func_0205d0fc(2, 0x15, 1);
        }
        if (mUnk_1ac != 2) {
            func_0205d0fc(3, 0x16, 2);
        }
        mUnk_31 = 0x41;
        break;
    case 0x41:
        if (mWindows[0]->mUnk_061 == 1) {
            mUnk_31 = 0x42;
        }
        break;
    case 0x42:
        if (mUnk_1c == 3) {
            if (func_0206332c()) {
                if (mWindows[1]->GetSelectedId() != 5 && mWindows[1]->GetSelectedId() != 6) {
                    mWindows[0]->mUnk_061 = 3;
                    mWindows[1]->mUnk_061 = 3;
                    mUnk_31 = 0x44;
                    return;
                }
            }
        } else if (mUnk_18 == 1) {
            if (mWindows[0]->GetSelectedId() == 2) {
                mUnk_31 = 0x43;
                func_02063b10(1);
                func_02063d10();
                return;
            }
            mUnk_31 = 0x44;
            func_02063b10(1);
            func_02063dac();
            return;
        } else if (mUnk_18 == 2) {
            mUnk_31 = 0x43;
            func_02063b10(1);
            func_02063d10();
            return;
        } else if (InputManager::Get().IsHeld(8) && mWindows[0]->mUnk_061 == 1) {
            mUnk_1b0 = 1;
            func_02063b10(1);
            mWindows[0]->mUnk_061 = 3;
            mUnk_31 = 0x44;
            return;
        }
        func_020638b8();
        break;
    case 0x43:
        if (mWindows[0]->mUnk_061 != 4) {
            return;
        }
        mState = 1;
        mUnk_31 = 2;
        func_02063ad8(2);
        break;
    case 0x44:
        if (mWindows[0]->mUnk_061 != 4) {
            return;
        }
        if (func_02063e20()) {
            return;
        }
        if (mUnk_1b0 != 0) {
            mState = 0xc;
            mUnk_31 = 0x53;
            return;
        }
        if (mWindows[0]->GetSelectedId() == 3) {
            mState = 0xa;
            mUnk_31 = 0x45;
            mUnk_155[mUnk_154] = mWindows[0]->mCursor->mSelection;
            func_02063ad8(1);
            return;
        }
        if (mWindows[0]->GetSelectedId() == 4) {
            mState = 0xb;
            mUnk_31 = 0x4c;
            mUnk_155[mUnk_154] = mWindows[0]->mCursor->mSelection;
            func_02063ad8(1);
            return;
        }
        if (mWindows[0]->GetSelectedId() == 8) {
            mState = 0xc;
            mUnk_31 = 0x53;
        }
        break;
    }
}

void CharacterEditScene::func_02060cd4() {
    switch (mUnk_31) {
    case 0x45:
        mWindows[0]->func_0205c74c(6, data_0209ec5c, -1);
        mWindows[1]->func_0205c7c8(-1);
        func_02063b10(0);
        func_02063c64();
        func_020637a8();
        func_020637dc();
        func_020638a8(1);
        func_020642ac();
        func_02062fe4("sub_bottom_male.nce_ncg1D.bin", "sub_bottom_male_ncl.bin", "sub_bottom_male_nce.bin", "color_bottom_male_ncl.bin");
        func_0205d090();
        func_0205d0fc(0, 0x15, 0);
        if (mUnk_1ac == 0) {
            func_0205d0fc(1, 0x15, 0);
        }
        mUnk_31 = 0x46;
        break;
    case 0x46:
        if (mWindows[0]->mUnk_062 != 1) {
            return;
        }
        if (mUnk_1ac != 0) {
            func_0205d0fc(1, 0x15, 0);
        }
        mUnk_31 = 0x47;
        break;
    case 0x47:
        if (mWindows[0]->mUnk_061 == 1) {
            mUnk_31 = 0x48;
        }
        break;
    case 0x48:
        func_02064964();
        if (mUnk_1c == 3 && func_0206332c()) {
            if (mWindows[1]->GetSelectedId() != 5 && mWindows[1]->GetSelectedId() != 6) {
                mWindows[0]->mUnk_061 = 3;
                mWindows[1]->mUnk_061 = 3;
                mUnk_31 = 0x4b;
                return;
            }
        }
        if (mUnk_18 == 1) {
            mUnk_31 = 0x4a;
            func_02063b10(1);
            func_02063d10();
            func_020638a8(0);
            return;
        } else if (mUnk_18 == 2) {
            mUnk_31 = 0x49;
            func_02063b10(1);
            func_02063d10();
            func_020638a8(0);
            return;
        }
        func_020638b8();
        break;
    case 0x49:
        if (mWindows[0]->mUnk_061 != 4) {
            return;
        }
        mState = 5;
        mUnk_31 = 0x3f;
        func_02063ad8(2);
        break;
    case 0x4a:
        mUnk_31 = 0x49;
        break;
    case 0x4b:
        if (mWindows[0]->mUnk_061 != 4) {
            return;
        }
        mUnk_14c = 0;
        func_02063e20();
        break;
    }
}

void CharacterEditScene::func_02060fd0() {
    switch (mUnk_31) {
    case 0x4c:
        mWindows[0]->func_0205c74c(6, data_0209ec5c, -1);
        mWindows[1]->func_0205c7c8(-1);
        func_02063b10(0);
        func_02063c64();
        func_020637a8();
        func_020637dc();
        func_020638a8(1);
        func_020642ac();
        func_02062fe4("sub_shoes_male.nce_ncg1D.bin", "sub_shoes_male_ncl.bin", "sub_shoes_male_nce.bin", "color_shoes_male_ncl.bin");
        func_0205d090();
        func_0205d0fc(0, 0x16, 0);
        if (mUnk_1ac == 0) {
            func_0205d0fc(1, 0x16, 0);
        }
        mUnk_31 = 0x4d;
        break;
    case 0x4d:
        if (mWindows[0]->mUnk_062 != 1) {
            return;
        }
        if (mUnk_1ac != 0) {
            func_0205d0fc(1, 0x16, 0);
        }
        mUnk_31 = 0x4e;
        break;
    case 0x4e:
        if (mWindows[0]->mUnk_061 == 1) {
            mUnk_31 = 0x4f;
        }
        break;
    case 0x4f:
        func_02064964();
        if (mUnk_1c == 3 && func_0206332c()) {
            if (mWindows[1]->GetSelectedId() != 5 && mWindows[1]->GetSelectedId() != 6) {
                mWindows[0]->mUnk_061 = 3;
                mWindows[1]->mUnk_061 = 3;
                mUnk_31 = 0x52;
                return;
            }
        }
        if (mUnk_18 == 1) {
            mUnk_31 = 0x51;
            func_02063b10(1);
            func_02063d10();
            func_020638a8(0);
            return;
        } else if (mUnk_18 == 2) {
            mUnk_31 = 0x50;
            func_02063b10(1);
            func_02063d10();
            func_020638a8(0);
            return;
        }
        func_020638b8();
        break;
    case 0x50:
        if (mWindows[0]->mUnk_061 != 4) {
            return;
        }
        mState = 5;
        mUnk_31 = 0x3f;
        func_02063ad8(2);
        break;
    case 0x51:
        mUnk_31 = 0x50;
        break;
    case 0x52:
        if (mWindows[0]->mUnk_061 != 4) {
            return;
        }
        mUnk_14c = 0;
        func_02063e20();
        break;
    }
}

void CharacterEditScene::func_020612cc() {
    switch (mUnk_31) {
    case 0x53: {
        mWindows[0]->func_0205c74c(2, data_0209f12c, 0);
        mWindows[0]->mCells[0]->SetAnimation(1);
        func_020637a8();
        func_020637dc();
        func_020638a8(0);
        GXS_SetVisiblePlane(0x14);
        fx32 posZ = FxRound(0x5a000);
        fx32 posY = FxRound(0x6e000);
        Camera *cam = mCamera;
        cam->SetPosition(FxRound(0xb4000), posY, posZ);
        mCamera->SetTarget(FxRound(0x5000), FxRound(0x50000), FxRound(0));
        CameraManager &manager = CameraManager::Get();
        Camera *camera = manager.mCameras[manager.mActive];
        data_02211374.mCameraPosition = camera->mPosition;
        data_02211374.mCameraUp = camera->mUp;
        data_02211374.mCameraTarget = camera->mTarget;
        func_01ff8d34(&camera->mPosition, &camera->mUp, &camera->mTarget, &data_02211374.mView);
        data_02211374.mFlags &= ~0xe8;
        manager.mProjection[manager.mActive] = data_02211374.mProjection;
        manager.mView[manager.mActive] = data_02211374.mView;
        manager.mMtxA[manager.mActive] = *func_02032710();
        manager.mMtxB[manager.mActive] = *func_02032604();
        manager.mMtxC[manager.mActive] = *func_020323b8();
        func_0205d090();
        func_0205d0fc(0, 3, 0);
        mUnk_31 = 0x54;
        mUnk_1b0 = 0;
        mUnk_148 = 3;
        break;
    }
    case 0x54:
        if (mWindows[0]->mUnk_062 != 1) {
            return;
        }
        func_0205d0fc(1, 8, 0);
        func_0205d0fc(2, 9, 1);
        mUnk_31 = 0x55;
        break;
    case 0x55:
        if (mWindows[0]->mUnk_061 == 1) {
            mUnk_31 = 0x56;
        }
        break;
    case 0x56:
        if (mUnk_18 != 1) {
            return;
        }
        if (mWindows[0]->GetSelectedId() == 0) {
            func_02063570(NULL);
            mModel->func_0205461c(0x17, 0);
            data_020d433c.mUnk_14_1 = 1;
            mUnk_31 = 0x59;
            mUnk_32 = 0;
            return;
        }
        if (mWindows[0]->GetSelectedId() == 1) {
            mUnk_31 = 0x57;
        }
        break;
    case 0x57:
        if (mWindows[0]->mUnk_061 != 4) {
            return;
        }
        mState = 1;
        mUnk_31 = 2;
        mUnk_154 = 0;
        func_02063ad8(0);
        break;
    case 0x59:
        if (mWindows[0]->mUnk_061 != 4) {
            return;
        }
        mUnk_32++;
        if (mUnk_32 == 100) {
            FrameTask::Get().func_0203dc34(1, 0x183);
        }
        if (mModel->func_02054710()) {
            mUnk_1d0 = 1;
            mState = 0xd;
            mUnk_31 = 0x5a;
            mUnk_32 = 0;
        }
        break;
    }
}

void CharacterEditScene::func_0206183c() {
    switch (mUnk_31) {
    case 0x5a:
        SpriteManager::Get().StartFade(3);
        mUnk_31 = 0x5b;
        break;
    case 0x5b:
        if (!SpriteManager::Get().IsFading()) {
            mUnk_31 = 0x5c;
        }
        break;
    case 0x5c:
        RequestScene(mUnk_1d0);
        mFinished = TRUE;
        break;
    }
}

void CharacterEditScene::Update() {
    if (mUnk_1a8 != 0) {
        u32 value = ((CharacterModel *) mModel->mUnk_04)->func_02053f84();
        mUnk_1a8 = 0;
        UnkStruct_0205401c *model = mModel;
        if (model != NULL) {
            model->func_02054034();
            delete model;
        }
        CharacterLook look;
        func_02063570(&look);
        mModel = new UnkStruct_0205401c();
        mModel->func_0205411c(0x15, 1, (u32) &look, 0);
        mModel->func_02054740();
        mModel->mUnk_04->mUnk_12 = mUnk_1d2;
        mModel->func_0205461c(0, 1);
        ((CharacterModel *) mModel->mUnk_04)->func_02053fa8(value);
    }
    switch (mState) {
    case 0:
        func_0205d4cc();
        break;
    case 1:
        func_0205d5fc();
        break;
    case 2:
        func_0205e6ac();
        break;
    case 3:
        func_0205e958();
        break;
    case 6:
        func_0205f3c0();
        break;
    case 7:
        func_0205f698();
        break;
    case 8:
        func_0205f970();
        break;
    case 9:
        func_0205fc48();
        break;
    case 4:
        func_0205ff20();
        break;
    case 5:
        func_020604e8();
        break;
    case 10:
        func_02060cd4();
        break;
    case 11:
        func_02060fd0();
        break;
    case 12:
        func_020612cc();
        break;
    case 13:
        func_0206183c();
        break;
    }
    mUnk_18 = mWindows[0]->Update(1);
    mUnk_1c = mWindows[1]->Update(1);
    mUnk_20 = mWindows[2]->Update(1);
    func_02063218();
    UnkStruct_0205401c_04 *data = mModel->mUnk_04;
    u16 angle = data->mUnk_12;
    u16 delta = angle - 0x2cec;
    switch (mUnk_148) {
    case 1:
        data->mUnk_12 = angle + 0xcd;
        break;
    case 2:
        data->mUnk_12 = angle - 0xcd;
        break;
    case 3:
        if (delta > 0x8000) {
            data->mUnk_12 = angle + 0x1000;
            if (delta > (u16) (data->mUnk_12 - 0x2cec)) {
                data->mUnk_12 = 0x2cec;
                mUnk_148 = 0;
            }
        } else {
            data->mUnk_12 = angle - 0x1000;
            if (delta < (u16) (data->mUnk_12 - 0x2cec)) {
                data->mUnk_12 = 0x2cec;
                mUnk_148 = 0;
            }
        }
        break;
    }
    mUnk_1d2 = data->mUnk_12;
}

void CharacterEditScene::func_02061c3c() {
    SpriteManager::Get().func_02026dec(0, 7);
    GX_SetGraphicsMode(1, 0, 1);
    GXS_SetGraphicsMode(0);
    G2_SetBG0Offset(0, 0);
    G2_SetBG0Priority(0);
    G2S_SetBlendBrightness(0x1f, 0);
    G2S_SetBG0Control(0, 1, 0, 1, 0);
    G2S_SetBG0Offset(0, 0);
    G2S_SetBG0Priority(2);
    G2S_SetBG1Control(0, 1, 2, 2, 0);
    G2S_SetBG1Offset(0, 0);
    G2S_SetBG1Priority(1);
    G2S_SetBG2Control(0, 1, 4, 4, 0);
    G2S_SetBG2Offset(0, 0);
    G2S_SetBG2Priority(3);
    G2S_SetBG3Control(0, 1, 6, 7, 0);
    G2S_SetBG3Offset(0, 0);
    G2S_SetBG3Priority(0);
    GXS_SetVisibleWnd(0);
    G2S_SetWndOutsidePlane(0x1f, FALSE);
    G2S_SetWnd0InsidePlane(0x1f, FALSE);
    G2S_SetWnd1InsidePlane(0x1f, FALSE);
    G2S_SetWndOBJInsidePlane(0x1f, FALSE);
    G2S_SetWnd0Position(0, 0, 0, 0);
    G2S_SetWnd1Position(0, 0, 0, 0);
    MI_CpuClearFast((void *) 0x6000000, 0x80000);
    MI_CpuClearFast((void *) 0x6200000, 0x20000);
    mCamera = new Camera();
    mCamera->SetUp(0, 0x1000, 0);
    mCamera->SetClip(0x1000, 0x1388000);
    mCamera->func_02026004(0x2aaa);
    fx32 posZ = FxRound(0x5a000);
    fx32 posY = FxRound(0x6e000);
    Camera *cam = mCamera;
    cam->SetPosition(FxRound(0xb4000), posY, posZ);
    mCamera->SetTarget(FxRound(0x5000), FxRound(0x50000), FxRound(0));
    CameraManager::Get().mActive = 0;
    CameraManager::Get().func_02026504(1, mCamera, 0);
    CameraManager &manager = CameraManager::Get();
    Camera *camera = manager.mCameras[manager.mActive];
    data_02211374.mCameraPosition = camera->mPosition;
    data_02211374.mCameraUp = camera->mUp;
    data_02211374.mCameraTarget = camera->mTarget;
    func_01ff8d34(&camera->mPosition, &camera->mUp, &camera->mTarget, &data_02211374.mView);
    data_02211374.mFlags &= ~0xe8;
    manager.mProjection[manager.mActive] = data_02211374.mProjection;
    manager.mView[manager.mActive] = data_02211374.mView;
    manager.mMtxA[manager.mActive] = *func_02032710();
    manager.mMtxB[manager.mActive] = *func_02032604();
    manager.mMtxC[manager.mActive] = *func_020323b8();
}

void CharacterEditScene::func_02062194() {
    u32 size;
    mWindows[0] = NULL;
    mWindows[0] = new SelectWindow(10, (WindowItemRecord *) data_0209e9f4, -1);
    mWindows[1] = NULL;
    mWindows[1] = new SelectWindow(7, data_0209f5fc, -1);
    mWindows[1]->mUnk_07e = 1;
    mWindows[2] = NULL;
    mWindows[2] = new SelectWindow(3, data_0209facc, -1);
    mCursor = new SelectWindowCursor(0x14, data_0209f864, 0);
    func_020637a8();
    ChangeDir("/Game/2d/CharacterEditor/Bg");
    void *data;
    data = func_02023504("edit_ch_b_bg_00_ncl.bin", 0, 0, 0, &size, 0);
    func_02025c1c(1, 1, 0, data, size, 1);
    data = func_02023504("edit_ch_b_bg_01_ncg.bin", 0, 0, 0, &size, 0);
    func_02025c1c(1, 0xb, 0, data, size, 1);
    data = func_02023504("edit_ch_b_bg_01_nsc.bin", 0, 0, 0, &size, 0);
    func_02025c1c(1, 7, 0, data, size, 1);
    data = func_02023504("edit_ch_b_bg_00_ncl.bin", 0, 0, 0, &size, 0);
    func_02025c1c(1, 1, 0x4000, data, size, 1);
    data = func_02023504("edit_ch_b_bg_00_ncg.bin", 0, 0, 0, &size, 0);
    func_02025c1c(1, 0xd, 0, data, size, 1);
    data = func_02023504("edit_ch_b_bg_00_nsc.bin", 0, 0, 0, &size, 0);
    func_02025c1c(1, 9, 0, data, size, 1);
    ChangeDir("/Game/2D/union_b");
    data = func_02023504("union_b_bg_window_ncl.bin", 0, 0, 0, &size, 0);
    func_02025c1c(1, 1, 0x6000, data, size, 1);
    data = func_02023504("union_b_bg_window_ncg.bin", 0, 0, 0, &size, 0);
    func_02025c1c(1, 0xe, 0, data, size, 1);
    data = func_02023504("union_b_bg_window_b_nsc.bin", 0, 0, 0, &size, 0);
    func_02025c1c(1, 0xa, 0, data, size, 1);
    SpriteScreen &screen1 = SpriteManager::Get().Screen(1);
    screen1.func_02027624(3);
    ChangeDir("/Game/2d/union_b");
    screen1.LoadCharacters("union_b_obj_edit.nce_ncg1D.bin", 0x6000, 0);
    screen1.LoadPalette("union_b_obj_edit_ncl.bin", 0, 0);
    screen1.LoadCharacters("union_b_obj_cockpit.nce_ncg1D.bin", 0xd000, 0);
    screen1.LoadPalette("union_b_obj_cockpit_ncl.bin", 0x80, 0);
    screen1.LoadCharacters("union_b_obj_window.nce_ncg1D.bin", 0x19000, 0);
    screen1.LoadPalette("union_b_obj_window_ncl.bin", 0x1a0, 0);
    mWindows[2]->SetCells(3, data_0209e834);
    mWindows[0]->SetCells(0xd, data_0209e888);
    ChangeDir("/Game/2d/CharacterEditor/Obj");
    screen1.LoadCharacters("edit_ch_b_obj_00.nce_ncg1D.bin", 0xf000, 0);
    const char *pal = "edit_ch_b_obj_00_ncl.bin";
    screen1.LoadPalette(pal, 0xa0, 0);
    mWindows[0]->SetCells(1, data_020b3a28);
    mWindows[1]->SetCells(7, data_020b3a44);
    ChangeDir("/");
    ChangeDir("/Game/2d/CharacterEditor/Obj");
    screen1.LoadCharacters("color_base.nce_ncg1D.bin", 0x14000, 0);
    screen1.LoadPalette("color_base_ncl.bin", 0x120, 0);
    mCells[8] = new CellAnimation("color_base_nce.bin", 0, 1, 1);
    mCells[8]->mCharBase = screen1.func_020276f4(0x14000);
    mCells[8]->mUnk_11 = 9;
    mCells[8]->SetAnimation(0);
    screen1.LoadPalette("color_base_ncl.bin", 0x140, 0);
    mCells[9] = new CellAnimation("color_base_nce.bin", 0, 1, 1);
    mCells[9]->mCharBase = screen1.func_020276f4(0x14000);
    mCells[9]->mUnk_11 = 10;
    mCells[9]->SetAnimation(0);
    ChangeDir("/Game/2D/union_b");
    u8 i = 10;
    do {
        mCells[i] = new CellAnimation("union_b_obj_edit_nce.bin", 0, 1, 1);
        mCells[i]->mCharBase = screen1.func_020276f4(0x6000);
        mCells[i]->mUnk_11 = 0;
        i++;
    } while (i < 16);
    mCells[10]->SetAnimation(0x16);
    mCells[11]->SetAnimation(0x17);
    mCells[12]->SetAnimation(0x12);
    mCells[13]->SetAnimation(0x14);
    mCells[14]->SetAnimation(0x15);
    mCells[15]->SetAnimation(0x15);
    ChangeDir("/");
    ChangeDir("/font/");
    mUnk_140 = func_02023504("mes_chara_edit.mes", 0, 0, 0, 0, 0);
    mUnk_138 = TextManager::Get().BufferSize(0x20, 2);
    mUnk_1c4 = TextManager::Get().BufferSize(0x18, 8);
    u8 j = 0;
    do {
        mTextBuffers[j] = (u32 *) OS_Alloc(mUnk_138);
        MI_CpuClear16(mTextBuffers[j], mUnk_138);
        j++;
    } while (j < 6);
    mUnk_1bc = OS_Alloc(mUnk_1c4);
    MI_CpuClear16(mUnk_1bc, mUnk_1c4);
    mUnk_13c = TextManager::Get().GlyphCount(8);
    mUnk_1c8 = TextManager::Get().GlyphCount(0x18);
    mUnk_134 = OS_Alloc(mUnk_13c * 0xc);
    mUnk_1c0 = OS_Alloc(mUnk_1c8 * 0xc);
    TextManager::Get().Init(mUnk_134, 1, 0, 0x20, 2, 0x21);
    TextManager::Get().Init(mUnk_1c0, 1, 0, 0x18, 8, 0x21);
    TextManager::Get().SetColor(1, 0, 0xf);
    mMessage = new MessageWindow("mes_chara_edit", 1, 0);
    mMessage->func_02065bd4(0x14, 4, 0);
    mMessage->func_02065be4(0xd8);
    mMessage->func_02065890(0, 0, 1);
    func_0205d090();
    ChangeDir("/Story/Map/");
    NitroTexture *texture = new NitroTexture("cut_room.nsbtx");
    NitroModel *model     = new NitroModel();
    model->func_020289e0("cut_room.nsbmd", texture);
    texture->func_02028344();
    ModelAnimData *animData = new ModelAnimData("cut_room_data.bin");
    ModelAnim *anim         = new ModelAnim(animData, model);
    mUnk_150                = model->func_02028b78(anim);
    mUnk_150->func_02028e54(0, 1, 0x1000);
    ChangeDir("/");
    mModel = new UnkStruct_0205401c();
    mModel->func_0205411c(0x15, 1, 0, 0);
    mModel->func_02054740();
    mModel->mUnk_04->mUnk_12 = mUnk_1d2;
    mModel->func_0205461c(0, 1);
    ChangeDir("/");
    SpriteScreen &screen = SpriteManager::Get().Screen(0);
    screen.func_02027624(1);
    ChangeDir("/Game/2d/CharacterEditor/Obj");
    screen.LoadCharacters("nameplate_u_obj_32k.nce_ncg1D.bin", 0, 0);
    screen.LoadPalette("nameplate_u_obj_32k_ncl.bin", 0, 0);
    mUnk_1e4 = new CellAnimation("nameplate_u_obj_32k_nce.bin", 0, 1, 1);
    mUnk_1e4->mCharBase = screen.func_020276f4(0);
    mUnk_1e4->mUnk_11 = 0;
    mUnk_1e4->SetAnimation(0);
    ChangeDir("/");
    mUnk_1dc = TextManager::Get().BufferSize(0x20, 2);
    mUnk_1d4 = OS_Alloc(mUnk_1dc);
    func_02064f80();
    mUnk_1e0 = TextManager::Get().GlyphCount(0x20, 2, 0x21);
    mUnk_1d8 = OS_Alloc(mUnk_1e0 * 0xc);
    TextManager::Get().Init(mUnk_1d8, 0, 0, 0x20, 2, 0x21);
    TextManager::Get().SetColor(0, 0, 0xf);
}

void CharacterEditScene::func_02062fe4(const char *chars, const char *palette, const char *cells, const char *palette2) {
    u8 i = 0;
    do {
        CellAnimation *cell = mCells[i];
        if (cell != NULL) {
            delete cell;
        }
        mCells[i] = NULL;
        i++;
    } while (i <= 7);
    ChangeDir("/Game/2d/CharacterEditor/Obj");
    SpriteScreen &screen = SpriteManager::Get().Screen(1);
    screen.LoadCharacters(chars, 0x15000, 0);
    screen.LoadPalette(palette, 0x160, 0);
    u8 j = 0;
    do {
        if (mUnk_160 == NULL) {
            mCells[j] = new CellAnimation(cells, 0, 1, 1);
        } else {
            mCells[j] = new CellAnimation(cells, mUnk_160[j], 1, 1);
        }
        mCells[j]->mCharBase = screen.func_020276f4(0x15000);
        mCells[j]->mUnk_11 = 0xb;
        j++;
    } while (j < mUnk_15d);
    screen.LoadPalette(palette2, 0x120, 0);
    if (mState == 7) {
        screen.LoadPalette("color_hair_male_ncl.bin", 0x140, 0);
    } else if (mState == 9) {
        screen.LoadPalette("color_glasses_len_ncl.bin", 0x140, 0);
    }
    ChangeDir("/");
    mUnk_14c = 1;
}

void CharacterEditScene::func_02063218() {
    func_0205d2b4();
    mWindows[0]->Draw();
    mWindows[1]->Draw();
    mWindows[2]->Draw();
    func_020645fc();
    if (mUnk_150 != NULL) {
        MtxFx43 mtx = data_0220e86c;
        VecFx32 b;
        VecFx32 a;
        b.x = 0x1000;
        b.y = 0x1000;
        b.z = 0x1000;
        a.x = 0;
        a.y = -0x6000;
        a.z = 0;
        func_01ffcf64(&mtx, &a);
        mUnk_150->func_02028d80();
        mUnk_150->func_02029174(&mtx, &b);
    }
    if (mModel != NULL) {
        mModel->func_02054568();
    }
    func_02064ed8();
    if (mUnk_1e4 != NULL) {
        mUnk_1e4->Update();
        mUnk_1e4->func_0202a3ec(0, data_020d43f0[0].mUnk_1a_8, 0x80, 0x60, 0);
    }
}

BOOL CharacterEditScene::func_0206332c() {
    BOOL result = FALSE;
    if (mWindows[1]->GetSelectedId() == 1) {
        result = TRUE;
        mWindows[1]->mButtons[1].mEnabled = 1;
        mWindows[1]->mButtons[2].mEnabled = 0;
        mWindows[1]->mButtons[3].mEnabled = 0;
        mWindows[1]->mButtons[4].mEnabled = 0;
        mWindows[1]->mButtons[5].mEnabled = 0;
        mWindows[1]->mButtons[6].mEnabled = 0;
    } else if (mWindows[1]->GetSelectedId() == 3) {
        result = TRUE;
        mWindows[1]->mButtons[1].mEnabled = 0;
        mWindows[1]->mButtons[2].mEnabled = 1;
        mWindows[1]->mButtons[3].mEnabled = 1;
        mWindows[1]->mButtons[4].mEnabled = 1;
        mWindows[1]->mButtons[5].mEnabled = 0;
        mWindows[1]->mButtons[6].mEnabled = 0;
    } else if (mWindows[1]->GetSelectedId() == 2) {
        result = TRUE;
        mWindows[1]->mButtons[1].mEnabled = 0;
        mWindows[1]->mButtons[2].mEnabled = 1;
        mWindows[1]->mButtons[3].mEnabled = 1;
        mWindows[1]->mButtons[4].mEnabled = 1;
        mWindows[1]->mButtons[5].mEnabled = 0;
        mWindows[1]->mButtons[6].mEnabled = 0;
    } else if (mWindows[1]->GetSelectedId() == 4) {
        result = TRUE;
        mWindows[1]->mButtons[1].mEnabled = 0;
        mWindows[1]->mButtons[2].mEnabled = 1;
        mWindows[1]->mButtons[3].mEnabled = 1;
        mWindows[1]->mButtons[4].mEnabled = 1;
        mWindows[1]->mButtons[5].mEnabled = 0;
        mWindows[1]->mButtons[6].mEnabled = 0;
    } else if (mWindows[1]->GetSelectedId() == 6) {
        result = TRUE;
        mWindows[1]->mButtons[1].mEnabled = 0;
        mWindows[1]->mButtons[2].mEnabled = 0;
        mWindows[1]->mButtons[3].mEnabled = 0;
        mWindows[1]->mButtons[4].mEnabled = 0;
        mWindows[1]->mButtons[5].mEnabled = 1;
        mWindows[1]->mButtons[6].mEnabled = 1;
    } else if (mWindows[1]->GetSelectedId() == 5) {
        result = TRUE;
        mWindows[1]->mButtons[1].mEnabled = 0;
        mWindows[1]->mButtons[2].mEnabled = 0;
        mWindows[1]->mButtons[3].mEnabled = 0;
        mWindows[1]->mButtons[4].mEnabled = 0;
        mWindows[1]->mButtons[5].mEnabled = 1;
        mWindows[1]->mButtons[6].mEnabled = 1;
    }
    return result;
}

void CharacterEditScene::func_02063570(CharacterLook *look) {
    if (look == NULL) {
        look = (CharacterLook *) data_020d4404;
    }
    look->mUnk_06_0 = mLists[0].mValues[mLists[0].mIndex];
    look->mUnk_00_1 = mLists[1].mValues[mLists[1].mIndex];
    look->mUnk_00_4 = mLists[2].mValues[mLists[2].mIndex];
    look->mUnk_00_7 = mLists[3].mValues[mLists[3].mIndex];
    look->mUnk_00_10 = mLists[4].mValues[mLists[4].mIndex];
    look->mUnk_00_13 = mLists[5].mValues[mLists[5].mIndex];
    look->mUnk_02_0 = mLists[6].mValues[mLists[6].mIndex];
    look->mUnk_02_3 = mLists[7].mValues[mLists[7].mIndex];
    look->mUnk_02_6 = mLists[8].mValues[mLists[8].mIndex];
    look->mUnk_02_9 = mLists[9].mValues[mLists[9].mIndex];
    look->mUnk_02_12 = mLists[10].mValues[mLists[10].mIndex];
    look->mUnk_04_0 = mLists[11].mValues[mLists[11].mIndex];
    look->mUnk_04_3 = mLists[12].mValues[mLists[12].mIndex];
    look->mUnk_04_8 = mLists[13].mValues[mLists[13].mIndex];
    look->mUnk_04_14 = mLists[14].mValues[mLists[14].mIndex];
    look->mUnk_04_11 = mLists[15].mValues[mLists[15].mIndex];
    look->mUnk_06_8 = data_020d43f0[0].mUnk_1a_8;
}

void CharacterEditScene::func_020637a8() {
    u8 i = 0;
    do {
        SelectWindow *w = mWindows[1];
        w->mCellPos[i].x = 0x80;
        w->mCellPos[i].y = 0x60;
        i++;
    } while (i < 7);
}

void CharacterEditScene::func_020637dc() {
    mWindows[1]->mButtons[0].mEnabled = 1;
    u8 i = 1;
    do {
        mWindows[1]->mButtons[i].mEnabled = 0;
        i++;
    } while (i < 7);
    switch (mState) {
    case 3:
    case 6:
    case 7:
    case 8:
    case 9:
        mWindows[1]->mButtons[1].mEnabled = 1;
        break;
    case 4:
        mWindows[1]->mButtons[2].mEnabled = 1;
        mWindows[1]->mButtons[3].mEnabled = 1;
        mWindows[1]->mButtons[4].mEnabled = 1;
        break;
    case 5:
    case 10:
    case 11:
        mWindows[1]->mButtons[5].mEnabled = 1;
        mWindows[1]->mButtons[6].mEnabled = 1;
        break;
    case 12:
        mWindows[1]->mButtons[0].mEnabled = 0;
        break;
    }
}

void CharacterEditScene::func_020638a8(u32 v) {
    mWindows[0]->mButtons[13].mEnabled = v;
    mUnk_14c = v;
}

void CharacterEditScene::func_020638b8() {
    if (mUnk_148 != 3) {
        mUnk_148 = 0;
    }
    SelectWindow *w = mWindows[0];
    SelectWindowCursor *cur = w->mCursor;
    s8 id = -1;
    if (cur->mUnk_05 != -1) {
        id = cur->mItems[cur->mUnk_05].mId;
    }
    if (id != -1) {
        switch (id) {
        case 9:
            mUnk_148 = 1;
            break;
        case 10:
            mUnk_148 = 2;
            break;
        }
        return;
    }
    w->mCells[9]->SetAnimation(0xc);
    mWindows[0]->mCells[10]->SetAnimation(0xe);
    if (InputManager::Get().IsDown(0x200)) {
        mUnk_148 = 1;
        mWindows[0]->mCells[9]->SetAnimation(0xd);
        return;
    }
    if (InputManager::Get().IsDown(0x100)) {
        mUnk_148 = 2;
        mWindows[0]->mCells[10]->SetAnimation(0xf);
        return;
    }
    if (mUnk_18 == 3) {
        SelectWindowCursor *c = mWindows[0]->mCursor;
        s8 id2 = -1;
        if (c->mSelection != -1) {
            id2 = c->mItems[c->mSelection].mId;
        }
        if (id2 == 0xb) {
            mUnk_148 = 3;
        }
    }
}

void CharacterEditScene::func_02063a6c(u32 i, u32 v) {
    mWindows[0]->mCursor->mItems[i].mFlags = v;
}

void CharacterEditScene::func_02063a88(u32 i, u32 a, u32 b) {
    WindowItem *item = &mWindows[0]->mCursor->mItems[i];
    item->mSlideIn = a;
    item->mSlideOut = b;
}

void CharacterEditScene::func_02063aa8(u32 i, u32 a, u32 b, u32 c, u32 d) {
    WindowItem *item = &mCursor->mItems[i];
    item->mNeighbors[0] = a;
    item->mNeighbors[1] = b;
    item->mNeighbors[2] = *(u8 *) &c;
    item->mNeighbors[3] = *(u8 *) &d;
}

void CharacterEditScene::func_02063ad8(u32 v) {
    mUnk_158 = v;
    switch (v) {
    case 0:
        break;
    case 1:
        mUnk_154++;
        break;
    case 2:
        mUnk_154--;
        break;
    }
}

void CharacterEditScene::func_02063b10(u32 arg) {
    u8 count;
    switch (mState) {
    case 1:
        count = 5;
        break;
    case 3:
        count = 5;
        break;
    case 5:
        count = 3;
        break;
    case 2:
    case 4:
    case 6:
    case 7:
    case 8:
    case 9:
    case 10:
    case 11:
        count = 1;
        break;
    default:
        return;
    }
    u8 i = 0;
    do {
        if (i == 4) {
            switch (mUnk_1b4) {
            case 0:
                func_02063a6c(i, 1);
                func_02063a88(i, 3, 4);
                break;
            case 1:
                func_02063a6c(i, 0x8000);
                func_02063a88(i, 0, 0);
                break;
            }
        } else {
            func_02063a88(i, 3, 4);
        }
        i++;
    } while (i < count);
    if (arg == 0) {
        u8 j = 2;
        do {
            mWindows[0]->mButtons[j].mEnabled = 0;
            j++;
        } while (j <= 7);
    }
}

void CharacterEditScene::func_02063c64() {
    mUnk_1ac = -1;
    WindowItem *items = mWindows[0]->mCursor->mItems;
    if (mUnk_158 == 1) {
        items[0].mSlideIn = 0;
        mWindows[0]->mButtons[items[0].mId].mEnabled = 1;
        mUnk_1ac = 0;
    } else if (mUnk_158 == 2) {
        items[mUnk_155[mUnk_154]].mSlideIn = 0;
        mWindows[0]->mButtons[items[mUnk_155[mUnk_154]].mId].mEnabled = 1;
        mUnk_1ac = mUnk_155[mUnk_154];
    }
}

// non-matching: 10 words, register allocation only (the window pointer lands in lr and the stored value in ip, the
// original has them the other way round).
void CharacterEditScene::func_02063d10() {
    WindowItem *items = mWindows[0]->mCursor->mItems;
    items[0].mSlideOut = 2;
    if (mUnk_154 == 2) {
        u8 id = items[0].mId;
        SelectWindow *w = mWindows[0];
        w->mCellPos[id].x = 0x8c;
        w->mCellPos[id].y = 0x1a;
    }
    u8 prev = mUnk_155[mUnk_154 - 1];
    SelectWindow *w = mWindows[0];
    w->mUnk_068[(u8) items[0].mId] = 1;
    w->mUnk_0d8 = 0x87;
    w->mUnk_0da = prev * 0x1c + 0x1a;
    mWindows[0]->mCells[(u8) items[0].mId]->SetAnimation(3);
}

void CharacterEditScene::func_02063dac() {
    SelectWindowCursor *cur = mWindows[0]->mCursor;
    s8 sel = cur->mSelection;
    WindowItem *items = cur->mItems;
    if (items[sel].mId == 8) {
        return;
    }
    items[sel].mSlideOut = 1;
    SelectWindow *w = mWindows[0];
    w->mUnk_068[(u8) items[sel].mId] = 1;
    w->mUnk_0d8 = 0x87;
    w->mUnk_0da = 0x1a;
    mWindows[0]->mCells[(u8) items[sel].mId]->SetAnimation(3);
}

BOOL CharacterEditScene::func_02063e20() {
    if (mWindows[1]->GetSelectedId() == 1) {
        mState = 3;
        mUnk_31 = 0x16;
        mUnk_154 = 0;
        mUnk_155[mUnk_154] = 2;
        if (mUnk_1b4 == 1) {
            mUnk_155[mUnk_154] = 1;
        }
        mUnk_154++;
        func_02063ad8(0);
        return TRUE;
    }
    if (mWindows[1]->GetSelectedId() == 2 || mWindows[1]->GetSelectedId() == 3 || mWindows[1]->GetSelectedId() == 4) {
        mState = 4;
        mUnk_31 = 0x38;
        mUnk_154 = 0;
        mUnk_155[mUnk_154] = 3;
        if (mUnk_1b4 == 1) {
            mUnk_155[mUnk_154] = 2;
        }
        mUnk_154++;
        func_02063ad8(0);
        return TRUE;
    }
    if (mWindows[1]->GetSelectedId() == 5 || mWindows[1]->GetSelectedId() == 6) {
        mState = 5;
        mUnk_31 = 0x3f;
        mUnk_154 = 0;
        mUnk_155[mUnk_154] = 4;
        if (mUnk_1b4 == 1) {
            mUnk_155[mUnk_154] = 3;
        }
        mUnk_154++;
        func_02063ad8(0);
        return TRUE;
    }
    return FALSE;
}


void CharacterEditScene::func_02064024() {
    u8 count = 0;
    u8 value = 0;
    u8 i = 0;
    do {
        switch (i) {
        case 0:
            count = 5;
            value = SAVE_LOOK->mUnk_06_0;
            break;
        case 1:
            count = 4;
            value = SAVE_LOOK->mUnk_00_1;
            break;
        case 2:
            count = 6;
            value = SAVE_LOOK->mUnk_00_4;
            break;
        case 3:
            count = 8;
            value = SAVE_LOOK->mUnk_00_7;
            break;
        case 4:
            count = 6;
            value = SAVE_LOOK->mUnk_00_10;
            break;
        case 5:
            count = 6;
            value = SAVE_LOOK->mUnk_00_13;
            break;
        case 6:
            count = 6;
            value = SAVE_LOOK->mUnk_02_0;
            break;
        case 7:
            count = 5;
            value = SAVE_LOOK->mUnk_02_3;
            break;
        case 8:
            count = 4;
            value = SAVE_LOOK->mUnk_02_6;
            break;
        case 9:
            count = 5;
            value = SAVE_LOOK->mUnk_02_9;
            break;
        case 10:
            count = 6;
            value = SAVE_LOOK->mUnk_02_12;
            break;
        case 11:
            count = 6;
            value = SAVE_LOOK->mUnk_04_0;
            break;
        case 12:
            count = 6;
            value = SAVE_LOOK->mUnk_04_3;
            break;
        case 13:
            count = 6;
            value = SAVE_LOOK->mUnk_04_8;
            break;
        case 14:
            count = 3;
            value = SAVE_LOOK->mUnk_04_14;
            break;
        case 15:
            count = 6;
            value = SAVE_LOOK->mUnk_04_11;
            break;
        }
        mLists[i].mCount = 0;
        u8 j = 0;
        s32 mask = 0xff;
        do {
            if ((mask >> j) & 1) {
                mLists[i].mValues[mLists[i].mCount] = j;
                mLists[i].mCount++;
                if (j == value) {
                    mLists[i].mIndex = j;
                }
            }
            j++;
        } while (j < count && mLists[i].mCount < 8);
        i++;
    } while (i < 16);
}

void CharacterEditScene::func_020642ac() {
    mUnk_15e = 0;
    mUnk_15f = 0;
    func_02063aa8(10, 7, 0x10, 9, 0xb);
    func_02063aa8(11, 7, 0x11, 10, 0xc);
    func_02063aa8(12, 5, 0x12, 11, 0xd);
    switch (mState) {
    case 2:
        mUnk_15a = 0;
        mUnk_15d = 1;
        mUnk_160 = NULL;
        mUnk_15b = mLists[0].mIndex;
        mUnk_15e = mLists[0].mCount;
        func_02063aa8(10, 0, 0, 9, 0xb);
        func_02063aa8(11, 0, 0, 10, 0xc);
        func_02063aa8(12, 0, 0, 11, 0xd);
        break;
    case 6:
        mUnk_15a = mLists[1].mIndex;
        mUnk_15d = mLists[1].mCount;
        mUnk_160 = mLists[1].mValues;
        mUnk_15b = mLists[2].mIndex;
        mUnk_15e = mLists[2].mCount;
        break;
    case 7:
        mUnk_15a = mLists[3].mIndex;
        mUnk_15d = mLists[3].mCount;
        mUnk_160 = mLists[3].mValues;
        mUnk_15c = mLists[4].mIndex;
        mUnk_15f = mLists[4].mCount;
        break;
    case 8:
        mUnk_15a = mLists[5].mIndex;
        mUnk_15d = mLists[5].mCount;
        mUnk_160 = mLists[5].mValues;
        mUnk_15b = mLists[6].mIndex;
        mUnk_15e = mLists[6].mCount;
        break;
    case 9:
        mUnk_15a = mLists[7].mIndex;
        mUnk_15d = mLists[7].mCount;
        mUnk_160 = mLists[7].mValues;
        mUnk_15b = mLists[8].mIndex;
        mUnk_15e = mLists[8].mCount;
        mUnk_15c = mLists[9].mIndex;
        mUnk_15f = mLists[9].mCount;
        break;
    case 4:
        mUnk_15a = mLists[10].mIndex;
        mUnk_15d = mLists[10].mCount;
        mUnk_160 = mLists[10].mValues;
        mUnk_15b = mLists[11].mIndex;
        mUnk_15e = mLists[11].mCount;
        break;
    case 10:
        mUnk_15a = mLists[12].mIndex;
        mUnk_15d = mLists[12].mCount;
        mUnk_160 = mLists[12].mValues;
        mUnk_15b = mLists[13].mIndex;
        mUnk_15e = mLists[13].mCount;
        break;
    case 11:
        mUnk_15a = mLists[14].mIndex;
        mUnk_15d = mLists[14].mCount;
        mUnk_160 = mLists[14].mValues;
        mUnk_15b = mLists[15].mIndex;
        mUnk_15e = mLists[15].mCount;
        break;
    }
    u8 selected = mUnk_15a;
    mUnk_159 = selected;
    mCursor->mSelection = selected;
    mCells[12]->SetAnimation(0x12);
    WindowItem *items = mCursor->mItems;
    u8 i = 0;
    do {
        if (i < mUnk_15d) {
            items[i].mFlags = 8;
        } else {
            items[i].mFlags = 0x8000;
        }
        i++;
    } while (i < 8);
    i = 0;
    do {
        if (i < mUnk_15e) {
            items[i + 8].mFlags = 8;
        } else {
            items[i + 8].mFlags = 0x8000;
        }
        if (i < mUnk_15f) {
            items[i + 14].mFlags = 8;
        } else {
            items[i + 14].mFlags = 0x8000;
        }
        i++;
    } while (i < 6);
}

void CharacterEditScene::func_020645fc() {
    if (mUnk_14c == 0) {
        return;
    }
    u8 i = 0;
    do {
        if (mCells[i] != NULL) {
            mCells[i]->Update();
            mCells[i]->Draw(1, data_0209e7f4[i].x, data_0209e7f4[i].y, 1);
        }
        i++;
    } while (i < mUnk_15d);
    u8 j = 8;
    do {
        if (mCells[j] != NULL) {
            mCells[j]->Update();
            switch (j - 8) {
            case 0:
                if (mUnk_15e != 0) {
                    mCells[j]->func_0202a3ec(1, mUnk_15e - 1, data_0209e7f4[j].x, data_0209e7f4[j].y, 1);
                }
                break;
            case 1:
                if (mUnk_15f != 0) {
                    mCells[j]->func_0202a3ec(1, mUnk_15f - 1, data_0209e7f4[j].x, data_0209e7f4[j].y, 1);
                }
                break;
            case 2:
            case 3:
                if (mState == 9) {
                    mCells[j]->Draw(1, data_0209e7f4[j].x, data_0209e7f4[j].y, 1);
                }
                break;
            case 4: {
                u8 v = mUnk_159;
                if (v < 8) {
                    mCells[j]->Draw(1, (s16) (v % 3 * 32 + 0x91), (s16) (v / 3 * 32 + 0x34), 0);
                } else if (v < 0xe) {
                    mCells[j]->Draw(1, (s16) ((v - 8) * 15 + 0x90), 0x86, 0);
                } else {
                    mCells[j]->Draw(1, (s16) ((v - 0xe) * 15 + 0x90), 0x96, 0);
                }
                break;
            }
            case 5: {
                u8 v = mUnk_15a;
                mCells[j]->Draw(1, (s16) (data_0209e7f4[j].x + v % 3 * 32), (s16) (data_0209e7f4[j].y + v / 3 * 32), 0);
                break;
            }
            case 6:
                if (mUnk_15e != 0) {
                    mCells[j]->Draw(1, (s16) (data_0209e7f4[j].x + mUnk_15b * 15), data_0209e7f4[j].y, 0);
                }
                break;
            case 7:
                if (mUnk_15f != 0) {
                    mCells[j]->Draw(1, (s16) (data_0209e7f4[j].x + mUnk_15c * 15), data_0209e7f4[j].y, 0);
                }
                break;
            }
        }
        j++;
    } while (j < 16);
}

void CharacterEditScene::func_02064964() {
    if (mWindows[0]->mUnk_061 != 1) {
        return;
    }
    u32 result = mCursor->func_0202f468(1);
    s8 sel = mCursor->mSelection;
    if (mUnk_159 != sel) {
        mUnk_159 = sel;
        if (mUnk_159 < 8) {
            mCells[12]->SetAnimation(0x12);
        } else {
            mCells[12]->SetAnimation(0x13);
        }
        return;
    }
    if (result == 3) {
        func_020649e4();
    }
}

void CharacterEditScene::func_020649e4() {
    u8 v = mUnk_159;
    if (v < 8) {
        mUnk_15a = v;
    } else if (v < 0xe) {
        mUnk_15b = v - 8;
    } else {
        mUnk_15c = v - 0xe;
    }
    switch (mState) {
    case 2:
        if (mUnk_159 >= 8) {
            mLists[0].mIndex = mUnk_15b;
        }
        break;
    case 6:
        if (mUnk_159 < 8) {
            mLists[1].mIndex = mUnk_15a;
        } else {
            mLists[2].mIndex = mUnk_15b;
        }
        break;
    case 7:
        if (mUnk_159 >= 8) {
            mLists[4].mIndex = mUnk_15c;
        } else {
            mLists[3].mIndex = mUnk_15a;
            mLists[5].mIndex = 0;
        }
        break;
    case 8:
        if (mUnk_159 < 8) {
            mLists[5].mIndex = mUnk_15a;
        } else {
            mLists[6].mIndex = mUnk_15b;
        }
        break;
    case 9:
        if (mUnk_159 < 8) {
            mLists[7].mIndex = mUnk_15a;
        } else if (mUnk_159 < 0xe) {
            mLists[8].mIndex = mUnk_15b;
        } else {
            mLists[9].mIndex = mUnk_15c;
        }
        break;
    case 4:
        if (mUnk_159 < 8) {
            mLists[10].mIndex = mUnk_15a;
        } else {
            mLists[11].mIndex = mUnk_15b;
        }
        break;
    case 10:
        if (mUnk_159 < 8) {
            mLists[12].mIndex = mUnk_15a;
        } else {
            mLists[13].mIndex = mUnk_15b;
        }
        break;
    case 11:
        if (mUnk_159 < 8) {
            mLists[14].mIndex = mUnk_15a;
        } else {
            mLists[15].mIndex = mUnk_15b;
        }
        break;
    }
    func_02064b58();
    mUnk_1a8 = 1;
    mUnk_1b8 = 1;
}

void CharacterEditScene::func_02064b58() {
    u8 i = 0;
    do {
        mModel->mUnk_04->mUnk_65[i] = mLists[i].mValues[mLists[i].mIndex];
        i++;
    } while (i < 16);
    mModel->mUnk_04->mUnk_75 = data_020d43f0[0].mUnk_1a_8;
}

void CharacterEditScene::func_02064bb8() {
    u8 i = 0;
    if (mUnk_1b4 == 1) {
        i = 3;
    }
    do {
        mLists[i].mIndex = MATH_Rand32(&data_020d42e8, mLists[i].mCount);
        i++;
    } while (i < 16);
    if (MATH_Rand32(&data_020d42e8, 2) != 0) {
        mLists[7].mIndex = 0;
    }
    if (MATH_Rand32(&data_020d42e8, 2) != 0) {
        mLists[5].mIndex = 0;
    }
}

void CharacterEditScene::func_02064cd4(u32 flag) {
    u8 i = 0;
    do {
        if (flag != 0) {
            func_02063a88(i, 0, 0);
        } else if (i <= 4) {
            func_02063a88(i, 3, 4);
            if (mUnk_1b4 == 1 && i == 4) {
                func_02063a88(i, 0, 0);
            }
        } else {
            func_02063a88(i, 1, 2);
        }
        i++;
    } while (i < 10);
}

void CharacterEditScene::func_02064d7c() {
    TextManager &text = TextManager::Get();
    s16 a;
    s16 b;
    b = 0;
    a = 0;
    MessageFile *file = (MessageFile *) mUnk_140;
    text.Print(file->GetText(0x1c), file->GetLength(0x1c), mUnk_1bc, 0x18, 8, &a, &b, NULL, 0, 2, 0x21, 0x10, 0, 0,
               1, 1, 1);
    func_02025c1c(SpriteManager::Get().Screen(1).mBgIndex, 4, 0x1000, mUnk_1bc, mUnk_1c4, 0);
    mUnk_1cc = 1;
}

void CharacterEditScene::func_02064ed8() {
    SpriteManager::Get().Screen(0).func_02027778(mUnk_1d8, (u8) mUnk_1e0, 0x16, 0xaa, 0, 0x100, 0xf, 0);
}

void CharacterEditScene::func_02064f80() {
    MI_CpuClear16(mUnk_1d4, mUnk_1dc);
}

void CharacterEditScene::func_02064f98() {
    s16 a = 0;
    s16 b = 0;
    TextManager &text = TextManager::Get();
    text.mUnk_5b = 1;
    TextManager::Get().Print(data_020d43f0[0].mName, 0x18, mUnk_1d4, 0x20, 2, &a, &b, NULL, 0, 2, 0x21, 0, 0, 0, 0, 1,
                             1);
    TextManager::Get().mUnk_5b = 0;
    func_02025c1c(SpriteManager::Get().Screen(0).mBgIndex, 4, 0x2000, mUnk_1d4, mUnk_1dc, 0);
}
