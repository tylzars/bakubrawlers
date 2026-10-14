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
    /* 08 */ s16 mY;
    /* 0a */ PAD(0x0a, 0x0c);
    /* 0c */
};

extern "C" void DC_FlushRange(void *buffer, u32 size);

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
    idx = func_0202d1bc(*(u16 *) ((u8 *) mData + mData->mTables[0].mOffset + 0x20));
    g = 1;
    do {
        Widths()[idx + g] = Widths()[idx];
        g++;
    } while (g < 10);
}

u16 TextManager::func_0202c1a8(u32 index) {
    if (index < 0x10) {
        return mPalette[index];
    }
    return 0;
}

// non-matching: 47 words, entry block only. The original copies width, height, unk12, unk13 and unk10 to the stack
// before it stores maxX = 0 and tests unk14; ours copies unk12, unk13 and unk10 in the later group. The original
// sets unk17 = 0 by reloading maxX's slot (`ldrne r0, [sp, #92]`), so maxX = 0 comes first there, but every order
// tried that puts it before the unk14 test shares the zero in a register and is 3 words shorter. `volatile` on x and
// y is a placeholder: it is the only form found that keeps the `x = startX` store ahead of the `y` load.
void TextManager::Print(const void *text, u16 length, void *buffer, u32 width, u32 height, s16 *outWidth,
                        s16 *outHeight, u16 *outPixelWidth, u32 unk9, u32 unk10, u32 unk11, u32 unk12, u32 unk13,
                        u32 unk14, u32 unk15, u32 unk16, u32 unk17) {
    u32 color;
    u16 remaining;
    s32 fontHeight;
    u16 charCount;
    u8 savedFontMode;
    s32 startY;
    s32 maxX;
    u32 drawFlag;
    u16 pixelHeight;
    s16 startX;
    volatile s16 x;
    const u16 *line = (const u16 *) text;
    u16 pixelWidth;
    volatile s16 y;
    pixelWidth  = width * 8;
    pixelHeight = height * 8;
    if (unk14 != 0) {
        unk17 = 0;
    }
    color = unk10;
    maxX = 0;
    mUnk_59 = unk15;
    savedFontMode = mUnk_5b;
    startX = *outWidth;
    startY = *outHeight;
    charCount = 0;
    drawFlag = 0;
    x = startX;
    y = startY;
    if (mUnk_5b != 0) {
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
        fontMode           = mUnk_5b;
        more               = 1;
        do {
            if (remaining == 0) {
                if (maxX < tx) {
                    maxX = (u16) tx;
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
                    PrintNumber((&mUnk_10)[index], 0, 0, buffer, width, height, (s16 *) &x, (s16 *) &y, unk9, color, unk11, unk12, unk15, 0,
                                unk17);
                    break;
                }
                case 0xff82: {
                    u16 index = *line++;
                    units--;
                    Print(mStrings[index].mText, mStrings[index].mLength, buffer, width, height, (s16 *) &x, (s16 *) &y, NULL, unk9, color,
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
        DC_FlushRange(buffer, width * height * (unk9 == 0 ? 0x20 : 0x40));
    }
    mUnk_5b = savedFontMode;
}

// non-matching: 15 words, prologue scheduling only. The original copies unk12 to its home slot through r5 and
// loads unk13 (r6) late; here unk13 is loaded first and every copy goes through r0.
void TextManager::PrintNumber(u32 value, u8 minDigits, u32 unk3, void *buffer, u32 width, u32 height,
                              s16 *outWidth, s16 *outHeight, u32 unk9, u32 unk10, u32 unk11, u32 unk12, u32 unk13,
                              u32 unk14, u32 unk15) {
    u32 count    = 0;
    u32 reversed = 0;
    mUnk_59       = unk13;
    do {
        count = (u8) (count + 1);
        reversed *= 10;
        reversed += value % 10;
    } while ((value /= 10) != 0);
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
        u8 *codes = (u8 *) mData;
        codes += *(u16 *) ((u8 *) mData + table * 4 + 2);
        glyph = ((u16 *) codes)[glyph];
        Print(&glyph, 2, buffer, width, height, outWidth, outHeight, NULL, unk9, unk10, unk11, unk12, 0, 0, unk13, unk14,
              unk15);
        reversed = reversed / 10;
        count = (u8) (count - 1);
    } while (count != 0);
}

void TextManager::func_0202cd48(u32 mode, u16 *map, u32 palette, u32 base, u32 width, u32 height, u32 stride) {
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
        func_02025c1c(unk1, 5, unk3 << 5, mCursor, 0x20, 0);
    } else {
        func_02025c1c(unk1, 0, unk3 << 9, mCursor, 0x20, 0);
    }
}

void TextManager::func_0202ce44(u32 unk1, u32 unk2, u32 unk3, u8 unk4) {
    if (unk2 == 0) {
        func_02025c1c(unk1, 6, unk3 << 5, mCursor, 0x20, 0);
    } else {
        func_02025c1c(unk1, 1, (unk4 << 13) + (unk3 << 9), mCursor, 0x20, 0);
    }
}

void TextManager::func_0202ceac(u32 glyph, void *buffer, u32 eightBit, s32 layout, s16 x, s16 y, u16 width, u16 height) {
    if ((x < 0) | (x >= width)) {
        return;
    }
    if ((y < 0) | (y >= height)) {
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

void TextManager::func_0202d208(u32 glyph, void *buffer, u32 eightBit, u32 color, s32 layout, s16 x, s16 y, u16 width,
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
            if (mUnk_59 != 0) {
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

// non-matching: 27 words, entry block only: the original puts the screen offset (unk2 * 0x408) in r4 and unk6 in r0,
// unitY/unitX in r8/r7, and copies width/height through r2; here they land in r0/r4/r7/r5, which reorders the block.
void TextManager::Init(void *glyphs, u32 unk2, u32 unk3, u32 width, u32 height, u32 unk6) {
    TextSprite *sprite = (TextSprite *) glyphs;
    s32 flags          = unk6;
    u16 unitY          = (flags >> 4) & 0xf;
    u16 unitX          = flags & 0xf;
    u16 tableIndex     = (unitX - 1) + ((unitY - 1) << 2);
    u16 tile           = 0;
    u16 stepX          = unitX << 1;
    u16 stepY          = unitY << 1;
    u16 y              = 0;
    do {
        u16 x = 0;
        do {
            sprite->mAttr = data_020a56f0[tableIndex] | (unk3 << 13);
            sprite->mTile = SpriteManager::Get().Screen(unk2).func_020276f4(tile);
            sprite->mX    = x << 3;
            sprite->mY    = y << 3;
            if (unk3 == 0) {
                tile += (stepY * stepX) << 5;
            } else {
                tile += (stepY * stepX) << 6;
            }
            sprite++;
            x += stepY;
        } while (x < width);
        y += stepX;
    } while (y < height);
}
