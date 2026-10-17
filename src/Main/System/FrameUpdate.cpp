#include "System/FrameTask.hpp"

#include <nitro/os.h>

typedef void (*FrameCallback)(void *arg);

#define FRAME_CALLBACK_COUNT 17
#define FRAME_HOOK_COUNT 4

extern "C" void GX_VBlankIntr(BOOL enable);
extern "C" void GX_HBlankIntr(BOOL enable);

// Per-frame callback queue, drained at the start of each frame update.
static vu8 sFrameCount;
static u8 sQueueWrite;
static u8 sQueueRead;
static void (*sHooks[FRAME_HOOK_COUNT])();
static FrameCallback sCallbacks[FRAME_CALLBACK_COUNT];
static void *sCallbackArgs[FRAME_CALLBACK_COUNT];
static u8 sFrameInterval = 1;

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

void CallHBlankHooks() {
    for (u8 i = 0; i < FRAME_HOOK_COUNT; i++) {
        if (sHooks[i] != NULL) {
            sHooks[i]();
        }
    }
}

void QueueFrameCallback(FrameCallback callback, void *arg) {
    GX_VBlankIntr(FALSE);
    sCallbacks[sQueueWrite]    = callback;
    sCallbackArgs[sQueueWrite] = arg;
    sQueueWrite                = (sQueueWrite + 1) % FRAME_CALLBACK_COUNT;
    GX_VBlankIntr(TRUE);
}

void ClearFrameCallbacks() {
    GX_VBlankIntr(FALSE);
    sQueueWrite = 0;
    sQueueRead  = 0;
    GX_VBlankIntr(TRUE);
}

// A hook already in the table is moved to the first free entry; with no free entry nothing is added.
void AddHBlankHook(void (*hook)()) {
    GX_HBlankIntr(FALSE);
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
    GX_HBlankIntr(TRUE);
}

// NULL removes every hook.
void RemoveHBlankHook(void (*hook)()) {
    GX_HBlankIntr(FALSE);
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
    GX_HBlankIntr(TRUE);
}
