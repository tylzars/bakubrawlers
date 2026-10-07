#include "Scene/SceneManager.hpp"

SceneManager *SceneManager::Init(SceneBase *root) {
    mRootNode  = &mRoot;
    mPool      = mNodes;
    mPoolSize  = SCENE_NODE_COUNT;
    mLayerCount = SCENE_LAYER_COUNT;
    mRoot.mScene = root;
    *(u32 *) &mRoot.mOrder = (u32) (mRoot.mPrev = mRoot.mNext = mRoot.mParent = 0);
    Reset();
    return this;
}

void SceneManager::Update() {
    mCurrent = mRootNode;
    mRootNode->mScene->Update();
    UpdateLayer();
}

void SceneManager::UpdateLayer() {
    UnkStruct_SceneContext *node = &mPool[mLayer * 2];
    do {
        mCurrent = node;
        node->mScene->Update();
        SceneBase *scene = node->mScene;
        if (scene->IsFinished()) {
            delete scene;
            Remove();
        }
        node = node->mNext;
    } while (node != 0);
}

UnkStruct_SceneContext *SceneManager::SetSlot1(SceneBase *scene) {
    if (scene == 0) {
        scene = &mNull;
    }
    UnkStruct_SceneContext *slot = &mPool[mLayer * 2 + 1];
    SceneBase *old = slot->mScene;
    if ((old != 0) & (old != &mNull)) {
        delete old;
    }
    slot->mScene = scene;
    return slot;
}

UnkStruct_SceneContext *SceneManager::SetSlot0(SceneBase *scene) {
    if (scene == 0) {
        scene = &mNull;
    }
    UnkStruct_SceneContext *slot = &mPool[mLayer * 2];
    SceneBase *old = slot->mScene;
    if ((old != 0) & (old != &mNull)) {
        delete old;
    }
    slot->mScene = scene;
    return slot;
}

void SceneManager::LinkSentinels(UnkStruct_SceneContext *first, UnkStruct_SceneContext *second) {
    first->mPrev   = 0;
    first->mNext   = second;
    first->mParent = mRootNode;
    *(u32 *) &first->mOrder = 0;
    second->mPrev   = first;
    second->mNext   = 0;
    second->mParent = mRootNode;
    *(u32 *) &second->mOrder = 0xffff;
}

void SceneManager::DeleteLayer() {
    UnkStruct_SceneContext *node = &mPool[mLayer * 2];
    do {
        if (node->mScene != &mNull) {
            mCurrent = node;
            SceneBase *scene = node->mScene;
            if (scene != 0) {
                delete scene;
            }
            Remove();
        }
        node = node->mNext;
    } while (node != 0);
}

void SceneManager::InitLayers() {
    int count = mLayerCount;
    UnkStruct_SceneContext *node = mPool;
    for (; count != 0; count--) {
        LinkSentinels(node, node + 1);
        node += 2;
    }
    mCount    = 0;
    mLayer    = 0;
    mNextFree = mLayerCount * 2 - 1;
    for (s32 i = mLayerCount * 2; i < mPoolSize; i++) {
        mPool[i].mScene = 0;
    }
}

void SceneManager::Reset() {
    InitLayers();
    s32 count = mLayerCount * 2;
    UnkStruct_SceneContext *node = mPool;
    if (count == 0) {
        return;
    }
    do {
        SceneBase *scene = node->mScene;
        if ((scene != 0) & (scene != &mNull)) {
            delete scene;
        }
        node->mScene = &mNull;
        node++;
        count--;
    } while (count != 0);
}

UnkStruct_SceneContext *SceneManager::Add(SceneBase *scene, u32 order) {
    do {
        mNextFree++;
        if (mNextFree >= mPoolSize) {
            mNextFree = 0;
        }
    } while (mPool[mNextFree].mScene != 0);

    UnkStruct_SceneContext *next = &mPool[mLayer * 2];
    do {
        next = next->mNext;
    } while (next->mOrder < order);

    UnkStruct_SceneContext *node = &mPool[mNextFree];
    UnkStruct_SceneContext *prev = next->mPrev;
    next->mPrev = node;
    prev->mNext = node;
    node->mScene  = scene;
    node->mPrev   = prev;
    node->mNext   = next;
    node->mParent = mCurrent;
    node->mOrder  = order;
    node->mUnk_12 = 0;
    mCount++;
    u32 *words = (u32 *) &node->mInfo;
    words[0]  = 0;
    words[1]  = 0;
    words[2]  = 0;
    words[3]  = 0;
    words[4]  = 0;
    words[5]  = 0;
    words[6]  = 0;
    words[7]  = 0;
    words[8]  = 0;
    words[9]  = 0;
    words[10] = 0;
    return node;
}

void SceneManager::Remove() {
    if (mCount != 0) {
        mCurrent->mPrev->mNext = mCurrent->mNext;
        mCurrent->mNext->mPrev = mCurrent->mPrev;
        mCurrent->mScene       = 0;
        mCount--;
    }
}

SceneBase::SceneBase(u32 order, u32 mode) {
    switch (mode) {
        case 0:
            mContext = SceneManager::Get().Add(this, order);
            break;
        case 1:
            mContext = SceneManager::Get().SetSlot0(this);
            break;
        case 2:
            mContext = SceneManager::Get().SetSlot1(this);
            break;
        case 3:
            mContext = 0;
            break;
    }
    mFinished = 0;
}
