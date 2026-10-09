#include "Text/TextManager.hpp"
#include "Graphics/SpriteManager.hpp"
#include "System/FileSystem.hpp"
#include "Unknown/UnkFunctions.hpp"
#include <nitro/mi.h>

extern FontHeader data_020a598c;
extern GlyphMetrics data_020ab80c;
extern FontData data_020ab150;
extern u16 data_020a596c;
extern u8 data_020b48dc[];
extern "C" void OS_Terminate();
extern u32 data_020a56f0[];

// One glyph block of a text buffer registered as an OBJ: attribute word, tile number, position.
struct TextSprite {
    /* 00 */ u32 mAttr;
    /* 04 */ u16 mTile;
    /* 06 */ u16 mX;
    /* 08 */ u16 mY;
    /* 0a */ PAD(0x0a, 0x0c);
    /* 0c */
};

extern "C" void func_02005e9c(void *buffer, u32 size);

// non-matching: 5 words.
void TextManager::func_0202c02c() {
    mCursor  = &data_020a596c;
    *mCursor = 0;
    mFont    = &data_020a598c;
    mMetrics = &data_020ab80c;
    mData    = &data_020ab150;
    mUnk_59  = 0;
    mUnk_5a  = 6;
    mUnk_5b  = 0;
    u16 count = mData->mCodeSize >> 1;
    if (count > 0x8b) {
        OS_Terminate();
    }
    MI_CpuFill8(Widths(), 0, count);
    u8 g = 0;
    do {
        u8 y = 0;
        do {
            s8 x = 15;
            u8 *row = data_020b48dc + (y % 8) * 4 + (y / 8) * 0x40 + g * 0x80;
            do {
                u8 b = (row + (x % 8) / 2)[(x / 8) * 0x20];
                u8 v;
                if ((x & 1) != 0) {
                    v = (b & 0xf0) >> 4;
                } else {
                    v = b & 0xf;
                }
                if (v != 0) {
                    if (x + 1 > Widths()[g]) {
                        Widths()[g] = x + 1;
                    }
                }
                x--;
            } while (x >= 0);
            y++;
        } while (y < 16);
        g++;
    } while (g < count);
    u16 idx;
    u8 i = 1;
    idx = func_0202d1bc(*(u16 *) ((u8 *) mData + mData->mTables[0].mOffset + 0x20));
    do {
        Widths()[idx + i] = Widths()[idx];
        i++;
    } while (i < 10);
}

u16 TextManager::func_0202c1a8(u32 index) {
    if (index < 0x10) {
        return mPalette[index];
    }
    return 0;
}

// non-matching: prologue: order of the stack-argument copies and spill slots; the body is the same. MW copies
// each stack parameter onto itself in the prologue, in an order that follows the statements that first use them;
// the original's order is width, height, unk12, unk13, unk10, unk14, outWidth, outHeight, unk11, unk15, unk17,
// which suggests locals initialised from those parameters at the top of the function.
void TextManager::Print(const void *text, u16 length, void *buffer, u32 width, u32 height, s16 *outWidth,
                        s16 *outHeight, u16 *outPixelWidth, u32 unk9, u32 unk10, u32 unk11, u32 unk12, u32 unk13,
                        u32 unk14, u32 unk15, u32 unk16, u32 unk17) {
    s32 lastAdvance;
    s32 startY;
    s16 x;
    u16 pixelHeight;
    u8 savedFontMode;
    s32 fontHeight;
    s32 maxX;
    u16 pixelWidth;
    const u16 *line = (const u16 *) text;
    s16 y;
    u16 remaining;
    u16 charCount;
    s16 startX;
    u32 drawFlag;
    u32 more;
    s32 fontMode;
    u32 color = unk10;
    maxX        = 0;
    pixelWidth  = width * 8;
    pixelHeight = height * 8;
    mUnk_59       = unk15;
    savedFontMode = mUnk_5b;
    startX        = *outWidth;
    startY        = *outHeight;
    charCount     = maxX;
    drawFlag      = charCount;
    x             = startX;
    y             = startY;
    if (unk14 != 0) {
        unk17 = maxX;
    }
    if (mUnk_5b != 0) {
        fontHeight = 12;
    } else {
        fontHeight = mFont->mHeight;
    }
    remaining = length;
    do {
        s16 tx             = x;
        s16 ty             = y;
        const u16 *p       = line;
        u16 units          = 0;
        fontMode           = mUnk_5b;
        more               = 1;
        do {
            if (remaining == 0) {
                if (maxX < tx) {
                    maxX = (u16) tx;
                }
                break;
            }
            units++;
            charCount++;
            remaining -= 2;
            u16 code = *p++;
            switch (code) {
            case 0xff84:
            case 0xff85:
                if (maxX < tx) {
                    maxX = (u16) tx;
                }
                more = 0;
                break;
            case 0xff80:
                if (unk14 != 0) {
                    if (mUnk_58 < 0x10) {
                        mPalette[mUnk_58++] = charCount;
                    }
                }
                if (maxX < tx) {
                    maxX = (u16) tx;
                }
                more = 0;
                break;
            case 0xff81: {
                u16 index = *p++;
                units++;
                charCount++;
                remaining -= 2;
                PrintNumber((&mUnk_10)[index], 0, 0, buffer, width, height, &tx, &ty, unk9, color, unk11, unk12, unk15, 0,
                            unk17);
                break;
            }
            case 0xff82: {
                u16 index = *p++;
                units++;
                charCount++;
                remaining -= 2;
                Print(mStrings[index].mText, mStrings[index].mLength, buffer, width, height, &tx, &ty, NULL, unk9, color,
                      unk11, unk12, 0, 0, unk15, 1, unk17);
                break;
            }
            case 0xff83:
                p++;
                units++;
                charCount++;
                remaining -= 2;
                break;
            case 0xff86: {
                u8 mode = *p++;
                units++;
                charCount++;
                remaining -= 2;
                if (mode == 0) {
                    fontMode = savedFontMode;
                } else {
                    fontMode = (u8) (mode - 1);
                }
                break;
            }
            case 0xff87:
                lastAdvance = 6;
                tx += 6;
                break;
            default:
                if (fontMode != 0) {
                    lastAdvance = func_0202d19c(code);
                } else {
                    lastAdvance = mMetrics[code].mAdvance;
                }
                tx += lastAdvance;
                break;
            }
            if (tx > pixelWidth) {
                if (x != startX) {
                    x = startX;
                    y += unk12;
                    if (unk14 != 0) {
                        if (mUnk_58 < 0x10) {
                            mPalette[mUnk_58++] = charCount - units;
                        }
                    }
                    charCount = charCount - units;
                    remaining = remaining + units * 2;
                    units     = 0;
                } else if (units > 1) {
                    tx = tx - lastAdvance;
                    charCount--;
                    units--;
                    remaining += 2;
                    if (maxX < tx) {
                        maxX = (u16) tx;
                    }
                }
                more = 0;
            }
            if (charCount >= unk13) {
                drawFlag = unk17;
            }
        } while (more != 0);
        if (units != 0) {
            do {
                u16 code = *line++;
                units--;
                switch (code) {
                case 0xff84:
                    if ((x != startX) | (y == startY)) {
                        x += 6;
                    }
                    break;
                case 0xff85:
                    if ((x != startX) | (y == startY)) {
                        x += 12;
                    }
                    break;
                case 0xff80:
                    if (x != startX) {
                        x = startX;
                        y += unk12;
                    }
                    break;
                case 0xff81: {
                    u16 index = *line++;
                    units--;
                    PrintNumber((&mUnk_10)[index], 0, 0, buffer, width, height, &x, &y, unk9, color, unk11, unk12, unk15, 0,
                                unk17);
                    break;
                }
                case 0xff82: {
                    u16 index = *line++;
                    units--;
                    Print(mStrings[index].mText, mStrings[index].mLength, buffer, width, height, &x, &y, NULL, unk9, color,
                          unk11, unk12, 0, 0, unk15, 1, unk17);
                    break;
                }
                case 0xff83:
                    color = *line++;
                    units--;
                    if (color == 0) {
                        color = unk10;
                    }
                    break;
                case 0xff86: {
                    u16 mode = *line++;
                    units--;
                    if (mode == 0) {
                        mUnk_5b = savedFontMode;
                    } else {
                        mUnk_5b = mode - 1;
                    }
                    if (mUnk_5b != 0) {
                        fontHeight = 12;
                    } else {
                        fontHeight = mFont->mHeight;
                    }
                    break;
                }
                default: {
                    s16 yy = y;
                    if (yy + fontHeight <= pixelHeight && code != 0xff87) {
                        if (mUnk_5b != 0) {
                            if (drawFlag != 0) {
                                func_0202ceac(code, buffer, unk9, unk11, x, yy, pixelWidth, pixelHeight);
                            }
                        } else if (drawFlag != 0) {
                            func_0202d208(code, buffer, unk9, color, unk11, (s16) (x + mMetrics[code].mLeft), yy,
                                          pixelWidth, pixelHeight);
                        }
                    }
                    if (code == 0xff87) {
                        x += 6;
                    } else if (mUnk_5b != 0) {
                        x += func_0202d19c(code);
                    } else {
                        x += mMetrics[code].mAdvance;
                    }
                    break;
                }
                }
            } while (units != 0);
        }
    } while (remaining != 0);
    *outWidth  = x;
    *outHeight = y;
    if (outPixelWidth != NULL) {
        *outPixelWidth = maxX - startX;
    }
    if (unk16 & drawFlag) {
        u32 area = width * height;
        func_02005e9c(buffer, (unk9 == 0 ? 0x20 : 0x40) * area);
    }
    mUnk_5b = savedFontMode;
}

// non-matching: the digit count and the reversed number swap registers (r4/r5).
void TextManager::PrintNumber(u32 value, u32 minDigits, u32 unk3, void *buffer, u32 width, u32 height,
                              s16 *outWidth, s16 *outHeight, u32 unk9, u32 unk10, u32 unk11, u32 unk12, u32 unk13,
                              u32 unk14, u32 unk15) {
    u32 reversed = 0;
    u8 count     = 0;
    mUnk_59       = unk13;
    do {
        count++;
        reversed = reversed * 10 + value % 10;
        value    = value / 10;
    } while (value != 0);
    if (minDigits != 0 && minDigits > count) {
        u32 code = *(u16 *) ((u8 *) mData + *(u16 *) ((u8 *) mData + (u16) unk3 * 4 + 2) + 0x20);
        s32 advance;
        if (mUnk_5b != 0) {
            advance = func_0202d19c(code);
        } else {
            advance = mMetrics[code].mAdvance;
        }
        *outWidth = *outWidth + advance * (minDigits - count);
    }
    u32 table = (u16) unk3;
    do {
        u16 glyph = reversed % 10 + 0x10;
        glyph = ((u16 *) ((u8 *) mData + *(u16 *) ((u8 *) mData + table * 4 + 2)))[glyph];
        Print(&glyph, 2, buffer, width, height, outWidth, outHeight, NULL, unk9, unk10, unk11, unk12, 0, 0, unk13, unk14,
              unk15);
        reversed = reversed / 10;
        count--;
    } while (count != 0);
}

// non-matching: register allocation.
void TextManager::func_0202cd48(u32 mode, u16 *map, u32 palette, u32 base, u32 width, u32 height, u32 stride) {
    u16 start;
    u32 pitch = *(u16 *) &stride;
    if (mode == 0) {
        start = *(u16 *) &base >> 5;
    } else {
        start = *(u16 *) &base >> 6;
    }
    u16 y = 0;
    u16 n = 0;
    do {
        u16 x = 0;
        u16 *row = map + pitch * y;
        do {
            row[x] = (start + n++) | (palette << 12);
            x++;
        } while (x < width);
        y++;
    } while (y < height);
    func_02005e9c(map, pitch * height * 2);
}

void TextManager::SetColor(u32 unk1, u32 unk2, u32 unk3) {
    if (unk2 == 0) {
        func_02025c1c(unk1, 5, unk3 << 5, mCursor, 0x20, 0);
    } else {
        func_02025c1c(unk1, 0, unk3 << 9, mCursor, 0x20, 0);
    }
}

// non-matching: size 0x74 vs 0x68.
void TextManager::func_0202ce44(u32 unk1, u32 unk2, u32 unk3, u8 unk4) {
    if (unk2 == 0) {
        func_02025c1c(unk1, 6, unk3 << 5, mCursor, 0x20, 0);
    } else {
        func_02025c1c(unk1, 1, (unk3 << 9) + (unk4 << 13), mCursor, 0x20, 0);
    }
}

// non-matching: size 0x2ec vs 0x2f0.
void TextManager::func_0202ceac(u32 glyph, void *buffer, u32 eightBit, s32 layout, s16 x, s16 y, u16 width, u16 height) {
    s16 px  = *(s16 *) &x;
    u16 pw  = *(u16 *) &width;
    if ((px < 0) | (px >= pw)) {
        return;
    }
    s16 py = *(s16 *) &y;
    u16 ph = *(u16 *) &height;
    if ((py < 0) | (py >= ph)) {
        return;
    }
    s32 idx = func_0202d1bc(glyph);
    if (idx < 0) {
        return;
    }
    u32 glyphWidth = Widths()[idx];
    if (glyphWidth == 0) {
        return;
    }
    u8 *font    = data_020b48dc + idx * 0x80;
    u16 unitA   = (layout & 0xf) << 4;
    u16 unitB   = ((layout >> 4) & 0xf) << 4;
    u16 pw2 = *(u16 *) &width;
    u16 row     = 0;
    do {
        s32 absY = row + py;
        u8 *fontRow = font + (row % 8) * 4 + (row / 8) * 0x40;
        s32 tileRow = (pw2 >> 3) * (absY / 8);
        s32 yMod = absY % 8;
        u16 col = 0;
        do {
            u8 b = (fontRow + (col % 8) / 2)[(col / 8) * 0x20];
            u8 v;
            if ((col & 1) != 0) {
                v = (b & 0xf0) >> 4;
            } else {
                v = b & 0xf;
            }
            if (v != 0) {
                s32 absX = col + px;
                u32 pos;
                if (layout == 0) {
                    pos = (tileRow + absX / 8) * 0x40 + yMod * 8 + absX % 8;
                } else {
                    s32 part = unitA * (pw * (absY / unitA) + unitB * (absX / unitB));
                    pos = part + unitB * ((absY % unitA) / 8) * 8 + ((absX % unitB) / 8) * 0x40 + yMod * 8 + absX % 8;
                }
                u8 *buf = (u8 *) buffer;
                if (eightBit == 0) {
                    if ((pos & 1) != 0) {
                        buf[pos >> 1] = (buf[pos >> 1] & 0xf) | (v << 4);
                    } else {
                        buf[pos >> 1] = (buf[pos >> 1] & 0xf0) | v;
                    }
                } else {
                    buf[pos] = v;
                }
            }
            col++;
        } while (col < glyphWidth);
        row++;
    } while (row < 12);
}

u8 TextManager::func_0202d19c(u32 glyph) {
    s32 i = func_0202d1bc(glyph);
    if (i < 0) {
        return 0;
    }
    return Widths()[i];
}

s32 TextManager::func_0202d1bc(u32 code) {
    u16 *list = (u16 *) ((u8 *) mData + mData->mCodeOffset);
    u32 count = (u32) mData->mCodeSize >> 1;
    u16 i = 0;
    do {
        if (*list++ == code) {
            return (s16) i;
        }
        i++;
    } while (i < (u16) count);
    return -1;
}

// non-matching: size 0x404 vs 0x3fc.
void TextManager::func_0202d208(u32 glyph, void *buffer, u32 eightBit, u32 color, s32 layout, s16 x, s16 y, u32 width,
                                u32 height) {
    s32 px = *(s16 *) &x;
    if ((px < 0) | (px >= (s32) width)) {
        return;
    }
    s32 py = *(s16 *) &y;
    if ((py < 0) | (py >= (s32) height)) {
        return;
    }
    u8 *bits    = (u8 *) mFont + 8 + glyph * mFont->mGlyphSize;
    u16 col     = *(u16 *) &x;
    u16 row     = *(u16 *) &y;
    u16 unitA   = (layout & 0xf) << 4;
    u16 unitB   = ((layout >> 4) & 0xf) << 4;
    u32 tiles   = width >> 3;
    u8 mask     = 0;
    u32 bitsVal;
    u8 c8       = color;
    do {
        if (mask == 0) {
            bitsVal = *bits++;
            mask    = 0x80;
        }
        if (col < width && (u8) (bitsVal & mask) != 0) {
            if (mUnk_59 != 0) {
                s32 sy = row + 1;
                u32 pos;
                if (layout == 0) {
                    pos = (tiles * (sy / 8) + col / 8) * 0x40 + (sy % 8) * 8 + col % 8;
                } else {
                    pos = unitA * (width * (sy / unitA) + unitB * (col / unitB)) + unitB * ((sy % unitA) / 8) * 8 +
                          ((col % unitB) / 8) * 0x40 + (sy % 8) * 8 + col % 8;
                }
                u8 *buf = (u8 *) buffer;
                if (eightBit == 0) {
                    if ((pos & 1) != 0) {
                        buf[pos >> 1] = (buf[pos >> 1] & 0xf) | (mUnk_5a << 4);
                    } else {
                        buf[pos >> 1] = (buf[pos >> 1] & 0xf0) | mUnk_5a;
                    }
                } else {
                    buf[pos] = mUnk_5a;
                }
            }
            u32 pos;
            if (layout == 0) {
                pos = (tiles * (row / 8) + col / 8) * 0x40 + (row % 8) * 8 + col % 8;
            } else {
                pos = unitA * (width * (row / unitA) + unitB * (col / unitB)) + unitB * ((row % unitA) / 8) * 8 +
                      ((col % unitB) / 8) * 0x40 + (row % 8) * 8 + col % 8;
            }
            u8 *buf = (u8 *) buffer;
            if (eightBit == 0) {
                if ((pos & 1) != 0) {
                    buf[pos >> 1] = (buf[pos >> 1] & 0xf) | (color << 4);
                } else {
                    buf[pos >> 1] = (buf[pos >> 1] & 0xf0) | color;
                }
            } else {
                buf[pos] = c8;
            }
        }
        col++;
        mask >>= 1;
        if (col - px >= mFont->mWidth) {
            row++;
            col = *(u16 *) &x;
        }
        if (row >= height) {
            return;
        }
    } while (row - py < mFont->mHeight);
}

// non-matching: same size; the prologue computes the screen offset (index * 0x408) later than the original.
void TextManager::Init(void *glyphs, u32 unk2, u32 unk3, u32 width, u32 height, u32 unk6) {
 TextSprite *sprite = (TextSprite *) glyphs; s32 flags = unk6; s32 unitY = (flags >> 4) & 0xf; s32 unitX = flags & 0xf;
 u16 tableIndex = (unitX - 1) + ((unitY - 1) << 2); u16 tile = 0; u16 stepX = unitX << 1; u16 stepY = unitY << 1; u16 y = 0;
 do { s16 py = y << 3; u32 blocks = stepY * stepX; u16 x = 0;
 do {
 sprite->mAttr = data_020a56f0[tableIndex] | (unk3 << 13);
 sprite->mTile = SpriteManager::Get().Screen(unk2).func_020276f4(tile);
 sprite->mX = x << 3; sprite->mY = py;
 if (unk3 == 0) { tile += blocks << 5; } else { tile += blocks << 6; }
 sprite++; x += stepY; } while (x < width);
 y += stepX; } while (y < height);
}
