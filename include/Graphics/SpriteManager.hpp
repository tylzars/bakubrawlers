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

struct SpriteFadeDefaults {
    /* 00 */ unk32 mUnk_00;
    /* 04 */

    SpriteFadeDefaults();
};

// One screen's OBJ (sprite) state: character and palette uploads.
struct SpriteScreen {
    /* 000 */ SpriteOamEntry mOam[128];
    /* 400 */ u16 mCharSize; // OBJ tile unit (bytes), from the mode table
    /* 402 */ u8 mBgIndex;
    /* 403 */ u8 mObjVramMode; // OBJ mapping mode
    /* 404 */ u8 mOamCount; // OAM entries used
    /* 405 */ u8 mMatrixCount; // matrices used
    /* 406 */ PAD(0x406, 0x408);
    /* 408 */

    SpriteScreen();
    void SetBgIndex(u32 unk1); // set mBgIndex
    void FlushOam();         // upload the OAM copy and reset it
    void ResetOam();         // reset the OAM copy
    u32 LoadCharacters(const char *path, u32 offset, u32 unk3);
    u32 LoadPalette(const char *path, u32 offset, u32 unk3);
    void LoadExtPalette(const char *path, u32 unk2, u32 unk3); // palette load variant
    void *func_02028054(u8 *data); // reorders 0x1000 bytes of character data into a new buffer and frees data
    u32 GetCharBase(u32 unk1);
    void SetObjVramMode(s32 unk1);
    void AddOam(void *buffer, u8 count, s16 unk3, s16 unk4, u32 unk5, u32 unk6, u32 unk7, u32 unk8);
};

// One of the eight slots at SpriteManager+0x828 (copied by SetSlot, set up by func_020360cc).
struct UnkStruct_SpriteManager_28 {
    /* 00 */ u32 mUnk_00[10];
    /* 28 */
};

// Global sprite/display manager, constructed on first use.
class SpriteManager {
public:
    /* 000 */ SpriteFadeDefaults mFadeDefaults;
    /* 004 */ SpriteScreen mScreens[2];
    /* 814 */ PAD(0x814, 0x824); // initialised by func_020313f8
    /* 824 */ u32 mMode;         // VRAM/display layout 1..9 chosen by SetupMode
    /* 828 */ UnkStruct_SpriteManager_28 mUnk_828[8];
    /* 968 */ u8 mSlotUsed[8];    // slot in use flags
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
    void SetupMode(u32 unk1, u32 unk2);
    void EndFrame(); // end of frame: flush the OAM copies, queue the per-frame callback func_02026c4c
    void Init();
    void func_02026d90();
    void ResetVram(u32 unk1);
    void func_020273dc(u32 unk1, u32 unk2);
    void Init3D();
    void func_02027504(s32 index);
    void func_0202753c(s32 index);
    void ClearSlotUsed(s32 index);
    void SetSlot(s32 index, const UnkStruct_SpriteManager_28 *src);

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
