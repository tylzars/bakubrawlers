#include "Graphics/AntennaIndicator.hpp"
#include "Graphics/CellAnimation.hpp"
#include "Graphics/SpriteManager.hpp"
#include "System/WirelessManager.hpp"
#include "Graphics/Camera.hpp"
#include "System/FileSystem.hpp"
#include "Text/TextManager.hpp"
#include <nitro/os.h>
#include "Unknown/UnkFunctions.hpp"

struct AntennaLayout {
    /* 00 */ u32 mCharOffset;
    /* 04 */ u8 mScreen;
    /* 05 */ u8 mPalette;
    /* 06 */ PAD(0x06, 0x08);
    /* 08 */
};
struct AntennaPos {
    /* 00 */ u8 mY;
    /* 01 */ u8 mX;
};
extern const AntennaLayout gAntennaLayouts[];
extern const AntennaPos gAntennaPositions[];

AntennaIndicator::AntennaIndicator(u32 unk1) :
    SceneBase(0x384, 0) {
    mFirstCellAnim = NULL;
    mSecondCellAnim = NULL;
    mLayoutIndex = unk1;
    mSecondVisible = 0;
    SetupLayout(unk1);
}

AntennaIndicator::~AntennaIndicator() {
    delete mFirstCellAnim;
    delete mSecondCellAnim;
}

void AntennaIndicator::Update() {
    u32 frame;
    CameraManager &camB = CameraManager::Get();
    CameraManager &camA = CameraManager::Get();
    CameraMode modeB = (CameraMode) camB.mMode;
    CameraMode modeA = (CameraMode) camA.mMode;
    if ((modeB == CAMERA_MODE_2) | (modeA == CAMERA_MODE_3)) {
        if (gAntennaLayouts[mLayoutIndex].mScreen == 0) {
            if (CameraManager::Get().mCameraIndex == 1) {
                return;
            }
        } else {
            if (CameraManager::Get().mCameraIndex == 0) {
                return;
            }
        }
        frame = 0;
    } else {
        frame = gAntennaLayouts[mLayoutIndex].mScreen;
    }
    WirelessManager::Get();
    mFirstCellAnim->DrawFrame(frame, data_020c4030.mUnk_24d, gAntennaPositions[mLayoutIndex].mY, gAntennaPositions[mLayoutIndex].mX, 0);
    if (mSecondVisible) {
        mSecondCellAnim->Draw(frame, gAntennaPositions[mLayoutIndex].mY, gAntennaPositions[mLayoutIndex].mX, 0);
    }
}

void AntennaIndicator::SetupLayout(u32 unk1) {
    mLayoutIndex = unk1;
    u16 width = 0x80;
    CameraManager &camB = CameraManager::Get();
    CameraManager &camA = CameraManager::Get();
    u32 screen;
    CameraMode modeB = (CameraMode) camB.mMode;
    CameraMode modeA = (CameraMode) camA.mMode;
    if ((modeB == CAMERA_MODE_2) | (modeA == CAMERA_MODE_3)) {
        screen = 0;
        width  = 0x20;
    } else {
        screen = gAntennaLayouts[mLayoutIndex].mScreen;
    }
    SpriteScreen &sprites = SpriteManager::Get().Screen(screen);
    sprites.LoadCharacters("/Game/2D/antenna/antenna.nce_ncg1D.bin", gAntennaLayouts[mLayoutIndex].mCharOffset, 0);
    TextManager::Get().SetColor(screen, 0, gAntennaLayouts[mLayoutIndex].mPalette);
    delete mFirstCellAnim;
    delete mSecondCellAnim;
    mFirstCellAnim = new CellAnimation("/Game/2D/antenna/antenna_nce.bin", 0, width, 0x80);
    mFirstCellAnim->mCharBase = sprites.GetCharBase(gAntennaLayouts[mLayoutIndex].mCharOffset);
    mFirstCellAnim->mPaletteBase   = gAntennaLayouts[mLayoutIndex].mPalette;
    mSecondCellAnim = new CellAnimation("/Game/2D/antenna/antenna_nce.bin", 1, width, 0x80);
    mSecondCellAnim->mCharBase = sprites.GetCharBase(gAntennaLayouts[mLayoutIndex].mCharOffset);
    mSecondCellAnim->mPaletteBase   = gAntennaLayouts[mLayoutIndex].mPalette;
}

// Packed palette selection of the character icon: 3-bit indices into the palette files.
struct AntennaPalette {
    /* 00 */ u16 mUnk_00_0 : 1;
    /* 00 */ u16 mUnk_00_1 : 3;
    /* 00 */ u16 mUnk_00_4 : 3;
    /* 00 */ u16 mUnk_00_7 : 3;
    /* 00 */ u16 mUnk_00_10 : 3;
    /* 00 */ u16 mUnk_00_13 : 3;
    /* 02 */ u16 mUnk_02_0 : 3;
    /* 02 */ u16 mUnk_02_3 : 3;
    /* 02 */ u16 mUnk_02_6 : 3;
    /* 02 */ u16 mUnk_02_9 : 3;
    /* 02 */ u16 mUnk_02_12 : 4;
    /* 04 */ u16 mUnk_04;
    /* 06 */ u16 mUnk_06_0 : 3;
    /* 06 */ u16 mUnk_06_3 : 13;
    /* 08 */
};

extern "C" void MIi_CpuCopy16(const void *src, void *dst, u32 size);
extern "C" void MI_CpuCopy8(const void *src, void *dst, u32 size);
void LoadEditedIconPalette(u8 screen, u32 palette, const AntennaPalette *colors, u32 unk4);
void CopyNonZeroNibbles(u8 *dst, const u8 *src);

void LoadEditedIcon(u8 screen, u32 offset, u32 palette, const void *colors, u32 unk5) {
    const AntennaPalette *pal = (const AntennaPalette *) colors;
    u8 *file;
    u8 *buffer = (u8 *) OS_Alloc(0x200);
    ChangeDir("/Game/Character/EditedIcon/");
    u32 size;
    file = (u8 *) LoadFile("icon_ch_edit.nce_ncg1D.bin", 0, 0, 0, &size, 0);
    u8 a     = pal->mUnk_00_1;
    u8 b     = pal->mUnk_00_7;
    u8 c     = pal->mUnk_00_13;
    u8 d     = pal->mUnk_02_3;
    u8 e     = pal->mUnk_02_9;
    MI_CpuCopy8(file + a * 0x200, buffer, 0x200);
    if (d != 0) {
        if (e != 0) {
            CopyNonZeroNibbles(buffer, file + ((d - 1) * 0x200 + 0x2e00));
        }
        CopyNonZeroNibbles(buffer, file + (d * 0x200 + 0x2400));
    }
    if (c != 0) {
        CopyNonZeroNibbles(buffer, file + (c * 0x200 + 0x1800));
    } else {
        CopyNonZeroNibbles(buffer, file + (b * 0x200 + 0x800));
    }
    DC_FlushRange(buffer, 0x200);
    QueueUpload(screen, 4, offset, buffer, 0x200, 1);
    OS_Free(file);
    LoadEditedIconPalette(screen, palette, pal, unk5);
    ChangeDir("/");
}

void CopyNonZeroNibbles(u8 *dst, const u8 *src) {
    for (u16 i = 0; i < 0x400; i++) {
        if (i & 1) {
            u8 v = *src & 0xf;
            if (v != 0) {
                u8 t = *dst & 0xf0;
                *dst = t | v;
            }
            dst++;
            src++;
        } else {
            u8 v = *src & 0xf0;
            if (v != 0) {
                *dst = (*dst & 0xf) | v;
            }
        }
    }
}

void LoadEditedIconPalette(u8 screen, u32 palette, const AntennaPalette *colors, u32 unk4) {
    u32 h;
    u32 i;
    u32 d;
    u32 f;
    u32 b;
    u16 *buffer;
    u16 *file;
    u32 j16;
    u32 size;
    u8 pos;
    u16 *extra;
    buffer = (u16 *) OS_Alloc(0x20);
    j16    = colors->mUnk_06_0 << 4;
    b      = colors->mUnk_00_4;
    d      = colors->mUnk_00_10;
    f      = colors->mUnk_02_0;
    i      = colors->mUnk_02_9;
    h      = colors->mUnk_02_6;
    extra  = NULL;
    pos    = 1;
    ChangeDir("/Game/Character/EditedIcon/");
    if (unk4 == 0) {
        file = (u16 *) LoadFile("icon_ch_edit_ncl.bin", 0, 0, 0, &size, 0);
        MIi_CpuCopy16(file, buffer, 0x20);
        u16 *p = (u16 *) LoadFile("icon_ch_edit_skin_ncl.bin", 0, 0, 0, &size, 0);
        MIi_CpuCopy16(&p[pos + j16], &buffer[pos], 6);
        pos += 3;
        OS_Free(p);
        p = (u16 *) LoadFile("icon_ch_edit_eye_ncl.bin", 0, 0, 0, &size, 0);
        MIi_CpuCopy16(&p[pos + b * 16], &buffer[pos], 6);
        pos += 3;
        OS_Free(p);
        p = (u16 *) LoadFile("icon_ch_edit_hair_ncl.bin", 0, 0, 0, &size, 0);
        MIi_CpuCopy16(&p[pos + d * 16], &buffer[pos], 6);
        pos += 3;
        OS_Free(p);
        p = (u16 *) LoadFile("icon_ch_edit_cap_ncl.bin", 0, 0, 0, &size, 0);
        MIi_CpuCopy16(&p[pos + f * 16], &buffer[pos], 4);
        pos += 2;
        OS_Free(p);
        p = (u16 *) LoadFile("icon_ch_edit_fr_ncl.bin", 0, 0, 0, &size, 0);
        MIi_CpuCopy16(&p[pos + h * 16], &buffer[pos], 4);
        pos += 2;
        OS_Free(p);
        extra = (u16 *) LoadFile("icon_ch_edit_len_ncl.bin", 0, 0, 0, &size, 0);
        MIi_CpuCopy16(&extra[pos + i * 16], &buffer[pos], 2);
    } else {
        file = (u16 *) LoadFile("icon_ch_edit_grayscale_ncl.bin", 0, 0, 0, &size, 0);
        MIi_CpuCopy16(file, buffer, 0x20);
    }
    DC_FlushRange(buffer, 0x20);
    QueueUpload(screen, 5, palette << 5, buffer, 0x20, 1);
    if (extra != NULL) {
        OS_Free(extra);
    }
    if (file != NULL) {
        OS_Free(file);
    }
    ChangeDir("/");
}
