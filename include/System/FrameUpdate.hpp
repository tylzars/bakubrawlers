#pragma once

#include "types.h"

typedef void (*FrameCallback)(void *arg);

// Queues a callback to run at the start of the next frame update.
void func_020254f0(FrameCallback callback, void *arg);
