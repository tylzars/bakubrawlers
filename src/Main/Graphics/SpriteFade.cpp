#include "Graphics/SpriteManager.hpp"

void func_02001eec(vu16 *reg, int value);

typedef void (*FadeFunc)(s32 brightness);

void func_02025668(int brightness);
void func_02025680(int brightness);

FadeFunc data_020a54e4[2] = {func_02025680, func_02025668};

FadeState SpriteManager::sFade;
FadeState SpriteManager::sBrightness;

void func_02025668(int brightness) {
    func_02001eec((vu16 *) 0x400106c, brightness);
}

void func_02025680(int brightness) {
    func_02001eec((vu16 *) 0x400006c, brightness);
}

UnkStruct_SpriteManager_00::UnkStruct_SpriteManager_00() {
    for (u8 i = 0; i < 2; i++) {
        (&SpriteManager::sFade.mMain)[i]       = 1;
        (&SpriteManager::sBrightness.mMain)[i] = 16;
    }
}

void func_020256d4() {
    for (s8 i = 1; i >= 0; i--) {
        s8 *fade       = &(&SpriteManager::sFade.mMain)[i];
        s8 *brightness = &(&SpriteManager::sBrightness.mMain)[i];
        switch (*fade) {
            case 1:
                data_020a54e4[i](*brightness);
                break;
            case 2:
                if (*brightness == 0) {
                    *fade = 1;
                } else if (*brightness < 0) {
                    *brightness += 1;
                } else {
                    *brightness -= 1;
                }
                data_020a54e4[i](*brightness);
                break;
            case 3:
                if (*brightness > -16) {
                    *brightness -= 1;
                } else {
                    *fade = 1;
                }
                data_020a54e4[i](*brightness);
                break;
            case 4:
                if (*brightness < 16) {
                    *brightness += 1;
                } else {
                    *fade = 1;
                }
                data_020a54e4[i](*brightness);
                break;
        }
    }
}

