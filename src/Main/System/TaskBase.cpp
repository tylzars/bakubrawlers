#include "System/FrameTask.hpp"
#include <nitro/os.h>

struct UnkStruct_CriConfig {
    void *(*mAlloc)(u32);
    void (*mFree)(void *);
};

extern "C" void func_02051804(UnkStruct_CriConfig *config, int a, const void *b, void (*c)(void *));
extern "C" void *func_02051ab0(UnkStruct_CriConfig *config, int a, int b);
extern "C" void func_0205214c(void *handle, int a, int b);
extern "C" void func_020520e8(void *handle, int a);
extern "C" void func_02051d7c(void *handle);
extern "C" void func_020519bc();
extern "C" int func_02051e40(void *handle);
extern "C" void func_02051a3c();
extern "C" void func_02051ed4(void *handle, const char *path);
extern "C" void func_02051fb8(void *handle, int a);
extern "C" void func_02051f5c(void *handle);
extern "C" void func_0205201c(void *handle, float volume);
extern "C" void func_02051a88();
extern "C" void func_02007230();
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

static void *func_020524e8(u32 size);
static void func_02052500(void *ptr);
static void func_02052518(void *unused);

void TaskBase::Init() {
    UnkStruct_CriConfig config;
    if (mInitialized != 0) {
        return;
    }
    config.mAlloc = func_020524e8;
    config.mFree  = func_02052500;
    func_02051804(&config, 3, data_0209e538, func_02052518);
    mHandle0 = func_02051ab0(&config, 2, 0);
    mHandle1 = func_02051ab0(&config, 1, 0);
    func_0205214c(mHandle1, 0, 0);
    func_020520e8(mHandle0, 0);
    func_020520e8(mHandle1, 0);
    mInitialized = 1;
}

void TaskBase::Release() {
    if (mInitialized == 0) {
        return;
    }
    func_02051d7c(mHandle0);
    func_02051d7c(mHandle1);
    func_020519bc();
    mInitialized = 0;
}

void TaskBase::UpdateHandles() {
    if (mInitialized != 0) {
        mStatus0 = func_02051e40(mHandle0);
        mStatus1 = func_02051e40(mHandle1);
    }
    func_02051a3c();
}

void TaskBase::Start0(const char *path) {
    func_02051ed4(mHandle0, path);
}

void TaskBase::Start1(const char *path) {
    func_02051ed4(mHandle1, path);
}

void TaskBase::func_02052480() {
    func_02051fb8(mHandle1, 1);
}

void TaskBase::func_02052494() {
    func_02051fb8(mHandle1, 0);
}

void TaskBase::Stop0() {
    func_02051f5c(mHandle0);
}

void TaskBase::Stop1() {
    func_02051f5c(mHandle1);
}

void TaskBase::SetVolume0(float volume) {
    func_0205201c(mHandle0, volume);
}

void TaskBase::SetVolume1(float volume) {
    func_0205201c(mHandle1, volume);
}

static void *func_020524e8(u32 size) {
    return OS_Alloc(size);
}

static void func_02052500(void *ptr) {
    OS_Free(ptr);
}

static void func_02052518(void *unused) {}

void TaskBase::Wait() {
    if (mInitialized != 0) {
        func_02051a88();
    } else {
        func_02007230();
    }
}

extern "C" void func_0205253c() {
    func_02051a5c();
}
