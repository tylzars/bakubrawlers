#pragma once

#include "global.h"
#include "types.h"

struct UnkStruct_SpriteManager_00 {
    /* 00 */ unk32 mUnk_00;
    /* 04 */

    UnkStruct_SpriteManager_00();
};

// One screen's OBJ (sprite) state: character and palette uploads.
struct SpriteScreen {
    /* 000 */ PAD(0x000, 0x402);
    /* 402 */ u8 mBgIndex;
    /* 403 */ PAD(0x403, 0x408);
    /* 408 */

    SpriteScreen();
    void LoadCharacters(const char *path, u32 offset, u32 unk3);
    void LoadPalette(const char *path, u32 offset, u32 unk3);
    u16 func_020276f4(u32 unk1);
    void func_02027624(u32 unk1);
    void func_02027778(void *buffer, u8 count, u32 unk3, u32 unk4, u32 unk5, u32 unk6, u32 unk7, u32 unk8);
};

struct FadeState {
    /* 00 */ s8 mMain;
    /* 01 */ s8 mSub;
};

// Global sprite/display manager, constructed on first use.
class SpriteManager {
public:
    /* 000 */ UnkStruct_SpriteManager_00 mUnk_000;
    /* 004 */ SpriteScreen mScreens[2];
    /* 814 */

    static FadeState sFade;

    static SpriteManager &Get() {
        static SpriteManager instance;
        return instance;
    }

    SpriteScreen &Screen(int index) {
        return mScreens[index];
    }

    void func_02026dec(u32 unk1, u32 unk2);

    void StartFade(s8 mode) {
        sFade.mMain = mode;
        sFade.mSub  = mode;
    }

    BOOL IsFading() {
        return (sFade.mMain > 1) | (sFade.mSub > 1);
    }
};
