#include "System/SoundManager.hpp"
#include "System/FrameClock.hpp"

// Parameters of the microphone's automatic sampling (same layout as the microphone library's MICAutoParam).
struct MicAutoParam {
    /* 00 */ u32 mType;
    /* 04 */ void *mBuffer;
    /* 08 */ u32 mSize;
    /* 0c */ u32 mRate;
    /* 10 */ u32 mLoop;
    /* 14 */ void *mCallback;
    /* 18 */ void *mArg;
    /* 1c */
};

// Microphone input: starts sampling and turns the level into a blow flag with edge events.
class MicManager {
public:
    /* 00 */ u32 mTime;
    /* 04 */ MicAutoParam mParam;
    /* 20 */ u8 mActive;
    /* 21 */ u8 mHold;
    /* 22 */ u8 mBlowing;
    /* 23 */ u8 mPrevBlowing;
    /* 24 */ u8 mEvents;
    /* 25 */ PAD(0x25, 0x28);

    void func_0202d794();
    void func_0202d7a0();
    void func_0202d8cc();
    void func_0202d94c();
    void func_0202d9ac();
};

extern "C" void MIC_Init(void);
extern "C" void func_0200d2f0(u32 on);
extern "C" void func_0200d338(u32 gain);
extern "C" void MIC_StartAutoSampling(MicAutoParam *param);
extern "C" void MIC_StopAutoSampling(void);
extern "C" s8 *func_0200c9ac(void);
extern u8 data_0220f140[];

void MicManager::func_0202d794() {
    MIC_Init();
}

// non-matching: the zero and the instance pointer swap registers (r0/r1).
void MicManager::func_0202d7a0() {
    if (mActive != 0) {
        return;
    }
    MIC_Init();
    func_0200d2f0(1);
    func_0200d338(1);
    func_0200d338(2);
    mParam.mType     = 2;
    mParam.mBuffer   = data_0220f140;
    mParam.mSize     = 0x400;
    mParam.mRate     = 0x105d;
    mParam.mLoop     = 1;
    mParam.mCallback = NULL;
    mParam.mArg      = NULL;
    MIC_StartAutoSampling(&mParam);
    mActive = 1;
    SoundManager::Get().func_020245c4(0x32);
    mTime        = UnkStruct_020c1884::Get().mFrame;
    mHold = 0;
    mBlowing = mPrevBlowing = 0;
    mEvents = 0;
}

void MicManager::func_0202d8cc() {
    if (mActive != 1) {
        return;
    }
    MIC_StopAutoSampling();
    func_0200d2f0(0);
    mActive = 0;
    SoundManager::Get().func_020245c4(0x32);
    mEvents = 0;
}

void MicManager::func_0202d94c() {
    mTime   = UnkStruct_020c1884::Get().mFrame;
    mEvents = 0;
}

void MicManager::func_0202d9ac() {
    s32 level = 0;
    mEvents   = 0;
    if (mActive == 1) {
        if (UnkStruct_020c1884::Get().mFrame - mTime >= 0xb4) {
            s8 *sample = func_0200c9ac();
            if (sample != NULL) {
                level = *sample;
            }
        }
        mPrevBlowing = mBlowing;
        if (level < 0) {
            level = -level;
        }
        mBlowing = level;
        mBlowing = (u32) (level & 0xff) > 0x32;
        if (mBlowing == 0) {
            if (mHold != 0) {
                mHold--;
                if (mHold != 0) {
                    mBlowing = 1;
                }
            }
        } else {
            mHold = 5;
        }
        mEvents |= (mBlowing & (mBlowing ^ mPrevBlowing)) ? 2 : 0;
        mEvents |= (mPrevBlowing & (mBlowing ^ mPrevBlowing)) ? 4 : 0;
        mEvents |= mBlowing ? 1 : 0;
    } else {
        mBlowing = mPrevBlowing = mHold = level;
    }
}
