#include <nitro/private/os_types.h>

typedef u64 OSTick;

extern OSTick OS_GetTick(void);
extern void OSi_EnterTimerCallback(int timerNum, void (*callback)(void *), void *arg);   // 0x02004b1c
extern u32 OS_EnableIrqMask(u32 mask);        // 0x02004b90
extern u32 OS_DisableIrqMask(u32 mask);       // 0x02004bc0
extern void OSi_SetTimerReserved(int timerNum);
extern void OS_Terminate(void);
extern void AlarmTimerEntry(void *arg);   // 0x02006fb0 (asm)
extern void OSi_AlarmHandler(void);
extern void OSi_ArrangeTimer(OSAlarm *alarm);
extern void OSi_InsertAlarm(OSAlarm *alarm, OSTick fire);

// 0x020be02c
static struct {
    u16 initialized;
    OSAlarm *head;
    OSAlarm *tail;
} alarmQueue;

void OSi_ArrangeTimer(OSAlarm *alarm) {
    OSTick tick = OS_GetTick();
    s64 delta;
    u16 timer;
    *(vu16 *) 0x4000106 = 0;
    delta = (s64) (alarm->fire - tick);
    OSi_EnterTimerCallback(1, AlarmTimerEntry, NULL);
    if (delta < 0) {
        timer = 0xfffe;
    } else if (delta < 0x10000) {
        timer = (u16) ~delta;
    } else {
        timer = 0;
    }
    *(vu16 *) 0x4000104 = timer;
    *(vu16 *) 0x4000106 = 0xc1;
    OS_EnableIrqMask(0x10);
}

void OS_InitAlarm(void) {
    if (alarmQueue.initialized) {
        return;
    }
    alarmQueue.initialized = TRUE;
    OSi_SetTimerReserved(1);
    alarmQueue.head = NULL;
    alarmQueue.tail = NULL;
    OS_DisableIrqMask(0x10);
}

void OS_CreateAlarm(OSAlarm *alarm) {
    alarm->handler = NULL;
    alarm->tag = 0;
}

void OSi_InsertAlarm(OSAlarm *alarm, OSTick fire) {
    OSAlarm *next;
    if (alarm->period != 0) {
        OSTick tick = OS_GetTick();
        fire = alarm->start;
        if (fire < tick) {
            fire += alarm->period * ((tick - fire) / alarm->period + 1);
        }
    }
    alarm->fire = fire;
    for (next = alarmQueue.head; next != NULL; next = next->next) {
        if ((s64) (fire - next->fire) < 0) {
            alarm->prev = next->prev;
            next->prev = alarm;
            alarm->next = next;
            if (alarm->prev != NULL) {
                alarm->prev->next = alarm;
                return;
            }
            alarmQueue.head = alarm;
            OSi_ArrangeTimer(alarm);
            return;
        }
    }
    alarm->next = NULL;
    next = alarmQueue.tail;
    alarmQueue.tail = alarm;
    alarm->prev = next;
    if (alarm->prev != NULL) {
        alarm->prev->next = alarm;
        return;
    }
    alarmQueue.tail = alarm;
    alarmQueue.head = alarm;
    OSi_ArrangeTimer(alarm);
}

void OS_SetAlarm(OSAlarm *alarm, OSTick tick, void (*handler)(void *), void *arg) {
    OSIntrMode e;
    if (alarm == NULL || alarm->handler != NULL) {
        OS_Terminate();
    }
    e = OS_DisableInterrupts();
    alarm->period = 0;
    alarm->handler = handler;
    alarm->arg = arg;
    OSi_InsertAlarm(alarm, OS_GetTick() + tick);
    OS_RestoreInterrupts(e);
}

void OS_SetPeriodicAlarm(OSAlarm *alarm, OSTick start, OSTick period, void (*handler)(void *), void *arg) {
    OSIntrMode e;
    if (alarm == NULL || alarm->handler != NULL) {
        OS_Terminate();
    }
    e = OS_DisableInterrupts();
    alarm->period = period;
    alarm->start = start;
    alarm->handler = handler;
    alarm->arg = arg;
    OSi_InsertAlarm(alarm, 0);
    OS_RestoreInterrupts(e);
}

void OS_CancelAlarm(OSAlarm *alarm) {
    OSIntrMode e = OS_DisableInterrupts();
    OSAlarm *next;
    OSAlarm *prev;
    if (alarm->handler == NULL) {
        OS_RestoreInterrupts(e);
        return;
    }
    next = alarm->next;
    if (next == NULL) {
        alarmQueue.tail = alarm->prev;
    } else {
        next->prev = alarm->prev;
    }
    prev = alarm->prev;
    if (prev != NULL) {
        prev->next = next;
    } else {
        alarmQueue.head = next;
        if (next != NULL) {
            OSi_ArrangeTimer(next);
        }
    }
    alarm->handler = NULL;
    alarm->period = 0;
    OS_RestoreInterrupts(e);
}

// Not compiled: saves r0 beside lr (`stmfd sp!, {r0, lr}`) and returns with `ldmfd sp!, {r0, lr}; bx lr`. In this
// build's compiled code r0 is pushed only in a variadic function's r0-r3 spill (lr goes in a second push); an `__irq`
// function also saves it, with every other scratch register the call may change (`push {r0, r1, r2, r3, ip, lr}`) and
// returns with `ldm sp!, {..., pc}^`; the plain call compiles to the tail call `ldr ip, =OSi_AlarmHandler; bx ip`.
asm void AlarmTimerEntry(void *arg) {
    stmfd sp!, {r0, lr}
    bl OSi_AlarmHandler
    ldmfd sp!, {r0, lr}
    bx lr
}

void OSi_AlarmHandler(void) {
    OSAlarm *alarm;
    void (*handler)(void *);
    OSTick tick;
    *(vu16 *) 0x4000106 = 0;
    OS_DisableIrqMask(0x10);
    *(vu32 *) (HW_DTCM + 0x3ff8) |= 0x10;
    tick = OS_GetTick();
    alarm = alarmQueue.head;
    if (alarm == NULL) {
        return;
    }
    if (tick < alarm->fire) {
        OSi_ArrangeTimer(alarm);
        return;
    }
    alarmQueue.head = alarm->next;
    if (alarmQueue.head == NULL) {
        alarmQueue.tail = NULL;
    } else {
        alarmQueue.head->prev = NULL;
    }
    handler = alarm->handler;
    if (alarm->period == 0) {
        alarm->handler = NULL;
    }
    if (handler != NULL) {
        handler(alarm->arg);
    }
    if (alarm->period != 0) {
        alarm->handler = handler;
        OSi_InsertAlarm(alarm, 0);
    }
    if (alarmQueue.head != NULL) {
        OSi_ArrangeTimer(alarmQueue.head);
    }
}
