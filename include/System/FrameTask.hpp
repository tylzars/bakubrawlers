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
    /* 0c */ u8 mStatus0; // CriHdlGetState(mHandle0), stored by UpdateHandles; nothing reads it in this build
    /* 0d */ u8 mStatus1; // the same for mHandle1
    /* 0e */ PAD(0x0e, 0x10);
    /* 10 */ s32 mInitialized;
    /* 14 */

    TaskBase();
    ~TaskBase();

    /* 00 */ virtual void Init();
    /* 04 */ virtual void SetVolume0(float volume);
    /* 08 */ virtual void SetVolume1(float volume);

    void UpdateHandles();
    void Wait();
    // Nothing unless mInitialized; CriHdlDealloc on both handles, CriGlobExit, clears mInitialized; mHandle0 /
    // mHandle1 keep their values.
    void Release();
    void Start0(const char *path);
    void Start1(const char *path);
    void func_02052480();
    void func_02052494();
    void Stop0();
    void Stop1();
};

// TaskBase with three channels: 0 plays on mHandle0, 1 and 2 share mHandle1. GameSystem::Run calls Update every
// frame and UpdateFrame calls Run.
class FrameTask : public TaskBase {
public:
    /* 14 */ PAD(0x14, 0x954);
    /* 954 */ float mVolume0;
    /* 958 */ float mVolume1;
    /* 95c */ float mVolume2;
    /* 960 */ PAD(0x960, 0x96c); // size from the instance's place in Game's .bss
    /* 96c */

    FrameTask() {}
    ~FrameTask();

    // Replaces TaskBase::Init in the vtable: resets the volume scales.
    virtual void Init();

    static FrameTask &Get() {
        static FrameTask instance;
        return instance;
    }

    void Run();
    void OpenHandles(); // TaskBase::Init (once), then volume level 4 on every channel
    void Update();
    void WaitFrame();
    void CloseHandles();
    void StartChannel(u32 unk1, u32 unk2);
    void func_0203dd4c(u32 channel);
    void func_0203dd60(u32 channel);
    void StopChannel(u32 channel); // 0 stops mHandle0; any other channel stops mHandle1
    void StopAllChannels();
    void SetChannelVolume(u32 channel, u32 level); // gVolumeScales[level] (level < 10); channel 1 scales mVolume2, 2 mVolume1
};

BOOL func_0203dd0c(FrameTask *task, u32 channel); // nonzero when the channel's task has finished
