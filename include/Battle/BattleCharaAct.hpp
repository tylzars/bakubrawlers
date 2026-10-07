#pragma once

#include "global.h"
#include "types.h"

#include "Scene/SceneBase.hpp"

#include <nitro/fx.h>

class Camera;
class BattleMapObj;
class UnkStruct_0205401c;
class UnkStruct_02029ca8;
class ModelInstance;

// A character (story / edited) shown on the battle field with its own camera path (vtable 0228a2d8, 0x40 bytes).
// Path strings: /Game/Character/Act, /Game/Character/Character, /Game/Character/EditedCharacter/Cam.
class BattleCharaAct : public SceneBase {
public:
    /* 0c */ Camera *mUnk_0c;
    /* 10 */ Camera *mUnk_10;
    /* 14 */ void *mUnk_14;
    /* 18 */ BattleMapObj *mUnk_18;
    /* 1c */ UnkStruct_0205401c *mUnk_1c;
    /* 20 */ UnkStruct_02029ca8 *mUnk_20;
    /* 24 */ ModelInstance *mUnk_24;
    /* 28 */ ModelInstance *mUnk_28;
    /* 2c */ u32 mUnk_2c;
    /* 30 */ PAD(0x30, 0x32);
    /* 32 */ u16 mUnk_32;
    /* 34 */ PAD(0x34, 0x35);
    /* 35 */ u8 mUnk_35;
    /* 36 */ u8 mUnk_36;
    /* 37 */ u8 mUnk_37;
    /* 38 */ u8 mUnk_38;
    /* 39 */ u8 mUnk_39;
    /* 3a */ u8 mUnk_3a;
    /* 3b */ u8 mUnk_3b;
    /* 3c */ u8 mUnk_3c;
    /* 3d */ u8 mUnk_3d;
    /* 3e */ PAD(0x3e, 0x40);
    /* 40 */

    BattleCharaAct(Camera *camera, u8 player, u32 kind, u8 unk4, u8 unk5);
    virtual ~BattleCharaAct();
    /* 08 */ virtual void Update();
    void func_ov007_02267a8c();
    u8 func_ov007_02268110();
    void func_ov007_02268190();
    void func_ov007_022684f0();
    void func_ov007_02269380();
    void func_ov007_022693f4();
    void func_ov007_02269518();
    void func_ov007_022695f4();
    void func_ov007_0226961c();
};
