#include <nitro/private/snd_h.h>

extern void *SNDi_SharedWork;   // 0x020bfc00

// 0x020be0c0-0x020be0e3: separate statics, defined in this order for their original layout in .bss.
static u32 cmdProcCount;   // 0x020be0c4
static u32 cmdReplyCount;   // 0x020be0e0
static SNDCommand *cmdFreeTail;   // 0x020be0d0
static SNDCommand *cmdPendingHead;   // 0x020be0c8
static SNDCommand *cmdFreeList;   // 0x020be0c0
static SNDCommand *cmdPendingTail;   // 0x020be0cc
static int cmdRingRead;   // 0x020be0d4
static int cmdRingWrite;   // 0x020be0d8
static int cmdPendingCount;   // 0x020be0dc

static SNDCommand *cmdRing[9];  // 0x020be0e4
static u32 sharedWork[0xa0] __attribute__((aligned(32)));    // 0x020be120
static SNDCommand cmdPool[256]; // 0x020be3a0

void SND_CommandInit(void) {
    SNDCommand *cmd;
    int i; SNDCommand *n;
    func_02008948();
    cmdFreeList = cmdPool;
    i = 0;
    while (i < 255) {
        n = &cmdPool[i + 1];
        cmdPool[i].next = n;
        i++;
    }
    cmdPool[255].next = NULL;
    cmdFreeTail = &cmdPool[255];
    cmdPendingHead = NULL;
    cmdPendingTail = NULL;
    cmdPendingCount = 0;
    cmdRingRead = 0;
    cmdRingWrite = 0;
    cmdReplyCount = 1;
    cmdProcCount = 0;
    SNDi_SharedWork = sharedWork;
    InitSharedWork(sharedWork);
    cmd = SND_AllocCommand(1);
    if (cmd == NULL) {
        return;
    }
    cmd->id = 0x1d;
    cmd->arg[0] = (u32) SNDi_SharedWork;
    SND_PushCommand(cmd);
    SND_FlushCommand(1);
}

SNDCommand *SND_RecvCommandReply(u32 flags) {
    OSIntrMode e = OS_DisableInterrupts();
    SNDCommand *node;
    SNDCommand *tail;
    if (flags & 1) {
        if (func_02008b48() == cmdProcCount) {
            do {
                OS_RestoreInterrupts(e);
                OS_SpinWait(100);
                e = OS_DisableInterrupts();
            } while (func_02008b48() == cmdProcCount);
        }
    } else {
        if (func_02008b48() == cmdProcCount) {
            OS_RestoreInterrupts(e);
            return NULL;
        }
    }
    node = cmdRing[cmdRingRead];
    cmdRingRead++;
    if (cmdRingRead > 8) {
        cmdRingRead = 0;
    }
    tail = node;
    if (tail->next != NULL) {
        do {
            tail = tail->next;
        } while (tail->next != NULL);
    }
    if (cmdFreeTail != NULL) {
        cmdFreeTail->next = node;
    } else {
        cmdFreeList = node;
    }
    cmdFreeTail = tail;
    cmdPendingCount--;
    cmdProcCount++;
    OS_RestoreInterrupts(e);
    return node;
}

SNDCommand *SND_AllocCommand(u32 flags) {
    SNDCommand *cmd;
    u32 i;
    if (!func_02008a18()) {
        return NULL;
    }
    cmd = PopFreeCommand();
    if (cmd != NULL) {
        return cmd;
    }
    if (!(flags & 1)) {
        return NULL;
    }
    if ((int) func_02008908() > 0) {
        while (SND_RecvCommandReply(0) != NULL) {
        }
        cmd = PopFreeCommand();
        if (cmd != NULL) {
            return cmd;
        }
    } else {
        SND_FlushCommand(1);
    }
    func_020089a8();
    do {
        SND_RecvCommandReply(1);
        cmd = PopFreeCommand();
    } while (cmd == NULL);
    return cmd;
}

void SND_PushCommand(SNDCommand *cmd) {
    OSIntrMode e = OS_DisableInterrupts();
    SNDCommand *t;
    t = cmdPendingTail;   // 0x020be0cc
    if (t == NULL) {
        cmdPendingHead = cmd;
        cmdPendingTail = cmd;
    } else {
        t->next = cmd;
        cmdPendingTail = cmd;
    }
    cmd->next = NULL;
    OS_RestoreInterrupts(e);
}

BOOL SND_FlushCommand(u32 flags) {
    OSIntrMode e = OS_DisableInterrupts();
    if (cmdPendingHead == NULL) {
        OS_RestoreInterrupts(e);
        return TRUE;
    }
    if (cmdPendingCount >= 8) {
        if (!(flags & 1)) {
            OS_RestoreInterrupts(e);
            return FALSE;
        }
        do {
            SND_RecvCommandReply(1);
        } while (cmdPendingCount >= 8);
        if (cmdPendingHead == NULL) {
            OS_RestoreInterrupts(e);
            return TRUE;
        }
    }
    DC_FlushRange(cmdPool, 0x1800);
    if (PXI_SendWordByFifo(7, (u32) cmdPendingHead, FALSE) < 0) {
        if (!(flags & 1)) {
            OS_RestoreInterrupts(e);
            return FALSE;
        }
        while (cmdPendingCount >= 8 || PXI_SendWordByFifo(7, (u32) cmdPendingHead, FALSE) < 0) {
            OS_RestoreInterrupts(e);
            SND_RecvCommandReply(0);
            e = OS_DisableInterrupts();
            DC_FlushRange(cmdPool, 0x1800);
            if (cmdPendingHead == NULL) {
                OS_RestoreInterrupts(e);
                return TRUE;
            }
        }
    }
    {
        cmdRing[cmdRingWrite] = cmdPendingHead;   // 0x020be0c8
        cmdRingWrite++;
        if (cmdRingWrite > 8) {
            cmdRingWrite = 0;
        }
    }
    cmdPendingHead = NULL;
    cmdPendingTail = NULL;
    cmdPendingCount++;
    cmdReplyCount++;
    OS_RestoreInterrupts(e);
    if (flags & 2) {
        func_020089a8();
    }
    return TRUE;
}

void SND_WaitForCommandProc(u32 tag) {
    if (SND_IsFinishedCommandTag(tag)) {
        return;
    }
    while (SND_RecvCommandReply(0) != NULL) {
    }
    if (SND_IsFinishedCommandTag(tag)) {
        return;
    }
    func_020089a8();
    if (SND_IsFinishedCommandTag(tag)) {
        return;
    }
    do {
        SND_RecvCommandReply(1);
    } while (!SND_IsFinishedCommandTag(tag));
}

u32 SND_GetCurrentCommandTag(void) {
    OSIntrMode e = OS_DisableInterrupts();
    u32 tag;
    if (cmdPendingHead == NULL) {
        tag = cmdProcCount;   // 0x020be0c4
    } else {
        tag = cmdReplyCount;   // 0x020be0e0
    }
    OS_RestoreInterrupts(e);
    return tag;
}

BOOL SND_IsFinishedCommandTag(u32 tag) {
    OSIntrMode e = OS_DisableInterrupts();
    if (tag > cmdProcCount) {
        if ((tag - cmdProcCount) < 0x80000000) tag = FALSE; else tag = TRUE;
    } else {
        tag = (cmdProcCount - tag) < 0x80000000 ? TRUE : FALSE;
    }
    OS_RestoreInterrupts(e);
    return tag;
}

u32 SND_CountFreeCommand(void) {
    OSIntrMode e = OS_DisableInterrupts();
    u32 count = 0;
    SNDCommand *c = cmdFreeList;   // 0x020be0c0
    for (; c != NULL; c = c->next) {
        count++;
    }
    OS_RestoreInterrupts(e);
    return count;
}

u32 SND_CountWaitingCommand(void) {
    OSIntrMode e = OS_DisableInterrupts();
    u32 count = 0;
    SNDCommand *c = cmdPendingHead;   // 0x020be0c8
    for (; c != NULL; c = c->next) {
        count++;
    }
    OS_RestoreInterrupts(e);
    return count;
}

u32 func_02008908(void) {
    u32 a = SND_CountFreeCommand();
    return 256 - a - SND_CountWaitingCommand();
}

void SndFifoReceive(int tag, u32 data, BOOL err) {
    OSIntrMode e = OS_DisableInterrupts();
    CallAlarmCallback(data);
    OS_RestoreInterrupts(e);
}

void func_02008948(void) {
    PXI_SetFifoRecvCallback(7, SndFifoReceive);
    if (!func_02008a18()) {
        return;
    }
    if (PXI_IsCallbackReady(7, 1)) {
        return;
    }
    do {
        OS_SpinWait(100);
    } while (!PXI_IsCallbackReady(7, 1));
}

void func_020089a8(void) {
    while (PXI_SendWordByFifo(7, 0, FALSE) < 0) {
    }
}

SNDCommand *PopFreeCommand(void) {
    OSIntrMode e = OS_DisableInterrupts();
    SNDCommand *cmd = cmdFreeList;   // 0x020be0c0
    if (cmd == NULL) {
        OS_RestoreInterrupts(e);
        return NULL;
    }
    {
        SNDCommand *next = cmd->next;
        cmdFreeList = next;
        if (next == NULL) {
            cmdFreeTail = NULL;
        }
    }
    OS_RestoreInterrupts(e);
    return cmd;
}

BOOL func_02008a18(void) {
    u32 v;
    OSIntrMode e;
    if (func_02005a60() == 0) {
        return TRUE;
    }
    e = OS_DisableInterrupts();
    *(vu32 *) 0x4fff200 = 0x10;
    v = *(vu32 *) 0x4fff200;
    OS_RestoreInterrupts(e);
    return v != 0;
}
