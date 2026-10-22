#include <stdio.h>
#include "System/FileSystem.hpp"
#include "Battle/GimmickTypes.hpp"
#include <nitro/mi.h>

static const char *sSetDataFormat = "SetData_%02d_%02d.bin";
int LoadGimmickSetFile(GimmickSetFile *file, const char *path);

GimmickManager::GimmickManager() {
    mNodes = new GimmickNode[0x40];
    MI_CpuClear32(mNodes, 0x300);
    mIds = new u8[0x40];
    for (u8 i = 0; i < 0x40; i++) {
        mIds[i] = i;
    }
    mTop = 1;
}

GimmickManager::~GimmickManager() {
    for (u8 i = 0; i < 0x40; i++) {
        Gimmick *g = mNodes[i].mGimmick;
        if (g) {
            g->Finish();
        }
    }
    delete[] mIds;
    delete[] mNodes;
}

void GimmickManager::LoadAll() {
    for (GimmickNode *n = mNodes->mNext; n; n = n->mNext) {
        n->mGimmick->Load();
    }
    mState = 3;
}

void GimmickManager::UnloadAll() {
    for (GimmickNode *n = mNodes->mNext; n; n = n->mNext) {
        n->mGimmick->Unload();
    }
}

void GimmickManager::CountTurnAll() {
    for (GimmickNode *n = mNodes->mNext; n; n = n->mNext) {
        n->mGimmick->CountTurn();
    }
}

void GimmickManager::CreateFromFile(GimmickSetFile *file) {
    u8 i = 0;
    if (file->mCount == 0) {
        return;
    }
    do {
        Gimmick *g = 0;
        switch (file->mEntries[i].mType) {
        case GIMMICK_TYPE_FIELD_POWER_S:
            g = new GimmickFieldPowerS(mIds[mTop], 0);
            break;
        case GIMMICK_TYPE_FIELD_POWER_G:
            g = new GimmickFieldPowerG(mIds[mTop]);
            break;
        case GIMMICK_TYPE_FIELD_DESTROY:
            g = new GimmickFieldDestroy(mIds[mTop]);
            break;
        case GIMMICK_TYPE_BLOCK:
            g = new GimmickBlock(mIds[mTop]);
            break;
        case GIMMICK_TYPE_TORNADO:
            g = new GimmickTornado(mIds[mTop], 1);
            break;
        case GIMMICK_TYPE_BAKUGAN:
            g = new GimmickBakugan(mIds[mTop], file->mEntries[i].mParam);
            break;
        case GIMMICK_TYPE_DASH_BASE:
            g = new GimmickDashBase(mIds[mTop]);
            break;
        case GIMMICK_TYPE_JUMP_OBJ:
            g = new GimmickJumpObj(mIds[mTop]);
            break;
        case GIMMICK_TYPE_SUTE_WALL:
            g = new GimmickSuteWall(mIds[mTop]);
            break;
        case GIMMICK_TYPE_IKAZUCHI_START:
            g = new GimmickIkazuchiStart(mIds[mTop]);
            break;
        case GIMMICK_TYPE_HAOS_BALL:
            g = new GimmickHaosBall(mIds[mTop]);
            break;
        case GIMMICK_TYPE_HAOS_WALL:
            g = new GimmickHaosWall(mIds[mTop], file->mEntries[i].mParam);
            break;
        case GIMMICK_TYPE_VENT_KAZE:
            g = new GimmickVentKaze(mIds[mTop]);
            break;
        case GIMMICK_TYPE_EXE_POINT:
            if (data_020d433c.mScriptMode) {
                g = new GimmickExePoint(mIds[mTop]);
            } else {
                g = new GimmickFieldPowerS(mIds[mTop], 1);
            }
            break;
        case GIMMICK_TYPE_PYRUS_ERUPTION:
            g = new GimmickPyrusEruption(mIds[mTop]);
            break;
        case 0x1f:
            g = new GimmickUnk_02274830(mIds[mTop]);
            break;
        case GIMMICK_TYPE_ABILITY_FIELD:
            g = new GimmickAbilityField(mIds[mTop], file->mEntries[i].mParam);
            break;
        case 0x28:
            g = new GimmickPull(mIds[mTop]);
            break;
        case GIMMICK_TYPE_AQUOS_FUKIDASHI:
            g = new GimmickAquosFukidashi(mIds[mTop]);
            break;
        case 0x3d:
            g = new GimmickUnk_02230cec(mIds[mTop]);
            break;
        }
        Add(g);
        g->mPosition = file->mEntries[i].mPosition;
        g->mRotX = file->mEntries[i].mRotX;
        g->mRotY = file->mEntries[i].mRotY;
        g->mRotZ = file->mEntries[i].mRotZ;
        i++;
    } while (i < file->mCount);
}

void GimmickManager::LoadSetData(int a, int b) {
    char buf[0x804];
    char path[0x40];
    sprintf(path, sSetDataFormat, a, b);
    ChangeDir("/Game/Gimmick/SetData");
    if (LoadGimmickSetFile((GimmickSetFile *)buf, path)) {
        CreateFromFile((GimmickSetFile *)buf);
    }
    ChangeDir("/");
}

int GimmickManager::Add(Gimmick *gimmick) {
    u8 top   = mTop;
    u8 id    = mIds[top];
    GimmickNode *node = &mNodes[id];
    mTop = (top + 1) % 0x40;
    if (mTop == 0) {
        mTop++;
    }
    node->mNext = mNodes->mNext;
    node->mPrev = mNodes;
    mNodes->mNext = node;
    if (node->mNext) {
        node->mNext->mPrev = node;
    }
    node->mGimmick = gimmick;
    return id;
}

void GimmickManager::Remove(int id) {
    mNodes[id].mGimmick = 0;
    mNodes[id].mPrev->mNext = mNodes[id].mNext;
    if (mNodes[id].mNext) {
        mNodes[id].mNext->mPrev = mNodes[id].mPrev;
    }
    mTop--;
    if (mTop == 0) {
        mTop = 0x3f;
    }
    mIds[mTop] = id;
}
