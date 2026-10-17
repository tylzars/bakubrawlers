#pragma once

#include "types.h"

typedef void (*FrameCallback)(void *arg);

// Queues a callback to run at the start of the next frame update.
void QueueFrameCallback(FrameCallback callback, void *arg);
