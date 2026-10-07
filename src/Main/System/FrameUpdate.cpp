#include "System/FrameTask.hpp"

#include <nitro/os.h>

typedef void (*FrameCallback)(void *arg);

#define FRAME_CALLBACK_COUNT 17

// Per-frame callback queue, drained at the start of each frame update.
static u8 sQueueWrite;
static vu8 sFrameCount;
static u8 sQueueRead;
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
