#pragma once

#include "global.h"
#include "types.h"

// One OAM entry as the 3D-less sprite code writes it: attributes 0 and 1 are stored as one word.
struct SpriteOamEntry {
    /* 00 */ u32 mAttr01;
    /* 04 */ u16 mAttr2;
    /* 06 */ u16 mUnk_06;
    /* 08 */
};

struct UnkStruct_SpriteManager_00 {
    /* 00 */ unk32 mUnk_00;
    /* 04 */

    UnkStruct_SpriteManager_00();
};

// One screen's OBJ (sprite) state: character and palette uploads.
struct SpriteScreen {
    /* 000 */ SpriteOamEntry mOam[128];
    /* 400 */ u16 mUnk_400; // OBJ tile unit (bytes), from the mode table
    /* 402 */ u8 mBgIndex;
    /* 403 */ u8 mUnk_403; // OBJ mapping mode
    /* 404 */ u8 mUnk_404; // OAM entries used
    /* 405 */ u8 mUnk_405; // matrices used
    /* 406 */ PAD(0x406, 0x408);
    /* 408 */

    SpriteScreen();
    void func_020275e8(u32 unk1); // set mBgIndex
    void func_020275f0();         // upload the OAM copy and reset it
    void func_02027750();         // reset the OAM copy
    void LoadCharacters(const char *path, u32 offset, u32 unk3);
    void LoadPalette(const char *path, u32 offset, u32 unk3);
    void func_02027fec(const char *path, u32 unk2, u32 unk3); // palette load variant
    void *func_02028054(u8 *data); // reorders 0x1000 bytes of character data into a new buffer and frees data
    u16 func_020276f4(u32 unk1);
    void func_02027624(s32 unk1);
    void func_02027778(void *buffer, u8 count, s16 unk3, s16 unk4, u32 unk5, u32 unk6, u32 unk7, u32 unk8);
};

// One of the eight slots at SpriteManager+0x828 (copied by func_02027578, set up by func_020360cc).
struct UnkStruct_SpriteManager_28 {
    /* 00 */ u32 mUnk_00[10];
    /* 28 */
};

// Global sprite/display manager, constructed on first use.
class SpriteManager {
public:
    /* 000 */ UnkStruct_SpriteManager_00 mUnk_000;
    /* 004 */ SpriteScreen mScreens[2];
    /* 814 */ PAD(0x814, 0x824); // initialised by func_020313f8
    /* 824 */ u32 mMode;         // VRAM/display layout 1..9 chosen by func_02026dec
    /* 828 */ UnkStruct_SpriteManager_28 mUnk_828[8];
    /* 968 */ u8 mUnk_968[8];    // slot in use flags
    /* 970 */

    // Fade mode and master brightness per screen ([0] main, [1] sub), stepped every frame by func_020256d4.
    static s8 sFade[2];
    static s8 sBrightness[2];

    static SpriteManager &Get() {
        static SpriteManager instance;
        return instance;
    }

    SpriteScreen &Screen(int index) {
        return mScreens[index];
    }

    void func_0202740c();
    void func_02026dec(u32 unk1, u32 unk2);
    void func_02026cc0(); // end of frame: flush the OAM copies, queue the per-frame callback func_02026c4c
    void func_02026d48();
    void func_02026d90();
    void func_02027354(u32 unk1);
    void func_020273dc(u32 unk1, u32 unk2);
    void func_0202743c();
    void func_02027504(s32 index);
    void func_0202753c(s32 index);
    void func_02027568(s32 index);
    void func_02027578(s32 index, const UnkStruct_SpriteManager_28 *src);

    void StartFade(s8 mode) {
        sFade[0] = mode;
        sFade[1] = mode;
    }

    void SetFade(s8 mode, s8 brightness) {
        sFade[0]       = mode;
        sBrightness[0] = brightness;
        sFade[1]       = mode;
        sBrightness[1] = brightness;
    }

    void StartSubFade(s8 mode) {
        sFade[1] = mode;
    }

    BOOL IsFading() {
        return (sFade[0] > 1) | (sFade[1] > 1);
    }
};
