#pragma once

#include "global.h"
#include "types.h"

#include "Scene/SceneBase.hpp"

class SelectWindow;
struct MessageFile;

// Created with `new` and left to manage itself (the result is not kept).
class UnkStruct_02070608 : public SceneBase {
public:
    /* 0c */ SelectWindow *mWindow;
    /* 10 */ MessageFile *mMessages;
    /* 14 */ void *mUnk_14;
    /* 18 */ u32 mUnk_18;
    /* 1c */ u32 mUnk_1c;
    /* 20 */ u32 mUnk_20;
    /* 24 */ u32 mUnk_24;
    /* 28 */ u32 mUnk_28;
    /* 2c */ u16 mUnk_2c;
    /* 2e */ u16 mUnk_2e;
    /* 30 */ u8 mUnk_30;
    /* 31 */ u8 mUnk_31;
    /* 32 */ u8 mUnk_32;
    /* 33 */ u8 mUnk_33;
    /* 34 */

    UnkStruct_02070608(u32 unk1, u32 unk2, u32 unk3, u32 unk4, u32 unk5, u32 unk6, u32 unk7, u32 unk8, u32 unk9);
    virtual ~UnkStruct_02070608();
    virtual void Update();
    void func_02070d10();
    void func_020716b0(u32 unk1);
};
