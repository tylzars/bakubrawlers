#pragma once
#include <nitro/private/sdk.h>

typedef struct OSThread OSThread;
typedef struct OSMutex OSMutex;
typedef struct OSAlarm OSAlarm;

typedef struct { OSThread *head; OSThread *tail; } OSThreadQueue;
typedef struct { OSMutex *head; OSMutex *tail; } OSMutexQueue;
typedef struct { OSThread *prev; OSThread *next; } OSThreadLink;
typedef struct { OSMutex *next; OSMutex *prev; } OSMutexLink;

typedef struct {
    u32 cpsr;        // 0x00
    u32 r[13];       // 0x04
    u32 sp;          // 0x38
    u32 lr;          // 0x3c
    u32 pc_plus4;    // 0x40
    u32 sp_svc;      // 0x44
    u32 cp_context[7]; // 0x48
} OSContext;         // 0x64

enum { OS_THREAD_STATE_WAITING = 0, OS_THREAD_STATE_READY = 1, OS_THREAD_STATE_TERMINATED = 2 };

struct OSThread {
    OSContext context;      // 0x00
    u32 state;              // 0x64
    OSThread *next;         // 0x68
    u32 id;                 // 0x6c
    u32 priority;           // 0x70
    void *profiler;         // 0x74
    OSThreadQueue *queue;   // 0x78
    OSThreadLink link;      // 0x7c
    OSMutex *mutex;         // 0x84
    OSMutexQueue mutexQueue; // 0x88
    u32 stackTop;           // 0x90
    u32 stackBottom;        // 0x94
    u32 stackWarningOffset; // 0x98
    OSThreadQueue joinQueue; // 0x9c
    void *specific[3];      // 0xa4
    OSAlarm *alarmForSleep; // 0xb0
    void (*destructor)(void *); // 0xb4
    void *userParameter;    // 0xb8
    int systemErrno;        // 0xbc
};

struct OSMutex {
    OSThreadQueue queue;    // 0x00
    OSThread *thread;       // 0x08
    s32 count;              // 0x0c
    OSMutexLink link;       // 0x10
};

typedef struct {
    OSThreadQueue queueSend;    // 0x00
    OSThreadQueue queueReceive; // 0x08
    void **msgArray;            // 0x10
    s32 msgCount;               // 0x14
    s32 firstIndex;             // 0x18
    s32 usedCount;              // 0x1c
} OSMessageQueue;

#define OS_MESSAGE_NOBLOCK 0
#define OS_MESSAGE_BLOCK 1

struct OSAlarm {
    void (*handler)(void *);  // 0x00
    void *arg;                // 0x04
    u32 tag;                  // 0x08
    u64 fire;                 // 0x0c
    OSAlarm *prev;            // 0x14
    OSAlarm *next;            // 0x18
    u64 period;               // 0x1c
    u64 start;                // 0x24
};                            // 0x2c

// thread system state at 0x020bdcc4; the nested OSThreadInfo starts at 0x020bdce8
typedef struct {
    u16 isNeedRescheduling;                       // 0x24
    u16 irqDepth;                                 // 0x26
    OSThread *current;                            // 0x28
    OSThread *list;                               // 0x2c
    void (*switchCallback)(OSThread *, OSThread *); // 0x30
    OSThread idleThread;                          // 0x34
    OSThread mainThread;                          // 0xf4
    u8 idleStack[0xc8];                           // 0x1b4
} OSThreadInfo;

typedef struct {
    void (*switchHook)(OSThread *, OSThread *);   // 0x00
    u32 schedulerLock;                            // 0x04
    OSThread **pCurrent;                          // 0x08
    u32 initialized;                              // 0x0c
    u32 unk10, unk14, unk18;
    u32 exitStack;                                // 0x1c
    u32 lastThreadId;                             // 0x20
    OSThreadInfo ti;                              // 0x24
} OSThreadSys;

extern OSThreadSys threadSys;   // 0x020bdcc4

extern OSIntrMode OS_DisableInterrupts(void);
extern OSIntrMode OS_RestoreInterrupts(OSIntrMode);
extern void OS_SleepThread(OSThreadQueue *queue);
extern void OS_WakeupThread(OSThreadQueue *queue);

// Same memory as threadSys.ti (0x020bdce8); some functions address it through this symbol
extern OSThreadInfo OSi_ThreadInfo;
