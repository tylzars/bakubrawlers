#include "System/FrameTask.hpp"
#include <nitro/os.h>

struct CriMemConfig {
    void *(*mAlloc)(u32);
    void (*mFree)(void *);
};

extern "C" void CriGlobInitWithThreads(CriMemConfig *config, int a, const void *b, void (*c)(void *));
extern "C" void *CriHdlAlloc(CriMemConfig *config, int a, int b);
extern "C" void func_0205214c(void *handle, int a, int b);
extern "C" void CriHdlSetVolume(void *handle, int a);
extern "C" void CriHdlDealloc(void *handle);
extern "C" void CriGlobExit();
extern "C" int CriHdlGetState(void *handle);
extern "C" void CriGlobUpdate();
extern "C" void CriHdlStart(void *handle, const char *path);
extern "C" void func_02051fb8(void *handle, int a);
extern "C" void CriHdlStop(void *handle);
extern "C" void CriHdlSetVolumeFloat(void *handle, float volume);
extern "C" void func_02051a88();
extern "C" void SleepUntilVBlank();
extern "C" void func_02051a5c();
extern const u8 data_0209e538[];

TaskBase::TaskBase() {
    mHandle0     = 0;
    mHandle1     = 0;
    mStatus0      = 0;
    mStatus1      = 0;
    mInitialized = 0;
}

// mwcc also emits a deleting destructor (D0) for this class, which the original does not have here.
TaskBase::~TaskBase() {}

static void *CriAlloc(u32 size);
static void CriFree(void *ptr);
static void func_02052518(void *unused);

void TaskBase::Init() {
    CriMemConfig config;
    if (mInitialized != 0) {
        return;
    }
    config.mAlloc = CriAlloc;
    config.mFree  = CriFree;
    CriGlobInitWithThreads(&config, 3, data_0209e538, func_02052518);
    mHandle0 = CriHdlAlloc(&config, 2, 0);
    mHandle1 = CriHdlAlloc(&config, 1, 0);
    func_0205214c(mHandle1, 0, 0);
    CriHdlSetVolume(mHandle0, 0);
    CriHdlSetVolume(mHandle1, 0);
    mInitialized = 1;
}

void TaskBase::Release() {
    if (mInitialized == 0) {
        return;
    }
    CriHdlDealloc(mHandle0);
    CriHdlDealloc(mHandle1);
    CriGlobExit();
    mInitialized = 0;
}

void TaskBase::UpdateHandles() {
    if (mInitialized != 0) {
        mStatus0 = CriHdlGetState(mHandle0);
        mStatus1 = CriHdlGetState(mHandle1);
    }
    CriGlobUpdate();
}

void TaskBase::Start0(const char *path) {
    CriHdlStart(mHandle0, path);
}

void TaskBase::Start1(const char *path) {
    CriHdlStart(mHandle1, path);
}

void TaskBase::func_02052480() {
    func_02051fb8(mHandle1, 1);
}

void TaskBase::func_02052494() {
    func_02051fb8(mHandle1, 0);
}

void TaskBase::Stop0() {
    CriHdlStop(mHandle0);
}

void TaskBase::Stop1() {
    CriHdlStop(mHandle1);
}

void TaskBase::SetVolume0(float volume) {
    CriHdlSetVolumeFloat(mHandle0, volume);
}

void TaskBase::SetVolume1(float volume) {
    CriHdlSetVolumeFloat(mHandle1, volume);
}

static void *CriAlloc(u32 size) {
    return OS_Alloc(size);
}

static void CriFree(void *ptr) {
    OS_Free(ptr);
}

static void func_02052518(void *unused) {}

void TaskBase::Wait() {
    if (mInitialized != 0) {
        func_02051a88();
    } else {
        SleepUntilVBlank();
    }
}

extern "C" void func_0205253c() {
    func_02051a5c();
}
