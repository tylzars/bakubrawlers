#pragma once

#include "global.h"
#include "types.h"

#include "Scene/SceneBase.hpp"

class EffectManager;
class Camera;
class ModelInstance;

class BattleCardModel { // 0x98 bytes, ctor 02272540
public:
    u8 mPad[0x98];
    BattleCardModel(EffectManager *effects, Camera *camera, u32 a, u32 b, u32 c, u32 d);
};

// Window showing the real bakugan model of a card (realbase_00 / realnormal_99): Update loads the models and
// then draws them with the camera of the current battle. Vtable 02289de0 (the symbol starts at 02289dd8).
void func_ov007_02273e3c(BattleCardModel *model); // releases the model

class BattleCardSpec : public SceneBase {
public:
    /* 0c */ BattleCardModel *mUnk_0c;
    /* 10 */ ModelInstance *mUnk_10;
    /* 14 */ ModelInstance *mUnk_14;
    /* 18 */ u8 mState;
    /* 19 */ u8 mUnk_19;
    /* 1a */ u8 mUnk_1a;
    /* 1b */ u8 mUnk_1b;
    /* 1c */ u8 mUnk_1c;
    /* 1d */ u8 mUnk_1d;
    /* 1e */ u8 mUnk_1e;
    /* 1f */ PAD(0x1f, 0x20);
    /* 20 */

    BattleCardSpec(u32 a, u32 b, u32 c, u8 d);
    virtual ~BattleCardSpec();
    virtual void Update();
    void func_ov007_0225fbdc();
};
