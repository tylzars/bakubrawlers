#pragma once

#include "global.h"
#include "types.h"

class TaskBase {
public:
    TaskBase();
    virtual ~TaskBase();
};

// Runs once per frame from the frame update.
class FrameTask : public TaskBase {
public:
    FrameTask() {}
    virtual ~FrameTask();

    static FrameTask &Get() {
        static FrameTask instance;
        return instance;
    }

    void Run();
};
