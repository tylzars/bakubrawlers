#pragma once

#include "global.h"
#include "Graphics/BakuganModel.hpp"
#include "types.h"

#include "Scene/SceneBase.hpp"

#include "Graphics/Camera.hpp"

#include <nitro/fx.h>

class Camera;
class CellAnimation;
class MessageWindow;
class ModelInstance;
class SelectWindow;
class ModelLightGroup;
struct MessageFile;

// Overlay 8: the shoot screen, a list of six shooters with a model of the selected one.
class MyroomShootScene : public SceneBase {
public:
    /* 00 (base) */
    /* 0c */ Camera *mCamera;
    /* 10 */ ModelLightGroup *mLights;
    /* 14 */ CellAnimation *mCellAnim;
    /* 18 */ BakuganModel *mShooter;
    /* 1c */ ModelInstance *mRoom;
    /* 20 */ MessageFile *mMessages;
    /* 24 */ void *mTextBuffers[2];
    /* 2c */ void *mGlyphBuffers[2];
    /* 34 */ MessageWindow *mMessage;
    /* 38 */ SelectWindow *mWindow;
    /* 3c */ u32 mResult;
    /* 40 */ u32 mTextBufferSize[2];
    /* 48 */ u32 mGlyphCount[2];
    /* 50 */ u16 mEffect;
    /* 52 */ u16 mRot[3];
    /* 58 */ u16 mUnk_58;
    /* 5a */ s16 mUnk_5a;
    /* 5c */ u16 mUnk_5c;
    /* 5e */ s8 mUnk_5e;
    /* 5f */ s8 mUnk_5f;
    /* 60 */ s8 mSelected;
    /* 61 */ u8 mUnk_61;
    /* 62 */ u8 mUnk_62;
    /* 63 */ s8 mUnk_63;
    /* 64 */ u8 mUnlocked[6];
    /* 6a */ u8 mState;
    /* 6b */ s8 mTimer;
    /* 6c */

    MyroomShootScene(); // 0x02221030

    /* 00 */ virtual ~MyroomShootScene() override; // 0x022210fc
    /* 08 */ virtual void Update() override;       // 0x02221674
    /* 0c */

    void InitDisplay();            // 0x02221830
    void func_ov008_02221a14();
    void func_ov008_02222474(s8 index);
    void InitUnlocked();           // 0x02222714
    void Draw();                   // 0x0222276c
    void DrawSprites();            // 0x02222784
    void DrawWindows();            // 0x02222a40
    void ClearTextBuffers(u8 index); // 0x02222a5c
    void func_ov008_02222a78();
    s32 func_ov008_02222dbc();
    s8 func_ov008_02222fa4();
    void func_ov008_02223264();
};
