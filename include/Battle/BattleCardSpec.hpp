#pragma once

#include "global.h"
#include "types.h"

#include "Battle/BattleCardModel.hpp"
#include "Scene/SceneBase.hpp"

class EffectManager;
class Camera;
class ModelInstance;

// Window showing the real bakugan model of a card (realbase_00 / realnormal_99): Update loads the models and
// then draws them with the camera of the current battle. Vtable 02289de0 (the symbol starts at 02289dd8).

class BattleCardSpec : public SceneBase {
public:
    /* 0c */ BattleCardModel *mCardModel;
    /* 10 */ ModelInstance *mRealbaseModel;
    /* 14 */ ModelInstance *mRealnormalModel;
    /* 18 */ u8 mState;
    /* 19 */ u8 mUnk_19;
    /* 1a */ u8 mCardModelType;
    /* 1b */ u8 mBakuganId;
    /* 1c */ u8 mVariant;
    /* 1d */ u8 mUnk_1d;
    /* 1e */ u8 mUnk_1e;
    /* 1f */ PAD(0x1f, 0x20);
    /* 20 */

    BattleCardSpec(u32 a, u32 b, u32 c, u8 d);
    virtual ~BattleCardSpec();
    virtual void Update();
    void LoadModels();
};
