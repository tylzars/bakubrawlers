#include <nitro/private/os_types.h>

extern OSMutex *OSi_RemoveMutexLinkFromQueue(OSMutexQueue *queue);
extern void HeldMutexAppend(OSThread *thread, OSMutex *mutex);
extern void HeldMutexUnlink(OSThread *thread, OSMutex *mutex);

static inline OSThread *OS_GetCurrentThread(void) {
    return OSi_ThreadInfo.current;
}

u32 func_02005a60(void) {
    return 0;
}

static u32 consoleTypeCache;   // data_020a4f38

u32 func_02005a68(void) {
    consoleTypeCache = 0x82000001;
    return 0x82000001;
}

void OS_InitMessageQueue(OSMessageQueue *mq, void **msgArray, s32 msgCount) {
    mq->queueSend.head = mq->queueSend.tail = NULL;
    mq->queueReceive.head = mq->queueReceive.tail = NULL;
    mq->msgArray = msgArray;
    mq->msgCount = msgCount;
    mq->firstIndex = 0;
    mq->usedCount = 0;
}

BOOL OS_SendMessage(OSMessageQueue *mq, void *msg, s32 flags) {
    OSIntrMode e = OS_DisableInterrupts();
    while (mq->msgCount <= mq->usedCount) {
        if (!(flags & OS_MESSAGE_BLOCK)) {
            OS_RestoreInterrupts(e);
            return FALSE;
        }
        OS_SleepThread(&mq->queueSend);
    }
    mq->msgArray[(mq->firstIndex + mq->usedCount) % mq->msgCount] = msg;
    mq->usedCount++;
    OS_WakeupThread(&mq->queueReceive);
    OS_RestoreInterrupts(e);
    return TRUE;
}

BOOL OS_ReceiveMessage(OSMessageQueue *mq, void **msg, s32 flags) {
    OSIntrMode e = OS_DisableInterrupts();
    while (mq->usedCount == 0) {
        if (!(flags & OS_MESSAGE_BLOCK)) {
            OS_RestoreInterrupts(e);
            return FALSE;
        }
        OS_SleepThread(&mq->queueReceive);
    }
    if (msg != NULL) {
        *msg = mq->msgArray[mq->firstIndex];
    }
    mq->firstIndex = (mq->firstIndex + 1) % mq->msgCount;
    mq->usedCount--;
    OS_WakeupThread(&mq->queueSend);
    OS_RestoreInterrupts(e);
    return TRUE;
}

BOOL OS_JamMessage(OSMessageQueue *mq, void *msg, s32 flags) {
    OSIntrMode e = OS_DisableInterrupts();
    while (mq->msgCount <= mq->usedCount) {
        if (!(flags & OS_MESSAGE_BLOCK)) {
            OS_RestoreInterrupts(e);
            return FALSE;
        }
        OS_SleepThread(&mq->queueSend);
    }
    mq->firstIndex = (mq->firstIndex + mq->msgCount - 1) % mq->msgCount;
    mq->msgArray[mq->firstIndex] = msg;
    mq->usedCount++;
    OS_WakeupThread(&mq->queueReceive);
    OS_RestoreInterrupts(e);
    return TRUE;
}

void OS_InitMutex(OSMutex *mutex) {
    mutex->queue.head = mutex->queue.tail = NULL;
    mutex->thread = NULL;
    mutex->count = 0;
}

void OS_LockMutex(OSMutex *mutex) {
    OSIntrMode e = OS_DisableInterrupts();
    OSThread *cur = OS_GetCurrentThread();
    for (;;) {
        if (mutex->thread == NULL) {
            mutex->thread = cur;
            mutex->count++;
            HeldMutexAppend(cur, mutex);
            break;
        }
        if (mutex->thread == cur) {
            mutex->count++;
            break;
        }
        cur->mutex = mutex;
        OS_SleepThread(&mutex->queue);
        cur->mutex = NULL;
    }
    OS_RestoreInterrupts(e);
}

void OS_UnlockMutex(OSMutex *mutex) {
    OSIntrMode e = OS_DisableInterrupts();
    OSThread *cur = OS_GetCurrentThread();
    if (mutex->thread == cur) {
        if (--mutex->count == 0) {
            HeldMutexUnlink(cur, mutex);
            mutex->thread = NULL;
            OS_WakeupThread(&mutex->queue);
        }
    }
    OS_RestoreInterrupts(e);
}

void OSi_UnlockAllMutex(OSThread *thread) {
    if (thread->mutexQueue.head != NULL) {
        do {
            OSMutex *mutex = OSi_RemoveMutexLinkFromQueue(&thread->mutexQueue);
            mutex->count = 0;
            mutex->thread = NULL;
            OS_WakeupThread(&mutex->queue);
        } while (thread->mutexQueue.head != NULL);
    }
}

void HeldMutexAppend(OSThread *thread, OSMutex *mutex) {
    OSMutex *tail = thread->mutexQueue.tail;
    if (tail == NULL) {
        thread->mutexQueue.head = mutex;
    } else {
        tail->link.next = mutex;
    }
    mutex->link.prev = tail;
    mutex->link.next = NULL;
    thread->mutexQueue.tail = mutex;
}

void HeldMutexUnlink(OSThread *thread, OSMutex *mutex) {
    OSMutex *next = mutex->link.next;
    OSMutex *prev = mutex->link.prev;
    if (next == NULL) {
        thread->mutexQueue.tail = prev;
    } else {
        next->link.prev = prev;
    }
    if (prev == NULL) {
        thread->mutexQueue.head = next;
    } else {
        prev->link.next = next;
    }
}
