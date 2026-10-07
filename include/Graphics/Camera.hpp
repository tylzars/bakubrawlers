#pragma once

#include "global.h"
#include "types.h"

#include <nitro/fx.h>

// Camera for the 3D scenes (0x64 bytes).
class Camera {
public:
    /* 00 */ PAD(0x00, 0x08);
    /* 08 */ VecFx32 mPosition;
    /* 14 */ VecFx32 mUp;
    /* 20 */ VecFx32 mTarget;
    /* 2c */ fx32 mNear;
    /* 30 */ fx32 mFar;
    /* 34 */ PAD(0x34, 0x50);
    /* 50 */ u16 mUnk_50; // camera animation: frame count
    /* 52 */ u16 mUnk_52; // camera animation: current frame
    /* 54 */ PAD(0x54, 0x64);
    /* 64 */

    Camera(); // 0x02025e50
    void func_02026004(u32 fov); // sets the field of view
    void func_02026164(u32 frame);
    void func_02026180();
    void func_020264a4(u32 unk1);
    void func_0202610c(void *file, u32 unk2); // 0x0202610c, load a camera animation

    void SetPosition(fx32 x, fx32 y, fx32 z) {
        mPosition.x = x;
        mPosition.y = y;
        mPosition.z = z;
    }
    void SetUp(fx32 x, fx32 y, fx32 z) {
        mUp.x = x;
        mUp.y = y;
        mUp.z = z;
    }
    void SetTarget(fx32 x, fx32 y, fx32 z) {
        mTarget.x = x;
        mTarget.y = y;
        mTarget.z = z;
    }
    void SetClip(fx32 nearPlane, fx32 farPlane) {
        mNear = nearPlane;
        mFar  = farPlane;
    }
};

// Singleton at 020c1afc (ctor 020264dc, dtor 02020a40): two camera slots and a snapshot of the
// 3D engine's matrices for each (mActive selects the slot that is current).
class CameraManager {
public:
    /* 000 */ PAD(0x000, 0x008);
    /* 008 */ s32 mActive;
    /* 00c */ PAD(0x00c, 0x010);
    /* 010 */ Camera *mCameras[2];
    /* 018 */ MtxFx43 mView[2];
    /* 078 */ MtxFx44 mProjection[2];
    /* 0f8 */ MtxFx44 mMtxA[2];
    /* 178 */ MtxFx44 mMtxB[2];
    /* 1f8 */ MtxFx43 mMtxC[2];
    /* 258 */

    CameraManager();
    ~CameraManager();
    void func_02026504(u32 slot, Camera *camera, u32 unk3);

    static CameraManager &Get() {
        static CameraManager instance;
        return instance;
    }
};

// Global 3D state at 02211374.
struct UnkStruct_02211374 {
    /* 000 */ PAD(0x000, 0x008);
    /* 008 */ MtxFx44 mProjection;
    /* 048 */ PAD(0x048, 0x04c);
    /* 04c */ MtxFx43 mView;
    /* 07c */ PAD(0x07c, 0x09c);
    /* 09c */ u32 mUnk_9c;
    /* 0a0 */ PAD(0x0a0, 0x0fc);
    /* 0fc */ u32 mFlags;
    /* 100 */ PAD(0x100, 0x240);
    /* 240 */ VecFx32 mCameraPosition;
    /* 24c */ VecFx32 mCameraUp;
    /* 258 */ VecFx32 mCameraTarget;
    /* 264 */
};
extern UnkStruct_02211374 data_02211374;
extern MtxFx43 data_0220e86c;

MtxFx44 *func_02032710();
MtxFx44 *func_02032604();
MtxFx43 *func_020323b8();

// Matrix routines in ITCM (not identified as SDK functions; by shape func_01ff8d34 is a look-at builder,
// func_01ffcf64 applies a translation and func_01ff89c0 concatenates two matrices).
void func_01ff8d34(VecFx32 *pos, VecFx32 *up, VecFx32 *target, MtxFx43 *out);
void func_01ffcf64(MtxFx43 *m, VecFx32 *v);
void func_01ff89c0(MtxFx43 *a, MtxFx43 *b, MtxFx43 *out);
