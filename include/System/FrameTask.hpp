#pragma once

#include "global.h"
#include "types.h"

// Base of objects that own a pair of criSs handles (+04: first channel, +08: second channel), created lazily
// by Init. It has no virtual destructor: the three virtual slots are Init and the two channel volume setters.
class TaskBase {
public:
    /* 00 (vtable) */
    /* 04 */ void *mHandle0;
    /* 08 */ void *mHandle1;
    /* 0c */ u8 mUnk_0c;
    /* 0d */ u8 mUnk_0d;
    /* 0e */ PAD(0x0e, 0x10);
    /* 10 */ s32 mInitialized;
    /* 14 */

    TaskBase();
    ~TaskBase();

    /* 00 */ virtual void Init();
    /* 04 */ virtual void SetVolume0(float volume);
    /* 08 */ virtual void SetVolume1(float volume);

    void func_0205242c();
    void func_0205251c();
    void func_020523f8();
    void func_02052460(const char *path);
    void func_02052470(const char *path);
    void func_02052480();
    void func_02052494();
    void func_020524a8();
    void func_020524b8();
};

// Runs once per frame from the frame update.
class FrameTask : public TaskBase {
public:
    /* 14 */ PAD(0x14, 0x954);
    /* 954 */ float mVolume0;
    /* 958 */ float mVolume1;
    /* 95c */ float mVolume2;
    /* 960 */

    FrameTask() {}
    ~FrameTask();

    // Replaces TaskBase::Init in the vtable: resets the volume scales.
    virtual void Init();

    static FrameTask &Get() {
        static FrameTask instance;
        return instance;
    }

    void Run();
    void func_0203dbc4();
    void func_0203dc04();
    void func_0203dc10();
    void func_0203dc28();
    void func_0203dc34(u32 unk1, u32 unk2);
    void func_0203dd4c(u32 channel);
    void func_0203dd60(u32 channel);
    void func_0203dd74(u32 channel);
    void func_0203dd90();
    void func_0203ddb0(u32 channel, u32 level);
};
