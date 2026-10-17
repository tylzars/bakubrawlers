#include "UI/MessageScreen.hpp"

#include "Graphics/Display.hpp"
#include "Graphics/SpriteManager.hpp"
#include "System/FileSystem.hpp"
#include "System/GameSystem.hpp"
#include "Text/TextManager.hpp"
#include "UI/MessageWindow.hpp"

#include <nitro/gx.h>
#include <nitro/mi.h>
#include <nitro/os.h>

extern u16 gTextPalette;

MessageScreen::MessageScreen(u32 unk1) {
    GX_SetDispSelect(1);
    SpriteManager::Get().SetupMode(1, 1);
    GX_SetGraphicsMode(1, 0, 0);
    GXS_SetGraphicsMode(0);
    GX_SetVisiblePlane(0);
    GXS_SetVisiblePlane(1);
    G2x_SetMasterBrightness_((vu16 *) 0x400006c, -16);
    G2x_SetMasterBrightness_((vu16 *) 0x400106c, 0);
    G2_BlendNone();
    G2S_BlendNone();
    SpriteManager::Get().SetFade(1, 0);
    G2S_SetBG0Control(0, 1, 0, 1, 1);
    MessageFile *file;
    if (unk1 != MESSAGE_SCREEN_TITLE_8) {
        file = (MessageFile *) LoadFile("/font/mes_system.mes", 0, 0, 0, 0, 0);
    } else {
        file = (MessageFile *) LoadFile("/font/mes_title.mes", 0, 0, 0, 0, 0);
    }
    TextManager::Get();
    void *glyphs = OS_Alloc(0xc000);
    MIi_CpuClear16(0, glyphs, 0xc000);
    TextManager::Get();
    void *map = OS_Alloc(0x600);
    MIi_CpuClear16(0, map, 0x600);
    u32 index;
    switch (unk1) {
        case MESSAGE_SCREEN_SYSTEM_5_HALT:
            index = 5;
            break;
        case MESSAGE_SCREEN_SYSTEM_4_HALT:
            index = 4;
            break;
        case MESSAGE_SCREEN_TITLE_8:
            index = 8;
            break;
    }
    TextManager &text = TextManager::Get();
    s16 width;
    s16 height;
    height = 0x40;
    width  = 0x20;
    text.Print(file->GetText(index), file->GetLength(index), glyphs, 0x20, 0x18, &width, &height, NULL, 1, 2, 0, 0x10, 0,
               0, 0, 1, 1);
    OS_Free(file);
    TextManager::Get().FillScrChars(1, (u16 *) map, 0, 0, 0x20, 0x18, 0x20);
    GXS_LoadBG0Char(glyphs, 0, 0xc000);
    OS_Free(glyphs);
    GXS_LoadBG0Scr(map, 0, 0x600);
    OS_Free(map);
    GX_BeginLoadSubBGExtPltt();
    GX_LoadSubBGExtPltt(&gTextPalette, 0x4000, 0x20);
    GX_EndLoadSubBGExtPltt();
    G2S_SetBG0Offset(0x10, 0);
    if (unk1 != MESSAGE_SCREEN_TITLE_8) {
        GameSystem::Get().mHalted = 1;
    }
}

MessageScreen::~MessageScreen() {
}

