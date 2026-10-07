#pragma once

#include "global.h"
#include "types.h"

#include "Scene/SceneBase.hpp"

class CellAnimation;
class ModelInstance;
class Camera;
class UnkStruct_02029ca8;
class UnkStruct_0205401c;
struct MessageFile;

// Character preview scene object: a 3D model on a stage that slides between positions, with a name text and
// sprite cells (vtable 0x020b938c).
class UnkStruct_02074938 : public SceneBase {
public:
    /* 0c */ CellAnimation *mUnk_0c[4];
    /* 1c */ CellAnimation *mUnk_1c[10];
    /* 44 */ ModelInstance *mUnk_44;
    /* 48 */ Camera *mUnk_48;
    /* 4c */ UnkStruct_02029ca8 *mUnk_4c;
    /* 50 */ UnkStruct_0205401c *mUnk_50;
    /* 54 */ MessageFile *mUnk_54;
    /* 58 */ u8 mUnk_58;
    /* 59 */ u8 mUnk_59;
    /* 5a */ u16 mUnk_5a;
    /* 5c */ u32 mUnk_5c;
    /* 60 */ u8 mUnk_60;
    /* 61 */ u8 mUnk_61;
    /* 62 */ u16 mUnk_62;
    /* 64 */ u32 mUnk_64;
    /* 68 */ void *mUnk_68;
    /* 6c */ void *mUnk_6c;
    /* 70 */ u32 mUnk_70;
    /* 74 */ u32 mUnk_74;
    /* 78 */ s32 mUnk_78;
    /* 7c */ u8 mUnk_7c;
    /* 7d */ PAD(0x7d, 0x7e);
    /* 7e */ u8 mUnk_7e;
    /* 7f */ PAD(0x7f, 0x80);
    /* 80 */ u32 mUnk_80;
    /* 84 */ u32 mUnk_84;
    /* 88 */

    UnkStruct_02074938(u8 unk1, u16 unk2, u32 unk3, u32 unk4); // 0x020741b4
    virtual ~UnkStruct_02074938();
    virtual void Update();
    BOOL func_02074938(u8 unk1, u16 unk2, u32 unk3);
    void func_02074964();
    void func_02074828(u32 kind);
    void func_02075160();
    void func_0207520c();
    void func_020754b8();
    void func_02075740();
    void func_02075758();
    BOOL func_02075878(s32 target);
};
