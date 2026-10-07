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

struct UnkStruct_SceneContext {
    /* 00 */ PAD(0x00, 0x0c);
    /* 0c */ UnkStruct_SceneList *mList;
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
