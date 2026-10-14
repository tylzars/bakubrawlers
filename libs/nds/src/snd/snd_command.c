#include <nitro/private/snd_h.h>

extern void *SNDi_SharedWork;   // 0x020bfc00

// 0x020be0c0
static struct {
    SNDCommand *freeList;       // 0x00
    u32 procCount;              // 0x04
    SNDCommand *pendingHead;    // 0x08
    SNDCommand *pendingTail;    // 0x0c
    SNDCommand *freeTail;       // 0x10
    int ringRead;               // 0x14
    int ringWrite;              // 0x18
    int pendingCount;           // 0x1c
    u32 replyCount;             // 0x20
} cmdState;

static SNDCommand *cmdRing[9];  // 0x020be0e4
static u32 sharedWork[0xa0] __attribute__((aligned(32)));    // 0x020be120
static SNDCommand cmdPool[256]; // 0x020be3a0

void SND_CommandInit(void) {
    SNDCommand *cmd;
    int i; SNDCommand *n;
    func_02008948();
    cmdState.freeList = cmdPool;
    i = 0;
    while (i < 255) {
        n = &cmdPool[i + 1];
        cmdPool[i].next = n;
        i++;
    }
    cmdPool[255].next = NULL;
    cmdState.freeTail = &cmdPool[255];
    cmdState.pendingHead = NULL;
    cmdState.pendingTail = NULL;
    cmdState.pendingCount = 0;
    cmdState.ringRead = 0;
    cmdState.ringWrite = 0;
    cmdState.replyCount = 1;
    cmdState.procCount = 0;
    SNDi_SharedWork = sharedWork;
    func_02008b70(sharedWork);
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
        if (func_02008b48() == cmdState.procCount) {
            do {
                OS_RestoreInterrupts(e);
                OS_SpinWait(100);
                e = OS_DisableInterrupts();
            } while (func_02008b48() == cmdState.procCount);
        }
    } else {
        if (func_02008b48() == cmdState.procCount) {
            OS_RestoreInterrupts(e);
            return NULL;
        }
    }
    {
        int idx = cmdState.ringRead;
        int nextIdx = idx + 1;
        node = cmdRing[idx];
        cmdState.ringRead = nextIdx;
        if (nextIdx > 8) {
            cmdState.ringRead = 0;
        }
    }
    tail = node;
    if (tail->next != NULL) {
        do {
            tail = tail->next;
        } while (tail->next != NULL);
    }
    if (cmdState.freeTail != NULL) {
        cmdState.freeTail->next = node;
    } else {
        cmdState.freeList = node;
    }
    cmdState.freeTail = tail;
    cmdState.pendingCount--;
    cmdState.procCount++;
    OS_RestoreInterrupts(e);
    return node;
}

SNDCommand *SND_AllocCommand(u32 flags) {
    SNDCommand *cmd;
    u32 i;
    if (!func_02008a18()) {
        return NULL;
    }
    cmd = func_020089d0();
    if (cmd != NULL) {
        return cmd;
    }
    if (!(flags & 1)) {
        return NULL;
    }
    if ((int) func_02008908() > 0) {
        while (SND_RecvCommandReply(0) != NULL) {
        }
        cmd = func_020089d0();
        if (cmd != NULL) {
            return cmd;
        }
    } else {
        SND_FlushCommand(1);
    }
    func_020089a8();
    do {
        SND_RecvCommandReply(1);
        cmd = func_020089d0();
    } while (cmd == NULL);
    return cmd;
}

void SND_PushCommand(SNDCommand *cmd) {
    OSIntrMode e = OS_DisableInterrupts();
    SNDCommand *t;
    t = cmdState.pendingTail;
    if (t == NULL) {
        cmdState.pendingHead = cmd;
        cmdState.pendingTail = cmd;
    } else {
        t->next = cmd;
        cmdState.pendingTail = cmd;
    }
    cmd->next = NULL;
    OS_RestoreInterrupts(e);
}

BOOL SND_FlushCommand(u32 flags) {
    OSIntrMode e = OS_DisableInterrupts();
    if (cmdState.pendingHead == NULL) {
        OS_RestoreInterrupts(e);
        return TRUE;
    }
    if (cmdState.pendingCount >= 8) {
        if (!(flags & 1)) {
            OS_RestoreInterrupts(e);
            return FALSE;
        }
        do {
            SND_RecvCommandReply(1);
        } while (cmdState.pendingCount >= 8);
        if (cmdState.pendingHead == NULL) {
            OS_RestoreInterrupts(e);
            return TRUE;
        }
    }
    DC_FlushRange(cmdPool, 0x1800);
    if (PXI_SendWordByFifo(7, (u32) cmdState.pendingHead, FALSE) < 0) {
        if (!(flags & 1)) {
            OS_RestoreInterrupts(e);
            return FALSE;
        }
        while (cmdState.pendingCount >= 8 || PXI_SendWordByFifo(7, (u32) cmdState.pendingHead, FALSE) < 0) {
            OS_RestoreInterrupts(e);
            SND_RecvCommandReply(0);
            e = OS_DisableInterrupts();
            DC_FlushRange(cmdPool, 0x1800);
            if (cmdState.pendingHead == NULL) {
                OS_RestoreInterrupts(e);
                return TRUE;
            }
        }
    }
    {
        int w = cmdState.ringWrite;
        SNDCommand *h = cmdState.pendingHead;
        int nw = w + 1;
        cmdRing[w] = h;
        cmdState.ringWrite = nw;
        if (nw > 8) {
            cmdState.ringWrite = 0;
        }
    }
    cmdState.pendingHead = NULL;
    cmdState.pendingTail = NULL;
    cmdState.pendingCount++;
    cmdState.replyCount++;
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
    if (cmdState.pendingHead == NULL) {
        tag = cmdState.procCount;
    } else {
        tag = cmdState.replyCount;
    }
    OS_RestoreInterrupts(e);
    return tag;
}

BOOL SND_IsFinishedCommandTag(u32 tag) {
    OSIntrMode e = OS_DisableInterrupts();
    if (tag > cmdState.procCount) {
        if ((tag - cmdState.procCount) < 0x80000000) tag = FALSE; else tag = TRUE;
    } else {
        tag = (cmdState.procCount - tag) < 0x80000000 ? TRUE : FALSE;
    }
    OS_RestoreInterrupts(e);
    return tag;
}

u32 SND_CountFreeCommand(void) {
    OSIntrMode e = OS_DisableInterrupts();
    u32 count = 0;
    SNDCommand *c = cmdState.freeList;
    for (; c != NULL; c = c->next) {
        count++;
    }
    OS_RestoreInterrupts(e);
    return count;
}

u32 SND_CountWaitingCommand(void) {
    OSIntrMode e = OS_DisableInterrupts();
    u32 count = 0;
    SNDCommand *c = cmdState.pendingHead;
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

void func_02008924(int tag, u32 data, BOOL err) {
    OSIntrMode e = OS_DisableInterrupts();
    func_02008ad8(data);
    OS_RestoreInterrupts(e);
}

void func_02008948(void) {
    PXI_SetFifoRecvCallback(7, func_02008924);
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

SNDCommand *func_020089d0(void) {
    OSIntrMode e = OS_DisableInterrupts();
    SNDCommand *cmd = cmdState.freeList;
    if (cmd == NULL) {
        OS_RestoreInterrupts(e);
        return NULL;
    }
    {
        SNDCommand *next = cmd->next;
        cmdState.freeList = next;
        if (next == NULL) {
            cmdState.freeTail = NULL;
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
