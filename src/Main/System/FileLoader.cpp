#include "Scene/SceneBase.hpp"
#include "Game/SaveData.hpp"
#include "System/FileSystem.hpp"
#include "System/SaveManager.hpp"

#include <nitro/mi.h>
#include <nitro/os.h>

extern "C" u32 OS_GetLockID();
extern "C" void OS_FreeLockID(u32 id);
extern "C" void func_0200e658(u32 id);
extern "C" void func_0200e668(u32 id);
extern "C" BOOL func_0200ec04(u32 src, void *dst, u32 len, u32 a, u32 b, u32 c, u32 d, u32 e, u32 f);
extern "C" BOOL func_0200ed00(u32 cmd);
extern "C" BOOL func_0200ee44();
extern "C" int GetCardResult();
extern "C" void MI_CpuCopy8(const void *src, void *dst, u32 size);

void SaveManager::Init() {
    u16 id = OS_GetLockID();
    func_0200e658(id);
    func_0200ed00(0xd01);
    func_0200e668(id);
    OS_FreeLockID(id);
    mBusy = 0;
}

u16 SaveManager::ComputeCrc(const void *data, u32 size) {
    const u8 *p = (const u8 *) data;
    int crc = 0xffff;
    for (u32 i = 0; i < size; i++) {
        crc ^= *p;
        for (int bit = 0; bit < 8; bit++) {
            if (crc & 1) {
                crc = (u16) ((crc >> 1) ^ 0x8408);
            } else {
                crc = (u16) (crc >> 1);
            }
        }
        p++;
    }
    return crc ^ 0xffff;
}

u32 SaveManager::GetSlotStatus(u32 unk1) {
    u32 id;
    u32 result = SAVE_STATUS_NO_MAGIC;
    u32 magicA;
    u32 magicB;
    u8 *base = 0; // placeholder: the original adds the offsets to a zero base (add rX, rY, #0); its real form is unknown
    id       = (u16) OS_GetLockID();
    func_0200e658(id);
    func_0200ec04((u32) (base + (unk1 << 13)), &magicA, 4, 0, 0, 0, 6, 1, 0);
    BOOL ok = func_0200ec04((u32) (base + (unk1 << 13) + 0x1000), &magicB, 4, 0, 0, 0, 6, 1, 0);
    func_0200e668(id);
    OS_FreeLockID(id);
    if (!ok) {
        result = SAVE_STATUS_ERROR;
    } else if (magicA == 0x19691015) {
        result = SAVE_STATUS_MARKER;
    } else if (magicB == 0x19691015) {
        result = SAVE_STATUS_MARKER;
    } else if (magicA == 0x20071019) {
        result = SAVE_STATUS_OK;
    } else if (magicB == 0x20071019) {
        result = SAVE_STATUS_OK;
    }
    return result;
}

u32 SaveManager::WriteSlotMarker(u32 unk1) {
    u32 magic = 0x19691015;
    u32 id;
    u32 result = 1; // SAVE_STATUS_ERROR, kept a literal: it is also the last argument of both calls below
    u8 *base   = 0; // placeholder, as in GetSlotStatus
    id         = (u16) OS_GetLockID();
    func_0200e658(id);
    if (func_0200ec04((u32) &magic, base + (unk1 << 13), 4, 0, 0, 0, 8, 10, result)) {
        if (func_0200ec04((u32) &magic, base + (unk1 << 13) + 0x1000, 4, 0, 0, 0, 8, 10, result)) {
            result = SAVE_STATUS_OK;
        }
    }
    func_0200e668(id);
    OS_FreeLockID(id);
    return result;
}

// Per-scene work area: the save scenes use the scene context after its SceneInfo as scratch space.
struct UnkStruct_SaveTask {
    /* 00 */ PAD(0x00, 0x14);
    /* 14 */ void *mBuffer;
    /* 18 */ u8 *mBase;
    /* 1c */ int mResult;
    /* 20 */ u16 mLock;
    /* 22 */ u8 mState;
    /* 23 */ u8 mSlot;
    /* 24 */ u8 mTry;
};

// One save slot copy (0x1000 bytes): two magic words, the CRC of mData, then the 0xff4 bytes of save data.
struct UnkStruct_SaveBlock {
    /* 00 */ u32 mMagic0;
    /* 04 */ u32 mMagic1;
    /* 08 */ u16 mCrc;
    /* 0a */ PAD(0x0a, 0x0c);
    /* 0c */ u8 mData[0xff4];
};

// Writes the save data (vtable 0x020a54a8): copy 0, then copy 1 after copy 0 succeeded (no retry). The start check
// tests GetSlotStatus(0), while the writes go to task->mSlot.
class SaveWriter : public SceneBase {
public:
    virtual ~SaveWriter();
    virtual void Update();
};

// Declared second: vtables come out in reverse class order, and 0x020a5494 is the first of the two.
// Loads the save data (vtable 0x020a5494).
class SaveReader : public SceneBase {
public:
    virtual ~SaveReader();
    virtual void Update();
};

void SaveWriter::Update() {
    UnkStruct_SaveTask *task = (UnkStruct_SaveTask *) mContext;
    switch (task->mState) {
        case 0: {
            RequestScene(0);
            if (SaveManager::Get().GetSlotStatus(0) == SAVE_STATUS_ERROR) {
                RequestScene(SAVE_STATUS_ERROR);
                mFinished = 1;
                return;
            }
            task->mBase = 0;
            SaveManager::Get().mBusy = 1;
            task->mTry   = 0;
            task->mState = 5;
            break;
        }
        case 5: {
            task->mBuffer = OS_AllocFromHeap(OS_ARENA_MAIN, OS_CURRENT_HEAP_HANDLE, 0x1000);
            MI_CpuCopy8(gSaveData, (u8 *) task->mBuffer + 0xc, 0xff4);
            u16 crc = SaveManager::Get().ComputeCrc((u8 *) task->mBuffer + 0xc, 0xff4);
            ((UnkStruct_SaveBlock *) task->mBuffer)->mCrc    = crc;
            ((UnkStruct_SaveBlock *) task->mBuffer)->mMagic0 = 0x20071019;
            ((UnkStruct_SaveBlock *) task->mBuffer)->mMagic1 = 0x19721021;
            task->mLock = OS_GetLockID();
            func_0200e658(task->mLock);
            task->mState++;
            break;
        }
        case 6:
            func_0200ec04((u32) task->mBuffer, task->mBase + (task->mSlot << 13) + (task->mTry << 12), 0x1000, 0, 0, 1, 8, 10, 2);
            task->mState++;
            break;
        case 7:
            if (!func_0200ee44()) {
                return;
            }
            task->mResult = GetCardResult();
            if (task->mResult == 0) {
                if (++task->mTry >= 2) {
                    task->mState++;
                } else {
                    task->mState--;
                }
            } else {
                task->mState++;
            }
            break;
        case 8:
            func_0200e668(task->mLock);
            OS_FreeLockID(task->mLock);
            OS_FreeToHeap(OS_ARENA_MAIN, OS_CURRENT_HEAP_HANDLE, task->mBuffer);
            task->mState = 9;
            break;
        case 9:
            SaveManager::Get().mBusy = 0;
            if (task->mResult == 0) {
                RequestScene(SAVE_STATUS_OK);
                mFinished = 1;
            } else {
                RequestScene(SAVE_STATUS_ERROR);
                mFinished = 1;
            }
            break;
    }
}

void SaveReader::Update() {
    UnkStruct_SaveTask *task = (UnkStruct_SaveTask *) mContext;
    switch (task->mState) {
        case 0: {
            u32 status = SaveManager::Get().GetSlotStatus(task->mSlot);
            if (status != SAVE_STATUS_OK) {
                RequestScene(status);
                mFinished = 1;
                return;
            }
            RequestScene(0);
            SaveManager::Get().mBusy = 1;
            task->mBase  = 0;
            task->mTry   = 0;
            task->mState = 1;
            break;
        }
        case 1:
            task->mBuffer = OS_AllocFromHeap(OS_ARENA_MAIN, OS_CURRENT_HEAP_HANDLE, 0x1000);
            task->mState++;
            break;
        case 2:
            task->mLock = OS_GetLockID();
            func_0200e658(task->mLock);
            func_0200ec04((u32) (task->mBase + (task->mSlot << 13) + (task->mTry << 12)), task->mBuffer, 0x1000, 0, 0, 1, 6, 1, 0);
            task->mState++;
            break;
        case 3: {
            if (!func_0200ee44()) {
                return;
            }
            task->mResult = GetCardResult();
            func_0200e668(task->mLock);
            OS_FreeLockID(task->mLock);
            UnkStruct_SaveBlock *block;
            u16 crc = SaveManager::Get().ComputeCrc((block = (UnkStruct_SaveBlock *) task->mBuffer)->mData, 0xff4);
            if (block->mCrc == crc && block->mMagic1 == 0x19721021 && block->mMagic0 == 0x20071019) {
                MI_CpuCopy8(block->mData, gSaveData, 0xff4);
                RequestScene(SAVE_STATUS_OK);
                task->mState++;
            } else if (++task->mTry < 2) {
                task->mState--;
            } else {
                RequestScene(SAVE_STATUS_CORRUPT);
                task->mState++;
            }
            break;
        }
        case 4:
            SaveManager::Get().mBusy = 0;
            OS_FreeToHeap(OS_ARENA_MAIN, OS_CURRENT_HEAP_HANDLE, task->mBuffer);
            mFinished = 1;
            break;
    }
}

SaveWriter::~SaveWriter() {}

SaveReader::~SaveReader() {}
