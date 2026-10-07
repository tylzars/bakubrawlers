#include "System/FrameTask.hpp"

#include <nitro/os.h>

typedef void (*FrameCallback)(void *arg);

#define FRAME_CALLBACK_COUNT 17
#define FRAME_HOOK_COUNT 4

void func_02001dec(BOOL enable);
void func_02001db8(BOOL enable);

// Per-frame callback queue, drained at the start of each frame update.
static vu8 sFrameCount;
static u8 sQueueWrite;
static u8 sQueueRead;
static void (*sHooks[FRAME_HOOK_COUNT])();
static FrameCallback sCallbacks[FRAME_CALLBACK_COUNT];
static void *sCallbackArgs[FRAME_CALLBACK_COUNT];
static u8 sFrameInterval = 1;

void *func_0202537c(void *ptr) {
    operator delete(ptr);
    return ptr;
}

void SetFrameInterval(u8 frames) {
    sFrameInterval = frames;
}

void UpdateFrame() {
    while (sQueueWrite != sQueueRead) {
        sCallbacks[sQueueRead](sCallbackArgs[sQueueRead]);
        sQueueRead = (sQueueRead + 1) % FRAME_CALLBACK_COUNT;
    }
    FrameTask::Get().Run();
    sFrameCount++;
    if (sFrameCount >= sFrameInterval) {
        sFrameCount = 0;
        OS_SetIrqCheckFlag(OS_IE_V_BLANK);
    }
}

void func_020254bc() {
    for (u8 i = 0; i < FRAME_HOOK_COUNT; i++) {
        if (sHooks[i] != NULL) {
            sHooks[i]();
        }
    }
}

void func_020254f0(FrameCallback callback, void *arg) {
    func_02001dec(FALSE);
    sCallbacks[sQueueWrite]    = callback;
    sCallbackArgs[sQueueWrite] = arg;
    sQueueWrite                = (sQueueWrite + 1) % FRAME_CALLBACK_COUNT;
    func_02001dec(TRUE);
}

void func_0202555c() {
    func_02001dec(FALSE);
    sQueueWrite = 0;
    sQueueRead  = 0;
    func_02001dec(TRUE);
}

void func_02025588(void (*hook)()) {
    func_02001db8(FALSE);
    for (u8 i = 0; i < FRAME_HOOK_COUNT; i++) {
        if (sHooks[i] == hook) {
            sHooks[i] = NULL;
        }
    }
    for (u8 i = 0; i < FRAME_HOOK_COUNT; i++) {
        if (sHooks[i] == NULL) {
            sHooks[i] = hook;
            break;
        }
    }
    func_02001db8(TRUE);
}

void func_020255f8(void (*hook)()) {
    func_02001db8(FALSE);
    if (hook == NULL) {
        for (u8 i = 0; i < FRAME_HOOK_COUNT; i++) {
            sHooks[i] = NULL;
        }
    } else {
        for (u8 i = 0; i < FRAME_HOOK_COUNT; i++) {
            if (sHooks[i] == hook) {
                sHooks[i] = NULL;
            }
        }
    }
    func_02001db8(TRUE);
}
