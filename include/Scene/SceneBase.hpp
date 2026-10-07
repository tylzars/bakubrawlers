#pragma once

#include "global.h"
#include "types.h"

struct UnkStruct_SceneRequest {
    /* 00 */ PAD(0x00, 0x12);
    /* 12 */ u16 mNextScene;
};

struct UnkStruct_SceneNode {
    /* 00 */ PAD(0x00, 0x08);
    /* 08 */ UnkStruct_SceneRequest *mRequest;
};

struct UnkStruct_SceneList {
    /* 00 */ UnkStruct_SceneNode *mHead;
};

// Per-scene state kept in the scene context, cleared by the scene constructors.
struct SceneInfo {
    /* 00 */ u16 mUnk_00;
    /* 02 */ u8 mState;
    /* 03 */ u8 mMenuResult;
    /* 04 */ s8 mPrevSelection;
    /* 05 */ s8 mSelection;
    /* 06 */ u8 mUnk_06;
    /* 07 */ u8 mScroll;
    /* 08 */ u8 mScrollDir;
    /* 09 */ u8 mPage;
    /* 0a */ u8 mUnk_0a;
    /* 0b */ u8 mUnk_0b;
    /* 0c */
};

struct UnkStruct_SceneContext {
    /* 00 */ PAD(0x00, 0x0c);
    /* 0c */ UnkStruct_SceneList *mList;
    /* 10 */ PAD(0x10, 0x12);
    /* 12 */ s16 mUnk_12;
    /* 14 */ SceneInfo mInfo;
    /* 20 */
};

// Base of every game scene; each overlay's entry point constructs one in place.
class SceneBase {
public:
    /* 00 (vtable) */
    /* 04 */ u8 mFinished;
    /* 08 */ UnkStruct_SceneContext *mContext;
    /* 0c */

    SceneBase(u32 unk1, u32 unk2);

    /* 00 */ virtual ~SceneBase() {}
    /* 08 */ virtual void Update() = 0;
    /* 0c */

    void RequestScene(int scene) {
        mContext->mList->mHead->mRequest->mNextScene = scene;
    }
};
