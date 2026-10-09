#include "Graphics/CellAnimation.hpp"
#include "Graphics/SpriteManager.hpp"
#include "System/FileSystem.hpp"
#include "Unknown/UnkFunctions.hpp"

#include <nitro/fx.h>
#include <nitro/gx.h>
#include <nitro/mi.h>
#include <nitro/os.h>


extern "C" void GX_LoadOAM(const void *src, u32 offset, u32 size);

extern "C" void GXS_LoadOAM(const void *src, u32 offset, u32 size);

struct OamEntry {
    u32 mAttr01;
    u16 mAttr2;
    u16 mPad;
};

struct OamSource {
    u32 mAttr01;
    u16 mAttr2;
    s16 mX;
    s16 mY;
    u16 mPad;
};

void func_02027c18(void *slot_, SpriteTransform *transform);

void func_02027cf4(s32 *x, s32 *y, s32 shape, SpriteTransform *transform);

extern "C" fx32 func_01ff8000(fx32 num, fx32 den);

void func_02027c64(MtxFx22 *out, SpriteTransform *transform);

void func_02000ccc(MtxFx22 *mtx, fx32 x, fx32 y);

extern "C" void MTX_Concat22(const MtxFx22 *a, const MtxFx22 *b, MtxFx22 *out);

SpriteScreen::SpriteScreen() {
    mUnk_400 = 0;
    mBgIndex = 0;
    mUnk_403 = 9;
    func_02027750();
}

void SpriteScreen::func_020275e8(u32 unk1) {
    mBgIndex = unk1;
}

void SpriteScreen::func_020275f0() {
    if (mBgIndex == 0) {
        GX_LoadOAM(this, 0, 0x400);
    } else {
        GXS_LoadOAM(this, 0, 0x400);
    }
    func_02027750();
}

static u16 data_020a5624[10] = {0x20, 0x20, 0x40, 0x80, 0x100, 0x80, 0x100, 0x80, 0x100, 0x100};
static u32 data_020a5638[9]  = {0x0, 0x10, 0x100010, 0x200010, 0x300010, 0x40, 0x400040, 0x0, 0x20};

// non-matching: store and scheduling order (the ROM's store order fits inlined OBJ VRAM mode setters).
void SpriteScreen::func_02027624(s32 mode) {
    mUnk_403 = mode;
    mUnk_400 = data_020a5624[mode];
    if (mode < 5) {
        if (mBgIndex == 0) {
            reg_GX_DISPCNT = (reg_GX_DISPCNT & 0xffcfffef) | data_020a5638[mode];
        } else {
            reg_GXS_DB_DISPCNT = (reg_GXS_DB_DISPCNT & 0xffcfffef) | data_020a5638[mode];
        }
    } else {
        if (mBgIndex == 0) {
            reg_GX_DISPCNT = (reg_GX_DISPCNT & 0xffbfff9f) | data_020a5638[mode];
        } else {
            reg_GXS_DB_DISPCNT = (reg_GXS_DB_DISPCNT & ~0x60) | data_020a5638[mode];
        }
    }
}

// non-matching: 4 bytes long: the original returns the value untruncated, but callers need the u16 return type.
u16 SpriteScreen::func_020276f4(u32 offset) {
    u32 result = offset;
    if (mUnk_403 < 7) {
        result = result / mUnk_400;
    } else {
        u32 size = mUnk_400;
        u32 rem  = result % (size * 2);
        result   = (result / (size * 8)) * 32 + rem / 16;
    }
    return result;
}




void SpriteScreen::func_02027750() {
    MIi_CpuClear16(0xc0, this, 0x400);
    mUnk_404 = 0;
    mUnk_405 = 0;
}

// non-matching: same size; loop-head register allocation and stack slots.
void SpriteScreen::func_02027778(void *buffer, u8 count, u32 unk3, u32 unk4, u32 unk5, u32 unk6, u32 unk7, u32 unk8) {
    OamEntry *entry = (OamEntry *) this + mUnk_404;
    OamSource *source = (OamSource *) buffer;
    SpriteTransform *transform = (SpriteTransform *) unk8;
    u8 i;
    u32 matrix;
    matrix = i = 0;
    if (transform != NULL) {
        func_02027c18((u8 *) this + mUnk_405 * 0x20, transform);
        matrix = mUnk_405;
        mUnk_405 = matrix + 1;
    }
    s8 paletteBase = *(s8 *) &unk7;
    u16 tileBase = *(u16 *) &unk6;
    s16 offsetY = *(s16 *) &unk4;
    u8 priority = *(u8 *) &unk5;
    for (; ; ) {
        if (mUnk_404 >= 0x80) {
            return;
        }
        s32 attr2 = source->mAttr2;
        s32 palette = paletteBase + ((attr2 & 0xf000) >> 12);
        s32 x = source->mX;
        s32 tile = (attr2 & 0x3ff) + tileBase;
        u32 attr = source->mAttr01;
        u32 mode = (attr & 0xc00) >> 10;
        u32 flip = attr & 0x30000300;
        s32 shape = attr & 0xc000c000;
        s32 y = source->mY;
        if (transform != NULL) {
            func_02027cf4(&x, &y, shape, transform);
            flip = 0x300;
        } else if ((flip == 0x300) | (flip == 0x100)) {
            SpriteTransform identity(0x1000, 0x1000, 0);
            func_02027c18((u8 *) this + mUnk_405 * 0x20, &identity);
            matrix = *(volatile u8 *) &mUnk_405;
            mUnk_405 = mUnk_405 + 1;
        }
        s16 cull;
        if (transform != NULL) {
            s16 base = source->mY + offsetY;
            s32 height;
            switch (shape) {
            case 0:
                height = 0x8000;
                break;
            case 0x40000000:
                height = 0x10000;
                break;
            case (s32) 0x80000000:
                height = 0x20000;
                break;
            case (s32) 0xC0000000:
                height = 0x40000;
                break;
            case 0x4000:
                height = 0x8000;
                break;
            case 0x40004000:
                height = 0x8000;
                break;
            case (s32) 0x80004000:
                height = 0x10000;
                break;
            case (s32) 0xC0004000:
                height = 0x20000;
                break;
            case 0x8000:
                height = 0x10000;
                break;
            case 0x40008000:
                height = 0x20000;
                break;
            case (s32) 0x80008000:
                height = 0x20000;
                break;
            case (s32) 0xC0008000:
                height = 0x40000;
                break;
            }
            s16 adjust = (float) func_01ff8000(height - FX_Mul(height, transform->mScaleY), 0x2000) / 4096.0f;
            if (source->mY >= 0) {
                cull = base - adjust;
            } else {
                cull = adjust + base;
            }
        } else {
            cull = y + offsetY;
        }
        if ((cull > -0x40) & (cull < 0xc0)) {
            u32 colorBit = (source->mAttr01 & 0x2000) >> 13;
            u32 mosaic = (source->mAttr01 & 0x1000) >> 12;
            s32 px = (y + offsetY);
            s32 py = x + unk3;
            u32 value;
            if (flip == 0x100 || flip == 0x300) {
                if (mode == 3) {
                    value = (px & 0xff) | (matrix << 25) | (mode << 10) | (mosaic << 12) | shape | ((py & 0x1ff) << 16) | flip;
                    entry->mAttr01 = value;
                } else {
                    value = (colorBit << 13) | (matrix << 25) | (px & 0xff) | (mode << 10) | (mosaic << 12) | shape | ((py & 0x1ff) << 16) | flip;
                    entry->mAttr01 = value;
                }
            } else {
                if (mode == 3) {
                    value = (px & 0xff) | (mode << 10) | (mosaic << 12) | shape | ((py & 0x1ff) << 16) | flip;
                    entry->mAttr01 = value;
                } else {
                    value = (px & 0xff) | (colorBit << 13) | (mode << 10) | (mosaic << 12) | shape | ((py & 0x1ff) << 16) | flip;
                    entry->mAttr01 = value;
                }
            }
            entry->mAttr2 = tile | (priority << 10) | (palette << 12);
            mUnk_404++;
            entry++;
        }
        source++;
        i++;
        if (i >= count) {
            break;
        }
    }
}

void func_02027c18(void *slot_, SpriteTransform *transform) {
    u8 *slot = (u8 *) slot_;
    MtxFx22 mtx;
    func_02027c64(&mtx, transform);
    *(s16 *) (slot + 0x06) = mtx._[0] >> 4;
    *(s16 *) (slot + 0x0e) = mtx._[1] >> 4;
    *(s16 *) (slot + 0x16) = mtx._[2] >> 4;
    *(s16 *) (slot + 0x1e) = mtx._[3] >> 4;
}

void func_02027c64(MtxFx22 *out, SpriteTransform *transform) {
    MtxFx22 scale;
    MtxFx22 rotation;
    fx32 y = func_01ff8000(0x1000, transform->mScaleY);
    fx32 x = func_01ff8000(0x1000, transform->mScaleX);
    func_02000ccc(&scale, x, y);
    s32 index = transform->mRotation >> 4;
    fx32 sin = FX_SinCosTable_[index * 2];
    fx32 cos = FX_SinCosTable_[index * 2 + 1];
    rotation._[0] = cos;
    rotation._[1] = sin;
    rotation._[2] = -sin;
    rotation._[3] = cos;
    MTX_Concat22(&scale, &rotation, out);
}






// non-matching: register allocation only.
void func_02027cf4(s32 *x, s32 *y, s32 shape, SpriteTransform *transform) {
    fx32 cx = *x << 12;
    fx32 cy = *y << 12;
    fx32 width;
    fx32 height;
    switch (shape) {
    case 0:
        width = 0x4000;
        height = 0x4000;
        break;
    case 0x4000:
        width = 0x8000;
        height = 0x4000;
        break;
    case 0x8000:
        width = 0x4000;
        height = 0x8000;
        break;
    case 0x40000000:
        width = 0x8000;
        height = 0x8000;
        break;
    case (s32) 0x80008000:
        width = 0x8000;
        height = 0x10000;
        break;
    case 0x40004000:
        width = 0x10000;
        height = 0x4000;
        break;
    case 0x40008000:
        width = 0x4000;
        height = 0x10000;
        break;
    case (s32) 0x80000000:
        width = 0x10000;
        height = 0x10000;
        break;
    case (s32) 0x80004000:
        width = 0x10000;
        height = 0x8000;
        break;
    case (s32) 0xC0000000:
        width = 0x20000;
        height = 0x20000;
        break;
    case (s32) 0xC0004000:
        width = 0x20000;
        height = 0x10000;
        break;
    case (s32) 0xC0008000:
        width = 0x10000;
        height = 0x20000;
        break;
    }
    *x = cx >> 12;
    *y = cy >> 12;
    fx32 px = FX_Mul(cx + width, transform->mScaleX);
    fx32 py = FX_Mul(cy + height, transform->mScaleY);
    s32 index = transform->mRotation >> 4;
    fx32 sin = FX_SinCosTable_[index * 2];
    fx32 cos = FX_SinCosTable_[index * 2 + 1];
    *x = (FX_Mul(px, cos) - FX_Mul(py, sin)) >> 12;
    *y = (FX_Mul(px, sin) + FX_Mul(py, cos)) >> 12;
    *x = *x - ((width << 1) >> 12);
    *y = *y - ((height << 1) >> 12);
}

SpriteTransform::SpriteTransform(s32 scaleX, s32 scaleY, u16 rotation) {
    mScaleX   = scaleX;
    mScaleY   = scaleY;
    mRotation = rotation;
}

void SpriteScreen::LoadCharacters(const char *path, u32 offset, u32 unk3) {
    u32 size;
    void *data = func_02023504(path, 0, 0, 0, &size, 0);
    if (unk3 != 0) {
        size = unk3;
    }
    func_02025c1c(mBgIndex, 4, offset, data, size, 1);
}


void SpriteScreen::LoadPalette(const char *path, u32 offset, u32 unk3) {
    u32 size;
    void *data = func_02023504(path, 0, 0, 0, &size, 0);
    if (unk3 != 0) {
        size = unk3;
    }
    func_02025c1c(mBgIndex, 5, offset, data, size, 1);
}






extern "C" fx32 func_01ff8000(fx32 num, fx32 den);
float _fdiv(float a, float b);

void SpriteScreen::func_02027fec(const char *path, u32 unk2, u32 unk3) {
    u32 size;
    void *data = func_02023504(path, 0, 0, 0, &size, 0);
    if (unk3 != 0) {
        size = unk3;
    }
    func_02025c1c(mBgIndex, 0, unk2, data, size, 1);
}



// non-matching: 4 bytes long; register allocation.
void *SpriteScreen::func_02028054(u8 *data) {
    u8 *buffer = (u8 *) OS_Alloc(0x1000);
    MI_CpuClear32(buffer, 0x1000);
    u16 i = 0;
    do {
        u8 a = i % 32;
        u8 b = i / 32;
        u8 x = a * 2;
        u8 y = b * 2;
        u16 source = x % 8 + y % 8 * 8 + (y / 64) * 4096 + (x / 8) % 8 * 64 + (y / 8) % 8 * 512;
        u16 dest   = a % 8 + (a / 8) % 8 * 64 + (b / 8) % 8 * 512 + b % 8 * 8 + (b / 64) * 4096;
        buffer[dest] = (data + source)[0x1000];
        i++;
    } while (i < 0x800);
    u16 j = 0;
    do {
        u8 a = j % 16;
        u8 b = j / 16;
        u8 x = a * 2;
        u8 y = b * 2;
        u16 source = (y / 8) % 8 * 256 + (x / 8) % 8 * 64 + y % 8 * 8 + x % 8 + (y / 64) * 2048;
        u16 dest   = a % 8 + (a / 8) % 8 * 64 + (b / 64) * 4096 + (b / 8) % 8 * 512 + b % 8 * 8;
        (buffer + dest)[0x100] = data[source];
        j++;
    } while (j < 0x400);
    func_02005e9c(buffer, 0x1000);
    OS_Free(data);
    return buffer;
}
