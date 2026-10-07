#include "Unknown/UnkStruct_02070608.hpp"

#include "Game/UnkStruct_020d433c.hpp"
#include "Game/UnkStruct_020d43f0.hpp"
#include "Graphics/SpriteManager.hpp"
#include "System/FileSystem.hpp"
#include "Text/TextManager.hpp"
#include "UI/SelectWindow.hpp"
#include "Unknown/UnkStruct_02071868.hpp"
#include "Unknown/UnkStruct_020c17a8.hpp"
#include "System/FrameTask.hpp"

#include <nitro/gx.h>
#include <nitro/mi.h>
#include <nitro/os.h>

UnkStruct_02070608::UnkStruct_02070608(u32 unk1, u32 unk2, u32 unk3, u32 unk4, u32 unk5, u32 unk6, u32 unk7, u32 unk8,
                                       u32 unk9) :
    SceneBase(2, 0) {
    mWindow = NULL;
    mMessages = NULL;
    mUnk_14 = NULL;
    mUnk_18 = unk3;
    mUnk_1c = unk4;
    mUnk_20 = unk6;
    mUnk_24 = unk7;
    mUnk_28 = unk8;
    mUnk_2c = *(u16 *) &unk9;
    mUnk_30 = unk1;
    mUnk_31 = unk2;
    mUnk_32 = unk5;
    mUnk_33 = 0;
    FrameTask::Get().func_0203dd90();
}

UnkStruct_02070608::~UnkStruct_02070608() {
    if (mMessages != NULL) {
        OS_Free(mMessages);
    }
    if (mUnk_14 != NULL) {
        OS_Free(mUnk_14);
    }
    if (mWindow != NULL) {
        delete mWindow;
    }
    switch (mUnk_32) {
        case 0:
            G2S_SetBG0Offset(0, 0);
            break;
        case 1:
            G2S_SetBG1Offset(0, 0);
            break;
        case 2:
            G2S_SetBG2Offset(0, 0);
            break;
        case 3:
            G2S_SetBG3Offset(0, 0);
            break;
    }
}

extern const u32 data_020a2dbc[];

static inline void SetCellPos(SelectWindow *window, int index, s16 x, s16 y) {
    window->mCellPos[index].x = x;
    window->mCellPos[index].y = y;
}

// Small scene created once the save dialog is done (vtable 0x020a54a0).
class UnkStruct_020a54a0 : public SceneBase {
public:
    UnkStruct_020a54a0() :
        SceneBase(0xffdc, 0) {
        ((u8 *) mContext)[0x23] = 0;
    }
    virtual ~UnkStruct_020a54a0();
    virtual void Update();
};

void UnkStruct_02070608::Update() {
    switch (mUnk_33) {
        case 0:
            if (func_0203dd0c(&FrameTask::Get(), 0) != 0) {
                break;
            }
            func_02070d10();
            if (UnkStruct_020c17a8::Get().func_02023c38(0) == 4) {
                func_020716b0(0);
            } else {
                func_020716b0(9);
            }
            mUnk_33++;
            break;
        case 1:
            if ((u8) mWindow->Update(1) != 1) {
                break;
            }
            switch (mWindow->GetSelectedId()) {
                case 0:
                    mUnk_33++;
                    break;
                case 1:
                    mUnk_33 = 5;
                    break;
            }
            break;
        case 2:
            FrameTask::Get().func_0203dd90();
            SetCellPos(mWindow, 3, 0x80, 0x60);
            SetCellPos(mWindow, 0, 0x100, 0xc0);
            SetCellPos(mWindow, 1, 0x100, 0xc0);
            func_020716b0(1);
            mUnk_33++;
            break;
        case 3:
            if (func_0203dd0c(&FrameTask::Get(), 0) != 0) {
                break;
            }
            data_020d43f0[0].mUnk_26 = data_020d433c.mUnk_79;
            UnkStruct_020c17a8::Get();
            new UnkStruct_020a54a0();
            mUnk_33++;
            break;
        case 4:
            if (mContext->mUnk_12 == 0) {
                break;
            }
            if (mContext->mUnk_12 == 1) {
                new UnkStruct_02071868(1);
            } else {
                mUnk_33 = 5;
            }
            break;
        case 5:
            SpriteManager::Get().StartFade(3);
            mUnk_33++;
            break;
        case 6:
            if (!SpriteManager::Get().IsFading()) {
                G2_BlendNone();
                G2S_BlendNone();
                RequestScene(1);
                mFinished = 1;
            }
            break;
    }
    if (mWindow != NULL) {
        mWindow->Draw();
    }
}

void UnkStruct_02070608::func_020716b0(u32 index) {
    MI_CpuClearFast(mUnk_14, mUnk_2e);
    TextManager &text = TextManager::Get();
    u16 pixelWidth;
    s16 width;
    s16 height;
    height = 0x38;
    width  = 0;
    MessageFile *file = mMessages;
    text.Print(file->GetText(index), file->GetLength(index), mUnk_14, 0x18, 0x10, &width, &height, &pixelWidth, 0, 2, 0,
               0x10, 0, 0, 1, 1, 1);
    func_02025c1c(mUnk_30, data_020a2dbc[mUnk_32], mUnk_20, mUnk_14, mUnk_2e, 0);
    s16 offset = (pixelWidth - 0x100) / 2;
    switch (mUnk_32) {
        case 0:
            G2S_SetBG0Offset(offset, 0);
            break;
        case 1:
            G2S_SetBG1Offset(offset, 0);
            break;
        case 2:
            G2S_SetBG2Offset(offset, 0);
            break;
        case 3:
            G2S_SetBG3Offset(offset, 0);
            break;
    }
}
