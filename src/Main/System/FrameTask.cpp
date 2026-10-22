#include "System/FrameTask.hpp"

#include <stdio.h>

extern "C" void func_0205253c();
extern "C" int CriHdlGetState(void *handle);

struct GameData {
    PAD(0x00, 0x79);
    u8 mLanguage;
};
extern GameData gGameData;

extern const u16 data_020ac92c[];
extern const char *const data_020ac934[];
extern const char *const data_020ac940[];
extern const char gChannelPathFormat[];
extern const float gVolumeScales[];

void FrameTask::Init() {
    mVolume0 = 1.0f;
    mVolume1 = 1.0f;
    mVolume2 = 1.0f;
}

void FrameTask::OpenHandles() {
    TaskBase::Init();
    SetChannelVolume(0, 4);
    SetChannelVolume(2, 4);
    SetChannelVolume(1, 4);
}

void FrameTask::Update() {
    UpdateHandles();
}

void FrameTask::WaitFrame() {
    Wait();
}

void FrameTask::Run() {
    func_0205253c();
}

void FrameTask::CloseHandles() {
    Release();
}

void FrameTask::StartChannel(u32 unk1, u32 unk2) {
    char path[0x80];
    if (unk1 >= 3) {
        return;
    }
    u8 variant = 0;
    if (unk1 == 1) {
        variant = gGameData.mLanguage;
    }
    sprintf(path, gChannelPathFormat, data_020ac934[unk1], unk2 - data_020ac92c[unk1], variant, data_020ac940[unk1]);
    switch (unk1) {
        case 0:
            Start0(path);
            break;
        case 2:
            Start1(path);
            break;
        case 1:
            Start1(path);
            break;
    }
    SetChannelVolume(unk1, 4);
}

BOOL func_0203dd0c(FrameTask *task, u32 channel) {
    if (channel == 0) {
        if (CriHdlGetState(task->mHandle0) == 2) {
            return TRUE;
        }
    } else {
        if (CriHdlGetState(task->mHandle1) == 2) {
            return TRUE;
        }
    }
    return FALSE;
}

void FrameTask::func_0203dd4c(u32 channel) {
    if (channel == 1) {
        func_02052480();
    }
}

void FrameTask::func_0203dd60(u32 channel) {
    if (channel == 1) {
        func_02052494();
    }
}

void FrameTask::StopChannel(u32 channel) {
    if (channel == 0) {
        Stop0();
    } else {
        Stop1();
    }
}

void FrameTask::StopAllChannels() {
    StopChannel(0);
    StopChannel(1);
}

void FrameTask::SetChannelVolume(u32 channel, u32 level) {
    if (level >= 10) {
        return;
    }
    switch (channel) {
        case 0:
            TaskBase::SetVolume0(mVolume0 * gVolumeScales[level]);
            break;
        case 2:
            TaskBase::SetVolume1(mVolume1 * gVolumeScales[level]);
            break;
        case 1:
            TaskBase::SetVolume1(mVolume2 * gVolumeScales[level]);
            break;
    }
}
