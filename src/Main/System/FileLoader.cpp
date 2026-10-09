#include "Scene/SceneBase.hpp"
#include "Game/UnkStruct_020d43f0.hpp"
#include "System/FileSystem.hpp"
#include "Unknown/UnkStruct_020c17a8.hpp"

#include <nitro/mi.h>
#include <nitro/os.h>

extern u32 func_02004f54();
extern void func_02004fbc(u32 id);
extern "C" void func_0200e658(u32 id);
extern "C" void func_0200e668(u32 id);
extern "C" BOOL func_0200ec04(u32 src, void *dst, u32 len, u32 a, u32 b, u32 c, u32 d, u32 e, u32 f);
extern "C" BOOL func_0200ed00(u32 cmd);
extern "C" BOOL func_0200ee44();
extern "C" int func_0200e60c();
extern "C" void func_01ff96c4(const void *src, void *dst, u32 size);

void UnkStruct_020c17a8::func_02023b80() {
    u16 id = func_02004f54();
    func_0200e658(id);
    func_0200ed00(0xd01);
    func_0200e668(id);
    func_02004fbc(id);
    mBusy = 0;
}

u16 UnkStruct_020c17a8::func_02023bc4(const void *data, u32 size) {
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

// non-matching: 12 words, result/id registers swapped and the original's `add r0, r6, #0` for `mov r0, r6`.
u32 UnkStruct_020c17a8::func_02023c38(u32 unk1) {
    u16 id;
    u32 result = 2;
    u32 magicA;
    u32 magicB;
    id = func_02004f54();
    func_0200e658(id);
    func_0200ec04(unk1 << 13, &magicA, 4, 0, 0, 0, 6, 1, 0);
    BOOL ok = func_0200ec04((unk1 << 13) + 0x1000, &magicB, 4, 0, 0, 0, 6, 1, 0);
    func_0200e668(id);
    func_02004fbc(id);
    if (!ok) {
        result = 1;
    } else if (magicA == 0x19691015) {
        result = 5;
    } else if (magicB == 0x19691015) {
        result = 5;
    } else if (magicA == 0x20071019) {
        result = 4;
    } else if (magicB == 0x20071019) {
        result = 4;
    }
    return result;
}

// non-matching: 17 words, same two causes as func_02023c38.
u32 UnkStruct_020c17a8::func_02023d30(u32 unk1) {
    u32 magic = 0x19691015;
    u32 result = 1;
    u16 id = func_02004f54();
    func_0200e658(id);
    if (func_0200ec04((u32) &magic, (void *) (unk1 << 13), 4, 0, 0, 0, 8, 10, result)) {
        if (func_0200ec04((u32) &magic, (void *) ((unk1 << 13) + 0x1000), 4, 0, 0, 0, 8, 10, result)) {
            result = 4;
        }
    }
    func_0200e668(id);
    func_02004fbc(id);
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

// One save slot copy as stored in the backup memory (0x1000 bytes).
struct UnkStruct_SaveBlock {
    /* 00 */ u32 mMagic0;
    /* 04 */ u32 mMagic1;
    /* 08 */ u16 mCrc;
    /* 0a */ PAD(0x0a, 0x0c);
    /* 0c */ u8 mData[0xff4];
};

// Loads the save data (vtable 0x020a5494).
class UnkScene_020a5494 : public SceneBase {
public:
    virtual ~UnkScene_020a5494() {}
    virtual void Update();
};

// Writes the save data (vtable 0x020a54a8).
class UnkScene_020a54a8 : public SceneBase {
public:
    virtual ~UnkScene_020a54a8() {}
    virtual void Update();
};

void UnkScene_020a54a8::Update() {
    UnkStruct_SaveTask *task = (UnkStruct_SaveTask *) mContext;
    switch (task->mState) {
        case 0: {
            RequestScene(0);
            if (UnkStruct_020c17a8::Get().func_02023c38(0) == 1) {
                RequestScene(1);
                mFinished = 1;
                return;
            }
            task->mBase = 0;
            UnkStruct_020c17a8::Get().mBusy = 1;
            task->mTry   = 0;
            task->mState = 5;
            break;
        }
        case 5: {
            task->mBuffer = OS_AllocFromHeap(OS_ARENA_MAIN, OS_CURRENT_HEAP_HANDLE, 0x1000);
            func_01ff96c4(data_020d43f0, (u8 *) task->mBuffer + 0xc, 0xff4);
            u16 crc = UnkStruct_020c17a8::Get().func_02023bc4((u8 *) task->mBuffer + 0xc, 0xff4);
            ((UnkStruct_SaveBlock *) task->mBuffer)->mCrc    = crc;
            ((UnkStruct_SaveBlock *) task->mBuffer)->mMagic0 = 0x20071019;
            ((UnkStruct_SaveBlock *) task->mBuffer)->mMagic1 = 0x19721021;
            task->mLock = func_02004f54();
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
            task->mResult = func_0200e60c();
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
            func_02004fbc(task->mLock);
            OS_FreeToHeap(OS_ARENA_MAIN, OS_CURRENT_HEAP_HANDLE, task->mBuffer);
            task->mState = 9;
            break;
        case 9:
            UnkStruct_020c17a8::Get().mBusy = 0;
            if (task->mResult == 0) {
                RequestScene(4);
                mFinished = 1;
            } else {
                RequestScene(1);
                mFinished = 1;
            }
            break;
    }
}

void UnkScene_020a5494::Update() {
    UnkStruct_SaveTask *task = (UnkStruct_SaveTask *) mContext;
    switch (task->mState) {
        case 0: {
            u32 status = UnkStruct_020c17a8::Get().func_02023c38(task->mSlot);
            if (status != 4) {
                RequestScene(status);
                mFinished = 1;
                return;
            }
            RequestScene(0);
            UnkStruct_020c17a8::Get().mBusy = 1;
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
            task->mLock = func_02004f54();
            func_0200e658(task->mLock);
            func_0200ec04((u32) (task->mBase + (task->mSlot << 13) + (task->mTry << 12)), task->mBuffer, 0x1000, 0, 0, 1, 6, 1, 0);
            task->mState++;
            break;
        case 3: {
            if (!func_0200ee44()) {
                return;
            }
            task->mResult = func_0200e60c();
            func_0200e668(task->mLock);
            func_02004fbc(task->mLock);
            UnkStruct_SaveBlock *block;
            u16 crc = UnkStruct_020c17a8::Get().func_02023bc4((block = (UnkStruct_SaveBlock *) task->mBuffer)->mData, 0xff4);
            if (block->mCrc == crc && block->mMagic1 == 0x19721021 && block->mMagic0 == 0x20071019) {
                func_01ff96c4(block->mData, data_020d43f0, 0xff4);
                RequestScene(4);
                task->mState++;
            } else if (++task->mTry < 2) {
                task->mState--;
            } else {
                RequestScene(3);
                task->mState++;
            }
            break;
        }
        case 4:
            UnkStruct_020c17a8::Get().mBusy = 0;
            OS_FreeToHeap(OS_ARENA_MAIN, OS_CURRENT_HEAP_HANDLE, task->mBuffer);
            mFinished = 1;
            break;
    }
}
