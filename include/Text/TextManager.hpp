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

// Global text renderer, constructed on first use and destroyed at exit.
class TextManager {
public:
    /* 00 */ PAD(0x00, 0x10);
    /* 10 */ u16 mUnk_10;
    /* 12 */ PAD(0x12, 0x36);
    union {
        /* 36 */ u16 mUnk_36[17]; // filled as one run from 0x36 (ov002 02220830)
        struct {
            /* 36 */ u16 mUnk_36_0;
            /* 38 */ u16 mPalette[16];
        };
    };
    /* 58 */ u8 mUnk_58;
    /* 59 */ PAD(0x59, 0x5b);
    /* 5b */ u8 mUnk_5b;
    /* 5c */

    ~TextManager();

    static TextManager &Get() {
        static TextManager instance;
        return instance;
    }

    u32 BufferSize(u32 width, u32 height) {
        return width * height * 0x20;
    }
    u32 GlyphCount(u32 count) {
        return count;
    }
    // Glyph slots needed for a width x height tile area, from Init's flags argument (tile unit sizes in its nibbles).
    u16 GlyphCount(u32 width, u32 height, s32 flags) {
        u16 unitX  = (flags & 0xf) << 4;
        u16 unitY  = ((flags >> 4) & 0xf) << 4;
        s32 countX = (unitY + width * 8 - 1) / unitY;
        s32 countY = (unitX + height * 8 - 1) / unitX;
        return countX * countY;
    }

    void Init(void *glyphs, u32 unk2, u32 unk3, u32 width, u32 height, u32 unk6);
    void SetColor(u32 unk1, u32 unk2, u32 unk3);
    void Print(const void *text, u16 length, void *buffer, u32 width, u32 height, s16 *outWidth, s16 *outHeight,
               u16 *outPixelWidth, u32 unk9, u32 unk10, u32 unk11, u32 unk12, u32 unk13, u32 unk14, u32 unk15,
               u32 unk16, u32 unk17);
    void PrintNumber(u32 value, u32 unk2, u32 unk3, void *buffer, u32 width, u32 height, s16 *outWidth,
                     s16 *outHeight, u32 unk9, u32 unk10, u32 unk11, u32 unk12, u32 unk13, u32 unk14,
                     u32 unk15); // 0202cb50
};
