#include <nitro/private/os_types.h>
#include <nitro/mi.h>

extern OSIntrMode OS_EnableInterrupts(void);   // 0x020070f8
extern u32 OS_GetProcMode(void);               // 0x02007170
extern void OS_Halt(void);                     // 0x02007530
extern void OS_Terminate(void);                // 0x02007520
extern u32 OS_SaveContext(OSContext *ctx);     // 0x020059d0 (asm)
extern void OS_LoadContext(OSContext *ctx);    // 0x02005a1c (asm)
extern void OS_CreateAlarm(OSAlarm *alarm);    // 0x02006d10
extern void OS_SetAlarm(OSAlarm *alarm, u64 tick, void (*handler)(void *), void *arg); // 0x02006e4c
extern void OSi_UnlockAllMutex(OSThread *thread); // 0x02005d74
extern void OS_SleepThread(OSThreadQueue *queue);
extern void OS_WakeupThreadDirect(OSThread *thread);
extern u32 OS_EnableScheduler(void);
extern void *OS_SetSwitchThreadCallback(void (*cb)(OSThread *, OSThread *));
extern void OSi_ExitThread(OSThread *thread, void *arg);
extern u32 OS_DisableScheduler(void);
extern void OS_RescheduleThread(void);
extern void OS_InitContext(OSContext *context, u32 newpc, u32 newsp);
extern void OS_SetThreadDestructor(OSThread *thread, void (*dtor)(void *));
extern void OS_ExitThread(void);
extern void OSi_IdleThreadProc(void);
extern void OSi_SleepAlarmCallback(void *arg);
extern OSThread *FirstReadyThread(void);
extern void ExitThreadRunDestructor(void *arg);
extern void OSi_ExitThread_Destroy(void);
extern void OSi_RescheduleThread(void);
extern OSThread *OSi_RemoveLinkFromQueue(OSThreadQueue *queue);
extern OSThread *OSi_RemoveSpecifiedLinkFromQueue(OSThreadQueue *queue, OSThread *thread);
extern void OSi_InsertLinkToQueue(OSThreadQueue *queue, OSThread *thread);
extern void OSi_InsertThreadToList(OSThread *thread);
extern void OSi_RemoveThreadFromList(OSThread *thread);
extern u32 OSi_GetUnusedThreadId(void);
extern void OS_WakeupThread(OSThreadQueue *queue);
extern void OS_CreateThread(OSThread *thread, void (*func)(void *), void *arg, void *stack, u32 stackSize, u32 prio);

#define OS_IRQ_MODE 0x12
#define OSi_IRQ_STACK_BOTTOM (HW_DTCM + 0x3f80 - (u32) SDK_IRQ_STACKSIZE)

u32 OSi_GetUnusedThreadId(void) {
    return ++threadSys.lastThreadId;
}

void OSi_InsertLinkToQueue(OSThreadQueue *queue, OSThread *thread) {
    OSThread *t = queue->head;
    OSThread *prev;
    while (t != NULL && t->priority <= thread->priority) {
        if (t == thread) {
            return;
        }
        t = t->link.next;
    }
    if (t == NULL) {
        prev = queue->tail;
        if (prev == NULL) {
            queue->head = thread;
        } else {
            prev->link.next = thread;
        }
        thread->link.prev = prev;
        thread->link.next = NULL;
        queue->tail = thread;
    } else {
        prev = t->link.prev;
        if (prev == NULL) {
            queue->head = thread;
        } else {
            prev->link.next = thread;
        }
        thread->link.prev = prev;
        thread->link.next = t;
        t->link.prev = thread;
    }
}

OSThread *OSi_RemoveLinkFromQueue(OSThreadQueue *queue) {
    OSThread *t = queue->head;
    if (t != NULL) {
        OSThread *next = t->link.next;
        queue->head = next;
        if (next != NULL) {
            next->link.prev = NULL;
        } else {
            queue->tail = NULL;
            t->queue = NULL;
        }
    }
    return t;
}

OSThread *OSi_RemoveSpecifiedLinkFromQueue(OSThreadQueue *queue, OSThread *thread) {
    OSThread *t;
    OSThread *next;
    OSThread *prev;
    prev = queue->head;
    t = prev;
    while (t != NULL) {
        next = t->link.next;
        if (t == thread) {
            if (prev == t) {
                prev = t->link.prev;
                queue->head = next;
            } else {
                prev = t->link.prev;
                prev->link.next = next;
            }
            if (queue->tail == t) {
                queue->tail = prev;
            } else {
                next->link.prev = prev;
            }
            break;
        }
        t = next;
    }
    return t;
}

OSMutex *OSi_RemoveMutexLinkFromQueue(OSMutexQueue *queue) {
    OSMutex *m = queue->head;
    if (m != NULL) {
        OSMutex *next = m->link.next;
        queue->head = next;
        if (next != NULL) {
            next->link.prev = NULL;
        } else {
            queue->tail = NULL;
        }
    }
    return m;
}

void OSi_InsertThreadToList(OSThread *thread) {
    OSThread *t;
    OSThread *prev = NULL;
    OSThread *head = threadSys.ti.list;
    t = head;
    while (t != NULL && t->priority < thread->priority) {
        prev = t;
        t = t->next;
    }
    if (prev == NULL) {
        thread->next = head;
        threadSys.ti.list = thread;
    } else {
        thread->next = prev->next;
        prev->next = thread;
    }
}

void OSi_RemoveThreadFromList(OSThread *thread) {
    OSThread *prev = NULL;
    OSThread *t = threadSys.ti.list;
    while (t != NULL && t != thread) {
        prev = t;
        t = t->next;
    }
    if (prev == NULL) {
        threadSys.ti.list = thread->next;
    } else {
        prev->next = thread->next;
    }
}

void OSi_RescheduleThread(void) {
    OSThreadInfo *ti;
    OSThread *next;
    OSThread *cur;
    if (threadSys.schedulerLock != 0) {
        return;
    }
    ti = &threadSys.ti;
    if (threadSys.ti.irqDepth != 0 || OS_GetProcMode() == OS_IRQ_MODE) {
        ti->isNeedRescheduling = TRUE;
        return;
    }
    cur = *threadSys.pCurrent;
    next = FirstReadyThread();
    if (cur == next || next == NULL) {
        return;
    }
    if (cur->state != OS_THREAD_STATE_TERMINATED) {
        if (OS_SaveContext(&cur->context) != 0) {
            return;
        }
    }
    if (threadSys.switchHook != NULL) {
        threadSys.switchHook(cur, next);
    }
    if (ti->switchCallback != NULL) {
        ti->switchCallback(cur, next);
    }
    threadSys.ti.current = next;
    OS_LoadContext(&next->context);
}

void OS_InitThread(void) {
    OSThread *mainThread;
    u32 stackTop;
    if (threadSys.initialized != 0) {
        return;
    }
    mainThread = &threadSys.ti.mainThread;
    threadSys.initialized = TRUE;
    threadSys.pCurrent = &threadSys.ti.current;
    threadSys.ti.mainThread.priority = 0x10;
    threadSys.ti.mainThread.id = 0;
    threadSys.ti.mainThread.state = OS_THREAD_STATE_READY;
    threadSys.ti.mainThread.next = NULL;
    threadSys.ti.mainThread.profiler = NULL;
    threadSys.ti.list = mainThread;
    threadSys.ti.current = mainThread;
    if ((s32) SDK_SYS_STACKSIZE > 0) {
        stackTop = OSi_IRQ_STACK_BOTTOM;
    } else {
        stackTop = 0x27e0080;
    }
    threadSys.ti.mainThread.stackTop = stackTop - (u32) SDK_SYS_STACKSIZE;
    threadSys.ti.mainThread.stackBottom = OSi_IRQ_STACK_BOTTOM;
    threadSys.ti.mainThread.stackWarningOffset = 0;
    *(u32 *) (threadSys.ti.mainThread.stackBottom - 4) = 0xfddb597d;
    *(u32 *) threadSys.ti.mainThread.stackTop = 0x7bf9dd5b;
    threadSys.ti.mainThread.joinQueue.tail = NULL;
    threadSys.ti.mainThread.joinQueue.head = NULL;
    threadSys.ti.isNeedRescheduling = FALSE;
    threadSys.ti.irqDepth = 0;
    *(void **) 0x27fffa0 = &threadSys.ti.isNeedRescheduling;
    OS_SetSwitchThreadCallback(NULL);
    OS_CreateThread(&threadSys.ti.idleThread, (void (*)(void *)) OSi_IdleThreadProc, NULL,
                    threadSys.ti.idleStack + sizeof(threadSys.ti.idleStack), sizeof(threadSys.ti.idleStack), 0x1f);
    threadSys.ti.idleThread.priority = 0x20;
    threadSys.ti.idleThread.state = OS_THREAD_STATE_READY;
}

void OS_CreateThread(OSThread *thread, void (*func)(void *), void *arg, void *stack, u32 stackSize, u32 prio) {
    OSIntrMode e = OS_DisableInterrupts();
    u32 stackTop;
    u32 id = OSi_GetUnusedThreadId();
    thread->priority = prio;
    thread->id = id;
    thread->state = OS_THREAD_STATE_WAITING;
    thread->profiler = NULL;
    OSi_InsertThreadToList(thread);
    thread->stackBottom = (u32) stack;
    stackTop = (u32) stack - stackSize;
    thread->stackTop = stackTop;
    thread->stackWarningOffset = 0;
    *(u32 *) (thread->stackBottom - 4) = 0xfddb597d;
    *(u32 *) thread->stackTop = 0x7bf9dd5b;
    thread->joinQueue.tail = NULL;
    thread->joinQueue.head = NULL;
    OS_InitContext(&thread->context, (u32) func, (u32) stack - 4);
    thread->context.r[0] = (u32) arg;
    thread->context.lr = (u32) OS_ExitThread;
    MI_CpuClear32((void *) (stackTop + 4), stackSize - 8);
    thread->mutex = NULL;
    thread->mutexQueue.head = NULL;
    thread->mutexQueue.tail = NULL;
    OS_SetThreadDestructor(thread, NULL);
    thread->queue = NULL;
    thread->link.next = NULL;
    thread->link.prev = NULL;
    MI_CpuClear32(&thread->specific, sizeof(thread->specific));
    thread->alarmForSleep = NULL;
    OS_RestoreInterrupts(e);
}

void OS_ExitThread(void) {
    OS_DisableInterrupts();
    OSi_ExitThread(threadSys.ti.current, NULL);
}

void OSi_ExitThread(OSThread *thread, void *arg) {
    if (threadSys.exitStack != 0) {
        OS_InitContext(&thread->context, (u32) ExitThreadRunDestructor, threadSys.exitStack);
        thread->context.r[0] = (u32) arg;
        thread->context.cpsr |= 0x80;
        thread->state = OS_THREAD_STATE_READY;
        OS_LoadContext(&thread->context);
    } else {
        ExitThreadRunDestructor(arg);
    }
}

void ExitThreadRunDestructor(void *arg) {
    OSThread *cur = *threadSys.pCurrent;
    void (*dtor)(void *) = cur->destructor;
    if (dtor != NULL) {
        cur->destructor = NULL;
        dtor(arg);
        OS_DisableInterrupts();
    }
    OSi_ExitThread_Destroy();
}

void OSi_ExitThread_Destroy(void) {
    OSThread *cur = *threadSys.pCurrent;
    OS_DisableScheduler();
    OSi_UnlockAllMutex(cur);
    if (cur->queue != NULL) {
        OSi_RemoveSpecifiedLinkFromQueue(cur->queue, cur);
    }
    OSi_RemoveThreadFromList(cur);
    cur->state = OS_THREAD_STATE_TERMINATED;
    OS_WakeupThread(&cur->joinQueue);
    OS_EnableScheduler();
    OS_RescheduleThread();
    OS_Terminate();
}

void OS_SleepThread(OSThreadQueue *queue) {
    OSIntrMode e = OS_DisableInterrupts();
    OSThread *cur = *threadSys.pCurrent;
    if (queue != NULL) {
        cur->queue = queue;
        OSi_InsertLinkToQueue(queue, cur);
    }
    cur->state = OS_THREAD_STATE_WAITING;
    OSi_RescheduleThread();
    OS_RestoreInterrupts(e);
}

void OS_WakeupThread(OSThreadQueue *queue) {
    OSIntrMode e = OS_DisableInterrupts();
    if (queue->head != NULL) {
        while (queue->head != NULL) {
            OSThread *t = OSi_RemoveLinkFromQueue(queue);
            t->state = OS_THREAD_STATE_READY;
            t->queue = NULL;
            t->link.next = NULL;
            t->link.prev = NULL;
        }
        queue->tail = NULL;
        queue->head = NULL;
        OSi_RescheduleThread();
    }
    OS_RestoreInterrupts(e);
}

void OS_WakeupThreadDirect(OSThread *thread) {
    OSIntrMode e = OS_DisableInterrupts();
    thread->state = OS_THREAD_STATE_READY;
    OSi_RescheduleThread();
    OS_RestoreInterrupts(e);
}

OSThread *FirstReadyThread(void) {
    OSThread *t = threadSys.ti.list;
    while (t != NULL && t->state != OS_THREAD_STATE_READY) {
        t = t->next;
    }
    return t;
}

void OS_RescheduleThread(void) {
    OSIntrMode e = OS_DisableInterrupts();
    OSi_RescheduleThread();
    OS_RestoreInterrupts(e);
}

BOOL OS_SetThreadPriority(OSThread *thread, u32 prio) {
    OSThread *t = threadSys.ti.list;
    OSThread *prev = NULL;
    OSIntrMode e = OS_DisableInterrupts();
    while (t != NULL && t != thread) {
        prev = t;
        t = t->next;
    }
    if (t == NULL || t == &threadSys.ti.idleThread) {
        OS_RestoreInterrupts(e);
        return FALSE;
    }
    if (t->priority != prio) {
        if (prev == NULL) {
            threadSys.ti.list = thread->next;
        } else {
            prev->next = thread->next;
        }
        thread->priority = prio;
        OSi_InsertThreadToList(thread);
        OSi_RescheduleThread();
    }
    OS_RestoreInterrupts(e);
    return TRUE;
}

void OS_Sleep(u32 msec) {
    OSAlarm alarm;
    OSThread *cur;
    OSIntrMode e;
    OS_CreateAlarm(&alarm);
    cur = *threadSys.pCurrent;
    e = OS_DisableInterrupts();
    cur->alarmForSleep = &alarm;
    OS_SetAlarm(&alarm, ((u64) msec * 0x82ea) / 64, OSi_SleepAlarmCallback, &cur);
    while (cur != NULL) {
        OS_SleepThread(NULL);
    }
    OS_RestoreInterrupts(e);
}

void OSi_SleepAlarmCallback(void *arg) {
    OSThread **pt = (OSThread **) arg;
    OSThread *t = *pt;
    *pt = NULL;
    t->alarmForSleep = NULL;
    OS_WakeupThreadDirect(t);
}



void *OS_SetSwitchThreadCallback(void (*cb)(OSThread *, OSThread *)) {
    OSIntrMode e = OS_DisableInterrupts();
    void *old = (void *) threadSys.ti.switchCallback;
    threadSys.ti.switchCallback = cb;
    OS_RestoreInterrupts(e);
    return old;
}

void OSi_IdleThreadProc(void) {
    OS_EnableInterrupts();
    while (1) {
        OS_Halt();
    }
}

u32 OS_DisableScheduler(void) {
    OSIntrMode e = OS_DisableInterrupts();
    u32 cnt = threadSys.schedulerLock;
    u32 old;
    if (cnt < 0xffffffff) {
        old = cnt;
        threadSys.schedulerLock = cnt + 1;
    }
    OS_RestoreInterrupts(e);
    return old;
}

u32 OS_EnableScheduler(void) {
    OSIntrMode e = OS_DisableInterrupts();
    u32 old = 0;
    u32 cnt = threadSys.schedulerLock;
    if (cnt != 0) {
        old = cnt;
        threadSys.schedulerLock = cnt - 1;
    }
    OS_RestoreInterrupts(e);
    return old;
}

void OS_SetThreadDestructor(OSThread *thread, void (*dtor)(void *)) {
    thread->destructor = dtor;
}

void OS_InitContext(OSContext *context, u32 newpc, u32 newsp) {
    u32 sp;
    context->pc_plus4 = newpc + 4;
    context->sp_svc = newsp;
    sp = newsp - 0x40;
    context->sp = (sp & 4) ? sp - 4 : sp;
    context->cpsr = (newpc & 1) ? 0x3f : 0x1f;
    context->r[0] = 0;
    context->r[1] = 0;
    context->r[2] = 0;
    context->r[3] = 0;
    context->r[4] = 0;
    context->r[5] = 0;
    context->r[6] = 0;
    context->r[7] = 0;
    context->r[8] = 0;
    context->r[9] = 0;
    context->r[10] = 0;
    context->r[11] = 0;
    context->r[12] = 0;
    context->lr = 0;
}
