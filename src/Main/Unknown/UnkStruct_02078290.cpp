#include "Unknown/UnkStruct_02078290.hpp"
#include "Graphics/CellAnimation.hpp"
#include "Graphics/Camera.hpp"
#include "Graphics/SpriteManager.hpp"
#include "System/WirelessManager.hpp"
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
extern const AntennaLayout data_020a30f8[];
extern const AntennaPos data_020a30dc[];

UnkStruct_02078290::UnkStruct_02078290(u32 unk1) :
    SceneBase(0x384, 0) {
    mUnk_0c = NULL;
    mUnk_10 = NULL;
    mUnk_19 = unk1;
    mUnk_1a = 0;
    func_020785d8(unk1);
}

UnkStruct_02078290::~UnkStruct_02078290() {
    delete mUnk_0c;
    delete mUnk_10;
}

// non-matching: 12 words, scheduling of the two CameraManager mode loads in the (mode == 2) | (mode == 3) test (the original loads both before comparing)
void UnkStruct_02078290::Update() {
    u32 frame;
    CameraManager &cam = CameraManager::Get();
    if ((cam.mMode == 2) | (CameraManager::Get().mMode == 3)) {
        if (data_020a30f8[mUnk_19].mScreen == 0) {
            if (CameraManager::Get().mActive == 1) {
                return;
            }
        } else {
            if (CameraManager::Get().mActive == 0) {
                return;
            }
        }
        frame = 0;
    } else {
        frame = data_020a30f8[mUnk_19].mScreen;
    }
    WirelessManager::Get();
    mUnk_0c->func_0202a3ec(frame, data_020c4030.mUnk_24d, data_020a30dc[mUnk_19].mY, data_020a30dc[mUnk_19].mX, 0);
    if (mUnk_1a) {
        mUnk_10->Draw(frame, data_020a30dc[mUnk_19].mY, data_020a30dc[mUnk_19].mX, 0);
    }
}

// non-matching: 5 words, same CameraManager mode-load scheduling as Update
void UnkStruct_02078290::func_020785d8(u32 unk1) {
    mUnk_19 = unk1;
    u32 width = 0x80;
    CameraManager &cam = CameraManager::Get();
    u32 screen;
    if ((cam.mMode == 2) | (CameraManager::Get().mMode == 3)) {
        screen = 0;
        width  = 0x20;
    } else {
        screen = data_020a30f8[mUnk_19].mScreen;
    }
    SpriteScreen &sprites = SpriteManager::Get().Screen(screen);
    sprites.LoadCharacters("/Game/2D/antenna/antenna.nce_ncg1D.bin", data_020a30f8[mUnk_19].mCharOffset, 0);
    TextManager::Get().SetColor(screen, 0, data_020a30f8[mUnk_19].mPalette);
    delete mUnk_0c;
    delete mUnk_10;
    mUnk_0c = new CellAnimation("/Game/2D/antenna/antenna_nce.bin", 0, width, 0x80);
    mUnk_0c->mCharBase = sprites.func_020276f4(data_020a30f8[mUnk_19].mCharOffset);
    mUnk_0c->mUnk_11   = data_020a30f8[mUnk_19].mPalette;
    mUnk_10 = new CellAnimation("/Game/2D/antenna/antenna_nce.bin", 1, width, 0x80);
    mUnk_10->mCharBase = sprites.func_020276f4(data_020a30f8[mUnk_19].mCharOffset);
    mUnk_10->mUnk_11   = data_020a30f8[mUnk_19].mPalette;
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

extern "C" void func_01ff94e0(const void *src, void *dst, u32 size);
extern "C" void MI_CpuCopy8(const void *src, void *dst, u32 size);
void func_02078ab8(u32 screen, u32 palette, const AntennaPalette *colors, u32 unk4);
void func_02078a48(u8 *dst, const u8 *src);

void func_0207889c(u32 screen, u32 offset, u32 palette, const void *colors, u32 unk5) {
    const AntennaPalette *pal = (const AntennaPalette *) colors;
    u8 *file;
    u8 *buffer = (u8 *) OS_Alloc(0x200);
    ChangeDir("/Game/Character/EditedIcon/");
    u32 size;
    file = (u8 *) func_02023504("icon_ch_edit.nce_ncg1D.bin", 0, 0, 0, &size, 0);
    u8 a     = pal->mUnk_00_1;
    u8 b     = pal->mUnk_00_7;
    u8 c     = pal->mUnk_00_13;
    u8 d     = pal->mUnk_02_3;
    u8 e     = pal->mUnk_02_9;
    MI_CpuCopy8(file + a * 0x200, buffer, 0x200);
    if (d != 0) {
        if (e != 0) {
            func_02078a48(buffer, file + ((d - 1) * 0x200 + 0x2e00));
        }
        func_02078a48(buffer, file + (d * 0x200 + 0x2400));
    }
    if (c != 0) {
        func_02078a48(buffer, file + (c * 0x200 + 0x1800));
    } else {
        func_02078a48(buffer, file + (b * 0x200 + 0x800));
    }
    DC_FlushRange(buffer, 0x200);
    func_02025c1c(screen, 4, offset, buffer, 0x200, 1);
    OS_Free(file);
    func_02078ab8(screen, palette, pal, unk5);
    ChangeDir("/");
}

void func_02078a48(u8 *dst, const u8 *src) {
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

// non-matching: 21 words. The original computes the last `pos += 2` before freeing the "fr" file and keeps it in r9;
// here it is sunk below the "len" load into a scratch register. The final call reloads `screen` last instead of first.
void func_02078ab8(u32 screen, u32 palette, const AntennaPalette *colors, u32 unk4) {
    u32 h;
    u32 i;
    u32 d;
    u32 f;
    u32 b;
    u8 *buffer;
    u8 *file;
    u32 j16;
    u32 size;
    u8 pos;
    u8 *extra;
    buffer = (u8 *) OS_Alloc(0x20);
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
        file = (u8 *) func_02023504("icon_ch_edit_ncl.bin", 0, 0, 0, &size, 0);
        func_01ff94e0(file, buffer, 0x20);
        u8 *p = (u8 *) func_02023504("icon_ch_edit_skin_ncl.bin", 0, 0, 0, &size, 0);
        func_01ff94e0(p + (j16 + 1) * 2, buffer + 2, 6);
        pos += 3;
        OS_Free(p);
        p = (u8 *) func_02023504("icon_ch_edit_eye_ncl.bin", 0, 0, 0, &size, 0);
        func_01ff94e0(p + (pos + b * 16) * 2, buffer + pos * 2, 6);
        pos += 3;
        OS_Free(p);
        p = (u8 *) func_02023504("icon_ch_edit_hair_ncl.bin", 0, 0, 0, &size, 0);
        func_01ff94e0(p + (pos + d * 16) * 2, buffer + pos * 2, 6);
        pos += 3;
        OS_Free(p);
        p = (u8 *) func_02023504("icon_ch_edit_cap_ncl.bin", 0, 0, 0, &size, 0);
        func_01ff94e0(p + (pos + f * 16) * 2, buffer + pos * 2, 4);
        pos += 2;
        OS_Free(p);
        p = (u8 *) func_02023504("icon_ch_edit_fr_ncl.bin", 0, 0, 0, &size, 0);
        func_01ff94e0(p + (pos + h * 16) * 2, buffer + pos * 2, 4);
        pos += 2;
        OS_Free(p);
        extra = (u8 *) func_02023504("icon_ch_edit_len_ncl.bin", 0, 0, 0, &size, 0);
        func_01ff94e0(extra + (pos + i * 16) * 2, buffer + pos * 2, 2);
    } else {
        file = (u8 *) func_02023504("icon_ch_edit_grayscale_ncl.bin", 0, 0, 0, &size, 0);
        func_01ff94e0(file, buffer, 0x20);
    }
    DC_FlushRange(buffer, 0x20);
    func_02025c1c(screen, 5, palette << 5, buffer, 0x20, 1);
    if (extra != NULL) {
        OS_Free(extra);
    }
    if (file != NULL) {
        OS_Free(file);
    }
    ChangeDir("/");
}
