#pragma once

#include "global.h"
#include "types.h"

// A loaded .mes message file.
struct MessageFile {
    /* 00 */ PAD(0x00, 0x2a);
    /* 2a */ u16 mTextOffset;
    /* 2c */ u16 mTextLength;
};

// Global text renderer, constructed on first use and destroyed at exit.
class TextManager {
public:
    ~TextManager();

    static TextManager &Get() {
        static TextManager instance;
        return instance;
    }

    u32 BufferSize() {
        return 0x1800;
    }
    u32 GlyphCount() {
        return 0x30;
    }

    void Init(void *glyphs, u32 unk2, u32 unk3, u32 width, u32 height, u32 unk6);
    void SetColor(u32 unk1, u32 unk2, u32 unk3);
    void Print(const void *text, u16 length, void *buffer, u32 width, u32 height, s16 *outWidth, s16 *outHeight,
               u32 unk8, u32 unk9, u32 unk10, u32 unk11, u32 unk12, u32 unk13, u32 unk14, u32 unk15, u32 unk16,
               u32 unk17);
};
