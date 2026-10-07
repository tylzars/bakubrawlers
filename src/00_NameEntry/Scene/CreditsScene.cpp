#include "Scene/CreditsScene.hpp"
#include "System/FrameTask.hpp"
#include <nitro/gx.h>
#include <nitro/os.h>
#include "Graphics/Display.hpp"
#include "Graphics/SpriteManager.hpp"
#include "System/FileSystem.hpp"
#include "System/FrameUpdate.hpp"
#include "System/InputManager.hpp"
#include "Text/TextManager.hpp"
#include "Unknown/UnkFunctions.hpp"
#include <nitro/mi.h>

CreditsScene::CreditsScene() :
    SceneBase(2, 0) {
    mMessages = NULL;
    mState = 0;
    mTick = 0;
    mSubScroll = 0;
    mBufferSize = 0;
    for (u8 i = 0; i < 2; i++) {
        mColState[i] = 0;
        mMessage[i] = 0;
        mScroll[i] = 0;
        mRow[i] = 0;
        mBuffers[i] = NULL;
    }
    mTimer[0] = 0;
    mTimer[1] = 12;
    FrameTask::Get().func_0203dc34(0, 0xf);
}

CreditsScene::~CreditsScene() {
    if (mMessages != NULL) {
        OS_Free(mMessages);
    }
    for (u8 i = 0; i < 2; i++) {
        if (mBuffers[i] != NULL) {
            OS_Free(mBuffers[i]);
        }
    }
    G2_SetBG1Offset(0, 0);
    G2S_SetBG1Offset(0, 0);
    FrameTask::Get().func_0203dd90();
}

void CreditsScene::InitDisplay() {
    GX_SetDispSelect(1);
    SpriteManager::Get().func_02026dec(1, 1);
    func_02001e68(1, 0, 0);
    func_02001ed0(0);
    G2_SetBG0Control(0, 1, 0, 1, 0);
    G2S_SetBG0Control(0, 1, 0, 1, 0);
    G2_SetBG1Control(0, 0, 4, 4, 0);
    G2S_SetBG1Control(0, 0, 4, 4, 0);
    G2_SetBG0Offset(0, 0);
    G2_SetBG1Offset(0, 0);
    G2S_SetBG0Offset(0, 0);
    G2S_SetBG1Offset(0, 0);
    G2_SetBG0Priority(3);
    G2_SetBG1Priority(0);
    G2S_SetBG0Priority(3);
    G2S_SetBG1Priority(0);
    GX_SetVisiblePlane(0x13);
    GXS_SetVisiblePlane(0x13);
    G2_SetBlendBrightness(0x1f, 0);
    G2S_SetBlendBrightness(0x1f, 0);
    GX_SetVisibleWnd(0);
    GXS_SetVisibleWnd(0);
    G2_BlendNone();
    G2S_BlendNone();
}

void CreditsScene::LoadGraphics() {
    ChangeDir("/Game/Credit/");
    func_02025b64(0, 1, 0, "credits_u_bg_00_ncl.bin", 0, 0, 0);
    func_02025b64(0, 11, 0, "credits_u_bg_00_ncg.bin", 0, 0, 0);
    func_02025b64(0, 7, 0, "credits_u_bg_00_nsc.bin", 0, 0, 0);
    func_02025b64(1, 1, 0, "credits_b_bg_00_ncl.bin", 0, 0, 0);
    func_02025b64(1, 11, 0, "credits_b_bg_00_ncg.bin", 0, 0, 0);
    func_02025b64(1, 7, 0, "credits_b_bg_00_nsc.bin", 0, 0, 0);
}

void func_ov000_02225f98(void *scene) {
    CreditsScene *self = (CreditsScene *) scene;
    G2_SetBG1Offset(0, self->mScroll[0]);
    G2S_SetBG1Offset(0, self->mScroll[1]);
}

void CreditsScene::LoadText() {
    u16 *map = (u16 *) OS_Alloc(TextManager::Get().BufferSize(8, 8));
    MI_CpuClear16(map, 0x800);
    TextManager::Get().func_0202cd48(0, map, 0, 0, 0x20, 0x20, 0x20);
    func_02025c1c(0, 8, 0, map, 0x800, 1);
    map = (u16 *) OS_Alloc(0x800);
    MI_CpuClear16(map, 0x800);
    TextManager::Get().func_0202cd48(0, map, 0, 0, 0x20, 0x20, 0x20);
    func_02025c1c(1, 8, 0, map, 0x800, 1);
    TextManager::Get().func_0202ce44(0, 0, 0, 0);
    TextManager::Get().func_0202ce44(1, 0, 0, 0);
    void *chars = OS_Alloc(TextManager::Get().BufferSize(0x20, 0x20));
    MI_CpuClear16(chars, 0x8000);
    func_02005e9c(chars, 0x8000);
    func_02025c1c(0, 12, 0, chars, 0x8000, 1);
    chars = OS_Alloc(0x8000);
    MI_CpuClear16(chars, 0x8000);
    func_02005e9c(chars, 0x8000);
    func_02025c1c(1, 12, 0, chars, 0x8000, 1);
    mBufferSize = TextManager::Get().BufferSize(8, 8);
    for (u32 i = 0; i < 2; i++) {
        mBuffers[i] = OS_Alloc(mBufferSize);
        MI_CpuClear16(mBuffers[i], mBufferSize);
    }
    mMessages = (MessageFile *) func_02023504("/font/mes_Credit.mes", 0, 0, 0, 0, 0);
}

void CreditsScene::PrintLine(u32 message, u32 row, u32 column, u32 shadow) {
    s16 pixelWidth;
    s16 outWidth;
    s16 outHeight;
    s16 outWidth2;
    s16 outHeight2;
    u32 offset = row * mBufferSize;
    MI_CpuClear16(mBuffers[column], mBufferSize);
    func_02005e9c(mBuffers[column], mBufferSize);
    if (shadow) {
        pixelWidth = 0;
        outWidth = 0;
        outHeight = 0;
        TextManager::Get();
        const u8 *text = mMessages->GetText((u16) message);
        TextManager::Get().Print(text, mMessages->GetLength(message), mBuffers[column], 0x20, 2, &outWidth,
                                 &outHeight, (u16 *) &pixelWidth, 0, 2, 0x21, 0, 0xffff, 0, 0, 1, 1);
        outHeight = 0;
        outWidth = (0x100 - (u16) pixelWidth) / 2;
        MessageFile *file = mMessages;
        TextManager::Get().Print(file->GetText((u16) message), file->GetLength(message), mBuffers[column], 0x20, 2, &outWidth,
                                 &outHeight, NULL, 0, 2, 0, 0x10, 0, 0, 0, 1, 1);
    }
    func_02025c1c(column, 12, offset, mBuffers[column], mBufferSize, 0);
}

BOOL CreditsScene::UpdateText() {
    u32 step = 0;
    if (InputManager::Get().IsHeld(8)) {
        mColState[0] = 3;
        mColState[1] = 3;
        G2_SetBG0Priority(3);
        G2S_SetBG0Priority(3);
    }
    mTick++;
    if (mTick > 1) {
        mTick = 0;
        mSubScroll++;
        step++;
    }
    mScroll[0] += step;
    mScroll[1] += step;
    func_020254f0(func_ov000_02225f98, this);
    while (mSubScroll >= 0x10) {
        mSubScroll -= 0x10;
        for (u32 i = 0; i < 2; i++) {
            switch (mColState[i]) {
                case 0:
                    mTimer[i]++;
                    if (mTimer[i] > 0xc) {
                        mScroll[i] = 0;
                        mMessage[i] = 0;
                        mRow[i] = 0xc;
                        mColState[i]++;
                    }
                    break;
                case 1:
                    mRow[i]++;
                    if (mRow[i] >= 0x10) {
                        mRow[i] = 0;
                    }
                    PrintLine(mMessage[i], mRow[i], i, 1);
                    mMessage[i]++;
                    if (mMessage[i] > *(u16 *) mMessages - 1) {
                        mTimer[i] = 0;
                        mColState[i]++;
                    }
                    break;
                case 2:
                    mRow[i]++;
                    if (mRow[i] >= 0x10) {
                        mRow[i] = 0;
                    }
                    PrintLine(mMessage[i], mRow[i], i, 0);
                    mTimer[i]++;
                    if (mTimer[i] > 0x10) {
                        mColState[i]++;
                    }
                    break;
                case 3:
                    break;
            }
        }
    }
    return mColState[0] == 3 && mColState[1] == 3;
}

void CreditsScene::Update() {
    switch (mState) {
        case 0:
            InitDisplay();
            LoadGraphics();
            LoadText();
            SpriteManager::Get().StartFade(2);
            mState++;
            break;
        case 1:
            if (!SpriteManager::Get().IsFading()) {
                mState++;
            }
            break;
        case 2:
            if (UpdateText()) {
                mState++;
            }
            break;
        case 3:
            mState++;
            SpriteManager::Get().StartFade(3);
            break;
        case 4:
            if (SpriteManager::Get().IsFading()) {
                break;
            }
            mState++;
        case 5:
            RequestScene(1);
            mFinished = 1;
            break;
    }
}
