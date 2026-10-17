#pragma once

#include "global.h"
#include "types.h"

class SceneBase;

// Scene ids: the values SceneRunner::Update copies from the runner's mRequest, passes to SwitchToScene and then
// switches on (SceneRunner.cpp), each named after the class Update creates for it. RequestScene carries one only
// when the caller was created by SceneRunner::Update (its mParent is the runner's node); a scene created by another
// scene uses RequestScene to hand its parent a result instead (most of them 1), which is not one of these.
// Left as literals: 1 (SwitchToScene's replacement for an id < 1 or >= 14; Update's case 1 steps on to 2), 3 (an
// empty case nothing requests), 14 (the runner's "no request" value and the Update step that waits for one), 15 (the
// step that switches), 0 (data_020d433c.mNextScene cleared).
#define SCENE_LOGO 2
#define SCENE_TITLE 4
#define SCENE_BATTLE_PLAYER_LIST 5
#define SCENE_STORE 6
#define SCENE_MYROOM 7
#define SCENE_ARENA_WINDOW 8
#define SCENE_MAP 9
#define SCENE_TUTORIAL 10
#define SCENE_GAME_OVER 11
#define SCENE_PARK 12
#define SCENE_TOURNAMENT 13

// Old view of the parent link (mList), kept so existing code keeps compiling.
struct UnkStruct_SceneRequest {
    /* 00 */ PAD(0x00, 0x12);
    /* 12 */ u16 mRequest;
};

struct UnkStruct_SceneNode {
    /* 00 */ PAD(0x00, 0x08);
    /* 08 */ UnkStruct_SceneRequest *mRequest; // the parent scene's mContext (SceneBase +0x08), not a request value
};

struct UnkStruct_SceneList {
    /* 00 */ UnkStruct_SceneNode *mHead; // the parent node's mScene
};

struct SceneContext;

// Per-scene state kept in the scene context, cleared by the scene constructors.
struct SceneInfo {
    union {
        /* 00 */ u16 mUnk_00;
        /* 00 */ u8 mStep; // low byte, used as the scene state by MyroomTipsScene
    };
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
struct SceneContext {
    /* 00 */ SceneBase *mScene;
    /* 04 */ SceneContext *mPrev;
    /* 08 */ SceneContext *mNext;
    /* 0c */ union {
        SceneContext *mParent; // node of the scene that was running when this one was added
        UnkStruct_SceneList *mList;
    };
    /* 10 */ u16 mOrder;
    /* 12 */ s16 mRequest; // set by a child's RequestScene: a SCENE_* id in SceneRunner's node, else a result code
    /* 14 */ SceneInfo mInfo;
    /* 20 */ PAD(0x20, 0x40);
    /* 40 */
};

// Base of every game scene; each overlay's entry point constructs one in place.
class SceneBase {
public:
    /* 00 (vtable) */
    /* 04 */ u8 mFinished;
    /* 08 */ SceneContext *mContext;
    /* 0c */

    // mode: 0 adds a node ordered by `order`, 1 / 2 take a layer's first / second slot, 3 is unmanaged.
    SceneBase(u32 order, u32 mode);

    /* 00 */ virtual ~SceneBase() {}
    /* 08 */ virtual void Update() = 0;
    /* 0c */

    u8 IsFinished() {
        return (mFinished == 1) & 1;
    }

    // Writes `scene` into the parent's mRequest: a SCENE_* id when the parent is SceneRunner, otherwise a value
    // that parent reads back.
    void RequestScene(int scene) {
        SceneContext *context = mContext;
        context->mParent->mScene->mContext->mRequest = scene;
    }
};
