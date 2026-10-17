#include "Text/TextManager.hpp"
#include "Graphics/SpriteManager.hpp"
#include "System/FileSystem.hpp"
#include "Unknown/UnkFunctions.hpp"
#include <nitro/mi.h>

extern FontHeader gFontHeader;
extern GlyphMetrics gGlyphMetrics;
extern FontData gFontData;
extern u16 gTextPalette;
extern u8 gFont4bppGlyphs[];
extern "C" void OS_Terminate();
extern u32 gTextSpriteAttrs[];

// One glyph block of a text buffer registered as an OBJ: attribute word, tile number, position.
struct TextSprite {
    /* 00 */ u32 mAttr;
    /* 04 */ u16 mTile;
    /* 06 */ u16 mX;
    /* 08 */ s16 mY;
    /* 0a */ PAD(0x0a, 0x0c);
    /* 0c */
};

extern "C" void DC_FlushRange(void *buffer, u32 size);

void TextManager::InitFont() {
    mPalette = &gTextPalette;
    *mPalette = 0;
    mFont    = &gFontHeader;
    mMetrics = &gGlyphMetrics;
    mData    = &gFontData;
    mUseShadow  = 0;
    mShadowColor  = 6;
    mUse4bppFont  = 0;
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
            u8 *row = gFont4bppGlyphs + (y % 8) * 4 + (y / 8) * 0x40 + g * 0x80;
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
    idx = FindGlyphIndex(*(u16 *) ((u8 *) mData + mData->mTables[0].mOffset + 0x20));
    g = 1;
    do {
        Widths()[idx + g] = Widths()[idx];
        g++;
    } while (g < 10);
}

u16 TextManager::GetLineStart(u32 index) {
    if (index < 0x10) {
        return mLineStarts[index];
    }
    return 0;
}

void TextManager::Print(const void *text, u16 length, void *buffer, u16 width, u16 height, s16 *outWidth,
                        s16 *outHeight, u16 *outPixelWidth, u32 unk9, u32 unk10, u32 unk11, u16 unk12, u16 unk13,
                        u32 unk14, u32 unk15, u32 unk16, u32 unk17) {
    u32 color;
    u16 remaining;
    s32 fontHeight;
    u16 charCount;
    u8 savedFontMode;
    s16 startY;
    u16 maxX;
    u32 drawFlag;
    u16 pixelHeight;
    s16 startX;
    s16 x;
    const u16 *line = (const u16 *) text;
    u16 pixelWidth;
    s16 y;
    pixelWidth  = width * 8;
    pixelHeight = height * 8;
    color = unk10;
    maxX = 0;
    mUseShadow = unk15;
    savedFontMode = mUse4bppFont;
    startX = *outWidth;
    startY = *outHeight;
    charCount = 0;
    drawFlag = 0;
    x = startX;
    y = startY;
    if (unk14 != 0) {
        unk17 = 0;
    }
    if (mUse4bppFont != 0) {
        fontHeight = 12;
    } else {
        fontHeight = mFont->mHeight;
    }
    remaining = length;
    do {
        u32 more;
        u8 fontMode;
        s32 lastAdvance;
        s16 tx             = x;
        s16 ty             = y;
        const u16 *p       = line;
        u16 units          = 0;
        fontMode           = mUse4bppFont;
        more               = 1;
        do {
            if (remaining == 0) {
                if (maxX < tx) {
                    maxX = tx;
                }
                break;
            }
            u16 code = *p++;
            units++;
            charCount++;
            remaining -= 2;
            switch (code) {
            case 0xff84:
            case 0xff85:
                if (maxX < tx) {
                    maxX = tx;
                }
                more = 0;
                break;
            case 0xff80:
                if (unk14 != 0) {
                    if (mLineStartCount < 0x10) {
                        mLineStarts[mLineStartCount++] = charCount;
                    }
                }
                if (maxX < tx) {
                    maxX = tx;
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
                    lastAdvance = Get4bppGlyphWidth(code);
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
                        if (mLineStartCount < 0x10) {
                            mLineStarts[mLineStartCount++] = charCount - units;
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
                        maxX = tx;
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
                        mUse4bppFont = savedFontMode;
                    } else {
                        mUse4bppFont = mode - 1;
                    }
                    if (mUse4bppFont != 0) {
                        fontHeight = 12;
                    } else {
                        fontHeight = mFont->mHeight;
                    }
                    break;
                }
                default: {
                    s16 yy = y;
                    if (yy + fontHeight <= pixelHeight && code != 0xff87) {
                        if (mUse4bppFont != 0) {
                            if (drawFlag != 0) {
                                Write4bppGlyph(code, buffer, unk9, unk11, x, yy, pixelWidth, pixelHeight);
                            }
                        } else if (drawFlag != 0) {
                            Write1bppGlyph(code, buffer, unk9, color, unk11, (s16) (x + mMetrics[code].mLeft), yy,
                                          pixelWidth, pixelHeight);
                        }
                    }
                    if (code == 0xff87) {
                        x += 6;
                    } else if (mUse4bppFont != 0) {
                        x += Get4bppGlyphWidth(code);
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
        DC_FlushRange(buffer, width * height * (unk9 == 0 ? 0x20 : 0x40));
    }
    mUse4bppFont = savedFontMode;
}

void TextManager::PrintNumber(u32 value, u8 minDigits, u32 unk3, void *buffer, u16 width, u16 height,
                              s16 *outWidth, s16 *outHeight, u32 unk9, u32 unk10, u32 unk11, u16 unk12, u32 unk13,
                              u32 unk14, u32 unk15) {
    u32 count    = 0;
    u32 reversed = 0;
    mUseShadow       = unk13;
    do {
        count = (u8) (count + 1);
        reversed *= 10;
        reversed += value % 10;
    } while ((value /= 10) != 0);
    if (minDigits != 0 && minDigits > count) {
        u32 code = *(u16 *) ((u8 *) mData + *(u16 *) ((u8 *) mData + (u16) unk3 * 4 + 2) + 0x20);
        s32 advance;
        if (mUse4bppFont != 0) {
            advance = Get4bppGlyphWidth(code);
        } else {
            advance = mMetrics[code].mAdvance;
        }
        *outWidth = *outWidth + advance * (minDigits - count);
    }
    u32 table = (u16) unk3;
    do {
        u16 glyph = reversed % 10 + 0x10;
        u8 *codes = (u8 *) mData;
        codes += *(u16 *) ((u8 *) mData + table * 4 + 2);
        glyph = ((u16 *) codes)[glyph];
        Print(&glyph, 2, buffer, width, height, outWidth, outHeight, NULL, unk9, unk10, unk11, unk12, 0, 0, unk13, unk14,
              unk15);
        reversed = reversed / 10;
        count = (u8) (count - 1);
    } while (count != 0);
}

void TextManager::FillScrChars(u32 mode, u16 *map, u32 palette, u32 base, u32 width, u32 height, u32 stride) {
    u16 start;
    u16 pitch;
    u16 n;
    u16 x;
    u16 y;
    pitch = *(u16 *) &stride;
    if (mode == 0) {
        start = *(u16 *) &base >> 5;
    } else {
        start = *(u16 *) &base >> 6;
    }
    y = 0;
    n = 0;
    do {
        x = 0;
        do {
            map[y * pitch + x] = (start + n) | (palette << 12);
            n++;
            x++;
        } while (x < width);
        y++;
    } while (y < height);
    DC_FlushRange(map, pitch * height * 2);
}

void TextManager::SetColor(u32 unk1, u32 unk2, u32 unk3) {
    if (unk2 == 0) {
        QueueUpload(unk1, UPLOAD_KIND_OBJ_PLTT, unk3 << 5, mPalette, 0x20, 0);
    } else {
        QueueUpload(unk1, UPLOAD_KIND_OBJ_EXT_PLTT, unk3 << 9, mPalette, 0x20, 0);
    }
}

void TextManager::UploadBgPalette(u32 unk1, u32 unk2, u32 unk3, u8 unk4) {
    if (unk2 == 0) {
        QueueUpload(unk1, UPLOAD_KIND_BG_PLTT, unk3 << 5, mPalette, 0x20, 0);
    } else {
        QueueUpload(unk1, UPLOAD_KIND_BG_EXT_PLTT, (unk4 << 13) + (unk3 << 9), mPalette, 0x20, 0);
    }
}

void TextManager::Write4bppGlyph(u32 glyph, void *buffer, u32 eightBit, s32 layout, s16 x, s16 y, u16 width, u16 height) {
    if ((x < 0) | (x >= width)) {
        return;
    }
    if ((y < 0) | (y >= height)) {
        return;
    }
    s32 idx = FindGlyphIndex(glyph);
    if (idx < 0) {
        return;
    }
    u32 glyphWidth = Widths()[idx];
    if (glyphWidth == 0) {
        return;
    }
    u8 *font    = gFont4bppGlyphs + idx * 0x80;
    u16 unitA   = (layout & 0xf) << 4;
    u16 unitB   = ((layout >> 4) & 0xf) << 4;
    u16 row     = 0;
    do {
        u16 col = 0;
        do {
            u8 b = (font + (row % 8) * 4 + (row / 8) * 0x40 + (col % 8) / 2)[(col / 8) * 0x20];
            u8 v;
            if ((col & 1) != 0) {
                v = (b & 0xf0) >> 4;
            } else {
                v = b & 0xf;
            }
            if (v != 0) {
                u32 pos;
                if (layout == 0) {
                    pos = ((width / 8) * ((row + y) / 8) + (col + x) / 8) * 0x40 + ((row + y) % 8) * 8 + (col + x) % 8;
                } else {
                    s32 qy = row + y;
                    s32 qx = col + x;
                    qy /= unitA;
                    qx /= unitB;
                    s32 ry = row + y;
                    ry %= unitA;
                    s32 rx = col + x;
                    rx %= unitB;
                    pos = unitA * (width * qy + unitB * qx);
                    pos += unitB * (ry / 8) * 8;
                    pos += (rx / 8) * 0x40;
                    pos += ((row + y) % 8) * 8;
                    pos += (col + x) % 8;
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

u8 TextManager::Get4bppGlyphWidth(u32 glyph) {
    s32 i = FindGlyphIndex(glyph);
    if (i < 0) {
        return 0;
    }
    return Widths()[i];
}

s32 TextManager::FindGlyphIndex(u32 code) {
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

void TextManager::Write1bppGlyph(u32 glyph, void *buffer, u32 eightBit, u32 color, s32 layout, s16 x, s16 y, u16 width,
                                u16 height) {
    if ((x < 0) | (x >= (s32) width)) {
        return;
    }
    if ((y < 0) | (y >= (s32) height)) {
        return;
    }
    u8 *bits  = (u8 *) mFont + 8 + glyph * mFont->mGlyphSize;
    u16 col   = x;
    u16 row   = y;
    u16 unitA = (layout & 0xf) << 4;
    u16 unitB = ((layout >> 4) & 0xf) << 4;
    u8 mask   = 0;
    u32 bitsVal;
    do {
        if (mask == 0) {
            bitsVal = *bits++;
            mask    = 0x80;
        }
        if (col < width && (u8) (bitsVal & mask) != 0) {
            if (mUseShadow != 0) {
                u32 pos;
                if (layout == 0) {
                    pos = (((row + 1) / 8) * (width / 8) + col / 8) * 0x40 + ((row + 1) % 8) * 8 + col % 8;
                } else {
                    s32 qy = row + 1;
                    s32 qx = col;
                    qy /= unitA;
                    qx /= unitB;
                    s32 ry = row + 1;
                    ry %= unitA;
                    s32 rx = col;
                    rx %= unitB;
                    pos = unitA * (width * qy + unitB * qx);
                    pos += unitB * (ry / 8) * 8;
                    pos += (rx / 8) * 0x40;
                    pos += ((row + 1) % 8) * 8;
                    pos += col % 8;
                }
                u8 *buf = (u8 *) buffer;
                if (eightBit == 0) {
                    if ((pos & 1) != 0) {
                        buf[pos >> 1] = (buf[pos >> 1] & 0xf) | (mShadowColor << 4);
                    } else {
                        buf[pos >> 1] = (buf[pos >> 1] & 0xf0) | mShadowColor;
                    }
                } else {
                    buf[pos] = mShadowColor;
                }
            }
            u32 pos;
            if (layout == 0) {
                pos = ((row / 8) * (width / 8) + col / 8) * 0x40 + (row % 8) * 8 + col % 8;
            } else {
                s32 qy = row;
                s32 qx = col;
                qy /= unitA;
                qx /= unitB;
                s32 ry = row;
                ry %= unitA;
                s32 rx = col;
                rx %= unitB;
                pos = unitA * (width * qy + unitB * qx);
                pos += unitB * (ry / 8) * 8;
                pos += (rx / 8) * 0x40;
                pos += (row % 8) * 8;
                pos += col % 8;
            }
            u8 *buf = (u8 *) buffer;
            if (eightBit == 0) {
                if ((pos & 1) != 0) {
                    buf[pos >> 1] = (buf[pos >> 1] & 0xf) | (color << 4);
                } else {
                    buf[pos >> 1] = (buf[pos >> 1] & 0xf0) | color;
                }
            } else {
                buf[pos] = color;
            }
        }
        col++;
        mask >>= 1;
        if (col - x >= mFont->mWidth) {
            row++;
            col = x;
        }
        if (row >= height) {
            return;
        }
    } while (row - y < mFont->mHeight);
}

void TextManager::Init(void *glyphs, u32 unk2, u32 unk3, u16 width, u16 height, s32 flags) {
    TextSprite *sprite = (TextSprite *) glyphs;
    u16 unitY          = (flags >> 4) & 0xf;
    u16 unitX          = flags & 0xf;
    u16 tableIndex     = (unitX - 1) + ((unitY - 1) << 2);
    unitY <<= 1;
    unitX <<= 1;
    u16 tile = 0;
    u16 y    = 0;
    do {
        u16 x = 0;
        do {
            sprite->mAttr = gTextSpriteAttrs[tableIndex] | (unk3 << 13);
            sprite->mTile = SpriteManager::Get().Screen(unk2).GetCharBase(tile);
            sprite->mX    = x << 3;
            sprite->mY    = y << 3;
            if (unk3 == 0) {
                tile += (unitY * unitX) << 5;
            } else {
                tile += (unitY * unitX) << 6;
            }
            sprite++;
            x += unitY;
        } while (x < width);
        y += unitX;
    } while (y < height);
}
