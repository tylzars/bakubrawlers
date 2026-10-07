#pragma once

#include "global.h"
#include "types.h"

#include "Scene/SceneBase.hpp"

class UnkStruct_02071868;

// Filled by func_020073d8; the first byte is a kind (1 or 2 skips the scene).
struct UnkStruct_020073d8 {
    /* 00 */ u8 mKind;
    /* 01 */ PAD(0x01, 0x54);
    /* 54 */
};

void func_020073d8(UnkStruct_020073d8 *info);

// Overlay 0 scene with vtable 0x02226e4c: shows an object until A or a touch, then requests scene 1.
class UnkScene_02226e4c : public SceneBase {
public:
    /* 00 (base) */
    /* 0c */ UnkStruct_02071868 *mObj0c;
    /* 10 */ UnkStruct_02071868 *mObj10;
    /* 14 */ u8 mState;
    /* 15 */

    UnkScene_02226e4c();

    /* 00 */ virtual ~UnkScene_02226e4c() override;
    /* 08 */ virtual void Update() override;
    /* 0c */

    void InitDisplay(); // 0221c524
};

// Overlay 0 scene with vtable 0x02226e60.
class UnkScene_02226e60 : public SceneBase {
public:
    /* 00 (base) */
    /* 0c */ u16 mFrames;
    /* 0e */ u8 mState;
    /* 0f */

    UnkScene_02226e60();

    /* 00 */ virtual ~UnkScene_02226e60() override;
    /* 08 */ virtual void Update() override; // 0221c71c, not decompiled yet
    /* 0c */

    void InitDisplay(); // 0221cf30
    void func_ov000_0221d21c();
    void func_ov000_0221d2e8();
    void func_ov000_0221d3b4();
    void func_ov000_0221d500();
    void func_ov000_0221d5d4();
    BOOL WaitFrames(u32 maxFrames, u32 minFrames); // 0221d034
};
