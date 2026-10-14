#include <stdio.h>
#include "System/FileSystem.hpp"
#include "Battle/GimmickTypes.hpp"
#include <nitro/mi.h>

static const char *sSetDataFormat = "SetData_%02d_%02d.bin";
int func_ov007_02271a00(GimmickSetFile *file, const char *path);

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

void GimmickManager::func_ov007_0222e25c() {
    for (GimmickNode *n = mNodes->mNext; n; n = n->mNext) {
        n->mGimmick->vfunc_18();
    }
}

void GimmickManager::func_ov007_0222e290(GimmickSetFile *file) {
    u8 i = 0;
    if (file->mCount == 0) {
        return;
    }
    do {
        Gimmick *g = 0;
        switch (file->mEntries[i].mType) {
        case 3:
            g = new GimmickFieldPowerS(mIds[mTop], 0);
            break;
        case 2:
            g = new GimmickFieldPowerG(mIds[mTop]);
            break;
        case 7:
            g = new GimmickFieldDestroy(mIds[mTop]);
            break;
        case 6:
            g = new GimmickBlock(mIds[mTop]);
            break;
        case 0x32:
            g = new GimmickTornado(mIds[mTop], 1);
            break;
        case 8:
            g = new GimmickBakugan(mIds[mTop], file->mEntries[i].mParam);
            break;
        case 5:
            g = new GimmickDashBase(mIds[mTop]);
            break;
        case 4:
            g = new GimmickJumpObj(mIds[mTop]);
            break;
        case 0x3c:
            g = new GimmickSuteWall(mIds[mTop]);
            break;
        case 0x52:
            g = new GimmickIkazuchiStart(mIds[mTop]);
            break;
        case 0x46:
            g = new GimmickHaosBall(mIds[mTop]);
            break;
        case 0x47:
            g = new GimmickHaosWall(mIds[mTop], file->mEntries[i].mParam);
            break;
        case 0x33:
            g = new GimmickVentKaze(mIds[mTop]);
            break;
        case 1:
            if (data_020d433c.mUnk_7a) {
                g = new GimmickExePoint(mIds[mTop]);
            } else {
                g = new GimmickFieldPowerS(mIds[mTop], 1);
            }
            break;
        case 0x1e:
            g = new GimmickPyrusEruption(mIds[mTop]);
            break;
        case 0x1f:
            g = new GimmickUnk_02274830(mIds[mTop]);
            break;
        case 9:
            g = new GimmickAbilityField(mIds[mTop], file->mEntries[i].mParam);
            break;
        case 0x28:
            g = new GimmickUnk_022308a8(mIds[mTop]);
            break;
        case 0x29:
            g = new GimmickAquosFukidashi(mIds[mTop]);
            break;
        case 0x3d:
            g = new GimmickUnk_02230cec(mIds[mTop]);
            break;
        }
        func_ov007_0222e7f4(g);
        g->mPosition = file->mEntries[i].mPosition;
        g->mRotX = file->mEntries[i].mRotX;
        g->mRotY = file->mEntries[i].mRotY;
        g->mRotZ = file->mEntries[i].mRotZ;
        i++;
    } while (i < file->mCount);
}

void GimmickManager::func_ov007_0222e75c(int a, int b) {
    char buf[0x804];
    char path[0x40];
    sprintf(path, sSetDataFormat, a, b);
    ChangeDir("/Game/Gimmick/SetData");
    if (func_ov007_02271a00((GimmickSetFile *)buf, path)) {
        func_ov007_0222e290((GimmickSetFile *)buf);
    }
    ChangeDir("/");
}

int GimmickManager::func_ov007_0222e7f4(Gimmick *gimmick) {
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

void GimmickManager::func_ov007_0222e860(int id) {
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
