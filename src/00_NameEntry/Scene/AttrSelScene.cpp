#include "Scene/AttrSelScene.hpp"
#include "Game/UnkStruct_020d43f0.hpp"
#include "Graphics/CellAnimation.hpp"
#include "UI/MessageWindow.hpp"
#include "UI/SelectWindow.hpp"
#include <nitro/gx.h>
#include <nitro/os.h>
#include "Graphics/Display.hpp"
#include "Graphics/SpriteManager.hpp"
#include "System/FileSystem.hpp"
#include "Text/TextManager.hpp"
#include <nitro/mi.h>
#include <stdio.h>
#include "System/FrameUpdate.hpp"
#include <nitro/fx.h>
#include "System/InputManager.hpp"
#include "System/SoundManager.hpp"
#include "System/TouchManager.hpp"

extern u8 data_ov000_0222622c[];

void operator delete(void *ptr);
u32 func_0200198c(s32 x, s32 y);
extern u8 data_ov000_02226224[];
extern u8 data_ov000_0222621c[];




AttrSelScene::AttrSelScene() :
    SceneBase(1000, 0) {
    mUnk_0c = 0;
    mAttrib = data_ov000_0222622c[data_020d43f0[0].mUnk_1a_8];
    mUnk_0e = 0;
    mUnk_0f = 0;
    mUnk_10_0 = 0;
    mUnk_11 = 0;
    mFade = 0;
    mState = 4;
    mUnk_16 = 0;
    mUnk_17 = 0;
    mUnk_18 = 0;
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
    switch (mUnk_0c) {
        case 0:
            SpriteManager::Get().StartFade(2);
            mUnk_0c++;
        case 1:
            if (!SpriteManager::Get().IsFading()) {
                mUnk_0c++;
            }
            break;
        case 2: {
            u32 event = 0;
            if ((u8) mSelectWindow->Update(TRUE) == 2) {
                event = 1;
            } else {
                if (TouchManager::Get().mUnk_57) {
                    if (Distance(TouchManager::Get().mX, TouchManager::Get().mY, 0x80, 0x60) < 0x50) {
                        int x = FX32_CONST((float) (u32) TouchManager::Get().mX - 128.0f);
                        int y = FX32_CONST((float) (u32) TouchManager::Get().mY - 96.0f);
                        u32 sector = (func_0200198c(x, y) + 0x1555) / 10922;
                        if (sector > 5) {
                            sector = 0;
                        }
                        u8 attr = data_ov000_02226224[sector];
                        if (mAttrib == attr) {
                            event = 3;
                            SoundManager::Get().func_020246f8(0);
                        } else {
                            event = 2;
                            SoundManager::Get().func_020246f8(5);
                        }
                        mAttrib = attr;
                    }
                }
                if (event == 0) {
                    if (InputManager::Get().IsTrigger(0x50)) {
                        mAttrib++;
                        event = 2;
                        if (mAttrib >= 6) {
                            mAttrib = 0;
                        }
                        SoundManager::Get().func_020246f8(5);
                    } else if (InputManager::Get().IsTrigger(0xa0)) {
                        event = 2;
                        if (mAttrib == 0) {
                            mAttrib = 6;
                        }
                        mAttrib--;
                        SoundManager::Get().func_020246f8(5);
                    } else if (InputManager::Get().IsHeld(1)) {
                        SoundManager::Get().func_020246f8(0);
                        event = 3;
                    }
                }
            }
            switch (event) {
                case 1:
                    SpriteManager::Get().StartFade(3);
                    mUnk_15 = 1;
                    mUnk_0c = 4;
                    break;
                case 2:
                    if (mState == 3) {
                        LoadSubBg();
                    }
                    mState = 2;
                    mCells[6]->mUnk_0f = mAttrib;
                    mUnk_0e = 0;
                    mUnk_0f = 0;
                    break;
                case 3:
                    mUnk_0e = 2;
                    mUnk_0c++;
                    break;
            }
            break;
        }
        case 3:
            if (mUnk_0e != 2) {
                data_020d43f0[0].mUnk_1a_8 = data_ov000_0222621c[mAttrib];
                SpriteManager::Get().StartFade(3);
                mUnk_15 = 2;
                mUnk_0c++;
            }
            break;
        case 4:
            if (!SpriteManager::Get().IsFading()) {
                RequestScene(mUnk_15);
                mFinished = 1;
            }
            break;
    }
    TransitionStep();
    PulseBlend();
    DrawText();
    Draw();
}

// Same arithmetic as TextManager::GlyphCount(width, height, flags) but with signed operands: the original calls
// _s32_div_f here (and in ov008 0221e2d8/0221e2ec), the header's u32 parameters would give _u32_div_f.
extern u8 data_ov000_02226214[];

extern u8 data_ov000_02226218[];

extern u32 data_ov000_02226234[];

extern WindowItemRecord data_ov000_0222623c[];

extern WindowCellRecord data_ov000_02226258[];

// Same helper as FieldSelectScene.cpp defines locally; belongs in nitro/gx.h next to G2S_SetBlendAlpha.
void AttrSelScene::InitDisplay() {
    GX_SetDispSelect(1);
    SpriteManager::Get().func_02026dec(1, 1);
    func_02001e68(1, 3, 0);
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



// non-matching: 7 words, registers only: the original keeps flags in r8 and unitX in r6.
void AttrSelScene::LoadGraphics() {
    ChangeDir("/Game/AttrSel/Bg");
    func_02025b64(0, 1, 0, "edit_attribute_u_bg_00_ncl.bin", 0, 0, 0);
    func_02025b64(0, 11, 0, "edit_attribute_u_bg_00_ncg.bin", 0, 0, 0);
    func_02025b64(0, 7, 0, "edit_attribute_u_bg_00_nsc.bin", 0, 0, 0);
    func_02025b64(1, 1, 0, "edit_attribute_b_bg_00_ncl.bin", 0, 0, 0);
    func_02025b64(1, 11, 0, "edit_attribute_b_bg_00_ncg.bin", 0, 0, 0);
    func_02025b64(1, 7, 0, "edit_attribute_b_bg_00_nsc.bin", 0, 0, 0);
    ChangeDir("/");
    SpriteScreen &screen0 = SpriteManager::Get().Screen(0);
    ChangeDir("/Game/AttrSel/Obj");
    screen0.LoadCharacters("nameplate_u_obj.nce_ncg1D.bin", 0, 0);
    screen0.LoadPalette("nameplate_u_obj_ncl.bin", 0, 0);
    mCells[7] = new CellAnimation("nameplate_u_obj_nce.bin", 0, 1, 1);
    mCells[7]->mCharBase = screen0.func_020276f4(0);
    mCells[7]->mUnk_11 = 0;
    mCells[7]->SetAnimation(0);
    SpriteScreen &screen1 = SpriteManager::Get().Screen(1);
    screen1.LoadCharacters("edit_attribute_obj_00.nce_ncg1D.bin", 0, 0);
    screen1.LoadPalette("edit_attribute_obj_00_ncl.bin", 0, 0);
    for (u8 i = 0; i < 6; i++) {
        mCells[i] = new CellAnimation("edit_attribute_obj_00_nce.bin", 0, 1, 1);
        mCells[i]->mCharBase = screen1.func_020276f4(0);
        mCells[i]->mUnk_11 = 0;
        mCells[i]->SetAnimation(i);
    }
    mCells[6] = new CellAnimation("edit_attribute_obj_00_nce.bin", 0, 1, 1);
    mCells[6]->mCharBase = screen1.func_020276f4(0);
    mCells[6]->mUnk_11 = 0;
    mCells[6]->SetAnimation(6);
    mCells[6]->mUnk_0f = mAttrib;
    ChangeDir("/");
    ChangeDir("/Game/2D/union_b");
    screen1.LoadCharacters("union_b_obj_cockpit.nce_ncg1D.bin", 0xd000, 0);
    screen1.LoadPalette("union_b_obj_cockpit_ncl.bin", 0x180, 0);
    ChangeDir("/");
    mMessageWindow = new MessageWindow("mes_chara_edit", 1, 0x1f000);
    mMessageWindow->func_02065bd4(0x14, 4, 0);
    mMessageWindow->func_02065be4(0xd8);
    mMessageWindow->func_02065890(0, 0, 1);
    mSelectWindow = new SelectWindow(1, data_ov000_0222623c, -1);
    mSelectWindow->SetCells(1, data_ov000_02226258);
    mSelectWindow->mCursor->mSelection = -1;
    TextManager::Get().SetColor(0, 0, 0xf);
    u8 i;
    u8 height;
    s32 flags;
    u8 width;
    for (i = 0; i < 2; i++) {
        mBufferSize[i] = (u16) TextManager::Get().BufferSize((s16) (width = data_ov000_02226214[i]), (s16) (height = data_ov000_02226218[i]));
        mTextBuffer[i] = OS_Alloc(mBufferSize[i]);
        MI_CpuClear16(mTextBuffer[i], mBufferSize[i]);
        mGlyphCount[i] = TextManager::Get().GlyphCount(width, height, flags = data_ov000_02226234[i]);
        mGlyphBuffer[i] = OS_Alloc(mGlyphCount[i] * 0xc);
        TextManager::Get().Init(mGlyphBuffer[i], 0, 0, width, height, flags);
    }
    PrintName(mTextBuffer[0], mBufferSize[0], 0x2000, 0x10, 2, 0x21);
    mMessages = (MessageFile *) func_02023504("/font/mes_chara_edit.mes", 0, 0, 0, 0, 0);
    LoadSubBg();
}

// non-matching: 10 words, the guard load before a preceding store (see docs/decompiling.md).
void AttrSelScene::PrintName(void *buffer, u32 size, u32 vram, u16 width, u16 height, u32 unk) {
    s16 outWidth = 0;
    s16 outHeight = 0;
    s16 pixelWidth = 0;
    TextManager::Get().mUnk_5b = 1;
    TextManager::Get().Print(data_020d43f0[0].mName, 0x18, buffer, width, height, &outWidth, &outHeight,
                             (u16 *) &pixelWidth, 0, 2, unk, 0x10, 0, 0, 1, 1, 1);
    TextManager::Get().mUnk_5b = 0;
    func_02025c1c(SpriteManager::Get().Screen(0).mBgIndex, 4, vram, buffer, size, 0);
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
    func_02025c1c(SpriteManager::Get().Screen(0).mBgIndex, 4, vram, buffer, size, 0);
}

void AttrSelScene::LoadSubBg() {
    char path[0x40];
    ChangeDir("/Game/AttrSel/Bg");
    sprintf(path, "edit_attribute_u_sub_%02d_ncl.bin", mAttrib);
    func_02025b64(0, 1, 0x6000, path, 0, 0, 0);
    sprintf(path, "edit_attribute_u_sub_%02d_ncg.bin", mAttrib);
    func_02025b64(0, 14, 0, path, 0, 0, 0);
    sprintf(path, "edit_attribute_u_sub_%02d_nsc.bin", mAttrib);
    func_02025b64(0, 10, 0, path, 0, 0, 0);
    ChangeDir("/");
    PrintMessage(mMessages, data_ov000_0222621c[mAttrib] + 0x1d, mTextBuffer[1], mBufferSize[1], 0x4000, 0x1c, 0x10, 0x44);
}

void AttrSelScene::TransitionStep() {
    switch (mState) {
        case 0:
            mFade = 0x1f;
            mState++;
        case 1:
            mFade -= 3;
            if (mFade <= 0) {
                mFade = 0;
                mState = 4;
                mUnk_16 = 0x1f;
                mUnk_17 = 0;
            } else {
                mUnk_16 = 0x1f - mFade;
                mUnk_17 = mFade;
            }
            func_020254f0(func_ov000_0221f080, this);
            break;
        case 2:
            mFade = 0;
            mState++;
        case 3:
            mFade -= 3;
            if (mFade <= -0x1f) {
                mFade = 0x1f;
                mState = 0;
                LoadSubBg();
                mUnk_17 = 0x1f;
                mUnk_16 = 0;
            } else {
                int v = -mFade;
                mUnk_17 = v;
                mUnk_16 = 0x1f - (v & 0xff);
            }
            func_020254f0(func_ov000_0221f080, this);
            break;
        case 4:
            break;
    }
}

void func_ov000_0221f080(void *scene) {
    AttrSelScene *self = (AttrSelScene *) scene;
    MtxFx22 mtx;
    func_01ffcef4(&mtx, 0x1000, 0x1000, 0);
    reg_G2_BLDALPHA = self->mUnk_16 | (self->mUnk_17 << 8);
    G2x_SetBGyAffine_((vu16 *) 0x4000030, &mtx, 0, 0, -self->mFade, 0);
}

void AttrSelScene::PulseBlend() {
    switch (mUnk_0e) {
        case 0:
            mUnk_0f += 2;
            if (mUnk_0f >= 7) {
                mUnk_10_0 = 0;
                mUnk_0e = 1;
            }
            break;
        case 1:
            mUnk_11++;
            if (mUnk_11 >= 4) {
                mUnk_11 = 0;
                if (!mUnk_10_0) {
                    mUnk_0f += 2;
                    if (mUnk_0f >= 0x1f) {
                        mUnk_0f = 0x1f;
                        mUnk_10_0 ^= 1;
                    }
                } else {
                    mUnk_0f -= 2;
                    if (mUnk_0f <= 7) {
                        mUnk_0f = 7;
                        mUnk_10_0 ^= 1;
                    }
                }
            }
            break;
        case 2:
            if (!mUnk_10_0) {
                mUnk_0f += 6;
                if (mUnk_0f >= 0x1f) {
                    mUnk_0f = 0x1f;
                    mUnk_10_0 ^= 1;
                }
            } else {
                mUnk_0f -= 6;
                if (mUnk_0f <= 7) {
                    mUnk_0f = 7;
                    mUnk_10_0 ^= 1;
                    mUnk_18++;
                    if (mUnk_18 >= 6) {
                        mUnk_0e = 1;
                    }
                }
            }
            break;
    }
    reg_G2S_DB_BLDALPHA = mUnk_0f | 0x1f00;
}

void AttrSelScene::DrawText() {
    SpriteManager::Get().Screen(0).func_02027778(mGlyphBuffer[0], (u8) mGlyphCount[0], 0x16, 0xaa, 0, 0x40, 0xf, 0);
    SpriteManager::Get().Screen(0).func_02027778(mGlyphBuffer[1], (u8) mGlyphCount[1], 0x10, 0x7c, 0, 0x80, 0xf, 0);
    mCells[7]->Draw(0, 0x80, 0x60, 0);
}

void AttrSelScene::Draw() {
    for (u8 i = 0; i < 6; i++) {
        mCells[i]->Draw(1, 0x80, 0x60, 0);
    }
    mCells[6]->Draw(1, 0x80, 0x68, 0);
    mMessageWindow->func_020658d0();
    mSelectWindow->Draw();
}

extern u8 data_ov000_0222621c[];





int AttrSelScene::Distance(s16 x1, s16 y1, s16 x2, s16 y2) {
    s16 dy = y2 - y1;
    s16 dx = x2 - x1;
    int sq = dx * dx + dy * dy;
    return (int) ((float) FX_Sqrt(FX32_CONST((float) sq)) / 4096.0f);
}
