#pragma once

#include "global.h"
#include "types.h"

class SceneBase;

// Old view of the parent link (mList), kept so existing code keeps compiling.
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

struct UnkStruct_SceneContext;

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

// One entry of the scene manager's list (0x40 bytes, SceneManager::mNodes). Scenes keep a pointer to theirs in mContext.
struct UnkStruct_SceneContext {
    /* 00 */ SceneBase *mScene;
    /* 04 */ UnkStruct_SceneContext *mPrev;
    /* 08 */ UnkStruct_SceneContext *mNext;
    /* 0c */ union {
        UnkStruct_SceneContext *mParent; // node of the scene that was running when this one was added
        UnkStruct_SceneList *mList;
    };
    /* 10 */ u16 mOrder;
    /* 12 */ s16 mUnk_12;
    /* 14 */ SceneInfo mInfo;
    /* 20 */ PAD(0x20, 0x40);
    /* 40 */
};

// Base of every game scene; each overlay's entry point constructs one in place.
class SceneBase {
public:
    /* 00 (vtable) */
    /* 04 */ u8 mFinished;
    /* 08 */ UnkStruct_SceneContext *mContext;
    /* 0c */

    // mode: 0 adds a node ordered by `order`, 1 / 2 take a layer's first / second slot, 3 is unmanaged.
    SceneBase(u32 order, u32 mode);

    /* 00 */ virtual ~SceneBase() {}
    /* 08 */ virtual void Update() = 0;
    /* 0c */

    u8 IsFinished() {
        return (mFinished == 1) & 1;
    }

    void RequestScene(int scene) {
        UnkStruct_SceneContext *context = mContext;
        context->mParent->mScene->mContext->mUnk_12 = scene;
    }
};
