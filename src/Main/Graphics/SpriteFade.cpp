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
        SpriteManager::sFade[i]       = FADE_MODE_HOLD;
        SpriteManager::sBrightness[i] = 16;
    }
}

void UpdateSpriteFade() {
    for (s8 i = 1; i >= 0; i--) {
        s8 *fade       = &SpriteManager::sFade[i];
        s8 *brightness = &SpriteManager::sBrightness[i];
        switch (*fade) {
            case FADE_MODE_HOLD:
                gBrightnessFuncs[i](*brightness);
                break;
            case FADE_MODE_TO_ZERO:
                if (*brightness == 0) {
                    *fade = FADE_MODE_HOLD;
                } else if (*brightness < 0) {
                    *brightness += 1;
                } else {
                    *brightness -= 1;
                }
                gBrightnessFuncs[i](*brightness);
                break;
            case FADE_MODE_DOWN:
                if (*brightness > -16) {
                    *brightness -= 1;
                } else {
                    *fade = FADE_MODE_HOLD;
                }
                gBrightnessFuncs[i](*brightness);
                break;
            case FADE_MODE_UP:
                if (*brightness < 16) {
                    *brightness += 1;
                } else {
                    *fade = FADE_MODE_HOLD;
                }
                gBrightnessFuncs[i](*brightness);
                break;
        }
    }
}

