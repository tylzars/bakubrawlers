#include "Graphics/SpriteManager.hpp"

extern "C" void G2x_SetMasterBrightness_(vu16 *reg, int value);

typedef void (*FadeFunc)(s32 brightness);

void SetScreen1Brightness(int brightness);
void SetScreen0Brightness(int brightness);

FadeFunc gBrightnessFuncs[2] = {SetScreen0Brightness, SetScreen1Brightness};

s8 SpriteManager::sFade[2];
s8 SpriteManager::sBrightness[2];

void SetScreen1Brightness(int brightness) {
    G2x_SetMasterBrightness_((vu16 *) 0x400106c, brightness);
}

void SetScreen0Brightness(int brightness) {
    G2x_SetMasterBrightness_((vu16 *) 0x400006c, brightness);
}

SpriteFadeDefaults::SpriteFadeDefaults() {
    for (u8 i = 0; i < 2; i++) {
        SpriteManager::sFade[i]       = 1;
        SpriteManager::sBrightness[i] = 16;
    }
}

void UpdateSpriteFade() {
    for (s8 i = 1; i >= 0; i--) {
        s8 *fade       = &SpriteManager::sFade[i];
        s8 *brightness = &SpriteManager::sBrightness[i];
        switch (*fade) {
            case 1:
                gBrightnessFuncs[i](*brightness);
                break;
            case 2:
                if (*brightness == 0) {
                    *fade = 1;
                } else if (*brightness < 0) {
                    *brightness += 1;
                } else {
                    *brightness -= 1;
                }
                gBrightnessFuncs[i](*brightness);
                break;
            case 3:
                if (*brightness > -16) {
                    *brightness -= 1;
                } else {
                    *fade = 1;
                }
                gBrightnessFuncs[i](*brightness);
                break;
            case 4:
                if (*brightness < 16) {
                    *brightness += 1;
                } else {
                    *fade = 1;
                }
                gBrightnessFuncs[i](*brightness);
                break;
        }
    }
}

