#include "Graphics/SpriteManager.hpp"
#include "System/SaveManager.hpp"
#include "Text/TextManager.hpp"
#include "UI/SaveWindow.hpp"
#include "Game/UnkStruct_020d433c.hpp"
#include "Game/SaveData.hpp"
#include "System/FileSystem.hpp"
#include "UI/SelectWindow.hpp"
#include "UI/MessageScreen.hpp"
#include <nitro/gx.h>
#include <nitro/mi.h>
#include <nitro/os.h>

static WindowCellRecord sSaveWindowCells[4] = {
    {"/Game/2D/union_b/union_b_obj_window_nce.bin", 3, 0, 0, 0, 1, {3, 5, 4, 0}},
    {"/Game/2D/union_b/union_b_obj_window_nce.bin", 7, 0, 0, 0, 1, {6, 8, 7, 0}},
    {"/Game/2D/union_b/union_b_obj_window_nce.bin", 10, 0, 0, 0, 1, {10, 10, 10, 0}},
    {"/Game/2D/union_b/union_b_obj_window_nce.bin", 12, 0, 0, 0, 1, {12, 12, 12, 0}},
};

#include "System/FrameTask.hpp"

SaveWindow::SaveWindow(u32 unk1, u32 unk2, u32 unk3, u32 unk4, u32 unk5, u32 unk6, u32 unk7, u32 unk8,
                                       u32 unk9) :
    SceneBase(2, 0) {
    mWindow = NULL;
    mMessages = NULL;
    mTextBuffer = NULL;
    mWindowCharOffset = unk3;
    mWindowPaletteOffset = unk4;
    mTextCharOffset = unk6;
    mTextPaletteOffset = unk7;
    mCellCharOffset = unk8;
    mCellPaletteOffset = *(u16 *) &unk9;
    mScreen = unk1;
    mWindowBg = unk2;
    mTextBg = unk5;
    mState = 0;
    FrameTask::Get().StopAllChannels();
}

SaveWindow::~SaveWindow() {
    if (mMessages != NULL) {
        OS_Free(mMessages);
    }
    if (mTextBuffer != NULL) {
        OS_Free(mTextBuffer);
    }
    if (mWindow != NULL) {
        delete mWindow;
    }
    switch (mTextBg) {
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

extern const u32 gBgCharUploadKinds[];

static inline void SetCellPos(SelectWindow *window, int index, s16 x, s16 y) {
    window->mCellPos[index].x = x;
    window->mCellPos[index].y = y;
}

// Writes the save data (vtable 0x020a54a8; defined in FileLoader.cpp). Created once the save dialog is done.
class SaveWriter : public SceneBase {
public:
    SaveWriter() :
        SceneBase(0xffdc, 0) {
        ((u8 *) mContext)[0x23] = 0;
    }
    virtual ~SaveWriter();
    virtual void Update();
};

void SaveWindow::Update() {
    switch (mState) {
        case 0:
            if (func_0203dd0c(&FrameTask::Get(), 0) != 0) {
                break;
            }
            Load();
            if (SaveManager::Get().GetSlotStatus(0) == SAVE_STATUS_OK) {
                PrintMessage(0);
            } else {
                PrintMessage(9);
            }
            mState++;
            break;
        case 1:
            if ((u8) mWindow->Update(1) != 1) {
                break;
            }
            switch (mWindow->GetSelectedId()) {
                case 0:
                    mState++;
                    break;
                case 1:
                    mState = 5;
                    break;
            }
            break;
        case 2:
            FrameTask::Get().StopAllChannels();
            SetCellPos(mWindow, 3, 0x80, 0x60);
            SetCellPos(mWindow, 0, 0x100, 0xc0);
            SetCellPos(mWindow, 1, 0x100, 0xc0);
            PrintMessage(1);
            mState++;
            break;
        case 3:
            if (func_0203dd0c(&FrameTask::Get(), 0) != 0) {
                break;
            }
            gSaveData[0].mLanguage = data_020d433c.mLanguage;
            SaveManager::Get();
            new SaveWriter();
            mState++;
            break;
        case 4:
            if (mContext->mRequest == 0) {
                break;
            }
            if (mContext->mRequest == SAVE_STATUS_ERROR) {
                new MessageScreen(MESSAGE_SCREEN_SYSTEM_4_HALT);
            } else {
                mState = 5;
            }
            break;
        case 5:
            SpriteManager::Get().StartFade(FADE_MODE_DOWN);
            mState++;
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

extern const u32 gBgScrUploadKinds[];
extern const WindowItemRecord gSaveWindowItems[];

// Loads the window files, creates the SelectWindow, and sets the BG offsets/priorities, visible planes, blend
// brightness and SpriteManager::sFade/sBrightness (:259-266, :321-328).
void SaveWindow::Load() {
    ChangeDir("/Game/2D/union_b/");
    LoadAndQueueUpload(mScreen, UPLOAD_KIND_BG_EXT_PLTT, mWindowPaletteOffset, "union_b_bg_window_ncl.bin", 0, 0, 0);
    LoadAndQueueUpload(mScreen, gBgCharUploadKinds[mWindowBg], mWindowCharOffset, "union_b_bg_window_ncg.bin", 0, 0, 0);
    u32 size;
    u16 *tiles = (u16 *) LoadFile("union_b_bg_window_a_nsc.bin", 0, 0, 0, &size, 0);
    size >>= 1;
    u16 tileAdd = mWindowCharOffset >> 6;
    u16 palAdd  = (mWindowPaletteOffset & 0x1fff) / 0x200;
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
    QueueUpload(mScreen, gBgScrUploadKinds[mWindowBg], 0, tiles, size * 2, 1);
    ChangeDir("/");
    mMessages = (MessageFile *) LoadFile("/font/mes_system.mes", 0, 0, 0, 0, 0);
    TextManager::Get().func_0202ce44(mScreen, 0, (mTextPaletteOffset >> 5) & 0xff, 0);
    TextManager::Get();
    u16 *buffer = (u16 *) OS_Alloc(0x600);
    MIi_CpuClear16((u16) ((mTextCharOffset >> 5) | ((mTextPaletteOffset >> 5) << 12)), buffer, 0x600);
    TextManager::Get().FillScrChars(0, buffer, (mTextPaletteOffset >> 5) & 0xff, (u16) mTextCharOffset, 0x18, 0x18, 0x20);
    QueueUpload(mScreen, gBgScrUploadKinds[mTextBg], 0, buffer, 0x600, 1);
    TextManager::Get();
    mTextBufferSize = 0x4000;
    mTextBuffer = OS_Alloc(mTextBufferSize);
    SpriteScreen &screen = SpriteManager::Get().Screen(mScreen);
    screen.LoadCharacters("/Game/2D/union_b/union_b_obj_window.nce_ncg1D.bin", mCellCharOffset, 0);
    screen.LoadPalette("/Game/2D/union_b/union_b_obj_window_ncl.bin", mCellPaletteOffset, 0);
    for (u8 i = 0; i < 4; i++) {
        sSaveWindowCells[i].mCharOffset = mCellCharOffset;
        sSaveWindowCells[i].mPaletteBase = mCellPaletteOffset / 32;
    }
    mWindow = new SelectWindow(4, gSaveWindowItems, 1);
    mWindow->SetCells(4, sSaveWindowCells);
    u16 mask = 0;
    if (mScreen == 0) {
        u16 plane = (reg_GX_DISPCNT & GX_DISPCNT_PLANE_MASK) >> GX_DISPCNT_PLANE_SHIFT;
        switch (mWindowBg) {
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
        switch (mTextBg) {
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
        SpriteManager::sFade[0]       = FADE_MODE_HOLD;
        SpriteManager::sBrightness[0] = 0;
        SpriteManager::sFade[1]        = FADE_MODE_HOLD;
        SpriteManager::sBrightness[1]  = -8;
    } else {
        u16 plane = (reg_GXS_DB_DISPCNT & GX_DISPCNT_PLANE_MASK) >> GX_DISPCNT_PLANE_SHIFT;
        switch (mWindowBg) {
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
        switch (mTextBg) {
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
        SpriteManager::sFade[0]       = FADE_MODE_HOLD;
        SpriteManager::sBrightness[0] = -8;
        SpriteManager::sFade[1]        = FADE_MODE_HOLD;
        SpriteManager::sBrightness[1]  = 0;
    }
}

void SaveWindow::PrintMessage(u32 index) {
    MI_CpuClearFast(mTextBuffer, mTextBufferSize);
    TextManager &text = TextManager::Get();
    u16 pixelWidth;
    s16 width;
    s16 height;
    height = 0x38;
    width  = 0;
    MessageFile *file = mMessages;
    text.Print(file->GetText(index), file->GetLength(index), mTextBuffer, 0x18, 0x10, &width, &height, &pixelWidth, 0, 2, 0,
               0x10, 0, 0, 1, 1, 1);
    QueueUpload(mScreen, gBgCharUploadKinds[mTextBg], mTextCharOffset, mTextBuffer, mTextBufferSize, 0);
    s16 offset = (pixelWidth - 0x100) / 2;
    switch (mTextBg) {
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
