#include "Text/TextManager.hpp"
#include "System/TouchManager.hpp"
#include "Graphics/SpriteManager.hpp"
#include "System/InputManager.hpp"
#include "UI/SelectWindow.hpp"

// Defined among the headers: where they sit among the singletons' objects sets the data layout.
static const u8 data_ov000_02226218[2] = {2, 0x10};
static const u8 data_ov000_02226224[6] = {3, 2, 1, 0, 5, 4};
static const u8 data_ov000_0222621c[6] = {0, 2, 3, 4, 1, 5};
static const u32 data_ov000_02226234[2] = {0x21, 0x44};
static const u8 data_ov000_02226214[2] = {0x10, 0x1c};
static const u8 data_ov000_0222622c[6] = {0, 4, 1, 2, 3, 5};
static const WindowItemRecord data_ov000_0222623c[1] = {
    {0, 0x2, 225, 170, 30, 20, 0, {-1, -1, -1, -1}, 0, 1},
};
static const WindowCellRecord data_ov000_02226258[1] = {
    {"/Game/2D/union_b/union_b_obj_cockpit_nce.bin", 2, 0xd000, 12, 0, 1, {2, 3, 3, 0}},
};

#include "Scene/AttrSelScene.hpp"
#include "Game/SaveData.hpp"
#include "Graphics/CellAnimation.hpp"
#include "UI/MessageWindow.hpp"
#include <nitro/gx.h>
#include <nitro/os.h>
#include "Graphics/Display.hpp"
#include "System/FileSystem.hpp"
#include <nitro/mi.h>
#include <stdio.h>
#include "System/FrameUpdate.hpp"
#include <nitro/fx.h>
#include "System/SoundManager.hpp"

void operator delete(void *ptr);
extern "C" u32 FX_Atan2Idx(s32 x, s32 y);

AttrSelScene::AttrSelScene() :
    SceneBase(1000, 0) {
    mState = 0;
    mAttrib = data_ov000_0222622c[gSaveData[0].mUnk_1a_8];
    mPulseState = 0;
    mPulseAlpha = 0;
    mUnk_10_0 = 0;
    mPulseFrames = 0;
    mFade = 0;
    mTransitionState = 4;
    mBlendAlphaLowByte = 0;
    mBlendAlphaHighByte = 0;
    mPulseCount = 0;
    mMessages = NULL;
    mMessageWindow = NULL;
    mSelectWindow = NULL;
    for (u8 i = 0; i < 8; i++) {
        mCells[i] = NULL;
    }
    mGlyphCount[1] = 0;
    mGlyphCount[0] = 0;
    mBufferSize[1] = 0;
    mBufferSize[0] = 0;
    mGlyphBuffer[1] = NULL;
    mGlyphBuffer[0] = NULL;
    mTextBuffer[1] = NULL;
    mTextBuffer[0] = NULL;
    InitDisplay();
    LoadGraphics();
}

AttrSelScene::~AttrSelScene() {
    G2_SetBG3Offset(0, 0);
    OS_Free(mTextBuffer[1]);
    OS_Free(mTextBuffer[0]);
    OS_Free(mGlyphBuffer[1]);
    OS_Free(mGlyphBuffer[0]);
    operator delete(mMessages);
    delete mSelectWindow;
    delete mMessageWindow;
    for (u8 i = 0; i < 8; i++) {
        if (mCells[i] != NULL) {
            delete mCells[i];
        }
    }
}

void AttrSelScene::Update() {
    switch (mState) {
        case 0:
            SpriteManager::Get().StartFade(2);
            mState++;
        case 1:
            if (!SpriteManager::Get().IsFading()) {
                mState++;
            }
            break;
        case 2: {
            u32 event = 0;
            if ((u8) mSelectWindow->Update(TRUE) == 2) {
                event = 1;
            } else {
                if (TouchManager::Get().mPressed) {
                    if (Distance(TouchManager::Get().mX, TouchManager::Get().mY, 0x80, 0x60) < 0x50) {
                        int x = FX32_CONST((float) (u32) TouchManager::Get().mX - 128.0f);
                        int y = FX32_CONST((float) (u32) TouchManager::Get().mY - 96.0f);
                        u32 sector = (FX_Atan2Idx(x, y) + 0x1555) / 10922;
                        if (sector > 5) {
                            sector = 0;
                        }
                        u8 attr = data_ov000_02226224[sector];
                        if (mAttrib == attr) {
                            event = 3;
                            SoundManager::Get().StartSeqArc(0);
                        } else {
                            event = 2;
                            SoundManager::Get().StartSeqArc(5);
                        }
                        mAttrib = attr;
                    }
                }
                if (event == 0) {
                    if (InputManager::Get().IsTrigger(KEY_UP | KEY_RIGHT)) {
                        mAttrib++;
                        event = 2;
                        if (mAttrib >= 6) {
                            mAttrib = 0;
                        }
                        SoundManager::Get().StartSeqArc(5);
                    } else if (InputManager::Get().IsTrigger(KEY_DOWN | KEY_LEFT)) {
                        event = 2;
                        if (mAttrib == 0) {
                            mAttrib = 6;
                        }
                        mAttrib--;
                        SoundManager::Get().StartSeqArc(5);
                    } else if (InputManager::Get().IsHeld(1)) {
                        SoundManager::Get().StartSeqArc(0);
                        event = 3;
                    }
                }
            }
            switch (event) {
                case 1:
                    SpriteManager::Get().StartFade(3);
                    mRequest = 1;
                    mState = 4;
                    break;
                case 2:
                    if (mTransitionState == 3) {
                        LoadSubBg();
                    }
                    mTransitionState = 2;
                    mCells[6]->mFrameIndex = mAttrib;
                    mPulseState = 0;
                    mPulseAlpha = 0;
                    break;
                case 3:
                    mPulseState = 2;
                    mState++;
                    break;
            }
            break;
        }
        case 3:
            if (mPulseState != 2) {
                gSaveData[0].mUnk_1a_8 = data_ov000_0222621c[mAttrib];
                SpriteManager::Get().StartFade(3);
                mRequest = 2;
                mState++;
            }
            break;
        case 4:
            if (!SpriteManager::Get().IsFading()) {
                RequestScene(mRequest);
                mFinished = 1;
            }
            break;
    }
    TransitionStep();
    PulseBlend();
    DrawText();
    Draw();
}

void AttrSelScene::InitDisplay() {
    GX_SetDispSelect(1);
    SpriteManager::Get().SetupMode(1, 1);
    GX_SetGraphicsMode(1, 3, 0);
    G2_SetBG0Control(0, 1, 0, 1, 0);
    G2_SetBG3Control(1, 0, 2, 4, 0);
    G2_SetBG0Priority(3);
    G2_SetBG3Priority(2);
    G2S_SetBG0Control(0, 1, 0, 1, 0);
    G2S_SetBG0Priority(3);
    GX_SetVisiblePlane(0x19);
    GXS_SetVisiblePlane(0x11);
    G2_SetBlendAlpha(8, 1, 0x1f, 0);
    G2S_SetBlendAlpha(0, 1, 0x1f, 0);
}

void AttrSelScene::LoadGraphics() {
    ChangeDir("/Game/AttrSel/Bg");
    LoadAndQueueUpload(0, 1, 0, "edit_attribute_u_bg_00_ncl.bin", 0, 0, 0);
    LoadAndQueueUpload(0, 11, 0, "edit_attribute_u_bg_00_ncg.bin", 0, 0, 0);
    LoadAndQueueUpload(0, 7, 0, "edit_attribute_u_bg_00_nsc.bin", 0, 0, 0);
    LoadAndQueueUpload(1, 1, 0, "edit_attribute_b_bg_00_ncl.bin", 0, 0, 0);
    LoadAndQueueUpload(1, 11, 0, "edit_attribute_b_bg_00_ncg.bin", 0, 0, 0);
    LoadAndQueueUpload(1, 7, 0, "edit_attribute_b_bg_00_nsc.bin", 0, 0, 0);
    ChangeDir("/");
    SpriteScreen &screen0 = SpriteManager::Get().Screen(0);
    ChangeDir("/Game/AttrSel/Obj");
    screen0.LoadCharacters("nameplate_u_obj.nce_ncg1D.bin", 0, 0);
    screen0.LoadPalette("nameplate_u_obj_ncl.bin", 0, 0);
    mCells[7] = new CellAnimation("nameplate_u_obj_nce.bin", 0, 1, 1);
    mCells[7]->mCharBase = screen0.GetCharBase(0);
    mCells[7]->mPaletteBase = 0;
    mCells[7]->SetAnimation(0);
    SpriteScreen &screen1 = SpriteManager::Get().Screen(1);
    screen1.LoadCharacters("edit_attribute_obj_00.nce_ncg1D.bin", 0, 0);
    screen1.LoadPalette("edit_attribute_obj_00_ncl.bin", 0, 0);
    for (u8 i = 0; i < 6; i++) {
        mCells[i] = new CellAnimation("edit_attribute_obj_00_nce.bin", 0, 1, 1);
        mCells[i]->mCharBase = screen1.GetCharBase(0);
        mCells[i]->mPaletteBase = 0;
        mCells[i]->SetAnimation(i);
    }
    mCells[6] = new CellAnimation("edit_attribute_obj_00_nce.bin", 0, 1, 1);
    mCells[6]->mCharBase = screen1.GetCharBase(0);
    mCells[6]->mPaletteBase = 0;
    mCells[6]->SetAnimation(6);
    mCells[6]->mFrameIndex = mAttrib;
    ChangeDir("/");
    ChangeDir("/Game/2D/union_b");
    screen1.LoadCharacters("union_b_obj_cockpit.nce_ncg1D.bin", 0xd000, 0);
    screen1.LoadPalette("union_b_obj_cockpit_ncl.bin", 0x180, 0);
    ChangeDir("/");
    mMessageWindow = new MessageWindow("mes_chara_edit", 1, 0x1f000);
    mMessageWindow->func_02065bd4(0x14, 4, 0);
    mMessageWindow->SetWidth(0xd8);
    mMessageWindow->StartMessage(0, 0, 1);
    mSelectWindow = new SelectWindow(1, data_ov000_0222623c, -1);
    mSelectWindow->SetCells(1, data_ov000_02226258);
    mSelectWindow->mCursor->mSelection = -1;
    TextManager::Get().SetColor(0, 0, 0xf);
    for (u8 i = 0; i < 2; i++) {
        mBufferSize[i] = (u16) TextManager::Get().BufferSize((s16) data_ov000_02226214[i], (s16) data_ov000_02226218[i]);
        mTextBuffer[i] = OS_Alloc(mBufferSize[i]);
        MI_CpuClear16(mTextBuffer[i], mBufferSize[i]);
        mGlyphCount[i] = TextManager::Get().GlyphCount(data_ov000_02226214[i], data_ov000_02226218[i], data_ov000_02226234[i]);
        mGlyphBuffer[i] = OS_Alloc(mGlyphCount[i] * 0xc);
        TextManager::Get().Init(mGlyphBuffer[i], 0, 0, data_ov000_02226214[i], data_ov000_02226218[i], data_ov000_02226234[i]);
    }
    PrintName(mTextBuffer[0], mBufferSize[0], 0x2000, 0x10, 2, 0x21);
    mMessages = (MessageFile *) LoadFile("/font/mes_chara_edit.mes", 0, 0, 0, 0, 0);
    LoadSubBg();
}

void AttrSelScene::PrintName(void *buffer, u32 size, u32 vram, u16 width, u16 height, u32 unk) {
    s16 outWidth = 0;
    s16 outHeight = 0;
    s16 pixelWidth = 0;
    TextManager::Get().mUse4bppFont = 1;
    TextManager::Get().Print(gSaveData[0].mName, 0x18, buffer, width, height, &outWidth, &outHeight,
                             (u16 *) &pixelWidth, 0, 2, unk, 0x10, 0, 0, 1, 1, 1);
    TextManager::Get().mUse4bppFont = 0;
    QueueUpload(SpriteManager::Get().Screen(0).mBgIndex, 4, vram, buffer, size, 0);
}

void AttrSelScene::PrintMessage(MessageFile *file, u16 index, void *buffer, u32 size, u32 vram, u16 width, u16 height, u32 unk) {
    MI_CpuClear16(buffer, size);
    TextManager &text = TextManager::Get();
    s16 outWidth;
    s16 outHeight;
    outHeight = 0;
    outWidth = 0;
    text.Print(file->GetText(index), file->GetLength(index), buffer, width, height, &outWidth, &outHeight, NULL, 0, 2,
               unk, 0xc, 0, 0, 0, 1, 1);
    QueueUpload(SpriteManager::Get().Screen(0).mBgIndex, 4, vram, buffer, size, 0);
}

void AttrSelScene::LoadSubBg() {
    char path[0x40];
    ChangeDir("/Game/AttrSel/Bg");
    sprintf(path, "edit_attribute_u_sub_%02d_ncl.bin", mAttrib);
    LoadAndQueueUpload(0, 1, 0x6000, path, 0, 0, 0);
    sprintf(path, "edit_attribute_u_sub_%02d_ncg.bin", mAttrib);
    LoadAndQueueUpload(0, 14, 0, path, 0, 0, 0);
    sprintf(path, "edit_attribute_u_sub_%02d_nsc.bin", mAttrib);
    LoadAndQueueUpload(0, 10, 0, path, 0, 0, 0);
    ChangeDir("/");
    PrintMessage(mMessages, data_ov000_0222621c[mAttrib] + 0x1d, mTextBuffer[1], mBufferSize[1], 0x4000, 0x1c, 0x10, 0x44);
}

void AttrSelScene::TransitionStep() {
    switch (mTransitionState) {
        case 0:
            mFade = 0x1f;
            mTransitionState++;
        case 1:
            mFade -= 3;
            if (mFade <= 0) {
                mFade = 0;
                mTransitionState = 4;
                mBlendAlphaLowByte = 0x1f;
                mBlendAlphaHighByte = 0;
            } else {
                mBlendAlphaLowByte = 0x1f - mFade;
                mBlendAlphaHighByte = mFade;
            }
            QueueFrameCallback(AttrSelTransitionCallback, this);
            break;
        case 2:
            mFade = 0;
            mTransitionState++;
        case 3:
            mFade -= 3;
            if (mFade <= -0x1f) {
                mFade = 0x1f;
                mTransitionState = 0;
                LoadSubBg();
                mBlendAlphaHighByte = 0x1f;
                mBlendAlphaLowByte = 0;
            } else {
                int v = -mFade;
                mBlendAlphaHighByte = v;
                mBlendAlphaLowByte = 0x1f - (v & 0xff);
            }
            QueueFrameCallback(AttrSelTransitionCallback, this);
            break;
        case 4:
            break;
    }
}

void AttrSelTransitionCallback(void *scene) {
    AttrSelScene *self = (AttrSelScene *) scene;
    MtxFx22 mtx;
    func_01ffcef4(&mtx, 0x1000, 0x1000, 0);
    reg_G2_BLDALPHA = self->mBlendAlphaLowByte | (self->mBlendAlphaHighByte << 8);
    G2x_SetBGyAffine_((vu16 *) 0x4000030, &mtx, 0, 0, -self->mFade, 0);
}

void AttrSelScene::PulseBlend() {
    switch (mPulseState) {
        case 0:
            mPulseAlpha += 2;
            if (mPulseAlpha >= 7) {
                mUnk_10_0 = 0;
                mPulseState = 1;
            }
            break;
        case 1:
            mPulseFrames++;
            if (mPulseFrames >= 4) {
                mPulseFrames = 0;
                if (!mUnk_10_0) {
                    mPulseAlpha += 2;
                    if (mPulseAlpha >= 0x1f) {
                        mPulseAlpha = 0x1f;
                        mUnk_10_0 ^= 1;
                    }
                } else {
                    mPulseAlpha -= 2;
                    if (mPulseAlpha <= 7) {
                        mPulseAlpha = 7;
                        mUnk_10_0 ^= 1;
                    }
                }
            }
            break;
        case 2:
            if (!mUnk_10_0) {
                mPulseAlpha += 6;
                if (mPulseAlpha >= 0x1f) {
                    mPulseAlpha = 0x1f;
                    mUnk_10_0 ^= 1;
                }
            } else {
                mPulseAlpha -= 6;
                if (mPulseAlpha <= 7) {
                    mPulseAlpha = 7;
                    mUnk_10_0 ^= 1;
                    mPulseCount++;
                    if (mPulseCount >= 6) {
                        mPulseState = 1;
                    }
                }
            }
            break;
    }
    reg_G2S_DB_BLDALPHA = mPulseAlpha | 0x1f00;
}

void AttrSelScene::DrawText() {
    SpriteManager::Get().Screen(0).AddOam(mGlyphBuffer[0], (u8) mGlyphCount[0], 0x16, 0xaa, 0, 0x40, 0xf, 0);
    SpriteManager::Get().Screen(0).AddOam(mGlyphBuffer[1], (u8) mGlyphCount[1], 0x10, 0x7c, 0, 0x80, 0xf, 0);
    mCells[7]->Draw(0, 0x80, 0x60, 0);
}

void AttrSelScene::Draw() {
    for (u8 i = 0; i < 6; i++) {
        mCells[i]->Draw(1, 0x80, 0x60, 0);
    }
    mCells[6]->Draw(1, 0x80, 0x68, 0);
    mMessageWindow->Update();
    mSelectWindow->Draw();
}

int AttrSelScene::Distance(s16 x1, s16 y1, s16 x2, s16 y2) {
    s16 dy = y2 - y1;
    s16 dx = x2 - x1;
    int sq = dx * dx + dy * dy;
    return (int) ((float) FX_Sqrt(FX32_CONST((float) sq)) / 4096.0f);
}
