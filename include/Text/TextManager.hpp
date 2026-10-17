#pragma once

#include "global.h"
#include "types.h"

// One message of a .mes file: where its text starts and how long it is.
struct MessageEntry {
    // Read as volatile: the original always loads the offset where the text pointer is formed, never later
    // (it is not moved past the following stores). Plain u16 lets the load sink to the call.
    /* 00 */ volatile u16 mOffset;
    /* 02 */ u16 mLength;
    /* 04 */
};

// A loaded .mes message file.
struct MessageFile {
    /* 00 */ PAD(0x00, 0x02);
    /* 02 */ MessageEntry mEntries[1];
    /* 06 */

    const u8 *GetText(int index) {
        return (const u8 *) this + mEntries[index].mOffset;
    }
    u16 GetLength(int index) {
        return mEntries[index].mLength;
    }
};

// Header of the 1bpp glyph bitmaps; the bitmaps follow at +8, mGlyphSize bytes each.
struct FontHeader {
    /* 00 */ u8 mWidth;
    /* 01 */ u8 mHeight; // line height of the font
    /* 02 */ u16 mGlyphSize;
    /* 04 */ PAD(0x04, 0x08);
    /* 08 */
};

// Per-glyph metrics, 3 bytes each.
struct GlyphMetrics {
    /* 00 */ s8 mLeft;
    /* 01 */ s8 mUnk_01;
    /* 02 */ s8 mAdvance;
    /* 03 */
};

// An entry of the glyph tables at the start of the font data: mOffset is the position of a u16 glyph list.
struct GlyphTable {
    /* 00 */ u16 mUnk_00;
    /* 02 */ u16 mOffset;
    /* 04 */
};

// Font data: glyph tables, then at +0x22 the offset of the sorted glyph code list whose byte size is at +0x24.
struct FontData {
    /* 00 */ GlyphTable mTables[1];
    /* 04 */ PAD(0x04, 0x20);
    /* 20 */ u16 mUnk_20;
    /* 22 */ u16 mCodeOffset;
    /* 24 */ u16 mCodeSize;
    /* 26 */
};

// A text string registered with the manager: a control code in message text prints it in place.
struct TextString {
    /* 00 */ u32 mLength;
    /* 04 */ const void *mText;
    /* 08 */
};

// Global text renderer, constructed on first use and destroyed at exit.
class TextManager {
public:
    /* 00 */ FontHeader *mFont;
    /* 04 */ GlyphMetrics *mMetrics;
    /* 08 */ FontData *mData;
    /* 0c */ u16 *mCursor;
    /* 10 */ u16 mUnk_10; // numbers for control code 0xff81: mUnk_10 and the three u16 after it
    /* 12 */ u16 mUnk_12[3];
    /* 18 */ TextString mStrings[3]; // texts for control code 0xff82
    /* 30 */ PAD(0x30, 0x36);
    union {
        /* 36 */ u16 mUnk_36[17]; // filled as one run from 0x36 (ov002 02220830)
        struct {
            /* 36 */ u16 mUnk_36_0;
            /* 38 */ u16 mLineStarts[16]; // text position where each printed line starts
        };
    };
    /* 58 */ u8 mLineStartCount; // entries recorded in mLineStarts
    /* 59 */ u8 mUseShadow; // draw a shadow behind each glyph
    /* 5a */ u8 mShadowColor; // shadow colour index
    /* 5b */ u8 mUse4bppFont; // 1 = use the 4bpp bitmap font
    /* 5c */ // the instance really extends to 0xe8: u8 widths of the bitmap font glyphs from +0x5c (see Widths());
    //        declaring them as a member changes the code of the users, so they are reached by offset

    /* 5c */ u8 mTail[0x8c]; // glyph widths of the bitmap font, reached through Widths(); declared so sizeof is the real 0xe8

    u8 *Widths() {
        return (u8 *) this + 0x5c;
    }

#ifdef GAME_SINGLETON_CTORS
    // As Game.cpp saw the class: an empty constructor, so its copies of the singleton objects join guard first.
    TextManager() {}
#endif
    ~TextManager();

    static TextManager &Get() {
        static TextManager instance;
        return instance;
    }

    u32 BufferSize(u16 width, u16 height) {
        return width * height * 0x20;
    }
    u32 GlyphCount(u32 count) {
        return count;
    }
    // Glyph slots needed for a width x height tile area, from Init's flags argument (tile unit sizes in its nibbles).
    u16 GlyphCount(s32 width, s32 height, s32 flags) {
        u16 unitX  = (flags & 0xf) << 4;
        u16 unitY  = ((flags >> 4) & 0xf) << 4;
        s32 countX = (unitY + width * 8 - 1) / unitY;
        s32 countY = (unitX + height * 8 - 1) / unitX;
        return countX * countY;
    }

    void InitFont(); // sets up the font pointers and measures the bitmap font
    u16 GetLineStart(u32 index); // text position where a recorded line starts
    void Init(void *glyphs, u32 unk2, u32 unk3, u16 width, u16 height, s32 flags);
    void SetColor(u32 unk1, u32 unk2, u32 unk3);
    void Print(const void *text, u16 length, void *buffer, u16 width, u16 height, s16 *outWidth, s16 *outHeight,
               u16 *outPixelWidth, u32 unk9, u32 unk10, u32 unk11, u16 unk12, u16 unk13, u32 unk14, u32 unk15,
               u32 unk16, u32 unk17);
    void PrintNumber(u32 value, u8 minDigits, u32 unk3, void *buffer, u16 width, u16 height, s16 *outWidth,
                     s16 *outHeight, u32 unk9, u32 unk10, u32 unk11, u16 unk12, u32 unk13, u32 unk14,
                     u32 unk15); // 0202cb50
    void FillScrChars(u32 mode, u16 *map, u32 palette, u32 base, u32 width, u32 height, u32 stride);
    void func_0202ce44(u32 unk1, u32 unk2, u32 unk3, u8 unk4);
    void Write4bppGlyph(u32 glyph, void *buffer, u32 eightBit, s32 layout, s16 x, s16 y, u16 width, u16 height);
    u8 Get4bppGlyphWidth(u32 glyph);
    s32 FindGlyphIndex(u32 code);
    void Write1bppGlyph(u32 glyph, void *buffer, u32 eightBit, u32 color, s32 layout, s16 x, s16 y, u16 width, u16 height);
};
