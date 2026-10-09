#pragma once

#include "Scene/SceneBase.hpp"
#include "global.h"
#include "types.h"

#define SCENE_NODE_COUNT 0x400
#define SCENE_LAYER_COUNT 2

// Placeholder scene occupying empty slots (vtable 0x020a5438, emitted with the game start-up code).
class NullScene : public SceneBase {
public:
    NullScene() : SceneBase(0, 3) {}
    virtual ~NullScene() {}
    virtual void Update(); // func_020219c4, defined in Game.cpp: the key function, so the vtable is emitted there only
};

// Owns every running scene. Scenes live in a doubly linked list of fixed nodes; each "layer" has two sentinel
// nodes (order 0 and 0xffff) and every SceneBase constructor links its node into the active layer (mode 0), or
// replaces a layer's first (mode 1) or second (mode 2) sentinel slot.
class SceneManager {
public:
    /* 00000 */ UnkStruct_SceneContext mRoot; // node 0 stands for the root scene passed to Init
    /* 00040 */ UnkStruct_SceneContext mNodes[SCENE_NODE_COUNT];
    /* 10040 */ UnkStruct_SceneContext *mCurrent; // node whose scene is being updated
    /* 10044 */ UnkStruct_SceneContext *mRootNode;
    /* 10048 */ UnkStruct_SceneContext *mPool;
    /* 1004c */ u16 mPoolSize;
    /* 1004e */ u8 mLayerCount;
    /* 1004f */ u8 mLayer; // layer new scenes are added to
    /* 10050 */ u16 mCount;
    /* 10052 */ u16 mNextFree;
    /* 10054 */ NullScene mNull;
    /* 10060 */

    ~SceneManager() {}


    static SceneManager &Get() {
        static SceneManager instance;
        return instance;
    }

    SceneManager *Init(SceneBase *root);
    void Update();
    void UpdateLayer();
    UnkStruct_SceneContext *SetSlot0(SceneBase *scene);
    UnkStruct_SceneContext *SetSlot1(SceneBase *scene);
    void LinkSentinels(UnkStruct_SceneContext *first, UnkStruct_SceneContext *second);
    void DeleteLayer();
    void InitLayers();
    void Reset();
    UnkStruct_SceneContext *Add(SceneBase *scene, u32 order);
    void Remove();
};
