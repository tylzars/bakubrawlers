#include "Graphics/SpriteManager.hpp"
#include "Unknown/UnkStruct_020c17a8.hpp"
#include "Text/TextManager.hpp"
#include "Unknown/UnkStruct_02070608.hpp"
#include "Game/UnkStruct_020d433c.hpp"
#include "Game/UnkStruct_020d43f0.hpp"
#include "System/FileSystem.hpp"
#include "UI/SelectWindow.hpp"
#include "Unknown/UnkStruct_02071868.hpp"
#include <nitro/gx.h>
#include <nitro/mi.h>
#include <nitro/os.h>

static WindowCellRecord data_020b8ea0[4] = {
    {"/Game/2D/union_b/union_b_obj_window_nce.bin", 3, 0, 0, 0, 1, {3, 5, 4, 0}},
    {"/Game/2D/union_b/union_b_obj_window_nce.bin", 7, 0, 0, 0, 1, {6, 8, 7, 0}},
    {"/Game/2D/union_b/union_b_obj_window_nce.bin", 10, 0, 0, 0, 1, {10, 10, 10, 0}},
    {"/Game/2D/union_b/union_b_obj_window_nce.bin", 12, 0, 0, 0, 1, {12, 12, 12, 0}},
};

#include "System/FrameTask.hpp"

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

extern const u32 data_020a2dac[];
extern const WindowItemRecord data_020a2dcc[];

void UnkStruct_02070608::func_02070d10() {
    ChangeDir("/Game/2D/union_b/");
    func_02025b64(mUnk_30, 1, mUnk_1c, "union_b_bg_window_ncl.bin", 0, 0, 0);
    func_02025b64(mUnk_30, data_020a2dbc[mUnk_31], mUnk_18, "union_b_bg_window_ncg.bin", 0, 0, 0);
    u32 size;
    u16 *tiles = (u16 *) func_02023504("union_b_bg_window_a_nsc.bin", 0, 0, 0, &size, 0);
    size >>= 1;
    u16 tileAdd = mUnk_18 >> 6;
    u16 palAdd  = (mUnk_1c & 0x1fff) / 0x200;
    if ((palAdd | tileAdd) != 0) {
        u16 i = 0;
        do {
            u16 tile = tiles[i] & 0x3ff;
            u16 flipH = (tiles[i] & 0x400) >> 10;
            u16 pal = (tiles[i] & 0xf000) >> 12;
            u16 flipV = (tiles[i] & 0x800) >> 11;
            tile += tileAdd;
            pal += palAdd;
            tiles[i] = tile | (flipH << 10) | (flipV << 11) | (pal << 12);
            i++;
        } while (size > i);
    }
    func_02025c1c(mUnk_30, data_020a2dac[mUnk_31], 0, tiles, size * 2, 1);
    ChangeDir("/");
    mMessages = (MessageFile *) func_02023504("/font/mes_system.mes", 0, 0, 0, 0, 0);
    TextManager::Get().func_0202ce44(mUnk_30, 0, (mUnk_24 >> 5) & 0xff, 0);
    TextManager::Get();
    u16 *buffer = (u16 *) OS_Alloc(0x600);
    MIi_CpuClear16((u16) ((mUnk_20 >> 5) | ((mUnk_24 >> 5) << 12)), buffer, 0x600);
    TextManager::Get().func_0202cd48(0, buffer, (mUnk_24 >> 5) & 0xff, (u16) mUnk_20, 0x18, 0x18, 0x20);
    func_02025c1c(mUnk_30, data_020a2dac[mUnk_32], 0, buffer, 0x600, 1);
    TextManager::Get();
    mUnk_2e = 0x4000;
    mUnk_14 = OS_Alloc(mUnk_2e);
    SpriteScreen &screen = SpriteManager::Get().Screen(mUnk_30);
    screen.LoadCharacters("/Game/2D/union_b/union_b_obj_window.nce_ncg1D.bin", mUnk_28, 0);
    screen.LoadPalette("/Game/2D/union_b/union_b_obj_window_ncl.bin", mUnk_2c, 0);
    for (u8 i = 0; i < 4; i++) {
        data_020b8ea0[i].mUnk_08 = mUnk_28;
        data_020b8ea0[i].mUnk_0c = mUnk_2c / 32;
    }
    mWindow = new SelectWindow(4, data_020a2dcc, 1);
    mWindow->SetCells(4, data_020b8ea0);
    u16 mask = 0;
    if (mUnk_30 == 0) {
        u16 plane = (reg_GX_DISPCNT & GX_DISPCNT_PLANE_MASK) >> GX_DISPCNT_PLANE_SHIFT;
        switch (mUnk_31) {
            case 0:
                G2_SetBG0Offset(0, 0);
                G2_SetBG0Priority(1);
                plane |= 1;
                mask = 1;
                break;
            case 1:
                G2_SetBG1Offset(0, 0);
                G2_SetBG1Priority(1);
                plane |= 2;
                mask = 2;
                break;
            case 2:
                G2_SetBG2Offset(0, 0);
                G2_SetBG2Priority(1);
                plane |= 4;
                mask = 4;
                break;
            case 3:
                G2_SetBG3Offset(0, 0);
                G2_SetBG3Priority(1);
                plane |= 8;
                mask = 8;
                break;
        }
        switch (mUnk_32) {
            case 0:
                G2_SetBG0Offset(0, 0);
                G2_SetBG0Priority(0);
                mask |= 1;
                plane |= 1;
                break;
            case 1:
                G2_SetBG1Offset(0, 0);
                G2_SetBG1Priority(0);
                mask |= 2;
                plane |= 2;
                break;
            case 2:
                G2_SetBG2Offset(0, 0);
                G2_SetBG2Priority(0);
                mask |= 4;
                plane |= 4;
                break;
            case 3:
                G2_SetBG3Offset(0, 0);
                G2_SetBG3Priority(0);
                mask |= 8;
                plane |= 8;
                break;
        }
        GX_SetVisiblePlane(plane);
        mask |= 0x10;
        G2_SetBlendBrightness(mask ^ 0x3f, -8);
        SpriteManager::Get();
        SpriteManager::sFade[0]       = 1;
        SpriteManager::sBrightness[0] = 0;
        SpriteManager::sFade[1]        = 1;
        SpriteManager::sBrightness[1]  = -8;
    } else {
        u16 plane = (reg_GXS_DB_DISPCNT & GX_DISPCNT_PLANE_MASK) >> GX_DISPCNT_PLANE_SHIFT;
        switch (mUnk_31) {
            case 0:
                G2S_SetBG0Offset(0, 0);
                G2S_SetBG0Priority(1);
                plane |= 1;
                mask = 1;
                break;
            case 1:
                G2S_SetBG1Offset(0, 0);
                G2S_SetBG1Priority(1);
                plane |= 2;
                mask = 2;
                break;
            case 2:
                G2S_SetBG2Offset(0, 0);
                G2S_SetBG2Priority(1);
                plane |= 4;
                mask = 4;
                break;
            case 3:
                G2S_SetBG3Offset(0, 0);
                G2S_SetBG3Priority(1);
                plane |= 8;
                mask = 8;
                break;
        }
        switch (mUnk_32) {
            case 0:
                G2S_SetBG0Offset(0, 0);
                G2S_SetBG0Priority(0);
                mask |= 1;
                plane |= 1;
                break;
            case 1:
                G2S_SetBG1Offset(0, 0);
                G2S_SetBG1Priority(0);
                mask |= 2;
                plane |= 2;
                break;
            case 2:
                G2S_SetBG2Offset(0, 0);
                G2S_SetBG2Priority(0);
                mask |= 4;
                plane |= 4;
                break;
            case 3:
                G2S_SetBG3Offset(0, 0);
                G2S_SetBG3Priority(0);
                mask |= 8;
                plane |= 8;
                break;
        }
        GXS_SetVisiblePlane(plane);
        mask |= 0x10;
        G2S_SetBlendBrightness(mask ^ 0x3f, -8);
        SpriteManager::Get();
        SpriteManager::sFade[0]       = 1;
        SpriteManager::sBrightness[0] = -8;
        SpriteManager::sFade[1]        = 1;
        SpriteManager::sBrightness[1]  = 0;
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
